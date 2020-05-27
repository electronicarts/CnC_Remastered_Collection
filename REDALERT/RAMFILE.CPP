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

/* $Header: /CounterStrike/RAMFILE.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RAMFILE.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/30/96                                                     *
 *                                                                                             *
 *                  Last Update : July 3, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   RAMFileClass::Close -- This will 'close' the ram file.                                    *
 *   RAMFileClass::Create -- Effectively clears the buffer of data.                            *
 *   RAMFileClass::Delete -- Effectively clears the buffer of data.                            *
 *   RAMFileClass::Is_Available -- Determines if the "file" is available.                      *
 *   RAMFileClass::Is_Open -- Is the file open?                                                *
 *   RAMFileClass::Open -- Opens a RAM based file for read or write.                           *
 *   RAMFileClass::Open -- Opens the RAM based file.                                           *
 *   RAMFileClass::RAMFileClass -- Construct a RAM buffer based "file" object.                 *
 *   RAMFileClass::Read -- Read data from the file.                                            *
 *   RAMFileClass::Seek -- Controls the ram file virtual read position.                        *
 *   RAMFileClass::Size -- Returns with the size of the ram file.                              *
 *   RAMFileClass::Write -- Copies data to the ram file.                                       *
 *   RAMFileClass::~RAMFileClass -- Destructor for the RAM file class.                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "FUNCTION.H"
#include "ramfile.h"
#include	<string.h>


/***********************************************************************************************
 * RAMFileClass::RAMFileClass -- Construct a RAM buffer based "file" object.                   *
 *                                                                                             *
 *    This routine will construct a "file" object that actually is just a front end processor  *
 *    for a buffer. Access to the buffer will appear as if it was accessing a file. This       *
 *    is different from the caching ability of the buffered file class in that this file       *
 *    class has no real file counterpart. Typical use of this is for algorithms that were      *
 *    originally designed for file processing, but are now desired to work with a buffer.      *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer to use for this file. The buffer will already    *
 *                      contain data if the file is opened for READ. It will be considered     *
 *                      a scratch buffer if opened for WRITE. If the buffer pointer is NULL    *
 *                      but the length parameter is not, then a buffer will be allocated       *
 *                      of the specified length. This case is only useful for opening the      *
 *                      file for WRITE.                                                        *
 *                                                                                             *
 *          length   -- The length of the buffer submitted to this routine.                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
RAMFileClass::RAMFileClass(void * buffer, int len) :
	Buffer((char *)buffer),
	MaxLength(len),
	Length(len),
	Offset(0),
	Access(READ),
	IsOpen(false),
	IsAllocated(false)
{
	if (buffer == NULL && len > 0) {
		Buffer = new char[len];
		IsAllocated = true;
	}
}


/***********************************************************************************************
 * RAMFileClass::~RAMFileClass -- Destructor for the RAM file class.                           *
 *                                                                                             *
 *    The destructor will deallocate any buffer that it allocated. Otherwise it does nothing.  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
RAMFileClass::~RAMFileClass(void)
{
	Close();
	if (IsAllocated) {
		delete [] Buffer;
		Buffer = NULL;
		IsAllocated = false;
	}
}


/***********************************************************************************************
 * RAMFileClass::Create -- Effectively clears the buffer of data.                              *
 *                                                                                             *
 *    This routine "clears" the buffer of data. It only makes the buffer appear empty by       *
 *    resetting the internal length to zero.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the file reset in this fashion?                                                *
 *                                                                                             *
 * WARNINGS:   If the file was open, then resetting by this routine is not allowed.            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int RAMFileClass::Create(void)
{
	if (!Is_Open()) {
		Length = 0;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * RAMFileClass::Delete -- Effectively clears the buffer of data.                              *
 *                                                                                             *
 *    This routine "clears" the buffer of data. It only makes the buffer appear empty by       *
 *    resetting the internal length to zero.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the file reset in this fashion?                                                *
 *                                                                                             *
 * WARNINGS:   If the file was open, then resetting by this routine is not allowed.            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int RAMFileClass::Delete(void)
{
	if (!Is_Open()) {
		Length = 0;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * RAMFileClass::Is_Available -- Determines if the "file" is available.                        *
 *                                                                                             *
 *    RAM files are always available.                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  TRUE                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int RAMFileClass::Is_Available(int )
{
	return(true);
}


/***********************************************************************************************
 * RAMFileClass::Is_Open -- Is the file open?                                                  *
 *                                                                                             *
 *    This answers the question whether the file is open or not.                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the file open?                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int RAMFileClass::Is_Open(void) const
{
	return(IsOpen);
}


/***********************************************************************************************
 * RAMFileClass::Open -- Opens a RAM based file for read or write.                             *
 *                                                                                             *
 *    This routine will open the ram file. The name is meaningless so that parameter is        *
 *    ignored. If the access mode is for write, then the pseudo-file can be written until the  *
 *    buffer is full. If the file is opened for read, then the buffer is presumed to be full   *
 *    of the data to be read.                                                                  *
 *                                                                                             *
 * INPUT:   name  -- ignored.                                                                  *
 *                                                                                             *
 *          access-- The access method to use for the data buffer -- either READ or WRITE.     *
 *                                                                                             *
 * OUTPUT:  bool; Was the open successful?                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int RAMFileClass::Open(char const * , int access)
{
	return(Open(access));
}


/***********************************************************************************************
 * RAMFileClass::Open -- Opens the RAM based file.                                             *
 *                                                                                             *
 *    This will open the ram based file for read or write. If the file is opened for write,    *
 *    the the 'file' can be written up to the limit of the buffer's size. If the file is       *
 *    opened for read, then the buffer is presumed to hold the data to be read.                *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int RAMFileClass::Open(int access)
{
	if (Buffer == NULL || Is_Open()) {
		return(false);
	}

	Offset = 0;
	Access = access;
	IsOpen = true;

	switch (access) {
		default:
		case READ:
			break;

		case WRITE:
			Length = 0;
			break;

		case READ|WRITE:
			break;
	}

	return(Is_Open());
}


/***********************************************************************************************
 * RAMFileClass::Read -- Read data from the file.                                              *
 *                                                                                             *
 *    Use this routine just like a normal file read. It will copy the bytes from the ram       *
 *    buffer to the destination specified. When the ram buffer is exhausted, less bytes than   *
 *    requested will be read.                                                                  *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer to store the data to.                            *
 *                                                                                             *
 *          size     -- The number of bytes to 'read' into the specified buffer.               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes copied to the destination buffer. If the number   *
 *          of bytes returned is less than requested, then this indicates that the source      *
 *          buffer is exhausted.                                                               *
 *                                                                                             *
 * WARNINGS:   The read function only applies to ram 'files' opened for read access.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
long RAMFileClass::Read(void * buffer, long size)
{
	if (Buffer == NULL || buffer == NULL || size == 0) {
		return(0);
	}

	bool hasopened = false;
	if (!Is_Open()) {
		Open(READ);
		hasopened = true;
	} else {
		if ((Access & READ) == 0) {
			return(0);
		}
	}

	int tocopy = (size < (Length-Offset)) ? size : (Length-Offset);
	memmove(buffer, &Buffer[Offset], tocopy);
	Offset += tocopy;

	if (hasopened) {
		Close();
	}

	return(tocopy);
}


/***********************************************************************************************
 * RAMFileClass::Seek -- Controls the ram file virtual read position.                          *
 *                                                                                             *
 *    This routine will move the read/write position of the ram file to the location specified *
 *    by the offset and direction parameters. It functions similarly to the regular file       *
 *    seek method.                                                                             *
 *                                                                                             *
 * INPUT:   pos   -- The signed offset from the home position specified by the "dir"           *
 *                   parameter.                                                                *
 *                                                                                             *
 *          dir   -- The home position to base the position offset on. This will either be     *
 *                   the start of the file, the end of the file, or the current read/write     *
 *                   position.                                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the new file position.                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
long RAMFileClass::Seek(long pos, int dir)
{
	if (Buffer == NULL || !Is_Open()) {
		return(Offset);
	}

	int maxoffset = Length;
	if ((Access & WRITE) != 0) {
		maxoffset = MaxLength;
	}

	switch (dir) {
		case SEEK_CUR:
			Offset += pos;
			break;

		case SEEK_SET:
			Offset = 0 + pos;
			break;

		case SEEK_END:
			Offset = maxoffset + pos;
			break;
	}

	if (Offset < 0) Offset = 0;
	if (Offset > maxoffset) Offset = maxoffset;

	if (Offset > Length) {
		Length = Offset;
	}

	return(Offset);
}


/***********************************************************************************************
 * RAMFileClass::Size -- Returns with the size of the ram file.                                *
 *                                                                                             *
 *    This will return the size of the 'real' data in the ram file. The real data is either    *
 *    the entire buffer, if opened for READ, or just the written data if opened for WRITE.     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes that the ram file system considers to be valid    *
 *          data of the 'file'.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
long RAMFileClass::Size(void)
{
	return(Length);
}


/***********************************************************************************************
 * RAMFileClass::Write -- Copies data to the ram file.                                         *
 *                                                                                             *
 *    This function similarly to the regular write operation supported for files. It copies    *
 *    the data specified to the current write position in the ram file.                        *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the data to be written.                                     *
 *                                                                                             *
 *          size     -- The number of bytes to write to the file.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes written. This will be less than requested  *
 *          if the buffer is exhausted of space prematurely.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
long RAMFileClass::Write(void const * buffer, long size)
{
	if (Buffer == NULL || buffer == NULL || size == 0) {
		return(0);
	}

	bool hasopened = false;
	if (!Is_Open()) {
		Open(WRITE);
		hasopened = true;
	} else {
		if ((Access & WRITE) == 0) {
			return(0);
		}
	}

	int maxwrite = MaxLength - Offset;
	int towrite = (size < maxwrite) ? size : maxwrite;
	memmove(&Buffer[Offset], buffer, towrite);
	Offset += towrite;

	if (Offset > Length) {
		Length = Offset;
	}

	if (hasopened) {
		Close();
	}

	return(towrite);
}


/***********************************************************************************************
 * RAMFileClass::Close -- This will 'close' the ram file.                                      *
 *                                                                                             *
 *    Closing a ram file actually does nothing but record that it is now closed.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void RAMFileClass::Close(void)
{
	IsOpen = false;
}
