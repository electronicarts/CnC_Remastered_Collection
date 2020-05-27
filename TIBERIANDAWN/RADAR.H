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

/* $Header:   F:\projects\c&c\vcs\code\radar.h_v   2.17   16 Oct 1995 16:48:04   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RADAR.H                                                      *
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

#ifndef RADAR_H
#define RADAR_H

#include	"display.h"

class RadarClass: public DisplayClass
{
	public:
		int	RadX;
		int	RadOffX;
		int	RadY;
		int	RadOffY;
		int	RadWidth;
		int   RadHeight;
		int	RadIWidth;
		int	RadIHeight;
		int	RadPWidth;
		int	RadPHeight;

		RadarClass(void);

		/*
		** Initialization
		*/
		virtual void One_Time(void);							// One-time inits
		virtual void Init_Clear(void);						// Clears all to known state

		virtual bool Map_Cell(CELL cell, HouseClass * house, bool and_for_allies);
		virtual CELL Click_Cell_Calc(int x, int y);
		virtual void AI(KeyNumType &input, int x, int y);
		virtual void Draw_It(bool complete=false);
		virtual void Refresh_Cells(CELL cell, short const *list);
		virtual void Set_Map_Dimensions(int x, int y, int w, int h);
//		virtual void Set_Tactical_Position(int x, int y, int leptonx=0, int leptony=0);
//		virtual void Set_Tactical_Position(CELL cell);
		virtual void Set_Tactical_Position(COORDINATE coord);
		void	Zoom_Mode(CELL cell);
		int Click_In_Radar(int &x, int &y, bool change=false);
		void Cell_XY_To_Radar_Pixel(int cellx, int celly, int &x, int &y);

		void Set_Radar_Position(CELL cell);
		CELL Radar_Position(void);
		bool Radar_Activate(int control);
		void Plot_Radar_Pixel(CELL cell);
		void Radar_Pixel(CELL cell);
		void Coord_To_Radar_Pixel(COORDINATE coord, int &x, int &y);
		void Cursor_Cell(CELL cell, int value);
		void RadarClass::Mark_Radar(int x1, int y1, int x2, int y2, int value, int barlen);
		void Radar_Cursor(int forced = false);
		void Render_Terrain(CELL cell, int x, int y, int size);
		bool Cell_On_Radar(CELL cell);
		void Render_Infantry(CELL cell, int x, int y, int size);
		void Render_Overlay(CELL cell, int x, int y, int size);
		void Radar_Anim(void);
		bool Is_Radar_Active(void) {return IsRadarActive;};
		bool Is_Radar_Activating(void) {return IsRadarActivating;};
		bool Is_Radar_Existing(void) {return(DoesRadarExist);};

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		/*
		** Toggles player names on & off
		*/
		void Player_Names(bool on);
		int Is_Player_Names(void) {return IsPlayerNames;}
		void Draw_Names(void);
		int Is_Zoomed(void) {return IsZoomed;}

	protected:

		/*
		**	Radar map constant values.
		*/
		enum RadarClassEnums {
			RADAR_ACTIVATED_FRAME=22,
			MAX_RADAR_FRAMES = 41
		};

		/*
		**	If the radar map must be completely redrawn, then this flag will be true.
		**	Typical causes of this would be when the radar first appears, or when the
		**	screen has been damaged.
		*/
		unsigned IsToRedraw:1;
		unsigned RadarCursorRedraw:1;

		/*
		**	If the radar map is visible then this flag is true.
		*/
		unsigned DoesRadarExist:1;
		unsigned IsRadarActive:1;
		unsigned IsRadarActivating:1;
		unsigned IsRadarDeactivating:1;

		/*
		** Special radar frame is set when a new location is selected on the
		** radar map.  It counts down through the special radar cursors until
		** either the radar cursor becomes normal or the radar cursor is moved
		** again.
		*/
		unsigned SpecialRadarFrame:3;
		unsigned RadarAnimFrame:6;

		static void const * RadarAnim;

		/*
		**	This gadget class is used for capturing input to the tactical map. All mouse input
		**	will be routed through this gadget.
		*/
		class TacticalClass : public GadgetClass {
			public:
				TacticalClass(void) : GadgetClass(0,0,0,0,LEFTPRESS|LEFTRELEASE|LEFTHELD|LEFTUP|RIGHTPRESS,true) {};

			protected:
				virtual int Action(unsigned flags, KeyNumType & key);
			friend class RadarClass;
		};
		friend class TacticalClass;

		/*
		**	This is the "button" that tracks all input to the tactical map.
		** It must be available to derived classes, for Save/Load purposes.
		*/
		static TacticalClass RadarButton;

	private:

		/*
		**	The current radar position as the upper left corner cell for the
		**	radar map display. The width and height is controlled by the
		**	actual dimensions of the radar map display box (in pixels).
		*/
		unsigned RadarX;
		unsigned RadarY;
		unsigned RadarCell;

		/*
		**	This is the origin (pixel offsets) for the upper left corner
		**	of the radar map within the full radar map area of the screen.
		**	This is biased so that the radar map, when smaller than full
		**	size will appear centered.
		*/
		unsigned BaseX;
		unsigned BaseY;

		unsigned RadarWidth;
		unsigned RadarCellWidth;
		unsigned RadarHeight;
		unsigned RadarCellHeight;

		/*
		**	If the radar map is in zoom mode, then this value will be true.
		*/
		unsigned IsZoomed:1;

		/*
		** This flag is true if the radar map is in its special show-the-player
		** names mode.
		*/
		unsigned IsPlayerNames:1;

		/*
		**	This is the list of radar pixels that need to be updated. Only a partial
		**	list is maintained for maximum speed.
		*/
		unsigned PixelPtr;
		int ZoomFactor;
		enum PixelStackEnums {PIXELSTACK=200};
		CELL PixelStack[PIXELSTACK];
};


#endif