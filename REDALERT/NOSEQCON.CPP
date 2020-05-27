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

/* $Header:   F:\projects\c&c\vcs\code\noseqcon.cpv   1.10   01 Mar 1996 18:08:30   JOE_BOSTIC  $ */
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
 *   NonSequencedConnClass::NonSequencedConnClass -- class constructor     *
 *   NonSequencedConnClass::~NonSequencedConnClass -- class destructor     *
 *   NonSequencedConnClass::Init -- Initializes connection queue to empty	*
 *   NonSequencedConnClass::Send_Packet -- adds a packet to the send queue	*
 *   NonSequencedConnClass::Receive_Packet -- adds packet to receive queue	*
 *   NonSequencedConnClass::Get_Packet -- gets a packet from receive queue	*
 *   NonSequencedConnClass::Service_Send_Queue -- services the send queue	*
 *   NonSequencedConnClass::Service_Receive_Queue -- services recieve queue*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "NOSEQCON.H"


/***************************************************************************
 * NonSequencedConnClass::NonSequencedConnClass -- class constructor       *
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
NonSequencedConnClass::NonSequencedConnClass (int numsend, int numreceive, 
	int maxlen, unsigned short magicnum, unsigned long retry_delta,
	unsigned long max_retries, unsigned long timeout) :
	ConnectionClass (maxlen, magicnum, retry_delta, max_retries, timeout)
{
	/*------------------------------------------------------------------------
	Allocate the packet Queue.  This will store incoming packets (which will
	be placed there by the Connection Manager), and outgoing packets (which
	are placed there by this class when it "sends" a packet).
	------------------------------------------------------------------------*/
	Queue = new CommBufferClass (numsend, numreceive, MaxPacketLen);
}


/***************************************************************************
 * NonSequencedConnClass::~NonSequencedConnClass -- class destructor       *
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
NonSequencedConnClass::~NonSequencedConnClass ()
{
	delete Queue;
}


/***************************************************************************
 * NonSequencedConnClass::Init -- Initializes connection queue to empty		*
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
void NonSequencedConnClass::Init (void)
{
	NumRecNoAck = 0;
	NumRecAck = 0;
	NumSendNoAck = 0;
	NumSendAck = 0;

	LastSeqID = 0xffffffff;
	LastReadID = 0xffffffff;

	Queue->Init();
}


/***************************************************************************
 * NonSequencedConnClass::Send_Packet -- adds a packet to the send queue	*
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
int NonSequencedConnClass::Send_Packet (void * buf, int buflen, int ack_req)
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
// Smart_Printf( "Packet ack Queued ID %d \n", ((CommHeaderType *)PacketBuf)->PacketID );
			NumSendAck++;
		} else {
// Smart_Printf( "Packet noack Queued ID %d \n", ((CommHeaderType *)PacketBuf)->PacketID );
			NumSendNoAck++;
		}
		return(true);
	} else {
// Smart_Printf( "Packet not Queued ID %d \n", ((CommHeaderType *)PacketBuf)->PacketID );
		return(false);
	}
}


/***************************************************************************
 * NonSequencedConnClass::Receive_Packet -- adds packet to the receive queue	*
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
int NonSequencedConnClass::Receive_Packet (void * buf, int buflen)
{
	CommHeaderType *packet;								// ptr to packet header
	SendQueueType *send_entry;							// ptr to send entry header
	ReceiveQueueType *rec_entry;						// ptr to recv entry header
	CommHeaderType *entry_data;						// ptr to queue entry data
	CommHeaderType ackpacket;							// ACK packet to send
	int i;
	int save_packet = 1;									// 0 = this is a resend
	int found;

	/*
	--------------------------- Check the magic # ----------------------------
	*/
	packet = (CommHeaderType *)buf;
	if (packet->MagicNumber != MagicNum) {
// Smart_Printf( "Bad Magic Number\n" );
		return(false);
	}

	/*------------------------------------------------------------------------
	Handle an incoming ACK
	------------------------------------------------------------------------*/
	if (packet->Code == PACKET_ACK) {

		for (i = 0; i < Queue->Num_Send(); i++) {
			/*
			....................... Get queue entry ptr ........................
			*/
			send_entry = Queue->Get_Send(i);
			/*
			............... If ptr is valid, get ptr to its data ...............
			*/
			if (send_entry != NULL) {
				entry_data = (CommHeaderType *)send_entry->Buffer;
				/*
				.............. If ACK is for this entry, mark it ................
				*/
				if (packet->PacketID==entry_data->PacketID && 
					entry_data->Code == PACKET_DATA_ACK) {
// Smart_Printf( "Received ACK for %d \n", packet->PacketID );
					send_entry->IsACK = 1;
					break;
				}
			}
		}

//{
//		if (i == Queue->Num_Send() ) {
// Smart_Printf( "Received bad ACK for %d \n", packet->PacketID );
//		}
//}

		return(true);
	}

	/*------------------------------------------------------------------------
	Handle an incoming PACKET_DATA_NOACK packet
	------------------------------------------------------------------------*/
	else if (packet->Code == PACKET_DATA_NOACK) {
		/*---------------------------------------------------------------------
		If there's only one slot left, don't tie up the queue with this packet
		---------------------------------------------------------------------*/
		if (Queue->Max_Receive() - Queue->Num_Receive() <= 1) {
// Smart_Printf( "Only one slot left don't tie up with DATA NOACK packet %d \n", packet->PacketID );
				return(false);
		}

		/*---------------------------------------------------------------------
		Error if we can't queue the packet
		---------------------------------------------------------------------*/
		if (!Queue->Queue_Receive (buf, buflen)) {
// Smart_Printf( "Can't Queue the packet %d \n", packet->PacketID );
			return(false);
		}

// Smart_Printf( "Queued DATA NOACK for %d \n", packet->PacketID );
		NumRecNoAck++;

		return(true);
	}

	/*------------------------------------------------------------------------
	Handle an incoming PACKET_DATA_ACK packet
	------------------------------------------------------------------------*/
	else if (packet->Code == PACKET_DATA_ACK) {
// Smart_Printf( "Looking at ID %d, LastSeqID=%d \n", packet->PacketID, LastSeqID );
		/*....................................................................
		If this is a packet requires an ACK, and it's ID is older than our
		"oldest" ID, we know it's a resend; send an ACK, but don't queue it
		....................................................................*/
		if (packet->PacketID <= LastSeqID && LastSeqID != 0xffffffff) {
// Smart_Printf( "Older than oldest\n" );
			save_packet = 0;
		}
		/*....................................................................
		Otherwise, scan the queue for this entry; if it's found, it's a
		resend, so don't save it.
		....................................................................*/
		else {
			save_packet = 1;
			for (i = 0; i < Queue->Num_Receive(); i++) {
				rec_entry = Queue->Get_Receive(i);
				if (rec_entry) {
					entry_data = (CommHeaderType *)rec_entry->Buffer;
					/*...........................................................
					Packet is found; it's a resend
					...........................................................*/
					if (entry_data->Code == PACKET_DATA_ACK &&
						entry_data->PacketID == packet->PacketID) {
// Smart_Printf( "It's a resend\n" );
						save_packet = 0;
						break;
					}
				}
			}
		}	/* end of scan for resend */

		/*---------------------------------------------------------------------
		Queue the packet & update our LastSeqID value.
		---------------------------------------------------------------------*/
		if (save_packet) {
			/*------------------------------------------------------------------
			If there's only one slot left, make sure we only put a packet in it if
			this packet will let us increment our LastSeqID; otherwise, we'll get
			stuck, forever unable to increment LastSeqID.
			------------------------------------------------------------------*/
			if (Queue->Max_Receive() - Queue->Num_Receive() <= 1) {
				if (packet->PacketID != (LastSeqID + 1) ) {
// Smart_Printf( "One slot left not what we looking for max=%d,num=%d \n",
//	Queue->Max_Receive(), Queue->Num_Receive() );
					return(0);
				}
			}

			/*------------------------------------------------------------------
			If we can't queue the packet, return; don't send an ACK.
			------------------------------------------------------------------*/
			if (!Queue->Queue_Receive (buf, buflen)) {
// Smart_Printf( "unable to queue packet\n" );
				return(0);
			}

			NumRecAck++;

			/*------------------------------------------------------------------
			Update our LastSeqID value if we can.  Anything less than LastSeqID
			we'll know is a resend.
			------------------------------------------------------------------*/
			if (packet->PacketID == (LastSeqID + 1)) {
				LastSeqID = packet->PacketID;
				/*............................................................
				Now that we have a new 'LastSeqID', search our Queue to see if
				the next ID is there; if so, keep checking for the next one;
				break only when the next one isn't found.  This forces 
				LastSeqID to be the largest possible value.
				............................................................*/
				do {
					found = 0;
					for (i = 0; i < Queue->Num_Receive(); i++) {

						rec_entry = Queue->Get_Receive(i);

						if (rec_entry) {
							entry_data = (CommHeaderType *)rec_entry->Buffer;

							/*......................................................
							Entry is found
							......................................................*/
							if (entry_data->Code == PACKET_DATA_ACK &&
								entry_data->PacketID == (LastSeqID + 1)) {

								LastSeqID = entry_data->PacketID;
								found = 1;
								break;
							}
						}
					}
				} while (found);
			}
		}	/* end of save packet */

		/*---------------------------------------------------------------------
		Send an ACK, regardless of whether this was a resend or not.
		---------------------------------------------------------------------*/
		ackpacket.MagicNumber = Magic_Num();
		ackpacket.Code = PACKET_ACK;
		ackpacket.PacketID = packet->PacketID;
// Smart_Printf( "Sending ACK for %d \n", packet->PacketID );
		Send ((char *)&ackpacket, sizeof(CommHeaderType));

		return(true);

	} else {
// Smart_Printf( "invalid packet type %d \n", packet->Code );
	}

	return(false);
}


/***************************************************************************
 * NonSequencedConnClass::Get_Packet -- gets a packet from receive queue	*
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
int NonSequencedConnClass::Get_Packet (void * buf, int *buflen)
{
	ReceiveQueueType *rec_entry;					// ptr to receive entry header
	int packetlen;										// size of received packet
	CommHeaderType *entry_data;
	int i;

	/*------------------------------------------------------------------------
	Ensure that we read the packets in order.  LastReadID is the ID of the
	last PACKET_DATA_ACK packet we read.
	------------------------------------------------------------------------*/
	for (i = 0; i < Queue->Num_Receive(); i++) {

		rec_entry = Queue->Get_Receive(i);

		/*.....................................................................
		Only read this entry if it hasn't been yet
		.....................................................................*/
		if (rec_entry && rec_entry->IsRead==0) {

			entry_data = (CommHeaderType *)rec_entry->Buffer;

			/*..................................................................
			If this is a DATA_ACK packet, its ID must be one greater than
			the last one we read.
			..................................................................*/
			if ( (entry_data->Code == PACKET_DATA_ACK) &&
				(entry_data->PacketID == (LastReadID + 1))) {

				LastReadID = entry_data->PacketID;
				rec_entry->IsRead = 1;

				packetlen = rec_entry->BufLen - sizeof(CommHeaderType);
				if (packetlen > 0) {
					memcpy(buf, rec_entry->Buffer + sizeof(CommHeaderType), packetlen);
				}
				(*buflen) = packetlen;
				return(true);
			}
			/*..................................................................
			If this is a DATA_NOACK packet, who cares what the ID is?
			..................................................................*/
			else if (entry_data->Code == PACKET_DATA_NOACK) {

				rec_entry->IsRead = 1;

				packetlen = rec_entry->BufLen - sizeof(CommHeaderType);
				if (packetlen > 0) {
					memcpy(buf, rec_entry->Buffer + sizeof(CommHeaderType), packetlen);
				}
				(*buflen) = packetlen;
				return(true);
			}
		}
	}

	return(false);
}


/***************************************************************************
 * NonSequencedConnClass::Service_Send_Queue -- services the send queue		*
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
int NonSequencedConnClass::Service_Send_Queue (void)
{
	int i;
	int num_entries;
	SendQueueType *send_entry;						// ptr to send queue entry
	CommHeaderType *packet_hdr;					// packet header
	unsigned long curtime;							// current time
	int bad_conn = 0;

	/*------------------------------------------------------------------------
	Remove any ACK'd packets from the queue
	------------------------------------------------------------------------*/
	for (i = 0; i < Queue->Num_Send(); i++) {
		/*
		------------------------- Get this queue entry ------------------------
		*/
		send_entry = Queue->Get_Send(i);
		/*
		---------------- If ACK has been received, unqueue it -----------------
		*/
		if (send_entry->IsACK) {
			/*
			................ Update this queue's response time .................
			*/
			packet_hdr = (CommHeaderType *)send_entry->Buffer;
			if (packet_hdr->Code == PACKET_DATA_ACK) {
				Queue->Add_Delay(Time() - send_entry->FirstTime);
			}
			/*
			....................... unqueue the packet .........................
			*/
			Queue->UnQueue_Send(NULL,NULL,i);
			i--;
		}
	}

	/*------------------------------------------------------------------------
	Loop through all entries in the Send queue.  [Re]Send any entries that
	need it.
	------------------------------------------------------------------------*/
	num_entries = Queue->Num_Send();

	for (i = 0; i < num_entries; i++) {
		send_entry = Queue->Get_Send(i);

		if (send_entry->IsACK) {
			continue;
		}

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
#if(0)
{
			packet_hdr = (CommHeaderType *)send_entry->Buffer;
			if (send_entry->SendCount) {
				if (packet_hdr->Code == PACKET_DATA_NOACK) {
// Smart_Printf( "Resending DATA NOACK for %d \n", packet_hdr->PacketID );
				} else {
					if (packet_hdr->Code == PACKET_DATA_ACK) {
// Smart_Printf( "Resending DATA ACK for %d \n", packet_hdr->PacketID );
					}
				} 
			} else {
				if (packet_hdr->Code == PACKET_DATA_NOACK) {
// Smart_Printf( "Sending DATA NOACK for %d \n", packet_hdr->PacketID );
				} else {
					if (packet_hdr->Code == PACKET_DATA_ACK) {
// Smart_Printf( "Sending DATA ACK for %d \n", packet_hdr->PacketID );
					}
				} 
			}
}
#endif
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
				if (packet_hdr->Code == PACKET_DATA_NOACK) {
					send_entry->IsACK = 1;
				}
			}
			/*
			......................... Update SendCount .........................
			*/
			send_entry->SendCount++;
			/*..................................................................
			Perform error detection, based on either MaxRetries or Timeout
			..................................................................*/
			if (MaxRetries != -1 && send_entry->SendCount > MaxRetries) {
// Smart_Printf( "Max Retries!!! %d !!! \n", MaxRetries );
				bad_conn = 1;
			}

			if (Timeout != -1 &&
				(send_entry->LastTime - send_entry->FirstTime) > Timeout) {
// Smart_Printf( "Timed out!!! Time %d, Timeout %d, buflen %d !!! \n",
//	(send_entry->LastTime - send_entry->FirstTime), Timeout,
//	send_entry->BufLen );
				bad_conn = 1;
			}
		}
	}

	/*------------------------------------------------------------------------
	If the connection is going bad, return an error
	------------------------------------------------------------------------*/
	if (bad_conn) {
// Smart_Printf( "Connection going bad!!! \n" );
		return(false);
	} else {
		return(true);
	}
}


/***************************************************************************
 * NonSequencedConnClass::Service_Receive_Queue -- services recieve queue	*
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
int NonSequencedConnClass::Service_Receive_Queue (void)
{
	ReceiveQueueType *rec_entry;					// ptr to receive entry header
	CommHeaderType *packet_hdr;					// packet header
	int i;

	/*------------------------------------------------------------------------
	Remove all dead packets.  
	PACKET_DATA_NOACK: if it's been read, throw it away.
	PACKET_DATA_ACK: if it's been read, and its ID is older than LastSeqID,
	throw it away.
	------------------------------------------------------------------------*/
	for (i = 0; i < Queue->Num_Receive(); i++) {
		rec_entry = Queue->Get_Receive(i);

		if (rec_entry->IsRead) {
			packet_hdr = (CommHeaderType *)(rec_entry->Buffer);

			if (packet_hdr->Code == PACKET_DATA_NOACK) {
				Queue->UnQueue_Receive(NULL,NULL,i);
				i--;
			} else {
				if (packet_hdr->PacketID < LastSeqID) {
					Queue->UnQueue_Receive(NULL,NULL,i);
					i--;
				}
			}
		}
	}

	return(true);
}


