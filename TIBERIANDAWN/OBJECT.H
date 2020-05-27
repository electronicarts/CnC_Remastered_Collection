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

/* $Header:   F:\projects\c&c\vcs\code\object.h_v   2.15   16 Oct 1995 16:46:16   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OBJECT.H                                                     *
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

#ifndef OBJECT_H
#define OBJECT_H

#include	"abstract.h"

class ObjectClass;
class TechnoClass;
class ObjectTypeClass;
class HouseClass;
class TriggerClass;
class BuildingClass;
class RadioClass;

//extern "C" {
//unsigned Cardinal_To_Fixed(unsigned base, unsigned cardinal);
//}

/**********************************************************************
**	Every game object (that can exist on the map) is ultimately derived from this object
**	class. It holds the common information between all objects. This is primarily the
**	object unique ID number and its location in the world. All common operations between
**	game objects are represented by virtual functions in this class.
*/
class ObjectClass : public AbstractClass
{
	public:
		/*
		**	The object can be in one of two states -- placed down on the map, or not. If the
		**	object is placed down on the map, then this flag will be true.
		*/
		unsigned IsDown:1;

		/*
		**	This is a support flag that is only used while building a list of objects to
		**	be damaged by a proximity affect (explosion). When this flag is set, this object
		**	will not be added to the list of units to damage. When damage is applied to the
		**	object, this flag is cleared again. This process ensures that an object is never
		**	subject to "double jeapordy".
		*/
		unsigned IsToDamage:1;

//	private:
		/*
		**	Is this object flagged to be displayed during the next rendering process?  This
		**	flag could be set by many different circumstances. It is automatically cleared
		**	when the object is rerendered.
		*/
		unsigned IsToDisplay:1;


	public:
		/*
		**	An object in the game may be valid yet held in a state of "limbo". Units are in such
		**	a state if they are being transported or are otherwise "inside" another unit. They can
		**	also be in limbo if they have been created but are being held until the proper time
		**	for delivery.
		*/
		unsigned IsInLimbo:1;

		/*
		**	When an object is "selected" it is given a floating bar graph or other graphic imagery
		**	to display this fact. When the player performs I/O, the actions may have a direct
		**	bearing on the actions of the currently selected object. For quick checking purposes,
		**	if this object is the one that is "selected", this flag will be true.
		*/
		unsigned IsSelected:1;

		//Added a mask instead of bool for selecting players. This is because we must now support multiplayer.
		// - 6/26/2019
		unsigned short IsSelectedMask;

		/*
		**	If an animation is attached to this object, then this flag will be true.
		*/
		unsigned IsAnimAttached:1;

		/*
		**	Several objects could exist in the same cell list. This is a pointer to the
		**	next object in the cell list. The objects in this list are not in any
		**	significant order.
		*/
		ObjectClass * Next;

		/*
		** Every object can be assigned a trigger; the same trigger can be assigned
		** to multiple objects.
		*/
		TriggerClass * Trigger;

		/*
		**	This is the current strength of this object.
		*/
		short Strength;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[16];

		/*-----------------------------------------------------------------------------------
		**	Constructor & destructors.
		*/
		ObjectClass(void);
		virtual ~ObjectClass(void) {};
		virtual RTTIType What_Am_I(void) const;
		int operator < (ObjectClass const & object) const {return Sort_Y() < object.Sort_Y();};
		int operator > (ObjectClass const & object) const {return Sort_Y() > object.Sort_Y();};

		/*
		**	Object selection control.
		*/
		static void Init(void);

		/*
		**	Query functions.
		*/
		virtual ActionType What_Action(ObjectClass *) const;
		virtual ActionType What_Action(CELL) const;
		virtual LayerType In_Which_Layer(void) const;
		virtual bool Is_Infantry(void) const;
		virtual bool Is_Techno(void) const;
		virtual unsigned char Get_Ownable(void) const;
		virtual ObjectTypeClass const & Class_Of(void) const = 0;
		virtual int Full_Name(void) const;
		virtual bool Can_Repair(void) const;
		virtual bool Can_Demolish(void) const;
		virtual bool Can_Demolish_Unit(void) const;
		virtual bool Can_Player_Fire(void) const;
		virtual bool Can_Player_Move(void) const;

		/*
		**	Coordinate inquiry functions. These are used for both display and
		**	combat purposes.
		*/
		virtual COORDINATE Docking_Coord(void) const;
		virtual COORDINATE Target_Coord(void) const;
		virtual COORDINATE Center_Coord(void) const;
		virtual COORDINATE Render_Coord(void) const;
		virtual COORDINATE Sort_Y(void) const;
		virtual COORDINATE Fire_Coord(int ) const;

		/*
		**	Object entry and exit from the game system.
		*/
		virtual bool Limbo(void);
		virtual bool Unlimbo(COORDINATE , DirType facing = DIR_N);
		virtual void Detach(TARGET, bool) {};
		virtual void Detach_All(bool all=true);
		static void Detach_This_From_All(TARGET target, bool all=true);
		virtual void Record_The_Kill(TechnoClass * );

		/*
		**	Display and rendering support functionality. Supports imagery and how
		**	object interacts with the map and thus indirectly controls rendering.
		*/
		virtual void Do_Shimmer(void);
		virtual int Exit_Object(TechnoClass *);
		virtual bool Render(bool forced);
		virtual short const * Occupy_List(bool placement=false) const;
		virtual short const * Overlap_List(void) const;
		virtual unsigned Health_Ratio(void) const;
		virtual void Draw_It(int x, int y, WindowNumberType ) = 0;
		virtual void Hidden(void);
		virtual void Look(bool =false);
		virtual bool Mark(MarkType);

	private:
		virtual void Mark_For_Redraw(void);

	public:

		/*
		**	User I/O.
		*/
		virtual void Active_Click_With(ActionType , ObjectClass *);
		virtual void Active_Click_With(ActionType , CELL );
		virtual void Clicked_As_Target(HousesType house,int = 7);
		virtual bool Select(bool allow_mixed = false);
		virtual void Unselect(void);
		
		//These selection functions were added to handle the fact that we now need to support 
		//client-server multiplayer. - JAS 6/26/2019
		virtual void Unselect_All_Players(void);
		virtual void Unselect_All_Players_Except_Owner(void);
		virtual bool Is_Selected_By_Player(HouseClass *player = NULL) const;
		virtual void Set_Selected_By_Player(HouseClass *player = NULL);
		virtual void Set_Unselected_By_Player(HouseClass *player = NULL);

		/*
		**	Combat related.
		*/
		virtual bool In_Range(COORDINATE , int=0) const;
		virtual int Weapon_Range(int =0) const;
		virtual ResultType Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source=0);
		virtual TARGET As_Target(void) const;
		virtual void Scatter(COORDINATE , bool=false, bool=false);
		virtual bool Catch_Fire(void);
		virtual void Fire_Out(void);
		virtual int Value(void) const;
		virtual MissionType Get_Mission(void) const;

		/*
		**	AI.
		*/
		virtual BuildingClass * Who_Can_Build_Me(bool intheory, bool legal) const;
		virtual RadioMessageType Receive_Message(RadioClass * from, RadioMessageType message, long & param);
		virtual bool Revealed(HouseClass * house);
		virtual void Repair(int );
		virtual void Sell_Back(int );

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		/*
		**	Scenario and debug support.
		*/
		#ifdef CHEAT_KEYS
		virtual void Debug_Dump(MonoClass *mono) const;
		#endif
		virtual void Move(FacingType);

};

#endif