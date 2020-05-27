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

/* $Header:   F:\projects\c&c\vcs\code\team.h_v   2.16   16 Oct 1995 16:48:04   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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

#include	<wwfile.h>
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
#ifdef USE_RA_AI
	friend class HouseClass;		       // Needed for RA AI moved into C&C. ST - 7/24/2019 2:44PM
#endif // USE_RA_AI

	public:
		/*
		**	This specifies the type of team this is.
		*/
		TeamTypeClass const * const Class;

		/*
		**	This specifies the owner of this team.
		*/
		HouseClass * const House;

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

	private:
		/*
		**	If a team member was removed or added, then this flag will be set to true. The
		**	team system uses this flag to tell whether it should recalculate the team
		**	under strength or full strength flags. This process does not need to occur
		**	EVERY time a unit added or deleted from a team, just every so often if the
		**	team has been changed.
		*/
		unsigned IsAltered:1;

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
		** Records whether the team is suspended from production.
		*/
		unsigned Suspended:1;

	public:
		/*
		**	A team will have a center point. This is the point used to determine if
		**	any member of the team is "too far" from the team and must return. This
		**	center point is usually calculated as the average position of all the
		**	team members.
		*/
		CELL Center;
		CELL ObjectiveCenter;

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
		** This is the amount of time the team is suspended for.
		*/
		TCountDownTimerClass SuspendTimer;

		//------------------------------------------------------------
		TeamClass(void) : Class(0), House(0) {IsActive=false;Member=0;IsAltered=true;};
		TeamClass(TeamTypeClass const * team, HouseClass * owner);
		virtual ~TeamClass(void);
		virtual RTTIType What_Am_I(void) const {return RTTI_TEAM;};
		static void operator delete(void *ptr);
		static void * operator new(size_t size);
		static void Init(void);
		static void Suspend_Teams(int priority);

		TARGET As_Target(void) const;

		/*
		**	File I/O.
		*/
		bool  Load(FileClass & file);
		bool  Save(FileClass & file);
		void  Code_Pointers(void);
		void  Decode_Pointers(void);

		void  Force_Active(void) {IsForcedActive = true;IsUnderStrength=false;};
		bool  Remove(FootClass *, int typeindex=-1);
		void  Detach(TARGET target, bool all);
		void  AI(void);
		void  Took_Damage(FootClass * obj, ResultType result, TechnoClass * source);
		bool  Add(FootClass *, int typeindex=-1);
		void  Assign_Mission_Target(TARGET new_target);

		/*
		**	Dee-buggin' support.
		*/
		int Validate(void) const;

		/*
		**	This is a record of the current number of active teams of each
		**	type. It can range from zero to MaxAllowed.
		*/
		static unsigned char Number[TEAMTYPE_MAX];

	private:

		/*
		**	The current mission index into the mission list is recorded here.
		*/
		int CurrentMission;

		/*
		**	Some missions will time out. This is the timer that keeps track of the
		**	time to transition between missions.
		*/
		TCountDownTimerClass TimeOut;

		void  Coordinate_Unload(void);
		bool  Coordinate_Regroup(void);
		void  Coordinate_Attack(void);
		void  Coordinate_Move(void);
		void  Coordinate_Conscript(FootClass * unit);
//		void Control(FootClass *, bool initial=false);
		void  Calc_Center(CELL &center, CELL &obj_center) const;
		int  Recruit(int typeindex);
		bool  Is_A_Member(void const * who) const;
		bool  Lagging_Units(void);

		/*
		**	Points to the first member in the list of members for this team.
		*/
		FootClass * Member;

		unsigned char Quantity[TeamTypeClass::MAX_TEAM_CLASSCOUNT];

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];

		/*
		**	This records the success of each team type. As the team carries out its
		**	mission, it increments this counter if it considers the mission
		**	to have been successfully completed. Teams with greater success
		**	will be created more than the others.
		*/
		static unsigned char Success[TEAMTYPE_MAX];

		/*
		** This contains the value of the Virtual Function Table Pointer
		*/
		static void * VTable;
};

#endif