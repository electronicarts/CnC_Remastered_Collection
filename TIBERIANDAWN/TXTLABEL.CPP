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

/* $Header:   F:\projects\c&c\vcs\code\txtlabel.cpv   1.9   16 Oct 1995 16:49:44   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TXTLABEL.H                                                   *
 *                                                                                             *
 *                   Programmer : Bill Randolph                                                *
 *                                                                                             *
 *                   Start Date : 02/06/95                                                     *
 *                                                                                             *
 *                  Last Update : February 6, 1995 [BR]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


/***********************************************************************************************
 * TextLabelClass -- Constructor                                                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      txt         pointer to text buffer to print from                                       *
 *      x            x-coord for text printing                                                 *
 *      y            y-coord for text printing                                                 *
 *      color         color to print in                                                        *
 *      style         style to print (determines the meaning of x & y)                         *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/24/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
TextLabelClass::TextLabelClass(char *txt, int x, int y, int color,
 TextPrintType style) : GadgetClass(x,y,1,1,0,0)
{
	Text = txt;
	Color = color;
	Style = style;
	UserData = 0;
	PixWidth = -1;
	Segments = 0;
}


/***********************************************************************************************
 * Draw_Me -- Graphical update routine                                                         *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      forced      true = draw regardless of the current redraw flag state                    *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      true = gadget was redrawn, false = wasn't                                              *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/24/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
int TextLabelClass::Draw_Me(int forced)
{
	if (GadgetClass::Draw_Me(forced)) {
		if (PixWidth == -1) {
			Fancy_Text_Print("%s", X, Y, Color, TBLACK, Style, Text);
		} else {
			Conquer_Clip_Text_Print(Text, X, Y, Color, TBLACK, Style, PixWidth);
		}
		return(true);
	}
	return(false);
}
