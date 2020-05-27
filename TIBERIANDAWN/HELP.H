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

/* $Header:   F:\projects\c&c\vcs\code\help.h_v   2.17   16 Oct 1995 16:46:28   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : HELP.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/18/94                                                     *
 *                                                                                             *
 *                  Last Update : November 18, 1994 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef HELP_H
#define HELP_H

#include	"tab.h"

class HelpClass: public TabClass
{
	public:
		HelpClass(void);

		/*
		** Initialization
		*/
		virtual void Init_Clear(void);						// Clears all to known state

		virtual void Draw_It(bool complete=false);
		virtual void AI(KeyNumType &input, int x, int y);
		virtual bool Scroll_Map(DirType facing, int &distance, bool really);
		virtual void Set_Tactical_Position(COORDINATE coord);

		void Help_Text(int text, int x=-1, int y=-1, int color=LTGREY, bool quick=false, int cost = 0);
		void Set_Cost(int cost);
		short const * Overlap_List(void) const;

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

	private:

		static char const *HelpText;
		int HelpX;
		int HelpY;
		int HelpWidth;


		void Set_Text(int text);

		/*
		**	If the help text is right justified (as with the help text that pops up over the
		**	sidebar icons), then this flag is set to true.
		*/
		unsigned IsRight:1;

		/*
		**	If the optional second line of text that displays cost is desired, then this
		**	value will be non-zero. Typically, this is true when the help text is associated
		**	with one of the sidebar construction icons.
		*/
		int Cost;

		/*
		**	This is the recorded position of the cursor at the time the help text
		**	pops up. The help text is rendered as an offset from this pixel position.
		*/
		int X;
		int Y;

		/*
		**	This is the draw X and Y coordinate. This position is relative to the X and
		**	Y coordinates but adjusted for screen edges as necessary.
		*/
		int DrawX;
		int DrawY;

		/*
		**	The width of the help text (in pixels) is stored here. This is a convenience
		**	since calculating the width takes a bit of time.
		*/
		int Width;

		/*
		**	The text number of the help text to display is held here. If no text is to be
		**	displayed, then this value will be TXT_NONE.
		*/
		int Text;

		/*
		**	This is the background color to use for the help text. It can change according
		**	to the message displayed.
		*/
		int Color;

		/*
		**	This countdown timer controls when the help text will pop up. If the mouse
		**	remains stationary while this countdown timer expires, then the help text
		**	will pop up.
		*/
		static CountDownTimerClass CountDownTimer;

		/*
		**	This is a calculated cell offset list (from the Map.TacticalCell) that indicates
		**	which cells are under the help text and thus which cells need to be redrawn if
		**	the help text is to be erased.
		*/
		//static short const OverlapList[30];
		static short OverlapList[30];		// Can't be const - it's expected to be written to. ST - 2/7/2019 5:16PM

		enum HelpClassEnum {
			HELP_DELAY=TIMER_SECOND*1,				// The countdown timer delay before help text pops up.
			Y_OFFSET=0,					// The Y pixel offset from cursor for help text print.
			X_OFFSET=10,				// The X pixel offset from cursor for help text print.
		};
};

#endif