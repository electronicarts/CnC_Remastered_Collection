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
 *                 Project Name : Westwood 32 Bit Library                  *
 *                                                                         *
 *                    File Name : BUFFER.CPP                               *
 *                                                                         *
 *                   Programmer : Phil W. Gorrow                           *
 *                                                                         *
 *                   Start Date : May 18, 1994                             *
 *                                                                         *
 *                  Last Update : June 1, 1994   [PWG]                     *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   BC::BufferClass -- The default (void) constructor for a buffer class  *
 *   BC::~BufferClass -- The destructor for the buffer class               *
 *   BC::BufferClass -- The standard constructor for a buffer class        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef BUFFER_H
#include "buffer.h"
#endif

/*=========================================================================*/
/* The following PRIVATE functions are in this file:                       */
/*=========================================================================*/


/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

/***************************************************************************
 * BC::BufferClass -- The standard constructor for a buffer class          *
 *                                                                         *
 * INPUT:		VOID *	buffer to which should be included in buffer class *
 *					LONG		size of the buffer which we included					*
 *                                                                         *
 * OUTPUT:     NONE                                                        *
 *                                                                         *
 * WARNINGS:   If the buffer passed to this function is equal to NULL,     *
 *					the buffer will be allocated using new.							*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/01/1994 PWG : Created.                                             *
 *=========================================================================*/
BufferClass::BufferClass(VOID *buffer, LONG size)
{
	Size			= size;								// find size of physical buffer

	if (buffer) {										// if buffer is specified
		Buffer			= (BYTE *)buffer;			//		point to it and mark
		Allocated		= FALSE;						//		it as user allocated
	} else {
		Buffer			= new BYTE[Size];			// otherwise allocate it and
		Allocated		= TRUE;						//		mark it system alloced
	}
}

/***************************************************************************
 * BC::BufferClass -- constructor for BufferClass with size only    			*
 *                                                                         *
 * INPUT:		LONG the size of the buffer that needs to be allocated		*
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/01/1994 PWG : Created.                                             *
 *=========================================================================*/
BufferClass::BufferClass(LONG size)
{
	Size				= size;
	Buffer			= new BYTE[Size];			// otherwise allocate it and
	Allocated		= TRUE;						//		mark it system alloced
}

/***************************************************************************
 * BC::BufferClass -- The default (void) constructor for a buffer class    *
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * NOTES:   	The primary function of this class is to be called by a     *
 *					derived class which will fill in the values after the			*
 *					fact.																			*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/01/1994 PWG : Created.                                             *
 *=========================================================================*/
BufferClass::BufferClass(VOID)
{
	Buffer		= NULL;
	Size			= 0;
	Allocated	= FALSE;
}

/***************************************************************************
 * BC::~BUFFERCLASS -- The destructor for the buffer class                 *
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/01/1994 PWG : Created.                                             *
 *=========================================================================*/
BufferClass::~BufferClass(VOID)
{
	if (Allocated) {
		delete[] Buffer;
	}
}
