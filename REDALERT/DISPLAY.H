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

/* $Header: /CounterStrike/DISPLAY.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DISPLAY.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 1, 1994                                                  *
 *                                                                                             *
 *                  Last Update : May 1, 1994   [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef DISPLAY_H
#define DISPLAY_H

#include	"map.h"
#include	"layer.h"


#define	ICON_PIXEL_W	 		24
#define	ICON_PIXEL_H			24
#define	ICON_LEPTON_W			256
#define	ICON_LEPTON_H			256
#define	CELL_PIXEL_W	 		ICON_PIXEL_W
#define	CELL_PIXEL_H			ICON_PIXEL_H
#define	CELL_LEPTON_W			ICON_LEPTON_W
#define	CELL_LEPTON_H			ICON_LEPTON_H

//	-----------------------------------------------------------
#define	PIXEL_LEPTON_W			(ICON_LEPTON_W/ICON_PIXEL_W)
#define	PIXEL_LEPTON_H			(ICON_LEPTON_H/ICON_PIXEL_H)

#define	SIDE_BAR_TAC_WIDTH	10
#define  SIDE_BAR_TAC_HEIGHT	8

extern COORDINATE Coord_Add(COORDINATE coord1, COORDINATE coord2);

class DisplayClass: public MapClass
{
	public:
		friend class DLLExportClass;	// ST - 5/13/2019

		/*
		** The tactical map display position is indicated by the cell of the
		**	upper left hand corner. These should not be altered directly. Use
		**	the Set_Tactical_Position function instead.
		*/
		COORDINATE TacticalCoord;

		/*
		**	The dimensions (in cells) of the visible window onto the game map. This tactical
		**	map is how the player interacts and views the game world.
		*/
		LEPTON TacLeptonWidth;
		LEPTON TacLeptonHeight;

		/*
		**	These layer control elements are used to group the displayable objects
		**	so that proper overlap can be obtained.
		*/
		static LayerClass Layer[LAYER_COUNT];

		/*
		**	This records the position and shape of a placement cursor to display
		**	over the map. This cursor is used when placing buildings and also used
		**	extensively by the scenario editor.
		*/
		CELL ZoneCell;
		short ZoneOffset;
		short const *CursorSize;
		short CursorShapeSave[256];	// For save/load
		bool ProximityCheck;				// Is proximity check ok?

		/*
		** This holds the building type that is about to be placed upon the map.
		**	It is only valid during the building placement state. The PendingLegal
		**	flag is updated as the cursor moves and it reflects the legality of
		**	placing the building at the desired location.
		*/
		ObjectClass * PendingObjectPtr;
		ObjectTypeClass const * PendingObject;
		HousesType PendingHouse;

		static unsigned char FadingBrighten[256];
		static unsigned char FadingShade[256];
		static unsigned char FadingWayDark[256];
		static unsigned char FadingLight[256];
		static unsigned char FadingGreen[256];
		static unsigned char FadingYellow[256];
		static unsigned char FadingRed[256];
		static unsigned char TranslucentTable[(MAGIC_COL_COUNT+1)*256];
		static unsigned char WhiteTranslucentTable[(1+1)*256];
		static unsigned char MouseTranslucentTable[(4+1)*256];
		static void const *TransIconset;
		static unsigned char UnitShadow[(USHADOW_COL_COUNT+1)*256];
		static unsigned char UnitShadowAir[(USHADOW_COL_COUNT+1)*256];
		static unsigned char SpecialGhost[2*256];

		//-------------------------------------------------------------------------
		DisplayClass(void);
		DisplayClass(NoInitClass const & x) : MapClass(x) {};

		virtual void Read_INI(CCINIClass & ini);
		void Write_INI(CCINIClass & ini);

		/*
		** Initialization
		*/
		virtual void One_Time(void);							// One-time inits
		virtual void Init_Clear(void);						// Clears all to known state
		virtual void Init_IO(void);							// Inits button list
		virtual void Init_Theater(TheaterType theater);	// Theater-specific inits

		/*
		**	General display/map/interface support functionality.
		*/
		virtual void AI(KeyNumType &input, int x, int y);
		virtual void Draw_It(bool complete=false);

		/*
		**	Added functionality.
		*/
		void All_To_Look(HouseClass *house, bool units_only=false);										// Added house parameter so it can work for multiple players. ST - 8/6/2019 2:30PM
		void Constrained_Look(COORDINATE coord, LEPTON distance, HouseClass *house);				// Added house parameter for client/server multiplayer. ST - 8/12/2019 3:25PM
		void Shroud_Cell(CELL cell, HouseClass *house);
		void Encroach_Shadow(HouseClass *house);
		COORDINATE Center_Map(COORDINATE center=0L);
		virtual bool Map_Cell(CELL cell, HouseClass *house, bool check_radar_spied = true, bool and_for_allies = true);		// Added check_radar_spied parameter to prevent recursion. ST - 8/6/2019 10:16AM. Added and_for_allies ST - 10/31/2019 1:18PM
		virtual CELL Click_Cell_Calc(int x, int y) const;
		virtual void Help_Text(int , int =-1, int =-1, int =YELLOW, bool =false) {};
		virtual MouseType Get_Mouse_Shape(void) const = 0;
		virtual bool Scroll_Map(DirType facing, int & distance, bool really);
		virtual void Refresh_Cells(CELL cell, short const *list);
		virtual void Set_View_Dimensions(int x, int y, int width=-1, int height=-1);

		/*
		**	Pending object placement control.
		*/
		virtual void Put_Place_Back(TechnoClass * ) {}; // Affects 'pending' system.
		void Cursor_Mark(CELL pos, bool on);
		void Set_Cursor_Shape(short const * list);
		CELL Set_Cursor_Pos(CELL pos = -1);
		void Get_Occupy_Dimensions(int & w, int & h, short const *list) const;

		/*
		**	Tactical map only functionality.
		*/
		virtual void Set_Tactical_Position(COORDINATE coord);
		void Refresh_Band(void);
		void Select_These(COORDINATE coord1, COORDINATE coord2, bool additive = false);
		COORDINATE Pixel_To_Coord(int x, int y) const;
		bool Coord_To_Pixel(COORDINATE coord, int & x, int & y) const;
		bool Push_Onto_TacMap(COORDINATE &source, COORDINATE &dest);
		void Remove(ObjectClass const * object, LayerType layer);
		void Submit(ObjectClass const * object, LayerType layer);
		CELL Calculated_Cell(SourceType dir, WAYPOINT waypoint=-1, CELL cell=-1, SpeedType loco=SPEED_FOOT, bool zonecheck=true, MZoneType mzone=MZONE_NORMAL) const;
		bool In_View(register CELL cell) const;
		bool Passes_Proximity_Check(ObjectTypeClass const * object, HousesType house, short const * list, CELL trycell) const;
		ObjectClass * Cell_Object(CELL cell, int x=0, int y=0) const;
		ObjectClass * Next_Object(ObjectClass * object) const;
		ObjectClass * Prev_Object(ObjectClass * object) const;
		int Cell_Shadow(CELL cell, HouseClass *house) const;
		short const * Text_Overlap_List(char const * text, int x, int y) const;
		bool Is_Spot_Free(COORDINATE coord) const;
		COORDINATE Closest_Free_Spot(COORDINATE coord, bool any=false) const;
		void Sell_Mode_Control(int control);
		void Repair_Mode_Control(int control);

		virtual void Flag_Cell(CELL cell);
		bool Is_Cell_Flagged(CELL cell) const {return CellRedraw.Is_True(cell);};

		/*
		** Computes starting position based on player's units' Coords.
		*/
		void Compute_Start_Pos(long& x, long& y);

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

	protected:
		virtual void Mouse_Right_Press(void);
		virtual void Mouse_Left_Press(int x, int y);
		virtual void Mouse_Left_Up(CELL cell, bool shadow, ObjectClass * object, ActionType action, bool wsmall = false);
		virtual void Mouse_Left_Held(int x, int y);
		virtual void Mouse_Left_Release(CELL cell, int x, int y, ObjectClass * object, ActionType action, bool wsmall = false);

	public:
		/*
		**	This is the pixel offset for the upper left corner of the tactical map.
		*/
		int TacPixelX;
		int TacPixelY;

		/*
		**	This is the coordinate that the tactical map should be in at next available opportunity.
		*/
		COORDINATE DesiredTacticalCoord;

		/*
		**	If something in the tactical map is to be redrawn, this flag is set to true.
		*/
		unsigned IsToRedraw:1;

		/*
		**	If the player is currently wielding a wrench (to select buildings for repair),
		**	then this flag is true. In such a state, normal movement and combat orders
		**	are preempted.
		*/
		unsigned IsRepairMode:1;

		/*
		**	If the player is currently in "sell back" mode, then this flag will be
		**	true. While in this mode, anything clicked on will be sold back to the
		**	"factory".
		*/
		unsigned IsSellMode:1;

		/*
		**	If the player is currently in ion cannon targeting mode, then this
		** flag will be true.  While in this mode, anything clicked on will be
		** be destroyed by the ION cannon.
		*/
		SpecialWeaponType IsTargettingMode;

	protected:

		/*
		**	If it is currently in rubber band mode (multi unit selection), then this
		**	flag will be true. While in such a mode, normal input is preempted while
		**	the extended selection is in progress.
		*/
		unsigned IsRubberBand:1;

		/*
		**	The moment the mouse is held down, this flag gets set. If the mouse is dragged
		**	a sufficient distance while held down, then true rubber band mode selection
		**	can begin. Using a minimum distance prevents accidental rubber band selection
		**	mode from being initiated.
		*/
		unsigned IsTentative:1;

		/*
		**	This gadget class is used for capturing input to the tactical map. All mouse input
		**	will be routed through this gadget.
		*/
		class TacticalClass : public GadgetClass {
			public:
				TacticalClass(void) : GadgetClass(0,0,0,0,LEFTPRESS|LEFTRELEASE|LEFTHELD|LEFTUP|RIGHTPRESS,true) {};

				int Selection_At_Mouse(unsigned flags, KeyNumType & key);
				int Command_Object(unsigned flags, KeyNumType & key);

			protected:
				virtual int Action(unsigned flags, KeyNumType & key);
		};
		friend class TacticalClass;

		/*
		**	This is the "button" that tracks all input to the tactical map.
		** It must be available to derived classes, for Save/Load purposes.
		*/
public:		//ST - 1/21/2019 11:59AM
		static TacticalClass TacButton;

	private:

		/*
		**	This is a utility flag that is set during the icon draw process only if there
		**	was at least one shadow icon detected that should be redrawn. When the shadow
		**	drawing logic is to take place, but this flag is false, then the shadow drawing
		**	will be skipped since it would perform no function.
		*/
		unsigned IsShadowPresent:1;

		/*
		**	Rubber band mode consists of stretching a box from the anchor point (specified
		**	here) to the current cursor position.
		*/
		int BandX,BandY;
		int NewX,NewY;

		static void const *ShadowShapes;
		static unsigned char ShadowTrans[(SHADOW_COL_COUNT+1)*256];

		void Redraw_Icons(void);
		void Redraw_OIcons(void);
		void Redraw_Shadow(void);

		/*
		**	This bit array is used to flag cells to be redrawn. If the icon needs to
		**	be redrawn for a cell, then the corresponding flag will be true.
		*/
		static BooleanVectorClass CellRedraw;

		bool Good_Reinforcement_Cell(CELL outcell, CELL incell, SpeedType loco, int zone, MZoneType mzone) const;

		//
		// We need a way to bypass visible view checks when we are running in the context of GlyphX without using the
		// internal C&C renderer. We shouldn't know or care what the user is actually looking at
		// ST - 4/17/2019 9:01AM
		//
		static bool IgnoreViewConstraints;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[1024];

};


#endif