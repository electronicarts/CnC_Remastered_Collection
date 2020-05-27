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

/* $Header: /CounterStrike/CARRY.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CARRY.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 02/26/96                                                     *
 *                                                                                             *
 *                  Last Update : February 26, 1996 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef CARRY_H
#define CARRY_H

class CarryoverClass : public LinkClass {
	public:
		CarryoverClass(TechnoClass * techno = 0);
		CarryoverClass(NoInitClass const & x) : LinkClass(x) {}

		bool Create(void) const;

	//protected:
		/*
		**	What type of object this is.
		*/
		RTTIType RTTI;

		/*
		**	This is the object type that is to be carried over. The exact nature of
		**	this type depends on the RTTI value. Only certain object types are
		**	recorded.
		*/
		union {
			StructType Building;
			UnitType Unit;
			InfantryType Infantry;
			VesselType Vessel;
		} Type;

		/*
		**	The location of the object.
		*/
		CELL Cell;

		/*
		**	The strength of the object at the time is was recorded.
		*/
		int Strength;

		/*
		**	This is the owner of the object.
		*/
		HousesType House;
};


#endif
