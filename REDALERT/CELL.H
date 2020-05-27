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

/* $Header: /CounterStrike/CELL.H 1     3/03/97 10:24a Joe_bostic $ */
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
		**	This is the ID number of this cell. By placing the ID number here, it doesn't have
		**	be calculated. Calculating this number requires a divide and would occur about
		**	5.72031 bijillion times per second.
		*/
		short ID;

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
		**	A mapped cell has some portion of it visible. Maybe it has a shroud piece
		**	over it and maybe not.
		*/
		unsigned IsMapped:1;

		/*
		**	A visible cell means that it is completely visible with no shroud over
		**	it at all.
		*/
		unsigned IsVisible:1;

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
		**	This is a working flag used to help keep track of what cells should be
		**	shrouded. By using this flag it allows a single pass through the map
		**	cells for determining shadow regrowth logic.
		*/
		unsigned IsToShroud:1;

		/*
		**	This records the movement zone for this map. Movement zones share the
		**	same number if they are contiguous (terrain consideration only). There
		**	are basically two kinds of zones. The difference being determined by
		**	walls that can be crushed by movement. A vehicle that can crush walls
		**	will only consider the CrushZone. All other terrestrial travellers will
		**	use the normal Zone.
		*/
		unsigned char Zones[MZONE_COUNT];

		/*
		** This field controls whether an area is being jammed by a gap
		** generator.
		*/
		unsigned short Jammed;

		/*
		**	This is the trigger ID for any trigger that might be attached to
		**	this cell.
		*/
		CCPtr<TriggerClass> Trigger;

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
	private:
		ObjectClass * OccupierPtr;

	public:
#ifdef SORTDRAW
		ObjectClass * Overlapper[10];
#else
		ObjectClass * Overlapper[6];
#endif


		/*
		**	Per-player view of whether a cell is mapped. One bit for each house type. ST - 8/2/2019 3:00PM
		*/
		unsigned int IsMappedByPlayerMask;

		/*
		**	Per-player view of whether a cell is visible. One bit for each house type. ST - 8/2/2019 3:00PM
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
		CellClass(NoInitClass const & x) : Trigger(x) {}
		~CellClass(void) {OccupierPtr=0;}

		int operator == (CellClass const & cell) const {return &cell == this;}

		/*
		**	Query functions.
		*/
		bool Can_Tiberium_Germinate(void) const;
		bool Can_Tiberium_Grow(void) const;
		bool Can_Tiberium_Spread(void) const;
		bool Is_Bridge_Here(void) const;
		RTTIType What_Am_I(void) const {return(RTTI_CELL);}
		BuildingClass * Cell_Building(void) const;
		CELL Cell_Number(void) const {return(ID);}
		COORDINATE Cell_Coord(void) const;
		COORDINATE Closest_Free_Spot(COORDINATE coord, bool any=false) const;
		COORDINATE Free_Spot(void) const {return Closest_Free_Spot(Cell_Coord());}
		CellClass & Adjacent_Cell(FacingType face) {return (CellClass &)((*((CellClass const *)this)).Adjacent_Cell(face));}
		CellClass const & Adjacent_Cell(FacingType face) const;
		InfantryClass * Cell_Infantry(void) const;
		LandType Land_Type(void) const {return((OverrideLand != LAND_NONE) ? OverrideLand : Land);}
		ObjectClass * Cell_Find_Object(RTTIType rtti) const;
		ObjectClass * Cell_Object(int x=0, int y=0) const;
		ObjectClass * Cell_Occupier(void) const {return(OccupierPtr);}
		ObjectClass * Fetch_Occupier(void) const;
		TARGET As_Target(void) const {return ::As_Target(Cell_Number());}
		TechnoClass * Cell_Techno(int x=0, int y=0) const;
		TerrainClass * Cell_Terrain(void) const;
		UnitClass * Cell_Unit(void) const;
		VesselClass * Cell_Vessel(void) const;
		bool Goodie_Check(FootClass * object);
		bool Is_Clear_To_Build(SpeedType loco = SPEED_TRACK) const;
		bool Is_Clear_To_Move(SpeedType loco, bool ignoreinfantry, bool ignorevehicles, int zone=-1, MZoneType check=MZONE_NORMAL) const;
		bool Is_Spot_Free(int spot_index) const {return (! (Flag.Composite & (1 << spot_index)) ); }
		int Cell_Color(bool override=false) const;
		int Clear_Icon(void) const;
		static int Spot_Index(COORDINATE coord);
		bool Get_Template_Info(char *template_name, int &icon, void *&image_data);


		/*
		**	Object placement and removal flag operations.
		*/
		void Occupy_Down(ObjectClass * object);
		void Occupy_Up(ObjectClass * object);
		void Overlap_Down(ObjectClass * object);
		void Overlap_Up(ObjectClass * object);
		bool Flag_Place(HousesType house);
		bool Flag_Remove(void);

		/*
		**	File I/O.
		*/
		bool Should_Save(void) const;
		bool Save(Pipe & file) const;
		bool Load(Straw & file);
		void Code_Pointers(void);
		void Decode_Pointers(void);

		/*
		**	Display and rendering controls.
		*/
		void Draw_It(int x, int y, bool objects=false) const;
		void Redraw_Objects(bool forced=false);
		void Shimmer(void);

		/*
		**	Maintenance calculation support.
		*/
		bool Grow_Tiberium(void);
		bool Spread_Tiberium(bool forced=false);
		long Tiberium_Adjust(bool pregame=false);
		void Wall_Update(void);
		void Concrete_Calc(void);
		void Recalc_Attributes(void);
		int  Reduce_Tiberium(int levels);
		int  Reduce_Wall(int damage);
		void Incoming(COORDINATE threat=0, bool forced=false, bool nokidding=false);
		void Adjust_Threat(HousesType house, int threat_value);

		int operator != (CellClass const &) const {return 0;}

		
		/*
		**	Additional per-player functionality is needed for multiplayer. ST - 8/2/2019 3:01PM 
		*/
		void Set_Mapped(HousesType house, bool set = true);
		void Set_Mapped(HouseClass *player, bool set = true);
		bool Is_Mapped(HousesType house) const;
		bool Is_Mapped(HouseClass *player) const;
		void Set_Visible(HousesType house, bool set = true);
		void Set_Visible(HouseClass *player, bool set = true);
		bool Is_Visible(HousesType house) const;
		bool Is_Visible(HouseClass *player) const;
		bool Is_Jamming(HousesType house) const;
		bool Is_Jamming(HouseClass *player) const;

		/*
		**	Override land type to fix passability issues on some maps
		*/
		void Override_Land_Type(LandType type);

	private:
		CellClass (CellClass const &) ;

		LandType Land;			// The land type of this cell.

		LandType OverrideLand;	// The overriden land type of this cell.

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];
};

#endif