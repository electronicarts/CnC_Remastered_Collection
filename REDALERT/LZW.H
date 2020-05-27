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

/* $Header: /CounterStrike/LZW.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LZW.H                                                        *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 08/28/96                                                     *
 *                                                                                             *
 *                  Last Update : August 28, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef _LZW_H
#define _LZW_H

#include	"buff.h"

class LZWEngine
{
	public:
		LZWEngine(void);

		int Compress(Buffer const & input, Buffer const & output);
		int Uncompress(Buffer const & input, Buffer const & output);

		void Reset(void);

	private:
		typedef short CodeType;
		struct CodeClass {
			CodeType CodeValue;
			CodeType ParentCode;
			char CharValue;

			CodeClass(void) {}
			CodeClass(CodeType code, CodeType parent, char c) : CodeValue(code), ParentCode(parent), CharValue(c) {}

			enum {UNUSED=-1};
			void Make_Unused(void) {CodeValue = UNUSED;}
			bool Is_Unused(void) const {return(CodeValue == UNUSED);}
			bool Is_Matching(CodeType code, char c) const {return(ParentCode == code && CharValue == c);}
		};

		enum {
			BITS=12,
			MAX_CODE=((1 << BITS ) - 1),
			FIRST_CODE=257,
			END_OF_STREAM=256,
			TABLE_SIZE=5021
		};
		CodeClass dict[TABLE_SIZE];

		char decode_stack[TABLE_SIZE];

		int Find_Child_Node(CodeType parent_code, char child_character);
		int Decode_String(char * ptr, CodeType code);
		static int Make_LZW_Hash(CodeType code, char character);
};


int LZW_Compress(Buffer const & inbuff, Buffer const & outbuff);
int LZW_Uncompress(Buffer const & inbuff, Buffer const & outbuff);

#endif
