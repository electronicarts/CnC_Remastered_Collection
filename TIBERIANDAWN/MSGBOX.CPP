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

/* $Header:   F:\projects\c&c\vcs\code\msgbox.cpv   2.18   16 Oct 1995 16:50:48   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *   CCMessageBox::Process -- Handles all the options graphic interface.                         *
 *   CCMessageBox::Process -- Displays message box.                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"msgbox.h"
#include "gadget.h"

#ifdef JAPANESE
CCMessageBox::CCMessageBox(int caption, bool pict) : Caption(caption), IsPicture(pict)
{
}
#endif

/***********************************************************************************************
 * CCMessageBox::Process -- pops up a message with yes/no, etc                                   *
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
int CCMessageBox::Process(const char *msg, const char *b1txt, const char *b2txt, const char *b3txt, bool preserve)
{
#define BUFFSIZE (511)
//#define BUFFSIZE (255)
	char buffer[BUFFSIZE];
	int retval;
	bool process;								// loop while true
	KeyNumType input;							// user input
	int selection;
	bool pressed;
	int curbutton;
	TextButtonClass *buttons[3];
	void *back;
	BOOL display;									// display level
	int  realval[5];

	GraphicBufferClass seen_buff_save(VisiblePage.Get_Width(),VisiblePage.Get_Height(),(void*)NULL);

	int factor = (SeenBuff.Get_Width() == 320) ? 1 : 2;

	if (b1txt && *b1txt == '\0') b1txt = 0;
	if (b2txt && *b2txt == '\0') b2txt = 0;
	if (b3txt && *b3txt == '\0') b3txt = 0;

	/*
	**	Examine the optional button parameters. Fetch the width and starting
	**	characters for each.
	*/
	Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_6PT_GRAD|TPF_NOSHADOW);
	char b1char, b2char, b3char;	// 1st char of each string
	int bwidth, bheight;				// button width and height
	int numbuttons = 0;
	if (b1txt) {
		b1char = toupper(b1txt[0]);

		/*
		**	Build the button list.
		*/
		bheight = FontHeight + FontYSpacing + (2*factor);
		bwidth = MAX((String_Pixel_Width(b1txt) + (8 *factor)), 30U * (unsigned)factor);

		if (b2txt) {
			numbuttons = 2;
			b2char = toupper(b2txt[0]);
			bwidth = MAX((String_Pixel_Width( b2txt ) + (8*factor)), (unsigned)bwidth);

			if (b3txt) {
				numbuttons = 3;
				b3char = toupper(b3txt[0]);
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
	strncpy(buffer, msg, BUFFSIZE-2);
	int width;
	int height;

#ifdef JAPANESE
	if(IsPicture) {
		width = 90;
		height = 140 - 60;
	} else
#endif
	Format_Window_String(buffer, 255 * factor, width, height);

//BG #ifdef JAPANESE
//BG 	if(!IsPicture) {
//BG #else
	width = MAX(width, 50 * factor);
	width += 40 * factor;
//BG #endif
//BG #ifdef JAPANESE
//BG 	}
//BG #endif
	height += (numbuttons == 0) ? (30 * factor) : (60 * factor);

	int x = (SeenBuff.Get_Width() - width) / 2;
	int y = (SeenBuff.Get_Height() - height) / 2;

	/*
	**	Other inits.
	*/
	Set_Logic_Page(SeenBuff);
	VisiblePage.Blit(seen_buff_save);

	/*
	**	Initialize the button structures. All are initialized, even though one (or none) may
	**	actually be added to the button list.
	*/
	TextButtonClass button1(BUTTON_1, b1txt, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW,
		x + ((numbuttons == 1) ? ((width - bwidth) >> 1) : (10*factor)), y + height - (bheight + (5*factor)), bwidth);

	TextButtonClass button2(BUTTON_2, b2txt, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW,
		x + width - (bwidth + (10*factor)), y + height - (bheight + (5*factor)), bwidth);

	TextButtonClass button3(BUTTON_3, b3txt, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW, 0, y + height - (bheight + (5*factor)));
	button3.X = x + ((width - button3.Width) >> 1);

	TextButtonClass * buttonlist = 0;
	curbutton = 0;

	/*
	**	Add and inialize the buttons to the button list.
	*/
	if (numbuttons) {
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
		} else if (numbuttons == 2) {
			button2.Add(*buttonlist);
			buttons[1] = &button2;
			realval[1] = BUTTON_2;
			buttons[curbutton]->Turn_On();
		}
	}

	/*
	**	Draw the dialog.
	*/
	Hide_Mouse();
	if (preserve) {
		back = new char[width * height];
		SeenBuff.To_Buffer(x, y, width, height, back, width * height);
	}
	//display = TRUE;
#ifdef JAPANESE
	if(IsPicture) {
		Load_Uncompress(CCFileClass(msg), SysMemPage, SysMemPage);
		SysMemPage.Blit(SeenBuff, 160, 100);
	} else {
#endif
	Dialog_Box(x, y, width, height);
	Draw_Caption(Caption, x, y, width);

	/*
	**	Draw the caption.
	*/
	Fancy_Text_Print(buffer, x + (20*factor), y + (25*factor), CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);
#ifdef JAPANESE
	}
#endif
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

			/*
			** If we have just received input focus again after running in the background then
			** we need to redraw.
			*/
			if (AllSurfaces.SurfacesRestored){
				AllSurfaces.SurfacesRestored=FALSE;
				seen_buff_save.Blit(VisiblePage);
				display = TRUE;
			}

			if (display){
				display = FALSE;

				Hide_Mouse();

#ifdef JAPANESE
				if(IsPicture) {
					Load_Uncompress(CCFileClass(msg), SysMemPage, SysMemPage);
					SysMemPage.Blit(SeenBuff, 160, 100);
				} else {
#endif
					Dialog_Box(x, y, width, height);
					Draw_Caption(Caption, x, y, width);

					/*
					**	Draw the caption.
					*/
					Fancy_Text_Print(buffer, x + (20 * factor), y + (25 * factor), CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);
#ifdef JAPANESE
				}
#endif
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
			input = buttonlist->Input();
			switch (input) {
				case (BUTTON_1|BUTTON_FLAG):
					selection = realval[0];
					pressed = true;
					break;

				case (KN_ESC):
					if (numbuttons > 2) {
						selection = realval[1];
						pressed = true;
					} else {
						selection = realval[2];
						pressed = true;
					}
					break;


				case (BUTTON_2|BUTTON_FLAG):
					selection = realval[1];
					pressed = true;
					break;

				case (BUTTON_3|BUTTON_FLAG):
					selection = realval[2];
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
					selection = curbutton + BUTTON_1;
					pressed = true;
					break;

				/*
				**	Check 'input' to see if it's the 1st char of button text
				*/
				default:
#ifdef NEVER
					if (b1char == toupper(Keyboard::To_ASCII((KeyNumType)(input & 0xFF)))) {
						selection = BUTTON_1;
						pressed = true;
					} else if (b2txt!=NULL &&
						b2char == toupper(Keyboard::To_ASCII((KeyNumType)(input & 0xFF)))) {
						selection = BUTTON_2;
						pressed = true;
					}
#endif
					break;
			}

			if (pressed) {
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

		/*
		**	Delay for a brief moment so that the dialog box will be visible long
		**	enough to read the text.
		*/
		CountDownTimerClass timer(BT_SYSTEM, 0);
		timer.Start();
		timer.Set(TICKS_PER_SECOND*4);
		while (timer.Time() > 0) {
			Call_Back();
		}
		Keyboard::Clear();
	}

	/*
	**	Restore the screen if necessary.
	*/
	if (preserve) {
		Hide_Mouse();
		if (SeenBuff.Lock()){
			Buffer_To_Page(x, y, width, height, back, &SeenBuff);
		}
		SeenBuff.Unlock();
		delete[] back;
		back = NULL;
		Show_Mouse();
	}
	return(retval);
}


/***********************************************************************************************
 * CCMessageBox::Process -- this one takes integer text arguments                                *
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
int CCMessageBox::Process(int msg, int b1txt, int b2txt, int b3txt, bool preserve)
{
	return(Process(Text_String(msg), b1txt, b2txt, b3txt, preserve));
}


/***********************************************************************************************
 * CCMessageBox::Process -- Displays message box.                                                *
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
int CCMessageBox::Process(char const *msg, int b1txt, int b2txt, int b3txt, bool preserve)
{
	return(Process(msg, Text_String(b1txt), Text_String(b2txt), Text_String(b3txt), preserve));
}
