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

/* $Header: /counterstrike/GLOBALS.CPP 2     3/10/97 6:22p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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

bool IsVQ640 = false;
unsigned long GameVersion = 0;
bool Debug_MotionCapture = false;
bool Debug_Rotate = false;				// Rotation algorithm control.
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

bool Debug_Heap_Dump = false;			// true = print the Heap Dump
bool Debug_Smart_Print = false;		// true = print everything that calls Smart_Printf
bool Debug_Trap_Check_Heap = false;	// true = check the Heap
bool Debug_Modem_Dump = false;		// true = print the Modem Stuff
bool Debug_Print_Events = false;		// true = print event & packet processing
bool Debug_Force_Crash = false;

TFixedIHeapClass<AircraftClass>		Aircraft;
TFixedIHeapClass<AnimClass>			Anims;
TFixedIHeapClass<BuildingClass>		Buildings;
TFixedIHeapClass<BulletClass>			Bullets;
TFixedIHeapClass<FactoryClass>		Factories;
TFixedIHeapClass<HouseClass>			Houses;
TFixedIHeapClass<InfantryClass>		Infantry;
TFixedIHeapClass<OverlayClass>		Overlays;
TFixedIHeapClass<SmudgeClass>			Smudges;
TFixedIHeapClass<TeamClass>			Teams;
TFixedIHeapClass<TeamTypeClass>		TeamTypes;
TFixedIHeapClass<TemplateClass>		Templates;
TFixedIHeapClass<TerrainClass>		Terrains;
TFixedIHeapClass<TriggerClass>		Triggers;
TFixedIHeapClass<UnitClass>			Units;
TFixedIHeapClass<VesselClass>			Vessels;
TFixedIHeapClass<TriggerTypeClass>	TriggerTypes;

TFixedIHeapClass<HouseTypeClass>		HouseTypes;
TFixedIHeapClass<BuildingTypeClass>	BuildingTypes;
TFixedIHeapClass<AircraftTypeClass>	AircraftTypes;
TFixedIHeapClass<InfantryTypeClass>	InfantryTypes;
TFixedIHeapClass<BulletTypeClass>	BulletTypes;
TFixedIHeapClass<AnimTypeClass>		AnimTypes;
TFixedIHeapClass<UnitTypeClass>		UnitTypes;
TFixedIHeapClass<VesselTypeClass>	VesselTypes;
TFixedIHeapClass<TemplateTypeClass>	TemplateTypes;
TFixedIHeapClass<TerrainTypeClass>	TerrainTypes;
TFixedIHeapClass<OverlayTypeClass>	OverlayTypes;
TFixedIHeapClass<SmudgeTypeClass>	SmudgeTypes;


/*
**	These are the instantiate static heap pointers for the various
**	CCPtr class objects that are allowed to exist. If the linker generates
**	an error about a missing heap pointer, then this indicates that CCPtr objects
**	for that type are not allowed. For every case of a TFixedIHeap manager of
**	game objects, then a CCPtr can be instantiated for it.
*/
#if (0)	//Moved to runtime initialization. ST - 5/20/2019
template<> FixedIHeapClass * CCPtr<AircraftClass>::Heap = &Aircraft;
template<> FixedIHeapClass * CCPtr<AnimClass>::Heap = &Anims;
template<> FixedIHeapClass * CCPtr<BuildingClass>::Heap = &Buildings;
template<> FixedIHeapClass * CCPtr<BulletClass>::Heap = &Bullets;
template<> FixedIHeapClass * CCPtr<FactoryClass>::Heap = &Factories;
template<> FixedIHeapClass * CCPtr<HouseClass>::Heap = &Houses;
template<> FixedIHeapClass * CCPtr<InfantryClass>::Heap = &Infantry;
template<> FixedIHeapClass * CCPtr<OverlayClass>::Heap = &Overlays;
template<> FixedIHeapClass * CCPtr<SmudgeClass>::Heap = &Smudges;
template<> FixedIHeapClass * CCPtr<TeamClass>::Heap = &Teams;
template<> FixedIHeapClass * CCPtr<TeamTypeClass>::Heap = &TeamTypes;
template<> FixedIHeapClass * CCPtr<TemplateClass>::Heap = &Templates;
template<> FixedIHeapClass * CCPtr<TerrainClass>::Heap = &Terrains;
template<> FixedIHeapClass * CCPtr<TriggerClass>::Heap = &Triggers;
template<> FixedIHeapClass * CCPtr<TriggerTypeClass>::Heap = &TriggerTypes;

template<> FixedIHeapClass * CCPtr<HouseTypeClass>::Heap = &HouseTypes;
template<> FixedIHeapClass * CCPtr<BuildingTypeClass>::Heap = &BuildingTypes;
template<> FixedIHeapClass * CCPtr<AircraftTypeClass>::Heap = &AircraftTypes;
template<> FixedIHeapClass * CCPtr<InfantryTypeClass>::Heap = &InfantryTypes;
template<> FixedIHeapClass * CCPtr<BulletTypeClass>::Heap = &BulletTypes;
template<> FixedIHeapClass * CCPtr<AnimTypeClass>::Heap = &AnimTypes;
template<> FixedIHeapClass * CCPtr<UnitTypeClass>::Heap = &UnitTypes;
template<> FixedIHeapClass * CCPtr<VesselTypeClass>::Heap = &VesselTypes;
template<> FixedIHeapClass * CCPtr<TemplateTypeClass>::Heap = &TemplateTypes;
template<> FixedIHeapClass * CCPtr<TerrainTypeClass>::Heap = &TerrainTypes;
template<> FixedIHeapClass * CCPtr<OverlayTypeClass>::Heap = &OverlayTypes;
template<> FixedIHeapClass * CCPtr<SmudgeTypeClass>::Heap = &SmudgeTypes;
#endif

/* These variables are used to keep track of the slowest speed of a team */
MPHType TeamMaxSpeed[MAX_TEAMS];
SpeedType TeamSpeed[MAX_TEAMS];
bool FormMove;
SpeedType FormSpeed;
MPHType FormMaxSpeed;


char _staging_buffer[32000];

/*
** Global flag for the life of Tanya.  If this flag is set, she is
** no longer available.
*/
bool IsTanyaDead;
bool SaveTanya;

#ifdef FIXIT_ANTS
bool AntsEnabled = false;
#endif

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
bool NewUnitsEnabled = false;
bool SecretUnitsEnabled = false;
int MTankDistance = 15;
bool OverrideNewUnitsEnabled = false;	 // ST - 12/13/2019 12:21PM	
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
int CarrierLaunchDelay = 60;
#endif
#endif

int NewINIFormat = 0;

bool TimeQuake;

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
bool PendingTimeQuake;
TARGET TimeQuakeCenter;
fixed QuakeUnitDamage=0x300;
fixed QuakeBuildingDamage=0x300;
int QuakeInfantryDamage=25;
int QuakeDelay;
fixed ChronoTankDuration=0x300;	// chrono override for chrono tanks
#ifdef FIXIT_ENGINEER	//	checked - ajw 9/28/98
fixed EngineerDamage=0x55;			// Amount of damage an engineer does
fixed EngineerCaptureLevel=0x40;	// Building damage level before engineer can capture
#endif
#endif

#ifdef WIN32
unsigned short			Hard_Error_Occured = 0;
WWMouseClass *			WWMouse = NULL;
GraphicBufferClass	SysMemPage(DEFAULT_SCREEN_WIDTH, 200, (void*)NULL);
WinTimerClass *		WindowsTimer=NULL;
int						ScreenWidth=3072;
int						ScreenHeight=3072;
GraphicBufferClass	ModeXBuff;
bool						InMovie = FALSE;	//Are we currently playing a VQ movie?
HANDLE					hInstance;
int						AllDone;


/***************************************************************************
**	This is true if the game is the currently in focus windows app
**
*/
bool GameInFocus = false;

#endif


/***************************************************************************
**	Encryption keys.
*/
PKey FastKey;
#ifdef CHEAT_KEYS
PKey SlowKey;
#endif

#ifdef FIXIT_NAME_OVERRIDE
/***************************************************************************
**	This is where the name overrides for the units will reside.
*/
char const * NameOverride[25];
int NameIDOverride[25];
#endif

/***************************************************************************
**	These are the mission control structures. They hold the information about
**	how the missions should behave in the system.
*/
MissionControlClass MissionControl[MISSION_COUNT];


/***************************************************************************
**	There are various tutorial messages that can appear in the game. These
**	are called upon by number and pointed to by this array.
*/
char const * TutorialText[225];


/***************************************************************************
**	This holds the rules database. The rules database won't change during the
**	program's run, but may need to be referenced intermitently.
*/
CCINIClass RuleINI;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
CCINIClass AftermathINI;
#endif

/***************************************************************************
**	This points to the benchmark objects that are allocated only if the
**	machine is running on a Pentium and this is a debug version.
*/
//Benchmark * Benches;


/***************************************************************************
**	General rules that control the game.
*/
RulesClass Rule;


/***************************************************************************
** All keyboard input is routed through the object pointed to by this
**	keyboard class pointer.
*/
KeyboardClass * Keyboard;


/***************************************************************************
**	Remap control array. This is used to hold the remap
**	tables for the various possible player colors, and the color schemes
** for dialogs.
*/
RemapControlType ColorRemaps[PCOLOR_COUNT];

/*
** Special remap scheme for font that hs to print over metallic tabs
*/
RemapControlType MetalScheme;

/*
** This remap table is for special purposes.  It consists of dark grey shades,
** and is used for dimming things out.
*/
RemapControlType GreyScheme;


/***************************************************************************
**	This is the source of the random numbers used in the game. This controls
**	the game logic and thus must be in sync with any networked machines.
*/
RandomClass NonCriticalRandomNumber;
RandomStraw CryptRandom;


/***************************************************************************
**	This tracks all selected objects per house (for this map).
*/
SelectedObjectsType CurrentObject;


/***************************************************************************
**	This is the game version.
*/
//PG VersionClass VerNum;


/***************************************************************************
**	This is the VQ animation controller structure. It is filled in by reading
**	the PLAYER.INI and overridden through program control.
*/
//VQAConfig AnimControl;

int PreserveVQAScreen;			// Used for screen mode transition control.
bool BreakoutAllowed = true;	// "true" if aborting of movies is allowed.
bool Brokeout;						// Was the movie broken out of?
bool SlowPalette = false;		// Slow palette flag set?


/***************************************************************************
**	These are the movie names to use for mission briefing, winning, and losing
**	sequences. They are read from the INI file.
*/
ScenarioClass Scen;


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
**	By carefully controlling this global, multiple responses are suppressed
**	when a large group of infantry is given the movement order.
*/
bool AllowVoice = true;


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
void * SpeechBuffer[2];
VoxType SpeechRecord[2];


/***************************************************************************
**	The theater specific mixfiles are cached into the buffer pointed to by
**	this global.
*/
Buffer * TheaterBuffer;


/***************************************************************************
**	This is a running accumulation of the number of ticks that were unused.
** This accumulates into a useful value that contributes to a
**	histogram of game performance.
*/
long SpareTicks;
long PathCount;			// Number of findpaths called.
long CellCount;			// Number of cells redrawn.
long TargetScan;			// Number of target scans.
long SidebarRedraws;		// Number of sidebar redraws.


/***************************************************************************
**	This is the monochrome debug page array. The various monochrome data
**	screens are located here.
*/
MonoClass MonoArray[DMONO_COUNT];
DMonoType MonoPage = DMONO_STRESS;	// The current page.


/***************************************************************************
**	This holds the theater specific mixfiles.
*/
MFCD * TheaterData = 0;
MFCD * MoviesMix = 0;
MFCD * GeneralMix = 0;
MFCD * ScoreMix = 0;
MFCD * MainMix = 0;
MFCD * ConquerMix = 0;


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


/**************************************************************************
** This class records the special command override options that C&C
**	supports.
*/
SpecialClass Special;


bool PassedProximity;	// used in display.cpp


/***************************************************************************
**	This is the scenario data for the currently loaded scenario.
** These variables should all be set together.
*/
HousesType Whom;							// Initial command line house choice.
int ScenarioInit;
bool SpecialFlag = false;


/***************************************************************************
** This value tells the sidebar what items it's allowed to add.  The
** lower the value, the simpler the sidebar will be. This value is the
**	displayed value for tech level in the multiplay dialogs. It remaps to
**	the in-game rules.ini tech levels.
*/
int BuildLevel = 10;				// Buildable level (1 = simplest)


/***************************************************************************
**	The various tutor and dialog messages are located in the data block
**	referenced by this pointer.
*/
char const * SystemStrings;
char const * DebugStrings;


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
void const * Metal12FontPtr;	//Font for use on in-game tabs in hires
void const * MapFontPtr;		// Standard very small font.
void const * TypeFontPtr;		// Teletype font for mission briefings.
void const * Font3Ptr;			// Standard very small font.
void const * Font6Ptr;			// Standard small font.
void const * EditorFont;		// Font used for scenario editor.
void const * Font8Ptr;			// 8 point proportional.
void const * FontLEDPtr;		// LED fixed point font.
void const * VCRFontPtr;		// VCR font pointer.
void const * ScoreFontPtr;		// font for score & map selection screens
void const * GradFont6Ptr;		// gradient 6 point font pointer.


/***************************************************************************
**	This is the house that the human player is currently playing.
*/
HouseClass * PlayerPtr;


/***************************************************************************
**	Special palettes for MCGA mode goes here. These palette buffers are used
**	for pictures that do not use the game palette or are used for fading to
**	black.
*/
PaletteClass CCPalette;
PaletteClass GamePalette;
//PaletteClass InGamePalette;
PaletteClass BlackPalette(RGBClass(0, 0, 0));
PaletteClass WhitePalette(RGBClass(RGBClass::MAX_VALUE, RGBClass::MAX_VALUE, RGBClass::MAX_VALUE));
PaletteClass OriginalPalette;
PaletteClass ScorePalette;


/***************************************************************************
**	These are the event queues. One is for holding events until they are ready to be
**	sent to the remote computer for processing. The other list is for incoming events
**	that need to be executed when the correct frame has been reached.
*/
QueueClass<EventClass, MAX_EVENTS> OutList;
QueueClass<EventClass, (MAX_EVENTS * 64)> DoList;

#ifdef MIRROR_QUEUE
QueueClass<EventClass, (MAX_EVENTS * 64)> MirrorList;
#endif


/***************************************************************************
**	These are arrays/lists of trigger pointers for each cell & the houses.
*/
DynamicVectorClass<TriggerClass *> HouseTriggers[HOUSE_COUNT];
DynamicVectorClass<TriggerClass *> MapTriggers;
int MapTriggerID;
DynamicVectorClass<TriggerClass *> LogicTriggers;
int LogicTriggerID;


/***************************************************************************
**	This is the list of BuildingTypes that define the AI's base.
*/
BaseClass Base;


/***************************************************************************
**	This is the list of carry over objects. These objects are part of the
**	pseudo saved game that might be carried along with the current saved
**	game.
*/
CarryoverClass * Carryover;


/***************************************************************************
** This value is computed every time a new scenario is loaded; it's a
** CRC of the INI and binary map files.
*/
unsigned long ScenarioCRC;


/***************************************************************************
** This class manages data specific to multiplayer games.
*/
SessionClass Session;
#if(TIMING_FIX)
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
#endif

#ifdef FIXIT_VERSION_3
bool	bAftermathMultiplayer;		//	Is multiplayer game being played with Aftermath rules?
#else
unsigned long 	PlayingAgainstVersion;		// Negotiated version number
bool				Version107InMix;				// Is there a v1.07 in the game
#endif

/***************************************************************************
**	This is the null modem manager class.  Declaring this class doesn't
** perform any allocations;
*/
#if (0)
NullModemClass NullModem (
   16,											// number of send entries
	16,											// number of receive entries
	(MAX_SERIAL_PACKET_SIZE / sizeof(EventClass) ) * sizeof(EventClass) + sizeof( CommHeaderType ),
	0x1234);										// Magic number must have each digit unique
													// and different from the queue magic number
#endif


/***************************************************************************
**	This is the network IPX manager class.  It handles multiple remote
** connections.  Declaring this class doesn't perform any allocations;
** the class itself is 140 bytes.
*/
//IPXManagerClass Ipx (
//	MAX (sizeof (GlobalPacketType), sizeof(RemoteFileTransferType)),		// size of Global Channel packets
//	((546 - sizeof(CommHeaderType)) / sizeof(EventClass) ) * sizeof(EventClass),
//	10, 													// # entries in Global Queue
//	8, 													// # entries in Private Queues
//	VIRGIN_SOCKET, 									// Socket ID #
//	IPXGlobalConnClass::COMMAND_AND_CONQUER0);// Product ID #

IPXManagerClass Ipx (
	MAX (sizeof (GlobalPacketType), sizeof(RemoteFileTransferType)),		// size of Global Channel packets
	((546 - sizeof(CommHeaderType)) / sizeof(EventClass) ) * sizeof(EventClass),
	160, 													// # entries in Global Queue
	32, 													// # entries in Private Queues
	VIRGIN_SOCKET, 									// Socket ID #
	IPXGlobalConnClass::COMMAND_AND_CONQUER0);// Product ID #


#if(TEN)
/***************************************************************************
** This is the connection manager for Ten.  Special Ten notes:
** - TEN connection ID's are equal to the HousesType for that player.
** - The TEN internal player ID is used to determine the player's color.
** - Ten's broadcast destination address -1
*/
TenConnManClass *Ten = NULL;

#endif


#if(MPATH)
/***************************************************************************
** This is the connection manager for Ten.  Special Ten notes:
** - MPATH connection ID's are equal to the HousesType for that player.
** - The player's color is read from the OPTIONS.INI file
** - MPath's broadcast destination address is 0
*/
MPlayerManClass *MPath = NULL;

#endif


/***************************************************************************
**	This is the random-number seed; it's synchronized between systems for
** multiplayer games.
*/
int Seed = 0;


/***************************************************************************
** If this value is non-zero, use it as the random # seed instead; this should
** help reproduce some bugs.
*/
int CustomSeed = 0;

int WindowList[][9] = {
/* xbyte, ypixel, bytewid, pixelht, cursor color, bkgd color,	cursor x, cursor y */

	/* do not change the first 2 entries!! they are necc. to the system */

	{0,0,40*8*RESFACTOR,200*RESFACTOR,WHITE,BLACK,0,0},	/* screen window */
	{1*8,75,38*8,100,WHITE,BLACK,0,0},	/* DOS Error window */

	// Tactical map.
	{0, 0, 40*8*RESFACTOR, 200*RESFACTOR, WHITE,LTGREY,0,0},

	// Initial menu window.
	{12*8, 199-42, 16*8, 42, LTGREY, DKGREY, 0, 0},

	// Sidebar clipping window.
	{0,0,0,0,0,0,0,0},

	// Scenario editor window.
	{5*8, 30, 30*8, 140, 0, 0, 0, 0},

	// Partial object draw sub-window.
	{0,0,0,0,WHITE,BLACK,0,0},

	// Custom window.
	{0, 0, 0, 0, 0, 0, 0, 0},

	// Virtual window for external rendering. ST - 1/15/2019 3:02PM
	{0, 0, 0, 0, 0, 0, 0, 0}

};


/* X,Y,Item Width,Items High,Selected,Norm Color,Sel Color,zero 	*/
int MenuList[][8]={
		{1, 3, 12, 3, 0, WHITE, PINK, 0},
};


#ifdef WIN32
GraphicBufferClass	VisiblePage;
GraphicBufferClass	HiddenPage;
GraphicViewPortClass	SeenBuff(&VisiblePage, 0, 0, 3072, 3072);
GraphicViewPortClass	HidPage(&HiddenPage, 0, 0, 3072, 3072);
#else
GraphicBufferClass	HidPage(DEFAULT_SCREEN_WIDTH, 201, (void*)NULL);
GraphicBufferClass	SeenBuff(320, 200, (void *)0xA0000L);
VideoBufferClass		SeenPage;
GraphicBufferClass & VisiblePage = SeenBuff;
#endif


#ifdef WIN32
#else
#endif

int 						SoundOn;
CDTimerClass<SystemTimerClass> FrameTimer;
CDTimerClass<SystemTimerClass> CountDownTimer;

NewConfigType NewConfig;
TheaterType LastTheater = THEATER_NONE;	//Lets us know when theater type changes.


/***************************************************************************
**	This flag is for popping up dialogs that call the main loop.
*/
SpecialDialogType SpecialDialog = SDLG_NONE;


int RequiredCD = -1;
int CurrentCD = -1;
int MouseInstalled;

//
// Variables for helping track how much time goes bye in routines
//
int LogLevel = 0;
unsigned long LogLevelTime[ MAX_LOG_LEVEL ] = { 0 };
unsigned long LogLastTime = 0;
bool LogDump_Print = false;		// true = print the Log time Stuff


/***************************************************************************
** Tick Count global timer object.
*/
TTimerClass<SystemTimerClass> TickCount = 0;


/***************************************************************************
**  Win32 specific globals
*/
#ifdef WIN32

bool InDebugger = false;
int ReadyToQuit = 0;

#else
bool IsTheaterShape = false;	// must be defined only if not Win32
#endif	//WIN32

//PG GetCDClass		CDList;
int UnitBuildPenalty = 100;

#ifdef MPEGMOVIE // Denzil 6/15/98
#ifdef MCIMPEG
#include "mcimovie.h"
MCIMovie* MciMovie = NULL;
#endif

#include "mpgset.h"
MPGSettings* MpgSettings = NULL;
#endif

#ifdef FIXIT_VERSION_3			//	For endgame auto-sonar pulse.
bool bAutoSonarPulse = false;
#endif

bool MMXAvailable = false;


// ST - 5/14/2019
bool RunningAsDLL = false;
bool RunningFromEditor = false;
