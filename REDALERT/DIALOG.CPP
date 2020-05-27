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

/* $Header: /CounterStrike/DIALOG.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DIALOG.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : July 31, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Clip_Text_Print -- Prints text with clipping and <TAB> support.                           *
 *   Dialog_Box -- draws a dialog background box                                               *
 *   Display_Place_Building -- Displays the "place building" dialog box.                       *
 *   Display_Select_Target -- Displays the "choose target" prompt.                             *
 *   Display_Status -- Display the player scenario status box.                                 *
 *   Draw_Box -- Displays a highlighted box.                                                   *
 *   Draw_Caption -- Draws a caption on a dialog box.                                          *
 *   Fancy_Text_Print -- Prints text with a drop shadow.                                       *
 *   Plain_Text_Print -- Prints text without using a color scheme                              *
 *   Redraw_Needed -- Determine if sidebar needs to be redrawn.                                *
 *   Render_Bar_Graph -- Renders a specified bargraph.                                         *
 *   Simple_Text_Print -- Prints text with a drop shadow.                                      *
 *   Window_Box -- Draws a fancy box over the specified window.                                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#include   "defines.h"   //VG 10/17/96

unsigned char * Font_Palette(int color);


/***********************************************************************************************
 * Dialog_Box -- draws a dialog background box                                                 *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      x,y,w,h      the usual                                                                 *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/26/1995 BR : Created.                                                                  *
 *   07/31/1996 JLB : Uses shapes to draw the box.                                             *
 *=============================================================================================*/
void Dialog_Box(int x, int y, int w, int h)
{
// Try to expand the box a little taller and a little wider to make room for
// the dialog box graphics in the DOS version.
#ifndef WIN32
	x = max(0, x-4);
	y = max(0, y-4);
	w = min(w+8, 320-x);
	h = min(h+8, 200-y);
#endif

	WindowList[WINDOW_PARTIAL][WINDOWX] = x;
	WindowList[WINDOW_PARTIAL][WINDOWY] = y;
	WindowList[WINDOW_PARTIAL][WINDOWWIDTH] = w;
	WindowList[WINDOW_PARTIAL][WINDOWHEIGHT] = h;

	/*
	**	Always draw to the hidpage and then blit forward.
	*/
#ifdef WIN32
	GraphicViewPortClass * oldpage = Set_Logic_Page(HidPage);
#else
	GraphicBufferClass * oldpage = Set_Logic_Page(HidPage);
#endif

	/*
	**	Draw the background block.
	*/
	int cx = w/2;
	int cy = h/2;
	void const * shapedata = MFCD::Retrieve("DD-BKGND.SHP");
#ifdef WIN32
	CC_Draw_Shape(shapedata, 0, cx-312, cy-192, WINDOW_PARTIAL, SHAPE_WIN_REL);
	CC_Draw_Shape(shapedata, 1, cx,     cy-192, WINDOW_PARTIAL, SHAPE_WIN_REL);
	CC_Draw_Shape(shapedata, 2, cx-312, cy,     WINDOW_PARTIAL, SHAPE_WIN_REL);
	CC_Draw_Shape(shapedata, 3, cx,     cy,     WINDOW_PARTIAL, SHAPE_WIN_REL);
#else
	CC_Draw_Shape(shapedata, 0, cx-156, cy-96, WINDOW_PARTIAL, SHAPE_WIN_REL);
#endif
	/*
	**	Draw the side strips.
	*/
	shapedata = MFCD::Retrieve("DD-EDGE.SHP");
	for (int yy = 0; yy < h; yy += 6) {
		CC_Draw_Shape(shapedata, 0, 7*RESFACTOR,         yy, WINDOW_PARTIAL, SHAPE_WIN_REL);
		CC_Draw_Shape(shapedata, 1, w-((7+8)*RESFACTOR), yy, WINDOW_PARTIAL, SHAPE_WIN_REL);
	}

	/*
	**	Draw the border bars.
	*/
	shapedata = MFCD::Retrieve("DD-LEFT.SHP");
	CC_Draw_Shape(shapedata, 0, 0, cy-100*RESFACTOR, WINDOW_PARTIAL, SHAPE_WIN_REL);
	CC_Draw_Shape(shapedata, 0, 0, cy, WINDOW_PARTIAL, SHAPE_WIN_REL);

	shapedata = MFCD::Retrieve("DD-RIGHT.SHP");
	int rightx = w - (7*RESFACTOR);
#ifndef WIN32
	rightx--;
#endif
	CC_Draw_Shape(shapedata, 0, rightx, cy-100*RESFACTOR, WINDOW_PARTIAL, SHAPE_WIN_REL);
	CC_Draw_Shape(shapedata, 0, rightx, cy,					WINDOW_PARTIAL, SHAPE_WIN_REL);

	shapedata = MFCD::Retrieve("DD-BOTM.SHP");
	CC_Draw_Shape(shapedata, 0, cx-160*RESFACTOR, h-8*RESFACTOR, WINDOW_PARTIAL, SHAPE_WIN_REL);
	CC_Draw_Shape(shapedata, 0, cx, h-8*RESFACTOR, WINDOW_PARTIAL, SHAPE_WIN_REL);

	shapedata = MFCD::Retrieve("DD-TOP.SHP");
	CC_Draw_Shape(shapedata, 0, cx-160*RESFACTOR, 0, WINDOW_PARTIAL, SHAPE_WIN_REL);
	CC_Draw_Shape(shapedata, 0, cx, 0, WINDOW_PARTIAL, SHAPE_WIN_REL);

	/*
	**	Draw the corner caps.
	*/
	shapedata = MFCD::Retrieve("DD-CRNR.SHP");
	CC_Draw_Shape(shapedata, 0, 0,                  0, 					WINDOW_PARTIAL, SHAPE_WIN_REL);
	CC_Draw_Shape(shapedata, 1, w-(12*RESFACTOR-1), 0, 					WINDOW_PARTIAL, SHAPE_WIN_REL);
	CC_Draw_Shape(shapedata, 2, 0,                  h-(12*RESFACTOR), WINDOW_PARTIAL, SHAPE_WIN_REL);
	CC_Draw_Shape(shapedata, 3, w-(12*RESFACTOR-1), h-(12*RESFACTOR), WINDOW_PARTIAL, SHAPE_WIN_REL);

#ifdef WIN32
	WWMouse->Draw_Mouse(&HidPage);
	HidPage.Blit(SeenBuff, x, y, x, y, w, h, false);
	WWMouse->Erase_Mouse(&HidPage, FALSE);
#else
//	Shadow_Blit(0, 0, 320, 200, HidPage, SeenPage, Map.ShadowPage->Get_Buffer());
	Hide_Mouse();
	HidPage.Blit(SeenBuff);
	Show_Mouse();
//	Shadow_Blit(0, 0, 320, 200, HidPage, SeenPage, ((GraphicBufferClass*)Map.Shadow_Address())->Get_Buffer());
#endif
	Set_Logic_Page(oldpage);
}


/***********************************************************************************************
 * Draw_Box -- Displays a highlighted box.                                                     *
 *                                                                                             *
 *    This will draw a highlighted box to the logicpage. It can                                *
 *    optionally fill the box with a color as well. This is a low level                        *
 *    function and thus, it doesn't do any graphic mode color adjustments.                     *
 *                                                                                             *
 * INPUT:   x,y   -- Upper left corner of the box to be drawn (pixels).                        *
 *                                                                                             *
 *          w,h   -- Width and height of box (in pixels).                                      *
 *                                                                                             *
 *          up    -- Is the box rendered in the "up" stated?                                   *
 *                                                                                             *
 *          filled-- Is the box to be filled.                                                  *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1991 JLB : Created.                                                                 *
 *   05/30/1992 JLB : Embedded color codes.                                                    *
 *   07/31/1992 JLB : Depressed option added.                                                  *
 *=============================================================================================*/
void Draw_Box(int x, int y, int w, int h, BoxStyleEnum up, bool filled)
{
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	// Filler, Shadow, Hilite, Corner colors

	BoxStyleType const ButtonColors[BOXSTYLE_COUNT] = {
		{ scheme->Background, scheme->Highlight, scheme->Shadow,  scheme->Corners},	// Down
		{ scheme->Background, scheme->Shadow, scheme->Highlight,  scheme->Corners},	// Raised
		{ DKGREY, WHITE,  BLACK,  DKGREY},				// Disabled down
		{ DKGREY, BLACK,  LTGREY, DKGREY},				// Disabled up
		{ BLACK,  scheme->Box, scheme->Box,  BLACK},	// List box
		{ BLACK,  scheme->Box, scheme->Box,  BLACK},	// Dialog box
	};

	w--;
	h--;
	BoxStyleType const &style = ButtonColors[up];

	if (filled) {
		LogicPage->Fill_Rect( x, y, x+w, y+h, style.Filler);
	}

	switch (up) {
		case (BOXSTYLE_BOX):
			LogicPage->Draw_Rect(x, y, x+w, y+h, style.Highlight);
			break;

		case (BOXSTYLE_BORDER):
			LogicPage->Draw_Rect(x+1, y+1, x+w-1, y+h-1, style.Highlight);
			break;

		default:
			LogicPage->Draw_Line(x, y+h, x+w, y+h, style.Shadow);
			LogicPage->Draw_Line(x+w, y, x+w, y+h, style.Shadow);

			LogicPage->Draw_Line(x, y, x+w, y, style.Highlight);
			LogicPage->Draw_Line(x, y, x, y+h, style.Highlight);

			LogicPage->Put_Pixel(x, y+h, style.Corner);
			LogicPage->Put_Pixel(x+w, y, style.Corner);
			break;
	}
}


/***********************************************************************************************
 * Format_Window_String -- Separates a String into Lines.                                      *
 *   This function will take a long string and break it up into lines                          *
 *   which are not longer then the window width. Any character < ' ' is                        *
 *   considered a new line marker and will be replaced by a NULL.                              *
 *                                                                                             *
 * INPUT:      char *String - string to be formated.                                           *
 *             int maxlinelen - Max length of any line in pixels.                              *
 *                                                                                             *
 * OUTPUT:     int - number of lines string is.                                                *
 *                                                                                             *
 * WARNINGS:    The string passed in will be modified - NULLs will be put                      *
 *                into each position that will be a new line.                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/27/1992  SB : Created.                                                                 *
 *   05/18/1995 JLB : Greatly revised for new font system.                                     *
 *	  09/04/1996 BWG : Added '@' is treated as a carriage return for width calculations.		  *
 *=============================================================================================*/
int Format_Window_String(char * string, int maxlinelen, int & width, int & height)
{
	int	linelen;
	int	lines = 0;
	width	= 0;
	height = 0;

	// In no string was passed in, then there are no lines.
	if (!string) return(0);

	// While there are more letters left divide the line up.
	while (*string) {
		linelen = 0;
		height += FontHeight + FontYSpacing;
		lines++;

		/*
		**	Look for special line break character and force a line break when it is
		**	discovered.
		*/
		if (*string == '@') {
			*string = '\r';
		}

		// While the current line is less then the max length...
		while (linelen < maxlinelen && *string != '\r' && *string != '\0' && *string != '@') {
			linelen += Char_Pixel_Width(*string++);
		}

		// if the line is to long...
		if (linelen >= maxlinelen) {

			/*
			**	Back up to an appropriate location to break.
			*/
			while (*string != ' ' && *string != '\r' && *string != '\0' && *string != '@') {
				linelen -= Char_Pixel_Width(*string--);
			}

		}

		/*
		**	Record the largest width of the worst case string.
		*/
		if (linelen > width) {
			width = linelen;
		}

		/*
		**	Force a break at the end of the line.
		*/
		if (*string) {
		 	*string++ = '\r';
		}
	}
	return(lines);
}


/***********************************************************************************************
 * Window_Box -- Draws a fancy box over the specified window.                                  *
 *                                                                                             *
 *    This routine will draw a fancy (shaded) box over the specified                           *
 *    window. This is the effect used to give the polished look to                             *
 *    screen rectangles without having to use art.                                             *
 *                                                                                             *
 * INPUT:   window   -- Specified window to fill and border.                                   *
 *                                                                                             *
 *          style    -- The style to render the window.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The rendering is done to the LogicPage.                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/03/1992 JLB : Created.                                                                 *
 *   07/31/1992 JLB : Cool raised border effect.                                               *
 *   06/08/1994 JLB : Takes appropriate enumeration parameters.                                *
 *=============================================================================================*/
void Window_Box(WindowNumberType window, BoxStyleEnum style)
{
	int x = WindowList[window][WINDOWX];
	int y = WindowList[window][WINDOWY];
	int w = WindowList[window][WINDOWWIDTH];
	int h = WindowList[window][WINDOWHEIGHT];

	/*
	**	If it is to be rendered to the seenpage, then
	**	hide the mouse.
	*/
	if (LogicPage == (&SeenBuff)) Conditional_Hide_Mouse(x ,y, x+w, y+h);

	Draw_Box(x, y, w, h, style, true);

	/*
	**	Restore the mouse if it has been hidden and return.
	*/
	if (LogicPage == &SeenBuff) Conditional_Show_Mouse();
}


/***********************************************************************************************
 * Simple_Text_Print -- Prints text with a drop shadow.                                        *
 *                                                                                             *
 *    This routine functions like Text_Print, but will render a drop                           *
 *    shadow (in black).                                                                       *
 *                                                                                             *
 *    The C&C gradient font colors are as follows:															  *
 *			0		transparent (background)																		  *
 *			1		foreground color for mono-color fonts only												  *
 *			2		shadow under characters ("drop shadow")													  *
 *			3		shadow all around characters ("full shadow")												  *
 *			4-10	unused																								  *
 *			11		top row																								  *
 *			12		next row																								  *
 *			13		next row																								  *
 *			14		next row																								  *
 *			15		bottom row																							  *
 *                                                                                             *
 * INPUT:   text  -- Pointer to text to render.                                                *
 *                                                                                             *
 *          x,y   -- Pixel coordinate for to print text.                                       *
 *                                                                                             *
 *          fore  -- Foreground color.                                                         *
 *                                                                                             *
 *          back  -- Background color.                                                         *
 *                                                                                             *
 *          flag  -- Text print control flags.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1991 JLB : Created.                                                                 *
 *   10/26/94   JLB : Handles font X spacing in a more friendly manner.                        *
 *=============================================================================================*/
void Simple_Text_Print(char const * text, unsigned x, unsigned y, RemapControlType * fore, unsigned back, TextPrintType flag)
{
	static int		yspace=0;			// Y spacing adjustment for font.
	static int		xspace=0;			// Spacing adjustment for font.
	void const *	font=0;		// Font to use.
	int				shadow;				// Requested shadow value.
	unsigned char	fontpalette[16];	// Working font palette array.
	int forecolor;

	if (fore == NULL) {
		fore = &ColorRemaps[PCOLOR_RED];
	}

	/*
	** Init the font palette to the given background color
	*/
	memset(&fontpalette[0], back, 16);

	forecolor = fore->Color;

	/*
	**	A gradient font always requires special fixups for the palette
	*/
	int point = (flag & (TextPrintType)0x000F);
	if (point == TPF_VCR || point == TPF_6PT_GRAD || point == TPF_METAL12 || point == TPF_EFNT || point == TPF_TYPE) {

		/*
		** If a gradient palette is specified, copy the remap table directly, otherwise
		**	use the foreground color as the entire font remap color.
		*/
		if (flag & TPF_USE_GRAD_PAL) {
			memcpy(fontpalette, fore->FontRemap, 16);
			forecolor = fore->Color;
			if (point == TPF_TYPE) {
				forecolor = fontpalette[1];
			}
		} else {
			memset(&fontpalette[4], fore->Color, 12);
			forecolor = fore->Color;
		}

		/*
		** Medium color: set all font colors to a medium value.  This flag
		** overrides any gradient effects.
		*/
		if (flag & TPF_MEDIUM_COLOR) {
			forecolor = fore->Color;
			memset(&fontpalette[4], fore->Color, 12);
		}

		/*
		** Bright color: set all font colors to a bright value.  This flag
		** overrides any gradient effects.
		*/
		if (flag & TPF_BRIGHT_COLOR) {
			forecolor = fore->Bright;
			memset(&fontpalette[4], fore->BrightColor, 12);
		}
	}

	/*
	**	Change the current font if it differs from the font desired.
	*/
#ifdef WIN32
	xspace = 1;
#else
	xspace = 0;
#endif
	yspace = 0;

	switch (point) {
		case TPF_SCORE:
			font = ScoreFontPtr;
			break;

		case TPF_METAL12:
			font = Metal12FontPtr;
			//xspace += 1;
			break;

		case TPF_MAP:
			font = MapFontPtr;
			xspace -= 1;
			break;

		case TPF_VCR:
			font = VCRFontPtr;
			break;

		case TPF_6PT_GRAD:
			font = GradFont6Ptr;
			xspace -= 1;
			break;

		case TPF_3POINT:
			xspace += 1;
			font = Font3Ptr;
			flag = flag & ~(TPF_DROPSHADOW|TPF_FULLSHADOW|TPF_NOSHADOW);
			break;

		case TPF_6POINT:
			font = Font6Ptr;
			xspace -= 1;
			break;

		case TPF_EFNT:
			font = EditorFont;
#ifdef WIN32
			yspace += 1;
			xspace -= 1;
#endif
			xspace -= 1;
			break;

		case TPF_8POINT:
			font = Font8Ptr;
#ifdef WIN32
			xspace -= 2;
			yspace -= 4;
#else
			xspace -= 1;
			yspace -= 2;
#endif
			break;

		case TPF_LED:
#ifdef WIN32
			xspace -= 4;
#else
			xspace -= 2;
#endif
			font = FontLEDPtr;
			break;

		case TPF_TYPE:
			font = TypeFontPtr;
			xspace -= 1;

#ifdef WOLAPI_INTEGRATION
		   xspace -= 2;
		   yspace += 2;
#else	//	I am implicitly assuming that TPF_TYPE was no longer being used, before I came along, despite the following. ajw
#ifdef GERMAN
		   yspace += 4;   //VG 10/17/96
#endif
#endif

			break;

		default:
			font = FontPtr;
			break;
	}

	/*
	**	Change the current font palette according to the dropshadow flags.
	*/
	shadow = (flag & (TPF_NOSHADOW|TPF_DROPSHADOW|TPF_FULLSHADOW|TPF_LIGHTSHADOW));
	switch (shadow) {

		/*
		**	The text is rendered plain.
		*/
		case TPF_NOSHADOW:
			fontpalette[2] = back;
			fontpalette[3] = back;
			xspace -= 1;
#ifdef WIN32
			yspace -= 2;
#else
			yspace -= 1;
#endif
			break;

		/*
		**	The text is rendered with a simple
		**	drop shadow.
		*/
		case TPF_DROPSHADOW:
			fontpalette[2] = BLACK;
			fontpalette[3] = back;
			xspace -= 1;
			break;

		/*
		**	Special engraved text look for the options
		**	dialog system.
		*/
		case TPF_LIGHTSHADOW:
			fontpalette[2] = ((14 * 16) + 7)+1;
			fontpalette[3] = back;
			xspace -= 1;
			break;

		/*
		**	Each letter is surrounded by black. This is used
		**	when the text will be over a non-plain background.
		*/
		case TPF_FULLSHADOW:
			fontpalette[2] = BLACK;
			fontpalette[3] = BLACK;
			xspace -= 1;
			break;

		default:
			break;
	}
	if (point != TPF_TYPE) {
		fontpalette[0] = back;
		fontpalette[1] = fore->Color;
	}

	/*
	**	Set the font and spacing according to the values they should be.
	*/
	FontXSpacing = xspace;
	FontYSpacing = yspace;
	Set_Font(font);
	Set_Font_Palette(fontpalette);

	/*
	**	Display the (centered) message if there is one.
	*/
	if (text && *text) {
		switch (flag & (TPF_CENTER|TPF_RIGHT)) {
			case TPF_CENTER:
				x -= String_Pixel_Width(text)>>1;
				break;

			case TPF_RIGHT:
				x -= String_Pixel_Width(text);
				break;

			default:
				break;
		}

		if (x < (unsigned)LogicPage->Get_Width() && y < (unsigned)LogicPage->Get_Height()) {
			LogicPage->Print(text, x, y, forecolor, back);
//			LogicPage->Print(text, x, y, fore->Color, back);
		}
	}
}


/***********************************************************************************************
 * Fancy_Text_Print -- Prints text with a drop shadow.                                         *
 *                                                                                             *
 *    This routine functions like Text_Print, but will render a drop                           *
 *    shadow (in black).                                                                       *
 *                                                                                             *
 * INPUT:   text  -- Text number to print.                                                     *
 *                                                                                             *
 *          x,y   -- Pixel coordinate for to print text.                                       *
 *                                                                                             *
 *          fore  -- Foreground color.                                                         *
 *                                                                                             *
 *          back  -- Background color.                                                         *
 *                                                                                             *
 *          flag  -- Text print control flags.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine is much slower than normal text print and                          *
 *             if rendered to the SEENPAGE, the intermediate rendering                         *
 *             steps could be visible.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 JLB : Created                                                                  *
 *=============================================================================================*/
void Fancy_Text_Print(int text, unsigned x, unsigned y, RemapControlType * fore, unsigned back, TextPrintType flag, ...)
{
	char		buffer[512];		// Working staging buffer.
	va_list	arg;					// Argument list var.

	/*
	**	If the text number is valid, then process it.
	*/
	if (text != TXT_NONE) {
		va_start(arg, flag);

		/*
		**	The text string must be locked since the vsprintf function doesn't know
		**	how to handle EMS pointers.
		*/
		char const * tptr = Text_String(text);
		vsprintf(buffer, tptr, arg);
		va_end(arg);

		Simple_Text_Print(buffer, x, y, fore, back, flag);
	} else {

		/*
		**	Just the flags are to be changed, since the text number is TXT_NONE.
		*/
		Simple_Text_Print((char const *)0, x, y, fore, back, flag);
	}
}


/***********************************************************************************************
 * Fancy_Text_Print -- Prints text with a drop shadow.                                         *
 *                                                                                             *
 *    This routine functions like Text_Print, but will render a drop                           *
 *    shadow (in black).                                                                       *
 *                                                                                             *
 * INPUT:   text  -- Pointer to text to render.                                                *
 *                                                                                             *
 *          x,y   -- Pixel coordinate for to print text.                                       *
 *                                                                                             *
 *          fore  -- Foreground color.                                                         *
 *                                                                                             *
 *          back  -- Background color.                                                         *
 *                                                                                             *
 *          flag  -- Text print control flags.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine is much slower than normal text print and                          *
 *             if rendered to the SEENPAGE, the intermediate rendering                         *
 *             steps could be visible.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1991 JLB : Created.                                                                 *
 *   10/26/94   JLB : Handles font X spacing in a more friendly manner.                        *
 *   11/29/1994 JLB : Separated actual draw action.                                            *
 *=============================================================================================*/
void Fancy_Text_Print(char const * text, unsigned x, unsigned y, RemapControlType * fore, unsigned back, TextPrintType flag, ...)
{
	char		buffer[512];		// Working staging buffer.
	va_list	arg;					// Argument list var.

	/*
	**	If there is a valid text string pointer then build the final string into the
	**	working buffer before sending it to the simple string printing routine.
	*/
	if (text) {

		/*
		**	Since vsprintf doesn't know about EMS pointers, be sure to surround this
		**	call with locking code.
		*/
		va_start(arg, flag);
		vsprintf(buffer, text, arg);
		va_end(arg);

		Simple_Text_Print(buffer, x, y, fore, back, flag);
	} else {

		/*
		**	Just the flags are desired to be changed, so call the simple print routine with
		**	a NULL text pointer.
		*/
		Simple_Text_Print((char const *)0, x, y, fore, back, flag);
	}
}


/***********************************************************************************************
 * Clip_Text_Print -- Prints text with clipping and <TAB> support.                             *
 *                                                                                             *
 *    Use this routine to print text that that should be clipped at an arbitrary right margin  *
 *    as well as possibly recognizing <TAB> characters. Typical users of this routine would    *
 *    be list boxes.                                                                           *
 *                                                                                             *
 * INPUT:   text  -- Reference to the text to print.                                           *
 *                                                                                             *
 *          x,y   -- Pixel coordinate of the upper left corner of the text position.           *
 *                                                                                             *
 *          fore  -- The foreground color to use.                                              *
 *                                                                                             *
 *          back  -- The background color to use.                                              *
 *                                                                                             *
 *          flag  -- The text print flags to use.                                              *
 *                                                                                             *
 *          width -- The maximum pixel width to draw the text. Extra characters beyond this    *
 *                   point will not be printed.                                                *
 *                                                                                             *
 *          tabs  -- Optional pointer to a series of pixel tabstop positions.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Conquer_Clip_Text_Print(char const * text, unsigned x, unsigned y, RemapControlType * fore, unsigned back, TextPrintType flag, int width, int const * tabs)
{
	char buffer[512];

	if (text) {
		strcpy(buffer, text);

		/*
		**	Set the font and spacing characteristics according to the flag
		**	value passed in.
		*/
		//PG_TO_FIX
		//Simple_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, flag);

		char * source = &buffer[0];
		unsigned offset = 0;
		int processing = true;
		while (processing && offset < (unsigned) width) {
			char * ptr = strchr(source, '\t');

			/*
			**	Zap the tab character. It will be processed later.
			*/
			if (ptr) {
				*ptr = '\0';
			}

			if (*source) {

				/*
				**	Scan forward until the end of the string is reached or the
				**	maximum width, whichever comes first.
				*/
				int w = 0;
				char * bptr = source;
				do {
					w += Char_Pixel_Width(*bptr++);
				} while (*bptr && offset+w < (unsigned)width);

				/*
				**	If the maximum width has been exceeded, then remove the last
				**	character and signal that further processing is not necessary.
				*/
				if (offset+w >= (unsigned)width) {
					bptr--;
					w -= Char_Pixel_Width(*bptr);
					*bptr = '\0';
					processing = 0;
				}

				/*
				**	Print this text block and advance the offset accordingly.
				*/
				Simple_Text_Print(source, x+offset, y, fore, back, flag);
				offset += w;
			}

			/*
			**	If a <TAB> was the terminator for this text block, then advance
			**	to the next tabstop.
			*/
			if (ptr) {
				if (tabs) {
					while ((int)offset > *tabs) {
						tabs++;
					}
					offset = *tabs;
				} else {
					offset = ((offset+1 / 50) + 1) * 50;
				}
				source = ptr+1;
			} else {
				break;
			}
		}
	}
}

/***************************************************************************
 * Plain_Text_Print -- Prints text without using a color scheme            *
 *                                                                         *
 * INPUT:                                                                  *
 *		text		text to print																*
 *		x,y		coords to print at														*
 *		fore		desired foreground color												*
 *		back		desired background color												*
 *		flag		text print control flags												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Do not use the gradient control flag with this routine!  For 			*
 *		a gradient appearance, use Fancy_Text_Print.									*
 *		Despite this routine's name, it is actually faster to call				*
 *		Fancy_Text_Print than this routine.												*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/05/1996 BRR : Created.                                             *
 *=========================================================================*/
void Plain_Text_Print(int text, unsigned x, unsigned y, unsigned fore, unsigned back, TextPrintType flag, ...)
{
	RemapControlType scheme;

	memset(&scheme, 0, sizeof(RemapControlType));
	memset(&(scheme.FontRemap[4]), fore, 12);

	scheme.BrightColor = fore;
	scheme.Color = fore;
	scheme.Shadow = fore;
	scheme.Background = fore;
	scheme.Corners = fore;
	scheme.Highlight = fore;
	scheme.Box = fore;
	scheme.Bright = fore;
	scheme.Underline = fore;
	scheme.Bar = fore;

	Fancy_Text_Print(text, x, y, &scheme, back, flag);
}


/***************************************************************************
 * Plain_Text_Print -- Prints text without using a color scheme            *
 *                                                                         *
 * INPUT:                                                                  *
 *		text		text to print																*
 *		x,y		coords to print at														*
 *		fore		desired foreground color												*
 *		back		desired background color												*
 *		flag		text print control flags												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Do not use the gradient control flag with this routine!  For 			*
 *		a gradient appearance, use Fancy_Text_Print.									*
 *		Despite this routine's name, it is actually faster to call				*
 *		Fancy_Text_Print than this routine.												*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/05/1996 BRR : Created.                                             *
 *=========================================================================*/
void Plain_Text_Print(char const * text, unsigned x, unsigned y, unsigned fore, unsigned back, TextPrintType flag, ...)
{
	RemapControlType scheme;

	memset(&scheme, 0, sizeof(RemapControlType));
	memset(&(scheme.FontRemap[4]), fore, 12);

	scheme.BrightColor = fore;
	scheme.Color = fore;
	scheme.Shadow = fore;
	scheme.Background = fore;
	scheme.Corners = fore;
	scheme.Highlight = fore;
	scheme.Box = fore;
	scheme.Bright = fore;
	scheme.Underline = fore;
	scheme.Bar = fore;

	Fancy_Text_Print(text, x, y, &scheme, back, flag);
}



unsigned char * Font_Palette(int color)
{
	static unsigned char _fpalette[16];

	memset(_fpalette, '\0', sizeof(_fpalette));
	memset(&_fpalette[11], color, 5);
	return(_fpalette);
}



/***********************************************************************************************
 * Draw_Caption -- Draws a caption on a dialog box.                                            *
 *                                                                                             *
 *    This routine draws the caption text and any fancy filigree that the dialog may require.  *
 *                                                                                             *
 * INPUT:   text  -- The text of the caption. This is the text number.                         *
 *                                                                                             *
 *          x,y   -- The dialog box X and Y pixel coordinate of the upper left corner.         *
 *                                                                                             *
 *          w     -- The width of the dialog box (in pixels).                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Draw_Caption(int text, int x, int y, int w)
{
	Draw_Caption(Text_String(text), x, y, w);
}


void Draw_Caption(char const * text, int x, int y, int w)
{
	/*
	**	Draw the caption.
	*/
	if (text != NULL && *text != '\0') {
		if (Debug_Map) {
			Fancy_Text_Print(text, w/2 + x, (2 * RESFACTOR) + y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_CENTER|TPF_EFNT|TPF_USE_GRAD_PAL|TPF_NOSHADOW);
		} else {
			Fancy_Text_Print(text, w/2 + x, (8 * RESFACTOR) + y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_CENTER|TPF_TEXT);
			int length = String_Pixel_Width(text);
			LogicPage->Draw_Line((x+(w/2))-(length/2), y+FontHeight+FontYSpacing + (8 * RESFACTOR), (x+(w/2))+(length/2),
				y+FontHeight+FontYSpacing + (8 * RESFACTOR), GadgetClass::Get_Color_Scheme()->Box);
		}
	}
}
