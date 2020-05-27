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

/* $Header: /CounterStrike/WWFILE.H 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Westwood Library                                             *
 *                                                                                             *
 *                    File Name : WWFILE.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 8, 1994                                               *
 *                                                                                             *
 *                  Last Update : August 8, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef WWFILE_Hx
#define WWFILE_Hx

#define YEAR(dt)	(((dt & 0xFE000000) >> (9 + 16)) + 1980)
#define MONTH(dt)	 ((dt & 0x01E00000) >> (5 + 16))
#define DAY(dt)	 ((dt & 0x001F0000) >> (0 + 16))
#define HOUR(dt)	 ((dt & 0x0000F800) >> 11)
#define MINUTE(dt) ((dt & 0x000007E0) >> 5)
#define SECOND(dt) ((dt & 0x0000001F) << 1)

#include	<io.h>
#include	<stddef.h>

#ifndef READ
#define READ 1
#endif
#ifndef WRITE
#define WRITE 2
#endif

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


class FileClass
{
	public:
		virtual ~FileClass(void) {};
		virtual char const * File_Name(void) const = 0;
		virtual char const * Set_Name(char const *filename) = 0;
		virtual int Create(void) = 0;
		virtual int Delete(void) = 0;
		virtual int Is_Available(int forced=false) = 0;
		virtual int Is_Open(void) const = 0;
		virtual int Open(char const *filename, int rights=READ) = 0;
		virtual int Open(int rights=READ) = 0;
		virtual long Read(void *buffer, long size) = 0;
		virtual long Seek(long pos, int dir=SEEK_CUR) = 0;
		virtual long Size(void) = 0;
		virtual long Write(void const *buffer, long size) = 0;
		virtual void Close(void) = 0;
		virtual unsigned long Get_Date_Time(void) {return(0);}
		virtual bool Set_Date_Time(unsigned long ) {return(false);}
		virtual void Error(int error, int canretry = false, char const * filename=NULL) = 0;

		operator char const * () {return File_Name();};
};

#endif
