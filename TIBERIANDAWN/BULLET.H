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

/* $Header:   F:\projects\c&c\vcs\code\bullet.h_v   2.18   16 Oct 1995 16:47:40   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BULLET.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 23, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 23, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef BULLET_H
#define BULLET_H

#include "object.h"
#include	"fly.h"
#include	"fuse.h"


class BulletClass :	public ObjectClass,
							public FlyClass,
							public FuseClass
{
	public:

		/*
		**	This specifies exactly what kind of bullet this is. All of the static attributes
		**	for this bullet is located in the BulletTypeClass pointed to by this variable.
		*/
		BulletTypeClass const * const Class;
		operator BulletType(void) const {return Class->Type;};

		/*
		**	Records who sent this "present" so that an appropriate "thank you" can
		**	be returned.
		*/
		TechnoClass * Payback;

		/*
		**	This is the facing that the projectile is travelling.
		*/
		FacingClass PrimaryFacing;

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		static void * BulletClass::operator new(size_t size);
		static void BulletClass::operator delete(void *ptr);
		BulletClass(void);
		BulletClass(BulletType id);
		virtual ~BulletClass(void) {if (GameActive) BulletClass::Limbo();};
		virtual RTTIType What_Am_I(void) const {return RTTI_BULLET;};

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		static void Init(void);

		virtual void Assign_Target(TARGET target) {TarCom = target;};
		virtual bool Unlimbo(COORDINATE , DirType facing = DIR_N);
		virtual LayerType In_Which_Layer(void) const {return LAYER_TOP;};
		virtual ObjectTypeClass const & Class_Of(void) const {return *Class;};
		virtual void Detach(TARGET target, bool all);
		virtual void Draw_It(int x, int y, WindowNumberType window);
		virtual bool Mark(MarkType mark=MARK_CHANGE);
		virtual void AI(void);
		virtual short const * Occupy_List(void) const;
		virtual short const * Overlap_List(void) const {return Occupy_List();};
		virtual TARGET As_Target(void) const;

		/*
		**	File I/O.
		*/
		bool Load(FileClass & file);
		bool Save(FileClass & file);
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		/*
		**	Dee-buggin' support.
		*/
		int Validate(void) const;

		/*
		**	If this bullet is forced to be inaccurate because of some outside means. A tank
		**	firing while moving is a good example.
		*/
		unsigned IsInaccurate:1;

	private:

		// Crude animation flag.
		unsigned IsToAnimate:1;

		/*
		**	This is the height of the projectile. It starts at a low height, rises to an
		**	apogee and then drops to explode upon impact. The height is used to render
		**	the bullet's vertical offset.
		*/
		int Altitude;

		/*
		**	This is a modifier for the altitude that rises and falls in order to simulate
		**	the arc of the projectile. This value is added to the height every game tick
		**	while simultaneously being reduced itself. The net effect, is a rising
		**	projectile that slows and then eventually drops.
		*/
		signed char Riser;

		/*
		**	This is the target of the projectile. It is especially significant for those projectiles
		**	that home in on a target.
		*/
		TARGET TarCom;

		/*
		** Is this missle allowed to come in from out of bounds?
		*/
		unsigned IsLocked:1;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];

		/*
		** This contains the value of the Virtual Function Table Pointer
		*/
		static void  * VTable;
};

#endif