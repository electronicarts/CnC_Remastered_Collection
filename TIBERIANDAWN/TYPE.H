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

/* $Header:   F:\projects\c&c\vcs\code\type.h_v   2.20   16 Oct 1995 16:45:42   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TYPE.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 14, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 14, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef TYPE_H
#define TYPE_H

#include	"mission.h"
#include	"target.h"

class MapEditClass;
class HouseClass;


/**********************************************************************
**	This is the constant data associated with a weapon. Some objects
**	can have multiple weapons and this class is used to isolate and
**	specify this data in a convenient and selfcontained way.
*/
class WeaponTypeClass
{
	public:

		/*
		**	This is the unit class of the projectile fired. A subset of the unit types
		**	represent projectiles. It is one of these classes that is specified here.
		**	If this object does not fire anything, then this value will be UNIT_NONE.
		*/
		BulletType Fires;

		/*
		**	This is the damage (explosive load) to be assigned to the projectile that
		**	this object fires.
		*/
		unsigned char Attack;

		/*
		**	Objects that fire (which can be buildings as well) will fire at a
		**	frequency controlled by this value. This value serves as a count
		**	down timer between shots. The smaller the value, the faster the
		**	rate of fire.
		*/
		unsigned char ROF;

		/*
		**	When this object fires, the range at which it's projectiles travel is
		**	controlled by this value. The value represents the number of cells the
		**	projectile will travel. Objects outside of this range will not be fired
		**	upon (in normal circumstances).
		*/
		int Range;

		/*
		**	This is the typical sound generated when firing.
		*/
		VocType Sound;

		/*
		**	This is the animation to display at the firing coordinate.
		*/
		AnimType Anim;
};


/**********************************************************************
**	Each of the warhead types has specific characteristics. This structure
**	holds these characteristics.
*/
class WarheadTypeClass
{
	public:

		/*
		**	This value control how damage from this warhead type will reduce
		**	over distance. The larger the number, the less the damage is reduced
		**	the farther the distance from the source of the damage.
		*/
		int SpreadFactor;

		/*
		**	If this warhead type can destroy walls, then this flag will be true.
		*/
		bool IsWallDestroyer;

		/*
		**	If this warhead can destroy wooden walls, then this flag will be true.
		*/
		bool IsWoodDestroyer;

		/*
		**	Does this warhead damage tiberium?
		*/
		bool IsTiberiumDestroyer;

		/*
		**	The warhead damage is reduced depending on the the type of armor the
		**	defender has. This table is what gives weapons their "character".
		*/
		unsigned Modifier[ARMOR_COUNT];
};


/**********************************************************************
**	Each house has certain unalienable characteristics. This structure
**	elaborates these.
*/
class HouseTypeClass {
	public:

		/*
		**	This is the house number (enum). This is a unique identification
		**	number for the house.
		*/
		HousesType House;

		/*
		**	The INI name of the house is pointed to by this element. This is the
		**	identification name used in the scenario INI file.
		*/
		char const *IniName;

		/*
		**	The full name (translated) of the house is identified by this number.
		**	The actual text of the name is located in a text file loaded at run
		**	time.
		*/
		int FullName;

		/*
		**	This is the filename suffix to use when creating a house specific
		**	file name. It is three characters long.
		*/
		char Suffix[4];

		/*
		**	This is the "lemon percentage" to use when determining if a particular
		**	object owned by this house is to be flagged as a "lemon". Objects so
		**	flagged have a greater break-down chance. The percentage is expressed
		**	as a fixed point number with 0x000 meaning 0% and 0x100 meaning 100%.
		*/
		unsigned Lemon;

		/*
		**	Each house is assigned a unique identification color to be used on the
		**	radar map and other color significant areas.
		*/
		unsigned char Color;

		unsigned char BrightColor;

		/*
		**	This points to the default remap table for this house.
		*/
		unsigned char const * RemapTable;
		PlayerColorType RemapColor;

		/*
		**	This is a unique ASCII character used when constructing filenames. It
		**	serves a similar purpose as the "Suffix" element, but is only one
		**	character long.
		*/
		char Prefix;

		/*
		**	This controls the various general adjustments to the house owned
		**	unit and building ratings. The default value for these ratings is 1.0.
		*/
		float FirepowerBias;
		float GroundspeedBias;
		float AirspeedBias;
		float ArmorBias;
		float ROFBias;
		float CostBias;
		float BuildSpeedBias;

		//------------------------------------------------------------------------
		HouseTypeClass(HousesType house,
							char const *ini,
							int fullname,
							char const *ext,
							int lemon,
							int color,
							int bright_color,
							PlayerColorType remapcolor,
							unsigned char const * remap,
							char prefix);

		static HousesType  From_Name(char const *name);
		static HouseTypeClass const &  As_Reference(HousesType house);
		static void One_Time(void);

	private:
		static HouseTypeClass const * const Pointers[HOUSE_COUNT];
};


/***************************************************************************
**	This is the abstract type class. It holds information common to all
**	objects that might exist. This contains the name of
**	the object type.
*/
class AbstractTypeClass
{
	public:

		/*
		**	This is the internal control name of the object. This name does
		**	not change regardless of language specified. This is the name
		**	used in scenario control files and for other text based unique
		**	identification purposes.
		*/
		char IniName[9];

		/*
		**	The translated (language specific) text name number of this object.
		**	This number is used to fetch the object's name from the language
		**	text file. Whenever the name of the object needs to be displayed,
		**	this is used to determine the text string.
		*/
		int Name;

		AbstractTypeClass(void) {};
		AbstractTypeClass(int name, char const * ini);
		virtual RTTIType  What_Am_I(void) const;

		virtual COORDINATE  Coord_Fixup(COORDINATE coord) const;
		virtual int  Full_Name(void) const;
		void  Set_Name(char const *buf) const
		{
			strncpy((char *)IniName, buf, sizeof(IniName));
			((char &)IniName[sizeof(IniName)-1]) = '\0';
		};
		virtual unsigned short  Get_Ownable(void) const;
};


/***************************************************************************
**	This the the common base class of game objects. Since these values
**	represent the unchanging object TYPES, this data is initialized at game
**	start and not changed during play. It is "const" data.
*/
class ObjectTypeClass : public AbstractTypeClass
{
	public:

		/*
		**	Is this object squashable by heavy vehicles?  If it is, then the vehicle
		**	can travel over this object and destroy it in the process.
		*/
		unsigned IsCrushable:1;

		/*
		**	Does this object type NOT show up on radar scans?  If true, then in any
		**	radar display, only the underlying ground will be show, not this object.
		**	Most terrain falls into this category, but only a few special real units/buildings
		**	do.
		*/
		unsigned IsStealthy:1;

		/*
		**	It is legal to "select" some objects in the game. If it is legal to select this
		**	object type then this flag will be true. Selected game objects typically display
		**	a floating health bar and allows special user I/O control.
		*/
		unsigned IsSelectable:1;

		/*
		**	Can this object be the target of an attack or move command?  Typically, only objects
		**	that take damage or can be destroyed are allowed to be a target.
		*/
		unsigned IsLegalTarget:1;

		/*
		**	"Insignificant" objects will not be announced when they are destroyed or when they
		**	appear. Terrain elements and some lesser vehicles have this characteristic.
		*/
		unsigned IsInsignificant:1;

		/*
		**	Is this object immune to normal combat damage?  Rocks and other inert type terrain
		**	object are typically of this type.
		*/
		unsigned IsImmune:1;

		/*
		**	If this terrain object is flammable (such as trees are) then this
		**	flag will be true. Flammable objects can catch fire if damaged by
		**	flame type weapons.
		*/
		unsigned IsFlammable:1;

		/*
		**	"Sentient" objects are ones that have logic AI processing performed on them. All
		**	vehicles, buildings, infantry, and aircraft are so flagged. Terrain elements also
		**	fall under this category, but only because certain animation effects require this.
		*/
		unsigned IsSentient:1;

		/*
		**	The defense of this object is greatly affected by the type of armor
		**	it possesses. This value specifies the type of armor.
		*/
		ArmorType Armor;

		/*
		**	This is the maximum strength of this object type.
		*/
		unsigned short MaxStrength;

		/*
		**	These point to the shape imagery for this object type. Since the shape imagery
		**	exists in a separate file, the data is filled in after this object is constructed.
		**	The "mutable" keyword allows easy modification to this otherwise const object.
		*/
		void const * ImageData;

		/*
		**	This points to the radar imagery for this object.
		*/
		void const * RadarIcon;

		//--------------------------------------------------------------------
		ObjectTypeClass(	bool is_sentient,
								bool is_flammable,
								bool is_crushable,
								bool is_stealthy,
								bool is_selectable,
								bool is_legal_target,
								bool is_insignificant,
								bool is_immune,
								int fullname,
								char const *name,
								ArmorType armor,
								unsigned short strength);

		static void One_Time(void);

		virtual int  Max_Pips(void) const;
		virtual void  Dimensions(int &width, int &height) const;
		virtual bool  Create_And_Place(CELL , HousesType =HOUSE_NONE) const = 0;
		virtual int  Cost_Of(void) const;
		virtual int  Time_To_Build(HousesType house) const;
		virtual ObjectClass *  Create_One_Of(HouseClass *) const = 0;
		virtual short const *  Occupy_List(bool placement=false) const;
		virtual short const *  Overlap_List(void) const;
		virtual BuildingClass *  Who_Can_Build_Me(bool, bool, HousesType) const;
		virtual void const *  Get_Cameo_Data(void) const;
		void const *  Get_Image_Data(void) const {return ImageData;};
		void const *  Get_Radar_Data(void) const {return RadarIcon;};

		#ifdef SCENARIO_EDITOR
		virtual void  Display(int, int, WindowNumberType, HousesType) const {};
		#endif

		static void const * SelectShapes;
		static void const * PipShapes;
};


/***************************************************************************
**	This class is the common data for all objects that can be owned, produced,
** or delivered as reinforcements. These are objects that typically contain
**	crews and weapons -- the fighting objects of the game.
*/
class TechnoTypeClass : public ObjectTypeClass
{
	public:

		/*
		**	If this object can serve as a good leader for a group selected
		**	series of objects, then this flag will be true. Unarmed or
		**	ability challenged units do not make good leaders.
		*/
		unsigned IsLeader:1;

		/*
		**	Does this object have the ability to detect the presence of a nearby
		**	cloaked object?
		*/
		unsigned IsScanner:1;

		/*
		**	If this object is always given its proper name rather than a generic
		**	name, then this flag will be true. Typically, civilians and Dr. Mobius
		**	fall under this catagory.
		*/
		unsigned IsNominal:1;

		/*
		**	If the artwork for this object (only for generics) is theater specific, then
		**	this flag will be true. Civilian buildings are a good example of this.
		*/
		unsigned IsTheater:1;

		/*
		**	Does this object type contain a rotating turret?  Gun emplacements, SAM launchers,
		**	and many vehicles contain a turret. If a turret is present, special rendering and
		**	combat logic must be performed.
		*/
		unsigned IsTurretEquipped:1;

		/*
		**	Certain units and buildings fire two shots in quick succession. If this is
		**	the case, then this flag is true.
		*/
		unsigned IsTwoShooter:1;

		/*
		**	Certain objects can be repaired. For buildings, they repair "in place". For units,
		**	they must travel to a repair center to be repaired. If this flag is true, then
		**	allow the player or computer AI to repair the object.
		*/
		unsigned IsRepairable:1;

		/*
		**	Is this object possible to be constructed?  Certain buildings and units cannot
		**	be constructed using normal means. They are either initially placed in the scenario
		**	or can only arrive by pre arranged reinforcement scheduling. Civilian buildings and
		**	vehicles are typical examples of this type of object. They would set this flag to
		**	false.
		*/
		unsigned IsBuildable:1;

		/*
		**	Does this object contain a crew?  If it does, then when the object is destroyed, there
		**	is a distinct possibility that infantry will "pop out". Only units with crews can
		**	become "heros".
		*/
		unsigned IsCrew:1;

		/*
		**	Is this object typically used to transport reinforcements or other cargo?
		**	Transport aircraft, helicopters, and hovercraft are typicall examples of
		**	this.
		*/
		unsigned IsTransporter:1;

		/*
		**	Most objects have the ability to reveal the terrain around themselves.
		**	This sight range (expressed in cell distance) is specified here. If
		**	this value is 0, then this unit never reveals terrain. Bullets are
		**	typically of this nature.
		*/
		int SightRange;

		/*
		**	These values control the cost to produce, the time to produce, and
		**	the scenario when production can first start.
		*/
		int Cost;
		unsigned char Scenario;

		/*
		**	Special build prerequisite control values. These are primarily used for
		**	multi-player or special events.
		*/
		unsigned char Level;
		long Pre;

		/*
		**	The risk and reward values are used to determine targets and paths
		**	toward targets. When heading toward a target, a path of least
		**	risk will be followed. When picking a target, the object of
		**	greatest reward will be selected. The values assigned are
		** arbitrary.
		*/
		int Risk,Reward;

		/*
		**	This value indicates the maximum speed that this object can achieve.
		*/
		MPHType MaxSpeed;

		/*
		**	This is the maximum number of ammo shots this object can hold. If
		**	this number is -1, then this indicates unlimited ammo.
		*/
		int MaxAmmo;

		/*
		**	This is a bit field representing the houses that are allowed to
		**	own (by normal means) this particular object type. This value is
		**	typically used in production contexts. It is possible for a side
		**	to take possession of an object type otherwise not normally allowed.
		**	This event usually occurs as a result of capture.
		*/
		unsigned short Ownable;

		/*
		**	This is the small icon image that is used to display the object in
		**	the sidebar for construction selection purposes.
		*/
		void const * CameoData;

		/*
		**	These are the weapons that this techno object is armed with.
		*/
		WeaponType Primary;
		WeaponType Secondary;

		//--------------------------------------------------------------------
		TechnoTypeClass(
				int name,
				char const *ininame,
				unsigned char level,
				long pre,
				bool is_leader,
				bool is_scanner,
				bool is_nominal,
				bool is_transporter,
				bool is_flammable,
				bool is_crushable,
				bool is_stealthy,
				bool is_selectable,
				bool is_legal_target,
				bool is_insignificant,
				bool is_immune,
				bool is_theater,
				bool is_twoshooter,
				bool is_turret_equipped,
				bool is_repairable,
				bool is_buildable,
				bool is_crew,
				int ammo,
				unsigned short strength,
				MPHType maxspeed,
				int sightrange,
				int cost,
				int scenario,
				int risk,
				int reward,
				int ownable,
				WeaponType primary,
				WeaponType secondary,
				ArmorType armor);

		virtual int  Raw_Cost(void) const;
		virtual int  Max_Passengers(void) const;
		virtual int  Repair_Cost(void) const;
		virtual int  Repair_Step(void) const;
		virtual void const *  Get_Cameo_Data(void) const;
		virtual int  Cost_Of(void) const;
		virtual int  Time_To_Build(HousesType house) const;
		virtual unsigned short  Get_Ownable(void) const;
		
#ifdef USE_RA_AI
		int Legal_Placement(CELL pos) const;  // From RA for AI. ST - 7/24/2019 5:20PM		
#endif // USE_RA_AI

};


/***************************************************************************
**	Building types need some special information custom to buildings. This
**	is a derived class that elaborates these additional data elements.
*/
class BuildingTypeClass : public TechnoTypeClass {
		enum BuildingTypeClassRepairEnums {
			//REPAIR_COST=1,				// Cost to repair a single "step".
			REPAIR_PERCENT=102,		// 40% fixed point number.
			REPAIR_STEP=5				// Number of damage points recovered per "step".
		};

	public:

		/*
		**	This flag controls whether the building is equiped with a dirt
		**	bib or not. A building with a bib has a dirt patch automatically
		**	attached to the structure when it is placed.
		*/
		unsigned IsBibbed:1;

		/*
		**	If this building is a special wall type, such that it exists as a building
		**	for purposes of construction but transforms into an overlay wall object when
		**	it is placed on the map, then this flag will be true.
		*/
		unsigned IsWall:1;

		/*
		**	Some buildings are producers. This flag will be true in that case. Producer,
		**	or factory, type buildings have special logic performed.
		*/
		unsigned IsFactory:1;

		/*
		**	Buildings can have either simple or complex damage stages. If simple,
		**	then the second to the last frame is the half damage stage, and the last
		**	frame is the complete damage stage. For non-simple damage, buildings
		**	have a complete animation set for damaged as well as undamaged condition.
		**	Turrets, oil pumps, and repair facilities are a few examples.
		*/
		unsigned IsSimpleDamage:1;

		/*
		**	Some buildings can be placed directly on raw ground. Such buildings don't require
		**	and are not affected by concrete or lack thereof. Typically, concrete itself is
		**	considered sturdy. The same goes for walls and similar generic type structures.
		**	The more sophisticated buildings are greatly affected by lack of concrete and thus
		**	would have this flag set to false.
		*/
		unsigned IsSturdy:1;

		/*
		**	Certain building types can be captures by enemy infantry. For those
		**	building types, this flag will be true. Typically, military or hardened
		**	structures such as turrets cannot be captured.
		*/
		unsigned IsCaptureable:1;

		/*
		**	If this building really only has cosmetic idle animation, then this flag will be
		**	true if this animation should run at a relatively constant rate regardless of game
		**	speed setting.
		*/
		unsigned IsRegulated:1;

		/*
		**	If this flag is true, then the building cannot be sold even if it could have been built. This
		**	is especially useful for mines which can be built but cannot be sold.
		*/
		unsigned IsUnsellable:1;

		/*
		**	This flag specifies the type of object this factory building can "produce". For non
		**	factory buildings, this value will be RTTI_NONE.
		*/
		RTTIType ToBuild;

		/*
		**	For building that produce ground units (infantry and vehicles), there is a default
		**	exit poit defined. This point is where the object is first placed on the map.
		**	Typically, this is located next to a door. The unit will then travel on to a clear
		**	terrain area and enter normal game processing.
		*/
		COORDINATE ExitPoint;

		/*
		**	When determine which cell to head toward when exiting a building, use the
		**	list elaborated by this variable. There are directions of exit that are
		**	more suitable than others. This list is here to inform the system which
		**	directions those are.
		*/
		short const *ExitList;

		/*
		**	This is the structure type identifier. It can serve as a unique
		**	identification number for building types.
		*/
		StructType Type;

		/*
		**	This is a bitflag that represents which unit types can enter this
		**	building. Determine if a unit can enter by taking 1 and shifting it
		**	left by the unit type ID. If the corresponding bit is set, then that
		**	unit type can enter this building.
		*/
		unsigned long CanEnter;

		/*
		**	This is the starting facing to give this building when it first
		**	gets constructed. The facing matches the final stage of the
		**	construction animation.
		*/
		DirType StartFace;

		/*
		**	This is the Tiberium storage capacity of the building. The sum of all
		**	building's storage capacity is used to determine how much Tiberium can
		**	be accumulated.
		*/
		unsigned Capacity;

		/*
		**	Each building type produces and consumes power. These values tell how
		**	much.
		*/
		int Power;
		int Drain;

		/*
		**	This is the size of the building. This size value is a rough indication
		**	of the building's "footprint".
		*/
		BSizeType Size;

		/**********************************************************************
		**	For each stage that a building may be in, its animation is controlled
		**	by this structure. It dictates the starting and length of the animation
		**	frames needed for the specified state. In addition it specifies how long
		**	to delay between changes in animation. With this data it is possible to
		**	control the appearance of all normal buildings. Turrets and SAM sites are
		**	an exception since their animation is not merely cosmetic.
		*/
		typedef struct {
			int	Start;			// Starting frame of animation.
			int	Count;			// Number of frames in this animation.
			int	Rate;				// Number of ticks to delay between each frame.
		} AnimControlType;
		AnimControlType Anims[BSTATE_COUNT];

		/*
		**	This is a mask flag used to determine if all the necessary prerequisite
		**	buildings have been built.
		*/
 //		long Prerequisite;

		/*---------------------------------------------------------------------------
		**	This is the building type explicit constructor.
		*/
		BuildingTypeClass	(
						StructType type,
						int name,
						char const *ininame,
						COORDINATE exitpoint,
						unsigned char level,
						long pre,
						bool is_scanner,
						bool is_regulated,
						bool is_bibbed,
						bool is_nominal,
						bool is_wall,
						bool is_factory,
						bool is_capturable,
						bool is_flammable,
						bool is_simpledamage,
						bool is_stealthy,
						bool is_selectable,
						bool is_legal_target,
						bool is_insignificant,
						bool is_immune,
						bool is_theater,
						bool is_turret_equipped,
						bool is_twoshooter,
						bool is_repairable,
						bool is_buildable,
						bool is_crew,
						bool is_sturdy,
						RTTIType tobuild,
						DirType sframe,
						unsigned short strength,
						int sightrange,
						int cost,
						int scenario,
						int risk,
						int reward,
						int ownable,
						WeaponType primary,
						WeaponType secondary,
						ArmorType armor,
						unsigned long canenter,
						unsigned capacity,
						int power,
						int drain,
						BSizeType size,
						short const *exitlist,
						short const *sizelist,
						short const *overlap,
						bool is_unsellable=false);
		virtual RTTIType  What_Am_I(void) const {return RTTI_BUILDINGTYPE;};
		operator StructType(void) const {return(Type);};

		static BuildingTypeClass const &  As_Reference(StructType type);
		static StructType  From_Name(char const *name);
		static void  Init(TheaterType theater);
		static void One_Time(void);
		static void Prep_For_Add(void);

		int  Width(void) const;
		int  Height(void) const;

		virtual int  Cost_Of(void) const;
		virtual int  Full_Name(void) const;
		virtual COORDINATE  Coord_Fixup(COORDINATE coord) const {return coord & 0xFF00FF00L;}
		virtual int  Max_Pips(void) const;
		virtual void  Dimensions(int &width, int &height) const;
		virtual int  Legal_Placement(CELL pos) const;
		virtual bool  Create_And_Place(CELL cell, HousesType house) const;
		virtual ObjectClass *  Create_One_Of(HouseClass * house) const;
		virtual short const *  Occupy_List(bool placement=false) const;
		virtual short const *  Overlap_List(void) const;
		virtual BuildingClass *  Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const;
		virtual void const *  Get_Buildup_Data(void) const {return(BuildupData);};

		virtual int  Raw_Cost(void) const;
		virtual int  Repair_Cost(void) const;
		virtual int  Repair_Step(void) const;
		bool  Bib_And_Offset(SmudgeType & bib, CELL & cell) const;

		#ifdef SCENARIO_EDITOR
		virtual void  Display(int x, int y, WindowNumberType window, HousesType house) const;
		#endif


	private:

		/*
		**	This is a pointer to a list of offsets (from the upper left corner) that
		**	are used to indicate the building's "footprint". This footprint is used
		**	to determine building placement legality and terrain passibility.
		*/
		short const *OccupyList;

		/*
		**	Buildings can often times overlap a cell but not actually "occupy" it for
		**	purposes of movement. This points to a list of offsets that indicate which
		**	cells the building has visual overlap but does not occupy.
		*/
		short const *OverlapList;

		static BuildingTypeClass const * const Pointers[STRUCT_COUNT];

		/*
		**	The construction animation graphic data pointer is
		**	pointed to by this element.
		*/
		void const * BuildupData;

		void  Init_Anim(BStateType state, int start, int count, int rate) const;
};


/***************************************************************************
**	The various unit types need specific data that is unique to units as
**	opposed to buildings. This derived class elaborates these additional
**	data types.
*/
class UnitTypeClass : public TechnoTypeClass
{
	public:
		enum UnitTypeClassRepairEnums {
			TIBERIUM_STEP=25,					// Credits per step of Tiberium.
			STEP_COUNT=28,						// Number of steps a harvester can carry.
			FULL_LOAD_CREDITS=(TIBERIUM_STEP*STEP_COUNT),
			REPAIR_PERCENT=102,		// 40% fixed point number.
			REPAIR_STEP=4				// Number of damage points recovered per "step".
		};

		/*
		**	If this unit can appear out of a crate, then this flag will be true.
		*/
		unsigned IsCrateGoodie:1;

		/*
		**	Does this unit have only 8 facings? Special test units have limited
		**	facings.
		*/
		unsigned IsPieceOfEight:1;

		/*
		**	Can this unit squash infantry?  If it can then if the player selects
		**	an (enemy) infantry unit as the movement target, it will ride over and
		**	squish the infantry unit.
		*/
		unsigned IsCrusher:1;

		/*
		**	Does this unit go into harvesting mode when it stops on a tiberium
		**	field?  Typically, only one unit does this and that is the harvester.
		*/
		unsigned IsToHarvest:1;

		/*
		**	Does this unit's shape data consist of "chunky" facings?  This kind of unit
		**	art has the unit in only 4 facings (N, W, S, and E) and in each of those
		**	directions, the unit's turrets rotates 32 facings (counter clockwise from north).
		**	This will result in 32 x 4 = 128 unit shapes in the shape data file.
		*/
		unsigned IsChunkyShape:1;

		/*
		**	Some units are equipped with a rotating radar dish. These units have special
		**	animation processing. The rotating radar dish is similar to a turret, but
		**	always rotates and does not affect combat.
		*/
		unsigned IsRadarEquipped:1;

		/*
		**	If this unit has a firing animation, this flag is true. Infantry and some special
		**	vehicles are the ones with firing animations.
		*/
		unsigned IsFireAnim:1;

		/*
		**	Many vehicles have a turret with restricted motion. These vehicles must move the
		**	turret into a locked down position while travelling. Rocket launchers and artillery
		**	are good examples of this kind of unit.
		*/
		unsigned IsLockTurret:1;

		/*
		**	Does this unit lay tracks when it travels?  Most tracked vehicles and some wheeled
		**	vehicles have this ability.
		*/
		unsigned IsTracked:1;

		/*
		**	Is this unit of the humongous size?  Harvesters and mobile construction vehicles are
		**	of this size. If the vehicle is greater than 24 x 24 but less than 48 x 48, it is
		**	considered "Gigundo".
		*/
		unsigned IsGigundo:1;

		/*
		** Is the unit capable of cloaking?  Only Stealth Tank can do so now.
		*/
		unsigned IsCloakable:1;

		/*
		** Does this unit have a constant animation (like Visceroid?)
		*/
		unsigned IsAnimating:1;

		/*
		**	This value represents the unit class. It can serve as a unique
		**	identification number for this unit class.
		*/
		UnitType Type;

		/*
		**	This indicates the speed (locomotion) type for this unit. Through this
		**	value the movement capabilities are deduced.
		*/
		SpeedType Speed;

		/*
		**	This is the rotational speed of the unit. This value represents the
		**	turret rotation speed.
		*/
		unsigned char ROT;

		/*
		**	This is the distance along the centerline heading in the direction the body
		**	is facing used to reach the center point of the turret. This distance is
		**	in leptons.
		*/
		signed char TurretOffset;

		/*
		**	This value is used to provide the unit with a default mission order when
		**	first created. Usually, this is a resting or idle type of order.
		*/
		MissionType Mission;

		/*
		**	This is the default explosion to use when this vehicle is destroyed.
		*/
		AnimType Explosion;

		/*
		**	The width or height of the largest dimension for this unit.
		*/
		int MaxSize;

		/*
		**	This is the explicit unit class constructor.
		*/
		UnitTypeClass	(
						UnitType type,
						int name,
						char const *ininame,
						AnimType exp,
						unsigned char level,
						long pre,
						bool is_goodie,
						bool is_leader,
						bool is_eight,
						bool is_nominal,
						bool is_transporter,
						bool is_crushable,
						bool is_crusher,
						bool is_harvest,
						bool is_stealthy,
						bool is_selectable,
						bool is_legal_target,
						bool is_insignificant,
						bool is_immune,
						bool is_turret_equipped,
						bool is_twoshooter,
						bool is_repairable,
						bool is_buildable,
						bool is_crew,
						bool is_radar_equipped,
						bool is_fire_anim,
						bool is_lock_turret,
						bool is_tracked,
						bool is_gigundo,
						bool is_chunky,
						bool is_cloakable,
						bool is_animating,
						int ammo,
						unsigned short strength,
						int sightrange,
						int cost,
						int scenario,
						int risk,
						int reward,
						int ownable,
						WeaponType primary,
						WeaponType secondary,
						ArmorType armor,
						SpeedType speed,
						MPHType maxSpeed,
						unsigned rot,
						int toffset,
						MissionType order);
		virtual RTTIType  What_Am_I(void) const {return RTTI_UNITTYPE;};

		static UnitType  From_Name(char const *name);
		static UnitTypeClass const &  As_Reference(UnitType type);
		static void  Init(TheaterType );
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual void  Dimensions(int &width, int &height) const;
		virtual bool  Create_And_Place(CELL cell, HousesType house) const;
		virtual ObjectClass *  Create_One_Of(HouseClass * house) const;
		virtual short const *  Occupy_List(bool placement=false) const;
		virtual BuildingClass *  Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const;
		virtual int  Max_Pips(void) const;

		virtual int  Repair_Cost(void) const;
		virtual int  Repair_Step(void) const;

		#ifdef SCENARIO_EDITOR
		virtual void  Display(int x, int y, WindowNumberType window, HousesType house) const;
		#endif

		/*
		**	This is a pointer to the wake shape (as needed by the gunboat).
		*/
		static void const * WakeShapes;

	private:
		static UnitTypeClass const * const Pointers[UNIT_COUNT];
};


/***************************************************************************
**	The various unit types need specific data that is unique to units as
**	opposed to buildings. This derived class elaborates these additional
**	data types.
*/
class InfantryTypeClass : public TechnoTypeClass
{
	private:
		static InfantryTypeClass const * const Pointers[INFANTRY_COUNT];

	public:

		/*
		**	If this civilian infantry type is female, then this flag
		**	will be true. This information is used to get the correct
		**	voice response.
		*/
		unsigned IsFemale:1;

		/*
		**	Does this infantry unit have crawling animation? If not, then this
		**	means that the "crawling" frames are actually running animation frames.
		*/
		unsigned IsCrawling:1;

		/*
		**	For those infantry types that can capture buildings, this flag
		**	will be set to true. Typically, this is minigun soldiers.
		*/
		unsigned IsCapture:1;

		/*
		**	For infantry types that will run away from any damage causing
		**	events, this flag will be true. Typically, this is so for all
		**	civilians as well as the flame thrower guys.
		*/
		unsigned IsFraidyCat:1;

		/*
		**	This flags whether this infantry is actually a civilian. A
		**	civilian uses different voice responses, has less ammunition,
		**	and runs from danger more often.
		*/
		unsigned IsCivilian:1;

		/*
		**	This value represents the unit class. It can serve as a unique
		**	identification number for this unit class.
		*/
		InfantryType Type;

		/*
		**	This is an array of the various animation frame data for the actions that
		**	the infantry may perform.
		*/
		DoInfoStruct DoControls[DO_COUNT];

		/*
		**	There are certain units with special animation sequences built into the
		**	shape file. These values tell how many frames are used for the firing animation.
		*/
		char FireLaunch;
		char ProneLaunch;

		/*
		**	This is the explicit unit class constructor.
		*/
		InfantryTypeClass	(
						InfantryType type,
						int name,
						char const *ininame,
						unsigned char level,
						long pre,
						bool is_female,
						bool is_leader,
						bool is_crawling,
						bool is_civilian,
						bool is_nominal,
						bool is_fraidycat,
						bool is_capture,
						bool is_theater,
						int ammo,
						int *do_table,
						int firelaunch,
						int pronelaunch,
						unsigned short strength,
						int sightrange,
						int cost,
						int scenario,
						int risk,
						int reward,
						int ownable,
						WeaponType primary,
						WeaponType secondary,
						MPHType maxSpeed);
		virtual RTTIType  What_Am_I(void) const {return RTTI_INFANTRYTYPE;};

		static InfantryType  From_Name(char const *name);
		static InfantryTypeClass const &  As_Reference(InfantryType type) {return *Pointers[type];};
		static void  Init(TheaterType );
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual void  Dimensions(int &width, int &height) const {width = 12;height = 16;};
		virtual bool  Create_And_Place(CELL cell, HousesType house) const;
		virtual ObjectClass *  Create_One_Of(HouseClass * house) const;
		virtual short const *  Occupy_List(bool placement=false) const;
		virtual BuildingClass *  Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const;
		virtual int  Full_Name(void) const;

		#ifdef SCENARIO_EDITOR
		virtual void  Display(int x, int y, WindowNumberType window, HousesType house) const;
		#endif
};


/***************************************************************************
** Bullets and other projectiles need some specific information according
**	to their type.
*/
class BulletTypeClass : public ObjectTypeClass
{
	public:

		/*
		**	Does this bullet type fly over walls?
		*/
		unsigned IsHigh:1;

		/*
		**	If this projecile is one that balistically arcs from ground level, up into the air and
		**	then back to the ground, where it explodes. Typical uses of this are for grenades and
		**	artillery shells.
		*/
		unsigned IsArcing:1;

		/*
		**	If the projectile changes course as it flies in order to home in on the
		**	projectile's target, then this flag is true. Missiles are typically ones
		**	that do this.
		*/
		unsigned IsHoming:1;

		/*
		**	Certain projectiles do not travel horizontally, but rather, vertically -- they drop
		**	from a height. Bombs fall into this category and will have this value set to
		**	true. Dropping projectiles do not calculate collision with terrain (such as walls).
		*/
		unsigned IsDropping:1;

		/*
		**	Is this projectile invisible?  Some bullets and weapon effects are not directly
		**	rendered. Small caliber bullets and flame thrower flames are treated like
		**	normal projectiles for damage purposes, but are displayed using custom
		**	rules.
		*/
		unsigned IsInvisible:1;

		/*
		**	Does this bullet explode when near the target?  Some bullets only explode if
		**	it actually hits the target. Some explode even if nearby.
		*/
		unsigned IsProximityArmed:1;

		/*
		**	Does this projectile spew puffs of smoke out its tail while it
		**	travels? Missiles are prime examples of this projectile type.
		*/
		unsigned IsFlameEquipped:1;

		/*
		**	Should fuel consumption be tracked for this projectile?  Rockets are the primary
		**	projectile with this characteristic, but even for bullets it should be checked so that
		**	bullets don't travel too far.
		*/
		unsigned IsFueled:1;

		/*
		**	Is this projectile without different facing visuals?  Most plain bullets do not change
		**	visual imagery if their facing changes. Rockets, on the other hand, are equipped with
		**	the full 32 facing imagery.
		*/
		unsigned IsFaceless:1;

		/*
		**	If this is a typically inaccurate projectile, then this flag will be true. Artillery
		**	is a prime example of this type.
		*/
		unsigned IsInaccurate:1;

		/*
		**	If the bullet contains translucent pixels, then this flag will be true. These
		**	translucent pixels really are "shadow" pixels in the same style as the shadow
		**	cast by regular ground units.
		*/
		unsigned IsTranslucent:1;

		/*
		**	If this bullet can be fired on aircraft, then this flag will be true.
		*/
		unsigned IsAntiAircraft:1;

		/*
		**	This element is a unique identification number for the bullet
		**	type.
		*/
		BulletType Type;

		/*
		**	Maximum speed for this bullet type.
		*/
		MPHType MaxSpeed;

		/*
		**	This projectile has a certain style of warhead. This value specifies
		**	what that warhead type is.
		*/
		WarheadType Warhead;

		/*
		**	This is the "explosion" effect to use when this projectile impacts
		*/
		AnimType Explosion;

		/*
		**	This is the rotation speed of the bullet. It only has practical value
		**	for those projectiles that performing homing action during flight -- such
		**	as with rockets.
		*/
		unsigned char ROT;

		/*
		**	Some projectiles have a built in arming distance that must elapse before the
		**	projectile may explode. If this value is non-zero, then this override is
		**	applied.
		*/
		int Arming;

		/*
		**	Some projectiles have a built in override for distance travelled before it
		**	automatically explodes. This value, if non-zero, specifies this distance.
		*/
		int Range;

		//---------------------------------------------------------------------
		BulletTypeClass(
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
			bool is_antiair,
			int arming,
			int range,
			MPHType maxspeed,
			unsigned rot,
			WarheadType warhead,
			AnimType explosion);

		virtual RTTIType  What_Am_I(void) const {return RTTI_BULLETTYPE;};

		static BulletTypeClass const &  As_Reference(BulletType type) {return *Pointers[type];};
		static void  Init(TheaterType ) {};
		static void One_Time(void);

		virtual bool  Create_And_Place(CELL , HousesType =HOUSE_NONE) const {return false;};
		virtual ObjectClass *  Create_One_Of(HouseClass *) const {return 0;};

	private:
		static BulletTypeClass const * const Pointers[BULLET_COUNT];
};


/****************************************************************************
**	These are the different TYPES of terrain objects. Every terrain object must
**	be one of these types.
*/
class TerrainTypeClass : public ObjectTypeClass
{
	public:
		/*
		**	Which terrain object does this class type represent.
		*/
		TerrainType Type;

		/*
		**	Does this terrain element consist of a normal frame followed by a
		**	series of crumble frames?  Trees fall under this case.
		*/
		unsigned IsDestroyable:1;

		/*
		** Does this object have the capability to transform after a period
		** of time (such as a blossom tree?
		*/
		unsigned IsTransformable:1;

		/*
		**	Does this terrain object spawn the growth of Tiberium? Blossom trees are
		**	a good example of this.
		*/
		unsigned IsTiberiumSpawn:1;

		/*
		**	This is the fully translated name for the terrain element.
		*/
		short FullName;

		/*
		**	This is the coordinate offset (from upper left) of where the center base
		**	position of the terrain object lies. For trees, this would be the base of
		**	the trunk. This is used for sorting purposes.
		*/
		COORDINATE CenterBase;

		/*
		**	This is the bitfield control that tells which theater this terrain object is
		**	valid for. If the bit (1 << TheaterType) is true, then this terrain object
		**	is allowed.
		*/
		unsigned char Theater;

		//----------------------------------------------------------------
		TerrainTypeClass(
				TerrainType terrain,
				int theater,
				COORDINATE centerbase,
				bool is_spawn,
				bool is_destroyable,
				bool is_transformable,
				bool is_flammable,
				bool is_crushable,
				bool is_selectable,
				bool is_legal_target,
				bool is_insignificant,
				bool is_immune,
				char const *ininame,
				int fullname,
				unsigned short strength,
				ArmorType armor,
				short const *occupy,
				short const *overlap);
		virtual RTTIType  What_Am_I(void) const {return RTTI_TERRAINTYPE;};

		static TerrainType  From_Name(char const*name);
		static TerrainTypeClass const &  As_Reference(TerrainType type) {return *Pointers[type];};
		static void  Init(TheaterType theater = THEATER_TEMPERATE);
		static void One_Time(void) {};
		static void Prep_For_Add(void);

		virtual COORDINATE  Coord_Fixup(COORDINATE coord) const {return coord & 0xFF00FF00L;}
		virtual bool  Create_And_Place(CELL cell, HousesType house) const;
		virtual ObjectClass *  Create_One_Of(HouseClass *) const;
		virtual short const *  Occupy_List(bool placement=false) const;
		virtual short const *  Overlap_List(void) const;

		#ifdef SCENARIO_EDITOR
		virtual void  Display(int x, int y, WindowNumberType window, HousesType house=HOUSE_NONE) const;
		#endif

	private:
		short const *Occupy;
		short const *Overlap;

		static TerrainTypeClass const * const Pointers[TERRAIN_COUNT];
};


/****************************************************************************
**	The tile type objects are controlled by this class. It specifies the form
**	of the tile set for the specified object as well as other related datum.
**	It is derived from the ObjectTypeClass solely for the purpose of scenario
**	editing and creation.
*/
class TemplateTypeClass: public ObjectTypeClass
{
	public:
		/*
		**	What template is this.
		*/
		TemplateType Type;

		/*
		**	A bitfield container that indicates which theaters this template is allowed
		**	in. A bit set in the (1<<TheaterType) position indicates the template is legal
		**	in that particular theater.
		*/
		unsigned char Theater;

		/*
		**	The icons of this template default to this specified
		**	land type.
		*/
		LandType Land;

		/*
		**	This is the fully translated name for the terrain element.
		*/
		int FullName;

		/*
		**	Raw dimensions of this template (in icons).
		*/
		unsigned char Width,Height;

		LandType AltLand;
		char const *AltIcons;

		//----------------------------------------------------------
		TemplateTypeClass(
			TemplateType iconset,
			int theater,
			char const *ininame,
			int fullname,
			LandType land,
			int width,
			int height,
			LandType altland,
			char const *alticons);
		virtual RTTIType  What_Am_I(void) const {return RTTI_TEMPLATETYPE;};

		static TemplateType  From_Name(char const *name);
		static TemplateTypeClass const &  As_Reference(TemplateType type) {return *Pointers[type];};
		static void  Init(TheaterType theater);
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual COORDINATE  Coord_Fixup(COORDINATE coord) const {return coord & 0xFF00FF00L;}
		virtual bool  Create_And_Place(CELL cell, HousesType house=HOUSE_NONE) const;
		virtual ObjectClass *  Create_One_Of(HouseClass *) const;
		virtual short const *  Occupy_List(bool placement=false) const;

		#ifdef SCENARIO_EDITOR
		virtual void  Display(int x, int y, WindowNumberType window, HousesType house=HOUSE_NONE) const;
		#endif

	private:
		static TemplateTypeClass const * const Pointers[TEMPLATE_COUNT];
};


/****************************************************************************
**	All the animation objects are controlled by this class. It holds the static
**	data associated with each animation type.
*/
class AnimTypeClass : public ObjectTypeClass
{
	public:

		/*
		**	If this animation should run at a constant apparent rate regardless
		**	of game speed setting, then this flag will be set to true.
		*/
		unsigned IsNormalized:1;

		/*
		**	If this animation should be rendered and sorted with the other ground
		**	units, then this flag is true. Typical of this would be fire and other
		**	low altitude animation effects.
		*/
		unsigned IsGroundLayer:1;

		/*
		**	If this animation should be rendered in a translucent fashion, this flag
		**	will be true. Translucent colors are some of the reds and some of the
		**	greys. Typically, smoke and some fire effects have this flag set.
		*/
		unsigned IsTranslucent:1;

		/*
		**	If this animation uses the white translucent table, then this flag
		**	will be true.
		*/
		unsigned IsWhiteTrans:1;

		/*
		**	If this is the special flame thrower animation, then custom affects
		**	occur as it is playing. Specifically, scorch marks and little fire
		**	pieces appear as the flame jets forth.
		*/
		unsigned IsFlameThrower:1;

		/*
		**	Some animations leave a scorch mark behind. Napalm and other flame
		**	type explosions are typical of this type.
		*/
		unsigned IsScorcher:1;

		/*
		**	Some explosions are of such violence that they leave craters behind.
		**	This flag will be true for those types.
		*/
		unsigned IsCraterForming:1;

		/*
		**	If this animation should attach itself to any unit that is in the same
		**	location as itself, then this flag will be true. Most vehicle impact
		**	explosions are of this type.
		*/
		unsigned IsSticky:1;

		/*
		**	This is the type number for this animation kind. It can be used as
		**	a unique identifier for animation types.
		*/
		AnimType Type;

		/*
		**	This specified the maximum dimension of the shape (edge to edge). This dimension
		**	is used to build the appropriate cell refresh list. Keep this value as small
		**	as possible to ensure maximum performance. This is especially critical, since
		**	animations always cause the cells under them to be redrawn every frame.
		*/
		int Size;

		/*
		**	This is the frame that the animation is biggest. The biggest frame of animation
		**	will hide any changes to underlying ground (e.g., craters) that the animation
		**	causes, so these effects are delayed until this frame is reached. The end result
		**	is to prevent the player from seeing craters "pop" into existance.
		*/
		int Biggest;

		/*
		**	Some animations (when attached to another object) damage the object it
		**	is in contact with. Fire is a good example of this. This value is a
		**	fixed point number of the damage that is applied to the attached object
		**	every game tick.
		*/
		unsigned int Damage;

		/*
		**	Simple animation delay value between advancing of frames. This can
		**	be overridden by the control list.
		*/
		unsigned char Delay;

		/*
		**	The starting frame number for each animation sequence. Usually this is
		**	zero, but can sometimes be different if this animation is a sub sequence
		**	of a larger animation file.
		*/
		int Start;

		/*
		**	Looping animations might start at a different frame than the initial one.
		**	This is true for smoke effects that have a startup sequence followed by a
		**	continuous looping sequence.
		*/
		int LoopStart;

		/*
		**	For looping animations, this is the frame that will end all the middle loops
		**	of the animation. The last loop of the animation will proceed until the Stages
		**	has been fully completed.
		*/
		int LoopEnd;

		/*
		**	The number of stages that this animation sequence will progress through
		**	before it loops or ends.
		*/
		int Stages;

		/*
		**	This is the normal loop count for this animation. Usually this is one, but
		**	for some animations, it may be larger.
		*/
		unsigned char Loops;

		/*
		**	This is the sound effect to play when this animation starts. Usually, this
		**	applies to explosion animations.
		*/
		VocType Sound;

		/*
		**	If the animation is to launch into another animation, then
		**	the secondary animation will be defined here.
		*/
		AnimType ChainTo;

		/*
		**	The number of virtual animation stages. Keeps the animation alive longer for the purpose
		**  of 4K rendering, but is ignored by legacy rendering.
		*/
		int VirtualStages;

		/*
		**	The scale of the virtual animation. Allows for higher-resolution art to appear larger.
		*/
		int VirtualScale;

		/*
		**	Animation data to use specifically for virtual rendering (implies this animation only for legacy).
		*/
		AnimType VirtualAnim;

		//---------------------------------------------------------------------------
		AnimTypeClass(AnimType anim,
							char const *name,
							int size,
							int biggest,
							bool isnormal,
							bool iswhite,
							bool isscorcher,
							bool iscrater,
							bool issticky,
							bool ground,
							bool istrans,
							bool isflame,
							unsigned int damage,
							int delaytime,
							int start,
							int loopstart,
							int loopend,
							int stages,
							int loops,
							VocType sound,
							AnimType chainto,
							int virtualstages=-1,
							int virtualscale=0x100,
							AnimType virtualanim=ANIM_NONE);
		virtual RTTIType  What_Am_I(void) const {return RTTI_ANIMTYPE;};

		static AnimTypeClass const &  As_Reference(AnimType type) {return *Pointers[type];};
		static void Init(TheaterType) {};
		static void One_Time(void);

		virtual bool  Create_And_Place(CELL , HousesType =HOUSE_NONE) const {return false;};
		virtual ObjectClass *  Create_One_Of(HouseClass *) const {return 0;};

	private:
		static AnimTypeClass const * const Pointers[ANIM_COUNT];
};



/****************************************************************************
**	The various aircraft types are controlled by this list.
*/
class AircraftTypeClass : public TechnoTypeClass
{
	enum UnitTypeClassRepairEnums {
		REPAIR_PERCENT=102,		// 40% fixed point number.
		REPAIR_STEP=2				// Number of damage points recovered per "step".
	};
	public:

		/*
		**	Fixed wing aircraft (ones that cannot hover) have this flag set to true.
		**	Such aircraft will not vary speed while it is flying.
		*/
		unsigned IsFixedWing:1;

		/*
		**	Can this aircraft land?  If it can land it is presumed to be controllable by the player.
		*/
		unsigned IsLandable:1;

		/*
		**	Does this aircraft have a rotor blade (helicopter) type propulsion?
		*/
		unsigned IsRotorEquipped:1;	// Is a rotor attached?

		/*
		**	Is there a custom rotor animation stage set for each facing of the aircraft?
		*/
		unsigned IsRotorCustom:1;	// Custom rotor sets for each facing?

		AircraftType Type;
		unsigned char ROT;
		MissionType Mission;

		AircraftTypeClass(
				AircraftType airtype,
				int name,
				char const *ininame,
				unsigned char level,
				long pre,
				bool is_leader,
				bool is_twoshooter,
				bool is_transporter,
				bool is_fixedwing,
				bool is_rotorequipped,
				bool is_rotorcustom,
				bool is_landable,
				bool is_crushable,
				bool is_stealthy,
				bool is_selectable,
				bool is_legal_target,
				bool is_insignificant,
				bool is_immune,
				bool is_theater,
				bool is_repairable,
				bool is_buildable,
				bool is_crew,
				int ammo,
				unsigned short strength,
				int sightrange,
				int cost,
				int scenario,
				int risk,
				int reward,
				int ownable,
				WeaponType primary,
				WeaponType secondary,
				ArmorType armor,
				MPHType MaxSpeed,
				int ROT,
				MissionType deforder);
		virtual RTTIType  What_Am_I(void) const;

		static AircraftType  From_Name(char const *name);
		static AircraftTypeClass const &  As_Reference(AircraftType a) {return *Pointers[a];};
		static void  Init(TheaterType);
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual int  Repair_Cost(void) const;
		virtual int  Repair_Step(void) const;
		virtual void  Dimensions(int &width, int &height) const;
		virtual bool  Create_And_Place(CELL, HousesType) const;
		virtual ObjectClass *  Create_One_Of(HouseClass * house) const;
		virtual short const *  Occupy_List(bool placement=false) const;
		virtual short const *  Overlap_List(void) const;
		virtual BuildingClass *  Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const;
		virtual int  Max_Pips(void) const;

		#ifdef SCENARIO_EDITOR
		virtual void  Display(int x, int y, WindowNumberType window, HousesType house) const;
		#endif

		static void const * LRotorData;
		static void const * RRotorData;

	private:
		static AircraftTypeClass const * const Pointers[AIRCRAFT_COUNT];
};


/****************************************************************************
**	This controls the overlay object types. These object types include walls
**	and concrete. They are always considered to be one icon in size and
**	are processed on an icon by icon basis. This is different from normal
**	templates which can be an arbitrary size. Other than this they are
**	mostly similar to normal templates but with some characteristics of
**	structures (they can be destroyed).
*/
class OverlayTypeClass: public ObjectTypeClass
{
	public:
		/*
		**	What overlay is this.
		*/
		OverlayType Type;

		/*
		**	This is the fully translated name for the terrain element.
		*/
		int FullName;

		/*
		**	What type of ground does this make the cell it occupies?
		*/
		LandType Land;

		/*
		** If this overlay is a wall, how many stages of destruction are there
		** for this wall type? i.e. sandbags = 2, concrete = 4, etc.
		*/
		int DamageLevels;

		/*
		** If this overlay is a wall, what amount of damage is necessary
		** before the wall takes damage?
		*/
		int DamagePoints;

		/*
		**	Is this overlay graphic theater specific. This means that if there is
		**	custom art for this overlay that varies between different theaters, then
		**	this flag will be true.
		*/
		unsigned IsTheater:1;

		/*
		**	Is this a wall type overlay?  Wall types change their shape
		**	depending on the existence of adjacent walls of the same type.
		*/
		unsigned IsWall:1;

		/*
		**	If this overlay is actually a wall and this wall type is tall enough that
		**	normal ground based straight line weapons will be blocked by it, then this
		**	flag will be true. Brick fences are typical of this type.
		*/
		unsigned IsHigh:1;

		/*
		**	If this overlay represents harvestable tiberium, then this flag
		**	will be true.
		*/
		unsigned IsTiberium:1;

		/*
		**	If this is a wall that is made of wood, then this flag will be
		**	true. Such walls are affected by fire damage.
		*/
		unsigned IsWooden:1;

		/*
		**	Is this a crate? If it is, then goodies may come out of it.
		*/
		unsigned IsCrate:1;

		/*
		**	If this is true, then the overlay will not show up on the radar map.
		*/
		unsigned IsRadarVisible:1;

		//----------------------------------------------------------
		OverlayTypeClass(
			OverlayType iconset,
			char const *ininame,
			int  fullname,
			LandType ground,
			int  damagelevels,
			int  damagepoints,
			bool isradarinvisible,
			bool iswooden,
			bool istarget,
			bool iscrushable,
			bool istiberium,
			bool high,
			bool theater,
			bool iswall,
			bool iscrate);
		virtual RTTIType  What_Am_I(void) const {return RTTI_OVERLAYTYPE;};

		static OverlayType  From_Name(char const *name);
		static OverlayTypeClass const &  As_Reference(OverlayType type) {return *Pointers[type];};
		static void  Init(TheaterType);
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual COORDINATE  Coord_Fixup(COORDINATE coord) const {return coord & 0xFF00FF00L;}
		virtual bool  Create_And_Place(CELL cell, HousesType house=HOUSE_NONE) const;
		virtual ObjectClass *  Create_One_Of(HouseClass *) const;
		virtual short const *  Occupy_List(bool placement=false) const;
		virtual void  Draw_It(int x, int y, int data) const;
		virtual unsigned char *  Radar_Icon(int data) const;

		#ifdef SCENARIO_EDITOR
		virtual void  Display(int x, int y, WindowNumberType window, HousesType house=HOUSE_NONE) const;
		#endif

	private:
		static OverlayTypeClass const * const Pointers[OVERLAY_COUNT];
};


/****************************************************************************
**	This type elaborates the various "smudge" effects that can occur. Smudges are
**	those elements which are on top off all the ground icons, but below anything
**	that is "above" it. This includes scorch marks, craters, and infantry bodies.
**	Smudges, be definition, contain transparency. The are modifiers to underlying
**	terrain imagery.
*/
class SmudgeTypeClass : public ObjectTypeClass
{
	public:
		/*
		**	What overlay is this.
		*/
		SmudgeType Type;

		/*
		**	This is the fully translated smudge name.
		*/
		int FullName;

		/*
		**	Some smudges are larger than one cell. If this is the case, then
		**	these dimensions specify the number of cells wide and tall the
		**	smudge is.
		*/
		int Width;
		int Height;

		/*
		**	Is this smudge a crater type? If so, then a second crater can be added to
		**	this smudge so that a more cratered landscape results.
		*/
		unsigned IsCrater:1;

		/*
		**	Is this overlay used as the attached road piece for buildings (bib)?
		*/
		unsigned IsBib:1;

		//----------------------------------------------------------
		SmudgeTypeClass(
			SmudgeType smudge,
			char const *ininame,
			int fullname,
			int width,
			int height,
			bool isbib,
			bool iscrater
			);
		virtual RTTIType  What_Am_I(void) const {return RTTI_SMUDGETYPE;};

		static SmudgeType  From_Name(char const *name);
		static SmudgeTypeClass const &  As_Reference(SmudgeType type) {return *Pointers[type];};
		static void  Init(TheaterType);
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual bool  Create_And_Place(CELL cell, HousesType house=HOUSE_NONE) const;
		virtual ObjectClass *  Create_One_Of(HouseClass *) const;
		virtual short const *  Occupy_List(bool placement=false) const;
		virtual short const *  Overlap_List(void) const {return Occupy_List();};
		virtual void  Draw_It(int x, int y, int data) const ;

		#ifdef SCENARIO_EDITOR
		virtual void  Display(int x, int y, WindowNumberType window, HousesType house=HOUSE_NONE) const;
		#endif

	private:
		static SmudgeTypeClass const * const Pointers[SMUDGE_COUNT];
};

#endif
