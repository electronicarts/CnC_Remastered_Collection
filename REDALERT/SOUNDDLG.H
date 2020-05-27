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

/* $Header: /CounterStrike/SOUNDDLG.H 1     3/03/97 10:25a Joe_bostic $ */
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

#ifndef SOUNDDLG_H
#define SOUNDDLG_H

#include "gadget.h"

class SoundControlsClass
{
	enum SoundControlsClassEnums {
#ifdef FRENCH
		OPTION_WIDTH=308,
#else
		OPTION_WIDTH=292,
#endif
		OPTION_HEIGHT=146,

		OPTION_X=((320 - OPTION_WIDTH) / 2),
		OPTION_Y=(200 - OPTION_HEIGHT) / 2,

		LISTBOX_X=17,
		LISTBOX_Y=54,
		LISTBOX_W=OPTION_WIDTH-(LISTBOX_X*2),
		LISTBOX_H=72,

		BUTTON_WIDTH=70,
		BUTTON_X=OPTION_WIDTH-(BUTTON_WIDTH+17),	 				// Options button x pos
		BUTTON_Y=128, 					// Options button y pos

		STOP_X=17,						// Stop button X.
		STOP_Y=128,						//	Stop button Y.

		PLAY_X=35,
		PLAY_Y=128,

		ONOFF_WIDTH=25,
#ifdef GERMAN
		SHUFFLE_X=79,//BGA:91,
#else
#ifdef FRENCH
		SHUFFLE_X=99,
#else
		SHUFFLE_X=97,
#endif
#endif
		SHUFFLE_Y=128,

#ifdef FRENCH
		REPEAT_X=169,
#else
		REPEAT_X=164,
#endif
		REPEAT_Y=128,

		MSLIDER_X=147,
		MSLIDER_Y=28,
		MSLIDER_W=108,
		MSLIDER_HEIGHT=5,

		FXSLIDER_X=147,
		FXSLIDER_Y=40,
		FXSLIDER_W=108,
		FXSLIDER_HEIGHT=5,

		BUTTON_STOP = 605,
		BUTTON_PLAY,
		BUTTON_SHUFFLE,
		BUTTON_REPEAT,
		BUTTON_OPTIONS,
		SLIDER_MUSIC,
		SLIDER_SOUND,
		BUTTON_LISTBOX,
	};

	public:
		SoundControlsClass(void) {}
		void Process(void);
};

#endif
