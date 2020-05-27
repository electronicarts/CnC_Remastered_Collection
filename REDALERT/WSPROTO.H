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
 *                     $Archive:: /Sun/WSProto.h                                              $*
 *                                                                                             *
 *                      $Author:: Joe_b                                                       $*
 *                                                                                             *
 *                     $Modtime:: 8/12/97 5:42p                                               $*
 *                                                                                             *
 *                    $Revision:: 4                                                          $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef WSPROTO_H
#define WSPROTO_H

#include	"_WSProto.h"

/*
** Include standard Winsock 1.0 header file.
*/
#include	<winsock.h>

/*
** Misc defines
*/
#define WINSOCK_MINOR_VER		1			// Version of Winsock
#define WINSOCK_MAJOR_VER		1        // 	that we require

//#define WS_RECEIVE_BUFFER_LEN	32768		// Length of our temporary receive buffer. Needs to be more that the max packet size which is about 550 bytes.
//#define SOCKET_BUFFER_SIZE		32768		// Length of winsocks internal buffer.
#define WS_RECEIVE_BUFFER_LEN	1024		// Length of our temporary receive buffer.
#define SOCKET_BUFFER_SIZE		1024*128	// Length of winsocks internal buffer.

#define PLANET_WESTWOOD_HANDLE_MAX 20	// Max length of a WChat handle

/*
** Define events for Winsock callbacks
*/
#define WM_IPXASYNCEVENT		(WM_USER + 115)	// IPX socket Async event
#define WM_UDPASYNCEVENT		(WM_USER + 116)	// UDP socket Async event


/*
** Enum to identify the protocols supported by the Winsock interface.
*/
typedef enum tProtocolEnum {
	PROTOCOL_NONE,
	PROTOCOL_IPX,
	PROTOCOL_UDP
} ProtocolEnum;



/*
**
** Class to interface with Winsock. This interface only supports connectionless packet protocols
** like UDP & IPX. Connection orientated or streaming protocols like TCP are not supported by this
** class.
**
*/
class WinsockInterfaceClass {

	public:

		WinsockInterfaceClass(void);
		virtual ~WinsockInterfaceClass(void);

		bool Init(void);
		void Close(void);


		virtual void Close_Socket(void);
		virtual int  Read(void *buffer, int &buffer_len, void *address, int &address_len);
		virtual void WriteTo (void *buffer, int buffer_len, void *address);
		virtual void Broadcast (void *buffer, int buffer_len);
		virtual void Discard_In_Buffers (void);
		virtual void Discard_Out_Buffers (void);
		virtual bool Start_Listening (void);
		virtual void Stop_Listening (void);
		virtual void Clear_Socket_Error(SOCKET socket);
		virtual bool Set_Socket_Options ( void );
		virtual void Set_Broadcast_Address ( void * ) {};

		virtual ProtocolEnum Get_Protocol (void) {
			return (PROTOCOL_NONE);
		};

		virtual int Protocol_Event_Message (void) {
			return (0);
		};

		virtual bool Open_Socket ( SOCKET ) {
			return (false);
		};

	 	virtual long Message_Handler(HWND, UINT, UINT, LONG) {
			return (1);
		}


		typedef enum ConnectStatusEnum {
			CONNECTED_OK = 0,
			NOT_CONNECTING,
			CONNECTING,
			UNABLE_TO_CONNECT_TO_SERVER,
			CONTACTING_SERVER,
			SERVER_ADDRESS_LOOKUP_FAILED,
			RESOLVING_HOST_ADDRESS,
			UNABLE_TO_ACCEPT_CLIENT,
			UNABLE_TO_CONNECT,
			CONNECTION_LOST
		} ConnectStatusEnum;

		inline ConnectStatusEnum Get_Connection_Status(void) {return (ConnectStatus);}

	protected:

		/*
		** This struct contains the information needed for each incoming and outgoing packet.
		** It acts as a temporary control for these packets.
		*/
		typedef struct tWinsockBufferType {
			unsigned char		Address [64];	// Address. IN_ADDR, IPXAddressClass etc.
			int					BufferLen;		// Length of data in buffer
			bool					IsBroadcast;	// Flag to broadcast this packet
			unsigned char		Buffer[1024];	// Buffer to store packet in.
		} WinsockBufferType;

		/*
		** Array of buffers to temporarily store incoming and outgoing packets.
		*/
		DynamicVectorClass <WinsockBufferType *> InBuffers;
		DynamicVectorClass <WinsockBufferType *> OutBuffers;


		/*
		** Is Winsock present and initialised?
		*/
		bool 					WinsockInitialised;

		/*
		** Socket that communications will take place over.
		*/
		SOCKET				Socket;

		/*
		** Async object required for callbacks to our message handler.
		*/
		HANDLE				ASync;

		/*
		** Temporary receive buffer to use when querying Winsock for incoming packets.
		*/
		unsigned char		ReceiveBuffer[WS_RECEIVE_BUFFER_LEN];

		/*
		** Current connection status.
		*/
		ConnectStatusEnum	ConnectStatus;
};







#endif	//WSPROTO_H
