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

/* $Header: /CounterStrike/PKSTRAW.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : PKSTRAW.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/08/96                                                     *
 *                                                                                             *
 *                  Last Update : July 11, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   PKStraw::Encrypted_Key_Length -- Fetch the encrypted key length.                          *
 *   PKStraw::Get -- Fetch data and process it accordingly.                                    *
 *   PKStraw::Get_From -- Chains one straw to another.                                         *
 *   PKStraw::Key -- Assign a key to the cipher process straw.                                 *
 *   PKStraw::PKStraw -- Initialize the public key straw object.                               *
 *   PKStraw::Plain_Key_Length -- Returns the number of bytes to encrypt key.                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "pkstraw.h"
#include	"rndstraw.h"
#include	"blwstraw.h"


/***********************************************************************************************
 * PKStraw::PKStraw -- Initialize the public key straw object.                                 *
 *                                                                                             *
 *    This constructs the public key straw object. The operation to perform (encrypt or        *
 *    decrypt) as well as a random number generator must be provided.                          *
 *                                                                                             *
 * INPUT:   control  -- What operation to perform on the data. Pass in either ENCRYPT or       *
 *                      DECRYPT.                                                               *
 *                                                                                             *
 *          rnd      -- Reference to a random number straw that is used internally to          *
 *                      generate the sub-key. The final strength of the cipher depends on      *
 *                      quality of this random number generator.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
PKStraw::PKStraw(CryptControl control, RandomStraw & rnd) :
	IsGettingKey(true),
	Rand(rnd),
	BF((control == ENCRYPT) ? BlowStraw::ENCRYPT : BlowStraw::DECRYPT),
	Control(control),
	CipherKey(NULL),
	Counter(0),
	BytesLeft(0)
{
	Straw::Get_From(BF);
}


/***********************************************************************************************
 * PKStraw::Get_From -- Chains one straw to another.                                           *
 *                                                                                             *
 *    This routine handles the special case of this straw object in that there is an           *
 *    embedded blowfish straw segment. It must be chained on correctly.                        *
 *                                                                                             *
 * INPUT:   straw -- Pointer to the straw segment that this segment is to receive data from.   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void PKStraw::Get_From(Straw * straw)
{
	if (BF.ChainTo != straw) {
		if (straw != NULL && straw->ChainFrom != NULL) {
			straw->ChainFrom->Get_From(NULL);
			straw->ChainFrom = NULL;
		}

		if (BF.ChainTo != NULL) {
			BF.ChainTo->ChainFrom = NULL;
		}

		BF.ChainTo = straw;
		BF.ChainFrom = this;
		ChainTo = &BF;
		if (BF.ChainTo != NULL) {
			BF.ChainTo->ChainFrom = this;
		}
	}
}



/***********************************************************************************************
 * PKStraw::Key -- Assign a key to the cipher process straw.                                   *
 *                                                                                             *
 *    This routine will assign the key (or NULL if the current key is to be removed) to the    *
 *    cipher stream process. When a key has been assigned, encryption or decryption will       *
 *    take place. In the absence (NULL key pointer) of a key, the data passes through          *
 *    unchanged.                                                                               *
 *                                                                                             *
 * INPUT:   key   -- Pointer to the key to assign to the stream. If the key pointer is NULL,   *
 *                   then this causes the cipher stream to stop processing the data and will   *
 *                   pass the data through unchanged.                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure that the key passed to this routine is the opposite key to that used    *
 *             to process the stream originally (when decrypting).                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void PKStraw::Key(PKey const * key)
{
	CipherKey = key;
	if (key != NULL) {
		IsGettingKey = true;
	}
	Counter = 0;
	BytesLeft = 0;
}


/***********************************************************************************************
 * PKStraw::Get -- Fetch data and process it accordingly.                                      *
 *                                                                                             *
 *    This routine will fetch the number of bytes requested. If a valid key has been assigned  *
 *    to this stream, then the data will be processed as it passes through.                    *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer that will hold the requested data.               *
 *                                                                                             *
 *          length   -- The number of data bytes requested.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of data bytes stored to the destination buffer. If  *
 *          this number is less than that requested, then it indicates that the data source    *
 *          has been exhausted.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int PKStraw::Get(void * source, int length)
{
	/*
	**	If the parameters seem invalid, then pass the request on so that someone
	**	else can deal with it.
	*/
	if (source == NULL || length < 1 || CipherKey == NULL) {
		return(Straw::Get(source, length));
	}

	int total = 0;

	/*
	**	The first part of the data flow must process the special key. After the special
	**	key has been processed, the data flows through this straw without direct
	**	modification (the blowfish straw will process the data).
	*/
	if (IsGettingKey) {

		if (Control == DECRYPT) {

			/*
			**	Retrieve the pk encrypted blowfish key block.
			*/
			char cbuffer[MAX_KEY_BLOCK_SIZE];
			int got = Straw::Get(cbuffer, Encrypted_Key_Length());

			/*
			**	If the entire key block could not be retrieved, then this indicates
			**	a major data flow error -- just return with no action performed.
			*/
			if (got != Encrypted_Key_Length()) return(0);

			/*
			**	Decrypt the blowfish key and then activate the blowfish straw
			**	with that key.
			*/
			CipherKey->Decrypt(cbuffer, got, Buffer);
			BF.Key(Buffer, BLOWFISH_KEY_SIZE);

		} else {

			/*
			**	Generate the blowfish key by using random numbers.
			*/
			char buffer[MAX_KEY_BLOCK_SIZE];
			memset(buffer, '\0', sizeof(buffer));
			Rand.Get(buffer, BLOWFISH_KEY_SIZE);

			/*
			**	Encrypt the blowfish key (along with any necessary pad bytes).
			*/
			Counter = BytesLeft = CipherKey->Encrypt(buffer, Plain_Key_Length(), Buffer);
			BF.Key(buffer, BLOWFISH_KEY_SIZE);
		}

		/*
		**	The first phase of getting the special key has been accomplished. Now, all
		**	subsequent data is passed (unmodified) though this straw segment. The blowfish
		**	straw takes over the compression/decompression from this point forward.
		*/
		IsGettingKey = false;
	}

	/*
	**	If there are any pending bytes in the buffer, then pass
	**	these on first. The only time this should be is when the blowfish
	**	key has first been generated.
	*/
	if (BytesLeft > 0) {
		int tocopy = (length < BytesLeft) ? length : BytesLeft;
		memmove(source, &Buffer[Counter-BytesLeft], tocopy);
		source = (char *)source + tocopy;
		BytesLeft -= tocopy;
		length -= tocopy;
		total += tocopy;
	}

	/*
	**	Any requested bytes that haven't been satisfied are copied over now by
	**	drawing the data through the blowfish engine. The blowfish engine happens
	**	to be linked to the chain so a normal Get() operation is sufficient.
	*/
	total += Straw::Get(source, length);

	return(total);
}


/***********************************************************************************************
 * PKStraw::Encrypted_Key_Length -- Fetch the encrypted key length.                            *
 *                                                                                             *
 *    This returns the total number of bytes (after encryption) that the blowfish key will     *
 *    consume. It should be possible to get a block of this size, then pass it to the          *
 *    public key decrypter and the result will be the full blowfish key.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes that the encrypted blowfish key required.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int PKStraw::Encrypted_Key_Length(void) const
{
	if (CipherKey == NULL) return(0);
	return(CipherKey->Block_Count(BLOWFISH_KEY_SIZE) * CipherKey->Crypt_Block_Size());
}


/***********************************************************************************************
 * PKStraw::Plain_Key_Length -- Returns the number of bytes to encrypt key.                    *
 *                                                                                             *
 *    This is the number of plain (unencrypted) bytes that the blowfish key will take up. This *
 *    is actually the number of plain blocks minimum that can contain the full blowfish        *
 *    key. The public key cryptography system encrypts in whole blocks only.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the total number of bytes that will contain the full blowfish key     *
 *          and still be an even block size for the public key cryptography process.           *
 *                                                                                             *
 * WARNINGS:   This value is probably be larger than the actual blowfish key length.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int PKStraw::Plain_Key_Length(void) const
{
	if (CipherKey == NULL) return(0);
	return(CipherKey->Block_Count(BLOWFISH_KEY_SIZE) * CipherKey->Plain_Block_Size());
}
