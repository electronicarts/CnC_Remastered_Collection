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

/* $Header: /CounterStrike/FLY.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FLY.H                                                        *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 24, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 24, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef FLY_H
#define FLY_H

typedef enum ImpactType : unsigned char {
	IMPACT_NONE,		// No movement (of significance) occurred.
	IMPACT_NORMAL,		// Some (non eventful) movement occurred.
	IMPACT_EDGE			// The edge of the world was reached.
} ImpactType;


/****************************************************************************
**	Flying objects are handled by this class definition.
*/
class FlyClass {
	public:
		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		FlyClass(void) : SpeedAccum(0), SpeedAdd(MPH_IMMOBILE) {};
		FlyClass(NoInitClass const &) {};
		~FlyClass(void) {};

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		void Fly_Speed(int speed, MPHType maximum);
		ImpactType Physics(COORDINATE &coord, DirType facing);
		MPHType Get_Speed(void) const {return(SpeedAdd);};

	private:
		/*
		**	Object movement consists of incrementing the accumulator until enough "distance"
		**	has accumulated so that moving the object becomes reasonable.
		*/
		unsigned SpeedAccum;			// Lepton accumulator.
		MPHType SpeedAdd;			// Lepton add (per frame).
};

#endif
