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

/* $Header: /CounterStrike/XSTRAW.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : XSTRAW.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/04/96                                                     *
 *                                                                                             *
 *                  Last Update : July 4, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BufferStraw::Get -- Fetch data from the straw's buffer holding tank.                      *
 *   FileStraw::Get -- Fetch data from the file.                                               *
 *   FileStraw::~FileStraw -- The destructor for the file straw.                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "FUNCTION.H"
#include	"xstraw.h"
#include	<stddef.h>
#include	<string.h>

//---------------------------------------------------------------------------------------------------------
// BufferStraw
//---------------------------------------------------------------------------------------------------------


/***********************************************************************************************
 * BufferStraw::Get -- Fetch data from the straw's buffer holding tank.                        *
 *                                                                                             *
 *    This routine will copy the requested number of bytes from the buffer holding tank (as    *
 *    set up by the straw's constructor) to the buffer specified.                              *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer to be filled with data.                          *
 *                                                                                             *
 *          length   -- The number of bytes requested.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes copied to the buffer. If this is less than        *
 *          requested, then it indicates that the data holding tank buffer is exhausted.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BufferStraw::Get(void * source, int slen)
{
	int total = 0;

	if (Is_Valid() && source != NULL && slen > 0) {
		int len = slen;
		if (BufferPtr.Get_Size() != 0) {
			int theoretical_max = BufferPtr.Get_Size() - Index;
			len = (slen < theoretical_max) ? slen : theoretical_max;
		}

		if (len > 0) {
			memmove(source, ((char*)BufferPtr.Get_Buffer()) + Index, len);
		}

		Index += len;
//		Length -= len;
//		BufferPtr = ((char *)BufferPtr) + len;
		total += len;
	}
	return(total);
}


//---------------------------------------------------------------------------------------------------------
// FileStraw
//---------------------------------------------------------------------------------------------------------


/***********************************************************************************************
 * FileStraw::Get -- Fetch data from the file.                                                 *
 *                                                                                             *
 *    This routine will read data from the file (as specified in the straw's constructor) into *
 *    the buffer indicated.                                                                    *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer to hold the data.                                *
 *                                                                                             *
 *          length   -- The number of bytes requested.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes stored into the buffer. If this number is less    *
 *          than the number requested, then this indicates that the file is exhausted.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int FileStraw::Get(void * source, int slen)
{
	if (Valid_File() && source != NULL && slen > 0) {
		if (!File->Is_Open()) {
			HasOpened = true;
			if (!File->Is_Available()) return(0);
			if (!File->Open(READ)) return(0);
		}

		return(File->Read(source, slen));
	}
	return(0);
}


/***********************************************************************************************
 * FileStraw::~FileStraw -- The destructor for the file straw.                                 *
 *                                                                                             *
 *    This destructor only needs to close the file if it was the one to open it.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
FileStraw::~FileStraw(void)
{
	if (Valid_File() && HasOpened) {
		File->Close();
		HasOpened = false;
		File = NULL;
	}
}
