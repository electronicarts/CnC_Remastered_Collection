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
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : TCPIP.CPP                                *
 *                                                                         *
 *                   Programmer : Steve Tall                               *
 *                                                                         *
 *                   Start Date : March 11th, 1996                         *
 *                                                                         *
 *                  Last Update : March 20th, 1996 [ST]                    *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Overview:                                                               *
 *                                                                         *
 *  Member functions of the TcpipManagerClass which provides the Winsock   *
 * interface for C&C                                                       *
 *                                                                         *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *                                                                         *
 * TMC::TcpipManagerClass -- constructor for the TcpipManagerClass         *
 * TMC::~TcpipManagerClass -- destructor for the TcpipManagerClass         *
 * TMC::Close -- restores any currently in use Winsock resources           *
 * TMC::Init -- Initialised Winsock for use.                               *
 * TMC::Start_Server -- Initialise connection and start listening.         *
 * TMC::Read -- read any pending input from the stream socket              *
 * TMC::Write -- Send data via the Winsock streaming socket                *
 * TMC::Add_Client -- A client has requested to connect.                   *
 * TMC::Message_Handler -- Message handler for Winsock.                    *
 * TMC::Set_Host_Address -- Set the address of the host                    *
 * TMC::Start_Client -- Start trying to connect to a game host             *
 * TMC::Close_Socket -- Close an opened Winsock socket.                    *
 * TMC::Copy_To_In_Buffer -- copy data from our winsock buffer             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "function.h"
#include "tcpip.h"

#ifdef FORCE_WINSOCK


/*
** Nasty globals
*/
bool					Server;			//Is this player acting as client or server
TcpipManagerClass	Winsock;			//The object for interfacing with Winsock



/***********************************************************************************************
 * TMC::TcpipManagerClass -- constructor for the TcpipManagerClass                             *
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
TcpipManagerClass::TcpipManagerClass(void)
{
	WinsockInitialised = FALSE;
	Connected = FALSE;
	UseUDP = TRUE;
	SocketReceiveBuffer = 4096;
	SocketSendBuffer = 4096;

}


/***********************************************************************************************
 * TMC::~TcpipManagerClass -- destructor for the TcpipManagerClass                             *
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

TcpipManagerClass::~TcpipManagerClass(void)
{
	Close();
}


/***********************************************************************************************
 * TMC::Close -- restores any currently in use Winsock resources                               *
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

void TcpipManagerClass::Close(void)
{
	/*
	** If we never initialised the class in the first place then just return
	*/
	if (!WinsockInitialised) return;

	/*
	** Cancel any outstaning asyncronous events
	*/
	if (Async){
		WSACancelAsyncRequest(Async);
	}

	/*
	** Close any open sockets
	*/
	if (ConnectSocket != INVALID_SOCKET){
		Close_Socket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	if (ListenSocket != INVALID_SOCKET){
		Close_Socket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
	}

	if (UDPSocket != INVALID_SOCKET){
		Close_Socket(ListenSocket);
		UDPSocket = INVALID_SOCKET;
	}

	/*
	** Call the Winsock cleanup function to say we are finished using Winsock
	*/
	WSACleanup();

	WinsockInitialised = FALSE;
	Connected = FALSE;
}



/***********************************************************************************************
 * TMC::Init -- Initialised Winsock for use.                                                   *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   TRUE if Winsock is available and was initialised                                  *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 2:54PM ST : Created                                                              *
 *=============================================================================================*/

BOOL TcpipManagerClass::Init(void)
{
	short version;
	int 	rc;

	/*
	** Just return true if we are already set up
	*/
	if (WinsockInitialised) return (TRUE);

	/*
	** Initialise sockets to null
	*/
	ListenSocket = INVALID_SOCKET;
	ConnectSocket =INVALID_SOCKET;
	UDPSocket = INVALID_SOCKET;

	/*
	** Start WinSock, and fill in our WinSockData
	*/
	version = (WINSOCK_MINOR_VER << 8) | WINSOCK_MAJOR_VER;
	rc = WSAStartup(version, &WinsockInfo);
	if (rc != 0) {
		return (FALSE);
	}

	/*
	** Check the Winsock version number
	*/
	if ((WinsockInfo.wVersion & 0x00ff) != (version & 0x00ff) ||
		(WinsockInfo.wVersion >> 8) != (version >> 8)) {
		return (FALSE);
	}

	/*
	** Everything is OK so return success
	*/
	WinsockInitialised = TRUE;
	return (TRUE);

}




/***********************************************************************************************
 * TMC::Start_Server -- initialise out connection as the server. Start listening for clients.  *
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
 *    3/20/96 2:56PM ST : Created                                                              *
 *=============================================================================================*/

void TcpipManagerClass::Start_Server(void)
{
	int i;
	//struct sockaddr_in addr;

	Start_Client();

	/*
	** Set up the incoming and outgoing data buffers head and tail pointers
	*/
	//InBufferHead = 0;
	//InBufferTail = 0;
	//OutBufferHead= 0;
	//OutBufferTail= 0;

	TXBufferHead = 0;
	TXBufferTail = 0;
	RXBufferHead = 0;
	RXBufferTail = 0;

	for (i=0 ; i<WS_NUM_TX_BUFFERS ; i++){
		TransmitBuffers[i].InUse = false;
	}

	for (i=0 ; i<WS_NUM_RX_BUFFERS ; i++){
		ReceiveBuffers[i].InUse = false;
	}

	/*
	** Flag that we are the server side not the client
	*/
	IsServer = TRUE;
	UseUDP = TRUE;
#if (0)
	/*
	** Create our socket and bind it to our port number
	*/
	ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET) {
		ConnectStatus = NOT_CONNECTING;
		return ;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORTNUM);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ListenSocket, (LPSOCKADDR)&addr, sizeof(addr)) ==
		SOCKET_ERROR) {
		Close_Socket(ListenSocket);
		ConnectStatus = NOT_CONNECTING;
		return;
	}

	/*
	** Start listening for connections on this socket
	*/
	if (listen(ListenSocket, 1) == SOCKET_ERROR) {
		Close_Socket(ListenSocket);
		ConnectStatus = NOT_CONNECTING;
		return;
	}

	/*
	** Select the asynchronous events we want to process
	*/
	if (WSAAsyncSelect (ListenSocket, MainWindow, WM_ACCEPT, FD_ACCEPT) == SOCKET_ERROR) {
		Close_Socket(ListenSocket);
		ConnectStatus = NOT_CONNECTING;
		return;
	}
#endif
	ConnectStatus = CONNECTING;

}




/***********************************************************************************************
 * TMC::Read -- read any pending input from the stream socket                                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to buffer to receive input                                                    *
 *           length of buffer                                                                  *
 *                                                                                             *
 * OUTPUT:   number of bytes transfered to buffer                                              *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 2:58PM ST : Created                                                              *
 *=============================================================================================*/

int TcpipManagerClass::Read(void *buffer, int buffer_len)
{
	int 	bytes_copied = 0;
	char 	*dest_buf = (char*) buffer;

	/*
	** Make sure the message loop gets called because all the Winsock notifications
	** are done via messages.
	*/
	Keyboard::Check();

	/*
	** Copy any outstanding incoming data to the buffer provided
	*/
	if (ReceiveBuffers[RXBufferTail].InUse){
		memcpy (buffer, ReceiveBuffers[RXBufferTail].Buffer,
					MIN(ReceiveBuffers[RXBufferTail].DataLength, buffer_len));
		ReceiveBuffers[RXBufferTail].InUse = false;

		bytes_copied = MIN(ReceiveBuffers[RXBufferTail++].DataLength, buffer_len);

		RXBufferTail &= WS_NUM_RX_BUFFERS-1;
	}

	return (bytes_copied);
}



/***********************************************************************************************
 * TMC::Write -- Send data via the Winsock UDP socket                                          *
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

void TcpipManagerClass::Write(void *buffer, int buffer_len)
{
	char 	*source_buf = (char*) buffer;

	/*
	** Copy the data to one of the classes internal buffers
	*/
	if (!TransmitBuffers[TXBufferHead].InUse){
		memcpy (TransmitBuffers[TXBufferHead].Buffer,
					buffer,
					MIN (buffer_len, WS_INTERNET_BUFFER_LEN));
		TransmitBuffers[TXBufferHead].InUse = true;
		TransmitBuffers[TXBufferHead++].DataLength = MIN(buffer_len, WS_INTERNET_BUFFER_LEN);
		TXBufferHead &= WS_NUM_TX_BUFFERS-1;
	}

	/*
	** Send a message to ourselves to start off the event
	*/
	if (UseUDP){
		SendMessage(MainWindow, WM_UDPASYNCEVENT, 0, (LONG)FD_WRITE);
	}else{
		SendMessage(MainWindow, WM_ASYNCEVENT, 0, (LONG)FD_WRITE);
	}
	Keyboard::Check();
}






/***********************************************************************************************
 * TMC::Add_Client -- a client has requested to connect. Make the connection                   *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   TRUE if client was successfully connected                                         *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 3:02PM ST : Created                                                              *
 *=============================================================================================*/

BOOL TcpipManagerClass::Add_Client(void)
{
	struct 	sockaddr_in addr;
	int 		addrsize;
	bool 		delay = TRUE;

	/*
	** Accept the connection. If there is an error then dont do anything else
	*/
	addrsize = sizeof(addr);
	ConnectSocket = accept (ListenSocket, (LPSOCKADDR)&addr, &addrsize);
	if (ConnectSocket == INVALID_SOCKET) {
		//Show_Error("accept", WSAGetLastError());
		return(FALSE);
	}

	/*
	** Set options for this socket
	*/
	setsockopt (ConnectSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&delay, 4);
	setsockopt (ConnectSocket, SOL_SOCKET, SO_RCVBUF, (char*)&SocketReceiveBuffer, 4);
	setsockopt (ConnectSocket, SOL_SOCKET, SO_SNDBUF, (char*)&SocketSendBuffer, 4);

	/*
	** Save the clients address
	*/
	memcpy(&ClientIPAddress, &addr.sin_addr.s_addr,4);
	memcpy(&UDPIPAddress, &addr.sin_addr.s_addr,4);

	/*
	** Initiate an asynchronous host lookup by address. Our window will receive notification
	** when this is complete or when it times out.
	*/
	Async = WSAAsyncGetHostByAddr (MainWindow, WM_HOSTBYADDRESS,
		(char const *)&addr.sin_addr, 4, PF_INET, &HostBuff[0],
		MAXGETHOSTSTRUCT);

	/*
	** Enable asynchronous events on this socket
	*/
	if (WSAAsyncSelect (ConnectSocket, MainWindow, WM_ASYNCEVENT,
		FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR){
		WSACancelAsyncRequest(Async);
		Close_Socket (ConnectSocket);
		return(FALSE);
	}

	/*
	** Create our UDP socket
	*/
	UDPSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (UDPSocket == INVALID_SOCKET) {
		return (FALSE);
	}

	/*
	** Bind our UDP socket to our UDP port number
	*/
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PlanetWestwoodPortNumber);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(UDPSocket, (LPSOCKADDR)&addr, sizeof(addr)) ==
		SOCKET_ERROR) {
		Close_Socket(UDPSocket);
		ConnectStatus = NOT_CONNECTING;
		return(FALSE);
	}

	/*
	** Set options for the UDP socket
	*/
	setsockopt (UDPSocket, SOL_SOCKET, SO_RCVBUF, (char*)&SocketReceiveBuffer, 4);
	setsockopt (UDPSocket, SOL_SOCKET, SO_SNDBUF, (char*)&SocketSendBuffer, 4);

	/*
	** Enable asynchronous events on this socket
	*/
	if (WSAAsyncSelect (UDPSocket, MainWindow, WM_UDPASYNCEVENT,
		FD_READ | FD_WRITE) == SOCKET_ERROR){
		WSACancelAsyncRequest(Async);
		Close_Socket (UDPSocket);
		Close_Socket (ConnectSocket);
		return(FALSE);
	}

	return (TRUE);

}




/***********************************************************************************************
 * TMC::Message_Handler -- Message handler function for Winsock related messages               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Windows message handler stuff                                                     *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 3:05PM ST : Created                                                              *
 *=============================================================================================*/

void TcpipManagerClass::Message_Handler(HWND, UINT message, UINT , LONG lParam)
{
	struct 	hostent *hentry;
	struct 	sockaddr_in addr;
	int	 	event;
	int	 	rc;
	int		addr_len;

	switch (message){

		/*
		** Handle the GetHostByAddress result
		*/
		case WM_HOSTBYADDRESS:

			if (IsServer){
				/*
				** We are the server
				*/
				ConnectStatus = CONNECTING;
				if (WSAGETASYNCERROR(lParam)==0) {
					hentry = (struct hostent *)&HostBuff[0];
					strcpy (&ClientName[0], hentry->h_name);
				}
				Async = 0;
				return;

			}else{
				/*
				** We are the client
				*/
				ConnectStatus = CONTACTING_SERVER;
				if (WSAGETASYNCERROR(lParam)==0) {
					hentry = (struct hostent *)&HostBuff[0];
					strcpy (Server.Name, hentry->h_name);
				}
				else {
					Server.Name[0] = 0;
				}
				Async = 0;
				return;
			}


		/*
		** Retrieve host by name: Start connecting now that we have the
		** address.
		*/
		case WM_HOSTBYNAME:
			if (WSAGETASYNCERROR(lParam)==0) {
				hentry = (struct hostent *)&HostBuff[0];
				memcpy (&(Server.Addr.s_addr), hentry->h_addr, 4);
				memcpy(&UDPIPAddress, hentry->h_addr, 4);
				strcpy (Server.DotAddr, inet_ntoa(Server.Addr));
				ConnectStatus = CONNECTED_OK;
				Connected = TRUE;
			}
			else {
				Server.Name[0] = 0;
				strcpy (Server.DotAddr, "????");
				ConnectStatus = SERVER_ADDRESS_LOOKUP_FAILED;
			}
			Async = 0;
			return;


		/*
		** Connection is ready - accept the client
		*/
		case WM_ACCEPT:
			rc = WSAGETSELECTERROR(lParam);
			if (rc != 0) {
				ConnectStatus = UNABLE_TO_ACCEPT_CLIENT;
				return;
			}
			if (Add_Client()) {
				ConnectStatus = CONNECTED_OK;
				Connected = TRUE;
			}
			else {
				ConnectStatus = UNABLE_TO_ACCEPT_CLIENT;
			}
			return;



		/*
		** Handle UDP packet events
		*/
		case WM_UDPASYNCEVENT:
			event = WSAGETSELECTEVENT(lParam);
			switch (event) {

				case FD_READ:
					rc = WSAGETSELECTERROR(lParam);
					if (rc != 0) {
						Clear_Socket_Error(UDPSocket);
						return;
					}
					addr_len = sizeof(addr);
					rc = recvfrom(UDPSocket, ReceiveBuffer, WS_RECEIVE_BUFFER_LEN, 0,
					(LPSOCKADDR)&addr, &addr_len);
					if (rc == SOCKET_ERROR) {
						Clear_Socket_Error(UDPSocket);
						return;
					}
					memcpy(&UDPIPAddress, &addr.sin_addr.s_addr, 4);
					Copy_To_In_Buffer(rc);
					return;


				case FD_WRITE:
					if (UseUDP){
						rc = WSAGETSELECTERROR(lParam);
						if (rc != 0) {
							Clear_Socket_Error(UDPSocket);
							return;
						}

						addr.sin_family = AF_INET;
						addr.sin_port = htons(PlanetWestwoodPortNumber);
						memcpy (&addr.sin_addr.s_addr, &UDPIPAddress, 4);

						/*
						**  Send as many bytes as there are in the buffer; if there's
						**  an error, just bail out.  If we get a WOULDBLOCK error,
						**  WinSock will send us another message when the socket is
						**  available for another write.
						*/
						while (TransmitBuffers[TXBufferTail].InUse){
							rc = sendto(UDPSocket,
											TransmitBuffers[TXBufferTail].Buffer,
											TransmitBuffers[TXBufferTail].DataLength,
											0,
											(LPSOCKADDR)&addr,
											sizeof (addr));

							if (rc == SOCKET_ERROR){
								if (WSAGetLastError() != WSAEWOULDBLOCK) {
									Clear_Socket_Error(UDPSocket);
								}
								break;
							}
							TransmitBuffers[TXBufferTail++].InUse = false;
							TXBufferTail &= WS_NUM_TX_BUFFERS-1;
						}
						return;
					}
			}



		/*
		** Handle the asynchronous event callbacks
		*/
		case WM_ASYNCEVENT:
			event = WSAGETSELECTEVENT(lParam);
			switch (event) {
				/*
				** FD_CLOSE: the client has gone away. Remove the client from our system.
				*/
				case FD_CLOSE:
					rc = WSAGETSELECTERROR(lParam);
					if (rc != 0 && rc != WSAECONNRESET) {
						ConnectStatus = CONNECTION_LOST;
						return;
					}
					if (Async != 0) {
						WSACancelAsyncRequest(Async);
					}
					WSAAsyncSelect (ConnectSocket, MainWindow, WM_ASYNCEVENT, 0);
					Close_Socket (ConnectSocket);
					ConnectSocket = INVALID_SOCKET;
					//Connected = FALSE;
					ConnectStatus = CONNECTION_LOST;
					break;
#if (0)
				/*
				** FD_READ: Data is available on our socket. This message is sent every time
				**  data becomes available so we only have to do one read
				*/
				case FD_READ:
					if (!UseUDP){
						rc = WSAGETSELECTERROR(lParam);
						if (rc != 0) {
							return;
						}
						rc = recv(ConnectSocket, ReceiveBuffer, WS_RECEIVE_BUFFER_LEN, 0);
						if (rc == SOCKET_ERROR) {
							Clear_Socket_Error(ConnectSocket);
							return;
						}
						Copy_To_In_Buffer(rc);
						return;
					}

				/*
				** FD_WRITE: The socket is available for writing. We may actually have sent this
				** message from elewhere in the class.
				*/
				case FD_WRITE:
					rc = WSAGETSELECTERROR(lParam);
					if (rc != 0) {
						return;
					}
					/*
					**  Send as many bytes as there are in the buffer; if there's
					**  an error, just bail out.  If we get a WOULDBLOCK error,
					**  WinSock will send us another message when the socket is
					**  available for another write.
					*/
					while (OutBufferHead > OutBufferTail){
						rc = send(ConnectSocket, OutBuffer + OutBufferTail,
									OutBufferHead - OutBufferTail, 0);
						if (rc == SOCKET_ERROR){
							if (WSAGetLastError() != WSAEWOULDBLOCK) {
								Clear_Socket_Error(ConnectSocket);
							}
							break;
						}
						OutBufferTail+=rc;
					}
					if (OutBufferHead == OutBufferTail){
						OutBufferHead = OutBufferTail = 0;
					}
					return;
#endif	//(0)
				/*
				** FD_CONNECT: A connection was made, or an error occurred.
				*/
				case FD_CONNECT:
					rc = WSAGETSELECTERROR(lParam);
					if (rc != 0) {
						ConnectStatus = UNABLE_TO_CONNECT;
						return;
					}

					ConnectStatus = CONNECTED_OK;
					Connected = TRUE;
					return;

			}
	}
}



/***********************************************************************************************
 * TMC::Copy_To_In_Buffer -- copy data from our winsock buffer to our internal buffer          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    bytes to copy                                                                     *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 3:17PM ST : Created                                                              *
 *=============================================================================================*/
void TcpipManagerClass::Copy_To_In_Buffer(int bytes)
{
	if (!ReceiveBuffers[RXBufferHead].InUse){
		memcpy (ReceiveBuffers[RXBufferHead].Buffer, ReceiveBuffer, MIN(bytes, WS_INTERNET_BUFFER_LEN));
		ReceiveBuffers[RXBufferHead].InUse = true;
		ReceiveBuffers[RXBufferHead++].DataLength = MIN(bytes, WS_INTERNET_BUFFER_LEN);
		RXBufferHead &= WS_NUM_RX_BUFFERS-1;
	}
}



/***********************************************************************************************
 * TMC::Set_Host_Address -- Set the address of the host game we want to connect to             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to address string                                                             *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 3:19PM ST : Created                                                              *
 *=============================================================================================*/
void TcpipManagerClass::Set_Host_Address(char *address)
{
	strcpy(HostAddress, address);
}



/***********************************************************************************************
 * TMC::Start_Client -- Start trying to connect to a game host                                 *
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
 *    3/20/96 3:19PM ST : Created                                                              *
 *=============================================================================================*/

void TcpipManagerClass::Start_Client(void)
{
	struct 	sockaddr_in addr;
	bool		delay = true;
	int 		i;

	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*
	** Set up the incoming and outgoing data buffers head and tail pointers
	*/
//	InBufferHead = 0;
//	InBufferTail = 0;
//	OutBufferHead= 0;
//	OutBufferTail= 0;

	TXBufferHead = 0;
	TXBufferTail = 0;
	RXBufferHead = 0;
	RXBufferTail = 0;

	for (i=0 ; i<WS_NUM_TX_BUFFERS ; i++){
		TransmitBuffers[i].InUse = false;
	}

	for (i=0 ; i<WS_NUM_RX_BUFFERS ; i++){
		ReceiveBuffers[i].InUse = false;
	}

	Connected = FALSE;
	/*
	** Flag that we are the client side not the server
	*/
	IsServer = FALSE;
	UseUDP = TRUE;

	/*
	** Create our UDP socket
	*/
	UDPSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (UDPSocket == INVALID_SOCKET) {
		Close_Socket(ConnectSocket);
		ConnectStatus = NOT_CONNECTING;
		return;
	}
	/*
	** Bind our UDP socket to our UDP port number
	*/
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PlanetWestwoodPortNumber);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(UDPSocket, (LPSOCKADDR)&addr, sizeof(addr)) ==
		SOCKET_ERROR) {
		Close_Socket(UDPSocket);
		Close_Socket(ConnectSocket);
		ConnectStatus = NOT_CONNECTING;
		return;
	}

	/*
	** Set options for the UDP socket
	*/
	setsockopt (UDPSocket, SOL_SOCKET, SO_RCVBUF, (char*)&SocketReceiveBuffer, 4);
	setsockopt (UDPSocket, SOL_SOCKET, SO_SNDBUF, (char*)&SocketSendBuffer, 4);

	/*
	** Enable asynchronous events on the UDP socket
	*/
	if (WSAAsyncSelect (UDPSocket, MainWindow, WM_UDPASYNCEVENT,
		FD_READ | FD_WRITE) == SOCKET_ERROR){
		WSACancelAsyncRequest(Async);
		Close_Socket (UDPSocket);
		Close_Socket (ConnectSocket);
		ConnectStatus = NOT_CONNECTING;
		return;
	}

	/*
	** If the name is not a dot-decimal ip address then do a nameserver lookup
	*/


	Server.Addr.s_addr = inet_addr(PlanetWestwoodIPAddress);
	memcpy(&UDPIPAddress, &Server.Addr.s_addr, 4);
	if (Server.Addr.s_addr == INADDR_NONE){
		strcpy (Server.Name, PlanetWestwoodIPAddress);
		Async = WSAAsyncGetHostByName(MainWindow, WM_HOSTBYNAME,
			Server.Name, HostBuff, MAXGETHOSTSTRUCT);
		ConnectStatus = RESOLVING_HOST_ADDRESS;
	}else{
		ConnectStatus = CONNECTED_OK;
		Connected = TRUE;
	}

}



/***********************************************************************************************
 * TMC::Close_Socket -- Close an opened Winsock socket.                                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Socket to close                                                                   *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 3:24PM ST : Created                                                              *
 *=============================================================================================*/

void TcpipManagerClass::Close_Socket(SOCKET s)
{
	LINGER ling;

	ling.l_onoff = 0;		// linger off
	ling.l_linger = 0;	// timeout in seconds (ie close now)
	setsockopt(s, SOL_SOCKET, SO_LINGER, (LPSTR)&ling, sizeof(ling));
	closesocket (s);
}


void TcpipManagerClass::Set_Protocol_UDP(BOOL state)
{
	UseUDP = state;
}



void TcpipManagerClass::Clear_Socket_Error(SOCKET socket)
{
	unsigned long error_code;
	int length = 4;

	getsockopt (socket, SOL_SOCKET, SO_ERROR, (char*)&error_code, &length);
	error_code = 0;
	setsockopt (socket, SOL_SOCKET, SO_ERROR, (char*)&error_code, length);
}



#endif	//FORCE_WINSOCK


