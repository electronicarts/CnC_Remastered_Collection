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

/* $Header:   F:\projects\c&c\vcs\code\ipxmgr.cpv   1.9   16 Oct 1995 16:48:22   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPXMGR.CPP                               *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 20, 1994                        *
 *                                                                         *
 *                  Last Update : May 4, 1995 [BRR]                        *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                       			*
 *   IPXManagerClass::IPXManagerClass -- class constructor                 *
 *   IPXManagerClass::~IPXManagerClass -- class destructor                 *
 *   IPXManagerClass::Init -- initialization routine								*
 *   IPXManagerClass::Is_IPX -- tells if IPX is installed or not				*
 *   IPXManagerClass::Set_Timing -- sets timing for all connections			*
 *   IPXManagerClass::Create_Connection -- creates a new connection        *
 *   IPXManagerClass::Delete_Connection -- deletes a connection            *
 *   IPXManagerClass::Num_Connections -- gets the # of connections			*
 *   IPXManagerClass::Connection_ID -- gets the given connection's ID      *
 *   IPXManagerClass::Connection_Name -- gets name for given connection    *
 *   IPXManagerClass::Connection_Address -- retrieves connection's address	*
 *   IPXManagerClass::Connection_Index -- gets given connection's index    *
 *   IPXManagerClass::Send_Global_Message -- sends a Global Message			*
 *   IPXManagerClass::Get_Global_Message -- polls the Global Message queue	*
 *   IPXManagerClass::Send_Private_Message -- Sends a Private Message		*
 *   IPXManagerClass::Get_Private_Message -- Polls Private Message queue	*
 *   IPXManagerClass::Service -- main polling routine for IPX Connections	*
 *   IPXManagerClass::Get_Bad_Connection -- returns bad connection ID      *
 *   IPXManagerClass::Global_Num_Send	-- gets # entries in send queue	   *
 *   IPXManagerClass::Global_Num_Receive -- gets # entries in recv queue   *
 *   IPXManagerClass::Private_Num_Send -- gets # entries in send queue	   *
 *   IPXManagerClass::Private_Num_Receive -- gets # entries in recv queue  *
 *   IPXManagerClass::Set_Bridge -- prepares to cross a bridge             *
 *   IPXManagerClass::Set_Socket -- sets socket ID for all connections		*
 *   IPXManagerClass::Response_Time -- Returns largest Avg Response Time   *
 *   IPXManagerClass::Global_Response_Time -- Returns Avg Response Time    *
 *   IPXManagerClass::Reset_Response_Time -- Reset response time 				*
 *   IPXManagerClass::Oldest_Send -- gets ptr to oldest send buf           *
 *   IPXManagerClass::Mono_Debug_Print -- debug output routine					*
 *   IPXManagerClass::Alloc_RealMode_Mem -- allocates real-mode memory		*
 *   IPXManagerClass::Free_RealMode_Mem -- frees real-mode memory				*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "ipx95.h"
#include "tcpip.h"

/***************************************************************************
 * IPXManagerClass::IPXManagerClass -- class constructor                   *
 *                                                                         *
 * INPUT:                                                                  *
 *		glb_maxlen		Global Channel maximum packet length						*
 *		pvt_maxlen		Private Channel maximum packet length						*
 *		socket			socket ID to use													*
 *		product_id		a unique numerical ID for this product						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		The socket number is byte-swapped, since IPX requires socket ID's		*
 *		to be stored high/low.																*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
IPXManagerClass::IPXManagerClass (int glb_maxlen, int pvt_maxlen,
	int glb_num_packets, int pvt_num_packets, unsigned short socket,
	unsigned short product_id)
{
	int i;

	/*------------------------------------------------------------------------
	Initialize data members
	------------------------------------------------------------------------*/
	/*........................................................................
	IPXStatus = 1 if IPX is installed, 0 if not
	........................................................................*/
	if (IPX_SPX_Installed()==0) {
		IPXStatus = 0;
	} else {
		IPXStatus = 1;
	}

	/*........................................................................
	Set listening state flag to off
	........................................................................*/
	Listening = 0;

	/*........................................................................
	No memory has been alloc'd yet
	........................................................................*/
	RealMemAllocd = 0;

	/*........................................................................
	Set max packet sizes, for allocating real-mode memory
	........................................................................*/
	Glb_MaxPacketLen = glb_maxlen;
	Glb_NumPackets = glb_num_packets;
	Pvt_MaxPacketLen = pvt_maxlen;
	Pvt_NumPackets = pvt_num_packets;

	/*........................................................................
	Save the app's product ID
	........................................................................*/
	ProductID = product_id;

	/*........................................................................
	Save our socket ID number
	........................................................................*/
	Socket = (unsigned short)( (((unsigned long)socket & 0x00ff) << 8) |
		(((unsigned long)socket & 0xff00) >> 8));

	/*........................................................................
	Get the user's IPX local connection number
	........................................................................*/
	if (IPXStatus) {
		ConnectionNum = IPX_Get_Connection_Number();
	} else {
		ConnectionNum = 0;
	}

	/*........................................................................
	Init connection states
	........................................................................*/
	NumConnections = 0;
	CurConnection = 0;
	for (i = 0; i < CONNECT_MAX; i++)
		Connection[i] = 0;
	GlobalChannel = 0;

	SendOverflows = 0;
	ReceiveOverflows = 0;
	BadConnection = IPXConnClass::CONNECTION_NONE;

	/*........................................................................
	Init timing parameters
	........................................................................*/
	RetryDelta = 2;		// 2 ticks between retries
	MaxRetries = -1;		// disregard # retries
	Timeout = 60;			// report bad connection after 1 second

}	/* end of IPXManagerClass */


/***************************************************************************
 * IPXManagerClass::~IPXManagerClass -- class destructor                   *
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
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
IPXManagerClass::~IPXManagerClass()
{
	int i;

	/*------------------------------------------------------------------------
	Stop all IPX events
	------------------------------------------------------------------------*/
	if (Listening) {
		IPXConnClass::Stop_Listening();
		Listening = 0;
	}

	/*------------------------------------------------------------------------
	Free all protected-mode memory
	------------------------------------------------------------------------*/
	if (GlobalChannel) {
		delete GlobalChannel;
		GlobalChannel = 0;
	}
	for (i = 0; i < NumConnections; i++) {
		delete Connection[i];
		Connection[i] = 0;
	}
	NumConnections = 0;

	/*------------------------------------------------------------------------
	Free all real-mode memory
	------------------------------------------------------------------------*/
	if (RealMemAllocd) {
		Free_RealMode_Mem();
		RealMemAllocd = 0;
	}

}	/* end of ~IPXManagerClass */


/***************************************************************************
 * IPXManagerClass::Init -- initialization routine									*
 *                                                                         *
 * This routine allocates memory,
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
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Init()
{
	int i;

	if (!(GameToPlay == GAME_INTERNET)){
		/*
		----------------------- Error if IPX not installed -----------------------
		*/
		if (!IPXStatus) {
			return(false);
		}

		/*------------------------------------------------------------------------
		Stop Listening
		------------------------------------------------------------------------*/
		if (Listening) {
			IPXConnClass::Stop_Listening();
			Listening = 0;
		}

		/*------------------------------------------------------------------------
		Free Real-mode memory
		------------------------------------------------------------------------*/
		if (RealMemAllocd) {
			Free_RealMode_Mem();
			RealMemAllocd = 0;
		}
	}else{
		/*
		** Pretend IPX is available for Internet games whether it is or not
		*/
		IPXStatus = 1;
	}

	/*------------------------------------------------------------------------
	Free protected-mode memory
	------------------------------------------------------------------------*/
	if (GlobalChannel) {
		delete GlobalChannel;
		GlobalChannel = 0;
	}
	for (i = 0; i < NumConnections; i++) {
		delete Connection[i];
		Connection[i] = 0;
	}
	NumConnections = 0;


	if (!(GameToPlay == GAME_INTERNET)){
		/*------------------------------------------------------------------------
		Allocate real-mode memory
		------------------------------------------------------------------------*/
		if (!Alloc_RealMode_Mem()) return(false);
		RealMemAllocd = 1;
	}

	/*------------------------------------------------------------------------
	Allocate the Global Channel
	------------------------------------------------------------------------*/
	GlobalChannel = new IPXGlobalConnClass (Glb_NumPackets, Glb_NumPackets,
		Glb_MaxPacketLen, ProductID);
	if (!GlobalChannel)
		return(false);
	GlobalChannel->Init();
	GlobalChannel->Set_Retry_Delta (RetryDelta);
	GlobalChannel->Set_Max_Retries (MaxRetries);
	GlobalChannel->Set_TimeOut (Timeout);

	/*------------------------------------------------------------------------
	Configure the IPX Connections
	------------------------------------------------------------------------*/
	IPXConnClass::Configure(Socket, ConnectionNum, ListenECB, SendECB,
		FirstHeaderBuf, SendHeader, FirstDataBuf, SendBuf, Handler, PacketLen);

	/*------------------------------------------------------------------------
	Start Listening
	------------------------------------------------------------------------*/
	if (!(GameToPlay == GAME_INTERNET)){
		if (!IPXConnClass::Start_Listening()) return(false);
	}

	Listening = 1;

	return(true);
}


/***************************************************************************
 * IPXManagerClass::Is_IPX -- tells if IPX is installed or not					*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = IPX is installed; 0 = isn't													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Is_IPX(void)
{
	return(IPXStatus);

}	/* end of Is_IPX */


/***************************************************************************
 * IPXManagerClass::Set_Timing -- sets timing for all connections				*
 *                                                                         *
 * This will set the timing parameters for all existing connections, and	*
 * all connections created from now on.  This allows an application to		*
 * measure the Response_Time while running, and adjust timing accordingly.	*
 *                                                                         *
 * INPUT:                                                                  *
 *		retrydelta	value to set for retry delta 										*
 *		maxretries	value to set for max # retries									*
 *		timeout		value to set for connection timeout								*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   07/02/1995 BR : Created.                                              *
 *=========================================================================*/
void IPXManagerClass::Set_Timing (unsigned long retrydelta,
	unsigned long maxretries, unsigned long timeout)
{
	int i;

	RetryDelta = retrydelta;
	MaxRetries = maxretries;
	Timeout = timeout;

	if (GlobalChannel) {
		GlobalChannel->Set_Retry_Delta (RetryDelta);
		GlobalChannel->Set_Max_Retries (MaxRetries);
		GlobalChannel->Set_TimeOut (Timeout);
	}

	for (i = 0; i < NumConnections; i++) {
		Connection[i]->Set_Retry_Delta (RetryDelta);
		Connection[i]->Set_Max_Retries (MaxRetries);
		Connection[i]->Set_TimeOut (Timeout);
	}

}	/* end of Set_Timing */


/***************************************************************************
 * IPXManagerClass::Create_Connection -- creates a new connection          *
 *                                                                         *
 * INPUT:                                                                  *
 *		id				application-specific numerical ID for this connection		*
 *		node			ptr to IPXNodeIDType (name & address)							*
 *		address		IPX address for this connection									*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Never create a connection with an 'id' of -1.								*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
bool IPXManagerClass::Create_Connection(int id, char *name, IPXAddressClass *address)
{
#if (0)
	char temp[80];

	NetNumType num;
	NetNodeType node;

	address->Get_Address (num, node);

	sprintf (temp, "Address:%02x %02x %02x %02x %02x %02x\n", node[0],
														  node[1],
														  node[2],
														  node[3],
														  node[4],
														  node[5]);
	CCDebugString (temp);

	sprintf (temp, "Network:%02x %02x %02x %02x\n", num[0],
														  num[1],
														  num[2],
														  num[3]);
	CCDebugString (temp);
#endif	//(0)

	/*
	----------------------- Error if IPX not installed -----------------------
	*/
	if (!IPXStatus)
		return(false);

	/*
	------------------------- Error if no more room --------------------------
	*/
	if (NumConnections==CONNECT_MAX)
		return(false);

	/*
	------------------------- Create new connection --------------------------
	*/
	Connection[NumConnections] = new IPXConnClass(Pvt_NumPackets,
		Pvt_NumPackets, Pvt_MaxPacketLen, ProductID, address, id, name);
	if (!Connection[NumConnections])
		return(false);

	Connection[NumConnections]->Init ();
	Connection[NumConnections]->Set_Retry_Delta (RetryDelta);
	Connection[NumConnections]->Set_Max_Retries (MaxRetries);
	Connection[NumConnections]->Set_TimeOut (Timeout);

	NumConnections++;

	return(true);

}	/* end of Create_Connection */


/***************************************************************************
 * IPXManagerClass::Delete_Connection -- deletes a connection              *
 *                                                                         *
 * INPUT:                                                                  *
 *		id		ID of connection to delete													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
bool IPXManagerClass::Delete_Connection(int id)
{
	int i,j;

	/*
	----------------------- Error if IPX not installed -----------------------
	*/
	if (!IPXStatus)
		return(false);

	/*
	------------------- Error if no connections to delete --------------------
	*/
	if (NumConnections==0)
		return(false);

	/*
	---------------------- Loop through all connections ----------------------
	*/
	for (i = 0; i < NumConnections; i++) {
		/*
		........................ If a match, delete it ........................
		*/
		if (Connection[i]->ID==id) {
			delete Connection[i];
			/*
			................ Move array elements back one index ................
			*/
			for (j = i; j < NumConnections - 1; j++) {
				Connection[j] = Connection[j+1];
			}
			/*
			......................... Adjust counters ..........................
			*/
			NumConnections--;
			if (CurConnection >= NumConnections)
				CurConnection = 0;
			return(true);
		}
	}

	/*
	---------------------------- No match; error -----------------------------
	*/
	return(false);

}	/* end of Delete_Connection */


/***************************************************************************
 * IPXManagerClass::Num_Connections -- gets the # of connections				*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		# of connections																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Num_Connections(void)
{
#ifdef VIRTUAL_SUBNET_SERVER
	/*
	** If we are connected to the VSS then dont coumt that in the number of connections.
	*/
	if (Connection_Index (VSS_ID) == IPXConnClass::CONNECTION_NONE){
		return(NumConnections);
	}else{
		return(NumConnections -1);
	}
#else	// VIRTUAL_SUBNET_SERVER

	return(NumConnections);

#endif	//VIRTUAL_SUBNET_SERVER

}	/* end of Num_Connections */


/***************************************************************************
 * IPXManagerClass::Connection_ID -- gets the given connection's ID        *
 *                                                                         *
 * INPUT:                                                                  *
 *		index			index of connection to retrieve									*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ID for that connection, CONNECTION_NONE if invalid index					*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Connection_ID(int index)
{
	if (index >= 0 && index < NumConnections) {
		return(Connection[index]->ID);
	} else {
		return(IPXConnClass::CONNECTION_NONE);
	}
}


/***************************************************************************
 * IPXManagerClass::Connection_Name -- retrieves name for given connection *
 *                                                                         *
 * INPUT:                                                                  *
 *		id		ID of connection to get name of											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ptr to connection's name, NULL if not found									*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/19/1995 BR : Created.                                              *
 *=========================================================================*/
char *IPXManagerClass::Connection_Name(int id)
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		if (Connection[i]->ID==id) {
			return(Connection[i]->Name);
		}
	}

	return(NULL);

}	/* end of Connection_Name */


/***************************************************************************
 * IPXManagerClass::Connection_Address -- retrieves connection's address	*
 *                                                                         *
 * INPUT:                                                                  *
 *		id		ID of connection to get address for										*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		pointer to IXPAddressClass, NULL if not found								*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/19/1995 BR : Created.                                              *
 *=========================================================================*/
IPXAddressClass * IPXManagerClass::Connection_Address(int id)
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		if (Connection[i]->ID==id) {
			return(&Connection[i]->Address);
		}
	}

	return(NULL);

}	/* end of Connection_Address */


/***************************************************************************
 * IPXManagerClass::Connection_Index -- gets given connection's index      *
 *                                                                         *
 * INPUT:                                                                  *
 *		ID to retrieve index for															*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		index for this connection, CONNECTION_NONE if not found					*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Connection_Index(int id)
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		if (Connection[i]->ID==id)
			return(i);
	}

	return(IPXConnClass::CONNECTION_NONE);

}	/* end of Connection_Index */


/***************************************************************************
 * IPXManagerClass::Send_Global_Message -- sends a Global Message				*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to send															*
 *		buflen		length of buf															*
 *		ack_req		1 = ACK required; 0 = no ACK required							*
 *		address		address to send to; NULL = broadcast							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Send_Global_Message(void *buf, int buflen,
	int ack_req, IPXAddressClass *address)
{
	int rc;

	/*
	------------ Error if IPX not installed or not Listening -----------------
	*/
	if (!IPXStatus || !Listening) return(false);

	rc = GlobalChannel->Send_Packet (buf, buflen, address, ack_req);
	if (!rc)
		SendOverflows++;

	return(rc);
}


/***************************************************************************
 * IPXManagerClass::Get_Global_Message -- polls the Global Message queue	*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf				buffer to store received packet								*
 *		buflen			length of data placed in 'buf'								*
 *		address			IPX address of sender											*
 *		product_id		product ID of sender												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Get_Global_Message(void *buf, int *buflen,
	IPXAddressClass *address, unsigned short *product_id)
{
	/*
	------------ Error if IPX not installed or not Listening -----------------
	*/
	if (!IPXStatus || !Listening) return(false);

	return(GlobalChannel->Get_Packet (buf, buflen, address, product_id));
}


/***************************************************************************
 * IPXManagerClass::Send_Private_Message -- Sends a Private Message			*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to send															*
 *		buflen		length of 'buf'														*
 *		conn_id		connection ID to send to (CONNECTION_NONE = all)			*
 *		ack_req		1 = ACK required; 0 = no ACK required							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Send_Private_Message(void *buf, int buflen, int ack_req,
	int conn_id)
{
	int i;						// loop counter
	int connect_idx;			// index of channel to send to, if specified

	/*
	------------ Error if IPX not installed or not Listening -----------------
	*/
	if (!IPXStatus || !Listening || (NumConnections==0)) return(false);

	/*------------------------------------------------------------------------
	Send the message to all connections
	------------------------------------------------------------------------*/
	if (conn_id==IPXConnClass::CONNECTION_NONE) {

		/*
		** If this is an internet game and no ack is reqired then we only need to send
		**	 the packet to the VSS and it will forward it to all the other players.
		*/

#ifdef VIRTUAL_SUBNET_SERVER
		if (ack_req || (!Winsock.Get_Connected() || !UseVirtualSubnetServer)){
#endif	//VIRTUAL_SUBNET_SERVER
			/*.....................................................................
			Check for room in all connections
			.....................................................................*/
			for (i = 0; i < NumConnections; i++) {
#ifdef VIRTUAL_SUBNET_SERVER
				if (Connection[i]->ID != VSS_ID){
#endif	//VIRTUAL_SUBNET_SERVER
					if (Connection[i]->Queue->Num_Send()==Connection[i]->Queue->Max_Send()) {
						SendOverflows++;
						return(false);
					}
#ifdef VIRTUAL_SUBNET_SERVER
				}
#endif	// VIRTUAL_SUBNET_SERVER
			}

			/*.....................................................................
			Send packet to all connections
			.....................................................................*/

			for (i = 0; i < NumConnections; i++) {
#ifdef VIRTUAL_SUBNET_SERVER
				if (Connection[i]->ID != VSS_ID){
#endif	//VIRTUAL_SUBNET_SERVER
					Connection[i]->Send_Packet (buf, buflen, ack_req);
#ifdef VIRTUAL_SUBNET_SERVER
				}
#endif	//VIRTUAL_SUBNET_SERVER
			}

#ifdef VIRTUAL_SUBNET_SERVER
		}else{
			/*
			** Send the packet to the VSS with a 0 header so it gets forwarded to all players.
			*/
			if (Connection[ Connection_Index(VSS_ID) ]->Queue->Num_Send() == Connection[ Connection_Index(VSS_ID) ]->Queue->Max_Send()) {
				SendOverflows++;
				return(false);
			}
			Connection[ Connection_Index(VSS_ID) ]->Send_Packet (buf, buflen, 0);
		}
#endif	// VIRTUAL_SUBNET_SERVER

		return(true);


	} else {

		/*------------------------------------------------------------------------
		Send the message to the specified connection
		------------------------------------------------------------------------*/
		connect_idx = Connection_Index (conn_id);
		if (connect_idx == IPXConnClass::CONNECTION_NONE) {
			SendOverflows++;
			return(false);
		}

		/*.....................................................................
		Check for room in the connection
		.....................................................................*/
		if (Connection[connect_idx]->Queue->Num_Send() ==
			Connection[connect_idx]->Queue->Max_Send()) {
			SendOverflows++;
			return(false);
		}

		/*.....................................................................
		Send the packet to that connection
		.....................................................................*/
		Connection[connect_idx]->Send_Packet (buf, buflen, ack_req);
		return(true);
	}
}


/***************************************************************************
 * IPXManagerClass::Get_Private_Message -- Polls the Private Message queue	*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to store incoming packet									*
 *		buflen		length of data placed in 'buf'									*
 *		conn_id		filled in with connection ID of sender							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Get_Private_Message(void *buf, int *buflen, int *conn_id)
{
	int i;
	int rc;
	int c_id;
#ifdef VIRTUAL_SUBNET_SERVER
	int vss = 0;
#endif	// VIRTUAL_SUBNET_SERVER

	/*
	------------ Error if IPX not installed or not Listening -----------------
	*/
	if (!IPXStatus || !Listening || (NumConnections==0)) return(false);

#ifdef VIRTUAL_SUBNET_SERVER
	if (Winsock.Get_Connected()){
		vss = (int) UseVirtualSubnetServer;
	}
#endif	//VIRTUAL_SUBNET_SERVER

	/*------------------------------------------------------------------------
	Safety check: ensure CurConnection is in range.
	------------------------------------------------------------------------*/
#ifdef VIRTUAL_SUBNET_SERVER
	if (CurConnection >= NumConnections - vss)
#else	// VIRTUAL_SUBNET_SERVER
	if (CurConnection >= NumConnections)
#endif	//VIRTUAL_SUBNET_SERVER
		CurConnection = 0;

	/*------------------------------------------------------------------------
	Scan all connections for a received packet, starting with 'CurConnection'
	------------------------------------------------------------------------*/
#ifdef VIRTUAL_SUBNET_SERVER
	for (i = 0; i < NumConnections - vss; i++) {
#else	// VIRTUAL_SUBNET_SERVER
	for (i = 0; i < NumConnections; i++) {
#endif	// VIRTUAL_SUBNET_SERVER
		/*.....................................................................
		Check this connection for a packet
		.....................................................................*/
		rc = Connection[CurConnection]->Get_Packet (buf, buflen);
		c_id = Connection[CurConnection]->ID;

		/*.....................................................................
		Increment CurConnection to the next connection index
		.....................................................................*/
		CurConnection++;
#ifdef VIRTUAL_SUBNET_SERVER
		if (CurConnection >= NumConnections - vss)
#else	// VIRTUAL_SUBNET_SERVER
		if (CurConnection >= NumConnections)
#endif	// VIRTUAL_SUBNET_SERVER
			CurConnection = 0;

		/*.....................................................................
		If we got a packet, return the connection ID
		.....................................................................*/
		if (rc) {
			(*conn_id) = c_id;
			return(true);
		}
	}

	return(false);
}


/***************************************************************************
 * IPXManagerClass::Service -- main polling routine for IPX Connections		*
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
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Service(void)
{

	int rc = 1;
	int i;
	CommHeaderType *packet;
	int packetlen;
	IPXAddressClass address;



#ifndef NOT_FOR_WIN95

	unsigned char temp_receive_buffer[1024];
	int	recv_length;

	if (Winsock.Get_Connected()){

		while ((recv_length = Winsock.Read(temp_receive_buffer, 1024))!=0){

#ifdef VIRTUAL_SUBNET_SERVER
			/*
			** Get a pointer to the data header and swap the bit mask
			*/
			CurHeaderBuf = (IPXHEADER*)&temp_receive_buffer[0]; 	//NULL;
			unsigned short *swapptr = (unsigned short*)CurHeaderBuf;
			*swapptr = ntohs (*swapptr);

			CurDataBuf = (char*)&temp_receive_buffer[2];

			/*.....................................................................
			Compute the length of the packet (byte-swap the length in the IPX hdr)
			.....................................................................*/
			packetlen = recv_length-2;
#else	//VIRTUAL_SUBNET_SERVER
			CurHeaderBuf = NULL;
			CurDataBuf = (char*)&temp_receive_buffer[0];

			/*.....................................................................
			Compute the length of the packet (byte-swap the length in the IPX hdr)
			.....................................................................*/
			packetlen = recv_length;

#endif	//VIRTUAL_SUBNET_SERVER


			/*.....................................................................
			Extract the sender's address from the IPX header
			.....................................................................*/
			address.Set_Address (CurHeaderBuf);

			/*.....................................................................
			Examine the Magic Number of the received packet to determine if this
			packet goes into the Global Queue, or into one of the Private Queues
			.....................................................................*/
			packet = (CommHeaderType *)CurDataBuf;
#if (0)
char tempbuf[256];

static char pcode [4][18]={
	"PACKET_DATA_ACK",			// this is a data packet requiring an ACK
 	"PACKET_DATA_NOACK",		// this is a data packet not requiring an ACK
	"PACKET_ACK",					// this is an ACK for a packet
	"PACKET_COUNT"				// for computational purposes
};

sprintf (tempbuf, "Received packet type %d, ID=%d, code=%s, length=%d \n", CurDataBuf[sizeof(CommHeaderType)],
																						packet->PacketID,
																						pcode[packet->Code],
																						recv_length);
CCDebugString (tempbuf);
#endif	//(0)
			if (packet->MagicNumber == GlobalChannel->Magic_Num()) {
				/*..................................................................
				Put the packet in the Global Queue
				..................................................................*/
				if (!GlobalChannel->Receive_Packet (packet, packetlen, &address))
					ReceiveOverflows++;
			} else {
				if (packet->MagicNumber == ProductID) {
					/*..................................................................
					Find the Private Queue that this packet is for
					..................................................................*/
					for (i = 0; i < NumConnections; i++) {
#ifdef VIRTUAL_SUBNET_SERVER
						if (Connection[i]->Address == address && Connection[i]->ID != VSS_ID) {
#else	//VIRTUAL_SUBNET_SERVER
						if (Connection[i]->Address == address) {
#endif	//VIRTUAL_SUBNET_SERVER
							if (!Connection[i]->Receive_Packet (packet, packetlen))
								ReceiveOverflows++;
							break;
						}
					}
				}
			}

		}
	}else{

		while (IPX_Get_Outstanding_Buffer95(&temp_receive_buffer[0])){

			CurHeaderBuf = (IPXHEADER*)&temp_receive_buffer[0];
			CurDataBuf = (char*)&temp_receive_buffer[sizeof(IPXHeaderType)];

			/*.....................................................................
			Compute the length of the packet (byte-swap the length in the IPX hdr)
			.....................................................................*/
			packetlen = ((CurHeaderBuf->Length & 0xff) << 8) |
				(CurHeaderBuf->Length >> 8);
			packetlen -= sizeof(IPXHeaderType);

			/*.....................................................................
			Extract the sender's address from the IPX header
			.....................................................................*/
			address.Set_Address (CurHeaderBuf);

			/*.....................................................................
			Examine the Magic Number of the received packet to determine if this
			packet goes into the Global Queue, or into one of the Private Queues
			.....................................................................*/
			packet = (CommHeaderType *)CurDataBuf;

			if (packet->MagicNumber == GlobalChannel->Magic_Num()) {
				/*..................................................................
				Put the packet in the Global Queue
				..................................................................*/
				if (!GlobalChannel->Receive_Packet (packet, packetlen, &address))
					ReceiveOverflows++;
			} else {
				if (packet->MagicNumber == ProductID) {
					/*..................................................................
					Find the Private Queue that this packet is for
					..................................................................*/
					for (i = 0; i < NumConnections; i++) {
						if (Connection[i]->Address == address) {
							if (!Connection[i]->Receive_Packet (packet, packetlen))
								ReceiveOverflows++;
							break;
						}
					}
				}
			}

		}
	}

	//IPX_Start_Listening95();


#else //NOT_FOR_WIN95


	/*
	------------ Error if IPX not installed or not Listening -----------------
	*/
	if (!IPXStatus || !Listening)
		return(false);

	/*------------------------------------------------------------------------
	Loop until there are no more packets to process.
	------------------------------------------------------------------------*/
	for (;;) {
		/*.....................................................................
		Check the BufferFlags for the "current" buffer; if it's empty,
		break; out of the loop.
		.....................................................................*/
		if (BufferFlags[CurIndex]==0)
			break;

		/*.....................................................................
		Compute the length of the packet (byte-swap the length in the IPX hdr)
		.....................................................................*/
		packetlen = ((CurHeaderBuf->Length & 0xff) << 8) |
			(CurHeaderBuf->Length >> 8);
		packetlen -= sizeof(IPXHeaderType);

		/*.....................................................................
		Extract the sender's address from the IPX header
		.....................................................................*/
		address.Set_Address (CurHeaderBuf);

		/*.....................................................................
		Examine the Magic Number of the received packet to determine if this
		packet goes into the Global Queue, or into one of the Private Queues
		.....................................................................*/
		packet = (CommHeaderType *)CurDataBuf;
		if (packet->MagicNumber == GlobalChannel->Magic_Num()) {
			/*..................................................................
			Put the packet in the Global Queue
			..................................................................*/
			if (!GlobalChannel->Receive_Packet (packet, packetlen, &address))
				ReceiveOverflows++;
		} else {
			if (packet->MagicNumber == ProductID) {
				/*..................................................................
				Find the Private Queue that this packet is for
				..................................................................*/
				for (i = 0; i < NumConnections; i++) {
					if (Connection[i]->Address == address) {
						if (!Connection[i]->Receive_Packet (packet, packetlen))
							ReceiveOverflows++;
						break;
					}
				}
			}
		}

		/*.....................................................................
		Set the current BufferFlags to 0 (since we've cleaned out this buffer)
		.....................................................................*/
		BufferFlags[CurIndex] = 0;

		/*.....................................................................
		Go to the next packet buffer
		.....................................................................*/
		CurIndex++;
		CurHeaderBuf = (IPXHeaderType *)(((char *)CurHeaderBuf) + FullPacketLen);
		CurDataBuf = ((char *)CurDataBuf) + FullPacketLen;
		if (CurIndex >= NumBufs) {
			CurHeaderBuf = FirstHeaderBuf;
			CurDataBuf = FirstDataBuf;
			CurIndex = 0;
		}

	}

#endif //NOT_FOR_WIN95

	/*------------------------------------------------------------------------
	Service all connections.  If a connection reports that it's gone "bad",
	report an error to the caller.  If it's the Global Channel, un-queue the
	send entry that's holding things up.  This will keep the Global Channel
	from being clogged by one un-ACK'd outgoing packet.
	------------------------------------------------------------------------*/
	if (GlobalChannel) {
		if (!GlobalChannel->Service()) {
			GlobalChannel->Queue->UnQueue_Send (NULL, NULL, 0);
			rc = 0;
		}
	}
	for (i = 0; i < NumConnections; i++) {
		if (!Connection[i]->Service()) {
#ifdef VIRTUAL_SUBNET_SERVER
			if (Connection[i]->ID != VSS_ID){
#endif	//VIRTUAL_SUBNET_SERVER
				rc = 0;
				BadConnection = Connection[i]->ID;
#ifdef VIRTUAL_SUBNET_SERVER
			}
#endif	//VIRTUAL_SUBNET_SERVER
		}
	}

	if (rc)
		BadConnection = IPXConnClass::CONNECTION_NONE;

	return(rc);


}	/* end of Service */


/***************************************************************************
 * IPXManagerClass::Get_Bad_Connection -- returns bad connection ID        *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ID of bad connection; CONNECTION_NONE if none.								*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/04/1995 BRR : Created.                                             *
 *=========================================================================*/
int IPXManagerClass::Get_Bad_Connection(void)
{
	return(BadConnection);

}	/* end of Get_Bad_Connection */


/***************************************************************************
 * IPXManagerClass::Global_Num_Send	-- reports # entries in send queue		*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		# entries in the Global Send Queue												*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Global_Num_Send(void)
{
	/*
	------------ Error if IPX not installed or not Listening -----------------
	*/
	if (!IPXStatus || !Listening)
		return(0);

	return(GlobalChannel->Queue->Num_Send());

}	/* end of Global_Num_Send */


/***************************************************************************
 * IPXManagerClass::Global_Num_Receive -- reports # entries in recv queue	*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		# entries in the Global Receive Queue											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Global_Num_Receive(void)
{
	/*
	------------ Error if IPX not installed or not Listening -----------------
	*/
	if (!IPXStatus || !Listening)
		return(0);

	return(GlobalChannel->Queue->Num_Receive());

}	/* end of Global_Num_Receive */


/***************************************************************************
 * IPXManagerClass::Private_Num_Send -- reports # entries in send queue		*
 *                                                                         *
 * INPUT:                                                                  *
 *		# entries in the Private Send Queue												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Private_Num_Send(int id)
{
	int i;
	int maxnum;

	/*
	------------ Error if IPX not installed or not Listening -----------------
	*/
	if (!IPXStatus || !Listening || (NumConnections==0)) return(false);

	/*------------------------------------------------------------------------
	If connection ID specified, return that connection's # of packets
	------------------------------------------------------------------------*/
	if (id != IPXConnClass::CONNECTION_NONE) {
		i = Connection_Index(id);
		if (i != IPXConnClass::CONNECTION_NONE) {
			return(Connection[i]->Queue->Num_Send());
		} else {
			return(false);
		}
	} else {

		/*------------------------------------------------------------------------
		Otherwise, return the max # of all connections
		------------------------------------------------------------------------*/
		maxnum = 0;
		for (i = 0; i < NumConnections; i++) {
			if (Connection[i]->Queue->Num_Send() > maxnum) {
				maxnum = Connection[i]->Queue->Num_Send();
			}
		}
		return(maxnum);
	}
}


/***************************************************************************
 * IPXManagerClass::Private_Num_Receive -- reports # entries in recv queue	*
 *                                                                         *
 * INPUT:                                                                  *
 *		id		ID of connection to query													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		# entries in the Private Receive Queue											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Private_Num_Receive(int id)
{
	int i;
	int maxnum;

	/*
	------------ Error if IPX not installed or not Listening -----------------
	*/
	if (!IPXStatus || !Listening || (NumConnections==0))
		return(0);

	/*------------------------------------------------------------------------
	If connection ID specified, return that connection's # of packets
	------------------------------------------------------------------------*/
	if (id != IPXConnClass::CONNECTION_NONE) {
		i = Connection_Index(id);
		if (i != IPXConnClass::CONNECTION_NONE) {
			return(Connection[i]->Queue->Num_Receive());
		} else {
			return(0);
		}
	} else {

		/*------------------------------------------------------------------------
		Otherwise, return the max # of all connections
		------------------------------------------------------------------------*/
		maxnum = 0;
		for (i = 0; i < NumConnections; i++) {
			if (Connection[i]->Queue->Num_Receive() > maxnum)
				maxnum = Connection[i]->Queue->Num_Receive();
		}
		return(maxnum);
	}
}


/***************************************************************************
 * IPXManagerClass::Set_Socket -- sets socket ID for all connections			*
 *                                                                         *
 * INPUT:                                                                  *
 *		socket	new socket ID to use														*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Do not call this function after communications have started; you		*
 *		must call it before calling Init().												*
 *		The socket number is byte-swapped, since IPX requires socket ID's		*
 *		to be stored high/low.																*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
void IPXManagerClass::Set_Socket(unsigned short socket)
{
	Socket = (unsigned short)( (((unsigned long)socket & 0x00ff) << 8) |
		(((unsigned long)socket & 0xff00) >> 8));

}	/* end of Set_Socket */


/***************************************************************************
 * IPXManagerClass::Response_Time -- Returns largest Avg Response Time     *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		largest avg response time															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/04/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned long IPXManagerClass::Response_Time(void)
{
	unsigned long resp;
	unsigned long maxresp = 0;
	int i;
#ifdef VIRTUAL_SUBNET_SERVER
	int vss=0;

	if (Winsock.Get_Connected()){
		vss = (int) UseVirtualSubnetServer;
	}

	for (i = 0; i < NumConnections - vss; i++) {
#else //VIRTUAL_SUBNET_SERVER
	for (i = 0; i < NumConnections; i++) {
#endif	// VIRTUAL_SUBNET_SERVER
		resp = Connection[i]->Queue->Avg_Response_Time();
		if (resp > maxresp)
			maxresp = resp;
	}

	return(maxresp);
}


/***************************************************************************
 * IPXManagerClass::Global_Response_Time -- Returns Avg Response Time      *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		avg global channel response time													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/04/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned long IPXManagerClass::Global_Response_Time(void)
{
	if (GlobalChannel) {
		return (GlobalChannel->Queue->Avg_Response_Time());
	} else {
		return (0);
	}
}


/***************************************************************************
 * IPXManagerClass::Reset_Response_Time -- Reset response time					*
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
 *   05/04/1995 BRR : Created.                                             *
 *=========================================================================*/
void IPXManagerClass::Reset_Response_Time(void)
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		Connection[i]->Queue->Reset_Response_Time();
	}

	if (GlobalChannel)
		GlobalChannel->Queue->Reset_Response_Time();

}	/* end of Reset_Response_Time */


/***************************************************************************
 * IPXManagerClass::Oldest_Send -- gets ptr to oldest send buf             *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		buf ptr																					*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/04/1995 BRR : Created.                                             *
 *=========================================================================*/
void * IPXManagerClass::Oldest_Send(void)
{
	int i,j;
	unsigned long time;
	unsigned long mintime = 0xffffffff;
	SendQueueType *send_entry;				// ptr to send entry header
	CommHeaderType *packet;
	void *buf = NULL;

	for (i = 0; i < NumConnections; i++) {

		send_entry = NULL;

		for (j = 0; j < Connection[i]->Queue->Num_Send(); j++) {
			send_entry = Connection[i]->Queue->Get_Send(j);
			if (send_entry) {
				packet = (CommHeaderType *)send_entry->Buffer;
				if (packet->Code == ConnectionClass::PACKET_DATA_ACK && send_entry->IsACK == 0) {
					break;
				} else {
					send_entry = NULL;
				}
			}
		}

		if (send_entry!=NULL) {

			time = send_entry->FirstTime;

			if (time < mintime) {
				mintime = time;
				buf = send_entry->Buffer;
			}
		}
	}

	return(buf);

}	/* end of Oldest_Send */


/***************************************************************************
 * IPXManagerClass::Set_Bridge -- prepares to cross a bridge               *
 *                                                                         *
 * This routine is designed to prevent the connection from having to			*
 * call Get_Local_Target, except the minimum number of times, since that	*
 * routine is buggy & goes away for long periods sometimes.						*
 *                                                                         *
 * INPUT:                                                                  *
 *		bridge		network number of the destination bridge						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   07/06/1995 BRR : Created.                                             *
 *=========================================================================*/
void IPXManagerClass::Set_Bridge(NetNumType bridge)
{
	if (GlobalChannel) {
		GlobalChannel->Set_Bridge(bridge);
	}
}


/***************************************************************************
 * IPXManagerClass::Configure_Debug -- sets up special debug values        *
 *                                                                         *
 * Mono_Debug_Print2() can look into a packet to pull out a particular		*
 * ID, and can print both that ID and a string corresponding to				*
 * that ID.  This routine configures these values so it can find				*
 * and decode the ID.  This ID is used in addition to the normal				*
 * CommHeaderType values.																	*
 *                                                                         *
 * INPUT:                                                                  *
 *		index			connection index to configure (-1 = Global Channel)		*
 *		offset		ID's byte offset into packet										*
 *		size			size of ID, in bytes; 0 if none									*
 *		names			ptr to array of names; use ID as an index into this		*
 *		maxnames		max # in the names array; 0 if none.							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Names shouldn't be longer than 12 characters.								*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/31/1995 BRR : Created.                                             *
 *=========================================================================*/
void IPXManagerClass::Configure_Debug(int index, int offset, int size,
	char **names, int maxnames)
{
	if (index == -1) {
		GlobalChannel->Queue->Configure_Debug (offset, size, names, maxnames);
	} else {
		if (Connection[index]) {
			Connection[index]->Queue->Configure_Debug (offset, size, names, maxnames);
		}
	}
}


/***************************************************************************
 * IPXManagerClass::Mono_Debug_Print -- debug output routine					*
 *                                                                         *
 * INPUT:                                                                  *
 *		index			index of connection to display (-1 = Global Channel)		*
 *		refresh		1 = complete screen refresh										*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
void IPXManagerClass::Mono_Debug_Print(int index, int refresh)
{
#ifdef WWLIB32_H
	char txt[80];
	int i;

	if (index == -1)
		GlobalChannel->Queue->Mono_Debug_Print (refresh);
	else if (Connection[index])
		Connection[index]->Queue->Mono_Debug_Print (refresh);

	if (refresh) {
		Mono_Set_Cursor (20,1);
		Mono_Printf ("IPX Queue:");

		Mono_Set_Cursor (9,2);
		Mono_Printf ("Average Response Time:");

		Mono_Set_Cursor (43,1);
		Mono_Printf ("Send Overflows:");

		Mono_Set_Cursor (40,2);
		Mono_Printf ("Receive Overflows:");

	}

	Mono_Set_Cursor (32,1);
	Mono_Printf ("%d",index);

	Mono_Set_Cursor (32,2);
	if (index == -1) {
		Mono_Printf ("%d  ", GlobalChannel->Queue->Avg_Response_Time());
	} else {
		Mono_Printf ("%d  ", Connection[index]->Queue->Avg_Response_Time());
	}

	Mono_Set_Cursor (59,1);
	Mono_Printf ("%d  ", SendOverflows);

	Mono_Set_Cursor (59,2);
	Mono_Printf ("%d  ", ReceiveOverflows);

	for (i = 0; i < NumBufs; i++) {
		if (BufferFlags[i]) {
			txt[i] = 'X';
		} else {
			txt[i] = '_';
		}
	}
	txt[i] = 0;
	Mono_Set_Cursor ((80-NumBufs)/2,3);
	Mono_Printf ("%s",txt);

#else
	index = index;
	refresh = refresh;
#endif
}	/* end of Mono_Debug_Print */


/***************************************************************************
 * IPXManagerClass::Alloc_RealMode_Mem -- allocates real-mode memory			*
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
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Alloc_RealMode_Mem(void)
{
#ifdef NOT_FOR_WIN95
	union REGS regs;
	struct SREGS sregs;
	int size;									// required size of allocation
	unsigned char *realmode;				// start addresses of real-mode data
	int realmodelen;							// length of real-mode data
	unsigned long func_val;
	char *p;										// for parsing buffer
	int i;

	/*------------------------------------------------------------------------
	Compute # of buffers we need to allocate, & the max size of each one
	------------------------------------------------------------------------*/
	NumBufs = Glb_NumPackets + (Pvt_NumPackets * CONNECT_MAX);

	PacketLen = Glb_MaxPacketLen + sizeof (GlobalHeaderType);
	if (Pvt_MaxPacketLen + sizeof (CommHeaderType) > PacketLen)
		PacketLen = Pvt_MaxPacketLen + sizeof (CommHeaderType);

	FullPacketLen = PacketLen + sizeof(IPXHeaderType);

	/*------------------------------------------------------------------------
	Compute the size of everything we'll ever need, allocate it in one big
	chunk.  The memory is used as follows:
	- Real-mode assembly IPX callback routine, plus its data,
	  (which includes the ListenECB)
	- Array of IPX Packet buffers (IPXHeader plus data buffer)
	- SendECB: ECB for sending
	- SendHeader: IPX Header for sending
	- SendBuf: Packet buffer for sending
	- BufferFlags: 1 byte for each incoming packet buffer; 1=in use, 0=free
	------------------------------------------------------------------------*/
	realmode = (unsigned char *)Get_RM_IPX_Address();
	realmodelen = Get_RM_IPX_Size();
	size = realmodelen + 				// assembly routine & its data
		(FullPacketLen * NumBufs) +	// array of packet buffers
		sizeof(ECBType) +					// SendECB
		FullPacketLen +					// SendHeader & SendBuf
		NumBufs;								// BufferFlags
	if (size > 65535)
		return(false);

	/*------------------------------------------------------------------------
	Allocate DOS memory for the ECB, IPXHeader & packet buffers:
	AX = 0x100
	BX = # paragraphs to allocate
	- if Success, AX = real-mode segment, DX = selector
	- if Failure, carry flag is set
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_ALLOC_DOS_MEM;						// DPMI function to call
	regs.x.ebx = ((size + 15) >> 4);						// # paragraphs to allocate
	int386x (DPMI_INT, &regs, &regs, &sregs);			// allocate the memory
	/*........................................................................
	If the carry flag is set, DPMI is indicating an error.
	........................................................................*/
	if (regs.x.cflag) {
		return(false);
	}

	/*........................................................................
	Save the values of the returned segment & selector
	........................................................................*/
	Selector = regs.w.dx;
	Segment = regs.w.ax;
	RealMemSize = size;
	RealModeData = (RealModeDataType *)(((long)Segment) << 4);

	/*------------------------------------------------------------------------
	Lock the memory (since we're servicing interrupts with it)
	AX = 0x600
	BX:CX = starting linear address of memory to lock
	SI:DI = size of region to lock (in bytes)
	- If Failure, carry flag is set.
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_LOCK_MEM;							// DPMI function to call
	regs.x.ebx = ((long)RealModeData & 0xffff0000) >> 16;
	regs.x.ecx = ((long)RealModeData & 0x0000ffff);
	regs.x.esi = ((long)RealMemSize & 0xffff0000) >> 16;
	regs.x.edi = ((long)RealMemSize & 0x0000ffff);
	int386x (DPMI_INT, &regs, &regs, &sregs);			// call DPMI
	/*........................................................................
	If the carry flag is set, DPMI is indicating an error.
	........................................................................*/
	if (regs.x.cflag) {
		memset (&regs, 0 ,sizeof(regs));
		segread (&sregs);
		regs.x.eax = DPMI_FREE_DOS_MEM;						// DPMI function to call
		regs.x.edx = Selector;									// ptr to free
		int386x (DPMI_INT, &regs, &regs, &sregs);			// free the memory
		return(false);
	}

	/*------------------------------------------------------------------------
	Copy the Real-mode code into our memory buffer
	------------------------------------------------------------------------*/
	p = (char *)(((long)Segment) << 4);
	memcpy (p,realmode,realmodelen);
	p += realmodelen;

	/*------------------------------------------------------------------------
	Compute & save the entry point for the real-mode packet handler
	------------------------------------------------------------------------*/
	func_val = (unsigned long)RealModeData;
	Handler = (((func_val & 0xffff0) << 12) |
		((func_val & 0x000f) + RealModeData->FuncOffset));

	/*------------------------------------------------------------------------
	Fill in buffer pointers
	------------------------------------------------------------------------*/
	ListenECB = &(RealModeData->ListenECB);

	FirstHeaderBuf = (IPXHeaderType *)p;
	FirstDataBuf = (((char *)FirstHeaderBuf) + sizeof(IPXHeaderType));
	CurIndex = 0;
	CurHeaderBuf = FirstHeaderBuf;
	CurDataBuf = FirstDataBuf;
	p += FullPacketLen * NumBufs;

	SendECB = (ECBType *)p;
	p += sizeof (ECBType);

	SendHeader = (IPXHeaderType *)p;
	p += sizeof (IPXHeaderType);

	SendBuf = (char *)p;
	p += PacketLen;

	BufferFlags = (char *)p;

	/*------------------------------------------------------------------------
	Fill in the real-mode routine's data (The ECB will be filled in when we
	command IPX to Listen).
	------------------------------------------------------------------------*/
	RealModeData->NumBufs = (short)NumBufs;
	RealModeData->BufferFlags = (char *)
		((((long)BufferFlags & 0xffff0) << 12) |
		((long)BufferFlags & 0x0000f));
	RealModeData->PacketSize = (short)FullPacketLen;
	RealModeData->FirstPacketBuf = (IPXHeaderType *)
		((((long)FirstHeaderBuf & 0xffff0) << 12) |
		((long)FirstHeaderBuf & 0x0000f));
	RealModeData->CurIndex = 0;
	RealModeData->CurPacketBuf = RealModeData->FirstPacketBuf;
	RealModeData->Semaphore = 0;
	RealModeData->ReEntrantCount = 0;

	/*------------------------------------------------------------------------
	Init state of all buffers to empty
	------------------------------------------------------------------------*/
	for (i = 0; i < NumBufs; i++)
		BufferFlags[i] = 0;

	/*------------------------------------------------------------------------
	Check the start & end markers in the real-mode memory area
	------------------------------------------------------------------------*/
	if (RealModeData->Marker1 != 0x1111 ||
		RealModeData->Marker2 != 0x2222) {
		Free_RealMode_Mem();
		return(false);
	} else {
		return(true);
	}
#else	//NOT_FOR_WIN95

	return (TRUE);

#endif	//NOT_FOR_WIN95
}


/***************************************************************************
 * IPXManagerClass::Free_RealMode_Mem -- frees real-mode memory				*
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
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Free_RealMode_Mem(void)
{
#ifdef NOT_FOR_WIN95
	union REGS regs;
	struct SREGS sregs;
	int rc = 1;

	/*------------------------------------------------------------------------
	Unlock the memory
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_UNLOCK_MEM;							// DPMI function to call
	regs.x.ebx = ((long)RealModeData & 0xffff0000) >> 16;
	regs.x.ecx = ((long)RealModeData & 0x0000ffff);
	regs.x.esi = ((long)RealMemSize & 0xffff0000) >> 16;
	regs.x.edi = ((long)RealMemSize & 0x0000ffff);
	int386x (DPMI_INT, &regs, &regs, &sregs);			// call DPMI
	/*........................................................................
	If the carry flag is set, DPMI is indicating an error.
	........................................................................*/
	if (regs.x.cflag) {
		rc = 0;
	}

	/*------------------------------------------------------------------------
	Free DOS memory
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_FREE_DOS_MEM;						// DPMI function to call
	regs.x.edx = Selector;									// ptr to free
	int386x (DPMI_INT, &regs, &regs, &sregs);			// free the memory

	return(rc);

#else	// NOT_FOR_WIN95

	return (1);

#endif	// NOT_FOR_WIN95
}	/* end of Free_RealMode_Mem */


