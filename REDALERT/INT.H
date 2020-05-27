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

/* $Header: /CounterStrike/INT.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INT.H                                                        *
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


#ifndef INT_H
#define INT_H

#include <memory.h>
#include	<limits.h>
#include	<assert.h>
#include	"mp.h"
#include	"straw.h"

//#pragma warn -inl

template<int PRECISION>
class Int {
	public:

		/*
		**	Constructors and initializers.
		*/
		Int(void) {XMP_Init(&reg[0], 0, PRECISION);}
		Int(unsigned long value) {XMP_Init(&reg[0], value, PRECISION);}

		void Randomize(Straw & rng, int bitcount) {XMP_Randomize(&reg[0], rng, bitcount, PRECISION);}
		void Randomize(Straw & rng, const Int & minval, const Int & maxval) {XMP_Randomize(&reg[0], rng, minval, maxval, PRECISION); reg[0] |= 1;}

		/*
		**	Convenient conversion operators to get at the underlying array of
		**	integers. Big number math is basically manipulation of arbitrary
		**	length arrays.
		*/
		operator digit * () {return & reg[0];}
		operator const digit * () const {return & reg[0];}

		/*
		**	Array access operator (references bit position). Bit 0 is the first bit.
		*/
		bool operator[](unsigned bit) const {return(XMP_Test_Bit(&reg[0], bit));}

		/*
		**	Unary operators.
		*/
		Int & operator ++ (void) {XMP_Inc(&reg[0], PRECISION);return(*this);}
		Int & operator -- (void) {XMP_Dec(&reg[0], PRECISION);return(*this);}
		int operator ! (void) const {return(XMP_Test_Eq_Int(&reg[0], 0, PRECISION));}
		Int operator ~ (void) {XMP_Not(&reg[0], PRECISION);return(*this);}
		Int operator - (void) const {Int a = *this;a.Negate();return (a);}

		/*
		**	Attribute query functions.
		*/
		int ByteCount(void) const {return(XMP_Count_Bytes(&reg[0], PRECISION));}
		int BitCount(void) const {return(XMP_Count_Bits(&reg[0], PRECISION));}
		bool Is_Negative(void) const {return(XMP_Is_Negative(&reg[0], PRECISION));}
		unsigned MaxBitPrecision() const {return PRECISION*(sizeof(unsigned long)*CHAR_BIT);}
		bool IsSmallPrime(void) const {return(XMP_Is_Small_Prime(&reg[0], PRECISION));}
		bool SmallDivisorsTest(void) const {return(XMP_Small_Divisors_Test(&reg[0], PRECISION));}
		bool FermatTest(unsigned rounds) const {return(XMP_Fermat_Test(&reg[0], rounds, PRECISION));}
		bool IsPrime(void) const {return(XMP_Is_Prime(&reg[0], PRECISION));}
		bool RabinMillerTest(Straw & rng, unsigned int rounds) const {return(XMP_Rabin_Miller_Test(rng, &reg[0], rounds, PRECISION));}

		/*
		**	'in-place' binary operators.
		*/
		Int & operator += (const Int & number) {Carry = XMP_Add(&reg[0], &reg[0], number, 0, PRECISION);return(*this);}
		Int & operator -= (const Int & number) {Borrow = XMP_Sub(&reg[0], &reg[0], number, 0, PRECISION);return(*this);}
		Int & operator *= (const Int & multiplier) {Remainder = *this;Error=XMP_Signed_Mult(&reg[0], Remainder, multiplier, PRECISION);return(*this);}
		Int & operator /= (const Int & t) {*this = (*this) / t;return *this;}
		Int & operator %= (const Int & t) {*this = (*this) % t;return *this;}
		Int &  operator <<= (int bits) {XMP_Shift_Left_Bits(&reg[0], bits, PRECISION);return *this;}
		Int &  operator >>= (int bits) {XMP_Shift_Right_Bits(&reg[0], bits, PRECISION);return *this;}

		/*
		**	Mathematical binary operators.
		*/
		Int operator + (const Int & number) const {Int term;Carry = XMP_Add(term, &reg[0], number, 0, PRECISION);return(term);}
		Int operator + (unsigned short b) const {Int result;Carry=XMP_Add_Int(result, &reg[0], b, 0, PRECISION);return(result);}
//		friend Int<PRECISION> operator + (digit b, const Int<PRECISION> & a) {return(Int<PRECISION>(b) + a);}
		Int operator - (const Int & number) const {Int term;Borrow = XMP_Sub(term, &reg[0], number, 0, PRECISION);return(term);}
		Int operator - (unsigned short b) const {Int result;Borrow = XMP_Sub_Int(result, &reg[0], b, 0, PRECISION);return(result);}
//		friend Int<PRECISION> operator - (digit b, const Int<PRECISION> & a) {return(Int<PRECISION>(b) - a);}
		Int operator * (const Int & multiplier) const {Int result;Error=XMP_Signed_Mult(result, &reg[0], multiplier, PRECISION);return result;}
		Int operator * (unsigned short b) const {Int result;Error=XMP_Unsigned_Mult_Int(result, &reg[0], b, PRECISION);return(result);}
//		friend Int<PRECISION> operator * (digit b, const Int<PRECISION> & a) {return(Int<PRECISION>(b) * a);}
		Int operator / (const Int & divisor) const {Int quotient = *this;XMP_Signed_Div(Remainder, quotient, &reg[0], divisor, PRECISION);return (quotient);}
		Int operator / (unsigned long b) const {return(*this / Int<PRECISION>(b));}
		Int operator / (unsigned short divisor) const {Int quotient;Error=XMP_Unsigned_Div_Int(quotient, &reg[0], divisor, PRECISION);return(quotient);}
//		friend Int<PRECISION> operator / (digit a, const Int<PRECISION> & b) {return(Int<PRECISION>(a) / b);}
		Int operator % (const Int & divisor) const {Int remainder;XMP_Signed_Div(remainder, Remainder, &reg[0], divisor, PRECISION);return(remainder);}
		Int operator % (unsigned long b) const {return(*this % Int<PRECISION>(b));}
		unsigned short operator % (unsigned short divisor) const {return(XMP_Unsigned_Div_Int(Remainder, &reg[0], divisor, PRECISION));}
//		friend Int<PRECISION> operator % (digit a, const Int<PRECISION> & b) {return(Int<PRECISION>(a) % b);}

		/*
		**	Bitwise binary operators.
		*/
		Int operator >> (int bits) const {Int result = *this; XMP_Shift_Right_Bits(result, bits, PRECISION);return result;}
		Int operator << (int bits) const {Int result = *this; XMP_Shift_Left_Bits(result, bits, PRECISION);return result;}

		/*
		**	Comparison binary operators.
		*/
		int operator == (const Int &b) const {return (memcmp(&reg[0], &b.reg[0], (MAX_BIT_PRECISION/CHAR_BIT))==0);}
		int operator != (const Int& b) const {return !(*this == b);}
		int operator > (const Int & number) const {return(XMP_Compare(&reg[0], number, PRECISION) > 0);}
		int operator >= (const Int & number) const {return(XMP_Compare(&reg[0], number, PRECISION) >= 0);}
		int operator < (const Int & number) const {return(XMP_Compare(&reg[0], number, PRECISION) < 0);}
		int operator <= (const Int & number) const {return(XMP_Compare(&reg[0], number, PRECISION) <= 0);}

		/*
		**	Misc. mathematical and logical functions.
		*/
		void Negate(void) {XMP_Neg(&reg[0], PRECISION);}
		Int Abs(void) {XMP_Abs(&reg[0], PRECISION);return(*this);}
		Int times_b_mod_c(Int const & multiplier, Int const & modulus) const {
			Int result;
			Error=xmp_stage_modulus(modulus, PRECISION);
			Error=XMP_Mod_Mult(result, &reg[0], multiplier, PRECISION);
			XMP_Mod_Mult_Clear(PRECISION);
			return result;
		}

		Int exp_b_mod_c(const Int & e, const Int & m) const {
			Int result;
			Error=xmp_exponent_mod(result, &reg[0], e, m, PRECISION);
			return result;
		}


		static Int Unsigned_Mult(Int const & multiplicand, Int const & multiplier) {Int product;Error=XMP_Unsigned_Mult(&product.reg[0], &multiplicand.reg[0], &multiplier.reg[0], PRECISION);return(product);}
		static void Unsigned_Divide(Int & remainder, Int & quotient, const Int & dividend, const Int & divisor) {Error=XMP_Unsigned_Div(remainder, quotient, dividend, divisor, PRECISION);}
		static void Signed_Divide(Int & remainder, Int & quotient, const Int & dividend, const Int & divisor) {XMP_Signed_Div(remainder, quotient, dividend, divisor, PRECISION);}
		Int Inverse(const Int & modulus) const {Int result;XMP_Inverse_A_Mod_B(result, &reg[0], modulus, PRECISION);return(result);}

		static Int Decode_ASCII(char const * string) {Int result;XMP_Decode_ASCII(string, result, PRECISION);return(result);}

		// Number (sign independand) inserted into buffer.
		int Encode(unsigned char *output) const {return(XMP_Encode(output, &reg[0], PRECISION));}
		int Encode(unsigned char * output, unsigned length) const {return(XMP_Encode(output, length, &reg[0], PRECISION));}
		void Signed_Decode(const unsigned char * from, int frombytes) {XMP_Signed_Decode(&reg[0], from, frombytes, PRECISION);}
		void Unsigned_Decode(const unsigned char * from, int frombytes) {XMP_Unsigned_Decode(&reg[0], from, frombytes, PRECISION);}

		// encode Int using Distinguished Encoding Rules, returns size of output
		int DEREncode(unsigned char * output) const {return(XMP_DER_Encode(&reg[0], output, PRECISION));}
		void DERDecode(const unsigned char *input) {XMP_DER_Decode(&reg[0], input, PRECISION);}

		// Friend helper functions.
		friend Int<PRECISION> Generate_Prime(Straw & rng, int pbits, Int<PRECISION> const * = 0);
		friend Int<PRECISION> Gcd(const Int<PRECISION> & a, const Int<PRECISION> & b);
//		friend bool NextPrime(Int<PRECISION> & p, const Int<PRECISION> & max, bool blumInt=false);
//		friend Int<PRECISION> a_exp_b_mod_pq(const Int<PRECISION> & a, const Int<PRECISION> & ep, const Int<PRECISION> & eq, const Int<PRECISION> & p, const Int<PRECISION> & q, const Int<PRECISION> & u);

		static int Error;

		// Carry result from last addition.
		static bool Carry;

		// Borrow result from last subtraction.
		static bool Borrow;

		// Remainder value from the various division routines.
		static Int Remainder;


	private:
		digit reg[PRECISION];


		struct RemainderTable
		{
			RemainderTable(const Int<PRECISION> & p) : HasZeroEntry(false)
			{
				for (unsigned i = 0; i < ARRAY_SIZE(primeTable); i++) {
					table[i] = p % primeTable[i];
				}
			}
			bool HasZero() const {return(HasZeroEntry);}
			void Increment(unsigned short increment = 1)
			{
				HasZeroEntry = false;
				for (unsigned int i = 0; i < ARRAY_SIZE(primeTable); i++) {
					table[i] += increment;
					while (table[i] >= primeTable[i]) {
						table[i] -= primeTable[i];
					}
					HasZeroEntry = (HasZeroEntry || !table[i]);
				}
			}
			void Increment(const RemainderTable & rtQ)
			{
				HasZeroEntry = false;
				for (unsigned int i = 0; i < ARRAY_SIZE(primeTable); i++) {
					table[i] += rtQ.table[i];
					if (table[i] >= primeTable[i]) {
						table[i] -= primeTable[i];
					}
					HasZeroEntry = (HasZeroEntry || !table[i]);
				}
			}

			bool HasZeroEntry;
			unsigned short table[ARRAY_SIZE(primeTable)];
		};

};


template<class T>
T Gcd(const T & a, const T & n)
{
	T g[3]={n, a, 0UL};

	unsigned int i = 1;
	while (!!g[i%3]) {
		g[(i+1)%3] = g[(i-1)%3] % g[i%3];
		i++;
	}
	return g[(i-1)%3];
}



//#pragma warning 604 9
//#pragma warning 595 9
template<class T>
T Generate_Prime(Straw & rng, int pbits, T const *)
{
	T minQ = (T(1UL) << (unsigned short)(pbits-(unsigned short)2));
	T maxQ = ((T(1UL) << (unsigned short)(pbits-(unsigned short)1)) - (unsigned short)1);

	T q;
	T p;

	do {
		q.Randomize(rng, minQ, maxQ);
		p = (q*2) + (unsigned short)1;

		T::RemainderTable rtQ(q);
		T::RemainderTable rtP(p);

		while (rtQ.HasZero() || rtP.HasZero() || !q.IsPrime() || !p.IsPrime()) {
			q += 2;
			p += 4;
			if (q > maxQ) break;

			rtQ.Increment(2);
			rtP.Increment(4);
		}
	} while (q > maxQ);

	return(p);
}






typedef Int<MAX_UNIT_PRECISION>	bignum;
typedef Int<MAX_UNIT_PRECISION>	BigInt;



//BigInt Gcd(const BigInt & a, const BigInt & n);
//BigInt Generate_Prime(RandomNumberGenerator & rng, int pbits, BigInt const * dummy);

#endif

