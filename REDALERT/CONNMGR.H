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

/* $Header: /CounterStrike/CONNMGR.H 1     3/03/97 10:24a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : CONNMGR.H                                *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : April 3, 1995   [BR]                 		*
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 * This is the Connection Manager base class.  This is an abstract base		*
 * class that's just a shell for more functional derived classes.				*
 * The main job of the Connection Manager classes is to parse a "pool" of	*
 * incoming packets, which may be from different computers, and distribute	*
 * those packets to Connection Classes via their Receive_Packet function.	*
 *                                                                         *
 * This class should be the only access to the network/modem for the			*
 * application, so if the app needs any functions to access the 				*
 * connections or the queue's, the derived versions of this class should	*
 * provide them.																				*
 *                                                                         *
 * It's up to the derived class to define:											*
 * - Service:		polling routine; should Service each connection				*
 * - Init:			initialization; should perform hardware-dependent			*
 *						initialization, then Init each connection; this function	*
 *   					isn't defined in this class, since the parameters will 	*
 *						be highly protocol-dependent)										*
 * - Send_Message:sends a packet across the connection (this function 		*
 * 					isn't defined in this class, since the parameters will 	*
 *						be highly protocol-dependent)										*
 * - Get_Message:	gets a message from the connection (this function 			*
 * 					isn't defined in this class, since the parameters will 	*
 *						be highly protocol-dependent)										*
 *                                                                         *
 * If the derived class supports multiple connections, it should provide	*
 * functions for creating the connections, associating them with a name		*
 * or ID or both, destroying them, and sending data through all or any		*
 * connection.																					*
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CONNMGR_H
#define CONNMGR_H


/*
***************************** Class Declaration *****************************
*/
class ConnManClass
{
	/*
	---------------------------- Public Interface ----------------------------
	*/
	public:
		/*.....................................................................
		Various useful enums:
		.....................................................................*/
		enum IPXConnTag {
			CONNECTION_NONE = -1			// value of an invalid connection ID
		};

		/*.....................................................................
		Constructor/Destructor.  These currently do nothing.
		.....................................................................*/
		ConnManClass (void) {};
		virtual ~ConnManClass () {};

		/*.....................................................................
		The Service routine:
		- Parses incoming packets, and adds them to the Receive Queue for the
		  Connection Class(s) for this protocol
		- Invokes each connection's Service routine; returns an error if the
		  connection's Service routine indicates an error.
		.....................................................................*/
		virtual int Service (void) = 0;

		/*.....................................................................
		Sending & receiving data
		.....................................................................*/
		virtual int Send_Private_Message (void *buf, int buflen,
			int ack_req = 1, int conn_id = CONNECTION_NONE) = 0;
		virtual int Get_Private_Message (void *buf, int *buflen,
			int *conn_id) = 0;

		/*.....................................................................
		Connection management
		.....................................................................*/
		virtual int Num_Connections(void) = 0;
		virtual int Connection_ID(int index) = 0;
		virtual int Connection_Index(int id) = 0;

		/*.....................................................................
		Queue utility routines
		.....................................................................*/
		virtual int Global_Num_Send(void) = 0;
		virtual int Global_Num_Receive(void) = 0;
		virtual int Private_Num_Send(int id = CONNECTION_NONE) = 0;
		virtual int Private_Num_Receive(int id = CONNECTION_NONE) = 0;

		/*.....................................................................
		Timing management
		.....................................................................*/
		virtual void Reset_Response_Time(void) = 0;
		virtual unsigned long Response_Time(void) = 0;
		virtual void Set_Timing (unsigned long retrydelta,
			unsigned long maxretries, unsigned long timeout) = 0;

		/*.....................................................................
		Debugging
		.....................................................................*/
		virtual void Configure_Debug(int index, int type_offset, int type_size,
			char **names, int namestart, int namecount) = 0;
#ifdef CHEAT_KEYS
		virtual void Mono_Debug_Print(int index, int refresh) = 0;
#endif

	/*
	--------------------------- Private Interface ----------------------------
	*/
	private:
		/*.....................................................................
		This abstract class contains no data members; but a derived class
		will contain:
		- An instance of one or more derived Connection Classes
		- A buffer to store incoming packets
		.....................................................................*/
};

#endif
