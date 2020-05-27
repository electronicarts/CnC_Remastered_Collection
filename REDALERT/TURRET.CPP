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

/* $Header:   F:\projects\c&c0\vcs\code\turret.cpv   3.1   13 Mar 1996 09:49:34   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TURRET.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 25, 1994                                               *
 *                                                                                             *
 *                  Last Update : August 13, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TurretClass::AI -- Handles the reloading of the turret weapon.                            *
 *   TurretClass::Can_Fire -- Determines if turret can fire upon target.                       *
 *   TurretClass::Debug_Dump -- Debug printing of turret values.                               *
 *   TurretClass::Fire_At -- Try to fire upon the target specified.                            *
 *   TurretClass::Fire_Coord -- Determines the coorindate that projectile would appear.        *
 *   TurretClass::Fire_Direction -- Determines the directinon of firing.                       *
 *   TurretClass::Ok_To_Move -- Queries whether the vehicle can move.                          *
 *   TurretClass::TurretClass -- Normal constructor for the turret class.                      *
 *   TurretClass::TurretClass -- The default constructor for turret class objects.             *
 *   TurretClass::Unlimbo -- Unlimboes turret object.                                          *
 *   TurretClass::~TurretClass -- Default destructor for turret class objects.                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"turret.h"


/***********************************************************************************************
 * TurretClass::~TurretClass -- Default destructor for turret class objects.                   *
 *                                                                                             *
 *    This is the default destructor for turret class objects. It does nothing.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TurretClass::~TurretClass(void)
{
}


/***********************************************************************************************
 * TurretClass::TurretClass -- The default constructor for turret class objects.               *
 *                                                                                             *
 *    This is the default constructor for turret class objects. It does nothing.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TurretClass::TurretClass(void)
{
}


/***********************************************************************************************
 * TurretClass::TurretClass -- Normal constructor for the turret class.                        *
 *                                                                                             *
 *    This is the normal constructor for the turret class. It merely sets the turret up to     *
 *    face north.                                                                              *
 *                                                                                             *
 * INPUT:   classid  -- The type id for this particular unit.                                  *
 *                                                                                             *
 *          house    -- The house that this unit will belong to.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TurretClass::TurretClass(UnitType classid, HousesType house) :
	DriveClass(classid, house)
{
}



