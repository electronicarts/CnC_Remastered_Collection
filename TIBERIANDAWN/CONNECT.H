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

/* $Header:   F:\projects\c&c\vcs\code\connect.h_v   1.12   16 Oct 1995 16:46:04   JOE_BOSTIC  $ */
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
 * This class contains a CommQueueClass member, which stores received		*
 * & transmitted packets.  The ConnectionClass has virtual functions to		*
 * handle adding packets to the queue, reading them from the queue,			*
 * a Send routine for actually sending data, and a Receive_Packet function *
 * which is used to tell the connection that a new packet has come in.		*
 *																									*
 * The virtual Service routine will handle all ACK & Retry logic for			*
 * communicating between this system & another.  Thus, any class derived	*
 * from this class must provide the basic ACK/Retry logic.						*
 *                                                                         *
 * THE HEADER:																					*
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
 * The packet's ID is used to check for re-sends.  The ID is set to the 	*
 * Queue's total Send Count; if the receiving system checks this value, 	*
 * and it's less than that system's Receive Count, this is a resend.  		*
 * (ie packet 0 will be the 1st packet sent, since the Send Queue's count 	*
 * is 0 when it's sent;  as soon as it's received, the Receive Count goes 	*
 * up to 1, and an ID of 0 then means a resend.)  This scheme keeps the 	*
 * application from seeing the same packet twice.  All the Connection 		*
 * Manager has to do is mark the resent packet as non-ACK'd.  Of course, 	*
 * the Manager doesn't have to use this value at all.								*
 *                                                                         *
 * Both DATA_ACK packets and DATA_NOACK packets must go through the Send	*
 * Queue when "sent", so that the SendTotal value for this system				*
 * will still match the ReceiveTotal value for the other system; this is	*
 * why a NOACK packet can't just be sent immediately; it must go through	*
 * the queue.																					*
 *                                                                         *
 * If the protocol being used already guarantees delivery of packets,		*
 * no ACK is required for the packets.  In this case, the connection			*
 * class for this protocol can overload the Service routine to avoid			*
 * sending ACK packets, or the Connection Manager can just mark the			*
 * packet as ACK'd when it adds it to the Receive Queue for the connection.*
 *                                                                         *
 *	Derived classes must provide:															*
 * - Init						a version of Init that gives the connection 		*
 *									access to any hardware-specific values it needs	*
 *									Must chain to the parent's Init routine.			*
 * - Send_Packet				adds the CommHeaderType header, adds the packet	*
 *									to the out-going queue									*
 * - Receive_Packet			processes incoming ACK packets, detects resends,*
 *									adds new packets to the in-coming queue			*
 * - Get_Packet				reads the next-available packet from the			*
 *									receive queue												*
 * - Send						the hardware-dependent data-sending routine		*
 * - Service_Send_Queue		services the send queue; handles re-sends,		*
 *									detects when outgoing packets have been ACK'd;	*
 *									cleans out the queue of old packets					*
 * - Service_Receive_Queue	services the receive queue; handles sending		*
 *									ACK's for new or re-sent packets; cleans out		*
 *									the queue of old packets								*
 *																									*
 *	Any other routines can be overloaded as the derived class needs.			*
 *																									*
 *	CLASS HIERARCHY:																			*
 *										ConnectionClass										*
 *												 |													*
 *												 |													*
 *						--------------------------------------							*
 *						| 	   	 		  		    		       |							*
 *						|        				  		          |							*
 *			SequencedConnClass   		 	       NonSequencedConnClass			*
 *						|										       |							*
 *						|									          |							*
 *			   IPXConnClass     			        ------------------------			*
 *						|							     |                      |			*
 *						|							     |                      |			*
 *		   IPXGlobalConnClass        NullModemConnClass       ModemConnClass	*
 *																									*
 *																									*
 *	ConnectionClass:																			*
 *		Abstract base class.																	*
 *		Provides: Queue for sent/recv'd packets										*
 *		          PacketBuf for preparing packets										*
 *					 Timeout variables														*
 *					 Service() routine														*
 *																									*
 *	SequencedConnClass:																		*
 *		Abstract base class																	*
 *		Provides: * "Sequenced" ACK/Retry logic, in Service_Send_Queue() &	*
 *						Service_Receive_Queue() routines									*
 *					 * Send_Packet(): adds header to packet, adds it to Queue	*
 *					 * Receive_Packet(): adds incoming packet to receive Queue, *
 *					   handles incoming ACK's & resends									*
 *					 * Get_Packet(): gets packet from the receive queue			*
 *																									*
 *	NonSequencedConnClass:																	*
 *		Abstract base class																	*
 *		Provides: * "Non-Sequenced" ACK/Retry logic, in Service_Send_Queue()	*
 *						& Service_Receive_Queue() routines								*
 *					 * Send_Packet(): adds header to packet, adds it to Queue	*
 *					 * Receive_Packet(): adds incoming packet to receive Queue, *
 *					   handles incoming ACK's & resends									*
 *					 * Get_Packet(): gets packet from the receive queue			*
 *																									*
 *	IPXConnClass:																				*
 *		Provides: * Hardware-dependent IPX interface routines, which allow	*
 *					   Service_Send_Queue() & Service_Receive_Queue() to do		*
 *						their job																*
 *					 * Ability to associate an IPX Address, a numerical ID, and	*
 *					   a character-string Name with a connection						*
 *		Inherits: * Sequenced ACK/Retry logic, Service routines, Queue &		*
 *						PacketBuf, timeout variables										*
 *																									*
 *	IPXGlobalConnClass:																		*
 *		Special type of IPX Connection; supports receiving packets from		*
 *		multiple systems at once, and sending packets via Broadcast or			*
 *		to a specific address.																*
 *		Provides: * Specialized Receive_Packet() routine, which handles		*
 *						receiving packets from multiple systems						*
 *					 * Specialized Send_Packet() & Get_Packet() routines,			*
 *					   which pass IPX address of destination through to			*
 *						the application, giving the application control over		*
 *						whether the packet will be Broadcast or sent to a 			*
 *						specific destination (embeds destination address within	*
 *						the packet itself)													*
 *					 * Specialized Send routine, which extracts the destination	*
 *					   address from the packet												*
 *		Inherits: * Sequenced ACK/Retry logic, Service routines, Queue &		*
 *						PacketBuf, timeout variables, IPX-specific routines		*
 *																									*
 *	NullModemConnClass:																		*
 *		Provides: * Hardware-dependent Serial-communication routines, which	*
 *						allow Service_Send_Queue() & Service_Receive_Queue() to 	*
 *						do their job															*
 *		Inherits: * Non-Sequenced ACK/Retry logic, Service routines, Queue &	*
 *						PacketBuf, timeout variables										*
 *																									*
 *	ModemConnClass:																			*
 *		Provides: * Hardware-dependent Modem-communication routines, which	*
 *						allow Service_Send_Queue() & Service_Receive_Queue() to 	*
 *						do their job															*
 *		Inherits: * Non-Sequenced ACK/Retry logic, Service routines, Queue &	*
 *						PacketBuf, timeout variables										*
 *																									*
 *	So, do ya think this header is long enough, or what?							*
 *																									*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CONNECTION_H
#define CONNECTION_H

#define CONN_DEBUG	0

/*
********************************** Defines **********************************
*/
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
			PACKET_COUNT,				// for computational purposes
		};

		/*.....................................................................
		Constructor/destructor.
		.....................................................................*/
		ConnectionClass (int maxlen, unsigned short magicnum, 
			unsigned long retry_delta, unsigned long max_retries, 
			unsigned long timeout);
		virtual ~ConnectionClass ();

		/*.....................................................................
		Initialization.
		.....................................................................*/
		virtual void Init (void) {};

		/*.....................................................................
		Send/Receive routines.
		.....................................................................*/
		virtual int Send_Packet (void * buf, int buflen, int ack_req) = 0;
		virtual int Receive_Packet (void * buf, int buflen) = 0;
		virtual int Get_Packet (void * buf, int * buflen) = 0;

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

	/*
	-------------------------- Protected Interface ---------------------------
	*/
	protected:
		/*.....................................................................
		Routines to service the Send & Receive queues.
		.....................................................................*/
		virtual int Service_Send_Queue(void) = 0;
		virtual int Service_Receive_Queue(void) = 0;

		/*.....................................................................
		This routine actually performs a hardware-dependent data send.  It's
		pure virtual, so it >must< be defined by a derived class.
		.....................................................................*/
		virtual int Send(char *buf, int buflen) = 0;

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
		Names of all packet commands
		.....................................................................*/
		static char *ConnectionClass::Commands[PACKET_COUNT];
};

#endif

