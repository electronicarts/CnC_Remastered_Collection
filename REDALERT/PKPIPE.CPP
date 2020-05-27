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

/* $Header: /CounterStrike/PKPIPE.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                                                                                             *
 *                    File Name : PKPIPE.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/07/96                                                     *
 *                                                                                             *
 *                  Last Update : July 12, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   PKPipe::Encrypted_Key_Length -- Fetch the encrypted key length.                           *
 *   PKPipe::Key -- Submit a key to enable processing of data flow.                            *
 *   PKPipe::PKPipe -- Constructor for the public key pipe object.                             *
 *   PKPipe::Plain_Key_Length -- Returns the number of bytes to encrypt key.                   *
 *   PKPipe::Put -- Submit data to the pipe for processing.                                    *
 *   PKPipe::Put_To -- Chains one pipe to another.                                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"pkpipe.h"


/***********************************************************************************************
 * PKPipe::PKPipe -- Constructor for the public key pipe object.                               *
 *                                                                                             *
 *    This will construct the public key pipe object.                                          *
 *                                                                                             *
 * INPUT:   control  -- The method used to process the data flow (encrypt or decrypt).         *
 *                                                                                             *
 *          rnd      -- Reference to a random number generate used to create the internal      *
 *                      blowfish key.                                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
PKPipe::PKPipe(CryptControl control, RandomStraw & rnd) :
	IsGettingKey(true),
	Rand(rnd),
	BF((control == ENCRYPT) ? BlowPipe::ENCRYPT : BlowPipe::DECRYPT),
	Control(control),
	CipherKey(NULL),
	Counter(0),
	BytesLeft(0)
{
}


/***********************************************************************************************
 * PKPipe::Put_To -- Chains one pipe to another.                                               *
 *                                                                                             *
 *    This handles linking of one pipe to this pipe. Data will flow from this PKPipe to the    *
 *    pipe segment specified. Special handling is done so that piping actually flows to the    *
 *    embedded blowfish pipe and then flows to the designated pipe.                            *
 *                                                                                             *
 * INPUT:   pipe  -- Pointer to the pipe that this pipe segment is to send data to.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void PKPipe::Put_To(Pipe * pipe)
{
	if (BF.ChainTo != pipe) {
		if (pipe != NULL && pipe->ChainFrom != NULL) {
			pipe->ChainFrom->Put_To(NULL);
			pipe->ChainFrom = NULL;
		}

		if (BF.ChainTo != NULL) {
			BF.ChainTo->ChainFrom = NULL;
		}
		BF.ChainTo = pipe;
		if (pipe != NULL) {
			pipe->ChainFrom = &BF;
		}
		BF.ChainFrom = this;
		ChainTo = &BF;
	}
}


/***********************************************************************************************
 * PKPipe::Key -- Submit a key to enable processing of data flow.                              *
 *                                                                                             *
 *    This routine must be called with a valid key pointer in order for encryption/description *
 *    to be performed on the data stream. Prior to calling this routine or after calling this  *
 *    routine with a NULL pointer, the data stream will pass through this pipe without         *
 *    modification.                                                                            *
 *                                                                                             *
 * INPUT:   key   -- Pointer to the key to use for processing. Pass NULL if process is to be   *
 *                   terminated.                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/07/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void PKPipe::Key(PKey const * key)
{
	if (key == NULL) {
		Flush();
		IsGettingKey = false;
	}
	CipherKey = key;

	if (CipherKey != NULL) {
		IsGettingKey = true;
		if (Control == DECRYPT) {
			Counter = BytesLeft = Encrypted_Key_Length();
		}
	}
}


/***********************************************************************************************
 * PKPipe::Put -- Submit data to the pipe for processing.                                      *
 *                                                                                             *
 *    This routine (if processing as been enabled by a previous key submission) will           *
 *    encrypt or decrypt the data stream that passes through it. When encrypting, the data     *
 *    stream will increase in size by about 10% (bit it varies according to the key used).     *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the data to be submitted to the pipe stream.                *
 *                                                                                             *
 *          length   -- The number of bytes submitted.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of byte output at the final end of the pipe.        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/07/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int PKPipe::Put(void const * source, int length)
{
	/*
	**	If the parameter seem illegal, then pass the pipe request to the
	**	next pipe in the chain and let them deal with it.
	*/
	if (source == NULL || length < 1 || CipherKey == NULL) {
		return(Pipe::Put(source, length));
	}

	int total = 0;

	/*
	**	Perform a special process if the this is the first part of the data flow. The special
	**	key must be processed first. After this initial key processing, the rest of the data flow
	**	is processed by the blowfish pipe and ignored by the PKPipe.
	*/
	if (IsGettingKey) {

		/*
		**	When encrypting, first make the key block and then pass the data through the
		**	normal blowfish processor.
		*/
		if (Control == ENCRYPT) {

			/*
			**	Generate the largest blowfish key possible.
			*/
			char buffer[MAX_KEY_BLOCK_SIZE];
			memset(buffer, '\0', sizeof(buffer));
			Rand.Get(buffer, BLOWFISH_KEY_SIZE);

			/*
			**	Encrypt the blowfish key (along with any necessary pad bytes).
			*/
			int didput = CipherKey->Encrypt(buffer, Plain_Key_Length(), Buffer);
			total += Pipe::Put(Buffer, didput);
			BF.Key(buffer, BLOWFISH_KEY_SIZE);

			IsGettingKey = false;

		} else {

			/*
			**	First try to accumulate a full key.
			*/
			int toget = (BytesLeft < length) ? BytesLeft : length;
			memmove(&Buffer[Counter-BytesLeft], source, toget);
			length -= toget;
			BytesLeft -= toget;
			source = (char *)source + toget;

			/*
			**	If a full key has been accumulated, then decrypt it and feed the
			**	key to the blowfish engine.
			*/
			if (BytesLeft == 0) {
				char buffer[MAX_KEY_BLOCK_SIZE];
				CipherKey->Decrypt(Buffer, Counter, buffer);
				BF.Key(buffer, BLOWFISH_KEY_SIZE);

				IsGettingKey = false;
			}
		}
	}

	/*
	**	If there are any remaining bytes to pipe through, then
	**	pipe them through now -- they will be processed by the
	**	blowfish engine.
	*/
	total += Pipe::Put(source, length);
	return(total);
}


/***********************************************************************************************
 * PKPipe::Encrypted_Key_Length -- Fetch the encrypted key length.                             *
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
int PKPipe::Encrypted_Key_Length(void) const
{
	if (CipherKey == NULL) return(0);
	return(CipherKey->Block_Count(BLOWFISH_KEY_SIZE) * CipherKey->Crypt_Block_Size());
}


/***********************************************************************************************
 * PKPipe::Plain_Key_Length -- Returns the number of bytes to encrypt key.                     *
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
int PKPipe::Plain_Key_Length(void) const
{
	if (CipherKey == NULL) return(0);
	return(CipherKey->Block_Count(BLOWFISH_KEY_SIZE) * CipherKey->Plain_Block_Size());
}
