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

#if (0)//PG
#ifndef MP_LOAD_DLL_DYNAMICALLY
#define MP_LOAD_DLL_DYNAMICALLY
#endif

#include "mpmgrw.h"

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
}

#include <windows.h>

#define     STATUS_OK    1
#define     STATUS_BAD   0

#define     BROADCAST_ADDR    0

#define PRIVATE 0
#define PUBLIC  1

#define min(a,b) (((a) < (b)) ? (a) : (b))

typedef void __cdecl (*MPlayerInit_Type)(void);
typedef void __cdecl (*MPlayerDestroy_Type)(void);
typedef int __cdecl (*Send_Private_Message_Type)(void *buf,
					 int buflen,
                     int ack_req,
                     int conn_id);
typedef int __cdecl	(*Get_Private_Message_Type)(void *buf, int *buflen,
                    int *conn_id);
typedef int __cdecl (*Send_Global_Message_Type)(void *buf, int buflen, int ack_req,
   int addr);
typedef int __cdecl (*Get_Global_Message_Type)(void *buf, int *buflen, int *address);		
typedef int __cdecl (*Create_Connection_Type)(int id, char *name, int address);		
typedef int __cdecl (*Delete_Connection_Type)(int id);		
typedef char * __cdecl (*Connection_Name_Type)(int id);			
typedef int __cdecl	(*Connection_Address_Type)(int id);		
typedef int __cdecl	(*Num_Connections_Type)(void);			
typedef int __cdecl (*Connection_ID_Type)(int id);			
typedef int __cdecl (*Connection_Index_Type)(int id);
typedef int __cdecl	(*Init_Type)(void);					
typedef int __cdecl (*Find_Num_Connections_Type)(void);

MPlayerInit_Type MPlayerManCreate;
MPlayerDestroy_Type MPlayerManDestroy;
Send_Private_Message_Type Send_Private_Message_DLL;
Get_Private_Message_Type Get_Private_Message_DLL;
Send_Global_Message_Type Send_Global_Message_DLL;
Get_Global_Message_Type Get_Global_Message_DLL;
Create_Connection_Type Create_Connection_DLL;
Delete_Connection_Type Delete_Connection_DLL;
Connection_Name_Type Connection_Name_DLL;
Connection_Address_Type Connection_Address_DLL;
Num_Connections_Type Num_Connections_DLL;
Connection_ID_Type Connection_ID_DLL;
Connection_Index_Type Connection_Index_DLL;
Init_Type Init_DLL;
Find_Num_Connections_Type Find_Num_Connections_DLL;


FARPROC MPGetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
        FARPROC ret=GetProcAddress(hModule, lpProcName);
        if (!ret) {
                char msg[255];
                sprintf(msg, "Unable to load function %s from %s",
						lpProcName, "RA95MP.DLL");
                MessageBox(0, msg, "Warning", MB_OK);
        }
        return ret;
}

MPlayerManClass::MPlayerManClass(void) : ConnManClass()
{
	HMODULE lib;

	lib = LoadLibrary("ra95mp.dll");
	if (lib != 0) {
		MPlayerManCreate = (MPlayerInit_Type) MPGetProcAddress(lib,
			"MPlayerManCreate");
		MPlayerManDestroy = (MPlayerDestroy_Type) MPGetProcAddress(lib,
			"MPlayerManDestroy");
		Send_Private_Message_DLL = (Send_Private_Message_Type) MPGetProcAddress(lib,
			"Send_Private_Message");
		Get_Private_Message_DLL = (Get_Private_Message_Type) MPGetProcAddress(lib,
			"Get_Private_Message");
		Send_Global_Message_DLL = (Send_Global_Message_Type) MPGetProcAddress(lib,
			"Send_Global_Message");
		Get_Global_Message_DLL = (Get_Global_Message_Type) MPGetProcAddress(lib,
			"Get_Global_Message");
		Create_Connection_DLL = (Create_Connection_Type) MPGetProcAddress(lib,
			"Create_Connection");
		Delete_Connection_DLL = (Delete_Connection_Type) MPGetProcAddress(lib,
			"Delete_Connection");
		Connection_Name_DLL = (Connection_Name_Type) MPGetProcAddress(lib,
			"Connection_Name");
		Connection_Address_DLL = (Connection_Address_Type) MPGetProcAddress(lib,
			"Connection_Address");
		Num_Connections_DLL = (Num_Connections_Type) MPGetProcAddress(lib,
			"Num_Connections");
		Connection_ID_DLL = (Connection_ID_Type) MPGetProcAddress(lib,
			"Connection_ID");
		Connection_Index_DLL = (Connection_Index_Type) MPGetProcAddress(lib,
			"Connection_Index");
		Init_DLL = (Init_Type) MPGetProcAddress(lib,
			"Init");
		Find_Num_Connections_DLL = (Find_Num_Connections_Type) MPGetProcAddress(lib,
			"Find_Num_Connections");
	} else {
       MessageBox(0, "RA95MP.DLL not found!", "Warning", MB_OK);
		 exit(0);
	}
	
	if (MPlayerManCreate != 0) {
		MPlayerManCreate();
	}
}

MPlayerManClass::~MPlayerManClass()
{
	MPlayerManDestroy();
}

// here's what we do to get private & broadcasts over the same chunnel
// we package up an extra dword at the beginning to indicate the address

int
MPlayerManClass::Send_Private_Message(void *buf,
                                      int buflen,
                                      int ack_req,
                                      int conn_id)
{
	return Send_Private_Message_DLL(buf, buflen, ack_req, conn_id);
}

int
MPlayerManClass::Get_Private_Message(void *buf, int *buflen,
                                     int *conn_id)
{
	return Get_Private_Message_DLL(buf, buflen, conn_id);
}

int
MPlayerManClass::Send_Global_Message(void *buf, int buflen, int ack_req,
   int addr)
{
 	return Send_Global_Message_DLL(buf, buflen, ack_req, addr);
}

int
MPlayerManClass::Get_Global_Message(void *buf, int *buflen, int *address)
{
	return Get_Global_Message_DLL(buf, buflen, address);
}

int
MPlayerManClass::Service(void)
{
   return STATUS_OK;
}

int
MPlayerManClass::Create_Connection(int id, char *name, int address)
{
	return Create_Connection_DLL(id, name, address);
}

int
MPlayerManClass::Delete_Connection(int id)
{
	return Delete_Connection_DLL(id);
}

char *
MPlayerManClass::Connection_Name(int id)
{
	return Connection_Name_DLL(id);
}

int
MPlayerManClass::Connection_Address(int id)
{
	return Connection_Address_DLL(id);
}

int
MPlayerManClass::Num_Connections(void)
{
   return Num_Connections_DLL();
}

int
MPlayerManClass::Connection_ID(int index)
{
   return Connection_ID_DLL(index);
}

int
MPlayerManClass::Connection_Index(int id)
{
	return Connection_Index_DLL(id);
}

int
MPlayerManClass::Global_Num_Send(void)
{
   return 0;
}

int
MPlayerManClass::Global_Num_Receive(void)
{
	return 0;
//   return MGenGetQueueCtr(GDOSPENDINGQUEUE);
}

int
MPlayerManClass::Private_Num_Send(int /*id*/)
{
   return 0;
}

int
MPlayerManClass::Private_Num_Receive(int /*id*/)
{
   return 0;
}

void
MPlayerManClass::Reset_Response_Time(void)
{
   // unsupported
}

unsigned long
MPlayerManClass::Response_Time(void)
{
   return (160 * 60) / 1000;   // 160 microseconds one way (9 ticks)
}

void
MPlayerManClass::Set_Timing(unsigned long /*retrydelta*/,
                            unsigned long /*maxretries*/,
                            unsigned long /*timeout*/)
{
   // unsupported
}

void
MPlayerManClass::Configure_Debug(int /*index*/, int /*type_offset*/,
                                 int /*type_size*/, char ** /*names*/,
                                 int /*namestart*/, int /*namecount*/)
{
   // unsupported
}

void
MPlayerManClass::Mono_Debug_Print(int /*index*/, int /*refresh*/)
{
   // unsupported
}

int
MPlayerManClass::Init(void)
{
	return Init_DLL();
}

int MPlayerManClass::Find_Num_Connections(void)
{
	return Find_Num_Connections_DLL();
}


void MPlayerManClass::Flush_All(void)
{
}

#endif