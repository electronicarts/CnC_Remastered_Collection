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

/* $Header:   F:\projects\c&c\vcs\code\dialog.cpv   2.17   16 Oct 1995 16:51:50   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : May 18, 1995 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Clip_Text_Print -- Prints text with clipping and <TAB> support.                           *
 *   Dialog_Box -- draws a dialog background box                                               *
 *   Display_Place_Building -- Displays the "place building" dialog box.                       *
 *   Display_Select_Target -- Displays the "choose target" prompt.                             *
 *   Display_Status -- Display the player scenario status box.                                 *
 *   Draw_Box -- Displays a highlighted box.                                                   *
 *   Fancy_Text_Print -- Prints text with a drop shadow.                                       *
 *   Redraw_Needed -- Determine if sidebar needs to be redrawn.                                *
 *   Render_Bar_Graph -- Renders a specified bargraph.                                         *
 *   Simple_Text_Print -- Prints text with a drop shadow.                                      *
 *   Window_Box -- Draws a fancy box over the specified window.                                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


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
 *=============================================================================================*/
void Dialog_Box(int x, int y, int w, int h)
{
	Draw_Box( x, y, w, h, BOXSTYLE_GREEN_BORDER, true);
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
extern void CC_Texture_Fill (void const *shapefile, int shapenum, int xpos, int ypos, int width, int height);

void Draw_Box(int x, int y, int w, int h, BoxStyleEnum up, bool filled)
{
	static BoxStyleType const ButtonColors[BOXSTYLE_COUNT] = {

		//Filler, Shadow, Hilite, Corner colors

		{ LTGREY, WHITE,  DKGREY, LTGREY},		// 0 Button is down.
		{ LTGREY, DKGREY, WHITE,  LTGREY},		// 1 Button is up w/border.
		{ LTBLUE, BLUE,   LTCYAN, LTBLUE},		// 2 Raised blue.
		{ DKGREY, WHITE,  BLACK,  DKGREY},		// 3 Button is disabled down.
		{ DKGREY, BLACK,  WHITE,  LTGREY},		// 4 Button is disabled up.
		{ LTGREY, DKGREY, WHITE,  LTGREY},		// 5 Button is up w/arrows.
		//{ CC_GREEN_BKGD, CC_LIGHT_GREEN, CC_GREEN_SHADOW,  CC_GREEN_CORNERS },	// 6 Button is down.
		//{ CC_GREEN_BKGD, CC_GREEN_SHADOW, CC_LIGHT_GREEN,  CC_GREEN_CORNERS },	// 7 Button is up w/border.
		{ CC_GREEN_BKGD, 14, 12,  13 },	// 6 Button is down.
		{ CC_GREEN_BKGD, 12, 14,  13 },	// 7 Button is up w/border.
		{ DKGREY, WHITE,  BLACK,  DKGREY},		// 8 Button is disabled down.
		{ DKGREY, BLACK,  LTGREY, DKGREY},		// 9 Button is disabled up.
		//{ BLACK,  CC_GREEN_BOX, CC_GREEN_BOX,  BLACK},	// 10 List box.
		//{ BLACK,  CC_GREEN_BOX, CC_GREEN_BOX,  BLACK},	// 11 Menu box.
		{ BLACK,  14, 14,  BLACK},	// 10 List box.
		{ BLACK,  14, 14,  BLACK},	// 11 Menu box.
	};

	w--;
	h--;
	BoxStyleType const &style = ButtonColors[up];

	if (filled) {
		if (style.Filler == CC_GREEN_BKGD){
			CC_Texture_Fill (MixFileClass::Retrieve("BTEXTURE.SHP"), InMainLoop, x, y, w, h);
		}else{
			LogicPage->Fill_Rect( x, y, x+w, y+h, style.Filler);
		}
	}

	switch ( up ) {
		case ( BOXSTYLE_GREEN_BOX ):
			LogicPage->Draw_Rect(x, y, x+w, y+h, style.Highlight);
			break;

		case ( BOXSTYLE_GREEN_BORDER ):
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

		// While the current line is less then the max length...
		while (linelen < maxlinelen && *string != '\r' && *string != '\0') {
			linelen += Char_Pixel_Width(*string++);
		}

		// if the line is to long...
		if (linelen >= maxlinelen) {

			/*
			**	Back up to an appropriate location to break.
			*/
			while (*string != ' ' && *string != '\r' && *string != '\0') {
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
	int	x,y,w,h;			// Window dimensions.
	int	border;			// Width of border.

	static int _border[BOXSTYLE_COUNT][2] = {
		{0,0},				// 0 Simple beveled edge.
		{2,4},				// 1 Wide raised border.
		{1,1},				// 2 Thick beveled edge.
		{2,1},				// 3 Thin raised border.
		{0,0},				// 4 Simple beveled edge.
		{20,0},				// 5 Simple beveled edge.
		{0,0},				// 6 Simple beveled edge.
		{2,4},				// 7 Wide raised border.
		{0,0},				// 8 Simple beveled edge.
		{20,0},				// 9 Simple beveled edge.
		{0,1}					// 10 Simple 1 pixel box.
	};

	x = WindowList[window][WINDOWX]<<3;
	y = WindowList[window][WINDOWY];
	w = WindowList[window][WINDOWWIDTH]<<3;
	h = WindowList[window][WINDOWHEIGHT];

	/*
	**	If it is to be rendered to the seenpage, then
	**	hide the mouse.
	*/
	if (LogicPage == (&SeenBuff)) Conditional_Hide_Mouse(x,y,x+w,y+h);

	Draw_Box(x, y, w, h, style, true);
	border = _border[style][1];

	/*
	**	Draw the second border if requested.
	*/
	if (border) {
		Draw_Box(x+border, y+border, w-(border<<1), h-(border<<1), style, false);
	}

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
void Simple_Text_Print(char const *text, unsigned x, unsigned y, unsigned fore, unsigned back, TextPrintType flag)
{
	static int		yspace=0;			// Y spacing adjustment for font.
	static int		xspace=0;			// Spacing adjustment for font.
	void const *	font=0;		// Font to use.

////////////////#if (0)
 	static unsigned char _textfontpal[16][16] = {
		{ 0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0	},
		{ 0, 26,  0,  0,   0,  0,  0,  0,   0,  0,  0, 27,  27, 26, 25, 24	},
		{ 0,135,  0,  0,   0,  0,  0,  0,   0,  0,  0,136, 136,135,119,  2	},
		{ 0,159,  0,  0,   0,  0,  0,  0,   0,  0,  0,142, 143,159,41 ,167	},

		{ 0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0	},
		{ 0,157,  0,  0,   0,  0,  0,  0,   0,  0,  0,180, 180,157,158,  5	},
		{ 0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0	},
		{ 0,179,  0,  0,   0,  0,  0,  0,   0,  0,  0,180, 180,179,178,176	},

		{ 0,123,  0,  0,   0,  0,  0,  0,   0,  0,  0,122, 122,123,125,127	},
		{ 0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0	},
		{ 0,203,  0,  0,   0,  0,  0,  0,   0,  0,  0,204, 204,203,202,201	},
		{ 0,  1,  4,166,  41,  3,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0	},

		{ 0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0	},

		{ 0,203,  0,  0,   0,  0,  0,  0,   0,  0,  0,204, 204,203,202,201	},
		{ 0,203,  0,  0,   0,  0,  0,  0,   0,  0,  0,204, 204,203,202,201	},

		{ 0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0	},
	};
	static unsigned char _textpalmedium[16] = {
		  0,  25, 119,41, 0,  158,0,  178, 125,0,  202,0,   0,  0,  0,  0
	};

	static unsigned char _textpalbright[16] = {
		  0,  24, 2,  4,   0,  5,  0,  176, 127,0,  201,0,   0,  0,  0,  0
	};
///////////////////////#endif	//(0)

	int				point;				// Requested font size.
	int				shadow;				// Requested shadow value.
	unsigned char	fontpalette[16];	// Working font palette array.
	memset(&fontpalette[0], back, 16);

	if ((flag & 0xf) == TPF_VCR) {
		fontpalette[3] = 12;
		fontpalette[9] = 15;
		fontpalette[10] = 200;
		fontpalette[11] = 201;
		fontpalette[12] = 202;
		fontpalette[13] = 203;
		fontpalette[14] = 204;
		fontpalette[15] = 205;
	}

	char *tempstr = NULL;

	if (text){
		/*
		** remove any 0xff characters from the string
		*/
		tempstr = new char [strlen (text)+1];
		char *tempptr = tempstr;

		for ( int i=0 ; i<(int)strlen(text)+1 ; i++ ) {
			if (text[i] != -1){
				*tempptr = text[i];
				tempptr++;
			}
		}
	}

	/*
	**	A gradient font always requires special fixups for the palette.
	*/
	if ((flag & 0xf) == TPF_6PT_GRAD ||
		 (flag & 0xf) == TPF_GREEN12_GRAD ||
		 (flag & 0xf) == TPF_GREEN12) {
		/*
		**	If a gradient palette was requested, then fill in the font palette array
		**	according to the color index specified.
		*/
		if (flag & TPF_USE_GRAD_PAL) {
			memcpy(&fontpalette[0], _textfontpal[ (fore & 0x0f) ], 16);
		} else {

			/*
			**	Special adjustment for fonts that have gradient artwork. When there is
			**	no special gradient effect desired, then set the font color based on the
			**	forground color specified.
			*/
			memset(&fontpalette[4], fore, 12);
		}

		if (flag & TPF_MEDIUM_COLOR) {
			fore = _textpalmedium[fore & 0x0F];
			memset(&fontpalette[4], fore, 12);
		}else{
			if (flag & TPF_BRIGHT_COLOR) {
				fore = _textpalbright[fore & 0x0F];
				memset(&fontpalette[4], fore, 12);
			}else{
				fore = fontpalette[1];
			}
		}
	}


	/*
	**	Change the current font if it differs from the font desired.
	*/
	point = (flag & (TextPrintType)0x000F);
	xspace = 1;
	yspace = 0;

	switch (point) {
		case TPF_GREEN12:
			font = Green12FontPtr;
			break;

		case TPF_GREEN12_GRAD:
			font = Green12GradFontPtr;
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
			//yspace -= 1;
			break;

		case TPF_3POINT:
			xspace += 1;
			font = Font3Ptr;
			flag = flag & ~(TPF_DROPSHADOW|TPF_FULLSHADOW|TPF_NOSHADOW);
			break;

		case TPF_6POINT:
			font = Font6Ptr;
			xspace -= 1;
			//yspace -= 1;
			break;

		case TPF_8POINT:
			font = Font8Ptr;
			xspace -= 2;
			yspace -= 4;
			break;

		case TPF_LED:
			xspace -= 4;
			font = FontLEDPtr;
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
			yspace -= 2;
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
	fontpalette[0] = back;
	fontpalette[1] = fore;

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
				x -= String_Pixel_Width(tempstr)>>1;
				break;

			case TPF_RIGHT:
				x -= String_Pixel_Width(tempstr);
				break;

			default:
				break;
		}

		if (x < (unsigned)SeenBuff.Get_Width() && y < (unsigned)SeenBuff.Get_Height()) {
			LogicPage->Print(tempstr, x, y, fore, back);
		}
	}
	if (tempstr){
		delete [] tempstr;
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
void Fancy_Text_Print(int text, unsigned x, unsigned y, unsigned fore, unsigned back, TextPrintType flag, ...)
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
void Fancy_Text_Print(char const *text, unsigned x, unsigned y, unsigned fore, unsigned back, TextPrintType flag, ...)
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
void Conquer_Clip_Text_Print(char const *text, unsigned x, unsigned y, unsigned fore, unsigned back, TextPrintType flag, unsigned width, int const * tabs)
{
	char buffer[512];

	if (text) {
		strcpy(buffer, text);

		/*
		**	Set the font and spacing characteristics according to the flag
		**	value passed in.
		*/
		Simple_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, flag);

		char * source = &buffer[0];
		unsigned offset = 0;
		int processing = true;
		while (processing && offset < width) {
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
				} while(*bptr && offset+w < width);

				/*
				**	If the maximum width has been exceeded, then remove the last
				**	character and signal that further processing is not necessary.
				*/
				if (offset+w >= width) {
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
					while (offset > (unsigned) *tabs) {
						tabs++;
					}
					offset = (unsigned) *tabs;
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
