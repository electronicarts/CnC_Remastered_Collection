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

/* $Header: /CounterStrike/CREDITS.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CREDITS.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 17, 1994                                               *
 *                                                                                             *
 *                  Last Update : March 13, 1995 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CreditClass::AI -- Handles updating the credit display.                                   *
 *   CreditClass::CreditClass -- Default constructor for the credit class object.              *
 *   CreditClass::Graphic_Logic -- Handles the credit redraw logic.                            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***********************************************************************************************
 * CreditClass::CreditClass -- Default constructor for the credit class object.                *
 *                                                                                             *
 *    This is the constructor for the credit class object. It merely sets the credit display   *
 *    state to null.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
CreditClass::CreditClass(void) :
	Credits(0),
	Current(0),
	IsToRedraw(false),
	IsUp(false),
	IsAudible(false),
	Countdown(0)
{
}


/***********************************************************************************************
 * CreditClass::Graphic_Logic -- Handles the credit redraw logic.                              *
 *                                                                                             *
 *    This routine should be called whenever the main game screen is to be updated. It will    *
 *    check to see if the credit display should be redrawn. If so, it will redraw it.          *
 *                                                                                             *
 * INPUT:   forced   -- Should the credit display be redrawn regardless of whether the redraw  *
 *                      flag is set? This is typically the case when the screen needs to be    *
 *                      redrawn from scratch.                                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
//#define	XX (320-120)
//#define	WW	50
void CreditClass::Graphic_Logic(bool forced)
{
	if (forced || IsToRedraw) {
		BStart(BENCH_TABS);

		int xx = SeenBuff.Get_Width() - (120 * RESFACTOR);

		/*
		** Adjust the credits display to be above the sidebar for 640x400
		*/
#ifdef WIN32
		xx += 80 * RESFACTOR;
#endif

		/*
		**	Play a sound effect when the money display changes, but only if a sound
		**	effect was requested.
		*/
		if (IsAudible) {
			if (IsUp) {
				Sound_Effect(VOC_MONEY_UP, fixed(1, 2));
			} else  {
				Sound_Effect(VOC_MONEY_DOWN, fixed(1, 2));
			}
		}

		/*
		**	Display the new current value.
		*/
		// PG TabClass::Draw_Credits_Tab();
#ifdef WIN32
		//PG Fancy_Text_Print("%ld", xx, 0, &MetalScheme, TBLACK, TPF_METAL12 | TPF_CENTER | TPF_USE_GRAD_PAL, Current);
#else
		Fancy_Text_Print("%ld", xx, 0, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_NOSHADOW|TPF_6PT_GRAD|TPF_CENTER|TPF_BRIGHT_COLOR, Current);
#endif	//WIN32

		if (Scen.MissionTimer.Is_Active()) {
			long secs = Scen.MissionTimer / TICKS_PER_SECOND;
			long mins = secs / 60;
			long hours = mins / 60;
			secs %= 60;
			mins %= 60;
#if (0) //Moved to LOGIC.CPP
			/*
			**	Speak mission timer reminders.
			*/
			VoxType vox = VOX_NONE;
			if (Scen.MissionTimer == (1 * TICKS_PER_MINUTE)) vox = VOX_TIME_1;
			if (Scen.MissionTimer == (2 * TICKS_PER_MINUTE)) vox = VOX_TIME_2;
			if (Scen.MissionTimer == (3 * TICKS_PER_MINUTE)) vox = VOX_TIME_3;
			if (Scen.MissionTimer == (4 * TICKS_PER_MINUTE)) vox = VOX_TIME_4;
			if (Scen.MissionTimer == (5 * TICKS_PER_MINUTE)) vox = VOX_TIME_5;
			if (Scen.MissionTimer == (10 * TICKS_PER_MINUTE)) vox = VOX_TIME_10;
			if (Scen.MissionTimer == (20 * TICKS_PER_MINUTE)) vox = VOX_TIME_20;
			if (Scen.MissionTimer == (30 * TICKS_PER_MINUTE)) vox = VOX_TIME_30;
			if (Scen.MissionTimer == (40 * TICKS_PER_MINUTE)) vox = VOX_TIME_40;
			if (vox != VOX_NONE) {
				Speak(vox);
				Map.FlasherTimer = 7;
			}
#endif
#ifdef WIN32
#if (0) //PG
			if (hours) {
				Fancy_Text_Print(TXT_TIME_FORMAT_HOURS, 200 * RESFACTOR, 0, &MetalScheme, TBLACK, TPF_METAL12|TPF_CENTER|TPF_USE_GRAD_PAL, hours, mins, secs);
			} else {
				Fancy_Text_Print(TXT_TIME_FORMAT_NO_HOURS, 200 * RESFACTOR, 0, &MetalScheme, TBLACK, TPF_METAL12|TPF_CENTER|TPF_USE_GRAD_PAL, mins, secs);
			}
#endif
#else
			if (hours) {
				Fancy_Text_Print("%02d:%02d:%02d", 120 * RESFACTOR, 0, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_NOSHADOW|TPF_6PT_GRAD|TPF_CENTER|TPF_BRIGHT_COLOR, hours, mins, secs);
			} else {
				Fancy_Text_Print("%02d:%02d", 120 * RESFACTOR, 0, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_NOSHADOW|TPF_6PT_GRAD|TPF_CENTER|TPF_BRIGHT_COLOR, mins, secs);
			}
#endif	//WIN32
		}

		IsToRedraw = false;
		IsAudible = false;
		BEnd(BENCH_TABS);
	}
}


/***********************************************************************************************
 * CreditClass::AI -- Handles updating the credit display.                                     *
 *                                                                                             *
 *    This routine handles the logic that controls the rate of credit change in the credit     *
 *    display. It doesn't actually redraw the credit display, but will flag it to be redrawn   *
 *    if it detects that a change is to occur.                                                 *
 *                                                                                             *
 * INPUT:   forced   -- Should the credit display immediately reflect the current credit       *
 *                      total for the player? This is usually desired when initially loading   *
 *                      a scenario or saved game.                                              *
 *          player_ptr -- Player to calculate visible credits for                              *
 *          logic_only -- If true, don't flag map for redraw                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/13/1995 JLB : Created.                                                                 *
 *   10/16/2019  ST : Added house and logic parameters so we can call this from HouseClass::AI *
 *=============================================================================================*/
void CreditClass::AI(bool forced, HouseClass *player_ptr, bool logic_only)
{
	static int _last = 0;

	if (!forced && !logic_only && Frame == _last) return;
	if (!logic_only) {
		_last = Frame;
	}

	if (player_ptr == NULL) {
		return;
	}

	Credits = player_ptr->Available_Money();

	/*
	**	Make sure that the credit counter doesn't drop below zero.
	*/
	Credits = max(Credits, 0L);

	if (Scen.MissionTimer.Is_Active() || Scen.MissionTimer) {
		IsToRedraw = true;
		Map.Flag_To_Redraw(false);
	}

	if (Current == Credits) return;

	if (forced) {
		IsAudible = false;
		Current = Credits;
	} else {

		if (Countdown) Countdown--;
		if (Countdown) return;

		/*
		**	Determine the amount to change the display toward the
		**	desired value.
		*/
		int adder = Credits - Current;

		if (adder > 0) {
			Countdown = 1;
		} else {
			Countdown = 3;
		}

		adder = ABS(adder);
		adder >>= 3;
//		adder >>= 4;
//		adder >>= 5;
		adder = Bound(adder, 1, 71+72);
		if (Current > Credits) adder = -adder;
		Current += adder;
		if (Current-adder != Current) {
			IsAudible = true;
			IsUp = (adder > 0);
		}
	}
	IsToRedraw = true;

	if (!logic_only) {
		Map.Flag_To_Redraw(false);
	}
}