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
 *                    File Name : TENMGR.CPP                               * 
 *                                                                         * 
 *                   Programmer : Bill R. Randolph                         * 
 *                                                                         * 
 *                   Start Date : 06/26/96                                 * 
 *                                                                         * 
 *                  Last Update : July 22, 1996 [BRR]                      * 
 *                                                                         * 
 *-------------------------------------------------------------------------* 
 * Functions:                                                              * 
 *   TenConnManClass::TenConnManClass -- Class constructor                 * 
 *   TenConnManClass::~TenConnManClass -- Class destructor                 * 
 *   TenConnManClass::Init -- Inits TEN                                    * 
 *   TenConnManClass::Service -- Service routine                           * 
 *   TenConnManClass::Send_Private_Message -- Sends a "private" message    * 
 *   TenConnManClass::Get_Private_Message -- Gets the next private message	*
 *   TenConnManClass::Send_Global_Message -- Sends a "global" message      * 
 *   TenConnManClass::Get_Global_Message -- Gets next global message			*
 *   TenConnManClass::Num_Connections -- Reports # connections             * 
 *   TenConnManClass::Connection_ID -- Reports a connection's ID           * 
 *   TenConnManClass::Connection_Index -- Gets a connection's index        * 
 *   TenConnManClass::Create_Connection -- Creates a new connection        * 
 *   TenConnManClass::Delete_Connection -- Deletes a connection            * 
 *   TenConnManClass::Connection_Name -- Reports a connection's name       * 
 *   TenConnManClass::Connection_Address -- Gets a connection's "address"  * 
 *   TenConnManClass::Global_Num_Send -- Reports # outgoing packets        * 
 *   TenConnManClass::Global_Num_Receive -- Reports # incoming packets     * 
 *   TenConnManClass::Private_Num_Send -- Reports # outgoing packets       * 
 *   TenConnManClass::Private_Num_Receive -- Reports # incoming packets    * 
 *   TenConnManClass::Flush_All -- Flushes all packets                     * 
 *   TenConnManClass::Reset_Response_Time -- Does nothing                  * 
 *   TenConnManClass::Response_Time -- Reports response time               * 
 *   TenConnManClass::Set_Timing -- Does nothing                           * 
 *   TenConnManClass::Configure_Debug -- Does nothing                      * 
 *   TenConnManClass::Mono_Debug_Print -- Does nothing                     * 
 *   terminateApp -- Callback: app terminates on error                     * 
 *   debugMessage -- outputs debug message                                 * 
 *   doAlert -- Outputs debug message                                      * 
 *   doPregameHook -- Callback: game is starting                           * 
 *   doIncomingPacket -- Callback: packet has arrived                      * 
 *   doPlayerJoins -- Callback: player joins                               * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "function.h"

#ifdef WIN32
#define WINDOWS
#endif

#if(TEN)
#include "ten.h"

//**************************************************************************
// Constants
//
const unsigned char kGlobalChannelFlag	= 0x80;
const int kMaxPlayers = 8;

//**************************************************************************
// Functions
//
static void terminateApp(void);
static void debugMessage(int msgLevel, char *msg);
static void doAlert(int, int, char *);
static void doPregameHook(char * joinType,  char *,  char *, 
	char *,  char *,  char *);
void doIncomingPacket(int addr, void *buf, size_t size);
void doPlayerEntered(int pid, int isYou, char *, char *, char *, long , char *);


//**************************************************************************
// Globals
//
static int IgnoreIncoming = 0;

/*************************************************************************** 
 * TenConnManClass::TenConnManClass -- Class constructor                   * 
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
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
TenConnManClass::TenConnManClass(void)
{
	int i;

	IsHost = 0;

	GlobalQueue = new CommBufferClass(1, 50, sizeof(GlobalPacketType), 4);
	PrivateQueue = new CommBufferClass(1, 50, Session.TenSize, 4);

	NumConnections = 0;
	for (i = 0; i < MAX_PLAYERS; i++) {
		Connections[i] = 0;
		ID[i] = 0;
		Names[i][0] = 0;
	}

}	// end of TenConnManClass


/*************************************************************************** 
 * TenConnManClass::~TenConnManClass -- Class destructor                   * 
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
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
TenConnManClass::~TenConnManClass()
{
	tenArExitArena();

	delete GlobalQueue;
	delete PrivateQueue;

}	// end of ~TenConnManClass


/*************************************************************************** 
 * TenConnManClass::Init -- Inits TEN                                      * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		none.																						*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		1.																							*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Init(void)
{
	//
	// set the debugging functions
	//
	setExitRoutine(terminateApp);
	setDebugMsgRoutine(debugMessage);

	//
	// callback function addresses
	//
	tenArSetAlertMessageRoutine(doAlert);

	tenArSetPregameHookRoutine(doPregameHook);

	tenArSetIncomingPacketRoutine(doIncomingPacket);

	tenArSetPlayerEnteredRoutine(doPlayerEntered);

	verifyNoErr(tenArInitArena("redalt"));	

	return (1);

}	// end of Init


/*************************************************************************** 
 * TenConnManClass::Service -- Service routine                             * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		none.																						*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		1.																							*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Service(void)
{
	tenArIdleArena();

	return (1);

}	// Service


/*************************************************************************** 
 * TenConnManClass::Send_Private_Message -- Sends a "private" message      * 
 *                                                                         * 
 * If Connection ID is -1, the packet is multicast; otherwise, it's sent	*
 * to only the specified connection.													*
 *                                                                         * 
 * Private & Global messages are sent via the same mechanism.  The only		*
 * way to tell the difference between them is that the Global Channel		*
 * packets have the 'kGlobalChannelFlag' bit set in the 1st byte (the		*
 * "Type" field for EventClass's, and the NetCommand field for Global		*
 * packets).																					*
 *                                                                         * 
 * INPUT:                                                                  * 
 *		buf			packet to send															*
 *		buflen		size of packet															*
 *		reliable		1 = must be delivered reliably									*
 *		conn_id		connection ID to send to, -1 = all								*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		1 = OK, 0 = error																		*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Send_Private_Message(void *buf, int buflen, 
	int reliable, int conn_id)
{
	int doBroadcast = conn_id == -1;
	unsigned char *ucbuf = (unsigned char *)buf;

	//
	// Ensure the global channel flag isn't set on this outgoing packet
	//
	(void)verify(!(ucbuf[0] & kGlobalChannelFlag));	

	if (doBroadcast)
	{
		if (reliable)
		{
			verifyNoErr(tenArSendToOtherPlayers(buf, buflen));
		}
		else
		{
			verifyNoErr(tenArUnreliableSendToOtherPlayers(buf, buflen));
		}
	}
	else
	{
		int pid = Connection_Address(conn_id);

		(void)verify(pid >= 0 && pid < kMaxPlayers);
		if (reliable)
		{
			verifyNoErr(tenArSendToPlayer(pid, buf, buflen));
		}
		else
		{
			verifyNoErr(tenArUnreliableSendToPlayer(pid, buf, buflen));
		}
	}
	return (1);
	
}	// end of Send_Private_Message


/*************************************************************************** 
 * TenConnManClass::Get_Private_Message -- Gets the next private message	*
 *                                                                         * 
 * Retrieves the next-available "private" message, if there is one.			*
 *                                                                         * 
 * INPUT:                                                                  * 
 *		buf			packet retrieved														*
 *		buflen		length of packet														*
 *		conn_id		ptr to store sender's connection ID								*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		1 = OK, 0 = error																		*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Get_Private_Message(void *buf, int *buflen, 
	int *conn_id)
{
	int addr;
	int addrlen;
	int i;

	if (PrivateQueue->Num_Receive() > 0) {
		PrivateQueue->UnQueue_Receive(buf, buflen, 0, &addr, &addrlen);
		(void)verify(addrlen == 4);
		(*conn_id) = CONNECTION_NONE;
		for (i = 0; i < NumConnections; i++) {
			if (addr == Connections[i]) {
				(*conn_id) = ID[i]; 
				return (1);
			}
		}
	}

	return (0);

}	// end of Get_Private_Message


/*************************************************************************** 
 * TenConnManClass::Send_Global_Message -- Sends a "global" message        * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		buf			packet to send															*
 *		buflen		length of packet														*
 *		reliable		1 = send reliably														*
 *		address		address to send to; -1 = broadcast it (for TEN, this		*
 *						means send to all connected players).							*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		1 = OK, 0 = error																		*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Send_Global_Message(void *buf, int buflen, 
	int reliable, int address)
{
	int doBroadcast = address == -1;
	unsigned char *ucbuf = (unsigned char *)buf;

	//
	// Ensure the global channel flag isn't set on this outgoing packet
	//
	(void)verify(!(ucbuf[0] & kGlobalChannelFlag));	

	//
	// Set the global channel flag for this packet
	//
	ucbuf[0] |= kGlobalChannelFlag;

	if (doBroadcast)
	{
		if (reliable)
		{
			verifyNoErr(tenArSendToOtherPlayers(buf, buflen));
		}
		else
		{
			verifyNoErr(tenArUnreliableSendToOtherPlayers(buf, buflen));
		}
	}
	else
	{
		int pid = address;

		(void)verify(pid >= 0 && pid < kMaxPlayers);
		if (reliable)
		{
			verifyNoErr(tenArSendToPlayer(pid, buf, buflen));
		}
		else
		{
			verifyNoErr(tenArUnreliableSendToPlayer(pid, buf, buflen));
		}
	}

	//
	// The caller may re-use this buffer, so clear the global channel flag.
	//
	ucbuf[0] &= ~kGlobalChannelFlag;

	return (1);

}	// end of Send_Global_Message


/*************************************************************************** 
 * TenConnManClass::Get_Global_Message -- Gets next global message			*
 *                                                                         * 
 * INPUT:                                                                  * 
 *		buf			buffer to store packet in											*
 *		buflen		ptr filled in with packet length									*
 *		address		ptr filled in with address (Player ID) of sender			*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		1 = OK, 0 = error																		*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Get_Global_Message(void *buf, int *buflen, 
	int *address)
{
	int addrlen;

	if (GlobalQueue->Num_Receive() > 0) {
		GlobalQueue->UnQueue_Receive(buf, buflen, 0, address, &addrlen);
		(void)verify(addrlen == 4);
		return (1);
	}

	return (0);

}	// end of Get_Global_Message


/*************************************************************************** 
 * TenConnManClass::Num_Connections -- Reports # connections               * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		none.																						*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		# connections																			*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *                                                                         * 
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Num_Connections(void)
{
	return (NumConnections);

}	// end of Num_Connections


/*************************************************************************** 
 * TenConnManClass::Connection_ID -- Reports a connection's ID             * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		index			index of connection to report										*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		connection ID for this connection												*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Connection_ID(int index)
{
	if (index >= 0 && index < NumConnections) {
		return(ID[index]);
	} else {
		return(CONNECTION_NONE);
	}

}	// end of Connection_ID


/*************************************************************************** 
 * TenConnManClass::Connection_Index -- Gets a connection's index          * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		id			Connection ID to find index for										*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		index for that connection															*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Connection_Index(int id)
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		if (ID[i] == id) {
			return (i);
		}
	}

	return(CONNECTION_NONE);

}	// end of Connection_Index


/*************************************************************************** 
 * TenConnManClass::Create_Connection -- Creates a new connection          * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		id				ID of connection														*
 *		name			name of connection													*
 *		address		TEN address (player ID) to give this connection				*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		1 = OK, 0 = error
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Create_Connection(int id, char *name, int address)
{
   Connections[NumConnections] = address;
	ID[NumConnections] = id;
   strcpy(Names[NumConnections], name);
   NumConnections++;

   return (1);

}	// end of Create_Connection


/*************************************************************************** 
 * TenConnManClass::Delete_Connection -- Deletes a connection              * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		id		ID for connection to delete												*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		1 = OK, 0 = error
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Delete_Connection(int id)
{
	int i;
	int idx = Connection_Index(id);
	if (idx == CONNECTION_NONE) {
		return 0;
	}

	for (i = idx; i < NumConnections - 1; i++) {
		Connections[i] = Connections[i+1];
		ID[i] = ID[i + 1];
		strcpy (Names[i], Names[i + 1]);
	}

   NumConnections--;

   return (1);

}	// end of Delete_Connection


/*************************************************************************** 
 * TenConnManClass::Connection_Name -- Reports a connection's name         * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		id		ID of connection to report													*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		connection name																		*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
char * TenConnManClass::Connection_Name(int id)
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		if (ID[i]==id) {
			return(Names[i]);
		}
	}

	return(NULL);

}	// end of Connection_Name


/*************************************************************************** 
 * TenConnManClass::Connection_Address -- Gets a connection's "address"    * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		id		ID of connection to report													*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		connection "address" (TEN player ID)											*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Connection_Address(int id)
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		if (ID[i]==id) {
			return(Connections[i]);
		}
	}

	return(NULL);

}	// end of Connection_Address


/*************************************************************************** 
 * TenConnManClass::Global_Num_Send -- Reports # outgoing packets          * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		none.																						*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		0.																							*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Global_Num_Send(void)
{
	return(0);

}	// end of Global_Num_Send

/*************************************************************************** 
 * TenConnManClass::Global_Num_Receive -- Reports # incoming packets       * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		none.																						*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		# packets waiting to be read														*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Global_Num_Receive(void)
{
	return (GlobalQueue->Num_Receive());

}	// end of Global_Num_Receive

/*************************************************************************** 
 * TenConnManClass::Private_Num_Send -- Reports # outgoing packets         * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		none.																						*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		0.																							*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Private_Num_Send(int /*id*/)
{
	return(0);

}	// end of Private_Num_Send

/*************************************************************************** 
 * TenConnManClass::Private_Num_Receive -- Reports # incoming packets      * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		none.																						*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		# packets waiting to be read														*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
int TenConnManClass::Private_Num_Receive(int /*id*/)
{
	return (PrivateQueue->Num_Receive());

}	// end of Private_Num_Receive


/*************************************************************************** 
 * TenConnManClass::Flush_All -- Flushes all packets                       * 
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
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
void TenConnManClass::Flush_All(void)
{
	int i;
	int maxqueuesize;
	int rc;

	//
	// Set the max # of packets that Ten will send me during any given
	// call to tenArIdleArena() to slightly smaller than my max queue
	// size.
	//
	maxqueuesize = 45;
	rc = tenArSetOption(kTenArOptReadQueueSize, &maxqueuesize, 
		sizeof(maxqueuesize));

	verifyNoErr(rc);


	//
	// Set the flag to tell the doIncomingPacket routine to ignore packets.
	// (doIncomingPacket() is called by tenArIdleArena().)
	//
	IgnoreIncoming = 1;

	while (i++ < 1000) {
		tenArIdleArena();
		if (GlobalQueue->Num_Receive() == 0 && 
			PrivateQueue->Num_Receive() == 0) {
			break;
		}
		GlobalQueue->Init();
		PrivateQueue->Init();
	}

	IgnoreIncoming = 0;

}	// end of Flush_All


/*************************************************************************** 
 * TenConnManClass::Reset_Response_Time -- Does nothing                    * 
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
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
void TenConnManClass::Reset_Response_Time(void)
{
	//
	// (This function intentionally left blank.)
	//

}	// end of Reset_Response_Time


/*************************************************************************** 
 * TenConnManClass::Response_Time -- Reports response time                 * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		none.																						*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		worst-case connection response time (round-trip)							*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
unsigned long TenConnManClass::Response_Time(void)
{
   return((Session.NetResponseTime * 60) / 1000);   // 300 milliseconds one way

}	// end of Response_Time


/*************************************************************************** 
 * TenConnManClass::Set_Timing -- Does nothing                             * 
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
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
void TenConnManClass::Set_Timing(unsigned long /*retrydelta*/, 
	unsigned long /*maxretries*/, unsigned long /*timeout*/)
{
	//
	// (This function intentionally left blank.)
	//

}	// end of Set_Timing


/*************************************************************************** 
 * TenConnManClass::Configure_Debug -- Does nothing                        * 
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
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
void TenConnManClass::Configure_Debug(int /*index*/, int /*type_offset*/, 
	int /*type_size*/, char **/*names*/, int /*namestart*/, int /*namecount*/)
{
	//
	// (This function intentionally left blank.)
	//

}	// end of Configure_Debug

/*************************************************************************** 
 * TenConnManClass::Mono_Debug_Print -- Does nothing                       * 
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
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
void TenConnManClass::Mono_Debug_Print(int /*index*/, int /*refresh*/)
{
	//
	// (This function intentionally left blank.)
	//

}	// end of Mono_Debug_Print

/*************************************************************************** 
 * terminateApp -- Callback: app terminates on error                       * 
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
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
static void terminateApp(void)
{
	Prog_End();
	dprintf("Exiting due to a fatal error.\n");
	exit(0);

}	// end of terminateApp


/*************************************************************************** 
 * debugMessage -- outputs debug message                                   * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		msgLevel			not used																*
 *		msg				message to print													*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		none.																						*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
static void debugMessage(int /*msgLevel*/, char *msg)
{
	static int recurse = 0;

	if (recurse) {
		return;
	}

	recurse = 1;

	if (MonoClass::Is_Enabled()) {
		Mono_Printf("%s\n",msg);
	} else {
		//printf("%s\n",msg);
		FILE *fp;
		fp = fopen("tendebug.log","at");
		if (fp) {
			fprintf(fp,"%s\n",msg);
			fclose(fp);
		}
	}

	if (GameActive) {
		WWMessageBox().Process(msg);
	}

	recurse = 0;

}	// end of debugMessage


/*************************************************************************** 
 * doAlert -- Outputs debug message                                        * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		msg			message to print														*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		none.																						*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
static void doAlert(int, int, char * msg)
{
	static int recurse = 0;

	if (recurse) {
		return;
	}

	recurse = 1;

	if (MonoClass::Is_Enabled()) {
		Mono_Printf("%s\n",msg);
	} else {
		//printf("%s\n",msg);
		FILE *fp;
		fp = fopen("tenalert.log","at");
		if (fp) {
			fprintf(fp,"%s\n",msg);
			fclose(fp);
		}
	}

	if (GameActive) {
		WWMessageBox().Process(msg);
	}
	
	recurse = 0;

}	// end of doAlert


/*************************************************************************** 
 * doPregameHook -- Callback: game is starting                             * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		joinType		if "create", we're the game host									*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		none.																						*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
static void doPregameHook(char * joinType,  char *,  char *, 
	char *,  char *,  char *)
{
	char typeToken[16];

	sscanf(joinType, "%s", typeToken);
	if (!strcmp(typeToken, "create")) {
		Ten->IsHost = 1;
	}

	if (Ten->IsHost) {
		verifyNoErr(tenArReturnGameOptions(""));
	}

	verifyNoErr(tenArReturnPlayerOptions(""));

}	// end of doPregameHook


/*************************************************************************** 
 * doIncomingPacket -- Callback: packet has arrived                        * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		addr			player ID of sender													*
 *		buf			buffer containing incoming packet								*
 *		size			size of incoming packet												*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *                                                                         * 
 * WARNINGS:                                                               * 
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
void doIncomingPacket(int addr, void *buf, size_t size)
{
	int rc;
	unsigned char *byte;

	//
	// Check to see if this packet belongs to the Global Channel or not
	//
	byte = (unsigned char *)buf;

	//
	// If the global channel flag is set in this packet, queue it onto the 
	// Global Channel queue.  Ignore any errors if "IgnoreIncoming" is set,
	// which means we're in the process of flushing the queues.
	//
	if (byte[0] & kGlobalChannelFlag) {
		byte[0] &= (~kGlobalChannelFlag);
		rc = Ten->GlobalQueue->Queue_Receive(buf, size, &addr, 4);
		if (!IgnoreIncoming) {
			(void)verify(rc==1);
		}
	} else {
		rc = Ten->PrivateQueue->Queue_Receive(buf, size, &addr, 4);
		if (!IgnoreIncoming) {
			(void)verify(rc==1);
		}
	}

}	// end of doIncomingPacket


/*************************************************************************** 
 * doPlayerJoins -- Callback: player joins                                 * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *		pid		player ID of the player joining										*
 *		isYou		true = this is you														*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		none.																						*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   07/22/1996 BRR : Created.                                             * 
 *=========================================================================*/
void doPlayerEntered(int pid, int isYou, char * /*options*/, 
	char */*termOptions*/, char */*address*/, long /*uniqueId*/, 
	char */*joinType*/)
{
	if (isYou) {
		Session.TenPlayerID = pid;
	}

}	// end of doPlayerJoins


#endif	//TEN
/************************** end of tenmgr.cpp ******************************/
