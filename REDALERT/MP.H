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

/* $Header: /CounterStrike/MP.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O                 ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MP.H                                                         *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 04/26/96                                                     *
 *                                                                                             *
 *                  Last Update : April 26, 1996 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MP_H
#define MP_H

//lint -e740 -e534 -e537 -e760

//lint -d_LINT=1
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


#include	"straw.h"
#include	<stdlib.h>

extern unsigned short primeTable[3511];


#define	digit	unsigned long
#define	signeddigit	signed long
#define	LOG_UNITSIZE			5
#define	UNITSIZE					32
#define	UPPER_MOST_BIT			0x80000000L
#define	SEMI_UPPER_MOST_BIT	0x8000
#define	SEMI_MASK				((unsigned short)~0)
#define	MAX_BIT_PRECISION		2048
#define	MAX_UNIT_PRECISION	(MAX_BIT_PRECISION/UNITSIZE)
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))
#endif


int XMP_Significance(const digit * r, int precision);
void XMP_Inc(digit * r, int precision);
void XMP_Dec(digit * r, int precision);
void XMP_Neg(digit * r, int precision);
void XMP_Abs(digit * r, int precision);
void XMP_Shift_Right_Bits(digit * r1, int bits, int precision);
void XMP_Shift_Left_Bits(digit * r1, int bits, int precision);
bool XMP_Rotate_Left(digit * r1, bool carry, int precision);
void XMP_Not(digit * digit_ptr, int precision);
void XMP_Init(digit * r, digit value, int precision);
unsigned XMP_Count_Bits(const digit * r, int precision);
int XMP_Count_Bytes(const digit * r, int precision);
void XMP_Move(digit * dest, digit const * source, int precision);
int XMP_Compare(const digit * r1, const digit * r2, int precision);
bool XMP_Add(digit * result, const digit * r1, const digit * r2, bool carry, int precision);
bool XMP_Add_Int(digit * result, const digit * r1, digit r2, bool carry, int precision);
bool XMP_Sub(digit * result, const digit * r1, const digit * r2, bool borrow, int precision);
bool XMP_Sub_Int(digit * result, const digit * r1, unsigned short r2, bool borrow, int precision);
int XMP_Unsigned_Mult(digit * prod, const digit * multiplicand, const digit * multiplier, int precision);
int XMP_Unsigned_Mult_Int(digit * prod, const digit * multiplicand, short multiplier, int precision);
int XMP_Signed_Mult_Int(digit * prod, const digit * multiplicand, signed short multiplier, int precision);
int XMP_Signed_Mult(digit * prod, const digit * multiplicand, const digit * multiplier, int precision);
unsigned short XMP_Unsigned_Div_Int(digit * quotient, digit const * dividend, unsigned short divisor, int precision);
int XMP_Unsigned_Div(digit * remainder, digit * quotient, digit const * dividend, digit const * divisor, int precision);
void XMP_Signed_Div(digit * remainder, digit * quotient, digit const * dividend, digit const * divisor, int precision);
int XMP_Reciprocal(digit * quotient, const digit * divisor, int precision);
void XMP_Decode_ASCII(char const * str, digit * mpn, int precision);
void xmp_single_mul(unsigned short * prod, unsigned short * multiplicand, unsigned short multiplier, int precision);
void XMP_Double_Mul(digit * prod, const digit * multiplicand, const digit * multiplier, int precision);
int xmp_stage_modulus(const digit * n_modulus, int precision);
int XMP_Mod_Mult(digit * prod, const digit * multiplicand, const digit * multiplier, int precision);
void XMP_Mod_Mult_Clear(int precision);
unsigned short mp_quo_digit(unsigned short * dividend);
int xmp_exponent_mod(digit * expout, const digit * expin, const digit * exponent_ptr, const digit * modulus, int precision);
bool XMP_Is_Small_Prime(const digit * candidate, int precision);
bool XMP_Small_Divisors_Test(const digit * candidate, int precision);
bool XMP_Fermat_Test(const digit * candidate_prime, unsigned rounds, int precision);
void XMP_Inverse_A_Mod_B(digit * result, digit const * number, digit const * modulus, int precision);
void XMP_Signed_Decode(digit * result, const unsigned char * from, int frombytes, int precision);
void XMP_Unsigned_Decode(digit * result, const unsigned char * from, int frombytes, int precision);
unsigned XMP_Encode(unsigned char * to, digit const * from, int precision);
unsigned XMP_Encode(unsigned char * to, unsigned tobytes, digit const * from, int precision);
void XMP_Randomize(digit * result, Straw & rng, int nbits, int precision);
void XMP_Randomize(digit * result, Straw & rng, digit const * min, digit const * max, int precision);
bool XMP_Is_Prime(digit const * prime, int precision);
bool XMP_Rabin_Miller_Test(Straw & rng, digit const * w, int rounds, int precision);
int XMP_DER_Length_Encode(unsigned long length, unsigned char * output);
int XMP_DER_Encode(digit const * from, unsigned char * output, int precision);
void XMP_DER_Decode(digit * result, unsigned char const * input, int precision);



inline int XMP_Digits_To_Bits(int digits)
{
	return(digits << LOG_UNITSIZE);
}


inline int XMP_Bits_To_Digits(int bits)
{
	return ((bits + (UNITSIZE-1)) / UNITSIZE);
}


inline digit XMP_Bits_To_Mask(int bits)
{
	if (!bits) return(0);
	return(1 << ((bits-1) % UNITSIZE));
}


inline bool XMP_Is_Negative(const digit * r, int precision)
{
	return((signeddigit) *(r + (precision-1)) < 0);
}


inline bool XMP_Test_Eq_Int(digit const * r, int i, int p)
{
	return( (*r == i ) && XMP_Significance(r,p) <= 1 );
}


inline void XMP_Set_Bit(digit * r, unsigned bit)
{
	r[bit >> LOG_UNITSIZE] |= ((digit)1 << (bit & (UNITSIZE-1)));
}

inline bool XMP_Test_Bit(const digit * r, unsigned bit)
{
	return (r[bit >> LOG_UNITSIZE] & ((digit)1 << (bit & (UNITSIZE-1))));
}



// Misc functions.
void memrev(char * buffer, size_t length);

#endif
