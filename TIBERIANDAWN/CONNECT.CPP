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

/* $Header:   F:\projects\c&c\vcs\code\connect.cpv   1.9   16 Oct 1995 16:48:56   JOE_BOSTIC  $ */
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
 *   ConnectionClass::Service -- main polling routine; services packets		*
 *   ConnectionClass::Time -- gets current time										*
 *   ConnectionClass::Command_Name -- returns name for a packet command		* 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

#ifdef WWLIB32_H
#include "TIMER.H"
#else
#include <sys\timeb.h>
#endif

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
 * If either max_retries or timeout is -1, that parameter is ignored in		*
 * timeout computations.  If both are -1, the connection will just keep		*
 * retrying forever.																			*
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
ConnectionClass::ConnectionClass (int maxlen, unsigned short magicnum, 
	unsigned long retry_delta, unsigned long max_retries, unsigned long timeout)
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

}	/* end of ~ConnectionClass */


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
	Service the Send Queue.  This [re]sends packets in the Send Queue which
	haven't been ACK'd yet, and if their retry timeout has expired, and 
	updates the FirstTime, LastTime & SendCount values in the Queue entry.
	Entries that have been ACK'd should be removed.
	------------------------------------------------------------------------*/
//	if (!Service_Send_Queue())
//		return(0);

	/*------------------------------------------------------------------------
	Service the Receive Queue.  This sends ACKs for packets that haven't
	been ACK'd yet.  Entries that the app has read, and have been ACK'd,
	should be removed.
	------------------------------------------------------------------------*/
//	if (!Service_Receive_Queue())
//		return(0);

//	return(1);

	if ( Service_Send_Queue() && Service_Receive_Queue() ) {
		return(1);
	} else {
		return(0);
	}

}	/* end of Service */

// ST = 12/17/2018 5:44PM
#ifndef TickCount
extern TimerClass					TickCount;
#endif

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
#ifdef WWLIB32_H
	return(TickCount.Time());				// Westwood Library time
#else
	static struct timeb mytime;			// DOS time
	unsigned long msec;

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
	} else {
		return(NULL);
	}
}

