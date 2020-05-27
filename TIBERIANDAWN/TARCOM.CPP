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

/* $Header:   F:\projects\c&c\vcs\code\tarcom.cpv   2.17   16 Oct 1995 16:52:20   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TARCOM.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 16, 1994                                               *
 *                                                                                             *
 *                  Last Update : July 19, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TarComClass::Debug_Dump -- Displays the status of the tarcom class to the mono screen.    *
 *   TarComClass::AI -- Handles the logical AI for the tarcom class.                           *
 *   TarComClass::~TarComClass -- Destructor for turret object.                                * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/*********************************************************************************************** 
 * TarComClass::~TarComClass -- Destructor for turret object.                                  * 
 *                                                                                             * 
 *    This is the destructor for turret objects.                                               * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   07/19/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
TarComClass::~TarComClass(void) 
{
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * TarComClass::Debug_Dump -- Displays the status of the tarcom class to the mono screen.      *
 *                                                                                             *
 *    This routine is used to display the tarcom class status to the monochrome monitor.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TarComClass::Debug_Dump(MonoClass *mono) const
{
	TurretClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * TarComClass::AI -- Handles the logical AI for the tarcom class.                             *
 *                                                                                             *
 *    This handles the AI logic for the targeting computer.                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TarComClass::AI(void)
{
	TurretClass::AI();

	if (Class->Primary != WEAPON_NONE) {

		/*
		**	Determine which weapon can fire. First check for the primary weapon. If that weapon 
		**	cannot fire, then check any secondary weapon. If neither weapon can fire, then the
		**	failure code returned is that from the primary weapon.
		*/
		WeaponTypeClass const * weapon = &Weapons[Class->Primary];
		int primary = 0;
		FireErrorType ok = Can_Fire(TarCom, 0);
		if (ok != FIRE_OK) {
			if (Can_Fire(TarCom, 1) == FIRE_OK) {
				ok = FIRE_OK;
				primary = 1;
				weapon = &Weapons[Class->Secondary];
			}
		}

		switch (ok) {
			case FIRE_OK:
				if (What_Am_I() != RTTI_UNIT) {
					IsFiring = false;
				} else {
					if (!((UnitClass *)this)->Class->IsFireAnim) {
						IsFiring = false;
					}
				}

				if (TurretClass::Fire_At(TarCom, primary)) {
					Sound_Effect(weapon->Sound, Coord);
				}
				break;

			case FIRE_FACING:
				if (Class->IsLockTurret) {
					if (!Target_Legal(NavCom) && !IsDriving) {
						PrimaryFacing.Set_Desired(Direction(TarCom));
						SecondaryFacing.Set_Desired(PrimaryFacing.Desired());
					}
				} else {
					if (*this == UNIT_FTANK) {
						SecondaryFacing.Set_Desired(Facing_Dir(Dir_Facing(Direction(TarCom))));
					} else {
						SecondaryFacing.Set_Desired(Direction(TarCom));
					}
//					SecondaryFacing.Set_Desired(Direction256(Center_Coord(), As_Coord(TarCom)));
				}
				break;

			case FIRE_CLOAKED:
				IsFiring = false;
				Do_Uncloak();
				break;
		}
	}

	if (Target_Legal(TarCom) && !IsRotating) {
		DirType dir = Direction(TarCom);

		if (Class->IsTurretEquipped) {
			SecondaryFacing.Set_Desired(dir);
		} else {
			
			/*
			**	Non turret equipped vehicles will rotate their body to face the target only
			**	if the vehicle isn't currently moving or facing the correct direction. This
			**	applies only to tracked vehicles. Wheeled vehicles never rotate to face the
			**	target, since they aren't maneuverable enough.
			*/
			if ((Class->Speed == SPEED_TRACK || *this == UNIT_BIKE) && !Target_Legal(NavCom) && !IsDriving && PrimaryFacing.Difference(dir)) {
				if (*this == UNIT_FTANK) {
					PrimaryFacing.Set_Desired(Facing_Dir(Dir_Facing(dir)));
				} else {
					PrimaryFacing.Set_Desired(dir);
				}
			}
		}
	}
}


