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

/* $Header: /CounterStrike/RNDSTRAW.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RNDSTRAW.H                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/04/96                                                     *
 *                                                                                             *
 *                  Last Update : July 4, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef RNDSTRAW_H
#define RNDSTRAW_H


#include	"straw.h"
#include	"random.h"

/*
**	This is a straw terminator class. It will generate random numbers to fill the data request.
**	Unlike regular straw terminators, this class will never run out of "data".
*/
class RandomStraw : public Straw
{
	public:
		RandomStraw(void);
		virtual ~RandomStraw(void);

		virtual int Get(void * source, int slen);

		void Reset(void);
		void Seed_Bit(int seed);
		void Seed_Byte(char seed);
		void Seed_Short(short seed);
		void Seed_Long(long seed);

		int Seed_Bits_Needed(void) const;

	private:
		/*
		**	Counter of the number of seed bits stored to this random number
		**	generator.
		*/
		int SeedBits;

		/*
		**	The current random generator to use when fetching the next random
		**	byte of data.
		*/
		int Current;

		/*
		**	Array of generators. There must be at least 448 bits of random number seed
		**	in order to be reasonably secure, however, using 1024 bits would be best.
		*/
		RandomClass Random[32];

		void Scramble_Seed(void);

		RandomStraw(RandomStraw & rvalue);
		RandomStraw & operator = (RandomStraw const & pipe);
};


#endif
