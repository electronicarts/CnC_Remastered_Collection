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

/* $Header:   F:\projects\c&c\vcs\code\tab.cpv   2.18   16 Oct 1995 16:52:04   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : August 25, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TabClass::AI -- Handles player I/O with the tab buttons.                                  *
 *   TabClass::Draw_It -- Displays the tab buttons as necessary.                               *
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
TabClass::TabClass(void)
{
	IsToRedraw = false;
//	Select = -1;
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
void TabClass::Draw_It(bool complete)
{

	SidebarClass::Draw_It(complete);

	if (Debug_Map){
		//HidPage.Unlock();
		return;
	}

// Disable tab drawing. ST - 3/1/2019 11:35AM
#if 0
	/*
	**	Redraw the top bar imagery if flagged to do so or if the entire display needs
	**	to be redrawn.
	*/
	int width  = SeenBuff.Get_Width();
	int rightx = width - 1;

	if (complete || IsToRedraw) {
		
		if (Tab_Height != 0) {
		

			if (LogicPage->Lock()){

				LogicPage->Fill_Rect(0, 0, rightx, Tab_Height-2, BLACK);
				CC_Draw_Shape(TabShape, 0, 0, 0, WINDOW_MAIN, SHAPE_NORMAL);
				CC_Draw_Shape(TabShape, 0, width-Eva_Width, 0, WINDOW_MAIN, SHAPE_NORMAL);
				Draw_Credits_Tab();
				LogicPage->Draw_Line(0, Tab_Height-1, rightx, Tab_Height-1, BLACK);

				Fancy_Text_Print(TXT_TAB_BUTTON_CONTROLS, Eva_Width/2, 0, 11, TBLACK, TPF_GREEN12_GRAD|TPF_CENTER | TPF_USE_GRAD_PAL);
				Fancy_Text_Print(TXT_TAB_SIDEBAR, width-(Eva_Width/2), 0, 11, TBLACK, TPF_GREEN12_GRAD|TPF_CENTER | TPF_USE_GRAD_PAL);
			}
			LogicPage->Unlock();


		}

	}

	Credits.Graphic_Logic(complete || IsToRedraw);
#endif
	IsToRedraw = false;
}


void TabClass::Draw_Credits_Tab(void)
{
	CC_Draw_Shape(TabShape, 0, 320, 0, WINDOW_MAIN, SHAPE_NORMAL);
}


/***********************************************************************************************
 * TC::Hilite_Tab -- Draw a tab in its depressed state                                         *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Tab to draw (not used)                                                            *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/21/96 10:47AM ST : Created                                                              *
 *=============================================================================================*/

void TabClass::Hilite_Tab(int tab)
{
	int xpos = 0;
	int text = TXT_TAB_BUTTON_CONTROLS;
	tab = tab;

	CC_Draw_Shape(TabShape, 1 , xpos, 0, WINDOW_MAIN, SHAPE_NORMAL);
	Fancy_Text_Print(TXT_TAB_BUTTON_CONTROLS, 80, 0, 11, TBLACK, TPF_GREEN12|TPF_CENTER | TPF_USE_GRAD_PAL);
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
	if (y >= 0 && y < Tab_Height && x < (SeenBuff.Get_Width() - 1) && x > 0) {

		bool ok = false;
		int	width = SeenBuff.Get_Width();

		/*
		**	If the mouse is at the top of the screen, then the tab bars only work
		**	in certain areas. If the special scroll modification is not active, then
		**	the tabs never work when the mouse is at the top of the screen.
		*/
		if (y > 0 || (Special.IsScrollMod && ((x > 3 && x < Eva_Width) || (x < width-3 && x > width-Eva_Width)))) {
			ok = true;
		}

		if (ok) {
			if (input == KN_LMOUSE) {
				int sel = -1;
				if (x < Eva_Width) sel = 0;
				if (x > width-Eva_Width) sel = 1;
				if (sel >= 0) {
					Set_Active(sel);
					input = KN_NONE;
				}
			}

			Override_Mouse_Shape(MOUSE_NORMAL, false);
		}
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

void TabClass::One_Time(void)
{
	int factor = (SeenBuff.Get_Width() == 320) ? 1 : 2;
	Eva_Width	= 80 * factor;
	//Tab_Height	= 8 * factor;
	Tab_Height	= 0;		// Disable tab drawing. ST - 3/1/2019 11:35AM

	SidebarClass::One_Time();
	TabShape = Hires_Retrieve("TABS.SHP");
}