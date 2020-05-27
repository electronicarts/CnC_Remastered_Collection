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

/* $Header:   F:\projects\c&c\vcs\code\seqconn.h_v   1.12   16 Oct 1995 16:47:58   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : SEQCONN.H                                *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : April 9, 1995   [BR]                 		*
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 * This class provides a "Sequenced" ACK/Retry approach to packet				*
 * transmission.  It waits until the last packet has been ACK'd before		*
 * sending another packet.  Thus, it guarantees order of delivery of			*
 * packets, but its performance will be slower than the Non-Sequenced		*
 * approach.																					*
 *																									*
 *	A derived class must provide:															*
 * - Init: Initialization of any hardware-specific values.						*
 * - Send: a hardware-dependent send routine.										*
 *																									*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef SEQCONN_H
#define SEQCONN_H

#include "connect.h"
#include "comqueue.h"


/*
***************************** Class Declaration *****************************
*/
class SequencedConnClass : public ConnectionClass
{
	/*
	---------------------------- Public Interface ----------------------------
	*/
	public:
		/*.....................................................................
		Constructor/destructor.
		.....................................................................*/
		SequencedConnClass (int numsend, int numrecieve, int maxlen, 
			unsigned short magicnum, unsigned long retry_delta,
			unsigned long max_retries, unsigned long timeout);
		virtual ~SequencedConnClass ();

		/*.....................................................................
		Initialization.
		.....................................................................*/
		virtual void Init (void);

		/*.....................................................................
		Send/Receive routines.
		.....................................................................*/
		virtual int Send_Packet (void * buf, int buflen, int ack_req);
		virtual int Receive_Packet (void * buf, int buflen);
		virtual int Get_Packet (void * buf, int *buflen);

		/*.....................................................................
		The packet queue.
		.....................................................................*/
		CommQueueClass *Queue;

	/*
	-------------------------- Protected Interface ---------------------------
	*/
	protected:
		/*.....................................................................
		Routines to service the Send & Receive queues.
		.....................................................................*/
		virtual int Service_Send_Queue (void);
		virtual int Service_Receive_Queue (void);

		/*.....................................................................
		Running totals of # of packets we send & receive which require an ACK, 
		and those that don't.
		.....................................................................*/
		unsigned long NumRecNoAck;
		unsigned long NumRecAck;
		unsigned long NumSendNoAck;
		unsigned long NumSendAck;

};

#endif
/*************************** end of seqconn.h ******************************/
