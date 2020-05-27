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

/* $Header: /CounterStrike/SHA.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SHA.H                                                        *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 04/26/96                                                     *
 *                                                                                             *
 *                  Last Update : April 26, 1996 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef SHA_H
#define SHA_H


/*
**	The "bool" integral type was defined by the C++ committee in
**	November of '94. Until the compiler supports this, use the following
**	definition.
*/
#ifndef __BORLANDC__
#ifndef TRUE_FALSE_DEFINED
#define TRUE_FALSE_DEFINED
enum {false=0,true=1};
typedef int bool;
#endif
#endif


#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<new.h>


/*
**	This implements the Secure Hash Algorithm. It is a cryptographically
**	secure hash with no known weaknesses. It generates a 160 bit hash
**	result given an arbitrary length data source.
*/
class SHAEngine
{
	public:
		SHAEngine(void) : IsCached(false), Length(0), PartialCount(0) {
			Acc.Long[0] = SA;
			Acc.Long[1] = SB;
			Acc.Long[2] = SC;
			Acc.Long[3] = SD;
			Acc.Long[4] = SE;
		};

		void Init(void) {
			new ((void*)this) SHAEngine;
		};

		// Fetch result as if source data were to stop now.
		int Result(void * result) const;

		void Hash(void const * data, long length);

		static int Digest_Size(void) {return(sizeof(SHADigest));}

	private:

		typedef union {
			unsigned long Long[5];
			unsigned char Char[20];
		} SHADigest;

		/*
		**	This holds the calculated final result. It is cached
		**	here to avoid the overhead of recalculating it over
		**	multiple sequential requests.
		*/
		bool IsCached;
		SHADigest FinalResult;

		enum {
			// These are the initial seeds to the block accumulators.
			SA=0x67452301L,
			SB=0xefcdab89L,
			SC=0x98badcfeL,
			SD=0x10325476L,
			SE=0xc3d2e1f0L,

			// These are the constants used in the block transformation.
			K1=0x5a827999L,		// t=0..19		2^(1/2)/4
			K2=0x6ed9eba1L,		// t=20..39		3^(1/2)/4
			K3=0x8f1bbcdcL,		// t=40..59		5^(1/2)/4
			K4=0xca62c1d6L,		// t=60..79		10^(1/2)/4

			// Source data is grouped into blocks of this size.
			SRC_BLOCK_SIZE=16*sizeof(long),

			// Internal processing data is grouped into blocks this size.
			PROC_BLOCK_SIZE=80*sizeof(long)
		};

		long Get_Constant(int index) const {
			if (index < 20) return K1;
			if (index < 40) return K2;
			if (index < 60) return K3;
			return K4;
		};

		// Used for 0..19
		long Function1(long X, long Y, long Z) const {
			return(Z ^ ( X & ( Y ^ Z ) ) );
		};

		// Used for 20..39
		long Function2(long X, long Y, long Z) const {
			return( X ^ Y ^ Z );
		};

		// Used for 40..59
		long Function3(long X, long Y, long Z) const {
			return( (X & Y) | (Z & (X | Y) ) );
		};

		// Used for 60..79
		long Function4(long X, long Y, long Z) const {
			return( X ^ Y ^ Z );
		};

		long Do_Function(int index, long X, long Y, long Z) const {
			if (index < 20) return Function1(X, Y, Z);
			if (index < 40) return Function2(X, Y, Z);
			if (index < 60) return Function3(X, Y, Z);
			return Function4(X, Y, Z);
		};

		// Process a full source data block.
		void Process_Block(void const * source, SHADigest & acc) const;

		// Processes a partially filled source accumulator buffer.
		void Process_Partial(void const * & data, long & length);

		/*
		**	This is the running accumulator values. These values
		**	are updated by a block processing step that occurs
		**	every 512 bits of source data.
		*/
		SHADigest Acc;

		/*
		**	This is the running length of the source data
		**	processed so far. This total is used to modify the
		**	resulting hash value as if it were appended to the end
		**	of the source data.
		*/
		long Length;

		/*
		**	This holds any partial source block. Partial source blocks are
		**	a consequence of submitting less than block sized data chunks
		**	to the SHA Engine.
		*/
		int PartialCount;
		char Partial[SRC_BLOCK_SIZE];
};


#define	SHA_SOURCE1		"abc"
#define	SHA_DIGEST1a	"\xA9\x99\x3E\x36\x47\x06\x81\x6A\xBA\x3E\x25\x71\x78\x50\xC2\x6C\x9C\xD0\xD8\x9D"
#define	SHA_DIGEST1b	"\x01\x64\xB8\xA9\x14\xCD\x2A\x5E\x74\xC4\xF7\xFF\x08\x2C\x4D\x97\xF1\xED\xF8\x80"


#define	SHA_SOURCE2		"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
#define	SHA_DIGEST2a	"\x84\x98\x3E\x44\x1C\x3B\xD2\x6E\xBA\xAE\x4A\xA1\xF9\x51\x29\xE5\xE5\x46\x70\xF1"
#define	SHA_DIGEST2b	"\xD2\x51\x6E\xE1\xAC\xFA\x5B\xAF\x33\xDF\xC1\xC4\x71\xE4\x38\x44\x9E\xF1\x34\xC8"

#define	SHA_SOURCE3		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
#define	SHA_DIGEST3a	"\x34\xAA\x97\x3C\xD4\xC4\xDA\xA4\xF6\x1E\xEB\x2B\xDB\xAD\x27\x31\x65\x34\x01\x6F"
#define	SHA_DIGEST3b	"\x32\x32\xAF\xFA\x48\x62\x8A\x26\x65\x3B\x5A\xAA\x44\x54\x1F\xD9\x0D\x69\x06\x03"

#endif
