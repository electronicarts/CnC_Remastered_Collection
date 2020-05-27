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

/* $Header: /CounterStrike/IPXGCONN.CPP 3     10/13/97 2:20p Steve_t $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPXGCONN.CPP                             *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 20, 1994                        *
 *                                                                         *
 *                  Last Update : July 6, 1995 [BRR]                       *
 *-------------------------------------------------------------------------*
 * Functions:    			                                                   *
 *   IPXGlobalConnClass::IPXGlobalConnClass -- class constructor           *
 *   IPXGlobalConnClass::~IPXGlobalConnClass -- class destructor           *
 *   IPXGlobalConnClass::Send_Packet -- adds a packet to the send queue		*
 *   IPXGlobalConnClass::Receive_Packet -- adds packet to the receive queue*
 *   IPXGlobalConnClass::Get_Packet -- gets a packet from the receive queue*
 *   IPXGlobalConnClass::Send -- sends a packet										*
 *   IPXGlobalConnClass::Service_Receive_Queue -- services receive queue	*
 *   IPXGlobalConnClass::Set_Bridge -- Sets up connection to cross a bridge*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include <stdio.h>
//#include <mem.h>
#include "ipxgconn.h"


/***************************************************************************
 * IPXGlobalConnClass::IPXGlobalConnClass -- class constructor             *
 *                                                                         *
 * This routine chains to the parent constructor, but it adjusts the size	*
 * of the packet by the added bytes in the GlobalHeaderType structure.  	*
 * This forces the parent classes to allocate the proper sized PacketBuf	*
 * for outgoing packets, and to set MaxPacketLen to the proper value.		*
 *                                                                         *
 * INPUT:                                                                  *
 *		numsend			desired # of entries for the send queue					*
 *		numreceive		desired # of entries for the receive queue				*
 *		maxlen			max length of an application packet							*
 *		product_id		unique ID for this product										*
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
IPXGlobalConnClass::IPXGlobalConnClass (int numsend, int numreceive,
	int maxlen, unsigned short product_id) :
	IPXConnClass (numsend, numreceive,
		maxlen + sizeof(GlobalHeaderType) - sizeof(CommHeaderType),
		GLOBAL_MAGICNUM, 		// magic number for this connection
		NULL,						// IPX Address (none)
		0, 						// Connection ID
		"",						// Connection Name
		sizeof (IPXAddressClass))	// extra storage for the sender's address
{
	int i;

	ProductID = product_id;
	IsBridge = 0;

	for (i = 0; i < 4; i++) {
		LastPacketID[i] = 0xffffffff;
	}
	LastRXIndex = 0;

}	/* end of IPXGlobalConnClass */


/***************************************************************************
 * IPXGlobalConnClass::Send_Packet -- adds a packet to the send queue		*
 *                                                                         *
 * This routine prefixes the given buffer with a GlobalHeaderType and		*
 * queues the resulting packet into the Send Queue.  The packet's 			*
 * MagicNumber, Code, PacketID, destination Address and ProductID are set 	*
 * here. 																						*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to send															*
 *		buflen		length of buffer														*
 *		address		address to send the packet to (NULL = Broadcast)			*
 *		ack_req		1 = ACK is required for this packet; 0 = isn't				*
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
int IPXGlobalConnClass::Send_Packet (void * buf, int buflen,
	IPXAddressClass *address, int ack_req)
{
	IPXAddressClass dest_addr;

	/*------------------------------------------------------------------------
	Store the packet's Magic Number
	------------------------------------------------------------------------*/
	((GlobalHeaderType *)PacketBuf)->Header.MagicNumber = MagicNum;

	/*------------------------------------------------------------------------
	If this is a ACK-required packet, sent to a specific system, mark it as
	ACK-required; otherwise, mark as no-ACK-required.
	------------------------------------------------------------------------*/
	if (ack_req && address != NULL) {
		((GlobalHeaderType *)PacketBuf)->Header.Code = PACKET_DATA_ACK;
	}
	else {
		((GlobalHeaderType *)PacketBuf)->Header.Code = PACKET_DATA_NOACK;
	}

	/*------------------------------------------------------------------------
	Fill in the packet ID.  This will have very limited meaning; it only
	allows us to determine if an ACK packet we receive later goes with this
	packet; it doesn't let us detect re-sends of other systems' packets.
	------------------------------------------------------------------------*/
	((GlobalHeaderType *)PacketBuf)->Header.PacketID = Queue->Send_Total();

	/*------------------------------------------------------------------------
	Set the product ID for this packet.
	------------------------------------------------------------------------*/
	((GlobalHeaderType *)PacketBuf)->ProductID = ProductID;

	/*------------------------------------------------------------------------
	Set this packet's destination address.  If no address is specified, use
	a Broadcast address (which IPXAddressClass's default constructor creates).
	------------------------------------------------------------------------*/
	if (address != NULL) {
		dest_addr = (*address);
	}

	/*------------------------------------------------------------------------
	Copy the application's data
	------------------------------------------------------------------------*/
	memcpy(PacketBuf + sizeof(GlobalHeaderType), buf, buflen);

	/*------------------------------------------------------------------------
	Queue it, along with the destination address
	------------------------------------------------------------------------*/
	return(Queue->Queue_Send(PacketBuf,buflen + sizeof(GlobalHeaderType),
		&dest_addr, sizeof (IPXAddressClass)));

}	/* end of Send_Packet */


/***************************************************************************
 * IPXGlobalConnClass::Receive_Packet -- adds packet to the receive queue	*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf		buffer to process (already includes GlobalHeaderType)			*
 *		buflen	length of buffer to process											*
 *		address	the address of the sender (the IPX Manager class must			*
 *					extract this from the IPX Header of the received packet.)	*
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
int IPXGlobalConnClass::Receive_Packet (void * buf, int buflen,
	IPXAddressClass *address)
{
	GlobalHeaderType *packet;				// ptr to this packet
	SendQueueType *send_entry;				// ptr to send entry header
	GlobalHeaderType *entry_data;			// ptr to queue entry data
	GlobalHeaderType ackpacket;			// ACK packet to send
	int i;
	int resend;

	/*------------------------------------------------------------------------
	Check the magic #
	------------------------------------------------------------------------*/
	packet = (GlobalHeaderType *)buf;
	if (packet->Header.MagicNumber!=MagicNum) {
		return(0);
	}

	/*------------------------------------------------------------------------
	Process the packet based on its Code
	------------------------------------------------------------------------*/
	switch (packet->Header.Code) {
		//.....................................................................
		// DATA_ACK: Check for a resend by comparing the source address &
		// ID of this packet with our last 4 received packets.
		// Send an ACK for the packet, regardless of whether it's a resend
		// or not.
		//.....................................................................
		case PACKET_DATA_ACK:
		{
			//..................................................................
			// Check for a resend
			//..................................................................
			resend = 0;
			for (i = 0; i < 4; i++) {
				if ((unsigned int)i >= Queue->Receive_Total()) {
					break;
				}
				if ((*address)==LastAddress[i] &&
					packet->Header.PacketID==LastPacketID[i]) {
					resend = 1;
					break;
				}
			}

			bool send_ack = true;

			//..................................................................
			// If it's not a resend, queue it; then, record the sender's address
			// & the packet ID for future resend detection.
			//..................................................................
			if (!resend) {
				if (Queue->Queue_Receive (buf, buflen, address, sizeof(IPXAddressClass))) {
					LastAddress[LastRXIndex] = (*address);
					LastPacketID[LastRXIndex] = packet->Header.PacketID;
					LastRXIndex++;
					if (LastRXIndex >= 4) {
						LastRXIndex = 0;
					}
				}else{
					//..................................................................
					// Don't send an ack if we didn't have room to store the packet.
					//..................................................................
					send_ack = false;
				}
			}


			//..................................................................
			// Send an ACK for this packet
			//..................................................................
			if (send_ack) {
				ackpacket.Header.MagicNumber = MagicNum;
				ackpacket.Header.Code = PACKET_ACK;
				ackpacket.Header.PacketID = packet->Header.PacketID;
				ackpacket.ProductID = ProductID;
				Send ((char *)&ackpacket, sizeof(GlobalHeaderType),
					address, sizeof(IPXAddressClass));
			}


			break;
		}
		/*.....................................................................
		DATA_NOACK: Queue this message, along with the sender's address.
		Don't bother checking for a Re-Send, since the other system will only
		send this packet once.
		.....................................................................*/
		case PACKET_DATA_NOACK:
			Queue->Queue_Receive (buf, buflen, address, sizeof(IPXAddressClass));
			break;

		/*.....................................................................
		ACK: If this ACK is for any of my packets, mark that packet as
		acknowledged, then throw this packet away.  Otherwise, ignore the ACK
		(if we re-sent before we received the other system's first ACK, this
		ACK will be a leftover)
		.....................................................................*/
		case PACKET_ACK:
			for (i = 0; i < Queue->Num_Send(); i++) {
				/*...............................................................
				Get queue entry ptr
				...............................................................*/
				send_entry = Queue->Get_Send(i);

				/*...............................................................
				If ptr is valid, get ptr to its data
				...............................................................*/
				entry_data = (GlobalHeaderType *)(send_entry->Buffer);

				/*...............................................................
				If ACK is for this entry, mark it
				...............................................................*/
				if (packet->Header.PacketID==entry_data->Header.PacketID &&
					entry_data->Header.Code == PACKET_DATA_ACK) {
					send_entry->IsACK = 1;
					break;
				}
			}
			break;

		/*.....................................................................
		Default: ignore the packet
		.....................................................................*/
		default:
			break;
	}

	return(1);

}	/* end of Receive_Packet */


/***************************************************************************
 * IPXGlobalConnClass::Get_Packet -- gets a packet from the receive queue	*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			location to store buffer											*
 *		buflen		filled in with length of 'buf'									*
 *		address		filled in with sender's address									*
 *		product_id	filled in with sender's ProductID								*
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
int IPXGlobalConnClass::Get_Packet (void * buf, int *buflen,
	IPXAddressClass *address, unsigned short *product_id)
{
	ReceiveQueueType *rec_entry;					// ptr to receive entry header
	GlobalHeaderType *packet;
	int packetlen;										// size of received packet

	/*------------------------------------------------------------------------
	Return if nothing to do
	------------------------------------------------------------------------*/
	if (Queue->Num_Receive() == 0) {
		return(0);
	}

	/*------------------------------------------------------------------------
	Get ptr to the next available entry
	------------------------------------------------------------------------*/
	rec_entry = Queue->Get_Receive(0);

	/*------------------------------------------------------------------------
	Read it if it's un-read
	------------------------------------------------------------------------*/
	if (rec_entry!=NULL && rec_entry->IsRead==0) {

		/*.....................................................................
		Mark as read
		.....................................................................*/
		rec_entry->IsRead = 1;

		/*.....................................................................
		Copy data packet
		.....................................................................*/
		packet = (GlobalHeaderType *)(rec_entry->Buffer);
		packetlen = rec_entry->BufLen - sizeof(GlobalHeaderType);
		if (packetlen > 0) {
			memcpy(buf, rec_entry->Buffer + sizeof(GlobalHeaderType), packetlen);
		}
		(*buflen) = packetlen;
		(*product_id) = packet->ProductID;
		(*address) = (*((IPXAddressClass *)(rec_entry->ExtraBuffer)));

		return(1);
	}

	return(0);

}	/* end of Get_Packet */


/***************************************************************************
 * IPXGlobalConnClass::Send -- sends a packet										*
 *                                                                         *
 * This routine gets invoked by NonSequencedConn, when it's processing		*
 * the Send & Receive Queues.  The buffer provided will already have the	*
 * GlobalHeaderType header embedded in it.											*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to send															*
 *		buflen		length of buffer														*
 *		extrabuf		extra buffer to send													*
 *		extralen		length of extra buffer												*
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
int IPXGlobalConnClass::Send(char *buf, int buflen, void *extrabuf, int )
{
	IPXAddressClass *addr;
	int rc;

	/*------------------------------------------------------------------------
	Extract the packet's embedded IPX address
	------------------------------------------------------------------------*/
	addr = (IPXAddressClass *)extrabuf;

	/*------------------------------------------------------------------------
	If it's a broadcast address, broadcast it
	------------------------------------------------------------------------*/
	if (addr->Is_Broadcast()) {
		return(Broadcast (buf, buflen));
	}

	/*------------------------------------------------------------------------
	Otherwise, send it
	------------------------------------------------------------------------*/
	else {
		if (IsBridge && !memcmp (addr, BridgeNet, 4)) {
			rc = Send_To (buf, buflen, addr, BridgeNode);
		}
		else {
			rc = Send_To (buf, buflen, addr, NULL);
		}
		return (rc);
	}

} 	/* end of Send */


/***************************************************************************
 * IPXGlobalConnClass::Service_Receive_Queue -- services the receive queue	*
 *                                                                         *
 * This routine is necessary because the regular ConnectionClass checks		*
 * for sequential packet ID's before removing them from the receive queue;	*
 * this class cannot do that.																*
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
int IPXGlobalConnClass::Service_Receive_Queue (void)
{
	int i;
	ReceiveQueueType *rec_entry;					// ptr to receive entry header

	//------------------------------------------------------------------------
	// Remove all dead packets:  If a packet's been read, throw it away.
	//------------------------------------------------------------------------
	for (i = 0; i < Queue->Num_Receive(); i++) {
		rec_entry = Queue->Get_Receive(i);

		if (rec_entry->IsRead) {
			Queue->UnQueue_Receive(NULL,NULL,i,NULL,NULL);
			i--;
		}
	}

	return (1);

} 	/* end of Service_Receive_Queue */


/***************************************************************************
 * Set_Bridge -- Sets up connection to cross a bridge                      *
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
void IPXGlobalConnClass::Set_Bridge(NetNumType bridge)
{
#ifdef WINSOCK_IPX

	if (Configured) {
		bridge = bridge;
		IsBridge = 0;
	}

#else	//WINSOCK_IPX
	if (Configured) {
		memcpy (BridgeNet, bridge, 4);
		memset (BridgeNode, 0xff, 6);

		if (IPX_Get_Local_Target (BridgeNet, BridgeNode, Socket, BridgeNode)==0) {
			IsBridge = 1;
		}
		else {
			IsBridge = 0;
		}
	}
#endif	//WINSOCK_IPX

} 	/* end of Set_Bridge */


/************************** end of ipxgconn.cpp ****************************/
