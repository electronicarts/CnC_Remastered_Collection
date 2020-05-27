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

/* $Header:   F:\projects\c&c\vcs\code\dial8.h_v   2.18   16 Oct 1995 16:47:28   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : DIAL8.H                                                      * 
 *                                                                                             * 
 *                   Programmer : Bill Randolph																  *
 *                                                                                             * 
 *                   Start Date : 02/06/95                                                     * 
 *                                                                                             * 
 *                  Last Update : February 6, 1995 [BR]													  *
 *                                                                                             * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef DIAL8_H
#define DIAL8_H

class Dial8Class : public ControlClass
{
	public:
		/*
		** Constructor/Destructor
		*/
		Dial8Class(int id, int x, int y, int w, int h, DirType dir);

		/*
		** Get/Set the direction the dial is currently pointing
		*/
		DirType Get_Direction(void) const;
		void Set_Direction(DirType dir);

		/*
		** Overloaded draw routine
		*/
		virtual int Draw_Me(int forced = false);

	protected:
		/*
		** Overloaded event processing routine
		*/
		virtual int Action(unsigned flags, KeyNumType &key);

	private:
		int FaceX;						// x-coord of center of face
		int FaceY;						// y-coord of center of face
		int FacePoint[8][2];			// coords of the little dial decorations
		int FaceLine[8][2];			// coords for drawing the dial hand
		DirType Direction;			// 0-255 numerical direction of dial
		FacingType Facing;			// numerical facing direction of dial (0 - 7)
		FacingType OldFacing;		// previous Facing value

};

#endif

