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


/*
    Implementation of Win95 tracing facility to mimic that of NT
*/

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <process.h>
#include "w95trace.h"


void OutputDebugStringW95( LPCTSTR /*lpOutputString*/, ...)
{
#if 0

    HANDLE heventDBWIN;  /* DBWIN32 synchronization object */
    HANDLE heventData;   /* data passing synch object */
    HANDLE hSharedFile;  /* memory mapped file shared data */
    LPSTR lpszSharedMem;
    char achBuffer[500];

    /* create the output buffer */
    va_list args;
    va_start(args, lpOutputString);
    vsprintf(achBuffer, lpOutputString, args);
    va_end(args);

	achBuffer[499] = 0;	//	Null-terminate here, just in case vsprintf didn't do it because lpOutputString was too long.

    /* 
        Do a regular OutputDebugString so that the output is 
        still seen in the debugger window if it exists.

        This ifdef is necessary to avoid infinite recursion 
        from the inclusion of W95TRACE.H
    */
#ifdef _UNICODE
    ::OutputDebugStringW(achBuffer);
#else
    ::OutputDebugStringA(achBuffer);
#endif

	//	added by ajw
	//FILE* pFile = fopen( "debugout.wri", "a" );
	FILE* pFile = fopen( "wolapi.out", "a" );
	fprintf( pFile, achBuffer );
	fclose( pFile );

//    /* bail if it's not Win95 */
//    {
//        OSVERSIONINFO VerInfo;
//        VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//        GetVersionEx(&VerInfo);
//        if ( VerInfo.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS )
//            return;
//    }

    /* make sure DBWIN is open and waiting */
    heventDBWIN = OpenEvent(EVENT_MODIFY_STATE, FALSE, "DBWIN_BUFFER_READY");
    if ( !heventDBWIN )
    {
        //MessageBox(NULL, "DBWIN_BUFFER_READY nonexistent", NULL, MB_OK);
        return;            
    }

    /* get a handle to the data synch object */
    heventData = OpenEvent(EVENT_MODIFY_STATE, FALSE, "DBWIN_DATA_READY");
    if ( !heventData )
    {
        // MessageBox(NULL, "DBWIN_DATA_READY nonexistent", NULL, MB_OK);
        CloseHandle(heventDBWIN);
        return;            
    }
    
    hSharedFile = CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, 4096, "DBWIN_BUFFER");
    if (!hSharedFile) 
    {
        //MessageBox(NULL, "DebugTrace: Unable to create file mapping object DBWIN_BUFFER", "Error", MB_OK);
        CloseHandle(heventDBWIN);
        CloseHandle(heventData);
        return;
    }

    lpszSharedMem = (LPSTR)MapViewOfFile(hSharedFile, FILE_MAP_WRITE, 0, 0, 512);
    if (!lpszSharedMem) 
    {
        //MessageBox(NULL, "DebugTrace: Unable to map shared memory", "Error", MB_OK);
        CloseHandle(heventDBWIN);
        CloseHandle(heventData);
        return;
    }

    /* wait for buffer event */
    WaitForSingleObject(heventDBWIN, INFINITE);

    /* write it to the shared memory */
    *((LPDWORD)lpszSharedMem) = getpid();
    wsprintf(lpszSharedMem + sizeof(DWORD), "%s", achBuffer);

    /* signal data ready event */
    SetEvent(heventData);

    /* clean up handles */
    CloseHandle(hSharedFile);
    CloseHandle(heventData);
    CloseHandle(heventDBWIN);

    return;
#endif
}
