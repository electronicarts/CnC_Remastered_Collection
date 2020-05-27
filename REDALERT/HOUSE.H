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

/* $Header: /CounterStrike/HOUSE.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : HOUSE.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 21, 1994                                                 *
 *                                                                                             *
 *                  Last Update : May 21, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef HOUSE_H
#define HOUSE_H

#include	"type.h"
#include	"region.h"
#include	"vector.h"
#include "Credits.h"

class TriggerClass;
class FootClass;
class FactoryClass;

#define HOUSE_NAME_MAX	12


/****************************************************************************
**	Certain aspects of the house "country" are initially set by the scenario
**	control file. This information is static for the duration of the current
**	scenario, but is dynamic between scenarios. As such, it can't be placed in
**	the static HouseTypeClass structure, but is embedded into the house
**	class instead.
*/
class HouseStaticClass {
	public:
		HouseStaticClass(void);
		HouseStaticClass(NoInitClass const & ) {};

		/*
		**	This value indicates the degree of smartness to assign to this house.
		**	A value is zero is presumed for human controlled houses.
		*/
		int IQ;

		/*
		**	This is the buildable tech level for this house. This value is used
		**	for when the computer is deciding what objects to build.
		*/
		int TechLevel;

		/*
		**	This is the original ally specification to use at scenario
		**	start. Various forces during play may adjust the ally state
		**	of this house.
		*/
		int Allies;

		/*
		**	This is the maximum number allowed to be built by this house. The
		**	value depends on the scenario being played.
		*/
		unsigned MaxUnit;
		unsigned MaxBuilding;
		unsigned MaxInfantry;
		unsigned MaxVessel;
		unsigned MaxAircraft;

		/*
		**	This records the initial credits assigned to this house when the scenario
		**	was loaded.
		*/
		long InitialCredits;

		/*
		**	For generic (unspecified) reinforcements, they arrive by a common method. This
		**	specifies which method is to be used.
		*/
		SourceType Edge;
};


/****************************************************************************
**	Player control structure. Each player (computer or human) has one of
**	these structures associated. These are located in a global array.
*/
class HouseClass {
	public:
		RTTIType RTTI;
		int ID;

		/*
		**	Pointer to the HouseTypeClass that this house is "owned" by.
		**	All constant data for a house type is stored in that class.
		*/
		CCPtr<HouseTypeClass> Class;

		/*
		**	This is the handicap (difficulty level) assigned to this house.
		*/
		DiffType Difficulty;

		/*
		**	Override handicap control values.
		*/
		fixed FirepowerBias;
		fixed GroundspeedBias;
		fixed AirspeedBias;
		fixed ArmorBias;
		fixed ROFBias;
		fixed CostBias;
		fixed BuildSpeedBias;
		fixed RepairDelay;
		fixed BuildDelay;

		/*
		**	The initial house data as loaded from the scenario control file is
		**	stored here. Although this data changes for each scenario, it remains
		**	static for the duration of the current scenario.
		*/
		HouseStaticClass Control;

		/*
		**	This is the house type that this house object should act like. This
		**	value controls production choices and radar cover plate imagery.
		*/
		HousesType ActLike;

		/*
		**	Is this player active?  Usually that answer is true, but for civilians, it
		**	might possibly be false.
		*/
		unsigned IsActive:1;

		/*
		**	If this house is controlled by the player, then this flag will be true. The
		**	computer controls all other active houses.
		*/
		unsigned IsHuman:1;
		unsigned WasHuman:1;

		/*
		**	If the player can control units of this house even if the player doesn't
		**	own units of this house, then this flag will be true.
		*/
		unsigned IsPlayerControl:1;

		/*
		**	This flag enables production. If the flag is false, production is disabled.
		**	By timing when this flag gets set, the player can be given some breathing room.
		*/
		unsigned IsStarted:1;

		/*
		**	When alerted, the house will create teams of the special "auto" type and
		**	will generate appropriate units to fill those team types.
		*/
		unsigned IsAlerted:1;

		/*
		**	If automatic base building is on, then this flag will be set to true.
		*/
		unsigned IsBaseBuilding:1;

		/*
		**	If the house has been discovered, then this flag will be set
		**	to true. However, the trigger even associated with discovery
		**	will only be executed during the next house AI process.
		*/
		unsigned IsDiscovered:1;

		/*
		**	If Tiberium storage is maxed out, then this flag will be set. At some point
		**	the player is told of this fact and then this flag is cleared. This allows the
		**	player to be told, but only occasionally rather than continuously.
		*/
		unsigned IsMaxedOut:1;

		/*
		** If this house is played by a human in a multiplayer game, this flag
		** keeps track of whether this house has been defeated or not.
		*/
		unsigned IsDefeated:1;

		/*
		**	These flags are used in conjunction with the BorrowedTime timer. When
		**	that timer expires and one of these flags are set, then that event is
		**	applied to the house. This allows a dramatic pause between the event
		**	trigger and the result.
		*/
		unsigned IsToDie:1;
		unsigned IsToWin:1;
		unsigned IsToLose:1;

		/*
		**	This flag is set when a transport carrying a civilian has been
		**	successfully evacuated. It is presumed that a possible trigger
		**	event will be sprung by this event.
		*/
		unsigned IsCivEvacuated:1;

		/*
		**	If potentially something changed that might affect the sidebar list of
		**	buildable objects, then this flag indicates that at the first LEGAL opportunity,
		**	the sidebar will be recalculated.
		*/
		unsigned IsRecalcNeeded:1;

		/*
		**	If the map has been completely revealed to the player, then this flag
		**	will be set to true. By examining this flag, a second "reveal all map"
		**	crate won't be given to the player.
		*/
		unsigned IsVisionary:1;

		/*
		**	This flag is set to true when the house has determined that
		**	there is insufficient Tiberium to keep the harvesters busy.
		**	In such a case, the further refinery/harvester production
		**	should cease. This is one of the first signs that the endgame
		**	has begun.
		*/
		unsigned IsTiberiumShort:1;

		/*
		**	These flags are used for the general house trigger events of being
		**	spied and thieved. The appropriate flag will be set when the event
		**	occurs.
		*/
		unsigned IsSpied:1;
		unsigned IsThieved:1;

		/*
		** This flag is used to control non-human repairing of buildings.  Each
		** house gets to repair one building per loop, and this flag controls
		** whether this house has 'spent' its repair option this time through.
		*/
		unsigned DidRepair:1;

		/*
		** This flag is used to control whether or not this house has the GPS
		** satellite in orbit.  If the satellite's there, they have unlimited
		** radar and the map is fully revealed.
		*/
		unsigned IsGPSActive:1;

		/*
		**	If the JustBuilt??? variable has changed, then this flag will
		**	be set to true.
		*/
		unsigned IsBuiltSomething:1;

		/*
		** Did this house lose via resignation?
		*/
		unsigned IsResigner:1;

		/*
		** Did this house lose because the player quit?
		*/
		unsigned IsGiverUpper:1;

		/*
		**	If this computer controlled house has reason to be mad at humans,
		**	then this flag will be true. Such a condition prevents alliances with
		**	a human and encourages the computers players to ally amongst themselves.
		*/
		unsigned IsParanoid:1;

		/*
		**	A gap generator shrouded cells and all units of this house must perform
		**	a look just in case their look radius intersects the shroud area.
		*/
		unsigned IsToLook:1;

		/*
		**	MBL 03.23.2020 - Support for queued movement mode (informed from the client)
		*/
		unsigned IsQueuedMovementToggle:1;

		/*
		**	This value indicates the degree of smartness to assign to this house.
		**	A value of zero indicates that the player controls everything.
		*/
		int IQ;

		/*
		**	This records the current state of the base. This state is used to control
		**	what action the base will perform and directly affects production and
		**	unit disposition. The state will change according to time and combat
		**	events.
		*/
		StateType State;

		/*
		**	These super weapon control objects are used to control the recharge
		**	and availability of these special weapons to this house.
		*/
		SuperClass SuperWeapon[SPC_COUNT];

		/*
		**	This is a record of the last building that was built. For buildings that
		**	were built as a part of scenario creation, it will be the last one
		**	discovered.
		*/
		StructType JustBuiltStructure;
		InfantryType JustBuiltInfantry;
		UnitType JustBuiltUnit;
		AircraftType JustBuiltAircraft;
		VesselType JustBuiltVessel;

		/*
		**	This records the number of triggers associated with this house that are
		**	blocking a win condition. A win will only occur if all the blocking
		**	triggers have been deleted.
		*/
		int Blockage;

		/*
		**	For computer controlled houses, there is an artificial delay between
		**	performing repair actions. This timer regulates that delay. If the
		**	timer has not expired, then no repair initiation is allowed.
		*/
		CDTimerClass<FrameTimerClass> RepairTimer;

		/*
		**	This timer controls the computer auto-attack logic. When this timer expires
		**	and the house has been alerted, then it will create a set of attack
		**	teams.
		*/
		CDTimerClass<FrameTimerClass> AlertTime;

		/*
		**	This timer is used to handle the delay between some catastrophic
		**	event trigger and when it is actually carried out.
		*/
		CDTimerClass<FrameTimerClass> BorrowedTime;

		/*
		**	This is the last working scan bits for buildings. If a building is
		**	active and owned by this house, it will have a bit set in this element
		**	that corresponds to the building type number. Since this value is
		**	accumulated over time, the "New" element contains the under-construction
		**	version.
		*/
		unsigned long BScan;
		unsigned long ActiveBScan;
		unsigned long OldBScan;

		/*
		**	This is the last working scan bits for units. For every existing unit
		**	type owned by this house, a corresponding bit is set in this element. As
		**	the scan bits are being constructed, they are built into the "New" element
		**	and then duplicated into the regular element at the end of every logic cycle.
		*/
		unsigned long UScan;
		unsigned long ActiveUScan;
		unsigned long OldUScan;

		/*
		**	Infantry type existence bits. Similar to unit and building bits.
		*/
		unsigned long IScan;
		unsigned long ActiveIScan;
		unsigned long OldIScan;

		/*
		**	Aircraft type existence bits. Similar to unit and building bits.
		*/
		unsigned long AScan;
		unsigned long ActiveAScan;
		unsigned long OldAScan;

		/*
		**	Vessel type existence bits. Similar to unit and building bits.
		*/
		unsigned long VScan;
		unsigned long ActiveVScan;
		unsigned long OldVScan;

		/*
		**	Record of gains and losses for this house during the course of the
		**	scenario.
		*/
		unsigned CreditsSpent;
		unsigned HarvestedCredits;
		int StolenBuildingsCredits;

		/*
		**	This is the running count of the number of units owned by this house. This
		**	value is used to keep track of ownership limits.
		*/
		unsigned CurUnits;
		unsigned CurBuildings;
		unsigned CurInfantry;
		unsigned CurVessels;
		unsigned CurAircraft;

		/*
		**	This is the running total of the number of credits this house has accumulated.
		*/
		long Tiberium;
		long Credits;
		long Capacity;

		/*
		** Stuff to keep track of the total number of units built by this house.
		*/
		UnitTrackerClass * AircraftTotals;
		UnitTrackerClass * InfantryTotals;
		UnitTrackerClass * UnitTotals;
		UnitTrackerClass * BuildingTotals;
		UnitTrackerClass * VesselTotals;

		/*
		** Total number of units destroyed by this house
		*/
		UnitTrackerClass * DestroyedAircraft;
		UnitTrackerClass * DestroyedInfantry;
		UnitTrackerClass * DestroyedUnits;
		UnitTrackerClass * DestroyedBuildings;
		UnitTrackerClass * DestroyedVessels;

		/*
		** Total number of enemy buildings captured by this house
		*/
		UnitTrackerClass * CapturedBuildings;

		/*
		** Total number of crates found by this house
		*/
		UnitTrackerClass * TotalCrates;

		/*
		**	Records the number of infantry and vehicle factories active. This value is
		**	used to regulate the speed of production.
		*/
		int AircraftFactories;
		int InfantryFactories;
		int UnitFactories;
		int VesselFactories;
		int BuildingFactories;

		/*
		**	This is the accumulation of the total power and drain factors. From these
		**	values a ratio can be derived. This ratio is used to control the rate
		**	of building decay.
		*/
		int Power;					// Current power output.
		int Drain;					// Power consumption.

		/*
		**	For human controlled houses, only one type of unit can be produced
		**	at any one instant. These factory objects control this production.
		*/
		int AircraftFactory;
		int InfantryFactory;
		int UnitFactory;
		int VesselFactory;
		int BuildingFactory;

		/*
		**	For human controlled houses, the current state of the radar map
		*/
		RadarEnum Radar;

		/*
		**	This target value specifies where the flag is located. It might be a cell
		**	or it might be an object.
		*/
		TARGET FlagLocation;

		/*
		** This is the flag-home-cell for this house.  This is where we must bring
		** another house's flag back to, to defeat that house.
		*/
		CELL FlagHome;

		/*
		** For multiplayer games, each house needs to keep track of how many
		** objects of each other house they've killed.
		*/
		unsigned UnitsKilled[HOUSE_COUNT];
		unsigned UnitsLost;
		unsigned BuildingsKilled[HOUSE_COUNT];
		unsigned BuildingsLost;

		/*
		** This keeps track of the last house to destroy one of my units.
		** It's used for scoring multiplayer games.
		*/
		HousesType WhoLastHurtMe;

		/*
		** Start location (waypoint index) passed in from GlyphX
		*/
		int StartLocationOverride;

		/*
		**	This records information about the location and size of
		**	the base.
		*/
		COORDINATE Center;			// Center of the base.
		int Radius;				// Average building distance from center (leptons).
		struct {
			int AirDefense;
			int ArmorDefense;
			int InfantryDefense;
		} ZoneInfo[ZONE_COUNT];

		/*
		**	This records information about the last time a building of this
		**	side was attacked. This information is used to determine proper
		**	response.
		*/
		int LATime;						// Time of attack.
		RTTIType LAType;				// Type of attacker.
		ZoneType LAZone;				// Last zone that was attacked.
		HousesType LAEnemy;			// Owner of attacker.

		/*
		**	This target value is the building that must be captured as soon as possible.
		**	Typically, this will be one of the buildings of this house that has been
		**	captured and needs to be recaptured.
		*/
		TARGET ToCapture;

		/*
		** This value shows who is spying on this house's radar facilities.
		** This is used for the other side to be able to update their radar
		** map based on the cells that this house's units reveal.
		*/
		int RadarSpied;

		/*
		** Running score, based on units destroyed and units lost.
		*/
		int PointTotal;

		/*
		**	This is the targeting directions for when this house gets a
		**	special weapon.
		*/
		QuarryType PreferredTarget;

		/*
		**	Screen shake timer.
		*/
		CDTimerClass<FrameTimerClass> ScreenShakeTime;

	private:
		/*
		**	Tracks number of each building type owned by this house. Even if the
		**	building is in construction, it will be reflected in this total.
		*/
#ifdef FIXIT_ANTS
		int BQuantity[STRUCT_COUNT-3];
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		int UQuantity[UNIT_RA_COUNT-3];
#else
		int UQuantity[UNIT_COUNT-3];
#endif
#else
		int BQuantity[STRUCT_COUNT];
		int UQuantity[UNIT_COUNT];
#endif

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		int IQuantity[INFANTRY_RA_COUNT];
#else
		int IQuantity[INFANTRY_COUNT];
#endif
		int AQuantity[AIRCRAFT_COUNT];
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		int VQuantity[VESSEL_RA_COUNT];
#else
		int VQuantity[VESSEL_COUNT];
#endif

		/*
		**	This timer keeps track of when an all out attack should be performed.
		**	When this timer expires, send most of this house's units in an
		**	attack.
		*/
		CDTimerClass<FrameTimerClass> Attack;

	public:
		/*
		**	This records the overriding enemy that the computer will try to
		**	destroy. Typically, this is the last house to attack, but can be
		**	influenced by nearness.
		*/
		HousesType Enemy;

		/*
		**	The house expert system is regulated by this timer. Each computer controlled
		**	house will process the Expert System AI at intermittent intervals. Not only will
		**	this distribute the overhead more evenly, but will add variety to play.
		*/
		CDTimerClass<FrameTimerClass> AITimer;

		/*
		** For the moebius effect, this is a pointer to the unit that we
		** selected to teleport.  Only one teleporter should be active per house.
		*/
		TARGET UnitToTeleport;

		/*
		**	This elaborates the suggested objects to construct. When the specified object
		**	is constructed, then this corresponding value will be reset to nill state. The
		**	expert system decides what should be produced, and then records the
		**	recommendation in these variables.
		*/
		StructType BuildStructure;
		UnitType BuildUnit;
		InfantryType BuildInfantry;
		AircraftType BuildAircraft;
		VesselType BuildVessel;

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		static void * operator new(size_t size);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void *ptr);
		HouseClass(HousesType house);
		HouseClass(NoInitClass const & x) : Class(x), Control(x), AlertTime(x), BorrowedTime(x), Attack(x), AITimer(x), DamageTime(x), TeamTime(x), TriggerTime(x), SpeakAttackDelay(x), SpeakPowerDelay(x), SpeakMoneyDelay(x), SpeakMaxedDelay(x) {};
		~HouseClass(void);
		operator HousesType(void) const;

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		CELL Random_Cell_In_Zone(ZoneType zone) const;
		static void Computer_Paranoid(void);
		bool Is_Allowed_To_Ally(HousesType house) const;
		void Do_All_To_Hunt(void) const;
		void Super_Weapon_Handler(void);
		int * Factory_Counter(RTTIType rtti);
		int Factory_Count(RTTIType rtti) const;
		DiffType Assign_Handicap(DiffType handicap);
		TARGET Find_Juicy_Target(COORDINATE coord) const;
		void Print_Zone_Stats(int x, int y, ZoneType zone, MonoClass * mono) const;
		CELL Where_To_Go(FootClass const * object) const;
		CELL Zone_Cell(ZoneType zone) const;
		ZoneType Which_Zone(COORDINATE coord) const;
		ZoneType Which_Zone(ObjectClass const * object) const;
		ZoneType Which_Zone(CELL cell) const;
		CELL Find_Cell_In_Zone(TechnoClass const * techno, ZoneType zone) const;
		ProdFailType Begin_Production(RTTIType type, int id);
		ProdFailType Suspend_Production(RTTIType type);
		ProdFailType Abandon_Production(RTTIType type);
		bool Place_Object(RTTIType type, CELL cell);
		bool Manual_Place(BuildingClass * builder, BuildingClass * object);
		void Special_Weapon_AI(SpecialWeaponType id);
		bool Place_Special_Blast(SpecialWeaponType id, CELL cell);
		bool Flag_Attach(CELL cell, bool set_home = false);
		bool Flag_Attach(UnitClass * object, bool set_home = false);
		bool Flag_Remove(TARGET target, bool set_home = false);
		void Init_Data(PlayerColorType color, HousesType house, int credits);
		COORDINATE Find_Build_Location(BuildingClass * building) const;
		BuildingClass * Find_Building(StructType type, ZoneType zone=ZONE_NONE) const;
		char const * Name(void) const {return(Class->Name());}

		// Added so the ally flags could be sent to client machines - 09 / 12 / 2019 JAS
		unsigned Get_Ally_Flags();

		bool Fire_Sale(void);
		bool Is_Hack_Prevented(RTTIType rtti, int value) const;
		bool Is_No_YakMig(void) const;
		int Expert_AI(void);
		void Production_Begun(TechnoClass const * rtti);
		void Sell_Wall(CELL cell);
		bool Flag_To_Die(void);
		bool Flag_To_Win(void);
		bool Flag_To_Lose(void);
		void Make_Ally(HousesType house);
		void Make_Ally(ObjectClass * object) {if (object) Make_Ally(object->Owner());};
		void Make_Enemy(HousesType house);
		void Make_Enemy(ObjectClass * object) {if (object) Make_Enemy(object->Owner());};
		bool Is_Ally(HousesType house) const;
		bool Is_Ally(HouseClass const * house) const;
		bool Is_Ally(ObjectClass const * object) const;
		#ifdef CHEAT_KEYS
		void Debug_Dump(MonoClass *mono) const;
		#endif
		void AI(void);
		bool Can_Build(RTTIType rtti, int type, HousesType house) const;

		// Factory controls.
		FactoryClass * Fetch_Factory(RTTIType rtti) const;
		void Set_Factory(RTTIType rtti, FactoryClass * factory);

		bool Can_Build(ObjectTypeClass const * type, HousesType house) const;

		int  Get_Quantity(AircraftType aircraft);
		int  Get_Quantity(StructType building);
		unsigned char const * Remap_Table(bool blushing=false, RemapType remap=REMAP_NORMAL) const;

		TechnoTypeClass const * Suggest_New_Object(RTTIType objectype, bool kennel=false) const;
		BuildingTypeClass const * Suggest_New_Building(void) const;
		void Recalc_Center(void);
		bool Does_Enemy_Building_Exist(StructType) const;
		void Harvested(unsigned tiberium);
		void Stole(unsigned worth);
		long Available_Money(void) const;
		void Spend_Money(unsigned money);
		void Refund_Money(unsigned money);
		void Attacked(BuildingClass* source);
		void Adjust_Power(int adjust);
		void Adjust_Drain(int adjust);
		void Update_Spied_Power_Plants(void);
		int Adjust_Capacity(int adjust, bool inanger=false);
		fixed Power_Fraction(void) const;
		fixed Tiberium_Fraction(void) const;
		void Begin_Production(void) {IsStarted = true;};
		TeamTypeClass const * Suggested_New_Team(bool alertcheck = false);
		void Adjust_Threat(int region, int threat);
		void Tracking_Remove(TechnoClass const * techno);
		void Tracking_Add(TechnoClass const * techno);
		void Active_Remove(TechnoClass const * techno);
		void Active_Add(TechnoClass const * techno);

		UrgencyType Check_Attack(void) const;
		UrgencyType Check_Build_Power(void) const;
		UrgencyType Check_Build_Defense(void) const;
		UrgencyType Check_Build_Offense(void) const;
		UrgencyType Check_Build_Income(void) const;
		UrgencyType Check_Fire_Sale(void) const;
		UrgencyType Check_Build_Engineer(void) const;
		UrgencyType Check_Raise_Money(void) const;
		UrgencyType Check_Raise_Power(void) const;
		UrgencyType Check_Lower_Power(void) const;

		bool AI_Attack(UrgencyType urgency);
		bool AI_Build_Power(UrgencyType urgency) const;
		bool AI_Build_Defense(UrgencyType urgency) const;
		bool AI_Build_Offense(UrgencyType urgency) const;
		bool AI_Build_Income(UrgencyType urgency) const;
		bool AI_Fire_Sale(UrgencyType urgency);
		bool AI_Build_Engineer(UrgencyType urgency) const;
		bool AI_Raise_Money(UrgencyType urgency) const;
		bool AI_Raise_Power(UrgencyType urgency) const;
		bool AI_Lower_Power(UrgencyType urgency) const;

		bool Can_Make_Money(void) const {
			return(Available_Money() > 300 || (BScan & STRUCTF_REFINERY));
		};

		static void Init(void);
		static void One_Time(void);
		static HouseClass * As_Pointer(HousesType house);
		static void Recalc_Attributes(void);

		/*
		** New default win mode to avoid griefing. ST - 1/31/2020 3:33PM
		*/
		void	Check_Pertinent_Structures(void);

		void Init_Unit_Trackers(void);
		void Free_Unit_Trackers(void);

		/*
		**	File I/O.
		*/
		static void Read_INI(CCINIClass & ini);
		static void Write_INI(CCINIClass & ini);
		static void Read_Flag_INI(char *buffer);
		static void Write_Flag_INI(char *buffer);
		bool Load(Straw & file);
		bool Save(Pipe & file) const;
		void Code_Pointers(void);
		void Decode_Pointers(void);

		/*
		**	Special house actions.
		*/
		void Detach(TARGET target, bool all);

		/*
		**	This vector holds the recorded status of the map regions. It is through
		**	this region information that team paths are calculated.
		*/
		RegionClass Regions[MAP_TOTAL_REGIONS];

		/*
		**	This count down timer class decrements and then changes
		** the Atomic Bomb state.
		*/
		CELL NukeDest;

		/*
		** Per-house credits class to track the visible credits state for each house. Redundant in the original game, but needed
		** to preserve the exact credits count behavior in the GlyphX client. ST - 10/16/2019 2:31PM
		*/
		CreditClass VisibleCredits;
		
		bool DebugUnlockBuildables;

		/*
		** This routine completely removes this house & all its objects from the game.
		*/
		void Clobber_All(void);

		/*
		** This routine blows up everything in this house.  Fun!
		*/
		void Blowup_All(void);

		/*
		** This routine gets called in multiplayer games when every unit, building,
		** and infantry for a house is destroyed.
		*/
		void MPlayer_Defeated(void);

		/*
		** When the game's over, this routine assigns everyone their score.
		*/
		void Tally_Score(void);

		friend class MapEditClass;

	private:
		void Silo_Redraw_Check(long oldtib, long oldcap);
		int AI_Building(void);
		int AI_Unit(void);
		int AI_Vessel(void);
		int AI_Infantry(void);
		int AI_Aircraft(void);

		/*
		**	This is a bit field record of all the other houses that are allies with
		**	this house. It is presumed that any house that isn't an ally, is therefore
		**	an enemy. A house is always considered allied with itself.
		*/
		unsigned Allies;

		/*
		**	General low-power related damaged is doled out whenever this timer
		**	expires.
		*/
		CDTimerClass<FrameTimerClass> DamageTime;

		/*
		**	Team creation is done whenever this timer expires.
		*/
		CDTimerClass<FrameTimerClass> TeamTime;

		/*
		**	This controls the rate that the trigger time logic is processed.
		*/
		CDTimerClass<FrameTimerClass> TriggerTime;

		/*
		**	At various times, the computer may announce the player's condition. The following
		**	variables are used as countdown timers so that these announcements are paced
		**	far enough apart to reduce annoyance.
		*/
		CDTimerClass<FrameTimerClass> SpeakAttackDelay;
		CDTimerClass<FrameTimerClass> SpeakPowerDelay;
		CDTimerClass<FrameTimerClass> SpeakMoneyDelay;
		CDTimerClass<FrameTimerClass> SpeakMaxedDelay;

		/*
		**	This structure is used to record a build request as determined by
		**	the house AI processing. Higher priority build requests take precidence.
		*/
		struct BuildChoiceClass {
			static void * operator new(size_t, void * ptr) {return(ptr);};
			UrgencyType	Urgency;			// The urgency of the build request.
			StructType	Structure;		// The type of building to produce.

			BuildChoiceClass(UrgencyType u, StructType s) : Urgency(u), Structure(s) {};
			BuildChoiceClass(NoInitClass const & ) {};
			int Save(Pipe &) const {return(true);};
			int Load(Straw &) {return(true);};
			void Code_Pointers(void) {};
			void Decode_Pointers(void) {};
		};

		static TFixedIHeapClass<BuildChoiceClass> BuildChoice;


	/*
	** These values are for multiplay only.
	*/
	public:
		/*
		** For multiplayer games, each house instance has a remap table; the table
		** in the HousesTypeClass isn't used.  This variable is set to the remap
		** table for the color the player wants to play.
		*/
		PlayerColorType RemapColor;

		/*
		** This is the name ("handle") the player has chosen for himself.
		*/
		char IniName[HOUSE_NAME_MAX];

#ifdef WOLAPI_INTEGRATION
		//	For Internet games only, unchanging name of player when game began.
		//	This name does not get changed to "Computer" if computer takes over for player.
		char InitialName[HOUSE_NAME_MAX];
#endif

		int QuantityB(int index) {return(BQuantity[index]);}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		int QuantityU(int index) {
			if(index >= UNIT_RA_COUNT) index -= UNIT_RA_COUNT;
			return(UQuantity[index]);
		}
		int QuantityI(int index) {
			if(index >= INFANTRY_RA_COUNT) index -= INFANTRY_RA_COUNT;
			return(IQuantity[index]);
		}
		int QuantityA(int index) {return(AQuantity[index]);}
		int QuantityV(int index) {
			if(index >= VESSEL_RA_COUNT) index -= VESSEL_RA_COUNT;
			return(VQuantity[index]);
		}
#else
		int QuantityU(int index) {return(UQuantity[index]);}
		int QuantityI(int index) {return(IQuantity[index]);}
		int QuantityA(int index) {return(AQuantity[index]);}
		int QuantityV(int index) {return(VQuantity[index]);}
#endif

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[256];

};

#endif