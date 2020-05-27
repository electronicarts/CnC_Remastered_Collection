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

/* $Header: /CounterStrike/IPXMGR.CPP 3     10/13/97 2:20p Steve_t $ */
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
 *   IPXManagerClass::Set_Connection_Parms -- sets connection's name & id	*
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

#include	"function.h"
#include <stdio.h>
//#include <mem.h> PG
//#include <i86.h> PG
#include "ipxmgr.h"
#include "wwlib32.h"	// to enable mono output

#ifdef WINSOCK_IPX

#include "WSProto.h"
#include "WSPIPX.h"

#else	//WINSOCK_IPX

#include "ipx95.h"
#ifdef WIN32
#include "tcpip.h"
#else
#include	"fakesock.h"
#endif	//WIN32

#endif	//WINSOCK_IPX

// Turn off "expression is not meaningful".
//#pragma warning 628 9

//#include "WolDebug.h"

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
#ifdef WINSOCK_IPX
	/*
	** Find out if Packet protocol services are available through Winsock.
	*/
	if ( PacketTransport ) {
		delete  PacketTransport;
		PacketTransport = NULL;
	}
	PacketTransport = new WinsockInterfaceClass;
	assert ( PacketTransport != NULL );

	if ( PacketTransport->Init() ){
		IPXStatus = 1;
	}else{
		IPXStatus = 0;
	}
	delete PacketTransport;
	PacketTransport = NULL;

#else	//WINSOCK_IPX

	//------------------------------------------------------------------------
	//	Initialize data members
	//------------------------------------------------------------------------
	//........................................................................
	//	IPXStatus = 1 if IPX is installed, 0 if not
	//........................................................................
	if (IPX_SPX_Installed()==0) {
		IPXStatus = 0;
	}
	else {
		IPXStatus = 1;
	}
#endif	//WINSOCK_IPX

	//........................................................................
	//	Set listening state flag to off
	//........................................................................
	Listening = 0;

	//........................................................................
	//	No memory has been alloc'd yet
	//........................................................................
	RealMemAllocd = 0;

	//........................................................................
	//	Set max packet sizes, for allocating real-mode memory
	//........................................................................
	Glb_MaxPacketLen = glb_maxlen;
	Glb_NumPackets = glb_num_packets;
	Pvt_MaxPacketLen = pvt_maxlen;
	Pvt_NumPackets = pvt_num_packets;

	//........................................................................
	//	Save the app's product ID
	//........................................................................
	ProductID = product_id;

	//........................................................................
	//	Save our socket ID number
	//........................................................................
	Socket = (unsigned short)( (((unsigned long)socket & 0x00ff) << 8) |
		(((unsigned long)socket & 0xff00) >> 8));

	//------------------------------------------------------------------------
	//	Get the user's IPX local connection number
	//------------------------------------------------------------------------
	ConnectionNum = 0;
#ifndef WINSOCK_IPX
	if (IPXStatus) {
		ConnectionNum = IPX_Get_Connection_Number();
	}
#endif	//WINSOCK_IPX

	//------------------------------------------------------------------------
	//	Init connection states
	//------------------------------------------------------------------------
	NumConnections = 0;
	CurConnection = 0;
	for (i = 0; i < CONNECT_MAX; i++) {
		Connection[i] = 0;
	}
	GlobalChannel = 0;

	SendOverflows = 0;
	ReceiveOverflows = 0;
	BadConnection = CONNECTION_NONE;

	//------------------------------------------------------------------------
	//	Init timing parameters
	//------------------------------------------------------------------------
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

	//------------------------------------------------------------------------
	//	Stop all IPX events
	//------------------------------------------------------------------------
	if (Listening) {
		IPXConnClass::Stop_Listening();
		Listening = 0;
	}

	//------------------------------------------------------------------------
	//	Free all protected-mode memory
	//------------------------------------------------------------------------
	if (GlobalChannel) {
		delete GlobalChannel;
		GlobalChannel = 0;
	}
	for (i = 0; i < NumConnections; i++) {
		delete Connection[i];
		Connection[i] = 0;
	}
	NumConnections = 0;

	//------------------------------------------------------------------------
	//	Free all real-mode memory
	//------------------------------------------------------------------------
	if (RealMemAllocd) {
		Free_RealMode_Mem();
		RealMemAllocd = 0;
	}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
#ifdef WIN32
#ifndef WINSOCK_IPX
//PG	Unload_IPX_Dll();
#endif
#endif
#endif
}	/* end of ~IPXManagerClass */


/***************************************************************************
 * IPXManagerClass::Init -- initialization routine									*
 *                                                                         *
 * This routine allocates memory, & initializes variables						*
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

	if (Session.Type != GAME_INTERNET) {

		//------------------------------------------------------------------------
		//	Error if IPX not installed
		//------------------------------------------------------------------------
		if (!IPXStatus) {
			return(0);
		}

		//------------------------------------------------------------------------
		//	Stop Listening
		//------------------------------------------------------------------------
		if (Listening) {
			IPXConnClass::Stop_Listening();
			Listening = 0;
		}

		//------------------------------------------------------------------------
		//	Free Real-mode memory
		//------------------------------------------------------------------------
		if (RealMemAllocd) {
			Free_RealMode_Mem();
			RealMemAllocd = 0;
		}

	} else {
		/*
		** Pretend IPX is available for Internet games whether it is or not
		*/
		IPXStatus = 1;
	}

	//------------------------------------------------------------------------
	//	Free protected-mode memory
	//------------------------------------------------------------------------
	if (GlobalChannel) {
		delete GlobalChannel;
		GlobalChannel = 0;
	}
	for (i = 0; i < NumConnections; i++) {
		delete Connection[i];
		Connection[i] = 0;
	}
	NumConnections = 0;

	if (Session.Type != GAME_INTERNET) {
		//------------------------------------------------------------------------
		//	Allocate real-mode memory
		//------------------------------------------------------------------------
		if (!Alloc_RealMode_Mem()) return(0);
		RealMemAllocd = 1;
	}

	//------------------------------------------------------------------------
	//	Allocate the Global Channel
	//------------------------------------------------------------------------
	GlobalChannel = new IPXGlobalConnClass (Glb_NumPackets, Glb_NumPackets,
		Glb_MaxPacketLen, ProductID);
	if (!GlobalChannel) {
		return(0);
	}
	GlobalChannel->Init();
	GlobalChannel->Set_Retry_Delta (RetryDelta);
	GlobalChannel->Set_Max_Retries (MaxRetries);
	GlobalChannel->Set_TimeOut (Timeout);

	//------------------------------------------------------------------------
	//	Configure the IPX Connections
	//------------------------------------------------------------------------
	IPXConnClass::Configure(Socket, ConnectionNum, ListenECB, SendECB,
		FirstHeaderBuf, SendHeader, FirstDataBuf, SendBuf, Handler, PacketLen);

	//------------------------------------------------------------------------
	//	Start Listening
	//------------------------------------------------------------------------
	if (Session.Type != GAME_INTERNET) {
		if (!IPXConnClass::Start_Listening()) return(0);
	}
	Listening = 1;

	return(1);

}	/* end of Init */


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
int IPXManagerClass::Create_Connection(int id, char *name,
	IPXAddressClass *address)
{
	//------------------------------------------------------------------------
	//	Error if IPX not installed
	//------------------------------------------------------------------------
	if (!IPXStatus) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	Error if no more room
	//------------------------------------------------------------------------
	if (NumConnections==CONNECT_MAX) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	Create new connection
	//------------------------------------------------------------------------
	Connection[NumConnections] = new IPXConnClass(Pvt_NumPackets,
		Pvt_NumPackets, Pvt_MaxPacketLen, ProductID, address, id, name);
	if (!Connection[NumConnections]) {
		return(0);
	}

	Connection[NumConnections]->Init ();
	Connection[NumConnections]->Set_Retry_Delta (RetryDelta);
	Connection[NumConnections]->Set_Max_Retries (MaxRetries);
	Connection[NumConnections]->Set_TimeOut (Timeout);

	NumConnections++;

	return(1);

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
int IPXManagerClass::Delete_Connection(int id)
{
	int i,j;

	//------------------------------------------------------------------------
	//	Error if IPX not installed
	//------------------------------------------------------------------------
	if (!IPXStatus) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	Error if no connections to delete
	//------------------------------------------------------------------------
	if (NumConnections==0) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	Loop through all connections
	//------------------------------------------------------------------------
	for (i = 0; i < NumConnections; i++) {
		//.....................................................................
		//	If a match, delete it
		//.....................................................................
		if (Connection[i]->ID==id) {
			delete Connection[i];

			//..................................................................
			//	Move array elements back one index
			//..................................................................
			for (j = i; j < NumConnections - 1; j++) {
				Connection[j] = Connection[j+1];
			}

			//..................................................................
			//	Adjust counters
			//..................................................................
			NumConnections--;
			if (CurConnection >= NumConnections)
				CurConnection = 0;
			return(1);
		}
	}

	//------------------------------------------------------------------------
	//	No match; error
	//------------------------------------------------------------------------
	return(0);

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
	return(NumConnections);

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
	}
	else {
		return(CONNECTION_NONE);
	}
}	/* end of Connection_ID */


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
		if (Connection[i]->ID==id) {
			return(i);
		}
	}

	return(CONNECTION_NONE);

}	/* end of Connection_Index */


/***************************************************************************
 * IPXManagerClass::Set_Connection_Parms -- sets connection's name & id		*
 *                                                                         *
 * INPUT:                                                                  *
 *		index		connection index															*
 *		id			new connection ID															*
 *		name		new connection name														*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
void IPXManagerClass::Set_Connection_Parms(int index, int id, char *name)
{
	if (index >= NumConnections)
		return;

	Connection[index]->ID = id;
	strcpy(Connection[index]->Name,name);

}	/* end of Set_Connection_Parms */


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

	//------------------------------------------------------------------------
	//	Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening) return(0);

	rc = GlobalChannel->Send_Packet (buf, buflen, address, ack_req);
	if (!rc) {
		SendOverflows++;
	}

	return(rc);

}	/* end of Send_Global_Message */


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
	//------------------------------------------------------------------------
	//	Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening) return(0);

	return(GlobalChannel->Get_Packet (buf, buflen, address, product_id));

}	/* end of Get_Global_Message */


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

	//------------------------------------------------------------------------
	//	Error if IPX not installed or not Listening
	//------------------------------------------------------------------------

	if (!IPXStatus || !Listening || (NumConnections==0)) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	Send the message to all connections
	//------------------------------------------------------------------------
	if (conn_id==CONNECTION_NONE) {
		//.....................................................................
		//	Check for room in all connections
		//.....................................................................
		for (i = 0; i < NumConnections; i++) {
			if (Connection[i]->Queue->Num_Send() ==
				Connection[i]->Queue->Max_Send()) {
				SendOverflows++;
				return(0);
			}
		}

		//.....................................................................
		//	Send packet to all connections
		//.....................................................................
		for (i = 0; i < NumConnections; i++) {
			Connection[i]->Send_Packet (buf, buflen, ack_req);
		}
		return(1);
	}

	//------------------------------------------------------------------------
	//	Send the message to the specified connection
	//------------------------------------------------------------------------
	else {
		connect_idx = Connection_Index (conn_id);
		if (connect_idx == CONNECTION_NONE) {
			SendOverflows++;
			return(0);
		}

		//.....................................................................
		//	Check for room in the connection
		//.....................................................................
		if (Connection[connect_idx]->Queue->Num_Send() ==
			Connection[connect_idx]->Queue->Max_Send()) {
			SendOverflows++;
			return(0);
		}

		//.....................................................................
		//	Send the packet to that connection
		//.....................................................................
		Connection[connect_idx]->Send_Packet (buf, buflen, ack_req);
		return(1);
	}

}	/* end of Send_Private_Message */


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

	//------------------------------------------------------------------------
	//	Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening || (NumConnections==0)) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	Safety check: ensure CurConnection is in range.
	//------------------------------------------------------------------------
	if (CurConnection >= NumConnections) {
		CurConnection = 0;
	}

	//------------------------------------------------------------------------
	//	Scan all connections for a received packet, starting with 'CurConnection'
	//------------------------------------------------------------------------
	for (i = 0; i < NumConnections; i++) {

		//.....................................................................
		//	Check this connection for a packet
		//.....................................................................
		rc = Connection[CurConnection]->Get_Packet (buf, buflen);
		c_id = Connection[CurConnection]->ID;

		//.....................................................................
		//	Increment CurConnection to the next connection index
		//.....................................................................
		CurConnection++;
		if (CurConnection >= NumConnections) {
			CurConnection = 0;
		}

		//.....................................................................
		//	If we got a packet, return the connection ID
		//.....................................................................
		if (rc) {
			(*conn_id) = c_id;
			return(1);
		}
	}

	return(0);

}	/* end of Get_Private_Message */


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

#ifdef WINSOCK_IPX


	unsigned char temp_receive_buffer[1024];
	int	temp_receive_buffer_len;
	int	temp_address_len;


	char temp_address [128];

	if ( PacketTransport ) {

		do {
			temp_receive_buffer_len = sizeof (temp_receive_buffer);
			temp_address_len = sizeof (temp_address);
			packetlen = PacketTransport->Read ( temp_receive_buffer, temp_receive_buffer_len, temp_address, temp_address_len );
			if ( packetlen ) {
				CurDataBuf = (char*)temp_receive_buffer;
				address = *((IPXAddressClass*) temp_address);

				packet = (CommHeaderType *)CurDataBuf;
				if (packet->MagicNumber == GlobalChannel->Magic_Num()) {

					/*
					** Put the packet in the Global Queue
					*/
					if (!GlobalChannel->Receive_Packet (packet, packetlen, &address))
						ReceiveOverflows++;
				} else {
					if (packet->MagicNumber == ProductID) {

						/*
						** Find the Private Queue that this packet is for
						*/
						bool found_address = false;
						for (i = 0; i < NumConnections; i++) {
							if (Connection[i]->Address == address) {
								found_address = true;
								if (!Connection[i]->Receive_Packet (packet, packetlen))
									ReceiveOverflows++;
								break;
							}
						}
						if( Session.Type == GAME_INTERNET )
						{
							/*
							** This packet came from an unknown source. If it looks like one of our players
							** packets then it might be from a player whos IP has changed.
							*/
							if (!found_address) {
								if (packet->Code == ConnectionClass::PACKET_DATA_NOACK){
									/*
									** Magic number and packet code are valid. It's probably a C&C packet.
									*/
									EventClass *event = (EventClass*) (((char*) packet) + sizeof (CommHeaderType));

									/*
									** If this is a framesync packet then grab the address and match it to an existing player.
									*/
									if (event->Type == EventClass::FRAMESYNC) {
										int id = event->ID;

										assert (id != PlayerPtr->ID);
										for ( int i=1 ; i<Session.Players.Count() ; i++) {
											if (Session.Players[i]->Player.ID == id) {

												int iConnectionIndex = Connection_Index(id);
												if( iConnectionIndex != CONNECTION_NONE ) //	(else Create_Connections() has not yet been called)
												{
													/*
													** Found a likely candidate. Update his address. It should be OK to drop this
													** packet since it's a framesync packet and will will pick up the next one.
													*/
													Session.Players[i]->Address = address;
													Connection[iConnectionIndex]->Address = address;
												}
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}

		} while (packetlen);

	}




#else	//WINSOCK_IPX

#ifdef WIN32

	unsigned char temp_receive_buffer[1024];
	int	recv_length;

	if (Winsock.Get_Connected() || Special.IsFromWChat) {

		if (!Winsock.Get_Connected()) return (0);

		/*
		** This is an internet connection so get the packets from winsock
		*/
		while ((recv_length = Winsock.Read(temp_receive_buffer, 1024))!=0) {

			CurHeaderBuf = NULL;
			CurDataBuf = (char*)&temp_receive_buffer[0];

			/*.....................................................................
			Compute the length of the packet (byte-swap the length in the IPX hdr)
			.....................................................................*/
			packetlen = recv_length;

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
	} else {
#if(0)//PG
		while (IPX_Get_Outstanding_Buffer95(&temp_receive_buffer[0])) {

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
#endif
	}


#else	//WIN32


	//------------------------------------------------------------------------
	//	Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	Loop until there are no more packets to process.
	//------------------------------------------------------------------------
	while (1) {

		//.....................................................................
		//	Check the BufferFlags for the "current" buffer; if it's empty,
		//	break; out of the loop.
		//.....................................................................
		if (BufferFlags[CurIndex]==0) {
			break;
		}

		//.....................................................................
		//	Compute the length of the packet (byte-swap the length in the IPX hdr)
		//.....................................................................
		packetlen = ((CurHeaderBuf->Length & 0xff) << 8) |
			(CurHeaderBuf->Length >> 8);
		packetlen -= sizeof(IPXHeaderType);

		//.....................................................................
		//	Extract the sender's address from the IPX header
		//.....................................................................
		address.Set_Address (CurHeaderBuf);

		//.....................................................................
		//	Examine the Magic Number of the received packet to determine if this
		//	packet goes into the Global Queue, or into one of the Private Queues
		//.....................................................................
		packet = (CommHeaderType *)CurDataBuf;
		if (packet->MagicNumber == GlobalChannel->Magic_Num()) {

			//..................................................................
			//	Put the packet in the Global Queue
			//..................................................................
			if (!GlobalChannel->Receive_Packet (packet, packetlen, &address)) {
				ReceiveOverflows++;
			}
		}

		//.....................................................................
		//	Find the Private Queue that this packet is for
		//.....................................................................
		else if (packet->MagicNumber == ProductID) {
			for (i = 0; i < NumConnections; i++) {
				if (Connection[i]->Address == address) {
					if (!Connection[i]->Receive_Packet (packet, packetlen)) {
						ReceiveOverflows++;
					}
					break;
				}
			}
		}

		//.....................................................................
		//	Set the current BufferFlags to 0 (since we've cleaned out this buffer)
		//.....................................................................
		BufferFlags[CurIndex] = 0;

		//.....................................................................
		//	Go to the next packet buffer
		//.....................................................................
		CurIndex++;
		CurHeaderBuf = (IPXHeaderType *)(((char *)CurHeaderBuf) + FullPacketLen);
		CurDataBuf = ((char *)CurDataBuf) + FullPacketLen;
		if (CurIndex >= NumBufs) {
			CurHeaderBuf = FirstHeaderBuf;
			CurDataBuf = FirstDataBuf;
			CurIndex = 0;
		}
	}

#endif	//WIN32
#endif	//WINSOCK_IPX

	//------------------------------------------------------------------------
	//	Service all connections.  If a connection reports that it's gone "bad",
	//	report an error to the caller.  If it's the Global Channel, un-queue the
	//	send entry that's holding things up.  This will keep the Global Channel
	//	from being clogged by one un-ACK'd outgoing packet.
	//------------------------------------------------------------------------
	if (GlobalChannel) {
		if (!GlobalChannel->Service()) {
			GlobalChannel->Queue->UnQueue_Send (NULL, NULL, 0);
			rc = 0;
		}
	}
	for (i = 0; i < NumConnections; i++) {
		if (!Connection[i]->Service()) {
			rc = 0;
			BadConnection = Connection[i]->ID;
		}
	}

	if (rc) {
		BadConnection = CONNECTION_NONE;
	}

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
	//------------------------------------------------------------------------
	//	Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening) {
		return(0);
	}

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
	//------------------------------------------------------------------------
	//	Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening) {
		return(0);
	}

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

	//------------------------------------------------------------------------
	//	Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening || (NumConnections==0)) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	If connection ID specified, return that connection's # of packets
	//------------------------------------------------------------------------
	if (id != CONNECTION_NONE) {
		i = Connection_Index(id);
		if (i != CONNECTION_NONE) {
			return(Connection[i]->Queue->Num_Send());
		}
		else {
			return(0);
		}

	}

	//------------------------------------------------------------------------
	//	Otherwise, return the max # of all connections
	//------------------------------------------------------------------------
	else {
		maxnum = 0;
		for (i = 0; i < NumConnections; i++) {
			if (Connection[i]->Queue->Num_Send() > maxnum) {
				maxnum = Connection[i]->Queue->Num_Send();
			}
		}
		return(maxnum);
	}

}	/* end of Private_Num_Send */


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

	//------------------------------------------------------------------------
	//	Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening || (NumConnections==0))
		return(0);

	//------------------------------------------------------------------------
	//	If connection ID specified, return that connection's # of packets
	//------------------------------------------------------------------------
	if (id != CONNECTION_NONE) {
		i = Connection_Index(id);
		if (i != CONNECTION_NONE) {
			return(Connection[i]->Queue->Num_Receive());
		}
		else {
			return(0);
		}

	}

	//------------------------------------------------------------------------
	//	Otherwise, return the max # of all connections
	//------------------------------------------------------------------------
	else {
		maxnum = 0;
		for (i = 0; i < NumConnections; i++) {
			if (Connection[i]->Queue->Num_Receive() > maxnum) {
				maxnum = Connection[i]->Queue->Num_Receive();
			}
		}
		return(maxnum);
	}

}	/* end of Private_Num_Receive */


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

	for (i = 0; i < NumConnections; i++) {
		resp = Connection[i]->Queue->Avg_Response_Time();
		if (resp > maxresp) {
			maxresp = resp;
		}
	}

	return(maxresp);

}	/* end of Response_Time */


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
	}
	else {
		return (0);
	}

}	/* end of Global_Response_Time */


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
				if (packet->Code == ConnectionClass::PACKET_DATA_ACK &&
					send_entry->IsACK == 0) {
					break;
				}
				else {
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

}	/* end of Set_Bridge */


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
 *		index				connection index to configure (-1 = Global Channel)	*
 *		type_offset		ID's byte offset into packet									*
 *		type_size		size of ID, in bytes; 0 if none								*
 *		names				ptr to array of names; use ID as an index into this	*
 *		namestart		numerical value of 1st name in the array					*
 *		namecount		# in the names array; 0 if none.								*
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
void IPXManagerClass::Configure_Debug(int index, int type_offset,
	int type_size, char **names, int namestart, int namecount)
{
	if (index == -1) {
		GlobalChannel->Queue->Configure_Debug (type_offset, type_size, names,
			namestart, namecount);
	}
	else if (Connection[index]) {
		Connection[index]->Queue->Configure_Debug (type_offset, type_size, names,
			namestart, namecount);
	}

}	/* end of Configure_Debug */


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
	}
	else {
		Mono_Printf ("%d  ", Connection[index]->Queue->Avg_Response_Time());
	}

	Mono_Set_Cursor (59,1);
	Mono_Printf ("%d  ", SendOverflows);

	Mono_Set_Cursor (59,2);
	Mono_Printf ("%d  ", ReceiveOverflows);

	for (i = 0; i < NumBufs; i++) {
		if (BufferFlags[i]) {
			txt[i] = 'X';
		}
		else {
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

#ifdef WIN32
	return (1);
#else

	union REGS regs;
	struct SREGS sregs;
	int size;									// required size of allocation
	unsigned char *realmode;				// start addresses of real-mode data
	int realmodelen;							// length of real-mode data
	unsigned long func_val;
	char *p;										// for parsing buffer
	int i;

	//------------------------------------------------------------------------
	//	Compute # of buffers we need to allocate, & the max size of each one
	//------------------------------------------------------------------------
	NumBufs = Glb_NumPackets + (Pvt_NumPackets * CONNECT_MAX);

	PacketLen = Glb_MaxPacketLen + sizeof (GlobalHeaderType);
	if (Pvt_MaxPacketLen + sizeof (CommHeaderType) > PacketLen)
		PacketLen = Pvt_MaxPacketLen + sizeof (CommHeaderType);

	FullPacketLen = PacketLen + sizeof(IPXHeaderType);

	//------------------------------------------------------------------------
	//	Compute the size of everything we'll ever need, allocate it in one big
	//	chunk.  The memory is used as follows:
	//	- Real-mode assembly IPX callback routine, plus its data,
	//	  (which includes the ListenECB)
	//	- Array of IPX Packet buffers (IPXHeader plus data buffer)
	//	- SendECB: ECB for sending
	//	- SendHeader: IPX Header for sending
	//	- SendBuf: Packet buffer for sending
	//	- BufferFlags: 1 byte for each incoming packet buffer; 1=in use, 0=free
	//------------------------------------------------------------------------
	realmode = (unsigned char *)Get_RM_IPX_Address();
	realmodelen = Get_RM_IPX_Size();
	size = realmodelen + 				// assembly routine & its data
		(FullPacketLen * NumBufs) +	// array of packet buffers
		sizeof(ECBType) +					// SendECB
		FullPacketLen +					// SendHeader & SendBuf
		NumBufs;								// BufferFlags
	if (size > 65535) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	Allocate DOS memory for the ECB, IPXHeader & packet buffers:
	//	AX = 0x100
	//	BX = # paragraphs to allocate
	//	- if Success, AX = real-mode segment, DX = selector
	//	- if Failure, carry flag is set
	//------------------------------------------------------------------------
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_ALLOC_DOS_MEM;						// DPMI function to call
	regs.x.ebx = ((size + 15) >> 4);						// # paragraphs to allocate
	int386x (DPMI_INT, &regs, &regs, &sregs);			// allocate the memory

	//------------------------------------------------------------------------
	//	If the carry flag is set, DPMI is indicating an error.
	//------------------------------------------------------------------------
	if (regs.x.cflag) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	Save the values of the returned segment & selector
	//------------------------------------------------------------------------
	Selector = regs.w.dx;
	Segment = regs.w.ax;
	RealMemSize = size;
	RealModeData = (RealModeDataType *)(((long)Segment) << 4);

	//------------------------------------------------------------------------
	//	Lock the memory (since we're servicing interrupts with it)
	//	AX = 0x600
	//	BX:CX = starting linear address of memory to lock
	//	SI:DI = size of region to lock (in bytes)
	//	- If Failure, carry flag is set.
	//------------------------------------------------------------------------
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_LOCK_MEM;							// DPMI function to call
	regs.x.ebx = ((long)RealModeData & 0xffff0000) >> 16;
	regs.x.ecx = ((long)RealModeData & 0x0000ffff);
	regs.x.esi = ((long)RealMemSize & 0xffff0000) >> 16;
	regs.x.edi = ((long)RealMemSize & 0x0000ffff);
	int386x (DPMI_INT, &regs, &regs, &sregs);			// call DPMI

	//------------------------------------------------------------------------
	//	If the carry flag is set, DPMI is indicating an error.
	//------------------------------------------------------------------------
	if (regs.x.cflag) {
		memset (&regs, 0 ,sizeof(regs));
		segread (&sregs);
		regs.x.eax = DPMI_FREE_DOS_MEM;						// DPMI function to call
		regs.x.edx = Selector;									// ptr to free
		int386x (DPMI_INT, &regs, &regs, &sregs);			// free the memory
		return(0);
	}

	//------------------------------------------------------------------------
	//	Copy the Real-mode code into our memory buffer
	//------------------------------------------------------------------------
	p = (char *)(((long)Segment) << 4);
	memcpy (p,realmode,realmodelen);
	p += realmodelen;

	//------------------------------------------------------------------------
	//	Compute & save the entry point for the real-mode packet handler
	//------------------------------------------------------------------------
	func_val = (unsigned long)RealModeData;
	Handler = (((func_val & 0xffff0) << 12) |
		((func_val & 0x000f) + RealModeData->FuncOffset));

	//------------------------------------------------------------------------
	//	Fill in buffer pointers
	//------------------------------------------------------------------------
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

	//------------------------------------------------------------------------
	//	Fill in the real-mode routine's data (The ECB will be filled in when we
	//	command IPX to Listen).
	//------------------------------------------------------------------------
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

	//------------------------------------------------------------------------
	//	Init state of all buffers to empty
	//------------------------------------------------------------------------
	for (i = 0; i < NumBufs; i++) {
		BufferFlags[i] = 0;
	}

	//------------------------------------------------------------------------
	//	Check the start & end markers in the real-mode memory area
	//------------------------------------------------------------------------
	if (RealModeData->Marker1 != 0x1111 ||
		RealModeData->Marker2 != 0x2222) {
		Free_RealMode_Mem();
		return(0);
	}
	else {
		return(1);
	}
#endif	//WIN32
}	/* end of Alloc_Realmode_Mem */


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

#ifdef WIN32

	return (1);

#else	//WIN32

	union REGS regs;
	struct SREGS sregs;
	int rc = 1;

	//------------------------------------------------------------------------
	//	Unlock the memory
	//------------------------------------------------------------------------
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_UNLOCK_MEM;							// DPMI function to call
	regs.x.ebx = ((long)RealModeData & 0xffff0000) >> 16;
	regs.x.ecx = ((long)RealModeData & 0x0000ffff);
	regs.x.esi = ((long)RealMemSize & 0xffff0000) >> 16;
	regs.x.edi = ((long)RealMemSize & 0x0000ffff);
	int386x (DPMI_INT, &regs, &regs, &sregs);			// call DPMI

	//------------------------------------------------------------------------
	//	If the carry flag is set, DPMI is indicating an error.
	//------------------------------------------------------------------------
	if (regs.x.cflag) {
		rc = 0;
	}

	//------------------------------------------------------------------------
	//	Free DOS memory
	//------------------------------------------------------------------------
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_FREE_DOS_MEM;						// DPMI function to call
	regs.x.edx = Selector;									// ptr to free
	int386x (DPMI_INT, &regs, &regs, &sregs);			// free the memory

	return(rc);
#endif	//WIN32

}	/* end of Free_Realmode_Mem */

/*************************** end of ipxmgr.cpp *****************************/
