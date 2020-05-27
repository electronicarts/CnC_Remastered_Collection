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

/* $Header:   F:\projects\c&c\vcs\code\logic.h_v   2.17   16 Oct 1995 16:45:52   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LOGIC.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 29, 1994                                                 *
 *                                                                                             *
 *                  Last Update : May 29, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef LOGIC_H
#define LOGIC_H

#include	"layer.h"

/***********************************************************************************************
**	Game logic processing is controlled by this class. The graphic and AI logic is handled
**	separately so that on slower machines, the graphic display is least affected.
*/
class LogicClass : public LayerClass 
{
	public:
		void AI(void);
		#ifdef CHEAT_KEYS
		void Debug_Dump(MonoClass *mono) const;
		#endif

		/*
		** Added. ST - 8/19/2019 5:46PM
		*/
		void Clear_Recently_Created_Bits(void);
};
#endif