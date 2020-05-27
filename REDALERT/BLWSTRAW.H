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

/* $Header: /CounterStrike/BLWSTRAW.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BLWSTRAW.H                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/02/96                                                     *
 *                                                                                             *
 *                  Last Update : July 2, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef BLWSTRAW_H
#define BLWSTRAW_H

#include	"straw.h"
#include	"blowfish.h"


/*
**	Performs Blowfish encryption/decryption to the data that is drawn through this straw. The
**	process is controlled by the key which must be submitted to the class before any data
**	manipulation will occur. The Blowfish algorithm is symmetric, thus the same key is used
**	for encryption as is for decryption.
*/
class BlowStraw : public Straw
{
	public:
		typedef enum CryptControl {
			ENCRYPT,
			DECRYPT
		} CryptControl;

		BlowStraw(CryptControl control) : BF(NULL), Counter(0), Control(control) {}
		virtual ~BlowStraw(void) {delete BF;BF = NULL;}

		virtual int Get(void * source, int slen);

		// Submit key for blowfish engine.
		void Key(void const * key, int length);

	protected:
		/*
		**	The Blowfish engine used for encryption/decryption. If this pointer is
		**	NULL, then this indicates that the blowfish engine is not active and no
		**	key has been submitted. All data would pass through this straw unchanged
		**	in that case.
		*/
		BlowfishEngine * BF;

	private:
		char Buffer[8];
		int Counter;
		CryptControl Control;

		BlowStraw(BlowStraw & rvalue);
		BlowStraw & operator = (BlowStraw const & straw);
};


#endif
