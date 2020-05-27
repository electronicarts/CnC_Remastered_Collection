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

/* $Header: /CounterStrike/FACING.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FACING.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 03/21/95                                                     *
 *                                                                                             *
 *                  Last Update : March 21, 1995 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   FacingClass::FacingClass -- Default constructor for the facing class.                     *
 *   FacingClass::Rotation_Adjust -- Perform a rotation adjustment to current facing.          *
 *   FacingClass::Set_Current -- Sets the current rotation value.                              *
 *   FacingClass::Set_Desired -- Sets the desired facing  value.                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"facing.h"


/***********************************************************************************************
 * FacingClass::FacingClass -- Default constructor for the facing class.                       *
 *                                                                                             *
 *    This default constructor merely sets the desired and current facing values to be the     *
 *    same (North).                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
FacingClass::FacingClass(void)
{
	CurrentFacing = DIR_N;
	DesiredFacing = DIR_N;
}


/***********************************************************************************************
 * FacingClass::Set_Desired -- Sets the desired facing  value.                                 *
 *                                                                                             *
 *    This routine is used to set the desired facing value without altering the current        *
 *    facing setting. Typical use of this routine is when a vehicle needs to face a           *
 *    direction, but currently isn't facing the correct direction. After this routine is       *
 *    called, it is presumed that subsequent calls to Rotation_Adjust() will result in the     *
 *    eventual alignment of the current facing.                                                *
 *                                                                                             *
 * INPUT:   facing   -- The new facing to assign to the desired value.                         *
 *                                                                                             *
 * OUTPUT:  bool; Did the desired facing value actually change by this routine call?           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int FacingClass::Set_Desired(DirType facing)
{
	if (DesiredFacing != facing) {
		DesiredFacing = facing;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * FacingClass::Set_Current -- Sets the current rotation value.                                *
 *                                                                                             *
 *    This routine will set the current rotation value. It is used to override the facing      *
 *    value without adjusting the desired setting.                                             *
 *                                                                                             *
 * INPUT:   facing   -- The new facing to assign to the current facing value.                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the current setting changed by this routine. Failure means that the      *
 *                current setting was already at the value specified.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int FacingClass::Set_Current(DirType facing)
{
	if (CurrentFacing != facing) {
		CurrentFacing = facing;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * FacingClass::Rotation_Adjust -- Perform a rotation adjustment to current facing.            *
 *                                                                                             *
 *    This routine is used when the current and desired facings differ but the current         *
 *    facing should be adjusted toward the desired facing. The amount of rotation to adjust    *
 *    is provided as a rotation rate parameter. Typical use of this routine is for turrets     *
 *    and other vehicle related rotating.                                                      *
 *                                                                                             *
 * INPUT:   rate  -- The rotation rate to use when adjusting the current facing toward the     *
 *                   desired facing. A value of 127 means instantaneous rotation.              *
 *                                                                                             *
 * OUTPUT:  bool; Did the rotation result in the current facing transitioning from one         *
 *                1/32 zone to another? If true, then the owning object most likely will       *
 *                need to be redrawn to reflect the change.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int FacingClass::Rotation_Adjust(int rate)
{
	/*
	**	Only perform the rotation adjustment if the desired facing is not the
	**	same as the current facing.
	*/
	if (Is_Rotating()) {
		rate = min(rate, 127);

		DirType oldfacing = CurrentFacing;
		int diff = Difference();

		/*
		**	If the allowed facing change is greater than the difference between
		**	the current facing and the desired facing, then just snap the
		**	facing to the new value.
		*/
		if (ABS(diff) < rate) {
			CurrentFacing = DesiredFacing;
		} else {

			/*
			**	Adjust the current facing clockwise or counterclockwise depending
			**	on the shortest distance to the desired facing from the current
			**	facing.
			*/
			if (diff < 0) {
				CurrentFacing = (DirType)(CurrentFacing - (DirType)rate);
			} else {
				CurrentFacing = (DirType)(CurrentFacing + (DirType)rate);
			}
		}

		/*
		**	If this facing adjustment caused the current facing to rotate into a
		**	new 1/32 rotation zone (likely to cause a redraw), then return
		**	this fact with a true value.
		*/
		return(Dir_To_32(CurrentFacing) != Dir_To_32(oldfacing));
	}
	return(false);
}
