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

/* $Header: /CounterStrike/FLASHER.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FLASHER.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 28, 1994                                                 *
 *                                                                                             *
 *                  Last Update : October 17, 1994   [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   FlasherClass::Debug_Dump -- Displays current status to the monochrome screen.             *
 *   FlasherClass::Process -- Performs the logic processing for the flashing ability.          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * FlasherClass::Debug_Dump -- Displays current status to the monochrome screen.               *
 *                                                                                             *
 *    This utility function will output the current status of the FlasherClass to the mono     *
 *    screen. It is through this display that bugs may be fixed or detected.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void FlasherClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(50, 7);
	mono->Printf("%2d", FlashCount);
}
#endif


/***********************************************************************************************
 * FlasherClass::Process -- Performs the logic processing for the flashing ability.            *
 *                                                                                             *
 *    The ability for an object to flash is controlled by this logic processing routine. It    *
 *    should be called once per game tick per unit.                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Should the associated object be redrawn?                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *   06/20/1994 JLB : Is now independent of object it represents.                              *
 *=============================================================================================*/
bool FlasherClass::Process(void)
{
	// 2019/09/20 JAS - Flashing info needs to exist per player
	for (int i = 0; i < HOUSE_COUNT; i++)
	{
		if (FlashCountPerPlayer[i])
		{
			FlashCountPerPlayer[i]--;
		}
	}

	if (FlashCount) {
		FlashCount--;
		IsBlushing = false;

		if (FlashCount & 0x01) {
			IsBlushing = true;
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * FlasherClass::Get_Flashing_Flags --																			  *
 *                                                                                             *
 *    Gets the flags tell which players this object should flash for.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  unsigned int; Flag representing the players to flash for                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   2019/09/20 JAS : Created.                                                                 *
 *=============================================================================================*/
unsigned int FlasherClass::Get_Flashing_Flags() const
{
	unsigned flags = 0;
	for (int i = 0; i < HOUSE_COUNT; ++i)
	{
		if (FlashCountPerPlayer[i] > 0)
		{
			flags |= (1 << i);
		}
	}

	return flags;
}