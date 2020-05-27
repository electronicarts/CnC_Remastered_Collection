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

/* $Header: /CounterStrike/TRACKER.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TRACKER.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/14/96                                                     *
 *                                                                                             *
 *                  Last Update : June 14, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Detach_This_From_All -- Detaches this object from all others.                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"


/***********************************************************************************************
 * Detach_This_From_All -- Detaches this object from all others.                               *
 *                                                                                             *
 *    This routine sweeps through all game objects and makes sure that it is no longer         *
 *    referenced by them. Typically, this is called in preparation for the object's death      *
 *    or limbo state.                                                                          *
 *                                                                                             *
 * INPUT:   target   -- This object expressed as a target number.                              *
 *                                                                                             *
 *          all      -- Is this object really in truly being removed from the game? The        *
 *                      answer would be false if the target was actually a stealth             *
 *                      tank that is cloaking. In such a case, the object should be removed    *
 *                      from all non-friendly tracking systems, but otherwise left alone.      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Detach_This_From_All(TARGET target, bool all)
{
	int index;
	if (Target_Legal(target)) {

		for (index = 0; index < Houses.Count(); index++) {
			Houses.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Teams.Count(); index++) {
			Teams.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < TeamTypes.Count(); index++) {
			TeamTypes.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Units.Count(); index++) {
			Units.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Vessels.Count(); index++) {
			Vessels.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Aircraft.Count(); index++) {
			Aircraft.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Buildings.Count(); index++) {
			Buildings.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Bullets.Count(); index++) {
			Bullets.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Infantry.Count(); index++) {
			Infantry.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Anims.Count(); index++) {
			Anims.Ptr(index)->Detach(target, all);
		}

		Map.Detach(target, all);

		Logic.Detach(target, all);

		ChronalVortex.Detach(target);

		/*
		**	Removing a trigger type must also remove all triggers that are dependant
		**	upon that type.
		*/
		if (As_TriggerType(target) != NULL) {
			for (int index = 0; index < Triggers.Count(); index++) {
				TriggerClass * tp = Triggers.Ptr(index);

				if (tp->Class->As_Target() == target) {
					Detach_This_From_All(tp->As_Target());
					delete tp;
					index--;
				}
			}
		}

		for (index = 0; index < Triggers.Count(); index++) {
			Triggers.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < TriggerTypes.Count(); index++) {
			TriggerTypes.Ptr(index)->Detach(target, all);
		}
	}
}





