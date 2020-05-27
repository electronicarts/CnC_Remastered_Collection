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

/* $Header: /CounterStrike/VISUDLG.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : VISUDLG.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Maria del Mar McCready Legg                                  *
 *                                  Joe L. Bostic                                              *
 *                                                                                             *
 *                   Start Date : Jan 8, 1995                                                  *
 *                                                                                             *
 *                  Last Update : June 18, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   VisualControlsClass::Process -- Process the visual control dialog box.                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"visudlg.h"


/***********************************************************************************************
 * VisualControlsClass::Process -- Process the visual control dialog box.                      *
 *                                                                                             *
 *    This routine displays and processes the visual controls dialog box.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void VisualControlsClass::Process(void)
{
	static int _titles[4] = {
		TXT_BRIGHTNESS,
		TXT_COLOR,
		TXT_CONTRAST,
		TXT_TINT
	};

	enum {
		NUM_OF_BUTTONS = 6,
	};

	/*
	** Make em resolution independent
	*/
	int option_width = OPTION_WIDTH * RESFACTOR;				// Width of dialog box.
	int option_height = OPTION_HEIGHT * RESFACTOR;			// Height of dialog box.
	int option_x = OPTION_X * RESFACTOR;
	int option_y = OPTION_Y * RESFACTOR;
	int text_x = TEXT_X * RESFACTOR;
	int text_y = TEXT_Y * RESFACTOR;
	int slider_x = SLIDER_X * RESFACTOR;
	int slider_y = SLIDER_Y * RESFACTOR;
	int slider_width = SLIDER_WIDTH * RESFACTOR;				// Width of each control slider.
	int slider_height = SLIDER_HEIGHT * RESFACTOR;			// Height of each control slider.
	int slider_y_spacing = SLIDER_Y_SPACING * RESFACTOR;	// Vertical spacing between sliders.
	int button_x = BUTTON_X * RESFACTOR;	 						// Options button x pos
	int button_y = BUTTON_Y * RESFACTOR; 						// Options button y pos


	/*
	**	Variables.
	*/
	int selection;
	bool pressed;
	int curbutton;
	TextButtonClass * buttons[NUM_OF_BUTTONS];
	SliderClass * buttonsliders[NUM_OF_BUTTONS];

	Set_Logic_Page(SeenBuff);

	/*
	**	Create Buttons.  Button coords are in pixels, but are window-relative.
	*/
	TextButtonClass optionsbtn(BUTTON_OPTIONS, TXT_OK, TPF_BUTTON, 0, button_y, 60*RESFACTOR);
//	TextButtonClass optionsbtn(BUTTON_OPTIONS, TXT_OPTIONS_MENU, TPF_BUTTON, 0, button_y);
	TextButtonClass resetbtn(BUTTON_RESET, TXT_RESET_MENU, TPF_BUTTON, 0, button_y, 80*RESFACTOR);

	/*
	**	Centers options button.
	*/
	optionsbtn.X = option_x + (option_width - optionsbtn.Width - (17 * RESFACTOR));
	resetbtn.X   = option_x + (17 * RESFACTOR);

	resetbtn.Add_Tail(optionsbtn);

	/*
	**	Brightness (value) control.
	*/
	SliderClass brightness(BUTTON_BRIGHTNESS, slider_x, slider_y + (slider_y_spacing*0), slider_width, slider_height, true);
	brightness.Set_Thumb_Size(20 * RESFACTOR);
	brightness.Set_Value(Options.Get_Brightness() * 256);
	brightness.Add_Tail(optionsbtn);

	/*
	**	Color (saturation) control.
	*/
	SliderClass color(BUTTON_COLOR, slider_x, slider_y + (slider_y_spacing*1), slider_width, slider_height, true);
	color.Set_Thumb_Size(20 * RESFACTOR);
	color.Set_Value(Options.Get_Saturation() * 256);
	color.Add_Tail(optionsbtn);

	/*
	**	Contrast control.
	*/
	SliderClass contrast(BUTTON_CONTRAST, slider_x, slider_y + (slider_y_spacing*2), slider_width, slider_height, true);
	contrast.Set_Thumb_Size(20 * RESFACTOR);
	contrast.Set_Value(Options.Get_Contrast() * 256);
	contrast.Add_Tail(optionsbtn);

	/*
	**	Tint (hue) control.
	*/
	SliderClass tint(BUTTON_TINT, slider_x, slider_y + (slider_y_spacing*3), slider_width, slider_height, true);
	tint.Set_Thumb_Size(20 * RESFACTOR);
	tint.Set_Value(Options.Get_Tint() * 256);
	tint.Add_Tail(optionsbtn);

	/*
	**	This causes left mouse button clicking within the confines of the dialog to
	**	be ignored if it wasn't recognized by any other button or slider.
	*/
	GadgetClass dialog(option_x, option_y, option_width, option_height, GadgetClass::LEFTPRESS);
	dialog.Add_Tail(optionsbtn);

	/*
	**	This causes a right click anywhere or a left click outside the dialog region
	**	to be equivalent to clicking on the return to options dialog.
	*/
	ControlClass background(BUTTON_OPTIONS, 0, 0, SeenBuff.Get_Width(), SeenBuff.Get_Height(),
									GadgetClass::LEFTPRESS|GadgetClass::RIGHTPRESS);
	background.Add_Tail(optionsbtn);

	curbutton = 0;
	buttons[0] = NULL;
	buttons[1] = NULL;
	buttons[2] = NULL;
	buttons[3] = NULL;
	buttons[4] = &resetbtn;
	buttons[5] = &optionsbtn;

	buttonsliders[0] = &brightness;
	buttonsliders[1] = &color;
	buttonsliders[2] = &contrast;
	buttonsliders[3] = &tint;
	buttonsliders[4] = NULL;
	buttonsliders[5] = NULL;

	/*
	**	Main Processing Loop.
	*/
	bool display = true;
	bool process = true;
	bool partial = true;
	pressed = false;
	while (process) {

		/*
		**	Invoke game callback.
		*/
		if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
			Call_Back();
		} else {
			if (Main_Loop()) {
				process = false;
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
			Hide_Mouse();
			Dialog_Box(option_x, option_y, option_width, option_height);
			Draw_Caption(TXT_VISUAL_CONTROLS, option_x, option_y, option_width);
			Show_Mouse();
			display = false;
			partial = true;
		}

		/*
		**	If just the buttons and captions need to be redrawn, then do so now.
		*/
		if (partial) {
			Hide_Mouse();

			/*
			**	Draw the titles.
			*/
			for (int i = 0; i < (sizeof(_titles)/sizeof(_titles[0])); i++) {
				Fancy_Text_Print(_titles[i], slider_x - (8 * RESFACTOR), text_y + (i*slider_y_spacing),
					GadgetClass::Get_Color_Scheme(), TBLACK, TPF_TEXT|TPF_RIGHT| ((curbutton == i) ? TPF_BRIGHT_COLOR : TPF_TEXT));
			}
			optionsbtn.Draw_All();
			Show_Mouse();
			partial = false;
		}

		/*
		**	Get and process player input.
		*/
		KeyNumType input = optionsbtn.Input();
		switch (input) {
			case (BUTTON_BRIGHTNESS | KN_BUTTON):
				Options.Set_Brightness(fixed(brightness.Get_Value(), 256));
				break;

			case (BUTTON_COLOR | KN_BUTTON):
				Options.Set_Saturation(fixed(color.Get_Value(), 256));
				break;

			case (BUTTON_CONTRAST | KN_BUTTON):
				Options.Set_Contrast(fixed(contrast.Get_Value(), 256));
				break;

			case (BUTTON_TINT | KN_BUTTON):
				Options.Set_Tint(fixed(tint.Get_Value(), 256));
				break;

			case (BUTTON_RESET | KN_BUTTON):
				selection = BUTTON_RESET;
				pressed = true;
				break;

			case KN_ESC:
			case BUTTON_OPTIONS|KN_BUTTON:
				selection = BUTTON_OPTIONS;
				pressed = true;
				break;

			case (KN_LEFT):
				if (curbutton <= (BUTTON_TINT - BUTTON_BRIGHTNESS)) {
					buttonsliders[curbutton]->Bump(1);
					switch (curbutton) {
						case (BUTTON_BRIGHTNESS - BUTTON_BRIGHTNESS):
							Options.Set_Brightness(fixed(brightness.Get_Value(), 256));
							break;

						case (BUTTON_COLOR - BUTTON_BRIGHTNESS):
							Options.Set_Saturation(fixed(color.Get_Value(), 256));
							break;

						case (BUTTON_CONTRAST - BUTTON_BRIGHTNESS):
							Options.Set_Contrast(fixed(contrast.Get_Value(), 256));
							break;

						case (BUTTON_TINT - BUTTON_BRIGHTNESS):
							Options.Set_Tint(fixed(tint.Get_Value(), 256));
							break;
					}
				} else {
					buttons[curbutton]->Turn_Off();
					buttons[curbutton]->Flag_To_Redraw();

					curbutton--;
					if (curbutton < (BUTTON_RESET - BUTTON_BRIGHTNESS) ) {
						curbutton = (BUTTON_OPTIONS - BUTTON_BRIGHTNESS);
					}

					buttons[curbutton]->Turn_On();
					buttons[curbutton]->Flag_To_Redraw();
				}
				break;

			case (KN_RIGHT):
				if (curbutton <= (BUTTON_TINT - BUTTON_BRIGHTNESS)) {
					buttonsliders[curbutton]->Bump(0);
					switch (curbutton) {
						case (BUTTON_BRIGHTNESS - BUTTON_BRIGHTNESS):
							Options.Set_Brightness(fixed(brightness.Get_Value(), 256));
							break;

						case (BUTTON_COLOR - BUTTON_BRIGHTNESS):
							Options.Set_Saturation(fixed(color.Get_Value(), 256));
							break;

						case (BUTTON_CONTRAST - BUTTON_BRIGHTNESS):
							Options.Set_Contrast(fixed(contrast.Get_Value(), 256));
							break;

						case (BUTTON_TINT - BUTTON_BRIGHTNESS):
							Options.Set_Tint(fixed(tint.Get_Value(), 256));
							break;
					}
				} else {
					buttons[curbutton]->Turn_Off();
					buttons[curbutton]->Flag_To_Redraw();

					curbutton++;
					if (curbutton > (BUTTON_OPTIONS - BUTTON_BRIGHTNESS) ) {
						curbutton = (BUTTON_RESET - BUTTON_BRIGHTNESS);
					}

					buttons[curbutton]->Turn_On();
					buttons[curbutton]->Flag_To_Redraw();
				}
				break;

			case (KN_UP):
				if (curbutton <= (BUTTON_TINT - BUTTON_BRIGHTNESS) ) {
					partial = true;
				} else {
					buttons[curbutton]->Turn_Off();
					buttons[curbutton]->Flag_To_Redraw();
				}

				curbutton--;
				if (curbutton == (BUTTON_RESET - BUTTON_BRIGHTNESS) ) {
					curbutton--;
				}

				if (curbutton < 0) {
					curbutton = (BUTTON_RESET - BUTTON_BRIGHTNESS);
				}

				if (curbutton <= (BUTTON_TINT - BUTTON_BRIGHTNESS) ) {
					partial = true;
				} else {
					buttons[curbutton]->Turn_On();
					buttons[curbutton]->Flag_To_Redraw();
				}
				break;

			case (KN_DOWN):
				if (curbutton <= (BUTTON_TINT - BUTTON_BRIGHTNESS) ) {
					partial = true;
				} else {
					buttons[curbutton]->Turn_Off();
					buttons[curbutton]->Flag_To_Redraw();
				}

				curbutton++;
				if (curbutton > (BUTTON_RESET - BUTTON_BRIGHTNESS) ) {
					curbutton = 0;
				}

				if (curbutton <= (BUTTON_TINT - BUTTON_BRIGHTNESS) ) {
					partial = true;
				} else {
					buttons[curbutton]->Turn_On();
					buttons[curbutton]->Flag_To_Redraw();
				}
				break;

			case (KN_RETURN):
				selection = curbutton + BUTTON_BRIGHTNESS;
				pressed = true;
				break;

			default:
				break;
		}


		if (pressed) {
			switch (selection) {
				case (BUTTON_RESET):
					brightness.Set_Value(128);
					contrast.Set_Value(128);
					color.Set_Value(128);
					tint.Set_Value(128);

					Options.Set_Brightness(fixed::_1_2);
					Options.Set_Contrast(fixed::_1_2);
					Options.Set_Saturation(fixed::_1_2);
					Options.Set_Tint(fixed::_1_2);
					break;

				case (BUTTON_OPTIONS):
					process = false;
					break;
			}

			pressed = false;
		}
	}
}

