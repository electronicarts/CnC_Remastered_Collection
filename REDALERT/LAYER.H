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

/* $Header: /CounterStrike/LAYER.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LAYER.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 31, 1994                                                 *
 *                                                                                             *
 *                  Last Update : May 31, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef LAYER_H
#define LAYER_H

#include "vector.h"

class ObjectClass;

class LayerClass : public DynamicVectorClass<ObjectClass *>
{
	public:

		//-----------------------------------------------------------------
		void Sort(void);
		bool Submit(ObjectClass const * object, bool sort=false);
		int Sorted_Add(ObjectClass const * const object);


		virtual void Init(void) {Clear();};
		virtual void One_Time(void) {};

		/*
		**	File I/O.
		*/
		bool Load(Straw & file);
		bool Save(Pipe & file) const;
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);
};

#endif
