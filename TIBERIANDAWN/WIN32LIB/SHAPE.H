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
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : WWLIB32                                  *
 *                                                                         *
 *                    File Name : SHAPE.H                                  *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : May 25, 1994                             *
 *                                                                         *
 *                  Last Update : September 14, 1994   [IML]               *
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef SHAPE_H
#define SHAPE_H

#ifndef GBUFFER_H
#include "gbuffer.h"
#endif
/*
*********************************** Types ***********************************
*/
/*
--------------------------- Shape creation flags ----------------------------
*/
typedef enum {
	MAKESHAPE_NORMAL	 = 0x0000,	// 256-color compressed shape
	MAKESHAPE_COMPACT  = 0x0001,	// 16-color shape (with built-in color table)
	MAKESHAPE_NOCOMP	 = 0x0002,	// Uncompressed shape
	MAKESHAPE_VARIABLE =	0x0004	// <16-color shape
} MakeShapeFlags_Type;

inline MakeShapeFlags_Type operator|(MakeShapeFlags_Type a, MakeShapeFlags_Type b)
{return static_cast<MakeShapeFlags_Type>(static_cast<int>(a) | static_cast<int>(b));}

inline MakeShapeFlags_Type operator&(MakeShapeFlags_Type a, MakeShapeFlags_Type b)
{return static_cast<MakeShapeFlags_Type>(static_cast<int>(a) & static_cast<int>(b));}

inline MakeShapeFlags_Type operator~(MakeShapeFlags_Type a)
{return static_cast<MakeShapeFlags_Type>(~static_cast<int>(a));}


/*---------------------------------------------------------------------------
Shape drawing flags:
- The low byte is for coordinate transformations.
- The high byte is for drawing effects.
---------------------------------------------------------------------------*/
typedef enum {
	SHAPE_NORMAL 			= 0x0000,		// Standard shape
	SHAPE_HORZ_REV 		= 0x0001,		// Flipped horizontally
	SHAPE_VERT_REV 		= 0x0002,		// Flipped vertically
	SHAPE_SCALING 			= 0x0004,		// Scaled (WORD scale_x, WORD scale_y)
	SHAPE_VIEWPORT_REL 	= 0x0010,		// Coords are window-relative
	SHAPE_WIN_REL 			= 0x0010,		// Coordinates are window relative instead of absolute.
	SHAPE_CENTER 			= 0x0020,		// Coords are based on shape's center pt
	SHAPE_BOTTOM			= 0x0040,		// Y coord is based on shape's bottom pt
	SHAPE_FADING 			= 0x0100,		// Fading effect (void * fading_table,
													//  WORD fading_num)
	SHAPE_PREDATOR 		= 0x0200,		// Transparent warping effect
	SHAPE_COMPACT 			= 0x0400,		// Never use this bit
	SHAPE_PRIORITY 		= 0x0800,		// Use priority system when drawing
	SHAPE_GHOST				= 0x1000,		// Shape is drawn ghosted
	SHAPE_SHADOW			= 0x2000,
	SHAPE_PARTIAL  		= 0x4000,
	SHAPE_COLOR 			= 0x8000			// Remap the shape's colors
													//  (void * color_table)
} ShapeFlags_Type;

inline ShapeFlags_Type operator|(ShapeFlags_Type a, ShapeFlags_Type b)
{return static_cast<ShapeFlags_Type>(static_cast<int>(a) | static_cast<int>(b));}

inline ShapeFlags_Type operator&(ShapeFlags_Type a, ShapeFlags_Type b)
{return static_cast<ShapeFlags_Type>(static_cast<int>(a) & static_cast<int>(b));}

inline ShapeFlags_Type operator~(ShapeFlags_Type a)
{return static_cast<ShapeFlags_Type>(~static_cast<int>(a));}


/*
------------------------------- Shape header --------------------------------
*/
typedef struct {
	unsigned short		ShapeType;			// 0 = normal, 1 = 16 colors,
										//  2 = uncompressed, 4 = 	<16 colors
	unsigned char		Height;				// Height of the shape in scan lines
	unsigned short		Width;				// Width of the shape in bytes
	unsigned char		OriginalHeight;	// Original height of shape in scan lines
	unsigned short		ShapeSize;			// Size of the shape, including header
	unsigned short		DataLength;			// Size of the uncompressed shape (just data)
	unsigned char		Colortable[16];	// Optional color table for compact shape
} Shape_Type;

/*
------------------------------- Shape block ---------------------------------
*/
//#pragma warning (push, 4200)
#pragma warning (disable : 4200)
typedef struct {
	unsigned short		NumShapes;			// number of shapes in the block
	long		Offsets[];			// array of offsets to shape data
										//  (offsets within the shape block, with
										//  0 being the first offset value, not the
										//  start of the shape block)
} ShapeBlock_Type;
//#pragma warning (pop)

/*
******************************** Prototypes *********************************
*/

/*
-------------------------------- prioinit.c ---------------------------------
*/

extern "C" {
extern void  *MaskPage;
extern void  *BackGroundPage;
extern long  _ShapeBufferSize;
extern char  *_ShapeBuffer;
}


void __cdecl Init_Priority_System (GraphicBufferClass *mask,
											GraphicBufferClass *back);


/*
-------------------------------- drawshp.asm --------------------------------
*/

extern "C" {
int Draw_Shape(GraphicViewPortClass *gvp, void const *shape, LONG x, LONG y, LONG flags, ...);
}

/*
---------------------------------- shape.c ----------------------------------
*/
short __cdecl Get_Shape_Data(void const *shape, int data);
int __cdecl Extract_Shape_Count(void const *buffer);
void * __cdecl Extract_Shape(void const *buffer, int shape);
int __cdecl Restore_Shape_Height(void *shape);
int __cdecl Set_Shape_Height(void const *shape, int newheight);

extern "C" {
int __cdecl Get_Shape_Width(void const *shape);
int __cdecl Get_Shape_Height(void const *shape);
int __cdecl Get_Shape_Original_Height(void const *shape);
int __cdecl Get_Shape_Uncomp_Size(void const *shape);
}


/*
------------------------------- setshape.asm --------------------------------
*/
extern "C" {
void __cdecl Set_Shape_Buffer(void const *buffer, int size);
}
/*
------------------------------- shapeinf.asm --------------------------------
*/
int __cdecl Get_Shape_Flags(void const *shape);
int  __cdecl Get_Shape_Size(void const *shape);
int  __cdecl Get_Shape_Scaled_Width(void const *shape, int scale);
int  __cdecl Get_Shape_Scaled_Height(void const *shape, int scale);

#endif // SHAPE_H

/****************************** End of shape.h *****************************/

