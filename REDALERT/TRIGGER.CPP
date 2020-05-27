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

/* $Header: /CounterStrike/TRIGGER.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TRIGGER.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/12/94                                                     *
 *                                                                                             *
 *                  Last Update : August 13, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Find_Or_Make -- Find or create a trigger of the type specified.                           *
 *   TriggerClass::As_Target -- Converts trigger to a target value                             *
 *   TriggerClass::Attaches_To -- Determines what trigger can attach to.                       *
 *   TriggerClass::Description -- Fetch a one line ASCII description of the trigger.           *
 *   TriggerClass::Detach -- Detach specified target from this trigger.                        *
 *   TriggerClass::Draw_It -- Draws this trigger as if it were part of a list box.             *
 *   TriggerClass::Init -- clears triggers for new scenario                                    *
 *   TriggerClass::Spring -- Spring the trigger (possibly).                                    *
 *   TriggerClass::TriggerClass -- constructor                                                 *
 *   TriggerClass::operator delete -- Returns a trigger to the special memory pool.            *
 *   TriggerClass::operator new -- 'new' operator                                              *
 *   TriggerClass::~TriggerClass -- Destructor for trigger objects.                            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


#if defined(CHEAT_KEYS) || defined(SCENARIO_EDITOR)
/***********************************************************************************************
 * TriggerClass::Description -- Fetch a one line ASCII description of the trigger.             *
 *                                                                                             *
 *    In the rare (possibly never?) case of requiring a description for this trigger, then     *
 *    just have the model class generate a description and return that.                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with an ASCII description of this trigger.                                 *
 *                                                                                             *
 * WARNINGS:   see TriggerTypeClass::Description()                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * TriggerClass::Description(void) const
{
	return(Class->Description());
}


/***********************************************************************************************
 * TriggerClass::Draw_It -- Draws this trigger as if it were part of a list box.               *
 *                                                                                             *
 *    This routine is called when the trigger has been assigned to a list box. It will         *
 *    display a description of the trigger.                                                    *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerClass::Draw_It(int , int x, int y, int width, int height, bool selected, TextPrintType flags) const
{
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();
	static int _tabs[] = {13,40};
	if ((flags & 0x0F) == TPF_6PT_GRAD || (flags & 0x0F) == TPF_EFNT) {

		if (selected) {
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect(x, y, x + width - 1, y + height - 1, scheme->Shadow);
		} else {
			if (!(flags & TPF_USE_GRAD_PAL)) {
				flags = flags | TPF_MEDIUM_COLOR;
			}
		}

		Conquer_Clip_Text_Print(Description(), x, y, scheme, TBLACK, flags, width, _tabs);
	} else {
		Conquer_Clip_Text_Print(Description(), x, y, (selected ? &ColorRemaps[PCOLOR_DIALOG_BLUE] : &ColorRemaps[PCOLOR_GREY]), TBLACK, flags, width, _tabs);
	}
}
#endif


/***********************************************************************************************
 * TriggerClass::TriggerClass -- constructor                                                   *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
TriggerClass::TriggerClass(TriggerTypeClass * trigtype) :
	RTTI(RTTI_TRIGGER),
	ID(Triggers.ID(this)),
	Class(trigtype),
	AttachCount(0),
	Cell(0)
{
	Class->Event1.Reset(Event1);
	Class->Event2.Reset(Event2);
}


/***********************************************************************************************
 * TriggerClass::~TriggerClass -- Destructor for trigger objects.                              *
 *                                                                                             *
 *    This destructor will update the house blockage value if necessary. No other action need  *
 *    be performed on trigger destruction.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerClass::~TriggerClass(void)
{
	if (GameActive && Class.Is_Valid() && (Class->Attaches_To() & ATTACH_GENERAL) != 0) {
		if (LogicTriggerID >= LogicTriggers.ID(this)) {
			LogicTriggerID--;
			if (LogicTriggerID < 0 && LogicTriggers.Count() == 0) {
				LogicTriggerID = 0;
			}
		}
	}

	if (GameActive && Class.Is_Valid() && (Class->Attaches_To() & ATTACH_MAP) != 0) {
		if (MapTriggerID >= MapTriggers.ID(this)) {
			MapTriggerID--;
			if (MapTriggerID < 0 && MapTriggers.Count() == 0) {
				MapTriggerID = 0;
			}
		}
	}

	if (GameActive && Class->House != HOUSE_NONE && Class->Action1.Action == TACTION_ALLOWWIN) {
		if (Houses.Ptr(Class->House)->Blockage) Houses.Ptr(Class->House)->Blockage--;
		Houses.Ptr(Class->House)->BorrowedTime = TICKS_PER_SECOND*4;
	}
	ID = -1;
}


/***********************************************************************************************
 * TriggerClass::Init -- clears triggers for new scenario                                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void TriggerClass::Init(void)
{
	Triggers.Free_All();
}


/***********************************************************************************************
 * TriggerClass::Spring -- Spring the trigger (possibly).                                      *
 *                                                                                             *
 *    This routine is called when a potential trigger even has occurred. The event is matched  *
 *    with the trigger event needed by this trigger. If the condition warrants, the trigger    *
 *    action is performed.                                                                     *
 *                                                                                             *
 * INPUT:   event    -- The event that is occurring.                                           *
 *                                                                                             *
 *          obj      -- If the trigger is attached to an object, this points to the object.    *
 *                                                                                             *
 *          cell     -- If the trigger is attached to a cell, this is the cell number.         *
 *                                                                                             *
 *          forced   -- Should the trigger be forced to execute regardless of the event?       *
 *                                                                                             *
 * OUTPUT:  bool; Was the trigger sprung?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1996 JLB : Created.                                                                 *
 *   08/13/1996 JLB : Linked triggers supported.                                               *
 *=============================================================================================*/
bool TriggerClass::Spring(TEventType event, ObjectClass * obj, CELL cell, bool forced)
{
	assert(Triggers.ID(this) == ID);

	bool e1 = Class->Event1(Event1, event, Class->House, obj, forced);
	bool e2 = false;
	bool execute = false;

	/*
	**	Forced triggers must presume that the cell parameter is invalid. It then
	**	uses the embedded cell value in the trigger as the official location where
	**	the trigger will detonate at.
	*/
	if (forced) {
		cell = Cell;
	} else {

		/*
		**	Determine if the trigger event is considered to have been sprung according to the
		**	event control value. This might require that both events be triggered, one event
		**	triggered, or either event triggered to activate the trigger action.
		*/
		switch (Class->EventControl) {
			case MULTI_ONLY:
				execute = e1;
				break;

			case MULTI_AND:
				e2 = Class->Event2(Event2, event, Class->House, obj, forced);
				execute = (e1 && e2);
				break;

			case MULTI_LINKED:
			case MULTI_OR:
				e2 = Class->Event2(Event2, event, Class->House, obj, forced);
				execute = (e1 || e2);
				break;
		}
	}

	/*
	**	See if the trigger is sprung with a qualifying event.
	*/
	if (execute || forced) {

		/*
		**	Verify that the trigger event should really be sprung. Exceptions
		**	would include semi-persistent triggers that don't actually
		**	spring until all triggers have sprung.
		*/
		if (Class->IsPersistant == TriggerTypeClass::SEMIPERSISTANT) {

			/*
			** Detach ourselves from the object and record that there
			**	is one less attachment to keep track of.
			*/
			if (obj) {
				obj->Trigger = NULL;
			}
			if (cell) {
				Map[cell].Trigger = NULL;
			}

			/*
			** If we're attached to more objects, don't spring; otherwise, spring.
			** And, mark ourselves as volatile so we'll completely remove ourselves
			** from the game after we go off.
			*/
			AttachCount--;
			if (AttachCount > 0) {
				return(false);
			}
		}

		/*
		**	For linked trigger events, perform the action associated with the matching
		**	trigger event. Otherwise perform the action for both events.
		*/
		bool ok = false;
		HousesType hh = Class->House;
		if (Class->EventControl == MULTI_LINKED) {
			if (e1 || forced) ok |= Class->Action1(hh, obj, ID, cell);
			if (e2 && !forced) ok |= Class->Action2(hh, obj, ID, cell);
		} else {

			switch (Class->ActionControl) {
				case MULTI_ONLY:
					ok |= Class->Action1(hh, obj, ID, cell);
					break;

				default:
				case MULTI_AND:
					ok |= Class->Action1(hh, obj, ID, cell);
					ok |= Class->Action2(hh, obj, ID, cell);
					break;
			}
		}
		if (!IsActive) return(true);

		/*
		**	If at least one action was performed, then consider this
		**	trigger to have completed and thus will be deleted if
		**	necessary.
		*/
		if (ok) {
			#ifdef CHEAT_KEYS
			MonoArray[DMONO_STRESS].Sub_Window(61, 1, 17, 11);
			MonoArray[DMONO_STRESS].Scroll();
			MonoArray[DMONO_STRESS].Sub_Window(61, 1, 18, 11);
			MonoArray[DMONO_STRESS].Set_Cursor(0, 10);
			MonoArray[DMONO_STRESS].Printf("%02d:%02d:%02d-%s", Scen.Timer / TICKS_PER_HOUR, (Scen.Timer % TICKS_PER_HOUR)/TICKS_PER_MINUTE, (Scen.Timer % TICKS_PER_MINUTE)/TICKS_PER_SECOND, Class->IniName);
			MonoArray[DMONO_STRESS].Sub_Window();
			#endif

			if (Class->IsPersistant == TriggerTypeClass::VOLATILE || (Class->IsPersistant == TriggerTypeClass::SEMIPERSISTANT && AttachCount <= 1)) {
				Detach_This_From_All(As_Target(), true);
				delete this;
				return(true);
			} else {

				/*
				**	Reset event data so that the event will
				**	repeat as necessary.
				*/
				Class->Event1.Reset(Event1);
				Class->Event2.Reset(Event2);
			}
		}
	}

	return(false);
}


/***********************************************************************************************
 * TriggerClass::operator new -- 'new' operator                                                *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      pointer to new trigger                                                                 *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void * TriggerClass::operator new(size_t )
{
	void * ptr = Triggers.Allocate();
	if (ptr) {
		((TriggerClass *)ptr)->IsActive = true;
	}

	return(ptr);
}


/***********************************************************************************************
 * TriggerClass::operator delete -- Returns a trigger to the special memory pool.              *
 *                                                                                             *
 *    This routine will return a previously allocated trigger object back to the memory        *
 *    pool from which it came.                                                                 *
 *                                                                                             *
 * INPUT:   pointer  -- Pointer to the trigger to return to the memory pool.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerClass::operator delete(void * pointer)
{
	if (pointer) {
		((TriggerClass *)pointer)->IsActive = false;
	}
	Triggers.Free((TriggerClass *)pointer);
}


/***********************************************************************************************
 * TriggerClass::As_Target -- Converts trigger to a target value                               *
 *                                                                                             *
 *    Converts the trigger object into a target identifier.                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  TARGET value                                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET TriggerClass::As_Target(void) const
{
	assert(Triggers.ID(this) == ID);

	return(Build_Target(RTTI_TRIGGER, ID));
}


/***********************************************************************************************
 * Find_Or_Make -- Find or create a trigger of the type specified.                             *
 *                                                                                             *
 *    This routine is used when, given a trigger type, an actual trigger object is needed.     *
 *    In this case, an existing trigger of the correct type must be located, or a trigger      *
 *    object must be created. In either case, this routine will return a trigger object that   *
 *    corresponds to the trigger type class specified.                                         *
 *                                                                                             *
 * INPUT:   trigtype -- Pointer to the trigger type to find (or create) a matching trigger     *
 *                      object.                                                                *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to a matching trigger object. If no more triggers could be       *
 *          allocated and no matching trigger could be found, then this routine will return    *
 *          NULL (a very rare case).                                                           *
 *                                                                                             *
 * WARNINGS:   This routine could return NULL.                                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerClass * Find_Or_Make(TriggerTypeClass * trigtype)
{
	if (!trigtype) return(NULL);

	for (int index = 0; index < Triggers.Count(); index++) {
		if (trigtype == Triggers.Ptr(index)->Class) {
			return(Triggers.Ptr(index));
		}
	}

	/*
	**	No trigger was found, so make one.
	*/
	TriggerClass * trig = new TriggerClass(trigtype);
	return(trig);
}


/***********************************************************************************************
 * TriggerClass::Detach -- Detach specified target from this trigger.                          *
 *                                                                                             *
 *    This routine is called when the specified trigger MUST be detached from all references   *
 *    to it. The only reference maintained by a trigger is the reference to the trigger        *
 *    type class it is modeled after.                                                          *
 *                                                                                             *
 * INPUT:   target   -- The target identifier to remove all attachments to.                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   You must never detach the trigger type class from a trigger. Such a process     *
 *             will leave the trigger orphan and in a 'crash the game immediately if used'     *
 *             state. As such, this routine will throw an assertion if this is tried.          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerClass::Detach(TARGET target, bool )
{
	if (Is_Target_TriggerType(target)) {
		assert((TriggerTypeClass*)Class != As_TriggerType(target));
//		if (Class == As_TriggerType(target)) {
//			Class = NULL;
//		}
	}
}
