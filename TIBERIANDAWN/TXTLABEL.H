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

/* $Header:   F:\projects\c&c\vcs\code\txtlabel.h_v   1.14   16 Oct 1995 16:46:08   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TXTLABEL.H                                                   *
 *                                                                                             *
 *                   Programmer : Bill Randolph																  *
 *                                                                                             *
 *                   Start Date : 02/06/95                                                     *
 *                                                                                             *
 *                  Last Update : February 6, 1995 [BR]													  *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef TXTLABEL_H
#define TXTLABEL_H

class TextLabelClass : public GadgetClass
{
	public:
		/*
		** Constructor/Destructor
		*/
		TextLabelClass(char *txt, int x, int y, int color, TextPrintType style);

		/*
		** Overloaded draw routine
		*/
		virtual int Draw_Me(int forced = false);

		/*
		** Sets the displayed text of the label
		*/
		virtual void Set_Text(char *txt) {Text = txt;};

		/*
		** General-purpose data field
		*/
		unsigned long 	UserData;
		TextPrintType 	Style;
		char 				*Text;
		int 				Color;
		int 				PixWidth;
		char 				Segments;
		unsigned short	CRC;
};

#endif

