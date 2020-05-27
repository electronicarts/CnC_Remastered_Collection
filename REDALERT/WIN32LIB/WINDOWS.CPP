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

/* $Header: g:/library/source/rcs/./windows.c 1.12 1994/05/20 15:35:25 joe_bostic Exp $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : LIBRARY                                  *
 *                                                                         *
 *                    File Name : WINDOWS.C                                *
 *                                                                         *
 *                   Programmer : Everyone                                 *
 *                                                                         *
 *                  Last Update : February 3, 1992   [DRD]                 *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Change_New_Window -- Combined Change_Window and New_Window.           *
 *   Change_Window -- Changes the 'current' window in the system.          *
 *   Fetch_Char -- Gets one undipthonged char from input.                  *
 *   Flush_Line -- Outputs the accumulate text line to screen.             *
 *   In_Char -- Stores (un-dipped) character(s) from input to buffer.      *
 *   New_Window -- Clears the current window to the background color.      *
 *   Set_More_Off -- Turns the 'more' prompting off.                       *
 *   Set_More_On -- Turns the 'more' prompting on.                         *
 *   Set_More_Prompt -- Adjusts the more prompt text for default routine   *
 *   Standard_More_Prompt -- Default more prompt code for Window_Print     *
 *   Window_Int_Print -- Prints an integer to the window.                  *
 *   Window_Print -- Displays and wraps text into a window.                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include	<stdio.h>
#include	<stdarg.h>
#include <wwstd.h>
#include "ww_win.h"
#include <keyboard.h>
#include	<font.h>
#include <dipthong.h>

PRIVATE void Scroll_Window(void);
PRIVATE void Flush_Line(void);
PRIVATE void In_Char(char *str);
PRIVATE char Fetch_Char(void);


PRIVATE int ScrollCounter = 0;	//	Count of the lines displayed before a pause.
PRIVATE char Line[84];	// Staging line buffer.
PRIVATE int Pos;			// Char Position of next free character.
PRIVATE int PPos;		// Pixel position of next free character.
PRIVATE int WPos;		// Char position in window.
PRIVATE char *MainSource;
PRIVATE char *AltSource;
PRIVATE char Char[2];
PRIVATE char Stack;
PRIVATE char WordWrapFlag = FALSE;	// flag for a word wrap.

PRIVATE int MoreSpace = 7;
PRIVATE int MoreFColor = 0;
PRIVATE int MoreBColor = 0;


int WindowColumns=40;
int WindowLines=25;
int WindowWidth=40;
unsigned int WinB=0;
unsigned int WinC=1;
unsigned int WinX=0;
unsigned int WinY=0;
unsigned int WinCx=0;
unsigned int WinCy=0;
unsigned int WinH=25;
unsigned int WinW=40;
unsigned int Window=0;

int MoreOn = TRUE;
char *TXT_MoreText = "--More--";
void (*Window_More_Ptr)(char const *,int,int,int) = Standard_More_Prompt;

extern GraphicViewPortClass *LogicPage;
/***************************************************************************
 * STANDARD_MORE_PROMPT -- Default more prompt code for Window_Print       *
 *                                                                         *
 *    This is the standard "<more>" prompting code that is used by         *
 *    Window_Print when a page is full of text and a pause is desired      *
 *    before the next page of text is printed.  This function is called    *
 *    through the Window_More_Ptr global.                                  *
 *                                                                         *
 * INPUT:   prompt   -- Pointer to ASCII text that will be window printed  *
 *                      at the right margin.                               *
 *                                                                         *
 *          space    -- The number of spaces to allow for the 'more' text. *
 *                                                                         *
 *          fcolor   -- The foreground color to use for the 'more' text.   *
 *                                                                         *
 *          bcolor   -- The background oclor to use for the 'more' text.   *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/29/1991 JLB : Created.                                             *
 *=========================================================================*/
void Standard_More_Prompt(char const *prompt, int space, int fcolor, int bcolor)
{
	int	x, y, moresize;

	moresize = (space - 1) * (FontWidth+FontXSpacing);
	x = ((WinX+WinW) << 3) - moresize;
	//y = WinY + ((WinH/FontHeight)-1)*FontHeight;
	y = WinY + (WinCy-1) * (FontHeight+FontYSpacing);

	// Default "more" prompter.
	LogicPage->Print(prompt, x, y, fcolor ? fcolor : WindowList[Window][WINDOWBCOL], bcolor ? bcolor : WindowList[Window][WINDOWFCOL]);
//BG	if (LogicPage == SEENPAGE) {
//BG		Window_Show_Mouse();
//BG	}

	// PWG - have to figure out how to do this in windows library

//	Clear_KeyBuffer();
//	Get_Key();

//BG	if (LogicPage == SEENPAGE) {
//BG		Window_Hide_Mouse(Window);
//BG	}

	// Erase the more prompt prompt.
	//	Text_Print(prompt, x, y, WinB, WinB);
	LogicPage->Fill_Rect(x, y, x + moresize - 1, y + (FontHeight+FontYSpacing) - 1, (unsigned char)WinB);
}


/***************************************************************************
 * SET_MORE_PROMPT -- Adjusts the more prompt text for default routine     *
 *                                                                         *
 *    Use this routine to control the text of the "<MORE>" prompt that     *
 *    the default more prompt routine uses.  This can be useful for        *
 *    foreign language translations.                                       *
 *                                                                         *
 * INPUT:   prompt   -- Pointer to ASCII text that will be window printed  *
 *                      at the right margin.                               *
 *                                                                         *
 *          space    -- The number of spaces to allow for the 'more' text. *
 *                                                                         *
 *          fcolor   -- The foreground color to use for the 'more' text.   *
 *                                                                         *
 *          bcolor   -- The background color to use for the 'more' text.   *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/29/1991 JLB : Created.                                             *
 *=========================================================================*/
void Set_More_Prompt(char const *prompt, int space, int fcolor, int bcolor)
{
	if (prompt) {
		TXT_MoreText = (char*)prompt;
		MoreSpace = space;
		MoreFColor = fcolor;
		MoreBColor = bcolor;
	}
	else {
		TXT_MoreText = "<MORE>";
		MoreSpace = 7;
		MoreFColor = MoreBColor = 0;
	}
}


/***************************************************************************
 * SET_MORE_ON -- Turns the 'more' prompting on.                           *
 *                                                                         *
 *    Use this routine to turn on the 'more' prompting that Window_Print   *
 *    does.  If you have a custom more function pointer, then that         *
 *    routine will be called, otherwise the library default 'more' prompt  *
 *    will be utilized.                                                    *
 *                                                                         *
 * INPUT:      none                                                        *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *=========================================================================*/
void Set_More_On(void)
{
	MoreOn = TRUE;
	ScrollCounter = 0;
}


/***************************************************************************
 * SET_MORE_OFF -- Turns the 'more' prompting off.                         *
 *                                                                         *
 *    This routine will turn the 'more' prompting that Window_Print does   *
 *    off.                                                                 *
 *                                                                         *
 * INPUT:   none                                                           *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *=========================================================================*/
void Set_More_Off(void)
{
	MoreOn = FALSE;
}


/***************************************************************************
 * CHANGE_WINDOW -- Changes the 'current' window in the system.            *
 *                                                                         *
 *    Use this routine to change the 'current' window.  The current window *
 *    is used in Window_Print and some other graphic output routines.      *
 *                                                                         *
 * INPUT:   windnum  -- The window number to change to.                    *
 *                                                                         *
 * OUTPUT:  Returns with the previously current window.                    *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *=========================================================================*/
int Change_Window(int windnum)
{
	int	oldwindow;
	int	*data;

	oldwindow = Window;
	Window = windnum;
	data = &WindowList[windnum][0];

	WinX = *data++;
	WinY = *data++;
	WinW = *data++;
	WinH = *data++;
	WinC = *data++;
	WinB = *data++;
	WinCx = *data++;
	WinCy = *data++;
	ScrollCounter = 0;
	WPos = WinCx / (FontWidth+FontXSpacing);
	WindowLines = (WinH-FontYSpacing) / (FontHeight+FontYSpacing);
	WindowWidth = WinW << 3;
	WindowColumns = WindowWidth / (FontWidth+FontXSpacing);
	return (oldwindow);
}


/***************************************************************************
 * CHANGE_NEW_WINDOW -- Combined Change_Window and New_Window.             *
 *                                                                         *
 *    This is a combo-routine.  It merely combines the Change_Window       *
 *    with the New_Window routines.  It will save some (small) code if     *
 *    you use this routine instead of the two function calls.              *
 *                                                                         *
 * INPUT:   window   -- Window number to change to and clear.              *
 *                                                                         *
 * OUTPUT:  Returns with the previously current window.                    *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *=========================================================================*/
int Change_New_Window(int windnum)
{
	int	oldwindow;

	oldwindow = Change_Window(windnum);
	New_Window();
	return(oldwindow);
}


/***************************************************************************
 * NEW_WINDOW -- Clears the current window to the background color.        *
 *                                                                         *
 *    This routine clears the current window to the background color.  It  *
 *    is used in preparation to Window_Print because it ensures a clean    *
 *    'slate' for the text.                                                *
 *                                                                         *
 * INPUT:   none                                                           *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *=========================================================================*/
void New_Window(void)
{
	int	x,y,w,h;

	x = WinX << 3;
	y = WinY;
	w = (WinX + WinW) << 3;
	h = WinY + WinH;

	LogicPage->Fill_Rect(x, y, w - 1, h - 1, (unsigned char)WinB);

	WinCx = WPos = 0;
	WinCy = 0;
	ScrollCounter = 0;
}


/***************************************************************************
 * WINDOW_INT_PRINT -- Prints an integer to the window.                    *
 *                                                                         *
 *    Use this routine to print an integer to the window.  This routine    *
 *    as all other Window printing routines will handle word wrap.         *
 *                                                                         *
 * INPUT:   num   -- The integer to convert to ASCII and print.            *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *=========================================================================*/
void Window_Int_Print(int num)
{
	Window_Print("%d", num);
}


/***************************************************************************
 * WINDOW_PRINT -- Displays and wraps text into a window.                  *
 *                                                                         *
 *    This is the general purpos text output routine that will handle      *
 *    word wrap within a window.  It is useful for displaying arbitrary    *
 *    text.  This routine will handle dipthonged text and as such it       *
 *    can be quite useful in saving memory.                                *
 *                                                                         *
 * INPUT:   string   -- String to print.  This can be of ANY length and    *
 *                      can even contain some formatting codes.  The       *
 *                      codes supported are:                               *
 *                                                                         *
 *             KA_SETX        Forces the cursor X position to the value    *
 *                            specified.                                   *
 *                                                                         *
 *             KA_SETY        Forces the cursor Y position to the value    *
 *                            specified.                                   *
 *                                                                         *
 *             KA_MORE        Causes an immediate "<MORE>" prompt          *
 *                            regardless of the scroll situation.          *
 *                                                                         *
 *             KA_RETURN      Breaks line and continues output at the      *
 *                            left edge of following line.                 *
 *                                                                         *
 *                                                                         *
 *             KA_FORMFEED    Clears the window and continues printing at  *
 *                            the upper left corner.                       *
 *                                                                         *
 *             KA_SETBKGDCOL  Set the background color with the color      *
 *                            specified by the following byte.             *
 *                                                                         *
 *                                                                         *
 *             KA_SETFORECOL  Set the foreground color with the color      *
 *                            specified by the following byte.             *
 *                                                                         *
 *             KA_TAB         Move the cursor over to the next tabstop.    *
 *                            Tabstops are set every 8 columns.            *
 *                                                                         *
 *             KA_SPCTAB      Insert spaces until the cursor is positioned *
 *                            at the next tabstop.                         *
 *                                                                         *
 *             %s             Replace the "%s" with the text pointed to    *
 *                            by the pointer argument passed to the        *
 *                            routine (follows same method a printf).      *
 *                                                                         *
 *             %d             Replace the "%d" with an integer ASCII       *
 *                            number of the int passed to the routine.    *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *   07/29/1991 JLB : Added MORE, SETX, and SETY                           *
 *=========================================================================*/
void Window_Print(char const string[], ...)
{
	int		oldcx, x, y;	// Scratch variables.
	char		c;					// Current character.
	char		buffer[10];		// Working %d buffer.
	int		old_c, old_b;	// Original window colors.
	va_list	arg;				// Argument list var.


	va_start(arg, string);

	WordWrapFlag = FALSE;			// initialize word wrap flag.
	Pos = PPos = 0;
	Line[0] = '\0';
	Char[0] = Char[1] = 0;
	MainSource = (char*)&string[0];
	AltSource = NULL;
	old_c = WinC;
	old_b = WinB;

//BG	if (LogicPage == SEENPAGE) {
//BG		Window_Hide_Mouse(Window);
//BG	}

	while (TRUE) {

		c = Fetch_Char();

		if (!c) break;	// Exit on NULL character.

		/*
		**	Substitution commands only work if not already expanding a
		**	string.
		*/
		if (!AltSource) {
			if (c == '%') {
		 		switch(tolower(Char[0])) {
					case 's':
						AltSource = va_arg(arg, char*);
						if (AltSource) {
							Stack = Char[1];
							Char[0] = Char[1] = '\0';
							c = Fetch_Char();
						}
						break;

					case 'd':
						AltSource = buffer;
						sprintf(buffer, "%d", va_arg(arg, int));
						Stack = Char[1];
						Char[0] = Char[1] = '\0';
						c = Fetch_Char();
						break;

					default:
						break;
				}
			}
		}

		switch(c) {

#if(FALSE)
	// these are the positions of foreign language characters
			/*
			** These are characters that shouldn't be window printed because
			**	they are currently reserved.
			*/
			case KA_CTRL_C:
			case KA_CTRL_D:
			case KA_CTRL_E:
			case KA_CTRL_G:
			case KA_CTRL_J:
			case KA_CTRL_K:
			case KA_CTRL_N:
			case KA_CTRL_O:
			case KA_CTRL_P:
			case KA_CTRL_Q:
			case KA_CTRL_R:
			case KA_CTRL_T:
			case KA_CTRL_U:
			case KA_CTRL_V:
			case KA_CTRL_W:
			case KA_CTRL_Z:
			case KA_CTRL_BACKSLASH:
			case KA_CTRL_CARROT:
			case KA_CTRL_UNDERLINE:
				break;
#endif
			/*
			**	Force cursor column to specified X value.
			*/
			case KA_SETX:
				Flush_Line();
				WPos  = Fetch_Char();
				WPos  = MAX(0, WPos);

				// WPos is max width char position

				WPos  = MIN(WindowColumns-1, WPos);
				WinCx = WPos * (FontWidth+FontXSpacing);
				break;

			/*
			**	Force the cursor to specified Y value.
			*/
			case KA_SETY:
				Flush_Line();
				WinCy = Fetch_Char();
				//WinCy = MAX(0, WinCy);
				WinCy = MIN((long)WindowLines-1, (long)WinCy);
				break;

			/*
			**	Force a "<MORE>" prompt.
			*/
			case KA_MORE:
				Flush_Line();
				if (Window_More_Ptr) {
//BG					if (LogicPage == SEENPAGE) Window_Show_Mouse();
					Window_More_Ptr(TXT_MoreText, MoreSpace, MoreFColor, MoreBColor);
//BG					if (LogicPage == SEENPAGE) Window_Hide_Mouse(Window);
				}
				break;

			/*
			**	Clear and home the window cursor.  This is the same
			**	as New_Window().
			*/
			case KA_FORMFEED:
				New_Window();
				break;

			/*
			**	Move cursor to start of next line.
			*/
			case KA_RETURN:
				Flush_Line();
				ScrollCounter++;
				WinCx = 0;

#if(FALSE)
				if (WinCy >= WindowLines-1) {
					Scroll_Window();
				}
				else {
					WinCy++;
				}
#else
	  			WinCy++;
#endif

				break;

			/*
			**	Set the background color.
			*/
			case KA_SETBKGDCOL:
				Flush_Line();
				WinB = Fetch_Char();
				break;

			/*
			**	Set the foreground color.
			*/
			case KA_SETFORECOL:
				Flush_Line();
				WinC = Fetch_Char();
				break;

			/*
			**	Move cursor to next column.
			*/
			case KA_TAB:
				Flush_Line();
				WPos = ((WPos + 8) & 0xFFF8) - 1;
				if (WPos >= WindowColumns) {
					WPos = 0;
				}
				WinCx = WPos * (FontWidth+FontXSpacing);
				break;

			/*
			**	Tab to specified column but add spaces.
			*/
			case KA_SPCTAB:
				Flush_Line();
				oldcx = WinCx;
				x = WinX << 3;
				y = WinY + (WinCy * (FontHeight+FontYSpacing));
				WPos = ((WPos + 8) & 0xFFF8) - 1;

				if (WPos >= WindowColumns) {
					WinCx = WPos = 0;

					// Fill_Rect instead of printing spaces

					LogicPage->Fill_Rect(x + oldcx, y,
								x + WindowWidth - 1, y + (FontHeight+FontYSpacing) - 1, (unsigned char)WinB);

					ScrollCounter++;
		  			WinCy++;
				}
				else {
					WinCx = WPos * (FontWidth+FontXSpacing);

					// Fill_Rect instead of printing spaces

					LogicPage->Fill_Rect(x + oldcx, y,
								x + WinCx - 1, y + (FontHeight+FontYSpacing) - 1, (unsigned char)WinB);
				}
				break;

			/*
			**	next character is a extended value 1-127, but 128 is added
			** for a value 129-255
			*/
			case KA_EXTEND:
				c = 127;

			// NOTE: this falls thru to the default case DO NOT MOVE!!!!!


			/*
			**	next character is a literal value 1-127, except 13
			*/
//			case KA_LITERAL:
//				if (c != (char) 127) {	// check if fell thru from extend case
//					c = 0;					// set to zero for literal case
//				}
//				c += Fetch_Char();

			// NOTE: this falls thru to the default case DO NOT MOVE!!!!!


			/*
			**	Normal character output.
			*/
			default:
				PPos += Char_Pixel_Width(c);	// get pixel location of next char
				Line[Pos++] = c;
				Line[Pos] = '\0';

				if (WinCx + PPos > (unsigned)WindowWidth) {
					Flush_Line();
				}
				break;
		}
	}

	/*
	**	If there is text still pending, then display it before exiting.
	*/
	if (Pos) Flush_Line();

	/*
	**	Record changes in the cursor position.
	*/
	WindowList[Window][WINDOWCURSORX] = WinCx;
	WindowList[Window][WINDOWCURSORY] = WinCy;

	/*
	**	Restore the window colors to their original values.
	*/
	WindowList[Window][WINDOWFCOL] = WinC = old_c;
	WindowList[Window][WINDOWBCOL] = WinB = old_b;

//BG	if (LogicPage == SEENPAGE) {
//BG		Window_Show_Mouse();
//BG	}

	va_end(arg);
}


/***************************************************************************
 * SCROLL_WINDOW -- Scrolls the text window up one line.                   *
 *                                                                         *
 *    This will scroll the text window up one line.  It will handle any    *
 *    pausing for "more" if the MoreOn flag is set.                        *
 *                                                                         *
 * INPUT:   none                                                           *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   This routine assumes that the LogicPage is the SEENPAGE.    *
 *             If this is not the case, the program may appear to hang     *
 *             if a "more" prompt is generated.                            *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *=========================================================================*/
PRIVATE void Scroll_Window(void)
{
	int	y;		// Top pixel row of bottom line of window.

	/*
	**	Possibly prompt for more text.
	*/
	if (ScrollCounter >= WindowLines-1 && MoreOn) {
		ScrollCounter = 0;

		if (Window_More_Ptr) {
//BG			if (LogicPage == SEENPAGE) Window_Show_Mouse();
			Window_More_Ptr(TXT_MoreText, MoreSpace, MoreFColor, MoreBColor);
//BG			if (LogicPage == SEENPAGE) Window_Hide_Mouse(Window);
		}
	}

	/*
	**	Scroll the window up one line.
	*/
	y = ((WinH / (FontHeight+FontYSpacing)) - 1) * (FontHeight+FontYSpacing);
	LogicPage->Blit(*LogicPage,WinX<<3, WinY + (FontHeight+FontYSpacing), WinX<<3, WinY, WinW<<3, WinH - (FontHeight+FontYSpacing) );
	LogicPage->Fill_Rect(WinX<<3,
				WinY + y,
				((WinX+WinW)<<3) - 1,
				WinY + WinH - 1,
				(unsigned char)WinB);
}


/***************************************************************************
 * FLUSH_LINE -- Outputs the accumulate text line to screen.               *
 *                                                                         *
 *    This will display the accumlated text line to the screen.  It will   *
 *    handle breaking the text line at an appropriate position.            *
 *                                                                         *
 * INPUT:   none                                                           *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *=========================================================================*/
PRIVATE void Flush_Line(void)
{
	int	breakit, breaksize, breakwidth;
	int	x, y;					// Coordinates of text print.
	int	breakpoint;			// Point to break the line (if possible).
	char	breakchar;			// Break replace character.
	int	index;				// Backward moving index var.

	/*
	** There could be a held <CR> and this is implied by the cursor Y position
	** beyond the bottom of the window.  If this is the case, then scroll the
	**	window and proceed with the line flush.
	*/
	while (WinCy >= (unsigned)WindowLines /*&& Pos > 0*/) {
		Scroll_Window();
		if (WinCy >= (unsigned)WindowLines) WinCy--;
	}
	//if (WinCy >= WindowLines) WinCy = WindowLines-1;

	x = (WinX<<3) + WinCx;
	y = WinY + (WinCy*(FontHeight+FontYSpacing));

	breakwidth = WindowWidth;
//	if (ScrollCounter >= WindowLines - 1 && MoreOn) {
//		breakwidth -= (MoreSpace * (FontWidth+FontXSpacing));		// use maximum font width
//	}

	/*
	**	Try to break the line at the last space IF the line has reached the edge
	**	of the window.
	*/
	breakpoint = Pos;
	breaksize = PPos;
	if (WinCx + breaksize > (unsigned)breakwidth) {

		/*
		**	Since the text WILL spill past the edge of the window, determine the
		**	point where the break should occur.  If this line is ready for the <MORE>
		**	prompt, then breaking must account for the <MORE> text.
		*/
		if (ScrollCounter >= WindowLines - 1 && MoreOn) {
			breakwidth -= (MoreSpace * (FontWidth+FontXSpacing));		// use maximum font width
		}
		breakwidth -= WinCx;

		breakit = 0;
		for (index = breakpoint - 1; index > 0; index--) {
			breakchar = Line[index];
			breaksize -= Char_Pixel_Width(breakchar);

			// only once, find largest text that can fit on the line
			if (!breakit) {
				// was this the char that went past the right edge
				if (breaksize <= breakwidth) {
					breakit = index;	// save this position if there is no spaces
				}
			}

			// after largest text is found then look for a space to break on
			if (breakit && breakchar == KA_SPACE) {
				breakpoint = index;
				WordWrapFlag = FALSE; // word will start at beginning of next line
				break;
			}
		}

		/*
		**	Exception: When the current text buffer cannot be broken at a logical
		**	place AND the text is starting past the left margin, THEN there is
		**	an implied break between the previous text output and this one.
		**	Output the current text on the next line left margin.
		*/
		if (!index) {
			if (WinCx && !WordWrapFlag) {
				breakpoint = breaksize = 0;		// Continue text on next line.
				WordWrapFlag = TRUE;		// indicate a word continuation.
			}
			else {
				breakpoint = breakit;	// Just print as much as possible.
			}
		}
	}

	breakchar = Line[breakpoint];
	Line[breakpoint] = '\0';

	LogicPage->Print(Line, x, y, WinC, WinB);
	WinCx += breaksize;					// add size of text string printed.

	Line[breakpoint] = breakchar;
	if (breakchar == KA_SPACE) {		// take out a space between words.
		breakpoint++;
	}

	// take out another space for double spacing after end of sentence.
	if (Line[breakpoint] == KA_SPACE) {
		breakpoint++;
	}

	strcpy(Line, &Line[breakpoint]);
	Pos = strlen(Line);
	PPos = String_Pixel_Width(Line);

	/*
	**	If at this point there is still text in the buffer, then flushing has
	**	not been completed.  Scroll to next line and repeat the text flushing
	**	process.
	*/
	if (Pos || WinCx >= (unsigned)WindowWidth) {
		WinCx = WPos = 0;
		#if(FALSE)
			if (WinCy >= WindowLines-1) {
				Scroll_Window();
			} else {
				WinCy++;
			}
		#else
			WinCy++;
		#endif
		Flush_Line();
		ScrollCounter++;	// must be done after flush line for correct counting
	}
}


/***************************************************************************
 * IN_CHAR -- Stores (un-dipped) character(s) from input to buffer.        *
 *                                                                         *
 *    Use this routine to fetch the next character from the input stream.  *
 *    If the character was dipthonged, then it will be broken into its     *
 *    component ASCII characters and stored in the specified location.     *
 *    This is the core character stream reading code.                      *
 *                                                                         *
 * INPUT:   str   -- char pointer to the position to store the character(s)*
 *                   fetched from the input stream.                        *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *=========================================================================*/
PRIVATE void In_Char(char *str)
{
	char	c;		// Character to return.
	char	next;	// Following character (if any).

	c = next = '\0';

	/*
	**	Fetch a raw byte from the input stream.
	*/
	if (AltSource) {
		if (*AltSource == '\0') {
			AltSource = NULL;
			c = Stack;
		} else {
			c = *AltSource++;
		}
	}

	if (!c && MainSource) {
		if (*MainSource == '\0') {
			MainSource = NULL;
		} else {
			c = *MainSource++;
		}
	}

	/*
	**	Convert a dipthong character into it's component
	**	ASCII characters.
	*/
	if (c & 0x80) {
		c &= 0x7F;
		next = c & (char)0x07;
		c = (char)((c & (char)0x78) >> 3);

		next = Dipthong[c][next];	// Dipthong character.
		c = Common[c];				// Common character.
	}

	*str++ = c;
	*str = next;
}


/***************************************************************************
 * FETCH_CHAR -- Gets one undipthonged char from input.                    *
 *                                                                         *
 *    This routine will fetch one character from the input stream.  The    *
 *    character has already been un-dipthonged.  It is a straight ASCII    *
 *    character.  This routine ensures that if the next character in the   *
 *    input stream needs to be examined, it is available in Char[0].       *
 *                                                                         *
 * INPUT:   none                                                           *
 *                                                                         *
 * OUTPUT:  Returns next character in the input stream (ASCII).  If NULL   *
 *          is returned, then this indicates the end of the input stream.  *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/25/1991 JLB : Created.                                             *
 *=========================================================================*/
PRIVATE char Fetch_Char(void)
{
	char	c;		// Character to return.

	if (!Char[0]) {
		In_Char(&Char[0]);
	}

	c = Char[0];
	Char[0] = Char[1];
	Char[1] = '\0';

	if (!Char[0]) {
		In_Char(&Char[0]);
	}

	return (c);
}

