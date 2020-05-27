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

/* $Header: /CounterStrike/WEAPON.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : WEAPON.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/20/96                                                     *
 *                                                                                             *
 *                  Last Update : September 9, 1996 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Armor_From_Name -- Convert ASCII name into armor type number.                             *
 *   WeaponTypeClass::As_Pointer -- Give a weapon type ID, fetch pointer to weapon type object.*
 *   WeaponTypeClass::Read_INI -- Fetch the weapon data from the INI database.                 *
 *   WeaponTypeClass::WeaponTypeClass -- Default constructor for weapon type objects.          *
 *   WeaponTypeClass::operator delete -- Returns weapon type object back to special heap.      *
 *   WeaponTypeClass::operator new -- Allocates a weapon type object form the special heap.    *
 *   WeaponTypeClass::~WeaponTypeClass -- Destructor for weapon type class objects.            *
 *   Weapon_From_Name -- Conver ASCII name to weapon type ID number.                           *
 *   WeaponTypeClass::Allowed_Threats -- Determine what threats this weapon can address.       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***************************************************************************
**	These are the various weapons and their characteristics.
*/
TFixedIHeapClass<WeaponTypeClass> Weapons;


/***********************************************************************************************
 * WeaponTypeClass::WeaponTypeClass -- Default constructor for weapon type objects.            *
 *                                                                                             *
 *    This default constructor will initialize all the values of the weapon to the default     *
 *    state. Thus, if any of these settings are not specifically overridden by the rules.ini   *
 *    file, they will remain this value in the game.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
WeaponTypeClass::WeaponTypeClass(char const * name) :
	ID(Weapons.ID(this)),
	IniName(name),
	IsSupressed(false),
	IsCamera(false),
	IsElectric(false),
	Burst(1),
	Bullet(NULL),
	Attack(0),
	MaxSpeed(MPH_IMMOBILE),
	WarheadPtr(NULL),
	ROF(0),
	Range(0),
	Sound(VOC_NONE),
	Anim(ANIM_NONE)
{
}


/***********************************************************************************************
 * WeaponTypeClass::~WeaponTypeClass -- Destructor for weapon type class objects.              *
 *                                                                                             *
 *    This destructor really doesn't do anything but set the pointers to NULL. This is a       *
 *    general purposes safety tactic but is otherwise useless.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
WeaponTypeClass::~WeaponTypeClass(void)
{
	IniName = NULL;
	Bullet = NULL;
	WarheadPtr = NULL;
}


/***********************************************************************************************
 * WeaponTypeClass::operator new -- Allocates a weapon type object form the special heap.      *
 *                                                                                             *
 *    This will allocate a weapon type object from a special heap that has been set up for     *
 *    that purpose.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the weapon type object allocated. If there was           *
 *          insufficient memory available, NULL is returned.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * WeaponTypeClass::operator new(size_t)
{
	return(Weapons.Alloc());
}


/***********************************************************************************************
 * WeaponTypeClass::operator delete -- Returns weapon type object back to special heap.        *
 *                                                                                             *
 *    This routine will return the weapon type object back to the heap so that the memory      *
 *    can be reused for subsiquent allocations of weapon type objects.                         *
 *                                                                                             *
 * INPUT:   pointer  -- Pointer to the weapon type object to return to the special heap.       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void WeaponTypeClass::operator delete(void * pointer)
{
	Weapons.Free((WeaponTypeClass *)pointer);
}


/***********************************************************************************************
 * WeaponTypeClass::As_Pointer -- Give a weapon type ID, fetch pointer to weapon type object.  *
 *                                                                                             *
 *    This routine will conver the weapon type ID specified into a pointer to the weapon type  *
 *    object it represents.                                                                    *
 *                                                                                             *
 * INPUT:   weapon   -- The weapon type object ID to convert to a pointer.                     *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the weapon type class object that is represented by the  *
 *          weapon ID number specified. If no match could be found, then NULL is returned.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
WeaponTypeClass * WeaponTypeClass::As_Pointer(WeaponType weapon)
{
	if (weapon != WEAPON_NONE) {
		return(Weapons.Ptr(weapon));
//		for (int index = 0; index < Weapons.Count(); index++) {
//			WeaponTypeClass * ptr = Weapons.Ptr(index);
//			if (ptr->ID == weapon) {
//				return(ptr);
//			}
//		}
	}
	return(NULL);
}


/***********************************************************************************************
 * WeaponTypeClass::Read_INI -- Fetch the weapon data from the INI database.                   *
 *                                                                                             *
 *    This routine will fetch the weapon data for this weapon type object from the INI         *
 *    database specified.                                                                      *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that the weapon data will be fetched        *
 *                   from.                                                                     *
 *                                                                                             *
 * OUTPUT:  bool; Was this weapon type described in the database and the values retrieved?     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool WeaponTypeClass::Read_INI(CCINIClass & ini)
{
	if (ini.Is_Present(Name())) {
		IsSupressed = ini.Get_Bool(Name(), "Supress", IsSupressed);
		Burst = ini.Get_Int(Name(), "Burst", Burst);
		Attack = ini.Get_Int(Name(), "Damage", Attack);
		MaxSpeed = ini.Get_MPHType(Name(), "Speed", MaxSpeed);
		ROF = ini.Get_Int(Name(), "ROF", ROF);
		Range = ini.Get_Lepton(Name(), "Range", Range);
		Sound = ini.Get_VocType(Name(), "Report", Sound);
		Anim = ini.Get_AnimType(Name(), "Anim", Anim);
		IsCamera = ini.Get_Bool(Name(), "Camera", IsCamera);
		IsElectric = ini.Get_Bool(Name(), "Charges", IsElectric);
		IsTurboBoosted = ini.Get_Bool(Name(), "TurboBoost", IsTurboBoosted);

		WarheadType wtype = (WarheadPtr != NULL) ? WarheadType(WarheadPtr->ID) : WARHEAD_NONE;
		wtype = ini.Get_WarheadType(Name(), "Warhead", wtype);
		if (wtype != WARHEAD_NONE) {
			WarheadPtr = WarheadTypeClass::As_Pointer(wtype);
//			WarheadPtr = &Warheads[wtype];
		} else {
			WarheadPtr = NULL;
		}

		BulletType btype = (Bullet != NULL) ? BulletType(Bullet->ID) : BULLET_NONE;
		btype = ini.Get_BulletType(Name(), "Projectile", btype);
		if (btype != BULLET_NONE) {
			Bullet = &BulletTypeClass::As_Reference(btype);
		} else {
			Bullet = NULL;
		}

		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * Weapon_From_Name -- Conver ASCII name to weapon type ID number.                             *
 *                                                                                             *
 *    This will find the weapon whos name matches that specified and then it will return the   *
 *    weapon ID number associated with it.                                                     *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name of the weapon type.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the weapon type ID number that matches the name specified. If no      *
 *          match could be found, then WEAPON_NONE is returned.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
WeaponType Weapon_From_Name(char const * name)
{
	if (!name) return(WEAPON_NONE);

	for (int index = 0; index < Weapons.Count(); index++) {
		if (stricmp(Weapons.Ptr(index)->Name(), name) == 0) {
			return(WeaponType(Weapons.Ptr(index)->ID));
		}
	}

	return(WEAPON_NONE);
}


/***********************************************************************************************
 * Armor_From_Name -- Convert ASCII name into armor type number.                               *
 *                                                                                             *
 *    This will find the armor type that matches the ASCII name specified and then will return *
 *    the armor ID number of it.                                                               *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name of the armor to find.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the armor ID number of the armor that matches the name specified. If  *
 *          no match could be found, then ARMOR_NONE is returned.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ArmorType Armor_From_Name(char const * name)
{
	if (!name) return(ARMOR_NONE);

	for (ArmorType index = ARMOR_FIRST; index < ARMOR_COUNT; index++) {
		if (stricmp(ArmorName[index], name) == 0) {
			return(index);
		}
	}

	return(ARMOR_NONE);
}


/***********************************************************************************************
 * WeaponTypeClass::Allowed_Threats -- Determine what threats this weapon can address.         *
 *                                                                                             *
 *    This routine will examine the capabilities of this weapon and return with the threat     *
 *    types that it can address.                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the threat types that this weapon can address.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ThreatType WeaponTypeClass::Allowed_Threats(void) const
{
	ThreatType threat = THREAT_NORMAL;
	if (Bullet->IsAntiAircraft) {
		threat = threat | THREAT_AIR;
	}
	if (Bullet->IsAntiGround) {
		threat = threat | THREAT_INFANTRY|THREAT_VEHICLES|THREAT_BOATS|THREAT_BUILDINGS;
	}
	return(threat);
}


bool WeaponTypeClass::Is_Wall_Destroyer(void) const
{
	if (WarheadPtr != NULL && WarheadPtr->IsWallDestroyer) {
		return(true);
	}
	return(false);
}
