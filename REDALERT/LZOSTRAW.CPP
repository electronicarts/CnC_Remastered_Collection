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

/* $Header: /CounterStrike/LZOSTRAW.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LZOSTRAW.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/02/96                                                     *
 *                                                                                             *
 *                  Last Update : July 4, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   LZOStraw::Get -- Fetch data through the LZO processor.                                    *
 *   LZOStraw::LZOStraw -- Constructor for LZO straw object.                                   *
 *   LZOStraw::~LZOStraw -- Destructor for the LZO straw.                                      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"lzostraw.h"
#include	"lzo.h"
#include	<string.h>
#include	<assert.h>


/***********************************************************************************************
 * LZOStraw::LZOStraw -- Constructor for LZO straw object.                                     *
 *                                                                                             *
 *    This will initialize the LZO straw object. Whether the object is to compress or          *
 *    decompress and the block size to use is specified. The data is compressed in blocks      *
 *    that are sized to be quick to compress and yet still yield good compression ratios.      *
 *                                                                                             *
 * INPUT:   decrypt  -- Should the data be decompressed?                                       *
 *                                                                                             *
 *          blocksize-- The size of the blocks to process.                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   It takes two buffers of the blocksize specified if compression is to be         *
 *             performed.                                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
LZOStraw::LZOStraw(CompControl control, int blocksize) :
		Control(control),
		Counter(0),
		Buffer(NULL),
		Buffer2(NULL),
		BlockSize(blocksize)
{
	SafetyMargin = BlockSize;
	Buffer = new char[BlockSize+SafetyMargin];
	if (control == COMPRESS) {
		Buffer2 = new char[BlockSize+SafetyMargin];
	}
}


/***********************************************************************************************
 * LZOStraw::~LZOStraw -- Destructor for the LZO straw.                                        *
 *                                                                                             *
 *    The destructor will free up the allocated buffers that it allocated in the constructor.  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
LZOStraw::~LZOStraw(void)
{
	delete [] Buffer;
	Buffer = NULL;

	delete [] Buffer2;
	Buffer2 = NULL;
}


/***********************************************************************************************
 * LZOStraw::Get -- Fetch data through the LZO processor.                                      *
 *                                                                                             *
 *    This routine will fetch the data bytes specified. It does this by first accumulating     *
 *    a full block of data and then compressing or decompressing it as indicated. Subsequent   *
 *    requests for data will draw from this buffer of processed data until it is exhausted     *
 *    and another block must be fetched.                                                       *
 *                                                                                             *
 * INPUT:   destbuf  -- Pointer to the buffer to hold the data requested.                      *
 *                                                                                             *
 *          length   -- The number of data bytes requested.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes stored into the buffer. If this number     *
 *          is less than that requested, then this indicates that the data source has been     *
 *          exhausted.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int LZOStraw::Get(void * destbuf, int slen)
{
	assert(Buffer != NULL);

	int total = 0;

	/*
	**	Verify parameters for legality.
	*/
	if (destbuf == NULL || slen < 1) {
		return(0);
	}

	while (slen > 0) {

		/*
		**	Copy as much data is requested and available into the desired
		**	destination buffer.
		*/
		if (Counter) {
			int len = (slen < Counter) ? slen : Counter;
			if (Control == DECOMPRESS) {
				memmove(destbuf, &Buffer[BlockHeader.UncompCount-Counter], len);
			} else {
				memmove(destbuf, &Buffer2[(BlockHeader.CompCount+sizeof(BlockHeader))-Counter], len);
			}
			destbuf = ((char *)destbuf) + len;
			slen -= len;
			Counter -= len;
			total += len;
		}
		if (slen == 0) break;

		if (Control == DECOMPRESS) {
			int incount = Straw::Get(&BlockHeader, sizeof(BlockHeader));
			if (incount != sizeof(BlockHeader)) break;

			char *staging_buffer = new char [BlockHeader.CompCount];
			incount = Straw::Get(staging_buffer, BlockHeader.CompCount);
			if (incount != BlockHeader.CompCount) break;
			unsigned int length = sizeof(Buffer);
			lzo1x_decompress ((unsigned char*)staging_buffer, BlockHeader.CompCount, (unsigned char*)Buffer, &length, NULL);
			delete [] staging_buffer;
			Counter = BlockHeader.UncompCount;
		} else {
			BlockHeader.UncompCount = (unsigned short)Straw::Get(Buffer, BlockSize);
			if (BlockHeader.UncompCount == 0) break;
			char *dictionary = new char [64*1024];
			unsigned int length = sizeof (Buffer2) - sizeof (BlockHeader);
			lzo1x_1_compress ((unsigned char*)Buffer, BlockHeader.UncompCount, (unsigned char*)(&Buffer2[sizeof(BlockHeader)]), &length, dictionary);
			BlockHeader.CompCount = (unsigned short)length;
			delete [] dictionary;
			memmove(Buffer2, &BlockHeader, sizeof(BlockHeader));
			Counter = BlockHeader.CompCount+sizeof(BlockHeader);
		}
	}

	return(total);
}
