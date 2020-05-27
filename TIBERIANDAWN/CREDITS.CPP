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

/* $Header:   F:\projects\c&c\vcs\code\credits.cpv   2.17   16 Oct 1995 16:51:28   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *   CreditClass::Graphic_Logic -- Handles the credit redraw logic.                            *
 *   CreditClass::CreditClass -- Default constructor for the credit class object.              *
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
CreditClass::CreditClass(void)
{
	IsToRedraw = false;
	IsUp = false;
	IsAudible = false;
	Credits = 0;
	Current = 0;
	Countdown = 0;
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
#define	XX (320-120)
#define	WW	50
void CreditClass::Graphic_Logic(bool forced)
{
	int factor = Get_Resolution_Factor();
	int xx = SeenBuff.Get_Width() - (120 << factor);
	if (forced || IsToRedraw) {

		/*
		**	Play a sound effect when the money display changes, but only if a sound
		**	effect was requested.
		*/
		if (IsAudible) {
			if (IsUp) {
				Sound_Effect(VOC_UP, VOL_1);
			} else  {
				Sound_Effect(VOC_DOWN, VOL_1);
			}
		}

		/*
		**	Display the new current value.
		*/
		//LogicPage->Fill_Rect(xx-(20 << factor), 1 << factor, xx+(20 << factor), 6 << factor, LTGREY);
		TabClass::Draw_Credits_Tab();
		Fancy_Text_Print("%ld", xx, 0, 11, TBLACK, TPF_GREEN12_GRAD|TPF_CENTER | TPF_USE_GRAD_PAL, Current);

		IsToRedraw = false;
		IsAudible = false;
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
	if (player_ptr == NULL) {
		return;
	}

	Credits = player_ptr->Available_Money();

	/*
	**	Make sure that the credit counter doesn't drop below zero.
	*/
	Credits = MAX(Credits, 0L);

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
		long adder = Credits - Current;
		adder = ABS(adder);
		adder >>= 5;
		adder = Bound(adder, 1L, 71+72);
		if (Current > Credits) adder = -adder;
		Current += adder;
		Countdown = 1;

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