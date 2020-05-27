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

/* $Header: /CounterStrike/TACTION.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : ACTION.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/28/95                                                     *
 *                                                                                             *
 *                  Last Update : July 17, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Action_From_Name -- retrieves ActionType for given name                                   *
 *   Action_Needs -- Figures out what data an action object needs.                             *
 *   Name_From_Action -- retrieves name for ActionType                                         *
 *   TActionClass::Build_INI_Entry -- Builds an INI entry for this trigger action.             *
 *   TActionClass::Code_Pointers -- Convert embedded pointers into a transportable format.     *
 *   TActionClass::Decode_Pointers -- Converts coded pointers into usable format.              *
 *   TActionClass::Detach -- Removes any attachment from associated action.                    *
 *   TActionClass::Read_INI -- Converts INI text into appropriate action data.                 *
 *   TActionClass::operator -- Performs the action that this object does.                      *
 *   ActionChoiceClass::Draw_It -- Display the action choice as part of a list box.            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/*
** New sidebar for GlyphX multiplayer. ST - 8/7/2019 10:10AM
*/
#include "SidebarGlyphx.h"

/*
**	These are the text names for the various actions. If the action name ends with "..." then
**	this means that additional data is probably required.
*/
static const char * ActionText[TACTION_COUNT] = {
	"-No Action-",
	"Winner is...",
	"Loser is...",
	"Production Begins",
	"Create Team...",
	"Destroy All Teams",
	"All to Hunt...",
	"Reinforcement (team)...",
	"Drop Zone Flare (waypoint)...",
	"Fire Sale...",
	"Play Movie...",
	"Text Trigger (ID num)...",
	"Destroy Trigger...",
	"Autocreate Begins...",
	"~don't use~",
	"Allow Win",
	"Reveal all map",
	"Reveal around waypoint...",
	"Reveal zone of waypoint...",
	"Play sound effect...",
	"Play music theme...",
	"Play speech...",
	"Force Trigger...",
	"Timer Start",
	"Timer Stop",
	"Timer Extend (1/10th min)...",
	"Timer Shorten (1/10th min)...",
	"Timer Set (1/10th min)...",
	"Global Set...",
	"Global Clear...",
	"Auto Base Building...",
	"Grow shroud one 'step'",
	"Destroy attached building",
	"Add 1-time special weapon...",
	"Add repeating special weapon...",
	"Preferred target...",
	"Launch Nukes"
};


ActionChoiceClass ActionChoices[TACTION_COUNT] = {
	{TACTION_NONE},
	{TACTION_WIN},
	{TACTION_LOSE},
	{TACTION_BEGIN_PRODUCTION},
	{TACTION_CREATE_TEAM},
	{TACTION_DESTROY_TEAM},
	{TACTION_ALL_HUNT},
	{TACTION_REINFORCEMENTS},
	{TACTION_DZ},
	{TACTION_FIRE_SALE},
	{TACTION_PLAY_MOVIE},
	{TACTION_TEXT_TRIGGER},
	{TACTION_DESTROY_TRIGGER},
	{TACTION_AUTOCREATE},
	{TACTION_WINLOSE},
	{TACTION_ALLOWWIN},
	{TACTION_REVEAL_ALL},
	{TACTION_REVEAL_SOME},
	{TACTION_REVEAL_ZONE},
	{TACTION_PLAY_SOUND},
	{TACTION_PLAY_MUSIC},
	{TACTION_PLAY_SPEECH},
	{TACTION_FORCE_TRIGGER},
	{TACTION_START_TIMER},
	{TACTION_STOP_TIMER},
	{TACTION_ADD_TIMER},
	{TACTION_SUB_TIMER},
	{TACTION_SET_TIMER},
	{TACTION_SET_GLOBAL},
	{TACTION_CLEAR_GLOBAL},
	{TACTION_BASE_BUILDING},
	{TACTION_CREEP_SHADOW},
	{TACTION_DESTROY_OBJECT},
	{TACTION_1_SPECIAL},
	{TACTION_FULL_SPECIAL},
	{TACTION_PREFERRED_TARGET},
	{TACTION_LAUNCH_NUKES}
};


/***********************************************************************************************
 * ActionChoiceClass::Draw_It -- Display the action choice as part of a list box.              *
 *                                                                                             *
 *    This is a support routine only used to display this object when it is part of a list     *
 *    box.                                                                                     *
 *                                                                                             *
 * INPUT:   x,y   -- The coordinate (upper left) to display the description.                   *
 *                                                                                             *
 *          width,height   -- Dimensions of the area to display the description.               *
 *                                                                                             *
 *          selected -- Is this item highlighted?                                              *
 *                                                                                             *
 *          flags -- The text print flags to use.                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void ActionChoiceClass::Draw_It(int , int x, int y, int width, int height, bool selected, TextPrintType flags) const
{
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();
	static int _tabs[] = {13,40};
	if ((flags & 0x0F) == TPF_6PT_GRAD || (flags & 0x0F) == TPF_EFNT) {

		if (selected) {
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect(x, y, x + width - 1, y + height - 1, scheme->Shadow);
		} else {
			if (!(flags & TPF_USE_GRAD_PAL)) {
				flags = flags | TPF_MEDIUM_COLOR;
			}
		}

		Conquer_Clip_Text_Print(Description(), x, y, scheme, TBLACK, flags, width, _tabs);
	} else {
		Conquer_Clip_Text_Print(Description(), x, y, (selected ? &ColorRemaps[PCOLOR_DIALOG_BLUE] : &ColorRemaps[PCOLOR_GREY]), TBLACK, flags, width, _tabs);
	}
}


/***********************************************************************************************
 * TActionClass::Detach -- Removes any attachment from associated action.                      *
 *                                                                                             *
 *    This routine will remove any action reference to the team type specified. This routine   *
 *    is called when the team type is being destroyed. All references to that team type must   *
 *    also be severed. This routine does that with respect to trigger actions.                 *
 *                                                                                             *
 * INPUT:   target-- The target object or type to remove from this taction object.             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TActionClass::Detach(TARGET target)
{
	if (Is_Target_TeamType(target) && Team == As_TeamType(target)) {
		Team = NULL;
	}
	if (Is_Target_TriggerType(target) && Trigger == As_TriggerType(target)) {
		Trigger = NULL;
	}
}


/***********************************************************************************************
 * TActionClass::Build_INI_Entry -- Builds an INI entry for this trigger action.               *
 *                                                                                             *
 *    This routine will build the text (INI entry) format for the data of this trigger         *
 *    action object. Typical use of this is when the INI file is being written.                *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the location to build the INI text to. The buffer is presumed  *
 *                   to be big enough.                                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The buffer passed to this routine must be big enough to hold the largest        *
 *             text that will be created into it.                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TActionClass::Build_INI_Entry(char * ptr) const
{
	sprintf(ptr, "%d,%d,%d,%d", Action, TeamTypes.Logical_ID(Team), TriggerTypes.Logical_ID(Trigger), Data.Value);
}


/***********************************************************************************************
 * TActionClass::Read_INI -- Converts INI text into appropriate action data.                   *
 *                                                                                             *
 *    This routine will convert INI data into the right values within this trigger action      *
 *    object. Typical use of this routine is when the INI file is being read. It is the        *
 *    counterpart to the Build_INI_Entry function.                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TActionClass::Read_INI(void)
{
	switch (NewINIFormat) {
		default:
			Action = TActionType(atoi(strtok(NULL, ",")));
			Team.Set_Raw(atoi(strtok(NULL, ",")));
			Trigger.Set_Raw(atoi(strtok(NULL, ",")));
			Data.Value = atoi(strtok(NULL, ","));
			break;

		case 1:
		case 0:
			Action = TActionType(atoi(strtok(NULL, ",")));

			char const * ptr = strtok(NULL, ",");
			Team = TeamTypeClass::From_Name(ptr);
			assert(Action_Needs(Action) != NEED_TEAM || Team.Is_Valid());

			/*
			**	Since triggers refer to other triggers, only record a copy of the trigger text
			**	name. This will be fixed up later.
			*/
			Trigger.Set_Raw((long)strdup(strtok(NULL, ",")));

			Data.Value = atoi(strtok(NULL, ","));
			break;
	}
}


/***********************************************************************************************
 * TActionClass::Code_Pointers -- Convert embedded pointers into a transportable format.       *
 *                                                                                             *
 *    This routine is called prior to saving the game. It will convert any pointers into a     *
 *    format that is safe for persistent storage.                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine makes the object unfit for use. The Decode_Pointers() routine      *
 *             must be called prior to using this object.                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TActionClass::Code_Pointers(void)
{
}


/***********************************************************************************************
 * TActionClass::Decode_Pointers -- Converts coded pointers into usable format.                *
 *                                                                                             *
 *    This routine is called after a game has been loaded. The encoded pointers will be        *
 *    converted back into usable format by this routine.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine will NOT work unless the pointer are, in fact, coded. There is     *
 *             no prevention check to protect against calling this routine twice.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TActionClass::Decode_Pointers(void)
{
}


/***********************************************************************************************
 * TActionClass::operator -- Performs the action that this object does.                        *
 *                                                                                             *
 *    This routine is called when the action associated with this action object must be        *
 *    performed. Typically, this occurs when a trigger has "sprung" and now it must take       *
 *    effect. The action object is what carries out this effect.                               *
 *                                                                                             *
 * INPUT:   house -- The owner of this action. This information is necessary since some        *
 *                   actions depend on who the trigger was owned by.                           *
 *                                                                                             *
 *          object-- Pointer to the object that the springing trigger was attached to. If      *
 *                   this parameter is null, then the trigger wasn't attached to any object.   *
 *                                                                                             *
 *          id    -- Trigger ID (only if forced) otherwise -1.                                 *
 *                                                                                             *
 *          cell  -- The cell this trigger is attached to (if any).                            *
 *                                                                                             *
 * OUTPUT:  bool; Was this action able to perform what it needed to do? Failure could be       *
 *                because a reinforcement couldn't be generated, for example.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/22/1996 JLB : Created.                                                                 *
 *   04/10/1996 JLB : Added the ID parameter.                                                  *
 *=============================================================================================*/
bool TActionClass::operator() (HousesType house, ObjectClass * object, int id, CELL cell)
{
	/*
	**	Otherwise, take an appropriate action.
	*/
	HouseClass * hptr = HouseClass::As_Pointer(house);
	TriggerClass * trig = NULL;
	if (id != -1) {
		trig = Triggers.Raw_Ptr(id);
	}
	bool success = true;
//	TeamTypeClass * ttype = Team;

	/*
	**	Ensure that the specified object is not actually dead. A dead object could
	**	be passed to this routine in the case of a multiple event trigger that
	**	had the first event kill the object.
	*/
	if (object && !object->IsActive) {
		object = 0;
	}

	switch (Action) {
		/*
		**	Display a text message overlayed onto the tactical map.
		*/
		case TACTION_TEXT_TRIGGER:
			Session.Messages.Add_Message(NULL, Data.Value, (char *)TutorialText[Data.Value], PCOLOR_GREEN, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE);
			break;

		/*
		** Launch nuclear missiles (duds) from all mslo's
		*/
		case TACTION_LAUNCH_NUKES:
			{
				for(int index = 0; index < Buildings.Count(); index++) {
					BuildingClass *bldg = Buildings.Ptr(index);
					if (*bldg == STRUCT_MSLO) {
						bldg->Assign_Mission(MISSION_MISSILE);
					}
				}
				break;
			}

		/*
		**	Set the preferred target for the house.
		*/
		case TACTION_PREFERRED_TARGET:
			if (hptr) {
				hptr->PreferredTarget = Data.Quarry;
			}
			break;

		/*
		**	Initiate (or disable) the computer AI. When active, the computer will
		**	build bases and units.
		*/
		case TACTION_BASE_BUILDING:
			if (Data.Bool) {
				hptr->IsBaseBuilding = true;
			} else {
				hptr->IsBaseBuilding = false;
			}
			break;

		/*
		**	Cause the shadow to creep back one step.
		*/
		case TACTION_CREEP_SHADOW:
			Map.Encroach_Shadow(PlayerPtr);
			break;

		/*
		**	Set a scenario global.
		*/
		case TACTION_SET_GLOBAL:
			Scen.Set_Global_To(Data.Value, true);
			break;

		/*
		**	Clear a scenario global.
		*/
		case TACTION_CLEAR_GLOBAL:
			Scen.Set_Global_To(Data.Value, false);
			break;

		/*
		**	Reveal the map around the area specified.
		*/
		case TACTION_REVEAL_SOME:
			if (!PlayerPtr->IsVisionary) {
				Map.Sight_From(Scen.Waypoint[Data.Value], Rule.GapShroudRadius, PlayerPtr, false);
			}
			break;

		/*
		**	Reveal all cells of the zone that the specified waypoint is located
		**	in. This can be used to reveal whole islands or bodies of water
		*/
		case TACTION_REVEAL_ZONE:
			if (!PlayerPtr->IsVisionary) {
				int zone = Map[Scen.Waypoint[Data.Value]].Zones[MZONE_CRUSHER];

				for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
					if (Map[cell].Zones[MZONE_CRUSHER] == zone) {
						Map.Map_Cell(cell, PlayerPtr);
					}
				}

			}
			break;

		/*
		**	Reveal the entire map.
		*/
		case TACTION_REVEAL_ALL:
			if (!PlayerPtr->IsVisionary) {
				PlayerPtr->IsVisionary = true;
				for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
					Map.Map_Cell(cell, PlayerPtr);
				}
			}
			break;

		/*
		**	Star the mission timer.
		*/
		case TACTION_START_TIMER:
			if (!Scen.MissionTimer.Is_Active()) {
				Scen.MissionTimer.Start();
				Map.Redraw_Tab();
			}
			break;

		/*
		**	Stop the mission timer. This will really just
		**	suspend the timer.
		*/
		case TACTION_STOP_TIMER:
			if (Scen.MissionTimer.Is_Active()) {
				Scen.MissionTimer.Stop();
				Map.Redraw_Tab();
			}
			break;

		/*
		**	Add time to the mission timer.
		*/
		case TACTION_ADD_TIMER:
			Scen.MissionTimer = Scen.MissionTimer + (Data.Value * (TICKS_PER_MINUTE/10));
			Map.Redraw_Tab();
			break;

		/*
		**	Remove time from the mission timer.
		*/
		case TACTION_SUB_TIMER:
			if ((int)Scen.MissionTimer <= Data.Value * (TICKS_PER_MINUTE/10)) {
				Scen.MissionTimer = 0;
			} else {
				Scen.MissionTimer = Scen.MissionTimer - (Data.Value * (TICKS_PER_MINUTE/10));
			}
			Map.Redraw_Tab();
			break;

		/*
		**	Set the mission timer to the value specified.
		*/
		case TACTION_SET_TIMER:
			Scen.MissionTimer = Data.Value * (TICKS_PER_MINUTE/10);
			Scen.MissionTimer.Start();
			Map.Redraw_Tab();
			break;

		/*
		**	Play a movie immediately. The game is temporarily
		**	suspended while the movie plays.
		*/
		case TACTION_PLAY_MOVIE:
			Hide_Mouse();
			SeenPage.Clear();
			Play_Movie(VQType(char(Data.Movie)), THEME_NONE, true, true);
			GamePalette.Set();
			Map.Flag_To_Redraw(true);
			Show_Mouse();
			break;

		/*
		**	Play a sound effect.
		*/
		case TACTION_PLAY_SOUND:
			Sound_Effect(Data.Sound);
			break;

		/*
		**	Play a musical theme.
		*/
		case TACTION_PLAY_MUSIC:
			Theme.Queue_Song(Data.Theme);
			break;

		/*
		**	Play the speech data specified.
		*/
		case TACTION_PLAY_SPEECH:
			Speak(Data.Speech);
			break;

		/*
		**	Give the special weapon to the house.
		*/
		case TACTION_1_SPECIAL:
		case TACTION_FULL_SPECIAL:
			hptr->SuperWeapon[Data.Special].Enable(Action==TACTION_1_SPECIAL, false);
//			hptr->SuperWeapon[Data.Special].Forced_Charge(PlayerPtr == hptr);

			// Add to Glyphx multiplayer sidebar. ST - 8/7/2019 10:13AM
			if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
				if (hptr->IsHuman) {
					Sidebar_Glyphx_Add(RTTI_SPECIAL, Data.Special, hptr);
				}
			}
			else {
				if (PlayerPtr == hptr) {
					Map.Add(RTTI_SPECIAL, Data.Special);
					Map.Column[1].Flag_To_Redraw();
				}
			}
			break;

		/*
		**	Destroying a trigger means that all triggers of that type will be destroyed.
		*/
		case TACTION_DESTROY_TRIGGER:
			if (Trigger.Is_Valid()) {
				for (int index = 0; index < Triggers.Count(); index++) {
					if (Triggers.Ptr(index)->Class == Trigger) {
						Detach_This_From_All(Triggers.Ptr(index)->As_Target());
						delete Triggers.Ptr(index);
						index--;
					}
				}
			}
			break;

		/*
		**	A forced trigger will force an existing trigger of that type or
		**	will create a trigger of that type and then force it to be sprung.
		*/
		case TACTION_FORCE_TRIGGER:
			if (Trigger.Is_Valid()) {
				Find_Or_Make(Trigger)->Spring(TEVENT_ANY, 0, 0, true);
			}
			break;

		/*
		**	Place a smoke marker at the waypoint specified.
		*/
		case TACTION_DZ:
			new AnimClass(ANIM_LZ_SMOKE, Cell_Coord(Scen.Waypoint[Data.Value]));
			break;

		/*
		**	Flag the house specified as the winner. Really the house value
		**	is only used to determine if it is the player or the computer.
		*/
		case TACTION_WIN:
			if (Data.House == PlayerPtr->Class->House) {
				PlayerPtr->Flag_To_Win();
			} else {
				PlayerPtr->Flag_To_Lose();
			}
			break;

		/*
		**	Flag the house specified as the loser. The house parameter is only
		**	used to determine if it refers to the player or the computer.
		*/
		case TACTION_LOSE:
			if (Data.House != PlayerPtr->Class->House) {
				PlayerPtr->Flag_To_Win();
			} else {
				PlayerPtr->Flag_To_Lose();
			}
			break;

		/*
		**	This will enable production to begin for the house specified.
		*/
		case TACTION_BEGIN_PRODUCTION:
			if (Data.House != HOUSE_NONE) {
				HouseClass * specified_house = HouseClass::As_Pointer(Data.House);
				specified_house->Begin_Production();
			}
			break;

		/*
		**	Cause all buildings to be sold and all units to go into
		**	hunt mode.
		*/
		case TACTION_FIRE_SALE:
			if (Data.House != HOUSE_NONE) {
				HouseClass * specified_house = HouseClass::As_Pointer(Data.House);
				specified_house->State = STATE_ENDGAME;
			}
			break;

		/*
		**	Begin the team autocreate logic for the house specified.
		*/
		case TACTION_AUTOCREATE:
			if (Data.House != HOUSE_NONE) {
				HouseClass * specified_house = HouseClass::As_Pointer(Data.House);
				specified_house->IsAlerted = true;
			}
			break;

		/*
		**	Manually create the team specified.
		*/
		case TACTION_CREATE_TEAM:
			ScenarioInit++;
			Team->Create_One_Of();
			ScenarioInit--;
			break;

		/*
		**	Destroy all teams of the type specified.
		*/
		case TACTION_DESTROY_TEAM:
			Team->Destroy_All_Of();
			break;

		/*
		**	Create a reinforcement of the team specified.
		*/
		case TACTION_REINFORCEMENTS:
			success = Do_Reinforcements(&*Team);
			break;

		/*
		**	Force all units of the house specified to go into
		**	hunt mode.
		*/
		case TACTION_ALL_HUNT:
			HouseClass::As_Pointer(Data.House)->Do_All_To_Hunt();
			break;

		/*
		**	This will destroy all objects that this trigger is
		**	attached to.
		*/
		case TACTION_DESTROY_OBJECT:
			if (object) {
				int damage = object->Strength;
				object->Take_Damage(damage, 0, WARHEAD_AP, 0, true);
			} else {
				success = false;
			}

			/*
			**	If the trigger is attached to a bridge, then the bridge
			**	gets destroyed regardless of whether the trigger was a
			**	forced or natural spring event.
			*/
			if (cell != 0) {
				Map.Destroy_Bridge_At(cell);
			}

			/*
			**	Loop through and destroy all objects that have this trigger
			**	attached to them.
			*/
			if (trig) {
				for (int u_index = 0; u_index < Units.Count(); u_index++) {
					UnitClass * unit = Units.Ptr(u_index);

					if (unit && unit->Trigger == trig) {
						unit->Trigger = NULL;
						int damage = unit->Strength;
						unit->Take_Damage(damage, 0, WARHEAD_AP, 0, true);
					}
				}

				for (int i_index = 0; i_index < Infantry.Count(); i_index++) {
					InfantryClass * infantry = Infantry.Ptr(i_index);

					if (infantry && infantry->Trigger == trig) {
						infantry->Trigger = NULL;
						int damage = infantry->Strength;
						infantry->Take_Damage(damage, 0, WARHEAD_AP, 0, true);
					}
				}

				for (int a_index = 0; a_index < Aircraft.Count(); a_index++) {
					AircraftClass * aircraft = Aircraft.Ptr(a_index);

					if (aircraft && aircraft->Trigger == trig) {
						aircraft->Trigger = NULL;
						int damage = aircraft->Strength;
						aircraft->Take_Damage(damage, 0, WARHEAD_AP, 0, true);
					}
				}

				for (int b_index = 0; b_index < Buildings.Count(); b_index++) {
					BuildingClass * building = Buildings.Ptr(b_index);

					if (building && building->Trigger == trig) {
						building->Trigger = NULL;
						int damage = building->Strength;
						building->Take_Damage(damage, 0, WARHEAD_AP, 0, true);
					}
				}
			}
			break;

		/*
		**	Do no action at all.
		*/
		case TACTION_NONE:
		default:
			break;
	}
	return(success);
}


/***********************************************************************************************
 * Action_From_Name -- retrieves ActionType for given name                                     *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      name         name to get ActionType for                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      ActionType for given name                                                              *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 BR : Created.                                                                  *
 *=============================================================================================*/
TActionType Action_From_Name (char const * name)
{
	if (name == NULL) {
		return(TACTION_NONE);
	}

	for (TActionType i = TACTION_NONE; i < TACTION_COUNT; i++) {
		if (!stricmp(name, ActionText[i])) {
			return(i);
		}
	}

	return(TACTION_NONE);
}


/***********************************************************************************************
 * Name_From_Action -- retrieves name for ActionType                                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      action      ActionType to get name for                                                 *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      name of ActionType                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 BR : Created.                                                                  *
 *=============================================================================================*/
char const * Name_From_Action(TActionType action)
{
	return(ActionText[action]);
}


/***********************************************************************************************
 * Action_Needs -- Figures out what data an action object needs.                               *
 *                                                                                             *
 *    Use this routine to determine what extra data is needed for the specified action. This   *
 *    data will be prompted for in the scenario editor.                                        *
 *                                                                                             *
 * INPUT:   action   -- The action that is to be queried.                                      *
 *                                                                                             *
 * OUTPUT:  Returns with the data type (enumeration) needed for this action type.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
NeedType Action_Needs(TActionType action)
{
	switch (action) {
		case TACTION_1_SPECIAL:
		case TACTION_FULL_SPECIAL:
			return(NEED_SPECIAL);

		case TACTION_FIRE_SALE:
		case TACTION_WIN:
		case TACTION_LOSE:
		case TACTION_ALL_HUNT:
		case TACTION_BEGIN_PRODUCTION:
		case TACTION_AUTOCREATE:
			return(NEED_HOUSE);

		case TACTION_BASE_BUILDING:
			return(NEED_BOOL);

		case TACTION_CREATE_TEAM:
		case TACTION_DESTROY_TEAM:
		case TACTION_REINFORCEMENTS:
			return(NEED_TEAM);

		case TACTION_FORCE_TRIGGER:
		case TACTION_DESTROY_TRIGGER:
			return(NEED_TRIGGER);

		case TACTION_DZ:
			return(NEED_WAYPOINT);

		case TACTION_REVEAL_SOME:
		case TACTION_REVEAL_ZONE:
			return(NEED_WAYPOINT);

		case TACTION_PLAY_MUSIC:
			return(NEED_THEME);

		case TACTION_PLAY_MOVIE:
			return(NEED_MOVIE);

		case TACTION_PLAY_SOUND:
			return(NEED_SOUND);

		case TACTION_PLAY_SPEECH:
			return(NEED_SPEECH);

		case TACTION_TEXT_TRIGGER:
		case TACTION_ADD_TIMER:
		case TACTION_SUB_TIMER:
		case TACTION_SET_TIMER:
		case TACTION_SET_GLOBAL:
		case TACTION_CLEAR_GLOBAL:
			return(NEED_NUMBER);

		case TACTION_PREFERRED_TARGET:
			return(NEED_QUARRY);

		default:
			break;
	}
	return(NEED_NONE);
}


