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

/* $Header: /CounterStrike/BUFFERX.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BUFFER.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/04/96                                                     *
 *                                                                                             *
 *                  Last Update : May 4, 1996 [JLB]                                            *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef BUFFERx_H
#define BUFFERx_H

#include	"wwfile.h"

/*
**	This is a transmuter interface designed to aid implementation of compression, encryption, or
**	data analysis classes. The Transmuter should be derived into a class that performs the necessary
**	processing.
*/
class Transmuter {
	public:
		Transmuter(void) : Output(0) {}
		virtual ~Transmuter(void) {}

		/*
		**	These are the interface function that are used to pass data to the transmuter. The
		**	default implementation of these functions do nothing other than pass the data onto
		**	the subsequent transmuter. For practical use, these functions should be overloaded to
		**	do something more useful.
		*/
		virtual void Attach(Transmuter * transmuter) {Output = transmuter;}
		virtual void Flush(void) {if (Output) Output->Flush();}
		virtual void Put(const void * input, unsigned length) {if (Output) Output->Put(input, length);}

	protected:

		/*
		**	Pointer to the output transmuter.
		*/
		Transmuter * Output;
};


class FileTransmuter {
	public:
		FileTransmuter(FileClass * file = NULL) : OutputFile(file) {}

		virtual void Attach(FileClass * file) {OutputFile = file;}
		virtual void Flush(void) {}
		virtual void Put(const void * input, unsigned length) {if (OutputFile) OutputFile->Write(input, length);}

	protected:
		FileClass * OutputFile;
};


class BufferTransmuter {
	public:
		BufferTransmuter(void * buffer = NULL) : BufferPtr(buffer) {}

		virtual void Attach(void * buffer) {BufferPtr = buffer;}
		virtual void Flush(void) {}
		virtual void Put(const void * input, unsigned length) {if (BufferPtr) {memcpy(BufferPtr, input, length);((char *&)BufferPtr) += length;}}

	protected:
		void * BufferPtr;
};


#endif


