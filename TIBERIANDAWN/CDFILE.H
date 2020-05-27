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

/* $Header:   F:\projects\c&c\vcs\code\cdfile.h_v   2.20   16 Oct 1995 16:45:58   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Westwood LIbrary                                             *
 *                                                                                             *
 *                    File Name : CDFILE.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : October 18, 1994                                             *
 *                                                                                             *
 *                  Last Update : October 18, 1994   [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CDFILE_H
#define CDFILE_H

#include	<dos.h>
#include	"rawfile.h"

/*
**	This class is derived from the raw file class. This class adds the functionality of searching
**	across multiple directories or drives. It is designed for the typical case of a CD-ROM game
**	were some data exists in the current directory (hard drive) and the rest exists on the CD-ROM.
**	Searching for the file occurs by first examining the current directory. If the file does not
**	exist there, then all the paths available are examined in turn until the file can be found.
**	For opening files to write, only the current directory is examined. The directory search order
**	is controlled by the path list as submitted to Set_Search_Drives(). The format of the path
**	string is the same as the DOS path string.
*/
class CDFileClass : public RawFileClass
{
	public:
		CDFileClass(char const *filename);
		CDFileClass(void);
		virtual ~CDFileClass(void) {};

		virtual char const * Set_Name(char const *filename);
		virtual int Open(char const *filename, int rights=READ);
		virtual int Open(int rights=READ);

		void Searching(int on) {IsDisabled = !on;};

		static bool Is_There_Search_Drives(void) {return(First != NULL);};
		static int Set_Search_Drives(char * pathlist);
		static void Add_Search_Drive(char *path);
		static void Clear_Search_Drives(void);
		static void Refresh_Search_Drives(void);
		static void Set_CD_Drive(int drive);
		static int  Get_CD_Drive(void) {return(CurrentCDDrive);};
		static int  Get_Last_CD_Drive(void) {return(LastCDDrive);};

		// RawPath will overflow if we keep setting the path. ST - 1/23/2019 11:12AM
		static void Reset_Raw_Path(void) {RawPath[0] = 0;}

		// Need to access the paths. ST - 3/15/2019 2:14PM
		static const char *Get_Search_Path(int index);

	private:

		/*
		**	Is multi-drive searching disabled for this file object?
		*/
		unsigned IsDisabled:1;

		/*
		**	This is the control record for each of the drives specified in the search
		**	path. There can be many such search paths available.
		*/
		typedef struct {
			void * Next;		// Pointer to next search record.
			char const * Path;			// Pointer to path string.
		} SearchDriveType;

		/*
		**	This points to the first path record.
		*/
		static SearchDriveType * First;

		/*
		** This is a copy of the unparsed search path list
		*/
		static char RawPath[512];

		/*
		** The drive letter of the current cd drive
		*/
		static int CurrentCDDrive;

		/*
		** The drive letter of the last used CD drive
		*/
		static int LastCDDrive;
};


#endif