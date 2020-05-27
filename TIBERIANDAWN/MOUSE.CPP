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

/* $Header:   F:\projects\c&c\vcs\code\mouse.cpv   2.18   16 Oct 1995 16:49:56   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MOUSE.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/15/94                                                     *
 *                                                                                             *
 *                  Last Update : June 30, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   MouseClass::AI -- Process player input as it relates to the mouse                         *
 *   MouseClass::Override_Mouse_Shape -- Alters the shape of the mouse.                        *
 *   MouseClass::Init_Clear -- Sets the mouse system to a known state                          *
 *   MouseClass::MouseClass -- Default constructor for the mouse handler class.                *
 *   MouseClass::One_Time -- Performs the one time initialization of the mouse system.         *
 *   MouseClass::Set_Default_Mouse -- Sets the mouse to match the shape specified.             *
 *   MouseClass::Revert_Mouse_Shape -- Reverts the mouse shape to the non overridden shape.    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/*
**	This points to the loaded mouse shapes.
*/
void const * MouseClass::MouseShapes;

/*
**	This is the timer that controls the mouse animation. It is always at a fixed
**	rate so it uses the constant system timer.
*/
CountDownTimerClass MouseClass::Timer;

/*
** This contains the value of the Virtual Function Table Pointer
*/
void * MouseClass::VTable;


/***********************************************************************************************
 * MouseClass::Set_Default_Mouse -- Sets the mouse to match the shape specified.               *
 *                                                                                             *
 *    This routine is used to inform the display system as to which mouse shape is desired.    *
 *                                                                                             *
 * INPUT:   mouse -- The mouse shape number to set the mouse to.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MouseClass::Set_Default_Mouse(MouseType mouse, bool size)
{
	NormalMouseShape = mouse;
	Override_Mouse_Shape(mouse, size);
}


/***********************************************************************************************
 * MouseClass::Revert_Mouse_Shape -- Reverts the mouse shape to the non overridden shape.      *
 *                                                                                             *
 *    Use this routine to cancel the effects of Override_Mouse_Shape(). It will revert the     *
 *    mouse back to the original shape.                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void MouseClass::Revert_Mouse_Shape(void)
{
	Override_Mouse_Shape(NormalMouseShape, false);
}


void MouseClass::Mouse_Small(bool wwsmall)
{
	MouseStruct const * control = &MouseControl[CurrentMouseShape];

	if (IsSmall == wwsmall) {
		return;
	}

	IsSmall	= wwsmall;

	if (wwsmall) {
		if (control->SmallFrame != -1) {
			Set_Mouse_Cursor(control->X, control->Y, Extract_Shape(MouseShapes, control->SmallFrame + Frame/4));
		} else {
			Set_Mouse_Cursor(MouseControl[MOUSE_NORMAL].X, MouseControl[MOUSE_NORMAL].Y, Extract_Shape(MouseShapes, MOUSE_NORMAL));
		}
	} else {
		Set_Mouse_Cursor(control->X, control->Y, Extract_Shape(MouseShapes, control->StartFrame + Frame/4));
	}
}


/***********************************************************************************************
 * MouseClass::Override_Mouse_Shape -- Alters the shape of the mouse.                          *
 *                                                                                             *
 *    This routine is used to alter the shape of the mouse as needed.                          *
 *    Typical mouse shape change occurs when scrolling the map or                              *
 *    selecting targets.                                                                       *
 *                                                                                             *
 * INPUT:   mouse -- The mouse shape number to use.                                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the mouse shape changed?                                                 *
 *                                                                                             *
 * WARNINGS:   This is not intended to be used as a means to hide the                          *
 *             mouse. Nor will it work correctly if the mouse shape                            *
 *             file hasn't been loaded.                                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1994 JLB : Created.                                                                 *
 *   06/03/1994 JLB : Made into member function.                                               *
 *   12/24/1994 JLB : Added small control parameter.                                           *
 *=============================================================================================*/
bool MouseClass::Override_Mouse_Shape(MouseType mouse, bool wwsmall)
{
	MouseStruct const * control = &MouseControl[mouse];
	static bool startup = false;
	int baseshp;

	/*
	**	Only certain mouse shapes have a small counterpart. If the requested mouse
	**	shape is not one of these, then force the small size override flag to false.
	*/
	if (control->SmallFrame == -1) {
		wwsmall = false;
	}

	/*
	**	If the mouse shape is going to change, then inform the mouse driver of the
	**	change.
	*/
	if (!startup || (MouseShapes && ((mouse != CurrentMouseShape) || (wwsmall != IsSmall)))) {
		startup = true;

		Timer.Set(control->FrameRate);
		Frame = 0;

#ifdef OBSOLETE
		Control.Set_Stage(0);
		int rate = Options.Normalize_Delay(control->FrameRate);
		Control.Set_Rate(MAX(rate, 1));
#endif
		baseshp = (wwsmall) ? control->SmallFrame : control->StartFrame;

		Set_Mouse_Cursor(control->X, control->Y, Extract_Shape(MouseShapes, baseshp + Frame/4));
		CurrentMouseShape = mouse;
		IsSmall = wwsmall;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * MouseClass::AI -- Process player input as it relates to the mouse                           *
 *                                                                                             *
 *    This routine will is to be called once per game tick and is passed the player keyboard   *
 *    or mouse input code. It processes this code and updates the mouse shape as appropriate.  *
 *                                                                                             *
 * INPUT:   input -- The player input code as returned from Keyboard::Get().                   *
 *                                                                                             *
 *          x,y   -- The mouse coordinate values to use.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *   12/31/1994 JLB : Uses mouse coordinate parameters.                                        *
 *   03/27/1995 JLB : New animation control.                                                   *
 *   05/28/1995 JLB : Moderates animation so is more steady regardless of speed.               *
 *   06/30/1995 JLB : Uses constant timer system.                                              *
 *=============================================================================================*/
void MouseClass::AI(KeyNumType &input, int x, int y)
{
//	bool doit = false;
	void *mouse_shape_ptr;
	MouseStruct const * control = &MouseControl[CurrentMouseShape];

	if (control->FrameRate && Timer.Time() == 0) {

		Frame++;
		Frame %= control->FrameCount;
		Timer.Set(control->FrameRate);

#ifdef OBSOLETE
		Control.Set_Stage(Control.Fetch_Stage() % control->FrameCount);
#endif

		if (!IsSmall || control->SmallFrame != -1) {
			int baseframe = (IsSmall) ? control->SmallFrame : control->StartFrame;
			mouse_shape_ptr = Extract_Shape(MouseShapes, baseframe + Frame);
			if (mouse_shape_ptr){
				Set_Mouse_Cursor(control->X, control->Y, mouse_shape_ptr);
			}
		}
	}

	ScrollClass::AI(input, x, y);
}


/***********************************************************************************************
 * MouseClass::MouseClass -- Default constructor for the mouse handler class.                  *
 *                                                                                             *
 *    This is the default constructor for the mouse handling class. It merely sets up the      *
 *    mouse system to its default state.                                                       *
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
MouseClass::MouseClass(void)
{
	CurrentMouseShape = MOUSE_NORMAL;
	NormalMouseShape = MOUSE_NORMAL;
	Timer.Start();
}


/***********************************************************************************************
 * MouseClass::One_Time -- Performs the one time initialization of the mouse system.           *
 *                                                                                             *
 *    Use this routine to load the mouse data file and perform any other necessary one time    *
 *    preparations for the game.                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine ONCE.                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MouseClass::One_Time(void)
{
	ScrollClass::One_Time();

	/*
	**	Override the mouse shape file with the one in the current directory, but only if there
	**	is an override file available.
	*/
	RawFileClass file("MOUSE.SHP");
	if (file.Is_Available()) {
		MouseShapes = Load_Alloc_Data(file);
	} else {
		MouseShapes = MixFileClass::Retrieve("MOUSE.SHP");
	}

	VTable = ((void **)(((char *)this) + sizeof(VectorClass<CellClass>) - 4))[0];
}


/***********************************************************************************************
 * MouseClass::Init_Clear -- Sets the mouse system to a known state                            *
 *                                                                                             *
 *    This routine will reset the mouse handling system. Typically, this routine is called     *
 *    when preparing for the beginning of a new scenario.                                      *
 *                                                                                             *
 * INPUT:   theater  -- The theater that the scenario will take place.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MouseClass::Init_Clear(void)
{
	ScrollClass::Init_Clear();
	IsSmall = false;
	NormalMouseShape = MOUSE_NORMAL;
}


/*
**	This array of structures is used to control the mouse animation
**	sequences.
*/
MouseClass::MouseStruct MouseClass::MouseControl[MOUSE_COUNT] = {
	{0, 	1,		0,		86,	 0,	0},	//	MOUSE_NORMAL
	{1, 	1,		0,		-1,	15,	0},	//	MOUSE_N
	{2, 	1,		0,		-1,	29,	0},	//	MOUSE_NE
	{3, 	1,		0,		-1,	29,	12},	//	MOUSE_E
	{4,	1,		0,		-1,	29,	23},	//	MOUSE_SE
	{5,	1,		0,		-1,	15,	23},	//	MOUSE_S
	{6,	1,		0,		-1,	0, 	23},	//	MOUSE_SW
	{7,	1,		0,		-1,	0, 	13},	//	MOUSE_W
	{8,	1,		0,		-1,	0, 	0},	//	MOUSE_NW

	{130, 1,		0,		-1,	15,	0},	//	MOUSE_NO_N
	{131, 1,		0,		-1,	29,	0},	//	MOUSE_NO_NE
	{132, 1,		0,		-1,	29,	12},	//	MOUSE_NO_E
	{133,	1,		0,		-1,	29,	23},	//	MOUSE_NO_SE
	{134,	1,		0,		-1,	15,	23},	//	MOUSE_NO_S
	{135,	1,		0,		-1,	0, 	23},	//	MOUSE_NO_SW
	{136,	1,		0,		-1,	0, 	13},	//	MOUSE_NO_W
	{137,	1,		0,		-1,	0, 	0},	//	MOUSE_NO_NW

	{11,	1,		0,		27,	15,	12},	//	MOUSE_NO_MOVE
	{10,	1,		0,		26,	15,	12},	//	MOUSE_CAN_MOVE
	{119,	3,		4,		148,	15,	12},	//	MOUSE_ENTER
	{53,	9,		4,		-1,	15,	12},	//	MOUSE_DEPLOY
	{12,	6,		4,		-1,	15,	12},	//	MOUSE_CAN_SELECT
	{18,	8,		4,		140,	15,	12},	//	MOUSE_CAN_ATTACK
	{62,	24,	2,		-1,	15,	12},	//	MOUSE_SELL_BACK
	{154,	24,	2,		-1,	15,	12},	//	MOUSE_SELL_UNIT
	{29,	24,	2,		-1,	15,	12},	//	MOUSE_REPAIR
	{126,	1,		0,		-1,	15,	12},	//	MOUSE_NO_REPAIR
	{125,	1,		0,		-1,	15,	12},	//	MOUSE_NO_SELL_BACK
	{87,	1,		0,		151,	0, 	0},	//	MOUSE_RADAR_CURSOR
	{103,	16,	2,		-1,	15,	12},	//	MOUSE_ION_CANNON
	{96,	7,		4,		-1,	15,	12},	//	MOUSE_NUCLEAR_BOMB
	{88,	8,		2,		-1,	15,	12},	//	MOUSE_AIR_STRIKE
	{122,	3,		4,		127,	15,	12},	//	MOUSE_DEMOLITIONS
	{153,	1,		0,		152,	15,	12},	//	MOUSE_AREA_GUARD
};
