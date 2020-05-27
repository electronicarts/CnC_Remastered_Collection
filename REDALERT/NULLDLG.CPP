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

/* $Header: /counterstrike/NULLDLG.CPP 14    3/17/97 1:05a Steve_tall $ */
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
 *                  Last Update : Jan. 21, 1997 [V.Grippi]                     *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Build_InitString_Listbox -- [re]builds the initstring entry listbox   *
 *   Build_Phone_Listbox -- [re]builds the phone entry listbox             *
 *   Com_Scenario_Dialog -- Serial game scenario selection dialog				*
 *   Com_Settings_Dialog -- Lets user select serial port settings          *
 *   Destroy_Null_Connection -- destroys the given connection					*
 *   Edit_Phone_Dialog -- lets user edit a phone book entry                *
 *   Init_Null_Modem -- Initializes Null Modem communications              *
 *   Init_String_Compare -- for qsort													*
 *   Phone_Compare -- for qsort															*
 *   Phone_Dialog -- Lets user edit phone directory & dial                 *
 *   Reconnect_Null_Modem -- allows user to reconnect								*
 *   Select_Serial_Dialog -- Serial Communications menu dialog             *
 *   Shutdown_Modem -- Shuts down modem/null-modem communications          *
 *   Test_Null_Modem -- Null-Modem test routine                            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

//PG
GameType Select_Serial_Dialog(void) { return GAME_NORMAL; }





#if (0)//PG
#ifdef FIXIT_RANDOM_GAME
#include "time.h"
#endif
#ifdef WIN32
#include "wincomm.h"
#include "modemreg.h"
ModemRegistryEntryClass *ModemRegistry = NULL;		//Ptr to modem registry data
#endif	//WIN32
#include "COORDA.h"
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
extern bool Is_Mission_126x126 (char *file_name);
extern bool Is_Mission_Counterstrike (char *file_name);
extern bool Is_Mission_Aftermath( char* file_name );
#endif

//#include "WolDebug.h"

//
// how much time (ticks) to go by before thinking other system
// is not responding.
//
#define PACKET_SENDING_TIMEOUT	1800
#define PACKET_CANCEL_TIMEOUT		900

//extern char const *ForMisStr[];
extern char const *EngMisStr[];


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
PlayerColorType TheirColor;
HousesType TheirHouse;
static char DialString[ CWAITSTRBUF_MAX + PhoneEntryClass::PHONE_MAX_NUM - 1 ];
static SerialSettingsType *DialSettings;

#ifdef FIXIT_VERSION_3
bool Force_Scenario_Available( const char* szName );
bool bSpecialAftermathScenario( const char* szScenarioDescription );
#endif

#define PCOLOR_BROWN	PCOLOR_GREY


#define SHOW_MONO	0

/***************************************************************************
 * Init_Null_Modem -- Initializes Null Modem communications                *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																					   *
 *                                                                         *
 * OUTPUT:                                                                 *
 *		true = OK, false = error														   *
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																					   *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *   8/2/96      ST : Win32 support added                                  *
 *=========================================================================*/
int Init_Null_Modem( SerialSettingsType *settings )
{

#ifdef WIN32
	if ( NullModem.Init( settings->Port, settings->IRQ,
								settings->ModemName,
								settings->Baud, 0,  8, 1,
								settings->HardwareFlowControl ) ) {
		return (true);
	} else {
		return (false);
	}

#else	//WIN32
	if ( NullModem.Init( settings->Port, settings->IRQ,
								settings->ModemName,
								settings->Baud, 'N',  8, 1,
								settings->HardwareFlowControl) ) {
		NullModem.Change_IRQ_Priority( settings->IRQ );

		return(true);
	} else {
		return(false);
	}
#endif	//WIN32
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
	if (!Session.Play) {
		if (Session.Type == GAME_MODEM) {
			NullModem.Hangup_Modem();
		}
	}

	NullModem.Change_IRQ_Priority( 0 );		// reset priority of interrupts

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

	if (!Session.Play) {
		/*
		** Send a sign-off packet
		*/
		memset (&event, 0, sizeof(EventClass));
		event.Type = EventClass::EXIT;
		NullModem.Send_Message (&event,sizeof(EventClass),0);
		NullModem.Send_Message (&event,sizeof(EventClass),0);

		starttime = TickCount;
		while ( (TickCount - starttime) < 30) {
			NullModem.Service();
		}
	}
}


/***************************************************************************
 * Test_Null_Modem -- Null-Modem test routine                              *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = failure to connect; 1 = I'm the game owner, 2 = I'm not        *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/29/1995 BRR : Created.                                             *
 *   8/2/96      ST : Win32 support added                                  *
 *=========================================================================*/
int Test_Null_Modem( void )
{
	/*
	** Get the resolution factor
	*/
//	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	/*
	** Button Enumerations
	*/
	enum {
		BUTTON_CANCEL = 100,
	};

	/*
	** Dialog variables
	*/
	bool process = true;						// process while true

	int retval;
	unsigned long starttime;
	int packetlen;

	int x,y,width,height;					// dialog dimensions
	char buffer[80*3];
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	/*
	** Buttons
	*/
	GadgetClass *commands;										// button list

	/*
	**	Determine the dimensions of the text to be used for the dialog box.
	**	These dimensions will control how the dialog box looks.
	*/
	strcpy( buffer, Text_String( TXT_WAITING_CONNECT ) );
	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String(buffer, SeenBuff.Get_Height(), width, height);

	width = max(width, 50 * RESFACTOR);
	width += 40 * RESFACTOR;
	height += 60 * RESFACTOR;

	x = (SeenBuff.Get_Width() - width) / 2;
	y = (SeenBuff.Get_Height() - height) / 2;

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON,
		x + ((width - (String_Pixel_Width( Text_String( TXT_CANCEL ) ) + 8 * RESFACTOR)) >> 1),
		y + height - (FontHeight + FontYSpacing + 2 * RESFACTOR) - 10 * RESFACTOR);

	/*
	** Initialize
	*/
	Set_Logic_Page(SeenBuff);
	process = true;

	/*
	** Create the list
	*/
	commands = &cancelbtn;

	commands->Flag_List_To_Redraw();

	/*
	** Draw the dialog
	*/
	Hide_Mouse();
	Load_Title_Page(true);

	Dialog_Box(x, y, width, height);
	Draw_Caption(TXT_NONE, x, y, width);

	Fancy_Text_Print(buffer, x + 20 * RESFACTOR, y + 25 * RESFACTOR, scheme, TBLACK, TPF_TEXT);

	commands->Draw_All();
	while (Get_Mouse_State() > 0) Show_Mouse();

#ifdef WIN32
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
	while (time.Time()) {}

	/*
	** Back out of break mode
	*/
	ClearCommBreak(SerialPort->Get_Port_Handle());

	/*
	** Drop DTR as well - just in case the modem still hasnt got the message
	*/
	EscapeCommFunction(SerialPort->Get_Port_Handle(), CLRDTR);
#endif	//WIN32


	/*
	** Check for a packet.  If we detect one, the other system has already been
	** started.  Wait 1/2 sec for him to receive my ACK, then exit with success.
	** Note: The initial time must be a little longer than the resend delay.
	** 	Just in case we just missed the packet.
	*/
	starttime = TickCount;
	while ( TickCount - starttime < 80) {
		NullModem.Service();
		if ( NullModem.Get_Message( &ReceivePacket, &packetlen ) > 0) {
			if (ReceivePacket.Command == SERIAL_CONNECT) {
				starttime = TickCount;
				while (TickCount - starttime < 30)
					NullModem.Service();
				process = false;
				retval = 2;
				break;
			}
		}
	}

	/*
	** Send a packet across.  As long as Num_Send() is non-zero, the other system
	** hasn't received it yet.
	*/
	if (process) {
		memset (&SendPacket, 0, sizeof(SerialPacketType));
		SendPacket.Command = SERIAL_CONNECT;
		//
		// put time from start of game for determining the host in case of tie.
		//
		SendPacket.ScenarioInfo.Seed = TickCount;
		SendPacket.ID = (int) buffer;		// address of buffer for more uniqueness.

		NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

		starttime = TickCount;
		while (TickCount - starttime < 80) {
			NullModem.Service();
			if (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {
				if (ReceivePacket.Command == SERIAL_CONNECT) {
					starttime = TickCount;
					while (TickCount - starttime < 30)
						NullModem.Service();

					//
					// whoever has the highest time is the host
					//
					if (ReceivePacket.ScenarioInfo.Seed > SendPacket.ScenarioInfo.Seed) {
						process = false;
						retval = 2;
					} else if (ReceivePacket.ScenarioInfo.Seed == SendPacket.ScenarioInfo.Seed) {
						if (ReceivePacket.ID > SendPacket.ID) {
							process = false;
							retval = 2;
						//
						// if they are equal then it's a loopback cable or a modem
						//
						} else if (ReceivePacket.ID == SendPacket.ID) {
							process = false;
							retval = 3;
						}
					}

					break;
				}
			}
		}
	}

	starttime = TickCount;

	/*
	** Main Processing Loop
	*/
	while (process) {
#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			commands->Draw_All();
		}
#endif	//WIN32
		/*
		** Invoke game callback
		*/
		Call_Back();

		/*
		** Get user input
		*/
		KeyNumType input = commands->Input();

		/*
		** Process input
		*/
		switch (input) {
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				retval = 0;
				process = false;
				break;

			default:
				break;
		}
		/*
		** Service the connection.
		*/
		NullModem.Service();
		if (NullModem.Num_Send() == 0) {
			if (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {
				if (ReceivePacket.Command == SERIAL_CONNECT) {
					starttime = TickCount;
					while (TickCount - starttime < 30)
						NullModem.Service();

					//
					// whoever has the highest time is the host
					//
					if (ReceivePacket.ScenarioInfo.Seed > SendPacket.ScenarioInfo.Seed) {
						process = false;
						retval = 2;

					} else if (ReceivePacket.ScenarioInfo.Seed == SendPacket.ScenarioInfo.Seed) {
						if (ReceivePacket.ID > SendPacket.ID) {
							process = false;
							retval = 2;

						//
						// if they are equal then it's a loopback cable or a modem
						//
						} else if (ReceivePacket.ID == SendPacket.ID) {
							process = false;
							retval = 3;
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

		if (TickCount - starttime > 3600) {		// only wait 1 minute
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


	switch (Session.ModemType) {
		case (MODEM_NULL_HOST):
		case (MODEM_NULL_JOIN):
			status = Reconnect_Null_Modem();
			break;

		case (MODEM_DIALER):
			modemstatus = NullModem.Get_Modem_Status();
			if ( (modemstatus & CD_SET) ) {
				status = Reconnect_Null_Modem();
			} else {
				status = Dial_Modem( DialSettings, true );
			}
			break;

		case (MODEM_ANSWERER):
			modemstatus = NullModem.Get_Modem_Status();
			if ( (modemstatus & CD_SET) ) {
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
	/*
	** Button Enumerations
	*/
	enum {
		BUTTON_CANCEL = 100,
	};

	/*
	** Dialog variables
	*/
	bool process = true;						// process while true
	KeyNumType input;

	int retval;
	unsigned long starttime;
	unsigned long lastmsgtime;
	int packetlen;
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	int x,y,width,height;					// dialog dimensions
	char buffer[80*3];

	/*
	** Buttons
	*/
	GadgetClass *commands;										// button list


	/*
	**	Determine the dimensions of the text to be used for the dialog box.
	**	These dimensions will control how the dialog box looks.
	*/
	strcpy( buffer, Text_String( TXT_NULL_CONNERR_CHECK_CABLES ) );
	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String(buffer, SeenBuff.Get_Height(), width, height);

	width = max(width, 50 * RESFACTOR);
	width += 40 * RESFACTOR;
	height += 60 * RESFACTOR;

	x = (SeenBuff.Get_Width() - width) / 2;
	y = (SeenBuff.Get_Height() - height) / 2;

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON,
		x + ((width - (String_Pixel_Width( Text_String( TXT_CANCEL ) ) + 8 * RESFACTOR)) >> 1),
		y + height - (FontHeight + FontYSpacing + 2*RESFACTOR) - 10 * RESFACTOR);

	/*
	** Initialize
	*/
	Set_Logic_Page(SeenBuff);
	process = true;

	/*
	** Create the list
	*/
	commands = &cancelbtn;

	commands->Flag_List_To_Redraw();

	/*
	** Draw the dialog
	*/
	Hide_Mouse();

	Dialog_Box(x, y, width, height);
	Draw_Caption(TXT_NONE, x, y, width);

	Fancy_Text_Print(buffer, x + 20*RESFACTOR, y + 25*RESFACTOR, scheme, TBLACK, TPF_TEXT);

	commands->Draw_All();
	Show_Mouse();

	/*
	** Main Processing Loop
	*/
	starttime = lastmsgtime = TickCount;
	while (process) {
#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			commands->Draw_All();
		}
#endif	//WIN32
		/*
		** Invoke game callback
		*/
		Call_Back();

		/*
		** Get user input
		*/
		input = commands->Input();

		/*
		** Process input
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
		/*
		** Service the connection.
		*/
		NullModem.Service();

		/*
		** Resend our message if it's time
		*/
		if (TickCount - starttime > PACKET_RETRANS_TIME) {
			starttime = TickCount;
			memset (&SendPacket, 0, sizeof(SerialPacketType));
			SendPacket.Command = SERIAL_CONNECT;
			SendPacket.ID = Session.ColorIdx;
			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 0);
		}

		/*
		** Check for an incoming message
		*/
		if (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {

			lastmsgtime = TickCount;

			if (ReceivePacket.Command == SERIAL_CONNECT) {

				// are we getting our own packets back??

				if (ReceivePacket.ID == Session.ColorIdx) {
					WWMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);
					retval = false;
					process = false;
					break;
				}

				/*
				** OK, we got our message; now we have to make certain the other
				** guy gets his, so send him one with an ACK required.
				*/
				memset (&SendPacket, 0, sizeof(SerialPacketType));
				SendPacket.Command = SERIAL_CONNECT;
				SendPacket.ID = Session.ColorIdx;
				NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);
				starttime = TickCount;
				while (TickCount - starttime < 60)
					NullModem.Service();
				retval = true;
				process = false;
			}
		}

		//
		// timeout if we do not get any packets
		//
		if (TickCount - lastmsgtime > PACKET_CANCEL_TIMEOUT) {
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
 *		id			connection ID to destroy; this should be the HousesType of the player being	  *
 *             "destroyed".																						  *
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
	int i;
	HouseClass *housep;
	char txt[80];


	if ( Session.NumPlayers == 1 ) {
		return;
	}

	/*
	**	Do nothing if the house isn't human.
	*/
	housep = HouseClass::As_Pointer((HousesType)id);
	if (!housep || !housep->IsHuman)
		return;

	/*
	**	Create a message to display to the user
	*/
	txt[0] = '\0';
	switch (error) {
		case 1:
			sprintf(txt,Text_String(TXT_CONNECTION_LOST), housep->IniName);
			break;

		case 0:
			sprintf(txt,Text_String(TXT_LEFT_GAME), housep->IniName);
			break;

		case -1:
			NullModem.Delete_Connection();
			break;
	}

	if (strlen(txt)) {
		Session.Messages.Add_Message (NULL, 0, txt,
			(housep->RemapColor == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : housep->RemapColor,
			TPF_TEXT, Rule.MessageDelay * TICKS_PER_MINUTE);
		Map.Flag_To_Redraw(false);
	}

	/*
	** Remove this player from the Players vector
	*/
	for (i = 0; i < Session.Players.Count(); i++) {
		if (!stricmp(Session.Players[i]->Name,housep->IniName)) {
			delete Session.Players[i];
			Session.Players.Delete(Session.Players[i]);
			break;
		}
	}

	/*
	**	Turn the player's house over to the computer's AI
	*/
	housep->IsHuman = false;
//	housep->Smartness = IQ_MENSA;
	housep->IQ = Rule.MaxIQ;
	strcpy (housep->IniName,Text_String(TXT_COMPUTER));

	Session.NumPlayers--;

	/*
	**	If we're the last player left, tell the user.
	*/
	if (Session.NumPlayers == 1) {
		sprintf(txt,"%s",Text_String(TXT_JUST_YOU_AND_ME));
		Session.Messages.Add_Message (NULL, 0, txt,
			(housep->RemapColor == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : housep->RemapColor,
			TPF_TEXT, Rule.MessageDelay * TICKS_PER_MINUTE);
		Map.Flag_To_Redraw(false);
	}

}


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

	/*
	** Dialog & button dimensions
	*/
	int d_dialog_w = 160 * RESFACTOR;								// dialog width
	int d_dialog_h = 94 * RESFACTOR;								// dialog height
	int d_dialog_x = ((320 * RESFACTOR - d_dialog_w) / 2);	// dialog x-coord
	int d_dialog_y = 80*RESFACTOR;
//	int d_dialog_y = ((136 * RESFACTOR - d_dialog_h) / 2);	// dialog y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);	// center x-coord

	int d_txt6_h = 7 * RESFACTOR;									// ht of 6-pt text
	int d_margin = 7 * RESFACTOR;									// margin width/height

	int d_dial_w = 90 * RESFACTOR;
	int d_dial_h = 9 * RESFACTOR;
	int d_dial_x = d_dialog_cx - d_dial_w / 2;
	int d_dial_y = d_dialog_y + d_margin + d_txt6_h + d_margin;

	int d_answer_w = 90 * RESFACTOR;
	int d_answer_h = 9 * RESFACTOR;
	int d_answer_x = d_dialog_cx - d_answer_w / 2;
	int d_answer_y = d_dial_y + d_dial_h + 2;

	int d_nullmodem_w = 90 * RESFACTOR;
	int d_nullmodem_h = 9 * RESFACTOR;
	int d_nullmodem_x = d_dialog_cx - d_nullmodem_w / 2;
	int d_nullmodem_y = d_answer_y + d_answer_h + 2;

	int d_settings_w = 90 * RESFACTOR;
	int d_settings_h = 9 * RESFACTOR;
	int d_settings_x = d_dialog_cx - d_settings_w / 2;
	int d_settings_y = d_nullmodem_y + d_nullmodem_h + 2;

	int d_cancel_w = 50 * RESFACTOR;
	int d_cancel_h = 9 * RESFACTOR;
	int d_cancel_x = d_dialog_cx - d_cancel_w / 2;
	int d_cancel_y = d_settings_y + d_settings_h + d_margin;

	/*
	** Button Enumerations
	*/
	enum {
		BUTTON_DIAL = 100,
		BUTTON_ANSWER,
		BUTTON_NULLMODEM,
		BUTTON_SETTINGS,
		BUTTON_CANCEL,

		NUM_OF_BUTTONS = 5,
	};

	/*
	** Redraw values: in order from "top" to "bottom" layer of the dialog
	*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*
	** Dialog variables
	*/
	RedrawType display = REDRAW_ALL;		// redraw level
	bool process = true;						// process while true
	KeyNumType input;
	char namebuf[MPLAYER_NAME_MAX] = {0};		// buffer for player's name
	int tabs[] = {77*RESFACTOR};				// tabs for player list box
	GameType retval;							// return value

	int selection;
	bool pressed;
	int curbutton;
	TextButtonClass *buttons[NUM_OF_BUTTONS];

	SerialSettingsType *settings;
	bool selectsettings = false;
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();


	/*
	** Buttons
	*/
	GadgetClass *commands;										// button list

	TextButtonClass dialbtn(BUTTON_DIAL, TXT_DIAL_MODEM, TPF_BUTTON, d_dial_x, d_dial_y, d_dial_w, d_dial_h);
	TextButtonClass answerbtn(BUTTON_ANSWER, TXT_ANSWER_MODEM, TPF_BUTTON, d_answer_x, d_answer_y, d_answer_w, d_answer_h);
	TextButtonClass nullmodembtn(BUTTON_NULLMODEM, TXT_NULL_MODEM, TPF_BUTTON, d_nullmodem_x, d_nullmodem_y, d_nullmodem_w, d_nullmodem_h);
	TextButtonClass settingsbtn(BUTTON_SETTINGS, TXT_SETTINGS, TPF_BUTTON, d_settings_x, d_settings_y, d_settings_w, d_settings_h);
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);

	/*
	** Initialize
	*/
	Set_Logic_Page(SeenBuff);

	if (Session.SerialDefaults.Port == 0 ||
		Session.SerialDefaults.IRQ == -1 ||
		Session.SerialDefaults.Baud == -1) {
		selectsettings = true;

	} else if ( NullModem.Detect_Port( &Session.SerialDefaults ) != PORT_VALID ) {
		selectsettings = true;
	}

	/*
	** Create the list
	*/
	commands = &dialbtn;
	answerbtn.Add_Tail(*commands);
	nullmodembtn.Add_Tail(*commands);
	settingsbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);

	/*
	** Fill array of button ptrs
	*/
	curbutton = 0;
	buttons[0] = &dialbtn;
	buttons[1] = &answerbtn;
	buttons[2] = &nullmodembtn;
	buttons[3] = &settingsbtn;
	buttons[4] = &cancelbtn;
	buttons[curbutton]->Turn_On();

	Keyboard->Clear();

	Fancy_Text_Print(TXT_NONE, 0, 0, scheme, TBLACK, TPF_CENTER | TPF_TEXT);

	/*
	** Main Processing Loop
	*/
	display = REDRAW_ALL;
	process = true;
	pressed = false;
	while (process) {
		/*
		** Invoke game callback
		*/
		Call_Back();

		/*
		** Refresh display if needed
		*/
#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}
#endif

		if (display) {
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				/*
				** Refresh the backdrop
				*/
				Load_Title_Page(true);
				/*
				** Draw the background
				*/
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);
				/*
				** Draw the labels
				*/
				Draw_Caption (TXT_SELECT_SERIAL_GAME, d_dialog_x, d_dialog_y, d_dialog_w);
			}
			commands->Draw_All();
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		** Get user input
		*/
		input = commands->Input();

		/*
		** Process input
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
						WWMessageBox().Process(TXT_SELECT_SETTINGS);

					/*
					** Remote-connect
					*/
					} else if ( Phone_Dialog() ) {
						if (Session.PhoneBook[Session.CurPhoneIdx]->Settings.Port == 0) {
							settings = &Session.SerialDefaults;
						} else {
							settings = &(Session.PhoneBook[Session.CurPhoneIdx]->Settings);
						}
#ifdef WIN32
						if (SerialPort) {
							delete SerialPort;
						}
						SerialPort = new WinModemClass;
#endif	//WIN32
						if ( Init_Null_Modem( settings ) ) {

							if (settings->CallWaitStringIndex == CALL_WAIT_CUSTOM) {
								strcpy( DialString, settings->CallWaitString );
							} else {
								strcpy( DialString,
									Session.CallWaitStrings[ settings->CallWaitStringIndex ] );
							}
							strcat( DialString, Session.PhoneBook[ Session.CurPhoneIdx ]->Number );

							if ( Dial_Modem( settings, false ) ) {
								Session.ModemType = MODEM_DIALER;
								if ( Com_Scenario_Dialog() ) {
									retval = GAME_MODEM;
									process = false;
								}
							}

							if (process) {		// restore to default
								NullModem.Change_IRQ_Priority( 0 );
							}
						} else {
							WWMessageBox().Process(TXT_SELECT_SETTINGS);
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
						WWMessageBox().Process(TXT_SELECT_SETTINGS);
					} else {
						/*
						** Remote-connect
						*/
						settings = &Session.SerialDefaults;
#ifdef WIN32
						if (SerialPort) {
							delete SerialPort;
						}
						SerialPort = new WinModemClass;
#endif	//WIN32
						if ( Init_Null_Modem( settings ) ) {
							if ( Answer_Modem( settings, false ) ) {
								Session.ModemType = MODEM_ANSWERER;
								if ( Com_Show_Scenario_Dialog() ) {
									retval = GAME_MODEM;
									process = false;
								}
							}

							if (process) {		// restore to default
								NullModem.Change_IRQ_Priority( 0 );
							}
						} else {
							WWMessageBox().Process(TXT_SELECT_SETTINGS);
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
						WWMessageBox().Process(TXT_SELECT_SETTINGS);

					/*
					** Otherwise, remote-connect; save values if we're recording
					*/
					} else if ( Init_Null_Modem( &Session.SerialDefaults ) ) {
						rc = Test_Null_Modem();
						switch (rc) {
							case (1):
								Session.ModemType = MODEM_NULL_HOST;
								if ( Com_Scenario_Dialog() ) {
									retval = GAME_NULL_MODEM;
									process = false;
								}
								break;

							case (2):
								Session.ModemType = MODEM_NULL_JOIN;
								if ( Com_Show_Scenario_Dialog() ) {
									retval = GAME_NULL_MODEM;
									process = false;
								}
								break;

							case (3):
								WWMessageBox().Process( TXT_MODEM_OR_LOOPBACK );
								break;
						}

						if (process) {		// restore to default
							NullModem.Change_IRQ_Priority( 0 );
						}
					} else {
						WWMessageBox().Process(TXT_SELECT_SETTINGS);
					}

					if (process) {
						buttons[curbutton]->IsPressed = false;
						buttons[curbutton]->Flag_To_Redraw();
					}

					display = REDRAW_ALL;
					break;

				case (BUTTON_SETTINGS):
					if ( Com_Settings_Dialog( &Session.SerialDefaults ) ) {
						Session.Write_MultiPlayer_Settings ();

						selectsettings = true;

						if (Session.SerialDefaults.Port != 0 &&
							Session.SerialDefaults.IRQ != -1 &&
							Session.SerialDefaults.Baud != -1) {
							if ( NullModem.Detect_Port( &Session.SerialDefaults ) == PORT_VALID) {
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

	return( retval );
}






#ifdef WIN32
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

	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
#if (FRENCH | GERMAN)
	int d_dialog_w = 440;													// dialog width
#else
	int d_dialog_w = 340;													// dialog width
#endif

	int d_dialog_h = 200;													// dialog height
	int d_dialog_x = 320 - d_dialog_w/2;								// dialog x-coord
	int d_dialog_y = 200 - d_dialog_h/ 4;								// dialog y-coord


	int d_compression_w = 50;
	int d_compression_h = 18;
	int d_compression_x = d_dialog_x + d_dialog_w/2 +40;
	int d_compression_y = d_dialog_y + 40;

	int d_errorcorrection_w = 50;
	int d_errorcorrection_h = 18;
	int d_errorcorrection_x = d_dialog_x + d_dialog_w/2 +40;
	int d_errorcorrection_y = d_dialog_y + 65;

	int d_hardwareflowcontrol_w = 50;
	int d_hardwareflowcontrol_h = 18;
	int d_hardwareflowcontrol_x = d_dialog_x + d_dialog_w/2 +40;
	int d_hardwareflowcontrol_y = d_dialog_y + 90;

	int d_default_w = 100;
	int d_default_h = 18;
	int d_default_x = d_dialog_x + d_dialog_w / 2 - d_default_w / 2;
	int d_default_y = d_dialog_y + d_dialog_h - 70;

	int d_ok_w = 100;
	int d_ok_h = 18;
	int d_ok_x = d_dialog_x + d_dialog_w/2 - d_ok_w / 2;
	int d_ok_y = d_dialog_y + d_dialog_h - 40;

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


	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

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
				Load_Title_Page(true);
				CCPalette.Set();

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				// init font variables

				Fancy_Text_Print(TXT_NONE, 0, 0, scheme, TBLACK, TPF_TEXT);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Draw_Caption (TXT_MODEM_INITIALISATION, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print( TXT_DATA_COMPRESSION,
					d_compression_x - 26, d_compression_y + 2,
					scheme, TBLACK, TPF_TEXT | TPF_RIGHT);

				Fancy_Text_Print( TXT_ERROR_CORRECTION,
					d_errorcorrection_x - 26, d_errorcorrection_y +2,
					scheme, TBLACK, TPF_TEXT | TPF_RIGHT);

				Fancy_Text_Print( TXT_HARDWARE_FLOW_CONTROL,
					d_hardwareflowcontrol_x -26, d_hardwareflowcontrol_y +2,
					scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
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
				if (display < REDRAW_BUTTONS) display = REDRAW_BUTTONS;
				break;

			case (BUTTON_ERROR_CORRECTION | KN_BUTTON):
				settings->ErrorCorrection = settings->ErrorCorrection ^ 1;
				strcpy (correction_text, settings->ErrorCorrection ?
										Text_String (TXT_ON) : Text_String (TXT_OFF) );
				if (display < REDRAW_BUTTONS) display = REDRAW_BUTTONS;
				break;

			case (BUTTON_HARDWARE_FLOW_CONTROL | KN_BUTTON):
				settings->HardwareFlowControl = settings->HardwareFlowControl ^ 1;
				strcpy (flowcontrol_text, settings->HardwareFlowControl ?
										Text_String (TXT_ON) : Text_String (TXT_OFF) );
				if (display < REDRAW_BUTTONS) display = REDRAW_BUTTONS;
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
#endif	//WIN32



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

	/*
	** Dialog & button dimensions
	*/
	int d_dialog_w = 320 * RESFACTOR;									// dialog width
	int d_dialog_h = 200 * RESFACTOR;									// dialog height
	int d_dialog_x = ((320 * RESFACTOR - d_dialog_w) / 2);		// dialog x-coord
	int d_dialog_y = ((200 * RESFACTOR - d_dialog_h) / 2);		// dialog y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 7 * RESFACTOR;										// ht of 6-pt text
	int d_margin = 5 * RESFACTOR;										// margin width/height

	int d_portlist_w = 80 * RESFACTOR + 80*(RESFACTOR-1);	//Port list wider in hires
	int d_portlist_h = 33 * RESFACTOR;
	int d_portlist_x = d_dialog_x + (d_dialog_w / 6) - (d_portlist_w / 2);
#ifdef WIN32
 	d_portlist_x = 0x45;
#endif
	int d_portlist_y = d_dialog_y + ((d_margin + d_txt6_h) * 2) + d_margin + 10 * RESFACTOR;

#ifdef WIN32
	int d_port_w = d_portlist_w;
	int d_port_x = 0x45;
#else
	int d_port_w = (5 * 6 * RESFACTOR) + 3 * RESFACTOR;

//	int d_port_x = d_portlist_x + 29 * RESFACTOR;
#ifdef FRENCH  //VG2
	int d_port_x = (d_portlist_x + 29 * RESFACTOR) + 5;
#else
	int d_port_x = d_portlist_x + 29 * RESFACTOR;
#endif

#endif	//WIN32
	int d_port_h = 9 * RESFACTOR;
	int d_port_y = d_portlist_y - d_margin - d_txt6_h;

	int d_irqlist_w = 80 * RESFACTOR;
	int d_irqlist_h = 33 * RESFACTOR;
	int d_irqlist_x = d_dialog_x + (d_dialog_w / 2) - (d_irqlist_w / 2);
	int d_irqlist_y = d_portlist_y;

	int d_irq_w = ((IRQBUF_MAX - 1) * 6 * RESFACTOR) + 3 * RESFACTOR;
	int d_irq_h = 9 * RESFACTOR;
	int d_irq_x = d_irqlist_x + 25 * RESFACTOR;
	int d_irq_y = d_irqlist_y - d_margin - d_txt6_h;

	int d_baudlist_w = 80 * RESFACTOR;
	int d_baudlist_h = 33 * RESFACTOR;
	int d_baudlist_x = d_dialog_x + ((d_dialog_w * 5) / 6) - (d_baudlist_w / 2);
#ifdef WIN32
 d_baudlist_x -= 32;
#endif
	int d_baudlist_y = d_irqlist_y;

	int d_baud_w = ((BAUDBUF_MAX - 1) * 6 * RESFACTOR) + 3 * RESFACTOR;
	int d_baud_h = 9 * RESFACTOR;
	int d_baud_x = d_baudlist_x + 29 * RESFACTOR;
	int d_baud_y = d_baudlist_y - d_margin - d_txt6_h;

	int d_initstrlist_w = ((INITSTRBUF_MAX - 1) * 6 * RESFACTOR) + 8 * RESFACTOR + 3 * RESFACTOR;
	int d_initstrlist_h = 21 * RESFACTOR;
	int d_initstrlist_x = d_dialog_cx - (d_initstrlist_w / 2);
	int d_initstrlist_y = d_portlist_y + d_portlist_h + ((d_margin + d_txt6_h) * 2) + d_margin + 4;

	int d_initstr_w = ((INITSTRBUF_MAX - 1) * 6 * RESFACTOR) + 3 * RESFACTOR;
	int d_initstr_h = 9 * RESFACTOR;
	int d_initstr_x = d_initstrlist_x;
	int d_initstr_y = d_initstrlist_y - d_margin - d_txt6_h;

	int d_add_w = 45 * RESFACTOR;
#ifdef FRENCH
	int d_add_x = d_dialog_cx - (d_add_w / 2);
#else
	int d_add_x = (d_dialog_cx - (d_add_w / 2)) + 30;
#endif
	int d_add_h = 9 * RESFACTOR;
	int d_add_y = d_initstr_y - d_add_h - 3 * RESFACTOR;

	int d_delete_w = 45 * RESFACTOR;
#ifdef FRENCH
	int d_delete_x = (d_dialog_x + ((d_dialog_w * 3) / 4) - (d_delete_w / 2)) + 10;
#else
	int d_delete_x = d_dialog_x + ((d_dialog_w * 3) / 4) - (d_delete_w / 2);
#endif
	int d_delete_h = 9 * RESFACTOR;
	int d_delete_y = d_initstr_y - d_add_h - 3 * RESFACTOR;

	int d_cwaitstrlist_w = (((CWAITSTRBUF_MAX - 1) + 9) * 6) * RESFACTOR + 3 * RESFACTOR;
	int d_cwaitstrlist_h = 27 * RESFACTOR;
	int d_cwaitstrlist_x = d_initstrlist_x;
	int d_cwaitstrlist_y = d_initstrlist_y + d_initstrlist_h + ((d_margin + d_txt6_h) * 2) + 2;

	int d_cwaitstr_w = ((CWAITSTRBUF_MAX - 1) * 6) * RESFACTOR + 3 * RESFACTOR;
	int d_cwaitstr_h = 9 * RESFACTOR;
	int d_cwaitstr_x = d_cwaitstrlist_x;
	int d_cwaitstr_y = d_cwaitstrlist_y - d_margin - d_txt6_h;

	int d_tone_w = 80 * RESFACTOR;
	int d_tone_h = 9 * RESFACTOR;
	int d_tone_x = d_dialog_x + ((d_dialog_w * 3) / 4) - (d_tone_w / 2);
	int d_tone_y = d_cwaitstrlist_y;

	int d_pulse_w = 80 * RESFACTOR;
	int d_pulse_h = 9 * RESFACTOR;
	int d_pulse_x = d_dialog_x + ((d_dialog_w * 3) / 4) - (d_pulse_w / 2);
	int d_pulse_y = d_tone_y + d_tone_h + d_margin;

#ifdef FRENCH
	int d_save_w = 80 * RESFACTOR;
#else
	int d_save_w = 40 * RESFACTOR;
#endif
	int d_save_h = 9 * RESFACTOR;
	int d_save_x = d_dialog_x + (d_dialog_w / 5) - (d_save_w / 2);
	int d_save_y = d_dialog_y + d_dialog_h - d_save_h - d_margin - 4*RESFACTOR;

	int d_cancel_w = 50 * RESFACTOR;
	int d_cancel_h = 9 * RESFACTOR;
	int d_cancel_x = d_dialog_x + ((d_dialog_w * 4) / 5) - (d_cancel_w / 2);
	int d_cancel_y = d_dialog_y + d_dialog_h - d_cancel_h - d_margin - 4*RESFACTOR;

#if (GERMAN | FRENCH)
	int d_advanced_w = 50*RESFACTOR;
#else
	int d_advanced_w = 40*RESFACTOR;
#endif
	int d_advanced_h = 9*RESFACTOR;
	int d_advanced_x = d_dialog_x + ((d_dialog_w) / 2) - (d_advanced_w / 2);
	int d_advanced_y = d_dialog_y + d_dialog_h - d_advanced_h - d_margin - 4 *RESFACTOR;

	/*
	** Button Enumerations
	*/
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
		BUTTON_CANCEL,
	};

	/*
	** Redraw values: in order from "top" to "bottom" layer of the dialog
	*/
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
		"COM4 - 2E8",
	};

	static char custom_port[10 + MODEM_NAME_MAX] = {"CUSTOM - ????"};



#ifndef WIN32		// No IRQ dialog in Win version
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

#endif	//WIN32

#ifdef WIN32
	static char *baudname[5] = {
		"14400",
		"19200",
		"28800",
		"38400",
		"57600",
	};

	static char modemnames[10][MODEM_NAME_MAX];

#else	//WIN32
	static char *baudname[5] = {
		"9600",
		"14400",
		"19200",
		"28800",
		"38400"
	};
#endif	//WIN32
	/*
	** Dialog variables
	*/
	RedrawType display = REDRAW_ALL;		// redraw level
	bool process = true;						// process while true
	KeyNumType input;
	char * item;								// general-purpose string
	char * temp;								// general-purpose string
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	char portbuf[ PORTBUF_MAX ] = {0};	// buffer for port
	char irqbuf[ IRQBUF_MAX ] = {0};		// buffer for irq
	char baudbuf[ BAUDBUF_MAX ] = {0};	// buffer for baud
	char initstrbuf[ INITSTRBUF_MAX ] = {0};		// buffer for init string
	char cwaitstrbuf[ CWAITSTRBUF_MAX ] = {0};	// buffer for call waiting string

	int port_index = 1;	// index of currently-selected port (default = com2)
	int port_custom_index = 4;	//index of custom entry in port list
	int irq_index = 1;	// index of currently-selected irq (default = 3)
	int baud_index = 1;	// index of currently-selected baud (default = 19200)
	int initstr_index = 0;	// index of currently-selected modem init (default = "ATZ")
	int cwaitstr_index = CALL_WAIT_CUSTOM;	// index of currently-selected call waiting (default = "")
	int rc = 0;									// -1 = user cancelled, 1 = New
	int i;										// loop counter
	int pos;
	int len;
	bool firsttime = true;
	SerialSettingsType tempsettings;
	DetectPortType dpstatus;

	/*
	** Buttons
	*/
	GadgetClass *commands;										// button list

	EditClass port_edt (BUTTON_PORT, portbuf, PORTBUF_MAX,
		TPF_TEXT, d_port_x, d_port_y, d_port_w, d_port_h, EditClass::ALPHANUMERIC);

	ListClass portlist(BUTTON_PORTLIST, d_portlist_x, d_portlist_y, d_portlist_w, d_portlist_h,
		TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
#ifndef WIN32
	EditClass irq_edt (BUTTON_IRQ, irqbuf, IRQBUF_MAX, TPF_TEXT, d_irq_x, d_irq_y, d_irq_w, d_irq_h, EditClass::NUMERIC);

	ListClass irqlist(BUTTON_IRQLIST, d_irqlist_x, d_irqlist_y, d_irqlist_w, d_irqlist_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
#endif	//WIN32

	EditClass baud_edt (BUTTON_BAUD, baudbuf, BAUDBUF_MAX, TPF_TEXT, d_baud_x, d_baud_y, d_baud_w, d_baud_h, EditClass::NUMERIC);
	ListClass baudlist(BUTTON_BAUDLIST, d_baudlist_x, d_baudlist_y, d_baudlist_w, d_baudlist_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
	EditClass initstr_edt (BUTTON_INITSTR, initstrbuf, INITSTRBUF_MAX, TPF_TEXT, d_initstr_x, d_initstr_y, d_initstr_w, d_initstr_h, EditClass::ALPHANUMERIC);
	ListClass initstrlist(BUTTON_INITSTRLIST, d_initstrlist_x, d_initstrlist_y, d_initstrlist_w, d_initstrlist_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
	TextButtonClass addbtn(BUTTON_ADD, TXT_ADD, TPF_BUTTON, d_add_x, d_add_y, d_add_w, d_add_h);
	TextButtonClass deletebtn(BUTTON_DELETE, TXT_DELETE_BUTTON, TPF_BUTTON, d_delete_x, d_delete_y, d_delete_w, d_delete_h);
	EditClass cwaitstr_edt (BUTTON_CWAITSTR, cwaitstrbuf, CWAITSTRBUF_MAX, TPF_TEXT, d_cwaitstr_x, d_cwaitstr_y, d_cwaitstr_w, d_cwaitstr_h, EditClass::ALPHANUMERIC);
	ListClass cwaitstrlist(BUTTON_CWAITSTRLIST, d_cwaitstrlist_x, d_cwaitstrlist_y, d_cwaitstrlist_w, d_cwaitstrlist_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
	TextButtonClass tonebtn(BUTTON_TONE, TXT_TONE_BUTTON, TPF_BUTTON, d_tone_x, d_tone_y, d_tone_w, d_tone_h);
	TextButtonClass pulsebtn(BUTTON_PULSE, TXT_PULSE_BUTTON, TPF_BUTTON, d_pulse_x, d_pulse_y, d_pulse_w, d_pulse_h);
	TextButtonClass savebtn(BUTTON_SAVE, TXT_SAVE_BUTTON, TPF_BUTTON, d_save_x, d_save_y, d_save_w, d_save_h);
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
#ifdef WIN32
	TextButtonClass advancedbutton(BUTTON_ADVANCED, TXT_ADVANCED, TPF_BUTTON, d_advanced_x, d_advanced_y, d_advanced_w, d_advanced_h);
#endif	//WIN32
	/*
	** Various Inits
	*/
	memcpy( &tempsettings, settings, sizeof(SerialSettingsType) );

	if (tempsettings.Port == 0) {
		tempsettings.Port = 0x2f8;
	}

	if (tempsettings.IRQ == -1) {
		tempsettings.IRQ = 3;
	}

	if (tempsettings.Baud == -1) {
#ifdef WIN32
		tempsettings.Baud = 19200;
#else	//WIN32
		tempsettings.Baud = 9600;
#endif	//WIN32
	}


	/*
	** Set the current indices
	*/
#ifndef WIN32
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
#endif	//WIN32

#ifdef WIN32
	if (tempsettings.Baud == 14400) {
		baud_index = 0;
	} else if (tempsettings.Baud == 19200) {
		baud_index = 1;
	} else if (tempsettings.Baud == 28800) {
		baud_index = 2;
	} else if (tempsettings.Baud == 38400) {
		baud_index = 3;
	} else {
		baud_index = 4;
	}
#else	//WIN32
	if (tempsettings.Baud == 9600) {
		baud_index = 0;
	} else if (tempsettings.Baud == 14400) {
		baud_index = 1;
	} else if (tempsettings.Baud == 19200) {
		baud_index = 2;
	} else if (tempsettings.Baud == 28800) {
		baud_index = 3;
	} else {
		baud_index = 4;
	}
#endif	//WIN32
	sprintf (baudbuf, "%d", tempsettings.Baud);

	/*
	** Set up the port list box & edit box
	*/
	for (i = 0; i < 4; i++) {
		portlist.Add_Item( portname[ i ] );
	}

#ifdef WIN32
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

#endif	//WIN32

	portlist.Add_Item ( custom_port );


	/*
	** Work out the current port index
	*/
	port_index = -1;
#ifdef WIN32
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
#endif	//WIN32

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
#ifndef WIN32
	/*
	** Set up the IRQ list box & edit box
	*/
	for (i = 0; i < 5; i++) {
		irqlist.Add_Item( irqname[ i ] );
	}

	irqlist.Set_Selected_Index( irq_index );
	irq_edt.Set_Text( irqbuf, IRQBUF_MAX );
#endif	//WIN32
	/*
	** Set up the baud rate list box & edit box
	*/
	for (i = 0; i < 5; i++) {
		baudlist.Add_Item( baudname[ i ] );
	}

	baudlist.Set_Selected_Index( baud_index );
	baud_edt.Set_Text( baudbuf, BAUDBUF_MAX );

	initstr_index = tempsettings.InitStringIndex;
	Build_Init_String_Listbox(&initstrlist, &initstr_edt, initstrbuf, &initstr_index);

	/*
	** Set up the cwait rate list box & edit box
	*/
	cwaitstr_index = tempsettings.CallWaitStringIndex;
	for (i = 0; i < CALL_WAIT_STRINGS_NUM; i++) {
		if ( i == CALL_WAIT_CUSTOM ) {
			item = Session.CallWaitStrings[ i ];
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
		} else if (i == cwaitstr_index) {
			strncpy( cwaitstrbuf, Session.CallWaitStrings[ i ], CWAITSTRBUF_MAX );
		}
		cwaitstrlist.Add_Item( Session.CallWaitStrings[ i ] );
	}

	cwaitstrlist.Set_Selected_Index( cwaitstr_index );
	cwaitstr_edt.Set_Text( cwaitstrbuf, CWAITSTRBUF_MAX );

	/*
	** Build the button list
	*/
	commands = &cancelbtn;
	port_edt.Add_Tail(*commands);
	portlist.Add_Tail(*commands);
#ifndef WIN32
	irq_edt.Add_Tail(*commands);
	irqlist.Add_Tail(*commands);
#endif	//WIN32
	baud_edt.Add_Tail(*commands);
	baudlist.Add_Tail(*commands);
	initstr_edt.Add_Tail(*commands);
	initstrlist.Add_Tail(*commands);
	addbtn.Add_Tail(*commands);
	deletebtn.Add_Tail(*commands);
	cwaitstr_edt.Add_Tail(*commands);
	cwaitstrlist.Add_Tail(*commands);
	tonebtn.Add_Tail(*commands);
	pulsebtn.Add_Tail(*commands);
	savebtn.Add_Tail(*commands);
#ifdef WIN32
	advancedbutton.Add_Tail(*commands);
#endif	//WIN32

	if (tempsettings.DialMethod == DIAL_TOUCH_TONE) {
		tonebtn.Turn_On();
	} else {
		pulsebtn.Turn_On();
	}

	/*
	** Processing loop
	*/
	while (process) {
		/*
		** Invoke game callback
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

#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}
#endif
		/*
		** Refresh display if needed
		*/
		if (display) {
			Hide_Mouse();
			/*
			** Redraw backgound & dialog box
			*/
			if (display >= REDRAW_BACKGROUND) {
				Load_Title_Page(true);
				CCPalette.Set();

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				// init font variables

				Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_CENTER | TPF_TEXT);

				/*
				** Dialog & Field labels
				*/
				Draw_Caption(TXT_SETTINGS, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print(TXT_PORT_COLON, d_port_x - 3 * RESFACTOR, d_port_y + 1 * RESFACTOR, scheme, TBLACK, TPF_RIGHT | TPF_TEXT);
#ifndef WIN32
				Fancy_Text_Print(TXT_IRQ_COLON, d_irq_x - 3 * RESFACTOR, d_irq_y + 1 * RESFACTOR, scheme, TBLACK, TPF_RIGHT | TPF_TEXT);
#endif	//WIN32
				Fancy_Text_Print(TXT_BAUD_COLON, d_baud_x - 3 * RESFACTOR, d_baud_y + 1 * RESFACTOR, scheme, TBLACK, TPF_RIGHT | TPF_TEXT);

				Fancy_Text_Print(TXT_INIT_STRING, d_initstr_x, d_initstr_y - d_txt6_h - 3 * RESFACTOR, scheme, TBLACK, TPF_TEXT);

				Fancy_Text_Print(TXT_CWAIT_STRING, d_cwaitstr_x, d_cwaitstr_y - d_txt6_h - 3 * RESFACTOR, scheme, TBLACK, TPF_TEXT);

			}

			/*
			** Redraw buttons
			*/
			if (display >= REDRAW_BUTTONS) {
				cancelbtn.Flag_To_Redraw();
				port_edt.Flag_To_Redraw();
				portlist.Flag_To_Redraw();
#ifndef WIN32
				irq_edt.Flag_To_Redraw();
				irqlist.Flag_To_Redraw();
#endif	//WIN32
				baud_edt.Flag_To_Redraw();
				baudlist.Flag_To_Redraw();
#ifdef WIN32
				advancedbutton.Flag_To_Redraw();
#endif	//WIN32
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
		** Get user input
		*/
		input = commands->Input();

		if ( firsttime ) {
//			port_edt.Set_Focus();
			port_edt.Flag_To_Redraw();
			input = commands->Input();
			firsttime = false;
		}

		/*
		** Process input
		*/
		switch (input) {

#ifdef WIN32
			case (BUTTON_ADVANCED | KN_BUTTON):
				Advanced_Modem_Settings (&tempsettings);
				display = REDRAW_ALL;
				break;
#endif	//WIN32

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
#ifndef WIN32
					irq_edt.Set_Focus();
					irq_edt.Flag_To_Redraw();
#endif	//WIN32
				} else {
					strupr( portbuf );
					if ( stricmp(portbuf, "3F8") == 0 ) {
						port_index = 0;
						portlist.Set_Selected_Index( port_index );
						strcpy (portbuf, "COM1");
						display = REDRAW_BUTTONS;

					} else if ( stricmp(portbuf, "2F8") == 0 ) {
						port_index = 1;
						portlist.Set_Selected_Index( port_index );
						strcpy (portbuf, "COM2");
						display = REDRAW_BUTTONS;

					} else if ( stricmp(portbuf, "3E8") == 0 ) {
						port_index = 2;
						portlist.Set_Selected_Index( port_index );
						strcpy (portbuf, "COM3");
						display = REDRAW_BUTTONS;

					} else if ( stricmp(portbuf, "2E8") == 0 ) {
						port_index = 3;
						portlist.Set_Selected_Index( port_index );
						strcpy (portbuf, "COM4");
						display = REDRAW_BUTTONS;

					} else if ( strncmp(portbuf, "COM", 3) == 0 ) {
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
#ifdef WIN32
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
#endif	//WIN32
								WWMessageBox().Process( TXT_INVALID_PORT_ADDRESS );
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
#ifndef WIN32
					if (display == REDRAW_BUTTONS) {
						irq_edt.Set_Focus();
						irq_edt.Flag_To_Redraw();
					}
#endif	//WIN32
				}
				break;

			case (BUTTON_PORTLIST | KN_BUTTON):
				if (portlist.Current_Index() != port_index) {
					port_index = portlist.Current_Index();
					item = (char *)portlist.Current_Item();
					if (port_index < 4) {
						temp = strchr( item, ' ' );
						pos = (int)(temp - item);
						strncpy( portbuf, item, pos );
						portbuf[pos] = 0;
						port_edt.Clear_Focus();

						// auto select the irq for port
#ifndef WIN32
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
#endif	//WIN32
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
						} else {
							/*
							** Must be a modem name entry so just copy iy
							*/
							strncpy (portbuf, item, PORTBUF_MAX);
						}
					}
					port_edt.Set_Text( portbuf, PORTBUF_MAX );
				} else if (port_index < port_custom_index) {
					port_edt.Clear_Focus();
				} else {
					port_edt.Set_Focus();
				}
				display = REDRAW_BUTTONS;
				break;

#ifndef WIN32
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
				} else if (irq_index < 4) {
					irq_edt.Clear_Focus();
				} else {
					irq_edt.Set_Focus();
				}
				display = REDRAW_BUTTONS;
				break;
#endif	//WIN32
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
				strncpy( Session.InitStrings[ initstr_index ], initstrbuf, INITSTRBUF_MAX );
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

			/*
			** Add a new InitString entry
			*/
			case (BUTTON_ADD | KN_BUTTON):

				item = new char[ INITSTRBUF_MAX ];
				memset (item, 0, INITSTRBUF_MAX);

				strupr ( initstrbuf );
				strncpy ( item, initstrbuf, INITSTRBUF_MAX-1 );

				Session.InitStrings.Add ( item );
				Build_Init_String_Listbox (&initstrlist, &initstr_edt, initstrbuf,
					&initstr_index);
				/*............................................................
				Set the current listbox index to the newly-added item.
				............................................................*/
				for (i = 0; i < Session.InitStrings.Count(); i++) {
					if (item == Session.InitStrings[i]) {
						initstr_index = i;
						strcpy( initstrbuf, Session.InitStrings[ initstr_index ] );
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

				if ( Session.InitStrings.Count() && initstr_index != -1) {
					Session.InitStrings.Delete( initstr_index );
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
				} else if (cwaitstr_index < 3) {
					cwaitstr_edt.Clear_Focus();
				} else {
					cwaitstr_edt.Set_Focus();
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
#ifdef WIN32
							strncpy ( tempsettings.ModemName, portbuf, MODEM_NAME_MAX );
							tempsettings.Port = 1;
#else	//WIN32
							sscanf( portbuf, "%x", &tempsettings.Port );
							tempsettings.ModemName[0] = 0;
#endif	//WIN32
						} else {
							/*
							** Must be a modem name index
							*/
							strcpy (tempsettings.ModemName, portlist.Current_Item());
							tempsettings.Port = 1;
						}
						break;
				}


#ifndef WIN32
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
#endif	//WIN32
				sscanf( baudbuf, "%d", &tempsettings.Baud );

				tempsettings.InitStringIndex = initstr_index;
				tempsettings.CallWaitStringIndex = cwaitstr_index;

				item = Session.CallWaitStrings[ CALL_WAIT_CUSTOM ];
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

				} else if (dpstatus == PORT_INVALID) {
#ifdef WIN32
					WWMessageBox().Process( TXT_UNABLE_TO_OPEN_PORT );
#else	//WIN32
					WWMessageBox().Process( TXT_INVALID_SETTINGS );
#endif	//WIN32
					firsttime = true;
					display = REDRAW_ALL;

				} else if (dpstatus == PORT_IRQ_INUSE) {
					WWMessageBox().Process( TXT_IRQ_ALREADY_IN_USE );
					firsttime = true;
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
	Load_Title_Page(true);
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
	qsort ((void *)(&Session.InitStrings[0]), Session.InitStrings.Count(), sizeof(char *), Init_String_Compare);

	/*........................................................................
	Build the list
	........................................................................*/
	for (i = 0; i < Session.InitStrings.Count(); i++) {
		item = new char[ INITSTRBUF_MAX ];
		strcpy( item, Session.InitStrings[i] );
		list->Add_Item(item);
	}
	list->Flag_To_Redraw();

	/*........................................................................
	Init the current phone book index
	........................................................................*/
	if (list->Count() == 0 || curidx < -1) {
		curidx = -1;
	} else if (curidx >= list->Count() ) {
		curidx = 0;
	}

	/*........................................................................
	Fill in initstring edit buffer
	........................................................................*/
	if (curidx > -1) {
		strcpy (buf, Session.InitStrings[ curidx ]);
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
 * INPUT:                                                                  						  *
 *		none.																												  *
 *                                                                         						  *
 * OUTPUT:                                                                 						  *
 *		true = success, false = cancel																			  *
 *                                                                         						  *
 * WARNINGS:                                                               						  *
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.
 *   01/21/97 V.Grippi added check for CS before sending scenario file                                             						  *
 *=============================================================================================*/
int Com_Scenario_Dialog(bool skirmish)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	int d_dialog_w = 320 * RESFACTOR;									// dialog width
	int d_dialog_h = 200 * RESFACTOR;									// dialog height
	int d_dialog_x = ((320 * RESFACTOR - d_dialog_w) / 2);		// dialog x-coord
	int d_dialog_y = ((200 * RESFACTOR - d_dialog_h) / 2);		// dialog y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 6 * RESFACTOR+1;										// ht of 6-pt text
	int d_margin1 = 5 * RESFACTOR;										// margin width/height
	int d_margin2 = 7 * RESFACTOR;										// margin width/height

	int d_name_w = 70 * RESFACTOR;
	int d_name_h = 9 * RESFACTOR;
	int d_name_x = d_dialog_x + (d_dialog_w / 4) - (d_name_w / 2);
	int d_name_y = d_dialog_y + d_margin2 + d_txt6_h + 1 * RESFACTOR;

#ifdef OLDWAY
	int d_gdi_w = 40 * RESFACTOR;
	int d_gdi_h = 9 * RESFACTOR;
	int d_gdi_x = d_dialog_cx - d_gdi_w;
	int d_gdi_y = d_name_y;

	int d_nod_w = 40 * RESFACTOR;
	int d_nod_h = 9 * RESFACTOR;
	int d_nod_x = d_dialog_cx;
	int d_nod_y = d_name_y;
#else
	int d_house_w = 60 *RESFACTOR;
	int d_house_h = (8 * 5 *RESFACTOR);
	int d_house_x = d_dialog_cx - (d_house_w / 2);
	int d_house_y = d_name_y;
#endif

	int d_color_w = 10 * RESFACTOR;
	int d_color_h = 9 * RESFACTOR;
	int d_color_y = d_name_y;
	int d_color_x = d_dialog_x + ((d_dialog_w / 4) * 3) - (d_color_w * 3);

	int d_playerlist_w = 118 * RESFACTOR;
	int d_playerlist_h = (6 * 6 * RESFACTOR) + 3 * RESFACTOR;			// 6 rows high
	int d_playerlist_x = d_dialog_x + d_margin1 + d_margin1 + 5*RESFACTOR;
	int d_playerlist_y = d_color_y + d_color_h + d_margin2 + 2*RESFACTOR/*KO + d_txt6_h*/;

	int d_scenariolist_w = 162 * RESFACTOR;
	int d_scenariolist_h = (6 * 6 * RESFACTOR) + 3 * RESFACTOR;		// 6 rows high

	if (skirmish) {
		d_scenariolist_h *= 2;
	}


	int d_scenariolist_x = d_dialog_x + d_dialog_w - d_margin1 - d_margin1 - d_scenariolist_w - 5*RESFACTOR;
	int d_scenariolist_y = d_color_y + d_color_h + d_margin2 + 2*RESFACTOR;

	if (skirmish) {
		d_scenariolist_x = d_dialog_x + (d_dialog_w-d_scenariolist_w)/2;
	}

	int d_count_w = 25 * RESFACTOR;
	int d_count_h = d_txt6_h;
	int d_count_x = d_playerlist_x + (d_playerlist_w / 2) + 20 * RESFACTOR;		// (fudged)
	int d_count_y = d_playerlist_y + d_playerlist_h + (d_margin1 * 2) - 2 * RESFACTOR;

	if (skirmish) {
		d_count_y = d_scenariolist_y + d_scenariolist_h + d_margin1 - 2*RESFACTOR;
	}

	int d_level_w = 25 * RESFACTOR;
	int d_level_h = d_txt6_h;
	int d_level_x = d_playerlist_x + (d_playerlist_w / 2) + 20 * RESFACTOR;		// (fudged)
	int d_level_y = d_count_y + d_count_h;

	int d_credits_w = 25 * RESFACTOR;
	int d_credits_h = d_txt6_h;
	int d_credits_x = d_playerlist_x + (d_playerlist_w / 2) + 20 * RESFACTOR;	// (fudged)
	int d_credits_y = d_level_y + d_level_h;

	int d_aiplayers_w = 25*RESFACTOR;
	int d_aiplayers_h = d_txt6_h;
	int d_aiplayers_x = d_playerlist_x + (d_playerlist_w / 2) + 20*RESFACTOR;	// (fudged)
	int d_aiplayers_y = d_credits_y + d_credits_h;

	int d_options_w = 106 * RESFACTOR;
	int d_options_h = (5 * 6* RESFACTOR) + 4*RESFACTOR;
	int d_options_x = d_dialog_x + d_dialog_w - 149 * RESFACTOR;
	int d_options_y = d_scenariolist_y + d_scenariolist_h + d_margin1 - 2*RESFACTOR;

	int d_message_w = d_dialog_w - (d_margin1 * 2) - 20*RESFACTOR;
	int d_message_h = (8 * d_txt6_h) + 3 * RESFACTOR;		// 4 rows high
	int d_message_x = d_dialog_x + d_margin1 + 10*RESFACTOR;
	int d_message_y = d_options_y + d_options_h + 2*RESFACTOR;

	int d_send_w = d_dialog_w - (d_margin1 * 2) - 20*RESFACTOR;
	int d_send_h = 9 * RESFACTOR;
	int d_send_x = d_dialog_x + d_margin1 + 10*RESFACTOR;
	int d_send_y = d_message_y + d_message_h;

	int d_ok_w = 45 * RESFACTOR;
	int d_ok_h = 9 * RESFACTOR;
	int d_ok_x = d_dialog_x + (d_dialog_w / 6) - (d_ok_w / 2);
	int d_ok_y = d_dialog_y + d_dialog_h - d_ok_h - d_margin1 - RESFACTOR*6;

	int d_cancel_w = 45 * RESFACTOR;
	int d_cancel_h = 9 * RESFACTOR;
	int d_cancel_x = d_dialog_cx - (d_cancel_w / 2);
	int d_cancel_y = d_dialog_y + d_dialog_h - d_cancel_h - d_margin1 - RESFACTOR*6;

	int d_load_w = 45 * RESFACTOR;
	int d_load_h = 9 * RESFACTOR;
	int d_load_x = d_dialog_x + ((d_dialog_w * 5) / 6) - (d_load_w / 2);
	int d_load_y = d_dialog_y + d_dialog_h - d_load_h - d_margin1 - RESFACTOR*6;


	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_NAME = 100,
#ifdef OLDWAY
		BUTTON_GDI,
		BUTTON_NOD,
#else
		BUTTON_HOUSE,
#endif
		BUTTON_CREDITS,
		BUTTON_AIPLAYERS,
		BUTTON_OPTIONS,
		BUTTON_PLAYERLIST,
		BUTTON_SCENARIOLIST,
		BUTTON_COUNT,
		BUTTON_LEVEL,
		BUTTON_OK,
		BUTTON_LOAD,
		BUTTON_CANCEL,
		BUTTON_DIFFICULTY,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_PARMS,
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

	int playertabs[] = {77*RESFACTOR};	// tabs for player list box
	int optiontabs[] = {8};					// tabs for player list box
	char namebuf[MPLAYER_NAME_MAX] = {0};		// buffer for player's name
	bool transmit;								// 1 = re-transmit new game options
	int cbox_x[] = {
							d_color_x,
							d_color_x + d_color_w,
							d_color_x + (d_color_w * 2),
							d_color_x + (d_color_w * 3),
							d_color_x + (d_color_w * 4),
							d_color_x + (d_color_w * 5),
							d_color_x + (d_color_w * 6),
							d_color_x + (d_color_w * 7)
							};
	bool parms_received = false;					// 1 = game options received
	bool changed = false;							// 1 = user has changed an option

	int rc;
	int recsignedoff = false;
	int i;
	unsigned long version;
	unsigned long starttime;
	unsigned long timingtime;
	unsigned long lastmsgtime;
	unsigned long lastredrawtime;
	unsigned long transmittime = 0;
	unsigned long theirresponsetime;
	int packetlen;
	static bool first_time = true;
	bool oppscorescreen = false;
	bool gameoptions = Session.Type == GAME_SKIRMISH;
	EventClass *event;					// event ptr
	unsigned long msg_timeout = 1200;	// init to 20 seconds

	CCFileClass loadfile ("SAVEGAME.NET");
	bool load_game = false;					// 1 = load a saved game
	NodeNameType *who;					// node to add to Players
	char *item;								// for filling in lists
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();
	bool messages_have_focus = true;		// Gadget focus starts on the message system

	Set_Logic_Page(SeenBuff);

	CDTimerClass<SystemTimerClass> kludge_timer;		// Timer to allow a wait after client joins
																	// game before game can start
	bool ok_button_added = false;

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass * commands;										// button list

	EditClass name_edt(BUTTON_NAME, namebuf, MPLAYER_NAME_MAX, TPF_TEXT, d_name_x, d_name_y, d_name_w, d_name_h, EditClass::ALPHANUMERIC);

#ifdef OLDWAY
	TextButtonClass gdibtn(BUTTON_GDI, TXT_ALLIES, TPF_BUTTON, d_gdi_x, d_gdi_y, d_gdi_w, d_gdi_h);
	TextButtonClass nodbtn(BUTTON_NOD, TXT_SOVIET, TPF_BUTTON, d_nod_x, d_nod_y, d_nod_w, d_nod_h);
#else
	char housetext[25] = "";
	Fancy_Text_Print("", 0, 0, 0, 0, TPF_TEXT);
	DropListClass housebtn(BUTTON_HOUSE, housetext, sizeof(housetext),
		TPF_TEXT,
		d_house_x, d_house_y, d_house_w, d_house_h,
		MFCD::Retrieve("BTN-UP.SHP"),
		MFCD::Retrieve("BTN-DN.SHP"));
#endif
	ColorListClass playerlist(BUTTON_PLAYERLIST, d_playerlist_x, d_playerlist_y, d_playerlist_w, d_playerlist_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
	ListClass scenariolist(BUTTON_SCENARIOLIST, d_scenariolist_x, d_scenariolist_y, d_scenariolist_w, d_scenariolist_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
	GaugeClass countgauge(BUTTON_COUNT, d_count_x, d_count_y, d_count_w, d_count_h);

	char staticcountbuff[35];
	StaticButtonClass staticcount(0, "     ", TPF_TEXT, d_count_x+d_count_w+3*RESFACTOR, d_count_y);

	GaugeClass levelgauge(BUTTON_LEVEL, d_level_x, d_level_y, d_level_w, d_level_h);

	char staticlevelbuff[35];
	StaticButtonClass staticlevel(0, "     ", TPF_TEXT, d_level_x+d_level_w+3*RESFACTOR, d_level_y);

	GaugeClass creditsgauge(BUTTON_CREDITS, d_credits_x, d_credits_y, d_credits_w, d_credits_h);

	char staticcreditsbuff[35];
	StaticButtonClass staticcredits(0, "         ", TPF_TEXT, d_credits_x+d_credits_w+3*RESFACTOR, d_credits_y);

	GaugeClass aiplayersgauge(BUTTON_AIPLAYERS, d_aiplayers_x, d_aiplayers_y, d_aiplayers_w, d_aiplayers_h);

	char staticaibuff[35];
	StaticButtonClass staticai(0, "     ", TPF_TEXT, d_aiplayers_x+d_aiplayers_w+3*RESFACTOR, d_aiplayers_y);

	CheckListClass optionlist(BUTTON_OPTIONS, d_options_x, d_options_y, d_options_w, d_options_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
	TextButtonClass okbtn(BUTTON_OK, TXT_OK, TPF_BUTTON, d_ok_x, d_ok_y, d_ok_w, d_ok_h);
	TextButtonClass loadbtn(BUTTON_LOAD, TXT_LOAD_BUTTON, TPF_BUTTON, d_load_x, d_load_y, d_load_w, d_load_h);
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);

	SliderClass difficulty(BUTTON_DIFFICULTY, d_name_x, optionlist.Y + optionlist.Height + d_margin1 + d_margin1, d_dialog_w - (d_name_x-d_dialog_x)*2, 8*RESFACTOR, true);
	if (Rule.IsFineDifficulty) {
		difficulty.Set_Maximum(5);
		difficulty.Set_Value(2);
	} else {
		difficulty.Set_Maximum(3);
		difficulty.Set_Value(1);
	}

	/*
	------------------------- Build the button list --------------------------
	*/
	commands = &name_edt;
	staticcount.Add_Tail(*commands);
	staticcredits.Add_Tail(*commands);
	staticai.Add_Tail(*commands);
	staticlevel.Add_Tail(*commands);
	if (!skirmish) {
		playerlist.Add_Tail(*commands);
	} else {
		difficulty.Add_Tail(*commands);
	}
	scenariolist.Add_Tail(*commands);
	countgauge.Add_Tail(*commands);
	levelgauge.Add_Tail(*commands);
	creditsgauge.Add_Tail(*commands);
	aiplayersgauge.Add_Tail(*commands);
	optionlist.Add_Tail(*commands);
	if (Session.Type == GAME_SKIRMISH){
		okbtn.Add_Tail(*commands);
	}
	cancelbtn.Add_Tail(*commands);
	if (!skirmish && loadfile.Is_Available()) {
#ifdef FIXIT_MULTI_SAVE
		//Load button added only when other player has arrived
		//loadbtn.Add_Tail(*commands);
#endif
	} else {
		cancelbtn.X = loadbtn.X;
	}
#ifdef OLDWAY
	gdibtn.Add_Tail(*commands);
	nodbtn.Add_Tail(*commands);
#else
	housebtn.Add_Tail(*commands);
#endif


	/*
	----------------------------- Various Inits ------------------------------
	*/
	/*........................................................................
	Init player name & house
	........................................................................*/
	Session.ColorIdx = Session.PrefColor;			// init my preferred color
	strcpy (namebuf, Session.Handle);				// set my name
	name_edt.Set_Text(namebuf,MPLAYER_NAME_MAX);
	name_edt.Set_Color(&ColorRemaps[(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : Session.ColorIdx]);

#ifdef OLDWAY
	if (Session.House==HOUSE_GOOD) {
		gdibtn.Turn_On();
	} else {
		nodbtn.Turn_On();
	}
#else
	for (HousesType house = HOUSE_USSR; house <= HOUSE_FRANCE; house++) {
		housebtn.Add_Item(Text_String(HouseTypeClass::As_Reference(house).Full_Name()));
	}
	housebtn.Set_Selected_Index(Session.House - HOUSE_USSR);
	housebtn.Set_Read_Only (true);
#endif

	/*........................................................................
	Init scenario values, only the first time through
	........................................................................*/
	Special.IsCaptureTheFlag = Rule.IsMPCaptureTheFlag;
	if (first_time) {
		Session.Options.Credits = Rule.MPDefaultMoney;	// init credits & credit buffer
		Session.Options.Bases = Rule.IsMPBasesOn;			// init scenario parameters
		Session.Options.Tiberium = Rule.IsMPTiberiumGrow;
		Session.Options.Goodies = Rule.IsMPCrates;
		Session.Options.AIPlayers = 0;
		Special.IsShadowGrow = Rule.IsMPShadowGrow;
		Session.Options.UnitCount = (SessionClass::CountMax[Session.Options.Bases] + SessionClass::CountMin[Session.Options.Bases]) / 2;
		first_time = false;
	}

	/*........................................................................
	Init button states
	........................................................................*/
	playerlist.Set_Tabs(playertabs);
	playerlist.Set_Selected_Style(ColorListClass::SELECT_NORMAL);

	optionlist.Set_Tabs(optiontabs);
	optionlist.Set_Read_Only(0);

	optionlist.Add_Item(Text_String(TXT_BASES));
	optionlist.Add_Item(Text_String(TXT_ORE_SPREADS));
	optionlist.Add_Item(Text_String(TXT_CRATES));
	optionlist.Add_Item(Text_String(TXT_SHADOW_REGROWS));
	if (!skirmish) {
		optionlist.Add_Item(Text_String(TXT_CAPTURE_THE_FLAG));
	}

	optionlist.Check_Item(0, Session.Options.Bases);
	optionlist.Check_Item(1, Session.Options.Tiberium);
	optionlist.Check_Item(2, Session.Options.Goodies);
	optionlist.Check_Item(3, Special.IsShadowGrow);
	if (!skirmish) {
		optionlist.Check_Item(4, Special.IsCaptureTheFlag);
	}

	countgauge.Set_Maximum(SessionClass::CountMax[Session.Options.Bases] - SessionClass::CountMin[Session.Options.Bases]);
	countgauge.Set_Value(Session.Options.UnitCount - SessionClass::CountMin[Session.Options.Bases]);

	levelgauge.Set_Maximum(MPLAYER_BUILD_LEVEL_MAX - 1);
	levelgauge.Set_Value(BuildLevel - 1);

	creditsgauge.Set_Maximum(Rule.MPMaxMoney);
	creditsgauge.Set_Value(Session.Options.Credits);

	int maxp = Rule.MaxPlayers - 2;
//	int maxp = Rule.MaxPlayers - (skirmish ? 1 : 2);
	aiplayersgauge.Set_Maximum(maxp);

	if (skirmish) {
		if ( Session.Options.AIPlayers > 7 ) {
			Session.Options.AIPlayers = 7;
		}
		Session.Options.AIPlayers = max(Session.Options.AIPlayers, 1);
	}else{
		if ( Session.Options.AIPlayers > 6 ) {
			Session.Options.AIPlayers = 6;
		}
	}

	aiplayersgauge.Set_Value(Session.Options.AIPlayers - (skirmish ? 1 : 0));

	/*........................................................................
	Init other scenario parameters
	........................................................................*/
	Special.IsTGrowth = //Session.Options.Tiberium;
		Rule.IsTGrowth = //Session.Options.Tiberium;
			Special.IsTSpread = //Session.Options.Tiberium;
				Rule.IsTSpread = Session.Options.Tiberium;
	transmit = true;

	/*........................................................................
	Clear the Players vector
	........................................................................*/
	Clear_Vector(&Session.Players);

	/*........................................................................
	Init scenario description list box
	........................................................................*/
	for (i = 0; i < Session.Scenarios.Count(); i++) {
		for (int j = 0; EngMisStr[j] != NULL;  j++) {
			if (!strcmp(Session.Scenarios[i]->Description(), EngMisStr[j])) {
#ifdef FIXIT_CSII	//	ajw Added Aftermath installed checks (before, it was assumed).
			//	Add mission if it's available to us.
			if( !( ( Is_Mission_Counterstrike((char *)(Session.Scenarios[i]->Get_Filename())) && !Is_Counterstrike_Installed() ) ||
					( Is_Mission_Aftermath((char *)(Session.Scenarios[i]->Get_Filename())) && !Is_Aftermath_Installed() ) ) )
#endif
#if defined(GERMAN) || defined(FRENCH)
				scenariolist.Add_Item(EngMisStr[j+1]);
#else
				scenariolist.Add_Item(EngMisStr[j]);
#endif
				break;
			}
		}
		if (EngMisStr[j] == NULL) {
#ifdef FIXIT_CSII	//	ajw Added Aftermath installed checks (before, it was assumed). Added officialness check.
			//	Add mission if it's available to us.
			if( !Session.Scenarios[i]->Get_Official() ||
				!( ( Is_Mission_Counterstrike((char *)(Session.Scenarios[i]->Get_Filename())) && !Is_Counterstrike_Installed() ) ||
					( Is_Mission_Aftermath((char *)(Session.Scenarios[i]->Get_Filename())) && !Is_Aftermath_Installed() ) ) )
#endif
			scenariolist.Add_Item(Session.Scenarios[i]->Description());
		}
	}

	Session.Options.ScenarioIndex = 0;		// 1st scenario is selected

	/*........................................................................
	Init random-number generator, & create a seed to be used for all random
	numbers from here on out
	........................................................................*/
#ifdef FIXIT_RANDOM_GAME
	srand(time(NULL));
	Seed = rand();
#else
//	randomize();
//	Seed = rand();
#endif

	/*........................................................................
	Init the message display system
	........................................................................*/
	if (!skirmish) {
		Session.Messages.Init (d_message_x + 1, d_message_y + 1, 8,
			MAX_MESSAGE_LENGTH, d_txt6_h, d_send_x + 1, d_send_y + 1, 1,
			20, MAX_MESSAGE_LENGTH - 5, d_message_w);
		Session.Messages.Add_Edit ((Session.ColorIdx == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : Session.ColorIdx,
									TPF_TEXT, NULL, '_', d_message_w);
	}

	/*........................................................................
	Init version number clipping system
	........................................................................*/
	VerNum.Init_Clipping();
	Load_Title_Page(true);
	CCPalette.Set();

	extern char ModemRXString[];

	if (strlen(ModemRXString) > 36)
		ModemRXString[36] = 0;

	if (strlen(ModemRXString) > 0)
		Session.Messages.Add_Message (NULL, 0, ModemRXString, PCOLOR_BROWN,
			TPF_TEXT, -1);

	ModemRXString[0] = '\0';

	/*
	---------------------------- Init Mono Output ----------------------------
	*/
	#if(SHOW_MONO)
	if (!skirmish) {
		NullModem.Configure_Debug(sizeof (CommHeaderType),sizeof (SerialCommandType),
			SerialPacketNames, 100, 8);
		NullModem.Mono_Debug_Print(1);
	}
	#endif

	/*
	---------------------------- Processing loop -----------------------------
	*/
	if (!skirmish) {
		NullModem.Reset_Response_Time();		// clear response time
	}
	theirresponsetime = 10000;				// initialize to an invalid value
	timingtime = lastmsgtime = lastredrawtime = TickCount;


	/*
	** Those easily offended should avert their eyes from the following line. And whatever
	** you do, dont search for 'goto'.
	*/
oh_dear_its_a_label:

	while (process) {
		#if(SHOW_MONO)
		if (!skirmish) {
			NullModem.Mono_Debug_Print(0);
		}
		#endif

		if (!skirmish){
			if (!ok_button_added && gameoptions && kludge_timer == 0){
				okbtn.Add_Tail(*commands);
				ok_button_added = true;
#ifdef FIXIT_VERSION_3
				if( loadfile.Is_Available() )
				{
					loadbtn.Add_Tail( *commands );
				}
#else
#ifdef FIXIT_MULTI_SAVE
				if ( loadfile.Is_Available() && PlayingAgainstVersion > VERSION_RED_ALERT_104 ) {
					loadbtn.Add_Tail ( *commands );
				}
#endif
#endif
				if (display < REDRAW_BUTTONS) display = REDRAW_BUTTONS;
			}
		}

		/*
		** Kludge to make sure we redraw the message input line when it loses focus.
		** If we dont do this then the cursor doesnt disappear.
		*/
		if (!skirmish) {
			if (messages_have_focus) {
				if (name_edt.Has_Focus()) {
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
					messages_have_focus = false;
				}
			} else {
				if (!name_edt.Has_Focus()) {
					messages_have_focus = true;
					if (display < REDRAW_MESSAGE)	display = REDRAW_MESSAGE;
					Session.Messages.Set_Edit_Focus();
				}
			}
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}
		#endif

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			if (housebtn.IsDropped) {
				housebtn.Collapse();
				display = REDRAW_BACKGROUND;
			}
			Hide_Mouse();

			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				// init font variables

				Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_CENTER | TPF_TEXT);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Fancy_Text_Print(TXT_YOUR_NAME, d_name_x + (d_name_w / 2), d_name_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
#ifdef OLDWAY
				Fancy_Text_Print(TXT_SIDE_COLON, d_gdi_x + d_gdi_w, d_gdi_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
#else
				Fancy_Text_Print(TXT_SIDE_COLON, d_house_x + (d_house_w / 2), d_house_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
#endif
				Fancy_Text_Print(TXT_COLOR_COLON, d_dialog_x + ((d_dialog_w / 4) * 3), d_color_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
				if (!skirmish) {
					Fancy_Text_Print(TXT_PLAYERS, d_playerlist_x + (d_playerlist_w / 2), d_playerlist_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
				} else {
					Fancy_Text_Print(TXT_EASY, difficulty.X, difficulty.Y-8*RESFACTOR, scheme, TBLACK, TPF_TEXT);
					Fancy_Text_Print(TXT_HARD, difficulty.X + difficulty.Width, difficulty.Y-8*RESFACTOR, scheme, TBLACK, TPF_RIGHT|TPF_TEXT);
					Fancy_Text_Print(TXT_NORMAL, difficulty.X + difficulty.Width/2, difficulty.Y-8*RESFACTOR, scheme, TBLACK, TPF_CENTER|TPF_TEXT);
				}
				Fancy_Text_Print(TXT_SCENARIOS, d_scenariolist_x + (d_scenariolist_w / 2), d_scenariolist_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
				Fancy_Text_Print(TXT_COUNT, d_count_x - 2, d_count_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
				Fancy_Text_Print(TXT_LEVEL, d_level_x - 2, d_level_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
				Fancy_Text_Print(TXT_CREDITS_COLON, d_credits_x - 2, d_credits_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
				Fancy_Text_Print(TXT_AI_PLAYERS_COLON, d_aiplayers_x - 2*RESFACTOR, d_aiplayers_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
			}

			/*..................................................................
			Draw the color boxes
			..................................................................*/
			if (display >= REDRAW_COLORS) {
				for (i = 0; i < MAX_MPLAYER_COLORS; i++) {
					LogicPage->Fill_Rect (cbox_x[i] + 1, d_color_y + 1,
						cbox_x[i] + 1 + d_color_w - 2, d_color_y + 1 + d_color_h - 2,
						ColorRemaps[i].Box);
//						(i == PCOLOR_DIALOG_BLUE) ? ColorRemaps[PCOLOR_REALLY_BLUE].Box : ColorRemaps[i].Box);

					if (i == Session.ColorIdx) {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h, BOXSTYLE_DOWN, false);
					} else {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h, BOXSTYLE_RAISED, false);
					}
				}
			}

			/*..................................................................
			Draw the message system; erase old messages first
			..................................................................*/
			if (display >= REDRAW_MESSAGE && !skirmish) {
				Draw_Box(d_message_x, d_message_y, d_message_w, d_message_h, BOXSTYLE_BOX, true);
				Draw_Box(d_send_x, d_send_y, d_send_w, d_send_h, BOXSTYLE_BOX, true);
				Session.Messages.Draw();
			}

			//..................................................................
			// Update game parameter labels
			//..................................................................
			if (display >= REDRAW_PARMS) {

//				LogicPage->Fill_Rect(d_count_x + d_count_w + 2, d_count_y, d_count_x + d_count_w + 35 * RESFACTOR, d_aiplayers_y + d_aiplayers_h+RESFACTOR, BLACK);

				sprintf(staticcountbuff, "%d", Session.Options.UnitCount);
				staticcount.Set_Text(staticcountbuff);
				staticcount.Draw_Me();
//				Fancy_Text_Print("%d ", d_count_x + d_count_w + 3 * RESFACTOR, d_count_y, scheme, BLACK, TPF_TEXT, Session.Options.UnitCount);

				if (BuildLevel <= MPLAYER_BUILD_LEVEL_MAX) {
					sprintf(staticlevelbuff, "%d ", BuildLevel);
				} else {
					sprintf(staticlevelbuff, "**");
				}
				staticlevel.Set_Text(staticlevelbuff);
				staticlevel.Draw_Me();
//				Fancy_Text_Print(txt, d_level_x + d_level_w + 3 * RESFACTOR, d_level_y, scheme, BLACK, TPF_TEXT);

				sprintf(staticcreditsbuff, "%d", Session.Options.Credits);
				staticcredits.Set_Text(staticcreditsbuff);
				staticcredits.Draw_Me();
//				Fancy_Text_Print("%d", d_credits_x + d_credits_w + 2 * RESFACTOR, d_credits_y, scheme, BLACK, TPF_TEXT, Session.Options.Credits);

				sprintf(staticaibuff, "%d", Session.Options.AIPlayers);
				staticai.Set_Text(staticaibuff);
				staticai.Draw_Me();
//				Fancy_Text_Print("%d", d_aiplayers_x + d_aiplayers_w + 2*RESFACTOR, d_aiplayers_y, scheme, BLACK, TPF_TEXT, Session.Options.AIPlayers);
			}

			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Flag_List_To_Redraw();
				commands->Draw_All();
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		messages_have_focus = Session.Messages.Has_Edit_Focus();
		bool droplist_is_dropped = housebtn.IsDropped;
		input = commands->Input();

		/*
		** Sort out the input focus between the name edit box and the message system
		*/
		if (!skirmish) {
			if (messages_have_focus) {
				if (!name_edt.Has_Focus()) {
					Session.Messages.Set_Edit_Focus();
				} else {
					messages_have_focus = false;
					display = REDRAW_MESSAGE;
				}
			}
		}

		/*
		** Redraw everything if the droplist collapsed
		*/
		if (droplist_is_dropped && !housebtn.IsDropped) {
			display = REDRAW_BACKGROUND;
		}

		if (input & KN_BUTTON) {
			if (housebtn.IsDropped) {
				housebtn.Collapse();
				display = REDRAW_BACKGROUND;
			}
		}


		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {
			/*------------------------------------------------------------------
			User clicks on a color button
			------------------------------------------------------------------*/
			case KN_LMOUSE:
				if (Keyboard->MouseQX > cbox_x[0] &&
						Keyboard->MouseQX < (cbox_x[MAX_MPLAYER_COLORS - 1] + d_color_w) &&
						Keyboard->MouseQY > d_color_y &&
						Keyboard->MouseQY < (d_color_y + d_color_h)) {

					Session.PrefColor = (PlayerColorType)((Keyboard->MouseQX - cbox_x[0]) / d_color_w);
					Session.ColorIdx = Session.PrefColor;
					if (display < REDRAW_COLORS) display = REDRAW_COLORS;

					name_edt.Set_Color (&ColorRemaps[(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : Session.ColorIdx]);
					name_edt.Flag_To_Redraw();
					Session.Messages.Set_Edit_Color((Session.ColorIdx == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : Session.ColorIdx);
					strcpy (Session.Handle, namebuf);
					transmit = true;
					changed = true;
					if (housebtn.IsDropped) {
						housebtn.Collapse();
						display = REDRAW_BACKGROUND;
					}
				}
				break;

			/*------------------------------------------------------------------
			User edits the name field; retransmit new game options
			------------------------------------------------------------------*/
			case (BUTTON_NAME | KN_BUTTON):
				if (housebtn.IsDropped) {
					housebtn.Collapse();
					display = REDRAW_BACKGROUND;
				}
				strcpy (Session.Handle, namebuf);
				transmit = true;
				changed = true;
				break;

#ifdef OLDWAY
			/*------------------------------------------------------------------
			House Buttons: set the player's desired House
			------------------------------------------------------------------*/
			case (BUTTON_GDI | KN_BUTTON):
				Session.House = HOUSE_GOOD;
				gdibtn.Turn_On();
				nodbtn.Turn_Off();
				strcpy (Session.Handle, namebuf);
				transmit = true;
				break;

			case (BUTTON_NOD | KN_BUTTON):
				Session.House = HOUSE_BAD;
				gdibtn.Turn_Off();
				nodbtn.Turn_On();
				strcpy (Session.Handle, namebuf);
				transmit = true;
				break;
#else
			case (BUTTON_HOUSE | KN_BUTTON):
				Session.House = HousesType(housebtn.Current_Index()+HOUSE_USSR);
				strcpy (Session.Handle, namebuf);
				display = REDRAW_BACKGROUND;
				transmit = true;
				break;
#endif

			/*------------------------------------------------------------------
			New Scenario selected.
			------------------------------------------------------------------*/
#ifdef FIXIT_VERSION_3		//	All scenarios now allowable as downloads. ajw
			case (BUTTON_SCENARIOLIST | KN_BUTTON):
				if (housebtn.IsDropped) {
					housebtn.Collapse();
					display = REDRAW_BACKGROUND;
				}
				if (scenariolist.Current_Index() != Session.Options.ScenarioIndex)
				{
					Session.Options.ScenarioIndex = scenariolist.Current_Index();
					strcpy (Session.Handle, namebuf);
					transmit = true;
				}
				break;

#else	//	FIXIT_VERSION_3		Whoever duplicated Netdlg into Nulldlg should be shot. Wasn't it enough?

				Abandon all hope ye who hit enter here.

			case (BUTTON_SCENARIOLIST | KN_BUTTON):
				if (housebtn.IsDropped) {
					housebtn.Collapse();
					display = REDRAW_BACKGROUND;
				}
				if (scenariolist.Current_Index() != Session.Options.ScenarioIndex) {
#ifdef FIXIT_CSII	//	checked - ajw
					if ( !skirmish && (PlayingAgainstVersion != VERSION_RED_ALERT_107 && PlayingAgainstVersion != VERSION_RED_ALERT_108 && PlayingAgainstVersion < VERSION_AFTERMATH_CS) &&
#else
					if ( !skirmish && PlayingAgainstVersion < VERSION_RED_ALERT_107 &&
#endif
								Session.Scenarios[scenariolist.Current_Index()]->Get_Expansion()){
						scenariolist.Set_Selected_Index (Session.Options.ScenarioIndex);
						Session.Messages.Add_Message(NULL, 0,
							(char *)Text_String(TXT_NO_CS_SCENARIOS), PCOLOR_BROWN, TPF_TEXT, 1200);
						Sound_Effect(VOC_SYS_ERROR);
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
#ifdef FIXIT_CSII	//	checked - ajw
					} else
					if ( !skirmish && PlayingAgainstVersion < VERSION_AFTERMATH_CS &&
							Is_Mission_126x126((char *)Session.Scenarios[scenariolist.Current_Index()]->Get_Filename() ) ){
						scenariolist.Set_Selected_Index (Session.Options.ScenarioIndex);
						Session.Messages.Add_Message(NULL, 0,
							(char *)Text_String(TXT_NO_CS_SCENARIOS), PCOLOR_BROWN, TPF_TEXT, 1200);
						Sound_Effect(VOC_SYS_ERROR);
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
#endif
					}else{
						Session.Options.ScenarioIndex = scenariolist.Current_Index();
						transmit = 1;
					}

					strcpy (Session.Handle, namebuf);
					transmit = true;
				}
				break;
#endif

			/*------------------------------------------------------------------
			User adjusts max # units
			------------------------------------------------------------------*/
			case (BUTTON_COUNT | KN_BUTTON):
				Session.Options.UnitCount = countgauge.Get_Value() + SessionClass::CountMin[Session.Options.Bases];
				if (display < REDRAW_PARMS) display = REDRAW_PARMS;
				if (housebtn.IsDropped) {
					housebtn.Collapse();
					display = REDRAW_BACKGROUND;
				}
				transmit = true;
				break;

			/*------------------------------------------------------------------
			User adjusts build level
			------------------------------------------------------------------*/
			case (BUTTON_LEVEL | KN_BUTTON):
				BuildLevel = levelgauge.Get_Value() + 1;
				if (BuildLevel > MPLAYER_BUILD_LEVEL_MAX)	// if it's pegged, max it out
					BuildLevel = MPLAYER_BUILD_LEVEL_MAX;
				if (display < REDRAW_PARMS) display = REDRAW_PARMS;
				if (housebtn.IsDropped) {
					housebtn.Collapse();
					display = REDRAW_BACKGROUND;
				}
				transmit = true;
				break;

			/*------------------------------------------------------------------
			User adjusts max # units
			------------------------------------------------------------------*/
			case (BUTTON_CREDITS | KN_BUTTON):
				Session.Options.Credits = creditsgauge.Get_Value();
				Session.Options.Credits = ((Session.Options.Credits + 250) / 500) * 500;
				if (display < REDRAW_PARMS) display = REDRAW_PARMS;
				if (housebtn.IsDropped) {
					housebtn.Collapse();
					display = REDRAW_BACKGROUND;
				}
				transmit = true;
				break;

			//..................................................................
			//	User adjusts # of AI players
			//..................................................................
			case (BUTTON_AIPLAYERS | KN_BUTTON):
				{
					Session.Options.AIPlayers = aiplayersgauge.Get_Value();
					int humans = 2;			// Two humans.
					if (skirmish) {
						Session.Options.AIPlayers += 1;	// Always one forced AI player.
						humans = 1;			// One human.
	//						if (Session.Options.AIPlayers == 0) {
	//							Session.Options.AIPlayers = 1;
	//							aiplayersgauge.Set_Value(0);
	//						}
					}
					if (Session.Options.AIPlayers+humans >= Rule.MaxPlayers) {	// if it's pegged, max it out
						Session.Options.AIPlayers = Rule.MaxPlayers - humans;
						aiplayersgauge.Set_Value(Session.Options.AIPlayers - (skirmish ? 1 : 0));
					}
					transmit = true;
					if (display < REDRAW_PARMS) display = REDRAW_PARMS;

					if (housebtn.IsDropped) {
						housebtn.Collapse();
						display = REDRAW_BACKGROUND;
					}

					break;
				}

			//------------------------------------------------------------------
			// Toggle-able options:
			// If 'Bases' gets toggled, we have to change the range of the
			// UnitCount slider.
			// Also, if Tiberium gets toggled, we have to set the flags
			// in SpecialClass.
			//------------------------------------------------------------------
			case (BUTTON_OPTIONS | KN_BUTTON):
				if (!skirmish && Special.IsCaptureTheFlag != optionlist.Is_Checked(4) && !Special.IsCaptureTheFlag) {
					optionlist.Check_Item(0, true);
				}
				if (Session.Options.Bases != optionlist.Is_Checked(0)) {
					Session.Options.Bases = optionlist.Is_Checked(0);
					if (Session.Options.Bases) {
						Session.Options.UnitCount = Fixed_To_Cardinal (
							SessionClass::CountMax[1] -
							SessionClass::CountMin[1],
							Cardinal_To_Fixed(
								SessionClass::CountMax[0]-SessionClass::CountMin[0],
								Session.Options.UnitCount-SessionClass::CountMin[0])) +
							SessionClass::CountMin[1];
					} else {
						if (!skirmish) optionlist.Check_Item(4, false);
						Session.Options.UnitCount = Fixed_To_Cardinal (
							SessionClass::CountMax[0] -
							SessionClass::CountMin[0],
							Cardinal_To_Fixed(
								SessionClass::CountMax[1]-SessionClass::CountMin[1],
								Session.Options.UnitCount - SessionClass::CountMin[1])) +
							SessionClass::CountMin[0];
					}
					countgauge.Set_Maximum(
						SessionClass::CountMax[Session.Options.Bases] -
						SessionClass::CountMin[Session.Options.Bases]);
					countgauge.Set_Value(Session.Options.UnitCount -
						SessionClass::CountMin[Session.Options.Bases]);
				}
				Session.Options.Tiberium = optionlist.Is_Checked(1);
				Special.IsTGrowth = Session.Options.Tiberium;
				Rule.IsTGrowth = Session.Options.Tiberium;
				Special.IsTSpread = Session.Options.Tiberium;
				Rule.IsTSpread = Session.Options.Tiberium;

				Session.Options.Goodies = optionlist.Is_Checked(2);
				Special.IsShadowGrow = optionlist.Is_Checked(3);
				if (!skirmish) {
					Special.IsCaptureTheFlag = optionlist.Is_Checked(4);
				}

				transmit = true;
				if (display < REDRAW_PARMS) display = REDRAW_PARMS;
				if (housebtn.IsDropped) {
					housebtn.Collapse();
					display = REDRAW_BACKGROUND;
				}
				break;

			/*------------------------------------------------------------------
			OK: exit loop with true status
			------------------------------------------------------------------*/
			case (BUTTON_LOAD | KN_BUTTON):
			case (BUTTON_OK | KN_BUTTON):
				if (housebtn.IsDropped) {
					housebtn.Collapse();
					display = REDRAW_BACKGROUND;
				}
				//
				// make sure we got a game options packet from the other player
				//
				if (gameoptions) {
					rc = true;
					process = false;

					// force transmitting of game options packet one last time

					transmit = true;
					transmittime = 0;
				} else {
					WWMessageBox().Process (TXT_ONLY_ONE,TXT_OOPS,NULL);
					display = REDRAW_ALL;
				}
				if (input==(BUTTON_LOAD | KN_BUTTON))
					load_game = true;
				break;

			/*------------------------------------------------------------------
			CANCEL: send a SIGN_OFF, bail out with error code
			------------------------------------------------------------------*/
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				if (housebtn.IsDropped) {
					housebtn.Collapse();
					display = REDRAW_BACKGROUND;
				}
				process = false;
				rc = false;
				break;

			/*------------------------------------------------------------------
			Default: manage the inter-player messages
			------------------------------------------------------------------*/
			default:
				if (!skirmish) {
					if (Session.Messages.Manage()) {
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
					}

					/*...............................................................
					Service keyboard input for any message being edited.
					...............................................................*/
					i = Session.Messages.Input(input);

					/*...............................................................
					If 'Input' returned 1, it means refresh the message display.
					(We have to redraw the edit line, to erase the cursor.)
					...............................................................*/
					if (i==1) {
						Hide_Mouse();
						Draw_Box(d_send_x, d_send_y, d_send_w, d_send_h,
							BOXSTYLE_BOX, true);
						Session.Messages.Draw();
						Show_Mouse();
					} else if (i==2) {
					/*...............................................................
					If 'Input' returned 2, it means redraw the message display.
					Rather than setting 'display', which would redraw all msgs,
					we only need to erase & redraw the edit box here.
					...............................................................*/
						Hide_Mouse();
						Draw_Box(d_send_x, d_send_y, d_send_w, d_send_h, BOXSTYLE_BOX, true);
						Session.Messages.Draw();
						Show_Mouse();
					} else if (i==3 || i==4) {
					/*...............................................................
					If 'input' returned 3, it means send the current message.
					...............................................................*/
						memset (&SendPacket, 0, sizeof(SerialPacketType));
						SendPacket.Command = SERIAL_MESSAGE;
						strcpy (SendPacket.Name, namebuf);
						SendPacket.ID = Session.ColorIdx;
						if (i==3) {
							strcpy (SendPacket.Message.Message, Session.Messages.Get_Edit_Buf());
						} else {
							strcpy (SendPacket.Message.Message, Session.Messages.Get_Overflow_Buf());
							Session.Messages.Clear_Overflow_Buf();
						}

						/*..................................................................
						Send the message
						..................................................................*/
						if (!skirmish) {
							NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);
							NullModem.Service();
						}
						/*..................................................................
						Add the message to our own screen
						..................................................................*/
						Session.Messages.Add_Message (SendPacket.Name, SendPacket.ID,
							SendPacket.Message.Message,
							(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : Session.ColorIdx,
							TPF_TEXT, -1);
						Session.Messages.Add_Edit((Session.ColorIdx == PCOLOR_DIALOG_BLUE) ?
																				PCOLOR_REALLY_BLUE : Session.ColorIdx,
														TPF_TEXT, NULL, '_', d_message_w);

						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
					}	/* end of send message */
				}
				break;
		}

		/*---------------------------------------------------------------------
		Detect editing of the name buffer, transmit new values to players
		---------------------------------------------------------------------*/
		if (strcmp (namebuf, Session.Handle)) {
			strcpy (Session.Handle, namebuf);
			transmit = true;
			changed = true;
		}

		/*---------------------------------------------------------------------
		If our Transmit flag is set, we need to send out a game option packet.
		This message requires an ACK.  The first time through the loop, transmit
		should be set, so we send out our default options; we'll then send
		any changes we make to the defaults.
		---------------------------------------------------------------------*/
		if (skirmish) {
			transmit = false;
		}

		if (transmit && (TickCount - transmittime) > PACKET_RETRANS_TIME) {
			memset (&SendPacket, 0, sizeof(SerialPacketType));
			SendPacket.Command = SERIAL_GAME_OPTIONS;
			strcpy (SendPacket.Name, namebuf);
			SendPacket.ScenarioInfo.CheatCheck = RuleINI.Get_Unique_ID();
			SendPacket.ScenarioInfo.MinVersion = VerNum.Min_Version();
			SendPacket.ScenarioInfo.MaxVersion = VerNum.Max_Version();
			SendPacket.ScenarioInfo.House = Session.House;
			SendPacket.ScenarioInfo.Color = Session.ColorIdx;
			SendPacket.ScenarioInfo.Credits = Session.Options.Credits;
			SendPacket.ScenarioInfo.IsBases = Session.Options.Bases;
			SendPacket.ScenarioInfo.IsTiberium = Session.Options.Tiberium;
			SendPacket.ScenarioInfo.IsGoodies = Session.Options.Goodies;
			SendPacket.ScenarioInfo.AIPlayers = Session.Options.AIPlayers;
			SendPacket.ScenarioInfo.BuildLevel = BuildLevel;
			SendPacket.ScenarioInfo.UnitCount = Session.Options.UnitCount;
			SendPacket.ScenarioInfo.Seed = Seed;
			SendPacket.ScenarioInfo.Special = Special;
			SendPacket.ScenarioInfo.GameSpeed = Options.GameSpeed;
			SendPacket.ID = Session.ModemType;

			/*
			** Set up the scenario info so the remote player can match the scenario on his machine
			** or request a download if it doesnt exist
			*/
			strcpy (SendPacket.ScenarioInfo.Scenario, Session.Scenarios[Session.Options.ScenarioIndex]->Description());
			CCFileClass file (Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename());

			SendPacket.ScenarioInfo.FileLength = file.Size();

#ifdef WOLAPI_INTEGRATION
			strcpy( SendPacket.ScenarioInfo.ShortFileName, Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename() );
#else
			strncpy (SendPacket.ScenarioInfo.ShortFileName, Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename(), sizeof(SendPacket.ScenarioInfo.ShortFileName));
#endif
			strncpy ((char*)SendPacket.ScenarioInfo.FileDigest, Session.Scenarios[Session.Options.ScenarioIndex]->Get_Digest(), sizeof SendPacket.ScenarioInfo.FileDigest);
			SendPacket.ScenarioInfo.OfficialScenario = Session.Scenarios[Session.Options.ScenarioIndex]->Get_Official();
			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			transmittime = TickCount;
			transmit = false;

			//..................................................................
			// Keep the player list up to date
			//..................................................................
			if (playerlist.Count()) {
				item = (char *)playerlist.Get_Item(0);
#ifdef OLDWAY
				if (Session.House==HOUSE_GOOD) {
					sprintf(item,"%s\t%s",namebuf,
						Text_String(TXT_ALLIES));
				} else {
					sprintf(item,"%s\t%s",namebuf,
						Text_String(TXT_SOVIET));
				}
#else	//OLDWAY
				sprintf (item, "%s\t%s", namebuf, Text_String(HouseTypeClass::As_Reference(Session.House).Full_Name()));
#endif	//OLDWAY
				playerlist.Colors[0] = &ColorRemaps[(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ?
																				PCOLOR_REALLY_BLUE : Session.ColorIdx];
				playerlist.Flag_To_Redraw();
			}

			//..................................................................
			// Play a little sound effect
			//..................................................................
			Sound_Effect(VOC_OPTIONS_CHANGED);
		}

		//
		// send a timing packet if enough time has gone by.
		//
		if (!skirmish &&  (TickCount - timingtime) > PACKET_TIMING_TIMEOUT) {
			memset (&SendPacket, 0, sizeof(SerialPacketType));
			SendPacket.Command = SERIAL_TIMING;
			SendPacket.ScenarioInfo.ResponseTime = NullModem.Response_Time();
			SendPacket.ID = Session.ModemType;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 0);
			timingtime = TickCount;
		}

		/*---------------------------------------------------------------------
		Check for an incoming message
		---------------------------------------------------------------------*/
		if (!skirmish && NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {

			lastmsgtime = TickCount;
			msg_timeout = 600;		// reset timeout value to 10 seconds
											// (only the 1st time through is 20 seconds)

			// are we getting our own packets back??

			if (ReceivePacket.Command >= SERIAL_CONNECT &&
				ReceivePacket.Command < SERIAL_LAST_COMMAND &&
				ReceivePacket.Command != SERIAL_MESSAGE &&
				ReceivePacket.ID == Session.ModemType) {

				WWMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);

				// to skip the other system not responding msg
				lastmsgtime = TickCount;

				process = false;
				rc = false;

				// say we did receive sign off to keep from sending one
				recsignedoff = true;
				break;
			}

			event = (EventClass *)&ReceivePacket;
			if (event->Type <= EventClass::FRAMEINFO) {
				if ( (TickCount - lastredrawtime) > PACKET_REDRAW_TIME) {
					lastredrawtime = TickCount;
					oppscorescreen = true;
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
					parms_received = true;
				}
			} else {
				switch ( ReceivePacket.Command ) {
					/*..................................................................
					Sign-off: Give the other machine time to receive my ACK, display a
					message, and exit.
					..................................................................*/
					case (SERIAL_SIGN_OFF):
						starttime = TickCount;
						while (TickCount - starttime < 60)
							NullModem.Service();
						WWMessageBox().Process(TXT_USER_SIGNED_OFF);

						// to skip the other system not responding msg
						lastmsgtime = TickCount;

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
						kludge_timer = 2*60;
						strcpy (TheirName, ReceivePacket.Name);
						TheirColor = ReceivePacket.ScenarioInfo.Color;
						TheirHouse = ReceivePacket.ScenarioInfo.House;
						transmit = true;

						parms_received = true;
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;

						//.........................................................
						// "Clip" the other system's version range to our own
						// ........................................................
						version = VerNum.Clip_Version(ReceivePacket.ScenarioInfo.MinVersion,
							ReceivePacket.ScenarioInfo.MaxVersion);
						// ........................................................
						// If the greatest-common-version comes back 0, the other
						// system's range is too low for ours
						// ........................................................
						if (version == 0) {
							WWMessageBox().Process (TXT_DESTGAME_OUTDATED);

							// to skip the other system not responding msg
							lastmsgtime = TickCount;

							process = false;
							rc = false;
						} else if (version == 0xffffffff) {
						// ........................................................
						// If the greatest-common-version comes back 0xffffffff,
						// the other system's range is too high for ours
						// ........................................................
							WWMessageBox().Process (TXT_YOURGAME_OUTDATED);

							// to skip the other system not responding msg
							lastmsgtime = TickCount;

							process = false;
							rc = false;
						} else {

							if (ReceivePacket.ScenarioInfo.CheatCheck != RuleINI.Get_Unique_ID()) {
								WWMessageBox().Process (TXT_MISMATCH);

								// to skip the other system not responding msg
								lastmsgtime = TickCount;

								process = false;
								rc = false;

							} else {


								// ........................................................
								// Otherwise, 'version' is the highest version we have in
								// common; look up the protocol that goes with this version.
								// ........................................................
								Session.CommProtocol = VerNum.Version_Protocol(version);
#ifndef FIXIT_VERSION_3
								PlayingAgainstVersion = version;
#endif
							}
						}
						/*.........................................................
						If this is the first game-options packet we've received,
						init the game & player lists
						.........................................................*/
						if (playerlist.Count()==0) {
							//......................................................
							// Add two strings to the player list
							//......................................................
							item = new char [MPLAYER_NAME_MAX + 64];	//Need room to display country name
							playerlist.Add_Item(item,
								&ColorRemaps[Session.ColorIdx]);
							item = new char [MPLAYER_NAME_MAX + 64];	//Need room to display country name
							playerlist.Add_Item(item,
								&ColorRemaps[TheirColor]);
						}

						//.........................................................
						// Ensure the player list has the latest, greatest copy of
						// our names & colors.  Do this every time we receive an
						// options packet.
						//.........................................................
						item = (char *)playerlist.Get_Item(0);
#ifdef OLDWAY
						if (Session.House==HOUSE_GOOD) {
							sprintf(item,"%s\t%s",namebuf,
								Text_String(TXT_ALLIES));
						} else {
							sprintf(item,"%s\t%s",namebuf,
								Text_String(TXT_SOVIET));
						}
#else	//OLDWAY
						sprintf (item, "%s\t%s", namebuf, Text_String(HouseTypeClass::As_Reference(Session.House).Full_Name()));
#endif	//OLDWAY
						playerlist.Colors[0] = &ColorRemaps[(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ?
																				PCOLOR_REALLY_BLUE : Session.ColorIdx];

						item = (char *)playerlist.Get_Item(1);
#ifdef	OLDWAY
						if (TheirHouse==HOUSE_GOOD) {
							sprintf(item,"%s\t%s",TheirName,
								Text_String(TXT_ALLIES));
						} else {
							sprintf(item,"%s\t%s",TheirName,
								Text_String(TXT_SOVIET));
						}
#else	//OLDWAY
						sprintf (item, "%s\t%s", TheirName, Text_String(HouseTypeClass::As_Reference(TheirHouse).Full_Name()));
#endif	//OLDWAY
						playerlist.Colors[1] = &ColorRemaps[(TheirColor == PCOLOR_DIALOG_BLUE) ?
																						PCOLOR_REALLY_BLUE : TheirColor];

						playerlist.Flag_To_Redraw();

						//.........................................................
						// Play a little sound effect
						//.........................................................
						Sound_Effect(VOC_OPTIONS_CHANGED);

						break;

					/*..................................................................
					Incoming message: add to our list
					..................................................................*/
					case (SERIAL_MESSAGE):
						oppscorescreen = false;

						Session.Messages.Add_Message (ReceivePacket.Name,
							((PlayerColorType)ReceivePacket.ID == PCOLOR_DIALOG_BLUE) ?
														PCOLOR_REALLY_BLUE : (PlayerColorType)ReceivePacket.ID,
							ReceivePacket.Message.Message,
							((PlayerColorType)ReceivePacket.ID == PCOLOR_DIALOG_BLUE) ?
														PCOLOR_REALLY_BLUE : (PlayerColorType)ReceivePacket.ID,
							TPF_TEXT, -1);

						Sound_Effect(VOC_INCOMING_MESSAGE);
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;

						break;

					//
					// get their response time
					//
					case (SERIAL_TIMING):
						oppscorescreen = false;
						theirresponsetime = ReceivePacket.ScenarioInfo.ResponseTime;

						if ( !gameoptions ) {

							// retransmit of game options packet again
							transmit = true;
						}
						break;

					//
					// print msg waiting for opponent
					//
					case (SERIAL_SCORE_SCREEN):
						oppscorescreen = true;
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
						parms_received = true;
						break;

					default:
						break;
				}
			}
		}

		// if we haven't received a msg for 10 seconds exit

		if (!skirmish && (TickCount - lastmsgtime) > msg_timeout) {
			WWMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);
			process = false;
			rc = false;

			// say we did receive sign off to keep from sending one
			recsignedoff = true;
		}

		/*---------------------------------------------------------------------
		Service the connection
		---------------------------------------------------------------------*/
		if (!skirmish) {
			NullModem.Service();
		}
	}

	/*------------------------------------------------------------------------
	Prepare to load the scenario
	------------------------------------------------------------------------*/
	if (rc) {
		Session.NumPlayers = skirmish ? 1 : 2;

		Scen.Scenario = Session.Options.ScenarioIndex;
		strcpy (Scen.ScenarioName, Session.Scenarios[Session.Options.ScenarioIndex]->Get_Filename());

		/*.....................................................................
		Add both players to the Players vector; the local system is always
		index 0.
		.....................................................................*/
		who = new NodeNameType;
		strcpy(who->Name, namebuf);
		who->Player.House = Session.House;
		who->Player.Color = Session.ColorIdx;
		who->Player.ProcessTime = -1;
		Session.Players.Add (who);

		/*
		**	Fetch the difficulty setting when in skirmish mode.
		*/
		if (skirmish) {
			int diff = difficulty.Get_Value() * (Rule.IsFineDifficulty ? 1 : 2);
			switch (diff) {
				case 0:
					Scen.CDifficulty = DIFF_HARD;
					Scen.Difficulty = DIFF_EASY;
					break;

				case 1:
					Scen.CDifficulty = DIFF_HARD;
					Scen.Difficulty = DIFF_NORMAL;
					break;

				case 2:
					Scen.CDifficulty = DIFF_NORMAL;
					Scen.Difficulty = DIFF_NORMAL;
					break;

				case 3:
					Scen.CDifficulty = DIFF_EASY;
					Scen.Difficulty = DIFF_NORMAL;
					break;

				case 4:
					Scen.CDifficulty = DIFF_EASY;
					Scen.Difficulty = DIFF_HARD;
					break;
			}
		} else {
			Scen.CDifficulty = DIFF_NORMAL;
			Scen.Difficulty = DIFF_NORMAL;
		}

		if (!skirmish) {
			who = new NodeNameType;

			#ifdef FIXIT_MODEM_LOAD_CRASH
			/* If the names of the players are the same then we MUST force them
			 * be be unique. This is necessary to prevent a crash after loading
			 * a modem save game.
			 */
			if (strcmp(TheirName, namebuf) == 0)
				{
				if (strlen(TheirName) == (MPLAYER_NAME_MAX - 1))
					{
					TheirName[MPLAYER_NAME_MAX - 1] = '\0';
					}
				else
					{
					strcat(TheirName, "2");
					}
				}
			#endif

			strcpy(who->Name, TheirName);
			who->Player.House = TheirHouse;
			who->Player.Color = TheirColor;
			who->Player.ProcessTime = -1;
			Session.Players.Add (who);
		}

		/*.....................................................................
		Send all players a GO packet.
		.....................................................................*/
		memset (&SendPacket, 0, sizeof(SerialPacketType));
		if (load_game) {
			SendPacket.Command = SERIAL_LOADGAME;
		} else {
			SendPacket.Command = SERIAL_GO;
		}

		if (!skirmish) {
			SendPacket.ScenarioInfo.ResponseTime = NullModem.Response_Time();
			if ( theirresponsetime == 10000 ) {
				;
			} else if (SendPacket.ScenarioInfo.ResponseTime < theirresponsetime) {
				SendPacket.ScenarioInfo.ResponseTime = theirresponsetime;
			}
		}

		//
		// calculated one way delay for a packet and overall delay to execute
		// a packet
		//
		if (!skirmish) {
			if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
				Session.MaxAhead = max( ((((SendPacket.ScenarioInfo.ResponseTime / 8) +
					(Session.FrameSendRate - 1)) / Session.FrameSendRate) *
					Session.FrameSendRate), (Session.FrameSendRate * 2)
											);
			} else {
				Session.MaxAhead = max( (SendPacket.ScenarioInfo.ResponseTime / 8),
											  MODEM_MIN_MAX_AHEAD );
			}
		}
		SendPacket.ID = Session.ModemType;

		if (!skirmish) {
			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);
			starttime = TickCount;
			while ( ( NullModem.Num_Send()
				&& ((TickCount - starttime) < PACKET_SENDING_TIMEOUT) )
				|| ((TickCount - starttime) < 60) ) {
				#if(SHOW_MONO)
				NullModem.Mono_Debug_Print(0);
				#endif

				NullModem.Service();
			}

			/*
			** Wait for the go response. This will be either a 'GO' reply, a
			** request for the scenario to be sent or a reply to say that the scenario
			** cant be played.
			*/
#ifdef WIN32
			WWDebugString ("RA95 - About to wait for 'GO' response.\n");
#endif
			do {
				NullModem.Service();

				if (NullModem.Get_Message (&ReceivePacket, &packetlen) > 0) {

#ifdef FIXIT_VERSION_3
 					if (ReceivePacket.Command == SERIAL_READY_TO_GO)
					{
						if( Session.Scenarios[Session.Options.ScenarioIndex]->Get_Official() )
						{
							if( !Force_Scenario_Available( Scen.ScenarioName ) )
								Emergency_Exit(EXIT_FAILURE);
						}
						break;
					}
#else
 					if (ReceivePacket.Command == SERIAL_READY_TO_GO) break;
#endif

					if (ReceivePacket.Command == SERIAL_NO_SCENARIO) {
						WWMessageBox().Process(TXT_NO_EXPANSION_SCENARIO, TXT_CANCEL);
						/*
						** We have to recover from this somehow so....
						*/
						process = true;
						display = REDRAW_ALL;
						lastmsgtime = TickCount;
						goto oh_dear_its_a_label;
					}

					if (ReceivePacket.Command == SERIAL_REQ_SCENARIO) {
#ifdef WIN32
						WWDebugString ("RA95 - About to call 'Send_Remote_File'.\n");

#endif

#ifdef FIXIT_VERSION_3
						if( Session.Scenarios[Session.Options.ScenarioIndex]->Get_Official() )
						{
							if( !Force_Scenario_Available( Scen.ScenarioName ) )
								Emergency_Exit(EXIT_FAILURE);
						}
#endif

						Send_Remote_File (Scen.ScenarioName, 0);

						break;
					}
				}

			} while ( !Keyboard->Check() );

		// clear queue to keep from doing any resends
			NullModem.Init_Send_Queue();
		}

	} else {
		if ( !recsignedoff ) {
			/*.....................................................................
			Broadcast my sign-off over my network
			.....................................................................*/
			if (!skirmish) {
				memset (&SendPacket, 0, sizeof(SerialPacketType));
				SendPacket.Command = SERIAL_SIGN_OFF;
				SendPacket.ScenarioInfo.Color = Session.ColorIdx;		// use Color for ID
				SendPacket.ID = Session.ModemType;
				NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

				starttime = TickCount;
				while ( (NullModem.Num_Send()
					&& ((TickCount - starttime) < PACKET_CANCEL_TIMEOUT) )
					|| ((TickCount - starttime) < 60) ) {
					#if(SHOW_MONO)
					NullModem.Mono_Debug_Print(0);
					#endif

					if ( NullModem.Get_Message( &ReceivePacket, &packetlen ) > 0) {

						// are we getting our own packets back??

						if (ReceivePacket.Command == SERIAL_SIGN_OFF
							&& ReceivePacket.ID == Session.ModemType) {

							// exit while
							break;
						}
					}

					NullModem.Service();
				}
			}
		}

		if (!skirmish) Shutdown_Modem();
	}

	/*------------------------------------------------------------------------
	Clear all lists
	------------------------------------------------------------------------*/
	while (scenariolist.Count()) {
		scenariolist.Remove_Item(scenariolist.Get_Item(0));
	}

	/*------------------------------------------------------------------------
	Clean up the list boxes
	------------------------------------------------------------------------*/
	while (playerlist.Count()>0) {
		item = (char *)playerlist.Get_Item(0);
		delete [] item;
		playerlist.Remove_Item(item);
	}

	/*------------------------------------------------------------------------
	Remove the chat edit box
	------------------------------------------------------------------------*/
	Session.Messages.Remove_Edit();

	/*------------------------------------------------------------------------
	Restore screen
	------------------------------------------------------------------------*/
	Hide_Mouse();
	Load_Title_Page(true);
	Show_Mouse();

	/*------------------------------------------------------------------------
	Save any changes made to our options
	------------------------------------------------------------------------*/
	if (changed) {
		Session.Write_MultiPlayer_Settings();
	}

	if (load_game && !skirmish) {
		if (!Load_Game (-1)) {
			WWMessageBox().Process (TXT_ERROR_LOADING_GAME);
			rc = false;
		}
		Frame++;
	}

	return(rc);
}


#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
extern bool Is_Mission_Aftermath (char *file_name);
#endif

/***********************************************************************************************
 * Find_Local_Scenario -- finds the file name of the scenario with matching attributes         *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to Scenario description                                                       *
 *           ptr to Scenario filename to fix up                                                *
 *           length of file for trivial rejection of scenario files                            *
 *           ptr to digest. Digests must match.                                                *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:   true if scenario is available locally                                             *
 *                                                                                             *
 * WARNINGS: We need to reject files that don't match exactly because scenarios with the same  *
 *           description can exist on both machines but have different contents. For example   *
 *           there will be lots of scenarios called 'my map' and 'aaaaaa'.          			  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/23/96 12:36PM ST : Created                                                             *
 *=============================================================================================*/
bool Find_Local_Scenario (char *description, char *filename, unsigned int length, char *digest, bool official)
{
//FILE *fp;
//fp = fopen("findscen.txt","wt");
//debugprint("looking for local scenario: description = %s, name=%s, length=%d, digest=%s, official=%d\n", description, filename, length, digest, official);

	char digest_buffer[32];
	/*
	** Scan through the scenario list looking for scenarios with matching descriptions.
	*/
	for (int index = 0; index < Session.Scenarios.Count(); index++) {

//debugprint( "Checking against scenario: %s\n", Session.Scenarios[index]->Description());
		if (!strcmp (Session.Scenarios[index]->Description(), description)) {
//debugprint("found matching description.\n");
			CCFileClass file (Session.Scenarios[index]->Get_Filename());

			/*
			** Possible rejection on the basis of availability.
			*/
			if (file.Is_Available()) {
//debugprint("file is available.\n");
				/*
				** Possible rejection on the basis of size.
				*/
				if (file.Size() == length) {
//debugprint("length matches.\n");
					/*
					** We don't know the digest for 'official' scenarios so assume its correct
					*/
					if (!official) {
//debugprint("!official.\n");
						/*
						** Possible rejection on the basis of digest
						*/
						INIClass ini;
						ini.Load(file);
						ini.Get_String ("Digest", "1", "No digest here mate. Nope.", digest_buffer, sizeof (digest_buffer) );
					}
//debugprint("digest = %s, digest_buffer = %s.\n", digest, digest_buffer);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98. But don't know why this happens. Because of autodownload?
					/*
					** If this is an aftermath scenario then ignore the digest and return success.
					*/
					if ( Is_Mission_Aftermath ((char*)Session.Scenarios[index]->Get_Filename()) ) {
//debugprint("a 1match!\n");
						strcpy (filename, Session.Scenarios[index]->Get_Filename());
						return (true);
					}
#endif

					/*
					** This must be the same scenario. Copy the name and return true.
					*/
					if (official || !strcmp (digest, digest_buffer)) {
//debugprint("a match!\n");
						strcpy (filename, Session.Scenarios[index]->Get_Filename());
						return (true);
					}
				}
			}
//			else
//				debugprint("file not available '%s'.\n", Session.Scenarios[index]->Get_Filename());

		}
	}
//debugprint("failed match.\n");
	/*
	** Couldnt find the scenario locally. Return failure.
	*/
	return (false);
}






/***********************************************************************************************
 * Com_Show_Scenario_Dialog -- Serial game scenario selection dialog									  *
 *                                                                         						  *
 * The 'Players' vector is filled in by this routine, when the game starts; this					  *
 * is for the Assign_Houses routine, which expects this vector to contain all						  *
 * players' names & houses & colors.  Other than that, the Players vector, Games					  *
 * vector, and Chat vector aren't used at all by this routine.  The Game & Players				  *
 * list boxes are filled in manually in the processing loop.											  *
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
 *		none.																												  *
 *                                                                         						  *
 * HISTORY:                                                                						  *
 *   02/14/1995 BR : Created.                                              						  *
 *=============================================================================================*/
int Com_Show_Scenario_Dialog(void)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	int d_dialog_w = 320 * RESFACTOR;									// dialog width
	int d_dialog_h = 200 * RESFACTOR;									// dialog height
	int d_dialog_x = ((320 * RESFACTOR - d_dialog_w) / 2);		// dialog x-coord
	int d_dialog_y = ((200 * RESFACTOR - d_dialog_h) / 2);		// dialog y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 6 * RESFACTOR+1;										// ht of 6-pt text
	int d_margin1 = 5 * RESFACTOR;										// margin width/height
	int d_margin2 = 2 * RESFACTOR;										// margin width/height

	int d_name_w = 70 * RESFACTOR;
	int d_name_h = 9 * RESFACTOR;
	int d_name_x = d_dialog_x + (d_dialog_w / 4) - (d_name_w / 2);
	int d_name_y = d_dialog_y + d_margin1 + d_margin2 + d_txt6_h + 1*RESFACTOR;

#ifdef OLDWAY
	int d_gdi_w = 40 * RESFACTOR;
	int d_gdi_h = 9 * RESFACTOR;
	int d_gdi_x = d_dialog_cx - d_gdi_w;
	int d_gdi_y = d_name_y;

	int d_nod_w = 40 * RESFACTOR;
	int d_nod_h = 9 * RESFACTOR;
	int d_nod_x = d_dialog_cx;
	int d_nod_y = d_name_y;

#else	//OLDWAY

	int d_house_w = 60 *RESFACTOR;
	int d_house_h = (8 * 5 *RESFACTOR);
	int d_house_x = d_dialog_cx - (d_house_w / 2);
	int d_house_y = d_name_y;

#endif	//OLDWAY

	int d_color_w = 10 * RESFACTOR;
	int d_color_h = 9 * RESFACTOR;
	int d_color_x = d_dialog_x + ((d_dialog_w / 4) * 3) - (d_color_w * 3);
	int d_color_y = d_name_y;

	int d_scenario_y = d_name_y + d_name_h + d_margin2;

	int d_gamelist_w = 160 * RESFACTOR;
	int d_gamelist_h = (6 * 6 * RESFACTOR) + 3 * RESFACTOR;		// 6 rows high
	int d_gamelist_x = d_dialog_x + d_margin1 + 10*RESFACTOR;
	int d_gamelist_y = d_scenario_y + d_txt6_h + d_margin2 + d_txt6_h + d_margin2;

//BG	int d_playerlist_w = 112 * RESFACTOR;
	int d_playerlist_w = 118 * RESFACTOR;
	int d_playerlist_h = (6 * 6 * RESFACTOR) + 3 * RESFACTOR;		// 6 rows high
	int d_playerlist_x = d_dialog_x + d_dialog_w - d_margin1 - d_margin1 - d_playerlist_w - 5*RESFACTOR;
	int d_playerlist_y = d_gamelist_y;

	int d_count_w = 25 * RESFACTOR;
	int d_count_h = d_txt6_h;
	int d_count_x = d_gamelist_x + (d_gamelist_w / 2);
	int d_count_y = d_gamelist_y + d_gamelist_h + (d_margin1 * 2) - d_margin2;

	int d_level_w = 25 * RESFACTOR;
	int d_level_h = d_txt6_h;
	int d_level_x = d_gamelist_x + (d_gamelist_w / 2);
	int d_level_y = d_count_y + d_count_h;

	int d_credits_w = 25 * RESFACTOR;
	int d_credits_h = d_txt6_h;
	int d_credits_x = d_gamelist_x + (d_gamelist_w / 2);
	int d_credits_y = d_level_y + d_level_h;

	int d_aiplayers_w = 25 * RESFACTOR;
	int d_aiplayers_h = d_txt6_h;
	int d_aiplayers_x = d_gamelist_x + (d_gamelist_w / 2);
	int d_aiplayers_y = d_credits_y + d_credits_h;

	int d_options_w = 112 * RESFACTOR;
	int d_options_h = (5 * 6* RESFACTOR) + 4*RESFACTOR;
	int d_options_x = d_playerlist_x;
	int d_options_y = d_playerlist_y + d_playerlist_h + d_margin1 - d_margin2;

	int d_message_w = d_dialog_w - (d_margin1 * 2) - 20*RESFACTOR;
	int d_message_h = (7 * d_txt6_h) + 3 * RESFACTOR;		// 7 rows high
	int d_message_x = d_gamelist_x;//d_dialog_x + d_margin1 + 10*RESFACTOR;
	int d_message_y = d_options_y + d_options_h + d_margin2/*KO + d_margin1*/;

	int d_send_w = d_message_w;
	int d_send_h = 9 * RESFACTOR;
	int d_send_x = d_message_x;
	int d_send_y = d_message_y + d_message_h;

	int d_cancel_w = 45 * RESFACTOR;
	int d_cancel_h = 9 * RESFACTOR;
	int d_cancel_x = d_dialog_cx - (d_cancel_w / 2);
	int d_cancel_y = d_send_y + d_send_h/*KO + d_margin2*/;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_NAME = 100,
#ifdef OLDWAY
		BUTTON_GDI,
		BUTTON_NOD,
#else	//OLDWAY
		BUTTON_HOUSE,
#endif	//OLDWAY
		BUTTON_GAMELIST,
		BUTTON_PLAYERLIST,
		BUTTON_CANCEL,
		BUTTON_COUNT,
		BUTTON_LEVEL,
		BUTTON_CREDITS,
		BUTTON_AI_PLAYERS,
		BUTTON_OPTIONS,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_PARMS,
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
	int cbox_x[] = {
							d_color_x,
							d_color_x + d_color_w,
							d_color_x + (d_color_w * 2),
							d_color_x + (d_color_w * 3),
							d_color_x + (d_color_w * 4),
							d_color_x + (d_color_w * 5),
							d_color_x + (d_color_w * 6),
							d_color_x + (d_color_w * 7)
							};

	char namebuf[MPLAYER_NAME_MAX] = {0};		// buffer for player's name
//BG	int playertabs[] = {77};				// tabs for player list box
	int playertabs[] = {71*RESFACTOR};				// tabs for player list box
	int optiontabs[] = {8};					// tabs for options list box
	bool transmit;										// 1 = re-transmit new game options
	bool first;											// 1 = no packets received yet
	bool parms_received = false;					// 1 = game options received
	bool changed = false;							// 1 = user has changed an option

	int rc;
	int recsignedoff = 0;
	int i;
	unsigned long version;
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
	bool load_game = false;					// 1 = load saved game
	NodeNameType *who;					// node to add to Players
	char *item;								// for filling in lists
	char *p;
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();
	Session.Options.ScenarioDescription[0] = 0;	//Flag that we dont know the scenario name yet
	bool messages_have_focus = true;
	bool ready_packet_was_sent = false;

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	EditClass name_edt(BUTTON_NAME, namebuf, MPLAYER_NAME_MAX, TPF_TEXT, d_name_x, d_name_y, d_name_w, d_name_h, EditClass::ALPHANUMERIC);
#ifdef OLDWAY
	TextButtonClass gdibtn(BUTTON_GDI, TXT_ALLIES, TPF_BUTTON, d_gdi_x, d_gdi_y, d_gdi_w, d_gdi_h);
	TextButtonClass nodbtn(BUTTON_NOD, TXT_SOVIET, TPF_BUTTON, d_nod_x, d_nod_y, d_nod_w, d_nod_h);
#else	//OLDWAY
	char housetext[25] = "";
	Fancy_Text_Print("", 0, 0, 0, 0, TPF_TEXT);
	DropListClass housebtn(BUTTON_HOUSE, housetext, sizeof(housetext),
		TPF_TEXT,
		d_house_x, d_house_y, d_house_w, d_house_h,
		MFCD::Retrieve("BTN-UP.SHP"),
		MFCD::Retrieve("BTN-DN.SHP"));
#endif	//OLDWAY
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w);
	ListClass gamelist(BUTTON_GAMELIST, d_gamelist_x, d_gamelist_y, d_gamelist_w, d_gamelist_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
	ColorListClass playerlist(BUTTON_PLAYERLIST, d_playerlist_x, d_playerlist_y, d_playerlist_w, d_playerlist_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));

	GaugeClass countgauge(BUTTON_COUNT, d_count_x, d_count_y, d_count_w, d_count_h);
	char staticcountbuff[35];
	StaticButtonClass staticcount(0, "     ", TPF_TEXT, d_count_x+d_count_w+3*RESFACTOR, d_count_y);

	GaugeClass levelgauge(BUTTON_LEVEL, d_level_x, d_level_y, d_level_w, d_level_h);
	char staticlevelbuff[35];
	StaticButtonClass staticlevel(0, "     ", TPF_TEXT, d_level_x+d_level_w+3*RESFACTOR, d_level_y);

	GaugeClass creditsgauge(BUTTON_CREDITS, d_credits_x, d_credits_y, d_credits_w, d_credits_h);
	char staticcreditsbuff[35];
	StaticButtonClass staticcredits(0, "         ", TPF_TEXT, d_credits_x+d_credits_w+3*RESFACTOR, d_credits_y);

	GaugeClass aiplayersgauge(BUTTON_AI_PLAYERS, d_aiplayers_x, d_aiplayers_y, d_aiplayers_w, d_aiplayers_h);
	char staticaibuff[35];
	StaticButtonClass staticai(0, "     ", TPF_TEXT, d_aiplayers_x+d_aiplayers_w+3*RESFACTOR, d_aiplayers_y);

	CheckListClass optionlist(BUTTON_OPTIONS, d_options_x, d_options_y, d_options_w, d_options_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));

	/*
	------------------------- Build the button list --------------------------
	*/
	commands = &name_edt;
	staticcount.Add_Tail(*commands);
	staticcredits.Add_Tail(*commands);
	staticai.Add_Tail(*commands);
	staticlevel.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);
	gamelist.Add_Tail(*commands);
	playerlist.Add_Tail(*commands);
	countgauge.Add_Tail(*commands);
	levelgauge.Add_Tail(*commands);
	creditsgauge.Add_Tail(*commands);
	aiplayersgauge.Add_Tail(*commands);
	optionlist.Add_Tail(*commands);
#ifdef OLDWAY
	gdibtn.Add_Tail(*commands);
	nodbtn.Add_Tail(*commands);
#else	//OLDWAY
	housebtn.Add_Tail(*commands);
#endif	//OLDWAY


	//------------------------------------------------------------------------
	//	Init the button states
	//------------------------------------------------------------------------
	//........................................................................
	// Name & Color
	//........................................................................
	Session.ColorIdx = Session.PrefColor;			// init my preferred color
	strcpy (namebuf, Session.Handle);				// set my name
	name_edt.Set_Text(namebuf,MPLAYER_NAME_MAX);
	name_edt.Set_Color(&ColorRemaps[(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : Session.ColorIdx]);

	//........................................................................
	// List boxes
	//........................................................................
	playerlist.Set_Tabs(playertabs);
	playerlist.Set_Selected_Style(ColorListClass::SELECT_NORMAL);

	optionlist.Set_Tabs(optiontabs);
	optionlist.Set_Read_Only(1);

	optionlist.Add_Item(Text_String(TXT_BASES));
	optionlist.Add_Item(Text_String(TXT_ORE_SPREADS));
	optionlist.Add_Item(Text_String(TXT_CRATES));
	optionlist.Add_Item(Text_String(TXT_CAPTURE_THE_FLAG));
	optionlist.Add_Item(Text_String(TXT_SHADOW_REGROWS));

	optionlist.Check_Item(0, Session.Options.Bases);
	optionlist.Check_Item(1, Session.Options.Tiberium);
	optionlist.Check_Item(2, Session.Options.Goodies);
	optionlist.Check_Item(3, Special.IsCaptureTheFlag);
	optionlist.Check_Item(4, Special.IsShadowGrow);

	//........................................................................
	// House buttons
	//........................................................................
#ifdef OLDWAY
	if (Session.House==HOUSE_GOOD) {
		gdibtn.Turn_On();
	} else {
		nodbtn.Turn_On();
	}
#else	//OLDWAY
	for (HousesType house = HOUSE_USSR; house <= HOUSE_FRANCE; house++) {
		housebtn.Add_Item(Text_String(HouseTypeClass::As_Reference(house).Full_Name()));
	}
	housebtn.Set_Selected_Index(Session.House - HOUSE_USSR);
	housebtn.Set_Read_Only (true);
#endif	//OLDWAY

	//........................................................................
	// Option gauges
	//........................................................................
	countgauge.Use_Thumb(0);
	countgauge.Set_Maximum(SessionClass::CountMax[Session.Options.Bases] - SessionClass::CountMin[Session.Options.Bases]);
	countgauge.Set_Value(Session.Options.UnitCount - SessionClass::CountMin[Session.Options.Bases]);

	levelgauge.Use_Thumb(0);
	levelgauge.Set_Maximum(MPLAYER_BUILD_LEVEL_MAX - 1);
	levelgauge.Set_Value(BuildLevel - 1);

	creditsgauge.Use_Thumb(0);
	creditsgauge.Set_Maximum(Rule.MPMaxMoney);
	creditsgauge.Set_Value(Session.Options.Credits);

	aiplayersgauge.Use_Thumb(0);
	aiplayersgauge.Set_Maximum(Rule.MaxPlayers-2);
	aiplayersgauge.Set_Value(Session.Options.AIPlayers);

	Fancy_Text_Print("", 0, 0, scheme, TBLACK, TPF_CENTER | TPF_TEXT);

	transmit = true;
	first = true;

	/*........................................................................
	Clear the Players vector
	........................................................................*/
	Clear_Vector(&Session.Players);

	/*........................................................................
	Init the message display system
	........................................................................*/
	Session.Messages.Init (d_message_x + 1, d_message_y + 1, 7,
		MAX_MESSAGE_LENGTH, d_txt6_h, d_send_x + 1 * RESFACTOR, d_send_y + 1 * RESFACTOR, 1,
		20, MAX_MESSAGE_LENGTH - 5, d_message_w);
	Session.Messages.Add_Edit ((Session.ColorIdx == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : Session.ColorIdx,
										TPF_TEXT, NULL, '_', d_message_w);
	Session.WWChat = 0;

	/*........................................................................
	Init version number clipping system
	........................................................................*/
	VerNum.Init_Clipping();
	Load_Title_Page(true);
	CCPalette.Set();

	extern char ModemRXString[];

	if (strlen(ModemRXString) > 36)
		ModemRXString[36] = 0;

	if (strlen(ModemRXString) > 0)
		Session.Messages.Add_Message (NULL, 0, ModemRXString, PCOLOR_BROWN,
			TPF_TEXT, -1);

	ModemRXString[0] = '\0';

	/*
	---------------------------- Init Mono Output ----------------------------
	*/
	#if(SHOW_MONO)
	NullModem.Configure_Debug(sizeof (CommHeaderType),sizeof (SerialCommandType),
		SerialPacketNames, 100, 8);
	NullModem.Mono_Debug_Print(1);
	#endif

	/*
	---------------------------- Processing loop -----------------------------
	*/
	NullModem.Reset_Response_Time();		// clear response time
	timingtime = lastmsgtime = lastredrawtime = TickCount;

	bool process = true;						// process while true
	while (process) {
		#if(SHOW_MONO)
		NullModem.Mono_Debug_Print(0);
		#endif

		/*
		** Kludge to make sure we redraw the message input line when it loses focus.
		** If we dont do this then the cursor doesnt disappear.
		*/
		if (messages_have_focus) {
			if (name_edt.Has_Focus()) {
				if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
				messages_have_focus = false;
			}
		} else {
			if (!name_edt.Has_Focus()) {
				messages_have_focus = true;
				if (display < REDRAW_MESSAGE)	display = REDRAW_MESSAGE;
				Session.Messages.Set_Edit_Focus();
			}
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}
		#endif

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			if (housebtn.IsDropped) {
				housebtn.Collapse();
				display = REDRAW_BACKGROUND;
			}
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Fancy_Text_Print(TXT_CHANNEL_GAMES, d_gamelist_x + (d_gamelist_w / 2), d_gamelist_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
				Fancy_Text_Print(TXT_PLAYERS, d_playerlist_x + (d_playerlist_w / 2), d_playerlist_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
				Fancy_Text_Print(TXT_YOUR_NAME, d_name_x + (d_name_w / 2), d_name_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
#ifdef OLDWAY
				Fancy_Text_Print(TXT_SIDE_COLON, d_gdi_x + d_gdi_w, d_gdi_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
#else
				Fancy_Text_Print(TXT_SIDE_COLON, d_house_x + (d_house_w / 2), d_house_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
#endif
				Fancy_Text_Print(TXT_COLOR_COLON, d_dialog_x + ((d_dialog_w / 4) * 3), d_color_y - d_txt6_h, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
				Fancy_Text_Print (TXT_COUNT, d_count_x - 2 * RESFACTOR, d_count_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
				Fancy_Text_Print (TXT_LEVEL, d_level_x - 2 * RESFACTOR, d_level_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
				Fancy_Text_Print (TXT_CREDITS_COLON, d_credits_x - 2 * RESFACTOR, d_credits_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
				Fancy_Text_Print (TXT_AI_PLAYERS_COLON, d_aiplayers_x - 2 * RESFACTOR, d_aiplayers_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);

			}

			/*..................................................................
			Draw the color boxes
			..................................................................*/
			if (display >= REDRAW_COLORS) {
				for (i = 0; i < MAX_MPLAYER_COLORS; i++) {
					LogicPage->Fill_Rect (cbox_x[i] + 1 * RESFACTOR, d_color_y + 1 * RESFACTOR,
						cbox_x[i] + 1 * RESFACTOR + d_color_w - 2 * RESFACTOR, d_color_y + 1 * RESFACTOR + d_color_h - 2 * RESFACTOR,
						ColorRemaps[i].Box);
//						(i == PCOLOR_DIALOG_BLUE) ? ColorRemaps[PCOLOR_REALLY_BLUE].Box : ColorRemaps[i].Box);

					if (i == Session.ColorIdx) {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h,
							BOXSTYLE_DOWN, false);
					} else {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h,
							BOXSTYLE_RAISED, false);
					}
				}
			}

			/*..................................................................
			Draw the message system; erase old messages first
			..................................................................*/
			if (display >= REDRAW_MESSAGE) {
				Draw_Box(d_message_x, d_message_y, d_message_w, d_message_h,
					BOXSTYLE_BOX, true);
				Draw_Box(d_send_x, d_send_y, d_send_w, d_send_h,
					BOXSTYLE_BOX, true);
				Session.Messages.Draw();

			//..................................................................
			// Redraw the game options
			//..................................................................
			if (display >= REDRAW_PARMS && parms_received) {
					if (oppscorescreen) {
						sprintf(txt,"%s",Text_String(TXT_WAITING_FOR_OPPONENT));

						int txtwidth = String_Pixel_Width( txt );

						Fancy_Text_Print(txt, d_dialog_cx, d_scenario_y, scheme, TBLACK, TPF_CENTER | TPF_TEXT);
					} else {
						/*............................................................
						Scenario description
						............................................................*/
						//LogicPage->Fill_Rect(d_dialog_x + 16*RESFACTOR, d_scenario_y,
						//	d_dialog_x + d_dialog_w - 16*RESFACTOR, d_scenario_y + d_txt6_h, BLACK);

						p = (char *)Text_String(TXT_SCENARIO_COLON);
						if (Session.Options.ScenarioDescription[0]) {
//							sprintf(txt,"%s %s",p, Session.Options.ScenarioDescription);
//							Fancy_Text_Print (txt, d_dialog_cx, d_scenario_y, scheme, TBLACK, TPF_TEXT | TPF_CENTER);

							// EW - Scenario language translation goes here!!!!!!!! VG
							for (i = 0; EngMisStr[i] != NULL;  i++) {
								if (!strcmp(Session.Options.ScenarioDescription, EngMisStr[i])) {
#if defined(GERMAN) || defined(FRENCH)
									sprintf(txt, "%s %s", p, EngMisStr[i+1]);
#else
									sprintf(txt, "%s %s", p, Session.Options.ScenarioDescription);
#endif
									break;
								}
							}
							if (EngMisStr[i] == NULL) {
								sprintf(txt, "%s %s", p, Session.Options.ScenarioDescription);
							}
							Fancy_Text_Print (txt, d_dialog_cx, d_scenario_y, scheme, TBLACK, TPF_TEXT | TPF_CENTER);

						} else {
							sprintf(txt,"%s %s",p,Text_String(TXT_NOT_FOUND));

							Fancy_Text_Print (txt, d_dialog_cx, d_scenario_y, &ColorRemaps[PCOLOR_RED], TBLACK, TPF_TEXT | TPF_CENTER);
						}

						//.........................................................
						// Unit count, tech level, credits
						//.........................................................
						//LogicPage->Fill_Rect(d_count_x + d_count_w + 2 * RESFACTOR, d_count_y,
						//	d_count_x + d_count_w + 35 * RESFACTOR, d_aiplayers_y + d_aiplayers_h+RESFACTOR,
						//	BLACK);

						sprintf(staticcountbuff, "%d", Session.Options.UnitCount);
						staticcount.Set_Text(staticcountbuff);
						staticcount.Draw_Me();
						if (BuildLevel <= MPLAYER_BUILD_LEVEL_MAX) {
							sprintf(staticlevelbuff, "%d ", BuildLevel);
						} else {
							sprintf(staticlevelbuff, "**");
						}
						staticlevel.Set_Text(staticlevelbuff);
						staticlevel.Draw_Me();

						sprintf(staticcreditsbuff, "%d", Session.Options.Credits);
						staticcredits.Set_Text(staticcreditsbuff);
						staticcredits.Draw_Me();

						sprintf(staticaibuff, "%d", Session.Options.AIPlayers);
						staticai.Set_Text(staticaibuff);
						staticai.Draw_Me();
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
		messages_have_focus = Session.Messages.Has_Edit_Focus();
		bool droplist_is_dropped = housebtn.IsDropped;
		KeyNumType input = commands->Input();

		/*
		** Sort out the input focus between the name edit box and the message system
		*/
		if (messages_have_focus) {
			if (!name_edt.Has_Focus()) {
				Session.Messages.Set_Edit_Focus();
			} else {
				messages_have_focus = false;
				if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
			}
		}

		/*
		** Redraw everything if the droplist collapsed
		*/
		if (droplist_is_dropped && !housebtn.IsDropped) {
			display = REDRAW_BACKGROUND;
		}

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {
			/*------------------------------------------------------------------
			User clicks on a color button
			------------------------------------------------------------------*/
			case KN_LMOUSE:
				if (Keyboard->MouseQX > cbox_x[0] &&
					Keyboard->MouseQX < (cbox_x[MAX_MPLAYER_COLORS - 1] + d_color_w) &&
					Keyboard->MouseQY > d_color_y &&
					Keyboard->MouseQY < (d_color_y + d_color_h)) {
						/*.........................................................
						Compute my preferred color as the one I clicked on.
						.........................................................*/
						Session.PrefColor = (PlayerColorType)
							((Keyboard->MouseQX - cbox_x[0]) / d_color_w);
						changed = true;

						/*.........................................................
						If 'TheirColor' is set to the other player's color, make
						sure we can't pick that color.
						.........................................................*/
						if (parms_received) {
							if (Session.PrefColor == TheirColor)
								break;
						}
						Session.ColorIdx = Session.PrefColor;

						name_edt.Set_Color(&ColorRemaps[(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ?
																				PCOLOR_REALLY_BLUE : Session.ColorIdx]);
						name_edt.Flag_To_Redraw();
						Session.Messages.Set_Edit_Color((Session.ColorIdx == PCOLOR_DIALOG_BLUE) ?
																				PCOLOR_REALLY_BLUE : Session.ColorIdx);
						if (display < REDRAW_COLORS) display = REDRAW_COLORS;
						strcpy (Session.Handle, namebuf);
						transmit = true;
						if (housebtn.IsDropped) {
							housebtn.Collapse();
							display = REDRAW_BACKGROUND;
						}
				} else if ( (Get_Mouse_X() >= d_count_x &&
					Get_Mouse_X() <= d_count_x + d_count_w &&
					Get_Mouse_Y() >= d_count_y &&
					Get_Mouse_Y() <= d_aiplayers_y + d_aiplayers_h) ||
					(Get_Mouse_X() >= d_options_x &&
					Get_Mouse_X() <= d_options_x + d_options_w &&
					Get_Mouse_Y() >= d_options_y &&
					Get_Mouse_Y() <= d_options_y + d_options_h) ) {
					Session.Messages.Add_Message(NULL, 0, (char *)Text_String(TXT_ONLY_HOST_CAN_MODIFY), PCOLOR_BROWN, TPF_TEXT, 1200);
					Sound_Effect(VOC_SYS_ERROR);
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
					if (housebtn.IsDropped) {
						housebtn.Collapse();
						display = REDRAW_BACKGROUND;
					}
				}

				break;

#ifdef OLDWAY
			/*------------------------------------------------------------------
			House Buttons: set the player's desired House
			------------------------------------------------------------------*/
			case (BUTTON_GDI | KN_BUTTON):
				Session.House = HOUSE_GOOD;
				gdibtn.Turn_On();
				nodbtn.Turn_Off();
				strcpy (Session.Handle, namebuf);
				transmit = true;
				break;

			case (BUTTON_NOD | KN_BUTTON):
				Session.House = HOUSE_BAD;
				gdibtn.Turn_Off();
				nodbtn.Turn_On();
				strcpy (Session.Handle, namebuf);
				transmit = true;
				break;
#else	//OLDWAY
			case (BUTTON_HOUSE | KN_BUTTON):
				Session.House = HousesType(housebtn.Current_Index()+HOUSE_USSR);
				strcpy (Session.Handle, namebuf);
				transmit = true;
				//display = REDRAW_BACKGROUND;
				break;
#endif	//OLDWAY

			/*------------------------------------------------------------------
			User edits the name value; retransmit
			------------------------------------------------------------------*/
			case (BUTTON_NAME | KN_BUTTON):
				if (housebtn.IsDropped) {
					housebtn.Collapse();
					display = REDRAW_BACKGROUND;
				}
				strcpy (Session.Handle, namebuf);
				transmit = true;
				changed = true;
				break;

			/*------------------------------------------------------------------
			CANCEL: send a SIGN_OFF, bail out with error code
			------------------------------------------------------------------*/
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				process = false;
				rc = false;
				break;

			/*------------------------------------------------------------------
			Default: manage the inter-player messages
			------------------------------------------------------------------*/
			default:
				if (Session.Messages.Manage()) {
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
				}

				/*...............................................................
				Service keyboard input for any message being edited.
				...............................................................*/
				i = Session.Messages.Input(input);

				/*...............................................................
				If 'Input' returned 1, it means refresh the message display.
				(We have to redraw the edit line, to erase the cursor.)
				...............................................................*/
				if (i==1) {
					Hide_Mouse();
					Draw_Box(d_send_x, d_send_y, d_send_w, d_send_h,
						BOXSTYLE_BOX, true);
					Session.Messages.Draw();
					Show_Mouse();
				} else if (i==2) {
				/*...............................................................
				If 'Input' returned 2, it means redraw the message display.
				Rather than setting 'display', which would redraw all msgs,
				we only need to erase & redraw the edit box here.
				...............................................................*/
					Hide_Mouse();
					Draw_Box(d_send_x, d_send_y, d_send_w, d_send_h,
						BOXSTYLE_BOX, true);
					Session.Messages.Draw();
					Show_Mouse();
				} else if (i==3 || i==4) {
				/*...............................................................
				If 'input' returned 3, it means send the current message.
				...............................................................*/
					memset (&SendPacket, 0, sizeof(SerialPacketType));
					SendPacket.Command = SERIAL_MESSAGE;
					strcpy (SendPacket.Name, namebuf);
					SendPacket.ID = Session.ColorIdx;
					if (i==3) {
						strcpy (SendPacket.Message.Message, Session.Messages.Get_Edit_Buf());
					} else {
						strcpy (SendPacket.Message.Message, Session.Messages.Get_Overflow_Buf());
						Session.Messages.Clear_Overflow_Buf();
					}

					/*..................................................................
					Send the message
					..................................................................*/
					NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);
					NullModem.Service();

					/*..................................................................
					Add the message to our own screen
					..................................................................*/
					Session.Messages.Add_Message (SendPacket.Name, SendPacket.ID,
						SendPacket.Message.Message,
						(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : Session.ColorIdx,
						TPF_TEXT, -1);
					Session.Messages.Add_Edit((Session.ColorIdx == PCOLOR_DIALOG_BLUE) ?
																				PCOLOR_REALLY_BLUE : Session.ColorIdx,
						TPF_TEXT, NULL, '_', d_message_w);
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
				}
				break;
		}

		/*---------------------------------------------------------------------
		Detect editing of the name buffer, transmit new values to players
		---------------------------------------------------------------------*/
		if (strcmp (namebuf, Session.Handle)) {
			strcpy (Session.Handle, namebuf);
			transmit = true;
			changed = true;
		}

		/*---------------------------------------------------------------------
		If our Transmit flag is set, we need to send out a game option packet
		---------------------------------------------------------------------*/
		if (transmit && (TickCount - transmittime) > PACKET_RETRANS_TIME) {
			memset (&SendPacket, 0, sizeof(SerialPacketType));
			SendPacket.Command = SERIAL_GAME_OPTIONS;
			strcpy (SendPacket.Name, namebuf);
			SendPacket.ScenarioInfo.CheatCheck = RuleINI.Get_Unique_ID();
			SendPacket.ScenarioInfo.MinVersion = VerNum.Min_Version();
			SendPacket.ScenarioInfo.MaxVersion = VerNum.Max_Version();
			SendPacket.ScenarioInfo.House = Session.House;
			SendPacket.ScenarioInfo.Color = Session.ColorIdx;
			SendPacket.ID = Session.ModemType;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			transmittime = TickCount;
			transmit = false;

			//..................................................................
			// Keep the player list up to date
			//..................................................................
			if (playerlist.Count()) {
				item = (char *)playerlist.Get_Item(0);
#ifdef	OLDWAY
				if (Session.House==HOUSE_GOOD) {
					sprintf(item,"%s\t%s",namebuf,
						Text_String(TXT_ALLIES));
				} else {
					sprintf(item,"%s\t%s",namebuf,
						Text_String(TXT_SOVIET));
				}
#else	//OLDWAY
				sprintf (item, "%s\t%s", namebuf, Text_String(HouseTypeClass::As_Reference(Session.House).Full_Name()));
#endif	//OLDWAY
				playerlist.Colors[0] = &ColorRemaps[(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ?
																				PCOLOR_REALLY_BLUE : Session.ColorIdx];
				playerlist.Flag_To_Redraw();
			}

			//..................................................................
			// Play a little sound effect
			//..................................................................
			Sound_Effect(VOC_OPTIONS_CHANGED);
		}

		//
		// send a timing packet if enough time has gone by.
		//
		if ((TickCount - timingtime) > PACKET_TIMING_TIMEOUT) {
			memset (&SendPacket, 0, sizeof(SerialPacketType));
			SendPacket.Command = SERIAL_TIMING;
			SendPacket.ScenarioInfo.ResponseTime = NullModem.Response_Time();
			SendPacket.ID = Session.ModemType;

			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 0);
			timingtime = TickCount;
		}

		/*---------------------------------------------------------------------
		Check for an incoming message
		---------------------------------------------------------------------*/
		if (NullModem.Get_Message(&ReceivePacket, &packetlen) > 0) {

			lastmsgtime = TickCount;

			msg_timeout = 600;

			// are we getting our own packets back??

			if (ReceivePacket.Command >= SERIAL_CONNECT &&
				ReceivePacket.Command < SERIAL_LAST_COMMAND &&
				ReceivePacket.Command != SERIAL_MESSAGE &&
				ReceivePacket.ID == Session.ModemType) {

				WWMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);

				// to skip the other system not responding msg
				lastmsgtime = TickCount;

				process = false;
				rc = false;

				// say we did receive sign off to keep from sending one
				recsignedoff = true;
				break;
			}

			event = (EventClass *)&ReceivePacket;
			if (event->Type <= EventClass::FRAMEINFO) {
				if ( (TickCount - lastredrawtime) > PACKET_REDRAW_TIME) {
					lastredrawtime = TickCount;
					oppscorescreen = true;
					if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
					parms_received = true;
				}
			} else {
				switch ( ReceivePacket.Command ) {
					/*..................................................................
					Other system signs off:  Give it time to receive my ACK, then show
					a message.
					..................................................................*/
					case (SERIAL_SIGN_OFF):
						starttime = TickCount;
						while ( (TickCount - starttime) < 60)
							NullModem.Service();
						WWMessageBox().Process(TXT_USER_SIGNED_OFF);

						// to skip the other system not responding msg
						lastmsgtime = TickCount;

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
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
						parms_received = true;

						strcpy (TheirName, ReceivePacket.Name);
						TheirColor = ReceivePacket.ScenarioInfo.Color;
						TheirHouse = ReceivePacket.ScenarioInfo.House;

						/*...............................................................
						Make sure I don't have the same color as the other guy.
						...............................................................*/
						if (Session.ColorIdx == TheirColor) {

							// force transmitting of game options packet

							transmit = true;
							transmittime = 0;

							Session.ColorIdx = (PlayerColorType)(TheirColor + 1);
							if (Session.ColorIdx >= 6) {
								Session.ColorIdx = PCOLOR_FIRST;
							}
							name_edt.Set_Color(&ColorRemaps[(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : Session.ColorIdx]);
							name_edt.Flag_To_Redraw();
							if (display < REDRAW_COLORS) display = REDRAW_COLORS;
							if (housebtn.IsDropped) {
								housebtn.Collapse();
								display = REDRAW_BACKGROUND;
							}
						}

						/*...............................................................
						Save scenario settings.
						...............................................................*/
						Session.Options.Credits = ReceivePacket.ScenarioInfo.Credits;
						Session.Options.Bases = ReceivePacket.ScenarioInfo.IsBases;
						Session.Options.Tiberium = ReceivePacket.ScenarioInfo.IsTiberium;
						Session.Options.Goodies = ReceivePacket.ScenarioInfo.IsGoodies;
						Session.Options.AIPlayers = ReceivePacket.ScenarioInfo.AIPlayers;
						BuildLevel = ReceivePacket.ScenarioInfo.BuildLevel;
						Session.Options.UnitCount = ReceivePacket.ScenarioInfo.UnitCount;
						Seed = ReceivePacket.ScenarioInfo.Seed;
						Special = ReceivePacket.ScenarioInfo.Special;
						Options.GameSpeed = ReceivePacket.ScenarioInfo.GameSpeed;

						if (Session.Options.Tiberium) {
							Special.IsTGrowth = true;
							Rule.IsTGrowth = true;
							Special.IsTSpread = true;
							Rule.IsTSpread = true;
						} else {
							Special.IsTGrowth = false;
							Rule.IsTGrowth = false;
							Special.IsTSpread = false;
							Rule.IsTSpread = false;
						}

						//.........................................................
						// Adjust the gauges
						//.........................................................
						countgauge.Set_Maximum(
							SessionClass::CountMax[Session.Options.Bases] -
							SessionClass::CountMin[Session.Options.Bases]);
						countgauge.Set_Value(Session.Options.UnitCount -
							SessionClass::CountMin[Session.Options.Bases]);
						levelgauge.Set_Value(BuildLevel - 1);
						creditsgauge.Set_Value(Session.Options.Credits);
						aiplayersgauge.Set_Value(Session.Options.AIPlayers);

						//.........................................................
						// Update the options list box
						//.........................................................
						optionlist.Check_Item(0, Session.Options.Bases);
						optionlist.Check_Item(1, Session.Options.Tiberium);
						optionlist.Check_Item(2, Session.Options.Goodies);
						optionlist.Check_Item(3, Special.IsCaptureTheFlag);
						optionlist.Check_Item(4, Special.IsShadowGrow);
						optionlist.Flag_To_Redraw();

						/*
						** If the scenario name changed then we need to redraw the whole lot.
						*/
						if (strcmp (Session.Options.ScenarioDescription, ReceivePacket.ScenarioInfo.Scenario)) {
							if (display < REDRAW_BACKGROUND) display = REDRAW_BACKGROUND;
						}

						/*...............................................................
						Copy the information about the scenario that the host wants to
						play so ee can request this scenario from the host if we don't
						have it locally.
						...............................................................*/
						strcpy (Session.Options.ScenarioDescription, ReceivePacket.ScenarioInfo.Scenario);
						strcpy (Session.ScenarioFileName, ReceivePacket.ScenarioInfo.ShortFileName);
#ifdef WOLAPI_INTEGRATION
						strncpy (Session.ScenarioDigest, (char*)ReceivePacket.ScenarioInfo.FileDigest, sizeof( ReceivePacket.ScenarioInfo.FileDigest ));
#else
						strcpy (Session.ScenarioDigest, (char*)ReceivePacket.ScenarioInfo.FileDigest);
#endif
						Session.ScenarioIsOfficial = ReceivePacket.ScenarioInfo.OfficialScenario;
						Session.ScenarioFileLength = ReceivePacket.ScenarioInfo.FileLength;

						//.........................................................
						// "Clip" the other system's version range to our own
						// ........................................................
						version = VerNum.Clip_Version(ReceivePacket.ScenarioInfo.MinVersion,
							ReceivePacket.ScenarioInfo.MaxVersion);
						// ........................................................
						// If the greatest-common-version comes back 0, the other
						// system's range is too low for ours
						// ........................................................
						if (version == 0) {
							WWMessageBox().Process (TXT_DESTGAME_OUTDATED);

							// to skip the other system not responding msg
							lastmsgtime = TickCount;

							process = false;
							rc = false;
						} else if (version == 0xffffffff) {
							// ........................................................
							// If the greatest-common-version comes back 0xffffffff,
							// the other system's range is too high for ours
							// ........................................................
							WWMessageBox().Process (TXT_YOURGAME_OUTDATED);

							// to skip the other system not responding msg
							lastmsgtime = TickCount;

							process = false;
							rc = false;
						}
						// ........................................................
						// Otherwise, 'version' is the highest version we have in
						// common; look up the protocol that goes with this version.
						// ........................................................
						else {

							if (ReceivePacket.ScenarioInfo.CheatCheck != RuleINI.Get_Unique_ID()) {
								WWMessageBox().Process(TXT_MISMATCH);

								// to skip the other system not responding msg
								lastmsgtime = TickCount;

								process = false;
								rc = false;
							} else {

								Session.CommProtocol = VerNum.Version_Protocol(version);
#ifndef FIXIT_VERSION_3
								PlayingAgainstVersion = version;
#endif
							}
						}

						/*.........................................................
						If this is the first game-options packet we've received,
						init the game & player lists, then transmit our options
						to him.
						.........................................................*/
						if (first) {
							//......................................................
							// Add a string to the game list, and two to the player
							// list
							//......................................................
							item = new char [MPLAYER_NAME_MAX + 64];
							gamelist.Add_Item(item);
							item = new char [MPLAYER_NAME_MAX + 64];	//Need room to display country name
							playerlist.Add_Item(item,
								&ColorRemaps[(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ?
																			PCOLOR_REALLY_BLUE : Session.ColorIdx]);
							item = new char [MPLAYER_NAME_MAX + 64];	//Need room to display country name
							playerlist.Add_Item(item,
								&ColorRemaps[(TheirColor == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : TheirColor]);

							first = false;
							transmit = true;
							transmittime = 0;
						}

						//.........................................................
						// Ensure the game list & player list have the latest,
						// greatest copy of our names & colors.  Do this every time
						// we receive an options packet.
						//.........................................................
						item = (char *)gamelist.Get_Item(0);
						sprintf(item,Text_String(TXT_THATGUYS_GAME),
							TheirName);
						item = (char *)playerlist.Get_Item(0);
#ifdef	OLDWAY
						if (Session.House==HOUSE_GOOD) {
							sprintf(item,"%s\t%s",namebuf,
								Text_String(TXT_ALLIES));
						} else {
							sprintf(item,"%s\t%s",namebuf,
								Text_String(TXT_SOVIET));
						}
#else	//OLDWAY
						sprintf (item, "%s\t%s", namebuf, Text_String(HouseTypeClass::As_Reference(Session.House).Full_Name()));

#endif	//OLDWAY
						playerlist.Colors[0] = &ColorRemaps[(Session.ColorIdx == PCOLOR_DIALOG_BLUE) ?
																				PCOLOR_REALLY_BLUE : Session.ColorIdx];

						item = (char *)playerlist.Get_Item(1);
#ifdef	OLDWAY
						if (TheirHouse==HOUSE_GOOD) {
							sprintf(item,"%s\t%s",TheirName,
								Text_String(TXT_ALLIES));
						} else {
							sprintf(item,"%s\t%s",TheirName,
								Text_String(TXT_SOVIET));
						}
#else	//OLDWAY
						sprintf (item, "%s\t%s", TheirName, Text_String(HouseTypeClass::As_Reference(TheirHouse).Full_Name()));
#endif	//OLDWAY
						playerlist.Colors[1] = &ColorRemaps[(TheirColor == PCOLOR_DIALOG_BLUE) ? PCOLOR_REALLY_BLUE : TheirColor];

						gamelist.Flag_To_Redraw();
						playerlist.Flag_To_Redraw();

						//.........................................................
						// Play a little sound effect
						//.........................................................
						Sound_Effect(VOC_OPTIONS_CHANGED);

						break;

					/*..................................................................
					GO: Exit this routine with a success code.
					..................................................................*/
					case (SERIAL_LOADGAME):
						load_game = true;
					case (SERIAL_GO):

						ready_packet_was_sent = false;

						if (!load_game){
							/*
							** Special new kludge for counterstrike.
							**
							** Find local scenario will fail to match a counterstrike mission
							** unless the CS CD is in the drive. So....
							**
							** If Counterstrike is installed and this is an official map and
							** the file name matches a counterstrike map then tell the host
							** that I have the scenario so he can continue while we make
							** sure the local user has the Counterstrike CD in the drive.
							**
							*/
							//	This is duplicated for Aftermath scenarios. ajw

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
							if (Session.ScenarioIsOfficial &&
								( (Expansion_CS_Present() && Is_Mission_Counterstrike(Session.ScenarioFileName)) ||
								  (Expansion_AM_Present() && Is_Mission_Aftermath(Session.ScenarioFileName)) ) ) {
#else
							if ( Expansion_CS_Present() && Session.ScenarioIsOfficial ) {
#endif

								CCFileClass check_file ( Session.ScenarioFileName );
								if ( !check_file.Is_Available() ) {

									int current_drive = CCFileClass::Get_CD_Drive();
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
									int index = Get_CD_Index(current_drive, 1*60);
									bool needcd = false;
									if (Is_Mission_Counterstrike(Session.ScenarioFileName)) {
										if (index != 2 && index != 3) {
											RequiredCD = 2;
											needcd = true;
										}
									}
									if (Is_Mission_Aftermath(Session.ScenarioFileName)) {
										if (index != 3) {
											RequiredCD = 3;
											needcd = true;
										}
									}
									if (needcd) {
#else
									if ( Get_CD_Index(current_drive, 1*60) != 2 ){
										RequiredCD = 2;
#endif
	#ifdef WIN32
										WWDebugString ("RA95 - Counterstrike CD is not in drive\n");
	#endif

										/*
										** We should have the scenario but the wrong disk is in.
										** Tell the host that I am ready to go anyway.
										*/
										memset ((void*)&SendPacket, 0, sizeof (SendPacket));
										SendPacket.Command = SERIAL_READY_TO_GO;
										NullModem.Send_Message (&SendPacket, sizeof (SendPacket), 1);

										starttime = TickCount;
										while ( ( NullModem.Num_Send()
											&& ((TickCount - starttime) < PACKET_SENDING_TIMEOUT) )
											|| ((TickCount - starttime) < 60) ) {
											NullModem.Service();
										}
										ready_packet_was_sent = true;

										if (!Force_CD_Available (RequiredCD)){
											Emergency_Exit(EXIT_FAILURE);
										}

										/*
										** Update the internal list of scenarios to include the counterstrike
										** list.
										*/
										Session.Read_Scenario_Descriptions();

										/*
										** Make sure we dont time out because of the disk swap
										*/
										lastmsgtime = TickCount;
									}
								}
							}

							/*
							** If the scenario that the host wants to play doesnt exist locally then we
							**	need to request that it is sent. If we can identify the scenario locally then
							**	we need to fix up the file name so we load the right one.
							*/
							if (Find_Local_Scenario (Session.Options.ScenarioDescription,
															Session.ScenarioFileName,
															Session.ScenarioFileLength,
															Session.ScenarioDigest,
															Session.ScenarioIsOfficial)) {

								/*
								** We have the scenario. Tell the host that I am ready to go.
								*/
								if (!ready_packet_was_sent) {
									memset ((void*)&SendPacket, 0, sizeof (SendPacket));
									SendPacket.Command = SERIAL_READY_TO_GO;
									NullModem.Send_Message (&SendPacket, sizeof (SendPacket), 1);
										starttime = TickCount;

									while ( ( NullModem.Num_Send()
										&& ((TickCount - starttime) < PACKET_SENDING_TIMEOUT) )
										|| ((TickCount - starttime) < 60) ) {
										NullModem.Service();
									}
								}
							} else {
#ifndef FIXIT_VERSION_3			//	Removed restriction on downloading official maps.
							/*
								** Oh dear. Thats a scenario I dont have. Request that the host sends the
								**	scenario to me provided it isnt an official Westwood scenario.
								**
								** If the file is received OK then we will get a true return value and the
								** actual file name to load will be in Session.ScenarioFileName
								*/
								if (Session.ScenarioIsOfficial) {
									/*
									** We dont have the scenario and we dont want to request that it gets
									** sent because its an official one.
									** Print up a message saying we cant play this scenario and reply to the
									** host, telling him to select another.
									*/
									memset ((void*)&SendPacket, 0, sizeof (SendPacket));
									SendPacket.Command = SERIAL_NO_SCENARIO;
									NullModem.Send_Message (&SendPacket, sizeof (SendPacket), 1);
									NullModem.Service();
									WWMessageBox().Process (TXT_UNABLE_PLAY_WAAUGH);
									display = REDRAW_ALL;
									lastmsgtime = TickCount;
									break;
								} else {
#endif
#ifdef FIXIT_VERSION_3
									if( bSpecialAftermathScenario( Session.Options.ScenarioDescription ) )
										break;
#endif
									if (!Get_Scenario_File_From_Host(Session.ScenarioFileName, 0)) {
										rc = false;
										break;
									} else {
										/*
										** Make sure we dont time-out because of the download
										*/
										lastmsgtime = TickCount;
									}
#ifndef FIXIT_VERSION_3
								}
#endif
							}
						}else{
							/*
							** Make sure we respond to the host in a load game
							*/
							memset ((void*)&SendPacket, 0, sizeof (SendPacket));
							SendPacket.Command = SERIAL_READY_TO_GO;
							NullModem.Send_Message (&SendPacket, sizeof (SendPacket), 1);
								starttime = TickCount;

							while ( ( NullModem.Num_Send()
								&& ((TickCount - starttime) < PACKET_SENDING_TIMEOUT) )
								|| ((TickCount - starttime) < 60) ) {
								NullModem.Service();
							}
						}

						/*
						** Fall through here...
						*/
						strcpy (Scen.ScenarioName, Session.ScenarioFileName);
						//
						// calculated one way delay for a packet and overall delay
						// to execute a packet
						//
						if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
							Session.MaxAhead = max( ((((ReceivePacket.ScenarioInfo.ResponseTime / 8) +
								(Session.FrameSendRate - 1)) / Session.FrameSendRate) *
								Session.FrameSendRate), (Session.FrameSendRate * 2) );
						} else {
							Session.MaxAhead = max( (ReceivePacket.ScenarioInfo.ResponseTime / 8),
														  MODEM_MIN_MAX_AHEAD );
						}

						process = false;
						rc = true;
						if (ReceivePacket.Command == SERIAL_LOADGAME)
							load_game = true;
						break;

					/*..................................................................
					Incoming message: add to our list
					..................................................................*/
					case (SERIAL_MESSAGE):
						oppscorescreen = false;

						Session.Messages.Add_Message (ReceivePacket.Name,
							((PlayerColorType)ReceivePacket.ID == PCOLOR_DIALOG_BLUE) ?
														PCOLOR_REALLY_BLUE : (PlayerColorType)ReceivePacket.ID,
							ReceivePacket.Message.Message,
							((PlayerColorType)ReceivePacket.ID == PCOLOR_DIALOG_BLUE) ?
														PCOLOR_REALLY_BLUE : (PlayerColorType)ReceivePacket.ID,
							TPF_TEXT, -1);

						Sound_Effect(VOC_INCOMING_MESSAGE);
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
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
						oppscorescreen = true;
						if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
						parms_received = true;
						break;

					default:
						break;
				}
			}
		}

		// if we haven't received a msg for 10 seconds exit

		if ((TickCount - lastmsgtime) > msg_timeout) {
			WWMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);
			process = false;
			rc = false;

			// say we did receive sign off to keep from sending one
			recsignedoff = true;
		}

		/*---------------------------------------------------------------------
		Service the connection
		---------------------------------------------------------------------*/
		NullModem.Service();
	}

	/*------------------------------------------------------------------------
	Prepare to load the scenario
	------------------------------------------------------------------------*/
	if (rc) {
		Session.NumPlayers = 2;

		/*.....................................................................
		Add both players to the Players vector; the local system is always
		index 0.
		.....................................................................*/
		who = new NodeNameType;

		#ifdef FIXIT_MODEM_LOAD_CRASH
		/* If the names of the players are the same then we MUST force them
		 * be be unique. This is necessary to prevent a crash after loading
		 * a modem save game.
		 */
		if (strcmp(TheirName, namebuf) == 0)
			{
			if (strlen(TheirName) == (MPLAYER_NAME_MAX - 1))
				{
				namebuf[MPLAYER_NAME_MAX - 1] = '\0';
				}
			else
				{
				strcat(namebuf, "2");
				}
			}
		#endif

		strcpy(who->Name, namebuf);
		who->Player.House = Session.House;
		who->Player.Color = Session.ColorIdx;
		who->Player.ProcessTime = -1;
		Session.Players.Add (who);

		who = new NodeNameType;
		strcpy(who->Name, TheirName);
		who->Player.House = TheirHouse;
		who->Player.Color = TheirColor;
		who->Player.ProcessTime = -1;
		Session.Players.Add (who);

		starttime = TickCount;
		while ( ( NullModem.Num_Send()
			&& ((TickCount - starttime) < PACKET_SENDING_TIMEOUT) )
			|| ((TickCount - starttime) < 60) ) {
			#if(SHOW_MONO)
			NullModem.Mono_Debug_Print(0);
			#endif

			NullModem.Service();
		}

		// clear queue to keep from doing any resends
		NullModem.Init_Send_Queue();

	} else {
		if ( !recsignedoff ) {
			/*.....................................................................
			Broadcast my sign-off over my network
			.....................................................................*/
			memset (&SendPacket, 0, sizeof(SerialPacketType));
			SendPacket.Command = SERIAL_SIGN_OFF;
			SendPacket.ScenarioInfo.Color = Session.ColorIdx;		// use Color for ID
			SendPacket.ID = Session.ModemType;
			NullModem.Send_Message (&SendPacket, sizeof(SendPacket), 1);

			starttime = TickCount;
			while ( (NullModem.Num_Send()
				&& ((TickCount - starttime) < PACKET_CANCEL_TIMEOUT) )
				|| ((TickCount - starttime) < 60) ) {
				#if(SHOW_MONO)
				NullModem.Mono_Debug_Print(0);
				#endif

				if ( NullModem.Get_Message( &ReceivePacket, &packetlen ) > 0) {

					// are we getting our own packets back??

					if (ReceivePacket.Command == SERIAL_SIGN_OFF
						&& ReceivePacket.ID == Session.ModemType) {

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
	Clean up the list boxes
	------------------------------------------------------------------------*/
//	while (optionlist.Count()>0) {
//		item = (char *)optionlist.Get_Item(0);
//		delete [] item;
//		optionlist.Remove_Item(item);
//	}
	while (gamelist.Count()>0) {
		item = (char *)gamelist.Get_Item(0);
		delete [] item;
		gamelist.Remove_Item(item);
	}
	while (playerlist.Count()>0) {
		item = (char *)playerlist.Get_Item(0);
		delete [] item;
		playerlist.Remove_Item(item);
	}

	/*------------------------------------------------------------------------
	Remove the chat edit box
	------------------------------------------------------------------------*/
	Session.Messages.Remove_Edit();

	/*------------------------------------------------------------------------
	Restore screen
	------------------------------------------------------------------------*/
	Hide_Mouse();
	Load_Title_Page(true);
//#ifdef WIN32
//	Load_Uncompress(CCFileClass("TITLE.CPS"), SysMemPage, SysMemPage, CCPalette);
//	SysMemPage.Scale(SeenPage);
//#else
//	Load_Uncompress(CCFileClass("TITLE.CPS"), HidPage, HidPage, CCPalette);
//	HidPage.Blit(SeenPage);
//#endif
	Show_Mouse();

	/*------------------------------------------------------------------------
	Save any changes made to our options
	------------------------------------------------------------------------*/
	if (changed) {
		Session.Write_MultiPlayer_Settings ();
	}

	if (load_game) {
		if (!Load_Game (-1)) {
			WWMessageBox().Process (TXT_ERROR_LOADING_GAME);
			rc = false;
		}
		Frame++;
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
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	int d_dialog_w = 280 * RESFACTOR;									// dialog width
	int d_dialog_h = 160 * RESFACTOR;									// dialog height
	int d_dialog_x = ((320 * RESFACTOR - d_dialog_w) / 2);		// dialog x-coord
	int d_dialog_y = ((200 * RESFACTOR - d_dialog_h) / 2);		// dialog y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 7 * RESFACTOR;										// ht of 6-pt text
	int d_margin = 7 * RESFACTOR;										// margin width/height

	int d_phonelist_w = 248 * RESFACTOR;
	int d_phonelist_h = 87 * RESFACTOR;
	int d_phonelist_x = d_dialog_cx - (d_phonelist_w / 2);
	int d_phonelist_y = d_dialog_y + d_margin + d_txt6_h + 11 * RESFACTOR;

	int d_add_w = 45 * RESFACTOR;
	int d_add_h = 9 * RESFACTOR;
	int d_add_x = d_dialog_cx - (d_add_w / 2) - d_margin - d_add_w;
	int d_add_y = d_phonelist_y + d_phonelist_h + d_margin;

	int d_edit_w = 45 * RESFACTOR;
	int d_edit_h = 9 * RESFACTOR;
	int d_edit_x = d_dialog_cx - (d_edit_w / 2);
	int d_edit_y = d_phonelist_y + d_phonelist_h + d_margin;

	int d_delete_w = 45 * RESFACTOR;
	int d_delete_h = 9 * RESFACTOR;
	int d_delete_x = d_dialog_cx + (d_delete_w / 2) + d_margin;
	int d_delete_y = d_phonelist_y + d_phonelist_h + d_margin;

	int d_numedit_w = ( (PhoneEntryClass::PHONE_MAX_NUM - 1) * 6) * RESFACTOR + 3 * RESFACTOR;
	int d_numedit_h = 9 * RESFACTOR;
	int d_numedit_x = d_dialog_cx - (d_numedit_w / 2);
	int d_numedit_y = d_add_y + d_add_h + d_margin;

	int d_dial_w = 45 * RESFACTOR;
	int d_dial_h = 9 * RESFACTOR;
	int d_dial_x = d_dialog_cx - (d_numedit_w / 2) - d_margin - d_dial_w;
	int d_dial_y = d_add_y + d_add_h + d_margin;

	int d_cancel_w = 45 * RESFACTOR;
	int d_cancel_h = 9 * RESFACTOR;
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
	bool process = true;						// process while true
	KeyNumType input;

	char phone_num[ PhoneEntryClass::PHONE_MAX_NUM ] = { 0 }; // buffer for editing phone #
	int rc;
	int i;
	int tabs[] = {123*RESFACTOR, 207*RESFACTOR};				// tabs for list box
	char *item;									// for removing items from list box
	PhoneEntryClass *p_entry;				// for creating / editing phonebook entries
	bool changed = false;					// 1 = save changes to INI file
	bool firsttime = false;

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	ListClass phonelist(BUTTON_PHONELIST, d_phonelist_x, d_phonelist_y, d_phonelist_w, d_phonelist_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
	TextButtonClass addbtn(BUTTON_ADD, TXT_ADD, TPF_BUTTON, d_add_x, d_add_y, d_add_w, d_add_h);
	TextButtonClass editbtn(BUTTON_EDIT, TXT_EDIT, TPF_BUTTON, d_edit_x, d_edit_y, d_edit_w, d_edit_h);
	TextButtonClass deletebtn(BUTTON_DELETE, TXT_DELETE_BUTTON, TPF_BUTTON, d_delete_x, d_delete_y, d_delete_w, d_delete_h);
	TextButtonClass dialbtn(BUTTON_DIAL, TXT_DIAL, TPF_BUTTON, d_dial_x, d_dial_y, d_dial_w, d_dial_h);
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
	EditClass numedit (BUTTON_NUMEDIT, phone_num, PhoneEntryClass::PHONE_MAX_NUM, TPF_TEXT, d_numedit_x, d_numedit_y, d_numedit_w, d_numedit_h, EditClass::ALPHANUMERIC);

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

	if (Session.CurPhoneIdx == -1) {
		firsttime = true;
	}

	/*
	---------------------------- Processing loop -----------------------------
	*/
	while (process) {
		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}
		#endif
		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Load_Title_Page(true);
				CCPalette.Set();

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				// init font variables

				Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_CENTER | TPF_TEXT);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Draw_Caption (TXT_PHONE_LIST, d_dialog_x, d_dialog_y, d_dialog_w);
				phonelist.Draw_Me (true);
			}
			/*
			.......................... Redraw buttons ..........................
			*/
			if (display >= REDRAW_BUTTONS) {
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
			firsttime = false;
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
				#ifdef FIXIT_PHONELIST_CRASH
				if (Session.CurPhoneIdx != -1) {
				#endif
					if (phonelist.Current_Index() != Session.CurPhoneIdx) {
						Session.CurPhoneIdx = phonelist.Current_Index();
						strcpy (phone_num, Session.PhoneBook[Session.CurPhoneIdx]->Number);
						numedit.Set_Text (phone_num, PhoneEntryClass::PHONE_MAX_NUM );
						changed = true;
					}
				#ifdef FIXIT_PHONELIST_CRASH
				}
				#endif
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
					Session.PhoneBook.Add (p_entry);
					Build_Phone_Listbox( &phonelist, &numedit, phone_num );
					/*............................................................
					Set the current listbox index to the newly-added item.
					............................................................*/
					for (i = 0; i < Session.PhoneBook.Count(); i++) {
						if (p_entry == Session.PhoneBook[i]) {
							Session.CurPhoneIdx = i;
							strcpy (phone_num, Session.PhoneBook[Session.CurPhoneIdx]->Number);
							numedit.Set_Text (phone_num, PhoneEntryClass::PHONE_MAX_NUM );
							phonelist.Set_Selected_Index( Session.CurPhoneIdx );
						}
					}
					changed = true;
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
				if (Session.CurPhoneIdx==-1)
					break;

				/*...............................................................
				Allocate a new entry & copy the currently-selected entry into it
				...............................................................*/
				p_entry = new PhoneEntryClass();
				(*p_entry) = (*Session.PhoneBook[Session.CurPhoneIdx]);

				/*...............................................................
				Pass the new entry to the entry editor; if the user selects OK,
				copy the data back into our phone book.  Rebuild the list so
				the changes show up in the list box.
				...............................................................*/
				if ( Edit_Phone_Dialog( p_entry ) ) {
					(*Session.PhoneBook[Session.CurPhoneIdx]) = (*p_entry);
					Build_Phone_Listbox(&phonelist, &numedit, phone_num);
					/*............................................................
					Set the current listbox index to the newly-added item.
					............................................................*/
					for (i = 0; i < Session.PhoneBook.Count(); i++) {
						if (Session.PhoneBook[Session.CurPhoneIdx] == Session.PhoneBook[i]) {
							Session.CurPhoneIdx = i;
							strcpy (phone_num, Session.PhoneBook[Session.CurPhoneIdx]->Number);
							numedit.Set_Text (phone_num, PhoneEntryClass::PHONE_MAX_NUM );
							phonelist.Set_Selected_Index( Session.CurPhoneIdx );
						}
					}
					changed = true;
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
				if (Session.CurPhoneIdx == -1)
					break;

				/*...............................................................
				Delete the current item & rebuild the phone listbox
				...............................................................*/
				Session.PhoneBook.Delete (Session.CurPhoneIdx);
				Build_Phone_Listbox(&phonelist, &numedit, phone_num);

				if (Session.CurPhoneIdx == -1) {
					*phone_num = 0;
					numedit.Set_Text (phone_num, PhoneEntryClass::PHONE_MAX_NUM );
				}
				changed = true;
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
				if (Session.CurPhoneIdx == -1 ||
					strcmp( Session.PhoneBook[Session.CurPhoneIdx]->Number, phone_num) ) {

					if ( strlen(phone_num) == 0) {	// do not dial
						dialbtn.IsPressed = true;
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

					Session.PhoneBook.Add (p_entry);
					Build_Phone_Listbox(&phonelist, &numedit, phone_num);
					/*............................................................
					Set the current listbox index to the newly-added item.
					............................................................*/
					for (i = 0; i < Session.PhoneBook.Count(); i++) {
						if (p_entry == Session.PhoneBook[i]) {
							Session.CurPhoneIdx = i;
						}
					}
					changed = true;
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
		Session.Write_MultiPlayer_Settings ();
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
	qsort ((void *)(&Session.PhoneBook[0]), Session.PhoneBook.Count(), sizeof(class PhoneEntryClass *), Phone_Compare);

	/*........................................................................
	Build the list
	........................................................................*/
	for (i = 0; i < Session.PhoneBook.Count(); i++) {
		item = new char[80];
		if ( !(strlen( Session.PhoneBook[i]->Name )) ) {
			strcpy( phonename, " " );
		} else {
			strncpy( phonename, Session.PhoneBook[i]->Name, 20 );
			phonename[21] = 0;
		}

		if ( !(strlen( Session.PhoneBook[i]->Number )) ) {
			strcpy( phonenum, " " );
		} else {
			if ( strlen( Session.PhoneBook[i]->Number ) < 14) {
				strcpy( phonenum, Session.PhoneBook[i]->Number );
			} else {
				strncpy( phonenum, Session.PhoneBook[i]->Number, 12 );
				phonenum[12] = 0;
				strcat( phonenum, "..." );
			}
		}

		if (Session.PhoneBook[i]->Settings.Baud != -1) {
			sprintf(item,"%s\t%s\t%d", phonename, phonenum,
				Session.PhoneBook[i]->Settings.Baud);
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
	if (list->Count() == 0 || Session.CurPhoneIdx < -1) {
		Session.CurPhoneIdx = -1;
	} else {
		if (Session.CurPhoneIdx >= list->Count() ) {
			Session.CurPhoneIdx = 0;
		}
	}

	/*........................................................................
	Fill in phone number edit buffer
	........................................................................*/
	if (Session.CurPhoneIdx > -1) {
		strcpy (buf, Session.PhoneBook[Session.CurPhoneIdx]->Number);
		edit->Set_Text (buf, PhoneEntryClass::PHONE_MAX_NUM );
		list->Set_Selected_Index( Session.CurPhoneIdx );
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
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	int d_dialog_w = 230 * RESFACTOR;									// dialog width
	int d_dialog_h = 110 * RESFACTOR;									// dialog height
	int d_dialog_x = ((320 * RESFACTOR - d_dialog_w) / 2);		// dialog x-coord
	int d_dialog_y = ((136 * RESFACTOR - d_dialog_h) / 2);		// dialog y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 7 * RESFACTOR;										// ht of 6-pt text
	int d_margin = 7 * RESFACTOR;										// margin width/height

	int d_name_w = ( (PhoneEntryClass::PHONE_MAX_NAME - 1) * 6) * RESFACTOR + 3 * RESFACTOR;
	int d_name_h = 9 * RESFACTOR;
	int d_name_x = d_dialog_x + (((d_dialog_w - d_name_w) * 3) / 4) - 5 * RESFACTOR;
	int d_name_y = d_dialog_y + 25 * RESFACTOR;

	int d_number_w = ( (PhoneEntryClass::PHONE_MAX_NUM - 1) * 6) * RESFACTOR + 3 * RESFACTOR;
	int d_number_h = 9 * RESFACTOR;
	int d_number_x = d_dialog_x + (((d_dialog_w - d_number_w) * 3) / 4) - 5 * RESFACTOR;
	int d_number_y = d_name_y + d_name_h + d_margin;

	int d_default_w = 130 * RESFACTOR;
	int d_default_h = 9 * RESFACTOR;
	int d_default_x = d_dialog_cx - (d_default_w / 2);
	int d_default_y = d_number_y + d_number_h + d_margin;

	int d_custom_w = 130 * RESFACTOR;
	int d_custom_h = 9 * RESFACTOR;
	int d_custom_x = d_dialog_cx - (d_default_w / 2);
	int d_custom_y = d_default_y + d_default_h + d_margin;

	int d_save_w = 55 * RESFACTOR;
	int d_save_h = 9 * RESFACTOR;
	int d_save_x = d_dialog_cx - d_margin - d_save_w;
	int d_save_y = d_dialog_y + d_dialog_h - d_margin - d_save_h - 5*RESFACTOR;

	int d_cancel_w = 55 * RESFACTOR;
	int d_cancel_h = 9 * RESFACTOR;
	int d_cancel_x = d_dialog_cx + d_margin;
	int d_cancel_y = d_dialog_y + d_dialog_h - d_margin - d_cancel_h - 5*RESFACTOR;

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

	char namebuf[PhoneEntryClass::PHONE_MAX_NAME] = { 0 };	// buffer for editing name
	char numbuf[PhoneEntryClass::PHONE_MAX_NUM] = { 0 };		// buffer for editing phone #
	int rc;
	SerialSettingsType settings;
	bool custom = false;
	bool firsttime = true;
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	/*........................................................................
	Buttons
	........................................................................*/
	GadgetClass *commands;										// button list

	EditClass nameedit(BUTTON_NAME, namebuf, PhoneEntryClass::PHONE_MAX_NAME, TPF_TEXT, d_name_x, d_name_y, d_name_w, d_name_h, EditClass::ALPHANUMERIC);
	EditClass numedit(BUTTON_NUMBER, numbuf, PhoneEntryClass::PHONE_MAX_NUM, TPF_TEXT, d_number_x, d_number_y, d_number_w, d_number_h, EditClass::ALPHANUMERIC);
	TextButtonClass defaultbtn(BUTTON_DEFAULT, TXT_DEFAULT_SETTINGS, TPF_BUTTON, d_default_x, d_default_y, d_default_w, d_default_h);
	TextButtonClass custombtn(BUTTON_CUSTOM, TXT_CUSTOM_SETTINGS, TPF_BUTTON, d_custom_x, d_custom_y, d_custom_w, d_custom_h);
	TextButtonClass savebtn(BUTTON_SAVE, TXT_SAVE_BUTTON, TPF_BUTTON, d_save_x, d_save_y, d_save_w, d_save_h);
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);

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
		settings = Session.SerialDefaults;
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
	bool process = true;						// process while true
	while (process) {
		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}
		#endif
		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			/*
			.................. Redraw backgound & dialog box ...................
			*/
			if (display >= REDRAW_BACKGROUND) {
				Load_Title_Page(true);
				CCPalette.Set();

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);
				Draw_Caption (TXT_PHONE_LISTING, d_dialog_x, d_dialog_y, d_dialog_w);

				/*...............................................................
				Dialog & Field labels
				...............................................................*/
				Fancy_Text_Print (TXT_NAME_COLON, d_name_x - 5 * RESFACTOR, d_name_y + 1 * RESFACTOR, scheme, TBLACK, TPF_RIGHT | TPF_TEXT);
				Fancy_Text_Print (TXT_NUMBER_COLON, d_number_x - 5 * RESFACTOR, d_number_y + 1 * RESFACTOR, scheme, TBLACK, TPF_RIGHT | TPF_TEXT);
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
		KeyNumType input = commands->Input();

		if ( firsttime ) {
			nameedit.Set_Focus();
			nameedit.Flag_To_Redraw();
			input = commands->Input();
			firsttime = false;
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
				custom = false;
				break;

			/*------------------------------------------------------------------
			Use Custom Serial Settings
			------------------------------------------------------------------*/
			case (BUTTON_CUSTOM | KN_BUTTON):
				if (Com_Settings_Dialog (&settings)) {
					custombtn.Turn_On();
					defaultbtn.Turn_Off();
				}
				custom = true;
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
	Session.ModemService = false;

	// save for later to reconnect

	DialSettings = settings;

	modemstatus = NullModem.Get_Modem_Status();
	if (reconnect) {
		if ( (modemstatus & CD_SET) ) {
			connected = true;
			Session.ModemService = true;
			return( connected );
		}
	} else if ( (modemstatus & CD_SET) ) {
		NullModem.Hangup_Modem();
		Session.ModemService = false;
	}

	NullModem.Setup_Modem_Echo( Modem_Echo );

	modemstatus = NullModem.Detect_Modem( settings, reconnect );
	if ( !modemstatus ) {
		NullModem.Remove_Modem_Echo();
		NullModem.Print_EchoBuf();
		NullModem.Reset_EchoBuf();
//#ifndef WIN32
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
					WWMessageBox().Process( TXT_UNABLE_FIND_MODEM );
					Session.ModemService = true;
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
					WWMessageBox().Process( TXT_UNABLE_FIND_MODEM );
					Session.ModemService = true;
					return( connected );
				}
				break;

			default:
				WWMessageBox().Process( TXT_UNABLE_FIND_MODEM );
				Session.ModemService = true;
				return( connected );

		}
//#else	//WIN32
//		Session.ModemService = true;
//		return( connected );
//#endif	//WIN32


	} else if ( modemstatus == -1 ) {
		NullModem.Remove_Modem_Echo();
		NullModem.Print_EchoBuf();
		NullModem.Reset_EchoBuf();
		WWMessageBox().Process( TXT_ERROR_IN_INITSTRING );
//		WWMessageBox().Process( "Error in the InitString." );
		Session.ModemService = true;
		return( connected );
	}


#ifdef WIN32
#ifdef FIXIT_APTIVA_MODEM

	/*
	** Completely disable audio. This is required for MWave devices like those
	** found in the IBM Aptiva.
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
#endif	//FIXIT_APTIVA_MODEM
#endif	//WIN32

	dialstatus = NullModem.Dial_Modem( DialString, settings->DialMethod, reconnect );

	if (reconnect) {
		/*
		--------------------------- Redraw the display ---------------------------
		*/
		HidPage.Clear();
		Map.Flag_To_Redraw(true);
		Map.Render();
	}

	switch ( dialstatus ) {
		case DIAL_CONNECTED:
			connected = true;
			break;

		case DIAL_NO_CARRIER:
			WWMessageBox().Process(TXT_NO_CARRIER);
			connected = false;
			break;

		case DIAL_BUSY:
			WWMessageBox().Process(TXT_LINE_BUSY);
			connected = false;
			break;

		case DIAL_ERROR:
			WWMessageBox().Process(TXT_NUMBER_INVALID);
			connected = false;
			break;

		case DIAL_NO_DIAL_TONE:
			WWMessageBox().Process(TXT_NO_DIAL_TONE);
			connected = false;
			break;

		case DIAL_CANCELED:
			NullModem.Hangup_Modem();
			Session.ModemService = false;
			WWMessageBox().Process(TXT_DIALING_CANCELED);
			connected = false;
			break;
	}

	NullModem.Remove_Modem_Echo();
	NullModem.Print_EchoBuf();
	NullModem.Reset_EchoBuf();

#ifdef WIN32
#ifdef FIXIT_APTIVA_MODEM
	/*
	** Restore audio capability
	*/
	SoundOn = Audio_Init ( MainWindow , 16 , false , 11025*2 , 0 );
	if (SoundOn){
		Theme.Play_Song (old_theme);
	}
#endif	//FIXIT_APTIVA_MODEM
#endif	//WIN32

	Session.ModemService = true;
	return( connected );

}	/* end of Dial_Modem */


static bool Answer_Modem( SerialSettingsType *settings, bool reconnect )
{
	bool connected = false;
	DialStatusType dialstatus;
	int modemstatus;

	/*
	**	Turn modem servicing off in the callback routine.
	*/
	Session.ModemService = false;

	// save for later to reconnect

	DialSettings = settings;

	modemstatus = NullModem.Get_Modem_Status();
	if (reconnect) {
		if ( (modemstatus & CD_SET) ) {
			connected = true;
			Session.ModemService = true;
			return( connected );
		}
	} else if ( (modemstatus & CD_SET) ) {
		NullModem.Hangup_Modem();
		Session.ModemService = false;
	}

	NullModem.Setup_Modem_Echo( Modem_Echo );

	modemstatus = NullModem.Detect_Modem( settings, reconnect );
	if ( !modemstatus ) {
		NullModem.Remove_Modem_Echo();
		NullModem.Print_EchoBuf();
		NullModem.Reset_EchoBuf();
//#ifndef WIN32
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
					WWMessageBox().Process( TXT_UNABLE_FIND_MODEM );
					Session.ModemService = true;
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
					WWMessageBox().Process( TXT_UNABLE_FIND_MODEM );
					Session.ModemService = true;
					return( connected );
				}
				break;

			default:
				WWMessageBox().Process( TXT_UNABLE_FIND_MODEM );
				Session.ModemService = true;
				return( connected );

		}
//#else	//WIN32
//		Session.ModemService = true;
//		return( connected );
//#endif	//WIN32
	} else if ( modemstatus == -1 ) {
		NullModem.Remove_Modem_Echo();
		NullModem.Print_EchoBuf();
		NullModem.Reset_EchoBuf();
		WWMessageBox().Process( TXT_ERROR_IN_INITSTRING );
		Session.ModemService = true;
		return( connected );
	}


#ifdef WIN32
#ifdef FIXIT_APTIVA_MODEM
	/*
	** Completely disable audio. This is required for some MWave devices like those
	** found in the IBM Aptiva.
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
#endif	//FIXIT_APTIVA_MODEM
#endif	//WIN32

	dialstatus = NullModem.Answer_Modem( reconnect );

	switch ( dialstatus ) {
		case DIAL_CONNECTED:
			connected = true;
			break;

		case DIAL_NO_CARRIER:
			WWMessageBox().Process(TXT_NO_CARRIER);
			connected = false;
			break;

//		case DIAL_BUSY:
//			WWMessageBox().Process(TXT_LINE_BUSY);
//			connected = false;
//			break;

		case DIAL_ERROR:
			WWMessageBox().Process(TXT_NUMBER_INVALID);
			connected = false;
			break;

		case DIAL_CANCELED:
			WWMessageBox().Process(TXT_ANSWERING_CANCELED);
			connected = false;
			break;
	}

	NullModem.Remove_Modem_Echo();
	NullModem.Print_EchoBuf();
	NullModem.Reset_EchoBuf();

#ifdef WIN32
#ifdef FIXIT_APTIVA_MODEM
	/*
	** Restore audio capability
	*/
	SoundOn = Audio_Init ( MainWindow , 16 , false , 11025*2 , 0 );
	if (SoundOn){
		Theme.Play_Song (old_theme);
	}
#endif	//FIXIT_APTIVA_MODEM
#endif	//WIN32

	Session.ModemService = true;
	return( connected );

}	/* end of Answer_Modem */


static void Modem_Echo( char c )
{
	if (NullModem.EchoCount < (NullModem.EchoSize - 1) ) {
		*(NullModem.EchoBuf + NullModem.EchoCount) = c;
		*(NullModem.EchoBuf + NullModem.EchoCount + 1) = 0;
		NullModem.EchoCount++;
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
		if (MonoClass::Is_Enabled()) {
			Mono_Printf("%s",buf);
		} else {
			printf("%s",buf);
		}
	} else {
		if (Debug_Heap_Dump) {
			printf("%s",buf);
		}
		if (Debug_Modem_Dump) {
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

void Log_Start_Time( char *string )
{
//	LogDump_Print = true;

	LogLevel = 0;
	LogLevelTime[ LogLevel ] = LogLastTime = TickCount;

	Smart_Printf( "start tick=%d, %s \n", LogLastTime, string );
}


void Log_End_Time( char *string )
{
	int i;
	unsigned long currtime;
	unsigned long ticks;


	currtime = TickCount;
	while (LogLevel >= 0) {
		if (LogLevel < MAX_LOG_LEVEL) {
			//
			// put one space for each level as indenting
			//
			i = 0;
			while (i++ < LogLevel) {
				Smart_Printf( " " );
			}
		} else {
			Smart_Printf( "LogLevel %d too large!-! \n", LogLevel);
			LogLevel = MAX_LOG_LEVEL - 1;
		}

		ticks = currtime - LogLevelTime[ LogLevel-- ];
		Smart_Printf( "end tick=%d, ticks=%d, tsecs=%d, %s \n",
			currtime, ticks, ((ticks * 10) / 60), string );
	}

	LogDump_Print = false;
}


void Log_Time( char *string )
{
	int i;
	unsigned long currtime;
	unsigned long ticks;


	currtime = TickCount;

	if (LogLevel < MAX_LOG_LEVEL) {
		//
		// put one space for each level as indenting
		//
		i = 0;
		while (i++ < LogLevel) {
			Smart_Printf( " " );
		}
	} else {
		Smart_Printf( "LogLevel %d too large!-! \n", LogLevel);
		LogLevel = MAX_LOG_LEVEL - 1;
	}

	ticks = currtime - LogLastTime;

	Smart_Printf( "tick=%d, ticks=%d, tsecs=%d, %s \n",
		currtime, ticks, ((ticks * 10) / 60), string );

	LogLastTime = currtime;
}


void Log_Start_Nest_Time( char *string )
{
	int i;
	unsigned long currtime;
	unsigned long ticks;


	currtime = TickCount;

	if (LogLevel < MAX_LOG_LEVEL) {
		//
		// put one space for each level as indenting
		//
		i = 0;
		while (i++ < LogLevel) {
			Smart_Printf( " " );
		}
	} else {
		Smart_Printf( "LogLevel %d too large!-! \n", LogLevel);
		LogLevel = MAX_LOG_LEVEL - 1;
	}

	ticks = currtime - LogLastTime;
	Smart_Printf( "start ntick=%d, ticks=%d, tsecs=%d, %s \n",
		currtime, ticks, ((ticks * 10) / 60), string );

	if (LogLevel >= (MAX_LOG_LEVEL - 1) ) {
		Smart_Printf( "Could not start another nesting Maxed at %d,%d!-! \n",
			LogLevel, (MAX_LOG_LEVEL - 1) );
	} else {
		LogLevelTime[ ++LogLevel ] = currtime;
	}

	LogLastTime = currtime;
}


void Log_End_Nest_Time( char *string )
{
	int i;
	unsigned long currtime;
	unsigned long ticks;


	currtime = TickCount;

	if (LogLevel <= 0) {
		Smart_Printf( "Could not end another nesting Mined at %d,%d!-! \n",
			LogLevel, 0 );
		LogLevel = 0;
	}

	if (LogLevel < MAX_LOG_LEVEL) {
		//
		// put one space for each level as indenting
		//
		i = 0;
		while (i++ < LogLevel) {
			Smart_Printf( " " );
		}
	} else {
		Smart_Printf( "LogLevel %d too large!-! \n", LogLevel);
		LogLevel = MAX_LOG_LEVEL - 1;
	}

	ticks = currtime - LogLevelTime[ LogLevel ];
	Smart_Printf( "end ntick=%d, ticks=%d, secs=%d, %s \n",
		currtime, ticks, ((ticks * 10) / 60), string );

	if (LogLevel) {
		LogLevel--;
	}

	LogLastTime = currtime;
}

#endif