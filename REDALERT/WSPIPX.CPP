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

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                     $Archive:: /Sun/WSPIPX.cpp                                             $*
 *                                                                                             *
 *                      $Author:: Joe_b                                                       $*
 *                                                                                             *
 *                     $Modtime:: 8/20/97 10:54a                                              $*
 *                                                                                             *
 *                    $Revision:: 6                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *                                                                                             *
 * IPXInterfaceClass::IPXInterfaceClass -- Class constructor                                   *
 * IPXInterfaceClass::Get_Network_Card_Address -- Get the ID of the installed net card         *
 * IPXInterfaceClass::Open_Socket -- Opens an IPX socket for reading & writing                 *
 * IPXInterfaceClass::Message_Handler -- Handler for windows messages relating to IPX          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"wspipx.h"
#include	"ipxaddr.h"

#include	<assert.h>
#include	<stdio.h>


/***********************************************************************************************
 * IPXInterfaceClass::IPXInterfaceClass -- Class constructor                                   *
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
 *    8/4/97 11:41AM ST : Created                                                              *
 *=============================================================================================*/
IPXInterfaceClass::IPXInterfaceClass (void) : WinsockInterfaceClass()
{
	/*
	** Set the net and node addressed to their default values.
	*/
	memset ( BroadcastNet, 0xff, sizeof (BroadcastNet) );
	memset ( BroadcastNode, 0xff, sizeof (BroadcastNode) );
	memset ( MyNode, 0xff, sizeof (MyNode) );
}


/***********************************************************************************************
 * IPXInterfaceClass::Get_Network_Card_Address -- Get the ID of the installed net card         *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    card number to retrieve ID for                                                    *
 *           ptr to addr to return ID in                                                       *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/1/97 3:04PM ST : Created                                                               *
 *=============================================================================================*/
bool IPXInterfaceClass::Get_Network_Card_Address (int card_number, SOCKADDR_IPX *addr)
{
	int            	cbOpt;
	int					cbAddr = sizeof( SOCKADDR_IPX );
    SOCKET         	s;
    SOCKADDR_IPX   	Addr;
    IPX_ADDRESS_DATA  IpxData;

    /*
	** Create a temporary IPX socket.
	*/
    s = socket( AF_IPX, SOCK_DGRAM, NSPROTO_IPX );
	if ( s == SOCKET_ERROR ) {
		assert ( s != SOCKET_ERROR );
		return (false);
	}

	/*
	** Socket must be bound prior to calling IPX_MAX_ADAPTER_NUM
	*/
    memset( &Addr, 0, sizeof( Addr ));
    Addr.sa_family = AF_IPX;
    int err = bind( s, (SOCKADDR*) &Addr, cbAddr);
	if ( err == SOCKET_ERROR ) {
		assert ( err != SOCKET_ERROR );
		closesocket (s);
		return (false);
	}

    memset( &IpxData, 0, sizeof(IpxData));

	/*
	** Specify which adapter to check.
	*/
    IpxData.adapternum = card_number;
    cbOpt = sizeof( IpxData );

	/*
	** Get information for the current adapter.
	*/
    err = getsockopt( s, NSPROTO_IPX, IPX_ADDRESS, (char*) &IpxData, &cbOpt );
	if ( err == SOCKET_ERROR ) {
		assert ( err != SOCKET_ERROR );
		closesocket (s);
		return (false);
	}

	/*
	** IpxData contains the address for the current adapter.
	** The network number will be needed later for broadcasts as the net number ff,ff,ff,ff
	** doesn't work under NT.
	**
	** Note: Due to a bug in Win95s implementation of Winsock, only the netnum & nodenum
	** values are correctly returned. NT returns all expected values. ST - 7/31/97 0:57AM
	*/
	memcpy (addr->sa_netnum, IpxData.netnum, sizeof (addr->sa_netnum));
	memcpy (BroadcastNet, IpxData.netnum, sizeof (addr->sa_netnum));
	memcpy (addr->sa_nodenum, IpxData.nodenum, sizeof (addr->sa_nodenum));

	closesocket (s);
	return (true);
}






/***********************************************************************************************
 * IPXInterfaceClass::Open_Socket -- Opens an IPX socket for reading & writing                 *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    SOCKET number to open. This is usually VIRGIN_SOCKET                              *
 *                                                                                             *
 * OUTPUT:   true if socket was opened without error                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/4/97 5:54PM ST : Created                                                               *
 *=============================================================================================*/
bool IPXInterfaceClass::Open_Socket( SOCKET socketnum )
{
	SOCKADDR_IPX 	addr;
	bool				delay = true;
	int				err;

	/*
	** If Winsock is not initialised then do it now.
	*/
	if ( !WinsockInitialised ) {
		if ( !Init()) return ( false );;
	}

	IPXSocketNumber = socketnum;

	/*
	** Set up the addr structure for the IPX socket
	*/
	addr.sa_family = AF_IPX;
	memset (addr.sa_netnum, 0, sizeof (addr.sa_netnum));
	memset (addr.sa_nodenum, -1, sizeof (addr.sa_nodenum));
	addr.sa_socket = htons ( socketnum );

	/*
	** Create the socket.
	*/
	Socket = socket (AF_NS, SOCK_DGRAM, NSPROTO_IPX);
	if (Socket == INVALID_SOCKET) {
		char out[128];
		sprintf (out, "TS: Failed to create IPX socket - error code %d.\n", GetLastError() );
		OutputDebugString (out);
		assert ( Socket != INVALID_SOCKET );
		closesocket(Socket);
		return ( false );
	}

	/*
	** Get the network card address. This is needed so we can bind the socket to the net card.
	*/
	if ( !Get_Network_Card_Address (0, &addr)){
		closesocket ( Socket );
		return ( false );
	}

	/*
	** Bind the IPX socket to the network card.
	*/
	if (bind ( Socket, (const struct sockaddr *) &addr, 16) == SOCKET_ERROR ){
		char out[128];
		sprintf (out, "TS: IPX socket bind failed with error code %d.\n", GetLastError() );
		OutputDebugString (out);
		assert ( false );
		closesocket(Socket);
		return ( false );;
	}


	/*
	** Set the various options for this IPX socket
	*/
	unsigned long 	optval = true;
	int 	packet_type = 4;

	/*
	** The SO_BROADCAST option allows broadcasting on this socket. This shouldn't be needed
	** except for the bug in the Win95 implementation of Winsock which causes broadcasts to
	** fail if it isn't set.
	*/
	if ( setsockopt ( Socket, SOL_SOCKET, SO_BROADCAST, (char*)&optval,	sizeof(optval) ) == SOCKET_ERROR ) {
		char out[128];
		sprintf (out, "TS: Failed to set IPX socket option SO_BROADCAST - error code %d.\n", GetLastError() );
		OutputDebugString (out);
		assert ( false );
	}

	/*
	** Set the value in the packet type field for outgoing packets.
	*/
	err = setsockopt ( Socket, NSPROTO_IPX, IPX_PTYPE, (char*)&packet_type,	sizeof(packet_type));
	if ( err == INVALID_SOCKET ) {
		char out[128];
		sprintf (out, "TS: Failed to set IPX protocol option IPX_PTYPE - error code %d.\n", GetLastError() );
		OutputDebugString (out);
		assert ( err != INVALID_SOCKET );
	}

	/*
	** Ignore all incoming packets not of this type.
	*/
	err = setsockopt ( Socket, NSPROTO_IPX, IPX_FILTERPTYPE, (char*)&packet_type,	sizeof(packet_type));
	if ( err == INVALID_SOCKET ) {
		char out[128];
		sprintf (out, "TS: Failed to set IPX protocol option IPX_FILTERTYPE - error code %d.\n", GetLastError() );
		OutputDebugString (out);
		assert ( err != INVALID_SOCKET );
	}

	/*
	** Set the the base class socket options for buffer sizes.
	*/
	WinsockInterfaceClass::Set_Socket_Options();

	/*
	** Woohoo!
	*/
	return ( true );
}









/***********************************************************************************************
 * IPXInterfaceClass::Message_Handler -- Handler for windows messages relating to IPX          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Usual windoze message handler stuff                                               *
 *                                                                                             *
 * OUTPUT:   0 if message was handled                                                          *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/4/97 5:55PM ST : Created                                                               *
 *=============================================================================================*/
long IPXInterfaceClass::Message_Handler(HWND , UINT message, UINT , LONG lParam)
{

	int					addr_len;		// Length of address structure
	int	 				rc;				// Result code
	SOCKADDR_IPX 		addr;				// Winsock IPX addressing structure
	WinsockBufferType *packet;			// Ptr to packet
	NetNumType			netnum;
	NetNodeType			nodenum;


	/*
	** We only handle IPX events.
	*/
	if ( message != WM_IPXASYNCEVENT ) return ( 1 );


	switch ( WSAGETSELECTEVENT(lParam) ) {

		/*
		** Read event. Winsock has data it would like to give us.
		*/
		case FD_READ:
			/*
			** Clear any outstanding errors on the socket.
			*/
			rc = WSAGETSELECTERROR(lParam);
			if (rc != 0) {
				Clear_Socket_Error (Socket);
				return(0);
			}

			/*
			** Call the Winsock recvfrom function to get the outstanding packet.
			*/
			addr_len = sizeof(addr);
			rc = recvfrom ( Socket, (char*) ReceiveBuffer, sizeof (ReceiveBuffer), 0, (LPSOCKADDR)&addr, &addr_len );
			if (rc == SOCKET_ERROR) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) {
					Clear_Socket_Error (Socket);
				}
				return(0);
			}

			/*
			** rc is the number of bytes received from Winsock
			*/
			if ( rc ) {

				/*
				** Make a copy of the address that this packet came from.
				*/
				memcpy ( netnum, addr.sa_netnum, sizeof (netnum) );
				memcpy ( nodenum, addr.sa_nodenum, sizeof (nodenum) );

				/*
				** If this packet was from me then ignore it.
				*/
				if ( !memcmp (netnum, BroadcastNet, sizeof (BroadcastNet)) && !memcmp(nodenum, MyNode, sizeof (MyNode)) ) {
					return (0);
				}

				/*
				** Create a new buffer and store this packet in it.
				*/
				packet = new WinsockBufferType;
				packet->BufferLen = rc;
				memcpy ( packet->Buffer, ReceiveBuffer, rc );
				IPXAddressClass *paddress = (IPXAddressClass*) (&packet->Address[0]);
				paddress->Set_Address ( netnum, nodenum );
				InBuffers.Add ( packet );
			}
			return(0);


		/*
		** Write event. We send ourselves this event when we have more data to send. This
		** event will also occur automatically when a packet has finished being sent.
		*/
		case FD_WRITE:
			/*
			** Clear any outstanding erros on the socket.
			*/
			rc = WSAGETSELECTERROR(lParam);
			if (rc != 0) {
				Clear_Socket_Error ( Socket );
				return(0);
			}

			/*
			** If there are no packets waiting to be sent then bail.
			*/
			while ( OutBuffers.Count() != 0 ) {
				int packetnum = 0;

				/*
				** Get a pointer to the packet.
				*/
				packet = OutBuffers [ packetnum ];

				/*
				** Set up the address structure of the outgoing packet
				*/
				addr.sa_family = AF_IPX;
				addr.sa_socket = htons ( IPXSocketNumber );

				/*
				** Set up the address as either a broadcast address or the given address
				*/
				if ( packet->IsBroadcast ) {
					memcpy ( addr.sa_netnum, BroadcastNet, sizeof (BroadcastNet) );
					memcpy ( addr.sa_nodenum, BroadcastNode, sizeof (BroadcastNode) );
				}else{
					IPXAddressClass *paddress = (IPXAddressClass*) (&packet->Address[0]);
					paddress->Get_Address ( netnum, nodenum );
					memcpy ( addr.sa_netnum, netnum, sizeof (netnum) );
					memcpy ( addr.sa_nodenum, nodenum, sizeof (nodenum) );
				}

				/*
				** Send it.
				** If we get a WSAWOULDBLOCK error it means that Winsock is unable to accept the packet
				** at this time. In this case, we clear the socket error and just exit. Winsock will
				** send us another WRITE message when it is ready to receive more data.
				*/
				rc = sendto ( Socket, (const char*) packet->Buffer, packet->BufferLen, 0, (LPSOCKADDR)&addr, sizeof (addr) );

				if (rc == SOCKET_ERROR){
					if (WSAGetLastError() != WSAEWOULDBLOCK) {
						Clear_Socket_Error (Socket);
						break;
					}
				}

				/*
				** Delete the sent packet.
				*/
				OutBuffers.Delete ( packetnum );
				delete packet;
			}

			return(0);
	}

	return (0);
}


