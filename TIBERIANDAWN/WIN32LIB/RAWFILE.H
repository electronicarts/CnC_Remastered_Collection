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

/* $Header:   F:\projects\c&c\vcs\code\rawfile.h_v   2.15   06 Sep 1995 16:29:30   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Westwood Library                                             *
 *                                                                                             *
 *                    File Name : RAWFILE.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 8, 1994                                               *
 *                                                                                             *
 *                  Last Update : October 18, 1994   [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   RawFileClass::File_Name -- Returns with the filename associate with the file object.      *
 *   RawFileClass::RawFileClass -- Default constructor for a file object.                      *
 *   RawFileClass::Is_Open -- Checks to see if the file is open or not.                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef RAWFILE_H
#define RAWFILE_H

#ifndef WIN32
#define WIN32 1
#ifndef _WIN32 // Denzil 6/2/98 Watcom 11.0 complains without this check
#define _WIN32
#endif // _WIN32
#endif
#include <windows.h>

//#include	<wwlib32.h>
#include	<limits.h>
#include	<errno.h>
#include	<windows.h>
//#include	<algo.h>
#include	"wwfile.h"

#ifdef NEVER
	/*
	**	This is a duplicate of the error numbers. The error handler for the RawFileClass handles
	**	these errors. If the error routine is overridden and additional errors are defined, then
	**	use numbers starting with 100. Note that these errors here are listed in numerical order.
	**	These errors are defined in the standard header file "ERRNO.H".
	*/
	EZERO,				// Non-error.
	EINVFNC,				// Invalid function number.
	ENOFILE,				// File not found.
	ENOENT=ENOFILE,	// No such file or directory.
	ENOPATH,				// Path not found.
	EMFILE,				// Too many open files.
	EACCES,				// Permission denied.
	EBADF,				// Bad file number.
	ECONTR,				// Memory blocks destroyed.
	ENOMEM,				// Not enough core memory.
	EINVMEM,				// Invalid memory block address.
	EINVENV,				// Invalid environment.
	EINVFMT,				// Invalid format.
	EINVACC,				// Invalid access code.
	EINVDAT,				// Invalid data.
	EFAULT,				// Unknown error.
	EINVDRV,				// Invalid drive specified.
	ENODEV=EINVDRV,	// No such device.
	ECURDIR,				// Attempt to remove CurDir.
	ENOTSAM,				// Not same device.
	ENMFILE,				// No more files.
	EINVAL,				// Invalid argument.
	E2BIG,				// Argument list too long.
	ENOEXEC,				// exec format error.
	EXDEV,				// Cross-device link.
	ENFILE,				// Too many open files.
	ECHILD,				// No child process.
	ENOTTY,				// not used
	ETXTBSY,				// not used
	EFBIG,				// not used
	ENOSPC,				// No space left on device.
	ESPIPE,				// Illegal seek.
	EROFS,				// Read-only file system.
	EMLINK,				// not used
	EPIPE,				// Broken pipe.
	EDOM,					// Math argument.
	ERANGE,				// Result too large.
	EEXIST,				// File already exists.
	EDEADLOCK,			// Locking violation.
	EPERM,				// Operation not permitted.
	ESRCH,				// not used
	EINTR,				// Interrupted function call.
	EIO,					// Input/output error.
	ENXIO,				// No such device or address.
	EAGAIN,				// Resource temporarily unavailable.
	ENOTBLK,				// not used
	EBUSY,				// Resource busy.
	ENOTDIR,				// not used
	EISDIR,				// not used
	EUCLEAN,				// not used
#endif

/*
**	This is the definition of the raw file class. It is derived from the abstract base FileClass
**	and handles the interface to the low level DOS routines. This is the first class in the
**	chain of derived file classes that actually performs a useful function. With this class,
**	I/O is possible. More sophisticated features, such as packed files, CD-ROM support,
**	file caching, and XMS/EMS memory support, are handled by derived classes.
**
**	Of particular importance is the need to override the error routine if more sophisticated
**	error handling is required. This is more than likely if greater functionality is derived
**	from this base class.
*/
class RawFileClass : public FileClass
{
	public:

		/*
		**	This is a record of the access rights used to open the file. These rights are
		**	used if the file object is duplicated.
		*/
		int Rights;

		RawFileClass(char const *filename);
		RawFileClass(void);
		RawFileClass (RawFileClass const & f);
		RawFileClass & operator = (RawFileClass const & f);
		virtual ~RawFileClass(void) {if (Allocated && Filename) free((char *)Filename);};

		virtual char const * File_Name(void) const;
		virtual char const * Set_Name(char const *filename);
		virtual int Create(void);
		virtual int Delete(void);
		virtual int Is_Available(int forced=false);
		virtual int Is_Open(void) const;
		virtual int Open(char const *filename, int rights=READ);
		virtual int Open(int rights=READ);
		virtual long Read(void *buffer, long size);
		virtual long Seek(long pos, int dir=SEEK_CUR);
		virtual long Size(void);
		virtual long Write(void const *buffer, long size);
		virtual void Close(void);
		virtual void Error(int error, int canretry = false, char const * filename=NULL);
		virtual void Set_Buffer_Size(int size);


	protected:

		/*
		**	This function returns the largest size a low level DOS read or write may
		**	perform. Larger file transfers are performed in chunks of this size or less.
		*/
		long Transfer_Block_Size(void) {return (long)((unsigned)UINT_MAX)-16L;};

	private:

		/*
		**	This is the low level DOS handle. A -1 indicates an empty condition.
		*/
		int Handle;

		/*
		**	This points to the filename as a NULL terminated string. It may point to either a
		**	constant or an allocated string as indicated by the "Allocated" flag.
		*/
		char const *Filename;

		/*
		**	Filenames that were assigned as part of the construction process
		**	are not allocated. It is assumed that the filename string is a
		**	constant in that case and thus making duplication unnecessary.
		**	This value will be non-zero if the filename has be allocated
		**	(using strdup()).
		*/
		unsigned Allocated:1;
};


/***********************************************************************************************
 * RawFileClass::File_Name -- Returns with the filename associate with the file object.        *
 *                                                                                             *
 *    Use this routine to determine what filename is associated with this file object. If no   *
 *    filename has yet been assigned, then this routing will return NULL.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the file name associated with this file object or NULL   *
 *          if one doesn't exist.                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
;*   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
inline char const * RawFileClass::File_Name(void) const
{
	return Filename;
}

/***********************************************************************************************
 * RawFileClass::RawFileClass -- Default constructor for a file object.                        *
 *                                                                                             *
 *    This constructs a null file object. A null file object has no file handle or filename    *
 *    associated with it. In order to use a file object created in this fashion it must be     *
 *    assigned a name and then opened.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
;*   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
inline RawFileClass::RawFileClass(void) : Filename(0)
{
	Handle = -1;
	Allocated = false;
}

/***********************************************************************************************
 * RawFileClass::Is_Open -- Checks to see if the file is open or not.                          *
 *                                                                                             *
 *    Use this routine to determine if the file is open. It returns true if it is.             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the file open?                                                            *
 *                                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
;*   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int RawFileClass::Is_Open(void) const
{
	return (Handle != -1);
}

#endif
