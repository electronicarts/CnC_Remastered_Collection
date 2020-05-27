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

/* $Header: /CounterStrike/SHASTRAW.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SHASTRAW.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/02/96                                                     *
 *                                                                                             *
 *                  Last Update : July 3, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   SHAStraw::Get -- Fetch data from the straw and process the SHA with the data.             *
 *   SHAStraw::Result -- Fetches the current SHA digest.                                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"shastraw.h"


/***********************************************************************************************
 * SHAStraw::Get -- Fetch data from the straw and process the SHA with the data.               *
 *                                                                                             *
 *    This routine will fetch the requested data and as it passes through this straw it will   *
 *    submit it to the SHA processor. The data that passes through is unmodified by this       *
 *    straw segment.                                                                           *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer that will hold the requested data.               *
 *                                                                                             *
 *          length   -- The length of the data requested.                                      *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes stored in the buffer. If this number is less      *
 *          than the number requested, then this indicates that the data stream has been       *
 *          exhausted.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int SHAStraw::Get(void * source, int slen)
{
	if (source == NULL || slen < 1) {
		return(0);
	}

	int counter = Straw::Get(source, slen);
	SHA.Hash(source, counter);
	return(counter);
}


/***********************************************************************************************
 * SHAStraw::Result -- Fetches the current SHA digest.                                         *
 *                                                                                             *
 *    Use this routine to fetch the current SHA digest from the straw. It will return the      *
 *    digest of the data that has passed through this straw segment.                           *
 *                                                                                             *
 * INPUT:   result   -- Pointer to the buffer to hold the message digest. The buffer must be   *
 *                      20 bytes long.                                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes stored into the digest buffer. This will always   *
 *          be 20.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int SHAStraw::Result(void * result) const
{
	return(SHA.Result(result));
}
