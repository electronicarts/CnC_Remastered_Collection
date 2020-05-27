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

/* $Header:   F:\projects\c&c\vcs\code\seqconn.cpv   1.10   01 Mar 1996 18:29:54   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : SEQCONN.CPP                              *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 20, 1994                        *
 *                                                                         *
 *                  Last Update : April 9, 1995 [BRR]								*
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:    			                                                   *
 *   SequencedConnClass::SequencedConnClass -- class constructor           *
 *   SequencedConnClass::~SequencedConnClass -- class destructor           *
 *   SequencedConnClass::Init -- Initializes connection queue to empty		*
 *   SequencedConnClass::Send_Packet -- adds a packet to the send queue		*
 *   SequencedConnClass::Receive_Packet -- adds packet to receive queue		*
 *   SequencedConnClass::Get_Packet -- gets a packet from receive queue		*
 *   SequencedConnClass::Service_Send_Queue -- services the send queue		*
 *   SequencedConnClass::Service_Receive_Queue -- services recieve queue	*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if (0)//PG
#include "function.h"

#include "WolDebug.h"

/***************************************************************************
 * SequencedConnClass::SequencedConnClass -- class constructor             *
 *                                                                         *
 * INPUT:                                                                  *
 *		numsend			desired # of entries for the send queue					*
 *		numreceive		desired # of entries for the recieve queue				*
 *		maxlen			max length of an application packet							*
 *		magicnum			the packet "magic number" for this connection			*
 *		retry_delta		the time to wait between sends								*
 *		max_retries		the max # of retries allowed for a packet					*
 *							(-1 means retry forever, based on this parameter)		*
 *		timeout			the max amount of time before we give up on a packet	*
 *							(-1 means retry forever, based on this parameter)		*
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
SequencedConnClass::SequencedConnClass (int numsend, int numreceive, 
	int maxlen, unsigned short magicnum, unsigned long retry_delta,
	unsigned long max_retries, unsigned long timeout) :
	ConnectionClass (maxlen, magicnum, retry_delta, max_retries, timeout)
{
	NumRecNoAck = 0;
	NumRecAck = 0;
	NumSendNoAck = 0;
	NumSendAck = 0;

	/*------------------------------------------------------------------------
	Allocate the packet Queue.  This will store incoming packets (which will
	be placed there by the Connection Manager), and outgoing packets (which
	are placed there by this class when it "sends" a packet).
	------------------------------------------------------------------------*/
	Queue = new CommQueueClass (numsend, numreceive, MaxPacketLen);

}	/* end of SequencedConnClass */


/***************************************************************************
 * SequencedConnClass::~SequencedConnClass -- class destructor             *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
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
SequencedConnClass::~SequencedConnClass ()
{
	delete Queue;
}


/***************************************************************************
 * SequencedConnClass::Init -- Initializes connection queue to empty			*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
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
void SequencedConnClass::Init (void)
{
	Queue->Init();

}	/* end of Init */


/***************************************************************************
 * SequencedConnClass::Send_Packet -- adds a packet to the send queue		*
 *                                                                         *
 * This routine prefixes the given buffer with a CommHeaderType and			*
 * queues the resulting packet into the Send Queue.  (It's actually the		*
 * Service() routine that handles the hardware-dependent Send of the data).*
 * The packet's MagicNumber, Code, and PacketID are set here.					*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to send															*
 *		buflen		length of buffer														*
 *		ack_req		true = ACK is required for this packet; false = isn't		*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = packet was queue'd OK, 0 = wasn't											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int SequencedConnClass::Send_Packet (void * buf, int buflen, int ack_req)
{
	/*........................................................................
	Set the magic # for the packet
	........................................................................*/
	((CommHeaderType *)PacketBuf)->MagicNumber = MagicNum;

	/*........................................................................
	Set the packet Code: DATA_ACK if it requires an ACK, NOACK if it doesn't
	Set the packet ID to the appropriate counter value.
	........................................................................*/
	if (ack_req) {
		((CommHeaderType *)PacketBuf)->Code = PACKET_DATA_ACK;
		((CommHeaderType *)PacketBuf)->PacketID = NumSendAck;
	} else {
		((CommHeaderType *)PacketBuf)->Code = PACKET_DATA_NOACK;
		((CommHeaderType *)PacketBuf)->PacketID = NumSendNoAck;
	}

	/*........................................................................
	Now build the packet
	........................................................................*/
	memcpy(PacketBuf + sizeof(CommHeaderType), buf, buflen);

	/*........................................................................
	Add it to the queue.
	........................................................................*/
	if (Queue->Queue_Send(PacketBuf,buflen + sizeof(CommHeaderType))) {
		if (ack_req) {
			NumSendAck++;
		} else {
			NumSendNoAck++;
		}
		return(true);

	} else {
		return(false);
	}
}


/***************************************************************************
 * SequencedConnClass::Receive_Packet -- adds packet to the receive queue	*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf		buffer to process (already includes CommHeaderType)			*
 *		buflen	length of buffer to process											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = packet was processed OK, 0 = error											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int SequencedConnClass::Receive_Packet (void * buf, int buflen)
{
	CommHeaderType *packet;					// ptr to this packet
	SendQueueType *send_entry;				// ptr to send entry header
	ReceiveQueueType *rec_entry;			// ptr to receive entry header
	CommHeaderType *entry_data;			// ptr to queue entry data
	int save_packet = 1;						// 0 = this is a resend, or 
													//  out-of-order packet

	/*
	--------------------------- Check the magic # ----------------------------
	*/
	packet = (CommHeaderType *)buf;
	if (packet->MagicNumber!=MagicNum)
		return(false);
	
	/*------------------------------------------------------------------------
	Process the packet based on its Code
	------------------------------------------------------------------------*/
	switch (packet->Code) {
		/*.....................................................................
		DATA: If this is a No-Ack packet, always save it.  Otherwise, only
		save it if it's received in the proper sequence.
		.....................................................................*/
		case PACKET_DATA_ACK:
		case PACKET_DATA_NOACK:
			if (packet->Code == PACKET_DATA_NOACK) {
#ifdef DEBUG_SEQ
printf("PACKET_DATA_NOACK received (%d)\n",packet->PacketID);
#endif
				save_packet = 1;
			} else {
#ifdef DEBUG_SEQ
printf("PACKET_DATA_ACK received (%d)\n",packet->PacketID);
#endif
				if ((packet->PacketID == NumRecAck)) {
					save_packet = 1;
				} else {
					save_packet = 0;
					/*...............................................................
					If this is a resend of our next-available received message, it 
					means the other app didn't get our ACK, so mark it as 
					non-acknowledged to tell Service_Receive_Queue to send an ACK.
					...............................................................*/
					rec_entry = Queue->Next_Receive();
					if (rec_entry) {
						entry_data = (CommHeaderType *)rec_entry->Buffer;
						if (entry_data->PacketID==packet->PacketID &&
							entry_data->Code == PACKET_DATA_ACK) {
							rec_entry->IsACK = 0;
#ifdef DEBUG_SEQ
printf("(Resend)\n");
#endif
						}
					}
				}
			}

			/*
			...................... queue this packet ........................
			*/
			if (save_packet) {
				if (!Queue->Queue_Receive(buf, buflen)) {
					return(false);
				}
				if (packet->Code == PACKET_DATA_ACK) {
					NumRecAck++;
				} else {
					NumRecNoAck++;
				}
			}
			break;

		/*.....................................................................
		ACK: If this ACK is for the latest-sent packet, mark that packet as 
		acknowledged, then throw this packet away.  Otherwise, ignore the ACK 
		(if we re-sent before we received the other system's first ACK, this 
		ACK will be a leftover)
		.....................................................................*/
		case PACKET_ACK:
#ifdef DEBUG_SEQ
printf("ACK received (%d)\n",packet->PacketID);
#endif
			/*
			....................... Get queue entry ptr ........................
			*/
			send_entry = Queue->Next_Send();
			/*
			............... If ptr is valid, get ptr to its data ...............
			*/
			if (send_entry!=NULL) {
				entry_data = (CommHeaderType *)send_entry->Buffer;

				/*
				.............. If ACK is for this entry, mark it ................
				*/
				if (packet->PacketID==entry_data->PacketID && 
					entry_data->Code == PACKET_DATA_ACK) {
					send_entry->IsACK = 1;
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
 * SequencedConnClass::Get_Packet -- gets a packet from the receive queue	*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf		location to store buffer												*
 *		buflen	filled in with length of 'buf'										*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = packet was read, 0 = wasn't													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int SequencedConnClass::Get_Packet (void * buf, int *buflen)
{
	ReceiveQueueType *rec_entry;					// ptr to receive entry header
	int packetlen;										// size of received packet

	/*
	------------------ Get ptr to the next available entry -------------------
	*/
	rec_entry = Queue->Next_Receive();

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
		packetlen = rec_entry->BufLen - sizeof(CommHeaderType);
		if (packetlen > 0) {
			memcpy(buf, rec_entry->Buffer + sizeof(CommHeaderType), packetlen);
		}
		(*buflen) = packetlen;
		return(true);
	}

	return(false);
}


/***************************************************************************
 * SequencedConnClass::Service_Send_Queue -- services the send queue			*
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
int SequencedConnClass::Service_Send_Queue (void)
{
	SendQueueType *send_entry;						// ptr to send queue entry
	CommHeaderType *packet_hdr;					// packet header
	unsigned long curtime;							// current time

	/*------------------------------------------------------------------------
	- If the next packet is ACK'd remove it from the queue
	- If the next packet isn't ACK'd, [re-]send it
	------------------------------------------------------------------------*/
	/*
	......................... Get ptr to data to send ........................
	*/
	send_entry = Queue->Next_Send();
	if (send_entry==NULL)
		return(true);

	/*
	------------------ If ACK has been received, unqueue it ------------------
	*/
	if (send_entry->IsACK) {

		/*
		.................. Update this queue's response time ..................
		*/
		packet_hdr = (CommHeaderType *)send_entry->Buffer;
		if (packet_hdr->Code == PACKET_DATA_ACK) {
			Queue->Add_Delay(Time() - send_entry->FirstTime);
		}

		/*
		......................... unqueue the packet ..........................
		*/
#ifdef DEBUG_SEQ
printf(">>>Unqueueing Receive packet #%d<<<\n",packet_hdr->PacketID);
#endif
		Queue->UnQueue_Send(NULL,NULL);
	} else {

		/*
		----------------- ACK not received yet, [re-]send packet -----------------
		*/
		/*.....................................................................
		Only send the message if time has elapsed.  (The message's Time
		fields are init'd to 0 when a message is queue'd or unqueue'd, so the
		first time through, the delta time will appear large.)
		.....................................................................*/
		curtime = Time();
		if (curtime - send_entry->LastTime > RetryDelta) {
			/*
			......................... Send the message .........................
			*/
			Send (send_entry->Buffer, send_entry->BufLen);
			/*
			....................... Fill in Time fields ........................
			*/
			send_entry->LastTime = curtime;
			if (send_entry->SendCount==0) {
				send_entry->FirstTime = curtime;
				/*...............................................................
				If this is the 1st time we're sending this packet, and it doesn't
				require an ACK, mark it as ACK'd; then, the next time through,
				it will just be removed from the queue.
				...............................................................*/
				packet_hdr = (CommHeaderType *)send_entry->Buffer;
				if (packet_hdr->Code == PACKET_DATA_NOACK)
					send_entry->IsACK = 1;
			}

#ifdef DEBUG_SEQ
packet_hdr = (CommHeaderType *)send_entry->Buffer;
if (packet_hdr->Code == PACKET_DATA_NOACK) {
	printf("Sending PACKET_DATA_NOACK (%d)\n",packet_hdr->PacketID);
} else {
	printf("Sending PACKET_DATA_ACK (%d)\n",packet_hdr->PacketID);
}
#endif
			/*
			......................... Update SendCount .........................
			*/
			send_entry->SendCount++;
			/*..................................................................
			Perform error detection, based on either MaxRetries or Timeout
			..................................................................*/
			if (MaxRetries != -1 && send_entry->SendCount > MaxRetries)
				return(false);
			if (Timeout != -1 && send_entry->LastTime - 
				send_entry->FirstTime > Timeout)
				return(false);

		}
	}

	return(true);
}


/***************************************************************************
 * SequencedConnClass::Service_Receive_Queue -- services the recieve queue	*
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
int SequencedConnClass::Service_Receive_Queue (void)
{
	CommHeaderType ackpacket;						// ACK packet to send
	ReceiveQueueType *rec_entry;					// ptr to receive entry header
	CommHeaderType *packet_hdr;					// packet header

	/*------------------------------------------------------------------------
	Get a pointer to the next received entry
	------------------------------------------------------------------------*/
	rec_entry = Queue->Next_Receive();
	if (rec_entry==NULL)
		return(true);

	/*------------------------------------------------------------------------
	If this packet doesn't require an ACK, mark it as ACK'd.
	------------------------------------------------------------------------*/
	packet_hdr = (CommHeaderType *)(rec_entry->Buffer);
	if (packet_hdr->Code==PACKET_DATA_NOACK)
		rec_entry->IsACK = 1;

	/*------------------------------------------------------------------------
	If this packet hasn't been ACK'd, send an ACK:
	- Fill in the MagicNum & the Code
	- Set the PacketID to the same ID that the sending system used, so the
	  sending system knows which packet the ACK is for
	------------------------------------------------------------------------*/
	if (rec_entry->IsACK==0) {
#ifdef DEBUG_SEQ
printf("Sending ACK (%d)\n",packet_hdr->PacketID);
#endif
		ackpacket.MagicNumber = MagicNum;
		ackpacket.Code = PACKET_ACK;
		ackpacket.PacketID = packet_hdr->PacketID;

		Send((char *)&ackpacket, sizeof(CommHeaderType));

		rec_entry->IsACK = 1;
	}
			
	/*------------------------------------------------------------------------
	If this packet has been read by the application, and has been ACK'd, and
	there is another packet in the queue behind this one, it means the other
	system got the ACK we sent for this packet; remove this packet from the
	queue.
	------------------------------------------------------------------------*/
	if (rec_entry!=NULL && rec_entry->IsRead && rec_entry->IsACK &&
		Queue->Num_Receive() > 1) {
#ifdef DEBUG_SEQ
printf(">>>Unqueueing Send packet #%d<<<\n",packet_hdr->PacketID);
#endif
		Queue->UnQueue_Receive(NULL,NULL);
	}

	return(true);
}

#endif