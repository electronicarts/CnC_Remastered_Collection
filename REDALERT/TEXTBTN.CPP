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

/* $Header: /CounterStrike/TEXTBTN.CPP 1     3/03/97 10:25a Joe_bostic $ */
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
 *   TextButtonClass::Draw_Background -- Draws the background to the text button.              *
 *   TextButtonClass::Draw_Me -- Draws the text buttons as indicated.                          *
 *   TextButtonClass::Draw_Text -- This draws the text for the text button.                    *
 *   TextButtonClass::Set_Text -- Assigns a new text string to this button.                    *
 *   TextButtonClass::Set_Text -- Sets the text for this text button.                          *
 *   TextButtonClass::TextButtonClass -- Normal constructor for a text button.                 *
 *   TextButtonClass::TextButtonClass -- Normal constructor for a text button.                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include "textbtn.h"


/***********************************************************************************************
 * TextButtonClass::TextButtonClass -- Normal constructor for a text button.                   *
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
 *          border-- If the button is to be surrounded by a black border, then this flag       *
 *                   should be set to true.                                                    *
 *                                                                                             *
 * OUTPUT:   none                                                                              *
 *                                                                                             *
 * WARNINGS: Call Set_Text & Set_Style, & init X,Y,Width,Height,ID before using this button.	  *
 *                                                                                             *
 * HISTORY:  01/15/1995 JLB : Created.                                                         *
 *=============================================================================================*/
TextButtonClass::TextButtonClass(unsigned id, char const * text, TextPrintType style, int x, int y, int w, int h, int blackborder) :
	ToggleClass(id, x, y, w, h),
	String(text)
{
	PrintFlags = style;
	IsBlackBorder = blackborder;

	if (w == -1 || h == -1) {
		//PG_TO_FIX
		//Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, PrintFlags);
		if (w == -1) {
			Width = String_Pixel_Width(String)+8;
		}
		if (h == -1) {
			Height = FontHeight + FontYSpacing + 2;
		}
	}
}


/***********************************************************************************************
 * TextButtonClass::TextButtonClass -- Default constructor for a text button.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:   none                                                                              *
 *                                                                                             *
 * WARNINGS: none                                                                              *
 *                                                                                             *
 * HISTORY:  01/15/1995 JLB : Created.                                                         *
 *=============================================================================================*/
TextButtonClass::TextButtonClass(void) :
	ToggleClass(0, 0, 0, 0, 0)
{
	IsBlackBorder = false;
	String = NULL;
	PrintFlags = TPF_8POINT;
}


/***********************************************************************************************
 * TextButtonClass::TextButtonClass -- Normal constructor for a text button.                   *
 *                                                                                             *
 *    This is the constructor for text buttons if the text is provided as a string pointer.    *
 *                                                                                             *
 * INPUT:   id    -- The ID number to assign to this button.                                   *
 *                                                                                             *
 *          text  -- The text number to use for displaying on top of the button.               *
 *                                                                                             *
 *          x,y   -- Pixel coordinate of button's upper left corner.                           *
 *                                                                                             *
 *          w,h   -- Dimensions of the button. If these are not filled in (or with -1), then   *
 *                   the dimensions are adapted to fit the text assigned to the button.        *
 *                                                                                             *
 *                                                                                             *
 *          style -- The print style for the text in the button. These are the TPF_ flags      *
 *                   used by Fancy_Text_Print().                                               *
 *                                                                                             *
 *          border-- If the button is to be surrounded by a black border, then this flag       *
 *                   should be set to true.                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TextButtonClass::TextButtonClass (unsigned id, int text, TextPrintType style, int x, int y, int w, int h, int blackborder) :
	ToggleClass (id, x, y, w, h),
	String(0)
{
	PrintFlags = style;
	IsBlackBorder = blackborder;
	Set_Text(text);

	if (w == -1 || h == -1) {
		//PG_TO_FIX
		//Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, PrintFlags);
		if (w == -1) {
			Width = String_Pixel_Width(String)+8;
		}
		if (h == -1) {
			Height = FontHeight + FontYSpacing + 2;
		}
	}
}


/***********************************************************************************************
 * TextButtonClass::Draw_Me -- Draws the text buttons as indicated.                            *
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
int TextButtonClass::Draw_Me(int forced)
{
	if (ControlClass::Draw_Me(forced)) {
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
 * TextButtonClass::Set_Text -- Assigns a new text string to this button.                      *
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
void TextButtonClass::Set_Text(char const * text, bool resize)
{
	String = text;
	Flag_To_Redraw();
	if (resize && String) {
		//PG_TO_FIX
		//Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, PrintFlags);
		Width = String_Pixel_Width(String)+8;
		Height = FontHeight + FontYSpacing + 2;
	}
}


/***********************************************************************************************
 * TextButtonClass::Set_Text -- Sets the text for this text button.                            *
 *                                                                                             *
 *    This will set the text for this button. The text is provided as a text number. This      *
 *    number is automatically converted to the appropriate text string before being stored     *
 *    in the button's structure.                                                               *
 *                                                                                             *
 * INPUT:   text  -- The text number to assign to this button.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The text number information is lost when it is assigned to the button. Once     *
 *             the assignment takes place, the text number is NOT remembered by the button.    *
 *             Only the associated text string is.                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TextButtonClass::Set_Text(int text, bool resize)
{
	if (text != TXT_NONE) {
		Set_Text(Text_String(text), resize);
	}
}


/***********************************************************************************************
 * TextButtonClass::Draw_Background -- Draws the background to the text button.                *
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
void TextButtonClass::Draw_Background(void)
{
	/*
	**	Draw a border if selected style.
	*/
	if (IsBlackBorder) {
		LogicPage->Draw_Rect (X-1, Y-1, X+Width+2, Y+Height+2, BLACK);
	}

	/*
	**	Draw the body & set text color.
	*/
	BoxStyleEnum style;

	if (IsDisabled) {
#ifdef WOLAPI_INTEGRATION
		style = BOXSTYLE_BOX;
#else
		style = BOXSTYLE_DIS_RAISED;
#endif
	} else {
		if (IsPressed) {
			style = BOXSTYLE_DOWN;
		} else {
			style = BOXSTYLE_RAISED;
		}
	}

	Draw_Box(X, Y, Width, Height, style, true);
}


/***********************************************************************************************
 * TextButtonClass::Draw_Text -- This draws the text for the text button.                      *
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
void TextButtonClass::Draw_Text(char const * text)
{
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	/*
	**	Display the text.
	*/
	if (String) {
		TextPrintType flags;

		if (IsDisabled) {
			flags = (TextPrintType)0;
		} else {
			if (IsPressed || IsOn) {
				flags = TPF_USE_GRAD_PAL|TPF_BRIGHT_COLOR;
			} else {
				flags = TPF_USE_GRAD_PAL|TPF_MEDIUM_COLOR;
			}
		}

		Fancy_Text_Print(text, X+(Width>>1)-1, Y+1, scheme, TBLACK, PrintFlags|flags|TPF_CENTER);
	}
}
