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

#ifndef BMP8_H
#define BMP8_H

//#include<stdlib.h>
//#include<stdio.h>
//#include "wstypes.h"
//#include "winblows.h"

class BMP8
{
public:
	BMP8() : hBMP( NULL ), hPal( NULL ), hWnd( NULL )	{}
	~BMP8();

	bool	Init( const char* szFile, HWND hWnd );
	bool	Draw(void);  // call this from your WM_PAINT message

private:
	HBITMAP		hBMP;
	HPALETTE	hPal;
	HWND		hWnd;
};


#endif