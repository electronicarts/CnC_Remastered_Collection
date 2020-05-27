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

/* $Header:   F:\projects\c&c\vcs\code\control.h_v   2.18   16 Oct 1995 16:46:08   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : CONTROL.H                                                    * 
 *                                                                                             * 
 *                   Programmer : Joe L. Bostic                                                * 
 *                                                                                             * 
 *                   Start Date : 01/15/95                                                     * 
 *                                                                                             * 
 *                  Last Update : January 15, 1995 [JLB]                                       * 
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CONTROL_H
#define CONTROL_H

#include	"gadget.h"

/***************************************************************************
 * ControlClass -- Region tracking class									         *
 *                                                                         *
 * INPUT:      int x -- x position of gadget											*
 *					int y -- y position of gadget											*
 *					int w -- width of gadget												*							
 *					int h -- height of gadget												*							
 *					UWORD flags -- see enumeration choices								*							
 *                                                                         *
 * OUTPUT:     0 = new scenario created, -1 = not									*
 * WARNINGS:	This class is Abstract (cannot make an instance of it)      *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/03/1995 MML : Created.                                             *
 *=========================================================================*/
class ControlClass : public GadgetClass
{
	public:
		ControlClass(unsigned id, int x, int y, int w, int h, unsigned flags=LEFTPRESS|RIGHTPRESS, int sticky=false);
//		static ControlClass * Create_One_Of(unsigned id, int x, int y, int w, int h, unsigned flags=LEFTPRESS|RIGHTPRESS, int sticky=false);

		virtual void Make_Peer(GadgetClass & gadget);

		/*
		**	Render support function.
		*/
		virtual int Draw_Me(int forced=false);

		/*
		**	This is the ID number for this control gadget. This number is used to generate
		**	a special pseudo-key when the gadget detects valid input.
		*/
		unsigned ID;

	protected:
		virtual unsigned Get_ID(void) const;
		virtual int Action(unsigned flags, KeyNumType & key);

		/*
		**	This points to the peer button to inform when something happens to this
		**	gadget.
		*/
		GadgetClass * Peer;
};

#endif

