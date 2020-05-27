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

/* $Header:   F:\projects\c&c0\vcs\code\confdlg.h_v   4.69   27 Aug 1996 15:43:36   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CONFDLG.H                                                    *
 *                                                                                             *
 *                   Programmer : Maria del Mar McCready Legg											  *
 *											 Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : Jan 30, 1995                                                 *
 *                                                                                             *
 *                  Last Update : Jan 30, 1995   [MML]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*/

#ifndef CONFDLG_H
#define CONFDLG_H

#include "gadget.h"

class ConfirmationClass
{
	private:
		enum ConfirmationClassEnum {
			BUTTON_YES=1,								// Button number for "Options menu"
			BUTTON_NO									// Button number for "Options menu"
		};

	public:
		ConfirmationClass(void) {  };
		bool Process(char const * string);
		bool Process(int text);
};

#endif
