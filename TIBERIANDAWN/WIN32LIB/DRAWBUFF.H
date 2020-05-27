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


#ifndef DRAWBUFF_H
#define DRAWBUFF_H


#ifndef WWSTD_H
#include "wwstd.h"
#endif

class GraphicViewPortClass;
class GraphicBufferClass;
/*=========================================================================*/
/* Define functions which have not under-gone name mangling						*/
/*=========================================================================*/

extern "C" {
	/*======================================================================*/
	/* Externs for all of the common functions between the video buffer		*/
	/*		class and the graphic buffer class.											*/
	/*======================================================================*/
	 long __cdecl Buffer_Size_Of_Region(void *thisptr, int w, int h);

	 void	__cdecl Buffer_Put_Pixel(void * thisptr, int x, int y, unsigned char color);
	 int	__cdecl Buffer_Get_Pixel(void * thisptr, int x, int y);
	 void	__cdecl Buffer_Clear(void *thisptr, unsigned char color);
	 long	__cdecl Buffer_To_Buffer(void *thisptr, int x, int y, int w, int h, void *buff, long size);
	 long __cdecl Buffer_To_Page(int x, int y, int w, int h, void *Buffer, void *view);
	 BOOL	__cdecl Linear_Blit_To_Linear(	void *thisptr, void * dest, int x_pixel, int y_pixel, int dx_pixel,
			 										int dy_pixel, int pixel_width, int pixel_height, BOOL trans);
	 BOOL __cdecl Linear_Scale_To_Linear(	void *, void *, int, int, int, int, int, int, int, int, BOOL, char *);

	 LONG __cdecl Buffer_Print(void *thisptr, const char *str, int x, int y, int fcolor, int bcolor);

	/*======================================================================*/
	/* Externs for all of the graphic buffer class only functions				*/
	/*======================================================================*/
	 VOID __cdecl Buffer_Draw_Line(void *thisptr, int sx, int sy, int dx, int dy, unsigned char color);
	 VOID __cdecl Buffer_Fill_Rect(void *thisptr, int sx, int sy, int dx, int dy, unsigned char color);
	 VOID __cdecl Buffer_Remap(void * thisptr, int sx, int sy, int width, int height, void *remap);
	 VOID __cdecl Buffer_Fill_Quad(void * thisptr, VOID *span_buff, int x0, int y0, int x1, int y1,
							 	int x2, int y2, int x3, int y3, int color);
	 void __cdecl Buffer_Draw_Stamp(void const *thisptr, void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap);
	 void __cdecl Buffer_Draw_Stamp_Clip(void const *thisptr, void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap, int ,int,int,int);
	 void * __cdecl Get_Font_Palette_Ptr ( void );
}

extern GraphicViewPortClass *LogicPage;
extern BOOL AllowHardwareBlitFills;
#endif
