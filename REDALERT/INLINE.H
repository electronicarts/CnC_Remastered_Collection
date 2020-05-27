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

/* $Header: /CounterStrike/INLINE.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INLINE.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 08/21/96                                                     *
 *                                                                                             *
 *                  Last Update : September 30, 1996 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Adjacent_Cell -- Calculate the adjacent cell in the direction specified.                  *
 *   Adjacent_Cell -- Calculate the adjacent cell in the direction specified.                  *
 *   Cell_Coord -- Convert a cell to a coordinate value.                                       *
 *   Cell_To_Lepton -- Convert a cell distance into a lepton distance.                         *
 *   Cell_X -- Fetch the X cell component from the cell value.                                 *
 *   Cell_Y -- Fetch the Y cell component from the cell value specified.                       *
 *   Coord_Add -- Adds coordinates together.                                                   *
 *   Coord_Fraction -- Discards all but the sub-cell components of the coordinate.             *
 *   Coord_Mid -- Finds the midpoint between two coordinates.                                  *
 *   Coord_Snap -- Coerce coordinate to refer to center of a cell.                             *
 *   Coord_Sub -- Subtracts one coordinate from another.                                       *
 *   Coord_Whole -- Discards the sub-cell components of the coordinate.                        *
 *   Coord_X -- Fetches the X lepton component from a coordinate value.                        *
 *   Coord_XCell -- Fetch the X cell component from a coordinate value.                        *
 *   Coord_XLepton -- Fetch the X sub-cell lepton component from the coordinate.               *
 *   Coord_Y -- Fetch the Y lepton component from the coordinate value.                        *
 *   Coord_YCell -- Fetch the Y cell component from a coordinate.                              *
 *   Coord_YLepton -- Fetches the Y lepton sub-cell component from the coordinate.             *
 *   Dir_Facing -- Convert a DirType into a FacingType value.                                  *
 *   Dir_To_16 -- Convert a facing to a 0..15 value.                                           *
 *   Dir_To_32 -- Convert a DirType into a 0..31 value.                                        *
 *   Dir_To_8 -- Convert a DirType into a value from 0 to 7.                                   *
 *   Direction -- Calculates the DirType from one cell to another.                             *
 *   Direction -- Determines the facing value from one coordinate to another.                  *
 *   Direction256 -- Calculate the facing value from one coordinate to another.                *
 *   Direction8 -- Fetches the direction from one coordinate to another.                       *
 *   Distance -- Finds the distance between two arbitrary points.                              *
 *   Facing_Dir -- Convert a FacingType into a DirType.                                        *
 *   Lepton_To_Cell -- Convert lepton distance to cell distance.                               *
 *   Lepton_To_Pixel -- Convert a lepton value into pixel value.                               *
 *   Percent_Chance -- Calculate a percentage chance event.                                    *
 *   Pixel_To_Lepton -- Convert pixel value into lepton equivalent.                            *
 *   Random_Pick -- Pick a random number in a specified range.                                 *
 *   Sim_Percent_Chance -- Calculates a percentage chance event for local events.              *
 *   Sim_Random_Pick -- Picks a random number that will not affect the game.                   *
 *   Text_String -- Convert a text number into a text pointer.                                 *
 *   XYP_COORD -- Convert pixel components into a coordinate value.                            *
 *   XYP_Coord -- Combine pixel values into a coordinate.                                      *
 *   XY_Cell -- Create a cell from X and Y cell components.                                    *
 *   XY_Coord -- Convert X Y lepton components into a COORD.                                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef INLINE_H
#define INLINE_H


/***********************************************************************************************
 * Lepton_To_Pixel -- Convert a lepton value into pixel value.                                 *
 *                                                                                             *
 *    Use this routine to convert the specified lepton value into it's pixel corresponding     *
 *    value. The pixel value returned will be the closest pixel value to the lepton value. It  *
 *    will round up or down as necessary.                                                      *
 *                                                                                             *
 * INPUT:   lepton   -- The lepton value to convert into a pixel value.                        *
 *                                                                                             *
 * OUTPUT:  Returns with the lepton value rounded to the nearest pixel component.              *
 *                                                                                             *
 * WARNINGS:   Precision is not maintained by this routine. Thus, if a value is converted to   *
 *             pixel and then back to leptons, the value will probably not be the same.        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int Lepton_To_Pixel(LEPTON lepton)
{
	return (((int)(signed short)lepton * ICON_PIXEL_W) + (ICON_LEPTON_W / 2) - ((lepton < 0) ? (ICON_LEPTON_W - 1) : 0)) / ICON_LEPTON_W;
}


/***********************************************************************************************
 * Pixel_To_Lepton -- Convert pixel value into lepton equivalent.                              *
 *                                                                                             *
 *    This routine will take the specified pixel value and convert it into lepton value.       *
 *                                                                                             *
 * INPUT:   pixel -- The pixel value to convert.                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the lepton equivalent of the pixel value specified.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline LEPTON Pixel_To_Lepton(int pixel)
{
	return (LEPTON)(((pixel * ICON_LEPTON_W) + (ICON_PIXEL_W / 2) - ((pixel < 0) ? (ICON_PIXEL_W - 1) : 0)) / ICON_PIXEL_W);
}


/***********************************************************************************************
 * XY_Coord -- Convert X Y lepton components into a COORD.                                     *
 *                                                                                             *
 *    This routine will take the specified X and Y lepton components and combine them into     *
 *    a coordinate value.                                                                      *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y lepton components to combine.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with a coordinate value that is created from the X and Y lepton components.*
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE XY_Coord(LEPTON x, LEPTON y)
{
	COORD_COMPOSITE coord;

	coord.Sub.X.Raw = x;
	coord.Sub.Y.Raw = y;
	return(coord.Coord);
}


/***********************************************************************************************
 * XYP_COORD -- Convert pixel components into a coordinate value.                              *
 *                                                                                             *
 *    This routine will take the specified pixel components and convert and combine them into  *
 *    a coordinate value.                                                                      *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y pixel components to coerce into a coordinate value.           *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate value that matches the pixel values specified.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE XYP_COORD(int x, int y)
{
	return(XY_Coord(Pixel_To_Lepton(x), Pixel_To_Lepton(y)));
}


/***********************************************************************************************
 * Coord_XCell -- Fetch the X cell component from a coordinate value.                          *
 *                                                                                             *
 *    This routine will extract the X cell component from the coordinate value specified and   *
 *    return the value.                                                                        *
 *                                                                                             *
 * INPUT:   coord -- The coordinate value to extract the X component from.                     *
 *                                                                                             *
 * OUTPUT:  Returns with the X cell component of the coordinate value.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline CELL Coord_XCell(COORDINATE coord)
{
	return(((COORD_COMPOSITE &)coord).Sub.X.Sub.Cell);
}


/***********************************************************************************************
 * Coord_YCell -- Fetch the Y cell component from a coordinate.                                *
 *                                                                                             *
 *    This routine will extract the Y cell component from the coordinate value specified.      *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to extract the Y cell from.                                *
 *                                                                                             *
 * OUTPUT:  Returns with just the Y cell component of the coordinate value.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline CELL Coord_YCell(COORDINATE coord)
{
	return(((COORD_COMPOSITE &)coord).Sub.Y.Sub.Cell);
}


/***********************************************************************************************
 * XY_Cell -- Create a cell from X and Y cell components.                                      *
 *                                                                                             *
 *    This routine will construct a cell value by taking the X and Y cell value components     *
 *    and combining them appropriately.                                                        *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y cell components to combine.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the CELL value created from the specified components.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline CELL XY_Cell(int x, int y)
{
	CELL_COMPOSITE cell;
	cell.Cell = 0;
	cell.Sub.X = x;
	cell.Sub.Y = y;
	return(cell.Cell);
}


/***********************************************************************************************
 * Cell_To_Lepton -- Convert a cell distance into a lepton distance.                           *
 *                                                                                             *
 *    This routine will take the cell distance specified and convert it into a lepton distance.*
 *                                                                                             *
 * INPUT:   cell_distance  -- The distance in cells to convert.                                *
 *                                                                                             *
 * OUTPUT:  Returns with the lepton equivalent of the cell distance specified.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline LEPTON Cell_To_Lepton(int cell_distance)
{
	LEPTON_COMPOSITE lepton;
	lepton.Sub.Cell = (unsigned char)cell_distance;
	lepton.Sub.Lepton = 0;
	return(lepton.Raw);
}


/***********************************************************************************************
 * Lepton_To_Cell -- Convert lepton distance to cell distance.                                 *
 *                                                                                             *
 *    This routine will convert the specified lepton distance into the closest cell distance   *
 *    possible. This might require rounding up or down as necessary.                           *
 *                                                                                             *
 * INPUT:   lepton_distance   -- The lepton distance to convert.                               *
 *                                                                                             *
 * OUTPUT:  Returns with the cell distance that most closely corresponds to the lepton         *
 *          distance specified.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int Lepton_To_Cell(LEPTON lepton_distance)
{
	if (((LEPTON_COMPOSITE &)lepton_distance).Sub.Lepton >= (CELL_LEPTON_W/2)) {
		return(((LEPTON_COMPOSITE &)lepton_distance).Sub.Cell + 1);
	}
	return(((LEPTON_COMPOSITE &)lepton_distance).Sub.Cell);
}


/***********************************************************************************************
 * Coord_X -- Fetches the X lepton component from a coordinate value.                          *
 *                                                                                             *
 *    This routine will extract the X lepton component from the coordinate.                    *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to extract the X lepton equivalent from.                   *
 *                                                                                             *
 * OUTPUT:  Returns with the X lepton portion of the coordinate value specified.               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline LEPTON Coord_X(COORDINATE coord)
{
	return(((COORD_COMPOSITE &)coord).Sub.X.Raw);
}


/***********************************************************************************************
 * Coord_Y -- Fetch the Y lepton component from the coordinate value.                          *
 *                                                                                             *
 *    This routine will extract the Y lepton component from the coordinate value specified.    *
 *                                                                                             *
 * INPUT:   coord -- The coordinate value to dissect.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the Y lepton component from the specified coordinate value.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline LEPTON Coord_Y(COORDINATE coord)
{
	return(((COORD_COMPOSITE &)coord).Sub.Y.Raw);
}


/***********************************************************************************************
 * Cell_X -- Fetch the X cell component from the cell value.                                   *
 *                                                                                             *
 *    This routine will extract the X cell component from the cell value specified.            *
 *                                                                                             *
 * INPUT:   cell  -- The cell to extract.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns with the X cell component portion of the cell value specified.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int Cell_X(CELL cell)
{
	return(((CELL_COMPOSITE &)cell).Sub.X);
}


/***********************************************************************************************
 * Cell_Y -- Fetch the Y cell component from the cell value specified.                         *
 *                                                                                             *
 *    This routine will extract the Y cell component from the cell value.                      *
 *                                                                                             *
 * INPUT:   cell  -- The cell value to extract from.                                           *
 *                                                                                             *
 * OUTPUT:  Returns with the Y cell component of the cell value specified.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int Cell_Y(CELL cell)
{
	return(((CELL_COMPOSITE &)cell).Sub.Y);
}


/***********************************************************************************************
 * Coord_XLepton -- Fetch the X sub-cell lepton component from the coordinate.                 *
 *                                                                                             *
 *    This routine will extract just the X sub cell lepton component from the coordinate       *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   coord -- The coordinate value to extract from.                                     *
 *                                                                                             *
 * OUTPUT:  Returns with the X lepton component of the coordinate that is part of the cell.    *
 *          Thus, a coordinate that exactly lines up on the left edge of a cell would return   *
 *          zero. One that exactly lines up on the right edge would return CELL_LEPTON_W.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int Coord_XLepton(COORDINATE coord)
{
	return(((COORD_COMPOSITE &)coord).Sub.X.Sub.Lepton);
}


/***********************************************************************************************
 * Coord_YLepton -- Fetches the Y lepton sub-cell component from the coordinate.               *
 *                                                                                             *
 *    This routine will extract the sub-cell Y lepton portion of the coordinate.               *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to dissect.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with just the Y lepton portion of the coordinate and only for the sub-cell *
 *          it refers to.                                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int Coord_YLepton(COORDINATE coord)
{
	return(((COORD_COMPOSITE &)coord).Sub.Y.Sub.Lepton);
}


/***********************************************************************************************
 * XYP_Coord -- Combine pixel values into a coordinate.                                        *
 *                                                                                             *
 *    This will convert X and Y pixel values into a coordinate. Primarily this is used for     *
 *    converting mouse clicks into coordinate values.                                          *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y pixel coordinates to convert. Origin is upper left corner.    *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate that most closely corresponds to the pixel values      *
 *          specified.                                                                         *
 *                                                                                             *
 * WARNINGS:   The coordinate is relative to the upper left corner (0,0). To conver the        *
 *             coordinate to a game relative one, it must be biased by the display coordinate  *
 *             of the tactical map and the screen position of the tactical display.            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE XYP_Coord(int x, int y)
{
	COORD_COMPOSITE coord;

	coord.Sub.X.Raw = Pixel_To_Lepton(x);
	coord.Sub.Y.Raw = Pixel_To_Lepton(y);
	return(coord.Coord);
}


/***********************************************************************************************
 * Cell_Coord -- Convert a cell to a coordinate value.                                         *
 *                                                                                             *
 *    This routine will convert the specified cell into a coordinat value. The coordinate      *
 *    will refer to the center of the cell specified.                                          *
 *                                                                                             *
 * INPUT:   cell  -- The cell to convert into a coordinate.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate that refers to the center of the cell specified.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE Cell_Coord(CELL cell)
{
	COORD_COMPOSITE coord;

	coord.Sub.X.Sub.Cell = (unsigned char)(((CELL_COMPOSITE &)cell).Sub.X);
	coord.Sub.X.Sub.Lepton = (unsigned char)(CELL_LEPTON_W / 2);
	coord.Sub.Y.Sub.Cell = (unsigned char)(((CELL_COMPOSITE &)cell).Sub.Y);
	coord.Sub.Y.Sub.Lepton = (unsigned char)(CELL_LEPTON_W / 2);
	return(coord.Coord);
}


/***********************************************************************************************
 * Coord_Snap -- Coerce coordinate to refer to center of a cell.                               *
 *                                                                                             *
 *    This routine will take the specified coordinate and force it to refer to the center of   *
 *    the cell.                                                                                *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to modify.                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the specified coordinate after it has been modified to refer to the   *
 *          center of the cell.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE Coord_Snap(COORDINATE coord)
{
	((COORD_COMPOSITE &)coord).Sub.X.Sub.Lepton = CELL_LEPTON_W/2;
	((COORD_COMPOSITE &)coord).Sub.Y.Sub.Lepton = CELL_LEPTON_W/2;
	return(coord);
}


/***********************************************************************************************
 * Coord_Fraction -- Discards all but the sub-cell components of the coordinate.               *
 *                                                                                             *
 *    Use this routine to discard the cell components of the coordinate, leaving only the      *
 *    sub-cell component.                                                                      *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to modify.                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with just the sub-cell components intact from the supplied coordinate.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE Coord_Fraction(COORDINATE coord)
{
	((COORD_COMPOSITE &)coord).Sub.X.Sub.Cell = 0;
	((COORD_COMPOSITE &)coord).Sub.Y.Sub.Cell = 0;
	return(coord);
}


/***********************************************************************************************
 * Coord_Whole -- Discards the sub-cell components of the coordinate.                          *
 *                                                                                             *
 *    This routine will discard the sub-cell components, leaving only the whole cell portion.  *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to modify.                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with only the whole cell components of the coordinate intact. The          *
 *          resulting coordinate will refer to the upper left corner of the cell.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE Coord_Whole(COORDINATE coord)
{
	((COORD_COMPOSITE &)coord).Sub.X.Sub.Lepton = 0;
	((COORD_COMPOSITE &)coord).Sub.Y.Sub.Lepton = 0;
	return(coord);
}


/***********************************************************************************************
 * Coord_Add -- Adds coordinates together.                                                     *
 *                                                                                             *
 *    This routine will add one coordinate to another. Actually, it adds the X and Y components*
 *    separately (signed) and then recombines them back into a coordinate.                     *
 *                                                                                             *
 * INPUT:   coord1   -- One coordinate to add.                                                 *
 *                                                                                             *
 *          coord2   -- The other coordinate to add.                                           *
 *                                                                                             *
 * OUTPUT:  Returns with the logical add of the two coordinates.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE Coord_Add(COORDINATE coord1, COORDINATE coord2)
{
	COORD_COMPOSITE coord;

	coord.Sub.X.Raw = (LEPTON)((int)(short)((COORD_COMPOSITE &)coord1).Sub.X.Raw + (int)(short)((COORD_COMPOSITE &)coord2).Sub.X.Raw);
	coord.Sub.Y.Raw = (LEPTON)((int)(short)((COORD_COMPOSITE &)coord1).Sub.Y.Raw + (int)(short)((COORD_COMPOSITE &)coord2).Sub.Y.Raw);
	return(coord.Coord);
}


/***********************************************************************************************
 * Coord_Sub -- Subtracts one coordinate from another.                                         *
 *                                                                                             *
 *    This routine will subtract one coordinate from the other. The coordinates are broken     *
 *    up into their X and Y components, the subtraction is performed, and then they are        *
 *    recombined back into a coordinate to be returned.                                        *
 *                                                                                             *
 * INPUT:   coord1   -- The coordinate to be subtracted from.                                  *
 *                                                                                             *
 *          coord2   -- The coordinate to subtract.                                            *
 *                                                                                             *
 * OUTPUT:  Returns with the result of subtracting coord2 from coord1.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE Coord_Sub(COORDINATE coord1, COORDINATE coord2)
{
	COORD_COMPOSITE coord;

	coord.Sub.X.Raw = (LEPTON)((int)(short)((COORD_COMPOSITE &)coord1).Sub.X.Raw - (int)(short)((COORD_COMPOSITE &)coord2).Sub.X.Raw);
	coord.Sub.Y.Raw = (LEPTON)((int)(short)((COORD_COMPOSITE &)coord1).Sub.Y.Raw - (int)(short)((COORD_COMPOSITE &)coord2).Sub.Y.Raw);
	return(coord.Coord);
}


/***********************************************************************************************
 * Coord_Mid -- Finds the midpoint between two coordinates.                                    *
 *                                                                                             *
 *    This will find the coordinate that is exactly between the two coordinates specified.     *
 *                                                                                             *
 * INPUT:   coord1   -- The first coordinate.                                                  *
 *                                                                                             *
 *          coord2   -- The second coordinate.                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the midpoint between the two coordinates.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE Coord_Mid(COORDINATE coord1, COORDINATE coord2)
{
	COORD_COMPOSITE coord;

	coord.Sub.X.Raw = (LEPTON)(((int)((COORD_COMPOSITE &)coord1).Sub.X.Raw + (int)((COORD_COMPOSITE &)coord2).Sub.X.Raw) / 2);
	coord.Sub.Y.Raw = (LEPTON)(((int)((COORD_COMPOSITE &)coord1).Sub.Y.Raw + (int)((COORD_COMPOSITE &)coord2).Sub.Y.Raw) / 2);
	return(coord.Coord);
}


/***********************************************************************************************
 * Dir_Facing -- Convert a DirType into a FacingType value.                                    *
 *                                                                                             *
 *    Use this routine to convert the specified DirType value into the closest FacingType      *
 *    value that matches it.                                                                   *
 *                                                                                             *
 * INPUT:   facing   -- The DirType to convert.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with a FacingType value that most closely matches the DirType specified.   *
 *                                                                                             *
 * WARNINGS:   Precision of direction is lost by this transformation.                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline FacingType Dir_Facing(DirType facing)
{
	return (FacingType)(((unsigned char)((int)facing+0x10)&0xFF)>>5);
}


/***********************************************************************************************
 * Facing_Dir -- Convert a FacingType into a DirType.                                          *
 *                                                                                             *
 *    This will conver the specified FacingType value into the DirType that exactly matches    *
 *    it.                                                                                      *
 *                                                                                             *
 * INPUT:   facing   -- The FacingType to convert.                                             *
 *                                                                                             *
 * OUTPUT:  Returns with the DirType that exactly matches the facing.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline DirType Facing_Dir(FacingType facing)
{
	return (DirType)((int)facing << 5);
}


/***********************************************************************************************
 * Dir_To_16 -- Convert a facing to a 0..15 value.                                             *
 *                                                                                             *
 *    Use this routine to convert a DirType into a 0 through 15 value.                         *
 *                                                                                             *
 * INPUT:   facing   -- The DirType to convert into a 0..15 value.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the facing converted into a value where 0 equals North, 4 equals      *
 *          East, 8 equals South, etc.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int Dir_To_16(DirType facing)
{
	return Facing16[facing];
}


/***********************************************************************************************
 * Dir_To_32 -- Convert a DirType into a 0..31 value.                                          *
 *                                                                                             *
 *    This will convert the DirType specified, into a 0 through 31 value where zero is North,  *
 *    and rotates clockwise. The return value is baised to take into consideration the         *
 *    distortion caused by 3D studio upon the game vehicle objects.                            *
 *                                                                                             *
 * INPUT:   facing   -- The DirType to convert.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the facing converted into a value from zero to 31.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int Dir_To_32(DirType facing)
{
	return Facing32[facing];
}


/***********************************************************************************************
 * Direction256 -- Calculate the facing value from one coordinate to another.                  *
 *                                                                                             *
 *    This will calculate the facing from the first coordinate to the second.                  *
 *                                                                                             *
 * INPUT:   coord1   -- The first coordinate that facing will be calculated from.              *
 *                                                                                             *
 *          coord2   -- The second coordinate that facing will be calcuated to.                *
 *                                                                                             *
 * OUTPUT:  Returns with the DirType that is the facing from coord1 to coord2.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline DirType Direction256(COORDINATE coord1, COORDINATE coord2)
{
	return (Desired_Facing256(Coord_X(coord1), Coord_Y(coord1), Coord_X(coord2), Coord_Y(coord2)));
}


/***********************************************************************************************
 * Direction -- Determines the facing value from one coordinate to another.                    *
 *                                                                                             *
 *    This will determine the DirType from the first coordinate to the second.                 *
 *                                                                                             *
 * INPUT:   coord1   -- The first coordinate that facing will be calculated from.              *
 *                                                                                             *
 *          coord2   -- The second coordinate to calculate facing to.                          *
 *                                                                                             *
 * OUTPUT:  Returns with the DirType that represents the facing from coordinate 1 to coordinate*
 *          2.                                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline DirType Direction(COORDINATE coord1, COORDINATE coord2)
{
	return (Desired_Facing256(Coord_X(coord1), Coord_Y(coord1), Coord_X(coord2), Coord_Y(coord2)));
}


/***********************************************************************************************
 * Direction8 -- Fetches the direction from one coordinate to another.                         *
 *                                                                                             *
 *    This will calculate the facing from the first coordinate to the second. The return value *
 *    is of limited accuracy, but the calculation is fast.                                     *
 *                                                                                             *
 * INPUT:   coord1   -- The coordinate to calculate the facing from.                           *
 *                                                                                             *
 *          coord2   -- The coordinate to figure the facing to.                                *
 *                                                                                             *
 * OUTPUT:  Returns with the DirType to get from coordinate 1 to coordinate 2.                 *
 *                                                                                             *
 * WARNINGS:   The return DirType is only accurate to the 8 primary compass rose directions.   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline DirType Direction8(COORDINATE coord1, COORDINATE coord2)
{
	return (Desired_Facing8(Coord_X(coord1), Coord_Y(coord1), Coord_X(coord2), Coord_Y(coord2)));
}


/***********************************************************************************************
 * Direction -- Calculates the DirType from one cell to another.                               *
 *                                                                                             *
 *    This routine will calculate the facing to get from one cell to another. Since dealing    *
 *    with cells is much less precise than with coordinates, the return value is only          *
 *    accurate to 8 facings.                                                                   *
 *                                                                                             *
 * INPUT:   cell1 -- The cell to calculate the DirType from.                                   *
 *                                                                                             *
 *          cell2 -- The cell to calculate the DirType to.                                     *
 *                                                                                             *
 * OUTPUT:  Returns with the DirType to get from the first cell to the second.                 *
 *                                                                                             *
 * WARNINGS:   The return value is only accurate to the 8 primary compass rose directions.     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline DirType Direction(CELL cell1, CELL cell2)
{
	return (Desired_Facing8(Cell_X(cell1), Cell_Y(cell1), Cell_X(cell2), Cell_Y(cell2)));
}


/***********************************************************************************************
 * Adjacent_Cell -- Calculate the adjacent cell in the direction specified.                    *
 *                                                                                             *
 *    This will coerce the coordinate specified so that it will refer to the immediately       *
 *    adjacent cell in the direction specified.                                                *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to calculate the adjacency from.                           *
 *                                                                                             *
 *          dir   -- The direction to travel to calculate the adjacent cell.                   *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate the refers to the adjacent cell in the direciton       *
 *          specified.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE Adjacent_Cell(COORDINATE coord, FacingType dir)
{
	return (Coord_Snap(Coord_Add(AdjacentCoord[(int)dir & 0x07], coord)));
}


/***********************************************************************************************
 * Adjacent_Cell -- Calculate the adjacent cell in the direction specified.                    *
 *                                                                                             *
 *    This will coerce the coordinate specified so that it will refer to the immediately       *
 *    adjacent cell in the direction specified.                                                *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to calculate the adjacency from.                           *
 *                                                                                             *
 *          dir   -- The direction to travel to calculate the adjacent cell.                   *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate the refers to the adjacent cell in the direciton       *
 *          specified.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline COORDINATE Adjacent_Cell(COORDINATE coord, DirType dir)
{
	return Adjacent_Cell(coord, Dir_Facing(dir));
}


/***********************************************************************************************
 * Adjacent_Cell -- Calculate the adjacent cell in the direction specified.                    *
 *                                                                                             *
 *    This routine will take the specified cell and coerce it to refer to the immediately      *
 *    adjacent cell in the direction specified.                                                *
 *                                                                                             *
 * INPUT:   cell  -- The cell to coerce into an adjacent cell.                                 *
 *                                                                                             *
 *          dir   -- The direction to determine the adjacent cell.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the cell value that represents the adjacent cell in the direction     *
 *          specified.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline CELL Adjacent_Cell(CELL cell, FacingType dir)
{
	return (CELL)(cell + AdjacentCell[dir]);
}


/***********************************************************************************************
 * Adjacent_Cell -- Calculate the adjacent cell in the direction specified.                    *
 *                                                                                             *
 *    This routine will take the specified cell and coerce it to refer to the immediately      *
 *    adjacent cell in the direction specified.                                                *
 *                                                                                             *
 * INPUT:   cell  -- The cell to coerce into an adjacent cell.                                 *
 *                                                                                             *
 *          dir   -- The direction to determine the adjacent cell.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the cell value that represents the adjacent cell in the direction     *
 *          specified.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline CELL Adjacent_Cell(CELL cell, DirType dir)
{
	return (CELL)(cell + AdjacentCell[Dir_Facing(dir)]);
}


/***********************************************************************************************
 * Dir_To_8 -- Convert a DirType into a value from 0 to 7.                                     *
 *                                                                                             *
 *    This routine will convert a DirType value into a facing number from 0 to 7.              *
 *                                                                                             *
 * INPUT:   facing   -- The DirType to convert.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the DirType converted to a number from 0 to 7 with 0 being North and  *
 *          rotating clockwise.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline FacingType Dir_To_8(DirType facing)
{
	return (FacingType)(((unsigned char)((int)facing|0x10))>>5);
}


/***********************************************************************************************
 * Text_String -- Convert a text number into a text pointer.                                   *
 *                                                                                             *
 *    This routine will convert text numbers (as generated elsewhere) into an actual text      *
 *    pointer that can be used for normal purposes.                                            *
 *                                                                                             *
 * INPUT:   string   -- The text number to extract a pointer to.                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the text that represents the text number specified.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline char const * Text_String(int string)
{
#ifdef FIXIT_NAME_OVERRIDE
	if (string < 0 && abs(string) < ARRAY_SIZE(NameOverride)) {
		return(NameOverride[-(string+1)]);
	}
#endif

	if (string < 1000) return(Extract_String(SystemStrings, string));
	return(Extract_String(DebugStrings, string-1000));
}


/***********************************************************************************************
 * Random_Pick -- Pick a random number in a specified range.                                   *
 *                                                                                             *
 *    This routine is used to pick a game influencing random number between (inclusive) the    *
 *    range specified.                                                                         *
 *                                                                                             *
 * INPUT:   a  -- Low limit of range to pick from.                                             *
 *                                                                                             *
 *          b  -- High limit of range to pick from.                                            *
 *                                                                                             *
 * OUTPUT:  Returns with a random number picked between (inclusive) the range of values        *
 *          specified.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T> inline T Random_Pick(T a, T b)
{
	return T(Scen.RandomNumber((int)a, (int)b));
};


/***********************************************************************************************
 * Percent_Chance -- Calculate a percentage chance event.                                      *
 *                                                                                             *
 *    This will calculate a percentage chance and return with 'true' as likely as the          *
 *    chance value would occur (or less) on a random pick from 1 to 100. Thus a                *
 *    Percent_Chance(50) would return 'true' 50 percent of the time. Percent_Chance(25) would  *
 *    return 'true' 25% of the time, etc.                                                      *
 *                                                                                             *
 * INPUT:   percent  -- The percent value to calculate the chance upon.                        *
 *                                                                                             *
 * OUTPUT:  Returns with 'true' in the same percentage as the percentage number supplied.      *
 *                                                                                             *
 * WARNINGS:   This affects the game syncronization random number generator and should be used *
 *             for those events that could affect the game engine.                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline bool Percent_Chance(int percent)
{
	return (Scen.RandomNumber(0, 99) < percent);
}


/***********************************************************************************************
 * Sim_Random_Pick -- Picks a random number that will not affect the game.                     *
 *                                                                                             *
 *    Use this routine to pick a random number such that it will be used so that it won't      *
 *    actually affect the outcome of the game. It is critical to use this routine for any      *
 *    random need that won't be needed on other machines in a multiplayer game. The result     *
 *    can be freely used as long as it doesn't affect the outcome of the game.                 *
 *                                                                                             *
 * INPUT:   a  -- Low range of the random number to pick.                                      *
 *                                                                                             *
 *          b  -- High range of the random number to pick.                                     *
 *                                                                                             *
 * OUTPUT:  Returns with a random number between (inclusive) the range limit values            *
 *          specified.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
extern RandomClass NonCriticalRandomNumber;
template<class T> inline T Sim_Random_Pick(T a, T b)
{
	return T(NonCriticalRandomNumber((int)a, (int)b));
};


/***********************************************************************************************
 * Sim_Percent_Chance -- Calculates a percentage chance event for local events.                *
 *                                                                                             *
 *    This routine is similar to the normal Percent_Chance() routine except that it doesn't    *
 *    alter the main random number gerenator sequence. As such, this routine should be used    *
 *    for those events that should have a random character, but will either not affect the     *
 *    game engine or are only calculated on one machine in a multiplayer game.                 *
 *                                                                                             *
 * INPUT:   percent  -- The percent chance to calculate the possible return of 'true' on.      *
 *                                                                                             *
 * OUTPUT:  Returns 'true' with the same percentage chance as the percent number specified.    *
 *          A percent value of 50 means 50%, 25 means 25%, etc.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline bool Sim_Percent_Chance(int percent)
{
	return (NonCriticalRandomNumber(0, 99) < percent);
}


/***********************************************************************************************
 * Distance -- Finds the distance between two arbitrary points.                                *
 *                                                                                             *
 *    This finds the (Dragon Strike) distance between two arbitrary points in flat space.      *
 *    It does this by adding 1/2 the smaller absolute axis difference to the other absolute    *
 *    axis distance. The result is rough but quick to calculate.                               *
 *                                                                                             *
 * INPUT:   x1,y1 -- Coordinate location for point 1.                                          *
 *                                                                                             *
 *          x2,y2 -- Coordinate location for point 2.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the rough distance between the two points. The value returned is      *
 *          expressed in the same units as the parameters were specified in.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline int Distance(int x1, int y1, int x2, int y2)
{
	int diff1 = y1 - y2;
	if (diff1 < 0) diff1 = -diff1;
	int diff2 = x1 - x2;
	if (diff2 < 0) diff2 = -diff2;
	if (diff1 > diff2) {
		return(diff1 + ((unsigned)diff2 / 2));
	}
	return(diff2 + ((unsigned)diff1 / 2));
}


#endif

