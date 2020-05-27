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

/* $Header: /CounterStrike/XPIPE.H 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : XPIPE.H                                                      *
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

#ifndef XPIPE_H
#define XPIPE_H

#include	"pipe.h"
#include	"wwfile.h"
#include	"buff.h"

/*
**	This is a simple store-into-buffer pipe terminator. Use it as the final link in a pipe process
**	that needs to store the data into a memory buffer. This can only serve as the final
**	link in the chain of pipe segments.
*/
class BufferPipe : public Pipe
{
	public:
		BufferPipe(Buffer const & buffer) : BufferPtr(buffer), Index(0) {}
		BufferPipe(void * buffer, int length) : BufferPtr(buffer, length), Index(0) {}
		virtual int Put(void const * source, int slen);

	private:
		Buffer BufferPtr;
		int Index;

//		void * Buffer;
//		int Length;

		bool Is_Valid(void) {return(BufferPtr.Is_Valid());}
		BufferPipe(BufferPipe & rvalue);
		BufferPipe & operator = (BufferPipe const & pipe);
};


/*
**	This is a store-to-file pipe terminator. Use it as the final link in a pipe process that
**	needs to store the data to a file. This can only serve as the last link in the chain
**	of pipe segments.
*/
class FilePipe : public Pipe
{
	public:
		FilePipe(FileClass * file) : File(file), HasOpened(false) {}
		FilePipe(FileClass & file) : File(&file), HasOpened(false) {}
		virtual ~FilePipe(void);

		virtual int Put(void const * source, int slen);
		virtual int End(void);

	private:
		FileClass * File;
		bool HasOpened;

		bool Valid_File(void) {return(File != NULL);}
		FilePipe(FilePipe & rvalue);
		FilePipe & operator = (FilePipe const & pipe);

};

#endif
