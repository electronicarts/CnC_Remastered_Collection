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

/* $Header: /CounterStrike/IPXCONN.H 1     3/03/97 10:24a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPXCONN.H                                *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : April 9, 1995   [BR]                 		*
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 * This is the Connection Class for IPX communications.  It inherits			*
 * a Queue, PacketBuf, timeout variables from ConnectionClass.  It 			*
 * inherits its Send_/Receive_/Get_Packet functions, and the sequenced		*
 * ACK/Retry logic in Service_Send_Queue & Service_Receive_Queue from		*
 * SequencedConnClass.  It guarantees order of delivery of packets.			*
 *																									*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef IPXCONN_H
#define IPXCONN_H


/*
********************************* Includes **********************************
*/
#include "connect.h"
#include "ipxaddr.h"


/*
***************************** Class Declaration *****************************
*/
class IPXConnClass : public ConnectionClass
{
	/*
	---------------------------- Public Interface ----------------------------
	*/
	public:
		/*.....................................................................
		Various useful enums:
		.....................................................................*/
		enum IPXConnTag {
			CONN_NAME_MAX = 40			// max # chars allowed for connection name
		};

		/*.....................................................................
		Constructor/destructor.
		.....................................................................*/
		IPXConnClass(int numsend, int numrecieve, int maxlen,
			unsigned short magicnum, IPXAddressClass *address, int id, char *name,
			int extralen = 0);
		virtual ~IPXConnClass () {};

		/*.....................................................................
		Initialization.
		.....................................................................*/
		virtual void Init (void);

		/*.....................................................................
		The Configure function is for configuring all connections at once.
		It's static because it doesn't apply to any specific connection, but
		all of them.
		.....................................................................*/
		static void Configure(unsigned short socket,
			int conn_num, ECBType *listen_ecb, ECBType *send_ecb,
			IPXHeaderType *listen_header, IPXHeaderType *send_header,
			char *listen_buf, char *send_buf, long handler_rm_ptr,
			int maxpacketlen);

		/*.....................................................................
		These routines tell IPX to start listening for packets, and to stop
		listening for packets.  They're static because they affect all
		connections at once (there's no way to turn listening on for only one
		connection; it's all or nothing).
		.....................................................................*/
		static int Start_Listening (void);
		static int Stop_Listening (void);

		/*.....................................................................
		The Destination IPX Address for this connection
		.....................................................................*/
		IPXAddressClass Address;

		/*.....................................................................
		The "Immediate" (Bridge) address for this connection, and a flag
		telling if the address has been precomputed.
		.....................................................................*/
		NetNodeType ImmediateAddress;
		int Immed_Set;

		/*.....................................................................
		Each IPX Connection can have a Name & Unique numerical ID
		.....................................................................*/
		int ID;
		char Name[CONN_NAME_MAX];

	/*
	-------------------------- Protected Interface ---------------------------
	*/
	protected:

		/*.....................................................................
		This is the overloaded Send routine declared in ConnectionClass, and
		used in SequencedConnClass.
		.....................................................................*/
		virtual int Send (char *buf, int buflen, void *extrabuf, int extralen);

		/*.....................................................................
		These are the routines that access IPX.  Open_Socket & Close_Socket are
		static because they're called by Start_Listening & Stop_Listening.
		Send_To & Broadcast are static since they're direct interfaces to IPX,
		and there's only one IPX instance running.
		.....................................................................*/
		static int Open_Socket(unsigned short socket);
		static void Close_Socket(unsigned short socket);
		static int Send_To(char *buf, int buflen, IPXAddressClass *address,
			NetNodeType immed);
		static int Broadcast(char *buf, int buflen);

		/*.....................................................................
		The socket ID for this connection
		.....................................................................*/
		static unsigned short Socket;

		/*.....................................................................
		User's local Connection # (0 = not logged in)
		.....................................................................*/
		static int ConnectionNum;

		/*.....................................................................
		This is a static version of MaxPacketLen, which is the size of the
		app's packets, plus our internal CommHeaderType.  It's used in the
		Start_Listening routine.
		.....................................................................*/
		static int PacketLen;

		/*.....................................................................
		Variables for Listening (created by the IPXManagerClass, and passed
		in via Init).  All IPX connections share these buffers.
		.....................................................................*/
		static ECBType *ListenECB;
		static IPXHeaderType *ListenHeader;
		static char *ListenBuf;

		/*.....................................................................
		Variables for Sending (created by the IPXManagerClass, and passed
		in via Init).  All IPX connections share these buffers.
		.....................................................................*/
		static ECBType *SendECB;
		static IPXHeaderType *SendHeader;
		static char *SendBuf;

		/*.....................................................................
		This is a REAL-MODE pointer to the event-service-routine for IPX.
		If it's 0, IPX will operate in polled mode.  Otherwise, the high word
		must contain the segment, and the low word must contain the offset.
		CS will be the high word value when the routine is called.  (Requiring
		the segment/offset to be computed by the caller gives the caller
		control over CS.)
		.....................................................................*/
		static long Handler;

		/*.....................................................................
		This status flag tells us if Configure() has been called or not.
		.....................................................................*/
		static int Configured;

		/*.....................................................................
		This status flag tells us if the socket has been opened or not.
		.....................................................................*/
		static int SocketOpen;

		/*.....................................................................
		This status flag tells us if Start_Listening() has been called or not.
		.....................................................................*/
		static int Listening;
};

#endif

/*************************** end of ipxconn.h ******************************/
