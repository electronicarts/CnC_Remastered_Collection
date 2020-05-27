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
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPX95PP                                  *
 *                                                                         *
 *                   Programmer : Steve Tall                               *
 *                                                                         *
 *                   Start Date : January 22nd, 1996                       *
 *                                                                         *
 *                  Last Update : July 10th, 1996   [ST]                   *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Overview:                                                               *
 *                                                                         *
 *  Windows 95 equivelent functions from IPX.CPP                           *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "function.h"

#ifdef WIN32

#include "ipx95.h"

 // Stub in old IPX here ST - 12/20/2018 1:53PM
extern "C" {
	extern BOOL __stdcall IPX_Initialise(void) { return 0; }
	extern BOOL __stdcall IPX_Get_Outstanding_Buffer95(unsigned char *buffer) { return 0; }
	extern void __stdcall IPX_Shut_Down95(void) {}
	extern int  __stdcall IPX_Send_Packet95(unsigned char *, unsigned char *, int, unsigned char*, unsigned char*) { return 0; }
	extern int  __stdcall IPX_Broadcast_Packet95(unsigned char *, int) { return 0; }
	extern BOOL __stdcall IPX_Start_Listening95(void) { return 0; }
	extern int  __stdcall IPX_Open_Socket95(int socket) { return 0; }
	extern void __stdcall IPX_Close_Socket95(int socket) {}
	extern int  __stdcall IPX_Get_Connection_Number95(void) { return 0; }
	extern int  __stdcall IPX_Get_Local_Target95(unsigned char *, unsigned char*, unsigned short, unsigned char*) { return 0; }
}

#if (0)
/*
** Instance handle for the THIPX32 .DLL
*/
HINSTANCE	IpxDllInstance = NULL;

/*
** Function pointers
*/
//extern "C" {
	IPXInitialiseType					IPX_Initialise = NULL;
	IPXGetOutstandingBuffer95Type	IPX_Get_Outstanding_Buffer95 = NULL;
	IPXShutDown95Type             IPX_Shut_Down95 = NULL;
	IPXSendPacket95Type           IPX_Send_Packet95 = NULL;
	IPXBroadcastPacket95Type      IPX_Broadcast_Packet95 = NULL;
	IPXStartListening95Type       IPX_Start_Listening95 = NULL;
	IPXOpenSocket95Type           IPX_Open_Socket95 = NULL;
	IPXCloseSocket95Type          IPX_Close_Socket95 = NULL;
	IPXGetConnectionNumber95Type  IPX_Get_Connection_Number95 = NULL;
	IPXGetLocalTarget95           IPX_Get_Local_Target95 = NULL;
//}

char const *FunctionNames[] = {
	"_IPX_Initialise",
	"_IPX_Get_Outstanding_Buffer95",
	"_IPX_Shut_Down95",
	"_IPX_Send_Packet95",
	"_IPX_Broadcast_Packet95",
	"_IPX_Start_Listening95",
	"_IPX_Open_Socket95",
	"_IPX_Close_Socket95",
	"_IPX_Get_Connection_Number95",
	"_IPX_Get_Local_Target95",
	NULL
};
#endif

extern void Get_OS_Version (void);
bool				WindowsNT = false;


/***********************************************************************************************
 * Load_IPX_Dll -- Loads the THIPX32 DLL                                                       *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   true if DLL loaded                                                                *
 *                                                                                             *
 * WARNINGS: This call will fail under NT due to a side effect of loading the THIPX32.DLL      *
 *           which causes the 16 bit DLL THIPX16.DLL to load.                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    4/1/97 11:40AM ST : Created                                                              *
 *=============================================================================================*/
bool Load_IPX_Dll (void)
{
	//ST 5/13/2019
	return false;

#if (0)
	Get_OS_Version();
	if (WindowsNT) return (false);

	SetErrorMode( SEM_NOOPENFILEERRORBOX );
	IpxDllInstance = LoadLibrary ( "THIPX32.DLL" );
	SetErrorMode ( 0 );

	if ( IpxDllInstance ){

		const char		*function_name;
		unsigned long 	*fptr = (unsigned long *) &IPX_Initialise;
		int				count = 0;

		do {
			function_name = FunctionNames[count];
			if (function_name){
				*fptr = (unsigned long) GetProcAddress (IpxDllInstance, function_name);
				assert (*fptr != NULL);
				fptr++;
				count++;
			}

		} while ( function_name );

		return (true);

	}else{
		return (false);
	}
#endif
}



/***********************************************************************************************
 * Unload_IPX_Dll -- Frees the THIPX32 library                                                 *
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
 *    4/1/97 2:37PM ST : Created                                                               *
 *=============================================================================================*/
void Unload_IPX_Dll (void)
{
	//ST 5/13/2019

#if (0)
	if (IpxDllInstance){
		FreeLibrary (IpxDllInstance);
		IpxDllInstance = NULL;
	}
#endif
}


int IPX_Open_Socket(unsigned short socket)
{
	return -1;	//ST 5/13/2019
	//return ( IPX_Open_Socket95((int)socket));
}

int IPX_Close_Socket(unsigned short socket)
{
	//ST 5/13/2019 IPX_Close_Socket95((int)socket);
	return (0);
}


int IPX_Get_Connection_Number(void)
{
	return -1;//ST 5/13/2019
	//return (IPX_Get_Connection_Number95());
}



int IPX_Broadcast_Packet(unsigned char * buf, int buflen)
{
	return 0; //ST 5/13/2019
	//return(IPX_Broadcast_Packet95(buf, buflen));
}

#if (0) //ST 5/13/2019
extern "C"{
	extern void __cdecl Int3(void);
}
#endif

int IPX_Get_Local_Target(unsigned char * dest_network, unsigned char * dest_node,
	unsigned short dest_socket, unsigned char * bridge_address)
{
	//Int3();
	
	return 0; //ST 5/13/2019
	//return (IPX_Get_Local_Target95(dest_network, dest_node, dest_socket, bridge_address));
}


#endif	//WIN32
