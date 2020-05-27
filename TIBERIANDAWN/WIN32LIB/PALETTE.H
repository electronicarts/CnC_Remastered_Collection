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
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Palette 32bit Library.                   *
;*                                                                         *
;*                    File Name : PALETTE.H                                *
;*                                                                         *
;*                   Programmer : Scott K. Bowen                           *
;*                                                                         *
;*                   Start Date : April 25, 1994                           *
;*                                                                         *
;*                  Last Update : April 27, 1994 [BRR]							*
;*                                                                         *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef PALETTE_H
#define PALETTE_H

#include <gbuffer.h>
/*
********************************* Constants *********************************
*/
#define RGB_BYTES			3
#define PALETTE_SIZE		256
#define PALETTE_BYTES	768

/*
******************************** Prototypes *********************************
*/
/*
-------------------------------- Palette.cpp --------------------------------
*/
void __cdecl Set_Palette(void *palette);
void __cdecl Set_Palette_Color(void *palette, int color, void *data);
void Fade_Palette_To(void *palette1, unsigned int delay, void (*callback)() );

/*
-------------------------------- loadpal.cpp --------------------------------
*/
void __cdecl Load_Palette(char *palette_file_name, void *palette_pointer);

/*
------------------------------- morphpal.cpp --------------------------------
*/
void __cdecl Morph_Palette (void *src_palette, void *dst_palette, unsigned int delay,
	void *callback);

/*
---------------------------------- pal.asm ----------------------------------
*/
#ifdef __cplusplus
extern "C" {
#endif

extern void __cdecl Set_Palette_Range(void *palette);
extern BOOL __cdecl Bump_Color(void *palette, int changable, int target);

#ifdef __cplusplus
}
#endif
extern "C" extern unsigned char  CurrentPalette[];		/* in pal.asm */


#endif // PALETTE_H

/***************************** End of palette.h ****************************/
