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

/* $Header: /CounterStrike/TEMPLATE.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TEMPLATE.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 17, 1994                                                 *
 *                                                                                             *
 *                  Last Update : January 23, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TemplateClass::Init -- Resets the template object system.                                 *
 *   TemplateClass::Mark -- Lifts or drops a template object.                                  *
 *   TemplateClass::Select -- Select the template object.                                      *
 *   TemplateClass::TemplateClass -- Template object constructor.                              *
 *   TemplateClass::Unlimbo -- Places a template object into the game/map system.              *
 *   TemplateClass::delete -- Returns a template object to the pool.                           *
 *   TemplateClass::new -- Allocates a template object from pool                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"template.h"


/***********************************************************************************************
 * TemplateClass::Init -- Resets the template object system.                                   *
 *                                                                                             *
 *    This routine resets the template object system. It is called                             *
 *    prior to loading a new scenario.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateClass::Init(void)
{
	Templates.Free_All();
}


/***********************************************************************************************
 * TemplateClass::Mark -- Lifts or drops a template object.                                    *
 *                                                                                             *
 *    This routine handles placing or removing a template object. This                         *
 *    entails marking the map as appropriate and redisplaying affected                         *
 *    cells.                                                                                   *
 *                                                                                             *
 * INPUT:   mark  -- The marking operation to perform.                                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the template successfully marked?                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/17/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Examines low level legality before processing.                           *
 *=============================================================================================*/
bool TemplateClass::Mark(MarkType mark)
{
	assert(Templates.ID(this) == ID);
	assert(IsActive);

	static bool noup = false;
	void const * iset = Get_Image_Data();
	if (iset && ObjectClass::Mark(mark)) {

		void * map = Get_Icon_Set_Map(iset);

		for (int y = 0; y < Class->Height; y++) {
			for (int x = 0; x < Class->Width; x++) {
				CELL cell = Coord_Cell(Coord) + y*MAP_CELL_W + x;
				if (Map.In_Radar(cell)) {
					CellClass * cellptr = &Map[cell];
					int number = y*Class->Width + x;

					/*
					**	Determine if this logical icon actually maps to a real icon. If no real
					**	icon is associated with this logical position, then don't do any action
					**	since none is required.
					*/
					char * mapptr = (char*)map;
					bool real = (mapptr[number] != -1);

					if (real) {
						/*
						**	Lift the terrain object from the map.
						*/
						if (mark == MARK_UP && !noup) {
							if (cellptr->TType == Class->Type && cellptr->TIcon == number) {
								cellptr->TType = TEMPLATE_NONE;
								cellptr->TIcon = 0;
							}
						}

						/*
						**	Place the terrain object down.
						*/
						if (mark == MARK_DOWN) {
							if (*this == TEMPLATE_CLEAR1) {
								cellptr->TType = TEMPLATE_NONE;
								cellptr->TIcon = 0;
							} else {
								cellptr->TType = Class->Type;
								cellptr->TIcon = number;
							}

							/*
							**	Make sure that no overlays or smudges exist after
							**	placing the template down.
							*/
							cellptr->Smudge = SMUDGE_NONE;
							cellptr->SmudgeData = 0;
							cellptr->Overlay = OVERLAY_NONE;
							cellptr->OverlayData = 0;
						}

						cellptr->Redraw_Objects();
						cellptr->Recalc_Attributes();
					}
				}
			}
		}

		/*
		**	When marking this template down onto the map, the map template numbers are update
		**	but the template is removed from existence. Make sure that the deletion of the
		**	template object doesn't also lift the template numbers up from the map.
		*/
		if (mark == MARK_DOWN) {
			noup = true;
			delete this;
			noup = false;
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TemplateClass::new -- Allocates a template object from pool                                 *
 *                                                                                             *
 *    This routine is used to allocate a template object from the                              *
 *    template object pool.                                                                    *
 *                                                                                             *
 * INPUT:   size  -- The size of a template object (not used).                                 *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to an available template object.                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void * TemplateClass::operator new(size_t )
{
	void * ptr = Templates.Allocate();
	if (ptr) {
		((TemplateClass *)ptr)->IsActive = true;
	}
	return(ptr);
}


/***********************************************************************************************
 * TemplateClass::delete -- Returns a template object to the pool.                             *
 *                                                                                             *
 *    This routine will return a template object to the template object                        *
 *    pool. A template so returned is available for allocation again.                          *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the object to be returned.                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateClass::operator delete(void * ptr)
{
	if (ptr) {
		((TemplateClass *)ptr)->IsActive = false;
	}
	Templates.Free((TemplateClass *)ptr);
}


/***********************************************************************************************
 * TemplateClass::TemplateClass -- Template object constructor.                                *
 *                                                                                             *
 *    This is the constructor for a template object.                                           *
 *                                                                                             *
 * INPUT:   type  -- The template object this is to become.                                    *
 *                                                                                             *
 *          pos   -- The position on the map to place the object.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TemplateClass::TemplateClass(TemplateType type, CELL pos) :
	ObjectClass(RTTI_TEMPLATE, Templates.ID(this)),
	Class(TemplateTypes.Ptr((int)type))
{
	if (pos != -1) {
		Unlimbo(Cell_Coord(pos));
	}
}
