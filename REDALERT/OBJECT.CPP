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

/* $Header: /CounterStrike/OBJECT.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OBJECT.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 29, 1994                                               *
 *                                                                                             *
 *                  Last Update : October 6, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ObjectClass::AI -- Handles generic object AI processing.                                  *
 *   ObjectClass::Active_Click_With -- Dispatches action on the object specified.              *
 *   ObjectClass::Active_Click_With -- Dispatches action on the specified cell.                *
 *   ObjectClass::Attach_Trigger -- Attach specified trigger to object.                        *
 *   ObjectClass::Can_Demolish -- Queries whether this object can be sold back.                *
 *   ObjectClass::Can_Player_Fire -- Can the player give this object an attack mission?        *
 *   ObjectClass::Can_Player_Move -- Can the player give this object a movement mission?       *
 *   ObjectClass::Can_Repair -- Queries whether this object can be repaired.                   *
 *   ObjectClass::Catch_Fire -- Called when animation is attached to this object.              *
 *   ObjectClass::Center_Coord -- Fetches the center coordinate for the object.                *
 *   ObjectClass::Clicked_As_Target -- Triggers target selection animation.                    *
 *   ObjectClass::Debug_Dump -- Displays status of the object class to the mono monitor.       *
 *   ObjectClass::Detach -- Detach the specified target from this object.                      *
 *   ObjectClass::Detach_All -- Removes the object from all tracking systems.                  *
 *   ObjectClass::Do_Shimmer -- Shimmers this object if it is cloaked.                         *
 *   ObjectClass::Docking_Coord -- Fetches the coordinate to dock at this object.              *
 *   ObjectClass::Exit_Coord -- Return with the exit coordinate for this object.               *
 *   ObjectClass::Exit_Object -- Causes the specified object to leave this object.             *
 *   ObjectClass::Fire_Coord -- Fetches the coordinate a projectile will launch from.          *
 *   ObjectClass::Fire_Out -- Informs object that attached animation has finished.             *
 *   ObjectClass::Get_Mission -- Fetches the current mission of this object.                   *
 *   ObjectClass::Get_Ownable -- Fetches the house owner legality options for this object.     *
 *   ObjectClass::Hidden -- Called when this object becomes hidden from the player.            *
 *   ObjectClass::In_Range -- Determines if the coordinate is within weapon range.             *
 *   ObjectClass::In_Which_Layer -- Fetches what layer this object is located in.              *
 *   ObjectClass::Init -- Initializes the basic object system.                                 *
 *   ObjectClass::Limbo -- Brings the object into a state of limbo.                            *
 *   ObjectClass::Look -- Called when this object needs to reveal terrain.                     *
 *   ObjectClass::Mark -- Handles basic marking logic.                                         *
 *   ObjectClass::Mark_For_Redraw -- Marks object and system for redraw.                       *
 *   ObjectClass::Move -- Moves (by force) the object in the desired direction.                *
 *   ObjectClass::Name -- Fetches the identification name of this object.                      *
 *   ObjectClass::ObjectClass -- Default constructor for objects.                              *
 *   ObjectClass::Paradrop -- Unlimbos object in paradrop mode.                                *
 *   ObjectClass::Passive_Click_With -- Right mouse button click process.                      *
 *   ObjectClass::Receive_Message -- Processes an incoming radio message.                      *
 *   ObjectClass::Record_The_Kill -- Records this object as killed by the specified object.    *
 *   ObjectClass::Render -- Displays the object onto the map.                                  *
 *   ObjectClass::Render_Coord -- Fetches the coordinate to draw this object at.               *
 *   ObjectClass::Repair -- Handles object repair control.                                     *
 *   ObjectClass::Revealed -- Reveals this object to the house specified.                      *
 *   ObjectClass::Scatter -- Tries to scatter this object.                                     *
 *   ObjectClass::Select -- Try to make this object the "selected" object.                     *
 *   ObjectClass::Sell_Back -- Sells the object -- if possible.                                *
 *   ObjectClass::Sort_Y -- Returns the coordinate used for display order sorting.             *
 *   ObjectClass::Take_Damage -- Applies damage to the object.                                 *
 *   ObjectClass::Target_Coord -- Fetches the coordinate if this object is a target.           *
 *   ObjectClass::Unlimbo -- Brings the object into the game system.                           *
 *   ObjectClass::Unselect -- This will un-select the object if it was selected.               *
 *   ObjectClass::Value -- Fetches the target value of this object.                            *
 *   ObjectClass::Weapon_Range -- Returns the weapon range for the weapon specified.           *
 *   ObjectClass::What_Action -- Determines what action to perform on specified object.        *
 *   ObjectClass::What_Action -- Returns with the action to perform for this object.           *
 *   ObjectTypeClass::Cost_Of -- Returns the cost to buy this unit.                            *
 *   ObjectTypeClass::Dimensions -- Gets the dimensions of the object in pixels.               *
 *   ObjectTypeClass::Get_Cameo_Data -- Fetches pointer to cameo data for this object type.    *
 *   ObjectTypeClass::Max_Pips -- Fetches the maximum pips allowed for this object.            *
 *   ObjectTypeClass::ObjectTypeClass -- Normal constructor for object type class objects.     *
 *   ObjectTypeClass::Occupy_List -- Returns with simple occupation list for object.           *
 *   ObjectTypeClass::One_Time -- Handles one time processing for object types.                *
 *   ObjectTypeClass::Overlap_List -- Returns a pointer to a simple overlap list.              *
 *   ObjectTypeClass::Time_To_Build -- Fetches the time to construct this object.              *
 *   ObjectTypeClass::Who_Can_Build_Me -- Determine what building can build this object type.  *
 *   ObjectTypeClass::Who_Can_Build_Me -- Finds the factory building that can build this object*
 *   ObjectClass::Get_Image_Data -- Fetches the image data to use for this object.             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

/*
**	Selected objects have a special marking box around them. This is the shapes that are
**	used for this purpose.
*/
void const * ObjectTypeClass::SelectShapes = 0;

void const * ObjectTypeClass::PipShapes = 0;



/***********************************************************************************************
 * ObjectClass::ObjectClass -- Default constructor for objects.                                *
 *                                                                                             *
 *    This is the default constructor for objects. It is called as an inherent part of the     *
 *    construction process for all the normal game objects instantiated. It serves merely to   *
 *    initialize the object values to a common (default) state.                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Objects always start in a state of limbo. They must be Unlimbo()ed before they  *
 *             can be used.                                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass::ObjectClass(RTTIType rtti, int id) :
	AbstractClass(rtti, id),
	IsDown(false),
	IsToDamage(false),
	IsToDisplay(false),
	IsInLimbo(true),
	IsSelected(false),
	IsAnimAttached(false),
	IsFalling(false),
	Riser(0),
	Next(0),
	Trigger(NULL),
	Strength(255),
	IsSelectedMask(0)		// Mask showing who has selected this object
{
}


/***********************************************************************************************
 * ObjectClass::Get_Image_Data -- Fetches the image data to use for this object.               *
 *                                                                                             *
 *    This routine will return with a pointer to the image data that should be used when       *
 *    this object is drawn.                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the shape data for this object.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * ObjectClass::Get_Image_Data(void) const
{
	return(Class_Of().Get_Image_Data());
}


/***********************************************************************************************
 * ObjectClass::Name -- Fetches the identification name of this object.                        *
 *                                                                                             *
 *    This routine will return a pointer to the identifier name for this object. This name     *
 *    is usually short and is used in the INI files to identify this object.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the text identifier name of this object.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * ObjectClass::Name(void) const
{
	return(Class_Of().Name());
}


/***********************************************************************************************
 * ObjectClass::Exit_Coord -- Return with the exit coordinate for this object.                 *
 *                                                                                             *
 *    Ths exit coordinate is the location that a piggy back or newly produced object will      *
 *    appear at when it exits this object. Transports and factory buildings will utilize this  *
 *    routine.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate that an object will appear at when exiting this        *
 *          object.                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE ObjectClass::Exit_Coord(void) const
{
	return(Center_Coord());
}


/***********************************************************************************************
 * ObjectClass::AI -- Handles generic object AI processing.                                    *
 *                                                                                             *
 *    This routine is used to handle the AI processing that occurs for all object types.       *
 *    Typically, this isn't much, but there is the concept of falling that all objects can     *
 *    be subjected to (e.g., grenades).                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/07/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::AI(void)
{
	assert(this != 0);
	assert(IsActive);

	/*
	**	Falling logic is handled here.
	*/
	if (IsFalling) {
		LayerType layer = In_Which_Layer();

		Height += Riser;
		if (Height <= 0) {
			Height = 0;
			IsFalling = false;
			Per_Cell_Process(PCP_END);

			Shorten_Attached_Anims(this);
		}
		if (IsAnimAttached) {
			Riser -= 1;
			Riser = max(Riser, -3);
		} else {
			Riser -= Rule.Gravity;
//			Riser -= GRAVITY;
			Riser = max(Riser, -100);
		}

		if (layer != In_Which_Layer()) {
			Map.Remove(this, layer);
			Map.Submit(this, In_Which_Layer());

			if (Class_Of().IsFootprint) {
				if (In_Which_Layer() == LAYER_GROUND) {
					Map.Place_Down(Coord_Cell(Center_Coord()), this);
				} else {
					Map.Pick_Up(Coord_Cell(Center_Coord()), this);
				}
			}
		}
	}
}


/***********************************************************************************************
 * ObjectClass::What_Action -- Determines what action to perform on specified object.          *
 *                                                                                             *
 *    This routine will return that action that this object could perform if the mouse were    *
 *    clicked over the object specified.                                                       *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object to check this object against when determining    *
 *                      the action to perform.                                                 *
 *                                                                                             *
 * OUTPUT:  It returns that action that will be performed if the mouse were clicked over the   *
 *          object. Since non-derived objects cannot do anything, and cannot even be           *
 *          instantiated, this routine will always return ACTION_NONE.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType ObjectClass::What_Action(ObjectClass const *) const
{
	assert(this != 0);
	assert(IsActive);

	return(ACTION_NONE);
}


/***********************************************************************************************
 * ObjectClass::What_Action -- Returns with the action to perform for this object.             *
 *                                                                                             *
 *    This routine is called when information on a potential action if the mouse were clicked  *
 *    on the cell specified. This routine merely serves as a virtual placeholder so that       *
 *    object types that can actually perform some action will override this routine to provide *
 *    true functionality.                                                                      *
 *                                                                                             *
 * INPUT:   cell  -- The cell that the mouse is over and might be clicked on.                  *
 *                                                                                             *
 * OUTPUT:  Returns with the action that this object would try to perform if the mouse were    *
 *          clicked. Since objects at this level have no ability to do anything, this routine  *
 *          will always returns ACTION_NONE unless it is overridden.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType ObjectClass::What_Action(CELL) const
{
	assert(this != 0);
	assert(IsActive);

	return(ACTION_NONE);
}


/***********************************************************************************************
 * ObjectClass::In_Which_Layer -- Fetches what layer this object is located in.                *
 *                                                                                             *
 *    The default layer for object location is the LAYER_GROUND. Aircraft will override this   *
 *    routine and make adjustments as necessary according to the aircraft's altitude.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the layer that this object is located in.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
LayerType ObjectClass::In_Which_Layer(void) const
{
	assert(this != 0);
	assert(IsActive);

	if (Height < (FLIGHT_LEVEL - (FLIGHT_LEVEL/3))) {
		return(LAYER_GROUND);
	}
	return(LAYER_TOP);
}


/***********************************************************************************************
 * ObjectClass::Get_Ownable -- Fetches the house owner legality options for this object.       *
 *                                                                                             *
 *    This routine will return the ownable bits for this object. Objects at this level can't   *
 *    really be owned by anyone, but return the full spectrum of legality just to be safe.     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the ownable flags (as a combined bitfield) for this object.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ObjectClass::Get_Ownable(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(HOUSEF_ALLIES | HOUSEF_SOVIET | HOUSEF_OTHERS);
}


/***********************************************************************************************
 * ObjectClass::Can_Repair -- Queries whether this object can be repaired.                     *
 *                                                                                             *
 *    Most objects cannot be repaired. This routine defaults to returning "false", but is      *
 *    overridden by derived functions defined by object types that can support repair.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Can this object be repaired?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Can_Repair(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(false);
}


/***********************************************************************************************
 * ObjectClass::Can_Demolish -- Queries whether this object can be sold back.                  *
 *                                                                                             *
 *    This routine is used to determine if this object can be sold. Most objects cannot be     *
 *    but for those objects that can, this routine will be overridden as necessary.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Can this object be sold back? Typically, the answer is no.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Can_Demolish(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(false);
}


bool ObjectClass::Can_Demolish_Unit(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(false);
}


/***********************************************************************************************
 * ObjectClass::Can_Player_Fire -- Can the player give this object an attack mission?          *
 *                                                                                             *
 *    This routine is used to determine if attacking is an option under player control with    *
 *    respect to this unit. This routine will be overridden as necessary for those objects     *
 *    that have the ability to attack.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Can this object be given an attack order by the player?                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Can_Player_Fire(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(false);
}


/***********************************************************************************************
 * ObjectClass::Can_Player_Move -- Can the player give this object a movement mission?         *
 *                                                                                             *
 *    This routine is used to determine if the player has the ability to command this object   *
 *    with a movement mission. This routine will be overridden as necessary to support this    *
 *    ability.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Can this object be given a movement mission by the player?                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Can_Player_Move(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(false);
}


/***********************************************************************************************
 * ObjectClass::Target_Coord -- Fetches the coordinate if this object is a target.             *
 *                                                                                             *
 *    When the coordinate to use when firing at this object is needed, this routine will       *
 *    provide it. Normal objects just use the center of the object for this, but there are     *
 *    some more sophisticated objects that are not fired upon the center.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to fire at if this object is a target.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE ObjectClass::Target_Coord(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(Coord_Add(XY_Coord(0, -Height), Center_Coord()));
//	return(Center_Coord());
}


/***********************************************************************************************
 * ObjectClass::Center_Coord -- Fetches the center coordinate for the object.                  *
 *                                                                                             *
 *    This routine will return the center coordinate for the object. The center coordinate is  *
 *    typically the coordinate recorded in the object structure. Exceptions to this include    *
 *    the trees and other terrain elements.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the coordinate that is considered the center point of this object.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE ObjectClass::Center_Coord(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(Coord);
}


/***********************************************************************************************
 * ObjectClass::Render_Coord -- Fetches the coordinate to draw this object at.                 *
 *                                                                                             *
 *    This routine will return the coordinate to base the drawing of this object's graphic     *
 *    at. This is adjusted according to the nature of the graphic associated with this         *
 *    object.                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the coordinate to draw the graphic of this object at.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE ObjectClass::Render_Coord(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(Center_Coord());
}


/***********************************************************************************************
 * ObjectClass::Docking_Coord -- Fetches the coordinate to dock at this object.                *
 *                                                                                             *
 *    This routine returns the coordinate that a potential docking object should home in on.   *
 *    Typically, this the center of the object, but in certain cases it is adjusted off center *
 *    according to the object type. An example of this would be the airfield.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate that a docking object should head for.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE ObjectClass::Docking_Coord(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(Center_Coord());
}


/***********************************************************************************************
 * ObjectClass::Sort_Y -- Returns the coordinate used for display order sorting.               *
 *                                                                                             *
 *    This routine will return the value to be used for object sorting. The sorting ensures    *
 *    that the object are rendered from a top to bottom order. Certain object use a sorting    *
 *    value different from their center coordinate. This is true if the object "touches the    *
 *    ground" at a point that is different from the object's center point.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the value to use as the Y sorting value.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE ObjectClass::Sort_Y(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(Coord);
}


/***********************************************************************************************
 * ObjectClass::Fire_Coord -- Fetches the coordinate a projectile will launch from.            *
 *                                                                                             *
 *    For those objects that fire, the coordinate that the projectile it launches will appear  *
 *    at the location specified by the return value from this function.                        *
 *                                                                                             *
 * INPUT:   which -- Which weapon to consider when determining fire coordinate?                *
 *                   0: primary weapon                                                         *
 *                   1: secondary weapon                                                       *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate that a launched projectile will appear at if this      *
 *          object were to fire the weapon specified.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE ObjectClass::Fire_Coord(int ) const
{
	assert(this != 0);
	assert(IsActive);

	return(Coord);
}


/***********************************************************************************************
 * ObjectClass::Record_The_Kill -- Records this object as killed by the specified object.      *
 *                                                                                             *
 *    This routine is called when this object is killed. If the source of the death is known,  *
 *    then a pointer to the responsible object is provided as a parameter.                     *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the cause of this unit's death.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Record_The_Kill(TechnoClass * )
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::Do_Shimmer -- Shimmers this object if it is cloaked.                           *
 *                                                                                             *
 *    When an object is cloaked, there are several conditions that would cause it to shimmer   *
 *    and thus reveal itself. When such a condition arrises, this function is called. If the   *
 *    object is cloaked, then it will shimmer. At this derivation level, cloaking is           *
 *    undefined. Objects that can cloak will override this function as necessary.              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Do_Shimmer(void)
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::Exit_Object -- Causes the specified object to leave this object.               *
 *                                                                                             *
 *    This routine is called, typically, by a transport building type that requires an object  *
 *    to leave it. This routine will place the object at a suitable location or return         *
 *    a value indicating why not.                                                              *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that wishes to leave this object.                *
 *                                                                                             *
 * OUTPUT:  Returns the success value of the attempt:                                          *
 *             0: Object could not be placed -- ever                                           *
 *             1: Object placement is temporarily delayed -- try again later.                  *
 *             2: Object placement proceeded normally                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ObjectClass::Exit_Object(TechnoClass *)
{
	assert(this != 0);
	assert(IsActive);
	return(0);
}


/***********************************************************************************************
 * ObjectClass::Hidden -- Called when this object becomes hidden from the player.              *
 *                                                                                             *
 *    This routine is called when the object becomes hidden from the player. It can result in  *
 *    lost targeting and tracking abilities with respect to the hidden object.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Hidden(void)
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::Look -- Called when this object needs to reveal terrain.                       *
 *                                                                                             *
 *    This routine is called when the object needs to look around the terrain. For player      *
 *    owned objects, the terrain is revealed. For non-player objects, not effect occurs.       *
 *                                                                                             *
 * INPUT:   incremental -- If true, then the looking algorithm will only examine the edges     *
 *                         of the sight range. This is more efficient and work well if the     *
 *                         object has only moved one cell since the last time it has performed *
 *                         the look operation.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This can be a time consuming operation. Call only when necessary.               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Look(bool )
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::Active_Click_With -- Dispatches action on the object specified.                *
 *                                                                                             *
 *    This routine is called when this object is selected and the mouse was clicked on the     *
 *    tactical map. An action is required from the object. The object that the mouse was       *
 *    over and the tentative action to perform are provided as parameters.                     *
 *                                                                                             *
 * INPUT:   action   -- The requested action to perform with the object specified.             *
 *                                                                                             *
 *          object   -- The object that the action should be performed on. This object is      *
 *                      what the mouse was over when the click occurred.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Active_Click_With(ActionType , ObjectClass *)
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::Active_Click_With -- Dispatches action on the specified cell.                  *
 *                                                                                             *
 *    This routine will dispatch the action requested upon the cell specified. It is called    *
 *    when the mouse is clicked over a cell while this object is selected.                     *
 *                                                                                             *
 * INPUT:   action   -- The action to perform.                                                 *
 *                                                                                             *
 *          cell     -- The location (cell) to perform this action upon.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Active_Click_With(ActionType , CELL )
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::Clicked_As_Target -- Triggers target selection animation.                      *
 *                                                                                             *
 *    This routine is called when this object is the target of some player click action.       *
 *    For more sophisticated object, this will trigger the object to begin flashing a few      *
 *    times. At this level, no action is performed.                                            *
 *                                                                                             *
 * INPUT:   flashes  -- The requested number of times to flash this object.                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Clicked_As_Target(HousesType house, int) // 2019/09/20 JAS - Added record of who clicked on the object
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::In_Range -- Determines if the coordinate is within weapon range.               *
 *                                                                                             *
 *    This routine will determine if the specified coordinate is within weapon range.          *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to check to see if it is within weapon range.              *
 *                                                                                             *
 *          which -- The weapon to check against.                                              *
 *                   0: primary weapon                                                         *
 *                   1: secondary weapon                                                       *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified coordinate within weapon range for the weapon type          *
 *                specified?                                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::In_Range(COORDINATE , int) const
{
	assert(this != 0);
	assert(IsActive);

	return(false);
}


/***********************************************************************************************
 * ObjectClass::Weapon_Range -- Returns the weapon range for the weapon specified.             *
 *                                                                                             *
 *    This routine will return the weapon range according to the type of weapon specified.     *
 *                                                                                             *
 * INPUT:   which -- The weapon to fetch the range from.                                       *
 *                   0: primary weapon                                                         *
 *                   1: secondary weapon                                                       *
 *                                                                                             *
 * OUTPUT:  Returns with the range (in leptons) of the weapon specified.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ObjectClass::Weapon_Range(int) const
{
	assert(this != 0);
	assert(IsActive);

	return(0);
}


/***********************************************************************************************
 * ObjectClass::Scatter -- Tries to scatter this object.                                       *
 *                                                                                             *
 *    This routine is used when the object should scatter from its current location. It        *
 *    applies to units that have the ability to move.                                          *
 *                                                                                             *
 * INPUT:   coord -- The source of the threat that is causing the scatter.                     *
 *                                                                                             *
 *          forced-- Whether this scatter attempt is serious and scattering should occur       *
 *                   regardless of what is doing now.                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This may or may not cause the object to scatter. It is merely a request to the  *
 *             object that it would be good if it were to scatter.                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Scatter(COORDINATE , bool, bool)
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::Catch_Fire -- Called when animation is attached to this object.                *
 *                                                                                             *
 *    This routine is called when an animation is attached to this object. It might be a       *
 *    fire animation (hence the name), but it might also be smoke or any other animation       *
 *    as well.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the object caught on fire by this routine? Actually, this is really      *
 *                the answer to this question; "Is this animation attaching to this object     *
 *                that doesn't already have an animation attached?"                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Catch_Fire(void)
{
	assert(this != 0);
	assert(IsActive);

	return false;
}


/***********************************************************************************************
 * ObjectClass::Fire_Out -- Informs object that attached animation has finished.               *
 *                                                                                             *
 *    This routine is called if there is an attached animation on this object and that         *
 *    animation has finished. Typically, this is necessary for when trees are on fire.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Fire_Out(void)
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::Value -- Fetches the target value of this object.                              *
 *                                                                                             *
 *    This routine will return the target value of this object. The higher the number, the     *
 *    better the object will be as a target. This routine is called when searching for         *
 *    targets. Generic objects have no target potential, and this routine returns zero to      *
 *    reflect that. Other object types will override this routine to return the appropriate    *
 *    target value.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the value of this object as a target. Higher values mean better       *
 *          target.                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ObjectClass::Value(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(0);
}


/***********************************************************************************************
 * ObjectClass::Get_Mission -- Fetches the current mission of this object.                     *
 *                                                                                             *
 *    Generic objects don't have a mission, so this routine will just return MISSION_NONE.     *
 *    However, techno objects do have a mission and this routine is overloaded to handle       *
 *    those objects in order to return the correct mission value.                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current mission being followed by this object.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
MissionType ObjectClass::Get_Mission(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(MISSION_NONE);
}


/***********************************************************************************************
 * ObjectClass::Repair -- Handles object repair control.                                       *
 *                                                                                             *
 *    This routine will control object repair mode. At the object level, no repair is          *
 *    possible, so it is expected that any object that can repair will override this function  *
 *    as necessary.                                                                            *
 *                                                                                             *
 * INPUT:   control  -- The repair control parameter.                                          *
 *                      0  = turn repair off                                                   *
 *                      1  = turn repair on                                                    *
 *                      -1 = toggle repair state                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Repair(int )
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::Sell_Back -- Sells the object -- if possible.                                  *
 *                                                                                             *
 *    This routine is called to sell back the object. Override this routine for the more       *
 *    sophisticated objects that can actually be sold back. Normal objects can't be sold and   *
 *    this routine does nothing as a consequence.                                              *
 *                                                                                             *
 * INPUT:   control  -- How to control the sell state of this object.                          *
 *                      0  = stop selling.                                                     *
 *                      1  = start selling.                                                    *
 *                      -1 = toggle selling state.                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Sell_Back(int )
{
	assert(this != 0);
	assert(IsActive);
}


/***********************************************************************************************
 * ObjectClass::Move -- Moves (by force) the object in the desired direction.                  *
 *                                                                                             *
 *    This routine will instantly move the object one cell in the specified direction. It      *
 *    moves the object by force. This is typically ONLY used by the scenario editor            *
 *    process.                                                                                 *
 *                                                                                             *
 * INPUT:   facing   -- The direction to move the object.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Naturally, this can cause illegal placement situations -- use with caution.     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Move(FacingType facing)
{
	assert(this != 0);
	assert(IsActive);

	COORDINATE	coord;

	Mark(MARK_UP);
	coord = Adjacent_Cell(Coord, facing);
	if (Can_Enter_Cell(Coord_Cell(coord)) == MOVE_OK) {
		Coord = coord;
	}
	Mark(MARK_DOWN);
}


// Object selection list is switched with player context for GlyphX. ST - 4/17/2019 9:42AM
extern void Logic_Switch_Player_Context(ObjectClass *object);

/***********************************************************************************************
 * ObjectClass::Unselect -- This will un-select the object if it was selected.                 *
 *                                                                                             *
 *    This routine brings a currently selected object into an unselected state. This is        *
 *    needed when another object becomes selected as well as if the object is destroyed.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Unselect(void)
{
	assert(this != 0);
	assert(IsActive);

	//if (IsSelected) { 
	// Updated to function for multiplayer - 6/26/2019 JAS
	if (Is_Selected_By_Player()) {
		
		if (In_Which_Layer() == LAYER_GROUND) Mark(MARK_OVERLAP_UP);
		
		IsSelected = false;
		
		// Updated to function for multiplayer - 6/26/2019 JAS
		Set_Unselected_By_Player();
		
		if (In_Which_Layer() == LAYER_GROUND) Mark(MARK_OVERLAP_DOWN);
	}
}


/***********************************************************************************************
 * ObjectClass::Unselect_All_Players --	This will un-select the object if it was selected    *
 *											from all players															  *
 *                                                                                             *
 *    This routine brings a currently selected object into an unselected state for all players.*
 *		This is needed when  the object is destroyed.														  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/2019 JAS : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Unselect_All_Players(void)
{
	CurrentObject.Delete_All(this);

	if (In_Which_Layer() == LAYER_GROUND) {
		Mark(MARK_OVERLAP_UP);
	}

	IsSelected = false;
	IsSelectedMask = 0;

	if (In_Which_Layer() == LAYER_GROUND) {
		Mark(MARK_OVERLAP_DOWN);
	}
}

/***********************************************************************************************
 * ObjectClass::Unselect_All_Players_Except_Owner --	This will un-select the object if it was *
 *											selected for all players except for the object's owner		  *
 *                                                                                             *
 *    This routine brings a currently selected object into an unselected state for all players.*
 *		This is needed when the object cloaks.																	  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/28/2019 JAS : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Unselect_All_Players_Except_Owner(void)
{
	CurrentObject.Delete_All_Except(this, Owner());

	if (In_Which_Layer() == LAYER_GROUND) {
		Mark(MARK_OVERLAP_UP);
	}

	int owner_mask = 1 << Owner();
	if (IsSelectedMask & owner_mask)
	{
		IsSelected = true;
		IsSelectedMask = owner_mask;
	}
	else
	{
		IsSelected = false;
		IsSelectedMask = 0;
	}

	if (In_Which_Layer() == LAYER_GROUND) {
		Mark(MARK_OVERLAP_DOWN);
	}
}

/***********************************************************************************************
 * ObjectClass::Select -- Try to make this object the "selected" object.                       *
 *                                                                                             *
 *    This routine is used to make this object into the one that is "selected". A selected     *
 *    object usually displays a floating bar graph and is available to be given orders from    *
 *    the player's I/O.                                                                        *
 *                                                                                             *
 * INPUT:   allow_mixed -- Allow a mix of player and non-player controlled units?              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1994 JLB : Created.                                                                 *
 *   06/12/1995 JLB : Cannot select a loaner object.                                           *
 *   07/23/1995 JLB : Adds to head or tail depending on leader type flag.                      *
 *=============================================================================================*/
bool ObjectClass::Select(bool allow_mixed)
{
	assert(this != 0);
	assert(IsActive);

	//if (!Debug_Map && (IsSelected || !Class_Of().IsSelectable)) {
	//	return(false);
	//}
	// Updated to function for multiplayer - 6/26/2019 JAS
	if (!Debug_Map && (Is_Selected_By_Player() || !Class_Of().IsSelectable)) {
		return(false);
	}

	if (!Debug_Map && Can_Player_Move() && Is_Techno() && ((TechnoClass *)this)->IsALoaner) {
		return(false);
	}

	/*
	**	Don't allow selection if the object is still in the air.
	*/
	if (Height > 0 && (What_Am_I() == RTTI_UNIT || What_Am_I() == RTTI_VESSEL || What_Am_I() == RTTI_INFANTRY)) {
		return(false);
	}

	/*
	**	Don't allow selection of object when in building placement mode.
	*/
	if (Map.PendingObject) {
		return(false);
	}

	if (!allow_mixed) {
		/*
		**	If selecting an object of a different house than the player's, make sure that
		**	the entire selection list is cleared.
		*/
		for (int i = 0; i < CurrentObject.Count(); i++) {
			HouseClass * tryhptr = HouseClass::As_Pointer(Owner());
			HouseClass * oldhptr = HouseClass::As_Pointer(CurrentObject[i]->Owner());
	//		if (Owner() != CurrentObject[0]->Owner() || CurrentObject[0]->Owner() != PlayerPtr->Class->House) {
			if (oldhptr->IsPlayerControl != tryhptr->IsPlayerControl) {
				Unselect_All();
				break;
			}
		}
	}

	if (In_Which_Layer() == LAYER_GROUND) Mark(MARK_OVERLAP_UP);

	//IsSelected = true;
	// Updated to function for multiplayer - 6/26/2019 JAS
	Set_Selected_By_Player();
	
	if (In_Which_Layer() == LAYER_GROUND) Mark(MARK_OVERLAP_DOWN);
	
	return(true);
}


/***********************************************************************************************
 * ObjectClass::Render -- Displays the object onto the map.                                    *
 *                                                                                             *
 *    This routine will determine the location of the object and if it is roughly on the       *
 *    visible screen, it will display it. Not displaying objects that are not on the screen    *
 *    will save valuable time.                                                                 *
 *                                                                                             *
 * INPUT:   bool; Should the render be forced regardless of whether the object is flagged to   *
 *                be redrawn?                                                                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the draw code called for this object?                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Render(bool forced) const
{
	assert(this != 0);
	assert(IsActive);

	int	x, y;
	COORDINATE coord = Render_Coord();
	CELL cell = Coord_Cell(coord);

	if (Debug_Map || Debug_Unshroud || ((forced || IsToDisplay) && IsDown && !IsInLimbo)) {
		const_cast<ObjectClass*>(this)->IsToDisplay = false;		// added const_cast ST - 5/9/2019

		if (Map.Coord_To_Pixel(coord, x, y)) {

			/*
			**	Draw the object itself
			*/
			Draw_It(x, y, WINDOW_TACTICAL);

#ifdef SCENARIO_EDITOR
			/*
			**	Draw the trigger attached to the object. Draw_It is window-
			**	relative, so add the window's x-coord to 'x'.
			*/
			if (Debug_Map && Trigger.Is_Valid()) {
				Fancy_Text_Print(Trigger->Class->IniName,
					x + (WinX), y,
					&ColorRemaps[PCOLOR_RED], TBLACK,
					TPF_CENTER | TPF_NOSHADOW | TPF_6POINT);
			}
#endif

			return(true);
		}
	}
	return(false);
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * ObjectClass::Debug_Dump -- Displays status of the object class to the mono monitor.         *
 *                                                                                             *
 *    This routine is used to display the current status of the object class to the mono       *
 *    monitor.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(1, 1);mono->Printf("%-18.18s", Text_String(Full_Name()));
	if (Next != NULL) {
		mono->Set_Cursor(20, 5);mono->Printf("%08X", Next->As_Target());
	}
	if (Trigger.Is_Valid()) {
		mono->Text_Print(Trigger->Class->IniName, 11, 3);
	}
	mono->Set_Cursor(34, 1);mono->Printf("%3d", Strength);

	mono->Fill_Attrib(1, 13, 12, 1, IsDown ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 14, 12, 1, IsToDamage ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 15, 12, 1, IsToDisplay ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 16, 12, 1, IsInLimbo ? MonoClass::INVERSE : MonoClass::NORMAL);
	// Updated to function for multiplayer - 6/26/2019 JAS
	mono->Fill_Attrib(1, 17, 12, 1, Is_Selected_By_Player() ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 13, 12, 1, IsAnimAttached ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Set_Cursor(23, 14);mono->Printf("%d", Riser);
	mono->Fill_Attrib(14, 12, 14, 1, IsFalling ? MonoClass::INVERSE : MonoClass::NORMAL);

	AbstractClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * ObjectClass::Mark_For_Redraw -- Marks object and system for redraw.                         *
 *                                                                                             *
 *    This routine will mark the object and inform the display system                          *
 *    that appropriate rendering is needed. Whenever it is determined                          *
 *    that an object needs to be redrawn, call this routine.                                   *
 *                                                                                             *
 * INPUT:      none                                                                            *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   This is a subordinate function to the function Mark(). If an object needs to    *
 *             be redrawn it is probably better to call the function Mark(MARK_CHANGE) rather  *
 *             than this function. This function does not inform the map system that           *
 *             overlapping objects are to be redrawn and thus unless you are really sure that  *
 *             this routine should be called, don't.                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Flags map and flags unit only.                                           *
 *=============================================================================================*/
void ObjectClass::Mark_For_Redraw(void)
{
	assert(this != 0);
	assert(IsActive);

	if (!IsToDisplay) {
		IsToDisplay = true;

		/*
		**	This tells the map rendering logic to "go through the motions" and call the
		**	rendering function. In the rendering function, it will sort out what gets
		**	rendered and what doesn't.
		*/
		Map.Flag_To_Redraw(false);
	}
}


/***********************************************************************************************
 * ObjectClass::Limbo -- Brings the object into a state of limbo.                              *
 *                                                                                             *
 *    An object brought into a state of limbo by this routine can be safely deleted. This      *
 *    routine will remove the object from all game lists and tracking systems. It is called    *
 *    prior to deleting the object or placing the object "on ice".                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the object successfully placed in limbo?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Limbo(void)
{
	assert(this != 0);
	assert(IsActive);

	if (GameActive && !IsInLimbo) {

		//Unselect();
		// Updated to function for multiplayer - 6/26/2019 JAS
		Unselect_All_Players();

		Detach_All();
		Mark(MARK_UP);

		/*
		**	Remove the object from the appropriate display list.
		*/
		Map.Remove(this, In_Which_Layer());

		/*
		**	Remove the object from the logic processing list.
		*/
		if (Class_Of().IsSentient) {
			Logic.Delete(this);
		}

		Hidden();
		IsInLimbo = true;
		IsToDisplay = false;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * ObjectClass::Unlimbo -- Brings the object into the game system.                             *
 *                                                                                             *
 *    This routine will place the object into the game tracking and display systems. It is     *
 *    called as a consequence of creating the object. Every game object must be unlimboed at   *
 *    some point.                                                                              *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to place the object into the game system.                  *
 *                                                                                             *
 *          dir (optional) -- initial facing direction for this object                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the game object successfully unlimboed?                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Sets object strength.                                                    *
 *=============================================================================================*/
bool ObjectClass::Unlimbo(COORDINATE coord, DirType )
{
	assert(this != 0);
	assert(IsActive);
	if (GameActive && IsInLimbo && !IsDown) {
		if (ScenarioInit || Can_Enter_Cell(Coord_Cell(coord), FACING_NONE) == MOVE_OK) {
			IsInLimbo = false;
			IsToDisplay = false;
			Coord = Class_Of().Coord_Fixup(coord);

			if (Mark(MARK_DOWN)) {
				if (IsActive) {

					/*
					**	Add the object to the appropriate map layer. This layer is used
					**	for rendering purposes.
					*/
					if (In_Which_Layer() != LAYER_NONE) {
						Map.Submit(this, In_Which_Layer());
					}

					if (Class_Of().IsSentient) {
						Logic.Submit(this);
					}
				}
				return(true);
			}
		}
	}
	return(false);
}


/***********************************************************************************************
 * ObjectClass::Detach -- Detach the specified target from this object.                        *
 *                                                                                             *
 *    This routine is called when the object (as specified) is to be removed from the game     *
 *    engine and thus, all references to it must be severed. Typically, the only thing         *
 *    checked for at this level is the attached trigger.                                       *
 *                                                                                             *
 * INPUT:   target   -- The target that will be removed from the game system.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Detach(TARGET target, bool )
{
	if (Trigger.Is_Valid() && Is_Target_Trigger(target) && Trigger->As_Target() == target) {
		Attach_Trigger(NULL);
	}
}


/***********************************************************************************************
 * ObjectClass::Detach_All -- Removes the object from all tracking systems.                    *
 *                                                                                             *
 *    This routine will take the object and see that it is removed from all miscellaneous      *
 *    tracking systems in the game. This operation is vital when deleting an object. It is     *
 *    necessary so that when the object is removed from the game, existing game objects won't  *
 *    be referencing a now invalid game object. This typically affects the targeting           *
 *    and navigation computers of other game objects.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Detach_All(bool all)
{
	assert(this != 0);
	assert(IsActive);

	/*
	**	Unselect this object if it was selected.
	*/
	//if (all || Owner() != PlayerPtr->Class->House) {
	//	Unselect();
	//}

	//Added some error handling incase there was an issue removing the object - JAS 6/28/2019
	if (all) {
		//Unselect();
		// Updated to function for multiplayer - 6/28/2019 JAS
		Unselect_All_Players();
	}
	else
	{
		Unselect_All_Players_Except_Owner();
	}
	//End of change - JAS 6/28/2019

	Map.Detach(this);

	/*
	**	Remove from targeting computers.
	*/
	Detach_This_From_All(As_Target(), all);
}


/***********************************************************************************************
 * ObjectClass::Receive_Message -- Processes an incoming radio message.                        *
 *                                                                                             *
 *    Any radio message received that applies to objects in general are handled by this        *
 *    routine. Typically, this is the "redraw" message, which occurs when another object is    *
 *    loading or unloading and thus overlapping.                                               *
 *                                                                                             *
 * INPUT:   message  -- The message received.                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the appropriate radio response. If the message was recognized, then   *
 *          RADIO_ROGER is returned, otherwise, just RADIO_STATIC is returned.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
RadioMessageType ObjectClass::Receive_Message(RadioClass *, RadioMessageType message, long & )
{
	assert(this != 0);
	assert(IsActive);

	switch (message) {

		/*
		**	This message serves as a rendering convenience. It lets the system
		**	know that there might be a visual conflict and the unit in radio
		**	contact should be redrawn. This typically occurs when a vehicle
		**	is being unloaded from a hover lander.
		*/
		case RADIO_REDRAW:
			Mark(MARK_CHANGE);
			return(RADIO_ROGER);

		default:
			break;
	}
	return(RADIO_STATIC);
}


/***********************************************************************************************
 * ObjectClass::Take_Damage -- Applies damage to the object.                                   *
 *                                                                                             *
 *    This routine applies damage to the object according to the damage parameters. It handles *
 *    reducing the strength of the object and also returns the result of that damage. The      *
 *    result value can be examined to determine if the object was destroyed, greatly damaged,  *
 *    or other results.                                                                        *
 *                                                                                             *
 * INPUT:   damage   -- Reference to the damage number to apply. This number will be adjusted  *
 *                      according to defensive armor and distance. Examine this value after    *
 *                      the call to determine the actual amount of damage applied.             *
 *                                                                                             *
 *          distance -- The distance (in leptons) from the center of the damage causing        *
 *                      explosion to the object itself.                                        *
 *                                                                                             *
 *          warhead  -- The warhead type that is causing the damage.                           *
 *                                                                                             *
 *          source   -- The perpetrator of this damage.                                        *
 *                                                                                             *
 *          forced   -- Is the damage forced upon the object regardless of whether it          *
 *                      is normally immune?                                                    *
 *                                                                                             *
 * OUTPUT:  Returns the ResultType that indicates what the affect of the damage was.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 JLB : Created.                                                                 *
 *   12/27/1994 JLB : Trigger event processing for attacked or destroyed.                      *
 *   01/01/1995 JLB : Reduces damage greatly depending on range.                               *
 *=============================================================================================*/
ResultType ObjectClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source, bool forced)
{
	assert(this != 0);
	assert(IsActive);

	ResultType result = RESULT_NONE;
	int oldstrength = Strength;

	if (oldstrength && damage != 0 && (forced || !Class_Of().IsImmune)) {
		int maxstrength = Class_Of().MaxStrength;

		/*
		**	Modify damage based on the warhead type and the armor of the object. This results
		**	in a reduced damage value, but never below 1 damage point.  Unless
		** it's forced damage, in which case we want full damage.
		*/
		if (!forced /*&& damage > 0*/) {
			damage = Modify_Damage(damage, warhead, Class_Of().Armor, distance);

			/*
			**	Special hack to ensure that dogs only do damage to intended victim and no
			**	damage to others.
			*/
			if (source && source->What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)source)->Class->IsDog) {
				if (source->TarCom == As_Target()) {
					damage = Strength;
				} else {
					damage = 0;
				}
			}
		}
		if (damage == 0) return(RESULT_NONE);

		/*
		** Are we healing/repairing?  If so, add strength, but in
		** any case, return that no damage was done.
		*/
		if (damage < 0) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if (What_Am_I() == RTTI_INFANTRY || What_Am_I() == RTTI_UNIT || What_Am_I() == RTTI_AIRCRAFT) {
#else
			if (What_Am_I() == RTTI_INFANTRY) {
#endif
				Clicked_As_Target(PlayerPtr->Class->House, 7); // 2019/09/20 JAS - Added record of who clicked on the object
				Strength -= damage;
				if (Strength > maxstrength) {
					Strength = maxstrength;
				}
			}
			return(RESULT_NONE);
		}

		/*
		**	At this point, we KNOW that at least light damage has occurred.
		*/
		result = RESULT_LIGHT;

		/*
		**	A non-fatal blow has occurred. Check to see if the object transitioned to below
		**	half strength or if it is now down to one hit point.
		*/
		if (oldstrength > damage) {

			if (oldstrength >= (maxstrength >> 1) && (oldstrength-damage) < (maxstrength >> 1)) {
				result = RESULT_HALF;
			}
		} else {

			/*
			**	When an object is damaged to destruction, it will instead stop at one
			**	damage point. This will prolong the damage state as well as
			**	give greater satisfaction when it is finally destroyed.
			*/
			damage = oldstrength;
		}

		/*
		**	Apply the damage to the object.
		*/
		Strength = oldstrength - damage;

		/*
		**	Check to see if the object is majorly damaged or destroyed.
		*/
		switch (Strength) {
			case 0:
				Record_The_Kill(source);
				result = RESULT_DESTROYED;
				if (this->Is_Techno()) {
					if (this == ::As_Object(((TechnoClass *)this)->House->UnitToTeleport)) ((TechnoClass *)this)->House->UnitToTeleport = 0;
				}
				Detach_All();
				break;

			case 1:
				result = RESULT_MAJOR;
				break;

			default:
				break;
		}

		/*
		**	Handle any trigger event associated with this object.
		*/
		if (source && Trigger.Is_Valid() && result != RESULT_DESTROYED) {
			Trigger->Spring(TEVENT_ATTACKED, this);
		}

		/*
		**	If any damage was assessed and this object is selected, then flag
		**	the object to be redrawn so that the health bar will be updated.
		*/
		//if (result != RESULT_NONE && IsSelected) {
		// Updated to function for multiplayer - 6/26/2019 JAS
		if (result != RESULT_NONE && Is_Selected_By_Player()) {			
			Mark(MARK_CHANGE);
		}
	}

	/*
	**	Return with the result of the damage taken.
	*/
	return(result);
}


/***********************************************************************************************
 * ObjectClass::Mark -- Handles basic marking logic.                                           *
 *                                                                                             *
 *    This routine handles the base logic for marking an object up or down on the map. It      *
 *    manages the IsDown flag as well as flagging the object to be redrawn if necessary.       *
 *    Whenever an object is to be marked, it should call this base class function first. If    *
 *    this function returns true, then the higher level function should proceed with its own   *
 *    logic.                                                                                   *
 *                                                                                             *
 * INPUT:   mark  -- The marking method to use for this object. It can be either MARK_DOWN,    *
 *                   MARK_UP, or MARK_CHANGE.                                                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the object marked successfully?                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Mark(MarkType mark)
{
	assert(this != 0);
	assert(IsActive);

	if (!IsInLimbo && IsActive) {

		/*
		**	A mark for change is always successful UNLESS the object
		**	is not placed down or has already been flagged as changed
		**	this game frame.
		*/
		if (mark == MARK_CHANGE || mark == MARK_CHANGE_REDRAW) {
			if (IsToDisplay && mark != MARK_CHANGE_REDRAW) return(false);
			if (IsDown) {
				Mark_For_Redraw();
				return(true);
			}
			return(false);
		}

		/*
		** Handle adding or removing the object in the cells' overlap lists
		*/
		if (mark == MARK_OVERLAP_UP) {
			if (IsDown == true) {
				if (Class_Of().IsFootprint) {
					Map.Overlap_Up(Coord_Cell(Coord), this);
				}
				Mark_For_Redraw();
				return(true);
			}
		}
		if (mark == MARK_OVERLAP_DOWN) {
			if (IsDown == true) {
				if (Class_Of().IsFootprint) {
					Map.Overlap_Down(Coord_Cell(Coord), this);
				}
				Mark_For_Redraw();
				return(true);
			}
		}

		/*
		** It is important to know whether the object is a techno class
		** or not to see if we have to adjust the regional threat ratings
		*/
		int threat = 0;
		HousesType house = HOUSE_NONE;
		CELL cell = 0;
		TechnoClass * tech;
		if (Is_Techno()) {
			tech 	 = (TechnoClass *)this;
			threat = tech->Risk();
			house  = tech->Owner();
			cell   = Coord_Cell(Coord);
		} else {
			tech = NULL;
		}

		/*
		**	Marking down is only successful if the object isn't already
		**	placed down.
		*/
		if (mark == MARK_DOWN && !IsDown) {
			if (tech && Session.Type == GAME_NORMAL && In_Which_Layer() == LAYER_GROUND) {
				Map[cell].Adjust_Threat(house, threat);
			}
			IsDown = true;
			Mark_For_Redraw();
			return(true);
		}

		/*
		**	Lifting up is only successful if the object isn't already
		**	lifted up from the map.
		*/
		if (mark == MARK_UP && IsDown) {
			if (tech && Session.Type == GAME_NORMAL && In_Which_Layer() == LAYER_GROUND) {
				Map[cell].Adjust_Threat(house, -threat);
			}
			if (Class_Of().IsFootprint) {
				Map.Overlap_Up(Coord_Cell(Coord), this);
			}
			IsDown = false;
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * ObjectClass::Init -- Initializes the basic object system.                                   *
 *                                                                                             *
 *    This routine should be called when the basic object system needs to be initialized. This *
 *    occurs when the scenario is about to be loaded.                                          *
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
void ObjectClass::Init(void)
{
	CurrentObject.Clear_All();
}


/***********************************************************************************************
 * ObjectClass::Revealed -- Reveals this object to the house specified.                        *
 *                                                                                             *
 *    This routine is called when this object gets revealed to the house specified.            *
 *                                                                                             *
 * INPUT:   house -- Pointer to the house that this object is being revealed to.               *
 *                                                                                             *
 * OUTPUT:  Was this object revealed for the first time to this house? Generic objects always  *
 *          return true unless an invalid house pointer was specified.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Revealed(HouseClass * house)
{
	assert(this != 0);
	assert(IsActive);

	return(house != NULL);
}




/***********************************************************************************************
 * ObjectClass::Set_Selected_By_Player -- Set this object as selected by the given player or   *
 *														the default player.											  *
 *                                                                                             *
 * INPUT:   Player pointer                                                                     *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   6/25/2019 - JAS																									  *
 *=============================================================================================*/
void ObjectClass::Set_Selected_By_Player(HouseClass *player)
{
	if (!player || !player->Class) {
		player = PlayerPtr;
	}

	HousesType house = player->Class->House;
	if (((TechnoTypeClass const &)Class_Of()).IsLeader) {
		CurrentObject.Add_Head(house, this);
	}
	else {
		CurrentObject.Add(house, this);
	}

	int shift = (int)house;
	IsSelectedMask |= (1 << shift);

	if (Session.Type == GAME_NORMAL && player == PlayerPtr) {
		IsSelected = true;
	}
}

/***********************************************************************************************
 * ObjectClass::Set_Unselected_By_Player -- Set this object as unselected by the given player  *
 *														orthe default player.										  *
 *                                                                                             *
 * INPUT:   Player pointer                                                                     *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   6/25/2019 - JAS																									  *
 *=============================================================================================*/
void ObjectClass::Set_Unselected_By_Player(HouseClass *player)
{
	if (!player || !player->Class) {
		player = PlayerPtr;
	}

	HousesType house = player->Class->House;
	CurrentObject.Delete(house, this);

	int shift = (int)house;
	IsSelectedMask &= ~(1 << shift);

	if (Session.Type == GAME_NORMAL && player == PlayerPtr) {
		IsSelected = false;
	}
}

/***********************************************************************************************
 * ObjectClass::Is_Selected_By_Player -- Has this object been selected by the given player	  *
 *                                                                                             *
 * INPUT:   Player pointer                                                                     *
 *                                                                                             *
 * OUTPUT:  True if selected by that player                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   6/25/2019	- JAS 																								  *
 *=============================================================================================*/
bool ObjectClass::Is_Selected_By_Player(HouseClass *player) const
{
	if (player && player->Class) {
		int shift = (int)player->Class->House;
		return (IsSelectedMask & (1 << shift)) ? true : false;
	}
	else {
		int shift = (int)PlayerPtr->Class->House;
		return (IsSelectedMask & (1 << shift)) ? true : false;
	}
	return false;
}




/***********************************************************************************************
 * ObjectClass::Paradrop -- Unlimbos object in paradrop mode.                                  *
 *                                                                                             *
 *    Call this routine as a replacement for Unlimbo() if the object is to be paradropped onto *
 *    the playing field.                                                                       *
 *                                                                                             *
 * INPUT:   coord -- The desired landing coordinate to give the dropping unit.                 *
 *                                                                                             *
 * OUTPUT:  bool; Was the object successfully unlimboed and has begun paradropping?            *
 *                                                                                             *
 * WARNINGS:   The unit may not be successful in paradropping if the desired destination       *
 *             location cannot be occupied by the object.                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/07/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Paradrop(COORDINATE coord)
{
	assert(this != 0);
	assert(IsActive);

	Height = FLIGHT_LEVEL;
	IsFalling = true;
	if (Unlimbo(coord, DIR_S)) {
		AnimClass * anim = NULL;

		if (What_Am_I() == RTTI_BULLET) {
			anim = new AnimClass(ANIM_PARA_BOMB, Coord_Move(Center_Coord(), DIR_N, 0x0030 + Height));
		} else {
			anim = new AnimClass(ANIM_PARACHUTE, Coord_Move(Center_Coord(), DIR_N, 0x0030 + Height));
		}

		/*
		**	If the animation was created, then attach it to this object.
		*/
		if (anim != NULL) {
			anim->Attach_To(this);
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * ObjectClass::Attach_Trigger -- Attach specified trigger to object.                          *
 *                                                                                             *
 *    This routine is used to attach the specified trigger to the object.                      *
 *                                                                                             *
 * INPUT:   trigger  -- Pointer to the trigger to attach. If any existing trigger is desired   *
 *                      to be detached, then pass NULL to this routine.                        *
 *                                                                                             *
 * OUTPUT:  bool; Was the trigger attached?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Attach_Trigger(TriggerClass * trigger)
{
	if (Trigger.Is_Valid()) {
		TriggerClass * tptr = Trigger;
		tptr->AttachCount--;
		Trigger = NULL;
	}

	if (trigger) {
		Trigger = trigger;
		trigger->AttachCount++;
		return(true);
	}
	return(false);
}


// These can't be made inline (for various reasons).
short const * ObjectClass::Occupy_List(bool placement) const {return(Class_Of().Occupy_List(placement));};
short const * ObjectClass::Overlap_List(bool ) const {return(Class_Of().Overlap_List());};
BuildingClass * ObjectClass::Who_Can_Build_Me(bool intheory, bool legal) const {return(Class_Of().Who_Can_Build_Me(intheory, legal, Owner()));};
fixed ObjectClass::Health_Ratio(void) const {return(fixed(Strength, Class_Of().MaxStrength));};
int ObjectClass::Full_Name(void) const {return Class_Of().Full_Name();};


//**********************************************************************************************
// MODULE SEPARATION -- ObjectTypeClass member functions follow.
//**********************************************************************************************


/***********************************************************************************************
 * ObjectTypeClass::ObjectTypeClass -- Normal constructor for object type class objects.       *
 *                                                                                             *
 *    This is the base constructor that is used when constructing the object type classes.     *
 *    Every tangible game piece type calls this constructor for the ObjectTypeClass. This      *
 *    class holds static information that is common to objects in general.                     *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectTypeClass::ObjectTypeClass(
						RTTIType rtti,
						int id,
						bool is_sentient,
						bool is_stealthy,
						bool is_selectable,
						bool is_legal_target,
						bool is_insignificant,
						bool is_immune,
						bool is_footprint,
						int name,
						char const * ini) :
	AbstractTypeClass(rtti, id, name, ini),
	IsCrushable(false),
	IsStealthy(is_stealthy),
	IsSelectable(is_selectable),
	IsLegalTarget(is_legal_target),
	IsInsignificant(is_insignificant),
	IsImmune(is_immune),
	IsSentient(is_sentient),
	IsFootprint(is_footprint),
	Armor(ARMOR_NONE),
	MaxStrength(0),
	ImageData(0),
	RadarIcon(0)
{
	/*
	** Init the DimensionData rect. Not sure how this was ever working before without being allocated. It was just trashing
	** memory later on when the pointer was being dereferenced and written to without being initialized. ST - 8/14/2019 3:15PM
	*/
	DimensionData = NULL;
}


/***********************************************************************************************
 * ObjectTypeClass::Max_Pips -- Fetches the maximum pips allowed for this object.              *
 *                                                                                             *
 *    This routine will return the maximum number of pips that can be displayed for this       *
 *    object. When dealing with generic objects, this value is always zero.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of pip boxes (empty or otherwise) to display.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ObjectTypeClass::Max_Pips(void) const
{
	return(0);
}


/***********************************************************************************************
 * ObjectTypeClass::Dimensions -- Gets the dimensions of the object in pixels.                 *
 *                                                                                             *
 *    This routine will fetch the dimensions of this object expressed as pixels width and      *
 *    pixels height. This information can be used to intelligently update the clipping         *
 *    rectangles.                                                                              *
 *                                                                                             *
 * INPUT:   width    -- Reference to the width variable that will be filled in.                *
 *                                                                                             *
 *          height   -- Reference to the height variable that will be filled in.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectTypeClass::Dimensions(int &width, int &height) const
{
	width = 10;
	height = 10;
}


/***********************************************************************************************
 * ObjectTypeClass::Cost_Of -- Returns the cost to buy this unit.                              *
 *                                                                                             *
 *    This routine will return the cost to purchase this unit. This routine is expected to be  *
 *    overridden by the objects that can actually be purchased. All other object types can     *
 *    simply return zero since this value won't be used.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the cost of the object.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ObjectTypeClass::Cost_Of(void) const
{
	return(0);
}


/***********************************************************************************************
 * ObjectTypeClass::Time_To_Build -- Fetches the time to construct this object.                *
 *                                                                                             *
 *    This routine will fetch the time in takes to construct this object. Objects that can     *
 *    be constructed will override this routine in order to return a useful value.             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the time units (arbitrary) that it takes to construct this object.    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ObjectTypeClass::Time_To_Build(void) const
{
	return(0);
}


/***********************************************************************************************
 * ObjectTypeClass::Get_Cameo_Data -- Fetches pointer to cameo data for this object type.      *
 *                                                                                             *
 *    This routine will return with the cameo data pointer for this object type. It is         *
 *    expected that objects that can appear on the sidebar will override this routine in order *
 *    to provide proper cameo data pointer.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the cameo shape data.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * ObjectTypeClass::Get_Cameo_Data(void) const
{
	return(NULL);
}


/***********************************************************************************************
 * ObjectTypeClass::Occupy_List -- Returns with simple occupation list for object.             *
 *                                                                                             *
 *    This routine returns a pointer to a simple occupation list for this object. Since at     *
 *    this tier of the object class chain, the exact shape of the object is indeterminate,     *
 *    this function merely returns a single cell occupation list. This actually works for      *
 *    most vehicles.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to a simple occupation list.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * ObjectTypeClass::Occupy_List(bool) const
{
	static short const _list[] = {0, REFRESH_EOL};
	return(_list);
}


/***********************************************************************************************
 * ObjectTypeClass::Overlap_List -- Returns a pointer to a simple overlap list.                *
 *                                                                                             *
 *    This function returns a pointer to an overlap list for the object. An overlap list is    *
 *    the offsets from the object's cell to get the cells the imagery overlaps, but is object  *
 *    is not considered to occupy. Since at this stage, the overlap information is not         *
 *    available, this function merely returns a pointer to an empty list.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to the generic overlap list.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * ObjectTypeClass::Overlap_List(void) const
{
	static short const _list[] = {REFRESH_EOL};
	return(_list);
}


/***********************************************************************************************
 * ObjectTypeClass::One_Time -- Handles one time processing for object types.                  *
 *                                                                                             *
 *    This routine is used to handle the once per game processing required for object types.   *
 *    This consists of loading any data and initializing any data tables the game requires.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine goes to disk.                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectTypeClass::One_Time(void)
{
	SelectShapes = MFCD::Retrieve("SELECT.SHP");

	#ifndef NDEBUG
		RawFileClass file("PIPS.SHP");
		if (file.Is_Available()) {
			PipShapes = Load_Alloc_Data(file);
		} else {
			PipShapes = MFCD::Retrieve("PIPS.SHP");
		}
	#else
		PipShapes = MFCD::Retrieve("PIPS.SHP");
	#endif
}


/***********************************************************************************************
 * ObjectTypeClass::Who_Can_Build_Me -- Determine what building can build this object type.    *
 *                                                                                             *
 *    This routine will scan through all available factory buildings and determine which       *
 *    is capable of building this object type. The scan can be controlled to scan for only     *
 *    factory buildings that are free to produce now or those that could produce this          *
 *    object type if conditions permit.                                                        *
 *                                                                                             *
 * INPUT:   intheory -- Should the general (when conditions permit) case be examined to see    *
 *                      if a building could build this object type "in theory" even though it  *
 *                      might currently be otherwise occupied?                                 *
 *                                                                                             *
 *          legal    -- Check for building prerequisite and technology level rules? Usually    *
 *                      this would be 'true' for human controlled requests and 'false' for     *
 *                      the computer. This is because the computer is usually not under        *
 *                      the normal restrictions that the player is under.                      *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building that can produce the object of this         *
 *          type. If no suitable factory building could be found, then NULL is returned.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass * ObjectTypeClass::Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const
{
	BuildingClass * anybuilding = NULL;

	if (!intheory && What_Am_I() == RTTI_AIRCRAFTTYPE && ((AircraftTypeClass*)this)->IsFixedWing) {
		int num_builders = 0, num_fixed_wings = 0;
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * building = Buildings.Ptr(index);
			assert(building != NULL);

			if (	!building->IsInLimbo &&
					building->House->Class->House == house &&
					building->Class->ToBuild == RTTI &&
					building->Mission != MISSION_DECONSTRUCTION && building->MissionQueue != MISSION_DECONSTRUCTION &&
					((1L << building->ActLike) & Get_Ownable()) &&
					(!legal || building->House->Can_Build(this, building->ActLike))) {
				num_builders++;
			}
		}
		for (int index = 0; index < Aircraft.Count(); index++) {
			AircraftClass * aircraft = Aircraft.Ptr(index);
			assert(aircraft != NULL);

			if (	!aircraft->IsInLimbo &&
					aircraft->House->Class->House == house &&
					aircraft->Class->IsFixedWing) {
				num_fixed_wings++;
			}
		}
		if (num_fixed_wings >= num_builders) {
			return NULL;
		}
	}

	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * building = Buildings.Ptr(index);
		assert(building != NULL);

		if (	!building->IsInLimbo &&
				building->House->Class->House == house &&
				building->Class->ToBuild == RTTI &&
				building->Mission != MISSION_DECONSTRUCTION && building->MissionQueue != MISSION_DECONSTRUCTION &&
				((1L << building->ActLike) & Get_Ownable()) &&
				(!legal || building->House->Can_Build(this, building->ActLike)) &&
				(intheory || !building->In_Radio_Contact())) {

			// BG: Hack so only kennels can build dogs, and no other, and barracks can
			//     only build humans and no other.
			if (What_Am_I() == RTTI_INFANTRYTYPE) {
				InfantryTypeClass * me = (InfantryTypeClass *)this;
				if (me->IsDog) {
					if (*building == STRUCT_KENNEL) {
						if (building->IsLeader) return(building);
						anybuilding = building;
					}
				} else {
					if (*building != STRUCT_KENNEL) {
						if (building->IsLeader) return(building);
						anybuilding = building;
					}
				}
			} else {

				/*
				**	HACK ALERT: Helipads can build aircraft and airstrips can build
				**	fixed wing craft only.
				*/
				if (What_Am_I() == RTTI_AIRCRAFTTYPE) {
					AircraftTypeClass * air = (AircraftTypeClass *)this;
					if ((*building == STRUCT_HELIPAD && !air->IsFixedWing) || (*building == STRUCT_AIRSTRIP && air->IsFixedWing)) {
						if (building->IsLeader) return(building);
						anybuilding = building;
					}

				} else {
					if (building->IsLeader) return(building);
					anybuilding = building;
				}
			}
		}
	}
	return(anybuilding);
}