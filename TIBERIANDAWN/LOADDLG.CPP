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

/* $Header:   F:\projects\c&c\vcs\code\loaddlg.cpv   2.18   16 Oct 1995 16:51:18   JOE_BOSTIC  $ */
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
 *   LoadOptionsClass::Clear_List -- clears the list box & Files arrays                        *
 *   LoadOptionsClass::Compare -- for qsort                                                    *
 *   LoadOptionsClass::Fill_List -- fills the list box & GameNum arrays                        *
 *   LoadOptionsClass::LoadOptionsClass -- class constructor                                   *
 *   LoadOptionsClass::Num_From_Ext -- clears the list box & GameNum arrays                    *
 *   LoadOptionsClass::Process -- main processing routine                                      *
 *   LoadOptionsClass::~LoadOptionsClass -- class destructor                                   *
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
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	int d_dialog_w		= 250 *factor;
	int d_dialog_h		= 156 * factor;
	int d_dialog_x		= (SeenBuff.Get_Width() - d_dialog_w) >> 1;
	int d_dialog_y		= (SeenBuff.Get_Height() - d_dialog_h) >> 1;
	int d_dialog_cx	= d_dialog_x + (d_dialog_w >> 1);
	int d_txt8_h		= 11 * factor;
	int d_margin		= 7 * factor;

	int d_list_w 		= d_dialog_w - (d_margin * 2);
	int d_list_h 		= 104 * factor;
	int d_list_x 		= d_dialog_x + d_margin;
	int d_list_y 		= d_dialog_y + d_margin + d_txt8_h + d_margin;

	int d_edit_w 		= d_dialog_w - (d_margin * 2);
	int d_edit_h 		= 13 * factor;
	int d_edit_x 		= d_dialog_x + d_margin;
	int d_edit_y 		= d_list_y + d_list_h - (30 * factor) + d_margin + d_txt8_h;

#ifdef german
	int d_button_w 	= 50 * factor;
#else
	int d_button_w 	= 40 * factor;
#endif
	int d_button_h 	= 13 * factor;
	int d_button_x 	= d_dialog_cx - d_button_w - d_margin;
	int d_button_y 	= d_dialog_y + d_dialog_h - d_button_h - d_margin;

#ifdef german
	int d_cancel_w 	= 50 * factor;
#else
	int d_cancel_w 	= 40 * factor;
#endif
	int d_cancel_h 	= 13 * factor;
	int d_cancel_x 	= d_dialog_cx + d_margin;
	int d_cancel_y 	= d_dialog_y + d_dialog_h - d_cancel_h - d_margin;
#if(0)
	enum {
		D_DIALOG_W = 250,											// dialog width
		D_DIALOG_H = 156,											// dialog height
		D_DIALOG_X = ((320 - D_DIALOG_W) / 2),				// centered x-coord
		D_DIALOG_Y = ((200 - D_DIALOG_H) / 2),				// centered y-coord
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),		// coord of x-center

		D_TXT8_H = 11,												// ht of 8-pt text
		D_MARGIN = 7,												// margin width/height

		D_LIST_W = D_DIALOG_W - (D_MARGIN * 2),
		D_LIST_H = 104,
		D_LIST_X = D_DIALOG_X + D_MARGIN,
		D_LIST_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H + D_MARGIN,

		D_EDIT_W = D_DIALOG_W - (D_MARGIN * 2),
		D_EDIT_H = 13,
		D_EDIT_X = D_DIALOG_X + D_MARGIN,
		D_EDIT_Y = D_LIST_Y + D_LIST_H - 30 + D_MARGIN + D_TXT8_H,

#if (GERMAN | FRENCH)
		D_BUTTON_W = 50,
#else
		D_BUTTON_W = 40,
#endif
		D_BUTTON_H = 13,
		D_BUTTON_X = D_DIALOG_CX - D_BUTTON_W - D_MARGIN,
		D_BUTTON_Y = D_DIALOG_Y + D_DIALOG_H - D_BUTTON_H - D_MARGIN,

#if (GERMAN | FRENCH)
		D_CANCEL_W = 50,//BG:40
#else
		D_CANCEL_W = 40,
#endif
		D_CANCEL_H = 13,
		D_CANCEL_X = D_DIALOG_CX + D_MARGIN,
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_CANCEL_H - D_MARGIN,
	};
#endif
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
	char game_descr[40] = {0};				// save-game description
	char fname[13];							// for generating filename to delete

	void const *up_button;
	void const *down_button;

	if (InMainLoop){
		up_button = Hires_Retrieve("BTN-UP.SHP");
		down_button = Hires_Retrieve("BTN-DN.SHP");
	}else{
		up_button = Hires_Retrieve("BTN-UP2.SHP");
		down_button = Hires_Retrieve("BTN-DN2.SHP");
	}

	/*
	**	Buttons
	*/
	ControlClass *commands = NULL;		// the button list

 	if (Style == LOAD) {
		btn_txt = TXT_LOAD_BUTTON;
		btn_id = BUTTON_LOAD;
		caption = TXT_LOAD_MISSION;
	} else {
		if (Style == SAVE) {
			btn_txt = TXT_SAVE_BUTTON;
			btn_id = BUTTON_SAVE;
			caption = TXT_SAVE_MISSION;
			list_ht -= 30;
		} else {
			btn_txt = TXT_DELETE_BUTTON;
			btn_id = BUTTON_DELETE;
			caption = TXT_DELETE_MISSION;
		}
	}

	TextButtonClass button (btn_id, btn_txt, TPF_6PT_GRAD|TPF_CENTER|TPF_NOSHADOW,
		d_button_x, d_button_y, d_button_w);

	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL, TPF_6PT_GRAD|TPF_CENTER|TPF_NOSHADOW,
		d_cancel_x, d_cancel_y, d_cancel_w);

	ListClass listbtn (BUTTON_LIST, d_list_x, d_list_y, d_list_w, list_ht,
		TPF_6PT_GRAD | TPF_NOSHADOW,
		up_button,
		down_button);

	EditClass editbtn (BUTTON_EDIT, game_descr, 40, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW,
		d_edit_x, d_edit_y, d_edit_w, -1, EditClass::ALPHANUMERIC);

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
		CCMessageBox().Process(TXT_NO_SAVES);
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
	bool firsttime = true;
	bool display = true;
	bool process = true;
	while (process) {

		/*
		**	Invoke game callback.
		*/
		if (GameToPlay == GAME_NORMAL) {
			Call_Back();
		} else {
			if (Main_Loop()) {
				process = false;
				cancel = true;
			}
		}

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=TRUE;
		}

		/*
		**	Refresh display if needed.
		*/
		if (display) {

			Hide_Mouse();
			/*
			**	Redraw the map.
			*/
			if (InMainLoop){
				HiddenPage.Clear();
				Map.Flag_To_Redraw(true);
				Map.Render();
			}else{
				HiddenPage.Clear();
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
			}


			/*
			**	Display the dialog box.
			*/
			if (display) {
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);
				Draw_Caption(caption, d_dialog_x, d_dialog_y, d_dialog_w);

				if (Style == SAVE) {
					Fancy_Text_Print(TXT_MISSION_DESCRIPTION, d_dialog_cx,
						d_edit_y - d_txt8_h, CC_GREEN, TBLACK, TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_CENTER | TPF_NOSHADOW);
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
		if (input == KN_RETURN) {
			switch (Style) {
				case SAVE:
					input = (KeyNumType)(BUTTON_SAVE|KN_BUTTON);
					break;

				case LOAD:
					input = (KeyNumType)(BUTTON_LOAD|KN_BUTTON);
					break;

				case WWDELETE:
					input = (KeyNumType)(BUTTON_DELETE|KN_BUTTON);
					break;
			}
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
					CCMessageBox().Process(TXT_LOADING, TXT_NONE);
					if (!Load_Game(game_num)) {
						CCMessageBox().Process(TXT_ERROR_LOADING_GAME);
					} else {
						Hide_Mouse();
						VisiblePage.Clear();
						Set_Palette(GamePalette);
						Show_Mouse();
						process = false;
					}
				} else {
					CCMessageBox().Process(TXT_OBSOLETE_SAVEGAME);
				}
				break;

			/*
			** Save: Save the game & exit the dialog
			*/
			case (BUTTON_SAVE | KN_BUTTON):
				if (!strlen(game_descr)) {
					CCMessageBox().Process(TXT_MUSTENTER_DESCRIPTION);
					firsttime = true;
					display = true;
					break;
				}
				game_idx = listbtn.Current_Index();
				if (Disk_Space_Available() < SAVE_GAME_DISK_SPACE && game_idx == 0) {
//					CCMessageBox().Process("Insuficent disk space to save a game.  Please delete a previous save to free up some disk space and try again.");
					CCMessageBox().Process(TXT_SPACE_CANT_SAVE);
					firsttime = true;
					display = true;
					break;
				}

				game_num = Files[game_idx]->Num;
				if (!Save_Game(game_num,game_descr)) {
					CCMessageBox().Process(TXT_ERROR_SAVING_GAME);
				} else {
					CCMessageBox().Process(TXT_GAME_WAS_SAVED, TXT_NONE, TXT_NONE);
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
				if (CCMessageBox().Process(TXT_DELETE_FILE_QUERY,TXT_YES,TXT_NO)==0) {
					sprintf(fname,"SAVEGAME.%03d",game_num);
					unlink(fname);
					Clear_List(&listbtn);
					Fill_List(&listbtn);
					if (listbtn.Count() == 0) {
						process = false;
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
						strcpy(game_descr,listbtn.Get_Item(game_idx));
					} else {
						game_descr[0] = 0;
					}
					editbtn.Set_Text(game_descr,40);
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
void LoadOptionsClass::Clear_List(ListClass *list)
{
	/*
	** For every item in the list, free its buffer & remove it from the list.
	*/
	int j = list->Count();
	int i;
	for (i = 0; i < j; i++) {
		list->Remove_Item(list->Get_Item(0));
	}

	/*
	** Clear the array of game numbers
	*/
	for (i = 0; i < Files.Count(); i++) {
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
void LoadOptionsClass::Fill_List(ListClass *list)
{
//PG_TO_FIX
#if (0)
	FileEntryClass *fdata;	// for adding entries to 'Files'
	char descr[DESCRIP_MAX];
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
		strcpy(fdata->Descr,Text_String(TXT_EMPTY_SLOT));
		fdata->DateTime = 0xffffffff;	// will always be first
		Files.Add(fdata);
	}

	/*
	** Find all savegame files
	*/
	int rc = _dos_findfirst("SAVEGAME.*", _A_NORMAL, &ff);

	while (!rc) {
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
		if (!ok) strcpy(fdata->Descr, Text_String(TXT_OLD_GAME));
		strncat(fdata->Descr, descr, (sizeof(fdata->Descr)-strlen(fdata->Descr))-1);
		fdata->Valid = ok;
		fdata->Scenario = scenario;
		fdata->House = house;
		fdata->Num = id;
		fdata->DateTime = (((unsigned long)ff.wr_date) << 16) | (unsigned long)ff.wr_time;
		Files.Add(fdata);

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
int LoadOptionsClass::Num_From_Ext(char *fname)
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
int LoadOptionsClass::Compare(const void *p1, const void *p2)
{
	class FileEntryClass *fe1,*fe2;

	fe1 = *((class FileEntryClass **)p1);
	fe2 = *((class FileEntryClass **)p2);

	if (fe1->DateTime > fe2->DateTime) return(-1);
	if (fe1->DateTime < fe2->DateTime) return(1);
	return(0);
}