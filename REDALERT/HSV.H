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

/* $Header: /CounterStrike/HSV.H 1     3/03/97 10:24a Joe_bostic $ */
/*********************************************************************************************** 
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : HSV.H                                                        *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/02/95                                                     *
 *                                                                                             *
 *                  Last Update : December 2, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef HSV_H
#define HSV_H

class RGBClass;
class HSVClass;

/*
**	Each color entry is represented by this class. It holds the values for the color
**	attributes. The values are recorded in a range from 0 to 255 with 255 being the
**	maximum.
*/
class HSVClass
{
	private:
		static HSVClass const BlackColor;

	public:
		HSVClass(void) : Hue(0), Saturation(0), Value(0) {};
		HSVClass(unsigned char hue, unsigned char saturation, unsigned char value) :
				Hue(hue),
				Saturation(saturation),
				Value(value)
			{};
		operator RGBClass (void) const;

		enum {
			MAX_VALUE=255
		};

		void Adjust(int ratio, HSVClass const & hsv);
		int Difference(HSVClass const & hsv) const;
		int Hue_Component(void) const {return(Hue);};
		int Saturation_Component(void) const {return(Saturation);};
		int Value_Component(void) const {return(Value);};
		void Set(int color) const;

	private:
		unsigned char Hue;
		unsigned char Saturation;
		unsigned char Value;
};

#endif
