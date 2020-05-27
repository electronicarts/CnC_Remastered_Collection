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

/* $Header: /CounterStrike/COORD.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : COORD.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : July 22, 1996 [JLB]                                          *
 *                                                                                             *
 * Support code to handle the coordinate system is located in this module.                     *
 * Routines here will be called QUITE frequently during play and must be                       *
 * as efficient as possible.                                                                   *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Cardinal_To_Fixed -- Converts cardinal numbers into a fixed point number.                 *
 *   Coord_Cell -- Convert a coordinate into a cell number.                                    *
 *   Coord_Move -- Moves a coordinate an arbitrary direction for an arbitrary distance         *
 *   Coord_Scatter -- Determines a random coordinate from an anchor point.                     *
 *   Coord_Spillage_List -- Calculate a spillage list for the dirty rectangle specified.       *
 *   Coord_Spillage_List -- Determines the offset list for cell spillage/occupation.           *
 *   Distance -- Determines the cell distance between two cells.                               *
 *   Distance -- Determines the lepton distance between two coordinates.                       *
 *   Distance -- Fetch distance between two target values.                                     *
 *   Fixed_To_Cardinal -- Converts a fixed point number into a cardinal number.                *
 *   Normal_Move_Point -- Moves point with tilt compensation.                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***********************************************************************************************
 * Coord_Cell -- Convert a coordinate into a cell number.                                      *
 *                                                                                             *
 *    This routine will convert the specified coordinate value into a cell number. This is     *
 *    useful to determine the map index number into the cell array that corresponds to a       *
 *    particular coordinate.                                                                   *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to convert into a cell number.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the cell number that corresponds to the coordinate specified.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL Coord_Cell(COORDINATE coord)
{
	CELL_COMPOSITE cell;
	cell.Cell = 0;
	cell.Sub.X = ((COORD_COMPOSITE &)coord).Sub.X.Sub.Cell;
	cell.Sub.Y = ((COORD_COMPOSITE &)coord).Sub.Y.Sub.Cell;
	return(cell.Cell);
//	return(XY_Cell(((COORD_COMPOSITE)coord).Sub.X, ((COORD_COMPOSITE)composite).Sub.Y));
}


/***********************************************************************************************
 * Distance -- Fetch distance between two target values.                                       *
 *                                                                                             *
 *    This routine will determine the lepton distance between the two specified target         *
 *    values.                                                                                  *
 *                                                                                             *
 * INPUT:   target1  -- First target value.                                                    *
 *                                                                                             *
 *          target2  -- Second target value.                                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the lepton distance between the two target values.                    *
 *                                                                                             *
 * WARNINGS:   Be sure that the targets are legal before calling this routine. Otherwise, the  *
 *             return value is meaningless.                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int Distance(TARGET target1, TARGET target2)
{
	return(Distance(As_Coord(target1), As_Coord(target2)));
}


/***********************************************************************************************
 * Distance -- Determines the lepton distance between two coordinates.                         *
 *                                                                                             *
 *    This routine is used to determine the distance between two coordinates. It uses the      *
 *    Dragon Strike method of distance determination and thus it is very fast.                 *
 *                                                                                             *
 * INPUT:   coord1   -- First coordinate.                                                      *
 *                                                                                             *
 *          coord2   -- Second coordinate.                                                     *
 *                                                                                             *
 * OUTPUT:  Returns the lepton distance between the two coordinates.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int Distance(COORDINATE coord1, COORDINATE coord2)
{
	int	diff1, diff2;

	diff1 = Coord_Y(coord1) - Coord_Y(coord2);
	if (diff1 < 0) diff1 = -diff1;
	diff2 = Coord_X(coord1) - Coord_X(coord2);
	if (diff2 < 0) diff2 = -diff2;
	if (diff1 > diff2) {
		return(diff1 + ((unsigned)diff2 / 2));
	}
	return(diff2 + ((unsigned)diff1 / 2));
}


/***********************************************************************************************
 * Coord_Spillage_List -- Determines the offset list for cell spillage/occupation.             *
 *                                                                                             *
 *    This routine will take an arbitrary position and object size and return with a list of   *
 *    cell offsets from the current cell for all cells that are overlapped by the object. The  *
 *    first cell offset is always zero, so to just get the adjacent spill cell list, add one   *
 *    to the return pointer.                                                                   *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to examine.                                                *
 *                                                                                             *
 *          maxsize -- The maximum width/height of the object (pixels).                        *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a spillage list.                                         *
 *                                                                                             *
 * WARNINGS:   The algorithm is limited to working with a maxsize of 48 or less. Larger values *
 *             will generate an incomplete overlap list.                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/06/1993 JLB : Created.                                                                 *
 *   03/25/1994 JLB : Added width optimization.                                                *
 *   04/29/1994 JLB : Converted to C.                                                          *
 *   06/03/1994 JLB : Converted to general purpose spillage functionality.                     *
 *   01/07/1995 JLB : Manually calculates spillage list for large objects.                     *
 *=============================================================================================*/
short const * Coord_Spillage_List(COORDINATE coord, int maxsize)
{
	static short const _MoveSpillage[(int)FACING_COUNT+1][5] = {
		{0, -MAP_CELL_W, REFRESH_EOL, 0, 0},						// N
		{0, -MAP_CELL_W, 1, -(MAP_CELL_W-1), REFRESH_EOL},		// NE
		{0, 1, REFRESH_EOL, 0, 0},										// E
		{0, 1, MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL},			// SE
		{0, MAP_CELL_W, REFRESH_EOL, 0, 0},							// S
		{0, -1, MAP_CELL_W, MAP_CELL_W-1, REFRESH_EOL},			// SW
		{0, -1, REFRESH_EOL, 0, 0},									// W
		{0, -1, -MAP_CELL_W, -(MAP_CELL_W+1), REFRESH_EOL},	// NW
		{0, REFRESH_EOL, 0, 0, 0}										// non-moving.
	};
	static short _manual[10];
//;	00 = on axis
//;	01 = below axis
//;	10 = above axis
//;	11 = undefined
	static signed char const _SpillTable[16]	= {8,6,2,-1,0,7,1,-1,4,5,3,-1,-1,-1,-1,-1};
	int	index=0;
	int	x,y;

	/*
	**	For mondo-enourmo-gigundo objects, use a prebuilt mammoth table
	**	that covers a 5x5 square region.
	*/
	if (maxsize > ICON_PIXEL_W * 2) {
		static short const _gigundo[] = {
			-((2*MAP_CELL_W)-2),-((2*MAP_CELL_W)-1),-((2*MAP_CELL_W)),-((2*MAP_CELL_W)+1),-((2*MAP_CELL_W)+2),
			-((1*MAP_CELL_W)-2),-((1*MAP_CELL_W)-1),-((1*MAP_CELL_W)),-((1*MAP_CELL_W)+1),-((1*MAP_CELL_W)+2),
			-((0*MAP_CELL_W)-2),-((0*MAP_CELL_W)-1),-((0*MAP_CELL_W)),-((0*MAP_CELL_W)+1),-((0*MAP_CELL_W)+2),
			((1*MAP_CELL_W)-2),((1*MAP_CELL_W)-1),((1*MAP_CELL_W)),((1*MAP_CELL_W)+1),((1*MAP_CELL_W)+2),
			+((2*MAP_CELL_W)-2),+((2*MAP_CELL_W)-1),+((2*MAP_CELL_W)),+((2*MAP_CELL_W)+1),+((2*MAP_CELL_W)+2),
			REFRESH_EOL
		};
		return(&_gigundo[0]);
	}

	/*
	**	For very large objects, build the overlap list by hand. This is time consuming, but
	**	not nearly as time consuming as drawing even a single cell unnecessarily.
	*/
	if (maxsize > ICON_PIXEL_W) {
		maxsize = min(maxsize, (ICON_PIXEL_W*2))/2;

		x = (ICON_PIXEL_W * Coord_XLepton(coord)) / ICON_LEPTON_W;
		y = (ICON_PIXEL_H * Coord_YLepton(coord)) / ICON_LEPTON_H;
		int left = x-maxsize;
		int right = x+maxsize;
		int top = y-maxsize;
		int bottom = y+maxsize;

		_manual[index++] = 0;
		if (left < 0) _manual[index++] = -1;
		if (right >= ICON_PIXEL_W) _manual[index++] = 1;
		if (top < 0) _manual[index++] = -MAP_CELL_W;
		if (bottom >= ICON_PIXEL_H) _manual[index++] = MAP_CELL_W;
		if (left < 0 && top < 0) _manual[index++] = -(MAP_CELL_W+1);
		if (right >= ICON_PIXEL_W && bottom >= ICON_PIXEL_H) _manual[index++] = MAP_CELL_W+1;
		if (left < 0 && bottom >= ICON_PIXEL_H) _manual[index++] = MAP_CELL_W-1;
		if (right >= ICON_PIXEL_H && top < 0) _manual[index++] = -(MAP_CELL_W-1);
		_manual[index] = REFRESH_EOL;
		return(&_manual[0]);
	}

	/*
	**	Determine the number of leptons "leeway" allowed this unit.
	*/
	int posval = Pixel2Lepton[(ICON_PIXEL_W-maxsize)/2];

	x = Coord_XLepton(coord) - 0x0080;
	y = Coord_YLepton(coord) - 0x0080;
	if (y > posval) index |= 0x08;			// Spilling South.
	if (y < -posval) index |= 0x04;			// Spilling North.
	if (x > posval) index |= 0x02;			// Spilling East.
	if (x < -posval) index |= 0x01;			// Spilling West.

	return(&_MoveSpillage[_SpillTable[index]][0]);
}


/***********************************************************************************************
 * Coord_Spillage_List -- Calculate a spillage list for the dirty rectangle specified.         *
 *                                                                                             *
 *    Given a center coordinate and a dirty rectangle, calcuate a cell offset list for         *
 *    determining such things as overlap and redraw logic. Optionally, the center cell         *
 *    location will not be part of the list.                                                   *
 *                                                                                             *
 * INPUT:   coord -- The center coordinate that the dirty rectangle is based off of.           *
 *                                                                                             *
 *          rect  -- Reference to the dirty rectangle.                                         *
 *                                                                                             *
 *          nocenter -- If true, then the center cell offset will not be part of the spillage  *
 *                      list returned. This is handy when the center cell is known to be       *
 *                      processed by some other method and it can be safely and efficiently    *
 *                      ignored by the list generated.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the spillage list that corresponds to the data           *
 *          specified. This is a pointer to a static buffer and as such it will only be valid  *
 *          until the next time that this routine is called.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * Coord_Spillage_List(COORDINATE coord, Rect const & rect, bool nocenter)
{
	if (!rect.Is_Valid()) {
		static short const _list[] = {REFRESH_EOL};
		return(_list);
	}

	CELL coordcell = Coord_Cell(coord);
	LEPTON x = Coord_X(coord);
	LEPTON y = Coord_Y(coord);

	/*
	**	Add the rectangle values to the coordinate in order to normalize the start and end
	**	corners of the rectangle. The values are now absolute to the real game world rather
	**	than relative to the coordinate.
	*/
	LEPTON_COMPOSITE startx;
	LEPTON_COMPOSITE starty;
	LEPTON_COMPOSITE endx;
	LEPTON_COMPOSITE endy;
	startx.Raw = (int)x + (short)Pixel_To_Lepton(rect.X);
	starty.Raw = (int)y + (short)Pixel_To_Lepton(rect.Y);
	endx.Raw = startx.Raw + Pixel_To_Lepton(rect.Width-1);
	endy.Raw = starty.Raw + Pixel_To_Lepton(rect.Height-1);

	/*
	**	Determine the upper left and lower right cell indexes. This is a simple conversion from
	**	their lepton counterpart. These cells values are used to form the bounding box for the
	**	map offset list.
	*/
	int cellx = startx.Sub.Cell;
	int cellx2 = endx.Sub.Cell;
	int celly = starty.Sub.Cell;
	int celly2 = endy.Sub.Cell;

	/*
	**	Generate the spillage list by counting off the rows and colums of the cells
	**	that are affected. This is easy since the upper left and lower right corner cells
	**	are known.
	*/
	int count = 0;
	static short _spillagelist[128];
	short * ptr = _spillagelist;
	for (int yy = celly; yy <= celly2; yy++) {
		for (int xx = cellx; xx <= cellx2; xx++) {
			short offset = (XY_Cell(xx, yy) - coordcell);
			if (!nocenter || offset != 0) {
				*ptr++ = offset;
				count++;
				if (count+2 >= ARRAY_SIZE(_spillagelist)) break;
			}
		}
		if (count+2 >= ARRAY_SIZE(_spillagelist)) break;
	}

	/*
	**	Cap the list with the end of list marker and then return a pointer
	**	to the completed list.
	*/
	*ptr = REFRESH_EOL;
	return(_spillagelist);
}


/***********************************************************************************************
 * Coord_Move -- Moves a coordinate an arbitrary direction for an arbitrary distance           *
 *                                                                                             *
 *    This function will move a coordinate in a using SIN and COS arithmetic.                  *
 *                                                                                             *
 * INPUT:   start    -- The starting coordinate.                                               *
 *                                                                                             *
 *          dir      -- The direction to move the coordinate.                                  *
 *                                                                                             *
 *          distance -- The distance to move the coordinate position (in leptons).             *
 *                                                                                             *
 * OUTPUT:  Returns the new coordinate position.                                               *
 *                                                                                             *
 * WARNINGS:   This routine uses multiplies -- use with caution.                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE Coord_Move(COORDINATE start, register DirType dir, unsigned short distance)
{
#ifdef NEVER
	short x = Coord_X(start);
	short y = Coord_Y(start);

	Move_Point(x, y, dir, distance);
	return(XY_Coord(x,y));
#endif

	Move_Point(*(short *)&start, *(((short *)&start)+1), dir, distance);
	return(start);
}


/***********************************************************************************************
 * Coord_Scatter -- Determines a random coordinate from an anchor point.                       *
 *                                                                                             *
 *    This routine will perform a scatter algorithm on the specified                           *
 *    anchor point in order to return with another coordinate that is                          *
 *    randomly nearby the original. Typical use of this would be for                           *
 *    missile targeting.                                                                       *
 *                                                                                             *
 * INPUT:   coord    -- This is the anchor coordinate.                                         *
 *                                                                                             *
 *          distance -- This is the distance in pixels that the scatter                        *
 *                      should fall within.                                                    *
 *                                                                                             *
 *          lock     -- bool; Convert the new coordinate into a center                         *
 *                      cell based coordinate?                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with a new coordinate that is nearby the original.                         *
 *                                                                                             *
 * WARNINGS:   Maximum pixel scatter distance is 255.                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/01/1992 JLB : Created.                                                                 *
 *   05/13/1992 JLB : Only uses Random().                                                      *
 *=============================================================================================*/
COORDINATE Coord_Scatter(COORDINATE coord, unsigned distance, bool lock)
{
	COORDINATE	newcoord;

	newcoord = Coord_Move(coord, Random_Pick(DIR_N, DIR_MAX), distance);

	if (newcoord & HIGH_COORD_MASK) newcoord = coord;

	if (lock) {
		newcoord = Coord_Snap(newcoord);
	}

	return(newcoord);
}


int __cdecl calcx(signed short param1, short distance)
{
	__asm {

		//#pragma aux calcx parm [ax] [bx] \
			
		movzx	eax, [param1]
		mov	bx, [distance]
		imul 	bx
		shl	ax, 1
		rcl	dx, 1
		mov	al, ah
		mov	ah, dl
		cwd
	}
}


int __cdecl calcy(signed short param1, short distance)
{
	__asm {

		//#pragma aux calcy parm [ax] [bx] \
			
		movzx	eax, [param1]
		mov	bx, [distance]
		imul bx
		shl	ax, 1
		rcl	dx, 1
		mov	al, ah
		mov	ah, dl
		cwd
		neg	eax
	}
}


#if (0)
extern int calcx(signed short, short distance);
#pragma aux calcx parm [ax] [bx] \
	modify [eax dx] \
	value [eax]		= 				\
	"imul bx"						\
	"shl	ax,1"						\
	"rcl	dx,1"						\
	"mov	al,ah"					\
	"mov	ah,dl"					\
	"cwd"								\
//	"and	eax,0FFFFh";

extern int calcy(signed short, short distance);
#pragma aux calcy parm [ax] [bx] \
	modify [eax dx] \
	value [eax]		= 				\
	"imul bx"						\
	"shl	ax,1"						\
	"rcl	dx,1"						\
	"mov	al,ah"					\
	"mov	ah,dl"					\
	"cwd"								\
	"neg	eax";
//	"and	eax,0FFFFh"				\

#endif

void Move_Point(short &x, short &y, register DirType dir, unsigned short distance)
{
	static unsigned char const CosTable[256] = {
		0x00,0x03,0x06,0x09,0x0c,0x0f,0x12,0x15,
		0x18,0x1b,0x1e,0x21,0x24,0x27,0x2a,0x2d,
		0x30,0x33,0x36,0x39,0x3b,0x3e,0x41,0x43,
		0x46,0x49,0x4b,0x4e,0x50,0x52,0x55,0x57,
		0x59,0x5b,0x5e,0x60,0x62,0x64,0x65,0x67,
		0x69,0x6b,0x6c,0x6e,0x6f,0x71,0x72,0x74,
		0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7b,
		0x7c,0x7d,0x7d,0x7e,0x7e,0x7e,0x7e,0x7e,

		0x7f,0x7e,0x7e,0x7e,0x7e,0x7e,0x7d,0x7d,
		0x7c,0x7b,0x7b,0x7a,0x79,0x78,0x77,0x76,
		0x75,0x74,0x72,0x71,0x70,0x6e,0x6c,0x6b,
		0x69,0x67,0x66,0x64,0x62,0x60,0x5e,0x5b,
		0x59,0x57,0x55,0x52,0x50,0x4e,0x4b,0x49,
		0x46,0x43,0x41,0x3e,0x3b,0x39,0x36,0x33,
		0x30,0x2d,0x2a,0x27,0x24,0x21,0x1e,0x1b,
		0x18,0x15,0x12,0x0f,0x0c,0x09,0x06,0x03,

		0x00,0xfd,0xfa,0xf7,0xf4,0xf1,0xee,0xeb,
		0xe8,0xe5,0xe2,0xdf,0xdc,0xd9,0xd6,0xd3,
		0xd0,0xcd,0xca,0xc7,0xc5,0xc2,0xbf,0xbd,
		0xba,0xb7,0xb5,0xb2,0xb0,0xae,0xab,0xa9,
		0xa7,0xa5,0xa2,0xa0,0x9e,0x9c,0x9a,0x99,
		0x97,0x95,0x94,0x92,0x91,0x8f,0x8e,0x8c,
		0x8b,0x8a,0x89,0x88,0x87,0x86,0x85,0x85,
		0x84,0x83,0x83,0x82,0x82,0x82,0x82,0x82,

		0x82,0x82,0x82,0x82,0x82,0x82,0x83,0x83,
		0x84,0x85,0x85,0x86,0x87,0x88,0x89,0x8a,
		0x8b,0x8c,0x8e,0x8f,0x90,0x92,0x94,0x95,
		0x97,0x99,0x9a,0x9c,0x9e,0xa0,0xa2,0xa5,
		0xa7,0xa9,0xab,0xae,0xb0,0xb2,0xb5,0xb7,
		0xba,0xbd,0xbf,0xc2,0xc5,0xc7,0xca,0xcd,
		0xd0,0xd3,0xd6,0xd9,0xdc,0xdf,0xe2,0xe5,
		0xe8,0xeb,0xee,0xf1,0xf4,0xf7,0xfa,0xfd,
	};

	static unsigned char const SinTable[256] = {
		0x7f,0x7e,0x7e,0x7e,0x7e,0x7e,0x7d,0x7d,
		0x7c,0x7b,0x7b,0x7a,0x79,0x78,0x77,0x76,
		0x75,0x74,0x72,0x71,0x70,0x6e,0x6c,0x6b,
		0x69,0x67,0x66,0x64,0x62,0x60,0x5e,0x5b,
		0x59,0x57,0x55,0x52,0x50,0x4e,0x4b,0x49,
		0x46,0x43,0x41,0x3e,0x3b,0x39,0x36,0x33,
		0x30,0x2d,0x2a,0x27,0x24,0x21,0x1e,0x1b,
		0x18,0x15,0x12,0x0f,0x0c,0x09,0x06,0x03,

		0x00,0xfd,0xfa,0xf7,0xf4,0xf1,0xee,0xeb,
		0xe8,0xe5,0xe2,0xdf,0xdc,0xd9,0xd6,0xd3,
		0xd0,0xcd,0xca,0xc7,0xc5,0xc2,0xbf,0xbd,
		0xba,0xb7,0xb5,0xb2,0xb0,0xae,0xab,0xa9,
		0xa7,0xa5,0xa2,0xa0,0x9e,0x9c,0x9a,0x99,
		0x97,0x95,0x94,0x92,0x91,0x8f,0x8e,0x8c,
		0x8b,0x8a,0x89,0x88,0x87,0x86,0x85,0x85,
		0x84,0x83,0x83,0x82,0x82,0x82,0x82,0x82,

		0x82,0x82,0x82,0x82,0x82,0x82,0x83,0x83,
		0x84,0x85,0x85,0x86,0x87,0x88,0x89,0x8a,
		0x8b,0x8c,0x8e,0x8f,0x90,0x92,0x94,0x95,
		0x97,0x99,0x9a,0x9c,0x9e,0xa0,0xa2,0xa5,
		0xa7,0xa9,0xab,0xae,0xb0,0xb2,0xb5,0xb7,
		0xba,0xbd,0xbf,0xc2,0xc5,0xc7,0xca,0xcd,
		0xd0,0xd3,0xd6,0xd9,0xdc,0xdf,0xe2,0xe5,
		0xe8,0xeb,0xee,0xf1,0xf4,0xf7,0xfa,0xfd,

		0x00,0x03,0x06,0x09,0x0c,0x0f,0x12,0x15,
		0x18,0x1b,0x1e,0x21,0x24,0x27,0x2a,0x2d,
		0x30,0x33,0x36,0x39,0x3b,0x3e,0x41,0x43,
		0x46,0x49,0x4b,0x4e,0x50,0x52,0x55,0x57,
		0x59,0x5b,0x5e,0x60,0x62,0x64,0x65,0x67,
		0x69,0x6b,0x6c,0x6e,0x6f,0x71,0x72,0x74,
		0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7b,
		0x7c,0x7d,0x7d,0x7e,0x7e,0x7e,0x7e,0x7e,
	};
	distance = distance;		// Keep LINT quiet.

#ifdef OBSOLETE
	/*
	**	Calculate and add in the X component of the move.
	*/
	_AX = CosTable[dir];
	asm imul word ptr distance
	asm shl ax,1
	asm rcl dx,1
	asm mov al,ah
	asm mov ah,dl
	_DX = _AX;
	x += _DX;
#else
	//
	// Have to declare table as unsigned otherwise MSVC complains, but we need to treat the actual values as signed.
	//
	static const char *_cos_table = (char*)&CosTable[0];
	x += calcx(_cos_table[dir], distance);
#endif
//	asm add [word ptr start],ax

#ifdef OBSOLETE
	/*
	**	Calculate and add in the Y component of the move.
	*/
	_AX = SinTable[dir];
	asm imul word ptr distance
	asm shl ax,1
	asm rcl dx,1
	asm mov al,ah
	asm mov ah,dl
	asm neg ax				// Subtraction needed because of inverted sine table.
	_DX = _AX;
	y += _DX;
#else
	//
	// Have to declare table as unsigned otherwise MSVC complains, but we need to treat the actual values as signed.
	//
	static const char *_sin_table = (char*)&SinTable[0];
	y += calcy(_sin_table[dir], distance);
#endif
//	asm add [word ptr start+2],ax

}


/***********************************************************************************************
 * Normal_Move_Point -- Moves point with tilt compensation.                                    *
 *                                                                                             *
 *    This routine will move the point in the direction and distance specified but it will     *
 *    take into account the tilt of the playing field. Typical use of this routine is to       *
 *    determine positioning as it relates to the playfield. Turrets are a good example of      *
 *    this.                                                                                    *
 *                                                                                             *
 * INPUT:   x,y   -- References to the coordinates to adjust.                                  *
 *                                                                                             *
 *          dir   -- The direction of the desired movement.                                    *
 *                                                                                             *
 *          distance -- The distance (in coordinate units) to move the point.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// Loss of precision in initializations (8 bits to 7 bits) warning. Hmmm.. can this be fixed?
//lint -e569
void Normal_Move_Point(short &x, short &y, register DirType dir, unsigned short distance)
{
	static unsigned char const CosTable[256] = {
		0x00,0x03,0x06,0x09,0x0c,0x0f,0x12,0x15,
		0x18,0x1b,0x1e,0x21,0x24,0x27,0x2a,0x2d,
		0x30,0x33,0x36,0x39,0x3b,0x3e,0x41,0x43,
		0x46,0x49,0x4b,0x4e,0x50,0x52,0x55,0x57,
		0x59,0x5b,0x5e,0x60,0x62,0x64,0x65,0x67,
		0x69,0x6b,0x6c,0x6e,0x6f,0x71,0x72,0x74,
		0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7b,
		0x7c,0x7d,0x7d,0x7e,0x7e,0x7e,0x7e,0x7e,

		0x7f,0x7e,0x7e,0x7e,0x7e,0x7e,0x7d,0x7d,
		0x7c,0x7b,0x7b,0x7a,0x79,0x78,0x77,0x76,
		0x75,0x74,0x72,0x71,0x70,0x6e,0x6c,0x6b,
		0x69,0x67,0x66,0x64,0x62,0x60,0x5e,0x5b,
		0x59,0x57,0x55,0x52,0x50,0x4e,0x4b,0x49,
		0x46,0x43,0x41,0x3e,0x3b,0x39,0x36,0x33,
		0x30,0x2d,0x2a,0x27,0x24,0x21,0x1e,0x1b,
		0x18,0x15,0x12,0x0f,0x0c,0x09,0x06,0x03,

		0x00,0xfd,0xfa,0xf7,0xf4,0xf1,0xee,0xeb,
		0xe8,0xe5,0xe2,0xdf,0xdc,0xd9,0xd6,0xd3,
		0xd0,0xcd,0xca,0xc7,0xc5,0xc2,0xbf,0xbd,
		0xba,0xb7,0xb5,0xb2,0xb0,0xae,0xab,0xa9,
		0xa7,0xa5,0xa2,0xa0,0x9e,0x9c,0x9a,0x99,
		0x97,0x95,0x94,0x92,0x91,0x8f,0x8e,0x8c,
		0x8b,0x8a,0x89,0x88,0x87,0x86,0x85,0x85,
		0x84,0x83,0x83,0x82,0x82,0x82,0x82,0x82,

		0x82,0x82,0x82,0x82,0x82,0x82,0x83,0x83,
		0x84,0x85,0x85,0x86,0x87,0x88,0x89,0x8a,
		0x8b,0x8c,0x8e,0x8f,0x90,0x92,0x94,0x95,
		0x97,0x99,0x9a,0x9c,0x9e,0xa0,0xa2,0xa5,
		0xa7,0xa9,0xab,0xae,0xb0,0xb2,0xb5,0xb7,
		0xba,0xbd,0xbf,0xc2,0xc5,0xc7,0xca,0xcd,
		0xd0,0xd3,0xd6,0xd9,0xdc,0xdf,0xe2,0xe5,
		0xe8,0xeb,0xee,0xf1,0xf4,0xf7,0xfa,0xfd,
	};

	static unsigned char const SinTable[256] = {
		0x7f,0x7e,0x7e,0x7e,0x7e,0x7e,0x7d,0x7d,
		0x7c,0x7b,0x7b,0x7a,0x79,0x78,0x77,0x76,
		0x75,0x74,0x72,0x71,0x70,0x6e,0x6c,0x6b,
		0x69,0x67,0x66,0x64,0x62,0x60,0x5e,0x5b,
		0x59,0x57,0x55,0x52,0x50,0x4e,0x4b,0x49,
		0x46,0x43,0x41,0x3e,0x3b,0x39,0x36,0x33,
		0x30,0x2d,0x2a,0x27,0x24,0x21,0x1e,0x1b,
		0x18,0x15,0x12,0x0f,0x0c,0x09,0x06,0x03,

		0x00,0xfd,0xfa,0xf7,0xf4,0xf1,0xee,0xeb,
		0xe8,0xe5,0xe2,0xdf,0xdc,0xd9,0xd6,0xd3,
		0xd0,0xcd,0xca,0xc7,0xc5,0xc2,0xbf,0xbd,
		0xba,0xb7,0xb5,0xb2,0xb0,0xae,0xab,0xa9,
		0xa7,0xa5,0xa2,0xa0,0x9e,0x9c,0x9a,0x99,
		0x97,0x95,0x94,0x92,0x91,0x8f,0x8e,0x8c,
		0x8b,0x8a,0x89,0x88,0x87,0x86,0x85,0x85,
		0x84,0x83,0x83,0x82,0x82,0x82,0x82,0x82,

		0x82,0x82,0x82,0x82,0x82,0x82,0x83,0x83,
		0x84,0x85,0x85,0x86,0x87,0x88,0x89,0x8a,
		0x8b,0x8c,0x8e,0x8f,0x90,0x92,0x94,0x95,
		0x97,0x99,0x9a,0x9c,0x9e,0xa0,0xa2,0xa5,
		0xa7,0xa9,0xab,0xae,0xb0,0xb2,0xb5,0xb7,
		0xba,0xbd,0xbf,0xc2,0xc5,0xc7,0xca,0xcd,
		0xd0,0xd3,0xd6,0xd9,0xdc,0xdf,0xe2,0xe5,
		0xe8,0xeb,0xee,0xf1,0xf4,0xf7,0xfa,0xfd,

		0x00,0x03,0x06,0x09,0x0c,0x0f,0x12,0x15,
		0x18,0x1b,0x1e,0x21,0x24,0x27,0x2a,0x2d,
		0x30,0x33,0x36,0x39,0x3b,0x3e,0x41,0x43,
		0x46,0x49,0x4b,0x4e,0x50,0x52,0x55,0x57,
		0x59,0x5b,0x5e,0x60,0x62,0x64,0x65,0x67,
		0x69,0x6b,0x6c,0x6e,0x6f,0x71,0x72,0x74,
		0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7b,
		0x7c,0x7d,0x7d,0x7e,0x7e,0x7e,0x7e,0x7e,
	};
	distance = distance;		// Keep LINT quiet.

	//
	// Have to declare table as unsigned otherwise MSVC complains, but we need to treat the actual values as signed.
	//
	static const char *_sin_table = (char*)&SinTable[0];
	static const char *_cos_table = (char*)&CosTable[0];

	x += calcx(_cos_table[dir], distance);

	y += calcy(_sin_table[dir] / 2, distance);
}
