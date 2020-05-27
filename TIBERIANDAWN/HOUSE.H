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

/* $Header:   F:\projects\c&c\vcs\code\house.h_v   2.21   16 Oct 1995 16:46:14   JOE_BOSTIC  $ */
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
#include "COORDA.h"
#include "Credits.h"

//extern "C" {
//unsigned Cardinal_To_Fixed(unsigned base, unsigned cardinal);
//}
class TriggerClass;
#ifdef USE_RA_AI
class FootClass;		// ST - 7/17/2019 11:54AM
class FactoryClass;	// ST - 7/17/2019 11:54AM
#endif // USE_RA_AI
/****************************************************************************
**	Player control structure. Each player (computer or human) has one of
**	these structures associated. These are located in a global array.
*/
//#define	REBUILD_MAX		5		// Maximum number of structures to rebuild.
class HouseClass {
	public:
		/*
		**	Pointer to the HouseTypeClass that this house is "owned" by.
		**	All constant data for a house type is stored in that class.
		*/
		HouseTypeClass const * const Class;

		/*
		**	Override handicap control values.
		*/
		float FirepowerBias;
		float GroundspeedBias;
		float AirspeedBias;
		float ArmorBias;
		float ROFBias;
		float CostBias;
		float BuildSpeedBias;
		float RepairDelay;
		float BuildDelay;

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
		**	When the computer becomes alerted to the presence of the player's forces, it
		**	begins production and attack logic. This flag is set to true if the human
		**	player has been discovered by the computer.
		*/
		unsigned IsStarted:1;

		/*
		**	When alerted, the house will create teams of the special "auto" type and
		**	will generate appropriate units to fill those team types.
		*/
		unsigned IsAlerted:1;

		/*
		**	If the house has been discovered, then this flag will be set
		**	to true. However, the trigger even associated with discovery
		**	will only be executed during the next house AI process.
		*/
		unsigned IsDiscovered:1;

		/*
		**	If Tiberium storage is maxed out, then this flag will be set. At some point
		**	the player is told of this fact and then this flag is cleared. This allows the
		**	player to be told, but only occationally rather than continuously.
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
		**	If a trigger has indicated that the airstrike option should appear, this flag
		**	will be set to true. It is up to the normal house AI processing to actually
		**	add the airstrike to the sidebar.
		*/
		unsigned IsAirstrikePending:1;

		/*
		**	This records the existance of the three nuke weapon pieces.
		*/
		unsigned	NukePieces:3;

		/*
		**	This flag indicates that a free harvester is pending and will be
		**	created when the FreeHarvester timer expires.
		*/
		unsigned IsFreeHarvester:1;

		TCountDownTimerClass FreeHarvester;

		/*
		**	These super weapon control objects are used to control the recharge
		**	and availability of these special weapons to this house.
		*/
		SuperClass IonCannon;
		SuperClass AirStrike;
		SuperClass NukeStrike;

		/*
		**	This is a record of the last building that was built. For buildings that
		**	were built as a part of scenario creation, it will be the last one
		**	discovered.
		*/
		StructType JustBuilt;

		/*
		**	This records the number of triggers associated with this house that are
		**	blocking a win condition. A win will only occur if all the blocking
		**	triggers have been deleted.
		*/
		int Blockage;

		/*
		**	This timer controls the computer auto-attack logic. When this timer expires
		**	and the house has been alerted, then it will create a set of attack
		**	teams.
		*/
		TCountDownTimerClass AlertTime;

		/*
		**	This timer is used to handle the delay between some catastrophic
		**	event trigger and when it is actually carried out.
		*/
		TCountDownTimerClass BorrowedTime;

		/*
		**	This is the last working scan bits for buildings. If a building is
		**	active and owned by this house, it will have a bit set in this element
		**	that corresponds to the building type number. Since this value is
		**	accumulated over time, the "New" element contains the under-construction
		**	version.
		*/
		unsigned long BScan;
		unsigned long ActiveBScan;
		unsigned long NewBScan;
		unsigned long NewActiveBScan;
#ifdef USE_RA_AI
		unsigned long OldBScan;
#endif

		/*
		**	This is the last working scan bits for units. For every existing unit
		**	type owned by this house, a corresponding bit is set in this element. As
		**	the scan bits are being constructed, they are built into the "New" element
		**	and then duplicated into the regular element at the end of every logic cycle.
		*/
		unsigned long UScan;
		unsigned long ActiveUScan;
		unsigned long NewUScan;
		unsigned long NewActiveUScan;

		/*
		**	Infantry type existence bits. Similar to unit and building bits.
		*/
		unsigned long IScan;
		unsigned long ActiveIScan;
		unsigned long NewIScan;
		unsigned long NewActiveIScan;

		/*
		**	Aircraft type existence bits. Similar to unit and building buts.
		*/
		unsigned long AScan;
		unsigned long ActiveAScan;
		unsigned long NewAScan;
		unsigned long NewActiveAScan;

		/*
		**	Record of gains and losses for this house during the course of the
		**	scenario.
		*/
		unsigned CreditsSpent;
		unsigned HarvestedCredits;

		/*
		**	This is the running count of the number of units owned by this house. This
		**	value is used to keep track of ownership limits.
		*/
		unsigned CurUnits;
		unsigned CurBuildings;
#ifdef USE_RA_AI
		unsigned CurInfantry;				// Added from RA for AI. ST - 7/17/2019 12:03PM
		unsigned CurAircraft;				// Added from RA for AI. ST - 7/17/2019 12:03PM
#endif //USE_RA_AI

		/*
		**	This is the maximum number allowed to be built by this house. The
		**	value depends on the scenario being played.
		*/
		unsigned MaxUnit;
		unsigned MaxBuilding;
#ifdef USE_RA_AI
		unsigned MaxInfantry;				// Added from RA for AI. ST - 7/19/2019 4:31PM
		unsigned MaxAircraft;            // Added from RA for AI. ST - 7/19/2019 4:31PM
#endif //USE_RA_AI

		/*
		**	This is the running total of the number of credits this house has accumulated.
		*/
		long Tiberium;
		long Credits;
		long InitialCredits;
		long Capacity;

		/*
		** Did this house lose via resignation?
		*/
		unsigned Resigned:1;

		/*
		** Did this house lose because the player quit?
		*/
		unsigned IGaveUp:1;

		/*
		** Stuff to keep track of the total number of units built by this house.
		*/
		UnitTrackerClass	*AircraftTotals;
		UnitTrackerClass	*InfantryTotals;
		UnitTrackerClass	*UnitTotals;
		UnitTrackerClass	*BuildingTotals;

		/*
		** Total number of units destroyed by this house
		*/
		UnitTrackerClass	*DestroyedAircraft;
		UnitTrackerClass	*DestroyedInfantry;
		UnitTrackerClass	*DestroyedUnits;
		UnitTrackerClass	*DestroyedBuildings;

		/*
		** Total number of enemy buildings captured by this house
		*/
		UnitTrackerClass	*CapturedBuildings;

		/*
		** Total number of crates found by this house
		*/
		UnitTrackerClass	*TotalCrates;

		/*
		**	Records the number of infantry and vehicle factories active. This value is
		**	used to regulate the speed of production.
		*/
		int AircraftFactories;
		int InfantryFactories;
		int UnitFactories;
		int BuildingFactories;
		int SpecialFactories;

		/*
		**	This is the accumulation of the total power and drain factors. From these
		**	values a ratio can be derived. This ratio is used to control the rate
		**	of building decay.
		*/
		int Power;					// Current power output.
		int Drain;					// Power consumption.

		/*
		**	For generic (unspecified) reinforcements, they arrive by a common method. This
		**	specifies which method is to be used.
		*/
		SourceType Edge;

		/*
		**	For human controlled houses, only one type of unit can be produced
		**	at any one instant. These factory objects control this production.
		*/
		int AircraftFactory;
		int InfantryFactory;
		int UnitFactory;
		int BuildingFactory;
		int SpecialFactory;

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
		** For multiplayer games, each house instance has a remap table; the table
		** in the HousesTypeClass isn't used.  This variable is set to the remap
		** table for the color the player wants to play.
		*/
		unsigned char const * RemapTable;
		PlayerColorType RemapColor;
		char Name[MPLAYER_NAME_MAX];

		/*
		** For multiplayer games, each house needs to keep track of how many
		** objects of each other house they've killed.
		*/
		unsigned UnitsKilled[HOUSE_COUNT];
		unsigned UnitsLost;
		unsigned BuildingsKilled[HOUSE_COUNT];
		unsigned BuildingsLost;

		/*
		** For multiplayer games, this keeps track of the last house to destroy
		** one of my units.
		*/
		HousesType WhoLastHurtMe;

		/*
		** Start location (waypoint index) passed in from GlyphX
		*/
		int StartLocationOverride;
				
		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		static void * operator new(size_t size);
		static void operator delete(void *ptr);
		HouseClass(void) : Class(0) {};
		HouseClass(HousesType house);
		~HouseClass(void);
		operator HousesType(void) const;

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		ProdFailType  Begin_Production(RTTIType type, int id);
		ProdFailType  Suspend_Production(RTTIType type);
		ProdFailType  Abandon_Production(RTTIType type);
		bool  Place_Object(RTTIType type, CELL cell);
		bool  Manual_Place(BuildingClass * builder, BuildingClass * object);
		void  Special_Weapon_AI(SpecialWeaponType id);
		bool  Place_Special_Blast(SpecialWeaponType id, CELL cell);
		bool  Flag_Attach(CELL cell, bool set_home = false);
		bool  Flag_Attach(UnitClass * object, bool set_home = false);
		bool  Flag_Remove(TARGET target, bool set_home = false);
		void  Init_Data(PlayerColorType color, HousesType house, int credits);

		void  Sell_Wall(CELL cell);
		bool  Flag_To_Die(void);
		bool  Flag_To_Win(void);
		bool  Flag_To_Lose(void);
		void  Make_Ally(HousesType house);
		void  Make_Ally(ObjectClass * object) {if (object) Make_Ally(object->Owner());};
		void  Make_Enemy(HousesType house);
		void  Make_Enemy(ObjectClass * object) {if (object) Make_Enemy(object->Owner());};
		bool  Is_Ally(HousesType house) const;
		bool  Is_Ally(HouseClass const * house) const;
		bool  Is_Ally(ObjectClass const * object) const;
		#ifdef CHEAT_KEYS
		void  Debug_Dump(MonoClass *mono) const;
		#endif
		void  AI(void);
		bool  Can_Build(StructType structure, HousesType house) const;
		bool  Can_Build(InfantryType infantry, HousesType house) const;
		bool  Can_Build(UnitType unit, HousesType) const;
		bool  Can_Build(AircraftType aircraft, HousesType house) const;
		bool  Can_Build(TechnoTypeClass const * type, HousesType house) const;
		unsigned char const *  Remap_Table(bool blushing=false, bool unit=false) const;

		TechnoTypeClass const *  Suggest_New_Object(RTTIType objectype) const;
		bool  Does_Enemy_Building_Exist(StructType) const;
		void  Harvested(unsigned tiberium);
		long  Available_Money(void) const;
		void  Spend_Money(unsigned money);
		void  Refund_Money(unsigned money);
		void  Attacked(BuildingClass* source);
#ifndef USE_RA_AI
		void  Adjust_Power(int adjust) {Power += adjust;};  // Replaced by function from RA. ST - 7/17/2019 11:51AM		
		void  Adjust_Drain(int adjust) {Drain += adjust;};  // Replaced by function from RA. ST - 7/17/2019 11:51AM
#endif
		int  Adjust_Capacity(int adjust, bool inanger=false);
		int  Power_Fraction(void) const;
		int  Tiberium_Fraction(void) {return (!Tiberium) ? 0 : Cardinal_To_Fixed(Capacity, Tiberium);};
		void  Begin_Production(void) {IsStarted = true;};
		TeamTypeClass const *  Suggested_New_Team(bool alertcheck = false);
		void  Adjust_Threat(int region, int threat);

		static void Init(void);
		static void One_Time(void);
		static HouseClass *  As_Pointer(HousesType house);

		/*
		**	File I/O.
		*/
		static void  Read_INI(char *buffer);
		static void  Write_INI(char *buffer);
		static void  Read_Flag_INI(char *buffer);
		static void  Write_Flag_INI(char *buffer);
		bool  Load(FileClass & file);
		bool  Save(FileClass & file);
		void  Code_Pointers(void);
		void  Decode_Pointers(void);

		/*
		**	Dee-buggin' support.
		*/
		int Validate(void) const;

		/*
		**	Special house actions.
		*/
//		void  Init_Ion_Cannon(bool first_time, bool one_time_effect = false);
//		void  Init_Air_Strike(bool first_time, bool one_time_effect = false);
//		void  Init_Nuke_Bomb(bool first_time, bool one_time_effect = false);
//		void  Remove_Ion_Cannon(void);
//		void  Remove_Air_Strike(void);
//		void  Remove_Nuke_Bomb(void);
		void  Detach(TARGET target, bool all);
		void  Add_Nuke_Piece(int piece=-1);
//		void  Make_Air_Strike_Available(bool present, bool one_time_effect = false);
		bool  Has_Nuke_Device(void);


		/*
		** New default win mode to avoid griefing. ST - 1/31/2020 3:33PM
		*/
		void	Check_Pertinent_Structures(void);

		void Init_Unit_Trackers(void);
		void Free_Unit_Trackers(void);

#ifdef USE_RA_AI
		/*
		** AI Functions imported from RA
		*/
		//TechnoTypeClass const * Suggest_New_Object(RTTIType objectype, bool kennel=false) const;
		BuildingTypeClass const * Suggest_New_Building(void) const;
		void Recalc_Center(void);
		//void Harvested(unsigned tiberium);
		void Stole(unsigned worth);
		//long Available_Money(void) const;
		//void Spend_Money(unsigned money);
		//void Refund_Money(unsigned money);
		//void Attacked(void);
		void Adjust_Power(int adjust);
		void Adjust_Drain(int adjust);
		//int Adjust_Capacity(int adjust, bool inanger=false);
		//fixed Power_Fraction(void) const;
		//fixed Tiberium_Fraction(void) const;
		//void Begin_Production(void) {IsStarted = true;};
		//TeamTypeClass const * Suggested_New_Team(bool alertcheck = false);
		//void Adjust_Threat(int region, int threat);
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

		int Expert_AI(void);
		COORDINATE Find_Build_Location(BuildingClass * building) const;
		BuildingClass * Find_Building(StructType type, ZoneType zone=ZONE_NONE) const;
		int AI_Building(void);
		int AI_Unit(void);
		int AI_Vessel(void);
		int AI_Infantry(void);
		int AI_Aircraft(void);
		void Production_Begun(TechnoClass const * rtti);
		int * Factory_Counter(RTTIType rtti);
		int Factory_Count(RTTIType rtti) const;
		CELL Where_To_Go(FootClass const * object) const;
		CELL Zone_Cell(ZoneType zone) const;
		ZoneType Which_Zone(COORDINATE coord) const;
		ZoneType Which_Zone(ObjectClass const * object) const;
		ZoneType Which_Zone(CELL cell) const;
		DiffType Assign_Handicap(DiffType handicap);
		TARGET Find_Juicy_Target(COORDINATE coord) const;
		int  Get_Quantity(AircraftType aircraft);
		int  Get_Quantity(StructType building);
		FactoryClass * Fetch_Factory(RTTIType rtti) const;
		void Set_Factory(RTTIType rtti, FactoryClass * factory);
		bool Fire_Sale(void);
		bool Is_Hack_Prevented(RTTIType rtti, int value) const;
		bool Is_No_YakMig(void) const;
		CELL Random_Cell_In_Zone(ZoneType zone) const;
		static void Computer_Paranoid(void);
		bool Is_Allowed_To_Ally(HousesType house) const;
		void Do_All_To_Hunt(void) const;
		void Super_Weapon_Handler(void);
		CELL Find_Cell_In_Zone(TechnoClass const * techno, ZoneType zone) const;

		// Added so the ally flags could be sent to client machines - 09 / 12 / 2019 JAS
		unsigned Get_Ally_Flags();

		static void Recalc_Attributes(void);
#endif //USE_RA_AI


		/*
		**	This vector holds the recorded status of the map regions. It is through
		**	this region information that team paths are calculated.
		*/
		RegionClass Regions[MAP_TOTAL_REGIONS];

#ifdef OBSOLETE
		/*
		**	This count down timer class handles decrements and then changes
		** the ion cannon state.  If the Ion cannon was out of range it is
		** now in range.  If the Ion cannon was in range it will toggle out
		** of range.
		*/
		TCountDownTimerClass IonControl;
		int						IonOldStage;

		TCountDownTimerClass AirControl;
		int						AirOldStage;

		TCountDownTimerClass NukeControl;
		int						NukeOldStage;
#endif


		/*
		** This timer is for multiplayer mode; for a computer-controlled house,
		** it determines how long until this player "blitzes" the hapless humans.
		*/
		TCountDownTimerClass BlitzTime;

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
		**	Screen shake timer.
		*/
		TCountDownTimerClass ScreenShakeTime;

	private:
		void Silo_Redraw_Check(long oldtib, long oldcap);

		/*
		**	This is a bit field record of all the other houses that are allies with
		**	this house. It is presumed that any house that isn't an ally, is therefore
		**	an enemy. A house is always considered allied with itself.
		*/
		unsigned Allies;

		/*
		**	This is the standard delay time between announcements concerning the
		**	state of the base or other intermittent house related events.
		*/
		enum SpeakDelayEnum {
			SPEAK_DELAY=TICKS_PER_MINUTE*2,
			TEAM_DELAY=TICKS_PER_MINUTE/10,
			DAMAGE_DELAY=TICKS_PER_MINUTE
		};

		/*
		**	General low-power related damaged is doled out whenever this timer
		**	expires.
		*/
		TCountDownTimerClass DamageTime;

		/*
		**	Team creation is done whenever this timer expires.
		*/
		TCountDownTimerClass TeamTime;

		/*
		**	This controls the rate that the trigger time logic is processed.
		*/
		TCountDownTimerClass TriggerTime;

		/*
		**	At various times, the computer may announce the player's condition. The following
		**	variables are used as countdown timers so that these announcements are paced
		**	far enough appart to reduce annoyance.
		*/
		TCountDownTimerClass SpeakAttackDelay;
		TCountDownTimerClass SpeakPowerDelay;
		TCountDownTimerClass SpeakMoneyDelay;
		TCountDownTimerClass SpeakMaxedDelay;


#ifdef USE_RA_AI

		/***************************************************************************************
		** 
		** AI data imported from RA. ST - 7/17/2019 11:35AM
		** 
		** 
		*/

	public:
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
		//QuarryType PreferredTarget;

	private:
		/*
		**	Tracks number of each building type owned by this house. Even if the
		**	building is in construction, it will be reflected in this total.
		*/
		int BQuantity[STRUCT_COUNT];
		int UQuantity[UNIT_COUNT];
		int IQuantity[INFANTRY_COUNT];
		int AQuantity[AIRCRAFT_COUNT];

		/*
		**	This timer keeps track of when an all out attack should be performed.
		**	When this timer expires, send most of this house's units in an
		**	attack.
		*/
		/////CDTimerClass<FrameTimerClass> Attack;
		TCountDownTimerClass Attack;

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
		/////CDTimerClass<FrameTimerClass> AITimer;
		TCountDownTimerClass AITimer;

		/*
		** For the moebius effect, this is a pointer to the unit that we
		** selected to teleport.  Only one teleporter should be active per house.
		*/
		//TARGET UnitToTeleport;

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
		//VesselType BuildVessel;

		/*
		**	This records the current state of the base. This state is used to control
		**	what action the base will perform and directly affects production and
		**	unit disposition. The state will change according to time and combat
		**	events.
		*/
		StateType State;

		/*
		**	If automatic base building is on, then this flag will be set to true.
		*/
		unsigned IsBaseBuilding:1;

		/*
		**	This flag is set to true when the house has determined that
		**	there is insufficient Tiberium to keep the harvesters busy.
		**	In such a case, the further refinery/harvester production
		**	should cease. This is one of the first signs that the endgame
		**	has begun.
		*/
		unsigned IsTiberiumShort:1;

		/*
		**	If this computer controlled house has reason to be mad at humans,
		**	then this flag will be true. Such a condition prevents alliances with
		**	a human and encourages the computers players to ally amongst themselves.
		*/
		unsigned IsParanoid:1;

		/*
		**	This value indicates the degree of smartness to assign to this house.
		**	A value is zero is presumed for human controlled houses.
		*/
		int IQ;

		/*
		**	This is the handicap (difficulty level) assigned to this house.
		*/
		DiffType Difficulty;

		/*
		**	This structure is used to record a build request as determined by
		**	the house AI processing. Higher priority build requests take precidence.
		*/
		struct BuildChoiceClass {
			static void * operator new(size_t, void * ptr) {return(ptr);};
			UrgencyType	Urgency;			// The urgency of the build request.
			StructType	Structure;		// The type of building to produce.

			BuildChoiceClass(UrgencyType u, StructType s) : Urgency(u), Structure(s) {};
			int Save(FileClass &) const {return(true);};
			int Load(FileClass &) {return(true);};
			void Code_Pointers(void) {};
			void Decode_Pointers(void) {};

		};

		static TFixedIHeapClass<BuildChoiceClass> BuildChoice;

#endif // USE_RA_AI

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[256];

};
#endif