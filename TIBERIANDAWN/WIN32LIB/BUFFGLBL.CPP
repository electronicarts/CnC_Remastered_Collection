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
 **      C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S      **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Westwood 32 bit Library                  *
 *                                                                         *
 *                    File Name : BUFFGLBL.CPP                             *
 *                                                                         *
 *                   Programmer : Phil W. Gorrow                           *
 *                                                                         *
 *                   Start Date : January 10, 1995                         *
 *                                                                         *
 *                  Last Update : January 10, 1995   [PWG]                 *
 *                                                                         *
 * This module holds the global fixup tables for the MCGA buffer class.    *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "gbuffer.h"

/*=========================================================================*/
/* The following PRIVATE functions are in this file:                       */
/*=========================================================================*/


/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

/*=========================================================================*/
/* Globals required by GraphicBufferClass for function pointers.  These		*/
/*   pointers will be set to the proper function when set mode is called.	*/
/*=========================================================================*/
BOOL (*GVPC_Blit_to_VVPC_Func)(void *, void *, int, int, int, int, int, int, BOOL);
BOOL (*GVPC_Scale_To_VVPC)(	void *, void *, int, int, int, int, int, int, int, int, BOOL, char *);

#ifdef not_any_more_it_doesnt
/*=========================================================================*/
/* Globals required by VideoBufferClass for function pointers.  These		*/
/*   pointers will be set to the proper function when set mode is called.	*/
/*=========================================================================*/
void (*VVPC_Clear_Func)(void *, unsigned char);
long (*VVPC_To_Buffer_Func)(void *,int x, int y, int w, int h, void *buff, long size);
void (*VVPC_Put_Pixel_Func)(void *,int x, int y, unsigned char color);
int  (*VVPC_Get_Pixel_Func)(void *, int x, int y);
long (*VVPC_Buffer_To_Page)(int x, int y, int w, int h, void *Buffer, void *view);
BOOL (*VVPC_Blit_to_GVPC_Func)(void *, void *, int, int, int, int, int, int, BOOL);
BOOL (*VVPC_Blit_to_VVPC_Func)(void *, void *, int, int, int, int, int, int, BOOL);
BOOL (*VVPC_Scale_To_GVPC)(	void *, void *, int, int, int, int, int, int, int, int, BOOL, char *);
BOOL (*VVPC_Scale_To_VVPC)(	void *, void *, int, int, int, int, int, int, int, int, BOOL, char *);
LONG (*VVPC_Print_Func)(		void *, const char *, int, int, int, int);
void (*VVPC_Draw_Stamp)(void *, void *, int, int, int, void *);
long (*VVPC_Size_Of_Region)(void *, int, int);

#endif	//not_any_more_it_doesnt

/*=========================================================================*/
/* We need to keep a pointer to the logic page hanging around somewhere		*/
/*=========================================================================*/
GraphicViewPortClass	*LogicPage;

BOOL IconCacheAllowed = TRUE;

/*
** Pointer to a function we will call if we detect loss of focus
*/
void (*Gbuffer_Focus_Loss_Function)(void) = NULL;
