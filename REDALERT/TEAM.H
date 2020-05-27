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

/* $Header: /CounterStrike/TEAM.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TEAM.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/11/94                                                     *
 *                                                                                             *
 *                  Last Update : December 11, 1994 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef TEAM_H
#define TEAM_H


#include	"wwfile.h"
#include	"teamtype.h"
#include	"abstract.h"

/*
** Units are only allowed to stray a certain distance away from their
** team.  When they exceed this distance, some sort of fixup must be
** done.
*/
#define STRAY_DISTANCE		2

class TeamClass : public AbstractClass
{
	public:
		/*
		**	This specifies the type of team this is.
		*/
		CCPtr<TeamTypeClass> Class;

		/*
		**	This specifies the owner of this team.
		*/
		CCPtr<HouseClass> House;

		/*
		**	This flag forces the team into active state regardless of whether it
		**	is understrength or not.
		*/
		unsigned IsForcedActive:1;

		/*
		**	This flag is set to true when the team initiates into active mode. The
		**	flag is never cleared. By examining this flag, it is possible to determine
		**	if the team has ever launched into active mode.
		*/
		unsigned IsHasBeen:1;

		/*
		**	If the team is full strength, then this flag is true. A full strength
		**	team will not try to recruit members.
		*/
		unsigned IsFullStrength:1;

		/*
		**	A team that is below half strength has this flag true. It means that the
		**	the team should hide back at the owner's base and try to recruit
		**	members.
		*/
		unsigned IsUnderStrength:1;

		/*
		**	If a team is not understrength but is not yet full strength, then
		**	the team is regrouping.  If this flag is set and the team becomes
		**	full strength, the all members of the team will become initiated
		** and this flag will be reset.
		*/
		unsigned IsReforming:1;

		/*
		** This bit should be set if a team is determined to have lagging
		** units in its formation.
		*/
		unsigned IsLagging:1;

		/*
		**	If a team member was removed or added, then this flag will be set to true. The
		**	team system uses this flag to tell whether it should recalculate the team
		**	under strength or full strength flags. This process does not need to occur
		**	EVERY time a unit added or deleted from a team, just every so often if the
		**	team has been changed.
		*/
		unsigned IsAltered:1;
		unsigned JustAltered:1;

		/*
		**	If the team is working on it's primary mission (it is past the build up stage)
		**	then this flag will be true. The transition between "moving" and "stationary"
		**	stages usually requires some action on the team's part.
		*/
		unsigned IsMoving:1;

		/*
		**	When the team determines that the next mission should be advanced to, it will
		**	set this flag to true. Mission advance will either change the behavior of the
		**	team or cause it to disband.
		*/
		unsigned IsNextMission:1;

		/*
		**	If at least one member of this team successfully left the map, then this
		**	flag will be set. At the time the team is terminated, if this flag is true, then
		**	if there are any triggers that depend upon this team leaving, they will be
		**	sprung.
		*/
		unsigned IsLeaveMap:1;

		/*
		** Records whether the team is suspended from production.
		*/
		unsigned Suspended:1;

		/*
		**	A team will have a center point. This is the point used to determine if
		**	any member of the team is "too far" from the team and must return. This
		**	center point is usually calculated as the average position of all the
		**	team members.
		*/
		TARGET Zone;

		/*
		**	This is the target value of the team member that is closest to the
		**	destination of the team. The implied location serves as the
		**	regroup point for the unit as it is moving.
		*/
		TARGET ClosestMember;

		/*
		**	This is the target of the team. Typically, it is a unit or structure, but
		**	for the case of teams with a movement mission, it might represent a
		**	destination cell.
		*/
		TARGET MissionTarget;
		TARGET Target;

		/*
		**	This is the total number of members in this team.
		*/
		int Total;

		/*
		**	This is the teams combined risk value
		*/
		int Risk;

		/*
		**	If this team is assigned a formation, then the formation type
		**	will be stored here. If the formation type is FORMATION_NONE, then
		**	the team is a loose grouping -- just like C&C.
		*/
		FormationType Formation;

		/*
		** This is the amount of time the team is suspended for.
		*/
		CDTimerClass<FrameTimerClass> SuspendTimer;

		/*
		**	If there is a trigger that should be attached to every member of this team, then
		**	it is pointed to by this.
		*/
		CCPtr<TriggerClass> Trigger;

		//------------------------------------------------------------
		TeamClass(TeamTypeClass const * team=0, HouseClass * owner=0);
		TeamClass(NoInitClass const & x) : AbstractClass(x), Class(x), House(x), SuspendTimer(x), Trigger(x), TimeOut(x), Member(x) {};
		virtual ~TeamClass(void);
		static void operator delete(void *ptr);
		static void * operator new(size_t size);
		static void * operator new(size_t, void * ptr) {return(ptr);};
		static void Init(void);
		static void Suspend_Teams(int priority, HouseClass const * house);
		void Debug_Dump(MonoClass * mono) const;

		/*
		**	File I/O.
		*/
		bool Load(Straw & file);
		bool Save(Pipe & file) const;
		void Code_Pointers(void);
		void Decode_Pointers(void);

		bool Is_Empty(void) const {return(Member == (void*)NULL);}
		bool Has_Entered_Map(void) const;
		void Force_Active(void) {IsForcedActive = true;IsUnderStrength=false;};
		bool Remove(FootClass *, int typeindex=-1);
		void Detach(TARGET target, bool all);
		void AI(void);
		void Took_Damage(FootClass * obj, ResultType result, TechnoClass * source);
		bool Add(FootClass *);
		bool Can_Add(FootClass * obj, int & typeindex) const;
		void Assign_Mission_Target(TARGET new_target);
		bool Is_Leaving_Map(void) const;
		void Scan_Limit(void);

	private:

		/*
		**	The current mission index into the mission list is recorded here.
		*/
		int CurrentMission;

		/*
		**	Some missions will time out. This is the timer that keeps track of the
		**	time to transition between missions.
		*/
		CDTimerClass<FrameTimerClass> TimeOut;

		int TMission_Formation(void);
		int TMission_Attack(void);
		int TMission_Spy(void);
		int TMission_Follow(void);
		int TMission_Loop(void);
		int TMission_Load(void);
		int TMission_Unload(void);
		int TMission_Invulnerable(void);
		int TMission_Set_Global(void);
		int TMission_Patrol(void);
		int TMission_Deploy(void);
		bool Coordinate_Regroup(void);
		void Coordinate_Attack(void);
		void Coordinate_Move(void);
		bool Coordinate_Conscript(FootClass * unit);
		void Coordinate_Do(void);
		void Calc_Center(TARGET &center, TARGET &obj_center) const;
		int Recruit(int typeindex);
		bool Is_A_Member(void const * who) const;
		bool Lagging_Units(void);
		FootClass * Fetch_A_Leader(void) const;

		/*
		**	Points to the first member in the list of members for this team.
		*/
		SmartPtr<FootClass> Member;

		unsigned char Quantity[TeamTypeClass::MAX_TEAM_CLASSCOUNT];

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];

};

#endif