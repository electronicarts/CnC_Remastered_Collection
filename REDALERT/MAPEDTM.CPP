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

/* $Header: /CounterStrike/MAPEDTM.CPP 1     3/03/97 10:25a Joe_bostic $ */
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
 *                  Last Update : May 7, 1996 [JLB]                        *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   MapEditClass::Draw_Member -- Draws a member of the team dialog box.   *
 *   MapEditClass::Handle_Teams -- main team-dialog-handling function      *
 *   MapEditClass::Select_Team -- user selects a team from a list          *
 *   MapEditClass::Team_Members -- user picks makeup of a team             *
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

	/*
	**	Team dialog processing loop:
	**	- Invoke the team selection dialog. If a team's selected, break
	**	  & return
	**	- If user wants to edit the current team, do so
	**	- If user wants to create new team, new a TeamTypeClass & edit it
	**	- If user wants to delete team, delete the current team
	**	- Keep looping until 'OK'
	*/
	for (;;) {

		/*
		**	Select team
		*/
		rc = Select_Team(caption);

		/*
		**	'OK'; break
		*/
		if (rc == 0) {
			break;
		} else {

			/*
			**	'Edit'
			*/
			if (rc == 1 && CurTeam) {
				if (CurTeam->Edit()) {
					Changed = 1;
				}
				HidPage.Clear();
				Flag_To_Redraw(true);
				Render();
			} else {

				/*
				**	'New'
				*/
				if (rc == 2) {

					/*
					**	Create a new team
					*/
					CurTeam = new TeamTypeClass();
					if (CurTeam) {
						/*
						**	delete it if user cancels
						*/
						if (!CurTeam->Edit()) {
							delete CurTeam;
							CurTeam = NULL;
						} else {
							Changed = 1;
						}
						HidPage.Clear();
						Flag_To_Redraw(true);
						Render();
					} else {

						/*
						**	Unable to create; issue warning
						*/
						WWMessageBox().Process("No more teams available.");
						HidPage.Clear();
						Flag_To_Redraw(true);
						Render();
					}
				} else {

					/*
					**	'Delete'
					*/
					if (rc==3) {
						if (CurTeam) {
							Detach_This_From_All(CurTeam->As_Target(), true);
							delete CurTeam;
							//CurTeam->Remove();
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
 *   05/07/1996 JLB : Streamlined and sorted team list.                    *
 *=========================================================================*/
int MapEditClass::Select_Team(char const * )
{
	/*
	**	Dialog & button dimensions
	*/
	enum {
		D_DIALOG_W = 400,											// dialog width
		D_DIALOG_H = 250,											// dialog height
		D_DIALOG_X = 0,				// centered x-coord
		D_DIALOG_Y = 0,				// centered y-coord
//		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),		// coord of x-center

		D_TXT8_H = 11,												// ht of 8-pt text
		D_MARGIN = 25,												// margin width/height

		D_LIST_W = (D_DIALOG_W-(D_MARGIN*2))-20,
		D_LIST_X = D_DIALOG_X + (D_DIALOG_W-D_LIST_W)/2,
		D_LIST_Y = D_DIALOG_Y + 20,
		D_LIST_H = (D_DIALOG_H-50)-D_LIST_Y,

		BUTTON_W = 45,
		BUTTON_H = 9,

		D_EDIT_W = BUTTON_W,
		D_EDIT_H = BUTTON_H,
		D_EDIT_X = D_DIALOG_X + D_DIALOG_W - (((D_EDIT_W+10)*4)+25),
		D_EDIT_Y = D_DIALOG_Y + D_DIALOG_H - 20 - D_EDIT_H,

		D_NEW_W = BUTTON_W,
		D_NEW_H = BUTTON_H,
		D_NEW_X = D_EDIT_X + D_EDIT_W + 10,
		D_NEW_Y = D_DIALOG_Y + D_DIALOG_H - 20 - D_NEW_H,

		D_DELETE_W = BUTTON_W,
		D_DELETE_H = BUTTON_H,
		D_DELETE_X = D_NEW_X + D_NEW_W + 10,
		D_DELETE_Y = D_DIALOG_Y + D_DIALOG_H - 20 - D_DELETE_H,

		D_OK_W = BUTTON_W,
		D_OK_H = BUTTON_H,
		D_OK_X = D_DELETE_X + D_DELETE_W + 10,
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - 20 - D_OK_H,

		TEAMTXT_LEN = 43,				// max length of a team entry
	};

	/*
	**	Button enumerations:
	*/
	enum {
		TEAM_LIST=100,
		BUTTON_EDIT,
		BUTTON_NEW,
		BUTTON_DELETE,
		BUTTON_OK,
	};

	/*
	**	Dialog variables
	*/
	bool edit_team = false;						// true = user wants to edit
	bool new_team = false;						// true = user wants to new
	bool del_team = false;						// true = user wants to new
	static int tabs[] = {35, 60, 80, 100};	// list box tab stops
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	/*
	**	Buttons
	*/
	GadgetClass * commands = NULL;				// the button list

	TListClass<CCPtr<TeamTypeClass> > teamlist (TEAM_LIST,
		D_LIST_X, D_LIST_Y, D_LIST_W, D_LIST_H,
		TPF_EFNT | TPF_NOSHADOW,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	TextButtonClass editbtn (BUTTON_EDIT, "Edit", TPF_EBUTTON, D_EDIT_X, D_EDIT_Y, D_EDIT_W);
	TextButtonClass newbtn (BUTTON_NEW, "New", TPF_EBUTTON, D_NEW_X, D_NEW_Y, D_NEW_W);
	TextButtonClass deletebtn (BUTTON_DELETE, "Delete", TPF_EBUTTON, D_DELETE_X, D_DELETE_Y, D_DELETE_W);
	TextButtonClass okbtn (BUTTON_OK, "OK", TPF_EBUTTON, D_OK_X, D_OK_Y, D_OK_W);

	/*
	**	Initialize
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Fill in team names
	*/
	for (int index = 0; index < TeamTypes.Count(); index++) {
		teamlist.Add_Item(TeamTypes.Ptr(index));
	}

	PNBubble_Sort(&teamlist[0], teamlist.Count());

	if (!CurTeam || !CurTeam->IsActive) CurTeam = NULL;

	if (CurTeam) {
		teamlist.Set_Selected_Index(CurTeam);
		CurTeam = teamlist.Current_Item();
	} else {
		teamlist.Set_Selected_Index(0);
		if (TeamTypes.Count()) {
			CurTeam = teamlist.Current_Item();
		}
	}

	/*
	**	Create the list
	*/
	commands = &teamlist;
	editbtn.Add_Tail(*commands);
	newbtn.Add_Tail(*commands);
	deletebtn.Add_Tail(*commands);
	okbtn.Add_Tail(*commands);

	/*
	**	Init tab stops for list
	*/
	teamlist.Set_Tabs(tabs);

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
			Hide_Mouse();
			Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
			Draw_Caption(TXT_TEAM_EDIT, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);
			commands->Draw_All();
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
			case (TEAM_LIST | KN_BUTTON):
				CurTeam = teamlist.Current_Item();
				break;

			case (BUTTON_EDIT | KN_BUTTON):
				if (teamlist.Count()) {
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
	**	Redraw the display
	*/
	HidPage.Clear();
	Flag_To_Redraw(true);
	Render();

	if (edit_team) return(1);
	if (new_team) return(2);
	if (del_team) return(3);
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
//#define TEENSY_WEENSY
/*
**	Dialog & button dimensions
*/
enum {
	D_DIALOG_W = 640,
	D_DIALOG_X = 0,
	D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

	D_TXT6_H = 7,
	D_MARGIN = 7,

#ifdef TEENSY_WEENSY
	D_PICTURE_W = 32,
	D_PICTURE_H = 24,
#else
	D_PICTURE_W = 64,
	D_PICTURE_H = 48,
#endif
	D_ROW_H = (D_PICTURE_H + 3),

	D_OK_W = 50,
	D_OK_H = 9,
	D_OK_X = D_DIALOG_CX - 5 - D_OK_W,
	D_OK_Y = 0,

	D_CANCEL_W = 50,
	D_CANCEL_H = 9,
	D_CANCEL_X = D_DIALOG_CX + 5,
	D_CANCEL_Y = 0,

};

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
	**	Dialog variables
	*/
	KeyNumType input;								// user input
	bool cancel = false;							// true = user cancels
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	/*
	**	Team display variables
	*/
	const TechnoTypeClass **teamclass;		// array of team classes
	int *teamcount;								// array of class counts
	int numcols;									// # units displayed horizontally
	int numrows;									// # units displayed vertically

	/*
	**	Dialog dimensions.
	*/
	int dlg_y;
	int dlg_h;										// dialog height
	int msg_y;										// y-coord for object names

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
	ControlClass * commands;

	TextButtonClass okbtn (BUTTON_OK, TXT_OK, TPF_CENTER | TPF_EFNT | TPF_NOSHADOW, D_OK_X, D_OK_Y, D_OK_W, D_OK_H);
	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL, TPF_CENTER | TPF_EFNT | TPF_NOSHADOW, D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	/*
	**	Set up the team data arrays (ObjectTypeClass pointers & count)
	*/
#ifdef WIN32
	teamclass = (const TechnoTypeClass **)SysMemPage.Get_Buffer();
	teamcount = (int *)SysMemPage.Get_Buffer() + MAX_TEAM_CLASSES * sizeof (ObjectTypeClass *);
#else
	teamclass = (const TechnoTypeClass **)HidPage.Get_Buffer();
	teamcount = (int *)HidPage.Get_Buffer() + MAX_TEAM_CLASSES * sizeof (ObjectTypeClass *);
#endif

	/*
	**	Fill in the ObjectTypeClass array with all available object type ptrs,
	**	checking to be sure this house can own the object
	*/
	i = 0;
	for (InfantryType i_id = INFANTRY_FIRST; i_id < INFANTRY_COUNT; i_id++) {
		teamclass[i] = &InfantryTypeClass::As_Reference(i_id);
		i++;
	}

	for (AircraftType a_id = AIRCRAFT_FIRST; a_id < AIRCRAFT_COUNT; a_id++) {
		teamclass[i] = &AircraftTypeClass::As_Reference(a_id);
		i++;
	}

	for (UnitType u_id = UNIT_FIRST; u_id < UNIT_COUNT; u_id++) {
		teamclass[i] = &UnitTypeClass::As_Reference(u_id);
		i++;
	}

	for (VesselType v_id = VESSEL_FIRST; v_id < VESSEL_COUNT; v_id++) {
		teamclass[i] = &VesselTypeClass::As_Reference(v_id);
		i++;
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
			if (CurTeam->Members[i].Class == teamclass[j]) {
				teamcount[j] = CurTeam->Members[i].Quantity;
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

	/*
	**	Dialog's height = top margin + label + picture rows + margin + label + margin + btn
	*/
	dlg_h = 400;
	dlg_y = 0;
	msg_y = dlg_y+dlg_h - 26 - 15;

	okbtn.Y = dlg_y + dlg_h - D_MARGIN - D_OK_H - 15;
	cancelbtn.Y = dlg_y + dlg_h - D_MARGIN - D_CANCEL_H - 15;

	/*
	**	Draw to SeenPage.
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Make sure 'house' is valid.
	*/
//	if (house!=HOUSE_GOOD && house!=HOUSE_BAD && house != HOUSE_MULTI1 &&
//		house != HOUSE_MULTI2 && house != HOUSE_MULTI3 && house != HOUSE_MULTI4 ) {
//		if (Scen.ScenPlayer == SCEN_PLAYER_MPLAYER) {
//			house = HOUSE_MULTI1;
//		} else {
//			house = HOUSE_GOOD;
//		}
//	}

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
				Draw_Caption(TXT_TEAM_MEMBERS, D_DIALOG_X, dlg_y, D_DIALOG_W);

				/*
				**	Draw the objects.
				*/
				for (i = 0; i < maxclasses; i++) {

					/*
					**	Display the object along with any count value for it.
					*/
					Draw_Member(teamclass[i], i, teamcount[i], house);
				}

				if ((unsigned)curclass < maxclasses) {
					Fancy_Text_Print(teamclass[curclass]->Full_Name(),
						D_DIALOG_X + D_DIALOG_W / 2, msg_y,
						&ColorRemaps[PCOLOR_BROWN], TBLACK,
						TPF_CENTER|TPF_EFNT|TPF_NOSHADOW);
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
				i = (Get_Mouse_X() - 32 - D_DIALOG_X) / D_PICTURE_W +
					((Get_Mouse_Y() - (dlg_y+8+11)) / D_ROW_H) * numcols;

				/*
				**	If it's changed, update class label.
				*/
				if (i != curclass) {

					curclass = i;

					/*
					**	Clear out the previously printed name of the item.
					*/
					Hide_Mouse();
					LogicPage->Fill_Rect(D_DIALOG_X + 32, msg_y, D_DIALOG_X + D_DIALOG_W - 64, msg_y + D_TXT6_H, BLACK);

					if ((unsigned)curclass < maxclasses) {
						Fancy_Text_Print(teamclass[curclass]->Full_Name(),
							D_DIALOG_X + D_DIALOG_W / 2, msg_y, scheme, TBLACK, TPF_CENTER|TPF_EFNT|TPF_NOSHADOW);
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
			if (TickCount - heldtime > tdelay[tindex]) {
				heldtime = TickCount;
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
				Draw_Member(teamclass[curclass], curclass, teamcount[curclass], house);
			}

		} else {

			if (rheld) {

				/*
				**	The first time in, TickCount - heldtime will be larger than
				**	tdelay[2], so we increment the count immediately; then, we decrement
				**	tindex to go to the next time delay, which is longer; then, decr.
				**	again to go to the 1st time delay which is the shortest.
				*/
				if (TickCount - heldtime > tdelay[tindex]) {
					if (tindex) {
						tindex--;
					}
					heldtime = TickCount;

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
					Draw_Member(teamclass[curclass], curclass, teamcount[curclass], house);
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
				CurTeam->Members[i].Quantity = teamcount[j];
				CurTeam->Members[i].Class = teamclass[j];
				i++;
			}
		}
	}

	/*
	**	Redraw the display.
	*/
	HidPage.Clear();
	Flag_To_Redraw(true);
	Render();

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
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void MapEditClass::Draw_Member(TechnoTypeClass const * ptr, int index, int quant, HousesType house)
{
	int numcols = (D_DIALOG_W - 64) / D_PICTURE_W;
	int col = index % numcols;
	int row = index / numcols;
	int dlg_y = 0;
	int x = D_DIALOG_X + 32 + col * D_PICTURE_W;
	int y = dlg_y + 8 + 13 + row * D_ROW_H;
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	/*
	**	Change the window to this box.
	*/
	WindowList[WINDOW_EDITOR][WINDOWX] = x;
	WindowList[WINDOW_EDITOR][WINDOWY] = y;
	WindowList[WINDOW_EDITOR][WINDOWWIDTH] = D_PICTURE_W;
	WindowList[WINDOW_EDITOR][WINDOWHEIGHT] = D_PICTURE_H;
	Change_Window((int)WINDOW_EDITOR);

	Hide_Mouse();
	Draw_Box(x, y, D_PICTURE_W, D_PICTURE_H, BOXSTYLE_DOWN, true);
	ptr->Display(WinW/2, WinH>>1, WINDOW_EDITOR, house);
	if (quant > 0) {
		Fancy_Text_Print("%d", x+1, y+1, scheme, TBLACK, TPF_8POINT|TPF_DROPSHADOW, quant);
//		Fancy_Text_Print("%d", x+1, y+D_PICTURE_H-8, scheme, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_DROPSHADOW, quant);
	}
	Show_Mouse();
}


#endif
