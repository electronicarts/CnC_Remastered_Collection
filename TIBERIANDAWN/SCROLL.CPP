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

/* $Header:   F:\projects\c&c\vcs\code\scroll.cpv   2.18   16 Oct 1995 16:49:08   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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


#define	SCROLL_DELAY	1

CountDownTimerClass ScrollClass::Counter;


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
ScrollClass::ScrollClass(void)
{
	IsAutoScroll = true;
	Counter.Set(SCROLL_DELAY);
	Inertia = 0;
	Counter.Start();
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
	static DirType direction;
	bool				player_scrolled=false;

	/*
	**	If rubber band mode is in progress, then don't allow scrolling of the tactical map.
	*/
	if (!IsRubberBand /*&& !IsTentative*/) {

		/*
		**	Special check to not scroll within the special no-scroll regions.
		*/
		bool noscroll = false;
		if (Special.IsScrollMod && y == 0 && ((x > 3 && x < EVA_WIDTH) || (x > SeenBuff.Get_Width()-EVA_WIDTH && x < SeenBuff.Get_Width()-3))) {
			noscroll = true;
		}

		if (!noscroll) {

			/*
			**	Verify that the mouse is over a scroll region.
			*/
			if (Inertia || y == 0 || x == 0 || x == (SeenBuff.Get_Width()-1) || y == (SeenBuff.Get_Height()-1)) {

				if (y == 0 || x == 0 || x == (SeenBuff.Get_Width()-1) || y == (SeenBuff.Get_Height()-1)) {

					player_scrolled=true;
					/*
					**	Adjust the mouse coordinates to emphasise the
					**	cardinal directions over the diagonals.
					*/
					int altx = x;
					if (altx < 50) altx -= (50-altx)*2;
					altx = MAX(altx, 0);
					if (altx > (SeenBuff.Get_Width()-50)) altx += (altx-(SeenBuff.Get_Width()-50))*2;
					altx = MIN(altx, SeenBuff.Get_Width());
					if (altx > 50 && altx < (SeenBuff.Get_Width()-50)) {
						altx += ((SeenBuff.Get_Width()/2)-altx)/2;
					}

					int alty = y;
					if (alty < 50) alty -= (50-alty);
					alty = MAX(alty, 0);
					if (alty > (SeenBuff.Get_Height()-50)) alty += ((alty-(SeenBuff.Get_Height()-50)));
					alty = MIN(alty, SeenBuff.Get_Height());

					direction = (DirType)Desired_Facing256((SeenBuff.Get_Width())/2, (SeenBuff.Get_Height())/2, altx, alty);
				}
				int control = Dir_Facing(direction);

				/*
				**	The mouse is over a scroll region so set the mouse shape accordingly if the map
				**	can be scrolled in the direction indicated.
				*/
				static int _rate[9] = {
					0x01C0,
					0x0180,
					0x0140,
					0x0100,
					0x00C0,
					0x0080,
					0x0040,
					0x0020,
					0x0010
				};

				int rate = 8-Inertia;

				if (rate<Options.ScrollRate+1){
					rate = Options.ScrollRate+1;
					Inertia = 8-rate;
				}

				/*
				**	Increase the scroll rate if the mouse button is held down.
				*/
	//			if (Keyboard::Down(KN_LMOUSE)) {
	//				rate = Bound(rate-3, 0, 4);
	//			}
				if (Keyboard::Down(KN_RMOUSE)) {
					rate = Bound(rate+1, 4, (int)(sizeof(_rate)/sizeof(_rate[0]))-1);
				}

				/*
				**	If options indicate that scrolling should be forced to
				**	one of the 8 facings, then adjust the direction value
				**	accordingly.
				*/
				if (!Options.IsFreeScroll) {
					direction = Facing_Dir(Dir_Facing(direction));
				}

				int distance = _rate[rate]/2;

				if (!Scroll_Map(direction, distance, false)) {
					Override_Mouse_Shape((MouseType)(MOUSE_NO_N+control), false);
				} else {
					Override_Mouse_Shape((MouseType)(MOUSE_N+control), false);

					/*
					**	If the mouse button is pressed or auto scrolling is active, then scroll
					**	the map if the delay counter indicates.
					*/
					if (Keyboard::Down(KN_LMOUSE) || IsAutoScroll) {
						distance = _rate[rate];
						Scroll_Map(direction, distance, true);

						if (Counter.Time()==0 && player_scrolled) {
							Counter.Set(SCROLL_DELAY);
							Inertia++;
						}
					}
				}
			}
			if (!player_scrolled){
				if (Counter.Time()==0) {
					Inertia--;
					if (Inertia<0) Inertia++;
					Counter.Set(SCROLL_DELAY);
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
 *                      -1 = toggle currrent setting                                           *
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


