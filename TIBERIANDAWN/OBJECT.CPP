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

/* $Header:   F:\projects\c&c\vcs\code\object.cpv   2.17   16 Oct 1995 16:49:22   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : August 13, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ObjectClass::Debug_Dump -- Displays status of the object class to the mono monitor.       *
 *   ObjectClass::Detach_All -- Removes the object from all tracking systems.                  *
 *   ObjectClass::Detach_This_From_All -- Detatches this object from all others.               *
 *   ObjectClass::Fire_Out -- Informs object that attached animation has finished.             *
 *   ObjectClass::Get_Mission -- Fetches the current mission of this object.                   *
 *   ObjectClass::Init -- Initializes the basic object system.                                 *
 *   ObjectClass::Limbo -- Brings the object into a state of limbo.                            *
 *   ObjectClass::Mark -- Handles basic marking logic.                                         *
 *   ObjectClass::Mark_For_Redraw -- Marks object and system for redraw.                       *
 *   ObjectClass::Move -- Moves (by force) the object in the desired direction.                *
 *   ObjectClass::ObjectClass -- Default constructor for objects.                              *
 *   ObjectClass::Passive_Click_With -- Right mouse button click process.                      *
 *   ObjectClass::Receive_Message -- Processes an incoming radio message.                      *
 *   ObjectClass::Render -- Displays the object onto the map.                                  *
 *   ObjectClass::Repair -- Handles object repair control.                                     *
 *   ObjectClass::Revealed -- Reveals this object to the house specified.                      *
 *   ObjectClass::Select -- Try to make this object the "selected" object.                     *
 *   ObjectClass::Sell_Back -- Sells the object -- if possible.                                *
 *   ObjectClass::Take_Damage -- Applies damage to the object.                                 *
 *   ObjectClass::Unlimbo -- Brings the object into the game system.                           *
 *   ObjectClass::Unselect -- This will un-select the object if it was selected.               *
 *   ObjectClass::Value -- Fetches the target value of this object.                            *
 *   ObjectClass::What_Action -- Deteremines what action to perform on specified object.       *
 *   ObjectClass::What_Am_I -- RTTI query of this object type.                                 *
 *   ObjectTypeClass::Cost_Of -- Returns the cost to buy this unit.                            *
 *   ObjectTypeClass::Dimensions -- Gets the dimensions of the object in pixels.               *
 *   ObjectTypeClass::Get_Cameo_Data -- Fetches pointer to cameo data for this object type.    *
 *   ObjectTypeClass::Max_Pips -- Fetches the maximum pips allowed for this object.            *
 *   ObjectTypeClass::ObjectTypeClass -- Normal constructor for object type class objects.     *
 *   ObjectTypeClass::Occupy_List -- Returns with simple occupation list for object.           *
 *   ObjectTypeClass::One_Time -- Handles one time processing for object types.                *
 *   ObjectTypeClass::Overlap_List -- Returns a pointer to a simple overlap list.              *
 *   ObjectTypeClass::Time_To_Build -- Fetches the time to construct this object.              *
 *   ObjectTypeClass::Who_Can_Build_Me -- Finds the factory building that can build this object*
 *   ObjectClass::What_Action -- Returns with the action to perform for this object.           *
 *   ObjectClass::In_Which_Layer -- Fetches what layer this object is located in.              *
 *   ObjectClass::Is_Techno -- Checks to see if this object is a techno type.                  *
 *   ObjectClass::Get_Ownable -- Fetches the house owner legality options for this object.     *
 *   ObjectClass::Can_Repair -- Queries whether this object can be repaired.                   *
 *   ObjectClass::Can_Demolish -- Queries whether this object can be sold back.                *
 *   ObjectClass::Can_Player_Fire -- Can the player give this object an attack mission?        *
 *   ObjectClass::Can_Player_Move -- Can the player give this object a movement mission?       *
 *   ObjectClass::Target_Coord -- Fetches the coordinate if this object is a target.           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


/*
**	Selected objects have a special marking box around them. This is the shapes that are
**	used for this purpose.
*/
void const * ObjectTypeClass::SelectShapes = 0;

void const * ObjectTypeClass::PipShapes = 0;


bool ObjectClass::Is_Infantry(void) const
{
	return(false);
}



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
						bool is_sentient,
						bool is_flammable,
						bool is_crushable,
						bool is_stealthy,
						bool is_selectable,
						bool is_legal_target,
						bool is_insignificant,
						bool is_immune,
						int name,
						char const *ini,
						ArmorType armor,
						unsigned short strength) :
	AbstractTypeClass(name, ini)
{
	IsSentient = is_sentient;
	IsFlammable = is_flammable;
	IsCrushable = is_crushable;
	IsStealthy = is_stealthy;
	IsSelectable = is_selectable;
	IsLegalTarget = is_legal_target;
	IsInsignificant = is_insignificant;
	IsImmune = is_immune;
	Armor = armor;
	MaxStrength = strength;
	ImageData = NULL;
	//RadarIcon = NULL;
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
int ObjectTypeClass::Time_To_Build(HousesType ) const
{
	return(0);
}


/***********************************************************************************************
 * ObjectTypeClass::Who_Can_Build_Me -- Finds the factory building that can build this object. *
 *                                                                                             *
 *    This routine will search for a factory building that can build this object type.         *
 *                                                                                             *
 * INPUT:   <NA> this routine is just here to be overridden by other classes.                  *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building that can construct the object specified.    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass * ObjectTypeClass::Who_Can_Build_Me(bool, bool, HousesType) const
{
	return(NULL);
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
ObjectClass::ObjectClass(void)
{
	Coord = 0xFFFFFFFFL;				// Some bogus illegal value.
	Next = 0;							// Not part of any object list.
	Trigger = 0;						// No associated trigger.
	IsToDamage = false;
	IsToDisplay = false;				// Redraw is presumed unnecessary.
	IsInLimbo = true;					// Always presumed to start in limbo state.
	IsSelected = false;				// Limboed units cannot be selected.
	IsDown = false;					// Limboed units cannot be on the map.
	IsAnimAttached = false;			// Anim is not attached.
	Strength = 255;					// nominal strength value
	IsSelectedMask = 0;				// Mask showing who has selected this object
}


/***********************************************************************************************
 * ObjectClass::What_Am_I -- RTTI query of this object type.                                   *
 *                                                                                             *
 *    This routine will never be called, but is here for completeness. Every object that       *
 *    is derived from object class must overload this function and return their own proper     *
 *    object RTTI value.                                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the RTTI value that coresponds to the object's type.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
RTTIType ObjectClass::What_Am_I(void) const
{
	return(RTTI_OBJECT);
}


/***********************************************************************************************
 * ObjectClass::What_Action -- Deteremines what action to perform on specified object.         *
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
ActionType ObjectClass::What_Action(ObjectClass *) const
{
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
	return(LAYER_GROUND);
}


/***********************************************************************************************
 * ObjectClass::Is_Techno -- Checks to see if this object is a techno type.                    *
 *                                                                                             *
 *    Most active objects in the game are of the techno type. This routine will return true    *
 *    if called on an object that is derived from TechnoClass. The RTTI interface is           *
 *    insufficient for this purpose -- hence the existence of this routine.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Is this object derived from the TechnoClass object? This is true for units,        *
 *          infantry, aircraft, and buildings.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ObjectClass::Is_Techno(void) const
{
	return(false);
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
unsigned char ObjectClass::Get_Ownable(void) const
{
	return(0xff);
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
	return(false);
}


bool ObjectClass::Can_Demolish_Unit(void) const
{
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
	return(Center_Coord());
}

COORDINATE ObjectClass::Center_Coord(void) const {return Coord;};
COORDINATE ObjectClass::Render_Coord(void) const {return(Center_Coord());}
COORDINATE ObjectClass::Docking_Coord(void) const {return(Center_Coord());}
COORDINATE ObjectClass::Sort_Y(void) const {return Coord;};
COORDINATE ObjectClass::Fire_Coord(int ) const {return Coord;};
void ObjectClass::Record_The_Kill(TechnoClass * ) {};
void ObjectClass::Do_Shimmer(void) {};
int ObjectClass::Exit_Object(TechnoClass *) {return 0;};
void ObjectClass::Hidden(void) {};
void ObjectClass::Look(bool ) {};
void ObjectClass::Active_Click_With(ActionType , ObjectClass *) {};
void ObjectClass::Active_Click_With(ActionType , CELL ) {};
void ObjectClass::Clicked_As_Target(HousesType house, int) {}; // 2019/09/20 JAS - Added record of who clicked on the object
bool ObjectClass::In_Range(COORDINATE , int) const {return false;};
int ObjectClass::Weapon_Range(int) const {return 0x0000;};
TARGET ObjectClass::As_Target(void) const {return TARGET_NONE;};
void ObjectClass::Scatter(COORDINATE , bool, bool) {};
bool ObjectClass::Catch_Fire(void) {return false;};


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
	//if (IsSelected) { 
	// Updated to function for multiplayer - 6/26/2019 JAS
	if (Is_Selected_By_Player()) {
		
		if (In_Which_Layer()==LAYER_GROUND){
			Mark(MARK_OVERLAP_UP);
		}

		//IsSelected = false;
		// Updated to function for multiplayer - 6/26/2019 JAS
		Set_Unselected_By_Player();

		if (In_Which_Layer()==LAYER_GROUND){
			Mark(MARK_OVERLAP_DOWN);
		}
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
	//	if (!Debug_Map && (IsSelected || !Class_Of().IsSelectable)) return(false);
	// Updated to function for multiplayer - 6/26/2019 JAS
	if (!Debug_Map && (Is_Selected_By_Player() || !Class_Of().IsSelectable)) return(false);

	if (Can_Player_Move() && Is_Techno() && ((TechnoClass *)this)->IsALoaner) return(false);

	/*
	**	Don't allow selection of object when in building placement mode.
	*/
	if (Map.PendingObject) return(false);

	if (!allow_mixed) {
		/*
		**	If selecting an object of a different house than the player's, make sure that
		**	the entire selection list is cleared.
		*/
		for (int i = 0; i < CurrentObject.Count(); i++) {
			if (Owner() != CurrentObject[i]->Owner()) {
				Unselect_All();
				break;
			}
		}
	}

	if (In_Which_Layer()==LAYER_GROUND){
		Mark(MARK_OVERLAP_UP);
	}

	//IsSelected = true;
	// Updated to function for multiplayer - 6/26/2019 JAS
	Set_Selected_By_Player();

	if (In_Which_Layer()==LAYER_GROUND){
		Mark(MARK_OVERLAP_DOWN);
	}
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
bool ObjectClass::Render(bool forced)
{
	int	x,y;
	COORDINATE	coord = Render_Coord();
	CELL	cell = Coord_Cell(coord);

	if (Debug_Map || Debug_Unshroud || ((forced || IsToDisplay) && IsDown && !IsInLimbo)) {
		IsToDisplay = false;

		/*
		**	Draw the path as lines on the map if so directed and the object is one that
		**	contains a path.
		*/

		//if (Special.IsShowPath && IsSelected) {
		// Updated to function for multiplayer - 6/26/2019 JAS
		if (Special.IsShowPath && Is_Selected_By_Player()) {
			switch (What_Am_I()) {
				case RTTI_INFANTRY:
				case RTTI_UNIT:
					FootClass * foot = (FootClass *)this;
					CELL cell;
					int oldx, oldy;

					if (foot->Head_To_Coord() && foot->Path[0] != FACING_NONE) {
						cell = Adjacent_Cell(Coord_Cell(foot->Head_To_Coord()), (FacingType)((foot->Path[0] + FACING_S) & FACING_NW));
						Map.Coord_To_Pixel(Cell_Coord(cell), oldx, oldy);
						for (int index = 0; index < MAX_PATH; index++) {
							if (foot->Path[index] == FACING_NONE) break;
							cell = Adjacent_Cell(cell, foot->Path[index]);
							if (Map.Coord_To_Pixel(Cell_Coord(cell), x, y)) {
								LogicPage->Draw_Line(oldx, 8+oldy, x, 8+y, BLACK);
							}
							oldx = x;
							oldy = y;
						}
					}
					break;
			}
		}

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
			if (Debug_Map && Trigger) {
				Fancy_Text_Print(Trigger->Get_Name(), x + (WinX<<3), y, PINK, TBLACK, TPF_CENTER | TPF_NOSHADOW | TPF_6POINT);
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
void ObjectClass::Debug_Dump(MonoClass *mono) const
{
	mono->Text_Print("X", 16 + (IsToDisplay?2:0), 18);
	mono->Text_Print("X", 16 + (IsActive?2:0), 3);
	mono->Text_Print("X", 16 + (IsInLimbo?2:0), 4);
	//mono->Text_Print("X", 16 + (IsSelected?2:0), 7);
	// Updated to function for multiplayer - 6/26/2019 JAS
	mono->Text_Print("X", 16 + (Is_Selected_By_Player() ?2:0), 7);
	mono->Set_Cursor(56, 1);
	mono->Printf("%08lX", Coord);
	mono->Set_Cursor(14, 1);mono->Printf("[%04X]", As_Target());
	mono->Set_Cursor(20, 3);mono->Printf("%2d[%d]", Strength, Class_Of().MaxStrength);
}
#endif


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
	SelectShapes = MixFileClass::Retrieve("SELECT.SHP");
#if (FRENCH)
	PipShapes = Hires_Retrieve("PIPS_F.SHP");
#else
#if (GERMAN)
	PipShapes = Hires_Retrieve("PIPS_G.SHP");
#else
	PipShapes = Hires_Retrieve("PIPS.SHP");
#endif
#endif
}


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
 * ObjectClass::Detach_This_From_All -- Detatches this object from all others.                 *
 *                                                                                             *
 *    This routine sweeps through all game objects and makes sure that it is no longer         *
 *    referenced by them. Typically, this is called in preparation for the object's death      *
 *    or limbo state.                                                                          *
 *                                                                                             *
 * INPUT:   target   -- This object expressed as a target number.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectClass::Detach_This_From_All(TARGET target, bool all)
{
	int index;
	if (Target_Legal(target)) {

		for (index = 0; index < Houses.Count(); index++) {
			Houses.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Teams.Count(); index++) {
			Teams.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Units.Count(); index++) {
			Units.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Infantry.Count(); index++) {
			Infantry.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Aircraft.Count(); index++) {
			Aircraft.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Buildings.Count(); index++) {
			Buildings.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Bullets.Count(); index++) {
			Bullets.Ptr(index)->Detach(target, all);
		}
		for (index = 0; index < Anims.Count(); index++) {
			Anims.Ptr(index)->Detach(target, all);
		}
	}
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
 * OUTPUT:  Returns the ResultType that indicates what the affect of the damage was.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 JLB : Created.                                                                 *
 *   12/27/1994 JLB : Trigger event processing for attacked or destroyed.                      *
 *   01/01/1995 JLB : Reduces damage greatly depending on range.                               *
 *=============================================================================================*/
ResultType ObjectClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source)
{
	ResultType result = RESULT_NONE;
	int oldstrength = Strength;

	if (oldstrength && damage && !Class_Of().IsImmune) {
		int maxstrength = Class_Of().MaxStrength;

		/*
		**	Modify damage based on the warhead type and the armor of the object. This results
		**	in a reduced damage value, but never below 1 damage point.
		*/
		damage = Modify_Damage(damage, warhead, Class_Of().Armor, distance);
		if (!damage) return(RESULT_NONE);

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
		if (source && Trigger && result != RESULT_DESTROYED) {
			Trigger->Spring(EVENT_ATTACKED, this);
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
	TechnoClass *tech;
	CELL			cell;
	int			threat;
	HousesType	house;

	if (!IsInLimbo && IsActive) {

		/*
		**	A mark for change is always successful UNLESS the object
		**	is not placed down or has already been flagged as changed
		**	this game frame.
		*/
		if (mark == MARK_CHANGE) {
			if (IsToDisplay) return(false);
			if (IsDown == true) {
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
				Map.Overlap_Up(Coord_Cell(Coord),this);
				Mark_For_Redraw();
				return(true);
			}
		}
		if (mark == MARK_OVERLAP_DOWN) {
			if (IsDown == true) {
				Map.Overlap_Down(Coord_Cell(Coord),this);
				Mark_For_Redraw();
				return(true);
			}
		}

		/*
		** It is important to know whether the object is a techno class
		** or not to see if we have to adjust the regional threat ratings
		*/
		if (Is_Techno()) {
			tech 	 = (TechnoClass *)this;
			threat = tech->Risk();
			house  = tech->Owner();
			cell   = Coord_Cell(Coord);
		} else
			tech = NULL;

		/*
		**	Marking down is only successful if the object isn't already
		**	placed down.
		*/
		if (mark == MARK_DOWN && !IsDown) {
			if (tech && GameToPlay == GAME_NORMAL) {
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
			if (tech && GameToPlay == GAME_NORMAL) {
				Map[cell].Adjust_Threat(house, -threat);
			}
			Map.Overlap_Up(Coord_Cell(Coord), this);
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

	if (GameToPlay == GAME_NORMAL && player == PlayerPtr) {
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

	if (GameToPlay == GAME_NORMAL && player == PlayerPtr) {
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



// These can't be made inline (for various reasons).
short const * ObjectClass::Occupy_List(bool placement) const {return(Class_Of().Occupy_List(placement));};
short const * ObjectClass::Overlap_List(void) const {return(Class_Of().Overlap_List());};
BuildingClass * ObjectClass::Who_Can_Build_Me(bool intheory, bool legal) const {return(Class_Of().Who_Can_Build_Me(intheory, legal, Owner()));};
unsigned ObjectClass::Health_Ratio(void) const {return(Cardinal_To_Fixed(Class_Of().MaxStrength, Strength));};
int ObjectClass::Full_Name(void) const {return Class_Of().Full_Name();};

