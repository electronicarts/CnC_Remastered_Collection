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
 *                 Project Name : Westwood Library                         *
 *                                                                         *
 *                    File Name : LOADFONT.C                               *
 *                                                                         *
 *                   Programmer : Joe L. Bostic                            *
 *                                                                         *
 *                   Start Date : September 6, 1991                        *
 *                                                                         *
 *                  Last Update : June 27, 1994   [SKB]                    *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Load_Font -- Loads a font from disk.                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "font.h"
#include <file.h>
#include <wwmem.h>
#include <wwstd.h>

#if(IBM)
#include <fcntl.h>
#include <io.h>

#include <errno.h>

int FontXSpacing = 0;
int FontYSpacing = 0;
void const *FontPtr = NULL;
char FontWidth  = 8;
char FontHeight = 8;

// only font.c and set_font.c use the following
char *FontWidthBlockPtr = NULL;



/***************************************************************************
 * LOAD_FONT -- Loads a font from disk.                                    *
 *                                                                         *
 *    This loads a font from disk.  This function must be called as a    	*
 *    precursor to calling Set_Font().  You need only call this function 	*
 *    once per desired font at the beginning of your code, but AFTER     	*
 *    Prog_Init() is called.                                             	*
 *                                                                         *
 * INPUT:      name  - Pointer to font name to use (eg. "topaz.font")    	*
 *                                                                       	*
 *             fontsize - Size in points of the font loaded.             	*
 *                                                                       	*
 * OUTPUT:     Pointer to font data or NULL if unable to load.           	*
 *                                                                       	*
 * WARNINGS:   Some system memory is grabbed by this routine.            	*
 *                                                                       	*
 * HISTORY:                                                                *
 *   4/10/91    BS  : 2.0 compatibily                                     	*
 *   6/09/91    JLB : IBM and Amiga compatability.                        	*
 *   11/27/1991 JLB : Uses file I/O routines for disk access.              *
 *   01/29/1992 DRD : Modified to use new font format.                     *
 *   02/01/1992 DRD : Added font file verification.                        *
 *   06/29/1994 SKB : modified for 32 bit library                          *
 *=========================================================================*/
void * __cdecl Load_Font(char const *name)
{
	char	valid;
	int		fh;		// DOS file handle for font file.
	unsigned short	size;		// Size of the data in the file (-2);
	char	*ptr = NULL;		// Pointer to newly loaded font.



	fh=Open_File(name,READ);
	if ( fh>=0 ){
		if ( Read_File(fh, (char *) &size, 2) != 2) return(NULL);

		ptr = (char *) Alloc(size , MEM_NORMAL );
		*(short *)ptr = size;
		Read_File(fh, ptr + 2, size - 2);
		Close_File(fh);
	} else {
		return ((void*)errno);
	}



#ifdef cuts
	if (Find_File(name)) {
		fh = Open_File(name, READ);
		if (Read_File(fh, (char *) &size, 2) != 2) return(NULL);

		ptr = (char *) Alloc(size, MEM_NORMAL);
		*(short *)ptr = size;
		Read_File(fh, ptr + 2, size - 2);
		Close_File(fh);
	} else {
		return (NULL);
	}
#endif

	//
	// verify that the file loaded is a valid font file.
	//

	valid = FALSE;
	if (*(ptr + 2) == 0) {		// no compression
		if (*(ptr + 3) == 5) {		// currently only 5 data blocks are used.
			valid = TRUE;
		}
	}

	if ( !valid ) {
		return (NULL);
	}

   return(ptr);
}

#endif
