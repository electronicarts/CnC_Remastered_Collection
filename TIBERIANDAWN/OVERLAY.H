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

/* $Header:   F:\projects\c&c\vcs\code\overlay.h_v   2.16   16 Oct 1995 16:44:50   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OVERLAY.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 17, 1994                                                 *
 *                                                                                             *
 *                  Last Update : May 17, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef OVERLAY_H
#define OVERLAY_H

#include	"object.h"
#include	"type.h"

/******************************************************************************
**	This class controls the overlay object. Overlay objects function congruously
**	to carpet on a floor. They have no depth, but merely control the icon to be rendered
**	as the cell's bottom most layer.
*/
class OverlayClass : public ObjectClass
{
	public:
		/*-------------------------------------------------------------------
		**	Constructors and destructors.
		*/
		static void * operator new(size_t size);
		static void operator delete(void *ptr);
		OverlayClass(void);
		OverlayClass(OverlayType type, CELL pos=-1, HousesType = HOUSE_NONE);
		virtual ~OverlayClass(void) {if (GameActive) OverlayClass::Limbo();};
		operator OverlayType(void) const {return Class->Type;};
		virtual RTTIType What_Am_I(void) const {return RTTI_OVERLAY;};

		static void Init(void);

		/*
		**	File I/O.
		*/
		static void Read_INI(char *);
		static void Write_INI(char *);
		static char *INI_Name(void) {return "OVERLAY";};
		bool Load(FileClass & file);
		bool Save(FileClass & file);
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		/*
		**	Virtual support functionality.
		*/
		virtual bool Mark(MarkType);
		virtual ObjectTypeClass const & Class_Of(void) const {return *Class;};
		virtual void Draw_It(int , int , WindowNumberType ) {};

		/*
		**	Dee-buggin' support.
		*/
		int Validate(void) const;

	private:
		/*
		**	This is used to control the marking process of the overlay. If this is
		**	set to a valid house number, then the cell that the overlay is marked down
		**	upon will be flagged as being owned by the specified house.
		*/
		static HousesType ToOwn;

		/*
		**	This is a pointer to the overlay object's class.
		*/
		OverlayTypeClass const * const Class;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[8];

		/*
		** This contains the value of the Virtual Function Table Pointer
		*/
		static void * VTable;
};

#endif