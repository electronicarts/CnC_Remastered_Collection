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
 *                 Project Name : WSA 32bit LIbrary                        *
 *                                                                         *
 *                    File Name : WSA.H                                    *
 *                                                                         *
 *                   Programmer : Scott K. Bowen                           *
 *                                                                         *
 *                   Start Date : May 23, 1994                             *
 *                                                                         *
 *                  Last Update : May 25, 1994   [SKB]                     *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Open_Animation -- file name and flags, system allocates buffer.       *
 *   Open_Animation -- file name, flags, palette, system allocates buffer. *
 *   Open_Animation -- file_name, graphic buffer, flags.                   *
 *   Open_Animation -- file name, bufferclass, flags, palette.             *
 *   Open_Animation -- filename, ptr, size, flags, no palette.             *
 *   Animate_Frame -- Animate a frame to a page with magic colors.         *
 *   Animate_Frame -- Animate a frame to a viewport with magic colors.     *
 *   Animate_Frame -- Animate a frame to a page.                           *
 *   Animate_Frame -- Animate a frame to a viewport.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef WSA_H
#define WSA_H

#ifndef WWSTD_H
#include "wwstd.h"
#endif

#ifndef GBUFFER_H
#include "gbuffer.h"
#endif

//lint -strong(AJX,WSAType)
typedef enum {
	WSA_NORMAL,								// Normal WSA animation
	WSA_GHOST	 	= 0x1000,			// Or'd with the above flags to get ghosting
	WSA_PRIORITY2 	= 0x2000,			// Copy using a priority (or in the priority)
	WSA_TRANS    	= 0x4000,			// Copy frame, ignoring transparent colors
	WSA_PRIORITY 	= 0x8000				// Copy using a priority (or in the priority)
} WSAType;


//lint -strong(AJX,WSAOpenType)
typedef enum {
	WSA_OPEN_FROM_MEM		= 0x0000,	// Try to load entire anim into memory.
	WSA_OPEN_INDIRECT		= 0x0000,	// First animate to internal buffer, then copy to page/viewport.
	WSA_OPEN_FROM_DISK	= 0x0001,	// Force the animation to be disk based.
	WSA_OPEN_DIRECT		= 0x0002,	// Animate directly to page or viewport.

	// These next two have been added for the 32 bit library to give a better idea of what is
	// happening.  You may want to animate directly to the destination or indirectly to the
	// destination by using the animations buffer.  Indirecly is best if the dest is a seenpage
	// and the animation is not linear or if the destination is modified between frames.
	WSA_OPEN_TO_PAGE  = WSA_OPEN_DIRECT ,
	WSA_OPEN_TO_BUFFER= WSA_OPEN_INDIRECT ,

} WSAOpenType;

/*=========================================================================*/
/* The following prototypes are for the file: WSA.CPP								*/
/*=========================================================================*/

void * __cdecl Open_Animation(char const *file_name, char *user_buffer, long user_buffer_size, WSAOpenType user_flags, unsigned char *palette=NULL);
void __cdecl Close_Animation( void *handle );
BOOL __cdecl Animate_Frame(void *handle, GraphicViewPortClass& view,
                         int frame_number, int x_pixel=0, int y_pixel=0,
                         WSAType flags_and_prio = WSA_NORMAL, void *magic_cols=NULL, void *magic=NULL);
int __cdecl Get_Animation_Frame_Count(void *handle);
BOOL __cdecl Animate_Frame(void *handle, VideoViewPortClass& view,
                         int frame_number, int x_pixel=0, int y_pixel=0,
                         WSAType flags_and_prio = WSA_NORMAL, void *magic_cols=NULL, void *magic=NULL);
int __cdecl Get_Animation_Frame_Count(void *handle);
int __cdecl Get_Animation_X(void const *handle);
int __cdecl Get_Animation_Y(void const *handle);
int __cdecl Get_Animation_Width(void const *handle);
int __cdecl Get_Animation_Height(void const *handle);
int __cdecl Get_Animation_Palette(void const *handle);
unsigned long __cdecl Get_Animation_Size(void const *handle);


/***************************************************************************
 * OPEN_ANIMATION -- file name, flags, palette, system allocates buffer.   *
 *                                                                         *
 *                                                                         *
 * INPUT:   char *file_name - name of file to open.                        *
 *          WSAOpenType user_flags - flags on how to open.                 *
 *          unsigned char *palette - pointer to a palette buffer to fill.          *
 *                                                                         *
 * OUTPUT:  void *pointer to animation data.  Must be used for all 			*
 *               other WSA calls.                                          *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/24/1994 SKB : Created.                                             *
 *=========================================================================*/
inline void  * __cdecl Open_Animation(char *file_name, WSAOpenType user_flags, unsigned char *palette=NULL)
{
	return (Open_Animation(file_name, NULL, 0L, user_flags,  palette));
}


/***************************************************************************
 * OPEN_ANIMATION -- file_name, bufferclass, flags. 								*
 *                                                                         *
 *                                                                         *
 * INPUT:   char *file_name - name of file to open.                        *
 *          GraphicBufferClass - pointer to a buffer.                      *
 *          WSAOpenType user_flags - flags on how to open.                 *
 *          unsigned char *palette - pointer to a palette buffer to fill.          *
 *                                                                         *
 * OUTPUT:  void *pointer to animation data.  Must be used for all 			*
 *               other WSA calls.                                          *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/24/1994 SKB : Created.                                             *
 *=========================================================================*/
inline void * __cdecl Open_Animation(char *file_name, BufferClass& buffer, WSAOpenType user_flags,  unsigned char *palette=NULL)
{
	return (Open_Animation(file_name, (char *)buffer.Get_Buffer(), buffer.Get_Size(), user_flags, palette));
}


/*=========================================================================*/
/* The following prototypes are for the file: LP_ASM.ASM							*/
/*=========================================================================*/


extern "C" {
unsigned int __cdecl Apply_XOR_Delta(char *source_ptr, char *delta_ptr);
void  __cdecl Apply_XOR_Delta_To_Page_Or_Viewport(void *target, void *delta, int width, int nextrow, int copy);
}



#endif // WSA_H

