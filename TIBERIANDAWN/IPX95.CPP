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
 *                  Last Update : January 22nd, 1996   [ST]                *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 *                                                                         *
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
#include "ipx95.h"


// Stub in old IPX here ST - 12/20/2018 1:53PM
extern "C"{
	extern BOOL __stdcall IPX_Initialise(void) {return 0;}
	extern BOOL __stdcall IPX_Get_Outstanding_Buffer95(unsigned char *buffer) {return 0;}
	extern void __stdcall IPX_Shut_Down95(void) {}
	extern int  __stdcall IPX_Send_Packet95(unsigned char *, unsigned char *, int, unsigned char*, unsigned char*) {return 0;}
	extern int  __stdcall IPX_Broadcast_Packet95(unsigned char *, int){return 0;}
	extern BOOL __stdcall IPX_Start_Listening95(void){return 0;}
	extern int  __stdcall IPX_Open_Socket95(int socket){return 0;}
	extern void __stdcall IPX_Close_Socket95(int socket){}
	extern int  __stdcall IPX_Get_Connection_Number95(void){return 0;}
	extern int  __stdcall IPX_Get_Local_Target95(unsigned char *, unsigned char*, unsigned short, unsigned char*){return 0;}
}



int IPX_Open_Socket(unsigned short socket)
{
	return -1;
	//return (IPX_Open_Socket95((int)socket));	//ST - 12/20/2018 11:34AM
}


int IPX_Close_Socket(unsigned short socket)
{
	//IPX_Close_Socket95((int)socket);				//ST - 12/20/2018 11:34AM
	return (0);
}


int IPX_Get_Connection_Number(void)
{
	//return (IPX_Get_Connection_Number95());		//ST - 12/20/2018 11:34AM
	return -1;
}



int IPX_Broadcast_Packet(unsigned char *buf, int buflen)
{
	//return(IPX_Broadcast_Packet95(buf, buflen));		//ST - 12/20/2018 11:34AM
	return 0;
}

//extern "C"{
//	extern void __cdecl Int3(void);
//}
//ST - 12/20/2018 11:34AM

int IPX_Get_Local_Target(unsigned char *dest_network, unsigned char *dest_node,
	unsigned short dest_socket, unsigned char *bridge_address)
{
	//Int3();
	//	return (IPX_Get_Local_Target95(dest_network, dest_node, dest_socket, bridge_address));	//ST - 12/20/2018 11:34AM
	return 0;
}

