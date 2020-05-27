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

/* $Header: /CounterStrike/BASE.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BASE.H                                                       *
 *                                                                                             *
 *                   Programmer : Bill Randolph																  *
 *                                                                                             *
 *                   Start Date : 03/27/95                                                     *
 *                                                                                             *
 *                  Last Update : March 27, 1995															  *
 *                                                                                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef BASE_H
#define BASE_H


/****************************************************************************
** This class defines one "node" in the pre-built base list.  Each node
** contains a type of building to build, and the COORDINATE to build it at.
*/
class BaseNodeClass
{
	public:
		BaseNodeClass(void) {};
		BaseNodeClass(StructType building, CELL cell) : Type(building), Cell(cell) {};
		int operator == (BaseNodeClass const & node);
		int operator != (BaseNodeClass const & node);
		int operator > (BaseNodeClass const & node);

		StructType Type;
		CELL Cell;
};


/****************************************************************************
** This is the class that defines a pre-built base for the computer AI.
** (Despite its name, this is NOT the "base" class for C&C's class hierarchy!)
*/
class BaseClass
{
	public:

		/*
		** Constructor/Destructor
		*/
		BaseClass(void) {};
		virtual ~BaseClass() {Nodes.Clear();}

		/*
		** Initialization
		*/
		void Init(void) {House = HOUSE_NONE; Nodes.Clear();}

		/*
		** The standard suite of load/save support routines
		*/
		void Read_INI(CCINIClass & ini);
		void Write_INI(CCINIClass & ini);
		static char *INI_Name(void) {return "Base";}
		bool Load(Straw & file);
		bool Save(Pipe & file) const;
		virtual void Code_Pointers(void) {};
		virtual void Decode_Pointers(void) {};

		/*
		** Tells if the given node has been built or not
		*/
		bool Is_Built(int index) const;

		/*
		** Returns a pointer to the object for the given node
		*/
		BuildingClass * Get_Building(int index) const;

		/*
		** Tells if the given building ptr is a node in this base's list.
		*/
		bool Is_Node(BuildingClass const * obj);

		/*
		** Returns a pointer to the requested node.
		*/
		BaseNodeClass * Get_Node(BuildingClass const * obj);
		BaseNodeClass * Get_Node(int index) { return (&Nodes[index]); }
		BaseNodeClass * Get_Node(CELL cell);

		/*
		** Returns a pointer to the next "hole" in the Nodes list.
		*/
		BaseNodeClass * Next_Buildable(StructType type = STRUCT_NONE);

		/*
		** This is the list of "nodes" that define the base.  Portions of this
		** list can be pre-built by simply saving those buildings in the INI
		** along with non-base buildings, so Is_Built will return true for them.
		*/
		DynamicVectorClass<BaseNodeClass> Nodes;

		/*
		** This is the house this base belongs to.
		*/
		HousesType House;
};


#endif

