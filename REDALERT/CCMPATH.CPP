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

/***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : CCMPATH.CPP                              *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 01/09/96                                 *
 *                                                                         *
 *                  Last Update : January 11, 1996 [BRR]                   *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Init_MPATH -- Performs MPATH-specific initialization                  *
 *   Shutdown_MPATH -- Shuts down MPATH connections                        *
 *   Connect_MPATH -- Waits for connections to other players               *
 *   Destroy_MPATH_Connection -- Destroys the given connection             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "function.h"


/***************************************************************************
 * Init_MPATH -- Performs MPATH-specific initialization                    *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/09/1996 BRR : Created.                                             *
 *=========================================================================*/
int Init_MPATH(void)
{
#if(MPATH)
	//------------------------------------------------------------------------
	// Allocate a packet buffer for MPATH's use
	//------------------------------------------------------------------------
	Session.MPathPacket = new char[Session.MPathSize];

	//------------------------------------------------------------------------
	// Read the multiplayer settings from the CONQUER.INI file, and the game
	// options from the options file.
	//------------------------------------------------------------------------
	Session.Read_MultiPlayer_Settings();

	if (!Read_MPATH_Game_Options()) {
		WWMessageBox().Process("Unable to load game settings!");
		//Prog_End();
		Emergency_Exit(0);
	}

	//------------------------------------------------------------------------
	// Flush all incoming packets
	//------------------------------------------------------------------------
	MPath->Flush_All();

	//------------------------------------------------------------------------
	// Form connections to all other players
	//------------------------------------------------------------------------
	Connect_MPATH();

	//------------------------------------------------------------------------
	// Set multiplayer values for the local system, and timing values.
	//------------------------------------------------------------------------
	Session.CommProtocol = COMM_PROTOCOL_MULTI_E_COMP;

	return (1);
#else
	return (1);
#endif

}	// end of Init_MPATH


/***************************************************************************
 * Shutdown_MPATH -- Shuts down MPATH connections                          *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/09/1996 BRR : Created.                                             *
 *=========================================================================*/
void Shutdown_MPATH(void)
{
#if(MPATH)
	CDTimerClass<SystemTimerClass> timer;

	//------------------------------------------------------------------------
	// Wait a full second before exiting, to ensure all packets get sent.
	//------------------------------------------------------------------------
	timer = 60;
	while (timer) ;

	//------------------------------------------------------------------------
	// Free memory
	//------------------------------------------------------------------------
	if (Session.MPathPacket) {
		delete [] Session.MPathPacket;
		Session.MPathPacket = NULL;
	}

	if (MPath) {
		delete MPath;
		MPath = NULL;
	}

	return;

#endif
}	// end of Shutdown_MPATH


/***************************************************************************
 * Connect_MPATH -- Waits for connections to other players                 *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/10/1996 BRR : Created.                                             *
 *=========================================================================*/
void Connect_MPATH(void)
{
#if(MPATH)
	typedef struct ConnectPacketTag {
		NetCommandType Dummy;				// packet type; set to PING
		char Name[MPLAYER_NAME_MAX];		// player's name
		HousesType House;						// player's ActLike
		unsigned char Color;					// player's Color
	} ConnectPacketType;
	int num_players;
	int num_found;
	ConnectPacketType send_packet;
	ConnectPacketType receive_packet;
	int address;
	int found;
	int size;
	int i;
	CDTimerClass<SystemTimerClass> send_timer;
	NodeNameType *who;

	enum {
		D_TXT6_H = 7,
		D_MARGIN = 5,
	};
	static int x,y,w,h;
	char const *buf1;
	char const *buf2;

	int display = 0;
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	//
	// Clear the Players list
	//
	while (Session.Players.Count() > 0) {
		delete Session.Players[0];
		Session.Players.Delete(Session.Players[0]);
	}

	//
	// Add myself to the list first thing
	//
	who = new NodeNameType;
	strcpy(who->Name, Session.Handle);
	who->Player.House = Session.House;
	who->Player.Color = Session.ColorIdx;
	Session.Players.Add (who);

	//
	// Find out how many players to wait for
	//
	num_players = MPath->Find_Num_Connections();
	num_found = 0;
	Session.NumPlayers = num_players + 1;

	//
	// Send out a packet announcing my presence
	//
	send_packet.Dummy = NET_PING;
	strcpy(send_packet.Name, Session.Handle);
	send_packet.House = Session.House;
	send_packet.Color = Session.ColorIdx;
	MPath->Send_Global_Message(&send_packet, sizeof(send_packet), 0, 0);

	//
	// Start our packet-sending timer
	//
	send_timer = 240;

	//
	// Wait for all players to enter the game
	//
	display = 1;
	while (num_found < num_players) {

		#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = 1;
		}
		#endif

		if (display) {
			Fancy_Text_Print("", 0, 0, 0, 0, TPF_TEXT);
			buf1 = Text_String(TXT_WAITING_FOR_CONNECTIONS);
			buf2 = Text_String(TXT_PRESS_ESC);
			w = MAX(String_Pixel_Width(buf1),String_Pixel_Width(buf2));
			w += (D_MARGIN * 2);
			h = (D_TXT6_H * 2) + (D_MARGIN * 7);
			x = 160 - (w / 2);
			y = 100 - (h / 2);
			Hide_Mouse();
			//Set_Logic_Page(SeenBuff);
			Dialog_Box(x * RESFACTOR, y * RESFACTOR, w * RESFACTOR, h * RESFACTOR);

			Fancy_Text_Print(buf1,
				160 * RESFACTOR,
				(y + (D_MARGIN * 2)) * RESFACTOR,
				scheme, TBLACK, TPF_CENTER | TPF_TEXT);
			Fancy_Text_Print(buf2,
				160 * RESFACTOR,
				(y + (D_MARGIN * 2) + D_TXT6_H + D_MARGIN) * RESFACTOR,
				scheme, TBLACK, TPF_CENTER | TPF_TEXT);
			Show_Mouse();
			display = 0;
		}

		MPath->Service();

		//
		// Check for an incoming packet; if a PING comes in, see if we already
		// have this player in our Players list.  If not, add him.
		//
		if (MPath->Get_Global_Message (&receive_packet, &size, &address) &&
			receive_packet.Dummy == NET_PING) {
			found = 0;
			for (i = 1; i < Session.Players.Count(); i++) {
				if (Session.Players[i]->MPathAddress == address) {
					found = 1;
					break;
				}
			}

			//
			// Create a new connection and a new node in the list.
			//
			if (!found) {

				who = new NodeNameType;
				strcpy(who->Name, receive_packet.Name);
				who->MPathAddress = address;
				who->Player.House = receive_packet.House;
				who->Player.Color = (PlayerColorType)receive_packet.Color;
				Session.Players.Add (who);

				num_found++;

				MPath->Send_Global_Message(&send_packet, sizeof(send_packet), 1,
					address);
			}
		}

		//
		// If the user hits ESC, bail out.
		//
		if (Keyboard->Check()) {
			if (Keyboard->Get() == KN_ESC) {
				//Prog_End();
				Emergency_Exit(0);
			}
		}

		//
		// When our timer expires, re-send the packet.
		//
		if (!send_timer) {
			send_packet.Dummy = NET_PING;
			MPath->Send_Global_Message(&send_packet, sizeof(send_packet), 0, 0);
			send_timer = 240;
		}
	}

#else
	return;
#endif
}


/***************************************************************************
 * Destroy_MPATH_Connection -- Destroys the given connection               *
 *                                                                         *
 * INPUT:                                                                  *
 *		id			connection ID to destroy												*
 *		error		0 = user signed off; 1 = connection error; otherwise, 		*
 *					no error is shown.		  												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/11/1996 BRR : Created.                                             *
 *=========================================================================*/
void Destroy_MPATH_Connection(int id, int error)
{
#if(MPATH)
	int i;
	HouseClass *housep;
	char txt[80];

	//------------------------------------------------------------------------
	//	Do nothing if the house isn't human.
	//------------------------------------------------------------------------
	housep = HouseClass::As_Pointer((HousesType)id);
	if (!housep || !housep->IsHuman)
		return;

	/*------------------------------------------------------------------------
	Create a message to display to the user
	------------------------------------------------------------------------*/
	txt[0] = '\0';
	if (error==1) {
		sprintf(txt,Text_String(TXT_CONNECTION_LOST),MPath->Connection_Name(id));
	} else if (error==0) {
		sprintf(txt,Text_String(TXT_LEFT_GAME),MPath->Connection_Name(id));
	}

	if (strlen(txt)) {
		Session.Messages.Add_Message (NULL,0, txt, housep->RemapColor,
			TPF_TEXT, Rule.MessageDelay * TICKS_PER_MINUTE);
		Map.Flag_To_Redraw(false);
	}

	//------------------------------------------------------------------------
	// Remove this player from the Players vector
	//------------------------------------------------------------------------
	for (i = 0; i < Session.Players.Count(); i++) {
		if (!stricmp(Session.Players[i]->Name,housep->IniName)) {
			delete Session.Players[i];
			Session.Players.Delete(Session.Players[i]);
			break;
		}
	}

	/*------------------------------------------------------------------------
	Delete the MPATH connection
	------------------------------------------------------------------------*/
	MPath->Delete_Connection(id);

	//------------------------------------------------------------------------
	//	Turn the player's house over to the computer's AI
	//------------------------------------------------------------------------
	housep->IsHuman = false;
	housep->IQ = Rule.MaxIQ;
	strcpy (housep->IniName,Text_String(TXT_COMPUTER));

	Session.NumPlayers--;

	/*------------------------------------------------------------------------
	If we're the last player left, tell the user.
	------------------------------------------------------------------------*/
	if (Session.NumPlayers == 1) {
		sprintf(txt,"%s",Text_String(TXT_JUST_YOU_AND_ME));
		Session.Messages.Add_Message (NULL, 0, txt, housep->RemapColor,
			TPF_TEXT, Rule.MessageDelay * TICKS_PER_MINUTE);
		Map.Flag_To_Redraw(false);
	}

#else
	id = id;
	error = error;

#endif
}	// end of Destroy_MPATH_Connection


/***************************** end of ccmpath.cpp **************************/
