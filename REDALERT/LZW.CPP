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

/* $Header: /CounterStrike/LZW.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LZW.CPP                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 08/28/96                                                     *
 *                                                                                             *
 *                  Last Update : August 28, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Find_Child_Node -- Find a matching dictionary entry.                                      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include	"xstraw.h"
#include	"xpipe.h"
#include	"buff.h"
#include	"lzw.h"


LZWEngine::LZWEngine(void)
{
	Reset();
}


void LZWEngine::Reset(void)
{
	for (int i = 0; i < TABLE_SIZE; i++) {
		dict[i].Make_Unused();
	}
}

int LZWEngine::Compress(Buffer const & input, Buffer const & output)
{
	BufferStraw instraw(input);
	BufferPipe outpipe(output);

	int outcount = 0;
	CodeType string_code = END_OF_STREAM;
	CodeType next_code = FIRST_CODE;

	string_code = 0;
	if (instraw.Get(&string_code, sizeof(char)) == 0) {
		string_code = END_OF_STREAM;
	}

	for (;;) {

		/*
		**	Fetch a character from the source data stream. If exhausted,
		**	then break out of the process loop so that the final code
		**	can be written out.
		*/
		unsigned char character;
		if (instraw.Get(&character, sizeof(character)) == 0) break;

		/*
		**	See if there is a match for the current code and current
		**	character. A match indicates that there is already a
		**	dictionary entry that fully represents the character
		**	sequence.
		*/
		int index = Find_Child_Node(string_code, character);

		/*
		**	If a code match was found, then set the current code
		**	value to this code value that represents the concatenation
		**	of the previous code value and the current character.
		*/
		if (index != -1 && dict[index].CodeValue != -1) {
			string_code = dict[index].CodeValue;
		} else {

			/*
			**	Since no exact match was found, then create a new code
			**	entry that represents the current code and character
			**	value concatenated. This presumes there is room in the
			**	code table.
			*/
			if (index != -1 && next_code <= MAX_CODE) {
				dict[index] = CodeClass(next_code, string_code, character);
				next_code++;
			}

			/*
			**	Output the code to the compression stream and reset the
			**	current code value to match the current character. This
			**	has the effect of clearing out the current character
			**	sequence scan in preparation for building a new one. It
			**	also ensures that the character will be written out.
			*/
			outcount += outpipe.Put(&string_code, sizeof(string_code));
			string_code = character;
		}
	}

	outcount += outpipe.Put(&string_code, sizeof(string_code));
	if (string_code != END_OF_STREAM) {
		string_code = END_OF_STREAM;
		outcount += outpipe.Put(&string_code, sizeof(string_code));
	}

	return(outcount);
}


int LZWEngine::Uncompress(Buffer const & input, Buffer const & output)
{
	int outcount = 0;
	BufferStraw instraw(input);
	BufferPipe outpipe(output);

	CodeType old_code;
	if (instraw.Get(&old_code, sizeof(old_code)) == 0) {
		return(outcount);
	}

	unsigned char character = (unsigned char)old_code;
	outcount += outpipe.Put(&character, sizeof(character));

	unsigned int count;
	CodeType new_code;
	CodeType next_code = FIRST_CODE;
	for (;;) {
		if (instraw.Get(&new_code, sizeof(new_code)) == 0) break;

		if (new_code == END_OF_STREAM) break;

		/*
		** This code checks for the CHARACTER+STRING+CHARACTER+STRING+CHARACTER
		** case which generates an undefined code.  It handles it by decoding
		** the last code, and adding a single character to the end of the decode string.
		*/
		if (new_code >= next_code) {
			decode_stack[0] = character;
			count = 1;
			count += Decode_String(&decode_stack[1], old_code);
		} else {
			count = Decode_String(decode_stack, new_code);
		}

		character = decode_stack[count-1];
		while (count > 0) {
			--count;
			outcount += outpipe.Put(&decode_stack[count], sizeof(decode_stack[0]));
		}

		/*
		**	Add the new code sequence to the dictionary (presuming there is still
		**	room).
		*/
		if (next_code <= MAX_CODE) {
			dict[next_code] = CodeClass(next_code, old_code, character);
			next_code++;
		}
		old_code = new_code;
	}

	return(outcount);
}


int LZWEngine::Make_LZW_Hash(CodeType code, char character)
{
	return((((int)(unsigned char)character) << ( BITS - 8 ) ) ^ (int)code);
}


int LZWEngine::Find_Child_Node(CodeType parent_code, char child_character)
{
	/*
	**	Fetch the first try index for the code and character.
	*/
	int hash_index = Make_LZW_Hash(parent_code, child_character);

	/*
	**	Base the hash-miss-try-again-displacement value on the current
	**	index. [Shouldn't the value be some large prime number???].
	*/
	int offset = 1;
	if (hash_index != 0) {
		offset = TABLE_SIZE - hash_index;
	}

	/*
	**	Keep offsetting through the dictionary until an exact match is
	**	found for the code and character specified.
	*/
	int initial = hash_index;
	while (!dict[hash_index].Is_Matching(parent_code, child_character)) {

		/*
		**	Stop searching if an unused index is found since this means that
		**	a match doesn't exist in the table at all.
		*/
		if (dict[hash_index].Is_Unused()) break;

		/*
		**	Bump the hash index to another value such that sequential bumps
		**	will not result in the same index value until all of the table
		**	has been scanned.
		*/
		hash_index -= offset;
		if (hash_index < 0) {
			hash_index += TABLE_SIZE;
		}

		/*
		**	If the entire table has been scanned and no match or unused
		**	entry was found, then return a special value indicating this
		**	condition.
		*/
		if (initial == hash_index) {
			hash_index = -1;
			break;
		}
	}
	return(hash_index);
}


int LZWEngine::Decode_String(char * ptr, CodeType code)
{
	int count = 0;
	while (code > 255) {
		*ptr++ = dict[code].CharValue;
		count++;
		code = dict[code].ParentCode;
	}
	*ptr = (char)code;
	count++;
	return(count);
}


int LZW_Uncompress(Buffer const & inbuff, Buffer const & outbuff)
{
	LZWEngine lzw;
	return(lzw.Uncompress(inbuff, outbuff));
}


int LZW_Compress(Buffer const & inbuff, Buffer const & outbuff)
{
	LZWEngine lzw;
	return(lzw.Compress(inbuff, outbuff));
}





#ifdef NEVER


/*
 * Constants used throughout the program.  BITS defines how many bits
 * will be in a code.  TABLE_SIZE defines the size of the dictionary
 * table.
 */
#define BITS                       12
#define MAX_CODE                   ( ( 1 << BITS ) - 1 )
#define TABLE_SIZE                 5021
#define END_OF_STREAM              256
#define FIRST_CODE                 257
#define UNUSED                     -1

typedef unsigned short CodeType;

/*
 * This data structure defines the dictionary.  Each entry in the dictionary
 * has a code value.  This is the code emitted by the compressor.  Each
 * code is actually made up of two pieces:  a parent_code, and a
 * character.  Code values of less than 256 are actually plain
 * text codes.
 */
struct CodeClass
{
	CodeType CodeValue;
	CodeType ParentCode;
	char CharValue;

	CodeClass(void) {}
	CodeClass(CodeType code, CodeType parent, char c) : CodeValue(code), ParentCode(parent), CharValue(c) {}

	void Make_Unused(void) {CodeValue = UNUSED;}
	bool Is_Unused(void) const {return(CodeValue == UNUSED);}
	bool Is_Matching(CodeType code, char c) const {return(ParentCode == code && CharValue == c);}
};
CodeClass dict[TABLE_SIZE];

char decode_stack[TABLE_SIZE];

inline int Make_LZW_Hash(CodeType code, char character)
{
	return((((int)(unsigned char)character) << ( BITS - 8 ) ) ^ (int)code);
}


/***********************************************************************************************
 * Find_Child_Node -- Find a matching dictionary entry.                                        *
 *                                                                                             *
 *    This hashing routine is responsible for finding the table location                       *
 *    for a string/character combination.  The table index is created                          *
 *    by using an exclusive OR combination of the prefix and character.                        *
 *    This code also has to check for collisions, and handles them by                          *
 *    jumping around in the table.                                                             *
 *                                                                                             *
 * INPUT:   parent_code -- The code of the parent string sequence.                             *
 *                                                                                             *
 *          character   -- The current character.                                              *
 *                                                                                             *
 * OUTPUT:  Returns with the index for the matching dictionary entry. If no matching index     *
 *          could be found, then it returns with the index to an unused dictionary entry. If   *
 *          there are also no unused entries in the dictionary, then -1 is returned.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/28/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static int Find_Child_Node(CodeType parent_code, char child_character)
{
	/*
	**	Fetch the first try index for the code and character.
	*/
	int hash_index = Make_LZW_Hash(parent_code, child_character);

	/*
	**	Base the hash-miss-try-again-displacement value on the current
	**	index. [Shouldn't the value be some large prime number???].
	*/
	int offset = 1;
	if (hash_index != 0) {
		offset = TABLE_SIZE - hash_index;
	}

	/*
	**	Keep offsetting through the dictionary until an exact match is
	**	found for the code and character specified.
	*/
	int initial = hash_index;
	while (!dict[hash_index].Is_Matching(parent_code, child_character)) {

		/*
		**	Stop searching if an unused index is found since this means that
		**	a match doesn't exist in the table at all.
		*/
		if (dict[hash_index].Is_Unused()) break;

		/*
		**	Bump the hash index to another value such that sequential bumps
		**	will not result in the same index value until all of the table
		**	has been scanned.
		*/
		hash_index -= offset;
		if (hash_index < 0) {
			hash_index += TABLE_SIZE;
		}

		/*
		**	If the entire table has been scanned and no match or unused
		**	entry was found, then return a special value indicating this
		**	condition.
		*/
		if (initial == hash_index) {
			hash_index = -1;
			break;
		}
	}
	return(hash_index);
}


/*
 * This routine decodes a string from the dictionary, and stores it
 * in the decode_stack data structure.  It returns a count to the
 * calling program of how many characters were placed in the stack.
 */
static int Decode_String(char * ptr, CodeType code)
{
	int count = 0;
	while (code > 255) {
		*ptr++ = dict[code].CharValue;
		count++;
		code = dict[code].ParentCode;
	}
	*ptr = (char)code;
	count++;
	return(count);
}


/*
 * The compressor is short and simple.  It reads in new symbols one
 * at a time from the input file.  It then  checks to see if the
 * combination of the current symbol and the current code are already
 * defined in the dictionary.  If they are not, they are added to the
 * dictionary, and we start over with a new one symbol code.  If they
 * are, the code for the combination of the code and character becomes
 * our new code.
 */

int LZW_Compress(Buffer & inbuff, Buffer & outbuff)
{
	BufferStraw input(inbuff);
	BufferPipe output(outbuff);

	for (int i = 0; i < TABLE_SIZE; i++) {
		dict[i].Make_Unused();
//		dict[i].code_value = UNUSED;
	}

	int outcount = 0;
	CodeType string_code = END_OF_STREAM;
	CodeType next_code = FIRST_CODE;
	for (;;) {
		char character;

		if (input.Get(&character, sizeof(character)) == 0) break;

		int index = Find_Child_Node(string_code, character);

		if (index == -1) {
			string_code = character;
			outcount += output.Put(&string_code, sizeof(string_code));
		} else {

			if (dict[index].CodeValue != -1) {
				string_code = dict[ index ].CodeValue;
			} else {
				if (next_code <= MAX_CODE) {
					dict[index] = CodeClass(next_code++, string_code, character);
				}
				outcount += output.Put(&string_code, sizeof(string_code));
				string_code = character;
			}
		}
	}

	outcount += output.Put(&string_code, sizeof(string_code));
	string_code = END_OF_STREAM;
	outcount += output.Put(&string_code, sizeof(string_code));

	return(outcount);
}


/*
 * The file expander operates much like the encoder.  It has to
 * read in codes, the convert the codes to a string of characters.
 * The only catch in the whole operation occurs when the encoder
 * encounters a CHAR+STRING+CHAR+STRING+CHAR sequence.  When this
 * occurs, the encoder outputs a code that is not presently defined
 * in the table.  This is handled as an exception.
 */
int LZW_Uncompress(Buffer & inbuff, Buffer & outbuff)
{
	int outcount = 0;
	BufferStraw input(inbuff);
	BufferPipe output(outbuff);

	CodeType old_code;
	if (input.Get(&old_code, sizeof(old_code)) == 0) {
		return(outcount);
	}

	char character = (char)old_code;
	outcount += output.Put(&character, sizeof(character));

	unsigned int count;
	CodeType new_code;
	CodeType next_code = FIRST_CODE;
	for (;;) {
		if (input.Get(&new_code, sizeof(new_code)) == 0) break;

		/*
		** This code checks for the CHARACTER+STRING+CHARACTER+STRING+CHARACTER
		** case which generates an undefined code.  It handles it by decoding
		** the last code, and adding a single character to the end of the decode string.
		*/
		if (new_code >= next_code) {
			decode_stack[0] = character;
			count = 1;
			count += Decode_String(&decode_stack[1], old_code);
		} else {
			count = Decode_String(decode_stack, new_code);
		}

		character = decode_stack[count-1];
		while (count > 0) {
			--count;
			outcount += output.Put(&decode_stack[count], sizeof(decode_stack[0]));
		}

		/*
		**	Add the new code sequence to the dictionary (presuming there is still
		**	room).
		*/
		if (next_code <= MAX_CODE) {
			dict[next_code] = CodeClass(next_code, old_code, character);
			next_code++;
		}
		old_code = new_code;
	}

	return(outcount);
}

#endif
