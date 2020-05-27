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

/* $Header: /CounterStrike/MP.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MP.CPP                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 04/26/96                                                     *
 *                                                                                             *
 *                  Last Update : July 2, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   _Byte_Precision -- Determines the number of bytes significant in long integer.            *
 *   memrev -- Reverse the byte order of the buffer specified.                                 *
 *   XMP_Abs -- Perform an absolute value on the specified MP number.                          *
 *   XMP_Add -- Add two MP numbers with a carry option.                                        *
 *   XMP_Add_Int -- Add an integer to an MP number (with carry).                               *
 *   XMP_Compare -- Compare one MP number with another.                                        *
 *   XMP_Count_Bits -- Count the total number of bits (precision) in MP number.                *
 *   XMP_Count_Bytes -- Counts the number of precision bytes in MP number.                     *
 *   XMP_Dec -- Decrement the MP number by one.                                                *
 *   XMP_Decode_ASCII -- Convert ASCII into an MP number.                                      *
 *   XMP_DER_Decode -- Decode a DER number into an MP number.                                  *
 *   XMP_DER_Encode -- Encode a number into a buffer using DER.                                *
 *   XMP_DER_Length_Encode -- Output the length of a DER block.                                *
 *   XMP_Double_Mul -- Double precision MP multiply.                                           *
 *   XMP_Encode -- Encode MP number into buffer as compactly as possible.                      *
 *   XMP_Fermat_Test -- Performs Fermat's Little Theorem on an MP number.                      *
 *   XMP_Hybrid_Mul -- Special hybrid short multiply (with carry).                             *
 *   XMP_Inc -- Increment an MP number by one.                                                 *
 *   XMP_Init -- Initialize an MP number to a starting value.                                  *
 *   XMP_Inverse_A_Mod_B -- Inverts and performs modulus on an MP number.                      *
 *   XMP_Is_Prime -- Determine if the specified MP number is prime.                            *
 *   XMP_Is_Small_Prime -- Determine if MP number is a small prime.                            *
 *   XMP_Mod_Mult -- Perform a multiply - modulus operation.                                   *
 *   XMP_Mod_Mult_Clear -- Remove temporary values from memory.                                *
 *   XMP_Move -- Assign one MP number to another.                                              *
 *   XMP_Neg -- Negate the specified MP number.                                                *
 *   XMP_Not -- Perform bitwise NOT operation on MP number.                                    *
 *   XMP_Prepare_Modulus -- Prepare globals for modulus operation.                             *
 *   XMP_Rabin_Miller_Test -- Performs the Rabin Miller test for primality.                    *
 *   XMP_Randomize -- Generate a random MP number between the boundaries specified.            *
 *   XMP_Randomize -- Generate a random MP number.                                             *
 *   XMP_Reciprocal -- Compute the reciprocal (inverse) of the MP number.                      *
 *   XMP_Rotate_Left -- Rotate specified MP number leftward.                                   *
 *   XMP_Shift_Left_Bits -- Shifts the MP number left by the specified bit count.              *
 *   XMP_Shift_Right_Bits -- Shift the MP number right by specified bit count.                 *
 *   XMP_Signed_Decode -- Decode a number as if it were signed.                                *
 *   XMP_Signed_Div -- Signed divide of one MP number into another.                            *
 *   XMP_Signed_Mult -- A signed multiply between two MP numbers.                              *
 *   XMP_Signed_Mult_Int -- Multiply an MP number by a signed simple integer.                  *
 *   XMP_Significance -- Fetch the precision (bytes) of the MP number.                         *
 *   XMP_Small_Divisors_Test -- Perform the small divisors test on an MP number.               *
 *   XMP_Sub -- Subtract one MP number from another (with borrow).                             *
 *   XMP_Sub_Int -- Subtract an integer from an MP number (with borrow).                       *
 *   XMP_Unsigned_Decode -- Decode a number as if it were unsigned.                            *
 *   XMP_Unsigned_Div -- Unsigned divide of one MP number into another.                        *
 *   XMP_Unsigned_Div_Int -- Perform a short integer divide into an MP number.                 *
 *   XMP_Unsigned_Mult -- Multiply two unsigned MP numbers together.                           *
 *   XMP_Unsigned_Mult_Int -- Multiply an MP number by a simple integer.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<assert.h>
#include	<limits.h>
#include	"mp.h"


#ifndef __BORLANDC__
#define	min(a, b)	(((a) < (b)) ? (a) : (b))
#define	_USERENTRY
#endif


/***********************************************************************************************
 * _Byte_Precision -- Determines the number of bytes significant in long integer.              *
 *                                                                                             *
 *    This utility routine will determine the number of precision bytes exist in the long      *
 *    integer specified. There are some optimizations that can occur if the byte precision     *
 *    is known.                                                                                *
 *                                                                                             *
 * INPUT:   value -- The value of the long integer that the byte precision will be calculated  *
 *                   for.                                                                      *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes that the long integer requires (at a minimum)     *
 *          to cover the precision of the integer. The minimum value will be 1, the maximum    *
 *          will be 4.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static int _Byte_Precision(unsigned long value)
{
	int byte_count;
	for (byte_count = sizeof(value); byte_count; byte_count--) {
		if (value >> ((byte_count-1)*8)) break;
	}
	return(byte_count);
}


/***********************************************************************************************
 * XMP_DER_Length_Encode -- Output the length of a DER block.                                  *
 *                                                                                             *
 *    This routine will output the length of the block using Distinguished Encoding Rules.     *
 *    The rest of the block must be filled in as appropriate. For data blocks that are less    *
 *    than 128 bytes long, the header consists of only one byte. Longer buffers lengths        *
 *    can consume up to 5 bytes (depends on magnitude of the length value).                    *
 *                                                                                             *
 * INPUT:   length   -- The length of the data block to be output.                             *
 *                                                                                             *
 *          output   -- Pointer to the memory block that will be set up.                       *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes (header) that was used to store the length        *
 *          value. Subsequent data must be placed after the header.                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_DER_Length_Encode(unsigned long length, unsigned char * output)
{
	assert(output != NULL);

	int header_length = 0;

	if (length <= SCHAR_MAX) {
		output[header_length++] = (unsigned char)length;
	} else {
		output[header_length++] = (unsigned char)(_Byte_Precision(length) | 0x80);
		for (int byte_counter = _Byte_Precision(length); byte_counter; --byte_counter) {
			output[header_length++] = (unsigned char)(length >> ((byte_counter-1)*8));
		}
	}
	return(header_length);
}


/***********************************************************************************************
 * XMP_DER_Encode -- Encode a number into a buffer using DER.                                  *
 *                                                                                             *
 *    This routine is used to encode a number into a buffer using Distinguished Encoding       *
 *    Rules. The number of bytes used will be, typically, two bytes more than the number of    *
 *    precision bytes in the number.                                                           *
 *                                                                                             *
 * INPUT:   from     -- Pointer to the multi-precision number.                                 *
 *                                                                                             *
 *          output   -- Pointer to the buffer that will hold the DER encoded number.           *
 *                                                                                             *
 *          precision-- The precision of the multi-precision number.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes used in the output buffer.                        *
 *                                                                                             *
 * WARNINGS:   Make sure the buffer is big enough to hold the DER encoded number. For safety   *
 *             make sure it is precision+6 bytes long.                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_DER_Encode(digit const * from, unsigned char * output, int precision)
{
	assert(from != NULL);
	assert(output != NULL);
	assert(precision > 0);

	unsigned char buffer[MAX_UNIT_PRECISION*sizeof(digit)+1];
	int header_count = 0;

	unsigned number_count = XMP_Encode(buffer, from, precision);

	output[header_count++] = 0x02;
	header_count += XMP_DER_Length_Encode(number_count, &output[header_count]);
	memcpy(&output[header_count], buffer, number_count);

	return(header_count+number_count);
}


/***********************************************************************************************
 * XMP_DER_Decode -- Decode a DER number into an MP number.                                    *
 *                                                                                             *
 *    Use this routine to decode a Distinguished Encoding Rules number into a multi-precision  *
 *    number. This is the counterpart function to the XMP_DER_Encode() function.               *
 *                                                                                             *
 * INPUT:   result      -- The buffer the hold the result MP number.                           *
 *                                                                                             *
 *          input       -- Pointer to the DER encoded number.                                  *
 *                                                                                             *
 *          precision   -- The precision of the MP number. This is the maximum precision the   *
 *                         DER number can be.                                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_DER_Decode(digit * result, unsigned char const * input, int precision)
{
	assert(result != NULL);
	assert(input != NULL);
	assert(precision > 0);

	if (*input++ == 0x02) {
		unsigned byte_count;

		if ((*input & 0x80) == 0) {
			byte_count = *input++;
		} else {
			int length = *input++ & 0x7f;
			if (length > 2) return;
			byte_count = *input++;
			if (length > 1) byte_count = (byte_count << 8) | *input++;
		}
		if (byte_count <= (precision * sizeof(digit))) {
			XMP_Signed_Decode(result, input, byte_count, precision);
		}
	}
}


/***********************************************************************************************
 * XMP_Encode -- Encode MP number into buffer.                                                 *
 *                                                                                             *
 *    This routine will encode an multi-precision number into a buffer of specified length.    *
 *    The number of stored in "big endian" format with appropriate sign extension.             *
 *                                                                                             *
 * INPUT:   to       -- Pointer to the buffer to place the number.                             *
 *                                                                                             *
 *          tobytes  -- The number of bytes to use in the destination buffer.                  *
 *                                                                                             *
 *          from     -- Pointer to the MP number to be encoded.                                *
 *                                                                                             *
 *          precision-- The precision of the MP number.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes placed into the destination buffer.               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
unsigned XMP_Encode(unsigned char * to, unsigned tobytes, digit const * from, int precision)
{
	assert(to != NULL);
	assert(from != NULL);
	assert(tobytes > 0);
	assert(precision > 0);

	unsigned frombytes = precision * sizeof(digit);
	unsigned char filler = (unsigned char)(XMP_Is_Negative(from, precision) ? 0xff : 0);

	int index;
	for (index = 0; index < (int)(tobytes-frombytes); index++) {
		*to++ = filler;
	}

	const unsigned char * fptr = ((const unsigned char *)from) + min(tobytes, frombytes);
	for (index = 0; index < (int)min(tobytes, frombytes); index++) {
		*to++ = *--fptr;
	}

	return(tobytes);
}


/***********************************************************************************************
 * XMP_Encode -- Encode MP number into buffer as compactly as possible.                        *
 *                                                                                             *
 *    This routine will encode the MP number into the specified buffer. The number will be     *
 *    encoded using the least number of bytes possible.                                        *
 *                                                                                             *
 * INPUT:   to       -- The buffer to encode the MP number into.                               *
 *                                                                                             *
 *          from     -- Pointer to the MP number to be encoded.                                *
 *                                                                                             *
 *          precision-- The precision of the MP number.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes used in the destination buffer to hold the        *
 *          encoded number.                                                                    *
 *                                                                                             *
 * WARNINGS:   Be sure the destination buffer is big enough to hold the encoded MP number.     *
 *             A safe size would be the precision plus one.                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
//#pragma warning 364 9
unsigned XMP_Encode(unsigned char * to, digit const * from, int precision)
{
	assert(to != NULL);
	assert(from != NULL);
	assert(precision > 0);

	bool is_negative = XMP_Is_Negative(from, precision);
	unsigned char filler = (unsigned char)(is_negative ? 0xff : 0);
	unsigned char * number_ptr;

	unsigned char * const end = (unsigned char *)from;
	for (number_ptr = (unsigned char *)end + precision - 1; number_ptr > (unsigned char *)end; number_ptr--) {
		if (*number_ptr != filler) break;
	}

	unsigned index = 0;
	if (((*number_ptr & 0x80) && !is_negative) || (!(*number_ptr & 0x80) && is_negative)) {
		to[index++] = filler;
	}

	to[index++] = *number_ptr;

	while (number_ptr != end) {
		to[index++] = *--number_ptr;
	}
	return(index);
}


/***********************************************************************************************
 * XMP_Signed_Decode -- Decode a number as if it were signed.                                  *
 *                                                                                             *
 *    Use this routine to convert a coded number back into an MP number. The coded number      *
 *    is presumed to be signed.                                                                *
 *                                                                                             *
 * INPUT:   result   -- Pointer to the buffer that will hold the decoded MP number.            *
 *                                                                                             *
 *          from     -- Pointer to the encoded MP number.                                      *
 *                                                                                             *
 *          frombytes-- The number of bytes consumed by the encoded MP number.                 *
 *                                                                                             *
 *          precision -- The precision of the MP number (maximum) of the result.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure that the precision is sufficient to hold the decoded MP number.         *
 *             Otherwise, the result is undefined.                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Signed_Decode(digit * result, const unsigned char * from, int frombytes, int precision)
{
	assert(result != NULL);
	assert(from != NULL);
	assert(frombytes > 0);
	assert(precision > 0);

	unsigned char filler = (unsigned char)((*from & 0x80) ? 0xff : 0);

	int fillcount = precision * sizeof(digit) - frombytes;
	unsigned char * dest = (unsigned char *)&result[precision];

	/*
	**	Fill in any excess significant bytes.
	*/
	int index;
	for (index = 0; index < fillcount; index++) {
		*--dest = filler;
	}

	/*
	**	Move in the remaining bytes.
	*/
	for (index = 0; index < frombytes; index++) {
		*--dest = *from++;
	}
}


/***********************************************************************************************
 * XMP_Unsigned_Decode -- Decode a number as if it were unsigned.                              *
 *                                                                                             *
 *    Use this routine to decode a MP number and treat it as if it were unsigned.              *
 *                                                                                             *
 * INPUT:   result   -- Pointer to the buffer to hold the result MP number.                    *
 *                                                                                             *
 *          from     -- Pointer to the encoded MP number.                                      *
 *                                                                                             *
 *          frombytes-- The number of bytes in the encoded number.                             *
 *                                                                                             *
 *          precision-- The precision of the result MP number -- maximum precision.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure the result MP precision is sufficient to hold the decoded number or     *
 *             else the result is undefined.                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Unsigned_Decode(digit * result, const unsigned char * from, int frombytes, int precision)
{
	assert(result != NULL);
	assert(from != NULL);
	assert(frombytes > 0);
	assert(precision > 0);

	int fillcount = precision * sizeof(digit) - frombytes;
	unsigned char * dest = (unsigned char *)&result[precision];

	/*
	**	Fill in any excess significant bytes.
	*/
	int index;
	for (index = 0; index < fillcount; index++) {
		*--dest = '\0';
	}

	/*
	**	Move in the remaining bytes.
	*/
	for (index = 0; index < frombytes; index++) {
		*--dest = *from++;
	}
}


/***********************************************************************************************
 * XMP_Significance -- Fetch the precision (bytes) of the MP number.                           *
 *                                                                                             *
 *    This routine will return with the precision of the MP number expressed as bytes. The     *
 *    MP number is presumed unsigned.                                                          *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to examine.                                   *
 *                                                                                             *
 *          precision-- The precision of the MP number to examine.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the minimum number of bytes consumed by this MP number.               *
 *                                                                                             *
 * WARNINGS:   Passing a signed MP number to this routine will return an artificially greater  *
 *             precision than it really is.                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Significance(const digit * number, int precision)
{
	assert(number != NULL);
	assert(precision > 0);

	number += precision;
	do {
		if (*(--number)) break;
	} while (--precision);
	return(precision);
}


/***********************************************************************************************
 * XMP_Inc -- Increment an MP number by one.                                                   *
 *                                                                                             *
 *    This will increment the MP number by one.                                                *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to increment.                                 *
 *                                                                                             *
 *          precision-- The precision of the MP number.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   If the number wraps around the maximum precision, the results are undefined.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Inc(digit * number, int precision)
{
	assert(number != NULL);
	assert(precision > 0);

	do {
		if (++(*number)) break;
		number++;
	} while (--precision);
}


/***********************************************************************************************
 * XMP_Dec -- Decrement the MP number by one.                                                  *
 *                                                                                             *
 *    Use this routine to decrement the specified MP number by one.                            *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to decrement.                                 *
 *                                                                                             *
 *          precision-- The precision of the MP number to decrement.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   If the number wraps below zero, the results are undefined.                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Dec(digit * number, int precision)
{
	assert(number != NULL);
	assert(precision > 0);

	do {
		*number -= 1;
		if ((*number) != ~(digit)0) break;
		number++;
	} while (--precision);
}


/***********************************************************************************************
 * XMP_Neg -- Negate the specified MP number.                                                  *
 *                                                                                             *
 *    This routine will negate (reverse sign) of the specified MP number.                      *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to negate.                                    *
 *                                                                                             *
 *          precision-- The precision of the MP number.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Neg(digit * number, int precision)
{
	assert(number != NULL);
	assert(precision > 0);

	XMP_Not(number, precision);
	XMP_Inc(number, precision);
}


/***********************************************************************************************
 * XMP_Abs -- Perform an absolute value on the specified MP number.                            *
 *                                                                                             *
 *    This will perform the absolute value function on the specified MP number. That is, if    *
 *    the MP number is negative, it will be transformed into a positive number. If the number  *
 *    is already positive, then it will be left alone.                                         *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to ABS.                                       *
 *                                                                                             *
 *          precision-- The precision of the MP number.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Abs(digit * number, int precision)
{
	assert(number != NULL);
	assert(precision > 0);

	if (XMP_Is_Negative(number, precision)) {
		XMP_Neg(number, precision);
	}
}


/***********************************************************************************************
 * XMP_Shift_Right_Bits -- Shift the MP number right by specified bit count.                   *
 *                                                                                             *
 *    Use this routine to perform a right shift of the MP number by the number of bits         *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to perform the shift upon.                    *
 *                                                                                             *
 *          bits     -- The number of bits to shift.                                           *
 *                                                                                             *
 *          precision-- The precision of the MP number.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This is an unsigned shift.                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Shift_Right_Bits(digit * number, int bits, int precision)
{
	assert(number != NULL);
	assert(bits >= 0);
	assert(precision > 0);

	if (bits == 0) return;			/* shift zero bits is a no-op */

	/*
	**	If the shift is by whole bytes, then the shift operation can
	**	be performed very quickly.
	*/
	if (bits == UNITSIZE) {
		number += precision;
		digit carry = 0;
		while (precision--) {
			number--;
			digit temp = *number;
		  	*number = carry;
			carry = temp;
		}
		return;
	}

	/*
	**	If the number of bits to shift is less than one byte, then the
	**	shift operation is a relatively simple "ripple" effect through
	**	the MP number buffer.
	*/
	if (bits < UNITSIZE) {
		number += precision;
		digit carry = 0;
		digit bitmask = (1L << bits) - 1;
		int unbits = UNITSIZE - bits;

		while (precision--) {
			number--;
			digit temp = *number & bitmask;
			*number >>= bits;
			*number |= carry << unbits;
			carry = temp;
		}
		return;
	}

	/*
	**	General purpose slow right.
	*/
	int digits_to_shift = bits / UNITSIZE;
	int bits_to_shift = bits % UNITSIZE;

	int index;
	for (index = digits_to_shift; index < (precision-1); index++) {
		*number = (*(number + digits_to_shift) >> bits_to_shift) | (*(number + (digits_to_shift + 1)) << (UNITSIZE - bits_to_shift));
		number++;
	}

	if (digits_to_shift < precision) {
		*number = (*(number + digits_to_shift) >> bits_to_shift);
		number++;
	}

	for (index= 0; index < min(digits_to_shift, precision); index++) {
		*number++ = 0;
	}
}


/***********************************************************************************************
 * XMP_Shift_Left_Bits -- Shifts the MP number left by the specified bit count.                *
 *                                                                                             *
 *    Use this routine to perform a left shift of the specified MP number.                     *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to perform the shift operation on.            *
 *                                                                                             *
 *          bits     -- The number of bits to shift the MP number leftward.                    *
 *                                                                                             *
 *          precision-- The precision of the MP number.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Shift_Left_Bits(digit * number, int bits, int precision)
{
	assert(number != NULL);
	assert(bits >= 0);
	assert(precision > 0);

	if (bits == 0) return;			/* shift zero bits is a no-op */

	/*
	**	If the shift is by whole bytes, then the shift operation can
	**	be performed very quickly.
	*/
	if (bits == UNITSIZE) {
		digit carry = 0;
		while (precision--) {
			digit temp = *number;
			*number = carry;
			carry = temp;
			number++;
		}
		return;
	}

	/*
	**	If the number of bits to shift is less than one byte, then the
	**	shift operation is a relatively simple "ripple" effect through
	**	the MP number buffer.
	*/
	if (bits < UNITSIZE) {
		digit carry = 0;
		digit bitmask = ~(((digit)-1) >> bits);
		int unbits = UNITSIZE - bits;	/* shift bits must be <= UNITSIZE */

		while (precision--) {
			digit temp = *number & bitmask;
			*number = (*number << bits) | (carry >> unbits);
			carry = temp;
			number++;
		}
		return;
	}

	/*
	**	General purpose slow left;
	*/
	int digits_to_shift = bits / UNITSIZE;
	int bits_to_shift = bits % UNITSIZE;

	int index;
	number += precision-1;
	for (index = digits_to_shift; index < (precision-1); index++) {
		*number = (*(number - digits_to_shift) << bits_to_shift) | (*(number - (digits_to_shift + 1)) >> (UNITSIZE - bits_to_shift));
		number--;
	}

	if (digits_to_shift < precision) {
		*number = (*(number - digits_to_shift) << bits_to_shift);
		number--;
	}

	for (index = 0; index < min(digits_to_shift, precision); index++) {
		*number-- = 0;
	}
}


/***********************************************************************************************
 * XMP_Rotate_Left -- Rotate specified MP number leftward.                                     *
 *                                                                                             *
 *    This routine will rotate the MP number to the left by one bit. The rotation passes bits  *
 *    through a "carry" bit position. The initial value of this "carry" bit is passed to the   *
 *    routine and the final value is returned as the result.                                   *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to perform the left rotate upon.              *
 *                                                                                             *
 *          carry    -- The initial value of the "carry" bit.                                  *
 *                                                                                             *
 *          precision-- The precision of the MP number specified.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the final value of the carry bit. This is the the bit value of the    *
 *          upper most bit of the MP number prior to the rotate operation.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool XMP_Rotate_Left(digit * number, bool carry, int precision)
{
	assert(number != NULL);
	assert(precision > 0);

	while (precision--) {
		bool temp = ((*number & UPPER_MOST_BIT) != 0);
		*number = (*number << 1);
		if (carry) *number = *number + 1;
		carry = temp;
		number++;
	}
	return carry;
}


/***********************************************************************************************
 * XMP_Not -- Perform bitwise NOT operation on MP number.                                      *
 *                                                                                             *
 *    Perform a bitwise NOT operation.                                                         *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to operate on.                                *
 *                                                                                             *
 *          precision-- The precision of the MP number.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Not(digit * number, int precision)
{
	assert(number != NULL);
	assert(precision > 0);

	for (int index = 0; index < precision; index++) {
		*number = ~(*number);
		number++;
	}
}


/***********************************************************************************************
 * XMP_Init -- Initialize an MP number to a starting value.                                    *
 *                                                                                             *
 *    This will initialize (assign) a number to an MP number. The initial value is limited     *
 *    to the precision allowed by a DIGIT type.                                                *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to initialize.                                *
 *                                                                                             *
 *          value    -- Initial integer value to assign to the MP number.                      *
 *                                                                                             *
 *          precision-- The precision of the MP number.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Init(digit * number, digit value, int precision)
{
	assert(number != NULL);
	assert(precision > 0);

	memset(number, '\0', precision * sizeof(digit));
	*number = value;
}


/***********************************************************************************************
 * XMP_Count_Bits -- Count the total number of bits (precision) in MP number.                  *
 *                                                                                             *
 *    This routine will count the maximum number of bits used by this MP number. The result    *
 *    could be referred to as the "bit precision" of the MP number.                            *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to examine.                                   *
 *                                                                                             *
 *          precision-- The (digit) precision of the MP number.                                *
 *                                                                                             *
 * OUTPUT:  Returns with the number of significant bits in the MP number.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
unsigned XMP_Count_Bits(const digit * number, int precision)
{
	assert(number != NULL);
	assert(precision > 0);

	int sub_precision = XMP_Significance(number, precision);
	if (!sub_precision) return(0);
	int total_bit_count = XMP_Digits_To_Bits(sub_precision);
	number += sub_precision-1;
	digit high_bit_mask = UPPER_MOST_BIT;

	while (!((*number) & high_bit_mask)) {
		high_bit_mask >>= 1;
		total_bit_count--;
	}

	return(total_bit_count);
}


/***********************************************************************************************
 * XMP_Count_Bytes -- Counts the number of precision bytes in MP number.                       *
 *                                                                                             *
 *    This routine will scan the MP number and determine the number of bytes needed to         *
 *    represent the MP number. Consider the result the "byte precision" of the number.         *
 *                                                                                             *
 * INPUT:   number   -- Pointer to the MP number to examine.                                   *
 *                                                                                             *
 *          precision-- Precision of the MP number.                                            *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes required to represent the precision of the number.*
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Count_Bytes(const digit * number, int precision)
{
	unsigned char * ptr = (unsigned char *)number;
	int count = 0;
	for (unsigned index = 0; index < precision*sizeof(digit); index++) {
		if (!*ptr) break;
		count++;
		ptr++;
	}
	return(count);
}


/***********************************************************************************************
 * XMP_Move -- Assign one MP number to another.                                                *
 *                                                                                             *
 *    This will move one MP number over the top of another.                                    *
 *                                                                                             *
 * INPUT:   dest     -- Destination MP number (will get clobbered).                            *
 *                                                                                             *
 *          source   -- Source MP number.                                                      *
 *                                                                                             *
 *          precision-- Precision of both MP numbers.                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Both MP numbers must have the same precision.                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Move(digit * dest, digit const * source, int precision)
{
	memcpy(dest, source, precision * sizeof(digit));
}


/***********************************************************************************************
 * XMP_Compare -- Compare one MP number with another.                                          *
 *                                                                                             *
 *    This routine will compare two MP numbers. It will return a value indicating which MP     *
 *    number is greater or if they are equal.                                                  *
 *                                                                                             *
 * INPUT:   left_number -- The left hand MP number.                                            *
 *                                                                                             *
 *          right_number-- The right hand MP number.                                           *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers.                                    *
 *                                                                                             *
 * OUTPUT:  Returns -1 if the left_number is less than the right_number.                       *
 *          Returns 1 if the left_number is greater than the right number.                     *
 *          Returns 0 if both numbers are identical.                                           *
 *                                                                                             *
 * WARNINGS:   Both numbers must have the same precision.                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Compare(const digit * left_number, const digit * right_number, int precision)
{
	left_number += precision-1;
	right_number += precision-1;
	do {
		if (*left_number < *right_number) return -1;
		if (*left_number > *right_number) return 1;
		left_number--;
		right_number--;
	} while (--precision);
	return 0;
}


/***********************************************************************************************
 * XMP_Add -- Add two MP numbers with a carry option.                                          *
 *                                                                                             *
 *    Use this routine to add one MP number to another. There is an optional "carry" value     *
 *    that (when true) will add an additional 1 to the result.                                 *
 *                                                                                             *
 * INPUT:   result   -- Pointer to the MP buffer that will hold the result. This can be the    *
 *                      same value as the left_number or right_number pointers.                *
 *                                                                                             *
 *          left_number -- The left hand MP number.                                            *
 *                                                                                             *
 *          right_number-- The right hand MP number.                                           *
 *                                                                                             *
 *          carry       -- Optional carry flag (typically this will be false).                 *
 *                                                                                             *
 *          precision   -- The precision of the numbers involved.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the carry flag after the addition. If the value is true then an       *
 *          overflow occurred.                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool XMP_Add(digit * result, const digit * left_number, const digit * right_number, bool carry, int precision)
{
	while (precision--) {
		digit term = *left_number + *right_number;
		digit final = term + carry;
		carry = (term < *left_number || (carry && final == 0));

		right_number++;
		left_number++;
		*result++ = final;
	}
	return(carry);
}


/***********************************************************************************************
 * XMP_Add_Int -- Add an integer to an MP number (with carry).                                 *
 *                                                                                             *
 *    This routine will add an integer number to an MP number. There is an optional carry      *
 *    parameter. If the carry flag is true, and additional 1 will be added to the result.      *
 *    This routine is much faster than adding two MP numbers together.                         *
 *                                                                                             *
 * INPUT:   result      -- Pointer to the result MP number. This pointer can be the same as    *
 *                         the left_number parameter.                                          *
 *                                                                                             *
 *          left_number -- Pointer to the left hand MP number.                                 *
 *                                                                                             *
 *          right_number-- The integer number to add to the left hand number.                  *
 *                                                                                             *
 *          carry       -- Input carry flag. If this is true, then an additional one will be   *
 *                         added to the result.                                                *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the result carry flag. A true value means the addition overflowed.    *
 *                                                                                             *
 * WARNINGS:   All MP numbers must share the same precision. Negative numbers are not          *
 *             supported.                                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool XMP_Add_Int(digit * result, const digit * left_number, digit right_number, bool carry, int precision)
{
	while (precision--) {
		digit term = *left_number + right_number;
		digit final = term + carry;
		carry = (term < *left_number || (carry && final == 0));

		right_number = 0;
		left_number++;
		*result++ = final;
	}
	return(carry);
}


/***********************************************************************************************
 * XMP_Sub -- Subtract one MP number from another (with borrow).                               *
 *                                                                                             *
 *    This routine will subtract one MP number from another. There is an optional borrow       *
 *    flag that can be specified.                                                              *
 *                                                                                             *
 * INPUT:   result      -- Pointer to the MP number that will hold the result. This pointer    *
 *                         can be the same as the left_number or right_number parameters.      *
 *                                                                                             *
 *          left_number -- The left hand number (value will be subtracted from this).          *
 *                                                                                             *
 *          right_number-- The right hand number (the value to subtract from the left number)  *
 *                                                                                             *
 *          borrow      -- The optional borrow flag. If this flag is true, the an extra one    *
 *                         will be subtracted from the result.                                 *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers involved.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the borrow result flag. If the value is true, then an underflow       *
 *          occurred during subtraction.                                                       *
 *                                                                                             *
 * WARNINGS:   All MP numbers must share the same precision.                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool XMP_Sub(digit * result, const digit * left_number, const digit * right_number, bool borrow, int precision)
{
	const unsigned short * left_number_ptr = (const unsigned short *)left_number;
	const unsigned short * right_number_ptr = (const unsigned short *)right_number;
	unsigned short * result_ptr = (unsigned short *)result;

	precision *= 2;
	while (precision--) {
		digit x = (digit) *left_number_ptr - (digit) *right_number_ptr - (digit) borrow;
		right_number_ptr++;
		left_number_ptr++;
		*result_ptr++ = (unsigned short)x;
		borrow = (((1L << 16) & x) != 0L);
	}
	return (borrow);
}


/***********************************************************************************************
 * XMP_Sub_Int -- Subtract an integer from an MP number (with borrow).                         *
 *                                                                                             *
 *    This will subtract an integer from the specified MP number. There is an optional borrow  *
 *    flag available.                                                                          *
 *                                                                                             *
 * INPUT:   result      -- Pointer to the MP buffer that will hold the result.                 *
 *                                                                                             *
 *          left_number -- Pointer to the MP number that will be subtracted FROM.              *
 *                                                                                             *
 *          right_number-- The integer to subtract from the left hand number.                  *
 *                                                                                             *
 *          borrow      -- The optional borrow flag. If this value is true, then an extra one  *
 *                         will be subtracted from the result.                                 *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers involved.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the borrow flag of the result. If this value is true, then an         *
 *          underflow occurred during subtraction.                                             *
 *                                                                                             *
 * WARNINGS:   The precision must be identical between the MP numbers involved.                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool XMP_Sub_Int(digit * result, const digit * left_number, unsigned short right_number, bool borrow, int precision)
{
	const unsigned short * left_number_ptr = (const unsigned short *)left_number;
	unsigned short * result_ptr = (unsigned short *)result;

	precision *= 2;
	while (precision--) {
		digit x = (digit) *left_number_ptr - right_number - borrow;
		left_number_ptr++;
		*result_ptr++ = (unsigned short)x;
		borrow = (((1L << 16) & x) != 0L);

		right_number = 0;
	}
	return (borrow);
}


/***********************************************************************************************
 * XMP_Unsigned_Mult -- Multiply two unsigned MP numbers together.                             *
 *                                                                                             *
 *    This routine will multiply two MP numbers together. The result will have the sum of the  *
 *    significance of the two.                                                                 *
 *                                                                                             *
 * INPUT:   prod        -- Pointer to the product MP buffer that will hold the result.         *
 *                                                                                             *
 *          multiplicand-- Pointer to the multiplicand MP number.                              *
 *                                                                                             *
 *          multiplier  -- Pointer to the multiplier MP number.                                *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure the product will fit within the precision of the result.                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Unsigned_Mult(digit * prod, const digit * multiplicand, const digit * multiplier, int precision)
{
	XMP_Init(prod, 0, precision);

	/*
	**	Multiplying by zero is always a zero product.
	*/
	if (XMP_Test_Eq_Int(multiplicand, 0, precision) || XMP_Test_Eq_Int(multiplier, 0, precision)) {
		return 0;
	}

	int total_bit_count = XMP_Count_Bits(multiplier, precision);
	digit high_bit_mask = XMP_Bits_To_Mask(total_bit_count);
	int sub_precision = XMP_Bits_To_Digits(total_bit_count);
	if (!sub_precision) return(0);
	multiplier += sub_precision;

	while (total_bit_count--) {
		XMP_Shift_Left_Bits(prod, 1, precision);

		if ((*(multiplier-1)) & high_bit_mask) {
			XMP_Add(prod, prod, multiplicand, 0, precision);
		}

		high_bit_mask >>= 1;
		if (!high_bit_mask) {
			high_bit_mask = UPPER_MOST_BIT;
			multiplier--;
		}

	}
	return 0;
}


/***********************************************************************************************
 * XMP_Unsigned_Mult_Int -- Multiply an MP number by a simple integer.                         *
 *                                                                                             *
 *    This is a very fast multiply since the multiplier is just an integer integral.           *
 *                                                                                             *
 * INPUT:   prod        -- Pointer to the product MP number.                                   *
 *                                                                                             *
 *          multiplicand-- Pointer to the MP number that is the multiplicand.                  *
 *                                                                                             *
 *          multiplier  -- The integral integer that is the multiplier.                        *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The multiplier must fit in a signed integer (although it isn't a signed value). *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Unsigned_Mult_Int(digit * prod, const digit * multiplicand, short multiplier, int precision)
{
	const unsigned short * m2 = (const unsigned short *)multiplicand;
	unsigned short * pr = (unsigned short *)prod;
	unsigned long carry = 0;
	for (int i = 0; i < precision*2; ++i) {
		unsigned long p = (((unsigned long)multiplier) * *m2) + carry;;
		*pr = (unsigned short) p;
		carry = p >> 16;
		m2++;
		pr++;
	}

	/* Add carry to the next higher word of product / dividend */
//	*pr += (unsigned short)carry;
	return(0);
}


/***********************************************************************************************
 * XMP_Signed_Mult_Int -- Multiply an MP number by a signed simple integer.                    *
 *                                                                                             *
 *    This will multiply the specified integer with the MP number. It is a much faster         *
 *    multiply than when multiplying two MP numbers.                                           *
 *                                                                                             *
 * INPUT:   prod        -- Pointer to the product MP number.                                   *
 *                                                                                             *
 *          multiplicand-- Pointer to the MP number that serves as the multiplicand.           *
 *                                                                                             *
 *          multiplier  -- The simple integral integer used as the multiplier.                 *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers involved.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The multiplier must fist within a signed short integer.                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Signed_Mult_Int(digit * prod, const digit * multiplicand, signed short multiplier, int precision)
{
	if (XMP_Is_Negative(multiplicand, precision)) {
		digit abs_multiplicand[MAX_UNIT_PRECISION];
		XMP_Move(abs_multiplicand, multiplicand, precision);
		XMP_Neg(abs_multiplicand, precision);

		if (multiplier < 0) {
			multiplier = (signed short)-multiplier;

			XMP_Unsigned_Mult_Int(prod, abs_multiplicand, multiplier, precision);
		} else {
			XMP_Unsigned_Mult_Int(prod, abs_multiplicand, multiplier, precision);
			XMP_Neg(prod, precision);
		}
	} else {
		if (multiplier < 0) {
			multiplier = (signed short)-multiplier;

			XMP_Unsigned_Mult_Int(prod, multiplicand, multiplier, precision);
			XMP_Neg(prod, precision);
		} else {
			XMP_Unsigned_Mult_Int(prod, multiplicand, multiplier, precision);
		}
	}
	return(0);
}


/***********************************************************************************************
 * XMP_Signed_Mult -- A signed multiply between two MP numbers.                                *
 *                                                                                             *
 *    This routine will perform a multiply between two signed MP numbers.                      *
 *                                                                                             *
 * INPUT:   prod        -- Pointer to the product MP number buffer.                            *
 *                                                                                             *
 *          multiplicand-- Pointer to the multiplicand MP number.                              *
 *                                                                                             *
 *          multiplier  -- Pointer to the multiplier MP number.                                *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers involved.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This is not a very fast routine.                                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Signed_Mult(digit * prod, const digit * multiplicand, const digit * multiplier, int precision)
{
	if (XMP_Is_Negative(multiplicand, precision)) {
		digit abs_multiplicand[MAX_UNIT_PRECISION];
		XMP_Move(abs_multiplicand, multiplicand, precision);
		XMP_Neg(abs_multiplicand, precision);

		if (XMP_Is_Negative(multiplier, precision)) {
			digit abs_multiplier[MAX_UNIT_PRECISION];
			XMP_Move(abs_multiplier, multiplier, precision);
			XMP_Neg(abs_multiplier, precision);

			XMP_Unsigned_Mult(prod, abs_multiplicand, abs_multiplier, precision);
		} else {
			XMP_Unsigned_Mult(prod, abs_multiplicand, multiplier, precision);
			XMP_Neg(prod, precision);
		}
	} else {
		if (XMP_Is_Negative(multiplier, precision)) {
			digit abs_multiplier[MAX_UNIT_PRECISION];
			XMP_Move(abs_multiplier, multiplier, precision);
			XMP_Neg(abs_multiplier, precision);

			XMP_Unsigned_Mult(prod, multiplicand, abs_multiplier, precision);
			XMP_Neg(prod, precision);
		} else {
			XMP_Unsigned_Mult(prod, multiplicand, multiplier, precision);
		}
	}
	return(0);
}


/***********************************************************************************************
 * XMP_Unsigned_Div_Int -- Perform a short integer divide into an MP number.                   *
 *                                                                                             *
 *    This routine performs a fast divide of the specified MP dividend by a simple             *
 *    short integer. The division is an UNSIGNED division however.                             *
 *                                                                                             *
 * INPUT:   quotient    -- Pointer to the MP number buffer where the quotient will go.         *
 *                                                                                             *
 *          dividend    -- Pointer to the MP number that serves as the dividend.               *
 *                                                                                             *
 *          divisor     -- The simple signed short integer that serves as the divisor.         *
 *                                                                                             *
 *          precision   -- The precision that is used by the MP numbers involved.              *
 *                                                                                             *
 * OUTPUT:  Returns with the remainder of the division.                                        *
 *                                                                                             *
 * WARNINGS:   This is an UNSIGNED divide even.                                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
unsigned short XMP_Unsigned_Div_Int(digit * quotient, digit const * dividend, unsigned short divisor, int precision)
{
	if (!divisor) return 0;		/* zero divisor means divide error */

	unsigned short remainder = 0;

	XMP_Init(quotient, 0, precision);

	int total_bit_count = XMP_Count_Bits(dividend, precision);
	int digit_precision = XMP_Bits_To_Digits(total_bit_count);
	digit const * dividend_ptr = dividend + (digit_precision-1);

	if (!digit_precision) return(0);

	digit high_bit_mask = XMP_Bits_To_Mask(total_bit_count);
	digit * quotient_ptr = quotient + (digit_precision-1);

	while (total_bit_count--) {
		remainder <<= 1;

		if ((*dividend_ptr) & high_bit_mask) remainder++;

		if (remainder >= divisor) {
			remainder -= divisor;
			*quotient_ptr |= high_bit_mask;
		}

		high_bit_mask >>= 1;
		if (!high_bit_mask) {
			high_bit_mask = UPPER_MOST_BIT;
			--dividend_ptr;
			--quotient_ptr;
		}
	}

	return(remainder);
}


/***********************************************************************************************
 * XMP_Unsigned_Div -- Unsigned divide of one MP number into another.                          *
 *                                                                                             *
 *    This will perform the (dog slow) divide of one MP number into another. Because of the    *
 *    slowness of this routine, both the quotient and the remainder are available as a         *
 *    result of the operation.                                                                 *
 *                                                                                             *
 * INPUT:   remainder   -- Pointer to the MP buffer that will hold the remainder of the divide.*
 *                                                                                             *
 *          quotient    -- Pointer to the MP buffer that will hold the quotient of the divide. *
 *                                                                                             *
 *          dividend    -- The MP dividend (numerator) number.                                 *
 *                                                                                             *
 *          divisor     -- The MP divisor (denominator) number.                                *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers involved.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This is very slow.                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Unsigned_Div(digit * remainder, digit * quotient, digit const * dividend, digit const * divisor, int precision)
{
	// check for divide by zero.
	if (XMP_Test_Eq_Int(divisor, 0, precision)) return(-1);

	XMP_Init(remainder, 0, precision);
	XMP_Init(quotient, 0, precision);

	int total_bit_count = XMP_Count_Bits(dividend, precision);
	int digit_precision = XMP_Bits_To_Digits(total_bit_count);
	if (!digit_precision) return(0);

	digit const * dividend_ptr = dividend + (digit_precision-1);
	digit * quotient_ptr = quotient + (digit_precision-1);

	digit high_bit_mask = XMP_Bits_To_Mask(total_bit_count);
	while (total_bit_count--) {
		XMP_Shift_Left_Bits(remainder, 1, precision);

		if (((*dividend_ptr) & high_bit_mask) != 0) {
			XMP_Inc(remainder, precision);
		}

		if (XMP_Compare(remainder, divisor, precision) >= 0) {
			XMP_Sub(remainder, remainder, divisor, 0, precision);
			*quotient_ptr |= high_bit_mask;
		}

		high_bit_mask >>= 1;
		if (!high_bit_mask) {
			high_bit_mask = UPPER_MOST_BIT;
			dividend_ptr--;
			quotient_ptr--;
		}

	}
	return 0;
}


/***********************************************************************************************
 * XMP_Signed_Div -- Signed divide of one MP number into another.                              *
 *                                                                                             *
 *    This will perform a signed divide (very very slow) of one MP number into another.        *
 *    Because of the slow nature of this routine, both the quotient and the remainder are      *
 *    available as results.                                                                    *
 *                                                                                             *
 * INPUT:   remainder   -- Pointer to the buffer that will hold the remainder of the divide.   *
 *                                                                                             *
 *          quotient    -- Pointer to the buffer that will hold the quotient of the divide.    *
 *                                                                                             *
 *          dividend    -- The dividend (numerator) MP number.                                 *
 *                                                                                             *
 *          divisor     -- The divisor (denominator) MP number.                                *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers involved.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This is very very slow.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Signed_Div(digit * remainder, digit * quotient, digit const * dividend, digit const * divisor, int precision)
{
	bool negative = false;

	digit scratch_dividend[MAX_UNIT_PRECISION];
	XMP_Move(scratch_dividend, dividend, precision);

	digit scratch_divisor[MAX_UNIT_PRECISION];
	XMP_Move(scratch_divisor, divisor, precision);

	if (XMP_Is_Negative(scratch_dividend, precision)) {
		XMP_Neg(scratch_dividend, precision);
		negative = !negative;
	}

	if (XMP_Is_Negative(scratch_divisor, precision)) {
		XMP_Neg(scratch_divisor, precision);
		negative = !negative;
	}

	XMP_Unsigned_Div(remainder, quotient, scratch_dividend, scratch_divisor, precision);

	if (negative) {
		XMP_Neg(quotient, precision);
		if (!XMP_Test_Eq_Int(remainder, 0, precision)) {
			XMP_Dec(quotient, precision);
			XMP_Neg(remainder, precision);
			XMP_Add(remainder, remainder, scratch_divisor, 0, precision);
		}
	}
}


/***********************************************************************************************
 * XMP_Inverse_A_Mod_B -- Inverts and performs modulus on an MP number.                        *
 *                                                                                             *
 *    This is a utility routine that will perform an inverse on the MP number and then         *
 *    perform a modulus of that number by another MP number. There are some algorithms that    *
 *    require this process.                                                                    *
 *                                                                                             *
 * INPUT:   result      -- Pointer to the MP buffer that will hold the result.                 *
 *                                                                                             *
 *          number      -- The MP number that will be inverted then modulo-ized.               *
 *                                                                                             *
 *          modulus     -- The MP number to modulus the first number by.                       *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers involved.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Inverse_A_Mod_B(digit * result, digit const * number, digit const * modulus, int precision)
{
	digit g[3][MAX_UNIT_PRECISION];
	XMP_Move(g[0], modulus, precision);
	XMP_Move(g[1], number, precision);

	digit v[3][MAX_UNIT_PRECISION];
	XMP_Init(v[0], 0, precision);
	XMP_Init(v[1], 1, precision);

	digit y[MAX_UNIT_PRECISION];

	int i;
	for (i = 1; !XMP_Test_Eq_Int(g[i%3], 0, precision); i++) {
		XMP_Unsigned_Div(g[(i+1)%3], y, g[(i-1)%3], g[i%3], precision);

		XMP_Unsigned_Mult(result, v[i%3], y, precision);
		XMP_Sub(v[(i+1)%3], v[(i-1)%3], result, 0, precision);
	}

	if (XMP_Is_Negative(v[(i-1)%3], precision)) {
		XMP_Add(v[(i-1)%3], v[(i-1)%3], modulus, 0, precision);
	}

	XMP_Move(result, v[(i-1)%3], precision);
}


/***********************************************************************************************
 * XMP_Reciprocal -- Compute the reciprocal (inverse) of the MP number.                        *
 *                                                                                             *
 *    Use this routine to determine the inverse of the specified MP number. The inverse is     *
 *    defined as 1/number.                                                                     *
 *                                                                                             *
 * INPUT:   result   -- Pointer to the result MP number buffer.                                *
 *                                                                                             *
 *          number   -- The number to be inverted.                                             *
 *                                                                                             *
 *          precision-- The precision of the MP number.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Reciprocal(digit * quotient, const digit * divisor, int precision)
{
	digit remainder[MAX_UNIT_PRECISION];

	if (XMP_Test_Eq_Int(divisor, 0, precision)) return -1;		/* zero divisor means divide error */

	XMP_Init(remainder, 0, precision);
	XMP_Init(quotient, 0, precision);

	/* normalize and compute number of bits in quotient first */
	unsigned total_bit_count = XMP_Count_Bits(divisor, precision);
	digit high_bit_mask = XMP_Bits_To_Mask(total_bit_count + 1);	/* bitmask within a single digit */
	int sub_precision = XMP_Bits_To_Digits(total_bit_count + 1);

	XMP_Set_Bit(remainder, total_bit_count - 1);

	/* rescale quotient to precision of divisor bits */
	quotient += sub_precision-1;

	while (total_bit_count--) {
		XMP_Shift_Left_Bits(remainder, 1, precision);
		if (XMP_Compare(remainder, divisor, precision) >= 0) {
			XMP_Sub(remainder, remainder, divisor, 0, precision);
			*quotient |= high_bit_mask;
		}

		high_bit_mask >>= 1;
		if (!high_bit_mask) {
			high_bit_mask = UPPER_MOST_BIT;
			quotient--;
		}
	}

	XMP_Init(remainder, 0, precision);
	return 0;
}


/***********************************************************************************************
 * XMP_Decode_ASCII -- Convert ASCII into an MP number.                                        *
 *                                                                                             *
 *    This routine will convert a supplied ASCII string into an MP number.                     *
 *                                                                                             *
 * INPUT:   str      -- Pointer to the ASCII string that will be converted.                    *
 *                                                                                             *
 *          mpn      -- Pointer to the MP number buffer that will be initialized.              *
 *                                                                                             *
 *          precision -- The precision of the MP number.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Decode_ASCII(char const * str, digit * mpn, int precision)
{
	/*
	**	Initialize the multiprecision number to zero. From this point
	**	onward, this object can be manipulated as a regular number.
	**	This is, in fact, what is done as the ascii string is parsed
	**	into a working number.
	*/
	XMP_Init(mpn, 0, precision);

	/*
	**	No string or zero length is considered '0'.
	*/
	if (!str) return;
	int i = strlen(str);
	if (i == 0) return;

	unsigned short radix;		/* base 2-16 */
	switch (toupper(str[i-1])) {		/* classify radix select suffix character */
		case '.':
			radix = 10;
			break;

		case 'H':
			radix = 16;
			break;

		case 'O':
			radix = 8;
			break;

		case 'B':			/* caution! 'b' is a hex digit! */
			radix = 2;
			break;

		default:
			radix = 10;
			break;
	}

	bool minus = (*str == '-');
	if (minus) str++;

	digit c;
	while ((c = (unsigned char)*str++) != 0) {
		if (c == ',') continue;		/* allow commas in number */

		/*
		**	If not a hexadecimal (highest base) digit then it is
		**	clearly the end of the processable string. Bail out
		**	of the scan loop.
		*/
		if (!isxdigit((char)c)) break;

		/*
		**	Convert the character into an integer number 0 through 15.
		*/
		if (isdigit((char)c)) {
			c -= '0';
		} else {
			c = (unsigned char)(toupper((char)c) - 'A') + 10;
		}

		/*
		**	If the integer digit is greater than the radix, then we
		**	know that further processing should stop. This is the
		**	end of the number string.
		*/
		if (c >= radix) break;		/* scan terminated by any non-digit */


		XMP_Unsigned_Mult_Int(mpn, mpn, radix, precision);
		XMP_Add_Int(mpn, mpn, c, 0, precision);
	}
	if (minus) {
		XMP_Neg(mpn, precision);
	}
}


/***********************************************************************************************
 * XMP_Hybrid_Mul -- Special hybrid short multiply (with carry).                               *
 *                                                                                             *
 *    Multiply the single-word multiplier times the multiprecision integer                     *
 *    in multiplicand, accumulating result in prod.  The resulting                             *
 *    multiprecision prod will be 1 word longer than the multiplicand.                         *
 *    multiplicand is double precision words long.  We add into prod, so caller                *
 *    should zero it out first.  For best results, this time-critical                          *
 *    function should be implemented in assembly.                                              *
 *    NOTE:  Unlike other functions in the multiprecision arithmetic                           *
 *    library, both multiplicand and prod are pointing at the LSB,                             *
 *    regardless of byte order of the machine.  On an 80x86, this makes                        *
 *    no difference.  But if this assembly function is implemented                             *
 *    on a 680x0, it becomes important.                                                        *
 *                                                                                             *
 *    Note that this has been modified from the previous version to allow                      *
 *    better support for Smith's modmult:                                                      *
 *         The final carry bit is added to the existing product                                *
 *         array, rather than simply stored.                                                   *
 *                                                                                             *
 * INPUT:   prod     -- Pointer to the product MP number buffer.                               *
 *                                                                                             *
 *          multiplicand   -- Pointer to the multiplicand MP number.                           *
 *                                                                                             *
 *          multiplier  -- The short integer used as the multiplier.                           *
 *                                                                                             *
 *          precision   -- The precision of the MP number used.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The carry (if any) is added into the integer one beyond the end of the          *
 *             product buffer.                                                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Hybrid_Mul(unsigned short * prod, unsigned short * multiplicand, unsigned short multiplier, int precision)
{
	unsigned long carry = 0;
	for (int i = 0; i < precision; ++i) {
		unsigned long p = (unsigned long)multiplier * *multiplicand++;
		p += *prod + carry;
		*prod++ = (unsigned short) p;
		carry = p >> 16;
	}

	/* Add carry to the next higher word of product / dividend */
	*prod += (unsigned short) carry;
}


/***********************************************************************************************
 * XMP_Double_Mul -- Double precision MP multiply.                                             *
 *                                                                                             *
 *    This will perform a double precision multiply of MP numbers. This means that the product *
 *    will be twice the precision of the components.                                           *
 *                                                                                             *
 * INPUT:   prod        -- Pointer to the result buffer. This buffer must be able to hold      *
 *                         double the precision specified.                                     *
 *                                                                                             *
 *          multiplicand-- Pointer to the multiplicand MP number.                              *
 *                                                                                             *
 *          multiplier  -- Pointer to the multiplier number.                                   *
 *                                                                                             *
 *          precision   -- The precision of the two component MP numbers.                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure the product buffer can hold a double precision number.                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Double_Mul(digit * prod, const digit * multiplicand, const digit * multiplier, int precision)
{
	/*
	**	Clear out the double precision product buffer.
	*/
	XMP_Init(prod, 0, precision*2);

	const unsigned short * multiplier_ptr = (const unsigned short *) multiplier;
	unsigned short * product_ptr = (unsigned short *) prod;

	// Multiply multiplicand by each word in multiplier, accumulating prod.
	for (int i = 0; i < precision*2; ++i) {
		XMP_Hybrid_Mul(product_ptr++, (unsigned short *)multiplicand, *multiplier_ptr++, precision*2);
	}
}



static int _modulus_shift;									// number of bits for recip scaling
static unsigned short _reciprical_high_digit;		// MSdigit of scaled recip
static unsigned short _reciprical_low_digit;			// LSdigit of scaled recip

static int _modulus_sub_precision;						//	length of modulus in MULTUNITs
static int _modulus_bit_count;							//	number of modulus significant bits
static digit _scratch_modulus[MAX_UNIT_PRECISION];	// modulus

// The double precision modulus staging buffer.
static digit _double_staging_number[MAX_UNIT_PRECISION * 2 + 2];

// most significant digits of modulus.
static digit _mod_quotient[4];
static digit _mod_divisor[4];


/***********************************************************************************************
 * XMP_Prepare_Modulus -- Prepare globals for modulus operation.                               *
 *                                                                                             *
 *    Calculate the reciprocal of modulus with a precision of two MULTUNITs.                   *
 *    Assumes that precision has already been adjusted to the                                  *
 *    size of the modulus, plus SLOP_BITS.                                                     *
 *                                                                                             *
 *    Note: This routine was designed to work with large values and                            *
 *    doesn't have the necessary testing or handling to work with a                            *
 *    modulus having less than three significant digits.  For such cases,                      *
 *    the separate multiply and modulus routines can be used.                                  *
 *                                                                                             *
 * INPUT:   modulus  -- Pointer to the modulus number.                                         *
 *                                                                                             *
 *          precision-- The precision of the modulus number.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Prepare_Modulus(const digit * n_modulus, int precision)
{
	XMP_Move(_scratch_modulus, n_modulus, precision);

	_modulus_bit_count = XMP_Count_Bits(_scratch_modulus, precision);
	_modulus_sub_precision = (_modulus_bit_count + 16 - 1) / 16;

	/*
	**	Keep 2*16 bits in _mod_divisor.
	** This will (normally) result in a reciprocal of 2*16+1 bits.
	*/
	int sub_precision = XMP_Significance(_scratch_modulus, precision);	// significant digits in modulus
	XMP_Move(_mod_divisor, &_scratch_modulus[sub_precision-2], 2);
	_modulus_shift = XMP_Count_Bits(_mod_divisor, 2) - 2 * 16;
	XMP_Shift_Right_Bits(_mod_divisor, _modulus_shift, 2);

	XMP_Reciprocal(_mod_quotient, _mod_divisor, 2);
	XMP_Shift_Right_Bits(_mod_quotient, 1, 2);

	/* Reduce to:   0 < _modulus_shift <= 16 */
	_modulus_shift = ((_modulus_shift + (16 - 1)) % 16) + 1;

	/* round up */
	XMP_Inc(_mod_quotient, 2);
	if (XMP_Count_Bits(_mod_quotient, 2) > 2 * 16) {
		XMP_Shift_Right_Bits(_mod_quotient, 1, 2);
		_modulus_shift--;		/* now  0 <= _modulus_shift <= 16 */
	}
	unsigned short * mpm = (unsigned short *) _mod_quotient;
	_reciprical_low_digit = *mpm++;
	_reciprical_high_digit = *mpm;

	return 0;
}


/***********************************************************************************************
 * XMP_Mod_Mult -- Perform a multiply - modulus operation.                                     *
 *                                                                                             *
 *    This routine will combine a multiply and a modulus operation. This takes advantage of    *
 *    a tremendous speed advantage possible if these two processes are combined rather than    *
 *    being performed separately.                                                              *
 *                                                                                             *
 * INPUT:   prod        -- Pointer to the MP buffer that will hold the result.                 *
 *                                                                                             *
 *          multiplicand-- The number to multiply.                                             *
 *                                                                                             *
 *          multiplier  -- The number to multiply by.                                          *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers involved.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The modulus must already have been prepared by the routine XMP_Prepare_Modulus. *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int XMP_Mod_Mult(digit * prod, const digit * multiplicand, const digit * multiplier, int precision)
{
	XMP_Double_Mul(_double_staging_number, multiplicand, multiplier, precision);

	int double_precision = precision * 2 + 1;

	_double_staging_number[double_precision - 1] = 0;	/* leading 0 digit */

	/*
	**	We now start working with MULTUNITs.
	**	Determine the most significant MULTUNIT of the product so we don't
	**	have to process leading zeros in our divide loop.
	*/
	int dmi = XMP_Significance(_double_staging_number, double_precision) * 2;	//	number of significant MULTUNITs in product

	if (dmi >= _modulus_sub_precision) {

		/* Make dividend negative.  This allows the use of mp_single_mul to
		** "subtract" the product of the modulus and the trial divisor
		** by actually adding to a negative dividend.
		** The one's complement of the dividend is used, since it causes
		** a zero value to be represented as all ones.  This facilitates
		** testing the result for possible overflow, since a sign bit
		** indicates that no adjustment is necessary, and we should not
		** attempt to adjust if the result of the addition is zero.
		*/
		XMP_Inc(_double_staging_number, double_precision);
		XMP_Neg(_double_staging_number, double_precision);

		int nqd = dmi + 1 - _modulus_sub_precision; 	// number of quotient digits remaining to be generated

		/* Set msb, lsb, and normal ptrs of dividend */
		unsigned short * dmph = ((unsigned short *)_double_staging_number) + dmi + 1;	// points to one higher than precision would indicate
		unsigned short * dmpl = dmph - _modulus_sub_precision;

		/*
		** Divide loop.
		** Each iteration computes the next quotient MULTUNIT digit, then
		** multiplies the divisor (modulus) by the quotient digit and adds
		** it to the one's complement of the dividend (equivalent to
		** subtracting).  If the product was greater than the remaining dividend,
		** we get a non-negative result, in which case we subtract off the
		** modulus to get the proper negative remainder.
		*/
		for (; nqd; nqd--) {
			--dmph;
			--dmpl;

			unsigned short q = mp_quo_digit(dmph);	// trial quotient digit
			if (q > 0) {
				XMP_Hybrid_Mul(dmpl, (unsigned short *)_scratch_modulus, q, precision*2);

				/* Perform correction if q too large.
				**  This rarely occurs.
				*/
				if (!(*dmph & SEMI_UPPER_MOST_BIT)) {
					unsigned short * dmp = dmpl;
					if (XMP_Sub((unsigned long *)dmp, (unsigned long *)dmp, _scratch_modulus, false, precision)) {
						(*dmph)--;
					}
				}
			}
		}

		/* d contains the one's complement of the remainder. */
		XMP_Neg(_double_staging_number, precision);
		XMP_Dec(_double_staging_number, precision);
	}

	XMP_Move(prod, _double_staging_number, precision);
	return (0);
}


/***********************************************************************************************
 * XMP_Mod_Mult_Clear -- Remove temporary values from memory.                                  *
 *                                                                                             *
 *    Smith's mp_modmult function leaves some internal arrays in memory,                       *
 *    so we have to call modmult_burn() at the end of mp_exponent_mod.                         *
 *    This is so that no cryptographically sensitive data is left in memory                    *
 *    after the program exits.                                                                 *
 *                                                                                             *
 * INPUT:   precision   -- The precision of the numbers involved.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Mod_Mult_Clear(int precision)
{
	XMP_Init(_scratch_modulus, 0, precision);
	XMP_Init(_double_staging_number, 0, precision);
	XMP_Init(_mod_quotient, 0, ARRAY_SIZE(_mod_quotient));
	XMP_Init(_mod_divisor, 0, ARRAY_SIZE(_mod_divisor));
	_modulus_shift = _modulus_bit_count = 0;
	_reciprical_high_digit = _reciprical_low_digit = 0;
	_modulus_sub_precision = /*mutemp =*/ 0;
}


/*
** The function mp_quo_digit is the heart of Smith's modulo reduction,
** which uses a form of long division.  It computes a trial quotient
** "digit" (MULTUNIT-sized digit) by multiplying the three most
** significant MULTUNITs of the dividend by the two most significant
** MULTUNITs of the reciprocal of the modulus.  Note that this function
** requires that 16 * 2 <= sizeof(unsigned long).
**
** An important part of this technique is that the quotient never be
** too small, although it may occasionally be too large.  This was
** done to eliminate the need to check and correct for a remainder
** exceeding the divisor.       It is easier to check for a negative
** remainder.  The following technique rarely needs correction for
** MULTUNITs of at least 16 bits.
**
** The following routine has two implementations:
**
** Parameter: dividend - points to the most significant MULTUNIT
**      of the dividend.  Note that dividend actually contains the
**      one's complement of the actual dividend value (see comments for
**      XMP_Mod_Mult).
**
**  Return: the trial quotient digit resulting from dividing the first
**      three MULTUNITs at dividend by the upper two MULTUNITs of the
**      modulus.
*/
unsigned short mp_quo_digit(unsigned short * dividend)
{
	unsigned long q, q0, q1, q2;

	/*
	* Compute the least significant product group.
	* The last terms of q1 and q2 perform upward rounding, which is
	* needed to guarantee that the result not be too small.
	*/
	q1 = (dividend[-2] ^ SEMI_MASK) * (unsigned long) _reciprical_high_digit + _reciprical_high_digit;
	q2 = (dividend[-1] ^ SEMI_MASK) * (unsigned long) _reciprical_low_digit + (1L << 16);
	q0 = (q1 >> 1) + (q2 >> 1) + 1;

	/*      Compute the middle significant product group.   */
	q1 = (dividend[-1] ^ SEMI_MASK) * (unsigned long) _reciprical_high_digit;
	q2 = (dividend[0] ^ SEMI_MASK) * (unsigned long) _reciprical_low_digit;
	q = (q0 >> 16) + (q1 >> 1) + (q2 >> 1) + 1;

	/*      Compute the most significant term and add in the others */
	q = (q >> (16 - 2)) + (((dividend[0] ^ SEMI_MASK) * (unsigned long) _reciprical_high_digit) << 1);
	q >>= _modulus_shift;

	/*      Prevent overflow and then wipe out the intermediate results. */
	return (unsigned short) min(q, (unsigned long)(1L << 16) - 1);
}


/*
** Russian peasant combined exponentiation/modulo algorithm.
** Calls modmult instead of mult.
** Computes:  expout = (expin**exponent) mod modulus
** WARNING: All the arguments must be less than the modulus!
*/
int xmp_exponent_mod(digit * expout, const digit * expin, const digit * exponent_ptr, const digit * modulus, int precision)
{
	digit product[MAX_UNIT_PRECISION];

	XMP_Init(expout, 1, precision);
	if (XMP_Test_Eq_Int(exponent_ptr, 0, precision)) {
		if (XMP_Test_Eq_Int(expin, 0, precision)) {
			return -1;		/* 0 to the 0th power means return error */
		}
		return 0;		/* otherwise, zero exponent means expout is 1 */
	}

	if (XMP_Test_Eq_Int(modulus, 0, precision)) {
		return -2;		/* zero modulus means error */
	}

	if (XMP_Compare(expin, modulus, precision) >= 0) {
		return -3;		/* if expin >= modulus, return error */
	}

	if (XMP_Compare(exponent_ptr, modulus, precision) >= 0) {
		return -4;		/* if exponent >= modulus, return error */
	}

	/* set smallest optimum precision for this modulus */
	int limited_precision = XMP_Significance(modulus, precision);

	if (XMP_Prepare_Modulus(modulus, limited_precision)) {
		return -5;		/* unstageable modulus (STEWART algorithm) */
	}

	/* normalize and compute number of bits in exponent first */
//	int exp_precision = XMP_Significance(exponent_ptr, limited_precision);
//	if (!exp_precision) return(0);
//	int bits = XMP_Digits_To_Bits(exp_precision);
//	exponent_ptr += (exp_precision-1);
//	digit high_bit_mask = UPPER_MOST_BIT;
//	while (! ((*exponent_ptr) & high_bit_mask)) {
//		high_bit_mask >>= 1;
//		bits--;
//	}

	int total_bit_count = XMP_Count_Bits(exponent_ptr, limited_precision);
	int sub_precision = XMP_Bits_To_Digits(total_bit_count);
	if (!sub_precision) return(0);
	digit high_bit_mask = XMP_Bits_To_Mask(total_bit_count);
	exponent_ptr += (sub_precision-1);

	/* We can "optimize out" the first modsquare and modmult: */
	total_bit_count--;			/* We know for sure at this point that bits>0 */

	XMP_Move(expout, expin, limited_precision);

	high_bit_mask >>= 1;
	if (!high_bit_mask) {
		high_bit_mask = UPPER_MOST_BIT;
		exponent_ptr--;
	}

	while (total_bit_count--) {
		XMP_Mod_Mult(product, expout, expout, limited_precision);

		if (((*exponent_ptr) & high_bit_mask)) {
			XMP_Mod_Mult(expout, product, expin, limited_precision);
		} else {
			XMP_Move(expout, product, limited_precision);
		}

		high_bit_mask >>= 1;
		if (!high_bit_mask) {
			high_bit_mask = UPPER_MOST_BIT;
			exponent_ptr--;
		}

	}

	XMP_Init(product, 0, limited_precision);
	XMP_Mod_Mult_Clear(limited_precision);		/* ask mp_modmult to also burn its own evidence */

	return 0;
}


/***********************************************************************************************
 * memrev -- Reverse the byte order of the buffer specified.                                   *
 *                                                                                             *
 *    This routine will reverse the byte order in the buffer specified.                        *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that will be reversed.                           *
 *                                                                                             *
 *          length   -- The length of the buffer.                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void memrev(char * buffer, size_t length)
{
	char * r2 = &(buffer[length - 1]);
	while (buffer < r2) {
		char b = *buffer;
		*buffer++ = *r2;
		*r2-- = b;
	}
}


int _USERENTRY pfunc(const void * pkey, const void * base)
{
	if (*(unsigned short *)pkey < *(unsigned short *)base) return(-1);
	if (*(unsigned short *)pkey > *(unsigned short *)base) return(1);
	return(0);
}


/***********************************************************************************************
 * XMP_Is_Small_Prime -- Determine if MP number is a small prime.                              *
 *                                                                                             *
 *    This routine will compare the MP number against all known small prime numbers. It will   *
 *    return true if a match was found.                                                        *
 *                                                                                             *
 * INPUT:   candidate   -- Pointer to MP number that is to be tested.                          *
 *                                                                                             *
 *          precision   -- The precision of the MP number specified.                           *
 *                                                                                             *
 * OUTPUT:  bool; Was the MP number a member of the small prime community?                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool XMP_Is_Small_Prime(const digit * candidate, int precision)
{
	/*
	**	If the number is too large for comparison to the known small primes table, then
	**	bail immediately.
	*/
	if (XMP_Significance(candidate, precision) > 1) return(false);
	if (*candidate > primeTable[ARRAY_SIZE(primeTable)-1]) return false;

	unsigned long * ptr = (unsigned long *)bsearch(&candidate, &primeTable[0], ARRAY_SIZE(primeTable), sizeof(primeTable[0]), pfunc);
	return(ptr != NULL);
}


/***********************************************************************************************
 * XMP_Small_Divisors_Test -- Perform the small divisors test on an MP number.                 *
 *                                                                                             *
 *    This test for primality will divide an MP number by the set of small primes. If any of   *
 *    these numbers divides evenly into the candidate number, then it is known that the        *
 *    candidate is NOT prime.                                                                  *
 *                                                                                             *
 * INPUT:   candidate   -- Pointer to the MP number that is to be tested.                      *
 *                                                                                             *
 *          precision   -- The precision of the MP number/                                     *
 *                                                                                             *
 * OUTPUT:  bool; Did the MP number pass the small divisors test?                              *
 *                                                                                             *
 * WARNINGS:   If the MP number passes, it doesn't mean that it is prime, just that is hasn't  *
 *             yet been proven to be not prime.                                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool XMP_Small_Divisors_Test(const digit * candidate, int precision)
{
	digit quotient[MAX_UNIT_PRECISION];

	for (unsigned i = 0; i < ARRAY_SIZE(primeTable); i++) {
		if (XMP_Unsigned_Div_Int(quotient, candidate, primeTable[i], precision) == 0) return(false);
	}
	return(true);
}


/***********************************************************************************************
 * XMP_Fermat_Test -- Performs Fermat's Little Theorem on an MP number.                        *
 *                                                                                             *
 *    This is a more expensive but thorough test for primality. The aggressiveness of this     *
 *    test can be controlled by the number of rounds specified. Four rounds is usually         *
 *    sufficient.                                                                              *
 *                                                                                             *
 * INPUT:   candidate   -- Pointer to the candidate MP number that is to be tested.            *
 *                                                                                             *
 *          rounds      -- The number of rounds to test the MP number (keep it small).         *
 *                                                                                             *
 *          precision   -- The precision of the MP number.                                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the number not proven to be not prime. A FALSE means that it is not      *
 *                prime. A TRUE means that it might be prime.                                  *
 *                                                                                             *
 * WARNINGS:   This takes a bit of time. The time it takes is directly controlled by the       *
 *             number of rounds specified. Keep the number of rounds as small as possible.     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool XMP_Fermat_Test(const digit * candidate_prime, unsigned rounds, int precision)
{
	assert(rounds < ARRAY_SIZE(primeTable));

	digit term[MAX_UNIT_PRECISION];
	XMP_Move(term, candidate_prime, precision);
	XMP_Dec(term, precision);

	for (unsigned i = 0; i < rounds; i++) {
		// if ((x**(p-1)) mod p) != 1, then p is not prime
		digit result[MAX_UNIT_PRECISION];

		digit small_prime[MAX_UNIT_PRECISION];
		XMP_Init(small_prime, primeTable[i], precision);

		xmp_exponent_mod(result, small_prime, term, candidate_prime, precision);

		if (!XMP_Test_Eq_Int(result, 1, precision)) return(false);
	}
	return(true);
}


/***********************************************************************************************
 * XMP_Rabin_Miller_Test -- Performs the Rabin Miller test for primality.                      *
 *                                                                                             *
 *    This test for primality is even more expensive the Fermat's Little Theorem. It doesn't   *
 *    prove that a number is prime, but it can prove that it is not prime.                     *
 *                                                                                             *
 * INPUT:   rng      -- Reference to to a random number generator.                             *
 *                                                                                             *
 *          candidate-- Pointer to the candidate MP number that is to be tested.               *
 *                                                                                             *
 *          rounds   -- The number of test rounds to perform.                                  *
 *                                                                                             *
 *          precision-- The precision of the MP number specified.                              *
 *                                                                                             *
 * OUTPUT:  bool; Was the number not proven to be not prime? A FALSE means that the number is  *
 *          not prime. A TRUE means that it might be.                                          *
 *                                                                                             *
 * WARNINGS:   This routine takes a long time. Use as few rounds as possible.                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool XMP_Rabin_Miller_Test(Straw & rng, digit const * w, int rounds, int precision)
{
	digit wminus1[MAX_UNIT_PRECISION];
	XMP_Sub_Int(wminus1, w, 1, 0, precision);

	unsigned maxbitprecision = precision * sizeof(digit) * 8;
	unsigned a;
	for (a = 0; a < maxbitprecision; a++) {
		if (XMP_Test_Bit(wminus1, a)) {
			break;
		}
	}

	digit m[MAX_UNIT_PRECISION];
	XMP_Move(m, wminus1, precision);
	XMP_Shift_Right_Bits(wminus1, a, precision);

	for (int i = 0; i < rounds; i++) {
		digit b[MAX_UNIT_PRECISION];
		digit temp[MAX_UNIT_PRECISION];
		XMP_Init(temp, 2, precision);
		XMP_Randomize(b, rng, temp, wminus1, precision);

		digit z[MAX_UNIT_PRECISION];
		xmp_exponent_mod(z, b, m, w, precision);

		if (XMP_Test_Eq_Int(z, 1, precision) || XMP_Compare(z, wminus1, precision) == 0) {
			continue;   // passes this round
		}

		int j;
		for (j = 1; j < (int)a; j++) {
			digit t2[MAX_UNIT_PRECISION];
			xmp_exponent_mod(t2, z, temp, w, precision);

			if (XMP_Compare(t2, wminus1, precision) == 0) {
				break;  // passed this round
			}
			if (XMP_Test_Eq_Int(z, 1, precision)) {
				return false;
			}
		}
		if (j == a) {
			return false;
		}
	}
	return true;
}


/***********************************************************************************************
 * XMP_Randomize -- Generate a random MP number.                                               *
 *                                                                                             *
 *    This routine will generate a random MP number with the number of bits precision          *
 *    specified. This is the starting point for generating large random prime numbers. It is   *
 *    very important that the random number generated is truly random.                         *
 *                                                                                             *
 * INPUT:   result   -- Pointer to the buffer that will hold the MP number.                    *
 *                                                                                             *
 *          rng      -- Reference to a random number generator.                                *
 *                                                                                             *
 *          total_bits-- The number of bits precision that the MP number must have.            *
 *                                                                                             *
 *          precision-- The precision of the MP number to be generated (maximum)               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Randomize(digit * result, Straw & rng, int total_bits, int precision)
{
	assert(XMP_Bits_To_Digits(total_bits) <= MAX_UNIT_PRECISION);

	total_bits = min(total_bits, precision * 32);

	unsigned nbytes = total_bits/8 + 1;

	XMP_Init(result, 0, precision);
	rng.Get(result, nbytes);

	((unsigned char *)result)[nbytes-1] &= (unsigned char)(~((~0) << (total_bits % 8)));
}


/***********************************************************************************************
 * XMP_Randomize -- Generate a random MP number between the boundaries specified.              *
 *                                                                                             *
 *    This routine will generate a random MP number but it will be bounded by the minimum      *
 *    and maximum MP numbers specified.                                                        *
 *                                                                                             *
 * INPUT:   result      -- Pointer to the MP buffer that will hold the result.                 *
 *                                                                                             *
 *          rng         -- Reference to a random number generator to use.                      *
 *                                                                                             *
 *          minval      -- Minimum value allowed.                                              *
 *                                                                                             *
 *          maxval      -- Maximum value allowed.                                              *
 *                                                                                             *
 *          precision   -- The precision of the MP numbers involved.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void XMP_Randomize(digit * result, Straw & rng, digit const * minval, digit const * maxval, int precision)
{
	digit range[MAX_UNIT_PRECISION];
	XMP_Sub(range, maxval, minval, 0, precision);
	unsigned int bit_count = XMP_Count_Bits(range, precision);
	do	{
		XMP_Randomize(result, rng, bit_count, precision);
	} while (XMP_Compare(result, range, precision) > 0);

	XMP_Add(result, result, minval, 0, precision);
}


/***********************************************************************************************
 * XMP_Is_Prime -- Determine if the specified MP number is prime.                              *
 *                                                                                             *
 *    This routine will perform some checks to try and determine if the specified MP number    *
 *    is a prime number. The result of this test is not 100% conclusive, but it is pretty      *
 *    darn close.                                                                              *
 *                                                                                             *
 * INPUT:   prime    -- Pointer to a candidate number to test for primality.                   *
 *                                                                                             *
 *          precision-- The precision of the MP number specified.                              *
 *                                                                                             *
 * OUTPUT:  bool; Was the number not proven to be not prime? If FALSE, then the number is      *
 *          not prime. If TRUE, then it might be.                                              *
 *                                                                                             *
 * WARNINGS:   This can take a very very very very very long time. Especially for the larger   *
 *             numbers.                                                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool XMP_Is_Prime(digit const * prime, int precision)
{
	/*
	**	Even numbers are ALWAYS not prime.
	*/
	if (!(*prime & 0x01)) return(false);

	/*
	**	Compare the prime number against the exhaustive list of prime
	**	numbers below 14 bits in size. If it finds a match, then
	**	the number is a known prime.
	*/
	if (XMP_Is_Small_Prime(prime, precision)) return(true);

	/*
	**	Perform the small divisors test. This is not exhaustive, but
	**	will weed out a large percentage of non-prime numbers.
	*/
	if (!XMP_Small_Divisors_Test(prime, precision)) return(false);

	/*
	**	Perform Fermat's Little Theorum on the candidate prime. Run
	**	the theorum for several rounds to ensure a high degree of
	**	confidence.
	*/
	if (!XMP_Fermat_Test(prime, 2, precision)) return(false);

	/*
	**	If all of the above tests have not confirmed primality nor
	**	confirmed non-primality, presume that the number must be prime.
	*/
	return(true);
}


/*
**	Complete list of all prime numbers that are less than 32719 (inclusive).
*/
unsigned short primeTable[3511] = {
	0x0002,0x0003,0x0005,0x0007,0x000B,0x000D,0x0011,0x0013,0x0017,0x001D,0x001F,0x0025,0x0029,0x002B,0x002F,0x0035,
	0x003B,0x003D,0x0043,0x0047,0x0049,0x004F,0x0053,0x0059,0x0061,0x0065,0x0067,0x006B,0x006D,0x0071,0x007F,0x0083,
	0x0089,0x008B,0x0095,0x0097,0x009D,0x00A3,0x00A7,0x00AD,0x00B3,0x00B5,0x00BF,0x00C1,0x00C5,0x00C7,0x00D3,0x00DF,
	0x00E3,0x00E5,0x00E9,0x00EF,0x00F1,0x00FB,0x0101,0x0107,0x010D,0x010F,0x0115,0x0119,0x011B,0x0125,0x0133,0x0137,
	0x0139,0x013D,0x014B,0x0151,0x015B,0x015D,0x0161,0x0167,0x016F,0x0175,0x017B,0x017F,0x0185,0x018D,0x0191,0x0199,
	0x01A3,0x01A5,0x01AF,0x01B1,0x01B7,0x01BB,0x01C1,0x01C9,0x01CD,0x01CF,0x01D3,0x01DF,0x01E7,0x01EB,0x01F3,0x01F7,
	0x01FD,0x0209,0x020B,0x021D,0x0223,0x022D,0x0233,0x0239,0x023B,0x0241,0x024B,0x0251,0x0257,0x0259,0x025F,0x0265,
	0x0269,0x026B,0x0277,0x0281,0x0283,0x0287,0x028D,0x0293,0x0295,0x02A1,0x02A5,0x02AB,0x02B3,0x02BD,0x02C5,0x02CF,
	0x02D7,0x02DD,0x02E3,0x02E7,0x02EF,0x02F5,0x02F9,0x0301,0x0305,0x0313,0x031D,0x0329,0x032B,0x0335,0x0337,0x033B,
	0x033D,0x0347,0x0355,0x0359,0x035B,0x035F,0x036D,0x0371,0x0373,0x0377,0x038B,0x038F,0x0397,0x03A1,0x03A9,0x03AD,
	0x03B3,0x03B9,0x03C7,0x03CB,0x03D1,0x03D7,0x03DF,0x03E5,0x03F1,0x03F5,0x03FB,0x03FD,0x0407,0x0409,0x040F,0x0419,
	0x041B,0x0425,0x0427,0x042D,0x043F,0x0443,0x0445,0x0449,0x044F,0x0455,0x045D,0x0463,0x0469,0x047F,0x0481,0x048B,
	0x0493,0x049D,0x04A3,0x04A9,0x04B1,0x04BD,0x04C1,0x04C7,0x04CD,0x04CF,0x04D5,0x04E1,0x04EB,0x04FD,0x04FF,0x0503,
	0x0509,0x050B,0x0511,0x0515,0x0517,0x051B,0x0527,0x0529,0x052F,0x0551,0x0557,0x055D,0x0565,0x0577,0x0581,0x058F,
	0x0593,0x0595,0x0599,0x059F,0x05A7,0x05AB,0x05AD,0x05B3,0x05BF,0x05C9,0x05CB,0x05CF,0x05D1,0x05D5,0x05DB,0x05E7,
	0x05F3,0x05FB,0x0607,0x060D,0x0611,0x0617,0x061F,0x0623,0x062B,0x062F,0x063D,0x0641,0x0647,0x0649,0x064D,0x0653,
	0x0655,0x065B,0x0665,0x0679,0x067F,0x0683,0x0685,0x069D,0x06A1,0x06A3,0x06AD,0x06B9,0x06BB,0x06C5,0x06CD,0x06D3,
	0x06D9,0x06DF,0x06F1,0x06F7,0x06FB,0x06FD,0x0709,0x0713,0x071F,0x0727,0x0737,0x0745,0x074B,0x074F,0x0751,0x0755,
	0x0757,0x0761,0x076D,0x0773,0x0779,0x078B,0x078D,0x079D,0x079F,0x07B5,0x07BB,0x07C3,0x07C9,0x07CD,0x07CF,0x07D3,
	0x07DB,0x07E1,0x07EB,0x07ED,0x07F7,0x0805,0x080F,0x0815,0x0821,0x0823,0x0827,0x0829,0x0833,0x083F,0x0841,0x0851,
	0x0853,0x0859,0x085D,0x085F,0x0869,0x0871,0x0883,0x089B,0x089F,0x08A5,0x08AD,0x08BD,0x08BF,0x08C3,0x08CB,0x08DB,
	0x08DD,0x08E1,0x08E9,0x08EF,0x08F5,0x08F9,0x0905,0x0907,0x091D,0x0923,0x0925,0x092B,0x092F,0x0935,0x0943,0x0949,
	0x094D,0x094F,0x0955,0x0959,0x095F,0x096B,0x0971,0x0977,0x0985,0x0989,0x098F,0x099B,0x09A3,0x09A9,0x09AD,0x09C7,
	0x09D9,0x09E3,0x09EB,0x09EF,0x09F5,0x09F7,0x09FD,0x0A13,0x0A1F,0x0A21,0x0A31,0x0A39,0x0A3D,0x0A49,0x0A57,0x0A61,
	0x0A63,0x0A67,0x0A6F,0x0A75,0x0A7B,0x0A7F,0x0A81,0x0A85,0x0A8B,0x0A93,0x0A97,0x0A99,0x0A9F,0x0AA9,0x0AAB,0x0AB5,
	0x0ABD,0x0AC1,0x0ACF,0x0AD9,0x0AE5,0x0AE7,0x0AED,0x0AF1,0x0AF3,0x0B03,0x0B11,0x0B15,0x0B1B,0x0B23,0x0B29,0x0B2D,
	0x0B3F,0x0B47,0x0B51,0x0B57,0x0B5D,0x0B65,0x0B6F,0x0B7B,0x0B89,0x0B8D,0x0B93,0x0B99,0x0B9B,0x0BB7,0x0BB9,0x0BC3,
	0x0BCB,0x0BCF,0x0BDD,0x0BE1,0x0BE9,0x0BF5,0x0BFB,0x0C07,0x0C0B,0x0C11,0x0C25,0x0C2F,0x0C31,0x0C41,0x0C5B,0x0C5F,
	0x0C61,0x0C6D,0x0C73,0x0C77,0x0C83,0x0C89,0x0C91,0x0C95,0x0C9D,0x0CB3,0x0CB5,0x0CB9,0x0CBB,0x0CC7,0x0CE3,0x0CE5,
	0x0CEB,0x0CF1,0x0CF7,0x0CFB,0x0D01,0x0D03,0x0D0F,0x0D13,0x0D1F,0x0D21,0x0D2B,0x0D2D,0x0D3D,0x0D3F,0x0D4F,0x0D55,
	0x0D69,0x0D79,0x0D81,0x0D85,0x0D87,0x0D8B,0x0D8D,0x0DA3,0x0DAB,0x0DB7,0x0DBD,0x0DC7,0x0DC9,0x0DCD,0x0DD3,0x0DD5,
	0x0DDB,0x0DE5,0x0DE7,0x0DF3,0x0DFD,0x0DFF,0x0E09,0x0E17,0x0E1D,0x0E21,0x0E27,0x0E2F,0x0E35,0x0E3B,0x0E4B,0x0E57,
	0x0E59,0x0E5D,0x0E6B,0x0E71,0x0E75,0x0E7D,0x0E87,0x0E8F,0x0E95,0x0E9B,0x0EB1,0x0EB7,0x0EB9,0x0EC3,0x0ED1,0x0ED5,
	0x0EDB,0x0EED,0x0EEF,0x0EF9,0x0F07,0x0F0B,0x0F0D,0x0F17,0x0F25,0x0F29,0x0F31,0x0F43,0x0F47,0x0F4D,0x0F4F,0x0F53,
	0x0F59,0x0F5B,0x0F67,0x0F6B,0x0F7F,0x0F95,0x0FA1,0x0FA3,0x0FA7,0x0FAD,0x0FB3,0x0FB5,0x0FBB,0x0FD1,0x0FD3,0x0FD9,
	0x0FE9,0x0FEF,0x0FFB,0x0FFD,0x1003,0x100F,0x101F,0x1021,0x1025,0x102B,0x1039,0x103D,0x103F,0x1051,0x1069,0x1073,
	0x1079,0x107B,0x1085,0x1087,0x1091,0x1093,0x109D,0x10A3,0x10A5,0x10AF,0x10B1,0x10BB,0x10C1,0x10C9,0x10E7,0x10F1,
	0x10F3,0x10FD,0x1105,0x110B,0x1115,0x1127,0x112D,0x1139,0x1145,0x1147,0x1159,0x115F,0x1163,0x1169,0x116F,0x1181,
	0x1183,0x118D,0x119B,0x11A1,0x11A5,0x11A7,0x11AB,0x11C3,0x11C5,0x11D1,0x11D7,0x11E7,0x11EF,0x11F5,0x11FB,0x120D,
	0x121D,0x121F,0x1223,0x1229,0x122B,0x1231,0x1237,0x1241,0x1247,0x1253,0x125F,0x1271,0x1273,0x1279,0x127D,0x128F,
	0x1297,0x12AF,0x12B3,0x12B5,0x12B9,0x12BF,0x12C1,0x12CD,0x12D1,0x12DF,0x12FD,0x1307,0x130D,0x1319,0x1327,0x132D,
	0x1337,0x1343,0x1345,0x1349,0x134F,0x1357,0x135D,0x1367,0x1369,0x136D,0x137B,0x1381,0x1387,0x138B,0x1391,0x1393,
	0x139D,0x139F,0x13AF,0x13BB,0x13C3,0x13D5,0x13D9,0x13DF,0x13EB,0x13ED,0x13F3,0x13F9,0x13FF,0x141B,0x1421,0x142F,
	0x1433,0x143B,0x1445,0x144D,0x1459,0x146B,0x146F,0x1471,0x1475,0x148D,0x1499,0x149F,0x14A1,0x14B1,0x14B7,0x14BD,
	0x14CB,0x14D5,0x14E3,0x14E7,0x1505,0x150B,0x1511,0x1517,0x151F,0x1525,0x1529,0x152B,0x1537,0x153D,0x1541,0x1543,
	0x1549,0x155F,0x1565,0x1567,0x156B,0x157D,0x157F,0x1583,0x158F,0x1591,0x1597,0x159B,0x15B5,0x15BB,0x15C1,0x15C5,
	0x15CD,0x15D7,0x15F7,0x1607,0x1609,0x160F,0x1613,0x1615,0x1619,0x161B,0x1625,0x1633,0x1639,0x163D,0x1645,0x164F,
	0x1655,0x1669,0x166D,0x166F,0x1675,0x1693,0x1697,0x169F,0x16A9,0x16AF,0x16B5,0x16BD,0x16C3,0x16CF,0x16D3,0x16D9,
	0x16DB,0x16E1,0x16E5,0x16EB,0x16ED,0x16F7,0x16F9,0x1709,0x170F,0x1723,0x1727,0x1733,0x1741,0x175D,0x1763,0x1777,
	0x177B,0x178D,0x1795,0x179B,0x179F,0x17A5,0x17B3,0x17B9,0x17BF,0x17C9,0x17CB,0x17D5,0x17E1,0x17E9,0x17F3,0x17F5,
	0x17FF,0x1807,0x1813,0x181D,0x1835,0x1837,0x183B,0x1843,0x1849,0x184D,0x1855,0x1867,0x1871,0x1877,0x187D,0x187F,
	0x1885,0x188F,0x189B,0x189D,0x18A7,0x18AD,0x18B3,0x18B9,0x18C1,0x18C7,0x18D1,0x18D7,0x18D9,0x18DF,0x18E5,0x18EB,
	0x18F5,0x18FD,0x1915,0x191B,0x1931,0x1933,0x1945,0x1949,0x1951,0x195B,0x1979,0x1981,0x1993,0x1997,0x1999,0x19A3,
	0x19A9,0x19AB,0x19B1,0x19B5,0x19C7,0x19CF,0x19DB,0x19ED,0x19FD,0x1A03,0x1A05,0x1A11,0x1A17,0x1A21,0x1A23,0x1A2D,
	0x1A2F,0x1A35,0x1A3F,0x1A4D,0x1A51,0x1A69,0x1A6B,0x1A7B,0x1A7D,0x1A87,0x1A89,0x1A93,0x1AA7,0x1AAB,0x1AAD,0x1AB1,
	0x1AB9,0x1AC9,0x1ACF,0x1AD5,0x1AD7,0x1AE3,0x1AF3,0x1AFB,0x1AFF,0x1B05,0x1B23,0x1B25,0x1B2F,0x1B31,0x1B37,0x1B3B,
	0x1B41,0x1B47,0x1B4F,0x1B55,0x1B59,0x1B65,0x1B6B,0x1B73,0x1B7F,0x1B83,0x1B91,0x1B9D,0x1BA7,0x1BBF,0x1BC5,0x1BD1,
	0x1BD7,0x1BD9,0x1BEF,0x1BF7,0x1C09,0x1C13,0x1C19,0x1C27,0x1C2B,0x1C2D,0x1C33,0x1C3D,0x1C45,0x1C4B,0x1C4F,0x1C55,
	0x1C73,0x1C81,0x1C8B,0x1C8D,0x1C99,0x1CA3,0x1CA5,0x1CB5,0x1CB7,0x1CC9,0x1CE1,0x1CF3,0x1CF9,0x1D09,0x1D1B,0x1D21,
	0x1D23,0x1D35,0x1D39,0x1D3F,0x1D41,0x1D4B,0x1D53,0x1D5D,0x1D63,0x1D69,0x1D71,0x1D75,0x1D7B,0x1D7D,0x1D87,0x1D89,
	0x1D95,0x1D99,0x1D9F,0x1DA5,0x1DA7,0x1DB3,0x1DB7,0x1DC5,0x1DD7,0x1DDB,0x1DE1,0x1DF5,0x1DF9,0x1E01,0x1E07,0x1E0B,
	0x1E13,0x1E17,0x1E25,0x1E2B,0x1E2F,0x1E3D,0x1E49,0x1E4D,0x1E4F,0x1E6D,0x1E71,0x1E89,0x1E8F,0x1E95,0x1EA1,0x1EAD,
	0x1EBB,0x1EC1,0x1EC5,0x1EC7,0x1ECB,0x1EDD,0x1EE3,0x1EEF,0x1EF7,0x1EFD,0x1F01,0x1F0D,0x1F0F,0x1F1B,0x1F39,0x1F49,
	0x1F4B,0x1F51,0x1F67,0x1F75,0x1F7B,0x1F85,0x1F91,0x1F97,0x1F99,0x1F9D,0x1FA5,0x1FAF,0x1FB5,0x1FBB,0x1FD3,0x1FE1,
	0x1FE7,0x1FEB,0x1FF3,0x1FFF,0x2011,0x201B,0x201D,0x2027,0x2029,0x202D,0x2033,0x2047,0x204D,0x2051,0x205F,0x2063,
	0x2065,0x2069,0x2077,0x207D,0x2089,0x20A1,0x20AB,0x20B1,0x20B9,0x20C3,0x20C5,0x20E3,0x20E7,0x20ED,0x20EF,0x20FB,
	0x20FF,0x210D,0x2113,0x2135,0x2141,0x2149,0x214F,0x2159,0x215B,0x215F,0x2173,0x217D,0x2185,0x2195,0x2197,0x21A1,
	0x21AF,0x21B3,0x21B5,0x21C1,0x21C7,0x21D7,0x21DD,0x21E5,0x21E9,0x21F1,0x21F5,0x21FB,0x2203,0x2209,0x220F,0x221B,
	0x2221,0x2225,0x222B,0x2231,0x2239,0x224B,0x224F,0x2263,0x2267,0x2273,0x2275,0x227F,0x2285,0x2287,0x2291,0x229D,
	0x229F,0x22A3,0x22B7,0x22BD,0x22DB,0x22E1,0x22E5,0x22ED,0x22F7,0x2303,0x2309,0x230B,0x2327,0x2329,0x232F,0x2333,
	0x2335,0x2345,0x2351,0x2353,0x2359,0x2363,0x236B,0x2383,0x238F,0x2395,0x23A7,0x23AD,0x23B1,0x23BF,0x23C5,0x23C9,
	0x23D5,0x23DD,0x23E3,0x23EF,0x23F3,0x23F9,0x2405,0x240B,0x2417,0x2419,0x2429,0x243D,0x2441,0x2443,0x244D,0x245F,
	0x2467,0x246B,0x2479,0x247D,0x247F,0x2485,0x249B,0x24A1,0x24AF,0x24B5,0x24BB,0x24C5,0x24CB,0x24CD,0x24D7,0x24D9,
	0x24DD,0x24DF,0x24F5,0x24F7,0x24FB,0x2501,0x2507,0x2513,0x2519,0x2527,0x2531,0x253D,0x2543,0x254B,0x254F,0x2573,
	0x2581,0x258D,0x2593,0x2597,0x259D,0x259F,0x25AB,0x25B1,0x25BD,0x25CD,0x25CF,0x25D9,0x25E1,0x25F7,0x25F9,0x2605,
	0x260B,0x260F,0x2615,0x2627,0x2629,0x2635,0x263B,0x263F,0x264B,0x2653,0x2659,0x2665,0x2669,0x266F,0x267B,0x2681,
	0x2683,0x268F,0x269B,0x269F,0x26AD,0x26B3,0x26C3,0x26C9,0x26CB,0x26D5,0x26DD,0x26EF,0x26F5,0x2717,0x2719,0x2735,
	0x2737,0x274D,0x2753,0x2755,0x275F,0x276B,0x276D,0x2773,0x2777,0x277F,0x2795,0x279B,0x279D,0x27A7,0x27AF,0x27B3,
	0x27B9,0x27C1,0x27C5,0x27D1,0x27E3,0x27EF,0x2803,0x2807,0x280D,0x2813,0x281B,0x281F,0x2821,0x2831,0x283D,0x283F,
	0x2849,0x2851,0x285B,0x285D,0x2861,0x2867,0x2875,0x2881,0x2897,0x289F,0x28BB,0x28BD,0x28C1,0x28D5,0x28D9,0x28DB,
	0x28DF,0x28ED,0x28F7,0x2903,0x2905,0x2911,0x2921,0x2923,0x293F,0x2947,0x295D,0x2965,0x2969,0x296F,0x2975,0x2983,
	0x2987,0x298F,0x299B,0x29A1,0x29A7,0x29AB,0x29BF,0x29C3,0x29D5,0x29D7,0x29E3,0x29E9,0x29ED,0x29F3,0x2A01,0x2A13,
	0x2A1D,0x2A25,0x2A2F,0x2A4F,0x2A55,0x2A5F,0x2A65,0x2A6B,0x2A6D,0x2A73,0x2A83,0x2A89,0x2A8B,0x2A97,0x2A9D,0x2AB9,
	0x2ABB,0x2AC5,0x2ACD,0x2ADD,0x2AE3,0x2AEB,0x2AF1,0x2AFB,0x2B13,0x2B27,0x2B31,0x2B33,0x2B3D,0x2B3F,0x2B4B,0x2B4F,
	0x2B55,0x2B69,0x2B6D,0x2B6F,0x2B7B,0x2B8D,0x2B97,0x2B99,0x2BA3,0x2BA5,0x2BA9,0x2BBD,0x2BCD,0x2BE7,0x2BEB,0x2BF3,
	0x2BF9,0x2BFD,0x2C09,0x2C0F,0x2C17,0x2C23,0x2C2F,0x2C35,0x2C39,0x2C41,0x2C57,0x2C59,0x2C69,0x2C77,0x2C81,0x2C87,
	0x2C93,0x2C9F,0x2CAD,0x2CB3,0x2CB7,0x2CCB,0x2CCF,0x2CDB,0x2CE1,0x2CE3,0x2CE9,0x2CEF,0x2CFF,0x2D07,0x2D1D,0x2D1F,
	0x2D3B,0x2D43,0x2D49,0x2D4D,0x2D61,0x2D65,0x2D71,0x2D89,0x2D9D,0x2DA1,0x2DA9,0x2DB3,0x2DB5,0x2DC5,0x2DC7,0x2DD3,
	0x2DDF,0x2E01,0x2E03,0x2E07,0x2E0D,0x2E19,0x2E1F,0x2E25,0x2E2D,0x2E33,0x2E37,0x2E39,0x2E3F,0x2E57,0x2E5B,0x2E6F,
	0x2E79,0x2E7F,0x2E85,0x2E93,0x2E97,0x2E9D,0x2EA3,0x2EA5,0x2EB1,0x2EB7,0x2EC1,0x2EC3,0x2ECD,0x2ED3,0x2EE7,0x2EEB,
	0x2F05,0x2F09,0x2F0B,0x2F11,0x2F27,0x2F29,0x2F41,0x2F45,0x2F4B,0x2F4D,0x2F51,0x2F57,0x2F6F,0x2F75,0x2F7D,0x2F81,
	0x2F83,0x2FA5,0x2FAB,0x2FB3,0x2FC3,0x2FCF,0x2FD1,0x2FDB,0x2FDD,0x2FE7,0x2FED,0x2FF5,0x2FF9,0x3001,0x300D,0x3023,
	0x3029,0x3037,0x303B,0x3055,0x3059,0x305B,0x3067,0x3071,0x3079,0x307D,0x3085,0x3091,0x3095,0x30A3,0x30A9,0x30B9,
	0x30BF,0x30C7,0x30CB,0x30D1,0x30D7,0x30DF,0x30E5,0x30EF,0x30FB,0x30FD,0x3103,0x3109,0x3119,0x3121,0x3127,0x312D,
	0x3139,0x3143,0x3145,0x314B,0x315D,0x3161,0x3167,0x316D,0x3173,0x317F,0x3191,0x3199,0x319F,0x31A9,0x31B1,0x31C3,
	0x31C7,0x31D5,0x31DB,0x31ED,0x31F7,0x31FF,0x3209,0x3215,0x3217,0x321D,0x3229,0x3235,0x3259,0x325D,0x3263,0x326B,
	0x326F,0x3275,0x3277,0x327B,0x328D,0x3299,0x329F,0x32A7,0x32AD,0x32B3,0x32B7,0x32C9,0x32CB,0x32CF,0x32D1,0x32E9,
	0x32ED,0x32F3,0x32F9,0x3307,0x3325,0x332B,0x332F,0x3335,0x3341,0x3347,0x335B,0x335F,0x3367,0x336B,0x3373,0x3379,
	0x337F,0x3383,0x33A1,0x33A3,0x33AD,0x33B9,0x33C1,0x33CB,0x33D3,0x33EB,0x33F1,0x33FD,0x3401,0x340F,0x3413,0x3419,
	0x341B,0x3437,0x3445,0x3455,0x3457,0x3463,0x3469,0x346D,0x3481,0x348B,0x3491,0x3497,0x349D,0x34A5,0x34AF,0x34BB,
	0x34C9,0x34D3,0x34E1,0x34F1,0x34FF,0x3509,0x3517,0x351D,0x352D,0x3533,0x353B,0x3541,0x3551,0x3565,0x356F,0x3571,
	0x3577,0x357B,0x357D,0x3581,0x358D,0x358F,0x3599,0x359B,0x35A1,0x35B7,0x35BD,0x35BF,0x35C3,0x35D5,0x35DD,0x35E7,
	0x35EF,0x3605,0x3607,0x3611,0x3623,0x3631,0x3635,0x3637,0x363B,0x364D,0x364F,0x3653,0x3659,0x3661,0x366B,0x366D,
	0x368B,0x368F,0x36AD,0x36AF,0x36B9,0x36BB,0x36CD,0x36D1,0x36E3,0x36E9,0x36F7,0x3701,0x3703,0x3707,0x371B,0x373F,
	0x3745,0x3749,0x374F,0x375D,0x3761,0x3775,0x377F,0x378D,0x37A3,0x37A9,0x37AB,0x37C9,0x37D5,0x37DF,0x37F1,0x37F3,
	0x37F7,0x3805,0x380B,0x3821,0x3833,0x3835,0x3841,0x3847,0x384B,0x3853,0x3857,0x385F,0x3865,0x386F,0x3871,0x387D,
	0x388F,0x3899,0x38A7,0x38B7,0x38C5,0x38C9,0x38CF,0x38D5,0x38D7,0x38DD,0x38E1,0x38E3,0x38FF,0x3901,0x391D,0x3923,
	0x3925,0x3929,0x392F,0x393D,0x3941,0x394D,0x395B,0x396B,0x3979,0x397D,0x3983,0x398B,0x3991,0x3995,0x399B,0x39A1,
	0x39A7,0x39AF,0x39B3,0x39BB,0x39BF,0x39CD,0x39DD,0x39E5,0x39EB,0x39EF,0x39FB,0x3A03,0x3A13,0x3A15,0x3A1F,0x3A27,
	0x3A2B,0x3A31,0x3A4B,0x3A51,0x3A5B,0x3A63,0x3A67,0x3A6D,0x3A79,0x3A87,0x3AA5,0x3AA9,0x3AB7,0x3ACD,0x3AD5,0x3AE1,
	0x3AE5,0x3AEB,0x3AF3,0x3AFD,0x3B03,0x3B11,0x3B1B,0x3B21,0x3B23,0x3B2D,0x3B39,0x3B45,0x3B53,0x3B59,0x3B5F,0x3B71,
	0x3B7B,0x3B81,0x3B89,0x3B9B,0x3B9F,0x3BA5,0x3BA7,0x3BAD,0x3BB7,0x3BB9,0x3BC3,0x3BCB,0x3BD1,0x3BD7,0x3BE1,0x3BE3,
	0x3BF5,0x3BFF,0x3C01,0x3C0D,0x3C11,0x3C17,0x3C1F,0x3C29,0x3C35,0x3C43,0x3C4F,0x3C53,0x3C5B,0x3C65,0x3C6B,0x3C71,
	0x3C85,0x3C89,0x3C97,0x3CA7,0x3CB5,0x3CBF,0x3CC7,0x3CD1,0x3CDD,0x3CDF,0x3CF1,0x3CF7,0x3D03,0x3D0D,0x3D19,0x3D1B,
	0x3D1F,0x3D21,0x3D2D,0x3D33,0x3D37,0x3D3F,0x3D43,0x3D6F,0x3D73,0x3D75,0x3D79,0x3D7B,0x3D85,0x3D91,0x3D97,0x3D9D,
	0x3DAB,0x3DAF,0x3DB5,0x3DBB,0x3DC1,0x3DC9,0x3DCF,0x3DF3,0x3E05,0x3E09,0x3E0F,0x3E11,0x3E1D,0x3E23,0x3E29,0x3E2F,
	0x3E33,0x3E41,0x3E57,0x3E63,0x3E65,0x3E77,0x3E81,0x3E87,0x3EA1,0x3EB9,0x3EBD,0x3EBF,0x3EC3,0x3EC5,0x3EC9,0x3ED7,
	0x3EDB,0x3EE1,0x3EE7,0x3EEF,0x3EFF,0x3F0B,0x3F0D,0x3F37,0x3F3B,0x3F3D,0x3F41,0x3F59,0x3F5F,0x3F65,0x3F67,0x3F79,
	0x3F7D,0x3F8B,0x3F91,0x3FAD,0x3FBF,0x3FCD,0x3FD3,0x3FDD,0x3FE9,0x3FEB,0x3FF1,0x3FFD,0x401B,0x4021,0x4025,0x402B,
	0x4031,0x403F,0x4043,0x4045,0x405D,0x4061,0x4067,0x406D,0x4087,0x4091,0x40A3,0x40A9,0x40B1,0x40B7,0x40BD,0x40DB,
	0x40DF,0x40EB,0x40F7,0x40F9,0x4109,0x410B,0x4111,0x4115,0x4121,0x4133,0x4135,0x413B,0x413F,0x4159,0x4165,0x416B,
	0x4177,0x417B,0x4193,0x41AB,0x41B7,0x41BD,0x41BF,0x41CB,0x41E7,0x41EF,0x41F3,0x41F9,0x4205,0x4207,0x4219,0x421F,
	0x4223,0x4229,0x422F,0x4243,0x4253,0x4255,0x425B,0x4261,0x4273,0x427D,0x4283,0x4285,0x4289,0x4291,0x4297,0x429D,
	0x42B5,0x42C5,0x42CB,0x42D3,0x42DD,0x42E3,0x42F1,0x4307,0x430F,0x431F,0x4325,0x4327,0x4333,0x4337,0x4339,0x434F,
	0x4357,0x4369,0x438B,0x438D,0x4393,0x43A5,0x43A9,0x43AF,0x43B5,0x43BD,0x43C7,0x43CF,0x43E1,0x43E7,0x43EB,0x43ED,
	0x43F1,0x43F9,0x4409,0x440B,0x4417,0x4423,0x4429,0x443B,0x443F,0x4445,0x444B,0x4451,0x4453,0x4459,0x4465,0x446F,
	0x4483,0x448F,0x44A1,0x44A5,0x44AB,0x44AD,0x44BD,0x44BF,0x44C9,0x44D7,0x44DB,0x44F9,0x44FB,0x4505,0x4511,0x4513,
	0x452B,0x4531,0x4541,0x4549,0x4553,0x4555,0x4561,0x4577,0x457D,0x457F,0x458F,0x45A3,0x45AD,0x45AF,0x45BB,0x45C7,
	0x45D9,0x45E3,0x45EF,0x45F5,0x45F7,0x4601,0x4603,0x4609,0x4613,0x4625,0x4627,0x4633,0x4639,0x463D,0x4643,0x4645,
	0x465D,0x4679,0x467B,0x467F,0x4681,0x468B,0x468D,0x469D,0x46A9,0x46B1,0x46C7,0x46C9,0x46CF,0x46D3,0x46D5,0x46DF,
	0x46E5,0x46F9,0x4705,0x470F,0x4717,0x4723,0x4729,0x472F,0x4735,0x4739,0x474B,0x474D,0x4751,0x475D,0x476F,0x4771,
	0x477D,0x4783,0x4787,0x4789,0x4799,0x47A5,0x47B1,0x47BF,0x47C3,0x47CB,0x47DD,0x47E1,0x47ED,0x47FB,0x4801,0x4807,
	0x480B,0x4813,0x4819,0x481D,0x4831,0x483D,0x4847,0x4855,0x4859,0x485B,0x486B,0x486D,0x4879,0x4897,0x489B,0x48A1,
	0x48B9,0x48CD,0x48E5,0x48EF,0x48F7,0x4903,0x490D,0x4919,0x491F,0x492B,0x4937,0x493D,0x4945,0x4955,0x4963,0x4969,
	0x496D,0x4973,0x4997,0x49AB,0x49B5,0x49D3,0x49DF,0x49E1,0x49E5,0x49E7,0x4A03,0x4A0F,0x4A1D,0x4A23,0x4A39,0x4A41,
	0x4A45,0x4A57,0x4A5D,0x4A6B,0x4A7D,0x4A81,0x4A87,0x4A89,0x4A8F,0x4AB1,0x4AC3,0x4AC5,0x4AD5,0x4ADB,0x4AED,0x4AEF,
	0x4B07,0x4B0B,0x4B0D,0x4B13,0x4B1F,0x4B25,0x4B31,0x4B3B,0x4B43,0x4B49,0x4B59,0x4B65,0x4B6D,0x4B77,0x4B85,0x4BAD,
	0x4BB3,0x4BB5,0x4BBB,0x4BBF,0x4BCB,0x4BD9,0x4BDD,0x4BDF,0x4BE3,0x4BE5,0x4BE9,0x4BF1,0x4BF7,0x4C01,0x4C07,0x4C0D,
	0x4C0F,0x4C15,0x4C1B,0x4C21,0x4C2D,0x4C33,0x4C4B,0x4C55,0x4C57,0x4C61,0x4C67,0x4C73,0x4C79,0x4C7F,0x4C8D,0x4C93,
	0x4C99,0x4CCD,0x4CE1,0x4CE7,0x4CF1,0x4CF3,0x4CFD,0x4D05,0x4D0F,0x4D1B,0x4D27,0x4D29,0x4D2F,0x4D33,0x4D41,0x4D51,
	0x4D59,0x4D65,0x4D6B,0x4D81,0x4D83,0x4D8D,0x4D95,0x4D9B,0x4DB1,0x4DB3,0x4DC9,0x4DCF,0x4DD7,0x4DE1,0x4DED,0x4DF9,
	0x4DFB,0x4E05,0x4E0B,0x4E17,0x4E19,0x4E1D,0x4E2B,0x4E35,0x4E37,0x4E3D,0x4E4F,0x4E53,0x4E5F,0x4E67,0x4E79,0x4E85,
	0x4E8B,0x4E91,0x4E95,0x4E9B,0x4EA1,0x4EAF,0x4EB3,0x4EB5,0x4EC1,0x4ECD,0x4ED1,0x4ED7,0x4EE9,0x4EFB,0x4F07,0x4F09,
	0x4F19,0x4F25,0x4F2D,0x4F3F,0x4F49,0x4F63,0x4F67,0x4F6D,0x4F75,0x4F7B,0x4F81,0x4F85,0x4F87,0x4F91,0x4FA5,0x4FA9,
	0x4FAF,0x4FB7,0x4FBB,0x4FCF,0x4FD9,0x4FDB,0x4FFD,0x4FFF,0x5003,0x501B,0x501D,0x5029,0x5035,0x503F,0x5045,0x5047,
	0x5053,0x5071,0x5077,0x5083,0x5093,0x509F,0x50A1,0x50B7,0x50C9,0x50D5,0x50E3,0x50ED,0x50EF,0x50FB,0x5107,0x510B,
	0x510D,0x5111,0x5117,0x5123,0x5125,0x5135,0x5147,0x5149,0x5171,0x5179,0x5189,0x518F,0x5197,0x51A1,0x51A3,0x51A7,
	0x51B9,0x51C1,0x51CB,0x51D3,0x51DF,0x51E3,0x51F5,0x51F7,0x5209,0x5213,0x5215,0x5219,0x521B,0x521F,0x5227,0x5243,
	0x5245,0x524B,0x5261,0x526D,0x5273,0x5281,0x5293,0x5297,0x529D,0x52A5,0x52AB,0x52B1,0x52BB,0x52C3,0x52C7,0x52C9,
	0x52DB,0x52E5,0x52EB,0x52FF,0x5315,0x531D,0x5323,0x5341,0x5345,0x5347,0x534B,0x535D,0x5363,0x5381,0x5383,0x5387,
	0x538F,0x5395,0x5399,0x539F,0x53AB,0x53B9,0x53DB,0x53E9,0x53EF,0x53F3,0x53F5,0x53FB,0x53FF,0x540D,0x5411,0x5413,
	0x5419,0x5435,0x5437,0x543B,0x5441,0x5449,0x5453,0x5455,0x545F,0x5461,0x546B,0x546D,0x5471,0x548F,0x5491,0x549D,
	0x54A9,0x54B3,0x54C5,0x54D1,0x54DF,0x54E9,0x54EB,0x54F7,0x54FD,0x5507,0x550D,0x551B,0x5527,0x552B,0x5539,0x553D,
	0x554F,0x5551,0x555B,0x5563,0x5567,0x556F,0x5579,0x5585,0x5597,0x55A9,0x55B1,0x55B7,0x55C9,0x55D9,0x55E7,0x55ED,
	0x55F3,0x55FD,0x560B,0x560F,0x5615,0x5617,0x5623,0x562F,0x5633,0x5639,0x563F,0x564B,0x564D,0x565D,0x565F,0x566B,
	0x5671,0x5675,0x5683,0x5689,0x568D,0x568F,0x569B,0x56AD,0x56B1,0x56D5,0x56E7,0x56F3,0x56FF,0x5701,0x5705,0x5707,
	0x570B,0x5713,0x571F,0x5723,0x5747,0x574D,0x575F,0x5761,0x576D,0x5777,0x577D,0x5789,0x57A1,0x57A9,0x57AF,0x57B5,
	0x57C5,0x57D1,0x57D3,0x57E5,0x57EF,0x5803,0x580D,0x580F,0x5815,0x5827,0x582B,0x582D,0x5855,0x585B,0x585D,0x586D,
	0x586F,0x5873,0x587B,0x588D,0x5897,0x58A3,0x58A9,0x58AB,0x58B5,0x58BD,0x58C1,0x58C7,0x58D3,0x58D5,0x58DF,0x58F1,
	0x58F9,0x58FF,0x5903,0x5917,0x591B,0x5921,0x5945,0x594B,0x594D,0x5957,0x595D,0x5975,0x597B,0x5989,0x5999,0x599F,
	0x59B1,0x59B3,0x59BD,0x59D1,0x59DB,0x59E3,0x59E9,0x59ED,0x59F3,0x59F5,0x59FF,0x5A01,0x5A0D,0x5A11,0x5A13,0x5A17,
	0x5A1F,0x5A29,0x5A2F,0x5A3B,0x5A4D,0x5A5B,0x5A67,0x5A77,0x5A7F,0x5A85,0x5A95,0x5A9D,0x5AA1,0x5AA3,0x5AA9,0x5ABB,
	0x5AD3,0x5AE5,0x5AEF,0x5AFB,0x5AFD,0x5B01,0x5B0F,0x5B19,0x5B1F,0x5B25,0x5B2B,0x5B3D,0x5B49,0x5B4B,0x5B67,0x5B79,
	0x5B87,0x5B97,0x5BA3,0x5BB1,0x5BC9,0x5BD5,0x5BEB,0x5BF1,0x5BF3,0x5BFD,0x5C05,0x5C09,0x5C0B,0x5C0F,0x5C1D,0x5C29,
	0x5C2F,0x5C33,0x5C39,0x5C47,0x5C4B,0x5C4D,0x5C51,0x5C6F,0x5C75,0x5C77,0x5C7D,0x5C87,0x5C89,0x5CA7,0x5CBD,0x5CBF,
	0x5CC3,0x5CC9,0x5CD1,0x5CD7,0x5CDD,0x5CED,0x5CF9,0x5D05,0x5D0B,0x5D13,0x5D17,0x5D19,0x5D31,0x5D3D,0x5D41,0x5D47,
	0x5D4F,0x5D55,0x5D5B,0x5D65,0x5D67,0x5D6D,0x5D79,0x5D95,0x5DA3,0x5DA9,0x5DAD,0x5DB9,0x5DC1,0x5DC7,0x5DD3,0x5DD7,
	0x5DDD,0x5DEB,0x5DF1,0x5DFD,0x5E07,0x5E0D,0x5E13,0x5E1B,0x5E21,0x5E27,0x5E2B,0x5E2D,0x5E31,0x5E39,0x5E45,0x5E49,
	0x5E57,0x5E69,0x5E73,0x5E75,0x5E85,0x5E8B,0x5E9F,0x5EA5,0x5EAF,0x5EB7,0x5EBB,0x5ED9,0x5EFD,0x5F09,0x5F11,0x5F27,
	0x5F33,0x5F35,0x5F3B,0x5F47,0x5F57,0x5F5D,0x5F63,0x5F65,0x5F77,0x5F7B,0x5F95,0x5F99,0x5FA1,0x5FB3,0x5FBD,0x5FC5,
	0x5FCF,0x5FD5,0x5FE3,0x5FE7,0x5FFB,0x6011,0x6023,0x602F,0x6037,0x6053,0x605F,0x6065,0x606B,0x6073,0x6079,0x6085,
	0x609D,0x60AD,0x60BB,0x60BF,0x60CD,0x60D9,0x60DF,0x60E9,0x60F5,0x6109,0x610F,0x6113,0x611B,0x612D,0x6139,0x614B,
	0x6155,0x6157,0x615B,0x616F,0x6179,0x6187,0x618B,0x6191,0x6193,0x619D,0x61B5,0x61C7,0x61C9,0x61CD,0x61E1,0x61F1,
	0x61FF,0x6209,0x6217,0x621D,0x6221,0x6227,0x623B,0x6241,0x624B,0x6251,0x6253,0x625F,0x6265,0x6283,0x628D,0x6295,
	0x629B,0x629F,0x62A5,0x62AD,0x62D5,0x62D7,0x62DB,0x62DD,0x62E9,0x62FB,0x62FF,0x6305,0x630D,0x6317,0x631D,0x632F,
	0x6341,0x6343,0x634F,0x635F,0x6367,0x636D,0x6371,0x6377,0x637D,0x637F,0x63B3,0x63C1,0x63C5,0x63D9,0x63E9,0x63EB,
	0x63EF,0x63F5,0x6401,0x6403,0x6409,0x6415,0x6421,0x6427,0x642B,0x6439,0x6443,0x6449,0x644F,0x645D,0x6467,0x6475,
	0x6485,0x648D,0x6493,0x649F,0x64A3,0x64AB,0x64C1,0x64C7,0x64C9,0x64DB,0x64F1,0x64F7,0x64F9,0x650B,0x6511,0x6521,
	0x652F,0x6539,0x653F,0x654B,0x654D,0x6553,0x6557,0x655F,0x6571,0x657D,0x658D,0x658F,0x6593,0x65A1,0x65A5,0x65AD,
	0x65B9,0x65C5,0x65E3,0x65F3,0x65FB,0x65FF,0x6601,0x6607,0x661D,0x6629,0x6631,0x663B,0x6641,0x6647,0x664D,0x665B,
	0x6661,0x6673,0x667D,0x6689,0x668B,0x6695,0x6697,0x669B,0x66B5,0x66B9,0x66C5,0x66CD,0x66D1,0x66E3,0x66EB,0x66F5,
	0x6703,0x6713,0x6719,0x671F,0x6727,0x6731,0x6737,0x673F,0x6745,0x6751,0x675B,0x676F,0x6779,0x6781,0x6785,0x6791,
	0x67AB,0x67BD,0x67C1,0x67CD,0x67DF,0x67E5,0x6803,0x6809,0x6811,0x6817,0x682D,0x6839,0x683B,0x683F,0x6845,0x684B,
	0x684D,0x6857,0x6859,0x685D,0x6863,0x6869,0x686B,0x6871,0x6887,0x6899,0x689F,0x68B1,0x68BD,0x68C5,0x68D1,0x68D7,
	0x68E1,0x68ED,0x68EF,0x68FF,0x6901,0x690B,0x690D,0x6917,0x6929,0x692F,0x6943,0x6947,0x6949,0x694F,0x6965,0x696B,
	0x6971,0x6983,0x6989,0x6997,0x69A3,0x69B3,0x69B5,0x69BB,0x69C1,0x69C5,0x69D3,0x69DF,0x69E3,0x69E5,0x69F7,0x6A07,
	0x6A2B,0x6A37,0x6A3D,0x6A4B,0x6A67,0x6A69,0x6A75,0x6A7B,0x6A87,0x6A8D,0x6A91,0x6A93,0x6AA3,0x6AC1,0x6AC9,0x6AE1,
	0x6AE7,0x6B05,0x6B0F,0x6B11,0x6B23,0x6B27,0x6B2D,0x6B39,0x6B41,0x6B57,0x6B59,0x6B5F,0x6B75,0x6B87,0x6B89,0x6B93,
	0x6B95,0x6B9F,0x6BBD,0x6BBF,0x6BDB,0x6BE1,0x6BEF,0x6BFF,0x6C05,0x6C19,0x6C29,0x6C2B,0x6C31,0x6C35,0x6C55,0x6C59,
	0x6C5B,0x6C5F,0x6C65,0x6C67,0x6C73,0x6C77,0x6C7D,0x6C83,0x6C8F,0x6C91,0x6C97,0x6C9B,0x6CA1,0x6CA9,0x6CAF,0x6CB3,
	0x6CC7,0x6CCB,0x6CEB,0x6CF5,0x6CFD,0x6D0D,0x6D0F,0x6D25,0x6D27,0x6D2B,0x6D31,0x6D39,0x6D3F,0x6D4F,0x6D5D,0x6D61,
	0x6D73,0x6D7B,0x6D7F,0x6D93,0x6D99,0x6DA5,0x6DB1,0x6DB7,0x6DC1,0x6DC3,0x6DCD,0x6DCF,0x6DDB,0x6DF7,0x6E03,0x6E15,
	0x6E17,0x6E29,0x6E33,0x6E3B,0x6E45,0x6E75,0x6E77,0x6E7B,0x6E81,0x6E89,0x6E93,0x6E95,0x6E9F,0x6EBD,0x6EBF,0x6EE3,
	0x6EE9,0x6EF3,0x6EF9,0x6EFB,0x6F0D,0x6F11,0x6F17,0x6F1F,0x6F2F,0x6F3D,0x6F4D,0x6F53,0x6F61,0x6F65,0x6F79,0x6F7D,
	0x6F83,0x6F85,0x6F8F,0x6F9B,0x6F9D,0x6FA3,0x6FAF,0x6FB5,0x6FBB,0x6FBF,0x6FCB,0x6FCD,0x6FD3,0x6FD7,0x6FE3,0x6FE9,
	0x6FF1,0x6FF5,0x6FF7,0x6FFD,0x700F,0x7019,0x701F,0x7027,0x7033,0x7039,0x704F,0x7051,0x7057,0x7063,0x7075,0x7079,
	0x7087,0x708D,0x7091,0x70A5,0x70AB,0x70BB,0x70C3,0x70C7,0x70CF,0x70E5,0x70ED,0x70F9,0x70FF,0x7105,0x7115,0x7121,
	0x7133,0x7151,0x7159,0x715D,0x715F,0x7163,0x7169,0x7183,0x7187,0x7195,0x71AD,0x71C3,0x71C9,0x71CB,0x71D1,0x71DB,
	0x71E1,0x71EF,0x71F5,0x71FB,0x7207,0x7211,0x7217,0x7219,0x7225,0x722F,0x723B,0x7243,0x7255,0x7267,0x7271,0x7277,
	0x727F,0x728F,0x7295,0x729B,0x72A3,0x72B3,0x72C7,0x72CB,0x72CD,0x72D7,0x72D9,0x72E3,0x72EF,0x72F5,0x72FD,0x7303,
	0x730D,0x7321,0x732B,0x733D,0x7357,0x735B,0x7361,0x737F,0x7381,0x7385,0x738D,0x7393,0x739F,0x73AB,0x73BD,0x73C1,
	0x73C9,0x73DF,0x73E5,0x73E7,0x73F3,0x7415,0x741B,0x742D,0x7439,0x743F,0x7441,0x745D,0x746B,0x747B,0x7489,0x748D,
	0x749B,0x74A7,0x74AB,0x74B1,0x74B7,0x74B9,0x74DD,0x74E1,0x74E7,0x74FB,0x7507,0x751F,0x7525,0x753B,0x753D,0x754D,
	0x755F,0x756B,0x7577,0x7589,0x758B,0x7591,0x7597,0x759D,0x75A1,0x75A7,0x75B5,0x75B9,0x75BB,0x75D1,0x75D9,0x75E5,
	0x75EB,0x75F5,0x75FB,0x7603,0x760F,0x7621,0x762D,0x7633,0x763D,0x763F,0x7655,0x7663,0x7669,0x766F,0x7673,0x7685,
	0x768B,0x769F,0x76B5,0x76B7,0x76C3,0x76DB,0x76DF,0x76F1,0x7703,0x7705,0x771B,0x771D,0x7721,0x772D,0x7735,0x7741,
	0x774B,0x7759,0x775D,0x775F,0x7771,0x7781,0x77A7,0x77AD,0x77B3,0x77B9,0x77C5,0x77CF,0x77D5,0x77E1,0x77E9,0x77EF,
	0x77F3,0x77F9,0x7807,0x7825,0x782B,0x7835,0x783D,0x7853,0x7859,0x7861,0x786D,0x7877,0x7879,0x7883,0x7885,0x788B,
	0x7895,0x7897,0x78A1,0x78AD,0x78BF,0x78D3,0x78D9,0x78DD,0x78E5,0x78FB,0x7901,0x7907,0x7925,0x792B,0x7939,0x793F,
	0x794B,0x7957,0x795D,0x7967,0x7969,0x7973,0x7991,0x7993,0x79A3,0x79AB,0x79AF,0x79B1,0x79B7,0x79C9,0x79CD,0x79CF,
	0x79D5,0x79D9,0x79F3,0x79F7,0x79FF,0x7A05,0x7A0F,0x7A11,0x7A15,0x7A1B,0x7A23,0x7A27,0x7A2D,0x7A4B,0x7A57,0x7A59,
	0x7A5F,0x7A65,0x7A69,0x7A7D,0x7A93,0x7A9B,0x7A9F,0x7AA1,0x7AA5,0x7AED,0x7AF5,0x7AF9,0x7B01,0x7B17,0x7B19,0x7B1D,
	0x7B2B,0x7B35,0x7B37,0x7B3B,0x7B4F,0x7B55,0x7B5F,0x7B71,0x7B77,0x7B8B,0x7B9B,0x7BA1,0x7BA9,0x7BAF,0x7BB3,0x7BC7,
	0x7BD3,0x7BE9,0x7BEB,0x7BEF,0x7BF1,0x7BFD,0x7C07,0x7C19,0x7C1B,0x7C31,0x7C37,0x7C49,0x7C67,0x7C69,0x7C73,0x7C81,
	0x7C8B,0x7C93,0x7CA3,0x7CD5,0x7CDB,0x7CE5,0x7CED,0x7CF7,0x7D03,0x7D09,0x7D1B,0x7D1D,0x7D33,0x7D39,0x7D3B,0x7D3F,
	0x7D45,0x7D4D,0x7D53,0x7D59,0x7D63,0x7D75,0x7D77,0x7D8D,0x7D8F,0x7D9F,0x7DAD,0x7DB7,0x7DBD,0x7DBF,0x7DCB,0x7DD5,
	0x7DE9,0x7DED,0x7DFB,0x7E01,0x7E05,0x7E29,0x7E2B,0x7E2F,0x7E35,0x7E41,0x7E43,0x7E47,0x7E55,0x7E61,0x7E67,0x7E6B,
	0x7E71,0x7E73,0x7E79,0x7E7D,0x7E91,0x7E9B,0x7E9D,0x7EA7,0x7EAD,0x7EB9,0x7EBB,0x7ED3,0x7EDF,0x7EEB,0x7EF1,0x7EF7,
	0x7EFB,0x7F13,0x7F15,0x7F19,0x7F31,0x7F33,0x7F39,0x7F3D,0x7F43,0x7F4B,0x7F5B,0x7F61,0x7F63,0x7F6D,0x7F79,0x7F87,
	0x7F8D,0x7FAF,0x7FB5,0x7FC3,0x7FC9,0x7FCD,0x7FCF
};
