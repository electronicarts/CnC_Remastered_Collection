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

//**********************************************************************
//
//  dibutil.c
//
//  Source file for Device-Independent Bitmap (DIB) API.  Provides
//  the following functions:
//
//  CreateDIB()         - Creates new DIB
//  FindDIBBits()       - Sets pointer to the DIB bits
//  DIBWidth()          - Gets the width of the DIB
//  DIBHeight()         - Gets the height of the DIB
//  PaletteSize()       - Calculates the buffer size required by a palette
//  DIBNumColors()      - Calculates number of colors in the DIB's color table
//  CreateDIBPalette()  - Creates a palette from a DIB
//  DIBToBitmap()       - Creates a bitmap from a DIB
//  BitmapToDIB()       - Creates a DIB from a bitmap
//  PalEntriesOnDevice()- Gets the number of palette entries of a device
//  GetSystemPalette()  - Returns a handle to the current system palette
//  AllocRoomForDIB()   - Allocates memory for a DIB
//  ChangeDIBFormat()   - Changes a DIB's BPP and/or compression format
//  ChangeBitmapFormat()- Changes a bitmap to a DIB with specified BPP and
//                        compression format
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
//**********************************************************************
#if (0) // ST - 5/8/2019
/* header files */
#include <windows.h>
#include <assert.h>
#include "dibapi.h"
#include "dibutil.h"
#include <stdio.h>


/*************************************************************************
 *
 * CreateDIB()
 *
 * Parameters:
 *
 * DWORD dwWidth    - Width for new bitmap, in pixels
 * DWORD dwHeight   - Height for new bitmap 
 * WORD  wBitCount  - Bit Count for new DIB (1, 4, 8, or 24)
 *
 * Return Value:
 *
 * HDIB             - Handle to new DIB
 *
 * Description:
 *
 * This function allocates memory for and initializes a new DIB by
 * filling in the BITMAPINFOHEADER, allocating memory for the color
 * table, and allocating memory for the bitmap bits.  As with all
 * HDIBs, the header, colortable and bits are all in one contiguous
 * memory block.  This function is similar to the CreateBitmap() 
 * Windows API.
 *
 * The colortable and bitmap bits are left uninitialized (zeroed) in the
 * returned HDIB.
 *
 *
 * History:   Date      Author              Reason
 *            3/20/92   Mark Bader          Created
 *
 ************************************************************************/

HDIB FAR CreateDIB(DWORD dwWidth, DWORD dwHeight, WORD wBitCount)
{
   BITMAPINFOHEADER bi;         // bitmap header
   LPBITMAPINFOHEADER lpbi;     // pointer to BITMAPINFOHEADER
   DWORD dwLen;                 // size of memory block
   HDIB hDIB;
   DWORD dwBytesPerLine;        // Number of bytes per scanline


   // Make sure bits per pixel is valid
   if (wBitCount <= 1)
      wBitCount = 1;
   else if (wBitCount <= 4)
      wBitCount = 4;
   else if (wBitCount <= 8)
      wBitCount = 8;
   else if (wBitCount <= 24)
      wBitCount = 24;
   else
      wBitCount = 4;  // set default value to 4 if parameter is bogus

   // initialize BITMAPINFOHEADER
   bi.biSize = sizeof(BITMAPINFOHEADER);
   bi.biWidth = dwWidth;         // fill in width from parameter
   bi.biHeight = dwHeight;       // fill in height from parameter
   bi.biPlanes = 1;              // must be 1
   bi.biBitCount = wBitCount;    // from parameter
   bi.biCompression = BI_RGB;    
   bi.biSizeImage = 0;           // 0's here mean "default"
   bi.biXPelsPerMeter = 0;
   bi.biYPelsPerMeter = 0;
   bi.biClrUsed = 0;
   bi.biClrImportant = 0;

   // calculate size of memory block required to store the DIB.  This
   // block should be big enough to hold the BITMAPINFOHEADER, the color
   // table, and the bits

   dwBytesPerLine = WIDTHBYTES(wBitCount * dwWidth);
   dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + (dwBytesPerLine * dwHeight);

   // alloc memory block to store our bitmap
   hDIB = GlobalAlloc(GHND, dwLen);

   // major bummer if we couldn't get memory block
   if (!hDIB)
   {
      return NULL;
   }

   // lock memory and get pointer to it
   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

   // use our bitmap info structure to fill in first part of
   // our DIB with the BITMAPINFOHEADER
   *lpbi = bi;

   // Since we don't know what the colortable and bits should contain,
   // just leave these blank.  Unlock the DIB and return the HDIB.

   GlobalUnlock(hDIB);

   /* return handle to the DIB */
   return hDIB;
}



/*************************************************************************
 *
 * FindDIBBits()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * LPSTR            - pointer to the DIB bits
 *
 * Description:
 *
 * This function calculates the address of the DIB's bits and returns a
 * pointer to the DIB bits.
 *
 * History:   Date      Author              Reason
 *            6/01/91   Garrett McAuliffe   Created
 *            9/15/91   Patrick Schreiber   Added header and comments
 *
 ************************************************************************/


LPSTR FAR FindDIBBits(LPCSTR lpDIB)
{
   return (LPSTR)(lpDIB + *(LPDWORD)lpDIB + PaletteSize(lpDIB));
}


/*************************************************************************
 *
 * DIBWidth()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - width of the DIB
 *
 * Description:
 *
 * This function gets the width of the DIB from the BITMAPINFOHEADER
 * width field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * width field if it is an OS/2-style DIB.
 *
 * History:   Date      Author               Reason
 *            6/01/91   Garrett McAuliffe    Created
 *            9/15/91   Patrick Schreiber    Added header and comments
 *
 ************************************************************************/


DWORD FAR DIBWidth(LPCSTR lpDIB)
{
   LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
   LPBITMAPCOREHEADER lpbmc;  // pointer to an OS/2-style DIB

   /* point to the header (whether Win 3.0 and OS/2) */

   lpbmi = (LPBITMAPINFOHEADER)lpDIB;
   lpbmc = (LPBITMAPCOREHEADER)lpDIB;

   /* return the DIB width if it is a Win 3.0 DIB */
   if (lpbmi->biSize == sizeof(BITMAPINFOHEADER))
      return lpbmi->biWidth;
   else  /* it is an OS/2 DIB, so return its width */
      return (DWORD)lpbmc->bcWidth;
}


/*************************************************************************
 *
 * DIBHeight()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - height of the DIB
 *
 * Description:
 *
 * This function gets the height of the DIB from the BITMAPINFOHEADER
 * height field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * height field if it is an OS/2-style DIB.
 *
 * History:   Date      Author               Reason
 *            6/01/91   Garrett McAuliffe    Created
 *            9/15/91   Patrick Schreiber    Added header and comments
 *
 ************************************************************************/


DWORD FAR DIBHeight(LPCSTR lpDIB)
{
   LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
   LPBITMAPCOREHEADER lpbmc;  // pointer to an OS/2-style DIB

   /* point to the header (whether OS/2 or Win 3.0 */

   lpbmi = (LPBITMAPINFOHEADER)lpDIB;
   lpbmc = (LPBITMAPCOREHEADER)lpDIB;

   /* return the DIB height if it is a Win 3.0 DIB */
   if (lpbmi->biSize == sizeof(BITMAPINFOHEADER))
      return lpbmi->biHeight;
   else  /* it is an OS/2 DIB, so return its height */
      return (DWORD)lpbmc->bcHeight;
}


/*************************************************************************
 *
 * PaletteSize()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function gets the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD (for a
 * Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an OS/2-
 * style DIB).
 *
 * History:   Date      Author             Reason
 *            6/01/91   Garrett McAuliffe  Created
 *            9/15/91   Patrick Schreiber  Added header and comments
 *
 ************************************************************************/


WORD FAR PaletteSize(LPCSTR lpDIB)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpDIB))
      return (WORD FAR)(DIBNumColors(lpDIB) * sizeof(RGBQUAD));
   else
      return (WORD FAR)(DIBNumColors(lpDIB) * sizeof(RGBTRIPLE));
}


/*************************************************************************
 *
 * DIBNumColors()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or OS/2-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 * History:   Date      Author               Reason
 *            6/01/91   Garrett McAuliffe    Created
 *            9/15/91   Patrick Schreiber    Added header and comments
 *
 ************************************************************************/


WORD FAR DIBNumColors(LPCSTR lpDIB)
{
   WORD wBitCount;  // DIB bit count

   /*  If this is a Windows-style DIB, the number of colors in the
    *  color table can be less than the number of bits per pixel
    *  allows for (i.e. lpbi->biClrUsed can be set to some value).
    *  If this is the case, return the appropriate value.
    */

   if (IS_WIN30_DIB(lpDIB))
   {
      DWORD dwClrUsed;

      dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed;
      if (dwClrUsed)
     return (WORD)dwClrUsed;
   }

   /*  Calculate the number of colors in the color table based on
    *  the number of bits per pixel for the DIB.
    */
   if (IS_WIN30_DIB(lpDIB))
      wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
   else
      wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;

   /* return number of colors based on bits per pixel */
   switch (wBitCount)
      {
   case 1:
      return 2;

   case 4:
      return 16;

   case 8:
      return 256;

   default:
      return 0;
      }
}


/*************************************************************************
 *
 * CreateDIBPalette()
 *
 * Parameter:
 *
 * HDIB hDIB        - specifies the DIB
 *
 * Return Value:
 *
 * HPALETTE         - specifies the palette
 *
 * Description:
 *
 * This function creates a palette from a DIB by allocating memory for the
 * logical palette, reading and storing the colors from the DIB's color table
 * into the logical palette, creating a palette from this logical palette,
 * and then returning the palette's handle. This allows the DIB to be
 * displayed using the best possible colors (important for DIBs with 256 or
 * more colors).
 *
 * History:   Date      Author               Reason
 *            6/01/91   Garrett McAuliffe    Created
 *            9/15/91   Patrick Schreiber    Added header and comments
 *
 ************************************************************************/


HPALETTE FAR CreateDIBPalette(HDIB hDIB)
{
   LPLOGPALETTE lpPal;      // pointer to a logical palette
   HANDLE hLogPal;          // handle to a logical palette
   HPALETTE hPal = NULL;    // handle to a palette
   int i, wNumColors;       // loop index, number of colors in color table
   LPSTR lpbi;              // pointer to packed-DIB
   LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
   LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (OS/2)
   BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB

   /* if handle to DIB is invalid, return NULL */

   if (!hDIB)
      return NULL;

   /* lock DIB memory block and get a pointer to it */
   lpbi = (LPSTR)GlobalLock(hDIB);

   /* get pointer to BITMAPINFO (Win 3.0) */
   lpbmi = (LPBITMAPINFO)lpbi;

   /* get pointer to BITMAPCOREINFO (OS/2 1.x) */
   lpbmc = (LPBITMAPCOREINFO)lpbi;

   /* get the number of colors in the DIB */
   wNumColors = DIBNumColors(lpbi);

   /* is this a Win 3.0 DIB? */
   bWinStyleDIB = IS_WIN30_DIB(lpbi);
   if (wNumColors)
   {
      /* allocate memory block for logical palette */
      hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) *
                wNumColors);

      /* if not enough memory, clean up and return NULL */
      if (!hLogPal)
      {
     GlobalUnlock(hDIB);
     return NULL;
      }

      /* lock memory block and get pointer to it */
      lpPal = (LPLOGPALETTE)GlobalLock(hLogPal);

      /* set version and number of palette entries */
      lpPal->palVersion = PALVERSION;
      lpPal->palNumEntries = (WORD)wNumColors;

      /*  store RGB triples (if Win 3.0 DIB) or RGB quads (if OS/2 DIB)
       *  into palette
       */
      for (i = 0; i < wNumColors; i++)
      {
     if (bWinStyleDIB)
     {
        lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
        lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
        lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
        lpPal->palPalEntry[i].peFlags = 0;
     }
     else
     {
        lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
        lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
        lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
        lpPal->palPalEntry[i].peFlags = 0;
     }
      }

      /* create the palette and get handle to it */
      hPal = CreatePalette(lpPal);

      /* if error getting handle to palette, clean up and return NULL */
      if (!hPal)
      {
     GlobalUnlock(hLogPal);
     GlobalFree(hLogPal);
     return NULL;
      }
   }

   /* clean up */
   GlobalUnlock(hLogPal);
   GlobalFree(hLogPal);
   GlobalUnlock(hDIB);

   /* return handle to DIB's palette */
   return hPal;
}


/*************************************************************************
 *
 * DIBToBitmap()
 *
 * Parameters:
 *
 * HDIB hDIB        - specifies the DIB to convert
 *
 * HPALETTE hPal    - specifies the palette to use with the bitmap
 *
 * Return Value:
 *
 * HBITMAP          - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function creates a bitmap from a DIB using the specified palette.
 * If no palette is specified, default is used.
 *
 * NOTE:
 *
 * The bitmap returned from this funciton is always a bitmap compatible
 * with the screen (e.g. same bits/pixel and color planes) rather than
 * a bitmap with the same attributes as the DIB.  This behavior is by
 * design, and occurs because this function calls CreateDIBitmap to
 * do its work, and CreateDIBitmap always creates a bitmap compatible
 * with the hDC parameter passed in (because it in turn calls
 * CreateCompatibleBitmap).
 *
 * So for instance, if your DIB is a monochrome DIB and you call this
 * function, you will not get back a monochrome HBITMAP -- you will
 * get an HBITMAP compatible with the screen DC, but with only 2
 * colors used in the bitmap.
 *
 * If your application requires a monochrome HBITMAP returned for a
 * monochrome DIB, use the function SetDIBits().
 *
 * Also, the DIBpassed in to the function is not destroyed on exit. This
 * must be done later, once it is no longer needed.
 *
 * History:   Date      Author               Reason
 *            6/01/91   Garrett McAuliffe    Created
 *            9/15/91   Patrick Schreiber    Added header and comments
 *            3/27/92   Mark Bader           Added comments about resulting
 *                                           bitmap format
 *
 ************************************************************************/


HBITMAP FAR DIBToBitmap(HDIB hDIB, HPALETTE hPal)
{
   LPSTR lpDIBHdr, lpDIBBits;  // pointer to DIB header, pointer to DIB bits
   HBITMAP hBitmap;            // handle to device-dependent bitmap
   HDC hDC;                    // handle to DC
   HPALETTE hOldPal = NULL;    // handle to a palette

   /* if invalid handle, return NULL */

   if (!hDIB)
      return NULL;

   /* lock memory block and get a pointer to it */
   lpDIBHdr = (LPSTR)GlobalLock(hDIB);

   /* get a pointer to the DIB bits */
   lpDIBBits = FindDIBBits(lpDIBHdr);

   /* get a DC */
   hDC = GetDC(NULL);
   if (!hDC)
   {
      /* clean up and return NULL */
      GlobalUnlock(hDIB);
      return NULL;
   }

   /* select and realize palette */
   if (hPal)
      hOldPal = SelectPalette(hDC, hPal, FALSE);
   RealizePalette(hDC);

   /* create bitmap from DIB info. and bits */
   hBitmap = CreateDIBitmap(hDC, (LPBITMAPINFOHEADER)lpDIBHdr, CBM_INIT,
                lpDIBBits, (LPBITMAPINFO)lpDIBHdr, DIB_RGB_COLORS);

   /* restore previous palette */
   if (hOldPal)
      SelectPalette(hDC, hOldPal, FALSE);

   /* clean up */
   ReleaseDC(NULL, hDC);
   GlobalUnlock(hDIB);

   /* return handle to the bitmap */
   return hBitmap;
}


/*************************************************************************
 *
 * BitmapToDIB()
 *
 * Parameters:
 *
 * HBITMAP hBitmap  - specifies the bitmap to convert
 *
 * HPALETTE hPal    - specifies the palette to use with the bitmap
 *
 * Return Value:
 *
 * HDIB             - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function creates a DIB from a bitmap using the specified palette.
 *
 * History:   Date      Author               Reason
 *            6/01/91   Garrett McAuliffe    Created
 *            9/15/91   Patrick Schreiber    Added header and comments
 *            12/10/91  Patrick Schreiber    Added bits per pixel validation
 *                                           and check GetObject return value
 *
 ************************************************************************/


HDIB FAR BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal)
{
   BITMAP bm;                   // bitmap structure
   BITMAPINFOHEADER bi;         // bitmap header
   BITMAPINFOHEADER FAR *lpbi;  // pointer to BITMAPINFOHEADER
   DWORD dwLen;                 // size of memory block
   HANDLE hDIB, h;              // handle to DIB, temp handle
   HDC hDC;                     // handle to DC
   WORD biBits;                 // bits per pixel

   /* check if bitmap handle is valid */

   if (!hBitmap)
      return NULL;

   /* fill in BITMAP structure, return NULL if it didn't work */
   if (!GetObject(hBitmap, sizeof(bm), (LPSTR)&bm))
      return NULL;

   /* if no palette is specified, use default palette */
   if (hPal == NULL)
      hPal = GetStockObject(DEFAULT_PALETTE);

   /* calculate bits per pixel */
   biBits = (WORD)( bm.bmPlanes * bm.bmBitsPixel );

   /* make sure bits per pixel is valid */
   if (biBits <= 1)
      biBits = 1;
   else if (biBits <= 4)
      biBits = 4;
   else if (biBits <= 8)
      biBits = 8;
   else /* if greater than 8-bit, force to 24-bit */
      biBits = 24;

   /* initialize BITMAPINFOHEADER */
   bi.biSize = sizeof(BITMAPINFOHEADER);
   bi.biWidth = bm.bmWidth;
   bi.biHeight = bm.bmHeight;
   bi.biPlanes = 1;
   bi.biBitCount = biBits;
   bi.biCompression = BI_RGB;
   bi.biSizeImage = 0;
   bi.biXPelsPerMeter = 0;
   bi.biYPelsPerMeter = 0;
   bi.biClrUsed = 0;
   bi.biClrImportant = 0;

   /* calculate size of memory block required to store BITMAPINFO */
   dwLen = bi.biSize + PaletteSize((LPSTR)&bi);

   /* get a DC */
   hDC = GetDC(NULL);

   /* select and realize our palette */
   hPal = SelectPalette(hDC, hPal, FALSE);
   RealizePalette(hDC);

   /* alloc memory block to store our bitmap */
   hDIB = GlobalAlloc(GHND, dwLen);

   /* if we couldn't get memory block */
   if (!hDIB)
   {
      /* clean up and return NULL */
      SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
      return NULL;
   }

   /* lock memory and get pointer to it */
   lpbi = (BITMAPINFOHEADER FAR *)GlobalLock(hDIB);

   /* use our bitmap info. to fill BITMAPINFOHEADER */
   *lpbi = bi;

   /*  call GetDIBits with a NULL lpBits param, so it will calculate the
    *  biSizeImage field for us
    */
   GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, NULL, (LPBITMAPINFO)lpbi,
         DIB_RGB_COLORS);

   /* get the info. returned by GetDIBits and unlock memory block */
   bi = *lpbi;
   GlobalUnlock(hDIB);

   /* if the driver did not fill in the biSizeImage field, make one up */
   if (bi.biSizeImage == 0)
      bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;

   /* realloc the buffer big enough to hold all the bits */
   dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + bi.biSizeImage;
   h = GlobalReAlloc(hDIB, dwLen, 0);
   if (h)
      hDIB = h;
   else
   {
      /* clean up and return NULL */
      GlobalFree(hDIB);
      hDIB = NULL;
      SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
      return NULL;
   }

   /* lock memory block and get pointer to it */
   lpbi = (BITMAPINFOHEADER FAR *)GlobalLock(hDIB);

   /*  call GetDIBits with a NON-NULL lpBits param, and actualy get the
    *  bits this time
    */
   if (GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, (LPSTR)lpbi + (WORD)lpbi
         ->biSize + PaletteSize((LPSTR)lpbi), (LPBITMAPINFO)lpbi,
         DIB_RGB_COLORS) == 0)
   {
      /* clean up and return NULL */
      GlobalUnlock(hDIB);
      hDIB = NULL;
      SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
      return NULL;
   }
   bi = *lpbi;

   /* clean up */
   GlobalUnlock(hDIB);
   SelectPalette(hDC, hPal, TRUE);
   RealizePalette(hDC);
   ReleaseDC(NULL, hDC);

   /* return handle to the DIB */
   return hDIB;
}


/*************************************************************************
 *
 * PalEntriesOnDevice()
 *
 * Parameter:
 *
 * HDC hDC          - device context
 *
 * Return Value:
 *
 * int              - number of palette entries on device
 *
 * Description:
 *
 * This function gets the number of palette entries on the specified device
 *
 * History:   Date      Author               Reason
 *            6/01/91   Garrett McAuliffe    Created
 *            9/15/91   Patrick Schreiber    Added header and comments
 *
 ************************************************************************/


int FAR PalEntriesOnDevice(HDC hDC)
{
   int nColors;  // number of colors

   /*  Find out the number of palette entries on this
    *  device.
    */

   nColors = GetDeviceCaps(hDC, SIZEPALETTE);

   /*  For non-palette devices, we'll use the # of system
    *  colors for our palette size.
    */
   if (!nColors)
      nColors = GetDeviceCaps(hDC, NUMCOLORS);
   assert(nColors);
   return nColors;
}


/*************************************************************************
 *
 * GetSystemPalette()
 *
 * Parameters:
 *
 * None
 *
 * Return Value:
 *
 * HPALETTE         - handle to a copy of the current system palette
 *
 * Description:
 *
 * This function returns a handle to a palette which represents the system
 * palette.  The system RGB values are copied into our logical palette using
 * the GetSystemPaletteEntries function.  
 *
 * History:   
 *            
 *    Date      Author               Reason        
 *    6/01/91   Garrett McAuliffe    Created        
 *    9/15/91   Patrick Schreiber    Added header and comments
 *    12/20/91  Mark Bader           Added GetSystemPaletteEntries call
 *
 ************************************************************************/


HPALETTE FAR GetSystemPalette(void)
{
   HDC hDC;                // handle to a DC
   static HPALETTE hPal = NULL;   // handle to a palette
   HANDLE hLogPal;         // handle to a logical palette
   LPLOGPALETTE lpLogPal;  // pointer to a logical palette
   int nColors;            // number of colors

   /* Find out how many palette entries we want. */

   hDC = GetDC(NULL);
   if (!hDC)
      return NULL;
   nColors = PalEntriesOnDevice(hDC);   // Number of palette entries

   /* Allocate room for the palette and lock it. */
   hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + nColors * sizeof(
             PALETTEENTRY));

   /* if we didn't get a logical palette, return NULL */
   if (!hLogPal)
      return NULL;

   /* get a pointer to the logical palette */
   lpLogPal = (LPLOGPALETTE)GlobalLock(hLogPal);

   /* set some important fields */
   lpLogPal->palVersion = (WORD)PALVERSION;
   lpLogPal->palNumEntries = (WORD)nColors;

   /* Copy the current system palette into our logical palette */

   GetSystemPaletteEntries(hDC, 0, nColors, 
                           (LPPALETTEENTRY)(lpLogPal->palPalEntry));

   /*  Go ahead and create the palette.  Once it's created,
    *  we no longer need the LOGPALETTE, so free it.
    */

   hPal = CreatePalette(lpLogPal);

   /* clean up */
   GlobalUnlock(hLogPal);
   GlobalFree(hLogPal);
   ReleaseDC(NULL, hDC);

   return hPal;
}


/*************************************************************************
 *
 * AllocRoomForDIB()
 *
 * Parameters:
 *
 * BITMAPINFOHEADER - bitmap info header stucture
 *
 * HBITMAP          - handle to the bitmap
 *
 * Return Value:
 *
 * HDIB             - handle to memory block
 *
 * Description:
 *
 *  This routine takes a BITMAPINOHEADER, and returns a handle to global
 *  memory which can contain a DIB with that header.  It also initializes
 *  the header portion of the global memory.  GetDIBits() is used to determine
 *  the amount of room for the DIB's bits.  The total amount of memory
 *  needed = sizeof(BITMAPINFOHEADER) + size of color table + size of bits.
 *
 * History:   Date      Author               Reason
 *            6/01/91   Garrett McAuliffe    Created
 *            12/11/91  Patrick Schreiber    Added header and some comments
 *
 ************************************************************************/

HANDLE AllocRoomForDIB(BITMAPINFOHEADER bi, HBITMAP hBitmap)
{
   DWORD              dwLen;
   HANDLE             hDIB;
   HDC                hDC;
   LPBITMAPINFOHEADER lpbi;
   HANDLE             hTemp;

   /* Figure out the size needed to hold the BITMAPINFO structure
    * (which includes the BITMAPINFOHEADER and the color table).
    */

   dwLen = bi.biSize + PaletteSize((LPSTR) &bi);
   hDIB  = GlobalAlloc(GHND,dwLen);

   /* Check that DIB handle is valid */
   if (!hDIB)
      return NULL;

   /* Set up the BITMAPINFOHEADER in the newly allocated global memory,
    * then call GetDIBits() with lpBits = NULL to have it fill in the
    * biSizeImage field for us.
    */
   lpbi  = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
   *lpbi = bi;

   hDC   = GetDC(NULL);
   GetDIBits(hDC, hBitmap, 0, (WORD) bi.biHeight,
          NULL, (LPBITMAPINFO) lpbi, DIB_RGB_COLORS);
   ReleaseDC(NULL, hDC);

   /* If the driver did not fill in the biSizeImage field,
    * fill it in -- NOTE: this is a bug in the driver!
    */
   if (lpbi->biSizeImage == 0)
      lpbi->biSizeImage = WIDTHBYTES((DWORD)lpbi->biWidth * lpbi->biBitCount) *
              lpbi->biHeight;

   /* Get the size of the memory block we need */
   dwLen = lpbi->biSize + PaletteSize((LPSTR) &bi) + lpbi->biSizeImage;

   /* Unlock the memory block */
   GlobalUnlock(hDIB);

   /* ReAlloc the buffer big enough to hold all the bits */
   hTemp = GlobalReAlloc(hDIB,dwLen,0);
   if (hTemp)
      return hTemp;
   else
      {
      /* Else free memory block and return failure */
      GlobalFree(hDIB);
      return NULL;
      }
}


/*************************************************************************
 *
 * ChangeDIBFormat()
 *
 * Parameter:
 *
 * HDIB             - handle to packed-DIB in memory
 *
 * WORD             - desired bits per pixel
 *
 * DWORD            - desired compression format
 *
 * Return Value:
 *
 * HDIB             - handle to the new DIB if successful, else NULL
 *
 * Description:
 *
 * This function will convert the bits per pixel and/or the compression
 * format of the specified DIB. Note: If the conversion was unsuccessful,
 * we return NULL. The original DIB is left alone. Don't use code like the
 * following:
 *
 *    hMyDIB = ChangeDIBFormat(hMyDIB, 8, BI_RLE4);
 *
 * The conversion will fail, but hMyDIB will now be NULL and the original
 * DIB will now hang around in memory. We could have returned the old
 * DIB, but we wanted to allow the programmer to check whether this
 * conversion succeeded or failed.
 *
 * History:   
 *            
 *   Date      Author             Reason         
 *   6/01/91   Garrett McAuliffe  Created         
 *   12/10/91  Patrick Schreiber  Modified from converting RGB to RLE8        
 *                                  to converting RGB/RLE to RGB/RLE.         
 *                                  Added wBitCount and dwCompression         
 *                                  parameters. Also added header and         
 *                                  comments.         
 *
 ************************************************************************/

HDIB FAR ChangeDIBFormat(HDIB hDIB, WORD wBitCount, DWORD dwCompression)
{
   HDC                hDC;             // Handle to DC
   HBITMAP            hBitmap;         // Handle to bitmap
   BITMAP             Bitmap;          // BITMAP data structure
   BITMAPINFOHEADER   bi;              // Bitmap info header
   LPBITMAPINFOHEADER lpbi;            // Pointer to bitmap info
   HDIB               hNewDIB = NULL;  // Handle to new DIB
   HPALETTE           hPal, hOldPal;   // Handle to palette, prev pal
   WORD               DIBBPP, NewBPP;  // DIB bits per pixel, new bpp
   DWORD              DIBComp, NewComp;// DIB compression, new compression

   /* Check for a valid DIB handle */
   if (!hDIB)
      return NULL;

   /* Get the old DIB's bits per pixel and compression format */
   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
   DIBBPP = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
   DIBComp = ((LPBITMAPINFOHEADER)lpbi)->biCompression;
   GlobalUnlock(hDIB);

   /* Validate wBitCount and dwCompression
    * They must match correctly (i.e., BI_RLE4 and 4 BPP or
    * BI_RLE8 and 8BPP, etc.) or we return failure */
   if (wBitCount == 0)
      {
      NewBPP = DIBBPP;
      if ((dwCompression == BI_RLE4 && NewBPP == 4) ||
      (dwCompression == BI_RLE8 && NewBPP == 8) ||
      (dwCompression == BI_RGB))
     NewComp = dwCompression;
      else
     return NULL;
      }
   else if (wBitCount == 1 && dwCompression == BI_RGB)
      {
      NewBPP = wBitCount;
      NewComp = BI_RGB;
      }
   else if (wBitCount == 4)
      {
      NewBPP = wBitCount;
      if (dwCompression == BI_RGB || dwCompression == BI_RLE4)
     NewComp = dwCompression;
      else
     return NULL;
      }
   else if (wBitCount == 8)
      {
      NewBPP = wBitCount;
      if (dwCompression == BI_RGB || dwCompression == BI_RLE8)
     NewComp = dwCompression;
      else
     return NULL;
      }
   else if (wBitCount == 24 && dwCompression == BI_RGB)
      {
      NewBPP = wBitCount;
      NewComp = BI_RGB;
      }
   else
      return NULL;

   /* Save the old DIB's palette */
   hPal = CreateDIBPalette(hDIB);
   if (!hPal)
      return NULL;

   /* Convert old DIB to a bitmap */
   hBitmap = DIBToBitmap(hDIB, hPal);
   if (!hBitmap)
      {
      DeleteObject(hPal);
      return NULL;
      }

   /* Get info about the bitmap */
   GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);

   /* Fill in the BITMAPINFOHEADER appropriately */
   bi.biSize               = sizeof(BITMAPINFOHEADER);
   bi.biWidth              = Bitmap.bmWidth;
   bi.biHeight             = Bitmap.bmHeight;
   bi.biPlanes             = 1;
   bi.biBitCount           = NewBPP;
   bi.biCompression        = NewComp;
   bi.biSizeImage          = 0;
   bi.biXPelsPerMeter      = 0;
   bi.biYPelsPerMeter      = 0;
   bi.biClrUsed            = 0;
   bi.biClrImportant       = 0;

   /* Go allocate room for the new DIB */
   hNewDIB = AllocRoomForDIB(bi, hBitmap);
   if (!hNewDIB)
      return NULL;

   /* Get a pointer to the new DIB */
   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hNewDIB);

   /* Get a DC and select/realize our palette in it */
   hDC  = GetDC(NULL);
   hOldPal = SelectPalette(hDC, hPal, FALSE);
   RealizePalette(hDC);

   /* Call GetDIBits and get the new DIB bits */
   if (!GetDIBits(hDC, hBitmap, 0, (WORD) lpbi->biHeight,
       (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize((LPSTR)lpbi),
       (LPBITMAPINFO)lpbi, DIB_RGB_COLORS))
      {
      GlobalUnlock(hNewDIB);
      GlobalFree(hNewDIB);
      hNewDIB = NULL;
      }

   /* Clean up and return */
   SelectPalette(hDC, hOldPal, TRUE);
   RealizePalette(hDC);
   ReleaseDC(NULL, hDC);

   if (hNewDIB)
      /* Unlock the new DIB's memory block */
      GlobalUnlock(hNewDIB);

   DeleteObject(hBitmap);
   DeleteObject(hPal);

   return hNewDIB;
}


/*************************************************************************
 *
 * ChangeBitmapFormat()
 *
 * Parameter:
 *
 * HBITMAP          - handle to a bitmap
 *
 * WORD             - desired bits per pixel
 *
 * DWORD            - desired compression format
 *
 * HPALETTE         - handle to palette
 *
 * Return Value:
 *
 * HDIB             - handle to the new DIB if successful, else NULL
 *
 * Description:
 *
 * This function will convert a bitmap to the specified bits per pixel
 * and compression format. The bitmap and it's palette will remain
 * after calling this function.
 *
 * History:   
 *            
 *   Date      Author             Reason         
 *   6/01/91   Garrett McAuliffe  Created         
 *   12/10/91  Patrick Schreiber  Modified from converting RGB to RLE8         
 *                                 to converting RGB/RLE to RGB/RLE.         
 *                                 Added wBitCount and dwCompression         
 *                                 parameters. Also added header and         
 *                                 comments.         
 *   12/11/91  Patrick Schreiber  Destroy old DIB if conversion was         
 *                                 successful.         
 *   12/16/91  Patrick Schreiber  Modified from converting DIB to new         
 *                                 DIB to bitmap to new DIB. Added palette
 *                                 parameter.
 *
 ************************************************************************/

HDIB FAR ChangeBitmapFormat(HBITMAP  hBitmap,
            WORD     wBitCount,
            DWORD    dwCompression,
            HPALETTE hPal)
{
   HDC                hDC;          // Screen DC
   HDIB               hNewDIB=NULL; // Handle to new DIB
   BITMAP             Bitmap;       // BITMAP data structure
   BITMAPINFOHEADER   bi;           // Bitmap info. header
   LPBITMAPINFOHEADER lpbi;         // Pointer to bitmap header
   HPALETTE           hOldPal=NULL; // Handle to palette
   WORD               NewBPP;       // New bits per pixel
   DWORD              NewComp;      // New compression format

   /* Check for a valid bitmap handle */
   if (!hBitmap)
      return NULL;

   /* Validate wBitCount and dwCompression
    * They must match correctly (i.e., BI_RLE4 and 4 BPP or
    * BI_RLE8 and 8BPP, etc.) or we return failure
    */
   if (wBitCount == 0)
      {
      NewComp = dwCompression;
      if (NewComp == BI_RLE4)
     NewBPP = 4;
      else if (NewComp == BI_RLE8)
     NewBPP = 8;
      else /* Not enough info */
     return NULL;
      }
   else if (wBitCount == 1 && dwCompression == BI_RGB)
      {
      NewBPP = wBitCount;
      NewComp = BI_RGB;
      }
   else if (wBitCount == 4)
      {
      NewBPP = wBitCount;
      if (dwCompression == BI_RGB || dwCompression == BI_RLE4)
         NewComp = dwCompression;
      else
         return NULL;
      }
   else if (wBitCount == 8)
      {
      NewBPP = wBitCount;
      if (dwCompression == BI_RGB || dwCompression == BI_RLE8)
         NewComp = dwCompression;
      else
         return NULL;
      }
   else if (wBitCount == 24 && dwCompression == BI_RGB)
      {
      NewBPP = wBitCount;
      NewComp = BI_RGB;
      }
   else
      return NULL;

   /* Get info about the bitmap */
   GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);

   /* Fill in the BITMAPINFOHEADER appropriately */
   bi.biSize               = sizeof(BITMAPINFOHEADER);
   bi.biWidth              = Bitmap.bmWidth;
   bi.biHeight             = Bitmap.bmHeight;
   bi.biPlanes             = 1;
   bi.biBitCount           = NewBPP;
   bi.biCompression        = NewComp;
   bi.biSizeImage          = 0;
   bi.biXPelsPerMeter      = 0;
   bi.biYPelsPerMeter      = 0;
   bi.biClrUsed            = 0;
   bi.biClrImportant       = 0;

   /* Go allocate room for the new DIB */
   hNewDIB = AllocRoomForDIB(bi, hBitmap);
   if (!hNewDIB)
      return NULL;

   /* Get a pointer to the new DIB */
   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hNewDIB);

   /* If we have a palette, get a DC and select/realize it */
   if (hPal)
   {
      hDC  = GetDC(NULL);
      hOldPal = SelectPalette(hDC, hPal, FALSE);
      RealizePalette(hDC);
   }

   /* Call GetDIBits and get the new DIB bits */
   if (!GetDIBits(hDC, hBitmap, 0, (WORD) lpbi->biHeight,
       (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize((LPSTR)lpbi),
       (LPBITMAPINFO)lpbi, DIB_RGB_COLORS))
      {
      GlobalUnlock(hNewDIB);
      GlobalFree(hNewDIB);
      hNewDIB = NULL;
      }

   /* Clean up and return */
   if (hOldPal)
   {
      SelectPalette(hDC, hOldPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
   }

   if (hNewDIB)
      {
      /* Unlock the new DIB's memory block */
      GlobalUnlock(hNewDIB);
      }

   return hNewDIB;
}
#endif