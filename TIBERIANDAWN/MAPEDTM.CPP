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

/* $Header:   F:\projects\c&c\vcs\code\mapedtm.cpv   2.18   16 Oct 1995 16:52:16   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : MAPEDTM.CPP                              *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 7, 1994                         *
 *                                                                         *
 *                  Last Update : April 9, 1996 [BRR]                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   MapEditClass::Handle_Teams -- main team-dialog-handling function      *
 *   MapEditClass::Select_Team -- user selects a team from a list          *
 *   MapEditClass::Edit_Team -- user edits a team's options                *
 *   MapEditClass::Team_Members -- user picks makeup of a team             *
 *   MapEditClass::Build_Mission_list -- fills in mission list box         *
 *   MapEditClass::Draw_Member -- Draws a member of the team dialog box.   *
 *   MapEditClass::Team_Members -- Team members dialog                     * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

#ifdef SCENARIO_EDITOR


/***************************************************************************
 * MapEditClass::Handle_Teams -- main team-dialog-handling function        *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/08/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Handle_Teams(char const * caption)
{
	int rc;

	/*------------------------------------------------------------------------
	Team dialog processing loop:
	- Invoke the team selection dialog. If a team's selected, break
	  & return
	- If user wants to edit the current team, do so
	- If user wants to create new team, new a TeamTypeClass & edit it
	- If user wants to delete team, delete the current team
	- Keep looping until 'OK'
	------------------------------------------------------------------------*/
	for (;;) {

		/*
		............................. Select team .............................
		*/
		rc = Select_Team(caption);

		/*
		............................. 'OK'; break .............................
		*/
		if (rc == 0) {
			break;
		} else {

			/*
			............................... 'Edit' ................................
			*/
			if (rc == 1 && CurTeam) {
				if (Edit_Team()==0) {
					Changed = 1;
				}
			} else {

				/*
				................................ 'New' ................................
				*/
				if (rc == 2) {
					/*
					........................ Create a new team .........................
					*/
					CurTeam = new TeamTypeClass();
					if (CurTeam) {
						/*
						................... delete it if user cancels ...................
						*/
						if (Edit_Team()==-1) {
							delete CurTeam;
							CurTeam = NULL;
						} else {
							Changed = 1;
						}
					} else {

						/*
						................. Unable to create; issue warning ..................
						*/
						CCMessageBox().Process("No more teams available.");
						HiddenPage.Clear();
						Flag_To_Redraw(true);
						Render();
					}
				} else {

					/*
					.............................. 'Delete' ...............................
					*/
					if (rc==3) {
						if (CurTeam) {
							CurTeam->Remove();
							CurTeam = NULL;
						}
					}
				}
			}
		}
	}
}


/***************************************************************************
 * MapEditClass::Select_Team -- user selects a team from a list            *
 *                                                                         *
 *    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿           *
 *    ³                         Teams                          ³           *
 *    ³    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿    ³           *
 *    ³    ³ Name     House    Class:Count,Class:Count  ³³    ³           *
 *    ³    ³ Name     House    Class:Count,Class:Count  ÃÄ´    ³           *
 *    ³    ³ Name     House    Class:Count,Class:Count  ³ ³    ³           *
 *    ³    ³ Name     House    Class:Count,Class:Count  ³ ³    ³           *
 *    ³    ³                                            ³ ³    ³           *
 *    ³    ³                                            ³ ³    ³           *
 *    ³    ³                                            ÃÄ´    ³           *
 *    ³    ³                                            ³³    ³           *
 *    ³    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ    ³           *
 *    ³                                                        ³           *
 *    ³      [Edit]        [New]        [Delete]      [OK]     ³           *
 *    ³                                                        ³           *
 *    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, 1 = Edit, 2 = New, 3 = Delete                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      Uses HIDBUFF.                                                      *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/08/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Select_Team(char const * caption)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	enum {
		D_DIALOG_W = 528,											// dialog width
		D_DIALOG_H = 290,											// dialog height
		D_DIALOG_X = ((640 - D_DIALOG_W) / 2),				// centered x-coord
		D_DIALOG_Y = ((400 - D_DIALOG_H) / 2),				// centered y-coord
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),		// coord of x-center

		D_TXT8_H = 22,												// ht of 8-pt text
		D_MARGIN = 14,												// margin width/height

		D_LIST_W = 500,
		D_LIST_H = 208,
		D_LIST_X = D_DIALOG_X + D_MARGIN,
		D_LIST_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H,

		D_EDIT_W = 90,
		D_EDIT_H = 18,
		D_EDIT_X = D_DIALOG_X + (D_DIALOG_W / 8) - (D_EDIT_W / 2),
		D_EDIT_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_EDIT_H,

		D_NEW_W = 90,
		D_NEW_H = 18,
		D_NEW_X = D_DIALOG_X + (D_DIALOG_W / 8) * 3 - (D_NEW_W / 2),
		D_NEW_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_NEW_H,

		D_DELETE_W = 90,
		D_DELETE_H = 18,
		D_DELETE_X = D_DIALOG_X + (D_DIALOG_W / 8) * 5 - (D_DELETE_W / 2),
		D_DELETE_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_DELETE_H,

		D_OK_W = 90,
		D_OK_H = 18,
		D_OK_X = D_DIALOG_X + (D_DIALOG_W / 8) * 7 - (D_OK_W / 2),
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_OK_H,

		TEAMTXT_LEN = 43,				// max length of a team entry
	};

	/*........................................................................
	Button enumerations:
	........................................................................*/
	enum {
		TEAM_LIST=100,
		BUTTON_EDIT,
		BUTTON_NEW,
		BUTTON_DELETE,
		BUTTON_OK,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;
	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display;							// requested redraw level
	bool process;									// loop while true
	char *teamtext[TEAMTYPE_MAX + 1];		// text for defined teams
	KeyNumType input;								// user input
	bool edit_team = false;						// true = user wants to edit
	bool new_team = false;						// true = user wants to new
	bool del_team = false;						// true = user wants to new
	int i;											// loop counters
	int j;
	int def_idx;									// default list index
	static int tabs[] = {120, 180};				// list box tab stops
	char txt[10];
//	int housetxt;

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands = NULL;				// the button list

	ListClass teamlist (TEAM_LIST,
		D_LIST_X, D_LIST_Y, D_LIST_W, D_LIST_H,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		Hires_Retrieve("BTN-UP.SHP"),
		Hires_Retrieve("BTN-DN.SHP"));

	TextButtonClass editbtn (BUTTON_EDIT, "Edit",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_EDIT_X, D_EDIT_Y, D_EDIT_W, D_EDIT_H);

	TextButtonClass newbtn (BUTTON_NEW, "New",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NEW_X, D_NEW_Y, D_NEW_W, D_NEW_H);

	TextButtonClass deletebtn (BUTTON_DELETE, "Delete",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_DELETE_X, D_DELETE_Y, D_DELETE_W, D_DELETE_H);

	TextButtonClass okbtn (BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	/*
	........................... Fill in team names ...........................
	*/
	def_idx = 0;
	for (i = 0; i < TeamTypes.Count(); i++) {
		/*
		................... Generate string for this team .....................
		*/
		//teamtext[i] = (char *)HidPage.Get_Graphic_Buffer()->Get_Buffer() + TEAMTXT_LEN * i;
		teamtext[i] = new char[255];

		/*
		........................ Fill in name & house .........................
		*/
		strcpy(teamtext[i],TeamTypes.Ptr(i)->IniName);
		strcat(teamtext[i],"\t");
		strcat(teamtext[i], HouseTypeClass::As_Reference(TeamTypes.Ptr(i)->House).Suffix);
		strcat(teamtext[i],"\t");

		/*
		................ Fill in class & count for all classes ................
		*/
		for (j = 0; j < TeamTypes.Ptr(i)->ClassCount; j++) {
			sprintf (txt,"%s:%d", TeamTypes.Ptr(i)->Class[j]->IniName, TeamTypes.Ptr(i)->DesiredNum[j]);

			/*..................................................................
			Add entry if there's room; break otherwise
			(+ 3 for the ", " and the NULL; +3 again for the "..." for the next
			entry)
			..................................................................*/
			if (strlen(txt) + strlen(teamtext[i]) + 6 < TEAMTXT_LEN) {
				if (j > 0) {
					strcat(teamtext[i],", ");
				}
				strcat(teamtext[i],txt);
			} else {
				strcat(teamtext[i], "...");
				break;
			}
		}

		/*
		.................. Set def_idx if this is CurTeam .....................
		*/
		if (TeamTypes.Ptr(i)==CurTeam) {
			def_idx = i;
		}

		/*
		........................... Add to list box ...........................
		*/
		teamlist.Add_Item(teamtext[i]);
	}

	/*
	....................... Set CurTeam if it isn't ..........................
	*/
	if (TeamTypes.Count()==0) {
		CurTeam = NULL;
	} else {
		if (!CurTeam) {
			CurTeam = TeamTypes.Ptr(def_idx);
		}
	}

	/*
	............................ Create the list .............................
	*/
	commands = &teamlist;
	editbtn.Add_Tail(*commands);
	newbtn.Add_Tail(*commands);
	deletebtn.Add_Tail(*commands);
	okbtn.Add_Tail(*commands);

	/*
	------------------------ Init tab stops for list -------------------------
	*/
	teamlist.Set_Tabs(tabs);

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	display = REDRAW_ALL;
	process = true;
	while (process) {
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {

			/*
			...................... Display the dialog box ......................
			*/
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
				Draw_Caption(TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

				/*
				....................... Draw the captions .......................
				*/
				Fancy_Text_Print(caption, D_DIALOG_CX, D_DIALOG_Y + D_MARGIN,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}
			/*
			........................ Redraw the buttons ........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Draw_All();
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {
			case (TEAM_LIST | KN_BUTTON):
				def_idx = teamlist.Current_Index();
				if (def_idx < TeamTypes.Count())
					CurTeam = TeamTypes.Ptr(def_idx);
				break;

			case (BUTTON_EDIT | KN_BUTTON):
				if (CurTeam) {		// only allow if there's one selected
					process = false;
					edit_team = true;
				}
				break;

			case (BUTTON_NEW | KN_BUTTON):
				process = false;
				new_team = true;
				break;

			case (BUTTON_DELETE | KN_BUTTON):
				process = false;
				del_team = true;
				break;

			case (KN_RETURN):
			case (BUTTON_OK | KN_BUTTON):
				process = false;
				break;
		}
	}

	/*
	--------------------------- Redraw the display ---------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	for (i = 0; i < TeamTypes.Count(); i++) {
		delete [] teamtext[i];
	}
	if (edit_team) return(1);
	if (new_team) return(2);
	if (del_team) return(3);
	return(0);
}


/***************************************************************************
 * MapEditClass::Edit_Team -- user edits a team's options                  *
 *                                                                         *
 *  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿      *
 *  ³                         Team Editor                           ³      *
 *  ³                                                               ³      *
 *  ³            Name ______               [Roundabout]             ³      *
 *  ³        Priority ______  [   GDI   ]  [Learning  ]             ³      *
 *  ³         Max Num ______  [   NOD   ]  [Suicide   ]             ³      *
 *  ³        Init Num ______               [Autocreate]             ³      *
 *  ³            Fear ______               [Mercenary ]             ³      *
 *  ³                                      [Prebuild  ]             ³      *
 *  ³                                      [Reinforce ]             ³      *
 *  ³                                                               ³      *
 *  ³  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿             ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿  ³      *
 *  ³  ³                   ³^³             ³                   ³^³  ³      *
 *  ³  ³                   ÃÄ´  [Add >>]   ³                   ÃÄ´  ³      *
 *  ³  ³                   ³ ³  [Insert]   ³                   ³ ³  ³      *
 *  ³  ³                   ³ ³  [Delete]   ³                   ³ ³  ³      *
 *  ³  ³                   ÃÄ´    ____     ³                   ÃÄ´  ³      *
 *  ³  ³                   ³v³             ³                   ³v³  ³      *
 *  ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ             ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ  ³      *
 *  ³                                                               ³      *
 *  ³        [Members]          [Cancel]           [OK]             ³      *
 *  ³                                                               ³      *
 *  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ      *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = cancel                                                *
 *                                                                         *
 * WARNINGS:                                                               *
 *      CurTeam must NOT be NULL when this function is called.             *
 *      This routine invokes the Members dialog, which uses HIDBUFF.       *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/08/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Edit_Team(void)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	enum {
		D_DIALOG_W = 516,
		D_DIALOG_H = 376,
		D_DIALOG_X = ((640 - D_DIALOG_W) / 2),
		D_DIALOG_Y = ((400 - D_DIALOG_H) / 2),
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

		D_TXT8_H = 22,
		D_MARGIN = 14,

		D_NAME_W = 120,
		D_NAME_H = 18,
		D_NAME_X = D_DIALOG_X + D_MARGIN + 100,
		D_NAME_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H,

		D_PRIORITY_W = 120,
		D_PRIORITY_H = 18,
		D_PRIORITY_X = D_DIALOG_X + D_MARGIN + 100,
		D_PRIORITY_Y = D_NAME_Y + D_NAME_H,

		D_MAXNUM_W = 120,
		D_MAXNUM_H = 18,
		D_MAXNUM_X = D_DIALOG_X + D_MARGIN + 100,
		D_MAXNUM_Y = D_PRIORITY_Y + D_PRIORITY_H,

		D_INITNUM_W = 120,
		D_INITNUM_H = 18,
		D_INITNUM_X = D_DIALOG_X + D_MARGIN + 100,
		D_INITNUM_Y = D_MAXNUM_Y + D_MAXNUM_H,

		D_FEAR_W = 120,
		D_FEAR_H = 18,
		D_FEAR_X = D_DIALOG_X + D_MARGIN + 100,
		D_FEAR_Y = D_INITNUM_Y + D_INITNUM_H,

		D_GDI_W = 100,
		D_GDI_H = 18,
		D_GDI_X = D_NAME_X + D_NAME_W + D_MARGIN,
		D_GDI_Y = D_NAME_Y + D_NAME_H + D_NAME_H / 2,

		D_NOD_W = 100,
		D_NOD_H = 18,
		D_NOD_X = D_NAME_X + D_NAME_W + D_MARGIN,
		D_NOD_Y = D_GDI_Y + D_GDI_H,

		D_NEU_W = 100,
		D_NEU_H = 18,
		D_NEU_X = D_NAME_X + D_NAME_W + D_MARGIN,
		D_NEU_Y = D_NOD_Y + D_NOD_H,

		D_MULTI1_W = 50,
		D_MULTI1_H = 18,
		D_MULTI1_X = D_GDI_X,
		D_MULTI1_Y = D_GDI_Y,

		D_MULTI2_W = 50,
		D_MULTI2_H = 18,
		D_MULTI2_X = D_GDI_X + D_MULTI2_W,
		D_MULTI2_Y = D_GDI_Y,

		D_MULTI3_W = 50,
		D_MULTI3_H = 18,
		D_MULTI3_X = D_NOD_X,
		D_MULTI3_Y = D_NOD_Y,

		D_MULTI4_W = 50,
		D_MULTI4_H = 18,
		D_MULTI4_X = D_NOD_X + D_MULTI4_W,
		D_MULTI4_Y = D_NOD_Y,

		D_ROUNDABOUT_W = 130,
		D_ROUNDABOUT_H = 18,
		D_ROUNDABOUT_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_ROUNDABOUT_W,
		D_ROUNDABOUT_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H - 10,

		D_LEARNING_W = D_ROUNDABOUT_W,
		D_LEARNING_H = 18,
		D_LEARNING_X = D_ROUNDABOUT_X,
		D_LEARNING_Y = D_ROUNDABOUT_Y + D_ROUNDABOUT_H,

		D_SUICIDE_W = D_ROUNDABOUT_W,
		D_SUICIDE_H = 18,
		D_SUICIDE_X = D_ROUNDABOUT_X,
		D_SUICIDE_Y = D_LEARNING_Y + D_LEARNING_H,

		D_AUTOCREATE_W = D_ROUNDABOUT_W,
		D_AUTOCREATE_H = 18,
		D_AUTOCREATE_X = D_ROUNDABOUT_X,
		D_AUTOCREATE_Y = D_SUICIDE_Y + D_SUICIDE_H,

		D_MERCENARY_W = D_ROUNDABOUT_W,
		D_MERCENARY_H = 18,
		D_MERCENARY_X = D_ROUNDABOUT_X,
		D_MERCENARY_Y = D_AUTOCREATE_Y + D_AUTOCREATE_H,

		D_PREBUILT_W = D_ROUNDABOUT_W,
		D_PREBUILT_H = 18,
		D_PREBUILT_X = D_ROUNDABOUT_X,
		D_PREBUILT_Y = D_MERCENARY_Y + D_MERCENARY_H,

		D_REINFORCE_W = D_ROUNDABOUT_W,
		D_REINFORCE_H = 18,
		D_REINFORCE_X = D_ROUNDABOUT_X,
		D_REINFORCE_Y = D_PREBUILT_Y + D_PREBUILT_H,

		D_MISSION1_W = 180,
		D_MISSION1_H = 128,
		D_MISSION1_X = D_DIALOG_X + D_MARGIN,
		D_MISSION1_Y = D_REINFORCE_Y + D_REINFORCE_H + D_MARGIN,

		D_MISSION2_W = 180,
		D_MISSION2_H = 128,
		D_MISSION2_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_MISSION2_W,
		D_MISSION2_Y = D_MISSION1_Y,

		D_ADD_W = 100,
		D_ADD_H = 18,
		D_ADD_X = D_MISSION1_X + D_MISSION1_W + D_MARGIN,
		D_ADD_Y = D_MISSION1_Y + D_ADD_H,

		D_INSERT_W = 100,
		D_INSERT_H = 18,
		D_INSERT_X = D_MISSION1_X + D_MISSION1_W + D_MARGIN,
		D_INSERT_Y = D_ADD_Y + D_ADD_H,

		D_DEL_W = 100,
		D_DEL_H = 18,
		D_DEL_X = D_MISSION1_X + D_MISSION1_W + D_MARGIN,
		D_DEL_Y = D_INSERT_Y + D_INSERT_H,

		D_ARG_W = 100,
		D_ARG_H = 18,
		D_ARG_X = D_MISSION1_X + D_MISSION1_W + D_MARGIN,
		D_ARG_Y = D_DEL_Y + D_DEL_H,

		D_MEMBERS_W = 100,
		D_MEMBERS_H = 18,
		D_MEMBERS_X = D_DIALOG_X + (D_DIALOG_W / 6) - D_MEMBERS_W / 2,
		D_MEMBERS_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_MEMBERS_H,

		D_CANCEL_W = 100,
		D_CANCEL_H = 18,
		D_CANCEL_X = D_DIALOG_X + (D_DIALOG_W / 6) * 3 - D_CANCEL_W / 2,
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_CANCEL_H,

		D_OK_W = 100,
		D_OK_H = 18,
		D_OK_X = D_DIALOG_X + (D_DIALOG_W / 6) * 5 - D_OK_W / 2,
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_OK_H,
	};

	/*........................................................................
	Button enumerations:
	........................................................................*/
	enum {
		BUTTON_NAME=100,
		BUTTON_RECRUIT,
		BUTTON_MAXNUM,
		BUTTON_INITNUM,
		BUTTON_FEAR,
		BUTTON_GDI,
		BUTTON_NOD,
		BUTTON_NEU,
		BUTTON_JP,			// placeholder
		BUTTON_MULTI1,
		BUTTON_MULTI2,
		BUTTON_MULTI3,
		BUTTON_MULTI4,
		BUTTON_MULTI5,
		BUTTON_MULTI6,
		BUTTON_ROUNDABOUT,
		BUTTON_LEARNING,
		BUTTON_SUICIDE,
		BUTTON_AUTO,
		BUTTON_MERCENARY,
		BUTTON_PREBUILT,
		BUTTON_REINFORCE,
		BUTTON_MISSION1,
		BUTTON_MISSION2,
		BUTTON_ADD,
		BUTTON_INSERT,
		BUTTON_DEL,
		BUTTON_ARG,
		BUTTON_MEMBERS,
		BUTTON_OK,
		BUTTON_CANCEL,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables:
	........................................................................*/
	RedrawType display;							// requested redraw level
	bool process;									// loop while true
	KeyNumType input;
	bool cancel = false;							// true = user cancels
	char name_buf[12];
	char recr_buf[4];
	char maxnum_buf[4];
	char initnum_buf[4];
	char fear_buf[4];
	HousesType house;
	int roundabout;
	int learning;
	int suicide;
	int autocreate;
	int mercenary;
	int prebuilt;
	int reinforce;
	int missioncount;
	TeamMissionStruct missions[TeamTypeClass::MAX_TEAM_MISSIONS];
	char missionbuf[TeamTypeClass::MAX_TEAM_MISSIONS][20];
	int curmission;					// currently-selected mission index

	char arg_buf[4] = {0};
	static int tabs[] = {130, 180};		// list box tab stops
	int i,j;

	/*........................................................................
	Buttons:
	........................................................................*/
	ControlClass *commands;
	EditClass name_edt (BUTTON_NAME,
		name_buf, 8,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NAME_X, D_NAME_Y, D_NAME_W, D_NAME_H, EditClass::ALPHANUMERIC);

	EditClass recr_edt (BUTTON_RECRUIT,
		recr_buf, 3,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_PRIORITY_X, D_PRIORITY_Y, D_PRIORITY_W, D_PRIORITY_H, EditClass::NUMERIC);

	EditClass maxnum_edt (BUTTON_MAXNUM,
		maxnum_buf, 3,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MAXNUM_X, D_MAXNUM_Y, D_MAXNUM_W, D_MAXNUM_H, EditClass::NUMERIC);

	EditClass initnum_edt (BUTTON_INITNUM,
		initnum_buf, 3,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_INITNUM_X, D_INITNUM_Y, D_INITNUM_W, D_INITNUM_H, EditClass::NUMERIC);

	EditClass fear_edt (BUTTON_FEAR,
		fear_buf, 3,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_FEAR_X, D_FEAR_Y, D_FEAR_W, D_FEAR_H, EditClass::NUMERIC);

	TextButtonClass gdibtn (BUTTON_GDI, "GDI",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_GDI_X, D_GDI_Y, D_GDI_W, D_GDI_H);

	TextButtonClass nodbtn (BUTTON_NOD, "NOD",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NOD_X, D_NOD_Y, D_NOD_W, D_NOD_H);

	TextButtonClass neubtn (BUTTON_NEU, "NEUTRAL",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NEU_X, D_NEU_Y, D_NEU_W, D_NEU_H);

	TextButtonClass multi1btn (BUTTON_MULTI1, "M1",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI1_X, D_MULTI1_Y, D_MULTI1_W, D_MULTI1_H);

	TextButtonClass multi2btn (BUTTON_MULTI2, "M2",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI2_X, D_MULTI2_Y, D_MULTI2_W, D_MULTI2_H);

	TextButtonClass multi3btn (BUTTON_MULTI3, "M3",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI3_X, D_MULTI3_Y, D_MULTI3_W, D_MULTI3_H);

	TextButtonClass multi4btn (BUTTON_MULTI4, "M4",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI4_X, D_MULTI4_Y, D_MULTI4_W, D_MULTI4_H);

	TextButtonClass roundbtn (BUTTON_ROUNDABOUT, "Roundabout",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_ROUNDABOUT_X, D_ROUNDABOUT_Y, D_ROUNDABOUT_W, D_ROUNDABOUT_H);

	TextButtonClass learnbtn (BUTTON_LEARNING, "Learning",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_LEARNING_X, D_LEARNING_Y, D_LEARNING_W, D_LEARNING_H);

	TextButtonClass suicidebtn (BUTTON_SUICIDE, "Suicide",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_SUICIDE_X, D_SUICIDE_Y, D_SUICIDE_W, D_SUICIDE_H);

	TextButtonClass autocreatebtn (BUTTON_AUTO, "Autocreate",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_AUTOCREATE_X, D_AUTOCREATE_Y, D_AUTOCREATE_W, D_AUTOCREATE_H);

	TextButtonClass mercbtn (BUTTON_MERCENARY, "Mercenary",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MERCENARY_X, D_MERCENARY_Y, D_MERCENARY_W, D_MERCENARY_H);

	TextButtonClass prebuiltbtn (BUTTON_PREBUILT, "Prebuild",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_PREBUILT_X, D_PREBUILT_Y, D_PREBUILT_W, D_PREBUILT_H);

	TextButtonClass reinforcebtn (BUTTON_REINFORCE, "Reinforce",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_REINFORCE_X, D_REINFORCE_Y, D_REINFORCE_W, D_REINFORCE_H);

	ListClass missionlist1 (BUTTON_MISSION1,
		D_MISSION1_X, D_MISSION1_Y, D_MISSION1_W, D_MISSION1_H,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		Hires_Retrieve("BTN-UP.SHP"),
		Hires_Retrieve("BTN-DN.SHP"));

	ListClass missionlist2 (BUTTON_MISSION2,
		D_MISSION2_X, D_MISSION2_Y, D_MISSION2_W, D_MISSION2_H,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		Hires_Retrieve("BTN-UP.SHP"),
		Hires_Retrieve("BTN-DN.SHP"));

	TextButtonClass addbtn (BUTTON_ADD, "Add >>",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_ADD_X, D_ADD_Y, D_ADD_W, D_ADD_H);

	TextButtonClass insertbtn (BUTTON_INSERT, "Insert",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_INSERT_X, D_INSERT_Y, D_INSERT_W, D_INSERT_H);

	TextButtonClass delbtn (BUTTON_DEL, "Delete",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_DEL_X, D_DEL_Y, D_DEL_W, D_DEL_H);

	EditClass arg_edt (BUTTON_ARG, arg_buf, 4,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_ARG_X, D_ARG_Y, D_ARG_W, D_ARG_H, EditClass::ALPHANUMERIC);

	TextButtonClass membersbtn (BUTTON_MEMBERS, "Members",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MEMBERS_X, D_MEMBERS_Y, D_MEMBERS_W, D_MEMBERS_H);

	TextButtonClass okbtn (BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	/*
	........................... Copy team's state ............................
	*/
	strcpy(name_buf,CurTeam->IniName);
	sprintf(recr_buf,"%d",CurTeam->RecruitPriority);
	sprintf(maxnum_buf,"%d",CurTeam->MaxAllowed);
	sprintf(initnum_buf,"%d",CurTeam->InitNum);
	sprintf(fear_buf,"%d",CurTeam->Fear);
	roundabout = CurTeam->IsRoundAbout;
	learning = CurTeam->IsLearning;
	suicide = CurTeam->IsSuicide;
	house = CurTeam->House;
	autocreate = CurTeam->IsAutocreate;
	mercenary = CurTeam->IsMercenary;
	prebuilt = CurTeam->IsPrebuilt;
	reinforce = CurTeam->IsReinforcable;

	/*
	......................... Fill in mission lists ..........................
	*/
	for (i = 0; i < TMISSION_COUNT; i++) {
		missionlist1.Add_Item(TeamTypeClass::Name_From_Mission((TeamMissionType)i));
	}

	missioncount = CurTeam->MissionCount;
	for (i = 0; i < missioncount; i++) {
		missions[i] = CurTeam->MissionList[i];
	}
	Build_Mission_List(missioncount, missions, missionbuf, &missionlist2);

	curmission = 0;
	if (missioncount) {
		if (missions[curmission].Mission == TMISSION_MOVE || missions[curmission].Mission == TMISSION_UNLOAD) {
			sprintf(arg_buf,"%c",missions[curmission].Argument + 'A');
		} else {
			sprintf(arg_buf,"%d",missions[curmission].Argument);
		}
	}
	missionlist2.Set_Tabs(tabs);

	/*
	......................... Init the button states .........................
	*/
	name_edt.Set_Text(name_buf,8);
	recr_edt.Set_Text(recr_buf,3);
	maxnum_edt.Set_Text(maxnum_buf,3);
	initnum_edt.Set_Text(initnum_buf,3);
	fear_edt.Set_Text(fear_buf,3);
	arg_edt.Set_Text(arg_buf,3);

	if (roundabout) {
		roundbtn.Turn_On();
	}
	if (learning) {
		learnbtn.Turn_On();
	}
	if (suicide) {
		suicidebtn.Turn_On();
	}
	if (autocreate) {
		autocreatebtn.Turn_On();
	}
	if (mercenary) {
		mercbtn.Turn_On();
	}
	if (reinforce) {
		reinforcebtn.Turn_On();
	}
	if (prebuilt) {
		prebuiltbtn.Turn_On();
	}

	/*
	............................ Create the list .............................
	*/
	commands = &okbtn;
	cancelbtn.Add_Tail(*commands);
	membersbtn.Add_Tail(*commands);

	name_edt.Add_Tail(*commands);
	recr_edt.Add_Tail(*commands);
	maxnum_edt.Add_Tail(*commands);
	initnum_edt.Add_Tail(*commands);
	fear_edt.Add_Tail(*commands);

	gdibtn.Add_Tail(*commands);
	nodbtn.Add_Tail(*commands);
	neubtn.Add_Tail(*commands);

	roundbtn.Add_Tail(*commands);
	learnbtn.Add_Tail(*commands);
	suicidebtn.Add_Tail(*commands);
	autocreatebtn.Add_Tail(*commands);
	mercbtn.Add_Tail(*commands);
	prebuiltbtn.Add_Tail(*commands);
	reinforcebtn.Add_Tail(*commands);

	missionlist1.Add_Tail(*commands);
	missionlist2.Add_Tail(*commands);
	addbtn.Add_Tail(*commands);
	insertbtn.Add_Tail(*commands);
	delbtn.Add_Tail(*commands);
	arg_edt.Add_Tail(*commands);

	Set_House_Buttons (house, commands, BUTTON_GDI);

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	display = REDRAW_ALL;
	process = true;
	while (process) {
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {

			/*
			...................... Display the dialog box ......................
			*/
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);

				Draw_Caption(TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);
				/*
				....................... Draw the captions .......................
				*/
				Fancy_Text_Print("Team Edit", D_DIALOG_CX, D_DIALOG_Y + D_MARGIN,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Name", D_NAME_X - 5, D_NAME_Y,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Priority", D_PRIORITY_X - 5, D_PRIORITY_Y,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Max Num", D_MAXNUM_X - 5, D_MAXNUM_Y,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Init Num", D_INITNUM_X - 5, D_INITNUM_Y,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Fear", D_FEAR_X - 5, D_FEAR_Y,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}
			/*
			........................ Redraw the buttons ........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Draw_All();
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {
			case (BUTTON_NAME | KN_BUTTON):
				break;

			case (BUTTON_RECRUIT | KN_BUTTON):
				break;

			case (BUTTON_MAXNUM | KN_BUTTON):
				break;

			case (BUTTON_INITNUM | KN_BUTTON):
				break;

			case (BUTTON_FEAR | KN_BUTTON):
				break;

			/*..................................................................
			Toggle RoundAbout
			..................................................................*/
			case (BUTTON_ROUNDABOUT | KN_BUTTON):
				if (roundabout) {
					roundabout = 0;
					roundbtn.Turn_Off();
				} else {
					roundabout = 1;
					roundbtn.Turn_On();
				}
				break;

			/*..................................................................
			Toggle Learning
			..................................................................*/
			case (BUTTON_LEARNING | KN_BUTTON):
				if (learning) {
					learning = 0;
					learnbtn.Turn_Off();
				} else {
					learning = 1;
					learnbtn.Turn_On();
				}
				break;

			/*..................................................................
			Toggle Suicide
			..................................................................*/
			case (BUTTON_SUICIDE | KN_BUTTON):
				if (suicide) {
					suicide = 0;
					suicidebtn.Turn_Off();
				} else {
					suicide = 1;
					suicidebtn.Turn_On();
				}
				break;

			/*..................................................................
			Toggle Spy
			..................................................................*/
			case (BUTTON_AUTO | KN_BUTTON):
				if (autocreate) {
					autocreate = 0;
					autocreatebtn.Turn_Off();
				} else {
					autocreate = 1;
					autocreatebtn.Turn_On();
				}
				break;

			/*..................................................................
			Toggle Mercenary
			..................................................................*/
			case (BUTTON_MERCENARY | KN_BUTTON):
				if (mercenary) {
					mercenary = 0;
					mercbtn.Turn_Off();
				} else {
					mercenary = 1;
					mercbtn.Turn_On();
				}
				break;

			case (BUTTON_PREBUILT | KN_BUTTON):
				if (prebuilt) {
					prebuilt = 0;
					prebuiltbtn.Turn_Off();
				} else {
					prebuilt = 1;
					prebuiltbtn.Turn_On();
				}
				break;

			case (BUTTON_REINFORCE | KN_BUTTON):
				if (reinforce) {
					reinforce = 0;
					reinforcebtn.Turn_Off();
				} else {
					reinforce = 1;
					reinforcebtn.Turn_On();
				}
				break;

			/*..................................................................
			Select a Mission on the left-hand mission list
			..................................................................*/
			case (BUTTON_MISSION1 | KN_BUTTON):
				break;

			/*..................................................................
			Select a Mission on the right-hand mission list; update the Argument
			field to reflect the current value
			..................................................................*/
			case (BUTTON_MISSION2 | KN_BUTTON):
				if (missionlist2.Count() > 0 &&
					missionlist2.Current_Index() != curmission) {
					curmission = missionlist2.Current_Index();
					if (missions[curmission].Mission==TMISSION_MOVE || missions[curmission].Mission == TMISSION_UNLOAD) {
						sprintf(arg_buf,"%c",missions[curmission].Argument + 'A');
					} else {
						sprintf(arg_buf,"%d",missions[curmission].Argument);
					}
					arg_edt.Set_Text(arg_buf,3);
				}
				break;

			/*..................................................................
			Copy mission from left list box to right list box
			..................................................................*/
			case (BUTTON_ADD | KN_BUTTON):
			case (BUTTON_INSERT | KN_BUTTON):
				if (missioncount < TeamTypeClass::MAX_TEAM_MISSIONS) {
					/*
					** Set 'i' to the position we're going to add into; this will
					** be just AFTER the current item if we're adding, and it will
					** be the current item if we're inserting.
					*/
					if (input == (BUTTON_ADD | KN_BUTTON)) {
						i = missionlist2.Current_Index() + 1;
						if (i < 0) {
							i = 0;
						}
						if (i > missioncount) {
							i = missioncount;
						}
					} else {
						i = missionlist2.Current_Index();
						if (i < 0) {
							i = 0;
						}
						if (i >= missioncount && missioncount > 0) {
							i = missioncount - 1;
						}
					}

					/*
					** Move all other missions forward in the array
					*/
					for (j = missioncount; j > i; j--) {
						missions[j] = missions[j - 1];
					}

					/*
					** Set the Mission value based on 1st list box's index
					*/
					missions[i].Mission = (TeamMissionType)(TMISSION_FIRST + missionlist1.Current_Index());

					/*
					** Set the missions argument field
					*/
					if (missions[i].Mission == TMISSION_MOVE || missions[i].Mission == TMISSION_UNLOAD) {
						missions[i].Argument = toupper(arg_buf[0]) - 'A';
					} else {
						missions[i].Argument = atoi(arg_buf);
					}
					missioncount++;

					/*
					** Rebuild the list box from scratch
					*/
					Build_Mission_List(missioncount, missions, missionbuf, &missionlist2);

					/*
					** Update the list's current item index
					*/
					missionlist2.Set_Selected_Index(i);
				}
				break;

			/*..................................................................
			Delete mission from right-hand list box
			..................................................................*/
			case (BUTTON_DEL | KN_BUTTON):
				if (missioncount > 0) {
					i = missionlist2.Current_Index();
					if (i < 0 || i >= missioncount) {
						break;
					}

					/*
					** Move all missions back in the array
					*/
					for (j = i; j < missioncount - 1; j++) {
						missions[j] = missions[j + 1];
					}
					missioncount--;

					/*
					** Rebuild the list box from scratch
					*/
					Build_Mission_List(missioncount, missions, missionbuf, &missionlist2);

					/*
					** Update the list's current item index
					*/
					if (i >= missioncount) {
						i--;
						if (i < 0) {
							i = 0;
						}
						missionlist2.Set_Selected_Index(i);
					}
				}
				break;

			/*..................................................................
			Set house
			..................................................................*/
			case (BUTTON_GDI | KN_BUTTON):
			case (BUTTON_NOD | KN_BUTTON):
			case (BUTTON_NEU | KN_BUTTON):
			case (BUTTON_MULTI1 | KN_BUTTON):
			case (BUTTON_MULTI2 | KN_BUTTON):
			case (BUTTON_MULTI3 | KN_BUTTON):
			case (BUTTON_MULTI4 | KN_BUTTON):
				house = (HousesType)( (input & (~KN_BUTTON)) - BUTTON_GDI);
				Set_House_Buttons(house, commands, BUTTON_GDI);
				break;

			/*..................................................................
			Invoke the members dialog
			..................................................................*/
			case (BUTTON_MEMBERS | KN_BUTTON):
				/*
				.................... Take editor focus away .....................
				*/
				membersbtn.Turn_Off();

				/*
				....................... Invoke the dialog .......................
				*/
				Team_Members(house);

				/*
				............................ Redraw .............................
				*/
				display = REDRAW_ALL;
				break;

			/*..................................................................
			OK: return
			..................................................................*/
			case (BUTTON_OK | KN_BUTTON):
				cancel = false;
				process = false;
				break;

			/*..................................................................
			Cancel: return
			..................................................................*/
			case (BUTTON_CANCEL | KN_BUTTON):
				cancel = true;
				process = false;
				break;

			/*..................................................................
			Pass all other events to the currently-active text editor
			..................................................................*/
			default:
				break;
		}
	}

	/*
	--------------------------- Redraw the display ---------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	/*
	------------------------- If cancel, just return -------------------------
	*/
	if (cancel) {
		return(-1);
	}

	/*
	------------------------ Save selections & return ------------------------
	*/
	CurTeam->Set_Name(name_buf);
	CurTeam->RecruitPriority = atoi(recr_buf);
	CurTeam->MaxAllowed = atoi(maxnum_buf);
	CurTeam->InitNum = atoi(initnum_buf);
	CurTeam->IsRoundAbout = roundabout;
	CurTeam->IsLearning = learning;
	CurTeam->IsSuicide = suicide;
	CurTeam->IsAutocreate = autocreate;
	CurTeam->IsPrebuilt = prebuilt;
	CurTeam->IsReinforcable = reinforce;
	CurTeam->IsMercenary = mercenary;
	CurTeam->House = house;
	CurTeam->MissionCount = missioncount;
	for (i = 0 ; i < missioncount; i++) {
		CurTeam->MissionList[i] = missions[i];
	}

	return(0);
}


/***************************************************************************
 * MapEditClass::Team_Members -- user picks makeup of a team               *
 *                                                                         *
 * Team members are rendered in a 24 x 24 area; the Window coordinates     *
 * have to be set to this area when the object's 'Display()' routine is    *
 * called. Thus, the dialog's window coords have to be divisible by        *
 * 24. The height of the dialog is computed based on how many objects      *
 * there are in it.                                                        *
 *                                                                         *
 * 10 pixels are left between rows of objects, so the # of that type of    *
 * object can be displayed underneath the object.                          *
 *                                                                         *
 *  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                    *
 *  ³                 Team Members                    ³                    *
 *  ³                                                 ³                    *
 *  ³  ÚÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄ¿  ³                    *
 *  ³  ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³  ³                    *
 *  ³  ÃÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ´  ³                    *
 *  ³  ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³  ³                    *
 *  ³  ÃÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ´  ³                    *
 *  ³  ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³  ³                    *
 *  ³  ÃÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ´  ³                    *
 *  ³  ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³  ³                    *
 *  ³  ÃÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ´  ³                    *
 *  ³  ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³  ³                    *
 *  ³  ÃÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ´  ³                    *
 *  ³  ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³   ³  ³                    *
 *  ³  ÀÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÙ  ³                    *
 *  ³               [OK]      [Cancel]                ³                    *
 *  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                    *
 *                                                                         *
 * INPUT:                                                                  *
 *      house      house to display objects for                            *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = cancel                                                *
 *                                                                         *
 * WARNINGS:                                                               *
 *      CurTeam must NOT be NULL when this function is called.             *
 *      This routine uses HIDBUFF for data storage.                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *=========================================================================*/
#define TEENSY_WEENSY
/*
**	Dialog & button dimensions
*/
enum {
	D_DIALOG_W = 608,
	D_DIALOG_X = ((640 - D_DIALOG_W) / 2),
	D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

	D_TXT6_H = 14,
	D_MARGIN = 14,

#ifdef TEENSY_WEENSY
	//D_PICTURE_W = 32,
	//D_PICTURE_H = 24,
	D_PICTURE_W = 64,				// 9 pictures / row, 16 pixel margin on each side
	D_PICTURE_H = 48,
#else
	//D_PICTURE_W = 32,
	//D_PICTURE_H = 30,
	D_PICTURE_W = 64,
	D_PICTURE_H = 60,
#endif
	D_ROW_H = (D_PICTURE_H + 6),

	D_OK_W = 100,
	D_OK_H = 18,
	D_OK_X = D_DIALOG_CX - 10 - D_OK_W,
	D_OK_Y = 0,

	D_CANCEL_W = 100,
	D_CANCEL_H = 18,
	D_CANCEL_X = D_DIALOG_CX + 10,
	D_CANCEL_Y = 0,

};

/*************************************************************************** 
 * MapEditClass::Team_Members -- Team members dialog                       * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		house			house to show members for											*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		0 = OK, -1 = cancel																	*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   04/09/1996 BRR : Created.                                             * 
 *=========================================================================*/
int MapEditClass::Team_Members(HousesType house)
{
	/*
	**	Button enumerations:
	*/
	enum {
		BUTTON_OK = 100,
		BUTTON_CANCEL,
	};

	/*
	**	Redraw values: in order from "top" to "bottom" layer of the dialog
	**	(highest enum is the lowest layer). Each section of the map checks
	**	the requested redraw level to see if it's supposed to draw; if it's
	**	>= its level, it redraws.
	*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;
	RedrawType display;							// requested redraw level
	bool process;									// loop while true

	/*
	............................ Dialog variables ............................
	*/
	KeyNumType input;								// user input
	bool cancel = false;							// true = user cancels

	/*
	......................... Team display variables .........................
	*/
	const TechnoTypeClass **teamclass;		// array of team classes
	int *teamcount;								// array of class counts
	int numcols;									// # units displayed horizontally
	int numrows;									// # units displayed vertically
//	int col;											// horizontal picture index
//	int row;											// vertical picture index
//	int x,y;

	/*
	**	Dialog dimensions.
	*/
	int dlg_y;
	int dlg_h;										// dialog height
	int dlg_picture_top;							// coord of top of pictures
	int msg_y;										// y-coord for object names

	/*
	**	Values for parsing the classes.
	*/
	InfantryType i_id;
	AircraftType a_id;
	UnitType u_id;
	int curclass = -1;			// current index into 'teamclass'; can be invalid!
										// (is based on current mouse position)
	int numclasses;				// current # classes in the team (limited to <=5)
	int maxclasses;				// max # classes available
	int i,j;

	/*
	**	Values for timing when mouse held down.
	*/
	int lheld = 0;
	int rheld = 0;
	long tdelay[3] = {5, 20, 0};
	int tindex = 0;
	long heldtime;

	/*
	**	Buttons.
	*/
	ControlClass *commands;

	TextButtonClass okbtn (BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	/*
	**	Set up the team data arrays (ObjectTypeClass pointers & count)
	*/
	teamclass = (const TechnoTypeClass **)(new TechnoTypeClass *[MAX_TEAM_CLASSES]);
	teamcount = new int[MAX_TEAM_CLASSES];

	/*
	**	Fill in the ObjectTypeClass array with all available object type ptrs,
	**	checking to be sure this house can own the object
	*/
	i = 0;
	for (i_id = INFANTRY_FIRST; i_id < INFANTRY_COUNT; i_id++) {
		if (Verify_House(house,&InfantryTypeClass::As_Reference(i_id))) {
			teamclass[i] = &InfantryTypeClass::As_Reference(i_id);
			i++;
		}
	}

	for (a_id = AIRCRAFT_FIRST; a_id < AIRCRAFT_COUNT; a_id++) {
		if (Verify_House(house,&AircraftTypeClass::As_Reference(a_id))) {
			teamclass[i] = &AircraftTypeClass::As_Reference(a_id);
			i++;
		}
	}

	for (u_id = UNIT_FIRST; u_id < UNIT_COUNT; u_id++) {
		if (Verify_House(house,&UnitTypeClass::As_Reference(u_id))) {
			teamclass[i] = &UnitTypeClass::As_Reference(u_id);
			i++;
		}
	}

	/*
	**	Save max # classes.
	*/
	maxclasses = i;

	/*
	**	Fill in the 'count' array with data from the current team:
	**	- For every class in the current team, find that class type in the
	**	  'teamclass' array & set its count value
	*/
	for (j = 0; j < maxclasses; j++) {
		teamcount[j] = 0;
	}

	/*
	**	Loop through all classes in the team.
	*/
	for (i = 0; i < CurTeam->ClassCount; i++) {

		/*
		**	Find this class in our array.
		*/
		for (j = 0; j < maxclasses; j++) {

			/*
			**	Set the count; detect a match between the team's class & the
			**	'teamclass' array entry by comparing the actual pointers; typeid
			**	won't work because E1 & E2 are the same type class.
			*/
			if (CurTeam->Class[i] == teamclass[j]) {
				teamcount[j] = CurTeam->DesiredNum[i];
				break;
			}
		}
	}
	numclasses = CurTeam->ClassCount;

	/*
	**	Set up the dialog dimensions based on number of classes we have to draw
	**
	**	Compute picture rows & cols.
	*/
	numcols = (D_DIALOG_W - 16) / D_PICTURE_W;
	numrows = (maxclasses + numcols - 1) / numcols;

	//
	//	Dialog's height = top margin + label + picture rows + 
	// margin + label + margin + btn
	//
	dlg_h = (D_MARGIN + D_TXT6_H + D_MARGIN + (numrows * D_ROW_H) + 
		D_MARGIN + D_TXT6_H + D_MARGIN + D_OK_H + D_MARGIN);
	if (dlg_h > 400) {
		dlg_h = 400;
	}
	dlg_y = (400 - dlg_h) / 2;
	dlg_picture_top = dlg_y + D_MARGIN + D_TXT6_H + D_MARGIN;
	msg_y = dlg_y + D_MARGIN + D_TXT6_H + D_MARGIN + (numrows * D_ROW_H) + D_MARGIN;

	okbtn.Y = dlg_y + dlg_h - D_MARGIN - D_OK_H;
	cancelbtn.Y = dlg_y + dlg_h - D_MARGIN - D_CANCEL_H;

	/*
	**	Draw to SeenBuff.
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Make sure 'house' is valid.
	*/
	if (house!=HOUSE_GOOD && house!=HOUSE_BAD && house != HOUSE_MULTI1 &&
		house != HOUSE_MULTI2 && house != HOUSE_MULTI3 && house != HOUSE_MULTI4 ) {
		if (ScenPlayer == SCEN_PLAYER_MPLAYER) {
			house = HOUSE_MULTI1;
		} else {
			house = HOUSE_GOOD;
		}
	}

	/*
	**	Create the list.
	*/
	commands = &okbtn;
	cancelbtn.Add_Tail(*commands);

	/*
	**	Main Processing Loop.
	*/
	display = REDRAW_ALL;
	process = true;
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		**	Invoke game callback.
		*/
		Call_Back();

		/*
		**	Refresh display if needed.
		*/
		if (display) {

			/*
			**	Display the dialog box.
			*/
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {

				/*
				**	Display the constant background of this dialog.
				*/
				Dialog_Box(D_DIALOG_X, dlg_y, D_DIALOG_W, dlg_h);
				Draw_Caption(TXT_NONE, D_DIALOG_X, dlg_y, D_DIALOG_W);
				Fancy_Text_Print("Team Members", D_DIALOG_CX, dlg_y + D_MARGIN, CC_GREEN, TBLACK,
					TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);

				//
				//	Draw the objects.
				//
				for (i = 0; i < maxclasses; i++) {
					//
					//	Display the object along with any count value for it.
					//
					Draw_Member(teamclass[i], i, teamcount[i], house, 
						D_DIALOG_X + 16, dlg_picture_top);
				}

				if ((unsigned)curclass < maxclasses) {
					Fancy_Text_Print(teamclass[curclass]->Full_Name(),
						D_DIALOG_X + D_DIALOG_W / 2, msg_y, CC_TAN, TBLACK,
						TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);
				}
			}

			/*
			**	Redraw the buttons.
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Draw_All();
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		**	Get user input.
		*/
		input = commands->Input();

		/*
		**	Process input.
		*/
		switch (input) {

			/*
			**	Mouse buttons set or clear 'held' values
			*/
			case (KN_LMOUSE):
				if (curclass >= 0 && curclass < maxclasses) {
					lheld = 1;
					tindex = 2;
					heldtime = 0;
				}
				break;

			case (KN_RMOUSE):
				if (curclass >= 0 && curclass < maxclasses) {
					rheld = 1;
					tindex = 2;
					heldtime = 0;
				}
				break;

			case ((int)KN_LMOUSE | (int)KN_RLSE_BIT):
				lheld = 0;
				break;

			case ((int)KN_RMOUSE | (int)KN_RLSE_BIT):
				rheld = 0;
				break;

			/*
			**	OK: save values & return.
			*/
			case (BUTTON_OK | KN_BUTTON):
				process = false;
				break;

			/*
			**	Cancel: abort & return.
			*/
			case (BUTTON_CANCEL | KN_BUTTON):
				cancel = true;
				process = false;
				break;

			default:
				/*
				**	Compute new 'curclass' based on mouse position.
				*/
				i = (Get_Mouse_X() - 16 - D_DIALOG_X) / D_PICTURE_W +
					((Get_Mouse_Y() - dlg_picture_top) / D_ROW_H) * numcols;

				/*
				**	If it's changed, update class label.
				*/
				if (i != curclass) {

					curclass = i;

					/*
					**	Clear out the previously printed name of the item.
					*/
					Hide_Mouse();
					LogicPage->Fill_Rect(D_DIALOG_X + 8, msg_y, D_DIALOG_X + D_DIALOG_W - 9, msg_y + D_TXT6_H, BLACK);

					if ((unsigned)curclass < maxclasses) {
						Fancy_Text_Print(teamclass[curclass]->Full_Name(),
							D_DIALOG_X + D_DIALOG_W / 2, msg_y,
							CC_GREEN, TBLACK, 
							TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);
					}

					/*
					**	Force buttons to not be held.
					*/
					lheld = 0;
					rheld = 0;
					Show_Mouse();
				}
				break;
		}

		/*
		**	Check for a 'held' mouse button; if it's down, and the correct
		**	amount of time has gone by, increment/decrement the count for the
		**	current class.
		*/
		if (lheld) {

			/*
			**	The first time in, TickCount - heldtime will be larger than
			**	tdelay[2], so we increment the count immediately; then, we decrement
			**	tindex to go to the next time delay, which is longer; then, decr.
			**	again to go to the 1st time delay which is the shortest.
			*/
			if (TickCount.Time() - heldtime > tdelay[tindex]) {
				heldtime = TickCount.Time();
				if (tindex) {
					tindex--;
				}

				/*
				**	Detect addition of a new class.
				*/
				if (teamcount[curclass]==0) {

					/*
					**	Don't allow more classes than we can handle.
					*/
					if (numclasses == TeamTypeClass::MAX_TEAM_CLASSCOUNT) {
						continue;
					}
					numclasses++;
				}
				teamcount[curclass]++;

				/*
				**	Update number label.
				*/
				Draw_Member(teamclass[curclass], curclass, teamcount[curclass], 
					house, D_DIALOG_X + 16, dlg_picture_top);

			}

		} else {

			if (rheld) {

				/*
				**	The first time in, TickCount - heldtime will be larger than
				**	tdelay[2], so we increment the count immediately; then, we decrement
				**	tindex to go to the next time delay, which is longer; then, decr.
				**	again to go to the 1st time delay which is the shortest.
				*/
				if (TickCount.Time() - heldtime > tdelay[tindex]) {
					if (tindex) {
						tindex--;
					}
					heldtime = TickCount.Time();

					if (teamcount[curclass] > 0) {
						teamcount[curclass]--;

						/*
						**	Detect removal of a class.
						*/
						if (teamcount[curclass] == 0) {
							numclasses--;
						}
					}

					/*
					**	Update number label.
					*/
					Draw_Member(teamclass[curclass], curclass, teamcount[curclass], 
						house, D_DIALOG_X + 16, dlg_picture_top);

				}
			}
		}
	}

	/*
	**	Copy data into team.
	*/
	if (!cancel) {
		CurTeam->ClassCount = numclasses;
		i = 0;		// current team class index
		for (j = 0; j < maxclasses; j++) {
			if (teamcount[j] > 0) {
				CurTeam->DesiredNum[i] = teamcount[j];
				CurTeam->Class[i] = teamclass[j];
				i++;
			}
		}
	}

	/*
	**	Redraw the display.
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	delete [] teamclass;
	delete [] teamcount;

	if (cancel) return(-1);
	return(0);
}


/***********************************************************************************************
 * MapEditClass::Draw_Member -- Draws a member of the team dialog box.                         *
 *                                                                                             *
 *    This routine will display the cameo image of the potential team member. In the corner,   *
 *    it will show the current quantity of this member for the current team being edited.      *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the member object type.                                        *
 *                                                                                             *
 *          index -- The index into the team dialog box array of selectable objects. This is   *
 *                   used to determine the correct X and Y offsets to draw.                    *
 *                                                                                             *
 *          quant -- The quantity number to display in the corner of the image.                *
 *                                                                                             *
 *          house -- The owner of this object.                                                 *
 *				pic_x, pic_y -- x,y coords of upper-left corner to start drawing at
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void MapEditClass::Draw_Member(TechnoTypeClass const * ptr, int index, 
	int quant, HousesType house, int pic_x, int pic_y)
{
	int numcols = (D_DIALOG_W - 32) / D_PICTURE_W;
	int col = index % numcols;
	int row = index / numcols;
	int x = pic_x + col * D_PICTURE_W;
	int y = pic_y + row * D_ROW_H;

	WindowList[WINDOW_EDITOR][WINDOWX] = 0;
	WindowList[WINDOW_EDITOR][WINDOWY] = 0;
	WindowList[WINDOW_EDITOR][WINDOWWIDTH] = 640 / 8;
	WindowList[WINDOW_EDITOR][WINDOWHEIGHT] = 400;
	Change_Window((int)WINDOW_EDITOR);

	Hide_Mouse();
	Draw_Box(x, y, D_PICTURE_W, D_PICTURE_H, BOXSTYLE_GREEN_DOWN, true);

	ptr->Display(x + D_PICTURE_W / 2, y + D_PICTURE_H / 2, WINDOW_EDITOR, house);

	if (quant > 0) {
		Fancy_Text_Print("%d", x + 1, y + D_PICTURE_H - 16, 
			CC_GREEN, TBLACK, 
			TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_DROPSHADOW, quant);
	}

	Show_Mouse();

#if 0
	int numcols = (D_DIALOG_W - 16) / D_PICTURE_W;
	int col = index % numcols;
	int row = index / numcols;
	int dlg_y = 0;
	int x = D_DIALOG_X + 8 + col * D_PICTURE_W;
	int y = dlg_y + 8 + 13 + row * D_ROW_H;

	/*
	**	Change the window to this box.
	*/
	WindowList[WINDOW_EDITOR][WINDOWX] = x >> 3;
	WindowList[WINDOW_EDITOR][WINDOWY] = y;
	WindowList[WINDOW_EDITOR][WINDOWWIDTH] = D_PICTURE_W >> 3;
	WindowList[WINDOW_EDITOR][WINDOWHEIGHT] = D_PICTURE_H;
	Change_Window((int)WINDOW_EDITOR);

	Hide_Mouse();
	Draw_Box(x, y, D_PICTURE_W, D_PICTURE_H, BOXSTYLE_GREEN_DOWN, true);
	ptr->Display(WinW<<2, WinH>>1, WINDOW_EDITOR, house);
	if (quant > 0) {
		Fancy_Text_Print("%d", x+1, y+D_PICTURE_H-8, CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_DROPSHADOW, quant);
	}
	Show_Mouse();
#endif
}


/***************************************************************************
 * MapEditClass::Build_Mission_list -- fills in mission list box           *
 *                                                                         *
 * INPUT:                                                                  *
 *      missioncount      # of missions to add to the list                 *
 *      missions            array of TeamMissionStruct's                   *
 *      missionbuf         character arrays to store strings in            *
 *      list               list box to add strings to                      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = cancel                                                *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Build_Mission_List(int missioncount, TeamMissionStruct *missions,
	char missionbuf[TeamTypeClass::MAX_TEAM_MISSIONS][20], ListClass *list)
{
	/*
	** Start with an empty list
	*/
	while (list->Count()) {
		list->Remove_Item(list->Get_Item(0));
	}

	for (int i = 0; i < missioncount; i++) {
		/*
		** generate the string for a MOVE mission; the argument is the
		** letter-designation of the cell to move to.
		*/
		if (missions[i].Mission == TMISSION_MOVE || missions[i].Mission == TMISSION_UNLOAD) {
			sprintf(missionbuf[i],"%s\t%c",
				TeamTypeClass::Name_From_Mission(missions[i].Mission),
				missions[i].Argument + 'A');
		} else {

			/*
			** All other missions take a numeric argument.
			*/
			sprintf(missionbuf[i],"%s\t%d",
				TeamTypeClass::Name_From_Mission(missions[i].Mission),
				missions[i].Argument);
		}

		/*
		** Add the string to the list box
		*/
		list->Add_Item(missionbuf[i]);
	}
}

#endif
