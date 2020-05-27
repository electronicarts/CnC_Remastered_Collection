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

/* $Header: /CounterStrike/STATBTN.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TEXTBTN.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/15/95                                                     *
 *                                                                                             *
 *                  Last Update : January 19, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   StaticButtonClass::Draw_Background -- Draws the background to the text button.            *
 *   StaticButtonClass::Draw_Me -- Draws the text buttons as indicated.                        *
 *   StaticButtonClass::Draw_Text -- This draws the text for the text button.                  *
 *   StaticButtonClass::Set_Text -- Assigns a new text string to this button.                  *
 *   StaticButtonClass::StaticButtonClass -- Normal constructor for a text button.             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"
#include "statbtn.h"


/***********************************************************************************************
 * StaticButtonClass::StaticButtonClass -- Normal constructor for a text button.               *
 *                                                                                             *
 *    This is the constructor for text buttons if the text is provided as a string pointer.    *
 *                                                                                             *
 * INPUT:   id    -- The ID number to assign to this button.                                   *
 *                                                                                             *
 *          text  -- Pointer to the text string to display on top of the button.               *
 *                                                                                             *
 *          x,y   -- Pixel coordinate of button's upper left corner.                           *
 *                                                                                             *
 *          w,h   -- Dimensions of the button. If these are not filled in (or with -1), then   *
 *                   the dimensions are adapted to fit the text assigned to the button.        *
 *                                                                                             *
 *          style -- The print style for the text in the button. These are the TPF_ flags      *
 *                   used by Fancy_Text_Print().                                               *
 *                                                                                             *
 * OUTPUT:   none                                                                              *
 *                                                                                             *
 * WARNINGS: Call Set_Text & Set_Style, & init X,Y,Width,Height,ID before using this button.	  *
 *                                                                                             *
 * HISTORY:  01/15/1995 JLB : Created.                                                         *
 *=============================================================================================*/
StaticButtonClass::StaticButtonClass(unsigned , char const * text, TextPrintType style, int x, int y, int w, int h) :
	GadgetClass(x, y, w, h, FlagEnum(0)),
	String(NULL),
	PrintFlags(style)
{
	/*
	**	Make a duplicate of the string to display.
	*/
	Set_Text(text, false);

	if (w == -1 || h == -1) {
		//PG_TO_FIX
		//Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, PrintFlags);
		if (w == -1) {
			Width = String_Pixel_Width(String);
		}
		if (h == -1) {
			Height = FontHeight;
		}
	}
}


/***********************************************************************************************
 * StaticButtonClass::StaticButtonClass -- Default constructor for a text button.              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:   none                                                                              *
 *                                                                                             *
 * WARNINGS: none                                                                              *
 *                                                                                             *
 * HISTORY:  01/15/1995 JLB : Created.                                                         *
 *=============================================================================================*/
StaticButtonClass::StaticButtonClass(void) :
	GadgetClass(0, 0, 0, 0, FlagEnum(0)),
	String(NULL),
	PrintFlags(TPF_8POINT)
{
}


/***********************************************************************************************
 * StaticButtonClass::Draw_Me -- Draws the text buttons as indicated.                          *
 *                                                                                             *
 *    This routine will draw the text button.                                                  *
 *                                                                                             *
 * INPUT:   forced   -- If the button is to be redrawn regardless of the state of the redraw   *
 *                      flag, then this parameter will be true.                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the button redrawn?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/03/1995 MML : Created.                                                                 *
 *   01/16/1995 JLB : Modified                                                                 *
 *=============================================================================================*/
int StaticButtonClass::Draw_Me(int forced)
{
	if (GadgetClass::Draw_Me(forced)) {
		/*
		**	Hide the mouse.
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Hide_Mouse(X, Y, X+Width-1, Y+Height-1);
		}

		/*
		**	Draw the background and overlaying text. These are virtual function
		**	calls so that they may be overridden.
		*/
		Draw_Background();
		Draw_Text(String);

		/*
		**	Display the mouse.
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Show_Mouse();
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * StaticButtonClass::Set_Text -- Assigns a new text string to this button.                    *
 *                                                                                             *
 *    Use this routine to assign a new text string to this button. By using this function it   *
 *    is possible to dynamically change the button's text. An example of this would be an      *
 *    on/off button that every time it is clicked, the text toggles between "on" and "off".    *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text string to assign to this button.                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The text is NOT copied to this button. You must make sure that the text         *
 *             remains valid throughout the lifetime of this text button.                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void StaticButtonClass::Set_Text(char const * text, bool resize)
{
	if (String != NULL) {
		delete [] String;
		String = NULL;
	}

	if (text != NULL) {
		String = new char[strlen(text)+1];
		if (String != NULL) {
			strcpy(String, text);
		}
	}

	Flag_To_Redraw();
	if (resize && String != NULL) {
		Draw_Background();
		//PG_TO_FIX
		//Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, PrintFlags);
		Width = String_Pixel_Width(String);
		Height = FontHeight + FontYSpacing;
		Background = Buffer();
	}
}


/***********************************************************************************************
 * StaticButtonClass::Draw_Background -- Draws the background to the text button.              *
 *                                                                                             *
 *    This localizes the drawing of the background for the text button. By overriding this     *
 *    function you can give a different background to the button. The text is drawn using      *
 *    a different routine. The mouse is hidden, if necessary, before this routine is called.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void StaticButtonClass::Draw_Background(void)
{
	/*
	**	If the background hasn't been recorded from the buffer, then
	**	allocate and record the background image now.
	*/
	if (Background.Get_Buffer() == NULL && Width > 0 && Height > 0) {
		new(&Background) Buffer(Width*Height);
		if (Background.Get_Buffer() != NULL) {
			LogicPage->To_Buffer(X, Y, Width, Height, Background, Background.Get_Size());
		}
	}

	/*
	**	If there is a background image present, then restore it to the buffer now.
	*/
	if (Background.Get_Buffer() != NULL && LogicPage->Lock()) {
		Buffer_To_Page(X, Y, Width, Height, Background, *LogicPage);
		LogicPage->Unlock();
	}
}


/***********************************************************************************************
 * StaticButtonClass::Draw_Text -- This draws the text for the text button.                    *
 *                                                                                             *
 *    This routine draws the text for the text button. You can override this routine if you    *
 *    wish different text rendering styles or colors. The background has already been drawn    *
 *    by the time this function is called. The mouse is hidden, if necessary, as well.         *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text string to print over the button.                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void StaticButtonClass::Draw_Text(char const * text)
{
	/*
	**	Display the text.
	*/
	if (String != NULL) {
		int x = X;

		if (PrintFlags & TPF_CENTER) {
			x += Width/2;
		}
		if (PrintFlags & TPF_RIGHT) {
			x += Width-1;
		}

		Fancy_Text_Print(text, x, Y, GadgetClass::Get_Color_Scheme(), TBLACK, PrintFlags);
	}
}
