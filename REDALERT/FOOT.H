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

/* $Header: /CounterStrike/FOOT.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FOOT.H                                                       *
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

#ifndef FOOT_H
#define FOOT_H

#include	"target.h"
#include	"type.h"
#include	"techno.h"
#include	"ftimer.h"

class UnitClass;
class BuildingClass;


/****************************************************************************
**	Movable objects are handled by this class definition. Moveable objects
**	cover everything except buildings.
*/
class FootClass :	public TechnoClass
{
	public:
		/*
		**	This flag controls whether a range limiting effect should be in place. If
		**	true, then target scanning will be limited to the range of the object
		**	regardless of what was requested from the target scanning logic. This value
		**	is used for ships so that they won't permanently stick on a an attack mission
		**	for a target they can never get within range of. This value will toggle when
		**	a path cannot be generated and the target is not within range. It will also
		**	toggle when path limiting is true, but there is not target found within
		**	the limited range.
		*/
		unsigned IsScanLimited:1;

		/*
		**	If this unit has officially joined the team's group, then this flag is
		**	true. A newly assigned unit to a team is not considered part of the
		**	team until it actually reaches the location where the team is. By
		**	using this flag, it allows a team to continue to intelligently attack
		**	a target without falling back to regroup the moment a distant member
		**	joins.
		*/
		unsigned IsInitiated:1;

		/*
		**	When the player gives this object a navigation target AND that target
		**	does not result in any movement of the unit, then a beep should be
		**	sounded. This typically occurs when selecting an invalid location for
		**	movement. This flag is cleared if any movement was able to be performed.
		**	It never gets set for computer controlled units.
		*/
		unsigned IsNewNavCom:1;

		/*
		**	There are certain cases where a unit should perform a full scan rather than
		**	the more efficient "ring scan". This situation occurs when a unit first
		**	appears on the map or when it finishes a multiple cell movement track.
		*/
		unsigned IsPlanningToLook:1;

		/*
		**	Certain units have the ability to metamorphize into a building. When this
		**	operation begins, certain processes must occur. During these operations, this
		**	flag will be true. This ensures that any necessary special case code gets
		**	properly executed for this unit.
		*/
		unsigned IsDeploying:1;

		/*
		**	This flag tells the system that the unit is doing a firing animation. This is
		**	critical to the firing logic.
		*/
		unsigned IsFiring:1;

		/*
		**	This unit could be either rotating its body or rotating its turret. During the
		**	process of rotation, this flag is set. By examining this flag, unnecessary logic
		**	can be avoided.
		*/
		unsigned IsRotating:1;

		/*
		**	If this object is current driving to a short range destination, this flag is
		**	true. A short range destination is either the next cell or the end of the
		**	current "curvy" track. An object that is driving is not allowed to do anything
		**	else until it reaches its destination. The exception is when infantry wish to
		**	head to a different destination, they are allowed to start immediately.
		*/
		unsigned IsDriving:1;

		/*
		**	If this object is unloading from a hover transport, then this flag will be
		**	set to true. This handles the unusual case of an object disembarking from the
		**	hover lander yet not necessarily tethered but still located in an overlapping
		**	position. This flag will be cleared automatically when the object moves to the
		**	center of a cell.
		*/
		unsigned IsUnloading:1;

		/*
		** If this object is part of a formation, this bit will be set.  The
		** formation only occurs when every member of a team is selected, and
		** only those members of the team are the ones selected.
		*/
		unsigned IsFormationMove:1;

		/*
		**	If the navigation movement queue is to be looped rather than consumed, then
		**	this flag will be true. By looping, the unit will travel through the locations
		**	in the queue indefinately.
		*/
		unsigned IsNavQueueLoop:1;

		/*
		**	If this object is scattering, then this flag will be true. While true, the
		**	NavCom should not be arbitrarily changed. This flag will automatcially be
		**	cleared when the object moves one cell.
		*/
		unsigned IsScattering:1;

		/*
		**	If this object is moving onto a bridge, then this flag will be true.
		*/
		unsigned IsMovingOntoBridge:1;

		/*
		**	This is the "throttle setting" of the unit. It is a fractional value with 0 = stop
		**	and 255 = full speed.
		*/
		int Speed;

		/*
		**	This is the override speed adjuster. Normally, this is a fixed point
		**	value of 0x0100, but it can be modified by crate powerups.
		*/
		fixed SpeedBias;

		/*
		** For units in a formation, these values represent the distance from
		** the target destination where the unit should move to.  For example,
		** in a horizontal line formation, XFormOffset would be set to some
		** value, and YFormOffset would be zero.
		*/
		int XFormOffset;
		int YFormOffset;

		/*
		**
		**	This is the desired destination of the unit. The unit will attempt to head
		**	toward this target (avoiding intervening obstacles).
		*/
		TARGET NavCom;
		TARGET SuspendedNavCom;

		/*
		**	A sequence of move destinations can be given to a unit. The sequence is
		**	stores as an array of movement targets. The list is terminated with a
		**	TARGET_NONE.
		*/
		TARGET NavQueue[10];

		/*
		**	This points to the team that "owns" this object. This pointer is used to
		**	quickly process the team when this object is the source of the change. An
		**	example would be if this object were to be destroyed, it would inform the
		**	team of this fact by using this pointer.
		*/
		CCPtr<TeamClass> Team;

		/*
		**	If this object is part of a pseudo-team that the player is managing, then
		**	this will be set to the team number (0 - 9). If it is not part of any
		**	pseudo-team, then the number will be -1.
		*/
		unsigned char Group;

		/*
		**	This points to the next member in the team that this object is part of. This
		**	is used to quickly process each team member when the team class is the source
		**	of the change. An example would be if the team decided that everyone is going
		**	to move to a new location, it would inform each of the objects by chaining
		**	through this pointer.
		*/
		FootClass * Member;

		/*
		**	Since all objects derived from this class move according to a path list.
		**	This is the path list. It specifies, as a simple list of facings, the
		**	path that the object should follow in order to reach its destination.
		**	This path list is limited in size, so it might require several generations
		**	of path lists before the ultimate destination is reached. The game logic
		**	handles regenerating the path list as necessary.
		*/
		FacingType Path[CONQUER_PATH_MAX];

		/*
		**	This value keeps track of how serious the unit is in trying to reach the
		**	destination specified. As blockages arise, this threshold will rise
		**	unit it reaches the point of complete failure. When that event occurs, the
		**	unit will realize that it cannot get to its specified destination and will
		**	try to perform some other action instead.
		*/
		MoveType PathThreshhold;

		/*
		**	When there is a complete findpath failure, this timer is initialized so
		**	that a findpath won't be calculated until this timer expires.
		*/
		CDTimerClass<FrameTimerClass> PathDelay;
		enum {PATH_RETRY=10};
		int TryTryAgain;		// Number of retry attempts remaining.

		/*
		** If the object has recently attacked a base, then this timer will not
		** have expired yet.  It is used so a building does not keep calling
		** for help from the same attacker.
		*/
		CDTimerClass<FrameTimerClass> BaseAttackTimer;

		/*
		** For formation moves, this will be the override speed.
		*/
		SpeedType FormationSpeed;

		/*
		** For formation moves, this will be the override maximum speed.
		*/
		MPHType	FormationMaxSpeed;

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		FootClass(NoInitClass const & x) : TechnoClass(x), Team(x), PathDelay(x), BaseAttackTimer(x) {};
		FootClass(RTTIType rtti, int id, HousesType house);

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		bool Basic_Path(void);

		virtual RadioMessageType Receive_Message(RadioClass * from, RadioMessageType message, long & param);
		virtual bool Can_Demolish(void) const;
		bool Is_Recruitable(HouseClass const * house=NULL) const;
		bool Is_On_Priority_Mission(void) const;

		/*
		**	Coordinate inquiry functions. These are used for both display and
		**	combat purposes.
		*/
		virtual COORDINATE Sort_Y(void) const;
		virtual COORDINATE Likely_Coord(void) const;

		/*
		**	Driver control support functions. These are used to control cell
		**	occupation flags and driver instructions.
		*/
		COORDINATE Head_To_Coord(void) const {return (HeadToCoord);};
		virtual bool Start_Driver(COORDINATE &headto);
		virtual bool Stop_Driver(void);
		virtual void Assign_Destination(TARGET target);
		bool Is_Allowed_To_Leave_Map(void) const;

		/*
		**	Display and rendering support functionality. Supports imagery and how
		**	object interacts with the map and thus indirectly controls rendering.
		*/
		virtual bool Unlimbo(COORDINATE , DirType dir = DIR_N);
		virtual bool Mark(MarkType mark=MARK_CHANGE);

		/*
		**	User I/O.
		*/
		virtual void Active_Click_With(ActionType action, ObjectClass * object);
		virtual void Active_Click_With(ActionType action, CELL cell);

		/*
		**	Combat related.
		*/
		virtual void Stun(void);
		virtual ResultType Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source=0, bool forced=false);
		virtual void Death_Announcement(TechnoClass const * source=0) const;

		/*
		**	AI.
		*/
		virtual void AI(void);
		virtual void Sell_Back(int control);
		virtual int Offload_Tiberium_Bail(void);
		virtual TARGET Greatest_Threat(ThreatType method) const;
		virtual void Detach(TARGET target, bool all);
		virtual void Detach_All(bool all=true);
		virtual int Mission_Retreat(void);
		virtual int Mission_Enter(void);
		virtual int Mission_Move(void);
		virtual int Mission_Capture(void);
		virtual int Mission_Attack(void);
		virtual int Mission_Guard(void);
		virtual int Mission_Hunt(void);
		virtual int Mission_Guard_Area(void);

		/*
		**	Scenario and debug support.
		*/
		#ifdef CHEAT_KEYS
		virtual void Debug_Dump(MonoClass *mono) const;
		#endif

		/*
		**	Movement and animation.
		*/
		void Handle_Navigation_List(void);
		void Queue_Navigation_List(TARGET target);
		void Clear_Navigation_List(void);
		virtual void Per_Cell_Process(PCPType why);
		virtual void Approach_Target(void);
		virtual void Fixup_Path(PathType *) {};
		virtual void Set_Speed(int speed);
		virtual MoveType Can_Enter_Cell(CELL cell, FacingType from=FACING_NONE) const;
		int Optimize_Moves(PathType *path, MoveType threshhold);
		virtual void Override_Mission(MissionType mission, TARGET tarcom, TARGET navcom);
		virtual bool Restore_Mission(void);
		CELL Adjust_Dest(CELL cell) const;

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		CELL Safety_Point(CELL src, CELL dst, int start, int max);
		int Rescue_Mission(TARGET tarcom);

	private:
		int Passable_Cell(CELL cell, FacingType face, int threat, MoveType threshhold);
		PathType * Find_Path(CELL dest, FacingType *final_moves, int maxlen, MoveType threshhold);
		void Debug_Draw_Map(char const * txt, CELL start, CELL dest, bool pause);
		void Debug_Draw_Path(PathType *path);
		bool Follow_Edge(CELL start, CELL target, PathType *path, FacingType search, FacingType olddir, int threat, int threat_stage, int max_cells, MoveType threshhold);
		bool Register_Cell(PathType *path, CELL cell, FacingType dir, int cost, MoveType threshhold);
		bool Unravel_Loop(PathType *path, CELL &cell, FacingType &dir, int sx, int sy, int dx, int dy, MoveType threshhold);

		/*
		**	This is the coordinate that the unit is heading to
		**	as an immediate destination. This coordinate is never further
		**	than once cell (or track) from the unit's location. When this coordinate
		**	is reached, then the next location in the path list becomes the
		**	next HeadTo coordinate.
		*/
		COORDINATE HeadToCoord;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[16];
};

#endif

