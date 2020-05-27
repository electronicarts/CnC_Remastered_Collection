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

/* $Header:   F:\projects\c&c\vcs\code\externs.h_v   2.15   16 Oct 1995 16:45:34   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : EXTERNS.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 27, 1994                                                 *
 *                                                                                             *
 *                  Last Update : May 27, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef EXTERNS_H
#define EXTERNS_H

#include	"cell.h"

#ifdef SCENARIO_EDITOR
#include	"mapedit.h"
#endif
#include	"techno.h"
#include	"type.h"
#include	"building.h"
#include	"unit.h"
#include	"credits.h"
#include	"goptions.h"
#include	"options.h"
#include	"infantry.H"

#ifdef JAPANESE
extern bool ForceEnglish;
#endif

extern bool Debug_Quiet;
extern bool Debug_Cheat;
extern bool Debug_Remap;
extern bool Debug_Flag;
extern bool Debug_Lose;
extern bool Debug_Map;
extern bool Debug_Win;
extern bool Debug_Icon;
extern bool Debug_Passable;
extern bool Debug_Unshroud;
extern bool Debug_Threat;
extern bool Debug_Find_Path;
extern bool Debug_Check_Map;
extern bool Debug_Playtest;

extern bool Debug_Heap_Dump;
extern bool Debug_Smart_Print;
extern bool Debug_Trap_Check_Heap;
extern bool Debug_Instant_Build;
extern bool Debug_Force_Crash;

extern void const *WarFactoryOverlay;


/*
**	Dynamic global variables (these change or are initialized at run time).
*/
#ifdef PATCH
extern bool							IsV107;
extern char							OverridePath[128];
#endif
extern bool							SlowPalette;
extern char							VersionText[16];
extern bool							ScoresPresent;
extern int							CrateCount;
extern TCountDownTimerClass	CrateTimer;
extern bool							CrateMaker;
extern ThemeType					TransitTheme;
extern bool							AllowVoice;
extern NewConfigType				NewConfig;
extern char							BriefingText[512];
extern char 						IntroMovie[_MAX_FNAME+_MAX_EXT];
extern char 						ActionMovie[_MAX_FNAME+_MAX_EXT];
extern char 						BriefMovie[_MAX_FNAME+_MAX_EXT];
extern char 						WinMovie[_MAX_FNAME+_MAX_EXT];
extern char 						WinMovie2[_MAX_FNAME + _MAX_EXT];
extern char 						WinMovie3[_MAX_FNAME + _MAX_EXT];
extern char 						WinMovie4[_MAX_FNAME + _MAX_EXT];
extern char 						LoseMovie[_MAX_FNAME+_MAX_EXT];
extern char							MovieThemeName[_MAX_FNAME + _MAX_EXT];
extern VoxType						SpeakQueue;
extern bool							PlayerWins;
extern bool							PlayerLoses;
extern bool							PlayerRestarts;
extern StructType					SabotagedType;
extern bool							TempleIoned;
extern long							Frame;
extern void *						SpeechBuffer;
extern int							PreserveVQAScreen;
extern bool							BreakoutAllowed;
extern bool							Brokeout;
extern CELL							Views[4];

extern GameOptionsClass 		Options;

extern LogicClass 				Logic;
#ifdef SCENARIO_EDITOR
extern MapEditClass 				Map;
#else
extern MouseClass 				Map;
#endif
extern ScoreClass 				Score;
extern MonoClass 					MonoArray[MonoClass::MAX_MONO_PAGES];
extern MixFileClass *			ScoreMix;
extern MixFileClass *			TheaterData;
extern MixFileClass *			LowTheaterData;
extern MixFileClass *			MoviesMix;
extern MixFileClass *			GeneralMix;
extern ThemeClass 				Theme;
extern SpecialClass 				Special;
extern RulesClass				Rule;

/*
**	Game object allocation and tracking classes.
*/
extern TFixedIHeapClass<UnitClass>							Units;
extern TFixedIHeapClass<FactoryClass>						Factories;
extern TFixedIHeapClass<TerrainClass>						Terrains;
extern TFixedIHeapClass<TemplateClass>						Templates;
extern TFixedIHeapClass<SmudgeClass>						Smudges;
extern TFixedIHeapClass<OverlayClass>						Overlays;
extern TFixedIHeapClass<InfantryClass>						Infantry;
extern TFixedIHeapClass<BulletClass>						Bullets;
extern TFixedIHeapClass<BuildingClass>						Buildings;
extern TFixedIHeapClass<AnimClass>							Anims;
extern TFixedIHeapClass<AircraftClass>						Aircraft;
extern TFixedIHeapClass<TriggerClass>						Triggers;
extern TFixedIHeapClass<TeamTypeClass>						TeamTypes;
extern TFixedIHeapClass<TeamClass>							Teams;
extern TFixedIHeapClass<HouseClass>							Houses;

extern QueueClass<EventClass, MAX_EVENTS>					OutList;
extern QueueClass<EventClass, (MAX_EVENTS * 8)>			DoList;

typedef DynamicVectorArrayClass<ObjectClass *, HOUSE_COUNT, HOUSE_FIRST> SelectedObjectsType;
extern SelectedObjectsType								CurrentObject;
extern DynamicVectorClass<TriggerClass *> 				CellTriggers;
extern DynamicVectorClass<TriggerClass *> 				HouseTriggers[HOUSE_COUNT];

extern CELL															Waypoint[WAYPT_COUNT];

extern BaseClass													Base;

/*
**	Loaded data file pointers.
*/
extern void const *	Green12FontPtr;
extern void const *	Green12GradFontPtr;
extern void const *	MapFontPtr;
extern void const *	VCRFontPtr;
extern void const *	Font3Ptr;
extern void const *	Font6Ptr;
extern void const *	Font8Ptr;
extern void const *	FontLEDPtr;
extern void const *	ScoreFontPtr;
extern void const *	GradFont6Ptr;
extern char const *	SystemStrings;

/*
**	Miscellaneous globals.
*/
extern HousesType				Whom;
//extern _VQAConfig				AnimControl;
extern long						SpareTicks;
extern int						MonoPage;
extern unsigned char *		OriginalPalette;
extern int						EndCountDown;
extern bool						GameActive;
extern bool						SpecialFlag;
extern int						ScenarioInit;
extern long						TutorFlags[2];
extern HouseClass *			PlayerPtr;
extern unsigned char *		BlackPalette;
extern unsigned char *		WhitePalette;
extern unsigned char *		GamePalette;
extern unsigned 				Scenario;
extern ScenarioPlayerType 	ScenPlayer;
extern ScenarioDirType 		ScenDir;
extern ScenarioVarType 		ScenVar;
extern int						CarryOverMoney;
extern int						CarryOverCap;
extern int						CarryOverPercent;
extern char 						ScenarioName[_MAX_FNAME+_MAX_EXT];
extern unsigned					BuildLevel;
extern unsigned long			ScenarioCRC;

#ifdef SCENARIO_EDITOR
extern CELL 						CurrentCell;
#endif

extern GameType					GameToPlay;

extern CommProtocolType		CommProtocol;

extern CCFileClass 			RecordFile;
extern int						RecordGame;
extern int						SuperRecord;
extern int						PlaybackGame;
extern int						AllowAttract;

extern GetCDClass				CDList;

/*
** Modem globals
*/
extern bool 											ModemService;
//extern NullModemClass 								NullModem;
//extern DynamicVectorClass<PhoneEntryClass *>	PhoneBook;
extern int												CurPhoneIdx;
extern DynamicVectorClass <char *>				InitStrings;
extern SerialSettingsType							SerialDefaults;
extern ModemGameType									ModemGameToPlay;
extern char *											DialMethodCheck[ DIAL_METHODS ];
extern char *											CallWaitStrings[ CALL_WAIT_STRINGS_NUM ];

/*
** Network/Modem globals
*/
extern int										ScenarioIdx;
extern int										ColorUsed[];
extern char										MPlayerName[MPLAYER_NAME_MAX];
extern int										MPlayerGColors[];
extern int										MPlayerTColors[];
extern char 									MPlayerDescriptions[100][40];
extern DynamicVectorClass <char *>		MPlayerScenarios;
extern DynamicVectorClass <int>			MPlayerFilenum;
extern int										MPlayerMax;
extern int 										MPlayerPrefColor;
extern int 										MPlayerColorIdx;
extern HousesType								MPlayerHouse;
extern unsigned char							MPlayerLocalID;
extern int										MPlayerCount;
extern int										MPlayerBases;
extern int										MPlayerCredits;
extern int										MPlayerTiberium;
extern int										MPlayerGoodies;
extern int										MPlayerGhosts;
extern int										MPlayerSolo;
extern int										MPlayerUnitCount;
extern int										MPlayerCountMin[2];
extern int										MPlayerCountMax[2];
extern unsigned long							MPlayerMaxAhead;
extern unsigned long							FrameSendRate;
extern unsigned char							MPlayerID[MAX_PLAYERS];
extern HousesType								MPlayerHouses[MAX_PLAYERS];
extern char										MPlayerNames [MAX_PLAYERS][MPLAYER_NAME_MAX];
extern MessageListClass						Messages;
extern IPXAddressClass						MessageAddress;
extern char										LastMessage[MAX_MESSAGE_LENGTH];
extern int										MPlayerBlitz;
extern int										MPlayerObiWan;
extern MPlayerScoreType						MPlayerScore[MAX_MULTI_NAMES];
extern int										MPlayerGamesPlayed;
extern int										MPlayerNumScores;
extern int										MPlayerWinner;
extern int										MPlayerCurGame;

extern int										TheirProcessTime[MAX_PLAYERS - 1];
extern int										DesiredFrameRate;

extern char *									GlobalPacketNames[];
extern char *									SerialPacketNames[];

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

extern long										TrapFrame;
extern RTTIType								TrapObjType;
extern TrapObjectType						TrapObject;
extern COORDINATE									TrapCoord;
extern void *									TrapThis;
extern CellClass *							TrapCell;
extern int										TrapCheckHeap;

/*
** Network (IPX) globals
*/
extern IPXManagerClass 	 					Ipx;
extern int										IsBridge;
extern IPXAddressClass						BridgeNet;
extern bool 									NetMaster;
extern bool 									NetStealth;
extern bool 									NetProtect;
extern bool 									NetOpen;
extern char										MPlayerGameName[MPLAYER_NAME_MAX];
extern GlobalPacketType						GPacket;
extern int										GPacketlen;
extern IPXAddressClass						GAddress;
extern unsigned short						GProductID;
extern char *									MetaPacket;
extern int										MetaSize;
extern DynamicVectorClass <NodeNameType *> Games;
extern DynamicVectorClass <NodeNameType *> Players;

extern int										Seed;
extern long *									RandSeedPtr;
extern int										CustomSeed;
extern int										NewMaxAheadFrame1;
extern int										NewMaxAheadFrame2;

/*
**	Constant externs (data is not modified during game play).
*/
extern unsigned char const					RemapGreen[256];
extern unsigned char const					RemapBlue[256];
extern unsigned char const					RemapOrange[256];
extern unsigned char const					RemapNone[256];
extern unsigned char const					RemapGold[256];
extern unsigned char const					RemapRed[256];
extern unsigned char const					RemapLtBlue[256];
extern WeaponTypeClass const				Weapons[WEAPON_COUNT];
extern WarheadTypeClass const				Warheads[WARHEAD_COUNT];
extern char const *							SourceName[SOURCE_COUNT];
extern GroundType const 					Ground[LAND_COUNT];
extern TheaterDataType const 				Theaters[THEATER_COUNT];
extern unsigned char const 				Facing32[256];
extern unsigned char const 				Facing8[256];
extern unsigned char const					Pixel2Lepton[24];
extern COORDINATE const 							StoppingCoordAbs[5];
extern CELL const 							AdjacentCell[FACING_COUNT];
extern COORDINATE const 							AdjacentCoord[FACING_COUNT];

extern int SoundOn;
//extern GraphicBufferClass	SeenPage;
extern GraphicBufferClass	VisiblePage;
extern GraphicBufferClass	HiddenPage;
extern GraphicViewPortClass	SeenBuff;
extern GraphicBufferClass	ModeXBuff;
extern GraphicViewPortClass	HidPage;
extern GraphicBufferClass	LoResHidPage;
extern GraphicBufferClass	SysMemPage;
extern int 						MenuList[][8];
extern CountDownTimerClass FrameTimer;
extern CountDownTimerClass	CountDownTimer;

extern							TimerClass ProcessTimer;
extern int						ProcessTicks;
extern int						ProcessFrames;

extern SpecialDialogType	SpecialDialog;
//extern bool						IsFindPath;

extern char *DebugFname;	// for stoopid debugging purposes
extern int DebugLine;		// for stoopid debugging purposes
extern int RequiredCD;
extern int MouseInstalled;
extern int AreThingiesEnabled;

extern WWKeyboardClass	Kbd;
extern int In_Debugger;
extern WWMouseClass	*WWMouse;
extern HANDLE hInstance;
extern int AllDone;
extern "C" bool MMXAvailable;
extern int Get_CD_Index (int cd_drive, int timeout);
void Memory_Error_Handler(void);
extern bool GameStatisticsPacketSent;
extern bool ConnectionLost;
extern bool InMainLoop;			         // True if in game state rather than menu state
void CCDebugString (char *string);
extern void *PacketLater;
void Load_Title_Screen(char *name, GraphicViewPortClass *video_page, unsigned char *palette);

extern "C"{
	extern bool IsTheaterShape;
}

extern void Reset_Theater_Shapes(void);
extern TheaterType LastTheater;

extern bool ShareAllyVisibility;

#endif