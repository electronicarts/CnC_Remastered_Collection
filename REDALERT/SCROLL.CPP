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

/* $Header: /CounterStrike/SCROLL.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SCROLL.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/08/95                                                     *
 *                                                                                             *
 *                  Last Update : August 25, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ScrollClass::AI -- Handles scroll AI processing.                                          *
 *   ScrollClass::ScrollClass -- Constructor for the scroll class object.                      *
 *   ScrollClass::Set_Autoscroll -- Turns autoscrolling on or off.                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#ifdef WIN32
#define	SCROLL_DELAY	1
#else
#define	SCROLL_DELAY	2
#endif

CDTimerClass<SystemTimerClass> ScrollClass::Counter;


/***********************************************************************************************
 * ScrollClass::ScrollClass -- Constructor for the scroll class object.                        *
 *                                                                                             *
 *    This is the constructor for the scroll class object.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ScrollClass::ScrollClass(void) :
	IsAutoScroll(true)
{
	Counter = SCROLL_DELAY;
	Inertia = 0;
}


/***********************************************************************************************
 * ScrollClass::AI -- Handles scroll AI processing.                                            *
 *                                                                                             *
 *    This routine is called every game frame for purposes of input processing.                *
 *                                                                                             *
 * INPUT:   input    -- Reference to the keyboard/mouse event that just occurred.              *
 *                                                                                             *
 *          x,y      -- The mouse coordinates.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *   08/10/1995 JLB : Revamped for free smooth scrolling.                                      *
 *   08/25/1995 JLB : Handles new scrolling option.                                            *
 *=============================================================================================*/
#define	EVA_WIDTH		80
void ScrollClass::AI(KeyNumType &input, int x, int y)
{
#if 0
	bool		player_scrolled=false;
	static 	DirType	direction;
	int		rate;


	/*
	**	If rubber band mode is in progress, then don't allow scrolling of the tactical map.
	*/
	if (!IsRubberBand /*&& !IsTentative*/) {

		/*
		**	Special check to not scroll within the special no-scroll regions.
		*/
		bool noscroll = false;

		if (!noscroll) {
			bool at_screen_edge = (y == 0 || x == 0 || x >= SeenBuff.Get_Width()-1 || y >= SeenBuff.Get_Height()-1);

			/*
			**	Verify that the mouse is over a scroll region.
			*/
			if (Inertia || at_screen_edge) {
				if (at_screen_edge) {

					player_scrolled=true;

					/*
					**	Adjust the mouse coordinates to emphasize the
					**	cardinal directions over the diagonals.
					*/
					int altx = x;
					if (altx < 50 * RESFACTOR) altx -= ((50 * RESFACTOR)-altx);
					altx = max(altx, 0);
					if (altx > ((320-50) * RESFACTOR)) altx += altx-((320-50) * RESFACTOR);
					altx = min(altx, (320 * RESFACTOR));
					if (altx > (50 * RESFACTOR) && altx < ((320-50) * RESFACTOR)) {
						altx += (((320/2) * RESFACTOR)-altx)/2;
					}

					int alty = y;
					if (alty < (50 * RESFACTOR)) alty -= (50 * RESFACTOR)-alty;
					alty = max(alty, 0);
					if (alty > (150 * RESFACTOR)) alty += alty-(150 * RESFACTOR);
					alty = min(alty, 200 * RESFACTOR);

					direction = (DirType)Desired_Facing256((320/2) * RESFACTOR, (200/2) * RESFACTOR, altx, alty);
				}

				int control = Dir_Facing(direction);

				/*
				**	The mouse is over a scroll region so set the mouse shape accordingly if the map
				**	can be scrolled in the direction indicated.
				*/
				static int _rate[9] = {
					0x00E0*RESFACTOR,
					0x00C0*RESFACTOR,
					0x00A0*RESFACTOR,
					0x0080*RESFACTOR,
					0x0060*RESFACTOR,
					0x0040*RESFACTOR,
					0x0020*RESFACTOR,
					0x0010*RESFACTOR,
					0x0008*RESFACTOR
				};
				if (Debug_Map) {
					rate = Options.ScrollRate+1;
				} else {
					rate = 8-Inertia;
				}

				if (rate < Options.ScrollRate+1) {
					rate = Options.ScrollRate+1;
					Inertia = 8-rate;
				}

				/*
				**	Increase the scroll rate if the mouse button is held down.
				*/
	//			if (Keyboard->Down(KN_LMOUSE)) {
	//				rate = Bound(rate-3, 0, 4);
	//			}
				if (Keyboard->Down(KN_RMOUSE)) {
					rate = Bound(rate+1, 4, (int)(sizeof(_rate)/sizeof(_rate[0]))-1);
				}

				/*
				**	If options indicate that scrolling should be forced to
				**	one of the 8 facings, then adjust the direction value
				**	accordingly.
				*/
				direction = Facing_Dir(Dir_Facing(direction));

				int distance = _rate[rate]/2;

				if (!Scroll_Map(direction, distance, false)) {
					Override_Mouse_Shape((MouseType)(MOUSE_NO_N+control), false);
				} else {
					Override_Mouse_Shape((MouseType)(MOUSE_N+control), false);

					/*
					**	If the mouse button is pressed or auto scrolling is active, then scroll
					**	the map if the delay counter indicates.
					*/
					if (Keyboard->Down(KN_LMOUSE) || IsAutoScroll) {
						distance = _rate[rate];

						if (Debug_Map) {
							Scroll_Map(direction, distance, true);
							Counter = SCROLL_DELAY;
						} else {
							distance = _rate[rate];
							Scroll_Map(direction, distance, true);

							if (Counter == 0 && player_scrolled) {
								Counter = SCROLL_DELAY;
								Inertia++;
							}
						}
					}
				}

			}

			if (!Debug_Map && !player_scrolled) {
				if (!Counter) {
					Inertia--;
					if (Inertia<0) Inertia++;
					Counter = SCROLL_DELAY;
				}
			}

		}
	}
#endif
	HelpClass::AI(input, x, y);
}


/***********************************************************************************************
 * ScrollClass::Set_Autoscroll -- Turns autoscrolling on or off.                               *
 *                                                                                             *
 *    This routine controls the autoscrolling setting. Autoscroll, when active, will cause the *
 *    map to scroll if the mouse is held over the scroll region. This is regardless of whether *
 *    any mouse button is held down or not.                                                    *
 *                                                                                             *
 * INPUT:   control  -- Should the autoscroll be turned on?                                    *
 *                      0  = turn off                                                          *
 *                      1  = turn on                                                           *
 *                      -1 = toggle current setting                                            *
 *                                                                                             *
 * OUTPUT:  Returns with the old setting of the autoscroll flag.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ScrollClass::Set_Autoscroll(int control)
{
	bool old = IsAutoScroll;

	switch (control) {
		case -1:
			IsAutoScroll = !IsAutoScroll;
			break;

		default:
			IsAutoScroll = control;
			break;
	}
	return(old);
}


