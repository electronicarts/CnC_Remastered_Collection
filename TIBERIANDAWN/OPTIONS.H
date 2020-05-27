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

/* $Header:   F:\projects\c&c\vcs\code\options.h_v   2.18   16 Oct 1995 16:46:20   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OPTIONS.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 8, 1994                                                 *
 *                                                                                             *
 *                  Last Update : June 8, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef OPTIONS_H
#define OPTIONS_H

class OptionsClass {
	public:
		enum {
			MAX_SCROLL_SETTING=7,
			MAX_SPEED_SETTING=7
		};

		OptionsClass(void);

		void One_Time(void);
		void Process(void);

		void Fixup_Palette(void) const;
		void Set_Shuffle(int on);
		void Set_Repeat(int on);
		void Set_Score_Volume(int volume);
		void Set_Sound_Volume(int volume, int feedback);
		void Set_Brightness(int brightness);
		int Get_Brightness(void) const;
		void Set_Color(int color);
		int Get_Color(void) const;
		void Set_Contrast(int contrast);
		int Get_Contrast(void) const;
		void Set_Tint(int tint);
		int Get_Tint(void) const;
		int Normalize_Delay(int delay) const;
		int Normalize_Sound(int volume) const;

		/*
		** File I/O routines
		*/
		void Load_Settings(void);
		void Save_Settings(void);

		void Set(void);

		/*
		**	This is actually the delay between game frames expressed as 1/60 of
		**	a second. The default value is 4 (1/15 second).
		*/
		unsigned int GameSpeed;

		int ScrollRate;	// Distance to scroll.
		unsigned char Brightness;
		unsigned char Volume;		// Volume for sound effects.
		unsigned char ScoreVolume;	// Volume for scores.
		unsigned char Contrast;		// Value
		unsigned char Color;			// Saturation
		unsigned char Tint;			// Hue
		unsigned AutoScroll:1;		// Does map autoscroll?
		unsigned IsScoreRepeat:1;	// Score should repeat?
		unsigned IsScoreShuffle:1;	// Score list should shuffle?
		unsigned IsDeathAnnounce:1;// Announce enemy deaths?
		unsigned IsFreeScroll:1;	// Allow free direction scrolling?

	protected:

		void Adjust_Palette(void *oldpal, void *newpal, unsigned char brightness, unsigned char color, unsigned char tint, unsigned char contrast) const;

	private:
};


#endif
