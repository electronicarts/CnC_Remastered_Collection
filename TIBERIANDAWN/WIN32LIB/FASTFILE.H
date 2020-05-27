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

/*==========================================================================
 *
 *  Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       fastfile.h
 *  Content:    Definitions for fastfile access.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTBILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 ***************************************************************************/

typedef LPVOID  HFASTFILE;

extern BOOL FastFileInit( LPSTR fname, int max_handles );
extern void FastFileFini( void );
extern HFASTFILE FastFileOpen( LPSTR name );
extern BOOL FastFileClose( HFASTFILE pfe );
extern BOOL FastFileRead( HFASTFILE pfh, LPVOID ptr, int size );
extern BOOL FastFileSeek( HFASTFILE pfe, int off, int how );
extern long FastFileTell( HFASTFILE pfe );
extern LPVOID FastFileLock( HFASTFILE pfe, int off, int len );
extern BOOL FastFileUnlock( HFASTFILE pfe, int off, int len );
