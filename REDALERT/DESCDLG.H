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

/* $Header: /CounterStrike/DESCDLG.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DESCDLG.H                                                    *
 *                                                                                             *
 *                   Programmer : Maria del Mar McCready Legg											  *
 *											 Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : Jan 26, 1995                                                 *
 *                                                                                             *
 *                  Last Update : Jan 26, 1995   [MML]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*/

#ifndef DESCDLG_H
#define DESCDLG_H

#include "gadget.h"

class DescriptionClass 
{
	private:

		enum DescriptionClassEnum {
			OPTION_WIDTH=216,							// Width of dialog box.
			OPTION_HEIGHT=122,						// Height of dialog box.
			OPTION_X=(((320 - OPTION_WIDTH) / 2) & ~7),
			OPTION_Y=((200 - OPTION_HEIGHT) / 2),
			TEXT_X=OPTION_X+32,			 			// Title's x pos
			TEXT_Y=OPTION_Y+32,  					// Add 11 for each following line
			BUTTON_OPTIONS=1,							// Button number for "Ok"
			BUTTON_CANCEL,							
			BUTTON_EDIT,
			BUTTON_X=OPTION_X+63,	 				// Options button x pos
			BUTTON_Y=OPTION_Y+102, 					// Options button y pos
			EDIT_Y  =OPTION_Y+50, 					
			EDIT_W  =180   //204,
		};

	public:
		DescriptionClass(void) {};
		void Process(char *string);
};

#endif


