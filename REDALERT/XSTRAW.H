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

/* $Header: /CounterStrike/XSTRAW.H 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : XSTRAW.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/04/96                                                     *
 *                                                                                             *
 *                  Last Update : July 4, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef XSTRAW_H
#define XSTRAW_H

#include	"straw.h"
#include	"buff.h"
#include	"wwfile.h"
#include	<stddef.h>

/*
**	This class is used to manage a buffer as a data source. Data requests will draw from the
**	buffer supplied until the buffer is exhausted.
*/
class BufferStraw : public Straw
{
	public:
		BufferStraw(Buffer const & buffer) : BufferPtr(buffer), Index(0) {}
		BufferStraw(void const * buffer, int length) : BufferPtr((void*)buffer, length), Index(0) {}
		virtual int Get(void * source, int slen);

	private:
		Buffer BufferPtr;
		int Index;
//		void const * BufferPtr;
//		int Length;

		bool Is_Valid(void) {return(BufferPtr.Is_Valid());}
		BufferStraw(BufferStraw & rvalue);
		BufferStraw & operator = (BufferStraw const & pipe);
};

/*
**	This class is used to manage a file as a data source. Data requests will draw from the
**	file until the file has been completely read.
*/
class FileStraw : public Straw
{
	public:
		FileStraw(FileClass * file) : File(file), HasOpened(false) {}
		FileStraw(FileClass & file) : File(&file), HasOpened(false) {}
		virtual ~FileStraw(void);
		virtual int Get(void * source, int slen);

	private:
		FileClass * File;
		bool HasOpened;

		bool Valid_File(void) {return(File != NULL);}
		FileStraw(FileStraw & rvalue);
		FileStraw & operator = (FileStraw const & pipe);
};


#endif
