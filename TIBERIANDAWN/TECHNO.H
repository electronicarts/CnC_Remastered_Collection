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

/* $Header:   F:\projects\c&c\vcs\code\techno.h_v   2.17   16 Oct 1995 16:46:58   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TECHNO.H                                                     *
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

#ifndef TECHNO_H
#define TECHNO_H

#include	"radio.h"
#include	"stage.h"
#include	"cargo.h"
#include	"flasher.h"
#include	"house.h"
#include	"target.h"
#include	"bullet.h"
#include	"door.h"
#include	"crew.h"

/****************************************************************************
**	This is the common data between building and units.
*/
class TechnoClass :	public RadioClass,
							public FlasherClass,
							public StageClass,
							public CargoClass,
							public DoorClass,
							public CrewClass
{
	public:

		/*
		**	This flag will be true if the object has been damaged with malace.
		**	Damage received due to friendly fire or wear and tear does not count.
		**	The computer is not allowed to sell a building unless it has been
		**	damaged with malace.
		*/
		unsigned IsTickedOff:1;

		/*
		**	If this object has inherited the ability to cloak, then this bit will
		**	be set to true.
		*/
		unsigned IsCloakable:1;

		/*
		**	If this object is designated as special then this flag will be true. For
		**	buildings, this means that it is the primary factory. For units, it means
		**	that the unit is the team leader.
		*/
		unsigned IsLeader:1;

		/*
		**	Certain units are flagged as "loaners".  These units are typically transports that
		**	are created solely for the purpose of delivering reinforcements.  Such "loaner"
		**	units are not owned by the player and thus cannot be directly controlled.  These
		**	units will leave the game as soon as they have fulfilled their purpose.
		*/
		unsigned IsALoaner:1;

		/*
		**	Once a unit enters the map, then this flag is set. This flag is used to make
		**	sure that a unit doesn't leave the map once it enters the map.
		*/
		unsigned IsLocked:1;

		/*
		**	Buildings and units with turrets usually have a recoil animation when they
		**	fire. If this flag is true, then the next rendering of the object will be
		**	in the "recoil state". The flag will then be cleared pending the next
		**	firing event.
		*/
		unsigned IsInRecoilState:1;

		/*
		**	If this unit is "loosely attached" to another unit it is given special
		**	processing. A unit is in such a condition when it is in the process of
		**	unloading from a transport type object. During the unloading process
		**	the transport object must stay still until the unit is free and clear.
		**	At that time it radios the transport object and the "tether" is broken -
		**	freeing both the unit and the transport object.
		*/
		unsigned IsTethered:1;

		/*
		**	Is this object owned by the player?  If not, then it is owned by the computer
		**	or remote opponent. This flag facilitates the many logic differences when dealing
		**	with player's or computer's units or buildings.
		*/
		unsigned IsOwnedByPlayer:1;

		/*
		**	The more sophisticated game objects must keep track of whether they are discovered
		**	or not. This is because the state of discovery can often control how the object
		**	behaves. In addition, this fact is used in radar and user I/O processing.
		*/
		unsigned IsDiscoveredByPlayer:1;

		/*
		**	This is used to control the computer recognizing this object.
		*/
		unsigned IsDiscoveredByComputer:1;

		/*
		**	Some game objects can be of the "lemon" variety. This means that they take damage
		**	even when everything is ok. This adds a little variety to the game.
		*/
		unsigned IsALemon:1;

		/*
		**	This flag is used to control second shot processing for those units or buildings
		**	that fire two shots in quick succession. When this flag is true, it indicates that
		**	the second shot is ready to fire. After this shot is fired, regular rearm timing
		**	is used rather than the short rearm time.
		*/
		unsigned IsSecondShot:1;

		/*
		**	For units in area guard mode, this is the recorded home position. The guarding
		**	unit will try to stay near this location in the course of it's maneuvers. This is
		**	also used to record a pending transport for those passengers that are waiting for
		**	the transport to become available. It is also used by harvesters so that they know
		**	where to head back to after unloading.
		*/
		TARGET ArchiveTarget;

		/*
		**	This is the house that the unit belongs to.
		*/
		HouseClass * House;

		/*
		**	This records the current cloak state for this vehicle.
		*/
		CloakType Cloak;
		StageClass CloakingDevice;

		/* (Targeting Computer)
		**	This is the target value for the item that this vehicle should ATTACK. If this
		**	is a vehicle with a turret, then it may differ from its movement destination.
		*/
		TARGET TarCom;
		TARGET SuspendedTarCom;

		/*
		**	This is the visible facing for the unit or building.
		*/
		FacingClass PrimaryFacing;

		/*
		**	This is the arming countdown. It represents the time necessary
		**	to reload the weapon.
		*/
		unsigned char Arm;

		/*
		**	The number of shot this object can fire before running out of ammo. If this
		**	value is zero, then firing is not allowed. If -1, then there is no ammunition
		**	limit.
		*/
		int Ammo;

		/*
		**	Used by the Obelisk laser to cache its laser weapon rendering data.
		**  Line data is cleared after the specified delay.
		**  Each line consists of: x, y, x1, y1, color, frame (max frame used to determine duration)
		*/
		int Lines[3][5];
		int LineCount;
		int LineFrame;
		int LineMaxFrames;

		/*
		**	This is the amount of money spent to produce this object. This value really
		**	only comes into play for the case of buildings that have special "free"
		**	objects available when purchased at the more expensive rate.
		*/
		int PurchasePrice;

		/*
		**	Per-player view of whether a techno object is discovered. One bit for each house type. ST - 3/6/2019 11:15AM
		*/
		unsigned int IsDiscoveredByPlayerMask;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[16];

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		TechnoClass(void);
		TechnoClass(HousesType house);
		virtual ~TechnoClass(void) {};

		/*
		**	Query functions.
		*/
		virtual int Refund_Amount(void) const;
		virtual CELL Find_Exit_Cell(TechnoClass const * techno) const;
		virtual BuildingClass * Find_Docking_Bay(StructType b, bool friendly) const;
		virtual int Threat_Range(int control) const;
		virtual InfantryType Crew_Type(void) const;
		TechnoTypeClass const * Techno_Type_Class(void) const {return((TechnoTypeClass const *)&Class_Of());};
		CELL  Nearby_Location(TechnoClass const * from=NULL) const;
		virtual unsigned char Get_Ownable(void) const;
		virtual bool Can_Player_Fire(void) const;
		virtual bool Can_Player_Move(void) const;
		virtual bool Is_Weapon_Equipped(void) const;
		virtual bool Can_Repair(void) const;
		virtual bool Is_Techno(void) const;
		virtual HousesType Owner(void) const;
		virtual int Risk(void) const;
		virtual int Value(void) const;
		virtual int Rearm_Delay(bool second=true) const;
		virtual ActionType What_Action(ObjectClass * target) const;
		virtual ActionType What_Action(CELL cell) const;
		virtual int Tiberium_Load(void) const;
		virtual DirType Desired_Load_Dir(ObjectClass * , CELL & moveto) const;
		virtual int Pip_Count(void) const;
		virtual DirType Fire_Direction(void) const;

		/*
		**	User I/O.
		*/
		virtual void Clicked_As_Target(HousesType house, int count=7); // 2019/09/20 JAS - Added record of who clicked on the object
		virtual bool Select(bool allow_mixed = false);
		virtual void Response_Select(void);
		virtual void Response_Move(void);
		virtual void Response_Attack(void);
		virtual void Player_Assign_Mission(MissionType order, TARGET target=TARGET_NONE, TARGET destination=TARGET_NONE);

		/*
		**	Combat related.
		*/
		void  Base_Is_Attacked(TechnoClass const *enemy);
		void  Kill_Cargo(TechnoClass * source);
		virtual void Record_The_Kill(TechnoClass * source);
		virtual bool Target_Something_Nearby(ThreatType threat=THREAT_NORMAL);
		virtual void Stun(void);
		virtual bool In_Range(COORDINATE coord, int which=0) const;
		virtual bool In_Range(TARGET target, int which=0, bool reciprocal_check = true) const;
		virtual bool In_Range(ObjectClass const * target, int which=0, bool reciprocal_check = true) const;
		virtual void Death_Announcement(TechnoClass const * source=0) const = 0;
		virtual FireErrorType Can_Fire(TARGET target, int which=0) const;
		virtual TARGET Greatest_Threat(ThreatType threat) const;
		virtual void Assign_Target(TARGET target);
		virtual void Override_Mission(MissionType mission, TARGET tarcom, TARGET navcom);
		virtual bool Restore_Mission(void);
		virtual BulletClass * Fire_At(TARGET target, int which=0);
		virtual int Weapon_Range(int which) const;
		virtual bool Captured(HouseClass * newowner);
		virtual ResultType Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source);
		bool  Evaluate_Cell(ThreatType method, int mask, CELL cell, int range, TechnoClass const ** object, int & value) const;
		bool  Evaluate_Object(ThreatType method, int mask, int range, TechnoClass const * object, int & value) const;

		/*
		**	AI.
		*/
		virtual void AI(void);
		virtual bool Revealed(HouseClass * house);
		virtual RadioMessageType Receive_Message(RadioClass * from, RadioMessageType message, long & param);

		/*
		**	Scenario and debug support.
		*/
		#ifdef CHEAT_KEYS
		virtual void Debug_Dump(MonoClass *mono) const;
		#endif

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		/*
		**	Display and rendering support functionality. Supports imagery and how
		**	object interacts with the map and thus indirectly controls rendering.
		*/
		virtual void const * Remap_Table(void);
		VisualType  Visual_Character(bool raw = false);
		void  Techno_Draw_Object(void const * shapefile, int shapenum, int x, int y, WindowNumberType window);
		
		// Added. ST - 6/20/2019 1:34PM
		void  Techno_Draw_Object_Virtual(void const * shapefile, int shapenum, int x, int y, WindowNumberType window, const char *shape_name);
		
		virtual void Draw_It(int x, int y, WindowNumberType window);
		virtual void Draw_Pips(int x, int y, WindowNumberType window);
		virtual void Hidden(void);
		virtual bool Mark(MarkType mark);
		virtual int Exit_Object(TechnoClass *);
		virtual void Do_Uncloak(void);
		virtual void Do_Cloak(void);
		virtual void Do_Shimmer(void);

		/*
		**	Movement and animation.
		*/
		virtual void Random_Animate(void);
		virtual void Assign_Destination(TARGET target);
		virtual void Scatter(COORDINATE source = NULL, bool forced=false, bool nokidding=false);
		virtual void Per_Cell_Process(bool);
		virtual void Enter_Idle_Mode(bool initial=false);

		/*
		**	Map entry and exit logic.
		*/
		virtual bool Unlimbo(COORDINATE , DirType facing=DIR_N);
		virtual void Detach(TARGET target, bool all);




		/*
		** New functions for per-player discovery for multiplayer. ST - 3/6/2019 11:17AM
		*/
		bool Is_Discovered_By_Player(HouseClass *player = NULL) const;
		void Set_Discovered_By_Player(HouseClass *player = NULL);
		void Clear_Discovered_By_Players();
		bool Is_Owned_By_Player(HouseClass *player = NULL) const;


#ifdef USE_RA_AI
		/*
		** For AI, from Red Alert. ST - 7/16/2019 11:38AM
		*/
		int Anti_Air(void) const;
		int Anti_Armor(void) const;
		int Anti_Infantry(void) const;
#endif // USE_RA_AI

		/*
		**	Facing translation tables that fix the flaw with 3D studio when
		**	it renders 45 degree angles.
		*/
		static int const BodyShape[32];
//		static int const TurretShape[32];


};

#endif