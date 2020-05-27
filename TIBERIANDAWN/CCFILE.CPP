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

/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CCFILE.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 8, 1994                                               *
 *                                                                                             *
 *                  Last Update : March 20, 1995 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CCFileClass::CCFileClass -- Default constructor for file object.                          *
 *   CCFileClass::CCFileClass -- Filename based constructor for C&C file.                      *
 *   CCFileClass::Close -- Closes the file.                                                    *
 *   CCFileClass::Is_Available -- Checks for existence of file on disk or in mixfile.          *
 *   CCFileClass::Is_Open -- Determines if the file is open.                                   *
 *   CCFileClass::Open -- Opens a file from either the mixfile system or the rawfile system.   *
 *   CCFileClass::Read -- Reads data from the file.                                            *
 *   CCFileClass::Seek -- Moves the current file pointer in the file.                          *
 *   CCFileClass::Size -- Determines the size of the file.                                     *
 *   CCFileClass::Write -- Writes data to the file (non mixfile files only).                   *
 *   CCFileClass::Error -- Handles displaying a file error message.                            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"
//#include	<direct.h>
//#include	<fcntl.h>
//#include	<io.h>
//#include	<dos.h>
//#include	<errno.h>
//#include	<share.h>
//#include	"ccfile.h"


/***********************************************************************************************
 * CCFileClass::Error -- Handles displaying a file error message.                              *
 *                                                                                             *
 *    Display an error message as indicated. If it is allowed to retry, then pressing a key    *
 *    will return from this function. Otherwise, it will exit the program with "exit()".       *
 *                                                                                             *
 * INPUT:   error    -- The error number (same as the DOSERR.H error numbers).                 *
 *                                                                                             *
 *          canretry -- Can this routine exit normally so that retrying can occur? If this is  *
 *                      false, then the program WILL exit in this routine.                     *
 *                                                                                             *
 *          filename -- Optional filename to report with this error. If no filename is         *
 *                      supplied, then no filename is listed in the error message.             *
 *                                                                                             *
 * OUTPUT:  none, but this routine might not return at all if the "canretry" parameter is      *
 *          false or the player pressed ESC.                                                   *
 *                                                                                             *
 * WARNINGS:   This routine may not return at all. It handles being in text mode as well as    *
 *             if in a graphic mode.                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void CCFileClass::Error(int , int , char const * )
{
#ifdef DEMO
	if (strstr(File_Name(), "\\")) {
		if (!Force_CD_Available(-1)) {
			Prog_End();
			if (!RunningAsDLL) {
				exit(EXIT_FAILURE);
			}
		}
	}

#else

	if (!Force_CD_Available(RequiredCD)) {
		Prog_End("CCFileClass::Error CD not found", true);
		if (!RunningAsDLL) {
			exit(EXIT_FAILURE);
		}
	}

#endif
}


/***********************************************************************************************
 * CCFileClass::CCFileClass -- Filename based constructor for C&C file.                        *
 *                                                                                             *
 *    Use this constructor for a file when the filename is known at construction time.         *
 *                                                                                             *
 * INPUT:   filename -- Pointer to the filename to use for this file object.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The filename pointer is presumed to be inviolate throughout the duration of     *
 *             the file object. If this is not guaranteed, then use the default constructor    *
 *             and then set the name manually.                                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
CCFileClass::CCFileClass(char const *filename) :
	CDFileClass(),
	FromDisk(false),
	Pointer(0),
	Position(0),
	Length(0),
	Start(0)
{
	Set_Name(filename);
}


/***********************************************************************************************
 * CCFileClass::CCFileClass -- Default constructor for file object.                            *
 *                                                                                             *
 *    This is the default constructor for a C&C file object.                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
CCFileClass::CCFileClass(void)
{
	FromDisk = false;
	Pointer = 0;
	Position = 0;
	Length = 0;
	Start = 0;
}


/***********************************************************************************************
 * CCFileClass::Write -- Writes data to the file (non mixfile files only).                     *
 *                                                                                             *
 *    This routine will write data to the file, but NOT to a file that is part of a mixfile.   *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that holds the data to be written.               *
 *                                                                                             *
 *          size     -- The number of bytes to write.                                          *
 *                                                                                             *
 * OUTPUT:  Returns the number of bytes actually written.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
long CCFileClass::Write(void const *buffer, long size)
{

	/*
	**	If this is part of a mixfile, then writing is not allowed. Error out with a fatal
	**	message.
	*/
	if (Pointer || FromDisk) {
		Error(EACCES, false, File_Name());
	}

	return(CDFileClass::Write(buffer, size));
}


/***********************************************************************************************
 * CCFileClass::Read -- Reads data from the file.                                              *
 *                                                                                             *
 *    This routine determines if the file is part of the mixfile system. If it is, then        *
 *    the file is copied from RAM if it is located there. Otherwise it is read from disk       *
 *    according to the correct position of the file within the parent mixfile.                 *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer to place the read data.                          *
 *                                                                                             *
 *          size     -- The number of bytes to read.                                           *
 *                                                                                             *
 * OUTPUT:  Returns the actual number of bytes read (this could be less than requested).       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
long CCFileClass::Read(void *buffer, long size)
{
	int opened = false;

	if (!Is_Open()) {
		if (Open()) {
			opened = true;
		}
	}

	/*
	**	If the file is part of a loaded mixfile, then a mere copy is
	**	all that is required for the read.
	*/
	if (Pointer) {
		long	maximum = Length - Position;

		size = MIN(maximum, size);
		if (size) {
			Mem_Copy(Add_Long_To_Pointer(Pointer, Position), buffer, size);
			Position += size;
		}
		if (opened) Close();
		return(size);
	}

	/*
	**	If the file is part of a mixfile, but the mixfile is located
	**	on disk, then a special read operation is necessary.
	*/
	if (FromDisk) {
		long	maximum = Length - Position;

		size = MIN(maximum, size);
		if (size > 0) {
			CDFileClass::Seek(Start + Position, SEEK_SET);
			size = CDFileClass::Read(buffer, size);
			Position += size;
		}
		if (opened) Close();
		return(size);
	}

	long s = CDFileClass::Read(buffer, size);
	if (opened) Close();
	return(s);
}


/***********************************************************************************************
 * CCFileClass::Seek -- Moves the current file pointer in the file.                            *
 *                                                                                             *
 *    This routine will change the current file pointer to the position specified. It follows  *
 *    the same rules the a normal Seek() does, but if the file is part of the mixfile system,  *
 *    then only the position value needs to be updated.                                        *
 *                                                                                             *
 * INPUT:   pos      -- The position to move the file to relative to the position indicated    *
 *                      by the "dir" parameter.                                                *
 *                                                                                             *
 *          dir      -- The direction to affect the position change against. This can be       *
 *                      either SEEK_CUR, SEEK_END, or SEEK_SET.                                *
 *                                                                                             *
 * OUTPUT:  Returns with the position of the new location.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
long CCFileClass::Seek(long pos, int dir)
{
	if (Pointer || FromDisk) {
		switch (dir) {
			case SEEK_END:
				Position = Length;
				break;

			case SEEK_SET:
				Position = 0;
				break;

			case SEEK_CUR:
			default:
				break;
		}
		Position += pos;
		if (Position < 0) Position = 0;
		if (Position > Length) Position = Length;
		return(Position);
	}
	return(CDFileClass::Seek(pos, dir));
}


/***********************************************************************************************
 * CCFileClass::Size -- Determines the size of the file.                                       *
 *                                                                                             *
 *    If the file is part of the mixfile system, then the size of the file is already          *
 *    determined and available. Otherwise, go to the low level system to find the file         *
 *    size.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the size of the file in bytes.                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
long CCFileClass::Size(void)
{
	if (Pointer || FromDisk) return(Length);

	return(CDFileClass::Size());
}


/***********************************************************************************************
 * CCFileClass::Is_Available -- Checks for existence of file on disk or in mixfile.            *
 *                                                                                             *
 *    This routine will examine the mixfile system looking for the file. If the file could     *
 *    not be found there, then the disk is examined directly.                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the file available for opening?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int CCFileClass::Is_Available(int )
{
	if (MixFileClass::Offset(File_Name())) {
		return(true);
	}
	return(CDFileClass::Is_Available());
}


/***********************************************************************************************
 * CCFileClass::Is_Open -- Determines if the file is open.                                     *
 *                                                                                             *
 *    A mixfile is open if there is a pointer to the mixfile data. In absence of this,         *
 *    the the file is open if the file handle is valid.                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the file open?                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int CCFileClass::Is_Open(void) const
{

	/*
	**	If the file is part of a cached file, then return that it is opened. A closed file
	**	doesn't have a valid pointer.
	*/
	if (Pointer) return(true);
	return(CDFileClass::Is_Open());
}


/***********************************************************************************************
 * CCFileClass::Close -- Closes the file.                                                      *
 *                                                                                             *
 *    If this is a mixfile file, then only the pointers need to be adjusted.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void CCFileClass::Close(void)
{
	FromDisk = false;
	Pointer = 0;
	Position = 0;				// Starts at beginning offset.
	Start = 0;
	Length = 0;
	CDFileClass::Close();
}


/***********************************************************************************************
 * CCFileClass::Open -- Opens a file from either the mixfile system or the rawfile system.     *
 *                                                                                             *
 *    This routine will open the specified file. It examines the mixfile system to find a      *
 *    match. If one is found then the file is "opened" in a special cached way. Otherwise      *
 *    it is opened as a standard DOS file.                                                     *
 *                                                                                             *
 * INPUT:   rights   -- The access rights desired.                                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the file opened successfully?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int CCFileClass::Open(int rights)
{
	/*
	**	Always close the file if it was open.
	*/
	Close();

	/*
	**	Perform a preliminary check to see if the specified file
	**	exists on the disk. If it does, then open this file regardless
	**	of whether it also exists in RAM. This is slower, but allows
	**	upgrade files to work.
	*/
	if ((rights & WRITE) || CDFileClass::Is_Available()) {
		return(CDFileClass::Open(rights));
	}

	/*
	**	Check to see if file is part of a mixfile and that mixfile is currently loaded
	**	into RAM.
	*/
	MixFileClass *mixfile = 0;
	if (MixFileClass::Offset(File_Name(), &Pointer, &mixfile, &Start, &Length)) {

		/*
		**	If the mixfile is located on disk, then fake out the file system to read from
		**	the mixfile, but think it is reading from a solitary file.
		*/
		if (!Pointer) {
			long	start = Start;
			long	length = Length;

			/*
			**	This is a legitimate open to the file. All access to the file through this
			**	file object will be appropriately adjusted for mixfile support however. Also
			**	note that the filename attached to this object is NOT the same as the file
			**	attached to the file handle.
			*/
			char const * dupfile = strdup(File_Name());
			Open(mixfile->Filename, READ);
			Searching(false);				// Disable multi-drive search.
			Set_Name(dupfile);
			Searching(true);
			if (dupfile) free((void *)dupfile);
			Start = start;
			Length = length;
			FromDisk = true;
		}

	} else {

		/*
		**	The file cannot be found in any mixfile, so it must reside as
		** an individual file on the disk. Or else it is just plain missing.
		*/
		return(CDFileClass::Open(rights));
	}
	return(true);
}


/***********************************************************************************
** Backward compatibility section.
*/
//extern "C" {

static CCFileClass Handles[10];

#ifdef NEVER
bool __cdecl Set_Search_Drives(char const *)
{
	CCFileClass::Set_Search_Path(path);
	return(true);
}
#endif

int __cdecl Open_File(char const *file_name, int mode)
{
	for (int index = 0; index < sizeof(Handles)/sizeof(Handles[0]); index++) {
		if (!Handles[index].Is_Open()) {
			Handles[index].Set_Name(file_name);
			if (Handles[index].Open(mode)) {
//			if (Handles[index].Open(file_name, mode)) {
				return(index);
			}
			break;
		}
	}
	return(WW_ERROR);
}

VOID __cdecl Close_File(int handle)
{
	if (handle != WW_ERROR && Handles[handle].Is_Open()) {
		Handles[handle].Close();
	}
}

LONG __cdecl Read_File(int handle, VOID *buf, ULONG bytes)
{
	if (handle != WW_ERROR && Handles[handle].Is_Open()) {
		return(Handles[handle].Read(buf, bytes));
	}
	return(0);
}

LONG __cdecl Write_File(int handle, VOID const *buf, ULONG bytes)
{
	if (handle != WW_ERROR && Handles[handle].Is_Open()) {
		return(Handles[handle].Write(buf, bytes));
	}
	return(0);
}

int __cdecl Find_File(char const *file_name)
{
	CCFileClass file(file_name);
	return(file.Is_Available());
}

#ifdef NEVER
int __cdecl Delete_File(char const *file_name)
{
	return(CCFileClass(file_name).Delete());
}

int __cdecl Create_File(char const *file_name)
{
	return(CCFileClass(file_name).Create());
}

ULONG __cdecl Load_Data(char const *name, VOID *ptr, ULONG size)
{
	return(CCFileClass(name).Read(ptr, size));
}
#endif

VOID * __cdecl Load_Alloc_Data(char const *name, int )
{
	CCFileClass file(name);

	return(Load_Alloc_Data(file));
}

ULONG __cdecl File_Size(int handle)
{
	if (handle != WW_ERROR && Handles[handle].Is_Open()) {
		return(Handles[handle].Size());
	}
	return(0);
}

#ifdef NEVER
ULONG __cdecl Write_Data(char const *name, VOID const *ptr, ULONG size)
{
	return(CCFileClass(name).Write(ptr, size));
}
#endif

ULONG __cdecl Seek_File(int handle, LONG offset, int starting)
{
	if (handle != WW_ERROR && Handles[handle].Is_Open()) {
		return(Handles[handle].Seek(offset, starting));
	}
	return(0);
}

void WWDOS_Shutdown(void)
{
	for (int index = 0; index < 10; index++) {
		Handles[index].Set_Name(NULL);
	}
}

#ifdef NEVER
bool __cdecl Multi_Drive_Search(bool on)
{
//	return(CCFileClass::Multi_Drive_Search(on));
	return(on);
}

VOID __cdecl WWDOS_Init(VOID)
{
}

VOID __cdecl WWDOS_Shutdown(VOID)
{
}

int __cdecl Find_Disk_Number(char const *)
{
	return(0);
}
#endif

//ULONG cdecl Load_Uncompress(BYTE const *file, BuffType uncomp_buff, BuffType dest_buff, VOID *reserved_data)
//{
//	return(Load_Uncompress(CCFileClass(file), uncomp_buff, dest_buff, reserved_data));
//	return(CCFileClass(file).Load_Uncompress(uncomp_buff, dest_buff, reserved_data));
//}

//extern "C" {
//int MaxDevice;
//int DefaultDrive;
//char CallingDOSInt;

//}


void Unfragment_File_Cache(void)
{
}