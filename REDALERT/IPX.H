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

/* $Header: /CounterStrike/IPX.H 1     3/03/97 10:24a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPX.H                                    *
 *                                                                         *
 *                   Programmer : Barry Nance										*
 * 										 from Client/Server LAN Programming			*
 *											 Westwood-ized by Bill Randolph				*
 *                                                                         *
 *                   Start Date : December 14, 1994                        *
 *                                                                         *
 *                  Last Update : December 14, 1994   [BR]                 *
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef IPX_H
#define IPX_H

/*
******************************** Structures *********************************
*/
typedef unsigned char NetNumType[4];
typedef unsigned char NetNodeType[6];
typedef char UserID[48];

/*---------------------------------------------------------------------------
This is the IPX Packet structure.  It's followed by the data itself, which
can be up to 546 bytes long.  Annotation of 'IPX' means IPX will set this
field; annotation of 'APP' means the application must set the field.
NOTE: All header fields are ordered high-byte,low-byte.
---------------------------------------------------------------------------*/
typedef struct IPXHEADER {
	unsigned short CheckSum;					// IPX: Not used; always 0xffff
	unsigned short Length;						// IPX: Total size, incl header & data
	unsigned char TransportControl;			// IPX: # bridges message crossed
	unsigned char PacketType;					// APP: Set to 4 for IPX (5 for SPX)
	unsigned char DestNetworkNumber[4];		// APP: destination Network Number
	unsigned char DestNetworkNode[6];		// APP: destination Node Address
	unsigned short DestNetworkSocket;		// APP: destination Socket Number
	unsigned char SourceNetworkNumber[4];	// IPX: source Network Number
	unsigned char SourceNetworkNode[6];		// IPX: source Node Address
	unsigned short SourceNetworkSocket;		// IPX: source Socket Number
} IPXHeaderType;

/*---------------------------------------------------------------------------
This is the IPX Event Control Block.  It serves as a communications area
between IPX and the application for a single IPX operation.  You should set
up a separate ECB for each IPX operation you perform.
---------------------------------------------------------------------------*/
typedef struct ECB {
	void *Link_Address;
	void (*Event_Service_Routine)(void);	// APP: event handler (NULL=none)
	unsigned char InUse;							// IPX: 0 = event complete
	unsigned char CompletionCode;				// IPX: event's return code
	unsigned short SocketNumber;				// APP: socket to send data through
	unsigned short ConnectionID;				// returned by Listen (???)
	unsigned short RestOfWorkspace;
	unsigned char DriverWorkspace[12];
	unsigned char ImmediateAddress[6];		// returned by Get_Local_Target
	unsigned short PacketCount;
	struct {
		void *Address;
		unsigned short Length;
	} Packet[2];
} ECBType;


/*---------------------------------------------------------------------------
This structure is used for calling DPMI function 0x300, Call-Real-Mode-
Interrupt.  It passes register values to & from the interrupt handler.
---------------------------------------------------------------------------*/
typedef struct {
	long edi;
	long esi;
	long ebp;
	long Reserved;
	long ebx;
	long edx;
	long ecx;
	long eax;
	short Flags;
	short es;
	short ds;
	short fs;
	short gs;
	short ip;
	short cs;
	short sp;
	short ss;
} RMIType;

/*
********************************** Defines **********************************
*/
/*---------------------------------------------------------------------------
These defines are for the IPX functions.  The function number is specified 
by placing it in BX when IPX is called.  There are two ways to invoke IPX: 
use interrupt 0x7a, or use a function whose address is obtained by calling 
interrupt 0x2f with AX=0x7a00; the function address is returned in ES:DI.  
This is the preferred method, since other apps are known to use int 0x7a.
---------------------------------------------------------------------------*/
#define	IPX_OPEN_SOCKET						0x0000
#define	IPX_CLOSE_SOCKET						0x0001
#define	IPX_GET_LOCAL_TARGET					0x0002
#define	IPX_SEND_PACKET						0x0003
#define	IPX_LISTEN_FOR_PACKET				0x0004
#define	IPX_SCHEDULE_EVENT					0x0005
#define	IPX_CANCEL_EVENT						0x0006
#define	IPX_GET_INTERVAL_MARKER				0x0008
#define	IPX_GET_INTERNETWORK_ADDRESS		0x0009
#define	IPX_RELINQUISH_CONTROL				0x000A
#define	IPX_DISCONNECT_FROM_TARGET			0x000B

/*---------------------------------------------------------------------------
These defines are for various IPX error codes:
---------------------------------------------------------------------------*/
#define	IPXERR_CONNECTION_SEVERED			0x00ec
#define	IPXERR_CONNECTION_FAILED			0x00ed
#define	IPXERR_NO_CONNECTION					0x00ee
#define	IPXERR_CONNECTION_TABLE_FULL		0x00ef
#define	IPXERR_NO_CANCEL_ECB					0x00f9
#define	IPXERR_NO_PATH							0x00fa
#define	IPXERR_ECB_INACTIVE					0x00fc
#define	IPXERR_INVALID_PACKET_LENGTH		0x00fd
#define	IPXERR_SOCKET_TABLE_FULL			0x00fe
#define	IPXERR_SOCKET_ERROR					0x00ff

/*---------------------------------------------------------------------------
These defines are for various interrupt vectors and DPMI functions:
---------------------------------------------------------------------------*/
#define	IPX_INT									0x007a
#define	DPMI_INT									0x0031
#define	DPMI_ALLOC_DOS_MEM					0x0100
#define	DPMI_FREE_DOS_MEM						0x0101
#define	DPMI_CALL_REAL_INT					0x0300
#define	DPMI_LOCK_MEM							0x0600
#define	DPMI_UNLOCK_MEM						0x0601

/*
******************************** Prototypes *********************************
*/
/*---------------------------------------------------------------------------
NOTE: All routines return 0 for a success code and one of the above IPX
error codes if there's an error, EXCEPT:
- IPX_SPX_Installed (0 = not installed)
- Get_Connection_Number / Get_1st_Connection_Number (0 = no connection)
---------------------------------------------------------------------------*/
/*
.................................. ipx.cpp ..................................
*/
int IPX_SPX_Installed(void);
int IPX_Open_Socket(unsigned short socket);
int IPX_Close_Socket(unsigned short socket);
int IPX_Get_Connection_Number(void);
int IPX_Get_1st_Connection_Num (char *username);
int IPX_Get_Internet_Address(int connection_number, 
	unsigned char *network_number, unsigned char *physical_node);
int IPX_Get_User_ID(int connection_number, char *user_id);
int IPX_Listen_For_Packet(struct ECB *ecb_ptr);
void IPX_Send_Packet(struct ECB *ecb_ptr);
int IPX_Get_Local_Target(unsigned char *dest_network, unsigned char *dest_node,
	unsigned short dest_socket, unsigned char *bridge_address);
int IPX_Cancel_Event(struct ECB *ecb_ptr);
void Let_IPX_Breath(void);

#endif

/***************************** end of ipx.h ********************************/

