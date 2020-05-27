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

/* $Header: /CounterStrike/RADAR.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
		RadarClass(void);
		RadarClass(NoInitClass const & x) : DisplayClass(x) {};

		/*
		**	The dimensions and coordinates of the radar map.
		*/
		int RadX;
		int RadOffX;
		int RadY;
		int RadOffY;
		int RadWidth;
		int RadHeight;
		int RadIWidth;
		int RadIHeight;
		int RadPWidth;
		int RadPHeight;

		/*
		** Initialization
		*/
		virtual void One_Time(void);							// One-time inits
		virtual void Init_Clear(void);						// Clears all to known state

		virtual void Flag_Cell(CELL cell);
		virtual bool Map_Cell(CELL cell, HouseClass *house, bool check_radar_spied = true, bool and_for_allies = true);		// Added check_radar_spied parameter to prevent recursion. ST - 8/6/2019 10:16AM. Added and_for_allies ST - 10/31/2019 1:18PM
		virtual bool Jam_Cell(CELL cell, HouseClass * house);
		virtual bool UnJam_Cell(CELL cell, HouseClass * house);
		virtual CELL Click_Cell_Calc(int x, int y) const;
		virtual void AI(KeyNumType &input, int x, int y);
		virtual void Draw_It(bool complete=false);
		virtual void Refresh_Cells(CELL cell, short const *list);
		virtual void Set_Map_Dimensions(int x, int y, int w, int h);
		virtual void Set_Tactical_Position(COORDINATE coord);
		void	Zoom_Mode(CELL cell);
		int Click_In_Radar(int &x, int &y, bool change=false) const;
		void Cell_XY_To_Radar_Pixel(int cellx, int celly, int &x, int &y);

		bool Is_Zoomable(void) const;
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
		bool Is_Radar_Active(void);
		bool Is_Radar_Activating(void);
		bool Is_Radar_Existing(void);

		/*
		** Toggles player names on & off
		*/
		void Player_Names(bool on);
		int Is_Player_Names(void) {return IsPlayerNames;}
		bool Spying_On_House(void) {return IsHouseSpy;}
		void Draw_Names(void);
		bool Draw_House_Info(void);
		int Is_Zoomed(void) {return IsZoomed;}
		bool Get_Jammed(HousesType house) const;
		bool Get_Jammed(HouseClass *player) const;
		void Set_Jammed(HousesType house, bool jam);
		void Set_Jammed(HouseClass *player, bool jam);
		bool Spy_Next_House(void);
		void Activate_Pulse(void);

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
		**	Per-player radar jammed flag.
		*/
		unsigned int IsRadarJammedByPlayerMask;

		/*
		** Flag to tell whether sonar pulse should be displayed on radar map
		*/
		unsigned IsPulseActive:1;
		int	RadarPulseFrame;

		/*
		** Special radar frame is set when a new location is selected on the
		** radar map.  It counts down through the special radar cursors until
		** either the radar cursor becomes normal or the radar cursor is moved
		** again.
		*/
		int SpecialRadarFrame;
		int RadarAnimFrame;

		static void const * RadarAnim;
		static void const * RadarPulse;
		static void const * RadarFrame;

		/*
		**	This gadget class is used for capturing input to the tactical map. All mouse input
		**	will be routed through this gadget.
		*/
		class RTacticalClass : public GadgetClass {
			public:
				RTacticalClass(void) : GadgetClass(0,0,0,0,LEFTPRESS|LEFTRELEASE|LEFTHELD|LEFTUP|RIGHTPRESS,true) {};

			protected:
				virtual int Action(unsigned flags, KeyNumType & key);
			friend class RadarClass;
		};
		friend class RTacticalClass;

		/*
		**	This is the "button" that tracks all input to the tactical map.
		** It must be available to derived classes, for Save/Load purposes.
		*/
		static RTacticalClass RadarButton;

	private:

		/*
		**	The current radar position as the upper left corner cell for the
		**	radar map display. The width and height is controlled by the
		**	actual dimensions of the radar map display box (in pixels).
		*/
		unsigned RadarX;
		unsigned RadarY;
		unsigned RadarCellWidth;
		unsigned RadarCellHeight;
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
		unsigned RadarHeight;

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
		** This flag is true if the radar map is in its special show-the-units
		** of-another-house mode.
		*/
		unsigned IsHouseSpy:1;

		/*
		**	This is the zoom factor to use. This value is the number of pixels wide
		**	each cell will occupy on the radar map. Completely zoomed out would be a
		**	value of 1.
		*/
		int ZoomFactor;

		/*
		** If we're spying on a house's radar facility, this field shows the
		** name of the house we're spying on.
		*/
		HousesType SpyingOn;

		/*
		**	This is the list of radar pixels that need to be updated. Only a partial
		**	list is maintained for maximum speed.
		*/
		unsigned PixelPtr;
		enum PixelStackEnums {PIXELSTACK=400};
		CELL PixelStack[PIXELSTACK];
};


#endif
