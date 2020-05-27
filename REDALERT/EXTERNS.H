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

/* $Header: /counterstrike/EXTERNS.H 2     3/10/97 6:23p Steve_tall $ */
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
#include "DSOUND.H"

extern char _staging_buffer[32000];
extern "C" {
void _PRO(void);
}

/*
**	Convenient alias for MixFileClass<CDFileClass> object. This allows
**	easier entry into the code and less clutter.
*/
typedef MixFileClass<CCFileClass>	MFCD;

extern bool IsVQ640;
extern unsigned long GameVersion;
extern bool Debug_MotionCapture;
extern bool Debug_Rotate;
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
extern bool Debug_Modem_Dump;
extern bool Debug_Print_Events;
extern bool Debug_Force_Crash;

extern void const *LightningShapes;

extern int NewINIFormat;


#ifdef FIXIT_ANTS
extern bool AntsEnabled;
#endif

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
extern bool NewUnitsEnabled;
extern bool SecretUnitsEnabled;
extern int MTankDistance;
extern bool OverrideNewUnitsEnabled;	 // ST - 12/13/2019 12:19PM	
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
extern int CarrierLaunchDelay;
#endif
#endif

#ifdef FIXIT_NAME_OVERRIDE
extern char const * NameOverride[25];
extern int NameIDOverride[25];
#endif

#ifdef WIN32
extern bool					GameInFocus;
extern unsigned char 	*InterpolatedPalettes[100];
extern BOOL					PalettesRead;
extern unsigned			PaletteCounter;
extern int 					AllDone;
extern HANDLE 				hInstance;
extern bool 				InMovie;
extern WinTimerClass	*		WindowsTimer;
extern WWMouseClass *		WWMouse;
extern GraphicBufferClass	HiddenPage;
#define	SeenPage		SeenBuff
extern GraphicBufferClass		VisiblePage;
extern GraphicViewPortClass	SeenBuff;
extern GraphicBufferClass		SysMemPage;
extern LPDIRECTSOUND				SoundObject;
extern LPDIRECTSOUNDBUFFER		PrimaryBufferPtr;
extern int							ScreenWidth;
extern int							ScreenHeight;
extern GraphicBufferClass		ModeXBuff;

#else

extern VideoBufferClass		SeenPage;
extern GraphicBufferClass	SeenBuff;
extern GraphicBufferClass &	VisiblePage;
#endif


/*
**	Dynamic global variables (these change or are initialized at run time).
*/
extern MissionControlClass		MissionControl[MISSION_COUNT];
extern char const *				TutorialText[225];
extern Buffer *					TheaterBuffer;
extern GetCDClass					CDList;
extern CCINIClass					RuleINI;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
extern CCINIClass					AftermathINI;
#endif
//extern Benchmark *				Benches;
extern int							MapTriggerID;
extern int							LogicTriggerID;
extern PKey							FastKey;
extern PKey							SlowKey;
extern RulesClass					Rule;
extern KeyboardClass *			Keyboard;
extern RandomStraw				CryptRandom;
extern RandomClass				NonCriticalRandomNumber;
extern CarryoverClass *			Carryover;
extern ScenarioClass				Scen;
extern RemapControlType			ColorRemaps[PCOLOR_COUNT];
extern RemapControlType			MetalScheme;
extern RemapControlType			GreyScheme;
extern VersionClass				VerNum;
extern bool							SlowPalette;
extern bool							ScoresPresent;
extern bool							AllowVoice;
extern NewConfigType				NewConfig;
extern VoxType						SpeakQueue;
extern bool							PlayerWins;
extern bool							PlayerLoses;
extern bool							PlayerRestarts;
extern long							Frame;
extern VoxType						SpeechRecord[2];
extern void *						SpeechBuffer[2];
extern int							PreserveVQAScreen;
extern bool							BreakoutAllowed;
extern bool							Brokeout;

extern GameOptionsClass 		Options;

extern LogicClass 				Logic;
#ifdef SCENARIO_EDITOR
extern MapEditClass 				Map;
#else
extern MouseClass 				Map;
#endif
extern ScoreClass 				Score;
extern MonoClass 					MonoArray[DMONO_COUNT];
extern MFCD *						TheaterData;
extern MFCD *						MoviesMix;
extern MFCD *						GeneralMix;
extern MFCD *						ScoreMix;
extern MFCD *						MainMix;
extern MFCD *						ConquerMix;
extern ThemeClass 				Theme;
extern SpecialClass 				Special;

/*
**	Game object allocation and tracking classes.
*/
extern TFixedIHeapClass<AircraftClass>						Aircraft;
extern TFixedIHeapClass<AnimClass>							Anims;
extern TFixedIHeapClass<BuildingClass>						Buildings;
extern TFixedIHeapClass<BulletClass>						Bullets;
extern TFixedIHeapClass<FactoryClass>						Factories;
extern TFixedIHeapClass<HouseClass>							Houses;
extern TFixedIHeapClass<InfantryClass>						Infantry;
extern TFixedIHeapClass<OverlayClass>						Overlays;
extern TFixedIHeapClass<SmudgeClass>						Smudges;
extern TFixedIHeapClass<TeamClass>							Teams;
extern TFixedIHeapClass<TeamTypeClass>						TeamTypes;
extern TFixedIHeapClass<TemplateClass>						Templates;
extern TFixedIHeapClass<TerrainClass>						Terrains;
extern TFixedIHeapClass<TriggerClass>						Triggers;
extern TFixedIHeapClass<UnitClass>							Units;
extern TFixedIHeapClass<VesselClass>						Vessels;
extern TFixedIHeapClass<TriggerTypeClass>					TriggerTypes;

extern TFixedIHeapClass<HouseTypeClass>					HouseTypes;
extern TFixedIHeapClass<BuildingTypeClass>				BuildingTypes;
extern TFixedIHeapClass<AircraftTypeClass>				AircraftTypes;
extern TFixedIHeapClass<InfantryTypeClass>				InfantryTypes;
extern TFixedIHeapClass<BulletTypeClass>					BulletTypes;
extern TFixedIHeapClass<AnimTypeClass>						AnimTypes;
extern TFixedIHeapClass<UnitTypeClass>						UnitTypes;
extern TFixedIHeapClass<VesselTypeClass>					VesselTypes;
extern TFixedIHeapClass<TemplateTypeClass>				TemplateTypes;
extern TFixedIHeapClass<TerrainTypeClass>					TerrainTypes;
extern TFixedIHeapClass<OverlayTypeClass>					OverlayTypes;
extern TFixedIHeapClass<SmudgeTypeClass>					SmudgeTypes;

extern FixedIHeapClass *										HeapPointers[RTTI_COUNT];

extern TFixedIHeapClass<WeaponTypeClass>					Weapons;
extern TFixedIHeapClass<WarheadTypeClass>					Warheads;

extern QueueClass<EventClass, MAX_EVENTS>					OutList;
extern QueueClass<EventClass, (MAX_EVENTS * 64)>		DoList;

#ifdef MIRROR_QUEUE
extern QueueClass<EventClass, (MAX_EVENTS * 64)>		MirrorList;
#endif

typedef DynamicVectorArrayClass<ObjectClass *, HOUSE_COUNT, HOUSE_FIRST> SelectedObjectsType;
extern SelectedObjectsType									CurrentObject;
extern DynamicVectorClass<TriggerClass *>					LogicTriggers;
extern DynamicVectorClass<TriggerClass *>					MapTriggers;
extern DynamicVectorClass<TriggerClass *> 				HouseTriggers[HOUSE_COUNT];

extern BaseClass													Base;

/* These variables are used to keep track of the slowest speed of a team */
extern MPHType TeamMaxSpeed[MAX_TEAMS];
extern SpeedType TeamSpeed[MAX_TEAMS];
extern bool FormMove;
extern SpeedType FormSpeed;
extern MPHType FormMaxSpeed;

extern bool IsTanyaDead;
extern bool SaveTanya;

extern bool TimeQuake;

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
extern bool PendingTimeQuake;
extern TARGET TimeQuakeCenter;
extern fixed QuakeUnitDamage;
extern fixed QuakeBuildingDamage;
extern int QuakeInfantryDamage;
extern int QuakeDelay;
extern fixed ChronoTankDuration;		// chrono override for chrono tanks
#ifdef FIXIT_ENGINEER	//	checked - ajw 9/28/98
extern fixed EngineerDamage;			// Amount of damage an engineer does
extern fixed EngineerCaptureLevel;	// Building damage level before engineer can capture
#endif
#endif

/*
**	Loaded data file pointers.
*/
extern void const *		Metal12FontPtr;
extern void const *		MapFontPtr;
extern void const *		VCRFontPtr;
extern void const *		TypeFontPtr;
extern void const *		Font3Ptr;
extern void const *		Font6Ptr;
extern void const *		EditorFont;
extern void const *		Font8Ptr;
extern void const *		FontLEDPtr;
extern void const *		ScoreFontPtr;
extern void const *		GradFont6Ptr;
extern char const *		SystemStrings;
extern char const *		DebugStrings;

/*
**	Miscellaneous globals.
*/
extern ChronalVortexClass		ChronalVortex;
extern TTimerClass<SystemTimerClass> TickCount;
extern bool							PassedProximity;	// used in display.cpp
extern HousesType					Whom;
//extern _VQAConfig					AnimControl;
extern long							SpareTicks;
extern long							PathCount;
extern long							CellCount;
extern long							TargetScan;
extern long							SidebarRedraws;
extern DMonoType					MonoPage;
extern bool							GameActive;
extern bool							SpecialFlag;
extern int							ScenarioInit;
extern HouseClass *				PlayerPtr;
extern PaletteClass 				CCPalette;
extern PaletteClass 				BlackPalette;
extern PaletteClass 				WhitePalette;
extern PaletteClass 				GamePalette;
//extern PaletteClass 				InGamePalette;
#define InGamePalette GamePalette
extern PaletteClass 				OriginalPalette;
extern PaletteClass				ScorePalette;
extern int							BuildLevel;
extern unsigned long				ScenarioCRC;

#ifdef FIXIT_VERSION_3
extern bool							bAftermathMultiplayer;		//	Is multiplayer game being played with Aftermath rules?
#else
extern unsigned long 				PlayingAgainstVersion;		// Negotiated version number
extern bool							Version107InMix;				// Is there a v1.07 in the game
#endif

#ifdef FIXIT_VERSION_3			//	For endgame auto-sonar pulse.
extern bool							bAutoSonarPulse;
#endif

#ifdef SCENARIO_EDITOR
extern CELL 						CurrentCell;
#endif

extern SessionClass				Session;
//extern NullModemClass 			NullModem;
extern IPXManagerClass 	 		Ipx;

#if(TEN)
extern TenConnManClass			*Ten;
#endif

#if(MPATH)
extern MPlayerManClass			*MPath;
#endif

#if(TIMING_FIX)
extern int										NewMaxAheadFrame1;
extern int										NewMaxAheadFrame2;
#endif

extern int							Seed;
extern int							CustomSeed;
extern GroundType  				Ground[LAND_COUNT];

/*
**	Constant externs (data is not modified during game play).
*/
extern char const *							Missions[MISSION_COUNT];
extern char const								Keys[];
extern char const * const					VQName[VQ_COUNT];
extern int										CrateData[CRATE_COUNT];
extern char const * const					CrateNames[CRATE_COUNT];
extern int										CrateShares[CRATE_COUNT];
extern AnimType								CrateAnims[CRATE_COUNT];
extern char const * const					SpecialWeaponName[SPC_COUNT];
extern int const								SpecialWeaponHelp[SPC_COUNT];
extern char const * const					SpecialWeaponFile[SPC_COUNT];
extern char const * const					ArmorName[ARMOR_COUNT];
extern char const * const					QuarryName[QUARRY_COUNT];
extern char const * const					FormationName[FORMATION_COUNT];
extern unsigned long const					PlayCodes[];
extern unsigned long const					CheatCodes[];
//extern char const * const					ProjectileNames[];
extern unsigned long const					EditorCodes[];
extern char const * const					SourceName[SOURCE_COUNT];
extern int const								GroundColor[LAND_COUNT];
extern int const								SnowColor[LAND_COUNT];
extern TheaterDataType const 				Theaters[THEATER_COUNT];
extern unsigned char const 				Facing32[256];
extern unsigned char const					Facing16[256];
extern signed char const					Rotation16[256];
extern unsigned char const 				Facing8[256];
extern unsigned char const					Pixel2Lepton[24];
extern COORDINATE const 					StoppingCoordAbs[5];
extern CELL const 							AdjacentCell[FACING_COUNT];
extern COORDINATE const 					AdjacentCoord[FACING_COUNT];
extern unsigned char const					RemapCiv2[];
extern unsigned char const					RemapCiv4[];
extern unsigned char const					RemapCiv5[];
extern unsigned char const					RemapCiv6[];
extern unsigned char const					RemapCiv7[];
extern unsigned char const					RemapCiv8[];
extern unsigned char const					RemapCiv9[];
extern unsigned char const					RemapCiv10[];
extern unsigned char const					RemapEmber[];

extern int SoundOn;

#ifdef WIN32
extern GraphicViewPortClass	HidPage;
#else
extern GraphicBufferClass	HidPage;
#endif
extern int 						MenuList[][8];
extern CDTimerClass<SystemTimerClass> FrameTimer;
extern CDTimerClass<SystemTimerClass> CountDownTimer;

extern SpecialDialogType	SpecialDialog;

extern int RequiredCD;
extern int CurrentCD;
extern int MouseInstalled;


extern int LogLevel;
extern unsigned long LogLevelTime[ MAX_LOG_LEVEL ];
extern unsigned long LogLastTime;

extern class DynamicVectorClass<EventChoiceClass> test2;
extern class DynamicVectorClass<ActionChoiceClass> test3;

extern bool LogDump_Print;

extern "C"{
	extern bool IsTheaterShape;
}

extern void Reset_Theater_Shapes(void);
extern TheaterType LastTheater;
void Coordinate_Remap ( GraphicViewPortClass *inbuffer, int x, int y, int width, int height, unsigned char *remap_table);
void Do_Vortex (int x, int y, int frame);


/************************************************************
** Win32 specific externs
*/
#ifdef WIN32
extern int ReadyToQuit;							//Are we about to exit cleanly
extern bool InDebugger;							//Are we being run from a debugger
void Memory_Error_Handler(void);				//Memory error handler function
void WWDebugString (char *string);
#else
extern bool IsTheaterShape;
#endif	//WIN32


/*************************************************************
** Internet specific externs
*/
#ifdef WIN32

extern char PlanetWestwoodHandle[];			//Planet WW user name
extern char PlanetWestwoodPassword[];		//Planet WW password
extern char PlanetWestwoodIPAddress[];		//IP of server or other player
extern long PlanetWestwoodPortNumber;		//Port number to send to
extern bool PlanetWestwoodIsHost;				//Flag true if player has control of game options
extern unsigned long PlanetWestwoodGameID;	//Game ID
extern HWND	WChatHWND;							//Handle to Wchat window.
extern bool	GameStatisticsPacketSent;
extern bool	ConnectionLost;
extern void *PacketLater;
extern bool SpawnedFromWChat;
extern int ShowCommand;
void	Register_Game_Start_Time(void);
void	Register_Game_End_Time(void);
void	Send_Statistics_Packet(void);
void 	Check_From_WChat(char *wchat_name);
bool 	Do_The_Internet_Menu_Thang (void);
bool 	Server_Remote_Connect(void);
bool 	Client_Remote_Connect(void);
extern	int UnitBuildPenalty;


#endif	 //WIN32

/*
** From SENDFILE.CPP - externs for scenario file transfers
*/
bool Receive_Remote_File ( char *file_name, unsigned int file_length, unsigned int crc, int gametype);
bool Send_Remote_File ( char *file_name, int gametype );
bool Get_Scenario_File_From_Host(char *return_name, int gametype);
bool Find_Local_Scenario (char *description, char *filename, unsigned int length, char *digest, bool official);

#ifdef MPEGMOVIE // Denzil 6/15/98
#ifdef MCIMPEG
#include "mcimovie.h"
extern MCIMovie* MciMovie;
#endif

#include "mpgset.h"
extern MPGSettings* MpgSettings;
#endif

extern "C" bool MMXAvailable;

extern bool ShareAllyVisibility;

#endif
