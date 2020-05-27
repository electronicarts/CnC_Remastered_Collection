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

/* $Header: /CounterStrike/EVENT.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : EVENT.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/09/94                                                     *
 *                                                                                             *
 *                  Last Update : November 10, 1995 [BRR]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   EventClass::EventClass -- Construct an id and cell based event.                           *
 *   EventClass::EventClass -- Construct simple target type event.                             *
 *   EventClass::EventClass -- Constructor for mission change events.                          *
 *   EventClass::EventClass -- Constructor for navigation computer events.                     *
 *   EventClass::EventClass -- Constructor for object types affecting cells event.             *
 *   EventClass::EventClass -- Constructor for sidebar build events.                           *
 *   EventClass::EventClass -- Constructs event to transfer special flags.                     *
 *   EventClass::EventClass -- Default constructor for event objects.                          *
 *   EventClass::EventClass -- Event for sequencing animations.                                *
 *   EventClass::EventClass -- Megamission assigned to unit.                                   *
 *   EventClass::Execute -- Execute a queued command.                                          *
 *   EventClass::EventClass -- construct a variable-sized event                                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#ifdef WIN32
#include "ccdde.h"
#endif	//WIN32

#ifdef FIXIT_VERSION_3		//	Stalemate games.
#include "WolStrng.h"
#endif

/***************************************************************************
** Table of what data is really used in the EventClass struct for different
** events.  This table must be kept current with the EventType enum.
*/
unsigned char EventClass::EventLength[EventClass::LAST_EVENT] = {
	0,															// EMPTY
	size_of(EventClass, Data.General ),				// ALLY
	size_of(EventClass, Data.MegaMission ),		// MEGAMISSION
	size_of(EventClass, Data.MegaMission_F ),		// MEGAMISSION_F
	size_of(EventClass, Data.Target ),				// IDLE
	size_of(EventClass, Data.Target ),				// SCATTER
	0,															// DESTRUCT
	0,															// DEPLOY
	size_of(EventClass, Data.Place ),				// PLACE
	0,															// OPTIONS
	size_of(EventClass, Data.General ),				// GAMESPEED
	size_of(EventClass, Data.Specific ),			// PRODUCE
	size_of(EventClass, Data.Specific.Type ),		// SUSPEND
	size_of(EventClass, Data.Specific.Type ),		// ABANDON
	size_of(EventClass, Data.Target ),				// PRIMARY
	size_of(EventClass, Data.Special ),				// SPECIAL_PLACE
	0,															// EXIT
	size_of(EventClass, Data.Anim ),					// ANIMATION
	size_of(EventClass, Data.Target ),				// REPAIR
	size_of(EventClass, Data.Target ),				// SELL
	size_of(EventClass, Data.SellCell),				// SELLCELL
	size_of(EventClass, Data.Options ),				// SPECIAL
	0,															// FRAMESYNC
	0,															//	MESSAGE
	size_of(EventClass, Data.FrameInfo.Delay ),	// RESPONSE_TIME
	size_of(EventClass, Data.FrameInfo ),			// FRAMEINFO
	0,															//	SAVEGAME
	size_of(EventClass, Data.NavCom ),				// ARCHIVE
	size_of(EventClass, Data.Variable.Size),		// ADDPLAYER
	size_of(EventClass, Data.Timing ),				// TIMING
	size_of(EventClass, Data.ProcessTime ),		// PROCESS_TIME
#ifdef FIXIT_VERSION_3		//	Stalemate games.
	0,												//	PROPOSE_DRAW
	0,												//	RETRACT_DRAW
#endif
};

char * EventClass::EventNames[EventClass::LAST_EVENT] = {
	"EMPTY",
	"ALLY",
	"MEGAMISSION",
	"MEGAMISSION_F",
	"IDLE",
	"SCATTER",
	"DESTRUCT",
	"DEPLOY",
	"PLACE",
	"OPTIONS",
	"GAMESPEED",
	"PRODUCE",
	"SUSPEND",
	"ABANDON",
	"PRIMARY",
	"SPECIAL_PLACE",
	"EXIT",
	"ANIMATION",
	"REPAIR",
	"SELL",
	"SELLCELL",
	"SPECIAL",
	"FRAMESYNC",
	"MESSAGE",
	"RESPONSE_TIME",
	"FRAMEINFO",
	"SAVEGAME",
	"ARCHIVE",
	"ADDPLAYER",
	"TIMING",
	"PROCESS_TIME",
#ifdef FIXIT_VERSION_3		//	Stalemate games.
	"PROPOSE_DRAW",
	"RETRACT_DRAW",
#endif
};


/***********************************************************************************************
 * EventClass::EventClass -- Constructs event to transfer special flags.                       *
 *                                                                                             *
 *    This constructs an event that will transfer the special flags.                           *
 *                                                                                             *
 * INPUT:   data  -- The special flags to be transported to all linked computers.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(SpecialClass data)
{
	ID = PlayerPtr->ID;
	Type = SPECIAL;
	Frame = ::Frame;
	Data.Options.Data = data;
}


/***********************************************************************************************
 * EventClass::EventClass -- Construct simple target type event.                               *
 *                                                                                             *
 *    This will construct a generic event that needs only a target parameter. The actual       *
 *    event and target values are specified as parameters.                                     *
 *                                                                                             *
 * INPUT:   type  -- The event type to construct.                                              *
 *                                                                                             *
 *          target-- The target value that this event is to apply to.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(EventType type, TargetClass target)
{
	ID = PlayerPtr->ID;
	Type = type;
	Frame = ::Frame;
	Data.Target.Whom = target;
}


EventClass::EventClass(EventType type, CELL cell)
{
	ID = PlayerPtr->ID;
	Type = type;
	Frame = ::Frame;
	Data.SellCell.Cell = cell;
}


/***********************************************************************************************
 * EventClass::EventClass -- Default constructor for event objects.                            *
 *                                                                                             *
 *    This constructs a simple event object that requires no parameters other than the         *
 *    type of event it is.                                                                     *
 *                                                                                             *
 * INPUT:   type  -- The type of event to construct.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(EventType type)
{
	ID = PlayerPtr->ID;
	Type = type;
	Frame = ::Frame;
}


/***********************************************************************************************
 * EventClass::EventClass -- Constructor for general-purpose-data events.                      *
 *                                                                                             *
 * INPUT:   type  -- The type of event to construct.                                           *
 *            val   -- data value                                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(EventType type, int val)
{
	ID = PlayerPtr->ID;
	Type = type;
	Data.General.Value = val;
	Frame = ::Frame;
}


/***********************************************************************************************
 * EventClass::EventClass -- Constructor for navigation computer events.                       *
 *                                                                                             *
 *    Constructor for events that are used to assign the navigation computer.                  *
 *                                                                                             *
 * INPUT:   type     -- The type of event (this constructor can be used by other navigation    *
 *                      type events).                                                          *
 *                                                                                             *
 *          src      -- The object that the event should apply to.                             *
 *                                                                                             *
 *          dest     -- The destination (or target) that the event needs to complete.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(EventType type, TargetClass src, TargetClass dest)
{
	ID = PlayerPtr->ID;
	Type = type;
	Frame = ::Frame;
	Data.NavCom.Whom = src;
	Data.NavCom.Where = TargetClass(dest);
}


/***********************************************************************************************
 * EventClass::EventClass -- Event for sequencing animations.                                  *
 *                                                                                             *
 *    This constructor is used for animations that must be created through the event system.   *
 *                                                                                             *
 * INPUT:   anim  -- The animation that will be created.                                       *
 *                                                                                             *
 *          coord -- The location where the animation is to be created.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(AnimType anim, HousesType owner, COORDINATE coord)
{
	ID = PlayerPtr->ID;
	Type = ANIMATION;
	Frame = ::Frame;
	Data.Anim.What = anim;
	Data.Anim.Owner = owner;
	Data.Anim.Where = coord;
}


/***********************************************************************************************
 * EventClass::EventClass -- Megamission assigned to unit.                                     *
 *                                                                                             *
 *    This is the event that is used to assign most missions to units. It combines both the    *
 *    mission and the target (navcom and tarcom).                                              *
 *                                                                                             *
 * INPUT:   src      -- The object that this mission is to apply to.                           *
 *                                                                                             *
 *          mission  -- The mission to assign to this object.                                  *
 *                                                                                             *
 *          target   -- The target to assign to this object's TarCom.                          *
 *                                                                                             *
 *          destination -- The destination to assign to this object's NavCom.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(TargetClass src, MissionType mission, TargetClass target, TargetClass destination)
{
	ID = PlayerPtr->ID;
	Type = MEGAMISSION;
	Frame = ::Frame;
	Data.MegaMission.Whom = src;
	Data.MegaMission.Mission = mission;
	Data.MegaMission.Target = target;
	Data.MegaMission.Destination = destination;
}


/***********************************************************************************************
 * EventClass::EventClass -- Megamission assigned to unit.                                     *
 *                                                                                             *
 *    This is the event that is used to assign most missions to units. It combines both the    *
 *    mission and the target (navcom and tarcom).  This variation is used for formation moves. *
 *                                                                                             *
 * INPUT:   src      -- The object that this mission is to apply to.                           *
 *                                                                                             *
 *          mission  -- The mission to assign to this object.                                  *
 *                                                                                             *
 *          target   -- The target to assign to this object's TarCom.                          *
 *                                                                                             *
 *          destination -- The destination to assign to this object's NavCom.                  *
 *                                                                                             *
 *          speed    -- The formation override speed for this move.                            *
 *                                                                                             *
 *          maxspeed -- The formation override maximum speed for this move.                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(TargetClass src, MissionType mission, TargetClass target, TargetClass destination, SpeedType speed, MPHType maxspeed)
{
	ID = PlayerPtr->ID;
	Type = MEGAMISSION_F;
	Frame = ::Frame;
	Data.MegaMission_F.Whom = src;
	Data.MegaMission_F.Mission = mission;
	Data.MegaMission_F.Target = TargetClass(target);
	Data.MegaMission_F.Destination = TargetClass(destination);
	Data.MegaMission_F.Speed = speed;
	Data.MegaMission_F.MaxSpeed = maxspeed;
}


/***********************************************************************************************
 * EventClass::EventClass -- Constructor for sidebar build events.                             *
 *                                                                                             *
 *    This constructor is used for events that deal with an object type and an object ID.      *
 *    Typically, this is used exclusively by the sidebar.                                      *
 *                                                                                             *
 * INPUT:   type     -- The event type of this object.                                         *
 *                                                                                             *
 *          object   -- The object type number.                                                *
 *                                                                                             *
 *          id       -- The object sub-type number.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(EventType type, RTTIType object, int id)
{
	ID = PlayerPtr->ID;
	Type = type;
	Frame = ::Frame;
	Data.Specific.Type = object;
	Data.Specific.ID = id;
}


/***********************************************************************************************
 * EventClass::EventClass -- Constructor for object types affecting cells event.               *
 *                                                                                             *
 *    This constructor is used for those events that have an object type and associated cell.  *
 *    Typically, this is for building placement after construction has completed.              *
 *                                                                                             *
 * INPUT:   type     -- The event type for this object.                                        *
 *                                                                                             *
 *          object   -- The object type number (actual object is probably inferred from the    *
 *                      sidebar data).                                                         *
 *                                                                                             *
 *          cell     -- The cell location where this event is to occur.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(EventType type, RTTIType object, CELL cell)
{
	ID = PlayerPtr->ID;
	Type = type;
	Frame = ::Frame;
	Data.Place.Type = object;
	Data.Place.Cell = cell;
}


/***********************************************************************************************
 * EventClass::EventClass -- Construct an id and cell based event.                             *
 *                                                                                             *
 *    This constructor is used for those events that require an ID number and a cell location. *
 *                                                                                             *
 * INPUT:   type  -- The event type this will be.                                              *
 *                                                                                             *
 *          id    -- The arbitrary id number to assign.                                        *
 *                                                                                             *
 *          cell  -- The location for this event.                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(EventType type, int id, CELL cell)
{
	ID = PlayerPtr->ID;
	Type = type;
	Frame = ::Frame;
	Data.Special.ID 	= id;
	Data.Special.Cell = cell;
}


/***********************************************************************************************
 * EventClass::EventClass -- construct a variable-sized event                                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		ptr		ptr to data associated with this event														  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		none.																												  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		none.																												  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(EventType type, void * ptr, unsigned long size)
{
	ID = PlayerPtr->ID;
	Type = type;
	Frame = ::Frame;
	Data.Variable.Pointer = ptr;
	Data.Variable.Size = size;
}


/***********************************************************************************************
 * EventClass::Execute -- Execute a queued command.                                            *
 *                                                                                             *
 *    This routine executes an event. The even must already have been confirmed by any         *
 *    remote machine before calling this routine.                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void EventClass::Execute(void)
{
	TechnoClass * techno;
	AnimClass * anim = 0;
	HouseClass * house = 0;
//	CELL cell = 0;
	char txt[80];
	bool formation = false;
//	RTTIType rt;

	if (Debug_Print_Events) {
		printf("(%d) Executing %s ID:%d Frame:%d ",
			::Frame, EventNames[Type], ID, Frame);
	}

	switch (Type) {

		/*
		**	Update the archive target for this building.
		*/
		case ARCHIVE:
			techno = Data.NavCom.Whom.As_Techno();
			if (techno && techno->IsActive) {
				techno->ArchiveTarget = Data.NavCom.Where.As_TARGET();
			}
			break;

		/*
		**	Make or break alliance.
		*/
		case ALLY:
			house = Houses.Raw_Ptr(Data.General.Value);
			if (Houses.Raw_Ptr(ID)->Is_Ally(house)) {
				Houses.Raw_Ptr(ID)->Make_Enemy((HousesType)Data.General.Value);
			} else {
				Houses.Raw_Ptr(ID)->Make_Ally((HousesType)Data.General.Value);
			}
			break;

		/*
		**	Special self destruct action requested. This is active in the multiplayer mode.
		*/
		case DESTRUCT:
			Houses.Raw_Ptr(ID)->Flag_To_Die();
			break;

		/*
		**	Update the special control flags. This is necessary so that in a multiplay
		**	game, all machines will agree on the rules. If these options change during
		**	game play, then all players are informed that options have changed.
		*/
		case SPECIAL:
			{
				Special = Data.Options.Data;
				HouseClass * house = Houses.Raw_Ptr(ID);

				//sprintf(txt, Text_String(TXT_SPECIAL_WARNING), house->Name);	// Should be IniName? ST - 5/8/2019
				sprintf(txt, Text_String(TXT_SPECIAL_WARNING), house->IniName);
				Session.Messages.Add_Message(NULL, 0, txt,
					house->RemapColor,
					TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 1200);
				Map.Flag_To_Redraw(false);
			}
			break;

		/*
		**	Starts or stops repair on the specified object. This event is triggered by the
		**	player clicking the repair wrench on a building.
		*/
		case REPAIR:
			techno = Data.Target.Whom.As_Techno();
			if (techno && techno->IsActive) {
				techno->Repair(-1);
			}
			break;

		/*
		**	Tells a building/unit to sell. This event is triggered by the player clicking the
		**	sell animating cursor over the building or unit.
		*/
		case SELL:
			techno = Data.Target.Whom.As_Techno();
			if (techno && techno->IsActive && techno->House == Houses.Raw_Ptr(ID)) {
				if (techno->What_Am_I() == RTTI_BUILDING || (techno->What_Am_I() == RTTI_UNIT && Map[techno->Center_Coord()].Cell_Building() != 0)) {
					techno->Sell_Back(-1);
				}
			} else {
//				if (Is_Target_Cell(Data.Target.Whom)) {
//					Houses.Raw_Ptr(ID)->Sell_Wall(As_Cell(Data.Target.Whom));
//				}
			}
			break;

		/*
		**	Tells the wall at the specified location to sell off.
		*/
		case SELLCELL:
//			cell = Data.SellCell.Cell;
			Houses.Raw_Ptr(ID)->Sell_Wall(Data.SellCell.Cell);
			break;

		/*
		**	This even is used to trigger an animation that is generated as a direct
		**	result of player intervention.
		*/
		case ANIMATION:
			anim = new AnimClass(Data.Anim.What, Data.Anim.Where);
			if (anim) {
				//2019/09/19 JAS - Visibility needs to be determined per player
				if (Data.Anim.Owner == HOUSE_NONE || Data.Anim.What != ANIM_MOVE_FLASH)
				{
					anim->Set_Visible_Flags(0xffff);
				}
				else
				{
					anim->Set_Visible_Flags(1 << Data.Anim.Owner);
				}
			}
			break;

		/*
		**	This event will place the specified object at the specified location.
		**	The event is used to place newly constructed buildings down on the map. The
		**	object type is specified. From this object type, the house can determine the
		**	exact factory and real object pointer to use.
		*/
		case PLACE:
			Houses.Raw_Ptr(ID)->Place_Object(Data.Place.Type, Data.Place.Cell);
			break;

		/*
		**	This event starts production of the specified object type. The house can
		**	determine from the type and ID value, what object to begin production on and
		**	what factory to use.
		*/
		case PRODUCE:
			Houses.Raw_Ptr(ID)->Begin_Production(Data.Specific.Type, Data.Specific.ID);
			break;

		/*
		**	This event is generated when the player puts production on hold. From the
		**	object type, the factory can be inferred.
		*/
		case SUSPEND:
			Houses.Raw_Ptr(ID)->Suspend_Production(Data.Specific.Type);
			break;

		/*
		**	This event is generated when the player cancels production of the specified
		**	object type. From the object type, the exact factory can be inferred.
		*/
		case ABANDON:
			Houses.Raw_Ptr(ID)->Abandon_Production(Data.Specific.Type);
			break;

		/*
		**	Toggles the primary factory state of the specified building.
		*/
		case PRIMARY:
			{
				BuildingClass * building = Data.Target.Whom.As_Building();
				if (building && building->IsActive) {
					building->Toggle_Primary();
				}
			}
			break;

		/*
		**	This is the general purpose mission control event. Most player
		**	action routes through this event. It sets a unit's mission, TarCom,
		**	and NavCom to the values specified.
		*/
		case MEGAMISSION_F:
			techno = Data.MegaMission_F.Whom.As_Techno();
			if (techno && techno->IsActive && techno->Is_Foot()) {
				((FootClass *)techno)->IsFormationMove = true;
				((FootClass *)techno)->FormationSpeed = Data.MegaMission_F.Speed;
				((FootClass *)techno)->FormationMaxSpeed = Data.MegaMission_F.MaxSpeed;
				FormMove = true;
				FormSpeed = Data.MegaMission_F.Speed;
				FormMaxSpeed = Data.MegaMission_F.MaxSpeed;
				formation = true;
			}
			// Fall thru to next case...

		case MEGAMISSION:
			if (Debug_Print_Events) {
				printf("Whom:%x Tgt:%x Dest:%x ",
					Data.MegaMission.Whom.As_TARGET(),
					Data.MegaMission.Target.As_TARGET(),
					Data.MegaMission.Destination.As_TARGET());
			}
			techno = Data.MegaMission.Whom.As_Techno();
			if (techno != NULL && techno->IsActive && techno->Strength > 0 && !techno->IsInLimbo) {

				/*
				**	Fetch a pointer to the object of the mission. If there is an error with
				**	this object, such as it is dead, then bail.
				*/
				ObjectClass * object = NULL;
				if (Data.MegaMission.Target.Is_Valid()) {
					object = Data.MegaMission.Target.As_Object();
					if (object != NULL && (!object->IsActive || object->Strength == 0 || object->IsInLimbo)) {
						break;
//						object = NULL;
//						Data.MegaMission.Target.Invalidate();
					}
				}

				/*
				**	If the destination target is invalid because the object is dead, then
				**	bail from processing this mega mission.
				*/
				if (Data.MegaMission.Destination.Is_Valid()) {
					object = Data.MegaMission.Destination.As_Object();
					if (object != NULL && (!object->IsActive || object->Strength == 0 || object->IsInLimbo)) {
						break;
//						object = NULL;
//						Data.MegaMission.Destination.Invalidate();
					}
				}

				/*
				**	Break any existing tether or team contact, since it is now invalid.
				*/
				if (!techno->IsTethered) {
					techno->Transmit_Message(RADIO_OVER_OUT);
				}
				if (techno->Is_Foot()) {
					if (!formation) ((FootClass *)techno)->IsFormationMove = false;
					if (((FootClass *)techno)->Team) {
						((FootClass *)techno)->Team->Remove((FootClass *)techno);
					}
				}

				if (object != NULL) {

					// 2019/09/20 JAS - Added record of who clicked on the object
					HouseClass* house = Houses.Raw_Ptr(ID);
					bool is_allied = house != nullptr && house->Is_Ally(techno);
					if (is_allied) {
						object->Clicked_As_Target((HousesType)ID);
					}
				}

				/*
				**	Test to see if the navigation target should really be queued rather
				**	than assigned to the object. This would be the case if this is a
				**	special queued move mission and there is already a valid navigation
				**	target for this unit.
				*/
				bool q = (Data.MegaMission.Mission == MISSION_QMOVE);

				techno->Assign_Mission(Data.MegaMission.Mission);

				if (techno->Is_Foot()) {
					((FootClass*)techno)->SuspendedNavCom = TARGET_NONE;
				}
				techno->SuspendedTarCom = TARGET_NONE;

				/*
				**	Guard area mode is handled with care. The specified target is actually
				**	assigned as the location that should be guarded. In addition, the
				**	movement destination is immediately set to this new location.
				*/
				if (Data.MegaMission.Mission == MISSION_GUARD_AREA && techno->Is_Foot()) {
					techno->Assign_Target(TARGET_NONE);
					techno->Assign_Destination(Data.MegaMission.Target.As_TARGET());
					techno->ArchiveTarget = Data.MegaMission.Target.As_TARGET();
				} else {
					if (q && techno->Is_Foot()) {
						((FootClass *)techno)->Queue_Navigation_List(Data.MegaMission.Destination.As_TARGET());
					} else {
						if (techno->Is_Foot()) {
							((FootClass *)techno)->Clear_Navigation_List();
						}
						techno->Assign_Target(Data.MegaMission.Target.As_TARGET());
						techno->Assign_Destination(Data.MegaMission.Destination.As_TARGET());
					}
				}

				//
				// Special case for ship repairing: If the assigned mission is to
				// move, and 'techno' is a Vessel:
				//   If the destination is a shipyard or sub pen, set the IsToSelfRepair flag
				//   Otherwise, clear both IsToSelfRepair and IsSelfRepairing
				//
				RTTIType rt = techno->What_Am_I();
//				rt = Data.MegaMission.Whom;
				if (rt == RTTI_VESSEL && techno != NULL && techno->What_Am_I() == RTTI_VESSEL) {
					VesselClass *ship = (VesselClass *)techno;
					if (Data.MegaMission.Mission == MISSION_MOVE) {
						if (object != NULL) {
							if (object->What_Am_I() == RTTI_BUILDING &&
								((BuildingClass *)object)->House->Is_Ally(techno) &&
//							if ((RTTIType)Data.MegaMission.Destination == RTTI_BUILDING &&
								(((BuildingClass *)object)->Class->Type == STRUCT_SHIP_YARD ||
								((BuildingClass *)object)->Class->Type == STRUCT_SUB_PEN)) {
								ship->IsToSelfRepair = true;
							} else {
								ship->IsToSelfRepair = false;
								ship->IsSelfRepairing = false;
							}
						} else {
							ship->IsToSelfRepair = false;
							ship->IsSelfRepairing = false;
						}
					} else {
						ship->IsToSelfRepair = false;
						ship->IsSelfRepairing = false;
					}
				}

#ifdef NEVER
				if ((techno->What_Am_I() == RTTI_UNIT || techno->What_Am_I() == RTTI_INFANTRY) &&
						Data.MegaMission.Mission == MISSION_GUARD_AREA) {

					((FootClass *)techno)->ArchiveTarget = Data.MegaMission.Destination;
				}
#endif
			}
			break;

		/*
		**	Request that the unit/infantry/aircraft go into idle mode.
		*/
		case IDLE:
			techno = Data.Target.Whom.As_Techno();
			if (techno != NULL && techno->IsActive && !techno->IsInLimbo && !techno->IsTethered && techno->What_Am_I() != RTTI_BUILDING) {
				techno->Transmit_Message(RADIO_OVER_OUT);
				techno->Assign_Destination(TARGET_NONE);
				techno->Assign_Target(TARGET_NONE);
				techno->Enter_Idle_Mode();
				if (techno->Is_Foot()) {
					((FootClass *)techno)->Clear_Navigation_List();
				}
			}
			break;

		/*
		**	Request that the unit/infantry/aircraft scatter from its current location.
		*/
		case SCATTER:
			techno = Data.Target.Whom.As_Techno();
			if (techno != NULL && techno->Is_Foot() && techno->IsActive && !techno->IsInLimbo && !techno->IsTethered) {
				((FootClass *)techno)->IsScattering = true;
				techno->Scatter(0, true, false);
			}
			break;

		/*
		**	If we are placing down the ion cannon blast then lets take
		** care of it.
		*/
		case SPECIAL_PLACE:
			Houses.Raw_Ptr(ID)->Place_Special_Blast((SpecialWeaponType)Data.Special.ID, Data.Special.Cell);
			break;

		/*
		**	Exit the game.
		** Give parting message while palette is fading to black.
		*/
		case EXIT:
			Theme.Queue_Song(THEME_NONE);
			Stop_Speaking();
			Speak(VOX_CONTROL_EXIT);
			while (Is_Speaking()) {
				Call_Back();
			}
			GameActive = false;
			break;

		/*
		**	Process the options menu, unless we're playing back a recording.
		*/
		case OPTIONS:
			if (!Session.Play) {
				SpecialDialog = SDLG_OPTIONS;
			}
			break;

		/*
		**	Process the options Game Speed
		*/
		case GAMESPEED:
			Options.GameSpeed = Data.General.Value;
			break;

		/*
		**	Adjust connection timing for multiplayer games
		*/
		case RESPONSE_TIME:
			Session.MaxAhead = Data.FrameInfo.Delay;
			break;

		/*
		**	Save a multiplayer game (this event is only generated in multiplayer mode)
		*/
		case SAVEGAME:
			/*
			** Show the user what's going on with a message box (but only if
			** we're not already inside a dialog box routine!)
			*/
			if (SpecialDialog == SDLG_NONE) {
				CDTimerClass<SystemTimerClass> timer;
				//timer.Start();
				timer = TICKS_PER_SECOND * 4;

				WWMessageBox().Process(TXT_SAVING_GAME, TXT_NONE);

				Save_Game (-1, (char *)Text_String(TXT_MULTIPLAYER_GAME));

				while (timer > 0) {
					Call_Back();
				}

				HidPage.Clear();
				Map.Flag_To_Redraw(true);
				Map.Render();
			}
			else {
				Save_Game (-1, (char *)Text_String(TXT_MULTIPLAYER_GAME));
			}
			break;

		/*
		** Add a new player to the game:
		** - Form a network connection to him
		** - Add his name, ID, House etc to our list of players
		** - Re-sort the ID array
		** - Place his units on the map
		*/
		case ADDPLAYER:
			int i;
			printf("ADDPLAYER EVENT!\n");
			for (i=0;i<(int)Data.Variable.Size;i++) {
				printf("%d\n", ((char *)Data.Variable.Pointer)[i]);
			}
			if (ID != PlayerPtr->ID) {
				delete [] Data.Variable.Pointer;
			}
			break;

		//
		// This event tells all systems to use new timing values.  It's like
		// RESPONSE_TIME, only it works.  It's only used with the
		// COMM_MULTI_E_COMP protocol.
		//
		case TIMING:
#if(TIMING_FIX)
			//
			// If MaxAhead is about to increase, we're vulnerable to a Packet-
			// Received-Too-Late error, if any system generates an event after
			// this TIMING event, but before it executes.  So, record the
			// period of vulnerability's frame start & end values, so we
			// can reschedule these events to execute after it's over.
			//
			if (Data.Timing.MaxAhead > Session.MaxAhead) {
				NewMaxAheadFrame1 = Frame;
				NewMaxAheadFrame2 = Frame + Data.Timing.MaxAhead;
			}
#endif
			Session.DesiredFrameRate = Data.Timing.DesiredFrameRate;
			Session.MaxAhead = Data.Timing.MaxAhead;

#ifndef WOLAPI_INTEGRATION
			/*
			** If spawned from WChat then we should be getting poked every minute. If not then
			** deliberately break the max ahead value
			*/
#ifdef WIN32
			if (Special.IsFromWChat) {
				Session.MaxAhead += DDEServer.Time_Since_Heartbeat()/(70*60);
			}
#endif	//WIN32
#endif	//	!WOLAPI_INTEGRATION
			if (Debug_Print_Events) {
				printf("DesiredFrameRate:%d MaxAhead:%d ",
					Session.DesiredFrameRate,
					Session.MaxAhead);
			}

			break;

		//
		// This event tells all systems what the other systems' process
		// timing requirements are; it's used to compute a desired frame rate
		// for the game.
		//
		case PROCESS_TIME:
			for (i = 0; i < Session.Players.Count(); i++) {
				if (ID == Session.Players[i]->Player.ID) {
					Session.Players[i]->Player.ProcessTime = Data.ProcessTime.AverageTicks;
					break;
				}
			}
			break;

#ifdef FIXIT_VERSION_3		//	Stalemate games.
		case PROPOSE_DRAW:
			if( ID == PlayerPtr->ID )
			{
				if( Scen.bOtherProposesDraw )
				{
					//	Both sides agree to draw. Game will end in a tie.
					Scen.bLocalProposesDraw = true;
					break;
				}
				Scen.bLocalProposesDraw = true;
				//PG Session.Messages.Add_Message( NULL, 0, TXT_WOL_DRAW_PROPOSED_LOCAL, PCOLOR_GOLD, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE );
			}
			else
			{
				if( Scen.bLocalProposesDraw )
				{
					//	Both sides agree to draw. Game will end in a tie.
					Scen.bOtherProposesDraw = true;
					break;
				}
				char szMessage[ 100 ];
				for (i = 0; i < Session.Players.Count(); i++) 
				{
					if (ID == Session.Players[i]->Player.ID) 
					{
						//PG sprintf( szMessage, TXT_WOL_DRAW_PROPOSED_OTHER, Session.Players[i]->Name );
						break;
					}
				}
				Scen.bOtherProposesDraw = true;
				Session.Messages.Add_Message( NULL, 0, szMessage, PCOLOR_GOLD, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE );
			}
			Sound_Effect( VOC_INCOMING_MESSAGE );
			break;

		case RETRACT_DRAW:
			if( ID == PlayerPtr->ID )
			{
				Scen.bLocalProposesDraw = false;
				//PG Session.Messages.Add_Message( NULL, 0, TXT_WOL_DRAW_RETRACTED_LOCAL, PCOLOR_GOLD, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE );
			}
			else
			{
				char szMessage[ 100 ];
				for (i = 0; i < Session.Players.Count(); i++) 
				{
					if (ID == Session.Players[i]->Player.ID) 
					{
						//PG sprintf( szMessage, TXT_WOL_DRAW_RETRACTED_OTHER, Session.Players[i]->Name );
						break;
					}
				}
				Scen.bOtherProposesDraw = false;
				Session.Messages.Add_Message( NULL, 0, szMessage, PCOLOR_GOLD, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE );
			}
			Sound_Effect( VOC_INCOMING_MESSAGE );
			break;
#endif

		/*
		**	Default: do nothing.
		*/
		default:
			break;
	}

	if (Debug_Print_Events) {
		printf("\n");
	}

}
