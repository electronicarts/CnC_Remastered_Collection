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

/* $Header:   F:\projects\c&c\vcs\code\netdlg.cpv   2.17   16 Oct 1995 16:52:26   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                         						  *
 *                 Project Name : Command & Conquer                        						  *
 *                                                                         						  *
 *                    File Name : NETDLG.CPP                               						  *
 *                                                                         						  *
 *                   Programmer : Bill Randolph                            						  *
 *                                                                         						  *
 *                   Start Date : January 23, 1995                         						  *
 *                                                                         						  *
 *                  Last Update : July 8, 1995 [BRR]                       						  *
 *                                                                         						  *
 *---------------------------------------------------------------------------------------------*
 *                                                                         						  *
 * These routines establish & maintain peer-to-peer connections between this system				  *
 * and all others in the game.  Each system finds out the IPX address of the others,			  *
 * and forms a direct connection (IPXConnectionClass) to that system.  Systems are				  *
 * found out via broadcast queries.  Every system broadcasts its queries, and every				  *
 * system replies to queries it receives.  At the point when the game owner signals				  *
 * 'OK', every system must know about all the other systems in the game.							  *
 *                                                                         						  *
 * How Bridges are handled:																						  *
 * Currently, bridges are handled by specifying the destination IPX address of the				  *
 * "server" (game owner's system) on the command-line.  This address is used to					  *
 * derive a broadcast address to that destination network, and this system's queries			  *
 * are broadcast over its network & the server's network; replies to the queries come			  *
 * with each system's IPX address attached, so once we have the address, we can form			  *
 * a connection with any system on the bridged net.														  *
 *                                                                         						  *
 * The flaw in this plan is that we can only cross one bridge.  If there are 3 nets				  *
 * bridged (A, B, & C), and the server is on net B, and we're on net A, our broadcasts			  *
 * will reach nets A & B, but not C.  The way to circumvent this (if it becomes a problem)	  *
 * would be to have the server tell us what other systems are in its game, not each				  *
 * individual player's system.  Thus, each system would find out about all the other systems	  *
 * by interacting with the game's owner system (this would be more involved than what			  *
 * I'm doing here).																									  *
 *                                                                         						  *
 * Here's a list of all the different packets sent over the Global Channel:						  *
 *																															  *
 *	NET_QUERY_GAME																										  *
 *							(no other data)																			  *
 *	NET_ANSWER_GAME																									  *
 *							Name:					game owner's name													  *
 *							GameInfo:			game's version & open state									  *
 *	NET_QUERY_PLAYER																									  *
 *							Name:					name of game we want players to respond for				  *
 *	NET_ANSWER_PLAYER																									  *
 *							Name:					player's name														  *
 *							PlayerInfo:			info about player													  *
 *	NET_QUERY_JOIN																										  *
 *							Name:					name of player wanting to join								  *
 *							PlayerInfo:			player's requested house & color								  *
 *	NET_CONFIRM_JOIN																									  *
 *							PlayerInfo:			approves player's house & color								  *
 *	NET_REJECT_JOIN																									  *
 *							(no other data)																			  *
 *	NET_GAME_OPTIONS																									  *
 *							ScenarioInfo:		info about scenario												  *
 *	NET_SIGN_OFF																										  *
 *							Name:					name of player signing off										  *
 *	NET_PING																												  *
 *							(no other data)																			  *
 *	NET_GO																												  *
 *							Delay:            value of one-way response time, in frames               *
 * 																														  *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                              						  *
 *   Clear_Game_List -- Clears the game-name listbox & 'Games' Vector        						  *
 *   Clear_Player_List -- Clears the player-name listbox & Vector     		   					  *
 *   Destroy_Connection -- destroys the given connection													  *
 *   Get_Join_Responses -- sends queries for the Join Dialog											  *
 *   Get_NewGame_Responses -- processes packets for New Game dialog                            *
 *   Init_Network -- initializes network stuff                             						  *
 *   Net_Join_Dialog -- lets user join an existing game, or start a new one						  *
 *   Net_New_Dialog -- lets user start a new game															  *
 *   Process_Global_Packet -- responds to remote queries                   						  *
 *   Remote_Connect -- handles connecting this user to others              						  *
 *   Request_To_Join -- Sends a JOIN request packet to game owner           	 					  *
 *   Send_Join_Queries -- sends queries for the Join Dialog												  *
 *   Shutdown_Network -- shuts down network stuff                          						  *
 *   Compute_Name_CRC -- computes CRC from char string                                         *
 *   Net_Reconnect_Dialog -- Draws/updates the network reconnect dialog                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include <time.h>
#include "tcpip.h"
#include "ccdde.h"
#define SHOW_MONO		0

// ST = 12/17/2018 5:44PM
#ifndef TickCount
extern TimerClass					TickCount;
#endif

#ifndef DEMO

/*---------------------------------------------------------------------------
The possible states of the join-game dialog
---------------------------------------------------------------------------*/
typedef enum {
	JOIN_REJECTED = -1,		// we've been rejected
	JOIN_NOTHING,				// we're not trying to join a game
	JOIN_WAIT_CONFIRM,		// we're asking to join, & waiting for confirmation
	JOIN_CONFIRMED,			// we've been confirmed
	JOIN_GAME_START,			// the game we've joined is starting
} JoinStateType;

/*---------------------------------------------------------------------------
The possible return codes from Get_Join_Responses()
---------------------------------------------------------------------------*/
typedef enum {
	EV_NONE,							// nothing happened
	EV_STATE_CHANGE,				// Join dialog is in a new state
	EV_NEW_GAME,					// a new game was detected
	EV_NEW_PLAYER,					// a new player was detected
	EV_PLAYER_SIGNOFF,			// a player has signed off
	EV_GAME_SIGNOFF,				// a gamed owner has signed off
	EV_GAME_OPTIONS,				// a game options packet was received
	EV_MESSAGE,						// a message was received
} JoinEventType;


/*
******************************** Prototypes *********************************
*/
static int Net_Join_Dialog(void);
static void Clear_Game_List (ListClass *gamelist);
static void Clear_Player_List (ListClass *playerlist);
static int Request_To_Join (char *playername, int join_index, ListClass *playerlist,
	HousesType house, int color);
static void Send_Join_Queries(int curgame, int gamenow, int playernow);
static JoinEventType Get_Join_Responses(JoinStateType *joinstate, ListClass *gamelist,
	ColorListClass *playerlist, int join_index);
static int Net_New_Dialog(void);
static JoinEventType Get_NewGame_Responses(ColorListClass *playerlist);
static int Net_Fake_New_Dialog(void);
static int Net_Fake_Join_Dialog(void);


/***********************************************************************************************
 * Init_Network -- initializes network stuff                               						  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		none.																												  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		true = Initialization OK, false = error																  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.                                              						  *
 *=============================================================================================*/
bool Init_Network (void)
{
	NetNumType net;
	NetNodeType node;

	/*------------------------------------------------------------------------
	This call allocates all necessary queue buffers, allocates Real-mode
	memory, and commands IPX to start listening on the Global Channel.
	------------------------------------------------------------------------*/
	if (!Ipx.Init())
		return(false);

	/*------------------------------------------------------------------------
	Allocate our "meta-packet" buffer
	------------------------------------------------------------------------*/
	if (!MetaPacket) {
		MetaPacket = new char [sizeof (EventClass) * MAX_EVENTS];
	}

	/*------------------------------------------------------------------------
	Set up the IPX manager to cross a bridge
	------------------------------------------------------------------------*/
	if (!(GameToPlay == GAME_INTERNET)){
		if (IsBridge) {
			BridgeNet.Get_Address(net,node);
			Ipx.Set_Bridge(net);
		}
	}

	return(true);

}	/* end of Init_Network */


/***********************************************************************************************
 * Shutdown_Network -- shuts down network stuff                            						  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		none.																												  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		none.																												  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.                                              						  *
 *=============================================================================================*/
void Shutdown_Network (void)
{

//
// Note: The thought behind this section of code was that if the program
// terminates early, without an EventClass::EXIT event, it still needs to
// tell the other systems that it's gone, so it would send a SIGN_OFF packet.
// BUT, this causes a sync bug if the systems are running slow and this system
// is running ahead of the others; it will send the NET_SIGN_OFF >>before<<
// the other system execute their EventClass::EXIT event, and the other systems
// will kill the connection at some random Frame # & turn my stuff over to
// the computer possibly at different times.
// BRR, 10/29/96
//
#if (0)
	/*------------------------------------------------------------------------
	Broadcast a sign-off packet, by sending the packet over the Global Channel,
	telling the IPX Manager that no ACK is required, and specifying a NULL
	destination address.
	------------------------------------------------------------------------*/
	memset (&GPacket, 0, sizeof(GlobalPacketType));

	GPacket.Command = NET_SIGN_OFF;
	strcpy (GPacket.Name, MPlayerName);

	Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType), 0, NULL);
	Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType), 0, NULL);

	if (IsBridge && !Winsock.Get_Connected()) {
		Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0, &BridgeNet);
		Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0, &BridgeNet);
	}

	/*------------------------------------------------------------------------
	Wait for the packets to finish going out (or the Global Channel times out)
	------------------------------------------------------------------------*/
	for (;;) {
		if (Ipx.Global_Num_Send()==0) {
			break;
		}
		Ipx.Service();
	}

#endif	//(0)

	/*------------------------------------------------------------------------
	Delete our "meta-packet"
	------------------------------------------------------------------------*/
	delete [] MetaPacket;
	MetaPacket = 0;

	/*------------------------------------------------------------------------
	If I was in a game, I'm not now, so clear the game name
	------------------------------------------------------------------------*/
	MPlayerGameName[0] = 0;
}


/***********************************************************************************************
 * Process_Global_Packet -- responds to remote queries                     						  *
 *                                                                         						  *
 * The only commands from other systems this routine responds to are NET_QUERY_GAME				  *
 * and NET_QUERY_PLAYER.  The other commands are too context-specific to be able					  *
 * to handle here, such as joining the game or signing off; but this routine handles			  *
 * the majority of the program's needs.																		  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		packet		ptr to packet to process																	  *
 *		address		source address of sender																	  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		true = packet was processed, false = wasn't															  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		MPlayerName & MPlayerGameName must have been filled in before this function				  *
 *		can be called.																									  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/15/1995 BR : Created.                                              						  *
 *=============================================================================================*/
bool Process_Global_Packet(GlobalPacketType *packet, IPXAddressClass *address)
{
	GlobalPacketType mypacket;

	/*
	---------------- Another system asking what game this is -----------------
	*/
	if (packet->Command==NET_QUERY_GAME && NetStealth==0) {
		/*.....................................................................
		If the game is closed, let every player respond, and let the sender of
		the query sort it all out.  This way, if the game's host exits the game,
		the game still shows up on other players' dialogs.
		If the game is open, only the game owner may respond.
		.....................................................................*/
		if (strlen(MPlayerName) > 0 && strlen(MPlayerGameName) > 0 &&
			((!NetOpen) || (NetOpen && !strcmp(MPlayerName,MPlayerGameName)))) {
			memset (packet, 0, sizeof(GlobalPacketType));

			mypacket.Command = NET_ANSWER_GAME;
			strcpy(mypacket.Name, MPlayerGameName);
#ifdef PATCH
			if (IsV107) {
				mypacket.GameInfo.Version = 1;
			} else {
				mypacket.GameInfo.Version = 2;
			}
#else
			mypacket.GameInfo.Version = Version_Number();
#endif
			mypacket.GameInfo.IsOpen = NetOpen;

			Ipx.Send_Global_Message (&mypacket, sizeof(GlobalPacketType), 1,
				address);
		}
		return(true);
	} else {

		/*
		----------------- Another system asking what player I am -----------------
		*/
		if (packet->Command==NET_QUERY_PLAYER &&
			!strcmp (packet->Name, MPlayerGameName) &&
				(strlen(MPlayerGameName) > 0) && NetStealth==0) {
			memset (packet, 0, sizeof(GlobalPacketType));

			mypacket.Command = NET_ANSWER_PLAYER;
			strcpy(mypacket.Name, MPlayerName);
			mypacket.PlayerInfo.House = MPlayerHouse;
			mypacket.PlayerInfo.Color = MPlayerColorIdx;
			mypacket.PlayerInfo.NameCRC = Compute_Name_CRC(MPlayerGameName);

			Ipx.Send_Global_Message (&mypacket, sizeof(GlobalPacketType), 1, address);
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * Destroy_Connection -- destroys the given connection													  *
 *                                                                         						  *
 * Call this routine when a connection goes bad, or another player signs off.						  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		id			connection ID to destroy																		  *
 *		error		0 = user signed off; 1 = connection error; otherwise, no error is shown.		  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		none.																												  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   04/22/1995 BR : Created.                                              						  *
 *=============================================================================================*/
void Destroy_Connection(int id, int error)
{
	int i,j;
	HousesType house;
	HouseClass *housep;
	char txt[80];

	/*------------------------------------------------------------------------
	Create a message to display to the user
	------------------------------------------------------------------------*/
	txt[0] = '\0';
	if (error==1) {
		sprintf(txt,Text_String(TXT_CONNECTION_LOST),Ipx.Connection_Name(id));
	} else if (error==0) {
		sprintf(txt,Text_String(TXT_LEFT_GAME),Ipx.Connection_Name(id));
	}

	if (strlen(txt)) {
		Messages.Add_Message (txt,
			MPlayerTColors[MPlayerID_To_ColorIndex((unsigned char)id)],
			TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 600, 0, 0);
		Map.Flag_To_Redraw(false);
	}

	/*------------------------------------------------------------------------
	Delete the IPX connection, shift the MPlayerID's & MPlayerHouses' back one.
	------------------------------------------------------------------------*/
	Ipx.Delete_Connection(id);

	for (i = 0; i < MPlayerCount; i++) {
		if (MPlayerID[i] == (unsigned char)id) {
			/*..................................................................
			Turn the player's house over to the computer's AI
			..................................................................*/
			house = MPlayerHouses[i];
			housep = HouseClass::As_Pointer (house);
			housep->IsHuman = false;
			housep->IsStarted = true;

			/*..................................................................
			Move arrays back by one
			..................................................................*/
			for (j = i; j < MPlayerCount - 1; j++) {
				MPlayerID[j] = MPlayerID[j + 1];
				MPlayerHouses[j] = MPlayerHouses[j + 1];
				strcpy (MPlayerNames[j], MPlayerNames[j+1]);
				TheirProcessTime[j] = TheirProcessTime[j+1];
			}
		}
	}

	MPlayerCount--;

	/*------------------------------------------------------------------------
	If we're the last player left, tell the user.
	------------------------------------------------------------------------*/
	if (MPlayerCount == 1) {
		sprintf(txt,"%s",Text_String(TXT_JUST_YOU_AND_ME));
		Messages.Add_Message (txt,
			MPlayerTColors[MPlayerID_To_ColorIndex((unsigned char)id)],
			TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 600, 0, 0);
		Map.Flag_To_Redraw(false);
	}

}	/* end of Destroy_Connection */


/***********************************************************************************************
 * Remote_Connect -- handles connecting this user to others                						  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		none.																												  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		true = connections established; false = not															  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.                                              						  *
 *=============================================================================================*/
bool Remote_Connect(void)
{
	int rc;
	int stealth;				// original state of NetStealth flag

	/*------------------------------------------------------------------------
	Init network timing parameters; these values should work for both a "real"
	network, and a simulated modem network (ie Kali)
	------------------------------------------------------------------------*/
	Ipx.Set_Timing (	30, 		// retry 2 times per second
							-1, 		// ignore max retries
							600);		// give up after 10 seconds

	/*------------------------------------------------------------------------
	Save the original value of the NetStealth flag, so we can turn stealth
	off for now (during this portion of the dialogs, we must show ourselves)
	------------------------------------------------------------------------*/
	stealth = NetStealth;
	NetStealth = 0;

	/*------------------------------------------------------------------------
	Init my game name to 0-length, since I haven't joined any game yet.
	------------------------------------------------------------------------*/
	MPlayerGameName[0] = 0;

	/*------------------------------------------------------------------------
	The game is now "open" for joining.  Close it as soon as we exit this
	routine.
	------------------------------------------------------------------------*/
	NetOpen = 1;

	/*------------------------------------------------------------------------
	Read the default values from the INI file
	------------------------------------------------------------------------*/
	Read_MultiPlayer_Settings ();

	/*------------------------------------------------------------------------
	Keep looping until something useful happens.
	------------------------------------------------------------------------*/
	while (1) {
		/*---------------------------------------------------------------------
		Pop up the network Join/New dialog
		---------------------------------------------------------------------*/
		rc = Net_Join_Dialog();

		/*---------------------------------------------------------------------
		-1 = user selected Cancel
		---------------------------------------------------------------------*/
		if (rc==-1) {
			NetStealth = stealth;
			NetOpen = 0;
			return(false);
		} else {

			/*---------------------------------------------------------------------
			0 = user has joined an existing game; save values & return
			---------------------------------------------------------------------*/
			if (rc==0) {
				Write_MultiPlayer_Settings ();
				NetStealth = stealth;
				NetOpen = 0;

				return(true);
			} else {

				/*---------------------------------------------------------------------
				1 = user requests New Network Game
				---------------------------------------------------------------------*/
				if (rc==1) {
					/*..................................................................
					Pop up the New Network Game dialog; if user selects OK, return
					'true'; otherwise, return to the Join Dialog.
					..................................................................*/
					if (Net_New_Dialog()) {
						Write_MultiPlayer_Settings ();
						NetOpen = 0;
						NetStealth = stealth;
						NetOpen = 0;

						return(true);
					} else {
						continue;
					}
				}
			}
		}
	}
}



/***********************************************************************************************
 * Remote_Connect -- handles connecting this host to the server in an internet game 			    *
 *                                                                         						    *
 * INPUT:                                                                  						    *
 *		none.																												 *
 *                                                                         						    *
 * OUTPUT:                                                                 						    *
 *		true = connections established; false = not															 *
 *                                                                         						    *
 * WARNINGS:                                                               						    *
 *		none.																												 *
 *                                                                         						    *
 * HISTORY:                                                                						    *
 *   02/14/1995 BR : Created.                                              						    *
 *=============================================================================================*/
bool Server_Remote_Connect(void)
{
	int stealth;				// original state of NetStealth flag

	/*------------------------------------------------------------------------
	Init network timing parameters; these values should work for both a "real"
	network, and a simulated modem network (ie Kali)
	------------------------------------------------------------------------*/
	Ipx.Set_Timing (	30, 		// retry 2 times per second
							-1, 		// ignore max retries
							600);		// give up after 10 seconds

	/*------------------------------------------------------------------------
	Save the original value of the NetStealth flag, so we can turn stealth
	off for now (during this portion of the dialogs, we must show ourselves)
	------------------------------------------------------------------------*/
	stealth = NetStealth;
	NetStealth = 0;

	/*------------------------------------------------------------------------
	The game is now "open" for joining.  Close it as soon as we exit this
	routine.
	------------------------------------------------------------------------*/
	NetOpen = 1;

	/*------------------------------------------------------------------------
	Read the default values from the INI file
	------------------------------------------------------------------------*/
	Read_MultiPlayer_Settings ();

	if (!Net_Fake_New_Dialog()){
		Write_MultiPlayer_Settings ();
		return (false);
	}

	NetOpen = 0;
	NetStealth = stealth;
	Write_MultiPlayer_Settings ();
	return (true);
}


/***********************************************************************************************
 * Client_Remote_Connect -- handles connecting this client to the server in an internet game   *
 *                                                                         						    *
 * INPUT:                                                                  						    *
 *		none.																												 *
 *                                                                         						    *
 * OUTPUT:                                                                 						    *
 *		true = connections established; false = not															 *
 *                                                                         						    *
 * WARNINGS:                                                               						    *
 *		none.																												 *
 *                                                                         						    *
 * HISTORY:                                                                						    *
 *   02/14/1995 ST : Created.                                              						    *
 *=============================================================================================*/
bool Client_Remote_Connect(void)
{
	int rc;
	int stealth;				// original state of NetStealth flag

	/*------------------------------------------------------------------------
	Init network timing parameters; these values should work for both a "real"
	network, and a simulated modem network (ie Kali)
	------------------------------------------------------------------------*/
	Ipx.Set_Timing (	30, 		// retry 2 times per second
							-1, 		// ignore max retries
							600);		// give up after 10 seconds

	/*------------------------------------------------------------------------
	Save the original value of the NetStealth flag, so we can turn stealth
	off for now (during this portion of the dialogs, we must show ourselves)
	------------------------------------------------------------------------*/
	stealth = NetStealth;
	NetStealth = 0;

	/*------------------------------------------------------------------------
	The game is now "open" for joining.  Close it as soon as we exit this
	routine.
	------------------------------------------------------------------------*/
	NetOpen = 1;

	/*------------------------------------------------------------------------
	Read the default values from the INI file
	------------------------------------------------------------------------*/
	Read_MultiPlayer_Settings ();

	/*---------------------------------------------------------------------
	Pop up the network Join/New dialog
	---------------------------------------------------------------------*/
	rc = Net_Fake_Join_Dialog();
	Write_MultiPlayer_Settings ();

	NetStealth = stealth;
	NetOpen = 0;

	if (rc == -1) {
		return(false);
	} else {
		return(true);
	}
}



/***********************************************************************************************
 * Net_Join_Dialog -- lets user join an existing game or start a new one							  *
 *                                                                         						  *
 * This dialog displays an edit field for the player's name, and a list of all non-stealth-	  *
 * mode games.  Clicking once on a game name displays a list of who's in that game.  Clicking  *
 * "New" takes the user to the Net_New dialog, where he waits for other users to join his		  *
 * game.  All other input is done through this dialog.													  *
 *                                                                         						  *
 * The dialog has several "states":																				  *
 *                                                                         						  *
 * 1) Initially, it waits for the user to fill in his/her name and then to select Join or New; *
 *    if New is selected, this dialog is exited.															  *
 *                                                                         						  *
 *	2) If Join is selected, the Join & New buttons are removed, but the Cancel button remains.  *
 *	   The join request is transmitted to the game's owner, and the message "Waiting for		  *
 *		Confirmation" is displayed, until a confirmation or denial is received from the game's	  *
 *		owner.  The user may click Cancel at this point to cancel the join request.				  *
 *		(Once Join is selected, the name editing field is disabled, and becomes a display-only	  *
 *		field.  If cancel is selected, it reappears as an edit field.) The user can still click  *
 *		around & see who's in which games.																		  *
 *                                                                         						  *
 *	3) If the join request is denied, the dialog re-initializes to its pre-join state; the		  *
 *		Join & New buttons reappear, & the Name field is available again.								  *
 *                                                                         						  *
 * 4) If join confirmation is obtained, the message just changes to "Confirmed.  Waiting for	  *
 *		Entry Signal." or some such nonsense.  The user can still click around & see who's		  *
 *		in which games.																								  *
 *                                                                         						  *
 * Any game running in Stealth mode won't show up on this dialog.										  *
 *                                                                         						  *
 *    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                     					  *
 *    ³                 Network Games                     ³                     					  *
 *    ³                                                   ³                     					  *
 *    ³              Your Name: ____________              ³                     					  *
 *    ³                  House: [GDI] [NOD]               ³                     					  *
 *    ³          Desired Color: [ ][ ][ ][ ]              ³                     					  *
 *    ³                                                   ³                     					  *
 *    ³            Games                 Players          ³                     					  *
 *    ³ ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿ ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿ ³                     					  *
 *    ³ ³(Bill's Game         )³³ ³ Peter Parker GDI ³³ ³                     					  *
 *    ³ ³ Peter Parker's Game  ÃÄ´ ³ Mary Jane    GDI ÃÄ´ ³                     					  *
 *    ³ ³(Magnum PI's Game    )³ ³ ³ JJ Jameson   NOD ³ ³ ³                     					  *
 *    ³ ³                      ÃÄ´ ³                  ÃÄ´ ³                     					  *
 *    ³ ³                      ³³ ³                  ³³ ³                     					  *
 *    ³ ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ ³                     					  *
 *    ³           Scenario: Big Long Description          ³                     					  *
 *    ³                Starting Credits: xxxx             ³                     					  *
 *    ³          Count: ---          Level: ---           ³                     					  *
 *    ³          Bases: ON          Crates: ON            ³                          					  *
 *    ³       Tiberium: ON      AI Players: ON            ³                           					  *
 *    ³                                                   ³                     					  *
 *    ³            [Join]  [Cancel]    [New]              ³                     					  *
 *    ³  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿  ³                     					  *
 *    ³  ³                                             ³  ³                     					  *
 *    ³  ³                                             ³  ³                     					  *
 *    ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ  ³                     					  *
 *    ³                  [Send Message]                   ³                     					  *
 *    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                     					  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		none.																												  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		-1 = cancel, 0 = OK, 1 = New net game requested														  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.                                              						  *
 *=============================================================================================*/
static int Net_Join_Dialog(void)
{
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
/* ###Change collision detected! C:\PROJECTS\CODE\NETDLG.CPP... */
	int d_dialog_w = 287 *factor;											// dialog width
	int d_dialog_h = 198*factor;											// dialog height
	int d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*factor - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 6*factor+1;												// ht of 6-pt text
	int d_margin1 = 5*factor;												// large margin
	int d_margin2 = 2*factor;												// small margin

	int d_name_w = 70*factor;
	int d_name_h = 9*factor;
	int d_name_x = d_dialog_cx - 10*factor;
	int d_name_y = d_dialog_y + d_margin1 + d_txt6_h + d_txt6_h;

	int d_gdi_w = 30*factor;
	int d_gdi_h = 9*factor;
	int d_gdi_x = d_dialog_cx - 10*factor;
	int d_gdi_y = d_name_y + d_name_h + d_margin2;

	int d_nod_w = 30*factor;
	int d_nod_h = 9*factor;
	int d_nod_x = d_gdi_x + d_gdi_w;
	int d_nod_y = d_name_y + d_name_h + d_margin2;

	int d_color_w = 10*factor;
	int d_color_h = 9*factor;
	int d_color_y = d_nod_y + d_nod_h + d_margin2;

	int d_gamelist_w = 160*factor;
	int d_gamelist_h = 27*factor;
	int d_gamelist_x = d_dialog_x + d_margin1;
	int d_gamelist_y = d_color_y + d_color_h + d_margin1 + d_txt6_h;

	int d_playerlist_w = 106*factor;
	int d_playerlist_h = 27*factor;
	int d_playerlist_x = d_dialog_x + d_dialog_w - d_margin1 - d_playerlist_w;
	int d_playerlist_y = d_color_y + d_color_h + d_margin1 + d_txt6_h;

	int d_msg1_y = d_gamelist_y + d_gamelist_h + d_margin1;
	int d_msg2_y = d_msg1_y + d_txt6_h;
	int d_msg3_y = d_msg2_y + d_txt6_h;
	int d_msg4_y = d_msg3_y + d_txt6_h;
	int d_msg5_y = d_msg4_y + d_txt6_h;

	int d_join_w = 40*factor;
	int d_join_h = 9*factor;
	int d_join_x = d_dialog_x + (d_dialog_w / 6) - (d_join_w / 2);
	int d_join_y = d_msg5_y + d_txt6_h + d_margin1;

#if (GERMAN | FRENCH)
	int d_cancel_w = 50*factor;
#else
	int d_cancel_w = 40*factor;
#endif
	int d_cancel_h = 9*factor;
	int d_cancel_x = d_dialog_cx - d_cancel_w / 2;
	int d_cancel_y = d_msg5_y + d_txt6_h + d_margin1;

	int d_new_w = 40*factor;
	int d_new_h = 9*factor;
	int d_new_x = d_dialog_x + ((d_dialog_w * 5) / 6) - (d_new_w / 2);
	int d_new_y = d_msg5_y + d_txt6_h + d_margin1;

	int d_message_w = d_dialog_w - (d_margin1 * 2);
	int d_message_h = 34*factor;
	int d_message_x = d_dialog_x + d_margin1;
	int d_message_y = d_cancel_y + d_cancel_h + d_margin1;

	int d_send_w = 80*factor;
	int d_send_h = 9*factor;
	int d_send_x = d_dialog_cx - (d_send_w / 2);
	int d_send_y = d_message_y + d_message_h + d_margin2;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_NAME = 100,
		BUTTON_GDI,
		BUTTON_NOD,
		BUTTON_GAMELIST,
		BUTTON_PLAYERLIST,
		BUTTON_JOIN,
		BUTTON_CANCEL,
		BUTTON_NEW,
		BUTTON_SEND,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_MESSAGE,
		REDRAW_COLORS,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display = REDRAW_ALL;		// redraw level
	bool process = true;						// process while true
	KeyNumType input;
	int cbox_x[] = {	d_gdi_x,
							d_gdi_x + d_color_w,
							d_gdi_x + (d_color_w * 2),
							d_gdi_x + (d_color_w * 3),
							d_gdi_x + (d_color_w * 4),
							d_gdi_x + (d_color_w * 5)};

	JoinStateType joinstate = JOIN_NOTHING;	// current "state" of this dialog
	char namebuf[MPLAYER_NAME_MAX] = {0};		// buffer for player's name
	int tabs[] = {77*factor};						// tabs for player list box
	int game_index = -1;						// index of currently-selected game
	int join_index = -1;						// index of game we're joining
	int rc = 0;									// -1 = user cancelled, 1 = New
	JoinEventType event;						// event from incoming packet
	int i,j;										// loop counter
	char txt[80];
	char const *p;
	int parms_received;						// 1 = game options received
	int found;

	unsigned char tmp_id[MAX_PLAYERS];	// temp storage for sorting player ID's
	int min_index;								// for sorting player ID's
	unsigned char min_id;					// for sorting player ID's
	unsigned char id;							// connection ID
	char * item;
	unsigned long starttime;

	NodeNameType *who;

	int					message_length;
	int					sent_so_far;
	unsigned short		magic_number;
	unsigned short		crc;

	void const *up_button;
	void const *down_button;

	if (InMainLoop){
		up_button = Hires_Retrieve("BTN-UP.SHP");
		down_button = Hires_Retrieve("BTN-DN.SHP");
	}else{
		up_button = Hires_Retrieve("BTN-UP2.SHP");
		down_button = Hires_Retrieve("BTN-DN2.SHP");
	}


	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	EditClass name_edt (BUTTON_NAME,
		namebuf, MPLAYER_NAME_MAX, TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_name_x, d_name_y, d_name_w, d_name_h, EditClass::ALPHANUMERIC);

	TextButtonClass gdibtn(BUTTON_GDI, TXT_G_D_I,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_gdi_x, d_gdi_y, d_gdi_w, d_gdi_h);

	TextButtonClass nodbtn(BUTTON_NOD, TXT_N_O_D,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_nod_x, d_nod_y, d_nod_w, d_nod_h);

	ListClass gamelist(BUTTON_GAMELIST,
		d_gamelist_x, d_gamelist_y, d_gamelist_w, d_gamelist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	ColorListClass playerlist(BUTTON_PLAYERLIST,
		d_playerlist_x, d_playerlist_y, d_playerlist_w, d_playerlist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	TextButtonClass joinbtn(BUTTON_JOIN, TXT_JOIN,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
#ifdef FRENCH
		d_join_x, d_join_y);
#else
		d_join_x, d_join_y, d_join_w, d_join_h);
#endif

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_cancel_x, d_cancel_y);
//#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
//#endif

	TextButtonClass newbtn(BUTTON_NEW, TXT_NEW,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_new_x, d_new_y, d_new_w, d_new_h);

	TextButtonClass sendbtn(BUTTON_SEND, TXT_SEND_MESSAGE,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_send_x, d_send_y);
//#else
		d_send_x, d_send_y, d_send_w, d_send_h);
//#endif

	playerlist.Set_Tabs(tabs);

	/*
	----------------------------- Various Inits ------------------------------
	*/
	MPlayerColorIdx = MPlayerPrefColor;			// init my preferred color
	strcpy (namebuf, MPlayerName);				// set my name
	name_edt.Set_Text(namebuf,MPLAYER_NAME_MAX);
	name_edt.Set_Color (MPlayerTColors[MPlayerColorIdx]);

	playerlist.Set_Selected_Style(ColorListClass::SELECT_NONE);

	if (MPlayerHouse==HOUSE_GOOD) {
		gdibtn.Turn_On();
	} else {
		nodbtn.Turn_On();
	}

	Fancy_Text_Print("", 0, 0, CC_GREEN, TBLACK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

	Messages.Init (d_message_x + 2, d_message_y + 2, 4,
		MAX_MESSAGE_LENGTH, d_txt6_h);

	/*
	--------------------------- Send network query ---------------------------
	*/
	Send_Join_Queries (game_index, 1, 0);

	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Set_Palette(Palette);

	/*
	---------------------------- Init Mono Output ----------------------------
	*/
	#if(SHOW_MONO)
	Ipx.Configure_Debug(-1, sizeof (GlobalHeaderType),
		sizeof(NetCommandType), GlobalPacketNames, 11);
	Ipx.Mono_Debug_Print(-1,1);
	#endif
	while (Get_Mouse_State() > 0) Show_Mouse();

	/*
	---------------------------- Processing loop -----------------------------
	*/
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		#if(SHOW_MONO)
		Ipx.Mono_Debug_Print(-1,0);
		#endif
		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
				Set_Palette(Palette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Draw_Caption (TXT_JOIN_NETWORK_GAME, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print(TXT_YOUR_NAME,
					d_name_x - 5, d_name_y + 1, CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print(TXT_SIDE_COLON,
					d_gdi_x - 5, d_gdi_y + 1, CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print(TXT_COLOR_COLON,
					cbox_x[0] - 5, d_color_y + 1, CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print(TXT_GAMES,
					d_gamelist_x + (d_gamelist_w / 2), d_gamelist_y - d_txt6_h,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print(TXT_PLAYERS,
					d_playerlist_x + (d_playerlist_w / 2), d_playerlist_y - d_txt6_h,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				/*...............................................................
				Join-state-specific labels:
				...............................................................*/
				if (joinstate > JOIN_NOTHING) {
					Fancy_Text_Print(namebuf, d_name_x, d_name_y + 1, CC_GREEN,
						TBLACK, TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
					if (MPlayerHouse==HOUSE_GOOD) {
						Fancy_Text_Print(TXT_G_D_I, d_gdi_x, d_gdi_y + 1, CC_GREEN,
							TBLACK, TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
					} else {
						Fancy_Text_Print(TXT_N_O_D, d_gdi_x, d_gdi_y + 1, CC_GREEN,
							TBLACK, TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
					}
				}

				/*
				.................... Rebuild the button list ....................
				*/
				cancelbtn.Zap();
				gamelist.Zap();
				playerlist.Zap();
				gdibtn.Zap();
				nodbtn.Zap();
				name_edt.Zap();
				joinbtn.Zap();
				newbtn.Zap();
				sendbtn.Zap();

				commands = &cancelbtn;
				gamelist.Add_Tail(*commands);
				playerlist.Add_Tail(*commands);
				/*...............................................................
				Only add the name edit field, the House, Join & New buttons if
				we're doing nothing, or we've just been rejected.
				...............................................................*/
				if (joinstate <= JOIN_NOTHING) {
					gdibtn.Add_Tail(*commands);
					nodbtn.Add_Tail(*commands);
					name_edt.Add_Tail(*commands);
					joinbtn.Add_Tail(*commands);
					newbtn.Add_Tail(*commands);
				}
				if (joinstate == JOIN_CONFIRMED)
					sendbtn.Add_Tail(*commands);
			}
			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Draw_All();
			}

			/*..................................................................
			Draw the color boxes
			..................................................................*/
			if (display >= REDRAW_COLORS) {
				for (i = 0; i < MAX_MPLAYER_COLORS; i++) {
					LogicPage->Fill_Rect (cbox_x[i] + 1, d_color_y + 1,
						cbox_x[i] + 1 + d_color_w - 2, d_color_y + 1 + d_color_h - 2,
						MPlayerGColors[i]);

					if (i == MPlayerColorIdx) {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h,
							BOXSTYLE_GREEN_DOWN, false);
					} else {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h,
							BOXSTYLE_GREEN_RAISED, false);
					}
				}
			}

			/*..................................................................
			Draw the message:
			- Erase an old message first
			- If we're in a game, print the game options (if they've been
			  received)
			- If we've been rejected from a game, print that message
			..................................................................*/
			if (display >= REDRAW_MESSAGE) {
				Draw_Box(d_message_x, d_message_y, d_message_w, d_message_h,
					BOXSTYLE_GREEN_BORDER, true);
				Messages.Draw();

				LogicPage->Fill_Rect( d_dialog_x + 2,
					d_msg1_y,
					d_dialog_x + d_dialog_w - 4,
					d_msg5_y + d_txt6_h,
					BLACK);

				if (joinstate==JOIN_CONFIRMED && parms_received) {
					/*............................................................
					Scenario title
					............................................................*/
					p = Text_String(TXT_SCENARIO_COLON);
					if (ScenarioIdx != -1) {
						sprintf(txt,"%s %s",p, MPlayerScenarios[ScenarioIdx]);

						Fancy_Text_Print (txt, d_dialog_cx,
							d_msg1_y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW | TPF_CENTER);
					} else {
						sprintf(txt,"%s %s",p,Text_String(TXT_NOT_FOUND));

						Fancy_Text_Print (txt, d_dialog_cx,
							d_msg1_y, CC_NOD_COLOR, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW | TPF_CENTER);
					}

					/*............................................................
					# of credits
					............................................................*/
					p = Text_String(TXT_START_CREDITS_COLON);
					sprintf(txt, "%s %d", p, MPlayerCredits);
					Fancy_Text_Print (txt, d_dialog_cx,
						d_msg2_y, CC_GREEN, TBLACK,
						TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW | TPF_CENTER);

					/*............................................................
					Count & Level values
					............................................................*/
					p = Text_String(TXT_COUNT);
					sprintf(txt,"%s %d",p,MPlayerUnitCount);
					Fancy_Text_Print (txt,
						d_dialog_x + (d_dialog_w / 4) - String_Pixel_Width(p),
						d_msg3_y, CC_GREEN, TBLACK,
						TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

					p = Text_String(TXT_LEVEL);
					if (BuildLevel <= MPLAYER_BUILD_LEVEL_MAX) {
						sprintf(txt, "%s %d", p, BuildLevel);
					} else {
						sprintf(txt, "%s **", p);
					}
					Fancy_Text_Print (txt,
						d_dialog_x + d_dialog_w - (d_dialog_w / 4) - String_Pixel_Width(p),
						d_msg3_y, CC_GREEN, TBLACK,
						TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

					/*............................................................
					Bases
					............................................................*/
					p = Text_String(TXT_BASES_COLON);
					if (MPlayerBases) {
						sprintf(txt,"%s %s",p,Text_String(TXT_ON));
					} else {
						sprintf(txt,"%s %s",p,Text_String(TXT_OFF));
					}
					Fancy_Text_Print (txt,
						d_dialog_x + (d_dialog_w / 4) - String_Pixel_Width(p),
						d_msg4_y, CC_GREEN, TBLACK,
						TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

					/*............................................................
					Tiberium
					............................................................*/
					p = Text_String(TXT_TIBERIUM_COLON);
					if (MPlayerTiberium) {
						sprintf(txt,"%s %s",p,Text_String(TXT_ON));
					} else {
						sprintf(txt,"%s %s",p,Text_String(TXT_OFF));
					}

					Fancy_Text_Print (txt,
						d_dialog_x + (d_dialog_w / 4) - String_Pixel_Width(p),
						d_msg5_y, CC_GREEN, TBLACK,
						TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

					/*............................................................
					Goody boxes
					............................................................*/
					p = Text_String(TXT_CRATES_COLON);
					if (MPlayerGoodies) {
						sprintf(txt,"%s %s",p,Text_String(TXT_ON));
					} else {
						sprintf(txt,"%s %s",p,Text_String(TXT_OFF));
					}

					Fancy_Text_Print (txt,
						d_dialog_x + d_dialog_w - (d_dialog_w / 4) - String_Pixel_Width(p),
						d_msg4_y, CC_GREEN, TBLACK,
						TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

					/*............................................................
					Computer AI players
					............................................................*/
					if (Special.IsCaptureTheFlag) {
						p = Text_String(TXT_CAPTURE_THE_FLAG_COLON);
						sprintf(txt,"%s %s",p,Text_String(TXT_ON));
					} else {
						p = Text_String(TXT_AI_PLAYERS_COLON);
						if (MPlayerGhosts) {
							sprintf(txt,"%s %s",p,Text_String(TXT_ON));
						} else {
							sprintf(txt,"%s %s",p,Text_String(TXT_OFF));
						}
					}
					Fancy_Text_Print (txt,
						d_dialog_x + d_dialog_w - (d_dialog_w / 4) - String_Pixel_Width(p),
						d_msg5_y, CC_GREEN, TBLACK,
						TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				} else {

					/*...............................................................
					Rejection notice
					...............................................................*/
					if (joinstate==JOIN_REJECTED) {
						Fancy_Text_Print(TXT_REQUEST_DENIED,
							d_dialog_cx, d_msg3_y, CC_GREEN, TBLACK,
							TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
					}
				}
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {
			/*------------------------------------------------------------------
			User clicks on a color button:
			- If we've joined a game, don't allow a new color selection
			- otherwise, select that color
			------------------------------------------------------------------*/
			case KN_LMOUSE:
				if (joinstate > JOIN_NOTHING)
					break;
				if (_Kbd->MouseQX > cbox_x[0] &&
					_Kbd->MouseQX < (cbox_x[MAX_MPLAYER_COLORS - 1] + d_color_w) &&
					_Kbd->MouseQY > d_color_y &&
					_Kbd->MouseQY < (d_color_y + d_color_h)) {
						MPlayerPrefColor = (_Kbd->MouseQX - cbox_x[0]) / d_color_w;
						MPlayerColorIdx = MPlayerPrefColor;

						name_edt.Set_Color (MPlayerTColors[MPlayerColorIdx]);
						name_edt.Flag_To_Redraw();

						display = REDRAW_COLORS;
				}
				break;

			/*------------------------------------------------------------------
			User clicks on the game list:
			- If we've joined a game, don't allow the selected item to change;
			otherwise:
			- Clear the player list
			- Send an immediate player query
			------------------------------------------------------------------*/
			case (BUTTON_GAMELIST | KN_BUTTON):
				if (joinstate==JOIN_CONFIRMED) {
					gamelist.Set_Selected_Index(game_index);
				} else {
					if (gamelist.Current_Index() != game_index) {
						Clear_Player_List (&playerlist);
						game_index = gamelist.Current_Index();
						Send_Join_Queries (game_index, 0, 1);
					}
				}
				break;

			/*------------------------------------------------------------------
			House Buttons: set the player's desired House
			------------------------------------------------------------------*/
			case (BUTTON_GDI | KN_BUTTON):
				MPlayerHouse = HOUSE_GOOD;
				gdibtn.Turn_On();
				nodbtn.Turn_Off();
				break;

			case (BUTTON_NOD | KN_BUTTON):
				MPlayerHouse = HOUSE_BAD;
				gdibtn.Turn_Off();
				nodbtn.Turn_On();
				break;

			/*------------------------------------------------------------------
			JOIN: send a join request packet & switch to waiting-for-confirmation
			mode.  (Request_To_Join fills in MPlayerName with my namebuf.)
			------------------------------------------------------------------*/
			case (BUTTON_JOIN | KN_BUTTON):
				name_edt.Clear_Focus();
				name_edt.Flag_To_Redraw();

				join_index = gamelist.Current_Index();
				parms_received = 0;
				if (Request_To_Join (namebuf, join_index, &playerlist, MPlayerHouse,
					MPlayerColorIdx)) {
					joinstate = JOIN_WAIT_CONFIRM;
				} else {
					display = REDRAW_ALL;
				}
				break;

			/*------------------------------------------------------------------
			CANCEL: send a SIGN_OFF
			- If we're part of a game, stay in this dialog; otherwise, exit
			------------------------------------------------------------------*/
			case (KN_ESC):
				if (Messages.Get_Edit_Buf() != NULL) {
					Messages.Input(input);
					display = REDRAW_MESSAGE;
					break;
				}
			case (BUTTON_CANCEL | KN_BUTTON):
				memset (&GPacket, 0, sizeof(GlobalPacketType));

				GPacket.Command = NET_SIGN_OFF;
				strcpy(GPacket.Name,MPlayerName);

				/*...............................................................
				If we're joined to a game, make extra sure the other players in
				that game know I'm exiting; send my SIGN_OFF as an ack-required
				packet.  Do not send this packet to myself (index 0).
				...............................................................*/
				if (joinstate == JOIN_CONFIRMED) {
					//
					// Remove myself from the player list box
					//
					if (playerlist.Count()) { 	// added: BRR 6/14/96
						item = (char *)(playerlist.Get_Item(0));
						playerlist.Remove_Item(item);
						delete [] item;
						playerlist.Flag_To_Redraw();
					}

					//
					// Remove myself from the Players list
					//
					if (Players.Count()) {		// added: BRR 6/14/96
						who = Players[0];
						Players.Delete(0);
						delete who;
					}

					for (i = 0; i < Players.Count(); i++) {
						Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 1,
							&(Players[i]->Address));
						Ipx.Service();
					}
				}

				/*...............................................................
				Now broadcast my SIGN_OFF so other players looking at this game
				know I'm leaving.
				...............................................................*/
				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					0, NULL);
				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					0, NULL);

				if (IsBridge) {
					Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
						&BridgeNet);
					Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
						&BridgeNet);
				}

				while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;

				if (joinstate != JOIN_CONFIRMED) {
					process = false;
					rc = -1;
				} else {
					MPlayerGameName[0] = 0;
					joinstate = JOIN_NOTHING;
					display = REDRAW_ALL;
				}
				break;

			/*------------------------------------------------------------------
			NEW: bail out with return code 1
			------------------------------------------------------------------*/
			case (BUTTON_NEW | KN_BUTTON):
				/*
				.................. Force user to enter a name ...................
				*/
				if (strlen(namebuf)==0) {
					CCMessageBox().Process(TXT_NAME_ERROR);
					display = REDRAW_ALL;
					break;
				}
				/*
				..................... Ensure name is unique .....................
				*/
				found = 0;
				for (i = 0; i < Games.Count(); i++) {
					if (!stricmp(Games[i]->Name, namebuf)) {
						found = 1;
						CCMessageBox().Process (TXT_GAMENAME_MUSTBE_UNIQUE);
						display = REDRAW_ALL;
						break;
					}
				}
				if (found)
					break;
				/*
				.................... Save player & game name ....................
				*/
				strcpy(MPlayerName,namebuf);
				strcpy(MPlayerGameName,namebuf);

				name_edt.Clear_Focus();
				name_edt.Flag_To_Redraw();

				rc = 1;
				process = false;
				break;

			/*------------------------------------------------------------------
			Default: manage the inter-player messages
			------------------------------------------------------------------*/
			default:
				/*...............................................................
				F4/SEND/'M' = edit a message
				...............................................................*/
				if (Messages.Get_Edit_Buf()==NULL) {
					if ( (input == KN_M && joinstate==JOIN_CONFIRMED) ||
						input==(BUTTON_SEND | KN_BUTTON) || input == KN_F4) {
						memset (txt, 0, 80);

						strcpy(txt,Text_String(TXT_TO_ALL));	// "To All:"

						Messages.Add_Edit (MPlayerTColors[MPlayerColorIdx],
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, d_message_w-70*factor);

						if (joinstate <= JOIN_NOTHING) {
							name_edt.Clear_Focus();
							name_edt.Flag_To_Redraw();
						}

						display = REDRAW_MESSAGE;

						break;
					}
				} else

				/*...............................................................
				If we're already editing a message and the user clicks on
				'Send', translate our input to a Return so Messages.Input() will
				work properly.
				...............................................................*/
				if (input==(BUTTON_SEND | KN_BUTTON)) {
					input = KN_RETURN;
				}

				/*...............................................................
				Manage the message system (get rid of old messages)
				...............................................................*/
				if (Messages.Manage()) {
					display = REDRAW_MESSAGE;
				}

				/*...............................................................
				Service keyboard input for any message being edited.
				...............................................................*/
				i = Messages.Input(input);

				/*...............................................................
				If 'Input' returned 1, it means refresh the message display.
				...............................................................*/
				if (i==1) {
					Messages.Draw();
				} else {

					/*...............................................................
					If 'Input' returned 2, it means redraw the message display.
					...............................................................*/
					if (i==2) {
						display = REDRAW_MESSAGE;
					} else {

						/*...............................................................
						If 'input' returned 3, it means send the current message.
						...............................................................*/
						if (i==3) {
							long actual_message_size;
							char *the_string;

							sent_so_far = 0;
 							magic_number = MESSAGE_HEAD_MAGIC_NUMBER;
							message_length = strlen(Messages.Get_Edit_Buf());
							crc = (unsigned short)
									(Calculate_CRC(Messages.Get_Edit_Buf(),message_length) &0xffff);

							while ( sent_so_far < message_length ){
								GPacket.Command = NET_MESSAGE;
								strcpy (GPacket.Name, namebuf);
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
										the_string[i + actual_message_size] = 0;	//0xff; ST - 12/18/2018 11:36AM
									}
								}else{
									actual_message_size = COMPAT_MESSAGE_LENGTH - 5;
								}


								*(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-5) = 0;
								*((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-4)) = magic_number;
								*((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-2)) = crc;
								GPacket.Message.ID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);
								GPacket.Message.NameCRC = Compute_Name_CRC(MPlayerGameName);

								/*..................................................................
								Send the message to every player in our player list.  The local
								system will also receive this message, since it's in the Player list.
								..................................................................*/
								if (joinstate == JOIN_CONFIRMED) {
									for (i = 1; i < Players.Count(); i++) {
										Ipx.Send_Global_Message (&GPacket,
											sizeof(GlobalPacketType), 1, &(Players[i]->Address));
										Ipx.Service();
									}

									sprintf(txt,Text_String (TXT_FROM), MPlayerName, GPacket.Message.Buf);
									Messages.Add_Message (txt,
										MPlayerTColors[MPlayerColorIdx],
										TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 600, magic_number, crc);
								}
								else {
									for (i = 0; i < Players.Count(); i++) {
										Ipx.Send_Global_Message (&GPacket,
											sizeof(GlobalPacketType), 1, &(Players[i]->Address));
										Ipx.Service();
									}
								}
								magic_number++;
								sent_so_far += actual_message_size;	//COMPAT_MESSAGE_LENGTH-5;
							}
						}
					}
				}
				break;
		}

		/*---------------------------------------------------------------------
		Resend our query packets
		---------------------------------------------------------------------*/
		Send_Join_Queries(game_index, 0, 0);

		/*---------------------------------------------------------------------
		Process incoming packets
		---------------------------------------------------------------------*/
		event = Get_Join_Responses(&joinstate, &gamelist, &playerlist,
			join_index);
		/*.....................................................................
		If we've changed state, redraw everything; if we're starting the game,
		break out of the loop.  If we've just joined, send out a player query
		so I'll get added to the list instantly.
		.....................................................................*/
		if (event == EV_STATE_CHANGE) {
			display = REDRAW_ALL;
			if (joinstate==JOIN_GAME_START) {
				rc = 0;
				process = false;
			} else {

				/*..................................................................
				If we're newly-confirmed, immediately send out a player query
				..................................................................*/
				if (joinstate==JOIN_CONFIRMED) {

					Clear_Player_List(&playerlist);

					item = new char [MPLAYER_NAME_MAX + 4];
					if (MPlayerHouse==HOUSE_GOOD) {
						sprintf(item,"%s\t%s",MPlayerName,Text_String(TXT_G_D_I));
					} else {
						sprintf(item,"%s\t%s",MPlayerName,Text_String(TXT_N_O_D));
					}
					playerlist.Add_Item (item, MPlayerTColors[MPlayerColorIdx]);

					who = new NodeNameType;
					strcpy(who->Name, MPlayerName);
					who->Address = IPXAddressClass();
					who->Player.House = MPlayerHouse;
					who->Player.Color = MPlayerColorIdx;
					Players.Add (who);

					Send_Join_Queries (game_index, 0, 1);
				} else {

					/*..................................................................
					If we've been rejected, clear any messages we may have been typing.
					..................................................................*/
					if (joinstate==JOIN_REJECTED) {

						//
						// Remove myself from the player list box
						//
						if (playerlist.Count()) { 	// added: BRR 6/14/96
							item = (char *)(playerlist.Get_Item(0));
							if (item){
								playerlist.Remove_Item(item);
								delete [] item;
								playerlist.Flag_To_Redraw();
							}
						}

						//
						// Remove myself from the Players list
						//
						if (Players.Count()){
							who = Players[0];
							Players.Delete(0);
							delete who;
						}

						Messages.Init (d_message_x + 2, d_message_y + 2, 4,
							MAX_MESSAGE_LENGTH, d_txt6_h);
					}
				}
			}
		} else

		/*.....................................................................
		If a new game is detected, and it's the first game on our list,
		automatically send out a player query for that game.
		.....................................................................*/
		if (event == EV_NEW_GAME && gamelist.Count()==1) {
			gamelist.Set_Selected_Index(0);
			game_index = gamelist.Current_Index();
			Send_Join_Queries (game_index, 0, 1);
		} else

		/*.....................................................................
		If the game options have changed, print them.
		.....................................................................*/
		if (event == EV_GAME_OPTIONS) {
			parms_received = 1;
			display = REDRAW_MESSAGE;
		} else

		/*.....................................................................
		Draw an incoming message
		.....................................................................*/
		if (event == EV_MESSAGE) {
			display = REDRAW_MESSAGE;
		} else

		/*.....................................................................
		A game before the one I've selected is gone, so we have a new index now.
		'game_index' must be kept set to the currently-selected list item, so
		we send out queries for the currently-selected game.  It's therefore
		imperative that we detect any changes to the game list.
		If we're joined in a game, we must decrement our game_index to keep
		it aligned with the game we're joined to.
		.....................................................................*/
		if (event == EV_GAME_SIGNOFF) {
			if (joinstate==JOIN_CONFIRMED) {
				game_index--;
				join_index--;
				gamelist.Set_Selected_Index(join_index);
			} else {
				gamelist.Flag_To_Redraw();
				Clear_Player_List(&playerlist);
				game_index = gamelist.Current_Index();
				Send_Join_Queries (game_index, 0, 1);
			}
		}

		/*---------------------------------------------------------------------
		Service the Ipx connections
		---------------------------------------------------------------------*/
		Ipx.Service();

		/*---------------------------------------------------------------------
		Clean out the Game List; if an old entry is found:
		- Remove it
		- Clear the player list
		- Send queries for the new selected game, if there is one
		---------------------------------------------------------------------*/
		for (i = 0; i < Games.Count(); i++) {
			if (TickCount.Time() - Games[i]->Game.LastTime > 400) {
				Games.Delete(Games[i]);
				item = (char *)(gamelist.Get_Item (i));
				gamelist.Remove_Item (item);
				delete [] item;
				if (i <= game_index) {
					gamelist.Flag_To_Redraw();
					Clear_Player_List(&playerlist);
					game_index = gamelist.Current_Index();
					Send_Join_Queries (game_index, 0, 1);
				}
			}
		}

		/*---------------------------------------------------------------------
		Service the sounds & score; GameActive must be false at this point,
		so Call_Back() doesn't intercept global messages from me!
		---------------------------------------------------------------------*/
		Call_Back();
	}

	/*------------------------------------------------------------------------
	Establish connections with all other players.
	------------------------------------------------------------------------*/
	if (rc == 0) {
		/*.....................................................................
		If the other guys are playing a scenario I don't have (sniff), I can't
		play.  Try to bail gracefully.
		.....................................................................*/
		if (ScenarioIdx==-1) {
			CCMessageBox().Process (TXT_UNABLE_PLAY_WAAUGH);

			//
			// Remove myself from the player list box
			//
			if (playerlist.Count()) { 	// added: BRR 6/14/96
				item = (char *)(playerlist.Get_Item(0));
				playerlist.Remove_Item(item);
				delete [] item;
				playerlist.Flag_To_Redraw();
			}

			//
			// Remove myself from the Players list
			//
			if (Players.Count()) {		// added: BRR 6/14/96
				who = Players[0];
				Players.Delete(0);
				delete who;
			}

			memset (&GPacket, 0, sizeof(GlobalPacketType));

			GPacket.Command = NET_SIGN_OFF;
			strcpy (GPacket.Name, MPlayerName);

			for (i = 0; i < Players.Count(); i++) {
				Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 1,
					&(Players[i]->Address));
				Ipx.Service();
			}

			Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
				0, NULL);
			Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
				0, NULL);

			if (IsBridge) {
				Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
					&BridgeNet);
				Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
					&BridgeNet);
			}

			while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;

			rc = -1;

		} else {

			/*..................................................................
			Set the number of players in this game, and my ID
			..................................................................*/
			MPlayerCount = Players.Count();
			MPlayerLocalID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);

			/*..................................................................
			Get the scenario number
			..................................................................*/
			Scenario = MPlayerFilenum[ScenarioIdx];

			/*..................................................................
			Form connections with all other players.  Form the IPX Connection ID
			from the player's Color and House.  This will let us extract any
			player's color & house at any time.  Fill in 'tmp_id' while we're
			doing this.
			..................................................................*/
			for (i = 0; i < Players.Count(); i++) {

				/*...............................................................
				Only create the connection if it's not myself!
				...............................................................*/
				if (strcmp (MPlayerName, Players[i]->Name)) {
					id = Build_MPlayerID(Players[i]->Player.Color,
						Players[i]->Player.House);

					tmp_id[i] = id;

					Ipx.Create_Connection((int)id, Players[i]->Name, &(Players[i]->Address) );
				} else {
					tmp_id[i] = MPlayerLocalID;
				}
			}

			/*..................................................................
			Store every player's ID in the MPlayerID[] array.  This array will
			determine the order of event execution, so the ID's must be stored
			in the same order on all systems.
			..................................................................*/
			for (i = 0; i < MPlayerCount; i++) {
				min_index = 0;
				min_id = 0xff;
				for (j = 0; j < MPlayerCount; j++) {
					if (tmp_id[j] < min_id) {
						min_id = tmp_id[j];
						min_index = j;
					}
				}
				MPlayerID[i] = tmp_id[min_index];
				tmp_id[min_index] = 0xff;
			}
			/*..................................................................
			Fill in the array of player names, including my own.
			..................................................................*/
			for (i = 0; i < MPlayerCount; i++) {
				if (MPlayerID[i] == MPlayerLocalID) {
					strcpy (MPlayerNames[i], MPlayerName);
				} else {
					strcpy (MPlayerNames[i], Ipx.Connection_Name(MPlayerID[i]));
				}
			}
		}
		/*---------------------------------------------------------------------
		Wait a while, polling the IPX service routines, to give our ACK
		a chance to get to the other system.  If he doesn't get our ACK, he'll
		be waiting the whole time we load MIX files.
		---------------------------------------------------------------------*/
		i = MAX(Ipx.Global_Response_Time() * 2, (unsigned long)60);
		starttime = TickCount.Time();
		while (TickCount.Time() - starttime < (unsigned)i) {
			Ipx.Service();
		}
	}

	/*------------------------------------------------------------------------
	Init network timing values, using previous response times as a measure
	of what our retry delta & timeout should be.
	------------------------------------------------------------------------*/
	Ipx.Set_Timing (Ipx.Global_Response_Time() + 2, -1,
		Ipx.Global_Response_Time() * 4);

	/*------------------------------------------------------------------------
	Clear all lists
	------------------------------------------------------------------------*/
	Clear_Game_List(&gamelist);
	Clear_Player_List(&playerlist);

	/*------------------------------------------------------------------------
	Restore screen
	------------------------------------------------------------------------*/
	Hide_Mouse();
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Show_Mouse();

	return(rc);
}


/***************************************************************************
 * Clear_Game_List -- Clears the game-name listbox & 'Games' Vector        *
 *                                                                         *
 * Assumes each entry in 'Games' & the list box have been allocated		   *
 * separately.																				   *
 *                                                                         *
 * INPUT:                                                                  *
 *		gamelist		ptr to list box													   *
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none																					   *
 *                                                                         *
 * WARNINGS:                                                               *
 *		none																					   *
 *                                                                         *
 * HISTORY:                                                                *
 *=========================================================================*/
static void Clear_Game_List (ListClass *gamelist)
{
	char * item;
	int i;

	/*------------------------------------------------------------------------
	Clear the list box
	------------------------------------------------------------------------*/
	while (gamelist->Count()) {
		item = (char *)(gamelist->Get_Item (0));
		gamelist->Remove_Item(item);
		delete [] item;
	}
	gamelist->Flag_To_Redraw();

	/*------------------------------------------------------------------------
	Clear the 'Games' Vector
	------------------------------------------------------------------------*/
	for (i = 0; i < Games.Count(); i++)
		delete Games[i];

	Games.Clear();

}	/* end of Clear_Game_List */


/***************************************************************************
 * Clear_Player_List -- Clears the player-name listbox & Vector     		   *
 *                                                                         *
 * Assumes each entry in 'Players' & the list box have been allocated		*
 * separately.																				   *
 *                                                                         *
 * INPUT:                                                                  *
 *		playerlist		ptr to list box												   *
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none																					   *
 *                                                                         *
 * WARNINGS:                                                               *
 *		none																					   *
 *                                                                         *
 * HISTORY:                                                                *
 *=========================================================================*/
static void Clear_Player_List (ListClass *playerlist)
{
	char * item;
	int i;

	/*------------------------------------------------------------------------
	Clear the list box
	------------------------------------------------------------------------*/
	while (playerlist->Count()) {
		item = (char *)(playerlist->Get_Item(0));
		playerlist->Remove_Item(item);
		delete [] item;
	}
	playerlist->Flag_To_Redraw();

	/*------------------------------------------------------------------------
	Clear the 'Players' Vector
	------------------------------------------------------------------------*/
	for (i = 0; i < Players.Count(); i++)
		delete Players[i];

	Players.Clear();

}	/* end of Clear_Player_List */


/***************************************************************************
 * Request_To_Join -- Sends a JOIN request packet to game owner            *
 *                                                                         *
 * Regardless of the return code, the Join Dialog will need to be redrawn	*
 * after calling this routine.															*
 *                                                                         *
 * INPUT:                                                                  *
 *		playername		player's name														*
 *		join_index		index of game we're joining									*
 *		playerlist		listbox containing other players' names					*
 *		house				requested house													*
 *		color				requested color													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = Packet sent, 0 = wasn't														*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *=========================================================================*/
static int Request_To_Join (char *playername, int join_index, ListClass *playerlist,
	HousesType house, int color)
{
	int i;

	playerlist = playerlist;	// shaddup, Mr stupid compiler!

	/*
	--------------------------- Validate join_index --------------------------
	*/
	if ( (Games.Count()==0) || join_index > Games.Count() || join_index < 0) {
		CCMessageBox().Process (TXT_NOTHING_TO_JOIN);
		return(false);
	}

	/*
	----------------------- Force user to enter a name -----------------------
	*/
	if (strlen(playername)==0) {
		CCMessageBox().Process (TXT_NAME_ERROR);
		return(false);
	}

	/*
	------------------------- The game must be open --------------------------
	*/
	if (!Games[join_index]->Game.IsOpen) {
		CCMessageBox().Process(TXT_GAME_IS_CLOSED);
		return (false);
	}

	/*
	------------------------ Make sure name is unique ------------------------
	*/
	for (i = 0; i < Players.Count(); i++) {
		if (!stricmp(playername, Players[i]->Name)) {
			CCMessageBox().Process (TXT_NAME_MUSTBE_UNIQUE);
			return(false);
		}
	}

	/*
	----------------------------- Check version #'s --------------------------
	*/
	int v;
#ifdef PATCH
	if (IsV107) {
		v = 1;
	} else {
		v = 2;
	}
#else
	v = Version_Number();
#endif
	if (Games[join_index]->Game.Version > v) {
		CCMessageBox().Process (TXT_YOURGAME_OUTDATED);
		return(false);
	} else {
		if (Games[join_index]->Game.Version < v) {
			CCMessageBox().Process (TXT_DESTGAME_OUTDATED);
			return(false);
		}
	}

	/*
	----------------------------- Save game name -----------------------------
	*/
	strcpy (MPlayerName,playername);

	/*
	----------------------- Send packet to game's owner ----------------------
	*/
	memset (&GPacket, 0, sizeof(GlobalPacketType));

	GPacket.Command = NET_QUERY_JOIN;
	strcpy (GPacket.Name, MPlayerName);
	GPacket.PlayerInfo.House = house;
	GPacket.PlayerInfo.Color = color;

	Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 1,
		&(Games[join_index]->Address));

	return(true);
}


/***********************************************************************************************
 * Send_Join_Queries -- sends queries for the Join Dialog												  *
 *                                                                         						  *
 * This routine [re]sends the queries related to the Join Dialog:										  *
 * - NET_QUERY_GAME																									  *
 * - NET_QUERY_PLAYER for the game currently selected (if there is one)								  *
 *																															  *
 * The queries are "staggered" in time so they aren't all sent at once; otherwise, we'd		  *
 * be inundated with reply packets & we'd miss some (even though the replies will require		  *
 * ACK's).																												  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		curgame		index of currently-selected game; -1 = none											  *
 *		gamenow		if 1, will immediately send the game query											  *
 *		playernow	if 1, will immediately send the player query for currently-selected game	  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		none.																												  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.                                              						  *
 *   04/15/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static void Send_Join_Queries(int curgame, int gamenow, int playernow)
{
	static int lasttime1 = 0;		// time since last Game query sent out
	static int lasttime2 = 0;		// time since last Player query sent out

	/*------------------------------------------------------------------------
	Send the game-name query if the time has expired, or we're told to do
	it right now
	------------------------------------------------------------------------*/
	if ( (TickCount.Time() - lasttime1 > 120) || gamenow) {
		lasttime1 = TickCount.Time();

		memset (&GPacket, 0, sizeof(GlobalPacketType));

		GPacket.Command = NET_QUERY_GAME;

		Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0, NULL);

		/*.....................................................................
		If the user specified a remote server address, broadcast over that
		network, too.
		.....................................................................*/
		if (IsBridge)
			Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
				&BridgeNet);
	}

	/*------------------------------------------------------------------------
	Send the player query for the game currently clicked on, if the time has
	expired and there is a currently-selected game, or we're told to do it
	right now
	------------------------------------------------------------------------*/
	if ( (curgame != -1) && curgame < Games.Count() &&
		((TickCount.Time() - lasttime2 > 35) || playernow) ) {
		lasttime2 = TickCount.Time();

		memset (&GPacket, 0, sizeof(GlobalPacketType));

		GPacket.Command = NET_QUERY_PLAYER;
		strcpy (GPacket.Name, Games[curgame]->Name);

		Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0, NULL);

		/*.....................................................................
		If the user specified a remote server address, broadcast over that
		network, too.
		.....................................................................*/
		if (IsBridge)
			Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
				&BridgeNet);
	}

}	/* end of Send_Join_Queries */


/***********************************************************************************************
 * Get_Join_Responses -- sends queries for the Join Dialog												  *
 *                                                                         						  *
 * This routine polls the Global Channel to see if there are any incoming packets;				  *
 * if so, it processes them.  This routine can change the state of the Join Dialog, or			  *
 * the contents of the list boxes, based on what the packet is.										  *
 *                                                                         						  *
 * The list boxes are passed in as pointers; they can't be made globals, because they			  *
 * can't be constructed, because they require shape pointers to the arrow buttons, and			  *
 * the mix files won't have been initialized when the global variables' constructors are		  *
 * called.																												  *
 *                                                                         						  *
 * This routine sets the globals 																				  *
 *		MPlayerHouse			(from NET_CONFIRM_JOIN)															  *
 *		MPlayerColorIdx		(from NET_CONFIRM_JOIN)															  *
 *		MPlayerBases			(from NET_GAME_OPTIONS)															  *
 *		MPlayerTiberium		(from NET_GAME_OPTIONS)															  *
 *		MPlayerGoodies			(from NET_GAME_OPTIONS)															  *
 *		MPlayerGhosts			(from NET_GAME_OPTIONS)															  *
 *		ScenarioIdx				(from NET_GAME_OPTIONS; -1 = scenario not found)						  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		joinstate		current state of Join Dialog															  *
 *		gamelist			list box containing game names														  *
 *		playerlist		list box containing player names for the currently-selected game			  *
 *		join_index		index of the game we've joined or are asking to join							  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		Event that occurred																							  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.                                              						  *
 *   04/15/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static JoinEventType Get_Join_Responses(JoinStateType *joinstate, ListClass *gamelist,
	ColorListClass *playerlist, int join_index)
{
	int rc;
	char * item;						// general-purpose string
	NodeNameType *who;				// node to add to Games or Players
	int i;
	int found;
	JoinEventType retcode = EV_NONE;
	char txt[80];
	int color;
	unsigned short magic_number;
	unsigned short	crc;

	/*------------------------------------------------------------------------
	If there is no incoming packet, just return
	------------------------------------------------------------------------*/
	rc = Ipx.Get_Global_Message (&GPacket, &GPacketlen, &GAddress, &GProductID);
	if (!rc || GProductID != IPXGlobalConnClass::COMMAND_AND_CONQUER)
		return(EV_NONE);

	/*------------------------------------------------------------------------
	If we're joined in a game, handle the packet in a standard way; otherwise,
	don't answer standard queries.
	------------------------------------------------------------------------*/
	if ( (*joinstate)==JOIN_CONFIRMED &&
		Process_Global_Packet(&GPacket,&GAddress)!=0)
		return(EV_NONE);

	/*------------------------------------------------------------------------
	NET_ANSWER_GAME:  Another system is answering our GAME query, so add that
	system to our list box if it's new.
	------------------------------------------------------------------------*/
	if (GPacket.Command==NET_ANSWER_GAME) {
		/*.....................................................................
		See if this name is unique
		.....................................................................*/
		retcode = EV_NONE;
		found = 0;
		for (i = 0; i < Games.Count(); i++) {
			if (!strcmp(Games[i]->Name, GPacket.Name)) {
				found = 1;
				/*...............................................................
				If name was found, update the node's time stamp & IsOpen flag.
				...............................................................*/
				Games[i]->Game.LastTime = TickCount.Time();
				if (Games[i]->Game.IsOpen != GPacket.GameInfo.IsOpen) {
					item = (char *)gamelist->Get_Item(i);
					if (GPacket.GameInfo.IsOpen) {
						sprintf(item,Text_String(TXT_THATGUYS_GAME),GPacket.Name);
					} else {
						sprintf(item,Text_String(TXT_THATGUYS_GAME_BRACKET),GPacket.Name);
					}
					Games[i]->Game.IsOpen = GPacket.GameInfo.IsOpen;
					gamelist->Flag_To_Redraw();
					/*............................................................
					If this game has gone from closed to open, copy the responder's
					address into our Game slot, since the guy responding to this
					must be game owner.
					............................................................*/
					if (Games[i]->Game.IsOpen)
						Games[i]->Address = GAddress;
				}
				break;
			}
		}
		/*.....................................................................
		name not found (or addresses are different); add it to 'Games'
		.....................................................................*/
		if (found==0) {
			/*..................................................................
			Create a new node structure, fill it in, add it to 'Games'
			..................................................................*/
			who = new NodeNameType;
			strcpy(who->Name, GPacket.Name);
			who->Address = GAddress;
			who->Game.Version = GPacket.GameInfo.Version;
			who->Game.IsOpen = GPacket.GameInfo.IsOpen;
			who->Game.LastTime = TickCount.Time();
			Games.Add (who);

			/*..................................................................
			Create a string for "xxx's Game", leaving room for brackets around
			the string if it's a closed game
			..................................................................*/
			item = new char [MPLAYER_NAME_MAX + 9];
			if (GPacket.GameInfo.IsOpen) {
				sprintf(item,Text_String(TXT_THATGUYS_GAME),GPacket.Name);
			} else {
				sprintf(item,Text_String(TXT_THATGUYS_GAME_BRACKET),GPacket.Name);
			}
			gamelist->Add_Item(item);

			retcode = EV_NEW_GAME;
		}
	}

	/*------------------------------------------------------------------------
	NET_ANSWER_PLAYER: Another system is answering our PLAYER query, so add it
	to our player list box & the Player Vector if it's new
	------------------------------------------------------------------------*/
	else if (GPacket.Command==NET_ANSWER_PLAYER) {
		/*.....................................................................
		See if this name is unique
		.....................................................................*/
		retcode = EV_NONE;
		found = 0;
		for (i = 0; i < Players.Count(); i++) {
			/*..................................................................
			If the address is already present, re-copy their name, color &
			house into the existing entry, in case they've changed it without
			our knowledge; set the 'found' flag so we won't create a new entry.
			..................................................................*/
			if (Players[i]->Address==GAddress) {
				strcpy(Players[i]->Name, GPacket.Name);
				Players[i]->Player.House = GPacket.PlayerInfo.House;
				Players[i]->Player.Color = GPacket.PlayerInfo.Color;
				playerlist->Colors[i] = MPlayerTColors[GPacket.PlayerInfo.Color];
				found = 1;
				break;
			}
		}
		/*.....................................................................
		Don't add this player if he's not part of the game that's selected.
		.....................................................................*/
		i = gamelist->Current_Index();
		if (Games.Count() && GPacket.PlayerInfo.NameCRC != Compute_Name_CRC(Games[i]->Name))
			found = 1;

		/*
		** Dont add this player if its really me! (hack, hack)
		*/
		if (!strcmp(GPacket.Name, MPlayerName)){
			found = 1;
		}


		/*.....................................................................
		name not found (or address didn't match); add to player list box & Vector
		.....................................................................*/
		if (found==0) {
			/*..................................................................
			Create & add a node to the Vector
			..................................................................*/
			who = new NodeNameType;
			strcpy(who->Name, GPacket.Name);
			who->Address = GAddress;
			who->Player.House = GPacket.PlayerInfo.House;
			who->Player.Color = GPacket.PlayerInfo.Color;
			Players.Add (who);

			/*..................................................................
			Create & add a string to the list box
			..................................................................*/
			item = new char [MPLAYER_NAME_MAX + 4];
			if (GPacket.PlayerInfo.House==HOUSE_GOOD) {
				sprintf(item,"%s\t%s",GPacket.Name,Text_String(TXT_G_D_I));
			} else {
				sprintf(item,"%s\t%s",GPacket.Name,Text_String(TXT_N_O_D));
			}
			playerlist->Add_Item(item, MPlayerTColors[who->Player.Color]);

			retcode = EV_NEW_PLAYER;
		}
	}

	/*------------------------------------------------------------------------
	NET_CONFIRM_JOIN: The game owner has confirmed our JOIN query; mark us as
	being confirmed, and start answering queries from other systems
	------------------------------------------------------------------------*/
	else if (GPacket.Command==NET_CONFIRM_JOIN) {
		if ( (*joinstate) != JOIN_CONFIRMED) {
			strcpy (MPlayerGameName, GPacket.Name);
			MPlayerHouse = GPacket.PlayerInfo.House;
			MPlayerColorIdx = GPacket.PlayerInfo.Color;

			(*joinstate) = JOIN_CONFIRMED;
			retcode = EV_STATE_CHANGE;
		}
	}

	/*------------------------------------------------------------------------
	NET_REJECT_JOIN: The game owner has turned down our JOIN query; restore
	the dialog state to its first pop-up state.  Broadcast a sign-off to
	tell all other systems that I'm no longer a part of any game; this way,
	I'll be properly removed from their dialogs.
	------------------------------------------------------------------------*/
	else if (GPacket.Command==NET_REJECT_JOIN) {
		if ( (*joinstate) != JOIN_REJECTED) {
			memset (&GPacket, 0, sizeof(GlobalPacketType));

			GPacket.Command = NET_SIGN_OFF;
			strcpy (GPacket.Name,MPlayerName);

			Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType), 0, NULL);
			Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType), 0, NULL);

			if (IsBridge) {
				Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
					&BridgeNet);
				Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
					&BridgeNet);
			}

			while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;

			MPlayerGameName[0] = 0;

			(*joinstate) = JOIN_REJECTED;
			retcode = EV_STATE_CHANGE;
		}
	}

	/*------------------------------------------------------------------------
	NET_GAME_OPTIONS: The game owner has changed the game options & is sending
	us the new values.
	------------------------------------------------------------------------*/
	else if (GPacket.Command==NET_GAME_OPTIONS) {
		if ( (*joinstate)==JOIN_CONFIRMED) {
			MPlayerCredits = GPacket.ScenarioInfo.Credits;
			MPlayerBases = GPacket.ScenarioInfo.IsBases;
			MPlayerTiberium = GPacket.ScenarioInfo.IsTiberium;
			MPlayerGoodies = GPacket.ScenarioInfo.IsGoodies;
			MPlayerGhosts = GPacket.ScenarioInfo.IsGhosties;
			BuildLevel = GPacket.ScenarioInfo.BuildLevel;
			MPlayerUnitCount = GPacket.ScenarioInfo.UnitCount;
			Seed = GPacket.ScenarioInfo.Seed;
			Special = GPacket.ScenarioInfo.Special;
			Options.GameSpeed = GPacket.ScenarioInfo.GameSpeed;

			if (MPlayerTiberium) {
				Special.IsTGrowth = 1;
				Special.IsTSpread = 1;
			} else {
				Special.IsTGrowth = 0;
				Special.IsTSpread = 0;
			}

			if (Winsock.Get_Connected()){
				ScenarioIdx = GPacket.ScenarioInfo.Scenario;
			}else{

				ScenarioIdx = -1;
				for (i = 0; i < MPlayerFilenum.Count(); i++) {
					if (GPacket.ScenarioInfo.Scenario == MPlayerFilenum[i])
						ScenarioIdx = i;
				}
			}

			retcode = EV_GAME_OPTIONS;
		}
	}

	/*------------------------------------------------------------------------
	NET_SIGN_OFF: Another system is signing off: search for that system in
	both the game list & player list, & remove it if found
	------------------------------------------------------------------------*/
	else if (GPacket.Command==NET_SIGN_OFF) {
		/*.....................................................................
		Remove this name from the list of games
		.....................................................................*/
		for (i = 0; i < Games.Count(); i++) {
			if (!strcmp(Games[i]->Name, GPacket.Name) &&
				Games[i]->Address==GAddress) {
				/*...............................................................
				If the system signing off is the currently-selected list
				item, clear the player list since that game is no longer
				forming.
				...............................................................*/
				if (i==gamelist->Current_Index()) {
					Clear_Player_List (playerlist);
				}

				/*...............................................................
				If the system signing off was the owner of our game, mark
				ourselves as rejected
				...............................................................*/
				if ( (*joinstate) > JOIN_NOTHING && i==join_index) {
					(*joinstate) = JOIN_REJECTED;
					retcode = EV_STATE_CHANGE;
				}

				/*
				....................... Set my return code ......................
				*/
				if (retcode == EV_NONE) {
					if (i <= gamelist->Current_Index()) {
						retcode = EV_GAME_SIGNOFF;
					} else {
						retcode = EV_PLAYER_SIGNOFF;
					}
				}

				/*
				................. Remove game name from game list ...............
				*/
				Games.Delete(Games[i]);
				item = (char *)(gamelist->Get_Item (i));
				gamelist->Remove_Item (item);
				delete [] item;
				gamelist->Flag_To_Redraw();

			}
		}
		/*.....................................................................
		Remove this name from the list of players
		.....................................................................*/
		for (i = 0; i < Players.Count(); i++) {
			/*
			..................... Name found; remove it .....................
			*/
			if (Players[i]->Address==GAddress) {
				item = (char *)(playerlist->Get_Item(i));
				playerlist->Remove_Item(item);
				delete [] item;
				Players.Delete(Players[i]);
				playerlist->Flag_To_Redraw();

				if (retcode == EV_NONE)
					retcode = EV_PLAYER_SIGNOFF;
			}
		}
	}

	/*------------------------------------------------------------------------
	NET_GO: The game's owner is signalling us to start playing.
	------------------------------------------------------------------------*/
	else if (GPacket.Command==NET_GO) {
		if ( (*joinstate)==JOIN_CONFIRMED) {
			MPlayerMaxAhead = GPacket.ResponseTime.OneWay;
			(*joinstate) = JOIN_GAME_START;
			retcode = EV_STATE_CHANGE;
CCDebugString ("C&C95 - Received the 'GO' packet\n");
		}
	}

	/*------------------------------------------------------------------------
	NET_MESSAGE: Someone is sending us a message
	------------------------------------------------------------------------*/
	else if (GPacket.Command==NET_MESSAGE) {
		sprintf(txt,Text_String (TXT_FROM), GPacket.Name, GPacket.Message.Buf);
		magic_number = *((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-4));
		crc = *((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-2));
		color = MPlayerID_To_ColorIndex(GPacket.Message.ID);
		Messages.Add_Message (txt, MPlayerTColors[color],
			TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 1200, magic_number, crc);
		retcode = EV_MESSAGE;
	}

	/*------------------------------------------------------------------------
	NET_PING: Someone is pinging me to get a response time measure (will only
	happen after I've joined a game).  Do nothing; the IPX Manager will handle
	sending an ACK, and updating the response time measurements.
	------------------------------------------------------------------------*/
	else if (GPacket.Command==NET_PING) {
		retcode = EV_NONE;
	}

	/*------------------------------------------------------------------------
	Default case: nothing happened.  (This case will be hit every time I
	receive my own NET_QUERY_GAME or NET_QUERY_PLAYER packets.)
	------------------------------------------------------------------------*/
	else {
		retcode = EV_NONE;
	}

	return(retcode);
}


/***********************************************************************************************
 * Net_New_Dialog -- lets user start a new game																  *
 *                                                                         						  *
 * This dialog shows a list of who's requesting to join this game, and lets						  *
 * the game initiator selectively approve each user.														  *
 *                                                                         						  *
 *    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                       						  *
 *    ³              New Network Game              ³                       						  *
 *    ³                                            ³                       						  *
 *    ³     Players               Scenario         ³   													  *
 *    ³ ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿ ³ 														  *
 *    ³ ³ Boffo       ³³   ³ Hell's Kitchen   ³³ ³ 														  *
 *    ³ ³ Bozo        ÃÄ´   ³ Heaven's Gate    ÃÄ´ ³														  *
 *    ³ ³ Bonzo       ³ ³   ³      ...         ³ ³ ³														  *
 *    ³ ³             ÃÄ´   ³                  ÃÄ´ ³ 														  *
 *    ³ ³             ³³   ³                  ³³ ³ 														  *
 *    ³ ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ ³ 														  *
 *    ³     [Reject]             Count:--- ##      ³														  *
 *    ³                          Level:--- ##      ³                       						  *
 *    ³                                            ³                       						  *
 *    ³               Credits: _____               ³                             				  *
 *    ³       [  Bases   ]   [   Crates   ]        ³                          					  *
 *    ³       [ Tiberium ]   [ AI Players ]        ³                           					  *
 *    ³                                            ³                       						  *
 *    ³              [OK]    [Cancel]              ³                       						  *
 *    ³  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿   ³                       						  *
 *    ³  ³                                     ³   ³                       						  *
 *    ³  ³                                     ³   ³                       						  *
 *    ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ   ³                       						  *
 *    ³               [Send Message]               ³                       						  *
 *    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                       						  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		none.																												  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		true = success, false = cancel																			  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		MPlayerName & MPlayerGameName must contain this player's name.									  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.                                              						  *
 *=============================================================================================*/
static int Net_New_Dialog(void)
{
/* ###Change collision detected! C:\PROJECTS\CODE\NETDLG.CPP... */
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
		//D_DIALOG_W = 281;											// dialog width
	int d_dialog_w = 287*factor;											// dialog width
	int d_dialog_h = 177*factor;											// dialog height
	int d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*factor - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 6*factor+1;												// ht of 6-pt text
	int d_margin1 = 5*factor;												// margin width/height
	int d_margin2 = 2*factor;												// margin width/height

	//d_playerlist_w = 100;
	int d_playerlist_w = 106*factor;
	int d_playerlist_h = 27*factor;
	int d_playerlist_x = d_dialog_x + d_margin1;
	int d_playerlist_y = d_dialog_y + d_margin1 + (d_txt6_h * 3);

	int d_scenariolist_w = 162*factor;
	int d_scenariolist_h = 27*factor;
	int d_scenariolist_x = d_dialog_x + d_dialog_w - d_margin1 - d_scenariolist_w;
	int d_scenariolist_y = d_dialog_y + d_margin1 + (d_txt6_h * 3);

#if (GERMAN | FRENCH)
	int d_reject_w = 55*factor;
#else
	int d_reject_w = 45*factor;
#endif
	int d_reject_h = 9*factor;
	int d_reject_x = d_playerlist_x + (d_playerlist_w / 2) - (d_reject_w / 2);
	int d_reject_y = d_playerlist_y + d_playerlist_h + d_margin2;

	int d_count_w = 25*factor;
	int d_count_h = d_txt6_h;
	int d_count_x = d_scenariolist_x + (d_scenariolist_w / 2);
	int d_count_y = d_scenariolist_y + d_scenariolist_h + d_margin2;

	int d_level_w = 25*factor;
	int d_level_h = d_txt6_h;
	int d_level_x = d_scenariolist_x + (d_scenariolist_w / 2);
	int d_level_y = d_count_y + d_count_h;

	int d_credits_w = ((CREDITSBUF_MAX - 1) * 7*factor) + 4*factor;
	//int d_credits_w = ((CREDITSBUF_MAX - 1) * 6*factor) + 3*factor;
	int d_credits_h = 9*factor;
	int d_credits_x = d_dialog_cx + 2*factor;
	int d_credits_y = d_level_y + d_level_h + d_margin1;

#if (GERMAN | FRENCH)
	int d_bases_w = 120*factor;//bga:100;
#else
	int d_bases_w = 100*factor;
#endif
	int d_bases_h = 9*factor;
	int d_bases_x = d_dialog_cx - d_bases_w - d_margin2;
	int d_bases_y = d_credits_y + d_credits_h + d_margin2;

#if (GERMAN | FRENCH)
	int d_tiberium_w = 120*factor;
#else
	int d_tiberium_w = 100*factor;
#endif
	int d_tiberium_h = 9*factor;
	int d_tiberium_x = d_dialog_cx - d_bases_w - d_margin2;
	int d_tiberium_y = d_bases_y + d_bases_h + d_margin2;

#if (GERMAN | FRENCH)
	int d_goodies_w = 120*factor;
#else
	int d_goodies_w = 100*factor;
#endif
	int d_goodies_h = 9*factor;
	int d_goodies_x = d_dialog_cx + d_margin2;
	int d_goodies_y = d_credits_y + d_credits_h + d_margin2;

#if (GERMAN | FRENCH)
	int d_ghosts_w = 120*factor;
#else
	int d_ghosts_w = 100*factor;
#endif
	int d_ghosts_h = 9*factor;
	int d_ghosts_x = d_dialog_cx + d_margin2;
	int d_ghosts_y = d_goodies_y + d_goodies_h + d_margin2;

	int d_ok_w = 45*factor;
	int d_ok_h = 9*factor;
	int d_ok_x = d_dialog_cx - d_margin2 - (d_bases_w / 2) - (d_ok_w / 2);
	int d_ok_y = d_ghosts_y + d_ghosts_h + d_margin1;

#if (GERMAN | FRENCH)
	int d_cancel_w = 50*factor;
#else
	int d_cancel_w = 45*factor;
#endif
	int d_cancel_h = 9*factor;
	int d_cancel_x = d_dialog_cx + d_margin2 + (d_goodies_w / 2) - (d_cancel_w / 2);
	int d_cancel_y = d_ghosts_y + d_ghosts_h + d_margin1;

	int d_message_w = d_dialog_w - (d_margin1 * 2);
	int d_message_h = 34*factor;
	int d_message_x = d_dialog_x + d_margin1;
	int d_message_y = d_cancel_y + d_cancel_h + d_margin1;

	int d_send_w = 80*factor;
	int d_send_h = 9*factor;
	int d_send_x = d_dialog_cx - (d_send_w / 2);
	int d_send_y = d_message_y + d_message_h + d_margin2;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_PLAYERLIST = 100,
		BUTTON_SCENARIOLIST,
		BUTTON_REJECT,
		BUTTON_COUNT,
		BUTTON_LEVEL,
		BUTTON_CREDITS,
		BUTTON_BASES,
		BUTTON_TIBERIUM,
		BUTTON_GOODIES,
		BUTTON_GHOSTS,
		BUTTON_OK,
		BUTTON_CANCEL,
		BUTTON_SEND,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_UNIT_COUNT,
		REDRAW_MESSAGE,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display = REDRAW_ALL;		// redraw level
	bool process = true;						// process while true
	KeyNumType input;

	char credbuf[CREDITSBUF_MAX];			// for credit edit box
	int old_cred;								// old value in credits buffer
	int transmit;								// 1 = re-transmit new game options

	long ok_timer = 0;						// for timing OK button
	int index;									// index for rejecting a player
	int rc;
	int i,j;
	char *item;
	int tabs[] = {77*factor};						// tabs for player list box

	long ping_timer = 0;						// for sending Ping packets

	unsigned char tmp_id[MAX_PLAYERS];	// temp storage for sorting player ID's
	int min_index;								// for sorting player ID's
	unsigned char min_id;					// for sorting player ID's
	unsigned char id;							// connection ID
	char txt[80];
	JoinEventType whahoppa;					// event generated by received packets
	static int first_time = 1;				// 1 = 1st time this dialog is run

	int					message_length;
	int					sent_so_far;
	unsigned short		magic_number;
	unsigned short		crc;

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	void const *up_button;
	void const *down_button;

	if (InMainLoop){
		up_button = Hires_Retrieve("BTN-UP.SHP");
		down_button = Hires_Retrieve("BTN-DN.SHP");
	}else{
		up_button = Hires_Retrieve("BTN-UP2.SHP");
		down_button = Hires_Retrieve("BTN-DN2.SHP");
	}


	ColorListClass playerlist(BUTTON_PLAYERLIST,
		d_playerlist_x, d_playerlist_y, d_playerlist_w, d_playerlist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	ListClass scenariolist(BUTTON_SCENARIOLIST,
		d_scenariolist_x, d_scenariolist_y, d_scenariolist_w, d_scenariolist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	EditClass credit_edt (BUTTON_CREDITS,
		credbuf, CREDITSBUF_MAX,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_credits_x, d_credits_y, d_credits_w, d_credits_h, EditClass::ALPHANUMERIC);

	TextButtonClass rejectbtn(BUTTON_REJECT, TXT_REJECT,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_reject_x, d_reject_y);
//#else
		d_reject_x, d_reject_y, d_reject_w, d_reject_h);
//#endif

	GaugeClass countgauge (BUTTON_COUNT,
		d_count_x, d_count_y, d_count_w, d_count_h);

	GaugeClass levelgauge (BUTTON_LEVEL,
		d_level_x, d_level_y, d_level_w, d_level_h);

	TextButtonClass basesbtn(BUTTON_BASES, TXT_BASES_OFF,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_bases_x, d_bases_y, d_bases_w, d_bases_h);

	TextButtonClass tiberiumbtn(BUTTON_TIBERIUM, TXT_TIBERIUM_OFF,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_tiberium_x, d_tiberium_y, d_tiberium_w, d_tiberium_h);

	TextButtonClass goodiesbtn(BUTTON_GOODIES, TXT_CRATES_OFF,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_goodies_x, d_goodies_y, d_goodies_w, d_goodies_h);

	TextButtonClass ghostsbtn(BUTTON_GHOSTS, TXT_AI_PLAYERS_OFF,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_ghosts_x, d_ghosts_y, d_ghosts_w, d_ghosts_h);

	TextButtonClass okbtn(BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_ok_x, d_ok_y, d_ok_w, d_ok_h);

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_cancel_x, d_cancel_y);
//#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
//#endif

	TextButtonClass sendbtn(BUTTON_SEND, TXT_SEND_MESSAGE,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_send_x, d_send_y);
//#else
		d_send_x, d_send_y, d_send_w, d_send_h);
//#endif

	/*
	------------------------- Build the button list --------------------------
	*/
	commands = &playerlist;
	scenariolist.Add_Tail(*commands);
	credit_edt.Add_Tail(*commands);
	rejectbtn.Add_Tail(*commands);
	countgauge.Add_Tail(*commands);
	levelgauge.Add_Tail(*commands);
	basesbtn.Add_Tail(*commands);
	tiberiumbtn.Add_Tail(*commands);
	goodiesbtn.Add_Tail(*commands);
	ghostsbtn.Add_Tail(*commands);
	okbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);
	sendbtn.Add_Tail(*commands);

	playerlist.Set_Tabs(tabs);

	/*
	----------------------------- Various Inits ------------------------------
	*/
	/*........................................................................
	Init dialog values, only the first time through
	........................................................................*/
	if (first_time) {
		MPlayerCredits = 3000;						// init credits & credit buffer
		MPlayerBases = 1;								// init scenario parameters
		MPlayerTiberium = 0;
		MPlayerGoodies = 0;
		MPlayerGhosts = 0;
		Special.IsCaptureTheFlag = 0;
		MPlayerUnitCount = (MPlayerCountMax[MPlayerBases] + MPlayerCountMin[MPlayerBases]) / 2;
		first_time = 0;
	}

	/*........................................................................
	Init button states
	........................................................................*/
	if (MPlayerBases) {
		basesbtn.Turn_On();
		basesbtn.Set_Text(TXT_BASES_ON);
	}
	if (MPlayerTiberium) {
		tiberiumbtn.Turn_On();
		tiberiumbtn.Set_Text(TXT_TIBERIUM_ON);
	}
	if (MPlayerGoodies) {
		goodiesbtn.Turn_On();
		goodiesbtn.Set_Text(TXT_CRATES_ON);
	}
	if (MPlayerGhosts) {
		ghostsbtn.Turn_On();
		ghostsbtn.Set_Text(TXT_AI_PLAYERS_ON);
	}
	if (Special.IsCaptureTheFlag) {
		MPlayerGhosts = 0;
		ghostsbtn.Turn_On();
		ghostsbtn.Set_Text(TXT_CAPTURE_THE_FLAG);
	}

	sprintf(credbuf, "%d", MPlayerCredits);
	credit_edt.Set_Text(credbuf, CREDITSBUF_MAX);
	old_cred = MPlayerCredits;

	levelgauge.Set_Maximum(MPLAYER_BUILD_LEVEL_MAX - 1);
	levelgauge.Set_Value(BuildLevel - 1);

	countgauge.Set_Maximum(MPlayerCountMax[MPlayerBases] - MPlayerCountMin[MPlayerBases]);
	countgauge.Set_Value(MPlayerUnitCount - MPlayerCountMin[MPlayerBases]);

	/*........................................................................
	Init other scenario parameters
	........................................................................*/
	Special.IsTGrowth = MPlayerTiberium;
	Special.IsTSpread = MPlayerTiberium;
	transmit = 0;

	/*........................................................................
	Init scenario description list box
	........................................................................*/
	for (i = 0; i < MPlayerScenarios.Count(); i++) {
		scenariolist.Add_Item (strupr(MPlayerScenarios[i]));
	}
	ScenarioIdx = 0;								// 1st scenario is selected

	/*........................................................................
	Init player color-used flags
	........................................................................*/
	for (i = 0; i < MAX_MPLAYER_COLORS; i++) {
		ColorUsed[i] = 0;							// init all colors to available
	}
	ColorUsed[MPlayerColorIdx] = 1;			// set my color to used
	playerlist.Set_Selected_Style(ColorListClass::SELECT_BAR, CC_GREEN_SHADOW);

	/*........................................................................
	Init random-number generator, & create a seed to be used for all random
	numbers from here on out
	........................................................................*/
	//ST - 12/18/2018 11:37AM
	//randomize();
	//Seed = rand();

	/*........................................................................
	Init the message display system
	........................................................................*/
	Messages.Init (d_message_x + 2*factor, d_message_y + 2*factor, 4, MAX_MESSAGE_LENGTH,
		d_txt6_h);

	/*------------------------------------------------------------------------
	Add myself to the list.  Note that since I'm not in the Players Vector,
	the Vector & listbox are now 1 out of sync.
	------------------------------------------------------------------------*/
	item = new char [MPLAYER_NAME_MAX + 4];
	if (MPlayerHouse==HOUSE_GOOD) {
		sprintf(item,"%s\t%s",MPlayerName,Text_String(TXT_G_D_I));
	} else {
		sprintf(item,"%s\t%s",MPlayerName,Text_String(TXT_N_O_D));
	}
	playerlist.Add_Item(item, MPlayerTColors[MPlayerColorIdx]);

	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Set_Palette(Palette);
	while (Get_Mouse_State() > 0) Show_Mouse();

	/*
	---------------------------- Processing loop -----------------------------
	*/
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		#if(SHOW_MONO)
		Ipx.Mono_Debug_Print(-1,0);
		#endif
		/*
		...................... Refresh display if needed ......................
		*/
		if (display == REDRAW_UNIT_COUNT){
			/*
			** Wipe the background behind the unit count then reprint it
			*/
			LogicPage->Fill_Rect(d_count_x + d_count_w + 2*factor,
										d_count_y,
										d_count_x + d_count_w + 2*factor + 20,
										d_count_y + 12,
										0 );
			sprintf(txt,"%d",MPlayerUnitCount);
			Fancy_Text_Print (txt, d_count_x + d_count_w + 2*factor, d_count_y,
									CC_GREEN, TBLACK,
									TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL);
			display = REDRAW_NONE;
		}


		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {

				/*
				** Reload and draw the title page
				*/
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
				Set_Palette(Palette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Draw_Caption (TXT_NETGAME_SETUP, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print(TXT_PLAYERS,
					d_playerlist_x + (d_playerlist_w / 2), d_playerlist_y - d_txt6_h,
					CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_CENTER);

				Fancy_Text_Print(TXT_SCENARIOS,
					d_scenariolist_x + (d_scenariolist_w / 2),
					d_scenariolist_y - d_txt6_h,
					CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_CENTER);

				Fancy_Text_Print (TXT_COUNT, d_count_x - 2*factor, d_count_y,
					CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_RIGHT);

				sprintf(txt,"%d",MPlayerUnitCount);
				Fancy_Text_Print (txt, d_count_x + d_count_w + 2*factor, d_count_y,
					CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL);

				Fancy_Text_Print (TXT_LEVEL, d_level_x - 2*factor, d_level_y,
					CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_RIGHT);

				if (BuildLevel <= MPLAYER_BUILD_LEVEL_MAX) {
					sprintf(txt,"%d",BuildLevel);
				} else {
					sprintf(txt, "**");
				}
				Fancy_Text_Print (txt, d_level_x + d_level_w + 2*factor, d_level_y,
					CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL);

				Fancy_Text_Print (TXT_START_CREDITS_COLON, d_credits_x - 5*factor,
					d_credits_y + 1*factor, CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_RIGHT);

			}

			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Draw_All();
			}

			/*..................................................................
			Draw the messages:
			- Erase an old message first
			- If we're in a game, print the game options (if they've been
			  received)
			- If we've been rejected from a game, print that message
			..................................................................*/
			if (display >= REDRAW_MESSAGE) {
				Draw_Box(d_message_x, d_message_y, d_message_w, d_message_h,
					BOXSTYLE_GREEN_BORDER, true);
				Messages.Draw();
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {
			/*------------------------------------------------------------------
			New Scenario selected.
			------------------------------------------------------------------*/
			case (BUTTON_SCENARIOLIST | KN_BUTTON):
				if (scenariolist.Current_Index() != ScenarioIdx) {
					ScenarioIdx = scenariolist.Current_Index();
					MPlayerCredits = atoi(credbuf);
					transmit = 1;
				}
				break;

			/*------------------------------------------------------------------
			Reject the currently-selected player (don't allow rejecting myself,
			who will be the first entry in the list)
			------------------------------------------------------------------*/
			case (BUTTON_REJECT | KN_BUTTON):
				index = playerlist.Current_Index();
				if (index == 0) {
					CCMessageBox().Process (TXT_CANT_REJECT_SELF, TXT_OOPS);
					display = REDRAW_ALL;
					break;
				} else {
					if (index < 0 || index >= playerlist.Count()) {
						CCMessageBox().Process (TXT_SELECT_PLAYER_REJECT,TXT_OOPS);
						display = REDRAW_ALL;
						break;
					}
				}
				memset (&GPacket, 0, sizeof(GlobalPacketType));

				GPacket.Command = NET_REJECT_JOIN;

				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType), 1,
					&(Players[index - 1]->Address));
				break;

			/*------------------------------------------------------------------
			User adjusts max # units
			------------------------------------------------------------------*/
			case (BUTTON_COUNT | KN_BUTTON):
				MPlayerUnitCount = countgauge.Get_Value() + MPlayerCountMin[MPlayerBases];

				Hide_Mouse();
				LogicPage->Fill_Rect (d_count_x + d_count_w + 2*factor, d_count_y,
					d_count_x + d_count_w + 14*factor, d_count_y + 6*factor, BLACK);

				sprintf(txt,"%d",MPlayerUnitCount);
				Fancy_Text_Print (txt, d_count_x + d_count_w + 2*factor, d_count_y,
					CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL);
				Show_Mouse();

				transmit = 1;
				break;

			/*------------------------------------------------------------------
			User adjusts build level
			------------------------------------------------------------------*/
			case (BUTTON_LEVEL | KN_BUTTON):
				BuildLevel = levelgauge.Get_Value() + 1;
				if (BuildLevel > MPLAYER_BUILD_LEVEL_MAX)	// if it's pegged, max it out
					BuildLevel = MPLAYER_BUILD_LEVEL_MAX;

				Hide_Mouse();
				LogicPage->Fill_Rect (d_level_x + d_level_w + 2*factor, d_level_y,
					d_level_x + d_level_w + 14*factor, d_level_y + 6*factor, BLACK);

				if (BuildLevel <= MPLAYER_BUILD_LEVEL_MAX) {
					sprintf(txt,"%d",BuildLevel);
				} else {
					sprintf(txt, "**");
				}
				Fancy_Text_Print (txt, d_level_x + d_level_w + 2*factor, d_level_y,
					CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL);
				Show_Mouse();

				transmit = 1;
				break;

			/*------------------------------------------------------------------
			User edits the credits value; retransmit new game options
			------------------------------------------------------------------*/
			case (BUTTON_CREDITS | KN_BUTTON):
				MPlayerCredits = atoi(credbuf);
				transmit = 1;
				break;

			/*------------------------------------------------------------------
			Toggle bases:
			- Clear scenario list & rebuild it with new names
			- toggle bases button, change its text
			- adjust the MPlayerUnitCount to reflect the new allowed range,
			  using the current gauge setting
			- Change the unit count gauge limit & value
			------------------------------------------------------------------*/
			case (BUTTON_BASES | KN_BUTTON):
				if (MPlayerBases) {
					MPlayerBases = 0;
					basesbtn.Turn_Off();
					basesbtn.Set_Text(TXT_BASES_OFF);
					MPlayerUnitCount = Fixed_To_Cardinal (MPlayerCountMax[0]-MPlayerCountMin[0],
						Cardinal_To_Fixed(MPlayerCountMax[1]-MPlayerCountMin[1],
						MPlayerUnitCount-MPlayerCountMin[1])) + MPlayerCountMin[0];
				} else {
					MPlayerBases = 1;
					basesbtn.Turn_On();
					basesbtn.Set_Text(TXT_BASES_ON);
					MPlayerUnitCount = Fixed_To_Cardinal (MPlayerCountMax[1]-MPlayerCountMin[1],
						Cardinal_To_Fixed(MPlayerCountMax[0]-MPlayerCountMin[0],
						MPlayerUnitCount-MPlayerCountMin[0])) + MPlayerCountMin[1];
				}
				MPlayerCredits = atoi(credbuf);
				countgauge.Set_Maximum(MPlayerCountMax[MPlayerBases] - MPlayerCountMin[MPlayerBases]);
				countgauge.Set_Value(MPlayerUnitCount - MPlayerCountMin[MPlayerBases]);
				transmit = 1;
				countgauge.Flag_To_Redraw();
				display = REDRAW_UNIT_COUNT;
				break;

			/*------------------------------------------------------------------
			Toggle tiberium
			------------------------------------------------------------------*/
			case (BUTTON_TIBERIUM | KN_BUTTON):
				if (MPlayerTiberium) {
					MPlayerTiberium = 0;
					Special.IsTGrowth = 0;
					Special.IsTSpread = 0;
					tiberiumbtn.Turn_Off();
					tiberiumbtn.Set_Text(TXT_TIBERIUM_OFF);
				} else {
					MPlayerTiberium = 1;
					Special.IsTGrowth = 1;
					Special.IsTSpread = 1;
					tiberiumbtn.Turn_On();
					tiberiumbtn.Set_Text(TXT_TIBERIUM_ON);
				}
				MPlayerCredits = atoi(credbuf);
				transmit = 1;
				break;

			/*------------------------------------------------------------------
			Toggle goodies
			------------------------------------------------------------------*/
			case (BUTTON_GOODIES | KN_BUTTON):
				if (MPlayerGoodies) {
					MPlayerGoodies = 0;
					goodiesbtn.Turn_Off();
					goodiesbtn.Set_Text(TXT_CRATES_OFF);
				} else {
					MPlayerGoodies = 1;
					goodiesbtn.Turn_On();
					goodiesbtn.Set_Text(TXT_CRATES_ON);
				}
				MPlayerCredits = atoi(credbuf);
				transmit = 1;
				break;

			/*------------------------------------------------------------------
			Toggle ghosts/capture-the-flag
			------------------------------------------------------------------*/
			case (BUTTON_GHOSTS | KN_BUTTON):
				if (!MPlayerGhosts && !Special.IsCaptureTheFlag) {	// ghosts OFF => ghosts ON
					MPlayerGhosts = 1;
					Special.IsCaptureTheFlag = 0;
					ghostsbtn.Turn_On();
					ghostsbtn.Set_Text(TXT_AI_PLAYERS_ON);
				} else {
					if (MPlayerGhosts) {					// ghosts ON => capture-flag
						MPlayerGhosts = 0;
						Special.IsCaptureTheFlag = 1;
						ghostsbtn.Turn_On();
						ghostsbtn.Set_Text(TXT_CAPTURE_THE_FLAG);
					} else {
						if (Special.IsCaptureTheFlag) {		// capture-flag => AI OFF
							MPlayerGhosts = 0;
							Special.IsCaptureTheFlag = 0;
							ghostsbtn.Turn_Off();
							ghostsbtn.Set_Text(TXT_AI_PLAYERS_OFF);
						}
					}
				}

				MPlayerCredits = atoi(credbuf);
				transmit = 1;
				break;

			/*------------------------------------------------------------------
			OK: exit loop with TRUE status
			------------------------------------------------------------------*/
			case (BUTTON_OK | KN_BUTTON):
				/*...............................................................
				If a new player has joined in the last second, don't allow
				an OK; force a wait longer than 1 second (to give all players
				a chance to know about this new guy)
				...............................................................*/
				i = MAX(Ipx.Global_Response_Time() * 2, (unsigned long)60);
				while (TickCount.Time() - ok_timer < i)
					Ipx.Service();

				/*...............................................................
				If there are at least 2 players, go ahead & play; error otherwise
				...............................................................*/
				if (MPlayerSolo || Players.Count() > 0) {
					rc = TRUE;
					process = FALSE;
				} else {
					CCMessageBox().Process (TXT_ONLY_ONE,TXT_OOPS,NULL);
					display = REDRAW_ALL;
				}
				break;

			/*------------------------------------------------------------------
			CANCEL: send a SIGN_OFF, bail out with error code
			------------------------------------------------------------------*/
			case (KN_ESC):
				if (Messages.Get_Edit_Buf() != NULL) {
					Messages.Input(input);
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
					break;
				}
			case (BUTTON_CANCEL | KN_BUTTON):
				memset (&GPacket, 0, sizeof(GlobalPacketType));

				GPacket.Command = NET_SIGN_OFF;
				strcpy (GPacket.Name, MPlayerName);

				/*...............................................................
				Broadcast my sign-off over my network
				...............................................................*/
				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					0, NULL);
				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					0, NULL);
				while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;

				/*...............................................................
				Broadcast my sign-off over a bridged network if there is one
				...............................................................*/
				if (IsBridge) {
					Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
						&BridgeNet);
					Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
						&BridgeNet);
				}
				while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;

				/*...............................................................
				And now, just be absolutely sure, send my sign-off to each
				player in my game.  (If there's a bridge between us, the other
				player will have specified my address, so he can cross the
				bridge; but I may not have specified a bridge address, so the
				only way I have of crossing the bridge is to send a packet
				directly to him.)
				...............................................................*/
				for (i = 0; i < Players.Count(); i++) {
					Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 1,
						&(Players[i]->Address));
					Ipx.Service();
				}
				while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;
				MPlayerGameName[0] = 0;
				process = false;
				rc = false;
				break;

			/*------------------------------------------------------------------
			Default: manage the inter-player messages
			------------------------------------------------------------------*/
			default:
				/*...............................................................
				F4/SEND/'M' = send a message
				...............................................................*/
				if (Messages.Get_Edit_Buf()==NULL) {
					if (input == KN_M || input==(BUTTON_SEND | KN_BUTTON) ||
						input == KN_F4) {
						memset (txt, 0, 80);

						strcpy(txt,Text_String(TXT_TO_ALL));	// "To All:"

						Messages.Add_Edit (MPlayerTColors[MPlayerColorIdx],
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, d_message_w-70*factor);

						credit_edt.Clear_Focus();
						credit_edt.Flag_To_Redraw();
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;

						break;
					}
				} else {

					/*...............................................................
					If we're already editing a message and the user clicks on
					'Send', translate our input to a Return so Messages.Input() will
					work properly.
					...............................................................*/
					if (input==(BUTTON_SEND | KN_BUTTON)) {
						input = KN_RETURN;
					}
				}

				/*...............................................................
				Manage the message system (get rid of old messages)
				...............................................................*/
				if (Messages.Manage()) {
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
				}

				/*...............................................................
				Re-draw the messages & service keyboard input for any message
				being edited.
				...............................................................*/
				i = Messages.Input(input);

				/*...............................................................
				If 'Input' returned 1, it means refresh the message display.
				...............................................................*/
				if (i==1) {
					Messages.Draw();
				}

				/*...............................................................
				If 'Input' returned 2, it means redraw the message display.
				...............................................................*/
				else if (i==2) {
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
				}

				/*...............................................................
				If 'input' returned 3, it means send the current message.
				...............................................................*/
				else if (i==3) {
					long actual_message_size;
					char *the_string;

					sent_so_far = 0;
 					magic_number = MESSAGE_HEAD_MAGIC_NUMBER;
					message_length = strlen(Messages.Get_Edit_Buf());
					crc = (unsigned short)
							(Calculate_CRC(Messages.Get_Edit_Buf(), message_length) &0xffff);
					while ( sent_so_far < message_length ){
						memset (&GPacket, 0, sizeof(GlobalPacketType));
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
								the_string[i + actual_message_size] = 0;	//0xff; ST - 12/18/2018 11:37AM
							}
						}else{
							actual_message_size = COMPAT_MESSAGE_LENGTH - 5;
						}

						*(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-5) = 0;
						*((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-4)) = magic_number;
						*((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-2)) = crc;
						GPacket.Message.ID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);
						GPacket.Message.NameCRC = Compute_Name_CRC(MPlayerGameName);

						/*..................................................................
						Send the message to every player in our player list.
						..................................................................*/
						for (i = 0; i < Players.Count(); i++) {
							Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 1,
								&(Players[i]->Address));
							Ipx.Service();
						}
						/*..................................................................
						Add the message to our own list, since we're not in the player list
						on this dialog.
						..................................................................*/
						sprintf(txt,Text_String (TXT_FROM), MPlayerName, GPacket.Message.Buf);
						Messages.Add_Message (txt, MPlayerTColors[MPlayerColorIdx],
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 1200, magic_number, crc);

						magic_number++;
						sent_so_far += actual_message_size;	//COMPAT_MESSAGE_LENGTH-5;

					}
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
				}
		}

		/*---------------------------------------------------------------------
		Detect editing of the credits buffer, transmit new values to players
		---------------------------------------------------------------------*/
		if (atoi(credbuf) != old_cred) {
			old_cred = Bound(atoi(credbuf), 0, 9999);
			MPlayerCredits = old_cred;
			transmit = 1;
			sprintf(credbuf, "%d", MPlayerCredits);
			credit_edt.Set_Text(credbuf, CREDITSBUF_MAX);
		}

		/*---------------------------------------------------------------------
		Process incoming packets
		---------------------------------------------------------------------*/
		whahoppa = Get_NewGame_Responses(&playerlist);
		if (whahoppa == EV_NEW_PLAYER) {
			ok_timer = TickCount.Time();
			transmit = 1;
		} else {
			if (whahoppa == EV_MESSAGE) {
				if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
			}
		}

		/*---------------------------------------------------------------------
		If our Transmit flag is set, we need to send out a game option packet
		---------------------------------------------------------------------*/
		if (transmit) {
			for (i = 0; i < Players.Count(); i++) {
				memset (&GPacket, 0, sizeof(GlobalPacketType));

				GPacket.Command = NET_GAME_OPTIONS;
				GPacket.ScenarioInfo.Scenario = MPlayerFilenum[ScenarioIdx];
				GPacket.ScenarioInfo.Credits = MPlayerCredits;
				GPacket.ScenarioInfo.IsBases = MPlayerBases;
				GPacket.ScenarioInfo.IsTiberium = MPlayerTiberium;
				GPacket.ScenarioInfo.IsGoodies = MPlayerGoodies;
				GPacket.ScenarioInfo.IsGhosties = MPlayerGhosts;
				GPacket.ScenarioInfo.BuildLevel = BuildLevel;
				GPacket.ScenarioInfo.UnitCount = MPlayerUnitCount;
				GPacket.ScenarioInfo.Seed = Seed;
				GPacket.ScenarioInfo.Special = Special;
				GPacket.ScenarioInfo.GameSpeed = Options.GameSpeed;

				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					1, &(Players[i]->Address) );
			}
			transmit = 0;
		}

		/*---------------------------------------------------------------------
		Ping every player in my game, to force the Global Channel to measure
		the connection response time.
		---------------------------------------------------------------------*/
		if (TickCount.Time() - ping_timer > 15) {
			memset (&GPacket, 0, sizeof(GlobalPacketType));
			GPacket.Command = NET_PING;
			for (i = 0; i < Players.Count(); i++) {
				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					1, &(Players[i]->Address) );
			}
			ping_timer = TickCount.Time();
		}

		/*---------------------------------------------------------------------
		Service the Ipx connections
		---------------------------------------------------------------------*/
		Ipx.Service();

		/*---------------------------------------------------------------------
		Service the sounds & score; GameActive must be false at this point,
		so Call_Back() doesn't intercept global messages from me!
		---------------------------------------------------------------------*/
		Call_Back();

	}	/* end of while */

	/*------------------------------------------------------------------------
	Establish connections with all other players.
	------------------------------------------------------------------------*/
	if (rc) {
		/*.....................................................................
		Set the number of players in this game, and my ID
		.....................................................................*/
		MPlayerCount = Players.Count() + 1;
		MPlayerLocalID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);

		/*.....................................................................
		Get the scenario filename
		.....................................................................*/
		Scenario = MPlayerFilenum[ScenarioIdx];

		/*.....................................................................
		Compute frame delay value for packet transmissions:
		- Divide global channel's response time by 8 (2 to convert to 1-way
		  value, 4 more to convert from ticks to frames)
		.....................................................................*/
		MPlayerMaxAhead = MAX( (Ipx.Global_Response_Time() / 8), (unsigned long)2);

		/*.....................................................................
		Send all players the NET_GO packet.  Wait until all ACK's have been
		received.
		.....................................................................*/
		memset (&GPacket, 0, sizeof(GlobalPacketType));
		GPacket.Command = NET_GO;
		GPacket.ResponseTime.OneWay = MPlayerMaxAhead;
		for (i = 0; i < Players.Count(); i++) {
			Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
				1, &(Players[i]->Address) );
			/*..................................................................
			Wait for all the ACK's to come in.
			..................................................................*/
			while (Ipx.Global_Num_Send() > 0)
				Ipx.Service();
		}

		/*.....................................................................
		Form connections with all other players.  Form the IPX Connection ID
		from the player's Color (high byte) and House (low byte).  This
		will let us extract any player's color & house at any time.
		Fill in 'tmp_id' while we're doing this.
		.....................................................................*/
		for (i = 0; i < Players.Count(); i++) {
			id = Build_MPlayerID (Players[i]->Player.Color,
				Players[i]->Player.House);

			tmp_id[i] = id;

			Ipx.Create_Connection(id, Players[i]->Name, &(Players[i]->Address) );
		}
		tmp_id[i] = MPlayerLocalID;

		/*.....................................................................
		Store every player's ID in the MPlayerID[] array.  This array will
		determine the order of event execution, so the ID's must be stored
		in the same order on all systems.
		.....................................................................*/
		for (i = 0; i < MPlayerCount; i++) {
			min_index = 0;
			min_id = 0xff;
			for (j = 0; j < MPlayerCount; j++) {
				if (tmp_id[j] < min_id) {
					min_id = tmp_id[j];
					min_index = j;
				}
			}
			MPlayerID[i] = tmp_id[min_index];
			tmp_id[min_index] = 0xff;
		}
		/*.....................................................................
		Fill in the array of player names, including my own.
		.....................................................................*/
		for (i = 0; i < MPlayerCount; i++) {
			if (MPlayerID[i] == MPlayerLocalID) {
				strcpy (MPlayerNames[i], MPlayerName);
			} else {
				strcpy (MPlayerNames[i], Ipx.Connection_Name(MPlayerID[i]));
			}
		}
	}

	/*------------------------------------------------------------------------
	Init network timing values, using previous response times as a measure
	of what our retry delta & timeout should be.
	------------------------------------------------------------------------*/
	Ipx.Set_Timing (Ipx.Global_Response_Time() + 2, -1,
		Ipx.Global_Response_Time() * 4);

	/*------------------------------------------------------------------------
	Clear all lists
	------------------------------------------------------------------------*/
	while (scenariolist.Count()) {
		scenariolist.Remove_Item(scenariolist.Get_Item(0));
	}
	Clear_Player_List(&playerlist);

	/*------------------------------------------------------------------------
	Restore screen
	------------------------------------------------------------------------*/
	Hide_Mouse();
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Show_Mouse();

	return(rc);
}


/***************************************************************************
 * Get_NewGame_Responses -- processes packets for New Game dialog          *
 *                                                                         *
 * This routine can modify the contents of the given list box, as well		*
 * as the contents of the Players Vector global.									*
 *                                                                         *
 * INPUT:                                                                  *
 *		playerlist		list of players in this game									*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		EV_NONE = nothing happened															*
 *		EV_NEW_PLAYER = a new player has joined; false otherwise					*
 *		EV_MESSAGE = a message was received												*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/18/1995 BRR : Created.                                             *
 *=========================================================================*/
static JoinEventType Get_NewGame_Responses(ColorListClass *playerlist)
{
	int rc;
	char * item;						// general-purpose string
	NodeNameType *who;				// node to add to Players Vector
	int i;
	int found;
	JoinEventType retval = EV_NONE;
	int resend;
	char txt[80];
	int color;
	unsigned short magic_number;
	unsigned short crc;

	/*------------------------------------------------------------------------
	If there is no incoming packet, just return
	------------------------------------------------------------------------*/
	rc = Ipx.Get_Global_Message (&GPacket, &GPacketlen, &GAddress, &GProductID);
	if (!rc || GProductID != IPXGlobalConnClass::COMMAND_AND_CONQUER) {
		return(EV_NONE);
	}

	/*------------------------------------------------------------------------
	Try to handle the packet in a standard way
	------------------------------------------------------------------------*/
	if (Process_Global_Packet(&GPacket,&GAddress) != 0) {
		return(EV_NONE);
	} else

	/*------------------------------------------------------------------------
	NET_QUERY_JOIN:
	------------------------------------------------------------------------*/
	if (GPacket.Command==NET_QUERY_JOIN) {
		/*.....................................................................
		See if this name is unique:
		- If the name matches, but the address is different, reject this player
		- If the name & address match, this packet must be a re-send of a
		  prevous request; in this case, do nothing.  The other player must have
		  received my CONFIRM_JOIN packet (since it was sent with an ACK
		  required), so we can ignore this resend.
		.....................................................................*/
		found = 0;
		resend = 0;
		for (i = 0; i < Players.Count(); i++) {
			if (!strcmp(Players[i]->Name,GPacket.Name)) {
				if (Players[i]->Address != GAddress) {
					found = 1;
				}
				else {
					resend = 1;
				}
				break;
			}
		}
		if (!strcmp (MPlayerName, GPacket.Name)) {
			found = 1;
		}

		/*.....................................................................
		Reject if name is a duplicate, or if there are too many players:
		.....................................................................*/
		if (found || (Players.Count() >= (MPlayerMax - 1) && !resend) ) {
			memset (&GPacket, 0, sizeof(GlobalPacketType));

			GPacket.Command = NET_REJECT_JOIN;

			Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
				1, &GAddress);
		}

		/*.....................................................................
		If this packet is NOT a resend, accept the player.  Grant him the
		requested color if possible.
		.....................................................................*/
		else if (!resend) {
			/*..................................................................
			Add node to the Vector list
			..................................................................*/
			who = new NodeNameType;
			strcpy(who->Name, GPacket.Name);
			who->Address = GAddress;
			who->Player.House = GPacket.PlayerInfo.House;
			Players.Add (who);

			/*..................................................................
			Set player's color; if requested color isn't used, give it to him;
			otherwise, give him the 1st available color.  Mark the color we
			give him as used.
			..................................................................*/
			if (ColorUsed[GPacket.PlayerInfo.Color] == 0) {
				who->Player.Color = GPacket.PlayerInfo.Color;
			} else {
				for (i = 0; i < MAX_MPLAYER_COLORS; i++) {
					if (ColorUsed[i]==0) {
						who->Player.Color = i;
						break;
					}
				}
			}
			ColorUsed[who->Player.Color] = 1;

			/*..................................................................
			Add player name to the list box
			..................................................................*/
			item = new char [MPLAYER_NAME_MAX + 4];
			if (GPacket.PlayerInfo.House==HOUSE_GOOD) {
				sprintf(item,"%s\t%s",GPacket.Name,Text_String(TXT_G_D_I));
			} else {
				sprintf(item,"%s\t%s",GPacket.Name,Text_String(TXT_N_O_D));
			}
			playerlist->Add_Item (item, MPlayerTColors[who->Player.Color]);

			/*..................................................................
			Send a confirmation packet
			..................................................................*/
			memset (&GPacket, 0, sizeof(GlobalPacketType));

			GPacket.Command = NET_CONFIRM_JOIN;
			strcpy(GPacket.Name,MPlayerName);
			GPacket.PlayerInfo.House = who->Player.House;
			GPacket.PlayerInfo.Color = who->Player.Color;

			Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
				1, &GAddress);

			retval = EV_NEW_PLAYER;
		}
	}

	/*------------------------------------------------------------------------
	NET_SIGN_OFF: Another system is signing off: search for that system in
	the player list, & remove it if found
	------------------------------------------------------------------------*/
	else if (GPacket.Command==NET_SIGN_OFF) {
		for (i = 0; i < Players.Count(); i++) {
			/*
			....................... Name found; remove it ......................
			*/
			if (!strcmp (Players[i]->Name, GPacket.Name) &&
				Players[i]->Address==GAddress) {
				/*...............................................................
				Remove from the list box
				...............................................................*/
				item = (char *)(playerlist->Get_Item(i + 1));
				playerlist->Remove_Item(item);
				playerlist->Flag_To_Redraw();
				delete [] item;
				/*...............................................................
				Mark his color as available
				...............................................................*/
				ColorUsed[Players[i]->Player.Color] = 0;
				/*...............................................................
				Delete from the Vector list
				...............................................................*/
				Players.Delete(Players[i]);
				break;
			}
		}
	}

	/*------------------------------------------------------------------------
	NET_MESSAGE: Someone is sending us a message
	------------------------------------------------------------------------*/
	else if (GPacket.Command==NET_MESSAGE) {
		sprintf(txt,Text_String (TXT_FROM), GPacket.Name, GPacket.Message.Buf);
		magic_number = *((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-4));
		crc = *((unsigned short*)(GPacket.Message.Buf + COMPAT_MESSAGE_LENGTH-2));
		color = MPlayerID_To_ColorIndex(GPacket.Message.ID);
		Messages.Add_Message (txt, MPlayerTColors[color],
			TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 1200, magic_number, crc);
		retval = EV_MESSAGE;
	}

	return(retval);
}



/***************************************************************************
 * Compute_Name_CRC -- computes CRC from char string                       *
 *                                                                         *
 * INPUT:                                                                  *
 *		name		string to create CRC for												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		CRC																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/29/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned long Compute_Name_CRC(char *name)
{
	char buf[80];
	unsigned long crc = 0L;
	int i;

	strcpy (buf, name);
	strupr (buf);

	for (i = 0; i < (int)strlen(buf); i++) {
		Add_CRC (&crc, (unsigned long)buf[i]);
	}

	return (crc);
}

/***************************************************************************
 * Net_Reconnect_Dialog -- Draws/updates the network reconnect dialog      *
 *                                                                         *
 * INPUT:                                                                  *
 *		reconn			1 = reconnect, 0 = waiting for first-time connection	*
 *		fresh				1 = draw from scratch, 0 = only update time counter	*
 *		oldest_index	IPX connection index of oldest connection 				*
 *							(only used for reconnection)									*
 *		timeval			value to print in the countdown field						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   07/08/1995 BRR : Created.                                             *
 *=========================================================================*/
void Net_Reconnect_Dialog(int reconn, int fresh, int oldest_index,
	unsigned long timeval)
{
	static int x,y,w,h;
	int id;
	char buf1[40] = {0};
	char buf2[40] = {0};
	char const *buf3 = "";

	int factor = (SeenBuff.Get_Width() == 320) ? 1 : 2;

	int d_txt6_h = 6*factor+1;
	int d_margin = 5*factor;


	/*------------------------------------------------------------------------
	Draw the dialog from scratch
	------------------------------------------------------------------------*/
	if (fresh) {
		Fancy_Text_Print ("", 0, 0, CC_GREEN, TBLACK,
			TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
		if (reconn) {
			id = Ipx.Connection_ID(oldest_index);
			sprintf(buf1,Text_String(TXT_RECONNECTING_TO),
				Ipx.Connection_Name(id));
		} else {
			sprintf(buf1,Text_String(TXT_WAITING_FOR_CONNECTIONS));
		}
		sprintf(buf2,Text_String(TXT_TIME_ALLOWED), timeval + 1);
		buf3 = Text_String(TXT_PRESS_ESC);

		w = MAX(String_Pixel_Width(buf1),String_Pixel_Width(buf2));
		w = MAX(String_Pixel_Width(buf3), (unsigned)w);
		w += (d_margin * 4);
		h = (d_txt6_h * 3) + (d_margin * 6);
		x = 160*factor - (w / 2);
		y = 100*factor - (h / 2);

		Hide_Mouse();
		Set_Logic_Page(SeenBuff);
		Dialog_Box(x, y, w, h);

		Fancy_Text_Print (buf1, 160*factor, y + (d_margin * 2), CC_GREEN, BLACK,
			TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

		Fancy_Text_Print (buf2, 160*factor, y + (d_margin * 2) + d_txt6_h + d_margin,
			CC_GREEN, BLACK,
			TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

		Fancy_Text_Print (buf3, 160*factor, y + (d_margin * 2) + (d_txt6_h + d_margin) * 2,
			CC_GREEN, BLACK,
			TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

		Show_Mouse();

	} else {

		/*------------------------------------------------------------------------
		Just update the timeout value on the dialog
		------------------------------------------------------------------------*/
		Hide_Mouse();
		Set_Logic_Page(SeenBuff);

		sprintf(buf2,Text_String(TXT_TIME_ALLOWED), timeval + 1);
		int pixwidth = String_Pixel_Width (buf2);
		LogicPage->Fill_Rect (160*factor - (pixwidth/2) - 12, y+(d_margin*2) + d_txt6_h + d_margin,
									160*factor + (pixwidth/2) + 12, y+(d_margin*2) + d_txt6_h*2 + d_margin,
									TBLACK);
		Fancy_Text_Print (buf2, 160*factor, y + (d_margin * 2) + d_txt6_h + d_margin,
			CC_GREEN, BLACK,
			TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

		Show_Mouse();
	}
}






/***********************************************************************************************
 * Wait_For_Focus -- Wait for game to be in focus before proceeding                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    1/6/97 3:23PM ST : Created                                                               *
 *=============================================================================================*/
void Wait_For_Focus (void)
{
	CountDownTimerClass focus_timer;
	focus_timer.Set(5*60);
	ShowWindow ( MainWindow, SW_SHOWMAXIMIZED );

	/*
	** Process the message loop until we are in focus.
	*/
	if (!GameInFocus){
		CCDebugString ("C&C95 - Waiting for game to come into focus.");
		do {
			CCDebugString (".");
			Keyboard::Check();
			if (!focus_timer.Time()){
				CCDebugString ("C&C95 - Calling SetForgroundWindow.\n");
				SetForegroundWindow ( MainWindow );
				CCDebugString ("C&C95 - Calling ShowWindow.\n");
				ShowWindow ( MainWindow, SW_SHOWMAXIMIZED );
				focus_timer.Set(5*60);
			}

		}while (!GameInFocus);
		CCDebugString ("\n");
		AllSurfaces.SurfacesRestored=FALSE;
	}
}





extern bool Spawn_WChat(bool can_launch);

/***********************************************************************************************
 * Net_Fake_New_Dialog -- Just like Net_New_Dialog but without the Dialog. For internet play   *
 *                                                                                             *
 *  This 'dialog' does all the non-dialog game set up stuff that is done in the normal         *
 * network game set up dialog. The only visible button is 'cancel'                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   true if successfully connected                                                    *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/24/96 10:34AM ST : Created                                                             *
 *=============================================================================================*/
static int Net_Fake_New_Dialog(void)
{
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	int d_dialog_w = 120*factor;											// dialog width
	int d_dialog_h = 80*factor;											// dialog height
	int d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*factor - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	//d_playerlist_w = 100;
	int d_playerlist_w = 106*factor;
	int d_playerlist_h = 27*factor;
	//int d_playerlist_x = 10 * factor;	//off screen
	int d_playerlist_x = 500*factor;	//10 * factor;	//off screen
	int d_playerlist_y = d_dialog_y + 20;

#if (GERMAN | FRENCH)
	int d_cancel_w = 50*factor;
#else
	int d_cancel_w = 45*factor;
#endif
	int d_cancel_h = 9*factor;
	int d_cancel_x = d_dialog_cx - (d_cancel_w / 2);
	int d_cancel_y = d_dialog_y + d_dialog_h - 20*factor;

#if (GERMAN | FRENCH)
	int	width=160*factor;
	int	height=80*factor;
#else
	int	width=120*factor;
	int	height=80*factor;
#endif	//GERMAN | FRENCH

	bool	player_joined = false;
	CountDownTimerClass join_timer;

	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String((char*)Text_String(TXT_CONNECTING), SeenBuff.Get_Height(), width, height);

#if (GERMAN | FRENCH)
	d_dialog_w = width + 25*factor;
	d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	d_cancel_x = d_dialog_cx - (d_cancel_w / 2);
#endif


	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_CANCEL = 100,
		BUTTON_PLAYERLIST,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_MESSAGE,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display = REDRAW_ALL;		// redraw level
	bool process = true;						// process while true
	KeyNumType input;

	char credbuf[CREDITSBUF_MAX];			// for credit edit box
	int old_cred;								// old value in credits buffer
	int transmit;								// 1 = re-transmit new game options

	long ok_timer = 0;						// for timing OK button
	int rc;
	int i,j;
	char *item;
	int tabs[] = {77*factor};				// tabs for player list box

	long ping_timer = 0;						// for sending Ping packets

	unsigned char tmp_id[MAX_PLAYERS];	// temp storage for sorting player ID's
	int min_index;								// for sorting player ID's
	unsigned char min_id;					// for sorting player ID's
	unsigned char id;							// connection ID
	JoinEventType whahoppa;					// event generated by received packets

	void const *up_button;
	void const *down_button;

	if (InMainLoop){
		up_button = Hires_Retrieve("BTN-UP.SHP");
		down_button = Hires_Retrieve("BTN-DN.SHP");
	}else{
		up_button = Hires_Retrieve("BTN-UP2.SHP");
		down_button = Hires_Retrieve("BTN-DN2.SHP");
	}

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	ColorListClass playerlist(BUTTON_PLAYERLIST,
		d_playerlist_x, d_playerlist_y, d_playerlist_w, d_playerlist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_cancel_x, d_cancel_y);
//#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
//#endif


	CCDebugString ("C&C95 - In new game dialog - initialising lists.\n");
	/*
	------------------------- Build the button list --------------------------
	*/
	commands = &playerlist;
	cancelbtn.Add_Tail(*commands);

	playerlist.Set_Tabs(tabs);

	/*
	----------------------------- Various Inits ------------------------------
	*/

	sprintf(credbuf, "%d", MPlayerCredits);
	old_cred = MPlayerCredits;

	/*........................................................................
	Init other scenario parameters
	........................................................................*/
	Special.IsTGrowth = MPlayerTiberium;
	Special.IsTSpread = MPlayerTiberium;
	transmit = 0;

	/*........................................................................
	Init player color-used flags
	........................................................................*/
	for (i = 0; i < MAX_MPLAYER_COLORS; i++) {
		ColorUsed[i] = 0;							// init all colors to available
	}
	ColorUsed[MPlayerColorIdx] = 1;			// set my color to used
	playerlist.Set_Selected_Style(ColorListClass::SELECT_BAR, CC_GREEN_SHADOW);

	/*........................................................................
	Init random-number generator, & create a seed to be used for all random
	numbers from here on out
	........................................................................*/
	//ST - 12/18/2018 11:37AM
	//randomize();
	//Seed = rand();

	/*------------------------------------------------------------------------
	Add myself to the list.  Note that since I'm not in the Players Vector,
	the Vector & listbox are now 1 out of sync.
	------------------------------------------------------------------------*/
	item = new char [MPLAYER_NAME_MAX + 4];
	if (MPlayerHouse==HOUSE_GOOD) {
		sprintf(item,"%s\t%s",MPlayerName,Text_String(TXT_G_D_I));
	} else {
		sprintf(item,"%s\t%s",MPlayerName,Text_String(TXT_N_O_D));
	}
	playerlist.Add_Item(item, MPlayerTColors[MPlayerColorIdx]);

	Wait_For_Focus();

	CCDebugString ("C&C95 - About to uncompress title page.\n");
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	CCDebugString ("C&C95 - About to set the palette.\n");
	Set_Palette(Palette);
	CCDebugString ("C&C95 - Palette was set OK.\n");

	if (LogicPage != &SeenBuff && LogicPage!= &HidPage){
		CCDebugString ("C&C95 - Logic page invalid");
		Set_Logic_Page (SeenBuff);
	}

	char a_buffer [128];
	sprintf (a_buffer, "Number of players:%d", Players.Count());
	CCDebugString (a_buffer);


#ifdef VIRTUAL_SUBNET_SERVER
	/*
	** Send a bogus packet to wake up the VSS
	*/
	memset (&GPacket, 0, sizeof(GlobalPacketType));

	GPacket.Command = (NetCommandType)50;		//Invalid command
	strcpy (GPacket.Name, MPlayerName);
	Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType), 0, NULL);
#endif	//VIRTUAL_SUBNET_SERVER


	CCDebugString ("C&C95 - About to reveal mouse\n");
	while (Get_Mouse_State() > 0) Show_Mouse();

	/*
	---------------------------- Processing loop -----------------------------
	*/
	CCDebugString ("C&C95 - Entering join dialogue loop\n");
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {

				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
				Set_Palette(Palette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Draw_Caption (TXT_NONE, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print(TXT_CONNECTING, d_dialog_cx-width/2, d_dialog_y + 25*factor, CC_GREEN, TBLACK,
									TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}

			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Draw_All();
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {

			/*------------------------------------------------------------------
			CANCEL: send a SIGN_OFF, bail out with error code
			------------------------------------------------------------------*/
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				memset (&GPacket, 0, sizeof(GlobalPacketType));

				GPacket.Command = NET_SIGN_OFF;
				strcpy (GPacket.Name, MPlayerName);

				/*...............................................................
				Broadcast my sign-off over my network
				...............................................................*/
				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					0, NULL);
				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					0, NULL);
				while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;

				/*...............................................................
				Broadcast my sign-off over a bridged network if there is one
				...............................................................*/
				while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;

				/*...............................................................
				And now, just be absolutely sure, send my sign-off to each
				player in my game.  (If there's a bridge between us, the other
				player will have specified my address, so he can cross the
				bridge; but I may not have specified a bridge address, so the
				only way I have of crossing the bridge is to send a packet
				directly to him.)
				...............................................................*/
				for (i = 0; i < Players.Count(); i++) {
					Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 1,
						&(Players[i]->Address));
					Ipx.Service();
				}
				while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;
				MPlayerGameName[0] = 0;
				process = false;
				rc = false;
				Send_Data_To_DDE_Server ("Hello", strlen("Hello"), DDEServerClass::DDE_CONNECTION_FAILED);
				GameStatisticsPacketSent = false;
				Spawn_WChat(false);
				break;

			/*------------------------------------------------------------------
			default: exit loop with TRUE status
			------------------------------------------------------------------*/
			default:
#ifdef VIRTUAL_SUBNET_SERVER
				if (Players.Count() == InternetMaxPlayers-1){


#else //VIRTUAL_SUBNET_SERVER
				if (Players.Count() > 0){
#endif	// VIRTUAL_SUBNET_SERVER

//char ddkks[128];
//sprintf (ddkks, "C&C95 - Players.Count() = %d\n", Players.Count());
//CCDebugString (ddkks);

					/*
					** Wait for several secs after receiving request to join before sending
					** start game packet
					*/
					if (!player_joined){
						player_joined = true;
						join_timer.Set (3*60, true);
						break;
					}else{
						if (join_timer.Time()) break;
					}

CCDebugString ("C&C95 - Join timer expired\n");

					/*...............................................................
					If a new player has joined in the last second, don't allow
					an OK; force a wait longer than 2 seconds (to give all players
					a chance to know about this new guy)
					...............................................................*/
					i = MAX(Ipx.Global_Response_Time() * 2, (unsigned long)120);
					while (TickCount.Time() - ok_timer < i)
						Ipx.Service();

					/*...............................................................
					If there are at least 2 players, go ahead & play; error otherwise
					...............................................................*/
					if (MPlayerSolo || Players.Count() > 0) {
						rc = TRUE;
						process = FALSE;
					} else {
						CCMessageBox().Process (TXT_ONLY_ONE,TXT_OOPS,NULL);
						display = REDRAW_ALL;
					}
				}
				break;
		}

		/*---------------------------------------------------------------------
		Process incoming packets
		---------------------------------------------------------------------*/
		whahoppa = Get_NewGame_Responses(&playerlist);
		if (whahoppa == EV_NEW_PLAYER) {
			ok_timer = TickCount.Time();
			transmit = 1;
		} else {
			if (whahoppa == EV_MESSAGE) {
				display = REDRAW_MESSAGE;
			}
		}

		/*---------------------------------------------------------------------
		If our Transmit flag is set, we need to send out a game option packet
		---------------------------------------------------------------------*/
		if (transmit) {
			for (i = 0; i < Players.Count(); i++) {
				memset (&GPacket, 0, sizeof(GlobalPacketType));

				GPacket.Command = NET_GAME_OPTIONS;
				GPacket.ScenarioInfo.Scenario = ScenarioIdx;	//MPlayerFilenum[ScenarioIdx];
				GPacket.ScenarioInfo.Credits = MPlayerCredits;
				GPacket.ScenarioInfo.IsBases = MPlayerBases;
				GPacket.ScenarioInfo.IsTiberium = MPlayerTiberium;
				GPacket.ScenarioInfo.IsGoodies = MPlayerGoodies;
				GPacket.ScenarioInfo.IsGhosties = MPlayerGhosts;
				GPacket.ScenarioInfo.BuildLevel = BuildLevel;
				GPacket.ScenarioInfo.UnitCount = MPlayerUnitCount;
				GPacket.ScenarioInfo.Seed = Seed;
				GPacket.ScenarioInfo.Special = Special;
				GPacket.ScenarioInfo.GameSpeed = Options.GameSpeed;

				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					1, &(Players[i]->Address) );
			}
			transmit = 0;
		}

		/*---------------------------------------------------------------------
		Ping every player in my game, to force the Global Channel to measure
		the connection response time.
		---------------------------------------------------------------------*/
		if (TickCount.Time() - ping_timer > 15) {
			memset (&GPacket, 0, sizeof(GlobalPacketType));
			GPacket.Command = NET_PING;
			for (i = 0; i < Players.Count(); i++) {
				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					1, &(Players[i]->Address) );
			}
			ping_timer = TickCount.Time();
		}

		/*---------------------------------------------------------------------
		Service the Ipx connections
		---------------------------------------------------------------------*/
		Ipx.Service();

		/*---------------------------------------------------------------------
		Service the sounds & score; GameActive must be false at this point,
		so Call_Back() doesn't intercept global messages from me!
		---------------------------------------------------------------------*/
		Call_Back();

	}	/* end of while */

CCDebugString ("C&C95 - Exited process loop\n");

	/*------------------------------------------------------------------------
	Establish connections with all other players.
	------------------------------------------------------------------------*/
	if (rc) {
		/*.....................................................................
		Set the number of players in this game, and my ID
		.....................................................................*/
		MPlayerCount = Players.Count() + 1;
		MPlayerLocalID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);

		/*.....................................................................
		Get the scenario filename
		.....................................................................*/
		Scenario = ScenarioIdx;	//PlayerFilenum[ScenarioIdx]; We are passed actual number now from wchat not index from
		//Scenario = MPlayerFilenum[ScenarioIdx];

		/*.....................................................................
		Compute frame delay value for packet transmissions:
		- Divide global channel's response time by 8 (2 to convert to 1-way
		  value, 4 more to convert from ticks to frames)
		.....................................................................*/
		MPlayerMaxAhead = MAX( (Ipx.Global_Response_Time() / 8), (unsigned long)2);

		/*.....................................................................
		Send all players the NET_GO packet.  Wait until all ACK's have been
		received.
		.....................................................................*/
CCDebugString ("C&C95 - Sending the 'GO' packet\n");
		memset (&GPacket, 0, sizeof(GlobalPacketType));
		GPacket.Command = NET_GO;
		GPacket.ResponseTime.OneWay = MPlayerMaxAhead;
		for (i = 0; i < Players.Count(); i++) {
char flopbuf [128];
sprintf (flopbuf, "Sending 'GO' packet to address %d\n", *((unsigned short*)&(Players[i]->Address)));
CCDebugString (flopbuf);


			Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
				1, &(Players[i]->Address) );
			/*..................................................................
			Wait for all the ACK's to come in.
			..................................................................*/
			while (Ipx.Global_Num_Send() > 0)
				Ipx.Service();
		}

		/*.....................................................................
		Form connections with all other players.  Form the IPX Connection ID
		from the player's Color (high byte) and House (low byte).  This
		will let us extract any player's color & house at any time.
		Fill in 'tmp_id' while we're doing this.
		.....................................................................*/
		for (i = 0; i < Players.Count(); i++) {
			id = Build_MPlayerID (Players[i]->Player.Color,
				Players[i]->Player.House);

			tmp_id[i] = id;

			Ipx.Create_Connection(id, Players[i]->Name, &(Players[i]->Address) );
		}

#ifdef VIRTUAL_SUBNET_SERVER
CCDebugString ("C&C95 - Creating connection to the VSS\n");
		/*
		** Create an additional connection to the VSS
		*/
		if (UseVirtualSubnetServer){
			IPXAddressClass vss_global_address;
			NetNodeType vss_node;
			NetNumType	vss_net;
			memset (vss_net, 1, sizeof (vss_net));
			memset (vss_node, 0, sizeof (vss_node));
			vss_global_address.Set_Address(vss_net, vss_node);
			Ipx.Create_Connection( VSS_ID, "VSS", &vss_global_address);
		}
#endif	//VIRTUAL_SUBNET_SERVER
		tmp_id[i] = MPlayerLocalID;

		/*.....................................................................
		Store every player's ID in the MPlayerID[] array.  This array will
		determine the order of event execution, so the ID's must be stored
		in the same order on all systems.
		.....................................................................*/
		for (i = 0; i < MPlayerCount; i++) {
			min_index = 0;
			min_id = 0xff;
			for (j = 0; j < MPlayerCount; j++) {
				if (tmp_id[j] < min_id) {
					min_id = tmp_id[j];
					min_index = j;
				}
			}
			MPlayerID[i] = tmp_id[min_index];
			tmp_id[min_index] = 0xff;
		}
		/*.....................................................................
		Fill in the array of player names, including my own.
		.....................................................................*/
		for (i = 0; i < MPlayerCount; i++) {
			if (MPlayerID[i] == MPlayerLocalID) {
				strcpy (MPlayerNames[i], MPlayerName);
			} else {
				strcpy (MPlayerNames[i], Ipx.Connection_Name(MPlayerID[i]));
			}
		}
	}

	/*------------------------------------------------------------------------
	Init network timing values, using previous response times as a measure
	of what our retry delta & timeout should be.
	------------------------------------------------------------------------*/
	Ipx.Set_Timing (Ipx.Global_Response_Time() + 2, -1,
		Ipx.Global_Response_Time() * 4);

	/*------------------------------------------------------------------------
	Clear all lists
	------------------------------------------------------------------------*/
	Clear_Player_List(&playerlist);

	/*------------------------------------------------------------------------
	Restore screen
	------------------------------------------------------------------------*/
	Hide_Mouse();
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Show_Mouse();

	if (rc){
		Wait_For_Focus();
	}

	return(rc);
}






/***********************************************************************************************
 * Net_Fake_Join_Dialog -- Like Net_Join_Dialog but with no dialogs. For Internet Play.        *
 *                                                                                             *
 *  This 'dialog' does all the non-dialog game set up stuff that is done in the normal         *
 * network game set up dialog. The only visible button is 'cancel'                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   0 = good, -1 = bad                                                                *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/24/96 11:07AM ST : Created                                                             *
 *=============================================================================================*/

static int Net_Fake_Join_Dialog(void)
{
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
/* ###Change collision detected! C:\PROJECTS\CODE\NETDLG.CPP... */
	int d_dialog_w = 120 *factor;											// dialog width
	int d_dialog_h = 80*factor;											// dialog height
	int d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*factor - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_margin1=10;
	int d_txt6_h=15;

	int d_gamelist_w = 160*factor;
	int d_gamelist_h = 27*factor;
	int d_gamelist_x = 500*factor;  //230*factor;	//Off screen
	//int d_gamelist_x = 230*factor;	//Off screen
	int d_gamelist_y = d_dialog_y + 20;

	int d_playerlist_w = 106*factor;
	int d_playerlist_h = 27*factor;
	//int d_playerlist_x = 10 * factor;	//Off screen
	int d_playerlist_x = 500*factor;	//10 * factor;	//Off screen
	int d_playerlist_y = d_gamelist_y + 20;

#if (GERMAN | FRENCH)
	int d_cancel_w = 50*factor;
#else
	int d_cancel_w = 45*factor;
#endif
	int d_cancel_h = 9*factor;
	int d_cancel_x = d_dialog_cx - d_cancel_w / 2;
	int d_cancel_y = d_dialog_y + d_dialog_h - 20*factor;

	bool ready_to_go = false;

#if (GERMAN | FRENCH)
	int	width=160*factor;
	int	height=80*factor;
#else
	int	width=120*factor;
	int	height=80*factor;
#endif	//GERMAN | FRENCH

	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String((char*)Text_String(TXT_CONNECTING), SeenBuff.Get_Height(), width, height);

#if (GERMAN | FRENCH)
	d_dialog_w = width + 25*factor;
	d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	d_cancel_x = d_dialog_cx - (d_cancel_w / 2);
#endif

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_CANCEL = 100,
		BUTTON_GAMELIST,
		BUTTON_PLAYERLIST,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_MESSAGE,
		REDRAW_COLORS,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display = REDRAW_ALL;		// redraw level
	bool process = true;						// process while true
	KeyNumType input;

	JoinStateType joinstate = JOIN_NOTHING;	// current "state" of this dialog
	char namebuf[MPLAYER_NAME_MAX] = {0};		// buffer for player's name
	int game_index = -1;						// index of currently-selected game
	int join_index = -1;						// index of game we're joining
	int rc = 0;									// -1 = user cancelled, 1 = New
	JoinEventType event;						// event from incoming packet
	int i,j;										// loop counter
	int parms_received;						// 1 = game options received

	unsigned char tmp_id[MAX_PLAYERS];	// temp storage for sorting player ID's
	int min_index;								// for sorting player ID's
	unsigned char min_id;					// for sorting player ID's
	unsigned char id;							// connection ID
	char * item;
	unsigned long starttime;

	NodeNameType *who;

	void const *up_button;
	void const *down_button;

	if (InMainLoop){
		up_button = Hires_Retrieve("BTN-UP.SHP");
		down_button = Hires_Retrieve("BTN-DN.SHP");
	}else{
		up_button = Hires_Retrieve("BTN-UP2.SHP");
		down_button = Hires_Retrieve("BTN-DN2.SHP");
	}

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	ColorListClass playerlist(BUTTON_PLAYERLIST,
		d_playerlist_x, d_playerlist_y, d_playerlist_w, d_playerlist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	ListClass gamelist(BUTTON_GAMELIST,
		d_gamelist_x, d_gamelist_y, d_gamelist_w, d_gamelist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_cancel_x, d_cancel_y);
//#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
//#endif

	/*
	----------------------------- Various Inits ------------------------------
	*/
	//MPlayerColorIdx = MPlayerPrefColor;			// init my preferred color

	playerlist.Set_Selected_Style(ColorListClass::SELECT_NONE);

	Fancy_Text_Print("", 0, 0, CC_GREEN, TBLACK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

	/*
	--------------------------- Send network query ---------------------------
	*/
	CCDebugString ("C&C95 - About to call Send_Join_Queries.\n");
	Send_Join_Queries (game_index, 1, 0);

	Wait_For_Focus();

	CCDebugString ("C&C95 - About to uncompress title page.\n");
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	CCDebugString ("C&C95 - About to set the palette.\n");
	Set_Palette(Palette);
	CCDebugString ("C&C95 - Palette was set OK.\n");

	if (LogicPage != &SeenBuff && LogicPage!= &HidPage){
		CCDebugString ("C&C95 - Logic page invalid\n");
		Set_Logic_Page (SeenBuff);
	}


	char a_buffer [128];
	sprintf (a_buffer, "C&C95 - Number of players:%d\n", Players.Count());
	CCDebugString (a_buffer);

	/*
	---------------------------- Init Mono Output ----------------------------
	*/
	CCDebugString ("C&C95 - About to reveal mouse\n");
	while (Get_Mouse_State() > 0) Show_Mouse();

	/*
	---------------------------- Processing loop -----------------------------
	*/
	CCDebugString ("C&C95 - Entering join dialogue loop\n");
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
				Set_Palette(Palette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Draw_Caption (TXT_NONE, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print(TXT_CONNECTING, d_dialog_cx-width/2, d_dialog_y + 25*factor, CC_GREEN, TBLACK,
									TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				/*
				.................... Rebuild the button list ....................
				*/
				cancelbtn.Zap();
				gamelist.Zap();
				playerlist.Zap();

				commands = &cancelbtn;
				gamelist.Add_Tail(*commands);
				playerlist.Add_Tail(*commands);
			}
			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Draw_All();
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {

			/*------------------------------------------------------------------
			CANCEL: send a SIGN_OFF
			- If we're part of a game, stay in this dialog; otherwise, exit
			------------------------------------------------------------------*/
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				memset (&GPacket, 0, sizeof(GlobalPacketType));

				GPacket.Command = NET_SIGN_OFF;
				strcpy(GPacket.Name,MPlayerName);

				/*...............................................................
				If we're joined to a game, make extra sure the other players in
				that game know I'm exiting; send my SIGN_OFF as an ack-required
				packet.  Do not send this packet to myself (index 0).
				...............................................................*/
				if (joinstate == JOIN_CONFIRMED) {
					//
					// Remove myself from the player list box
					//
					item = (char *)(playerlist.Get_Item(0));
					playerlist.Remove_Item(item);
					delete [] item;
					playerlist.Flag_To_Redraw();

					//
					// Remove myself from the Players list
					//
					who = Players[0];
					Players.Delete(0);
					delete who;

					for (i = 0; i < Players.Count(); i++) {
						Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 1,
							&(Players[i]->Address));
						Ipx.Service();
					}
				}

				/*...............................................................
				Now broadcast my SIGN_OFF so other players looking at this game
				know I'm leaving.
				...............................................................*/
				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					0, NULL);
				Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
					0, NULL);

				if (IsBridge) {
					Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
						&BridgeNet);
					Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
						&BridgeNet);
				}

				while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;

				Send_Data_To_DDE_Server ("Hello", strlen("Hello"), DDEServerClass::DDE_CONNECTION_FAILED);
				GameStatisticsPacketSent = false;
				Spawn_WChat(false);
				process = false;
				rc = -1;
#if (0)
				if (joinstate != JOIN_CONFIRMED) {
					process = false;
					rc = -1;
				} else {
					MPlayerGameName[0] = 0;
					joinstate = JOIN_NOTHING;
					display = REDRAW_ALL;
				}
#endif	//(0)
				break;

			/*------------------------------------------------------------------
			JOIN: send a join request packet & switch to waiting-for-confirmation
			mode.  (Request_To_Join fills in MPlayerName with my namebuf.)
			------------------------------------------------------------------*/
			default:
				if (joinstate == JOIN_NOTHING && Games.Count()!=0){
					gamelist.Set_Selected_Index(0);
					join_index = gamelist.Current_Index();
					parms_received = 0;
					if (Request_To_Join (MPlayerName, join_index, &playerlist, MPlayerHouse,
						MPlayerColorIdx)) {
						joinstate = JOIN_WAIT_CONFIRM;
					} else {
						display = REDRAW_ALL;
					}
				}
				break;

		}

		/*---------------------------------------------------------------------
		Resend our query packets
		---------------------------------------------------------------------*/
		Send_Join_Queries(game_index, 0, 0);

		/*---------------------------------------------------------------------
		Process incoming packets
		---------------------------------------------------------------------*/
		event = Get_Join_Responses(&joinstate, &gamelist, &playerlist,
			join_index);
		/*.....................................................................
		If we've changed state, redraw everything; if we're starting the game,
		break out of the loop.  If we've just joined, send out a player query
		so I'll get added to the list instantly.
		.....................................................................*/
		if (event == EV_STATE_CHANGE) {
			display = REDRAW_ALL;
			if (joinstate==JOIN_GAME_START) {
CCDebugString ("C&C95 - Received 'GO' packet\n");

				ready_to_go = true;
			} else {

				/*..................................................................
				If we're newly-confirmed, immediately send out a player query
				..................................................................*/
				if (joinstate==JOIN_CONFIRMED) {

					Clear_Player_List(&playerlist);

					item = new char [MPLAYER_NAME_MAX + 4];
					if (MPlayerHouse==HOUSE_GOOD) {
						sprintf(item,"%s\t%s",MPlayerName,Text_String(TXT_G_D_I));
					} else {
						sprintf(item,"%s\t%s",MPlayerName,Text_String(TXT_N_O_D));
					}
					playerlist.Add_Item (item, MPlayerTColors[MPlayerColorIdx]);

					who = new NodeNameType;
					strcpy(who->Name, MPlayerName);
					who->Address = IPXAddressClass();
					who->Player.House = MPlayerHouse;
					who->Player.Color = MPlayerColorIdx;
					Players.Add (who);

					Send_Join_Queries (game_index, 0, 1);
				} else {

					/*..................................................................
					If we've been rejected, clear any messages we may have been typing.
					..................................................................*/
					if (joinstate==JOIN_REJECTED) {

						//
						// Remove myself from the player list box
						//
						item = (char *)(playerlist.Get_Item(0));
						if (item){
							playerlist.Remove_Item(item);
							delete [] item;
							playerlist.Flag_To_Redraw();
						}

						//
						// Remove myself from the Players list
						//
						if (Players.Count()){
							who = Players[0];
							Players.Delete(0);
							delete who;
						}
					}
				}
			}
		} else

		/*.....................................................................
		If a new game is detected, and it's the first game on our list,
		automatically send out a player query for that game.
		.....................................................................*/
		if (event == EV_NEW_GAME && gamelist.Count()==1) {
			gamelist.Set_Selected_Index(0);
			game_index = gamelist.Current_Index();
			Send_Join_Queries (game_index, 0, 1);
		} else

		/*.....................................................................
		If the game options have changed, print them.
		.....................................................................*/
		if (event == EV_GAME_OPTIONS) {
			parms_received = 1;
			display = REDRAW_MESSAGE;
		} else

		/*.....................................................................
		Draw an incoming message
		.....................................................................*/
		if (event == EV_MESSAGE) {
			display = REDRAW_MESSAGE;
		} else

		/*.....................................................................
		A game before the one I've selected is gone, so we have a new index now.
		'game_index' must be kept set to the currently-selected list item, so
		we send out queries for the currently-selected game.  It's therefore
		imperative that we detect any changes to the game list.
		If we're joined in a game, we must decrement our game_index to keep
		it aligned with the game we're joined to.
		.....................................................................*/
		if (event == EV_GAME_SIGNOFF) {
			if (joinstate==JOIN_CONFIRMED) {
				game_index--;
				join_index--;
				gamelist.Set_Selected_Index(join_index);
			} else {
				gamelist.Flag_To_Redraw();
				Clear_Player_List(&playerlist);
				game_index = gamelist.Current_Index();
				Send_Join_Queries (game_index, 0, 1);
			}
		}

		/*---------------------------------------------------------------------
		Service the Ipx connections
		---------------------------------------------------------------------*/
		Ipx.Service();

		/*---------------------------------------------------------------------
		Clean out the Game List; if an old entry is found:
		- Remove it
		- Clear the player list
		- Send queries for the new selected game, if there is one
		---------------------------------------------------------------------*/
		for (i = 0; i < Games.Count(); i++) {
			if (TickCount.Time() - Games[i]->Game.LastTime > 400) {
				Games.Delete(Games[i]);
				item = (char *)(gamelist.Get_Item (i));
				gamelist.Remove_Item (item);
				delete [] item;
				if (i <= game_index) {
					gamelist.Flag_To_Redraw();
					Clear_Player_List(&playerlist);
					game_index = gamelist.Current_Index();
					Send_Join_Queries (game_index, 0, 1);
				}
			}
		}

		/*
		** If we were flagged to start the game and we recognise both players then quit the loop
		*/

//char ddkks[128];
//sprintf (ddkks, "C&C95 - Players.Count() = %d\n", Players.Count());
//CCDebugString (ddkks);
		if (ready_to_go){	// && Players.Count() == InternetMaxPlayers){
			rc = 0;
			process = false;
		}

		/*---------------------------------------------------------------------
		Service the sounds & score; GameActive must be false at this point,
		so Call_Back() doesn't intercept global messages from me!
		---------------------------------------------------------------------*/
		Call_Back();
	}

	/*------------------------------------------------------------------------
	Establish connections with all other players.
	------------------------------------------------------------------------*/
	if (rc == 0) {
		/*.....................................................................
		If the other guys are playing a scenario I don't have (sniff), I can't
		play.  Try to bail gracefully.
		.....................................................................*/
		if (ScenarioIdx==-1) {
			CCMessageBox().Process (TXT_UNABLE_PLAY_WAAUGH);

			//
			// Remove myself from the player list box
			//
			item = (char *)(playerlist.Get_Item(0));
			playerlist.Remove_Item(item);
			delete [] item;
			playerlist.Flag_To_Redraw();

			//
			// Remove myself from the Players list
			//
			who = Players[0];
			Players.Delete(0);
			delete who;

			memset (&GPacket, 0, sizeof(GlobalPacketType));

			GPacket.Command = NET_SIGN_OFF;
			strcpy (GPacket.Name, MPlayerName);

			for (i = 0; i < Players.Count(); i++) {
				Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 1,
					&(Players[i]->Address));
				Ipx.Service();
			}

			Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
				0, NULL);
			Ipx.Send_Global_Message (&GPacket, sizeof (GlobalPacketType),
				0, NULL);

			if (IsBridge) {
				Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
					&BridgeNet);
				Ipx.Send_Global_Message (&GPacket, sizeof(GlobalPacketType), 0,
					&BridgeNet);
			}

			while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) ;

			rc = -1;

		} else {

			/*..................................................................
			Set the number of players in this game, and my ID
			..................................................................*/
			MPlayerCount = Players.Count();
			MPlayerLocalID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);

			/*..................................................................
			Get the scenario number
			..................................................................*/
			Scenario = ScenarioIdx;	//PlayerFilenum[ScenarioIdx]; We are passed actual number now from wchat not index from

			/*..................................................................
			Form connections with all other players.  Form the IPX Connection ID
			from the player's Color and House.  This will let us extract any
			player's color & house at any time.  Fill in 'tmp_id' while we're
			doing this.
			..................................................................*/
			for (i = 0; i < Players.Count(); i++) {

				/*...............................................................
				Only create the connection if it's not myself!
				...............................................................*/
				if (strcmp (MPlayerName, Players[i]->Name)) {
					id = Build_MPlayerID(Players[i]->Player.Color,
						Players[i]->Player.House);

					tmp_id[i] = id;

					Ipx.Create_Connection((int)id, Players[i]->Name, &(Players[i]->Address) );
				} else {
					tmp_id[i] = MPlayerLocalID;
				}
			}

#ifdef VIRTUAL_SUBNET_SERVER
			/*
			** Create an additional connection to the VSS
			*/
			if (UseVirtualSubnetServer){
				IPXAddressClass vss_global_address;
				NetNodeType vss_node;
				NetNumType	vss_net;
				memset (vss_net, 1, sizeof (vss_net));
				memset (vss_node, 0, sizeof (vss_node));
				vss_global_address.Set_Address(vss_net, vss_node);
				Ipx.Create_Connection( VSS_ID, "VSS", &vss_global_address);
			}
#endif	//VIRTUAL_SUBNET_SERVER

			/*..................................................................
			Store every player's ID in the MPlayerID[] array.  This array will
			determine the order of event execution, so the ID's must be stored
			in the same order on all systems.
			..................................................................*/
			for (i = 0; i < MPlayerCount; i++) {
				min_index = 0;
				min_id = 0xff;
				for (j = 0; j < MPlayerCount; j++) {
					if (tmp_id[j] < min_id) {
						min_id = tmp_id[j];
						min_index = j;
					}
				}
				MPlayerID[i] = tmp_id[min_index];
				tmp_id[min_index] = 0xff;
			}
			/*..................................................................
			Fill in the array of player names, including my own.
			..................................................................*/
			for (i = 0; i < MPlayerCount; i++) {
				if (MPlayerID[i] == MPlayerLocalID) {
					strcpy (MPlayerNames[i], MPlayerName);
				} else {
					strcpy (MPlayerNames[i], Ipx.Connection_Name(MPlayerID[i]));
				}
			}
		}
		/*---------------------------------------------------------------------
		Wait a while, polling the IPX service routines, to give our ACK
		a chance to get to the other system.  If he doesn't get our ACK, he'll
		be waiting the whole time we load MIX files.
		---------------------------------------------------------------------*/
		i = MAX(Ipx.Global_Response_Time() * 2, (unsigned long)120);
		starttime = TickCount.Time();
		while (TickCount.Time() - starttime < (unsigned)i) {
			Ipx.Service();
		}
	}

	/*------------------------------------------------------------------------
	Init network timing values, using previous response times as a measure
	of what our retry delta & timeout should be.
	------------------------------------------------------------------------*/
	Ipx.Set_Timing (Ipx.Global_Response_Time() + 2, -1,
		Ipx.Global_Response_Time() * 4);

	/*------------------------------------------------------------------------
	Clear all lists
	------------------------------------------------------------------------*/
	Clear_Game_List(&gamelist);
	Clear_Player_List(&playerlist);

	/*------------------------------------------------------------------------
	Restore screen
	------------------------------------------------------------------------*/
	Hide_Mouse();
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Show_Mouse();

	if (rc != -1){
		Wait_For_Focus();
	}

	return(rc);
}



#endif