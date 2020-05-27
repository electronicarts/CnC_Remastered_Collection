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

/* $Header: /CounterStrike/MISSION.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MISSION.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 23, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 23, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MISSION_H
#define MISSION_H

#include "object.h"
#include	"monoc.h"

/****************************************************************************
**	This handles order assignment and tracking. The order is used to guide
**	overall AI processing.
*/
class MissionClass : public ObjectClass
{
	public:

		/*
		**	This the tactical strategy to use. It is used by the unit script. This
		**	is a general guide for unit AI processing.
		*/
		MissionType Mission;
		MissionType SuspendedMission;

		/*
		**	The order queue is used for orders that should take effect when the vehicle
		**	has reached the center point of a cell. The queued order number is +1 when stored here
		**	so that 0 will indicated there is no queued order.
		*/
		MissionType MissionQueue;

		int Status;

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		MissionClass(RTTIType rtti, int id);
		MissionClass(NoInitClass const & x) : ObjectClass(x), Timer(x) {};
		virtual ~MissionClass(void) {};

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		#ifdef CHEAT_KEYS
		void  Debug_Dump(MonoClass *mono) const;
		#endif

		void Shorten_Mission_Timer(void) {Timer = 0;}
		virtual MissionType Get_Mission(void) const;
		virtual void  Assign_Mission(MissionType mission);
		virtual bool Commence(void);
		virtual void AI(void);

		/*
		**	Support functions.
		*/
		virtual int Mission_Sleep(void);
		virtual int Mission_Ambush(void);
		virtual int Mission_Attack(void);
		virtual int Mission_Capture(void);
		virtual int Mission_Guard(void);
		virtual int Mission_Guard_Area(void);
		virtual int Mission_Harvest(void);
		virtual int Mission_Hunt(void);
//		virtual int Mission_Timed_Hunt(void);
		virtual int Mission_Move(void);
		virtual int Mission_Retreat(void);
		virtual int Mission_Return(void);
		virtual int Mission_Stop(void);
		virtual int Mission_Unload(void);
		virtual int Mission_Enter(void);
		virtual int Mission_Construction(void);
		virtual int Mission_Deconstruction(void);
		virtual int Mission_Repair(void);
		virtual int Mission_Missile(void);
		virtual void  Set_Mission(MissionType mission);
		static bool Is_Recruitable_Mission(MissionType mission);

		static char const *  Mission_Name(MissionType order);
		static MissionType  Mission_From_Name(char const *name);
		virtual void  Override_Mission(MissionType mission, TARGET, TARGET);
		virtual bool Restore_Mission(void);

	private:

		/*
		**	This the thread processing timer. When this value counts down to zero, then
		**	more script processing may occur.
		*/
		CDTimerClass<FrameTimerClass> Timer;
};


/****************************************************************************
**	This is the mission control (pun) that controls how each mission behaves
**	when it comes to interacting with the game world. Example; some
**	missions allow the object to scatter from threats, while others require
**	the object to remain in place. This kind of characteristics are specfied
**	by this class.
*/
class MissionControlClass
{
	public:
		MissionControlClass(void);

		bool Read_INI(CCINIClass & ini);
		int Normal_Delay(void) const {return(TICKS_PER_MINUTE * Rate);}
		int AA_Delay(void) const {return(TICKS_PER_MINUTE * AARate);}

		/*
		**	This is the mission identifier that this mission represents.
		*/
		MissionType Mission;

		char const * Name(void) const;

		/*
		**	If the object should not be considered a threat when it
		**	comes to target scanning, then this will be true.
		*/
		unsigned IsNoThreat:1;

		/*
		**	If objects in this mission should avoid targeting the enemy and
		**	also avoid responding to the enemy, then this will be true.
		*/
		unsigned IsZombie:1;

		/*
		**	An ojbect that can be recruited into a team must be on a mission
		**	of this type.
		*/
		unsigned IsRecruitable:1;

		/*
		**	If the object can behave normally except that it cannot
		**	move to another location, then this flag will be true.
		*/
		unsigned IsParalyzed:1;

		/*
		**	If an object on this mission is damaged, it is allowed to
		**	retaliate?
		*/
		unsigned IsRetaliate:1;

		/*
		**	Is the object allowed to scatter from immediate threats?
		*/
		unsigned IsScatter:1;

		/*
		**	This specifies the time to delay between calls to the mission handler for those cases
		**	where the delay could be indefinate. The exception would be when timing is critical.
		**	Typical use of this would be to regulate the delay between mundane mission processing
		**	in order to achieve less game overhead.
		*/
		fixed Rate;

		/*
		**	Anti-Aircraft buildings (and units) in guard or guard area mode will use this override
		**	delay interval instead of the normal "Rate" value.
		*/
		fixed AARate;
};


#endif
