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

/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Dynamic Data Encapsulation					*
 *                                                                         *
 *                    File Name : DDE.H												*
 *                                                                         *
 *                   Programmer : Steve Wetherill									*
 *                                                                         *
 *                   Start Date : June 1, 1996		                        *
 *                                                                         *
 *                  Last Update : June 8, 1996   [SW]                 		*
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 * This is the DDE (Instance_Class) which provides a simple CLIENT/SERVER	*
 * DDE model for data transactions between Windows applications.				*
 * This is a fairly naieve implementation allowing only one client/server	*
 * per Instance_Class object. 															*
 *																									*
 * Typical uses for this class are:														*
 *																									*
 * i. Robust verification of whether an application is running					*
 * ii. Data transfer between applications												*
 *																									*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*
***************************** Class defines *****************************
*/

#ifndef 	__DDE_H
#define	__DDE_H

#define	DDE_ADVISE_CONNECT		-1		// advisory "client has connected"
#define	DDE_ADVISE_DISCONNECT	-2		// advisory "client has disconnected"

/*
***************************** Class Declaration *****************************
*/

class	Instance_Class {

	/*
	---------------------------- Public Interface ----------------------------
	*/
	public:

		/*.....................................................................
		Constructor:
		- takes null terminated ASCII strings names for client and server
		.....................................................................*/

		Instance_Class(		// constructor
			LPSTR,				// null terminated local sever name string
			LPSTR					// null terminated remote server name string
			);

		/*.....................................................................
		Destructor:
		.....................................................................*/
		~Instance_Class(void);	// the destructor

		/*.....................................................................
		Send data routine:
		- sends an unsolicited packet of data to the remote server
		.....................................................................*/
		BOOL	Poke_Server( LPBYTE, DWORD);

		/*.....................................................................
		Send data routine:
		- sets up DNS for the server and registers a user callback to handle
		  incoming data
		.....................................................................*/
		BOOL	Register_Server( BOOL (CALLBACK *)(LPBYTE, long));

		/*.....................................................................
		Does a trial connect to the remote server.
		- used to determine whether server is alive or not (and thus running)
		.....................................................................*/
		BOOL	Test_Server_Running( HSZ );

		/*.....................................................................
		Enables user callback (disabled by default)
		.....................................................................*/
		BOOL	Enable_Callback( BOOL );		// enable or disable callback

		/*.....................................................................
		Open a connection for sending data to remote server
		.....................................................................*/
		BOOL	Open_Poke_Connection( HSZ );

		/*.....................................................................
		Close connection with remote server
		.....................................................................*/
		BOOL	Close_Poke_Connection( void );

		//
		// static members
		//

		/*.....................................................................
		User callback - called upon receipt of incoming data (static member!)
		.....................................................................*/
		static BOOL (CALLBACK *callback) (

			LPBYTE pointer,		// pointer to received data
			long length				// if >0 length of received data
										// if <0
										//	-1 == client connect detected
										// -2 == client disconnect detected
			);

		/*.....................................................................
		DDE callback, called when DDEML has an event for us
		.....................................................................*/
		static HDDEDATA CALLBACK dde_callback(

			UINT  uType,		// transaction type
			UINT  uFmt,			// clipboard data format
			HCONV  hconv,		// handle of the conversation
			HSZ  hsz1,			// handle of a string
			HSZ  hsz2,			// handle of a string
			HDDEDATA  hdata,	// handle of a global memory object
			DWORD  dwData1,	// transaction-specific data
			DWORD  dwData2 	// transaction-specific data
			);
		HANDLE	instance;	// this application's instance
		HWND		hwnd;			// valid window handle

		/*.....................................................................
		member variables
		.....................................................................*/

		static DWORD id_inst;			// instance identifier set by DdeInitialize
		static BOOL	process_pokes;		// controls response to pokes
		static char ascii_name[32];	// name of server

		//
		// non-static member variables
		//

		HSZ remote_name; 		// string handle for remote server name
		HSZ local_name; 		// string handle for local server name
		HSZ system_topic;		// string handle for the "system" topic
		HSZ poke_topic;		// string handle for poking data to server topic
		HSZ poke_item;			// string handle for poking data to server item

		HCONV conv_handle;   // conversation handle
		BOOL	dde_error;		// error flag

};

#endif

