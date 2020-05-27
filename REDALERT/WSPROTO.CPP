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
 *                     $Archive:: /Sun/WSProto.cpp                                            $*
 *                                                                                             *
 *                      $Author:: Joe_b                                                       $*
 *                                                                                             *
 *                     $Modtime:: 8/20/97 10:54a                                              $*
 *                                                                                             *
 *                    $Revision:: 5                                                           $*
 *                                                                                             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 *  WSProto.CPP WinsockInterfaceClass to provide an interface to Winsock protocols             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 *                                                                                             *
 * WIC::WinsockInterfaceClass -- constructor for the WinsockInterfaceClass                     *
 * WIC::~WinsockInterfaceClass -- destructor for the WinsockInterfaceClass                     *
 * WIC::Close -- Releases any currently in use Winsock resources.                              *
 * WIC::Close_Socket -- Close the communication socket if its open                             *
 * WIC::Start_Listening -- Enable callbacks for read/write events on our socket                *
 * WIC::Stop_Listening -- Disable the winsock event callback                                   *
 * WIC::Discard_In_Buffers -- Discard any packets in our incoming packet holding buffers       *
 * WIC::Discard_In_Buffers -- Discard any packets in our outgoing packet holding buffers       *
 * WIC::Init -- Initialised Winsock and this class for use.                                    *
 * WIC::Read -- read any pending input from the communications socket                          *
 * WIC::WriteTo -- Send data via the Winsock socket                                            *
 * WIC::Broadcast -- Send data via the Winsock socket                                          *
 * WIC::Clear_Socket_Error -- Clear any outstanding erros on the socket                        *
 * WIC::Set_Socket_Options -- Sets default socket options for Winsock buffer sizes             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"WSProto.h"

#include	<stdio.h>


/***********************************************************************************************
 * WIC::WinsockInterfaceClass -- constructor for the WinsockInterfaceClass                     *
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
 *    3/20/96 2:51PM ST : Created                                                              *
 *=============================================================================================*/
WinsockInterfaceClass::WinsockInterfaceClass(void)
{
	WinsockInitialised = false;
	ASync = INVALID_HANDLE_VALUE;
	Socket = INVALID_SOCKET;
}


/***********************************************************************************************
 * WIC::~WinsockInterfaceClass -- destructor for the WinsockInterfaceClass                     *
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
 *    3/20/96 2:52PM ST : Created                                                              *
 *=============================================================================================*/
WinsockInterfaceClass::~WinsockInterfaceClass(void)
{
	Close();
}


/***********************************************************************************************
 * WIC::Close -- Releases any currently in use Winsock resources.                              *
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
 *    3/20/96 2:52PM ST : Created                                                              *
 *=============================================================================================*/
void WinsockInterfaceClass::Close(void)
{
	/*
	** If we never initialised the class in the first place then just return
	*/
	if (!WinsockInitialised) return;

	/*
	** Cancel any outstaning asyncronous events
	*/
	Stop_Listening();

	/*
	** Close any open sockets
	*/
	Close_Socket();

	/*
	** Call the Winsock cleanup function to say we are finished using Winsock
	*/
	WSACleanup();

	WinsockInitialised = false;
}



/***********************************************************************************************
 * WIC::Close_Socket -- Close the communication socket if its open                             *
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
 *    8/5/97 11:53AM ST : Created                                                              *
 *=============================================================================================*/
void WinsockInterfaceClass::Close_Socket (void)
{
	if ( Socket != INVALID_SOCKET ) {
		closesocket (Socket);
		Socket = INVALID_SOCKET;
	}
}



/***********************************************************************************************
 * WIC::Start_Listening -- Enable callbacks for read/write events on our socket                *
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
 *    8/5/97 11:54AM ST : Created                                                              *
 *=============================================================================================*/
bool WinsockInterfaceClass::Start_Listening (void)
{
	/*
	** Enable asynchronous events on the socket
	*/
	if ( WSAAsyncSelect ( Socket, MainWindow, Protocol_Event_Message(), FD_READ | FD_WRITE) == SOCKET_ERROR ){
		WWDebugString ( "TS: Async select failed.\n" );
		assert (false);
		WSACancelAsyncRequest(ASync);
		ASync = INVALID_HANDLE_VALUE;
		return (false);
	}
	return (true);
}


/***********************************************************************************************
 * WIC::Stop_Listening -- Disable the winsock event callback                                   *
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
 *    8/5/97 12:06PM ST : Created                                                              *
 *=============================================================================================*/
void WinsockInterfaceClass::Stop_Listening (void)
{
	if ( ASync != INVALID_HANDLE_VALUE ) {
		WSACancelAsyncRequest ( ASync );
		ASync = INVALID_HANDLE_VALUE;
	}
}




/***********************************************************************************************
 * WIC::Discard_In_Buffers -- Discard any packets in our incoming packet holding buffers       *
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
 *    8/5/97 11:55AM ST : Created                                                              *
 *=============================================================================================*/
void WinsockInterfaceClass::Discard_In_Buffers (void)
{
	WinsockBufferType *packet;

	while ( InBuffers.Count() ) {
		packet = InBuffers [ 0 ];
		delete packet;
		InBuffers.Delete (0);
	}
}


/***********************************************************************************************
 * WIC::Discard_In_Buffers -- Discard any packets in our outgoing packet holding buffers       *
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
 *    8/5/97 11:55AM ST : Created                                                              *
 *=============================================================================================*/
void WinsockInterfaceClass::Discard_Out_Buffers (void)
{
	WinsockBufferType *packet;

	while ( OutBuffers.Count() ) {
		packet = OutBuffers [ 0 ];
		delete packet;
		OutBuffers.Delete (0);
	}
}


/***********************************************************************************************
 * WIC::Init -- Initialised Winsock and this class for use.                                    *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   true if Winsock is available and was initialised                                  *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 2:54PM ST : Created                                                              *
 *=============================================================================================*/
bool WinsockInterfaceClass::Init(void)
{
	short version;
	int 	rc;

	/*
	** Just return true if we are already set up
	*/
	if (WinsockInitialised) return (true);

	/*
	** Create a buffer much larger than the sizeof (WSADATA) would indicate since Bounds Checker
	** says that a buffer of that size gets overrun.
	*/
	char	*buffer = new char [sizeof (WSADATA) + 1024];
	WSADATA *winsock_info = (WSADATA*) (&buffer[0]);

	/*
	** Initialise socket and event handle to null
	*/
	Socket =INVALID_SOCKET;
	ASync = INVALID_HANDLE_VALUE;
	Discard_In_Buffers();
	Discard_Out_Buffers();

	/*
	** Start WinSock, and fill in our Winsock info structure
	*/
	version = (WINSOCK_MINOR_VER << 8) | WINSOCK_MAJOR_VER;
	rc = WSAStartup(version, winsock_info);
	if (rc != 0) {
		char out[128];
		sprintf (out, "TS: Winsock failed to initialise - error code %d.\n", GetLastError() );
		OutputDebugString (out);
		delete [] buffer;
		return (false);
	}

	/*
	** Check the Winsock version number
	*/
	if ((winsock_info->wVersion & 0x00ff) != (version & 0x00ff) ||
		(winsock_info->wVersion >> 8) != (version >> 8)) {
		OutputDebugString ("TS: Winsock version is less than 1.1\n" );
		delete [] buffer;
		return (false);
	}

	/*
	** Everything is OK so return success
	*/
	WinsockInitialised = true;

	delete [] buffer;
	return (true);

}




/***********************************************************************************************
 * WIC::Read -- read any pending input from the communications socket                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to buffer to receive input                                                    *
 *           length of buffer                                                                  *
 *           ptr to address to fill with address that packet was sent from                     *
 *           length of address buffer                                                          *
 *                                                                                             *
 * OUTPUT:   number of bytes transfered to buffer                                              *
 *                                                                                             *
 * WARNINGS: The format of the address is dependent on the protocol in use.                    *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 2:58PM ST : Created                                                              *
 *=============================================================================================*/
int WinsockInterfaceClass::Read(void *buffer, int &buffer_len, void *address, int &address_len)
{
	address_len = address_len;
	/*
	** Call the message loop in case there are any outstanding winsock READ messages.
	*/
	Keyboard->Check();

	/*
	** If there are no available packets then return 0
	*/
	if ( InBuffers.Count() == 0 ) return (0);

	/*
	** Get the oldest packet for reading
	*/
	int packetnum = 0;
	WinsockBufferType *packet = InBuffers [packetnum];

	assert ( buffer_len >= packet->BufferLen );
	assert ( address_len >= sizeof (packet->Address) );

	/*
	** Copy the data and the address it came from into the supplied buffers.
	*/
	memcpy ( buffer, packet->Buffer, packet->BufferLen );
	memcpy ( address, packet->Address, sizeof (packet->Address) );

	/*
	** Return the length of the packet in buffer_len.
	*/
	buffer_len = packet->BufferLen;

	/*
	** Delete the temporary storage for the packet now that it is being passed to the game.
	*/
	InBuffers.Delete ( packetnum );
	delete packet;

	return ( buffer_len );
}




/***********************************************************************************************
 * WIC::WriteTo -- Send data via the Winsock socket                                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to buffer containing data to send                                             *
 *           length of data to send                                                            *
 *           address to send data to.                                                          *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: The format of the address is dependent on the protocol in use.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 3:00PM ST : Created                                                              *
 *=============================================================================================*/
void WinsockInterfaceClass::WriteTo(void *buffer, int buffer_len, void *address)
{
	/*
	** Create a temporary holding area for the packet.
	*/
	WinsockBufferType *packet = new WinsockBufferType;

	/*
	** Copy the packet into the holding buffer.
	*/
	memcpy ( packet->Buffer, buffer, buffer_len );
	packet->BufferLen = buffer_len;
	packet->IsBroadcast = false;
//	memcpy ( packet->Address, address, sizeof (packet->Address) );
	memcpy ( packet->Address, address, sizeof( IPXAddressClass ) );		// Steve Tall has revised WriteTo due to this bug.

	/*
	** Add it to our out list.
	*/
	OutBuffers.Add ( packet );

	/*
	** Send a message to ourselves so that we can initiate a write if Winsock is idle.
	*/
	SendMessage ( MainWindow, Protocol_Event_Message(), 0, (LONG)FD_WRITE );

	/*
	** Make sure the message loop gets called.
	*/
	Keyboard->Check();
}




/***********************************************************************************************
 * WIC::Broadcast -- Send data via the Winsock socket                                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to buffer containing data to send                                             *
 *           length of data to send                                                            *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 3:00PM ST : Created                                                              *
 *=============================================================================================*/
void WinsockInterfaceClass::Broadcast (void *buffer, int buffer_len)
{

	/*
	** Create a temporary holding area for the packet.
	*/
	WinsockBufferType *packet = new WinsockBufferType;

	/*
	** Copy the packet into the holding buffer.
	*/
	memcpy ( packet->Buffer, buffer, buffer_len );
	packet->BufferLen = buffer_len;

	/*
	** Indicate that this packet should be broadcast.
	*/
	packet->IsBroadcast = true;

	/*
	** Add it to our out list.
	*/
	OutBuffers.Add ( packet );

	/*
	** Send a message to ourselves so that we can initiate a write if Winsock is idle.
	*/
	SendMessage ( MainWindow, Protocol_Event_Message(), 0, (LONG)FD_WRITE );

	/*
	** Make sure the message loop gets called.
	*/
	Keyboard->Check();
}




/***********************************************************************************************
 * WIC::Clear_Socket_Error -- Clear any outstanding erros on the socket                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Socket                                                                            *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/5/97 12:05PM ST : Created                                                              *
 *=============================================================================================*/
void WinsockInterfaceClass::Clear_Socket_Error(SOCKET socket)
{
	unsigned long error_code;
	int length = 4;

	getsockopt (socket, SOL_SOCKET, SO_ERROR, (char*)&error_code, &length);
	error_code = 0;
	setsockopt (socket, SOL_SOCKET, SO_ERROR, (char*)&error_code, length);
}







/***********************************************************************************************
 * WIC::Set_Socket_Options -- Sets default socket options for Winsock buffer sizes             *
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
 *    8/5/97 12:07PM ST : Created                                                              *
 *=============================================================================================*/
bool WinsockInterfaceClass::Set_Socket_Options ( void )
{
	static int		socket_transmit_buffer_size = SOCKET_BUFFER_SIZE;
	static int		socket_receive_buffer_size = SOCKET_BUFFER_SIZE;

	/*
	** Specify the size of the receive buffer.
	*/
	int err = setsockopt ( Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socket_receive_buffer_size, 4);
	if ( err == INVALID_SOCKET ) {
		char out[128];
		sprintf (out, "TS: Failed to set IPX socket option SO_RCVBUF - error code %d.\n", GetLastError() );
		OutputDebugString (out);
		assert ( err != INVALID_SOCKET );
	}

	/*
	** Specify the size of the send buffer.
	*/
	err = setsockopt ( Socket, SOL_SOCKET, SO_SNDBUF, (char*)&socket_transmit_buffer_size, 4);
	if ( err == INVALID_SOCKET ) {
		char out[128];
		sprintf (out, "TS: Failed to set IPX socket option SO_SNDBUF - error code %d.\n", GetLastError() );
		OutputDebugString (out);
		assert ( err != INVALID_SOCKET );
	}

	return ( true );
}


