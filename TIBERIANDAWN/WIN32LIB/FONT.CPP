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

						/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : LIBRARY                                  *
 *                                                                         *
 *                    File Name : FONT.C                                   *
 *                                                                         *
 *                   Programmer : David Dettmer                            *
 *                                                                         *
 *                  Last Update : July 20, 1994   [SKB]                    *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Char_Pixel_Width -- Return pixel width of a character.						*
 *   String_Pixel_Width -- Return pixel width of a string of characters.   *
 *   Get_Next_Text_Print_XY -- Calculates X and Y given ret value from Text_P*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "font.h"
#include <malloc.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <string.h>
#include <wwstd.h>


/***************************************************************************
 * CHAR_PIXEL_WIDTH -- Return pixel width of a character.						*
 *                                                                         *
 *    Retreives the pixel width of a character from the font width block.	*
 *                                                                         *
 * INPUT:      Character.																	*
 *                                                                         *
 * OUTPUT:     Pixel width of a string of characters.                      *
 *                                                                         *
 * WARNINGS:   Set_Font must have been called first.                       *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/31/1992 DRD : Created.                                             *
 *   06/30/1994 SKB : Converted to 32 bit library.                         *
 *=========================================================================*/
int __cdecl Char_Pixel_Width(char chr)
{
	int	width;

	width = (unsigned char)*(FontWidthBlockPtr + (unsigned char)chr) + FontXSpacing;

	return(width);
}


/***************************************************************************
 * STRING_PIXEL_WIDTH -- Return pixel width of a string of characters.     *
 *                                                                         *
 *    Calculates the pixel width of a string of characters.  This uses     *
 *		the font width block for the widths.											*
 *                                                                         *
 * INPUT:      Pointer to string of characters.                            *
 *                                                                         *
 * OUTPUT:     Pixel width of a string of characters.                      *
 *                                                                         *
 * WARNINGS:   Set_Font must have been called first.                       *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/30/1992 DRD : Created.                                             *
 *   01/31/1992 DRD : Use Char_Pixel_Width.                                *
 *   06/30/1994 SKB : Converted to 32 bit library.                         *
 *=========================================================================*/
unsigned int __cdecl String_Pixel_Width(char const *string)
{
	WORD	width;				// Working accumulator of string width.
	WORD	largest = 0;		// Largest recorded width of the string.

	if (!string) return(0);

	width = 0;
	while (*string) {
		if (*string == '\r') {
			string++;
			largest = MAX(largest, width);
			width = 0;
		} else {
			width += Char_Pixel_Width(*string++);	// add each char's width
		}
	}
	largest = MAX(largest, width);
	return(largest);
}



/***************************************************************************
 * GET_NEXT_TEXT_PRINT_XY -- Calculates X and Y given ret value from Text_P*
 *                                                                         *
 *                                                                         *
 * INPUT:   VVPC& vp - viewport that was printed to.                       *
 *          unsigned long offset - offset that Text_Print returned.                *
 *          INT *x - x return value.                                       *
 *          INT *y - y return value.                                       *
 *                                                                         *
 * OUTPUT:  x and y are set.                                               *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/20/1994 SKB : Created.                                             *
 *=========================================================================*/
VOID __cdecl Get_Next_Text_Print_XY(GraphicViewPortClass& gp, unsigned long offset, INT *x, INT *y)
{
	INT	buffwidth;

	if (offset) {
		buffwidth = gp.Get_Width() + gp.Get_XAdd();
		offset -= gp.Get_Offset();
		*x = offset % buffwidth;
		*y = offset / buffwidth;
	} else {
		*x = *y = 0;
	}
}
