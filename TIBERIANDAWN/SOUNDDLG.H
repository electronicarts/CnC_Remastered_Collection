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

/* $Header:   F:\projects\c&c\vcs\code\sounddlg.h_v   2.18   16 Oct 1995 16:46:48   JOE_BOSTIC  $ */
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
		
		LISTBOX_X=1,
		LISTBOX_Y=54,
		LISTBOX_W=290,
		LISTBOX_H=72,

		BUTTON_WIDTH=85,
		BUTTON_X=OPTION_WIDTH-(BUTTON_WIDTH+7),	 				// Options button x pos
		BUTTON_Y=130, 					// Options button y pos

		STOP_X=5,						// Stop button X.
		STOP_Y=129,						//	Stop button Y.

		PLAY_X=23,		
		PLAY_Y=129,		

		ONOFF_WIDTH=25,
#ifdef GERMAN
		SHUFFLE_X=79,//BGA:91,
#else
#ifdef FRENCH
		SHUFFLE_X=99,
#else
		SHUFFLE_X=91,
#endif
#endif
		SHUFFLE_Y=130,

#ifdef FRENCH
		REPEAT_X=174,
#else
		REPEAT_X=166,
#endif
		REPEAT_Y=130,

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
//		SLIDER_SPEECH,
		SLIDER_SOUND,
		BUTTON_LISTBOX,
	};

	public:
		SoundControlsClass(void) {}
		void Process(void);
		int  Init(void);

	private:

		int	Option_Width;
		int	Option_Height;
		
		int	Option_X;
		int	Option_Y;
		
		int	Listbox_X;
		int	Listbox_Y;
		int	Listbox_W;
		int	Listbox_H;

		int	Button_Width;
		int	Button_X;
		int	Button_Y;

		int	Stop_X;
		int	Stop_Y;

		int	Play_X;
		int	Play_Y;

		int	OnOff_Width;

		int	Shuffle_X;
		int	Shuffle_Y;

		int	Repeat_X;
		int	Repeat_Y;

		int	MSlider_X;
		int	MSlider_Y;
		int	MSlider_W;
		int	MSlider_Height;

		int	FXSlider_X;
		int	FXSlider_Y;
		int	FXSlider_W;
		int	FXSlider_Height;

};

#endif
