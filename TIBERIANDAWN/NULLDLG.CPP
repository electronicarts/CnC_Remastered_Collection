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

/* $Header:   F:\projects\c&c\vcs\code\nulldlg.cpv   1.9   16 Oct 1995 16:52:12   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : NULLDLG.CPP                              *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 04/29/95                                 *
 *                                                                         *
 *                  Last Update : April 29, 1995 [BRR]                     *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Init_Null_Modem -- Initializes Null Modem communications              *
 *   Shutdown_Modem -- Shuts down modem/null-modem communications          *
 *   Test_Null_Modem -- Null-Modem test routine                            *
 *   Reconnect_Null_Modem -- allows user to reconnect								*
 *   Destroy_Null_Connection -- destroys the given connection					*
 *   Select_Serial_Dialog -- Serial Communications menu dialog             *
 *   Com_Settings_Dialog -- Lets user select serial port settings          *
 *   Com_Scenario_Dialog -- Serial game scenario selection dialog				*
 *   Phone_Dialog -- Lets user edit phone directory & dial                 *
 *   Build_InitString_Listbox -- [re]builds the initstring entry listbox   *
 *   Init_String_Compare -- for qsort													*
 *   Build_Phone_Listbox -- [re]builds the phone entry listbox             *
 *   Phone_Compare -- for qsort															*
 *   Edit_Phone_Dialog -- lets user edit a phone book entry                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "function.h"
#include "wincomm.h"
#include "modemreg.h"
#include "tcpip.h"

//PG_TO_FIX
#if (0)
ModemRegistryEntryClass *ModemRegistry = NULL;		//Ptr to modem registry data

//
// how much time (ticks) to go by before thinking other system
// is not responding.
//
#define PACKET_SENDING_TIMEOUT	1800
#define PACKET_CANCEL_TIMEOUT		900

//
// how much time (ticks) to go by before sending another packet
// of game options or serial connect.
//
#define PACKET_RETRANS_TIME	30
#define PACKET_REDRAW_TIME		60


static int Reconnect_Null_Modem( void );
static int Com_Settings_Dialog( SerialSettingsType *settings );
static int Phone_Dialog (void);
static void Build_Init_String_Listbox (ListClass *list, EditClass *edit, char *buf, int *index);
static int Init_String_Compare (const void *p1, const void *p2);
static void Build_Phone_Listbox (ListClass *list, EditClass *edit, char *buf);
static int Phone_Compare (const void *p1, const void *p2);
static int Edit_Phone_Dialog (PhoneEntryClass *phone);
static bool Dial_Modem( SerialSettingsType *settings, bool reconnect );
static bool Answer_Modem( SerialSettingsType *settings, bool reconnect );
static void Modem_Echo( char c );

void Smart_Printf( char *format, ... );
void Hex_Dump_Data( char *buffer, int length );
void itoh( int i, char *s);


static SerialPacketType SendPacket;
static SerialPacketType ReceivePacket;
char TheirName[MPLAYER_NAME_MAX];
unsigned char TheirColor;
HousesType TheirHouse;
unsigned char TheirID;
static char DialString[ CWAITSTRBUF_MAX + PhoneEntryClass::PHONE_MAX_NUM - 1 ];
static SerialSettingsType *DialSettings;

#define SHOW_MONO	0

/***************************************************************************
 * Init_Null_Modem -- Initializes Null Modem communications                *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		true = OK, false = error															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *=========================================================================*/
int Init_Null_Modem( SerialSettingsType *settings )
{

	if ( NullModem.Init( settings->Port, settings->IRQ,
								settings->ModemName,
								settings->Baud, 0,  8, 1,
								settings->HardwareFlowControl ) ) {


		return(true);
	} else {
		return(false);
	}
}


/***************************************************************************
 * Shutdown_Modem -- Shuts down modem/null-modem communications            *
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
 *   04/29/1995 BRR : Created.                                             *
 *=========================================================================*/
void Shutdown_Modem( void )
{
	if (!PlaybackGame) {
		if (GameToPlay == GAME_MODEM) {
			NullModem.Hangup_Modem();
		}
	}

	//
	// close port
	//
	NullModem.Shutdown();
}


/***************************************************************************
 * Modem_Signoff -- sends EXIT event										         *
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
 *   08/03/1995 DRD : Created.                                             *
 *=========================================================================*/
void Modem_Signoff( void )
{
	unsigned long starttime;
	EventClass event;

	if (!PlaybackGame) {
		/*------------------------------------------------------------------------
		Send a sign-off packet
		------------------------------------------------------------------------*/
		event.Type = EventClass::EXIT;
		NullModem.Send_Message (&event,sizeof(EventClass),0);
		NullModem.Send_Message (&event,sizeof(EventClass),0);

		starttime = TickCount.Time();
		while( (TickCount.Time() - starttime) < 30) {
			NullModem.Service();
		}
	}
}


/***************************************************************************
 * Test_Null_Modem -- Null-Modem test routine                              *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = failure to connect; 1 = I'm the game owner, 2 = I'm not				*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *=========================================================================*/
int Test_Null_Modem( void )
{
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_CANCEL = 100,
	};

	/*........................................................................
	Dialog variables
	........................................................................*/
	bool process = true;						// process while true
	KeyNumType input;

	int retval;
	unsigned long starttime;
	int packetlen;

	int x,y,width,height;					// dialog dimensions
	char buffer[80*3];

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	/*
	**	Determine the dimensions of the text to be used for the dialog box.
	**	These dimensions will control how the dialog box looks.
	*/
	strcpy( buffer, Text_String( TXT_WAITING_CONNECT ) );
	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String(buffer, 200*factor, width, height);

	width = MAX(width, 50*factor);
	width += 40*factor;
	height += 60*factor;

	x = (320*factor - width) / 2;
	y = (200*factor - height) / 2;

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		x + ((width - (String_Pixel_Width( Text_String( TXT_CANCEL ) ) + 8 *factor)) >> 1),
		y + height - (FontHeight + FontYSpacing + 2*factor) - 5*factor);

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);
	process = true;

	/*
	............................ Create the list .............................
	*/
	commands = &cancelbtn;

	commands->Flag_List_To_Redraw();

	/*
	............................ Draw the dialog .............................
	*/
	Hide_Mouse();
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();

	Dialog_Box(x, y, width, height);
	Draw_Caption(TXT_NONE, x, y, width);

	Fancy_Text_Print(buffer, x + 20*factor, y + 25*factor, CC_GREEN, TBLACK,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

	commands->Draw_All();
	while (Get_Mouse_State() > 0) Show_Mouse();

	/*
	** This is supposed to be a direct connection so hang up any modem on this port
	** just to annoy British Telecom
	*/
	/*
	** Go into break mode
	*/
	SetCommBreak(SerialPort->Get_Port_Handle());

	/*
	** Send hangup command
	*/
	SerialPort->Write_To_Serial_Port ((unsigned char*)"ATH\r", strlen("ATH\r"));
	CountDownTimerClass time;
	time.Set(2*60);
	while (time.Time()){}

	/*
	** Back out of break mode
	*/
	ClearCommBreak(SerialPort->Get_Port_Handle());

	/*
	** Drop DTR as well - just in case the modem still hasnt got the message
	*/
	EscapeCommFunction(SerialPort->Get_Port_Handle(), CLRDTR);


	/*------------------------------------------------------------------------
	Check for a packet.  If we detect one, the other system has already been
	started.  Wait 1/2 sec for him to receive my ACK, then exit with success.
	Note: The initial time must be a little longer than the resend delay.
		Just in case we just missed the packet.
	------------------------------------------------------------------------*/
	starttime = TickCount.Time();
	while ( TickCount.Time() - starttime < 80) {
		NullModem.Service();
		if ( NullModem.Get_Message( &ReceivePacket, &packetlen ) > 0) {
			if (ReceivePacket.Command == SERIAL_CONNECT) {
 //Smart_Printf( "Received SERIAL_CONNECT %d, ID %d \n", ReceivePacket.Seed, ReceivePacket.ID );
				starttime = TickCount.Time();
				while (TickCount.Time() - starttime < 30)
					NullModem.Service();
				process = false;
				retval = 2;
				break;
			}
		}
	}

	/*------------------------------------------------------------------------
	Send a packet across.  As long as Num_Send() is non-zero, the other system
	hasn't received it yet.
	------------------------------------------------------------------------*/
	if (process) {
		memset (&SendPacket, 0, sizeof(SerialPacketType));
		SendPacket.Command = SERIAL_CONNECT;
		//
		// put time from start of game for determining the host in case of tie.
		//
		SendPacket.Seed = TickCount.Time();
		SendPacket.ID = (int) buffer;		// address of buffer for more uniqueness.

 //Smart_Printf( "Sending SERIAL_CONNECT %d, ID %d \n", SendPacket.Seed, SendPacket.ID );
		NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

		starttime = TickCount.Time();
		while (TickCount.Time() - starttime < 80) {
			NullModem.Service();
			if (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {
				if (ReceivePacket.Command == SERIAL_CONNECT) {
 //Smart_Printf( "Received2 SERIAL_CONNECT %d, ID %d \n", ReceivePacket.Seed, ReceivePacket.ID );
					starttime = TickCount.Time();
					while (TickCount.Time() - starttime < 30)
						NullModem.Service();

					//
					// whoever has the highest time is the host
					//
					if (ReceivePacket.Seed > SendPacket.Seed) {
						process = false;
						retval = 2;
					} else {
						if (ReceivePacket.Seed == SendPacket.Seed) {
							if (ReceivePacket.ID > SendPacket.ID) {
								process = false;
								retval = 2;
							} else
							//
							// if they are equal then it's a loopback cable or a modem
							//
							if (ReceivePacket.ID == SendPacket.ID) {
								process = false;
								retval = 3;
							}
						}
					}

					break;
				}
			}
		}
	}

	starttime = TickCount.Time();

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			commands->Draw_All();
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
 //Smart_Printf( "Canceled waiting for SERIAL_CONNECT\n" );
				retval = 0;
				process = false;
				break;

			default:
				break;
		}
		/*.....................................................................
		Service the connection.
		.....................................................................*/
		NullModem.Service();
		if (NullModem.Num_Send() == 0) {
 //Smart_Printf( "No more messages to send.\n" );
			if (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {
				if (ReceivePacket.Command == SERIAL_CONNECT) {
 //Smart_Printf( "Received3 SERIAL_CONNECT %d, ID %d \n", ReceivePacket.Seed, ReceivePacket.ID );
					starttime = TickCount.Time();
					while (TickCount.Time() - starttime < 30)
						NullModem.Service();

					//
					// whoever has the highest time is the host
					//
					if (ReceivePacket.Seed > SendPacket.Seed) {
						process = false;
						retval = 2;
					} else {

						if (ReceivePacket.Seed == SendPacket.Seed) {
							if (ReceivePacket.ID > SendPacket.ID) {
								process = false;
								retval = 2;
							} else {

								//
								// if they are equal then it's a loopback cable or a modem
								//
								if (ReceivePacket.ID == SendPacket.ID) {
									process = false;
									retval = 3;
								}
							}
						}
					}

				} else {
					retval = 0;
					process = false;
				}
			} else {
				retval = 1;
				process = false;
			}
		}

		if (TickCount.Time() - starttime > 3600) {		// only wait 1 minute
			retval = 0;
			process = false;
		}
	}	/* end of while */

	return( retval );

}


/***************************************************************************
 * Reconnect_Modem -- allows user to reconnect										*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = failure to connect; 1 = connect OK											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *=========================================================================*/
int Reconnect_Modem( void )
{
	int status;
	int modemstatus;


	switch (ModemGameToPlay) {
		case (MODEM_NULL_HOST):
		case (MODEM_NULL_JOIN):
			status = Reconnect_Null_Modem();
			break;

		case (MODEM_DIALER):
			modemstatus = NullModem.Get_Modem_Status();
			if ( (modemstatus & CD_SET) ) {
 //Smart_Printf( "Dial Modem connection error!  Attempting reconnect....\n" );
				status = Reconnect_Null_Modem();
			} else {
				status = Dial_Modem( DialSettings, true );
			}
			break;

		case (MODEM_ANSWERER):
			modemstatus = NullModem.Get_Modem_Status();
			if ( (modemstatus & CD_SET) ) {
 //Smart_Printf( "Answer Modem connection error!  Attempting reconnect....\n" );
				status = Reconnect_Null_Modem();
			} else {
				status = Answer_Modem( DialSettings, true );
			}
			break;
	}

	return( status );
}


/***************************************************************************
 * Reconnect_Null_Modem -- allows user to reconnect								*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = failure to connect; 1 = connect OK											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Reconnect_Null_Modem( void )
{
	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_CANCEL = 100,
	};

	/*........................................................................
	Dialog variables
	........................................................................*/
	bool process = true;						// process while true
	KeyNumType input;

	int retval;
	unsigned long starttime;
	unsigned long lastmsgtime;
	int packetlen;

	int x,y,width,height;					// dialog dimensions
	char buffer[80*3];

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list


	/*
	**	Determine the dimensions of the text to be used for the dialog box.
	**	These dimensions will control how the dialog box looks.
	*/
	strcpy( buffer, Text_String( TXT_NULL_CONNERR_CHECK_CABLES ) );
	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String(buffer, 200, width, height);

	width = MAX(width, 50);
	width += 40;
	height += 60;

	x = (320 - width) / 2;
	y = (200 - height) / 2;

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		x + ((width - (String_Pixel_Width( Text_String( TXT_CANCEL ) ) + 8)) >> 1),
		y + height - (FontHeight + FontYSpacing + 2) - 5);

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);
	process = true;

	/*
	............................ Create the list .............................
	*/
	commands = &cancelbtn;

	commands->Flag_List_To_Redraw();

	/*
	............................ Draw the dialog .............................
	*/
	Hide_Mouse();

	Dialog_Box(x, y, width, height);
	Draw_Caption(TXT_NONE, x, y, width);

	Fancy_Text_Print(buffer, x + 20, y + 25, CC_GREEN, TBLACK,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

	commands->Draw_All();
	Show_Mouse();

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	starttime = lastmsgtime = TickCount.Time();
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			commands->Draw_All();
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				retval = false;
				process = false;
				break;

			default:
				break;
		}
		/*.....................................................................
		Service the connection.
		.....................................................................*/
		NullModem.Service();

		/*.....................................................................
		Resend our message if it's time
		.....................................................................*/
		if (TickCount.Time() - starttime > PACKET_RETRANS_TIME) {
			starttime = TickCount.Time();
			SendPacket.Command = SERIAL_CONNECT;
			SendPacket.ID = MPlayerLocalID;
 //Smart_Printf( "Sending a SERIAL_CONNECT packet !!!!!!!!\n" );
			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 0);
		}

		/*.....................................................................
		Check for an incoming message
		.....................................................................*/
		if (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {

			lastmsgtime = TickCount.Time();

			if (ReceivePacket.Command == SERIAL_CONNECT) {
 //Smart_Printf( "Received a SERIAL_CONNECT packet !!!!!!!!\n" );

				// are we getting our own packets back??

				if (ReceivePacket.ID == MPlayerLocalID) {
					CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);
					retval = false;
					process = false;
					break;
				}

				/*...............................................................
				OK, we got our message; now we have to make certain the other
				guy gets his, so send him one with an ACK required.
				...............................................................*/
				SendPacket.Command = SERIAL_CONNECT;
				SendPacket.ID = MPlayerLocalID;
				NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);
				starttime = TickCount.Time();
				while (TickCount.Time() - starttime < 60)
					NullModem.Service();
				retval = true;
				process = false;
			}
		}

		//
		// timeout if we do not get any packets
		//
		if (TickCount.Time() - lastmsgtime > PACKET_CANCEL_TIMEOUT) {
			retval = false;
			process = false;
		}

	}	/* end of while */

	return( retval );

}


/***********************************************************************************************
 * Destroy_Null_Connection -- destroys the given connection												  *
 *                                                                         						  *
 * Call this routine when a connection goes bad, or another player signs off.						  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		id			connection ID to destroy																		  *
 *		error		0 = user signed off; 1 = connection error													  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		none.																												  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   07/31/1995 DRD : Created.                                              						  *
 *=============================================================================================*/
void Destroy_Null_Connection(int id, int error)
{
	int i,j,idx;
	HousesType house;
	HouseClass *housep;
	char txt[80];


	if ( MPlayerCount == 1 ) {
		return;
	}

	// find index for id

	idx = -1;
	for (i = 0; i < MPlayerCount; i++) {
		if (MPlayerID[i] == (unsigned char)id) {
			idx = i;
			break;
		}
	}

	if (idx == -1) {
		return;
	}

	/*------------------------------------------------------------------------
	Create a message to display to the user
	------------------------------------------------------------------------*/
	txt[0] = '\0';
	if (error == 1) {
		sprintf(txt,Text_String(TXT_CONNECTION_LOST), MPlayerNames[idx] );
	}
	else if (error == 0) {
		sprintf(txt,Text_String(TXT_LEFT_GAME), MPlayerNames[idx] );
	}
	else if (error == -1) {
		NullModem.Delete_Connection();
	}

	if (strlen(txt)) {
		Messages.Add_Message (txt,
			MPlayerTColors[MPlayerID_To_ColorIndex((unsigned char)id)],
			TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 600, 0, 0);
		Map.Flag_To_Redraw(false);
	}

	for (i = 0; i < MPlayerCount; i++) {
		if (MPlayerID[i] == (unsigned char)id) {
			/*..................................................................
			Turn the player's house over to the computer's AI
			..................................................................*/
			house = MPlayerHouses[i];
			housep = HouseClass::As_Pointer (house);
			housep->IsHuman = false;

			/*..................................................................
			Move arrays back by one
			..................................................................*/
			for (j = i; j < MPlayerCount - 1; j++) {
				MPlayerID[j] = MPlayerID[j + 1];
				MPlayerHouses[j] = MPlayerHouses[j + 1];
				strcpy (MPlayerNames[j], MPlayerNames[j+1]);
				TheirProcessTime[j] = TheirProcessTime[j+1];
			}
		}
	}

	MPlayerCount--;

	/*------------------------------------------------------------------------
	If we're the last player left, tell the user.
	------------------------------------------------------------------------*/
	if (MPlayerCount == 1) {
		sprintf(txt,"%s",Text_String(TXT_JUST_YOU_AND_ME));
		Messages.Add_Message (txt,
			MPlayerTColors[MPlayerID_To_ColorIndex((unsigned char)id)],
			TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 600, 0, 0);
		Map.Flag_To_Redraw(false);
	}

}	/* end of Destroy_Null_Connection */


/***************************************************************************
 * Select_Serial_Dialog -- Serial Communications menu dialog               *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		GAME_MODEM				user wants to play a modem game						*
 *		GAME_NULL_MODEM		user wants to play a null-modem game				*
 *		GAME_NORMAL				user hit Cancel											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *=========================================================================*/
GameType Select_Serial_Dialog( void )
{
	int rc;
//	int value, i;
	int com = -1, baud = -1;
	int error = 0;

	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	int	d_dialog_w = 160 *factor;											// dialog width
	int	d_dialog_h = 94 *factor;											// dialog height
	int	d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
//		D_DIALOG_Y = ((200 - D_DIALOG_H) / 2),				// dialog y-coord
	int	d_dialog_y = ((136*factor - d_dialog_h) / 2);				// dialog y-coord
	int	d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int	d_txt6_h = 11*factor;												// ht of 6-pt text
	int	d_margin = 7;												// margin width/height

	int	d_dial_w = 90 *factor;
	int	d_dial_h = 9 *factor;
	int	d_dial_x = d_dialog_cx - d_dial_w / 2;
	int	d_dial_y = d_dialog_y + d_margin + d_txt6_h + d_margin;

	int	d_answer_w = 90 *factor;
	int	d_answer_h = 9 *factor;
	int	d_answer_x = d_dialog_cx - d_answer_w / 2;
	int	d_answer_y = d_dial_y + d_dial_h + 2;

	int	d_nullmodem_w = 90 *factor;
	int	d_nullmodem_h = 9 *factor;
	int	d_nullmodem_x = d_dialog_cx - d_nullmodem_w / 2;
	int	d_nullmodem_y = d_answer_y + d_answer_h + 2;

	int	d_settings_w = 90 *factor;
	int	d_settings_h = 9 *factor;
	int	d_settings_x = d_dialog_cx - d_settings_w / 2;
	int	d_settings_y = d_nullmodem_y + d_nullmodem_h + 2;

#if (GERMAN | FRENCH)
	int	d_cancel_w = 50 *factor;
#else
	int	d_cancel_w = 40 *factor;
#endif
	int	d_cancel_h = 9 *factor;
	int	d_cancel_x = d_dialog_cx - d_cancel_w / 2;
	int	d_cancel_y = d_settings_y + d_settings_h + d_margin;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_DIAL = 100,
		BUTTON_ANSWER,
		BUTTON_NULLMODEM,
		BUTTON_SETTINGS,
		BUTTON_CANCEL,

		NUM_OF_BUTTONS = 5,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display = REDRAW_ALL;		// redraw level
	bool process = true;						// process while true
	KeyNumType input;
	char namebuf[MPLAYER_NAME_MAX] = {0};		// buffer for player's name
	int tabs[] = {77 * factor};					// tabs for player list box
	GameType retval;									// return value

	int selection;
	bool pressed;
	int curbutton;
	TextButtonClass *buttons[NUM_OF_BUTTONS];

	SerialSettingsType *settings;
	bool selectsettings = false;


	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	TextButtonClass dialbtn(BUTTON_DIAL, TXT_DIAL_MODEM,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_dial_x, d_dial_y, d_dial_w, d_dial_h);

	TextButtonClass answerbtn(BUTTON_ANSWER, TXT_ANSWER_MODEM,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_answer_x, d_answer_y, d_answer_w, d_answer_h);

	TextButtonClass nullmodembtn(BUTTON_NULLMODEM, TXT_NULL_MODEM,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_nullmodem_x, d_nullmodem_y, d_nullmodem_w, d_nullmodem_h);

	TextButtonClass settingsbtn(BUTTON_SETTINGS, TXT_SETTINGS,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_settings_x, d_settings_y, d_settings_w, d_settings_h);

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_cancel_x, d_cancel_y);
//#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
//#endif

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	/*........................................................................
	Read the CC.INI file to extract default serial settings, scenario numbers
	& descriptions, and the phone list.
	........................................................................*/
	Read_MultiPlayer_Settings ();

	if (SerialDefaults.Port == 0 ||
		SerialDefaults.IRQ == -1 ||
		SerialDefaults.Baud == -1) {
		selectsettings = true;
	} else {
		if ( NullModem.Detect_Port( &SerialDefaults ) != PORT_VALID ) {
			selectsettings = true;
		}
	}

	/*
	............................ Create the list .............................
	*/
	commands = &dialbtn;
	answerbtn.Add_Tail(*commands);
	nullmodembtn.Add_Tail(*commands);
	settingsbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);

	/*
	......................... Fill array of button ptrs ......................
	*/
	curbutton = 0;
	buttons[0] = &dialbtn;
	buttons[1] = &answerbtn;
	buttons[2] = &nullmodembtn;
	buttons[3] = &settingsbtn;
	buttons[4] = &cancelbtn;
	buttons[curbutton]->Turn_On();

	Keyboard::Clear();

	Fancy_Text_Print(TXT_NONE, 0, 0, CC_GREEN, TBLACK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

Debug_Smart_Print = true;

	MPlayerLocalID = 0xff;		// set to invalid value

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	display = REDRAW_ALL;
	process = true;
	pressed = false;
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				/*
				..................... Refresh the backdrop ......................
				*/
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
				/*
				..................... Draw the background .......................
				*/
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				/*
				..................... Redraw the buttons .......................
				*/
				commands->Draw_All();
				/*
				....................... Draw the labels .........................
				*/
				Draw_Caption (TXT_SELECT_SERIAL_GAME, d_dialog_x, d_dialog_y, d_dialog_w);
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {
			case (BUTTON_DIAL | KN_BUTTON):
				selection = BUTTON_DIAL;
				pressed = true;
				break;

			case (BUTTON_ANSWER | KN_BUTTON):
				selection = BUTTON_ANSWER;
				pressed = true;
				break;

			case (BUTTON_NULLMODEM | KN_BUTTON):
				selection = BUTTON_NULLMODEM;
				pressed = true;
				break;

			case (BUTTON_SETTINGS | KN_BUTTON):
				selection = BUTTON_SETTINGS;
				pressed = true;
				break;

			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				selection = BUTTON_CANCEL;
				pressed = true;
				break;

			case (KN_UP):
				buttons[curbutton]->Turn_Off();
				buttons[curbutton]->Flag_To_Redraw();
				curbutton--;
				if (curbutton < 0)
					curbutton = (NUM_OF_BUTTONS - 1);
				buttons[curbutton]->Turn_On();
				buttons[curbutton]->Flag_To_Redraw();
				break;

			case (KN_DOWN):
				buttons[curbutton]->Turn_Off();
				buttons[curbutton]->Flag_To_Redraw();
				curbutton++;
				if (curbutton > (NUM_OF_BUTTONS - 1) )
					curbutton = 0;
				buttons[curbutton]->Turn_On();
				buttons[curbutton]->Flag_To_Redraw();
				break;

			case (KN_RETURN):
				selection = curbutton + BUTTON_DIAL;
				pressed = true;
				break;

			default:
				break;
		}

		if (pressed) {
			//
			// to make sure the selection is correct in case they used the mouse
			//
			buttons[curbutton]->Turn_Off();
			buttons[curbutton]->Flag_To_Redraw();
			curbutton = selection - BUTTON_DIAL;
			buttons[curbutton]->Turn_On();
			buttons[curbutton]->IsPressed = true;
			buttons[curbutton]->Draw_Me(true);

			switch (selection) {
				case (BUTTON_DIAL):

					if (selectsettings) {
						CCMessageBox().Process(TXT_SELECT_SETTINGS);
					}

					/*
					** Remote-connect
					*/
					else if ( Phone_Dialog() ) {
						if (PhoneBook[CurPhoneIdx]->Settings.Port == 0) {
							settings = &SerialDefaults;
						} else {
							settings = &(PhoneBook[CurPhoneIdx]->Settings);
						}

						if (SerialPort){
							delete SerialPort;
						}
						SerialPort = new WinModemClass;

						if ( Init_Null_Modem( settings ) ) {

							if (settings->CallWaitStringIndex == CALL_WAIT_CUSTOM) {
								strcpy( DialString, settings->CallWaitString );
							} else {
								strcpy( DialString,
									CallWaitStrings[ settings->CallWaitStringIndex ] );
							}
							strcat( DialString, PhoneBook[ CurPhoneIdx ]->Number );

							if ( Dial_Modem( settings, false ) ) {
								ModemGameToPlay = MODEM_DIALER;
								if ( Com_Scenario_Dialog() ) {
									retval = GAME_MODEM;
									process = false;
								}
							}

							if (process) {		// restore to default
								NullModem.Change_IRQ_Priority( 0 );
							}
						} else {
							CCMessageBox().Process(TXT_SELECT_SETTINGS);
						}
					}

					if (process) {
						buttons[curbutton]->IsPressed = false;
						buttons[curbutton]->Flag_To_Redraw();
  					}

					display = REDRAW_ALL;
					break;

				case (BUTTON_ANSWER):

					if (selectsettings) {
						CCMessageBox().Process(TXT_SELECT_SETTINGS);
					} else {
						/*
						** Remote-connect
						*/
						settings = &SerialDefaults;

						if (SerialPort){
							delete SerialPort;
						}
						SerialPort = new WinModemClass;

						if ( Init_Null_Modem( settings ) ) {
							if ( Answer_Modem( settings, false ) ) {
								ModemGameToPlay = MODEM_ANSWERER;
								if ( Com_Show_Scenario_Dialog() ) {
									retval = GAME_MODEM;
									process = false;
								}
							}

							if (process) {		// restore to default
								NullModem.Change_IRQ_Priority( 0 );
							}
						} else {
							CCMessageBox().Process(TXT_SELECT_SETTINGS);
						}
					}

					if (process) {
						buttons[curbutton]->IsPressed = false;
						buttons[curbutton]->Flag_To_Redraw();
					}

					display = REDRAW_ALL;
					break;

				case (BUTTON_NULLMODEM):

					if (selectsettings) {
						CCMessageBox().Process(TXT_SELECT_SETTINGS);
					} else {
						/*
						** Otherwise, remote-connect; save values if we're recording
						*/

						if (SerialPort){
							delete SerialPort;
						}
						SerialPort = new WinNullModemClass;

						if ( Init_Null_Modem( &SerialDefaults ) ) {
							rc = Test_Null_Modem();
							switch (rc) {
								case (1):
									ModemGameToPlay = MODEM_NULL_HOST;
									if ( Com_Scenario_Dialog() ) {
										retval = GAME_NULL_MODEM;
										process = false;
									}
									break;

								case (2):
									ModemGameToPlay = MODEM_NULL_JOIN;
									if ( Com_Show_Scenario_Dialog() ) {
										retval = GAME_NULL_MODEM;
										process = false;
									}
									break;

								case (3):
									CCMessageBox().Process( TXT_MODEM_OR_LOOPBACK );
									break;
							}

							if (process) {		// restore to default
								NullModem.Change_IRQ_Priority( 0 );
							}
						} else {
							CCMessageBox().Process(TXT_SELECT_SETTINGS);
						}
					}

					if (process) {
						buttons[curbutton]->IsPressed = false;
						buttons[curbutton]->Flag_To_Redraw();
					}

					display = REDRAW_ALL;
					break;

				case (BUTTON_SETTINGS):
					if ( Com_Settings_Dialog( &SerialDefaults ) ) {
						Write_MultiPlayer_Settings ();

						selectsettings = true;

						if (SerialDefaults.Port != 0 &&
							SerialDefaults.IRQ != -1 &&
							SerialDefaults.Baud != -1) {
							if ( NullModem.Detect_Port( &SerialDefaults ) == PORT_VALID) {
								selectsettings = false;
							}
						}
					}

					buttons[curbutton]->IsPressed = false;
					buttons[curbutton]->Flag_To_Redraw();
					display = REDRAW_ALL;
					break;

				case (BUTTON_CANCEL):
					retval = GAME_NORMAL;
					process = false;
					break;
			}

			pressed = false;
		}
	}	/* end of while */

#if 0
	if (retval == GAME_NORMAL) {
		Debug_Smart_Print = false;
	}
#endif

Debug_Smart_Print = false;

	return( retval );
}




/***********************************************************************************************
 * Advanced_Modem_Settings -- Allows to user to set additional modem settings                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    current settings                                                                  *
 *                                                                                             *
 * OUTPUT:   modified settings                                                                 *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    12/16/96 2:29PM ST : Created                                                             *
 *=============================================================================================*/
Advanced_Modem_Settings (SerialSettingsType *settings)
{

	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	int d_dialog_w = 340;													// dialog width
	int d_dialog_h = 170;													// dialog height
	int d_dialog_x = 320 - d_dialog_w/2;								// dialog x-coord
	int d_dialog_y = 200 - d_dialog_h/ 2;								// dialog y-coord


	int d_compression_w = 50;
	int d_compression_h = 18;
	int d_compression_x = d_dialog_x + d_dialog_w/2 +40;
	int d_compression_y = d_dialog_y + 30;

	int d_errorcorrection_w = 50;
	int d_errorcorrection_h = 18;
	int d_errorcorrection_x = d_dialog_x + d_dialog_w/2 +40;
	int d_errorcorrection_y = d_dialog_y + 52;

	int d_hardwareflowcontrol_w = 50;
	int d_hardwareflowcontrol_h = 18;
	int d_hardwareflowcontrol_x = d_dialog_x + d_dialog_w/2 +40;
	int d_hardwareflowcontrol_y = d_dialog_y + 74;

	int d_default_w = 100;
	int d_default_h = 18;
	int d_default_x = d_dialog_x + d_dialog_w / 2 - d_default_w / 2;
	int d_default_y = d_dialog_y + 110;

	int d_ok_w = 100;
	int d_ok_h = 18;
	int d_ok_x = d_dialog_x + d_dialog_w/2 - d_ok_w / 2;
	int d_ok_y = d_dialog_y + d_dialog_h - 24;

	enum {
		BUTTON_COMPRESSION = 100,
		BUTTON_ERROR_CORRECTION,
		BUTTON_HARDWARE_FLOW_CONTROL,
		BUTTON_DEFAULT,
		BUTTON_OK,
	};

	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND,
	} RedrawType;

	/*
	** Yes/No strings
	*/
	char compress_text [16];
	char correction_text [16];
	char flowcontrol_text[16];


	/*
	** Initialise the button text
	*/
	strcpy (compress_text, settings->Compression ? Text_String (TXT_ON) : Text_String (TXT_OFF) );
	strcpy (correction_text, settings->ErrorCorrection ?
												Text_String (TXT_ON) : Text_String (TXT_OFF) );
	strcpy (flowcontrol_text, settings->HardwareFlowControl ?
												Text_String (TXT_ON) : Text_String (TXT_OFF) );


	/*
	** Create the buttons
	*/
	TextButtonClass compressionbutton(BUTTON_COMPRESSION, compress_text,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_compression_x, d_compression_y, d_compression_w, d_compression_h);

	TextButtonClass errorcorrectionbutton(BUTTON_ERROR_CORRECTION, correction_text,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_errorcorrection_x, d_errorcorrection_y, d_errorcorrection_w, d_errorcorrection_h);

	TextButtonClass hardwareflowcontrolbutton(BUTTON_HARDWARE_FLOW_CONTROL, flowcontrol_text,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_hardwareflowcontrol_x, d_hardwareflowcontrol_y, d_hardwareflowcontrol_w, d_hardwareflowcontrol_h);

	TextButtonClass defaultbutton(BUTTON_DEFAULT, TXT_DEFAULT,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_default_x, d_default_y, d_default_w, d_default_h);

	TextButtonClass okbutton(BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_ok_x, d_ok_y, d_ok_w, d_ok_h);


	/*
	** Misc. variables.
	*/
	RedrawType display = REDRAW_ALL;		// redraw level
	BOOL process = true;						// process while true
	KeyNumType input;
	GadgetClass *commands;					// button list


	commands = &okbutton;
	defaultbutton.Add_Tail(*commands);
	compressionbutton.Add_Tail(*commands);
	errorcorrectionbutton.Add_Tail(*commands);
	hardwareflowcontrolbutton.Add_Tail(*commands);


	/*
	** Main process loop
	*/
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
				Set_Palette(Palette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				// init font variables

				Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Draw_Caption (TXT_MODEM_INITIALISATION, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print( TXT_DATA_COMPRESSION,
					d_compression_x - 26, d_compression_y + 2,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print( TXT_ERROR_CORRECTION,
					d_errorcorrection_x - 26, d_errorcorrection_y +2,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print( TXT_HARDWARE_FLOW_CONTROL,
					d_hardwareflowcontrol_x -26, d_hardwareflowcontrol_y +2,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}

			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				compressionbutton.Flag_To_Redraw();
				errorcorrectionbutton.Flag_To_Redraw();
				hardwareflowcontrolbutton.Flag_To_Redraw();
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {
			case (BUTTON_COMPRESSION | KN_BUTTON):
				settings->Compression = settings->Compression ^ 1;
				strcpy (compress_text, settings->Compression ?
										Text_String (TXT_ON) : Text_String (TXT_OFF) );
				break;

			case (BUTTON_ERROR_CORRECTION | KN_BUTTON):
				settings->ErrorCorrection = settings->ErrorCorrection ^ 1;
				strcpy (correction_text, settings->ErrorCorrection ?
										Text_String (TXT_ON) : Text_String (TXT_OFF) );
				break;

			case (BUTTON_HARDWARE_FLOW_CONTROL | KN_BUTTON):
				settings->HardwareFlowControl = settings->HardwareFlowControl ^ 1;
				strcpy (flowcontrol_text, settings->HardwareFlowControl ?
										Text_String (TXT_ON) : Text_String (TXT_OFF) );
				break;

			case (BUTTON_DEFAULT | KN_BUTTON):
				settings->Compression = false;
				settings->ErrorCorrection = false;
				settings->HardwareFlowControl = true;

				strcpy (compress_text, settings->Compression ?
										Text_String (TXT_ON) : Text_String (TXT_OFF) );

				strcpy (correction_text, settings->ErrorCorrection ?
										Text_String (TXT_ON) : Text_String (TXT_OFF) );

				strcpy (flowcontrol_text, settings->HardwareFlowControl ?
										Text_String (TXT_ON) : Text_String (TXT_OFF) );

				if (display < REDRAW_BUTTONS)	display = REDRAW_BUTTONS;
				break;

			case (BUTTON_OK | KN_BUTTON):
				process = false;
				break;
		}
	}
}




/***************************************************************************
 * Com_Settings_Dialog -- Lets user select serial port settings            *
 *                                                                         *
 *  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿               *
 *  ³                    Settings                          ³               *
 *  ³                                                      ³               *
 *  ³     Port:____       IRQ:__        Baud:______        ³               *
 *  ³  ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿      ³               *
 *  ³  ³            ³  ³            ³  ³            ³      ³               *
 *  ³  ³            ³  ³            ³  ³            ³      ³               *
 *  ³  ³            ³  ³            ³  ³            ³      ³               *
 *  ³  ³            ³  ³            ³  ³            ³      ³               *
 *  ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ      ³               *
 *  ³                                                      ³               *
 *  ³   Initialization:        [Add]   [Delete]            ³               *
 *  ³    _____________________________                     ³               *
 *  ³   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿     ³               *
 *  ³   ³                                            ³     ³               *
 *  ³   ³                                            ³     ³               *
 *  ³   ³                                            ³     ³               *
 *  ³   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ     ³               *
 *  ³                                                      ³               *
 *  ³   Call Waiting:                                      ³               *
 *  ³    _______________                                   ³               *
 *  ³   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿          [Tone Dialing]        ³               *
 *  ³   ³                 ³                                ³               *
 *  ³   ³                 ³          [Pulse Dialing]       ³               *
 *  ³   ³                 ³                                ³               *
 *  ³   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                                ³               *
 *  ³                                                      ³               *
 *  ³                   [OK]   [Cancel]                    ³               *
 *  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ               *
 *                                                                         *
 * INPUT:                                                                  *
 *		settings		ptr to SerialSettingsType structure								*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		true = OK, false = Cancel															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Com_Settings_Dialog( SerialSettingsType *settings )
{
/* ###Change collision detected! C:\PROJECTS\CODE\NULLDLG.CPP... */
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	int d_dialog_w = 301 *factor;											// dialog width
	int d_dialog_h = 200 *factor;											// dialog height
	int d_dialog_x = ((320 *factor - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200 *factor - d_dialog_h) / 2);				// dialog y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 6 *factor +1;												// ht of 6-pt text
	int d_margin = 5 *factor;												// margin width/height

#ifdef EDIT_IRQ
	int d_portlist_w = 80 *factor;
	int d_portlist_h = 35 *factor;
	int d_portlist_x = d_dialog_x + (d_dialog_w / 6) - (d_portlist_w / 2);
	int d_portlist_y = d_dialog_y + ((d_margin + d_txt6_h) * 2) + d_margin + 10 *factor;

	int d_port_w = ((PORTBUF_MAX - 1) * 6 *factor) + 4 *factor;
	int d_port_h = 9 *factor;
	int d_port_x = d_portlist_x + 31 *factor;
	int d_port_y = d_portlist_y - d_margin - d_txt6_h;


	int d_irqlist_w = 80 *factor;
	int d_irqlist_h = 35 *factor;
	int d_irqlist_x = d_dialog_x + (d_dialog_w / 2) - (d_irqlist_w / 2);
	int d_irqlist_y = d_portlist_y;

	int d_irq_w = ((IRQBUF_MAX - 1) * 6 *factor) + 3 *factor;
	int d_irq_h = 9 *factor;
	int d_irq_x = d_irqlist_x + 25 *factor;
	int d_irq_y = d_irqlist_y - d_margin - d_txt6_h;

	int d_baudlist_w = 80 *factor;
	int d_baudlist_h = 35 *factor;
	int d_baudlist_x = d_dialog_x + ((d_dialog_w * 5) / 6) - (d_baudlist_w / 2);
	int d_baudlist_y = d_portlist_y;

	int d_baud_w = ((BAUDBUF_MAX - 1) * 6 *factor) + 3 *factor;
	int d_baud_h = 9 *factor;
	int d_baud_x = d_baudlist_x + 31 *factor;
	int d_baud_y = d_baudlist_y - d_margin - d_txt6_h;

#endif	//EDIT_IRQ
	int d_initstrlist_w = ((INITSTRBUF_MAX - 1) * 6 *factor) + 8 + 3 *factor;
	int d_initstrlist_h = 21 *factor;
	int d_initstrlist_x = d_dialog_cx - (d_initstrlist_w / 2);
	int d_initstrlist_y = d_dialog_y + ((d_margin + d_txt6_h) * 2) + d_margin + 10 *factor +
								35*factor +
								((d_margin + d_txt6_h) * 2) + d_margin + 4 *factor;

	int d_initstr_w = ((INITSTRBUF_MAX - 1) * 6 *factor) + 3 *factor;
	int d_initstr_h = 9 *factor;
	int d_initstr_x = d_initstrlist_x;
	int d_initstr_y = d_initstrlist_y - d_margin - d_txt6_h;

#ifndef EDIT_IRQ
	int d_portlist_w = 80 *factor + 80;
	int d_portlist_h = 35 *factor;
	int d_portlist_x = d_initstrlist_x;
	int d_portlist_y = d_dialog_y + ((d_margin + d_txt6_h) * 2) + d_margin + 10 *factor;

	int d_port_w = d_portlist_w;
	int d_port_h = 9 *factor;
	int d_port_x = d_portlist_x;	// + 31 *factor;
	int d_port_y = d_portlist_y - d_margin - d_txt6_h;

	int d_baudlist_w = 80 *factor;
	int d_baudlist_h = 35 *factor;
	int d_baudlist_x = d_dialog_x + ((d_dialog_w * 5) / 6) - (d_baudlist_w / 2);
 		 d_baudlist_x -= 32;
	//int d_baudlist_x = d_portlist_x + d_portlist_w + 20 * factor;
	int d_baudlist_y = d_portlist_y;

	int d_baud_w = ((BAUDBUF_MAX - 1) * 6 *factor) + 3 *factor;
	int d_baud_h = 9 *factor;
	int d_baud_x = d_baudlist_x + 31 *factor;
	int d_baud_y = d_baudlist_y - d_margin - d_txt6_h;

	int d_inittype_w = 30*factor;
	int d_inittype_h = 9*factor;
	int d_inittype_x = d_dialog_x + ((d_dialog_w * 5) / 6) - (d_inittype_w / 2);
	int d_inittype_y = d_baud_y + 20*factor;

#endif	//EDIT_IRQ

	int d_add_w = 45 *factor;
	int d_add_h = 9 *factor;
#ifdef FRENCH
	int d_add_x = (d_dialog_cx - (d_add_w / 2))+34*factor;
#else
	int d_add_x = d_dialog_cx - (d_add_w / 2);
#endif
	int d_add_y = d_initstr_y - d_add_h - 3*factor;

	int d_delete_w = 45 *factor;
	int d_delete_h = 9 *factor;

#ifdef FRENCH
	int d_delete_x = 14*factor + d_dialog_x + ((d_dialog_w * 3) / 4) - (d_delete_w / 2);
#else
	int d_delete_x = d_dialog_x + ((d_dialog_w * 3) / 4) - (d_delete_w / 2);
#endif
	int d_delete_y = d_initstr_y - d_add_h - 3 *factor;

	int d_cwaitstrlist_w = (((CWAITSTRBUF_MAX - 1) + 9) * 6 *factor) + 3 *factor;
	int d_cwaitstrlist_h = 27 *factor;
	int d_cwaitstrlist_x = d_initstrlist_x;
	int d_cwaitstrlist_y = d_initstrlist_y + d_initstrlist_h + ((d_margin + d_txt6_h) * 2) + 2 *factor;

	int d_cwaitstr_w = ((CWAITSTRBUF_MAX - 1) * 6 *factor) + 3 *factor;
	int d_cwaitstr_h = 9 *factor;
	int d_cwaitstr_x = d_cwaitstrlist_x;
	int d_cwaitstr_y = d_cwaitstrlist_y - d_margin - d_txt6_h;

	int d_tone_w = 80 *factor;
	int d_tone_h = 9 *factor;
	int d_tone_x = d_dialog_x + ((d_dialog_w * 3) / 4) - (d_tone_w / 2);
	int d_tone_y = d_cwaitstrlist_y;

	int d_pulse_w = 80 *factor;
	int d_pulse_h = 9 *factor;
	int d_pulse_x = d_dialog_x + ((d_dialog_w * 3) / 4) - (d_pulse_w / 2);
	int d_pulse_y = d_tone_y + d_tone_h + d_margin;

	int d_save_w = 40 *factor;
	int d_save_h = 9 *factor;
	int d_save_x = d_dialog_x + (d_dialog_w / 5) - (d_save_w / 2);
	int d_save_y = d_dialog_y + d_dialog_h - d_save_h - d_margin - 2 *factor;

#if (GERMAN | FRENCH)
	int d_cancel_w = 50 *factor;
#else
	int d_cancel_w = 40 *factor;
#endif
	int d_cancel_h = 9 *factor;
	int d_cancel_x = d_dialog_x + ((d_dialog_w * 4) / 5) - (d_cancel_w / 2);
	int d_cancel_y = d_dialog_y + d_dialog_h - d_cancel_h - d_margin - 2 *factor;

#if (GERMAN | FRENCH)
	int d_advanced_w = 50*factor;
#else
	int d_advanced_w = 40*factor;
#endif
	int d_advanced_h = 9*factor;
	int d_advanced_x = d_dialog_x + ((d_dialog_w) / 2) - (d_advanced_w / 2);
	int d_advanced_y = d_dialog_y + d_dialog_h - d_advanced_h - d_margin - 2 *factor;



	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_PORT = 100,
		BUTTON_PORTLIST,
		BUTTON_IRQ,
		BUTTON_IRQLIST,
		BUTTON_BAUD,
		BUTTON_BAUDLIST,
		BUTTON_INITSTR,
		BUTTON_INITSTRLIST,
		BUTTON_ADD,
		BUTTON_DELETE,
		BUTTON_CWAITSTR,
		BUTTON_CWAITSTRLIST,
		BUTTON_TONE,
		BUTTON_PULSE,
		BUTTON_SAVE,
		BUTTON_ADVANCED,
		BUTTON_INITTYPE,
		BUTTON_CANCEL,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	static char *portname[4] = {
		"COM1 - 3F8",
		"COM2 - 2F8",
		"COM3 - 3E8",
		"COM4 - 2E8"
	};

	static char custom_port[10 + MODEM_NAME_MAX] = {"CUSTOM - ????"};

#ifdef EDIT_IRQ
	static char *irqname[5] = {
		"2 / 9",
		"3 - [COM2 & 4]",
		"4 - [COM1 & 3]",
		"5",
		"CUSTOM - ??"
	};

	static int _irqidx[4] = {
		2,
		1,
		2,
		1
	};
#endif	// EDIT_IRQ

	static char modemnames[10][MODEM_NAME_MAX];

	static char *baudname[5] = {
		"14400",
		"19200",
		"28800",
		"38400",
		"57600",
	};

	static char *init_types[2] = {
		"Normal",
		"Full",
	};

	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display = REDRAW_ALL;		// redraw level
	BOOL process = true;						// process while true
	KeyNumType input;
	char * item;								// general-purpose string
	char * temp;								// general-purpose string

	char portbuf[ PORTBUF_MAX ] = {0};	// buffer for port
#ifdef EDIT_IRQ
	char irqbuf[ IRQBUF_MAX ] = {0};		// buffer for irq
#endif	//EDIT_IRQ
	char baudbuf[ BAUDBUF_MAX ] = {0};	// buffer for baud
	char initstrbuf[ INITSTRBUF_MAX ] = {0};		// buffer for init string
	char cwaitstrbuf[ CWAITSTRBUF_MAX ] = {0};	// buffer for call waiting string

	int port_index = 1;	// index of currently-selected port (default = com2)
	int port_custom_index = 4;	//index of custom entry in port list
#ifdef EDIT_IRQ
	int irq_index = 1;	// index of currently-selected irq (default = 3)
#endif	//EDIT_IRQ
	int baud_index = 1;	// index of currently-selected baud (default = 19200)
	int initstr_index = 0;	// index of currently-selected modem init (default = "ATZ")
	int cwaitstr_index = CALL_WAIT_CUSTOM;	// index of currently-selected call waiting (default = "")
	int rc = 0;									// -1 = user cancelled, 1 = New
	int i;										// loop counter
	int pos;
	int len;
	int firsttime = 1;
	SerialSettingsType tempsettings;
	DetectPortType dpstatus;
	char	init_text[32];

	void const *up_button;
	void const *down_button;

	if (InMainLoop){
		up_button = Hires_Retrieve("BTN-UP.SHP");
		down_button = Hires_Retrieve("BTN-DN.SHP");
	}else{
		up_button = Hires_Retrieve("BTN-UP2.SHP");
		down_button = Hires_Retrieve("BTN-DN2.SHP");
	}


	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	EditClass port_edt (BUTTON_PORT,
		portbuf, PORTBUF_MAX,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_port_x, d_port_y, d_port_w, d_port_h, EditClass::ALPHANUMERIC);

	ListClass portlist(BUTTON_PORTLIST,
		d_portlist_x, d_portlist_y, d_portlist_w, d_portlist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

#ifdef EDIT_IRQ
	EditClass irq_edt (BUTTON_IRQ,
		irqbuf, IRQBUF_MAX,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_irq_x, d_irq_y, d_irq_w, d_irq_h, EditClass::NUMERIC);

	ListClass irqlist(BUTTON_IRQLIST,
		d_irqlist_x, d_irqlist_y, d_irqlist_w, d_irqlist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);
#endif	//EDIT_IRQ

	EditClass baud_edt (BUTTON_BAUD,
		baudbuf, BAUDBUF_MAX,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_baud_x, d_baud_y, d_baud_w, d_baud_h, EditClass::NUMERIC);

	ListClass baudlist(BUTTON_BAUDLIST,
		d_baudlist_x, d_baudlist_y, d_baudlist_w, d_baudlist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	EditClass initstr_edt (BUTTON_INITSTR,
		initstrbuf, INITSTRBUF_MAX,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_initstr_x, d_initstr_y, d_initstr_w, d_initstr_h, EditClass::ALPHANUMERIC);

	ListClass initstrlist(BUTTON_INITSTRLIST,
		d_initstrlist_x, d_initstrlist_y, d_initstrlist_w, d_initstrlist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	TextButtonClass addbtn(BUTTON_ADD, TXT_ADD,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#ifdef FRENCH
//		d_add_x, d_add_y);
//#else
		d_add_x, d_add_y, d_add_w, d_add_h);
//#endif

	TextButtonClass deletebtn(BUTTON_DELETE, TXT_DELETE_BUTTON,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#ifdef FRENCH
//		d_delete_x, d_delete_y);
//#else
		d_delete_x, d_delete_y, d_delete_w, d_delete_h);
//#endif

	EditClass cwaitstr_edt (BUTTON_CWAITSTR,
		cwaitstrbuf, CWAITSTRBUF_MAX,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_cwaitstr_x, d_cwaitstr_y, d_cwaitstr_w, d_cwaitstr_h, EditClass::ALPHANUMERIC);

	ListClass cwaitstrlist(BUTTON_CWAITSTRLIST,
		d_cwaitstrlist_x, d_cwaitstrlist_y, d_cwaitstrlist_w, d_cwaitstrlist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	TextButtonClass tonebtn(BUTTON_TONE, TXT_TONE_BUTTON,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_tone_x, d_tone_y, d_tone_w, d_tone_h);

	TextButtonClass pulsebtn(BUTTON_PULSE, TXT_PULSE_BUTTON,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_pulse_x, d_pulse_y, d_pulse_w, d_pulse_h);

	TextButtonClass savebtn(BUTTON_SAVE, TXT_SAVE_BUTTON,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_save_x, d_save_y);
//#else
		d_save_x, d_save_y, d_save_w, d_save_h);
//#endif

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_cancel_x, d_cancel_y);
//#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
//#endif
#if (0)
	TextButtonClass inittypebutton(BUTTON_INITTYPE, init_text,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_inittype_x, d_inittype_y, d_inittype_w, d_inittype_h);
#endif	//(0)

	TextButtonClass advancedbutton(BUTTON_ADVANCED, TXT_ADVANCED,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_advanced_x, d_advanced_y, d_advanced_w, d_advanced_h);

	/*
	----------------------------- Various Inits ------------------------------
	*/
	memcpy( &tempsettings, settings, sizeof(SerialSettingsType) );

	strcpy (init_text, init_types[tempsettings.Init]);

	if (tempsettings.Port == 0) {
		tempsettings.Port = 0x2f8;
	}

	if (tempsettings.IRQ == -1) {
		tempsettings.IRQ = 3;
	}

	if (tempsettings.Baud == -1) {
		tempsettings.Baud = 19200;
	}

	/*........................................................................
	Set the current indices
	........................................................................*/

#ifdef EDIT_IRQ
	switch ( tempsettings.IRQ ) {
		case ( 2 ):
			irq_index = 0;
			strcpy (irqbuf, "2");
			break;

		case ( 3 ):
			irq_index = 1;
			strcpy (irqbuf, "3");
			break;

		case ( 4 ):
			irq_index = 2;
			strcpy (irqbuf, "4");
			break;

		case ( 5 ):
			irq_index = 3;
			strcpy (irqbuf, "5");
			break;

		default:
			irq_index = 4;
			sprintf (irqbuf, "%d", tempsettings.IRQ);
			temp = strchr( irqname[4], '-' );
			if ( temp ) {
				pos = (int)(temp - irqname[4]) + 2;
				len = strlen( irqbuf );
				strncpy( irqname[4] + pos, irqbuf, len );
				*(irqname[4] + pos + len) = 0;
			}
			break;
	}
#endif	//EDIT_IRQ

	if (tempsettings.Baud == 14400) {
		baud_index = 0;
	} else {
		if (tempsettings.Baud == 19200) {
			baud_index = 1;
		} else {
			if (tempsettings.Baud == 28800) {
				baud_index = 2;
			} else {
				if (tempsettings.Baud == 38400) {
					baud_index = 3;
				} else {
					baud_index = 4;
				}
			}
		}
	}

	sprintf (baudbuf, "%d", tempsettings.Baud);

	/*........................................................................
	Set up the port list box & edit box
	........................................................................*/
	for (i = 0; i < 4; i++) {
		portlist.Add_Item( portname[ i ] );
	}

	/*
	** Loop through the first 10 possible modem entries in the registry. Frankly, its just
	** tough luck if the user has more than 10 modems attached!
	*/
	if (ModemRegistry) {
		delete ModemRegistry;
	}
	int modems_found = 0;
	for (i=0 ; i<10 ; i++) {
		ModemRegistry = new ModemRegistryEntryClass (i);
		if (ModemRegistry->Get_Modem_Name()) {
			strncpy (modemnames[modems_found], ModemRegistry->Get_Modem_Name(), MODEM_NAME_MAX);
			portlist.Add_Item( modemnames [modems_found++] );
			port_custom_index ++;
		}
		delete ModemRegistry;
	}
	ModemRegistry = NULL;

	portlist.Add_Item ( custom_port );


	/*
	** Work out the current port index
	*/
	port_index = -1;

	if (tempsettings.ModemName[0]) {
		for ( i=0 ; i<port_custom_index ; i++) {
			if (!stricmp (portlist.Get_Item(i), tempsettings.ModemName)) {
				port_index = i;
				strcpy (portbuf, tempsettings.ModemName);
				break;
			}
		}
		/*
		** The modem name specified wasnt in the registry so add it as a custom entry
		*/
		if (port_index == -1) {
			temp = strchr( custom_port, '-' );
			if ( temp ) {
				pos = (int)(temp - custom_port) + 2;
				len = strlen( tempsettings.ModemName );
				strncpy( custom_port + pos, tempsettings.ModemName, len );
				*(custom_port + pos + len) = 0;
				strcpy (portbuf, tempsettings.ModemName);
				port_index = port_custom_index;
			}
		}
	}

	if (port_index == -1) {
		switch ( tempsettings.Port ) {
			case ( 0x3f8 ):
				port_index = 0;
				strcpy (portbuf, "COM1");
				break;

			case ( 0x2f8 ):
				port_index = 1;
				strcpy (portbuf, "COM2");
				break;

			case ( 0x3e8 ):
				port_index = 2;
				strcpy (portbuf, "COM3");
				break;

			case ( 0x2e8 ):
				port_index = 3;
				strcpy (portbuf, "COM4");
				break;

			default:
				port_index = port_custom_index;
				sprintf (portbuf, "%x", tempsettings.Port);
				temp = strchr( custom_port, '-' );
				if ( temp ) {
					pos = (int)(temp - custom_port) + 2;
					len = strlen( portbuf );
					strncpy( custom_port + pos, portbuf, len );
					*(custom_port + pos + len) = 0;
				}
				break;
		}
	}

	portlist.Set_Selected_Index( port_index );

	/*
	** Set up the port edit box
	*/
	port_edt.Set_Text( portbuf, PORTBUF_MAX );

	/*........................................................................
	Set up the IRQ list box & edit box
	........................................................................*/
#ifdef EDIT_IRQ
	for (i = 0; i < 5; i++) {
		irqlist.Add_Item( irqname[ i ] );
	}

	irqlist.Set_Selected_Index( irq_index );
	irq_edt.Set_Text( irqbuf, IRQBUF_MAX );
#endif	//EDIT_IRQ

	/*........................................................................
	Set up the baud rate list box & edit box
	........................................................................*/
	for (i = 0; i < 5; i++) {
		baudlist.Add_Item( baudname[ i ] );
	}

	baudlist.Set_Selected_Index( baud_index );
	baud_edt.Set_Text( baudbuf, BAUDBUF_MAX );

	initstr_index = tempsettings.InitStringIndex;
	Build_Init_String_Listbox(&initstrlist, &initstr_edt, initstrbuf, &initstr_index);

	/*........................................................................
	Set up the cwait rate list box & edit box
	........................................................................*/

	cwaitstr_index = tempsettings.CallWaitStringIndex;
	for (i = 0; i < CALL_WAIT_STRINGS_NUM; i++) {
		if ( i == CALL_WAIT_CUSTOM ) {
			item = CallWaitStrings[ i ];
			temp = strchr( item, '-' );
			if ( temp ) {
				pos = (int)(temp - item) + 2;
				len = strlen( tempsettings.CallWaitString );
				strncpy( item + pos, tempsettings.CallWaitString, len );
				*(item + pos + len) = 0;
				if (i == cwaitstr_index) {
					strncpy( cwaitstrbuf, item + pos, CWAITSTRBUF_MAX );
				}
			}
		} else {
			if (i == cwaitstr_index) {
				strncpy( cwaitstrbuf, CallWaitStrings[ i ], CWAITSTRBUF_MAX );
			}
		}
		cwaitstrlist.Add_Item( CallWaitStrings[ i ] );
	}

	cwaitstrlist.Set_Selected_Index( cwaitstr_index );
	cwaitstr_edt.Set_Text( cwaitstrbuf, CWAITSTRBUF_MAX );

	/*........................................................................
	Build the button list
	........................................................................*/
	commands = &cancelbtn;
	port_edt.Add_Tail(*commands);
	portlist.Add_Tail(*commands);
#ifdef EDIT_IRQ
	irq_edt.Add_Tail(*commands);
	irqlist.Add_Tail(*commands);
#endif	// EDIT_IRQ
	baud_edt.Add_Tail(*commands);
	baudlist.Add_Tail(*commands);
	//inittypebutton.Add_Tail(*commands);
	initstr_edt.Add_Tail(*commands);
	initstrlist.Add_Tail(*commands);
	addbtn.Add_Tail(*commands);
	deletebtn.Add_Tail(*commands);
	cwaitstr_edt.Add_Tail(*commands);
	cwaitstrlist.Add_Tail(*commands);
	tonebtn.Add_Tail(*commands);
	pulsebtn.Add_Tail(*commands);
	savebtn.Add_Tail(*commands);
	advancedbutton.Add_Tail(*commands);


	if (tempsettings.DialMethod == DIAL_TOUCH_TONE) {
		tonebtn.Turn_On();
	} else {
		pulsebtn.Turn_On();
	}
	/*
	---------------------------- Processing loop -----------------------------
	*/
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		** Dont allow editing of non-custom ports to fix the problem of the cursor appearing
		** outside the edit box.
		*/
		if (port_index == port_custom_index) {
			port_edt.Set_Read_Only(false);
		}else{
			port_edt.Set_Read_Only(true);
		}

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
				Set_Palette(Palette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				// init font variables

				Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Draw_Caption (TXT_SETTINGS, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print( TXT_PORT_COLON,
					d_port_x - 3, d_port_y + 1 *factor,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

#ifdef EDIT_IRQ
				Fancy_Text_Print( TXT_IRQ_COLON,
					d_irq_x - 3, d_irq_y + 1 *factor,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
#endif	//EDIT_IRQ

				Fancy_Text_Print( TXT_BAUD_COLON,
					d_baud_x - 3, d_baud_y + 1 *factor,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print( TXT_INIT_STRING,
					d_initstr_x, d_initstr_y - d_txt6_h - 3 *factor,
					CC_GREEN, TBLACK,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print( TXT_CWAIT_STRING,
					d_cwaitstr_x, d_cwaitstr_y - d_txt6_h - 3 *factor,
					CC_GREEN, TBLACK,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
#if (0)
				Fancy_Text_Print ( "Modem Init",
					d_inittype_x, d_inittype_y - d_txt6_h - d_margin,
					CC_GREEN, TBLACK,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
#endif	//(0)
			}

			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				cancelbtn.Flag_To_Redraw();
				port_edt.Flag_To_Redraw();
				portlist.Flag_To_Redraw();
#ifdef EDIT_IRQ
				irq_edt.Flag_To_Redraw();
				irqlist.Flag_To_Redraw();
#endif	// EDIT_IRQ
				baud_edt.Flag_To_Redraw();
				baudlist.Flag_To_Redraw();
				//inittypebutton.Flag_To_Redraw();
				advancedbutton.Flag_To_Redraw();
				initstr_edt.Flag_To_Redraw();
				initstrlist.Flag_To_Redraw();
				addbtn.Flag_To_Redraw();
				deletebtn.Flag_To_Redraw();
				cwaitstr_edt.Flag_To_Redraw();
				cwaitstrlist.Flag_To_Redraw();
				tonebtn.Flag_To_Redraw();
				pulsebtn.Flag_To_Redraw();
				savebtn.Flag_To_Redraw();
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		if ( firsttime ) {
			port_edt.Set_Focus();
			port_edt.Flag_To_Redraw();
			input = commands->Input();
			firsttime = 0;
		}

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {
#if (0)
			case (BUTTON_INITTYPE | KN_BUTTON):
				tempsettings.Init = !tempsettings.Init;
				strcpy (init_text, init_types[tempsettings.Init]);
				inittypebutton.Flag_To_Redraw();
				break;


#endif	//(0)

			case (BUTTON_ADVANCED | KN_BUTTON):
				Advanced_Modem_Settings (&tempsettings);
				display = REDRAW_ALL;
				break;


			case (BUTTON_PORT | KN_BUTTON):
				item = (char *)portlist.Current_Item();
				if (port_index < 4) {
					temp = strchr( item, ' ' );
					if ( !temp ) {
						strncpy( portbuf, item, PORTBUF_MAX );
					} else {
						pos = (int)(temp - item);
						strncpy( portbuf, item, pos );
						portbuf[pos] = 0;
					}
					port_edt.Set_Text( portbuf, PORTBUF_MAX );
					port_edt.Flag_To_Redraw();
#ifdef EDIT_IRQ
					irq_edt.Set_Focus();
					irq_edt.Flag_To_Redraw();
#endif	//EDIT_IRQ
				} else {
					strupr( portbuf );
					if ( stricmp(portbuf, "3F8") == 0 ) {
						port_index = 0;
						portlist.Set_Selected_Index( port_index );
						strcpy (portbuf, "COM1");
						display = REDRAW_BUTTONS;
					}
					else if ( stricmp(portbuf, "2F8") == 0 ) {
						port_index = 1;
						portlist.Set_Selected_Index( port_index );
						strcpy (portbuf, "COM2");
						display = REDRAW_BUTTONS;
					}
					else if ( stricmp(portbuf, "3E8") == 0 ) {
						port_index = 2;
						portlist.Set_Selected_Index( port_index );
						strcpy (portbuf, "COM3");
						display = REDRAW_BUTTONS;
					}
					else if ( stricmp(portbuf, "2E8") == 0 ) {
						port_index = 3;
						portlist.Set_Selected_Index( port_index );
						strcpy (portbuf, "COM4");
						display = REDRAW_BUTTONS;
					}
					else if ( strncmp(portbuf, "COM", 3) == 0 ) {
						display = REDRAW_BUTTONS;

						switch ( (portbuf[3] - '0') ) {
							case 1:
								port_index = 0;
								break;

							case 2:
								port_index = 1;
								break;

							case 3:
								port_index = 2;
								break;

							case 4:
								port_index = 3;
								break;

							default:
								if (portbuf[3] <= '9' && portbuf[3] >'0') {
									portbuf[4] = 0;
									port_index = port_custom_index;
									temp = strchr( item, '-' );
									if ( temp ) {
										pos = (int)(temp - item) + 2;
										len = strlen( portbuf );
										strncpy( item + pos, portbuf, len );
										*(item + pos + len) = 0;
										display = REDRAW_BUTTONS;
									}
									break;
								}
								CCMessageBox().Process( TXT_INVALID_PORT_ADDRESS );
								port_edt.Set_Focus();
								display = REDRAW_ALL;
								break;
						}

						portlist.Set_Selected_Index( port_index );
					} else {
						temp = strchr( item, '-' );
						if ( temp ) {
							pos = (int)(temp - item) + 2;
							len = strlen( portbuf );
							strncpy( item + pos, portbuf, len );
							*(item + pos + len) = 0;
							display = REDRAW_BUTTONS;
						}
					}

#ifdef EDIT_IRQ
					if (display == REDRAW_BUTTONS) {
						irq_edt.Set_Focus();
						irq_edt.Flag_To_Redraw();
					}
#endif	//EDIT_IRQ
				}
				break;

			case (BUTTON_PORTLIST | KN_BUTTON):
				if (portlist.Current_Index() != port_index) {
					port_index = portlist.Current_Index();
					item = (char *)portlist.Current_Item();
					if (port_index < 4) {
						temp = strchr( item, ' ' );
						if ( !temp ) {
							strncpy( portbuf, item, PORTBUF_MAX );
						} else {
							pos = (int)(temp - item);
							strncpy( portbuf, item, pos );
							portbuf[pos] = 0;
						}
						port_edt.Clear_Focus();

						// auto select the irq for port

#ifdef EDIT_IRQ
						irq_index = _irqidx[ port_index ];
						irqlist.Set_Selected_Index( irq_index );
						item = (char *)irqlist.Current_Item();
						temp = strchr( item, ' ' );
						if ( !temp ) {
							strncpy( irqbuf, item, 2 );
						} else {
							pos = (int)(temp - item);
							strncpy( irqbuf, item, pos );
							irqbuf[pos] = 0;
						}
						irq_edt.Clear_Focus();
#endif	//EDIT_IRQ
					} else {
						if (port_index == port_custom_index) {
							/*
							** This is the custom entry
							*/
							temp = strchr( item, '-' );
							if ( temp ) {
								pos = (int)(temp - item) + 2;
								if ( *(item + pos) == '?' ) {
									portbuf[0] = 0;
								} else {
									strncpy( portbuf, item + pos, PORTBUF_MAX );
								}
							}
							port_edt.Set_Focus();
						}else{
							/*
							** Must be a modem name entry so just copy iy
							*/
							strncpy (portbuf, item, PORTBUF_MAX);
						}

					}
					port_edt.Set_Text( portbuf, PORTBUF_MAX );
					display = REDRAW_BUTTONS;
				} else {
					if (port_index < port_custom_index) {
						port_edt.Clear_Focus();
					} else {
						port_edt.Set_Focus();
					}
					display = REDRAW_BUTTONS;
				}
				break;

#ifdef EDIT_IRQ
			case (BUTTON_IRQ | KN_BUTTON):
				item = (char *)irqlist.Current_Item();
				if (irq_index < 4) {
					temp = strchr( item, ' ' );
					if ( !temp ) {
						strncpy( irqbuf, item, IRQBUF_MAX );
					} else {
						pos = (int)(temp - item);
						strncpy( irqbuf, item, pos );
						irqbuf[pos] = 0;
					}
					irq_edt.Set_Text( irqbuf, IRQBUF_MAX );
					irq_edt.Flag_To_Redraw();
				} else {
					temp = strchr( item, '-' );
					if ( temp ) {
						pos = (int)(temp - item) + 2;
						len = strlen( irqbuf );
						strncpy( item + pos, irqbuf, len );
						*(item + pos + len) = 0;
						display = REDRAW_BUTTONS;
					}
				}
				baud_edt.Set_Focus();
				baud_edt.Flag_To_Redraw();
				break;

			case (BUTTON_IRQLIST | KN_BUTTON):
				if (irqlist.Current_Index() != irq_index) {
					irq_index = irqlist.Current_Index();
					item = (char *)irqlist.Current_Item();
					if (irq_index < 4) {
						temp = strchr( item, ' ' );
						if ( !temp ) {
							strncpy( irqbuf, item, IRQBUF_MAX );
						} else {
							pos = (int)(temp - item);
							strncpy( irqbuf, item, pos );
							irqbuf[pos] = 0;
						}
						irq_edt.Clear_Focus();
					} else {
						temp = strchr( item, '-' );
						if ( temp ) {
							pos = (int)(temp - item) + 2;
							if ( *(item + pos) == '?' ) {
								irqbuf[0] = 0;
							} else {
								strncpy( irqbuf, item + pos, IRQBUF_MAX );
							}
						}
						irq_edt.Set_Focus();
					}
					irq_edt.Set_Text( irqbuf, IRQBUF_MAX );
				} else {
					if (irq_index < 4) {
						irq_edt.Clear_Focus();
					} else {
						irq_edt.Set_Focus();
					}
				}
				display = REDRAW_BUTTONS;
				break;
#endif	//EDIT_IRQ

			case (BUTTON_BAUD | KN_BUTTON):
				item = (char *)baudlist.Current_Item();
				strncpy( baudbuf, item, BAUDBUF_MAX );
				baud_edt.Set_Text( baudbuf, BAUDBUF_MAX );
				initstr_edt.Set_Focus();
				initstr_edt.Flag_To_Redraw();
				display = REDRAW_BUTTONS;
				break;

			case (BUTTON_BAUDLIST | KN_BUTTON):
				if (baudlist.Current_Index() != baud_index) {
					baud_index = baudlist.Current_Index();
					item = (char *)baudlist.Current_Item();
					strncpy( baudbuf, item, BAUDBUF_MAX );
					baud_edt.Set_Text( baudbuf, BAUDBUF_MAX );
					baud_edt.Clear_Focus();
					display = REDRAW_BUTTONS;
				}
				break;

#if 0
			case (BUTTON_INITSTR | KN_BUTTON):
				strupr( initstrbuf );
				strncpy( InitStrings[ initstr_index ], initstrbuf, INITSTRBUF_MAX );
				Build_Init_String_Listbox(&initstrlist, &initstr_edt, initstrbuf,
					&initstr_index);
				cwaitstr_edt.Set_Focus();
				cwaitstr_edt.Flag_To_Redraw();
				display = REDRAW_BUTTONS;
				break;
#endif

			case (BUTTON_INITSTRLIST | KN_BUTTON):
				if (initstrlist.Current_Index() != initstr_index) {
					initstr_index = initstrlist.Current_Index();
					item = (char *)initstrlist.Current_Item();
					strncpy( initstrbuf, item, INITSTRBUF_MAX );
					initstr_edt.Set_Text( initstrbuf, INITSTRBUF_MAX );
				}
				initstr_edt.Set_Focus();
				initstr_edt.Flag_To_Redraw();
				display = REDRAW_BUTTONS;
				break;

			/*------------------------------------------------------------------
			Add a new InitString entry
			------------------------------------------------------------------*/
			case (BUTTON_ADD | KN_BUTTON):

				item = new char[ INITSTRBUF_MAX ];
				memset (item, 0, INITSTRBUF_MAX);

				strupr ( initstrbuf );
				strncpy ( item, initstrbuf, INITSTRBUF_MAX-1 );

				InitStrings.Add ( item );
				Build_Init_String_Listbox (&initstrlist, &initstr_edt, initstrbuf,
					&initstr_index);
				/*............................................................
				Set the current listbox index to the newly-added item.
				............................................................*/
				for (i = 0; i < InitStrings.Count(); i++) {
					if (item == InitStrings[i]) {
						initstr_index = i;
						strcpy( initstrbuf, InitStrings[ initstr_index ] );
						initstr_edt.Set_Text( initstrbuf, INITSTRBUF_MAX );
						initstrlist.Set_Selected_Index( initstr_index );
					}
				}
				initstr_edt.Set_Focus();
				initstr_edt.Flag_To_Redraw();
				display = REDRAW_BUTTONS;
				break;

			/*------------------------------------------------------------------
			Delete the current InitString entry
			------------------------------------------------------------------*/
			case (BUTTON_DELETE | KN_BUTTON):

				if ( InitStrings.Count() && initstr_index != -1) {
					InitStrings.Delete( initstr_index );
					Build_Init_String_Listbox(&initstrlist, &initstr_edt, initstrbuf,
						&initstr_index);
				}
				break;

			case (BUTTON_CWAITSTR | KN_BUTTON):
				item = (char *)cwaitstrlist.Current_Item();
				if (cwaitstr_index < 3) {
				} else {
					temp = strchr( item, '-' );
					if ( temp ) {
						pos = (int)(temp - item) + 2;
						len = strlen( cwaitstrbuf );
						strncpy( item + pos, cwaitstrbuf, len );
						*(item + pos + len) = 0;
						display = REDRAW_BUTTONS;
					}
				}
				break;

			case (BUTTON_CWAITSTRLIST | KN_BUTTON):
				if (cwaitstrlist.Current_Index() != cwaitstr_index) {
					cwaitstr_index = cwaitstrlist.Current_Index();
					item = (char *)cwaitstrlist.Current_Item();
					if (cwaitstr_index < 3) {
						strncpy( cwaitstrbuf, item, CWAITSTRBUF_MAX );
						cwaitstr_edt.Clear_Focus();
					} else {
						temp = strchr( item, '-' );
						if ( temp ) {
							pos = (int)(temp - item) + 2;
							strncpy( cwaitstrbuf, item + pos, CWAITSTRBUF_MAX );
						}
						cwaitstr_edt.Set_Focus();
					}
					cwaitstr_edt.Set_Text( cwaitstrbuf, CWAITSTRBUF_MAX );
				} else {
					if (cwaitstr_index < 3) {
						cwaitstr_edt.Clear_Focus();
					} else {
						cwaitstr_edt.Set_Focus();
					}
				}
				display = REDRAW_BUTTONS;
				break;

			case (BUTTON_TONE | KN_BUTTON):
				tempsettings.DialMethod = DIAL_TOUCH_TONE;
				tonebtn.Turn_On();
				pulsebtn.Turn_Off();
				break;

			case (BUTTON_PULSE | KN_BUTTON):
				tempsettings.DialMethod = DIAL_PULSE;
				tonebtn.Turn_Off();
				pulsebtn.Turn_On();
				break;

			/*------------------------------------------------------------------
			SAVE: save the com settings
			------------------------------------------------------------------*/
			case (KN_RETURN):
			case (BUTTON_SAVE | KN_BUTTON):
				switch (port_index) {
					case ( 0 ):
						tempsettings.Port = 0x3f8;
						tempsettings.ModemName[0] = 0;
						break;

					case ( 1 ):
						tempsettings.Port = 0x2f8;
						tempsettings.ModemName[0] = 0;
						break;

					case ( 2 ):
						tempsettings.Port = 0x3e8;
						tempsettings.ModemName[0] = 0;
						break;

					case ( 3 ):
						tempsettings.Port = 0x2e8;
						tempsettings.ModemName[0] = 0;
						break;

					default:
						if (port_index == port_custom_index) {
							strncpy ( tempsettings.ModemName, portbuf, MODEM_NAME_MAX );
							tempsettings.Port = 1;
						} else {
							/*
							** Must be a modem name index
							*/
							strcpy (tempsettings.ModemName, portlist.Current_Item());
							tempsettings.Port = 1;
						}
						break;
				}

#ifdef EDIT_IRQ
				switch (irq_index) {
					case ( 0 ):
						tempsettings.IRQ = 2;
						break;

					case ( 1 ):
						tempsettings.IRQ = 3;
						break;

					case ( 2 ):
						tempsettings.IRQ = 4;
						break;

					case ( 3 ):
						tempsettings.IRQ = 5;
						break;

					default:
						sscanf( irqbuf, "%d", &tempsettings.IRQ );
						break;
				}
#endif	//EDIT_IRQ

				sscanf( baudbuf, "%d", &tempsettings.Baud );

				tempsettings.InitStringIndex = initstr_index;
				tempsettings.CallWaitStringIndex = cwaitstr_index;

				item = CallWaitStrings[ CALL_WAIT_CUSTOM ];
				temp = strchr( item, '-' );
				if ( temp ) {
					pos = (int)(temp - item) + 2;
					strncpy( cwaitstrbuf, item + pos, CWAITSTRBUF_MAX );
				} else {
					cwaitstrbuf[ 0 ] = 0;
				}

				strncpy( tempsettings.CallWaitString, cwaitstrbuf, CWAITSTRBUF_MAX );

				dpstatus = NullModem.Detect_Port( &tempsettings );

				if (dpstatus == PORT_VALID) {
					process = false;
					rc = true;
				}
				else if (dpstatus == PORT_INVALID) {
					CCMessageBox().Process( TXT_INVALID_SETTINGS );
					firsttime = 1;
					display = REDRAW_ALL;
				}
				else if (dpstatus == PORT_IRQ_INUSE) {
					CCMessageBox().Process( TXT_IRQ_ALREADY_IN_USE );
					firsttime = 1;
					display = REDRAW_ALL;
				}
				break;

			/*------------------------------------------------------------------
			CANCEL: send a SIGN_OFF, bail out with error code
			------------------------------------------------------------------*/
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				process = false;
				rc = false;
				break;
		}

	}	/* end of while */

	/*------------------------------------------------------------------------
	Restore screen
	------------------------------------------------------------------------*/
	Hide_Mouse();
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Show_Mouse();

	/*------------------------------------------------------------------------
	Save values into the Settings structure
	------------------------------------------------------------------------*/
	if (rc) {
		memcpy( settings, &tempsettings, sizeof(SerialSettingsType) );
	}

	return(rc);

}	/* end of Com_Settings_Dialog */


/***************************************************************************
 * Build_Init_String_Listbox -- [re]builds the initstring listbox          *
 *                                                                         *
 * This routine rebuilds the initstring list box from scratch; it also		*
 * updates the contents of the initstring edit field.								*
 *                                                                         *
 * INPUT:                                                                  *
 *		list		ptr to list box															*
 *		edit		ptr to edit box															*
 *		buf		ptr to buffer for initstring											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/08/1995 DRD : Created.                                             *
 *=========================================================================*/
static void Build_Init_String_Listbox (ListClass *list, EditClass *edit, char *buf, int *index)
{
	int i, curidx;
	char *item;


	curidx = *index;

	/*........................................................................
	Clear the list
	........................................................................*/
	while (list->Count()) {
		item = (char *)(list->Get_Item(0));
		list->Remove_Item(item);
		delete [] item;
	}

	/*
	** Now sort the init string list by name then number
	*/
	qsort ((void *)(&InitStrings[0]), InitStrings.Count(), sizeof(char *), Init_String_Compare);

	/*........................................................................
	Build the list
	........................................................................*/
	for (i = 0; i < InitStrings.Count(); i++) {
		item = new char[ INITSTRBUF_MAX ];
		strcpy( item, InitStrings[i] );
		list->Add_Item(item);
	}
	list->Flag_To_Redraw();

	/*........................................................................
	Init the current phone book index
	........................................................................*/
	if (list->Count() == 0 || curidx < -1) {
		curidx = -1;
	} else {
		if (curidx >= list->Count() ) {
			curidx = 0;
		}
	}

	/*........................................................................
	Fill in initstring edit buffer
	........................................................................*/
	if (curidx > -1) {
		strcpy (buf, InitStrings[ curidx ]);
		edit->Set_Text (buf, INITSTRBUF_MAX );
		list->Set_Selected_Index( curidx );
	}

	*index = curidx;
}


/***************************************************************************
 * Init_String_Compare -- for qsort														*
 *                                                                         *
 * INPUT:                                                                  *
 *		p1,p2		ptrs to elements to compare											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = same, -1 = (*p1) goes BEFORE (*p2), 1 = (*p1) goes AFTER (*p2)	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/08/1995 DRD : Created.                                             *
 *=========================================================================*/
static int Init_String_Compare (const void *p1, const void *p2)
{
	return( strcmp( *((char **)p1), *((char **)p2) ) );
}


/***********************************************************************************************
 * Com_Scenario_Dialog -- Serial game scenario selection dialog										  *
 *                                                                         						  *
 *                                                                         						  *
 *    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                      	  *
 *    ³                        Serial Game                         ³                       	  *
 *    ³                                                            ³                      	  *
 *    ³     Your Name: __________          House: [GDI] [NOD]      ³                    	 	  *
 *    ³       Credits: ______      Desired Color: [ ][ ][ ][ ]     ³ 	   						  *
 *    ³      Opponent: Name                                        ³                    	 	  *
 *    ³                                                            ³                    	 	  *
 *    ³                         Scenario                           ³   								  *
 *    ³                  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿                    ³ 								  *
 *    ³                  ³ Hell's Kitchen   ³³                    ³ 								  *
 *    ³                  ³ Heaven's Gate    ÃÄ´                    ³									  *
 *    ³                  ³      ...         ³ ³                    ³									  *
 *    ³                  ³                  ÃÄ´                    ³ 								  *
 *    ³                  ³                  ³³                    ³ 								  *
 *    ³                  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ                    ³ 								  *
 *    ³                 [  Bases   ] [ Crates     ]                ³                           *
 *    ³                 [ Tiberium ] [ AI Players ]                ³           					  *
 *    ³                                                            ³                   	 	  *
 *    ³                      [OK]    [Cancel]                      ³                    	 	  *
 *    ³   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿   ³                   	 	  *
 *    ³   ³                                                    ³   ³                   	 	  *
 *    ³   ³                                                    ³   ³                   	 	  *
 *    ³   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ   ³                   	 	  *
 *    ³                       [Send Message]                       ³                   	 	  *
 *    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                      	  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		none.																												  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		true = success, false = cancel																			  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		MPlayerName & MPlayerGameName must contain this player's name.									  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.                                              						  *
 *=============================================================================================*/
#define TXT_HOST_INTERNET_GAME 4567+1
#define TXT_JOIN_INTERNET_GAME 4567+2
int Com_Scenario_Dialog(void)
{
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	int d_dialog_w = 290*factor;											// dialog width
	int d_dialog_h = 190*factor;											// dialog height
	int d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*factor - d_dialog_h) / 2);				// dialog y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 6*factor+1;												// ht of 6-pt text
	int d_margin1 = 5*factor;												// margin width/height
	int d_margin2 = 2*factor;												// margin width/height

	int d_name_w = 70*factor;
	int d_name_h = 9*factor;
	int d_name_x = d_dialog_x + 108*factor;
	int d_name_y = d_dialog_y + d_margin1 + d_txt6_h + d_txt6_h + d_margin1;

	int d_credits_w = ((CREDITSBUF_MAX - 1) * 6*factor) + 3*factor;
	int d_credits_h = 9*factor;
	int d_credits_x = d_name_x;
	int d_credits_y = d_name_y + d_name_h + d_margin2;

	int d_gdi_w = 30*factor;
	int d_gdi_h = 9*factor;
	int d_gdi_x = d_dialog_cx + (d_dialog_w / 4);
	int d_gdi_y = d_dialog_y + d_margin1 + d_txt6_h + d_txt6_h + d_margin1;

	int d_nod_w = 30*factor;
	int d_nod_h = 9*factor;
	int d_nod_x = d_gdi_x + d_gdi_w + (d_margin1 / 2);
	int d_nod_y = d_gdi_y;

	int d_color_w = 10*factor;
	int d_color_h = 9*factor;
	int d_color_y = d_gdi_y + d_gdi_h + d_margin2;

	int d_opponent_x = d_name_x;
	int d_opponent_y = d_color_y + d_color_h + d_margin2;

	int d_scenariolist_w = 182*factor;
	int d_scenariolist_h = 27*factor;
	int d_scenariolist_x = d_dialog_cx - (d_scenariolist_w / 2);
	int d_scenariolist_y = d_opponent_y + d_txt6_h + 3*factor + d_txt6_h;

	// d_count_x is calculated below after other enums
	int d_count_w = 25*factor;
	int d_count_h = 7*factor;
	int d_count_y = d_scenariolist_y + d_scenariolist_h + d_margin2;

	// d_level_x is calculated below after other enums
	int d_level_w = 25*factor;
	int d_level_h = 7*factor;
	int d_level_y = d_count_y;

#if (GERMAN | FRENCH)
	int d_bases_w = 120*factor;//BGA:100;
#else
	int d_bases_w = 110*factor;
#endif
	int d_bases_h = 9*factor;
	int d_bases_x = d_dialog_cx - d_bases_w - d_margin2;
	int d_bases_y = d_count_y + d_count_h + d_margin2;

#if (GERMAN | FRENCH)
	int d_goodies_w = 120*factor;
#else
	int d_goodies_w = 110*factor;
#endif
	int d_goodies_h = 9*factor;
	int d_goodies_x = d_dialog_cx + d_margin2;
	int d_goodies_y = d_bases_y;

	int d_count_x = d_dialog_cx - d_count_w - ((2 * 6*factor) + 3*factor)
					- ((d_bases_w - ((13 * 6*factor) + 3*factor + d_count_w)) / 2) - d_margin2;

	int d_level_x = d_dialog_cx + (11 * 6*factor)
					+ ((d_goodies_w - ((13 * 6*factor) + 3*factor + d_level_w)) / 2) + d_margin2;

#if (GERMAN | FRENCH)
	int d_tiberium_w = 120*factor;
#else
	int d_tiberium_w = 110*factor;
#endif
	int d_tiberium_h = 9*factor;
	int d_tiberium_x = d_dialog_cx - d_bases_w - d_margin2;
	int d_tiberium_y = d_bases_y + d_bases_h + d_margin2;

#if (GERMAN | FRENCH)
	int d_ghosts_w = 120*factor;
#else
	int d_ghosts_w = 110*factor;
#endif
	int d_ghosts_h = 9*factor;
	int d_ghosts_x = d_dialog_cx + d_margin2;
	int d_ghosts_y = d_tiberium_y;

	int d_ok_w = 45*factor;
	int d_ok_h = 9*factor;
	int d_ok_x = d_tiberium_x + (d_tiberium_w / 2) - (d_ok_w / 2);
	int d_ok_y = d_tiberium_y + d_tiberium_h + d_margin1;

	int d_cancel_w = 45*factor;
	int d_cancel_h = 9*factor;
	int d_cancel_x = d_ghosts_x + (d_ghosts_w / 2) - (d_cancel_w / 2);
	int d_cancel_y = d_tiberium_y + d_tiberium_h + d_margin1;

	int d_message_w = d_dialog_w - (d_margin1 * 2);
	int d_message_h = 34*factor;
	int d_message_x = d_dialog_x + d_margin1;
	int d_message_y = d_cancel_y + d_cancel_h + d_margin1;

	int d_send_w = 80*factor;
	int d_send_h = 9*factor;
	int d_send_x = d_dialog_cx - (d_send_w / 2);
	int d_send_y = d_message_y + d_message_h + d_margin2;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_NAME = 100,
		BUTTON_GDI,
		BUTTON_NOD,
		BUTTON_CREDITS,
		BUTTON_SCENARIOLIST,
		BUTTON_COUNT,
		BUTTON_LEVEL,
		BUTTON_BASES,
		BUTTON_TIBERIUM,
		BUTTON_GOODIES,
		BUTTON_GHOSTS,
		BUTTON_OK,
		BUTTON_CANCEL,
		BUTTON_SEND,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_MESSAGE,
		REDRAW_COLORS,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display = REDRAW_ALL;		// redraw level
	bool process = true;						// process while true
	KeyNumType input;

	char namebuf[MPLAYER_NAME_MAX] = {0};		// buffer for player's name
	char credbuf[CREDITSBUF_MAX];					// for credit edit box
	int old_cred;								// old value in credits buffer
	int transmit;								// 1 = re-transmit new game options
	int cbox_x[] = {	d_gdi_x,
							d_gdi_x + d_color_w,
							d_gdi_x + (d_color_w * 2),
							d_gdi_x + (d_color_w * 3),
							d_gdi_x + (d_color_w * 4),
							d_gdi_x + (d_color_w * 5)};
	int parms_received = 0;					// 1 = game options received
	int changed = 0;							// 1 = user has changed an option

	int rc;
	int recsignedoff = false;
	int i;
	int version;
	char txt[80];
	unsigned long starttime;
	unsigned long timingtime;
	unsigned long lastmsgtime;
	unsigned long lastredrawtime;
	unsigned long transmittime = 0;
	unsigned long theirresponsetime;
	int packetlen;
	static int first_time = 1;
	bool oppscorescreen = false;
	bool gameoptions = false;
	EventClass *event;					// event ptr
	unsigned long msg_timeout = 1200;	// init to 20 seconds

	int					message_length;
	int					sent_so_far;
	unsigned short		magic_number;
	unsigned short		crc;
	bool					ready_to_go = false;
	CountDownTimerClass ready_time;

	void const *up_button;
	void const *down_button;

	if (InMainLoop){
		up_button = Hires_Retrieve("BTN-UP.SHP");
		down_button = Hires_Retrieve("BTN-DN.SHP");
	}else{
		up_button = Hires_Retrieve("BTN-UP2.SHP");
		down_button = Hires_Retrieve("BTN-DN2.SHP");
	}

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	EditClass name_edt (BUTTON_NAME,
		namebuf, MPLAYER_NAME_MAX,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_name_x, d_name_y, d_name_w, d_name_h, EditClass::ALPHANUMERIC);

	TextButtonClass gdibtn(BUTTON_GDI, TXT_G_D_I,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_gdi_x, d_gdi_y, d_gdi_w, d_gdi_h);

	TextButtonClass nodbtn(BUTTON_NOD, TXT_N_O_D,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_nod_x, d_nod_y, d_nod_w, d_nod_h);

	EditClass credit_edt (BUTTON_CREDITS,
		credbuf, CREDITSBUF_MAX,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_credits_x, d_credits_y, d_credits_w, d_credits_h, EditClass::ALPHANUMERIC);

	ListClass scenariolist(BUTTON_SCENARIOLIST,
		d_scenariolist_x, d_scenariolist_y, d_scenariolist_w, d_scenariolist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	GaugeClass countgauge (BUTTON_COUNT,
		d_count_x, d_count_y, d_count_w, d_count_h);

	GaugeClass levelgauge (BUTTON_LEVEL,
		d_level_x, d_level_y, d_level_w, d_level_h);

	TextButtonClass basesbtn(BUTTON_BASES, TXT_BASES_OFF,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_bases_x, d_bases_y, d_bases_w, d_bases_h);

	TextButtonClass tiberiumbtn(BUTTON_TIBERIUM, TXT_TIBERIUM_OFF,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_tiberium_x, d_tiberium_y, d_tiberium_w, d_tiberium_h);

	TextButtonClass goodiesbtn(BUTTON_GOODIES, TXT_CRATES_OFF,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_goodies_x, d_goodies_y, d_goodies_w, d_goodies_h);

	TextButtonClass ghostsbtn(BUTTON_GHOSTS, TXT_AI_PLAYERS_OFF,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_ghosts_x, d_ghosts_y, d_ghosts_w, d_ghosts_h);

	TextButtonClass okbtn(BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_ok_x, d_ok_y, d_ok_w, d_ok_h);

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_cancel_x, d_cancel_y);
//#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
//#endif

	TextButtonClass sendbtn(BUTTON_SEND, TXT_SEND_MESSAGE,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_send_x, d_send_y);
//#else
		d_send_x, d_send_y, d_send_w, d_send_h);
//#endif

	/*
	------------------------- Build the button list --------------------------
	*/
	commands = &name_edt;
	gdibtn.Add_Tail(*commands);
	nodbtn.Add_Tail(*commands);
	credit_edt.Add_Tail(*commands);
	scenariolist.Add_Tail(*commands);
	countgauge.Add_Tail(*commands);
	levelgauge.Add_Tail(*commands);
	basesbtn.Add_Tail(*commands);
	tiberiumbtn.Add_Tail(*commands);
	goodiesbtn.Add_Tail(*commands);
	ghostsbtn.Add_Tail(*commands);
	okbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);
	sendbtn.Add_Tail(*commands);

	/*
	----------------------------- Various Inits ------------------------------
	*/
	/*........................................................................
	Init player name & house
	........................................................................*/
	MPlayerColorIdx = MPlayerPrefColor;			// init my preferred color
	strcpy (namebuf, MPlayerName);				// set my name
	name_edt.Set_Text(namebuf,MPLAYER_NAME_MAX);
	name_edt.Set_Color(MPlayerTColors[MPlayerColorIdx]);

	if (MPlayerHouse==HOUSE_GOOD) {
		gdibtn.Turn_On();
	} else {
		nodbtn.Turn_On();
	}

	/*........................................................................
	Init scenario values, only the first time through
	........................................................................*/
	if (first_time) {
		MPlayerCredits = 3000;						// init credits & credit buffer
		MPlayerBases = 1;								// init scenario parameters
		MPlayerTiberium = 0;
		MPlayerGoodies = 0;
		MPlayerGhosts = 0;
		Special.IsCaptureTheFlag = 0;
		MPlayerUnitCount = (MPlayerCountMax[MPlayerBases] + MPlayerCountMin[MPlayerBases]) / 2;
		first_time = 0;
	}

	/*........................................................................
	Init button states
	........................................................................*/
	if (MPlayerBases) {
		basesbtn.Turn_On();
		basesbtn.Set_Text(TXT_BASES_ON);
	}
	if (MPlayerTiberium) {
		tiberiumbtn.Turn_On();
		tiberiumbtn.Set_Text(TXT_TIBERIUM_ON);
	}
	if (MPlayerGoodies) {
		goodiesbtn.Turn_On();
		goodiesbtn.Set_Text(TXT_CRATES_ON);
	}
	if (MPlayerGhosts) {
		ghostsbtn.Turn_On();
		ghostsbtn.Set_Text(TXT_AI_PLAYERS_ON);
	}
	if (Special.IsCaptureTheFlag) {
		MPlayerGhosts = 0;
		ghostsbtn.Turn_On();
		ghostsbtn.Set_Text(TXT_CAPTURE_THE_FLAG);
	}

	sprintf(credbuf, "%d", MPlayerCredits);
	credit_edt.Set_Text(credbuf, CREDITSBUF_MAX);
	old_cred = MPlayerCredits;

	levelgauge.Set_Maximum(MPLAYER_BUILD_LEVEL_MAX - 1);
	levelgauge.Set_Value(BuildLevel - 1);

	countgauge.Set_Maximum(MPlayerCountMax[MPlayerBases] - MPlayerCountMin[MPlayerBases]);
	countgauge.Set_Value(MPlayerUnitCount - MPlayerCountMin[MPlayerBases]);

	/*........................................................................
	Init other scenario parameters
	........................................................................*/
	Special.IsTGrowth = MPlayerTiberium;
	Special.IsTSpread = MPlayerTiberium;
	transmit = 1;

	/*........................................................................
	Init scenario description list box
	........................................................................*/
	for (i = 0; i < MPlayerScenarios.Count(); i++) {
		scenariolist.Add_Item (strupr(MPlayerScenarios[i]));
	}
	ScenarioIdx = 0;								// 1st scenario is selected

	/*........................................................................
	Init random-number generator, & create a seed to be used for all random
	numbers from here on out
	........................................................................*/
	randomize();
	Seed = rand();

	/*........................................................................
	Init the message display system
	........................................................................*/
	Messages.Init (d_message_x + 2*factor, d_message_y + 2*factor, 4, MAX_MESSAGE_LENGTH, d_txt6_h);

	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Set_Palette(Palette);

	extern char ModemRXString[];

	if (strlen(ModemRXString) > 36)
		ModemRXString[36] = 0;

	if (strlen(ModemRXString) > 0)
		Messages.Add_Message (ModemRXString, CC_TAN,
			TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 1200, 0, 0);

	ModemRXString[0] = '\0';

	/*
	---------------------------- Init Mono Output ----------------------------
	*/
	#if(SHOW_MONO)
	NullModem.Configure_Debug(sizeof (CommHeaderType),sizeof (SerialCommandType),
		SerialPacketNames, 106);
	NullModem.Mono_Debug_Print(1);
	#endif

	/*
	---------------------------- Processing loop -----------------------------
	*/
	NullModem.Reset_Response_Time();		// clear response time
	theirresponsetime = 10000;				// initialize to an invalid value
	timingtime = lastmsgtime = lastredrawtime = TickCount.Time();
	while (Get_Mouse_State() > 0) Show_Mouse();


	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		#if(SHOW_MONO)
		NullModem.Mono_Debug_Print(0);
		#endif

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				// init font variables

				Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
#ifdef FORCE_WINSOCK
				if (Winsock.Get_Connected()){
					Draw_Caption (TXT_HOST_INTERNET_GAME, d_dialog_x, d_dialog_y, d_dialog_w);
				}else{
					Draw_Caption (TXT_HOST_SERIAL_GAME, d_dialog_x, d_dialog_y, d_dialog_w);
				}
#else
				Draw_Caption (TXT_HOST_SERIAL_GAME, d_dialog_x, d_dialog_y, d_dialog_w);
#endif	//FORCE_WINSOCK

				Fancy_Text_Print(TXT_YOUR_NAME,
					d_name_x - 5*factor, d_name_y + 1*factor,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print(TXT_SIDE_COLON,
					d_gdi_x - 5*factor, d_gdi_y + 1*factor,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print (TXT_START_CREDITS_COLON, d_credits_x - 5*factor, d_credits_y + 1*factor,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print(TXT_COLOR_COLON,
					cbox_x[0] - 5*factor, d_color_y + 1*factor,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print(TXT_SCENARIOS,
					d_scenariolist_x + (d_scenariolist_w / 2),
					d_scenariolist_y - d_txt6_h,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print (TXT_COUNT, d_count_x - 3*factor, d_count_y, CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_RIGHT);

				Fancy_Text_Print (TXT_LEVEL, d_level_x - 3*factor, d_level_y, CC_GREEN, TBLACK,
					TPF_NOSHADOW | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_RIGHT);
			}

			/*..................................................................
			Draw the color boxes
			..................................................................*/
			if (display >= REDRAW_COLORS) {
				for (i = 0; i < MAX_MPLAYER_COLORS; i++) {
					LogicPage->Fill_Rect (cbox_x[i] + 1*factor, d_color_y + 1*factor,
						cbox_x[i] + 1*factor + d_color_w - 2*factor, d_color_y + 1*factor + d_color_h - 2*factor,
						MPlayerGColors[i]);

					if (i == MPlayerColorIdx) {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h,
							BOXSTYLE_GREEN_DOWN, false);
					} else {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h,
							BOXSTYLE_GREEN_RAISED, false);
					}
				}
			}

			/*..................................................................
			Draw the message:
			- Erase an old message first
			..................................................................*/
			if (display >= REDRAW_MESSAGE) {
				Draw_Box(d_message_x, d_message_y, d_message_w, d_message_h,
					BOXSTYLE_GREEN_BORDER, true);
				Messages.Draw();

				LogicPage->Fill_Rect (d_dialog_x + 2*factor,
					d_opponent_y,
					d_dialog_x + d_dialog_w - 4*factor,
					d_opponent_y + d_txt6_h,
					BLACK);

				if (parms_received) {
					if (oppscorescreen) {
						sprintf(txt,"%s",Text_String(TXT_WAITING_FOR_OPPONENT));

						int txtwidth = String_Pixel_Width( txt );

						Fancy_Text_Print (txt, d_dialog_cx - (txtwidth / 2),
							d_opponent_y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
					} else {
						Fancy_Text_Print (TXT_OPPONENT_COLON, d_opponent_x - 3*factor,
							d_opponent_y, CC_GREEN, TBLACK,
							TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						if (TheirHouse == HOUSE_GOOD) {
							sprintf(txt,"%s %s",TheirName,Text_String(TXT_G_D_I));
						} else {
							sprintf(txt,"%s %s",TheirName,Text_String(TXT_N_O_D));
						}

						Fancy_Text_Print (txt, d_opponent_x,
							d_opponent_y, MPlayerTColors[TheirColor], TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
					}
				}

				sprintf( txt, "%d ", MPlayerUnitCount );
				Fancy_Text_Print (txt, d_count_x + d_count_w + 3*factor,
					d_count_y, CC_GREEN, BLACK,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				if (BuildLevel <= MPLAYER_BUILD_LEVEL_MAX) {
					sprintf(txt, "%d ", BuildLevel);
				} else {
					sprintf(txt, "**");
				}
				Fancy_Text_Print (txt, d_level_x + d_level_w + 3*factor,
					d_level_y, CC_GREEN, BLACK,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}

			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Flag_List_To_Redraw();
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {
			/*------------------------------------------------------------------
			User clicks on a color button
			------------------------------------------------------------------*/
			case KN_LMOUSE:
				if (_Kbd->MouseQX > cbox_x[0] &&
					_Kbd->MouseQX < (cbox_x[MAX_MPLAYER_COLORS - 1] + d_color_w) &&
					_Kbd->MouseQY > d_color_y &&
					_Kbd->MouseQY < (d_color_y + d_color_h)) {
						if (!ready_to_go){
							MPlayerPrefColor = (_Kbd->MouseQX - cbox_x[0]) / d_color_w;
							MPlayerColorIdx = MPlayerPrefColor;
							display = REDRAW_COLORS;

							name_edt.Set_Color (MPlayerTColors[MPlayerColorIdx]);
							name_edt.Flag_To_Redraw();
							MPlayerCredits = atoi(credbuf);
							strcpy (MPlayerName, namebuf);
							transmit = 1;
							changed = 1;
						}
				}
				break;

			/*------------------------------------------------------------------
			User edits the name field; retransmit new game options
			------------------------------------------------------------------*/
			case (BUTTON_NAME | KN_BUTTON):
				if (!ready_to_go){
					credit_edt.Clear_Focus();
					credit_edt.Flag_To_Redraw();
					MPlayerCredits = atoi(credbuf);
					strcpy (MPlayerName, namebuf);
					transmit = 1;
					changed = 1;
				}
				break;

			/*------------------------------------------------------------------
			House Buttons: set the player's desired House
			------------------------------------------------------------------*/
			case (BUTTON_GDI | KN_BUTTON):
				if (!ready_to_go){
					MPlayerHouse = HOUSE_GOOD;
					gdibtn.Turn_On();
					nodbtn.Turn_Off();
					MPlayerCredits = atoi(credbuf);
					strcpy (MPlayerName, namebuf);
					transmit = 1;
				}
				break;

			case (BUTTON_NOD | KN_BUTTON):
				if (!ready_to_go){
					MPlayerHouse = HOUSE_BAD;
					gdibtn.Turn_Off();
					nodbtn.Turn_On();
					MPlayerCredits = atoi(credbuf);
					strcpy (MPlayerName, namebuf);
					transmit = 1;
				}
				break;

			/*------------------------------------------------------------------
			User edits the credits value; retransmit new game options
			------------------------------------------------------------------*/
			case (BUTTON_CREDITS | KN_BUTTON):
				if (!ready_to_go){
					name_edt.Clear_Focus();
					name_edt.Flag_To_Redraw();
					MPlayerCredits = atoi(credbuf);
					strcpy (MPlayerName, namebuf);
					transmit = 1;
				}
				break;

			/*------------------------------------------------------------------
			New Scenario selected.
			------------------------------------------------------------------*/
			case (BUTTON_SCENARIOLIST | KN_BUTTON):
				if (scenariolist.Current_Index() != ScenarioIdx && !ready_to_go) {
					ScenarioIdx = scenariolist.Current_Index();
					MPlayerCredits = atoi(credbuf);
					strcpy (MPlayerName, namebuf);
					transmit = 1;
				}
				break;

			/*------------------------------------------------------------------
			User adjusts max # units
			------------------------------------------------------------------*/
			case (BUTTON_COUNT | KN_BUTTON):
				if (!ready_to_go){
					MPlayerUnitCount = countgauge.Get_Value() + MPlayerCountMin[MPlayerBases];
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
					transmit = 1;
				}
				break;

			/*------------------------------------------------------------------
			User adjusts build level
			------------------------------------------------------------------*/
			case (BUTTON_LEVEL | KN_BUTTON):
				if (!ready_to_go){
					BuildLevel = levelgauge.Get_Value() + 1;
					if (BuildLevel > MPLAYER_BUILD_LEVEL_MAX)	// if it's pegged, max it out
						BuildLevel = MPLAYER_BUILD_LEVEL_MAX;
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
					transmit = 1;
				}
				break;

			/*------------------------------------------------------------------
			Toggle bases
			------------------------------------------------------------------*/
			case (BUTTON_BASES | KN_BUTTON):
				if (!ready_to_go){
					if (MPlayerBases) {
						MPlayerBases = 0;
						basesbtn.Turn_Off();
						basesbtn.Set_Text(TXT_BASES_OFF);
						MPlayerUnitCount = Fixed_To_Cardinal (MPlayerCountMax[0]-MPlayerCountMin[0],
							Cardinal_To_Fixed(MPlayerCountMax[1]-MPlayerCountMin[1],
							MPlayerUnitCount-MPlayerCountMin[1])) + MPlayerCountMin[0];
					} else {
						MPlayerBases = 1;
						basesbtn.Turn_On();
						basesbtn.Set_Text(TXT_BASES_ON);
						MPlayerUnitCount = Fixed_To_Cardinal (MPlayerCountMax[1]-MPlayerCountMin[1],
							Cardinal_To_Fixed(MPlayerCountMax[0]-MPlayerCountMin[0],
							MPlayerUnitCount-MPlayerCountMin[0])) + MPlayerCountMin[1];
					}
					MPlayerCredits = atoi(credbuf);
					countgauge.Set_Maximum(MPlayerCountMax[MPlayerBases] - MPlayerCountMin[MPlayerBases]);
					countgauge.Set_Value(MPlayerUnitCount - MPlayerCountMin[MPlayerBases]);
					strcpy (MPlayerName, namebuf);
					transmit = 1;
					display = REDRAW_ALL;
				}
				break;

			/*------------------------------------------------------------------
			Toggle tiberium
			------------------------------------------------------------------*/
			case (BUTTON_TIBERIUM | KN_BUTTON):
				if (!ready_to_go){
					if (MPlayerTiberium) {
						MPlayerTiberium = 0;
						Special.IsTGrowth = 0;
						Special.IsTSpread = 0;
						tiberiumbtn.Turn_Off();
						tiberiumbtn.Set_Text(TXT_TIBERIUM_OFF);
					} else {
						MPlayerTiberium = 1;
						Special.IsTGrowth = 1;
						Special.IsTSpread = 1;
						tiberiumbtn.Turn_On();
						tiberiumbtn.Set_Text(TXT_TIBERIUM_ON);
					}
					MPlayerCredits = atoi(credbuf);
					strcpy (MPlayerName, namebuf);
					transmit = 1;
				}
				break;

			/*------------------------------------------------------------------
			Toggle goodies
			------------------------------------------------------------------*/
			case (BUTTON_GOODIES | KN_BUTTON):
				if (!ready_to_go){
					if (MPlayerGoodies) {
						MPlayerGoodies = 0;
						goodiesbtn.Turn_Off();
						goodiesbtn.Set_Text(TXT_CRATES_OFF);
					} else {
						MPlayerGoodies = 1;
						goodiesbtn.Turn_On();
						goodiesbtn.Set_Text(TXT_CRATES_ON);
					}
					MPlayerCredits = atoi(credbuf);
					strcpy (MPlayerName, namebuf);
					transmit = 1;
				}
				break;

			/*------------------------------------------------------------------
			Toggle ghosts
			------------------------------------------------------------------*/
			case (BUTTON_GHOSTS | KN_BUTTON):
				if (!ready_to_go){
					if (!MPlayerGhosts && !Special.IsCaptureTheFlag) {	// ghosts OFF => ghosts ON
						MPlayerGhosts = 1;
						Special.IsCaptureTheFlag = 0;
						ghostsbtn.Turn_On();
						ghostsbtn.Set_Text(TXT_AI_PLAYERS_ON);
					}
					else if (MPlayerGhosts) {					// ghosts ON => capture-flag
						MPlayerGhosts = 0;
						Special.IsCaptureTheFlag = 1;
						ghostsbtn.Turn_On();
						ghostsbtn.Set_Text(TXT_CAPTURE_THE_FLAG);
					}
					else if (Special.IsCaptureTheFlag) {		// capture-flag => AI OFF
						MPlayerGhosts = 0;
						Special.IsCaptureTheFlag = 0;
						ghostsbtn.Turn_Off();
						ghostsbtn.Set_Text(TXT_AI_PLAYERS_OFF);
					}
					MPlayerCredits = atoi(credbuf);
					strcpy (MPlayerName, namebuf);
					transmit = 1;
				}
				break;

			/*------------------------------------------------------------------
			OK: exit loop with true status
			------------------------------------------------------------------*/
			case (BUTTON_OK | KN_BUTTON):
				if (!ready_to_go){
					//
					// make sure we got a game options packet from the other player
					//
					if (gameoptions) {
						//rc = true;
						//process = false;

						// force transmitting of game options packet one last time



						SendPacket.Command = SERIAL_READY_TO_GO;
						SendPacket.ID = ModemGameToPlay;
						NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

						starttime = TickCount.Time();

						while ( ( NullModem.Num_Send()
							&& ((TickCount.Time() - starttime) < PACKET_SENDING_TIMEOUT) )){
							#if(SHOW_MONO)
							NullModem.Mono_Debug_Print(0);
							#endif

							NullModem.Service();
							Keyboard::Check();		//Make sure the message loop gets called
						}

						ready_to_go = true;
						ready_time.Set(120, true);

						transmit = 1;
						transmittime = 0;

					} else {
						CCMessageBox().Process (TXT_ONLY_ONE,TXT_OOPS,NULL);
						display = REDRAW_ALL;
					}
				}
				break;

			/*------------------------------------------------------------------
			CANCEL: send a SIGN_OFF, bail out with error code
			------------------------------------------------------------------*/
			case (KN_ESC):
				if (!ready_to_go){
					if (Messages.Get_Edit_Buf() != NULL) {
						Messages.Input(input);
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
						break;
					}
				}
			case (BUTTON_CANCEL | KN_BUTTON):
				if (!ready_to_go){
					process = false;
					rc = false;
				}
				break;

			/*------------------------------------------------------------------
			Default: manage the inter-player messages
			------------------------------------------------------------------*/
			default:
				if (ready_to_go) break;

				/*...............................................................
				F4/SEND/'M' = send a message
				...............................................................*/
				if (Messages.Get_Edit_Buf()==NULL) {
					if (input == KN_M || input==(BUTTON_SEND | KN_BUTTON) ||
						input == KN_F4) {
						memset (txt, 0, 80);

						strcpy(txt,Text_String(TXT_MESSAGE));	// "Message:"

						Messages.Add_Edit (MPlayerTColors[MPlayerColorIdx],
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, d_message_w-70*factor);
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;

						credit_edt.Clear_Focus();
						credit_edt.Flag_To_Redraw();
						name_edt.Clear_Focus();
						name_edt.Flag_To_Redraw();

						break;
					}
				} else {
					if ( input == (BUTTON_SEND | KN_BUTTON) ) {
						input = KN_RETURN;
					}
				}

				/*...............................................................
				Manage the message system (get rid of old messages)
				...............................................................*/
				if (Messages.Manage()) {
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
				}

				/*...............................................................
				Service keyboard input for any message being edited.
				...............................................................*/
				i = Messages.Input(input);

				/*...............................................................
				If 'Input' returned 1, it means refresh the message display.
				...............................................................*/
				if (i==1) {
					Messages.Draw();
				}

				/*...............................................................
				If 'Input' returned 2, it means redraw the message display.
				...............................................................*/
				else if (i==2) {
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
				}

				/*...............................................................
				If 'input' returned 3, it means send the current message.
				...............................................................*/
				else if (i==3) {
					long actual_message_size;
					char *the_string;

					sent_so_far = 0;
					magic_number = MESSAGE_HEAD_MAGIC_NUMBER;
					message_length = strlen(Messages.Get_Edit_Buf());
					crc = (unsigned short)
							(Calculate_CRC(Messages.Get_Edit_Buf(), message_length) &0xffff);

					while (sent_so_far < message_length){

						SendPacket.Command = SERIAL_MESSAGE;
						strcpy (SendPacket.Name, MPlayerName);
						SendPacket.ID = Build_MPlayerID(MPlayerColorIdx, MPlayerHouse);
						memcpy (SendPacket.Message, Messages.Get_Edit_Buf()+sent_so_far, COMPAT_MESSAGE_LENGTH-5);

						/*
						** Steve I's stuff for splitting message on word boundries
						*/
						actual_message_size = COMPAT_MESSAGE_LENGTH - 5;

						/* Start at the end of the message and find a space with 10 chars. */
						the_string = SendPacket.Message;
						while ( (COMPAT_MESSAGE_LENGTH -5) -actual_message_size < 10 &&
							the_string[actual_message_size] != ' '){
							--actual_message_size;
						}
						if ( the_string[actual_message_size] == ' ' ){

							/* Now delete the extra characters after the space (they musnt print) */
							for ( int i=0 ; i< (COMPAT_MESSAGE_LENGTH-5) - actual_message_size; i++ ){
								the_string[i + actual_message_size] = 0xff;
							}
						}else{
							actual_message_size = COMPAT_MESSAGE_LENGTH - 5;
						}

						*(SendPacket.Message + COMPAT_MESSAGE_LENGTH-5) = 0;
						*((unsigned short*)(SendPacket.Message + COMPAT_MESSAGE_LENGTH-4)) = magic_number;
						*((unsigned short*)(SendPacket.Message + COMPAT_MESSAGE_LENGTH-2)) = crc;

						/*..................................................................
						Send the message
						..................................................................*/
						NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);
						NullModem.Service();

						/*..................................................................
						Add the message to our own screen
						..................................................................*/
						sprintf(txt, Text_String (TXT_FROM), MPlayerName, SendPacket.Message);
						Messages.Add_Message (txt, MPlayerTColors[MPlayerColorIdx],
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 1200, magic_number, crc);

						magic_number++;
						sent_so_far += actual_message_size;	//COMPAT_MESSAGE_LENGTH-5;
					}

					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
				}	/* end of send message */

		}	/* end of input processing */

		/*---------------------------------------------------------------------
		Detect editing of the credits buffer, transmit new values to players
		---------------------------------------------------------------------*/
		if (atoi(credbuf) != old_cred) {
			old_cred = Bound(atoi(credbuf), 0, 9999);
			MPlayerCredits = old_cred;
			transmit = 1;
			sprintf(credbuf, "%d", MPlayerCredits);
			credit_edt.Set_Text(credbuf, CREDITSBUF_MAX);
		}

		/*---------------------------------------------------------------------
		Detect editing of the name buffer, transmit new values to players
		---------------------------------------------------------------------*/
		if (strcmp (namebuf, MPlayerName)) {
			strcpy (MPlayerName, namebuf);
			transmit = 1;
			changed = 1;
		}

		/*---------------------------------------------------------------------
		If our Transmit flag is set, we need to send out a game option packet.
		This message requires an ACK.  The first time through the loop, transmit
		should be set, so we send out our default options; we'll then send
		any changes we make to the defaults.
		---------------------------------------------------------------------*/
		if (transmit && (TickCount.Time() - transmittime) > PACKET_RETRANS_TIME) {
			SendPacket.Command = SERIAL_GAME_OPTIONS;
			strcpy (SendPacket.Name, MPlayerName);
#ifdef PATCH
			if (IsV107) {
				SendPacket.Version = 1;
			} else {
				SendPacket.Version = 2;
			}
#else
			SendPacket.Version = Version_Number();
#endif
			SendPacket.House = MPlayerHouse;
			SendPacket.Color = MPlayerColorIdx;

			SendPacket.Scenario = MPlayerFilenum[ScenarioIdx];

			SendPacket.Credits = MPlayerCredits;
			SendPacket.IsBases = MPlayerBases;
			SendPacket.IsTiberium = MPlayerTiberium;
			SendPacket.IsGoodies = MPlayerGoodies;
			SendPacket.IsGhosties = MPlayerGhosts;
			SendPacket.BuildLevel = BuildLevel;
			SendPacket.UnitCount = MPlayerUnitCount;
			SendPacket.Seed = Seed;
			SendPacket.Special = Special;
			SendPacket.GameSpeed = Options.GameSpeed;
			SendPacket.ID = ModemGameToPlay;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			transmittime = TickCount.Time();
			transmit = 0;

			starttime = TickCount.Time();
			while ( ( NullModem.Num_Send()
				&& ((TickCount.Time() - starttime) < PACKET_SENDING_TIMEOUT) )){
				#if(SHOW_MONO)
				NullModem.Mono_Debug_Print(0);
				#endif

				NullModem.Service();
				Keyboard::Check();		//Make sure the message loop gets called
			}
		}

		//
		// send a timing packet if enough time has gone by.
		//
		if ( (TickCount.Time() - timingtime) > PACKET_TIMING_TIMEOUT) {
			SendPacket.Command = SERIAL_TIMING;
			SendPacket.ResponseTime = NullModem.Response_Time();
			SendPacket.ID = ModemGameToPlay;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 0);
			timingtime = TickCount.Time();
		}

		/*---------------------------------------------------------------------
		Check for an incoming message
		---------------------------------------------------------------------*/
		while (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {
// Smart_Printf( "received packet of length %d\n", packetlen );

			lastmsgtime = TickCount.Time();
			msg_timeout = 600;		// reset timeout value to 10 seconds
											// (only the 1st time through is 20 seconds)

			// are we getting our own packets back??

			if (ReceivePacket.Command >= SERIAL_CONNECT &&
				ReceivePacket.Command < SERIAL_LAST_COMMAND &&
				ReceivePacket.Command != SERIAL_MESSAGE &&
				ReceivePacket.ID == ModemGameToPlay) {

				CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);

				// to skip the other system not responding msg
				lastmsgtime = TickCount.Time();

				process = false;
				rc = false;

				// say we did receive sign off to keep from sending one
				recsignedoff = true;
				break;
			}

			event = (EventClass *)&ReceivePacket;
			if (event->Type <= EventClass::FRAMEINFO) {
				if ( (TickCount.Time() - lastredrawtime) > PACKET_REDRAW_TIME) {
					lastredrawtime = TickCount.Time();
					oppscorescreen = true;

if (display != REDRAW_ALL) {
	display = REDRAW_MESSAGE;
}

//					display = REDRAW_MESSAGE;
					parms_received = 1;
				}
			} else {
				switch ( ReceivePacket.Command ) {
					/*..................................................................
					Sign-off: Give the other machine time to receive my ACK, display a
					message, and exit.
					..................................................................*/
					case (SERIAL_SIGN_OFF):
// Smart_Printf( "received sign off\n" );
						starttime = TickCount.Time();
						while (TickCount.Time() - starttime < 60)
							NullModem.Service();
						CCMessageBox().Process(TXT_USER_SIGNED_OFF);

						// to skip the other system not responding msg
						lastmsgtime = TickCount.Time();

						process = false;
						rc = false;
						recsignedoff = true;
						break;

					/*..................................................................
					Game Options:  Store the other machine's name, color & house;
					If they've picked the same color as myself, re-transmit my settings
					to force him to choose a different color.  (Com_Show_Scenario_Dialog
					is responsible for ensuring the colors are different.)
					..................................................................*/
					case (SERIAL_GAME_OPTIONS):
// Smart_Printf( "received game options\n" );
						oppscorescreen = false;
						gameoptions = true;
						strcpy (TheirName, ReceivePacket.Name);
						TheirColor = ReceivePacket.Color;
						TheirHouse = ReceivePacket.House;
						transmit = 1;

						parms_received = 1;
if (display != REDRAW_ALL) {
	display = REDRAW_MESSAGE;
}
//						display = REDRAW_MESSAGE;

						/*...............................................................
						Check the version number of the other system.
						...............................................................*/
#ifdef PATCH
						if (IsV107) {
							version = 1;
						} else {
							version = 2;
						}
#else
						version = Version_Number();
#endif
						if (ReceivePacket.Version > version) {
							CCMessageBox().Process (TXT_YOURGAME_OUTDATED);

							// to skip the other system not responding msg
							lastmsgtime = TickCount.Time();

							process = false;
							rc = false;
						} else {
							if (ReceivePacket.Version < version) {
								CCMessageBox().Process (TXT_DESTGAME_OUTDATED);

								// to skip the other system not responding msg
								lastmsgtime = TickCount.Time();

								process = false;
								rc = false;
							}
						}
						break;

					/*..................................................................
					Incoming message: add to our list
					..................................................................*/
					case (SERIAL_MESSAGE):
// Smart_Printf( "received serial message\n" );
						oppscorescreen = false;
						sprintf(txt, Text_String (TXT_FROM), ReceivePacket.Name,
							ReceivePacket.Message);
						magic_number = *((unsigned short*)(ReceivePacket.Message + COMPAT_MESSAGE_LENGTH-4));
						crc = *((unsigned short*)(ReceivePacket.Message + COMPAT_MESSAGE_LENGTH-2));
						Messages.Add_Message (txt,
							MPlayerTColors[MPlayerID_To_ColorIndex(ReceivePacket.ID)],
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 1200, magic_number, crc);
if (display != REDRAW_ALL) {
	display = REDRAW_MESSAGE;
}
//						display = REDRAW_MESSAGE;
						break;

					//
					// get their response time
					//
					case (SERIAL_TIMING):
// Smart_Printf( "received timing\n" );
						oppscorescreen = false;
						theirresponsetime = ReceivePacket.ResponseTime;

						if ( !gameoptions ) {

							// retransmit of game options packet again
							transmit = 1;
						}
						break;

					//
					// print msg waiting for opponent
					//
					case (SERIAL_SCORE_SCREEN):
// Smart_Printf( "received score screen\n" );
						oppscorescreen = true;
if (display != REDRAW_ALL) {
	display = REDRAW_MESSAGE;
}
//						display = REDRAW_MESSAGE;
						parms_received = 1;
						break;

					default:
// Smart_Printf( "received unknown command %X\n", ReceivePacket.Command );
						break;
				}
			}
		}

		// if we haven't received a msg for 10 seconds exit

		if ( (TickCount.Time() - lastmsgtime) > msg_timeout) {
			CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);
			process = false;
			rc = false;

			// say we did receive sign off to keep from sending one
			recsignedoff = true;
		}

		/*---------------------------------------------------------------------
		Service the connection
		---------------------------------------------------------------------*/
		NullModem.Service();

		/*
		** If user has clicked 'GO' and the timeout has elapsed then quit the loop
		*/
		if ( ready_to_go && ready_time.Time() == 0 ){
			rc = 1;
			process = false;
		}

	}	/* end of while */

	/*------------------------------------------------------------------------
	Sort player ID's, so we can execute commands in the same order on both
	machines.
	------------------------------------------------------------------------*/
	if (rc) {
		/*.....................................................................
		Set the number of players in this game, and my ID
		.....................................................................*/
		MPlayerCount = 2;
		MPlayerLocalID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);

		TheirID = Build_MPlayerID (TheirColor,TheirHouse);

		/*.....................................................................
		Store every player's ID in the MPlayerID[] array.  This array will
		determine the order of event execution, so the ID's must be stored
		in the same order on all systems.
		.....................................................................*/
		if (TheirID < MPlayerLocalID) {
			MPlayerID[0] = TheirID;
			MPlayerID[1] = MPlayerLocalID;
			strcpy (MPlayerNames[0], TheirName);
			strcpy (MPlayerNames[1], MPlayerName);
		} else {
			MPlayerID[0] = MPlayerLocalID;
			MPlayerID[1] = TheirID;
			strcpy (MPlayerNames[0], MPlayerName);
			strcpy (MPlayerNames[1], TheirName);
		}

		/*.....................................................................
		Get the scenario filename
		.....................................................................*/
		Scenario = MPlayerFilenum[ScenarioIdx];

		/*.....................................................................
		Send all players the GO packet.
		.....................................................................*/
		SendPacket.Command = SERIAL_GO;
		SendPacket.ResponseTime = NullModem.Response_Time();
		if ( theirresponsetime == 10000 ) {
//			Mono_Clear_Screen();
//			Smart_Printf( "Did not receive their response time!!!!!!!\n" );
//			Get_Key();
		} else {
			if (SendPacket.ResponseTime < theirresponsetime) {
				SendPacket.ResponseTime = theirresponsetime;
			}
		}

		//
		// calculated one way delay for a packet and overall delay to execute
		// a packet
		//
		MPlayerMaxAhead = MAX( (SendPacket.ResponseTime / 8), 2);
char flip[128];
sprintf (flip, "C&C95 - MaxAhead set to %d frames\n", MPlayerMaxAhead);
CCDebugString (flip);

		SendPacket.ID = ModemGameToPlay;

		NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

		starttime = TickCount.Time();
		while ( ( NullModem.Num_Send()
			&& ((TickCount.Time() - starttime) < PACKET_SENDING_TIMEOUT) )){
			#if(SHOW_MONO)
			NullModem.Mono_Debug_Print(0);
			#endif

			NullModem.Service();
			Keyboard::Check();		//Make sure the message loop gets called
		}

		// clear queue to keep from doing any resends
		NullModem.Init_Send_Queue();

	} else {
		if ( !recsignedoff ) {
			/*.....................................................................
			Broadcast my sign-off over my network
			.....................................................................*/
			SendPacket.Command = SERIAL_SIGN_OFF;
			SendPacket.Color = MPlayerLocalID;		// use Color for ID
			SendPacket.ID = ModemGameToPlay;
			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			starttime = TickCount.Time();
			while ( (NullModem.Num_Send()
				&& ((TickCount.Time() - starttime) < PACKET_CANCEL_TIMEOUT) )){
				#if(SHOW_MONO)
				NullModem.Mono_Debug_Print(0);
				#endif

				if ( NullModem.Get_Message( &ReceivePacket, &packetlen ) > 0) {

					// are we getting our own packets back??

					if (ReceivePacket.Command == SERIAL_SIGN_OFF
						&& ReceivePacket.ID == ModemGameToPlay) {

						// exit while
						break;
					}
				}

				NullModem.Service();
			}
		}

		Shutdown_Modem();
	}

	/*------------------------------------------------------------------------
	Clear all lists
	------------------------------------------------------------------------*/
	while (scenariolist.Count()) {
		scenariolist.Remove_Item(scenariolist.Get_Item(0));
	}

	/*------------------------------------------------------------------------
	Restore screen
	------------------------------------------------------------------------*/
	Hide_Mouse();
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Show_Mouse();

	/*------------------------------------------------------------------------
	Save any changes made to our options
	------------------------------------------------------------------------*/
	if (changed) {
		Write_MultiPlayer_Settings ();
	}

	return(rc);

}	/* end of Com_Scenario_Dialog */


/***********************************************************************************************
 * Com_Show_Scenario_Dialog -- Serial game scenario selection dialog									  *
 *                                                                         						  *
 *                                                                         						  *
 *    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                      	  *
 *    ³                        Serial Game                         ³                       	  *
 *    ³                                                            ³                      	  *
 *    ³                   Your Name: __________                    ³     							  *
 *    ³                       House: [GDI] [NOD]                   ³									  *
 *    ³               Desired Color: [ ][ ][ ][ ]                  ³ 	   						  *
 *    ³                                                            ³                    	 	  *
 *    ³                     Opponent: Name                         ³                    	 	  *
 *    ³                     Scenario: Description                  ³   								  *
 *    ³                      Credits: xxxx                         ³   								  *
 *    ³                        Bases: ON                           ³                   	 	  *
 *    ³                       Crates: ON                           ³                   	 	  *
 *    ³                     Tiberium: ON                           ³                   	 	  *
 *    ³                       Ghosts: ON                           ³                   	 	  *
 *    ³                                                            ³                   	 	  *
 *    ³                         [Cancel]                           ³                    	 	  *
 *    ³                                                            ³                   	 	  *
 *    ³   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿   ³                   	 	  *
 *    ³   ³                                                    ³   ³                   	 	  *
 *    ³   ³                                                    ³   ³                   	 	  *
 *    ³   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ   ³                   	 	  *
 *    ³                       [Send Message]                       ³                   	 	  *
 *    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                      	  *
 *                                                                         						  *
 * INPUT:                                                                  						  *
 *		none.																												  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		true = success, false = cancel																			  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		MPlayerName & MPlayerGameName must contain this player's name.									  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.                                              						  *
 *=============================================================================================*/
int Com_Show_Scenario_Dialog(void)
{
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	int d_dialog_w = 306*factor;											// dialog width
	int d_dialog_h = 187*factor;											// dialog height
	int d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*factor - d_dialog_h) / 2);				// dialog y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 6*factor+1;												// ht of 6-pt text
	int d_margin1 = 5*factor;												// margin width/height
	int d_margin2 = 2*factor;												// margin width/height

	int d_name_w = 70*factor;
	int d_name_h = 9*factor;
	int d_name_x = d_dialog_cx;
	int d_name_y = d_dialog_y + d_margin1 + d_txt6_h + d_txt6_h;

	int d_gdi_w = 30*factor;
	int d_gdi_h = 9*factor;
	int d_gdi_x = d_dialog_cx;
	int d_gdi_y = d_name_y + d_name_h + d_margin2;

	int d_nod_w = 30*factor;
	int d_nod_h = 9*factor;
	int d_nod_x = d_gdi_x + d_gdi_w + d_margin2;
	int d_nod_y = d_gdi_y;

	int d_color_w = 10*factor;
	int d_color_h = 9*factor;
	int d_color_y = d_gdi_y + d_gdi_h + d_margin2;

	int d_opponent_y = d_color_y + d_color_h + d_margin1;
	int d_scenario_y = d_opponent_y + d_txt6_h;
	int d_credits_y = d_scenario_y + d_txt6_h;
	int d_count_y = d_credits_y + d_txt6_h;
	int d_level_y = d_count_y + d_txt6_h;
	int d_bases_y = d_level_y + d_txt6_h;
	int d_goodies_y = d_bases_y + d_txt6_h;
	int d_tiberium_y = d_goodies_y + d_txt6_h;
	int d_ghosts_y = d_tiberium_y + d_txt6_h;

	int d_cancel_w = 45*factor;
	int d_cancel_h = 9*factor;
	int d_cancel_x = d_dialog_cx - (d_cancel_w / 2);
	int d_cancel_y = d_ghosts_y + d_txt6_h + d_margin1;

	int d_message_w = d_dialog_w - (d_margin1 * 2);
	int d_message_h = 34*factor;
	int d_message_x = d_dialog_x + d_margin1;
	int d_message_y = d_cancel_y + d_cancel_h + d_margin1;

	int d_send_w = 80*factor;
	int d_send_h = 9*factor;
	int d_send_x = d_dialog_cx - (d_send_w / 2);
	int d_send_y = d_message_y + d_message_h + d_margin2;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_NAME = 100,
		BUTTON_GDI,
		BUTTON_NOD,
		BUTTON_CANCEL,
		BUTTON_SEND,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_MESSAGE,
		REDRAW_COLORS,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display = REDRAW_ALL;		// redraw level
	BOOL process = true;						// process while true
	KeyNumType input;

	char namebuf[MPLAYER_NAME_MAX] = {0};		// buffer for player's name
	int transmit;										// 1 = re-transmit new game options
	int first;											// 1 = no packets received yet
	int cbox_x[] = {	d_dialog_cx,
							d_dialog_cx + d_color_w,
							d_dialog_cx + (d_color_w * 2),
							d_dialog_cx + (d_color_w * 3),
							d_dialog_cx + (d_color_w * 4),
							d_dialog_cx + (d_color_w * 5)};
	int parms_received = 0;							// 1 = game options received
	int changed = 0;							// 1 = user has changed an option

	int rc;
	int recsignedoff = 0;
	int i;
	int version;
	char txt[80];
	unsigned long starttime;
	unsigned long timingtime;
	unsigned long lastmsgtime;
	unsigned long lastredrawtime;
	unsigned long transmittime = 0;
	int packetlen;
	bool oppscorescreen = false;
	bool gameoptions = false;
	EventClass *event;					// event ptr
	unsigned long msg_timeout = 1200;	// init to 20 seconds

	int					message_length;
	int					sent_so_far;
	unsigned short		magic_number;
	unsigned short		crc;
	bool					ready_to_go = false;

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	EditClass name_edt (BUTTON_NAME,
		namebuf, MPLAYER_NAME_MAX,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_name_x, d_name_y, d_name_w, d_name_h, EditClass::ALPHANUMERIC);

	TextButtonClass gdibtn(BUTTON_GDI, TXT_G_D_I,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_gdi_x, d_gdi_y, d_gdi_w, d_gdi_h);

	TextButtonClass nodbtn(BUTTON_NOD, TXT_N_O_D,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_nod_x, d_nod_y, d_nod_w, d_nod_h);

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_cancel_x, d_cancel_y);
//#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
//#endif

	TextButtonClass sendbtn(BUTTON_SEND, TXT_SEND_MESSAGE,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_send_x, d_send_y);
//#else
		d_send_x, d_send_y, d_send_w, d_send_h);
//#endif

	/*
	------------------------- Build the button list --------------------------
	*/
	commands = &name_edt;
	gdibtn.Add_Tail(*commands);
	nodbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);
	sendbtn.Add_Tail(*commands);

	/*
	----------------------------- Various Inits ------------------------------
	*/
	/*........................................................................
	Init player name & house
	........................................................................*/
	MPlayerColorIdx = MPlayerPrefColor;			// init my preferred color
	strcpy (namebuf, MPlayerName);				// set my name
	name_edt.Set_Text(namebuf,MPLAYER_NAME_MAX);
	name_edt.Set_Color(MPlayerTColors[MPlayerColorIdx]);

	if (MPlayerHouse==HOUSE_GOOD) {
		gdibtn.Turn_On();
	} else {
		nodbtn.Turn_On();
	}

	Fancy_Text_Print("", 0, 0, CC_GREEN, TBLACK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

	transmit = 1;
	first = 1;

	/*........................................................................
	Init the message display system
	........................................................................*/
	Messages.Init (d_message_x + 2*factor, d_message_y + 2*factor, 4, MAX_MESSAGE_LENGTH, d_txt6_h);

	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Set_Palette(Palette);

	extern char ModemRXString[];

	if (strlen(ModemRXString) > 36)
		ModemRXString[36] = 0;

	if (strlen(ModemRXString) > 0)
		Messages.Add_Message (ModemRXString, CC_TAN,
			TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 1200, 0, 0);

	ModemRXString[0] = '\0';

	/*
	---------------------------- Init Mono Output ----------------------------
	*/
	#if(SHOW_MONO)
	NullModem.Configure_Debug(sizeof (CommHeaderType),sizeof (SerialCommandType),
		SerialPacketNames, 106);
	NullModem.Mono_Debug_Print(1);
	#endif

	/*
	---------------------------- Processing loop -----------------------------
	*/
	NullModem.Reset_Response_Time();		// clear response time
	timingtime = lastmsgtime = lastredrawtime = TickCount.Time();
	while (Get_Mouse_State() > 0) Show_Mouse();

	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		#if(SHOW_MONO)
		NullModem.Mono_Debug_Print(0);
		#endif

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
#ifdef FORCE_WINSOCK
				if (Winsock.Get_Connected()){
					Draw_Caption (TXT_JOIN_INTERNET_GAME, d_dialog_x, d_dialog_y, d_dialog_w);
				}else{
					Draw_Caption (TXT_JOIN_SERIAL_GAME, d_dialog_x, d_dialog_y, d_dialog_w);
				}
#else
				Draw_Caption (TXT_JOIN_SERIAL_GAME, d_dialog_x, d_dialog_y, d_dialog_w);
#endif	//FORCE_WINSOCK

				Fancy_Text_Print(TXT_YOUR_NAME,
					d_name_x - 5*factor, d_name_y + 1*factor,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print(TXT_SIDE_COLON,
					d_gdi_x - 5*factor, d_gdi_y + 1*factor,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print(TXT_COLOR_COLON,
					cbox_x[0] - 5*factor, d_color_y + 1*factor,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

			}

			/*..................................................................
			Draw the color boxes
			..................................................................*/
			if (display >= REDRAW_COLORS) {
				for (i = 0; i < MAX_MPLAYER_COLORS; i++) {
					LogicPage->Fill_Rect (cbox_x[i] + 1*factor, d_color_y + 1*factor,
						cbox_x[i] + 1*factor + d_color_w - 2*factor, d_color_y + 1*factor + d_color_h - 2*factor,
						MPlayerGColors[i]);

					if (i == MPlayerColorIdx) {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h,
							BOXSTYLE_GREEN_DOWN, false);
					} else {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h,
							BOXSTYLE_GREEN_RAISED, false);
					}
				}
			}

			/*..................................................................
			Draw the message:
			- Erase an old message first
			..................................................................*/
			if (display >= REDRAW_MESSAGE) {
				Draw_Box(d_message_x, d_message_y, d_message_w, d_message_h,
					BOXSTYLE_GREEN_BORDER, true);
				Messages.Draw();

				LogicPage->Fill_Rect( d_dialog_x + 2*factor,
					d_opponent_y,
					d_dialog_x + d_dialog_w - 4*factor,
					d_ghosts_y + d_txt6_h,
					BLACK);

				if (parms_received) {
					if (oppscorescreen) {
						sprintf(txt,"%s",Text_String(TXT_WAITING_FOR_OPPONENT));

						int txtwidth = String_Pixel_Width( txt );

						Fancy_Text_Print (txt, d_dialog_cx - (txtwidth / 2),
							d_opponent_y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
					} else {
						/*............................................................
						Opponent's name
						............................................................*/
						Fancy_Text_Print (TXT_OPPONENT_COLON, d_dialog_cx - 3*factor,
							d_opponent_y, CC_GREEN, TBLACK,
							TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						if (TheirHouse == HOUSE_GOOD) {
							sprintf(txt,"%s %s",TheirName,Text_String(TXT_G_D_I));
						} else {
							sprintf(txt,"%s %s",TheirName,Text_String(TXT_N_O_D));
						}

						Fancy_Text_Print (txt, d_dialog_cx,
							d_opponent_y, MPlayerTColors[TheirColor], TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						/*............................................................
						Scenario description
						............................................................*/
						Fancy_Text_Print (TXT_SCENARIO_COLON, d_dialog_cx - 3*factor,
							d_scenario_y, CC_GREEN, TBLACK,
							TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						if (ScenarioIdx != -1) {
							sprintf(txt,"%s",	MPlayerScenarios[ScenarioIdx]);

							Fancy_Text_Print (txt, d_dialog_cx,
								d_scenario_y, CC_GREEN, TBLACK,
								TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
						} else {
							strcpy(txt,Text_String(TXT_NOT_FOUND));

							Fancy_Text_Print (txt, d_dialog_cx,
								d_scenario_y, RED, TBLACK,
								TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
						}

						/*............................................................
						Credits
						............................................................*/
						Fancy_Text_Print (TXT_START_CREDITS_COLON, d_dialog_cx - 3*factor,
							d_credits_y, CC_GREEN, TBLACK,
							TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						sprintf(txt,"%d",MPlayerCredits);
						Fancy_Text_Print (txt, d_dialog_cx,
							d_credits_y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						/*............................................................
						Count
						............................................................*/

						Fancy_Text_Print (TXT_COUNT, d_dialog_cx - 3*factor,
							d_count_y, CC_GREEN, TBLACK,
							TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						sprintf( txt, "%d ", MPlayerUnitCount );
						Fancy_Text_Print (txt, d_dialog_cx,
							d_count_y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						/*............................................................
						Level
						............................................................*/

						Fancy_Text_Print (TXT_LEVEL, d_dialog_cx - 3*factor,
							d_level_y, CC_GREEN, TBLACK,
							TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						if (BuildLevel <= MPLAYER_BUILD_LEVEL_MAX) {
							sprintf(txt, "%d ", BuildLevel);
						} else {
							sprintf(txt, "**");
						}
						Fancy_Text_Print (txt, d_dialog_cx,
							d_level_y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						/*............................................................
						Bases status
						............................................................*/
						Fancy_Text_Print (TXT_BASES_COLON, d_dialog_cx - 3*factor,
							d_bases_y, CC_GREEN, TBLACK,
							TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						if (MPlayerBases) {
							strcpy(txt,Text_String(TXT_ON));
						} else {
							strcpy(txt,Text_String(TXT_OFF));
						}
						Fancy_Text_Print (txt, d_dialog_cx,
							d_bases_y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						/*............................................................
						Tiberium status
						............................................................*/
						Fancy_Text_Print (TXT_TIBERIUM_COLON, d_dialog_cx - 3*factor,
							d_tiberium_y, CC_GREEN, TBLACK,
							TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						if (MPlayerTiberium) {
							strcpy(txt,Text_String(TXT_ON));
						} else {
							strcpy(txt,Text_String(TXT_OFF));
						}
						Fancy_Text_Print (txt, d_dialog_cx,
							d_tiberium_y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						/*............................................................
						Goodies status
						............................................................*/
						Fancy_Text_Print (TXT_CRATES_COLON, d_dialog_cx - 3*factor,
							d_goodies_y, CC_GREEN, TBLACK,
							TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						if (MPlayerGoodies) {
							strcpy(txt,Text_String(TXT_ON));
						} else {
							strcpy(txt,Text_String(TXT_OFF));
						}
						Fancy_Text_Print (txt, d_dialog_cx,
							d_goodies_y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

						/*............................................................
						Capture the flag or AI player ON/OFF
						............................................................*/
						if ( Special.IsCaptureTheFlag ) {
							strcpy( txt, Text_String( TXT_CAPTURE_THE_FLAG ) );
							strcat( txt, ":" );
							Fancy_Text_Print (txt, d_dialog_cx - 3*factor,
								d_ghosts_y, CC_GREEN, TBLACK,
								TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

							strcpy(txt,Text_String(TXT_ON));
							Fancy_Text_Print (txt, d_dialog_cx,
								d_ghosts_y, CC_GREEN, TBLACK,
								TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
						} else {
							/*............................................................
							Ghost player status
							............................................................*/
							Fancy_Text_Print (TXT_AI_PLAYERS_COLON, d_dialog_cx - 3*factor,
								d_ghosts_y, CC_GREEN, TBLACK,
								TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

							if (MPlayerGhosts) {
								strcpy(txt,Text_String(TXT_ON));
							} else {
								strcpy(txt,Text_String(TXT_OFF));
							}
							Fancy_Text_Print (txt, d_dialog_cx,
								d_ghosts_y, CC_GREEN, TBLACK,
								TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
						}
					}
				}
			}

			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Flag_List_To_Redraw();
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {
			/*------------------------------------------------------------------
			User clicks on a color button
			------------------------------------------------------------------*/
			case KN_LMOUSE:
				if (_Kbd->MouseQX > cbox_x[0] &&
					_Kbd->MouseQX < (cbox_x[MAX_MPLAYER_COLORS - 1] + d_color_w) &&
					_Kbd->MouseQY > d_color_y &&
					_Kbd->MouseQY < (d_color_y + d_color_h)) {
						if (!ready_to_go){
							/*.........................................................
							Compute my preferred color as the one I clicked on.
							.........................................................*/
							MPlayerPrefColor = (_Kbd->MouseQX - cbox_x[0]) / d_color_w;
							changed = 1;
							/*.........................................................
							If 'TheirColor' is set to the other player's color, make
							sure we can't pick that color.
							.........................................................*/
							if (parms_received) {
								if (MPlayerPrefColor == TheirColor)
									break;
							}
							MPlayerColorIdx = MPlayerPrefColor;

							name_edt.Set_Color(MPlayerTColors[MPlayerColorIdx]);
							name_edt.Flag_To_Redraw();
							display = REDRAW_COLORS;
							strcpy (MPlayerName, namebuf);
							transmit = 1;
						}
				}
				break;

			/*------------------------------------------------------------------
			House Buttons: set the player's desired House
			------------------------------------------------------------------*/
			case (BUTTON_GDI | KN_BUTTON):
				if (!ready_to_go){
					MPlayerHouse = HOUSE_GOOD;
					gdibtn.Turn_On();
					nodbtn.Turn_Off();
					strcpy (MPlayerName, namebuf);
					transmit = 1;
				}
				break;

			case (BUTTON_NOD | KN_BUTTON):
				if (!ready_to_go){
					MPlayerHouse = HOUSE_BAD;
					gdibtn.Turn_Off();
					nodbtn.Turn_On();
					strcpy (MPlayerName, namebuf);
					transmit = 1;
				}
				break;

			/*------------------------------------------------------------------
			User edits the name value; retransmit
			------------------------------------------------------------------*/
			case (BUTTON_NAME | KN_BUTTON):
				if (!ready_to_go){
					strcpy (MPlayerName, namebuf);
					transmit = 1;
					changed = 1;
				}
				break;

			/*------------------------------------------------------------------
			CANCEL: send a SIGN_OFF, bail out with error code
			------------------------------------------------------------------*/
			case (KN_ESC):
				if (!ready_to_go){
					if (Messages.Get_Edit_Buf() != NULL) {
						Messages.Input(input);
						display = REDRAW_MESSAGE;
						break;
					}
				}
			case (BUTTON_CANCEL | KN_BUTTON):
				if (!ready_to_go){
					process = false;
					rc = false;
				}
				break;

			/*------------------------------------------------------------------
			Default: manage the inter-player messages
			------------------------------------------------------------------*/
			default:
				if (!ready_to_go){

					/*...............................................................
					F4/SEND/'M' = send a message
					...............................................................*/
					if (Messages.Get_Edit_Buf()==NULL) {
						if (input == KN_M || input==(BUTTON_SEND | KN_BUTTON) ||
							input == KN_F4) {
							memset (txt, 0, 80);

							strcpy(txt,Text_String(TXT_MESSAGE));	// "Message:"

							Messages.Add_Edit (MPlayerTColors[MPlayerColorIdx],
								TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, txt, d_message_w-70*factor);
							display = REDRAW_MESSAGE;

							name_edt.Clear_Focus();
							name_edt.Flag_To_Redraw();

							break;
						}
					} else {
						if ( input == (BUTTON_SEND | KN_BUTTON) ) {
							input = KN_RETURN;
						}
					}

					/*...............................................................
					Manage the message system (get rid of old messages)
					...............................................................*/
					if (Messages.Manage()) {
						display = REDRAW_MESSAGE;
					}

					/*...............................................................
					Re-draw the messages & service keyboard input for any message
					being edited.
					...............................................................*/
					i = Messages.Input(input);

					/*...............................................................
					If 'Input' returned 1, it means refresh the message display.
					...............................................................*/
					if (i==1) {
						Messages.Draw();
					} else {

						/*...............................................................
						If 'Input' returned 2, it means redraw the message display.
						...............................................................*/
						if (i==2) {
							display = REDRAW_MESSAGE;
						} else {

							/*...............................................................
							If 'input' returned 3, it means send the current message.
							...............................................................*/
							if (i==3) {
								long actual_message_size;
								char *the_string;

								sent_so_far = 0;
								magic_number = MESSAGE_HEAD_MAGIC_NUMBER;
								message_length = strlen(Messages.Get_Edit_Buf());
								crc = (unsigned short)
										(Calculate_CRC(Messages.Get_Edit_Buf(), message_length) &0xffff);

								while (sent_so_far < message_length){
									SendPacket.Command = SERIAL_MESSAGE;
									strcpy (SendPacket.Name, MPlayerName);
									SendPacket.ID = Build_MPlayerID(MPlayerColorIdx, MPlayerHouse);
									memcpy (SendPacket.Message, Messages.Get_Edit_Buf()+sent_so_far, COMPAT_MESSAGE_LENGTH-5);

									/*
									** Steve I's stuff for splitting message on word boundries
									*/
									actual_message_size = COMPAT_MESSAGE_LENGTH - 5;

									/* Start at the end of the message and find a space with 10 chars. */
									the_string = GPacket.Message.Buf;
									while ( (COMPAT_MESSAGE_LENGTH -5) -actual_message_size < 10 &&
										the_string[actual_message_size] != ' '){
										--actual_message_size;
									}
									if ( the_string[actual_message_size] == ' ' ){

										/* Now delete the extra characters after the space (they musnt print) */
										for ( int i=0 ; i< (COMPAT_MESSAGE_LENGTH-5) - actual_message_size; i++ ){
											the_string[i + actual_message_size] = 0xff;
										}
									}else{
										actual_message_size = COMPAT_MESSAGE_LENGTH - 5;
									}

									*(SendPacket.Message + COMPAT_MESSAGE_LENGTH-5) = 0;
									*((unsigned short*)(SendPacket.Message + COMPAT_MESSAGE_LENGTH-4)) = magic_number;
									*((unsigned short*)(SendPacket.Message + COMPAT_MESSAGE_LENGTH-2)) = crc;

									/*..................................................................
									Send the message
									..................................................................*/
									NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);
									NullModem.Service();

									/*..................................................................
									Add the message to our own screen
									..................................................................*/
									sprintf(txt, Text_String (TXT_FROM), MPlayerName, SendPacket.Message);
									Messages.Add_Message (txt, MPlayerTColors[MPlayerColorIdx],
										TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 1200, magic_number, crc);

									magic_number++;
									sent_so_far += actual_message_size;	//COMPAT_MESSAGE_LENGTH-5;
								}
								display = REDRAW_MESSAGE;
							}
						}
					}
				}
				break;
		}

		/*---------------------------------------------------------------------
		Detect editing of the name buffer, transmit new values to players
		---------------------------------------------------------------------*/
		if (strcmp (namebuf, MPlayerName)) {
			strcpy (MPlayerName, namebuf);
			transmit = 1;
			changed = 1;
		}

		/*---------------------------------------------------------------------
		If our Transmit flag is set, we need to send out a game option packet
		---------------------------------------------------------------------*/
		if (transmit && (TickCount.Time() - transmittime) > PACKET_RETRANS_TIME) {
			SendPacket.Command = SERIAL_GAME_OPTIONS;
			strcpy (SendPacket.Name, MPlayerName);
#ifdef PATCH
			if (IsV107) {
				SendPacket.Version = 1;
			} else {
				SendPacket.Version = 2;
			}
#else
			SendPacket.Version = Version_Number();
#endif
			SendPacket.House = MPlayerHouse;
			SendPacket.Color = MPlayerColorIdx;
			SendPacket.ID = ModemGameToPlay;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			transmittime = TickCount.Time();
			transmit = 0;
		}

		//
		// send a timing packet if enough time has gone by.
		//
		if ( (TickCount.Time() - timingtime) > PACKET_TIMING_TIMEOUT) {
			SendPacket.Command = SERIAL_TIMING;
			SendPacket.ResponseTime = NullModem.Response_Time();
			SendPacket.ID = ModemGameToPlay;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 0);
			timingtime = TickCount.Time();
		}

		/*---------------------------------------------------------------------
		Check for an incoming message
		---------------------------------------------------------------------*/
		if (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {
// Smart_Printf( "received packet of length %d\n", packetlen );

			lastmsgtime = TickCount.Time();

			msg_timeout = 600;

			// are we getting our own packets back??

			if (ReceivePacket.Command >= SERIAL_CONNECT &&
				ReceivePacket.Command < SERIAL_LAST_COMMAND &&
				ReceivePacket.Command != SERIAL_MESSAGE &&
				ReceivePacket.ID == ModemGameToPlay) {

				CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);

				// to skip the other system not responding msg
				lastmsgtime = TickCount.Time();

				process = false;
				rc = false;

				// say we did receive sign off to keep from sending one
				recsignedoff = true;
				break;
			}

			event = (EventClass *)&ReceivePacket;
			if (event->Type <= EventClass::FRAMEINFO) {
				if ( (TickCount.Time() - lastredrawtime) > PACKET_REDRAW_TIME) {
					lastredrawtime = TickCount.Time();
					oppscorescreen = true;
					display = REDRAW_MESSAGE;
					parms_received = 1;
				}
			} else {
				switch ( ReceivePacket.Command ) {

					/*
					** Once the host is ready to go, we can no longer change game options.
					*/
					case SERIAL_READY_TO_GO:
						ready_to_go = true;
						break;

					/*..................................................................
					Other system signs off:  Give it time to receive my ACK, then show
					a message.
					..................................................................*/
					case (SERIAL_SIGN_OFF):
						starttime = TickCount.Time();
						while ( (TickCount.Time() - starttime) < 60)
							NullModem.Service();
						CCMessageBox().Process(TXT_USER_SIGNED_OFF);

						// to skip the other system not responding msg
						lastmsgtime = TickCount.Time();

						process = false;
						rc = false;
						recsignedoff = true;
						break;

					/*..................................................................
					Game Options: Store all options; check my color & game version.
					..................................................................*/
					case (SERIAL_GAME_OPTIONS):
						oppscorescreen = false;
						gameoptions = true;
						display = REDRAW_MESSAGE;
						parms_received = 1;

						strcpy (TheirName, ReceivePacket.Name);
						TheirColor = ReceivePacket.Color;
						TheirHouse = ReceivePacket.House;

						/*...............................................................
						Make sure I don't have the same color as the other guy.
						...............................................................*/
						if (MPlayerColorIdx == TheirColor) {

							// force transmitting of game options packet

							transmit = 1;
							transmittime = 0;

							MPlayerColorIdx = TheirColor + 1;
							if (MPlayerColorIdx >= 6)
								MPlayerColorIdx = 0;
							name_edt.Set_Color(MPlayerTColors[MPlayerColorIdx]);
							name_edt.Flag_To_Redraw();
							display = REDRAW_COLORS;
						}

						/*...............................................................
						Save scenario settings.
						...............................................................*/
						MPlayerCredits = ReceivePacket.Credits;
						MPlayerBases = ReceivePacket.IsBases;
						MPlayerTiberium = ReceivePacket.IsTiberium;
						MPlayerGoodies = ReceivePacket.IsGoodies;
						MPlayerGhosts = ReceivePacket.IsGhosties;
						BuildLevel = ReceivePacket.BuildLevel;
						MPlayerUnitCount = ReceivePacket.UnitCount;
						Seed = ReceivePacket.Seed;
						Special = ReceivePacket.Special;
						Options.GameSpeed = ReceivePacket.GameSpeed;

						if (MPlayerTiberium) {
							Special.IsTGrowth = 1;
							Special.IsTSpread = 1;
						} else {
							Special.IsTGrowth = 0;
							Special.IsTSpread = 0;
						}

						/*...............................................................
						Find the index of the scenario number; if it's not found, leave
						it at -1.
						...............................................................*/
						ScenarioIdx = -1;
						for (i = 0; i < MPlayerFilenum.Count(); i++) {
							if (ReceivePacket.Scenario == MPlayerFilenum[i])
								ScenarioIdx = i;
						}

						/*...............................................................
						Check our version numbers; if they're incompatible, sign off.
						...............................................................*/
#ifdef PATCH
						if (IsV107) {
							version = 1;
						} else {
							version = 2;
						}
#else
						version = Version_Number();
#endif
						if (ReceivePacket.Version > version) {
							CCMessageBox().Process (TXT_YOURGAME_OUTDATED);

							// to skip the other system not responding msg
							lastmsgtime = TickCount.Time();

							process = false;
							rc = false;
						} else {
							if (ReceivePacket.Version < version) {
								CCMessageBox().Process (TXT_DESTGAME_OUTDATED);

								// to skip the other system not responding msg
								lastmsgtime = TickCount.Time();

								process = false;
								rc = false;
							}
						}

						/*...............................................................
						If this is the first game-options packet we've received, transmit
						our options to him.
						...............................................................*/
						if (first) {
							first = 0;

							// force transmitting of game options packet

							transmit = 1;
							transmittime = 0;
						}
						break;

					/*..................................................................
					GO: Exit this routine with a success code.
					..................................................................*/
					case (SERIAL_GO):
						//
						// calculated one way delay for a packet and overall delay
						// to execute a packet
						//
						MPlayerMaxAhead = MAX( (ReceivePacket.ResponseTime / 8), 2);
char flip[128];
sprintf (flip, "C&C95 - MaxAhead set to %d frames\n", MPlayerMaxAhead);
CCDebugString (flip);

						process = false;
						rc = true;
						break;

					/*..................................................................
					Incoming message: add to our list
					..................................................................*/
					case (SERIAL_MESSAGE):
						oppscorescreen = false;
						sprintf(txt, Text_String (TXT_FROM), ReceivePacket.Name,
							ReceivePacket.Message);
						magic_number = *((unsigned short*)(ReceivePacket.Message + COMPAT_MESSAGE_LENGTH-4));
						crc = *((unsigned short*)(ReceivePacket.Message + COMPAT_MESSAGE_LENGTH-2));
						Messages.Add_Message (txt,	MPlayerTColors[MPlayerID_To_ColorIndex(ReceivePacket.ID)],
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 1200, magic_number, crc);
						display = REDRAW_MESSAGE;
						break;

					//
					// throw away timing packet
					//
					case (SERIAL_TIMING):
						oppscorescreen = false;
						break;

					//
					// print msg waiting for opponent
					//
					case (SERIAL_SCORE_SCREEN):
// Smart_Printf( "received score screen\n" );
						oppscorescreen = true;
						display = REDRAW_MESSAGE;
						parms_received = 1;
						break;

					default:
						break;
				}
			}
		}

		// if we haven't received a msg for 10 seconds exit

		//if ( ((TickCount.Time() - lastmsgtime) > msg_timeout) ||
		//(Winsock.Get_Connected() && Winsock.Get_Connection_Status == TcpipManagerClass::CONNECTION_LOST)) {

		if ( (TickCount.Time() - lastmsgtime) > msg_timeout) {
			CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);
			process = false;
			rc = false;

			// say we did receive sign off to keep from sending one
			recsignedoff = true;
		}




		/*---------------------------------------------------------------------
		Service the connection
		---------------------------------------------------------------------*/
		NullModem.Service();

	}	/* end of while */

	/*------------------------------------------------------------------------
	Sort player ID's, so we can execute commands in the same order on both
	machines.
	------------------------------------------------------------------------*/
	if (rc) {
		/*.....................................................................
		Set the number of players in this game, and my ID
		.....................................................................*/
		MPlayerCount = 2;
		MPlayerLocalID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);

		TheirID = Build_MPlayerID (TheirColor,TheirHouse);

		/*.....................................................................
		Store every player's ID in the MPlayerID[] array.  This array will
		determine the order of event execution, so the ID's must be stored
		in the same order on all systems.
		.....................................................................*/
		if (TheirID < MPlayerLocalID) {
			MPlayerID[0] = TheirID;
			MPlayerID[1] = MPlayerLocalID;
			strcpy (MPlayerNames[0], TheirName);
			strcpy (MPlayerNames[1], MPlayerName);
		} else {
			MPlayerID[0] = MPlayerLocalID;
			MPlayerID[1] = TheirID;
			strcpy (MPlayerNames[0], MPlayerName);
			strcpy (MPlayerNames[1], TheirName);
		}

		/*.....................................................................
		Get the scenario filename
		.....................................................................*/
		Scenario = MPlayerFilenum[ScenarioIdx];

		starttime = TickCount.Time();
		while ( ( NullModem.Num_Send()
			&& ((TickCount.Time() - starttime) < PACKET_SENDING_TIMEOUT) )){
			#if(SHOW_MONO)
			NullModem.Mono_Debug_Print(0);
			#endif

			NullModem.Service();
			Keyboard::Check();		//Make sure the message loop gets called
		}

		// clear queue to keep from doing any resends
		NullModem.Init_Send_Queue();

	} else {
		if ( !recsignedoff ) {
			/*.....................................................................
			Broadcast my sign-off over my network
			.....................................................................*/
			SendPacket.Command = SERIAL_SIGN_OFF;
			SendPacket.Color = MPlayerLocalID;		// use Color for ID
			SendPacket.ID = ModemGameToPlay;
			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			starttime = TickCount.Time();
			while ( (NullModem.Num_Send()
				&& ((TickCount.Time() - starttime) < PACKET_CANCEL_TIMEOUT) )){
				#if(SHOW_MONO)
				NullModem.Mono_Debug_Print(0);
				#endif

				if ( NullModem.Get_Message( &ReceivePacket, &packetlen ) > 0) {

					// are we getting our own packets back??

					if (ReceivePacket.Command == SERIAL_SIGN_OFF
						&& ReceivePacket.ID == ModemGameToPlay) {

						// exit while
						break;
					}
				}

				NullModem.Service();
			}
		}

		Shutdown_Modem();
	}

	/*------------------------------------------------------------------------
	Restore screen
	------------------------------------------------------------------------*/
	Hide_Mouse();
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Show_Mouse();

	/*------------------------------------------------------------------------
	Save any changes made to our options
	------------------------------------------------------------------------*/
	if (changed) {
		Write_MultiPlayer_Settings ();
	}

	return(rc);

}	/* end of Com_Show_Scenario_Dialog */



/***************************************************************************
 * Phone_Dialog -- Lets user edit phone directory & dial                   *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		true = dial the current phone book entry, false = cancel.				*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Serial options must have been read from CC.INI.								*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Phone_Dialog (void)
{
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
		int d_dialog_w = 280 *factor;											// dialog width
		int d_dialog_h = 150 *factor;											// dialog height
		int d_dialog_x = ((320 *factor - d_dialog_w) / 2);				// dialog x-coord
		int d_dialog_y = ((200 *factor- d_dialog_h) / 2);				// dialog y-coord
		int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

		int d_txt6_h = 11 *factor;												// ht of 6-pt text
		int d_margin = 7 *factor;												// margin width/height

		int d_phonelist_w = 268 *factor;
		int d_phonelist_h = 87 *factor;
		int d_phonelist_x = d_dialog_cx - (d_phonelist_w / 2);
		int d_phonelist_y = d_dialog_y + d_margin + d_txt6_h + 11*factor;

		int d_add_w = 45*factor;
		int d_add_h = 9*factor;
		int d_add_x = d_dialog_cx - (d_add_w / 2) - d_margin - d_add_w;
		int d_add_y = d_phonelist_y + d_phonelist_h + d_margin;

		int d_edit_w = 45*factor;
		int d_edit_h = 9*factor;
		int d_edit_x = d_dialog_cx - (d_edit_w / 2);
		int d_edit_y = d_phonelist_y + d_phonelist_h + d_margin;

		int d_delete_w = 45*factor;
		int d_delete_h = 9*factor;
		int d_delete_x = d_dialog_cx + (d_delete_w / 2) + d_margin;
		int d_delete_y = d_phonelist_y + d_phonelist_h + d_margin;

		int d_numedit_w = ( (PhoneEntryClass::PHONE_MAX_NUM - 1) * 6*factor) + 3*factor;
		int d_numedit_h = 9*factor;
		int d_numedit_x = d_dialog_cx - (d_numedit_w / 2);
		int d_numedit_y = d_add_y + d_add_h + d_margin;

		int d_dial_w = 45*factor;
		int d_dial_h = 9*factor;
		int d_dial_x = d_dialog_cx - (d_numedit_w / 2) - d_margin - d_dial_w;
		int d_dial_y = d_add_y + d_add_h + d_margin;

		int d_cancel_w = 45*factor;
		int d_cancel_h = 9*factor;
		int d_cancel_x = d_dialog_cx + (d_numedit_w / 2) + d_margin;
		int d_cancel_y = d_add_y + d_add_h + d_margin;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_PHONELIST = 100,
		BUTTON_ADD,
		BUTTON_EDIT,
		BUTTON_DELETE,
		BUTTON_DIAL,
		BUTTON_CANCEL,
		BUTTON_NUMEDIT,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display = REDRAW_ALL;		// redraw level
	BOOL process = true;						// process while true
	KeyNumType input;

	char phone_num[ PhoneEntryClass::PHONE_MAX_NUM ] = { 0 }; // buffer for editing phone #
	int rc;
	int i;
	int tabs[] = {123*factor, 207*factor};				// tabs for list box
	char *item;									// for removing items from list box
	PhoneEntryClass *p_entry;				// for creating / editing phonebook entries
	int changed = 0;							// 1 = save changes to INI file
	int firsttime = 0;

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	void const *up_button;
	void const *down_button;

	if (InMainLoop){
		up_button = Hires_Retrieve("BTN-UP.SHP");
		down_button = Hires_Retrieve("BTN-DN.SHP");
	}else{
		up_button = Hires_Retrieve("BTN-UP2.SHP");
		down_button = Hires_Retrieve("BTN-DN2.SHP");
	}


	ListClass phonelist(BUTTON_PHONELIST,
		d_phonelist_x, d_phonelist_y, d_phonelist_w, d_phonelist_h,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		up_button,
		down_button);

	TextButtonClass addbtn(BUTTON_ADD, TXT_ADD,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#ifdef FRENCH
//		d_add_x-4, d_add_y);
//#else
		d_add_x, d_add_y, d_add_w, d_add_h);
//#endif

	TextButtonClass editbtn(BUTTON_EDIT, TXT_EDIT,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_edit_x, d_edit_y, d_edit_w, d_edit_h);

	TextButtonClass deletebtn(BUTTON_DELETE, TXT_DELETE_BUTTON,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#ifdef FRENCH
//		d_delete_x, d_delete_y);
//#else
		d_delete_x, d_delete_y, d_delete_w, d_delete_h);
//#endif

	TextButtonClass dialbtn(BUTTON_DIAL, TXT_DIAL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
/* ###Change collision detected! C:\PROJECTS\CODE\NULLDLG.CPP... */
		d_dial_x, d_dial_y, d_dial_w, d_dial_h);

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_cancel_x, d_cancel_y);
//#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
//#endif

	EditClass numedit (BUTTON_NUMEDIT,
		phone_num, PhoneEntryClass::PHONE_MAX_NUM,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_numedit_x, d_numedit_y, d_numedit_w, d_numedit_h, EditClass::ALPHANUMERIC);

	/*
	------------------------- Build the button list --------------------------
	*/
	commands = &phonelist;
	addbtn.Add_Tail(*commands);
	editbtn.Add_Tail(*commands);
	deletebtn.Add_Tail(*commands);
	dialbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);
	numedit.Add_Tail(*commands);
	dialbtn.Turn_On();

	/*
	----------------------------- Various Inits ------------------------------
	*/
	/*........................................................................
	Fill in the phone directory list box
	........................................................................*/
	phonelist.Set_Tabs(tabs);
	Build_Phone_Listbox(&phonelist, &numedit, phone_num);

	if (CurPhoneIdx == -1) {
		firsttime = 1;
	}

	/*
	---------------------------- Processing loop -----------------------------
	*/
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
				Set_Palette(Palette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				// init font variables

				Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Draw_Caption (TXT_PHONE_LIST, d_dialog_x, d_dialog_y, d_dialog_w);
			}
			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				phonelist.Flag_To_Redraw();
				addbtn.Flag_To_Redraw();
				editbtn.Flag_To_Redraw();
				deletebtn.Flag_To_Redraw();
				dialbtn.Flag_To_Redraw();
				cancelbtn.Flag_To_Redraw();
				numedit.Flag_To_Redraw();
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		if ( firsttime ) {
			numedit.Set_Focus();
			numedit.Flag_To_Redraw();
			input = commands->Input();
			firsttime = 0;
		}

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {
			/*------------------------------------------------------------------
			New phone listing selected.
			------------------------------------------------------------------*/
			case (BUTTON_PHONELIST | KN_BUTTON):
				/*...............................................................
				Detect a change in the selected item; update CurPhoneIdx, and
				the edit box buffer.
				...............................................................*/
				if (phonelist.Current_Index() != CurPhoneIdx) {
					CurPhoneIdx = phonelist.Current_Index();
					strcpy (phone_num, PhoneBook[CurPhoneIdx]->Number);
					numedit.Set_Text (phone_num, PhoneEntryClass::PHONE_MAX_NUM );
					changed = 1;
				}
				break;

			/*------------------------------------------------------------------
			Add a new entry
			------------------------------------------------------------------*/
			case (BUTTON_ADD | KN_BUTTON):

				/*...............................................................
				Allocate a new phone book entry
				...............................................................*/
				p_entry = new PhoneEntryClass();
				p_entry->Name[0] = 0;
				p_entry->Number[0] = 0;
				p_entry->Settings.Port = 0;
				p_entry->Settings.IRQ = -1;
				p_entry->Settings.Baud = -1;
				p_entry->Settings.DialMethod = DIAL_TOUCH_TONE;
				p_entry->Settings.InitStringIndex = 0;
				p_entry->Settings.CallWaitStringIndex = CALL_WAIT_CUSTOM;
				p_entry->Settings.CallWaitString[0] = 0;

				/*...............................................................
				Invoke the entry editor; if user clicks Save, add the new entry
				to the list, and rebuild the list box.
				...............................................................*/
				if ( Edit_Phone_Dialog( p_entry ) ) {
					PhoneBook.Add (p_entry);
					Build_Phone_Listbox( &phonelist, &numedit, phone_num );
					/*............................................................
					Set the current listbox index to the newly-added item.
					............................................................*/
					for (i = 0; i < PhoneBook.Count(); i++) {
						if (p_entry == PhoneBook[i]) {
							CurPhoneIdx = i;
							strcpy (phone_num, PhoneBook[CurPhoneIdx]->Number);
							numedit.Set_Text (phone_num, PhoneEntryClass::PHONE_MAX_NUM );
							phonelist.Set_Selected_Index( CurPhoneIdx );
						}
					}
					changed = 1;
				} else {

					/*...............................................................
					If the user clicked Cancel, delete the entry & keep looping.
					...............................................................*/
					delete p_entry;
				}
				display = REDRAW_ALL;
				break;

			/*------------------------------------------------------------------
			Edit the current entry
			------------------------------------------------------------------*/
			case (BUTTON_EDIT | KN_BUTTON):

				/*...............................................................
				Do nothing if no entry is selected.
				...............................................................*/
				if (CurPhoneIdx==-1)
					break;

				/*...............................................................
				Allocate a new entry & copy the currently-selected entry into it
				...............................................................*/
				p_entry = new PhoneEntryClass();
				(*p_entry) = (*PhoneBook[CurPhoneIdx]);

				/*...............................................................
				Pass the new entry to the entry editor; if the user selects OK,
				copy the data back into our phone book.  Rebuild the list so
				the changes show up in the list box.
				...............................................................*/
				if ( Edit_Phone_Dialog( p_entry ) ) {
					(*PhoneBook[CurPhoneIdx]) = (*p_entry);
					Build_Phone_Listbox(&phonelist, &numedit, phone_num);
					/*............................................................
					Set the current listbox index to the newly-added item.
					............................................................*/
					for (i = 0; i < PhoneBook.Count(); i++) {
						if (PhoneBook[CurPhoneIdx] == PhoneBook[i]) {
							CurPhoneIdx = i;
							strcpy (phone_num, PhoneBook[CurPhoneIdx]->Number);
							numedit.Set_Text (phone_num, PhoneEntryClass::PHONE_MAX_NUM );
							phonelist.Set_Selected_Index( CurPhoneIdx );
						}
					}
					changed = 1;
				}
				delete p_entry;
				display = REDRAW_ALL;
				break;

			/*------------------------------------------------------------------
			Delete the current entry
			------------------------------------------------------------------*/
			case (BUTTON_DELETE | KN_BUTTON):

				/*...............................................................
				Do nothing if no entry is selected.
				...............................................................*/
				if (CurPhoneIdx == -1)
					break;

				/*...............................................................
				Delete the current item & rebuild the phone listbox
				...............................................................*/
				PhoneBook.Delete (CurPhoneIdx);
				Build_Phone_Listbox(&phonelist, &numedit, phone_num);

				if (CurPhoneIdx == -1) {
					*phone_num = 0;
					numedit.Set_Text (phone_num, PhoneEntryClass::PHONE_MAX_NUM );
				}
				changed = 1;
				break;

			/*------------------------------------------------------------------
			Dial the current number
			------------------------------------------------------------------*/
			case (KN_RETURN):
				dialbtn.IsPressed = true;
				dialbtn.Draw_Me(true);
				// fall thru

			case (BUTTON_DIAL | KN_BUTTON):

				/*...............................................................
				If no item is selected, just dial the number in the phone #
				edit box:
				- Create a new phone entry
				- Copy the phone number into it
				- Set settings to defaults
				...............................................................*/
				if (CurPhoneIdx == -1 ||
					strcmp( PhoneBook[CurPhoneIdx]->Number, phone_num) ) {

					if ( strlen(phone_num) == 0) {	// do not dial
						dialbtn.IsPressed = false;
						dialbtn.Flag_To_Redraw();
						break;
					}

					p_entry = new PhoneEntryClass();
					strcpy( p_entry->Name, "NONAME" );
					strcpy( p_entry->Number, phone_num);
					p_entry->Settings.Port = 0;
					p_entry->Settings.IRQ = -1;
					p_entry->Settings.Baud = -1;
					p_entry->Settings.DialMethod = DIAL_TOUCH_TONE;
					p_entry->Settings.InitStringIndex = 0;
					p_entry->Settings.CallWaitStringIndex = CALL_WAIT_CUSTOM;
					p_entry->Settings.CallWaitString[0] = 0;

					PhoneBook.Add (p_entry);
					Build_Phone_Listbox(&phonelist, &numedit, phone_num);
					/*............................................................
					Set the current listbox index to the newly-added item.
					............................................................*/
					for (i = 0; i < PhoneBook.Count(); i++) {
						if (p_entry == PhoneBook[i]) {
							CurPhoneIdx = i;
						}
					}
					changed = 1;
				}

				process = false;
				rc = true;
				break;

			/*------------------------------------------------------------------
			CANCEL: bail out
			------------------------------------------------------------------*/
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				process = false;
				rc = false;
				break;
		}

	}	/* end of while */

	/*------------------------------------------------------------------------
	Save any changes we've made to the phone list or settings
	------------------------------------------------------------------------*/
	if (changed) {
		Write_MultiPlayer_Settings ();
	}

	/*------------------------------------------------------------------------
	Clear the list box
	------------------------------------------------------------------------*/
	while (phonelist.Count()) {
		item = (char *)phonelist.Get_Item(0);
		phonelist.Remove_Item(item);
		delete [] item;
	}

	return(rc);

}	/* end of Phone_Dialog */


/***************************************************************************
 * Build_Phone_Listbox -- [re]builds the phone entry listbox               *
 *                                                                         *
 * This routine rebuilds the phone list box from scratch; it also updates	*
 * the contents of the phone # edit field.											*
 *                                                                         *
 * INPUT:                                                                  *
 *		list		ptr to list box															*
 *		edit		ptr to edit box															*
 *		buf		ptr to buffer for phone #												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Build_Phone_Listbox (ListClass *list, EditClass *edit, char *buf)
{
	int i;
	char *item;
	char phonename[21];
	char phonenum[15];

	/*........................................................................
	Clear the list
	........................................................................*/
	while (list->Count()) {
		item = (char *)(list->Get_Item(0));
		list->Remove_Item(item);
		delete [] item;
	}

	/*
	** Now sort the phone list by name then number
	*/
	qsort ((void *)(&PhoneBook[0]), PhoneBook.Count(), sizeof(class PhoneEntryClass *), Phone_Compare);

	/*........................................................................
	Build the list
	........................................................................*/
	for (i = 0; i < PhoneBook.Count(); i++) {
		item = new char[80];
		if ( !(strlen( PhoneBook[i]->Name )) ) {
			strcpy( phonename, " " );
		} else {
			strncpy( phonename, PhoneBook[i]->Name, 20 );
			phonename[21] = 0;
		}

		if ( !(strlen( PhoneBook[i]->Number )) ) {
			strcpy( phonenum, " " );
		} else {
			if ( strlen( PhoneBook[i]->Number ) < 14) {
				strcpy( phonenum, PhoneBook[i]->Number );
			} else {
				strncpy( phonenum, PhoneBook[i]->Number, 12 );
				phonenum[12] = 0;
				strcat( phonenum, "..." );
			}
		}

		if (PhoneBook[i]->Settings.Baud != -1) {
			sprintf(item,"%s\t%s\t%d", phonename, phonenum,
				PhoneBook[i]->Settings.Baud);
		} else {
			sprintf(item,"%s\t%s\t[%s]", phonename, phonenum,
				Text_String(TXT_DEFAULT));
		}
		list->Add_Item(item);
	}
	list->Flag_To_Redraw();

	/*........................................................................
	Init the current phone book index
	........................................................................*/
	if (list->Count() == 0 || CurPhoneIdx < -1) {
		CurPhoneIdx = -1;
	} else {
		if (CurPhoneIdx >= list->Count() ) {
			CurPhoneIdx = 0;
		}
	}

	/*........................................................................
	Fill in phone number edit buffer
	........................................................................*/
	if (CurPhoneIdx > -1) {
		strcpy (buf, PhoneBook[CurPhoneIdx]->Number);
		edit->Set_Text (buf, PhoneEntryClass::PHONE_MAX_NUM );
		list->Set_Selected_Index( CurPhoneIdx );
	}
}


/***************************************************************************
 * Phone_Compare -- for qsort																*
 *                                                                         *
 * INPUT:                                                                  *
 *		p1,p2		ptrs to elements to compare											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = same, -1 = (*p1) goes BEFORE (*p2), 1 = (*p1) goes AFTER (*p2)	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   02/14/1995 BR : Created.                                              *
 *=========================================================================*/
static int Phone_Compare (const void *p1, const void *p2)
{
	class PhoneEntryClass *pe1,*pe2;
	int result;

	pe1 = *((class PhoneEntryClass **)p1);
	pe2 = *((class PhoneEntryClass **)p2);

	result = strcmp( pe1->Name, pe2->Name );

	// if strings are equal then check the phone number

	if ( !result ) {
		result = strcmp( pe1->Number, pe2->Number );
	}

	return(result);
}


/***************************************************************************
 * Edit_Phone_Dialog -- lets user edit a phone book entry                  *
 *                                                                         *
 * INPUT:                                                                  *
 *		phone		entry to edit																*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		true = OK, false = cancel															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Edit_Phone_Dialog (PhoneEntryClass *phone)
{
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	int d_dialog_w = 230 *factor;											// dialog width
	int d_dialog_h = 105*factor;											// dialog height
	int d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
//		D_DIALOG_Y = ((200 - D_DIALOG_H) / 2);				// dialog y-coord
	int d_dialog_y = ((136*factor - d_dialog_h) / 2);				// dialog y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 11 *factor;												// ht of 6-pt text
	int d_margin = 7 *factor;												// margin width/height

	int d_name_w = ( (PhoneEntryClass::PHONE_MAX_NAME - 1) * 6) + 3 *factor;
	int d_name_h = 9 *factor;
	int d_name_x = d_dialog_x + (((d_dialog_w - d_name_w) * 3) / 4) - 5 *factor;
	int d_name_y = d_dialog_y + 25 *factor;

	int d_number_w = ( (PhoneEntryClass::PHONE_MAX_NUM - 1) * 6) + 3 *factor;
	int d_number_h = 9 *factor;
	int d_number_x = d_dialog_x + (((d_dialog_w - d_number_w) * 3) / 4) - 5 *factor;
	int d_number_y = d_name_y + d_name_h + d_margin;

#if (GERMAN | FRENCH)
	int d_default_w = 130 *factor;
#else
	int d_default_w = 104 *factor;
#endif
	int d_default_h = 9 *factor;
	int d_default_x = d_dialog_cx - (d_default_w / 2);
	int d_default_y = d_number_y + d_number_h + d_margin;

#if (GERMAN | FRENCH)
	int d_custom_w = 130 *factor;
#else
	int d_custom_w = 100 *factor;
#endif
	int d_custom_h = 9 *factor;
	int d_custom_x = d_dialog_cx - (d_default_w / 2);
	int d_custom_y = d_default_y + d_default_h + d_margin;

#if (GERMAN | FRENCH)
	int d_save_w = 55 *factor;
#else
	int d_save_w = 45 *factor;
#endif
	int d_save_h = 9 *factor;
	int d_save_x = d_dialog_cx - d_margin - d_save_w;
	int d_save_y = d_dialog_y + d_dialog_h - d_margin - d_save_h;

#if (GERMAN | FRENCH)
	int d_cancel_w = 55 *factor;
#else
	int d_cancel_w = 45 *factor;
#endif
	int d_cancel_h = 9 *factor;
	int d_cancel_x = d_dialog_cx + d_margin;
	int d_cancel_y = d_dialog_y + d_dialog_h - d_margin - d_cancel_h;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_NAME = 100,
		BUTTON_NUMBER,
		BUTTON_DEFAULT,
		BUTTON_CUSTOM,
		BUTTON_SAVE,
		BUTTON_CANCEL,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display = REDRAW_ALL;		// redraw level
	BOOL process = true;						// process while true
	KeyNumType input;

	char namebuf[PhoneEntryClass::PHONE_MAX_NAME] = { 0 };	// buffer for editing name
	char numbuf[PhoneEntryClass::PHONE_MAX_NUM] = { 0 };		// buffer for editing phone #
	int rc;
	SerialSettingsType settings;
	int custom = 0;
	int firsttime = 1;

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	EditClass nameedit (BUTTON_NAME,
		namebuf, PhoneEntryClass::PHONE_MAX_NAME,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_name_x, d_name_y, d_name_w, d_name_h, EditClass::ALPHANUMERIC);

	EditClass numedit (BUTTON_NUMBER,
		numbuf, PhoneEntryClass::PHONE_MAX_NUM,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_number_x, d_number_y, d_number_w, d_number_h, EditClass::ALPHANUMERIC);

	TextButtonClass defaultbtn(BUTTON_DEFAULT, TXT_DEFAULT_SETTINGS,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_default_x, d_default_y, d_default_w, d_default_h);

	TextButtonClass custombtn(BUTTON_CUSTOM, TXT_CUSTOM_SETTINGS,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_custom_x, d_custom_y, d_custom_w, d_custom_h);

	TextButtonClass savebtn(BUTTON_SAVE, TXT_SAVE_BUTTON,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_save_x, d_save_y, d_save_w, d_save_h);

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);

	/*
	------------------------- Build the button list --------------------------
	*/
	commands = &nameedit;
	numedit.Add_Tail(*commands);
	defaultbtn.Add_Tail(*commands);
	custombtn.Add_Tail(*commands);
	savebtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);

	/*
	----------------------------- Various Inits ------------------------------
	*/
	/*........................................................................
	Init the settings; if the phone entry is set to use defaults, init our
	settings to sensible values (in case we invoke the setting editor);
	otherwise, copy the entry's settings.
	........................................................................*/
	if (phone->Settings.Port == 0 || phone->Settings.IRQ == -1 ||
		phone->Settings.Baud == -1) {
		settings = SerialDefaults;
		defaultbtn.Turn_On();
		custom = false;
	} else {
		settings = phone->Settings;
		custombtn.Turn_On();
		custom = true;
	}

	strcpy (namebuf, phone->Name);
	nameedit.Set_Text (namebuf, PhoneEntryClass::PHONE_MAX_NAME);

	strcpy (numbuf, phone->Number);
	numedit.Set_Text (numbuf, PhoneEntryClass::PHONE_MAX_NUM);

	/*
	---------------------------- Processing loop -----------------------------
	*/
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
				Set_Palette(Palette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				// init font variables

				Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Draw_Caption (TXT_PHONE_LISTING, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print (TXT_NAME_COLON,
					d_name_x - 5, d_name_y + 1,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print (TXT_NUMBER_COLON,
					d_number_x - 5, d_number_y + 1,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}
			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Flag_List_To_Redraw();
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		if ( firsttime ) {
			nameedit.Set_Focus();
			nameedit.Flag_To_Redraw();
			input = commands->Input();
			firsttime = 0;
		}

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {
			case (BUTTON_NAME | KN_BUTTON):
				numedit.Set_Focus();
				numedit.Flag_To_Redraw();
				break;

//			case (BUTTON_NUMBER | KN_BUTTON):
//				nameedit.Clear_Focus();
//				nameedit.Flag_To_Redraw();
//				break;

			/*------------------------------------------------------------------
			Use Default Serial Settings
			------------------------------------------------------------------*/
			case (BUTTON_DEFAULT | KN_BUTTON):
				custombtn.Turn_Off();
				defaultbtn.Turn_On();
				custom = 0;
				break;

			/*------------------------------------------------------------------
			Use Custom Serial Settings
			------------------------------------------------------------------*/
			case (BUTTON_CUSTOM | KN_BUTTON):
				if (Com_Settings_Dialog (&settings)) {
					custombtn.Turn_On();
					defaultbtn.Turn_Off();
				}
				custom = 1;
				display = REDRAW_ALL;
				break;

			/*------------------------------------------------------------------
			CANCEL: bail out
			------------------------------------------------------------------*/
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				process = false;
				rc = false;
				break;

			/*------------------------------------------------------------------
			Save: save changes
			------------------------------------------------------------------*/
			case (KN_RETURN):
			case (BUTTON_SAVE | KN_BUTTON):
				process = false;
				rc = true;
				break;
		}

	}	/* end of while */

	/*------------------------------------------------------------------------
	If 'Save', save all current settings
	------------------------------------------------------------------------*/
	if (rc) {
		strcpy( phone->Name, strupr( namebuf ) );

		// if nothing was entered then make if NONAME

		if ( !(phone->Name[0]) ) {
			strcpy( phone->Name, "NONAME" );
		}

		strcpy( phone->Number, strupr( numbuf ) );

		if (custom) {
			phone->Settings = settings;
		} else {
			phone->Settings.Port = 0;
			phone->Settings.IRQ = -1;
			phone->Settings.Baud = -1;
			phone->Settings.DialMethod = DIAL_TOUCH_TONE;
			phone->Settings.InitStringIndex = 0;
			phone->Settings.CallWaitStringIndex = CALL_WAIT_CUSTOM;
			phone->Settings.CallWaitString[0] = 0;
		}
	}

	return(rc);


}	/* end of Edit_Phone_Dialog */


static bool Dial_Modem( SerialSettingsType *settings, bool reconnect )
{
	bool connected = false;
	DialStatusType dialstatus;
	int modemstatus;

	/*
	**	Turn modem servicing off in the callback routine.
	*/
	ModemService = false;

	// save for later to reconnect

	DialSettings = settings;

	modemstatus = NullModem.Get_Modem_Status();
	if (reconnect) {
		if ( (modemstatus & CD_SET) ) {
			connected = true;
			ModemService = true;
			return( connected );
		}
	} else {
		if ( (modemstatus & CD_SET) ) {
			NullModem.Hangup_Modem();
			ModemService = false;
		}
	}

	NullModem.Setup_Modem_Echo( Modem_Echo );

	modemstatus = NullModem.Detect_Modem( settings, reconnect );
	if ( !modemstatus ) {
		NullModem.Remove_Modem_Echo();
		NullModem.Print_EchoBuf();
		NullModem.Reset_EchoBuf();

		/*
		** If our first attempt to detect the modem failed, and we're at
		** 14400 or 28800, bump up to the next baud rate & try again.
		*/
		switch (settings->Baud) {

			case 14400:
				settings->Baud = 19200;
				Shutdown_Modem();
				Init_Null_Modem(settings);
				NullModem.Setup_Modem_Echo( Modem_Echo );
				modemstatus = NullModem.Detect_Modem( settings, reconnect );
				if ( !modemstatus ) {
					NullModem.Remove_Modem_Echo();
					NullModem.Print_EchoBuf();
					NullModem.Reset_EchoBuf();
					CCMessageBox().Process( TXT_UNABLE_FIND_MODEM );
					ModemService = true;
					return( connected );
				}
				break;

			case 28800:
				settings->Baud = 38400;
				Shutdown_Modem();
				Init_Null_Modem(settings);
				NullModem.Setup_Modem_Echo( Modem_Echo );
				modemstatus = NullModem.Detect_Modem( settings, reconnect );
				if ( !modemstatus ) {
					NullModem.Remove_Modem_Echo();
					NullModem.Print_EchoBuf();
					NullModem.Reset_EchoBuf();
					CCMessageBox().Process( TXT_UNABLE_FIND_MODEM );
					ModemService = true;
					return( connected );
				}
				break;

			default:
				CCMessageBox().Process( TXT_UNABLE_FIND_MODEM );
				ModemService = true;
				return( connected );

		}
	}
	else if ( modemstatus == -1 ) {
		NullModem.Remove_Modem_Echo();
		NullModem.Print_EchoBuf();
		NullModem.Reset_EchoBuf();
		CCMessageBox().Process( TXT_ERROR_IN_INITSTRING );
		ModemService = true;
		return( connected );
	}


	/*
	** Completely disable audio. This is required for MWave devices
	*/
	ThemeType old_theme = THEME_NONE;
	if (SoundOn){
		old_theme = Theme.What_Is_Playing();
		Theme.Stop();
		CountDownTimerClass wait;
		Call_Back();
		wait.Set (60,true);
		while ( wait.Time() ) {
			Call_Back();
		}
		Sound_End();
		Call_Back();
		wait.Set (60,true);
		while ( wait.Time() ) {
			Call_Back();
		}
		SoundOn = 0;
	}

	dialstatus = NullModem.Dial_Modem( DialString, settings->DialMethod, reconnect );

	if (reconnect) {
		/*
		--------------------------- Redraw the display ---------------------------
		*/
		HiddenPage.Clear();
		Map.Flag_To_Redraw(true);
		Map.Render();
	}

	switch ( dialstatus ) {
		case DIAL_CONNECTED:
			connected = true;
			break;

		case DIAL_NO_CARRIER:
			CCMessageBox().Process(TXT_NO_CARRIER);
			connected = false;
			break;

		case DIAL_BUSY:
			CCMessageBox().Process(TXT_LINE_BUSY);
			connected = false;
			break;

		case DIAL_ERROR:
			CCMessageBox().Process(TXT_NUMBER_INVALID);
			connected = false;
			break;

		case DIAL_NO_DIAL_TONE:
			CCMessageBox().Process(TXT_NO_DIAL_TONE);
			connected = false;
			break;

		case DIAL_CANCELED:
			NullModem.Hangup_Modem();
			ModemService = false;
			CCMessageBox().Process(TXT_DIALING_CANCELED);
			connected = false;
			break;
	}

	NullModem.Remove_Modem_Echo();
	NullModem.Print_EchoBuf();
	NullModem.Reset_EchoBuf();

	/*
	** Restore audio capability
	*/
	SoundOn = Audio_Init ( MainWindow , 16 , false , 11025*2 , 0 );
	if (SoundOn){
		Theme.Play_Song (old_theme);
	}

	ModemService = true;
	return( connected );

}	/* end of Dial_Modem */


static bool Answer_Modem( SerialSettingsType *settings, bool reconnect )
{
	bool connected = false;
	DialStatusType dialstatus;
	int modemstatus;

/* ###Change collision detected! C:\PROJECTS\CODE\NULLDLG.CPP... */
	/*
	**	Turn modem servicing off in the callback routine.
	*/
	ModemService = false;

	// save for later to reconnect

	DialSettings = settings;

	modemstatus = NullModem.Get_Modem_Status();
	if (reconnect) {
		if ( (modemstatus & CD_SET) ) {
			connected = true;
			ModemService = true;
			return( connected );
		}
	} else {
		if ( (modemstatus & CD_SET) ) {
			NullModem.Hangup_Modem();
			ModemService = false;
		}
	}

	NullModem.Setup_Modem_Echo( Modem_Echo );

	modemstatus = NullModem.Detect_Modem( settings, reconnect );
	if ( !modemstatus ) {
		NullModem.Remove_Modem_Echo();
		NullModem.Print_EchoBuf();
		NullModem.Reset_EchoBuf();

		/*
		** If our first attempt to detect the modem failed, and we're at
		** 14400 or 28800, bump up to the next baud rate & try again.
		*/
		switch (settings->Baud) {

			case 14400:
				settings->Baud = 19200;
				Shutdown_Modem();
				Init_Null_Modem(settings);
				NullModem.Setup_Modem_Echo( Modem_Echo );
				modemstatus = NullModem.Detect_Modem( settings, reconnect );
				if ( !modemstatus ) {
					NullModem.Remove_Modem_Echo();
					NullModem.Print_EchoBuf();
					NullModem.Reset_EchoBuf();
					CCMessageBox().Process( TXT_UNABLE_FIND_MODEM );
					ModemService = true;
					return( connected );
				}
				break;

			case 28800:
				settings->Baud = 38400;
				Shutdown_Modem();
				Init_Null_Modem(settings);
				NullModem.Setup_Modem_Echo( Modem_Echo );
				modemstatus = NullModem.Detect_Modem( settings, reconnect );
				if ( !modemstatus ) {
					NullModem.Remove_Modem_Echo();
					NullModem.Print_EchoBuf();
					NullModem.Reset_EchoBuf();
					CCMessageBox().Process( TXT_UNABLE_FIND_MODEM );
					ModemService = true;
					return( connected );
				}
				break;

			default:
				CCMessageBox().Process( TXT_UNABLE_FIND_MODEM );
				ModemService = true;
				return( connected );
		}

	}
	else if ( modemstatus == -1 ) {
		NullModem.Remove_Modem_Echo();
		NullModem.Print_EchoBuf();
		NullModem.Reset_EchoBuf();
		CCMessageBox().Process( TXT_ERROR_IN_INITSTRING );
		ModemService = true;
		return( connected );
	}

	/*
	** Completely disable audio. This is required for MWave devices
	*/
	ThemeType old_theme = THEME_NONE;
	if (SoundOn){
		old_theme = Theme.What_Is_Playing();
		Theme.Stop();
		CountDownTimerClass wait;
		Call_Back();
		wait.Set (60,true);
		while ( wait.Time() ) {
			Call_Back();
		}
		Sound_End();
		Call_Back();
		wait.Set (60,true);
		while ( wait.Time() ) {
			Call_Back();
		}
		SoundOn = 0;
	}

	dialstatus = NullModem.Answer_Modem( reconnect );

	switch ( dialstatus ) {
		case DIAL_CONNECTED:
			connected = true;
			break;

		case DIAL_NO_CARRIER:
			CCMessageBox().Process(TXT_NO_CARRIER);
			connected = false;
			break;

		case DIAL_BUSY:
			CCMessageBox().Process(TXT_LINE_BUSY);
			connected = false;
			break;

		case DIAL_ERROR:
			CCMessageBox().Process(TXT_NUMBER_INVALID);
			connected = false;
			break;

		case DIAL_NO_DIAL_TONE:
			CCMessageBox().Process(TXT_NO_DIAL_TONE);
			connected = false;
			break;

		case DIAL_CANCELED:
			CCMessageBox().Process(TXT_ANSWERING_CANCELED);
			connected = false;
			break;
	}

	NullModem.Remove_Modem_Echo();
	NullModem.Print_EchoBuf();
	NullModem.Reset_EchoBuf();

	/*
	** Restore audio capability
	*/
	SoundOn = Audio_Init ( MainWindow , 16 , false , 11025*2 , 0 );
	if (SoundOn){
		Theme.Play_Song (old_theme);
	}

	ModemService = true;
	return( connected );

}	/* end of Answer_Modem */


static void Modem_Echo( char c )
{
	if (NullModem.EchoCount < (NullModem.EchoSize - 1) ) {
		*(NullModem.EchoBuf + NullModem.EchoCount) = c;
		*(NullModem.EchoBuf + NullModem.EchoCount + 1) = 0;
		NullModem.EchoCount++;
	} else {
		//Smart_Printf( "Echo buffer full!!!\n" );
	}

}	/* end of Modem_Echo */


void Smart_Printf( char *format, ... )
{
	va_list arglist;
	char buf[501];


	va_start(arglist,format);
	vsprintf(buf,format,arglist);
	va_end(arglist);

	if (Debug_Smart_Print) {
		if (Special.IsMonoEnabled) {
//			Mono_Set_Cursor(0,0);
			Mono_Printf("%s",buf);
		} else {
//			Mono_Printf("%s",buf);
			printf("%s",buf);
		}
	} else {
		if (Debug_Heap_Dump) {
			printf("%s",buf);
		}
	}
}


void Hex_Dump_Data( char *buffer, int length )
{
	int i;
	int offset = 0;
	char buff[10];
	char ptr[16];
	char c;


	while (length >= 16) {
		memcpy( ptr, (buffer + offset), 16);

		Smart_Printf("%05lX  ", offset);

		for (i = 0; i < 16; i++) {

			c = ptr[i];
			itoh(c, buff);

			if (!(i & 0x3) && i) {
				Smart_Printf("³ ");
			}

			Smart_Printf("%s ", buff);
		}

		Smart_Printf("  ");

		for (i = 0; i < 16; i++) {
			c = ptr[i];

			if (c && ((c < 7) || (c > 11)) && (c != 13)) {
				Smart_Printf("%c", c);
			} else {
				Smart_Printf(".");
			}
		}

		Smart_Printf("\n");

		offset += 16;
		length -= 16;
	}

	if (length) {
		memcpy( ptr, (buffer + offset), 16);

		Smart_Printf("%05lX  ", offset);

		for (i = 0; i < 16; i++) {
			if (i < length) {
				c = ptr[i];
				itoh(c, buff);
				if (!(i & 0x3) && i) {
					Smart_Printf("³ ");
				}
				Smart_Printf("%s ", buff);
			} else {
				if (!(i & 0x3) && i) {
					Smart_Printf("  ");
				}
				Smart_Printf("   ");
			}
		}

		Smart_Printf("  ");

		for (i = 0; i < length; i++) {

			c = ptr[i];

			if (c && ((c < 7) || (c > 11)) && (c != 13)) {
				Smart_Printf("%c", c);
			} else {
				Smart_Printf(".");
			}
		}

		Smart_Printf("\n");
	}

}	/* end of Hex_Dump_Data */


void itoh( int i, char *s)
{

	int nibble, loop;

//	*s++ = '0';
//	*s++ = 'x';

	if (i == 0) {
		*s++ = '0';
		*s++ = '0';
	} else {
		for (loop = 1; loop >= 0; loop--) {
			nibble = (i >> (loop << 2)) & 0x000F;

			/* decimal range */
			if (nibble < 10) {
				*s++ = '0' + nibble;
			} else {
				*s++ = 'A' + (nibble - 10);
			}
		}
	}
	*s = 0;							/* null terminate it */
}





























#if (0)

int Com_Fake_Scenario_Dialog(void)
{
	bool display = true;		// redraw level
	bool process = true;						// process while true

	char namebuf[MPLAYER_NAME_MAX] = {0};		// buffer for player's name
	int transmit;								// 1 = re-transmit new game options
	int parms_received = 1;					// 1 = game options received
	int changed = 0;							// 1 = user has changed an option

	int rc;
	int recsignedoff = false;
	int version;
	unsigned long starttime;
	unsigned long timingtime;
	unsigned long lastmsgtime;
	unsigned long lastredrawtime;
	unsigned long transmittime = 0;
	unsigned long theirresponsetime;
	int packetlen;
	bool oppscorescreen = false;
	bool gameoptions = false;
	EventClass *event;					// event ptr
	unsigned long msg_timeout = 60*60;	// init to 60 seconds

	int 	factor 		= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_CANCEL = 100,
	};

	/*........................................................................
	Dialog variables
	........................................................................*/
	TextButtonClass * buttons = 0;

	KeyNumType input;

	int x,y;
	int width=0;
	int height=0;
	char text_buffer[80*3];

	const char *current_status_string = Text_String(TXT_WINSOCK_CONNECTING);
	strcpy(text_buffer, current_status_string);

	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);

	Format_Window_String(text_buffer, SeenBuff.Get_Height(), width, height);

	width = MAX(width, 50*factor);
	width += 40*factor;
	height += 60*factor;

	x = (SeenBuff.Get_Width() - width) / 2;
	y = (SeenBuff.Get_Height() - height) / 2;

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		x + ((width - (String_Pixel_Width( Text_String( TXT_CANCEL ) ) + 8*factor)) >> 1),
		y + height - (FontHeight + FontYSpacing + 2*factor) - 5*factor);

	Set_Logic_Page(SeenBuff);
	buttons = &cancelbtn;

	buttons->Flag_List_To_Redraw();
	process = true;

	/*........................................................................
	Init other scenario parameters
	........................................................................*/
	Special.IsTGrowth = MPlayerTiberium;
	Special.IsTSpread = MPlayerTiberium;
	transmit = 1;

	/*........................................................................
	Init random-number generator, & create a seed to be used for all random
	numbers from here on out
	........................................................................*/
	randomize();
	//Seed = rand();

	/*
	---------------------------- Processing loop -----------------------------
	*/
	NullModem.Reset_Response_Time();		// clear response time
	theirresponsetime = 10000;				// initialize to an invalid value
	timingtime = lastmsgtime = lastredrawtime = TickCount.Time();


	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=true;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
			Blit_Hid_Page_To_Seen_Buff();
			Set_Palette(Palette);
			/*
			..................... Draw the background .......................
			*/
			Dialog_Box(x, y, width, height);
			/*
			....................... Draw the labels .........................
			*/
			Draw_Caption(TXT_NONE, x, y, width);

			Fancy_Text_Print(text_buffer, x + 20*factor, y + 25*factor, CC_GREEN, TBLACK,
				TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

			buttons->Draw_All();
			Show_Mouse();
			display = false;
		}

		/*
		............................ Process input ............................
		*/
		input = buttons->Input();
		switch (input) {
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				process = false;
				rc = false;
				break;

			default:
				break;
		}


		/*---------------------------------------------------------------------
		If our Transmit flag is set, we need to send out a game option packet.
		This message requires an ACK.  The first time through the loop, transmit
		should be set, so we send out our default options; we'll then send
		any changes we make to the defaults.
		---------------------------------------------------------------------*/
		if (transmit && (TickCount.Time() - transmittime) > PACKET_RETRANS_TIME) {
			SendPacket.Command = SERIAL_GAME_OPTIONS;
			strcpy (SendPacket.Name, MPlayerName);
#ifdef PATCH
			if (IsV107) {
				SendPacket.Version = 1;
			} else {
				SendPacket.Version = 2;
			}
#else
			SendPacket.Version = Version_Number();
#endif
			SendPacket.House = MPlayerHouse;
			SendPacket.Color = MPlayerColorIdx;

			SendPacket.Scenario = MPlayerFilenum[ScenarioIdx];

			SendPacket.Credits = MPlayerCredits;
			SendPacket.IsBases = MPlayerBases;
			SendPacket.IsTiberium = MPlayerTiberium;
			SendPacket.IsGoodies = MPlayerGoodies;
			SendPacket.IsGhosties = MPlayerGhosts;
			SendPacket.BuildLevel = BuildLevel;
			SendPacket.UnitCount = MPlayerUnitCount;
			SendPacket.Seed = Seed;
			SendPacket.Special = Special;
			SendPacket.GameSpeed = Options.GameSpeed;
			SendPacket.ID = ModemGameToPlay;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			transmittime = TickCount.Time();
			transmit = 0;
		}

		//
		// send a timing packet if enough time has gone by.
		//
		if ( (TickCount.Time() - timingtime) > PACKET_TIMING_TIMEOUT) {
			SendPacket.Command = SERIAL_TIMING;
			SendPacket.ResponseTime = NullModem.Response_Time();
			SendPacket.ID = ModemGameToPlay;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 0);
			timingtime = TickCount.Time();
		}

		/*---------------------------------------------------------------------
		Check for an incoming message
		---------------------------------------------------------------------*/
		if (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {

			lastmsgtime = TickCount.Time();
			msg_timeout = 600;		// reset timeout value to 10 seconds
											// (only the 1st time through is 20 seconds)


			if (ReceivePacket.Command >= SERIAL_CONNECT &&
				ReceivePacket.Command < SERIAL_LAST_COMMAND &&
				ReceivePacket.Command != SERIAL_MESSAGE &&
				ReceivePacket.ID == ModemGameToPlay) {

				CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);

				// to skip the other system not responding msg
				lastmsgtime = TickCount.Time();

				process = false;
				rc = false;

				// say we did receive sign off to keep from sending one
				recsignedoff = true;
				break;
			}

			event = (EventClass *)&ReceivePacket;
			if (event->Type <= EventClass::FRAMEINFO) {
				if ( (TickCount.Time() - lastredrawtime) > PACKET_REDRAW_TIME) {
					lastredrawtime = TickCount.Time();
					oppscorescreen = true;
					parms_received = 1;
				}
			} else {
				switch ( ReceivePacket.Command ) {
					/*..................................................................
					Sign-off: Give the other machine time to receive my ACK, display a
					message, and exit.
					..................................................................*/
					case (SERIAL_SIGN_OFF):
						starttime = TickCount.Time();
						while (TickCount.Time() - starttime < 60)
							NullModem.Service();
						CCMessageBox().Process(TXT_USER_SIGNED_OFF);

						// to skip the other system not responding msg
						lastmsgtime = TickCount.Time();

						process = false;
						rc = false;
						recsignedoff = true;
						break;

					/*..................................................................
					Game Options:  Store the other machine's name, color & house;
					If they've picked the same color as myself, re-transmit my settings
					to force him to choose a different color.  (Com_Show_Scenario_Dialog
					is responsible for ensuring the colors are different.)
					..................................................................*/
					case (SERIAL_GAME_OPTIONS):
						oppscorescreen = false;
						gameoptions = true;
						strcpy (TheirName, ReceivePacket.Name);
						TheirColor = ReceivePacket.Color;
						TheirHouse = ReceivePacket.House;
						transmit = 1;

						parms_received = 1;

						/*...............................................................
						Check the version number of the other system.
						...............................................................*/
#ifdef PATCH
						if (IsV107) {
							version = 1;
						} else {
							version = 2;
						}
#else
						version = Version_Number();
#endif
						if (ReceivePacket.Version > version) {
							CCMessageBox().Process (TXT_YOURGAME_OUTDATED);

							// to skip the other system not responding msg
							lastmsgtime = TickCount.Time();

							process = false;
							rc = false;
						} else {
							if (ReceivePacket.Version < version) {
								CCMessageBox().Process (TXT_DESTGAME_OUTDATED);

								// to skip the other system not responding msg
								lastmsgtime = TickCount.Time();

								process = false;
								rc = false;
							}
						}
						break;

					/*..................................................................
					Incoming message: add to our list
					..................................................................*/

					//
					// get their response time
					//
					case (SERIAL_TIMING):
						oppscorescreen = false;
						theirresponsetime = ReceivePacket.ResponseTime;

						if ( !gameoptions ) {
							transmit = 1;
						}else{
							process = false;
							rc = true;
						}
						break;

					//
					// print msg waiting for opponent
					//
					case (SERIAL_SCORE_SCREEN):
						oppscorescreen = true;
						parms_received = 1;
						break;

					default:
						break;
				}
			}
		}

		// if we haven't received a msg for 10 seconds exit

		if ( (TickCount.Time() - lastmsgtime) > msg_timeout) {
			CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);
			process = false;
			rc = false;

			// say we did receive sign off to keep from sending one
			recsignedoff = true;
		}

		/*---------------------------------------------------------------------
		Service the connection
		---------------------------------------------------------------------*/
		NullModem.Service();

	}	/* end of while */

	/*------------------------------------------------------------------------
	Sort player ID's, so we can execute commands in the same order on both
	machines.
	------------------------------------------------------------------------*/
	if (rc) {
		/*.....................................................................
		Set the number of players in this game, and my ID
		.....................................................................*/
		MPlayerCount = 2;
		MPlayerLocalID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);

		TheirID = Build_MPlayerID (TheirColor,TheirHouse);

		/*.....................................................................
		Store every player's ID in the MPlayerID[] array.  This array will
		determine the order of event execution, so the ID's must be stored
		in the same order on all systems.
		.....................................................................*/
		if (TheirID < MPlayerLocalID) {
			MPlayerID[0] = TheirID;
			MPlayerID[1] = MPlayerLocalID;
			strcpy (MPlayerNames[0], TheirName);
			strcpy (MPlayerNames[1], MPlayerName);
		} else {
			MPlayerID[0] = MPlayerLocalID;
			MPlayerID[1] = TheirID;
			strcpy (MPlayerNames[0], MPlayerName);
			strcpy (MPlayerNames[1], TheirName);
		}

		/*.....................................................................
		Get the scenario filename
		.....................................................................*/
		Scenario = MPlayerFilenum[ScenarioIdx];

		/*.....................................................................
		Send all players the GO packet.
		.....................................................................*/
		SendPacket.Command = SERIAL_GO;
		SendPacket.ResponseTime = NullModem.Response_Time();
		if ( theirresponsetime == 10000 ) {
//			Mono_Clear_Screen();
//			Smart_Printf( "Did not receive their response time!!!!!!!\n" );
//			Get_Key();
		} else {
			if (SendPacket.ResponseTime < theirresponsetime) {
				SendPacket.ResponseTime = theirresponsetime;
			}
		}

		//
		// calculated one way delay for a packet and overall delay to execute
		// a packet
		//
		////////MPlayerMaxAhead = MAX( (SendPacket.ResponseTime / 8), 2);
		SendPacket.ID = ModemGameToPlay;

		NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

		starttime = TickCount.Time();
		while ( ( NullModem.Num_Send()
			&& ((TickCount.Time() - starttime) < PACKET_SENDING_TIMEOUT) )
			|| ((TickCount.Time() - starttime) < 60) ) {
			#if(SHOW_MONO)
			NullModem.Mono_Debug_Print(0);
			#endif

			NullModem.Service();
			Keyboard::Check();		//Make sure the message loop gets called
		}

		// clear queue to keep from doing any resends
		NullModem.Init_Send_Queue();

	} else {
		if ( !recsignedoff ) {
			/*.....................................................................
			Broadcast my sign-off over my network
			.....................................................................*/
			SendPacket.Command = SERIAL_SIGN_OFF;
			SendPacket.Color = MPlayerLocalID;		// use Color for ID
			SendPacket.ID = ModemGameToPlay;
			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			starttime = TickCount.Time();
			while ( (NullModem.Num_Send()
				&& ((TickCount.Time() - starttime) < PACKET_CANCEL_TIMEOUT) )
				|| ((TickCount.Time() - starttime) < 60) ) {
				#if(SHOW_MONO)
				NullModem.Mono_Debug_Print(0);
				#endif

				if ( NullModem.Get_Message( &ReceivePacket, &packetlen ) > 0) {

					// are we getting our own packets back??

					if (ReceivePacket.Command == SERIAL_SIGN_OFF
						&& ReceivePacket.ID == ModemGameToPlay) {

						// exit while
						break;
					}
				}

				NullModem.Service();
			}
		}

		Shutdown_Modem();
	}

	/*------------------------------------------------------------------------
	Restore screen
	------------------------------------------------------------------------*/
	Hide_Mouse();
	Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
	Blit_Hid_Page_To_Seen_Buff();
	Show_Mouse();

	/*------------------------------------------------------------------------
	Save any changes made to our options
	------------------------------------------------------------------------*/
	if (changed) {
		Write_MultiPlayer_Settings ();
	}

	return(rc);

}	/* end of Com_Scenario_Dialog */


























int Com_Show_Fake_Scenario_Dialog(void)
{
	/*........................................................................
	Dialog variables
	........................................................................*/
	bool display = true;		// redraw level
	BOOL process = true;						// process while true

	char namebuf[MPLAYER_NAME_MAX] = {0};		// buffer for player's name
	int transmit;										// 1 = re-transmit new game options
	int first;											// 1 = no packets received yet
	int parms_received = 0;							// 1 = game options received
	int changed = 0;							// 1 = user has changed an option

	int rc;
	int recsignedoff = 0;
	int i;
	int version;
	unsigned long starttime;
	unsigned long timingtime;
	unsigned long lastmsgtime;
	unsigned long lastredrawtime;
	unsigned long transmittime = 0;
	int packetlen;
	bool oppscorescreen = false;
	bool gameoptions = false;
	EventClass *event;					// event ptr
	unsigned long msg_timeout = 60*60;	// init to 60 seconds


	int 	factor 		= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_CANCEL = 100,
	};

	/*........................................................................
	Dialog variables
	........................................................................*/
	TextButtonClass * buttons = 0;

	KeyNumType input;

	int x,y;
	int width=0;
	int height=0;
	char text_buffer[80*3];

	const char *current_status_string = Text_String(TXT_WINSOCK_CONNECTING);
	strcpy(text_buffer, current_status_string);

	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);

	Format_Window_String(text_buffer, SeenBuff.Get_Height(), width, height);

	width = MAX(width, 50*factor);
	width += 40*factor;
	height += 60*factor;

	x = (SeenBuff.Get_Width() - width) / 2;
	y = (SeenBuff.Get_Height() - height) / 2;

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		x + ((width - (String_Pixel_Width( Text_String( TXT_CANCEL ) ) + 8*factor)) >> 1),
		y + height - (FontHeight + FontYSpacing + 2*factor) - 5*factor);

	Set_Logic_Page(SeenBuff);
	buttons = &cancelbtn;

	buttons->Flag_List_To_Redraw();


	transmit = 1;
	first = 1;


	/*
	---------------------------- Processing loop -----------------------------
	*/
	NullModem.Reset_Response_Time();		// clear response time
	timingtime = lastmsgtime = lastredrawtime = TickCount.Time();

	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=true;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
			Blit_Hid_Page_To_Seen_Buff();
			Set_Palette(Palette);
			/*
			..................... Draw the background .......................
			*/
			Dialog_Box(x, y, width, height);
			/*
			....................... Draw the labels .........................
			*/
			Draw_Caption(TXT_NONE, x, y, width);

			Fancy_Text_Print(text_buffer, x + 20*factor, y + 25*factor, CC_GREEN, TBLACK,
				TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

			buttons->Draw_All();
			Show_Mouse();
			display = false;
		}


		/*
		............................ Process input ............................
		*/
		input = buttons->Input();
		switch (input) {
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				process = false;
				rc = false;
				break;

			default:
				break;
		}



		/*---------------------------------------------------------------------
		If our Transmit flag is set, we need to send out a game option packet
		---------------------------------------------------------------------*/
		if (transmit && (TickCount.Time() - transmittime) > PACKET_RETRANS_TIME) {
			SendPacket.Command = SERIAL_GAME_OPTIONS;
			strcpy (SendPacket.Name, MPlayerName);
#ifdef PATCH
			if (IsV107) {
				SendPacket.Version = 1;
			} else {
				SendPacket.Version = 2;
			}
#else
			SendPacket.Version = Version_Number();
#endif
			SendPacket.House = MPlayerHouse;
			SendPacket.Color = MPlayerColorIdx;
			SendPacket.ID = ModemGameToPlay;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			transmittime = TickCount.Time();
			transmit = 0;
		}

		//
		// send a timing packet if enough time has gone by.
		//
		if ( (TickCount.Time() - timingtime) > PACKET_TIMING_TIMEOUT) {
			SendPacket.Command = SERIAL_TIMING;
			SendPacket.ResponseTime = NullModem.Response_Time();
			SendPacket.ID = ModemGameToPlay;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 0);
			timingtime = TickCount.Time();
		}

		/*---------------------------------------------------------------------
		Check for an incoming message
		---------------------------------------------------------------------*/
		if (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {

			lastmsgtime = TickCount.Time();

			msg_timeout = 600;

			// are we getting our own packets back??

			if (ReceivePacket.Command >= SERIAL_CONNECT &&
				ReceivePacket.Command < SERIAL_LAST_COMMAND &&
				ReceivePacket.Command != SERIAL_MESSAGE &&
				ReceivePacket.ID == ModemGameToPlay) {

				CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);

				// to skip the other system not responding msg
				lastmsgtime = TickCount.Time();

				process = false;
				rc = false;

				// say we did receive sign off to keep from sending one
				recsignedoff = true;
				break;
			}

			event = (EventClass *)&ReceivePacket;
			if (event->Type <= EventClass::FRAMEINFO) {
				if ( (TickCount.Time() - lastredrawtime) > PACKET_REDRAW_TIME) {
					lastredrawtime = TickCount.Time();
					oppscorescreen = true;
					display = false;
					parms_received = 1;
				}
			} else {
				switch ( ReceivePacket.Command ) {
					/*..................................................................
					Other system signs off:  Give it time to receive my ACK, then show
					a message.
					..................................................................*/
					case (SERIAL_SIGN_OFF):
						starttime = TickCount.Time();
						while ( (TickCount.Time() - starttime) < 60)
							NullModem.Service();
						CCMessageBox().Process(TXT_USER_SIGNED_OFF);

						// to skip the other system not responding msg
						lastmsgtime = TickCount.Time();

						process = false;
						rc = false;
						recsignedoff = true;
						break;

					/*..................................................................
					Game Options: Store all options; check my color & game version.
					..................................................................*/
					case (SERIAL_GAME_OPTIONS):
						oppscorescreen = false;
						gameoptions = true;
						display = false;
						parms_received = 1;

						strcpy (TheirName, ReceivePacket.Name);
						TheirColor = ReceivePacket.Color;
						TheirHouse = ReceivePacket.House;

						/*...............................................................
						Save scenario settings.
						...............................................................*/
						MPlayerCredits = ReceivePacket.Credits;
						MPlayerBases = ReceivePacket.IsBases;
						MPlayerTiberium = ReceivePacket.IsTiberium;
						MPlayerGoodies = ReceivePacket.IsGoodies;
						MPlayerGhosts = ReceivePacket.IsGhosties;
						BuildLevel = ReceivePacket.BuildLevel;
						MPlayerUnitCount = ReceivePacket.UnitCount;
						Seed = ReceivePacket.Seed;
						Special = ReceivePacket.Special;
						Options.GameSpeed = ReceivePacket.GameSpeed;

						if (MPlayerTiberium) {
							Special.IsTGrowth = 1;
							Special.IsTSpread = 1;
						} else {
							Special.IsTGrowth = 0;
							Special.IsTSpread = 0;
						}

						/*...............................................................
						Find the index of the scenario number; if it's not found, leave
						it at -1.
						...............................................................*/
						ScenarioIdx = -1;
						for (i = 0; i < MPlayerFilenum.Count(); i++) {
							if (ReceivePacket.Scenario == MPlayerFilenum[i])
								ScenarioIdx = i;
						}

						/*...............................................................
						Check our version numbers; if they're incompatible, sign off.
						...............................................................*/
#ifdef PATCH
						if (IsV107) {
							version = 1;
						} else {
							version = 2;
						}
#else
						version = Version_Number();
#endif
						if (ReceivePacket.Version > version) {
							CCMessageBox().Process (TXT_YOURGAME_OUTDATED);

							// to skip the other system not responding msg
							lastmsgtime = TickCount.Time();

							process = false;
							rc = false;
						} else {
							if (ReceivePacket.Version < version) {
								CCMessageBox().Process (TXT_DESTGAME_OUTDATED);

								// to skip the other system not responding msg
								lastmsgtime = TickCount.Time();

								process = false;
								rc = false;
							}
						}

						/*...............................................................
						If this is the first game-options packet we've received, transmit
						our options to him.
						...............................................................*/
						if (first) {
							first = 0;

							// force transmitting of game options packet

							transmit = 1;
							transmittime = 0;
						}
						break;

					/*..................................................................
					GO: Exit this routine with a success code.
					..................................................................*/
					case (SERIAL_GO):

						//
						// calculated one way delay for a packet and overall delay
						// to execute a packet
						//
						////////MPlayerMaxAhead = MAX( (ReceivePacket.ResponseTime / 8), 2);

						process = false;
						rc = true;
						break;

					//
					// throw away timing packet
					//
					case (SERIAL_TIMING):
						oppscorescreen = false;
						break;

					//
					// print msg waiting for opponent
					//
					case (SERIAL_SCORE_SCREEN):
// Smart_Printf( "received score screen\n" );
						oppscorescreen = true;
						display = false;
						parms_received = 1;
						break;

					default:
						break;
				}
			}
		}

		// if we haven't received a msg for 10 seconds exit

		if ( (TickCount.Time() - lastmsgtime) > msg_timeout) {
			CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);
			process = false;
			rc = false;

			// say we did receive sign off to keep from sending one
			recsignedoff = true;
		}


		/*---------------------------------------------------------------------
		Service the connection
		---------------------------------------------------------------------*/
		NullModem.Service();

	}	/* end of while */

	/*------------------------------------------------------------------------
	Sort player ID's, so we can execute commands in the same order on both
	machines.
	------------------------------------------------------------------------*/
	if (rc) {
		/*.....................................................................
		Set the number of players in this game, and my ID
		.....................................................................*/
		MPlayerCount = 2;
		MPlayerLocalID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);

		TheirID = Build_MPlayerID (TheirColor,TheirHouse);

		/*.....................................................................
		Store every player's ID in the MPlayerID[] array.  This array will
		determine the order of event execution, so the ID's must be stored
		in the same order on all systems.
		.....................................................................*/
		if (TheirID < MPlayerLocalID) {
			MPlayerID[0] = TheirID;
			MPlayerID[1] = MPlayerLocalID;
			strcpy (MPlayerNames[0], TheirName);
			strcpy (MPlayerNames[1], MPlayerName);
		} else {
			MPlayerID[0] = MPlayerLocalID;
			MPlayerID[1] = TheirID;
			strcpy (MPlayerNames[0], MPlayerName);
			strcpy (MPlayerNames[1], TheirName);
		}

		/*.....................................................................
		Get the scenario filename
		.....................................................................*/
		Scenario = MPlayerFilenum[ScenarioIdx];

		starttime = TickCount.Time();
		while ( ( NullModem.Num_Send()
			&& ((TickCount.Time() - starttime) < PACKET_SENDING_TIMEOUT) )
			|| ((TickCount.Time() - starttime) < 60) ) {
			#if(SHOW_MONO)
			NullModem.Mono_Debug_Print(0);
			#endif

			NullModem.Service();
			Keyboard::Check();		//Make sure the message loop gets called
		}

		// clear queue to keep from doing any resends
		NullModem.Init_Send_Queue();

	} else {
		if ( !recsignedoff ) {
			/*.....................................................................
			Broadcast my sign-off over my network
			.....................................................................*/
			SendPacket.Command = SERIAL_SIGN_OFF;
			SendPacket.Color = MPlayerLocalID;		// use Color for ID
			SendPacket.ID = ModemGameToPlay;
			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			starttime = TickCount.Time();
			while ( (NullModem.Num_Send()
				&& ((TickCount.Time() - starttime) < PACKET_CANCEL_TIMEOUT) )
				|| ((TickCount.Time() - starttime) < 60) ) {
				#if(SHOW_MONO)
				NullModem.Mono_Debug_Print(0);
				#endif

				if ( NullModem.Get_Message( &ReceivePacket, &packetlen ) > 0) {

					// are we getting our own packets back??

					if (ReceivePacket.Command == SERIAL_SIGN_OFF
						&& ReceivePacket.ID == ModemGameToPlay) {

						// exit while
						break;
					}
				}

				NullModem.Service();
			}
		}

		Shutdown_Modem();
	}

	/*------------------------------------------------------------------------
	Save any changes made to our options
	------------------------------------------------------------------------*/
	if (changed) {
		Write_MultiPlayer_Settings ();
	}

	return(rc);

}	/* end of Com_Show_Scenario_Dialog */


#endif	//(0)


#endif	//(0)























































void Smart_Printf( char *format, ... )
{
	va_list arglist;
	char buf[501];


	va_start(arglist,format);
	vsprintf(buf,format,arglist);
	va_end(arglist);

	if (Debug_Smart_Print) {
		if (Special.IsMonoEnabled) {
//			Mono_Set_Cursor(0,0);
			Mono_Printf("%s",buf);
		} else {
//			Mono_Printf("%s",buf);
			printf("%s",buf);
		}
	} else {
		if (Debug_Heap_Dump) {
			printf("%s",buf);
		}
	}
}


void Hex_Dump_Data( char *buffer, int length )
{
	int i;
	int offset = 0;
	char buff[10];
	char ptr[16];
	char c;


	while (length >= 16) {
		memcpy( ptr, (buffer + offset), 16);

		Smart_Printf("%05lX  ", offset);

		for (i = 0; i < 16; i++) {

			c = ptr[i];
			itoh(c, buff);

			if (!(i & 0x3) && i) {
				Smart_Printf("³ ");
			}

			Smart_Printf("%s ", buff);
		}

		Smart_Printf("  ");

		for (i = 0; i < 16; i++) {
			c = ptr[i];

			if (c && ((c < 7) || (c > 11)) && (c != 13)) {
				Smart_Printf("%c", c);
			} else {
				Smart_Printf(".");
			}
		}

		Smart_Printf("\n");

		offset += 16;
		length -= 16;
	}

	if (length) {
		memcpy( ptr, (buffer + offset), 16);

		Smart_Printf("%05lX  ", offset);

		for (i = 0; i < 16; i++) {
			if (i < length) {
				c = ptr[i];
				itoh(c, buff);
				if (!(i & 0x3) && i) {
					Smart_Printf("³ ");
				}
				Smart_Printf("%s ", buff);
			} else {
				if (!(i & 0x3) && i) {
					Smart_Printf("  ");
				}
				Smart_Printf("   ");
			}
		}

		Smart_Printf("  ");

		for (i = 0; i < length; i++) {

			c = ptr[i];

			if (c && ((c < 7) || (c > 11)) && (c != 13)) {
				Smart_Printf("%c", c);
			} else {
				Smart_Printf(".");
			}
		}

		Smart_Printf("\n");
	}

}	/* end of Hex_Dump_Data */


void itoh( int i, char *s)
{

	int nibble, loop;

//	*s++ = '0';
//	*s++ = 'x';

	if (i == 0) {
		*s++ = '0';
		*s++ = '0';
	} else {
		for (loop = 1; loop >= 0; loop--) {
			nibble = (i >> (loop << 2)) & 0x000F;

			/* decimal range */
			if (nibble < 10) {
				*s++ = '0' + nibble;
			} else {
				*s++ = 'A' + (nibble - 10);
			}
		}
	}
	*s = 0;							/* null terminate it */
}

