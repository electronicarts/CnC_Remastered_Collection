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

/* $Header: /CounterStrike/PKPIPE.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : PKPIPE.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/06/96                                                     *
 *                                                                                             *
 *                  Last Update : July 6, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef PKPIPE_H
#define PKPIPE_H

#include	"pipe.h"
#include	"pk.h"
#include "rndstraw.h"
#include	"blowpipe.h"


/*
**	This pipe will encrypt/decrypt the data stream. The data is encrypted by generating a
**	symetric key that is then encrypted using the public key system. This symetric key is then
**	used to encrypt the remaining data.
*/
class PKPipe : public Pipe
{
	public:
		typedef enum CryptControl {
			ENCRYPT,
			DECRYPT
		} CryptControl;

		PKPipe(CryptControl control, RandomStraw & rnd);

		virtual void Put_To(Pipe * pipe);
		virtual void Put_To(Pipe & pipe) {Put_To(&pipe);}

		// Feed data through for processing.
		virtual int Put(void const * source, int length);

		// Submit key for encryption/decryption.
		void Key(PKey const * key);

	private:
		enum {
			BLOWFISH_KEY_SIZE=BlowfishEngine::MAX_KEY_LENGTH,
			MAX_KEY_BLOCK_SIZE=256		// Maximum size of pk encrypted blowfish key.
		};

		/*
		**	This flag indicates whether the PK (fetch blowfish key) phase is
		**	in progress or not.
		*/
		bool IsGettingKey;

		/*
		**	This is the random straw that is needed to generate the
		**	blowfish key.
		*/
		RandomStraw & Rand;

		/*
		**	This is the attached blowfish pipe. After the blowfish key has been
		**	decrypted, then the PK processor goes dormant and the blowfish processor
		**	takes over the data flow.
		*/
		BlowPipe BF;

		/*
		**	Controls the method of processing the data stream.
		*/
		CryptControl Control;

		/*
		**	Pointer to the key to use for encryption/decryption. The actual process
		**	performed is controlled by the Control member. A key can be used for
		**	either encryption or decryption -- it makes no difference. However, whichever
		**	process is performed, the opposite process must be performed using the
		**	other key.
		*/
		PKey const * CipherKey;

		/*
		**	This is the staging buffer for the block of data. This block must be as large as
		**	the largest possible key size or the largest blowfish key (whichever is greater).
		*/
		char Buffer[MAX_KEY_BLOCK_SIZE];

		/*
		**	The working counter that holds the number of bytes in the staging buffer.
		*/
		int Counter;

		/*
		**	This records the number of bytes remaining in the current block. This
		**	will be the number of bytes left to accumulate before the block can be
		**	processed either for encryption or decryption.
		*/
		int BytesLeft;

		int Encrypted_Key_Length(void) const;
		int Plain_Key_Length(void) const;

		PKPipe(PKPipe & rvalue);
		PKPipe & operator = (PKPipe const & pipe);
};


#endif
