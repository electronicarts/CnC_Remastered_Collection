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

#include "mpmgrd.h"

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "types.h"
#include "rtq.h"
#include "services.h"
}
#include "mplib.h"
#include "mplpc.h"


#define     STATUS_OK    1
#define     STATUS_BAD   0

#define     BROADCAST_ADDR    0

typedef struct {
   DWORD      address;
   char       Data[1];
} packet;

#define min(a,b) (((a) < (b)) ? (a) : (b))

#define     FREEQUEUE            0
#define     DOSWORKQUEUE         1
#define     WINWORKQUEUE         2
#define     WINSENDQUEUE         3
#define     DOSPENDINGQUEUE      4
#define     WINWORKQUEUE2        5

// 6, 7, 8, taken up by LPC services

#define     GDOSWORKQUEUE        14
#define     GWINWORKQUEUE        15
#define     GWINSENDQUEUE        16
#define     GDOSPENDINGQUEUE     17
#define     GWINWORKQUEUE2       18

MPlayerManClass::MPlayerManClass(void) : ConnManClass()
{
   unsigned  size;

   MGenGetMasterNode(&size);
   if (size != sizeof(RTQ_NODE)) {
      exit(-234);
   }
   _myAddr = LPCGetMPAddr();
   _nConnections = 0;

   for (int i = 0; i < CONNECT_MAX; i++) {
      _Connections[i] = 0;
      strcpy(_Names[i], "");
   }
}

// here's what we do to get private & broadcasts over the same chunnel
// we package up an extra dword at the beginning to indicate the address

int
MPlayerManClass::Send_Private_Message(void *buf,
                                      int buflen,
                                      int /* ack_req */,
                                      int conn_id)
{
   RTQ_NODE *n;
	int idx = Connection_Index(conn_id);

	if (_nConnections == 0) {
		return (STATUS_OK);
	}

   while ((n = MGenMoveTo(FREEQUEUE, DOSWORKQUEUE)) == 0);

   packet *p = (packet *) n->rtqDatum;

   if (conn_id == CONNECTION_NONE) {
      p->address = BROADCAST_ADDR;
   } else {
      p->address = _Connections[idx];
   }

   memcpy(p->Data, buf, buflen);
   n->rtqUpCtr = (WORD)(buflen + sizeof(DWORD));

   MGenMoveTo(DOSWORKQUEUE, WINSENDQUEUE);
   PostWindowsMessage();
   Yield();

   return STATUS_OK;
}

int
MPlayerManClass::Get_Private_Message(void *buf, int *buflen,
                                     int *conn_id)
{
   RTQ_NODE *n;
	int i;

   if ((n = MGenMoveTo(DOSPENDINGQUEUE, DOSWORKQUEUE)) == 0) {
      *buflen = 0;
      return 0;
   }

   packet *p = (packet *) n->rtqDatum;

   int lentocpy = n->rtqUpCtr - sizeof(DWORD);

   *conn_id = CONNECTION_NONE;
	for (i = 0; i < _nConnections; i++) {
		if (p->address == _Connections[i]) {
			(*conn_id) = _ID[i];
			break;
		}
	}

   memcpy(buf, p->Data, lentocpy);

   *buflen = lentocpy;

   MGenMoveTo(DOSWORKQUEUE, FREEQUEUE);

   return STATUS_OK;
}

int
MPlayerManClass::Send_Global_Message(void *buf, int buflen, int /*ack_req*/,
   int address)
{
   RTQ_NODE *n;

   while ((n = MGenMoveTo(FREEQUEUE, DOSWORKQUEUE)) == 0);

   packet *p = (packet *) n->rtqDatum;

   if (address == 0) {
      p->address = BROADCAST_ADDR;
   } else {
      p->address = address;
   }

   memcpy(p->Data, buf, buflen);
   n->rtqUpCtr = (WORD)(buflen + sizeof(DWORD));

   MGenMoveTo(DOSWORKQUEUE, GWINSENDQUEUE);
   PostWindowsMessage();
   Yield();

   return STATUS_OK;
}

int
MPlayerManClass::Get_Global_Message(void *buf, int *buflen, int *address)
{
   RTQ_NODE *n;

   if ((n = MGenMoveTo(GDOSPENDINGQUEUE, DOSWORKQUEUE)) == 0) {
      *buflen = 0;
      return 0;
   }

   packet *p = (packet *) n->rtqDatum;

   int lentocpy = n->rtqUpCtr - sizeof(DWORD);

   if (address) {
      if (p->address == BROADCAST_ADDR) {
         *address = 0;
      } else {
         *address = p->address;
      }
   }

   memcpy(buf, p->Data, lentocpy);

   *buflen = lentocpy;

   MGenMoveTo(DOSWORKQUEUE, FREEQUEUE);

   return STATUS_OK;
}

int
MPlayerManClass::Service(void)
{
   return STATUS_OK;
}

int
MPlayerManClass::Create_Connection(int id, char *name, int address)
{
   _Connections[_nConnections] = address;
	_ID[_nConnections] = id;
   strcpy(_Names[_nConnections], name);
   _nConnections++;
   return STATUS_OK;
}

int
MPlayerManClass::Delete_Connection(int id)
{
	int i;
	int idx = Connection_Index(id);
	if (idx == -1)
		return 0;

	for (i = idx; i < _nConnections - 1; i++) {
		_Connections[i] = _Connections[i+1];
		_ID[i] = _ID[i+1];
		strcpy (_Names[i], _Names[i+1]);
	}
   _nConnections--;
   return STATUS_OK;
}

char *
MPlayerManClass::Connection_Name(int id)
{
	int idx = Connection_Index(id);
	if (idx==-1) {
		return (NULL);
	}

   return _Names[idx];
}

int
MPlayerManClass::Connection_Address(int id)
{
	int idx = Connection_Index(id);
	if (idx==-1) {
		return (0);
	}

   return _Connections[idx];
}

int
MPlayerManClass::Num_Connections(void)
{
   return _nConnections;
}

int
MPlayerManClass::Connection_ID(int index)
{
   return _ID[index];
}

int
MPlayerManClass::Connection_Index(int id)
{
	int i;

	for (i = 0; i < _nConnections; i++) {
		if (_ID[i] == id) {
			return i;
		}
	}

   return -1;
}

int
MPlayerManClass::Global_Num_Send(void)
{
   return 0;
}

int
MPlayerManClass::Global_Num_Receive(void)
{
   return MGenGetQueueCtr(GDOSPENDINGQUEUE);
}

int
MPlayerManClass::Private_Num_Send(int /*id*/)
{
   return 0;
}

int
MPlayerManClass::Private_Num_Receive(int /*id*/)
{
   return MGenGetQueueCtr(DOSPENDINGQUEUE);
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
   return STATUS_OK;
}

int MPlayerManClass::Find_Num_Connections(void)
{
	TGAMEDEF game_def;
	int sz = sizeof(game_def);

	GetGameDef(&game_def, &sz);

	return (game_def.numPlayers - 1);

}


void MPlayerManClass::Flush_All(void)
{
	MGenFlushNodes(DOSPENDINGQUEUE, FREEQUEUE);
	MGenFlushNodes(GDOSPENDINGQUEUE, FREEQUEUE);
}

