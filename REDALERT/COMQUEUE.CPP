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

/* $Header:   F:\projects\c&c0\vcs\code\comqueue.cpv   4.1   11 Apr 1996 18:28:16   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : COMQUEUE.CPP                             *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : May 31, 1995 [BRR]                       *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   CommQueueClass::Add_Delay -- adds a new delay value for response time *
 *   CommQueueClass::Avg_Response_Time -- returns average response time    *
 *   CommQueueClass::CommQueueClass -- class constructor                   *
 *   CommQueueClass::Configure_Debug -- sets up special debug values       * 
 *   CommQueueClass::Get_Receive -- gets ptr to queue entry                *
 *   CommQueueClass::Get_Send -- gets ptr to queue entry                   *
 *   CommQueueClass::Init -- initializes this queue                        *
 *   CommQueueClass::Max_Response_Time -- returns max response time        *
 *   CommQueueClass::Mono_Debug_Print -- Debug output routine              * 
 *   CommQueueClass::Mono_Debug_Print2 -- Debug output; alternate format   * 
 *   CommQueueClass::Next_Receive -- gets ptr to next entry in send queue  *
 *   CommQueueClass::Next_Send -- gets ptr to next entry in send queue     *
 *   CommQueueClass::Queue_Receive -- queues a received message            *
 *   CommQueueClass::Queue_Send -- queues a message for sending            *
 *   CommQueueClass::Reset_Response_Time -- resets computations            *
 *   CommQueueClass::UnQueue_Receive -- removes next entry from send queue *
 *   CommQueueClass::UnQueue_Send -- removes next entry from send queue    *
 *   CommQueueClass::~CommQueueClass -- class destructor                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if (0)
#include "function.h"


/***************************************************************************
 * CommQueueClass::CommQueueClass -- class constructor             			*
 *                                                                         *
 * INPUT:                                                                  *
 *		numsend		# queue entries for sending										*
 *		numreceive	# queue entries for receiving										*
 *		maxlen		maximum desired packet length, in bytes						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
CommQueueClass::CommQueueClass(int numsend, int numreceive, int maxlen)
{
	int i;

	/*
	**	Init variables 
	*/
	MaxSend = numsend;
	MaxReceive = numreceive;
	MaxPacketSize = maxlen;

	/*
	**	Allocate the queue entries 
	*/
	SendQueue = new SendQueueType[numsend];
	ReceiveQueue = new ReceiveQueueType[numreceive];

	/*
	**	Allocate queue entry buffers 
	*/
	for (i = 0; i < MaxSend; i++) {
		SendQueue[i].Buffer = new char[maxlen];
	}

	for (i = 0; i < MaxReceive; i++) {
		ReceiveQueue[i].Buffer = new char[maxlen];
	}

	Init();
}


/***************************************************************************
 * CommQueueClass::~CommQueueClass -- class destructor             			*
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
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
CommQueueClass::~CommQueueClass()
{
	int i;

	/*
	**	Free queue entry buffers 
	*/
	for (i = 0; i < MaxSend; i++) {
		delete [] SendQueue[i].Buffer;
	}

	for (i = 0; i < MaxReceive; i++) {
		delete [] ReceiveQueue[i].Buffer;
	}

	delete [] SendQueue;
	delete [] ReceiveQueue;
}


/***************************************************************************
 * CommQueueClass::Init -- initializes this queue                          *
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
 *   01/20/1995 BR : Created.                                              *
 *=========================================================================*/
void CommQueueClass::Init(void)
{
	int i;

	/*
	**	Init data members
	*/
	SendTotal = 0L;
	ReceiveTotal = 0L;

	DelaySum = 0L;
	NumDelay = 0L;
	MeanDelay = 0L;
	MaxDelay = 0L;

	SendCount = 0;
	SendNext = 0;
	SendEmpty = 0;

	ReceiveCount = 0;
	ReceiveNext = 0;
	ReceiveEmpty = 0;

	/*
	**	Init the queue entries
	*/
	for (i = 0; i < MaxSend; i++) {
		SendQueue[i].IsActive = 0;
		SendQueue[i].IsACK = 0;
		SendQueue[i].FirstTime = 0L;
		SendQueue[i].LastTime = 0L;
		SendQueue[i].SendCount = 0L;
		ReceiveQueue[i].BufLen = 0;
	}
	for (i = 0; i < MaxReceive; i++) {
		ReceiveQueue[i].IsActive = 0;
		ReceiveQueue[i].IsRead = 0;
		ReceiveQueue[i].IsACK = 0;
		ReceiveQueue[i].BufLen = 0;
	}
	
	/*
	**	Init debug values
	*/
	DebugOffset = 0;
	DebugSize = 0;
	DebugNames = NULL;
	DebugMaxNames = 0;
}


/***************************************************************************
 * CommQueueClass::Queue_Send -- queues a message for sending              *
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer containing the message										*
 *		buflen		length of 'buf'														*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = no room in the queue													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int CommQueueClass::Queue_Send(void *buf, int buflen)
{
	/*
	**	Error if no room in the queue
	*/
	if (SendCount==MaxSend || SendQueue[SendEmpty].IsActive!=0) {
		return(0); 
	}

	/*
	**	Set entry flags 
	*/
	SendQueue[SendEmpty].IsActive = 1;			// entry is now active
	SendQueue[SendEmpty].IsACK = 0;				// entry hasn't been ACK'd
	SendQueue[SendEmpty].FirstTime = 0L;		// filled in by Manager when sent
	SendQueue[SendEmpty].LastTime = 0L;			// filled in by Manager when sent
	SendQueue[SendEmpty].SendCount = 0L;		// filled in by Manager when sent
	SendQueue[SendEmpty].BufLen = buflen;		// save buffer size

	/*
	**	Copy the packet data 
	*/
	memcpy(SendQueue[SendEmpty].Buffer,buf,buflen);

	/*
	**	Increment counters & entry ptr 
	*/
	SendCount++;
	SendEmpty++;
	if (SendEmpty==MaxSend) {
		SendEmpty = 0;
	}

	SendTotal++;

	return(1);
}


/***************************************************************************
 * CommQueueClass::UnQueue_Send -- removes next entry from send queue		*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to store entry's data in; if NULL, it's discarded	*
 *		buflen		filled in with length of entry retrieved						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = no entry to retreive													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int CommQueueClass::UnQueue_Send(void *buf, int *buflen)
{
	/*
	**	Error if no entry to retrieve 
	*/
	if (SendCount==0 || SendQueue[SendNext].IsActive==0) {
		return(0);
	}

	/*
	**	Copy the data from the entry 
	*/
	if (buf!=NULL) {
		memcpy(buf,SendQueue[SendNext].Buffer,SendQueue[SendNext].BufLen);
		(*buflen) = SendQueue[SendNext].BufLen;
	}

	/*
	**	Set entry flags 
	*/
	SendQueue[SendNext].IsActive = 0;
	SendQueue[SendNext].IsACK = 0;
	SendQueue[SendNext].FirstTime = 0L;
	SendQueue[SendNext].LastTime = 0L;
	SendQueue[SendNext].SendCount = 0L;
	SendQueue[SendNext].BufLen = 0;
	SendCount--;
	SendNext++;
	if (SendNext==MaxSend) {
		SendNext = 0;
	}

	return(1);
}


/***************************************************************************
 * CommQueueClass::Next_Send -- gets ptr to next entry in send queue       *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ptr to entry, NULL if there is none.											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
SendQueueType * CommQueueClass::Next_Send(void)
{
	if (SendCount==0) {
		return(NULL);
	} else {
		return(&SendQueue[SendNext]);
	}
}


/***************************************************************************
 * CommQueueClass::Get_Send -- gets ptr to queue entry                     *
 *                                                                         *
 * This routine gets a pointer to the indicated queue entry.  The index		*
 * value is relative to the next-accessable queue entry; 0 = get the			*
 * next available queue entry, 1 = get the one behind that, etc.				*
 *                                                                         *
 * INPUT:                                                                  *
 *		index		index of entry to get (0 = 1st available)							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ptr to entry																			*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/21/1994 BR : Created.                                              *
 *=========================================================================*/
SendQueueType * CommQueueClass::Get_Send(int index)
{
	int i;

	i = SendNext + index;
	if (i >= MaxSend) {
		i -= MaxSend;
	}

	if (SendQueue[i].IsActive==0) {
		return(NULL);
	} else {
		return(&SendQueue[i]);
	}
}


/***************************************************************************
 * CommQueueClass::Queue_Receive -- queues a received message					*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer containing the message										*
 *		buflen		length of 'buf'														*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = no room in the queue													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int CommQueueClass::Queue_Receive(void *buf, int buflen)
{
	/*
	**	Error if no room in the queue
	*/
	if (ReceiveCount==MaxReceive || ReceiveQueue[ReceiveEmpty].IsActive!=0) {
		return(0); 
	}

	/*
	**	Set entry flags
	*/
	ReceiveQueue[ReceiveEmpty].IsActive = 1;
	ReceiveQueue[ReceiveEmpty].IsRead = 0;
	ReceiveQueue[ReceiveEmpty].IsACK = 0;
	ReceiveQueue[ReceiveEmpty].BufLen = buflen;

	/*
	**	Copy the packet data
	*/
	memcpy(ReceiveQueue[ReceiveEmpty].Buffer,buf,buflen);

	/*
	**	Increment counters & entry ptr 
	*/
	ReceiveCount++;
	ReceiveEmpty++;
	if (ReceiveEmpty==MaxReceive) {
		ReceiveEmpty = 0;
	}

	ReceiveTotal++;

	return(1);
}


/***************************************************************************
 * CommQueueClass::UnQueue_Receive -- removes next entry from send queue	*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to store entry's data in; if NULL, it's discarded	*
 *		buflen		filled in with length of entry retrieved						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = no entry to retreive													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int CommQueueClass::UnQueue_Receive(void *buf, int *buflen)
{
	/*
	**	Error if no entry to retrieve 
	*/
	if (ReceiveCount==0 || ReceiveQueue[ReceiveNext].IsActive==0) {
		return(0);
	}

	/*
	**	Copy the data from the entry
	*/
	if (buf!=NULL) {
		memcpy(buf,ReceiveQueue[ReceiveNext].Buffer,
			ReceiveQueue[ReceiveNext].BufLen);
		(*buflen) = ReceiveQueue[ReceiveNext].BufLen;
	}

	/*
	**	Set entry flags
	*/
	ReceiveQueue[ReceiveNext].IsActive = 0;
	ReceiveQueue[ReceiveNext].IsRead = 0;
	ReceiveQueue[ReceiveNext].IsACK = 0;
	ReceiveQueue[ReceiveNext].BufLen = 0;
	ReceiveCount--;
	ReceiveNext++;
	if (ReceiveNext==MaxReceive) {
		ReceiveNext = 0;
	}

	return(1);
}


/***************************************************************************
 * CommQueueClass::Next_Receive -- gets ptr to next entry in send queue    *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ptr to entry, NULL if there is none.											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
ReceiveQueueType * CommQueueClass::Next_Receive(void)
{
	if (ReceiveCount==0) {
		return(NULL);
	} else {
		return(&ReceiveQueue[ReceiveNext]);
	}
}


/***************************************************************************
 * CommQueueClass::Get_Receive -- gets ptr to queue entry                  *
 *                                                                         *
 * This routine gets a pointer to the indicated queue entry.  The index		*
 * value is relative to the next-accessable queue entry; 0 = get the			*
 * next available queue entry, 1 = get the one behind that, etc.				*
 *                                                                         *
 * INPUT:                                                                  *
 *		index		index of entry to get (0 = 1st available)							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ptr to entry																			*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/21/1994 BR : Created.                                              *
 *=========================================================================*/
ReceiveQueueType * CommQueueClass::Get_Receive(int index)
{
	int i;

	i = ReceiveNext + index;
	if (i >= MaxReceive) {
		i -= MaxReceive;
	}

	if (ReceiveQueue[i].IsActive==0) {
		return(NULL);
	} else {
		return(&ReceiveQueue[i]);
	}
}


/***************************************************************************
 * CommQueueClass::Add_Delay -- adds a new delay value for response time   *
 *                                                                         *
 * This routine updates the average response time for this queue.  The		*
 * computation is based on the average of the last 'n' delay values given,	*
 * It computes a running total of the last n delay values, then divides 	*
 * that by n to compute the average.													*
 *																									*
 * When the number of values given exceeds the max, the mean is subtracted	*
 * off the total, then the new value is added in.  Thus, any single delay	*
 * value will have an effect on the total that approaches 0 over time, and	*
 * the new delay value contributes to 1/n of the mean.							*
 *                                                                         *
 * INPUT:                                                                  *
 *		delay			value to add into the response time computation				*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/19/1995 BR : Created.                                              *
 *=========================================================================*/
void CommQueueClass::Add_Delay(unsigned long delay)
{
	int roundoff = 0;

	if (NumDelay==256) {
		DelaySum -= MeanDelay;
		DelaySum += delay;
		if ( (DelaySum & 0x00ff) > 127) {
			roundoff = 1;
		}
		MeanDelay = (DelaySum >> 8) + roundoff;
	} else {
		NumDelay++;
		DelaySum += delay;
		MeanDelay = DelaySum / NumDelay;
	}

	if (delay > MaxDelay) {
		MaxDelay = delay;
	}
}


/***************************************************************************
 * CommQueueClass::Avg_Response_Time -- returns average response time    	*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		latest computed average response time											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/19/1995 BR : Created.                                              *
 *=========================================================================*/
unsigned long CommQueueClass::Avg_Response_Time(void)
{
	return(MeanDelay);
}


/***************************************************************************
 * CommQueueClass::Max_Response_Time -- returns max response time    		*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		latest computed average response time											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/19/1995 BR : Created.                                              *
 *=========================================================================*/
unsigned long CommQueueClass::Max_Response_Time(void)
{
	return(MaxDelay);
}


/***************************************************************************
 * CommQueueClass::Reset_Response_Time -- resets computations					*
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
 *   01/19/1995 BR : Created.                                              *
 *=========================================================================*/
void CommQueueClass::Reset_Response_Time(void)
{
	DelaySum = 0L;
	NumDelay = 0L;
	MeanDelay = 0L;
	MaxDelay = 0L;
}


/*************************************************************************** 
 * CommQueueClass::Configure_Debug -- sets up special debug values         * 
 *                                                                         * 
 * Mono_Debug_Print2() can look into a packet to pull out a particular		*
 * ID, and can print both that ID and a string corresponding to				*
 * that ID.  This routine configures these values so it can find				*
 * and decode the ID.  This ID is used in addition to the normal				*
 * CommHeaderType values.																	*
 *                                                                         * 
 * INPUT:                                                                  * 
 *		offset		ID's byte offset into packet										*
 *		size			size of ID, in bytes; 0 if none									*
 *		names			ptr to array of names; use ID as an index into this		*
 *		maxnames		max # in the names array; 0 if none.							*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		none.																						*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		Names shouldn't be longer than 12 characters.								*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   05/31/1995 BRR : Created.                                             * 
 *=========================================================================*/
void CommQueueClass::Configure_Debug(int offset, int size, char **names, 
	int maxnames)
{
	DebugOffset = offset;
	DebugSize = size;
	DebugNames = names;
	DebugMaxNames = maxnames;
}


/*************************************************************************** 
 * Mono_Debug_Print -- Debug output routine                                * 
 *                                                                         * 
 * This routine leaves 5 lines at the top for the caller's use.				*
 *                                                                         * 
 * INPUT:                                                                  * 
 *		refresh		1 = clear screen & completely refresh							*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		none.																						*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   05/02/1995 BRR : Created.                                             * 
 *=========================================================================*/
void CommQueueClass::Mono_Debug_Print(int refresh)
{
#ifdef WWLIB32_H
	int i;												// loop counter
	static int send_col[] = {1,14,28};			// coords of send queue columns
	static int receive_col[] = {40,54,68};		// coords of recv queue columns
	int row,col;										// current row,col for printing
	int num;												// max # items to print

	struct CommHdr {									// this mirrors the CommHeaderType
		unsigned short MagicNumber;
		unsigned char Code;
		unsigned long PacketID;
	} *hdr;

	/*
	**	If few enough entries, call the verbose debug version
	*/
	if (MaxSend <= 16) {
		Mono_Debug_Print2(refresh);
		return;
	}

	/*
	**	Refresh the screen
	*/
	if (refresh) {
		Mono_Clear_Screen ();
		Mono_Printf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n");
		Mono_Printf("³                                                                             ³\n");
		Mono_Printf("³                                                                             ³\n");
		Mono_Printf("³                                                                             ³\n");
		Mono_Printf("ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n");
		Mono_Printf("³              Send Queue              ³             Receive Queue            ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³ ID  Ct ACK   ID  Ct ACK    ID  Ct ACK³ ID  Rd ACK    ID  Rd ACK   ID  Rd ACK³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
	}

	/*
	**	Print Send Queue items
	*/
	if (MaxSend <= 48) {
		num = MaxSend;
	} else {
		num = 48;
	}
	col = 0;
	row = 0;
	for (i = 0; i < MaxSend; i++) {
		Mono_Set_Cursor (send_col[col],row + 8);
		if (SendQueue[i].IsActive) {
			hdr = (CommHdr *)SendQueue[i].Buffer;
			hdr->MagicNumber = hdr->MagicNumber;
			hdr->Code = hdr->Code;
			Mono_Printf ("%4d %2d  %d",hdr->PacketID, SendQueue[i].SendCount,
				SendQueue[i].IsACK);
		} else {
			Mono_Printf ("____ __  _ ");
		}

		row++;
		if (row > 15) {
			row = 0;
			col++;
		}
	}

	/*
	**	Print Receive Queue items
	*/
	if (MaxReceive <= 48) {
		num = MaxSend;
	} else {
		num = 48;
	}
	col = 0;
	row = 0;
	for (i = 0; i < MaxReceive; i++) {
		Mono_Set_Cursor (receive_col[col],row + 8);
		if (ReceiveQueue[i].IsActive) {
			hdr = (CommHdr *)ReceiveQueue[i].Buffer;
			Mono_Printf ("%4d  %d  %d",hdr->PacketID, ReceiveQueue[i].IsRead,
				ReceiveQueue[i].IsACK);
		} else {
			Mono_Printf ("____  _  _ ");
		}

		row++;
		if (row > 15) {
			row = 0;
			col++;
		}
	}

#else
	refresh = refresh;
#endif	
}


/*************************************************************************** 
 * CommQueueClass::Mono_Debug_Print2 -- Debug output; alternate format     * 
 *                                                                         * 
 * This routine prints more information than the other version; it's			*
 * called only if the number of queue entries is small enough to support	*
 * this format.																				*
 *                                                                         * 
 * INPUT:                                                                  * 
 *		refresh		1 = clear screen & completely refresh							*
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *		none.																						*
 *                                                                         * 
 * WARNINGS:                                                               * 
 *		none.																						*
 *                                                                         * 
 * HISTORY:                                                                * 
 *   05/31/1995 BRR : Created.                                             * 
 *=========================================================================*/
void CommQueueClass::Mono_Debug_Print2(int refresh)
{
#ifdef WWLIB32_H
	int i;												// loop counter
	char txt[80];
	int val;

	struct CommHdr {									// this mirrors the CommHeaderType
		unsigned short MagicNumber;
		unsigned char Code;
		unsigned long PacketID;
	} *hdr;

	/*
	**	Refresh the screen
	*/
	if (refresh) {
		Mono_Clear_Screen ();
		Mono_Printf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n");
		Mono_Printf("³                                                                             ³\n");
		Mono_Printf("³                                                                             ³\n");
		Mono_Printf("³                                                                             ³\n");
		Mono_Printf("ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n");
		Mono_Printf("³              Send Queue              ³             Receive Queue            ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³ ID  Ct Type   Data  Name         ACK ³ ID  Rd Type   Data  Name         ACK ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("³                                      ³                                      ³\n");
		Mono_Printf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
	}

	/*
	**	Print Send Queue items
	*/
	for (i = 0; i < MaxSend; i++) {
		Mono_Set_Cursor (1,8 + i);
		
		/*
		**	Print an active entry
		*/
		if (SendQueue[i].IsActive) {
			
			/*
			**	Get header info
			*/
			hdr = (CommHdr *)SendQueue[i].Buffer;
			hdr->MagicNumber = hdr->MagicNumber;
			hdr->Code = hdr->Code;
			sprintf(txt,"%4d %2d %-5s  ",
				hdr->PacketID,
				SendQueue[i].SendCount,
				ConnectionClass::Command_Name(hdr->Code));

			/*
			**	Decode app's ID & its name
			*/
			if (DebugSize && (DebugOffset + DebugSize) <= SendQueue[i].BufLen) {
				if (DebugSize==1) {
					val = *(SendQueue[i].Buffer + DebugOffset);
				} else {
					if (DebugSize==2) {
						val = *((short *)(SendQueue[i].Buffer + DebugOffset));
					} else {
						if (DebugSize==4) {
							val = *((int *)(SendQueue[i].Buffer + DebugOffset));
						}
					} 
				}
				sprintf(txt + strlen(txt),"%4d  ",val);

				if (DebugMaxNames && val > 0 && val < DebugMaxNames) {
					sprintf(txt + strlen(txt),"%-12s  %x",
						DebugNames[val],
						SendQueue[i].IsACK);
				} else {
					sprintf(txt + strlen(txt),"              %x",SendQueue[i].IsACK);
				}
			}
		} else {

			/*
			**	Entry isn't active; print blanks
			*/
			Mono_Printf("____ __                            _");
		}
	}

	/*
	**	Print Receive Queue items
	*/
	for (i = 0; i < MaxReceive; i++) {
		Mono_Set_Cursor (40,8 + i);
		
		/*
		**	Print an active entry
		*/
		if (ReceiveQueue[i].IsActive) {
			
			/*
			**	Get header info
			*/
			hdr = (CommHdr *)ReceiveQueue[i].Buffer;
			hdr->MagicNumber = hdr->MagicNumber;
			hdr->Code = hdr->Code;
			sprintf(txt,"%4d %2d %-5s  ",
				hdr->PacketID,
				ReceiveQueue[i].IsRead,
				ConnectionClass::Command_Name(hdr->Code));
			
			/*
			**	Decode app's ID & its name
			*/
			if (DebugSize && (DebugOffset + DebugSize) <= SendQueue[i].BufLen) {
				if (DebugSize==1) {
					val = *(ReceiveQueue[i].Buffer + DebugOffset);
				} else {
					if (DebugSize==2) {
						val = *((short *)(ReceiveQueue[i].Buffer + DebugOffset));
					} else {
						if (DebugSize==4) {
							val = *((int *)(ReceiveQueue[i].Buffer + DebugOffset));
						}
					}
				} 
				sprintf(txt + strlen(txt),"%4d  ",val);

				if (DebugMaxNames && val > 0 && val < DebugMaxNames) {
					sprintf(txt + strlen(txt),"%-12s  %x", DebugNames[val], ReceiveQueue[i].IsACK);
				} else {
					sprintf(txt + strlen(txt),"              %x",ReceiveQueue[i].IsACK);
				}
			}
		} else {

			/*
			**	Entry isn't active; print blanks
			*/
			Mono_Printf("____ __                            _");
		}
	}

#else
	refresh = refresh;
#endif	
}


#endif