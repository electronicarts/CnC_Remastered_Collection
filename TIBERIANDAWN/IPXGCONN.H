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

/* $Header:   F:\projects\c&c\vcs\code\ipxgconn.h_v   1.10   16 Oct 1995 16:47:30   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPXGCONN.H                               *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : April 11, 1995   [BR]                 	*
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 * This class is a special type of IPX Connection.  It can talk to more		*
 * than one system at a time.  It can Broadcast packets to all systems,		*
 * or send a packet to one individual system.  The packets it sends to		*
 * individual systems can be DATA_NOACK or DATA_ACK packets, but the			*
 * packets broadcast have to be DATA_NOACK packets.  This class is for		*
 * only the crudest "Who-are-you" type of network communications.  Once		*
 * the IPX Address of another system is identified, a "real" IPX				*
 * Connection should be created, & further communications done through it.	*
 *																									*
 * This means that the packet ID field no longer can be used to detect		*
 * resends, since the receive queue may recieve a lot more packets than		*
 * we send out.  So, re-sends cannot be detected; the application must be	*
 * designed so that it can handle multiple copies of the same packet.		*
 *																									*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef IPXGLOBALCONN_H
#define IPXGLOBALCONN_H


#include "ipxconn.h"


/*
********************************** Defines **********************************
*/
/*---------------------------------------------------------------------------
This is the header for Global Connection messages.  It includes the usual
"standard" header that the other connections do; but it also includes an
IPX address field, so the application can get the address of the sender
of this message.  This address field must be provided in by the IXP 
Connection Manager class, when it calls this class's Receive_Packet function.
---------------------------------------------------------------------------*/
typedef struct {
	CommHeaderType Header;
	IPXAddressClass Address;
	unsigned short ProductID;
} GlobalHeaderType;


/*
***************************** Class Declaration *****************************
*/
class IPXGlobalConnClass : public IPXConnClass
{
	/*
	---------------------------- Public Interface ----------------------------
	*/
	public:
		/*.....................................................................
		Here are some useful enums:
		.....................................................................*/
		enum GlobalConnectionEnum {
			/*..................................................................
			This is the magic number for all Global Connections.  Having the
			same magic number across products lets us ID different products
			on the net.
			..................................................................*/
			GLOBAL_MAGICNUM = 0x1234,
			/*..................................................................
			These are the values used for the ProductID field in the Global
			Message structure.  It also should be the Magic Number used for the 
			private connections within that product.
			This list should be continually updated & kept current.  Never ever 
			ever use an old product ID for your product!
			..................................................................*/
			COMMAND_AND_CONQUER = 0xaa01,
		};

		/*.....................................................................
		Constructor/destructor.
		.....................................................................*/
		IPXGlobalConnClass (int numsend, int numrecieve, int maxlen, 
			unsigned short product_id);
		virtual ~IPXGlobalConnClass () {};

		/*.....................................................................
		Send/Receive routines.
		.....................................................................*/
		virtual int Send_Packet (void * buf, int buflen, 
			IPXAddressClass *address, int ack_req);
		virtual int Receive_Packet (void * buf, int buflen,
			IPXAddressClass *address);
		virtual int Get_Packet (void * buf, int *buflen,
			IPXAddressClass *address, unsigned short *product_id);

		/*.....................................................................
		This is for telling the connection it can cross a bridge.
		.....................................................................*/
		void Set_Bridge (NetNumType bridge);

		/*.....................................................................
		The Product ID for this product.
		.....................................................................*/
		unsigned short ProductID;

		/*.....................................................................
		This describes the address of a bridge we have to cross.  This class
		supports crossing only one bridge.  Storing the bridge's network number
		allows us to obtain its local target address only once, then re-use it.
		.....................................................................*/
		NetNumType BridgeNet;
		NetNodeType BridgeNode;
		int IsBridge;

	/*
	-------------------------- Protected Interface ---------------------------
	*/
	protected:

		/*.....................................................................
		This is the overloaded Send routine declared in ConnectionClass, and
		used in SequencedConnClass.  This special version sends to the address
		embedded within the GlobalHeaderType.
		.....................................................................*/
		virtual int Send (char *buf, int buflen);

		/*.....................................................................
		This routine is overloaded from SequencedConnClass, because the
		Global Connection needs to ACK its packets differently from the
		other connections.
		.....................................................................*/
		virtual int Service_Receive_Queue (void);
};

#endif
