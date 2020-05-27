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

/* $Header:   F:\projects\c&c\vcs\code\descdlg.cpv   2.17   16 Oct 1995 16:49:44   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DESCDLG.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Maria del Mar McCready Legg                                  *
 *                                  Joe L. Bostic                                              *
 *                                                                                             *
 *                   Start Date : Jan 26, 1995                                                 *
 *                                                                                             *
 *                  Last Update : Jan 26, 1995   [MML]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   DescriptionClass::Process -- Handles all the options graphic interface.                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"descdlg.h"


/***********************************************************************************************
 * DescriptionClass::Process -- Handles all the options graphic interface.                     *
 *                                                                                             *
 *    This dialog uses an edit box to "fill-out" a description.                                *
 *                                                                                             *
 * INPUT:      char *string - return answer here.                                              *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    12/31/1994 MML : Created.                                                       *
 *=============================================================================================*/
void DescriptionClass::Process(char *string)
{
	/*
	-----------------------------------------------------------------
	Set up the window.  Window x-coords are in bytes not pixels.
	-----------------------------------------------------------------
	*/
	Set_Window(WINDOW_EDITOR, OPTION_X, OPTION_Y, OPTION_WIDTH, OPTION_HEIGHT);
	Set_Logic_Page(SeenBuff);

	/*
	-----------------------------------------------------------------------
	Create Buttons.  Button coords are in pixels, but are window-relative.
	-----------------------------------------------------------------------
	*/
	TextButtonClass optionsbtn(
 		BUTTON_OPTIONS,
 		TXT_OK,
 		TPF_6PT_GRAD,
 		0,
 		BUTTON_Y);

	TextButtonClass cancelbtn(
 		BUTTON_CANCEL,
 		TXT_CANCEL,
 		TPF_6PT_GRAD,
 		0,
 		BUTTON_Y);

	cancelbtn.X  = OPTION_X + ((OPTION_WIDTH - optionsbtn.Width)/3)*2;
	optionsbtn.X = OPTION_X + ((OPTION_WIDTH - optionsbtn.Width)/3);
	optionsbtn.Add_Tail(cancelbtn);

	EditClass edit(
		BUTTON_EDIT,
		string,
		31,
		TPF_6PT_GRAD,
		0,
		EDIT_Y
		,EDIT_W);

	edit.Set_Focus();
	edit.X = OPTION_X + (OPTION_WIDTH - edit.Width)/2,
	optionsbtn.Add_Tail(edit);

	/*
	**	This causes left mouse button clicking within the confines of the dialog to
	**	be ignored if it wasn't recognized by any other button or slider.
	*/
	GadgetClass dialog(OPTION_X, OPTION_Y, OPTION_WIDTH, OPTION_HEIGHT, GadgetClass::LEFTPRESS);
	optionsbtn.Add_Tail(dialog);

	/*
	**	This causes a right click anywhere or a left click outside the dialog region
	**	to be equivalent to clicking on the return to options dialog.
	*/
	ControlClass background(BUTTON_OPTIONS, 0, 0, SeenBuff.Get_Width(), SeenBuff.Get_Height(), GadgetClass::LEFTPRESS|GadgetClass::RIGHTPRESS);
	optionsbtn.Add_Tail(background);

	/*
	------------------- Main Processing Loop --------------------
	*/
	bool display = true;
	bool process = true;
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=TRUE;
		}

		/*
		-------------- Invoke game callback -------------
		*/
		Call_Back();

		/*
		-------------- Refresh display if needed --------------
		*/
		if (display) {

			Window_Hide_Mouse(WINDOW_EDITOR);

			/*
			--------- Draw the background -----------
			*/
			Window_Box (WINDOW_EDITOR, BOXSTYLE_GREEN_BORDER); // has border, raised up
			Draw_Caption(TXT_MISSION_DESCRIPTION, OPTION_X, OPTION_Y, OPTION_WIDTH);

			/*
			--------- Draw the titles -----------
			*/
			optionsbtn.Draw_All();
			Window_Show_Mouse();
			display = false;
		}

		/*
		-------------- Get user input ---------------
		*/
		KeyNumType input = optionsbtn.Input();

		/*
		-------------- Process Input ----------------
		*/
		switch (input) {

			case KN_RETURN:
			case BUTTON_OPTIONS|KN_BUTTON:
				strtrim(string);
				process = false;
				break;

			case KN_ESC:
			case BUTTON_CANCEL|KN_BUTTON:
				string[0]= NULL;
				strtrim(string);
				process = false;
				break;

			case BUTTON_EDIT|KN_BUTTON:
				break;
		}
	}
}

