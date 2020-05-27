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

/* $Header: /CounterStrike/WEAPON.H 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : WEAPON.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/17/96                                                     *
 *                                                                                             *
 *                  Last Update : May 17, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef WEAPON_H
#define WEAPON_H


/**********************************************************************
**	This is the constant data associated with a weapon. Some objects
**	can have multiple weapons and this class is used to isolate and
**	specify this data in a convenient and selfcontained way.
*/
class WeaponTypeClass
{
	public:
		WeaponTypeClass(char const * name);
		WeaponTypeClass(NoInitClass const &) {}
		~WeaponTypeClass(void);

		void * operator new(size_t);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		void operator delete(void * pointer);

		char const * Name(void) const {return(IniName);}
		bool Read_INI(CCINIClass & ini);
		static WeaponTypeClass * As_Pointer(WeaponType weapon);
		void Code_Pointers(void) {}
		void Decode_Pointers(void) {}
		ThreatType Allowed_Threats(void) const;
		bool Is_Wall_Destroyer(void) const;

		/*
		**	This is both the weapon type number and the index number into
		**	the weapon array.
		*/
		int ID;

		/*
		**	This is the identifying name of this weapon.
		*/
		char const * IniName;

		/*
		**	Increase the weapon speed if the target is flying.
		*/
		unsigned IsTurboBoosted:1;

		/*
		**	If potential targets of this weapon should be scanned for
		**	nearby friendly structures and if found, firing upon the target
		**	would be discouraged, then this flag will be true.
		*/
		unsigned IsSupressed:1;

		/*
		**	If this weapon is equipped with a camera that reveals the
		**	area around the firer, then this flag will be true.
		*/
		unsigned IsCamera:1;

		/*
		**	If this weapon requires charging before it can fire, then this
		**	flag is true. In actuality, this only applies to the Tesla coil
		**	which has specific charging animation. The normal rate of fire
		**	value suffices for all other cases.
		*/
		unsigned IsElectric:1;

		/*
		**	This is the number of shots this weapon first (in rapid succession).
		**	The normal value is 1, but for the case of two shooter weapons such as
		**	the double barreled gun turrets of the Mammoth tank, this value will be
		**	set to 2.
		*/
		int Burst;

		/*
		**	This is the unit class of the projectile fired. A subset of the unit types
		**	represent projectiles. It is one of these classes that is specified here.
		**	If this object does not fire anything, then this value will be BULLET_NONE.
		*/
		BulletTypeClass const * Bullet;

		/*
		**	This is the damage (explosive load) to be assigned to the projectile that
		**	this object fires. For the rare healing weapon, this value is negative.
		*/
		int Attack;

		/*
		**	Speed of the projectile launched.
		*/
		MPHType MaxSpeed;

		/*
		**	Warhead to attach to the projectile.
		*/
		WarheadTypeClass const * WarheadPtr;

		/*
		**	Objects that fire (which can be buildings as well) will fire at a
		**	frequency controlled by this value. This value serves as a count
		**	down timer between shots. The smaller the value, the faster the
		**	rate of fire.
		*/
		int ROF;

		/*
		**	When this object fires, the range at which it's projectiles travel is
		**	controlled by this value. The value represents the number of cells the
		**	projectile will travel. Objects outside of this range will not be fired
		**	upon (in normal circumstances).
		*/
		LEPTON Range;

		/*
		**	This is the typical sound generated when firing.
		*/
		VocType Sound;

		/*
		**	This is the animation to display at the firing coordinate.
		*/
		AnimType Anim;
};


#endif


