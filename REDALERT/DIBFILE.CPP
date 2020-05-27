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

//*******************************************************************
//
//  file.c
//
//  Source file for Device-Independent Bitmap (DIB) API.  Provides
//  the following functions:
//
//  SaveDIB()           - Saves the specified dib in a file
//  LoadDIB()           - Loads a DIB from a file
//  DestroyDIB()        - Deletes DIB when finished using it
//
// Development Team: Mark Bader
//                   Patrick Schreiber
//                   Garrett McAuliffe
//                   Eric Flo
//                   Tony Claflin
//
// Written by Microsoft Product Support Services, Developer Support.
// COPYRIGHT:
//
//   (C) Copyright Microsoft Corp. 1993.  All rights reserved.
//
//   You have a royalty-free right to use, modify, reproduce and
//   distribute the Sample Files (and/or any modified version) in
//   any way you find useful, provided that you agree that
//   Microsoft has no warranty obligations or liability for any
//   Sample Application Files which are modified.
//
//*******************************************************************
#if (0) // ST - 5/8/2019

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <direct.h>
#include <stdlib.h>
#include "dibutil.h"
#include "dibapi.h"

//#include "WolDebug.h"

/*
 * Dib Header Marker - used in writing DIBs to files
 */
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/*********************************************************************
 *
 * Local Function Prototypes
 *
 *********************************************************************/


HANDLE ReadDIBFile(int);
BOOL MyRead(int, LPSTR, DWORD);
BOOL SaveDIBFile(void);
BOOL WriteDIB(LPSTR, HANDLE);
DWORD PASCAL MyWrite(int, VOID FAR *, DWORD);

/*************************************************************************
 *
 * LoadDIB()
 *
 * Loads the specified DIB from a file, allocates memory for it,
 * and reads the disk file into the memory.
 *
 *
 * Parameters:
 *
 * LPSTR lpFileName - specifies the file to load a DIB from
 *
 * Returns: A handle to a DIB, or NULL if unsuccessful.
 *
 * NOTE: The DIB API were not written to handle OS/2 DIBs; This
 * function will reject any file that is not a Windows DIB.
 *
 * History:   Date      Author       Reason
 *            9/15/91   Mark Bader   Based on DIBVIEW
 *
 *************************************************************************/


HDIB FAR LoadDIB(LPSTR lpFileName)
{
   HDIB hDIB;
   int hFile;
   OFSTRUCT ofs;

   /*
    * Set the cursor to a hourglass, in case the loading operation
    * takes more than a sec, the user will know what's going on.
    */

   SetCursor(LoadCursor(NULL, IDC_WAIT));
   if ((hFile = OpenFile(lpFileName, &ofs, OF_READ)) != -1)
   {
      hDIB = ReadDIBFile(hFile);
      _lclose(hFile);
      SetCursor(LoadCursor(NULL, IDC_ARROW));
      return hDIB;
   }
   else
   {
//      DIBError(ERR_FILENOTFOUND);
      SetCursor(LoadCursor(NULL, IDC_ARROW));
      return NULL;
   }
}


/*************************************************************************
 *
 * SaveDIB()
 *
 * Saves the specified DIB into the specified file name on disk.  No
 * error checking is done, so if the file already exists, it will be
 * written over.
 *
 * Parameters:
 *
 * HDIB hDib - Handle to the dib to save
 *
 * LPSTR lpFileName - pointer to full pathname to save DIB under
 *
 * Return value: 0 if successful, or one of:
 *        ERR_INVALIDHANDLE
 *        ERR_OPEN
 *        ERR_LOCK
 *
 * History:
 *
 * NOTE: The DIB API were not written to handle OS/2 DIBs, so this
 * function will not save a file if it is not a Windows DIB.
 *
 * History:   Date      Author       Reason
 *            9/15/91   Mark Bader   Taken from DIBVIEW (which was taken
 *                                      from SHOWDIB)
 *            1/30/92   Mark Bader   Fixed problem of writing too many 
 *                                      bytes to the file
 *            6/24/92   Mark Bader   Added check for OS/2 DIB
 *
 *************************************************************************/


WORD FAR SaveDIB(HDIB hDib, LPSTR lpFileName)
{
   BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
   LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
   int fh;     // file handle for opened file
   OFSTRUCT of;     // OpenFile structure
   DWORD dwDIBSize;
   DWORD dwError;   // Error return from MyWrite

   if (!hDib)
      return ERR_INVALIDHANDLE;
   fh = OpenFile(lpFileName, &of, OF_CREATE | OF_READWRITE);
   if (fh == -1)
      return ERR_OPEN;

   /*
    * Get a pointer to the DIB memory, the first of which contains
    * a BITMAPINFO structure
    */
   lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib);
   if (!lpBI)
      return ERR_LOCK;

   // Check to see if we're dealing with an OS/2 DIB.  If so, don't
   // save it because our functions aren't written to deal with these
   // DIBs.

   if (lpBI->biSize != sizeof(BITMAPINFOHEADER))
   {
     GlobalUnlock(hDib);
     return ERR_NOT_DIB;
   }

   /*
    * Fill in the fields of the file header
    */

   /* Fill in file type (first 2 bytes must be "BM" for a bitmap) */
   bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"

   // Calculating the size of the DIB is a bit tricky (if we want to
   // do it right).  The easiest way to do this is to call GlobalSize()
   // on our global handle, but since the size of our global memory may have
   // been padded a few bytes, we may end up writing out a few too
   // many bytes to the file (which may cause problems with some apps,
   // like HC 3.0).
   //
   // So, instead let's calculate the size manually.
   //
   // To do this, find size of header plus size of color table.  Since the
   // first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
   // the size of the structure, let's use this.

   dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  // Partial Calculation

   // Now calculate the size of the image

   if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) {

      // It's an RLE bitmap, we can't calculate size, so trust the
      // biSizeImage field

      dwDIBSize += lpBI->biSizeImage;
      }
   else {
      DWORD dwBmBitsSize;  // Size of Bitmap Bits only

      // It's not RLE, so size is Width (DWORD aligned) * Height

      dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

      dwDIBSize += dwBmBitsSize;
      
      // Now, since we have calculated the correct size, why don't we
      // fill in the biSizeImage field (this will fix any .BMP files which 
      // have this field incorrect).

      lpBI->biSizeImage = dwBmBitsSize;
      }


   // Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)
                   
   bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
   bmfHdr.bfReserved1 = 0;
   bmfHdr.bfReserved2 = 0;

   /*
    * Now, calculate the offset the actual bitmap bits will be in
    * the file -- It's the Bitmap file header plus the DIB header,
    * plus the size of the color table.
    */
   bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize +
                      PaletteSize((LPSTR)lpBI);

   /* Write the file header */
   _lwrite(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));

   /*
    * Write the DIB header and the bits -- use local version of
    * MyWrite, so we can write more than 32767 bytes of data
    */
   dwError = MyWrite(fh, (LPSTR)lpBI, dwDIBSize);
   GlobalUnlock(hDib);
   _lclose(fh);

   if (dwError == 0)
     return ERR_OPEN; // oops, something happened in the write
   else
     return 0; // Success code
}


/*************************************************************************
 *
 * DestroyDIB ()
 *
 * Purpose:  Frees memory associated with a DIB
 *
 * Returns:  Nothing
 *
 * History:   Date      Author       Reason
 *            9/15/91   Mark Bader   Created
 *
 *************************************************************************/


WORD FAR DestroyDIB(HDIB hDib)
{
   GlobalFree(hDib);
   return 0;
}


//************************************************************************
//
// Auxiliary Functions which the above procedures use
//
//************************************************************************


/*************************************************************************
 *
 * Function:  ReadDIBFile (int)
 *
 *  Purpose:  Reads in the specified DIB file into a global chunk of
 *            memory.
 *
 *  Returns:  A handle to a dib (hDIB) if successful.
 *            NULL if an error occurs.
 *
 * Comments:  BITMAPFILEHEADER is stripped off of the DIB.  Everything
 *            from the end of the BITMAPFILEHEADER structure on is
 *            returned in the global memory handle.
 *
 *
 * NOTE: The DIB API were not written to handle OS/2 DIBs, so this
 * function will reject any file that is not a Windows DIB.
 *
 * History:   Date      Author       Reason
 *            9/15/91   Mark Bader   Based on DIBVIEW
 *            6/25/92   Mark Bader   Added check for OS/2 DIB
 *            7/21/92   Mark Bader   Added code to deal with bfOffBits
 *                                     field in BITMAPFILEHEADER      
 *            9/11/92   Mark Bader   Fixed Realloc Code to free original mem
 *
 *************************************************************************/

HANDLE ReadDIBFile(int hFile)
{
   BITMAPFILEHEADER bmfHeader;
   DWORD dwBitsSize;
   UINT nNumColors;   // Number of colors in table
   HANDLE hDIB;        
   HANDLE hDIBtmp;    // Used for GlobalRealloc() //MPB
   LPBITMAPINFOHEADER lpbi;
   DWORD offBits;

   /*
    * get length of DIB in bytes for use when reading
    */

   dwBitsSize = filelength(hFile);

   // Allocate memory for header & color table.	We'll enlarge this
   // memory as needed.

   hDIB = GlobalAlloc(GMEM_MOVEABLE,
       (DWORD)(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)));
   
   if (!hDIB) return NULL;

   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
   if (!lpbi) 
   {
     GlobalFree(hDIB);
     return NULL;
   }

   // read the BITMAPFILEHEADER from our file

   if (sizeof (BITMAPFILEHEADER) != _lread (hFile, (LPSTR)&bmfHeader, sizeof (BITMAPFILEHEADER)))
     goto ErrExit;

   if (bmfHeader.bfType != 0x4d42)	/* 'BM' */
     goto ErrExit;

   // read the BITMAPINFOHEADER

   if (sizeof(BITMAPINFOHEADER) != _lread (hFile, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)))
     goto ErrExit;

   // Check to see that it's a Windows DIB -- an OS/2 DIB would cause
   // strange problems with the rest of the DIB API since the fields
   // in the header are different and the color table entries are
   // smaller.
   //
   // If it's not a Windows DIB (e.g. if biSize is wrong), return NULL.

   if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
     goto ErrExit;

   // Now determine the size of the color table and read it.  Since the
   // bitmap bits are offset in the file by bfOffBits, we need to do some
   // special processing here to make sure the bits directly follow
   // the color table (because that's the format we are susposed to pass
   // back)
   nNumColors = (UINT)lpbi->biClrUsed;
   if (!nNumColors)
    {
      // no color table for 24-bit, default size otherwise
      if (lpbi->biBitCount != 24)
        nNumColors = 1 << lpbi->biBitCount; /* standard size table */
    }

   // fill in some default values if they are zero
   if (lpbi->biClrUsed == 0)
     lpbi->biClrUsed = nNumColors;

   if (lpbi->biSizeImage == 0)
   {
     lpbi->biSizeImage = ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 31) & ~31) >> 3)
			 * lpbi->biHeight;
   }

   // get a proper-sized buffer for header, color table and bits
   GlobalUnlock(hDIB);
   hDIBtmp = GlobalReAlloc(hDIB, lpbi->biSize +
                        nNumColors * sizeof(RGBQUAD) +
                        lpbi->biSizeImage, 0);

   if (!hDIBtmp) // can't resize buffer for loading
     goto ErrExitNoUnlock; //MPB
   else
     hDIB = hDIBtmp;

   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

   // read the color table
   _lread (hFile, (LPSTR)(lpbi) + lpbi->biSize, nNumColors * sizeof(RGBQUAD));

   // offset to the bits from start of DIB header
   offBits = lpbi->biSize + nNumColors * sizeof(RGBQUAD);

   // If the bfOffBits field is non-zero, then the bits might *not* be
   // directly following the color table in the file.  Use the value in
   // bfOffBits to seek the bits.

   if (bmfHeader.bfOffBits != 0L)
      _llseek(hFile, bmfHeader.bfOffBits, SEEK_SET);
   
   if (MyRead(hFile, (LPSTR)lpbi + offBits, lpbi->biSizeImage))
     goto OKExit;


ErrExit:
    GlobalUnlock(hDIB);    
ErrExitNoUnlock:    
    GlobalFree(hDIB);
    return NULL;

OKExit:
    GlobalUnlock(hDIB);
    return hDIB;
}

/*************************************************************************

  Function:  MyRead (int, LPSTR, DWORD)

   Purpose:  Routine to read files greater than 64K in size.

   Returns:  TRUE if successful.
             FALSE if an error occurs.

  
  History:   Date      Author       Reason
             9/15/91   Mark Bader   Based on DIBVIEW
 
*************************************************************************/


BOOL MyRead(int hFile, LPSTR lpBuffer, DWORD dwSize)
{
   char huge *lpInBuf = (char huge *)lpBuffer;
   int nBytes;

   /*
    * Read in the data in 32767 byte chunks (or a smaller amount if it's
    * the last chunk of data read)
    */

   while (dwSize)
   {
      nBytes = (int)(dwSize > (DWORD)32767 ? 32767 : LOWORD (dwSize));
      if (_lread(hFile, (LPSTR)lpInBuf, nBytes) != (WORD)nBytes)
         return FALSE;
      dwSize -= nBytes;
      lpInBuf += nBytes;
   }
   return TRUE;
}


/****************************************************************************

 FUNCTION   : MyWrite(int fh, VOID FAR *pv, DWORD ul)

 PURPOSE    : Writes data in steps of 32k till all the data is written.
              Normal _lwrite uses a WORD as 3rd parameter, so it is
              limited to 32767 bytes, but this procedure is not.

 RETURNS    : 0 - If write did not proceed correctly.
              number of bytes written otherwise.
 
  History:   Date      Author       Reason
             9/15/91   Mark Bader   Based on DIBVIEW

 ****************************************************************************/


DWORD PASCAL MyWrite(int iFileHandle, VOID FAR *lpBuffer, DWORD dwBytes)
{
   DWORD dwBytesTmp = dwBytes;       // Save # of bytes for return value
   BYTE huge *hpBuffer = (BYTE huge *)lpBuffer;   // make a huge pointer to the data

   /*
    * Write out the data in 32767 byte chunks.
    */

   while (dwBytes > 32767)
   {
      if (_lwrite(iFileHandle, (LPSTR)hpBuffer, (WORD)32767) != 32767)
         return 0;
      dwBytes -= 32767;
      hpBuffer += 32767;
   }

   /* Write out the last chunk (which is < 32767 bytes) */
   if (_lwrite(iFileHandle, (LPSTR)hpBuffer, (WORD)dwBytes) != (WORD)dwBytes)
      return 0;
   return dwBytesTmp;
}

//	ajw added
//	Added to allow "loading" from a location in memory.
//	A modification of ReadDIBFile(), above.
//***********************************************************************************************
HDIB LoadDIB_FromMemory( const unsigned char* pData, DWORD dwBitsSize )
{
	BITMAPFILEHEADER bmfHeader;
	UINT nNumColors;   // Number of colors in table
	HANDLE hDIB;        
	HANDLE hDIBtmp;    // Used for GlobalRealloc() //MPB
	LPBITMAPINFOHEADER lpbi;
	DWORD offBits;

	const unsigned char* const pDataStart = pData;
	const unsigned char* pDataEnd = pData + dwBitsSize;		//	One char past end of "file".

	// Allocate memory for header & color table.	We'll enlarge this
	// memory as needed.

//	debugprint( "LoadDIB_FromMemory, GlobalAlloc\n" );
	hDIB = GlobalAlloc(GMEM_MOVEABLE, (DWORD)(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)));
//	debugprint( "hDIB from GlobalALloc is %i\n", hDIB );

	if (!hDIB)
	{
//		debugprint( "LoadDIB_FromMemory error: failed alloc\n" );
		return NULL;
	}

//	debugprint( "LoadDIB_FromMemory, lpbi Lock\n" );
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
//	debugprint( "lpbi is %i\n", lpbi );
	if (!lpbi) 
	{
//		debugprint( "LoadDIB_FromMemory error: failed lock\n" );
		GlobalFree(hDIB);
		return NULL;
	}

	// read the BITMAPFILEHEADER from our file
//	if (sizeof (BITMAPFILEHEADER) != _lread (hFile, (LPSTR)&bmfHeader, sizeof (BITMAPFILEHEADER)))
//		goto ErrExit;
	if( pData + sizeof( BITMAPFILEHEADER ) >= pDataEnd )
	{
//		debugprint( "LoadDIB_FromMemory error: bad size\n" );
		goto ErrExit;
	}
//	debugprint( "LoadDIB_FromMemory, memcpy BITMAPFILEHEADER %i bytes\n", sizeof( BITMAPFILEHEADER ) );
	memcpy( &bmfHeader, pData, sizeof( BITMAPFILEHEADER ) );
	pData += sizeof( BITMAPFILEHEADER );

	if (bmfHeader.bfType != 0x4d42)	/* 'BM' */
	{
//		debugprint( "LoadDIB_FromMemory error: no BM\n" );
		goto ErrExit;
	}

	// read the BITMAPINFOHEADER
//	if (sizeof(BITMAPINFOHEADER) != _lread (hFile, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)))
//		goto ErrExit;
	if( pData + sizeof( BITMAPINFOHEADER ) >= pDataEnd )
	{
//		debugprint( "LoadDIB_FromMemory error: bad size 2\n" );
		goto ErrExit;
	}
//	debugprint( "LoadDIB_FromMemory, memcpy BITMAPINFOHEADER %i bytes\n", sizeof( BITMAPINFOHEADER ) );
	memcpy( lpbi, pData, sizeof( BITMAPINFOHEADER ) );
	pData += sizeof( BITMAPINFOHEADER );

	// Check to see that it's a Windows DIB -- an OS/2 DIB would cause
	// strange problems with the rest of the DIB API since the fields
	// in the header are different and the color table entries are
	// smaller.
	//
	// If it's not a Windows DIB (e.g. if biSize is wrong), return NULL.

	if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
	{
//		debugprint( "LoadDIB_FromMemory error: lpbi->biSize bad\n" );
		goto ErrExit;
	}

	if( lpbi->biCompression != BI_RGB )
	{
//		debugprint( "LoadDIB_FromMemory error: Image is compressed\n" );
		goto ErrExit;
	}

	// Now determine the size of the color table and read it.  Since the
	// bitmap bits are offset in the file by bfOffBits, we need to do some
	// special processing here to make sure the bits directly follow
	// the color table (because that's the format we are susposed to pass
	// back)
	nNumColors = (UINT)lpbi->biClrUsed;
	if (!nNumColors)
	{
		// no color table for 24-bit, default size otherwise
		if (lpbi->biBitCount != 24)
			nNumColors = 1 << lpbi->biBitCount; /* standard size table */
	}

	// fill in some default values if they are zero
	if (lpbi->biClrUsed == 0)
		lpbi->biClrUsed = nNumColors;

//	debugprint( "biSizeImage is %i. I would say it was %i, because the bpp is %i.\n", lpbi->biSizeImage, ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 31) & ~31) >> 3) * lpbi->biHeight, lpbi->biBitCount );
	if (lpbi->biSizeImage == 0)
	{
		lpbi->biSizeImage = ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 31) & ~31) >> 3) * lpbi->biHeight;
	}

	// get a proper-sized buffer for header, color table and bits
	GlobalUnlock(hDIB);
//	debugprint( "LoadDIB_FromMemory, GlobalReAlloc: lpbi->biSize=%i, nNumColors=%i, lpbi->biSizeImage=%i\n", lpbi->biSize, nNumColors,lpbi->biSizeImage );
	hDIBtmp = GlobalReAlloc(hDIB, lpbi->biSize + nNumColors * sizeof(RGBQUAD) + lpbi->biSizeImage, 0);

	if (!hDIBtmp) // can't resize buffer for loading
	{
//		debugprint( "LoadDIB_FromMemory error: realloc failed\n" );
		goto ErrExitNoUnlock; //MPB
	}
	else
		hDIB = hDIBtmp;

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

	// read the color table
//	_lread (hFile, (LPSTR)(lpbi) + lpbi->biSize, nNumColors * sizeof(RGBQUAD));
//	debugprint( "LoadDIB_FromMemory, memcpy color table %i colors, so %i bytes\n", nNumColors, nNumColors * sizeof(RGBQUAD) );
	memcpy( (LPSTR)(lpbi) + lpbi->biSize, pData, nNumColors * sizeof(RGBQUAD) );
	pData += nNumColors * sizeof(RGBQUAD);

	// offset to the bits from start of DIB header
	offBits = lpbi->biSize + nNumColors * sizeof(RGBQUAD);

	// If the bfOffBits field is non-zero, then the bits might *not* be
	// directly following the color table in the file.  Use the value in
	// bfOffBits to seek the bits.

	if (bmfHeader.bfOffBits != 0L)
//		_llseek(hFile, bmfHeader.bfOffBits, SEEK_SET);
		pData = pDataStart + bmfHeader.bfOffBits;

//	debugprint( "bmfHeader.bfOffBits is %i\n", bmfHeader.bfOffBits );

//	if (MyRead(hFile, (LPSTR)lpbi + offBits, lpbi->biSizeImage))
//		goto OKExit;
//	debugprint( "Checking that pData(%i) + biSizeImage(%i), which is %i, is equal to pDataEnd(%i)\n", 
//					pData, lpbi->biSizeImage, pData + lpbi->biSizeImage, pDataEnd );
//	if( pData + lpbi->biSizeImage != pDataEnd )			condition relaxed
//	{
//		debugprint( "LoadDIB_FromMemory error: bad size 3\n" );
//		goto ErrExit;
//	}

//	debugprint( "LoadDIB_FromMemory, memcpy the bits, %i bytes. Image is w %i, h.%i\n", 
//								lpbi->biSizeImage, lpbi->biWidth, lpbi->biHeight );
//	debugprint( "Writing to lpbi (%i) + offBits (%i)\n", lpbi, offBits );

	memcpy( (LPSTR)lpbi + offBits, pData, lpbi->biSizeImage );
//	pData += lpbi->biSizeImage;
//	if( pData != pDataEnd )		//	Should end up one byte past end of data. - condition relaxed
//		debugprint( "LoadDIB_FromMemory: ERROR! Ended up at %i instead of %i\n", pData, pDataEnd );
	goto OKExit;

ErrExit:
	GlobalUnlock(hDIB);
ErrExitNoUnlock:    
	GlobalFree(hDIB);
//	debugprint( "LoadDIB_FromMemory Error!\n" );
	return NULL;

OKExit:
	GlobalUnlock(hDIB);
	return hDIB;
}
#endif