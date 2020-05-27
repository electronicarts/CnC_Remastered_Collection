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

/* $Header: /CounterStrike/BUFF.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BUFF.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/29/96                                                     *
 *                                                                                             *
 *                  Last Update : July 29, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef CCBUFF_H
#define CCBUFF_H


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

/*
**	A general purpose buffer pointer handler object. It holds not only the pointer to the
**	buffer, but its size as well. By using this class instead of separate pointer and size
**	values, function interfaces and algorithms become simpler to manage and understand.
*/
class Buffer {
	public:
		Buffer(char * ptr, long size=0);
		Buffer(void * ptr=0, long size=0);
		Buffer(void const * ptr, long size=0);
		Buffer(long size);
		Buffer(Buffer const & buffer);
		~Buffer(void);

		Buffer & operator = (Buffer const & buffer);
		operator void * (void) const {return(BufferPtr);}
		operator char * (void) const {return((char *)BufferPtr);}

		void Reset(void);
		void * Get_Buffer(void) const {return(BufferPtr);}
		long Get_Size(void) const {return(Size);}
		bool Is_Valid(void) const {return(BufferPtr != 0);}

	protected:

		/*
		**	Pointer to the buffer memory.
		*/
		void * BufferPtr;

		/*
		**	The size of the buffer memory.
		*/
		long Size;

		/*
		**	Was the buffer allocated by this class? If so, then this class
		**	will be responsible for freeing the buffer.
		*/
		bool IsAllocated;
};


#endif
