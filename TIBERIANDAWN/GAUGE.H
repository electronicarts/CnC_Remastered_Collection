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

/* $Header:   F:\projects\c&c\vcs\code\gauge.h_v   2.17   16 Oct 1995 16:45:24   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : GAUGE.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic, Maria del Mar McCready Legg                   *
 *                                                                                             *
 *                   Start Date : 01/15/95                                                     *
 *                                                                                             *
 *                  Last Update : January 15, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef GAUGE_H
#define GAUGE_H

class GaugeClass : public ControlClass
{
	public:

		GaugeClass(unsigned id, int x, int y, int w, int h);
//		static GaugeClass * Create_One_Of(unsigned id, int x, int y, int w, int h);

		virtual int Draw_Me(int forced=false);
		virtual int Set_Maximum(int value);
		virtual int Set_Value(int value);
		virtual int Get_Value(void) const {return (CurValue);};
		virtual void Use_Thumb(int value) { HasThumb = value ? true : false; };

		virtual int Thumb_Pixels(void) { return (8);}

		/*
		**	If this gauge has a color to the left of the current setting, then this
		**	flag will be true.
		*/
		unsigned IsColorized:1;

	protected:

		/*
		**	If a thumb is desired, set to true.
		*/
		unsigned HasThumb:1;

		/*
		**	Is this a horizontal slider?
		*/
		unsigned IsHorizontal:1;

		int MaxValue;				// maximum value (in application units)
		int CurValue;				// index of 1st displayed string in box
										//  (in application units)

		/*
		** This value records the difference between where the user clicked
		** and the edge of the thumb, so that the thumb follows the mouse
		** with the proper offset.
		*/
		int ClickDiff;

	protected:
		virtual void Draw_Thumb(void);
		virtual int  Action(unsigned flags, KeyNumType &key);
		virtual int  Pixel_To_Value(int pixel);
		virtual int  Value_To_Pixel(int value);
};



class TriColorGaugeClass : public GaugeClass
{
	public:
		TriColorGaugeClass(unsigned id, int x, int y, int w, int h);
//		static TriColorGaugeClass * Create_One_Of(unsigned id, int x, int y, int w, int h);
		virtual int Draw_Me(int forced);
		virtual int Set_Red_Limit(int value);
		virtual int Set_Yellow_Limit(int value);

	protected:
		int RedLimit;				// maximum value for red
		int YellowLimit;			// maximum value for yellow
};




#endif
