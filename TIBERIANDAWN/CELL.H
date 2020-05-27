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

/* $Header:   F:\projects\c&c\vcs\code\cell.h_v   2.20   16 Oct 1995 16:45:36   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CELL.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 29, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 29, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CELL_H
#define CELL_H

#include	"building.h"
#include	"unit.h"
#include	"template.h"


/****************************************************************************
**	Each cell on the map is controlled by the following structure.
*/
class CellClass
{
	public:
		/*
		**	Does this cell need to be updated on the radar map?  If something changes in the cell
		**	that might change the radar map imagery, then this flag will be set. It gets cleared
		**	when the cell graphic is updated to the radar map.
		*/
		unsigned IsPlot:1;

		/*
		**	Does this cell contain the special placement cursor graphic?  This graphic is
		**	present when selecting a site for building placement.
		*/
		unsigned IsCursorHere:1;

		/*
		**	Is this cell mapped by the player?  A mapped cell is visible. An unmapped cell
		**	is covered in a dark shroud. In addition to visibility, mapped cells are the only
		**	legal place for transports to land.
		*/
		unsigned IsMapped:1;

		/*
		**	If any part of this cell is visible (even just peeking out from under the shadow),
		**	this this flag will be true. Mapped cells always have this flag set, but unmapped
		**	cells might not -- it depends on where the shadow edge is located.
		*/
		unsigned IsVisible:1;

		/*
		** Every cell can be assigned a trigger. The same trigger can be assigned to
		** multiple cells. This bitflag indicates whether this cell has a trigger.
		** The trigger pointers for all cells must be stored elsewhere.
		*/
		unsigned IsTrigger:1;

		/*
		** Every cell can be assigned a waypoint.  A waypoint can only be assigned
		** to one cell, and vice-versa.  This bit simply indicates whether this
		** cell is assigned a waypoint or not.
		*/
		unsigned IsWaypoint:1;

		/*
		** Is this cell currently under the radar map cursor?  If so then it
		**   needs to be updated whenever the map is updated.
		*/
		unsigned IsRadarCursor:1;

		/*
		**	If this cell contains a house flag, then this will be true. The actual house
		**	flag it contains is specified by the Owner field.
		*/
		unsigned IsFlagged:1;

		/*
		**	This contains the icon number and set to use for the base
		**	of the terrain. All rendering on an icon occurs AFTER the icon
		**	specified by this element is rendered. It is the lowest of the low.
		*/
		TemplateType TType;
		unsigned char TIcon;

		/*
		**	The second layer of 'terrain' icons is represented by a simple
		**	type number and a value byte. This is sufficient for handling
		**	concrete and walls.
		*/
		OverlayType Overlay;
		unsigned char OverlayData;

		/*
		**	This is used to specify any special 'stain' overlay icon. This
		**	typically includes infantry bodies or other temporary marks.
		*/
		SmudgeType Smudge;
		unsigned char SmudgeData;

		/*
		**	Smudges and walls need to record ownership values. For walls, this
		**	allows adjacent building placement logic to work. For smudges, it
		**	allows building over smudges that are no longer attached to buildings
		**	in addition to fixing the adjacent placement logic.
		*/
		HousesType Owner;

		/*
		** This flag tells you what type of infantry currently occupy the
		** cell or are moving into it.
		*/
		HousesType InfType;

		/*
		**	These point to the object(s) that are located in this cell or overlap
		**	this cell.
		*/
		ObjectClass *OccupierPtr;
		ObjectClass *Overlapper[3];


		/*
		**	Per-player view of whether a cell is mapped. One bit for each house type. ST - 3/5/2019 3:00PM
		*/
		unsigned int IsMappedByPlayerMask;

		/*
		**	Per-player view of whether a cell is visible. One bit for each house type. ST - 3/5/2019 3:00PM
		*/
		unsigned int IsVisibleByPlayerMask;


		/*
		**	This array of bit flags is used to indicate which sub positions
		**	within the cell are either occupied or are soon going to be
		**	occupied. For vehicles, the cells that the vehicle is passing over
		**	will be flagged with the vehicle bit. For infantry, the the sub
		**	position the infantry is stopped at or headed toward will be marked.
		**	The sub positions it passes over will NOT be marked.
		*/
		union {
			struct {
				unsigned Center:1;
				unsigned NW:1;
				unsigned NE:1;
				unsigned SW:1;
				unsigned SE:1;
				unsigned Vehicle:1;		// Reserved for vehicle occupation.
				unsigned Monolith:1;		// Some immovable blockage is in cell.
				unsigned Building:1;		// A building of some time (usually blocks movement).
			} Occupy;
			unsigned char Composite;
		} Flag;

		//----------------------------------------------------------------
		CellClass(void);
		~CellClass(void) {};

		int operator == (CellClass const & cell) const {return &cell == this;};

		/*
		**	Query functions.
		*/
		ObjectClass *  Cell_Occupier(void) const;
		static int  Spot_Index(COORDINATE coord);
		bool  Is_Spot_Free(int spot_index) const {return (! (Flag.Composite & (1 << spot_index)) ); }
		COORDINATE  Closest_Free_Spot(COORDINATE coord, bool any=false) const;
		COORDINATE  Free_Spot(void) const {return Closest_Free_Spot(Cell_Coord());};
		bool  Is_Generally_Clear(bool ignore_cloaked = false) const;
		TARGET  As_Target(void) const {return ::As_Target(Cell_Number());};
		BuildingClass *  Cell_Building(void) const;
		CellClass const &  Adjacent_Cell(FacingType face) const;
		CellClass &  Adjacent_Cell(FacingType face) {return (CellClass &)((*((CellClass const *)this)).Adjacent_Cell(face));};
		COORDINATE  Cell_Coord(void) const;
		int  Cell_Color(bool override=false) const;
		CELL  Cell_Number(void) const;
		LandType  Land_Type(void) const {return((OverrideLand != LAND_COUNT) ? OverrideLand : Land);};
		ObjectClass *  Cell_Find_Object(RTTIType rtti) const;
		ObjectClass *  Cell_Object(int x=0, int y=0) const;
		TechnoClass *  Cell_Techno(int x=0, int y=0) const;
		TerrainClass *  Cell_Terrain(void) const;
		UnitClass *  Cell_Unit(void) const;
		InfantryClass *  Cell_Infantry(void) const;
		TriggerClass *  Get_Trigger(void) const;
		int  Clear_Icon(void) const;
		bool  Goodie_Check(FootClass * object);
		ObjectClass *  Fetch_Occupier(void) const;
		bool Get_Template_Info(char *template_name, int &icon, void *&image_data);

		/*
		**	Object placement and removal flag operations.
		*/
		void  Occupy_Down(ObjectClass * object);
		void  Occupy_Up(ObjectClass * object);
		void  Overlap_Down(ObjectClass * object);
		void  Overlap_Up(ObjectClass * object);
		bool  Flag_Place(HousesType house);
		bool  Flag_Remove(void);

		/*
		**	File I/O.
		*/
		bool  Should_Save(void) const;
		bool  Save(FileClass & file);
		bool  Load(FileClass & file);
		void  Code_Pointers(void);
		void  Decode_Pointers(void);

		/*
		**	Display and rendering controls.
		*/
		void  Draw_It(int x, int y, int draw_flags = 0) const;
		void  Redraw_Objects(bool forced=false);
		void  Shimmer(void);

		/*
		**	Maintenance calculation support.
		*/
		long  Tiberium_Adjust(bool pregame=false);
		void  Wall_Update(void);
		void  Concrete_Calc(void);
		void  Recalc_Attributes(void);
		int   Reduce_Tiberium(int levels);
		int   Reduce_Wall(int damage);
		void  Incoming(COORDINATE threat=0, bool forced = false, bool nokidding = false);
		void  Adjust_Threat(HousesType house, int threat_value);

		int operator != (CellClass const &) const {return 0;};

		int Validate(void) const;


		/*
		**	Additional per-player functionality is needed for multiplayer. ST - 3/5/2019 3:03PM 
		*/
		void Set_Mapped(HousesType house, bool set = true);
		void Set_Mapped(HouseClass *player, bool set = true);
		bool Is_Mapped(HousesType house) const;
		bool Is_Mapped(HouseClass *player) const;
		void Set_Visible(HousesType house, bool set = true);
		void Set_Visible(HouseClass *player, bool set = true);
		bool Is_Visible(HousesType house) const;
		bool Is_Visible(HouseClass *player) const;

		/*
		**	Override land type to fix passability issues on some maps
		*/
		void Override_Land_Type(LandType type);

#ifdef USE_RA_AI
		/*
		** Imported from RA for AI. ST - 7/24/2019 5:36PM
		*/
		bool Is_Clear_To_Move(bool ignoreinfantry, bool ignorevehicles) const;
#endif

	private:
		CellClass (CellClass const &) {};

		LandType Land;		// The land type of this cell.

		LandType OverrideLand;	// The overriden land type of this cell.

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];
};

#endif