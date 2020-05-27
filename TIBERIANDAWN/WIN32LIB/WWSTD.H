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



#include <windows.h>
#include <windowsx.h>

#ifndef IBM
#define IBM				TRUE
#endif

#define WW_ERROR		-1

#define PRIVATE		static
#define PUBLIC			/* Routines & data don't have a specifier */

#ifdef __cplusplus
#define	__CPPARGS	...
#else
#define	__CPPARGS
#endif



#ifdef GET_SIZE
#undef GET_SIZE
#endif
#define GET_SIZE(a)					((sizeof(a) / sizeof(*a)))

//PG_TO_FIX
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

typedef enum {
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



#endif	//WWSTD_H