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

/* $Header: /CounterStrike/SPRITE.CPP 1     3/03/97 10:25a Joe_bostic $ */
/**********************************************************************

	Sprite.cpp

	Dec 28,1995

	GraphicBufferClass member functions for blitting, scaling,
	and rotating bitmaps

**********************************************************************/

#ifndef WIN32
class BitmapClass
{
	public:
		BitmapClass(int w, int h, unsigned char * data) :
			Width(w), Height(h), Data(data) {};

		int Width;
		int Height;
		unsigned char * Data;
};

class TPoint2D
{
	public:
		TPoint2D(int xx, int yy) : x(xx), y(yy) {};
		TPoint2D(void) : x(0), y(0) {};

		int x;
		int y;
};
#endif


//#include	"function.h"
#define FILE_H
#define RAWFILE_H
#define WWMEM_H
#define WWFILE_Hx
#include	<wwlib32.h>
#include <stdio.h>
//#include "gbuffer.h"
//#include "math.h"


long _SineTab[256] = {
0 ,6 ,12 , 18 , 25 , 31 , 37 , 43 , 49 , 56 , 62 , 68 , 74 , 80 , 86 ,
92 , 97 , 103 , 109 , 115 , 120 , 126 , 131 , 136 , 142 , 147 , 152 ,
157 , 162 , 167 , 171 , 176 , 180 , 185 , 189 , 193 , 197 , 201 , 205 ,
209 , 212 , 216 , 219 , 222 , 225 , 228 , 231 , 233 , 236 , 238 , 240 ,
243 , 244 , 246 , 248 , 249 , 251 , 252 , 253 , 254 , 254 , 255 , 255 ,
255 , 255 , 255 , 255 , 255 , 254 , 254 , 253 , 252 , 251 , 249 , 248 ,
246 , 245 , 243 , 241 , 238 , 236 , 234 , 231 , 228 , 225 , 222 , 219 ,
216 , 213 , 209 , 205 , 201 , 198 , 194 , 189 , 185 , 181 , 176 , 172 ,
167 , 162 , 157 , 152 , 147 , 142 , 137 , 131 , 126 , 120 , 115 , 109 ,
104 , 98 , 92 , 86 , 80 , 74 , 68 , 62 , 56 , 50 , 44 , 37 , 31 , 25 ,
19 , 12 , 6 , 0 , -5 , -12 , -18 , -24 , -30 , -37 , -43 , -49 , -55 ,
-61 , -67 , -73 , -79 , -85 , -91 , -97 , -103 , -109 , -114 , -120 ,
-125 , -131 , -136 , -141 , -147 , -152 , -157 , -162 , -166 , -171 ,
-176 , -180 , -185 , -189 , -193 , -197 , -201 , -205 , -208 , -212 ,
-215 , -219 , -222 , -225 , -228 , -231 , -233 , -236 , -238 , -240 ,
-242 , -244 , -246 , -248 , -249 , -250 , -252 , -253 , -254 , -254 ,
-255 , -255 , -255 , -255 , -255 , -255 , -255 , -254 , -254 , -253 ,
-252 , -251 , -249 , -248 , -246 , -245 , -243 , -241 , -239 , -236 ,
-234 , -231 , -228 , -226 , -223 , -219 , -216 , -213 , -209 , -206 ,
-202 , -198 , -194 , -190 , -185 , -181 , -177 , -172 , -167 , -162 ,
-158 , -153 , -148 , -142 , -137 , -132 , -126 , -121 , -115 , -110 ,
-104 , -98 , -92 , -86 , -81 , -75 , -69 , -62 , -56 , -50 , -44 ,
-38 , -32 , -25 , -19 , -13 , -7 ,
};

long _CosineTab[256] = {
256 , 255 , 255 , 255 , 254 , 254 , 253 , 252 , 251 , 249 , 248 , 246 ,
244 , 243 , 241 , 238 , 236 , 234 , 231 , 228 , 225 , 222 , 219 , 216 ,
212 , 209 , 205 , 201 , 197 , 193 , 189 , 185 , 181 , 176 , 171 , 167 ,
162 , 157 , 152 , 147 , 142 , 137 , 131 , 126 , 120 , 115 , 109 , 103 ,
98 , 92 , 86 , 80 , 74 , 68 , 62 , 56 , 50 , 43 , 37 , 31 , 25 , 19 ,
12 , 6 , 0 , -6 , -12 , -18 , -24 , -31 , -37 , -43 , -49 , -55 , -61 ,
-68 , -74 , -80 , -86 , -91 , -97 , -103 , -109 , -114 , -120 , -125 , -131 ,
-136 , -141 , -147 , -152 , -157 , -162 , -166 , -171 , -176 , -180 ,
-185 , -189 , -193 , -197 , -201 , -205 , -209 , -212 , -216 , -219 , -222 ,
-225 , -228 , -231 , -233 , -236 , -238 , -240 , -242 , -244 , -246 ,
-248 , -249 , -251 , -252 , -253 , -254 , -254 , -255 , -255 , -255 ,
-255 , -255 , -255 , -255 , -254 , -254 , -253 , -252 , -251 , -249 , -248 ,
-246 , -245 , -243 , -241 , -239 , -236 , -234 , -231 , -228 , -225 ,
-222 , -219 , -216 , -213 , -209 , -205 , -202 , -198 , -194 , -190 , -185 ,
-181 , -176 , -172 , -167 , -162 , -157 , -152 , -147 , -142 , -137 , -132 ,
-126 , -121 , -115 , -109 , -104 , -98 , -92 , -86 , -80 , -74 , -68 ,
-62 , -56 , -50 , -44 , -38 , -31 , -25 , -19 , -13 , -6 , 0 , 5 , 11 ,
18 , 24 , 30 , 36 , 43 , 49 , 55 , 61 , 67 , 73 , 79 , 85 , 91 , 97 , 103 ,
108 , 114 , 120 , 125 , 131 , 136 , 141 , 146 , 151 , 156 , 161 , 166 ,
171 , 176 , 180 , 184 , 189 , 193 , 197 , 201 , 205 , 208 , 212 , 215 ,
219 , 222 , 225 , 228 , 231 , 233 , 236 , 238 , 240 , 242 , 244 , 246 ,
248 , 249 , 250 , 252 , 253 , 253 , 254 , 255 , 255 , 255 ,
};



/***************************************************************
*
*	Scale_Rotate
*
*	FUNCTION:
*
*	Using Bi-Linear Interpolation, draws a scaled and rotated
*	bitmap onto the buffer.  No clipping is performed so beware.
*
*	INPUTS
*
*	bmp		- bitmap to draw
*	pt			- desired position of the center
*	scale		- 24.8 fixed point scale factor
*	angle		- 8bit angle (0=0deg, 255=360deg)
*
***************************************************************/

void GraphicBufferClass::Scale_Rotate(BitmapClass &bmp,TPoint2D const &pt,long scale,unsigned char angle)
{
	unsigned int scrpos;
	unsigned int temp;

	int i,j;			// counter vars
	int pxerror	=0;		// these three vars will be used in an
	int pyerror	=0;		// integer difference alg to keep track
	int pixpos	=0;		// of what pixel to draw.
	unsigned char pixel;

	TPoint2D p0;	// "upper left" corner of the rectangle
	TPoint2D p1;	// "upper right" corner of the rectangle
	TPoint2D p2;	// "lower left" corner of the rectangle

	/*-------------------------------------------------
		Compute three corner points of the rectangle
	-------------------------------------------------*/
	{
		angle &= 0x0FF;
		long c = _CosineTab[angle];
 		long s = _SineTab[angle];
		long W = (scale*bmp.Width)>>1;
		long L = (scale*bmp.Height)>>1;

		p0.x = (pt.x + ((( (L*c) >> 8)-((W*s) >> 8)) >> 8));
		p0.y = (pt.y + (((-(L*s) >> 8)-((W*c) >> 8)) >> 8));
		p1.x = (pt.x + ((( (L*c) >> 8)+((W*s) >> 8)) >> 8));
		p1.y = (pt.y + (((-(L*s) >> 8)+((W*c) >> 8)) >> 8));
		p2.x = (pt.x + (((-(L*c) >> 8)-((W*s) >> 8)) >> 8));
		p2.y = (pt.y + ((( (L*s) >> 8)-((W*c) >> 8)) >> 8));
	}

	/*-----------------------------------
		Initialize Breshnam constants
	-----------------------------------*/

	// This breshnam line goes across the FRONT of the rectangle
	// In the bitmap, this will step from left to right

	int f_deltax	=(p1.x-p0.x);
	int f_deltay	=(p1.y-p0.y);
	int f_error		=0;
	int f_xstep		=1;
	int f_ystep		=Width;

	// This breshnam line goes down the SIDE of the rectangle
	// In the bitmap, this line will step from top to bottom

	int s_deltax	=(p2.x-p0.x);
	int s_deltay	=(p2.y-p0.y);
	int s_error		=0;
	int s_xstep		=1;
	int s_ystep		=Width;

	/*--------------------------------
		fixup deltas and step values
	--------------------------------*/

	if (f_deltay<0) {
		f_deltay=-f_deltay;
		f_ystep=-Width;
	};

	if (f_deltax<0) {
		f_deltax=-f_deltax;
		f_xstep=-1;
	};

	if (s_deltay<0) {
		s_deltay=-s_deltay;
		s_ystep=-Width;
	};

	if (s_deltax<0) {
		s_deltax=-s_deltax;
		s_xstep=-1;
	};

	scrpos=p0.x+Width*p0.y; 	//address of initial screen pos.
	temp=scrpos;

	/*---------------------------------------------------------------------
		Now all of the differences, errors, and steps are set up so we can
		begin drawing the bitmap...

		There are two cases here,
		1 - the "Front" line has a slope of <  1.0 (45 degrees)
		2 - the "Front" line has a slope of >= 1.0

		For case 1, we step along the X direction, for case 2, step in y
	---------------------------------------------------------------------*/

	if (f_deltax>f_deltay) {		//CASE 1, step front in X, side in Y

		// outer loop steps from top to bottom of the rectangle
		for (j=0;j<s_deltay;j++)
		{
			temp=scrpos;

			// The inner loop steps across the rectangle
			for (i=0;i<f_deltax;i++)
			{
				pixel=bmp.Data[pixpos];				//read pixel
				if (pixel) ((unsigned char *)Get_Buffer())[scrpos]=pixel;	//draw if not transparent
//				if (pixel) Data[scrpos]=pixel;	//draw if not transparent
				pxerror+=bmp.Width;		 			//update position in bitmap
				while (pxerror>f_deltax)
				{
					pixpos++;
					pxerror-=f_deltax;
				};
				scrpos+=f_xstep;						//step to next screen pos
				f_error+=f_deltay;
				if (f_error>f_deltax)
				{
					if (pixel) ((unsigned char *)Get_Buffer())[scrpos]=pixel;
					f_error-=f_deltax;
					scrpos+=f_ystep;
				};
			};
			pxerror=0;
			pixpos-=bmp.Width-1;
			pyerror+=bmp.Height;

			while (pyerror>s_deltay)
			{
				pixpos+=bmp.Width;
				pyerror-=s_deltay;
			};

			f_error=0;
			scrpos=temp;
			scrpos+=s_ystep;
			s_error+=s_deltax;

			if (s_error>s_deltay)
			{
				s_error-=s_deltay;
				scrpos+=s_xstep;
			};
		}

	} else {		// CASE 2, Step front line in X, side line in Y

		// outer loop steps from top to bottom of the rectangle
		for (j=0;j<s_deltax;j++)
		{
			temp=scrpos;

			// The inner loop steps across the rectangle
			for (i=0;i<f_deltay;i++)
			{
				pixel=bmp.Data[pixpos];				//read pixel
				if (pixel) ((unsigned char *)Get_Buffer())[scrpos]=pixel;	//draw if not transparent
				pxerror+=bmp.Width;					//update position in bitmap
				while (pxerror>f_deltay)
				{
					pixpos++;
					pxerror-=f_deltay;
				};

				scrpos+=f_ystep;						//step to next screen pos
				f_error+=f_deltax;
				if (f_error>f_deltay)
				{
					if (pixel) ((unsigned char *)Get_Buffer())[scrpos]=pixel;
					f_error-=f_deltay;
					scrpos+=f_xstep;
				};
			};

			pxerror=0;
			pixpos-=bmp.Width-1;
			pyerror+=bmp.Height;
			while (pyerror>s_deltax)
			{
				pixpos+=bmp.Width;
				pyerror-=s_deltax;
			};

			scrpos=temp;
			scrpos+=s_xstep;
			s_error+=s_deltay;
			f_error=0;
			if (s_error>s_deltax)
			{
				s_error-=s_deltax;
				scrpos+=s_ystep;
			};
		}
	}
}