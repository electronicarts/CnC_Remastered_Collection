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

/* $Header: /CounterStrike/RULES.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RULES.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/12/96                                                     *
 *                                                                                             *
 *                  Last Update : September 10, 1996 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Difficulty_Get -- Fetch the difficulty bias values.                                       *
 *   RulesClass::AI -- Processes the AI control constants from the database.                   *
 *   RulesClass::General -- Process the general main game rules.                               *
 *   RulesClass::Heap_Maximums -- Fetch and process the heap override values.                  *
 *   RulesClass::IQ -- Fetches the IQ control values from the INI database.                    *
 *   RulesClass::Land_Types -- Inits the land type values.                                     *
 *   RulesClass::MPlayer -- Fetch and process the multiplayer default settings.                *
 *   RulesClass::Powerups -- Process the powerup values from the database.                     *
 *   RulesClass::Process -- Fetch the bulk of the rule data from the control file.             *
 *   RulesClass::Recharge -- Process the super weapon recharge statistics.                     *
 *   RulesClass::RulesClass -- Default constructor for rules class object.                     *
 *   RulesClass::Themes -- Fetches the theme control values from the INI database.             *
 *   Techno_Get -- Get rule data common for all techno type objects.                           *
 *   _Scale_To_256 -- Scales a 1..100 number into a 1..255 number.                             *
 *   RulesClass::Difficulty -- Fetch the various difficulty group settings.                    *
 *   RulesClass::Objects -- Fetch all the object characteristic values.                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"
#include	"vortex.h"


/***********************************************************************************************
 * _Scale_To_256 -- Scales a 1..100 number into a 1..255 number.                               *
 *                                                                                             *
 *    This is a helper routine that will take a decimal percentage number and convert it       *
 *    into a game based fixed point number.                                                    *
 *                                                                                             *
 * INPUT:   val   -- Decimal percent number to convert.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the decimal percent number converted to a game fixed point number.    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static inline int _Scale_To_256(int val)
{
	val = fixed(100, 256) * val;
	val = min(val, 255);
	return(val);
}


/***********************************************************************************************
 * RulesClass::RulesClass -- Default constructor for rules class object.                       *
 *                                                                                             *
 *    This is the default constructor for the rules class object. Although it initializes the  *
 *    rule data with default values, it is expected that they will all be overridden by the    *
 *    rules control file.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
RulesClass::RulesClass(void) :
	TurboBoost("1.5"),
	AttackInterval(3),
	AttackDelay(5),
	PowerEmergencyFraction(fixed::_3_4),
	BadgerBombCount(1),
	AirstripRatio(".12"),
	AirstripLimit(5),
	HelipadRatio(".12"),
	HelipadLimit(5),
	TeslaRatio(".16"),
	TeslaLimit(10),
	AARatio(".14"),
	AALimit(10),
	DefenseRatio(".5"),
	DefenseLimit(40),
	WarRatio(".1"),
	WarLimit(2),
	BarracksRatio(".16"),
	BarracksLimit(2),
	RefineryLimit(4),
	RefineryRatio(".16"),
	BaseSizeAdd(3),
	PowerSurplus(50),
	InfantryReserve(2000),
	InfantryBaseMult(2),
	ChronoDuration(3),
	WaterCrateChance(".2"),
	SoloCrateMoney(2000),
	GPSTechLevel(0),
	UnitCrateType(UNIT_NONE),
	PatrolTime(".016"),
	TeamDelay(".6"),
	CloakDelay(0),
	GameSpeedBias(1),
	NervousBias(1),
	VortexRange(10*CELL_LEPTON_W),
	VortexSpeed((MPHType)10),
	VortexDamage(200),
	VortexChance(".2"),
	ExplosionSpread(fixed::_1_2),
	SupressRadius(CELL_LEPTON_W),
	ParaInfantryTechLevel(10),
	SpyPlaneTechLevel(10),
	ParaBombTechLevel(10),
	MaxIQ(5),
	IQSuperWeapons(4),
	IQProduction(5),
	IQGuardArea(4),
	IQRepairSell(3),
	IQCrush(2),
	IQScatter(3),
	IQContentScan(4),
	IQAircraft(4),
	IQHarvester(3),
	IQSellBack(2),
	SilverCrate(CRATE_HEAL_BASE),
	WoodCrate(CRATE_MONEY),
	WaterCrate(CRATE_MONEY),
	CrateMinimum(1),
	CrateMaximum(255),
	LZScanRadius(16*CELL_LEPTON_W),
	MPDefaultMoney(3000),
	MPMaxMoney(10000),
	IsMPShadowGrow(true),
	IsMPBasesOn(true),
	IsMPTiberiumGrow(true),
	IsMPCrates(true),
	IsMPAIPlayers(false),
	IsMPCaptureTheFlag(false),
	DropZoneRadius(4*CELL_LEPTON_W),
	MessageDelay(".6"),
	SavourDelay(".03"),
	AVMineDamage(1200),
	APMineDamage(1000),
	MaxPlayers(8),
	BaseDefenseDelay(fixed::_1_4),
	SuspendPriority(20),
	SuspendDelay(2),
	SurvivorFraction(fixed::_1_2),
	ReloadRate(".05"),
	AutocreateTime(5),
	BuildupTime(".05"),
	OreDumpRate(2),
	AtomDamage(1000),
	IsComputerParanoid(true),
	IsCurleyShuffle(false),
	IsFlashLowPower(true),
	IsCompEasyBonus(true),
	IsFineDifficulty(false),
	IsExplosiveHarvester(false),
	IsMCVDeploy(false),
	IsAllyReveal(true),
	IsSeparate(false),
	IsTreeTarget(false),
	IsMineAware(true),
	IsTGrowth(true),
	IsTSpread(true),
	IsNamed(false),
	IsAutoCrush(false),
	IsSmartDefense(false),
	IsScatter(false),
	IsChronoKill(true),
	ProneDamageBias(fixed::_1_2),
	QuakeDamagePercent(".33"),
	QuakeChance(".2"),
	GrowthRate(2),
	ShroudRate(4),
	CrateTime(10),
	TimerWarning(2),
	ChronoTechLevel(1),
	SonarTime(14),
	ChronoTime(3),
	ParaBombTime(14),
	ParaInfantryTime(2),
	ParaSaboteurTime(14),
	SpyTime(2),
	IronCurtainTime(14),
	GPSTime(1),
	NukeTime(14),
	SpeakDelay(2),
	DamageDelay(1),
	Gravity(3),
	GapShroudRadius(10),
	GapRegenInterval(".1"),
	RadarJamRadius(10*CELL_LEPTON_W),
	Incoming(MPH_IMMOBILE),
	MinDamage(1),
	MaxDamage(1000),
	RepairStep(5),
	RepairPercent(fixed::_1_4),
	URepairStep(5),
	URepairPercent(fixed::_1_4),
	RepairRate(".016"),
	ConditionGreen(1),
	ConditionYellow(fixed::_1_2),
	ConditionRed(fixed::_1_4),
	RandomAnimateTime(".083"),
	BailCount(28),
	GoldValue(35),
	GemValue(110),
	AircraftMax(100),
	AnimMax(100),
	BuildingMax(500),
	BulletMax(40),
	FactoryMax(20),
	InfantryMax(500),
	OverlayMax(1),
	SmudgeMax(1),
	TeamMax(60),
	TeamTypeMax(60),
	TemplateMax(1),
	TerrainMax(500),
	TriggerMax(60),
	UnitMax(500),
	VesselMax(100),
	ProjectileMax(20),
	WeaponMax(20),
	WarheadMax(20),
	TrigTypeMax(80),
	CloseEnoughDistance(0x0280),
	StrayDistance(0x0200),
	CrushDistance(0x0180),
	CrateRadius(0x0280),
	HomingScatter(0x0200),
	BallisticScatter(0x0100),
	RefundPercent(fixed::_1_2),
	IronCurtainDuration(fixed::_1_2),
	BridgeStrength(1000),
	BuildSpeedBias(1),
	C4Delay(".03"),
	RepairThreshhold(1000),
	PathDelay(".016"),
	MovieTime(fixed::_1_4),
	TiberiumShortScan(0x0600),
	TiberiumLongScan(0x2000),
	HealthBarDisplayMode(HB_SELECTED),
	ResourceBarDisplayMode(RB_SELECTED)
{
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	NewUnitsEnabled = SecretUnitsEnabled = 0;
	MTankDistance = 30;
	QuakeUnitDamage = 0x080;
	QuakeBuildingDamage = 0x040;
	QuakeInfantryDamage = 0;
	QuakeDelay      = 120;
	ChronoTankDuration = 0x300;
#ifdef FIXIT_ENGINEER	//	checked - ajw 9/28/98
	EngineerDamage=(fixed)1 / (fixed)3;	// Amount of damage an engineer does
	EngineerCaptureLevel=ConditionRed;	// Building damage level before engineer can capture
#endif
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	CarrierLaunchDelay = 60;
#endif
#endif
}


/***********************************************************************************************
 * Difficulty_Get -- Fetch the difficulty bias values.                                         *
 *                                                                                             *
 *    This will fetch the difficulty bias values for the section specified.                    *
 *                                                                                             *
 * INPUT:   ini   -- Reference the INI database to fetch the values from.                      *
 *                                                                                             *
 *          diff  -- Reference to the difficulty class object to fill in with the values.      *
 *                                                                                             *
 *          section  -- The section identifier to lift the values from.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static void Difficulty_Get(CCINIClass & ini, DifficultyClass & diff, char const * section)
{
	if (ini.Is_Present(section)) {
		diff.FirepowerBias = ini.Get_Fixed(section, "FirePower", 1);
		diff.GroundspeedBias = ini.Get_Fixed(section, "Groundspeed", 1);
		diff.AirspeedBias = ini.Get_Fixed(section, "Airspeed", 1);
		diff.ArmorBias = ini.Get_Fixed(section, "Armor", 1);
		diff.ROFBias = ini.Get_Fixed(section, "ROF", 1);
		diff.CostBias = ini.Get_Fixed(section, "Cost", 1);
		diff.RepairDelay = ini.Get_Fixed(section, "RepairDelay", ".02");
		diff.BuildDelay = ini.Get_Fixed(section, "BuildDelay", ".03");
		diff.IsBuildSlowdown = ini.Get_Bool(section, "BuildSlowdown", false);
		diff.BuildSpeedBias = ini.Get_Fixed(section, "BuildTime", 1);
		diff.IsWallDestroyer = ini.Get_Bool(section, "DestroyWalls", true);
		diff.IsContentScan = ini.Get_Bool(section, "ContentScan", false);
	}
}


/***********************************************************************************************
 * RulesClass::Process -- Fetch the bulk of the rule data from the control file.               *
 *                                                                                             *
 *    This routine will fetch the rule data from the control file.                             *
 *                                                                                             *
 * INPUT:   file  -- Reference to the rule file to process.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the rule file processed?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Process(CCINIClass & ini)
{
	BStart(BENCH_RULES);

	General(ini);
	MPlayer(ini);
	Recharge(ini);
	Heap_Maximums(ini);
	AI(ini);
	Powerups(ini);
	Land_Types(ini);
	Themes(ini);
	IQ(ini);
	Objects(ini);
	Difficulty(ini);

	BEnd(BENCH_RULES);

	return(true);
}


/***********************************************************************************************
 * RulesClass::General -- Process the general main game rules.                                 *
 *                                                                                             *
 *    This fetches the control constants uses for regular game processing. Any game behavior   *
 *    controlling values that don't properly fit in any of the other catagories will be        *
 *    stored here.                                                                             *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the database to fetch the values from.                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the general section found and processed?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::General(CCINIClass & ini)
{
	static char const * const GENERAL = "General";
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	static char const * const AFTERMATH = "Aftermath";

	if(ini.Is_Present(AFTERMATH)) {
//debugprint( "NewUnitsEnabled previously %i\n", NewUnitsEnabled );
		NewUnitsEnabled = ini.Get_Int(AFTERMATH, "NewUnitsEnabled", 0);
//debugprint( "NewUnitsEnabled set to %i by Rules\n", NewUnitsEnabled );
		MTankDistance = ini.Get_Int(AFTERMATH,"MTankDistance",MTankDistance);
		QuakeUnitDamage = ini.Get_Fixed(AFTERMATH, "QuakeUnitDamage", QuakeUnitDamage);
		QuakeBuildingDamage = ini.Get_Fixed(AFTERMATH, "QuakeBuildingDamage", QuakeBuildingDamage);
		QuakeInfantryDamage = ini.Get_Int(AFTERMATH,"QuakeInfantryDamage",QuakeInfantryDamage);
		QuakeDelay      = ini.Get_Int(AFTERMATH,"QuakeDelay",QuakeDelay);
		ChronoTankDuration = ini.Get_Fixed(AFTERMATH, "ChronoTankDuration", ChronoTankDuration);
//Mono_Set_Cursor(0,0);Mono_Printf("Chrono duration: %08x \n",ChronoTankDuration);Keyboard->Get();Keyboard->Get();
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
		CarrierLaunchDelay = ini.Get_Int(AFTERMATH,"CarrierLaunchDelay",120);
#endif
#ifdef FIXIT_ENGINEER	//	checked - ajw 9/28/98
		//	Engineer changing fields were specifically left out of Aftrmath.ini, thus these values are not found to set. ajw
		//	Implies interesting security hole if user creates a separate Aftrmath.ini file!
		EngineerDamage = ini.Get_Fixed(AFTERMATH, "EngineerDamage", EngineerDamage);	// Amount of damage an engineer does
		EngineerCaptureLevel = ini.Get_Fixed(AFTERMATH, "EngineerCaptureLevel", EngineerCaptureLevel);	// Building damage level before engineer can capture
#endif
	}

#endif

	if (ini.Is_Present(GENERAL)) {
		TurboBoost = ini.Get_Fixed(GENERAL, "TurboBoost", TurboBoost);
		BadgerBombCount = ini.Get_Int(GENERAL, "BadgerBombCount", BadgerBombCount);
		IsCurleyShuffle = ini.Get_Bool(GENERAL, "CurleyShuffle", IsCurleyShuffle);
		IsFlashLowPower = ini.Get_Bool(GENERAL, "FlashLowPower", IsFlashLowPower);
		IsChronoKill = ini.Get_Bool(GENERAL, "ChronoKillCargo", IsChronoKill);
		ChronoDuration = ini.Get_Fixed(GENERAL, "ChronoDuration", ChronoDuration);
		IsFineDifficulty = ini.Get_Bool(GENERAL, "FineDiffControl", IsFineDifficulty);
		WaterCrateChance = ini.Get_Fixed(GENERAL, "WaterCrateChance", WaterCrateChance);
		SoloCrateMoney = ini.Get_Int(GENERAL, "SoloCrateMoney", SoloCrateMoney);
		ParaBombTechLevel = ini.Get_Int(GENERAL, "ParabombTech", ParaBombTechLevel);
		GPSTechLevel = ini.Get_Int(GENERAL, "GPSTechLevel", GPSTechLevel);
		UnitCrateType = ini.Get_UnitType(GENERAL, "UnitCrateType", UnitCrateType);
		IsExplosiveHarvester = ini.Get_Fixed(GENERAL, "OreExplosive", IsExplosiveHarvester);
		GapRegenInterval = ini.Get_Fixed(GENERAL, "GapRegenInterval", GapRegenInterval);
		TeamDelay = ini.Get_Fixed(GENERAL, "TeamDelay", TeamDelay);
		CloakDelay = ini.Get_Fixed(GENERAL, "SubmergeDelay", CloakDelay);
		GameSpeedBias = ini.Get_Fixed(GENERAL, "GameSpeedBias", GameSpeedBias);
		NervousBias = ini.Get_Fixed(GENERAL, "BaseBias", NervousBias);
		ExplosionSpread = ini.Get_Fixed(GENERAL, "ExpSpread", ExplosionSpread);
		SupressRadius = ini.Get_Lepton(GENERAL, "FireSupress", SupressRadius);
		ParaInfantryTechLevel = ini.Get_Int(GENERAL, "ParaTech", ParaInfantryTechLevel);
		SpyPlaneTechLevel = ini.Get_Int(GENERAL, "SpyPlaneTech", SpyPlaneTechLevel);
		SilverCrate = ini.Get_CrateType(GENERAL, "SilverCrate", SilverCrate);
		WoodCrate = ini.Get_CrateType(GENERAL, "WoodCrate", WoodCrate);
		WaterCrate = ini.Get_CrateType(GENERAL, "WaterCrate", WaterCrate);
		CrateMinimum = ini.Get_Int(GENERAL, "CrateMinimum", CrateMinimum);
		CrateMaximum = ini.Get_Int(GENERAL, "CrateMaximum", CrateMaximum);
		IsScatter = ini.Get_Bool(GENERAL, "PlayerScatter", IsScatter);
		IsSmartDefense = ini.Get_Bool(GENERAL, "PlayerReturnFire", IsSmartDefense);
		IsAutoCrush = ini.Get_Bool(GENERAL, "PlayerAutoCrush", IsAutoCrush);
		IsNamed = ini.Get_Bool(GENERAL, "NamedCivilians", IsNamed);
		IsTGrowth = ini.Get_Bool(GENERAL, "OreGrows", IsTGrowth);
		IsTSpread = ini.Get_Bool(GENERAL, "OreSpreads", IsTSpread);
		IsMineAware = ini.Get_Bool(GENERAL, "MineAware", IsMineAware);
		IsTreeTarget = ini.Get_Bool(GENERAL, "TreeTargeting", IsTreeTarget);
		IsSeparate = ini.Get_Bool(GENERAL, "SeparateAircraft", IsSeparate);
		DropZoneRadius = ini.Get_Lepton(GENERAL, "DropZoneRadius", DropZoneRadius);
		MessageDelay = ini.Get_Fixed(GENERAL, "MessageDelay", MessageDelay);
		SavourDelay = ini.Get_Fixed(GENERAL, "SavourDelay", SavourDelay);
		AVMineDamage = ini.Get_Int(GENERAL, "AVMineDamage", AVMineDamage);
		APMineDamage = ini.Get_Int(GENERAL, "APMineDamage", APMineDamage);
		BaseDefenseDelay = ini.Get_Fixed(GENERAL, "BaseDefenseDelay", BaseDefenseDelay);
		SuspendPriority = ini.Get_Int(GENERAL, "SuspendPriority", SuspendPriority);
		SuspendDelay = ini.Get_Fixed(GENERAL, "SuspendDelay", SuspendDelay);
		SurvivorFraction = ini.Get_Fixed(GENERAL, "SurvivorRate", SurvivorFraction);
		RadarJamRadius = ini.Get_Lepton(GENERAL, "RadarJamRadius", RadarJamRadius);
		ReloadRate = ini.Get_Fixed(GENERAL, "ReloadRate", ReloadRate);
		RandomAnimateTime = ini.Get_Fixed(GENERAL, "IdleActionFrequency", RandomAnimateTime);
		BuildupTime = ini.Get_Fixed(GENERAL, "BuildupTime", BuildupTime);
		OreDumpRate = ini.Get_Int(GENERAL, "OreTruckRate", OreDumpRate);
		AtomDamage = ini.Get_Int(GENERAL, "AtomDamage", AtomDamage);
		BailCount = ini.Get_Int(GENERAL, "BailCount", BailCount);
		BallisticScatter = ini.Get_Lepton(GENERAL, "BallisticScatter", BallisticScatter);
		BridgeStrength = ini.Get_Int(GENERAL, "BridgeStrength", BridgeStrength);
		BuildSpeedBias = ini.Get_Fixed(GENERAL, "BuildSpeed", BuildSpeedBias);
		ConditionGreen = 1;
		ConditionRed = ini.Get_Fixed(GENERAL, "ConditionRed", ConditionRed);
		ConditionYellow = ini.Get_Fixed(GENERAL, "ConditionYellow", ConditionYellow);
		CrateRadius = ini.Get_Lepton(GENERAL, "CrateRadius", CrateRadius);
		CrushDistance = ini.Get_Lepton(GENERAL, "Crush", CrushDistance);
		DamageDelay = ini.Get_Fixed(GENERAL, "DamageDelay", DamageDelay);
		GapShroudRadius = ini.Get_Int(GENERAL, "GapRadius", GapShroudRadius);
		GemValue = ini.Get_Int(GENERAL, "GemValue", GemValue);
		GoldValue = ini.Get_Int(GENERAL, "GoldValue", GoldValue);
		Gravity = ini.Get_Int(GENERAL, "Gravity", Gravity);
		GrowthRate = ini.Get_Fixed(GENERAL, "GrowthRate", GrowthRate);
		HomingScatter = ini.Get_Lepton(GENERAL, "HomingScatter", HomingScatter);
		Incoming = ini.Get_MPHType(GENERAL, "Incoming", MPH_IMMOBILE);
		IronCurtainDuration = ini.Get_Fixed(GENERAL, "IronCurtain", IronCurtainDuration);
		IsAllyReveal = ini.Get_Bool(GENERAL, "AllyReveal", IsAllyReveal);
		IsMCVDeploy = ini.Get_Bool(GENERAL, "MCVUndeploy", IsMCVDeploy);
		MaxDamage = ini.Get_Int(GENERAL, "MaxDamage", MaxDamage);
		MinDamage = ini.Get_Int(GENERAL, "MinDamage", MinDamage);
		ProneDamageBias = ini.Get_Fixed(GENERAL, "ProneDamage", ProneDamageBias);
		QuakeDamagePercent = ini.Get_Fixed(GENERAL, "QuakeDamage", QuakeDamagePercent);
		QuakeChance = ini.Get_Fixed(GENERAL, "QuakeChance", QuakeChance);
		RefundPercent = ini.Get_Fixed(GENERAL, "RefundPercent", RefundPercent);
		RepairPercent = ini.Get_Fixed(GENERAL, "RepairPercent", RepairPercent);
		RepairStep = ini.Get_Int(GENERAL, "RepairStep", RepairStep);
		URepairPercent = ini.Get_Fixed(GENERAL, "URepairPercent", URepairPercent);
		URepairStep = ini.Get_Int(GENERAL, "URepairStep", URepairStep);
		RepairRate = ini.Get_Fixed(GENERAL, "RepairRate", RepairRate);
		ShroudRate = ini.Get_Fixed(GENERAL, "ShroudRate", ShroudRate);
		SpeakDelay = ini.Get_Fixed(GENERAL, "SpeakDelay", SpeakDelay);
		StrayDistance = ini.Get_Lepton(GENERAL, "Stray", StrayDistance);
		CloseEnoughDistance = ini.Get_Lepton(GENERAL, "CloseEnough", CloseEnoughDistance);
		TimerWarning = ini.Get_Fixed(GENERAL, "TimerWarning", TimerWarning);
		MovieTime = ini.Get_Fixed(GENERAL, "MovieTime", MovieTime);
		C4Delay = ini.Get_Fixed(GENERAL, "C4Delay", C4Delay);
		ChronoTechLevel = ini.Get_Int(GENERAL, "ChronoTechLevel", ChronoTechLevel);
		CrateTime = ini.Get_Fixed(GENERAL, "CrateRegen", CrateTime);
		VortexRange = ini.Get_Lepton(GENERAL, "VortexRange", VortexRange);
		VortexSpeed = MPHType(_Scale_To_256(ini.Get_Int(GENERAL, "VortexSpeed", VortexSpeed)));
		VortexDamage = ini.Get_Int(GENERAL, "VortexDamage", VortexDamage);
		VortexChance = ini.Get_Fixed(GENERAL, "VortexChance", VortexChance);

		ChronalVortex.Set_Range(VortexRange / CELL_LEPTON_W);
		ChronalVortex.Set_Speed(VortexSpeed);
		ChronalVortex.Set_Damage(VortexDamage);

		//ChronalVortex.Set_Range ( ini.Get_Int (GENERAL, "VortexRange", ChronalVortex.Get_Range() ) );
		//ChronalVortex.Set_Speed ( ini.Get_Int (GENERAL, "VortexSpeed", ChronalVortex.Get_Speed() ) );
		//ChronalVortex.Set_Damage ( ini.Get_Int (GENERAL, "VortexDamage", ChronalVortex.Get_Damage() ) );

		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * RulesClass::MPlayer -- Fetch and process the multiplayer default settings.                  *
 *                                                                                             *
 *    This is used to set the default settings for the multiplayer system.                     *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database.                                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the multiplayer default override section found and processed?            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::MPlayer(CCINIClass & ini)
{
	static char const * const MPLAYER = "MultiplayerDefaults";
	if (ini.Is_Present(MPLAYER)) {
		MPDefaultMoney = ini.Get_Int(MPLAYER, "Money", MPDefaultMoney);
		MPMaxMoney = ini.Get_Int(MPLAYER, "MaxMoney", MPMaxMoney);
		IsMPShadowGrow = ini.Get_Bool(MPLAYER, "ShadowGrow", IsMPShadowGrow);
		IsMPBasesOn = ini.Get_Bool(MPLAYER, "Bases", IsMPBasesOn);
		IsMPTiberiumGrow = ini.Get_Bool(MPLAYER, "OreGrows", IsMPTiberiumGrow);
		IsMPCrates = ini.Get_Bool(MPLAYER, "Crates", IsMPCrates);
		IsMPCaptureTheFlag = ini.Get_Bool(MPLAYER, "CaptureTheFlag", IsMPCaptureTheFlag);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * RulesClass::Recharge -- Process the super weapon recharge statistics.                       *
 *                                                                                             *
 *    Use this to set the recharge times for the various super weapons available.              *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the database.                                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the recharge section found and processed?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Recharge(CCINIClass & ini)
{
	static char const * const RECHARGE = "Recharge";
	if (ini.Is_Present(RECHARGE)) {
		SonarTime = ini.Get_Fixed(RECHARGE, "Sonar", SonarTime);
		ChronoTime = ini.Get_Fixed(RECHARGE, "Chrono", ChronoTime);
		ParaBombTime = ini.Get_Fixed(RECHARGE, "ParaBomb", ParaBombTime);
		ParaInfantryTime = ini.Get_Fixed(RECHARGE, "Paratrooper", ParaInfantryTime);
		SpyTime = ini.Get_Fixed(RECHARGE, "SpyPlane", SpyTime);
		IronCurtainTime = ini.Get_Fixed(RECHARGE, "IronCurtain", IronCurtainTime);
		GPSTime = ini.Get_Fixed(RECHARGE, "GPS", GPSTime);
		NukeTime = ini.Get_Fixed(RECHARGE, "Nuke", NukeTime);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * RulesClass::Heap_Maximums -- Fetch and process the heap override values.                    *
 *                                                                                             *
 *    This fetches the maximum heap sizes from the database specified. The heaps will be       *
 *    initialized by this routine as indicated.                                                *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database.                                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the maximum section found and processed?                                 *
 *                                                                                             *
 * WARNINGS:   This process is catastrophic to any data currently existing in the heaps        *
 *             modified. This should only be processed during the game initialization stage.   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Heap_Maximums(CCINIClass & ini)
{
	/*
	**	Heap maximum values.
	*/
	static char const * const MAXIMUMS = "Maximums";
	if (ini.Is_Present(MAXIMUMS)) {
		MaxPlayers = ini.Get_Int(MAXIMUMS, "Players", MaxPlayers);
		AircraftMax = ini.Get_Int(MAXIMUMS, "Aircraft", AircraftMax);
		AnimMax = ini.Get_Int(MAXIMUMS, "Anim", AnimMax);
		BuildingMax = ini.Get_Int(MAXIMUMS, "Building", BuildingMax);
		BulletMax = ini.Get_Int(MAXIMUMS, "Bullet", BulletMax);
		FactoryMax = ini.Get_Int(MAXIMUMS, "Factory", FactoryMax);
		InfantryMax = ini.Get_Int(MAXIMUMS, "Infantry", InfantryMax);
		OverlayMax = ini.Get_Int(MAXIMUMS, "Overlay", OverlayMax);
		SmudgeMax = ini.Get_Int(MAXIMUMS, "Smudge", SmudgeMax);
		TeamMax = ini.Get_Int(MAXIMUMS, "Team", TeamMax);
		TeamTypeMax = ini.Get_Int(MAXIMUMS, "TeamType", TeamTypeMax);
		TemplateMax = ini.Get_Int(MAXIMUMS, "Template", TemplateMax);
		TerrainMax = ini.Get_Int(MAXIMUMS, "Terrain", TerrainMax);
		TriggerMax = ini.Get_Int(MAXIMUMS, "Trigger", TriggerMax);
		UnitMax = ini.Get_Int(MAXIMUMS, "Unit", UnitMax);
		VesselMax = ini.Get_Int(MAXIMUMS, "Vessel", VesselMax);
		ProjectileMax = ini.Get_Int(MAXIMUMS, "Projectile", ProjectileMax);
		WeaponMax = ini.Get_Int(MAXIMUMS, "Weapon", WeaponMax);
		WarheadMax = ini.Get_Int(MAXIMUMS, "Warhead", WarheadMax);
		TrigTypeMax = ini.Get_Int(MAXIMUMS, "TrigType", TrigTypeMax);
	}

	/*
	**	Special case: double maximum animations to accomodate lots of action
	*/
	AnimMax = max(AnimMax, 200);

	/*
	**	Any heaps that use the maximums that were just loaded, must
	**	be initialized as necessary.
	*/
	Warheads.Set_Heap(WarheadMax);
	new WarheadTypeClass("SA");
	new WarheadTypeClass("HE");
	new WarheadTypeClass("AP");
	new WarheadTypeClass("Fire");
	new WarheadTypeClass("HollowPoint");
	new WarheadTypeClass("Super");
	new WarheadTypeClass("Organic");
	new WarheadTypeClass("Nuke");
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	new WarheadTypeClass("Mechanical");
#endif

	Weapons.Set_Heap(WeaponMax);
	new WeaponTypeClass("Colt45");
	new WeaponTypeClass("ZSU-23");
	new WeaponTypeClass("Vulcan");
	new WeaponTypeClass("Maverick");
	new WeaponTypeClass("Camera");
	new WeaponTypeClass("FireballLauncher");
	new WeaponTypeClass("Sniper");
	new WeaponTypeClass("ChainGun");
	new WeaponTypeClass("Pistol");
	new WeaponTypeClass("M1Carbine");
	new WeaponTypeClass("Dragon");
	new WeaponTypeClass("Hellfire");
	new WeaponTypeClass("Grenade");
	new WeaponTypeClass("75mm");
	new WeaponTypeClass("90mm");
	new WeaponTypeClass("105mm");
	new WeaponTypeClass("120mm");
	new WeaponTypeClass("TurretGun");
	new WeaponTypeClass("MammothTusk");
	new WeaponTypeClass("155mm");
	new WeaponTypeClass("M60mg");
	new WeaponTypeClass("Napalm");
	new WeaponTypeClass("TeslaZap");
	new WeaponTypeClass("Nike");
	new WeaponTypeClass("8Inch");
	new WeaponTypeClass("Stinger");
	new WeaponTypeClass("TorpTube");
	new WeaponTypeClass("2Inch");
	new WeaponTypeClass("DepthCharge");
	new WeaponTypeClass("ParaBomb");
	new WeaponTypeClass("DogJaw");
	new WeaponTypeClass("Heal");
	new WeaponTypeClass("SCUD");
	new WeaponTypeClass("Flamer");
	new WeaponTypeClass("RedEye");

#ifdef FIXIT_ANTS
	new WeaponTypeClass("Mandible");
#endif

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	new WeaponTypeClass("PortaTesla");
	new WeaponTypeClass("GoodWrench");
	new WeaponTypeClass("SubSCUD");
	new WeaponTypeClass("TTankZap");
	new WeaponTypeClass("APTusk");
	new WeaponTypeClass("Democharge");
#endif
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	new WeaponTypeClass("AirAssault");
#endif

	return(true);
}


/***********************************************************************************************
 * RulesClass::AI -- Processes the AI control constants from the database.                     *
 *                                                                                             *
 *    This will examine the database specified and set the AI override values accordingly.     *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that holds the AI overrides.                *
 *                                                                                             *
 * OUTPUT:  bool; Was the AI section found and processed?                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::AI(CCINIClass & ini)
{
	static char const * const AI = "AI";
	if (ini.Is_Present(AI)) {
		AttackInterval = ini.Get_Fixed(AI, "AttackInterval", AttackInterval);
		AttackDelay = ini.Get_Fixed(AI, "AttackDelay", AttackDelay);
		PatrolTime = ini.Get_Fixed(AI, "PatrolScan", PatrolTime);
		RepairThreshhold = ini.Get_Int(AI, "CreditReserve", RepairThreshhold);
		PathDelay = ini.Get_Fixed(AI, "PathDelay", PathDelay);
		TiberiumShortScan = ini.Get_Lepton(AI, "OreNearScan", TiberiumShortScan);
		TiberiumLongScan = ini.Get_Lepton(AI, "OreFarScan", TiberiumLongScan);
		AutocreateTime = ini.Get_Fixed(AI, "AutocreateTime", AutocreateTime);
		InfantryReserve = ini.Get_Int(AI, "InfantryReserve", InfantryReserve);
		InfantryBaseMult = ini.Get_Int(AI, "InfantryBaseMult", InfantryBaseMult);
		PowerSurplus = ini.Get_Int(AI, "PowerSurplus", PowerSurplus);
		BaseSizeAdd = ini.Get_Int(AI, "BaseSizeAdd", BaseSizeAdd);
		RefineryRatio = ini.Get_Fixed(AI, "RefineryRatio", RefineryRatio);
		RefineryLimit = ini.Get_Int(AI, "RefineryLimit", RefineryLimit);
		BarracksRatio = ini.Get_Fixed(AI, "BarracksRatio", BarracksRatio);
		BarracksLimit = ini.Get_Int(AI, "BarracksLimit", BarracksLimit);
		WarRatio = ini.Get_Fixed(AI, "WarRatio", WarRatio);
		WarLimit = ini.Get_Int(AI, "WarLimit", WarLimit);
		DefenseRatio = ini.Get_Fixed(AI, "DefenseRatio", DefenseRatio);
		DefenseLimit = ini.Get_Int(AI, "DefenseLimit", DefenseLimit);
		AARatio = ini.Get_Fixed(AI, "AARatio", AARatio);
		AALimit = ini.Get_Int(AI, "AALimit", AALimit);
		TeslaRatio = ini.Get_Fixed(AI, "TeslaRatio", TeslaRatio);
		TeslaLimit = ini.Get_Int(AI, "TeslaLimit", TeslaLimit);
		HelipadRatio = ini.Get_Fixed(AI, "HelipadRatio", HelipadRatio);
		HelipadLimit = ini.Get_Int(AI, "HelipadLimit", HelipadLimit);
		AirstripRatio = ini.Get_Fixed(AI, "AirstripRatio", AirstripRatio);
		AirstripLimit = ini.Get_Int(AI, "AirstripLimit", AirstripLimit);
		IsCompEasyBonus = ini.Get_Bool(AI, "CompEasyBonus", IsCompEasyBonus);
		IsComputerParanoid = ini.Get_Bool(AI, "Paranoid", IsComputerParanoid);
		PowerEmergencyFraction = ini.Get_Fixed(AI, "PowerEmergency", PowerEmergencyFraction);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * RulesClass::Powerups -- Process the powerup values from the database.                       *
 *                                                                                             *
 *    This will examine the database and initialize the powerup override values accordingly.   *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database the the powerup values are to be            *
 *                   initialized from.                                                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the powerup section found and processed?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Powerups(CCINIClass & ini)
{
	static char const * const POWERUPS = "Powerups";
	if (ini.Is_Present(POWERUPS)) {
		for (CrateType crate = CRATE_FIRST; crate < CRATE_COUNT; crate++) {
			char buffer[128];
			if (ini.Get_String(POWERUPS, CrateNames[crate], "0,NONE", buffer, sizeof(buffer))) {

				/*
				**	Share odds.
				*/
				char * token = strtok(buffer, ",");
				if (token) {
					strtrim(token);
					CrateShares[crate] = atoi(token);
				}

				/*
				**	Animation to use.
				*/
				token = strtok(NULL, ",");
				if (token) {
					strtrim(token);
					CrateAnims[crate] = Anim_From_Name(token);
				}

				/*
				**	Optional data number.
				*/
				token = strtok(NULL, ",");
				if (token != NULL) {
					if (strpbrk(token, ".%") != NULL) {
						CrateData[crate] = fixed(token) * 256;
					} else {
						strtrim(token);
						CrateData[crate] = atoi(token);
					}
				}
			}
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * RulesClass::Land_Types -- Inits the land type values.                                       *
 *                                                                                             *
 *    This will set the land movement attributes from the database specified.                  *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the database that has the land value overrides.              *
 *                                                                                             *
 * OUTPUT:  bool; Was the land type sections found and processed?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Land_Types(CCINIClass & ini)
{
	/*
	**	Fetch the movement characteristic data for terrain types.
	*/
	for (LandType land = LAND_FIRST; land < LAND_COUNT; land++) {
		static char const * _lands[LAND_COUNT] = {
			"Clear",
			"Road",
			"Water",
			"Rock",
			"Wall",
			"Ore",
			"Beach",
			"Rough",
			"River"
		};

		GroundType * gptr = &Ground[land];

		if (ini.Is_Present(_lands[land])) {
			gptr->Cost[SPEED_FOOT] = ini.Get_Fixed(_lands[land], "Foot", 1);
			gptr->Cost[SPEED_TRACK] = ini.Get_Fixed(_lands[land], "Track", 1);
			gptr->Cost[SPEED_WHEEL] = ini.Get_Fixed(_lands[land], "Wheel", 1);
			gptr->Cost[SPEED_WINGED] = fixed(1);
			gptr->Cost[SPEED_FLOAT] = ini.Get_Fixed(_lands[land], "Float", 1);
			gptr->Build = ini.Get_Bool(_lands[land], "Buildable", false);
		}
	}
	return(true);
}


/***********************************************************************************************
 * RulesClass::Themes -- Fetches the theme control values from the INI database.               *
 *                                                                                             *
 *    The musical theme availability is controlled by the scenario and the player's house      *
 *    choice. These controls can be specified in the theme control section of the INI          *
 *    database.                                                                                *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to process.                                 *
 *                                                                                             *
 * OUTPUT:  bool; Was the theme section found and processed?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Themes(CCINIClass & ini)
{
	static char const * const THEMECONTROL = "ThemeControl";

	if (ini.Is_Present(THEMECONTROL)) {
		for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; theme++) {
			if (ini.Is_Present(THEMECONTROL, Theme.Base_Name(theme))) {

				char buffer[128];
				int scen = 1;
				int owners = HOUSEF_ALLIES | HOUSEF_SOVIET | HOUSEF_OTHERS;

				ini.Get_String(THEMECONTROL, Theme.Base_Name(theme), "", buffer, sizeof(buffer));
				char const * token = strtok(buffer, ",");
				if (token != NULL) {
					scen = atoi(token);
				}

				token = strtok(NULL, ",");
				if (token != NULL) {
					owners = Owner_From_Name(token);
				}

				Theme.Set_Theme_Data(theme, scen, owners);
			}
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * RulesClass::IQ -- Fetches the IQ control values from the INI database.                      *
 *                                                                                             *
 *    This will scan the database specified and retrieve the IQ control values from it. These  *
 *    IQ control values are what gives the IQ rating meaning. It fundimentally controls how    *
 *    the computer behaves.                                                                    *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to read the IQ controls from.               *
 *                                                                                             *
 * OUTPUT:  bool; Was the IQ section found and processed?                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::IQ(CCINIClass & ini)
{
	static char const * const IQCONTROL = "IQ";
	if (ini.Is_Present(IQCONTROL)) {
		MaxIQ = ini.Get_Int(IQCONTROL, "MaxIQLevels", MaxIQ);
		IQSuperWeapons = ini.Get_Int(IQCONTROL, "SuperWeapons", IQSuperWeapons);
		IQProduction = ini.Get_Int(IQCONTROL, "Production", IQProduction);
		IQGuardArea = ini.Get_Int(IQCONTROL, "GuardArea", IQGuardArea);
		IQRepairSell = ini.Get_Int(IQCONTROL, "RepairSell", IQRepairSell);
		IQCrush = ini.Get_Int(IQCONTROL, "AutoCrush", IQCrush);
		IQScatter = ini.Get_Int(IQCONTROL, "Scatter", IQScatter);
		IQContentScan = ini.Get_Int(IQCONTROL, "ContentScan", IQContentScan);
		IQAircraft = ini.Get_Int(IQCONTROL, "Aircraft", IQAircraft);
		IQHarvester = ini.Get_Int(IQCONTROL, "Harvester", IQHarvester);
		IQSellBack = ini.Get_Int(IQCONTROL, "SellBack", IQSellBack);

		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * RulesClass::Objects -- Fetch all the object characteristic values.                          *
 *                                                                                             *
 *    This will parse the specified INI database and fetch all the object characteristic       *
 *    values specified therein.                                                                *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the ini database to scan.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Objects(CCINIClass & ini)
{
	/*
	**	Fetch the game object values from the rules file.
	*/
	for (int index = 0; index < Warheads.Count(); index++) {
		Warheads.Ptr(index)->Read_INI(ini);
	}

	for (int proj = 0; proj < BulletTypes.Count(); proj++) {
		BulletTypes.Ptr(proj)->Read_INI(ini);
	}

	for (int windex = 0; windex < Weapons.Count(); windex++) {
		Weapons.Ptr(windex)->Read_INI(ini);
	}

	for (int uindex = 0; uindex < UnitTypes.Count(); uindex++) {
		UnitTypes.Ptr(uindex)->Read_INI(ini);
	}

	for (int iindex = 0; iindex < InfantryTypes.Count(); iindex++) {
		InfantryTypes.Ptr(iindex)->Read_INI(ini);
	}

	for (int vindex = 0; vindex < VesselTypes.Count(); vindex++) {
		VesselTypes.Ptr(vindex)->Read_INI(ini);
	}

	for (int aindex = 0; aindex < AircraftTypes.Count(); aindex++) {
		AircraftTypes.Ptr(aindex)->Read_INI(ini);
	}

	for (int bindex = 0; bindex < BuildingTypes.Count(); bindex++) {
		BuildingTypes.Ptr(bindex)->Read_INI(ini);
	}

	/*
	**	Fetch the house attribute override values.
	*/
	for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		HouseTypeClass::As_Reference(house).Read_INI(ini);
	}

	/*
	**	Fetch the mission control values.
	*/
	for (MissionType mission = MISSION_FIRST; mission < MISSION_COUNT; mission++) {
		MissionControlClass * miss = &MissionControl[mission];
		miss->Mission = mission;
		miss->Read_INI(ini);
	}

	return(true);
}


/***********************************************************************************************
 * RulesClass::Difficulty -- Fetch the various difficulty group settings.                      *
 *                                                                                             *
 *    This routine is used to fetch the various group settings for the difficulty levels.      *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that has the difficulty setting values.     *
 *                                                                                             *
 * OUTPUT:  bool; Was the difficulty section found and processed.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Difficulty(CCINIClass & ini)
{
#if 0
	Difficulty_Get(ini, Diff[DIFF_EASY], "Easy");
	Difficulty_Get(ini, Diff[DIFF_NORMAL], "Normal");
	Difficulty_Get(ini, Diff[DIFF_HARD], "Difficult");
#endif
	return(true);
}


/***********************************************************************************************
 * Is_MCV_Deploy -- Check if MCV can be deployed.                                              *
 *                                                                                             *
 *    This routine is used to check if the Construction Yard can revert back into an MCV.      *
 *    It allows the special variables to override anything set by the rules.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can the Construction Yard revert back into an MCV.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/24/2019 SKY : Created.                                                                 *
 *=============================================================================================*/
bool Is_MCV_Deploy()
{
	return Special.UseMCVDeploy ? Special.IsMCVDeploy : Rule.IsMCVDeploy;
}