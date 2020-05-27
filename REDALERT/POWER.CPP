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

/* $Header: /CounterStrike/POWER.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : POWER.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/15/94                                                     *
 *                                                                                             *
 *                  Last Update : October 14, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   PowerClass::AI -- Process the power bar logic.                                            *
 *   PowerClass::Draw_It -- Renders the power bar graphic.                                     *
 *   PowerClass::Init_Clear -- Clears all the power bar variables.                             *
 *   PowerClass::One_Time -- One time processing for the power bar.                            *
 *   PowerClass::PowerButtonClass::Action -- Handles the mouse over the power bar area.        *
 *   PowerClass::PowerClass -- Default constructor for the power bar class.                    *
 *   PowerClass::Refresh_Cells -- Intercepts the redraw logic to see if sidebar to redraw too. *
 *   PowerClass::Power_Height -- Given a value figure where it falls on bar                    *
 *   PowerClass::Flash_Power -- Flag the power bar to flash.                                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/*
**	Points to the shape to use for the "desired" power level indicator.
*/
void const * PowerClass::PowerShape;
void const * PowerClass::PowerBarShape;

PowerClass::PowerButtonClass PowerClass::PowerButton;


/***********************************************************************************************
 * PowerClass::PowerClass -- Default constructor for the power bar class.                      *
 *                                                                                             *
 *    This is the default constructor for the power bar class. It doesn't really do anything.  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/20/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
PowerClass::PowerClass(void) :
	IsToRedraw(false),
	IsActive(false),
	FlashTimer(0),
	RecordedDrain(-1),
	RecordedPower(-1),
	DesiredDrainHeight(0),
	DesiredPowerHeight(0),
	DrainHeight(0),
	PowerHeight(0),
	DrainBounce(0),
	PowerBounce(0),
	PowerDir(0),
	DrainDir(0)
{
}


/***********************************************************************************************
 * PowerClass::Init_Clear -- Clears all the power bar variables.                               *
 *                                                                                             *
 *    This routine is called in preparation for the start of a scenario. The power bar is      *
 *    initialized into the null state by this routine. As soon as the scenario starts, the     *
 *    power bar will rise to reflect the actual power output and drain.                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/07/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void PowerClass::Init_Clear(void)
{
	RadarClass::Init_Clear();
	RecordedDrain = -1;
	RecordedPower = -1;
	DesiredDrainHeight = 0;
	DesiredPowerHeight = 0;
	DrainHeight = 0;
	PowerHeight = 0;
	PowerBounce = 0;
	DrainBounce = 0;
	DrainDir = 0;
	PowerDir = 0;
	FlashTimer = 0;
}


/***********************************************************************************************
 * PowerClass::One_Time -- One time processing for the power bar.                              *
 *                                                                                             *
 * This routine is for code that truly only needs to be done once per game run.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void PowerClass::One_Time(void)
{
	RadarClass::One_Time();
	PowerButton.X = POWER_X * RESFACTOR;
	PowerButton.Y = POWER_Y * RESFACTOR;
	PowerButton.Width = (POWER_WIDTH * RESFACTOR)-1;
	PowerButton.Height = POWER_HEIGHT * RESFACTOR;
	PowerShape = MFCD::Retrieve("POWER.SHP");
	PowerBarShape = MFCD::Retrieve("POWERBAR.SHP");
}


/***********************************************************************************************
 * PowerClass::Draw_It -- Renders the power bar graphic.                                       *
 *                                                                                             *
 *    This routine will draw the power bar graphic to the LogicPage.                           *
 *                                                                                             *
 * INPUT:   complete -- Should the power bar be redrawn even if it isn't specifically flagged  *
 *                      to do so?                                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/20/1994 JLB : Created.                                                                 *
 *   12/27/1994 JLB : Changes power bar color depending on amount of power.                    *
 *=============================================================================================*/
void PowerClass::Draw_It(bool complete)
{
	static int _modtable[]={
		0, -1, 0, 1, 0, -1, -2, -1, 0, 1, 2, 1 ,0
	};

	if (complete || IsToRedraw) {
		BStart(BENCH_POWER);

		if (LogicPage->Lock()) {
			if (Map.IsSidebarActive) {
				IsToRedraw = false;
				ShapeFlags_Type flags = SHAPE_NORMAL;
				void const * remap = NULL;

				if (FlashTimer > 1 && ((FlashTimer % 3) & 0x01) != 0) {
					flags = flags | SHAPE_FADING;
					remap = Map.FadingRed;
				}

//				LogicPage->Fill_Rect(POWER_X, POWER_Y, POWER_X+POWER_WIDTH-1, POWER_Y+POWER_HEIGHT-1, LTGREY);
				CC_Draw_Shape(PowerBarShape, 0, 240 * RESFACTOR, 88 * RESFACTOR, WINDOW_MAIN, flags | SHAPE_NORMAL | SHAPE_WIN_REL, remap);

#ifdef WIN32
				/*
				** Hires power strip is too big to fit into a shape so it is in two parts
				*/
				CC_Draw_Shape(PowerBarShape, 1, 240 * RESFACTOR, (88 * RESFACTOR) + (56*RESFACTOR), WINDOW_MAIN, flags | SHAPE_NORMAL | SHAPE_WIN_REL, remap);
#endif
				/*
				**	Determine how much the power production exceeds or falls short
				**	of power demands.
				*/
				int bottom       = (POWER_Y + POWER_HEIGHT - 1) * RESFACTOR;
				int power_height  = (PowerHeight == DesiredPowerHeight) ? PowerHeight + (_modtable[PowerBounce] * PowerDir) : PowerHeight;
				int drain_height  = (DrainHeight == DesiredDrainHeight) ? DrainHeight + (_modtable[DrainBounce] * DrainDir) : DrainHeight;
				power_height = Bound(power_height, 0, POWER_HEIGHT - 2);
				drain_height = Bound(drain_height, 0, POWER_HEIGHT - 2);

				/*
				**	Draw the power output graphic on top of the power bar framework.
				*/
				if (power_height) {
					int color1 = 3;
					int color2 = 4;

					if (PlayerPtr->Drain > PlayerPtr->Power) {
						color1 = 214;
						color2 = 211;
					}
					if (PlayerPtr->Drain > (PlayerPtr->Power * 2)) {
						color1 = 235;
						color2 = 230;
					}

					/*
					** New power bar is in slightly different place
					**
					** Old power bar was 107 pixels high. New bar is 153 pixels high.
					**
					** ST - 5/2/96 11:23AM
					*/
#ifdef WIN32
					power_height = (power_height*(76*RESFACTOR+1)) / (53*RESFACTOR+1);
					drain_height = (drain_height*(76*RESFACTOR+1)) / (53*RESFACTOR+1);
#endif
					bottom = (175*RESFACTOR)+1;

					LogicPage->Fill_Rect(245*RESFACTOR, bottom-power_height, 245*RESFACTOR+1, bottom, color2);
					LogicPage->Fill_Rect(246*RESFACTOR, bottom-power_height, 246*RESFACTOR+1, bottom, color1);
				}

				/*
				**	Draw the power drain threshold marker.
				*/
				CC_Draw_Shape(PowerShape, 0, (POWER_X * RESFACTOR)+RESFACTOR, bottom - (drain_height + (2 * RESFACTOR)), WINDOW_MAIN, flags | SHAPE_NORMAL, remap);
			}
			LogicPage->Unlock();
		}
		BEnd(BENCH_POWER);
	}
	RadarClass::Draw_It(complete);
}


/***********************************************************************************************
 * PowerClass::AI -- Process the power bar logic.                                              *
 *                                                                                             *
 *    Use this routine to process the power bar logic. This consists of animation effects.     *
 *                                                                                             *
 * INPUT:   input -- The player input value to be consumed or ignored as appropriate.          *
 *                                                                                             *
 *          x,y   -- Mouse coordinate parameters to use.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/20/1994 JLB : Created.                                                                 *
 *   12/31/1994 JLB : Uses mouse coordinate parameters.                                        *
 *=============================================================================================*/
void PowerClass::AI(KeyNumType &input, int x, int y)
{
	if (Map.IsSidebarActive /*IsActive*/) {
		int olddrain = DrainHeight;
		int oldpower  = PowerHeight;


		/*
		** If the recorded power value has changed we need to adjust for
		** it.
		*/
		if (PlayerPtr->Power != RecordedPower) {
			DesiredPowerHeight = Power_Height(PlayerPtr->Power);
			RecordedPower		 = PlayerPtr->Power;
			PowerBounce			 = 12;
			if (PowerHeight > DesiredPowerHeight) {
				PowerDir = -1;
			} else if (PowerHeight < DesiredPowerHeight) {
				PowerDir = 1;
			} else {
				PowerBounce = 0;
			}
		}

		/*
		** If the recorded drain value has changed we need to adjust for
		** it.
		*/
		if (PlayerPtr->Drain != RecordedDrain) {
			DesiredDrainHeight = Power_Height(PlayerPtr->Drain);
			RecordedDrain		 = PlayerPtr->Drain;
			DrainBounce			 = 12;
			if (DrainHeight > DesiredDrainHeight) {
				DrainDir = -1;
			} else if (DrainHeight < DesiredDrainHeight) {
				DrainDir = 1;
			} else {
				DrainBounce = 0;
			}
		}

		if (DrainBounce && DrainHeight == DesiredDrainHeight) {
			IsToRedraw = true;
			Flag_To_Redraw(false);
			DrainBounce--;
		} else {
			/*
			** If we need to move the drain height then do so.
			*/
			if (DrainHeight != DesiredDrainHeight) {
				DrainHeight += DrainDir;
			}
		}

		if (PowerBounce && PowerHeight == DesiredPowerHeight) {
			IsToRedraw = true;
			Flag_To_Redraw(false);
			PowerBounce--;
		} else {
			/*
			** If we need to move the power height then do so.
			*/
			if (PowerHeight != DesiredPowerHeight) {
				PowerHeight += PowerDir;
			}
		}

		if (olddrain != DrainHeight || oldpower != PowerHeight) {
			IsToRedraw = true;
			Flag_To_Redraw(false);
		}

		/*
		**	Flag to redraw if the power bar flash effect has expired.
		*/
//		if (FlashTimer == 1) {
		if (FlashTimer > 0) {
			IsToRedraw = true;
			Flag_To_Redraw(false);
		}
	}
	RadarClass::AI(input, x, y);
}


/***********************************************************************************************
 * PowerClass::Refresh_Cells -- Intercepts the redraw logic to see if sidebar to redraw too.   *
 *                                                                                             *
 *    This routine will examine a refresh list request and determine if the sidebar would be   *
 *    affect. If so, it will flag the sidebar to be redrawn.                                   *
 *                                                                                             *
 * INPUT:   cell  -- The cell that the offset list is base on.                                 *
 *                                                                                             *
 *          list  -- The list of cell offset used to flag for redraw. If the special sidebar   *
 *                   affecting cell magic offset number is detected, the sidebar is flagged    *
 *                   for redraw and the magic offset is removed.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void PowerClass::Refresh_Cells(CELL cell, short const * list)
{
	if (*list == REFRESH_SIDEBAR) {
		IsToRedraw = true;
		Flag_To_Redraw(false);
	}
	RadarClass::Refresh_Cells(cell, list);
}


/***************************************************************************
 * PowerClass::Power_Height -- Given a value figure where it falls on bar  *
 *                                                                         *
 * INPUT:		int value - the value we are testing                        *
 *                                                                         *
 * OUTPUT:     int the height of the point that this value is on graph     *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/14/1995 PWG : Created.                                             *
 *=========================================================================*/
int PowerClass::Power_Height(int value)
{
	int num		= value/ POWER_STEP_LEVEL;		// figure out the initial num of DRAIN_VALUE's
	int retval	= 0;									// currently there is no power

	/*
	** Loop through the different hundreds figuring out the fractional piece
	** of each.
	*/
	for (int lp = 0; lp < num; lp ++)  {
		retval  = retval + (((POWER_HEIGHT - 2) - retval) / POWER_STEP_FACTOR);
		value  -= POWER_STEP_LEVEL;
	}

	/*
	** Adjust the retval to factor in the remainder
	*/
	if (value) {
		retval = retval + (((((POWER_HEIGHT - 2) - retval) / POWER_STEP_FACTOR) * value) / POWER_STEP_LEVEL);
	}

	retval = Bound(retval, 0, POWER_HEIGHT-2);
	return(retval);
}


/***********************************************************************************************
 * PowerClass::PowerButtonClass::Action -- Handles the mouse over the power bar area.          *
 *                                                                                             *
 *    This routine handles input on the power bar area. Since no input is used for the power   *
 *    bar, this routine just pops up appropriate help text for the power bar.                  *
 *                                                                                             *
 * INPUT:   flags    -- The event flags that triggered this action call.                       *
 *                                                                                             *
 *          key      -- The key code (if any) associated with the trigger event.               *
 *                                                                                             *
 * OUTPUT:  Should further button processing be stopped?                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/07/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int PowerClass::PowerButtonClass::Action(unsigned flags, KeyNumType & key)
{
	if (!Map.IsSidebarActive) {
		return(false);
	}

	/*
	**	Force any help label to disappear when the mouse is held over the
	**	radar map.
	*/
	Map.Override_Mouse_Shape(MOUSE_NORMAL);
	if (PlayerPtr->Power_Fraction() < 1 && PlayerPtr->Power > 0) {
		Map.Help_Text(TXT_POWER_OUTPUT_LOW, -1, -1, GadgetClass::Get_Color_Scheme()->Color);
	} else {
		Map.Help_Text(TXT_POWER_OUTPUT, -1, -1, GadgetClass::Get_Color_Scheme()->Color);
	}
	GadgetClass::Action(flags, key);
	return(true);
}


/***********************************************************************************************
 * PowerClass::Flash_Power -- Flag the power bar to flash.                                     *
 *                                                                                             *
 *    This will cause the power bar to display with a flash so as to draw attention to         *
 *    itself. Typical use of this effect is when power is low.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void PowerClass::Flash_Power(void)
{
	FlashTimer = TICKS_PER_SECOND;
	IsToRedraw = true;
	Flag_To_Redraw(false);
}
