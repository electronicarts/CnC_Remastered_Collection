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

/* $Header:   F:\projects\c&c\vcs\code\ccfile.h_v   2.18   16 Oct 1995 16:45:28   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CCFILE.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : October 17, 1994                                             *
 *                                                                                             *
 *                  Last Update : October 17, 1994   [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CCFILE_H
#define CCFILE_H

#include	<wwlib32.h>
#include	<limits.h>
#include	"mixfile.h"
#include	"cdfile.h"


/*
**	This derived class for file access knows about mixfiles (packed files). It can handle opening
**	a file that is embedded within a mixfile. This is true if the mixfile is cached or resides on
**	disk. It is functionally similar to pakfiles, except much faster and less RAM intensive.
*/
class CCFileClass : public CDFileClass
{
	public:
		CCFileClass(char const *filename);
		CCFileClass(void);
		virtual ~CCFileClass(void) {};

		// Delete should be overloaded here as well. Don't allow deletes of mixfiles.
		
		virtual int Open(char const *filename, int rights=READ) {Set_Name(filename);return Open(rights);};
		virtual int Open(int rights=READ);
		virtual int Is_Open(void) const;
		virtual int Is_Available(int forced=false);
		virtual long Read(void *buffer, long size);
		virtual long Seek(long pos, int dir=SEEK_CUR);
		virtual long Size(void);
		virtual long Write(void const *buffer, long size);
		virtual void Close(void);
		virtual void Error(int error, int canretry = false, char const * filename=NULL);

	private:

		/*
		**	This flag indicates that the file is part of a mixfile and the mixfile resides on
		**	disk. The file handle for this file is a legitimate DOS handle, although special
		**	handling is necessary that takes into account the embedded nature of the file.
		*/
		bool FromDisk;

		/*
		**	This indicates the file is actually part of a resident image of the mixfile
		**	itself. In this case, the embedded file handle is invalid. All file access actually
		**	gets routed through the cached version of the file. This is a pointer to the start
		**	of the RAM image of the file.
		*/
		void * Pointer;

		/*
		**	This is the starting offset of the beginning of the file. This value is only valid
		**	if the file is part of a mixfile that resides on disk. It serves as the counterpart
		**	to the "Pointer" variable.
		*/
		long Start;

		/*
		**	This is the current seek position of the file. It is duplicated here if the file is
		**	part of a mixfile since the DOS seek position is not accurate. This value will
		**	range from zero to the size of the file in bytes.
		*/
		long Position;

		/*
		**	This is the size of the file if it was embedded in a mixfile. The size must be manually
		**	kept track of because the DOS file size is invalid.
		*/
		long Length;

		// Force these to never be invoked.
		CCFileClass const operator = (CCFileClass const & c);
		CCFileClass (CCFileClass const & ) {};
};

#endif
