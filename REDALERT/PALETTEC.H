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

/* $Header: /CounterStrike/PALETTE.H 2     9/23/97 11:00p Steve_t $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : PALETTE.H                                                    *
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


#ifndef PALETTE_Hx
#define PALETTE_Hx

#include	"rgb.h"

/*
**	The palette class is used to manipulate a palette as a whole. All 256 colors are
**	represented by the palette class object.
*/
class PaletteClass
{
	public:
		enum {
			COLOR_COUNT=256			// Number of color indices on the palette.
		};

		PaletteClass(void) {};
		PaletteClass(RGBClass const & rgb);

		RGBClass & operator[] (unsigned index) {return(Palette[index % COLOR_COUNT]);};
		const RGBClass & operator[] (unsigned index) const {return(Palette[index % COLOR_COUNT]);};
		int operator == (PaletteClass const & palette) const;
		int operator != (PaletteClass const & palette) const {return(!(operator ==(palette)));};
		PaletteClass & operator = (PaletteClass const & palette);
		
		// Removed these and replaced with function for clarity. ST - 5/9/2019
		//operator const unsigned char * (void) const {return((const unsigned char *)&Palette[0]);};
		//operator unsigned char * (void) {return((unsigned char *)&Palette[0]);};
		const void *Get_Data(void) const { return &Palette[0]; }
		void *Get_Data(void) { return &Palette[0]; }

		void Adjust(int ratio);
		void Adjust(int ratio, PaletteClass const & palette);
		void Partial_Adjust(int ratio, char *lut);
		void Partial_Adjust(int ratio, PaletteClass const & palette, char *lut);
		void Set(int time = 0, void (*callback)(void) = 0) const;
		int Closest_Color(RGBClass const & rgb) const;

		static PaletteClass const & CurrentPalette;

	protected:
		RGBClass Palette[COLOR_COUNT];
};


#endif
