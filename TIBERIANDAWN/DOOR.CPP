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

/* $Header:   F:\projects\c&c\vcs\code\door.cpv   1.4   16 Oct 1995 16:49:16   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : DOOR.CPP                                                     * 
 *                                                                                             * 
 *                   Programmer : Joe L. Bostic                                                * 
 *                                                                                             * 
 *                   Start Date : 06/11/95                                                     * 
 *                                                                                             * 
 *                  Last Update : June 14, 1995 [JLB]                                          * 
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 *   DoorClass::AI -- Handles the door processing logic.                                       * 
 *   DoorClass::Close_Door -- Try to close the unit's door.                                    * 
 *   DoorClass::DoorClass -- Constructor for the DoorClass object.                             * 
 *   DoorClass::Door_Stage -- Fetches the current door animation frame.                        * 
 *   DoorClass::Open_Door -- Opens the door for this unit.                                     * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"


/*********************************************************************************************** 
 * DoorClass::DoorClass -- Constructor for the DoorClass object.                               * 
 *                                                                                             * 
 *    This constructor sets the door to an initial closed state.                               * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/14/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
DoorClass::DoorClass(void) 
{
	State = IS_CLOSED;
	IsToRedraw = false;
	Stages = 0;
}


/*********************************************************************************************** 
 * DoorClass::AI -- Handles the door processing logic.                                         * 
 *                                                                                             * 
 *    This routine should be called every game frame. It handles the door closing and opening  * 
 *    logic.                                                                                   * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/13/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
void DoorClass::AI(void)
{
	if (Control.Graphic_Logic()) {
		if (Control.Fetch_Stage() >= Stages) {
			Control.Set_Rate(0);
			switch (State) {
				case IS_OPENING:
					State = IS_OPEN;
					break;

				case IS_CLOSING:
					State = IS_CLOSED;
					break;
			}
		}
		IsToRedraw = true;
	}
}


/*********************************************************************************************** 
 * DoorClass::Open_Door -- Opens the door for this unit.                                       * 
 *                                                                                             * 
 *    This routine will perform the door open operation for this unit. It will control vehicle * 
 *    rotation if necessary.                                                                   * 
 *                                                                                             * 
 * INPUT:   rate     -- The animation rate (delay) to use for the door animation logic.        * 
 *                                                                                             * 
 *          stages   -- The number of animations stages that this door must pass through.      * 
 *                                                                                             * 
 * OUTPUT:  Was action initiated to open the door?                                             * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   05/08/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
bool DoorClass::Open_Door(int rate, int stages)
{
	switch (State) {
		case IS_CLOSED:
		case IS_CLOSING:
			State = IS_OPENING;
			Stages = stages-1;
			Control.Set_Stage(0);
			Control.Set_Rate(rate);
			return(true);
	}
	return(false);
}


/*********************************************************************************************** 
 * DoorClass::Close_Door -- Try to close the unit's door.                                      * 
 *                                                                                             * 
 *    This routine will attempt to close the unit's door. If the door is already closed or     * 
 *    in the process of closing, then no action is performed.                                  * 
 *                                                                                             * 
 * INPUT:   rate     -- The animation rate (delay) to use for the door animation logic.        * 
 *                                                                                             * 
 *          stages   -- The number of animations stages that this door must pass through.      * 
 *                                                                                             * 
 * OUTPUT:  Action was initiated to close the door?                                            * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   05/08/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
bool DoorClass::Close_Door(int rate, int stages)
{
	switch (State) {
		case IS_OPEN:
		case IS_OPENING:
			State = IS_CLOSING;
			Stages = stages-1;
			Control.Set_Stage(0);
			Control.Set_Rate(rate);
			return(true);
	}
	return(false);
}


/*********************************************************************************************** 
 * DoorClass::Door_Stage -- Fetches the current door animation frame.                          * 
 *                                                                                             * 
 *    Use this routine to fetch the current door animation frame number. Frame zero is the     * 
 *    closed frame and frame 'N' is the open frame. If the door is in the process of opening   * 
 *    or closing, the appropriate frame number is used. 'N' is defined as the number of        * 
 *    stages in the animation minus 1 (e.g., a four frame animation will return a door stage   * 
 *    number between 0 and 3, inclusive).                                                      * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns with the door animation frame number.                                      * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/14/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
int DoorClass::Door_Stage(void) const 
{
	switch (State) {
		case IS_CLOSING:
			return((Stages-1) - Control.Fetch_Stage());

		case IS_CLOSED:
			return(0);

		case IS_OPENING:
			return(Control.Fetch_Stage());

		case IS_OPEN:
			return(Stages-1);
	}
	return(0);
}
