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

#ifndef mpmgr_h
#define mpmgr_h

#include "connmgr.h"

// maximum number of connections
#define CONNECT_MAX    7
#define MAX_NAME_LEN   128

class MPlayerManClass : public ConnManClass {
public:
   MPlayerManClass(void);

   // queues up incoming packets appropriately
   int   Service(void);

   // initialization
   int   Init(void);
	int Find_Num_Connections(void);
	void Flush_All(void);

   // send/receive data
   int  Send_Private_Message(void *buf, int buflen, int ack_req = 1, int conn_id = CONNECTION_NONE);
   int  Get_Private_Message(void *buf, int *buflen, int *conn_id);

   int  Send_Global_Message(void *buf, int buflen, int ack_req = 0, int address = 0);
   int  Get_Global_Message(void *buf, int *buflen, int *address = 0);

   // manage connections
   int  Num_Connections(void);
   int  Connection_ID(int index);
   int  Connection_Index(int id);
   int  Create_Connection(int id, char *name, int address);
   int  Delete_Connection(int id);
   char *Connection_Name(int id);
   int  Connection_Address(int id);

   // queueing routines

   int Global_Num_Send(void);
   int Global_Num_Receive(void);
   int Private_Num_Send(int id = CONNECTION_NONE);
   int Private_Num_Receive(int id = CONNECTION_NONE);

   // timing magnagement
   void Reset_Response_Time(void);
   unsigned long Response_Time(void);
   void Set_Timing(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout);

   // debug
   void Configure_Debug(int index, int type_offset, int type_size, char **names, int namestart,
      int namecount);
   void Mono_Debug_Print(int index, int refresh);

private:
   int  _myAddr;
   int  _Connections[CONNECT_MAX];
	int  _ID[CONNECT_MAX];
   char _Names[CONNECT_MAX][MAX_NAME_LEN];
   int  _nConnections;
};

#endif // mpmgr_h

