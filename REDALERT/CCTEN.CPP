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
 *                    File Name : CCTEN.CPP                                *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 01/09/96                                 *
 *                                                                         *
 *                  Last Update : November 27, 1996 [BRR]                  *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Init_TEN -- Performs TEN-specific initialization                      *
 *   Shutdown_TEN -- Shuts down TEN connections                            *
 *   Connect_TEN -- Waits for connections to other players                 *
 *   Destroy_TEN_Connection -- Destroys the given connection               *
 *   Debug_Mono -- Custom mono prints                                      *
 *   Send_TEN_Win_Packet -- Sends a win packet to server                   *
 *   Send_TEN_Alliance -- Sends an ally/enemy packet to server             *
 *   Send_TEN_Out_Of_Sync -- Announces this game out of sync               *
 *   Send_TEN_Packet_Too_Late -- Announces packet-received-too-late        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "function.h"

#if(TEN)
#ifdef WIN32
#define WINDOWS
#endif
#include "ten.h"
#endif

void Connect_TEN(void);
void Debug_Mono(void);

/***************************************************************************
 * Init_TEN -- Performs TEN-specific initialization                        *
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
int Init_TEN(void)
{
#if(TEN)
	//------------------------------------------------------------------------
	// Allocate a packet buffer for TEN's use
	//------------------------------------------------------------------------
	Session.TenPacket = new char[Session.TenSize];

	//------------------------------------------------------------------------
	// Read the multiplayer settings from the CONQUER.INI file, and the game
	// options from the options file.
	//------------------------------------------------------------------------
	Session.Read_MultiPlayer_Settings();

	if (!Read_TEN_Game_Options()) {
		WWMessageBox().Process("Unable to load game settings!");
		//Prog_End();
		Emergency_Exit(0);
	}

	//------------------------------------------------------------------------
	// Flush all incoming packets
	//------------------------------------------------------------------------
	Ten->Flush_All();

	//------------------------------------------------------------------------
	// Form connections to all other players
	//------------------------------------------------------------------------
	Connect_TEN();

	//------------------------------------------------------------------------
	// Set multiplayer values for the local system, and timing values.
	//------------------------------------------------------------------------
	Session.CommProtocol = COMM_PROTOCOL_MULTI_E_COMP;

	return (1);
#else
	return (1);
#endif

}	// end of Init_TEN


/***************************************************************************
 * Shutdown_TEN -- Shuts down TEN connections                              *
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
void Shutdown_TEN(void)
{
#if(TEN)
	CDTimerClass<SystemTimerClass> timer;

	//------------------------------------------------------------------------
	// Wait a full second before exiting, to ensure all packets get sent.
	//------------------------------------------------------------------------
	timer = 60;
	while (timer) ;

	//------------------------------------------------------------------------
	// Free memory
	//------------------------------------------------------------------------
	if (Session.TenPacket) {
		delete [] Session.TenPacket;
		Session.TenPacket = NULL;
	}

	if (Ten) {
		delete Ten;
		Ten = NULL;
	}

	return;

#endif
}	// end of Shutdown_TEN


/***************************************************************************
 * Connect_TEN -- Waits for connections to other players                 	*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		MPlayerCount must have been initialized at this point.					*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/10/1996 BRR : Created.                                             *
 *=========================================================================*/
void Connect_TEN(void)
{
#if(TEN)
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
	num_players = Session.NumPlayers - 1;
	num_found = 0;

	//
	// Send out a packet announcing my presence
	//
	send_packet.Dummy = NET_PING;
	strcpy(send_packet.Name, Session.Handle);
	send_packet.House = Session.House;
	send_packet.Color = Session.ColorIdx;
	Ten->Send_Global_Message(&send_packet, sizeof(send_packet), 0, -1);

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

		Ten->Service();

		//
		// Check for an incoming packet; if a PING comes in, see if we already
		// have this player in our Players list.  If not, add him.
		//
		if (Ten->Get_Global_Message (&receive_packet, &size, &address) &&
			receive_packet.Dummy == NET_PING) {
			found = 0;
			for (i = 1; i < Session.Players.Count(); i++) {
				if (Session.Players[i]->TenAddress == address) {
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
				who->TenAddress = address;
				who->Player.House = receive_packet.House;
				who->Player.Color = (PlayerColorType)receive_packet.Color;
				Session.Players.Add (who);

				num_found++;

				Ten->Send_Global_Message(&send_packet, sizeof(send_packet), 1,
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
			Ten->Send_Global_Message(&send_packet, sizeof(send_packet), 0, -1);
			send_timer = 240;
		}
	}

#else
	return;
#endif
}


/***************************************************************************
 * Destroy_TEN_Connection -- Destroys the given connection                 *
 *                                                                         *
 * INPUT:                                                                  *
 *		id			connection ID to destroy (must be a HousesType)					*
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
void Destroy_TEN_Connection(int id, int error)
{
#if(TEN)
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
		sprintf(txt,Text_String(TXT_CONNECTION_LOST),Ten->Connection_Name(id));
	} else if (error==0) {
		sprintf(txt,Text_String(TXT_LEFT_GAME),Ten->Connection_Name(id));
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
	Delete the TEN connection
	------------------------------------------------------------------------*/
	Ten->Delete_Connection(id);

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
}	// end of Destroy_TEN_Connection


/***************************************************************************
 * Debug_Mono -- Custom mono prints                                        *
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
 *   11/27/1996 BRR : Created.                                             *
 *=========================================================================*/
void Debug_Mono(void)
{
#if(TEN)
	int i;
	int id;

	Mono_Printf("STATE: # Connections:%d\n",Ten->Num_Connections());
	for (i=0;i<Ten->Num_Connections();i++) {
		id = Ten->Connection_ID(i);
		Mono_Printf("Connection %d: Name:%s, ID:%d, Address:%d\n",
			i,
			Ten->Connection_Name(id),
			Ten->Connection_ID(i),
			Ten->Connection_Address(id));
	}

#endif
}


/***************************************************************************
 * Send_TEN_Win_Packet -- Sends a win packet to server                     *
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
 *   11/27/1996 BRR : Created.                                             *
 *=========================================================================*/
void Send_TEN_Win_Packet(void)
{
#if(TEN)
	char winbuf[80];
	char idbuf[20];
	int first = 1;
	HouseClass *hptr;
	int i;

	//
	// Build a special text buffer to send to the TEN server.  Format:
	// "winner 'id id'", where 'id' is the Ten Player ID of each player
	// on the winning team.  (For TEN, the color index is the player ID.)
	//
	sprintf(winbuf,"winner '");
	for (i = 0; i < Session.Players.Count(); i++) {
		hptr = HouseClass::As_Pointer(Session.Players[i]->Player.ID);
		if (!hptr->IsDefeated) {
			if (!first) {
				strcat(winbuf," ");
			} else {
				first = 0;
			}
			sprintf(idbuf,"%d", Session.Players[i]->Player.Color);
			strcat (winbuf, idbuf);
		}
	}
	strcat (winbuf,"' ");
	tenArSetPlayerState(winbuf);
#endif	// TEN

}	// end of Send_TEN_Win_Packet


/***************************************************************************
 * Send_TEN_Alliance -- Sends an ally/enemy packet to server               *
 *                                                                         *
 * INPUT:                                                                  *
 *		whom		name of player we're allying / enemying with						*
 *		ally		1 = we're allying; 0 = we're breaking the alliance				*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/27/1996 BRR : Created.                                             *
 *=========================================================================*/
void Send_TEN_Alliance(char *whom, int ally)
{
#if(TEN)
	char buf[80];

	if (ally) {
		sprintf(buf,"ally '%s' ",whom);
	} else {
		sprintf(buf,"enemy '%s' ",whom);
	}

	tenArSetPlayerState(buf);

#else
	whom = whom;
	ally = ally;
#endif	// TEN

}	// end of Send_TEN_Alliance


/***************************************************************************
 * Send_TEN_Out_Of_Sync -- Announces this game out of sync                 *
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
 *   11/27/1996 BRR : Created.                                             *
 *=========================================================================*/
void Send_TEN_Out_Of_Sync(void)
{
#if(TEN)
	tenArSetPlayerState("sync '1' ");
#endif	// TEN

}	// end of Send_TEN_Out_Of_Sync


/***************************************************************************
 * Send_TEN_Packet_Too_Late -- Announces packet-received-too-late          *
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
 *   11/27/1996 BRR : Created.                                             *
 *=========================================================================*/
void Send_TEN_Packet_Too_Late(void)
{
#if(TEN)
	tenArSetPlayerState("toolate '1' ");
#endif	// TEN

}	// end of Send_TEN_Packet_Too_Late


/***************************** end of ccten.cpp *****************************/
