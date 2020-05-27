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

/* $Header:   F:\projects\c&c\vcs\code\ipxgconn.cpv   1.9   16 Oct 1995 16:51:00   JOE_BOSTIC  $ */
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
 *   IPXGlobalConnClass::Service_Receive_Queue -- services recieve queue	*
 *   IPXGlobalConnClass::Set_Bridge -- Sets up connection to cross a bridge*
* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


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
 *		numreceive		desired # of entries for the recieve queue				*
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
IPXGlobalConnClass::IPXGlobalConnClass (int numsend, int numreceive, int maxlen,
	unsigned short product_id) :
	IPXConnClass (numsend, numreceive,
		maxlen + sizeof(GlobalHeaderType) - sizeof(CommHeaderType),
		GLOBAL_MAGICNUM, 		// magic number for this connection
		NULL,						// IPX Address (none)
		0, 						// Connection ID
		"")						// Connection Name
{
	ProductID = product_id;
	IsBridge = 0;

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
 *		ack_req		true = ACK is required for this packet; false = isn't		*
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
	} else {
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
		((GlobalHeaderType *)PacketBuf)->Address = (*address);
	} else {
		((GlobalHeaderType *)PacketBuf)->Address = IPXAddressClass();
	}

	/*------------------------------------------------------------------------
	Copy the application's data
	------------------------------------------------------------------------*/
	memcpy(PacketBuf + sizeof(GlobalHeaderType), buf, buflen);

	/*------------------------------------------------------------------------
	Queue it
	------------------------------------------------------------------------*/
	return(Queue->Queue_Send(PacketBuf,buflen + sizeof(GlobalHeaderType)));

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
	int i;

	/*
	--------------------------- Check the magic # ----------------------------
	*/
	packet = (GlobalHeaderType *)buf;
	if (packet->Header.MagicNumber!=MagicNum) {
		return(false);
	}

	/*------------------------------------------------------------------------
	Process the packet based on its Code
	------------------------------------------------------------------------*/
	switch (packet->Header.Code) {
		/*.....................................................................
		DATA: Save the given address in the message buffer (so Get_Message()
		can extract it later), and queue this message.
		Don't bother checking for a Re-Send; since this queue is receiving data
		from multiple systems, the Total_Receive() value for this queue will
		have nothing to do with the packet's ID.  The application must deal
		with this by being able to handle multiple receipts of the same packet.
		.....................................................................*/
		case PACKET_DATA_ACK:
		case PACKET_DATA_NOACK:
			packet->Address = (*address);
			Queue->Queue_Receive (buf, buflen);
			break;

		/*.....................................................................
		ACK: If this ACK is for any of my packets, mark that packet as
		acknowledged, then throw this packet away.  Otherwise, ignore the ACK
		(if we re-sent before we received the other system's first ACK, this
		ACK will be a leftover)
		.....................................................................*/
		case PACKET_ACK:
			for (i = 0; i < Queue->Num_Send(); i++) {
				/*
				..................... Get queue entry ptr .......................
				*/
				send_entry = Queue->Get_Send(i);
				/*
				............. If ptr is valid, get ptr to its data ..............
				*/
				entry_data = (GlobalHeaderType *)(send_entry->Buffer);
				/*
				.............. If ACK is for this entry, mark it ................
				*/
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

	}	/* end of switch */

	return(true);
}


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

	/*
	------------------------ Return if nothing to do -------------------------
	*/
	if (Queue->Num_Receive() == 0) {
		return(false);
	}

	/*
	------------------ Get ptr to the next available entry -------------------
	*/
	rec_entry = Queue->Get_Receive(0);

	/*
	------------------------ Read it if it's un-read -------------------------
	*/
	if (rec_entry!=NULL && rec_entry->IsRead==0) {
		/*
		........................... Mark as read ..............................
		*/
		rec_entry->IsRead = 1;

		/*
		.......................... Copy data packet ...........................
		*/
		packet = (GlobalHeaderType *)(rec_entry->Buffer);
		packetlen = rec_entry->BufLen - sizeof(GlobalHeaderType);
		if (packetlen > 0)
			memcpy(buf, rec_entry->Buffer + sizeof(GlobalHeaderType), packetlen);
		(*buflen) = packetlen;
		(*address) = packet->Address;
		(*product_id) = packet->ProductID;

		return(true);
	}

	return(false);
}


/***************************************************************************
 * IPXGlobalConnClass::Send -- sends a packet										*
 *                                                                         *
 * This routine gets invoked by NonSequencedConn, when it's processing		*
 * the Send & Receive Queues.  The buffer provided will already have the	*
 * GlobalHeaderType header embedded in it.											*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf		buffer to send																*
 *		buflen	length of buffer															*
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
int IPXGlobalConnClass::Send(char *buf, int buflen)
{
	IPXAddressClass *addr;
	int rc;

	/*------------------------------------------------------------------------
	Extract the packet's embedded IPX address
	------------------------------------------------------------------------*/
	addr = &(((GlobalHeaderType *)buf)->Address);

	/*------------------------------------------------------------------------
	If it's a broadcast address, broadcast it
	------------------------------------------------------------------------*/
	if (addr->Is_Broadcast()) {
		return(Broadcast (buf, buflen));
	}  else {
		/*------------------------------------------------------------------------
		Otherwise, send it
		------------------------------------------------------------------------*/
		if (IsBridge && !memcmp (addr, BridgeNet, 4)) {
			rc = Send_To (buf, buflen, &(((GlobalHeaderType *)buf)->Address),
				BridgeNode);
		} else {
			rc = Send_To (buf, buflen, &(((GlobalHeaderType *)buf)->Address), NULL);
		}
		return (rc);
	}

}	/* end of Send */


/***************************************************************************
 * IPXGlobalConnClass::Service_Receive_Queue -- services the recieve queue	*
 *                                                                         *
 * This routine is necessary because the Global Connection has to ACK		*
 * a packet differently from other types of connections; its Send routine	*
 * assumes that the destination address is embedded within the outgoing		*
 * packet, so we have to create our ACK Packet using the GlobalHeaderType,	*
 * not the CommHeaderType.																	*
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
	GlobalHeaderType ackpacket;					// ACK packet to send
	ReceiveQueueType *rec_entry;					// ptr to receive entry header
	GlobalHeaderType *packet_hdr;					// packet header

	/*------------------------------------------------------------------------
	Get a pointer to the next received entry
	------------------------------------------------------------------------*/
	rec_entry = Queue->Get_Receive(0);
	if (rec_entry==NULL)
		return(1);

	/*------------------------------------------------------------------------
	If this packet doesn't require an ACK, mark it as ACK'd.
	------------------------------------------------------------------------*/
	packet_hdr = (GlobalHeaderType *)(rec_entry->Buffer);
	if (packet_hdr->Header.Code==PACKET_DATA_NOACK)
		rec_entry->IsACK = 1;

	/*------------------------------------------------------------------------
	If this packet hasn't been ACK'd, send an ACK:
	- Fill in the MagicNum & the Code
	- Set the PacketID to the same ID that the sending system used, so the
	  sending system knows which packet the ACK is for
	------------------------------------------------------------------------*/
	if (rec_entry->IsACK==0) {
		ackpacket.Header.MagicNumber = MagicNum;
		ackpacket.Header.Code = PACKET_ACK;
		ackpacket.Header.PacketID = packet_hdr->Header.PacketID;
		ackpacket.Address = packet_hdr->Address;
		ackpacket.ProductID = ProductID;

		Send ((char *)&ackpacket, sizeof(GlobalHeaderType));

		rec_entry->IsACK = 1;
	}

	/*------------------------------------------------------------------------
	If this packet has been read by the application, and has been ACK'd, and
	there is another packet in the queue behind this one, it means the other
	system got the ACK we sent for this packet; remove this packet from the
	queue.
	------------------------------------------------------------------------*/
	if (rec_entry!=NULL && rec_entry->IsRead && rec_entry->IsACK &&
		Queue->Num_Receive() > 1)
		Queue->UnQueue_Receive(NULL,NULL,0);

	return(1);

}	/* end of Service_Receive_Queue */

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
	if (Configured) {
		memcpy (BridgeNet, bridge, 4);
		memset (BridgeNode, 0xff, 6);

		if (IPX_Get_Local_Target (BridgeNet, BridgeNode, Socket, BridgeNode)==0) {
			IsBridge = 1;
		} else {
			IsBridge = 0;
		}
	}
}

