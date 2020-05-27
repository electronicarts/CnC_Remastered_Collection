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

/* $Header: /CounterStrike/RAWFILE.H 1     3/03/97 10:25a Joe_bostic $ */
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
 *   RawFileClass::~RawFileClass -- Default deconstructor for a file object.                   *
 *   RawFileClass::Is_Open -- Checks to see if the file is open or not.                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef RAWFILE_Hx
#define RAWFILE_Hx

#include	<limits.h>
#include	<errno.h>
#include	<stddef.h>
#include	<stdlib.h>

#ifdef WIN32
#include	<windows.h>

#define	NULL_HANDLE		INVALID_HANDLE_VALUE
#define	HANDLE_TYPE		HANDLE
#else
#define	NULL_HANDLE		-1
#define	HANDLE_TYPE		int
#endif

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

#ifndef WWERROR
#define WWERROR	-1
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
		virtual ~RawFileClass(void);

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
		virtual unsigned long Get_Date_Time(void);
		virtual bool Set_Date_Time(unsigned long datetime);
		virtual void Error(int error, int canretry = false, char const * filename=NULL);

		void Bias(int start, int length=-1);

		HANDLE_TYPE Get_File_Handle(void) { return (Handle); };

		/*
		**	These bias values enable a sub-portion of a file to appear as if it
		**	were the whole file. This comes in very handy for multi-part files such as
		**	mixfiles.
		*/
		int BiasStart;
		int BiasLength;

	protected:

		/*
		**	This function returns the largest size a low level DOS read or write may
		**	perform. Larger file transfers are performed in chunks of this size or less.
		*/
		long Transfer_Block_Size(void) {return (long)((unsigned)UINT_MAX)-16L;};

		long Raw_Seek(long pos, int dir=SEEK_CUR);

	private:

		/*
		**	This is the low level DOS handle. A -1 indicates an empty condition.
		*/
		HANDLE_TYPE Handle;

		/*
		**	This points to the filename as a NULL terminated string. It may point to either a
		**	constant or an allocated string as indicated by the "Allocated" flag.
		*/
		char const * const Filename;

		//
		// file date and time are in the following formats:
		//
		//      date   bits 0-4   day (0-31)
		//             bits 5-8   month (1-12)
		//             bits 9-15  year (0-119 representing 1980-2099)
		//
		//      time   bits 0-4   second/2 (0-29)
		//             bits 5-10  minutes (0-59)
		//             bits 11-15 hours (0-23)
		//
		unsigned short Date;
		unsigned short Time;

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
 *   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
inline char const * RawFileClass::File_Name(void) const
{
	return(Filename);
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
 *   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
inline RawFileClass::RawFileClass(void) :
	Rights(READ),
	BiasStart(0),
	BiasLength(-1),
	#ifdef WIN32
	Handle(INVALID_HANDLE_VALUE),
	#else
	Handle(-1),
	#endif
	Filename(0),
	Date(0),
	Time(0),
	Allocated(false)
{
}


/***********************************************************************************************
 * RawFileClass::~RawFileClass -- Default deconstructor for a file object.                     *
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
 *   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
inline RawFileClass::~RawFileClass(void)
{
	Close();
	if (Allocated && Filename) {
		free((char *)Filename);
		((char *&)Filename) = 0;
		Allocated = false;
	}
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
 *   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int RawFileClass::Is_Open(void) const
{
#ifdef WIN32
	return(Handle != INVALID_HANDLE_VALUE);
#else
	return (Handle >= 0);
#endif
}

#endif
