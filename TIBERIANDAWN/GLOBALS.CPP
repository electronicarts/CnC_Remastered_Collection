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

/* $Header:   F:\projects\c&c\vcs\code\globals.cpv   2.17   16 Oct 1995 16:52:22   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : GLOBALS.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : September 10, 1993   [JLB]                                   *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

#ifdef JAPANESE
bool ForceEnglish = false;
#endif

bool Debug_Quiet = false;
bool Debug_Cheat = false;
bool Debug_Remap = false;
bool Debug_Icon = false;
bool Debug_Flag = false;
bool Debug_Lose = false;
bool Debug_Win = false;
bool Debug_Map = false;					// true = map editor mode
bool Debug_Passable = false;			// true = show passable/impassable terrain
bool Debug_Unshroud = false;			// true = hide the shroud
bool Debug_Threat = false;
bool Debug_Find_Path = false;
bool Debug_Check_Map = false;			// true = validate the map each frame
bool Debug_Playtest = false;
int  In_Debugger = 0;
bool Debug_Heap_Dump = false;			// true = print the Heap Dump
bool Debug_Smart_Print = false;		// true = print everything that calls Smart_Printf
bool Debug_Trap_Check_Heap = false;	// true = check the Heap
bool Debug_Instant_Build = false;
bool Debug_Force_Crash = false;


TFixedIHeapClass<UnitClass>		Units;
TFixedIHeapClass<FactoryClass>	Factories;
TFixedIHeapClass<TerrainClass>	Terrains;
TFixedIHeapClass<TemplateClass>	Templates;
TFixedIHeapClass<SmudgeClass>		Smudges;
TFixedIHeapClass<OverlayClass>	Overlays;
TFixedIHeapClass<InfantryClass>	Infantry;
TFixedIHeapClass<BulletClass>		Bullets;
TFixedIHeapClass<BuildingClass>	Buildings;
TFixedIHeapClass<AnimClass>		Anims;
TFixedIHeapClass<AircraftClass>	Aircraft;
TFixedIHeapClass<TriggerClass>	Triggers;
TFixedIHeapClass<TeamTypeClass>	TeamTypes;
TFixedIHeapClass<TeamClass>		Teams;
TFixedIHeapClass<HouseClass>		Houses;


#ifdef PATCH
/***************************************************************************
**	Compatibility with version 1.07 flag.
*/
bool IsV107 = false;
char OverridePath[128]=".";
#endif


/***************************************************************************
**	This tracks all selected objects per house (for this map).
*/
SelectedObjectsType CurrentObject;


/***************************************************************************
**	This holds the custom version text that is fetched from the version
**	text file. This version is displayed on the options dialog.
*/
char VersionText[16];


/***************************************************************************
**	This is the VQ animation controller structure. It is filled in by reading
**	the PLAYER.INI and overridden through program control.
*/
//PG_TO_FIX
//VQAConfig AnimControl;

int PreserveVQAScreen;			// Used for screen mode transition control.
bool BreakoutAllowed = true;	// "true" if aborting of movies is allowed.
bool Brokeout;						// Was the movie broken out of?
bool SlowPalette = true;		// Slow palette flag set?


/***************************************************************************
**	These are the movie names to use for mission briefing, winning, and losing
**	sequences. They are read from the INI file.
*/
char IntroMovie[_MAX_FNAME+_MAX_EXT];
char BriefMovie[_MAX_FNAME+_MAX_EXT];
char WinMovie[_MAX_FNAME+_MAX_EXT];
char WinMovie2[_MAX_FNAME + _MAX_EXT];
char WinMovie3[_MAX_FNAME + _MAX_EXT];
char WinMovie4[_MAX_FNAME + _MAX_EXT];
char LoseMovie[_MAX_FNAME+_MAX_EXT];
char ActionMovie[_MAX_FNAME+_MAX_EXT];
char MovieThemeName[_MAX_FNAME + _MAX_EXT];
char BriefingText[512];
ThemeType TransitTheme = THEME_NONE;


/***************************************************************************
**	This records the view hotspots for the player. These are the cell numbers
**	of the upper left corner for the view position.
*/
CELL Views[4];


/***************************************************************************
**	This is the pending speech sample to play. This sample will be played
**	at the first opportunity.
*/
VoxType SpeakQueue = VOX_NONE;


/***************************************************************************
**	This records if the score (music) file is present. If not, then much of
**	the streaming score system can be disabled.
*/
bool ScoresPresent;


/***************************************************************************
**	This flag will control whether there is a response from game units.
**	By carefully controlling this global, multiple responses are supressed
**	when a large group of infantry is given the movement order.
*/
bool AllowVoice = true;


/***************************************************************************
**	This counts the number of crates on the map. When this value reaches zero,
**	then a timer is started that will control crate creation.
*/
int CrateCount;
TCountDownTimerClass CrateTimer;
bool CrateMaker = false;


/***************************************************************************
**	This is the current frame number. This number is guaranteed to count
**	upward at the rate of one per game logic process. The target rate is 15
**	per second. This value is saved and restored with the saved game.
*/
long Frame = 0;


/***************************************************************************
**	These globals are constantly monitored to determine if the player
**	has won or lost. They get set according to the trigger events associated
**	with the scenario.
*/
bool PlayerWins;
bool PlayerLoses;
bool PlayerRestarts;

/*
** This flag is set if the player neither wins nor loses; it's mostly for
** multiplayer mode.
*/
bool PlayerAborts;


/***************************************************************************
**	This is the pointer for the speech staging buffer. This buffer is used
**	to hold the currently speaking voice data. Since only one speech sample
**	is played at a time, this buffer is only as big as the largest speech
**	sample that can be played.
*/
void * SpeechBuffer;


/***************************************************************************
**	This is a running accumulation of the number of ticks that were unused.
** This accumulates into a useful value that contributes to a
**	histogram of game performance.
*/
long SpareTicks;


/***************************************************************************
**	This is a special scenario count down value. End of game condition will
**	not be checked until this value reaches zero.
*/
int EndCountDown;


/***************************************************************************
**	When the player sabotages a building (scenario #6 GDI only) then when
**	the next scenario starts, that building will already be destroyed.
*/
StructType SabotagedType;


/***************************************************************************
**	If the Nod temple was destroyed by the ion cannon, then this flag will
**	be set to true.
*/
bool TempleIoned = false;


/***************************************************************************
**	This is the monochrome debug page array. The various monochrome data
**	screens are located here.
*/
MonoClass MonoArray[MonoClass::MAX_MONO_PAGES];
int MonoPage;			// The current page.


/***************************************************************************
**	This is true if the game is the currently in focus windows app
**
*/
bool GameInFocus = true;

/***************************************************************************
**	This holds the theater specific mixfiles.
*/
MixFileClass *TheaterData = NULL;
MixFileClass *TheaterIcons = NULL;
MixFileClass *LowTheaterData;
MixFileClass *MoviesMix = 0;
MixFileClass *GeneralMix = 0;
MixFileClass *ScoreMix = 0;


/***************************************************************************
**	This is the options control class. The options control such things as
**	game speed, visual controls, and other user settings.
*/
GameOptionsClass Options;


/***************************************************************************
**	Logic processing is controlled by this element. It handles both graphic
**	and AI logic.
*/
LogicClass Logic;


/***************************************************************************
**	This handles the background music.
*/
ThemeClass Theme;


/***************************************************************************
**	This is the main control class for the map.
*/
#ifdef SCENARIO_EDITOR
MapEditClass Map;
#else
MouseClass Map;
#endif

/**************************************************************************
**	The running game score is handled by this class (and member functions).
*/
ScoreClass Score;


/***************************************************************************
**	The running credit display is controlled by this class (and member
**	functions.
*/
CreditClass CreditDisplay;


/***************************************************************************
**	These are the bits that are set when the appropriate tutor message
**	has been displayed. Once the message has been displayed, it will not be
**	displayed again.
*/
long TutorFlags[2];


/**************************************************************************
** This class records the special command override options that C&C
**	supports.
*/
SpecialClass Special;


/***************************************************************************
**	General rules that control the game.
*/
RulesClass Rule;


/***************************************************************************
**	This is the scenario data for the currently loaded scenario.
** These variables should all be set together.
*/
HousesType Whom;							// Initial command line house choice.
unsigned Scenario;						// Scenario #
ScenarioPlayerType ScenPlayer;		// GDI, NOD, 2-Player, Multi-Player
ScenarioDirType ScenDir;				// East/West
ScenarioVarType ScenVar;				// variation A/B/C
char ScenarioName[_MAX_FNAME+_MAX_EXT];	// name of scenario
int CarryOverMoney;						// Carry over money from last scenario.
int CarryOverPercent;					// Carry over money percentage control.
int CarryOverCap;							// Maxmimum carry over money allowed.
int ScenarioInit;
bool SpecialFlag = false;


/***************************************************************************
** This value tells the sidebar what items it's allowed to add.  The
** lower the value, the simpler the sidebar will be.
*/
unsigned BuildLevel = 3;				// Buildable level (1 = simplest)


/***************************************************************************
** This value is computed every time a new scenario is loaded; it's a
** CRC of the INI and binary map files.
*/
unsigned long ScenarioCRC;


/***************************************************************************
**	The various tutor and dialog messages are located in the data block
**	referenced by this pointer.
*/
char const * SystemStrings;


/***************************************************************************
**	The game plays as long as this var is true.
*/
bool GameActive;


/***************************************************************************
**	This is a scratch variable that is used to when a reference is needed to
**	a long, but the value wasn't supplied to a function. This is used
**	specifically for the default reference value. As such, it is not stable.
*/
long LParam;


#ifdef SCENARIO_EDITOR
/***************************************************************************
** The currently-selected cell for the Scenario Editor
*/
CELL CurrentCell = 0;
#endif


/***************************************************************************
**	Most of the text in the game will use the six point font. These are the
**	pointers to the fonts. If it is NULL, then the font hasn't been loaded
**	yet.
*/
void const *Green12FontPtr;			// Green font for pressed in tabs
void const *Green12GradFontPtr;   // Graduated green font for tabs
void const *MapFontPtr;				// Standard very small font.
void const *Font3Ptr;					// Standard very small font.
void const *Font6Ptr;					// Standard small font.
void const *Font8Ptr;					// 8 point proportional.
void const *FontLEDPtr;				// LED fixed point font.
void const *VCRFontPtr;				// VCR font pointer.
void const *ScoreFontPtr;			// font for score & map selection screens
void const *GradFont6Ptr;			// gradient 6 point font pointer.


/***************************************************************************
**	This is the house that the human player is currently playing.
*/
HouseClass * PlayerPtr;


/***************************************************************************
**	Special palettes for MCGA mode goes here. These palette buffers are used
**	for pictures that do not use the game palette or are used for fading to
**	black.
*/
unsigned char *GamePalette;
unsigned char *BlackPalette;
unsigned char *WhitePalette;
unsigned char *OriginalPalette;
unsigned char *Palette;


/***************************************************************************
**	These are the event queues. One is for holding events until they are ready to be
**	sent to the remote computer for processing. The other list is for incoming events
**	that need to be executed when the correct frame has been reached.
*/
QueueClass<EventClass, MAX_EVENTS> OutList;
QueueClass<EventClass, (MAX_EVENTS * 8)> DoList;


/***************************************************************************
**	These are arrays/lists of trigger pointers for each cell & the houses.
*/
DynamicVectorClass<TriggerClass *> CellTriggers;
DynamicVectorClass<TriggerClass *> HouseTriggers[HOUSE_COUNT];


/***************************************************************************
**	This is an array of waypoints; each waypoint corresponds to a letter of
** the alphabet, and points to a cell number.  -1 means unassigned.
** The CellClass has a bit that tells if that cell has a waypoint attached to
** it; the only way to find which waypoint it is, is to scan this array.  This
** shouldn't be needed often; usually, you know the waypoint & you want the CELL.
*/
CELL Waypoint[WAYPT_COUNT];


/***************************************************************************
**	This is the list of BuildingTypes that define the AI's base.
*/
BaseClass Base;


/***************************************************************************
**	This value tells what type of multiplayer game we're playing.
*/
GameType GameToPlay = GAME_NORMAL;


/***************************************************************************
**	This is the current communications protocol
*/
CommProtocolType CommProtocol;


/***************************************************************************
**	These values are used for recording & playing back a game.
*/
CCFileClass RecordFile ("RECORD.BIN");
int RecordGame = 0;			// 1 = record a game
int SuperRecord = 0;			// 1 = reopen record file with every write
int PlaybackGame= 0;			// 1 = play back a game
int AllowAttract = 0;		// 1 = allow attract mode


/***************************************************************************
**	This is the null modem manager class.  Declaring this class doesn't
** perform any allocations;
** the class itself is ?? bytes.
*/
#if (0)	//PG_TO_FIX
bool ModemService = true;					// When false disable servicing modem.
NullModemClass NullModem (
   16,											// number of send entries
	64,											// number of receive entries
//	sizeof (EventClass) * MAX_EVENTS,	// maxlen of entry buffer
	(200 / sizeof(EventClass) ) * sizeof(EventClass) + sizeof( CommHeaderType ),
	0x1234);										// Magic number must have each digit unique
													// and different from the queue magic number


DynamicVectorClass <PhoneEntryClass *> PhoneBook;
int CurPhoneIdx;								// current phonebook index, for dialing

DynamicVectorClass <char *> InitStrings;

SerialSettingsType SerialDefaults;		// serial port default settings

//ModemGameType ModemGameToPlay;			// type of modem play Dialer, answerer, null

char *DialMethodCheck[ DIAL_METHODS ] = {
	"T",
	"P"
};

char *CallWaitStrings[ CALL_WAIT_STRINGS_NUM ] = {
	"*70,",
	"70#,",
	"1170,",
	"CUSTOM -                "
};
#endif

ModemGameType ModemGameToPlay;			// type of modem play Dialer, answerer, null

/***************************************************************************
**	Index into scenario description list box
*/
int ScenarioIdx;


/***************************************************************************
**	This array of flags tells if the given colors have been used, or are
*/
int ColorUsed[MAX_MPLAYER_COLORS];


/***************************************************************************
**	This string stores the player's name.
*/
char MPlayerName[MPLAYER_NAME_MAX];


/***************************************************************************
**	This is the array of remap colors.  Each player in a network game is
** assigned one of these colors.  The 'G' is for graphics drawing; the 'T'
** is for text printing (indicates a remap table for the font to use).
*/
int MPlayerGColors[MAX_MPLAYER_COLORS] = {
	5, 			// Yellow
	127, 			// Red
	135, 			// BlueGreen
	26,			// Orange
	4,				// Green
	202			// Blue-Grey
};

int MPlayerTColors[MAX_MPLAYER_COLORS] = {
	CC_GDI_COLOR, 			// Yellow
	CC_NOD_COLOR, 			// Red
	CC_BLUE_GREEN, 		// BlueGreen
	CC_ORANGE,				// Orange	//26
	CC_GREEN,				// Green
	CC_BLUE_GREY,			// Blue
};


/***************************************************************************
**	This is a list of all the names of the multiplayer scenarios that use
** bases (production), and those that don't.  There is a list for
** descriptions, and another for actual filenames.
*/
char MPlayerDescriptions[100][40];
DynamicVectorClass <char *> MPlayerScenarios;
DynamicVectorClass <int> MPlayerFilenum;


/***************************************************************************
**	This value determines the max allowable # of players.
*/
int MPlayerMax = 4;


/***************************************************************************
**	Multiplayer game options
*/
int MPlayerPrefColor;			// preferred color index for this player
int MPlayerColorIdx;				// actual color index of this player
HousesType MPlayerHouse;		// House of this player (GDI/NOD)
unsigned char MPlayerLocalID;	// ID of this player
int MPlayerCount;					// # of human players in this game
int MPlayerBases;					// 1 = bases are on for this scenario
int MPlayerCredits;				// # credits everyone gets
int MPlayerTiberium;				// 1 = tiberium enabled for this scenario
int MPlayerGoodies;				// 1 = goodies enabled for this scenario
int MPlayerGhosts;				// 1 = houses with no players will still play
int MPlayerSolo = 0;				// 1 = allows a single-player net game
int MPlayerUnitCount = 10;		// # units for non-base multiplayer scenarios


/*---------------------------------------------------------------------------
Min & Max unit count values; index0 = bases OFF, index1 = bases ON
---------------------------------------------------------------------------*/
int MPlayerCountMin[2] = {1,0};
int MPlayerCountMax[2] = {50,12};


/*---------------------------------------------------------------------------
MPlayerMaxAhead is the number of frames ahead of this one to execute a given
packet.  It's set by the RESPONSE_TIME event.
---------------------------------------------------------------------------*/
unsigned long MPlayerMaxAhead = 3;


/*---------------------------------------------------------------------------
'FrameSendRate' is the # frames between data packets
'FrameRateDelay' is the time ticks to wait between frames, for smoothing.
---------------------------------------------------------------------------*/
unsigned long FrameSendRate;


/***************************************************************************
**	Multiplayer ID's, stored in order of event execution.
** Format:
** bits 0-3: the "preferred" house of the player (GDI/NOD)
** bits 4-7: the player's Color Index
** These values are used as the IPX connection ID's.
*/
unsigned char MPlayerID [MAX_PLAYERS];


/***************************************************************************
** This array stores the actual HousesType for all players (MULT1, etc).
*/
HousesType MPlayerHouses [MAX_PLAYERS];


/***************************************************************************
** This array stores the names of all players in a multiplayer game.
*/
char MPlayerNames [MAX_PLAYERS][MPLAYER_NAME_MAX];


/***************************************************************************
**	This is a list of the messages received from / sent to other players,
** the address to send to (IPX only), and the last message received or
** sent (for the computer's messages).
*/
MessageListClass Messages;
IPXAddressClass MessageAddress;
char LastMessage[MAX_MESSAGE_LENGTH];


/***************************************************************************
** If this flag is set, computer AI will blitz the humans all at once;
** otherwise, the computer units trickle gradually out.
*/
int MPlayerBlitz = 0;


/***************************************************************************
** If this flag is set, we can move around the map, but we can't do anything.
** It means we've been defeated, but we're still allowed to watch the action.
*/
int MPlayerObiWan = 0;


/***************************************************************************
** These variables keep track of the multiplayer game scores.
*/
MPlayerScoreType MPlayerScore[MAX_MULTI_NAMES];
int MPlayerGamesPlayed;					// # games played this run
int MPlayerNumScores;					// # active entries in MPlayerScore
int MPlayerWinner;						// index of winner of last game
int MPlayerCurGame;						// index of current game being played


//
// This array stores the processing time required by all multiplayer systems.
// The values are stored in the same order as the 'MPlayerID' array.
//
int TheirProcessTime[MAX_PLAYERS - 1];
int DesiredFrameRate;


/***************************************************************************
** These values are used purely for the Mono debug display.  They show the
** names of the Global Channel packet types, and the event types.
*/
char *GlobalPacketNames[] = {
	"Game?",
	"Game!",
	"Player?",
	"Player!",
	"Join?",
	"Join!",
	"Reject",
	"GameOptions",
	"Sign Off",
	"GO!",
	"Message",
	"Ping"
};


// yeah, there's 100 empty names here, because the SerialCommandType starts at 100.
char *SerialPacketNames[] = {
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"CONNECT",
	"GAME_OPTIONS",
	"SIGN_OFF",
	"GO",
	"MESSAGE",
	"TIMING",
	"SCORE_SCREEN",
	"LAST_COMMAND",
};


/***************************************************************************
**	These variables are just to help find sync bugs.
*/
long TrapFrame = 0x7fffffff;				// frame to start trapping object values at
RTTIType TrapObjType = RTTI_NONE;		// type of object to trap
TrapObjectType TrapObject = {NULL};		// ptr to object being trapped
COORDINATE TrapCoord = 0;							// COORD of object to trap
void *TrapThis = NULL;						// 'this' ptr of object to trap
CellClass *TrapCell = NULL;				// for trapping a cell
int TrapCheckHeap = 0;						// start checking the Heap


/***************************************************************************
**	This is the network IPX manager class.  It handles multiple remote
** connections.  Declaring this class doesn't perform any allocations;
** the class itself is 140 bytes.
*/
IPXManagerClass Ipx (
	sizeof (GlobalPacketType),						// size of Global Channel packets
	((546 - sizeof(CommHeaderType)) / sizeof(EventClass) ) * sizeof(EventClass),
	10, 													// # entries in Global Queue
	8, 													// # entries in Private Queues
	VIRGIN_SOCKET, 									// Socket ID #
	IPXGlobalConnClass::COMMAND_AND_CONQUER);	// Product ID #


//#if(TIMING_FIX)
//
// These values store the min & max frame #'s for when MaxAhead >>increases<<.
// If MaxAhead increases, and the other systems free-run to the new MaxAhead
// value, they may miss an event generated after the MaxAhead event was sent,
// but before it executed, since it will have been scheduled with the older,
// shorter MaxAhead value.  This will cause a Packet_Received_Too_Late error.
// The frames from the point where the new MaxAhead takes effect, up to that
// frame Plus the new MaxAhead, represent a "period of vulnerability"; any
// events received that are scheduled to execute during this period should
// be re-scheduled for after that period.
//
int NewMaxAheadFrame1;
int NewMaxAheadFrame2;
//#endif

/***************************************************************************
**	This is the user-specified IPX address of a desired game owner machine.
** Use this to cross a bridge.  Only the 1st 4 numbers in the address are
** used; the rest are set to ff's, for broadcasting.  'IsBridge' is set
** if this address should be used.
*/
int IsBridge = 0;
IPXAddressClass BridgeNet;


/***************************************************************************
**	This flag is true if the user has requested that this game be "secret"
** (The game will not appear to other systems just starting up.)
*/
bool NetStealth = false;


/***************************************************************************
**	If this flag is true, the user won't receive messages from any player
** other than those in his own game. It defaults to protected mode.
*/
bool NetProtect = true;


/***************************************************************************
**	This flag indicates whether the game is "open" or not to other network players.
*/
bool NetOpen = false;


/***************************************************************************
**	This string stores the game's network name.
** GameName does not include the "'s Game"; comparing GameName to
** PlayerName can determine if this player is the originator of the game.
*/
char MPlayerGameName[MPLAYER_NAME_MAX];


/***************************************************************************
**	These variables are for servicing the Global Channel.
*/
GlobalPacketType GPacket;				// Global Channel packet
int GPacketlen;							// length of incoming packet
IPXAddressClass GAddress;				// address of sender
unsigned short GProductID;				// sender's Product ID


/***************************************************************************
**	This is the "meta-packet"; it's a bunch of events lumped together.
** The packet's size is IPX's max size (546), rounded down to accommodate
** the max number of events possible.
*/
char *MetaPacket = 0;
int MetaSize = ((546 - sizeof(CommHeaderType)) / sizeof(EventClass) ) * sizeof(EventClass);


/***************************************************************************
**	This is the random-number seed; it's synchronized between systems for
** multiplayer games.
*/
int Seed = 0;
long *RandSeedPtr;


/***************************************************************************
** If this value is non-zero, use it as the random # seed instead; this should
** help reproduce some bugs.
*/
int CustomSeed = 0;

int WindowList[][8] = {
/* xbyte, ypixel, bytewid, pixelht, cursor color, bkgd color,	cursor x, cursor y */

	/* do not change the first 2 entries!! they are necc. to the system */
	{0,0,40,200,WHITE,BLACK,0,0},	/* screen window */
	{1,75,38,100,WHITE,BLACK,0,0},	/* DOS Error window */

	// Tactical map.
	{0, 0, 40, 200, WHITE,LTGREY,0,0},

	// Initial menu window.
	{12, 199-42, 16, 42, LTGREY, DKGREY, 0, 0},

	// Sidebar clipping window.
	{0,0,0,0,0,0,0,0},

	// Scenario editor window.
	{5, 30, 30, 140, 0, 0, 0, 0},

	// Custom window.
	{0, 0, 0, 0, 0, 0, 0, 0},

	// Virtual window for external rendering. ST - 1/15/2019 3:02PM
	{0, 0, 0, 0, 0, 0, 0, 0},
};


/* X,Y,Item Width,Items High,Selected,Norm Color,Sel Color,zero 	*/
int MenuList[][8]={
		{1, 3, 12, 3, 0, WHITE, PINK, 0},
};

GraphicBufferClass		VisiblePage;
GraphicBufferClass		HiddenPage;

GraphicViewPortClass	SeenBuff(&VisiblePage, 0,0,1536,1536);
GraphicBufferClass		ModeXBuff;
GraphicViewPortClass	HidPage(&HiddenPage, 0,0, 1536,1536);
GraphicBufferClass		SysMemPage(DEFAULT_SCREEN_WIDTH, 200, (void*)NULL);
int 						SoundOn;
CountDownTimerClass	FrameTimer(BT_SYSTEM, 0L);
CountDownTimerClass	DebugTimer(BT_SYSTEM, 0L);
CountDownTimerClass	CountDownTimer(BT_SYSTEM, 0L);

NewConfigType NewConfig;

/***************************************************************************
**	This timer measures how long (in ticks) it takes to process the game's
** logic, with no packet processing or artificial delays.
*/
TimerClass ProcessTimer;
int ProcessTicks;				// accumulated ticks
int ProcessFrames;			// # frames used to measure 'ProcessTicks'


/***************************************************************************
**	This flag is for popping up dialogs that call the main loop.
*/
SpecialDialogType SpecialDialog = SDLG_NONE;


/*
** This flags if used to tell can enter cell that we are in a find path
** check and thus should not uncloak units via Can_Enter_Cell.
*/
//bool	IsFindPath = false;


/***************************************************************************
**	Globals for the network Dialogs.
*/

/*
**	List of all games out there, & the address of the game's owner
*/
DynamicVectorClass <NodeNameType *> Games;

/*
**	List of names & addresses of all the players in the game I'm joining.
**	This is the really critical list, since it's used to form connections with
**	all other players in my game.  It's updated when I get a response to my
**	outgoing query, or when I get a query from another system in my game asking
**	who I am.  This double-insurance means that if any system knows about me,
**	I know about them too.  The only catch is that if the game is started very,
**	very soon after a player joins, not everyone may know about him; to prevent
**	this, a timer restriction is put on the New Game dialog's GO button.
*/
DynamicVectorClass <NodeNameType *> Players;

char *DebugFname;	// for stoopid debugging purposes
int DebugLine;		// for stoopid debugging purposes
#ifdef DEMO
int RequiredCD = -2;
#else
int RequiredCD = -1;
#endif
int MouseInstalled;

/*
** Certain options must be enabled by both a command-line option, and an
** an entry in an INI file.  If this flag is 'true', those options have been
** enabled by the INI file.
*/
int AreThingiesEnabled = false;


/*
** Pointer to windows timer object
**
**
*/

WinTimerClass	*WindowsTimer=NULL;


/*
** Command line arguments
**
**
*/
char *				Argv[20];		//Pointers to command line arguments
int					Argc;				//Command line argument count


WWKeyboardClass	Kbd;
int					ScreenWidth=1536;
int					ScreenHeight=1536;
WWMouseClass		*WWMouse = NULL;
HANDLE hInstance;
int AllDone;
BOOL				InMovie = FALSE;	//Are we currently playing a VQ movie?
bool				MMXAvailable = false;	//Does this CPU support MMX extensions?
//GetCDClass		CDList;
bool				GameStatisticsPacketSent;
bool				ConnectionLost;

TheaterType		LastTheater = THEATER_NONE;


bool RunningAsDLL = false;