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

/* $Header: /CounterStrike/TYPE.H 1     3/03/97 10:26a Joe_bostic $ */
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
class WeaponTypeClass;


/***************************************************************************
**	This is the abstract type class. It holds information common to all
**	objects that might exist. This contains the name of the object type.
*/
class AbstractTypeClass
{
	public:

		/*
		**	This serves to identify the object class. The ID corresponds to the
		**	variation number (e.g., UNIT_TANK1, UNIT_TANK2, etc.).
		*/
		RTTIType RTTI;
		int ID;

		/*
		**	This is the internal control name of the object. This name does
		**	not change regardless of language specified. This is the name
		**	used in scenario control files and for other text based unique
		**	identification purposes.
		*/
		char IniName[24];

		/*
		**	The translated (language specific) text name number of this object.
		**	This number is used to fetch the object's name from the language
		**	text file. Whenever the name of the object needs to be displayed,
		**	this is used to determine the text string.
		*/
		int FullName;

		AbstractTypeClass(RTTIType rtti, int id, int name, char const * ini);
		AbstractTypeClass(NoInitClass const & ) {};
		~AbstractTypeClass(void) {};

		RTTIType What_Am_I(void) const {return(RTTI);};
		TARGET As_Target(void) const {return(Build_Target(RTTI, ID));};

		virtual COORDINATE Coord_Fixup(COORDINATE coord) const;
		virtual int Full_Name(void) const;
		char const * Name(void) const {return(IniName);}
		void Set_Name(char const * buf) const {
			strncpy((char *)IniName, buf, sizeof(IniName));
			((char &)IniName[sizeof(IniName)-1]) = '\0';
		};
		virtual int Get_Ownable(void) const;

		void Code_Pointers(void) {}
		void Decode_Pointers(void) {}
};


/**********************************************************************
**	Each house has certain unalienable characteristics. This structure
**	elaborates these.
*/
class HouseTypeClass : public AbstractTypeClass
{
	public:
		/*
		**	This is the house number (enum). This is a unique identification
		**	number for the house.
		*/
		HousesType House;

		/*
		**	This is the filename suffix to use when creating a house specific
		**	file name. It is three characters long.
		*/
		char Suffix[_MAX_EXT];

		/*
		**	This is the "lemon percentage" to use when determining if a particular
		**	object owned by this house is to be flagged as a "lemon". Objects so
		**	flagged have a greater break-down chance. The percentage is expressed
		**	as a fixed point number with 0x000 meaning 0% and 0x100 meaning 100%.
		*/
		unsigned Lemon;

		/*
		**	This points to the default remap table for this house.
		*/
		PlayerColorType RemapColor;

		/*
		**	This is a unique ASCII character used when constructing filenames. It
		**	serves a similar purpose as the "Suffix" element, but is only one
		**	character long.
		*/
		char Prefix;

		/*
		**	This controls the various general adjustments to the house owned
		**	unit and building ratings. The default value for these ratings is
		**	a fixed point number of 1.0.
		*/
		fixed FirepowerBias;
		fixed GroundspeedBias;
		fixed AirspeedBias;
		fixed ArmorBias;
		fixed ROFBias;
		fixed CostBias;
		fixed BuildSpeedBias;

		//------------------------------------------------------------------------
		HouseTypeClass(NoInitClass const & x) : AbstractTypeClass(x) {}
		HouseTypeClass(HousesType house,
							char const * ini,
							int fullname,
							char const * ext,
							int lemon,
							PlayerColorType remapcolor,
							char prefix);

		unsigned char const * Remap_Table(void) const;

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static HousesType From_Name(char const * name);
		static HouseTypeClass & As_Reference(HousesType house);
		static void One_Time(void);
		static void Init_Heap(void);

		virtual bool Read_INI(CCINIClass & ini);
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
		**	This is the base name of the graphic data associated with this object
		**	type. If the graphic name is a null string, then there is no graphic
		**	associated with this object type.
		*/
		char GraphicName[_MAX_FNAME];

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
		**	"Sentient" objects are ones that have logic AI processing performed on them. All
		**	vehicles, buildings, infantry, and aircraft are so flagged. Terrain elements also
		**	fall under this category, but only because certain animation effects require this.
		*/
		unsigned IsSentient:1;

		/*
		**	If this object type affects the occupation and collision logic associated with
		**	cells, then this flag will be true. Typically, this characteristic is limited
		**	to buildings, units, terrain objects, and landed aircraft.
		*/
		unsigned IsFootprint:1;

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
		**	Points to the dimension data for each shape in the image list. By using this
		**	data, the minimum number of cells will be redrawn when the object changes shape.
		*/
		Rect * DimensionData;

		/*
		**	This points to the radar imagery for this object.
		*/
		void const * RadarIcon;

		//--------------------------------------------------------------------
		ObjectTypeClass(NoInitClass const & x) : AbstractTypeClass(x) {}
		ObjectTypeClass(	RTTIType rtti,
								int id,
								bool is_sentient,
								bool is_stealthy,
								bool is_selectable,
								bool is_legal_target,
								bool is_insignificant,
								bool is_immune,
								bool is_footprint,
								int fullname,
								char const * name
								);

		static void One_Time(void);

		bool Is_Foot(void) const {return(RTTI == RTTI_INFANTRYTYPE || RTTI == RTTI_UNITTYPE || RTTI == RTTI_VESSELTYPE || RTTI == RTTI_AIRCRAFTTYPE);};
		char const * Graphic_Name(void) const {if (GraphicName[0] != '\0') return(GraphicName); return(Name());}
		virtual int Max_Pips(void) const;
		virtual void Dimensions(int &width, int &height) const;
		virtual bool Create_And_Place(CELL , HousesType =HOUSE_NONE) const = 0;
		virtual int Cost_Of(void) const;
		virtual int Time_To_Build(void) const;
		virtual ObjectClass * Create_One_Of(HouseClass *) const = 0;
		virtual short const * Occupy_List(bool placement=false) const;
		virtual short const * Overlap_List(void) const;
		virtual BuildingClass * Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const;
		virtual void const * Get_Cameo_Data(void) const;
		void const * Get_Image_Data(void) const {return ImageData;};
		void const * Get_Radar_Data(void) const {return RadarIcon;};

		#ifdef SCENARIO_EDITOR
		virtual void Display(int, int, WindowNumberType, HousesType) const {};
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
		**	This controls how this object type is remapped when it is displayed
		**	in the sidebar.
		*/
		RemapType Remap;

		/*
		**	Is this object ownable by all sides in a multiplayer game? There are some
		**	special case objects that need this override ability.
		*/
		unsigned IsDoubleOwned:1;

		/*
		**	If this object should be completely and always invisible to the enemy, then
		**	this flag will be true.
		*/
		unsigned IsInvisible:1;

		/*
		**	If this object can serve as a good leader for a group selected
		**	series of objects, then this flag will be true. Unarmed or
		**	ability challenged units do not make good leaders. This flag is
		**	also used to indicate the primary factory when dealing with
		**	buildings.
		*/
		unsigned IsLeader:1;

		/*
		**	Does this object have the ability to detect the presence of a nearby
		**	cloaked object?
		*/
		unsigned IsScanner:1;

		/*
		**	If this object is always given its proper name rather than a generic
		**	name, then this flag will be true. Typically, civilians and Dr. Moebius
		**	fall under this category.
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
		**	Certain objects can be repaired. For buildings, they repair "in place". For units,
		**	they must travel to a repair center to be repaired. If this flag is true, then
		**	allow the player or computer AI to repair the object.
		*/
		unsigned IsRepairable:1;

		/*
		**	Does this object contain a crew?  If it does, then when the object is destroyed, there
		**	is a distinct possibility that infantry will "pop out". Only units with crews can
		**	become "heros".
		*/
		unsigned IsCrew:1;

		/*
		**	This tells whether this unit should EVER be remapped when it is displayed
		**	on the tactical map. Normally, the unit is remapped, but for certain civilian
		**	object, remapping is not to be performed, regardless of owner.
		*/
		unsigned IsRemappable:1;

		/*
		** Is the unit capable of cloaking?  Only Stealth Tank can do so now.
		*/
		unsigned IsCloakable:1;

		/*
		**	Can this object self heal up to half strength? Mammoth tanks from C&C had this
		**	feature.
		*/
		unsigned IsSelfHealing:1;

		/*
		**	If this object explodes violently when destroyed, then this flag will be true.
		**	The type of explosion is based on the warhead type and the damage generated
		**	corresponds to the full strength of the object.
		*/
		unsigned IsExploding:1;

		/*
		**	This specifies the zone that an object of this type should recognize. Zones
		**	of this type or lower will be considered "possible to travel to".
		*/
		MZoneType MZone;

		/*
		**	When determining threat, the range can be overridden to be the value
		**	specified here. Otherwise, the range for enemy scan is equal to the
		**	longest weapon range the object has. If the value is zero, then the
		**	weapon range is used.
		*/
		LEPTON ThreatRange;

		/*
		**	If this is a transporter object (e.g., hovercraft, chinook, APC), then this
		**	value specifies the maximum number of passengers it may carry.
		*/
		int MaxPassengers;

		/*
		**	Most objects have the ability to reveal the terrain around themselves.
		**	This sight range (expressed in cell distance) is specified here. If
		**	this value is 0, then this unit never reveals terrain. Bullets are
		**	typically of this nature.
		*/
		int SightRange;

		/*
		**	This is the credit cost to produce this object (presuming production is
		**	allowed).
		*/
		int Cost;

		/*
		**	The tech level that this object can be produced at.
		*/
		unsigned Level;

		/*
		**	This specifies the building prerequisites required before an object
		**	of this type can be produced.
		*/
		long Prerequisite;

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
		**	This indicates the speed (locomotion) type for this unit. Through this
		**	value the movement capabilities are deduced.
		*/
		SpeedType Speed;

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
		long Ownable;

		/*
		**	This is the small icon image that is used to display the object in
		**	the sidebar for construction selection purposes.
		*/
		void const * CameoData;

		/*
		**	The number of animation frames allotted to rotation is specified here.
		**	For an object that has no rotation, this value will be 1. For normal
		**	vehicles this value will be 32. There are some special case units that
		**	have intermediate rotation frames.
		*/
		int Rotation;

		/*
		**	This is the rotational speed of the object. This value represents the
		**	turret or body rotation speed expresses as 360/256ths rotation steps per
		**	game tick.
		*/
		int ROT;

		/*
		**	These are the weapons that this techno object is armed with.
		*/
		WeaponTypeClass const * PrimaryWeapon;
		WeaponTypeClass const * SecondaryWeapon;

		/*
		**	These specify the lepton offsets to locate the exact coordinate of the
		**	'tip of the barrel' for the weapon. This is used for generating the bullet
		**	at the proper location.
		*/
		int HorizontalOffset;		// Distance to move east (compensates for offsets in animation from center coordinate).
		int VerticalOffset;			// Distance to move north (compensates for perspective).
		int PrimaryOffset;			// Offset along turret centerline and facing.
		int PrimaryLateral;			// Sideways offset from turret centerline and facing.
		int SecondaryOffset;
		int SecondaryLateral;

		/*
		** Points you're awarded for destroying an object of this type, and
		** points you lose if you lose an object of this type.
		*/
		int Points;

		//--------------------------------------------------------------------
		TechnoTypeClass(NoInitClass const & x) : ObjectTypeClass(x) {}
		TechnoTypeClass(
				RTTIType rtti,
				int id,
				int name,
				char const * ininame,
				RemapType remap,
				int verticaloffset,
				int primaryoffset,
				int primarylateral,
				int secondaryoffset,
				int secondarylateral,
				bool is_nominal,
				bool is_stealthy,
				bool is_selectable,
				bool is_legal_target,
				bool is_insignificant,
				bool is_immune,
				bool is_theater,
				bool is_turret_equipped,
				bool is_remappable,
				bool is_footprint,
				int rotation,
				SpeedType speed,
				int horizontaloffset=0
				);

		bool Is_Two_Shooter(void) const;
		int Legal_Placement(CELL pos) const;
		virtual int Raw_Cost(void) const;
		virtual int Max_Passengers(void) const {return(MaxPassengers);}
		virtual int Repair_Cost(void) const;
		virtual int Repair_Step(void) const;
		virtual void const * Get_Cameo_Data(void) const;
		virtual int Cost_Of(void) const;
		virtual int Time_To_Build(void) const;
		virtual int Get_Ownable(void) const;
		virtual bool Read_INI(CCINIClass & ini);

		/*
		**	This is a pointer to the wake shape (as needed by the gunboat).
		*/
		static void const * WakeShapes;
		static void const * TurretShapes;
		static void const * SamShapes;
		static void const * MGunShapes;
};


/***************************************************************************
**	Building types need some special information custom to buildings. This
**	is a derived class that elaborates these additional data elements.
*/
class BuildingTypeClass : public TechnoTypeClass {
	public:
		/*
		**	Is this building allowed to be considered for building adjacency
		**	checking? If false, then building off of (or adjacent to) this building
		**	is not considered.
		*/
		unsigned IsBase:1;

		/*
		** If this building is a fake, this flag will be set.
		*/
		unsigned IsFake:1;

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
		**	Buildings can have either simple or complex damage stages. If simple,
		**	then the second to the last frame is the half damage stage, and the last
		**	frame is the complete damage stage. For non-simple damage, buildings
		**	have a complete animation set for damaged as well as undamaged condition.
		**	Turrets, oil pumps, and repair facilities are a few examples.
		*/
		unsigned IsSimpleDamage:1;

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
		**	Does this building require power to function? Usually, this isn't the case. The building
		**	normally either has no effect by power level or is gradually reduced in effectiveness. This
		**	flag is for those buildings that completely cease to function when the power drops below
		**	full.
		*/
		unsigned IsPowered:1;

		/*
		**	If this flag is true, then the building cannot be sold even if it could have been built. This
		**	is especially useful for mines which can be built but cannot be sold.
		*/
		unsigned IsUnsellable:1;

		/*
		**	This is the direction (from the center cell) of the building in order to find a
		**	legitimate foundation square. This location will be used for targeting and capture
		**	move destination purposes.
		*/
		FacingType FoundationFace;

		/*
		**	Adjacent distance for building next to.
		*/
		int Adjacent;

		/*
		**	This flag specifies the type of object this factory building can "produce". For non
		**	factory buildings, this value will be RTTI_NONE.
		*/
		RTTIType ToBuild;

		/*
		**	For building that produce ground units (infantry and vehicles), there is a default
		**	exit point defined. This point is where the object is first placed on the map.
		**	Typically, this is located next to a door. The unit will then travel on to a clear
		**	terrain area and enter normal game processing.
		*/
		COORDINATE ExitCoordinate;

		/*
		**	When determine which cell to head toward when exiting a building, use the
		**	list elaborated by this variable. There are directions of exit that are
		**	more suitable than others. This list is here to inform the system which
		**	directions those are.
		*/
		short const * ExitList;

		/*
		**	This is the structure type identifier. It can serve as a unique
		**	identification number for building types.
		*/
		StructType Type;

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
		int Capacity;

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

		/*---------------------------------------------------------------------------
		**	This is the building type explicit constructor.
		*/
		BuildingTypeClass(NoInitClass const & x) : TechnoTypeClass(x) {}
		BuildingTypeClass	(
						StructType type,
						int name,
						char const * ininame,
						FacingType foundation,
						COORDINATE exitpoint,
						RemapType remap,
						int verticaloffset,
						int primaryoffset,
						int primarylateral,
						bool is_fake,
						bool is_regulated,
						bool is_nominal,
						bool is_wall,
						bool is_simpledamage,
						bool is_stealthy,
						bool is_selectable,
						bool is_legal_target,
						bool is_insignificant,
						bool is_theater,
						bool is_turret_equipped,
						bool is_remappable,
						RTTIType tobuild,
						DirType sframe,
						BSizeType size,
						short const * exitlist,
						short const * sizelist,
						short const * overlap
						);
		operator StructType(void) const {return(Type);};

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static void Init_Heap(void);
		static BuildingTypeClass & As_Reference(StructType type);
		static StructType From_Name(char const * name);
		static void Init(TheaterType theater);
		static void One_Time(void);
		static void Prep_For_Add(void);

		int Width(void) const;
		int Height(bool bib=false) const;

		virtual int Full_Name(void) const;
		virtual bool Read_INI(CCINIClass & ini);
		bool Flush_For_Placement(CELL cell, HouseClass * house) const;
		virtual int Cost_Of(void) const;
		virtual COORDINATE Coord_Fixup(COORDINATE coord) const;
		virtual int Max_Pips(void) const;
		virtual void Dimensions(int &width, int &height) const;
		virtual bool Create_And_Place(CELL cell, HousesType house) const;
		virtual ObjectClass * Create_One_Of(HouseClass * house) const;
		virtual short const * Occupy_List(bool placement=false) const;
		virtual short const * Overlap_List(void) const;
		virtual void const * Get_Buildup_Data(void) const {return(BuildupData);};

		bool Is_Factory(void) const {return(ToBuild != RTTI_NONE);}
		virtual int Raw_Cost(void) const;
		bool Bib_And_Offset(SmudgeType & bib, CELL & cell) const;

		#ifdef SCENARIO_EDITOR
		virtual void Display(int x, int y, WindowNumberType window, HousesType house) const;
		#endif

		/*
		**	Special overlay for the weapons factory.
		*/
		static void const * WarFactoryOverlay;

	private:

		/*
		**	This is a pointer to a list of offsets (from the upper left corner) that
		**	are used to indicate the building's "footprint". This footprint is used
		**	to determine building placement legality and terrain passibility.
		*/
		short const * OccupyList;

		/*
		**	Buildings can often times overlap a cell but not actually "occupy" it for
		**	purposes of movement. This points to a list of offsets that indicate which
		**	cells the building has visual overlap but does not occupy.
		*/
		short const * OverlapList;

		/*
		**	The construction animation graphic data pointer is
		**	pointed to by this element.
		*/
		void const * BuildupData;

		void Init_Anim(BStateType state, int start, int count, int rate) const;
};


/***************************************************************************
**	The various unit types need specific data that is unique to units as
**	opposed to buildings. This derived class elaborates these additional
**	data types.
*/
class UnitTypeClass : public TechnoTypeClass
{
	public:
		/*
		**	If this unit can appear out of a crate, then this flag will be true.
		*/
		unsigned IsCrateGoodie:1;

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
		**	Is this unit of the humongous size?  Harvesters and mobile construction vehicles are
		**	of this size. If the vehicle is greater than 24 x 24 but less than 48 x 48, it is
		**	considered "Gigundo".
		*/
		unsigned IsGigundo:1;

		/*
		** Does this unit have a constant animation (like Visceroid?)
		*/
		unsigned IsAnimating:1;

		/*
		** Does this unit have the ability to jam radar facilities?
		*/
		unsigned IsJammer:1;

		/*
		** Is this unit a mobile gap generator?
		*/
		unsigned IsGapper:1;

		/*
		**	If this unit cannot fire while moving, then this flag will be
		**	true. Such a unit must stop and stabilize for a bit before it
		**	can fire.
		*/
		unsigned IsNoFireWhileMoving:1;

		/*
		**	This value represents the unit class. It can serve as a unique
		**	identification number for this unit class.
		*/
		UnitType Type;

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
		UnitTypeClass(NoInitClass const & x) : TechnoTypeClass(x) {}
		UnitTypeClass	(
						UnitType type,
						int name,
						char const * ininame,
						AnimType exp,
						RemapType remap,
						int verticaloffset,
						int primaryoffset,
						int primarylateral,
						int secondaryoffset,
						int secondarylateral,
						bool is_goodie,
						bool is_nominal,
						bool is_crusher,
						bool is_harvest,
						bool is_stealthy,
						bool is_insignificant,
						bool is_turret_equipped,
						bool is_radar_equipped,
						bool is_fire_anim,
						bool is_lock_turret,
						bool is_gigundo,
						bool is_animating,
						bool is_jammer,
						bool is_gapper,
						int rotation,
						int toffset,
						MissionType order
						);

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static void Init_Heap(void);
		static UnitType From_Name(char const * name);
		static UnitTypeClass & As_Reference(UnitType type);
		static void Init(TheaterType ) {};
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual bool Read_INI(CCINIClass & ini);
		virtual void Dimensions(int &width, int &height) const;
		virtual bool Create_And_Place(CELL cell, HousesType house) const;
		virtual ObjectClass * Create_One_Of(HouseClass * house) const;
		virtual int Max_Pips(void) const;

		void Turret_Adjust(DirType dir, int & x, int & y) const;

		#ifdef SCENARIO_EDITOR
		virtual void Display(int x, int y, WindowNumberType window, HousesType house) const;
		#endif

		/*
		**	The animation stage list for harvester dumping into the refinery.
		*/
		static const int Harvester_Dump_List[22];

		/*
		**	The animatino stage list for harvester loading up on ore.
		*/
		static const int Harvester_Load_List[9];

		/*
		**	The number of animation stages when the harvester is loading
		**	up on ore in the field.
		*/
		static const int Harvester_Load_Count;
};


/***************************************************************************
**	This specifies the constant attribute data associated with naval
**	vessels.
*/
class VesselTypeClass : public TechnoTypeClass
{
	public:
		/*
		**	Does this unit have only 8 facings? Special test units have limited
		**	facings.
		*/
		unsigned IsPieceOfEight:1;

		/*
		**	This value represents the unit class. It can serve as a unique
		**	identification number for this unit class.
		*/
		VesselType Type;

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
		VesselTypeClass(NoInitClass const & x) : TechnoTypeClass(x) {}
		VesselTypeClass	(
						VesselType type,
						int name,
						char const * ininame,
						AnimType exp,
						int verticaloffset,
						int primaryoffset,
						int primarylateral,
						int secondaryoffset,
						int secondarylateral,
						bool is_eight,
						bool is_nominal,
						bool is_turret_equipped,
						int rotation,
						int toffset
						);

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static void Init_Heap(void);
		static VesselType From_Name(char const * name);
		static VesselTypeClass & As_Reference(VesselType type);
		static void Init(TheaterType ) {};
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual void Dimensions(int &width, int &height) const;
		virtual bool Create_And_Place(CELL cell, HousesType house) const;
		virtual ObjectClass * Create_One_Of(HouseClass * house) const;
		virtual int Max_Pips(void) const;
		virtual short const * Overlap_List(void) const;

		void Turret_Adjust(DirType dir, int & x, int & y) const;

		#ifdef SCENARIO_EDITOR
		virtual void Display(int x, int y, WindowNumberType window, HousesType house) const;
		#endif
};


/***************************************************************************
**	The various unit types need specific data that is unique to units as
**	opposed to buildings. This derived class elaborates these additional
**	data types.
*/
class InfantryTypeClass : public TechnoTypeClass
{
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
		**	will be set to true. Typically, this is the engineer.
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
		**	If the infantry unit is equipped with C4 explosives, then this
		**	flag will be true. Such infantry can enter and destroy enemy
		**	buildings.
		*/
		unsigned IsBomber:1;

		/*
		** This flags whether this infantry is actually a dog.  A dog
		** uses different voice responses, has no ammo, and runs instead
		** of walks to attack.
		*/
		unsigned IsDog:1;

		/*
		** This flag specifies whether this infantry type should use the
		** override remap table, instead of the house remap table.  This is
		** used to turn the two civilian animations into a veritable smorgasbord
		** of civilian types, for example.
		*/
		unsigned IsRemapOverride:1;

		/*
		**	This value represents the unit class. It can serve as a unique
		**	identification number for this unit class.
		*/
		InfantryType Type;

		/*
		**	When this infantry unit is loaded onto a transport, then this
		**	is the pip shape to use. Primarily, this is a color control.
		*/
		PipEnum Pip;

		/*
		**	This is an array of the various animation frame data for the actions that
		**	the infantry may perform.
		*/
		DoInfoStruct const * DoControls;

		/*
		**	Alternate animation info for the 'virtual' window which gets rendered on the GlyphX client.
		** The infantry frames here map to the original TD infantry frames, so a different set is needed depending on whether
		** we are rendering in legacy mode or on the GlyphX client. ST - 9/5/2019 12:17PM
		*/
		DoInfoStruct const * DoControlsVirtual;

		/*
		**	There are certain units with special animation sequences built into the
		**	shape file. These values tell how many frames are used for the firing animation.
		*/
		char FireLaunch;
		char ProneLaunch;

		/*
		** This is a pointer to the special override remap table, which is
		** used only in conjunction with the IsRemapOverride flag, and is
		** primarily used for the civilians.
		*/
		unsigned char const * OverrideRemap;

		/*
		**	This is the explicit unit class constructor.
		*/
		InfantryTypeClass(NoInitClass const & x) : TechnoTypeClass(x) {}
		InfantryTypeClass	(
						InfantryType type,
						int name,
						char const * ininame,
						int verticaloffset,
						int primaryoffset,
						bool is_female,
						bool is_crawling,
						bool is_civilian,
						bool is_remap_override,
						bool is_nominal,
						bool is_theater,
						PipEnum pip,
						DoInfoStruct const * controls,
						DoInfoStruct const * virtual_controls,
						int firelaunch,
						int pronelaunch,
						unsigned char const * override_remap,
						int horizontaloffset=0);

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static void Init_Heap(void);
		static InfantryType From_Name(char const * name);
		static InfantryTypeClass & As_Reference(InfantryType type);
		static void Init(TheaterType ) {};
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual bool Read_INI(CCINIClass & ini);
		virtual void Dimensions(int & width, int & height) const;
		virtual bool Create_And_Place(CELL cell, HousesType house) const;
		virtual ObjectClass * Create_One_Of(HouseClass * house) const;
		virtual short const * Occupy_List(bool placement=false) const;
		virtual int Full_Name(void) const;

		#ifdef SCENARIO_EDITOR
		virtual void Display(int x, int y, WindowNumberType window, HousesType house) const;
		#endif
};


/****************************************************************************
**	The various aircraft types are controlled by object types of
**	this class.
*/
class AircraftTypeClass : public TechnoTypeClass
{
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

		/*
		**	This is the kind of aircraft identifier number.
		*/
		AircraftType Type;

		/*
		**	This specifies the default mission order for this aircraft. Some aircraft default
		**	to guard mode (e.g., helicopters) while some default to attack mode (e.g., bombers).
		*/
		MissionType Mission;

		/*
		**	This is the preferred landing building. The aircraft will try to land at the
		**	building of this type.
		*/
		StructType Building;

		/*
		** This is the final approach speed of this aircraft type for landing
		** at an airfield.  Most aircraft hit it at full speed, but the MIG is
		** an example of a plane that needs a slower approach speed to hit the
		** airfield.
		*/
		int LandingSpeed;

		AircraftTypeClass(NoInitClass const & x) : TechnoTypeClass(x) {}
		AircraftTypeClass(
				AircraftType airtype,
				int name,
				char const * ininame,
				int verticaloffset,
				int primaryoffset,
				int primarylateral,
				bool is_fixedwing,
				bool is_rotorequipped,
				bool is_rotorcustom,
				bool is_landable,
				bool is_stealthy,
				bool is_selectable,
				bool is_legal_target,
				bool is_insignificant,
				bool is_immune,
				StructType building,
				int landingspeed,
				int rotation,
				MissionType deforder);

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static void Init_Heap(void);
		static AircraftType From_Name(char const * name);
		static AircraftTypeClass & As_Reference(AircraftType a);
		static void Init(TheaterType ) {};
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual void Dimensions(int &width, int &height) const;
		virtual bool Create_And_Place(CELL, HousesType) const;
		virtual ObjectClass * Create_One_Of(HouseClass * house) const;
		virtual short const * Occupy_List(bool placement=false) const;
		virtual short const * Overlap_List(void) const;
		virtual int Max_Pips(void) const;

		#ifdef SCENARIO_EDITOR
		virtual void Display(int x, int y, WindowNumberType window, HousesType house) const;
		#endif

		static void const * LRotorData;
		static void const * RRotorData;
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
		** Does this bullet need a shadow drawn under it?  Shadowed bullets
		** use the Height value to offset their Y position.
		*/
		unsigned IsShadow:1;

		/*
		**	If this projectile is one that ballistically arcs from ground level, up into the air and
		**	then back to the ground, where it explodes. Typical uses of this are for grenades and
		**	artillery shells.
		*/
		unsigned IsArcing:1;

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
		**	If this bullet can fire upon ground targets, then this flag will be true.
		*/
		unsigned IsAntiGround:1;

		/*
		**	If this bullet can be fired upon submarines (that are submerged), then
		**	this flag will be true.
		*/
		unsigned IsAntiSub:1;

		/*
		**	If this bullet should lose strength as it travels toward the target, then
		**	this flag will be true.
		*/
		unsigned IsDegenerate:1;

		/*
		**	Does this projectile travel under the water? If so, then its imagery will be modified
		**	to look like it is doing so.
		*/
		unsigned IsSubSurface:1;

		/*
		**	If this projectile is equipped with a parachute, then this flag will be set. Parachute
		**	bombs are usually the only one with this flag set.
		*/
		unsigned IsParachuted:1;

		/*
		**	Is this unit of the humongous size?  Certain very large projectiles have
		**	this flag set. Typically, they require a special offset list so that the cells
		**	they overlap will be properly redrawn.
		*/
		unsigned IsGigundo:1;

		/*
		**	This element is a unique identification number for the bullet
		**	type.
		*/
		BulletType Type;

		/*
		**	This is the rotation speed of the bullet. It only has practical value
		**	for those projectiles that performing homing action during flight -- such
		**	as with rockets. If the ROT is zero, then no homing is performed. Otherwise
		**	the projectile is considered to be a homing type.
		*/
		unsigned char ROT;

		/*
		**	Some projectiles have a built in arming distance that must elapse before the
		**	projectile may explode. If this value is non-zero, then this override is
		**	applied.
		*/
		int Arming;

		/*
		**	If this bullet is of the tumbling type, then this is the modulo to factor
		**	into the game frame when determining what shape number to use for the
		**	imagery.
		*/
		int Tumble;

		//---------------------------------------------------------------------
		BulletTypeClass(NoInitClass const & x) : ObjectTypeClass(x) {}
		BulletTypeClass(char const * name);

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static void Init_Heap(void);
		static BulletTypeClass & As_Reference(BulletType type);
		static void Init(TheaterType ) {};
		static void One_Time(void);

		virtual bool Read_INI(CCINIClass & ini);
		virtual bool Create_And_Place(CELL , HousesType =HOUSE_NONE) const {return false;};
		virtual ObjectClass * Create_One_Of(HouseClass *) const {return 0;};
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
		int Theater;

		/*
		**	Does this terrain object get placed on the water instead of the ground?
		*/
		unsigned IsWaterBased:1;

		//----------------------------------------------------------------
		TerrainTypeClass(NoInitClass const & x) : ObjectTypeClass(x) {}
		TerrainTypeClass(
				TerrainType terrain,
				int theater,
				COORDINATE centerbase,
				bool is_immune,
				bool is_water,
				char const * ininame,
				int fullname,
				short const * occupy,
				short const * overlap);

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static void Init_Heap(void);
		static TerrainType From_Name(char const * name);
		static TerrainTypeClass & As_Reference(TerrainType type);
		static void Init(TheaterType theater = THEATER_TEMPERATE);
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual COORDINATE Coord_Fixup(COORDINATE coord) const;
		virtual bool Create_And_Place(CELL cell, HousesType house) const;
		virtual ObjectClass * Create_One_Of(HouseClass *) const;
		virtual short const * Occupy_List(bool placement=false) const;
		virtual short const * Overlap_List(void) const;

		#ifdef SCENARIO_EDITOR
		virtual void Display(int x, int y, WindowNumberType window, HousesType house=HOUSE_NONE) const;
		#endif

	private:
		short const * Occupy;
		short const * Overlap;
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
		**	Raw dimensions of this template (in icons).
		*/
		unsigned char Width,Height;

		//----------------------------------------------------------
		TemplateTypeClass(NoInitClass const & x) : ObjectTypeClass(x) {};
		TemplateTypeClass(
			TemplateType iconset,
			int theater,
			char const * ininame,
			int fullname);

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static void Init_Heap(void);
		static TemplateType From_Name(char const * name);
		static TemplateTypeClass & As_Reference(TemplateType type);
		static void Init(TheaterType theater);
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual COORDINATE Coord_Fixup(COORDINATE coord) const;
		virtual bool Create_And_Place(CELL cell, HousesType house=HOUSE_NONE) const;
		virtual ObjectClass * Create_One_Of(HouseClass *) const;
		virtual short const * Occupy_List(bool placement=false) const;
		LandType Land_Type(int icon) const;

		#ifdef SCENARIO_EDITOR
		virtual void Display(int x, int y, WindowNumberType window, HousesType house=HOUSE_NONE) const;
		#endif
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
		**	If this animation is theater specific, then this flag will be
		**	set to true. Most animations are not theater specific.
		*/
		unsigned IsTheater:1;

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
		**	is to prevent the player from seeing craters "pop" into existence.
		*/
		int Biggest;

		/*
		**	Some animations (when attached to another object) damage the object it
		**	is in contact with. Fire is a good example of this. This value is a
		**	fixed point number of the damage that is applied to the attached object
		**	every game tick. The damage is expressed as damage points per game frame.
		**	Because it is a fixed point fraction, the damage can be very slight.
		*/
		fixed Damage;

		/*
		**	Simple animation delay value between advancing of frames. This can
		**	be overridden by the control list.
		*/
		int Delay;

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
		unsigned Loops;

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
		**	The shape name to used for 4K virtual rendering, overrides the INI name
		*/
		char const * VirtualName;

		/*
		**	Animation data to use specifically for virtual rendering (implies this animation only for legacy).
		*/
		AnimType VirtualAnim;

		//---------------------------------------------------------------------------
		AnimTypeClass(NoInitClass const & x) : ObjectTypeClass(x) {}
		AnimTypeClass(AnimType anim,
							char const * name,
							int size,
							int biggest,
							bool istheater,
							bool isnormal,
							bool iswhite,
							bool isscorcher,
							bool iscrater,
							bool issticky,
							bool ground,
							bool istrans,
							bool isflame,
							fixed damage,
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
							char const * virtualname=NULL,
							AnimType virtualanim=ANIM_NONE);

		static void Init_Heap(void);
		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static AnimTypeClass & As_Reference(AnimType type);
		static void Init(TheaterType theater);
		static void One_Time(void);

		virtual bool Create_And_Place(CELL , HousesType =HOUSE_NONE) const {return false;};
		virtual ObjectClass * Create_One_Of(HouseClass *) const {return 0;};
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
		OverlayTypeClass(NoInitClass const & x) : ObjectTypeClass(x) {}
		OverlayTypeClass(
			OverlayType iconset,
			char const * ininame,
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

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static void Init_Heap(void);
		static OverlayType From_Name(char const * name);
		static OverlayTypeClass & As_Reference(OverlayType type);
		static void Init(TheaterType);
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual COORDINATE Coord_Fixup(COORDINATE coord) const;
		virtual bool Create_And_Place(CELL cell, HousesType house=HOUSE_NONE) const;
		virtual ObjectClass * Create_One_Of(HouseClass *) const;
		virtual short const * Occupy_List(bool placement=false) const;
		virtual void Draw_It(int x, int y, int data) const;
		virtual unsigned char * Radar_Icon(int data) const;

		#ifdef SCENARIO_EDITOR
		virtual void Display(int x, int y, WindowNumberType window, HousesType house=HOUSE_NONE) const;
		#endif
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
		SmudgeTypeClass(NoInitClass const & x) : ObjectTypeClass(x) {}
		SmudgeTypeClass(
			SmudgeType smudge,
			char const * ininame,
			int fullname,
			int width,
			int height,
			bool isbib,
			bool iscrater
			);

		static void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		static void Init_Heap(void);
		static SmudgeType From_Name(char const * name);
		static SmudgeTypeClass & As_Reference(SmudgeType type);
		static void Init(TheaterType);
		static void One_Time(void);
		static void Prep_For_Add(void);

		virtual bool Create_And_Place(CELL cell, HousesType house=HOUSE_NONE) const;
		virtual ObjectClass * Create_One_Of(HouseClass *) const;
		virtual short const * Occupy_List(bool placement=false) const;
		virtual short const * Overlap_List(void) const {return Occupy_List();};
		virtual void Draw_It(int x, int y, int data) const ;

		#ifdef SCENARIO_EDITOR
		virtual void Display(int x, int y, WindowNumberType window, HousesType house=HOUSE_NONE) const;
		#endif
};

#endif

