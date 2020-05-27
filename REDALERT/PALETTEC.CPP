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

/* $Header: /CounterStrike/palette.cpp 2     9/23/97 11:00p Steve_t $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : PALETTE.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/02/95                                                     *
 *                                                                                             *
 *                  Last Update : February 5, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   PaletteClass::Adjust -- Adjusts the palette toward another palette.                       *
 *   PaletteClass::Adjust -- Adjusts this palette toward black.                                *
 *   PaletteClass::Closest_Color -- Finds closest match to color specified.                    *
 *   PaletteClass::Set -- Fade the display palette to this palette.                            *
 *   PaletteClass::PaletteClass -- Constructor that fills palette with color specified.        *
 *   PaletteClass::operator = -- Assignment operator for palette objects.                      *
 *   PaletteClass::operator == -- Equality operator for palette objects.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef NOINITCLASS
#define NOINITCLASS
struct NoInitClass {
	public:
		void operator () (void) const {};
};
#endif

void __cdecl Set_Palette(void * palette);


#ifndef BITMAPCLASS
#define BITMAPCLASS
class BitmapClass
{
	public:
		BitmapClass(int w, int h, unsigned char * data) :
			Width(w), Height(h), Data(data) {};

		int Width;
		int Height;
		unsigned char * Data;
};

class TPoint2D
{
	public:
		TPoint2D(int xx, int yy) : x(xx), y(yy) {};
		TPoint2D(void) : x(0), y(0) {};

		int x;
		int y;
};
#endif

#include	"function.h"
#include	"watcom.h"
#include	"palette.h"
#include	"palettec.h"
#include	"ftimer.h"
//#define TIMER_H
#include	"wwlib32.h"
//#include	"timer.h"
#include	<string.h>

#ifndef SYSTEM_TIMER_CLASS
#define SYSTEM_TIMER_CLASS

#ifdef WIN32
extern WinTimerClass	*		WindowsTimer;
#endif

class SystemTimerClass
{
	public:
		#ifdef WIN32
			long operator () (void) const {if (!WindowsTimer) return(0);return(WindowsTimer->Get_System_Tick_Count());};
			operator long (void) const {if (!WindowsTimer) return(0);return(WindowsTimer->Get_System_Tick_Count());};
		#else
			long operator () (void) const {return(Get_System_Tick_Count());};
			operator long (void) const {return(Get_System_Tick_Count());};
		#endif
};
#endif


//PaletteClass const PaletteClass::CurrentPalette;
extern "C" unsigned char CurrentPalette[];

PaletteClass const & PaletteClass::CurrentPalette = *(PaletteClass *)&::CurrentPalette[0];


/***********************************************************************************************
 * PaletteClass::PaletteClass -- Constructor that fills palette with color specified.          *
 *                                                                                             *
 *    This constructor will fill the palette with the color specified.                         *
 *                                                                                             *
 * INPUT:   rgb   -- Reference to the color to fill the entire palette with.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
PaletteClass::PaletteClass(RGBClass const & rgb)
{
	for (int index = 0; index < COLOR_COUNT; index++) {
		Palette[index] = rgb;
	}
}


/***********************************************************************************************
 * PaletteClass::operator == -- Equality operator for palette objects.                         *
 *                                                                                             *
 *    This is the comparison for equality operator. It will compare palette objects to         *
 *    determine if they are identical.                                                         *
 *                                                                                             *
 * INPUT:   palette  -- Reference to the palette to compare to this palette.                   *
 *                                                                                             *
 * OUTPUT:  Are the two palettes identical?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int PaletteClass::operator == (PaletteClass const & palette) const
{
	if (this == &palette) return(true);
	return(memcmp(&Palette[0], &palette.Palette[0], sizeof(Palette)) == 0);
}


/***********************************************************************************************
 * PaletteClass::operator = -- Assignment operator for palette objects.                        *
 *                                                                                             *
 *    This is the assignment operator for palette objects. Although the default C++ generated  *
 *    assignment operator would function correctly, it would not check for self-assignment    *
 *    and thus this routine can be faster.                                                     *
 *                                                                                             *
 * INPUT:   palette  -- Reference to that palette that will be copied into this palette.       *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the newly copied to palette.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
PaletteClass & PaletteClass::operator = (PaletteClass const & palette)
{
	if (this == &palette) return(*this);

	memcpy(&Palette[0], &palette.Palette[0], sizeof(Palette));
	return(*this);
}


/***********************************************************************************************
 * PaletteClass::Adjust -- Adjusts this palette toward black.                                  *
 *                                                                                             *
 *    This routine is used to adjust this palette toward black. Typical use of this routine    *
 *    is when fading the palette to black.                                                     *
 *                                                                                             *
 * INPUT:   ratio -- The ratio to fade this palette to black. 0 means no fading at all. 255    *
 *                   means 100% faded to black.                                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine doesn't actually set the palette to the video card. Use the Set()  *
 *             function to achieve that purpose.                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void PaletteClass::Adjust(int ratio)
{
	for (int index = 0; index < COLOR_COUNT; index++) {
		Palette[index].Adjust(ratio, BlackColor);
	}
}


/***********************************************************************************************
 * PaletteClass::Adjust -- Adjusts the palette toward another palette.                         *
 *                                                                                             *
 *    This routine is used to adjust a palette toward a destination palette by the ratio       *
 *    specified. This is primarily used by the palette fading routines.                        *
 *                                                                                             *
 * INPUT:   palette  -- Reference to the destination palette.                                  *
 *                                                                                             *
 *          ratio    -- The ratio to adjust this palette toward the destination palette. A     *
 *                      value of 0 means no adjustment at all. A value of 255 means 100%       *
 *                      adjustment.                                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void PaletteClass::Adjust(int ratio, PaletteClass const & palette)
{
	for (int index = 0; index < COLOR_COUNT; index++) {

//if (index == 1) {
//	Mono_Printf("From R=%d,G=%d,B=%d ", Palette[index].Red_Component(), Palette[index].Green_Component(), Palette[index].Blue_Component());
//	Mono_Printf("To R=%d,G=%d,B=%d [%d] ", palette[index].Red_Component(), palette[index].Green_Component(), palette[index].Blue_Component(), ratio);
//}
		Palette[index].Adjust(ratio, palette[index]);

//if (index == 1) {
//	Mono_Printf("Equals R=%d,G=%d,B=%d.\n", Palette[index].Red_Component(), Palette[index].Green_Component(), Palette[index].Blue_Component());
//}

	}
}


/***********************************************************************************************
 * PaletteClass::Partial_Adjust -- Adjusts the specified parts of this palette toward black.   *
 *                                                                                             *
 *    This routine is used to adjust this palette toward black. Typical use of this routine    *
 *    is when fading the palette to black. The input lookup table is used to determine         *
 *    which entries should fade and which should stay the same                                 *
 *                                                                                             *
 * INPUT:   ratio -- The ratio to fade this palette to black. 0 means no fading at all. 255    *
 *                   means 100% faded to black.                                                *
 *                                                                                             *
 *          lookup	-- ptr to lookup table                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine doesn't actually set the palette to the video card. Use the Set()  *
 *             function to achieve that purpose.                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void PaletteClass::Partial_Adjust(int ratio, char *lut)
{
	for (int index = 0; index < COLOR_COUNT; index++) {
		if (lut[index]) {
			Palette[index].Adjust(ratio, BlackColor);
		}
	}
}


/***********************************************************************************************
 * PaletteClass::Partial_Adjust -- Adjusts the palette toward another palette.                 *
 *                                                                                             *
 *    This routine is used to adjust a palette toward a destination palette by the ratio       *
 *    specified. This is primarily used by the palette fading routines.  The input lookup      *
 *    table is used to determine which entries should fade and which should stay the same      *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   palette  -- Reference to the destination palette.                                  *
 *                                                                                             *
 *          ratio    -- The ratio to adjust this palette toward the destination palette. A     *
 *                      value of 0 means no adjustment at all. A value of 255 means 100%       *
 *                      adjustment.                                                            *
 *                                                                                             *
 *          lookup   -- ptr to lookup table                                                    *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void PaletteClass::Partial_Adjust(int ratio, PaletteClass const & palette, char *lut)
{
	for (int index = 0; index < COLOR_COUNT; index++) {
		if (lut[index]) {
			Palette[index].Adjust(ratio, palette[index]);
		}
	}
}


/***********************************************************************************************
 * PaletteClass::Closest_Color -- Finds closest match to color specified.                      *
 *                                                                                             *
 *    This routine will examine the palette and return with the color index number for the     *
 *    color that most closely matches the color specified. Remap operations rely heavily on    *
 *    this routine to allow working with a constant palette.                                   *
 *                                                                                             *
 * INPUT:   rgb   -- Reference to a color to search for in the current palette.                *
 *                                                                                             *
 * OUTPUT:  Returns with a color index value to most closely matches the specified color.      *
 *                                                                                             *
 * WARNINGS:   This routine will quite likely not find an exact match.                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int PaletteClass::Closest_Color(RGBClass const & rgb) const
{
	int closest = 0;
	int value = -1;

	RGBClass const * ptr = &Palette[0];
	for (int index = 0; index < COLOR_COUNT; index++) {
		int difference = rgb.Difference(*ptr++);
		if (value == -1 || difference < value) {
			value = difference;
			closest = index;
		}
	}
	return(closest);
}


#ifndef	WIN32
extern void Vsync(void);
#pragma aux Vsync modify [edx ebx eax] = \
	"mov	edx,03DAh"				\
	"mov	ebx,[VertBlank]"		\
	"and	bl,001h"					\
	"shl	bl,3"						\
	"in_vbi:"						\
	"in	al,dx"					\
	"and	al,008h"					\
	"xor	al,bl"					\
	"je	in_vbi"					\
	"out_vbi:"						\
	"in	al,dx"					\
	"and	al,008h"					\
	"xor	al,bl"					\
	"jne	out_vbi"
#endif	//WIN32


/***********************************************************************************************
 * PaletteClass::Set -- Fade the display palette to this palette.                              *
 *                                                                                             *
 *    This routine will fade the display palette to match this palette over the time period    *
 *    specified. For smooth palette transitions, this is the routine to call.                  *
 *                                                                                             *
 * INPUT:   time  -- The time period (in system tick increments) to fade the display palette   *
 *                   to match this palette.                                                    *
 *                                                                                             *
 *          callback -- Optional pointer to callback function that, if non-null, will be       *
 *                      called as often as possible during the fading process.                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine will not return until the palette is completely faded to the       *
 *             destination palette.                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *   02/05/1996 JLB : Uses new timer system.                                                   *
 *=============================================================================================*/
void PaletteClass::Set(int time, void (* callback)(void)) const
{
	CDTimerClass<SystemTimerClass> timer = time;
	PaletteClass original = CurrentPalette;

	while (timer) {

		/*
		**	Build an intermediate palette that is as close to the destination palette
		**	as the current time is proportional to the ending time.
		*/
		PaletteClass palette = original;
		int adjust = ((time - timer) * 256) / time;
		palette.Adjust(adjust, *this);

		/*
		**	Remember the current time so that multiple palette sets within the same game
		**	time tick won't occur. This is probably unnecessary since the palette setting
		**	code, at the time of this writing, delays at least one game tick in the process
		**	of setting the palette.
		*/
		long holdtime = timer;

		/*
		**	Set the palette to this intermediate palette and then loop back
		**	to calculate and set a new intermediate palette.
		*/
#ifdef WIN32
		Set_Palette((void*)&palette[0]);
#else
		palette.Set();
#endif	//WIN32

		/*
		**	If the callback routine was specified, then call it once per palette
		**	setting loop.
		*/
		if (callback) {
			callback();
		}

		/*
		**	This loop ensures that the palette won't be set more than once per game tick. Setting
		**	the palette more than once per game tick will have no effect since the calculation will
		**	result in the same intermediate palette that was previously calculated.
		*/
		while (timer == holdtime && holdtime != 0) {
			if (callback) callback();
		}
	}

	/*
	**	Ensure that the final palette exactly matches the requested
	**	palette before exiting the fading routine.
	*/
#ifndef WIN32
	Vsync();
	RGBClass const * rgbptr = &Palette[0];
	RGBClass::Raw_Color_Prep(0);
	for (int index = 0; index < COLOR_COUNT; index++) {
		rgbptr->Raw_Set();
		rgbptr++;
	}
	((PaletteClass &)CurrentPalette) = *this;
#else	//WIN32
	Set_Palette((void*)&Palette[0]);
#endif
}
