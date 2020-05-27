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

/* $Header: /CounterStrike/BUILDING.CPP 5     3/13/97 5:18p Joe_b $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BUILDING.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : October 27, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BuildingClass::AI -- Handles non-graphic AI processing for buildings.                     *
 *   BuildingClass::Active_Click_With -- Handles cell selection for buildings.                 *
 *   BuildingClass::Animation_AI -- Handles normal building animation processing.              *
 *   BuildingClass::Assign_Target -- Assigns a target to the building.                         *
 *   BuildingClass::Begin_Mode -- Begins an animation mode for the building.                   *
 *   BuildingClass::BuildingClass -- Constructor for buildings.                                *
 *   BuildingClass::Can_Demolish -- Can the player demolish (sell back) the building?          *
 *   BuildingClass::Can_Enter_Cell -- Determines if building can be placed down.               *
 *   BuildingClass::Can_Fire -- Determines if this building can fire.                          *
 *   BuildingClass::Can_Player_Move -- Can this building be moved?                             *
 *   BuildingClass::Captured -- Captures the building.                                         *
 *   BuildingClass::Center_Coord -- Fetches the center coordinate for the building.            *
 *   BuildingClass::Charging_AI -- Handles the special charging logic for Tesla coils.         *
 *   BuildingClass::Check_Point -- Fetches the landing checkpoint for the given flight pattern.*
 *   BuildingClass::Click_With -- Handles clicking on the map while the building is selected.  *
 *   BuildingClass::Crew_Type -- This determines the crew that this object generates.          *
 *   BuildingClass::Death_Announcement -- Announce the death of this building.                 *
 *   BuildingClass::Debug_Dump -- Displays building status to the monochrome screen.           *
 *   BuildingClass::Detach -- Handles target removal from the game system.                     *
 *   BuildingClass::Detach_All -- Possibly abandons production according to factory type.      *
 *   BuildingClass::Docking_Coord -- Fetches the coordinate to use for docking.                *
 *   BuildingClass::Draw_It -- Displays the building at the location specified.                *
 *   BuildingClass::Drop_Debris -- Drops rubble when building is destroyed.                    *
 *   BuildingClass::Enter_Idle_Mode -- The building will enter its idle mode.                  *
 *   BuildingClass::Exit_Coord -- Determines location where object will leave it.              *
 *   BuildingClass::Exit_Object -- Initiates an object to leave the building.                  *
 *   BuildingClass::Factory_AI -- Handle factory production and initiation.                    *
 *   BuildingClass::Find_Exit_Cell -- Find a clear location to exit an object from this buildin*
 *   BuildingClass::Fire_Direction -- Fetches the direction of firing.                         *
 *   BuildingClass::Fire_Out -- Handles when attached animation expires.                       *
 *   BuildingClass::Flush_For_Placement -- Handles clearing a zone for object placement.       *
 *   BuildingClass::Get_Image_Data -- Fetch the image pointer for the building.                *
 *   BuildingClass::Grand_Opening -- Handles construction completed special operations.        *
 *   BuildingClass::Greatest_Threat -- Searches for target that building can fire upon.        *
 *   BuildingClass::How_Many_Survivors -- This determine the maximum number of survivors.      *
 *   BuildingClass::Init -- Initialize the building system to an empty null state.             *
 *   BuildingClass::Limbo -- Handles power adjustment as building goes into limbo.             *
 *   BuildingClass::Mark -- Building interface to map rendering system.                        *
 *   BuildingClass::Mission_Attack -- Handles attack mission for building.                     *
 *   BuildingClass::Mission_Construction -- Handles mission construction.                      *
 *   BuildingClass::Mission_Deconstruction -- Handles building deconstruction.                 *
 *   BuildingClass::Mission_Guard -- Handles guard mission for combat buildings.               *
 *   BuildingClass::Mission_Harvest -- Handles refinery unloading harvesters.                  *
 *   BuildingClass::Mission_Missile -- State machine for nuclear missile launch.               *
 *   BuildingClass::Mission_Repair -- Handles the repair (active) state for building.          *
 *   BuildingClass::Mission_Unload -- Handles the unload mission for a building.               *
 *   BuildingClass::Pip_Count -- Determines "full" pips to display for building.               *
 *   BuildingClass::Power_Output -- Fetches the current power output from this building.       *
 *   BuildingClass::Read_INI -- Reads buildings from INI file.                                 *
 *   BuildingClass::Receive_Message -- Handle an incoming message to the building.             *
 *   BuildingClass::Remap_Table -- Fetches the remap table to use for this building.           *
 *   BuildingClass::Remove_Gap_Effect -- Stop a gap generator from jamming cells               *
 *   BuildingClass::Repair -- Initiates or terminates the repair process.                      *
 *   BuildingClass::Repair_AI -- Handle the repair (and sell) logic for the building.          *
 *   BuildingClass::Revealed -- Reveals the building to the specified house.                   *
 *   BuildingClass::Rotation_AI -- Process any turret rotation required of this building.      *
 *   BuildingClass::Sell_Back -- Controls the sell back (demolish) operation.                  *
 *   BuildingClass::Shape_Number -- Fetch the shape number for this building.                  *
 *   BuildingClass::Sort_Y -- Returns the building coordinate used for sorting.                *
 *   BuildingClass::Take_Damage -- Inflicts damage points upon a building.                     *
 *   BuildingClass::Target_Coord -- Return the coordinate to use when firing on this building. *
 *   BuildingClass::Toggle_Primary -- Toggles the primary factory state.                       *
 *   BuildingClass::Turret_Facing -- Fetches the turret facing for this building.              *
 *   BuildingClass::Unlimbo -- Removes a building from limbo state.                            *
 *   BuildingClass::Update_Buildables -- Informs sidebar of additional construction options.   *
 *   BuildingClass::Value -- Determine the value of this building.                             *
 *   BuildingClass::What_Action -- Determines action to perform if click on specified object.  *
 *   BuildingClass::What_Action -- Determines what action will occur.                          *
 *   BuildingClass::Write_INI -- Write out the building data to the INI file specified.        *
 *   BuildingClass::delete -- Deallocates building object.                                     *
 *   BuildingClass::new -- Allocates a building object from building pool.                     *
 *   BuildingClass::~BuildingClass -- Destructor for building type objects.                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/*
** New sidebar for GlyphX multiplayer. ST - 8/2/2019 2:35PM
*/
#include "SidebarGlyphx.h"

enum SAMState {
	SAM_READY,					// Launcher can be facing any direction tracking targets.
	SAM_FIRING					// Stationary while missile is being fired.
};


/***************************************************************************
**	Center of building offset table.
*/
COORDINATE const BuildingClass::CenterOffset[BSIZE_COUNT] = {
	0x00800080L,
	0x008000FFL,
	0x00FF0080L,
	0x00FF00FFL,
	0x018000FFL,
	0x00FF0180L,
	0x01800180L,

	0x00FF0200L,

	0x02800280L,
};


/***********************************************************************************************
 * BuildingClass::Receive_Message -- Handle an incoming message to the building.               *
 *                                                                                             *
 *    This routine handles an incoming message to the building. Messages regulate the          *
 *    various cooperative ventures between buildings and units. This might include such        *
 *    actions as coordinating the construction yard animation with the actual building's       *
 *    construction animation.                                                                  *
 *                                                                                             *
 * INPUT:   from     -- The originator of the message received.                                *
 *                                                                                             *
 *          message  -- The radio message received.                                            *
 *                                                                                             *
 *          param    -- Reference to an optional parameter that might be used to return        *
 *                      extra information to the message originator.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the response to the message (typically, this is just RADIO_OK).       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/09/1994 JLB : Created.                                                                 *
 *   06/26/1995 JLB : Forces refinery load anim to start immediately.                          *
 *   08/13/1995 JLB : Uses ScenarioInit for special loose "CAN_LOAD" check.                    *
 *=============================================================================================*/
RadioMessageType BuildingClass::Receive_Message(RadioClass * from, RadioMessageType message, long & param)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	switch (message) {

		/*
		**	This message is received as a request to attach/load/dock with this building.
		**	Verify that this is allowed and return the appropriate response.
		*/
		case RADIO_CAN_LOAD:
			TechnoClass::Receive_Message(from, message, param);
			if (!House->Is_Ally(from)) return(RADIO_STATIC);
			if (Mission == MISSION_CONSTRUCTION || Mission == MISSION_DECONSTRUCTION || BState == BSTATE_CONSTRUCTION || (!ScenarioInit && In_Radio_Contact() && Contact_With_Whom() != from)) return(RADIO_NEGATIVE);
			switch (Class->Type) {
				case STRUCT_AIRSTRIP:
					if (from->What_Am_I() == RTTI_AIRCRAFT && ((AircraftClass const *)from)->Class->IsFixedWing) {
						return(RADIO_ROGER);
					}
					break;

				case STRUCT_HELIPAD:
					if (from->What_Am_I() == RTTI_AIRCRAFT && !((AircraftClass const *)from)->Class->IsFixedWing) {
						return(RADIO_ROGER);
					}
					break;

				case STRUCT_REPAIR:
					if (from->What_Am_I() == RTTI_UNIT || (from->What_Am_I() == RTTI_AIRCRAFT)) {
						if (Transmit_Message(RADIO_ON_DEPOT, from) != RADIO_ROGER) {
							return(RADIO_ROGER);
						}
					}
					return(RADIO_NEGATIVE);

				case STRUCT_REFINERY:
					if (from->What_Am_I() == RTTI_UNIT &&
						*((UnitClass *)from) == UNIT_HARVESTER &&
						(ScenarioInit || !Is_Something_Attached())) {

						return(RADIO_ROGER);
					}
					break;

				default:
					break;
			}
			return(RADIO_STATIC);

		/*
		**	This message is received when the object has attached itself to this
		**	building.
		*/
		case RADIO_IM_IN:
			if (Mission == MISSION_DECONSTRUCTION) {
				return(RADIO_NEGATIVE);
			}
			switch (Class->Type) {
				case STRUCT_REPAIR:
					IsReadyToCommence = true;
					Assign_Mission(MISSION_REPAIR);
					from->Assign_Mission(MISSION_SLEEP);
					return(RADIO_ROGER);

				case STRUCT_AIRSTRIP:
				case STRUCT_HELIPAD:
					Assign_Mission(MISSION_REPAIR);
					from->Assign_Mission(MISSION_SLEEP);
					return(RADIO_ROGER);

				case STRUCT_REFINERY:
					Mark(MARK_CHANGE);
					from->Assign_Mission(MISSION_UNLOAD);
					return(RADIO_ROGER);

				default:
					break;
			}
			break;

		/*
		**	Docking maneuver maintenance message. See if new order should be given to the
		**	unit trying to dock.
		*/
		case RADIO_DOCKING:
			TechnoClass::Receive_Message(from, message, param);

			/*
			**	When in radio contact for loading, the refinery starts
			**	flashing the lights.
			*/
			if (*this == STRUCT_REFINERY && BState != BSTATE_FULL) {
				Begin_Mode(BSTATE_FULL);
			}

			/*
			**	If this building is already in radio contact, then it might
			**	be able to satisfy the request to load by bumping off any
			**	preoccupying task.
			*/
			if (*this == STRUCT_REPAIR) {
				if (Contact_With_Whom() != from) {
					if (Transmit_Message(RADIO_ON_DEPOT) == RADIO_ROGER) {
						if (Transmit_Message(RADIO_NEED_REPAIR) == RADIO_NEGATIVE) {
							Transmit_Message(RADIO_RUN_AWAY);
							Transmit_Message(RADIO_OVER_OUT);
							return(RADIO_ROGER);
						}
					}
				}
			}

			/*
			**	Establish contact with the object if this building isn't already in contact
			**	with another.
			*/
			if (!In_Radio_Contact()) {
				Transmit_Message(RADIO_HELLO, from);
			}

			if (Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {
				switch (Class->Type) {
					case STRUCT_AIRSTRIP:
						param = As_Target();
						break;

					case STRUCT_HELIPAD:
						param = As_Target();
						break;

					case STRUCT_REPAIR:
						Transmit_Message(RADIO_TETHER);
						param = ::As_Target(Coord_Cell(Center_Coord()));
						break;

					case STRUCT_REFINERY:
						param = ::As_Target(Coord_Cell(Adjacent_Cell(Center_Coord(), DIR_S)));
						break;
				}

				/*
				**	Tell the harvester to move to the docking pad of the building.
				*/
				if (Transmit_Message(RADIO_MOVE_HERE, param) == RADIO_YEA_NOW_WHAT) {

					/*
					**	Since the harvester is already there, tell it to begin the backup
					**	procedure now. If it can't, then tell it to get outta here.
					*/
					Transmit_Message(RADIO_TETHER);
					if (*this == STRUCT_REFINERY && Transmit_Message(RADIO_BACKUP_NOW, from) != RADIO_ROGER) {
						from->Scatter(NULL, true, true);
					}
				}
			}
			return(RADIO_ROGER);

		/*
		**	If a transport or harvester is requesting permission to head toward, dock
		**	and load/unload, check to make sure that this is allowed given the current
		**	state of the building.
		*/
		case RADIO_ARE_REFINERY:
			if (Is_Something_Attached() || In_Radio_Contact() || IsInLimbo || House->Class->House != from->Owner() || (*this != STRUCT_REFINERY/* && *this != STRUCT_REPAIR*/)) {
				return(RADIO_NEGATIVE);
			}
			return(RADIO_ROGER);

		/*
		**	Someone is telling us that it is starting construction. This should only
		**	occur if this is a construction yard and a building was just placed on
		**	the map.
		*/
		case RADIO_BUILDING:
			Assign_Mission(MISSION_REPAIR);
			TechnoClass::Receive_Message(from, message, param);
			return(RADIO_ROGER);

		/*
		**	Someone is telling us that they have finished construction. This should
		**	only occur if this is a construction yard and the building that was being
		**	constructed has finished. In this case, stop the construction yard
		**	animation.
		*/
		case RADIO_COMPLETE:
			if (Mission != MISSION_DECONSTRUCTION) {
				Assign_Mission(MISSION_GUARD);
			}
			TechnoClass::Receive_Message(from, message, param);
			return(RADIO_ROGER);

		/*
		**	This message may occur unexpectedly if the unit in contact with this
		**	building is suddenly destroyed. Handle any cleanup necessary. For example,
		**	a construction yard should stop its construction animation in this case.
		*/
		case RADIO_OVER_OUT:
			Begin_Mode(BSTATE_IDLE);
			TechnoClass::Receive_Message(from, message, param);
			return(RADIO_ROGER);

		/*
		**	This message is received when an object has completely left
		** building. Sometimes special cleanup action is required when
		**	this event occurs.
		*/
		case RADIO_UNLOADED:
			if (*this == STRUCT_REPAIR) {
				if (Distance(from) < 0x0180) {
					return(RADIO_ROGER);
				}
			}
			TechnoClass::Receive_Message(from, message, param);
			if (*this == STRUCT_WEAP || *this == STRUCT_AIRSTRIP || *this == STRUCT_REPAIR) return(RADIO_RUN_AWAY);
			return(RADIO_ROGER);

		default:
			break;
	}

	/*
	**	Pass along the message to the default message handler in the radio itself.
	*/
	return(TechnoClass::Receive_Message(from, message, param));
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * BuildingClass::Debug_Dump -- Displays building status to the monochrome screen.             *
 *                                                                                             *
 *    This utility function will output the current status of the building class to the        *
 *    monochrome screen. It is through this data that bugs may be fixed or detected.           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Debug_Dump(MonoClass * mono) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	mono->Set_Cursor(0, 0);
	mono->Print(Text_String(TXT_DEBUG_BUILDING));
	mono->Fill_Attrib(66, 13, 12, 1, IsRepairing ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 14, 12, 1, IsToRebuild ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 15, 12, 1, IsAllowedToSell ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 16, 12, 1, IsCharging ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 17, 12, 1, IsCharged ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 18, 12, 1, IsJamming ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 19, 12, 1, IsJammed ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Set_Cursor(1, 11);
	if (Factory) {
		mono->Printf("%s %d%%", Factory->Get_Object()->Class_Of().IniName, (100*Factory->Completion())/FactoryClass::STEP_COUNT);
	}

	TechnoClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * BuildingClass::Draw_It -- Displays the building at the location specified.                  *
 *                                                                                             *
 *    This is the low level graphic routine that displays the building at the location         *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   x,y   -- The coordinate to draw the building at.                                   *
 *                                                                                             *
 *          window   -- The clipping window to use.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *   06/27/1994 JLB : Takes a clipping window parameter.                                       *
 *   07/06/1995 JLB : Handles damaged silos correctly.                                         *
 *=============================================================================================*/
void BuildingClass::Draw_It(int x, int y, WindowNumberType window) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	/*
	**	The shape file to use for rendering depends on whether the building
	**	is undergoing construction or not.
	*/
	void const * shapefile = Get_Image_Data();
	if (shapefile == NULL) return;

	/*
	**	Actually draw the building shape.
	*/
	IsTheaterShape = Class->IsTheater;	//Let Build_Frame know if this is a theater specific shape
	Techno_Draw_Object(shapefile, Shape_Number(), x, y, window);
	IsTheaterShape = false;

	/*
	** Patch for adding overlay onto weapon factory.  Only add the overlay if
	** the building has more than 1 hp.  Also, if the building's in radio
	** contact, he must be unloading a constructed vehicle, so draw that
	** vehicle before drawing the overlay.
	*/
	if (BState != BSTATE_CONSTRUCTION) {

		/*
		**	A Tethered object is always rendered AFTER the building.
		*/
		if (*this == STRUCT_WEAP && IsTethered && In_Radio_Contact() && !Contact_With_Whom()->IsInLimbo && Contact_With_Whom()->What_Am_I() != RTTI_BUILDING) {
			TechnoClass * contact = Contact_With_Whom();

			assert(contact->IsActive);
			int xxx = x + ((int)Lepton_To_Pixel((int)Coord_X(contact->Render_Coord())) - (int)Lepton_To_Pixel((int)Coord_X(Render_Coord())));
			int yyy = y + ((int)Lepton_To_Pixel((int)Coord_Y(contact->Render_Coord())) - (int)Lepton_To_Pixel((int)Coord_Y(Render_Coord())));
			contact->Draw_It(xxx, yyy, window);
			contact->IsToDisplay = false;
		}

		/*
		**	Draw the weapon factory custom overlay graphic.
		*/
		if ( (*this == STRUCT_WEAP || *this == STRUCT_FAKEWEAP)) {
			int shapenum = Door_Stage();
			if (Health_Ratio() <= Rule.ConditionYellow) shapenum += 4;
			// Added override shape file name. ST - 8/1/2019 5:24PM
			//Techno_Draw_Object(Class->WarFactoryOverlay, shapenum, x, y, window);
			Techno_Draw_Object_Virtual(Class->WarFactoryOverlay, shapenum, x, y, window, DIR_N, 0x0100, "WEAP2");
		}

		/*
		**	Draw any repair feedback graphic required.
		*/
		if (IsRepairing && IsWrenchVisible) {
			CC_Draw_Shape(ObjectTypeClass::SelectShapes, SELECT_WRENCH, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
		}
	}

	TechnoClass::Draw_It(x, y, window);

	/*
	** If this is a factory that we're spying on, show what it's producing
	*/
	if ((Spied_By() & (1<<(PlayerPtr->Class->House)) && Is_Selected_By_Player()) || ((window == WINDOW_VIRTUAL) && (Session.Type != GAME_NORMAL))) {

		/*
		**	Fetch the factory that is associate with this building. For computer controlled buildings, the
		**	factory pointer is integral to the building itself. For human controlled buildings, the factory
		**	pointer is part of the house structure and must be retrieved from there.
		*/
		FactoryClass * factory = NULL;
		if (House->IsHuman) {
			factory = House->Fetch_Factory(Class->ToBuild);
		} else {
			factory = Factory;
		}

		/*
		**	If there is a factory associated with this building, then fetch any attached
		**	object under production and display its cameo image over the top of this building.
		*/
		if (factory != NULL) {
			TechnoClass * obj = factory->Get_Object();
			if (obj != NULL) {
#ifdef FIXIT_CSII
				CC_Draw_Shape(obj, obj->Techno_Type_Class()->Get_Cameo_Data(), 0, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_NORMAL, NULL);
#else
				void const * remapper = obj->House->Remap_Table(false, obj->Techno_Type_Class()->Remap);
				CC_Draw_Shape(obj->Techno_Type_Class()->Get_Cameo_Data(), 0, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL | ((remapper != NULL) ? SHAPE_FADING : SHAPE_NORMAL), remapper);
#endif
			}
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Shape_Number -- Fetch the shape number for this building.                    *
 *                                                                                             *
 *    This routine will examine the current state of the building and return with the shape    *
 *    number to use. The shape number is subordinate to the building graphic image data.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the shape number to use when rendering this building.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Shape_Number(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	int shapenum = Fetch_Stage();

	/*
	**	The shape file to use for rendering depends on whether the building
	**	is undergoing construction or not.
	*/
	if (BState == BSTATE_CONSTRUCTION) {

		/*
		**	If the building is deconstructing, then the display frame progresses
		**	from the end to the beginning. Reverse the shape number accordingly.
		*/
		if (Mission == MISSION_DECONSTRUCTION) {
			shapenum = (Class->Anims[BState].Start+Class->Anims[BState].Count-1)-shapenum;
		}

	} else {

		/*
		**	If this is a camouflaged pill box and it is not owned by the player, then
		**	it is displayed with the MEGA-camouflaged imagery.
		*/
		if ((!IsOwnedByPlayer) && (*this == STRUCT_CAMOPILLBOX)) {
			shapenum += 1;
		}

		/*
		**	The Tesla Coil has a stage value that can be overridden by
		**	its current state.
		*/
		if (*this == STRUCT_TESLA) {
			if (IsCharged) {
				shapenum = 3;
			} else {
				if (IsCharging) {
					shapenum = Fetch_Stage();
				} else {
					shapenum = 0;
				}
			}
		}

		/*
		**	Buildings that contain a turret handle their shape determination
		**	differently than normal buildings. They need to take into consideration
		**	the direction the turret is facing.
		*/
		if (Class->IsTurretEquipped) {
			shapenum = UnitClass::BodyShape[Dir_To_32(PrimaryFacing.Current())];

			if (*this == STRUCT_SAM) {

				/*
				**	SAM sites that are free to rotate fetch their animation frame
				**	from the building's turret facing. All other animation stages
				**	fetch their frame from the embedded animation sequencer.
				*/
//				if (Status == SAM_READY || Status == SAM_FIRING || Status == SAM_LOCKING) {
//					shapenum = Fetch_Stage();
//				}
				if (Health_Ratio() <= Rule.ConditionYellow) {
					shapenum += 35;
				}
			} else {
				if (IsInRecoilState) {
					shapenum += 32;
				}
				if (Health_Ratio() <= Rule.ConditionYellow) {
					shapenum += 64;
				}
			}
		} else {

			/*
			**	If it is a significantly damaged weapons factory, it is shown in
			**	the worst state possible.
			*/
			if (*this == STRUCT_WEAP || *this == STRUCT_FAKEWEAP) {
				shapenum = 0;
				if (Health_Ratio() <= Rule.ConditionYellow) {
					shapenum = 1;
				}

			} else {

				/*
				**	Special render stage for silos. The stage is dependent on the current
				**	Tiberium collected as it relates to Tiberium capacity.
				*/
				if (*this == STRUCT_STORAGE) {

					int level = 0;
					if (House->Capacity) {
						level = (House->Tiberium * 5) / House->Capacity;
					}

					shapenum += Bound(level, 0, 4);
					if (Health_Ratio() <= Rule.ConditionYellow) {
						shapenum += 5;
					}

				} else {

					/*
					**	If below half strenth, then show the damage frames of the
					**	building.
					*/
					if (Health_Ratio() <= Rule.ConditionYellow) {
						if (*this == STRUCT_CHRONOSPHERE) {
							shapenum += 29;
						} else {
							int last1 = Class->Anims[BSTATE_IDLE].Start + Class->Anims[BSTATE_IDLE].Count;
							int last2 = Class->Anims[BSTATE_ACTIVE].Start + Class->Anims[BSTATE_ACTIVE].Count;
							int largest = max(last1, last2);
							last2 = Class->Anims[BSTATE_AUX1].Start + Class->Anims[BSTATE_AUX1].Count;
							largest = max(largest, last2);
							last2 = Class->Anims[BSTATE_AUX2].Start + Class->Anims[BSTATE_AUX2].Count;
							largest = max(largest, last2);
							shapenum += largest;
						}
					}
				}
			}
		}
	}
	return(shapenum);
}


/***********************************************************************************************
 * BuildingClass::Mark -- Building interface to map rendering system.                          *
 *                                                                                             *
 *    This routine is used to mark the map cells so that when it renders                       *
 *    the underlying icons will also be updated as necessary.                                  *
 *                                                                                             *
 * INPUT:   mark  -- Type of image change (MARK_UP, _DOWN, _CHANGE)                            *
 *             MARK_UP  -- Building is removed.                                                *
 *             MARK_CHANGE -- Building changes shape.                                          *
 *             MARK_DOWN -- Building is added.                                                 *
 *                                                                                             *
 * OUTPUT:  bool; Did the mark operation succeed? Failure could be the result of marking down  *
 *                when the building is already marked down, or visa versa.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/31/1994 JLB : Created.                                                                 *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   04/16/1994 JLB : Added health bar tracking.                                               *
 *   12/23/1994 JLB : Calls low level check before proceeding.                                 *
 *   01/27/1995 JLB : Special road spacer template added.                                      *
 *=============================================================================================*/
bool BuildingClass::Mark(MarkType mark)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (TechnoClass::Mark(mark)) {
		short const * offset = Overlap_List();
		short const * occupy = Occupy_List();
		CELL cell = Coord_Cell(Coord);
		SmudgeType bib;

		switch (mark) {
			case MARK_UP:
				Map.Pick_Up(cell, this);
				if (Class->Bib_And_Offset(bib, cell)) {
					SmudgeClass * smudge = new SmudgeClass(bib);
					if (smudge != NULL) {
						smudge->Disown(cell);
						delete smudge;
					}
				}
				break;

			case MARK_DOWN:

				/*
				**	Special wall logic is handled here. A building that is really a wall
				**	gets converted into an overlay wall type when it is placed down. The
				**	actual building object itself is destroyed.
				*/
				if (Class->IsWall) {
					switch (Class->Type) {
						case STRUCT_BRICK_WALL:
							new OverlayClass(OVERLAY_BRICK_WALL, cell, House->Class->House);
							break;

						case STRUCT_BARBWIRE_WALL:
							new OverlayClass(OVERLAY_BARBWIRE_WALL, cell, House->Class->House);
							break;

						case STRUCT_SANDBAG_WALL:
							new OverlayClass(OVERLAY_SANDBAG_WALL, cell, House->Class->House);
							break;

						case STRUCT_WOOD_WALL:
							new OverlayClass(OVERLAY_WOOD_WALL, cell, House->Class->House);
							break;

						case STRUCT_CYCLONE_WALL:
							new OverlayClass(OVERLAY_CYCLONE_WALL, cell, House->Class->House);
							break;

						case STRUCT_FENCE:
							new OverlayClass(OVERLAY_FENCE, cell, House->Class->House);
							break;

						default:
							break;
					}
					Transmit_Message(RADIO_OVER_OUT);
					delete this;

				} else {
					if (Can_Enter_Cell(cell) == MOVE_OK) {
						/*
						**	Determine if a bib is required for this building. If one is, then
						**	create and place it.
						*/
						CELL newcell = cell;
						if (Class->Bib_And_Offset(bib, newcell)) {
							new SmudgeClass(bib, Cell_Coord(newcell), Class->IsBase ? House->Class->House : HOUSE_NONE);
						}

						Map.Place_Down(cell, this);
					} else {
						return(false);
					}
				}
				break;

			case MARK_CHANGE_REDRAW:
				Map.Refresh_Cells(cell, Overlap_List(true));
				break;

			default:
				Map.Refresh_Cells(cell, Overlap_List(false));
				Map.Refresh_Cells(cell, occupy);
				break;
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * BuildingClass::AI -- Handles non-graphic AI processing for buildings.                       *
 *                                                                                             *
 *    This function is to handle the AI logic for the building. The graphic logic (facing,     *
 *    firing, and animation) is handled elsewhere.                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   12/26/1994 JLB : Handles production.                                                      *
 *   06/11/1995 JLB : Revamped.                                                                *
 *=============================================================================================*/
void BuildingClass::AI(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	/*
	**	Process building animation state changes. Transition to a following state
	**	if there is one specified and the current animation sequence has expired.
	**	This process must occur before mission AI since the mission AI relies on
	**	the bstate change to occur immediately before the MissionClass::AI.
	*/
	Animation_AI();

	/*
	**	If now is a good time to act on a new mission, then do so. This process occurs
	**	here because some outside event may have requested a mission change for the building.
	**	Such outside requests (player input) must be initiated BEFORE the normal AI process.
	*/
	if (IsReadyToCommence && BState != BSTATE_CONSTRUCTION) {

		/*
		**	Clear the commencement flag ONLY if something actually occurred. By acting
		**	this way, a building can set the IsReadyToCommence flag before it goes
		**	to "sleep" knowing that it will wake up as soon as a new mission comes
		**	along.
		*/
		if (Commence()) {
			IsReadyToCommence = false;
		}
	}

	/*
	**	Proceed with normal logic processing. This is where the mission processing
	**	occurs. This call must be located after the animation sequence makes the
	**	transition to the next frame (see above) in order for the mission logic to
	**	act at the exact moment of graphic transition BEFORE it has a chance to
	**	be displayed.
	*/
	TechnoClass::AI();

	/*
	**	Bail if the object died in the AI routine.
	*/
	if (!IsActive) {
		return;
	}

	/*
	**	Building ammo is instantly reloaded.
	*/
	if (!Ammo) {
		Ammo = Class->MaxAmmo;
	}

	/*
	**	If now is a good time to act on a new mission, then do so. This occurs here because
	**	some AI event may have requested a mission change (usually from another mission
	**	state machine). This must occur here before it has a chance to render.
	*/
	if (IsReadyToCommence) {

		/*
		**	Clear the commencement flag ONLY if something actually occurred. By acting
		**	this way, a building can set the IsReadyToCommence flag before it goes
		**	to "sleep" knowing that it will wake up as soon as a new mission comes
		**	along.
		*/
		if (Commence()) {
			IsReadyToCommence = false;
		}
	}

	/*
	**	If a change of animation was requested, then make the change
	**	now. The building animation system acts independently but subordinate
	**	to the mission state machine system. By performing the animation change-up
	**	here, the mission AI system is ensured of immediate visual affect when it
	**	decides to change the animation state of the building.
	*/
	if (QueueBState != BSTATE_NONE) {
		if (BState != QueueBState) {
			BState = QueueBState;
			BuildingTypeClass::AnimControlType const * ctrl = Fetch_Anim_Control();
			if (BState == BSTATE_CONSTRUCTION || BState == BSTATE_IDLE) {
				Set_Rate(Options.Normalize_Delay(ctrl->Rate));
			} else {
				Set_Rate(ctrl->Rate);
			}
			Set_Stage(ctrl->Start);
		}
		QueueBState = BSTATE_NONE;
	}

	/*
	**	If the building's strength has changed, then update the power
	**	accordingly.
	*/
	if (Strength != LastStrength) {
		int oldpower = Power_Output();
		LastStrength = Strength;
		int newpower = Power_Output();
		House->Adjust_Power(newpower - oldpower);
	}

	/*
	**	Check to see if the destruction countdown timer is active. If so, then decrement it.
	**	When this timer reaches zero, the building is removed from the map. All the explosions
	**	are presumed to be in progress at this time.
	*/
	if (Strength == 0) {
		if (CountDown == 0) {
			Limbo();
			Drop_Debris(WhomToRepay);
			delete this;
		}
		return;
	}

	/*
	**	Charging logic.
	*/
	Charging_AI();

	/*
	**	Handle any repair process that may be going on.
	*/
	Repair_AI();

	/*
	**	For computer controlled buildings, determine what should be produced and start
	**	production accordingly.
	*/
	Factory_AI();

	/*
	**	Check for demolition timeout. When timeout has expired, the building explodes.
	*/
	if (IsGoingToBlow && CountDown == 0) {

		/*
		** Maybe trigger an achivement. ST - 11/14/2019 1:53PM
		*/
		TechnoTypeClass const *object_type = Techno_Type_Class();
		if (object_type) {
			TechnoClass *saboteur = As_Techno(WhomToRepay);
			if (saboteur && saboteur->IsActive && saboteur->House && saboteur->House->IsHuman) {
				On_Achievement_Event(saboteur->House, "BUILDING_DESTROYED_C4", object_type->IniName);
			}
		}			
		
		int damage = Strength;
		Take_Damage(damage, 0, WARHEAD_FIRE, As_Techno(WhomToRepay), true);
		if (!IsActive) {
			return;
		}
		Mark(MARK_CHANGE);
	}

	/*
	**	Turret equiped buildings must handle turret rotation logic here. This entails
	**	rotating the turret to the desired facing as well as figuring out what that
	**	desired facing should be.
	*/
	Rotation_AI();

	/*
	** Gap Generators need to scan if they've just become activated, or if
	** the power has just come on enough so they can scan.  Also, they need
	** to un-jam if the power has just dropped off.
	*/
	if (*this == STRUCT_GAP) {
		if (Arm == 0) {
			IsJamming = false;
			Arm = TICKS_PER_MINUTE * Rule.GapRegenInterval + Random_Pick(1, TICKS_PER_SECOND);
		}

		if (!IsJamming) {
			if (House->Power_Fraction() >= 1) {
				Map.Jam_From(Coord_Cell(Center_Coord()), Rule.GapShroudRadius, House);
				IsJamming = true;
			}
		} else {
			if (House->Power_Fraction() < 1) {
				IsJamming = false;
				Map.UnJam_From(Coord_Cell(Center_Coord()), Rule.GapShroudRadius, House);
			}
		}
	}

	/*
	** Radar facilities and SAMs need to check for the proximity of a mobile
	** radar jammer.
	*/
	if ((*this == STRUCT_RADAR || *this == STRUCT_SAM) && (Frame % TICKS_PER_SECOND) == 0) {
		IsJammed = false;
		for (int index = 0; index < Units.Count(); index++) {
			UnitClass * obj = Units.Ptr(index);
			if (obj != NULL &&
					!obj->IsInLimbo &&
					!obj->House->Is_Ally(House) &&
					obj->Class->IsJammer &&
					Distance(obj) <= Rule.RadarJamRadius) {

				IsJammed = true;
				break;
			}
		}
	}

	/*
	** Chronosphere active animation control.
	*/
	if (*this == STRUCT_CHRONOSPHERE && BState == BSTATE_ACTIVE && QueueBState == BSTATE_NONE && Scen.FadeTimer == 0) {
		Begin_Mode(BSTATE_IDLE);
	}
}


/***********************************************************************************************
 * BuildingClass::Unlimbo -- Removes a building from limbo state.                              *
 *                                                                                             *
 *    Use this routine to transform a building that has been held in limbo                     *
 *    state, into one that really exists on the map. Once a building as                        *
 *    been unlimboed, then it becomes a normal object in the game world.                       *
 *                                                                                             *
 * INPUT:   pos   -- The position to place the building on the map.                            *
 *                                                                                             *
 *          dir (optional) -- not used for this class                                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the unlimbo successful?                                                  *
 *                                                                                             *
 * WARNINGS:   The unlimbo operation might not be successful if the                            *
 *             building could not be placed at the location specified.                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/16/1994 JLB : Created.                                                                 *
 *   06/07/1994 JLB : Matches virtual function format for base class.                          *
 *   05/09/1995 JLB : Handles wall placement.                                                  *
 *   06/18/1995 JLB : Checks for wall legality before placing down.                            *
 *=============================================================================================*/
bool BuildingClass::Unlimbo(COORDINATE coord, DirType dir)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	/*
	**	If this is a wall type building, then it never gets unlimboed. Instead, it gets
	**	converted to an overlay type.
	*/
	if (Class->IsWall) {
		if (Can_Enter_Cell(Coord_Cell(coord), FACING_NONE) == MOVE_OK) {
			OverlayType otype = OVERLAY_NONE;
			switch (Class->Type) {
				case STRUCT_SANDBAG_WALL:
					otype	= OVERLAY_SANDBAG_WALL;
					break;

				case STRUCT_CYCLONE_WALL:
					otype = OVERLAY_CYCLONE_WALL;
					break;

				case STRUCT_BRICK_WALL:
					otype = OVERLAY_BRICK_WALL;
					break;

				case STRUCT_BARBWIRE_WALL:
					otype = OVERLAY_BARBWIRE_WALL;
					break;

				case STRUCT_WOOD_WALL:
					otype = OVERLAY_WOOD_WALL;
					break;

				case STRUCT_FENCE:
					otype = OVERLAY_FENCE;
					break;

				default:
					otype = OVERLAY_NONE;
					break;

			}
			if (otype != OVERLAY_NONE) {
				ObjectClass * o = OverlayTypeClass::As_Reference(otype).Create_One_Of(House);
				if (o && o->Unlimbo(coord)) {
					Map[coord].Owner = House->Class->House;
					Transmit_Message(RADIO_OVER_OUT);
					Map.Sight_From(Coord_Cell(coord), Class->SightRange, House);
					delete this;
					return(true);
				}
			}
		}
		return(false);
	}

	/*
	**	Normal building unlimbo process.
	*/
	if (TechnoClass::Unlimbo(coord, dir)) {

		/*
		**	Ensure that the owning house knows about the
		**	new object.
		*/
		House->BScan |= (1L << Class->Type);
		House->ActiveBScan |= (1L << Class->Type);

		/*
		**	Recalculate the center point of the house's base.
		*/
		House->Recalc_Center();

		/*
		**	Update the total factory type, assuming this building has a factory.
		*/
		House->Active_Add(this);

		/*
		**	Possibly the sidebar will be affected by this addition.
		*/
		House->IsRecalcNeeded = true;
		LastStrength = 0;

		// Changes to support client/server multiplayer. ST - 8/2/2019 2:36PM
		//if ((!IsDiscoveredByPlayer && Map[coord].IsVisible) || Session.Type != GAME_NORMAL) {
		if ((!Is_Discovered_By_Player(House) && Map[coord].Is_Visible(House)) || Session.Type != GAME_NORMAL) {
			if (House->IsHuman) {
				//Revealed(PlayerPtr);
				Revealed(House);
			}
		}
		if (!House->IsHuman) {
			Revealed(House);
		}

		// Changes to support client/server multiplayer. ST - 8/2/2019 2:36PM
		//if (IsOwnedByPlayer) {
		if (Is_Owned_By_Player()) {
			Map.PowerClass::IsToRedraw = true;
			Map.Flag_To_Redraw(false);
		}

		if ((Class->Ownable & (HOUSEF_GOOD | HOUSEF_BAD)) != (HOUSEF_GOOD | HOUSEF_BAD)) {
			if (Class->Ownable & HOUSEF_GOOD) {
				ActLike = HOUSE_GREECE;
			} else {
				ActLike = HOUSE_USSR;
			}
		}

		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * BuildingClass::Take_Damage -- Inflicts damage points upon a building.                       *
 *                                                                                             *
 *    This routine will inflict damage points upon the specified building.                     *
 *    It will handle the damage animation and building destruction. Use                        *
 *    this routine whenever a building is attacked.                                            *
 *                                                                                             *
 * INPUT:   damage   -- Amount of damage to inflict.                                           *
 *                                                                                             *
 *          distance -- The distance from the damage center point to the object's center point.*
 *                                                                                             *
 *          warhead  -- The kind of damage to inflict.                                         *
 *                                                                                             *
 *          source   -- The source of the damage. This is used to change targeting.            *
 *                                                                                             *
 *          forced   -- Is the damage forced upon the object regardless of whether it          *
 *                      is normally immune?                                                    *
 *                                                                                             *
 * OUTPUT:  true/false; Was the building destroyed?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/21/1991     : Created.                                                                 *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   04/16/1994 JLB : Added warhead modifier to damage.                                        *
 *   06/03/1994 JLB : Added source of damage as target value.                                  *
 *   06/20/1994 JLB : Source is a base class pointer.                                          *
 *   11/22/1994 JLB : Shares base damage handler for techno objects.                           *
 *   07/15/1995 JLB : Power ratio gets adjusted.                                               *
 *=============================================================================================*/
ResultType BuildingClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source, bool forced)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	ResultType res = RESULT_NONE;
	int shakes;

	if (this != source /*&& !Class->IsInsignificant*/) {

		if (source) {
			House->LATime = Frame;
			House->LAType = source->What_Am_I();
			House->LAZone = House->Which_Zone(this);
			House->LAEnemy = source->Owner();

			if (!House->Is_Ally(source)) {
				House->Enemy = source->Owner();
			}

			Base_Is_Attacked(source);
		}

		short const * offset = Occupy_List();

		/*
		** Memorize who they used to be in radio contact with.
		*/
		TechnoClass *tech = Contact_With_Whom();
		/*
		**	Perform the low level damage assessment.
		*/
		res = TechnoClass::Take_Damage(damage, distance, warhead, source, forced);
		switch (res) {
			case RESULT_DESTROYED:

				/*
				**	Add the building to the base prebuild list if allowed. This will force
				**	the computer to rebuild this structure if it can.
				*/
				if (IsToRebuild && Class->Level != -1 && Base.House == House->Class->House && Base.Get_Node(this) == 0) {
//				if (IsToRebuild && Class->IsBuildable && Base.House == House->Class->House && Base.Get_Node(this) == 0) {
					Base.Nodes.Add(BaseNodeClass(Class->Type, Coord_Cell(Coord)));
				}

				/*
				**	Destroy all attached objects.
				*/
				while (Attached_Object()) {
					FootClass * obj = Detach_Object();

					Detach_All(true);
					delete obj;
				}

				/*
				** If we were in contact with a landed plane, blow the plane up too.
				*/
				if (tech && tech->IsActive && tech->What_Am_I() == RTTI_AIRCRAFT && ((AircraftClass *)tech)->Class->IsFixedWing && ((AircraftClass *)tech)->In_Which_Layer() == LAYER_GROUND) {
					int damage = 500;
					tech->Take_Damage(damage, 0, WARHEAD_AP, source, forced);
				}

				Sound_Effect(VOC_KABOOM22, Coord);
				while (*offset != REFRESH_EOL) {
					CELL cell = Coord_Cell(Coord) + *offset++;

					/*
					**	If the building is destroyed, then lots of
					**	explosions occur.
					*/
					new SmudgeClass(Random_Pick(SMUDGE_CRATER1, SMUDGE_CRATER6), Cell_Coord(cell));
					if (Percent_Chance(50)) {
						new AnimClass(ANIM_FIRE_SMALL, Coord_Scatter(Cell_Coord(cell), 0x0080), Random_Pick(0, 7), Random_Pick(1, 3));
						if (Percent_Chance(50)) {
							new AnimClass(ANIM_FIRE_MED, Coord_Scatter(Cell_Coord(cell), 0x0040), Random_Pick(0, 7), Random_Pick(1, 3));
						}
					}
					new AnimClass(ANIM_FBALL1, Coord_Scatter(Cell_Coord(cell), 0x0040), Random_Pick(0, 3));
				}

				shakes = Class->Cost_Of() / 400;
				if (shakes) {
					Shake_The_Screen(shakes, Owner());
					if (source && Owner() != source->Owner()) {
						Shake_The_Screen(shakes, source->Owner());
					}
				}
				Sound_Effect(VOC_CRUMBLE, Coord);
				if (Mission == MISSION_DECONSTRUCTION) {
					CountDown = 0;
					Set_Rate(0);
				} else {
					CountDown = 8;
				}

				/*
				**	If it is in radio contact and the object seems to be attached, then tell
				**	it to run away.
				*/
				if (In_Radio_Contact() && Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {
					Transmit_Message(RADIO_RUN_AWAY);
				}

				/*
				**	A force destruction will not generate survivors.
				*/
				if (forced || *this == STRUCT_KENNEL) {
					IsSurvivorless = true;
				}

				/*
				** Destruction of a radar facility or advanced communications
				** center will cause the spiedby field to change...
				*/
				if (SpiedBy) {
					SpiedBy = 0;
					StructType struc = *this;
					if (struc == STRUCT_RADAR /* || struc == STRUCT_EYE */) {
						Update_Radar_Spied();
					}
				}

				/*
				** Destruction of a gap generator will cause the cells it affects
				** to stop being jammed.
				*/
				if (*this == STRUCT_GAP) {
					Remove_Gap_Effect();
				}

				/*
				** Destruction of a shipyard or sub pen may cause attached ships
				** who are repairing themselves to discontinue repairs.
				*/
				if (*this == STRUCT_SHIP_YARD || *this == STRUCT_SUB_PEN) {
					for (int index = 0; index < Vessels.Count(); index++) {
						VesselClass *obj = Vessels.Ptr(index);
						if (obj && !obj->IsInLimbo && obj->House == House) {
							if (obj->IsSelfRepairing) {
								if (::Distance(Center_Coord(), obj->Center_Coord()) < 0x0200) {
									obj->IsSelfRepairing = false;
									obj->IsToSelfRepair = false;
								}
							}
						}
					}
				}

				/*
				** Destruction of a barrel will cause the surrounding squares to
				** be hit with damage.
				*/
				if (*this == STRUCT_BARREL || *this == STRUCT_BARREL3) {
					COORDINATE center = Center_Coord();
					CELL cellcenter = Coord_Cell(center);

					BulletClass * bullet;

					bullet = new BulletClass(BULLET_INVISIBLE, ::As_Target(Adjacent_Cell(cellcenter, FACING_N)), 0, 200, WARHEAD_FIRE, MPH_MEDIUM_FAST);
					if (bullet) {
						bullet->Unlimbo(center, DIR_N);
					}

					bullet = new BulletClass(BULLET_INVISIBLE, ::As_Target(Adjacent_Cell(cellcenter, FACING_E)), 0, 200, WARHEAD_FIRE, MPH_MEDIUM_FAST);
					if (bullet) {
						bullet->Unlimbo(center, DIR_E);
					}

					bullet = new BulletClass(BULLET_INVISIBLE, ::As_Target(Adjacent_Cell(cellcenter, FACING_S)), 0, 200, WARHEAD_FIRE, MPH_MEDIUM_FAST);
					if (bullet) {
						bullet->Unlimbo(center, DIR_S);
					}

					bullet = new BulletClass(BULLET_INVISIBLE, ::As_Target(Adjacent_Cell(cellcenter, FACING_W)), 0, 200, WARHEAD_FIRE, MPH_MEDIUM_FAST);
					if (bullet) {
						bullet->Unlimbo(center, DIR_W);
					}
				}

				if (House) {
					House->Check_Pertinent_Structures();
				}

				break;

			case RESULT_HALF:
				if (*this == STRUCT_PUMP) {
					AnimClass * anim = new AnimClass(ANIM_OILFIELD_BURN, Coord_Add(Coord, 0x00400130L), 1);
					if (anim) {
						anim->Attach_To(this);
					}
				}
				// Fall into next case.

			case RESULT_MAJOR:
				Sound_Effect(VOC_KABOOM1, Coord);
				while (*offset != REFRESH_EOL) {
					CELL cell = Coord_Cell(Coord) + *offset++;
					AnimClass * anim = NULL;

					/*
					**	Show pieces of fire to indicate that a significant change in
					**	damage level has occurred.
					*/
					if (warhead == WARHEAD_FIRE) {
						switch (Random_Pick(0, 5+Class->Width()+Class->Height())) {
							case 0:
								break;

							case 1:
							case 2:
							case 3:
							case 4:
							case 5:
								anim = new AnimClass(ANIM_ON_FIRE_SMALL, Coord_Scatter(Cell_Coord(cell), 0x0060), 0, Random_Pick(1, 3));
								break;

							case 6:
							case 7:
							case 8:
								anim = new AnimClass(ANIM_ON_FIRE_MED, Coord_Scatter(Cell_Coord(cell), 0x0060), 0, Random_Pick(1, 3));
								break;

							case 9:
								anim = new AnimClass(ANIM_ON_FIRE_BIG, Coord_Scatter(Cell_Coord(cell), 0x0060), 0, 1);
								break;

							default:
								break;
						}
					} else {
						if (Percent_Chance(50)) {
							/*
							** Building may catch on fire, but only if it wasn't a
							** renovator that caused the damage.
							*/
							if (source == NULL || source->What_Am_I() != RTTI_INFANTRY || *(InfantryClass *)source != INFANTRY_RENOVATOR) {
								anim = new AnimClass(ANIM_FIRE_SMALL, Coord_Scatter(Cell_Coord(cell), 0x0060), Random_Pick(0, 7), Random_Pick(1, 3));
							}
						}
					}
					/*
					**	If the animation was created, then attach it to the building.
					*/
					if (anim) {
						anim->Attach_To(this);
					}
				}
				break;

			case RESULT_NONE:
				break;

			case RESULT_LIGHT:
				break;
		}

		if (source && res != RESULT_NONE) {

			/*
			**	If any damage occurred, then inform the house of this fact. If it is the player's
			**	house, it might announce this fact.
			*/
			if (!Class->IsInsignificant) {
				House->Attacked(this);
			}

			/*
			** Save the type of the house that's doing the damage, so if the building burns
			** to death credit can still be given for the kill
			*/
			WhoLastHurtMe = source->Owner();

			/*
			**	When certain buildings are hit, they "snap out of it" and
			**	return fire if they are able and allowed.
			*/
			if (*this != STRUCT_SAM && *this != STRUCT_AAGUN &&
				!House->Is_Ally(source) &&
				Class->PrimaryWeapon != NULL &&
				(!Target_Legal(TarCom) || !In_Range(TarCom))) {

				if (source->What_Am_I() != RTTI_AIRCRAFT && (!House->IsHuman || Rule.IsSmartDefense)) {
					Assign_Target(source->As_Target());
				} else {

					/*
					**	Generate a random rotation effect since there is nothing else that this
					**	building can do.
					*/
					if (!PrimaryFacing.Is_Rotating()) {
						PrimaryFacing.Set_Desired(Random_Pick(DIR_N, DIR_MAX));
					}
				}
			}
		}
	}

	return(res);
}


/***********************************************************************************************
 * BuildingClass::new -- Allocates a building object from building pool.                       *
 *                                                                                             *
 *    This routine will allocate a building slot from the building alloc                       *
 *    system.                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the allocated building. If NULL is                       *
 *          returned, then this indicates a failure to allocate.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/11/1994 JLB : Created.                                                                 *
 *   04/21/1994 JLB : Converted to operator new.                                               *
 *   05/17/1994 JLB : Revamped allocation scheme                                               *
 *   07/29/1994 JLB : Simplified.                                                              *
 *=============================================================================================*/
void * BuildingClass::operator new(size_t )
{
	void * ptr = Buildings.Allocate();
	if (ptr) {
		((BuildingClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * BuildingClass::delete -- Deallocates building object.                                       *
 *                                                                                             *
 *    This is the memory deallocation operation for a building object.                         *
 *    Since buildings are allocated out of a fixed memory block, all that                      *
 *    is needed is to flag the unit as inactive.                                               *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to building to deallocate.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::operator delete(void *ptr)
{
	if (ptr) {
		((BuildingClass *)ptr)->IsActive = false;
	}
	Buildings.Free((BuildingClass *)ptr);
}


/***********************************************************************************************
 * BuildingClass::BuildingClass -- Constructor for buildings.                                  *
 *                                                                                             *
 *    This routine inserts a building into the object tracking system.                         *
 *    It is placed into a limbo state unless a location is provided for                        *
 *    it to unlimbo at.                                                                        *
 *                                                                                             *
 * INPUT:   type  -- The structure type to make this object.                                   *
 *                                                                                             *
 *          house -- The owner of this building.                                               *
 *                                                                                             *
 *          pos   -- The position to unlimbo the building. If -1 is                            *
 *                   specified, then the building remains in a limbo                           *
 *                   state.                                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/21/1994 JLB : Created.                                                                 *
 *   08/07/1995 JLB : Fixed act like value to match expected value.                            *
 *=============================================================================================*/
BuildingClass::BuildingClass(StructType type, HousesType house) :
	TechnoClass(RTTI_BUILDING, Buildings.ID(this), house),
	Class(BuildingTypes.Ptr((int)type)),
	Factory(0),
	ActLike(House->ActLike),
	IsToRebuild(false),
	IsToRepair(false),
	IsAllowedToSell(true),
	IsReadyToCommence(false),
	IsRepairing(false),
	IsWrenchVisible(false),
	IsGoingToBlow(false),
	IsSurvivorless(false),
	IsCharging(false),
	IsCharged(false),
	IsCaptured(false),
	IsJamming(false),
	IsJammed(false),
	HasFired(false),
	HasOpened(false),
	CountDown(0),
	BState(BSTATE_NONE),
	QueueBState(BSTATE_NONE),
	WhoLastHurtMe(house),
	WhomToRepay(TARGET_NONE),
	AnimToTrack(TARGET_NONE),
	LastStrength(0),
	PlacementDelay(0)
{
	House->Tracking_Add(this);
	IsSecondShot = !Class->Is_Two_Shooter();
	Strength = Class->MaxStrength;
	Ammo = Class->MaxAmmo;

	/*
	**	If the building could never be built, then it can never be sold either. This
	**	is due to the lack of buildup animation.
	*/
	if (Class->Get_Buildup_Data() != NULL) {
//	if (!Class->IsBuildable) {
		IsAllowedToSell = false;
	}

//	if (Session.Type == GAME_INTERNET) {
//		House->BuildingTotals->Increment_Unit_Total( (int) type);
//	}

}


/***********************************************************************************************
 * BuildingClass::~BuildingClass -- Destructor for building type objects.                      *
 *                                                                                             *
 *    This destructor for building objects will put the building in limbo if possible.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass::~BuildingClass(void)
{
	if (GameActive && Class) {
		if (House) {
			House->Tracking_Remove(this);
		}
		BuildingClass::Limbo();
	}
	Class = 0;

	delete (FactoryClass *)Factory;
	Factory = 0;
	ID = -1;
}


/***********************************************************************************************
 * BuildingClass::Drop_Debris -- Drops rubble when building is destroyed.                      *
 *                                                                                             *
 *    This routine is called when a building is destroyed. It handles                          *
 *    placing the rubble down.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *   06/13/1995 JLB : Added smoke and normal infantry survivor possibility.                    *
 *   07/16/1995 JLB : Survival rate depends on if captured or sabotaged.                       *
 *=============================================================================================*/
void BuildingClass::Drop_Debris(TARGET source)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	CELL const * offset;
	CELL cell;

	/*
	**	Generate random survivors from the destroyed building.
	*/
	cell = Coord_Cell(Coord);
	offset = Occupy_List();
	int odds = 2;
	if (Target_Legal(WhomToRepay)) odds -= 1;
	if (IsCaptured) odds += 6;
	int count = How_Many_Survivors();
	while (*offset != REFRESH_EOL) {
		CELL	newcell;

		newcell = cell + *offset++;
		CellClass const * cellptr = &Map[newcell];

		/*
		**	Infantry could run out of a destroyed building.
		*/
		if (!House->IsToDie && count > 0) {
			InfantryClass * i = NULL;

			if (Random_Pick(0, odds) == 1) {
				i = NULL;
				InfantryType typ = Crew_Type();
				if (typ != INFANTRY_NONE) i = new InfantryClass(typ, House->Class->House);
				if (i != NULL) {
					if (Class->Get_Buildup_Data() != NULL && i->Class->IsNominal) i->IsTechnician = true;
					ScenarioInit++;
					if (i->Unlimbo(Cell_Coord(newcell), DIR_N)) {
						count--;
						i->Strength = Random_Pick(5, (int)i->Class->MaxStrength);
						i->Scatter(0, true);
						if (source != TARGET_NONE && !House->Is_Ally(As_Object(source))) {
							i->Assign_Mission(MISSION_ATTACK);
							i->Assign_Target(source);
						} else {
							if (House->IsHuman) {
								i->Assign_Mission(MISSION_GUARD);
							} else {
								i->Assign_Mission(MISSION_HUNT);
							}
						}
					} else {
						delete i;
					}
					ScenarioInit--;
				}
			}
		}

		/*
		**	Smoke and fire only appear on terrestrail cells. They should not appear on
		**	rivers, clifs, or water cells.
		*/
		if (cellptr->Is_Clear_To_Move(SPEED_TRACK, true, true)) {

			/*
			**	Possibly add some smoke rising from the ashes of the building.
			*/
			switch (Random_Pick(0, 5)) {
				case 0:
				case 1:
				case 2:
					new AnimClass(ANIM_SMOKE_M, Coord_Scatter(Cell_Coord(newcell), 0x0050, false), Random_Pick(0, 5), Random_Pick(1, 2));
					break;

				default:
					break;
			}

			/*
			**	The building always scars the ground in some fashion.
			*/
			if (Percent_Chance(25)) {
				new SmudgeClass(Random_Pick(SMUDGE_SCORCH1, SMUDGE_SCORCH6), Cell_Coord(newcell));
			} else {
				new SmudgeClass(Random_Pick(SMUDGE_CRATER1, SMUDGE_CRATER6), Coord_Scatter(Cell_Coord(newcell), 0x0080, false));
			}
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Active_Click_With -- Handles clicking on the map while the building is selected.*
 *                                                                                             *
 *    This interface routine handles when the player clicks on the map while this building     *
 *    is currently selected. This is used to assign an override target to a turret or          *
 *    guard tower.                                                                             *
 *                                                                                             *
 * INPUT:   target   -- The target that was clicked upon.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Active_Click_With(ActionType action, ObjectClass * object)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (action == ACTION_ATTACK && object != NULL) {
		Player_Assign_Mission(MISSION_ATTACK, object->As_Target());
	}

	if (action == ACTION_TOGGLE_PRIMARY && Class->Is_Factory()) {
		OutList.Add(EventClass(EventClass::PRIMARY, TargetClass(this)));
	}
}


/***********************************************************************************************
 * BuildingClass::Active_Click_With -- Handles cell selection for buildings.                   *
 *                                                                                             *
 *    This routine really only serves one purpose -- to allow targeting of the ground for      *
 *    buildings that are equipped with weapons.                                                *
 *                                                                                             *
 * INPUT:   action   -- The requested action to perform.                                       *
 *                                                                                             *
 *          cell     -- The cell location to perform the action upon.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *   10/04/1995 JLB : Handles construction yard undeploy to move logic.                        *
 *=============================================================================================*/
void BuildingClass::Active_Click_With(ActionType action, CELL cell)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (action == ACTION_ATTACK) {
		Player_Assign_Mission(MISSION_ATTACK, ::As_Target(cell));
	}

	if (action == ACTION_MOVE && *this == STRUCT_CONST) {
		OutList.Add(EventClass(EventClass::ARCHIVE, TargetClass(this), TargetClass(::As_Target(cell))));
		OutList.Add(EventClass(EventClass::SELL, TargetClass(this)));

		COORDINATE coord = Map.Pixel_To_Coord(Get_Mouse_X(), Get_Mouse_Y());
		OutList.Add(EventClass(ANIM_MOVE_FLASH, PlayerPtr->Class->House, coord));
	}
}


/***********************************************************************************************
 * BuildingClass::Assign_Target -- Assigns a target to the building.                           *
 *                                                                                             *
 *    Assigning of a target to a building makes sense if the building is one that can attack.  *
 *    This routine would be used to assign the attack target to a turret or guard tower.       *
 *                                                                                             *
 * INPUT:   target   -- The target that was clicked on while this building was selected.       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *   11/02/1994 JLB : Checks for range before assigning target.                                *
 *=============================================================================================*/
void BuildingClass::Assign_Target(TARGET target)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (*this != STRUCT_SAM && *this != STRUCT_AAGUN && !In_Range(target, 0)) {
		target = TARGET_NONE;
	}

	TechnoClass::Assign_Target(target);
}


/***********************************************************************************************
 * BuildingClass::Init -- Initialize the building system to an empty null state.               *
 *                                                                                             *
 *    This routine initializes the building system in preparation for a scenario load.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Init(void)
{
	Buildings.Free_All();
}


/***********************************************************************************************
 * BuildingClass::Exit_Object -- Initiates an object to leave the building.                    *
 *                                                                                             *
 *    This function is used to cause an object to exit the building. It is called when a       *
 *    factory produces a vehicle or other mobile object and that object needs to exit the      *
 *    building to join the ranks of a regular unit. Typically, the object is placed down on    *
 *    the map such that it overlaps the building and then it is given a movement order so that *
 *    it will move to an adjacent free cell.                                                   *
 *                                                                                             *
 * INPUT:   base  -- Pointer to the object that is to exit the building.                       *
 *                                                                                             *
 * OUTPUT:  Returns the success rating for the exit attempt;                                   *
 *             0  = complete failure (refund money please)                                     *
 *             1  = temporarily prevented (try again later please)                             *
 *             2  = successful                                                                 *
 *                                                                                             *
 * WARNINGS:   The building is placed in radio contact with the object. The object is in a     *
 *             tethered condition. This condition will be automatically broken when the        *
 *             object reaches the adjacent square.                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 JLB : Created.                                                                 *
 *   04/10/1995 JLB : Handles building production by computer.                                 *
 *   06/17/1995 JLB : Handles refinery exit.                                                   *
 *=============================================================================================*/
int BuildingClass::Exit_Object(TechnoClass * base)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (!base) return(0);

	TechnoTypeClass const * ttype = (TechnoTypeClass const *)&base->Class_Of();

	/*
	**	A unit exiting a building is always considered to be "locked". That means, it
	**	will be considered as to have legally entered the visible map domain.
	*/
	base->IsLocked = true;

	/*
	**	Find a good cell to unload the object to. The object, probably a vehicle
	**	will drive/walk to the adjacent free cell.
	*/
	CELL cell = 0;

	switch (base->What_Am_I()) {

		case RTTI_AIRCRAFT:
			if (!In_Radio_Contact()) {
				AircraftClass * air = (AircraftClass *)base;

				air->Height = 0;
				ScenarioInit++;
				if (air->Unlimbo(Docking_Coord(), air->Pose_Dir())) {
					Transmit_Message(RADIO_HELLO, air);
					Transmit_Message(RADIO_TETHER);
					ScenarioInit--;
					return(2);
				}
				ScenarioInit--;
			} else {
				AircraftClass * air = (AircraftClass *)base;

				if (Cell_X(Coord_Cell(Center_Coord())) - Map.MapCellX < Map.MapCellWidth/2) {
					cell = XY_Cell(Map.MapCellX-1, Random_Pick(0, Map.MapCellHeight-1)+Map.MapCellY);
				} else {
					cell = XY_Cell(Map.MapCellX+Map.MapCellWidth, Random_Pick(0, Map.MapCellHeight-1)+Map.MapCellY);
				}
				ScenarioInit++;
				if (air->Unlimbo(Cell_Coord(cell), DIR_N)) {
//BG				air->Assign_Destination(::As_Target(Nearby_Location(air)));
/*BG*/			air->Assign_Destination(::As_Target(air->Nearby_Location(this)));
					air->Assign_Mission(MISSION_MOVE);
					ScenarioInit--;
					return(2);
				}
				ScenarioInit--;
			}
			break;

		case RTTI_VESSEL:
			switch (Class->Type) {
				case STRUCT_SUB_PEN:
				case STRUCT_SHIP_YARD:
					ScenarioInit++;
					cell = Find_Exit_Cell(base);
					if (cell != 0 && base->Unlimbo(Cell_Coord(cell), Direction(Cell_Coord(cell)))) {
						base->Assign_Mission(MISSION_GUARD);
						ScenarioInit--;
						return(2);
					}
					ScenarioInit--;
					break;

				default:
					break;
			}
			break;

		case RTTI_INFANTRY:
		case RTTI_UNIT:
			switch (Class->Type) {
				case STRUCT_REFINERY:
					if (base->What_Am_I() == RTTI_UNIT) {
						cell = Coord_Cell(Center_Coord());
						UnitClass * unit = (UnitClass *)base;

						cell = Adjacent_Cell(cell, FACING_SW);
						ScenarioInit++;
						if (unit->Unlimbo(Cell_Coord(Adjacent_Cell(cell, DIR_S)), DIR_SW_X2)) {
							unit->PrimaryFacing = DIR_S;
							unit->Assign_Mission(MISSION_HARVEST);
						}
						ScenarioInit--;
					} else {
						base->Scatter(0, true);
					}
					break;

				case STRUCT_WEAP:
					if (Mission == MISSION_UNLOAD) {
						for(int index = 0; index < Buildings.Count(); index++) {
							BuildingClass *bldg = Buildings.Ptr(index);
							if (bldg->Owner() == Owner() && *bldg == STRUCT_WEAP && bldg != this && bldg->Mission == MISSION_GUARD && !bldg->Factory) {
								FactoryClass * temp = Factory;
								bldg->Factory = Factory;
								Factory = 0;
								int retval = (bldg->Exit_Object(base));
								bldg->Factory = 0;
								Factory = temp;
								return(retval);
							}
						}
						return(1);	// fail while we're still unloading previous
					}
					ScenarioInit++;
					if (base->Unlimbo(Exit_Coord(), DIR_S)) {
						base->Mark(MARK_UP);
						base->Coord = Exit_Coord();
						base->Mark(MARK_DOWN);
						Transmit_Message(RADIO_HELLO, base);
						Transmit_Message(RADIO_TETHER);
						Assign_Mission(MISSION_UNLOAD);
						ScenarioInit--;
						return(2);
					}
					ScenarioInit--;
					break;

				case STRUCT_BARRACKS:
				case STRUCT_TENT:
				case STRUCT_KENNEL:

					cell = Find_Exit_Cell(base);
					if (cell != 0) {
						DirType	dir = Direction(cell);
						COORDINATE		start = Exit_Coord();

						ScenarioInit++;
						if (base->Unlimbo(start, dir)) {

							base->Assign_Mission(MISSION_MOVE);

							/*
							**	When disembarking from a transport then guard an area around the
							**	center of the base.
							*/
							base->Assign_Destination(::As_Target(cell));
							if (House->IQ >= Rule.IQGuardArea) {
								base->Assign_Mission(MISSION_GUARD_AREA);
								base->ArchiveTarget = ::As_Target(House->Where_To_Go((FootClass *)base));
							}

							/*
							**	Establish radio contact so unload coordination can occur. This
							**	radio contact should always succeed.
							*/
							if (Transmit_Message(RADIO_HELLO, base) == RADIO_ROGER) {
								Transmit_Message(RADIO_UNLOAD);
							}
							ScenarioInit--;
							return(2);
						}
						ScenarioInit--;
					}
					break;

				default:
					cell = Find_Exit_Cell(base);
					if (cell != 0) {
						DirType	dir = Direction(cell);
						COORDINATE		start = Exit_Coord();

						ScenarioInit++;
						if (base->Unlimbo(start, dir)) {

							base->Assign_Mission(MISSION_MOVE);

							/*
							**	When disembarking from a transport then guard an area around the
							**	center of the base.
							*/
							base->Assign_Destination(::As_Target(cell));
							if (House->IQ >= Rule.IQGuardArea) {
								base->Assign_Mission(MISSION_GUARD_AREA);
								base->ArchiveTarget = ::As_Target(House->Where_To_Go((FootClass *)base));
							}
							ScenarioInit--;
							return(2);
						}
						ScenarioInit--;
					}
					break;
			}
			break;

		case RTTI_BUILDING:

			if (!House->IsHuman) {

				/*
				**	Find the next available spot to place this newly created building. If the
				**	building could be placed at the desired location, fine. If not, then this
				**	routine will return failure. The calling routine will probably abandon this
				**	building in preference to building another.
				*/
				BaseNodeClass * node = Base.Next_Buildable(((BuildingClass *)base)->Class->Type);
				COORDINATE coord = 0;
				if (node) {
					coord = Cell_Coord(node->Cell);
				} else {

					/*
					**	Find a suitable new spot to place.
					*/
					coord = House->Find_Build_Location((BuildingClass *)base);
				}

				if (coord) {
					if (Flush_For_Placement(base, Coord_Cell(coord))) {
						return(1);
					}
					if (base->Unlimbo(coord)) {
						if (node && ((BuildingClass *)base)->Class->Type == House->BuildStructure) {
							House->BuildStructure = STRUCT_NONE;
						}
						return(2);
					}
				}
			}
			break;

		default:
			break;
	}

	/*
	**	Failure to exit the object results in a false return value.
	*/
	return(0);
}


/***********************************************************************************************
 * BuildingClass::Update_Buildables -- Informs sidebar of additional construction options.     *
 *                                                                                             *
 *    This routine will tell the sidebar of objects that can be built. The function is called  *
 *    whenever a building matures.                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/11/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Only updates for PLAYER buildings.                                       *
 *=============================================================================================*/
void BuildingClass::Update_Buildables(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	/*
	** Only do this for real human players. ST - 3/22/2019 1:38PM
	*/
	if (PlayerPtr != House) {
		if (Session.Type != GAME_GLYPHX_MULTIPLAYER || House->IsHuman == false) {
			return;
		}
	}

	bool buildable_via_capture = (IsCaptured && ActLike != House->ActLike) ? true : false;

	if (!IsInLimbo && Is_Discovered_By_Player()) {
		switch (Class->ToBuild) {
			int i;
			int u;
			int f;
			int a;
			int v;

			case RTTI_VESSELTYPE:
				for (v = VESSEL_FIRST; v < VESSEL_COUNT; v++) {
					if (PlayerPtr->Can_Build(&VesselTypeClass::As_Reference((VesselType)v), ActLike)) {
						if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
							Sidebar_Glyphx_Add(RTTI_VESSELTYPE, v, House, buildable_via_capture);
						} else {
							Map.Add(RTTI_VESSELTYPE, v, buildable_via_capture);
						}
					}
				}
				break;

			case RTTI_BUILDINGTYPE:
				for (i = STRUCT_FIRST; i < STRUCT_COUNT; i++) {
					if (PlayerPtr->Can_Build(&BuildingTypeClass::As_Reference((StructType)i), ActLike)) {
						if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
							Sidebar_Glyphx_Add(RTTI_BUILDINGTYPE, i, House, buildable_via_capture);
						} else {
							Map.Add(RTTI_BUILDINGTYPE, i, buildable_via_capture);
						}
					}
				}
				break;

			case RTTI_UNITTYPE:
				for (u = UNIT_FIRST; u < UNIT_COUNT; u++) {
					if (PlayerPtr->Can_Build(&UnitTypeClass::As_Reference((UnitType)u), ActLike)) {
						if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
							Sidebar_Glyphx_Add(RTTI_UNITTYPE, u, House, buildable_via_capture);
						} else {
							Map.Add(RTTI_UNITTYPE, u, buildable_via_capture);
						}
					}
				}
				break;

			case RTTI_INFANTRYTYPE:
				for (f = INFANTRY_FIRST; f < INFANTRY_COUNT; f++) {
					if (PlayerPtr->Can_Build(&InfantryTypeClass::As_Reference((InfantryType)f), ActLike)) {
						if (InfantryTypeClass::As_Reference((InfantryType)f).IsDog) {
							if (*this == STRUCT_KENNEL) {
								if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
									Sidebar_Glyphx_Add(RTTI_INFANTRYTYPE, f, House, buildable_via_capture);
								} else {
									Map.Add(RTTI_INFANTRYTYPE, f, buildable_via_capture);
								}
							}
						} else {
							if (*this != STRUCT_KENNEL) {
								if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
									Sidebar_Glyphx_Add(RTTI_INFANTRYTYPE, f, House, buildable_via_capture);
								} else {
									Map.Add(RTTI_INFANTRYTYPE, f, buildable_via_capture);
								}
							}
						}
					}
				}
				break;

			case RTTI_AIRCRAFTTYPE:
				for (a = AIRCRAFT_FIRST; a < AIRCRAFT_COUNT; a++) {
					if (PlayerPtr->Can_Build(&AircraftTypeClass::As_Reference((AircraftType)a), ActLike)) {
						if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
							Sidebar_Glyphx_Add(RTTI_AIRCRAFTTYPE, a, House, buildable_via_capture);
						} else {
							Map.Add(RTTI_AIRCRAFTTYPE, a, buildable_via_capture);
						}
					}
				}
				break;

			default:
				break;
		}
	}
}



#if (0) //Old code for reference. ST - 8/2/2019 2:41PM
/***********************************************************************************************
 * BuildingClass::Update_Buildables -- Informs sidebar of additional construction options.     *
 *                                                                                             *
 *    This routine will tell the sidebar of objects that can be built. The function is called  *
 *    whenever a building matures.                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/11/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Only updates for PLAYER buildings.                                       *
 *=============================================================================================*/
void BuildingClass::Update_Buildables(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (House == PlayerPtr && !IsInLimbo && IsDiscoveredByPlayer) {
		switch (Class->ToBuild) {
			int i;
			int u;
			int f;
			int a;
			int v;

			case RTTI_VESSELTYPE:
				for (v = VESSEL_FIRST; v < VESSEL_COUNT; v++) {
					if (PlayerPtr->Can_Build(&VesselTypeClass::As_Reference((VesselType)v), ActLike)) {
						Map.Add(RTTI_VESSELTYPE, v);
					}
				}
				break;

			case RTTI_BUILDINGTYPE:
				for (i = STRUCT_FIRST; i < STRUCT_COUNT; i++) {
					if (PlayerPtr->Can_Build(&BuildingTypeClass::As_Reference((StructType)i), ActLike)) {
						Map.Add(RTTI_BUILDINGTYPE, i);
					}
				}
				break;

			case RTTI_UNITTYPE:
				for (u = UNIT_FIRST; u < UNIT_COUNT; u++) {
					if (PlayerPtr->Can_Build(&UnitTypeClass::As_Reference((UnitType)u), ActLike)) {
						Map.Add(RTTI_UNITTYPE, u);
					}
				}
				break;

			case RTTI_INFANTRYTYPE:
				for (f = INFANTRY_FIRST; f < INFANTRY_COUNT; f++) {
					if (PlayerPtr->Can_Build(&InfantryTypeClass::As_Reference((InfantryType)f), ActLike)) {
						if (InfantryTypeClass::As_Reference((InfantryType)f).IsDog) {
							if (*this == STRUCT_KENNEL) {
								Map.Add(RTTI_INFANTRYTYPE, f);
							}
						} else {
							if (*this != STRUCT_KENNEL) {
								Map.Add(RTTI_INFANTRYTYPE, f);
							}
						}
					}
				}
				break;

			case RTTI_AIRCRAFTTYPE:
				for (a = AIRCRAFT_FIRST; a < AIRCRAFT_COUNT; a++) {
					if (PlayerPtr->Can_Build(&AircraftTypeClass::As_Reference((AircraftType)a), ActLike)) {
						Map.Add(RTTI_AIRCRAFTTYPE, a);
					}
				}
				break;

			default:
				break;
		}
	}
}
#endif


/***********************************************************************************************
 * BuildingClass::Fire_Out -- Handles when attached animation expires.                         *
 *                                                                                             *
 *    This routine is used to perform any fixups necessary when the attached animation has     *
 *    terminated. This occurs when the fire & smoke animation that a SAM site produces stops.  *
 *    At that point, normal reload procedures can commence.                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/30/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Fire_Out(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);
}


/***********************************************************************************************
 * BuildingClass::Limbo -- Handles power adjustment as building goes into limbo.               *
 *                                                                                             *
 *    This routine will handle the power adjustments for the associated house when the         *
 *    building goes into limbo. This means that its power drain or production is subtracted    *
 *    from the house accumulated totals.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the building limboed?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Limbo(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (!IsInLimbo) {

		/*
		**	Update the total factory type, assuming this building has a factory.
		*/
		House->Active_Remove(this);
		House->IsRecalcNeeded = true;
		House->Recalc_Center();

		/*
		**	Update the power status of the owner's house.
		*/
		House->Adjust_Power(-Power_Output());
		House->Adjust_Drain(-Class->Drain);
		House->Adjust_Capacity(-Class->Capacity, true);
		if (House == PlayerPtr) {
			Map.PowerClass::IsToRedraw = true;
			Map.Flag_To_Redraw(false);
		}

		/*
		**	This could be a building that builds. If so, then the sidebar may need adjustment.
		** Set IsInLimbo to true to "fool" the sidebar into knowing that this building
		** isn't available.  Set it back to false so the rest of the Limbo code works.
		** Otherwise, the sidebar won't properly remove non-available buildables.
		*/
//		if (IsOwnedByPlayer && !ScenarioInit) {
//			IsInLimbo = true;
//			Map.Recalc();
//			IsInLimbo = false;
//		}
	}
	return(TechnoClass::Limbo());
}


/***********************************************************************************************
 * BuildingClass::Turret_Facing -- Fetches the turret facing for this building.                *
 *                                                                                             *
 *    This will return the turret facing for this building. Some buildings don't have a        *
 *    visual turret (e.g., pillbox) so they return a turret facing that always faces their     *
 *    current target.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current facing of the turret.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType BuildingClass::Turret_Facing(void) const
{
	if (!Class->IsTurretEquipped && Target_Legal(TarCom)) {
		return(::Direction(Center_Coord(), As_Coord(TarCom)));
	}
	return(PrimaryFacing.Current());
}


/***********************************************************************************************
 * BuildingClass::Greatest_Threat -- Searches for target that building can fire upon.          *
 *                                                                                             *
 *    This routine intercepts the Greatest_Threat function so that it can add the ability      *
 *    to search for ground targets, if this isn't a SAM site.                                  *
 *                                                                                             *
 * INPUT:   threat   -- The base threat control value. Typically, it might be THREAT_RANGE     *
 *                      or THREAT_NORMAL.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns with a suitable target. If none could be found, then TARGET_NONE is        *
 *          returned instead.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET BuildingClass::Greatest_Threat(ThreatType threat) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (Class->PrimaryWeapon != NULL) {
		threat = threat | Class->PrimaryWeapon->Allowed_Threats();
	}
	if (Class->SecondaryWeapon != NULL) {
		threat = threat | Class->SecondaryWeapon->Allowed_Threats();
	}
	if (House->IsHuman) {
		threat = threat & ~THREAT_BUILDINGS;
	}
	threat = threat | THREAT_RANGE;

//	if (Class->PrimaryWeapon != NULL) {
//		if (Class->PrimaryWeapon->Bullet->IsAntiAircraft) {
//			threat = threat | THREAT_AIR;
//		}
//		if (Class->PrimaryWeapon->Bullet->IsAntiGround) {
//			threat = threat | THREAT_BUILDINGS|THREAT_INFANTRY|THREAT_BOATS|THREAT_VEHICLES;
//		}
//		threat = threat | THREAT_RANGE;
//	}
	return(TechnoClass::Greatest_Threat(threat));
}


/***********************************************************************************************
 * BuildingClass::Grand_Opening -- Handles construction completed special operations.          *
 *                                                                                             *
 *    This routine is called when construction has finished. Typically, this enables           *
 *    new production options for factories.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/08/1995 JLB : Created.                                                                 *
 *   06/13/1995 JLB : Added helipad.                                                           *
 *=============================================================================================*/
void BuildingClass::Grand_Opening(bool captured)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (!HasOpened || captured) {
		HasOpened = true;

		/*
		**	Adjust the owning house according to the power, drain, and Tiberium capacity that
		**	this building has.
		*/
		House->Adjust_Drain(Class->Drain);
		House->Adjust_Capacity(Class->Capacity);
		House->IsRecalcNeeded = true;

		/*	SPECIAL CASE:
		**	Tiberium Refineries get a free harvester. Add a harvester to the
		**	reinforcement list at this time.
		*/
		if (*this == STRUCT_REFINERY && !ScenarioInit && !captured && !Debug_Map && (!House->IsHuman || PurchasePrice == 0 || PurchasePrice > Class->Raw_Cost())) {
			CELL cell = Coord_Cell(Adjacent_Cell(Center_Coord(), DIR_S));

			UnitClass * unit = new UnitClass(UNIT_HARVESTER, House->Class->House);
			if (unit != NULL) {

				/*
				**	Try to place down the harvesters. If it could not be placed, then try
				**	to place it in a nearby location.
				*/
				if (!unit->Unlimbo(Cell_Coord(cell), DIR_W)) {
					cell = unit->Nearby_Location(this);

					/*
					**	If the harvester could still not be placed, then refund the money
					**	to the owner and then bail.
					*/
					if (!unit->Unlimbo(Cell_Coord(cell), DIR_SW)) {
						House->Refund_Money(unit->Class->Cost_Of());
						delete unit;
					}
				}
			} else {

				/*
				**	If the harvester could not be created in the first place, then give
				**	the full refund price to the owning player.
				*/
				House->Refund_Money(UnitTypeClass::As_Reference(UNIT_HARVESTER).Cost_Of());
			}
		}

		/*
		**	Helicopter pads get a free attack helicopter.
		*/
		if (!Rule.IsSeparate && *this == STRUCT_HELIPAD && !captured) {
			ScenarioInit++;
			AircraftClass * air = 0;
			if (House->ActLike == HOUSE_USSR || House->ActLike == HOUSE_BAD || House->ActLike == HOUSE_UKRAINE) {
				air = new AircraftClass(AIRCRAFT_HIND, House->Class->House);
			} else {
				air = new AircraftClass(AIRCRAFT_LONGBOW, House->Class->House);
			}
			if (air) {
				air->Height = 0;
				if (air->Unlimbo(Docking_Coord(), air->Pose_Dir())) {
					air->Assign_Mission(MISSION_GUARD);
					air->Transmit_Message(RADIO_HELLO, this);
					Transmit_Message(RADIO_TETHER);
				}
			}
			ScenarioInit--;
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Repair -- Initiates or terminates the repair process.                        *
 *                                                                                             *
 *    This routine will start, stop, or toggle the repair process. When a building repairs, it *
 *    occurs incrementally over time.                                                          *
 *                                                                                             *
 * INPUT:   control  -- Determines how to control the repair process.                          *
 *                      0: Turns repair process off (if it was on).                            *
 *                      1: Turns repair process on (if it was off).                            *
 *                      -1:Toggles repair process to other state.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Repair(int control)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	switch (control) {
		case -1:
			IsRepairing = (IsRepairing == false);
			break;

		case 1:
			if (IsRepairing) return;
			IsRepairing = true;
			break;

		case 0:
			if (!IsRepairing) return;
			IsRepairing = false;
			break;

		default:
			break;
	}

	/*
	**	At this point, we know that the repair state has changed. Perform
	**	appropriate action.
	*/
	VocType soundid = VOC_NONE;
	if (IsRepairing) {
		if (Strength == Class->MaxStrength) {
			soundid = VOC_SCOLD;
		} else {
			soundid = VOC_CLICK;
			if (House->IsPlayerControl) {
				Clicked_As_Target(PlayerPtr->Class->House); // 2019/09/20 JAS - Added record of who clicked on the object
			}
			IsWrenchVisible = true;
		}
	} else {
		soundid = VOC_CLICK;
	}
	
	if (House->IsPlayerControl) {
		Sound_Effect(soundid, Coord);
	}
}


/***********************************************************************************************
 * BuildingClass::Sell_Back -- Controls the sell back (demolish) operation.                    *
 *                                                                                             *
 *    This routine will initiate or stop the sell back process for a building. It is called    *
 *    when the player clicks on a building when the sell mode is active.                       *
 *                                                                                             *
 * INPUT:   control  -- The action to perform. 0 = turn deconstruction off, 1 = deconstruct,   *
 *                      -1 = toggle deconstruction state.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Sell_Back(int control)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (Class->Get_Buildup_Data()) {
		bool decon = false;
		switch (control) {
			case -1:
				decon = (Mission != MISSION_DECONSTRUCTION);
				break;

			case 1:
				if (Mission == MISSION_DECONSTRUCTION) return;
				if (IsGoingToBlow) return;
				decon = true;
				break;

			case 0:
				if (Mission != MISSION_DECONSTRUCTION) return;
				decon = false;
				break;

			default:
				break;
		}

		/*
		**	At this point, we know that the repair state has changed. Perform
		**	appropriate action.
		*/
		if (decon) {
			Assign_Mission(MISSION_DECONSTRUCTION);
			Commence();
			if (House->IsPlayerControl) {
				Clicked_As_Target(PlayerPtr->Class->House);
			}
		}
		if (House->IsPlayerControl) {
			Sound_Effect(VOC_CLICK);
		}
	}
}


/***********************************************************************************************
 * BuildingClass::What_Action -- Determines action to perform if click on specified object.    *
 *                                                                                             *
 *    This routine will determine what action to perform if the mouse was clicked on the       *
 *    object specified. This determination is used to control the mouse imagery and the        *
 *    function process when the mouse button is pressed.                                       *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that, if clicked on, will control what action    *
 *                      is to be performed.                                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the ActionType that will occur if the mouse is clicked over the       *
 *          object specified while the building is currently selected.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType BuildingClass::What_Action(ObjectClass const * object) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	ActionType action = TechnoClass::What_Action(object);

	if (action == ACTION_SELF) {
		int index;
		if (Class->Is_Factory() && PlayerPtr == House && *House->Factory_Counter(Class->ToBuild) > 1) {
			switch (Class->ToBuild) {
				case RTTI_INFANTRYTYPE:
				case RTTI_INFANTRY:
					action = ACTION_NONE;
					if (*this == STRUCT_KENNEL) {
						for (index = 0; index < Buildings.Count(); index++) {
							BuildingClass *bldg = Buildings.Ptr(index);
							if (bldg != this && bldg->Owner() == Owner() && *bldg == STRUCT_KENNEL) {
								action = ACTION_TOGGLE_PRIMARY;
								break;
							}
						}
					} else {
						for (index = 0; index < Buildings.Count(); index++) {
							BuildingClass *bldg = Buildings.Ptr(index);
							if (bldg != this && bldg->Owner() == Owner() && bldg->Class->ToBuild == RTTI_INFANTRYTYPE && *bldg != STRUCT_KENNEL) {
								action = ACTION_TOGGLE_PRIMARY;
								break;
							}
						}
					}
					break;

				case RTTI_AIRCRAFTTYPE:
				case RTTI_AIRCRAFT:
					action = ACTION_NONE;
					if (*this == STRUCT_AIRSTRIP) {
						for (index = 0; index < Buildings.Count(); index++) {
							BuildingClass *bldg = Buildings.Ptr(index);
							if (bldg != this && bldg->Owner() == Owner() && *bldg == STRUCT_AIRSTRIP) {
								action = ACTION_TOGGLE_PRIMARY;
								break;
							}
						}
					}
					else {
						for (index = 0; index < Buildings.Count(); index++) {
							BuildingClass *bldg = Buildings.Ptr(index);
							if (bldg != this && bldg->Owner() == Owner() && bldg->Class->ToBuild == RTTI_AIRCRAFTTYPE && *bldg != STRUCT_AIRSTRIP) {
								action = ACTION_TOGGLE_PRIMARY;
								break;
							}
						}
					}
					break;

				case RTTI_UNITTYPE:
				case RTTI_UNIT:
				case RTTI_VESSELTYPE:
				case RTTI_VESSEL:
					action = ACTION_TOGGLE_PRIMARY;
					break;

				case RTTI_NONE:
					action = ACTION_NONE;
					break;

				default:
					break;
			}

		} else {
			action = ACTION_NONE;
		}
	}

	/*
	**	Don't allow targeting of SAM sites, even if the CTRL key
	**	is held down. Also don't allow targeting if the object is too
	**	far away.
	*/
	if (action == ACTION_ATTACK && (*this == STRUCT_SAM || *this == STRUCT_AAGUN || !In_Range(object, 0))) {
		action = ACTION_NONE;
	}

	if (action == ACTION_MOVE) {
		action = ACTION_NONE;
	}

	return(action);
}


/***********************************************************************************************
 * BuildingClass::What_Action -- Determines what action will occur.                            *
 *                                                                                             *
 *    This routine examines the cell specified and returns with the action that will be        *
 *    performed if that cell were clicked upon while the building is selected.                 *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns the ActionType that indicates what should occur if the mouse is clicked    *
 *          on this cell.                                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType BuildingClass::What_Action(CELL cell) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	ActionType action = TechnoClass::What_Action(cell);

	if (action == ACTION_MOVE && (*this != STRUCT_CONST || !Is_MCV_Deploy())) {
		action = ACTION_NONE;
	}

	/*
	**	Don't allow targeting of SAM sites, even if the CTRL key
	**	is held down.
	*/
	if (action == ACTION_ATTACK && Class->PrimaryWeapon != NULL && !Class->PrimaryWeapon->Bullet->IsAntiGround) {
//	if (action == ACTION_ATTACK && (*this == STRUCT_SAM || *this == STRUCT_AAGUN)) {
		action = ACTION_NONE;
	}

	return(action);
}


/***********************************************************************************************
 * BuildingClass::Begin_Mode -- Begins an animation mode for the building.                     *
 *                                                                                             *
 *    This routine will start the building animating. This animation will loop indefinitely    *
 *    until explicitly stopped.                                                                *
 *                                                                                             *
 * INPUT:   bstate   -- The animation state to initiate.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The building graphic state will reflect the first stage of this animation the   *
 *             very next time it is rendered.                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   07/02/1995 JLB : Uses normalize animation rate where applicable.                          *
 *=============================================================================================*/
void BuildingClass::Begin_Mode(BStateType bstate)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	QueueBState = bstate;
	if (BState == BSTATE_NONE || bstate == BSTATE_CONSTRUCTION || ScenarioInit) {
		BState = bstate;
		QueueBState = BSTATE_NONE;
		BuildingTypeClass::AnimControlType const * ctrl = Fetch_Anim_Control();

		int rate = ctrl->Rate;
		if (Class->IsRegulated && bstate != BSTATE_CONSTRUCTION) {
			rate = Options.Normalize_Delay(rate);
		}
		Set_Rate(rate);
		Set_Stage(ctrl->Start);
	}
}


/***********************************************************************************************
 * BuildingClass::Center_Coord -- Fetches the center coordinate for the building.              *
 *                                                                                             *
 *    This routine is used to set the center coordinate for this building.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate for the center location for the building.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE BuildingClass::Center_Coord(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	return(Coord_Add(Coord, CenterOffset[Class->Size]));
}


/***********************************************************************************************
 * BuildingClass::Docking_Coord -- Fetches the coordinate to use for docking.                  *
 *                                                                                             *
 *    This routine will return the coordinate to use when an object wishes to dock with this   *
 *    building. Normally the docking coordinate would be the center of the building.           *
 *    Exceptions to this would be the airfield and helipad. Their docking coordinates are      *
 *    offset to match the building artwork.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to head to when trying to dock with this building.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE BuildingClass::Docking_Coord(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (*this == STRUCT_HELIPAD) {
		return(Coord_Add(Coord, XYP_COORD(24, 18)));
	}
	if (*this == STRUCT_AIRSTRIP) {
		return(Coord_Add(Coord, XYP_COORD(ICON_PIXEL_W + ICON_PIXEL_W/2, 28)));
	}
	return(TechnoClass::Docking_Coord());
}


/***********************************************************************************************
 * BuildingClass::Can_Fire -- Determines if this building can fire.                            *
 *                                                                                             *
 *    Use this routine to see if the building can fire its weapon.                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   target   -- The target that firing upon is desired.                                *
 *                                                                                             *
 *          which    -- Which weapon to use when firing. 0=primary, 1=secondary.               *
 *                                                                                             *
 * OUTPUT:  Returns with the fire possibility code. If firing is allowed, then FIRE_OK is      *
 *          returned. Other cases will result in appropriate fire code value that indicates    *
 *          why firing is not allowed.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/03/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
FireErrorType BuildingClass::Can_Fire(TARGET target, int which) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	FireErrorType canfire = TechnoClass::Can_Fire(target, which);

	if (canfire == FIRE_OK) {

		/*
		**	Double check to make sure that the facing is roughly toward
		**	the target. If the difference is too great, then firing is
		**	temporarily postponed.
		*/
		if (Class->IsTurretEquipped) {
			int diff = PrimaryFacing.Difference(Direction(TarCom));
			diff = abs(diff);
			if (ABS(diff) > (*this == STRUCT_SAM ? 64 : 8)) {
//			if (ABS(diff) > 8) {
				return(FIRE_FACING);
			}

			/*
			**	If the turret is rotating then firing must be delayed.
			*/
//			if (PrimaryFacing.Is_Rotating()) {
//				return(FIRE_ROTATING);
//			}
		}

		/*
		**	Certain buildings cannot fire if there is insufficient power.
		*/
		if (Class->IsPowered && House->Power_Fraction() < 1) {
			return(FIRE_BUSY);
		}

		/*
		** If an obelisk can fire, check the state of charge.
		*/
		if (Class->PrimaryWeapon != NULL && Class->PrimaryWeapon->IsElectric && !IsCharged) {
			return(FIRE_BUSY);
		}
	}
	return(canfire);
}


/***********************************************************************************************
 * BuildingClass::Toggle_Primary -- Toggles the primary factory state.                         *
 *                                                                                             *
 *    This routine will change the primary factory state of this building. The primary         *
 *    factory is the one that units will be produced from (by default).                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Is this building NOW the primary factory?                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/03/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Toggle_Primary(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (IsLeader) {
		IsLeader = false;
	} else {
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * building = Buildings.Ptr(index);

			if (!building->IsInLimbo && building->Owner() == Owner() && building->Class->ToBuild == Class->ToBuild) {
				if (Class->ToBuild == RTTI_INFANTRYTYPE) {
					if (*building == STRUCT_KENNEL && *this == STRUCT_KENNEL) {
						building->IsLeader = false;
					} else {
						if (*building != STRUCT_KENNEL && *this != STRUCT_KENNEL) {
							building->IsLeader = false;
						}
					}
				} else if (Class->ToBuild == RTTI_AIRCRAFTTYPE) {
					if (*building == STRUCT_AIRSTRIP && *this == STRUCT_AIRSTRIP) {
						building->IsLeader = false;
					} else {
						if (*building != STRUCT_AIRSTRIP && *this != STRUCT_AIRSTRIP) {
							building->IsLeader = false;
						}
					}
				} else {
					building->IsLeader = false;
				}
			}
		}
		IsLeader = true;
		//
		// MBL 04.20.2020 - Update so that each player in multiplayer will properly hear this when it applies to them
		//
		// if ((HouseClass *)House == PlayerPtr) {
		// 	Speak(VOX_PRIMARY_SELECTED);
		// }
		if ((HouseClass *)House->IsHuman) {
			Speak(VOX_PRIMARY_SELECTED, House);
		}
	}
	Mark(MARK_CHANGE);
	return(IsLeader);
}


/***********************************************************************************************
 * BuildingClass::Captured -- Captures the building.                                           *
 *                                                                                             *
 *    This routine will change the owner of the building. It handles updating any related      *
 *    game systems as a result. Factories are the most prone to have great game related        *
 *    consequences when captured. This could also affect the sidebar and building ownership.   *
 *                                                                                             *
 * INPUT:   newowner -- Pointer to the house that is now the new owner.                        *
 *                                                                                             *
 * OUTPUT:  Was the capture attempt successful?                                                *
 *                                                                                             *
 * WARNINGS:   Capturing could fail if the house is already owned by the one specified or      *
 *             the building isn't allowed to be captured.                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/03/1995 JLB : Created.                                                                 *
 *   07/05/1995 JLB : Fixed production problem with capturing enemy buildings.                 *
 *=============================================================================================*/
bool BuildingClass::Captured(HouseClass * newowner)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (Class->IsCaptureable && newowner != House) {
#ifdef TOFIX
		switch (Owner()) {
			case HOUSE_GOOD:
				Speak(VOX_GDI_CAPTURED);
				break;

			case HOUSE_BAD:
				Speak(VOX_NOD_CAPTURED);
				break;
		}
#endif

		/*
		** Maybe trigger an achivement. ST - 11/14/2019 1:53PM
		*/
		if (newowner->IsHuman) {
			TechnoTypeClass const *object_type = Techno_Type_Class();
			if (object_type) {
				if (newowner->ActLike != House->ActLike) {
					On_Achievement_Event(newowner, "OPPOSING_BUILDING_CAPTURED", object_type->IniName);
				} else {
					On_Achievement_Event(newowner, "BUILDING_CAPTURED", object_type->IniName);
				}	
			}
		}

		/*
		** Make sure the capturer isn't spying on his own building, and if
		** it was a radar facility, update the target house's RadarSpied field.
		*/
		if (SpiedBy & (1<<(newowner->Class->House)) ) {
			SpiedBy -= (1<<(newowner->Class->House));
			if (*this == STRUCT_RADAR) {
				Update_Radar_Spied();
			}
		}

		if (House == PlayerPtr) {
			Map.PowerClass::IsToRedraw = true;
			Map.Flag_To_Redraw(false);
		}

		if (*this == STRUCT_GAP) {
			Remove_Gap_Effect();
			IsJamming = false;
			Arm = 0;
		}

		/*
		** Add this building to the list of buildings captured this game. For internet stats purposes.
		*/
		if (Session.Type == GAME_INTERNET) {
			newowner->CapturedBuildings->Increment_Unit_Total (Class->Type);
		}

		House->Adjust_Power(-Power_Output());
		LastStrength = 0;
		House->Adjust_Drain(-Class->Drain);
		int booty = House->Adjust_Capacity(-Class->Capacity, true);

		/*
		**	If there is something loaded, then it gets captured as well.
		*/
		TechnoClass * tech = Attached_Object();
		if (tech) tech->Captured(newowner);

		/*
		**	If something isn't technically attached, but is sitting on this
		**	building for another reason (e.g., helicopter on helipad), then it
		**	gets captured as well.
		*/
		tech = Contact_With_Whom();
		if (tech) {
			if (Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER && (::Distance(tech->Center_Coord(), Docking_Coord()) < 0x0040 ||
				(tech->What_Am_I() == RTTI_AIRCRAFT && ((AircraftClass *)tech)->Class->IsFixedWing && ((AircraftClass *)tech)->In_Which_Layer() == LAYER_GROUND)) ) {
				tech->Captured(newowner);
			} else {
				Transmit_Message(RADIO_RUN_AWAY);
				Transmit_Message(RADIO_OVER_OUT);
			}
		}

		/*
		**	Abort any computer production in progress.
		*/
		if (Factory) {
			delete (FactoryClass *)Factory;
			Factory = 0;
		}

		/*
		**	Decrement the factory counter for the original owner.
		*/
		House->Active_Remove(this);

		/*
		**	Flag that both owners now need to update their buildable lists.
		*/
		House->IsRecalcNeeded = true;
		newowner->IsRecalcNeeded = true;
		HouseClass * oldowner = House;
		TARGET tocap = As_Target();

		IsCaptured = true;
		TechnoClass::Captured(newowner);

		oldowner->ToCapture = tocap;
		oldowner->Recalc_Center();
		House->Recalc_Center();
		if (House->ToCapture == As_Target()) {
			House->ToCapture = TARGET_NONE;
		}

		SmudgeType bib;
		CELL cell = Coord_Cell(Coord);
		if (Class->Bib_And_Offset(bib, cell)) {
			SmudgeClass * smudge = new SmudgeClass(bib);
			if (smudge) {
				smudge->Disown(cell);
				delete smudge;
			}
#ifdef FIXIT_CAPTURE_BIB
			if (Session.Type == GAME_NORMAL) {
				new SmudgeClass(bib, Cell_Coord(cell), Class->IsBase ? House->Class->House : HOUSE_NONE);
			} else {
				new SmudgeClass(bib, Cell_Coord(cell), House->Class->House);
			}
#else
			new SmudgeClass(bib, Cell_Coord(cell), House->Class->House);
#endif
		}

		House->Stole(Refund_Amount());

		/*
		**	Increment the factory count for the new owner.
		*/
		House->Active_Add(this);

		IsRepairing = false;
		Grand_Opening(true);
		House->Harvested(booty);

		Mark(MARK_CHANGE);

		/*
		**	Perform a look operation when captured if it was the player
		**	that performed the capture.
		*/
		if (Session.Type == GAME_GLYPHX_MULTIPLAYER && House->IsHuman) {
			Look(false);
		} else {
			if (House == PlayerPtr) {
				Look(false);
			}
		}
		/*
		** If it was spied upon by the player who just captured it, clear the
		** spiedby flag for that house.
		*/
		if (SpiedBy & (1 << (newowner->Class->House))) {
			SpiedBy &= ~(1 << (newowner->Class->House));
		}

		/*
		** Update the new building's colors on the radar map.
		*/
		short const * offset = Occupy_List();
		while (*offset != REFRESH_EOL) {
			CELL cell = Coord_Cell(Coord) + *offset++;
			Map.Radar_Pixel(cell);
		}

		if (oldowner) {
			oldowner->Check_Pertinent_Structures();
		}

		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * BuildingClass::Sort_Y -- Returns the building coordinate used for sorting.                  *
 *                                                                                             *
 *    The coordinate value returned from this function should be used for sorting purposes.    *
 *    It has special offset adjustment applied so that vehicles don't overlap (as much).       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a coordinate value suitable to be used for sorting.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *   06/19/1995 JLB : Handles buildings that come with bibs built-in.                          *
 *=============================================================================================*/
COORDINATE BuildingClass::Sort_Y(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (*this == STRUCT_REPAIR) {
		return(Coord);
	}
	if (*this == STRUCT_HELIPAD) {
		return(Center_Coord());
	}
	if (*this == STRUCT_AIRSTRIP) {
		return(Center_Coord());
	}
	if (*this == STRUCT_BARRACKS /*|| *this == STRUCT_POWER*/) {
		return(Center_Coord());
	}
	if (*this == STRUCT_REFINERY) {
		return(Center_Coord());
	}

	/*
	**	Mines need to bias their sort location such that they are typically drawn
	**	before any objects that might overlap them.
	*/
	if (*this == STRUCT_AVMINE || *this == STRUCT_APMINE) {
		return(Coord_Move(Center_Coord(), DIR_N, CELL_LEPTON_H));
	}

	return(Coord_Add(Center_Coord(), XY_Coord(0, (Class->Height()*256)/3)));
}


/***********************************************************************************************
 * BuildingClass::Can_Enter_Cell -- Determines if building can be placed down.                 *
 *                                                                                             *
 *    This routine will determine if the building can be placed down at the location           *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine. This is usually the cell of the upper left corner    *
 *                   of the building if it were to be placed down.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the move legality value for placement at the location specified. This *
 *          will either be MOVE_OK or MOVE_NO.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
MoveType BuildingClass::Can_Enter_Cell(CELL cell, FacingType) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (*this == STRUCT_CONST && IsDown) {
		return(Map[cell].Is_Clear_To_Build(Class->Speed) ? MOVE_OK : MOVE_NO);
	}

	if (!Debug_Map && ScenarioInit == 0 && Session.Type == GAME_NORMAL && House->IsPlayerControl && !Map[cell].IsMapped) {
		return(MOVE_NO);
	}

	return(Class->Legal_Placement(cell) ? MOVE_OK : MOVE_NO);
}


/***********************************************************************************************
 * BuildingClass::Can_Demolish -- Can the player demolish (sell back) the building?            *
 *                                                                                             *
 *    Determines if the player can sell this building. Selling is possible if the building     *
 *    is not currently in construction or deconstruction animation.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Can the building be demolished at this time?                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   07/01/1995 JLB : If there is no buildup data, then the building can't be sold.            *
 *   07/17/1995 JLB : Cannot sell a refinery that has a harvester attached.                    *
 *=============================================================================================*/
bool BuildingClass::Can_Demolish(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (Class->IsUnsellable) return(false);

	if (Class->Get_Buildup_Data() && BState != BSTATE_CONSTRUCTION && Mission != MISSION_DECONSTRUCTION && Mission != MISSION_CONSTRUCTION) {
		if (*this == STRUCT_REFINERY && Is_Something_Attached()) return(false);
		return(true);
	}
	return(false);
}


bool BuildingClass::Can_Demolish_Unit(void) const
{
	return((*this == STRUCT_REPAIR || *this == STRUCT_AIRSTRIP) && In_Radio_Contact() && Distance(Contact_With_Whom()) < 0x0080);
}


/***********************************************************************************************
 * BuildingClass::Mission_Guard -- Handles guard mission for combat buildings.                 *
 *                                                                                             *
 *    Buildings that can attack are given this mission. They will wait until a suitable target *
 *    comes within range and then launch into the attack mission. Buildings that have no       *
 *    weaponry will just sit in this routine forever.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before this routine will be called *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Mission_Guard(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	/*
	**	If this building has a weapon, then search for a target to attack. When
	**	a target is found, switch into attack mode to deal with the threat.
	*/
	if (Is_Weapon_Equipped()) {

		/*
		**	Weapon equipped buildings are ALWAYS ready to launch into another mission if
		**	they are sitting around in guard mode.
		*/
		IsReadyToCommence = true;

		/*
		**	If there is no target available, then search for one.
		*/
		if (!Target_Legal(TarCom)) {
			ThreatType threat = THREAT_NORMAL;
			Assign_Target(Greatest_Threat(threat));
		}

		/*
		**	There is a valid target. Switch into attack mode right away.
		*/
		if (Target_Legal(TarCom)) {
			Assign_Mission(MISSION_ATTACK);
			Commence();
			return(1);
		}
	} else {

		/*
		**	This is the very simple state machine that basically does
		**	nothing. This is the mode that non weapon equipped buildings
		**	are normally in.
		*/
		enum {
			INITIAL_ENTRY,
			IDLE
		};
		switch (Status) {
			case INITIAL_ENTRY:
				Begin_Mode(BSTATE_IDLE);
				Status = IDLE;
				break;

			case IDLE:
				/*
				**	Special case to break out of guard mode if this is a repair
				**	facility and there is a customer waiting at the grease pit.
				*/
				if (*this == STRUCT_REPAIR &&
					In_Radio_Contact() &&
					Contact_With_Whom()->Is_Techno() &&
					((TechnoClass *)Contact_With_Whom())->Mission == MISSION_ENTER &&
					Distance(Contact_With_Whom()) < 0x0040 &&
					Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {

					Assign_Mission(MISSION_REPAIR);
					return(1);
				}
				break;

			default:
				break;
		}

		if (*this == STRUCT_REPAIR) {
			return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
		} else {
			return(MissionControl[Mission].Normal_Delay() * 3 + Random_Pick(0, 2));
		}
	}
	return(MissionControl[Mission].AA_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * BuildingClass::Mission_Construction -- Handles mission construction.                        *
 *                                                                                             *
 *    This routine will handle mission construction. When this mission is complete, the        *
 *    building will begin normal operation.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Mission_Construction(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	enum {
		INITIAL,
		DURING
	};
	switch (Status) {
		case INITIAL:
			Begin_Mode(BSTATE_CONSTRUCTION);
			Transmit_Message(RADIO_BUILDING);
			if (House->IsPlayerControl) {
				Sound_Effect(VOC_CONSTRUCTION, Coord);
			}
			Status = DURING;
			break;

		case DURING:
			if (IsReadyToCommence) {

				/*
				**	When construction is complete, then transmit this
				**	to the construction yard so that it can stop its
				**	construction animation.
				*/
				Transmit_Message(RADIO_COMPLETE);		// "I'm finished."
				Transmit_Message(RADIO_OVER_OUT);		// "You're free."
				Begin_Mode(BSTATE_IDLE);
				Grand_Opening();
				Assign_Mission(MISSION_GUARD);
				PrimaryFacing = Class->StartFace;
			}
			break;

		default:
			break;
	}
	return(1);
}


/***********************************************************************************************
 * BuildingClass::Mission_Deconstruction -- Handles building deconstruction.                   *
 *                                                                                             *
 *    This state machine is only used when the building is deconstructing as a result of       *
 *    selling.  When this mission is finished, the building will no longer exist.              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   08/13/1995 JLB : Enable selling of units on a repair bay.                                 *
 *   08/20/1995 JLB : Scatters infantry from scattered starting points.                        *
 *=============================================================================================*/
int BuildingClass::Mission_Deconstruction(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	/*
	**	Always force repair off.
	*/
	Repair(0);

	enum {
		INITIAL,
		HOLDING,
		DURING
	};
	switch (Status) {
		case INITIAL:

			/*
			**	Special check for the repair bay which has the ability to sell
			**	whatever is on it. If there is something on the repair bay, then
			**	it will be sold. If there is nothing on the repair bay, then
			**	the repair bay itself will be sold.
			*/
			if (Can_Demolish_Unit() && Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {
				TechnoClass * tech = Contact_With_Whom();
				Transmit_Message(RADIO_OVER_OUT);
				if (IsOwnedByPlayer) Speak(VOX_UNIT_SOLD);
				tech->Sell_Back(1);
				Assign_Mission(MISSION_GUARD);
				return(1);
			}

			/*
			** Selling off a shipyard or sub pen may cause attached ships
			** who are repairing themselves to discontinue repairs.
			*/
			if (*this == STRUCT_SHIP_YARD || *this == STRUCT_SUB_PEN) {
				for (int index = 0; index < Vessels.Count(); index++) {
					VesselClass * obj = Vessels.Ptr(index);
					if (obj && !obj->IsInLimbo && obj->House == House) {
						if (obj->IsSelfRepairing) {
							if (::Distance(Center_Coord(), obj->Center_Coord()) < 0x0200) {
								obj->IsSelfRepairing = false;
								obj->IsToSelfRepair = false;
							}
						}
					}
				}
			}

			IsReadyToCommence = false;
			Transmit_Message(RADIO_RUN_AWAY);
			Status = HOLDING;
			break;

		case HOLDING:
			if (!IsTethered) {

				/*
				**	The crew will evacuate from the building. The number of crew
				**	members leaving is equal to the unrecovered cost of the building
				**	divided by 100 (the typical cost of a minigunner infantryman).
				*/
				if (!Target_Legal(ArchiveTarget) || !Is_MCV_Deploy() || *this != STRUCT_CONST) {
					int count = How_Many_Survivors();
					bool engine = false;

					while (count) {

						/*
						**	Ensure that the player only gets ONE engineer and not from a captured
						**	construction yard.
						*/
						InfantryType typ = Crew_Type();
						while (typ == INFANTRY_RENOVATOR && engine) {
							typ = Crew_Type();
						}
						if (typ == INFANTRY_RENOVATOR) engine = true;

						InfantryClass * infantry = 0;
						if (typ != INFANTRY_NONE) infantry = new InfantryClass(typ, House->Class->House);
						if (infantry != NULL) {
							ScenarioInit++;
							COORDINATE coord = Coord_Add(Center_Coord(), XYP_COORD(0, -12));
							coord = Map[coord].Closest_Free_Spot(coord, false);

							if (infantry->Unlimbo(coord, DIR_N)) {
								infantry->IsZoneCheat = infantry->Can_Enter_Cell(Coord_Cell(infantry->Center_Coord())) != MOVE_OK;
								if (infantry->Class->IsNominal) infantry->IsTechnician = true;
								ScenarioInit--;
								infantry->Scatter(0, true);
								ScenarioInit++;
								infantry->Assign_Mission(MISSION_GUARD_AREA);
							} else {
								delete infantry;
							}
							ScenarioInit--;
						}
						count--;
					}
				}

				if (House->IsPlayerControl) {
					Sound_Effect(VOC_CASHTURN, Coord);
				}
				
				/*
				**	Destroy all attached objects. ST - 4/24/2020 9:38PM
				*/
				while (Attached_Object()) {
					FootClass * obj = Detach_Object();

					Detach_All(true);
					delete obj;
				}
				
				Transmit_Message(RADIO_OVER_OUT);
				Status = DURING;
				Begin_Mode(BSTATE_CONSTRUCTION);
				IsReadyToCommence = false;
				break;
			}
			Transmit_Message(RADIO_RUN_AWAY);
			break;

		case DURING:
			if (IsReadyToCommence) {
				House->IsRecalcNeeded = true;

  				// MBL 05.06.2020 - "Structure Sold" is being heard when selecting/moving a redepolyable Con Yard to turn back into an MCV (RA only), so moving below
				#if 0
				// MBL 04.06.2020: Fix being heard by wrong player
				// if (IsOwnedByPlayer) Speak(VOX_STRUCTURE_SOLD);
				if (IsOwnedByPlayer) {
					if ((HouseClass *)House == PlayerPtr) {
						Speak(VOX_STRUCTURE_SOLD);
					}
				}
				#endif
				bool mcv_redeployed = false;

				/*
				**	Construction yards that deconstruct, really just revert back
				**	to an MCV.
				*/
				if (Target_Legal(ArchiveTarget) && *this == STRUCT_CONST && House->IsHuman && Strength > 0) {
					ScenarioInit++;
					UnitClass * unit = new UnitClass(UNIT_MCV, House->Class->House);
					ScenarioInit--;
					if (unit != NULL) {

						/*
						**	Unlimbo the MCV onto the map. The MCV should start in the same
						**	health condition that the construction yard was in.
						*/
						fixed ratio = Health_Ratio();
						int money = Refund_Amount();
						TARGET arch = ArchiveTarget;
						COORDINATE place = Coord_Snap(Adjacent_Cell(Coord, DIR_SE));

						delete this;

						if (unit->Unlimbo(place, DIR_SW)) {
							unit->Strength = (int)unit->Class_Of().MaxStrength * ratio;			// Cast to (int). ST - 5/8/2019

							/*
							**	Lift the move destination from the building and assign
							**	it to the unit.
							*/
							if (Target_Legal(arch)) {
								unit->Assign_Destination(arch);
								unit->Assign_Mission(MISSION_MOVE);
							}

							mcv_redeployed = true;

						} else {

							/*
							**	If, for some strange reason, the MCV could not be placed on the
							**	map, then give the player some money to compensate.
							*/
							House->Refund_Money(money);
						}
					} else {
						House->Refund_Money(Refund_Amount());
						delete this;
					}

				} else {

					/*
					** Selling off a gap generator will cause the cells it affects
					** to stop being jammed.
					*/
					if (*this == STRUCT_GAP) {
						Remove_Gap_Effect();
					}

					/*
					**	A sold building still counts as a kill, but it just isn't directly
					**	attributed to the enemy.
					*/
					WhoLastHurtMe = HOUSE_NONE;
					Record_The_Kill(NULL);

					/*
					**	The player gets part of the money back for the sell.
					*/
					House->Refund_Money(Refund_Amount());
					House->Stole(-Refund_Amount());
					Limbo();

					if (House) {
						House->Check_Pertinent_Structures();
					}

					/*
					**	Finally, delete the building from the game.
					*/
					delete this;
				}

				// MBL 05.06.2020 - "Structure Sold" was being heard when selecting/moving a redepolyable Con Yard to turn back into an MCV (RA only) above, so moved here
				#if 1
				if (!mcv_redeployed) 
				{
					if (IsOwnedByPlayer) {
						if ((HouseClass *)House == PlayerPtr) {
							Speak(VOX_STRUCTURE_SOLD);
						}
					}
				}
				#endif
			}
			break;

		default:
			break;
	}
	return(1);
}


/***********************************************************************************************
 * BuildingClass::Mission_Attack -- Handles attack mission for building.                       *
 *                                                                                             *
 *    Buildings that can attack are processed by this attack mission state machine.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   02/22/1996 JLB : SAM doesn't lower back into ground.                                      *
 *=============================================================================================*/
int BuildingClass::Mission_Attack(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (*this == STRUCT_SAM) {
		switch (Status) {

			/*
			**	This is the target tracking state of the launcher. It will rotate
			**	to face the current TarCom of the launcher.
			*/
			case SAM_READY:
				if ((Class->IsPowered && House->Power_Fraction() < 1) || IsJammed) {
					return(1);
				}
				if (!Target_Legal(TarCom) || !Is_Target_Aircraft(TarCom) || As_Aircraft(TarCom)->Height == 0) {
					Assign_Target(TARGET_NONE);
					Status = SAM_READY;
					Assign_Mission(MISSION_GUARD);
					Commence();
					return(1);
				} else {
					if (!PrimaryFacing.Is_Rotating()) {
						DirType facing = Direction(TarCom);
						if (PrimaryFacing.Difference(facing)) {
							PrimaryFacing.Set_Desired(facing);
						} else {
							Status = SAM_FIRING;
						}
					}
				}
				return(1);

			/*
			**	The launcher is in the process of firing.
			*/
			case SAM_FIRING:
				if (!Target_Legal(TarCom) || !Is_Target_Aircraft(TarCom) || As_Aircraft(TarCom)->Height == 0) {
					Assign_Target(TARGET_NONE);
					Status = SAM_READY;
				} else {
					FireErrorType error = Can_Fire(TarCom, 0);
					if (error == FIRE_ILLEGAL || error == FIRE_CANT || error == FIRE_RANGE) {
						Assign_Target(TARGET_NONE);
						Status = SAM_READY;
					} else {
						if (error == FIRE_FACING) {
							Status = SAM_READY;
						} else {
							if (error == FIRE_OK) {
								Fire_At(TarCom, 0);
								Fire_At(TarCom, 1);
								Status = SAM_READY;
							}
						}
					}
				}
				return(1);

			default:
				break;
		}
		return(MissionControl[Mission].AA_Delay() + Random_Pick(0, 2));

	}

	if (!Target_Legal(TarCom)) {
		Assign_Target(TARGET_NONE);
		Assign_Mission(MISSION_GUARD);
		Commence();
		return(1);
	}

	int primary = What_Weapon_Should_I_Use(TarCom);
	IsReadyToCommence = true;
	switch (Can_Fire(TarCom, primary)) {
		case FIRE_ILLEGAL:
		case FIRE_CANT:
		case FIRE_RANGE:
		case FIRE_AMMO:
			Assign_Target(TARGET_NONE);
			Assign_Mission(MISSION_GUARD);
			Commence();
			break;

		case FIRE_FACING:
			PrimaryFacing.Set_Desired(Direction(TarCom));
			return(2);

		case FIRE_REARM:
			PrimaryFacing.Set_Desired(Direction(TarCom));
			return(Arm);

		case FIRE_BUSY:
			return(1);

		case FIRE_CLOAKED:
			Do_Uncloak();
			break;

		case FIRE_OK:
			Fire_At(TarCom, primary);
			return(1);

		default:
			break;
	}
	PrimaryFacing.Set_Desired(Direction(TarCom));
	return(1);
//	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * BuildingClass::Mission_Harvest -- Handles refinery unloading harvesters.                    *
 *                                                                                             *
 *    This state machine handles the refinery when it unloads the harvester.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Mission_Harvest(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	enum {
		INITIAL,					// Dock the Tiberium cannister.
		WAIT_FOR_DOCK,			// Waiting for docking to complete.
		MIDDLE,					// Offload "bails" of tiberium.
		WAIT_FOR_UNDOCK		// Waiting for undocking to complete.
	};
	switch (Status) {
		case INITIAL:
			Status = WAIT_FOR_DOCK;
			break;

		case WAIT_FOR_DOCK:
			if (IsReadyToCommence) {
				IsReadyToCommence = false;
				Status = MIDDLE;
			}
			break;

		case MIDDLE:
			if (IsReadyToCommence) {
				IsReadyToCommence = false;

				/*
				**	Force any bib squatters to scatter.
				*/
				Map[Adjacent_Cell(Coord_Cell(Center_Coord()), DIR_S)].Incoming(0, true, true);

				FootClass * techno = Attached_Object();
				if (techno) {
					int bail = techno->Offload_Tiberium_Bail();

					if (bail) {
						House->Harvested(bail);
						if (techno->Tiberium_Load() > 0) {
							return(1);
						}
					}
				}
				Status = WAIT_FOR_UNDOCK;
			}
			break;

		case WAIT_FOR_UNDOCK:
			if (IsReadyToCommence) {

				/*
				**	Detach harvester and go back into idle state.
				*/
				Assign_Mission(MISSION_GUARD);
			}
			break;

		default:
			break;
	}
	return(1);
}


/***********************************************************************************************
 * BuildingClass::Mission_Repair -- Handles the repair (active) state for building.            *
 *                                                                                             *
 *    This state machine is used when the building is active in some sort of repair or         *
 *    construction mode. The construction yard will animate. The repair facility will repair   *
 *    anything that it docked on it.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   06/25/1995 JLB : Handles repair facility                                                  *
 *   07/29/1995 JLB : Repair rate is controlled by power rating.                               *
 *=============================================================================================*/
int BuildingClass::Mission_Repair(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (*this == STRUCT_CONST) {
		enum {
			INITIAL,
			DURING
		};
		switch (Status) {
			case INITIAL:
				Begin_Mode(BSTATE_ACTIVE);
				Status = DURING;
				break;

			case DURING:
				if (!In_Radio_Contact()) {
					Assign_Mission(MISSION_GUARD);
				}
				break;

			default:
				break;
		}
		return(1);
	}

	if (*this == STRUCT_REPAIR) {
		enum {
			INITIAL,
			IDLE,
			DURING
		};
		switch (Status) {
			case INITIAL:
				{
					if (!In_Radio_Contact()) {
						Begin_Mode(BSTATE_IDLE);
						Assign_Mission(MISSION_GUARD);
						return(1);
					}
					IsReadyToCommence = false;
					int distance = 0x10;
					TechnoClass *tech = Contact_With_Whom();

					/*
					** BG: If the unit to repair is an aircraft, and the aircraft is
					** fixed-wing, and it's landed, be much more liberal with the
					** distance check.  Fixed-wing aircraft are very inaccurate with
					** their landings.
					*/
					if (tech->What_Am_I() == RTTI_AIRCRAFT) {
						if ( ((AircraftClass *)tech)->Class->IsFixedWing &&
							((AircraftClass *)tech)->In_Which_Layer() == LAYER_GROUND) {
							distance = 0x80;
						}
					}
					if (Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER && Distance(Contact_With_Whom()) < distance) {
						Status = IDLE;
						return(TICKS_PER_SECOND/4);
					}
					break;
				}

			case IDLE:
				if (!In_Radio_Contact()) {
					Assign_Mission(MISSION_GUARD);
					return(1);
				}

				if (Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {
					TechnoClass * radio = Contact_With_Whom();

					if ( ((radio->Health_Ratio() < Rule.ConditionGreen) ||
						   (radio->What_Am_I() == RTTI_UNIT && *(UnitClass *)radio == UNIT_MINELAYER))
							&& Transmit_Message(RADIO_REPAIR) == RADIO_ROGER) {

						/*
						**	If the object over the repair bay is marked as useless, then
						**	sell it back to get some money.
						*/
						if (radio->IsUseless) {
							if (!radio->House->IsHuman) {
								radio->Sell_Back(1);
							}
							Status = INITIAL;
							IsReadyToCommence = true;
						} else {

							//
							// MBL 04.27.2020: Legacy VOX_REPAIRING seems to be never called in TD, but only in RA.
							// It is currently supported as a client GUI event when standard repairing begins, with "REPAIR1" on both TD and RA
							//
							// This repairing is in reference to the repair bay
							// There is a newer bug (https://jaas.ea.com/browse/TDRA-6224) reporting that it is heard in multiplayer by 
							// other players, from this call, so modifiying the original call here:
							//
							// if (IsOwnedByPlayer) Speak(VOX_REPAIRING);
							if (IsOwnedByPlayer) Speak(VOX_REPAIRING, House);

							Status = DURING;
							Begin_Mode(BSTATE_ACTIVE);
							IsReadyToCommence = false;
						}
					} else {
//						Transmit_Message(RADIO_RUN_AWAY);
///*BG*/					if(radio->Health_Ratio() >= Rule.ConditionGreen) {
//								Transmit_Message(RADIO_RUN_AWAY);
//							}
					}
				}
				break;

			case DURING:
				if (!In_Radio_Contact()) {
					Begin_Mode(BSTATE_IDLE);
					Status = IDLE;
					return(1);
				}

				/*
				**	Check to see if the repair light blink has completed and the attached
				**	unit is not doing something else. If these conditions are favorable,
				**	the repair can proceed another step.
				*/
				if (IsReadyToCommence && Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {
					IsReadyToCommence = false;

					/*
					**	Tell the attached unit to repair one step. It will respond with how
					**	it fared.
					*/
					switch (Transmit_Message(RADIO_REPAIR)) {

						/*
						**	The repair step proceeded smoothly. Proceed normally with the
						**	repair process.
						*/
						case RADIO_ROGER:
							break;

						/*
						**	The repair operation was aborted because of some reason. Presume
						**	that the reason is because of low cash.
						*/
						case RADIO_CANT:
							if (IsOwnedByPlayer) Speak(VOX_NO_CASH);
							Begin_Mode(BSTATE_IDLE);
							Status = IDLE;
							break;

						/*
						**	The repair step resulted in a completely repaired unit.
						*/
						case RADIO_ALL_DONE:

							// MBL 04.27.2020: Make only audible to the correct player
							// if (IsOwnedByPlayer) Speak(VOX_UNIT_REPAIRED);
							if (IsOwnedByPlayer) Speak(VOX_UNIT_REPAIRED, House);

//							Transmit_Message(RADIO_RUN_AWAY);
							Begin_Mode(BSTATE_IDLE);
							Status = IDLE;
							break;

						/*
						**	The repair step could not be completed because this unit is already
						**	at full strength.
						*/
						case RADIO_NEGATIVE:
						default:
//							Transmit_Message(RADIO_RUN_AWAY);
							Begin_Mode(BSTATE_IDLE);
							Status = IDLE;
							break;

					}
				}
				return(1);

			default:
				break;
		}
		return(MissionControl[Mission].Normal_Delay());
	}

	if (*this == STRUCT_HELIPAD || *this == STRUCT_AIRSTRIP) {
		enum {
			INITIAL,
			DURING
		};
		switch (Status) {
			case INITIAL:
				if (Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER && Transmit_Message(RADIO_PREPARED) == RADIO_NEGATIVE) {
					Begin_Mode(BSTATE_ACTIVE);
					Contact_With_Whom()->Assign_Mission(MISSION_SLEEP);
					Status = DURING;
					return(1);
				}
				Assign_Mission(MISSION_GUARD);
				break;

			case DURING:
				if (IsReadyToCommence) {
					if (!In_Radio_Contact() || Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_NEGATIVE) {
						Assign_Mission(MISSION_GUARD);
						return(1);
					}

					if (Transmit_Message(RADIO_PREPARED) == RADIO_ROGER) {
						Contact_With_Whom()->Assign_Mission(MISSION_GUARD);
						Assign_Mission(MISSION_GUARD);
						return(1);
					}

					if (Transmit_Message(RADIO_RELOAD) != RADIO_ROGER) {
						Assign_Mission(MISSION_GUARD);
						Contact_With_Whom()->Assign_Mission(MISSION_GUARD);
						return(1);
					} else {
						fixed pfrac = Saturate(House->Power_Fraction(), 1);
						if (pfrac < fixed::_1_2) pfrac = fixed::_1_2;
						int time = Inverse(pfrac) * Rule.ReloadRate * TICKS_PER_MINUTE;
//						int time = Bound((int)(TICKS_PER_SECOND * Saturate(House->Power_Fraction(), 1)), 0, TICKS_PER_SECOND);
//						time = (TICKS_PER_SECOND*3) - time;
						IsReadyToCommence = false;
						return(time);
					}
				}
				break;

			default:
				break;
		}
		return(3);
	}
	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * BuildingClass::Mission_Missile -- State machine for nuclear missile launch.                 *
 *                                                                                             *
 *    This handles the Temple of Nod launching its nuclear missile.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of frames to delay before calling this routine again.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Commented.                                                               *
 *=============================================================================================*/
int BuildingClass::Mission_Missile(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (*this == STRUCT_ADVANCED_TECH) {
		enum {
			DOOR_OPENING,
			LAUNCH_UP,
			SATELLITE_DEPLOY,
			DONE_LAUNCH
		};

		switch (Status) {

			/*
			** The initial case is responsible for starting the door
			** opening on the building, the missile rising, and smoke broiling.
			*/
			case DOOR_OPENING:
				{
#ifdef FIXIT_VERSION_3
					COORDINATE door = Coord_Move(Center_Coord(), (DirType)0xC0, 0x30);
					AnimClass * sput = new AnimClass(ANIM_SPUTDOOR, door);
					if (sput) {
						IsReadyToCommence = false;
						Status = LAUNCH_UP;
						AnimToTrack = sput->As_Target();
					}
#else
					IsReadyToCommence = false;
					COORDINATE door = Coord_Move(Center_Coord(), (DirType)0xC0, 0x30);
					AnimClass * sput = new AnimClass(ANIM_SPUTDOOR, door);
					Status = LAUNCH_UP;
					AnimToTrack = sput->As_Target();
					return(1);
#endif
				}

			/*
			** Once the smoke has been going for a little while this
			** actually handles launching the missile into the air.
			*/
			case LAUNCH_UP:
				{
					AnimClass * sput = As_Animation(AnimToTrack);
					if (sput) {
						if (sput->Fetch_Stage() >= 19) {
							CELL center = Coord_Cell(Center_Coord());
							CELL cell = XY_Cell( Cell_X(center), 1);
							TARGET targ = ::As_Target(cell);

							BulletClass * bullet = new BulletClass(BULLET_GPS_SATELLITE, targ, this, 200, WARHEAD_FIRE, MPH_ROCKET);
							if (bullet) {
								COORDINATE launch = Coord_Move(Center_Coord(), (DirType)0xC0, 0x30);
								if (!bullet->Unlimbo(launch, DIR_N)) {
									delete bullet;
									bullet = NULL;
								}
							}

							if (bullet) {
								Assign_Mission(MISSION_GUARD);
							}
						}
					}
				}
				return(1);
		}
	}

	if (*this == STRUCT_MSLO) {
		enum {
			INITIAL,
			DOOR_OPENING,
			LAUNCH_UP,
			LAUNCH_DOWN,
			DONE_LAUNCH
		};

		switch (Status) {

			/*
			** The initial case is responsible for starting the door
			** opening on the building.
			*/
			case INITIAL:
				IsReadyToCommence = false;
				Begin_Mode(BSTATE_ACTIVE);	// open the door
				Status = DOOR_OPENING;
				return(1);

			/*
			** This polls for the case when the door is actually open and
			** then kicks off the missile smoke.
			*/
			case DOOR_OPENING:
				if (IsReadyToCommence) {
					Begin_Mode(BSTATE_AUX1);	// hold the door open
					Status = LAUNCH_UP;
					return(14);
				}
				return(1);

			/*
			** Once the smoke has been going for a little while this
			** actually handles launching the missile into the air.
			*/
			case LAUNCH_UP:
				{
					CELL center = Coord_Cell(Center_Coord());
					CELL cell = XY_Cell( Cell_X(center), 1);
					TARGET targ = ::As_Target(cell);
					BulletClass * bullet = new BulletClass(BULLET_NUKE_UP, targ, this, 200, WARHEAD_HE, MPH_VERY_FAST);
					if (bullet) {
						COORDINATE launch = Coord_Move(Center_Coord(), (DirType)28, 0xA0);
						if (!bullet->Unlimbo(launch, DIR_N)) {
							delete bullet;
							bullet = NULL;
						}
					}

					if (bullet) {
						Speak(VOX_ABOMB_LAUNCH);
						Status = LAUNCH_DOWN;
						/*
						** Hack: If it's the artificial nukes, don't let the bullets come down (as
						** they're the only ones that blow up).  We know it's artificial if you're
						** at tech level 10 or below, because you can't build the nuclear silo until
						** tech level 15 or so.
						*/
						if (House->Control.TechLevel <= 10) {
							return(6);
						}
						bullet = new BulletClass(BULLET_NUKE_DOWN, ::As_Target(House->NukeDest), this, 200, WARHEAD_NUKE, MPH_VERY_FAST);
						if (bullet) {
							int celly = Cell_Y(House->NukeDest);
							celly -= 64;
							if (celly < 1) celly = 1;
							COORDINATE start = Cell_Coord(XY_Cell(Cell_X(House->NukeDest), celly));
							if (!bullet->Unlimbo(start, DIR_S)) {
								delete bullet;
							}
						}
						return(8 * TICKS_PER_SECOND);
					}
				}
				return(1);

			/*
			** Once the missile is in the air, this handles waiting for
			** the missile to be off the screen and then launching one down
			** over the target.
			*/
			case LAUNCH_DOWN:
				{
					Begin_Mode(BSTATE_AUX2);	// start the door closing

#ifdef OBSOLETE
					/*
					** Hack: If it's the artificial nukes, don't let the bullets come down (as
					** they're the only ones that blow up).  We know it's artificial if you're
					** at tech level 10 or below, because you can't build the nuclear silo until
					** tech level 15 or so.
					*/
					if (House->Control.TechLevel <= 10) {
						Status = DONE_LAUNCH;
						return(6);
					}
					BulletClass * bullet = new BulletClass(BULLET_NUKE_DOWN, ::As_Target(House->NukeDest), this, 200, WARHEAD_NUKE, MPH_VERY_FAST);
					if (bullet) {
						int celly = Cell_Y(House->NukeDest);
						celly -= 15;
						if (celly < 1) celly = 1;
						COORDINATE start = Cell_Coord(XY_Cell(Cell_X(House->NukeDest), celly));
						if (!bullet->Unlimbo(start, DIR_S)) {
							delete bullet;
						}
					}
					if (bullet) {
#endif
						Status = DONE_LAUNCH;
						return(6);
					}
#ifdef OBSOLETE
				}
				return(1);
#endif

			/*
			** Once the missile is done launching this handles allowing
			** the building to sit there with its door closed.
			*/
			case DONE_LAUNCH:
				Begin_Mode(BSTATE_IDLE);	// keep the door closed.
				Assign_Mission(MISSION_GUARD);
				return(60);
		}
	}
	return(MissionControl[Mission].Normal_Delay());
}


/***********************************************************************************************
 * BuildingClass::Revealed -- Reveals the building to the specified house.                     *
 *                                                                                             *
 *    This routine will reveal the building to the specified house. It will handle updating    *
 *    the sidebar for player owned buildings. A player owned building that hasn't been         *
 *    revealed, is in a state of pseudo-limbo. It cannot be used for any of its special        *
 *    abilities even though it exists on the map for all other purposes.                       *
 *                                                                                             *
 * INPUT:   house -- The house that this building is being revealed to.                        *
 *                                                                                             *
 * OUTPUT:  Was this building revealed by this procedure?                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Revealed(HouseClass * house)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (TechnoClass::Revealed(house)) {

		if (!ScenarioInit) {
			House->JustBuiltStructure = Class->Type;
			House->IsBuiltSomething = true;
		}
		House->IsRecalcNeeded = true;

		/*
		**	Perform any grand opening here so that in the scenarios where a player
		**	owned house is not yet revealed, it won't be reflected in the sidebar
		**	selection icons.
		*/
		if (!In_Radio_Contact() && House->IsHuman && Mission != MISSION_CONSTRUCTION) {
			Grand_Opening();
		} else {
			if (!In_Radio_Contact() && !House->IsHuman && house == House && Mission != MISSION_CONSTRUCTION) {
				Grand_Opening();
			}
		}

		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * BuildingClass::Enter_Idle_Mode -- The building will enter its idle mode.                    *
 *                                                                                             *
 *    This routine is called when the exact mode of the building isn't known. By examining     *
 *    the building's condition, this routine will assign an appropriate mission.               *
 *                                                                                             *
 * INPUT:   initial  -- This this being called during scenario init?                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Enter_Idle_Mode(bool initial)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	/*
	**	Assign an appropriate mission for the building. If the ScenarioInit flag is true, then
	**	this must be an initial building. Start such buildings in idle state. For other buildings
	**	it indicates that it is being placed during game play and thus it must start in
	**	the "construction" mission.
	*/
	MissionType mission = MISSION_GUARD;


	if (!initial || ScenarioInit || Debug_Map) {
		Begin_Mode(BSTATE_IDLE);
		mission = MISSION_GUARD;
	} else {
		Begin_Mode(BSTATE_CONSTRUCTION);
		mission = MISSION_CONSTRUCTION;
	}
	Assign_Mission(mission);
}


/***********************************************************************************************
 * BuildingClass::Pip_Count -- Determines "full" pips to display for building.                 *
 *                                                                                             *
 *    This routine will determine the number of pips that should be filled in when rendering   *
 *    the building.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of pips to display as filled in.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/28/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Pip_Count(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	return(Class->Max_Pips() * House->Tiberium_Fraction());
}


/***********************************************************************************************
 * BuildingClass::Death_Announcement -- Announce the death of this building.                   *
 *                                                                                             *
 *    This routine is called when the building is destroyed by "unnatural" means. Typically    *
 *    as a result of combat. If the building is known to the player, then it should be         *
 *    announced.                                                                               *
 *                                                                                             *
 * INPUT:   source   -- The object most directly responsible for the building's death.         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Death_Announcement(TechnoClass const * source) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (source != NULL && House->IsPlayerControl) {
		Speak(VOX_STRUCTURE_DESTROYED);
	}
}


/***********************************************************************************************
 * BuildingClass::Fire_Direction -- Fetches the direction of firing.                           *
 *                                                                                             *
 *    This routine will return with the default direction to use when firing from this         *
 *    building. This is the facing of the turret except for the case of non-turret equipped    *
 *    buildings that have a weapon (e.g., guard tower).                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the default firing direction for this building.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType BuildingClass::Fire_Direction(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (Class->IsTurretEquipped) {
		return(PrimaryFacing.Current());
	}
	return(Direction(TarCom));
}


/***********************************************************************************************
 * BuildingClass::Remap_Table -- Fetches the remap table to use for this building.             *
 *                                                                                             *
 *    Use this routine to fetch the remap table to use.  This override function is needed      *
 *    because the default remap table for techno objects presumes the object is a unit.        *
 *    Buildings aren't units.                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the proper remap table to use for this building.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * BuildingClass::Remap_Table(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	return(House->Remap_Table(IsBlushing, Class->Remap));
}


/***********************************************************************************************
 * BuildingClass::Mission_Unload -- Handles the unload mission for a building.                 *
 *                                                                                             *
 *    This is the unload mission for a building. This really only applies to the weapon's      *
 *    factory, since it needs the sophistication of an unload mission due to the door          *
 *    animation.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Mission_Unload(void)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (*this == STRUCT_WEAP) {
		CELL cell = Coord_Cell(Coord) + Class->ExitList[0];
		COORDINATE coord = Cell_Coord(cell);
		CellClass * cellptr = &Map[cell];
		enum {
			INITIAL,
			CLEAR_BIB,
			OPEN,
			LEAVE,
			CLOSE
		};
		enum {
			DOOR_STAGES = 5,
			DOOR_RATE = 8
		};
		UnitClass * unit;
		switch (Status) {
			/*
			**	Start the door opening.
			*/
			case INITIAL:
//				if (cellptr->Cell_Techno()) {
//					cellptr->Incoming(0, true);
//				}
				unit = (UnitClass *)Contact_With_Whom();
				if (unit) {
					unit->Assign_Mission(MISSION_GUARD);
					unit->Commence();
				}
				Open_Door(DOOR_RATE, DOOR_STAGES);
				Status = CLEAR_BIB;
				break;

			/*
			**	Now that the occupants can peek out the door, they will tell
			**	everyone that could be blocking the way, that they should
			**	scatter away.
			*/
			case CLEAR_BIB:
				if (cellptr->Cell_Techno()) {
					cellptr->Incoming(0, true, true);

					/*
					**	Scatter everything around the weapon's factory door.
					*/
					for (FacingType f = FACING_FIRST; f < FACING_COUNT; f++) {
						CellClass * cptr = &cellptr->Adjacent_Cell(f);
						if (cptr->Cell_Building() == NULL) {
							cptr->Incoming(coord, true, true);
						}
					}
				} else {
					Status = OPEN;
				}
				break;

			/*
			**	When the door is finally open and the way is clear, tell the
			**	unit to drive out.
			*/
			case OPEN:
				if (Is_Door_Open()) {
					unit = (UnitClass *)Contact_With_Whom();
					if (unit) {
						unit->Assign_Mission(MISSION_MOVE);

						if (House->IQ >= Rule.IQGuardArea) {
							unit->Assign_Mission(MISSION_GUARD_AREA);
							unit->ArchiveTarget = ::As_Target(House->Where_To_Go(unit));
						}
						unit->Force_Track(DriveClass::OUT_OF_WEAPON_FACTORY, coord);
//						unit->Force_Track(DriveClass::OUT_OF_WEAPON_FACTORY, Adjacent_Cell(Adjacent_Cell(Center_Coord(), FACING_S), FACING_S));
						unit->Set_Speed(128);
						Status = LEAVE;
					} else {
						Close_Door(DOOR_RATE, DOOR_STAGES);
						Status = CLOSE;
					}
				}
				break;

			/*
			**	Wait until the unit has completely left the building.
			*/
			case LEAVE:
				if (!IsTethered) {
					Close_Door(DOOR_RATE, DOOR_STAGES);
					Status = CLOSE;
				} else {

//					if (In_Radio_Contact() && !((FootClass *)Contact_With_Whom())->IsDriving) {
//						Transmit_Message(RADIO_OVER_OUT);
//					}

				}
				break;

			/*
			**	Wait while the door closes.
			*/
			case CLOSE:
				if (Is_Door_Closed()) {
					Enter_Idle_Mode();
				}
				break;

			default:
				break;
		}
		return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
	}

	Assign_Mission(MISSION_GUARD);
	return(1);
}


/***********************************************************************************************
 * BuildingClass::Power_Output -- Fetches the current power output from this building.         *
 *                                                                                             *
 *    This routine will return the current power output for this building. The power output    *
 *    is adjusted according to the damage level of the building.                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the current power output for this building.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Power_Output(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (Class->Power) {
		return(Class->Power * fixed(LastStrength, Class->MaxStrength));
	}
	return(0);
}


/***********************************************************************************************
 * BuildingClass::Detach -- Handles target removal from the game system.                       *
 *                                                                                             *
 *    This routine is called when the specified target is about to be removed from the game    *
 *    system.                                                                                  *
 *                                                                                             *
 * INPUT:   target   -- The target to be removed from this building's targeting computer.      *
 *                                                                                             *
 *          all      -- Is the target about to be completely eliminated?                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Detach(TARGET target, bool all)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	TechnoClass::Detach(target, all);
	if (target == WhomToRepay) {
		WhomToRepay = TARGET_NONE;
	}
	if (target == AnimToTrack) {
		AnimToTrack = TARGET_NONE;
	}
}


/***********************************************************************************************
 * BuildingClass::Crew_Type -- This determines the crew that this object generates.            *
 *                                                                                             *
 *    When selling very cheap buildings (such as the silo), a technician will pop out since    *
 *    generating minigunners would be overkill -- the player could use this loophole to        *
 *    gain an advantage.                                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the infantry type that this building will generate as a survivor.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryType BuildingClass::Crew_Type(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	switch (Class->Type) {
		case STRUCT_STORAGE:
			if (Percent_Chance(50)) {
				return(INFANTRY_C1);
			} else {
				return(INFANTRY_C7);
			}

		case STRUCT_CONST:
			if (!IsCaptured && House->IsHuman && Percent_Chance(25)) {
				return(INFANTRY_RENOVATOR);
			}
			break;

		case STRUCT_KENNEL:
			if (Percent_Chance(50)) {
				return(INFANTRY_DOG);
			} else {
				return(INFANTRY_NONE);
			}

		case STRUCT_TENT:
		case STRUCT_BARRACKS:
			return(INFANTRY_E1);

		default:
			break;
	}
	return(TechnoClass::Crew_Type());
}


/***********************************************************************************************
 * BuildingClass::Detach_All -- Possibly abandons production according to factory type.        *
 *                                                                                             *
 *    When this routine is called, it indicates that the building is about to be destroyed     *
 *    or captured. In such a case any production it may be doing, must be abandoned.           *
 *                                                                                             *
 * INPUT:   all   -- Is the object about the be completely destroyed?                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Detach_All(bool all)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	/*
	**	If it is producing something, then it must be abandoned.
	*/
	if (Factory) {
		Factory->Abandon();
		delete (FactoryClass *)Factory;
		Factory = 0;
	}

	/*
	** If the owner HouseClass is building something, and this building can
	** build that thing, we may be the last building for that house that can
	** build that thing; if so, abandon production of it.
	*/
	if (House) {
		FactoryClass * factory = House->Fetch_Factory(Class->ToBuild);

		/*
		**	If a factory was found, then temporarily disable this building and then
		**	determine if any object that is being produced can still be produced. If
		**	not, then the object being produced must be abandoned.
		*/
		if (factory) {
			TechnoClass * object = factory->Get_Object();
			IsInLimbo = true;
			if (object && !object->Techno_Type_Class()->Who_Can_Build_Me(true, false, House->Class->House)) {
				House->Abandon_Production(Class->ToBuild);
			}
			IsInLimbo = false;
		}
	}

	TechnoClass::Detach_All(all);
}


/***********************************************************************************************
 * BuildingClass::Flush_For_Placement -- Handles clearing a zone for object placement.         *
 *                                                                                             *
 *    This routine is used to clear the way for placement of the specified object (usually     *
 *    a building). If there are friendly units blocking the placement area, they are told      *
 *    to scatter. Enemy blocking units are attacked.                                           *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that is desired to be placed.                    *
 *                                                                                             *
 *          cell     -- The cell that placement wants to occur at.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1995 JLB : Created.                                                                 *
 *   09/27/1995 JLB : Revised to use type class function.                                      *
 *=============================================================================================*/
bool BuildingClass::Flush_For_Placement(TechnoClass * techno, CELL cell)
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (techno) {
		return (((BuildingTypeClass const &)techno->Class_Of()).Flush_For_Placement(cell, House));
	}
	return(false);
}


/***********************************************************************************************
 * BuildingClass::Find_Exit_Cell -- Find a clear location to exit an object from this building *
 *                                                                                             *
 *    This routine is called when the building needs to discharge a unit. It will find a       *
 *    nearby (adjacent) cell that is clear enough for the specified object to enter. Typical   *
 *    use of this routine is when the airfield disgorges its cargo.                            *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that wishes to exit this building.               *
 *                                                                                             *
 * OUTPUT:  Returns with the cell number to use for object placement. If no free location      *
 *          could be found, then zero (0) is returned.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *   02/20/1996 JLB : Added default case for exit cell calculation.                            *
 *=============================================================================================*/
CELL BuildingClass::Find_Exit_Cell(TechnoClass const * techno) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	CELL const * ptr;
	CELL origin = Coord_Cell(Coord);

	ptr = Class->ExitList;
	if (ptr != NULL) {
		while (*ptr != REFRESH_EOL) {
			CELL cell = origin + *ptr++;
			if (Map.In_Radar(cell) && techno->Can_Enter_Cell(cell) == MOVE_OK) {
				return(cell);
			}
		}
	} else {
		int x1, x2;
		int y1, y2;
		CELL cell;

		y1 = -1;
		y2 = Class->Height();
		for (x1 = -1; x1 <= Class->Width(); x1++) {
			cell = origin + x1 + (y1 * MAP_CELL_W);
			if (Map.In_Radar(cell) && techno->Can_Enter_Cell(cell) == MOVE_OK) {
				return(cell);
			}
			cell = origin + x1 + (y2 * MAP_CELL_W);
			if (Map.In_Radar(cell) && techno->Can_Enter_Cell(cell) == MOVE_OK) {
				return(cell);
			}
		}

		x1 = -1;
		x2 = Class->Width();
		for (y1 = -1; y1 <= Class->Height(); y1++) {
			cell = origin + (y1 * MAP_CELL_W) + x1;
			if (Map.In_Radar(cell) && techno->Can_Enter_Cell(cell) == MOVE_OK) {
				return(cell);
			}
			cell = origin + (y1 * MAP_CELL_W) + x2;
			if (Map.In_Radar(cell) && techno->Can_Enter_Cell(cell) == MOVE_OK) {
				return(cell);
			}
		}
	}
	return(0);
}


/***********************************************************************************************
 * BuildingClass::Can_Player_Move -- Can this building be moved?                               *
 *                                                                                             *
 *    This routine answers the question 'can this building be moved?' Typically, only the      *
 *    construction yard can be moved and it does this by undeploying back into a MCV.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Can the building move to a new location under player control?                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Can_Player_Move(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	return(*this == STRUCT_CONST && (Mission == MISSION_GUARD) && Special.IsMCVDeploy);
}


/***********************************************************************************************
 * BuildingClass::Exit_Coord -- Determines location where object will leave it.                *
 *                                                                                             *
 *    This routine will return the coordinate where an object that wishes to leave the         *
 *    building will exit at.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate that the object should be created at.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE BuildingClass::Exit_Coord(void) const
{
	assert(Buildings.ID(this) == ID);
	assert(IsActive);

	if (Class->ExitCoordinate) {
		return(Coord_Add(Coord, Class->ExitCoordinate));
	}
	return(TechnoClass::Exit_Coord());
}


/***********************************************************************************************
 * BuildingClass::Check_Point -- Fetches the landing checkpoint for the given flight pattern.  *
 *                                                                                             *
 *    Use this routine to coordinate a landing operation. The specified checkpoint is          *
 *    converted into a cell number. The landing aircraft should fly over that cell and then    *
 *    request the next check point.                                                            *
 *                                                                                             *
 * INPUT:   cp    -- The check point to convert to a cell number.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that the aircraft should fly over in order to complete       *
 *          that portion of the landing pattern.                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL BuildingClass::Check_Point(CheckPointType cp) const
{
	CELL xoffset = 6;		// Downwind offset.
	CELL yoffset = 5;		// Crosswind offset.
	CELL cell = Coord_Cell(Center_Coord());

	switch (cp) {
		case CHECK_STACK:
			xoffset = 0;
			break;

		case CHECK_CROSSWIND:
			yoffset = 0;
			break;

		case CHECK_DOWNWIND:
		default:
			break;
	}

	if ((Cell_X(cell) - Map.MapCellX) > Map.MapCellWidth/2)  {
		xoffset = -xoffset;
	}

	if ((Cell_Y(cell) - Map.MapCellY) > Map.MapCellHeight/2)  {
		yoffset = -yoffset;
	}

	return(XY_Cell(Cell_X(cell)+xoffset, Cell_Y(cell)+yoffset));
}


/***********************************************************************************************
 * BuildingClass::Update_Radar_Spied - set house's RadarSpied field appropriately.				  *
 *                                                                                             *
 *    This routine is called when a radar facility is captured or destroyed.  It fills in the  *
 *    RadarSpied field of the house based on whether there's a spied-upon radar facility or not*
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  House->RadarSpied field gets set appropriately.												  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/22/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Update_Radar_Spied(void)
{
	House->RadarSpied = 0;
	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * obj = Buildings.Ptr(index);
		if (obj && !obj->IsInLimbo && obj->House == House) {
			if (*obj == STRUCT_RADAR /* || *obj == STRUCT_EYE */) {
				House->RadarSpied |= obj->Spied_By();
			}
		}
	}
	Map.RadarClass::Flag_To_Redraw(true);
}


/***********************************************************************************************
 * BuildingClass::Read_INI -- Reads buildings from INI file.                                   *
 *                                                                                             *
 *    This is the basic scenario initialization of building function. It                       *
 *    is called when reading the scenario startup INI file and it handles                      *
 *    creation of all specified buildings.                                                     *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Housename, Typename, Strength, Cell, Facing, Triggername                               *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the loaded INI file data.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Read_INI(CCINIClass & ini)
{
	BuildingClass			* b;			// Working unit pointer.
	HousesType				bhouse;		// Building house.
	StructType				classid;		// Building type.
	CELL						cell;			// Cell of building.
	char						buf[128];
	char						* trigname;	// building's trigger's name


	int len = ini.Entry_Count(INI_Name());
	for (int index = 0; index < len; index++) {
		char const * entry = ini.Get_Entry(INI_Name(), index);

		/*
		**	Get a building entry.
		*/
		ini.Get_String(INI_Name(), entry, NULL, buf, sizeof(buf));

		/*
		**	1st token: house name.
		*/
		bhouse = HouseTypeClass::From_Name(strtok(buf, ","));

		/*
		**	2nd token: building name.
		*/
		classid = BuildingTypeClass::From_Name(strtok(NULL, ","));

		if (bhouse != HOUSE_NONE && classid != STRUCT_NONE) {
			int	strength;
			DirType facing;

			/*
			**	3rd token: strength.
			*/
			strength = atoi(strtok(NULL, ","));

			/*
			**	4th token: cell #.
			*/
			cell = atoi(strtok(NULL, ","));

			/*
			**	5th token: facing.
			*/
			facing = (DirType)atoi(strtok(NULL, ","));

			/*
			**	6th token: triggername (can be NULL).
			*/
			trigname = strtok(NULL, ",");

			bool sellable = false;
			char * token_pointer = strtok(NULL, ",");
			if (token_pointer) {
				sellable = atoi(token_pointer);
			}

			bool rebuild = false;
			token_pointer = strtok(NULL, ",");
			if (token_pointer) {
				rebuild = atoi(token_pointer);
			}

			if (HouseClass::As_Pointer(bhouse) != NULL) {
				b = new BuildingClass(classid, bhouse);
				if (b) {

					TriggerTypeClass * tp = TriggerTypeClass::From_Name(trigname);
					if (tp) {
						TriggerClass * tt = Find_Or_Make(tp);
						if (tt) {
							tt->AttachCount++;
							b->Trigger = tt;
						}
					}
					b->IsAllowedToSell = sellable;
					b->IsToRebuild = rebuild;
					b->IsToRepair = rebuild || *b == STRUCT_CONST;

					if (b->Unlimbo(Cell_Coord(cell), facing)) {
						strength = min(strength, 0x100);
						strength = (int)b->Class->MaxStrength * fixed(strength, 256);		// Cast to (int). ST - 5/8/2019
						b->Strength = strength;
						if (b->Strength > b->Class->MaxStrength-3) b->Strength = b->Class->MaxStrength;
						b->IsALemon = false;
					} else {

						/*
						**	If the building could not be unlimboed on the map, then this indicates
						**	a serious error. Delete the building.
						*/
						delete b;
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Write_INI -- Write out the building data to the INI file specified.          *
 *                                                                                             *
 *    This will store the building data (as it relates to scenario initialization) to the      *
 *    INI database specified.                                                                  *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that the building data will be stored to.   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Write_INI(CCINIClass & ini)
{
	/*
	**	First, clear out all existing building data from the ini file.
	*/
	ini.Clear(INI_Name());

	/*
	**	Write the data out.
	*/
	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * building = Buildings.Ptr(index);
		if (!building->IsInLimbo) {
			char	uname[10];
			char	buf[127];

			sprintf(uname, "%d", index);
			sprintf(buf, "%s,%s,%d,%u,%d,%s,%d,%d",
				building->House->Class->IniName,
				building->Class->IniName,
				building->Health_Ratio()*256,
				Coord_Cell(building->Coord),
				building->PrimaryFacing.Current(),
				building->Trigger.Is_Valid() ? building->Trigger->Class->IniName : "None",
				building->IsAllowedToSell,
				building->IsToRebuild
				);
			ini.Put_String(INI_Name(), uname, buf);
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Target_Coord -- Return the coordinate to use when firing on this building.   *
 *                                                                                             *
 *    This routine will determine the "center" location of this building for purposes of       *
 *    targeting. Usually, this location is somewhere near the foundation of the building.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to use when firing upon this building (or trying to    *
 *          walk onto it).                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE BuildingClass::Target_Coord(void) const
{
	COORDINATE coord = Center_Coord();

	if (Class->FoundationFace != FACING_NONE) {
		return(Adjacent_Cell(coord, Class->FoundationFace));
	}
	return(coord);
}


/***********************************************************************************************
 * BuildingClass::Factory_AI -- Handle factory production and initiation.                      *
 *                                                                                             *
 *    Some building (notably the computer controlled ones) can have a factory object attached. *
 *    This routine handles processing of that factory and also detecting when production       *
 *    should begin in order to initiate production.                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per building per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Factory_AI(void)
{
	/*
	**	Handle any production tied to this building. Only computer controlled buildings have
	**	production attached to the building itself. The player uses the sidebar interface for
	**	all production control.
	*/
	if (Factory.Is_Valid() && Factory->Has_Completed() && PlacementDelay == 0) {
		TechnoClass * product = Factory->Get_Object();
//		FactoryClass * fact = Factory;

		switch (Exit_Object(product)) {

			/*
			**	If the object could not leave the factory, then either request
			**	a transport, place the (what must be a) building using another method, or
			**	abort the production and refund money.
			*/
			case 0:
				Factory->Abandon();
				delete (FactoryClass *)Factory;
				Factory = 0;
				break;

			/*
			**	Exiting this building is prevented by some temporary blockage. Wait
			**	a bit before trying again.
			*/
			case 1:
				PlacementDelay = TICKS_PER_SECOND*3;
				break;

			/*
			**	The object was successfully sent from this factory. Inform the house
			**	tracking logic that the requested object has been produced.
			*/
			case 2:
				switch (product->What_Am_I()) {
					case RTTI_VESSEL:
						House->JustBuiltVessel = ((VesselClass*)product)->Class->Type;
						House->IsBuiltSomething = true;
						break;

					case RTTI_UNIT:
						House->JustBuiltUnit = ((UnitClass*)product)->Class->Type;
						House->IsBuiltSomething = true;
						break;

					case RTTI_INFANTRY:
						House->JustBuiltInfantry = ((InfantryClass*)product)->Class->Type;
						House->IsBuiltSomething = true;
						break;

					case RTTI_BUILDING:
						House->JustBuiltStructure = ((BuildingClass*)product)->Class->Type;
						House->IsBuiltSomething = true;
						break;

					case RTTI_AIRCRAFT:
						House->JustBuiltAircraft = ((AircraftClass*)product)->Class->Type;
						House->IsBuiltSomething = true;
						break;

					default:
						break;
				}
//				fact->Completed();
				Factory->Completed();
//				delete fact;
				delete (FactoryClass *)Factory;
				Factory = 0;
				break;

			default:
				break;
		}
	}

	/*
	**	Pick something to create for this factory.
	*/
	if (House->IsStarted && Mission != MISSION_CONSTRUCTION && Mission != MISSION_DECONSTRUCTION) {

		/*
		**	Buildings that produce other objects have special factory logic handled here.
		*/
		if (Class->ToBuild != RTTI_NONE) {
			if (Factory.Is_Valid()) {

				/*
				**	If production has halted, then just abort production and make the
				**	funds available for something else.
				*/
				if (PlacementDelay == 0 && !Factory->Is_Building()) {
					Factory->Abandon();
					delete (FactoryClass *)Factory;
					Factory = 0;
				}

			} else {

				/*
				**	Only look to start production if there is at least a small amount of
				**	money available. In cases where there is no practical money left, then
				**	production can never complete -- don't bother starting it.
				*/
				if (House->IsStarted && House->Available_Money() > 10) {
					TechnoTypeClass const * techno = House->Suggest_New_Object(Class->ToBuild, *this == STRUCT_KENNEL);

					/*
					**	If a suitable object type was selected for production, then start
					**	producing it now.
					*/
					if (techno != NULL) {
						Factory = new FactoryClass;
						if (Factory.Is_Valid()) {
							if (!Factory->Set(*techno, *House)) {
								delete (FactoryClass *)Factory;
								Factory = 0;
							} else {
								House->Production_Begun(Factory->Get_Object());
								Factory->Start();
							}
						}
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Rotation_AI -- Process any turret rotation required of this building.        *
 *                                                                                             *
 *    Some buildings have a turret and this routine handles processing the turret rotation.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per building per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *   10/27/1996 JLB : Rotation does not occur if power and no power avail.                     *
 *=============================================================================================*/
void BuildingClass::Rotation_AI(void)
{
	if (Class->IsTurretEquipped &&
			Mission != MISSION_CONSTRUCTION &&
			Mission != MISSION_DECONSTRUCTION &&
			(!Class->IsPowered || House->Power_Fraction() >= 1)) {

		/*
		**	Rotate turret to match desired facing.
		*/
		if (PrimaryFacing.Is_Rotating()) {
			if (PrimaryFacing.Rotation_Adjust(Class->ROT)) {
				Mark(MARK_CHANGE);
			}
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Charging_AI -- Handles the special charging logic for Tesla coils.           *
 *                                                                                             *
 *    This handles the special logic required of the charging tesla coil. It requires special  *
 *    processing since its charge up is dependant upon the target and power surplus of the     *
 *    owning house.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Charging_AI(void)
{
	if (Class->PrimaryWeapon != NULL && Class->PrimaryWeapon->IsElectric && BState != BSTATE_CONSTRUCTION) {
		if (Target_Legal(TarCom) && House->Power_Fraction() >= 1) {
			if (!IsCharged) {
				if (IsCharging) {
//					if (stagechange) {
						Mark(MARK_CHANGE);
						if (Fetch_Stage() >= 9) {
							IsCharged = true;
							IsCharging = false;
							Set_Rate(0);
						}
//					}
				} else if (!Arm) {
					IsCharged = false;
					IsCharging = true;
					Set_Stage(0);
					Set_Rate(3);
					Sound_Effect(VOC_TESLA_POWER_UP, Coord);
				}
			}
		} else {
			if (IsCharging || IsCharged) {
				Mark(MARK_CHANGE);
				IsCharging = false;
				IsCharged = false;
				Set_Stage(0);
				Set_Rate(0);
			}
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Repair_AI -- Handle the repair (and sell) logic for the building.            *
 *                                                                                             *
 *    This routine handle the repair animation and healing logic. It also detects when the     *
 *    (computer controlled) building should begin repair or sell itself.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per building per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Repair_AI(void)
{
	if (House->IQ >= Rule.IQRepairSell && Mission != MISSION_CONSTRUCTION && Mission != MISSION_DECONSTRUCTION) {
		/*
		**	Possibly start repair process if the building is below half strength.
		*/
//		unsigned ratio = MIN(House->Smartness, 0x00F0);
		if (Can_Repair()) {
			if (House->Available_Money() >= Rule.RepairThreshhold) {
				if (!House->DidRepair) {
					if (!IsRepairing && (IsCaptured || IsToRepair || House->IsHuman || Session.Type != GAME_NORMAL)) {
						House->DidRepair = true;	// flag that this house did its repair allocation for this frame
						Repair(1);

						if (!House->IsHuman) {
							House->RepairTimer = Random_Pick((int)(House->RepairDelay * (TICKS_PER_MINUTE/4)), (int)(House->RepairDelay * TICKS_PER_MINUTE * 2));
						}
					}
				}
			} else {
				if ((Session.Type != GAME_NORMAL || IsAllowedToSell) && IsTickedOff && House->Control.TechLevel >= Rule.IQSellBack && Random_Pick(0, 50) < House->Control.TechLevel && !Trigger.Is_Valid() && *this != STRUCT_CONST && Health_Ratio() < Rule.ConditionRed) {
					Sell_Back(1);
				}
			}
		}
	}

	/*
	**	If it is repairing, then apply any repair effects as necessary.
	*/
	if (IsRepairing && (Frame % (Rule.RepairRate * TICKS_PER_MINUTE)) == 0) {
		IsWrenchVisible = (IsWrenchVisible == false);
		Mark(MARK_CHANGE);
		int cost = Class->Repair_Cost();
		int step = Class->Repair_Step();

		/*
		**	Check for and expend any necessary monies to continue the repair.
		*/
		if (House->Available_Money() >= cost) {
			House->Spend_Money(cost);
			Strength += step;

			if (Strength >= Class->MaxStrength) {
				Strength = Class->MaxStrength;
				IsRepairing = false;
			}
		} else {
			IsRepairing = false;
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Animation_AI -- Handles normal building animation processing.                *
 *                                                                                             *
 *    This will process the general building animation mechanism. It detects when the          *
 *    building animation sequence has completed and flags the building to perform mission      *
 *    changes as a result.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Call this routine only once per building per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Animation_AI(void)
{
	bool stagechange = Graphic_Logic();
	bool toloop = false;

	/*
	**	Always refresh the SAM site if it has an animation change.
	*/
	if (*this == STRUCT_SAM && stagechange) Mark(MARK_CHANGE);

	if ((!Class->IsTurretEquipped && *this != STRUCT_TESLA) || Mission == MISSION_CONSTRUCTION || Mission == MISSION_DECONSTRUCTION) {
		if (stagechange) {

			/*
			**	Check for animation end or if special case of MCV deconstructing when it is allowed
			**	to convert back into an MCV.
			*/
			BuildingTypeClass::AnimControlType const * ctrl = Fetch_Anim_Control();

			/*
			**	When the last frame of the current animation sequence is reached, flag that
			**	a new mission may be started. This must occur before the animation actually
			**	loops so that if a mission change does occur, it will have a chance to change
			**	the building graphic before the last frame is replaced by the first frame of
			**	the loop.
			*/
			if (Fetch_Stage() == ctrl->Start+ctrl->Count-1  || (!Target_Legal(ArchiveTarget) /*Is_MCV_Deploy()*/ && *this == STRUCT_CONST && Mission == MISSION_DECONSTRUCTION && Fetch_Stage() == (42-19))) {
				IsReadyToCommence = true;
			}

			/*
			**	If the animation advances beyond the last frame, then start the animation
			**	sequence over from the beginning.
			*/
			if (Fetch_Stage() >= ctrl->Start+ctrl->Count) {
				toloop = true;
			}
			Mark(MARK_CHANGE);
		} else {
			if (BState == BSTATE_NONE || Fetch_Rate() == 0) {
				IsReadyToCommence = true;
			}
		}
	}

	/*
	**	If there is a door that is animating, then it might cause this building
	**	to be redrawn. Check for and flag to redraw as necessary.
	*/
	if (Time_To_Redraw()) {
		Clear_Redraw_Flag();
		Mark(MARK_CHANGE);
	}

	/*
	**	The animation sequence has looped. Restart it and flag this loop condition.
	**	This is used to tell the mission system that the animation has completed. It
	**	also signals that now is a good time to act on any pending mission.
	*/
	if (toloop) {
		BuildingTypeClass::AnimControlType const * ctrl = Fetch_Anim_Control();
		if (BState == BSTATE_CONSTRUCTION || BState == BSTATE_IDLE) {
			Set_Rate(Options.Normalize_Delay(ctrl->Rate));
		} else {
			Set_Rate(ctrl->Rate);
		}
		Set_Stage(ctrl->Start);
		Mark(MARK_CHANGE);
	}
}


/***********************************************************************************************
 * BuildingClass::How_Many_Survivors -- This determine the maximum number of survivors.        *
 *                                                                                             *
 *    This routine is called to determine how many survivors should run from this building     *
 *    when it is either sold or destroyed. Buildings that are captured have fewer survivors.   *
 *    The number of survivors is a portion of the cost of the building divided by the cost     *
 *    of a minigunner.                                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of soldiers that should run from this building.            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::How_Many_Survivors(void) const
{
	if (IsSurvivorless || !Class->IsCrew) return(0);

	int divisor = InfantryTypeClass::As_Reference(INFANTRY_E1).Raw_Cost();
	if (divisor == 0) return(0);
	if (IsCaptured) divisor *= 2;
	int count = (Class->Raw_Cost() * Rule.SurvivorFraction) / divisor;
	return(Bound(count, 1, 5));
}


/***********************************************************************************************
 * BuildingClass::Get_Image_Data -- Fetch the image pointer for the building.                  *
 *                                                                                             *
 *    This routine will return with a pointer to the shape data for the building. The shape    *
 *    data is different than normal when the building is undergoing construction and           *
 *    disassembly.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the shape data for this building.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * BuildingClass::Get_Image_Data(void) const
{
	if (BState == BSTATE_CONSTRUCTION) {
		return(Class->Get_Buildup_Data());
	}
	return(TechnoClass::Get_Image_Data());
}


/***********************************************************************************************
 * BuildingClass::Value -- Determine the value of this building.                               *
 *                                                                                             *
 *    The value of the building is normally just its ordinary assigned value. However, in the  *
 *    case of fakes, the value is artificially enhanced to match the structure that is         *
 *    being faked.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the point value of the building type.                                 *
 *                                                                                             *
 * WARNINGS:   The point value returned should not be used for scoring, only for target        *
 *             scanning.                                                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Value(void) const
{
	if (Class->IsFake) {
		switch (Class->Type) {
			case STRUCT_FAKEWEAP:
				return(BuildingTypeClass::As_Reference(STRUCT_WEAP).Reward + BuildingTypeClass::As_Reference(STRUCT_WEAP).Risk);

			case STRUCT_FAKECONST:
				return(BuildingTypeClass::As_Reference(STRUCT_CONST).Reward + BuildingTypeClass::As_Reference(STRUCT_CONST).Risk);

			case STRUCT_FAKE_YARD:
				return(BuildingTypeClass::As_Reference(STRUCT_SHIP_YARD).Reward + BuildingTypeClass::As_Reference(STRUCT_SHIP_YARD).Risk);

			case STRUCT_FAKE_PEN:
				return(BuildingTypeClass::As_Reference(STRUCT_SUB_PEN).Reward + BuildingTypeClass::As_Reference(STRUCT_SUB_PEN).Risk);

			case STRUCT_FAKE_RADAR:
				return(BuildingTypeClass::As_Reference(STRUCT_RADAR).Reward + BuildingTypeClass::As_Reference(STRUCT_RADAR).Risk);

			default:
				break;
		}
	}
	return(TechnoClass::Value());
}


/***********************************************************************************************
 * BuildingClass::Remove_Gap_Effect -- Stop a gap generator from jamming cells.					  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none																										  *
 *                                                                                             *
 * WARNINGS:   																										  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Remove_Gap_Effect(void)
{
	// unjam this one's field...
	Map.UnJam_From(Coord_Cell(Center_Coord()), Rule.GapShroudRadius, House);
	
	
	/*
	** Updated for client/server multiplayer. ST - 8/12/2019 11:14AM
	*/
	if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {
		if (!House->IsPlayerControl && PlayerPtr->IsGPSActive) {
			Map.Sight_From(Coord_Cell(Center_Coord()), Rule.GapShroudRadius, PlayerPtr);
		}
	
	} else {
		
		for (int i = 0; i < Session.Players.Count(); i++) {
			HouseClass *player_house = HouseClass::As_Pointer(Session.Players[i]->Player.ID);
			if (player_house->IsGPSActive && player_house != House) {
				Map.Sight_From(Coord_Cell(Center_Coord()), Rule.GapShroudRadius, player_house);
			}
		}
	}


	// and rejam any overlapping buildings' fields
	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass *obj = Buildings.Ptr(index);
		if (obj && !obj->IsInLimbo && obj->House == House && *obj == STRUCT_GAP && obj!=this) {
			obj->IsJamming = false;
			obj->Arm = 0;
//			Map.Jam_From(Coord_Cell(obj->Center_Coord()), Rule.GapShroudRadius, PlayerPtr);
		}
	}
}


short const * BuildingClass::Overlap_List(bool redraw) const
{
	if ((Spied_By() & (1 << PlayerPtr->Class->House)) != 0 && Is_Selected_By_Player()) {
		if (*this == STRUCT_BARRACKS || *this == STRUCT_TENT) {
			static short const _list[] = {
				-1, 2, (MAP_CELL_W*1)-1, (MAP_CELL_W*1)+2, REFRESH_EOL
			};
			return(_list);
		} else if (*this == STRUCT_REFINERY) {
			static short const _list[] = {
				0, 2, (MAP_CELL_W*2)+0, (MAP_CELL_W*2)+1, (MAP_CELL_W*2)+2, REFRESH_EOL
			};
			return(_list);
		}
	}
	return(TechnoClass::Overlap_List(redraw));
}


unsigned BuildingClass::Spied_By() const
{
	unsigned spiedby = TechnoClass::Spied_By();

	/*
	** If it's an ore refinery or other such storage-capable building,
	** loop thru all of their buildings to see if ANY of them are spied
	** upon, 'cause once you spy any money, you've spied all of it.
	*/
	if (Class->Capacity) {
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * building = Buildings.Ptr(index);
			if (building->House == House && building->Class->Capacity) {
				spiedby |= building->SpiedBy;
			}
		}
	}

	return spiedby;
}