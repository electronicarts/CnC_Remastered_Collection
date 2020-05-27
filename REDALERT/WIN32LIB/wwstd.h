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

/***************************************************************************
 **     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : wwstd.h                                  *
 *                                                                         *
 *                    File Name : WWLIB.H                                  *
 *                                                                         *
 *                   Programmer : Jeff Wilson                              *
 *                                                                         *
 *                   Start Date : March 1, 1994                            *
 *                                                                         *
 *                  Last Update : March 1, 1994   []                       *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef WWSTD_H
#define	WWSTD_H


//
// Win 95 includes
//

#ifndef WIN32
#ifndef _WIN32 // Denzil 6/2/98 Watcom 11.0 complains without this check
#define _WIN32
#endif // _WIN32
#define WIN32 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#else
#include <windows.h>
#include <windowsx.h>
#endif

// Note: SKB 4/11/94
// Before this library is done, this needs to be able to be set to TRUE.
// Once it is, the FALSE parts should be removed from the source code.
#define LIB_EXTERNS_RESOLVED FALSE


#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
//#include <bios.h>

//================================

// TRUE and FALSE are defined in pltypes.h

#ifndef IBM
#define IBM				TRUE
#endif

#ifndef AMIGA
#define AMIGA			FALSE
#endif

#ifndef	SEGA
#define	SEGA			FALSE
#endif

/*
**	Common constants used in normal code.
*/
#define WW_ERROR		-1

#if (0) //PG
#ifdef NULL
#undef	NULL
#endif

#ifndef NULL
#define NULL		0L
#endif

#ifdef VOID
#undef VOID
#endif
#endif

#define PRIVATE		static
#define PUBLIC			/* Routines & data don't have a specifier */

#ifdef __cplusplus
#define	__CPPARGS	...
#else
#define	__CPPARGS
#endif

// This macro will get the size (in elements) of the specified array.
#ifdef GET_SIZE
#undef GET_SIZE
#endif
#define GET_SIZE(a)					((sizeof(a) / sizeof(*a)))

//#pragma option -Jg
// Returns the absolute value of the number.
#ifdef ABS
#undef ABS
#endif
template<class T> T ABS(T a)
{
	return (a < 0) ? -a : a;
}
//int ABS(int);
//long ABS(long);

// Returns the minimum of the two numbers.
#ifdef MIN
#undef MIN
#endif
template<class T> T MIN(T a, T b)
{
	return (b < a) ? b : a;
};
//PG
//short MIN(short, short);
//int MIN(int, int);
//long MIN(long, long);

// Returns the maximum of the two numbers.
#ifdef MAX
#undef MAX
#endif
template<class T> T MAX(T a, T b)
{
	return (b > a) ? b : a;
};
//short MAX(short, short);
//int MAX(int, int);
//long MAX(long, long);
//#pragma option -Jgd

// Returns the low word of a long
#define	LOW_WORD(a)		((unsigned short)((long)(a) & 0x0000FFFFL))

// Returns the high word of a long
#define	HIGH_WORD(a)	((unsigned long)(a) >> 16)

// Merges to shorts to become a long
#define	MAKE_LONG(a,b) (((long)(a) << 16) | (long)((b)&0x0000FFFFL))

/*
** Macro allows our routines to act like
** sprintf etc..
*/
#ifdef AssembleTo
#undef AssembleTo
#endif

#define AssembleTo(dest,fmt)\
{\
	va_list argptr;\
	if (fmt != (dest))\
		{\
		va_start (argptr, fmt);\
		vsprintf ((dest), fmt, argptr);\
		va_end (argptr);\
		}\
}

#if (0)//PG
// type definitions
//=======================================
typedef void 	VOID;

//==================================================
// Pharlape defines these for use so we use their
// typedefs!
//		typedef	unsigned char	BOOL;
//		typedef signed long		LONG;
//		typedef unsigned long	ULONG;
//==================================================
#ifndef PRIVATE
#define  PRIVATE	static
#endif

// The purpose of the INT and UINT is for efficiency.  It says that while a short int (16 bit)
// has enough precision, it is more efficient to pass in an int (32 bits).  For efficiency, most
// WORD and UWORD should be an INT or UINT, especially on the stack and structures that will
// not be in large arrays.  When size efficiency is more important then speed, use WORD UWORD.

#define	VOID		void

//#pragma	warn -eas
#define	TRUE		1
#define	FALSE		0

/*
**	The "bool" integral type was defined by the C++ comittee in
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
//#define	true		1
//#define	false		0

#define	BOOL		int					// 32 bits for speed. use CHAR for size optimizations.
#if(0)
#ifndef HMI_DRIVER
#define	INT		int
#define	UINT		unsigned int
#define	BYTE		char
#define	UBYTE		unsigned char
#define  UCHAR	   unsigned char
#define	WORD		signed 	short
#define	UWORD		unsigned short
#define  USHORT	unsigned short

#define	LONG		signed 	long
#define	ULONG		unsigned long
#define  REALPTR	unsigned long

#define	FARPTR  char far *

#endif
#endif
#endif //PG

/*
**	The type of processor running on this system as
**	returned by Processor().
*/
#define	PROC_80386		0
#define	PROC_80486		1
#define	PROC_PENTIUM	2


// Inline Routines
//様様様様様様様様
//
// These Template functions are generally used
// by classes when they havce over loaded > and <.
//
#ifdef __cplusplus
template<class T> T Min(T a, T b)
{
	return (a<b ? a : b);
}

template<class T> inline  T Max(T a, T b)
{
	return (a>b ? a : b);
}

template<class T> T Abs(T a)
{
	return ((a<0) ? -(a) : a);
}

template<class T> VOID minimize(T &a, T b)
{
	if( b<a )
		a=b;
}

template<class T> VOID maximize(T &a, T b)
{
	if( b>a )
		a=b;
}
#endif

/*
**	Macros that control bit settings in a variable.
*/
#define Bit_Flags_On(a,b)			a |= (b)
#define Bit_Flags_Off(a,b)			a &= (~(b))
#define Bit_Flags_Value(a,b)		(a & (b))
#define Bit_Flags_Flip(a,b)		a ^= (b)

// Template replacements for the user defines above
#ifdef __cplusplus
template<class T> VOID BitFlagsOn(T &a, T b)
{
	a |= (b);
}

template<class T> VOID BitFlagsOff(T &a, T b)
{
	a &= (~(b));
}

template<class T> T BitFlagsValue(T a, T b)
{
	return (a & (b));
}

template<class T> VOID BitFlagsFlip(T &a, T b)
{
	a ^= (b);
}
#endif

typedef enum : unsigned short {
	TBLACK,
	PURPLE,
	CYAN,
	GREEN,
	LTGREEN,
	YELLOW,
	PINK,
	BROWN,
	RED,
	LTCYAN,
	LTBLUE,
	BLUE,
	BLACK,
	GREY,
	LTGREY,
	WHITE,
	COLOR_PADDING=0x1000
} ColorType;


#endif
