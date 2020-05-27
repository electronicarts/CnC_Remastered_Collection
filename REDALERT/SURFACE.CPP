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

/* $Header: /CounterStrike/SURFACE.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SURFACE.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 09/08/96                                                     *
 *                                                                                             *
 *                  Last Update : September 8, 1996 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if (0)//PG
#include "FUNCTION.H"
#include "surface.h"


Surface::Surface(int w, int h, Buffer const * buffer, int pitch) :
	Width(w),
	Height(h),
	Pitch(pitch)
{
	/*
	**	If a buffer was specified, then this means that the surface will use the buffer memory
	**	and thus not allocate and manage its own memory.
	*/
	if (buffer != NULL) {
		SurfaceData = *buffer;

		/*
		**	Reduce the dimensions if the buffer is not big enough. This size bounding is only
		**	possible if the buffer size is known. Otherwise, presume that it is big enough.
		*/
		if (buffer->Get_Size() > 0 && Get_Size() > buffer->Get_Size()) {

			Height = buffer->Get_Size() / (Width+Pitch);
			if (Height == 0) {
				Height = 1;
				Width = buffer->Get_Size();
			}
		}
	} else {
		/*
		**	A new buffer without existing memory specified, will allocate and manage its
		**	own memory for the surface.
		*/
		new(&SurfaceData) Buffer(Logical_Size());
	}
}


Surface::Surface(Surface const & surface, int x, int y, int w, int h) :
	Width(w),
	Height(h),
	Pitch(surface.Bytes_Per_Line() % w)
{
	new(&SurfaceData) Buffer((char*)surface.Get_Buffer() + y*surface.Bytes_Per_Line() + x);
}


void Surface::Copy_To(Buffer & buffer, int x, int y, int w, int h) const
{
	assert(buffer.Is_Valid());

	/*
	**	Determine the width of the region to copy from this surface.
	*/
	int width = w;
	if (width == -1) {
		width = Width;
	}

	/*
	**	Determine the height of the region to copy from this surface.
	*/
	int height = h;
	if (height == -1) {
		height = Height;
	}

	Copy_To(Rect(x, y, width, height), buffer);
}


void Surface::Copy_To(Rect const & fromrect, Buffer & tobuffer) const
{
	assert(fromrect.Is_Valid());
	assert(tobuffer.Is_Valid());

	/*
	**	Determine the copy-from rectangle. The size is bounded to the source
	**	size of the surface, regardless of what was specified as the source
	**	rectangle.
	*/
	Rect rect = fromrect.Intersect(Rect(0, 0, Width, Height));

	/*
	**	Determine the number of bytes to copy. If this number would be
	**	larger than the size of the destination buffer (presuming the size
	**	of the destination buffer is known), then limit the copy size
	**	to the buffer size.
	*/
	int tocopy = rect.Size();
	if (tobuffer.Get_Size() > 0 && tobuffer.Get_Size() > tocopy) {
		tocopy = tobuffer.Get_Size();
	}

	/*
	**	Determine the source starting byte pointer.
	*/
	char * source = (char*)Get_Buffer() + rect.Y*Bytes_Per_Line() + rect.X;

	/*
	**	Determine the destination buffer pointer. This will always be the
	**	start of the destination buffer object specified.
	*/
	char * dest = tobuffer;

	/*
	**	Determine the working pitch value to use. For full width surface
	**	copies on surfaces that have no inherent pitch, then a very fast
	**	copy-in-one-fast-step operation can be performed.
	*/
	int pitch = Bytes_Per_Line() % rect.Width;
	if (pitch == 0) {
		memmove(dest, source, tocopy);
	} else {

		/*
		**	Copy the source to the destination in line segments.
		*/
		for (int y = 0; y < rect.Height; y++) {
			memmove(dest, source, width);
			dest += rect.Width;
			source += rect.Width + pitch;
		}
	}
}
#endif