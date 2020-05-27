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

/* $Header: /CounterStrike/MSGBOX.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OPTIONS.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 8, 1994                                                 *
 *                                                                                             *
 *                  Last Update : August 24, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   WWMessageBox::Process -- Handles all the options graphic interface.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"msgbox.h"
#include "gadget.h"


#ifdef FIXIT_VERSION_3
bool cancel_current_msgbox = false;
#endif

/***********************************************************************************************
 * WWMessageBox::Process -- pops up a message with yes/no, etc                                 *
 *                                                                                             *
 * This function displays a dialog box with a one-line message, and                            *
 * up to two buttons. The 2nd button is optional. The buttons default                          *
 * to "OK" and nothing, respectively. The hotkeys for the buttons are                          *
 * RETURN and ESCAPE.                                                                          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      msg         message to display                                                         *
 *      b1txt         text for 1st button                                                      *
 *      b2txt         text for 2nd button; NULL = no 2nd button                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      # of button selected (0 = 1st)                                                         *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      'msg' text must be <= 38 chars                                                         *
 *      'b1txt' and 'b2txt' must each be <= 18 chars                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/08/1994 BR : Created.                                                                  *
 *   05/18/1995 JLB : Uses new font and dialog style.                                          *
 *   08/24/1995 JLB : Handles three buttons.                                                   *
 *=============================================================================================*/
#define	BUTTON_1		1
#define	BUTTON_2		2
#define	BUTTON_3		3
#define	BUTTON_FLAG	0x8000
int WWMessageBox::Process(const char * msg, const char * b1txt, const char * b2txt, const char * b3txt, bool preserve)
{
#define BUFFSIZE (511)
	char buffer[BUFFSIZE];
	int retval;
	bool process;								// loop while true
	int selection;
	bool pressed;
	int curbutton;
	TextButtonClass * buttons[3];
	void * back;
	BOOL display;									// display level
	int  realval[5];

#ifndef WIN32
	int	preservex,preservey,preservew,preserveh;
#endif

	#ifdef WIN32
	GraphicBufferClass seen_buff_save(VisiblePage.Get_Width(), VisiblePage.Get_Height(), (void*)NULL);
	#endif

	if (b1txt != NULL && *b1txt == '\0') b1txt = NULL;
	if (b2txt != NULL && *b2txt == '\0') b2txt = NULL;
	if (b3txt != NULL && *b3txt == '\0') b3txt = NULL;

	//PG_TO_FIX
	//Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_TEXT);

	/*
	**	Examine the optional button parameters. Fetch the width and starting
	**	characters for each.
	*/
	int bwidth, bheight;				// button width and height
	int numbuttons = 0;
	if (b1txt != NULL) {

		/*
		**	Build the button list.
		*/
		bheight = FontHeight + FontYSpacing + (2 * RESFACTOR);
		bwidth = max((String_Pixel_Width(b1txt) + (8 * RESFACTOR)), (30 * RESFACTOR));

		if (b2txt != NULL) {
			numbuttons = 2;
			bwidth = max(((int)String_Pixel_Width( b2txt ) + (8 * RESFACTOR)), bwidth);

			if (b3txt != NULL) {
				numbuttons = 3;
			}

		} else {
			numbuttons = 1;
		}
	}

	/*
	**	Determine the dimensions of the text to be used for the dialog box.
	**	These dimensions will control how the dialog box looks.
	*/
	buffer[BUFFSIZE-1] = 0;
	strncpy(buffer, msg, BUFFSIZE-1);
	//PG_TO_FIX
	//Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_TEXT);
	int width;
	int height;
	int lines = Format_Window_String(buffer, 255 * RESFACTOR, width, height);
	TextPrintType tpf = TPF_TEXT;

	width = max(width, (90 * RESFACTOR));
	width += 40 * RESFACTOR;
	height += (numbuttons == 0) ? (40 * RESFACTOR) : (60 * RESFACTOR);

	int x = (SeenBuff.Get_Width() - width) / 2;
	int y = (SeenBuff.Get_Height() - height) / 2;
	int printx = x + (20 * RESFACTOR);

	/*
	**	Special hack to center a one line dialog box text.
	*/
	if (lines == 1) {
		printx = x + width/2;
		tpf = tpf | TPF_CENTER;
	}

	/*
	**	Other inits.
	*/
	Set_Logic_Page(SeenBuff);
	#ifdef WIN32
	VisiblePage.Blit(seen_buff_save);
	#endif

	/*
	**	Initialize the button structures. All are initialized, even though one (or none) may
	**	actually be added to the button list.
	*/
	//DOS BUILD GERMAN BUTTONS NEED TO ONE ON TOP OF THE OTHER  VG 11/6/96
	TextButtonClass button1(BUTTON_1, b1txt, TPF_BUTTON,
		x + ((numbuttons == 1) ? ((width - bwidth) >> 1) : (20 * RESFACTOR)), y + height - (bheight + (15 * RESFACTOR)), bwidth);

	/*
	**	Center button.
	*/
	TextButtonClass button2(BUTTON_2, b2txt, TPF_BUTTON,
		x + width - (bwidth + (20 * RESFACTOR)), y + height - (bheight + (15 * RESFACTOR)), bwidth);

	/*
	**	Right button.
	*/
	TextButtonClass button3(BUTTON_3, b3txt, TPF_BUTTON, 0, y + height - (bheight + (15 * RESFACTOR)));
	button3.X = x + ((width - button3.Width) >> 1);

	TextButtonClass * buttonlist = 0;
	curbutton = 0;

	/*
	**	Add and initialize the buttons to the button list.
	*/
	memset(buttons, '\0', sizeof(buttons));
	if (numbuttons > 0) {
		buttonlist = &button1;
		buttons[0] = &button1;
		realval[0] = BUTTON_1;
		if (numbuttons > 2) {
			button3.Add(*buttonlist);
			buttons[1] = &button3;
			realval[1] = BUTTON_3;
			button2.Add(*buttonlist);
			buttons[2] = &button2;
			realval[2] = BUTTON_2;
			buttons[curbutton]->Turn_On();
		} else {
			if (numbuttons == 2) {
				button2.Add(*buttonlist);
				buttons[1] = &button2;
				realval[1] = BUTTON_2;
				buttons[curbutton]->Turn_On();
			}
		}
	}

	/*
	**	Draw the dialog.
	*/
	Hide_Mouse();
	if (preserve) {
#ifndef WIN32
	preservex = max(0, x-4);
	preservey = max(0, y-4);
	preservew = min(width+8, 320-preservex);
	preserveh = min(height+8, 200-preservey);
	back = new char[preservew * preserveh];
	SeenBuff.To_Buffer(preservex, preservey, preservew, preserveh, back, preservew * preserveh);
#else
		back = new char[width * height];
		SeenBuff.To_Buffer(x, y, width, height, back, width * height);
#endif
	}
	Dialog_Box(x, y, width, height);
	Draw_Caption(Caption, x, y, width);

	/*
	**	Draw the body of the message.
	*/
	Fancy_Text_Print(buffer, printx, y + 20*RESFACTOR, GadgetClass::Get_Color_Scheme(), TBLACK, tpf);

	/*
	**	Redraw the buttons.
	*/
	if (buttonlist) {
		buttonlist->Draw_All();
	}
	Show_Mouse();

	/*
	**	Main Processing Loop.
	*/
	if (buttonlist) {
		process = true;
		pressed = false;
		while (process) {

			#ifdef WIN32
			/*
			** If we have just received input focus again after running in the background then
			** we need to redraw.
			*/
			if (AllSurfaces.SurfacesRestored) {
				AllSurfaces.SurfacesRestored = false;
				seen_buff_save.Blit(VisiblePage);
				display = true;
			}
			#endif

			if (display) {
				display = false;

				Hide_Mouse();
				Dialog_Box(x, y, width, height);
				Draw_Caption(Caption, x, y, width);

				/*
				**	Draw the body of the message.
				*/
				Fancy_Text_Print(buffer, printx, y + 20*RESFACTOR, GadgetClass::Get_Color_Scheme(), TBLACK, tpf);

				/*
				**	Redraw the buttons.
				*/
				if (buttonlist) {
					buttonlist->Draw_All();
				}
				Show_Mouse();
			}

			/*
			**	Invoke game callback.
			*/
			Call_Back();

			/*
			**	Fetch and process input.
			*/
			KeyNumType input = buttonlist->Input();
#ifdef FIXIT_VERSION_3
			//	I really hate to do this, but...      ajw
			if( cancel_current_msgbox )
			{
				cancel_current_msgbox = false;
				input = KN_ESC;
			}
#endif
			switch (input) {
				case (KN_ESC):
					selection = realval[numbuttons-1];
					pressed = true;

#ifdef NEVER
					if (numbuttons > 2) {
						selection = realval[1];
						pressed = true;
					} else {
						selection = realval[2];
						pressed = true;
					}
#endif
					break;

				case (BUTTON_1|BUTTON_FLAG):
					selection = realval[0];
					pressed = true;
					break;

				case (BUTTON_2|BUTTON_FLAG):
					if (numbuttons > 2) {
						selection = realval[2];
					} else {
						selection = realval[1];
					}
					pressed = true;
					break;

				case (BUTTON_3|BUTTON_FLAG):
					selection = realval[1];
					pressed = true;
					break;

				case (KN_LEFT):
					if (numbuttons > 1) {
						buttons[curbutton]->Turn_Off();
						buttons[curbutton]->Flag_To_Redraw();

						curbutton--;
						if (curbutton < 0) {
							curbutton = numbuttons - 1;
						}

						buttons[curbutton]->Turn_On();
						buttons[curbutton]->Flag_To_Redraw();
					}
					break;

				case (KN_RIGHT):
					if (numbuttons > 1) {
						buttons[curbutton]->Turn_Off();
						buttons[curbutton]->Flag_To_Redraw();

						curbutton++;
						if (curbutton > (numbuttons - 1) ) {
							curbutton = 0;
						}

						buttons[curbutton]->Turn_On();
						buttons[curbutton]->Flag_To_Redraw();
					}
					break;

				case (KN_RETURN):
					selection = realval[curbutton];
					pressed = true;
					break;

				/*
				**	Check 'input' to see if it's the 1st char of button text
				*/
				default:
					break;
			}

			if (pressed) {

				TextButtonClass * toggle;
				/*
				**	Turn all the buttons off.
				*/
				toggle = (TextButtonClass *)buttonlist->Extract_Gadget(BUTTON_1);
				if (toggle != NULL) {
					toggle->Turn_Off();
					toggle->IsPressed = false;
				}
				toggle = (TextButtonClass *)buttonlist->Extract_Gadget(BUTTON_2);
				if (toggle != NULL) {
					toggle->Turn_Off();
					toggle->IsPressed = false;
				}
				toggle = (TextButtonClass *)buttonlist->Extract_Gadget(BUTTON_3);
				if (toggle != NULL) {
					toggle->Turn_Off();
					toggle->IsPressed = false;
				}

				/*
				**	Turn on and depress the button that was selected.
				*/
				if (selection == BUTTON_1 || selection == BUTTON_2 || selection == BUTTON_3) {
					TextButtonClass * toggle = (TextButtonClass *)buttonlist->Extract_Gadget(selection);
					if (toggle != NULL) {
						toggle->Turn_On();
//						toggle->IsOn = true;
						toggle->IsPressed = true;
					}
				}
				Hide_Mouse();
				buttonlist->Draw_All(true);
				Show_Mouse();

				switch (selection) {
					case (BUTTON_1):
						retval = 0;
						process = false;
						break;

					case (BUTTON_2):
						retval = 1;
						process = false;
						break;

					case BUTTON_3:
						retval = 2;
						process = false;
						break;
				}

				pressed = false;
			}
		}

	} else {

		Keyboard->Clear();
	}

	/*
	**	Restore the screen if necessary.
	*/
	if (preserve) {
		Hide_Mouse();
		if (SeenBuff.Lock()) {
			#ifdef WIN32
				Buffer_To_Page(x, y, width, height, back, &SeenBuff);
			#else
				MCGA_Buffer_To_Page(preservex, preservey, preservew, preserveh, back, &SeenBuff);
			#endif
		}
		SeenBuff.Unlock();

		delete[] back;
		back = NULL;
		Show_Mouse();
	}
	return(retval);
}


/***********************************************************************************************
 * WWMessageBox::Process -- this one takes integer text arguments                              *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      msg         message to display                                                         *
 *      b1txt         text for 1st button                                                      *
 *      b2txt         text for 2nd button; NULL = no 2nd button                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      # of button selected (0 = 1st)                                                         *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      'msg' text must be <= 38 chars                                                         *
 *      'b1txt' and 'b2txt' must each be <= 18 chars                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/12/1994 BR : Created.                                                                  *
 *   06/18/1995 JLB : Simplified.                                                              *
 *=============================================================================================*/
int WWMessageBox::Process(int msg, int b1txt, int b2txt, int b3txt, bool preserve)
{
	return(Process(Text_String(msg), b1txt, b2txt, b3txt, preserve));
}


/***********************************************************************************************
 * WWMessageBox::Process -- Displays message box.                                              *
 *                                                                                             *
 *    This routine will display a message box and wait for player input. It varies from the    *
 *    other message box functions only in the type of parameters it takes.                     *
 *                                                                                             *
 * INPUT:   msg   -- Pointer to text string for the message itself.                            *
 *                                                                                             *
 *          b1txt -- Text number for the "ok" button.                                          *
 *                                                                                             *
 *          b2txt -- Text number for the "cancel" button.                                      *
 *                                                                                             *
 * OUTPUT:  Returns with the button selected. "true" if "OK" was pressed, and "false" if       *
 *          "CANCEL" was pressed.                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int WWMessageBox::Process(char const * msg, int b1txt, int b2txt, int b3txt, bool preserve)
{
	return(Process(msg, Text_String(b1txt), Text_String(b2txt), Text_String(b3txt), preserve));
}
