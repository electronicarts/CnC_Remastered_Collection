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

/* $Header: /CounterStrike/BLOWFISH.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BLOWFISH.H                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 04/14/96                                                     *
 *                                                                                             *
 *                  Last Update : April 14, 1996 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef BLOWFISH_H
#define BLOWFISH_H

#include	<limits.h>


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


/*
**	This engine will process data blocks by encryption and decryption.
**	The "Blowfish" algorithm is in the public domain. It uses
**	a Feistal network (similar to IDEA). It has no known
**	weaknesses, but is still relatively new. Blowfish is particularly strong
**	against brute force attacks. It is also quite strong against linear and
**	differential cryptanalysis. Its weakness is that it takes a relatively
**	long time to set up with a new key (1/100th of a second on a P6-200).
**	The time to set up a key is equivalent to encrypting 4240 bytes.
*/
class BlowfishEngine {
	public:
		BlowfishEngine(void) : IsKeyed(false) {}
		~BlowfishEngine(void);

		void Submit_Key(void const * key, int length);

		int Encrypt(void const * plaintext, int length, void * cyphertext);
		int Decrypt(void const * cyphertext, int length, void * plaintext);

		/*
		**	This is the maximum key length supported.
		*/
		enum {MAX_KEY_LENGTH=56};

	private:
		bool IsKeyed;

		void Sub_Key_Encrypt(unsigned long & left, unsigned long & right);

		void Process_Block(void const * plaintext, void * cyphertext, unsigned long const * ptable);
		void Initialize_Tables(void);

		enum {
			ROUNDS = 16,		// Feistal round count (16 is standard).
			BYTES_PER_BLOCK=8	// The number of bytes in each cypher block (don't change).
		};

		/*
		**	Initialization data for sub keys. The initial values are constant and
		**	filled with a number generated from pi. Thus they are not random but
		**	they don't hold a weak pattern either.
		*/
		static unsigned long const P_Init[(int)ROUNDS+2];
		static unsigned long const S_Init[4][UCHAR_MAX+1];

		/*
		**	Permutation tables for encryption and decryption.
		*/
 		unsigned long P_Encrypt[(int)ROUNDS+2];
 		unsigned long P_Decrypt[(int)ROUNDS+2];

		/*
		**	S-Box tables (four).
		*/
		unsigned long bf_S[4][UCHAR_MAX+1];
};

#endif

