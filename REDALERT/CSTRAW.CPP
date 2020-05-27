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

/* $Header: /CounterStrike/CSTRAW.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CSTRAW.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/10/96                                                     *
 *                                                                                             *
 *                  Last Update : November 10, 1996 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CacheStraw::Get -- Fetch data from the data source.                                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"cstraw.h"
#include	<string.h>


/***********************************************************************************************
 * CacheStraw::Get -- Fetch data from the data source.                                         *
 *                                                                                             *
 *    This will supply the data quantity requested. It performs a regulating influence on the  *
 *    data requests passed through it. The data is requested from the next straw in the        *
 *    chain such that the data stream is requested in chunks. This serves to lessen the        *
 *    impact of multiple small data requests.                                                  *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer to hold the data.                                *
 *                                                                                             *
 *          slen     -- The number of data bytes requested.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the number of data bytes stored into the buffer specified. If this    *
 *          number is less than that requested, it indicates that the data source has been     *
 *          exhausted.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int CacheStraw::Get(void * source, int slen)
{
	int total = 0;

	if (Is_Valid() && source != NULL && slen > 0) {

		/*
		**	Keep processing the data request until there is no more data to supply or the request
		**	has been fulfilled.
		*/
		while (slen > 0) {

			/*
			**	First try to fetch the data from data previously loaded into the buffer.
			*/
			if (Length > 0) {
				int tocopy = (Length < slen) ? Length : slen;
				memmove(source, ((char *)BufferPtr.Get_Buffer()) + Index, tocopy);
				slen -= tocopy;
				Index += tocopy;
				total += tocopy;
				Length -= tocopy;
				source = (char*)source + tocopy;
			}
			if (slen == 0) break;

			/*
			**	Since there is more to be fulfilled yet the holding buffer is empty,
			**	refill the buffer with a fresh block of data from the source.
			*/
			Length = Straw::Get(BufferPtr, BufferPtr.Get_Size());
			Index = 0;
			if (Length == 0) break;
		}
	}
	return(total);
}
