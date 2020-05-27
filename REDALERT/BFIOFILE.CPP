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

/* $Header: /CounterStrike/BFIOFILE.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Westwood Library                                             *
 *                                                                                             *
 *                    File Name : RAMFILE.CPP                                                  *
 *                                                                                             *
 *                   Programmer : David R. Dettmer                                             *
 *                                                                                             *
 *                   Start Date : November 10, 1995                                            *
 *                                                                                             *
 *                  Last Update : November 10, 1995  [DRD]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BufferIOFileClass::BufferIOFileClass -- Filename based constructor for a file object.     *
 *   BufferIOFileClass::BufferIOFileClass -- default constructor for a file object.            *
 *   BufferIOFileClass::Cache -- Load part or all of a file data into RAM.                     *
 *   BufferIOFileClass::Close -- Perform a closure of the file.                                *
 *   BufferIOFileClass::Commit -- Writes the cache to the file if it has changed.              *
 *   BufferIOFileClass::Free -- Frees the allocated buffer.                                    *
 *   BufferIOFileClass::Is_Available -- Checks for existence of file cached or on disk.        *
 *   BufferIOFileClass::Is_Open -- Determines if the file is open.                             *
 *   BufferIOFileClass::Open -- Assigns name and opens file in one operation.                  *
 *   BufferIOFileClass::Open -- Opens the file object with the rights specified.               *
 *   BufferIOFileClass::Read -- Reads data from the file cache.                                *
 *   BufferIOFileClass::Seek -- Moves the current file pointer in the file.                    *
 *   BufferIOFileClass::Set_Name -- Checks for name changed for a cached file.                 *
 *   BufferIOFileClass::Size -- Determines size of file (in bytes).                            *
 *   BufferIOFileClass::Write -- Writes data to the file cache.                                *
 *   BufferIOFileClass::~BufferIOFileClass -- Destructor for the file object.                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"bfiofile.h"
#include	<string.h>


/***********************************************************************************************
 * BufferIOFileClass::BufferIOFileClass -- Filename based constructor for a file object.       *
 *                                                                                             *
 *    This constructor is called when a file object is created with a supplied filename, but   *
 *    not opened at the same time. In this case, an assumption is made that the supplied       *
 *    filename is a constant string. A duplicate of the filename string is not created since   *
 *    it would be wasteful in that case.                                                       *
 *                                                                                             *
 * INPUT:   filename -- The filename to assign to this file object.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
BufferIOFileClass::BufferIOFileClass(char const * filename) :
	IsAllocated(false),
	IsOpen(false),
	IsDiskOpen(false),
	IsCached(false),
	IsChanged(false),
	UseBuffer(false),
	BufferRights(0),
	Buffer(0),
	BufferSize(0),
	BufferPos(0),
	BufferFilePos(0),
	BufferChangeBeg(-1),
	BufferChangeEnd(-1),
	FileSize(0),
	FilePos(0),
	TrueFileStart(0)
{
	BufferIOFileClass::Set_Name(filename);
}


/***********************************************************************************************
 * BufferIOFileClass::BufferIOFileClass -- default constructor for a file object.              *
 *                                                                                             *
 *    This is the default constructor for a file object.                                       *
 *                                                                                             *
 * INPUT:  none                                                                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
BufferIOFileClass::BufferIOFileClass(void) :
	IsAllocated(false),
	IsOpen(false),
	IsDiskOpen(false),
	IsCached(false),
	IsChanged(false),
	UseBuffer(false),
	BufferRights(0),
	Buffer(0),
	BufferSize(0),
	BufferPos(0),
	BufferFilePos(0),
	BufferChangeBeg(-1),
	BufferChangeEnd(-1),
	FileSize(0),
	FilePos(0),
	TrueFileStart(0)
{
}


/***********************************************************************************************
 * BufferIOFileClass::~BufferIOFileClass -- Destructor for the file object.                    *
 *                                                                                             *
 *    This destructor will free all memory allocated thru using Cache routines.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
BufferIOFileClass::~BufferIOFileClass(void)
{
	Free();
}


/***********************************************************************************************
 * BufferIOFileClass::Cache -- Load part or all of a file data into RAM.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the file load successful?  It could fail if there wasn't enough room     *
 *                to allocate the raw data block.                                              *
 *                                                                                             *
 * WARNINGS:   This routine goes to disk for a potentially very long time.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
bool BufferIOFileClass::Cache( long size, void * ptr )
{
	if (Buffer) {
		//
		// if trying to cache again with size or ptr fail
		//
		if (size || ptr) {
			return( false );
		} else {
			return( true );
		}
	}

	if ( Is_Available() ) {
		FileSize = Size();
	} else {
		FileSize = 0;
	}

	if (size) {
		//
		// minimum buffer size for performance
		//
		if (size < MINIMUM_BUFFER_SIZE) {
			size = MINIMUM_BUFFER_SIZE;

			/*
			**	Specifying a size smaller than the minimum is an error
			**	IF a buffer pointer was also specified. In such a case the
			**	system cannot use the buffer.
			*/
			if (ptr) {
				Error(EINVAL);
			}
		}

		BufferSize = size;
	} else {
		BufferSize = FileSize;
	}

	//
	// if size == 0 and a ptr to a buffer is specified then that is invalid.
	// if the BufferSize is 0 then this must be a new file and no size was
	// specified so exit.
	//
	if ( (size == 0 && ptr) || !BufferSize) {
		return( false );
	}

	if (ptr) {
		Buffer = ptr;
	} else {
		Buffer = new char [BufferSize];
	}

	if (Buffer) {
		IsAllocated			= true;
		IsDiskOpen			= false;
		BufferPos			= 0;
		BufferFilePos		= 0;
		BufferChangeBeg	= -1;
		BufferChangeEnd	= -1;
		FilePos				= 0;
		TrueFileStart		= 0;

		//
		// the file was checked for availability then set the FileSize
		//
		if (FileSize) {
			long readsize;
			int opened = false;
			long prevpos = 0;


			if (FileSize <= BufferSize) {
				readsize = FileSize;
			} else {
				readsize = BufferSize;
			}

			if ( Is_Open() ) {
				//
				// get previous file position
				//
				prevpos = Seek(0);

				//
				// get true file position
				//
				if ( RawFileClass::Is_Open() ) {
					TrueFileStart = RawFileClass::Seek(0);
				} else {
					TrueFileStart = prevpos;
				}

				if (FileSize <= BufferSize) {
					//
					// if previous position is non-zero seek to the beginning
					//
					if (prevpos) {
						Seek(0, SEEK_SET);
					}

					//
					// set the buffer position for future reads/writes
					//
					BufferPos = prevpos;
				} else {
					BufferFilePos = prevpos;
				}

				FilePos = prevpos;
			} else {
				if ( Open() ) {
					TrueFileStart = RawFileClass::Seek(0);
					opened = true;
				}
			}

			long actual = Read(Buffer, readsize);

			if (actual != readsize) {
				Error(EIO);
			}

			if (opened) {
				Close();
			} else {
				//
				// seek to the previous position in the file
				//
				Seek(prevpos, SEEK_SET);
			}

			IsCached = true;
		}

		UseBuffer = true;
		return(true);
	}

	Error(ENOMEM);

	return(false);
}


/***********************************************************************************************
 * BufferIOFileClass::Free -- Frees the allocated buffer.                                      *
 *                                                                                             *
 *    This routine will free the buffer. By using this in conjunction with the                 *
 *    Cache() function, one can maintain tight control of memory usage.                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
void BufferIOFileClass::Free(void)
{
	if (Buffer) {
		if (IsAllocated) {
			delete [] Buffer;
			IsAllocated = false;
		}

		Buffer = 0;
	}

	BufferSize		= 0;
	IsOpen			= false;
	IsCached			= false;
	IsChanged		= false;
	UseBuffer		= false;
}


/***********************************************************************************************
 * BufferIOFileClass::Commit -- Writes the cache to the file if it has changed.                *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  false, did not need to write the buffer.                                           *
 *          true, wrote the buffer.                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/15/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
bool BufferIOFileClass::Commit( void )
{
	long size;


	if (UseBuffer) {
		if (IsChanged) {
			size = BufferChangeEnd - BufferChangeBeg;

			if (IsDiskOpen) {
				RawFileClass::Seek( TrueFileStart + BufferFilePos +
										  BufferChangeBeg, SEEK_SET );
				RawFileClass::Write( Buffer, size );
				RawFileClass::Seek( TrueFileStart + FilePos, SEEK_SET );
			} else {
				RawFileClass::Open();
				RawFileClass::Seek( TrueFileStart + BufferFilePos +
										  BufferChangeBeg, SEEK_SET );
				RawFileClass::Write( Buffer, size );
				RawFileClass::Close();
			}

			IsChanged = false;
			return( true );
		} else {
			return( false );
		}
	} else {
		return( false );
	}
}


/***********************************************************************************************
 * BufferIOFileClass::Set_Name -- Checks for name changed for a cached file.                   *
 *                                                                                             *
 *    Checks for a previous filename and that it is cached.  If so, then check the             *
 *    new filename against the old. If they are the same then return that filename.            *
 *    Otherwise, the file object's name is set with just the raw filename as passed            *
 *    to this routine.                                                                         *
 *                                                                                             *
 * INPUT:   filename -- Pointer to the filename to set as the name of this file object.        *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to the final and complete filename of this file object. This     *
 *          may have a path attached to the file.                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/15/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
char const * BufferIOFileClass::Set_Name(char const * filename)
{
	if ( File_Name() && UseBuffer) {
		if ( strcmp(filename, File_Name() ) == 0) {
			return( File_Name() );
		} else {
			Commit();
			IsCached = false;
		}
	}

	RawFileClass::Set_Name(filename);
	return( File_Name() );
}


/***********************************************************************************************
 * BufferIOFileClass::Is_Available -- Checks for existence of file cached or on disk.          *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the file available for opening?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/16/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
int BufferIOFileClass::Is_Available(int )
{
	if (UseBuffer) {
		return(true);
	}

	return( RawFileClass::Is_Available() );
}


/***********************************************************************************************
 * BufferIOFileClass::Is_Open -- Determines if the file is open.                               *
 *                                                                                             *
 *    If part or all of the file is cached, then return that it is opened. A closed file       *
 *    doesn't have a valid pointer.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the file open?                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
int BufferIOFileClass::Is_Open(void) const
{
	if (IsOpen && UseBuffer) {
		return( true );
	}

	return( RawFileClass::Is_Open() );
}


/***********************************************************************************************
 * BufferIOFileClass::Open -- Assigns name and opens file in one operation.                    *
 *                                                                                             *
 *    This routine will assign the specified filename to the file object and open it at the    *
 *    same time. If the file object was already open, then it will be closed first. If the     *
 *    file object was previously assigned a filename, then it will be replaced with the new    *
 *    name. Typically, this routine is used when an anonymous file object has been crated and  *
 *    now it needs to be assigned a name and opened.                                           *
 *                                                                                             *
 * INPUT:   filename -- The filename to assign to this file object.                            *
 *                                                                                             *
 *          rights   -- The open file access rights to use.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the file opened? The return value of this is moot, since the open file   *
 *          is designed to never return unless it succeeded.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
int BufferIOFileClass::Open(char const * filename, int rights)
{
	Set_Name(filename);
	return( BufferIOFileClass::Open( rights ) );
}


/***********************************************************************************************
 * BufferIOFileClass::Open -- Opens the file object with the rights specified.                 *
 *                                                                                             *
 *    This routine is used to open the specified file object with the access rights indicated. *
 *    This only works if the file has already been assigned a filename. It is guaranteed, by   *
 *    the error handler, that this routine will always return with success.                    *
 *                                                                                             *
 * INPUT:   rights   -- The file access rights to use when opening this file. This is a        *
 *                      combination of READ and/or WRITE bit flags.                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the file opened successfully? This will always return true by reason of  *
 *          the error handler.                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
int BufferIOFileClass::Open(int rights)
{
	BufferIOFileClass::Close();

	if (UseBuffer) {

		BufferRights = rights;		// save rights requested for checks later

		if (rights != READ ||
			 (rights == READ && FileSize > BufferSize) ) {

			if (rights == WRITE) {
				RawFileClass::Open( rights );
				RawFileClass::Close();
				rights = READ | WRITE;
				TrueFileStart = 0;		// now writing to single file
			}

			if (TrueFileStart) {
				UseBuffer = false;
				Open( rights );
				UseBuffer = true;
			} else {
				RawFileClass::Open( rights );
			}

			IsDiskOpen = true;

			if (BufferRights == WRITE) {
				FileSize = 0;
			}

		} else {
			IsDiskOpen = false;
		}

		BufferPos			= 0;
		BufferFilePos		= 0;
		BufferChangeBeg	= -1;
		BufferChangeEnd	= -1;
		FilePos				= 0;
		IsOpen				= true;
	} else {
		RawFileClass::Open( rights );
	}

	return( true );
}


/***********************************************************************************************
 * BufferIOFileClass::Write -- Writes data to the file cache.                                  *
 *                                                                                             *
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
 *   11/15/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
long BufferIOFileClass::Write(void const * buffer, long size)
{
	int opened = false;

	if ( !Is_Open() ) {
		if (!Open(WRITE)) {
			return(0);
		}
		TrueFileStart = RawFileClass::Seek(0);
		opened = true;
	}

	if (UseBuffer) {
		long sizewritten = 0;

		if (BufferRights != READ) {
			while (size) {
				long sizetowrite;

				if (size >= (BufferSize - BufferPos) ) {
					sizetowrite = (BufferSize - BufferPos);
				} else {
					sizetowrite = size;
				}

				if (sizetowrite != BufferSize) {

					if ( !IsCached ) {
						long readsize;

						if (FileSize < BufferSize) {
							readsize = FileSize;
							BufferFilePos = 0;
						} else {
							readsize = BufferSize;
							BufferFilePos = FilePos;
						}

						if (TrueFileStart) {
							UseBuffer = false;
							Seek( FilePos, SEEK_SET );
							Read( Buffer, BufferSize );
							Seek( FilePos, SEEK_SET );
							UseBuffer = true;
						} else {
							RawFileClass::Seek( BufferFilePos, SEEK_SET );
							RawFileClass::Read( Buffer, readsize );
						}

						BufferPos			= 0;
						BufferChangeBeg	= -1;
						BufferChangeEnd	= -1;

						IsCached = true;
					}
				}

				memmove((char *)Buffer + BufferPos, (char *)buffer + sizewritten, sizetowrite);

				IsChanged = true;
				sizewritten += sizetowrite;
				size -= sizetowrite;

				if (BufferChangeBeg == -1) {
					BufferChangeBeg = BufferPos;
					BufferChangeEnd = BufferPos;
				} else {
					if (BufferChangeBeg > BufferPos) {
						BufferChangeBeg = BufferPos;
					}
				}

				BufferPos += sizetowrite;

				if (BufferChangeEnd < BufferPos) {
					BufferChangeEnd = BufferPos;
				}

				FilePos = BufferFilePos + BufferPos;

				if (FileSize < FilePos) {
					FileSize = FilePos;
				}

				//
				// end of buffer reached?
				//
				if (BufferPos == BufferSize) {
					Commit();

					BufferPos = 0;
					BufferFilePos = FilePos;
					BufferChangeBeg = -1;
					BufferChangeEnd = -1;

					if (size && FileSize > FilePos) {
						if (TrueFileStart) {
							UseBuffer = false;
							Seek( FilePos, SEEK_SET );
							Read( Buffer, BufferSize );
							Seek( FilePos, SEEK_SET );
							UseBuffer = true;
						} else {
							RawFileClass::Seek( FilePos, SEEK_SET );
							RawFileClass::Read( Buffer, BufferSize );
						}
					} else {
						IsCached = false;
					}
				}
			}
		} else {
			Error(EACCES);
		}

		size = sizewritten;
	} else {
		size = RawFileClass::Write(buffer, size);
	}

	if (opened) {
		Close();
	}

	return( size );
}


/***********************************************************************************************
 * BufferIOFileClass::Read -- Reads data from the file cache.                                  *
 *                                                                                             *
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
 *   11/15/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
long BufferIOFileClass::Read(void * buffer, long size)
{
	int opened = false;

	if ( !Is_Open() ) {
		if ( Open() ) {
			TrueFileStart = RawFileClass::Seek(0);
			opened = true;
		}
	}

	if (UseBuffer) {
		long sizeread = 0;

		if (BufferRights != WRITE) {
			while (size) {
				long sizetoread;

				if (size >= (BufferSize - BufferPos) ) {
					sizetoread = (BufferSize - BufferPos);
				} else {
					sizetoread = size;
				}

				if ( !IsCached ) {
					long readsize;

					if (FileSize < BufferSize) {
						readsize = FileSize;
						BufferFilePos = 0;
					} else {
						readsize = BufferSize;
						BufferFilePos = FilePos;
					}

					if (TrueFileStart) {
						UseBuffer = false;
						Seek( FilePos, SEEK_SET );
						Read( Buffer, BufferSize );
						Seek( FilePos, SEEK_SET );
						UseBuffer = true;
					} else {
						RawFileClass::Seek( BufferFilePos, SEEK_SET );
						RawFileClass::Read( Buffer, readsize );
					}

					BufferPos			= 0;
					BufferChangeBeg	= -1;
					BufferChangeEnd	= -1;

					IsCached = true;
				}

				memmove((char *)buffer + sizeread, (char *)Buffer + BufferPos, sizetoread);

				sizeread += sizetoread;
				size -= sizetoread;
				BufferPos += sizetoread;
				FilePos = BufferFilePos + BufferPos;

				//
				// end of buffer reached?
				//
				if (BufferPos == BufferSize) {
					Commit();

					BufferPos = 0;
					BufferFilePos = FilePos;
					BufferChangeBeg = -1;
					BufferChangeEnd = -1;

					if (size && FileSize > FilePos) {
						if (TrueFileStart) {
							UseBuffer = false;
							Seek( FilePos, SEEK_SET );
							Read( Buffer, BufferSize );
							Seek( FilePos, SEEK_SET );
							UseBuffer = true;
						} else {
							RawFileClass::Seek( FilePos, SEEK_SET );
							RawFileClass::Read( Buffer, BufferSize );
						}
					} else {
						IsCached = false;
					}
				}
			}
		} else {
			Error(EACCES);
		}

		size = sizeread;
	} else {
		size = RawFileClass::Read(buffer, size);
	}

	if (opened) {
		Close();
	}

	return( size );
}


/***********************************************************************************************
 * BufferIOFileClass::Seek -- Moves the current file pointer in the file.                      *
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
 *   11/15/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
long BufferIOFileClass::Seek(long pos, int dir)
{
	if (UseBuffer) {
		bool adjusted = false;

		switch (dir) {
			case SEEK_END:
				FilePos = FileSize;
				break;

			case SEEK_SET:
				FilePos = 0;
				break;

			case SEEK_CUR:
			default:
				break;
		}

		if (TrueFileStart) {
			if (pos >= TrueFileStart) {
				pos -= TrueFileStart;
				adjusted = true;
			}
		}

		FilePos += pos;

		if (FilePos < 0) {
			FilePos = 0;
		}

		if (FilePos > FileSize ) {
			FilePos = FileSize;
		}

		if (FileSize <= BufferSize) {
			BufferPos = FilePos;
		} else {
			if (FilePos >= BufferFilePos &&
				 FilePos < (BufferFilePos + BufferSize) ) {
				BufferPos = FilePos - BufferFilePos;
			} else {
				Commit();
// check!!
				if (TrueFileStart) {
					UseBuffer = false;
					Seek(FilePos, SEEK_SET);
					UseBuffer = true;
				} else {
					RawFileClass::Seek(FilePos, SEEK_SET);
				}

				IsCached = false;
			}
		}

		if (TrueFileStart && adjusted) {
			return( FilePos + TrueFileStart );
		}

		return( FilePos );
	}

	return( RawFileClass::Seek(pos, dir) );
}


/***********************************************************************************************
 * BufferIOFileClass::Size -- Determines size of file (in bytes).                              *
 *                                                                                             *
 *    If part or all of the file is cached, then the size of the file is already               *
 *    determined and available. Otherwise, go to the low level system to find the file         *
 *    size.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes in the file.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
long BufferIOFileClass::Size(void)
{
	if (IsOpen && UseBuffer) {
		return( FileSize );
	}

	return( RawFileClass::Size() );
}


/***********************************************************************************************
 * BufferIOFileClass::Close -- Perform a closure of the file.                                  *
 *                                                                                             *
 *    Call Commit() to write the buffer if the file is cached and the buffer has changed,      *
 *    then call lower level Close().                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
void BufferIOFileClass::Close(void)
{
	if (UseBuffer) {
		Commit();

		if (IsDiskOpen) {

			if (TrueFileStart) {
				UseBuffer = false;
				Close();
				UseBuffer = true;
			} else {
				RawFileClass::Close();
			}

			IsDiskOpen = false;
		}

		IsOpen = false;
	} else {
		RawFileClass::Close();
	}
}

