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

/* $Header:   F:\projects\c&c\vcs\code\nullmgr.cpv   1.10   16 Oct 1995 16:51:52   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : NULLMGR.CPP                              *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : April 5, 1995       	 						*
 *                                                                         *
 *                  Last Update : May 1, 1995 [BRR]                        *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:    			                                                   *
 *   NullModemClass::NullModemClass -- class constructor                   *
 *   NullModemClass::~NullModemClass -- class destructor                   *
 *   NullModemClass::Init -- initialization											*
 *   NullModemClass::Send_Message -- sends a message								*
 *   NullModemClass::Get_Message -- polls the Queue for a message				*
 *   NullModemClass::Service -- main polling routine								*
 *   NullModemClass::Num_Send -- Returns # of unACK'd send entries			*
 *   NullModemClass::Num_Receive -- Returns # entries in the receive queue *
 *   NullModemClass::Response_Time -- Returns Queue's avg response time    *
 *   NullModemClass::Reset_Response_Time -- Resets response time computatio*
 *   NullModemClass::Oldest_Send -- Returns ptr to oldest unACK'd send buf *
 *   NullModemClass::Detect_Modem -- Detects and initializes the modem     *
 *   NullModemClass::Dial_Modem -- dials a number passed                   *
 *   NullModemClass::Answer_Modem -- waits for call and answers            *
 *   NullModemClass::Hangup_Modem -- hangs up the modem                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "wincomm.h"
#include "modemreg.h"
//#include "i86.h"
#include "tcpip.h"

//PG_TO_FIX
#if (0)

extern ModemRegistryEntryClass *ModemRegistry;

// the following line was taken from Greenleaf's <ibmkeys.h> <asciidef.h>
// because of other define conflicts

#define ESC		         27
#define NOKEY           0xffff
#define INIT_COMMAND_RETRIES	2

// this time is in milliseconds

#define DEFAULT_TIMEOUT			2000

//
// the following is for a fix around a greenleaf bug
// where they do not check for the value of abortkey
// to determine whether or not they call the abort modem function.
//
extern "C" {
	extern void (*_AbortModemFunctionPtr)(int);
}

static void (*NullModemClass::OrigAbortModemFunc)(int);

static KeyNumType NullModemClass::Input;
static GadgetClass *NullModemClass::Commands;		// button list

/*
** Ugly hack: this string stores the string received from the modem
*/
char ModemRXString[80];

/***************************************************************************
 * NullModemClass::NullModemClass -- class constructor                     *
 *                                                                         *
 * INPUT:                                                                  *
 *		numsend			# desired entries for the send queue						*
 *		numreceive		# desired entries for the receive queue					*
 *		maxlen			application's max packet length								*
 *		magicnum			application-specific magic # (so we don't 				*
 *							accidentally end up talking to another one of our own	*
 *							products using the same protocol)							*
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
NullModemClass::NullModemClass (int numsend, int numreceive, int maxlen,
	unsigned short magicnum) : ConnManClass()
{
	/*------------------------------------------------------------------------
	Init Port to NULL; we haven't opened Greenleaf yet.
	------------------------------------------------------------------------*/
	PortHandle = NULL;
	Connection = NULL;

	NumSend = numsend;
	NumReceive = numreceive;
	MaxLen = maxlen;
	MagicNum = magicnum;

	RXBuf = 0;
	BuildBuf = 0;

	EchoSize = 500;
	EchoBuf = 0;

	OldIRQPri = -1;

	ModemVerboseOn = false;					// default true
	ModemEchoOn = false;						// default true
	ModemWaitCarrier = 50000;				// default 50 * 1000ms = 50 secs
	ModemCarrierDetect = 600;				// default 6  * 100ms  = .6 secs
	ModemCarrierLoss = 1400;				// default 14 * 100ms  = 1.4 secs
	ModemHangupDelay = 20000;				// default 20 * 1000ms = 20 secs
	ModemGuardTime = 1000;					// default 50 * 20ms   = 1 sec
	ModemEscapeCode = '+';					// default ASCII 43

	SendOverflows = 0;
	ReceiveOverflows = 0;
	CRCErrors = 0;

	NumConnections = 0;

	/*........................................................................
	Init timing parameters
	........................................................................*/
	RetryDelta = 60;		// 60 ticks between retries
	MaxRetries = -1;		// disregard # retries
	Timeout = 1200;		// report bad connection after 20 seconds

}	/* end of NullModemClass */


/***************************************************************************
 * NullModemClass::~NullModemClass -- class destructor                     *
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
NullModemClass::~NullModemClass ()
{
	Delete_Connection();
}	/* end of ~NullModemClass */


/***************************************************************************
 * NullModemClass::Init -- initialization												*
 *                                                                         *
 * INPUT:                                                                  *
 *		port				address  															*
 *		irq				2-15     															*
 *		baud				300, 1200, 9600, etc												*
 *		parity			'O' (odd), 'E' (even), 'N' (none), 'S' (space),			*
 *							'M' (mark)															*
 *		wordlength		5, 6, 7, or 8														*
 *		stopbits			1 or 2																*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
//int NullModemClass::Init (int port, int irq, int baud, char parity, int wordlen, int stopbits)
#pragma off (unreferenced)
int NullModemClass::Init (int port, int ,char *dev_name, int baud, char parity, int wordlen, int stopbits, int flowcontrol)
{
	int com;
	//int irqnum;
	//int address;
	//int status;


	if (PortHandle) {
		CloseHandle(PortHandle);
		PortHandle = NULL;
	}

	if (!Connection){
		/*------------------------------------------------------------------------
		Init our Connection
		------------------------------------------------------------------------*/
		Connection = new NullModemConnClass (NumSend, NumReceive, MaxLen,
			MagicNum);

		Connection->Set_Retry_Delta (RetryDelta);
		Connection->Set_Max_Retries (MaxRetries);
		Connection->Set_TimeOut (Timeout);

		/*------------------------------------------------------------------------
		Allocate our packet parsing buffer; make it the same # of packets as the
		# of receive queue entries the application has requested.  Use the
		"Actual" maximum packet size, given from the connection; this allows for
		both headers that get added to the packet.
		------------------------------------------------------------------------*/
		RXSize = Connection->Actual_Max_Packet() * NumReceive;
		RXBuf = new char [RXSize];

		BuildBuf = new char [MaxLen];

		EchoBuf = new char [ EchoSize ];
	}

	RXCount = 0;
	EchoCount = 0;


	/*------------------------------------------------------------------------
	This call allocates all necessary queue buffers
	------------------------------------------------------------------------*/
	switch (port) {
		case 0x3f8:
			com = COM1;
			break;

		case 0x2f8:
			com = COM2;
			break;

		case 0x3e8:
			com = COM3;
			break;

		case 0x2e8:
			com = COM4;
			break;

		default:
			com = COM5;
			break;
	}

	int	i;

	/*
	** Create a new modem class for our com port
	*/
	if (!SerialPort) {
		SerialPort = new WinModemClass;
	}

	/*
	** Shift up the baud rate to sensible values
	*/
//	if (baud == 14400) baud = 19200;
//	if (baud == 28800) baud = 38400;

	static char com_ids[9][5]={
		"COM1",
		"COM2",
		"COM3",
		"COM4",
		"COM5",
		"COM6",
		"COM7",
		"COM8",
		"COM9"
	};

	char *device;

	switch ( port ) {

		case 0x3f8:
			device = com_ids[0];
			break;

		case 0x2f8:
			device = com_ids[1];
			break;

		case 0x3e8:
			device = com_ids[2];
			break;

		case 0x2e8:
			device = com_ids[3];
			break;

		case 1:
			/*
			** 1 is a special value. It means use the device name not the port address.
			*/
			device = dev_name;

			/*
			** If we can match a registry entry with the device name then use that, otherwise use
			** the device name directly to open the port with.
			*/
			if (ModemRegistry){
				delete ModemRegistry;
				ModemRegistry = NULL;
			}
			for ( i=0 ; i<10 ; i++ ){
				ModemRegistry = new ModemRegistryEntryClass (i);
				if (ModemRegistry->Get_Modem_Name()){
					if (!strcmp (dev_name, ModemRegistry->Get_Modem_Name() )){
						device = ModemRegistry->Get_Modem_Device_Name();
						break;
					}
				}
				delete ModemRegistry;
				ModemRegistry = NULL;
			}
			break;

		default:
			device = NULL;
	}

	/*
	** Open the com port
	*/
	PortHandle = SerialPort->Serial_Port_Open (device, baud, parity, wordlen, stopbits, flowcontrol);
	if (PortHandle == INVALID_HANDLE_VALUE) {
		Shutdown();
		return(false);
	}

	/*------------------------------------------------------------------------
	Init the Connection
	------------------------------------------------------------------------*/
	Connection->Init(PortHandle);

	NumConnections = 1;

	return(true);
}


/***********************************************************************************************
 * NMC::Num_Connections -- returns NumConnections member                                       *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   NumConnections                                                                    *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/2/96 11:44AM ST : Documented / Win32 support                                           *
 *=============================================================================================*/
int NullModemClass::Num_Connections( void )
{
	return(NumConnections);
}



/***********************************************************************************************
 * NMC::Delete_Connection -- removes the connection                                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   true                                                                              *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/2/96 11:44AM ST : Documented / Win32 support                                           *
 *=============================================================================================*/
int NullModemClass::Delete_Connection( void )
{
	if (Connection) {
		delete Connection;
		Connection = NULL;
	}

	if (RXBuf) {
		delete [] RXBuf;
		RXBuf = NULL;
	}

	if (BuildBuf) {
		delete [] BuildBuf;
		BuildBuf = NULL;
	}

	if (EchoBuf) {
		delete [] EchoBuf;
		EchoBuf = NULL;
	}

	NumConnections = 0;

	return( true );
}	/* end of Delete_Connection */


/***********************************************************************************************
 * NMC::Init_Send_Queue -- Initialises the connections send queue                              *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   true                                                                              *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/2/96 11:46AM ST : Documented / Win32 support                                           *
 *=============================================================================================*/
int NullModemClass::Init_Send_Queue( void )
{

	/*---------------------------------------------------------------
	Init the send queue
	-----------------------------------------------------------------*/
	if ( Connection ) {
		Connection->Queue->Init_Send_Queue();
	}

	return(true);
}


//DetectPortType NullModemClass::Detect_Port( SerialSettingsType *settings )
/***********************************************************************************************
 * NMC::Detect_Port -- Checks that the specified com port exists                               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to SerialSettingsType                                                         *
 *                                                                                             *
 * OUTPUT:   true if port is valid                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/2/96 11:47AM ST : Documented / Win32 support                                           *
 *=============================================================================================*/
DetectPortType NullModemClass::Detect_Port( SerialSettingsType *settings)
{

	static char com_ids[9][5]={
		"COM1",
		"COM2",
		"COM3",
		"COM4",
		"COM5",
		"COM6",
		"COM7",
		"COM8",
		"COM9"
	};

	int	i;

	/*
	** Create a new modem class for our com port
	*/
	if (!SerialPort) {
		SerialPort = new WinModemClass;
	}else{
		SerialPort->Serial_Port_Close();
	}

	/*
	** Shift up the baud rate to sensible values
	*/
	int baud = settings->Baud;
//	if (baud == 14400) baud = 19200;
//	if (baud == 28800) baud = 38400;


	/*
	** Translate the port address into a usable device name
	*/
	char *device;

	switch ( settings->Port ) {

		case 0x3f8:
			device = com_ids[0];
			break;

		case 0x2f8:
			device = com_ids[1];
			break;

		case 0x3e8:
			device = com_ids[2];
			break;

		case 0x2e8:
			device = com_ids[3];
			break;

		case 1:
			/*
			** 1 is a special value. It means use the device name not the port address.
			*/
			device = settings->ModemName;

			/*
			** If we can match a registry entry with the device name then use that, otherwise use
			** the device name directly to open the port with.
			*/
			if (ModemRegistry){
				delete ModemRegistry;
				ModemRegistry = NULL;
			}
			for ( i=0 ; i<10 ; i++){
				ModemRegistry = new ModemRegistryEntryClass (i);
				if (ModemRegistry->Get_Modem_Name()){
					if (!strcmp (device, ModemRegistry->Get_Modem_Name() )){
						/*
						** Got a match. Break out leaving the registry info intact.
						*/
						device = ModemRegistry->Get_Modem_Device_Name();
						break;
					}
				}
				delete ModemRegistry;
				ModemRegistry = NULL;
			}
			break;

		default:
			return (PORT_INVALID);
	}

	/*
	** Open the com port
	*/
	HANDLE porthandle = SerialPort->Serial_Port_Open (device, baud, 0, 8, 1, settings->HardwareFlowControl);

	if (porthandle == INVALID_HANDLE_VALUE){
		return (PORT_INVALID);
	}

	SerialPort->Serial_Port_Close();
	return (PORT_VALID);

}




/***********************************************************************************************
 * NullModemClass::ShutDown -- Closes serial port and removes the connection                   *
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
 *    8/2/96 11:43AM ST : Documented / Win32 support                                           *
 *=============================================================================================*/
void NullModemClass::Shutdown ( void )
{
	if (PortHandle && SerialPort) {
		SerialPort->Serial_Port_Close();
		delete SerialPort;
		SerialPort = NULL;
		PortHandle = NULL;
		Delete_Connection();
	}

#ifdef FORCE_WINSOCK
	if (Winsock.Get_Connected()){
		Delete_Connection();
	}
#endif

}	/* end of Shutdown */


/***************************************************************************
 * NullModemClass::Set_Timing -- sets timing for all connections				*
 *                                                                         *
 * This will set the timing parameters.  This allows an application to		*
 * measure the Response_Time while running, and adjust timing accordingly.	*
 *                                                                         *
 * INPUT:                                                                  *
 *		retrydelta	value to set for retry delta 										*
 *		maxretries	value to set for max # retries									*
 *		timeout		value to set for connection timeout								*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   08/07/1995 DRD : Created.                                             *
 *=========================================================================*/
void NullModemClass::Set_Timing (unsigned long retrydelta,
	unsigned long maxretries, unsigned long timeout)
{
	RetryDelta = retrydelta;
	MaxRetries = maxretries;
	Timeout = timeout;

	Connection->Set_Retry_Delta (RetryDelta);
	Connection->Set_Max_Retries (MaxRetries);
	Connection->Set_TimeOut (Timeout);

}	/* end of Set_Timing */


/***************************************************************************
 * NullModemClass::Send_Message -- sends a message									*
 *                                                                         *
 * For clarity's sake, here's what happens to the buffer passed in:			*
 * - It gets passed to the Connection's Send_Packet() routine					*
 * - The CommHeaderType header gets tacked onto it									*
 * - The resulting buffer gets added to the Connection's Send Queue			*
 * - When Service() determines that it needs to send the data, it				*
 *   copies the entire packet (CommHeaderType and all) into its local 		*
 *   SendBuf, adds the packet start ID, length, and CRC, then sends it out.*
 *                                                                         *
 * The ack_req argument will almost always be '1' (the default).  The only	*
 * reason to use 0 is if you don't know whether the other system is			*
 * ready or not, so you have to periodically send out a query packet,		*
 * and wait for a response.  (Using the connection's built-in retry			*
 * system would just blast out useless data if the other system isn't		*
 * even there.)																				*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to send															*
 *		buflen		length of buffer														*
 *		ack_req		1 = ACK is required; 0 = not										*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK; 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int NullModemClass::Send_Message (void *buf, int buflen, int ack_req)
{
	int rc;

	if (NumConnections == 0) {
		return( false );
	}

	rc = Connection->Send_Packet(buf,buflen,ack_req);
	if (!rc)
		SendOverflows++;

	return(rc);

}	/* end of Send_Message */


/***************************************************************************
 * NullModemClass::Get_Message -- polls the Queue for a message				*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to store message in											*
 *		buflen		ptr filled in with length of message							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = message was received; 0 = wasn't											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int NullModemClass::Get_Message (void *buf, int *buflen)
{
	if (NumConnections == 0) {
		return( false );
	}
	return( Connection->Get_Packet( buf, buflen ) );
}


/***************************************************************************
 * NullModemClass::Service -- main polling routine									*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = connection has gone bad												*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int NullModemClass::Service (void)
{
	int pos;				// current position in RXBuf
	int i;				// loop counter
	//int status;
	unsigned short length;
	SerialHeaderType *header;	// decoded packet start, length
	SerialCRCType *crc;			// decoded packet CRC
	char moredata = 0;

	if (NumConnections == 0) {
		return( false );
	}

	/*------------------------------------------------------------------------
	First, copy all the bytes we can from the Greenleaf RX buffer to our
	own buffer.
	------------------------------------------------------------------------*/
	RXCount += SerialPort->Read_From_Serial_Port((unsigned char*)(RXBuf + RXCount), int(RXSize - RXCount) );

//	if (RXCount){
//char port[128];
//sprintf (port, "C&C95 - RXCount = %d bytes.\n", RXCount);
//CCDebugString (port);
//	}

BOOL enabled = FALSE;

#if (0)
if (SerialPort->FramingErrors ||
	SerialPort->IOErrors ||
	SerialPort->InBufferOverflows ||
	SerialPort->BufferOverruns ||
	SerialPort->InBufferOverflows ||
	SerialPort->OutBufferOverflows){


if (!MonoClass::Is_Enabled()) {
MonoClass::Enable();
enabled = TRUE;
}
Special.IsMonoEnabled = TRUE;
Debug_Smart_Print = TRUE;
Mono_Set_Cursor(0,0);
Smart_Printf( "            In Queue: %5d  \n", SerialPort->InQueue);
Smart_Printf( "           Out Queue: %5d  \n", SerialPort->OutQueue);
Smart_Printf( "      Framing errors: %5d  \n", SerialPort->FramingErrors);
Smart_Printf( "           IO errors: %5d  \n", SerialPort->IOErrors);
Smart_Printf( "       Parity Errors: %5d  \n", SerialPort->InBufferOverflows);
Smart_Printf( "     Buffer overruns: %5d  \n", SerialPort->BufferOverruns);
Smart_Printf( " In buffer overflows: %5d  \n", SerialPort->InBufferOverflows);
Smart_Printf( "Out buffer overflows: %5d  \n", SerialPort->OutBufferOverflows);

MonoClass::Disable();
Debug_Smart_Print = FALSE;
}
#endif	//(0)

	// minimum packet size

	if ( RXCount < (PACKET_SERIAL_OVERHEAD_SIZE + 1) ) {
		return( Connection->Service() );
	}

	/*------------------------------------------------------------------------
	Now scan the buffer for the start of a packet.
	------------------------------------------------------------------------*/
	pos = -1;
	for (i = 0; i <= RXCount - sizeof( short ); i++) {
		if ( *((unsigned short *)(RXBuf + i)) == PACKET_SERIAL_START ) {
			pos = i;
			break;
		}
	}

	/*------------------------------------------------------------------------
	No start code was found; throw away all bytes except the last few, and
	return.
	------------------------------------------------------------------------*/
	if (pos==-1) {
// Smart_Printf( "No magic number found \n" );
		/*.....................................................................
		move the remaining, un-checked bytes to the start of the buffer
		.....................................................................*/
		memmove (RXBuf, RXBuf + i, sizeof( short ) - 1);
		RXCount = sizeof( short ) - 1;
		return( Connection->Service() );
	}

	/*........................................................................
	Check to see if there are enough bytes for the header to be decoded
	........................................................................*/
	if ( (RXCount - pos) < sizeof( SerialHeaderType ) ) {
		memmove (RXBuf, RXBuf + pos, RXCount - pos);
		RXCount -= pos;
		return(Connection->Service());
	}

	/*------------------------------------------------------------------------
	A start code was found; check the packet's length & CRC
	------------------------------------------------------------------------*/
	header = (SerialHeaderType *)(RXBuf + pos);

	/*........................................................................
	If we lost a byte in the length, we may end up waiting a very long time
	for the buffer to get to the right length; check the verify value to
	make sure this didn't happen.
	........................................................................*/
	if ( header->MagicNumber2 != PACKET_SERIAL_VERIFY ) {
// Smart_Printf( "Verify failed\n");
//		Hex_Dump_Data( (RXBuf + pos), PACKET_SERIAL_OVERHEAD_SIZE );

		pos += sizeof ( short );		// throw away the bogus start code
		memmove (RXBuf, RXBuf + pos, RXCount - pos);
		RXCount -= pos;
		return(Connection->Service());
	}

	length = header->Length;

	/*........................................................................
	Special case: if the length comes out too long for us to process:
	- Assume the packet is bad
	- Throw away the bogus packet-start code
	- Return;  we'll search for another packet-start code next time.
	........................................................................*/
	if (length > MaxLen) {
#if (CONN_DEBUG)
		printf( "length too lonnng\n" );
#endif
// Smart_Printf( "length too lonnng %d, max %d \n", length, MaxLen );

		pos += sizeof ( short );		// throw away the bogus start code
		memmove (RXBuf, RXBuf + pos, RXCount - pos);
		RXCount -= pos;
		return(Connection->Service());
	}

	/*........................................................................
	If the entire packet isn't stored in our buffer, copy the remaining bytes
	to the front of the buffer & return.
	........................................................................*/
	if ( (pos + length + PACKET_SERIAL_OVERHEAD_SIZE) > RXCount) {

		if ( moredata ) {
// Smart_Printf( "waiting for more data %d, pos = %d \n", ((length + PACKET_SERIAL_OVERHEAD_SIZE) - (RXCount - pos)), pos );
		}

		if (pos) {
			memmove (RXBuf, RXBuf + pos, RXCount - pos);
			RXCount -= pos;
		}
		return(Connection->Service());
	}

	/*........................................................................
	Now grab the CRC value in the packet, & compare it to the CRC value
	computed from the actual data.  If they don't match, throw away the bogus
	start-code, move the rest to the front of the buffer, & return.
	We'll continue parsing this data when we're called next time.
	........................................................................*/
	crc = (SerialCRCType *)(RXBuf + pos + sizeof( SerialHeaderType ) + length);
	if (NullModemConnClass::Compute_CRC(RXBuf + pos +
		sizeof( SerialHeaderType ), length) != crc->SerialCRC) {

		CRCErrors++;

#if (CONN_DEBUG)
		printf( "CRC check failed\n" );
#endif
// Smart_Printf( "CRC check failed for packet of length %d \n", length );

//		if (length < 100) {
//			Hex_Dump_Data( (RXBuf + pos), (PACKET_SERIAL_OVERHEAD_SIZE + length) );
//		}

		pos += sizeof ( short );		// throw away the bogus start code
		memmove (RXBuf, RXBuf + pos, RXCount - pos);
		RXCount -= pos;
		return(Connection->Service());
	}

	/*------------------------------------------------------------------------
	Give the new packet to the Connection to process.
	------------------------------------------------------------------------*/
	if (!Connection->Receive_Packet(RXBuf + pos + sizeof( SerialHeaderType ), length)) {
		ReceiveOverflows++;
// Smart_Printf( "Received overflows %d \n", ReceiveOverflows );
	}

#if (0)
	Hex_Dump_Data( (RXBuf + pos), (PACKET_SERIAL_OVERHEAD_SIZE + length) );
#endif

	/*------------------------------------------------------------------------
	Move all data past this packet to the front of the buffer.
	------------------------------------------------------------------------*/
	pos += (PACKET_SERIAL_OVERHEAD_SIZE + length);
	memmove (RXBuf, RXBuf + pos, RXCount - pos);
	RXCount -= pos;

	/*------------------------------------------------------------------------
	Now, service the connection's Queue's; this will handle ACK & Retries.
	------------------------------------------------------------------------*/
	return(Connection->Service());

}	/* end of Service */


/***************************************************************************
 * NullModemClass::Num_Send -- Returns # of unACK'd send entries				*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/01/1995 BRR : Created.                                             *
 *=========================================================================*/
int NullModemClass::Num_Send(void)
{
	if (Connection)
		return( Connection->Queue->Num_Send() );
	else
		return (0);

}	/* end of Num_Send */


/***************************************************************************
 * NullModemClass::Num_Receive -- Returns # entries in the receive queue   *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/01/1995 BRR : Created.                                             *
 *=========================================================================*/
int NullModemClass::Num_Receive(void)
{
	if (Connection)
		return( Connection->Queue->Num_Receive() );
	else
		return (0);

}	/* end of Num_Receive */


/***************************************************************************
 * NullModemClass::Response_Time -- Returns Queue's avg response time      *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/01/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned long NullModemClass::Response_Time(void)
{
	if (Connection)
		return( Connection->Queue->Avg_Response_Time() );
	else
		return (0);

}	/* end of Response_Time */


/***************************************************************************
 * NullModemClass::Reset_Response_Time -- Resets response time computation *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/01/1995 BRR : Created.                                             *
 *=========================================================================*/
void NullModemClass::Reset_Response_Time(void)
{
	if (Connection)
		Connection->Queue->Reset_Response_Time();

}	/* end of Reset_Response_Time */


/***************************************************************************
 * Oldest_Send -- Returns ptr to oldest unACK'd send buffer                *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/01/1995 BRR : Created.                                             *
 *=========================================================================*/
void * NullModemClass::Oldest_Send(void)
{
	int i;
	SendQueueType *send_entry;				// ptr to send entry header
	CommHeaderType *packet;
	void *buf = NULL;

	for (i = 0; i < Connection->Queue->Num_Send(); i++) {
		send_entry = Connection->Queue->Get_Send(i);
		if (send_entry) {
			packet = (CommHeaderType *)send_entry->Buffer;
			if (packet->Code == ConnectionClass::PACKET_DATA_ACK && send_entry->IsACK == 0) {
				buf = send_entry->Buffer;
				break;
			}
		}
	}

	return(buf);

}	/* end of Oldest_Send */


/***************************************************************************
 * NullModemClass::Configure_Debug -- sets up special debug values         *
 *                                                                         *
 * Mono_Debug_Print2() can look into a packet to pull out a particular		*
 * ID, and can print both that ID and a string corresponding to				*
 * that ID.  This routine configures these values so it can find				*
 * and decode the ID.  This ID is used in addition to the normal				*
 * CommHeaderType values.																	*
 *                                                                         *
 * INPUT:                                                                  *
 *		index			connection index to configure (-1 = Global Channel)		*
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
void NullModemClass::Configure_Debug(int , int offset, int size,
	char **names, int maxnames)
{
	if (Connection)
		Connection->Queue->Configure_Debug (offset, size, names, maxnames);
}


/***************************************************************************
 * Mono_Debug_Print -- Debug output routine                                *
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
void NullModemClass::Mono_Debug_Print(int, int refresh)
{
	if (!Connection)
		return;

	Connection->Queue->Mono_Debug_Print (refresh);

	if (refresh) {
		Mono_Set_Cursor (31,1);
		Mono_Printf ("Serial Port Queues");

		Mono_Set_Cursor (9,2);
		Mono_Printf ("Average Response Time:");

		Mono_Set_Cursor (20,3);
		Mono_Printf ("CRC Errors:");

		Mono_Set_Cursor (43,2);
		Mono_Printf ("Send Overflows:");

		Mono_Set_Cursor (40,3);
		Mono_Printf ("Receive Overflows:");
	}

	Mono_Set_Cursor (32,2);
	Mono_Printf ("%d  ", Connection->Queue->Avg_Response_Time());

	Mono_Set_Cursor (32,3);
	Mono_Printf ("%d  ", CRCErrors);

	Mono_Set_Cursor (59,2);
	Mono_Printf ("%d  ", SendOverflows);

	Mono_Set_Cursor (59,3);
	Mono_Printf ("%d  ", ReceiveOverflows);

	Mono_Set_Cursor (2,5);
	Mono_Printf ("%d  ", Num_Send());

	Mono_Set_Cursor (41,5);
	Mono_Printf ("%d  ", Num_Receive());

}	/* end of Mono_Debug_Print */


void Timer_Test (int line, char *file)
{

	char abuffer [128];

	sprintf (abuffer, "Testing timer at line %d in file %s", line, file);
	CCDebugString (abuffer);

	CountDownTimerClass timer;

	timer.Set (1);

	while (timer.Time()){
		CCDebugString (".");
	}

	CCDebugString ("OK\n");
}




/***************************************************************************
 * NullModemClass::Detect_Modem -- Detects and initializes the modem       *
 *                                                                         *
 * INPUT:                                                                  *
 *		settings		ptr to SerialSettings structure     							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		-1				init string invalid													*
 *		0				no modem found															*
 *		1				modem found			  													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/02/1995 DRD : Created.                                             *
 *=========================================================================*/
int NullModemClass::Detect_Modem( SerialSettingsType *settings, bool reconnect )
{
	/*........................................................................
	Button Enumerations
	........................................................................*/
//	enum {
//		BUTTON_CANCEL = 100,
//	};

	int status;
//	int modemstatus;
	int value;
	int error_count = 0;

	int x,y,width,height;					// dialog dimensions
	char buffer[80*3];

	int	factor = SeenBuff.Get_Width()/320;



//Timer_Test(__LINE__, __FILE__);


	/*
	**	Determine the dimensions of the text to be used for the dialog box.
	**	These dimensions will control how the dialog box looks.
	*/
	strcpy( buffer, Text_String( TXT_INITIALIZING_MODEM ) );

	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String(buffer, SeenBuff.Get_Height(), width, height);

	width = MAX(width, 50*factor);
	width += 40*factor;
	height += 60*factor;

	x = (SeenBuff.Get_Width() - width) / 2;
	y = (SeenBuff.Get_Height() - height) / 2;

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	/*
	............................ Draw the dialog .............................
	*/
	Hide_Mouse();
	if ( !reconnect ) {
		Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
		Blit_Hid_Page_To_Seen_Buff();
	}

	Dialog_Box(x, y, width, height);
	Draw_Caption(TXT_NONE, x, y, width);

	Fancy_Text_Print(buffer, x + 20*factor, y + 25*factor, CC_GREEN, TBLACK,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

	Show_Mouse();

	HMWaitForOK( 0, NULL );




	/*
	** OK, lets not mess about any more. Just turn on echo, verbose, and result codes
	** before we even begin. At least this way when we get an error later on we have already
	** removed all the steps we use to try and recover.
	** The timeouts need to be quite small in case the modem is turned off.
	*/

	/*
	** Turn on result codes.
	*/
	Send_Modem_Command ( "ATQ0", '\r', buffer, 81, DEFAULT_TIMEOUT / 2, 2);

	/*
	** Make result codes verbose.
	*/
	Send_Modem_Command ( "ATV1", '\r', buffer, 81, DEFAULT_TIMEOUT / 2, 2);

	/*
	** Turn on echo.
	*/
	Send_Modem_Command ( "ATE1", '\r', buffer, 81, DEFAULT_TIMEOUT / 2, 2);

	ModemVerboseOn = true;
	ModemEchoOn = true;



	/*
	** Try sending a plain old AT command to the modem. Now that we have theoretically
	** turned on verbose result codes we should get an 'OK' back.
	**
	*/
	status = Send_Modem_Command( "AT", '\r', buffer, 81, DEFAULT_TIMEOUT, 2 );

	if (status < ASSUCCESS) {
		/*
		** No 'OK' came back so return failure. We cant print an error message here because
		** the calling function may want to upshift the baud rate (eg from 14400 to 19200) and
		** try again.
		*/
		return (false);
	}

	/*
	** Send the user supplied modem init string
	*/
	if (settings->InitStringIndex == -1) {
		status = Send_Modem_Command( "", '\r', buffer, 81, 300, 1 );
	} else {
		/*
		** Split up the init string into seperate strings if it contains one or more '|' characters.
		** This character acts as a carriage return/pause.
		*/
		char *istr = new char [2 + strlen( InitStrings [settings->InitStringIndex] )];
		char *tokenptr;
		strcpy (istr, InitStrings [settings->InitStringIndex] );

		/*
		** Tokenise the string and send it in chunks
		*/
		tokenptr = strtok ( istr, "|" );
		while ( tokenptr ) {

			status = Send_Modem_Command( tokenptr, '\r', buffer, 81, DEFAULT_TIMEOUT, 1 );
			/*
			** Handle error case.
			*/
			if (status < ASSUCCESS) {
				if (CCMessageBox().Process(TXT_ERROR_NO_INIT, TXT_IGNORE, TXT_CANCEL)) {
					delete istr;
					return( false );
				}
				error_count++;
				break;
			}

			tokenptr = strtok ( NULL, "|");

		}
	}
	/*
	** Use the settings from the registry to further initialise the modem
	*/
	if (settings->Port == 1 && ModemRegistry) {
		/*
		** Send the init strings from the registry if available
		*/
		char send_string[256] = {"AT"};

		if (settings->HardwareFlowControl){
			/*
			** Send the init string for hardware flow control
			*/
			if (ModemRegistry->Get_Modem_Hardware_Flow_Control()) {
				strcpy (&send_string[2], ModemRegistry->Get_Modem_Hardware_Flow_Control());
				status = Send_Modem_Command (send_string, '\r', buffer, 81, DEFAULT_TIMEOUT, 1);
				if (status != MODEM_CMD_OK && status != MODEM_CMD_0) {
					if (CCMessageBox().Process(TXT_NO_FLOW_CONTROL_RESPONSE, TXT_IGNORE, TXT_CANCEL)) return (false);
				}
			}
		}else{
			/*
			** Send the init string for no flow control
			*/
			if (ModemRegistry->Get_Modem_No_Flow_Control()) {
				strcpy (&send_string[2], ModemRegistry->Get_Modem_No_Flow_Control());
				status = Send_Modem_Command (send_string, '\r', buffer, 81, DEFAULT_TIMEOUT, 1);
				if (status != MODEM_CMD_OK && status != MODEM_CMD_0) {
					if (CCMessageBox().Process(TXT_NO_FLOW_CONTROL_RESPONSE, TXT_IGNORE, TXT_CANCEL)) return (false);
				}
			}
		}


		/*
		** Send the string for data compresseion
		*/
		if (settings->Compression){

			if (ModemRegistry->Get_Modem_Compression_Enable()) {
				strcpy (&send_string[2], ModemRegistry->Get_Modem_Compression_Enable());
				Send_Modem_Command (send_string, '\r', buffer, 81, DEFAULT_TIMEOUT, 1);
				if (status != MODEM_CMD_OK && status != MODEM_CMD_0) {
					if (CCMessageBox().Process(TXT_NO_COMPRESSION_RESPONSE, TXT_IGNORE, TXT_CANCEL)) return (false);
				}
			}
		}else{

			if (ModemRegistry->Get_Modem_Compression_Disable()) {
				strcpy (&send_string[2], ModemRegistry->Get_Modem_Compression_Disable());
				Send_Modem_Command (send_string, '\r', buffer, 81, DEFAULT_TIMEOUT, 1);
				if (status != MODEM_CMD_OK && status != MODEM_CMD_0) {
					if (CCMessageBox().Process(TXT_NO_COMPRESSION_RESPONSE, TXT_IGNORE, TXT_CANCEL)) return (false);
				}
			}
		}


		/*
		** Send the string for error correction
		*/
		if (settings->ErrorCorrection){

			if (ModemRegistry->Get_Modem_Error_Correction_Enable()) {
				strcpy (&send_string[2], ModemRegistry->Get_Modem_Error_Correction_Enable());
				Send_Modem_Command (send_string, '\r', buffer, 81, DEFAULT_TIMEOUT, 1);
				if (status != MODEM_CMD_OK && status != MODEM_CMD_0) {
					if (CCMessageBox().Process(TXT_NO_ERROR_CORRECTION_RESPONSE, TXT_IGNORE, TXT_CANCEL)) return (false);
				}
			}
		}else{
			if (ModemRegistry->Get_Modem_Error_Correction_Disable()) {
				strcpy (&send_string[2], ModemRegistry->Get_Modem_Error_Correction_Disable());
				Send_Modem_Command (send_string, '\r', buffer, 81, DEFAULT_TIMEOUT, 1);
				if (status != MODEM_CMD_OK && status != MODEM_CMD_0) {
					if (CCMessageBox().Process(TXT_NO_ERROR_CORRECTION_RESPONSE, TXT_IGNORE, TXT_CANCEL)) return (false);
				}
			}
		}


	}
	/*
	** We require that auto-answer be disabled so turn it off now.
	*/
	status = Send_Modem_Command( "ATS0=0", '\r', buffer, 81, DEFAULT_TIMEOUT, INIT_COMMAND_RETRIES );
	if (status != MODEM_CMD_OK) {
		if (CCMessageBox().Process(TXT_ERROR_NO_DISABLE, TXT_IGNORE, TXT_CANCEL)) return( false );
		error_count++;
	}

	/*
	** If we had an unreasonable number of ignored errors then return failure
	*/
	if (error_count >= 3) {
		CCMessageBox().Process(TXT_ERROR_TOO_MANY, TXT_OK);
		return (false);
	}

	return( true );
}


/***************************************************************************
 * NullModemClass::Dial_Modem -- dials a number passed                     *
 *                                                                         *
 * INPUT:                                                                  *
 *		settings		ptr to SerialSettings structure     							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		status		DialStatus																*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/02/1995 DRD : Created.                                             *
 *=========================================================================*/
DialStatusType NullModemClass::Dial_Modem( char *string, DialMethodType method, bool reconnect )
{

//Timer_Test(__LINE__, __FILE__);


	int factor = SeenBuff.Get_Width()/320;

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

	//int status;
	int delay;
	DialStatusType dialstatus;

	int x,y,width,height;					// dialog dimensions
	char buffer[80*3];

	Input = 0;


//Timer_Test(__LINE__, __FILE__);


	/*
	**	Determine the dimensions of the text to be used for the dialog box.
	**	These dimensions will control how the dialog box looks.
	*/
	if (reconnect) {
		strcpy( buffer, Text_String( TXT_MODEM_CONNERR_REDIALING ) );
	} else {
		strcpy( buffer, Text_String( TXT_DIALING ) );
	}


//Timer_Test(__LINE__, __FILE__);

	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String(buffer, SeenBuff.Get_Height(), width, height);

	int text_width = width;
	width = MAX(width, 50*factor);
	width += 40*factor;
	height += 60*factor;

	x = (SeenBuff.Get_Width() - width) / 2;
	y = (SeenBuff.Get_Height() - height) / 2;

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		x + ((width - (String_Pixel_Width( Text_String( TXT_CANCEL ) ) + 8*factor)) >> 1),
		y + height - (FontHeight + FontYSpacing + 2*factor) - 5*factor);


//Timer_Test(__LINE__, __FILE__);


	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	/*
	............................ Create the list .............................
	*/
	Commands = &cancelbtn;


//Timer_Test(__LINE__, __FILE__);

	Commands->Flag_List_To_Redraw();


//Timer_Test(__LINE__, __FILE__);


	/*
	............................ Draw the dialog .............................
	*/
	Hide_Mouse();
	if ( !reconnect ) {
		Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
		Blit_Hid_Page_To_Seen_Buff();
	}


//Timer_Test(__LINE__, __FILE__);



	Dialog_Box(x, y, width, height);
	Draw_Caption(TXT_NONE, x, y, width);


//Timer_Test(__LINE__, __FILE__);


	Fancy_Text_Print(buffer, SeenBuff.Get_Width()/2 - text_width/2, y + 25*factor, CC_GREEN, TBLACK,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);


//Timer_Test(__LINE__, __FILE__);


	CCDebugString ("C&C95 - About to draw buttons.\n");
	Commands->Draw_All();
	CCDebugString ("C&C95 - About to show mouse.\n");
	Show_Mouse();


//Timer_Test(__LINE__, __FILE__);

	// start waiting

	//CCDebugString ("C&C95 - About to delay for 2 seconds.\n");
	//Delay(120);
	//HMSetDialingMethod( Port, (int)method );
	CCDebugString ("C&C95 - About to set modem dial type.\n");
	SerialPort->Set_Modem_Dial_Type((WinCommDialMethodType) method);


//Timer_Test(__LINE__, __FILE__);

	//Clear out any old modem result codes
	CCDebugString ("C&C95 - About to call 'Get_Modem_Result'.\n");
	SerialPort->Get_Modem_Result(60, buffer, 81);
	//status = HMDial( Port, string );
	CCDebugString ("C&C95 - About to call 'SerialPort->Dial_Modem'.\n");
	SerialPort->Dial_Modem(string);


//Timer_Test(__LINE__, __FILE__);


	//
	// Sets up the ability to abort modem commands when any input is in the
	// Keyboard buffer.  This also calls the game CallBack().
	//
	CCDebugString ("C&C95 - About to call 'Setup_Abort_Modem'.\n");
	Setup_Abort_Modem();


//Timer_Test(__LINE__, __FILE__);

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	process = true;
	delay = ModemWaitCarrier;
	CCDebugString ("C&C95 - About to enter main process loop.\n");
	while (process) {


//Timer_Test(__LINE__, __FILE__);


		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			CCDebugString ("C&C95 - About to restore video surfaces.\n");
			AllSurfaces.SurfacesRestored=FALSE;
			Commands->Draw_All();
		}

//Timer_Test(__LINE__, __FILE__);


		//delay = HMInputLine( Port, delay, buffer, 81 );
		CCDebugString ("C&C95 - About to call 'Get_Modem_Result 2'.\n");


//Timer_Test(__LINE__, __FILE__);


		char abuffer [128];
		sprintf (abuffer, "C&C95 - ModemWaitCarrier delay = %d\n", delay);
		CCDebugString (abuffer);
		delay = SerialPort->Get_Modem_Result(delay, buffer, 81);


		/*
		............................ Process input ............................
		*/
		CCDebugString ("C&C95 - About to check for keyboard input.\n");
		if (!Input) Input = Commands->Input();


		switch (Input) {
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				dialstatus = DIAL_CANCELED;
				process = false;
				break;

			default:
				break;
		}

		if (process) {
			if ( strncmp( buffer, "CON", 3 ) == 0 ) {
				memset (ModemRXString, 0, 80);
				strncpy (ModemRXString, buffer, 79);
				dialstatus = DIAL_CONNECTED;
				process = false;
			}
			else if ( strncmp( buffer, "BUSY", 4 ) == 0 ) {
				dialstatus = DIAL_BUSY;
				process = false;
			}
			else if ( strncmp( buffer, "NO C", 4 ) == 0 ) {
				dialstatus = DIAL_NO_CARRIER;
				process = false;
			}
			else if ( strncmp( buffer, "NO D", 4 ) == 0 ) {
				dialstatus = DIAL_NO_DIAL_TONE;
				process = false;
			}
			else if ( strncmp( buffer, "ERRO", 4 ) == 0 ) {
				dialstatus = DIAL_ERROR;
				process = false;
			}
		}

		if (delay <= 0) {
			if (delay < 0) {
			}
			process = false;
		}
	}

	Remove_Abort_Modem();

	return( dialstatus );

}	/* end of Dial_Modem */


/***************************************************************************
 * NullModemClass::Answer_Modem -- waits for call and answers              *
 *                                                                         *
 * INPUT:                                                                  *
 *		reconnect	whether this is to reconnect		     							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		status		DialStatus																*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/02/1995 DRD : Created.                                             *
 *=========================================================================*/
DialStatusType NullModemClass::Answer_Modem( bool reconnect )
{
	int factor 		= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_CANCEL = 100,
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

	//int status;
	int delay;
	DialStatusType dialstatus;
	bool ring = false;

	int x,y,width,height;					// dialog dimensions
	char text_buffer[80*3];
	char comm_buffer[80*3];

	int text_width;

	/*
	**	Determine the dimensions of the text to be used for the dialog box.
	**	These dimensions will control how the dialog box looks.
	*/
	if (reconnect) {
		strcpy( text_buffer, Text_String( TXT_MODEM_CONNERR_WAITING ) );
	} else {
		strcpy( text_buffer, Text_String( TXT_WAITING_FOR_CALL ) );
	}

	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String(text_buffer, SeenBuff.Get_Height(), width, height);

	text_width = width;
	width = MAX(width, 50*factor);
	width += 40*factor;
	height += 60*factor;

	x = (SeenBuff.Get_Width() - width) / 2;
	y = (SeenBuff.Get_Height() - height) / 2;

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		x + ((width - (String_Pixel_Width( Text_String( TXT_CANCEL ) ) + 8*factor)) >> 1),
		y + height - (FontHeight + FontYSpacing + 2*factor) - 5*factor);

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	//Load_Picture("TITLE.CPS", HidPage, HidPage, Palette, BM_DEFAULT);

	Input = 0;

	/*
	............................ Create the list .............................
	*/
	Commands = &cancelbtn;

	Commands->Flag_List_To_Redraw();

	// start waiting


//	HMWaitForOK( 1000, NULL );
//	status = HMSendString( Port, "ATS0=1" );


	//
	// Sets up the ability to abort modem commands when any input is in the
	// Keyboard buffer.  This also calls the game CallBack() and Input().
	//
	Setup_Abort_Modem();

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	process = true;
	delay = 60000;
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
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				/*
				..................... Refresh the backdrop ......................
				*/
				if ( !reconnect ) {
					Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
					Blit_Hid_Page_To_Seen_Buff();
				}
				/*
				..................... Draw the background .......................
				*/
				Dialog_Box(x, y, width, height);
				/*
				....................... Draw the labels .........................
				*/
				Draw_Caption(TXT_NONE, x, y, width);

				Fancy_Text_Print(text_buffer, SeenBuff.Get_Width()/2 - text_width/2, y + 25*factor, CC_GREEN, TBLACK,
					TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Commands->Draw_All();
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		//delay = HMInputLine( Port, delay, buffer, 81 );
		delay = SerialPort->Get_Modem_Result(delay, comm_buffer, 81);

		/*
		............................ Process input ............................
		*/
		if (!Input) Input = Commands->Input();
		switch (Input) {
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
//				Sound_Effect(VOC_BUTTON,255);
				dialstatus = DIAL_CANCELED;
				process = false;
				break;

			default:
				break;
		}

		if (process) {
			if ( strncmp( comm_buffer, "RING", 4 ) == 0 ) {

				strcpy( text_buffer, Text_String( TXT_ANSWERING ) );

				Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
				Format_Window_String(text_buffer, SeenBuff.Get_Height(), width, height);

				text_width = width;
				width = MAX(width, 50*factor);
				width += 40*factor;
				height += 60*factor;

				x = (SeenBuff.Get_Width() - width) / 2;
				y = (SeenBuff.Get_Height() - height) / 2;

				//PortKillTime( Port, 100 );

				//HMWaitForOK( 0, NULL );
				//status = HMAnswer( Port );
				SerialPort->Write_To_Serial_Port ((unsigned char*)"ATA\r", strlen("ATA\r"));

				ring = true;
				delay = ModemWaitCarrier;
				display = REDRAW_ALL;
			}
			else if ( strncmp( comm_buffer, "CON", 3 ) == 0 ) {
				memset (ModemRXString, 0, 80);
				strncpy (ModemRXString, comm_buffer, 79);
				dialstatus = DIAL_CONNECTED;
				process = false;
			}
			else if ( strncmp( comm_buffer, "BUSY", 4 ) == 0 ) {
				dialstatus = DIAL_BUSY;
				process = false;
			}
			else if ( strncmp( comm_buffer, "NO C", 4 ) == 0 ) {
				dialstatus = DIAL_NO_CARRIER;
				process = false;
			}
			else if ( strncmp( comm_buffer, "NO D", 4 ) == 0 ) {
				dialstatus = DIAL_NO_DIAL_TONE;
				process = false;
			}
			else if ( strncmp( comm_buffer, "ERRO", 4 ) == 0 ) {
				dialstatus = DIAL_ERROR;
				CCMessageBox().Process ("Error - Modem returned error status.", TXT_OK);
				process = false;
			}
		}

		if (delay <= 0) {
			if (ring) {
				if (SerialPort->Get_Modem_Status() & CD_SET){
					sprintf(ModemRXString, "%s", "Connected");
					dialstatus = DIAL_CONNECTED;
				}else{
					dialstatus = DIAL_ERROR;
					CCMessageBox().Process ("Error - TIme out waiting for connect.", TXT_OK);
				}
				process = false;
			} else {
				delay = 60000;
			}
		}
	}

	Remove_Abort_Modem();

	return( dialstatus );

}	/* end of Answer_Modem */


/***************************************************************************
 * NullModemClass::Hangup_Modem -- hangs up the modem                      *
 *                                                                         *
 * INPUT:                                                                  *
 *		none																					  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *		status		successful or not													  *
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																					  *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/02/1995 DRD : Created.                                             *
 *=========================================================================*/
bool NullModemClass::Hangup_Modem( void )
{
	int status;
	int delay;
	char buffer[81];
	char escape[4];

	/*
	**	Turn modem servicing off in the callback routine.
	*/
	ModemService = false;


	status = Send_Modem_Command( "AT", '\r', buffer, 81, DEFAULT_TIMEOUT, 1 );

	if (status == MODEM_CMD_OK) {
		ModemService = true;
		return( true );
	}

	SerialPort->Set_Serial_DTR(FALSE);
	Delay(3200/60);
	SerialPort->Set_Serial_DTR(TRUE);

	//SetDtr( Port, 0 );
	//PortKillTime( Port, 3200 );
	//SetDtr( Port, 1 );

	status = Send_Modem_Command( "AT", '\r', buffer, 81, DEFAULT_TIMEOUT, 1 );

	if (status == MODEM_CMD_OK) {
		ModemService = true;
		return( true );
	}

	delay = ModemGuardTime;
	while ( delay > 0 ) {
		//delay = HMInputLine( Port, delay, buffer, 81 );
		delay = SerialPort->Get_Modem_Result(delay, buffer, 81);

	}

	escape[0] = ModemEscapeCode;
	escape[1] = ModemEscapeCode;
	escape[2] = ModemEscapeCode;
	escape[3] = 0;

	//status = HMSendStringNoWait( Port, escape, -1 );
	SerialPort->Write_To_Serial_Port((unsigned char*)escape, 3);

	delay = ModemGuardTime;
	while ( delay > 0 ) {
		delay = SerialPort->Get_Modem_Result(delay, buffer, 81);
		//delay = HMInputLine( Port, delay, buffer, 81 );

		if ( strncmp( buffer, "OK", 2 ) == 0 ) {
			break;
		}
	}

	status = Send_Modem_Command( "ATH", '\r', buffer, 81, ModemHangupDelay, 1 );

	if (status == MODEM_CMD_OK) {
	} else {
		ModemService = true;
		return( false );
	}

	status = Send_Modem_Command( "ATZ", '\r', buffer, 81, DEFAULT_TIMEOUT, 1 );

	if (status == MODEM_CMD_OK) {
	} else {
		ModemService = true;
		return( false );
	}

	ModemService = true;
	return( true );

}	/* end of Hangup_Modem */


/***********************************************************************************************
 * NMC::Setup_Modem_Echo -- Sets the echo callback function pointer                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Ptr to callback function                                                          *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/2/96 12:48PM ST : Documented and added WIn32 support                                   *
 *=============================================================================================*/
void NullModemClass::Setup_Modem_Echo( void ( *func )( char c) )
{
	SerialPort->Set_Echo_Function(func);
	//HMSetUpEchoRoutine( func );

}	/* end of Setup_Modem_Echo */


/***********************************************************************************************
 * NMC::Remove_Modem_Echo -- Set the echo function callback pointer to null                    *
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
 *    8/2/96 12:50PM ST : Documented / Win32 support added                                     *
 *=============================================================================================*/
void NullModemClass::Remove_Modem_Echo( void )
{
	SerialPort->Set_Echo_Function(NULL);
	//HMSetUpEchoRoutine( NULL );

}	/* end of Remove_Modem_Echo */


/***********************************************************************************************
 * NMC::Print_EchoBuf -- Print out the contents of the echo buffer                             *
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
 *    8/2/96 12:51PM ST : Documented                                                           *
 *=============================================================================================*/
void NullModemClass::Print_EchoBuf( void )
{
	for (int i = 0; i < strlen(NullModem.EchoBuf); i++) {
		if (NullModem.EchoBuf[i] == '\r') {
			NullModem.EchoBuf[i] = 1;
		} else {
			if (NullModem.EchoBuf[i] == '\n') {
				NullModem.EchoBuf[i] = 2;
			}
		}
	}
}


/***********************************************************************************************
 * NMC::Reset_EchoBuf -- Empties the echo buffer                                               *
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
 *    8/2/96 12:51PM ST : Documented                                                           *
 *=============================================================================================*/
void NullModemClass::Reset_EchoBuf( void )
{
	*EchoBuf = 0;
	EchoCount = 0;
}


/***********************************************************************************************
 * NMC::Abort_Modem -- Checks for user input so that modem operations can be aborted           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   ASUSERABORT if abort key pressed. ASSUCESS otherwise.                             *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/2/96 12:52PM ST : Documented                                                           *
 *=============================================================================================*/
int NullModemClass::Abort_Modem(void)
//int NullModemClass::Abort_Modem( PORT * )
{
	/*........................................................................
	Button Enumerations
	........................................................................*/
	enum {
		BUTTON_CANCEL = 100,
	};

	/*
	........................ Invoke game callback .........................
	*/
	Call_Back();

	/*
	........................... Get user input ............................
	*/
	Input = Commands->Input();

	switch ( Input ) {
		case (KN_ESC):
		case (BUTTON_CANCEL | KN_BUTTON):
			return( ASUSERABORT );
	}

	return( ASSUCCESS );

}	/* end of Abort_Modem */


/***********************************************************************************************
 * NMC::Setup_Abort_Modem -- sets the modem abort function pointer                             *
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
 *    8/2/96 2:59PM ST : Documented / Win32 support added                                      *
 *=============================================================================================*/
void NullModemClass::Setup_Abort_Modem( void )
{
	SerialPort->Set_Abort_Function((int(*)(void))Abort_Modem);
}	/* end of Setup_Abort_Modem */


/***********************************************************************************************
 * NMC::Remove_Abort_Modem -- Removes the modem abort function pointer                         *
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
 *    8/2/96 3:01PM ST : Documented / Win32 support added                                      *
 *=============================================================================================*/
void NullModemClass::Remove_Abort_Modem( void )
{
	SerialPort->Set_Abort_Function(NULL);
}	/* end of Remove_Abort_Modem */



/***********************************************************************************************
 * NMC::Change_IRQ_Priority -- Increases the priority of the serial interrupt                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Interrupt request number                                                          *
 *                                                                                             *
 * OUTPUT:   ASSUCCESS if changed                                                              *
 *                                                                                             *
 * WARNINGS: The Win32 version of this function does nothing.                                  *
 *           Priorities are controlled by windoze                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/2/96 3:03PM ST : Documented / Win32 support added                                      *
 *=============================================================================================*/
int NullModemClass::Change_IRQ_Priority( int )
{
	return (ASSUCCESS);
}	/* end of Change_IRQ_Priority */


/***********************************************************************************************
 * NMC::Get_Modem_Status -- returns status of modem control bits                               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Modem status                                                                      *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/2/96 3:06PM ST : Documented / Win32 support added                                      *
 *=============================================================================================*/
int NullModemClass::Get_Modem_Status( void )
{
	int modemstatus;
	int status;
	char buffer[81];

	//modemstatus = GetModemStatus( Port );
	modemstatus = SerialPort->Get_Modem_Status();

	status = Send_Modem_Command( "AT", '\r', buffer, 81, DEFAULT_TIMEOUT, 1 );

	if (status == MODEM_CMD_OK) {
		modemstatus &= (~CD_SET);
	}

	return( modemstatus );

}	/* end of Get_Modem_Status */


/***********************************************************************************************
 * NMC::Send_Modem_Command -- Sends an 'AT' command to the modem and gets the response         *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    command to send to modem. e.g. 'ATZ'                                              *
 *           terminator byte for command string                                                *
 *           buffer to put modem response into                                                 *
 *           length of above buffer                                                            *
 *           delay to wait for response                                                        *
 *           number of times to retry when modem doesnt respond                                *
 *                                                                                             *
 * OUTPUT:   input delay less the time it took the modem to respond                            *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/2/96 3:09PM ST : Documented / Win32 support added                                      *
 *=============================================================================================*/
int NullModemClass::Send_Modem_Command( char *command, char terminator, char *buffer, int buflen, int delay, int retries )
{
	return (SerialPort->Send_Command_To_Modem(command, terminator, buffer, buflen, delay, retries));
}


/***********************************************************************************************
 * NMC::Verify_And_Convert_To_Int -- converts a text string of numbers to an int               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to buffer                                                                     *
 *                                                                                             *
 * OUTPUT:   value of text number in buffer                                                    *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/2/96 3:13PM ST : Documented                                                            *
 *=============================================================================================*/
int NullModemClass::Verify_And_Convert_To_Int( char *buffer )
{
	int value = 0;
	int len = strlen(buffer);


	for (int i = 0; i < len; i++) {
		if ( !isdigit( *(buffer + i) ) ) {
			value = -1;
			break;
		}
	}

	if (value == 0) {
		value = atoi( buffer );
	}

	return( value );

}	/* end of Verify_And_Convert_To_Int */

/*************************** end of nullmgr.cpp ****************************/
#endif