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

/* $Header:   F:\projects\c&c\vcs\code\gamedlg.cpv   2.17   16 Oct 1995 16:52:02   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : GAMEDLG.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Maria del Mar McCready Legg, Joe L. Bostic                   *
 *                                                                                             *
 *                   Start Date : Jan 8, 1995                                                  *
 *                                                                                             *
 *                  Last Update : Jan 18, 1995   [MML]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   OptionsClass::Process -- Handles all the options graphic interface.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"gamedlg.h"
#include "sounddlg.h"
#include "visudlg.h"


/***********************************************************************************************
 * OptionsClass::Process -- Handles all the options graphic interface.                         *
 *                                                                                             *
 *    This routine is the main control for the visual representation of the options            *
 *    screen. It handles the visual overlay and the player input.                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 * OUTPUT:  none                                                                               *
 * WARNINGS:   none                                                                            *
 * HISTORY:                                                                                    *
 *   12/31/1994 MML : Created.                                                                 *
 *=============================================================================================*/
void GameControlsClass::Process(void)
{
	int factor = (SeenBuff.Get_Width() == 320) ? 1 : 2;

	/*
	**	Dialog & button dimensions
	*/
	int	d_dialog_w	= 232 * factor;								// dialog width
	int	d_dialog_h	= 141 * factor;								// dialog height
	int	d_dialog_x	= ((SeenBuff.Get_Width()- d_dialog_w) / 2);				// dialog x-coord
	int	d_dialog_y	= ((SeenBuff.Get_Height() - d_dialog_h) / 2);				// centered y-coord
	int	d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord
	int	d_top_margin= 30 * factor;

	int	d_txt6_h		= 7 * factor;									// ht of 6-pt text
	int	d_margin1	= 5 * factor;									// large margin
	int	d_margin2	= 2 * factor;									// small margin

	int	d_speed_w	= d_dialog_w - (20 * factor);
	int	d_speed_h	= 6 * factor;
	int	d_speed_x	= d_dialog_x + (10 * factor);
	int	d_speed_y	= d_dialog_y + d_top_margin + d_margin1 + d_txt6_h;

	int	d_scroll_w	= d_dialog_w - (20 * factor);
	int	d_scroll_h	= 6 * factor;
	int	d_scroll_x	= d_dialog_x + (10 * factor);
	int	d_scroll_y	= d_speed_y + d_speed_h + d_txt6_h + (d_margin1 * 2) + d_txt6_h;

	int	d_visual_w 	= d_dialog_w - (40 * factor);
	int	d_visual_h 	= 9 * factor;
	int	d_visual_x 	= d_dialog_x + (20 * factor);
	int	d_visual_y 	= d_scroll_y + d_scroll_h + d_txt6_h + (d_margin1 * 2);

	int	d_sound_w 	= d_dialog_w - (40 * factor);
	int	d_sound_h 	= 9 * factor;
	int	d_sound_x 	= d_dialog_x + (20 * factor);
	int	d_sound_y 	= d_visual_y + d_visual_h + d_margin1;

	int	d_ok_w 		= 20 * factor;
	int	d_ok_h 		= 9 * factor;
	int	d_ok_x 		= d_dialog_cx - (d_ok_w / 2);
	int	d_ok_y 		= d_dialog_y + d_dialog_h - d_ok_h - d_margin1;

	/*
	**	Button Enumerations
	*/
	enum {
		BUTTON_SPEED = 100,
		BUTTON_SCROLLRATE,
		BUTTON_VISUAL,
		BUTTON_SOUND,
		BUTTON_OK,
		BUTTON_COUNT,
		BUTTON_FIRST = BUTTON_SPEED,
	};

	/*
	**	Dialog variables
	*/
	KeyNumType input;

	int gamespeed = Options.GameSpeed;
	int scrollrate = Options.ScrollRate;
	int selection;
	bool pressed = false;
	int curbutton = 0;
	TextButtonClass *buttons[BUTTON_COUNT - BUTTON_FIRST];
	TextPrintType style;

	/*
	**	Buttons
	*/
	GadgetClass *commands;										// button list

	SliderClass gspeed_btn(BUTTON_SPEED, d_speed_x, d_speed_y, d_speed_w, d_speed_h);

	SliderClass scrate_btn(BUTTON_SCROLLRATE, d_scroll_x, d_scroll_y, d_scroll_w, d_scroll_h);

	TextButtonClass visual_btn(BUTTON_VISUAL, TXT_VISUAL_CONTROLS,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_visual_x, d_visual_y, d_visual_w, d_visual_h);

	TextButtonClass sound_btn(BUTTON_SOUND, TXT_SOUND_CONTROLS,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_sound_x, d_sound_y, d_sound_w, d_sound_h);

	TextButtonClass okbtn(BUTTON_OK, TXT_OPTIONS_MENU,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_ok_x, d_ok_y);
	okbtn.X = (SeenBuff.Get_Width()-okbtn.Width)/2;

	/*
	**	Various Inits.
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Build button list
	*/
	commands = &okbtn;
	gspeed_btn.Add_Tail(*commands);
	scrate_btn.Add_Tail(*commands);
	visual_btn.Add_Tail(*commands);
	sound_btn.Add_Tail(*commands);

	/*
	**	Init button states
	**	For sliders, the thumb ranges from 0 - (maxval-1), so to convert the
	**	thumb value to a real-world value:
	**		val = (MAX - slider.Get_Value()) - 1;
	**	and,
	**		slider.Set_Value(-(val + 1 - MAX));
	*/
	gspeed_btn.Set_Maximum(OptionsClass::MAX_SPEED_SETTING);	// varies from 0 - 7
	gspeed_btn.Set_Thumb_Size(1);
	gspeed_btn.Set_Value((OptionsClass::MAX_SPEED_SETTING-1) - gamespeed);

	scrate_btn.Set_Maximum(OptionsClass::MAX_SCROLL_SETTING);	// varies from 0 - 7
	scrate_btn.Set_Thumb_Size(1);
	scrate_btn.Set_Value((OptionsClass::MAX_SCROLL_SETTING-1) - scrollrate);

	/*
	**	Fill array of button ptrs.
	*/
	buttons[0] = NULL;
	buttons[1] = NULL;
	buttons[2] = &visual_btn;
	buttons[3] = &sound_btn;
	buttons[4] = &okbtn;

	/*
	**	Processing loop.
	*/
	bool process = true;
	bool display = true;
	bool refresh = true;
	while (process) {

		/*
		**	Invoke game callback.
		*/
		if (GameToPlay == GAME_NORMAL) {
			Call_Back();
		} else {
			if (Main_Loop()) {
				process = false;
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
			Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);
			Draw_Caption(TXT_GAME_CONTROLS, d_dialog_x, d_dialog_y, d_dialog_w);
			Show_Mouse();
			display = false;
			refresh = true;
		}

		if (refresh) {
			Hide_Mouse();

			/*
			**	Label the game speed slider
			*/
			style = TPF_6PT_GRAD | TPF_NOSHADOW | TPF_USE_GRAD_PAL;
			if (curbutton == (BUTTON_SPEED - BUTTON_FIRST)) {
				style = (TextPrintType)(style | TPF_BRIGHT_COLOR);
			}
			Fancy_Text_Print(TXT_SPEED, d_speed_x, d_speed_y - d_txt6_h, CC_GREEN, TBLACK, style);

			Fancy_Text_Print(TXT_SLOWER, d_speed_x, d_speed_y + d_speed_h + 1, CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);
			Fancy_Text_Print(TXT_FASTER, d_speed_x + d_speed_w, d_speed_y + d_speed_h + 1, CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW|TPF_RIGHT);

			/*
			**	Label the scroll rate slider
			*/
			style = TPF_6PT_GRAD | TPF_NOSHADOW | TPF_USE_GRAD_PAL;
			if (curbutton == (BUTTON_SCROLLRATE - BUTTON_FIRST)) {
				style = (TextPrintType)(style | TPF_BRIGHT_COLOR);
			}
			Fancy_Text_Print(TXT_SCROLLRATE, d_scroll_x, d_scroll_y - d_txt6_h, CC_GREEN, TBLACK, style);

			Fancy_Text_Print (TXT_SLOWER, d_scroll_x, d_scroll_y + d_scroll_h + 1, CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);
			Fancy_Text_Print (TXT_FASTER, d_scroll_x + d_scroll_w, d_scroll_y + d_scroll_h + 1, CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW|TPF_RIGHT);

			commands->Draw_All();

			Show_Mouse();
			refresh = false;
		}

		/*
		**	Get user input.
		*/
		input = commands->Input();

		/*
		**	Process input.
		*/
		switch (input) {
			case (BUTTON_SPEED | KN_BUTTON):
				curbutton = (BUTTON_SPEED - BUTTON_FIRST);
				refresh = true;
				break;

			case (BUTTON_SCROLLRATE | KN_BUTTON):
				curbutton = (BUTTON_SCROLLRATE - BUTTON_FIRST);
				refresh = true;
				break;

			case (BUTTON_VISUAL | KN_BUTTON):
				selection = BUTTON_VISUAL;
				pressed = true;
				break;

			case (BUTTON_SOUND | KN_BUTTON):
				selection = BUTTON_SOUND;
				pressed = true;
				break;

			case (BUTTON_OK | KN_BUTTON):
				selection = BUTTON_OK;
				pressed = true;
				break;

			case (KN_ESC):
				process = false;
				break;

			case (KN_LEFT):
				if (curbutton == (BUTTON_SPEED - BUTTON_FIRST) ) {
					gspeed_btn.Bump(1);
				} else
					if (curbutton == (BUTTON_SCROLLRATE - BUTTON_FIRST) ) {
						scrate_btn.Bump(1);
					}
				break;

			case (KN_RIGHT):
				if (curbutton == (BUTTON_SPEED - BUTTON_FIRST) ) {
					gspeed_btn.Bump(0);
				} else
					if (curbutton == (BUTTON_SCROLLRATE - BUTTON_FIRST) ) {
						scrate_btn.Bump(0);
					}
				break;

			case (KN_UP):
				if (buttons[curbutton]) {
					buttons[curbutton]->Turn_Off();
					buttons[curbutton]->Flag_To_Redraw();
				}

				curbutton--;
				if (curbutton < 0) {
					curbutton = (BUTTON_COUNT - BUTTON_FIRST - 1);
				}

				if (buttons[curbutton]) {
					buttons[curbutton]->Turn_On();
					buttons[curbutton]->Flag_To_Redraw();
				}
				refresh = true;
				break;

			case (KN_DOWN):
				if (buttons[curbutton]) {
					buttons[curbutton]->Turn_Off();
					buttons[curbutton]->Flag_To_Redraw();
				}

				curbutton++;
				if (curbutton > (BUTTON_COUNT - BUTTON_FIRST - 1) ) {
					curbutton = 0;
				}

				if (buttons[curbutton]) {
					buttons[curbutton]->Turn_On();
					buttons[curbutton]->Flag_To_Redraw();
				}
				refresh = true;
				break;

			case (KN_RETURN):
				selection = curbutton + BUTTON_FIRST;
				pressed = true;
				break;

			default:
				break;
		}

		/*
		**	Perform some action. Either to exit the dialog or bring up another.
		*/
		if (pressed) {

			/*
			**	Record the new options slider settings.
			** The GameSpeed data member MUST NOT BE SET HERE!!!  It will cause multiplayer
			** games to go out of sync.  It's set by virtue of the event being executed.
			*/
			if (gamespeed != ((OptionsClass::MAX_SPEED_SETTING-1) - gspeed_btn.Get_Value()) ) {
				gamespeed = (OptionsClass::MAX_SPEED_SETTING-1) - gspeed_btn.Get_Value();
				OutList.Add(EventClass(EventClass::GAMESPEED, gamespeed));
			}

			if (scrollrate != ((OptionsClass::MAX_SCROLL_SETTING-1) - scrate_btn.Get_Value()) ) {
				scrollrate = (OptionsClass::MAX_SCROLL_SETTING-1) - scrate_btn.Get_Value();
				Options.ScrollRate = scrollrate;
			}
			process = false;

			/*
			** Save the settings in such a way that the GameSpeed is only set during
			** the save process; restore it when we're done, so multiplayer games don't
			** go out of sync.
			*/
			int old = Options.GameSpeed;		// save orig value
			Options.GameSpeed = gamespeed;
			Options.Save_Settings();			// save new value
			Options.GameSpeed = old;			// restore old value

			/*
			**	Possibly launch into another dialog if so directed.
			*/
			switch (selection) {
				case (BUTTON_VISUAL):
					VisualControlsClass().Process();
					process = true;
					display = true;
					refresh = true;
					break;

				case (BUTTON_SOUND):
					if (!SoundType) {
						CCMessageBox().Process(Text_String(TXT_NO_SOUND_CARD));
						process = true;
						display = true;
						refresh = true;
					} else {
						SoundControlsClass().Process();
					}
					break;

				case (BUTTON_OK):
					break;
			}

			pressed = false;
		}
	}
}

