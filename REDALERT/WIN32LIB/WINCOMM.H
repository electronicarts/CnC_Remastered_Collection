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
 *                 Project Name : Command & Conquer/ WW Library                                *
 *                                                                                             *
 *                    File Name : WINCOMM.H                                                    *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 1/10/96                                                      *
 *                                                                                             *
 *                  Last Update : January 10th 1996 [ST]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *                                                                                             *
 *   These classes was created to replace the greenleaf comms functions used in C&C DOS with   *
 *  WIN32 API calls.                                                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 *                                                                                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef WIN32
#define	WIN32
#define	_WIN32
#endif	//WIN32
#include <windows.h>

typedef enum WinCommDialMethodType {
	WC_TOUCH_TONE = 0,
	WC_PULSE
} WinCommDialMethodType;



#define	COMMSUCCESS		0
#define 	ASTIMEOUT      -10
#define	COMMUSERABORT	-16


/*
** The size of our serial buffer within the class.
**
** !!!!!! THIS MUST BE  A POWER OF 2 !!!!!!
**
*/
#define SIZE_OF_WINDOWS_SERIAL_BUFFER 2048



/*
** WinModemClass.
**
** This class provides access to modems under Win95. The functions are designed to be more or less
** drop in replacements for the Grenleaf comms functions.
*/

class WinModemClass
{

	public:

					WinModemClass (void);		//WinModemClass Contructor
		virtual	~WinModemClass (void);		//WinModemClass Destructor


					/*
					** Serial port open should be called to get a handle to the COM port
					** This needs to be called first as other class members rely on the handle
					**
					** Replacement for Greenleaf function: PortOpenGreenleafFast
					*/
		//virtual	HANDLE	Serial_Port_Open (int port, int baud, int parity, int wordlen, int stopbits);
		virtual	HANDLE	Serial_Port_Open (char *device_name, int baud, int parity, int wordlen, int stopbits, int flowcontrol);

					/*
					** This function releases the COM port handle and should be called after
					** communications have finished
					**
					** Replacement for Greenleaf function: PortClose
					*/
					void		Serial_Port_Close (void);

					/*
					** This member copies any bytes from the internal class serial buffer
					** into your user buffer.
					**
					** Replacement for Greenleaf function: ReadBuffer
					*/
					int		Read_From_Serial_Port (unsigned char *dest_ptr, int buffer_len);

					/*
					** Write chars to the serial port
					**
					** Replacement for Greenleaf function: WriteBuffer
					*/
					void		Write_To_Serial_Port (unsigned char *buffer, int length);

					/*
					** Wait for the outgoing buffer to empty
					*/
					void		Wait_For_Serial_Write (void);

					/*
					** Set the dial type to DIAL_TOUCH_TONE or DIAL_PULSE
					**
					** Replacement for Greenleaf function: HMSetDiallingMethod
					*/
		virtual	void		Set_Modem_Dial_Type (WinCommDialMethodType method);

					/*
					** Get the status of the modem control lines
					** Possible flags are: CTS_SET DSR_SET RI_SET & CD_SET
					**
					** Replacement for Greenleaf function: GetModemStatus
					*/
		virtual	unsigned	Get_Modem_Status (void);

					/*
					** Set the DTR line to the given state
					**
					** Replacement for Greenleaf function: SetDtr
					*/
		virtual	void		Set_Serial_DTR (BOOL state);

					/*
					** Get the result code from the modem after issuing an 'AT' command
					**
					** Replacement for Greenleaf function: HMInputLine
					*/
		virtual	int		Get_Modem_Result (int delay, char *buffer, int buffer_len);

					/*
					** Issue a dial command to the modem.
					** Use Set_Modem_Dial_Type to select pulse or tone dial
					**
					** Replacement for Greenleaf function: HMDial
					*/
		virtual	void		Dial_Modem (char *dial_number);

					/*
					** Send a command to the modem. This is usually an 'AT' command.
					** Function will optionally retry until 'OK' is received.
					*/
		virtual	int		Send_Command_To_Modem (char *command, char terminator, char *buffer, int buflen, int delay, int retries);

					/*
					** Sets a pointer to a function that will be called for each incoming serial char
					**
					** Replacement for Greenleaf function: HMSetUpEchoRoutine
					*/
		virtual	void		Set_Echo_Function (void(*func)(char c));

					/*
					** Sets a pointer to a function that will be called if ESC is pressed during a dial
					**
					** Replacement for Greenleaf function: HMSetUpAbortKey
					*/
		virtual	void		Set_Abort_Function (int (*func)(void));

					/*
					** Member to allow access to the serial port handle
					*/
					HANDLE	Get_Port_Handle(void);

		/*
		** Status vars for debugging purposes
		*/
		int FramingErrors;
		int IOErrors;
		int BufferOverruns;
		int InBufferOverflows;
		int ParityErrors;
		int OutBufferOverflows;
		int InQueue;
		int OutQueue;

		/*
		** Modem send result codes
		*/
		enum SendModemEnum {
			MODEM_CMD_TIMEOUT = 0,
			MODEM_CMD_OK,
			MODEM_CMD_0,
			MODEM_CMD_ERROR
		};


		/*
		** Enums for modem status flags
		*/
		enum {
			CTS_SET  = 0x10,
			DSR_SET  = 0x20,
          RI_SET   = 0x40,
          CD_SET   = 0x80
       };


	protected:


		/*
		** Copy incoming data from the windows file buffer into the internal class buffer
		*/
		BOOL							Read_Serial_Chars(void);

		/*
		** Pointer to the internal class circular buffer for incoming data
		*/
		unsigned char				*SerialBuffer;

		/*
		** Overlap object for asyncronous reads from the serial port
		*/
		OVERLAPPED					ReadOverlap;

		/*
		** Overlap object for asyncronous writes to the serial port
		*/
		OVERLAPPED					WriteOverlap;

		/*
		** Flag that there is no outstanding incoming data in the windows buffer
		*/
		BOOL							WaitingForSerialCharRead;

		/*
		** Flag that we are waiting for the last write to port operation to complete
		*/
		BOOL							WaitingForSerialCharWrite;

		/*
		** Head and Tail pointers for our internal serial buffer
		*/
		int							SerialBufferReadPtr;
		int							SerialBufferWritePtr;

		/*
		** Windows handle to the COM port device
		*/
		HANDLE						PortHandle;

		/*
		** Dialing method - DIAL_TOUCH_TONE or DIAL_PULSE
		*/
		WinCommDialMethodType	DialingMethod;

		/*
		** Pointer to function for echoing incoming data - can be NULL
		*/
		void 							(*EchoFunction)(char c);

		/*
		** Pointer to function for aborting when ESC pressed - can be NULL
		*/
		int							(*AbortFunction)(void);

		/*
		** Serial buffer for asyncronous reads
		*/
		char							TempSerialBuffer[SIZE_OF_WINDOWS_SERIAL_BUFFER];
};










/*
** WinNullModemClass.
**
** This class provides access to serial ports under Win95. The functions are designed to be more or less
** drop in replacements for the Grenleaf comms functions.
**
** This class just overloads the WinModemClass members that arent required for direct serial communications
** via a 'null modem' cable.
*/
class WinNullModemClass : public WinModemClass
{

	public:

		virtual	inline	void		Set_Modem_Dial_Type (int){};
		virtual	inline	unsigned	Get_Modem_Status (void){return (0);};
		virtual	inline	void		Set_Serial_DTR (BOOL){};
		virtual	inline	int		Get_Modem_Result (int, char*, int){return(0);};
		virtual	inline	void		Dial_Modem (char*){};
		virtual	inline	int		Send_Command_To_Modem (char*, char, char*, int, int, int){return (0);};
		virtual	inline	void		Set_Echo_Function (void(*)(char)){};
		virtual	inline	void		Set_Abort_Function (int(*)(void)){};

};


extern WinModemClass *SerialPort;









//
//
// This bit swiped from the SDK because its not in the Watcom headers yet
//
//

/************************************************************************
*                                                                       *
*   mcx.h -- This module defines the 32-Bit Windows MCX APIs            *
*                                                                       *
*   Copyright (c) 1990-1995, Microsoft Corp. All rights reserved.       *
*                                                                       *
************************************************************************/

#ifndef _MCX_H_
#define _MCX_H_

typedef struct _MODEMDEVCAPS {
    DWORD   dwActualSize;
    DWORD   dwRequiredSize;
    DWORD   dwDevSpecificOffset;
    DWORD   dwDevSpecificSize;

    // product and version identification
    DWORD   dwModemProviderVersion;
    DWORD   dwModemManufacturerOffset;
    DWORD   dwModemManufacturerSize;
    DWORD   dwModemModelOffset;
    DWORD   dwModemModelSize;
    DWORD   dwModemVersionOffset;
    DWORD   dwModemVersionSize;

    // local option capabilities
    DWORD   dwDialOptions;          // bitmap of supported values
    DWORD   dwCallSetupFailTimer;   // maximum in seconds
    DWORD   dwInactivityTimeout;    // maximum in seconds
    DWORD   dwSpeakerVolume;        // bitmap of supported values
    DWORD   dwSpeakerMode;          // bitmap of supported values
    DWORD   dwModemOptions;         // bitmap of supported values
    DWORD   dwMaxDTERate;           // maximum value in bit/s
    DWORD   dwMaxDCERate;           // maximum value in bit/s

    // Variable portion for proprietary expansion
    BYTE    abVariablePortion [1];
} MODEMDEVCAPS, *PMODEMDEVCAPS, *LPMODEMDEVCAPS;

typedef struct _MODEMSETTINGS {
    DWORD   dwActualSize;
    DWORD   dwRequiredSize;
    DWORD   dwDevSpecificOffset;
    DWORD   dwDevSpecificSize;

    // static local options (read/write)
    DWORD   dwCallSetupFailTimer;       // seconds
    DWORD   dwInactivityTimeout;        // seconds
    DWORD   dwSpeakerVolume;            // level
    DWORD   dwSpeakerMode;              // mode
    DWORD   dwPreferredModemOptions;    // bitmap

    // negotiated options (read only) for current or last call
    DWORD   dwNegotiatedModemOptions;   // bitmap
    DWORD   dwNegotiatedDCERate;        // bit/s

    // Variable portion for proprietary expansion
    BYTE    abVariablePortion [1];
} MODEMSETTINGS, *PMODEMSETTINGS, *LPMODEMSETTINGS;

// Dial Options
#define DIALOPTION_BILLING  0x00000040  // Supports wait for bong "$"
#define DIALOPTION_QUIET    0x00000080  // Supports wait for quiet "@"
#define DIALOPTION_DIALTONE 0x00000100  // Supports wait for dial tone "W"

// SpeakerVolume for MODEMDEVCAPS
#define MDMVOLFLAG_LOW      0x00000001
#define MDMVOLFLAG_MEDIUM   0x00000002
#define MDMVOLFLAG_HIGH     0x00000004

// SpeakerVolume for MODEMSETTINGS
#define MDMVOL_LOW          0x00000000
#define MDMVOL_MEDIUM       0x00000001
#define MDMVOL_HIGH         0x00000002

// SpeakerMode for MODEMDEVCAPS
#define MDMSPKRFLAG_OFF         0x00000001
#define MDMSPKRFLAG_DIAL        0x00000002
#define MDMSPKRFLAG_ON          0x00000004
#define MDMSPKRFLAG_CALLSETUP   0x00000008

// SpeakerMode for MODEMSETTINGS
#define MDMSPKR_OFF         0x00000000
#define MDMSPKR_DIAL        0x00000001
#define MDMSPKR_ON          0x00000002
#define MDMSPKR_CALLSETUP   0x00000003

// Modem Options
#define MDM_COMPRESSION      0x00000001
#define MDM_ERROR_CONTROL    0x00000002
#define MDM_FORCED_EC        0x00000004
#define MDM_CELLULAR         0x00000008
#define MDM_FLOWCONTROL_HARD 0x00000010
#define MDM_FLOWCONTROL_SOFT 0x00000020
#define MDM_CCITT_OVERRIDE   0x00000040
#define MDM_SPEED_ADJUST     0x00000080
#define MDM_TONE_DIAL        0x00000100
#define MDM_BLIND_DIAL       0x00000200
#define MDM_V23_OVERRIDE     0x00000400

#endif /* _MCX_H_ */
















