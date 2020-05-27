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

/* $Header: /CounterStrike/OPTIONS.H 1     3/03/97 10:25a Joe_bostic $ */
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
		void Set_Score_Volume(fixed volume, bool feedback);
		void Set_Sound_Volume(fixed volume, bool feedback);
		void Set_Brightness(fixed brightness);
		fixed Get_Brightness(void) const;
		void Set_Saturation(fixed color);
		fixed Get_Saturation(void) const;
		void Set_Contrast(fixed contrast);
		fixed Get_Contrast(void) const;
		void Set_Tint(fixed tint);
		fixed Get_Tint(void) const;
		int Normalize_Delay(int delay) const;
		int Normalize_Volume(int volume) const;

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

		int ScrollRate;		// Distance to scroll.
		fixed Volume;			// Volume for sound effects.
		fixed ScoreVolume;	// Volume for scores.
#ifdef FIXIT_VERSION_3
		fixed MultiScoreVolume;		//	Volume for scores during multiplayer games.
#endif
		fixed Brightness;		// Brightness.
		fixed Tint;				// Hue
		fixed Saturation;		// Saturation
		fixed Contrast;		// Value
		unsigned AutoScroll:1;		// Does map autoscroll?
		unsigned IsScoreRepeat:1;	// Score should repeat?
		unsigned IsScoreShuffle:1;	// Score list should shuffle?
		unsigned IsPaletteScroll:1;// Allow palette scrolling?

		/*
		**	These are the hotkeys used for keyboard control.
		*/
		KeyNumType KeyForceMove1;
		KeyNumType KeyForceMove2;
		KeyNumType KeyForceAttack1;
		KeyNumType KeyForceAttack2;
		KeyNumType KeySelect1;
		KeyNumType KeySelect2;
		KeyNumType KeyScatter;
		KeyNumType KeyStop;
		KeyNumType KeyGuard;
		KeyNumType KeyNext;
		KeyNumType KeyPrevious;
		KeyNumType KeyFormation;
		KeyNumType KeyHome1;
		KeyNumType KeyHome2;
		KeyNumType KeyBase;
		KeyNumType KeyResign;
		KeyNumType KeyAlliance;
		KeyNumType KeyBookmark1;
		KeyNumType KeyBookmark2;
		KeyNumType KeyBookmark3;
		KeyNumType KeyBookmark4;
		KeyNumType KeySelectView;
		KeyNumType KeyRepair;
		KeyNumType KeyRepairOn;
		KeyNumType KeyRepairOff;
		KeyNumType KeySell;
		KeyNumType KeySellOn;
		KeyNumType KeySellOff;
		KeyNumType KeyMap;
		KeyNumType KeySidebarUp;
		KeyNumType KeySidebarDown;
		KeyNumType KeyOption1;
		KeyNumType KeyOption2;
		KeyNumType KeyScrollLeft;
		KeyNumType KeyScrollRight;
		KeyNumType KeyScrollUp;
		KeyNumType KeyScrollDown;
		KeyNumType KeyQueueMove1;
		KeyNumType KeyQueueMove2;
		KeyNumType KeyTeam1;
		KeyNumType KeyTeam2;
		KeyNumType KeyTeam3;
		KeyNumType KeyTeam4;
		KeyNumType KeyTeam5;
		KeyNumType KeyTeam6;
		KeyNumType KeyTeam7;
		KeyNumType KeyTeam8;
		KeyNumType KeyTeam9;
		KeyNumType KeyTeam10;

		void Adjust_Palette(PaletteClass const & oldpal, PaletteClass & newpal, fixed brightness, fixed color, fixed tint, fixed contrast) const;
	protected:

	private:

		static char const * const HotkeyName;
};


#endif
