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

/* $Header: /CounterStrike/PK.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : PK.CPP                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/04/96                                                     *
 *                                                                                             *
 *                  Last Update : July 10, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   PKey::Decode_Exponent -- Decodes the exponent back into the key.                          *
 *   PKey::Decode_Modulus -- Decodes the modulus value back into the key.                      *
 *   PKey::Decrypt -- Decrypt supplied cyphertext into its original plaintext.                 *
 *   PKey::Encode_Exponent -- Encode the exponent portion of the key into a buffer.            *
 *   PKey::Encode_Modulus -- Encode the modulus portion of the key.                            *
 *   PKey::Encrypt -- Encrypt blocks of plaintext.                                             *
 *   PKey::Generate -- Generate a public and private key.                                      *
 *   PKey::PKey -- Construct a key using encoded strings.                                      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"pk.h"
#include	"rndstraw.h"


/***********************************************************************************************
 * PKey::PKey -- Construct a key using encoded strings.                                        *
 *                                                                                             *
 *    This constructor will construct a key based on the encoded strings supplied.             *
 *                                                                                             *
 * INPUT:   exponent -- The encoded string for the exponent portion of the key.                *
 *                                                                                             *
 *          modulus  -- The encoded string for the modulus portion of the key.                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
PKey::PKey(void const * exponent, void const * modulus)
{
	Modulus.DERDecode((unsigned char *)modulus);
	Exponent.DERDecode((unsigned char *)exponent);
	BitPrecision = Modulus.BitCount()-1;
}


/***********************************************************************************************
 * PKey::Encode_Modulus -- Encode the modulus portion of the key.                              *
 *                                                                                             *
 *    This will store the modulus portion of the key into a buffer. The number of bytes        *
 *    stored into the buffer depends on the value of the key.                                  *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that will hold the encoded modulus value.        *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes stored to the buffer.                             *
 *                                                                                             *
 * WARNINGS:   Be sure that the buffer can hold the encoded bytes. This is normally around the *
 *             same size as the Crypt_Block_Size() (plus a byte or two).                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int PKey::Encode_Modulus(void * buffer) const
{
	if (buffer == NULL) {
		return(0);
	}
	return(Modulus.DEREncode((unsigned char *)buffer));
}


/***********************************************************************************************
 * PKey::Encode_Exponent -- Encode the exponent portion of the key into a buffer.              *
 *                                                                                             *
 *    This routine will encode the exponent portion of the key. This is only necessary for the *
 *    slow key since the fast key always has an exponent of 65537.                             *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that will be filled with the encoded exponent.   *
 *                                                                                             *
 * OUTPUT:  Returns with the nuber of bytes stored into the buffer.                            *
 *                                                                                             *
 * WARNINGS:   Be sure the buffer is big enough to hold the encoded exponent. Usually this is  *
 *             about the same size as the Crypt_Block_Size (plus a byte or two).               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int PKey::Encode_Exponent(void * buffer) const
{
	if (buffer == NULL) {
		return(0);
	}
	return(Exponent.DEREncode((unsigned char *)buffer));
}


/***********************************************************************************************
 * PKey::Decode_Modulus -- Decodes the modulus value back into the key.                        *
 *                                                                                             *
 *    This is the counterpart to the Encode_Modulus() function. It will initialize the         *
 *    modulus portion of the key with the encoded data supplied.                               *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that holds the previously encoded modulus value. *
 *                                                                                             *
 * OUTPUT:  void                                                                               *
 *                                                                                             *
 * WARNINGS:   void                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void PKey::Decode_Modulus(void * buffer)
{
	Modulus.DERDecode((unsigned char *)buffer);
	BitPrecision = Modulus.BitCount()-1;
}


/***********************************************************************************************
 * PKey::Decode_Exponent -- Decodes the exponent back into the key.                            *
 *                                                                                             *
 *    This is the counterpart to the Encode_Exponent function. It will decode a previously     *
 *    encoded exponent portion back into the key.                                              *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that holds the encoded exponent value.           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void PKey::Decode_Exponent(void * buffer)
{
	Exponent.DERDecode((unsigned char *)buffer);
}


/***********************************************************************************************
 * PKey::Generate -- Generate a public and private key.                                        *
 *                                                                                             *
 *    Public key cryptography relies on having two paired keys. The key used to encrypt        *
 *    data must be decrypted by using the other key. Which key designated as the public or     *
 *    private key is arbitrary. However, one is faster than the other. Use the faster key for  *
 *    the more common operation.                                                               *
 *                                                                                             *
 * INPUT:   random   -- Reference to a source of random data.                                  *
 *                                                                                             *
 *          bits     -- The number of bits to use for key generation. Use a number greater     *
 *                      than 16 but less than 2048. The ideal bit size is one that is evenly   *
 *                      divisible by 8 and then add one. Practical numbers range from 65 to    *
 *                      1025 bits.                                                             *
 *                                                                                             *
 *          fastkey  -- Reference to the key that has fast encryption/decryption properties.   *
 *                                                                                             *
 *          slowkey  -- Reference to the mate key of the other.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine can take a very long time. It can take MINUTES to generate a       *
 *             1024 bit key (even on a Pentium Pro 200Mghz machine).                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/05/1996 JLB : Created.                                                                 *
 *   07/10/1996 JLB : Must supply source of random data.                                       *
 *=============================================================================================*/
void PKey::Generate(Straw & random, int bits, PKey & fastkey, PKey & slowkey)
{
	//PG_TO_FIX
	fastkey;
	slowkey;
#if (0)
	/*
	**	Key generation consists of create a key pair and then testing the key
	**	pair. If the test fails, then repeat the process. The test and repeat
	**	method is required since the prime number generating process can't
	**	guarantee the generation of a prime number -- it can only generate a
	**	highly likely prime number.
	*/
	for (;;) {
		/*
		**	Generate the two random prime numbers. This is the longest
		**	step.
		*/
		BigInt p = Generate_Prime(random, bits, &p);
		BigInt q = Generate_Prime(random, bits, &q);

		/*
		**	The exponent factors are easy to calculate from the prime numbers.
		*/
		BigInt e = Fast_Exponent();
		BigInt n = p * q;
		BigInt pqmin = (p-(unsigned short)1)*(q-(unsigned short)1);
		BigInt d = e.Inverse(pqmin);

		/*
		**	Store the data into the key objects. Notice that the modulus is the
		**	same for both the fast and slow keys. Also notice that the exponent for
		**	the fast key is ALWAYS 65537. Given this, it is possible to economize the
		**	fast key into being just the modulus and the slow key to being just the
		**	exponent (presuming the slow key also has access to the fast key so that
		**	it can get the modulus).
		*/
		fastkey.Exponent = e;
		fastkey.Modulus = n;
		fastkey.BitPrecision = n.BitCount()-1;

		slowkey.Exponent = d;
		slowkey.Modulus = n;
		slowkey.BitPrecision = fastkey.BitPrecision;

		/*
		**	Test the keys by encrypting a block of random bytes. If it decrypts
		**	correctly, then a valid key pair has been generated -- bail.
		*/
		char before[256];
		char after[256];

		for (int index = 0; index < fastkey.Plain_Block_Size(); index++) {
			before[index] = (char)rand();
		}
		fastkey.Encrypt(before, fastkey.Plain_Block_Size(), after);
		slowkey.Decrypt(after, slowkey.Crypt_Block_Size(), after);

		/*
		**	Compare the pre and post processing buffer. A match indicates
		**	a valid key pair.
		*/
		if (memcmp(before, after, fastkey.Plain_Block_Size()) == 0) break;
	}
#endif
}


/***********************************************************************************************
 * PKey::Encrypt -- Encrypt blocks of plaintext.                                               *
 *                                                                                             *
 *    This routine will encrypt the supplied plaintext into cyphertext by processing the input *
 *    in block. The source is processed in whole blocks. Partial blocks are not supported by   *
 *    public key cryptography.                                                                 *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the source plaintext that will be encrypted.                *
 *                                                                                             *
 *          length   -- The length of the plaintext to encrypt.                                *
 *                                                                                             *
 *          dest     -- Pointer to the buffer that will hold the encrypted data.               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of cypher text bytes placed into the destination buffer.   *
 *                                                                                             *
 * WARNINGS:   Be sure that the destination buffer is big enough to hold the output.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int PKey::Encrypt(void const * source, int slen, void * dest) const
{
	int total = 0;

	/*
	**	Encrypt the source data in full blocks. Partial blocks are not processed and are not
	**	copied to the destination buffer.
	*/
	while (slen >= Plain_Block_Size()) {

		/*
		**	Perform the encryption of the block.
		*/
		BigInt temp = 0;
		memmove(&temp, source, Plain_Block_Size());
		temp = temp.exp_b_mod_c(Exponent, Modulus);

		/*
		**	Move the cypher block to the destination.
		*/
		memmove(dest, &temp, Crypt_Block_Size());
		slen -= Plain_Block_Size();
		source = (char *)source + Plain_Block_Size();
		dest = (char *)dest + Crypt_Block_Size();
		total += Crypt_Block_Size();
	}

	return(total);
}


/***********************************************************************************************
 * PKey::Decrypt -- Decrypt supplied cyphertext into its original plaintext.                   *
 *                                                                                             *
 *    This routine will process the supplied cyphertext by breaking it up into blocks and      *
 *    then decrypting each block in turn. The block size is dependant upon the key. By NOT     *
 *    embedding this information into the cypher data, it makes the encryption more secure.    *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the cypher text to be decrypted.                            *
 *                                                                                             *
 *          length   -- The number of cypher text bytes supplied to this routine.              *
 *                                                                                             *
 *          dest     -- Pointer to the buffer to hold the plaintext.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the number of plaintext bytes output to the destination buffer.       *
 *                                                                                             *
 * WARNINGS:   Only whole blocks are processed. If the source has any partial block sized      *
 *             data, then it will be left unprocessed.                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int PKey::Decrypt(void const * source, int slen, void * dest) const
{
	int total = 0;
	BigInt temp;

	/*
	**	Decrypt the source data in full blocks. Partial blocks are not processed in any way.
	*/
	while (slen >= Crypt_Block_Size()) {

		/*
		**	Perform the encryption.
		*/
		temp = 0;
		memmove(&temp, source, Crypt_Block_Size());
		temp = temp.exp_b_mod_c(Exponent, Modulus);

		/*
		**	Move the cypher block to the destination.
		*/
		memmove(dest, &temp, Plain_Block_Size());
		slen -= Crypt_Block_Size();
		source = (char *)source + Crypt_Block_Size();
		dest = (char *)dest + Plain_Block_Size();
		total += Plain_Block_Size();
	}

	return(total);
}
