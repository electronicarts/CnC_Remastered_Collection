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

/* $Header: /CounterStrike/FLASHER.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FLASHER.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 28, 1994                                                 *
 *                                                                                             *
 *                  Last Update : May 28, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef FLASHER_H
#define FLASHER_H

class FlasherClass {
	public:
		/*
		**	When this object is targeted, it will flash a number of times. This is the
		**	flash control number. It counts down to zero and then stops. Odd values
		**	cause the object to be rendered in a lighter color.
		*/
		unsigned FlashCount:7;

		/*
		**	When an object is targeted, it flashes several times to give visual feedback
		**	to the player. Every other game "frame", this flag is true until the flashing
		**	is determined to be completed.
		*/
		unsigned IsBlushing:1;

		// 2019/09/20 JAS - Flashing info needs to exist per player
		unsigned int Get_Flashing_Flags() const;
		unsigned int FlashCountPerPlayer[HOUSE_COUNT];

		// 2019/09/20 JAS - Flashing info needs to exist per player
		FlasherClass(void) {
			FlashCount = 0;
			IsBlushing = false;

			for (int i = 0; i < HOUSE_COUNT; ++i)
			{
				FlashCountPerPlayer[i] = 0;
			}
		}

		FlasherClass(NoInitClass const & ) {};
		~FlasherClass(void) {};

		#ifdef CHEAT_KEYS
		void Debug_Dump(MonoClass *mono) const;
		#endif
		bool Process(void);
};

#endif
