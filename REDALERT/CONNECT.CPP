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

/* $Header: /CounterStrike/CONNECT.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : CONNECT.CPP                              *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 20, 1994                        *
 *                                                                         *
 *                  Last Update : May 31, 1995 [BRR]								*
 *-------------------------------------------------------------------------*
 * Functions:    			                                                   *
 *   ConnectionClass::ConnectionClass -- class constructor                 *
 *   ConnectionClass::~ConnectionClass -- class destructor                 *
 *   ConnectionClass::Init -- Initializes connection queue to empty			*
 *   ConnectionClass::Send_Packet -- adds a packet to the send queue			*
 *   ConnectionClass::Receive_Packet -- adds packet to receive queue			*
 *   ConnectionClass::Get_Packet -- gets a packet from receive queue			*
 *   ConnectionClass::Service -- main polling routine; services packets		*
 *   ConnectionClass::Service_Send_Queue -- services the send queue			*
 *   ConnectionClass::Service_Receive_Queue -- services receive queue		*
 *   ConnectionClass::Time -- gets current time										*
 *   ConnectionClass::Command_Name -- returns name for a packet command		*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



#include	"function.h"
#include <stdio.h>
//#include <mem.h>
#include <sys\timeb.h>
#include "connect.h"

//#include "WolDebug.h"

/*
********************************* Globals ***********************************
*/
char *ConnectionClass::Commands[PACKET_COUNT] = {
	"ADATA",
	"NDATA",
	"ACK"
};


/***************************************************************************
 * ConnectionClass::ConnectionClass -- class constructor                   *
 *                                                                         *
 * INPUT:                                                                  *
 *		numsend			desired # of entries for the send queue					*
 *		numreceive		desired # of entries for the receive queue				*
 *		maxlen			max length of an application packet							*
 *		magicnum			the packet "magic number" for this connection			*
 *		retry_delta		the time to wait between sends								*
 *		max_retries		the max # of retries allowed for a packet					*
 *							(-1 means retry forever, based on this parameter)		*
 *		timeout			the max amount of time before we give up on a packet	*
 *							(-1 means retry forever, based on this parameter)		*
 *		extralen			max size of app-specific extra bytes (optional)			*
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
ConnectionClass::ConnectionClass (int numsend, int numreceive,
	int maxlen, unsigned short magicnum, unsigned long retry_delta,
	unsigned long max_retries, unsigned long timeout, int extralen)
{
	/*------------------------------------------------------------------------
	Compute our maximum packet length
	------------------------------------------------------------------------*/
	MaxPacketLen = maxlen + sizeof(CommHeaderType);

	/*------------------------------------------------------------------------
	Assign the magic number
	------------------------------------------------------------------------*/
	MagicNum = magicnum;

	/*------------------------------------------------------------------------
	Initialize the retry time.  This is the time that t2 - t1 must be greater
	than before a retry will occur.
	------------------------------------------------------------------------*/
	RetryDelta = retry_delta;

	/*------------------------------------------------------------------------
	Set the maximum allowable retries.
	------------------------------------------------------------------------*/
	MaxRetries = max_retries;

	/*------------------------------------------------------------------------
	Set the timeout for this connection.
	------------------------------------------------------------------------*/
	Timeout = timeout;

	/*------------------------------------------------------------------------
	Allocate the packet staging buffer.  This will be used to
	------------------------------------------------------------------------*/
	PacketBuf = new char[ MaxPacketLen ];

	/*------------------------------------------------------------------------
	Allocate the packet Queue.  This will store incoming packets (placed there
	by Receive_Packet), and outgoing packets (placed there by Send_Packet).
	It can optionally store "extra" bytes, which are stored along with each
	packet, but aren't transmitted as part of the packet.  If 'extralen'
	is 0, the CommBufferClass ignores this parameter.
	------------------------------------------------------------------------*/
	Queue = new CommBufferClass (numsend, numreceive, MaxPacketLen, extralen);

}	/* end of ConnectionClass */


/***************************************************************************
 * ConnectionClass::~ConnectionClass -- class destructor                   *
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
ConnectionClass::~ConnectionClass ()
{
	/*------------------------------------------------------------------------
	Free memory.
	------------------------------------------------------------------------*/
	delete [] PacketBuf;
	delete Queue;

}	/* end of ~ConnectionClass */


/***************************************************************************
 * ConnectionClass::Init -- Initializes connection queue to empty				*
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
void ConnectionClass::Init (void)
{
	NumRecNoAck = 0;
	NumRecAck = 0;
	NumSendNoAck = 0;
	NumSendAck = 0;

	LastSeqID = 0xffffffff;
	LastReadID = 0xffffffff;

	Queue->Init();

}	/* end of Init */


/***************************************************************************
 * ConnectionClass::Send_Packet -- adds a packet to the send queue			*
 *                                                                         *
 * This routine prefixes the given buffer with a CommHeaderType and			*
 * queues the resulting packet into the Send Queue.  (It's actually the		*
 * Service() routine that handles the hardware-dependent Send of the data).*
 * The packet's MagicNumber, Code, and PacketID are set here.					*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to send															*
 *		buflen		length of buffer														*
 *		ack_req		1 = ACK is required for this packet; 0 = isn't				*
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
int ConnectionClass::Send_Packet (void * buf, int buflen, int ack_req)
{
	/*------------------------------------------------------------------------
	Set the magic # for the packet
	------------------------------------------------------------------------*/
	((CommHeaderType *)PacketBuf)->MagicNumber = MagicNum;

	/*------------------------------------------------------------------------
	Set the packet Code: DATA_ACK if it requires an ACK, NOACK if it doesn't
	Set the packet ID to the appropriate counter value.
	------------------------------------------------------------------------*/
	if (ack_req) {
		((CommHeaderType *)PacketBuf)->Code = PACKET_DATA_ACK;
		((CommHeaderType *)PacketBuf)->PacketID = NumSendAck;
	}
	else {
		((CommHeaderType *)PacketBuf)->Code = PACKET_DATA_NOACK;
		((CommHeaderType *)PacketBuf)->PacketID = NumSendNoAck;
	}

	/*------------------------------------------------------------------------
	Now build the packet
	------------------------------------------------------------------------*/
	memcpy(PacketBuf + sizeof(CommHeaderType), buf, buflen);

	/*------------------------------------------------------------------------
	Add it to the queue; don't add any extra data with it.
	------------------------------------------------------------------------*/
	if (Queue->Queue_Send(PacketBuf,buflen + sizeof(CommHeaderType), NULL, 0)) {
		if (ack_req) {
			NumSendAck++;
		}
		else {
			NumSendNoAck++;
		}
		return(1);
	}
	else {
		return(0);
	}

}	/* end of Send_Packet */


/***************************************************************************
 * ConnectionClass::Receive_Packet -- adds packet to receive queue			*
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
int ConnectionClass::Receive_Packet (void * buf, int buflen)
{
	CommHeaderType *packet;								// ptr to packet header
	SendQueueType *send_entry;							// ptr to send entry header
	ReceiveQueueType *rec_entry;						// ptr to recv entry header
	CommHeaderType *entry_data;						// ptr to queue entry data
	CommHeaderType ackpacket;							// ACK packet to send
	int i;
	int save_packet = 1;									// 0 = this is a resend
	int found;

	/*------------------------------------------------------------------------
	Check the magic #
	------------------------------------------------------------------------*/
	packet = (CommHeaderType *)buf;
	if (packet->MagicNumber != MagicNum) {
		return(0);
	}

	/*------------------------------------------------------------------------
	Handle an incoming ACK
	------------------------------------------------------------------------*/
	if (packet->Code == PACKET_ACK) {

		for (i = 0; i < Queue->Num_Send(); i++) {
			/*..................................................................
			Get queue entry ptr
			..................................................................*/
			send_entry = Queue->Get_Send(i);

			/*..................................................................
			If ptr is valid, get ptr to its data
			..................................................................*/
			if (send_entry != NULL) {
				entry_data = (CommHeaderType *)send_entry->Buffer;

				/*...............................................................
				If ACK is for this entry, mark it
				...............................................................*/
				if (packet->PacketID==entry_data->PacketID &&
					entry_data->Code == PACKET_DATA_ACK) {
					send_entry->IsACK = 1;
					break;
				}
			}
		}

		return(1);
	}

	/*------------------------------------------------------------------------
	Handle an incoming PACKET_DATA_NOACK packet
	------------------------------------------------------------------------*/
	else if (packet->Code == PACKET_DATA_NOACK) {
		/*.....................................................................
		If there's only one slot left, don't tie up the queue with this packet
		.....................................................................*/
		if (Queue->Max_Receive() - Queue->Num_Receive() <= 1) {
			return(0);
		}

		/*.....................................................................
		Error if we can't queue the packet
		.....................................................................*/
		if (!Queue->Queue_Receive (buf, buflen, NULL, 0)) {
			return(0);
		}

		NumRecNoAck++;

		return(1);
	}

	/*------------------------------------------------------------------------
	Handle an incoming PACKET_DATA_ACK packet
	------------------------------------------------------------------------*/
	else if (packet->Code == PACKET_DATA_ACK) {
		/*.....................................................................
		If this is a packet requires an ACK, and it's ID is older than our
		"oldest" ID, we know it's a resend; send an ACK, but don't queue it
		.....................................................................*/
		if (packet->PacketID <= LastSeqID && LastSeqID != 0xffffffff) {
			save_packet = 0;
		}

		/*.....................................................................
		Otherwise, scan the queue for this entry; if it's found, it's a
		resend, so don't save it.
		.....................................................................*/
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
						save_packet = 0;
						break;
					}
				}
			}
		}	/* end of scan for resend */

		/*.....................................................................
		Queue the packet & update our LastSeqID value.
		.....................................................................*/
		if (save_packet) {
			/*..................................................................
			If there's only one slot left, make sure we only put a packet in it
			if this packet will let us increment our LastSeqID; otherwise, we'll
			get stuck, forever unable to increment LastSeqID.
			..................................................................*/
			if (Queue->Max_Receive() - Queue->Num_Receive() <= 1) {
				if (packet->PacketID != (LastSeqID + 1) ) {
					return(0);
				}
			}

			/*..................................................................
			If we can't queue the packet, return; don't send an ACK.
			..................................................................*/
			if (!Queue->Queue_Receive (buf, buflen, NULL, 0)) {
				return(0);
			}

			NumRecAck++;

			/*..................................................................
			Update our LastSeqID value if we can.  Anything less than LastSeqID
			we'll know is a resend.
			..................................................................*/
			if (packet->PacketID == (LastSeqID + 1)) {
				LastSeqID = packet->PacketID;
				/*...............................................................
				Now that we have a new 'LastSeqID', search our Queue to see if
				the next ID is there; if so, keep checking for the next one;
				break only when the next one isn't found.  This forces
				LastSeqID to be the largest possible value.
				...............................................................*/
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

		/*.....................................................................
		Send an ACK, regardless of whether this was a resend or not.
		.....................................................................*/
		ackpacket.MagicNumber = Magic_Num();
		ackpacket.Code = PACKET_ACK;
		ackpacket.PacketID = packet->PacketID;
		Send ((char *)&ackpacket, sizeof(CommHeaderType), NULL, 0);

		return(1);
	}

	return(0);

}	/* end of Receive_Packet */


/***************************************************************************
 * ConnectionClass::Get_Packet -- gets a packet from receive queue			*
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
int ConnectionClass::Get_Packet (void * buf, int *buflen)
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
					memcpy(buf, rec_entry->Buffer + sizeof(CommHeaderType),
						packetlen);
				}
				(*buflen) = packetlen;
				return(1);
			}
			/*..................................................................
			If this is a DATA_NOACK packet, who cares what the ID is?
			..................................................................*/
			else if (entry_data->Code == PACKET_DATA_NOACK) {
				rec_entry->IsRead = 1;

				packetlen = rec_entry->BufLen - sizeof(CommHeaderType);
				if (packetlen > 0) {
					memcpy(buf, rec_entry->Buffer + sizeof(CommHeaderType),
						packetlen);
				}
				(*buflen) = packetlen;
				return(1);
			}
		}
	}

	return(0);

}	/* end of Get_Packet */


/***************************************************************************
 * ConnectionClass::Service -- main polling routine; services packets		*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error (connection is broken!)										*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int ConnectionClass::Service (void)
{
	/*------------------------------------------------------------------------
	Service the Send Queue:  This [re]sends packets in the Send Queue which
	haven't been ACK'd yet, and if their retry timeout has expired, and
	updates the FirstTime, LastTime & SendCount values in the Queue entry.
	Entries that have been ACK'd should be removed.

	Service the Receive Queue:  This sends ACKs for packets that haven't
	been ACK'd yet.  Entries that the app has read, and have been ACK'd,
	should be removed.
	------------------------------------------------------------------------*/
	if ( Service_Send_Queue() && Service_Receive_Queue() ) {
		return(1);
	}
	else {
		return(0);
	}

}	/* end of Service */


/***************************************************************************
 * ConnectionClass::Service_Send_Queue -- services the send queue				*
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
int ConnectionClass::Service_Send_Queue (void)
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
		/*.....................................................................
		Get this queue entry
		.....................................................................*/
		send_entry = Queue->Get_Send(i);

		/*.....................................................................
		If ACK has been received, unqueue it
		.....................................................................*/
		if (send_entry->IsACK) {

			/*..................................................................
			Update this queue's response time
			..................................................................*/
			packet_hdr = (CommHeaderType *)send_entry->Buffer;
			if (packet_hdr->Code == PACKET_DATA_ACK) {
				Queue->Add_Delay(Time() - send_entry->FirstTime);
			}

			/*..................................................................
			Unqueue the packet
			..................................................................*/
			Queue->UnQueue_Send(NULL,NULL,i,NULL,NULL);
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

			/*..................................................................
			Send the message
			..................................................................*/
			Send (send_entry->Buffer, send_entry->BufLen, send_entry->ExtraBuffer,
				send_entry->ExtraLen);

			/*..................................................................
			Fill in Time fields
			..................................................................*/
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

			/*..................................................................
			Update SendCount
			..................................................................*/
			send_entry->SendCount++;

			/*..................................................................
			Perform error detection, based on either MaxRetries or Timeout
			..................................................................*/
			if (MaxRetries != -1 && send_entry->SendCount > MaxRetries) {
				bad_conn = 1;
			}

			if (Timeout != -1 &&
				(send_entry->LastTime - send_entry->FirstTime) > Timeout) {
				bad_conn = 1;
			}
		}
	}

	/*------------------------------------------------------------------------
	If the connection is going bad, return an error
	------------------------------------------------------------------------*/
	if (bad_conn) {
		return(0);
	}
	else {
		return(1);
	}

}	/* end of Service_Send_Queue */


/***************************************************************************
 * ConnectionClass::Service_Receive_Queue -- services receive queue			*
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
int ConnectionClass::Service_Receive_Queue (void)
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
				Queue->UnQueue_Receive(NULL,NULL,i,NULL,NULL);
				i--;

			}
			else if (packet_hdr->PacketID < LastSeqID) {
				Queue->UnQueue_Receive(NULL,NULL,i,NULL,NULL);
				i--;
			}
		}
	}

	return(1);

}	/* end of Service_Receive_Queue */


/***************************************************************************
 * ConnectionClass::Time -- gets current time										*
 *                                                                         *
 * INPUT:                                                                  *
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
unsigned long ConnectionClass::Time (void)
{
	static struct timeb mytime;			// DOS time
	unsigned long msec;

#ifdef WWLIB32_H

	/*------------------------------------------------------------------------
	If the Westwood timer system has been activated, use TickCount's value
	------------------------------------------------------------------------*/
	if (TimerSystemOn) {
		return(TickCount);				// Westwood Library time
	}
	/*------------------------------------------------------------------------
	Otherwise, use the DOS timer
	------------------------------------------------------------------------*/
	else {
		ftime(&mytime);
		msec = (unsigned long)mytime.time * 1000L + (unsigned long)mytime.millitm;
		return((msec / 100) * 6);
	}

#else

	/*------------------------------------------------------------------------
	If the Westwood library isn't being used, use the DOS timer.
	------------------------------------------------------------------------*/
	ftime(&mytime);
	msec = (unsigned long)mytime.time * 1000L + (unsigned long)mytime.millitm;
	return((msec / 100) * 6);

#endif

}	/* end of Time */


/***************************************************************************
 * ConnectionClass::Command_Name -- returns name for given packet command  *
 *                                                                         *
 * INPUT:                                                                  *
 *		command		packet Command value to get name for							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ptr to command name, NULL if invalid											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/31/1995 BRR : Created.                                             *
 *=========================================================================*/
char *ConnectionClass::Command_Name(int command)
{
	if (command >= 0 && command < PACKET_COUNT) {
		return(Commands[command]);
	}
	else {
		return(NULL);
	}

}	/* end of Command_Name */

/************************** end of connect.cpp *****************************/


