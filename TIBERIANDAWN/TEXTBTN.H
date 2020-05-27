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

/* $Header:   F:\projects\c&c\vcs\code\textbtn.h_v   2.18   16 Oct 1995 16:46:54   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : TEXTBTN.H                                                    * 
 *                                                                                             * 
 *                   Programmer : Joe L. Bostic                                                * 
 *                                                                                             * 
 *                   Start Date : 01/15/95                                                     * 
 *                                                                                             * 
 *                  Last Update : January 15, 1995 [JLB]                                       * 
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef TEXTBTN_H
#define TEXTBTN_H

#include	"toggle.h"


class TextButtonClass : public ToggleClass
{
	public:
		TextButtonClass(void);
		TextButtonClass(unsigned id, char const * text, TextPrintType style, int x, int y, int w=-1, int h=-1, int blackborder=false);
		TextButtonClass(unsigned id, int text, TextPrintType style, int x, int y, int w=-1, int h=-1, int blackborder=false);
		virtual int Draw_Me(int forced=false);
		virtual void Set_Text(char const *text, bool resize = false);
		virtual void Set_Text(int text, bool resize = false);
		virtual void Set_Style (TextPrintType style) {PrintFlags = style;}

	protected:

		virtual void Draw_Background(void);
		virtual void Draw_Text(char const * text);

		unsigned IsBlackBorder:1;

		/*
		**	This points to a constant string that is used for the button's text. 
		*/
		char const * String;

		/*
		**	This is the print flags to use when rendering this button's text.
		*/
		TextPrintType PrintFlags;
};

#endif

