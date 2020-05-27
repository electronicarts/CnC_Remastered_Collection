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

/* $Header: /CounterStrike/RANDOM.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RAND.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 02/28/96                                                     *
 *                                                                                             *
 *                  Last Update : February 28, 1996 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   RandomClass::RandomClass -- Constructor for the random number class.                      *
 *   RandomClass::operator() -- Fetches the next random number in the sequence.                *
 *   RandomClass::operator() -- Ranged random number generator.                                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "random.h"
#ifdef RANDOM_COUNT
#include <stdio.h>
extern long Frame;
#endif

/***********************************************************************************************
 * RandomClass::RandomClass -- Constructor for the random number class.                        *
 *                                                                                             *
 *    This constructor can take an integer as a parameter. This allows the class to be         *
 *    constructed by assigning an integer to an existing object. The compiler creates a        *
 *    temporary with the constructor and then performs a copy constructor operation.           *
 *                                                                                             *
 * INPUT:   seed  -- The optional starting seed value to use.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/27/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
RandomClass::RandomClass(unsigned seed) :
	Seed(seed)
{
#ifdef RANDOM_COUNT
	Count1 = 0;
	Count2 = 0;
#endif
}


/***********************************************************************************************
 * RandomClass::operator() -- Fetches the next random number in the sequence.                  *
 *                                                                                             *
 *    This routine will fetch the next random number in the sequence.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the next random number.                                               *
 *                                                                                             *
 * WARNINGS:   This routine modifies the seed value so that subsequent calls will not return   *
 *             the same value. Take note that this routine only returns 15 bits of             *
 *             random number.                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/27/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int RandomClass::operator ()(void)
{
#ifdef RANDOM_COUNT
	Count1++;
	printf("Frame %d: Random Count1:%d Count2:%d (%x)\n",Frame,Count1,Count2,Seed);
#endif

	/*
	**	Transform the seed value into the next number in the sequence.
	*/
	Seed = (Seed * MULT_CONSTANT) + ADD_CONSTANT;

	/*
	**	Extract the 'random' bits from the seed and return that value as the
	**	random number result.
	*/
	return((Seed >> THROW_AWAY_BITS) & (~((~0) << SIGNIFICANT_BITS)));
}


/***********************************************************************************************
 * RandomClass::operator() -- Ranged random number generator.                                  *
 *                                                                                             *
 *    This function will return with a random number within the range specified. This replaces *
 *    the functionality of IRandom() in the old library.                                       *
 *                                                                                             *
 * INPUT:   minval   -- The minimum value to return from the function.                         *
 *                                                                                             *
 *          maxval   -- The maximum value to return from the function.                         *
 *                                                                                             *
 * OUTPUT:  Returns with a random number that falls between the minval and maxval (inclusive). *
 *                                                                                             *
 * WARNINGS:   The range specified must fall within the maximum bit significance of the        *
 *             random number algorithm (15 bits), otherwise the value returned will be         *
 *             decidedly non-random.                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/27/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int RandomClass::operator() (int minval, int maxval)
{
#ifdef RANDOM_COUNT
	Count2++;
	printf("Frame %d: Random Count1:%d Count2:%d (%x)\n",Frame,Count1,Count2,Seed);
#endif

	/*
	**	Test for shortcut case where the range is null and thus
	**	the number to return is actually implicit from the
	**	parameters.
	*/
	if (minval == maxval) return(minval);

	/*
	**	Ensure that the min and max range values are in proper order.
	*/
	if (minval > maxval) {
		int temp = minval;
		minval = maxval;
		maxval = temp;
	}

	/*
	**	Find the highest bit that fits within the magnitude of the
	**	range of random numbers desired. Notice that the scan is
	**	limited to the range of significant bits returned by the
	**	random number algorithm.
	*/
	int magnitude = maxval - minval;
	int highbit = SIGNIFICANT_BITS-1;
	while ((magnitude & (1 << highbit)) == 0 && highbit > 0) {
		highbit--;
	}

	/*
	**	Create a full bit mask pattern that has all bits set that just
	**	barely covers the magnitude of the number range desired.
	*/
	int mask = ~( (~0L) << (highbit+1) );

	/*
	**	Keep picking random numbers until it fits within the magnitude desired.
	*/
	int pick = magnitude+1;
	while (pick > magnitude) {
		pick = operator()() & mask;
	}

	/*
	**	Finally, bias the random number pick to the start of the range
	**	requested.
	*/
	return(pick + minval);
}

