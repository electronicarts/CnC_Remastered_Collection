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

/* $Header: /CounterStrike/HELP.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : HELP.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/18/94                                                     *
 *                                                                                             *
 *                  Last Update : September 22, 1995 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   HelpClass::Draw_Help -- Display the help message (if necessary).                          *
 *   HelpClass::HelpClass -- Default constructor for the help processor.                       *
 *   HelpClass::Help_AI -- Handles the help text logic.                                        *
 *   HelpClass::Help_Text -- Assigns text as the current help text.                            *
 *   HelpClass::Init_Clear -- Sets help system to a known state.                               *
 *   HelpClass::Overlap_List -- Returns with offset list for cells under help text.            *
 *   HelpClass::Scroll_Map -- Makes sure scrolling doesn't leave text shards.                  *
 *   HelpClass::Set_Cost -- Initiates the second line of help text showing item cost.          *
 *   HelpClass::Set_Tactical_Position -- Sets the position of the tactical map.                *
 *   HelpClass::Set_Tactical_Position -- Sets the tactical map position.                       *
 *   HelpClass::Set_Tactical_Position -- Sets the tactical map position.                       *
 *   HelpClass::Set_Text -- Determines the overlap list and draw coordinates.                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/*
**	This is the holding buffer for the text overlap list. This buffer must be in the near
**	data segment. It will be filled in by the Set_Text() function.
*/
//short const HelpClass::OverlapList[60] = {  // Can't be const - it's expected to be written to. ST - 2/7/2019 5:16PM		
short HelpClass::OverlapList[60] = {
	REFRESH_EOL
};

char const * HelpClass::HelpText;


/***********************************************************************************************
 * HelpClass::HelpClass -- Default constructor for the help processor.                         *
 *                                                                                             *
 *    The help processor is initialized by this routine. It merely sets up the help engine     *
 *    to the default state. The default state will not display any help text. Call the         *
 *    Help_Text() function to enable help processing.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
HelpClass::HelpClass(void) :
	HelpX(0),
	HelpY(0),
	HelpWidth(0),
	IsRight(false),
	Cost(0),
	X(0),
	Y(0),
	DrawX(0),
	DrawY(0),
	Width(0),
	Text(TXT_NONE),
	Color(LTGREY),
	CountDownTimer(0)
{
}


/***********************************************************************************************
 * HelpClass::Init_Clear -- Sets help system to a known state.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void HelpClass::Init_Clear(void)
{
	TabClass::Init_Clear();

	Set_Text(TXT_NONE);
}


/***********************************************************************************************
 * HelpClass::Overlap_List -- Returns with offset list for cells under help text.              *
 *                                                                                             *
 *    Use this routine to fetch an offset list for the cells under the text displayed. If      *
 *    there is no text displayed, then the list will consist of just the terminator code.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the offset list for the help text overlap. The offset    *
 *          list is based on the tactical map upper left corner cell.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * HelpClass::Overlap_List(void) const
{
	if (Text == TXT_NONE || CountDownTimer) {
		((short &)(OverlapList[0])) = REFRESH_EOL;
	}
	return(OverlapList);
}


/***********************************************************************************************
 * HelpClass::Help_AI -- Handles the help text logic.                                          *
 *                                                                                             *
 *    This routine handles tracking the mouse position to see if the mouse remains stationary  *
 *    for the required amount of time. If the time requirement has been met, then it flags     *
 *    the help system to display the help text the next time the Draw_Help() function is       *
 *    called.                                                                                  *
 *                                                                                             *
 * INPUT:   key   -- Keyboard input code.                                                      *
 *                                                                                             *
 *          x,y   -- Mouse coordinates.                                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine must be called once and only once per game frame (15 times per     *
 *             second).                                                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/18/1994 JLB : Created.                                                                 *
 *   12/31/1994 JLB : Uses mouse coordinates as passed in.                                     *
 *=============================================================================================*/
void HelpClass::AI(KeyNumType &key, int x, int y)
{
	if (!CountDownTimer && !IsRight && (x != X || y != Y)) {
		Help_Text(TXT_NONE);
	}

	/*
	**	Process the countdown timer only if it hasn't already expired and there is
	**	a real help text message to display.
	*/
	if (CountDownTimer && !HelpText && Text != TXT_NONE) {

		/*
		**	If the mouse has moved, then reset the timer since a moving mouse is not
		**	supposed to bring up the help text.
		*/
		if (!IsRight && (X != x || Y != y)) {
			X = x;
			Y = y;
			CountDownTimer = HELP_DELAY;
			Help_Text(TXT_NONE);
		} else {

			/*
			**	If the delay has expired, then the text must be drawn. Build the help text
			**	overlay list at this time. Better to do it now, when we KNOW it is needed, then
			**	to do it earlier when it might not be needed.
			*/
			Set_Text(Text);
		}
	}

	TabClass::AI(key, x, y);
}


/***********************************************************************************************
 * HelpClass::Help_Text -- Assigns text as the current help text.                              *
 *                                                                                             *
 *    Use this routine to change the help text that will pop up if the cursor isn't moved      *
 *    for the help delay duration. Call this routine as often as desired.                      *
 *                                                                                             *
 * INPUT:   text  -- The text number for the help text to use.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void HelpClass::Help_Text(int text, int x, int y, int color, bool quick)
{
	if (text != Text) {

		/*
		**	If there is an existing text message, then flag the map to redraw the underlying
		**	icons so that the text message is erased.
		*/
		if (Text != TXT_NONE) {
			Refresh_Cells(Coord_Cell(TacticalCoord), &OverlapList[0]);
		}

		/*
		**	Record the position of the mouse. This recorded position will be used to determine
		**	if the mouse has moved. A moving mouse prevents the help text from popping up.
		*/
		X = x;
		if (x == -1) X = Get_Mouse_X();
		Y = y;
		if (y == -1) Y = Get_Mouse_Y();
		IsRight = (y != -1) || (x != -1);

		if (quick) {
			CountDownTimer = 1;
		} else {
			CountDownTimer = HELP_DELAY;
		}

		/*
		** All help text prints in the same color for E3
		*/
		//Color = color;
		color = color;
		Color = HELP_TEXT_COLOR;
		Text = text;
		Cost = 0;
	}
}


/***********************************************************************************************
 * HelpClass::Draw_Help -- Display the help message (if necessary).                            *
 *                                                                                             *
 *    This function will print the help text if it thinks it should. The timer and text        *
 *    message can control whether this occurs. If there is no help text or the countdown timer *
 *    has not expired, then no text will be printed.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void HelpClass::Draw_It(bool forced)
{
	TabClass::Draw_It(forced);

	forced = false;		// TCTCTCTC
	if (Text != TXT_NONE && (forced || !CountDownTimer)) {

		if (LogicPage->Lock()) {
			Plain_Text_Print(Text, DrawX, DrawY, Color, BLACK, TPF_MAP|TPF_NOSHADOW);
			LogicPage->Draw_Rect(DrawX-1, DrawY-1, DrawX+Width+1, DrawY+FontHeight, Color);

			if (Cost) {
				char buffer[15];
				sprintf(buffer, "$%d", Cost);
				int width = String_Pixel_Width(buffer);

				Plain_Text_Print(buffer, DrawX, DrawY+FontHeight, Color, BLACK, TPF_MAP|TPF_NOSHADOW);
				LogicPage->Draw_Rect(DrawX-1, DrawY+FontHeight, DrawX+width+1, DrawY+FontHeight+FontHeight-1, Color);
				LogicPage->Draw_Line(DrawX, DrawY+FontHeight, DrawX+min(width+1, Width)-1, DrawY+FontHeight, BLACK);
			}
			LogicPage->Unlock();
		}
	}
}


/***********************************************************************************************
 * HelpClass::Set_Text -- Determines the overlap list and draw coordinates.                    *
 *                                                                                             *
 *    This routine is used to build the overlap list -- used for icon refreshing. It also      *
 *    determines if the text can fit on the screen and makes adjustments so that it will.      *
 *                                                                                             *
 * INPUT:   text  -- The text number to set the help system to use.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/18/1994 JLB : Created.                                                                 *
 *   12/11/1994 JLB : Won't draw past tactical map edges.                                      *
 *=============================================================================================*/
void HelpClass::Set_Text(int text)
{
	if (text != TXT_NONE) {
		Text = text;
		Plain_Text_Print(TXT_NONE, 0, 0, 0, 0, TPF_MAP|TPF_NOSHADOW);
		Width = String_Pixel_Width(Text_String(Text));
		if (IsRight) {
			DrawX = X - Width;
			DrawY = Y;
		} else {
			int right = TacPixelX + Lepton_To_Pixel(TacLeptonWidth) - 3*RESFACTOR;
			int bottom = TacPixelY + Lepton_To_Pixel(TacLeptonHeight) - 1*RESFACTOR;

			DrawX = X+X_OFFSET;
			DrawY = Y+Y_OFFSET;
			if (DrawX + Width > right) {
				DrawX -= (DrawX+Width) - right;
			}
			if (DrawY + 10*RESFACTOR > bottom) {
				DrawY -= (DrawY+10*RESFACTOR) - bottom;
			}
			if (DrawX < TacPixelX+1) DrawX = TacPixelX+1;
			if (DrawY < TacPixelY+1) DrawY = TacPixelY+1;
		}
		memcpy((void*)OverlapList, Text_Overlap_List(Text_String(Text), DrawX-1, DrawY), sizeof(OverlapList));
		*(short *)&OverlapList[ARRAY_SIZE(OverlapList)-1] = REFRESH_EOL;
	}
}


/***********************************************************************************************
 * HelpClass::Scroll_Map -- Makes sure scrolling doesn't leave text shards.                    *
 *                                                                                             *
 *    This routine intercepts the map scrolling request and then makes sure that if, in fact,  *
 *    the map is going to scroll, then reset and erase the help text so that it doesn't        *
 *    mess up the display.                                                                     *
 *                                                                                             *
 * INPUT:   facing   -- The direction to scroll (unused by this routine).                      *
 *                                                                                             *
 *          really   -- If the scroll is actually going to occur, rather than just be examined *
 *                      for legality, then this parameter will be true. If this parameter is   *
 *                      true, then the help text is reset.                                     *
 *                                                                                             *
 * OUTPUT:  Returns if it can, or did, scroll in the requested direction.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HelpClass::Scroll_Map(DirType facing, int & distance, bool really)
{
	if (really) {
		Help_Text(TXT_NONE);
	}
	return(TabClass::Scroll_Map(facing, distance, really));
}


/***********************************************************************************************
 * HelpClass::Set_Cost -- Initiates the second line of help text showing item cost.            *
 *                                                                                             *
 *    Use this routine after the Help_Text() function to activate the second line. The second  *
 *    line displays a cost. Typically, this is used by the sidebar to display the cost of the  *
 *    specified item.                                                                          *
 *                                                                                             *
 * INPUT:   cost  -- The cost to associate with this help text. If this value is zero, then    *
 *                   no second line is displayed, so don't pass in zero.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/09/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HelpClass::Set_Cost(int cost)
{
	Cost = cost;
}


/***********************************************************************************************
 * HelpClass::Set_Tactical_Position -- Sets the position of the tactical map.                  *
 *                                                                                             *
 *    This routine will set the position of the tactical map. At this class level, it merely   *
 *    makes sure that the help text disappears when this happens. The lower level classes      *
 *    actually change the map's position.                                                      *
 *                                                                                             *
 * INPUT:   coord -- The new coordinate to make the upper left corner of the visible display.  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HelpClass::Set_Tactical_Position(COORDINATE coord)
{
	if (TacticalCoord != coord) {
		Help_Text(TXT_NONE);
	}
	TabClass::Set_Tactical_Position(coord);
}
