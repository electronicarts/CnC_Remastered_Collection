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

/* $Header: /CounterStrike/POWER.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : POWER.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/15/94                                                     *
 *                                                                                             *
 *                  Last Update : December 15, 1994 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef POWER_H
#define POWER_H

#include	"radar.h"

class PowerClass : public RadarClass
{
	public:
		PowerClass(void);
		PowerClass(NoInitClass const & x) : RadarClass(x), FlashTimer(x) {};

		/*
		** Initialization
		*/
		virtual void One_Time(void);							// One-time inits

		virtual void Init_Clear(void);						// Clears all to known state
		virtual void Draw_It(bool complete=false);
		virtual void AI(KeyNumType &input, int x, int y);
		virtual void Refresh_Cells(CELL cell, short const *list);
		void Flash_Power(void);

		unsigned IsToRedraw:1;

	protected:
		/*
		**	This gadget is used to capture mouse input on the power bar.
		*/
		class PowerButtonClass : public GadgetClass {
			public:
				PowerButtonClass(void) : GadgetClass(0,0,0,0,LEFTPRESS|LEFTRELEASE|LEFTHELD|LEFTUP|RIGHTPRESS,true) {};

			protected:
				virtual int Action(unsigned flags, KeyNumType & key);
			friend class PowerClass;
		};

		/*
		**	This is the "button" that tracks all input to the tactical map.
		** It must be available to derived classes, for Save/Load purposes.
		*/
		static PowerButtonClass PowerButton;

		enum PowerEnums {
			POWER_X=10*ICON_PIXEL_W,
#ifdef WIN32
			POWER_Y= (7+70+13),
			POWER_HEIGHT=(200-(7+70+13)),
#else
			POWER_Y= (88+9),
			POWER_HEIGHT=80,
#endif
			POWER_WIDTH=8,
			POWER_LINE_SPACE=5,
			POWER_LINE_WIDTH=3,
			POWER_STEP_LEVEL=100,
			POWER_STEP_FACTOR=5
		};

	private:
		int Power_Height(int value);

		unsigned IsActive:1;

		/*
		**	If the power bar should be rendered with some flash effect then
		**	this specifies the duration that the flash will occur.
		*/
		CDTimerClass<FrameTimerClass> FlashTimer;

		int RecordedDrain;
		int RecordedPower;
		int DesiredDrainHeight;
		int DesiredPowerHeight;
		int DrainHeight;
		int PowerHeight;
		int DrainBounce;
		int PowerBounce;
		short PowerDir;
		short DrainDir;

		/*
		**	Points to the shape to use for the "desired" power level indicator.
		*/
		static void const * PowerShape;
		static void const * PowerBarShape;
};

#endif
