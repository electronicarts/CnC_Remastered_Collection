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

/* $Header:   F:\projects\c&c\vcs\code\monoc.cpv   2.13   16 Oct 1995 16:50:36   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : October 17, 1994   [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   MonoClass::Clear -- Clears the monochrome screen object.                                  *
 *   MonoClass::Draw_Box -- Draws a box using the IBM linedraw characters.                     *
 *   MonoClass::MonoClass -- The default constructor for monochrome screen object.             *
 *   MonoClass::operator = -- Handles making one mono object have the same imagery as another. *
 *   MonoClass::Print -- Prints the text string at the current cursor coordinates.             *
 *   MonoClass::Printf -- Prints a formatted string to the monochrome screen.                  *
 *   MonoClass::Scroll -- Scroll the monochrome screen up by the specified lines.              *
 *   MonoClass::Set_Cursor -- Sets the monochrome cursor to the coordinates specified.         *
 *   MonoClass::Text_Print -- Prints text to the monochrome object at coordinates indicated.   *
 *   MonoClass::View -- Brings the mono object to the main display.                            *
 *   MonoClass::~MonoClass -- The default destructor for a monochrome screen object.           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

//#pragma inline
#include	"function.h"
#include	"monoc.h"

#include	<stdlib.h>
#include	<stdio.h>
#include	<dos.h>
//#include	<mem.h>
#include	<stdarg.h>
#include	<string.h>


//extern void output(short port, short data);
//#pragma aux output parm [dx] [ax] =		\
//		"out	dx,al"		\
//		"inc	dx"			\
//		"mov	al,ah"		\
//		"out	dx,al"



int MonoClass::Enabled = 0;
MonoClass * MonoClass::PageUsage[MonoClass::MAX_MONO_PAGES] = {0,0,0,0,0,0,0,0};
//DOSSegmentClass MonoClass::MonoSegment(MonoClass::SEGMENT);
void * MonoClass::MonoSegment = (void*)0x000b0000;

/*
**	These are the IBM linedraw characters.
*/
MonoClass::BoxDataType const MonoClass::CharData[MonoClass::COUNT] = {
	{0xDA,0xC4,0xBF,0xB3,0xD9,0xC4,0xC0,0xB3},	// Single line
	{0xD5,0xCD,0xB8,0xB3,0xBE,0xCD,0xD4,0xB3},	// Double horz.
	{0xD6,0xC4,0xB7,0xBA,0xBD,0xC4,0xD3,0xBA},	// Double vert.
	{0xC9,0xCD,0xBB,0xBA,0xBC,0xCD,0xC8,0xBA}		// Double horz and vert.
};


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
MonoClass::MonoClass(void)
{
	int	index;

	Attrib = DEFAULT_ATTRIBUTE;		// Normal text color.
	X = Y = 0;
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
void MonoClass::Draw_Box(int x, int y, int w, int h, char attrib, BoxStyleType thick)
{
	CellType	cell;
	char	oldattrib = Attrib;

	if (!Enabled || !w || !h) return;

	cell.Attribute = attrib;

	/*
	**	Draw the horizontal lines.
	*/
	for (int xpos = 0; xpos < w-2; xpos++) {
		cell.Character = CharData[thick].TopEdge;
		Store_Cell(cell, x+xpos+1, y);
//		MonoSegment.Copy_Word_To(*(short*)&cell, Offset(x+xpos+1, y));
		cell.Character = CharData[thick].BottomEdge;
		Store_Cell(cell, x+xpos+1, y+h-1);
//		MonoSegment.Copy_Word_To(*(short*)&cell, Offset(x+xpos+1, y+h-1));
	}

	/*
	**	Draw the vertical lines.
	*/
	for (int ypos = 0; ypos < h-2; ypos++) {
		cell.Character = CharData[thick].LeftEdge;
		Store_Cell(cell, x, y+ypos+1);
//		MonoSegment.Copy_Word_To(*(short*)&cell, Offset(x, y+ypos+1));
		cell.Character = CharData[thick].RightEdge;
		Store_Cell(cell, x+w-1, y+ypos+1);
//		MonoSegment.Copy_Word_To(*(short*)&cell, Offset(x+w-1, y+ypos+1));
	}

	/*
	**	Draw the four corners.
	*/
	if (w > 1 && h > 1) {
		cell.Character = CharData[thick].UpperLeft;
		Store_Cell(cell, x, y);
//		MonoSegment.Copy_Word_To(*(short*)&cell, Offset(x, y));
		cell.Character = CharData[thick].UpperRight;
		Store_Cell(cell, x+w-1, y);
//		MonoSegment.Copy_Word_To(*(short*)&cell, Offset(x+w-1, y));
		cell.Character = CharData[thick].BottomRight;
		Store_Cell(cell, x+w-1, y+h-1);
//		MonoSegment.Copy_Word_To(*(short*)&cell, Offset(x+w-1, y+h-1));
		cell.Character = CharData[thick].BottomLeft;
		Store_Cell(cell, x, y+h-1);
//		MonoSegment.Copy_Word_To(*(short*)&cell, Offset(x, y+h-1));
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
	#ifdef FIX_ME_LATER
	int	pos = (y*COLUMNS)+x;

	if (!Enabled) return;

	X = (char)(x%COLUMNS);
	Y = (char)(y%LINES);

	if (Page == 0) {
		_DX = CONTROL_PORT;
		_AX = (short)(0x0E|(pos&0xFF00));
		asm {
			out	dx,al
			inc	dx
			mov	al,ah
			out	dx,al
		}

		_DX = CONTROL_PORT;
		_AX = (short)(0x0F|(pos<<8));
		asm {
			out	dx,al
			inc	dx
			mov	al,ah
			out	dx,al
		}

//		outportb(CONTROL_PORT,0x0E|(pos&0xFF00));
//		outportb(CONTROL_PORT,0x0F|(pos<<8));
	}

	#endif //FIX_ME_LATER

x=y;
y=x;
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
	CellType	cell;
//	int	offset;

	if (!Enabled) return;

	Set_Cursor(0, 0);

	cell.Attribute = Attrib;
	cell.Character = ' ';

//	offset = Offset(0, 0);
	for (int y = 0; y < LINES; y++) {
		for (int x = 0; x < COLUMNS; x++) {
			Store_Cell(cell, x, y);
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
	CellType	cell;

	if (!Enabled || lines <= 0) return;

	memmove( (void*)((long)MonoSegment + Offset(0, 0)),
				(void*)((long)MonoSegment + Offset(0, lines)),
				(LINES-lines)*COLUMNS*sizeof(CellType));

//	DOSSegmentClass::Copy(MonoSegment, Offset(0, lines), MonoSegment, Offset(0, 0), (LINES-lines)*COLUMNS*sizeof(CellType));

	Y--;
	cell.Attribute = Attrib;
	cell.Character = ' ';

	for (int l = LINES-lines; l < LINES; l++) {
		for (int index = 0; index < COLUMNS; index++) {
			Store_Cell(cell, index, l);
//			MonoSegment.Copy_Word_To(*(short*)&cell, Offset(index, l));
		}
	}
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


#ifdef NEVER
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
#endif


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
void MonoClass::Print(char const *ptr)
{
//	int optr;
	char startcol = X;
	char const * text;
	CellType	cell;

	if (!ptr || !Enabled) return;

	text = ptr;
	cell.Attribute = Attrib;
//	optr = Offset(X, Y);
	while (*text) {

		/*
		**	Sometimes the character string is used for cursor control instead
		**	of plain text output. Check for this case.
		*/
		switch (*text) {

			/*
			**	The "return" code behaves as it did in the old C library
			**	mono system. That is, it returns the cursor position to
			**	the next line but at the starting column of the print.
			*/
			case '\r':
				X = startcol;
				Y++;
				Scroll(Y-(LINES-1));
//				optr = Offset(X, Y);
				break;

			/*
			**	The "newline" code behaves like the console newline character.
			**	That is, it moves the cursor down one line and at the first
			**	column.
			*/
			case '\n':
				X = 0;
				Y++;
				Scroll(Y-(LINES-1));
//				optr = Offset(X, Y);
				break;

			/*
			**	All other characters are output directly and the cursor moves
			**	rightward to match. If the cursor wraps past the right
			**	edge is it moved to the next now down at left margin. If the
			**	cursor goes off the bottom of the display, the display is scrolled
			**	upward a line.
			*/
			default:
				cell.Character = *text;
				Store_Cell(cell, X, Y);
//				MonoSegment.Copy_Word_To(*(short*)&cell, optr);
//				optr += sizeof(CellType);

				X++;
				if (X >= COLUMNS) {
					X = 0;
					Y++;

					if (Y > (LINES-1)) {
						Scroll(Y-(LINES-1));
//						optr = Offset(X, Y);
					}
				}
				break;

		}
		text++;
	}

	Set_Cursor(X, Y);
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
void MonoClass::Text_Print(char const *text, int x, int y, char attrib)
{
	char	oldx = X;
	char	oldy = Y;
	char	oldattrib = Attrib;

	X = (char)x;
	Y = (char)y;
	Attrib = attrib;
	Print(text);
	Attrib = oldattrib;
	Set_Cursor(oldx, oldy);
}

#ifdef NEVER
void MonoClass::Text_Print(int text, int x, int y, char attrib)
{
	char	oldx = X;
	char	oldy = Y;
	char	oldattrib = Attrib;

	if (text != TXT_NONE) {
		X = (char)x;
		Y = (char)y;
		Attrib = attrib;
		Print(Text_String(text));
		Attrib = oldattrib;
		Set_Cursor(oldx, oldy);
	}
}

void MonoClass::Print(int text)
{
	Print(Text_String(text));
}
#endif


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
	memcpy((void*)((long)MonoSegment + src.Offset(0, 0)), (void*)((long)MonoSegment + Offset(0, 0)), SIZE_OF_PAGE);
//	DOSSegmentClass::Copy(MonoSegment, src.Offset(0, 0), MonoSegment, Offset(0,0), SIZE_OF_PAGE);
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
	MonoClass *displace;		// The page that is being displaced.

	if (Get_Current() == this) return;

	/*
	**	If the visible page is already assigned to a real monochrome page
	**	object, then it must be swapped with the new one.
	*/
	displace = Get_Current();
	if (displace) {
		char temp[SIZE_OF_PAGE];

		memcpy(&temp[0], MonoSegment, SIZE_OF_PAGE);
		memcpy(MonoSegment, (void*)((long)MonoSegment + Offset(0, 0)), SIZE_OF_PAGE);
		memcpy((void*)((long)MonoSegment + Offset(0, 0)), &temp[0], SIZE_OF_PAGE);

//		DOSSegmentClass::Swap(MonoSegment, Offset(0, 0), MonoSegment, 0, SIZE_OF_PAGE);
		displace->Page = Page;

	} else {

		/*
		**	Just copy the new page over since the display page is not assigned
		**	to a real monochrome page object.
		*/
		memcpy(MonoSegment, (void*)((long)MonoSegment + Offset(0, 0)), SIZE_OF_PAGE);
//		DOSSegmentClass::Copy(MonoSegment, Offset(0, 0), MonoSegment, 0, SIZE_OF_PAGE);
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
		MonoClass *mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		mono->Set_Cursor(x, y);
	}
}

int Mono_Printf(char const *string, ...)
{
	va_list	va;
	char buffer[256];

	buffer[0] = '\0';
	if (MonoClass::Is_Enabled()) {
		MonoClass *mono = MonoClass::Get_Current();
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


void Mono_Clear_Screen(void)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass *mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		mono->Clear();
	}
}

void Mono_Text_Print(void const *text, int x, int y, int attrib)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass *mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		mono->Text_Print((const char*)text, x, y, (char)attrib);
	}
}

void Mono_Draw_Rect(int x, int y, int w, int h, int attrib, int thick)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass *mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		mono->Draw_Box(x, y, w, h, (char)attrib, (MonoClass::BoxStyleType)thick);
	}
}

void Mono_Print(void const *text)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass *mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		mono->Print((const char*)text);
	}
}

int Mono_X(void)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass *mono = MonoClass::Get_Current();
		if (!mono) {
			mono = new MonoClass();
			mono->View();
		}
		return(short)mono->Get_X();
	}
	return(0);
}

int Mono_Y(void)
{
	if (MonoClass::Is_Enabled()) {
		MonoClass *mono = MonoClass::Get_Current();
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


#ifdef NEVER
int Mono_Printf(int string, ...)
{
	va_list	va;
	char buffer[256];

	buffer[0] = '\0';
	if (MonoClass::Is_Enabled()) {
		MonoClass *mono = MonoClass::Get_Current();
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
#endif

