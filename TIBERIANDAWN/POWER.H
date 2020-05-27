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

/* $Header:   F:\projects\c&c\vcs\code\power.h_v   2.16   16 Oct 1995 16:48:06   JOE_BOSTIC  $ */
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
		int PowX;
		int PowY;
		int PowWidth;
		int PowHeight;
		int PowLineSpace;
		int PowLineWidth;


		PowerClass();

		/*
		** Initialization
		*/
		virtual void One_Time(void);							// One-time inits

		virtual void Init_Clear(void);						// Clears all to known state
		virtual void Draw_It(bool complete=false);
		virtual void AI(KeyNumType &input, int x, int y);
		virtual void Refresh_Cells(CELL cell, short const *list);
//		virtual void Must_Redraw_Sidebar(void);

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

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
			POWER_STEP_LEVEL=100,
			POWER_STEP_FACTOR=6,
		};

	private:
		int  Power_Height(int value);

		unsigned IsActive:1;

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

		/*
		** Points to the shapes to be used for drawing the power bar
		*/
		static void const * PowerBarShape;
};

#endif
