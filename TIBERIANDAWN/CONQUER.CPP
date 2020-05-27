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

/* $Header:   F:\projects\c&c\vcs\code\conquer.cpv   2.18   16 Oct 1995 16:50:24   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *   Disk_Space_Available -- returns bytes of free disk space              						  *
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
 *   Main_Game -- Main game startup routine.                                                   *
 *   Main_Loop -- This is the main game loop (as a single loop).                               *
 *   Map_Edit_Loop -- a mini-main loop for map edit mode only                                  *
 *   Message_Input -- allows inter-player message input processing                             *
 *   MixFileHandler -- Handles VQ file access.                                                 *
 *   Name_From_Source -- retrieves the name for the given SourceType                           *
 *   Play_Movie -- Plays a VQ movie.                                                           *
 *   Source_From_Name -- Converts ASCII name into SourceType.                                  *
 *   Sync_Delay -- Forces the game into a 15 FPS rate.                                         *
 *   Theater_From_Name -- Converts ASCII name into a theater number.                           *
 *   Trap_Object -- gets a ptr to object of given type & coord                                 *
 *   Unselect_All -- Causes all selected objects to become unselected.                         *
 *   VQ_Call_Back -- Maintenance callback used for VQ movies.                                  *
 *   Validate_Error -- prints an error message when an object fails validation                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include  "tcpip.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<direct.h>
#include	<fcntl.h>
#include	<io.h>
#include	<dos.h>
#include	<share.h>
#include <malloc.h>
#include  "ccdde.h"

#define SHAPE_TRANS		0x40

void *Get_Shape_Header_Data(void *ptr);

/****************************************
**	Function prototypes for this module **
*****************************************/
bool Main_Loop();
void Keyboard_Process(KeyNumType & input);
#ifndef DEMO
static void Message_Input(KeyNumType &input);
#endif
bool Color_Cycle(void);
bool Map_Edit_Loop(void);
void Trap_Object(void);

#ifdef CHEAT_KEYS
void Heap_Dump_Check( char *string );
void Dump_Heap_Pointers( void );
void Error_In_Heap_Pointers( char *string );
#endif
static void Do_Record_Playback(void);
extern void Register_Game_Start_Time(void);
extern void Register_Game_End_Time(void);
extern void Send_Statistics_Packet(void);
extern "C" {
	extern char *__nheapbeg;
}
bool InMainLoop = false;

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))
#endif

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
extern int TotalLocks;
extern bool Spawn_WChat(bool can_launch);
extern bool SpawnedFromWChat;
void Main_Game(int argc, char *argv[])
{
	bool fade = false;	// don't fade title screen the first time through

	/*
	**	Perform one-time-only initializations
	*/
	if (!Init_Game(argc, argv)) {
		return;
	}

	CCDebugString ("C&C95 - Game initialisation complete.\n");
	/*
	**	Game processing loop:
	**	1) Select which game to play, or whether to exit (don't fade the palette
	**		on the first game selection, but fade it in on subsequent calls)
	**	2) Invoke either the main-loop routine, or the editor-loop routine,
	**		until they indicate that the user wants to exit the scenario.
	*/
	while (Select_Game(fade)) {
		
		if (RunningAsDLL) {
			return;
		}
		ScenarioInit = 0;		// Kludge.
//		Theme.Queue_Song(THEME_PICK_ANOTHER);

		fade = true;

		/*
		**	Make the game screen visible, clear the keyboard buffer of spurious
		**	values, and then show the mouse.  This PRESUMES that Select_Game() has
		**	told the map to draw itself.
		*/
		Fade_Palette_To(GamePalette, FADE_PALETTE_MEDIUM, NULL);
		Keyboard::Clear();

		/*
		** Only show the mouse if we're not playing back a recording.
		*/
		if (PlaybackGame) {
			Hide_Mouse();
		} else {
			Show_Mouse();
		}

		SpecialDialog = SDLG_NONE;
		//Start_Profiler();
		if (GameToPlay == GAME_INTERNET){
			Register_Game_Start_Time();
			GameStatisticsPacketSent = false;
			PacketLater = NULL;
			ConnectionLost = false;
		}else{
			DDEServer.Disable();
		}

		InMainLoop = true;

#ifdef SCENARIO_EDITOR
		/*
		**	Scenario-editor version of main-loop processing
		*/
		for (;;) {

			/*
			**	Non-scenario-editor-mode: call the game's main loop
			*/
			if (!Debug_Map) {
				TotalLocks=0;
				if (Main_Loop()) {
					break;
				}

				if (SpecialDialog != SDLG_NONE) {
					//Stop_Profiler();
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
							if (Surrender_Dialog()) {
								OutList.Add(EventClass(EventClass::DESTRUCT));
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

			/*
			**	Call the game's main loop
			*/
			TotalLocks=0;
			if (Main_Loop()) {
				break;
			}

			/*
			**	If the SpecialDialog flag is set, invoke the given special dialog.
			**	This must be done outside the main loop, since the dialog will call
			**	Main_Loop(), allowing the game to run in the background.
			*/
			if (SpecialDialog != SDLG_NONE) {
				//Stop_Profiler();
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
						if (Surrender_Dialog()) {
							OutList.Add(EventClass(EventClass::DESTRUCT));
						}
						SpecialDialog = SDLG_NONE;
						Map.Revert_Mouse_Shape();
						break;

					default:
						break;
				}
			}
		}
#endif
		//Stop_Profiler();
		InMainLoop = false;

		if (!GameStatisticsPacketSent && PacketLater){
			Send_Statistics_Packet();
		}

		/*
		**	Scenario is done; fade palette to black
		*/
		Fade_Palette_To(BlackPalette, FADE_PALETTE_SLOW, NULL);
		VisiblePage.Clear();

#ifndef DEMO
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
		if ( (RecordGame && !SuperRecord) || PlaybackGame) {
			RecordFile.Close();
		}

		if (!PlaybackGame){

			switch (GameToPlay){
				case GAME_NULL_MODEM:
				case GAME_MODEM:
					//ST - 1/2/2019 4:04PM
					//Modem_Signoff();
					break;

				case GAME_IPX:
					Shutdown_Network();
					break;

				case GAME_INTERNET:
					//Winsock.Close();
					break;
			}
		}


		/*
		**	If we're playing back, the mouse will be hidden; show it.
		** Also, set all variables back to normal, to return to the main menu.
		*/
		if (PlaybackGame) {
			Show_Mouse();
			GameToPlay = GAME_NORMAL;
			PlaybackGame = 0;
		}


		/*
		** If we were spawned from WChat then dont go back to the main menu - just quit
		**
		** New: If spawned from WChat then maximise WChat and go back to the main menu after all
		*/
#ifdef FORCE_WINSOCK
		if (Special.IsFromWChat){
			Shutdown_Network();		      // Clear up the pseudo IPX stuff
			Winsock.Close();
			Special.IsFromWChat = false;
			SpawnedFromWChat = false;
			DDEServer.Delete_MPlayer_Game_Info();	//Make sure we dont use the same start packet twice
			GameToPlay = GAME_NORMAL;			//Have to do this or we will got straight to the multiplayer menu
			Spawn_WChat(false);		//Will switch back to Wchat. It must be there because its been poking us
			//break;
		}
#endif	//FORCE_WINSOCK

#endif	//DEMO

	}

#ifdef DEMO
		Hide_Mouse();
		Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, NULL);
		Load_Title_Screen("DEMOPIC.PCX", &HidPage, Palette);
		Blit_Hid_Page_To_Seen_Buff();
		Fade_Palette_To(Palette, FADE_PALETTE_MEDIUM, NULL);
		Clear_KeyBuffer();
		Get_Key();
		Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, NULL);
//		Show_Mouse();
#else

	/*
	**	Free the scenario description buffers
	*/
	Free_Scenario_Descriptions();
#endif

#ifndef NOMEMCHECK
	Uninit_Game();
#endif

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
extern int DebugColour;
void Keyboard_Process(KeyNumType &input)
{
	ObjectClass * obj;
	int index;

	/*
	**	Don't do anything if there is not keyboard event.
	*/
	if (input == KN_NONE) {
		return;
	}

#ifndef DEMO
	/*
	**	For network & modem, process user input for inter-player messages.
	*/
	Message_Input(input);
#endif
	/*
	** Use WWKEY values because KN values have WWKEY_VK_BIT or'd in with them
	** and we need WWKEY_VK_BIT to still be set if it is.
	*/
	KeyNumType plain = input & ~(WWKEY_SHIFT_BIT|WWKEY_ALT_BIT|WWKEY_CTRL_BIT);

#ifdef CHEAT_KEYS

	if (Debug_Flag) {
		switch (input) {
			case (int)KN_M|(int)KN_SHIFT_BIT:
			case (int)KN_M|(int)KN_ALT_BIT:
			case (int)KN_M|(int)KN_CTRL_BIT:
				PlayerPtr->Credits += 10000;
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

//#ifdef CHEAT_KEYS
	if (/*Debug_Playtest && */input == (KN_W|KN_ALT_BIT)) {
		PlayerPtr->Blockage = false;
		PlayerPtr->Flag_To_Win();
	}

	if (Debug_Flag && input == KN_SLASH) {
		if (GameToPlay != GAME_NORMAL) {
			SpecialDialog = SDLG_SPECIAL;
			input = KN_NONE;
		} else {
			Special_Dialog();
		}
	}
//#endif

	/*
	**	If the options key(s) were pressed, then bring up the options screen.
	*/
	if (input == KN_SPACE || input == KN_ESC) {
		Map.Help_Text(TXT_NONE);			// Turns off help text.
		Queue_Options();
		input = KN_NONE;
		//DebugColour++;
		//DebugColour &=7;
	}

	/*
	**	Process prerecorded team selection. This will be an addative select
	**	if the SHIFT key is held down. It will create the team if the
	**	CTRL or ALT key is held down.
	*/
	int action = 0;
	if (input & WWKEY_SHIFT_BIT) action = 1;
	if (input & WWKEY_ALT_BIT) action = 3;
	if (input & WWKEY_CTRL_BIT) action = 2;

	switch (KN_To_VK(plain)) {

		/*
		**	Center the map around the currently selected objects. If no
		**	objects are selected, then fall into the home case.
		*/
		case VK_HOME:
			if (CurrentObject.Count()) {
				Map.Center_Map();
				Map.Flag_To_Redraw(true);
				break;
			}
			// Fall into next case.

		/*
		**	Center the map about the construction yard or construction vehicle
		**	if one is present.
		*/
		case VK_H:
			for (index = 0; index < Units.Count(); index++) {
				UnitClass * unit = Units.Ptr(index);

				if (unit && !unit->IsInLimbo && unit->House == PlayerPtr && *unit == UNIT_MCV) {
					Unselect_All();
					unit->Select();
					break;
				}
			}
			for (index = 0; index < Buildings.Count(); index++) {
				BuildingClass * building = Buildings.Ptr(index);

				if (building && !building->IsInLimbo && building->House == PlayerPtr && *building == STRUCT_CONST) {
					Unselect_All();
					building->Select();
					break;
				}
			}
			Map.Center_Map();
			Map.Flag_To_Redraw(true);
			break;

#ifdef CHEAT_KEYS
		/*
		**	Toggle free scrolling mode.
		*/
		case VK_F:
			Options.IsFreeScroll = (Options.IsFreeScroll == false);
			break;
#endif

		/*
		**	If the "N" key is pressed, then select the next object.
		*/
		case VK_N:
			if (action) {
				obj = Map.Prev_Object(CurrentObject.Count() ? CurrentObject[0] : NULL);
			} else {
				obj = Map.Next_Object(CurrentObject.Count() ? CurrentObject[0] : NULL);
			}
			if (obj) {
				Unselect_All();
			 	obj->Select();
				Map.Center_Map();
				Map.Flag_To_Redraw(true);
			}
			break;

		/*
		** For multiplayer, 'R' pops up the surrender dialog.
		*/
		case VK_R:
			if (/*GameToPlay != GAME_NORMAL &&*/ !PlayerPtr->IsDefeated) {
				SpecialDialog = SDLG_SURRENDER;
				input = KN_NONE;
			}
			break;

		/*
		**	Handle making and breaking alliances.
		*/
		case VK_A:
			if (GameToPlay != GAME_NORMAL || Debug_Flag) {
				if (CurrentObject.Count() && !PlayerPtr->IsDefeated) {
					if (CurrentObject[0]->Owner() != PlayerPtr->Class->House) {
						OutList.Add(EventClass(EventClass::ALLY, (int)CurrentObject[0]->Owner()));
					}
				}
			}
			break;

		/*
		**	Control the remembered tactical location.
		*/
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F10:
			if (!Debug_Map) {
				Handle_View(KN_To_VK(plain) - VK_F7, action);
			}
			break;
#if (0)
 case VK_F11:
	Winsock.Set_Protocol_UDP(FALSE);
	break;

 case VK_F12:
	Winsock.Set_Protocol_UDP(TRUE);
	break;
#endif	//(0)


		/*
		**	Control the custom team select state.
		*/
		case VK_1:
		case VK_2:
		case VK_3:
		case VK_4:
		case VK_5:
		case VK_6:
		case VK_7:
		case VK_8:
		case VK_9:
		case VK_0:
			Handle_Team(KN_To_VK(plain) - VK_1, action);
			break;

		/*
		**	All selected units will go into idle mode.
		*/
		case VK_S:
			if (CurrentObject.Count()) {
				for (int index = 0; index < CurrentObject.Count(); index++) {
					ObjectClass const * tech = CurrentObject[index];

					if (tech && (tech->Can_Player_Move() || (tech->Can_Player_Fire() &&
							tech->What_Am_I() != RTTI_BUILDING))) {
						OutList.Add(EventClass(EventClass::IDLE, tech->As_Target()));
					}
				}
			}
			break;

		/*
		**	All selected units will attempt to scatter.
		*/
		case VK_X:
			if (CurrentObject.Count()) {
				for (int index = 0; index < CurrentObject.Count(); index++) {
					ObjectClass const * tech = CurrentObject[index];

					if (tech && tech->Can_Player_Move()) {
						OutList.Add(EventClass(EventClass::SCATTER, tech->As_Target()));
					}
				}
			}
			break;

		/*
		**	All selected units will attempt to go into guard area mode.
		*/
		case VK_G:
			if (CurrentObject.Count()) {
				for (int index = 0; index < CurrentObject.Count(); index++) {
					ObjectClass const * tech = CurrentObject[index];

					if (tech && tech->Can_Player_Move() && tech->Can_Player_Fire()) {
						OutList.Add(EventClass(tech->As_Target(), MISSION_GUARD_AREA));
					}
				}
			}
			break;

		default:
			break;
	}

#ifdef NEVER
	FacingType	facing = KN_To_Facing(input);

	/*
	**	Scroll the map according to the cursor key pressed.
	*/
	if (facing != FACING_NONE) {
		Map.Scroll_Map(facing);
		input = 0;
		facing = FACING_NONE;
	}
#endif

#ifdef NEVER
	/*
	**	If the <TAB> key is pressed, then select the next object.
	*/
	if (input == KN_TAB) {
		ObjectClass * obj = Map.Next_Object(CurrentObject);
		if (obj) {
			if (CurrentObject) {
				CurrentObject->Unselect();
			}
		 	obj->Select();
		}
	}
#endif

#ifdef CHEAT_KEYS
	if (Debug_Flag && input && (input & KN_RLSE_BIT) == 0) {
		Debug_Key(input);
	}
#endif
}


#ifndef DEMO
/***********************************************************************************************
 * Message_Input -- allows inter-player message input processing                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		input		key value																							 *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		none.																												 *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		MAX_MESSAGE_LENGTH has increased over the DOS version. COMPAT_MESSAGE_LENGTH reflects   *                                                                                             *
 *  the length of the DOS message and also the length of the message in the packet header.     *
 *  To allow transmission of longer messages I split the message into COMPAT_MESSAGE_LENGTH-4  *
 *  sized chunks and use the extra space after the zero terminator to specify which segment    *
 *  of the whole message this is and also to supply a crc for the string.                      *
 *  This allows message segments to arrive out of order and still be displayed correctly.      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1995 BRR : Created.                                                                 *
 *   03/26/1995  ST : Modified to break up longer messages into multiple packets               *
 *=============================================================================================*/
static void Message_Input(KeyNumType &input)
{
	int 					rc;
	char 					txt[MAX_MESSAGE_LENGTH+12];
	int 					id;
//PG_TO_FIX
#if (0)
	SerialPacketType 	*serial_packet;
	int 					i;
	int					message_length;
	int					sent_so_far;
	unsigned short		magic_number;
	unsigned short		crc;
#endif
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;


	/*
	**	Check keyboard input for a request to send a message.
	**	The 'to' argument for Add_Edit is prefixed to the message buffer; the
	**	message buffer is big enough for the 'to' field plus MAX_MESSAGE_LENGTH.
	**	To send the message, calling Get_Edit_Buf retrieves the buffer minus the
	**	'to' portion.  At the other end, the buffer allocated to display the
	**	message must be MAX_MESSAGE_LENGTH plus the size of "From: xxx (house)".
	*/
	if (input >= KN_F1 && input < (KN_F1 + MPlayerMax) &&
		Messages.Get_Edit_Buf()==NULL) {
		memset (txt, 0, 40);

		/*
		**	For a serial game, send a message on F1 or F4; set 'txt' to the
		**	"Message:" string & add an editable message to the list.
		*/
		if (GameToPlay==GAME_NULL_MODEM
			|| GameToPlay==GAME_MODEM){
			//|| GameToPlay == GAME_INTERNET) {
			if (input==KN_F1 || input==(KN_F1 + MPlayerMax - 1)) {

				strcpy(txt,Text_String(TXT_MESSAGE));	// "Message:"

				Messages.Add_Edit (MPlayerTColors[MPlayerColorIdx],
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 180*factor);

				Map.Flag_To_Redraw(false);
			}
		} else {

			/*
			**	For a network game:
			**	F1-F3 = "To <name> (house):" (only allowed if we're not in ObiWan mode)
			**	F4 = "To All:"
			*/
			if (GameToPlay == GAME_IPX || GameToPlay == GAME_INTERNET) {
				if (input==(KN_F1 + MPlayerMax - 1) && Messages.Get_Edit_Buf()==NULL) {

					MessageAddress = IPXAddressClass();		// set to broadcast
					strcpy(txt,Text_String(TXT_TO_ALL));	// "To All:"

					Messages.Add_Edit(MPlayerTColors[MPlayerColorIdx],
						TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 180*factor);

					Map.Flag_To_Redraw(false);
				} else {
					if (Messages.Get_Edit_Buf()==NULL) {
						if ((input - KN_F1) < Ipx.Num_Connections() && !MPlayerObiWan) {

							id = Ipx.Connection_ID(input - KN_F1);
							MessageAddress = (*(Ipx.Connection_Address (id)));
							sprintf(txt,Text_String(TXT_TO),Ipx.Connection_Name(id));

							Messages.Add_Edit(MPlayerTColors[MPlayerColorIdx],
								TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, 180*factor);

							Map.Flag_To_Redraw(false);
						}
					}
				}
			}
		}
	}

	/*
	** Function key input is meaningless beyond this point
	*/
	if (input >= KN_F1 && input <= KN_F10) return;
	if (input >= KN_F11 && input <= KN_F12) return;

	/*
	**	Process message-system input; send the message out if RETURN is hit.
	*/
	rc = Messages.Input(input);

	/*
	**	If a single character has been added to an edit buffer, update the display.
	*/
	if (rc == 1) {
		Map.Flag_To_Redraw(false);
	}

	/*
	**	If backspace was hit, redraw the map.  This assumes the map is going to
	**	completely refresh all cells covered by the messages.  Set DisplayClass's
	**	IsToRedraw to true to tell it to re-compute the cells that it needs to
	**	redraw.
	*/
	if (rc==2) {
		Map.Flag_To_Redraw(false);
		Map.DisplayClass::IsToRedraw = true;
	}


	/*
	**	Send a message
	*/
	if (rc==3) {
//
// PG_TO_FIX		  
#if (0)		
		/*.....................................................................
		Store this message in our LastMessage buffer; the computer may send
		us a version of it later.
		.....................................................................*/
		if (strlen(Messages.Get_Edit_Buf())) {
			strcpy(LastMessage,Messages.Get_Edit_Buf());
		}

		message_length = strlen(Messages.Get_Edit_Buf());

		long actual_message_size;
		char *the_string;

		/*
		**	Serial game: fill in a SerialPacketType & send it.
		**	(Note: The size of the SerialPacketType.Command must be the same as
		**	the EventClass.Type!)
		*/
		if (GameToPlay==GAME_NULL_MODEM
			|| GameToPlay==GAME_MODEM){
			//|| GameToPlay==GAME_INTERNET) {

			sent_so_far = 0;
			magic_number = MESSAGE_HEAD_MAGIC_NUMBER;
			crc = (unsigned short) (Calculate_CRC(Messages.Get_Edit_Buf(), message_length) & 0xffff);

			while (sent_so_far < message_length){

				serial_packet = (SerialPacketType *)NullModem.BuildBuf;

				serial_packet->Command = SERIAL_MESSAGE;
				strcpy (serial_packet->Name, MPlayerName);
				memcpy (serial_packet->Message, Messages.Get_Edit_Buf()+sent_so_far, COMPAT_MESSAGE_LENGTH-5);

				/*
				** Steve I's stuff for splitting message on word boundries
				*/
				actual_message_size = COMPAT_MESSAGE_LENGTH - 5;

				/* Start at the end of the message and find a space with 10 chars. */
				the_string = serial_packet->Message;
				while ( (COMPAT_MESSAGE_LENGTH -5) -actual_message_size < 10 &&
					the_string[actual_message_size] != ' '){
					--actual_message_size;
				}
				if ( the_string[actual_message_size] == ' ' ){

					/* Now delete the extra characters after the space (they musnt print) */
					for ( int i=0 ; i< (COMPAT_MESSAGE_LENGTH-5) - actual_message_size; i++ ){
						the_string[i + actual_message_size] = 0xff;
					}
				}else{
					actual_message_size = COMPAT_MESSAGE_LENGTH - 5;
				}


				*(serial_packet->Message + COMPAT_MESSAGE_LENGTH-5) = 0;
				/*
				** Flag this message segment as either a message head or a message tail.
				*/
				*((unsigned short*)(serial_packet->Message + COMPAT_MESSAGE_LENGTH-4)) = magic_number;
				*((unsigned short*)(serial_packet->Message + COMPAT_MESSAGE_LENGTH-2)) = crc;
				serial_packet->ID = MPlayerLocalID;

				NullModem.Send_Message(NullModem.BuildBuf, sizeof(SerialPacketType), 1);

				magic_number++;
				sent_so_far += actual_message_size;	//COMPAT_MESSAGE_LENGTH-5;
			}

		} else {

			/*
			**	Network game: fill in a GlobalPacketType & send it.
			*/
			if (GameToPlay == GAME_IPX || GameToPlay == GAME_INTERNET) {

				sent_so_far = 0;
 				magic_number = MESSAGE_HEAD_MAGIC_NUMBER;
				crc = (unsigned short) (Calculate_CRC(Messages.Get_Edit_Buf(), message_length) & 0xffff);

				while (sent_so_far < message_length){

					GPacket.Command = NET_MESSAGE;
					strcpy (GPacket.Name, MPlayerName);
					memcpy (GPacket.Message.Buf, Messages.Get_Edit_Buf()+sent_so_far, COMPAT_MESSAGE_LENGTH-5);

						/*
						** Steve I's stuff for splitting message on word boundries
						*/
						actual_message_size = COMPAT_MESSAGE_LENGTH - 5;

						/* Start at the end of the message and find a space with 10 chars. */
						the_string = GPacket.Message.Buf;
						while ( (COMPAT_MESSAGE_LENGTH -5) -actual_message_size < 10 &&
							the_string[actual_message_size] != ' '){
							--actual_message_size;
						}
						if ( the_string[actual_message_size] == ' ' ){

							/* Now delete the extra characters after the space (they musnt print) */
							for ( int i=0 ; i< (COMPAT_MESSAGE_LENGTH-5) - actual_message_size; i++ ){
								the_string[i + actual_message_size] = 0xff;
							}
						}else{
							actual_message_size = COMPAT_MESSAGE_LENGTH - 5;
						}

					*(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-5) = 0;
					/*
					** Flag this message segment as either a message head or a message tail.
					*/
					*((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-4)) = magic_number;
					*((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-2)) = crc;

					GPacket.Message.ID = MPlayerLocalID;
					GPacket.Message.NameCRC = Compute_Name_CRC(MPlayerGameName);

					/*
					**	If 'F4' was hit, MessageAddress will be a broadcast address; send
					**	the message to every player we have a connection with.
					*/
					if (MessageAddress.Is_Broadcast()) {
						for (i = 0; i < Ipx.Num_Connections(); i++) {
							Ipx.Send_Global_Message(&GPacket, sizeof(GlobalPacketType), 1,
								Ipx.Connection_Address(Ipx.Connection_ID(i)));
							Ipx.Service();
						}
					} else {

						/*
						**	Otherwise, MessageAddress contains the exact address to send to.
						**	Send to that address only.
						*/
						Ipx.Send_Global_Message(&GPacket, sizeof(GlobalPacketType), 1,
							&MessageAddress);
						Ipx.Service();
					}

				magic_number++;
				sent_so_far += actual_message_size;	//COMPAT_MESSAGE_LENGTH-5;
				}
			}

		}

		/*
		**	Tell the map to completely update itself, since a message is now missing.
		*/
		Map.Flag_To_Redraw(true);
#endif
	}
}
#endif


/***********************************************************************************************
 * Color_Cycle -- Handle the general palette color cycling.                                    *
 *                                                                                             *
 *    This is a maintenance routine that handles the color cycling. It should be called as     *
 *    often as necessary to achieve smooth color cycling effects -- at least 8 times a second. *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  true if palette changed                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   06/10/1994 JLB : Uses new cycle color values.                                             *
 *   12/21/1994 JLB : Handles text fade color.                                                 *
 *=============================================================================================*/
bool Color_Cycle(void)
{
	static CountDownTimerClass _timer(BT_SYSTEM,0L);
	static CountDownTimerClass _ftimer(BT_SYSTEM,0L);
	static bool _up = false;
	bool changed = false;

	/*
	**	Process the fading white color. It is used for the radar box and other glowing
	**	game interface elements.
	*/
	if (!_ftimer.Time()) {
		_ftimer.Set(TIMER_SECOND/8);

		/*
		**	Pulse the pulsing text color.
		*/
		#define	STEP_RATE	5
		if (_up) {
			GamePalette[767] += STEP_RATE;
			GamePalette[766] += STEP_RATE;
			GamePalette[765] += STEP_RATE;
			if (GamePalette[767] > MAX_CYCLE_COLOR) {
				GamePalette[767] = MAX_CYCLE_COLOR;
				GamePalette[766] = MAX_CYCLE_COLOR;
				GamePalette[765] = MAX_CYCLE_COLOR;
				_up = false;
			}
		} else {
			GamePalette[767] -= STEP_RATE;
			GamePalette[766] -= STEP_RATE;
			GamePalette[765] -= STEP_RATE;
			if ((unsigned)GamePalette[767] < MIN_CYCLE_COLOR) {
				GamePalette[767] = MIN_CYCLE_COLOR;
				GamePalette[766] = MIN_CYCLE_COLOR;
				GamePalette[765] = MIN_CYCLE_COLOR;
				_up = true;
			}
		}
		changed = true;
	}

	/*
	**	Process the color cycling effects -- water.
	*/
	if (!_timer.Time()) {
		unsigned char	colors[3];

		_timer.Set(TIMER_SECOND/4);

		memmove(colors, &GamePalette[(CYCLE_COLOR_START+CYCLE_COLOR_COUNT-1)*3], sizeof(colors));
		memmove(&GamePalette[(CYCLE_COLOR_START+1)*3], &GamePalette[CYCLE_COLOR_START*3], (CYCLE_COLOR_COUNT-1)*3);
		memmove(&GamePalette[CYCLE_COLOR_START*3], colors, sizeof(colors));
		changed = true;
	}

	/*
	**	If any of the processing functions changed the palette, then this palette must be
	**	passed to the system.
	*/
	if (changed) {
		Wait_Vert_Blank();
		Set_Palette(GamePalette);
		return (true);
	}
	return (false);
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
#ifndef DEMO
	int i;
	int id;
	int color;
	unsigned short	magic_number;
	unsigned short crc;
#endif

	/*
	**	Score maintenance
	*/
	if (SampleType) {
		Theme.AI();
		Speak_AI();
	}

#ifndef DEMO
	/*
	**	Network maintenance
	*/
	if (GameToPlay == GAME_IPX || GameToPlay == GAME_INTERNET) {

		Ipx.Service();

		/*
		** Read packets only if the game is "closed", so we don't steal global
		** messages from the connection dialogs.
		*/
		if (!NetOpen) {
			if (Ipx.Get_Global_Message (&GPacket, &GPacketlen, &GAddress, &GProductID)) {
				if (GProductID == IPXGlobalConnClass::COMMAND_AND_CONQUER) {

					/*
					**	If this is another player signing off, remove the connection &
					**	mark that player's house as non-human, so the computer will take
					**	it over.
					*/
					if (GPacket.Command == NET_SIGN_OFF) {
						for (i = 0; i < Ipx.Num_Connections(); i++) {

							id = Ipx.Connection_ID(i);

							if (!strcmp (GPacket.Name, Ipx.Connection_Name(id) ) &&
								GAddress == (*Ipx.Connection_Address(id))) {

								CCDebugString ("C&C95 = Destroying connection due to sign off\n");
								Destroy_Connection (id,0);
							}
						}
					} else {

						/*
						**	Process a message from another user.
						*/
						if (GPacket.Command == NET_MESSAGE) {
							bool msg_ok = false;
							char txt[80];

							/*
							** If NetProtect is set, make sure this message came from within
							** this game.
							*/
							if (!NetProtect) {
								msg_ok = true;
							} else {
								if (GPacket.Message.NameCRC == Compute_Name_CRC(MPlayerGameName)) {
									msg_ok = true;
								} else {
									msg_ok = false;
								}
							}

							if (msg_ok) {
								sprintf(txt,Text_String (TXT_FROM), GPacket.Name, GPacket.Message.Buf);
								magic_number = *((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-4));
								crc = *((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-2));
								color = MPlayerID_To_ColorIndex(GPacket.Message.ID);
								Messages.Add_Message(txt, MPlayerTColors[color],
									TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 600, magic_number, crc);

								/*
								**	Tell the map to do a partial update (just to force the messages
								**	to redraw).
								*/
								Map.Flag_To_Redraw(false);

								/*
								**	Save this message in our last-message buffer
								*/
								if (strlen(GPacket.Message.Buf)) {
									strcpy(LastMessage, GPacket.Message.Buf);
								}
							}
						} else {
							Process_Global_Packet(&GPacket, &GAddress);
						}
					}
				}
			}
		}
	}

	/*
	**	Modem and Null Modem maintenance
	*/
	if (GameToPlay == GAME_NULL_MODEM
	|| ((GameToPlay == GAME_MODEM) && ModemService)){
	//|| GameToPlay == GAME_INTERNET) {
		
		// PG_TO_FIX
		//NullModem.Service();
	}
#endif
}


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
char const *Language_Name(char const *basename)
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
SourceType Source_From_Name(char const *name)
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
char const *Name_From_Source(SourceType source)
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
TheaterType Theater_From_Name(char const *name)
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
 * WARNINGS:   Will delay for up to 1/15 of a second.                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/04/1995 JLB : Created.                                                                 *
 *   03/06/1995 JLB : Fixed.                                                                   *
 *=============================================================================================*/
static void Sync_Delay(void)
{
	/*
	**	Delay one tick and keep a record that one tick was "wasted" here.
	**	This accumulates into a running histogram of performance.
	*/
	SpareTicks += FrameTimer.Time();
	while (FrameTimer.Time()) {
		Color_Cycle();
		Call_Back();

		if (SpecialDialog == SDLG_NONE) {
			WWMouse->Erase_Mouse(&HidPage, TRUE);
			KeyNumType input = KN_NONE;
			int x, y;
			WWMouse->Erase_Mouse(&HidPage, TRUE);
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
extern void Check_For_Focus_Loss(void);
void Reallocate_Big_Shape_Buffer(void);


bool Main_Loop()
{
	KeyNumType	input;					// Player input.
	int 			x;
	int 			y;
	int framedelay;

//	InMainLoop = true;

	/*
	** I think I'm gonna cry if this makes it work
	*/
	if (Get_Mouse_State())Show_Mouse();

	/*
	** Call the focus loss handler
	*/
	Check_For_Focus_Loss();

	/*
	** Allocate extra memory for uncompressed shapes as needed
	*/
	Reallocate_Big_Shape_Buffer();

	/*
	** Sync-bug trapping code
	*/
	if (Frame >= TrapFrame) {
		Trap_Object();
	}

	//
	// Initialize our AI processing timer
	//
	ProcessTimer.Set(0, true);


#if 1
	if (TrapCheckHeap) {
		Debug_Trap_Check_Heap = true;
	}
#endif

#ifdef CHEAT_KEYS
 Heap_Dump_Check( "After Trap" );

	/*
	**	Update the running status debug display.
	*/
	Self_Regulate();
#endif

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
	if (GameToPlay != GAME_NORMAL && CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		framedelay = 60 / DesiredFrameRate;
		FrameTimer.Set(framedelay);
	} else {
		FrameTimer.Set(Options.GameSpeed);
	}

	/*
	**	Update the display, unless we're inside a dialog.
	*/
	if (!PlaybackGame) {
		if (SpecialDialog == SDLG_NONE && GameInFocus) {

			WWMouse->Erase_Mouse(&HidPage, TRUE);
			Map.Input(input, x, y);
			if (input) {
				Keyboard_Process(input);
			}
//			HidPage.Lock();
			Map.Render();
//			HidPage.Unlock();
		}
	}

	/*
	** Save map's position & selected objects, if we're recording the game.
	*/
	if (RecordGame || PlaybackGame) {
		Do_Record_Playback();
	}

	/*
	** Sort the map's ground layer by y-coordinate value.  This is done
	** outside the IsToRedraw check, for the purposes of game sync'ing
	** between machines; this way, all machines will sort the Map's
	** layer in the same way, and any processing done that's based on
	** the order of this layer will sync on different machines.
	*/
	Map.Layer[LAYER_GROUND].Sort();

//	Heap_Dump_Check( "Before Logic.AI" );

	/*
	**	AI logic operations are performed here.
	*/
	Logic.AI();

//	Heap_Dump_Check( "After Logic.AI" );

	/*
	**	Manage the inter-player message list.  If Manage() returns true, it means
	**	a message has expired & been removed, and the entire map must be updated.
	*/
	if (Messages.Manage()) {
		HiddenPage.Clear();
		Map.Flag_To_Redraw(true);
	}

	//
	// Measure how long it took to process the AI
	//
	ProcessTicks += ProcessTimer.Time();
	ProcessFrames++;

//	Heap_Dump_Check( "Before Queue_AI" );

	/*
	**	Process all commands that are ready to be processed.
	*/
	Queue_AI();

// Heap_Dump_Check( "After Queue_AI" );

	/*
	**	Keep track of elapsed time in the game.
	*/
	Score.ElapsedTime += TIMER_SECOND / TICKS_PER_SECOND;

	Call_Back();

// Heap_Dump_Check( "After Call_Back" );

	/*
	**	Perform any win/lose code as indicated by the global control flags.
	*/
	if (EndCountDown) EndCountDown--;

	/*
	**	Check for player wins or loses according to global event flag.
	*/



	if (PlayerWins) {

		if (GameToPlay == GAME_INTERNET && !GameStatisticsPacketSent){
			Register_Game_End_Time();
			Send_Statistics_Packet();
		}

		WWMouse->Erase_Mouse(&HidPage, TRUE);
		PlayerLoses = false;
		PlayerWins = false;
		PlayerRestarts = false;
		Map.Help_Text(TXT_NONE);
		Do_Win();
	}
	if (PlayerLoses) {

		if (GameToPlay == GAME_INTERNET && !GameStatisticsPacketSent){
			Register_Game_End_Time();
			Send_Statistics_Packet();
		}

		WWMouse->Erase_Mouse(&HidPage, TRUE);
		PlayerWins = false;
		PlayerLoses = false;
		PlayerRestarts = false;
		Map.Help_Text(TXT_NONE);
		Do_Lose();
	}
	if (PlayerRestarts) {
		WWMouse->Erase_Mouse(&HidPage, TRUE);
		PlayerWins = false;
		PlayerLoses = false;
		PlayerRestarts = false;
		Map.Help_Text(TXT_NONE);
		Do_Restart();
	}

	/*
	**	The frame logic has been completed. Increment the frame
	**	counter.
	*/
	Frame++;

	/*
	** Very rarely, the human players will get a message from the computer.
	*/
	if (GameToPlay != GAME_NORMAL && MPlayerGhosts && IRandom(0,10000) == 1) {
		Computer_Message();
	}

	/*
	** Is there a memory trasher altering the map??
	*/
	if (Debug_Check_Map) {
		if (!Map.Validate()) {
#ifdef GERMAN
			if (CCMessageBox().Process ("Kartenfehler!","Halt","Weiter")==0)
#else
#ifdef FRENCH
			if (CCMessageBox().Process ("Erreur de carte!","Stop","Continuer")==0)
#else
			if (CCMessageBox().Process ("Map Error!","Stop","Continue")==0)
#endif
#endif
				GameActive = false;
			Map.Validate();		// give debugger a chance to catch it
		}
	}

	Sync_Delay();
//	InMainLoop = false;
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
		HiddenPage.Clear();
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
		HiddenPage.Clear();
		Map.Flag_To_Redraw(true);
		Map.Render();
	}
}

#endif

#if (0)
#define VQ_THREAD_BUFFER_SIZE 1024*1024
#define VQ_THREAD_BUFFER_CHUNK VQ_THREAD_BUFFER_SIZE/4
unsigned char *VQThreadBuffer = NULL;
volatile bool ThreadReading = false;
unsigned long VQThreadBlockHead;
unsigned long VQThreadBlockTail;
unsigned long VQBytesLeft;
unsigned long VQBytesRead;


void Init_VQ_Threading(CCFileClass *file)
{
	if (!VQThreadBuffer){
		VQThreadBuffer = new unsigned char [VQ_THREAD_BUFFER_SIZE];
	}
	Force_VM_Page_In(VQThreadBuffer, VQ_THREAD_BUFFER_SIZE);
	VQThreadBlockHead = 0;
	VQThreadBlockTail = 0;
	VQBytesRead = 0;
	VQBytesLeft = file->Size();
}


void Cleanup_VQ_Threading(void)
{
	while (ThreadReading){}
	if (VQThreadBuffer){
		delete VQThreadBuffer;
		VQThreadBuffer = NULL;
	}
}

unsigned long __stdcall Thread_Read(void *file)
{
	int bytes_to_read;
	int left_to_read;
	int read_this_time;
	unsigned long head;
	int sleep_time;

	CCFileClass *ccfile = (CCFileClass*)file;

	bytes_to_read = MIN (VQBytesLeft, VQ_THREAD_BUFFER_CHUNK);

	if (!bytes_to_read){
		ThreadReading = false;
		return(0);
	}

	left_to_read = bytes_to_read;

	while (left_to_read){
		read_this_time = MIN(8*1024, left_to_read);
		//if (read_this_time & 3){
			ccfile->Read(VQThreadBuffer+VQThreadBlockHead, read_this_time);
		//}else{
		//	ccfile->Read(VQThreadBuffer+VQThreadBlockHead, read_this_time/4);
		//	ccfile->Read(VQThreadBuffer+VQThreadBlockHead+read_this_time/4, read_this_time/4);
		//	ccfile->Read(VQThreadBuffer+VQThreadBlockHead+(read_this_time/4)*2, read_this_time/4);
		//	ccfile->Read(VQThreadBuffer+VQThreadBlockHead+(read_this_time/4)*3, read_this_time/4);
		//}
		VQThreadBlockHead += read_this_time;
		left_to_read -= read_this_time;

		head = VQThreadBlockHead;
		if (head<VQThreadBlockTail) head+= VQ_THREAD_BUFFER_SIZE;
		sleep_time = head - VQThreadBlockTail;
		sleep_time = sleep_time/ (VQ_THREAD_BUFFER_CHUNK/32);
		sleep_time += 2;
		if (sleep_time<1) sleep_time=1;
		Sleep(sleep_time);
	}

	VQThreadBlockHead &= VQ_THREAD_BUFFER_SIZE-1;
	VQBytesLeft -= bytes_to_read;
	ThreadReading = false;
	return (0);
}



void Read_VQ_Thread_Block(CCFileClass *file)
{
	HANDLE 	thread_handle;
	DWORD		thread_id;
	if (!ThreadReading){
		//_beginthreadex (&Thread_Read, NULL, 16*1024, NULL);
		ThreadReading = true;
		thread_handle = CreateThread(NULL, 0, &Thread_Read, (void*)file, 0, &thread_id);
		//SetThreadPriority (thread_handle, THREAD_PRIORITY_IDLE);

		CloseHandle (thread_handle);
	}
}



int VQ_Thread_Read (CCFileClass *file, void *buffer, long bytes)
{
	long bytes_to_read;

	do {
		if (VQThreadBlockHead > VQThreadBlockTail){
			bytes_to_read = MIN(bytes, VQThreadBlockHead-VQThreadBlockTail);

		}else{
			bytes_to_read = MIN(bytes, VQThreadBlockHead+VQ_THREAD_BUFFER_SIZE - VQThreadBlockTail);
		}

	}while(ThreadReading && bytes_to_read<bytes);


	if (VQThreadBlockTail+bytes_to_read > VQ_THREAD_BUFFER_SIZE){

		int first_chunk = VQ_THREAD_BUFFER_SIZE - VQThreadBlockTail;
		int second_chunk = bytes_to_read - first_chunk;

		memcpy (buffer, VQThreadBuffer + VQThreadBlockTail, first_chunk);
		memcpy ((unsigned char*)buffer + first_chunk, VQThreadBuffer, second_chunk);
	}else{
		memcpy (buffer, VQThreadBuffer + VQThreadBlockTail, bytes_to_read);
	}

	VQThreadBlockTail += bytes_to_read;
	VQThreadBlockTail &= VQ_THREAD_BUFFER_SIZE - 1;

	unsigned long head = VQThreadBlockHead;
	if (head<VQThreadBlockTail) head+= VQ_THREAD_BUFFER_SIZE;
	if (head-VQThreadBlockTail < VQ_THREAD_BUFFER_CHUNK && !ThreadReading) Read_VQ_Thread_Block(file);

	VQBytesRead += bytes_to_read;
	return (bytes_to_read);

}


int VQ_Thread_Seek (long bytes)
{
	VQThreadBlockTail += bytes;
	VQBytesRead += bytes;
	return (VQBytesRead);
}





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
long MixFileHandler(VQAHandle *vqa, long action, void *buffer, long nbytes)
{
	CCFileClass *file;
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
			error = VQ_Thread_Read (file, buffer, nbytes);
			//error = (file->Read(buffer, (unsigned short)nbytes) != (unsigned short)nbytes);
			if (error == nbytes) error = 0;
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
			//error = (file->Seek(nbytes, SEEK_CUR) == -1);
			VQ_Thread_Seek(nbytes);
			error = 0;
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
					//file->Set_Buffer_Size(8*1024);
				} else {
					delete file;
					file = 0;
					error = 1;
				}
			} else {
				error = 1;
			}

			if (error != -1){
				Init_VQ_Threading(file);
				Read_VQ_Thread_Block(file);
				CountDownTimerClass timer;
				timer.Set(60);
				while (ThreadReading || timer.Time()){}
			}
			break;

		case VQACMD_CLOSE:
			Cleanup_VQ_Threading();
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
	}

	return(error);
}
#endif	//(0)
//#if (0)


// PG_TO_FIX
typedef void* VQAHandle;

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
long MixFileHandler(VQAHandle *vqa, long action, void *buffer, long nbytes)
{
	return 0;;
#if (0)
	CCFileClass *file;
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
					file->Set_Buffer_Size(8*1024);
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
	}

	return(error);
#endif
}

//#endif	//(0)


void Rebuild_Interpolated_Palette(unsigned char *interpal)
{
	for (int y=0 ; y<255 ; y++){
		for (int x=y+1 ; x<256 ; x++){
			*(interpal + (y*256+x)) = *(interpal + (x*256+y));
		}
	}
}


unsigned char 	*InterpolatedPalettes[100];
BOOL				PalettesRead;
unsigned			PaletteCounter;


int Load_Interpolated_Palettes(char const *filename, BOOL add)
{
	int	num_palettes=0;
	int	i;
	int 	start_palette;

	PalettesRead = FALSE;
	CCFileClass	file(filename);

//	RawFileClass	*palette_file;

	if (!add){
		for (i=0 ; i<ARRAY_SIZE(InterpolatedPalettes) ; i++){
			InterpolatedPalettes[i]=NULL;
		}
		start_palette=0;
	} else {
		for (start_palette=0 ; start_palette< ARRAY_SIZE(InterpolatedPalettes) ; start_palette++){
			if (!InterpolatedPalettes[start_palette]) break;
		}
	}

//	palette_file = new RawFileClass (filename);
//	if (file.Is_Available()){

		file.Open(READ);
		file.Read(&num_palettes , 4);

		for (i=0 ; i<num_palettes ; i++){
			InterpolatedPalettes[i+start_palette] = (unsigned char *)malloc (65536);
			memset (InterpolatedPalettes[i+start_palette],0,65536);
			for (int y=0 ; y<256 ;y++){
				file.Read (InterpolatedPalettes[i+start_palette] + y*256 , y+1);
			}

			Rebuild_Interpolated_Palette(InterpolatedPalettes[i+start_palette]);
		}

		PalettesRead = TRUE;
		file.Close();
//	}
	PaletteCounter = 0;
	return (num_palettes);
}


void Free_Interpolated_Palettes(void)
{
	for (int i=0 ; i<ARRAY_SIZE(InterpolatedPalettes) ; i++){
		if (InterpolatedPalettes[i]){
			free(InterpolatedPalettes[i]);
			InterpolatedPalettes[i]=NULL;
		}
	}
}


/***********************************************************************************************
 * Play_Movie -- Plays a VQ movie.                                                             *
 *                                                                                             *
 *    Use this routine to play a VQ movie. It will disptach the specified movie to the         *
 *    VQ player. The routine will not return until the movie has finished playing.             *
 *                                                                                             *
 * INPUT:   file  -- The file object that contains the movie.                                  *
 *                                                                                             *
 *          anim  -- The anim control and configuration structure that controls how the        *
 *                   movie will be played.                                                     *
 *                                                                                             *
 *          clrscrn -- Set to 1 to clear the screen when the movie is over                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
extern	BOOL	InMovie;
extern bool	VQPaletteChange;
extern void Suspend_Audio_Thread(void);
extern void Resume_Audio_Thread(void);

//Play 
extern void Play_Movie_GlyphX(const char * movie_name, ThemeType theme );

void Play_Movie(char const * name, ThemeType theme, bool clrscrn)
{
#if (1)
	if (strcmp(name, "x") == 0 || strcmp(name, "X") == 0) {
		return;
	}

	Play_Movie_GlyphX(name, theme);
	return;
#else
	/*
	** Don't play movies in editor mode
	*/
	if (Debug_Map) {
		return;
	}

	/*
	** Don't play movies in multiplayer mode
	*/
	if (GameToPlay != GAME_NORMAL) {
		return;
	}

	memset (&PaletteInterpolationTable[0][0],0,65536);

	if (name) {
		char fullname[_MAX_FNAME+_MAX_EXT];
		char palname [_MAX_FNAME+_MAX_EXT];

		_makepath(fullname, NULL, NULL, name, ".VQA");
		_makepath(palname , NULL, NULL, name, ".VQP");
		#ifdef CHEAT_KEYS
			Mono_Set_Cursor(0, 0);Mono_Printf("[%s]", fullname);
		#endif

		/*
		**	Reset the anim control structure.
		*/
		Anim_Init();
		VQPaletteChange = false;

		/*
		**	Prepare to play a movie. First hide the mouse and stop any score that is playing.
		**	While the score (if any) is fading to silence, fade the palette to black as well.
		**	When the palette has finished fading, wait until the score has finished fading
		**	before launching the movie.
		*/
		Hide_Mouse();
		//Theme.Stop();
		//Theme.AI();
		Theme.Queue_Song(theme);
		if (PreserveVQAScreen == 0) {
			Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
			VisiblePage.Clear();
			memset(BlackPalette, 0x01, 768);
			Set_Palette(BlackPalette);
			memset(BlackPalette, 0x00, 768);
		}
		PreserveVQAScreen = 0;
		Keyboard::Clear();

		VQAHandle *vqa = NULL;

		if (!Debug_Quiet && Get_Digi_Handle() != -1) {
			AnimControl.OptionFlags |= VQAOPTF_AUDIO;
		} else {
			AnimControl.OptionFlags &= ~VQAOPTF_AUDIO;
		}


		if ((vqa = VQA_Alloc()) != NULL) {
			VQA_Init(vqa, MixFileHandler);
			if (VQA_Open(vqa, fullname, &AnimControl) == 0) {
				Brokeout = false;
				//Suspend_Audio_Thread();

#if (FRENCH | GERMAN | JAPANESE)
				/*
				** Kludge to use the old palette interpolation table for CC2TEASE
				** unless the covert CD is inserted.
				*/
				if (!stricmp (palname, "CC2TEASE.VQP")){
					int cd_index = Get_CD_Index(CCFileClass::Get_CD_Drive(), 1*60);
					/*
					** If cd_index == 2 then its a covert CD
					*/
					if (cd_index != 2){
							strcpy (palname, "OLDCC2T.VQP");
					}
				}
#endif	//(FRENCH | GERMAN)

#if (GERMAN)
				/*
				** Kludge to use a different palette interpolation table for RETRO.VQA
				** if the covert CD is inserted.
				*/
				if (!stricmp (palname, "RETRO.VQP")){
					int cd_index = Get_CD_Index(CCFileClass::Get_CD_Drive(), 1*60);
					/*
					** If cd_index == 2 then its a covert CD
					*/
					if (cd_index == 2){
						strcpy (palname, "RETROGER.VQP");
					}
				}

#endif	//GERMAN

				Load_Interpolated_Palettes(palname);
				//Set_Palette(BlackPalette);
				SysMemPage.Clear();
				InMovie = TRUE;
				VQA_Play(vqa, VQAMODE_RUN);
				VQA_Close(vqa);
				//Resume_Audio_Thread();
				InMovie = FALSE;
				Free_Interpolated_Palettes();
				Set_Primary_Buffer_Format();

				/*
				**	Any movie that ends prematurely must have the screen
				**	cleared to avoid any unexpected palette glitches.
				*/
				if (Brokeout) {
					clrscrn = true;
					VisiblePage.Clear();
					Brokeout = false;
				}
			}

			VQA_Free(vqa);
		}


		/*
		**	Presume that the screen is left in a garbage state as well as the palette
		**	being in an unknown condition. Recover from this by clearing the screen and
		**	forcing the palette to black.
		*/
		if (clrscrn) {
			VisiblePage.Clear();
			memset(BlackPalette, 0x01, 768);
			Set_Palette(BlackPalette);
			memset(BlackPalette, 0x00, 768);
			Set_Palette(BlackPalette);
		}
		Show_Mouse();
	}
#endif
}


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

	char *retval = NULL;
	char *buffer = NULL;
	void *ptr;


	/*
	**	If there is no shape file, then there can be no radar icon imagery.
	*/
	if (!shapefile) return(NULL);

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
	** we dont add these icons to the set.
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
		if ((ptr = (void *)(Build_Frame(shapefile, shapenum + framelp, SysMemPage.Get_Buffer()))) != NULL) {
			ptr = Get_Shape_Header_Data(ptr);
			/*
			** Loop through the icon width and the icon height building icons
			** into the buffer pointer.  When the getx or gety falls outside of
			** the width and height of the shape, just insert transparent pixels.
			*/
			for (int icony = 0; icony < icon_height; icony ++) {
				for (int iconx = 0; iconx < icon_width; iconx ++) {
					for (int y = 0; y < zoomfactor; y++) {
						for (int x = 0; x < zoomfactor; x++) {
							int getx = (iconx * 24) + (x * val) + (zoomfactor / 2);
							int gety = (icony * 24) + (y * val) + (zoomfactor / 2);
							if ((getx < pixel_width) && (gety < pixel_height)) {
								for (lp = 0; lp < 9; lp ++) {
									pixel	= *((char *)(Add_Long_To_Pointer(ptr, ((gety - _offy[lp]) * pixel_width) + getx-_offx[lp])));
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




void CC_Texture_Fill (void const *shapefile, int shapenum, int xpos, int ypos, int width, int height)
{
	unsigned char	*shape_pointer;
	//unsigned char	*shape_save;
	unsigned	long	shape_size;
	//int x,y;

	if (shapefile && shapenum != -1) {

		/*
		** Build frame returns a pointer now instead of the shapes length
		*/
		shape_size=Build_Frame(shapefile , shapenum , _ShapeBuffer);
		if (Get_Last_Frame_Length() > _ShapeBufferSize) {
			Mono_Printf("Attempt to use shape buffer for size %d buffer is only size %d", shape_size, _ShapeBufferSize);
			Get_Key();
		}

		if (shape_size) {
			shape_pointer = (unsigned char *)Get_Shape_Header_Data ((void*)shape_size);
			int source_width = Get_Build_Frame_Width (shapefile);
			int source_height = Get_Build_Frame_Height (shapefile);


			LogicPage->Texture_Fill_Rect (xpos, ypos, width, height, shape_pointer, source_width, source_height);
#if (0)
			if (LogicPage->Lock()){

				for (y = ypos ; y < ypos + MIN(source_height, height) ; y++ ){

					shape_save = shape_pointer;

					for (x = xpos ; x < xpos + MIN(source_width, width) ; x++ ){
						LogicPage->Put_Pixel (x, y, *shape_pointer++);
					}

					shape_pointer = shape_save + source_width;
				}

				LogicPage->Unlock();
			}
#endif
		}
	}
}




extern void DLL_Draw_Intercept(int shape_number, int x, int y, int width, int height, int flags, ObjectClass *object, const char *shape_file_name, char override_owner, int scale);
extern void DLL_Draw_Pip_Intercept(const ObjectClass* object, int pip);
extern void DLL_Draw_Line_Intercept(int x, int y, int x1, int y1, unsigned char color, int frame);

void CC_Draw_Shape(ObjectClass *object, void const * shapefile, int shapenum, int x, int y, WindowNumberType window, ShapeFlags_Type flags, void const * fadingdata, void const * ghostdata, int scale)
{
	if (window == WINDOW_VIRTUAL) {
		int width = Get_Build_Frame_Width(shapefile);
		int height = Get_Build_Frame_Height(shapefile);
		DLL_Draw_Intercept(shapenum, x, y, width, height, (int)flags, object, NULL, -1, scale);
		return;
	}

	CC_Draw_Shape(shapefile, shapenum, x, y, window, flags, fadingdata, ghostdata);
}			  


void CC_Draw_Shape(ObjectClass *object, const char *shape_file_name, void const * shapefile, int shapenum, int x, int y, WindowNumberType window, ShapeFlags_Type flags, void const * fadingdata, void const * ghostdata, char override_owner)
{
	if (window == WINDOW_VIRTUAL) {
		int width = Get_Build_Frame_Width(shapefile);
		int height = Get_Build_Frame_Height(shapefile);
		DLL_Draw_Intercept(shapenum, x, y, width, height, (int)flags, object, shape_file_name, override_owner, 0x100);
		return;
	}

	CC_Draw_Shape(shapefile, shapenum, x, y, window, flags, fadingdata, ghostdata);
}			  


void CC_Draw_Pip(ObjectClass *object, void const * shapefile, int shapenum, int x, int y, WindowNumberType window, ShapeFlags_Type flags, void const * fadingdata, void const * ghostdata)
{
	if (window == WINDOW_VIRTUAL) {
		DLL_Draw_Pip_Intercept(object, shapenum);
		return;
	}

	CC_Draw_Shape(shapefile, shapenum, x, y, window, flags, fadingdata, ghostdata);
}


void CC_Draw_Line(int x, int y, int x1, int y1, unsigned char color, int frame, WindowNumberType window)
{
	if (window == WINDOW_VIRTUAL) {
		DLL_Draw_Line_Intercept(x, y, x1, y1, color, frame);
		return;
	}

	LogicPage->Draw_Line(x, y, x1, y1, color);
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
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
//#pragma off(unreferenced)
void CC_Draw_Shape(void const * shapefile, int shapenum, int x, int y, WindowNumberType window, ShapeFlags_Type flags, void const * fadingdata, void const * ghostdata)
{
#if(TRUE)
	int predoffset;
	char				*shape_pointer;
	unsigned	long	shape_size;

	if (shapefile && shapenum != -1) {

		/*
		** Build frame returns a pointer now instead of the shapes length
		*/
		shape_size=Build_Frame(shapefile , shapenum , _ShapeBuffer);
		if (Get_Last_Frame_Length() > _ShapeBufferSize) {
			Mono_Printf("Attempt to use shape buffer for size %d buffer is only size %d", shape_size, _ShapeBufferSize);
			Get_Key();
		}

		if (shape_size) {
			GraphicViewPortClass draw_window(LogicPage->Get_Graphic_Buffer(),
														(WindowList[window][WINDOWX] << 3) + LogicPage->Get_XPos(),
														WindowList[window][WINDOWY] + LogicPage->Get_YPos(),
														WindowList[window][WINDOWWIDTH] << 3,
														WindowList[window][WINDOWHEIGHT]);

			shape_pointer = (char *)shape_size;

			/*
			**	Special shadow drawing code (used for aircraft and bullets).
			*/
			if ((flags & (SHAPE_FADING|SHAPE_PREDATOR)) == (SHAPE_FADING|SHAPE_PREDATOR)) {
				flags = flags & ~(SHAPE_FADING|SHAPE_PREDATOR);
				flags = flags | SHAPE_GHOST;
				ghostdata = Map.SpecialGhost;
			}

			predoffset = Frame;

			if (x > ( WindowList[window][WINDOWWIDTH] << 2)) {
				predoffset = -predoffset;
			}

			if (draw_window.Lock()){
				if ((flags & (SHAPE_GHOST|SHAPE_FADING)) == (SHAPE_GHOST|SHAPE_FADING)) {
					Buffer_Frame_To_Page(x, y, Get_Build_Frame_Width(shapefile), Get_Build_Frame_Height(shapefile),
						shape_pointer, draw_window, flags | SHAPE_TRANS, ghostdata, fadingdata, 1, predoffset);
				} else {
					if (flags & SHAPE_FADING) {
						Buffer_Frame_To_Page(x, y, Get_Build_Frame_Width(shapefile), Get_Build_Frame_Height(shapefile),
							shape_pointer, draw_window, flags | SHAPE_TRANS, fadingdata, 1, predoffset);
					} else {
						if (flags & SHAPE_PREDATOR) {
							Buffer_Frame_To_Page(x, y, Get_Build_Frame_Width(shapefile), Get_Build_Frame_Height(shapefile),
								shape_pointer, draw_window, flags | SHAPE_TRANS, predoffset);
						} else {
							Buffer_Frame_To_Page(x, y, Get_Build_Frame_Width(shapefile), Get_Build_Frame_Height(shapefile),
								shape_pointer, draw_window, flags | SHAPE_TRANS, ghostdata, predoffset);
						}
					}
				}
			}
			draw_window.Unlock();
//		} else {
//			Mono_Printf( "Overrun ShapeBuffer!!!!!!!!!\n" );
		}
	}
#endif
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
			return(&UnitTypeClass::As_Reference((UnitType)id));

		case RTTI_BUILDINGTYPE:
		case RTTI_BUILDING:
			return(&BuildingTypeClass::As_Reference((StructType)id));

		case RTTI_INFANTRYTYPE:
		case RTTI_INFANTRY:
			return(&InfantryTypeClass::As_Reference((InfantryType)id));

		case RTTI_AIRCRAFTTYPE:
		case RTTI_AIRCRAFT:
			return(&AircraftTypeClass::As_Reference((AircraftType)id));
	}
	return(NULL);
}


/***************************************************************************
 * Trap_Object -- gets a ptr to object of given type & coord               *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/02/1995 BRR : Created.                                             *
 *=========================================================================*/
void Trap_Object(void)
{
	int i;

	TrapObject.Ptr.All = NULL;

	switch (TrapObjType) {
		case RTTI_AIRCRAFT:
			for (i = 0; i < Aircraft.Count(); i++) {
				if (Aircraft.Ptr(i)->Coord == TrapCoord || Aircraft.Ptr(i)==TrapThis) {
					TrapObject.Ptr.Aircraft = Aircraft.Ptr(i);
					break;
				}
			}
			break;

		case RTTI_ANIM:
			for (i = 0; i < Anims.Count(); i++) {
				if (Anims.Ptr(i)->Coord == TrapCoord || Anims.Ptr(i)==TrapThis) {
					TrapObject.Ptr.Anim = Anims.Ptr(i);
					break;
				}
			}
			break;

		case RTTI_BUILDING:
			for (i = 0; i < Buildings.Count(); i++) {
				if (Buildings.Ptr(i)->Coord == TrapCoord || Buildings.Ptr(i)==TrapThis) {
					TrapObject.Ptr.Building = Buildings.Ptr(i);
					break;
				}
			}
			break;

		case RTTI_BULLET:
			for (i = 0; i < Bullets.Count(); i++) {
				if (Bullets.Ptr(i)->Coord == TrapCoord || Bullets.Ptr(i)==TrapThis) {
					TrapObject.Ptr.Bullet = Bullets.Ptr(i);
					break;
				}
			}
			break;

		case RTTI_INFANTRY:
			for (i = 0; i < Infantry.Count(); i++) {
				if (Infantry.Ptr(i)->Coord == TrapCoord || Infantry.Ptr(i)==TrapThis) {
					TrapObject.Ptr.Infantry = Infantry.Ptr(i);
					break;
				}
			}
			break;

		case RTTI_UNIT:
			for (i = 0; i < Units.Count(); i++) {
				if (Units.Ptr(i)->Coord == TrapCoord || Units.Ptr(i)==TrapThis) {
					TrapObject.Ptr.Unit = Units.Ptr(i);
					break;
				}
			}
			break;

		/*
		** Last-ditch find-the-object-right-now-darnit loop
		*/
		case RTTI_NONE:
			for (i = 0; i < Aircraft.Count(); i++) {
				if (Aircraft.Raw_Ptr(i)->Coord == TrapCoord || Aircraft.Raw_Ptr(i)==TrapThis) {
					TrapObject.Ptr.Aircraft = Aircraft.Raw_Ptr(i);
					TrapObjType = RTTI_AIRCRAFT;
					return;
				}
			}
			for (i = 0; i < Anims.Count(); i++) {
				if (Anims.Raw_Ptr(i)->Coord == TrapCoord || Anims.Raw_Ptr(i)==TrapThis) {
					TrapObject.Ptr.Anim = Anims.Raw_Ptr(i);
					TrapObjType = RTTI_ANIM;
					return;
				}
			}
			for (i = 0; i < Buildings.Count(); i++) {
				if (Buildings.Raw_Ptr(i)->Coord == TrapCoord || Buildings.Raw_Ptr(i)==TrapThis) {
					TrapObject.Ptr.Building = Buildings.Raw_Ptr(i);
					TrapObjType = RTTI_BUILDING;
					return;
				}
			}
			for (i = 0; i < Bullets.Count(); i++) {
				if (Bullets.Raw_Ptr(i)->Coord == TrapCoord || Bullets.Raw_Ptr(i)==TrapThis) {
					TrapObject.Ptr.Bullet = Bullets.Raw_Ptr(i);
					TrapObjType = RTTI_BULLET;
					return;
				}
			}
			for (i = 0; i < Infantry.Count(); i++) {
				if (Infantry.Raw_Ptr(i)->Coord == TrapCoord || Infantry.Raw_Ptr(i)==TrapThis) {
					TrapObject.Ptr.Infantry = Infantry.Raw_Ptr(i);
					TrapObjType = RTTI_INFANTRY;
					return;
				}
			}
			for (i = 0; i < Units.Count(); i++) {
				if (Units.Raw_Ptr(i)->Coord == TrapCoord || Units.Raw_Ptr(i)==TrapThis) {
					TrapObject.Ptr.Unit = Units.Raw_Ptr(i);
					TrapObjType = RTTI_UNIT;
					return;
				}
			}

		default:
			break;
	}
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
void VQA_PauseAudio(void) {};
void Check_VQ_Palette_Set(void);

long VQ_Call_Back(unsigned char *, long )
{
// PG_TO_FIX - 1/2/2019 3:59PM
#if (0)
	int key = 0;
	if (Keyboard::Check()){
		key = Keyboard::Get();
		Keyboard::Clear();
	}

	Check_VQ_Palette_Set();

	Interpolate_2X_Scale(&SysMemPage,&SeenBuff,NULL);
	//Call_Back();
	if ((BreakoutAllowed || Debug_Flag) && key == KN_ESC) {
		Keyboard::Clear();
		Brokeout = true;
		return(true);
	}

	if (!GameInFocus){
		VQA_PauseAudio();
		while (!GameInFocus){
			Keyboard::Check();
			Check_For_Focus_Loss();
		}
	}
#endif
	return(false);
}


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
				switch (CurrentObject[0]->What_Am_I()) {
					case RTTI_UNIT:
					case RTTI_INFANTRY:
					case RTTI_AIRCRAFT:
						if (((FootClass *)CurrentObject[0])->Group != team) {
							Unselect_All();
						}
						break;
				}
			}
			for (index = 0; index < Units.Count(); index++) {
				UnitClass * obj = Units.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House == PlayerPtr) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			for (index = 0; index < Infantry.Count(); index++) {
				InfantryClass * obj = Infantry.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House == PlayerPtr) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			for (index = 0; index < Aircraft.Count(); index++) {
				AircraftClass * obj = Aircraft.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House == PlayerPtr) {
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
				Map.Flag_To_Redraw(true);
			}
			break;

		/*
		**	Additive selection of team.
		*/
		case 1:
			for (index = 0; index < Units.Count(); index++) {
				UnitClass * obj = Units.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House == PlayerPtr) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			for (index = 0; index < Infantry.Count(); index++) {
				InfantryClass * obj = Infantry.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House == PlayerPtr) {
					if (!obj->Is_Selected_By_Player()) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}
			for (index = 0; index < Aircraft.Count(); index++) {
				AircraftClass * obj = Aircraft.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->Group == team && obj->House == PlayerPtr) {
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
		case 2:
			for (index = 0; index < Units.Count(); index++) {
				UnitClass * obj = Units.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->House == PlayerPtr) {
				 	if (obj->Group == team) obj->Group = -1;
					if (obj->Is_Selected_By_Player()) obj->Group = team;
				}
			}
			for (index = 0; index < Infantry.Count(); index++) {
				InfantryClass * obj = Infantry.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->House == PlayerPtr) {
				 	if (obj->Group == team) obj->Group = -1;
					if (obj->Is_Selected_By_Player()) obj->Group = team;
				}
			}
			for (index = 0; index < Aircraft.Count(); index++) {
				AircraftClass * obj = Aircraft.Ptr(index);
				if (obj && !obj->IsInLimbo && obj->House == PlayerPtr) {
				 	if (obj->Group == team) obj->Group = -1;
					if (obj->Is_Selected_By_Player()) obj->Group = team;
				}
			}
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
	if ((unsigned)view < sizeof(Views)/sizeof(Views[0])) {
		if (action == 0) {
			Map.Set_Tactical_Position(Cell_Coord(Views[view])&0xFF00FF00L);
			Map.Flag_To_Redraw(true);
		} else {
			Views[view] = Coord_Cell(Map.TacticalCoord);
		}
	}
}

#ifdef CHEAT_KEYS
void Heap_Dump_Check( char *string )
{
#if 0
	struct _heapinfo h_info;
	int heap_status;
#endif


	if ( !Debug_Trap_Check_Heap ) {	// check the heap?
		return;
	}

//	Debug_Heap_Dump = true;

	Smart_Printf( "%s\n", string );

	Dump_Heap_Pointers();

#if 0
	heap_status = _heapset( 0xee );

#if (1)
	if ( heap_status == _HEAPOK ||
		heap_status == _HEAPEMPTY ) {
		Debug_Heap_Dump = false;
		return;
	}

	h_info._pentry = NULL;

	for(;;) {
		heap_status = _heapwalk( &h_info );

		if ( heap_status != _HEAPOK )
			break;
	}

	if (heap_status != _HEAPEND &&
		heap_status != _HEAPEMPTY) {
#endif
		h_info._pentry = NULL;

		for(;;) {
			heap_status = _heapwalk( &h_info );

			if ( heap_status != _HEAPOK )
				break;

			Smart_Printf( " %s block at %Fp of size %4.4X\n",
				(h_info._useflag == _USEDENTRY ? "USED" : "FREE"),
				h_info._pentry, h_info._size );
		}

		Smart_Printf( " %d block at %Fp of size %4.4X\n",
			h_info._useflag, h_info._pentry, h_info._size );

		switch ( heap_status ) {
//			case _HEAPEND:
//				Smart_Printf( "OK - end of heap\n" );
//				break;

//			case _HEAPEMPTY:
//				Smart_Printf( "OK - heap is empty\n" );
//				break;

			case _HEAPBADBEGIN:
				Smart_Printf( "ERROR - heap is damaged\n" );
				break;

			case _HEAPBADPTR:
				Smart_Printf( "ERROR - bad pointer to heap\n" );
				break;

			case _HEAPBADNODE:
				Smart_Printf( "ERROR - bad node in heap\n" );
				break;
		}
#if (1)
	}
#endif
#endif

//	Debug_Heap_Dump = false;
}


void Dump_Heap_Pointers( void )
{
// ST - 1/2/2019 4:04PM
#if (0)	
	char *ptr, *lptr, *nptr, *cptr, *dptr, *wlptr, *nlptr, *aptr, *clptr;
	int numallocs, numfrees, sizefree;
	static char _freeorused[2][5] = { "FREE", "USED" };


	ptr = (char *)__nheapbeg;

	while ( ptr ) {

		if ( Debug_Heap_Dump ) {
			Smart_Printf( "%p pre header\n", (ptr - 8) );
			Hex_Dump_Data( (ptr - 8), 0x08);

			Smart_Printf( "%p header\n", ptr );
			Hex_Dump_Data( ptr, 0x30);
		}

		dptr = (char *)*(int *)(ptr + 0x0c);

		sizefree = *(int *)(ptr + 0x14);
		numallocs = *(int *)(ptr + 0x18);
		numfrees = *(int *)(ptr + 0x1c);

		cptr = (char *)*(int *)(ptr + 0x24);
		lptr = (char *)*(int *)(ptr + 0x28);

		if ( ((int)cptr & 0xff000000) ||
			((int)dptr & 0xff000000) ||
			((int)lptr & 0xff000000) ) {
			Error_In_Heap_Pointers( "local free heap ptrs too large" );
		}

		if ( Debug_Heap_Dump ) {
			if ( lptr != dptr ||
				lptr != cptr ||
				cptr != dptr ) {
				Smart_Printf( "The pointers are different!!\n" );
			}
		}

		nptr = (char *)*(int *)(ptr + 8);		// next block

		if ( ((int)nptr & 0xFF000000) ) {
			Error_In_Heap_Pointers( "next block ptr too large" );
		}

		if ( lptr != (ptr + 0x20) ) {

			if ( !(*((int *)(ptr + 0x20))) ) {		// allocated
				aptr = (ptr + 0x2c);

				while ( aptr < lptr ) {

					if ( (*(int *)(aptr)) == -1) {
//						Smart_Printf( "end alloc chain %p.\n", aptr );
//						Hex_Dump_Data( aptr, 0x10);
						break;
					}

					if ( Debug_Heap_Dump ) {
						Smart_Printf( "%p chain %s, size %X.\n", aptr,
							_freeorused[ ((*aptr) & 1) ],
							((*(int *)(aptr)) & 0xfffffffe) );
						Hex_Dump_Data( aptr, 0x10);
					}

					if ( ((*(int *)(aptr)) & 0xff000000) ) {
						Error_In_Heap_Pointers( "alloc block size way too large" );
					}

					aptr += ((*(int *)(aptr)) & 0xfffffffe);

					if ( ((int)aptr & 0xff000000) ) {
						Error_In_Heap_Pointers( "next alloc block ptr way too large" );
					}

					numallocs--;

					if ( aptr > lptr ) {
						Error_In_Heap_Pointers( "next alloc block ptr too large" );
					}
				}
			} else {
				if ( sizefree != -1 ) {
					sizefree -= ((*(int *)(ptr + 0x20)) & 0xfffffffe);
				}
				numfrees--;
			}

			wlptr = lptr;

			while ( wlptr != (ptr + 0x20) ) {

				if ( Debug_Heap_Dump ) {
					Smart_Printf( "%p link %s, size %X.\n", wlptr,
						_freeorused[ ((*wlptr) & 1) ],
						((*(int *)(wlptr)) & 0xfffffffe) );
					Hex_Dump_Data( wlptr, 0x10);
				}

				nlptr = (char *)*(int *)(wlptr + 8);

				if ( !(*((int *)(wlptr))) ) {		// allocated
					aptr = (wlptr + 0x0c);
				} else {
					if ( sizefree != -1 ) {
						sizefree -= ((*(int *)(wlptr)) & 0xfffffffe);
					}
					numfrees--;

					aptr = (wlptr + ((*(int *)(wlptr)) & 0xfffffffe));
				}

				if (nlptr == (ptr + 0x20) ) {
					clptr = nptr;
				} else {
					clptr = nlptr;
				}

				while ( aptr < clptr ) {

					if ( (*(int *)(aptr)) == -1) {
//						Smart_Printf( "end alloc chain %p.\n", aptr );
//						Hex_Dump_Data( aptr, 0x10);
						break;
					}

					if ( Debug_Heap_Dump ) {
						Smart_Printf( "%p chain %s, size %X.\n", aptr,
							_freeorused[ ((*aptr) & 1) ],
							((*(int *)(aptr)) & 0xfffffffe) );
						Hex_Dump_Data( aptr, 0x10);
					}

					if ( ((*(int *)(aptr)) & 0xff000000) ) {
						Error_In_Heap_Pointers( "alloc block size way too large" );
					}

					aptr += ((*(int *)(aptr)) & 0xfffffffe);

					if ( ((int)aptr & 0xff000000) ) {
						Error_In_Heap_Pointers( "next alloc block ptr way too large" );
					}

					numallocs--;

					if ( aptr > clptr ) {
						Error_In_Heap_Pointers( "next alloc block ptr too large" );
					}
				}

				wlptr = nlptr;
			}
		} else {
//			Smart_Printf( "only link %s, size %X.\n",
//				_freeorused[ ((*lptr) & 1) ],
//				((*(int *)(lptr)) & 0xfffffffe) );

			if ( !(*((int *)(lptr))) ) {		// allocated
				aptr = (ptr + 0x2c);

				while ( aptr < nptr ) {

					if ( (*(int *)(aptr)) == -1) {
//						Smart_Printf( "end alloc chain %p.\n", aptr );
//						Hex_Dump_Data( aptr, 0x10);
						break;
					}

					if ( Debug_Heap_Dump ) {
						Smart_Printf( "%p chain %s, size %X.\n", aptr,
							_freeorused[ ((*aptr) & 1) ],
							((*(int *)(aptr)) & 0xfffffffe) );
						Hex_Dump_Data( aptr, 0x10);
					}

					if ( ((*(int *)(aptr)) & 0xff000000) ) {
						Error_In_Heap_Pointers( "alloc block size way too large" );
					}

					aptr += ((*(int *)(aptr)) & 0xfffffffe);

					if ( ((int)aptr & 0xff000000) ) {
						Error_In_Heap_Pointers( "next alloc block ptr way too large" );
					}

					numallocs--;

					if ( aptr > nptr ) {
						Error_In_Heap_Pointers( "next alloc block ptr too large" );
					}
				}
			} else {
				if ( sizefree != -1 ) {
					sizefree -= ((*(int *)(ptr + 0x20)) & 0xfffffffe);
				}
				numfrees--;
			}
		}

		if ( sizefree != 0 && sizefree != -1 ) {
			Smart_Printf( "sizefree left over %X.\n", sizefree );
		}

		if ( numallocs != 0 ) {
			Smart_Printf( "numallocs unaccounted for %d.\n", numallocs );
		}

		if ( numfrees != 0 ) {
			Smart_Printf( "numfrees unaccounted for %d.\n", numfrees );
		}

		ptr = nptr;
	}
#endif
}


void Error_In_Heap_Pointers( char *string )
{
	Smart_Printf( "Error in Heap for %s\n", string );
}
#endif


#ifndef ROR_NOT_READY
#define ROR_NOT_READY 21
#endif

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
 *           2 = covert                                                                        *
 *          -1 = non C&C                                                                       *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/21/96 5:27PM ST : Created                                                              *
 *=============================================================================================*/
int Get_CD_Index (int cd_drive, int timeout)
{
	char		volume_name[128];
	unsigned	filename_length;
	unsigned	misc_dword;
	//unsigned	error_code;
	int		count = 0;

	CountDownTimerClass timer;

	static char * _volid[] = {
		"GDI95",
		"NOD95",
		"COVERT"
	};
	static int num_volumes = 3;

	char	buffer[128];

	timer.Set(timeout);


	/*
	** Get the volume label. If we get a 'not ready' error then retry for the timeout
	** period.
	*/
	do{
		sprintf(buffer, "%c:\\", 'A' + cd_drive);

		if (GetVolumeInformation ((char const *)buffer,
											&volume_name[0] ,
											(unsigned long)128 ,
											(unsigned long *)NULL ,
											(unsigned long *)&filename_length,
											(unsigned long *)&misc_dword ,
											(char *)NULL ,
											(unsigned long)0)) {

			/*
			** Try opening 'movies.mix' to verify that the CD is really there and is what
			** it says it is.
			*/
			sprintf(buffer, "%c:\\movies.mix", 'A' + cd_drive);

			HANDLE handle = CreateFile(buffer, GENERIC_READ, FILE_SHARE_READ,
													NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (handle != INVALID_HANDLE_VALUE) {
				CloseHandle(handle);

				/*
				** Match the volume label to the list of known C&C volume labels.
				*/
				for (int i=0 ; i<num_volumes ; i++){
					if (!stricmp(_volid[i], volume_name)) return (i);
				}
			}else{
				if (count++){
					return (-1);
				}
			}
		}else{
			/*
			** Failed to get the volume label on a known CD drive.
			** If this is a CD changer it may require time to swap the disks so dont return
			** immediately if the error is ROR_NOT_READY
			*/
			if (GetLastError() != ROR_NOT_READY || !timer.Time()) return (-1);
		}
	}while(true);
}




/***********************************************************************************************
 * Force_CD_Available -- Ensures that specified CD is available.                               *
 *                                                                                             *
 *    Call this routine when you need to ensure that the specified CD is actually in the       *
 *    CD-ROM drive.                                                                            *
 *                                                                                             *
 * INPUT:   cd    -- The CD that must be available. This will either be "0" for the GDI CD, or *
 *                   "1" for the Nod CD. If either CD will qualify, then pass in "-1".         *
 *                                                                                             *
 * OUTPUT:  Is the CD inserted and available? If false is returned, then this indicates that   *
 *          the player pressed <CANCEL>.                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1995 JLB : Created.                                                                 *
 *   05/22/1996  ST : Handles multiple CD drives / CD changers                                 *
 *=============================================================================================*/
#pragma warning (disable : 4101)
bool Force_CD_Available(int cd)
{
	
#if (1)	//ST - 1/2/2019 5:44PM
	
	static int _last = -1;

	if (_last != cd) {

		_last = cd;

		Theme.Stop();

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

		MoviesMix = new MixFileClass("MOVIES.MIX");
		GeneralMix = new MixFileClass("GENERAL.MIX");
		ScoreMix = new MixFileClass("SCORES.MIX");
#if (0)
		switch ( cd ) {
			case -1:
			default:
				MoviesMix = new MixFileClass("MOVIES.MIX");
				GeneralMix = new MixFileClass("GENERAL.MIX");
				ScoreMix = new MixFileClass("SCORES.MIX");
				break;

			case 0:
				MoviesMix = new MixFileClass("GDIMOVIES.MIX");
				GeneralMix = new MixFileClass("GENERAL.MIX");
				ScoreMix = new MixFileClass("GDISCORES.MIX");
				break;

			case 1:
				MoviesMix = new MixFileClass("NODMOVIES.MIX");
				GeneralMix = new MixFileClass("GENERAL.MIX");
				ScoreMix = new MixFileClass("NODSCORES.MIX");
				break;

			case 2:
				MoviesMix = new MixFileClass("COVERTMOVIES.MIX");
				GeneralMix = new MixFileClass("GENERAL.MIX");
				ScoreMix = new MixFileClass("COVERTSCORES.MIX");
				break;
		}
#endif
		ThemeClass::Scan();

	}

	return true;



#else
	
#ifndef DEMO
	static int _last = -1;
	int open_failed;
	int file;
#endif
	static char _palette[768];
	static char _hold[256];
	static void *font;
	static char * _volid[] = {
		"GDI",
		"NOD",
		"COVERT"
	};

	int	drive;

	char		volume_name[100];
	unsigned	filename_length;
	unsigned	misc_dword;
	int new_cd_drive = 0;
	int cd_index;
	char buffer[128];
	int cd_drive;
	int current_drive;
	int drive_search_timeout;
	bool old_in_main_loop;

	ThemeType theme_playing = THEME_NONE;

	/*
	** If the required CD is set to -2 then it means that the file is present
	** on the local hard drive and we shouldn't have to worry about it.
	*/
	if (cd == -2) return(true);


	/*
	** Find out if the CD in the current drive is the one we are looking for
	*/
	current_drive = CCFileClass::Get_CD_Drive();
	cd_index = Get_CD_Index(current_drive, 1*60);
	if (cd_index >= 0){
		if (cd == cd_index || cd == -1){
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


	if (!new_cd_drive){
		/*
		** Check the last CD drive we used if its different from the current one
		*/
		int last_drive = CCFileClass::Get_Last_CD_Drive();
		/*
		** Make sure the last drive is valid and it isnt the current drive
		*/
		if (last_drive && last_drive != CCFileClass::Get_CD_Drive()){
			/*
			** Find out if there is a C&C cd in the last drive and if so is it the one we are looking for
			** Give it a nice big timeout so the CD changer has time to swap the discs
			*/
			cd_index = Get_CD_Index(last_drive, 10*60);
			if (cd_index >= 0){
				if (cd == cd_index || cd == -1){
					/*
					** The required CD is in the CD drive we used last time
					*/
					new_cd_drive = last_drive;
				}
			}
		}
	}

	/*
	** Lordy. No sign of that blimming CD anywhere. Search all the CD drives
	** then if we still cant find it prompt the user to insert it.
	*/
	if (!new_cd_drive){

		/*
		** Small timeout for the first pass through the drives
		*/
		drive_search_timeout = 2*60;

		for (;;) {
			/*
			** Search all present CD drives for the required disc.
			*/
			for (int i=0 ; i<CDList.Get_Number_Of_Drives() ; i++){
				cd_drive = CDList.Get_Next_CD_Drive();
				cd_index = Get_CD_Index(cd_drive, drive_search_timeout);
				if (cd_index>=0){
					/*
					** We found a C&C cd - lets see if it was the one we were looking for
					*/
					if (cd == cd_index || cd == -1){
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
			if (cd == -1) {
				sprintf(buffer, Text_String(TXT_CD_DIALOG_1), cd+1, _volid[cd]);
			} else {
				if (cd == 2) {
					sprintf(buffer, Text_String(TXT_CD_DIALOG_3));
				} else {
					sprintf(buffer, Text_String(TXT_CD_DIALOG_2), cd+1, _volid[cd]);
				}
			}
			GraphicViewPortClass * oldpage = Set_Logic_Page(SeenBuff);
			theme_playing = Theme.What_Is_Playing();
			Theme.Stop();
			int hidden = Get_Mouse_State();
			font = (void *)FontPtr;
			Mem_Copy(CurrentPalette, _palette, 768);
			Mem_Copy(Get_Font_Palette_Ptr(), _hold, 256);


			/*
			**	Only set the palette if necessary.
			*/
//			if (CurrentPalette[3] == 0) {
				Set_Palette(GamePalette);
//			}

			/*
			** Pretend we are in the game, even if we arent
			*/
			old_in_main_loop = InMainLoop;
			InMainLoop = true;

			Keyboard::Clear();

			while (Get_Mouse_State()) Show_Mouse();

			if (CCMessageBox().Process(buffer, TXT_OK, TXT_CANCEL, TXT_NONE, TRUE) == 1) {
				Set_Logic_Page(oldpage);
				Hide_Mouse();
				InMainLoop = old_in_main_loop;
				return(false);
			}
			while (hidden--) Hide_Mouse();
			Set_Palette(_palette);
			Set_Font(font);
			Mem_Copy(_hold, Get_Font_Palette_Ptr(), 256);
			Set_Logic_Page(oldpage);
			InMainLoop = old_in_main_loop;
		}
	}


#ifndef DEMO

	CCFileClass::Set_CD_Drive(new_cd_drive);
	CCFileClass::Refresh_Search_Drives();

	/*
	**	If it broke out of the query for CD-ROM loop, then this means that the
	**	CD-ROM has been inserted.
	*/
	if (cd > -1 && _last != cd) {
		_last = cd;

		Theme.Stop();

		if (MoviesMix) delete MoviesMix;
		if (GeneralMix) delete GeneralMix;
		if (ScoreMix) delete ScoreMix;

		MoviesMix = new MixFileClass("MOVIES.MIX");
		GeneralMix = new MixFileClass("GENERAL.MIX");
		ScoreMix = new MixFileClass("SCORES.MIX");
		ThemeClass::Scan();
	}
#endif

	if (theme_playing != THEME_NONE) {
		Theme.Queue_Song(theme_playing);
	}

	return(true);
#endif
}


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

	return 0x7fffffff;

#if (0)
	struct diskfree_t diskdata;
	unsigned drive;

	_dos_getdrive(&drive);
	_dos_getdiskfree(drive, &diskdata);

	return(diskdata.avail_clusters * diskdata.sectors_per_cluster * diskdata.bytes_per_sector);
#endif
}


/***********************************************************************************************
 * Validate_Error -- prints an error message when an object fails validation                   *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		name		name of object type that failed																  *
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
void Validate_Error(char *name)
{
	GlyphX_Debug_Print("Validate_Error");
	GlyphX_Debug_Print(name);
#ifdef CHEAT_KEYS
	Prog_End(NULL, true);
	printf("%s object error!\n",name);
	if (!RunningAsDLL) {
		exit(0);
	}
#else
	name = name;
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
 *                                                                                              *
 * HISTORY:                                                                                    *
 *   08/15/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static void Do_Record_Playback(void)
{
	int count;
	TARGET tgt;
	int i;
	COORDINATE coord;
	ObjectClass *obj;
	unsigned long sum;
	unsigned long sum2;
	unsigned long ltgt;

	/*------------------------------------------------------------------------
	Record a game
	------------------------------------------------------------------------*/
	if (RecordGame) {

		/*.....................................................................
		For 'SuperRecord', we'll open & close the file with every entry.
		.....................................................................*/
		if (SuperRecord) {
			RecordFile.Open(READ|WRITE);
			RecordFile.Seek(0,SEEK_END);
		}

		/*.....................................................................
		Save the map's location
		.....................................................................*/
		RecordFile.Write(&Map.DesiredTacticalCoord, sizeof (Map.DesiredTacticalCoord));

		/*.....................................................................
		Save the current object list count
		.....................................................................*/
		count = CurrentObject.Count();
		RecordFile.Write(&count, sizeof(count));

		/*.....................................................................
		Save a CRC of the selected-object list.
		.....................................................................*/
		sum = 0;
		for (i = 0; i < count; i++) {
			ltgt = (unsigned long)(CurrentObject[i]->As_Target());
			sum += ltgt;
		}
		RecordFile.Write (&sum, sizeof(sum));

		/*.....................................................................
		Save all selected objects.
		.....................................................................*/
		for (i = 0; i < count; i++) {
			tgt = CurrentObject[i]->As_Target();
			RecordFile.Write (&tgt, sizeof(tgt));
		}

		/*.....................................................................
		If 'SuperRecord', close the file now.
		.....................................................................*/
		if (SuperRecord) {
			RecordFile.Close();
		}
	}

	/*------------------------------------------------------------------------
	Play back a game ("attract" mode)
	------------------------------------------------------------------------*/
	if (PlaybackGame) {

		/*.....................................................................
		Read & set the map's location.
		.....................................................................*/
		if (RecordFile.Read(&coord, sizeof(coord))==sizeof(coord)) {
			if (coord != Map.DesiredTacticalCoord) {
				Map.Set_Tactical_Position(coord);
			}
		}

		if (RecordFile.Read(&count, sizeof(count))==sizeof(count)) {
			/*..................................................................
			Compute a CRC of the current object-selection list.
			..................................................................*/
			sum = 0;
			for (i = 0; i < CurrentObject.Count(); i++) {
				ltgt = (unsigned long)(CurrentObject[i]->As_Target());
				sum += ltgt;
			}

			/*..................................................................
			Load the CRC of the objects on disk; if it doesn't match, select
			all objects as they're loaded.
			..................................................................*/
			RecordFile.Read (&sum2, sizeof(sum2));
			if (sum2 != sum)
				Unselect_All();

			AllowVoice = true;

			for (i = 0; i < count; i++) {
				if (RecordFile.Read (&tgt, sizeof(tgt))==sizeof(tgt)) {
					obj = As_Object(tgt);
					if (obj && (sum2 != sum)) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}

			AllowVoice = true;

		}

		/*.....................................................................
		The map isn't drawn in playback mode, so draw it here.
		.....................................................................*/
		Map.Render();
	}
}
/***************************************************************************
 * HIRES_RETRIEVE -- retrieves a resolution dependant file						*
 *                                                                         *
 * INPUT:		char * file name of the file to retrieve							*
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1996     : Created.                                             *
 *=========================================================================*/
void const * Hires_Retrieve(char *name)
{
	char filename[30];

	if (SeenBuff.Get_Width() != 320) {
		sprintf(filename,"H%s", name);
	} else {
	   strcpy(filename, name);
	}
	return(MixFileClass::Retrieve(filename));
}
int Get_Resolution_Factor(void)
{
	return ((SeenBuff.Get_Width() == 320) ? 0 : 1);
}





/**************************************************************************************************
* Blit_Hid_Page_To_Seen_Buff -- Intercept for when the game refreshes the visible page
*
* In:   Command line
*
* Out:
*
*
*
* History: 1/3/2019 11:33AM - ST
**************************************************************************************************/
void Blit_Hid_Page_To_Seen_Buff(void)
{
	HidPage.Blit(SeenBuff);
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
			hptr->ScreenShakeTime = hptr->ScreenShakeTime + shakes + shakes;
		}
	}
}