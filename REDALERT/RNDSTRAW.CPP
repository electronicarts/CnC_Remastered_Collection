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

/* $Header: /CounterStrike/RNDSTRAW.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RNDSTRAW.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/04/96                                                     *
 *                                                                                             *
 *                  Last Update : July 10, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   RandomStraw::Get -- Fetch random data.                                                    *
 *   RandomStraw::RandomStraw -- Constructor for the random straw class.                       *
 *   RandomStraw::Reset -- Reset the data to known initial state.                              *
 *   RandomStraw::Scramble_Seed -- Masks any coorelation between the seed bits.                *
 *   RandomStraw::Seed_Bit -- Add a random bit to the accumulated seed value.                  *
 *   RandomStraw::Seed_Bits_Needed -- Fetches the number of seed bits needed.                  *
 *   RandomStraw::Seed_Byte -- Submit 8 bits to the random number seed.                        *
 *   RandomStraw::Seed_Long -- Submit 32 bits to the random number seed.                       *
 *   RandomStraw::Seed_Short -- Submit 16 bits to the random number seed.                      *
 *   RandomStraw::~RandomStraw -- Destructor for random straw class.                           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	<limits.h>
#include	<string.h>
#include	"rndstraw.h"
#include	"sha.h"


/***********************************************************************************************
 * RandomStraw::RandomStraw -- Constructor for the random straw class.                         *
 *                                                                                             *
 *    This will initialize the random straw into a known state. The initial state is useless   *
 *    for cryptographic purposes. It must be seeded before it should be used.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
RandomStraw::RandomStraw(void) :
	SeedBits(0),
	Current(0)
{
	Reset();
}


/***********************************************************************************************
 * RandomStraw::~RandomStraw -- Destructor for random straw class.                             *
 *                                                                                             *
 *    This destructor will clear out the seed data so that there won't be any sensitive        *
 *    information left over in the memory this object occupied.                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
RandomStraw::~RandomStraw(void)
{
	Reset();
}


/***********************************************************************************************
 * RandomStraw::Reset -- Reset the data to known initial state.                                *
 *                                                                                             *
 *    This routine is functionally equivalent to performing an placement new on the object. It *
 *    clears out all the seed data.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   You must reseed it before fetching random numbers.                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void RandomStraw::Reset(void)
{
	SeedBits = 0;
	Current = 0;
	memset(Random, '\0', sizeof(Random));
}


/***********************************************************************************************
 * RandomStraw::Seed_Bits_Needed -- Fetches the number of seed bits needed.                    *
 *                                                                                             *
 *    This routine is used when seeding the random straw. Keep feeding random bits to this     *
 *    class until the Seed_Bits_Needed value returns zero. Random bits should be gathered from *
 *    sources external to the immediate program. Examples would be the system clock (good for  *
 *    a few bits), the delay between player keystrokes (good for a few bits per keystroke),    *
 *    etc. When gathering random bits from integers, use the low order bits (since they        *
 *    characteristically are less predictable and more 'random' than the others).              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bits required in order to fully seed this random        *
 *          number generator.                                                                  *
 *                                                                                             *
 * WARNINGS:   Even if this routine returns zero, you are still allowed and encouraged to feed *
 *             more random bits when the opportunity arrises.                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int RandomStraw::Seed_Bits_Needed(void) const
{
	const int total = sizeof(Random) * CHAR_BIT;
	if (SeedBits < total) {
		return(total - SeedBits);
	}
	return(0);
}


/***********************************************************************************************
 * RandomStraw::Seed_Bit -- Add a random bit to the accumulated seed value.                    *
 *                                                                                             *
 *    This routine should be called to feed a single random bit to the random straw object.    *
 *    You must feed as many bits as requested by the Seed_Bits_Needed() function. Submitting   *
 *    additional bits is not only allowed, but encouraged.                                     *
 *                                                                                             *
 * INPUT:   seed  -- The bit (lowest order bit) to feed to the random number seed.             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void RandomStraw::Seed_Bit(int seed)
{
	char * ptr = ((char *)&Random[0]) + ((SeedBits / CHAR_BIT) % sizeof(Random));
	char frac = (char)(1 << (SeedBits & (CHAR_BIT-1)));

	if (seed & 0x01) {
		*ptr ^= frac;
	}
	SeedBits++;

	if (SeedBits == (sizeof(Random) * CHAR_BIT)) {
		Scramble_Seed();
	}
}


/***********************************************************************************************
 * RandomStraw::Seed_Byte -- Submit 8 bits to the random number seed.                          *
 *                                                                                             *
 *    This will submit 8 bits (as specified) to the random number seed.                        *
 *                                                                                             *
 * INPUT:   seed  -- The seed bits to submit.                                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void RandomStraw::Seed_Byte(char seed)
{
	for (int index = 0; index < CHAR_BIT; index++) {
		Seed_Bit(seed);
		seed >>= 1;
	}
}


/***********************************************************************************************
 * RandomStraw::Seed_Short -- Submit 16 bits to the random number seed.                        *
 *                                                                                             *
 *    This will submit 16 bits to the accumulated seed.                                        *
 *                                                                                             *
 * INPUT:   seed  -- The 16 bits to submit to the seed.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void RandomStraw::Seed_Short(short seed)
{
	for (int index = 0; index < (sizeof(seed)*CHAR_BIT); index++) {
		Seed_Bit(seed);
		seed >>= 1;
	}
}


/***********************************************************************************************
 * RandomStraw::Seed_Long -- Submit 32 bits to the random number seed.                         *
 *                                                                                             *
 *    This will submit a full 32 bits to the accumulated seed value.                           *
 *                                                                                             *
 * INPUT:   seed  -- The 32 bits to submit.                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void RandomStraw::Seed_Long(long seed)
{
	for (int index = 0; index < (sizeof(seed)*CHAR_BIT); index++) {
		Seed_Bit(seed);
		seed >>= 1;
	}
}


/***********************************************************************************************
 * RandomStraw::Scramble_Seed -- Masks any coorelation between the seed bits.                  *
 *                                                                                             *
 *    This routine is called when a full set of seed bits has been accumulated. It will take   *
 *    the existing seed and scramble the bits. An effective way of doing this is to use the    *
 *    Secure Hash Algorithm. It is necessary to have this routine because there might be       *
 *    some coorelation in the seed bits. Even more important is that this routine will result  *
 *    in every bit of the seed having a scrambling effect on every other bit.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void RandomStraw::Scramble_Seed(void)
{
	SHAEngine sha;

	for (int index = 0; index < sizeof(Random); index++) {
		char digest[20];

		sha.Hash(&Random[0], sizeof(Random));
		sha.Result(digest);

		int tocopy = sizeof(digest) < (sizeof(Random)-index) ? sizeof(digest) : (sizeof(Random)-index);
		memmove(((char *)&Random[0]) + index, digest, tocopy);
	}
}


/***********************************************************************************************
 * RandomStraw::Get -- Fetch random data.                                                      *
 *                                                                                             *
 *    This routine will fetch random data and store it into the buffer specified.              *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer to fill with random data.                        *
 *                                                                                             *
 *          length   -- The number of bytes to store into the buffer.                          *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes stored into the buffer. This will always   *
 *          be the number of bytes requested since random numbers are unexhaustible.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *   07/10/1996 JLB : Revamped to make cryptographically secure.                               *
 *=============================================================================================*/
int RandomStraw::Get(void * source, int slen)
{
	if (source == NULL || slen < 1) {
		return(Straw::Get(source, slen));
	}

	int total = 0;
	while (slen > 0) {
		*(char *)source = (char)Random[Current++];
		Current = Current % (sizeof(Random) / sizeof(Random[0]));
		source = (char*)source + sizeof(char);
		slen--;
		total++;
	}
	return(total);
}
