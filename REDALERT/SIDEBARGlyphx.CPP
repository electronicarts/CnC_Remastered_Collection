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

/* $Header:   F:\projects\c&c\vcs\code\sidebar.cpv   2.13   02 Aug 1995 17:03:22   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SIDEBARGlyphx.CPP                                            *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : March 14th, 2019                                             *
 *                                                                                             *
 *                  Last Update : March 14th, 2019                                             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include "SidebarGlyphx.h"


/*
**  ST - 3/14/2019 10:49AM
** 
**  We are going to need one sidebar per player for multiplayer with GlyphX. We can't have different maps / cell arrays per 
**  player though, so SidebarClass being in the middle of the map/display class hierarchy is a problem.
** 
**  All the class static data will have to be made non-static so we can have multiple instances.
** 
**  So, this is a stub sidebar class with the functionality we need just to support the exporting of production data to the
**  GlyphX client.
** 
** 
*/



/***********************************************************************************************
 * SidebarGlyphxClass::SidebarGlyphxClass -- Default constructor for the sidebar.              *
 *                                                                                             *
 *    Constructor for the sidebar handler. It basically sets up the sidebar to the empty       *
 *    condition.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
SidebarGlyphxClass::SidebarGlyphxClass(void)
{
	//IsRepairActive = false;
	//IsUpgradeActive = false;
	//IsDemolishActive = false;
}




/***********************************************************************************************
 * SidebarGlyphxClass::Init_Clear -- Sets sidebar to a known (and deactivated) state           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarGlyphxClass::Init_Clear(HouseClass *player_ptr)
{
	SidebarPlayerPtr = player_ptr;
	
	//IsRepairActive = false;
	//IsUpgradeActive = false;
	//IsDemolishActive = false;

	Column[0].Set_Parent_Sidebar(this);
	Column[1].Set_Parent_Sidebar(this);

	Column[0].Init_Clear();
	Column[1].Init_Clear();


	//Activate(false);
}


/***********************************************************************************************
 * SidebarGlyphxClass::Init_IO -- Adds buttons to the button list                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarGlyphxClass::Init_IO(void)
{
	/*
	** If a game was loaded & the sidebar was enabled, pop it up now
	*/
	//if (IsSidebarActive) {
	//	IsSidebarActive = false;
	//	Activate(1);
	//}
}



/***********************************************************************************************
 * SidebarGlyphxClass::Which_Column -- Determines which column a given type should appear.     *
 *                                                                                             *
 *    Use this function to resolve what column the specified object type should be placed      *
 *    into.                                                                                    *
 *                                                                                             *
 * INPUT:   otype -- Pointer to the object type class of the object in question.               *
 *                                                                                             *
 * OUTPUT:  Returns with the column number that the object should be placed in.                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int SidebarGlyphxClass::Which_Column(RTTIType type)
{
	if (type == RTTI_BUILDINGTYPE || type == RTTI_BUILDING) {
		return(0);
	}
	return(1);
}


/***********************************************************************************************
 * SidebarGlyphxClass::Factory_Link -- Links a factory to a sidebar strip.                     *
 *                                                                                             *
 *    This routine will link the specified factory to the sidebar strip. A factory must be     *
 *    linked to the sidebar so that as the factory production progresses, the sidebar will     *
 *    show the production progress.                                                            *
 *                                                                                             *
 * INPUT:   factory  -- The factory number to attach.                                          *
 *                                                                                             *
 *          type     -- The object type number.                                                *
 *                                                                                             *
 *          id       -- The object sub-type number.                                            *
 *                                                                                             *
 * OUTPUT:  Was the factory successfully attached to the sidebar strip?                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SidebarGlyphxClass::Factory_Link(int factory, RTTIType type, int id)
{
	return(Column[Which_Column(type)].Factory_Link(factory, type, id));
}



/***********************************************************************************************
 * SidebarGlyphxClass::Add -- Adds a game object to the sidebar list.                          *
 *                                                                                             *
 *    This routine is used to add a game object to the sidebar. Call this routine when a       *
 *    factory type building is created. It handles the case of adding an item that has already *
 *    been added -- it just ignores it.                                                        *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that is being added.                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the object added to the sidebar?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SidebarGlyphxClass::Add(RTTIType type, int id, bool via_capture)
{
	int column;

	/*
	** Add the sidebar only if we're not in editor mode.
	*/
	if (!Debug_Map) {
		column = Which_Column(type);

		if (Column[column].Add(type, id, via_capture)) {
			//Activate(1);
			return(true);
		}
		return(false);
	}

	return(false);
}



/***********************************************************************************************
 * SidebarGlyphxClass::AI -- Handles player clicking on sidebar area.                          *
 *                                                                                             *
 *    This routine handles the processing necessary when the player clicks on the sidebar.     *
 *    Typically, this is selection of the item to build.                                       *
 *                                                                                             *
 * INPUT:   input -- Reference to the keyboard input value.                                    *
 *                                                                                             *
 *          x,y   -- Mouse coordinates at time of input.                                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the click handled?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/28/94   JLB : Created.                                                                 *
 *   11/11/1994 JLB : Processes input directly.                                                *
 *   12/26/1994 JLB : Uses factory manager class for construction handling.                    *
 *   12/31/1994 JLB : Simplified to use the sidebar strip class handlers.                      *
 *   12/31/1994 JLB : Uses mouse coordinate parameters.                                        *
 *   06/27/1995 JLB : <TAB> key toggles sidebar.                                               *
 *=============================================================================================*/
void SidebarGlyphxClass::AI(KeyNumType & input, int x, int y)
{
	if (!Debug_Map) {
		Column[0].AI(input, x, y);
		Column[1].AI(input, x, y);
	}
}


/***********************************************************************************************
 * SidebarGlyphxClass::Recalc -- Examines the sidebar data and updates it as necessary.        *
 *                                                                                             *
 *    Occasionally a factory gets destroyed. This routine must be called in such a case        *
 *    because it might be possible that sidebar object need to be removed. This routine will   *
 *    examine all existing objects in the sidebar class and if no possible factory can         *
 *    produce it, then it will be removed.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine is exhaustive and thus time consuming. Only call it when really    *
 *             necessary. Such as when a factory is destroyed rather than when a non-factory   *
 *             is destroyed.                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/30/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarGlyphxClass::Recalc(void)
{
	Column[0].Recalc();
	Column[1].Recalc();
}



/***********************************************************************************************
 * SidebarGlyphxClass::StripClass::StripClass -- Default constructor for the side strip class. *
 *                                                                                             *
 *    This constructor is used to reset the side strip to default empty state.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
SidebarGlyphxClass::StripClass::StripClass(void)
{
	IsBuilding = false;
	BuildableCount = 0;
	for (int index = 0; index < MAX_BUILDABLES; index++) {
		Buildables[index].BuildableID = 0;
		Buildables[index].BuildableType = RTTI_NONE;
		Buildables[index].Factory = -1;
		Buildables[index].BuildableViaCapture = false;
	}
	ParentSidebar = NULL;
}



/***********************************************************************************************
 * SidebarGlyphxClass::StripClass::Init_Clear -- Sets sidebar to a known (and deactivated) state*
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarGlyphxClass::StripClass::Init_Clear(void)
{
	IsBuilding = false;
	BuildableCount = 0;

	/*
	** Since we're resetting the strips, clear out all the buildables & factory pointers.
	*/
	for (int index = 0; index < MAX_BUILDABLES; index++) {
		Buildables[index].BuildableID = 0;
		Buildables[index].BuildableType = RTTI_NONE;
		Buildables[index].Factory = -1;
		Buildables[index].BuildableViaCapture = false;
	}
}




/***********************************************************************************************
 * SidebarGlyphxClass::StripClass::Add -- Add an object to the side strip.                     *
 *                                                                                             *
 *    Use this routine to add a buildable object to the side strip.                            *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object type that can be built and is to be added to     *
 *                      the side strip.                                                        *
 *                                                                                             *
 * OUTPUT:  bool; Was the object successfully added to the side strip? Failure could be the    *
 *                result of running out of room in the side strip array or the object might    *
 *                already be in the list.                                                      *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SidebarGlyphxClass::StripClass::Add(RTTIType type, int id, bool via_capture)
{
	if (BuildableCount <= MAX_BUILDABLES) {
		for (int index = 0; index < BuildableCount; index++) {
			if (Buildables[index].BuildableType == type && Buildables[index].BuildableID == id) {
				return(false);
			}
		}
		if (!ScenarioInit && type != RTTI_SPECIAL) {
			Speak(VOX_NEW_CONSTRUCT);
		}
		Buildables[BuildableCount].BuildableType = type;
		Buildables[BuildableCount].BuildableID = id;
		Buildables[BuildableCount].Factory = -1;
		Buildables[BuildableCount].BuildableViaCapture = via_capture;
		BuildableCount++;
		return(true);
	}
	return(false);
}



/***********************************************************************************************
 * SidebarGlyphxClass::StripClass::AI -- Input and AI processing for the side strip.                 *
 *                                                                                             *
 *    The side strip AI processing is performed by this function. This function not only       *
 *    checks for player input, but also handles any graphic logic updating necessary as a      *
 *    result of flashing or construction animation.                                            *
 *                                                                                             *
 * INPUT:   input -- The player input code.                                                    *
 *                                                                                             *
 *          x,y   -- Mouse coordinate to use.                                                  *
 *                                                                                             *
 * OUTPUT:  bool; Did the AI detect that it will need a rendering change? If this routine      *
 *                returns true, then the Draw_It function should be called at the              *
 *                earliest opportunity.                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/31/1994 JLB : Created.                                                                 *
 *   12/31/1994 JLB : Uses mouse coordinate parameters.                                        *
 *=============================================================================================*/
bool SidebarGlyphxClass::StripClass::AI(KeyNumType & input, int , int )
{
	/*
	** This is needed as it's where units get queued for structure exit. ST -3/14/2019 12:03PM
	*/
	
	
	if (IsBuilding) {
		for (int index = 0; index < BuildableCount; index++) {
			int factoryid = Buildables[index].Factory;

			if (factoryid != -1) {
				FactoryClass * factory = Factories.Raw_Ptr(factoryid);

				if (factory && (factory->Has_Changed() || factory->Is_Blocked())) {
					
					if (factory->Has_Completed()) {

						/*
						**	Construction has been completed. Announce this fact to the player and
						**	try to get the object to automatically leave the factory. Buildings are
						**	the main exception to the ability to leave the factory under their own
						**	power.
						*/
						TechnoClass * pending = factory->Get_Object();
						if (pending) {
							switch (pending->What_Am_I()) {
								case RTTI_VESSEL:
								case RTTI_UNIT:
								case RTTI_AIRCRAFT:
									OutList.Add(EventClass(EventClass::PLACE, pending->What_Am_I(), -1));
									if (!factory->Is_Blocked()) {
										Speak(VOX_UNIT_READY);
									}
									break;

								case RTTI_BUILDING:
									if (!factory->Is_Blocked()) {
										Speak(VOX_CONSTRUCTION);
									}
									break;

								case RTTI_INFANTRY:
									OutList.Add(EventClass(EventClass::PLACE, pending->What_Am_I(), -1));
									if (!factory->Is_Blocked()) {
										Speak(VOX_UNIT_READY);
									}
									break;
							}
						}
					}
				}
			}
		}
	}

	return(false);
}




/***********************************************************************************************
 * SidebarGlyphxClass::StripClass::Recalc -- Revalidates the current sidebar list of objects.  *
 *                                                                                             *
 *    This routine will revalidate all the buildable objects in the sidebar. This routine      *
 *    comes in handy when a factory has been destroyed, and the sidebar needs to reflect any   *
 *    change that this requires. It checks every object to see if there is a factory available *
 *    that could produce it. If none can be found, then the object is removed from the         *
 *    sidebar.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; The sidebar has changed as a result of this call?                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *   06/26/1995 JLB : Doesn't collapse sidebar when buildables removed.                        *
 *=============================================================================================*/
bool SidebarGlyphxClass::StripClass::Recalc(void)
{
	int ok;

	if (Debug_Map || !BuildableCount) {
		return(false);
	}

	/*
	**	Sweep through all objects listed in the sidebar. If any of those object can
	**	not be created -- even in theory -- then they must be removed form the sidebar and
	**	any current production must be abandoned.
	*/
	for (int index = 0; index < BuildableCount; index++) {
		TechnoTypeClass const * tech = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID);
		if (tech) {
			ok = tech->Who_Can_Build_Me(true, false, ParentSidebar->SidebarPlayerPtr->Class->House) != NULL;
		} else {
			
			if ((unsigned)Buildables[index].BuildableID < SPC_COUNT) {
				ok = ParentSidebar->SidebarPlayerPtr->SuperWeapon[Buildables[index].BuildableID].Is_Present();
			} else {
				ok = false;
			}
		}

		if (!ok) {

			/*
			**	Removes this entry from the list.
			*/
			if (BuildableCount > 1 && index < BuildableCount-1) {
				memcpy(&Buildables[index], &Buildables[index+1], sizeof(Buildables[0])*((BuildableCount-index)-1));
			}
			BuildableCount--;
			index--;

			Buildables[BuildableCount].BuildableID = 0;
			Buildables[BuildableCount].BuildableType = RTTI_NONE;
			Buildables[BuildableCount].Factory = -1;
			Buildables[BuildableCount].BuildableViaCapture = false;
		}
	}

	return(false);
}



/***********************************************************************************************
 * SidebarGlyphxClass::StripClass::Factory_Link -- Links a factory to a sidebar button.              *
 *                                                                                             *
 *    This routine will link the specified factory to this sidebar strip. The exact button to  *
 *    link to is determined from the object type and id specified. A linked button is one that *
 *    will show appropriate construction animation (clock shape) that matches the state of     *
 *    the factory.                                                                             *
 *                                                                                             *
 * INPUT:   factory  -- The factory number to link to the sidebar.                             *
 *                                                                                             *
 *          type     -- The object type that this factory refers to.                           *
 *                                                                                             *
 *          id       -- The object sub-type that this factory refers to.                       *
 *                                                                                             *
 * OUTPUT:  Was the factory successfully attached? Failure would indicate that there is no     *
 *          object of the specified type and sub-type in the sidebar list.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SidebarGlyphxClass::StripClass::Factory_Link(int factory, RTTIType type, int id)
{
	for (int index = 0; index < BuildableCount; index++) {
		if (Buildables[index].BuildableType == type && Buildables[index].BuildableID == id) {
			Buildables[index].Factory = factory;
			IsBuilding = true;

			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * SidebarGlyphxClass::Abandon_Production -- Stops production of the object specified.               *
 *                                                                                             *
 *    This routine is used to abandon production of the object specified. The factory will     *
 *    be completely disabled by this call.                                                     *
 *                                                                                             *
 * INPUT:   type     -- The object type that is to be abandoned. The sub-type is not needed    *
 *                      since it is presumed there can be only one type in production at any   *
 *                      one time.                                                              *
 *                                                                                             *
 *          factory  -- The factory number that is doing the production.                       *
 *                                                                                             *
 * OUTPUT:  Was the factory successfully abandoned?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SidebarGlyphxClass::Abandon_Production(RTTIType type, int factory)
{
	return(Column[Which_Column(type)].Abandon_Production(factory));
}


/***********************************************************************************************
 * SidebarGlyphxClass::StripClass::Abandon_Produ -- Abandons production associated with sidebar.     *
 *                                                                                             *
 *    Production of the object associated with this sidebar is abandoned when this routine is  *
 *    called.                                                                                  *
 *                                                                                             *
 * INPUT:   factory  -- The factory index that is to be suspended.                             *
 *                                                                                             *
 * OUTPUT:  Was the production abandonment successful?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *   08/06/1995 JLB : More intelligent abandon logic for multiple factories.                   *
 *=============================================================================================*/
bool SidebarGlyphxClass::StripClass::Abandon_Production(int factory)
{
	bool noprod = true;
	bool abandon = false;
	for (int index = 0; index < BuildableCount; index++) {
		if (Buildables[index].Factory == factory) {
			Factories.Raw_Ptr(factory)->Abandon();
			Buildables[index].Factory = -1;
			abandon = true;
		} else {
			if (Buildables[index].Factory != -1) {
				noprod = false;
			}
		}
	}

	/*
	**	If there is no production whatsoever on this strip, then flag it so.
	*/
	if (noprod) {
		IsBuilding = false;
	}
	return(abandon);
}

/***********************************************************************************************
 * SidebarGlyphxClass::Code_Pointers -- Converts classes pointers to savable representation    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   9/25/2019 5:36PM ST : Created.                                                            *
 *=============================================================================================*/
void SidebarGlyphxClass::Code_Pointers(void)
{
	if (SidebarPlayerPtr) {
		((HouseClass *&)SidebarPlayerPtr) = (HouseClass *)SidebarPlayerPtr->Class->House;
	} else {
		((HouseClass *&)SidebarPlayerPtr) = (HouseClass *)HOUSE_NONE;
	}	
}


/***********************************************************************************************
 * SidebarGlyphxClass::Decode_Pointers -- Converts classes savable representation to run-time  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   9/25/2019 5:36PM ST : Created.                                                            *
 *=============================================================================================*/
void SidebarGlyphxClass::Decode_Pointers(void)
{
	if (*((HousesType*)&SidebarPlayerPtr) == HOUSE_NONE) {
		SidebarPlayerPtr = NULL;
	} else {
		((HouseClass *&)SidebarPlayerPtr) = HouseClass::As_Pointer(*((HousesType*)&SidebarPlayerPtr));
	}
}


/***********************************************************************************************
 * SidebarGlyphxClass::Load -- Loads from a save game file.                                    *
 *                                                                                             *
 * INPUT:   file  -- The file to read the data from.                                           *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   9/26/2019 10:57AM ST : Created.                                                           *
 *=============================================================================================*/
bool SidebarGlyphxClass::Load(Straw &file)
{
	::new (this) SidebarGlyphxClass();

	//bool ok = Read_Object(this, sizeof(*this), file, false);
	if (file.Get(this, sizeof(*this)) != sizeof(*this)) {
		return false;
	}

	Column[0].Set_Parent_Sidebar(this);
	Column[1].Set_Parent_Sidebar(this);

	return true;
}


/***********************************************************************************************
 * SidebarGlyphxClass::Save -- Write to a save game file.                                      *
 *                                                                                             *
 * INPUT:   file  -- The file to write the data to.                                            *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   9/26/2019 10:57AM ST : Created.                                                           *
 *=============================================================================================*/
bool SidebarGlyphxClass::Save(Pipe &file)
{
	//return(Write_Object(this, sizeof(*this), file));
	
	file.Put(this, sizeof(*this));
	
	return true;
}


extern SidebarGlyphxClass *Get_Current_Context_Sidebar(HouseClass *player_ptr);

void Sidebar_Glyphx_Init_Clear(HouseClass *player_ptr)
{
	SidebarGlyphxClass *sidebar = Get_Current_Context_Sidebar(player_ptr);
	if (sidebar) {
		sidebar->Init_Clear(player_ptr);
	}
}

void Sidebar_Glyphx_Init_IO(HouseClass *player_ptr)
{
	SidebarGlyphxClass *sidebar = Get_Current_Context_Sidebar(player_ptr);
	if (sidebar) {
		sidebar->Init_IO();
	}
}

bool Sidebar_Glyphx_Abandon_Production(RTTIType type, int factory, HouseClass *player_ptr)
{
	SidebarGlyphxClass *sidebar = Get_Current_Context_Sidebar(player_ptr);
	if (sidebar) {
		return sidebar->Abandon_Production(type, factory);
	}

	return false;
}

bool Sidebar_Glyphx_Add(RTTIType type, int id, HouseClass *player_ptr, bool via_capture)
{
	SidebarGlyphxClass *sidebar = Get_Current_Context_Sidebar(player_ptr);
	if (sidebar) {
		return sidebar->Add(type, id, via_capture);
	}

	return false;
}

void Sidebar_Glyphx_Recalc(HouseClass *player_ptr)
{
	SidebarGlyphxClass *sidebar = Get_Current_Context_Sidebar(player_ptr);
	if (sidebar) {
		sidebar->Recalc();
	}
}

void Sidebar_Glyphx_AI(HouseClass *player_ptr, KeyNumType & input)
{
	SidebarGlyphxClass *sidebar = Get_Current_Context_Sidebar(player_ptr);
	if (sidebar) {
		sidebar->AI(input, 0, 0);
	}
}

bool Sidebar_Glyphx_Factory_Link(int factory, RTTIType type, int id, HouseClass *player_ptr)
{
	SidebarGlyphxClass *sidebar = Get_Current_Context_Sidebar(player_ptr);
	if (sidebar) {
		return sidebar->Factory_Link(factory, type, id);
	}

	return false;
}

bool Sidebar_Glyphx_Save(Pipe &file, SidebarGlyphxClass *sidebar)
{
	if (sidebar) {
		return sidebar->Save(file);
	}
	return false;
}			  

bool Sidebar_Glyphx_Load(Straw &file, SidebarGlyphxClass *sidebar)
{
	if (sidebar) {
		return sidebar->Load(file);
	}
	return false;
}			  

void Sidebar_Glyphx_Code_Pointers(SidebarGlyphxClass *sidebar)
{
	if (sidebar) {
		sidebar->Code_Pointers();
	}
}			  

void Sidebar_Glyphx_Decode_Pointers(SidebarGlyphxClass *sidebar)
{
	if (sidebar) {
		sidebar->Decode_Pointers();
	}
}