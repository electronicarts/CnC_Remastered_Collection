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

/* $Header: /CounterStrike/TRIGTYPE.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TRIGTYPE.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/05/96                                                     *
 *                                                                                             *
 *                  Last Update : July 9, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TriggerTypeClass::As_Target -- Convert this trigger type object into a target value.      *
 *   TriggerTypeClass::Attaches_To -- Determines what trigger can attach to.                   *
 *   TriggerTypeClass::Build_INI_Entry -- Construct the INI entry into the buffer specified.   *
 *   TriggerTypeClass::Description -- Build a text description of the trigger type.            *
 *   TriggerTypeClass::Detach -- Removes attachments to the target object specified.           *
 *   TriggerTypeClass::Draw_It -- Draws this trigger as if it were a line in a list box.       *
 *   TriggerTypeClass::Edit -- Edit the trigger type through the scenario editor.              *
 *   TriggerTypeClass::Fill_In -- fills in trigger from the given INI entry                    *
 *   TriggerTypeClass::From_Name -- Convert an ASCII name into a trigger type pointer.         *
 *   TriggerTypeClass::Init -- Initialize the trigger type object management system.           *
 *   TriggerTypeClass::Read_INI -- reads triggers from the INI file                            *
 *   TriggerTypeClass::TriggerTypeClass -- Constructor for trigger class object.               *
 *   TriggerTypeClass::Write_INI -- Stores all trigger types to the INI database specified.    *
 *   TriggerTypeClass::operator delete -- Returns a trigger type class object back to the pool *
 *   TriggerTypeClass::operator new -- Allocates a trigger type class object.                  *
 *   TriggerTypeClass::~TriggerTypeClass -- Deleting a trigger type deletes associated triggers*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"
#include	"trigtype.h"


/***********************************************************************************************
 * TriggerTypeClass::TriggerTypeClass -- Constructor for trigger class object.                 *
 *                                                                                             *
 *    This is the normal constructor for a trigger object. The trigger starts with no team     *
 *    members, no mission, and default values for all settings.                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerTypeClass::TriggerTypeClass(void) :
	AbstractTypeClass(RTTI_TRIGGERTYPE, TriggerTypes.ID(this), TXT_NONE, "x"),
	IsPersistant(VOLATILE),
	EventControl(MULTI_ONLY),
	ActionControl(MULTI_ONLY),
	House(HOUSE_SPAIN)
{
}


/***********************************************************************************************
 * TriggerTypeClass::~TriggerTypeClass -- Deleting a trigger type deletes associated triggers. *
 *                                                                                             *
 *    When a trigger type is deleted, then all triggers that refer to that type must also      *
 *    be deleted as well. There can be no 'orphan' triggers in existence.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerTypeClass::~TriggerTypeClass(void)
{
}


/***********************************************************************************************
 * TriggerTypeClass::operator new -- Allocates a trigger type class object.                    *
 *                                                                                             *
 *    This routine will allocate a block of memory from the special trigger type object        *
 *    pool.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the allocated trigger type memory block. If there is     *
 *          no more block available in the pool, then NULL is returned.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * TriggerTypeClass::operator new(size_t )
{
	void * ptr = TriggerTypes.Allocate();
	if (ptr) {
		((TriggerTypeClass *)ptr)->IsActive = true;
	}

	return(ptr);
}


/***********************************************************************************************
 * TriggerTypeClass::operator delete -- Returns a trigger type class object back to the pool   *
 *                                                                                             *
 *    This routine will return a previously allocated trigger type object to the private       *
 *    memory pool from which it was allocated.                                                 *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the trigger type class to return to the pool.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::operator delete(void * ptr)
{
	if (ptr) {
		((TriggerTypeClass *)ptr)->IsActive = false;
	}
	TriggerTypes.Free((TriggerTypeClass *)ptr);
}


/***********************************************************************************************
 * TriggerTypeClass::As_Target -- Convert this trigger type object into a target value.        *
 *                                                                                             *
 *    Use this routine to take this trigger type class object and convert it into a            *
 *    target number.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the target number that represents this trigger type class object.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET TriggerTypeClass::As_Target(void) const
{
	return(Build_Target(RTTI_TRIGGERTYPE, ID));
}


/***********************************************************************************************
 * TriggerTypeClass::Detach -- Removes attachments to the target object specified.             *
 *                                                                                             *
 *    When an object disappears from the game, it must be detached from all other objects that *
 *    may be referring to it. This routine will detach the specified target object from any    *
 *    references to it in this trigger type class.                                             *
 *                                                                                             *
 * INPUT:   target   -- The target object to be detached from this trigger type.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::Detach(TARGET target, bool)
{
	Action1.Detach(target);
	Action2.Detach(target);
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * TriggerTypeClass::Edit -- Edit the trigger type through the scenario editor.                *
 *                                                                                             *
 *    This is the scenario editor interface to a trigger type class object. It brings up a     *
 *    fancy schmancy dialog to allow full edit control of the trigger type.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the "OK" button pressed? A false return value indicates that the edits   *
 *                to this trigger type class object should be rejected.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TriggerTypeClass::Edit(void)
{
	enum {
		/*
		**	Dialog position and dimensions.
		*/
		D_DIALOG_W = 320 + 100,
		D_DIALOG_H = 200 + 20,
		D_DIALOG_X = 0,
		D_DIALOG_Y = 0,

		/*
		**	Event entry list box coordinates and dimensions.
		*/
		E1_X=D_DIALOG_X+45,
		E1_Y=D_DIALOG_Y+65,
		E2_X=E1_X,
		E2_Y=E1_Y+22,
		E_WIDTH=160,
		E_HEIGHT=8*5,

		/*
		**	Event optional data entry coordinates and dimensions.
		*/
		ED1_X=E1_X+E_WIDTH+20,
		ED1_Y=E1_Y,
		ED2_X=ED1_X,
		ED2_Y=E2_Y,

		ED_WIDTH=95,
		ED_HEIGHT=8*5,

		/*
		**	Action entry list box coordinates.
		*/
		A1_X=E1_X,
		A1_Y=D_DIALOG_Y+120,
		A2_X=E1_X,
		A2_Y=A1_Y+22,

		/*
		**	Action optional data entry coordinates.
		*/
		AD1_X=A1_X+E_WIDTH+20,
		AD1_Y=A1_Y,
		AD2_X=AD1_X,
		AD2_Y=A2_Y,

		/*
		**	Misc control values.
		*/
		GENERAL_SIZE=10,				// Text length for general data entry fields.
		ENTRY_SIZE=35,					// Maximum size of event or action description text.
		WAYPOINT_SIZE=3,				// Text length maximum for waypoint entry.
		TEAM_SIZE=10,					// Team name text entry field length.
		DESC_SIZE=35					// Maximum length of object full name description.
	};

	/*
	**	Button enumerations:
	*/
	enum {
		EVENT_LIST=100,				// Primary event list.
		EVENT_LIST2,					// Secondary event list.
		ACTION_LIST,					// Primary action list.
		ACTION_LIST2,					// Secondary action list.
		NAME_EDIT,						// Trigger name edit field.
		DATA_SPEECH1,					// Primary action speech.
		DATA_SPEECH2,					// Secondary action speech.
		DATA_THEME1,					// Primary action theme.
		DATA_THEME2,					// Secondary action theme.
		DATA_MOVIE1,					// Primary action movie.
		DATA_MOVIE2,					// Secondary action movie.
		DATA_SOUND1,					// Primary action sound effect.
		DATA_SOUND2,					// Secondary action sound effect.
		DATA_SPECIAL1,					// Primary action special weapon.
		DATA_SPECIAL2,					// Secondary action special weapon.
		DATA_EDIT,						// Primary event waypoint data field.
		DATA_EDIT2,						// Secondary event waypoint data field.
		DATA_EDIT3,						// Primary action waypoint data field.
		DATA_EDIT4,						// Secondary action waypoint data field.
		DATA_HTYPE1,					// Primary event house choice list.
		DATA_HTYPE2,					// Secondary event house choice list.
		DATA_HTYPE3,					// Primary action house choice list.
		DATA_HTYPE4,					// Secondary action house choice list.
		DATA_BOOLTYPE1,				// Primary action boolean data list.
		DATA_BOOLTYPE2,				// Secondary action boolean data list.
		DATA_GENERAL1,					// Primary event general data field.
		DATA_GENERAL2,					// Secondary event general data field.
		DATA_GENERAL3,					// Primary action general data field.
		DATA_GENERAL4,					// Secondary action general data field.
		DATA_BTYPE1,					// Primary event building type list.
		DATA_BTYPE2,					// Secondary event building type list.
		DATA_ITYPE1,					// Primary event infantry type list.
		DATA_ITYPE2,					// Secondary event infantry type list.
		DATA_ATYPE1,					// Primary event aircraft type list.
		DATA_ATYPE2,					// Secondary event aircraft type list.
		DATA_UTYPE1,					// Primary event unit type list.
		DATA_UTYPE2,					// Secondary event unit type list.
		DATA_TTYPE1,					// Primary event team type entry list.
		DATA_TTYPE2,					// Secondary event team type entry list.
		DATA_TTYPE3,					// Primary action team type entry list.
		DATA_TTYPE4,					// Secondary action team type entry list.
		DATA_TRTYPE1,					// Primary action trigger list.
		DATA_TRTYPE2,					// Secondary action trigger list.
		BUTTON_HOUSE,					// House ownership for this trigger.
		BUTTON_PERSISTANCE,			// Persistence of this trigger.
		BUTTON_OK,						// Ok button - save and exit.
		BUTTON_CANCEL,					// Cancel button - just exit.
		BUTTON_ACTION,					// Multiple action control button.
		BUTTON_EVENT,					// Multiple event control button.
	};

	/*
	**	Dialog variables:
	*/
	bool cancel = false;								// true = user cancels
	int i;												// loop counter
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	/*
	**	Buttons
	*/
	ControlClass * commands = NULL;				// the button list

	/*
	**	List of events allowed.
	*/
	char eventtext[ENTRY_SIZE] = "";
	TDropListClass<EventChoiceClass *> event1list(EVENT_LIST, eventtext, sizeof(eventtext),
		TPF_EFNT | TPF_NOSHADOW,
		E1_X, E1_Y, E_WIDTH, E_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char event2text[ENTRY_SIZE] = "";
	TDropListClass<EventChoiceClass *> event2list(EVENT_LIST2, event2text, sizeof(event2text),
		TPF_EFNT | TPF_NOSHADOW,
		E2_X, E2_Y, E_WIDTH, E_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (TEventType event = TEVENT_FIRST; event < TEVENT_COUNT; event++) {
		event1list.Add_Item(&EventChoices[event]);
		event2list.Add_Item(&EventChoices[event]);
	}

	PBubble_Sort(&event1list[0], event1list.Count());
	PBubble_Sort(&event2list[0], event2list.Count());

	if (Event1.Event == TEVENT_NONE) Event1.Event = TEVENT_FIRST;
	event1list.Set_Selected_Index(&EventChoices[Event1.Event]);
	if (Event2.Event == TEVENT_NONE) Event2.Event = TEVENT_FIRST;
	event2list.Set_Selected_Index(&EventChoices[Event2.Event]);

	/*
	**	List of actions allowed.
	*/
	char actiontext[ENTRY_SIZE] = "";
	TDropListClass<ActionChoiceClass *> action1list(ACTION_LIST, actiontext, sizeof(actiontext),
		TPF_EFNT | TPF_NOSHADOW,
		A1_X, A1_Y, E_WIDTH, E_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char action2text[ENTRY_SIZE] = "";
	TDropListClass<ActionChoiceClass *> action2list(ACTION_LIST2, action2text, sizeof(action2text),
		TPF_EFNT | TPF_NOSHADOW,
		A2_X, A2_Y, E_WIDTH, E_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (TActionType action = TACTION_FIRST; action < TACTION_COUNT; action++) {
		action1list.Add_Item(&ActionChoices[action]);
		action2list.Add_Item(&ActionChoices[action]);
	}

	PBubble_Sort(&action1list[0], action1list.Count());
	PBubble_Sort(&action2list[0], action2list.Count());

	if (Action1.Action == ACTION_NONE) Action1.Action = TACTION_FIRST;
	action1list.Set_Selected_Index(&ActionChoices[Action1.Action]);
	if (Action2.Action == ACTION_NONE) Action2.Action = TACTION_FIRST;
	action2list.Set_Selected_Index(&ActionChoices[Action2.Action]);

	/*
	**	Optional waypoint entry field.
	*/
	char way1[WAYPOINT_SIZE] = "A";
	EditClass way1data(DATA_EDIT, way1, sizeof(way1), TPF_EFNT | TPF_NOSHADOW,
		ED1_X, ED1_Y, ED_WIDTH, 9, EditClass::ALPHA);
	if (Event_Needs(Event1.Event) == NEED_WAYPOINT) {
		if (Event1.Data.Value < 26) {
			sprintf(way1data.Get_Text(), "%c", Event1.Data.Value + 'A');
		} else {
			sprintf(way1data.Get_Text(), "%c%c", (Event1.Data.Value / 26) + 'A'-1, (Event1.Data.Value % 26) + 'A');
		}
	}

	char way2[WAYPOINT_SIZE] = "A";
	EditClass way2data(DATA_EDIT2, way2, sizeof(way2), TPF_EFNT | TPF_NOSHADOW,
		ED2_X, ED2_Y, ED_WIDTH, 9, EditClass::ALPHA);
	if (Event_Needs(Event2.Event) == NEED_WAYPOINT) {
		if (Event2.Data.Value < 26) {
			sprintf(way2data.Get_Text(), "%c", Event2.Data.Value + 'A');
		} else {
			sprintf(way2data.Get_Text(), "%c%c", (Event2.Data.Value / 26) + 'A'-1, (Event2.Data.Value % 26) + 'A');
		}
	}

	char way3[WAYPOINT_SIZE] = "A";
	EditClass way3data(DATA_EDIT3, way3, sizeof(way3), TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, 9, EditClass::ALPHA);
	if (Action_Needs(Action1.Action) == NEED_WAYPOINT) {
		if (Action1.Data.Value < 26) {
			sprintf(way3data.Get_Text(), "%c", Action1.Data.Value + 'A');
		} else {
			sprintf(way3data.Get_Text(), "%c%c", (Action1.Data.Value / 26) + 'A'-1, (Action1.Data.Value % 26) + 'A');
		}
	}

	char way4[WAYPOINT_SIZE] = "A";
	EditClass way4data(DATA_EDIT4, way4, sizeof(way4), TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, 9, EditClass::ALPHA);
	if (Action_Needs(Action2.Action) == NEED_WAYPOINT) {
		if (Action2.Data.Value < 26) {
			sprintf(way4data.Get_Text(), "%c", Action2.Data.Value + 'A');
		} else {
			sprintf(way4data.Get_Text(), "%c%c", (Action2.Data.Value / 26) + 'A'-1, (Action2.Data.Value % 26) + 'A');
		}
	}

	/*
	**	Optional event data entry field.
	*/
	char databuf1[GENERAL_SIZE] = "";
	EditClass event1data(DATA_GENERAL1, databuf1, sizeof(databuf1),
		TPF_EFNT | TPF_NOSHADOW,
		ED1_X, ED1_Y, ED_WIDTH, 9, EditClass::NUMERIC);
	switch (Event_Needs(Event1.Event)) {
		case NEED_TIME:
		case NEED_NUMBER:
			sprintf(event1data.Get_Text(), "%d", Event1.Data.Value);
			break;
	}

	char databuf2[GENERAL_SIZE] = "";
	EditClass event2data(DATA_GENERAL2, databuf2, sizeof(databuf2),
		TPF_EFNT | TPF_NOSHADOW,
		ED2_X, ED2_Y, ED_WIDTH, 9, EditClass::NUMERIC);
	switch (Event_Needs(Event2.Event)) {
		case NEED_TIME:
		case NEED_NUMBER:
			sprintf(event2data.Get_Text(), "%d", Event2.Data.Value);
			break;
	}

	char actionbuf1[GENERAL_SIZE] = "";
	EditClass action1data(DATA_GENERAL3, actionbuf1, sizeof(actionbuf1),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, 9, EditClass::NUMERIC);
	switch (Action_Needs(Action1.Action)) {
		case NEED_NUMBER:
			sprintf(action1data.Get_Text(), "%d", Action1.Data.Value);
			break;
	}

	char actionbuf2[GENERAL_SIZE] = "";
	EditClass action2data(DATA_GENERAL4, actionbuf2, sizeof(actionbuf2),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, 9, EditClass::NUMERIC);
	switch (Action_Needs(Action2.Action)) {
		case NEED_NUMBER:
			sprintf(action2data.Get_Text(), "%d", Action2.Data.Value);
			break;
	}

	/*
	**	Optional team entry list.
	*/
	char tbuf1[TEAM_SIZE] = "";
	DropListClass ttype1list(DATA_TTYPE1, tbuf1, sizeof(tbuf1),
		TPF_EFNT | TPF_NOSHADOW,
		ED1_X, ED1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char tbuf2[TEAM_SIZE] = "";
	DropListClass ttype2list(DATA_TTYPE2, tbuf2, sizeof(tbuf2),
		TPF_EFNT | TPF_NOSHADOW,
		ED2_X, ED2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char tbuf3[TEAM_SIZE] = "";
	DropListClass ttype3list(DATA_TTYPE3, tbuf3, sizeof(tbuf3),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char tbuf4[TEAM_SIZE] = "";
	DropListClass ttype4list(DATA_TTYPE4, tbuf4, sizeof(tbuf4),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (int index = 0; index < TeamTypes.Count(); index++) {
		ttype1list.Add_Item(TeamTypes.Ptr(index)->IniName);
		ttype2list.Add_Item(TeamTypes.Ptr(index)->IniName);
		ttype3list.Add_Item(TeamTypes.Ptr(index)->IniName);
		ttype4list.Add_Item(TeamTypes.Ptr(index)->IniName);
	}

	if (Event1.Team.Is_Valid()) {
		ttype1list.Set_Selected_Index(Event1.Team->IniName);
	} else {
		ttype1list.Set_Selected_Index(0);
	}
	if (Event2.Team.Is_Valid()) {
		ttype2list.Set_Selected_Index(Event2.Team->IniName);
	} else {
		ttype2list.Set_Selected_Index(0);
	}
	if (Action1.Team.Is_Valid()) {
		ttype3list.Set_Selected_Index(Action1.Team->IniName);
	} else {
		ttype3list.Set_Selected_Index(0);
	}
	if (Action2.Team.Is_Valid()) {
		ttype4list.Set_Selected_Index(Action2.Team->IniName);
	} else {
		ttype4list.Set_Selected_Index(0);
	}

	/*
	**	Optional trigger entry list.
	*/
	char trbuf1[TEAM_SIZE] = "";
	DropListClass trtype1list(DATA_TRTYPE1, trbuf1, sizeof(trbuf1),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char trbuf2[TEAM_SIZE] = "";
	DropListClass trtype2list(DATA_TRTYPE2, trbuf2, sizeof(trbuf2),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (index = 0; index < TriggerTypes.Count(); index++) {
		trtype1list.Add_Item(TriggerTypes.Ptr(index)->IniName);
		trtype2list.Add_Item(TriggerTypes.Ptr(index)->IniName);
	}

	if (Action1.Trigger.Is_Valid()) {
		trtype1list.Set_Selected_Index(Action1.Trigger->IniName);
	} else {
		trtype1list.Set_Selected_Index(0);
	}
	if (Action2.Trigger.Is_Valid()) {
		trtype2list.Set_Selected_Index(Action2.Trigger->IniName);
	} else {
		trtype2list.Set_Selected_Index(0);
	}

	/*
	**	Optional boolean value list.
	*/
	char boolbuf1[TEAM_SIZE] = "";
	DropListClass booltype1list(DATA_BOOLTYPE1, boolbuf1, sizeof(boolbuf1),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char boolbuf2[TEAM_SIZE] = "";
	DropListClass booltype2list(DATA_BOOLTYPE2, boolbuf2, sizeof(boolbuf2),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	booltype1list.Add_Item("OFF");
	booltype1list.Add_Item("ON");
	booltype2list.Add_Item("OFF");
	booltype2list.Add_Item("ON");

	booltype1list.Set_Selected_Index(Action1.Data.Bool);
	booltype2list.Set_Selected_Index(Action2.Data.Bool);

	/*
	**	Optional musical theme choice list.
	*/
	char themebuf1[DESC_SIZE] = "";
	DropListClass themetype1list(DATA_THEME1, themebuf1, sizeof(themebuf1),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char themebuf2[DESC_SIZE] = "";
	DropListClass themetype2list(DATA_THEME2, themebuf2, sizeof(themebuf2),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; theme++) {
		themetype1list.Add_Item(Theme.Full_Name(theme));
		themetype2list.Add_Item(Theme.Full_Name(theme));
	}

	if (Action_Needs(Action1.Action) == NEED_THEME) {
		themetype1list.Set_Selected_Index(Action1.Data.Theme);
	} else {
		themetype1list.Set_Selected_Index(0);
	}
	if (Action_Needs(Action2.Action) == NEED_THEME) {
		themetype2list.Set_Selected_Index(Action2.Data.Theme);
	} else {
		themetype2list.Set_Selected_Index(0);
	}

	/*
	**	Optional movie list.
	*/
	char moviebuf1[DESC_SIZE] = "";
	DropListClass movietype1list(DATA_MOVIE1, moviebuf1, sizeof(moviebuf1),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char moviebuf2[DESC_SIZE] = "";
	DropListClass movietype2list(DATA_MOVIE2, moviebuf2, sizeof(moviebuf2),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (VQType movie = VQ_FIRST; movie < VQ_COUNT; movie++) {
		movietype1list.Add_Item(VQName[movie]);
		movietype2list.Add_Item(VQName[movie]);
	}

	if (Action_Needs(Action1.Action) == NEED_MOVIE) {
		movietype1list.Set_Selected_Index(Action1.Data.Movie);
	} else {
		movietype1list.Set_Selected_Index(0);
	}
	if (Action_Needs(Action2.Action) == NEED_MOVIE) {
		movietype2list.Set_Selected_Index(Action2.Data.Movie);
	} else {
		movietype2list.Set_Selected_Index(0);
	}

	/*
	**	Optional sound effect list.
	*/
	char soundbuf1[DESC_SIZE] = "";
	DropListClass soundtype1list(DATA_SOUND1, soundbuf1, sizeof(soundbuf1),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char soundbuf2[DESC_SIZE] = "";
	DropListClass soundtype2list(DATA_SOUND2, soundbuf2, sizeof(soundbuf2),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (VocType sound = VOC_FIRST; sound < VOC_COUNT; sound++) {
		soundtype1list.Add_Item(Voc_Name(sound));
		soundtype2list.Add_Item(Voc_Name(sound));
	}

	if (Action_Needs(Action1.Action) == NEED_SOUND) {
		soundtype1list.Set_Selected_Index(Action1.Data.Sound);
	} else {
		soundtype1list.Set_Selected_Index(0);
	}
	if (Action_Needs(Action2.Action) == NEED_SOUND) {
		soundtype2list.Set_Selected_Index(Action2.Data.Sound);
	} else {
		soundtype2list.Set_Selected_Index(0);
	}

	/*
	**	Optional speech effect list.
	*/
	char speechbuf1[DESC_SIZE] = "";
	DropListClass speechtype1list(DATA_SPEECH1, speechbuf1, sizeof(speechbuf1),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char speechbuf2[DESC_SIZE] = "";
	DropListClass speechtype2list(DATA_SPEECH2, speechbuf2, sizeof(speechbuf2),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (VoxType speech = VOX_FIRST; speech < VOX_COUNT; speech++) {
		speechtype1list.Add_Item(Speech_Name(speech));
		speechtype2list.Add_Item(Speech_Name(speech));
	}

	if (Action_Needs(Action1.Action) == NEED_SPEECH) {
		speechtype1list.Set_Selected_Index(Action1.Data.Speech);
	} else {
		speechtype1list.Set_Selected_Index(0);
	}
	if (Action_Needs(Action2.Action) == NEED_SPEECH) {
		speechtype2list.Set_Selected_Index(Action2.Data.Speech);
	} else {
		speechtype2list.Set_Selected_Index(0);
	}

	/*
	**	Optional building type entry list.
	*/
	char bbuf1[DESC_SIZE] = "";
	DropListClass btype1list(DATA_BTYPE1, bbuf1, sizeof(bbuf1),
		TPF_EFNT | TPF_NOSHADOW,
		ED1_X, ED1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char bbuf2[DESC_SIZE] = "";
	DropListClass btype2list(DATA_BTYPE2, bbuf2, sizeof(bbuf2),
		TPF_EFNT | TPF_NOSHADOW,
		ED2_X, ED2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (StructType ss = STRUCT_FIRST; ss < STRUCT_COUNT; ss++) {
		btype1list.Add_Item(Text_String(BuildingTypeClass::As_Reference(ss).Full_Name()));
		btype2list.Add_Item(Text_String(BuildingTypeClass::As_Reference(ss).Full_Name()));
	}

	if (Event_Needs(Event1.Event) == NEED_STRUCTURE) {
		btype1list.Set_Selected_Index(Event1.Data.Structure);
	} else {
		btype1list.Set_Selected_Index(0);
	}
	if (Event_Needs(Event2.Event) == NEED_STRUCTURE) {
		btype2list.Set_Selected_Index(Event2.Data.Structure);
	} else {
		btype2list.Set_Selected_Index(0);
	}

	/*
	**	Optional infantry type entry list.
	*/
	char ibuf1[DESC_SIZE] = "";
	DropListClass itype1list(DATA_ITYPE1, ibuf1, sizeof(ibuf1),
		TPF_EFNT | TPF_NOSHADOW,
		ED1_X, ED1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char ibuf2[DESC_SIZE] = "";
	DropListClass itype2list(DATA_ITYPE2, ibuf2, sizeof(ibuf2),
		TPF_EFNT | TPF_NOSHADOW,
		ED2_X, ED2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (InfantryType ii = INFANTRY_FIRST; ii < INFANTRY_COUNT; ii++) {
		itype1list.Add_Item(Text_String(InfantryTypeClass::As_Reference(ii).Full_Name()));
		itype2list.Add_Item(Text_String(InfantryTypeClass::As_Reference(ii).Full_Name()));
	}

	if (Event_Needs(Event1.Event) == NEED_INFANTRY) {
		itype1list.Set_Selected_Index(Event1.Data.Infantry);
	} else {
		itype1list.Set_Selected_Index(0);
	}
	if (Event_Needs(Event2.Event) == NEED_INFANTRY) {
		itype2list.Set_Selected_Index(Event2.Data.Infantry);
	} else {
		itype2list.Set_Selected_Index(0);
	}

	/*
	**	Optional aircraft type entry list.
	*/
	char abuf1[DESC_SIZE] = "";
	DropListClass atype1list(DATA_ATYPE1, abuf1, sizeof(abuf1),
		TPF_EFNT | TPF_NOSHADOW,
		ED1_X, ED1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char abuf2[DESC_SIZE] = "";
	DropListClass atype2list(DATA_ATYPE2, abuf2, sizeof(abuf2),
		TPF_EFNT | TPF_NOSHADOW,
		ED2_X, ED2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (AircraftType aa = AIRCRAFT_FIRST; aa < AIRCRAFT_COUNT; aa++) {
		atype1list.Add_Item(Text_String(AircraftTypeClass::As_Reference(aa).Full_Name()));
		atype2list.Add_Item(Text_String(AircraftTypeClass::As_Reference(aa).Full_Name()));
	}

	if (Event_Needs(Event1.Event) == NEED_AIRCRAFT) {
		atype1list.Set_Selected_Index(Event1.Data.Aircraft);
	} else {
		atype1list.Set_Selected_Index(0);
	}
	if (Event_Needs(Event2.Event) == NEED_AIRCRAFT) {
		atype2list.Set_Selected_Index(Event2.Data.Aircraft);
	} else {
		atype2list.Set_Selected_Index(0);
	}

	/*
	**	Optional unit type entry list.
	*/
	char ubuf1[DESC_SIZE] = "";
	DropListClass utype1list(DATA_UTYPE1, ubuf1, sizeof(ubuf1),
		TPF_EFNT | TPF_NOSHADOW,
		ED1_X, ED1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char ubuf2[DESC_SIZE] = "";
	DropListClass utype2list(DATA_UTYPE2, ubuf2, sizeof(ubuf2),
		TPF_EFNT | TPF_NOSHADOW,
		ED2_X, ED2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (UnitType uu = UNIT_FIRST; uu < UNIT_COUNT; uu++) {
		utype1list.Add_Item(Text_String(UnitTypeClass::As_Reference(uu).Full_Name()));
		utype2list.Add_Item(Text_String(UnitTypeClass::As_Reference(uu).Full_Name()));
	}

	if (Event_Needs(Event1.Event) == NEED_UNIT) {
		utype1list.Set_Selected_Index(Event1.Data.Unit);
	} else {
		utype1list.Set_Selected_Index(0);
	}
	if (Event_Needs(Event2.Event) == NEED_UNIT) {
		utype2list.Set_Selected_Index(Event2.Data.Unit);
	} else {
		utype2list.Set_Selected_Index(0);
	}

	/*
	**	Optional house type entry list.
	*/
	char housebuf1[DESC_SIZE] = "";
	DropListClass htype1list(DATA_HTYPE1, housebuf1, sizeof(housebuf1),
		TPF_EFNT | TPF_NOSHADOW,
		ED1_X, ED1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char housebuf2[DESC_SIZE] = "";
	DropListClass htype2list(DATA_HTYPE2, housebuf2, sizeof(housebuf2),
		TPF_EFNT | TPF_NOSHADOW,
		ED2_X, ED2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char housebuf3[DESC_SIZE] = "";
	DropListClass htype3list(DATA_HTYPE3, housebuf3, sizeof(housebuf3),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char housebuf4[DESC_SIZE] = "";
	DropListClass htype4list(DATA_HTYPE4, housebuf4, sizeof(housebuf4),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	for (HousesType hh = HOUSE_FIRST; hh < HOUSE_COUNT; hh++) {
		htype1list.Add_Item(HouseTypeClass::As_Reference(hh).IniName);
		htype2list.Add_Item(HouseTypeClass::As_Reference(hh).IniName);
		htype3list.Add_Item(HouseTypeClass::As_Reference(hh).IniName);
		htype4list.Add_Item(HouseTypeClass::As_Reference(hh).IniName);
	}

	if (Event_Needs(Event1.Event) == NEED_HOUSE) {
		htype1list.Set_Selected_Index(Event1.Data.House);
	} else {
		htype1list.Set_Selected_Index(0);
	}
	if (Event_Needs(Event2.Event) == NEED_HOUSE) {
		htype2list.Set_Selected_Index(Event2.Data.House);
	} else {
		htype2list.Set_Selected_Index(0);
	}
	if (Action_Needs(Action1.Action) == NEED_HOUSE) {
		htype3list.Set_Selected_Index(Action1.Data.House);
	} else {
		htype3list.Set_Selected_Index(0);
	}
	if (Action_Needs(Action2.Action) == NEED_HOUSE) {
		htype4list.Set_Selected_Index(Action2.Data.House);
	} else {
		htype4list.Set_Selected_Index(0);
	}

	/*
	**	Optional special weapon list.
	*/
	char special1[DESC_SIZE] = "";
	DropListClass spc1(DATA_SPECIAL1, special1, sizeof(special1),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char special2[DESC_SIZE] = "";
	DropListClass spc2(DATA_SPECIAL2, special2, sizeof(special2),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (SpecialWeaponType spec = SPC_FIRST; spec < SPC_COUNT; spec++) {
		spc1.Add_Item(SpecialWeaponName[spec]);
		spc2.Add_Item(SpecialWeaponName[spec]);
	}
	if ((unsigned)Action1.Data.Special < SPC_COUNT) {
		spc1.Set_Selected_Index(Action1.Data.Special);
	} else {
		spc1.Set_Selected_Index(0);
	}
	if ((unsigned)Action2.Data.Special < SPC_COUNT) {
		spc2.Set_Selected_Index(Action2.Data.Special);
	} else {
		spc2.Set_Selected_Index(0);
	}

	/*
	**	Optional quarry type.
	*/
	char quarry1[DESC_SIZE] = "";
	DropListClass qlist1(DATA_SPECIAL1, quarry1, sizeof(quarry1),
		TPF_EFNT | TPF_NOSHADOW,
		AD1_X, AD1_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	char quarry2[DESC_SIZE] = "";
	DropListClass qlist2(DATA_SPECIAL2, quarry2, sizeof(quarry2),
		TPF_EFNT | TPF_NOSHADOW,
		AD2_X, AD2_Y, ED_WIDTH, ED_HEIGHT,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (QuarryType q = QUARRY_FIRST; q < QUARRY_COUNT; q++) {
		qlist1.Add_Item(QuarryName[q]);
		qlist2.Add_Item(QuarryName[q]);
	}
	if ((unsigned)Action1.Data.Quarry < QUARRY_COUNT) {
		qlist1.Set_Selected_Index(Action1.Data.Quarry);
	} else {
		qlist1.Set_Selected_Index(0);
	}
	if ((unsigned)Action2.Data.Quarry < QUARRY_COUNT) {
		qlist2.Set_Selected_Index(Action2.Data.Quarry);
	} else {
		qlist2.Set_Selected_Index(0);
	}

	/*
	**	Name of this trigger text edit field.
	*/
	char namebuf[5] = "";
	EditClass name_edt(NAME_EDIT, namebuf, sizeof(namebuf), TPF_EFNT | TPF_NOSHADOW, D_DIALOG_X+40, D_DIALOG_Y+30, 40, 9, EditClass::ALPHANUMERIC);
	strcpy(namebuf, IniName);				// Name

	/*
	**	Create the list of house's allowed for trigger.
	*/
	char housetext[DESC_SIZE] = "";
	DropListClass housebtn(BUTTON_HOUSE, housetext, sizeof(housetext),
		TPF_EFNT | TPF_NOSHADOW,
		name_edt.X+name_edt.Width+20, name_edt.Y, 95, 8*5,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		housebtn.Add_Item(HouseTypeClass::As_Reference(house).IniName);
	}
	if (House == HOUSE_NONE) House = HOUSE_GOOD;
	housebtn.Set_Selected_Index(House);

	/*
	** Must match order and number of PersistantType specified in
	**	TriggerTypeClass definition.
	*/
	char perstext[DESC_SIZE] = "";
	static char * _perstext[3] = {
		"Volatile",
		"Semi-persistent",
		"Persistent"
	};
	DropListClass persbtn(BUTTON_PERSISTANCE, perstext, sizeof(perstext),
		TPF_EFNT | TPF_NOSHADOW,
		housebtn.X+housebtn.Width+20, housebtn.Y, 105, 8*5,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (i = 0; i < sizeof(_perstext)/sizeof(_perstext[0]); i++) {
		persbtn.Add_Item(_perstext[i]);
	}
	persbtn.Set_Selected_Index(IsPersistant);

	/*
	**	This button controls the existence and relationship of a second trigger
	**	event.
	*/
	int eventflag = EventControl;
	TextButtonClass eventbtn(BUTTON_EVENT, TXT_TRIGGER_JUST_EVENT, TPF_EBUTTON, event1list.X, event1list.Y+11, 100, 9);

	/*
	**	This button controls the existence of a secondary action.
	*/
	bool actionflag = ActionControl;
	TextButtonClass actionbtn(BUTTON_ACTION, TXT_TRIGGER_JUST_ACTION, TPF_EBUTTON, action1list.X, action1list.Y+11, 100, 9);

	/*
	**	Create the ubiquitous OK and Cancel buttons.
	*/
	TextButtonClass okbtn(BUTTON_OK, TXT_OK, TPF_EBUTTON, D_DIALOG_X+35, D_DIALOG_Y+D_DIALOG_H-30, 45, 9);
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_EBUTTON, D_DIALOG_X+D_DIALOG_W-80, D_DIALOG_Y+D_DIALOG_H-30, 45, 9);

	/*
	**	Initialize
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Build the button list
	*/
	commands = &okbtn;
	cancelbtn.Add_Tail(*commands);
	event1list.Add_Tail(*commands);
	action1list.Add_Tail(*commands);
	eventbtn.Add_Tail(*commands);
	actionbtn.Add_Tail(*commands);
	name_edt.Add_Tail(*commands);
	persbtn.Add_Tail(*commands);
	housebtn.Add_Tail(*commands);

	/*
	**	Main Processing Loop
	*/
	bool display = true;
	bool process = true;
	while (process) {

		/*
		**	Invoke game callback
		*/
		Call_Back();

		/*
		**	Refresh display if needed
		*/
		if (display /*&& LogicPage->Lock()*/) {

			/*
			**	Display the dialog box
			*/
			Hide_Mouse();
			Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
			Draw_Caption(TXT_TRIGGER_EDITOR, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

			/*
			**	Draw the captions
			*/
			Fancy_Text_Print("Trigger Event:", event1list.X, event1list.Y - 7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Action to Perform:", action1list.X, action1list.Y - 7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("House:", housebtn.X, housebtn.Y - 7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Name:", name_edt.X, name_edt.Y - 7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Persistence:", persbtn.X, persbtn.Y - 7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);

			if (eventflag == 3) {
				LogicPage->Draw_Line(event1list.X-1, event1list.Y+3, event1list.X-2*RESFACTOR, event1list.Y+3, WHITE);
				LogicPage->Draw_Line(event1list.X-2*RESFACTOR, event1list.Y+3, action1list.X-2*RESFACTOR, action1list.Y+3, WHITE);
				LogicPage->Draw_Line(action1list.X-1, action1list.Y+3, action1list.X-2*RESFACTOR, action1list.Y+3, WHITE);

				LogicPage->Draw_Line(event2list.X-1, event2list.Y+3, event2list.X-5*RESFACTOR, event2list.Y+3, WHITE);
				LogicPage->Draw_Line(event2list.X-5*RESFACTOR, event2list.Y+3, action2list.X-5*RESFACTOR, action2list.Y+3, WHITE);
				LogicPage->Draw_Line(action2list.X-1, action2list.Y+3, action2list.X-5*RESFACTOR, action2list.Y+3, WHITE);
			}

			/*
			**	Adjust the button list to match current control settings.
			*/
			event2list.Remove();
			switch (eventflag) {
				case 0:
					eventbtn.Set_Text(TXT_TRIGGER_JUST_EVENT);
					break;

				case 1:
					eventbtn.Set_Text(TXT_TRIGGER_AND);
					event2list.Add(*commands);
					break;

				case 2:
					eventbtn.Set_Text(TXT_TRIGGER_OR);
					event2list.Add(*commands);
					break;

				case 3:
					eventbtn.Set_Text(TXT_TRIGGER_LINKED);
					event2list.Add(*commands);
					break;
			}

			/*
			**	Prepare the primary event data field.
			*/
			htype1list.Remove();
			way1data.Remove();
			event1data.Remove();
			btype1list.Remove();
			itype1list.Remove();
			atype1list.Remove();
			utype1list.Remove();
			ttype1list.Remove();
			switch (Event_Needs(*event1list.Current_Item())) {
				case NEED_HOUSE:
					htype1list.Add(*commands);
					break;

				case NEED_TEAM:
					ttype1list.Add(*commands);
					break;

				case NEED_WAYPOINT:
					way1data.Add(*commands);
					break;

				case NEED_TIME:
				case NEED_NUMBER:
					event1data.Add(*commands);
					break;

				case NEED_STRUCTURE:
					btype1list.Add(*commands);
					break;

				case NEED_INFANTRY:
					itype1list.Add(*commands);
					break;

				case NEED_AIRCRAFT:
					atype1list.Add(*commands);
					break;

				case NEED_UNIT:
					utype1list.Add(*commands);
					break;

				default:
					break;
			}

			/*
			**	Prepare the secondary event data field.
			*/
			htype2list.Remove();
			way2data.Remove();
			event2data.Remove();
			btype2list.Remove();
			itype2list.Remove();
			atype2list.Remove();
			utype2list.Remove();
			ttype2list.Remove();
			if (commands->Extract_Gadget(EVENT_LIST2)) {
				switch (Event_Needs(*event2list.Current_Item())) {
					case NEED_HOUSE:
						htype2list.Add(*commands);
						break;

					case NEED_TEAM:
						ttype2list.Add(*commands);
						break;

					case NEED_WAYPOINT:
						way2data.Add(*commands);
						break;

					case NEED_TIME:
					case NEED_NUMBER:
						event2data.Add(*commands);
						break;

					case NEED_STRUCTURE:
						btype2list.Add(*commands);
						break;

					case NEED_INFANTRY:
						itype2list.Add(*commands);
						break;

					case NEED_AIRCRAFT:
						atype2list.Add(*commands);
						break;

					case NEED_UNIT:
						utype2list.Add(*commands);
						break;

					default:
						break;
				}
			}

			/*
			**	Setup the action buttons and associated data entry fields.
			*/
			actionbtn.Remove();
			action2list.Remove();
			if (eventflag == 3) {
				action2list.Add(*commands);
			} else {
				actionbtn.Add(*commands);
				if (actionflag) {
					actionbtn.Set_Text(TXT_TRIGGER_AND);
					action2list.Add(*commands);
				} else {
					actionbtn.Set_Text(TXT_TRIGGER_JUST_ACTION);
				}
			}

			qlist1.Remove();
			spc1.Remove();
			htype3list.Remove();
			booltype1list.Remove();
			trtype1list.Remove();
			way3data.Remove();
			action1data.Remove();
			ttype3list.Remove();
			themetype1list.Remove();
			soundtype1list.Remove();
			movietype1list.Remove();
			speechtype1list.Remove();
			switch (Action_Needs(*action1list.Current_Item())) {
				case NEED_MOVIE:
					movietype1list.Add(*commands);
					break;

				case NEED_SPECIAL:
					spc1.Add(*commands);
					break;

				case NEED_HOUSE:
					htype3list.Add(*commands);
					break;

				case NEED_BOOL:
					booltype1list.Add(*commands);
					break;

				case NEED_TRIGGER:
					trtype1list.Add(*commands);
					break;

				case NEED_TEAM:
					ttype3list.Add(*commands);
					break;

				case NEED_NUMBER:
					action1data.Add(*commands);
					break;

				case NEED_WAYPOINT:
					way3data.Add(*commands);
					break;

				case NEED_THEME:
					themetype1list.Add(*commands);
					break;

				case NEED_SOUND:
					soundtype1list.Add(*commands);
					break;

				case NEED_SPEECH:
					speechtype1list.Add(*commands);
					break;

				case NEED_QUARRY:
					qlist1.Add(*commands);
					break;
			}

			qlist2.Remove();
			spc2.Remove();
			htype4list.Remove();
			booltype2list.Remove();
			trtype2list.Remove();
			way4data.Remove();
			action2data.Remove();
			ttype4list.Remove();
			themetype2list.Remove();
			soundtype2list.Remove();
			movietype2list.Remove();
			speechtype2list.Remove();
			if (commands->Extract_Gadget(ACTION_LIST2)) {
				switch (Action_Needs(*action2list.Current_Item())) {
					case NEED_MOVIE:
						movietype2list.Add(*commands);
						break;

					case NEED_SPECIAL:
						spc2.Add(*commands);
						break;

					case NEED_HOUSE:
						htype4list.Add(*commands);
						break;

					case NEED_BOOL:
						booltype2list.Add(*commands);
						break;

					case NEED_TRIGGER:
						trtype2list.Add(*commands);
						break;

					case NEED_TEAM:
						ttype4list.Add(*commands);
						break;

					case NEED_NUMBER:
						action2data.Add(*commands);
						break;

					case NEED_WAYPOINT:
						way4data.Add(*commands);
						break;

					case NEED_THEME:
						themetype2list.Add(*commands);
						break;

					case NEED_SOUND:
						soundtype2list.Add(*commands);
						break;

					case NEED_SPEECH:
						speechtype2list.Add(*commands);
						break;

					case NEED_QUARRY:
						qlist2.Add(*commands);
						break;
				}
			}

			/*
			**	Collapse any dropped down list boxes.
			*/
			spc1.Collapse();
			spc2.Collapse();
			qlist1.Collapse();
			qlist2.Collapse();
			htype1list.Collapse();
			htype2list.Collapse();
			htype3list.Collapse();
			htype4list.Collapse();
			ttype1list.Collapse();
			ttype2list.Collapse();
			ttype3list.Collapse();
			ttype4list.Collapse();
			btype1list.Collapse();
			btype2list.Collapse();
			utype1list.Collapse();
			utype2list.Collapse();
			itype1list.Collapse();
			itype2list.Collapse();
			atype1list.Collapse();
			atype2list.Collapse();
			trtype1list.Collapse();
			trtype2list.Collapse();
			action1list.Collapse();
			action2list.Collapse();
			event1list.Collapse();
			event2list.Collapse();
			housebtn.Collapse();
			persbtn.Collapse();
			booltype1list.Collapse();
			booltype2list.Collapse();
			themetype1list.Collapse();
			themetype2list.Collapse();
			soundtype1list.Collapse();
			soundtype2list.Collapse();
			movietype1list.Collapse();
			movietype2list.Collapse();
			speechtype1list.Collapse();
			speechtype2list.Collapse();
			commands->Flag_List_To_Redraw();
			Show_Mouse();
			display = false;
//			LogicPage->Unlock();
		}

		/*
		**	Get user input
		*/
		KeyNumType input = commands->Input();

		/*
		**	Process input
		*/
		switch (input) {
			case BUTTON_EVENT | KN_BUTTON:
				eventflag = (eventflag+1) % 4;
				display = true;
				break;

			case BUTTON_ACTION | KN_BUTTON:
				actionflag = (actionflag == false);
				display = true;
				break;

			case DATA_SPEECH1 | KN_BUTTON:
				Speak(VoxType(speechtype1list.Current_Index()));
				display = true;
				break;

			case DATA_SPEECH2 | KN_BUTTON:
				Speak(VoxType(speechtype2list.Current_Index()));
				display = true;
				break;

			case DATA_SOUND1 | KN_BUTTON:
				Sound_Effect(VocType(soundtype1list.Current_Index()));
				display = true;
				break;

			case DATA_SOUND2 | KN_BUTTON:
				Sound_Effect(VocType(soundtype2list.Current_Index()));
				display = true;
				break;

			/*
			**	Transfer all the necessary values from the edit fields into their
			**	respective positions within the trigger object.
			*/
			case KN_RETURN:
			case BUTTON_OK | KN_BUTTON:
				House = HousesType(housebtn.Current_Index());
				IsPersistant = PersistantType(persbtn.Current_Index());
				if (strlen(namebuf)==0) {
					Set_Name("____");
				} else {
					Set_Name(namebuf);
				}

				/*
				**	Primary event specific data retrieval.
				*/
				EventControl = MultiStyleType(eventflag);
				Event1.Event = *event1list.Current_Item();
				switch (Event_Needs(Event1.Event)) {
					case NEED_HOUSE:
						Event1.Data.House = HousesType(htype1list.Current_Index());
						break;

					case NEED_TIME:
						Event1.Data.Value = atoi(event1data.Get_Text());
						break;

					case NEED_NUMBER:
						Event1.Data.Value = atoi(event1data.Get_Text());
						break;

					case NEED_STRUCTURE:
						Event1.Data.Structure = StructType(btype1list.Current_Index());
						break;

					case NEED_UNIT:
						Event1.Data.Unit = UnitType(utype1list.Current_Index());
						break;

					case NEED_INFANTRY:
						Event1.Data.Infantry = InfantryType(itype1list.Current_Index());
						break;

					case NEED_AIRCRAFT:
						Event1.Data.Aircraft = AircraftType(atype1list.Current_Index());
						break;

					case NEED_WAYPOINT:
						Event1.Data.Value = toupper(way1[0]) - 'A';
						if (way1[1] != '\0') {
							Event1.Data.Value = (Event1.Data.Value+1)*26;
							Event1.Data.Value += toupper(way1[1]) - 'A';
						}
						break;

					case NEED_TEAM:
						Event1.Team = TeamTypeClass::From_Name(ttype1list.Current_Item());
						break;
				}

				/*
				**	Secondary event specific data retrieval.
				*/
				Event2.Event = *event2list.Current_Item();
				switch (Event_Needs(Event2.Event)) {
					case NEED_HOUSE:
						Event2.Data.House = HousesType(htype2list.Current_Index());
						break;

					case NEED_TIME:
						Event2.Data.Value = atoi(event2data.Get_Text());
						break;

					case NEED_NUMBER:
						Event2.Data.Value = atoi(event2data.Get_Text());
						break;

					case NEED_STRUCTURE:
						Event2.Data.Structure = StructType(btype2list.Current_Index());
						break;

					case NEED_UNIT:
						Event2.Data.Unit = UnitType(utype2list.Current_Index());
						break;

					case NEED_INFANTRY:
						Event2.Data.Infantry = InfantryType(itype2list.Current_Index());
						break;

					case NEED_AIRCRAFT:
						Event2.Data.Aircraft = AircraftType(atype2list.Current_Index());
						break;

					case NEED_WAYPOINT:
						Event2.Data.Value = toupper(way2[0]) - 'A';
						if (way2[1] != '\0') {
							Event2.Data.Value = (Event2.Data.Value+1)*26;
							Event2.Data.Value += toupper(way2[1]) - 'A';
						}
						break;

					case NEED_TEAM:
						Event2.Team = TeamTypeClass::As_Pointer(ttype2list.Current_Item());
						break;
				}

				/*
				**	Primary action data retrieval.
				*/
				ActionControl = MultiStyleType(actionflag);
				Action1.Action = *action1list.Current_Item();
				switch (Action_Needs(Action1.Action)) {
					case NEED_SPECIAL:
						Action1.Data.Special = SpecialWeaponType(spc1.Current_Index());
						break;

					case NEED_HOUSE:
						Action1.Data.House = HousesType(htype3list.Current_Index());
						break;

					case NEED_TRIGGER:
						Action1.Trigger = TriggerTypeClass::From_Name(trtype1list.Current_Item());
						break;

					case NEED_TEAM:
						Action1.Team = TeamTypeClass::From_Name(ttype3list.Current_Item());
						break;

					case NEED_NUMBER:
						Action1.Data.Value = atoi(action1data.Get_Text());
						break;

					case NEED_WAYPOINT:
						Action1.Data.Value = toupper(way3[0]) - 'A';
						if (way3[1] != '\0') {
							Action1.Data.Value = (Action1.Data.Value+1)*26;
							Action1.Data.Value += toupper(way3[1]) - 'A';
						}
						break;

					case NEED_BOOL:
						Action1.Data.Bool = booltype1list.Current_Index();
						break;

					case NEED_THEME:
						Action1.Data.Theme = ThemeType(themetype1list.Current_Index());
						break;

					case NEED_SOUND:
						Action1.Data.Sound = VocType(soundtype1list.Current_Index());
						break;

					case NEED_MOVIE:
						Action1.Data.Movie = VQType(movietype1list.Current_Index());
						break;

					case NEED_SPEECH:
						Action1.Data.Speech = VoxType(speechtype1list.Current_Index());
						break;

					case NEED_QUARRY:
						Action1.Data.Quarry = QuarryType(qlist1.Current_Index());
						break;
				}

				/*
				**	Secondary action data retrieval.
				*/
				Action2.Action = *action2list.Current_Item();
				switch (Action_Needs(Action2.Action)) {
					case NEED_SPECIAL:
						Action2.Data.Special = SpecialWeaponType(spc2.Current_Index());
						break;

					case NEED_HOUSE:
						Action2.Data.House = HousesType(htype4list.Current_Index());
						break;

					case NEED_TRIGGER:
						Action2.Trigger = TriggerTypeClass::From_Name(trtype2list.Current_Item());
						break;

					case NEED_TEAM:
						Action2.Team = TeamTypeClass::From_Name(ttype4list.Current_Item());
						break;

					case NEED_NUMBER:
						Action2.Data.Value = atoi(action2data.Get_Text());
						break;

					case NEED_WAYPOINT:
						Action2.Data.Value = toupper(way4[0]) - 'A';
						if (way4[1] != '\0') {
							Action2.Data.Value = (Action2.Data.Value+1)*26;
							Action2.Data.Value += toupper(way4[1]) - 'A';
						}
						break;

					case NEED_BOOL:
						Action2.Data.Bool = booltype2list.Current_Index();
						break;

					case NEED_THEME:
						Action2.Data.Theme = ThemeType(themetype2list.Current_Index());
						break;

					case NEED_MOVIE:
						Action2.Data.Movie = VQType(movietype2list.Current_Index());
						break;

					case NEED_SOUND:
						Action2.Data.Sound = VocType(soundtype2list.Current_Index());
						break;

					case NEED_SPEECH:
						Action2.Data.Speech = VoxType(speechtype2list.Current_Index());
						break;

					case NEED_QUARRY:
						Action2.Data.Quarry = QuarryType(qlist1.Current_Index());
						break;
				}
				return(true);

			case KN_ESC:
			case BUTTON_CANCEL | KN_BUTTON:
				process = false;

			/*
			**	Always signal a redraw if any of the buttons were touched. This
			**	can be determined by examining the button bit flag in the input
			**	return value.
			*/
			default:
				if (input & KN_BUTTON) {
					display = true;
				}
				break;
		}
	}
	return(false);
}
#endif


#if defined(CHEAT_KEYS) || defined(SCENARIO_EDITOR)
/***********************************************************************************************
 * TriggerTypeClass::Description -- Build a text description of the trigger type.              *
 *                                                                                             *
 *    This will build a (static) text description of the trigger type. Use this description    *
 *    when displaying this trigger in a list box.                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a one line text description of this trigger.             *
 *                                                                                             *
 * WARNINGS:   The pointer returned actually points to a static buffer. The pointer is only    *
 *             valid until this routine is called again.                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * TriggerTypeClass::Description(void) const
{
	static char _buffer[128];

	char special;
	switch (EventControl) {
		case MULTI_AND:
			special = '&';
			break;

		case MULTI_OR:
			special = '|';
			break;

		case MULTI_LINKED:
			special = '=';
			break;

		default:
			special = '.';
			break;
	}

	char special2 = '.';
	if (ActionControl == MULTI_AND) {
		special2 = '&';
	}

	char tbuf[32];
	char const * added = "";
	switch (Event_Needs(Event1.Event)) {
		case NEED_NUMBER:
			sprintf(tbuf, "%d", Event1.Data.Value);
			added = tbuf;
			break;

		case NEED_UNIT:
			added = Text_String(UnitTypeClass::As_Reference(Event1.Data.Unit).Full_Name());;
			break;

		case NEED_AIRCRAFT:
			added = Text_String(AircraftTypeClass::As_Reference(Event1.Data.Aircraft).Full_Name());;
			break;

		case NEED_STRUCTURE:
			added = Text_String(BuildingTypeClass::As_Reference(Event1.Data.Structure).Full_Name());
			break;

		case NEED_INFANTRY:
			added = Text_String(InfantryTypeClass::As_Reference(Event1.Data.Infantry).Full_Name());
			break;

		case NEED_WAYPOINT:
			if (Event1.Data.Value < 26) {
				sprintf(tbuf, "'%c'", Event1.Data.Value + 'A');
			} else {
				sprintf(tbuf, "'%c%c'", (Event1.Data.Value / 26) + 'A'-1, (Event1.Data.Value % 26) + 'A');
			}
			added = tbuf;
			break;

		default:
			break;
	}

	/*
	**	Persistence indicator value.
	*/
	char pers = 'V';
	if (IsPersistant == SEMIPERSISTANT) pers = 'S';
	if (IsPersistant == PERSISTANT) pers = 'P';

	sprintf(_buffer, "%4.4s\t %s %c%c%c  %s%s",
		IniName,
		HouseTypeClass::As_Reference(House).Suffix,
		pers,
		special,
		special2,
		Name_From_Event(Event1.Event),
		added);
	return(_buffer);
}

#endif


/***********************************************************************************************
 * TriggerTypeClass::Attaches_To -- Determines what trigger can attach to.                     *
 *                                                                                             *
 *    This routine will examine the trigger events and return with a composit bitfield that    *
 *    indicates what this trigger can be attached to. This is used for trigger placement       *
 *    and logic processing.                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with AttachType bitfield representing what this trigger can be attached    *
 *          to.                                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/30/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AttachType TriggerTypeClass::Attaches_To(void) const
{
	AttachType attach = ::Attaches_To(Event1.Event);

	if (EventControl != MULTI_ONLY) {
		attach = attach | ::Attaches_To(Event2.Event);
	}
	return(attach);
}


/***********************************************************************************************
 * TriggerTypeClass::Read_INI -- reads triggers from the INI file                              *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Triggername = Eventname, Actionname, Data, Housename, TeamName, IsPersistant           *
 *                                                                                             *
 * This routine reads in the triggers & creates them. Then, other classes can                  *
 * get pointers to the triggers they're linked to.                                             *
 *                                                                                             *
 * The routine relies on the TeamTypeClasses already being loaded so it can resolve            *
 * references to teams in this function.                                                       *
 *                                                                                             *
 * Cell Trigger pointers & IsTrigger flags are set in DisplayClass::Read_INI(),                *
 * and cleared in the Map::Init() routine (which clears all cell objects to 0's).              *
 *                                                                                             *
 * Object's pointers are set in:                                                               *
 *      InfantryClass::Read_INI()                                                              *
 *      BuildingClass::Read_INI()                                                              *
 *      UnitClass::Read_INI()                                                                  *
 *      TerrainClass::Read_INI()                                                               *
 * The object trigger pointers are cleared in the ObjectClass constructor.                     *
 *                                                                                             *
 * The House's EMSListOf triggers is set in this routine, and cleared in the                   *
 * HouseClass::Init() routine.                                                                 *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      buffer      buffer to hold the INI data                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      This function must be called before any other class's Read_INI.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void TriggerTypeClass::Read_INI(CCINIClass & ini)
{
	TriggerTypeClass *trigger;				// Working trigger pointer.
	char buf[128];

	int len = ini.Entry_Count(INI_Name());
	for (int index = 0; index < len; index++) {
		char const * entry = ini.Get_Entry(INI_Name(), index);

		/*
		**	Create a new trigger.
		*/
		trigger = new TriggerTypeClass();

		/*
		**	Get the trigger entry.
		*/
		ini.Get_String(INI_Name(), entry, NULL, buf, sizeof(buf));

		/*
		**	Fill in the trigger.
		*/
		trigger->Fill_In((char *)entry, buf);
	}

	if (NewINIFormat < 2) {
		/*
		**	Fix up the self-referential trigger pointers.
		*/
		for (int trig_index = 0; trig_index < TriggerTypes.Count(); trig_index++) {
			TriggerTypeClass * trigger = TriggerTypes.Ptr(trig_index);

			char * ptr = (char *)trigger->Action1.Trigger.Raw();
			if (ptr /*&& trigger->Action1.Trigger.Raw() != -1*/) {
				trigger->Action1.Trigger = TriggerTypeClass::From_Name(ptr);
				free(ptr);
			}

			ptr = (char *)trigger->Action2.Trigger.Raw();
			if (ptr /*&& trigger->Action2.Trigger.Raw() != -1*/) {
				trigger->Action2.Trigger = TriggerTypeClass::From_Name(ptr);
				free(ptr);
			}
		}
	}
}


/***********************************************************************************************
 * TriggerTypeClass::Fill_In -- fills in trigger from the given INI entry                      *
 *                                                                                             *
 * This routine fills in the given trigger with the given name, and values from                *
 * the given INI entry.                                                                        *
 *                                                                                             *
 * (This routine is used by the scenario editor, to import teams from the MASTER.INI file.)    *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Triggername = Eventname, Actionname, Data, Housename, TeamName, IsPersistant           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      name      mnemonic for the desired trigger                                             *
 *      entry      INI entry to parse                                                          *
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
void TriggerTypeClass::Fill_In(char * name, char * entry)
{
	assert(TriggerTypes.ID(this) == ID);

	/*
	**	Set its name.
	*/
	Set_Name(name);

	IsPersistant = PersistantType(atoi(strtok(entry, ",")));
	House = HousesType(atoi(strtok(NULL, ",")));
	EventControl = MultiStyleType(atoi(strtok(NULL, ",")));
	ActionControl = MultiStyleType(atoi(strtok(NULL, ",")));

	Event1.Read_INI();
	Event2.Read_INI();
	Action1.Read_INI();
	Action2.Read_INI();
}


/***********************************************************************************************
 * TriggerTypeClass::Write_INI -- Stores all trigger types to the INI database specified.      *
 *                                                                                             *
 *    This routine will write out all trigger type objects to the INI database. Any existing   *
 *    trigger types in the database will be cleared out.                                       *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to have the trigger types added.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::Write_INI(CCINIClass & ini)
{
	ini.Clear("Triggers");
	ini.Clear(INI_Name());

	/*
	**	Now write all the trigger data out
	*/
	for (int index = 0; index < TriggerTypes.Count(); index++) {
//	for (int index = TriggerTypes.Count()-1; index >= 0; index--) {
		char buf[256];
		TriggerTypeClass * trigger = TriggerTypes.Ptr(index);

		trigger->Build_INI_Entry(buf);
		ini.Put_String(INI_Name(), trigger->IniName, buf);
	}
}


/***********************************************************************************************
 * TriggerTypeClass::Build_INI_Entry -- Construct the INI entry into the buffer specified.     *
 *                                                                                             *
 *    This low level routine will take the information in this trigger type and store it       *
 *    into a buffer such that the resultant string can be stored into an INI database for      *
 *    later retrieval.                                                                         *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer to store the INI entry string.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure the buffer is big enough. Usually 128 bytes is more than sufficient.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::Build_INI_Entry(char * buffer) const
{
	/*
	**	Build the root portion of the trigger event.
	*/
	sprintf(buffer, "%d,%d,%d,%d,", IsPersistant, House, EventControl, ActionControl);

	/*
	**	Append the event and action values.
	*/
	buffer += strlen(buffer);
	Event1.Build_INI_Entry(buffer);

	strcat(buffer, ",");
	buffer += strlen(buffer);
	Event2.Build_INI_Entry(buffer);

	strcat(buffer, ",");
	buffer += strlen(buffer);
	Action1.Build_INI_Entry(buffer);

	strcat(buffer, ",");
	buffer += strlen(buffer);
	Action2.Build_INI_Entry(buffer);
}


#if defined(CHEAT_KEYS) || defined(SCENARIO_EDITOR)
/***********************************************************************************************
 * TriggerTypeClass::Draw_It -- Draws this trigger as if it were a line in a list box.         *
 *                                                                                             *
 *    This routine is called when triggers are assigned to a list box and then must be drawn.  *
 *    It will display an identifying text string with as much information as is useful.        *
 *                                                                                             *
 * INPUT:   index    -- The index number of this line in the list box.                         *
 *                                                                                             *
 *          x,y      -- The pixel coordinate of the upper left corner of the text box.         *
 *                                                                                             *
 *          width,height   -- The dimensions of the text box to display the description in.    *
 *                                                                                             *
 *          selected -- Is this a selected line? If so, then it should be highlighted.         *
 *                                                                                             *
 *          flags    -- The text print flags to use to display this text string.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::Draw_It(int , int x, int y, int width, int height, bool selected, TextPrintType flags) const
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
 * TriggerTypeClass::Init -- Initialize the trigger type object management system.             *
 *                                                                                             *
 *    This routine should be called to initialize the trigger type object system. It should    *
 *    be called when clearing out a scenario.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   All trigger types will be destroyed by this routine.                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::Init(void)
{
	TriggerTypes.Free_All();
}


/***********************************************************************************************
 * TriggerTypeClass::From_Name -- Convert an ASCII name into a trigger type pointer.           *
 *                                                                                             *
 *    Given just an ASCII representation of the trigger type, this routine will return with    *
 *    a pointer to the trigger type it refers to. Typical use of this is when parsing          *
 *    scenario INI files.                                                                      *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the name to use to identify the trigger type class object to   *
 *                   be looked up.                                                             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the trigger type class object that matches the name      *
 *          specified. If no match could be found, then NULL is returned.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerTypeClass * TriggerTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (int index = 0; index < TriggerTypes.Count(); index++) {
			if (stricmp(TriggerTypes.Ptr(index)->Name(), name) == 0) {
				return(TriggerTypes.Ptr(index));
			}
		}
	}
	return(NULL);
}
