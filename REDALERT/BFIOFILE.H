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

/* $Header: /CounterStrike/BFIOFILE.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Westwood Library                                             *
 *                                                                                             *
 *                    File Name : BFIOFILE.H                                                   *
 *                                                                                             *
 *                   Programmer : David R. Dettmer                                             *
 *                                                                                             *
 *                   Start Date : November 10, 1995                                            *
 *                                                                                             *
 *                  Last Update : November 10, 1995  [DRD]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef BFIOFILE_H
#define BFIOFILE_H

#include	"rawfile.h"

/*
**	This derivation of the raw file class handles buffering the input/output in order to
**	achieve greater speed. The buffering is not active by default. It must be activated
**	by setting the appropriate buffer through the Cache() function.
*/
class BufferIOFileClass : public RawFileClass
{
	public:

		BufferIOFileClass(char const *filename);
		BufferIOFileClass(void);
		virtual ~BufferIOFileClass(void);

		bool Cache( long size=0, void *ptr=NULL );
		void Free( void );
		bool Commit( void );
		virtual char const * Set_Name(char const *filename);
		virtual int Is_Available(int forced=false);
		virtual int Is_Open(void) const;
		virtual int Open(char const *filename, int rights=READ);
		virtual int Open(int rights=READ);
		virtual long Read(void *buffer, long size);
		virtual long Seek(long pos, int dir=SEEK_CUR);
		virtual long Size(void);
		virtual long Write(void const *buffer, long size);
		virtual void Close(void);

		enum {MINIMUM_BUFFER_SIZE=1024};

	private:

		unsigned IsAllocated:1;
		unsigned IsOpen:1;
		unsigned IsDiskOpen:1;
		unsigned IsCached:1;
		unsigned IsChanged:1;
		unsigned UseBuffer:1;

		int BufferRights;

		void *Buffer;

		long BufferSize;
		long BufferPos;
		long BufferFilePos;
		long BufferChangeBeg;
		long BufferChangeEnd;
		long FileSize;
		long FilePos;
		long TrueFileStart;
};

#endif
