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

/* $Header:   F:\projects\c&c\vcs\code\event.cpv   2.17   16 Oct 1995 16:50:28   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : June 25, 1995 [JLB]                                          *
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
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "ccdde.h"

/***************************************************************************
** Table of what data is really used in the EventClass struct for different
** events.  This table must be kept current with the EventType enum.
*/
unsigned char EventClass::EventLength[EventClass::LAST_EVENT] = {
	0,															// EMPTY
	size_of(EventClass, Data.General ),				// ALLY
	size_of(EventClass, Data.MegaMission ),		// MEGAMISSION
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
	size_of(EventClass, Data.Options ),				// SPECIAL
	0,															// FRAMESYNC
	0,															//	MESSAGE
	size_of(EventClass, Data.FrameInfo.Delay ),	// RESPONSE_TIME
	size_of(EventClass, Data.FrameInfo ),			// FRAMEINFO
	size_of(EventClass, Data.NavCom ),			// ARCHIVE
	size_of(EventClass, Data.Timing ),				// TIMING
	size_of(EventClass, Data.ProcessTime ),		// PROCESS_TIME
};

char * EventClass::EventNames[EventClass::LAST_EVENT] = {
	"EMPTY",
	"ALLY",
	"MEGAMISSION",
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
	"SPECIAL",
	"FRAMESYNC",
	"MESSAGE",
	"RESPONSE_TIME",
	"FRAMEINFO",
	"ARCHIVE",
	"TIMING",
	"PROCESS_TIME",
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
	ID = Houses.ID(PlayerPtr);
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
EventClass::EventClass(EventType type, TARGET target)
{
	ID = Houses.ID(PlayerPtr);
	Type = type;
	Frame = ::Frame;
	Data.Target.Whom = target;
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
	ID = Houses.ID(PlayerPtr);
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
	ID = Houses.ID(PlayerPtr);
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
EventClass::EventClass(EventType type, TARGET src, TARGET dest)
{
	ID = Houses.ID(PlayerPtr);
	Type = type;
	Frame = ::Frame;
	Data.NavCom.Whom = src;
	Data.NavCom.Where = dest;
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
	ID = Houses.ID(PlayerPtr);
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
EventClass::EventClass(TARGET src, MissionType mission, TARGET target, TARGET destination)
{
	ID = Houses.ID(PlayerPtr);
	Type = MEGAMISSION;
	Frame = ::Frame;
	Data.MegaMission.Whom = src;
	Data.MegaMission.Mission = mission;
	Data.MegaMission.Target = target;
	Data.MegaMission.Destination = destination;
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
	ID = Houses.ID(PlayerPtr);
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
	ID = Houses.ID(PlayerPtr);
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
	ID = Houses.ID(PlayerPtr);
	Type = type;
	Frame = ::Frame;
	Data.Special.ID 	= id;
	Data.Special.Cell = cell;
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
	char txt[80];
	int i;
//#if (0)
if (Type < 0 || Type > PROCESS_TIME){
char tempbuf[128];
sprintf (tempbuf, "Packet type %d received\n", Type);
CCDebugString (tempbuf);

sprintf (tempbuf, "	ID = %d\n", ID);
CCDebugString (tempbuf);

sprintf (tempbuf, "	Frame = %d\n", Frame);
CCDebugString (tempbuf);

sprintf (tempbuf, "	MPlayer ID = %d\n", MPlayerID);
CCDebugString (tempbuf);

}
//#endif	//(0)


	switch (Type) {
		/*
		**	Update the archive target for this building.
		*/
		case ARCHIVE:
			techno = As_Techno(Data.NavCom.Whom);
			if (techno && techno->IsActive) {
				techno->ArchiveTarget = Data.NavCom.Where;
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
CCDebugString ("C&C95 - Resignation packet received\n");
			Houses.Raw_Ptr(ID)->Flag_To_Die();
			Houses.Raw_Ptr(ID)->Resigned = true;
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

				sprintf(txt, Text_String(TXT_SPECIAL_WARNING), house->Name);
				Messages.Add_Message(txt, MPlayerTColors[house->RemapColor],
					TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 1200, 0, 0);
				Map.Flag_To_Redraw(false);
			}
			break;

		/*
		**	Starts or stops repair on the specified object. This event is triggered by the
		**	player clicking the repair wrench on a building.
		*/
		case REPAIR:
CCDebugString ("C&C95 - Repair packet received\n");
			techno = As_Techno(Data.Target.Whom);
			if (techno && techno->IsActive) {
				techno->Repair(-1);
			}
			break;

		/*
		**	Tells a building/unit to sell. This event is triggered by the player clicking the
		**	sell animating cursor over the building or unit.
		*/
		case SELL:
CCDebugString ("C&C95 - Sell packet received\n");
			techno = As_Techno(Data.Target.Whom);
			if (techno && techno->IsActive && techno->House == Houses.Raw_Ptr(ID)) {
				techno->Sell_Back(-1);
			} else {
				if (Is_Target_Cell(Data.Target.Whom)) {
					Houses.Raw_Ptr(ID)->Sell_Wall(As_Cell(Data.Target.Whom));
				}
			}
			break;

		/*
		**	This even is used to trigger an animation that is generated as a direct
		**	result of player intervention.
		*/
		case ANIMATION:
			anim = new AnimClass(Data.Anim.What, Data.Anim.Where);
			if (anim) {
				//2019/09/19 JAS - Visibility needs to be determined per player
				if (Data.Anim.What != ANIM_MOVE_FLASH || Data.Anim.Owner == HOUSE_NONE || Special.IsVisibleTarget)
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
CCDebugString ("C&C95 - Place packet received\n");
			Houses.Raw_Ptr(ID)->Place_Object(Data.Place.Type, Data.Place.Cell);
			break;

		/*
		**	This event starts production of the speicified object type. The house can
		**	determine from the type and ID value, what object to begin production on and
		**	what factory to use.
		*/
		case PRODUCE:
CCDebugString ("C&C95 - Produce packet received\n");
			Houses.Raw_Ptr(ID)->Begin_Production(Data.Specific.Type, Data.Specific.ID);
			break;

		/*
		**	This event is generated when the player puts production on hold. From the
		**	object type, the factory can be inferred.
		*/
		case SUSPEND:
CCDebugString ("C&C95 - Suspend packet received\n");
			Houses.Raw_Ptr(ID)->Suspend_Production(Data.Specific.Type);
			break;

		/*
		**	This event is generated when the player cancels production of the specified
		**	object type. From the object type, the exact factory can be inferred.
		*/
		case ABANDON:
CCDebugString ("C&C95 - Abandon packet received\n");
			Houses.Raw_Ptr(ID)->Abandon_Production(Data.Specific.Type);
			break;

		/*
		**	Toggles the primary factory state of the specified building.
		*/
		case PRIMARY:{
CCDebugString ("C&C95 - Primary building packet received\n");
				BuildingClass * building = As_Building(Data.Target.Whom);
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
		case MEGAMISSION:
			techno = As_Techno(Data.MegaMission.Whom);
			if (techno && techno->IsActive) {

				/*
				**	Fetch a pointer to the object of the mission.
				*/
				ObjectClass * object;
				if (Target_Legal(Data.MegaMission.Target)) {
					object = As_Object(Data.MegaMission.Target);
				} else {
					object = As_Object(Data.MegaMission.Destination);
				}

				/*
				**	Break any existing team contact, since it is now invalid.
				*/
				if (!techno->IsTethered) {
					techno->Transmit_Message(RADIO_OVER_OUT);
				}
				switch (techno->What_Am_I()) {
					case RTTI_INFANTRY:
					case RTTI_UNIT:
						if (((FootClass *)techno)->Team) {
							((FootClass *)techno)->Team->Remove((FootClass *)techno);
						}
						break;
				}

				if (object) {
					
					// 2019/09/20 JAS - Added record of who clicked on the object
					HouseClass* house = Houses.Raw_Ptr(ID);
					bool is_allied = house != nullptr && house->Is_Ally(techno);
					if (is_allied || Special.IsVisibleTarget) {
						object->Clicked_As_Target((HousesType)ID); 
					}
				}
				techno->Assign_Mission(Data.MegaMission.Mission);

				/*
				**	Guard area mode is handled with care. The specified target is actually
				**	assigned as the location that should be guarded. In addition, the
				**	movement destination is immediately set to this new location.
				*/
				if (Data.MegaMission.Mission == MISSION_GUARD_AREA &&
//					Target_Legal(Data.MegaMission.Target) &&
					(techno->What_Am_I() == RTTI_INFANTRY || techno->What_Am_I() == RTTI_UNIT || techno->What_Am_I() == RTTI_AIRCRAFT)) {

					techno->ArchiveTarget = Data.MegaMission.Target;
					techno->Assign_Target(TARGET_NONE);
					techno->Assign_Destination(Data.MegaMission.Target);
				} else {
					techno->Assign_Target(Data.MegaMission.Target);
					techno->Assign_Destination(Data.MegaMission.Destination);
				}

#ifdef NEVER
				if ((techno->What_Am_I() == RTTI_UNIT || techno->What_Am_I() == RTTI_INFANTRY) &&
						Data.MegaMission.Mission == MISSION_GUARD_AREA) {

					techno->ArchiveTarget = Data.MegaMission.Destination;
				}
#endif
			}
			break;

		/*
		**	Request that the unit/infantry/aircraft go into idle mode.
		*/
		case IDLE:
			techno = As_Techno(Data.Target.Whom);
			if (techno && techno->IsActive && !techno->IsInLimbo && !techno->IsTethered) {
				techno->Assign_Destination(TARGET_NONE);
				techno->Assign_Target(TARGET_NONE);
				techno->Enter_Idle_Mode();
			}
			break;

		/*
		**	Request that the unit/infantry/aircraft scatter from its current location.
		*/
		case SCATTER:
			techno = As_Techno(Data.Target.Whom);
			if (techno && techno->IsActive && !techno->IsInLimbo && !techno->IsTethered) {
				techno->Scatter(0, true);
			}
			break;

		/*
		**	If we are placing down the ion cannon blast then lets take
		** care of it.
		*/
		case SPECIAL_PLACE:
CCDebugString ("C&C95 - Special blast packet received\n");
			Houses.Raw_Ptr(ID)->Place_Special_Blast((SpecialWeaponType)Data.Special.ID, Data.Special.Cell);
			break;

		/*
		**	Exit the game.
		** Give parting message while palette is fading to black.
		*/
		case EXIT:
CCDebugString ("C&C95 - Exit game packet received\n");
			Theme.Queue_Song(THEME_NONE);
			Stop_Speaking();
			Speak(VOX_CONTROL_EXIT);
			while (Is_Speaking()) {
				Call_Back();
			}
			GameActive = false;
			break;

		/*
		**	Process the options menu.
		*/
		case OPTIONS:
			SpecialDialog = SDLG_OPTIONS;
			break;

		/*
		**	Process the options Game Speed
		*/
		case GAMESPEED:
CCDebugString ("C&C95 - Game speed packet received\n");
			Options.GameSpeed = Data.General.Value;
			break;

		/*
		**	Adjust connection timing for multiplayer games
		*/
		case RESPONSE_TIME:
char flip[128];
sprintf (flip, "C&C95 - Changing MaxAhead to %d frames\n", Data.FrameInfo.Delay);
CCDebugString (flip);
			MPlayerMaxAhead = Data.FrameInfo.Delay;
			break;

		//
		// This event tells all systems to use new timing values.  It's like
		// RESPONSE_TIME, only it works.  It's only used with the
		// COMM_MULTI_E_COMP protocol.
		//
		case TIMING:
CCDebugString ("C&C95 - Timing packet received\n");
//#if(TIMING_FIX)
			//
			// If MaxAhead is about to increase, we're vulnerable to a Packet-
			// Received-Too-Late error, if any system generates an event after
			// this TIMING event, but before it executes.  So, record the
			// period of vulnerability's frame start & end values, so we
			// can reschedule these events to execute after it's over.
			//
			if (Data.Timing.MaxAhead > MPlayerMaxAhead) {
				NewMaxAheadFrame1 = Frame;
				NewMaxAheadFrame2 = Frame + Data.Timing.MaxAhead;
			}
//#endif

			DesiredFrameRate = Data.Timing.DesiredFrameRate;
			MPlayerMaxAhead = Data.Timing.MaxAhead;

sprintf (flip, "C&C95 -  Timing packet: DesiredFrameRate = %d\n", Data.Timing.DesiredFrameRate);
CCDebugString (flip);
sprintf (flip, "C&C95 -  Timing packet: MaxAhead = %d\n", Data.Timing.MaxAhead);
CCDebugString (flip);

			/*
			** If spawned from WChat then we should be getting poked every minute. If not then
			** deliberately break the max ahead value
			*/
			if (Special.IsFromWChat){
				MPlayerMaxAhead += DDEServer.Time_Since_Heartbeat()/(70*60);
//if (DDEServer.Time_Since_Heartbeat() >= 70*60) CCDebugString ("C&C95 - Missed a heartbeat\n");
			}
			break;

		//
		// This event tells all systems what the other systems' process
		// timing requirements are; it's used to compute a desired frame rate
		// for the game.
		//
		case PROCESS_TIME:
			for (i = 0; i < MPlayerCount; i++) {
				if (MPlayerID == ::MPlayerID[i]) {
					TheirProcessTime[i] = Data.ProcessTime.AverageTicks;

char flip[128];
sprintf (flip, "C&C95 - Received PROCESS_TIME packet of %04x ticks\n", Data.ProcessTime.AverageTicks);
CCDebugString (flip);

					break;
				}
			}
			break;

		/*
		**	Default: do nothing.
		*/
		default:
			break;
	}
}
