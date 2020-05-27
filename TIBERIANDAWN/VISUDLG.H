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

/* $Header:   F:\projects\c&c\vcs\code\visudlg.h_v   2.15   16 Oct 1995 16:47:46   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : VISUDLG.H                                                    *
 *                                                                                             *
 *                   Programmer : Maria del Mar McCready Legg											  *
 *											 Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : Jan 8, 1995                                                  *
 *                                                                                             *
 *                  Last Update : Jan 18, 1995   [MML]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *---------------------------------------------------------------------------------------------*/

#ifndef VISUDLG_H
#define VISUDLG_H

#include "gadget.h"

class VisualControlsClass 
{
	private:

		enum VisualControlEnums {
			BUTTON_BRIGHTNESS=1,
			BUTTON_COLOR,
			BUTTON_CONTRAST,
			BUTTON_TINT,
			BUTTON_RESET,
			BUTTON_OPTIONS,							// Button number for "Options menu"
			OPTION_WIDTH=216,							// Width of dialog box.
			OPTION_HEIGHT=122,						// Height of dialog box.
			OPTION_X=(((320 - OPTION_WIDTH) / 2)),
			OPTION_Y=((200 - OPTION_HEIGHT) / 2),
			TEXT_X=OPTION_X+28,			 			// Title's x pos
			TEXT_Y=OPTION_Y+30,  					// Add 11 for each following line
			SLIDER_X=OPTION_X+105, 					// Slider's x pos
			SLIDER_Y=OPTION_Y+30,  					// Add 11 for each following line
			SLIDER_WIDTH=70,							// Width of each control slider.
			SLIDER_HEIGHT=5,							// Height of each control slider.
			SLIDER_Y_SPACING=11,						// Vertical spacing between sliders.
			BUTTON_X=OPTION_X+63,	 				// Options button x pos
			BUTTON_Y=OPTION_Y+102, 					// Options button y pos
		};

	public:

		VisualControlsClass(void) {};
		void Process(void);
		int Init(void);

		int	Option_Width;							// Width of dialog box.
		int	Option_Height;							// Height of dialog box.
		int	Option_X;
		int	Option_Y;
		int	Text_X;						 			// Title's x pos
		int	Text_Y;  								// Add 11 for each following line
		int	Slider_X; 								// Slider's x pos
		int	Slider_Y;  								// Add 11 for each following line
		int	Slider_Width;							// Width of each control slider.
		int	Slider_Height;							// Height of each control slider.
		int	Slider_Y_Spacing;						// Vertical spacing between sliders.
		int	Button_X;	 							// Options button x pos
		int	Button_Y; 								// Options button y pos

};

#endif
