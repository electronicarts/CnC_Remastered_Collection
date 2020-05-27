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
 *                 Project Name : Load_Palette                             *
 *                                                                         *
 *                    File Name : LOADPAL.CPP                              *
 *                                                                         *
 *                   Programmer : Scott K. Bowen                           *
 *                                                                         *
 *                   Start Date : April 25, 1994                           *
 *                                                                         *
 *                  Last Update : April 27, 1994   [BR]                    *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Note: This module contains dependencies upon the file I/O system,			*
 * specifically Load_Data().																*
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *	  Load_Palette -- Loads a palette file into the given palette buffer.	*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*
********************************* Includes **********************************
*/
//#include <wwstd.h>
#include "wwstd.h"
#include "iff.h"
#include "palette.h"

/*
********************************* Constants *********************************
*/

/*
********************************** Globals **********************************
*/

/*
******************************** Prototypes *********************************
*/

/***************************************************************************
 *	Load_Palette -- Loads a palette file into the given palette buffer.		*
 *                                                                         *
 * INPUT:      																				*
 *		BYTE * file_name 			- name of the file to load.						*
 *		BYTE * palette_pointer	- pointer to palette buffer.	      			*
 *																									*
 * OUTPUT:     																				*
 *		none                                                        			*
 *                                                                         *
 * WARNINGS:   	                                                         *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/20/1991 BS : Created.                                              *
 *   04/27/1994 BR : Converted to 32-bit                                   *
 *=========================================================================*/
void __cdecl Load_Palette(char *palette_file_name, void *palette_pointer)
{
	#if(IBM)
		Load_Data(palette_file_name, palette_pointer, 768);
	#else
		Load_Data(palette_file_name, palette_pointer, (ULONG)(2<<BIT_PLANES));
	#endif
}

/**************************** End of loadpal.cpp ***************************/
