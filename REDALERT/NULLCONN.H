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

/* $Header: /CounterStrike/NULLCONN.H 1     3/03/97 10:25a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : NULLCONN.H                               *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : April 3, 1995   [BR]                 		*
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 * This is the Connection Class for a NULL-Modem connection.  It inherits	*
 * a Queue, PacketBuf, timeout variables from ConnectionClass.  It 			*
 * inherits its Send_/Receive_/Get_Packet functions, and the non-sequenced	*
 * ACK/Retry logic in Service_Send_Queue & Service_Receive_Queue from		*
 * ConnectionClass.																			*
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef NULLCONN_H
#define NULLCONN_H


/*
********************************* Includes **********************************
*/
#include "connect.h"
//#include "commlib.h"

/*
********************************** Defines **********************************
*/
#define	PACKET_SERIAL_START				0xDABD
#define	PACKET_SERIAL_VERIFY				0xDEAF

#define	PACKET_SERIAL_OVERHEAD_SIZE	(sizeof( SerialHeaderType ) + sizeof( SerialCRCType ))

typedef struct {
	unsigned short MagicNumber;
	unsigned short Length;
	unsigned short MagicNumber2;
} SerialHeaderType;

typedef struct {
	int SerialCRC;
} SerialCRCType;


/*
***************************** Class Declaration *****************************
*/
class NullModemConnClass : public ConnectionClass
{
	/*
	---------------------------- Public Interface ----------------------------
	*/
	public:
		/*.....................................................................
		Constructor/destructor.
		.....................................................................*/
		NullModemConnClass (int numsend, int numrecieve, int maxlen,
			unsigned short magicnum);
		virtual ~NullModemConnClass ();

		/*.....................................................................
		Initialization.
		.....................................................................*/
#ifdef WIN32
		void Init (HANDLE port_handle);
#else	//WIN32
		void Init (PORT *port);
#endif	//WIN32

		/*.....................................................................
		Utility routines.
		.....................................................................*/
		unsigned long Actual_Max_Packet (void) { return (MaxPacketLen +
			(sizeof(SerialHeaderType)) + sizeof(int) + sizeof (char)); }

		/*.....................................................................
		This routine computes a CRC value for the given buffer.
		.....................................................................*/
		static int Compute_CRC(char *buf, int buflen);

		/*.....................................................................
		This routine returns the number of bytes extra added the packet
		for communication.
		.....................................................................*/
		static int Packet_Overhead_Size( void );

	/*
	--------------------------- Private Interface ----------------------------
	*/
	protected:
		/*.....................................................................
		This routine actually performs a hardware-dependent data send.
		.....................................................................*/
		virtual int Send (char *buf, int buflen, void *extrabuf, int extralen);
#ifdef WIN32
		/*
		** This is the winsoze port handle
		*/
		HANDLE PortHandle;
#else	//WIN32
		/*.....................................................................
		This is the PORT value used by the GreenLeaf calls.
		.....................................................................*/
		PORT *Port;
#endif	//WIN32

		/*.....................................................................
		This buffer is a staging area for data sent out; it includes the
		packet sent by the parent class (which includes the application's
		packet, plus the CommHeaderType header), plus:
		- 2-byte buffer start ID
		- 2-byte length
		- 4-byte CRC value (at the end of the buffer)
		This is the actual packet that gets sent across the serial line.
		.....................................................................*/
		char *SendBuf;
};

#endif

/************************** end of nullconn.h ******************************/

