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
 *                                                                         * 
 *                 Project Name : Command & Conquer                        * 
 *                                                                         * 
 *                    File Name : TENMGR.H                                 * 
 *                                                                         * 
 *                   Programmer : Bill R. Randolph                         * 
 *                                                                         * 
 *                   Start Date : 06/26/96                                 * 
 *                                                                         * 
 *                  Last Update : June 26, 1996 [BRR]                      * 
 *                                                                         * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

class TenConnManClass : public ConnManClass
{
	public:
		//
		// Constructor / Destructor
		//
		TenConnManClass(void);
		virtual ~TenConnManClass();

		//
		// Initialization
		//
	   int Init(void);

		//
		// Service routine
		//
		virtual int Service(void);

		//
		// Sending & receiving data
		//
		virtual int Send_Private_Message (void *buf, int buflen, 
			int ack_req = 1, int conn_id = CONNECTION_NONE);
		virtual int Get_Private_Message (void *buf, int *buflen, 
			int *conn_id);

	   int Send_Global_Message(void *buf, int buflen, 
	   	int ack_req = 0, int address = 0);
	   int Get_Global_Message(void *buf, int *buflen, 
	   	int *address = 0);

		//
		// Connection management
		//
		virtual int Num_Connections(void);
		virtual int Connection_ID(int index);
		virtual int Connection_Index(int id);

	   int Create_Connection(int id, char *name, int address);
	   int Delete_Connection(int id);
	   char *Connection_Name(int id);
	   int Connection_Address(int id);

		//
		// Queue utility routines
		//
		virtual int Global_Num_Send(void);
		virtual int Global_Num_Receive(void);
		virtual int Private_Num_Send(int id = CONNECTION_NONE);
		virtual int Private_Num_Receive(int id = CONNECTION_NONE);

		void Flush_All(void);

		//
		// Timing management
		//
		virtual void Reset_Response_Time(void);
		virtual unsigned long Response_Time(void);
		virtual void Set_Timing (unsigned long retrydelta, 
			unsigned long maxretries, unsigned long timeout);

		//
		// Debugging
		//
		virtual void Configure_Debug(int index, int type_offset, int type_size, 
			char **names, int namestart, int namecount);
		virtual void Mono_Debug_Print(int index, int refresh);

		CommBufferClass *GlobalQueue;
		CommBufferClass *PrivateQueue;

		//
		// This flag will be set if I'm the game host.
		//
		int IsHost;

	private:
	   int Connections[MAX_PLAYERS];
		int ID[MAX_PLAYERS];
	   char Names[MAX_PLAYERS][128];
	   int NumConnections;

};

/***************************** end of tenmgr.h *****************************/
