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

/* $Header: /CounterStrike/BULLET.H 2     3/06/97 1:46p Joe_bostic $ */
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
		CCPtr<BulletTypeClass> Class;

	private:
		/*
		**	Records who sent this "present" so that an appropriate "thank you" can
		**	be returned.
		*/
		TechnoClass * Payback;

		/*
		**	This is the facing that the projectile is travelling.
		*/
		FacingClass PrimaryFacing;

	public:

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		static void * operator new(size_t size);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void *ptr);
		BulletClass(BulletType id, TARGET target, TechnoClass * Payback, int strength, WarheadType warhead, int speed);
#ifdef FIXIT_MULTI_SAVE
		BulletClass(NoInitClass const & x) : ObjectClass(x), Class(x), FlyClass(x), FuseClass(x), PrimaryFacing(x) {};
#else
		BulletClass(NoInitClass const & x) : ObjectClass(x), Class(x), FlyClass(x), FuseClass(x) {};
#endif
		virtual ~BulletClass(void);
		operator BulletType(void) const {return Class->Type;};

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		static void Init(void);

		bool Is_Forced_To_Explode(COORDINATE & coord) const;
		void Bullet_Explodes(bool forced);
		int Shape_Number(void) const;
		virtual LayerType In_Which_Layer(void) const;
		virtual COORDINATE Sort_Y(void) const;
		virtual void Assign_Target(TARGET target) {TarCom = target;};
		virtual bool Unlimbo(COORDINATE , DirType facing = DIR_N);
		virtual ObjectTypeClass const & Class_Of(void) const {return *Class;};
		virtual void Detach(TARGET target, bool all);
		virtual void Draw_It(int x, int y, WindowNumberType window) const;
		virtual bool Mark(MarkType mark=MARK_CHANGE);
		virtual void AI(void);
		virtual short const * Occupy_List(bool = false) const;
		virtual short const * Overlap_List(void) const {return Occupy_List(false);};
		virtual COORDINATE Target_Coord(void) const;

		/*
		**	File I/O.
		*/
		bool Load(Straw & file);
		bool Save(Pipe & file) const;
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		/*
		**	If this bullet is forced to be inaccurate because of some outside means. A tank
		**	firing while moving is a good example.
		*/
		unsigned IsInaccurate:1;

	private:
		// Crude animation flag.
		unsigned IsToAnimate:1;

		/*
		** Is this missile allowed to come in from out of bounds?
		*/
		unsigned IsLocked:1;

		/*
		**	This is the target of the projectile. It is especially significant for those projectiles
		**	that home in on a target.
		*/
		TARGET TarCom;

		/*
		**	The speed of this projectile.
		*/
		int MaxSpeed;

		/*
		**	The warhead of this projectile.
		*/
		WarheadType Warhead;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];
};

#endif
