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

/* $Header:   F:\projects\c&c\vcs\code\display.h_v   2.15   16 Oct 1995 16:47:42   JOE_BOSTIC  $ */
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


extern COORDINATE Coord_Add(COORDINATE coord1, COORDINATE coord2);

class DisplayClass: public MapClass
{
	// Need access to shadow shapes
	friend class DLLExportClass;
	
	public:

		/*
		**	This indicates the theater that the display is to represent.
		*/
		TheaterType Theater;

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
		int TacLeptonWidth;
		int TacLeptonHeight;

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
		static unsigned char FadingLight[256];
		static unsigned char RemapTables[HOUSE_COUNT][3][256];
		static unsigned char FadingGreen[256];
		static unsigned char FadingYellow[256];
		static unsigned char FadingRed[256];
		static unsigned char TranslucentTable[(MAGIC_COL_COUNT+1)*256];
		static unsigned char WhiteTranslucentTable[(1+1)*256];
		static unsigned char MouseTranslucentTable[(4+1)*256];
		static void const *TransIconset;
		static unsigned char UnitShadow[(USHADOW_COL_COUNT+1)*256];
		static unsigned char SpecialGhost[2*256];

		//-------------------------------------------------------------------------
		DisplayClass(void);

		virtual void Read_INI(char *buffer);
		void Write_INI(char *buffer);

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
		COORDINATE  Center_Map(void);
		virtual bool Map_Cell(CELL cell, HouseClass *house, bool and_for_allies);

		virtual CELL Click_Cell_Calc(int x, int y);
		virtual void Help_Text(int , int =-1, int =-1, int =YELLOW, bool =false, int =0) {};
		virtual MouseType Get_Mouse_Shape(void) const = 0;
		virtual bool Scroll_Map(DirType facing, int & distance, bool really);
		virtual void Refresh_Cells(CELL cell, short const *list);
		virtual void Set_View_Dimensions(int x, int y, int width=-1, int height=-1);

		/*
		**	Pending object placement control.
		*/
		virtual void Put_Place_Back(TechnoClass * ) {}; // Affects 'pending' system.
		void  Cursor_Mark(CELL pos, bool on);
		void  Set_Cursor_Shape(short const * list);
		CELL  Set_Cursor_Pos(CELL pos = -1);
		void  Get_Occupy_Dimensions(int & w, int & h, short const *list);

		/*
		**	Tactical map only functionality.
		*/
		virtual void Set_Tactical_Position(COORDINATE coord);
		void  Refresh_Band(void);
		void  Select_These(COORDINATE coord1, COORDINATE coord2, bool additive = false);
		COORDINATE  Pixel_To_Coord(int x, int y);
		bool  Coord_To_Pixel(COORDINATE coord, int &x, int &y);
		bool  Push_Onto_TacMap(COORDINATE &source, COORDINATE &dest);
		void  Remove(ObjectClass const *object, LayerType layer);
		void  Submit(ObjectClass const *object, LayerType layer);
		CELL  Calculated_Cell(SourceType dir, HousesType house);
		bool  In_View(register CELL cell);
		bool  Passes_Proximity_Check(ObjectTypeClass const *object);
#ifdef USE_RA_AI
		bool Passes_Proximity_Check(ObjectTypeClass const * object, HousesType house, short const * list, CELL trycell) const;
#endif
		ObjectClass *  Cell_Object(CELL cell, int x=0, int y=0);
		ObjectClass *  Next_Object(ObjectClass * object);
		ObjectClass *  Prev_Object(ObjectClass * object);
		int  Cell_Shadow(CELL cell, HouseClass *house);
		short const *  Text_Overlap_List(char const * text, int x, int y, int lines = 1);
		bool  Is_Spot_Free(COORDINATE coord) const;
		COORDINATE  Closest_Free_Spot(COORDINATE coord, bool any=false) const;
		void  Sell_Mode_Control(int control);
		void  Repair_Mode_Control(int control);

		void Flag_Cell(CELL cell) {
			Flag_To_Redraw(false);
			IsToRedraw = true;
			CellRedraw[cell] = true;
		};
		bool  Is_Cell_Flagged(CELL cell) const {return CellRedraw.Is_True(cell);};

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
		virtual void Mouse_Left_Up(bool shadow, ObjectClass * object, ActionType action, bool wwsmall = false);
		virtual void Mouse_Left_Held(int x, int y);
		virtual void Mouse_Left_Release(CELL cell, int x, int y, ObjectClass * object, ActionType action, bool wwsmall = false);

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
		**	If the player is currently in ion cannon targetting mode, then this
		** flag will be true.  While in this mode, anything clicked on will be
		** be destroyed by the ION cannon.
		*/
		unsigned IsTargettingMode:2;

	protected:

		/*
		**	If it is currently in rubber band mode (multi unit selection), then this
		**	flag will be true. While in such a mode, normal input is prempted while
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

		void Redraw_Icons(int draw_flags=0);
		void Redraw_Shadow(void);
		void Redraw_Shadow_Rects(void);

		/*
		**	This bit array is used to flag cells to be redrawn. If the icon needs to
		**	be redrawn for a cell, then the corresponding flag will be true.
		*/
		static BooleanVectorClass CellRedraw;

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