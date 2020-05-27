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

/* $Header: /counterstrike/SESSION.H 4     3/10/97 6:23p Steve_tall $ */
/***************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : SESSION.H                                *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 11/30/95                                 *
 *                                                                         *
 *                  Last Update : November 30, 1995 [BRR]                  *
 *                                                                         *
 * The purpose of this class is to contain those variables & routines		*
 * specifically related to a multiplayer game.										*
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef SESSION_H
#define SESSION_H

#include "ipxaddr.h"
#include "msglist.h"
#include "connect.h"

//---------------------------------------------------------------------------
// Forward declarations
//---------------------------------------------------------------------------
class AircraftClass;
class AnimClass;
class BuildingClass;
class BulletClass;
class InfantryClass;
class UnitClass;
class PhoneEntryClass;
class CellClass;

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
//...........................................................................
// Various limiting values
//...........................................................................
#define	MPLAYER_BUILD_LEVEL_MAX		10		// max build level in multiplay
#define	MAX_MPLAYER_COLORS			8		// max # of colors

//...........................................................................
// Max sizes of packets we want to send
// The IPX packet's size is IPX's max size (546), rounded down to accommodate
// the max number of events possible.
//...........................................................................
#define	MAX_IPX_PACKET_SIZE			(((546 - sizeof(CommHeaderType)) / \
												sizeof(EventClass) ) * sizeof(EventClass))
#define	MAX_SERIAL_PACKET_SIZE		256

//...........................................................................
// Max length of player names fields; attempt to use the constant for the
// HouseClass, if it's been defined; otherwise, define it myself.
//...........................................................................
#ifdef HOUSE_NAME_MAX
#define	MPLAYER_NAME_MAX				HOUSE_NAME_MAX
#else
#define	MPLAYER_NAME_MAX				12		// max length of a player's name
#endif

//...........................................................................
// Values to control the multiplayer score screen
//...........................................................................
#define	MAX_MULTI_NAMES	8		// max # names (rows) on the score screen
#define	MAX_MULTI_GAMES	4		// max # games (columns) on the score screen

//...........................................................................
// Min value for MaxAhead, for both net & modem; only applies for
// COMM_PROTOCOL_MULTI_E_COMP.
//...........................................................................
#define MODEM_MIN_MAX_AHEAD			5
#define NETWORK_MIN_MAX_AHEAD			2

//...........................................................................
// Send period (in frames) for COMM_PROTOCOL_MULTI_E_COMP and above
//...........................................................................
#define DEFAULT_FRAME_SEND_RATE		3

//...........................................................................
// Modem-specific constants
//...........................................................................
#define	PORTBUF_MAX						64		// dialog field sizes
#define	IRQBUF_MAX						3
#define	BAUDBUF_MAX						7
#define	INITSTRBUF_MAX					41
#define	CWAITSTRBUF_MAX				16
#define	CREDITSBUF_MAX					5
#define	PACKET_TIMING_TIMEOUT		40		// ticks b/w sending a timing packet
#define	MODEM_NAME_MAX					PORTBUF_MAX - 1	// Max length of modem name in list box

//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------
//...........................................................................
// Types of games; used to tell which protocol we're using
//...........................................................................
typedef enum GameEnum {
	GAME_NORMAL,									// not multiplayer
	GAME_MODEM,										// modem game
	GAME_NULL_MODEM,								// NULL-modem
	GAME_IPX,										// IPX Network game
	GAME_INTERNET,									// Internet H2H
	GAME_SKIRMISH,									// 1 plr vs. AI's
	GAME_TEN,										// TEN Network game
	GAME_MPATH,										// MPath Network game
	GAME_GLYPHX_MULTIPLAYER						// Multiplayer game controlled by the GLYPHX engine. ST - 5/14/2019 11:41AM
} GameType;

//...........................................................................
// Various Modem-specific enums
//...........................................................................
typedef enum DetectPortType {
	PORT_VALID = 0,
	PORT_INVALID,
	PORT_IRQ_INUSE
} DetectPortType;

typedef enum DialStatusType {
	DIAL_CONNECTED			= 0,
	DIAL_NO_CARRIER,
	DIAL_BUSY,
	DIAL_ERROR,
	DIAL_NO_DIAL_TONE,
	DIAL_CANCELED
} DialStatusType;

typedef enum DialMethodType {
	DIAL_TOUCH_TONE = 0,
	DIAL_PULSE,
	DIAL_METHODS
} DialMethodType;

typedef enum CallWaitStringType {
	CALL_WAIT_TONE_1 = 0,
	CALL_WAIT_TONE_2,
	CALL_WAIT_PULSE,
	CALL_WAIT_CUSTOM,
	CALL_WAIT_STRINGS_NUM
} CallWaitStringType;

typedef enum ModemGameType {
	MODEM_NULL_HOST = 0,
	MODEM_NULL_JOIN,
	MODEM_DIALER,
	MODEM_ANSWERER
} ModemGameType;

//...........................................................................
// Commands sent over the serial Global Channel
//...........................................................................
typedef enum SerialCommandType {
	SERIAL_CONNECT			= 100,	// Are you there?  Hello?  McFly?
	SERIAL_GAME_OPTIONS	= 101,	// Hey, dudes, here's some new game options
	SERIAL_SIGN_OFF		= 102,	// Bogus, dudes, my boss is coming; I'm outta here!
	SERIAL_GO				= 103,	// OK, dudes, jump into the game loop!
	SERIAL_MESSAGE			= 104,	// Here's a message
	SERIAL_TIMING			= 105,	// timimg packet
	SERIAL_SCORE_SCREEN	= 106,	// player at score screen
	SERIAL_LOADGAME		= 107,	// Start the game, loading a saved game first
	SERIAL_LAST_COMMAND,				// last command
	SERIAL_REQ_SCENARIO	= 1000,	// Reqest that host sends the scenario file to the other players.
	SERIAL_FILE_INFO		= 1001,	// Info about the file that is going to be transferred
	SERIAL_FILE_CHUNK		= 1002,	// A chunk of scenario
	SERIAL_READY_TO_GO	= 1003,	// Sent in response to a 'GO' command
	SERIAL_NO_SCENARIO	= 1004	// Scenario isnt available on remote machine so we cant play
} SerialCommandType;

//...........................................................................
// Commands sent over the network Global Channel
//...........................................................................
typedef enum NetCommandType {
	NET_QUERY_GAME,			// Hey, what games are out there?
	NET_ANSWER_GAME,			// Yo, Here's my game's name!
	NET_QUERY_PLAYER,			// Hey, what players are in this game?
	NET_ANSWER_PLAYER,		// Yo, I'm in that game!
	NET_CHAT_ANNOUNCE,		// I'm at the chat screen
	NET_CHAT_REQUEST,			// Respond with a CHAT_ANNOUNCE, please.
	NET_QUERY_JOIN,			// Hey guys, can I play too?
	NET_CONFIRM_JOIN,			// Well, OK, if you really want to.
	NET_REJECT_JOIN,			// No, you can't join; sorry, dude.
	NET_GAME_OPTIONS,			// Hey, dudes, here's some new game options
	NET_SIGN_OFF,				// Bogus, dudes, my boss is coming; I'm outta here!
	NET_GO,						// OK, jump into the game loop!
	NET_MESSAGE,				// Here's a message
	NET_PING,					// I'm pinging you to take a time measurement
	NET_LOADGAME,				// start a game by loading a saved game
	NET_REQ_SCENARIO  =1000,// Reqest that host sends the scenario file to the other players.
	NET_FILE_INFO		=1001,// Info about the file that is going to be transferred
	NET_FILE_CHUNK		=1002,// A chunk of scenario
	NET_READY_TO_GO	=1003,// Sent in response to a 'GO' command
	NET_NO_SCENARIO	=1004	// Scenario isnt available on remote machine so we cant play
} NetCommandType;

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------
//...........................................................................
// An entry on the score screen is defined by this structure
//...........................................................................
typedef struct {
	char Name[MPLAYER_NAME_MAX];
	int Wins;
	int Kills[MAX_MULTI_GAMES];
	PlayerColorType Color;
} MPlayerScoreType;

//...........................................................................
// Settings for the serial port
//...........................................................................
typedef struct {
	int Port;
	int IRQ;
	int Baud;
	DialMethodType DialMethod;
	int InitStringIndex;
	int CallWaitStringIndex;
	char CallWaitString[ CWAITSTRBUF_MAX ];
	bool Compression;
	bool ErrorCorrection;
	bool HardwareFlowControl;
	char ModemName [ MODEM_NAME_MAX ];
} SerialSettingsType;

//...........................................................................
//	This is a "node", used for the lists of available games & players.  The
//	'Game' structure is used for games; the 'Player' structure for players.
//...........................................................................
typedef struct NodeNameTag {
	char Name[MPLAYER_NAME_MAX];		// player or game name
		IPXAddressClass Address;
#if(TEN)
		int TenAddress;
#endif
#if(MPATH)
		int MPathAddress;
#endif
	union {
		struct {
			unsigned char IsOpen;		// is the game open?
			unsigned long LastTime;		// last time we heard from this guy
		} Game;
		struct {
			HousesType House;				// "ActLike" House of this player
			PlayerColorType Color;		// Color of this player
			HousesType ID;					// Actual House of this player
			int ProcessTime;				// Length of time to process players main loop
		} Player;
		struct {
			unsigned long LastTime;		// last time we heard from this guy
			unsigned char LastChance;	// we're about to remove him from the list
			PlayerColorType Color;		// chat player's color
		} Chat;
	};
} NodeNameType;


//...........................................................................
// Packet sent over the serial Global Channel
//...........................................................................
typedef struct {
	SerialCommandType Command;					// One of the enum's defined above
	char Name[MPLAYER_NAME_MAX];				// Player or Game Name
	unsigned char ID;								// unique ID of sender of message
	union {
		struct {
			HousesType House;								// player's House
			PlayerColorType Color;						// player's color or SIGNOFF ID
			unsigned long MinVersion;					// min version this game supports
			unsigned long MaxVersion;					// max version this game supports
			char Scenario[DESCRIP_MAX];				// Scenario name
			unsigned int Credits;						// player's credits
			unsigned int IsBases		: 1;				// 1 = bases are allowed
			unsigned int IsTiberium	: 1;				// 1 = tiberium is allowed
			unsigned int IsGoodies	: 1;				// 1 = goodies are allowed
			unsigned int IsGhosties	: 1;				// 1 = ghosts are allowed
			unsigned int OfficialScenario : 1;		//	Is this scenario an official Westwood one?
			int CheatCheck;								// Unique ID of "rules.ini" file.
			unsigned char BuildLevel;					// buildable level
			unsigned char UnitCount;					// max # units
			unsigned char AIPlayers;					// # of AI players allowed
			int Seed;										// random number seed
			SpecialClass Special;						// command-line options
			unsigned int GameSpeed;						// Game Speed
			unsigned long ResponseTime;				// packet response time
			unsigned int FileLength;					// Length of scenario file to expect from host.
#ifdef WOLAPI_INTEGRATION
			char ShortFileName[13];						// Name of scenario file to expect from host
#else
			char ShortFileName[12];						// Name of scenario file to expect from host
#endif
			unsigned char FileDigest[32];				// Digest of scenario file to expect from host
														//	ajw - This is not necessarily null-terminated.
		} ScenarioInfo;
		struct {
			char Message[MAX_MESSAGE_LENGTH];		// inter-player message
			PlayerColorType Color;						// player's color or SIGNOFF ID
		} Message;
		struct {
			PlayerColorType Color;						// player's color or SIGNOFF ID
		} Chat;
	};
} SerialPacketType;

//...........................................................................
// Other packet sent over the serial global channel (for file transfers)
//...........................................................................
#define MAX_SEND_FILE_PACKET_SIZE MAX_SERIAL_PACKET_SIZE - 64
typedef struct {
	SerialCommandType	Command;					// Enum defined above. Should be a file transfer enum.
	unsigned short 	BlockNumber;			// Index position of this file chunk in the file
	unsigned short		BlockLength;			// Length of data in the RawData buffer
	unsigned char 		RawData	[MAX_SEND_FILE_PACKET_SIZE];
} RemoteFileTransferType;


//...........................................................................
// Packet sent over the network Global Channel
//...........................................................................
typedef struct GlobalPacketType {
	NetCommandType Command;						// One of the enum's defined above
	char Name[MPLAYER_NAME_MAX];				// Player or Game Name
	union {
		struct {
			unsigned int IsOpen		: 1;		// 1 = game is open for joining
		} GameInfo;
		struct {
			HousesType House;						// player's House
			PlayerColorType Color;				// player's color
			unsigned long NameCRC;				// CRC of player's game's name
			unsigned long MinVersion;			// game's min supported version
			unsigned long MaxVersion;			// game's max supported version
			int CheatCheck;						// Unique ID of "rules.ini" file.
		} PlayerInfo;
		struct {
			char Scenario[DESCRIP_MAX];		// Scenario Name
			unsigned int Credits;				// player's credits
			unsigned int IsBases		: 1;		// 1 = bases are allowed
			unsigned int IsTiberium	: 1;		// 1 = tiberium is allowed
			unsigned int IsGoodies	: 1;		// 1 = goodies are allowed
			unsigned int IsGhosties	: 1;		// 1 = ghosts are allowed
			unsigned int OfficialScenario :1;// Is this scenario an official Westwood one?
			unsigned char BuildLevel;			// buildable level
			unsigned char UnitCount;			// max # units
			unsigned char AIPlayers;			// # of AI players allowed
			int Seed;								// random number seed
			SpecialClass Special;				// command-line options
			unsigned int GameSpeed;				// Game Speed
			unsigned long Version;				// version # common to all players
			unsigned int FileLength;			// Length of scenario file to expect from host.
#ifdef WOLAPI_INTEGRATION
			char ShortFileName[13];				// Name of scenario file to expect from host
#else
			char ShortFileName[12];				// Name of scenario file to expect from host
#endif
			unsigned char FileDigest[32];		// Digest of scenario file to expect from host
												//	ajw - This is not necessarily null-terminated.
		} ScenarioInfo;
		struct {
			char Buf[MAX_MESSAGE_LENGTH];		// inter-user message
			PlayerColorType Color;					// color of sender of message
			unsigned long NameCRC;				// CRC of sender's Game Name
		} Message;
		struct {
			int OneWay;								// one-way response time
		} ResponseTime;
		struct {
			int Why;									// why were we rejected from the game?
		} Reject;
		struct {
			unsigned long ID;						// unique ID for this chat node
			PlayerColorType Color;				// my color
		} Chat;
	};
} GlobalPacketType;


//...........................................................................
// For finding sync bugs; filled in by the engine when certain conditions
// are met; the pointers allow examination of objects in the debugger.
//...........................................................................
typedef struct {
	union {
		AircraftClass *Aircraft;
		AnimClass *Anim;
		BuildingClass *Building;
		BulletClass *Bullet;
		InfantryClass *Infantry;
		UnitClass *Unit;
		void *All;
	} Ptr;
} TrapObjectType;

/*
**	This is the identifier for a multiplayer mission. This can be used to
**	identify the filename of the mission as well as display the mission in a
**	mission selection list.
*/
class MultiMission
{
	public:
		MultiMission(char const * filename = NULL, char const * description = NULL, char const *digest = NULL, bool official = true, bool expansion = false);

		void Set_Description(char const * description);
		void Set_Filename(char const * filename);
		void Set_Digest(char const * digest);
		void Set_Official(bool official);
		void Set_Expansion(bool expansion);
		void Draw_It(int index, int x, int y, int width, int height, bool selected, TextPrintType flags) const;
		char const * Description(void) const {return(ScenarioDescription);}
		char const * Get_Filename(void) const {return(Filename);}
		char const * Get_Digest(void) const {return(Digest);}
		bool Get_Official(void) { return (IsOfficial); }
		bool Get_Expansion(void) { return (IsExpansion); }	//	Implied "IsCounterstrike". No longer used. -ajw

	private:
		char ScenarioDescription[DESCRIP_MAX];
		char Filename[_MAX_FNAME+_MAX_EXT];
		char Digest[32];
		bool IsOfficial;
		bool IsExpansion;
};


typedef struct {
	int 	ScenarioIndex;		//Used on host machine only as index into scenario list
	int 	Bases;
	int 	Credits;
	int 	Tiberium;
	int 	Goodies;
	int 	Ghosts;
	int 	UnitCount;
	int 	AIPlayers;			// # of AI players allowed to be built
	char	ScenarioDescription [DESCRIP_MAX];	//Used on client machines only
} GameOptionsType;

//---------------------------------------------------------------------------
// Class Definition
//---------------------------------------------------------------------------
class SessionClass
{
	//------------------------------------------------------------------------
	// Public interface
	//------------------------------------------------------------------------
	public:
		//.....................................................................
		// Constructor/Destructor
		//.....................................................................
		SessionClass(void);
		~SessionClass(void);

		//.....................................................................
		// Initialization
		//.....................................................................
		void One_Time(void);
		void Init(void);

		//.....................................................................
		// Reads/writes to the INI file
		//.....................................................................
		void Read_MultiPlayer_Settings (void);
		void Write_MultiPlayer_Settings (void);
		void Read_Scenario_Descriptions (void);
		void Free_Scenario_Descriptions(void);

		//.....................................................................
		// Utility functions
		//.....................................................................
		int Create_Connections(void);
		bool Am_I_Master(void);
		unsigned long Compute_Unique_ID(void);

#if(TEN)
		int Create_TEN_Connections(void);
#endif	// TEN

#if(MPATH)
		int Create_MPATH_Connections(void);
#endif	// MPATH

		//.....................................................................
		// File I/O
		//.....................................................................
		int Save(Pipe & file) const;
		int Load(Straw & file);
		int Save(CCFileClass & file);
		int Load(CCFileClass & file);

		//.....................................................................
		// Debugging / Sync Bugs
		//.....................................................................
		void Trap_Object(void);

		//---------------------------------------------------------------------
		// Public Data
		//---------------------------------------------------------------------
		//.....................................................................
		// The type of session being played
		//.....................................................................
		GameType Type;

		//.....................................................................
		// The current communications protocol
		//.....................................................................
		CommProtocolType CommProtocol;

		//.....................................................................
		// Game options
		//.....................................................................
		GameOptionsType Options;

		//.....................................................................
		// Unique workstation ID, for detecting my own packets
		//.....................................................................
		unsigned long UniqueID;

		//.....................................................................
		// Player's local options
		//.....................................................................
		char Handle[MPLAYER_NAME_MAX];		// player name
		PlayerColorType PrefColor;				// preferred color index
		PlayerColorType ColorIdx;				// actual color index
		HousesType House;							// GDI / NOD
		int ObiWan;									// 1 = player can see all
		int Solo;									// 1 = player can play alone

		//.....................................................................
		// Max allowable # of players & actual # of (human) players
		//.....................................................................
		int MaxPlayers;
		int NumPlayers;

		//.....................................................................
		// Frame-sync'ing timing variables
		// 'MaxAhead' is the number of frames ahead of this one to execute
		// a given packet.  It's set by the RESPONSE_TIME event.
		// 'FrameSendRate' is the # frames between data packets
		//.....................................................................
		unsigned long MaxAhead;
		unsigned long FrameSendRate;

		int			DesiredFrameRate;

		int			ProcessTimer;
		int			ProcessTicks;
		int			ProcessFrames;

		//.....................................................................
		// This flag is set when we've loaded a multiplayer game.
		//.....................................................................
		int LoadGame;

		//.....................................................................
		// This flag is set when the modem game saves the game due to a lost
		// connection.
		//.....................................................................
		int EmergencySave;

		//.....................................................................
		// List of scenarios & their file numbers
		//.....................................................................
		DynamicVectorClass<MultiMission *> Scenarios;
//		DynamicVectorClass <char *> Scenarios;
//		DynamicVectorClass <int> Filenum;

		char ScenarioFileName[_MAX_FNAME+_MAX_EXT+1];	//File name of scenario to load

		char ScenarioDigest [32];								//Digest of scenario to load
		unsigned int ScenarioFileLength;
		bool ScenarioIsOfficial;

		char ScenarioRequests[20];		//Which players requested scenario files
		int  RequestCount;
		IPXAddressClass	HostAddress;

		//.....................................................................
		// This is the multiplayer messaging system
		//.....................................................................
		MessageListClass Messages;
		IPXAddressClass MessageAddress;
		char LastMessage[MAX_MESSAGE_LENGTH];
		unsigned WWChat		: 1;	// 1 = go into special WW Chat mode

		//.....................................................................
		// This is the multiplayer scorekeeping system
		//.....................................................................
		MPlayerScoreType Score[MAX_MULTI_NAMES];
		int GamesPlayed;		// # games played this run
		int NumScores;			// # active entries in MPlayerScore
		int Winner;				// index of winner of last game
		int CurGame;			// index of current game being played

		//.....................................................................
		// Static arrays
		//.....................................................................
		static char Descriptions[100][40];
		static int CountMin[2];
		static int CountMax[2];
		static char * GlobalPacketNames[];
		static char * SerialPacketNames[];

		//.....................................................................
		// For Recording & Playing back a file
		//.....................................................................
		CCFileClass RecordFile;
		unsigned Record				: 1;
		unsigned Play				 	: 1;
		unsigned Attract			 	: 1;

		//.....................................................................
		// IPX-specific variables
		//.....................................................................
		int IsBridge;								// 1 = we're crossing a bridge
		IPXAddressClass BridgeNet;				// address of bridge
		bool NetStealth;							// makes us invisible
		bool NetProtect;							// keeps others from messaging us
		bool NetOpen;								// 1 = game is open for joining
		char GameName[MPLAYER_NAME_MAX];		// game's name
		GlobalPacketType GPacket;				// global packet
		int GPacketlen;							// global packet length
		IPXAddressClass GAddress;				// address of sender
		unsigned short GProductID;				// product ID of sender
		char MetaPacket[MAX_IPX_PACKET_SIZE]; // packet building buffer
		int MetaSize;								// size of MetaPacket
		DynamicVectorClass <NodeNameType *> Games;	// list of games
		DynamicVectorClass <NodeNameType *> Players;	// list of players
		DynamicVectorClass <NodeNameType *> Chat;		// list of chat nodes

		//.....................................................................
		// Modem-specific variables
		//.....................................................................
		unsigned ModemService		: 1;				// 1 = service modem in Call_Back
		int CurPhoneIdx;							// phone listing index
		SerialSettingsType SerialDefaults;	// default serial settings
		ModemGameType ModemType;				// caller or answerer?

		DynamicVectorClass<PhoneEntryClass *> PhoneBook;
		DynamicVectorClass <char *> InitStrings;
		static char * DialMethodCheck[ DIAL_METHODS ];
		static char * CallWaitStrings[ CALL_WAIT_STRINGS_NUM ];

		//.....................................................................
		// For finding Sync Bugs
		//.....................................................................
		long TrapFrame;				// frame # to start trapping 'TrapObject'
		RTTIType TrapObjType;		// type of object to trap
		TrapObjectType TrapObject;	// ptr to object to trap (watch)
		COORDINATE TrapCoord;		// coord of object, 0 = ignore
		TARGET TrapTarget;			// Target # of object, 0 = ignore
		CellClass * TrapCell;		// Ptr to cell to trap (watch)
		int TrapCheckHeap;			// true = check the heap as of TrapFrame
		long TrapPrintCRC;			// Frame # to print CRC state file

#if(TEN)
		//
		// TEN-specific variables
		//
		char *TenPacket;
		int TenSize;
		int TenMessageAddress;
		int TenAddress;
		int TenPlayerID;
		char OptionsFile[256];
		int AllowSolo;
		int NetResponseTime;
#endif	// TEN

#if(MPATH)
		//
		// MPATH-specific variables
		//
		char *MPathPacket;
		int MPathSize;
		int MPathMessageAddress;
		int MPathAddress;
		char OptionsFile[256];
		int AllowSolo;
		int NetResponseTime;
#endif	// MPATH
};

#endif		// SESSION_H

/*************************** end of session.h ******************************/
