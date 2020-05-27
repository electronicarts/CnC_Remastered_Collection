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

/* $Header: /CounterStrike/CONQUER.CPP 6     3/13/97 2:05p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CONQUER.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 3, 1991                                                *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CC_Draw_Shape -- Custom draw shape handler.                                               *
 *   Call_Back -- Main game maintenance callback routine.                                      *
 *   Color_Cycle -- Handle the general palette color cycling.                                  *
 *   Crate_From_Name -- Given a crate name convert it to a crate type.                         *
 *   Disk_Space_Available -- returns bytes of free disk space                                  *
 *   Do_Record_Playback -- handles saving/loading map pos & current object                     *
 *   Fading_Table_Name -- Builds a theater specific fading table name.                         *
 *   Fetch_Techno_Type -- Convert type and ID into TechnoTypeClass pointer.                    *
 *   Force_CD_Available -- Ensures that specified CD is available.                             *
 *   Get_Radar_Icon -- Builds and alloc a radar icon from a shape file                         *
 *   Handle_Team -- Processes team selection command.                                          *
 *   Handle_View -- Either records or restores the tactical view.                              *
 *   KN_To_Facing -- Converts a keyboard input number into a facing value.                     *
 *   Keyboard_Process -- Processes the tactical map input codes.                               *
 *   Language_Name -- Build filename for current language.                                     *
 *   List_Copy -- Makes a copy of a cell offset list.                                          *
 *   Main_Game -- Main game startup routine.                                                   *
 *   Main_Loop -- This is the main game loop (as a single loop).                               *
 *   Map_Edit_Loop -- a mini-main loop for map edit mode only                                  *
 *   Message_Input -- allows inter-player message input processing                             *
 *   MixFileHandler -- Handles VQ file access.                                                 *
 *   Name_From_Source -- retrieves the name for the given SourceType                           *
 *   Owner_From_Name -- Convert an owner name into a bitfield.                                 *
 *   Play_Movie -- Plays a VQ movie.                                                           *
 *   Shake_The_Screen -- Dispatcher that shakes the screen.                                    *
 *   Shape_Dimensions -- Determine the minimum rectangle for the shape.                        *
 *   Source_From_Name -- Converts ASCII name into SourceType.                                  *
 *   Sync_Delay -- Forces the game into a 15 FPS rate.                                         *
 *   Theater_From_Name -- Converts ASCII name into a theater number.                           *
 *   Unselect_All -- Causes all selected objects to become unselected.                         *
 *   VQ_Call_Back -- Maintenance callback used for VQ movies.                                  *
 *   Game_Registry_Key -- Returns pointer to string containing the registry subkey for the game.
 *   Is_Counterstrike_Installed -- Function to determine the availability of the CS expansion.
 *   Is_Aftermath_Installed -- Function to determine the availability of the AM expansion.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifdef TESTCODE
class A {
	public:
		enum {VAR=1};
};

template<class T>
class B {
	public:
		enum {VAR2=T::VAR};  // this is the line in question.
};

B<A> test;
#endif



#include	"function.h"
#ifdef WIN32
#ifdef WINSOCK_IPX
#include	"WSProto.h"
#else	//WINSOCK_IPX
#include	"tcpip.h"
#endif	//WINSOCK_IPX
#else
#include	"fakesock.h"
TcpipManagerClass	Winsock;
#endif
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<direct.h>
#include	<fcntl.h>
#include	<io.h>
#include	<dos.h>
#include	<share.h>
#include	"ccdde.h"
#include	"vortex.h"

#ifdef WOLAPI_INTEGRATION
//#include "WolDebug.h"
#include "WolStrng.h"
#include "WolapiOb.h"
extern WolapiObject* pWolapi;
#define PAGE_RESPOND_KEY	KN_RETURN	//KN_COMMA
#endif

#ifdef MPEGMOVIE
#ifdef MCIMPEG
#include "mcimovie.h"
#endif
#include "movie.h"
MPG_RESPONSE far __stdcall MpegCallback(MPG_CMD cmd, LPVOID data, LPVOID user);
#endif

#define SHAPE_TRANS		0x40

void * Get_Shape_Header_Data(void * ptr);
extern bool Spawn_WChat(bool can_launch);

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
void Enable_Secret_Units(void);
#endif

extern bool Is_Mission_Aftermath (char *file_name);
extern bool Is_Mission_Counterstrike (char *file_name);

#ifdef FIXIT_VERSION_3		//	Stalemate games.
extern void Do_Draw(void);
#endif

#ifdef CHEAT_KEYS
bool	bNoMovies = false;
#endif

/****************************************
**	Function prototypes for this module **
*****************************************/
bool Main_Loop(void);
void Keyboard_Process(KeyNumType & input);
static void Message_Input(KeyNumType &input);
void Color_Cycle(void);
bool Map_Edit_Loop(void);

extern "C" {
	bool UseOldShapeDraw = false;
}

#ifdef CHEAT_KEYS
void Dump_Heap_Pointers( void );
void Error_In_Heap_Pointers( char * string );
#endif
static void Do_Record_Playback(void);

void Toggle_Formation(void);

extern "C" {
	extern char * __nheapbeg;
}

//
// Special module globals for recording and playback
//
char TeamEvent = 0;			// 0 = no event, 1,2,3 = team event type
char TeamNumber = 0;			// which team was selected? (1-9)
char FormationEvent = 0;	// 0 = no event, 1 = formation was toggled


	/* -----------------10/14/96 7:29PM------------------

	 --------------------------------------------------*/

#if(TEN)
void TEN_Call_Back(void);
#endif	// TEN

#if(MPATH)
void MPATH_Call_Back(void);
#endif	// MPATH

/***********************************************************************************************
 * Main_Game -- Main game startup routine.                                                     *
 *                                                                                             *
 *    This is the first official routine of the game. It handles game initialization and       *
 *    the main game loop control.                                                              *
 *                                                                                             *
 *    Initialization:                                                                          *
 *    - Init_Game handles one-time-only inits                                                  *
 *    - Select_Game is responsible for initializations required for each new game played       *
 *      (these may be different depending on whether a multiplayer game is selected, and       *
 *      other parameters)                                                                      *
 *    - This routine performs any un-inits required, both for each game played, and one-time   *
 *                                                                                             *
 * INPUT:   argc  -- Number of command line arguments (including program name itself).         *
 *                                                                                             *
 *          argv  -- Array of command line argument pointers.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void Main_Game(int argc, char * argv[])
{
	static bool fade = true;

	/*
	**	Perform one-time-only initializations
	*/
	if (!Init_Game(argc, argv)) {
		return;
	}

	/*
	**	Game processing loop:
	**	1) Select which game to play, or whether to exit (don't fade the palette
	**		on the first game selection, but fade it in on subsequent calls)
	**	2) Invoke either the main-loop routine, or the editor-loop routine,
	**		until they indicate that the user wants to exit the scenario.
	*/
	while (Select_Game(fade)) {

		// ST 5/14/2019
		if (RunningAsDLL) {
			return;
		}

		fade = false;
		ScenarioInit = 0;		// Kludge.

		fade = true;

		/*
		** Initialise the color lookup tables for the chronal vortex
		*/
		ChronalVortex.Stop();
		ChronalVortex.Setup_Remap_Tables(Scen.Theater);

		/*
		**	Make the game screen visible, clear the keyboard buffer of spurious
		**	values, and then show the mouse.  This PRESUMES that Select_Game() has
		**	told the map to draw itself.
		*/
		GamePalette.Set(FADE_PALETTE_MEDIUM);
		Keyboard->Clear();
		/*
		** Only show the mouse if we're not playing back a recording.
		*/
		if (Session.Play) {
			Hide_Mouse();
			TeamEvent = 0;
			TeamNumber = 0;
			FormationEvent = 0;
		} else {
			Show_Mouse();
		}

#ifdef WIN32
		if (Session.Type == GAME_INTERNET) {
			Register_Game_Start_Time();
			GameStatisticsPacketSent = false;
			PacketLater = NULL;
			ConnectionLost = false;
		} else {
#ifndef WOLAPI_INTEGRATION
			DDEServer.Disable();
#endif	//	!WOLAPI_INTEGRATION
		}
#endif	//WIN32

#ifdef SCENARIO_EDITOR
		/*
		**	Scenario-editor version of main-loop processing
		*/
		for (;;) {
			/*
			**	Non-scenario-editor-mode: call the game's main loop
			*/
			if (!Debug_Map) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				TimeQuake = PendingTimeQuake;
				PendingTimeQuake = false;
#else
				TimeQuake = false;
#endif
				if (Main_Loop()) {
					break;
				}

				if (SpecialDialog != SDLG_NONE) {
					switch (SpecialDialog) {
						case SDLG_SPECIAL:
							Map.Help_Text(TXT_NONE);
							Map.Override_Mouse_Shape(MOUSE_NORMAL, false);
							Special_Dialog();
							Map.Revert_Mouse_Shape();
							SpecialDialog = SDLG_NONE;
							break;

						case SDLG_OPTIONS:
							Map.Help_Text(TXT_NONE);
							Map.Override_Mouse_Shape(MOUSE_NORMAL, false);
							Options.Process();
							Map.Revert_Mouse_Shape();
							SpecialDialog = SDLG_NONE;
							break;

						case SDLG_SURRENDER:
							Map.Help_Text(TXT_NONE);
							Map.Override_Mouse_Shape(MOUSE_NORMAL, false);
							if (Surrender_Dialog(TXT_SURRENDER)) {
								PlayerPtr->Flag_To_Lose();
							}
							SpecialDialog = SDLG_NONE;
							Map.Revert_Mouse_Shape();
							break;

						default:
							break;
					}
				}
			} else {

				/*
				**	Scenario-editor-mode: call the editor's main loop
				*/
				if (Map_Edit_Loop()) {
					break;
				}
			}
		}
#else
		/*
		**	Non-editor version of main-loop processing
		*/
		for (;;) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			TimeQuake = PendingTimeQuake;
			PendingTimeQuake = false;
#else
			TimeQuake = false;
#endif
			/*
			**	Call the game's main loop
			*/
			if (Main_Loop()) {
				break;
			}

			/*
			**	If the SpecialDialog flag is set, invoke the given special dialog.
			**	This must be done outside the main loop, since the dialog will call
			**	Main_Loop(), allowing the game to run in the background.
			*/
			if (SpecialDialog != SDLG_NONE) {
				switch (SpecialDialog) {
					case SDLG_SPECIAL:
						Map.Help_Text(TXT_NONE);
						Map.Override_Mouse_Shape(MOUSE_NORMAL, false);
						Special_Dialog();
						Map.Revert_Mouse_Shape();
						SpecialDialog = SDLG_NONE;
						break;

					case SDLG_OPTIONS:
						Map.Help_Text(TXT_NONE);
						Map.Override_Mouse_Shape(MOUSE_NORMAL, false);
						Options.Process();
						Map.Revert_Mouse_Shape();
						SpecialDialog = SDLG_NONE;
						break;

					case SDLG_SURRENDER:
						Map.Help_Text(TXT_NONE);
						Map.Override_Mouse_Shape(MOUSE_NORMAL, false);
						if (Surrender_Dialog(TXT_SURRENDER)) {
							OutList.Add(EventClass(EventClass::DESTRUCT));
						}
						SpecialDialog = SDLG_NONE;
						Map.Revert_Mouse_Shape();
						break;

/*ifdef FIXIT_VERSION_3		//	Stalemate games.
					case SDLG_PROPOSE_DRAW:
						Map.Help_Text(TXT_NONE);
						Map.Override_Mouse_Shape(MOUSE_NORMAL, false);
						if (Surrender_Dialog(TXT_WOL_PROPOSE_DRAW)) {
							OutList.Add(EventClass(EventClass::PROPOSE_DRAW));
						}
						SpecialDialog = SDLG_NONE;
						Map.Revert_Mouse_Shape();
						break;

					case SDLG_ACCEPT_DRAW:
						Map.Help_Text(TXT_NONE);
						Map.Override_Mouse_Shape(MOUSE_NORMAL, false);
						if (Surrender_Dialog(TXT_WOL_ACCEPT_DRAW)) {
							OutList.Add(EventClass(EventClass::ACCEPT_DRAW));
						}
						SpecialDialog = SDLG_NONE;
						Map.Revert_Mouse_Shape();
						break;
#endif
*/
					default:
						break;
				}
			}
		}
#endif


#ifdef WIN32
		/*
		** Send the game stats to WChat if we haven't already done so
		*/
		if (!GameStatisticsPacketSent && PacketLater) {
			Send_Statistics_Packet();		//	After game sending if PacketLater set.
		}
#endif	//WIN32

		/*
		**	Scenario is done; fade palette to black
		*/
		BlackPalette.Set(FADE_PALETTE_SLOW);
		VisiblePage.Clear();

		/*
		**	Un-initialize whatever needs it, for each game played.
		**
		**	Shut down either the modem or network; they'll get re-initialized if
		**	the user selections those options again in Select_Game().  This
		**	"re-boots" the modem & network code, which I currently feel is safer
		**	than just letting it hang around.
		** (Skip this step if we're in playback mode; the modem or net won't have
		** been initialized in that case.)
		*/
		if (Session.Record || Session.Play) {
			Session.RecordFile.Close();
		}

		if (Session.Type == GAME_NULL_MODEM || Session.Type == GAME_MODEM) {
			if (!Session.Play) {
				//PG Modem_Signoff();
			}
		} else {
			if (Session.Type == GAME_IPX) {
				if (!Session.Play) {
					//PG Shutdown_Network();
				}
			}
		}

#if(TEN)

		if (Session.Type == GAME_TEN) {
			Shutdown_TEN();
			//Prog_End();
			Emergency_Exit(0);
		}
#endif	// TEN

#if(MPATH)
		if (Session.Type == GAME_MPATH) {
			Shutdown_MPATH();
			//Prog_End();
			Emergency_Exit(0);
		}
#endif	// MPATH

		/*
		**	If we're playing back, the mouse will be hidden; show it.
		** Also, set all variables back to normal, to return to the main menu.
		*/
		if (Session.Play) {
			Show_Mouse();
			Session.Type = GAME_NORMAL;
			Session.Play = 0;
		}
#ifndef WOLAPI_INTEGRATION
#ifdef WIN32
		if (Special.IsFromWChat) {
			//PG Shutdown_Network();		      // Clear up the pseudo IPX stuff
#ifndef WINSOCK_IPX
			Winsock.Close();
#endif	//WINSOCK_IPX
			Special.IsFromWChat = false;
			SpawnedFromWChat = false;
			DDEServer.Delete_MPlayer_Game_Info();	//Make sure we dont use the same start packet twice
			Session.Type = GAME_NORMAL;			//Have to do this or we will got straight to the multiplayer menu
			Spawn_WChat(false);		//Will switch back to Wchat. It must be there because its been poking us
		}
#endif	//WIN32
#endif	//	!WOLAPI_INTEGRATION
	}

	/*
	**	Free the scenario description buffers
	*/
	Session.Free_Scenario_Descriptions();
}


/***********************************************************************************************
 * Keyboard_Process -- Processes the tactical map input codes.                                 *
 *                                                                                             *
 *    This routine is used to process the input codes while the player                         *
 *    has the tactical map displayed. It handles all the keys that                             *
 *    are appropriate to that mode.                                                            *
 *                                                                                             *
 * INPUT:   input -- Input code as returned from Input_Num().                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/21/1992 JLB : Created.                                                                 *
 *   07/04/1995 JLB : Handles team and map control hotkeys.                                    *
 *=============================================================================================*/
void Keyboard_Process(KeyNumType & input)
{
	ObjectClass * obj;
	int index;

	/*
	**	Don't do anything if there is not keyboard event.
	*/
	if (input == KN_NONE) {
		return;
	}
	/*
	**	For network & modem, process user input for inter-player messages.
	*/
	Message_Input(input);

#ifdef WIN32
	/*
	**	The VK_BIT must be stripped from the "plain" value of the key so that a comparison to
	**	KN_1, for example, will yield TRUE if in fact the "1" key was pressed.
	*/

	KeyNumType plain = KeyNumType(input & ~(WWKEY_SHIFT_BIT|WWKEY_ALT_BIT|WWKEY_CTRL_BIT|WWKEY_VK_BIT));
	KeyNumType key = KeyNumType(input & ~WWKEY_VK_BIT);


#else
	KeyNumType plain = KeyNumType(input & ~(KN_SHIFT_BIT|KN_ALT_BIT|KN_CTRL_BIT));
	KeyNumType key = plain;
#endif

#ifdef CHEAT_KEYS

	if (Debug_Flag) {
		HousesType h;

		switch (int(input)) {
			case int(int(KN_M) | int(KN_SHIFT_BIT)):
			case int(int(KN_M) | int(KN_ALT_BIT)):
			case int(int(KN_M) | int(KN_CTRL_BIT)):
				for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
					Houses.Ptr(h)->Refund_Money(10000);
				}
				break;

			default:
				break;
		}
	}
#endif

#ifdef VIRGIN_CHEAT_KEYS
	if (Debug_Playtest && input == (KN_W|KN_ALT_BIT)) {
		PlayerPtr->Blockage = false;
		PlayerPtr->Flag_To_Win();
	}
#endif

#ifdef CHEAT_KEYS
#ifdef WIN32
	if (Debug_Playtest && input == (KA_W|KN_ALT_BIT)) {
#else
	if (Debug_Playtest && input == (KN_W|KN_ALT_BIT)) {
#endif
		PlayerPtr->Blockage = false;
		PlayerPtr->Flag_To_Win();
	}

	if ((Debug_Flag || Debug_Playtest) && plain == KN_F4) {
		if (Session.Type == GAME_NORMAL) {
			Debug_Unshroud = (Debug_Unshroud == false);
			Map.Flag_To_Redraw(true);
		}
	}

	if (Debug_Flag && input == KN_SLASH) {
		if (Session.Type != GAME_NORMAL) {
			SpecialDialog = SDLG_SPECIAL;
			input = KN_NONE;
		} else {
			Special_Dialog();
		}
	}
#endif

	/*
	**	Process prerecorded team selection. This will be an additive select
	**	if the SHIFT key is held down. It will create the team if the
	**	CTRL or ALT key is held down.
	*/
	int action = 0;
#ifdef WIN32
	if (input & WWKEY_SHIFT_BIT) action = 1;
	if (input & WWKEY_ALT_BIT) action = 3;
	if (input & WWKEY_CTRL_BIT) action = 2;
#else
	if (input & KN_SHIFT_BIT) action = 1;
	if (input & KN_ALT_BIT) action = 3;
	if (input & KN_CTRL_BIT) action = 2;
#endif

	/*
	**	If the "N" key is pressed, then select the next object.
	*/
	if (key != 0 && key == Options.KeyNext) {
		if (action) {
			obj = Map.Prev_Object(CurrentObject.Count() ? CurrentObject[0] : NULL);
		} else {
			obj = Map.Next_Object(CurrentObject.Count() ? CurrentObject[0] : NULL);
		}
		if (obj != NULL) {
			Unselect_All();
		 	obj->Select();
			Map.Center_Map();
			Map.Flag_To_Redraw(true);
		}
		input = KN_NONE;
	}
	if (key != 0 && key == Options.KeyPrevious) {
		if (action) {
			obj = Map.Next_Object(CurrentObject.Count() ? CurrentObject[0] : NULL);
		} else {
			obj = Map.Prev_Object(CurrentObject.Count() ? CurrentObject[0] : NULL);
		}
		if (obj != NULL) {
			Unselect_All();
		 	obj->Select();
			Map.Center_Map();
			Map.Flag_To_Redraw(true);
		}
		input = KN_NONE;
	}


	/*
	**	All selected units will go into idle mode.
	*/
	if (key != 0 && key == Options.KeyStop) {
		if (CurrentObject.Count()) {
			for (index = 0; index < CurrentObject.Count(); index++) {
				ObjectClass const * tech = CurrentObject[index];

				if (tech != NULL && (tech->Can_Player_Move() || (tech->Can_Player_Fire() && tech->What_Am_I() != RTTI_BUILDING))) {
					OutList.Add(EventClass(EventClass::IDLE, TargetClass(tech)));
				}
			}
		}
		input = KN_NONE;
	}

	/*
	**	All selected units will attempt to go into guard area mode.
	*/
	if (key != 0 && key == Options.KeyGuard) {
		if (CurrentObject.Count()) {
			for (index = 0; index < CurrentObject.Count(); index++) {
				ObjectClass const * tech = CurrentObject[index];

				if (tech != NULL && tech->Can_Player_Move() && tech->Can_Player_Fire()) {
					OutList.Add(EventClass(TargetClass(tech), MISSION_GUARD_AREA));
				}
			}
		}
		input = KN_NONE;
	}

	/*
	**	All selected units will attempt to scatter.
	*/
	if (key != 0 && key == Options.KeyScatter) {
		if (CurrentObject.Count()) {
			for (index = 0; index < CurrentObject.Count(); index++) {
				ObjectClass const * tech = CurrentObject[index];

				if (tech != NULL && tech->Can_Player_Move()) {
					OutList.Add(EventClass(EventClass::SCATTER, TargetClass(tech)));
				}
			}
		}
		input = KN_NONE;
	}

	/*
	**	Center the map around the currently selected objects. If no
	**	objects are selected, then fall into the home case.
	*/
	if (key != 0 && (key == Options.KeyHome1 || key == Options.KeyHome2)) {
		if (CurrentObject.Count()) {
			Map.Center_Map();
#ifdef WIN32
			Map.Flag_To_Redraw(true);
#endif
			input = KN_NONE;
		} else {
			input = Options.KeyBase;
		}
	}

	/*
	**	Center the map about the construction yard or construction vehicle
	**	if one is present.
	*/
	if (key != 0 && key == Options.KeyBase) {
		Unselect_All();
		if (PlayerPtr->CurBuildings) {
			for (index = 0; index < Buildings.Count(); index++) {
				BuildingClass * building = Buildings.Ptr(index);

				if (building != NULL && !building->IsInLimbo && building->House == PlayerPtr && *building == STRUCT_CONST) {
					Unselect_All();
					building->Select();
					if (building->IsLeader) break;
				}
			}
		}
		if (CurrentObject.Count() == 0 && PlayerPtr->CurUnits) {
			for (index = 0; index < Units.Count(); index++) {
				UnitClass * unit = Units.Ptr(index);

				if (unit != NULL && !unit->IsInLimbo && unit->House == PlayerPtr && *unit == UNIT_MCV) {
					Unselect_All();
					unit->Select();
					break;
				}
			}
		}
		if (CurrentObject.Count()) {
			Map.Center_Map();
		} else {
			if (PlayerPtr->Center != 0) {
				Map.Center_Map(PlayerPtr->Center);
			}
		}
		Map.Flag_To_Redraw(true);
		input = KN_NONE;
	}

	/*
	** Toggle the status of formation for the current team
	*/
	if (key != 0 && key == Options.KeyFormation) {
		Toggle_Formation();
		input = KN_NONE;
	}

#ifdef TOFIX
	/*
	** For multiplayer, 'R' pops up the surrender dialog.
	*/
	if (input != 0 && input == Options.KeyResign) {
		if (!PlayerLoses && /*Session.Type != GAME_NORMAL &&*/ !PlayerPtr->IsDefeated) {
			SpecialDialog = SDLG_SURRENDER;
			input = KN_NONE;
		}
		input = KN_NONE;
	}
#endif

	/*
	**	Handle making and breaking alliances.
	*/
	if (key != 0 && key == Options.KeyAlliance) {
		if (Session.Type != GAME_NORMAL || Debug_Flag) {
			if (CurrentObject.Count() && !PlayerPtr->IsDefeated) {
				if (CurrentObject[0]->Owner() != PlayerPtr->Class->House) {
					OutList.Add(EventClass(EventClass::ALLY, CurrentObject[0]->Owner()));
				}
			}
		}
		input = KN_NONE;
	}

	/*
	**	Select all the units on the current display. This is equivalent to
	**	drag selecting the whole view.
	*/
	if (key != 0 && key == Options.KeySelectView) {
		Map.Select_These(0x00000000, XY_Coord(Map.TacLeptonWidth, Map.TacLeptonHeight));
		input = KN_NONE;
	}

	/*
	**	Toggles the repair state similarly to pressing the repair button.
	*/
	if (key != 0 && key == Options.KeyRepair) {
		Map.Repair_Mode_Control(-1);
		input = KN_NONE;
	}

	/*
	**	Toggles the sell state similarly to pressing the sell button.
	*/
	if (key != 0 && key == Options.KeySell) {
		Map.Sell_Mode_Control(-1);
		input = KN_NONE;
	}

	/*
	**	Toggles the map zoom mode similarly to pressing the map button.
	*/
	if (key != 0 && key == Options.KeyMap) {
		Map.Zoom_Mode_Control();
		input = KN_NONE;
	}

	/*
	**	Scrolls the sidebar up one slot.
	*/
	if (key != 0 && key == Options.KeySidebarUp) {
		Map.SidebarClass::Scroll(true, -1);
		input = KN_NONE;
	}

	/*
	**	Scrolls the sidebar down one slot.
	*/
	if (key != 0 && key == Options.KeySidebarDown) {
		Map.SidebarClass::Scroll(false, -1);
		input = KN_NONE;
	}

	/*
	**	Brings up the options dialog box.
	*/
	if (key != 0 && (key == Options.KeyOption1 || key == Options.KeyOption2)) {
		Map.Help_Text(TXT_NONE);			// Turns off help text.
		Queue_Options();
		input = KN_NONE;
	}

	/*
	**	Scrolls the tactical map in the direction specified.
	*/
	int distance = CELL_LEPTON_W;
	if (key != 0 && key == Options.KeyScrollLeft) {
		Map.Scroll_Map(DIR_W, distance, true);
		input = KN_NONE;
	}
	if (key != 0 && key == Options.KeyScrollRight) {
		Map.Scroll_Map(DIR_E, distance, true);
		input = KN_NONE;
	}
	if (key != 0 && key == Options.KeyScrollUp) {
		Map.Scroll_Map(DIR_N, distance, true);
		input = KN_NONE;
	}
	if (key != 0 && key == Options.KeyScrollDown) {
		Map.Scroll_Map(DIR_S, distance, true);
		input = KN_NONE;
	}

	/*
	**	Teams are handled by the 10 special team keys. The manual comparison
	**	to the KN numbers is because the Windows keyboard driver can vary
	**	the base code number for the key depending on the shift or alt key
	**	state!
	*/
	if (input != 0 && (plain == Options.KeyTeam1 || plain == KN_1)) {
		Handle_Team(0, action);
		input = KN_NONE;
	}
	if (input != 0 && (plain == Options.KeyTeam2 || plain == KN_2)) {
		Handle_Team(1, action);
		input = KN_NONE;
	}
	if (input != 0 && (plain == Options.KeyTeam3 || plain == KN_3)) {
		Handle_Team(2, action);
		input = KN_NONE;
	}
	if (input != 0 && (plain == Options.KeyTeam4 || plain == KN_4)) {
		Handle_Team(3, action);
		input = KN_NONE;
	}
	if (input != 0 && (plain == Options.KeyTeam5 || plain == KN_5)) {
		Handle_Team(4, action);
		input = KN_NONE;
	}
	if (input != 0 && (plain == Options.KeyTeam6 || plain == KN_6)) {
		Handle_Team(5, action);
		input = KN_NONE;
	}
	if (input != 0 && (plain == Options.KeyTeam7 || plain == KN_7)) {
		Handle_Team(6, action);
		input = KN_NONE;
	}
	if (input != 0 && (plain == Options.KeyTeam8 || plain == KN_8)) {
		Handle_Team(7, action);
		input = KN_NONE;
	}
	if (input != 0 && (plain == Options.KeyTeam9 || plain == KN_9)) {
		Handle_Team(8, action);
		input = KN_NONE;
	}
	if (input != 0 && (plain == Options.KeyTeam10 || plain == KN_0)) {
		Handle_Team(9, action);
		input = KN_NONE;
	}

	/*
	**	Handle the bookmark hotkeys.
	*/
	if (input != 0 && plain == Options.KeyBookmark1 && !Debug_Map) {
		Handle_View(0, action);
		input = KN_NONE;
	}
	if (input != 0 && plain == Options.KeyBookmark2 && !Debug_Map) {
		Handle_View(1, action);
		input = KN_NONE;
	}
	if (input != 0 && plain == Options.KeyBookmark3 && !Debug_Map) {
		Handle_View(2, action);
		input = KN_NONE;
	}
	if (input != 0 && plain == Options.KeyBookmark4 && !Debug_Map) {
		Handle_View(3, action);
		input = KN_NONE;
	}

#ifdef CHEAT_KEYS
	if (input != 0 && Debug_Flag && input && (input & KN_RLSE_BIT) == 0) {
		Debug_Key(input);
	}
#endif
}


void Toggle_Formation(void) {

// MBL 03.23.2020: this has been copied to DLLExportClass::Team_Units_Formation_Toggle_On(), and modified as needed
#if 0
	int team = -1;
	long minx = 0x7FFFFFFFL, miny = 0x7FFFFFFFL;
	long maxx = 0, maxy = 0;
	int index;
	bool setform = 0;

	//
	// Recording support
	//
	if (Session.Record) {
		FormationEvent = 1;
	}

	/*
	** Find the first selected object that is a member of a team, and
	** register his group as the team we're using.  Once we find the team
	** number, update the 'setform' flag to know whether we should be setting
	** the formation's offsets, or clearing them.  If they currently have
	** illegal offsets (as in 0x80000000), then we're setting.
	*/
	for (index = 0; index < Units.Count(); index++) {
		UnitClass * obj = Units.Ptr(index);
		if (obj && !obj->IsInLimbo && obj->House == PlayerPtr && obj->IsSelected) {
			team = obj->Group;
			if (team != -1) {
				setform = obj->XFormOffset == (int)0x80000000;
				TeamSpeed[team] = SPEED_WHEEL;
				TeamMaxSpeed[team] = MPH_LIGHT_SPEED;
				break;
			}
		}
	}
	if (team == -1) {
		for (index = 0; index < Infantry.Count(); index++) {
			InfantryClass * obj = Infantry.Ptr(index);
			if (obj && !obj->IsInLimbo && obj->House == PlayerPtr && obj->IsSelected) {
				team = obj->Group;
				if (team != -1) {
					setform = obj->XFormOffset == (int)0x80000000;
					TeamSpeed[team] = SPEED_WHEEL;
					TeamMaxSpeed[team] = MPH_LIGHT_SPEED;
					break;
				}
			}
		}
	}

	if (team == -1) {
		for (index = 0; index < Vessels.Count(); index++) {
			VesselClass * obj = Vessels.Ptr(index);
			if (obj && !obj->IsInLimbo && obj->House == PlayerPtr && obj->IsSelected) {
				team = obj->Group;
				if (team != -1) {
					setform = obj->XFormOffset == 0x80000000UL;
					TeamSpeed[team] = SPEED_WHEEL;
					TeamMaxSpeed[team] = MPH_LIGHT_SPEED;
					break;
				}
			}
		}
	}

	if (team == -1) return;
	/*
	** Now that we have a team, let's go set (or clear) the formation offsets.
	*/
	for (index = 0; index < Units.Count(); index++) {
		UnitClass * obj = Units.Ptr(index);
		if (obj && !obj->IsInLimbo && obj->House == PlayerPtr && obj->Group == team) {
			obj->Mark(MARK_CHANGE);
			if (setform) {
				long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
				long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));
				if (xc < minx) minx = xc;
				if (xc > maxx) maxx = xc;
				if (yc < miny) miny = yc;
				if (yc > maxy) maxy = yc;
				if (obj->Class->MaxSpeed < TeamMaxSpeed[team]) {
					TeamMaxSpeed[team] = obj->Class->MaxSpeed;
					TeamSpeed[team] = obj->Class->Speed;
				}
			} else {
				obj->XFormOffset = obj->YFormOffset = (int)0x80000000;
			}
		}
	}

	for (index = 0; index < Infantry.Count(); index++) {
		InfantryClass * obj = Infantry.Ptr(index);
		if (obj && !obj->IsInLimbo && obj->House == PlayerPtr && obj->Group == team) {
			obj->Mark(MARK_CHANGE);
			if (setform) {
				long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
				long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));
				if (xc < minx) minx = xc;
				if (xc > maxx) maxx = xc;
				if (yc < miny) miny = yc;
				if (yc > maxy) maxy = yc;
				if (obj->Class->MaxSpeed < TeamMaxSpeed[team]) {
					TeamMaxSpeed[team] = obj->Class->MaxSpeed;
				}
			} else {
				obj->XFormOffset = obj->YFormOffset = (int)0x80000000;
			}
		}
	}

	for (index = 0; index < Vessels.Count(); index++) {
		VesselClass * obj = Vessels.Ptr(index);
		if (obj && !obj->IsInLimbo && obj->House == PlayerPtr && obj->Group == team) {
			obj->Mark(MARK_CHANGE);
			if (setform) {
				long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
				long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));
				if (xc < minx) minx = xc;
				if (xc > maxx) maxx = xc;
				if (yc < miny) miny = yc;
				if (yc > maxy) maxy = yc;
				if (obj->Class->MaxSpeed < TeamMaxSpeed[team]) {
					TeamMaxSpeed[team] = obj->Class->MaxSpeed;
				}
			} else {
				obj->XFormOffset = obj->YFormOffset = 0x80000000UL;
			}
		}
	}

	/*
	** All the units have been counted to find the bounding rectangle and
	** center of the formation, or to clear their offsets.  Now, if we're to
	** set them into formation, proceed to do so.  Otherwise, bail.
	*/
	if (setform) {
		int centerx = (int)((maxx - minx)/2)+minx;
		int centery = (int)((maxy - miny)/2)+miny;

		for (index = 0; index < Units.Count(); index++) {
			UnitClass * obj = Units.Ptr(index);
			if (obj && !obj->IsInLimbo && obj->House == PlayerPtr && obj->Group == team) {
				long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
				long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));

				obj->XFormOffset = xc - centerx;
				obj->YFormOffset = yc - centery;
			}
		}

		for (index = 0; index < Infantry.Count(); index++) {
			InfantryClass * obj = Infantry.Ptr(index);
			if (obj && !obj->IsInLimbo && obj->House == PlayerPtr && obj->Group == team ) {
				long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
				long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));

				obj->XFormOffset = xc - centerx;
				obj->YFormOffset = yc - centery;
			}
		}

		for (index = 0; index < Vessels.Count(); index++) {
			VesselClass * obj = Vessels.Ptr(index);
			if (obj && !obj->IsInLimbo && obj->House == PlayerPtr && obj->Group == team ) {
				long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
				long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));

				obj->XFormOffset = xc - centerx;
				obj->YFormOffset = yc - centery;
			}
		}
	}
#endif
}


/***********************************************************************************************
 * Message_Input -- allows inter-player message input processing                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		input		key value																							  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		none.																												  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		none.																												  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
//#pragma off (unreferenced)
static void Message_Input(KeyNumType &input)
{
	int rc;
	char txt[MAX_MESSAGE_LENGTH+32];
	int id;
//	SerialPacketType * serial_packet;
	//int i;
	KeyNumType copy_input;
	//char *msg;

	/*
	**	Check keyboard input for a request to send a message.
	**	The 'to' argument for Add_Edit is prefixed to the message buffer; the
	**	message buffer is big enough for the 'to' field plus MAX_MESSAGE_LENGTH.
	**	To send the message, calling Get_Edit_Buf retrieves the buffer minus the
	**	'to' portion.  At the other end, the buffer allocated to display the
	**	message must be MAX_MESSAGE_LENGTH plus the size of "From: xxx (house)".
	*/
#ifdef WOLAPI_INTEGRATION
	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && 
		( ( input >= KN_F1 && input < (KN_F1 + Session.MaxPlayers) ) || input == PAGE_RESPOND_KEY ) && 
		!Session.Messages.Is_Edit()) {
#else
	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && input >= KN_F1 && input < (KN_F1 + Session.MaxPlayers) && !Session.Messages.Is_Edit()) {
#endif
		memset (txt, 0, 40);

		/*
		**	For a serial game, send a message on F1 or F4; set 'txt' to the
		**	"Message:" string & add an editable message to the list.
		*/
		if (Session.Type==GAME_NULL_MODEM || Session.Type==GAME_MODEM) {
			if (input==KN_F1 || input==(KN_F1 + Session.MaxPlayers - 1)) {

				strcpy(txt, Text_String(TXT_MESSAGE));	// "Message:"

				Session.Messages.Add_Edit (Session.ColorIdx,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 0, 232 * RESFACTOR);

				Map.Flag_To_Redraw(false);
			}
		} else if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && !Session.Messages.Is_Edit()) {
		/*
		**	For a network game:
		**	F1-F7 = "To <name> (house):" (only allowed if we're not in ObiWan mode)
		**	F8 = "To All:"
		*/
			if (input==(KN_F1 + Session.MaxPlayers - 1)) {

				Session.MessageAddress = IPXAddressClass();		// set to broadcast
				strcpy(txt, Text_String(TXT_TO_ALL));	// "To All:"

				Session.Messages.Add_Edit(Session.ColorIdx,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 0, 232 * RESFACTOR);

				Map.Flag_To_Redraw(false);

#ifdef WOLAPI_INTEGRATION
			} else if ((input - KN_F1) < Ipx.Num_Connections() && !Session.ObiWan && input != PAGE_RESPOND_KEY ) {
#else
			} else if ((input - KN_F1) < Ipx.Num_Connections() && !Session.ObiWan) {
#endif
				id = Ipx.Connection_ID(input - KN_F1);
				Session.MessageAddress = (*(Ipx.Connection_Address (id)));
				sprintf(txt, Text_String(TXT_TO), Ipx.Connection_Name(id));

				Session.Messages.Add_Edit(Session.ColorIdx,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 0, 232 * RESFACTOR);

				Map.Flag_To_Redraw(false);
			}
#ifdef WOLAPI_INTEGRATION
			else if( Session.Type == GAME_INTERNET && pWolapi && !pWolapi->bConnectionDown && input == PAGE_RESPOND_KEY )
			{
				if( *pWolapi->szExternalPager )
				{
					//	Respond to a page from external ww online user that paged me.
					//	Set MessageAddress to all zeroes, as a flag to ourselves later on.
					NetNumType blip;
					NetNodeType blop;
					memset( blip, 0, 4 );
					memset( blop, 0, 6 );
					Session.MessageAddress = IPXAddressClass( blip, blop );

					//	Tell pWolapi not to reset szExternalPager for the time being.
					pWolapi->bFreezeExternalPager = true;

					sprintf( txt, Text_String( TXT_TO ), pWolapi->szExternalPager );

					Session.Messages.Add_Edit(Session.ColorIdx,
						TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 0, 232 * RESFACTOR);

					Map.Flag_To_Redraw(false);

					Keyboard->Clear();
				}
				else
				{
					Session.Messages.Add_Message( NULL, 0, TXT_WOL_NOTPAGED, PCOLOR_GOLD, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE );
					Sound_Effect( VOC_SYS_ERROR );
				}
			}
#endif
		}
#if(TEN)
		/*
		**	For a TEN game:
		**	F1-F7 = "To <name> (house):" (only allowed if we're not in ObiWan mode)
		**	F8 = "To All:"
		*/
		else if (Session.Type == GAME_TEN && !Session.Messages.Is_Edit()) {
			if (input==(KN_F1 + Session.MaxPlayers - 1)) {

				Session.TenMessageAddress = -1;		// set to broadcast
				strcpy(txt,Text_String(TXT_TO_ALL));	// "To All:"

				Session.Messages.Add_Edit(Session.ColorIdx,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 0, 232 * RESFACTOR);

				Map.Flag_To_Redraw(false);

			}  else if ((input - KN_F1) < Ten->Num_Connections() && !Session.ObiWan) {

				id = Ten->Connection_ID(input - KN_F1);
				Session.TenMessageAddress = Ten->Connection_Address(id);
				sprintf(txt,Text_String(TXT_TO),Ten->Connection_Name(id));

				Session.Messages.Add_Edit(Session.ColorIdx,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 0, 232 * RESFACTOR);

				Map.Flag_To_Redraw(false);
			}
		}
#endif	// TEN
#if(MPATH)
		/*
		**	For a MPATH game:
		**	F1-F7 = "To <name> (house):" (only allowed if we're not in ObiWan mode)
		**	F8 = "To All:"
		*/
		else if (Session.Type == GAME_MPATH && !Session.Messages.Is_Edit()) {
			if (input==(KN_F1 + Session.MaxPlayers - 1)) {

				Session.MPathMessageAddress = 0;			// set to broadcast
				strcpy(txt,Text_String(TXT_TO_ALL));	// "To All:"

				Session.Messages.Add_Edit(Session.ColorIdx,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 0, 232 * RESFACTOR);

				Map.Flag_To_Redraw(false);

			}  else if ((input - KN_F1) < MPath->Num_Connections() && !Session.ObiWan) {

				id = MPath->Connection_ID(input - KN_F1);
				Session.MPathMessageAddress = MPath->Connection_Address(id);
				sprintf(txt,Text_String(TXT_TO),MPath->Connection_Name(id));

				Session.Messages.Add_Edit(Session.ColorIdx,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 0, 232 * RESFACTOR);

				Map.Flag_To_Redraw(false);
			}
		}
#endif	// MPATH
	}

	/*
	**	Process message-system input; send the message out if RETURN is hit.
	*/
	copy_input = input;
	rc = Session.Messages.Input(input);

	/*
	**	If a single character has been added to an edit buffer, update the display.
	*/
	if (rc == 1 && Session.Type != GAME_NORMAL) {
		Map.Flag_To_Redraw(false);
	}

	/*
	**	If backspace was hit, redraw the map.  If the edit message was removed,
	** the map must be force-drawn, since it won't be able to compute the
	** cells to redraw; otherwise, let the map compute the cells to redraw,
	** by not force-drawing it, but just setting the IsToRedraw bit.
	*/
	if (rc==2 && Session.Type != GAME_NORMAL) {
		if (copy_input==KN_ESC) {
			Map.Flag_To_Redraw(true);
#ifdef WOLAPI_INTEGRATION
			if( pWolapi )
				//	Just in case user was responding to a page from outside the game, and we had frozen the "szExternalPager".
				pWolapi->bFreezeExternalPager = false;
#endif
		} else {
			Map.Flag_To_Redraw(false);
		}
		Map.DisplayClass::IsToRedraw = true;
	}

	/*
	**	Send a message
	*/
	if ((rc==3 || rc==4) && Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
#if (0)
		/*
		**	Serial game: fill in a SerialPacketType & send it.
		**	(Note: The size of the SerialPacketType.Command must be the same as
		**	the EventClass.Type!)
		*/
		if (Session.Type==GAME_NULL_MODEM || Session.Type==GAME_MODEM) {
			serial_packet = (SerialPacketType *)NullModem.BuildBuf;

			serial_packet->Command = SERIAL_MESSAGE;
			strcpy (serial_packet->Name, Session.Players[0]->Name);
			serial_packet->ID = Session.ColorIdx;

			if (rc==3) {
				strcpy (serial_packet->Message.Message, Session.Messages.Get_Edit_Buf());
			} else {
				strcpy (serial_packet->Message.Message, Session.Messages.Get_Overflow_Buf());
				Session.Messages.Clear_Overflow_Buf();
			}

			/*
			** Send the message, and store this message in our LastMessage
			** buffer; the computer may send us a version of it later.
			*/
			NullModem.Send_Message(NullModem.BuildBuf,
				sizeof(SerialPacketType), 1);

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	char *ptr = &serial_packet->Message.Message[0];
	if (!strncmp(ptr,"SECRET UNITS ON ",15) && NewUnitsEnabled) {
		Enable_Secret_Units();
	}
#endif
			strcpy(Session.LastMessage, serial_packet->Message.Message);
		} else if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) {
#ifdef WOLAPI_INTEGRATION
			NetNumType blip;
			NetNodeType blop;
			Session.MessageAddress.Get_Address( blip, blop );
			if(	blip[0] + blip[1] + blip[2] + blip[3] + blop[0] + blop[1] + blop[2] + blop[3] + blop[4] + blop[5] == 0 )
			{
				//	This message is a response to the last person that paged me.
				if( pWolapi && !pWolapi->bConnectionDown )		//	(As connection may have gone down.)
				{
					pWolapi->Page( pWolapi->szExternalPager, Session.Messages.Get_Edit_Buf(), false );
					pWolapi->bFreezeExternalPager = false;
				}
			}
			else
#endif
			{

			/*
			**	Network game: fill in a GlobalPacketType & send it.
			*/
				Session.GPacket.Command = NET_MESSAGE;
				strcpy (Session.GPacket.Name, Session.Players[0]->Name);
				Session.GPacket.Message.Color = Session.ColorIdx;
				Session.GPacket.Message.NameCRC = Compute_Name_CRC(Session.GameName);

				if (rc==3) {
					strcpy (Session.GPacket.Message.Buf, Session.Messages.Get_Edit_Buf());
				} else {
					strcpy (Session.GPacket.Message.Buf,
						Session.Messages.Get_Overflow_Buf());
					Session.Messages.Clear_Overflow_Buf();
				}

				/*
				**	If 'F4' was hit, MessageAddress will be a broadcast address; send
				**	the message to every player we have a connection with.
				*/
				if (Session.MessageAddress.Is_Broadcast()) {
	#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		char *ptr = &Session.GPacket.Message.Buf[0];
		if (!strncmp(ptr,"SECRET UNITS ON ",15) && NewUnitsEnabled) {
			*ptr = 'X';		// force it to an odd hack so we know it was broadcast.
			Enable_Secret_Units();
		}
	#endif
					for (i = 0; i < Ipx.Num_Connections(); i++) {
						Ipx.Send_Global_Message(&Session.GPacket,
							sizeof(GlobalPacketType), 1,
							Ipx.Connection_Address(Ipx.Connection_ID(i)));
						Ipx.Service();
					}
				} else {

					/*
					**	Otherwise, MessageAddress contains the exact address to send to.
					**	Send to that address only.
					*/
					Ipx.Send_Global_Message(&Session.GPacket,
						sizeof(GlobalPacketType), 1,
						&Session.MessageAddress);
					Ipx.Service();

				}

				/*
				**	Store this message in our LastMessage buffer; the computer may send
				**	us a version of it later.
				*/
				strcpy(Session.LastMessage, Session.GPacket.Message.Buf);
			}
		}

#if(TEN)
		/*
		**	TEN game: fill in a GlobalPacketType & send it.
		*/
		else if (Session.Type == GAME_TEN) {
			Session.GPacket.Command = NET_MESSAGE;
			strcpy (Session.GPacket.Name, Session.Players[0]->Name);
			Session.GPacket.Message.Color = Session.ColorIdx;
			Session.GPacket.Message.NameCRC = Compute_Name_CRC(Session.GameName);

			if (rc==3) {
				strcpy (Session.GPacket.Message.Buf, Session.Messages.Get_Edit_Buf());
			} else {
				strcpy (Session.GPacket.Message.Buf,
					Session.Messages.Get_Overflow_Buf());
				Session.Messages.Clear_Overflow_Buf();
			}

			Ten->Send_Global_Message(&Session.GPacket, sizeof(GlobalPacketType),
				1, Session.TenMessageAddress);
		}
#endif	// TEN

#if(MPATH)
		/*
		**	MPATH game: fill in a GlobalPacketType & send it.
		*/
		else if (Session.Type == GAME_MPATH) {
			Session.GPacket.Command = NET_MESSAGE;
			strcpy (Session.GPacket.Name, Session.Players[0]->Name);
			Session.GPacket.Message.Color = Session.ColorIdx;
			Session.GPacket.Message.NameCRC = Compute_Name_CRC(Session.GameName);

			if (rc==3) {
				strcpy (Session.GPacket.Message.Buf, Session.Messages.Get_Edit_Buf());
			} else {
				strcpy (Session.GPacket.Message.Buf,
					Session.Messages.Get_Overflow_Buf());
				Session.Messages.Clear_Overflow_Buf();
			}

			MPath->Send_Global_Message(&Session.GPacket, sizeof(GlobalPacketType),
				1, Session.MPathMessageAddress);
		}
#endif	// MPATH

		/*
		**	Tell the map to completely update itself, since a message is now missing.
		*/
		Map.Flag_To_Redraw(true);
#endif
	}
}
//#pragma on (unreferenced)


/***********************************************************************************************
 * Color_Cycle -- Handle the general palette color cycling.                                    *
 *                                                                                             *
 *    This is a maintenance routine that handles the color cycling. It should be called as     *
 *    often as necessary to achieve smooth color cycling effects -- at least 8 times a second. *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   06/10/1994 JLB : Uses new cycle color values.                                             *
 *   12/21/1994 JLB : Handles text fade color.                                                 *
 *   07/16/1996 JLB : Faster pulsing of white color.                                           *
 *=============================================================================================*/
void Color_Cycle(void)
{
	static CDTimerClass<SystemTimerClass> _timer;
	static CDTimerClass<SystemTimerClass> _ftimer;
	static bool _up = false;
	static int val = 255;
	bool changed = false;

	if (Options.IsPaletteScroll) {
		/*
		**	Process the fading white color. It is used for the radar box and other glowing
		**	game interface elements.
		*/
		if (!_ftimer) {
			_ftimer = TIMER_SECOND/6;

			#define	STEP_RATE	20
			if (_up) {
				val += STEP_RATE;
				if (val > 150) {
					val = 150;
					_up = false;
				}
			} else {
				val -= STEP_RATE;
				if (val < 0x20) {
					val = 0x20;
					_up = true;
				}
			}

			/*
			**	Set the pulse color as the proportional value between white and the
			**	minimum value for pulsing.
			*/
			InGamePalette[CC_PULSE_COLOR] = GamePalette[WHITE];
			InGamePalette[CC_PULSE_COLOR].Adjust(val, BlackColor);

			/*
			**	Pulse the glowing embers between medium and dark red.
			*/
			InGamePalette[CC_EMBER_COLOR] = RGBClass(255, 80, 80);
			InGamePalette[CC_EMBER_COLOR].Adjust(val, BlackColor);

			changed = true;
		}

		/*
		**	Process the color cycling effects -- water.
		*/
		if (!_timer) {
			_timer = TIMER_SECOND/4;

			RGBClass first = InGamePalette[CYCLE_COLOR_START+CYCLE_COLOR_COUNT-1];
			for (int index = CYCLE_COLOR_START+CYCLE_COLOR_COUNT-1; index >= CYCLE_COLOR_START; index--) {
				InGamePalette[index] = InGamePalette[index-1];
			}
			InGamePalette[CYCLE_COLOR_START] = first;

			changed = true;
		}

		/*
		**	If any of the processing functions changed the palette, then this palette must be
		**	passed to the system.
		*/
		if (changed) {
			BStart(BENCH_PALETTE);
			InGamePalette.Set();
//			Set_Palette(InGamePalette);
			BEnd(BENCH_PALETTE);
		}
	}
}


/***********************************************************************************************
 * Call_Back -- Main game maintenance callback routine.                                        *
 *                                                                                             *
 *    This routine handles all the "real time" processing that needs to                        *
 *    occur. This includes palette fading and sound updating. It needs                         *
 *    to be called as often as possible.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
void Call_Back(void)
{
	/*
	**	Music and speech maintenance
	*/
	if (SampleType) {
		Sound_Callback();
		Theme.AI();
		Speak_AI();
	}

	/*
	**	Network maintenance.
	*/
	if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) {
		IPX_Call_Back();
	}

	/*
	**	Serial game maintenance.
	*/
	if (Session.Type == GAME_NULL_MODEM || ((Session.Type == GAME_MODEM) && Session.ModemService)) {
		//NullModem.Service();		ST - 5/7/2019
	}

#ifdef WOLAPI_INTEGRATION
	//	Wolapi maintenance.
	if( pWolapi )
	{
		if( pWolapi->bInGame )
		{
			if( !pWolapi->bConnectionDown && ::timeGetTime() > pWolapi->dwTimeNextWolapiPump )
			{
				pWolapi->pChat->PumpMessages();
				pWolapi->pNetUtil->PumpMessages();
				pWolapi->dwTimeNextWolapiPump = ::timeGetTime() + WOLAPIPUMPWAIT + 700;		//	Slower pump during games.
				if( pWolapi->bConnectionDown )
				{
					//	Connection to server lost.
					Session.Messages.Add_Message( NULL, 0, TXT_WOL_WOLAPIGONE, PCOLOR_GOLD, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE );
					Sound_Effect( WOLSOUND_LOGOUT );
//	ajw (Wolapi object is now left around, so we can try to send game results.)
//					//	Kill wolapi.
//					pWolapi->UnsetupCOMStuff();
//					delete pWolapi;
//					pWolapi = NULL;
				}
			}
		}
		else
		{
			//	When showing a modal dialog during chat, this pumping is turned on. It's turned off immediately following.
			if( pWolapi->bPump_In_Call_Back && ( ::timeGetTime() > pWolapi->dwTimeNextWolapiPump ) )
			{
				pWolapi->pChat->PumpMessages();
				pWolapi->pNetUtil->PumpMessages();
				pWolapi->dwTimeNextWolapiPump = ::timeGetTime() + WOLAPIPUMPWAIT;
			}
		}
	}
#endif

#if(TEN)
	if (Session.Type == GAME_TEN) {
		TEN_Call_Back();
	}
#endif	// TEN

#if(MPATH)
	if (Session.Type == GAME_MPATH) {
		MPATH_Call_Back();
	}
#endif	// MPATH
}


void IPX_Call_Back(void)
{
#if (0)//PG
	Ipx.Service();

	/*
	** Read packets only if the game is "closed", so we don't steal global
	** messages from the connection dialogs.
	*/
	if (!Session.NetOpen) {
		if (Ipx.Get_Global_Message (&Session.GPacket, &Session.GPacketlen, &Session.GAddress, &Session.GProductID)) {

			if (Session.GProductID == IPXGlobalConnClass::COMMAND_AND_CONQUER0) {

				/*
				**	If this is another player signing off, remove the connection &
				**	mark that player's house as non-human, so the computer will take
				**	it over.
				*/
				if (Session.GPacket.Command == NET_SIGN_OFF) {
					for (int i = 0; i < Ipx.Num_Connections(); i++) {

						int id = Ipx.Connection_ID(i);

						if (Session.GAddress == (*Ipx.Connection_Address(id))) {
							Destroy_Connection(id, 0);
						}
					}
				} else {

					/*
					**	Process a message from another user.
					*/

					if (Session.GPacket.Command == NET_MESSAGE) {
						bool msg_ok = false;

						/*
						** If NetProtect is set, make sure this message came from within
						** this game.
						*/
						if (!Session.NetProtect) {
							msg_ok = true;
						} else {
							if (Session.GPacket.Message.NameCRC ==
								Compute_Name_CRC(Session.GameName)) {
								msg_ok = true;
							} else {
								msg_ok = false;
							}
						}

						if (msg_ok) {
							if (!Session.Messages.Concat_Message(Session.GPacket.Name,
								Session.GPacket.Message.Color,
								Session.GPacket.Message.Buf, Rule.MessageDelay * TICKS_PER_MINUTE)) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (NewUnitsEnabled && !strncmp(Session.GPacket.Message.Buf,"XECRET UNITS ON ",15)) {
		Session.GPacket.Message.Buf[0]='S';
		Enable_Secret_Units();
	}
#endif
								Session.Messages.Add_Message (Session.GPacket.Name,
									Session.GPacket.Message.Color,
									Session.GPacket.Message.Buf,
									Session.GPacket.Message.Color,
									TPF_6PT_GRAD | TPF_USE_GRAD_PAL |
									TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE);

								Sound_Effect(VOC_INCOMING_MESSAGE);
							}

							/*
							**	Tell the map to do a partial update (just to force the messages
							**	to redraw).
							*/
							Map.Flag_To_Redraw(true);

							/*
							**	Save this message in our last-message buffer
							*/
							strcpy(Session.LastMessage, Session.GPacket.Message.Buf);
						}
					} else {
						Process_Global_Packet(&Session.GPacket, &Session.GAddress);
					}
				}
			}
		}
	}
#endif
}


#if(TEN)
void TEN_Call_Back(void)
{
	int id;

	Ten->Service();

	if (Ten->Get_Global_Message (&Session.GPacket, &Session.GPacketlen,
		&Session.TenAddress)) {

		//
		//	If this is another player signing off, remove the connection &
		//	mark that player's house as non-human, so the computer will take
		//	it over.
		//
		if (Session.GPacket.Command == NET_SIGN_OFF) {
			for (int i = 0; i < Ten->Num_Connections(); i++) {

				id = Ten->Connection_ID(i);

				if (Session.TenAddress == Ten->Connection_Address(id)) {
					Destroy_TEN_Connection(id, 0);
				}
			}
		}

		//
		//	Process a message from another user.
		//
		else if (Session.GPacket.Command == NET_MESSAGE) {
			if (!Session.Messages.Concat_Message(Session.GPacket.Name,
				Session.GPacket.Message.Color,
				Session.GPacket.Message.Buf, Rule.MessageDelay * TICKS_PER_MINUTE)) {

				Session.Messages.Add_Message (Session.GPacket.Name,
					Session.GPacket.Message.Color,
					Session.GPacket.Message.Buf,
					Session.GPacket.Message.Color,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL |
					TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE);

				Sound_Effect(VOC_INCOMING_MESSAGE);

				/*
				**	Tell the map to do a partial update (just to force the messages
				**	to redraw).
				*/
				Map.Flag_To_Redraw(true);

				/*
				**	Save this message in our last-message buffer
				*/
				strcpy(Session.LastMessage, Session.GPacket.Message.Buf);
			}
		}
	}
}
#endif	// TEN


#if(MPATH)
void MPATH_Call_Back(void)
{
	int id;

	MPath->Service();

	if (MPath->Get_Global_Message (&Session.GPacket, &Session.GPacketlen,
		&Session.MPathAddress)) {

		//
		//	If this is another player signing off, remove the connection &
		//	mark that player's house as non-human, so the computer will take
		//	it over.
		//
		if (Session.GPacket.Command == NET_SIGN_OFF) {
			for (int i = 0; i < MPath->Num_Connections(); i++) {

				id = MPath->Connection_ID(i);

				if (Session.MPathAddress == MPath->Connection_Address(id)) {
					Destroy_MPATH_Connection(id, 0);
				}
			}
		}

		//
		//	Process a message from another user.
		//
		else if (Session.GPacket.Command == NET_MESSAGE) {
			if (!Session.Messages.Concat_Message(Session.GPacket.Name,
				Session.GPacket.Message.Color,
				Session.GPacket.Message.Buf, Rule.MessageDelay * TICKS_PER_MINUTE)) {

				Session.Messages.Add_Message (Session.GPacket.Name,
					Session.GPacket.Message.Color,
					Session.GPacket.Message.Buf,
					Session.GPacket.Message.Color,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL |
					TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE);

				Sound_Effect(VOC_INCOMING_MESSAGE);

				/*
				**	Tell the map to do a partial update (just to force the messages
				**	to redraw).
				*/
				Map.Flag_To_Redraw(true);

				/*
				**	Save this message in our last-message buffer
				*/
				strcpy(Session.LastMessage, Session.GPacket.Message.Buf);
			}
		}
	}
}
#endif	// MPATH


/***********************************************************************************************
 * Language_Name -- Build filename for current language.                                       *
 *                                                                                             *
 *    This routine attaches a language specific suffix to the base                             *
 *    filename provided. Typical use of this is when loading language                          *
 *    specific files at game initialization time.                                              *
 *                                                                                             *
 * INPUT:   basename -- Base name to append language specific                                  *
 *                      extension to.                                                          *
 *                                                                                             *
 * OUTPUT:  Returns with pointer to completed filename.                                        *
 *                                                                                             *
 * WARNINGS:   The return pointer value is valid only until the next time                      *
 *             this routine is called.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * Language_Name(char const * basename)
{
	static char _fullname[_MAX_FNAME+_MAX_EXT];

	if (!basename) return(NULL);

	sprintf(_fullname, "%s.ENG", basename);
	return(_fullname);
}


/***********************************************************************************************
 * Source_From_Name -- Converts ASCII name into SourceType.                                    *
 *                                                                                             *
 *    This routine is used to convert an ASCII name representing a                             *
 *    SourceType into the actual SourceType value. Typically, this is                          *
 *    used when processing the scenario INI file.                                              *
 *                                                                                             *
 * INPUT:   name  -- The ASCII source name to process.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the SourceType represented by the name                                *
 *          specified.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
SourceType Source_From_Name(char const * name)
{
	if (name) {
		for (SourceType source = SOURCE_FIRST; source < SOURCE_COUNT; source++) {
			if (stricmp(SourceName[source], name) == 0) {
				return(source);
			}
		}
	}
	return(SOURCE_NONE);
}


/***********************************************************************************************
 * Name_From_Source -- retrieves the name for the given SourceType         						  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		source		SourceType to get the name for															  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		name of SourceType																							  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   11/15/1994 BR : Created.                                              						  *
 *=============================================================================================*/
char const * Name_From_Source(SourceType source)
{
	if ((unsigned)source < SOURCE_COUNT) {
		return(SourceName[source]);
	}
	return("None");
}


/***********************************************************************************************
 * Theater_From_Name -- Converts ASCII name into a theater number.                             *
 *                                                                                             *
 *    This routine converts an ASCII representation of a theater and converts it into a        *
 *    matching theater number. If no match was found, then THEATER_NONE is returned.           *
 *                                                                                             *
 * INPUT:   name  -- Pointer to ASCII name to convert.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the name converted into a theater number.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TheaterType Theater_From_Name(char const * name)
{
	TheaterType	index;

	if (name) {
		for (index = THEATER_FIRST; index < THEATER_COUNT; index++) {
			if (stricmp(name, Theaters[index].Name) == 0) {
				return(index);
			}
		}
	}
	return(THEATER_NONE);
}


/***********************************************************************************************
 * KN_To_Facing -- Converts a keyboard input number into a facing value.                       *
 *                                                                                             *
 *    This routine determine which compass direction is represented by the keyboard value      *
 *    provided. It is used for map scrolling and other directional control operations from     *
 *    the keyboard.                                                                            *
 *                                                                                             *
 * INPUT:   input -- The KN number to convert.                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the facing type that the keyboard number represents. If it could      *
 *          not be translated, then FACING_NONE is returned.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
FacingType KN_To_Facing(int input)
{
	input &= ~(KN_ALT_BIT|KN_SHIFT_BIT|KN_CTRL_BIT);
	switch (input) {
		case KN_LEFT:
			return(FACING_W);

		case KN_RIGHT:
			return(FACING_E);

		case KN_UP:
			return(FACING_N);

		case KN_DOWN:
			return(FACING_S);

		case KN_UPLEFT:
			return(FACING_NW);

		case KN_UPRIGHT:
			return(FACING_NE);

		case KN_DOWNLEFT:
			return(FACING_SW);

		case KN_DOWNRIGHT:
			return(FACING_SE);

		default:
			break;
	}
	return(FACING_NONE);
}


/***********************************************************************************************
 * Sync_Delay -- Forces the game into a 15 FPS rate.                                           *
 *                                                                                             *
 *    This routine will wait until the timer for the current frame has expired before          *
 *    returning. It is called at the end of every game loop in order to force the game loop    *
 *    to run at a fixed rate.                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine will delay an amount of time according to the game speed setting.  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/04/1995 JLB : Created.                                                                 *
 *   03/06/1995 JLB : Fixed.                                                                   *
 *=============================================================================================*/
static void Sync_Delay(void)
{
	/*
	**	Accumulate the number of 'spare' ticks that are frittered away here.
	*/
	SpareTicks += FrameTimer;

	/*
	**	Delay until the frame timer expires. This forces the game loop to be regulated to a
	**	speed controlled by the game options slider.
	*/
	while (FrameTimer) {
		Color_Cycle();
		Call_Back();

		if (SpecialDialog == SDLG_NONE) {
#ifdef WIN32
			WWMouse->Erase_Mouse(&HidPage, TRUE);
#endif	//WIN32
			KeyNumType input = KN_NONE;
			int x, y;
			Map.Input(input, x, y);
			if (input) {
				Keyboard_Process(input);
			}
			Map.Render();
		}
	}
	Color_Cycle();
	Call_Back();
}


/***********************************************************************************************
 * Main_Loop -- This is the main game loop (as a single loop).                                 *
 *                                                                                             *
 *    This function will perform one game loop.                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Should the game end?                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef WIN32
extern void Check_For_Focus_Loss(void);
void Reallocate_Big_Shape_Buffer(void);
#endif	//WIN32


bool Main_Loop()
{
	KeyNumType	input;					// Player input.
	int x;
	int y;
	int framedelay;

Mono_Set_Cursor(0,0);

	if (!GameActive) return(!GameActive);

#ifdef WIN32
	/*
	** Call the focus loss handler
	*/
	Check_For_Focus_Loss();

	/*
	** Allocate extra memory for uncompressed shapes as needed
	*/
	Reallocate_Big_Shape_Buffer();
#endif

	/*
	** Sync-bug trapping code
	*/
	if (Frame >= Session.TrapFrame) {
		Session.Trap_Object();
	}

	//
	// Initialize our AI processing timer
	//
	Session.ProcessTimer = TickCount;

#if 1
	if (Session.TrapCheckHeap) {
		Debug_Trap_Check_Heap = true;
	}
#endif

#ifdef CHEAT_KEYS

	/*
	**	Update the running status debug display.
	*/
	Self_Regulate();
#endif

	BStart(BENCH_GAME_FRAME);

	/*
	**	If there is no theme playing, but it looks like one is required, then start one
	**	playing. This is usually the symptom of there being no transition score.
	*/
	if (SampleType && Theme.What_Is_Playing() == THEME_NONE) {
		Theme.Queue_Song(THEME_PICK_ANOTHER);
	}

	/*
	**	Setup the timer so that the Main_Loop function processes at the correct rate.
	*/
	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH &&
		Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {

		//
		// In playback mode, run as fast as possible.
		//
		if (Session.Play) {
			FrameTimer = 0;
		} else {
#ifdef FIXIT_VERSION_3
			if( !Session.DesiredFrameRate )
				Session.DesiredFrameRate = 60;		//	A division by zero was happening (very rare).
#endif
			framedelay = TIMER_SECOND / Session.DesiredFrameRate;
			FrameTimer = framedelay;
		}
	} else {
		if (Options.GameSpeed != 0) {
			FrameTimer = Options.GameSpeed +
				(PlayerPtr->Difficulty == DIFF_EASY ? 1 : 0) -
				(PlayerPtr->Difficulty == DIFF_HARD ? 1 : 0);
		} else {
			FrameTimer = Options.GameSpeed + (PlayerPtr->Difficulty == DIFF_EASY ? 1 : 0);
		}
	}

	/*
	**	Update the display, unless we're inside a dialog.
	*/
	if (!Session.Play) {
#ifdef WIN32
		if (SpecialDialog == SDLG_NONE && GameInFocus) {
			WWMouse->Erase_Mouse(&HidPage, TRUE);
#else
		if (SpecialDialog == SDLG_NONE) {
#endif
			Map.Input(input, x, y);
			if (input) {
				Keyboard_Process(input);
			}
			Map.Render();
		}
	}

	/*
	** Save map's position & selected objects, if we're recording the game.
	*/
	if (Session.Record || Session.Play) {
		Do_Record_Playback();
	}

#ifndef SORTDRAW
	/*
	** Sort the map's ground layer by y-coordinate value.  This is done
	** outside the IsToRedraw check, for the purposes of game sync'ing
	** between machines; this way, all machines will sort the Map's
	** layer in the same way, and any processing done that's based on
	** the order of this layer will remain in sync.
	*/
	DisplayClass::Layer[LAYER_GROUND].Sort();
#endif

	/*
	**	AI logic operations are performed here.
	*/
	Logic.AI();
	TimeQuake = false;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (!PendingTimeQuake) {
		TimeQuakeCenter = 0;
	}
#endif

	/*
	**	Manage the inter-player message list.  If Manage() returns true, it means
	**	a message has expired & been removed, and the entire map must be updated.
	*/
	if (Session.Messages.Manage()) {
#ifdef WIN32
		HiddenPage.Clear();
#else	//WIN32
		HidPage.Clear();
#endif	//WIN32
		Map.Flag_To_Redraw(true);
	}

	//
	// Measure how long it took to process the AI
	//
	Session.ProcessTicks += (TickCount - Session.ProcessTimer);
	Session.ProcessFrames++;

	/*
	**	Process all commands that are ready to be processed.
	*/
	Queue_AI();

	/*
	**	Keep track of elapsed time in the game.
	*/
	Score.ElapsedTime += TIMER_SECOND / TICKS_PER_SECOND;

	Call_Back();

	/*
	**	Check for player wins or loses according to global event flag.
	*/
	if (PlayerWins) {

#ifdef WIN32

		/*
		** Send the game statistics to WChat.
		*/
		if (Session.Type == GAME_INTERNET && !GameStatisticsPacketSent) {
			Register_Game_End_Time();
			Send_Statistics_Packet();		//	Player just won.
		}

		WWMouse->Erase_Mouse(&HidPage, TRUE);
#endif	//WIN32
		PlayerLoses = false;
		PlayerWins = false;
		PlayerRestarts = false;
		Map.Help_Text(TXT_NONE);
		Do_Win();
		return(!GameActive);
	}
	if (PlayerLoses) {
#ifdef WIN32
		/*
		** Send the game statistics to WChat.
		*/
		if (Session.Type == GAME_INTERNET && !GameStatisticsPacketSent) {
			Register_Game_End_Time();
			Send_Statistics_Packet();		//	Player just lost.
		}

		WWMouse->Erase_Mouse(&HidPage, TRUE);
#endif	//WIN32
		PlayerWins = false;
		PlayerLoses = false;
		PlayerRestarts = false;
		Map.Help_Text(TXT_NONE);
		Do_Lose();
		return(!GameActive);
	}
	if (PlayerRestarts) {
#ifdef WIN32
		WWMouse->Erase_Mouse(&HidPage, TRUE);
#endif	//WIN32
		PlayerWins = false;
		PlayerLoses = false;
		PlayerRestarts = false;
		Map.Help_Text(TXT_NONE);
		Do_Restart();
		return(!GameActive);
	}

#ifdef FIXIT_VERSION_3		//	Stalemate games.
	if( Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && Session.Players.Count() == 2 &&
		Scen.bLocalProposesDraw && Scen.bOtherProposesDraw )
	{
		//	End game in a draw.
		if (Session.Type == GAME_INTERNET && !GameStatisticsPacketSent) {
			Register_Game_End_Time();
			Send_Statistics_Packet();
		}
		WWMouse->Erase_Mouse(&HidPage, TRUE);
		Map.Help_Text(TXT_NONE);
		Do_Draw();
		return(!GameActive);
	}
#endif

	/*
	**	The frame logic has been completed. Increment the frame
	**	counter.
	*/
	Frame++;

	/*
	** Is there a memory trasher altering the map??
	*/
	if (Debug_Check_Map) {
		if (!Map.Validate()) {
			if (WWMessageBox().Process (TEXT_MAP_ERROR, TEXT_STOP, TEXT_CONTINUE)==0) {
				GameActive = 0;
			}
			Map.Validate();		// give debugger a chance to catch it
		}
	}


#if (0)	//ST - 5/8/2019
#ifdef WIN32
	if (Debug_MotionCapture) {
		static void ** _array = 0;
		static int _sequence = 0;
		static int _seqsize = Rule.MovieTime * TICKS_PER_MINUTE;

		if (_array == NULL) {
			_array = new void * [_seqsize];
			memset(_array, '\0', _seqsize * sizeof(void*));
		}

		if (_array == NULL) {
			Debug_MotionCapture = false;
		}

		static GraphicBufferClass temp_page(	SeenBuff.Get_Width(),
													SeenBuff.Get_Height(),
													NULL,
													SeenBuff.Get_Width() * SeenBuff.Get_Height());

		int size = SeenBuff.Get_Width() * SeenBuff.Get_Height();

		if (_sequence < _seqsize) {
			if (_array[_sequence] == NULL) {
				_array[_sequence] = new char[size];
			}

			if (_array[_sequence] != NULL) {
				SeenBuff.Blit(temp_page);
				memmove(_array[_sequence], temp_page.Get_Buffer(), size);
			}
			_sequence++;

		} else {
			Debug_MotionCapture = false;

			CDFileClass file;
			file.Cache(200000);
			char filename[30];

			for (int index = 0; index < _sequence; index++) {
				memmove(temp_page.Get_Buffer(), _array[index], size);
				sprintf(filename, "cap%04d.pcx", index);
				file.Set_Name(filename);

				Write_PCX_File(file, temp_page, & GamePalette);
			}

			_sequence = 0;
		}
	}
#endif
#endif
	BEnd(BENCH_GAME_FRAME);

	Sync_Delay();
	return(!GameActive);
}


#ifdef SCENARIO_EDITOR
/***************************************************************************
 * Map_Edit_Loop -- a mini-main loop for map edit mode only                *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   10/19/1994 BR : Created.                                              *
 *=========================================================================*/
bool Map_Edit_Loop(void)
{
	/*
	**	Redraw the map.
	*/
	Map.Render();

	/*
	**	Get user input (keys, mouse clicks).
	*/
	KeyNumType input;

#ifdef WIN32
	WWMouse->Erase_Mouse(&HidPage, TRUE);
#endif	//WIN32

	int x;
	int y;
	Map.Input(input, x, y);

	/*
	**	Process keypress.
	*/
	if (input) {
		Keyboard_Process(input);
	}

	Call_Back();								// maintains Theme.AI() for music
	Color_Cycle();

	return(!GameActive);
}


/***************************************************************************
 * Go_Editor -- Enables/disables the map editor										*
 *                                                                         *
 * INPUT:                                                                  *
 *		flag		true = go into editor mode; false = go into game mode			*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/19/1994 BR : Created.                                              *
 *=========================================================================*/
void Go_Editor(bool flag)
{
	/*
	**	Go into Scenario Editor mode
	*/
	if (flag) {
		Debug_Map = true;
		Debug_Unshroud = true;

		/*
		** Un-select any selected objects
		*/
		Unselect_All();

		/*
		** Turn off the sidebar if it's on
		*/
		Map.Activate(0);

		/*
		** Reset the map's Button list for the new mode
		*/
		Map.Init_IO();

		/*
		** Force a complete redraw of the screen
		*/
#ifdef WIN32
		HiddenPage.Clear();
#else
		HidPage.Clear();
#endif
		Map.Flag_To_Redraw(true);
		Map.Render();

	} else {

		/*
		**	Go into normal game mode
		*/
		Debug_Map = false;
		Debug_Unshroud = false;

		/*
		** Un-select any selected objects
		*/
		Unselect_All();

		/*
		** Reset the map's Button list for the new mode
		*/
		Map.Init_IO();

		/*
		** Force a complete redraw of the screen
		*/
		HidPage.Clear();
		Map.Flag_To_Redraw(true);
		Map.Render();
	}
}

#endif

#if (0)
/***********************************************************************************************
 * MixFileHandler -- Handles VQ file access.                                                   *
 *                                                                                             *
 *    This routine is called from the VQ player when it needs to access the source file. By    *
 *    using this routine it is possible to virtualize the file system.                         *
 *                                                                                             *
 * INPUT:   vqa   -- Pointer to the VQA handle for this animation.                             *
 *                                                                                             *
 *          action-- The requested action to perform.                                          *
 *                                                                                             *
 *          buffer-- Optional buffer pointer as needed by the type of action.                  *
 *                                                                                             *
 *          nbytes-- The number of bytes (if needed) for this operation.                       *
 *                                                                                             *
 * OUTPUT:  Returns a value consistent with the action requested.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
long MixFileHandler(VQAHandle * vqa, long action, void * buffer, long nbytes)
{
	CCFileClass * file;
	long        error;

	file = (CCFileClass *)vqa->VQAio;

	/*
	**	Perform the action specified by the stream command.
	*/
	switch (action) {

		/*
		** VQACMD_READ means read NBytes from the stream and place it in the
		** memory pointed to by Buffer.
		**
		** Any error code returned will be remapped by VQA library into
		** VQAERR_READ.
		*/
		case VQACMD_READ:
			error = (file->Read(buffer, (unsigned short)nbytes) != (unsigned short)nbytes);
			break;

		/*
		**	VQACMD_WRITE is analogous to VQACMD_READ.
		**
		** Writing is not allowed to the VQA file, VQA library will remap the
		** error into VQAERR_WRITE.
		*/
		case VQACMD_WRITE:
			error = 1;
			break;

		/*
		**	VQACMD_SEEK asks that you perform a seek relative to the current
		** position. NBytes is a signed number, indicating seek direction
		** (positive for forward, negative for backward). Buffer has no meaning
		** here.
		**
		** Any error code returned will be remapped by VQA library into
		** VQAERR_SEEK.
		*/
		case VQACMD_SEEK:
			error = (file->Seek(nbytes, SEEK_CUR) == -1);
			break;

		/*
		**	VQACMD_OPEN asks that you open your stream for access.
		*/
		case VQACMD_OPEN:
			file = new CCFileClass((char *)buffer);

			if (file != NULL && file->Is_Available()) {
				error = file->Open((char *)buffer, READ);

				if (error != -1) {
					vqa->VQAio = (unsigned long)file;
					error = 0;
				} else {
					delete file;
					file = 0;
					error = 1;
				}
			} else {
				error = 1;
			}
			break;

		case VQACMD_CLOSE:
			file->Close();
			delete file;
			file = 0;
			vqa->VQAio = 0;
			error = 0;
			break;

		/*
		**	VQACMD_INIT means to prepare your stream for reading. This is used for
		** certain streams that can't be read immediately upon opening, and need
		** further preparation. This operation is allowed to fail; the error code
		** will be returned directly to the client.
		*/
		case VQACMD_INIT:

		/*
		**	IFFCMD_CLEANUP means to terminate the transaction with the associated
		** stream. This is used for streams that can't simply be closed. This
		** operation is not allowed to fail; any error returned will be ignored.
		*/
		case VQACMD_CLEANUP:
			error = 0;
			break;

		default:
			error = 0;
			break;
	}

	return(error);
}
#endif

void Rebuild_Interpolated_Palette(unsigned char * interpal)
{
	for (int y=0; y<255; y++) {
		for (int x=y+1; x<256; x++) {
			*(interpal + (y*256+x)) = *(interpal + (x*256+y));
		}
	}
}


unsigned char 	* InterpolatedPalettes[100];
BOOL				PalettesRead;
unsigned			PaletteCounter;


/***********************************************************************************************
 * Load_Interpolated_Palettes -- Loads in any precalculated palettes for hires VQs             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Name of palette file                                                              *
 *                                                                                             *
 * OUTPUT:   Number of palettes loaded                                                         *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/7/96 9:49AM ST : Created                                                               *
 *=============================================================================================*/
int Load_Interpolated_Palettes(char const * filename, BOOL add)
{
	int	num_palettes=0;
	int	i;
	int 	start_palette;

	PalettesRead = FALSE;
	CCFileClass	file(filename);

	if (!add) {
		for (i=0; i < ARRAY_SIZE(InterpolatedPalettes); i++) {
			InterpolatedPalettes[i]=NULL;
		}
		start_palette=0;
	} else {
		for (start_palette = 0; start_palette < ARRAY_SIZE(InterpolatedPalettes); start_palette++) {
			if (!InterpolatedPalettes[start_palette]) break;
		}
	}

	/*
	**	Hack another interpolated palette if the requested one is
	**	not present.
	*/
	if (!file.Is_Available()) {
		file.Set_Name("AAGUN.VQP");
	}

	if (file.Is_Available()) {

		file.Open(READ);
		file.Read(&num_palettes , 4);

		for (i=0; i < num_palettes; i++) {
			InterpolatedPalettes[i+start_palette] = (unsigned char *)malloc (65536);
			memset (InterpolatedPalettes[i+start_palette], 0, 65536);
			for (int y = 0; y < 256; y++) {
				file.Read (InterpolatedPalettes[i+start_palette] + y*256 , y+1);
			}

			Rebuild_Interpolated_Palette(InterpolatedPalettes[i+start_palette]);
		}

		PalettesRead = TRUE;
		file.Close();
	}
	PaletteCounter = 0;
	return (num_palettes);
}


void Free_Interpolated_Palettes(void)
{
	for (int i = 0; i < ARRAY_SIZE(InterpolatedPalettes) ;i++) {
		if (InterpolatedPalettes[i]) {
			free(InterpolatedPalettes[i]);
			InterpolatedPalettes[i]=NULL;
		}
	}
}


/***********************************************************************************************
 * Play_Movie -- Plays a VQ movie.                                                             *
 *                                                                                             *
 *    Use this routine to play a VQ movie. It will dispatch the specified movie to the         *
 *    VQ player. The routine will not return until the movie has finished playing.             *
 *                                                                                             *
 * INPUT:   name  -- The name of the movie file (sans ".VQA").                                 *
 *                                                                                             *
 *          theme -- The identifier for an optional theme that should be played in the         *
 *                   background while this VQ plays.                                           *
 *                                                                                             *
 *          clrscrn -- 'true' if to clear the screen when the movie is over                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef WIN32
extern void Suspend_Audio_Thread(void);
extern void Resume_Audio_Thread(void);

#ifdef MOVIE640
extern GraphicBufferClass VQ640;
#endif
#endif

extern void Play_Movie_GlyphX(const char * movie_name, ThemeType theme, bool immediate);

void Play_Movie(char const * name, ThemeType theme, bool clrscrn, bool immediate)
	{
#if (1)
	if (strcmp(name, "x") == 0 || strcmp(name, "X") == 0) {
		return;
	}

	Play_Movie_GlyphX(name, theme, immediate);
	return;
#else
	#ifdef MPEGMOVIE
	//theme = theme;
	//clrscrn = clrscrn;
	if( Using_DVD() )
	{
		if (PlayMpegMovie(name))
			return;
	}
	#endif

	#ifdef CHEAT_KEYS
	//	Mono_Printf("Movie: %s\n", name);
	#endif	//CHEAT_KEYS
	/*
	** Don't play movies in editor mode
	*/
	if (Debug_Map) {
		return;
	}
	#ifdef CHEAT_KEYS
	//	Mono_Printf("A\n");
	#endif	//CHEAT_KEYS
	/*
	** Don't play movies in multiplayer mode
	*/
	if (Session.Type != GAME_NORMAL) {
		return;
	}
	#ifdef CHEAT_KEYS
	//Mono_Printf("b\n");
	#endif	//CHEAT_KEYS

	if (name) {
		char fullname[_MAX_FNAME+_MAX_EXT];
		_makepath(fullname, NULL, NULL, name, ".VQA");
		#ifdef WIN32
		char palname [_MAX_FNAME+_MAX_EXT];
		_makepath(palname , NULL, NULL, name, ".VQP");
		#endif	//WIN32
		#ifdef CHEAT_KEYS
		//			Mono_Set_Cursor(0, 0);Mono_Printf("[%s]", fullname);
		#endif

		if (!CCFileClass(fullname).Is_Available()){
			#ifdef CHEAT_KEYS
			//		 Mono_Printf("fullname: %s\n", fullname);
			#endif	//CHEAT_KEYS
		 return;
		}
		/*
		**	Reset the anim control structure.
		*/
		Anim_Init();

		/*
		**	Prepare to play a movie. First hide the mouse and stop any score that is playing.
		**	While the score (if any) is fading to silence, fade the palette to black as well.
		**	When the palette has finished fading, wait until the score has finished fading
		**	before launching the movie.
		*/
		Hide_Mouse();
		Theme.Queue_Song(theme);
		if (PreserveVQAScreen == 0 && !clrscrn) {
			BlackPalette.Set(FADE_PALETTE_MEDIUM);
			VisiblePage.Clear();
			BlackPalette.Adjust(0x08, WhitePalette);
			BlackPalette.Set();
			BlackPalette.Adjust(0xFF);
			BlackPalette.Set();
		}
		PreserveVQAScreen = 0;
		Keyboard->Clear();

		VQAHandle * vqa = NULL;


		#ifdef WIN32
		#ifdef MOVIE640
		if(IsVQ640) {
			AnimControl.ImageWidth = 640;
			AnimControl.ImageHeight = 400;
			AnimControl.ImageBuf = (unsigned char *)VQ640.Get_Offset();
		} else {
			AnimControl.ImageWidth = 320;
			AnimControl.ImageHeight = 200;
			AnimControl.ImageBuf = (unsigned char *)SysMemPage.Get_Offset();
		}
		#endif
		#endif

		if (!Debug_Quiet && Get_Digi_Handle() != -1) {
			AnimControl.OptionFlags |= VQAOPTF_AUDIO;
		} else {
			AnimControl.OptionFlags &= ~VQAOPTF_AUDIO;
		}

		if ((vqa = VQA_Alloc()) != NULL) {
			VQA_Init(vqa, MixFileHandler);

			if (VQA_Open(vqa, fullname, &AnimControl) == 0) {
				Brokeout = false;
				#ifdef WIN32
				//Suspend_Audio_Thread();
				#ifdef MOVIE640
				if(!IsVQ640) {
					Load_Interpolated_Palettes(palname);
				}
				#else
				Load_Interpolated_Palettes(palname);
				#endif
				//Set_Palette(BlackPalette);
				SysMemPage.Clear();
				InMovie = true;
				#endif	//WIN32
				VQA_Play(vqa, VQAMODE_RUN);
				VQA_Close(vqa);
				#ifdef WIN32
				//Resume_Audio_Thread();
				InMovie = FALSE;
				#ifdef MOVIE640
				if(!IsVQ640) {
					Free_Interpolated_Palettes();
				}
				#else
				Free_Interpolated_Palettes();
				#endif
				IsVQ640 = false;
				Set_Primary_Buffer_Format();
				#endif	//WIN32

				/*
				**	Any movie that ends prematurely must have the screen
				**	cleared to avoid any unexpected palette glitches.
				*/
				if (Brokeout) {
					clrscrn = true;
					VisiblePage.Clear();
					Brokeout = false;
				}
			} else {
				#ifndef NDEBUG
				bool error = true;
				assert(error);
				#endif
			}

			VQA_Free(vqa);
		} else {
			assert(vqa != NULL);
		}
		#ifdef CHEAT_KEYS
		//Mono_Printf("d");
		#endif	//CHEAT_KEYS
		/*
		**	Presume that the screen is left in a garbage state as well as the palette
		**	being in an unknown condition. Recover from this by clearing the screen and
		**	forcing the palette to black.
		*/
		if (clrscrn) {
			VisiblePage.Clear();
			BlackPalette.Adjust(0x08, WhitePalette);
			BlackPalette.Set();
			BlackPalette.Adjust(0xFF);
			BlackPalette.Set();
		}
		Show_Mouse();
	}
#endif
}


void Play_Movie(VQType name, ThemeType theme, bool clrscrn, bool immediate)
{
	if (name != VQ_NONE) {
		if (name == VQ_REDINTRO) {
			IsVQ640 = true;
		}
		Play_Movie(VQName[name], theme, clrscrn, immediate);
		IsVQ640 = false;
	}
}


// Denzil 5/18/98 - Mpeg movie playback
#ifdef MPEGMOVIE
extern LPDIRECTDRAWPALETTE PalettePtr;

bool PlayMpegMovie(const char* name)
	{
	char path[MAX_PATH];
	CCFileClass file;
	const char* filename;

#ifdef CHEAT_KEYS
	if( bNoMovies )
		return true;
#endif

	sprintf(path, "movies\\%.8s.%.3s", name, "mpg");
	filename = file.Set_Name(path);
	
	if (!file.Is_Available())
		{
		#if(1)
		VisiblePage.Clear();
		GamePalette.Set();
		Show_Mouse();
		sprintf(path, "Couldn't find %s\n", filename);
		WWMessageBox().Process(path);
		#endif
		return false;
		}

	// Stop theme music
	if (Misc_Focus_Loss_Function)
		Misc_Focus_Loss_Function();

	// Release primary surface
	VisiblePage.Un_Init();

	#ifdef MCIMPEG
	if (MciMovie && MpgSettings && (MpgSettings->GetDeviceName() != NULL))
		{
		DirectDrawObject->SetCooperativeLevel(MainWindow, DDSCL_NORMAL);

		if (!MciMovie->Open(filename, MpgSettings->GetDeviceName()))
			{
			WWMessageBox().Process("Couldn't open movie.\n");
			}
		else if (!MciMovie->Play(MainWindow))
			{
			WWMessageBox().Process("Couldn't play movie.\n");
			}

		DirectDrawObject->SetCooperativeLevel(MainWindow, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
		}
	else
	#endif
		{
		DDSURFACEDESC ddsd;
		IDirectDrawSurface* primary = NULL;
		bool modeChange = false;
		RECT rect;
	
		if (FAILED(DirectDrawObject->SetDisplayMode(ScreenWidth, ScreenHeight, 16)))
			{
			WWMessageBox().Process("Couldn't change display mode.\n");
			}
		else
			{
			// Create primary surface reference
			memset(&ddsd, 0, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);
			ddsd.dwFlags = DDSD_CAPS;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

			if (FAILED(DirectDrawObject->CreateSurface(&ddsd, &primary, NULL)))
				{
				WWMessageBox().Process("Couldn't create primary movie surface.\n");
				}
			else
				{
				rect.top = rect.left = 0;
				rect.bottom = ScreenHeight;
				rect.right = ScreenWidth;
		
				MpgSetCallback(MpegCallback, NULL);
				MpgPlay(filename, DirectDrawObject, primary, &rect);

				if (primary)
					primary->Release();

				}

			DirectDrawObject->SetDisplayMode(ScreenWidth, ScreenHeight, 8);
			}
		}

	// Restore surfaces
	VisiblePage.Init(ScreenWidth, ScreenHeight, NULL, 0, (GBC_Enum)(GBC_VISIBLE|GBC_VIDEOMEM));
	PaletteSurface->SetPalette(PalettePtr);
	AllSurfaces.Set_Surface_Focus(true);
	AllSurfaces.Restore_Surfaces();
	return true;
	}


MPG_RESPONSE far __stdcall MpegCallback(MPG_CMD cmd, LPVOID data, LPVOID user)
	{
	static IDirectDrawPalette* _palette = NULL;

	user = user;

	switch (cmd)
		{
		case MPGCMD_ERROR:
			WWMessageBox().Process((char const *)data);
		break;

		case MPGCMD_INIT:
			VisiblePage.Clear();
		break;

		case MPGCMD_CLEANUP:
			VisiblePage.Clear();

			if (_palette != NULL)
				{
				PaletteSurface->SetPalette(_palette);
				_palette->Release();
				_palette = NULL;
				}
		break;
		
		case MPGCMD_PALETTE:
			if (FAILED(PaletteSurface->GetPalette(&_palette)))
				{
				WWMessageBox().Process("Couldn't get primary palette.\n");
				}
			else
				{
				if (FAILED(PaletteSurface->SetPalette((IDirectDrawPalette*)data)))
					{
					WWMessageBox().Process("Couldn't set movie palette.\n");
					}
				}
		break;
		
		case MPGCMD_UPDATE:
			if ((BreakoutAllowed || Debug_Flag) && Keyboard->Check())
				{
				if (Keyboard->Get() == KN_ESC)
					{
					Keyboard->Clear();
					return MPGRES_QUIT;
					}

				Keyboard->Clear();
				}

			if (!GameInFocus)
				{
				MpgPause();

				while (!GameInFocus)
					{
					Check_For_Focus_Loss();
					}

				MpgResume();
				return MPGRES_LOSTFOCUS;
				}
		break;

		default:
		break;
		}

	return MPGRES_CONTINUE;
	}
#endif

/***********************************************************************************************
 * Unselect_All -- Causes all selected objects to become unselected.                           *
 *                                                                                             *
 *    This routine will unselect all objects that are currently selected.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Unselect_All(void)
{
	//while (CurrentObject.Count()) {
	//	CurrentObject[0]->Unselect();
	//}

	//Added some error handling incase there was an issue removing the object - JAS 6/28/2019
	while (CurrentObject.Count()) {

		int count_before = CurrentObject.Count();
		CurrentObject[0]->Unselect();

		if (count_before <= CurrentObject.Count()) {
			GlyphX_Debug_Print("Unselect_All failed to remove an object");
			CurrentObject.Delete(CurrentObject[0]);
		}
	}
	//End of change - JAS 6/28/2019

}


void Unselect_All_Except(ObjectClass* object)
{
	int index = 0;
	while (index < CurrentObject.Count()) {

		if (CurrentObject[index] == object) {
			index++;
			continue;
		}

		int count_before = CurrentObject.Count();
		CurrentObject[index]->Unselect();

		if (count_before <= CurrentObject.Count()) {
			GlyphX_Debug_Print("Unselect_All failed to remove an object");
			CurrentObject.Delete(CurrentObject[index]);
		}
	}
}


/***********************************************************************************************
 * Fading_Table_Name -- Builds a theater specific fading table name.                           *
 *                                                                                             *
 *    This routine builds a standard fading table name. This name is dependant on the theater  *
 *    being played, since each theater has its own palette.                                    *
 *                                                                                             *
 * INPUT:   base  -- The base name of this fading table. The base name can be no longer than   *
 *                   seven characters.                                                         *
 *                                                                                             *
 *          theater  -- The theater that this fading table is specific to.                     *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the constructed fading table filename. This pointer is   *
 *          valid until this function is called again.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * Fading_Table_Name(char const * base, TheaterType theater)
{
	static char _buffer[_MAX_FNAME+_MAX_EXT];
	char root[_MAX_FNAME];

	sprintf(root, "%1.1s%s", Theaters[theater].Root, base);
	_makepath(_buffer, NULL, NULL, root, ".MRF");
	return(_buffer);
}


/***********************************************************************************************
 * Get_Radar_Icon -- Builds and alloc a radar icon from a shape file                           *
 *                                                                                             *
 * INPUT:      void const * shapefile - pointer to a key framed shapefile                      *
 *             int shapenum          - shape to extract from shapefile                         *
 *                                                                                             *
 * OUTPUT:     void const *           - 3/3 icon set of shape from file                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/12/1995 PWG : Created.                                                                 *
 *   05/10/1995 JLB : Handles a null shapefile pointer.                                        *
 *=============================================================================================*/
void const * Get_Radar_Icon(void const * shapefile, int shapenum, int frames, int zoomfactor)
{
	static int _offx[]={	0, 0,  -1,  1, 0, -1, 1, -1, 1};
	static int _offy[]={	0, 0,  -1,  1, 0, -1, 1, -1, 1};
	int lp,framelp;
	char pixel;

	char * retval = NULL;
	char * buffer = NULL;

	/*
	**	If there is no shape file, then there can be no radar icon imagery.
	*/
	if (!shapefile) return(NULL);

#if (0)
	CCPalette.Set();
	Set_Logic_Page(SeenBuff);
	CC_Draw_Shape(shapefile, shapenum, 64, 64, WINDOW_MAIN, SHAPE_WIN_REL);
#endif

	/*
	** Get the pixel width and height of the frame we built.  This will
	** be used to extract icons and build pixels.
	*/
	int pixel_width  = Get_Build_Frame_Width( shapefile );
	int pixel_height = Get_Build_Frame_Height( shapefile );

	/*
	** Find the width and height in icons, adjust these by half an
	** icon because the artists may be sloppy and miss the edge of an
	** icon one way or the other.
	*/
	int icon_width	 = (pixel_width + 12) / 24;
	int icon_height = (pixel_height + 12) / 24;

	/*
	** If we have been told to build as many frames as possible, then
	** find out how many frames there are to build.
	*/
	if (frames == -1) frames = Get_Build_Frame_Count( shapefile );

	/*
	** Allocate a position to store our icons.  If the alloc fails then
	** we don't add these icons to the set.
	**/
	buffer = new char[(icon_width * icon_height * 9 * frames)+2];
	if (!buffer) return(NULL);

	/*
	** Save off the return value so that we can return it to the calling
	** function.
	*/
	retval	 = (char *)buffer;
	*buffer++ = (char)icon_width;
	*buffer++ = (char)icon_height;
	int val = 24/zoomfactor;

	for (framelp = 0; framelp < frames; framelp ++) {
		/*
		** Build the current frame.  If the frame can not be built then we
		** just need to skip past this set of icons and try to build the
		** next frame.
		*/
#ifdef WIN32
		void * ptr;
		if ((ptr = (void *)(Build_Frame(shapefile, shapenum + framelp, SysMemPage.Get_Buffer()))) != NULL) {
			ptr = Get_Shape_Header_Data(ptr);
#else	//WIN#@
		if (Build_Frame(shapefile, shapenum + framelp, HidPage.Get_Buffer()) <= (unsigned long)HidPage.Get_Size() ) {
#endif	//WIN32

			/*
			** Loop through the icon width and the icon height building icons
			** into the buffer pointer.  When the getx or gety falls outside of
			** the width and height of the shape, just insert transparent pixels.
			*/
			for (int icony = 0; icony < icon_height; icony ++) {
				for (int iconx = 0; iconx < icon_width; iconx ++) {
#ifdef WIN32

					for (int y = 0; y < zoomfactor; y++) {
						for (int x = 0; x < zoomfactor; x++) {
							int getx = (iconx * 24) + (x * val) + (zoomfactor / 2);
							int gety = (icony * 24) + (y * val) + (zoomfactor / 2);
							if ((getx < pixel_width) && (gety < pixel_height)) {
								for (lp = 0; lp < 9; lp ++) {
									pixel	= *(char *)((char *)ptr + ((gety - _offy[lp]) * pixel_width) + getx-_offx[lp]);

#else	//WIN32
					for (int y = 0; y < 3; y++) {
						for (int x = 0; x < 3; x++) {
							int getx = (iconx * 24) + (x << 3) + 4;
							int gety = (icony * 24) + (y << 3) + 4;
							if ((getx < pixel_width) && (gety < pixel_height)) {
								for (lp = 0; lp < 9; lp ++) {
									pixel	= *(char *)((char *)HidPage.Get_Buffer(), ((gety - _offy[lp]) * pixel_width) + getx-_offx[lp]);
#endif	//WIN32
									if (pixel == LTGREEN) pixel = 0;
									if (pixel) {
										break;
									}
								}
								*buffer++ = pixel;
							} else {
								*buffer++ = 0;
							}
						}
					}
				}
			}
		} else {
			buffer += icon_width * icon_height * 9;
		}
	}
	return(retval);
}


extern void DLL_Draw_Intercept(int shape_number, int x, int y, int width, int height, int flags, const ObjectClass *object, DirType rotation, long scale, const char *shape_file_name, char override_owner);
extern void DLL_Draw_Pip_Intercept(const ObjectClass* object, int pip);
void CC_Draw_Shape(void const * shapefile, int shapenum, int x, int y, WindowNumberType window, ShapeFlags_Type flags, void const * fadingdata, void const * ghostdata, DirType rotation);

void CC_Draw_Shape(const ObjectClass *object, void const * shapefile, int shapenum, int x, int y, WindowNumberType window, ShapeFlags_Type flags, void const * fadingdata, void const * ghostdata, DirType rotation, long virtualscale)
{
	if (window == WINDOW_VIRTUAL) {
		int width = Get_Build_Frame_Width(shapefile);
		int height = Get_Build_Frame_Height(shapefile);
		DLL_Draw_Intercept(shapenum, x, y, width, height, (int)flags, object, rotation, virtualscale, NULL, HOUSE_NONE);
		return;
	}

	CC_Draw_Shape(shapefile, shapenum, x, y, window, flags, fadingdata, ghostdata, rotation);
}

void CC_Draw_Shape(const ObjectClass *object, const char *shape_file_name, void const * shapefile, int shapenum, int x, int y, WindowNumberType window, ShapeFlags_Type flags, void const * fadingdata, void const * ghostdata, DirType rotation, long virtualscale, char override_owner)
{
	if (window == WINDOW_VIRTUAL) {
		int width = Get_Build_Frame_Width(shapefile);
		int height = Get_Build_Frame_Height(shapefile);
		DLL_Draw_Intercept(shapenum, x, y, width, height, (int)flags, object, rotation, virtualscale, shape_file_name, override_owner);
		return;
	}

	CC_Draw_Shape(shapefile, shapenum, x, y, window, flags, fadingdata, ghostdata, rotation);
}


void CC_Draw_Pip(const ObjectClass *object, void const * shapefile, int shapenum, int x, int y, WindowNumberType window, ShapeFlags_Type flags, void const * fadingdata, void const * ghostdata, DirType rotation)
{
	if (window == WINDOW_VIRTUAL) {
		DLL_Draw_Pip_Intercept(object, shapenum);
		return;
	}

	CC_Draw_Shape(shapefile, shapenum, x, y, window, flags, fadingdata, ghostdata, rotation);
}


/***********************************************************************************************
 * CC_Draw_Shape -- Custom draw shape handler.                                                 *
 *                                                                                             *
 *    All draw shape calls will route through this function. It handles all draws for          *
 *    C&C. Such draws always occur to the logical page and assume certain things about         *
 *    the parameters passed.                                                                   *
 *                                                                                             *
 * INPUT:   shapefile   -- Pointer to the shape data file. This data file contains all the     *
 *                         embedded shapes.                                                    *
 *                                                                                             *
 *          shapenum    -- The shape number within the shapefile that will be drawn.           *
 *                                                                                             *
 *          x,y         -- The pixel coordinates to draw the shape.                            *
 *                                                                                             *
 *          window      -- The clipping window to use.                                         *
 *                                                                                             *
 *          flags       -- The custom draw shape flags. This controls how the parameters       *
 *                         are used (if any).                                                  *
 *                                                                                             *
 *          fadingdata  -- If SHAPE_FADING is desired, then this points to the fading          *
 *                         data table.                                                         *
 *                                                                                             *
 *          ghostdata   -- If SHAPE_GHOST is desired, then this points to the ghost remap      *
 *                         table.                                                              *
 *                                                                                             *
 *          rotation    -- Rotation to apply to the shape (DIR_N = no rotation at all).        *
 *                                                                                             *
 *          scale       -- 24.8 fixed point scale factor.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void CC_Draw_Shape(void const * shapefile, int shapenum, int x, int y, WindowNumberType window, ShapeFlags_Type flags, void const * fadingdata, void const * ghostdata, DirType rotation)
{
	int predoffset;
#ifdef WIN32
	unsigned	long	shape_pointer;
#endif	//WIN32


	/*
	** Special kludge for E3 to prevent crashes
	*/
	if ((flags & SHAPE_GHOST) && (!ghostdata)) {
		ghostdata = DisplayClass::SpecialGhost;
	}
	if ((flags & SHAPE_FADING) && (!fadingdata)) {
		fadingdata = DisplayClass::FadingShade;
	}

	static unsigned char * _xbuffer = 0;

	if (!_xbuffer) {
		_xbuffer = new unsigned char[SHAPE_BUFFER_SIZE];
	}

	if (shapefile != NULL && shapenum != -1) {

		int width = Get_Build_Frame_Width(shapefile);
		int height = Get_Build_Frame_Height(shapefile);

#ifdef NEVER
		/*
		**	Perform a quick clip check against the destination rectangle.
		*/
		if (flags & SHAPE_CENTER) {
			if (x-width/2 >= WindowList[window][WINDOWWIDTH]) return;
			if (y-width/2 >= WindowList[window][WINDOWHEIGHT]) return;
			if (x+width/2 < 0) return;
			if (y+height/2 < 0) return;

		} else {
			if (x >= WindowList[window][WINDOWWIDTH]) return;
			if (y >= WindowList[window][WINDOWHEIGHT]) return;
			if (x+width < 0) return;
			if (y+height < 0) return;
		}
#endif


#ifdef WIN32
		/*
		** In WIn95, build shape returns a pointer to the shape not its size
		*/
		shape_pointer = Build_Frame(shapefile, shapenum, _ShapeBuffer);
		if (shape_pointer) {
			GraphicViewPortClass draw_window(LogicPage->Get_Graphic_Buffer(),
														WindowList[window][WINDOWX] + LogicPage->Get_XPos(),
														WindowList[window][WINDOWY] + LogicPage->Get_YPos(),
														WindowList[window][WINDOWWIDTH],
														WindowList[window][WINDOWHEIGHT]);
			unsigned char * buffer = (unsigned char *) shape_pointer;	//Get_Shape_Header_Data((void*)shape_pointer);

#else	//WIN32
		if ( Build_Frame(shapefile, shapenum, _ShapeBuffer ) <= (unsigned long)_ShapeBufferSize) {
			GraphicViewPortClass draw_window(LogicPage,
														WindowList[window][WINDOWX],
														WindowList[window][WINDOWY],
														WindowList[window][WINDOWWIDTH],
														WindowList[window][WINDOWHEIGHT]);
			unsigned char * buffer = (unsigned char *)_ShapeBuffer;
#endif	//WIN32

			UseOldShapeDraw = false;
			/*
			**	Rotation handler.
			*/
			if (rotation != DIR_N) {

				/*
				** Get the raw shape data without the new header and flag to use the old shape drawing
				*/
				UseOldShapeDraw = true;
#ifdef WIN32
				buffer = (unsigned char *) Get_Shape_Header_Data((void*)shape_pointer);
#endif

				if (Debug_Rotate) {
#if (0)//PG
					GraphicBufferClass src(width, height, buffer);
					width *= 2;
					height *= 2;
					memset(_xbuffer, '\0', SHAPE_BUFFER_SIZE);
					GraphicBufferClass dst(width, height, _xbuffer);
					Rotate_Bitmap(&src, &dst, rotation);
					buffer = _xbuffer;
#endif
				} else {

					BitmapClass bm(width, height, buffer);
					width *= 2;
					height *= 2;
					memset(_xbuffer, '\0', SHAPE_BUFFER_SIZE);
					GraphicBufferClass gb(width, height, _xbuffer);
					TPoint2D pt(width/2, height/2);

					gb.Scale_Rotate(bm, pt, 0x0100, (256-(rotation-64)));
					buffer = _xbuffer;
				}
			}

			/*
			**	Special shadow drawing code (used for aircraft and bullets).
			*/
			if ((flags & (SHAPE_FADING|SHAPE_PREDATOR)) == (SHAPE_FADING|SHAPE_PREDATOR)) {
				flags = flags & ~(SHAPE_FADING|SHAPE_PREDATOR);
				flags = flags | SHAPE_GHOST;
				ghostdata = DisplayClass::SpecialGhost;
			}

			predoffset = Frame;

			if (x > ( WindowList[window][WINDOWWIDTH] << 2)) {
				predoffset = -predoffset;
			}

			if (draw_window.Lock()) {
				if ((flags & (SHAPE_GHOST|SHAPE_FADING)) == (SHAPE_GHOST|SHAPE_FADING)) {
					Buffer_Frame_To_Page(x, y, width, height, buffer, draw_window, flags | SHAPE_TRANS, ghostdata, fadingdata, 1, predoffset);
				} else {
					if (flags & SHAPE_FADING) {
						Buffer_Frame_To_Page(x, y, width, height, buffer, draw_window, flags | SHAPE_TRANS, fadingdata, 1, predoffset);
					} else {
						if (flags & SHAPE_PREDATOR) {
							Buffer_Frame_To_Page(x, y, width, height, buffer, draw_window, flags | SHAPE_TRANS, predoffset);
						} else {
							Buffer_Frame_To_Page(x, y, width, height, buffer, draw_window, flags | SHAPE_TRANS, ghostdata, predoffset);
						}
					}
				}
				draw_window.Unlock();
			}
		}
	}
}


/***********************************************************************************************
 * Shape_Dimensions -- Determine the minimum rectangle for the shape.                          *
 *                                                                                             *
 *    This routine will calculate (using brute forced) the minimum rectangle that will         *
 *    enclose the pixels of the shape. This rectangle will be relative to the upper left       *
 *    corner of the maximum shape size. By using this minimum rectangle, it is possible to     *
 *    greatly optimize the map 'dirty rectangle' logic.                                        *
 *                                                                                             *
 * INPUT:   shapedata   -- Pointer to the shape data block.                                    *
 *                                                                                             *
 *          shapenum    -- The shape number to examine. Each shape would have a different      *
 *                         dimension rectangle.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the rectangle that encloses the shape.                                *
 *                                                                                             *
 * WARNINGS:   This routine uses brute force and is slow. It is presumed that the results      *
 *             will be cached for subsiquent reuse.                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
Rect const Shape_Dimensions(void const * shapedata, int shapenum)
{
	Rect rect;

	if (shapedata == NULL || shapenum < 0 || shapenum > Get_Build_Frame_Count(shapedata)) {
		return(rect);
	}

	char * shape;
#ifdef WIN32
	void * sh = (void *)Build_Frame(shapedata, shapenum, _ShapeBuffer);
	if (sh == NULL) return(rect);
//	shape = (char *)sh;
	shape = (char *)Get_Shape_Header_Data(sh);
#else
	Build_Frame(shapedata, shapenum, _ShapeBuffer);
	shape = (char *)_ShapeBuffer;
#endif

	int width = Get_Build_Frame_Width(shapedata);
	int height = Get_Build_Frame_Height(shapedata);

	rect.X = 0;
	rect.Y = 0;
	int xlimit = width-1;
	int ylimit = height-1;

	/*
	**	Find top edge of the shape.
	*/
	for (int y = 0; y <= ylimit; y++) {
		for (int x = 0; x <= xlimit; x++) {
			if (shape[y*width + x] != 0) {
				rect.Y = y;
				rect.X = x;
				y = ylimit+1;
				break;
			}
		}
	}

	/*
	**	Find bottom edge of the shape.
	*/
	for (int y = ylimit; y >= rect.Y; y--) {
		for (int x = xlimit; x >= 0; x--) {
			if (shape[y*width + x] != 0) {
				rect.Height = (y-rect.Y)+1;
				xlimit = x;
				y = rect.Y-1;
				break;
			}
		}
	}

	/*
	**	Find left edge of the shape.
	*/
	for (int x = 0; x < rect.X; x++) {
		for (int y = rect.Y; y < rect.Y+rect.Height; y++) {
			if (shape[y*width + x] != 0) {
				rect.X = x;
				x = rect.X;
				break;
			}
		}
	}

	/*
	**	Find the right edge of the shape.
	*/
	for (int x = width-1; x >= xlimit; x--) {
		for (int y = rect.Y; y < rect.Y+rect.Height; y++) {
			if (shape[y*width + x] != 0) {
				rect.Width = (x-rect.X)+1;
				x = xlimit-1;
				break;
			}
		}
	}

	/*
	**	Normalize the rectangle around the center of the shape.
	*/
	rect.X -= width / 2;
	rect.Y -= height / 2;

	/*
	**	Return with the minimum rectangle that encloses the shape.
	*/
	return(rect);
}


/***********************************************************************************************
 * Fetch_Techno_Type -- Convert type and ID into TechnoTypeClass pointer.                      *
 *                                                                                             *
 *    This routine will convert the supplied RTTI type number and the ID value into a valid    *
 *    TechnoTypeClass pointer. If there is an error in conversion, then NULL is returned.      *
 *                                                                                             *
 * INPUT:   type  -- RTTI type of the techno class object.                                     *
 *                                                                                             *
 *          id    -- Integer representation of the techno sub type number.                     *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the techno type class object specified or NULL if the    *
 *          conversion could not occur.                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TechnoTypeClass const * Fetch_Techno_Type(RTTIType type, int id)
{
	switch (type) {
		case RTTI_UNITTYPE:
		case RTTI_UNIT:
			return(&UnitTypeClass::As_Reference(UnitType(id)));

		case RTTI_VESSELTYPE:
		case RTTI_VESSEL:
			return(&VesselTypeClass::As_Reference(VesselType(id)));

		case RTTI_BUILDINGTYPE:
		case RTTI_BUILDING:
			return(&BuildingTypeClass::As_Reference(StructType(id)));

		case RTTI_INFANTRYTYPE:
		case RTTI_INFANTRY:
			return(&InfantryTypeClass::As_Reference(InfantryType(id)));

		case RTTI_AIRCRAFTTYPE:
		case RTTI_AIRCRAFT:
			return(&AircraftTypeClass::As_Reference(AircraftType(id)));

		default:
			break;
	}
	return(NULL);
}


/***********************************************************************************************
 * VQ_Call_Back -- Maintenance callback used for VQ movies.                                    *
 *                                                                                             *
 *    This routine is called every frame of the VQ movie as it is being played. If this        *
 *    routine returns non-zero, then the movie will stop.                                      *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the image buffer for the current frame.                     *
 *                                                                                             *
 *          frame    -- The frame number about to be displayed.                                *
 *                                                                                             *
 * OUTPUT:  Should the movie be stopped?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef WIN32
void VQA_PauseAudio(void) {};
void Check_VQ_Palette_Set(void);

extern GraphicBufferClass VQ640;
extern bool IsVQ640;
long VQ_Call_Back(unsigned char *, long )
{
	return 0;
#if (0)//PG
	int key = 0;
	if (Keyboard->Check()) {
		key = Keyboard->Get();
		Keyboard->Clear();
	}
	Check_VQ_Palette_Set();
#ifdef MOVIE640
	if(IsVQ640) {
		VQ640.Blit(SeenBuff);
	} else {
		Interpolate_2X_Scale(&SysMemPage, &SeenBuff, NULL);
	}
#else
	Interpolate_2X_Scale(&SysMemPage, &SeenBuff, NULL);
#endif
	//Call_Back();

	if ((BreakoutAllowed || Debug_Flag) && key == KN_ESC) {
		Keyboard->Clear();
		Brokeout = true;
		return(true);
	}

	if (!GameInFocus) {
		VQA_PauseAudio();
		while (!GameInFocus) {
			Check_For_Focus_Loss();
		}
	}
	return(false);
#endif
}

#else	//WIN32

long VQ_Call_Back(unsigned char *, long )
{
	Call_Back();
	if ((BreakoutAllowed || Debug_Flag) && Keyboard->Check()) {
		if (Keyboard->Get() == KN_ESC) {
			Keyboard->Clear();
			Brokeout = true;
			return(true);
		}
		Keyboard->Clear();
	}
	return(false);
}
#endif	//WIN32


/***********************************************************************************************
 * Handle_Team -- Processes team selection command.                                            *
 *                                                                                             *
 *    This routine will handle creation and selection of pseudo teams that the player can      *
 *    create or control. A team in this sense is an arbitrary grouping of units such that      *
 *    rapid selection control is allowed.                                                      *
 *                                                                                             *
 * INPUT:   team  -- The logical team number to process.                                       *
 *                                                                                             *
 *          action-- The action to perform on this team:                                       *
 *                   0 - Toggle the select state for all members of this team.                 *
 *                   1 - Select the members of this team.                                      *
 *                   2 - Make all selected objects members of this team.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Handle_Team(int team, int action)
{
	int index;

	//
	// Recording support
	//
	if (Session.Record) {
		TeamNumber = (char)team;
		TeamEvent = (char)action + 1;
	}

	AllowVoice = true;
	switch (action) {

		/*
		**	Toggle the team selection. If the team is selected, then merely unselect it. If the
		**	team is not selected, then unselect all others before selecting this team.
		*/
		case 3:
		case 0:

			/*
			**	If a non team member is currently selected, then deselect all objects
			**	before selecting this team.
			*/
			if (CurrentObject.Count()) {
				if (CurrentObject[0]->Is_Foot() && ((FootClass *)CurrentObject[0])->Group != team) {
					Unselect_All();
				}
			}
			for (index = 0; index < Vessels.Count(); index++) {
				VesselClass * obj = Vessels.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House->IsPlayerControl) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			for (index = 0; index < Units.Count(); index++) {
				UnitClass * obj = Units.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House->IsPlayerControl) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			for (index = 0; index < Infantry.Count(); index++) {
				InfantryClass * obj = Infantry.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House->IsPlayerControl) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			for (index = 0; index < Aircraft.Count(); index++) {
				AircraftClass * obj = Aircraft.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House->IsPlayerControl) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}

			/*
			**	Center the map around the team if the ALT key was pressed too.
			*/
			if (action == 3) {
				Map.Center_Map();
#ifdef WIN32
				Map.Flag_To_Redraw(true);
#endif	//WIn32
			}
			break;

		/*
		**	Additive selection of team.
		*/
		case 1:
			for (index = 0; index < Units.Count(); index++) {
				UnitClass * obj = Units.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House->IsPlayerControl) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			for (index = 0; index < Vessels.Count(); index++) {
				VesselClass * obj = Vessels.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House->IsPlayerControl) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			for (index = 0; index < Infantry.Count(); index++) {
				InfantryClass * obj = Infantry.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House->IsPlayerControl) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			for (index = 0; index < Aircraft.Count(); index++) {
				AircraftClass * obj = Aircraft.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House->IsPlayerControl) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			break;

		/*
		**	Create the team.
		*/
		case 2: {
			long minx = 0x7FFFFFFFL, miny = 0x7FFFFFFFL;
			long maxx = 0, maxy = 0;
			TeamSpeed[team] = SPEED_WHEEL;
			TeamMaxSpeed[team] = MPH_LIGHT_SPEED;
			for (index = 0; index < Units.Count(); index++) {
				UnitClass * obj = Units.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->House->IsPlayerControl) {
				 	if (obj->Group == team) obj->Group = 0xFF;
					if (obj->Is_Selected_By_Player()) {
						obj->Group = team;
						obj->Mark(MARK_CHANGE);
						long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
						long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));
						if (xc < minx) minx = xc;
						if (xc > maxx) maxx = xc;
						if (yc < miny) miny = yc;
						if (yc > maxy) maxy = yc;
						if (obj->Class->MaxSpeed < TeamMaxSpeed[team]) {
							TeamMaxSpeed[team] = obj->Class->MaxSpeed;
							TeamSpeed[team] = obj->Class->Speed;
						}
					}
				}
			}

			for (index = 0; index < Vessels.Count(); index++) {
				VesselClass * obj = Vessels.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->House->IsPlayerControl) {
				 	if (obj->Group == team) obj->Group = -1;
					if (obj->Is_Selected_By_Player()) {
						obj->Group = team;
						obj->Mark(MARK_CHANGE);
						long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
						long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));
						if (xc < minx) minx = xc;
						if (xc > maxx) maxx = xc;
						if (yc < miny) miny = yc;
						if (yc > maxy) maxy = yc;
						if (obj->Class->MaxSpeed < TeamMaxSpeed[team]) {
							TeamMaxSpeed[team] = obj->Class->MaxSpeed;
							TeamSpeed[team] = obj->Class->Speed;
						}
					}
				}
			}

			for (index = 0; index < Infantry.Count(); index++) {
				InfantryClass * obj = Infantry.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->House->IsPlayerControl) {
				 	if (obj->Group == team) obj->Group = 0xFF;
					if (obj->Is_Selected_By_Player()) {
						obj->Group = team;
						obj->Mark(MARK_CHANGE);
						long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
						long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));
						if (xc < minx) minx = xc;
						if (xc > maxx) maxx = xc;
						if (yc < miny) miny = yc;
						if (yc > maxy) maxy = yc;
						if (obj->Class->MaxSpeed < TeamMaxSpeed[team]) {
							TeamMaxSpeed[team] = obj->Class->MaxSpeed;
						}
					}
				}
			}
			for (index = 0; index < Aircraft.Count(); index++) {
				AircraftClass * obj = Aircraft.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->House->IsPlayerControl) {
				 	if (obj->Group == team) obj->Group = 0xFF;
					if (obj->Is_Selected_By_Player()) {
						obj->Group = team;
						obj->Mark(MARK_CHANGE);
					}
				}
			}

			for (index = 0; index < Units.Count(); index++) {
				UnitClass * obj = Units.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->House->IsPlayerControl &&
				 	(obj->Group == team) && (obj->Is_Selected_By_Player()) ) {

					/*
					** When a team is first created, they're created without a
					** formation offset, so they will not be created in
					** formation.  Later, if they're assigned a formation, the
					** XFormOffset & YFormOffset numbers will change to valid
					** offsets, and they'll be formationed.
					*/
#if(1)
					obj->XFormOffset = obj->YFormOffset = (int)0x80000000;
#else
#if(1)
// Old always-north formation stuff
						long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
						long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));

						obj->XFormOffset = xc - centerx;
						obj->YFormOffset = yc - centery;
#else
// New method: save direction and distance rather than x & y offset
						obj->XFormOffset = ::Direction(As_Coord(center), obj->Center_Coord());
						obj->YFormOffset = ::Distance (As_Coord(center), obj->Center_Coord());
#endif
#endif
				}
			}

			for (index = 0; index < Infantry.Count(); index++) {
				InfantryClass * obj = Infantry.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->House->IsPlayerControl) {
				 	if (obj->Group == team) obj->Group = 0xFF;
					if (obj->Is_Selected_By_Player()) obj->Group = team;
					if (obj->Group == team  && obj->Is_Selected_By_Player()) {
#if(1)
						obj->XFormOffset = obj->YFormOffset = (int)0x80000000;
#else
#if(1)
// Old always-north formation stuff
						long xc = Cell_X(Coord_Cell(obj->Center_Coord()));
						long yc = Cell_Y(Coord_Cell(obj->Center_Coord()));

						obj->XFormOffset = xc - centerx;
						obj->YFormOffset = yc - centery;
#else
// New method: save direction and distance rather than x & y offset
						obj->XFormOffset = ::Direction(As_Coord(center), obj->Center_Coord());
						obj->YFormOffset = ::Distance (As_Coord(center), obj->Center_Coord());
#endif
#endif
					}
				}
			}
			break;
		}

		default:
			break;
	}
	AllowVoice = true;
}


/***********************************************************************************************
 * Handle_View -- Either records or restores the tactical view.                                *
 *                                                                                             *
 *    This routine is used to record or restore the current map tactical view.                 *
 *                                                                                             *
 * INPUT:   view  -- The view number to work with.                                             *
 *                                                                                             *
 *          action-- The action to perform with this view number.                              *
 *                   0  =  Restore the view to this previously remembered location.            *
 *                   1  =  Record the current view location.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Handle_View(int view, int action)
{
	if ((unsigned)view < ARRAY_SIZE(Scen.Views)) {
		if (action == 0) {

			Map.Set_Tactical_Position(Coord_Whole(Cell_Coord(Scen.Views[view] - (MAP_CELL_W * 4 * RESFACTOR) - (5*RESFACTOR))));

#ifdef WIN32
			/*
			** Win95 scrolling logic cant handle just jumps in screen position so redraw the lot.
			*/
			Map.Flag_To_Redraw (true);
#endif	//WIN32
		} else {
			Scen.Views[view] = Coord_Cell(Map.TacticalCoord) + (MAP_CELL_W*4*RESFACTOR) + (5*RESFACTOR);
		}
	}
}


#ifndef ROR_NOT_READY
#define ROR_NOT_READY 21
#endif

static char * _CD_Volume_Label[] = {
	"CD1",
	"CD2",
	"CD3",
	"CD4",
	// Denzil 4/15/98
	#ifdef DVD
	"CD1",		//	ajw - Pushes RADVD to position 5, to match enum in Force_CD_Available(). 4 will never be returned here.
	"RADVD",
	#endif
};
static int _Num_Volumes = ARRAY_SIZE(_CD_Volume_Label);


#ifdef WIN32
/***********************************************************************************************
 * Get_CD_Index -- returns the volume type of the CD in the given drive                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    drive number                                                                      *
 *           timeout                                                                           *
 *                                                                                             *
 * OUTPUT:   0 = gdi                                                                           *
 *           1 = nod                                                                           *
 *           2 = covert or CS                                                                       *
 *           3 = Aftermath
 *           5 = DVD
 *          -1 = non C&C                                                                       *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/21/96 5:27PM ST : Created                                                              *
 *    01/20/97 V.Grippi added CS support											       *
 *=============================================================================================*/
int Get_CD_Index (int cd_drive, int timeout)
	{
	char		volume_name[128];
	char		buffer[128];
	unsigned	filename_length;
	unsigned	misc_dword;
	int		count = 0;

	CountDownTimerClass timer;

	timer.Set(timeout);

	/*
	** Get the volume label. If we get a 'not ready' error then retry for the timeout
	** period.
	*/
	for (;;)
		{
		sprintf(buffer, "%c:\\", 'A' + cd_drive);

		if (GetVolumeInformation ((char const *)buffer, &volume_name[0] ,
				(unsigned long)sizeof(volume_name), (unsigned long *)NULL ,
				(unsigned long *)&filename_length, (unsigned long *)&misc_dword,
				(char *)NULL, (unsigned long)0))
			{
			/*
			** Try opening 'movies.mix' to verify that the CD is really there and is what
			** it says it is.
			*/
			sprintf(buffer, "%c:\\main.mix", 'A' + cd_drive);

			HANDLE handle = CreateFile(buffer, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (handle != INVALID_HANDLE_VALUE)
				{
				CloseHandle(handle);

				/*
				** Match the volume label to the list of known C&C volume labels.
				*/
				for (int i = 0 ; i < _Num_Volumes; i++)
					{
					if (!stricmp(_CD_Volume_Label[i], volume_name))
						return(i);
					}
				}
			else
				{
				if (!count)
					count++;
				else
					return -1;
				}
			}
		else
			{
			/*
			** Failed to get the volume label on a known CD drive.
			** If this is a CD changer it may require time to swap the disks so dont return
			** immediately if the error is ROR_NOT_READY
			*/
			if (!timer.Time())
				return -1;
				
			int val = GetLastError();
			
			if (val != ROR_NOT_READY)
				return -1;
			}	
		}
	}
#else
int Get_CD_Index(int cd_drive, int)
	{
	char buffer[128];

	/*
	** We need to do this twice because of the possibilities of a directory
	** being cached.  If this is so, it will only be discovered when we
	** actually attempt to read a file from the drive.
	*/
	if(cd_drive) for (int count = 0; count < 2; count ++)
		{
		struct find_t ft;
		int file;
		int open_failed;

		/*
		** Create a path for the cd drive and attempt to read the volume label from
		** it.
		*/
		sprintf(buffer, "%c:\\", 'A' + cd_drive);

		/*
		** If we are able to read the volume label, this is good but not enough.
		** Further verification must be done.
		*/
		if (!_dos_findfirst(buffer, _A_VOLID, &ft))
			{
			/*
			** Since some versions of disk cacheing software will cache the CD's
			** directory tracks, we may think the CD is in the drive when it is
			** actually not.  To resolve this we must attempt to open a file on
			** the cd.  Opening a file will always update the directory tracks
			** (suposedly).
			*/
			sprintf(buffer, "%c:\\main.mix", 'A' + cd_drive);
			open_failed = _dos_open(buffer, O_RDONLY|SH_DENYNO, &file);

			if (!open_failed)
				{
				_dos_close(file);

				/*
				** Hey some times the stupid dos driver appends a period to the
				** name if it is eight characters long.  If the last char is a
				** period then erase it.
				*/
				if (ft.name[strlen(ft.name)-1] == '.')
					{
					ft.name[strlen(ft.name)-1] = 0;
					}

				/*
				** Match the volume label to the list of known C&C volume labels.
				*/
				for (int i = 0 ; i < _Num_Volumes; i++)
					{
					if (!stricmp(_CD_Volume_Label[i], ft.name))
						return (i);
					}
				}
			}
		}

	return -1;
	}
#endif


/***********************************************************************************************
 * Force_CD_Available -- Ensures that specified CD is available.                               *
 *                                                                                             *
 *    Call this routine when you need to ensure that the specified CD is actually in the       *
 *    CD-ROM drive.                                                                            *
 *                                                                                             *
 * INPUT:   cd    -- The CD that must be available. This will either be "0" for the GDI CD, or *
 *                   "1" for the Nod CD. If either CD will qualify, then pass in "-1".         *
 *                0  = CD1
 *                1  = CD2
 *                2  = Counterstrike
 *                3  = Aftermath
 *                4  = Counterstrike or Aftermath
 *                5  = DVD
 *                -1 = Any CD
 *                -2 = Local Harddisk
 *                                                                                             *
 * OUTPUT:  Is the CD inserted and available? If false is returned, then this indicates that   *
 *          the player pressed <CANCEL>.                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1995 JLB : Created.                                                                 *
 *   05/22/1996  ST : Handles multiple CD drives / CD changers                                 *
 *   01/20/1997 V.Grippi added expansion cd message
 *=============================================================================================*/

#if (1) //ST - 5/13/2019
bool Force_CD_Available(int cd)
{
	static int _last = -1;

	if (_last != cd || cd == ALWAYS_RELOAD_CD) {
		if (cd != ALWAYS_RELOAD_CD) {
			_last = cd;
		}
		Theme.Stop();

		//		if (ConquerMix) delete ConquerMix;
		if (MoviesMix) {
			delete MoviesMix;
			MoviesMix = 0;
		}
		if (GeneralMix) {
			delete GeneralMix;
			GeneralMix = 0;
		}
		if (ScoreMix) {
			delete ScoreMix;
			ScoreMix = 0;
		}
		if (MainMix) {
			delete MainMix;
			MainMix = 0;
		}

		MainMix = new MFCD("MAIN.MIX", &FastKey);
		assert(MainMix != NULL);
		//		ConquerMix = new MFCD("CONQUER.MIX", &FastKey);
		if (CCFileClass("MOVIES1.MIX").Is_Available())
			MoviesMix = new MFCD("MOVIES1.MIX", &FastKey);
		else
			MoviesMix = new MFCD("MOVIES2.MIX", &FastKey);
		assert(MoviesMix != NULL);
		GeneralMix = new MFCD("GENERAL.MIX", &FastKey);
		ScoreMix = new MFCD("SCORES.MIX", &FastKey);
	}

	return true;
}


#endif

#if (0) //ST - 5/13/2019

typedef enum {
	CD_LOCAL = -2,
	CD_ANY = -1,
	CD_SOVIET = 0,
	CD_ALLIED,
	CD_COUNTERSTRIKE,
	CD_AFTERMATH,
	CD_CS_OR_AM,
	CD_DVD
} CD_VOLUME;

#ifdef FIXIT_VERSION_3

#ifndef DVD
#error DVD must be defined!
#endif

bool Force_CD_Available( int cd_desired )				//	ajw
{
	static int _last = -1;
	static void *font;
#ifdef FRENCH
	static char * _cd_name[] = {
		"ALERTE ROUGE CD1",
		"ALERTE ROUGE CD2",
		"CD Missions Taiga",
		"CD Missions M.A.D.",
		"ALERTE ROUGE DVD",
};
#endif
#ifdef GERMAN
	static char * _cd_name[] = {
		"ALARMSTUFE ROT CD1",
		"ALARMSTUFE ROT CD2",
		"CD Gegenangriff einlegen",
		"CD TRANS einlegen",
		"ALARMSTUFE ROT DVD",
	};
#endif
#ifdef ENGLISH
	static char * _cd_name[] = {
		"RED ALERT DISK 1",
		"RED ALERT DISK 2",
		"CounterStrike CD",
		"Aftermath CD",
		"RED ALERT DVD",
	};
#endif



	int new_cd_drive = 0;
	int cd_current;
	int current_drive;

	ThemeType theme_playing = THEME_NONE;

	/*
	** If the required CD is set to -2 then it means that the file is present
	** on the local hard drive and we shouldn't have to worry about it.
	*/
	if (cd_desired == CD_LOCAL) return(true);

	/*
	** Find out if the CD in the current drive is the one we are looking for
 	*/
	current_drive = CCFileClass::Get_CD_Drive();
	cd_current = Get_CD_Index(current_drive, 1*60);

//	debugprint("Get_CD_Index just returned %d\n", cd_current);
//	debugprint("We are checking for %d\n", cd_desired);
//	debugprint("current_drive = %d\n", current_drive);

	if( Using_DVD() )
	{
		//	All requested cd indexes get rerouted to the DVD.
		cd_desired = CD_DVD;
//		if( RequiredCD != -1 )
//			RequiredCD = CD_DVD;		//	Just seems like a good idea. Not sure if necessary.	ajw
	}

	if (cd_current >= 0 )
	{
		if( cd_desired == CD_CS_OR_AM )
		{
			// If the current cd is CS or AM then change request to whatever
			// is present.
			if( cd_current == CD_COUNTERSTRIKE || cd_current == CD_AFTERMATH )
				cd_desired = cd_current;
		}
		// If the current CD is requested or any CD will work
		if( cd_desired == cd_current || cd_desired == CD_ANY )
		{
			/*
			** The required CD is still in the CD drive we used last time
			*/
			new_cd_drive = current_drive;
		}
	}

	/*
	** Flag that we will have to restart the theme
	*/
	theme_playing = Theme.What_Is_Playing();
	Theme.Stop();

	// Check the last drive
	if (!new_cd_drive)
	{
		/*
		** Check the last CD drive we used if it's different from the current one
		*/
		int last_drive = CCFileClass::Get_Last_CD_Drive();

		/*
		** Make sure the last drive is valid and it isn't the current drive
		*/
		if (last_drive && last_drive != CCFileClass::Get_CD_Drive())		//	Else we have already checked this cd.
		{
			/*
			** Find out if there is a C&C cd in the last drive and if so is it the one we are looking for
			** Give it a nice big timeout so the CD changer has time to swap the discs
			*/
			cd_current = Get_CD_Index(last_drive, 10*60);

			if (cd_current >= 0 )
			{
				if( cd_desired == CD_CS_OR_AM )
				{
					// If the cd is CS or AM then change request to whatever
					// is present.
					if( cd_current == CD_COUNTERSTRIKE || cd_current == CD_AFTERMATH )
						cd_desired = cd_current;
				}
				// If the cd is present or any cd will work
				if( cd_desired == cd_current || cd_desired == CD_ANY )
				{
					/*
					** The required CD is in the CD drive we used last time
					*/
					new_cd_drive = last_drive;
				}
			}
		}
	}

	/*
	** Lordy.  No sign of that blimming CD anywhere. Search all the CD drives
	** then if we still can't find it prompt the user to insert it.
	*/
	if (!new_cd_drive)
	{
		/*
		** Small timeout for the first pass through the drives
		*/
		int drive_search_timeout = 2*60;

		for (;;)
		{
			char buffer[128];
			/*
			** Search all present CD drives for the required disc.
			*/
			for (int i = 0 ; i < CDList.Get_Number_Of_Drives(); i++)
			{
				int cd_drive = CDList.Get_Next_CD_Drive();
				cd_current = Get_CD_Index(cd_drive, drive_search_timeout);

				if (cd_current >= 0)
				{
					/*
					** We found a C&C cd - lets see if it was the one we were looking for
					*/
					// Require CS or AM
					if( cd_desired == CD_CS_OR_AM )
					{
						// If the cd is CS or AM then change request to whatever
						// is present.
						if( cd_current == CD_COUNTERSTRIKE || cd_current == CD_AFTERMATH )
							cd_desired = cd_current;
					}
					
					if( cd_desired == cd_current || cd_desired == CD_ANY )
					{
						/*
						** Woohoo! The disk was in a different cd drive. Refresh the search path list
				 		* and return.
						*/
						new_cd_drive = cd_drive;
						break;
					}
				}
			}

			/*
			** A new disc has become available so break
			*/
			if (new_cd_drive) break;

			/*
			** Increase the timeout for subsequent drive searches.
			*/
			drive_search_timeout = 5*60;

			/*
			**	Prompt to insert the CD into the drive.
			*/
			//V.Grippi
			if( cd_desired == CD_CS_OR_AM )
				cd_desired = CD_AFTERMATH;
			
			if( cd_desired == CD_DVD )
			{
				#ifdef FRENCH
			   	sprintf(buffer, "InsŠrez le %s",  _cd_name[4]);
				#else
				#ifdef GERMAN
				sprintf(buffer, "Bitte %s",  _cd_name[4]);
				#else
				sprintf(buffer, "Please insert the %s",  _cd_name[4]);
				#endif
				#endif
			}
			else if( cd_desired == CD_COUNTERSTRIKE || cd_desired == CD_AFTERMATH )
			{
				#ifdef FRENCH
			   	sprintf(buffer, "InsŠrez le %s",  _cd_name[cd_desired]);
				#else
				#ifdef GERMAN
				sprintf(buffer, "Bitte %s",  _cd_name[cd_desired]);
				#else
				sprintf(buffer, "Please insert the %s",  _cd_name[cd_desired]);
				#endif
				#endif
			}
			else if( cd_desired == CD_ANY )
			{
				sprintf(buffer, Text_String(TXT_CD_DIALOG_1), cd_desired+1, _cd_name[cd_desired]);
			}
			else		//	0 or 1
			{
				sprintf(buffer, Text_String(TXT_CD_DIALOG_2), cd_desired+1, _cd_name[cd_desired]);
			}

			GraphicViewPortClass * oldpage = Set_Logic_Page(SeenBuff);
			theme_playing = Theme.What_Is_Playing();
			Theme.Stop();
			int hidden = Get_Mouse_State();
			font = (void *)FontPtr;

			/*
			**	Only set the palette if necessary.
			*/
			if (PaletteClass::CurrentPalette[1].Red_Component() +
					PaletteClass::CurrentPalette[1].Blue_Component() +
					PaletteClass::CurrentPalette[1].Green_Component() == 0)
			{
				GamePalette.Set();
			}

			Keyboard->Clear();

			while (Get_Mouse_State()) Show_Mouse();

			if (WWMessageBox().Process(buffer, TXT_OK, TXT_CANCEL, TXT_NONE, TRUE) == 1)
			{
				Set_Logic_Page(oldpage);
#ifdef FIXIT_VERSION_3
				while (hidden--) Hide_Mouse();
#else
				Hide_Mouse();
#endif
				return(false);
			}

			while (hidden--) Hide_Mouse();
			Set_Font(font);
			Set_Logic_Page(oldpage);
		}
	}

	CurrentCD = cd_current;

	CCFileClass::Set_CD_Drive(new_cd_drive);
	CCFileClass::Refresh_Search_Drives();

	/*
	**	If it broke out of the query for CD-ROM loop, then this means that the
	**	CD-ROM has been inserted.
	*/
	if (cd_desired == 4) cd_desired--;

	//	ajw - Added condition of cd_desired != 5 to the following if.
	//	Reason: This was triggering before Init_Secondary_Mixfiles(), which was screwing up the mixfile system somehow.
	//
	//	Since the DVD is the only disk that can possibly be required when Using_DVD(), I never have to reload the mix
	//	files here, because no other disk could ever have been asked for. And if not Using_DVD(), cd_desired will never
	//	be equal to 5. So this is safe.
	if (cd_desired > -1 && _last != cd_desired && cd_desired != 5)
	{
		_last = cd_desired;

		Theme.Stop();

//		if (ConquerMix) delete ConquerMix;
		if (MoviesMix) delete MoviesMix;
		if (GeneralMix) delete GeneralMix;
		if (ScoreMix) delete ScoreMix;
		if (MainMix) delete MainMix;

		MainMix = new MFCD("MAIN.MIX", &FastKey);
		assert(MainMix != NULL);
//		ConquerMix = new MFCD("CONQUER.MIX", &FastKey);
		if (CCFileClass("MOVIES1.MIX").Is_Available())
			MoviesMix = new MFCD("MOVIES1.MIX", &FastKey);
		else
			MoviesMix = new MFCD("MOVIES2.MIX", &FastKey);
		assert(MoviesMix != NULL);
		GeneralMix = new MFCD("GENERAL.MIX", &FastKey);
		ScoreMix = new MFCD("SCORES.MIX", &FastKey);
		ThemeClass::Scan();
	}

	return(true);
}

#else		//	FIXIT_VERSION_3 not defined

bool Force_CD_Available(int cd)
{
	static int _last = -1;
//	static char _palette[768];
//	static char _hold[256];
	static void *font;
#ifdef FRENCH
	static char * _cd_name[] = {
		"ALERTE ROUGE CD1",
		"ALERTE ROUGE CD2",
		"CD Missions Taiga",
		"CD Missions M.A.D.",

		// Denzil 4/15/98
		#ifdef DVD
		"ALERTE ROUGE DVD",
		#endif
};

#endif
#ifdef GERMAN
	static char * _cd_name[] = {
		"ALARMSTUFE ROT CD1 einlegen",
		"ALARMSTUFE ROT CD2 einlegen",
		"CD Gegenangriff einlegen",
		"CD TRANS einlegen",

		// Denzil 4/15/98
		#ifdef DVD
		"ALARMSTUFE ROT DVD einlegen",
		#endif
	};
#endif
#ifdef ENGLISH
	static char * _cd_name[] = {
		"RED ALERT DISK 1",
		"RED ALERT DISK 2",
		"CounterStrike CD",
		"Aftermath CD",

		// Denzil 4/15/98
		#ifdef DVD
		"RED ALERT DVD",
		#endif
	};
#endif

	int new_cd_drive = 0;
	int cd_index;
	char buffer[128];
	int cd_drive;
	int current_drive;
	int drive_search_timeout;

	ThemeType theme_playing = THEME_NONE;

//#ifdef FIXIT_ANTS
//	if(Scen.ScenarioName[2] == 'A')
//	   cd = 2;
//#endif
	/*
	** If the required CD is set to -2 then it means that the file is present
	** on the local hard drive and we shouldn't have to worry about it.
	*/
	if (cd == CD_LOCAL) return(true);

	/*
	** Find out if the CD in the current drive is the one we are looking for
 	*/
	current_drive = CCFileClass::Get_CD_Drive();
	cd_index = Get_CD_Index(current_drive, 1*60);

	#ifdef CHEAT_KEYS
	//	Mono_Printf("Get_CD_Index just returned %d\n", cd_index);
	//	Mono_Printf("We are checking for %d\n", cd);
	//	Mono_Printf("current_drive = %d\n", current_drive);
	#endif	//CHEAT_KEYS

	#ifdef DVD // Denzil
	// CD1 and CD2 are ignored, force the DVD
	if (cd_index == 0 || cd_index == 1)
		cd_index = -1;
	#endif
	
	if (cd_index >= 0 )
		{
		#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		// Require CS or AM
		if (cd == CD_CS_OR_AM)
			{
			// If the current cd is CS or AM then change request to whatever
			// is present.
			if (cd_index == 2 || cd_index == 3)
				{
				cd = cd_index;
				}
			}
		#endif

		#ifdef DVD // Denzil
		// If the current drive is the DVD then requests for CD1 and CD2 are okay
		if (cd_index == 4)
			{
			// CD1, CD2 & DVD requests
			if (cd == 0 || cd == 1 || cd == 5)
				{
				cd_index = cd;
				}
			}
		#endif
		
		// If the current CD is requested or any CD will work
		if (cd == cd_index || cd == -1 )
			{
			/*
			** The required CD is still in the CD drive we used last time
			*/
			new_cd_drive = current_drive;
			}
		}

	/*
	** Flag that we will have to restart the theme
	*/
	theme_playing = Theme.What_Is_Playing();
	Theme.Stop();

	// Check the last drive
	if (!new_cd_drive)
		{
		/*
		** Check the last CD drive we used if it's different from the current one
		*/
		int last_drive = CCFileClass::Get_Last_CD_Drive();

		/*
		** Make sure the last drive is valid and it isn't the current drive
		*/
		if (last_drive && last_drive != CCFileClass::Get_CD_Drive())
			{
			/*
			** Find out if there is a C&C cd in the last drive and if so is it the one we are looking for
			** Give it a nice big timeout so the CD changer has time to swap the discs
			*/
			cd_index = Get_CD_Index(last_drive, 10*60);

			#ifdef DVD // Denzil
			// Ignore CD1 and CD2 disks, force DVD
			if (cd_index == 0 || cd_index == 1)
				cd_index = -1;
			#endif
			
			if (cd_index >= 0 )
				{
				#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				// Require CS or AM
				if (cd == 4)
					{
					// If CS or AM was the last drive then use it
					if (cd_index == 2 || cd_index == 3)
						{
						cd = cd_index;
						}
					}
				#endif

				#ifdef DVD // Denzil
				// If DVD is in drive
				if (cd_index == 4)
					{
					// CD1, CD2 and DVD requests are all on the DVD 
					if ((cd == 0) || (cd == 1) || (cd == 5))
						{
						cd_index = cd;
						}
					}
				#endif

				// If the cd is present or any cd will work
				if (cd == cd_index || cd == -1 )
					{
					/*
					** The required CD is in the CD drive we used last time
					*/
					new_cd_drive = last_drive;
					}
				}
			}
		}

	/*
	** Lordy.  No sign of that blimming CD anywhere. Search all the CD drives
	** then if we still can't find it prompt the user to insert it.
	*/
	if (!new_cd_drive)
		{
		/*
		** Small timeout for the first pass through the drives
		*/
		drive_search_timeout = 2*60;

		for (;;)
			{
			/*
			** Search all present CD drives for the required disc.
			*/
			for (int i = 0 ; i < CDList.Get_Number_Of_Drives(); i++)
				{
				cd_drive = CDList.Get_Next_CD_Drive();
				cd_index = Get_CD_Index(cd_drive, drive_search_timeout);

				#ifdef DVD // Denzil
				// Ignore CD1 and CD2, force the DVD
				if (cd_index == 0 || cd_index == 1)
					cd_index = -1;
				#endif
				
				if (cd_index >= 0)
					{
					/*
					** We found a C&C cd - lets see if it was the one we were looking for
					*/
					#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
					// Require CS or AM
					if (cd == 4)
						{
						// If the disk is CS or AM then request it
						if (cd_index == 2 || cd_index == 3)
							{
							cd = cd_index;
							}
						}
					#endif

					#ifdef DVD // Denzil
					if (cd_index == 4)
						{
						if ((cd == 0) || (cd == 1) || (cd == 5))
							{
							cd_index = cd;
							}
						}
					#endif
					
					if (cd == cd_index || cd == -1 || cd == -2 )
						{
						/*
						** Woohoo! The disk was in a different cd drive. Refresh the search path list
				 		* and return.
						*/
						new_cd_drive = cd_drive;
						break;
						}
					}
				}

			/*
			** A new disc has become available so break
			*/
			if (new_cd_drive) break;

			/*
			** Increase the timeout for subsequent drive searches.
			*/
			drive_search_timeout = 5*60;

			/*
			**	Prompt to insert the CD into the drive.
			*/
			//V.Grippi
			#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if(cd == 4) cd--;
			
			// CS or AM
			if(cd == 2 || cd == 3)
				{
			#else
			if(cd == 2)
		   		{
			#endif

				#ifdef FRENCH
			   	sprintf(buffer, "InsŠrez le %s",  _cd_name[cd]);
				#else
				#ifdef GERMAN
				sprintf(buffer, "Bitte %s",  _cd_name[cd]);
				#else
				sprintf(buffer, "Please insert the %s",  _cd_name[cd]);
				#endif
				#endif
				}
			else
				{
				#ifdef DVD
				#ifdef FRENCH
			   	sprintf(buffer, "InsŠrez le %s", _cd_name[4]);
				#else
				#ifdef GERMAN 
				sprintf(buffer, "Bitte %s", _cd_name[4]);
				#else
				sprintf(buffer, "Please insert the %s", _cd_name[4]);
				#endif
				#endif
				#else
				if (cd == -1 )
					{
					sprintf(buffer, Text_String(TXT_CD_DIALOG_1), cd+1, _cd_name[cd]);
					}
				else
					{
					sprintf(buffer, Text_String(TXT_CD_DIALOG_2), cd+1, _cd_name[cd]);
					}
				#endif
				}

			#ifdef WIN32
			GraphicViewPortClass * oldpage = Set_Logic_Page(SeenBuff);
			#else
			GraphicBufferClass * oldpage = Set_Logic_Page(SeenBuff);
			#endif
			theme_playing = Theme.What_Is_Playing();
			Theme.Stop();
			int hidden = Get_Mouse_State();
			font = (void *)FontPtr;

			/*
			**	Only set the palette if necessary.
			*/
			if (PaletteClass::CurrentPalette[1].Red_Component() +
					PaletteClass::CurrentPalette[1].Blue_Component() +
					PaletteClass::CurrentPalette[1].Green_Component() == 0)
				{
				GamePalette.Set();
				}

			Keyboard->Clear();

			while (Get_Mouse_State()) Show_Mouse();

			if (WWMessageBox().Process(buffer, TXT_OK, TXT_CANCEL, TXT_NONE, TRUE) == 1)
				{
				Set_Logic_Page(oldpage);
				Hide_Mouse();
				return(false);
				}

			while (hidden--) Hide_Mouse();
			Set_Font(font);
			Set_Logic_Page(oldpage);
			}
		}

	CurrentCD = cd_index;

	CCFileClass::Set_CD_Drive(new_cd_drive);
	CCFileClass::Refresh_Search_Drives();

	/*
	**	If it broke out of the query for CD-ROM loop, then this means that the
	**	CD-ROM has been inserted.
	*/
	#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (cd == 4) cd--;
	#endif
//	if (cd > -3 && _last != cd) {
	if (cd > -1 && _last != cd)
		{
		_last = cd;

		Theme.Stop();

//		if (ConquerMix) delete ConquerMix;
		if (MoviesMix) delete MoviesMix;
		if (GeneralMix) delete GeneralMix;
		if (ScoreMix) delete ScoreMix;
		if (MainMix) delete MainMix;

		MainMix = new MFCD("MAIN.MIX", &FastKey);
		
		assert(MainMix != NULL);
//		ConquerMix = new MFCD("CONQUER.MIX", &FastKey);

		if (CCFileClass("MOVIES1.MIX").Is_Available())
			{
			MoviesMix = new MFCD("MOVIES1.MIX", &FastKey);
			}
		else
			{
			MoviesMix = new MFCD("MOVIES2.MIX", &FastKey);
			}
		assert(MoviesMix != NULL);
		GeneralMix = new MFCD("GENERAL.MIX", &FastKey);
		ScoreMix = new MFCD("SCORES.MIX", &FastKey);
		ThemeClass::Scan();
		}

	if (theme_playing != THEME_NONE)
		{
		Theme.Queue_Song(theme_playing);
		}

	return(true);
	}


#endif	//	FIXIT_VERSION_3
#endif	// ST - 5/13/2019


/***************************************************************************
 * DISK_SPACE_AVAILABLE -- returns bytes of free disk space                *
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     returns amount of free disk space                           *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/11/1995 PWG : Created.                                             *
 *=========================================================================*/
unsigned long Disk_Space_Available(void)
{
	return 0x7fffffff;	//ST - 5/8/2019
#if (0)
	struct diskfree_t diskdata;
	unsigned drive;

	_dos_getdrive(&drive);
	_dos_getdiskfree(drive, &diskdata);

	return(diskdata.avail_clusters * diskdata.sectors_per_cluster * diskdata.bytes_per_sector);
#endif
}


/***********************************************************************************************
 * Do_Record_Playback -- handles saving/loading map pos & current object                       *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		none.																												  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		none.																												  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		none.																												  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/15/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static void Do_Record_Playback(void)
{
	int count;
	TARGET tgt;
	int i;
	COORDINATE coord;
	ObjectClass * obj;
	unsigned long sum;
	unsigned long sum2;
	unsigned long ltgt;

	/*
	**	Record a game
	*/
	if (Session.Record) {

		/*
		**	Save the map's location
		*/
		Session.RecordFile.Write(&Map.DesiredTacticalCoord,
			sizeof (Map.DesiredTacticalCoord));

		/*
		**	Save the current object list count
		*/
		count = CurrentObject.Count();
		Session.RecordFile.Write(&count, sizeof(count));

		/*
		**	Save a CRC of the selected-object list.
		*/
		sum = 0;
		for (i = 0; i < count; i++) {
			ltgt = (unsigned long)(CurrentObject[i]->As_Target());
			sum += ltgt;
		}
		Session.RecordFile.Write (&sum, sizeof(sum));

		/*
		**	Save all selected objects.
		*/
		for (i = 0; i < count; i++) {
			tgt = CurrentObject[i]->As_Target();
			Session.RecordFile.Write (&tgt, sizeof(tgt));
		}

		//
		// Save team-selection and formation events
		//
		Session.RecordFile.Write (&TeamEvent, sizeof(TeamEvent));
		Session.RecordFile.Write (&TeamNumber, sizeof(TeamNumber));
		Session.RecordFile.Write (&FormationEvent, sizeof(FormationEvent));
		Session.RecordFile.Write (TeamMaxSpeed, sizeof(TeamMaxSpeed));
		Session.RecordFile.Write (TeamSpeed, sizeof(TeamSpeed));
		Session.RecordFile.Write (&FormMove, sizeof(FormMove));
		Session.RecordFile.Write (&FormSpeed, sizeof(FormSpeed));
		Session.RecordFile.Write (&FormMaxSpeed, sizeof(FormMaxSpeed));
		TeamEvent = 0;
		TeamNumber = 0;
		FormationEvent = 0;
	}

	/*
	**	Play back a game ("attract" mode)
	*/
	if (Session.Play) {

		/*
		**	Read & set the map's location.
		*/
		if (Session.RecordFile.Read(&coord, sizeof(coord))==sizeof(coord)) {
			if (coord != Map.DesiredTacticalCoord) {
				Map.Set_Tactical_Position(coord);
			}
		}

		if (Session.RecordFile.Read(&count, sizeof(count))==sizeof(count)) {
			/*
			**	Compute a CRC of the current object-selection list.
			*/
			sum = 0;
			for (i = 0; i < CurrentObject.Count(); i++) {
				ltgt = (unsigned long)(CurrentObject[i]->As_Target());
				sum += ltgt;
			}

			/*
			**	Load the CRC of the objects on disk; if it doesn't match, select
			**	all objects as they're loaded.
			*/
			Session.RecordFile.Read (&sum2, sizeof(sum2));
			if (sum2 != sum) {
				Unselect_All();
			}

			AllowVoice = true;

			for (i = 0; i < count; i++) {
				if (Session.RecordFile.Read (&tgt, sizeof(tgt))==sizeof(tgt)) {
					obj = As_Object(tgt);
					if (obj && (sum2 != sum)) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}

			AllowVoice = true;

		}

		//
		// Save team-selection and formation events
		//
		Session.RecordFile.Read (&TeamEvent, sizeof(TeamEvent));
		Session.RecordFile.Read (&TeamNumber, sizeof(TeamNumber));
		Session.RecordFile.Read (&FormationEvent, sizeof(FormationEvent));
		if (TeamEvent) {
			Handle_Team(TeamNumber, TeamEvent - 1);
		}
		if (FormationEvent) {
			Toggle_Formation();
		}

Session.RecordFile.Read (TeamMaxSpeed, sizeof(TeamMaxSpeed));
Session.RecordFile.Read (TeamSpeed, sizeof(TeamSpeed));
Session.RecordFile.Read (&FormMove, sizeof(FormMove));
Session.RecordFile.Read (&FormSpeed, sizeof(FormSpeed));
Session.RecordFile.Read (&FormMaxSpeed, sizeof(FormMaxSpeed));
		/*
		**	The map isn't drawn in playback mode, so draw it here.
		*/
		Map.Render();
	}
}


/***********************************************************************************************
 * Hires_Load -- Allocates memory for, and loads, a resolution dependant file.                 *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Name of file to load                                                              *
 *                                                                                             *
 * OUTPUT:   Ptr to loaded file                                                                *
 *                                                                                             *
 * WARNINGS: Caller is responsible for releasing the memory allocated                          *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/13/96 3:20PM ST : Created                                                              *
 *=============================================================================================*/
void * Hires_Load(char * name)
{
	char 	filename[30];
	int 	length;
	void * return_ptr;

#ifdef WIN32
	sprintf(filename, "H%s", name);
#else
   strcpy(filename, name);
#endif
	CCFileClass file (filename);

	if (file.Is_Available()) {

		length = file.Size();
		return_ptr = new char[length];
		file.Read(return_ptr, length);
		return (return_ptr);

	} else {
		return (NULL);
	}
}


/***********************************************************************************************
 * Crate_From_Name -- Given a crate name convert it to a crate type.                           *
 *                                                                                             *
 *    Use this routine to convert an ASCII crate name into a crate type. If no match could     *
 *    be found, then CRATE_MONEY is assumed.                                                   *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the crate name text to convert into a crate type.              *
 *                                                                                             *
 * OUTPUT:  Returns with the crate name converted into a crate type.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CrateType Crate_From_Name(char const * name)
{
	if (name != NULL) {
		for (CrateType crate = CRATE_FIRST; crate < CRATE_COUNT; crate++) {
			if (stricmp(name, CrateNames[crate]) == 0) return(crate);
		}
	}
	return(CRATE_MONEY);
}


/***********************************************************************************************
 * Owner_From_Name -- Convert an owner name into a bitfield.                                   *
 *                                                                                             *
 *    This will take an owner specification and convert it into a bitfield that represents     *
 *    it. Sometimes this will be just a single house bit, but other times it could be          *
 *    all the allies or soviet house bits combined.                                            *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text to convert into a house bitfield.                     *
 *                                                                                             *
 * OUTPUT:  Returns with the houses specified. The value is in the form of a bit field with    *
 *          one bit per house type.                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int Owner_From_Name(char const * text)
{
	int ownable = 0;
	if (stricmp(text, "soviet") == 0) {
		ownable |= HOUSEF_SOVIET;
	} else {
		if (stricmp(text, "allies") == 0 || stricmp(text, "allied") == 0) {
			ownable |= HOUSEF_ALLIES;
		} else {
			HousesType h = HouseTypeClass::From_Name(text);
			if (h != HOUSE_NONE && (h < HOUSE_MULTI1 || h > HOUSE_MULTI8)) {
				ownable |= (1 << h);
			}
		}
	}
	return(ownable);
}


/***********************************************************************************************
 * Shake_The_Screen -- Dispatcher that shakes the screen.                                      *
 *                                                                                             *
 *    This routine will shake the game screen the number of shakes requested.                  *
 *                                                                                             *
 * INPUT:   shakes   -- The number of shakes to shake the screen.                              *
 *          house    -- House to perform the shake for (or HOUSE_NONE if all players).         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/04/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void Shake_The_Screen(int shakes, HousesType house)
{
	for (char h = HOUSE_FIRST; h < HOUSE_COUNT; ++h) {
		if ((house != HOUSE_NONE) && (h != house)) {
			continue;
		}
		HouseClass* hptr = HouseClass::As_Pointer((HousesType)h);
		if ((hptr != nullptr) && hptr->IsActive && hptr->IsHuman) {
			hptr->ScreenShakeTime = CDTimerClass<FrameTimerClass>(hptr->ScreenShakeTime + shakes + shakes);
			hptr->ScreenShakeTime.Start();
		}
	}
#if (0)
#ifdef WIN32
	shakes += shakes;

	Hide_Mouse();
	SeenPage.Blit(HidPage);
	int oldyoff = 0;
	int newyoff = 0;
	while(shakes--) {
		int x = TickCount;
//		CountDownTimer = 1;
		do {
			newyoff = Sim_Random_Pick(0,2) - 1;
		} while (newyoff == oldyoff);
		switch (newyoff) {
			case -1:
				HidPage.Blit(SeenPage, 0,2, 0,0, 640,398);
				break;
			case 0:
				HidPage.Blit(SeenPage);
				break;
			case 1:
				HidPage.Blit(SeenPage, 0,0, 0,2, 640,398);
				break;
		} while (x == TickCount);
//		} while (CountDownTimer != 0) ;
	}
	HidPage.Blit(SeenPage);
	Show_Mouse();
#else
	Shake_Screen(shakes);
#endif
#endif
}


/***********************************************************************************************
 * List_Copy -- Makes a copy of a cell offset list.                                            *
 *                                                                                             *
 *    This routine will make a copy of a cell offset list. It will only copy the significant   *
 *    elements of the list limited by the maximum length specified.                            *
 *                                                                                             *
 * INPUT:   source   -- Pointer to a cell offset list.                                         *
 *                                                                                             *
 *          len      -- The maximum number of cell offset elements to store in to the          *
 *                      destination list pointer.                                              *
 *                                                                                             *
 *          dest     -- Pointer to the destination list to store the copy into.                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Ensure that the destination list is large enough to hold the list copy.         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void List_Copy(short const * source, int len, short * dest)
{
	if (dest == NULL || dest == NULL) {
		return;
	}

	while (len > 0) {
		*dest = *source;
		if (*dest == REFRESH_EOL) break;
		dest++;
		source++;
		len--;
	}
}



#if 0
//
// Boy, this function sure is crummy
//
void Crummy(int crumb1, int crumb2)
{
	if (Debug_Check_Map && Debug_Heap_Dump) {
		Mono_Printf("Hi, I'm Crummy.  And so are these: %d, %d\n",crumb1,crumb2);
	}
}
#endif



/***********************************************************************************************
 * Game_Registry_Key -- Returns pointer to string containing the registry subkey for the game.
 *                      This is located under HKEY_LOCAL_MACHINE.
 * HISTORY:
 *    11/19/98 ajw : Created
 *=============================================================================================*/
const char* Game_Registry_Key()
{
#ifdef ENGLISH
	static char szKey[] = "SOFTWARE\\Westwood\\Red Alert Windows 95 Edition";
#else
#ifdef GERMAN
	static char szKey[] = "SOFTWARE\\Westwood\\Alarmstufe Rot Windows 95 Edition";
#else
	static char szKey[] = "SOFTWARE\\Westwood\\Alerte Rouge version Windows 95";
#endif
#endif
	return szKey;
}


/***********************************************************************************************
 * Is_Counterstrike_Installed -- Function to determine the availability of the CS expansion    *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   true if Counterstrike is present                                                  *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    4/1/97 11:39PM ST : Created                                                              *
 *=============================================================================================*/
bool Is_Counterstrike_Installed (void)
{
	return true;		// Remasters always have Counterstrike. ST - 10/18/2019 11:06AM

#if (0)
	//	ajw 9/29/98
	static bool	bAlreadyChecked = false;
	static bool bInstalled = false;

	if( !bAlreadyChecked )
	{
		HKEY hKey;
		if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, Game_Registry_Key(), 0, KEY_READ, &hKey ) != ERROR_SUCCESS )
			return false;
		DWORD dwValue;
		DWORD dwBufSize = sizeof( DWORD );
		if( RegQueryValueEx( hKey, "CStrikeInstalled", 0, NULL, (LPBYTE)&dwValue, &dwBufSize ) != ERROR_SUCCESS )
			bInstalled = false;
		else
			bInstalled = (bool)dwValue;			//	(Presumably true, if it's there...)

		RegCloseKey( hKey );
		bAlreadyChecked = true;
	}
	return bInstalled;

//	RawFileClass file("EXPAND.MIX");
//	return(file.Is_Available());
#endif
}

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
/***********************************************************************************************
 *=============================================================================================*/
bool Is_Aftermath_Installed (void)
{
	return true;		// Remasters always have Aftermath. ST - 10/18/2019 11:06AM

#if (0)	 
	//	ajw 9/29/98
	static bool	bAlreadyChecked = false;
	static bool bInstalled = false;

	if( !bAlreadyChecked )
	{
		HKEY hKey;
		if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, Game_Registry_Key(), 0, KEY_READ, &hKey ) != ERROR_SUCCESS )
			return false;
		DWORD dwValue;
		DWORD dwBufSize = sizeof( DWORD );
		if( RegQueryValueEx( hKey, "AftermathInstalled", 0, NULL, (LPBYTE)&dwValue, &dwBufSize ) != ERROR_SUCCESS )
			bInstalled = false;
		else
			bInstalled = (bool)dwValue;			//	(Presumably true, if it's there...)

		RegCloseKey( hKey );
		bAlreadyChecked = true;
	}
	return bInstalled;

//	RawFileClass file("EXPAND2.MIX");
//	return(file.Is_Available());
#endif
}
#endif

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
void Enable_Secret_Units(void)
{
#if 0
	SecretUnitsEnabled=true;
	UnitTypeClass::As_Reference(UNIT_PHASE).Level=10;
	VesselTypeClass::As_Reference(VESSEL_CARRIER).Level=10;
	for (int index = 0; index < Buildings.Count(); index++) {
		Buildings.Ptr(index)->Update_Buildables();
	}
#endif
}
#endif

#ifdef FIXIT_VERSION_3
bool Force_Scenario_Available( const char* szName )
{
	//	Calls Force_CD_Available based on type of scenario. szName is assumed to be an official scenario here.
	if( Is_Mission_Counterstrike( (char*)szName ) )
	{
//		debugprint( "Force_Scenario_Available requiring disk 4...\n" );
		return Force_CD_Available( 4 );
	}
	else if( Is_Mission_Aftermath( (char*)szName ) )
	{
//		debugprint( "Force_Scenario_Available requiring disk 3...\n" );
		return Force_CD_Available( 3 );
	}
	return true;
}
#endif