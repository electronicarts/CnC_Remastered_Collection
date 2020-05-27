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
 *                 Project Name : Dynamic Data Encapsulation               *
 *                                                                         *
 *                    File Name : DDE.CPP		                             *
 *                                                                         *
 *                   Programmer : Steve Wetherill                          *
 *                                                                         *
 *                   Start Date : June 1, 1996                             *
 *                                                                         *
 *                  Last Update : June 8, 1996 [SW]                        *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Instance_Class::InstanceClass -- class constructor                    *
 *   Instance_Class::InstanceClass -- class destructor                     *
 *   Instance_Class::Enable_Callback -- enables local processing of pokes  *
 *   Instance_Class::Register_Servers -- registers a local DDE DNS service *
 *   Instance_Class::Cleanup_App -- currently does nothing                 *
 *   Instance_Class::Test_Server_Running -- does a trial connect to remote *
 *	  Instance_Class::Open_Poke_Connection -- pokes some data to server	  *
 *   Instance_Class::Close_Poke_Connectionp -- closes connection to remote *
 *   Instance_Class::Poke_Server -- sends a chunk of data to remote        *
 *   Instance_Class::dde_callback -- processes DDE transactions            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef WIN32
#include	<windows.h>
#include	"dde.h"

/***************************************************************************
 * These are static members of Instance_Class
 *=========================================================================*/

DWORD	Instance_Class::id_inst;			// instance identifier set by DdeInitialize
BOOL		Instance_Class::process_pokes;	// controls response to pokes
char 	Instance_Class::ascii_name[32];	// name of server

#if (0)	//ST - 5/8/2019
static BOOL CALLBACK	(*Instance_Class::callback) (
			LPBYTE pointer,		// pointer to received data
			long	length			// length of received data or advisory flag
			) = NULL;
#endif

/***************************************************************************
 * Instance_Class::InstanceClass -- class constructor              			*
 *                                                                         *
 * INPUT:                                                                  *
 *		name1		null terminated ASCII client name									*
 *		name1		null terminated ASCII server name									*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		dde_error = TRUE if error occurs when initializing DDE					*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   6/1/1996 SW : Created.                                                *
 *=========================================================================*/

Instance_Class::Instance_Class( LPSTR name1, LPSTR name2 )
{
	name1; name2;
	return;

#if (0)	//ST - 5/8/2019
	dde_error = FALSE;		// no errors
	process_pokes = FALSE;	// disable pokes in callback

	id_inst = 0;				// set to 0 for first time through
	conv_handle = 0;   		// conversation handle reset

	lstrcpy( ascii_name, name1 );	// keep a record of ASCII name

	if	( DdeInitialize(
		(LPDWORD) &id_inst,			// instance identifier
		dde_callback,
		APPCLASS_STANDARD | 			// filter server messages
		CBF_FAIL_SELFCONNECTIONS,	// prevent from connecting with self
		0) != DMLERR_NO_ERROR) {   // reserved
			dde_error = TRUE;			// flag an error
	}

	local_name = DdeCreateStringHandle(
		id_inst,         		// instance identifier
    	name1,     				// string to register
    	CP_WINANSI);    		// Windows ANSI code page

	remote_name = DdeCreateStringHandle(
		id_inst,         		// instance identifier
    	name2,     				// string to register
    	CP_WINANSI);    		// Windows ANSI code page

	poke_topic = DdeCreateStringHandle(
		id_inst,         		// instance identifier
		"POKE TOPIC", 			// System topic
		CP_WINANSI);    		// Windows ANSI code page

	poke_item = DdeCreateStringHandle(
		id_inst,         		// instance identifier
		"POKE ITEM", 			// System topic
		CP_WINANSI);    		// Windows ANSI code page

	system_topic = DdeCreateStringHandle(
		id_inst,         		// instance identifier
		SZDDESYS_TOPIC, 		// System topic
		CP_WINANSI);    		// Windows ANSI code page
#endif
}

/***************************************************************************
 * Instance_Class::~Instance_Class -- class destructor	              		*
 *                                                                         *
 * INPUT:                                                                  *
 *    none.                                                                *
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   6/1/1996 SW : Created.                                                *
 *=========================================================================*/

Instance_Class::~Instance_Class()
{
	DdeUninitialize( id_inst );
}

/***************************************************************************
 * Instance_Class::Enable_Callback -- enables user callback						*
 *                                                                         *
 * INPUT:                                                                  *
 *    TRUE = enable poke processing														*
 *    FALSE = disable poke processing													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		echos the input																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		user callback must be explicitly enabled. Disbabled by default.		*
 *                                                                         *
 * HISTORY:                                                                *
 *   6/1/1996 SW : Created.                                                *
 *=========================================================================*/

BOOL	Instance_Class::Enable_Callback( BOOL flag )		// enable or disable callback
{
	return (process_pokes = flag);

}

/***************************************************************************
 * Instance_Class::Register_Server -- registers a local DDE DNS service    *
 *                                                                         *
 * INPUT:                                                                  *
 *    BOOL CALLBACK ( *callback_fnc) ( LPBYTE, DWORD) = user poke callbacl *
 *                                                                         *
 * OUTPUT:                                                                 *
 *		TRUE == success																		*
 *		FALSE == failed																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   6/1/1996 SW : Created.                                                *
 *=========================================================================*/
#if (0)	//ST - 5/8/2019
BOOL	Instance_Class::Register_Server( BOOL CALLBACK ( *callback_fnc) (LPBYTE, long) )
{

	if (DdeNameService( id_inst, local_name,	0L, DNS_REGISTER ) != 0L) {
		callback = callback_fnc;
		return ( TRUE );
	} else {
		return ( FALSE );
	}
}
#endif

/***************************************************************************
 * Instance_Class::Test_Server_Running -- does a trial connect to remote   *
 *                                                                         *
 * INPUT:                                                                  *
 *    name = HSZ string handle of server name.										*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		TRUE == successfully connected to remote										*
 *		FALSE == failed to connect															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		- Can be called for local or remote server but of course will 			*
 *      fail if a called for local and local server is not "up".				*
 *		- Disconects before exiting.														*
 *                                                                         *
 * HISTORY:                                                                *
 *   6/1/1996 SW : Created.                                                *
 *=========================================================================*/

BOOL	Instance_Class::Test_Server_Running( HSZ name )
{

	if( Open_Poke_Connection( name ) == TRUE) {
		Close_Poke_Connection();
		return( TRUE );
	} else {
		return( FALSE );
	}
}

/***************************************************************************
 *	Instance_Class::Open_Poke_Connection -- open a connection to server		*
 *                                                                         *
 * INPUT:                                                                  *
 *    name = HSZ server name.																*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		TRUE == successfully opened connection											*
 *		FALSE == failed to connect															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Can be called for local or remote server but of course will 			*
 *    fail if a called for local and local server is not "up".					*
 *                                                                         *
 * HISTORY:                                                                *
 *   6/1/1996 SW : Created.                                                *
 *=========================================================================*/

BOOL	Instance_Class::Open_Poke_Connection( HSZ name )
{
	conv_handle = DdeConnect(
		id_inst,           	// instance identifier
		name,         			// service name string handle
		poke_topic,        	// topic string handle
		(PCONVCONTEXT) NULL);// use default context

	if (conv_handle == NULL) {
		return FALSE;
	} else {
		return TRUE;
	}
}

/***************************************************************************
 *	Instance_Class::Close_Poke_Connection -- closes poke connection			*
 *                                                                         *
 * INPUT:                                                                  *
 *    none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		TRUE == successfully closed connection											*
 *		FALSE == failed to close connection for some reason						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   6/1/1996 SW : Created.                                                *
 *=========================================================================*/

BOOL	Instance_Class::Close_Poke_Connection( void )
{
	if( conv_handle ) {
		HCONV	temp_handle = conv_handle;
		conv_handle = NULL;
		return( DdeDisconnect( temp_handle ));
	} else {
		return( TRUE );
	}
}

/***************************************************************************
 *	Instance_Class::Poke_Server -- pokes some data to server						*
 *                                                                         *
 * INPUT:                                                                  *
 *    poke_data	points to data to send to remote									*
 *    poke_length	length of buffer to send											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		TRUE == successfully poked the data												*
 *		FALSE == failed to connect															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		has a 3 second timeout (change POKE_TIMEOUT, in milliseconds)			*
 *                                                                         *
 * HISTORY:                                                                *
 *   6/1/1996 SW : Created.                                                *
 *=========================================================================*/

#define	POKE_TIMEOUT	60*1000	      // 60 sec timeout

BOOL	Instance_Class::Poke_Server( LPBYTE poke_data, DWORD poke_length )
{

	if( DdeClientTransaction(

		poke_data,					// address of data to pass to server
	  	poke_length,				// length of data
		conv_handle,				// handle of conversation
		poke_topic,					// handle of item name string
		CF_TEXT,						// no special clipboard data format
		XTYP_POKE,					// transaction type
		POKE_TIMEOUT,							// time-out duration (millisecs)
		(LPDWORD) NULL		 		// address of transaction result (don't check)
	   ) == 0) {

		return( FALSE);
	} else {
		return( TRUE );
	}
}

/***************************************************************************
 *	Instance_Class::dde_callback -- callback dde event handler					*
 *                                                                         *
 * INPUT:                                                                  *
 *		dde_event	transaction type														*
 *		uFmt			clipboard data format												*
 *		hconv			handle of the conversation											*
 *		hsz1			handle of a string													*
 *		hsz2			handle of a string													*
 *		hdata			handle of a global memory object									*
 *		dwData1		transaction-specific data											*
 *		dwData2 		transaction-specific data											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		context specific HDDEDATA object													*
 *                                                                         *
 * WARNINGS:                                                               *
 *	  	NOTE: declared as HDDEDATA CALLBACK which means PASCAL parameters		*
 *                                                                         *
 * HISTORY:                                                                *
 *   6/1/1996 SW : Created.                                                *
 *=========================================================================*/

HDDEDATA CALLBACK Instance_Class::dde_callback(

	UINT  dde_event,	// transaction type
	UINT  uFmt,			// clipboard data format
	HCONV  ,				// handle of the conversation
	HSZ  hsz1,			// handle of a string
	HSZ  hsz2,			// handle of a string
	HDDEDATA  hdata,	// handle of a global memory object
	DWORD  			,	// transaction-specific data
	DWORD  			 	// transaction-specific data
	)
{
	dde_event;
	uFmt;
	hsz1;
	hsz2;
	hdata;
	return (HDDEDATA)NULL;

#if (0) // ST 5/8/2019
	if (!Instance_Class::callback){
		return (HDDEDATA) NULL;
	}

	switch ( dde_event ) {

		case XTYP_REGISTER:
      case XTYP_UNREGISTER:

			return (HDDEDATA) NULL;

		case XTYP_ADVDATA:
			return (HDDEDATA) DDE_FACK;

		case XTYP_XACT_COMPLETE:

			return (HDDEDATA) NULL;

		case XTYP_DISCONNECT:

			Instance_Class::callback( NULL, DDE_ADVISE_DISCONNECT);
			return (HDDEDATA) NULL;

		case	XTYP_CONNECT: {

			char	buffer[32];

			DdeQueryString (Instance_Class::id_inst, hsz2, buffer, sizeof (buffer), 0) ;

			if (0 != strcmp (buffer, Instance_Class::ascii_name)) {
				return (HDDEDATA) NULL;
			}

			DdeQueryString (Instance_Class::id_inst, hsz1, buffer, sizeof (buffer), 0) ;

			if (0 != strcmp (buffer, "POKE TOPIC")) {
				return (HDDEDATA) NULL;
			}

			Instance_Class::callback( NULL, DDE_ADVISE_CONNECT);
			return (HDDEDATA) TRUE;
		}

		case	XTYP_POKE:

			if	(Instance_Class::process_pokes == FALSE ) {
				return (HDDEDATA) DDE_FNOTPROCESSED;	// processing disabled
			} else {

				char	buffer[32];

				DdeQueryString (Instance_Class::id_inst, hsz1, buffer, sizeof (buffer), 0) ;

				if (0 != strcmp (buffer, "POKE TOPIC")) {
					return (HDDEDATA) DDE_FNOTPROCESSED;
				} else if (uFmt == CF_TEXT) {					// make sure it's CF_TEXT

					BOOL processed;
					BYTE FAR *pdata;
					DWORD dw_length;

					if ( (pdata = DdeAccessData( hdata, &dw_length)) == NULL ) {
						return (HDDEDATA) DDE_FNOTPROCESSED;
					}

					processed = Instance_Class::callback((LPBYTE) pdata, dw_length);

					DdeUnaccessData( hdata );

					if (processed == TRUE) {
						return (HDDEDATA) DDE_FACK;
					} else {
						return (HDDEDATA) NULL;
					}

				}
			}

		default:
      	return (HDDEDATA) NULL;
    }
#endif
}

#endif	//WIN32
