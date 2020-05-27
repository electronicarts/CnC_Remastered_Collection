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

/* $Header: /CounterStrike/TEMPLATE.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TEMPLATE.H                                                   *
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

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include	"object.h"
#include	"type.h"


/******************************************************************************
**	This class controls the template object. Template objects function congruously
**	to carpet on a floor. They have no depth, but merely control the icon to be rendered
**	as the cell's bottom most layer.
*/
class TemplateClass : public ObjectClass
{
	public:
		/*
		**	This is a pointer to the template object's class.
		*/
		CCPtr<TemplateTypeClass> Class;

		/*-------------------------------------------------------------------
		**	Constructors and destructors.
		*/
		static void * operator new(size_t size);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void *ptr);
		TemplateClass(TemplateType type, CELL pos=-1);
		TemplateClass(NoInitClass const & x) : ObjectClass(x), Class(x) {};
		virtual ~TemplateClass(void) {if (GameActive) TemplateClass::Limbo();Class=0;};
		operator TemplateType(void) const {return Class->Type;};

		static void Init(void);

		/*
		**	Query functions.
		*/
		virtual ObjectTypeClass const & Class_Of(void) const {return *Class;};
		int Icon_Number(CELL cell);

		/*
		**	Display and rendering support functionality. Supports imagery and how
		**	object interacts with the map and thus indirectly controls rendering.
		*/
		virtual void Draw_It(int , int , WindowNumberType ) const {};
		virtual bool Mark(MarkType mark);

		/*
		**	File I/O.
		*/
		bool Load(Straw & file);
		bool Save(Pipe & file) const;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[8];
};

#endif