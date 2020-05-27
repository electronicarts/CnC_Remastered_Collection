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

/* $Header: /CounterStrike/MONOC.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MONO.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : July 2, 1994                                                 *
 *                                                                                             *
 *                  Last Update : June 5, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   MonoClass::Clear -- Clears the monochrome screen object.                                  *
 *   MonoClass::Draw_Box -- Draws a box using the IBM linedraw characters.                     *
 *   MonoClass::Fill_Attrib -- Fill a block with specified attribute.                          *
 *   MonoClass::MonoClass -- The default constructor for monochrome screen object.             *
 *   MonoClass::Pan -- Scroll the window right or left.                                        *
 *   MonoClass::Print -- Prints the text string at the current cursor coordinates.             *
 *   MonoClass::Print -- Simple print of text number.                                          *
 *   MonoClass::Printf -- Prints a formatted string to the monochrome screen.                  *
 *   MonoClass::Printf -- Prints formatted text using text string number.                      *
 *   MonoClass::Scroll -- Scroll the monochrome screen up by the specified lines.              *
 *   MonoClass::Set_Cursor -- Sets the monochrome cursor to the coordinates specified.         *
 *   MonoClass::Sub_Window -- Partitions the mono screen into a sub-window.                    *
 *   MonoClass::Text_Print -- Prints text to the monochrome object at coordinates indicated.   *
 *   MonoClass::Text_Print -- Simple text printing from text number.                           *
 *   MonoClass::View -- Brings the mono object to the main display.                            *
 *   MonoClass::operator = -- Handles making one mono object have the same imagery as another. *
 *   MonoClass::~MonoClass -- The default destructor for a monochrome screen object.           *
 *   Mono_Clear_Screen -- Clear the currently visible monochrome page.                         *
 *   Mono_Draw_Rect -- Draws rectangle to monochrome screen.                                   *
 *   Mono_Print -- Prints simple text to monochrome screen.                                    *
 *   Mono_Printf -- Prints formatted text to visible page.                                     *
 *   Mono_Text_Print -- Prints text to location specified.                                     *
 *   Mono_X -- Fetches the X cursor position for current visible mono page.                    *
 *   Mono_Y -- Fetches the Y cursor position for current mono page.                            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

//#if (0)//PG
//#include	"watcom.h"
#include	"monoc.h"

#include	"function.h"

#include	<stdlib.h>
#include	<stdio.h>
#include	<dos.h>
//#include	<mem.h>
#include	<stdarg.h>
#include	<string.h>


//PG
//extern void output(short port, short data);
//#pragma aux output parm [dx] [ax] =		\
//		"out	dx,al"		\
//		"inc	dx"			\
//		"mov	al,ah"		\
//		"out	dx,al"


bool MonoClass::Enabled = 0;
MonoClass * MonoClass::PageUsage[MonoClass::MAX_MONO_PAGES];
//MonoClass::MonoPageType * MonoClass::MonoRAM = (MonoClass::MonoPageType *) 0xB0000;

/*
**	These are the IBM linedraw characters.
*/
MonoClass::BoxDataType const MonoClass::CharData[MonoClass::COUNT] = {
	{0xDA,0xC4,0xBF,0xB3,0xD9,0xC4,0xC0,0xB3},	// Single line
	{0xD5,0xCD,0xB8,0xB3,0xBE,0xCD,0xD4,0xB3},	// Double horz.
	{0xD6,0xC4,0xB7,0xBA,0xBD,0xC4,0xD3,0xBA},	// Double vert.
	{0xC9,0xCD,0xBB,0xBA,0xBC,0xCD,0xC8,0xBA}		// Double horz and vert.
};


#ifdef NEVER
template<class T>
T min(T a, T b) {
	if (a < b) return(a);
	return(b);
}

template<class T>
T max(T a, T b) {
	if (a > b) return(a);
	return(b);
}
#endif


/***********************************************************************************************
 * MonoClass::MonoClass -- The default constructor for monochrome screen object.               *
 *                                                                                             *
 *    This is the constructor for monochrome screen objects. It handles allocating a free      *
 *    monochrome page. If there are no more pages available, then this is a big error. The     *
 *    page allocated may not be the visible one. Call the View function in order to bring      *
 *    it to the displayed page.                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
MonoClass::MonoClass(void) :
	X(0),
	Y(0),
	Attrib(NORMAL),
	Page(0),
	SubX(0),
	SubY(0),
	SubW(COLUMNS),
	SubH(LINES)
{
	int	index;

	for (index = 0; index < MAX_MONO_PAGES; index++) {
		if (!PageUsage[index]) {
			PageUsage[index] = this;
			Page = (char)index;
			break;
		}
	}
	if (index == MAX_MONO_PAGES) {
		// Major error message should pop up here!
		delete this;
	}
}


/***********************************************************************************************
 * MonoClass::~MonoClass -- The default destructor for a monochrome screen object.             *
 *                                                                                             *
 *    This is the default destructor for a monochrome screen object.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
MonoClass::~MonoClass(void)
{
	PageUsage[Page] = 0;
}


/***********************************************************************************************
 * MonoClass::Pan -- Scroll the window right or left.                                          *
 *                                                                                             *
 *    This routine will scroll the window to the right or left as indicated by the number of   *
 *    rows.                                                                                    *
 *                                                                                             *
 * INPUT:   cols  -- The number of columns to pan the window. Positive numbers pan to the left *
 *                   while negative numbers pan to the right.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Pan(int cols)
{
	if (cols == 0) return;

	if (abs(cols) >= SubW) {
		Clear();
		return;
	}

	CellType cell;
	cell.Character = ' ';
	cell.Attribute = Attrib;

	if (cols > 0) {
		for (int index = SubY; index < SubY+SubH; index++) {
			memmove(&Page_Ptr()->Data[index][SubX], &Page_Ptr()->Data[index][SubX+cols], sizeof(CellType)*(SubW-cols));
			for (int cc = SubX+SubW-cols; cc < SubX+SubW; cc++) {
				Page_Ptr()->Data[index][cc] = cell;
			}
		}
	} else {
		for (int index = SubY; index < SubY+SubH; index++) {
			memmove(&Page_Ptr()->Data[index][SubX-cols], &Page_Ptr()->Data[index][SubX], sizeof(CellType)*(SubW+cols));
			for (int cc = SubX; cc < SubX-cols; cc++) {
				Page_Ptr()->Data[index][cc] = cell;
			}
		}
	}
	Set_Cursor(X-cols, Y);
}


/***********************************************************************************************
 * MonoClass::Sub_Window -- Partitions the mono screen into a sub-window.                      *
 *                                                                                             *
 *    This routine is used to partition the monochrome screen so that only a sub-window will   *
 *    be processed. By using this, a separate rectangle of the screen can be cleared,          *
 *    scrolled, panned, or printed into.                                                       *
 *                                                                                             *
 * INPUT:   x,y   -- The upper left corner of the new sub-window.                              *
 *                                                                                             *
 *          w,h   -- Dimensions of the sub-window specified in characters.                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The parameters are clipped as necessary.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Sub_Window(int x, int y, int w, int h)
{
	/*
	**	Undo any sub window adjustments to the cursor position.
	*/
	X += SubX;
	Y += SubY;

	/*
	**	Ensure parameters are legal.
	*/
	x = min(x, COLUMNS-1);
	x = max(x, 0);
	y = min(y, LINES-1);
	y = max(y, 0);
	if (w == -1) w = COLUMNS-x;
	if (h == -1) h = LINES-y;

	/*
	**	Assign the new sub-region.
	*/
	SubX = x;
	SubY = y;
	SubW = w;
	SubH = h;

	/*
	**	Move the cursor (if necessary) so that it falls within that region.
	*/
	int xx = X;
	int yy = Y;
	xx = min(xx, SubX+SubW-1);
	xx = max(xx, SubX);
	yy = min(yy, SubY+SubH-1);
	yy = max(yy, SubY);
	Set_Cursor(xx-SubX, yy-SubY);
}


/***********************************************************************************************
 * MonoClass::Draw_Box -- Draws a box using the IBM linedraw characters.                       *
 *                                                                                             *
 *    Use this routine to draw a box to the monochrome screen. The IBM line draw characters    *
 *    are used to give the it a fancy appearance. There are several line draw modes supported. *
 *                                                                                             *
 * INPUT:   x,y      -- The coordinates of the upper left corner of the box.                   *
 *                                                                                             *
 *          w,y      -- The width and height (respectively) to make the box.                   *
 *                                                                                             *
 *          attrib   -- The text attribute to use when drawing the box outline characters.     *
 *                                                                                             *
 *          thick    -- The thickness style to use. Examine the BoxStyleType enum for          *
 *                      elaboration on the supported styles.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The interior of the box is NOT cleared by this routine. It is advised that this *
 *             area be cleared before the box is drawn.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Draw_Box(int x, int y, int w, int h, MonoAttribute attrib, BoxStyleType thick)
{
	CellType	cell;
	MonoAttribute	oldattrib = Attrib;

	if (!Enabled || !w || !h) return;

	x = min(x, SubW);
	x = max(x, 0);
	y = min(y, SubH);
	y = max(y, 0);
	w = min(w, SubW-x);
	w = max(w, 1);
	h = min(h, SubH-y);
	h = max(h, 1);

	x += SubX;
	y += SubY;

	cell.Attribute = attrib;

	/*
	**	Draw the horizontal lines.
	*/
	for (int xpos = 0; xpos < w-2; xpos++) {
		cell.Character = CharData[thick].TopEdge;
		Page_Ptr()->Data[y][x+xpos+1] = cell;
		cell.Character = CharData[thick].BottomEdge;
		Page_Ptr()->Data[y+h-1][x+xpos+1] = cell;
	}

	/*
	**	Draw the vertical lines.
	*/
	for (int ypos = 0; ypos < h-2; ypos++) {
		cell.Character = CharData[thick].LeftEdge;
		Page_Ptr()->Data[y+ypos+1][x] = cell;
		cell.Character = CharData[thick].RightEdge;
		Page_Ptr()->Data[y+ypos+1][x+w-1] = cell;
	}

	/*
	**	Draw the four corners.
	*/
	if (w > 1 && h > 1) {
		cell.Character = CharData[thick].UpperLeft;
		Page_Ptr()->Data[y][x] = cell;
		cell.Character = CharData[thick].UpperRight;
		Page_Ptr()->Data[y][x+w-1] = cell;
		cell.Character = CharData[thick].BottomRight;
		Page_Ptr()->Data[y+h-1][x+w-1] = cell;
		cell.Character = CharData[thick].BottomLeft;
		Page_Ptr()->Data[y+h-1][x] = cell;
	}

	Attrib = oldattrib;
}


/***********************************************************************************************
 * MonoClass::Set_Cursor -- Sets the monochrome cursor to the coordinates specified.           *
 *                                                                                             *
 *    Use this routine to set the monochrome's cursor position to the coordinates specified.   *
 *    This is the normal way of controlling where the next Print or Printf will output the     *
 *    text to.                                                                                 *
 *                                                                                             *
 * INPUT:   x,y   -- The coordinate to position the monochrome cursor. 0,0 is the upper left   *
 *                   corner.                                                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Set_Cursor(int x, int y)
{
	x = min(x, SubW);
	x = max(x, 0);
	y = min(y, SubH);
	y = max(y, 0);

	X = x;
	Y = y;
#if (0)//PG
	if (!Enabled) return;

	/*
	**	Update the visible cursor position only if the this mono page is the currently
	**	visible one.
	*/
	int pos = ((y+SubY)*COLUMNS)+(x+SubX);
	if (Page == 0) {
		output(CONTROL_PORT, (short)(0x0E|(pos&0xFF00)));
		output(CONTROL_PORT, (short)(0x0F|(pos<<8)));
	}
#endif
}


/***********************************************************************************************
 * MonoClass::Clear -- Clears the monochrome screen object.                                    *
 *                                                                                             *
 *    This routine will fill the monochrome screen object with spaces. It is clearing the      *
 *    screen of data, making it free for output. The cursor is positioned at the upper left    *
 *    corner of the screen by this routine.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Clear(void)
{
	if (!Enabled) return;

	Set_Cursor(0, 0);

	CellType	cell;
	cell.Attribute = Attrib;
	cell.Character = ' ';

	for (int rows = 0; rows < SubH; rows++) {
		for (int cols = 0; cols < SubW; cols++) {
			Page_Ptr()->Data[rows+SubX][cols+SubY] = cell;
		}
	}
}


/***********************************************************************************************
 * MonoClass::Fill_Attrib -- Fill a block with specified attribute.                            *
 *                                                                                             *
 *    This routine will give the specified attribute to the characters within the block        *
 *    but will not change the characters themselves. You can use this routine to change the    *
 *    underline, blink, or inverse characteristics of text.                                    *
 *                                                                                             *
 * INPUT:   x,y      -- The upper left coordinate of the region to change.                     *
 *                                                                                             *
 *          w,h      -- The dimensions of the region to change (in characters).                *
 *                                                                                             *
 *          attrib   -- The attribute to fill into the region specified.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Fill_Attrib(int x, int y, int w, int h, MonoAttribute attrib)
{
	if (!w || !h || x >= SubW || h >= SubH || x+w > SubW || y+h > SubH) return;

	for (int rows = y; rows < y+h; rows++) {
		for (int cols = x; cols < x+w; cols++) {
			Page_Ptr()->Data[rows+SubY][cols+SubX].Attribute = attrib;
		}
	}
}


/***********************************************************************************************
 * MonoClass::Scroll -- Scroll the monochrome screen up by the specified lines.                *
 *                                                                                             *
 *    Use this routine to scroll the monochrome screen up by the number of lines specified.    *
 *    This routine is typically called by the printing functions so that the monochrome screen *
 *    behaves in the expected manner -- printing at the bottom of the screen scrolls it up     *
 *    to make room for new text.                                                               *
 *                                                                                             *
 * INPUT:   lines -- The number of lines to scroll the monochrome screen.                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Scroll(int lines)
{
	if (!Enabled || lines <= 0) return;

	if (abs(lines) >= SubH) {
		Clear();
		return;
	}

	CellType	cell;
	cell.Attribute = Attrib;
	cell.Character = ' ';

	if (lines > 0) {
		for (int row = 0; row < SubH-lines; row++) {
			memmove(&Page_Ptr()->Data[SubY+row][SubX], &Page_Ptr()->Data[SubY+row+1][SubX], SubW*sizeof(CellType));
		}
		for (int frow = SubH-lines; frow < SubH; frow++) {
			for (int cc = 0; cc < SubW; cc++) {
				Page_Ptr()->Data[SubY+frow][SubX+cc] = cell;
			}
		}
	} else {
		for (int row = SubH-1; row >= -lines; row--) {
			memmove(&Page_Ptr()->Data[SubY+row][SubX], &Page_Ptr()->Data[SubY+row-1][SubX], SubW*sizeof(CellType));
		}
		for (int frow = 0; frow < -lines; frow++) {
			for (int cc = 0; cc < SubW; cc++) {
				Page_Ptr()->Data[SubY+frow][SubX+cc] = cell;
			}
		}
	}

	Set_Cursor(X, Y-lines);
}


/***********************************************************************************************
 * MonoClass::Printf -- Prints a formatted string to the monochrome screen.                    *
 *                                                                                             *
 *    Use this routine to output a formatted string, using the standard formatting options,    *
 *    to the monochrome screen object's current cursor position.                               *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text to print.                                             *
 *                                                                                             *
 *          ...   -- Any optional parameters to supply in formatting the text.                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The total formatted text length must not exceed 255 characters.                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Printf(char const *text, ...)
{
	va_list	va;
	/*
	**	The buffer object is placed at the end of the local variable list
	**	so that if the sprintf happens to spill past the end, it isn't likely
	**	to trash anything (important). The buffer is then manually truncated
	**	to maximum allowed size before being printed.
	*/
	char buffer[256];

	if (!Enabled) return;

	va_start(va, text);
	vsprintf(buffer, text, va);
	buffer[sizeof(buffer)-1] = '\0';

	Print(buffer);
	va_end(va);
}


/***********************************************************************************************
 * MonoClass::Printf -- Prints formatted text using text string number.                        *
 *                                                                                             *
 *    This routine will take the given text string number and print the formatted text to      *
 *    the monochrome screen.                                                                   *
 *                                                                                             *
 * INPUT:   text  -- The text number to convert into real text (by way of external function).  *
 *                                                                                             *
 *          ...   -- Additional parameters as needed.                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Printf(int text, ...)
{
	va_list	va;

	/*
	**	The buffer object is placed at the end of the local variable list
	**	so that if the sprintf happens to spill past the end, it isn't likely
	**	to trash anything (important). The buffer is then manually truncated
	**	to maximum allowed size before being printed.
	*/
	char buffer[256];

	if (!Enabled) return;

	va_start(va, text);
	vsprintf(buffer, Text_String(text), va);
	buffer[sizeof(buffer)-1] = '\0';

	Print(buffer);
	va_end(va);
}


/***********************************************************************************************
 * MonoClass::Print -- Prints the text string at the current cursor coordinates.               *
 *                                                                                             *
 *    Use this routine to output the specified text string at the monochrome object's current  *
 *    text coordinate position.                                                                *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the string to print.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Print(char const * ptr)
{
	int startcol = X;
	char const * text;
	CellType	cell;

	if (!ptr || !Enabled) return;

	text = ptr;
	cell.Attribute = Attrib;
	while (*text) {

		cell.Character = *text;

		/*
		**	Sometimes the character string is used for cursor control instead
		**	of plain text output. Check for this case.
		*/
		switch (cell.Character) {

			/*
			**	The "return" code behaves as it did in the old C library
			**	mono system. That is, it returns the cursor position to
			**	the next line but at the starting column of the print.
			*/
			case '\r':
				if (Y == SubH-1) {
					Scroll(1);
				}
				Set_Cursor(startcol, Y+1);
				break;

			/*
			**	The TAB character is not currently handled. Convert it to
			**	a space instead.
			*/
			case '\t':
				cell.Character = ' ';
				// fall into normal print case.

			/*
			**	All other characters are output directly and the cursor moves
			**	rightward to match. If the cursor wraps past the right
			**	edge it is moved to the next now down at left margin. If the
			**	cursor goes off the bottom of the display, the display is scrolled
			**	upward a line.
			*/
			default:
				Page_Ptr()->Data[SubY+Y][SubX+X] = cell;

				if (X < SubW-1) {
					Set_Cursor(X+1, Y);
					break;
				}
				// Fall into newline case.

			/*
			**	The "newline" code behaves like the console newline character.
			**	That is, it moves the cursor down one line and at the first
			**	column.
			*/
			case '\n':
				if (Y == SubH-1) {
					Scroll(1);
				}
				Set_Cursor(0, Y+1);
				break;
		}
		text++;
	}
}


/***********************************************************************************************
 * MonoClass::Text_Print -- Prints text to the monochrome object at coordinates indicated.     *
 *                                                                                             *
 *    Use this routine to output text to the monochrome object at the X and Y coordinates      *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text string to display.                                    *
 *                                                                                             *
 *          x,y   -- The X and Y character coordinates to start the printing at.               *
 *                                                                                             *
 *          attrib-- Optional parameter that specifies what text attribute code to use.        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Text_Print(char const *text, int x, int y, MonoAttribute attrib)
{
	int	oldx = X;
	int	oldy = Y;
	MonoAttribute oldattrib = Attrib;

	X = (char)x;
	Y = (char)y;
	Attrib = attrib;
	Print(text);
	Attrib = oldattrib;
	Set_Cursor(oldx, oldy);
}


/***********************************************************************************************
 * MonoClass::Text_Print -- Simple text printing from text number.                             *
 *                                                                                             *
 *    This will print the text (represented by the text number) to the location on the         *
 *    monochrome screen specified.                                                             *
 *                                                                                             *
 * INPUT:   text  -- The text number to print (converted to real text by external routine).    *
 *                                                                                             *
 *          x,y   -- The coordinates to begin the printing at.                                 *
 *                                                                                             *
 *          attrib-- The character attribute to use while printing.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Text_Print(int text, int x, int y, MonoAttribute attrib)
{
	int	oldx = X;
	int	oldy = Y;
	MonoAttribute oldattrib = Attrib;

	if (text != 0) {
		X = (char)x;
		Y = (char)y;
		Attrib = attrib;
		Print(Text_String(text));
		Attrib = oldattrib;
		Set_Cursor(oldx, oldy);
	}
}


/***********************************************************************************************
 * MonoClass::Print -- Simple print of text number.                                            *
 *                                                                                             *
 *    Prints text represented by the text number specified.                                    *
 *                                                                                             *
 * INPUT:   text  -- The text number to print (converted to real text by external routine).    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::Print(int text)
{
	Print(Text_String(text));
}


/***********************************************************************************************
 * MonoClass::operator = -- Handles making one mono object have the same imagery as another.   *
 *                                                                                             *
 *    The assignment operator will handle copying the imagery from one monochrome object to    *
 *    another. Use this routine in to make two monochrome class objects visually identical.    *
 *                                                                                             *
 * INPUT:   src   -- A reference to the source (right side) monochrome object.                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
MonoClass & MonoClass::operator = (MonoClass const & src)
{
	memmove(Page_Ptr(), src.Page_Ptr(), sizeof(MonoPageType));
	Set_Cursor(src.X, src.Y);
	return(*this);
}


/***********************************************************************************************
 * MonoClass::View -- Brings the mono object to the main display.                              *
 *                                                                                             *
 *    Use this routine to display the mono object on the monochrome screen. It is possible     *
 *    that the mono object exists on some background screen memory. Calling this routine will  *
 *    perform the necessary memory swapping to bring the data to the front. The mono object    *
 *    that was currently being viewed is not destroyed by this function. It is merely moved    *
 *    off to some background page. It can be treated normally, except that is just isn't       *
 *    visible.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MonoClass::View(void)
{
	if (Get_Current() == this) return;

	/*
	**	If the visible page is already assigned to a real monochrome page
	**	object, then it must be swapped with the new one.
	*/
	MonoClass * displace = Get_Current();
	if (displace) {
		for (int line = 0; line < LINES; line++) {
			for (int col = 0; col < COLUMNS; col++) {
				CellType temp = Page_Ptr()->Data[line][col];
				Page_Ptr()->Data[line][col] = Raw_Ptr(0)->Data[line][col];
				Raw_Ptr(0)->Data[line][col] = temp;
			}
		}
		displace->Page = Page;

	} else {

		/*
		**	Just copy the new page over since the display page is not assigned
		**	to a real monochrome page object.
		*/
		memmove(Raw_Ptr(0), Page_Ptr(), sizeof(MonoPageType));
	}
	PageUsage[Page] = displace;
	PageUsage[0] = this;
	Page = 0;

	Set_Cursor(X, Y);
}



/************************************************************************************
**	This is the set of C wrapper functions that access the MonoClass support routines.
**	Since the C interface doesn't have the ability to write to non-visible pages, it
**	will just blast the output to whichever mono page is currently visible. If there is
**	no mono class object that is visible, then one will be created -- BUT NOT FREED.
**	Typically, this is ok, since the C interface will create only one MonoClass object
**	and the system supports up to 8.
*/
void Mono_Set_Cursor(int x, int y)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass * mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		mono->Set_Cursor(x, y);
	}
}


/***********************************************************************************************
 * Mono_Printf -- Prints formatted text to visible page.                                       *
 *                                                                                             *
 *    This routine will print formatted text (with parameters) to the visible monochrome page  *
 *    at whatever the current cursor location is.                                              *
 *                                                                                             *
 * INPUT:   string   -- The string to use as the main text and formatting control string.      *
 *                                                                                             *
 *          ...      -- Any additional parameters required by the formatting string.           *
 *                                                                                             *
 * OUTPUT:  Returns with the number of characters written to the display.                      *
 *                                                                                             *
 * WARNINGS:   The total size of the formatted text must not exceed 256 characters.            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int Mono_Printf(char const * string, ...)
{
	va_list	va;
	char buffer[256];

	buffer[0] = '\0';
	if (MonoClass::Is_Enabled()) {
		MonoClass * mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}

		va_start(va, string);
		vsprintf(buffer, string, va);

		mono->Print(buffer);

		va_end(va);
	}
	return((short)strlen(buffer));
}


/***********************************************************************************************
 * Mono_Clear_Screen -- Clear the currently visible monochrome page.                           *
 *                                                                                             *
 *    This routine will clear the currently visible monochrome page.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Mono_Clear_Screen(void)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass * mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		mono->Clear();
	}
}


/***********************************************************************************************
 * Mono_Text_Print -- Prints text to location specified.                                       *
 *                                                                                             *
 *    This routine will print the specified text to the location indicated.                    *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text to print.                                             *
 *                                                                                             *
 *          x,y   -- The coordinate to print the text at.                                      *
 *                                                                                             *
 *          attrib-- The attribute to use when printing the text.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Mono_Text_Print(void const *text, int x, int y, int attrib)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass * mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		mono->Text_Print((const char*)text, x, y, (MonoClass::MonoAttribute)attrib);
	}
}


/***********************************************************************************************
 * Mono_Draw_Rect -- Draws rectangle to monochrome screen.                                     *
 *                                                                                             *
 *    Use this routine to draw a rectangle to the monochrome screen. The dimensions, attribute,*
 *    and line style are controlled by parameters.                                             *
 *                                                                                             *
 * INPUT:   x,y   -- Coordinate of upper left corner of the box to draw.                       *
 *                                                                                             *
 *          w,h   -- The width and height of the box to draw.                                  *
 *                                                                                             *
 *          attrib-- The attribute to use when drawing the box.                                *
 *                                                                                             *
 *          thick -- The line drawing style to use.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Mono_Draw_Rect(int x, int y, int w, int h, int attrib, int thick)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass * mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		mono->Draw_Box(x, y, w, h, (MonoClass::MonoAttribute)attrib, (MonoClass::BoxStyleType)thick);
	}
}


/***********************************************************************************************
 * Mono_Print -- Prints simple text to monochrome screen.                                      *
 *                                                                                             *
 *    This is the non-formatting print to the monochrome screen.                               *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text that will be printed.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Mono_Print(void const *text)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass * mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		mono->Print((const char*)text);
	}
}


/***********************************************************************************************
 * Mono_X -- Fetches the X cursor position for current visible mono page.                      *
 *                                                                                             *
 *    Use this routine to get the current cursor X position. This only applies to the          *
 *    currently visible monochrome page.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with X position of cursor.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int Mono_X(void)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass * mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		return(short)mono->Get_X();
	}
	return(0);
}


/***********************************************************************************************
 * Mono_Y -- Fetches the Y cursor position for current mono page.                              *
 *                                                                                             *
 *    This routine will fetch the current Y cursor position for the monochrome page.           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current Y position of the monochrome page.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int Mono_Y(void)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass * mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		return(short)mono->Get_X();
	}
	return(0);
}


void Mono_Put_Char(char , int )
{
}

void Mono_Scroll(int )
{
}

void Mono_View_Page(int )
{
}

int Mono_Printf(int string, ...)
{
	va_list	va;
	char buffer[256];

	buffer[0] = '\0';
	if (MonoClass::Is_Enabled()) {
		MonoClass * mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}

		va_start(va, string);
		vsprintf(buffer, Text_String(string), va);

		mono->Print(buffer);

		va_end(va);
	}
	return((short)strlen(buffer));
}

//#endif