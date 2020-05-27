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

/***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Library - Filio header stuff.            *
;*                                                                         *
;*                    File Name : FILE.H                                 	*
;*                                                                         *
;*                   Programmer : Scott K. Bowen                           *
;*                                                                         *
;*                   Start Date : September 13, 1993                       *
;*                                                                         *
;*                  Last Update : April 11, 1994									*
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef FILE_H
#include "file.h"
#endif

#ifndef _FILE_H
#define _FILE_H


/*=========================================================================*/
/* Fileio defines																				*/
/*=========================================================================*/

#define	LIB_CDROM TRUE

#define MODE_OLDFILE				(O_RDONLY | O_BINARY)
#define MODE_NEWFILE				(O_WRONLY | O_BINARY | O_CREAT | O_TRUNC)
#define MODE_READWRITE			(O_RDWR   | O_BINARY)

#define FILEOPENERROR			-1
#define FILEOPEN(f,m)			ibm_open(f, m, (((UWORD) m) == MODE_OLDFILE) ? S_IREAD : (S_IREAD | S_IWRITE))

#define FILECLOSE(fd)			ibm_close(fd)
#define FILEREAD(f,b,n)			ibm_read(f,b,(WORD)(n))
#define FILEWRITE(f,b,n)		ibm_write(f,b,(WORD)(n))
#define FILESEEK(f,b,n)			ibm_lseek(f, b, n)
#define FILEDELETE(f)			ibm_unlink(f)
#define CHANGEDIR(p)				ibm_chdir(p)

#define FILENAMESIZE				13
#define IO_CHUNK_SIZE			0xfff0UL

/* 
**	Maximum number of file handles 
*/
#define TABLE_MAX			20

 
/*=========================================================================*/
/* The file handle table */
/*=========================================================================*/
typedef struct {
	BOOL						Empty;		// Is this handle empty?
	WORD						Handle;		// DOS file handle (0 = resident).
	LONG						Pos;			// Current file position.
	LONG						Start;		// Offset of file from pointer.
	WORD						Index;		// FileData[] index.
	WORD						Mode;			// Access mode (WW).
	BYTE						*Name;		// File name pointer.
} FileHandleType;


/*=========================================================================*/
/* The following prototypes are for the file: FILEIO.CPP							*/
/*=========================================================================*/

WORD ibm_getdisk(VOID);
WORD ibm_setdisk(WORD drive);
WORD ibm_close(WORD handle);
WORD ibm_unlink(BYTE const *name);
LONG ibm_lseek(WORD handle, LONG offset, WORD where);
UWORD ibm_read(WORD handle, VOID *ptr, UWORD bytes);
UWORD ibm_write(WORD handle, VOID *ptr, UWORD bytes);
WORD ibm_open(BYTE const *name, UWORD mode, WORD attrib);
WORD ibm_chdir(BYTE const *path);

/*=========================================================================*/
/* The following prototypes are for the file: FILELIB.CPP						*/
/*=========================================================================*/

WORD cdecl Do_Open_Error(FileErrorType errormsgnum, BYTE const *file_name);
VOID cdecl Do_IO_Error(FileErrorType errormsgnum, BYTE const *filename);
LONG cdecl Read_File_With_Recovery( WORD handle, VOID *buf, UWORD bytes );
WORD cdecl Open_File_With_Recovery( BYTE const *file_name, UWORD mode );
BOOL cdecl Cache_File(WORD index, WORD file_handle);


/*=========================================================================*/
/* The following prototypes are for the file: DEVICES.ASM						*/
/*=========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

extern VOID Get_Devices(VOID);
extern WORD Is_Device_Real(WORD device);

#ifdef __cplusplus
}
#endif

/*=========================================================================*/
/* The following prototypes are for the file: DEVTABLE.ASM						*/
/*=========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

extern VOID Init_Device_Table(BYTE *table);
extern WORD Max_Device(VOID);


#ifdef __cplusplus
}
#endif


/*=========================================================================*/
/* The following prototypes are for the file: HARDERR.ASM						*/
/*=========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

extern VOID Install_Hard_Error_Handler(VOID);
extern VOID Remove_Hard_Error_Handler(VOID);

#ifdef __cplusplus
}
#endif


/*=========================================================================*/
/* Globale variables in the fileio system.						               */
/*=========================================================================*/

extern BYTE CallingDOSInt;					 
extern "C" extern BYTE MaxDevice,DefaultDrive;
extern BYTE MultiDriveSearch;	
extern FileDataType *FileDataPtr;
extern FileHandleType FileHandleTable[TABLE_MAX];
extern UWORD NumFiles;							 	// Number of files, except PAK, in file table.
extern UWORD NumPAKFiles;							// Number of PAK files in filetable.
extern VOID *FileCacheHeap;						// Pointer to the cache in memory.
extern WORD DiskNumber;						 
extern WORD MaxDirNum;


/*=========================================================================*/
	 


#endif // _FILE_H


