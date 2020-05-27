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

/* $Header: /CounterStrike/TAB.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TAB.CPP                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/15/94                                                     *
 *                                                                                             *
 *                  Last Update : September 20, 1995 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TabClass::AI -- Handles player I/O with the tab buttons.                                  *
 *   TabClass::Draw_It -- Displays the tab buttons as necessary.                               *
 *   TabClass::One_Time -- Performs one time initialization of tab handler class.              *
 *   TabClass::Set_Active -- Activates a "filefolder tab" button.                              *
 *   TabClass::TabClass -- Default construct for the tab button class.                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


void const * TabClass::TabShape = NULL;


/***********************************************************************************************
 * TabClass::TabClass -- Default construct for the tab button class.                           *
 *                                                                                             *
 *    The default constructor merely sets the tab buttons to default non-selected state.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TabClass::TabClass(void) :
	FlasherTimer(0),
	IsToRedraw(false),
	MoneyFlashTimer(0)
{
}


/***********************************************************************************************
 * TabClass::Draw_It -- Displays the tab buttons as necessary.                                 *
 *                                                                                             *
 *    This routine is called whenever the display is being redrawn (in some fashion). The      *
 *    parameter can be used to force the tab buttons to redraw completely. The default action  *
 *    is to only redraw if the tab buttons have been explicitly flagged to be redraw. The      *
 *    result of this is the elimination of unnecessary redraws.                                *
 *                                                                                             *
 * INPUT:   complete -- bool; Force redraw of the entire tab button graphics?                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *   05/19/1995 JLB : New EVA style.                                                           *
 *=============================================================================================*/
#define	EVA_WIDTH		80
#define	TAB_HEIGHT		8
void TabClass::Draw_It(bool complete)
{
	SidebarClass::Draw_It(complete);

	if (Debug_Map) {
		return;
	}

// Disable tab drawing for menu, credits buttons etc. ST - 5/27/2019
#if (0)
	/*
	**	Redraw the top bar imagery if flagged to do so or if the entire display needs
	**	to be redrawn.
	*/
	if ((complete || IsToRedraw) && LogicPage->Lock()) {

		int width  = SeenBuff.Get_Width();
		int rightx = width - 1;
		int tab_height = TAB_HEIGHT * RESFACTOR;

		LogicPage->Fill_Rect(0, 0, rightx, tab_height-1, BLACK);
//		LogicPage->Fill_Rect(0, 0, rightx, tab_height-(2 * RESFACTOR), BLACK);

#ifdef WIN32
		/*
		** Use the new sidebar art for 640x400
		*/
		CC_Draw_Shape(TabShape, 0, 0, 0, WINDOW_MAIN, SHAPE_NORMAL);
#else
		CC_Draw_Shape(TabShape, 2, 0, 0, WINDOW_MAIN, SHAPE_NORMAL);
#endif
		Draw_Credits_Tab();
		LogicPage->Draw_Line(0, tab_height-(1* RESFACTOR), rightx, tab_height-(1 * RESFACTOR), BLACK);
#ifdef WIN32
		Fancy_Text_Print(TXT_TAB_BUTTON_CONTROLS, (EVA_WIDTH/2) * RESFACTOR, 0, &MetalScheme, TBLACK, TPF_METAL12 | TPF_CENTER | TPF_USE_GRAD_PAL);
#else
//			Fancy_Text_Print(TXT_TAB_BUTTON_CONTROLS, (EVA_WIDTH/2) * RESFACTOR, 0, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_6PT_GRAD|TPF_NOSHADOW|TPF_CENTER|TPF_BRIGHT_COLOR);
		Fancy_Text_Print(TXT_TAB_BUTTON_CONTROLS, (EVA_WIDTH/2) * RESFACTOR, 0, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_METAL12|TPF_NOSHADOW|TPF_CENTER|TPF_BRIGHT_COLOR);
#endif	//WIN32
		if (IsSidebarActive) {
#ifndef WIN32
			TabClass::Hilite_Tab(1);
#endif	//WIN32
		} else {
			CC_Draw_Shape(TabShape, 0, width-(EVA_WIDTH * RESFACTOR), 0, WINDOW_MAIN, SHAPE_NORMAL);
			Fancy_Text_Print(TXT_TAB_SIDEBAR, width-((EVA_WIDTH/2) * RESFACTOR), 0, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_METAL12|TPF_NOSHADOW|TPF_CENTER|TPF_BRIGHT_COLOR);
		}

		LogicPage->Unlock();
	}
	Credits.Graphic_Logic(complete || IsToRedraw);
#endif
	IsToRedraw = false;
}


void TabClass::Draw_Credits_Tab(void)
{
#ifdef WIN32
	/*
	** Use the new sidebar art for 640x400
	*/
	CC_Draw_Shape(TabShape, Map.MoneyFlashTimer > 1 ? 8 : 6, (320-EVA_WIDTH) * RESFACTOR, 0, WINDOW_MAIN, SHAPE_NORMAL);
#else
	CC_Draw_Shape(TabShape, 4, (320-(EVA_WIDTH*2)) * RESFACTOR, 0, WINDOW_MAIN, SHAPE_NORMAL);
#endif

	if (Scen.MissionTimer.Is_Active()) {
		bool light = ((int)Scen.MissionTimer < TICKS_PER_MINUTE * Rule.TimerWarning) || Map.FlasherTimer > 0;
#ifdef WIN32
		CC_Draw_Shape(TabShape, light ? 4 : 2, 320, 0, WINDOW_MAIN, SHAPE_NORMAL);
#else
		CC_Draw_Shape(TabShape, light ? 6 : 5, EVA_WIDTH * RESFACTOR, 0, WINDOW_MAIN, SHAPE_NORMAL);
#endif
	}
}


void TabClass::Hilite_Tab(int tab)
{
	int xpos = 0;
	int text = TXT_TAB_BUTTON_CONTROLS;
	int textx = (EVA_WIDTH/2) * RESFACTOR;

	if (tab) {
		xpos = (320-EVA_WIDTH) * RESFACTOR;
		text = TXT_TAB_SIDEBAR;
		textx = (320-(EVA_WIDTH/2)) * RESFACTOR;
	}

#ifdef WIN32
	/*
	** Use the new sidebar art for 640x400
	*/
	CC_Draw_Shape(TabShape, 1, xpos, 0, WINDOW_MAIN, SHAPE_NORMAL);
	MetalScheme.Color = 128+6;
	Fancy_Text_Print(TXT_TAB_BUTTON_CONTROLS, (EVA_WIDTH/2) * RESFACTOR, 0, &MetalScheme, TBLACK, TPF_METAL12 | TPF_CENTER | TPF_USE_GRAD_PAL);
	MetalScheme.Color = 128;
#else
	CC_Draw_Shape(TabShape, 1 + (tab ? 0 : 2), xpos, 0, WINDOW_MAIN, SHAPE_NORMAL);
	Fancy_Text_Print(text, textx, 0, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_METAL12|TPF_NOSHADOW|TPF_CENTER|TPF_BRIGHT_COLOR);
//	Fancy_Text_Print(text, textx, 0, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_6PT_GRAD|TPF_NOSHADOW|TPF_CENTER|TPF_BRIGHT_COLOR);
#endif
}


/***********************************************************************************************
 * TabClass::AI -- Handles player I/O with the tab buttons.                                    *
 *                                                                                             *
 *    This routine is called every game tick and passed whatever key the player has supplied.  *
 *    If the input selects a tab button, then the graphic gets updated accordingly.            *
 *                                                                                             *
 * INPUT:   input -- The player's input character (might be mouse click).                      *
 *                                                                                             *
 *          x,y   -- Mouse coordinates at time of input.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *   12/31/1994 JLB : Uses mouse coordinate parameters.                                        *
 *   05/31/1995 JLB : Fixed to handle mouse shape properly.                                    *
 *   08/25/1995 JLB : Handles new scrolling option.                                            *
 *=============================================================================================*/
void TabClass::AI(KeyNumType &input, int x, int y)
{
	if (y >= 0 && y < (TAB_HEIGHT * RESFACTOR) && x < (SeenBuff.Get_Width() - 1) && x > 0) {

		bool 	ok = false;
		int	width = SeenBuff.Get_Width();

		/*
		**	If the mouse is at the top of the screen, then the tab bars only work
		**	in certain areas. If the special scroll modification is not active, then
		**	the tabs never work when the mouse is at the top of the screen.
		*/
		if (y > 0) {
			ok = true;
		}

		if (ok) {
			if (input == KN_LMOUSE) {
				int sel = -1;
				if (x < EVA_WIDTH * RESFACTOR) sel = 0;
#ifndef WIN32	// No Sidebar tab in hires - sidebar is always active.
				if (x > (320-80) * RESFACTOR) sel = 1;
#endif	//WIN32
				if (sel >= 0) {
					Set_Active(sel);
					input = KN_NONE;
				}
			}

			Override_Mouse_Shape(MOUSE_NORMAL, false);
		}
	}

	if (MoneyFlashTimer == 1) {
		IsToRedraw = true;
		Flag_To_Redraw(false);
	}

	Credits.AI();

	SidebarClass::AI(input, x, y);
}


/***********************************************************************************************
 * TabClass::Set_Active -- Activates a "filefolder tab" button.                                *
 *                                                                                             *
 *    This function is used to activate one of the file folder tab buttons that appear at the  *
 *    top edge of the screen.                                                                  *
 *                                                                                             *
 * INPUT:   select   -- The button to activate. 0 = left button, 1=next button, etc.           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TabClass::Set_Active(int select)
{
	switch (select) {
		case 0:
			Queue_Options();
			break;

		case 1:
			Map.SidebarClass::Activate(-1);
			break;

		default:
			break;
	}
}


/***********************************************************************************************
 * TabClass::One_Time -- Performs one time initialization of tab handler class.                *
 *                                                                                             *
 *    This routine will perform any one time initializations of the tab handler class. This    *
 *    typically includes the loading of the shapes that appear on it.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TabClass::One_Time(void)
{
	SidebarClass::One_Time();
	RawFileClass file("tabs.shp");
	TabShape = MFCD::Retrieve("TABS.SHP");
}


void TabClass::Flash_Money(void)
{
	IsToRedraw = true;
	Flag_To_Redraw(false);
	MoneyFlashTimer = 7;
}
