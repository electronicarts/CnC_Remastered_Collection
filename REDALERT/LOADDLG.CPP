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

/* $Header: /CounterStrike/LOADDLG.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LOADDLG.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Maria Legg, Joe Bostic, Bill Randolph                        *
 *                                                                                             *
 *                   Start Date : March 19, 1995                                               *
 *                                                                                             *
 *                  Last Update : June 25, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   LoadOptionsClass::LoadOptionsClass -- class constructor                                   *
 *   LoadOptionsClass::~LoadOptionsClass -- class destructor                                   *
 *   LoadOptionsClass::Process -- main processing routine                                      *
 *   LoadOptionsClass::Clear_List -- clears the list box & Files arrays                        *
 *   LoadOptionsClass::Fill_List -- fills the list box & GameNum arrays                        *
 *   LoadOptionsClass::Num_From_Ext -- clears the list box & GameNum arrays                    *
 *   LoadOptionsClass::Compare -- for qsort                                                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include <io.h>				// for unlink


/***********************************************************************************************
 * LoadOptionsClass::LoadOptionsClass -- class constructor                                     *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      style      style for this load/save dialog (LOAD/SAVE/DELETE)                          *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
LoadOptionsClass::LoadOptionsClass(LoadStyleType style)
{
	Style = style;
	Files.Clear();
}


/***********************************************************************************************
 * LoadOptionsClass::~LoadOptionsClass -- class destructor                                     *
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
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
LoadOptionsClass::~LoadOptionsClass()
{
	for (int i = 0; i < Files.Count(); i++) {
		delete Files[i];
	}
	Files.Clear();
}


/***********************************************************************************************
 * LoadOptionsClass::Process -- main processing routine                                        *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      false = User cancelled, true = operation completed                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
int LoadOptionsClass::Process(void)
{
	/*
	**	Dialog & button dimensions
	*/
	int d_dialog_w = 250 * RESFACTOR;											// dialog width
	int d_dialog_h = 156 * RESFACTOR;											// dialog height
	int d_dialog_x = (((320 * RESFACTOR) - d_dialog_w) / 2);				// centered x-coord
	int d_dialog_y = (((200 * RESFACTOR) - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// coord of x-center

	int d_txt8_h = 11 * RESFACTOR;												// ht of 8-pt text
	int d_margin = 7 * RESFACTOR;												// margin width/height
	int x_margin = 16 * RESFACTOR;												// margin width/height

	int d_list_w = d_dialog_w - (x_margin * 2);
	int d_list_h = 104 * RESFACTOR;
	int d_list_x = d_dialog_x + x_margin;
	int d_list_y = d_dialog_y + d_margin + d_txt8_h + d_margin;

	int d_edit_w = d_dialog_w - (x_margin * 2);
	int d_edit_h = 13 * RESFACTOR;
	int d_edit_x = d_dialog_x + x_margin;
	int d_edit_y = d_list_y + d_list_h - (30 * RESFACTOR) + d_margin + d_txt8_h;

#if (GERMAN | FRENCH)
	int d_button_w = 50 * RESFACTOR;
#else
	int d_button_w = 40 * RESFACTOR;
#endif
	int d_button_h = 13 * RESFACTOR;
	int d_button_x = d_dialog_cx - d_button_w - d_margin;
	int d_button_y = d_dialog_y + d_dialog_h - d_button_h - d_margin;

#if defined(GERMAN) || defined(FRENCH)
	int d_cancel_w = 60 * RESFACTOR;//BG:40
#else
	int d_cancel_w = 40 * RESFACTOR;
#endif
	int d_cancel_h = 13 * RESFACTOR;
	int d_cancel_x = d_dialog_cx + d_margin;
	int d_cancel_y = d_dialog_y + d_dialog_h - d_cancel_h - d_margin;

	/*
	**	Button enumerations
	*/
	enum {
		BUTTON_LOAD = 100,
		BUTTON_SAVE,
		BUTTON_DELETE,
		BUTTON_CANCEL,
		BUTTON_LIST,
		BUTTON_EDIT,
	};

	/*
	**	Redraw values: in order from "top" to "bottom" layer of the dialog
	*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*
	**	Dialog variables
	*/
	bool cancel = false;						// true = user cancels
	int list_ht = d_list_h;					// adjusted list box height

	/*
	**	Other Variables
	*/
	int btn_txt;								// text on the 'OK' button
	int btn_id;									// ID of 'OK' button
	int caption;								// dialog caption
	int game_idx = 0;							// index of game to save/load/etc
	int game_num = 0;							// file number of game to load/save/etc
	char game_descr[DESCRIP_MAX] = {0};				// save-game description
	char fname[_MAX_NAME+_MAX_EXT];							// for generating filename to delete
	int rc;										// return code

	/*
	**	Buttons
	*/
	ControlClass * commands = NULL;		// the button list

	switch (Style) {
		case LOAD:
			btn_txt = TXT_LOAD_BUTTON;
			btn_id = BUTTON_LOAD;
			caption = TXT_LOAD_MISSION;
			break;

		case SAVE:
			btn_txt = TXT_SAVE_BUTTON;
			btn_id = BUTTON_SAVE;
			caption = TXT_SAVE_MISSION;
			list_ht -= 30;
			break;

		default:
			btn_txt = TXT_DELETE_BUTTON;
			btn_id = BUTTON_DELETE;
			caption = TXT_DELETE_MISSION;
			break;
	}

	TextButtonClass button (btn_id, btn_txt, TPF_BUTTON, d_button_x, d_button_y, d_button_w);
	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w);

	ListClass listbtn (BUTTON_LIST, d_list_x, d_list_y, d_list_w, list_ht,
		TPF_6PT_GRAD | TPF_NOSHADOW,
		MFCD::Retrieve("BTN-UP.SHP"),
		MFCD::Retrieve("BTN-DN.SHP"));

	EditClass editbtn (BUTTON_EDIT, game_descr, sizeof(game_descr)-4, TPF_6PT_GRAD|TPF_NOSHADOW, d_edit_x, d_edit_y, d_edit_w, -1, EditClass::ALPHANUMERIC);

	/*
	**	Initialize.
	*/
	Set_Logic_Page(SeenBuff);

	Fill_List(&listbtn);

	/*
	**	Do nothing if list is empty.
	*/
	if ((Style == LOAD || Style == WWDELETE) && listbtn.Count()==0) {
		Clear_List(&listbtn);
		WWMessageBox().Process(TXT_NO_SAVES);
		return(false);
	}

	/*
	**	Create the button list.
	*/
	commands = &button;
	cancelbtn.Add_Tail(*commands);
	listbtn.Add_Tail(*commands);
	if (Style == SAVE) {
		editbtn.Add_Tail(*commands);
		editbtn.Set_Focus();
	}

	/*
	**	Main Processing Loop.
	*/
	Keyboard->Clear();
	bool firsttime = true;
	bool display = true;
	bool process = true;
	while (process) {

		/*
		**	Invoke game callback.
		*/
		if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
			Call_Back();
		} else {
			if (Main_Loop()) {
				process = false;
				cancel = true;
			}
		}

		#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = true;
		}
		#endif

		/*
		**	Refresh display if needed.
		*/
		if (display) {

			/*
			**	Display the dialog box.
			*/
			Hide_Mouse();
			if (display) {
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);
				Draw_Caption(caption, d_dialog_x, d_dialog_y, d_dialog_w);

				if (Style == SAVE) {
					Fancy_Text_Print(TXT_MISSION_DESCRIPTION, d_dialog_cx,
						d_edit_y - d_txt8_h, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_TEXT | TPF_CENTER);
				}
			}

			/*
			**	Redraw the buttons.
			*/
			if (display) {
				commands->Flag_List_To_Redraw();
			}
			Show_Mouse();
			display = false;
		}

		/*
		**	Get user input.
		*/
		KeyNumType input = commands->Input();

		/*
		**	The first time through the processing loop, set the edit
		**	gadget to have the focus if this is the save dialog. The
		**	focus must be set here since the gadget list has changed
		**	and this change will cause any previous focus setting to be
		**	cleared by the input processing routine.
		*/
		if (firsttime && Style == SAVE) {
			firsttime = false;
			editbtn.Set_Focus();
			editbtn.Flag_To_Redraw();
		}

		/*
		**	If the <RETURN> key was pressed, then default to the appropriate
		**	action button according to the style of this dialog box.
		*/
		if (input == KN_RETURN || input == (BUTTON_EDIT|KN_BUTTON)) {
			ToggleClass * toggle = NULL;
			switch (Style) {
				case SAVE:
					input = (KeyNumType)(BUTTON_SAVE|KN_BUTTON);
					cancelbtn.Turn_Off();
//					cancelbtn.IsOn = false;
					toggle = (ToggleClass*)commands->Extract_Gadget(BUTTON_SAVE);
					if (toggle != NULL) {
						toggle->Turn_On();
//						toggle->IsOn = true;
						toggle->IsPressed = true;
					}
					break;

				case LOAD:
					input = (KeyNumType)(BUTTON_LOAD|KN_BUTTON);
//					cancelbtn.IsOn = false;
					cancelbtn.Turn_Off();
					toggle = (ToggleClass *)commands->Extract_Gadget(BUTTON_LOAD);
					if (toggle != NULL) {
						toggle->IsOn = true;
						toggle->IsPressed = true;
					}
					break;

				case WWDELETE:
					input = (KeyNumType)(BUTTON_DELETE|KN_BUTTON);
//					cancelbtn.IsOn = false;
					cancelbtn.Turn_Off();
					toggle = (ToggleClass *)commands->Extract_Gadget(BUTTON_DELETE);
					if (toggle != NULL) {
						toggle->IsOn = true;
						toggle->IsPressed = true;
					}
					break;
			}
			Hide_Mouse();
			commands->Draw_All(true);
			Show_Mouse();
		}

		/*
		**	Process input.
		*/
		switch (input) {
			/*
			** Load: if load fails, present a message, and stay in the dialog
			** to allow the user to try another game
			*/
			case (BUTTON_LOAD | KN_BUTTON):
				game_idx = listbtn.Current_Index();
				game_num = Files[game_idx]->Num;
				if (Files[game_idx]->Valid) {

					/*
					** Start a timer before we load the game
					*/
					CDTimerClass<SystemTimerClass> timer;
//					timer.Start();
					timer = TICKS_PER_SECOND*4;

					WWMessageBox().Process(TXT_LOADING, TXT_NONE);
					Theme.Fade_Out();
					rc = Load_Game(game_num);

					/*
					** Make sure the message says on the screen at least 1 second
					*/
					while (timer > 0) {
						Call_Back();
					}
					Keyboard->Clear();

					if (!rc) {
						WWMessageBox().Process(TXT_ERROR_LOADING_GAME);
					} else {
						Speak(VOX_LOAD1);
						while (Is_Speaking()) {
							Call_Back();
						}
						Hide_Mouse();
						SeenPage.Clear();
						GamePalette.Set();
//						Set_Palette(GamePalette);
						Show_Mouse();
						process = false;
					}
				} else {
					WWMessageBox().Process(TXT_OBSOLETE_SAVEGAME);
				}
				break;

			/*
			** Save: Save the game & exit the dialog
			*/
			case (BUTTON_EDIT | KN_BUTTON):

			case (BUTTON_SAVE | KN_BUTTON):
				if (!strlen(game_descr)) {
					WWMessageBox().Process(TXT_MUSTENTER_DESCRIPTION);
					firsttime = true;
					display = true;
					break;
				}
				game_idx = listbtn.Current_Index();
				if (Disk_Space_Available() < SAVE_GAME_DISK_SPACE && game_idx == 0) {
					WWMessageBox().Process(TXT_SPACE_CANT_SAVE);
					firsttime = true;
					display = true;
					break;
				}

				game_num = Files[game_idx]->Num;
				if (!Save_Game(game_num, game_descr)) {
					WWMessageBox().Process(TXT_ERROR_SAVING_GAME);
				} else {
					Speak(VOX_SAVE1);
					while (Is_Speaking()) {
						Call_Back();
					}
					CDTimerClass<SystemTimerClass> timer;
//					timer.Start();
					timer = TICKS_PER_SECOND*4;

					WWMessageBox().Process(TXT_GAME_WAS_SAVED, TXT_NONE, TXT_NONE);

					/*
					**	Delay to let the user read the message
					*/
					while (timer > 0) {
						Call_Back();
					}
					Keyboard->Clear();
				}
				process = false;
				break;

			/*
			** Delete: delete the file & stay in the dialog, to allow the user
			** to delete multiple files.
			*/
			case (BUTTON_DELETE | KN_BUTTON):
				game_idx = listbtn.Current_Index();
				game_num = Files[game_idx]->Num;
				if (WWMessageBox().Process(TXT_DELETE_FILE_QUERY, TXT_YES, TXT_NO)==0) {
					sprintf(fname, "SAVEGAME.%03d", game_num);
					unlink(fname);
					Clear_List(&listbtn);
					Fill_List(&listbtn);
					if (listbtn.Count() == 0) {
						process = false;
					} else {
						ToggleClass * toggle = (ToggleClass *)commands->Extract_Gadget(BUTTON_DELETE);
						if (toggle != NULL) {
//							toggle->IsOn = false;
							toggle->Turn_Off();
							toggle->IsPressed = false;
							toggle->Flag_To_Redraw();
						}
					}
				}
				display = true;
				break;

			/*
			** If the user clicks on the list, see if the there is a new current
			** item; if so, and if we're in SAVE mode, copy the list item into
			** the save-game description field.
			*/
			case (BUTTON_LIST | KN_BUTTON):
				if (Style != SAVE) {
					break;
				}

				if (listbtn.Count() && listbtn.Current_Index() != game_idx) {
					game_idx = listbtn.Current_Index();

					/*
					** Copy the game's description, UNLESS it's the empty slot; if
					** it is, set the edit buffer to empty.
					*/
					if (game_idx != 0) {
						strcpy(game_descr, listbtn.Get_Item(game_idx));

						/*
						**	Strip any leading parenthesis off of the description.
						*/
						if (game_descr[0] == '(') {
							char * ptr = strchr(game_descr, ')');
							if (ptr != NULL) {
								strcpy(game_descr, ptr+1);
								strtrim(game_descr);
							}
						}

					} else {
						game_descr[0] = 0;
					}
					editbtn.Set_Text(game_descr, 40);
				}
				break;

			/*
			** ESC/Cancel: break
			*/
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				cancel = true;
				process = false;
				break;

			default:
				break;
		}
	}

	Clear_List(&listbtn);

	if (cancel) return(false);

	return(true);
}


/***********************************************************************************************
 * LoadOptionsClass::Clear_List -- clears the list box & Files arrays                          *
 *                                                                                             *
 * This step is essential, because it frees all the strings allocated for list items.          *
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
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void LoadOptionsClass::Clear_List(ListClass * list)
{
	/*
	** For every item in the list, free its buffer & remove it from the list.
	*/
	int j = list->Count();
	for (int i = 0; i < j; i++) {
		list->Remove_Item(list->Get_Item(0));
	}

	/*
	** Clear the array of game numbers
	*/
	for (int i = 0; i < Files.Count(); i++) {
		delete Files[i];
	}
	Files.Clear();
}


/***********************************************************************************************
 * LoadOptionsClass::Fill_List -- fills the list box & GameNum arrays                          *
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
 *   02/14/1995 BR : Created.                                                                  *
 *   06/25/1995 JLB : Shows which saved games are "(old)".                                     *
 *=============================================================================================*/
void LoadOptionsClass::Fill_List(ListClass * list)
{
#if(0)		//PG
	FileEntryClass * fdata;	// for adding entries to 'Files'
	char descr[DESCRIP_MAX+32];
	unsigned scenario;			// scenario #
	HousesType house;				// house
	struct find_t ff;		// for _dos_findfirst
	int id;

	/*
	** Make sure the list is empty
	*/
	Clear_List(list);

	/*
	** Add the Empty Slot entry
	*/
	if (Style == SAVE) {
		fdata = new FileEntryClass;
		strcpy(fdata->Descr, Text_String(TXT_EMPTY_SLOT));
		fdata->DateTime = 0xffffffff;	// will always be first
		Files.Add(fdata);
	}

	/*
	** Find all savegame files
	*/
	int rc = _dos_findfirst("SAVEGAME.*", _A_NORMAL, &ff);

	while (!rc) {

		if (stricmp(ff.name, NET_SAVE_FILE_NAME) != 0) {

			/*
			** Extract the game ID from the filename
			*/
			id = Num_From_Ext(ff.name);

			/*
			** get the game's info; if success, add it to the list
			*/
			bool ok = Get_Savefile_Info(id, descr, &scenario, &house);

			fdata = new FileEntryClass;

			fdata->Descr[0] = '\0';
			if (!ok) {
				strcpy(fdata->Descr, Text_String(TXT_OLD_GAME));
			} else {
				if (house == HOUSE_USSR || house == HOUSE_UKRAINE) {
#ifdef WIN32
					sprintf(fdata->Descr, "(%s) ", Text_String(TXT_SOVIET));
#else
					sprintf(fdata->Descr, "(%c) ", *Text_String(TXT_SOVIET));
#endif
				} else {
#ifdef WIN32
					sprintf(fdata->Descr, "(%s) ", Text_String(TXT_ALLIES));
#else
					sprintf(fdata->Descr, "(%c) ", *Text_String(TXT_ALLIES));
#endif
				}
			}
			strncat(fdata->Descr, descr, (sizeof(fdata->Descr)-strlen(fdata->Descr))-1);
			fdata->Valid = ok;
			fdata->Scenario = scenario;
			fdata->House = house;
			fdata->Num = id;
			fdata->DateTime = (((unsigned long)ff.wr_date) << 16) | (unsigned long)ff.wr_time;
			Files.Add(fdata);
		}

		/*
		** Find the next file
		*/
		rc = _dos_findnext(&ff);
	}

	/*
	** If saving a game, determine a unique file ID for the empty slot
	*/
	if (Style == SAVE) {
		/*
		** Find an un-used number to associate with the Empty Slot by looking in
		** GameNum for each number from 0 to 'N', where 'N' is the # of entries
		** in the list; if any number isn't found, use that number; otherwise,
		** use 'N + 1'.
		*/
		for (int i = 0; i < Files.Count(); i++) {		// i = the # we're searching for
			id = -1;											// mark as 'not found'
			for (int j = 0; j < Files.Count(); j++) {	// loop through all game ID's
				if (Files[j]->Num==i) {					// if found, mark as found
					id = j;
					break;
				}
			}
			if (id == -1)	break;							// if ID not found, use this one
		}

		Files[0]->Num = i;								// set the empty slot's ID
	}

	/*
	** Now sort the list in order of Date/Time (newest first, oldest last)
	*/
	qsort((void *)(&Files[0]), Files.Count(), sizeof(class FileEntryClass *), LoadOptionsClass::Compare);

	/*
	** Now add every file's name to the list box
	*/
	for (int i = 0; i < Files.Count(); i++) {
		list->Add_Item(Files[i]->Descr);
	}
#endif
}


/***********************************************************************************************
 * LoadOptionsClass::Num_From_Ext -- clears the list box & GameNum arrays                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      fname      filename to parse                                                           *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      File number for this name.                                                             *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
int LoadOptionsClass::Num_From_Ext(char * fname)
{
	char ext[_MAX_EXT];

	_splitpath(fname, NULL, NULL, NULL, ext);
	int num = atoi(ext + 1);		// skip the '.'
	return(num);
}


/***********************************************************************************************
 * LoadOptionsClass::Compare -- for qsort                                                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      p1,p2      ptrs to elements to compare                                                 *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      0 = same, -1 = (*p1) goes BEFORE (*p2), 1 = (*p1) goes AFTER (*p2)                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
int LoadOptionsClass::Compare(const void * p1, const void * p2)
{
	class FileEntryClass * fe1, * fe2;

	fe1 = *((class FileEntryClass **)p1);
	fe2 = *((class FileEntryClass **)p2);

	if (fe1->DateTime > fe2->DateTime) return(-1);
	if (fe1->DateTime < fe2->DateTime) return(1);
	return(0);
}

