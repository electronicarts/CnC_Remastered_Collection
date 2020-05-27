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

/* $Header: /CounterStrike/MAP.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MAP.H                                                        *
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

#ifndef MAP_H
#define MAP_H

#include	"gscreen.h"
#include	"crate.h"

class MapClass: public GScreenClass
{
	public:

		MapClass(void) {};
		MapClass(NoInitClass const & x) : GScreenClass(x), Array(x) {};

		/*
		** Initialization
		*/
		virtual void One_Time(void);						// Theater-specific inits
		virtual void Init_Clear(void);						// Clears all to known state
		virtual void Alloc_Cells(void);						// Allocates buffers
		virtual void Free_Cells(void);							// Frees buffers
		virtual void Init_Cells(void);							// Frees buffers

		/*--------------------------------------------------------
		** Main functions that deal with groupings of cells within the map or deals with the cell
		** as it relates to the map - not what the cell contains.
		*/
		CELL Pick_Random_Location(void) const;
		int Intact_Bridge_Count(void) const;
		bool Base_Region(CELL cell, HousesType & house, ZoneType & zone) const;
		CELL Nearby_Location(CELL cell, SpeedType speed, int zone=-1, MZoneType check=MZONE_NORMAL, bool checkflagged=false) const;
		ObjectClass * Close_Object(COORDINATE coord) const;
		virtual void Detach(ObjectClass * ) {};
		int Cell_Region(CELL cell);
		int Cell_Threat(CELL cell, HousesType house);
		bool In_Radar(CELL cell) const;
		void Sight_From(CELL cell, int sightrange, HouseClass *house, bool incremental=false);
		void Jam_From(CELL cell, int jamrange, HouseClass *house);
		void Shroud_From(CELL cell, int sightrange, HouseClass *house);
		void UnJam_From(CELL cell, int jamrange, HouseClass *house);
		void Place_Down(CELL cell, ObjectClass * object);
		void Pick_Up(CELL cell, ObjectClass * object);
		void Overlap_Down(CELL cell, ObjectClass * object);
		void Overlap_Up(CELL cell, ObjectClass * object);
		bool Read_Binary(Straw & straw);
		int Write_Binary(Pipe & pipe);
		bool Place_Random_Crate(void);
		bool Remove_Crate(CELL cell);
		bool Zone_Reset(int method);
		bool Zone_Cell(CELL cell, int zone);
		int Zone_Span(CELL cell, int zone, MZoneType check);
		bool Destroy_Bridge_At(CELL cell);
		void Detach(TARGET target, bool all=true);
		void Shroud_The_Map(HouseClass *house);

		long Overpass(void);

		virtual void Logic(void);
		virtual void Set_Map_Dimensions(int x, int y, int w, int h);

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		/*
		** Debug routine
		*/
		int Validate(void);

		/*
		**	This is the dimensions and position of the sub section of the global map.
		**	It is this region that appears on the radar map and constrains normal
		**	movement.
		*/
		int MapCellX;
		int MapCellY;
		int MapCellWidth;
		int MapCellHeight;

		/*
		**	This is the total value of all harvestable Tiberium on the map.
		*/
		long TotalValue;

		CellClass & operator [] (COORDINATE coord) {return(Array[Coord_Cell(coord)]);};
		CellClass & operator [] (CELL cell) {return(Array[cell]);};
		CellClass const & operator [] (COORDINATE coord) const {return(Array[Coord_Cell(coord)]);};
		CellClass const & operator [] (CELL cell) const {return(Array[cell]);};
		int ID(CellClass const * ptr) {return(Array.ID(ptr));};
		int ID(CellClass const & ptr) {return(Array.ID(ptr));};

	protected:

		/*
		**	This is the array of cell objects.
		*/
		VectorClass<CellClass> Array;

		/*
		**	These are the size dimensions of the underlying array of cell objects.
		**	This is the dimensions of the "map" that the tactical view is
		**	restricted to.
		*/
		int	XSize;
		int	YSize;
		int	Size;

		static int const RadiusCount[11];
		static int const RadiusOffset[];

		/*
		**	This specifies the information for the various crates in the game.
		*/
		CrateClass Crates[256];

	private:
		friend class CellClass;

		/*
		**	Tiberium growth potential cells are recorded here.
		*/
		CELL TiberiumGrowth[MAP_CELL_W/2];
		int TiberiumGrowthCount;
		int TiberiumGrowthExcess;

		/*
		**	List of cells that are full enough strength that they could spread
		**	Tiberium to adjacent cells.
		*/
		CELL TiberiumSpread[MAP_CELL_W/2];
		int TiberiumSpreadCount;
		int TiberiumSpreadExcess;

		/*
		**	This is the current cell number in the incremental map scan process.
		*/
		CELL TiberiumScan;

		enum MapEnum {SCAN_AMOUNT=MAP_CELL_TOTAL};

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[1024];

};

#endif