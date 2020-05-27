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
 **     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Font and text print 32 bit library       *
 *                                                                         *
 *                    File Name : FONT.H                                   *
 *                                                                         *
 *                   Programmer : Scott K. Bowen                           *
 *                                                                         *
 *                   Start Date : June 27, 1994                            *
 *                                                                         *
 *                  Last Update : June 29, 1994   [SKB]                    *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   VVPC::Text_Print -- Text print into a virtual viewport.               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef FONT_H
#define FONT_H

#ifndef GBUFFER_H
#include <gbuffer.h>
#endif


//////////////////////////////////////// Defines //////////////////////////////////////////

// defines for font header, offsets to block offsets

#define FONTINFOBLOCK			4
#define FONTOFFSETBLOCK			6
#define FONTWIDTHBLOCK			8
#define FONTDATABLOCK			10
#define FONTHEIGHTBLOCK			12

// defines for font info block

#define FONTINFOMAXHEIGHT		4
#define FONTINFOMAXWIDTH		5

//////////////////////////////////////// Prototypes //////////////////////////////////////////

/*=========================================================================*/
/* The following prototypes are for the file: SET_FONT.CPP						*/
/*=========================================================================*/

void  * __cdecl Set_Font(void const *fontptr);

/*=========================================================================*/
/* The following prototypes are for the file: FONT.CPP							*/
/*=========================================================================*/

int __cdecl Char_Pixel_Width(char chr);
unsigned int __cdecl String_Pixel_Width(char const *string);
void __cdecl Get_Next_Text_Print_XY(GraphicViewPortClass& vp, unsigned long offset, INT *x, INT *y);

/*=========================================================================*/
/* The following prototypes are for the file: LOADFONT.CPP	  					*/
/*=========================================================================*/

void * __cdecl Load_Font(char  const *name);

/*=========================================================================*/
/* The following prototypes are for the file: TEXTPRNT.ASM	  					*/
/*=========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif


void __cdecl Set_Font_Palette_Range(void const *palette, INT start_idx, INT end_idx);


#ifdef __cplusplus
}
#endif

/*=========================================================================*/





//////////////////////////////////////// External varables ///////////////////////////////////////
extern "C" int FontXSpacing;
extern "C" int FontYSpacing;
extern char FontWidth ;
extern char FontHeight;
extern char *FontWidthBlockPtr;


extern "C" void const *FontPtr;




#endif // FONT_H
