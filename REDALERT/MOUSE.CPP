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

/* $Header: /CounterStrike/MOUSE.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *                  Last Update : September 21, 1995 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   MouseClass::AI -- Process player input as it relates to the mouse                         *
 *   MouseClass::Init_Clear -- Sets the mouse system to a known state                          *
 *   MouseClass::MouseClass -- Default constructor for the mouse handler class.                *
 *   MouseClass::Mouse_Small -- Controls the sizing of the mouse.                              *
 *   MouseClass::One_Time -- Performs the one time initialization of the mouse system.         *
 *   MouseClass::Override_Mouse_Shape -- Alters the shape of the mouse.                        *
 *   MouseClass::Revert_Mouse_Shape -- Reverts the mouse shape to the non overridden shape.    *
 *   MouseClass::Set_Default_Mouse -- Sets the mouse to match the shape specified.             *
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
CDTimerClass<SystemTimerClass> MouseClass::Timer = 0;


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
MouseClass::MouseClass(void) :
	IsSmall(false),
	CurrentMouseShape(MOUSE_NORMAL),
	NormalMouseShape(MOUSE_NORMAL),
	Frame(0)
{
}


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
	assert((unsigned)mouse < MOUSE_COUNT);

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


/***********************************************************************************************
 * MouseClass::Mouse_Small -- Controls the sizing of the mouse.                                *
 *                                                                                             *
 *    This routine is called to change the mouse sizing override. If the mouse can change      *
 *    size to that specified, then the mouse imagery will be changed. If a change of imagery   *
 *    cannot occur (due to lack of appropriate artwork), then no action will be performed.     *
 *                                                                                             *
 * INPUT:   small -- Should the mouse be made small? If not, then it will be made large.       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void MouseClass::Mouse_Small(bool wsmall)
{
	MouseStruct const * control = &MouseControl[CurrentMouseShape];

	if (IsSmall == wsmall) {
		return;
	}

	IsSmall	= wsmall;

	if (wsmall) {
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
#ifdef WIN32
void Block_Mouse(GraphicBufferClass *buffer);
void Unblock_Mouse(GraphicBufferClass *buffer);
#endif

bool MouseClass::Override_Mouse_Shape(MouseType mouse, bool wsmall)
{
	assert((unsigned)mouse < MOUSE_COUNT);

	MouseStruct const * control = &MouseControl[mouse];
	static bool startup = false;
	int baseshp;

	/*
	**	Only certain mouse shapes have a small counterpart. If the requested mouse
	**	shape is not one of these, then force the small size override flag to false.
	*/
	if (control->SmallFrame == -1) {
		wsmall = false;
	}

	/*
	**	If the mouse shape is going to change, then inform the mouse driver of the
	**	change.
	*/
	if (!startup || (MouseShapes && ((mouse != CurrentMouseShape) || (wsmall != IsSmall)))) {
		startup = true;

		Timer = control->FrameRate;
		Frame = 0;

		baseshp = (wsmall) ? control->SmallFrame : control->StartFrame;
		if (baseshp == -1) {
			baseshp = control->StartFrame;
		}

		Set_Mouse_Cursor(control->X, control->Y, Extract_Shape(MouseShapes, baseshp));
		CurrentMouseShape = mouse;
		IsSmall = wsmall;
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
 * INPUT:   input -- The player input code as returned from Keyboard->Get().                   *
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
	MouseStruct const * control = &MouseControl[CurrentMouseShape];

	if (control->FrameRate && Timer == 0) {

		Frame++;
		Frame %= control->FrameCount;
		Timer = control->FrameRate;

		if (!IsSmall || control->SmallFrame != -1) {
			int baseframe = (IsSmall) ? control->SmallFrame : control->StartFrame;
			if (baseframe == -1) baseframe = control->StartFrame;
			Set_Mouse_Cursor(control->X, control->Y, Extract_Shape(MouseShapes, baseframe + Frame));
		}
	}

	ScrollClass::AI(input, x, y);
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
	#ifndef NDEBUG
		RawFileClass file("MOUSE.SHP");
		if (file.Is_Available()) {
			MouseShapes = Load_Alloc_Data(file);
		} else {
			MouseShapes = MFCD::Retrieve("MOUSE.SHP");
		}
	#else
		MouseShapes = MFCD::Retrieve("MOUSE.SHP");
	#endif
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
//#ifdef WIN32
//#define	WD	45
//#define	HT	36
//#else
#define	WD	29
#define	HT	23
//#endif

MouseClass::MouseStruct MouseClass::MouseControl[MOUSE_COUNT] = {
	{0, 	1,		0,		80,	0,		0},		//	MOUSE_NORMAL
	{1, 	1,		0,		-1,	WD/2,	0},		//	MOUSE_N
	{2, 	1,		0,		-1,	WD,	0},		//	MOUSE_NE
	{3, 	1,		0,		-1,	WD,	HT/2},	//	MOUSE_E
	{4,	1,		0,		-1,	WD,	HT},		//	MOUSE_SE
	{5,	1,		0,		-1,	WD/2,	HT},		//	MOUSE_S
	{6,	1,		0,		-1,	0, 	HT},		//	MOUSE_SW
	{7,	1,		0,		-1,	0, 	HT/2},	//	MOUSE_W
	{8,	1,		0,		-1,	0, 	0},		//	MOUSE_NW

	{124, 1,		0,		-1,	WD/2,	0},		//	MOUSE_NO_N
	{125, 1,		0,		-1,	WD,	0},		//	MOUSE_NO_NE
	{126, 1,		0,		-1,	WD,	HT/2},	//	MOUSE_NO_E
	{127,	1,		0,		-1,	WD,	HT},		//	MOUSE_NO_SE
	{128,	1,		0,		-1,	WD/2,	HT},		//	MOUSE_NO_S
	{129,	1,		0,		-1,	0, 	HT},		//	MOUSE_NO_SW
	{130,	1,		0,		-1,	0, 	HT/2},	//	MOUSE_NO_W
	{131,	1,		0,		-1,	0, 	0},		//	MOUSE_NO_NW

	{14,	1,		0,		33,	WD/2,	HT/2},	//	MOUSE_NO_MOVE
	{10,	4,		4,		29,	WD/2,	HT/2},	//	MOUSE_CAN_MOVE
	{113,	3,		4,		142,	WD/2,	HT/2},	//	MOUSE_ENTER
	{59,	9,		4,		-1,	WD/2,	HT/2},	//	MOUSE_DEPLOY
	{15,	6,		4,		-1,	WD/2,	HT/2},	//	MOUSE_CAN_SELECT
	{21,	8,		4,		134,	WD/2,	HT/2},	//	MOUSE_CAN_ATTACK
	{68,	12,	2,		-1,	WD/2,	HT/2},	//	MOUSE_SELL_BACK
	{148,	12,	2,		-1,	WD/2,	HT/2},	//	MOUSE_SELL_UNIT
	{35,	24,	2,		-1,	WD/2,	HT/2},	//	MOUSE_REPAIR
	{120,	1,		0,		-1,	WD/2,	HT/2},	//	MOUSE_NO_REPAIR
	{119,	1,		0,		-1,	WD/2,	HT/2},	//	MOUSE_NO_SELL_BACK
	{81,	1,		0,		145,	WD/2, HT/2},	//	MOUSE_RADAR_CURSOR
	{90,	7,		4,		-1,	WD/2,	HT/2},	//	MOUSE_NUCLEAR_BOMB
	{82,	8,		2,		213,	WD/2,	HT/2},	//	MOUSE_AIR_STRIKE
	{116,	3,		4,		121,	WD/2,	HT/2},	//	MOUSE_DEMOLITIONS
	{147,	1,		0,		146,	WD/2,	HT/2},	//	MOUSE_AREA_GUARD
	{160,	4,		4,		194,	WD/2,	HT/2},	//	MOUSE_HEAL
	{164,	3,		4,		167,	WD/2,	HT/2},	//	MOUSE_DAMAGE
	{170,	24,	2,		-1,	WD/2,	HT/2},	//	MOUSE_GREPAIR
	{195,	8,		4,		203,	WD/2,	HT/2},	// MOUSE_STAY_ATTACK
	{211,	1,		0,		-1,	WD/2,	HT/2},	// MOUSE_NO_DEPLOY
	{212,	1,		0,		-1,	WD/2,	HT/2},	// MOUSE_NO_ENTER
	{213,	1,		0,		-1,	WD/2,	HT/2},	// MOUSE_NO_REPAIR

	{97,	8,		3,		-1,	WD/2,	HT/2},	//	MOUSE_CHRONO_SELECT
	{105,	8,		2,		-1,	WD/2,	HT/2},	//	MOUSE_CHRONO_DEST

};
