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

/* $Header: /CounterStrike/SHAPEBTN.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SHAPEBTN.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/15/95                                                     *
 *                                                                                             *
 *                  Last Update : September 20, 1995 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ShapeButtonClass::Draw_Me -- Renders the shape button's imagery.                          *
 *   ShapeButtonClass::Set_Shape -- Assigns a shape to this shape button.                      *
 *   ShapeButtonClass::ShapeButtonClass -- Constructor for a shape type button.                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include "shapebtn.h"


/***********************************************************************************************
 * ShapeButtonClass::ShapeButtonClass -- Default Constructor for a shape type button.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   You must call Set_Shape() before using a button constructed with this function, *
 *             and you must set X & Y, and ID.                                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ShapeButtonClass::ShapeButtonClass(void) :
	ToggleClass(0, 0, 0, 0, 0),
	ReflectButtonState(false)
{
}


/***********************************************************************************************
 * ShapeButtonClass::ShapeButtonClass -- Constructor for a shape type button.                  *
 *                                                                                             *
 *    This is the normal constructor for a shape type button. Shape buttons are ones that      *
 *    have their imagery controlled by a shape file. The various states of the button are      *
 *    given a visual form as one of these shapes. Button dimensions are controlled by the      *
 *    first shape.                                                                             *
 *                                                                                             *
 * INPUT:   id    -- The button ID.                                                            *
 *                                                                                             *
 *          shape -- Pointer to the shape file that controls the button's display.             *
 *                                                                                             *
 *          x,y   -- The pixel coordinate of the upper left corner of the button.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The width and height of the shape is controlled by the first shape in the       *
 *             shape file provided. This means that all the shapes in the shape file must be   *
 *             the same size.                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ShapeButtonClass::ShapeButtonClass(unsigned id, void const * shape, int x, int y) :
	ToggleClass(id, x, y, 0, 0),
	ReflectButtonState(false)
{
//	Width = 0;
//	Height = 0;
	Set_Shape(shape);
}


/***********************************************************************************************
 * ShapeButtonClass::Set_Shape -- Assigns a shape to this shape button.                        *
 *                                                                                             *
 *    This routine will assign the specified shape to this shape object.                       *
 *                                                                                             *
 * INPUT:   data  -- Pointer to the shape to assign.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ShapeButtonClass::Set_Shape(void const * data)
{
	ShapeData = data;
	if (ShapeData) {
		Width = Get_Build_Frame_Width(ShapeData);
		Height = Get_Build_Frame_Height(ShapeData);
	}
}


/***********************************************************************************************
 * ShapeButtonClass::Draw_Me -- Renders the shape button's imagery.                            *
 *                                                                                             *
 *    This function is called when the button detects that it must be redrawn. The actual      *
 *    shape to use is controled by the button's state and the shape file provided when then    *
 *    button was constructed.                                                                  *
 *                                                                                             *
 * INPUT:   forced   -- Should the button be redrawn regardless of the redraw flag?            *
 *                                                                                             *
 * OUTPUT:  bool; Was the shape redrawn?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ShapeButtonClass::Draw_Me(int forced)
{
	if (ControlClass::Draw_Me(forced) && ShapeData) {

		/*
		**	Hide the mouse.
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Hide_Mouse(X, Y, X+Width-1, Y+Height-1);
		}

		/*
		**	Draw the body & set text color.
		*/
		int shapenum = 0;
		if (IsDisabled) {
			shapenum = DISABLED_SHAPE;
		} else {

			if (!ReflectButtonState) {

				if (IsPressed) {
					shapenum = DOWN_SHAPE;
				} else {
					shapenum = UP_SHAPE;
				}
			} else {
				shapenum = IsOn;
			}

		}
		CC_Draw_Shape(ShapeData, shapenum, X, Y, WINDOW_MAIN, SHAPE_NORMAL);
		/*
		**	Display the mouse.
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Show_Mouse();
		}
		return(true);
	}
	return(false);
}



