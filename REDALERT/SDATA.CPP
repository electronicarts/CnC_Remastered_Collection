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

/* $Header: /CounterStrike/SDATA.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 9, 1994                                               *
 *                                                                                             *
 *                  Last Update : July 9, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   SmudgeTypeClass::As_Reference -- Fetches a reference to the smudge type specified.        *
 *   SmudgeTypeClass::Create_And_Place -- Creates and places on map, a smudge object.          *
 *   SmudgeTypeClass::Create_One_Of -- Creates a smudge object of this type.                   *
 *   SmudgeTypeClass::Display -- Draws a generic version of this smudge type.                  *
 *   SmudgeTypeClass::Draw_It -- Renders the smudge image at the coordinate specified.         *
 *   SmudgeTypeClass::From_Name -- Converts an ASCII name into a smudge type.                  *
 *   SmudgeTypeClass::Init -- Performs theater specific initializations.                       *
 *   SmudgeTypeClass::Init_Heap -- Initialize the smudge type class object heap.               *
 *   SmudgeTypeClass::One_Time -- Performs one-time initialization                             *
 *   SmudgeTypeClass::Prep_For_Add -- Prepares the scenario editor for adding a smudge object. *
 *   SmudgeTypeClass::SmudgeTypeClass -- Constructor for smudge type objects.                  *
 *   SmudgeTypeClass::operator delete -- Returns a smudge type class object to the pool.       *
 *   SmudgeTypeClass::operator new -- Allocate a smudge type object from the memory pool.      *
 *   SmudgetypeClass::Occupy_List -- Determines occupation list for smudge object.             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"type.h"


static SmudgeTypeClass const Crater1 (
	SMUDGE_CRATER1,
	"CR1",
	TXT_CRATER,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	true							// Is this a crater smudge?
);
static SmudgeTypeClass const Crater2 (
	SMUDGE_CRATER2,
	"CR2",
	TXT_CRATER,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	true							// Is this a crater smudge?
);
static SmudgeTypeClass const Crater3 (
	SMUDGE_CRATER3,
	"CR3",
	TXT_CRATER,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	true							// Is this a crater smudge?
);
static SmudgeTypeClass const Crater4 (
	SMUDGE_CRATER4,
	"CR4",
	TXT_CRATER,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	true							// Is this a crater smudge?
);
static SmudgeTypeClass const Crater5 (
	SMUDGE_CRATER5,
	"CR5",
	TXT_CRATER,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	true							// Is this a crater smudge?
);
static SmudgeTypeClass const Crater6 (
	SMUDGE_CRATER6,
	"CR6",
	TXT_CRATER,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	true							// Is this a crater smudge?
);
static SmudgeTypeClass const Scorch1 (
	SMUDGE_SCORCH1,
	"SC1",
	TXT_SCORCH,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	false							// Is this a crater smudge?
);
static SmudgeTypeClass const Scorch2 (
	SMUDGE_SCORCH2,
	"SC2",
	TXT_SCORCH,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	false							// Is this a crater smudge?
);
static SmudgeTypeClass const Scorch3 (
	SMUDGE_SCORCH3,
	"SC3",
	TXT_SCORCH,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	false							// Is this a crater smudge?
);
static SmudgeTypeClass const Scorch4 (
	SMUDGE_SCORCH4,
	"SC4",
	TXT_SCORCH,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	false							// Is this a crater smudge?
);
static SmudgeTypeClass const Scorch5 (
	SMUDGE_SCORCH5,
	"SC5",
	TXT_SCORCH,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	false							// Is this a crater smudge?
);
static SmudgeTypeClass const Scorch6 (
	SMUDGE_SCORCH6,
	"SC6",
	TXT_SCORCH,
	1,1,							// Width and height of smudge (in icons).
	false,						// Is this a building bib?
	false							// Is this a crater smudge?
);

static SmudgeTypeClass const Bibx1 (
	SMUDGE_BIB1,
	"BIB1",
	TXT_BIB,
	4,2,							// Width and height of smudge (in icons).
	true,							// Is this a building bib?
	false							// Is this a crater smudge?
);
static SmudgeTypeClass const Bibx2 (
	SMUDGE_BIB2,
	"BIB2",
	TXT_BIB,
	3,2,							// Width and height of smudge (in icons).
	true,							// Is this a building bib?
	false							// Is this a crater smudge?
);
static SmudgeTypeClass const Bibx3 (
	SMUDGE_BIB3,
	"BIB3",
	TXT_BIB,
	2,2,							// Width and height of smudge (in icons).
	true,							// Is this a building bib?
	false							// Is this a crater smudge?
);


/***********************************************************************************************
 * SmudgeTypeClass::SmudgeTypeClass -- Constructor for smudge type objects.                    *
 *                                                                                             *
 *    This constructor is used to create the smudge type objects. These type objects contain   *
 *    static information about the various smudge types supported in the game.                 *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
SmudgeTypeClass::SmudgeTypeClass(
	SmudgeType smudge,
	char const * ininame,
	int fullname,
	int width,
	int height,
	bool isbib,
	bool iscrater) :
		ObjectTypeClass(
			RTTI_SMUDGETYPE,
			int(smudge),
			false,
			true,
			false,
			false,
			true,
			true,
			false,
			fullname,
			ininame),
	Type(smudge),
	Width(width),
	Height(height),
	IsCrater(iscrater),
	IsBib(isbib)
{
}


/***********************************************************************************************
 * SmudgeTypeClass::operator new -- Allocate a smudge type object from the memory pool.        *
 *                                                                                             *
 *    This will allocate a smudge type class object from the special memory pool for that      *
 *    purpose.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the newly allocated smudge type class object. If there is insufficient*
 *          memory in the pool to fulfill the request, then NULL is returned.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * SmudgeTypeClass::operator new(size_t)
{
	return(SmudgeTypes.Alloc());
}


/***********************************************************************************************
 * SmudgeTypeClass::operator delete -- Returns a smudge type class object to the pool.         *
 *                                                                                             *
 *    This will return the smudge type class object back to the memory pool from whence it     *
 *    was originally allocated.                                                                *
 *                                                                                             *
 * INPUT:   pointer  -- Pointer to the smudge type class object to return the pool.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeTypeClass::operator delete(void * pointer)
{
	SmudgeTypes.Free((SmudgeTypeClass *)pointer);
}


/***********************************************************************************************
 * SmudgeTypeClass::Init_Heap -- Initialize the smudge type class object heap.                 *
 *                                                                                             *
 *    This will initialize the special heap for smudge type class objects, by pre-allocated    *
 *    all known smudge types.                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Call this routine only once and before the rules.ini file is processed.         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeTypeClass::Init_Heap(void)
{
	/*
	**	These smudge type class objects must be allocated in the exact order that they
	**	are specified in the SmudgeType enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	new SmudgeTypeClass(Crater1);			// SMUDGE_CRATER1
	new SmudgeTypeClass(Crater2);			// SMUDGE_CRATER2
	new SmudgeTypeClass(Crater3);			// SMUDGE_CRATER3
	new SmudgeTypeClass(Crater4);			// SMUDGE_CRATER4
	new SmudgeTypeClass(Crater5);			// SMUDGE_CRATER5
	new SmudgeTypeClass(Crater6);			// SMUDGE_CRATER6
	new SmudgeTypeClass(Scorch1);			// SMUDGE_SCORCH1
	new SmudgeTypeClass(Scorch2);			// SMUDGE_SCORCH2
	new SmudgeTypeClass(Scorch3);			// SMUDGE_SCORCH3
	new SmudgeTypeClass(Scorch4);			// SMUDGE_SCORCH4
	new SmudgeTypeClass(Scorch5);			// SMUDGE_SCORCH5
	new SmudgeTypeClass(Scorch6);			// SMUDGE_SCORCH6
	new SmudgeTypeClass(Bibx1);			// SMUDGE_BIB1
	new SmudgeTypeClass(Bibx2);			//	SMUDGE_BIB2
	new SmudgeTypeClass(Bibx3);			// SMUDGE_BIB3
}


/***********************************************************************************************
 * SmudgeTypeClass::From_Name -- Converts an ASCII name into a smudge type.                    *
 *                                                                                             *
 *    This converts an ASCII name into a smudge type number. This is typically necessary       *
 *    when processing scenario INI files and not used otherwise.                               *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the name to convert.                                           *
 *                                                                                             *
 * OUTPUT:  Returns with the SmudgeType number that matches the name supplied. If no match     *
 *          was found, then SMUDGE_NONE is returned.                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
SmudgeType SmudgeTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (SmudgeType index = SMUDGE_FIRST; index < SMUDGE_COUNT; index++) {
			if (stricmp(As_Reference(index).IniName, name) == 0) {
				return(index);
			}
		}
	}
	return(SMUDGE_NONE);
}


/***********************************************************************************************
 * SmudgetypeClass::Occupy_List -- Determines occupation list for smudge object.               *
 *                                                                                             *
 *    Smudges are always only one icon in dimension, so this routine always returns a cell     *
 *    occupation offset list of the center cell.                                               *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns occupation list specifying all the cells that the overlay occupies. This   *
 *          is just the center cell.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * SmudgeTypeClass::Occupy_List(bool) const
{
	static short _occupy[4*4];
	short * ptr = &_occupy[0];

	for (int x = 0; x < Width; x++) {
		for (int y = 0; y < Height; y++) {
			*ptr++ = x + (y*MAP_CELL_W);
		}
	}
	*ptr = REFRESH_EOL;
	return(_occupy);
}


/***********************************************************************************************
 * SmudgeTypeClass::Init -- Performs theater specific initializations.                         *
 *                                                                                             *
 *    Smudge object imagery varies between theaters. This routine will load the appropriate    *
 *    imagery for the theater specified.                                                       *
 *                                                                                             *
 * INPUT:   theater  -- The theater to prepare for.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeTypeClass::Init(TheaterType theater)
{
	if (theater != LastTheater) {
		for (SmudgeType index = SMUDGE_FIRST; index < SMUDGE_COUNT; index++) {
			SmudgeTypeClass const & smudge = As_Reference(index);
			char fullname[_MAX_FNAME+_MAX_EXT];	// Fully constructed smudge data set name.

			_makepath(fullname, NULL, NULL, smudge.IniName, Theaters[theater].Suffix);
			((void const *&)smudge.ImageData) = MFCD::Retrieve(fullname);
		}
	}
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * SmudgeTypeClass::Display -- Draws a generic version of this smudge type.                    *
 *                                                                                             *
 *    The scenario object editor will call this routine to display a typical imagery of this   *
 *    smudge object for graphical identification purposes.                                     *
 *                                                                                             *
 * INPUT:   x,y   -- Coordinate to render the smudge at.                                       *
 *                                                                                             *
 *          window-- The window to base the coordinate rendering upon.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeTypeClass::Display(int x, int y, WindowNumberType window, HousesType ) const
{
	void const * ptr = Get_Image_Data();

	x += WindowList[window][WINDOWX];
	y += WindowList[window][WINDOWY];

	IsTheaterShape = true;		// Smudges are theater specific
	if (ptr != NULL) {
		for (int w = 0; w < Width; w++) {
			for (int h = 0; h < Height; h++) {
				CC_Draw_Shape(ptr, w + (h*Width), x + w*ICON_PIXEL_W, y + h*ICON_PIXEL_H, WINDOW_TACTICAL, SHAPE_WIN_REL);
			}
		}
	}
	IsTheaterShape = false;
}


/***********************************************************************************************
 * SmudgeTypeClass::Prep_For_Add -- Prepares the scenario editor for adding a smudge object.   *
 *                                                                                             *
 *    This routine adds smudge objects to the list of objects that the scenario editor can     *
 *    place upon the ground. It is only called from the scenario editor.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeTypeClass::Prep_For_Add(void)
{
	for (SmudgeType index = SMUDGE_FIRST; index < SMUDGE_COUNT; index++) {
		if (As_Reference(index).Get_Image_Data()) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}
#endif


/***********************************************************************************************
 * SmudgeTypeClass::Create_And_Place -- Creates and places on map, a smudge object.            *
 *                                                                                             *
 *    This routine will, in one motion, create a smudge object and place it upon the map.      *
 *    Since placing a smudge on the map will destroy the object, this routine will leave the   *
 *    smudge object count unchanged. Typically, this routine is used by the scenario editor    *
 *    for creating smudges and placing them on the map.                                        *
 *                                                                                             *
 * INPUT:   cell  -- The cell to place the smudge object.                                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the placement successful?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SmudgeTypeClass::Create_And_Place(CELL cell, HousesType ) const
{
	if (new SmudgeClass(Type, Cell_Coord(cell))) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * SmudgeTypeClass::Create_One_Of -- Creates a smudge object of this type.                     *
 *                                                                                             *
 *    This routine will create a smudge object of the appropriate type. Smudge objects are     *
 *    transitory in nature. They exist only from the point of creation until they are given    *
 *    a spot on the map to reside. At that time the map data is updated and the smudge         *
 *    object is destroyed.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a created smudge object. If none could be created, then  *
 *          NULL is returned.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * SmudgeTypeClass::Create_One_Of(HouseClass *) const
{
	return(new SmudgeClass(Type, -1));
}


/***********************************************************************************************
 * SmudgeTypeClass::Draw_It -- Renders the smudge image at the coordinate specified.           *
 *                                                                                             *
 *    This routine will draw the smudge overlay image at the coordinate (upper left)           *
 *    specified. The underlying terrain icon is presumed to have already been rendered.        *
 *                                                                                             *
 * INPUT:   x,y   -- Coordinate of the upper left corner of icon to render the smudge object.  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeTypeClass::Draw_It(int x, int y, int data) const
{
	void const * ptr = Get_Image_Data();
	if (ptr != NULL) {
		IsTheaterShape = true;		// Smudges are theater specific
		CC_Draw_Shape(ptr, data, x, y, WINDOW_TACTICAL, SHAPE_WIN_REL);
		IsTheaterShape = false;
	}
}


/***********************************************************************************************
 * SmudgeTypeClass::One_Time -- Performs one-time initialization                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeTypeClass::One_Time(void)
{
}


/***********************************************************************************************
 * SmudgeTypeClass::As_Reference -- Fetches a reference to the smudge type specified.          *
 *                                                                                             *
 *    Use this routine to get a reference to the smudge type class object when given just      *
 *    the smudge type identifier.                                                              *
 *                                                                                             *
 * INPUT:   type  -- The smudge type identifier to convert into a reference.                   *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the smudge type class object.                          *
 *                                                                                             *
 * WARNINGS:   Be sure that the smudge type specified is legal. An illegal type value will     *
 *             produce undefined results.                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
SmudgeTypeClass & SmudgeTypeClass::As_Reference(SmudgeType type)
{
	return(*SmudgeTypes.Ptr(type));
}
