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
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : wwlib32                                  *
 *                                                                         *
 *                    File Name : PALTOPAL.CPP                             *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : May 2, 1994                              *
 *                                                                         *
 *                  Last Update : May 2, 1994   [BR]                       *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Morph_Palette -- morphs a palette from source to destination          *
 *   Palette_To_Palette -- morph src palette to a dst palette              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*
********************************* Includes **********************************
*/
#include "wwstd.h"
#include "palette.h"
#include "timer.h"

/*
********************************* Constants *********************************
*/
#define SCALE(a,b,c) (((((long)(a)<<8) / (long)(b) ) * (unsigned long)(c)) >>8)


/*
********************************** Globals **********************************
*/

/*
******************************** Prototypes *********************************
*/

PRIVATE int __cdecl Palette_To_Palette(void *src_palette, void *dst_palette, unsigned long current_time, unsigned long delay);


/***************************************************************************
 * Morph_Palette -- morphs a palette from source to destination            *
 *                                                                         *
 * INPUT:                                                                  *
 *		void *src_pal	- starting palette												*
 *		void *dst_pal	- ending palette													*
 *		unsigned int delay	- time delay in 60ths of a second							*
 *		void *callback	- user-defined callback, NULL if none						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/02/1994 BR : Created.                                              *
 *=========================================================================*/
void cdecl Morph_Palette (void *src_pal, void *dst_pal, unsigned int delay,
	void (*callback) (void) )
{
	int	result;
	unsigned long	pal_start = TickCount.Time();
	extern void   (*cb_ptr) ( void ) ;	// callback function pointer

//	(void *)cb_ptr = callback;
	cb_ptr = callback ;

	/*===================================================================*/
	/* Make sure that we don't go too fast but also make sure we keep		*/
	/*		processing the morph palette if we have one.							*/
	/*===================================================================*/
	while (1) {
		if (src_pal && dst_pal) {
			result = Palette_To_Palette (src_pal, dst_pal,
				(TickCount.Time() - pal_start), (unsigned long)delay);
			if (!result)
				break;

			if (callback) {
				(*cb_ptr)();
			}
		}
	}

	return;

}	/* end of Morph_Palette */


/***************************************************************************
 * Palette_To_Palette -- morph src palette to a dst palette                *
 *                                                                         *
 * Creates & sets a palette that's in-between 'src_palette' & 					*
 * 'dst_palette'; how close it is to dst_palette is based on how close 		*
 * 'current_time' is to 'delay'.  'current_time' & 'delay' are based on		*
 * 0 being the start time.																	*
 *                                                                         *
 * INPUT:               void *src_palette =  palette we want to morph from *
 *                      void *dst_palette =  palette we want to morph to   *
 *                      long current_time =  time we started morph pal		*
 *                      long delay         = time we want the morph to take*
 *                                                                         *
 * OUTPUT:					int if the time had elapsed and no chages were		*
 *									  necessary this routine returns FALSE				*
 *									  otherwise it will always return TRUE (this		*
 *									  was necessary to detect the end of the ice		*
 *									  effect.													*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/24/1993  MC : Created.                                             *
 *=========================================================================*/
PRIVATE int cdecl Palette_To_Palette(void *src_palette, void *dst_palette,
	unsigned long current_time, unsigned long delay)
{
	char	colour;
	char	diff;
	int	chgval;
	int	lp;
	int	change;
	static char  palette[768];
	char	*src_pal = (char*)src_palette;
	char	*dst_pal = (char*)dst_palette;

	/*======================================================================*/
	/* Loop through each RGB value attempting to change it to the correct	*/
	/*		color.																				*/
	/*======================================================================*/
	for (change = lp = 0; lp < 768; lp++) {
		if (current_time < delay ) {
			diff		= dst_pal[lp] & (char)63;
			diff	  -= src_pal[lp] & (char)63;
			if (diff)
				change = TRUE;
			chgval	= SCALE(diff, delay, current_time);
			colour 	= src_pal[lp] & (char)63;
			colour	+=(char)chgval;
		}
		else {
			colour = dst_pal[lp] & (char)63;
			change = FALSE;
		}
		palette[lp] = colour;
	}
	/*======================================================================*/
	/* Set the palette to the color that we created.								*/
	/*======================================================================*/
	Set_Palette(palette);
	return(change);

}	/* end of Palette_To_Palette */


/*************************** End of morphpal.cpp ***************************/


