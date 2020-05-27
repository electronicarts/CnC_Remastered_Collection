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

/* $Header:   F:\projects\c&c\vcs\code\power.cpv   2.18   16 Oct 1995 16:52:10   JOE_BOSTIC  $ */
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
 *                  Last Update : August 7, 1995 [JLB]                                         *
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
 *   Power_Height -- Given a value figure where it falls on bar                                *
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
PowerClass::PowerClass(void)
{
	IsToRedraw	= false;
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

	int factor = Get_Resolution_Factor();
	PowX 			= SeenBuff.Get_Width() - Map.RadWidth;
	PowY 			= Map.RadY+Map.RadHeight + (13 << factor);
	PowWidth 	= 8 << factor;
	PowHeight 	= SeenBuff.Get_Height() - PowY;
	PowLineSpace = 5 << factor;
	PowLineWidth = PowWidth - 4;

	PowerButton.X = PowX;
	PowerButton.Y = PowY;
	PowerButton.Width = PowWidth-1;
	PowerButton.Height = PowHeight;

	PowerShape = MixFileClass::Retrieve((factor)? "HPOWER.SHP" :"POWER.SHP");
	PowerBarShape = Hires_Retrieve("PWRBAR.SHP");
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
	int power_color;

	if (complete || IsToRedraw) {
//		PowX = TacPixelX + TacWidth*ICON_PIXEL_W;	// X position of upper left corner of power bar.

		if (LogicPage->Lock()){

			if (Map.IsSidebarActive) {
				IsToRedraw = false;

				/*
				** 1st get the height of the filled section of the power bar
				*/
				int bottom       = PowY + PowHeight - 1;
				int power_height  = (PowerHeight == DesiredPowerHeight) ? PowerHeight + (_modtable[PowerBounce] * PowerDir) : PowerHeight;
				int drain_height  = (DrainHeight == DesiredDrainHeight) ? DrainHeight + (_modtable[DrainBounce] * DrainDir) : DrainHeight;
				power_height = Bound(power_height, 0, PowHeight - 2);
				drain_height = Bound(drain_height, 0, PowHeight - 2);

				/*
				** Create a clip region to draw the unfilled section of the bar
				*/
				WindowList[WINDOW_CUSTOM][WINDOWX] = 0;
				WindowList[WINDOW_CUSTOM][WINDOWY] = 0;
				WindowList[WINDOW_CUSTOM][WINDOWWIDTH] = SeenBuff.Get_Width();
				WindowList[WINDOW_CUSTOM][WINDOWHEIGHT] = bottom-power_height;

				/*
				** Draw the unfilled section
				*/
				CC_Draw_Shape(PowerBarShape, 0, PowX, PowY, WINDOW_CUSTOM, SHAPE_WIN_REL);
				CC_Draw_Shape(PowerBarShape, 1 ,PowX, PowY+100, WINDOW_CUSTOM, SHAPE_WIN_REL);


				/*
				** Set up the clip region for the filled section
				*/
				WindowList[WINDOW_CUSTOM][WINDOWY] = bottom-power_height;
				WindowList[WINDOW_CUSTOM][WINDOWHEIGHT] = SeenBuff.Get_Height() - WindowList[WINDOW_CUSTOM][WINDOWY];

				/*
				** What color is the filled section?
				*/
				if (power_height) {
					power_color = 0;	//green

					if (PlayerPtr->Drain > PlayerPtr->Power) {
						power_color = 2;
					}
					if (PlayerPtr->Drain > (PlayerPtr->Power * 2)) {
						power_color = 4;
					}

					/*
					** Draw the filled section
					*/
					CC_Draw_Shape(PowerBarShape, 2+power_color,
															PowX,
															PowY - WindowList[WINDOW_CUSTOM][WINDOWY],
															WINDOW_CUSTOM,
															SHAPE_WIN_REL);

					CC_Draw_Shape(PowerBarShape, 3+power_color,
															PowX,
															PowY - WindowList[WINDOW_CUSTOM][WINDOWY] + 100,
															WINDOW_CUSTOM,
															SHAPE_WIN_REL);
				}

				/*
				**	Draw the power drain threshold marker.
				*/
				CC_Draw_Shape(PowerShape, 0, PowX, bottom - drain_height + 1, WINDOW_MAIN, SHAPE_NORMAL);

			}
			LogicPage->Unlock();
		}
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
//	if (!IsActive) {
//		IsActive = true;
//		IsToRedraw = true;
//		Flag_To_Redraw(false);
//	}

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
void PowerClass::Refresh_Cells(CELL cell, short const *list)
{
	if (*list == REFRESH_SIDEBAR) {
		IsToRedraw = true;
		Flag_To_Redraw(false);
	}
	RadarClass::Refresh_Cells(cell, list);
}


/***************************************************************************
 * PowHeight -- Given a value figure where it falls on bar              *
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
	int num		= value/ POWER_STEP_LEVEL;	// figure out the initial num of DRAIN_VALUE's
	int retval	= 0;									// currently there is no power

	/*
	** Loop through the diffrent hundreds figuring out the fractional piece
	** of each.
	*/
	for (int lp = 0; lp < num; lp ++)  {
		retval  = retval + (((PowHeight - 2) - retval) / POWER_STEP_FACTOR);
		value  -= POWER_STEP_LEVEL;
	}

	/*
	** Adjust the retval to factor in the remainder
	*/
	if (value) {
		retval = retval + (((((PowHeight - 2) - retval) / POWER_STEP_FACTOR) * value) / POWER_STEP_LEVEL);
	}

	retval = Bound(retval, 0, PowHeight -2);
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
	if (PlayerPtr->Power_Fraction() < 0x0100 && PlayerPtr->Power > 0) {
		Map.Help_Text(TXT_POWER_OUTPUT_LOW, -1, -1, CC_GREEN);
	} else {
		Map.Help_Text(TXT_POWER_OUTPUT, -1, -1, CC_GREEN);
	}
	GadgetClass::Action(flags, key);
	return(true);
}


