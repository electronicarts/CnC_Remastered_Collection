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
 *                    File Name : TCPIP.CPP                                *
 *                                                                         *
 *                   Programmer : Steve Tall                               *
 *                                                                         *
 *                   Start Date : March 11th, 1996                         *
 *                                                                         *
 *                  Last Update : March 11th, 1996 [ST]                    *
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

extern bool Server;

#define FORCE_WINSOCK				1

#define WINSOCK_MINOR_VER		1
#define WINSOCK_MAJOR_VER		1
#define PORTNUM						0x1000
#define UDP_PORT					0x1001
#define WS_INTERNET_BUFFER_LEN  1024
#define WS_NUM_TX_BUFFERS		16		//Must be a power of 2
#define WS_NUM_RX_BUFFERS		16		//MUst be a power of 2
#define WS_RECEIVE_BUFFER_LEN	1024
//#define WS_IN_BUFFER_LEN			8192
//#define WS_OUT_BUFFER_LEN		8192

#define PLANET_WESTWOOD_HANDLE_MAX 20
#define PLANET_WESTWOOD_PASSWORD_MAX 20
#define IP_ADDRESS_MAX 40
#define PORT_NUMBER_MAX 6

//...........................................................................
// Custom messages: WM_USER + 1 to WM_USER + 100
// These will be sent to the dialog procedure, for display only.
//...........................................................................
#define	WM_UPDATE_STATUS		(WM_USER + 1)	// update status text
#define	WM_UPDATE_CLIENTS		(WM_USER + 2)	// update client list box
#define	WM_UPDATE_MESSAGE		(WM_USER + 3)	// update received message list

//...........................................................................
// Messages for Async processing.
//...........................................................................
#define	WM_ACCEPT				(WM_USER + 101)	// client wants to connect
#define	WM_HOSTBYADDRESS		(WM_USER + 102)	// async get host by address
#define	WM_HOSTBYNAME			(WM_USER + 103)	// async get host by name
#define	WM_ASYNCEVENT			(WM_USER + 104)	// other Async event
#define	WM_UDPASYNCEVENT		(WM_USER + 105)	// UDP socket Async event


#define	VSS_ID					-1			// ID of the VSS connection.

class TcpipManagerClass {

	public:

		TcpipManagerClass(void);
		~TcpipManagerClass(void);

		BOOL Init(void);
		void Start_Server(void);
		void Start_Client(void);
		void Close_Socket(SOCKET s);
	 	void Message_Handler(HWND window, UINT message, UINT wParam, LONG lParam);
		void Copy_To_In_Buffer(int bytes);
		int  Read(void *buffer, int buffer_len);
		void Write(void *buffer, int buffer_len);
		BOOL Add_Client(void);
		void Close(void);
		void Set_Host_Address(char *address);
		void Set_Protocol_UDP(BOOL state);
		void Clear_Socket_Error(SOCKET socket);

		inline BOOL Get_Connected(void) {return (Connected);}

		typedef enum ConnectStatusEnum {
			CONNECTED_OK = 0,
			NOT_CONNECTING,
			CONNECTING,
			UNABLE_TO_CONNECT_TO_SERVER,
			CONTACTING_SERVER,
			SERVER_ADDRESS_LOOKUP_FAILED,
			RESOLVING_HOST_ADDRESS,
			UNABLE_TO_ACCEPT_CLIENT,
			UNABLE_TO_CONNECT,
			CONNECTION_LOST
		} ConnectStatusEnum;

		inline ConnectStatusEnum Get_Connection_Status(void) {return (ConnectStatus);}

	private:

		//...........................................................................
		// This structure defines all the info we need about a host
		//...........................................................................
		typedef struct {
			struct in_addr Addr;					// address
			char DotAddr[16];						// decimal-dot address string
			char Name[255];						// character-string name
		} HostType;

		typedef struct {
			char	Buffer[WS_INTERNET_BUFFER_LEN];
			int	DataLength;
			bool	InUse:1;
		} InternetBufferType;


		BOOL 					WinsockInitialised;
		WSADATA				WinsockInfo;
		SOCKET				ListenSocket;
		SOCKET				ConnectSocket;
		SOCKET				UDPSocket;
		IN_ADDR				ClientIPAddress;
		HANDLE				Async;
		char					HostBuff[MAXGETHOSTSTRUCT];
		char					ClientName[128];
		char					ReceiveBuffer[WS_RECEIVE_BUFFER_LEN];
		//char					InBuffer[WS_IN_BUFFER_LEN];
		//int					InBufferHead;
		//int					InBufferTail;
		//char					OutBuffer[WS_OUT_BUFFER_LEN];
		//int					OutBufferHead;
		//int					OutBufferTail;
		BOOL					IsServer;
		BOOL					Connected;
		HostType				Server;
		char					HostAddress[IP_ADDRESS_MAX];
		ConnectStatusEnum ConnectStatus;
		BOOL					UseUDP;
		IN_ADDR				UDPIPAddress;
		int					SocketReceiveBuffer;
		int					SocketSendBuffer;
		InternetBufferType ReceiveBuffers[WS_NUM_TX_BUFFERS];
		InternetBufferType TransmitBuffers[WS_NUM_RX_BUFFERS];
		int					TXBufferHead;
		int					TXBufferTail;
		int					RXBufferHead;
		int					RXBufferTail;

};


extern TcpipManagerClass Winsock;

extern char PlanetWestwoodIPAddress[IP_ADDRESS_MAX];
extern long PlanetWestwoodPortNumber;
extern bool PlanetWestwoodIsHost;
extern int  Read_Game_Options(char *);
extern bool UseVirtualSubnetServer;
extern int  InternetMaxPlayers;


#define TXT_WINSOCK_CONNECTING							4567+13
#define TXT_WINSOCK_NOT_CONNECTING					4567+14
#define TXT_WINSOCK_UNABLE_TO_CONNECT_TO_SERVER	4567+15
#define TXT_WINSOCK_CONTACTING_SERVER				4567+16
#define TXT_WINSOCK_SERVER_ADDRESS_LOOKUP_FAILED	4567+17
#define TXT_WINSOCK_UNABLE_TO_ACCEPT_CLIENT		4567+18
#define TXT_WINSOCK_UNABLE_TO_CONNECT				4567+19
#define TXT_WINSOCK_CONNECTION_LOST					4567+20
#define TXT_WINSOCK_RESOLVING_HOST_ADDRESS			4567+21


#if (0)


struct tag tGameStatisticsStruct{
	char	WinnersName[20];
	char	LosersName[20];
	int	WinnersTeam;
	int	LosersTeam;
	int	WinnersCredits;
	int	LosersCredits;
	int	WinnersKills;
	int	LosersKills;
	int	ScenarioPlayed;
	int	GameTimeElapsed;
	int	VersionNumber;
	char	TimeDateStamp[12];
} GameStatisticsStruct;





extern GameStatisticsStruct GameStatistics;

#endif	//(0)


