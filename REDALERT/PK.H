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

/* $Header: /CounterStrike/PK.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : PK.H                                                         *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/03/96                                                     *
 *                                                                                             *
 *                  Last Update : July 3, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef PK_H
#define PK_H

#include	"int.h"

/*
**	This class holds a public or private key used in Public Key Cryptography. It also serves
**	as the conduit for encrypting/decrypting data using that key. Cryptography, using this
**	method, has a couple of characteristics that affect how it is used. One, the process of
**	encrypting/decrypting is very slow. This limits the effective quantity of data that can
**	be processed. Two, the ciphertext is larger than the plaintext. This property generally
**	limits its use to streaming data as opposed to random access data. The data is processed
**	in blocks. The size of the ciphertext and plaintext blocks can be determined only from
**	the key itself.
**
**	A reasonable use of this technology would be to encrypt only critical data such as the
**	password for a fast general purpose cryptographic algorithm.
*/
class PKey
{
	public:
		PKey(void) : Modulus(0), Exponent(0), BitPrecision(0) {}
		PKey(void const * exponent, void const * modulus);		// DER initialization.

		int Encrypt(void const * source, int slen, void * dest) const;
		int Decrypt(void const * source, int slen, void * dest) const;

		static void Generate(Straw & random, int bits, PKey & fastkey, PKey & slowkey);

		int Plain_Block_Size(void) const {return((BitPrecision-1)/8);}
		int Crypt_Block_Size(void) const {return(Plain_Block_Size()+1);}
		int Block_Count(int plaintext_length) const {return((((plaintext_length-1)/Plain_Block_Size())+1));}

		int Encode_Modulus(void * buffer) const;
		int Encode_Exponent(void * buffer) const;

		void Decode_Modulus(void * buffer);
		void Decode_Exponent(void * buffer);

		static long Fast_Exponent(void) {return(65537L);}

	private:

		// p*q
		BigInt Modulus;

		// 65537 or
		// inverse of (p-1)(q-1).
		BigInt Exponent;

		// Maximum bits allowed for block.
		int BitPrecision;
};


#endif
