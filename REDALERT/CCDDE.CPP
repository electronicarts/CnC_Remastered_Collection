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


/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer - Red Alert                                *
 *                                                                                             *
 *                    File Name : CCDDE.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 10/04/95                                                     *
 *                                                                                             *
 *                  Last Update : August 5th, 1996 [ST]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *   C&C's interface to the DDE class                                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 * DDE_Callback -- DDE server callback function                                                *
 * DDEServerClass::DDEServerClass -- class constructor                                         *
 * DDEServerClass::Enable -- Enables the DDE callback                                          *
 * DDEServerClass::Disable -- Disables the DDE callback                                        *
 * DDEServerClass::~DDEServerClass -- class destructor                                         *
 * DDESC::Callback -- callback function. Called from the DDE_Callback wrapper function         *
 * DDESC::Get_MPlayer_Game_Info -- returns a pointer to the multiplayer setup info from wchat  *
 * DDESC::Delete_MPlayer_Game_Info -- clears out multi player game setup info                  *
 * DDESC::Time_Since_Heartbeat -- returns the time in ticks since the last heartbeat from wchat*
 *                                                                                             *
 * Send_Data_To_DDE_Server -- sends a packet to WChat                                          *
 *                                                                                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef WIN32

#include <WINDOWS.H>
#include "ccdde.h"
#include <stdio.h>
#include <timer.h>

DDEServerClass	DDEServer;				//Instance of the DDE Server class

Instance_Class	*DDE_Class = NULL;	// pointer for client callback
												// this *must* be called DDE_Class

BOOL RA95AlreadyRunning = FALSE;		//Was there an instance of Red Alert 95 already running when we started?

/*
** Misc externs so we dont have to include FUNCTION.H
*/
extern HWND 			MainWindow;
extern TimerClass	GameTimer;
extern bool			GameTimerInUse;
extern void 			WWDebugString (char *string);


/***********************************************************************************************
 * DDE_Callback -- DDE server callback function                                                *
 *                                                                                             *
 *   Just acts as a wrapper for the DDEServerClass callback function                           *
 *                                                                                             *
 * INPUT:    ptr to data from client                                                           *
 *           length of data                                                                    *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/8/96 3:19PM ST : Created                                                               *
 *=============================================================================================*/
BOOL CALLBACK DDE_Callback (unsigned char *data, long length)
{
	return (DDEServer.Callback(data, length));
}




/***********************************************************************************************
 * DDEServerClass::DDEServerClass -- class constructor                                         *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/8/96 3:20PM ST : Created                                                               *
 *=============================================================================================*/
DDEServerClass::DDEServerClass(void)
{
	MPlayerGameInfo = NULL;		//Flag that we havnt received a start game info packet yet

	//DDE_Class = new Instance_Class ("CONQUER", "WCHAT");
	DDE_Class = new Instance_Class ("REDALERT", "WCHAT");

	DDE_Class->Enable_Callback( TRUE );
	IsEnabled = TRUE;

	if (DDE_Class->Test_Server_Running(DDE_Class->local_name)){
		RA95AlreadyRunning = TRUE;
	}else{
		//DDE_Class->Register_Server( DDE_Callback );				// ST - 5/8/2019
	}
}



/***********************************************************************************************
 * DDEServerClass::Enable -- Enables the DDE callback                                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/5/96 9:44PM ST : Created                                                               *
 *=============================================================================================*/
void DDEServerClass::Enable(void)
{
	if (!IsEnabled){
		DDE_Class->Enable_Callback( TRUE );
		IsEnabled = TRUE;
	}
}



/***********************************************************************************************
 * DDEServerClass::Disable -- Disables the DDE callback                                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/5/96 9:44PM ST : Created                                                               *
 *=============================================================================================*/
void DDEServerClass::Disable(void)
{
	if (IsEnabled){
		DDE_Class->Enable_Callback( FALSE );
		IsEnabled = FALSE;
	}
}






/***********************************************************************************************
 * DDEServerClass::~DDEServerClass -- class destructor                                         *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/8/96 3:20PM ST : Created                                                               *
 *=============================================================================================*/
DDEServerClass::~DDEServerClass(void)
{
	Delete_MPlayer_Game_Info();
	delete( DDE_Class );
}



/***********************************************************************************************
 * DDESC::Callback -- callback function. Called from the DDE_Callback wrapper function         *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    data from DDE client                                                              *
 *           length of data                                                                    *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: Data has length and type as first 2 ints                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/8/96 3:21PM ST : Created                                                               *
 *=============================================================================================*/
BOOL DDEServerClass::Callback(unsigned char *data, long length)
{

	/*
	** If the packet length < 0 then this is a special advisory packet
	*/
	if ( length<0 ) {

		switch( length ) {

			case	DDE_ADVISE_CONNECT:
				WWDebugString("RA95 - DDE advisory: client connect detected.");
				return TRUE;

			case	DDE_ADVISE_DISCONNECT:
				WWDebugString("RA95 - DDE advisory: client disconnect detected.");
				return TRUE;

			default:
				WWDebugString("RA95 - DDE advisory: Unknown DDE advise type.");
				return FALSE;
		}

	}else{

		/*
		** Packet must be at least the length of the packet type & size fields to be valid
		*/
		if (length < 2*sizeof(int)) {
			WWDebugString ("RA95 - Received invalid packet.");
			return (FALSE);
		}

		/*
		** Find out what kind of packet this is and its length.
		*/
		int *packet_pointer = (int *)data;
		int  actual_length = ntohl(*packet_pointer++);
		int  packet_type =  ntohl(*packet_pointer++);

		/*
		** Strip the ID int from the start of the packet
		*/
		data   += 2*sizeof (int);
		length -= 2*sizeof (int);
		actual_length -= 2*sizeof (int);

		/*
		** Take the appropriate action for the packet type
		*/
		switch ( packet_type ){

			/*
			** This is a packet with the info required for starting a new internet game. This is really
		 	* just C&CSPAWN.INI sent from WChat instead of read from disk.
			*/
			case DDE_PACKET_START_MPLAYER_GAME:
				WWDebugString("RA95 - Received start game packet.");
				Delete_MPlayer_Game_Info();
				MPlayerGameInfo = new char [actual_length + 1];
				memcpy (MPlayerGameInfo, data, actual_length);
				*(MPlayerGameInfo + actual_length) = 0;		//Terminator in case we treat it as a string
				MPlayerGameInfoLength = actual_length;
				LastHeartbeat = 0;
				break;

			case DDE_TICKLE:
				WWDebugString("RA95 - Received 'tickle' packet.");
				//SetForegroundWindow ( MainWindow );
				//ShowWindow ( MainWindow, SW_SHOWMAXIMIZED );
				break;

			case DDE_PACKET_HEART_BEAT:
				WWDebugString("RA95 - Received heart beat packet.");
				if (GameTimerInUse){
					LastHeartbeat = GameTimer.Time();
				}else{
					LastHeartbeat = 0;
				}
				break;

			default:
				WWDebugString("RA95 - Received unrecognised packet.");
				break;

		}
	}

	return (TRUE);

}



/***********************************************************************************************
 * DDESC::Get_MPlayer_Game_Info -- returns a pointer to the multiplayer setup info from wchat  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   ptr to data in .INI file format                                                   *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/8/96 3:23PM ST : Created                                                               *
 *=============================================================================================*/
char *DDEServerClass::Get_MPlayer_Game_Info (void)
{
	return (MPlayerGameInfo);
}



/***********************************************************************************************
 * DDESC::Delete_MPlayer_Game_Info -- clears out multi player game setup info                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/8/96 3:24PM ST : Created                                                               *
 *=============================================================================================*/
void DDEServerClass::Delete_MPlayer_Game_Info(void)
{
	if (MPlayerGameInfo){
		delete [] MPlayerGameInfo;
		MPlayerGameInfo = NULL;
	}
}



/***********************************************************************************************
 * DDESC::Time_Since_Heartbeat -- returns the time in ticks since the last heartbeat from wchat*
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   time since heartbeat                                                              *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/9/96 11:05PM ST : Created                                                              *
 *=============================================================================================*/
int DDEServerClass::Time_Since_Heartbeat(void)
{
	return (GameTimer.Time() - LastHeartbeat);
}




/***********************************************************************************************
 * Send_Data_To_DDE_Server -- sends a packet to WChat                                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to the data to send                                                           *
 *           length of data                                                                    *
 *           packet type identifier                                                            *
 *                                                                                             *
 * OUTPUT:   true if packet successfully sent                                                  *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/9/96 11:07PM ST : Created                                                              *
 *=============================================================================================*/
BOOL Send_Data_To_DDE_Server (char *data, int length, int packet_type)
{
	if( DDE_Class->Open_Poke_Connection(DDE_Class->remote_name) == FALSE) {
		WWDebugString("RA95 - Failed to connect for POKE!");
		return (FALSE);
	}

	char *poke_data = new char [length + 2*sizeof(int)];

	int *poke_data_int = (int*)poke_data;

	*poke_data_int 	= htonl (length + 2*sizeof(int));
	*(poke_data_int+1)= htonl (packet_type);

	memcpy (poke_data + 8, data, length);


	if(DDE_Class->Poke_Server( (LPBYTE) poke_data, ntohl(*poke_data_int) ) == FALSE) {
		WWDebugString("RA95 - POKE failed!\n");
		DDE_Class->Close_Poke_Connection();		// close down the link
		delete poke_data;
		return (FALSE);
	}

	DDE_Class->Close_Poke_Connection();		// close down the link

	delete poke_data;

	return (TRUE);
}


#endif	//WIN32
