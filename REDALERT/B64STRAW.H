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

/* $Header: /CounterStrike/B64STRAW.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : B64STRAW.H                                                   *
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


#ifndef B64STRAW_H
#define B64STRAW_H

#include	"straw.h"

/*
**	Performs Base 64 encoding/decoding on the data that is drawn through the straw. Note that
**	encoding increases the data size by about 30%. The reverse occurs when decoding.
*/
class Base64Straw : public Straw
{
	public:
		typedef enum CodeControl {
			ENCODE,
			DECODE
		} CodeControl;

		Base64Straw(CodeControl control) : Control(control), Counter(0) {}
		virtual int Get(void * source, int slen);

	private:

		/*
		**	Indicates if this is for encoding or decoding of Base64 data.
		*/
		CodeControl Control;

		/*
		**	The counter of the number of accumulated bytes pending for processing.
		*/
		int Counter;

		/*
		**	Buffer that holds the Base64 coded bytes. This will be the staging buffer if
		**	this is for a decoding process. Otherwise, it will be used as a scratch buffer.
		*/
		char CBuffer[4];

		/*
		**	Buffer that holds the plain bytes. This will be the staging buffer if this
		**	is for an encoding process. Otherwise, it will be used as a scratch buffer.
		*/
		char PBuffer[3];

		/*
		**	Explicitly disable the copy constructor and the assignment operator.
		*/
		Base64Straw(Base64Straw & rvalue);
		Base64Straw & operator = (Base64Straw const & pipe);
};


#endif
