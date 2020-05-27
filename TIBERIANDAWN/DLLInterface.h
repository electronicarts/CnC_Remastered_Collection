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


#pragma once

#ifndef DLL_INTERFACE_H
#define DLL_INTERFACE_H
		
struct CarryoverObjectStruct;


/*
** DLL Interface version
** 
** 
** 
*/
#define CNC_DLL_API_VERSION		0x100



#define MAX_EXPORT_CELLS			(128 * 128)

#ifdef TIBERIAN_DAWN
#define MAP_MAX_CELL_WIDTH 64
#define MAP_MAX_CELL_HEIGHT 64
#else
#define MAP_MAX_CELL_WIDTH 128
#define MAP_MAX_CELL_HEIGHT 128
#endif


/*
** Interface structs require stricter packing
** 
** 
*/
#pragma pack(push)
#pragma pack(1)




/**************************************************************************************
** 
** Game state request types
** 
** 
*/
enum GameStateRequestEnum {
	GAME_STATE_NONE,
	GAME_STATE_STATIC_MAP,
	GAME_STATE_DYNAMIC_MAP,
	GAME_STATE_LAYERS,
	GAME_STATE_SIDEBAR,
	GAME_STATE_PLACEMENT,
	GAME_STATE_SHROUD,
	GAME_STATE_OCCUPIER,
	GAME_STATE_PLAYER_INFO
};	




/**************************************************************************************
** 
** Static map data (tiles)
** 
** 
*/
struct CNCStaticCellStruct {
	char	TemplateTypeName[32];
	int	IconNumber;
};



enum CnCTheaterType {
	CNC_THEATER_NONE=-1,
	CNC_THEATER_DESERT,
	CNC_THEATER_JUNGLE,
	CNC_THEATER_TEMPERATE,
	CNC_THEATER_WINTER,

	CNC_THEATER_COUNT,
	CNC_THEATER_FIRST=0
};


struct CNCMapDataStruct {
	int	MapCellX;
	int	MapCellY;
	int	MapCellWidth;
	int	MapCellHeight;
	int	OriginalMapCellX;
	int	OriginalMapCellY;
	int	OriginalMapCellWidth;
	int	OriginalMapCellHeight;

	CnCTheaterType Theater;
	char ScenarioName[_MAX_FNAME+_MAX_EXT];
	
	CNCStaticCellStruct StaticCells[MAX_EXPORT_CELLS];
};




/**************************************************************************************
** 
**  Object type enum
** 
** 
*/
#define DLL_LAYER_COUNT 3

enum DllObjectTypeEnum {
	UNKNOWN,
	INFANTRY,
	UNIT,
	AIRCRAFT,
	BUILDING,
	TERRAIN,
	ANIM,
	BULLET,
	OVERLAY,
	SMUDGE,
	OBJECT,
	SPECIAL,
	INFANTRY_TYPE,
	UNIT_TYPE,
	AIRCRAFT_TYPE,
	BUILDING_TYPE,
	VESSEL,
	VESSEL_TYPE
};	
 


/**************************************************************************************
**
**  Object action types
**
**
*/
enum DllActionTypeEnum : unsigned char {
	DAT_NONE,
	DAT_MOVE,
	DAT_NOMOVE,
	DAT_ENTER,
	DAT_SELF,
	DAT_ATTACK,
	DAT_ATTACK_OUT_OF_RANGE,
	DAT_GUARD,
	DAT_SELECT,
	DAT_CAPTURE,
	DAT_SABOTAGE,
	DAT_HEAL,
	DAT_DAMAGE,
	DAT_TOGGLE_PRIMARY,
	DAT_CANT_DEPLOY,
	DAT_REPAIR,
	DAT_CANT_REPAIR
};






/**************************************************************************************
** 
**  Object state data
** 
** 
*/
#define MAX_OCCUPY_CELLS 36
#define MAX_OBJECT_PIPS 18
#define MAX_OBJECT_LINES 3
#define MAX_HOUSES 32

struct CNCObjectLineStruct {
	int X;
	int Y;
	int X1;
	int Y1;
	int Frame;
	unsigned char Color;
};

#define CNC_OBJECT_ASSET_NAME_LENGTH 16
struct CNCObjectStruct {
	void				*CNCInternalObjectPointer;
	char				TypeName[CNC_OBJECT_ASSET_NAME_LENGTH];
	char 				AssetName[CNC_OBJECT_ASSET_NAME_LENGTH];		// CNC uses 8.3 filenames, so it shouldn't need to be bigger than 9
	DllObjectTypeEnum	Type;	
	int					ID;
	int					BaseObjectID;
	DllObjectTypeEnum	BaseObjectType;	
	int					PositionX;
	int					PositionY;
	int					Width;
	int					Height;
	int					Altitude;
	int					SortOrder;
	int					Scale;
	int					DrawFlags;
	short				MaxStrength;
	short				Strength;
	unsigned short		ShapeIndex;
	unsigned short		CellX;
	unsigned short		CellY;
	unsigned short		CenterCoordX;
	unsigned short		CenterCoordY;
	short				SimLeptonX;
	short				SimLeptonY;
	unsigned char		DimensionX;
	unsigned char		DimensionY;
	unsigned char		Rotation;
	unsigned char		MaxSpeed;
	char				Owner;
	char				RemapColor;
	char				SubObject;
	bool				IsSelectable;
	unsigned int		IsSelectedMask;
	bool				IsRepairing;
	bool				IsDumping;
	bool				IsTheaterSpecific;
	unsigned int		FlashingFlags;
	unsigned char		Cloak;
	bool				CanRepair;
	bool				CanDemolish;
	bool				CanDemolishUnit;
	short				OccupyList[MAX_OCCUPY_CELLS];
	int					OccupyListLength;
	int					Pips[MAX_OBJECT_PIPS];
	int					NumPips;
	int					MaxPips;
	CNCObjectLineStruct	Lines[MAX_OBJECT_LINES];
	int					NumLines;
	bool				RecentlyCreated;
	bool				IsALoaner;
	bool				IsFactory;
	bool				IsPrimaryFactory;
	bool				IsDeployable;
	bool				IsAntiGround;
	bool				IsAntiAircraft;
	bool				IsSubSurface;
	bool				IsNominal;
	bool				IsDog;
	bool				IsIronCurtain;
	bool				IsInFormation;
	bool				CanMove[MAX_HOUSES];
	bool				CanFire[MAX_HOUSES];
	bool				CanDeploy;
	bool				CanHarvest;
	bool				CanPlaceBombs;
	bool				IsFixedWingedAircraft;
	bool				IsFake;
	unsigned char		ControlGroup;
	unsigned int		VisibleFlags;
	unsigned int		SpiedByFlags;
	char 				ProductionAssetName[CNC_OBJECT_ASSET_NAME_LENGTH];
	const char*			OverrideDisplayName;
	DllActionTypeEnum	ActionWithSelected[MAX_HOUSES];

	static const unsigned int VISIBLE_FLAGS_ALL = 0xffffffff;
};

struct CNCObjectListStruct {
	int					Count;
	CNCObjectStruct	Objects[1];		// Variable length
};




/**************************************************************************************
** 
**  Placement validity data
** 
**  Used to pass back info about tructure placement validity
*/
struct CNCPlacementCellInfoStruct {
	bool	PassesProximityCheck;			// If the structure was placed in this cell, does that satisfy the proximity check for the whole structure?
	bool	GenerallyClear;					// Is this cell generally clear of obstructions that would prevent placement?
};

struct CNCPlacementInfoStruct {
	int								Count;
	CNCPlacementCellInfoStruct CellInfo[1];		// Variable length
};





/**************************************************************************************
** 
**  Sidebar/construction state data
** 
** 
*/
enum DllSuperweaponTypeEnum {
	SW_NONE,
	SW_UNKNOWN,

	//TD values
	SW_NUKE,
	SW_AIR_STRIKE,
	SW_ION_CANNON,

	//RA values
	SW_SONAR_PULSE,
	SW_CHRONOSPHERE,
	SW_PARA_BOMB,
	SW_PARA_INFANTRY,
	SW_SPY_MISSION,
	SW_IRON_CURTAIN,
	SW_GPS,
	SW_CHRONOSPHERE_DESTINATION
};

struct CNCSidebarEntryStruct {
	char							AssetName[16];						// CNC uses 8.3 filenames, so it shouldn't need to be bigger than 9
	int							BuildableType;						// This is the original buildable type that should be passed back if we want to start/cancel construction
	int							BuildableID;	   				// This is the original buildable id that should be passed back if we want to start/cancel construction
	DllObjectTypeEnum			Type;									// Type converted to shared enum
	DllSuperweaponTypeEnum	SuperWeaponType;
	int							Cost;									// Cost to construct
	int							PowerProvided;						// Power cost to construct
	int							BuildTime;							// Cost to construct
	float							Progress;							// Construction progress (0.0 - 1.0)
	short							PlacementList[MAX_OCCUPY_CELLS];	// Which cells this structure occupies for placement (if structure)
	int							PlacementListLength;					// How many cells
	bool							Completed;							// Construction has completed
	bool							Constructing;						// Is it currently constructing
	bool							ConstructionOnHold;				// Is the current construction on hold
	bool							Busy;									// Is the associated factory busy
	bool							BuildableViaCapture;				// Is this buildable due to the capture of a structure of a different faction. This will be false for captured structures of the same faction (ActLike)
	bool							Fake;									// Is this a fake structure?
};


struct CNCSidebarStruct {
	int							EntryCount[2];			// Counts for the left and right columns
	int							Credits;					// Amount of currency available (excluding Tiberium)
	int							CreditsCounter;		// Visible credits to display in the sidebar (includes count up/down logic)
	int							Tiberium;				// Amount of Tiberium in reserve
	int							MaxTiberium;			// Maximum amount of Tiberium storage available
	int							PowerProduced;
	int							PowerDrained;
	int							MissionTimer;
	unsigned int	  			UnitsKilled;				// Total count of enemy units killed by this player; Includes Infantry, Vehicles, Aircraft
	unsigned int	  			BuildingsKilled;        // Total count of enemy structures killed by this player
	unsigned int	  			UnitsLost;              // Total count player-owned units killed/lost           
	unsigned int	  			BuildingsLost;          // Total count player-owned structures killed/lost      
	unsigned int 				TotalHarvestedCredits;  // Complete total of gained credits over the match (does not include starting credits)
	bool							RepairBtnEnabled;
	bool							SellBtnEnabled;
	bool							RadarMapActive;

	CNCSidebarEntryStruct	Entries[1];			// Variable length column entries
};



enum SidebarRequestEnum {
	SIDEBAR_REQUEST_START_CONSTRUCTION,
	SIDEBAR_REQUEST_HOLD_CONSTRUCTION,
	SIDEBAR_REQUEST_CANCEL_CONSTRUCTION,
	SIDEBAR_REQUEST_START_PLACEMENT,
	SIDEBAR_REQUEST_PLACE,
	SIDEBAR_CANCEL_PLACE,
	SIDEBAR_CLICK_REPAIR,
	SIDEBAR_REQUEST_ENABLE_QUEUE,
	SIDEBAR_REQUEST_DISABLE_QUEUE,
	SIDEBAR_REQUEST_START_CONSTRUCTION_MULTI,
	SIDEBAR_REQUEST_CANCEL_CONSTRUCTION_MULTI
};	

enum SuperWeaponRequestEnum {
	SUPERWEAPON_REQUEST_PLACE_SUPER_WEAPON
};

enum ControlGroupRequestEnum {
	CONTROL_GROUP_REQUEST_CREATE,
	CONTROL_GROUP_REQUEST_TOGGLE,
	CONTROL_GROUP_REQUEST_ADDITIVE_SELECTION,
};



/**************************************************************************************
** 
**  Input events sent into the DLL
** 
** 
*/
enum InputRequestEnum {
	INPUT_REQUEST_NONE,
	INPUT_REQUEST_MOUSE_MOVE,
	INPUT_REQUEST_MOUSE_LEFT_CLICK,
	INPUT_REQUEST_MOUSE_RIGHT_DOWN,
	INPUT_REQUEST_MOUSE_RIGHT_CLICK,
	INPUT_REQUEST_MOUSE_AREA,
	INPUT_REQUEST_MOUSE_AREA_ADDITIVE,
	INPUT_REQUEST_SELL_AT_POSITION,
	INPUT_REQUEST_SELECT_AT_POSITION,
	INPUT_REQUEST_COMMAND_AT_POSITION,
	INPUT_REQUEST_SPECIAL_KEYS
};


/**************************************************************************************
**
**  Structure Requests Repair, Sell
**
**
*/
enum StructureRequestEnum {
	INPUT_STRUCTURE_NONE,
	INPUT_STRUCTURE_REPAIR_START,
	INPUT_STRUCTURE_REPAIR,
	INPUT_STRUCTURE_SELL_START,
	INPUT_STRUCTURE_SELL,
	INPUT_STRUCTURE_CANCEL,
};


/**************************************************************************************
**
**  Unit Requests Scatter, Select Next, Select Previous, Guard Mode, Stop
**
**
*/
enum UnitRequestEnum {
	INPUT_UNIT_NONE,
	INPUT_UNIT_SCATTER,
	INPUT_UNIT_SELECT_NEXT,
	INPUT_UNIT_SELECT_PREVIOUS,
	INPUT_UNIT_GUARD_MODE,
	INPUT_UNIT_STOP,
	INPUT_UNIT_FORMATION_TOGGLE,		// RA Only
	INPUT_UNIT_QUEUED_MOVEMENT_ON, 	// RA Only
	INPUT_UNIT_QUEUED_MOVEMENT_OFF,	// RA Only
};


/**************************************************************************************
**
**  Game Action Requests
**
**
*/
enum GameRequestEnum {
	INPUT_GAME_MOVIE_DONE,
	INPUT_GAME_LOADING_DONE,
};



/**************************************************************************************
**
**  Special Keys
**
**
*/
enum SpecialKeyRequestEnum {
	INPUT_SPECIAL_KEY_CTRL =	0b00000001,
	INPUT_SPECIAL_KEY_ALT =		0b00000010,
	INPUT_SPECIAL_KEY_SHIFT =	0b00000100,
};



/**************************************************************************************
** 
**  Non-static map data.
** 
**  Per-cell smudges and overlays. Smudges are used for things like craters and structure bibs that draw under units. 
**  Overlays are things like walls and tiberium that can't move from the cell but aren't flat like smudges.
** 
** 
*/
struct CNCDynamicMapEntryStruct {
	char						AssetName[16];
	int						PositionX;
	int						PositionY;
	int						Width;
	int						Height;
	short						Type;
	char						Owner;
	int						DrawFlags;
	unsigned char			CellX;
	unsigned char			CellY;
	unsigned char			ShapeIndex;
	bool						IsSmudge;
	bool						IsOverlay;
	bool						IsResource;
	bool						IsSellable;
	bool						IsTheaterShape;
	bool						IsFlag;
};

struct CNCDynamicMapStruct {
	bool							VortexActive;
	int								VortexX;
	int								VortexY;
	int								VortexWidth;
	int								VortexHeight;
	int								Count;
	CNCDynamicMapEntryStruct	Entries[1];			// Variable length
};





/**************************************************************************************
** 
**  Event data
** 
**  Used to call back into the GlyphX engine for one-time events like sound effect triggers
** 
** 
*/
enum EventCallbackType {
	CALLBACK_EVENT_INVALID = -1,
	CALLBACK_EVENT_SOUND_EFFECT = 0,
	CALLBACK_EVENT_SPEECH,
	CALLBACK_EVENT_GAME_OVER,
	CALLBACK_EVENT_DEBUG_PRINT,
	CALLBACK_EVENT_MOVIE,
	CALLBACK_EVENT_MESSAGE,
	CALLBACK_EVENT_UPDATE_MAP_CELL,
	CALLBACK_EVENT_ACHIEVEMENT,
	CALLBACK_EVENT_STORE_CARRYOVER_OBJECTS,
	CALLBACK_EVENT_SPECIAL_WEAPON_TARGETTING,
	CALLBACK_EVENT_BRIEFING_SCREEN,
	CALLBACK_EVENT_CENTER_CAMERA,
	CALLBACK_EVENT_PING
};


struct GameOverMultiPlayerStatsStruct
{
	GameOverMultiPlayerStatsStruct()
	:
		GlyphXPlayerID( 0 ),
		IsHuman( false ),
		WasHuman( false ),
		IsWinner( false ),
		ResourcesGathered( 0 ),
		TotalUnitsKilled( 0 ),
		TotalStructuresKilled( 0 ),
		Efficiency( 0 ),
		Score( 0 )
	{
	}
	__int64 GlyphXPlayerID;
	bool IsHuman;
	bool WasHuman;
	bool IsWinner;
	int ResourcesGathered;
	int TotalUnitsKilled;
	int TotalStructuresKilled;
	int Efficiency; // AKA Economy	
	int Score;
};

#define GAME_OVER_MULTIPLAYER_MAX_PLAYERS_TRACKED	8

enum EventCallbackMessageEnum {
	MESSAGE_TYPE_DIRECT = 0,
	MESSAGE_TYPE_PLAYER_DEFEATED,
	MESSAGE_TYPE_COMPUTER_TAUNT,
	MESSAGE_TYPE_PLAYER_DISCONNECTED
};

struct EventCallbackStruct {

	EventCallbackStruct::EventCallbackStruct(void) : EventType(CALLBACK_EVENT_INVALID), GlyphXPlayerID(0) { }

	EventCallbackType EventType;

	__int64 GlyphXPlayerID;

	union {
		
		struct SoundEffectEvent {
			int SFXIndex;
			int Variation;
			int PixelX;
			int PixelY;
			int PlayerID;		//TO_FIX
			char SoundEffectName[ 16 ];
			int SoundEffectPriority;
			int SoundEffectContext;
		} SoundEffect;
		
		struct SpeechEvent {
			int SpeechIndex;
			int PlayerID;		//TO_FIX
			char SpeechName[ 16 ];
		} Speech;
			  
		struct GameOverEvent {
			bool Multiplayer;
			//
			// Single-player data
			//
			bool PlayerWins;
			const char* MovieName;
			const char* MovieName2;
			const char* MovieName3;
			const char* MovieName4;
			const char* AfterScoreMovieName;
			int Score;
			int Leadership;
			int Efficiency;
			int CategoryTotal;
			int NODKilled;
			int GDIKilled;
			int CiviliansKilled;
			int NODBuildingsDestroyed;
			int GDIBuildingsDestroyed;
			int CiviliansBuildingsDestroyed;
			int RemainingCredits;
			int SabotagedStructureType;
			int TimerRemaining;
 			//
			// Multi-player data
			//
  		int MultiPlayerTotalPlayers;
			GameOverMultiPlayerStatsStruct MultiPlayerPlayersData[ GAME_OVER_MULTIPLAYER_MAX_PLAYERS_TRACKED ];
		} GameOver;
		
		struct DebugPrintEvent {
			const char *PrintString;
		} DebugPrint;

		struct MovieEvent {
			const char* MovieName;
			int Theme;
			bool Immediate;
		} Movie;

		struct MessageEvent {
			const char* Message;
			float TimeoutSeconds;
			EventCallbackMessageEnum MessageType;
			__int64 MessageParam1;
		} Message;

		struct UpdateMapCellEvent {
			int CellX;
			int CellY;
			char TemplateTypeName[32];
		} UpdateMapCell;

		struct AchievementEvent {
			const char* AchievementType;
			const char* AchievementReason;
		} Achievement;

		struct StoreCarryoverObjectsEvent {
			CarryoverObjectStruct* CarryoverList;
		} StoreCarryoverObjects;

		struct SpecialWeaponTargettingEvent {
			int Type;
			int ID;
			char Name[16];
			DllSuperweaponTypeEnum WeaponType;
		} SpecialWeaponTargetting;

		struct CenterCameraEvent {
			int CoordX;
			int CoordY;
		} CenterCamera;

		struct PingEvent {
			int CoordX;
			int CoordY;
		} Ping;
	};

};








/**************************************************************************************
** 
**  Multiplayer setup data
** 
**  Used to pass multiplayer setup info into the C&C code from the GlyphX engine
** 
** 
*/



struct CNCMultiplayerOptionsStruct {
	//int MPlayerPrefColor;				// preferred color index for this player
	//int MPlayerColorIdx;					// actual color index of this player
	//CnCHousesType MPlayerHouse;		// House of this player (GDI/NOD)
	//unsigned char MPlayerLocalID;		// ID of this player
	int MPlayerCount;						// # of human players in this game
	int MPlayerBases;						// 1 = bases are on for this scenario
	int MPlayerCredits;					// # credits everyone gets
	int MPlayerTiberium;					// 1 = tiberium enabled for this scenario
	int MPlayerGoodies;					// 1 = goodies enabled for this scenario
	int MPlayerGhosts;					// 1 = houses with no players will still play
	int MPlayerSolo;						// 1 = allows a single-player net game
	int MPlayerUnitCount;				// # units for non-base multiplayer scenarios
	bool IsMCVDeploy;						// MCV undeploys instead of selling
	bool SpawnVisceroids;				// Do visceroids spawn
	bool EnableSuperweapons;			// Are superweapons available
	bool MPlayerShadowRegrow;
	bool MPlayerAftermathUnits;
	bool CaptureTheFlag;
	bool DestroyStructures;				// New early win condition via destroying all a player's structures
};



struct CNCSpiedInfoStruct {
	int Power;
	int Drain;
	int Money;
};


struct CNCPlayerInfoStruct {
	char					Name[64];
	unsigned char		House;
	int					ColorIndex;
	unsigned __int64	GlyphxPlayerID;
	int					Team;
	int					StartLocationIndex;
	unsigned char		HomeCellX;
	unsigned char		HomeCellY;
	bool					IsAI;
	unsigned int		AllyFlags;
	bool					IsDefeated;
	unsigned int		SpiedPowerFlags;
	unsigned int		SpiedMoneyFlags;
	CNCSpiedInfoStruct	SpiedInfo[MAX_HOUSES];
	int					SelectedID;
	DllObjectTypeEnum	SelectedType;
	DllActionTypeEnum	ActionWithSelected[MAX_EXPORT_CELLS];
	unsigned int		ActionWithSelectedCount;
	unsigned int		ScreenShake;
	bool				IsRadarJammed;
};




//
enum GameRequestType {
	GAME_REQUEST_MOVIE_DONE,
};




/**************************************************************************************
**
**  Rules configuration data
**
**
*/
struct CNCDifficultyDataStruct
{
	float FirepowerBias;
	float GroundspeedBias;
	float AirspeedBias;
	float ArmorBias;
	float ROFBias;
	float CostBias;
	float BuildSpeedBias;

	float RepairDelay;
	float BuildDelay;

	bool IsBuildSlowdown;
	bool IsWallDestroyer;
	bool IsContentScan;
};

struct CNCRulesDataStruct
{
	CNCDifficultyDataStruct Difficulties[3];
};




/**************************************************************************************
** 
**  Debug input interface
** 
** 
*/

enum DebugRequestEnum {
	DEBUG_REQUEST_SPAWN_OBJECT,
	DEBUG_REQUEST_END_GAME,
	DEBUG_REQUEST_UNSHROUD,
	DEBUG_REQUEST_SUPERWEAPON_RECHARGE,
	DEBUG_REQUEST_KILL_OBJECT,
	DEBUG_REQUEST_END_PRODUCTION,
	DEBUG_REQUEST_ADD_RESOURCES,
	DEBUG_REQUEST_UNLOCK_BUILDABLES,
	DEBUG_REQUEST_FORCE_CRASH,
	DEBUG_REQUEST_SET_GLOBAL_FLAG,
};	






/**************************************************************************************
** 
**  Shroud data.
** 
**  Per-cell shroud info
** 
** 
*/
struct CNCShroudEntryStruct {
	char			ShadowIndex;
	bool			IsVisible;
	bool			IsMapped;
	bool			IsJamming;
};

struct CNCShroudStruct {
	int							Count;
	CNCShroudEntryStruct		Entries[1];			// Variable length
};






/**************************************************************************************
**
**  Occupier data.
**
**  Per-cell occupier info
**
**
*/
struct CNCOccupierObjectStruct {
	DllObjectTypeEnum			Type;
	int							ID;
};

struct CNCOccupierEntryHeaderStruct {
	int							Count;
};

struct CNCOccupierHeaderStruct {
	int							Count;
};




/**************************************************************************************
**
**  Carryover object.
**
**  Used to store object data that persists between missions
**
**
*/
struct CarryoverObjectStruct
{
	CarryoverObjectStruct() : Next(0) {}

	CarryoverObjectStruct* Next;

	int RTTI;
	int Type;
	int Cell;
	int Strength;
	int House;
};





/*
** End of strict structure packing
** 
** 
*/
#pragma pack(pop)



#endif //DLL_INTERFACE_H