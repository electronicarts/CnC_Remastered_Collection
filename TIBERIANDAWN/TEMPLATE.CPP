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

/* $Header:   F:\projects\c&c\vcs\code\template.cpv   2.18   16 Oct 1995 16:51:46   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *   TemplateClass::As_Target -- Converts a template object into a target number.              *
 *   TemplateClass::Init -- Resets the template object system.                                 *
 *   TemplateClass::Mark -- Lifts or drops a template object.                                  *
 *   TemplateClass::Read_INI -- Reads the scenario control INI file.                           *
 *   TemplateClass::Select -- Select the template object.                                      *
 *   TemplateClass::TemplateClass -- Default constructor for template class objects.           *
 *   TemplateClass::TemplateClass -- Template object constructor.                              *
 *   TemplateClass::Unlimbo -- Places a template object into the game/map system.              *
 *   TemplateClass::Write_INI -- Writes the template objects to the INI file.                  *
 *   TemplateClass::delete -- Returns a template object to the pool.                           *
 *   TemplateClass::new -- Allocates a template object from pool                               *
 *   TemplateClass::Validate -- validates template pointer												  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"template.h"


/*
** This contains the value of the Virtual Function Table Pointer
*/
void * TemplateClass::VTable;


/***********************************************************************************************
 * TemplateClass::Validate -- validates template pointer													  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		none.																												  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		1 = ok, 0 = error																								  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		none.																												  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/09/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
#ifdef CHEAT_KEYS
int TemplateClass::Validate(void) const
{
	int num;

	num = Templates.ID(this);
	if (num < 0 || num >= TEMPLATE_MAX) {
		Validate_Error("TEMPLATE");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


/***********************************************************************************************
 * TemplateClass::Read_INI -- Reads the scenario control INI file.                             *
 *                                                                                             *
 *    This routine reads the scenario control INI file and creates all                         *
 *    templates specified therein.                                                             *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the loaded scenario control INI file.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateClass::Read_INI(char *buffer)
{
	char	*tbuffer;	// Accumulation buffer of unit IDs.
	int	len;			// Size of data in buffer.
	CELL	cell;			// Cell of building.
	char	buf[128];	// Working string staging buffer.

	len = strlen(buffer) + 2;
	tbuffer = buffer + len;

	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-len, buffer);
	while (*tbuffer != '\0') {
		TemplateType	temp;		// Terrain type.

		cell = atoi(tbuffer);
		WWGetPrivateProfileString(INI_Name(), tbuffer, NULL, buf, sizeof(buf)-1, buffer);
		temp = TemplateTypeClass::From_Name(strtok(buf, ",\r\n"));
		if (temp != TEMPLATE_NONE) {
			new TemplateClass(temp, cell);
		}
		tbuffer += strlen(tbuffer)+1;
	}
}


/***********************************************************************************************
 * TemplateClass::Write_INI -- Writes the template objects to the INI file.                    *
 *                                                                                             *
 *    This routine is used to write all the template objects out to the INI file specified.    *
 *    It is used by the scenario editor when saving the game.                                  *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the INI file staging buffer.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateClass::Write_INI(char *buffer)
{
	char	uname[10];
	char	buf[127];
	char	*tbuffer;		// Accumulation buffer of unit IDs.

	/*
	**	First, clear out all existing template data from the ini file.
	*/
	tbuffer = buffer + strlen(buffer) + 2;
	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-strlen(buffer), buffer);
	while (*tbuffer != '\0') {
		WWWritePrivateProfileString(INI_Name(), tbuffer, NULL, buffer);
		tbuffer += strlen(tbuffer)+1;
	}

	/*
	**	Find all templates and write them to the file.
	*/
	for (int index = 0; index < MAP_CELL_TOTAL; index++) {
		CellClass * ptr;

		ptr = &Map[index];
		if (ptr->TType != TEMPLATE_NONE && ptr->TIcon == 0) {
			sprintf(uname, "%03d", index);
			sprintf(buf, "%s", TemplateTypeClass::As_Reference(ptr->TType).IniName);
			WWWritePrivateProfileString(INI_Name(), uname, buf, buffer);
		}
	}
}


/***********************************************************************************************
 * TemplateClass::TemplateClass -- Default constructor for template class objects.             *
 *                                                                                             *
 *    This is the default constructor for a template class object. This construction method    *
 *    should NEVER be used by the game except as a consequence of declaring an array of        *
 *    uninitialized template objects.                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TemplateClass::TemplateClass(void) :
	Class(0)
{
}


/***********************************************************************************************
 * TemplateClass::As_Target -- Converts a template object into a target number.                *
 *                                                                                             *
 *    This routine will convert a template object into a target number. Because templates      *
 *    never exist as a template object in the game system, this routine will never be called.  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/25/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET TemplateClass::As_Target(void) const
{
	Validate();
	return(Build_Target(KIND_TEMPLATE, Templates.ID(this)));
}


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
	TemplateClass *ptr;

	Templates.Free_All();

	ptr = new TemplateClass();
	VTable = ((void **)(((char *)ptr) + sizeof(AbstractClass) - 4))[0];
	delete ptr;
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
	Validate();
	static bool noup = false;
	void const * iset = Class->Get_Image_Data();
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
	//							cellptr->TIcon = real;
								cellptr->TIcon = number;
							}
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
		((TemplateClass *)ptr)->Set_Active();
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
void TemplateClass::operator delete(void *ptr)
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
	Class(&TemplateTypeClass::As_Reference(type))
{
	if (pos != -1) {
		Unlimbo(Cell_Coord(pos));
	}
}