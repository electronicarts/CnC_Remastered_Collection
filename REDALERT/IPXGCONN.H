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

/* $Header: /CounterStrike/IPXGCONN.H 1     3/03/97 10:24a Joe_bostic $ */
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
 * resends, since the receive queue may receive a lot more packets than		*
 * we send out.  So, re-sends cannot be detected; the application must be	*
 * designed so that it can handle multiple copies of the same packet.		*
 *																									*
 * The class uses a slightly different header from the normal Connections;	*
 * this header includes the ProductID of the sender, so multiple 				*
 * applications can share the same socket, but by using different product	*
 * ID's, can distinguish between their own packets & others.					*
 *																									*
 *	Because of this additional header, and because Receive ACK/Retry logic	*
 * is different (we can't detect resends), the following routines are		*
 * overloaded:																					*
 *		Send_Packet:		must embed the product ID into the packet header	*
 *		Receive_Packet:	must detect resends via the LastAddress & 			*
 *								LastPacketID arrays. This class doesn't ACK 			*
 *								packets until Service_Receive_Queue is called; 		*
 *								the parent classes ACK the packet when it's 			*
 *								received.														*
 *		Get_Packet:			extracts the product ID from the header;				*
 *								doesn't care about reading packets in order			*
 *		Send					is capable of broadcasting the packet, or sending	*
 *								to a specific address										*
 *																									*
 *	This class also has the ability to cross a Novell Network Bridge.			*
 * You provide the class with the bridge address, and subsequent				*
 * broadcasts are sent across the bridge as well as to the local network.	*
 * Address-specific sends contain the destination network's address,			*
 * so they cross a bridge automatically; it's just the broadcasts				*
 * that need to know the bridge address.												*
 *																									*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef IPXGLOBALCONN_H
#define IPXGLOBALCONN_H


#include "ipxconn.h"


/*
********************************** Defines **********************************
*/
//---------------------------------------------------------------------------
// This is the header for Global Connection messages.  It includes the usual
// "standard" header that the other connections do; but it also includes an
// IPX address field, so the application can get the address of the sender
// of this message.  This address field must be provided in by the IPX
// Connection Manager class, when it calls this class's Receive_Packet
// function.
//---------------------------------------------------------------------------
typedef struct {
	CommHeaderType Header;
	unsigned short ProductID;
} GlobalHeaderType;


/*
***************************** Class Declaration *****************************
*/
class IPXGlobalConnClass : public IPXConnClass
{
	//------------------------------------------------------------------------
	// Public Interface
	//------------------------------------------------------------------------
	public:
		//.....................................................................
		// Some useful enums:
		//.....................................................................
		enum GlobalConnectionEnum {
			//..................................................................
			// This is the magic number for all Global Connections.  Having the
			// same magic number across products lets us ID different products
			// on the net.  If you change the fundamental connection protocol,
			// you must use a different magic number.
			//..................................................................
			//GLOBAL_MAGICNUM = 0x1234,	// used for C&C 1
			GLOBAL_MAGICNUM = 0x1235,		// used for C&C 0
			//..................................................................
			// These are the values used for the ProductID field in the Global
			// Message structure.  It also should be the Magic Number used for
			// the private connections within that product.
			// This list should be continually updated & kept current.  Never
			// ever ever use an old product ID for your product!
			//..................................................................
			COMMAND_AND_CONQUER = 0xaa01,
			COMMAND_AND_CONQUER0 = 0xaa00
		};

		//.....................................................................
		// Constructor/destructor.
		//.....................................................................
		IPXGlobalConnClass (int numsend, int numrecieve, int maxlen,
			unsigned short product_id);
		virtual ~IPXGlobalConnClass () {};

		//.....................................................................
		// Send/Receive routines.
		//.....................................................................
		virtual int Send_Packet (void * buf, int buflen,
			IPXAddressClass *address, int ack_req);
		virtual int Receive_Packet (void * buf, int buflen,
			IPXAddressClass *address);
		virtual int Get_Packet (void * buf, int *buflen,
			IPXAddressClass *address, unsigned short *product_id);

		//.....................................................................
		// This is for telling the connection it can cross a bridge.
		//.....................................................................
		void Set_Bridge (NetNumType bridge);

		//.....................................................................
		// The Product ID for this product.
		//.....................................................................
		unsigned short ProductID;

		//.....................................................................
		// This describes the address of a bridge we have to cross.  This class
		// supports crossing only one bridge.  Storing the bridge's network
		// number allows us to obtain its local target address only once, then
		// re-use it.
		//.....................................................................
		NetNumType BridgeNet;
		NetNodeType BridgeNode;
		int IsBridge;

	//------------------------------------------------------------------------
	// Protected Interface
	//------------------------------------------------------------------------
	protected:

		//.....................................................................
		// This is the overloaded Send routine declared in ConnectionClass, and
		// used in SequencedConnClass.  This special version sends to the address
		// stored in the extra buffer within the Queue.
		//.....................................................................
		virtual int Send (char *buf, int buflen, void *extrabuf, int extralen);

		//.....................................................................
		// This routine is overloaded from SequencedConnClass, because the
		// Global Connection needs to ACK its packets differently from the
		// other connections.
		//.....................................................................
		virtual int Service_Receive_Queue (void);

	private:
		//.....................................................................
		// Since we can't detect resends by using the PacketID (since we're
		// receiving packets from a variety of sources, all using different
		// ID's), we'll have to remember the last 'n' packet addresses & id's
		// for comparison purposes.
		// Note that, if network traffic is heavy, it's still possible for an
		// app to receive the same packet twice!
		//.....................................................................
		IPXAddressClass LastAddress[4];	// array of last 4 addresses
		unsigned long LastPacketID[4];	// array of last 4 packet ID's
		int LastRXIndex;						// index of next avail pos
};

#endif

/*************************** end of ipxgconn.h *****************************/
