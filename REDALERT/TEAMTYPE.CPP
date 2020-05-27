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

/* $Header: /CounterStrike/TEAMTYPE.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TEAMTYPE.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/28/96                                                     *
 *                                                                                             *
 *                  Last Update : July 30, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TeamMissionClass::Description -- Compose a text description of team mi                    *
 *   TeamMissionClass::Draw_It -- Draws a team mission list box entry.                         *
 *   TeamMission_Needs -- Determines what extra data is needed by team miss                    *
 *   TeamTypeClass::As_Pointer -- gets ptr for team type with given name                       *
 *   TeamTypeClass::Build_INI_Entry -- Builds the INI entry for this team type.                *
 *   TeamTypeClass::Create_One_Of -- Creates a team of this type.                              *
 *   TeamTypeClass::Description -- Builds a description of the team.                           *
 *   TeamTypeClass::Destroy_All_Of -- Destroy all teams of this type.                          *
 *   TeamTypeClass::Detach -- Detach the specified target from this team type.                 *
 *   TeamTypeClass::Draw_It -- Display the team type in a list box.                            *
 *   TeamTypeClass::Edit -- Edit the team type.                                                *
 *   TeamTypeClass::Fill_In -- fills in trigger from the given INI entry                       *
 *   TeamTypeClass::From_Name -- Converts a name into a team type pointer.                     *
 *   TeamTypeClass::Init -- pre-scenario initialization                                        *
 *   TeamTypeClass::Member_Description -- Builds a member description string                   *
 *   TeamTypeClass::Mission_From_Name -- returns mission for given name                        *
 *   TeamTypeClass::Name_From_Mission -- returns name for given mission                        *
 *   TeamTypeClass::Read_INI -- reads INI data                                                 *
 *   TeamTypeClass::Suggested_New_Team -- Suggests a new team to create.                       *
 *   TeamTypeClass::TeamTypeClass -- class constructor                                         *
 *   TeamTypeClass::Write_INI -- Write out the team types to the INI database.                 *
 *   TeamTypeClass::operator delete -- 'delete' operator                                       *
 *   TeamTypeClass::operator new -- 'new' operator                                             *
 *   TeamTypeClass::~TeamTypeClass -- class destructor                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


TeamMissionClass TeamMissions[TMISSION_COUNT] = {
	{TMISSION_ATTACK},
	{TMISSION_ATT_WAYPT},
	{TMISSION_FORMATION},
	{TMISSION_MOVE},
	{TMISSION_MOVECELL},
	{TMISSION_GUARD},
	{TMISSION_LOOP},
	{TMISSION_ATTACKTARCOM},
	{TMISSION_UNLOAD},
	{TMISSION_DEPLOY},
	{TMISSION_HOUND_DOG},
	{TMISSION_DO},
	{TMISSION_SET_GLOBAL},
	{TMISSION_LOAD},
	{TMISSION_SPY},
	{TMISSION_PATROL},
};



int atoh(char * str);


#if defined(CHEAT_KEYS) || defined(SCENARIO_EDITOR)
/***********************************************************************************************
 * TeamTypeClass::Draw_It -- Display the team type in a list box.                              *
 *                                                                                             *
 *    This is a helper routine that is used when this team type is stored into a list box      *
 *    and must be displayed. It will display the team type as a single line of text.           *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamTypeClass::Draw_It(int , int x, int y, int width, int height, bool selected, TextPrintType flags) const
{
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();
	static int _tabs[] = {35, 60, 80, 100};
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


/*
********************************** Globals **********************************
*/
char const * TeamTypeClass::TMissions[TMISSION_COUNT] = {
	"Attack...",
	"Attack Waypoint...",
	"Change Formation to...",
	"Move to waypoint...",
	"Move to Cell...",
	"Guard area (1/10th min)...",
	"Jump to line #...",
	"Attack Tarcom",
	"Unload",
	"Deploy",
	"Follow friendlies",
	"Do this...",
	"Set global...",
	"Invulnerable",
	"Load onto Transport",
	"Spy on bldg @ waypt...",
	"Patrol to waypoint..."
};


/***************************************************************************
 * TeamTypeClass::TeamTypeClass -- class constructor                       *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *   11/22/1995 JLB : Uses initializer constructor method.                 *
 *=========================================================================*/
TeamTypeClass::TeamTypeClass(void) :
	AbstractTypeClass(RTTI_TEAMTYPE, TeamTypes.ID(this), TXT_NONE, ""),
	IsRoundAbout(false),
	IsSuicide(false),
	IsAutocreate(false),
	IsPrebuilt(true),
	IsReinforcable(true),
	IsTransient(false),
	RecruitPriority(7),
	InitNum(0),
	MaxAllowed(0),
	Fear(0),
	House(HOUSE_NONE),
	Trigger(0),
	Origin(-1),
	Number(0),
	MissionCount(0),
	ClassCount(0)
{
	for (int i = 0; i < MAX_TEAM_CLASSCOUNT; i++) {
		Members[i].Class = NULL;
		Members[i].Quantity = 0;
	}
}


/***************************************************************************
 * TeamTypeClass::Init -- pre-scenario initialization                      *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *=========================================================================*/
void TeamTypeClass::Init(void)
{
	TeamTypes.Free_All();
}


/***************************************************************************
 * TeamTypeClass::As_Pointer -- gets ptr for team type with given name     *
 *                                                                         *
 * INPUT:                                                                  *
 *      name      name of teamtype                                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ptr to TeamType with that name                                     *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *=========================================================================*/
TeamTypeClass * TeamTypeClass::As_Pointer(char const * name)
{
	if (name) {
		for (int index = 0; index < TeamTypes.Count(); index++) {
			if (!stricmp(name, TeamTypes.Ptr(index)->IniName)) {
				return(TeamTypes.Ptr(index));
			}
		}
	}
	return(NULL);
}


/***************************************************************************
 * TeamTypeClass::Mission_From_Name -- returns team mission for given name *
 *                                                                         *
 * INPUT:                                                                  *
 *      name         name to compare                                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      mission for that name                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/13/1994 BR : Created.                                              *
 *=========================================================================*/
TeamMissionType TeamTypeClass::Mission_From_Name(char const * name)
{
	if (name) {
		for (TeamMissionType order = TMISSION_FIRST; order < TMISSION_COUNT; order++) {
			if (stricmp(TMissions[order], name) == 0) {
				return(order);
			}
		}
	}

	return(TMISSION_NONE);
}


/***************************************************************************
 * TeamTypeClass::Name_From_Mission -- returns name for given mission      *
 *                                                                         *
 * INPUT:                                                                  *
 *      order      mission to get name for                                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      name of mission                                                    *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/13/1994 BR : Created.                                              *
 *=========================================================================*/
char const * TeamTypeClass::Name_From_Mission(TeamMissionType order)
{
	assert((unsigned)order < TMISSION_COUNT);

	return(TMissions[order]);
}


/***************************************************************************
 * TeamTypeClass::operator new -- 'new' operator                           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      pointer to new TeamType                                            *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/28/1994 BR : Created.                                              *
 *=========================================================================*/
void * TeamTypeClass::operator new(size_t )
{
	void * ptr = TeamTypes.Allocate();
	if (ptr) {
		((TeamTypeClass *)ptr)->IsActive = true;
	}
	return(ptr);
}


/***************************************************************************
 * TeamTypeClass::operator delete -- 'delete' operator                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      ptr      pointer to delete                                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/28/1994 BR : Created.                                              *
 *=========================================================================*/
void TeamTypeClass::operator delete(void * ptr)
{
	if (ptr) {
		((TeamTypeClass *)ptr)->IsActive = false;
	}
	TeamTypes.Free((TeamTypeClass *)ptr);
}


/***********************************************************************************************
 * TeamTypeClass::Create_One_Of -- Creates a team of this type.                                *
 *                                                                                             *
 *    Use this routine to create a team object from this team type.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the newly created team object. If one could not be       *
 *          created, then NULL is returned.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TeamClass * TeamTypeClass::Create_One_Of(void) const
{
	if (ScenarioInit || Number < MaxAllowed) {
//	if (ScenarioInit || TeamClass::Number[ID] < MaxAllowed) {
		return(new TeamClass(this, HouseClass::As_Pointer(House)));
	}
	return(NULL);
}


/***********************************************************************************************
 * TeamTypeClass::Destroy_All_Of -- Destroy all teams of this type.                            *
 *                                                                                             *
 *    This routine will destroy all teams of this type. Typical use of this is from a trigger  *
 *    event.                                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamTypeClass::Destroy_All_Of(void) const
{
	for (int index = 0; index < Teams.Count(); index++) {
		TeamClass * team = Teams.Ptr(index);

		if (team->Class == this) {
			delete team;
			index--;
		}
	}
}


/***********************************************************************************************
 * TeamTypeClass::Suggested_New_Team -- Suggests a new team to create.                         *
 *                                                                                             *
 *    This routine will scan through the team types available and create teams of the          *
 *    type that can best utilize the existing unit mix.                                        *
 *                                                                                             *
 * INPUT:   house    -- Pointer to the house that this team is to be created for.              *
 *                                                                                             *
 *          atypes   -- A bit mask of the aircraft types available for this house.             *
 *                                                                                             *
 *          utypes   -- A bit mask of the unit types available for this house.                 *
 *                                                                                             *
 *          itypes   -- A bit mask of the infantry types available for this house.             *
 *                                                                                             *
 *          vtypes   -- A bit mask of the vessel types available for this house.               *
 *                                                                                             *
 *          alerted  -- Is this house alerted? If true, then the Autocreate teams will be      *
 *                      considered in the selection process.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team type that should be created. If no team should  *
 *          be created, then it returns NULL.                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/13/1995 JLB : Created.                                                                 *
 *   07/21/1995 JLB : Will autocreate team even if no members in field.                        *
 *=============================================================================================*/
TeamTypeClass const * TeamTypeClass::Suggested_New_Team(HouseClass * house, long , long , long , long , bool alerted)
//TeamTypeClass const * TeamTypeClass::Suggested_New_Team(HouseClass * house, long atypes, long utypes, long itypes, long vtypes, bool alerted)
{
//	TeamTypeClass const * best = NULL;
//	int bestvalue = 0;

	TeamTypeClass const * choices[20];
	int choicecount = 0;

	for (int index = 0; index < TeamTypes.Count(); index++) {
		TeamTypeClass const * ttype = TeamTypes.Ptr(index);

		assert(ttype != NULL);

		int maxnum = ttype->MaxAllowed;
		if ((alerted && !ttype->IsAutocreate) || (!alerted && ttype->IsAutocreate)) {
			maxnum = 0;
		}

		if (choicecount >= ARRAY_SIZE(choices)) break;

		if (ttype != NULL && ttype->House == house->Class->House && ttype->Number < maxnum) {

			choices[choicecount++] = ttype;

#ifdef OBSOLETE
			/*
			**	Determine what kind of units this team requires.
			*/
			long uneeded = 0;
			long ineeded = 0;
			long vneeded = 0;
			long aneeded = 0;
			for (int ctype = 0; ctype < ttype->ClassCount; ctype++) {
				switch (ttype->Members[ctype].Class->What_Am_I()) {
					case RTTI_INFANTRYTYPE:
						ineeded |= (1 << ((InfantryTypeClass *)ttype->Members[ctype].Class)->Type);
						break;

					case RTTI_UNITTYPE:
						uneeded |= (1 << ((UnitTypeClass *)ttype->Members[ctype].Class)->Type);
						break;

					case RTTI_VESSELTYPE:
						vneeded |= (1 << ((VesselTypeClass *)ttype->Members[ctype].Class)->Type);
						break;

					case RTTI_AIRCRAFTTYPE:
						aneeded |= (1 << ((AircraftTypeClass *)ttype->Members[ctype].Class)->Type);
						break;
				}
			}

			/*
			**	If this team can use the types required, then consider it a possible
			**	team type to create.
			*/
			int value = 0;
			if ((aneeded & atypes) != 0 || (ineeded & itypes) != 0 || (uneeded & utypes) != 0 || (vneeded & vtypes) != 0) {
				value = ttype->RecruitPriority;
			} else {
				value = ttype->RecruitPriority/2;
			}

			if (best == NULL || bestvalue < value) {
				bestvalue = value;
				best = ttype;
			}
#endif
		}
	}

	if (choicecount > 0) {
		return(choices[Random_Pick(0, choicecount-1)]);
	}
	return(NULL);

//	return(best);
}


/***********************************************************************************************
 * TeamTypeClass::From_Name -- Converts a name into a team type pointer.                       *
 *                                                                                             *
 *    This routine is used to convert an ASCII name of a team type into the corresponding      *
 *    team type pointer.                                                                       *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name of the team type.                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team type that this ASCII name represents. If there  *
 *          is no match, the NULL is returned.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TeamTypeClass * TeamTypeClass::From_Name(char const * name)
{
	if (name) {
		for (int index = 0; index < TeamTypes.Count(); index++) {
			if (stricmp(name, TeamTypes.Ptr(index)->IniName) == 0) {
				return(TeamTypes.Ptr(index));
			}
		}
	}
	return(0);
}


/***********************************************************************************************
 * TeamMission_Needs -- Determines what extra data is needed by team mission.                  *
 *                                                                                             *
 *    This routine will return the required extra data that the specified team mission will    *
 *    need.                                                                                    *
 *                                                                                             *
 * INPUT:   tmtype   -- The team mission type to check.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the data type needed for this team mission.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
NeedType TeamMission_Needs(TeamMissionType tmtype)
{
	switch (tmtype) {
		/*
		**	Requires a formation type.
		*/
		case TMISSION_FORMATION:
			return(NEED_FORMATION);

		/*
		**	Team mission requires a target quarry value.
		*/
		case TMISSION_ATTACK:
			return(NEED_QUARRY);

		/*
		**	Team mission requires a data value.
		*/
		case TMISSION_MOVECELL:
			return(NEED_HEX_NUMBER);

		case TMISSION_SET_GLOBAL:
		case TMISSION_GUARD:
		case TMISSION_LOOP:
			return(NEED_NUMBER);

		/*
		**	Team mission requires a waypoint.
		*/
		case TMISSION_PATROL:
		case TMISSION_MOVE:
		case TMISSION_ATT_WAYPT:
		case TMISSION_SPY:
			return(NEED_WAYPOINT);

		/*
		**	Team mission requires a general mission type.
		*/
		case TMISSION_DO:
			return(NEED_MISSION);

		default:
			break;
	}
	return(NEED_NONE);
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * TeamMissionClass::Draw_It -- Draws a team mission list box entry.                           *
 *                                                                                             *
 *    This routine will display a team mission list box entry. It converts the index number    *
 *    into the appropriate text string and then displays the text at the coordinates           *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   index    -- The index number to use when rendering the team mission.               *
 *                                                                                             *
 *          x,y      -- The X,Y coordinate to use when drawing the team mission entry.         *
 *                                                                                             *
 *          width, height  -- The dimensions of the area that the description can be           *
 *                            rendered into.                                                   *
 *                                                                                             *
 *          selected -- Is this entry currently selected? If so, then it should be rendered    *
 *                      differently.                                                           *
 *                                                                                             *
 *          flags    -- Text print control flags used for when the text is printed.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamMissionClass::Draw_It(int index, int x, int y, int width, int height, bool selected, TextPrintType flags)
{
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();
	static int _tabs[] = {13,40};
	if ((flags & 0x0F) == TPF_6PT_GRAD || (flags & 0x0F) == TPF_EFNT) {

		if (selected) {
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect(x, y, x + width - 1, y + height - 1,
				scheme->Shadow);
		} else {
			if (!(flags & TPF_USE_GRAD_PAL)) {
				flags = flags | TPF_MEDIUM_COLOR;
			}
		}

		Conquer_Clip_Text_Print(Description(index), x, y, scheme, TBLACK, flags, width, _tabs);
	} else {
		Conquer_Clip_Text_Print(Description(index), x, y,
			(selected ? &ColorRemaps[PCOLOR_DIALOG_BLUE] : &ColorRemaps[PCOLOR_GREY]), TBLACK, flags, width, _tabs);
	}
}


/***********************************************************************************************
 * TeamTypeClass::Edit -- Edit the team type.                                                  *
 *                                                                                             *
 *    This routine handles the editing dialog box for the team type.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the team type edited? A FALSE indicates that the dialog was canceled.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TeamTypeClass::Edit(void)
{
	/*
	**	Dialog & button dimensions
	*/
	enum {
		D_DIALOG_W = 400,
		D_DIALOG_H = 250,
		D_DIALOG_X = 0,
		D_DIALOG_Y = 0,

		D_NAME_X=D_DIALOG_X+35,
		D_NAME_Y=D_DIALOG_Y+27,
		ED_WIDTH=40,

		D_CHECK_X=D_DIALOG_X+35,							// Start of check box attribute list.
		D_CHECK_Y=D_NAME_Y+25,

		CB_SPACING_Y=9,										// Vertical spacing between check box lines.
		CB_SPACING_X=8,										// Horizontal spacing for check box description text.
		D_SPACING_X=9,											// Horizontal spacing between data entry fields.

		D_CANCEL_W = 50,
		D_CANCEL_H = 9,
		D_CANCEL_X = D_DIALOG_X + D_DIALOG_W - (D_CANCEL_W+35),
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - (D_CANCEL_H+20),

		D_OK_W = 50,
		D_OK_H = 9,
		D_OK_X = D_DIALOG_X + D_DIALOG_W - (D_OK_W+18)*2,
		D_OK_Y = D_CANCEL_Y
	};

	/*
	**	Button enumerations:
	*/
	enum {
		BUTTON_NAME=100,
		BUTTON_RECRUIT,
		BUTTON_MAXNUM,
		BUTTON_INITNUM,
		BUTTON_HOUSE,
		BUTTON_ROUNDABOUT,
		BUTTON_LEARNING,
		BUTTON_SUICIDE,
		BUTTON_AUTO,
		BUTTON_PREBUILT,
		BUTTON_REINFORCE,
		BUTTON_MISSION1,
		BUTTON_MISSION2,
		BUTTON_ADD,
		BUTTON_INSERT,
		BUTTON_DELETE,
		BUTTON_REPLACE,
		BUTTON_ARG,
		BUTTON_FORMATION,
		BUTTON_MEMBERS,
		BUTTON_MISSION,
		BUTTON_TRIGGER,
		BUTTON_ORIGIN,
		BUTTON_OK,
		BUTTON_CANCEL,
		BUTTON_QUARRY,
	};

	/*
	**	Dialog variables:
	*/
	ControlClass * commands = 0;
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	/*
	**	Team name edit field.
	*/
	char name_buf[10];
	EditClass name_edt(BUTTON_NAME, name_buf, sizeof(name_buf),
		TPF_EFNT | TPF_NOSHADOW,
		D_NAME_X, D_NAME_Y, ED_WIDTH, 9, EditClass::ALPHANUMERIC);
	strcpy(name_buf, IniName);
	commands = &name_edt;

	/*
	**	House ownership of this team.
	*/
	char housetext[25] = "";
	DropListClass housebtn(BUTTON_HOUSE, housetext, sizeof(housetext),
		TPF_EFNT | TPF_NOSHADOW,
		name_edt.X+name_edt.Width+D_SPACING_X, name_edt.Y, 55, 8*5,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		housebtn.Add_Item(HouseTypeClass::As_Reference(house).IniName);
	}
	if (House == HOUSE_NONE) House = HOUSE_GOOD;
	housebtn.Set_Selected_Index(House);
	housebtn.Add(*commands);

	/*
	**	Recruit priority for this team.
	*/
	char recr_buf[4];
	EditClass recr_edt(BUTTON_RECRUIT, recr_buf, sizeof(recr_buf),
		TPF_EFNT | TPF_NOSHADOW,
		housebtn.X+housebtn.Width+5+D_SPACING_X, housebtn.Y, ED_WIDTH, 9, EditClass::NUMERIC);
	sprintf(recr_buf, "%d", RecruitPriority);
	recr_edt.Add(*commands);

	/*
	**	Maximum allowed for this team type.
	*/
	char maxnum_buf[4];
	EditClass maxnum_edt(BUTTON_MAXNUM, maxnum_buf, sizeof(maxnum_buf),
		TPF_EFNT | TPF_NOSHADOW,
		recr_edt.X+recr_edt.Width+D_SPACING_X, recr_edt.Y, ED_WIDTH, 9, EditClass::NUMERIC);
	sprintf(maxnum_buf, "%d", MaxAllowed);
	maxnum_edt.Add(*commands);

	/*
	**	Initial number for this team type.
	*/
	char initnum_buf[4];
	EditClass initnum_edt(BUTTON_INITNUM, initnum_buf, sizeof(initnum_buf),
		TPF_EFNT | TPF_NOSHADOW,
		maxnum_edt.X+maxnum_edt.Width+D_SPACING_X, maxnum_edt.Y, ED_WIDTH, 9, EditClass::NUMERIC);
	sprintf(initnum_buf, "%d", InitNum);
	initnum_edt.Add(*commands);

	/*
	**	Waypoint preference to create/reinforce this team.
	*/
	char origin[4];
	EditClass originbtn(BUTTON_ORIGIN, origin, sizeof(origin),
		TPF_EFNT | TPF_NOSHADOW,
		initnum_edt.X+initnum_edt.Width+D_SPACING_X, initnum_edt.Y, 20, 9, EditClass::ALPHA);
	*originbtn.Get_Text() = '\0';
	if (Origin != -1) {
		if (Origin < 26) {
			sprintf(originbtn.Get_Text(), "%c", Origin + 'A');
		} else {
			sprintf(originbtn.Get_Text(), "%c%c", (Origin/26) + 'A'-1, Origin % 26 + 'A');
		}
	}
	originbtn.Add(*commands);

	/*
	**	Members of this team control button.
	*/
	TextButtonClass membersbtn(BUTTON_MEMBERS, "Members", TPF_EBUTTON, name_edt.X, name_edt.Y+12, 50);
	membersbtn.Add(*commands);

	/*
	**	Trigger to assign to each member of this team (when object joins team).
	*/
	char trigtext[25] = "";
	DropListClass triggerbtn(BUTTON_TRIGGER, trigtext, sizeof(trigtext),
		TPF_EFNT | TPF_NOSHADOW,
		D_DIALOG_X+D_DIALOG_W-95, membersbtn.Y, 60, 8*5,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	triggerbtn.Add_Item("<NONE>");
	for (int index = 0; index < TriggerTypes.Count(); index++) {
		triggerbtn.Add_Item(TriggerTypes.Ptr(index)->IniName);
	}
	if (Trigger.Is_Valid()) {
		triggerbtn.Set_Selected_Index(Trigger->Name());
	} else {
		triggerbtn.Set_Selected_Index(0);
	}
	triggerbtn.Add(*commands);

	/*
	**	Roundabout travel logic attribute for this team.
	*/
	CheckBoxClass roundbtn(BUTTON_ROUNDABOUT, D_CHECK_X, D_CHECK_Y);
	if (IsRoundAbout) {
		roundbtn.Turn_On();
	} else {
		roundbtn.Turn_Off();
	}
	roundbtn.Add(*commands);

	/*
	**	Suicide travel to target attribute.
	*/
	CheckBoxClass suicidebtn(BUTTON_SUICIDE, D_CHECK_X, roundbtn.Y+CB_SPACING_Y);
	if (IsSuicide) {
		suicidebtn.Turn_On();
	} else {
		suicidebtn.Turn_Off();
	}
	suicidebtn.Add(*commands);

	/*
	**	Autocreate attribute for this team.
	*/
	CheckBoxClass autocreatebtn(BUTTON_AUTO, D_CHECK_X, suicidebtn.Y+CB_SPACING_Y);
	if (IsAutocreate) {
		autocreatebtn.Turn_On();
	} else {
		autocreatebtn.Turn_Off();
	}
	autocreatebtn.Add(*commands);

	/*
	**	Prebuild team members attribute for this team.
	*/
	CheckBoxClass prebuildbtn(BUTTON_PREBUILT, D_CHECK_X, autocreatebtn.Y+CB_SPACING_Y);
	if (IsPrebuilt) {
		prebuildbtn.Turn_On();
	} else {
		prebuildbtn.Turn_Off();
	}
	prebuildbtn.Add(*commands);

	/*
	**	Reinforce this team in progress attribute.
	*/
	CheckBoxClass reinforcebtn(BUTTON_REINFORCE, D_CHECK_X, prebuildbtn.Y+CB_SPACING_Y);
	if (IsReinforcable) {
		reinforcebtn.Turn_On();
	} else {
		reinforcebtn.Turn_Off();
	}
	reinforcebtn.Add(*commands);

	/*
	**	Create drop box of possible team missions. This is used when building/editing the
	**	team mission list.
	*/
	char droptext[45];
	DropListClass missionlist1(BUTTON_MISSION1, droptext, sizeof(droptext),
		TPF_EFNT | TPF_NOSHADOW,
		reinforcebtn.X, reinforcebtn.Y+15, 170, 8*8,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (TeamMissionType tm = TMISSION_FIRST; tm < TMISSION_COUNT; tm++) {
		missionlist1.Add_Item(TeamTypeClass::Name_From_Mission(tm));
	}
	missionlist1.Set_Selected_Index(0);
	missionlist1.Add_Tail(*commands);

	/*
	**	Optional mission argument entry field.
	*/
	char arg_buf[6] = {0};
	EditClass arg_edt(BUTTON_ARG, arg_buf, sizeof(arg_buf),
		TPF_EFNT | TPF_NOSHADOW,
		missionlist1.X + missionlist1.Width + 15, missionlist1.Y, 60, -1, EditClass::ALPHANUMERIC);
//	arg_edt.Add(*commands);

	char qtext[55];
	DropListClass qlist(BUTTON_QUARRY, qtext, sizeof(qtext),
		TPF_EFNT | TPF_NOSHADOW,
		missionlist1.X + missionlist1.Width + 15, missionlist1.Y, 100, 5*8,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (QuarryType q = QUARRY_FIRST; q < QUARRY_COUNT; q++) {
		qlist.Add_Item(QuarryName[q]);
	}
	qlist.Set_Selected_Index(0);
	qlist.Add_Tail(*commands);

	char ftext[55];
	DropListClass flist(BUTTON_FORMATION, ftext, sizeof(ftext),
		TPF_EFNT | TPF_NOSHADOW,
		missionlist1.X + missionlist1.Width + 15, missionlist1.Y, 100, 5*8,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (FormationType f = FORMATION_FIRST; f < FORMATION_COUNT; f++) {
		flist.Add_Item(FormationName[f]);
	}
	flist.Set_Selected_Index(0);
	flist.Add_Tail(*commands);

	char mtext[55];
	DropListClass mlist(BUTTON_MISSION, mtext, sizeof(mtext),
		TPF_EFNT | TPF_NOSHADOW,
		missionlist1.X + missionlist1.Width + 15, missionlist1.Y, 100, 5*8,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (MissionType m = MISSION_FIRST; m < MISSION_COUNT; m++) {
		mlist.Add_Item(MissionClass::Mission_Name(m));
	}
	mlist.Set_Selected_Index(0);
	mlist.Add_Tail(*commands);

	TListClass<TeamMissionClass *> missionlist2(BUTTON_MISSION2, missionlist1.X+60, missionlist1.Y+22, 240, 8*7,
		TPF_EFNT | TPF_NOSHADOW,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (index = 0; index < MissionCount; index++) {
		missionlist2.Add_Item(new TeamMissionClass(MissionList[index]));
//		missionlist2.Add_Item(&TeamMissions[MissionList[index].Mission]);
	}
	static int tabs[] = {13, 40};		// list box tab stops
	missionlist2.Set_Tabs(tabs);
	missionlist2.Add_Tail(*commands);

	/*
	**	Mission editing command buttons.
	*/
	TextButtonClass addbtn(BUTTON_ADD, "Append", TPF_EBUTTON, D_NAME_X, missionlist1.Y+missionlist1.Height+1, 50);
	addbtn.Add(*commands);

	TextButtonClass insertbtn(BUTTON_INSERT, "Insert", TPF_EBUTTON, addbtn.X, addbtn.Y+10, 50);
	insertbtn.Add(*commands);

	TextButtonClass delbtn(BUTTON_DELETE, "Delete", TPF_EBUTTON, insertbtn.X, insertbtn.Y+10, 50);
	delbtn.Add(*commands);

	TextButtonClass repbtn(BUTTON_REPLACE, "Replace", TPF_EBUTTON, delbtn.X, delbtn.Y+10, 50);
	repbtn.Add(*commands);


	TextButtonClass okbtn(BUTTON_OK, TXT_OK, TPF_EBUTTON, D_OK_X, D_OK_Y, D_OK_W, D_OK_H);
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_EBUTTON, D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	/*
	**	Initialize
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Create the list
	*/
	okbtn.Add(*commands);
	cancelbtn.Add_Tail(*commands);

	/*
	**	Main Processing Loop
	*/
	int lastindex = -1;
	int lastcount = -1;
	int lastbutton = -1;
	bool cancel = false;
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
		if (display) {

			/*
			**	Add the optional data entry field as necessary.
			*/
			arg_edt.Remove();
			qlist.Remove();
			flist.Remove();
			mlist.Remove();
			switch (TeamMission_Needs(TeamMissionType(missionlist1.Current_Index()))) {
				default:
					break;

				case NEED_MISSION:
					mlist.Add(*commands);
					break;

				case NEED_FORMATION:
					flist.Add(*commands);
					break;

				case NEED_WAYPOINT:
				case NEED_NUMBER:
				case NEED_HEX_NUMBER:
					arg_edt.Add(*commands);
					break;

				case NEED_QUARRY:
					qlist.Add(*commands);
					break;
			}

			/*
			**	Display the dialog box
			*/
			Hide_Mouse();
			Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);

			Draw_Caption(TXT_TEAM_EDIT, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

			/*
			**	Draw the captions
			*/
			Fancy_Text_Print("Name:", name_edt.X, name_edt.Y-7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("House:", housebtn.X, housebtn.Y-7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Pri:", recr_edt.X, recr_edt.Y-7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Max:", maxnum_edt.X, maxnum_edt.Y-7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Num:", initnum_edt.X, initnum_edt.Y-7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Loc:", originbtn.X, originbtn.Y-7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("#  Team Mission", missionlist2.X, missionlist2.Y-7, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Trigger:", triggerbtn.X-4, triggerbtn.Y+1, scheme, TBLACK, TPF_RIGHT | TPF_EFNT | TPF_NOSHADOW);

			Fancy_Text_Print(Member_Description(), membersbtn.X + membersbtn.Width + 3, membersbtn.Y+1, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);

			Fancy_Text_Print("Use safest, possibly longer, route to target?", roundbtn.X+CB_SPACING_X, roundbtn.Y, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Charge toward target ignoring distractions?", suicidebtn.X+CB_SPACING_X, suicidebtn.Y, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Only 'Autocreate A.I.' uses this team type?", autocreatebtn.X+CB_SPACING_X, autocreatebtn.Y, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Prebuild team members before team is created?", prebuildbtn.X+CB_SPACING_X, prebuildbtn.Y, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);
			Fancy_Text_Print("Automatically reinforce team whenever possible?", reinforcebtn.X+CB_SPACING_X, reinforcebtn.Y, scheme, TBLACK, TPF_EFNT | TPF_NOSHADOW);

			/*
			**	Redraw the buttons
			*/
			commands->Draw_All();
			Show_Mouse();

			display = false;
		}

		/*
		**	Get user input
		*/
		KeyNumType input = commands->Input();

		/*
		**	Process input
		*/
		switch (input) {

			/*
			**	Select a Mission on the right-hand mission list; update the Argument
			**	field to reflect the current value. This only serves as an aide to editing
			**	the team mission list.
			*/
			case BUTTON_MISSION2 | KN_BUTTON:
				if (missionlist2.Count() && lastcount == missionlist2.Count() && lastbutton == BUTTON_MISSION2 && lastindex == missionlist2.Current_Index()) {
					missionlist1.Set_Selected_Index(missionlist2.Current_Item()->Mission);

					switch (TeamMission_Needs(missionlist2.Current_Item()->Mission)) {
						case NEED_MISSION:
							mlist.Set_Selected_Index(missionlist2.Current_Item()->Data.Mission);
							break;

						case NEED_FORMATION:
							flist.Set_Selected_Index(missionlist2.Current_Item()->Data.Formation);
							break;

						case NEED_NUMBER:
							sprintf(arg_edt.Get_Text(), "%d", missionlist2.Current_Item()->Data.Value);
							break;

						case NEED_HEX_NUMBER:
							sprintf(arg_edt.Get_Text(), "%x", missionlist2.Current_Item()->Data.Value);
							break;

						case NEED_QUARRY:
							strcpy(qlist.Get_Text(), QuarryName[missionlist2.Current_Item()->Data.Quarry]);
							break;

						case NEED_WAYPOINT:
							if (missionlist2.Current_Item()->Data.Value < 26) {
								sprintf(arg_edt.Get_Text(), "%c", missionlist2.Current_Item()->Data.Value + 'A');
							} else {
								sprintf(arg_edt.Get_Text(), "%c%c", (missionlist2.Current_Item()->Data.Value)/26 + 'A'-1, (missionlist2.Current_Item()->Data.Value % 26) + 'A');
							}
							break;
					}
				}
				lastindex = missionlist2.Current_Index();
				lastcount = missionlist2.Count();
				display = true;
				break;

			/*
			**	Add current mission data to current position of team mission list. Any
			**	subsequent missions get moved downward.
			*/
			case BUTTON_INSERT | KN_BUTTON:
				if (missionlist2.Count() < MAX_TEAM_MISSIONS) {
					TeamMissionClass * tm = new TeamMissionClass;
					tm->Mission = TeamMissionType(missionlist1.Current_Index());
					tm->Data.Value = 0;
					switch (TeamMission_Needs(tm->Mission)) {
						case NEED_MISSION:
							tm->Data.Mission = MissionType(mlist.Current_Index());
							break;

						case NEED_FORMATION:
							tm->Data.Formation = FormationType(flist.Current_Index());
							break;

						case NEED_QUARRY:
							tm->Data.Quarry = QuarryType(qlist.Current_Index());
							break;

						case NEED_NUMBER:
							tm->Data.Value = atoi(arg_edt.Get_Text());
							break;

						case NEED_HEX_NUMBER:
							tm->Data.Value = atoh(arg_edt.Get_Text());
							break;

						case NEED_WAYPOINT:
							tm->Data.Value = toupper(*arg_edt.Get_Text()) - 'A';
							if ( *((arg_edt.Get_Text())+1)) {
								tm->Data.Value = (tm->Data.Value+1)*26;
								tm->Data.Value += toupper(*((arg_edt.Get_Text())+1)) - 'A';;
							}
							if ((unsigned)tm->Data.Value >= WAYPT_HOME) {
								tm->Data.Value = 0;
							}
							break;

						default:
							tm->Data.Value = 0;
							break;
					}
					missionlist2.Insert_Item(tm);
				}
				display = true;
				break;

			/*
			**	Add mission data to the end of the mission list.
			*/
			case BUTTON_ADD | KN_BUTTON:
				if (missionlist2.Count() < MAX_TEAM_MISSIONS) {
					TeamMissionClass * tm = new TeamMissionClass;
					tm->Mission = TeamMissionType(missionlist1.Current_Index());
					tm->Data.Value = 0;
					switch (TeamMission_Needs(tm->Mission)) {
						case NEED_MISSION:
							tm->Data.Mission = MissionType(mlist.Current_Index());
							break;

						case NEED_FORMATION:
							tm->Data.Formation = FormationType(flist.Current_Index());
							break;

						case NEED_QUARRY:
							tm->Data.Quarry = QuarryType(qlist.Current_Index());
							break;

						case NEED_NUMBER:
							tm->Data.Value = atoi(arg_edt.Get_Text());
							break;

						case NEED_HEX_NUMBER:
							tm->Data.Value = atoh(arg_edt.Get_Text());
							break;

						case NEED_WAYPOINT:
							tm->Data.Value = toupper(*arg_edt.Get_Text()) - 'A';;
							if ( *((arg_edt.Get_Text())+1)) {
								tm->Data.Value = (tm->Data.Value+1)*26;
								tm->Data.Value += toupper(*((arg_edt.Get_Text())+1)) - 'A';;
							}
							if ((unsigned)tm->Data.Value >= WAYPT_HOME) {
								tm->Data.Value = 0;
							}
							break;

						default:
							tm->Data.Value = 0;
							break;
					}
					missionlist2.Add_Item(tm);
					display = true;
				}
				break;

			/*
			**	Replace the currently selected mission with the work mission data.
			*/
			case BUTTON_REPLACE | KN_BUTTON:
				if (missionlist2.Count()) {
					TeamMissionClass * tm = missionlist2.Current_Item();
					tm->Mission = TeamMissionType(missionlist1.Current_Index());
					tm->Data.Value = 0;
					switch (TeamMission_Needs(tm->Mission)) {
						case NEED_MISSION:
							tm->Data.Mission = MissionType(mlist.Current_Index());
							break;

						case NEED_FORMATION:
							tm->Data.Formation = FormationType(flist.Current_Index());
							break;

						case NEED_QUARRY:
							tm->Data.Quarry = QuarryType(qlist.Current_Index());
							break;

						case NEED_NUMBER:
							tm->Data.Value = atoi(arg_edt.Get_Text());
							break;

						case NEED_HEX_NUMBER:
							tm->Data.Value = atoh(arg_edt.Get_Text());
							break;

						case NEED_WAYPOINT:
							tm->Data.Value = toupper(*arg_edt.Get_Text()) - 'A';
							if ( *((arg_edt.Get_Text())+1)) {
								tm->Data.Value = (tm->Data.Value+1)*26;
								tm->Data.Value += toupper(*((arg_edt.Get_Text())+1)) - 'A';
							}
							if ((unsigned)tm->Data.Value >= WAYPT_HOME) {
								tm->Data.Value = 0;
							}
							break;

						default:
							tm->Data.Value = 0;
							break;
					}
					missionlist2[missionlist2.Current_Index()] = tm;
				}
				display = true;
				break;

			/*
			**	Delete the currently selected mission.
			*/
			case BUTTON_DELETE | KN_BUTTON:
				if (missionlist2.Count()) {
					TeamMissionClass * tm = missionlist2.Current_Item();
					missionlist2.Remove_Index(missionlist2.Current_Index());
					delete tm;
				}
				display = true;
				break;

			/*
			**	Invoke the members dialog
			*/
			case BUTTON_MEMBERS | KN_BUTTON:

				/*
				**	Take editor focus away
				*/
				membersbtn.Turn_Off();

				/*
				**	Invoke the dialog
				*/
				Map.Team_Members(HousesType(housebtn.Current_Index()));

				/*
				**	Redraw
				*/
				display = true;
				break;

			/*
			**	When the OK button is selected, lift the values from the dialog box
			**	and place them into the team type object.
			*/
			case BUTTON_OK | KN_BUTTON:
				strtrim(name_edt.Get_Text());
				if (strlen(name_edt.Get_Text()) != 0) {
					strcpy(IniName, name_edt.Get_Text());
				} else {
					strcpy(IniName, "----");
				}

				IsRoundAbout = roundbtn.IsOn;
				IsSuicide = suicidebtn.IsOn;
				IsAutocreate = autocreatebtn.IsOn;
				IsPrebuilt = prebuildbtn.IsOn;
				IsReinforcable = reinforcebtn.IsOn;

				RecruitPriority = atoi(recr_edt.Get_Text());
				InitNum = atoi(initnum_edt.Get_Text());
				MaxAllowed = atoi(maxnum_edt.Get_Text());
				House = HousesType(housebtn.Current_Index());
				Trigger = NULL;
				if (triggerbtn.Current_Index() > 0) {
					Trigger = TriggerTypes.Ptr(triggerbtn.Current_Index()-1);
				}

				MissionCount = missionlist2.Count();
				for (index = 0; index < MissionCount; index++) {
					MissionList[index].Data.Value = 0;				// Clears extra bits.
					MissionList[index] = *missionlist2[index];
				}

#ifdef TOFIX
// the mission class objects pointed to in the list should be deleted?
#endif

				if (strlen(originbtn.Get_Text())) {
					if (strlen(originbtn.Get_Text()) == 1) {
						Origin = toupper(*originbtn.Get_Text()) - 'A';
					} else {
						Origin = (toupper(*originbtn.Get_Text())+1 - 'A' ) * 26;
						Origin += toupper(*(originbtn.Get_Text()+1)) - 'A';
					}
				} else {
					Origin = -1;
				}
				cancel = false;
				process = false;
				break;

			/*
			**	Cancel: return
			*/
			case BUTTON_CANCEL | KN_BUTTON:
				cancel = true;
				process = false;
				break;

			/*
			**	Unrecognized events are ignored. If any button related event is
			**	detected, then collapse any drop down list boxes. This keeps the
			**	dialog box clean.
			*/
			default:
				if (input & KN_BUTTON) {
					housebtn.Collapse();
					missionlist1.Collapse();
					triggerbtn.Collapse();
					display = true;
				}
				break;
		}

		/*
		**	Record the last dialog control touched so that a double click
		**	can be detected.
		*/
		if (input & KN_BUTTON) {
			lastbutton = (input & ~KN_BUTTON);
		}
	}

	return(!cancel);
}


int atoh(char * str)
{
	int retval = 0;
	while (*str) {
		retval *= 16;
		if (*str >= '0' && *str <= '9') {
			retval += *str-'0';
		} else {
			if (*str >= 'a' && *str <= 'f') {
				retval += 10 + (*str - 'a');
			} else {
				if (*str >= 'A' && *str <= 'F') {
					retval += 10 + (*str - 'A');
				}
			}
		}
		str++;
	}
	return(retval);
}

#endif


#if defined(CHEAT_KEYS) || defined(SCENARIO_EDITOR)
/***********************************************************************************************
 * TeamTypeClass::Member_Description -- Builds a member description string.                    *
 *                                                                                             *
 *    This routine will build a team member description string. The string will be composed    *
 *    of the team member type and quantity. As many team member types will be listed that      *
 *    can fit within a reasonable size.                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the text string that contains a description of the team  *
 *          type members.                                                                      *
 *                                                                                             *
 * WARNINGS:   The return string may be truncated if necessary to fit within reasonable size   *
 *             limits.                                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * TeamTypeClass::Member_Description(void) const
{
	static char buffer[128];

	buffer[0] = '\0';

	/*
	**	Fill in class & count for all classes
	*/
	for (int index = 0; index < ClassCount; index++) {
		char txt[10];

		strcat(buffer, Members[index].Class->IniName);
		strcat(buffer, ":");

		sprintf(txt, "%d", Members[index].Quantity);
		strcat(buffer, txt);

		if (index < ClassCount-1) {
			strcat(buffer, ",");
		}
	}

	if (strlen(buffer) > 25) {
		strcpy(&buffer[25-3], "...");
	}

	return(buffer);
}


/***********************************************************************************************
 * TeamTypeClass::Description -- Builds a description of the team.                             *
 *                                                                                             *
 *    This routine will build a brief description of the team type. This description is used   *
 *    in the team type list.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the composed text string that represents the team type.               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * TeamTypeClass::Description(void) const
{
	static char _buffer[128];
	char extra = ' ';
	char loc[3];

	loc[0] = loc[1] = loc[2] = 0;
	if (IsAutocreate) extra = '*';
	if (Origin > -1) {
//	if (Origin != -1) {
		if (Origin < 26) {
			loc[0] = 'A' + Origin;
		} else {
			loc[0] = Origin / 26 + 'A'-1;
			loc[1] = Origin % 26 + 'A';
		}
	}

	sprintf(_buffer, "%s\t%s\t%c%s\t%d\t%s", IniName, HouseTypeClass::As_Reference(House).Suffix, extra, loc, MissionCount, Member_Description());
	return(_buffer);
}


/***********************************************************************************************
 * TeamMissionClass::Description -- Compose a text description of team mission.                *
 *                                                                                             *
 *    This routine will create a text representation of a team mission. This description will  *
 *    be used in the list of team missions for display purposes.                               *
 *                                                                                             *
 * INPUT:   index -- The index to assign to this team. The index is used since some team       *
 *                   missions refer to the another team mission by index number.               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team mission text.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * TeamMissionClass::Description(int index) const
{
	static char buffer[64];

	sprintf(buffer, "%d\t%s", index, TeamTypeClass::Name_From_Mission(Mission));

	switch (TeamMission_Needs(Mission)) {
		case NEED_MISSION:
			strcat(buffer, MissionClass::Mission_Name(Data.Mission));
			break;

		case NEED_FORMATION:
			strcat(buffer, FormationName[Data.Quarry]);
			break;

		case NEED_NUMBER:
			sprintf(&buffer[strlen(buffer)], "%d", Data.Value);
			break;

		case NEED_HEX_NUMBER:
			sprintf(&buffer[strlen(buffer)], "%x", Data.Value);
			break;

		case NEED_QUARRY:
			strcat(buffer, QuarryName[Data.Quarry]);
			break;

		case NEED_WAYPOINT:
			if (Data.Value < 26) {
				sprintf(&buffer[strlen(buffer)], "%c", Data.Value + 'A');
			} else {
				sprintf(&buffer[strlen(buffer)], "%c%c", (Data.Value/26) + 'A'-1, (Data.Value % 26) + 'A');
			}
			break;

	}

	return(buffer);
}
#endif


/***********************************************************************************************
 * TeamTypeClass::Detach -- Detach the specified target from this team type.                   *
 *                                                                                             *
 *    This routine is called when some object is about to be removed from the game system and  *
 *    all references to it must be severed. This will check to see if the specified object     *
 *    is a trigger that this team refers to. If so, then the reference will be cleared.        *
 *                                                                                             *
 * INPUT:   target   -- The target object to remove references to.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamTypeClass::Detach(TARGET target, bool)
{
	if (Is_Target_TriggerType(target) && Trigger.Is_Valid() && Trigger == As_TriggerType(target)) {
		Trigger = NULL;
	}
}


/***************************************************************************
 * TeamTypeClass::Read_INI -- reads INI data                               *
 *                                                                         *
 * INI entry format:                                                       *
 *      TeamName = Housename,Roundabout,Learning,Suicide,Spy,Mercenary,    *
 *       RecruitPriority,MaxAllowed,InitNum,Fear,                          *
 *       ClassCount,Class:Num,Class:Num,...,                               *
 *       MissionCount,Mission:Arg,Mission:Arg,Mission:Arg,...              *
 *                                                                         *
 * INPUT:                                                                  *
 *      buffer      buffer to hold the INI data                            *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *   02/01/1995 BR : No del team if no classes (editor needs empty teams!) *
 *=========================================================================*/
void TeamTypeClass::Read_INI(CCINIClass & ini)
{
	TeamTypeClass * team;					// Working team pointer.
	char buf[500];								// INI entry buffer

	int len = ini.Entry_Count(INI_Name());

	/*
	**	Loop for all team entries, create and fill in.
	*/
	for (int index = 0; index < len; index++) {
		team = new TeamTypeClass();
		if (team != NULL) {
			char const * entry = ini.Get_Entry(INI_Name(), index);
			ini.Get_String(INI_Name(), entry, NULL, buf, sizeof(buf));
			team->Fill_In((char *)entry, buf);
		}
	}
}


/***********************************************************************************************
 * TeamTypeClass::Fill_In -- fills in trigger from the given INI entry                         *
 *                                                                                             *
 * This routine fills in the given teamtype with the given name, and values from               *
 * the given INI entry.                                                                        *
 *                                                                                             *
 * (This routine is used by the scenario editor, to import teams from the MASTER.INI file.)    *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      TeamName = Housename,Roundabout,Learning,Suicide,Spy,Mercenary,                        *
 *       RecruitPriority,MaxAllowed,InitNum,Fear,                                              *
 *       ClassCount,Class:Num,Class:Num,...,                                                   *
 *       MissionCount,Mission:Arg,Mission:Arg,Mission:Arg,...                                  *
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
 *   11/29/1995 JLB : Revamped to use new team class.                                          *
 *=============================================================================================*/
void TeamTypeClass::Fill_In(char * name, char * entry)
{
	assert(TeamTypes.ID(this) == ID);

	/*
	**	Set its name
	*/
	Set_Name(name);

	House = HousesType(atoi(strtok(entry, ",")));

	int code;
	switch (NewINIFormat) {
		default:
			code = atoi(strtok(NULL, ","));
			IsRoundAbout = ((code & 0x0001) != 0);
			IsSuicide = ((code & 0x0002) != 0);
			IsAutocreate = ((code & 0x0004) != 0);
			IsPrebuilt = ((code & 0x0008) != 0);
			IsReinforcable = ((code & 0x0010) != 0);
			break;

		case 0:
		case 1:
			IsRoundAbout = atoi(strtok(NULL, ","));
			IsSuicide = atoi(strtok(NULL, ","));
			IsAutocreate = atoi(strtok(NULL, ","));
			IsPrebuilt = atoi(strtok(NULL, ","));
			IsReinforcable = atoi(strtok(NULL, ","));
			break;
	}

	RecruitPriority = atoi(strtok(NULL, ","));
	InitNum = atoi(strtok(NULL, ","));
	MaxAllowed = atoi(strtok(NULL, ","));
	Origin = atoi(strtok(NULL, ","));

	switch (NewINIFormat) {
		default:
			Trigger.Set_Raw(atoi(strtok(NULL, ",")));
			break;

		case 0:
		case 1:
			// Throw this token away -- it isn't used.
			strtok(NULL, ",");
			break;
	}

	/*
	**	Fetch the team member types and quantity values.
	*/
	ClassCount = atoi(strtok(NULL, ","));
	for (int index = 0; index < ClassCount; index++) {
		char * p1 = strtok(NULL, ",:");
		char * p2 = strtok(NULL, ",:");
		TechnoTypeClass const * otype = NULL;

		/*
		**	See if this is an infantry name
		*/
		InfantryType i_id = InfantryTypeClass::From_Name(p1);
		if (i_id != INFANTRY_NONE) {
			otype = &InfantryTypeClass::As_Reference(i_id);
		} else {

			/*
			**	See if this is a unit name
			*/
			UnitType u_id = UnitTypeClass::From_Name(p1);
			if (u_id != UNIT_NONE) {
				otype = &UnitTypeClass::As_Reference(u_id);
			} else {

				/*
				**	See if this is an aircraft name
				*/
				AircraftType a_id = AircraftTypeClass::From_Name(p1);
				if (a_id != AIRCRAFT_NONE) {
					otype = &AircraftTypeClass::As_Reference(a_id);
				} else {

					/*
					**	See if this is a vessel name.
					*/
					VesselType v_id = VesselTypeClass::From_Name(p1);
					if (v_id != VESSEL_NONE) {
						otype = &VesselTypeClass::As_Reference(v_id);
					}
				}
			}
		}

		/*
		**	If the name was resolved, add this class
		*/
		if (otype) {
			if (index < MAX_TEAM_CLASSCOUNT) {
				Members[index].Class = otype;
				Members[index].Quantity = atoi(p2);
			}
		} else {
			ClassCount--;
			if (index == 0) break;
			index--;
		}
	}
	ClassCount = min(MAX_TEAM_CLASSCOUNT, ClassCount);

	/*
	**	Fetch the missions assigned to this team type.
	*/
	MissionCount = atoi(strtok(NULL, ","));
	for (int index = 0; index < MissionCount; index++) {
		MissionList[index].Mission = TeamMissionType(atoi(strtok(NULL, ",:")));
		MissionList[index].Data.Value = atoi(strtok(NULL, ",:"));
	}

	if (NewINIFormat < 2) {
		/*
		**	Fetch the trigger ID.
		*/
		Trigger.Set_Raw(atoi(strtok(NULL, ",")));
	}
}


/***********************************************************************************************
 * TeamTypeClass::Write_INI -- Write out the team types to the INI database.                   *
 *                                                                                             *
 *    This routine will take all team types and write them out to the INI database specified.  *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that will hold al the teams.                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   All preexisting team data in the database will be erased by this routine.       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamTypeClass::Write_INI(CCINIClass & ini)
{
	ini.Clear("TeamTypes");
	ini.Clear(INI_Name());

	/*
	**	Now write all the team data out
	*/
	for (int index = 0; index < TeamTypes.Count(); index++) {
//	for (int index = TeamTypes.Count()-1; index >= 0; index--) {
		TeamTypeClass * team = TeamTypes.Ptr(index);
		char buf[256];

		buf[0] = 0;
		team->Build_INI_Entry(buf);
		ini.Put_String(INI_Name(), team->IniName, buf);
	}
}


/***********************************************************************************************
 * TeamTypeClass::Build_INI_Entry -- Builds the INI entry for this team type.                  *
 *                                                                                             *
 *    This routine is used to build the text string that will go into the INI database for     *
 *    a team of this type. This text string will be parsed back into a team object when the    *
 *    scenario INI is read in.                                                                 *
 *                                                                                             *
 * INPUT:   buf   -- Pointer to a buffer that will hold the team text entry.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure that the buffer can hold the string build. A size of 80 or so is        *
 *             usually sufficient.                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamTypeClass::Build_INI_Entry(char * buf)
{
	int code = 0;
	code |= IsRoundAbout ? 0x0001 : 0;
	code |= IsSuicide ? 0x0002 : 0;
	code |= IsAutocreate ? 0x0004 : 0;
	code |= IsPrebuilt ? 0x0008 : 0;
	code |= IsReinforcable ? 0x0010 : 0;

	/*
	**	Output the general data for this team type.
	*/
	sprintf(buf, "%d,%d,%d,%d,%d,%d,%d",
		House,
		code,
		RecruitPriority,
		InitNum,
		MaxAllowed,
		Origin,
		TriggerTypes.Logical_ID(Trigger)
		);
	buf += strlen(buf);

	/*
	**	For every class in the team, record the class's name & desired count
	*/
	sprintf (buf, ",%d", ClassCount);
	buf += strlen(buf);
	for (int i = 0; i < ClassCount; i++) {
		sprintf (buf, ",%s:%d", Members[i].Class->IniName, Members[i].Quantity);
		buf += strlen(buf);
	}

	/*
	**	Record the # of missions, and each mission name & argument value.
	*/
	sprintf(buf, ",%d", MissionCount);
	buf += strlen(buf);
	for (int i = 0; i < MissionCount; i++) {
		sprintf (buf, ",%d:%d", MissionList[i].Mission, MissionList[i].Data.Value);
		buf += strlen(buf);
	}
}


