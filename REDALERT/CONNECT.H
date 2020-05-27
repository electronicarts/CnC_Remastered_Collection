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

/* $Header: /CounterStrike/CONNECT.H 1     3/03/97 10:24a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : CONNECT.H                                *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : April 1, 1995   [BR]                 		*
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 * DESCRIPTION:																				*
 * This class represents a single "connection" with another system.  It's	*
 * a pure virtual base class that acts as a framework for other classes.	*
 *																									*
 * This class contains a CommBufferClass member, which stores received		*
 * & transmitted packets.  The ConnectionClass has virtual functions to		*
 * handle adding packets to the queue, reading them from the queue,			*
 * a Send routine for actually sending data, and a Receive_Packet function *
 * which is used to tell the connection that a new packet has come in.		*
 *																									*
 * The virtual Service routines handle all ACK & Retry logic for				*
 * communicating between this system & another.  Thus, any class derived	*
 * from this class may overload the basic ACK/Retry logic.						*
 *                                                                         *
 * THE PACKET HEADER:																		*
 * The Connection Classes prefix every packet sent with a header that's		*
 * local to this class.  The header contains a "Magic Number" which should	*
 * be unique for each product, and Packet "Code", which will tell the		*
 * receiving end if this is DATA, or an ACK packet, and a packet ID, which	*
 * is a unique numerical ID for this packet (useful for detecting resends).*
 * The header is stored with each packet in the send & receive Queues; 		*
 * it's removed before it's passed back to the application, via				*
 * Get_Packet()																				*
 *                                                                         *
 * THE CONNECTION MANAGER:																	*
 * It is assumed that there will be a "Connection Manager" class which 		*
 * will handle parsing incoming packets; it will then tell the connection	*
 * that new packets have come in, and the connection will process them in	*
 * whatever way it needs to for its protocol (check for resends, handle		*
 * ACK packets, etc).  The job of the connection manager is to parse			*
 * incoming packets & distribute them to the connections that need to		*
 * store them (for multi-connection protocols).										*
 *                                                                         *
 * NOTES ON ACK/RETRY:																		*
 * This class provides a "non-sequenced" ACK/Retry approach to packet		*
 * transmission.  It sends out as many packets as are in the queue, whose	*
 * resend delta times have expired; and it ACK's any packets its received	*
 * who haven't been ACK'd yet.  Thus, order of delivery is NOT guaranteed;	*
 * but, the performance is better than a "sequenced" approach.  Also, the	*
 * Packet ID scheme (see below) ensures that the application will read		*
 * the packets in the proper order.  Thus, this class guarantees delivery	*
 * and order of deliver.																	*
 *																									*
 * Each packet has a unique numerical ID; the ID is set to a count of the	*
 * number of packets sent.  Different count values are provided, for both 	*
 * DATA_ACK & DATA_NOACK packets.  This ensures that the counter can be 	*
 * used to detect resends of DATA_ACK packets; the counters for DATA_NOACK *
 * packets aren't currently used.  Other counters keep track of the 			*
 * last-sequentially-received packet ID (for DATA_ACK packets), so we		*
 * can check for resends & missed packets, and the last-sequentially-read	*
 * packet ID, so we can ensure the app reads the packets in order.			*
 *                                                                         *
 * If the protocol being used already guarantees delivery of packets,		*
 * no ACK is required for the packets.  In this case, the connection			*
 * class for this protocol can overload the Service routine to avoid			*
 * sending ACK packets, or the Connection Manager can just mark the			*
 * packet as ACK'd when it adds it to the Receive Queue for the connection.*
 *                                                                         *
 *	Derived classes must provide:															*
 * - Init: Initialization of any hardware-specific values.						*
 * - Send: a hardware-dependent send routine.										*
 *																									*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef CONNECTION_H
#define CONNECTION_H

/*
********************************* Includes **********************************
*/
#include "combuf.h"

/*
********************************** Defines **********************************
*/
#define CONN_DEBUG	0
/*---------------------------------------------------------------------------
This structure is the header prefixed to any packet sent by the application.
MagicNumber:	This is a number unique to the application; it's up to the
					Receive_Packet routine to check this value, to be sure we're
					not getting data from some other product.  This value should
					be unique for each application.
Code:				This will be one of the below-defined codes.
PacketID:		This is a unique numerical ID for this packet.  The Connection
					sets this ID on all packets sent out.
---------------------------------------------------------------------------*/
typedef struct {
	unsigned short MagicNumber;
	unsigned char Code;
	unsigned long PacketID;
} CommHeaderType;


/*
***************************** Class Declaration *****************************
*/
class ConnectionClass
{
	/*
	---------------------------- Public Interface ----------------------------
	*/
	public:
		/*.....................................................................
		These are the possible values for the Code field of the CommHeaderType:
		.....................................................................*/
		enum ConnectionEnum {
			PACKET_DATA_ACK,			// this is a data packet requiring an ACK
			PACKET_DATA_NOACK,		// this is a data packet not requiring an ACK
			PACKET_ACK,					// this is an ACK for a packet
			PACKET_COUNT				// for computational purposes
		};

		/*.....................................................................
		Constructor/destructor.
		.....................................................................*/
		ConnectionClass (int numsend, int numrecieve, int maxlen,
			unsigned short magicnum, unsigned long retry_delta,
			unsigned long max_retries, unsigned long timeout, int extralen = 0);
		virtual ~ConnectionClass ();

		/*.....................................................................
		Initialization.
		.....................................................................*/
		virtual void Init (void);

		/*.....................................................................
		Send/Receive routines.
		.....................................................................*/
		virtual int Send_Packet (void * buf, int buflen, int ack_req);
		virtual int Receive_Packet (void * buf, int buflen);
		virtual int Get_Packet (void * buf, int * buflen);

		/*.....................................................................
		The main polling routine for the connection.  Should be called as often
		as possible.
		.....................................................................*/
		virtual int Service (void);

		/*.....................................................................
		This routine is used by the retry logic; returns the current time in
		60ths of a second.
		.....................................................................*/
		static unsigned long Time (void);

		/*.....................................................................
		Utility routines.
		.....................................................................*/
		unsigned short Magic_Num (void) { return (MagicNum); }
		unsigned long Retry_Delta (void) { return (RetryDelta); }
		void Set_Retry_Delta (unsigned long delta) { RetryDelta = delta;}
		unsigned long Max_Retries (void) { return (MaxRetries); }
		void Set_Max_Retries (unsigned long retries) { MaxRetries = retries;}
		unsigned long Time_Out (void) { return (Timeout); }
		void Set_TimeOut (unsigned long t) { Timeout = t;}
		unsigned long Max_Packet_Len (void) { return (MaxPacketLen); }
		static char * Command_Name(int command);

		/*.....................................................................
		The packet "queue"; this non-sequenced version isn't really much of
		a queue, but more of a repository.
		.....................................................................*/
		CommBufferClass *Queue;

	/*
	-------------------------- Protected Interface ---------------------------
	*/
	protected:
		/*.....................................................................
		Routines to service the Send & Receive queues.
		.....................................................................*/
		virtual int Service_Send_Queue(void);
		virtual int Service_Receive_Queue(void);

		/*.....................................................................
		This routine actually performs a hardware-dependent data send.  It's
		pure virtual, so it must be defined by a derived class.  The routine
		is protected; it's only called by the ACK/Retry logic, not the
		application.
		.....................................................................*/
		virtual int Send(char *buf, int buflen, void *extrabuf,
			int extralen) = 0;

		/*.....................................................................
		This is the maximum packet length, including our own internal header.
		.....................................................................*/
		int MaxPacketLen;

		/*.....................................................................
		Packet staging area; this is where the CommHeaderType gets tacked onto
		the application's packet before it's sent.
		.....................................................................*/
		char *PacketBuf;

		/*.....................................................................
		This is the magic number assigned to this connection.  It is the first
		few bytes of any transmission.
		.....................................................................*/
		unsigned short MagicNum;

		/*.....................................................................
		This value determines the time delay before a packet is re-sent.
		.....................................................................*/
		unsigned long RetryDelta;

		/*.....................................................................
		This is the maximum number of retries allowed for a packet; if this
		value is exceeded, the connection is probably broken.
		.....................................................................*/
		unsigned long MaxRetries;

		/*.....................................................................
		This is the total timeout for this connection; if this time is exceeded
		on a packet, the connection is probably broken.
		.....................................................................*/
		unsigned long Timeout;

		/*.....................................................................
		Running totals of # of packets we send & receive which require an ACK,
		and those that don't.
		.....................................................................*/
		unsigned long NumRecNoAck;
		unsigned long NumRecAck;
		unsigned long NumSendNoAck;
		unsigned long NumSendAck;

		/*.....................................................................
		This is the ID of the last consecutively-received packet; anything older
		than this, we know is a resend.  Anything newer than this MUST be lying
		around in the Queue for us to detect it as a resend.
		.....................................................................*/
		unsigned long LastSeqID;

		/*.....................................................................
		This is the ID of the PACKET_DATA_ACK packet we read last; it ensures
		that the application reads that type of packet in order.
		.....................................................................*/
		unsigned long LastReadID;

		/*.....................................................................
		Names of all packet commands
		.....................................................................*/
		static char * Commands[PACKET_COUNT];
};

#endif

/**************************** end of connect.h *****************************/
