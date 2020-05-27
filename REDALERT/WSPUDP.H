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
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                     $Archive:: /Sun/WSPUDP.h                                               $*
 *                                                                                             *
 *                      $Author:: Joe_b                                                       $*
 *                                                                                             *
 *                     $Modtime:: 8/05/97 6:45p                                               $*
 *                                                                                             *
 *                    $Revision:: 3                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef WSPUDP_H
#define WSPUDP_H

#include	"WSProto.h"
#include	<nspapi.h>




/*
** Class to allow access to UDP specific portions of the Winsock interface.
**
*/
class UDPInterfaceClass : public WinsockInterfaceClass {

	public:

		UDPInterfaceClass (void);
		virtual ~UDPInterfaceClass(void);

	 	virtual long Message_Handler(HWND window, UINT message, UINT wParam, LONG lParam);
		virtual bool Open_Socket ( SOCKET socketnum );
		virtual void Set_Broadcast_Address ( void *address );
		virtual void Broadcast (void *buffer, int buffer_len);

		virtual ProtocolEnum Get_Protocol (void) {
			return (PROTOCOL_UDP);
		};

		virtual int Protocol_Event_Message (void) {
			return (WM_UDPASYNCEVENT);
		};


	private:

		/*
		** Address to use when broadcasting a packet.
		*/
		DynamicVectorClass <unsigned char *> BroadcastAddresses;

		/*
		** List of local addresses.
		*/
		DynamicVectorClass <unsigned char *> LocalAddresses;
};



#endif