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


#include "bmp8.h"

//***********************************************************************************************
BMP8::~BMP8()
{
	// free resources
	if( hBitmap )
		::DeleteObject( hBitmap );
	if( hPal )
		::DeleteObject( hPal );
}

//***********************************************************************************************
bool BMP8::Init( const char* szFile, HWND hWnd )
{
  int                  i;
  char                 string[128];
  DWORD                dwRead;
  BITMAPFILEHEADER     bitmapHeader;
  BITMAPINFOHEADER     bitmapInfoHeader;
  LPLOGPALETTE         lpLogPalette;
  char                *palData;
  HGLOBAL              hmem2;
  LPVOID               lpvBits;
  PAINTSTRUCT          ps;
  HDC                  hdc;
  HPALETTE             select;
  UINT                 realize;
  RECT                 rect;


	//	Remember window handle for use later.
	this->hWnd = hWnd;

	//	Retrieve a handle identifying the file. 
	HANDLE hFile = ::CreateFile(
		szFile,
		GENERIC_READ,
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY,
		(HANDLE)NULL );

	if(	!hFile )
		return false;

	// Retrieve the BITMAPFILEHEADER structure. 
	::ReadFile( hFile, &bitmapHeader, sizeof(BITMAPFILEHEADER), &dwRead, (LPOVERLAPPED)NULL );

	// Retrieve the BITMAPFILEHEADER structure. 
	::ReadFile( hFile, &bitmapInfoHeader, sizeof(BITMAPINFOHEADER), &dwRead, (LPOVERLAPPED)NULL );
 
	// Allocate memory for the BITMAPINFO structure. 
	HGLOBAL infoHeaderMem = ::GlobalAlloc( GHND, sizeof(BITMAPINFOHEADER) + ((1<<bitmapInfoHeader.biBitCount) * sizeof(RGBQUAD)) );
 
	LPBITMAPINFO lpHeaderMem = (LPBITMAPINFO)::GlobalLock( infoHeaderMem ); 
 
	// Load BITMAPINFOHEADER into the BITMAPINFO structure. 
	lpHeaderMem->bmiHeader.biSize          = bitmapInfoHeader.biSize; 
	lpHeaderMem->bmiHeader.biWidth         = bitmapInfoHeader.biWidth; 
	lpHeaderMem->bmiHeader.biHeight        = bitmapInfoHeader.biHeight; 
	lpHeaderMem->bmiHeader.biPlanes        = bitmapInfoHeader.biPlanes; 
	lpHeaderMem->bmiHeader.biBitCount      = bitmapInfoHeader.biBitCount; 
	lpHeaderMem->bmiHeader.biCompression   = bitmapInfoHeader.biCompression; 
	lpHeaderMem->bmiHeader.biSizeImage     = bitmapInfoHeader.biSizeImage; 
	lpHeaderMem->bmiHeader.biXPelsPerMeter = bitmapInfoHeader.biXPelsPerMeter; 
	lpHeaderMem->bmiHeader.biYPelsPerMeter = bitmapInfoHeader.biYPelsPerMeter; 
	lpHeaderMem->bmiHeader.biClrUsed       = bitmapInfoHeader.biClrUsed; 
	lpHeaderMem->bmiHeader.biClrImportant  = bitmapInfoHeader.biClrImportant; 

	// Retrieve the color table. 
	// 1 << bitmapInfoHeader.biBitCount == 2 ^ bitmapInfoHeader.biBitCount 
	::ReadFile( hFile, lpHeaderMem->bmiColors, ((1<<bitmapInfoHeader.biBitCount) * sizeof(RGBQUAD)),
				&dwRead, (LPOVERLAPPED)NULL );


	lpLogPalette = (LPLOGPALETTE)new char[ (sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*256) ];
	lpLogPalette->palVersion=0x300;
	lpLogPalette->palNumEntries=256;

	palData = (char*)lpHeaderMem->bmiColors;

	for( i = 0; i < 256; i++ )
	{
		lpLogPalette->palPalEntry[i].peRed = *palData++;
		lpLogPalette->palPalEntry[i].peGreen = *palData++;
		lpLogPalette->palPalEntry[i].peBlue = *palData++;
		lpLogPalette->palPalEntry[i].peFlags = *palData++;
	}
	hPal = ::CreatePalette( lpLogPalette );
	delete [] lpLogPalette;

	// Allocate memory for the required number of bytes. 
	hmem2 = ::GlobalAlloc( GHND, (bitmapHeader.bfSize - bitmapHeader.bfOffBits) );
 
	lpvBits = ::GlobalLock( hmem2 );
 
	// Retrieve the bitmap data. 
	::ReadFile( hFile, lpvBits, (bitmapHeader.bfSize - bitmapHeader.bfOffBits), &dwRead, (LPOVERLAPPED)NULL );
 
	// Create a bitmap from the data stored in the .BMP file. 
	hdc = ::GetDC( hWnd );
	select = ::SelectPalette( hdc, hPal, 0 );
	if( !select )
		return false;
	realize = ::RealizePalette( hdc );
	if( realize == GDI_ERROR )
		return false;

	hBMP = ::CreateDIBitmap( hdc, &bitmapInfoHeader, CBM_INIT, lpvBits, lpHeaderMem, DIB_RGB_COLORS );
	::ReleaseDC( hWnd, hdc );

	// Unlock the global memory objects and close the .BMP file.  
	::GlobalUnlock( infoHeaderMem );
	::GlobalUnlock( hmem2 );
	::CloseHandle( hFile );
 
	if( !hBMP )
		return false;
 
	return true;
}


bit8 BMP8::drawBmp(void)
{
  // Paint the window (and draw the bitmap). 
 
  PAINTSTRUCT ps;
  HDC         hdc;
  char        string[128];

  InvalidateRect(WindowHandle_,NULL,FALSE); // keep windows from screwing up the
                                           //  redrawing (as much).
  hdc=BeginPaint(WindowHandle_,&ps);

  //Do palette stuff
  HPALETTE select=SelectPalette(ps.hdc,PalHandle_,0);
  if (select==NULL)
  {
    sprintf(string,"Select Pal Fail: %d",GetLastError());
    MessageBox(NULL,string,"OK",MB_OK);
  }
  UINT realize=RealizePalette(ps.hdc);
  if (realize==GDI_ERROR)
  {
    sprintf(string,"Realize Pal Fail: %d",GetLastError());
    MessageBox(NULL,string,"OK",MB_OK);
  }

  HDC hdcMem = CreateCompatibleDC(ps.hdc); 
  SelectObject(hdcMem, BitmapHandle_); 
  BITMAP bm;
  GetObject(BitmapHandle_, sizeof(BITMAP), (LPSTR) &bm);
  
  /// for non-stretching version
  ///////BitBlt(ps.hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY); 

  RECT clientRect;
  GetClientRect(WindowHandle_,&clientRect);
  SetStretchBltMode(ps.hdc,COLORONCOLOR);
  StretchBlt(ps.hdc,0,0,clientRect.right,clientRect.bottom,hdcMem,0,0,bm.bmWidth,
    bm.bmHeight,SRCCOPY);


  DeleteDC(hdcMem); 
  EndPaint(WindowHandle_,&ps);
  return(TRUE);
}
