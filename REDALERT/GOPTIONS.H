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

/* $Header: /CounterStrike/GOPTIONS.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OPTIONS.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 8, 1994                                                 *
 *                                                                                             *
 *                  Last Update : June 8, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef GOPTIONS_H
#define GOPTIONS_H

#include "options.h"
#include "gadget.h"


class GameOptionsClass : public OptionsClass {
		enum GameOptionsButtonEnum {
			BUTTON_LOAD=1,
			BUTTON_SAVE,
			BUTTON_DELETE,
			BUTTON_GAME,
			BUTTON_QUIT,
#ifdef FIXIT_VERSION_3		//	Stalemate games.
			BUTTON_DRAW,
#endif
			BUTTON_RESUME,
			BUTTON_RESTATE,

			BUTTON_COUNT
		};

		enum GameOptionsEnum {
			OPTION_WIDTH=(216+8),
			OPTION_HEIGHT=100,
			OPTION_X=((320 - (216+8)) / 2),
			OPTION_Y=((200 - 100) / 2),
#ifdef FRENCH
			BUTTON_WIDTH=142,
#else
			BUTTON_WIDTH=130,
#endif
			NUMBER_OF_BUTTONS=6,		//	ajw Not used.
			CAPTION_Y_POS=5,
			BUTTON_Y=21,
			BORDER1_LEN=72,
			BORDER2_LEN=16,
			BUTTON_RESUME_Y=(100-15)
		};

	public:
		GameOptionsClass(void): OptionsClass () { };
		void  Adjust_Variables_For_Resolution(void);
		void Process(void);

	private:
		int	OptionWidth;
		int 	OptionHeight;
		int 	OptionX;
		int 	OptionY;
		int 	ButtonWidth;
		int 	OButtonHeight;
		int 	CaptionYPos;
		int 	ButtonY;
		int 	Border1Len;
		int 	Border2Len;
		int 	ButtonResumeY;

};

#endif
