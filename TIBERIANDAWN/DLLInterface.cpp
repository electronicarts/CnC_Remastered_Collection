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


/*
** DLLInterfac.cpp
** 
**	This is where we implement the API expected by the Instance Server. 
** 
** The Instance Server will pass in requests for loading and starting maps, control input from players,
** and requests for game simulation and rendering states.
** 
** 
*/



#include <stdio.h>

#include	"function.h"
#include "externs.h"
#include "DLLInterface.h"
#include "Gadget.h"
#include "defines.h" // VOC_COUNT, VOX_COUNT
#include "SidebarGlyphx.h"




/*
** Externs
*/
extern int DLL_Startup(const char * command_line);
extern void Reallocate_Big_Shape_Buffer(void);
extern bool ProgEndCalled;
extern int Write_PCX_File(char* name, GraphicViewPortClass& pic, unsigned char* palette );
extern bool Color_Cycle(void);




/*
** Prototypes and constants
*/
bool Debug_Write_Shape_Type(const ObjectTypeClass *type, int shapenum);
bool Debug_Write_Shape(const char *file_name, void const * shapefile, int shapenum, int flags = 0, void const * ghostdata = NULL);

typedef void (__cdecl* CNC_Event_Callback_Type)(const EventCallbackStruct &event);
typedef unsigned __int64 uint64;
typedef __int64 int64;




/*
** Audio defines
** 
** 
** 
** 
** 
*/
// For compatibility with Watcom in audio enums
#pragma warning (disable : 4091)

// From TiberianDawn\Audio.cpp 
enum ContextType;
extern struct SoundEffectNameStruct {
	char const *Name;			// Digitized voice file name.
	int			Priority;	// Playback priority of this sample.
	ContextType	Where;		// In what game context does this sample exist.
} SoundEffectName[VOC_COUNT];

// From TiberianDawn\Audio.cpp 
extern char const* Speech[VOX_COUNT];

// From TiberianDawn\Audio.cpp 
typedef enum {
	IN_NOVAR,			// No variation or alterations allowed.
	IN_JUV,				// Juvenile sound effect alternate option.
	IN_VAR,				// Infantry variance response modification.
};




/*
** Misc defines
** 
** 
** 
** 
** 
*/
#define RANDOM_START_POSITION 0x7f





/*
**  DLL Interface
** 
** 
** 
** 
** 
*/
extern "C" __declspec(dllexport) unsigned int __cdecl CNC_Version(unsigned int version_in);
extern "C" __declspec(dllexport) void __cdecl CNC_Init(const char *command_line, CNC_Event_Callback_Type event_callback);
extern "C" __declspec(dllexport) void __cdecl CNC_Config(const CNCRulesDataStruct& rules);
extern "C" __declspec(dllexport) void __cdecl CNC_Add_Mod_Path(const char *mod_path);
extern "C" __declspec(dllexport) bool __cdecl CNC_Get_Visible_Page(unsigned char *buffer_in, unsigned int &width, unsigned int &height);
extern "C" __declspec(dllexport) bool __cdecl CNC_Get_Palette(unsigned char(&palette_in)[256][3]);
extern "C" __declspec(dllexport) bool __cdecl CNC_Start_Instance(int scenario_index, int build_level, const char *faction, const char *game_type, const char *content_directory, int sabotaged_structure, const char *override_map_name);
extern "C" __declspec(dllexport) bool __cdecl CNC_Start_Instance_Variation(int scenario_index, int scenario_variation, int scenario_direction, int build_level, const char *faction, const char *game_type, const char *content_directory, int sabotaged_structure, const char *override_map_name);
extern "C" __declspec(dllexport) bool __cdecl CNC_Start_Custom_Instance(const char* content_directory, const char* directory_path, const char* scenario_name, int build_level, bool multiplayer);
extern "C" __declspec(dllexport) bool __cdecl CNC_Advance_Instance(uint64 player_id);
extern "C" __declspec(dllexport) bool __cdecl CNC_Get_Game_State(GameStateRequestEnum state_type, uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size);
extern "C" __declspec(dllexport) bool __cdecl CNC_Read_INI(int scenario_index, int scenario_variation, int scenario_direction, const char *content_directory, const char *override_map_name, char *ini_buffer, int _ini_buffer_size);
extern "C" __declspec(dllexport) void __cdecl CNC_Set_Home_Cell(int x, int y, uint64 player_id);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Game_Request(GameRequestEnum request_type);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Game_Settings_Request(int health_bar_display_mode, int resource_bar_display_mode);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Input(InputRequestEnum mouse_event, unsigned char special_key_flags, uint64 player_id, int x1, int y1, int x2, int y2);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Structure_Request(StructureRequestEnum request_type, uint64 player_id, int object_id);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Unit_Request(UnitRequestEnum request_type, uint64 player_id);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Sidebar_Request(SidebarRequestEnum request_type, uint64 player_id, int buildable_type, int buildable_id, short cell_x, short cell_y);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_SuperWeapon_Request(SuperWeaponRequestEnum request_type, uint64 player_id, int buildable_type, int buildable_id, int x1, int y1);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_ControlGroup_Request(ControlGroupRequestEnum request_type, uint64 player_id, unsigned char control_group_index);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Debug_Request(DebugRequestEnum debug_request_type, uint64 player_id, const char *object_name, int x, int y, bool unshroud, bool enemy);
extern "C" __declspec(dllexport) bool __cdecl CNC_Set_Multiplayer_Data(int scenario_index, CNCMultiplayerOptionsStruct &game_options, int num_players, CNCPlayerInfoStruct *player_list, int max_players);
extern "C" __declspec(dllexport) bool __cdecl CNC_Clear_Object_Selection(uint64 player_id);
extern "C" __declspec(dllexport) bool __cdecl CNC_Select_Object(uint64 player_id, int object_type_id, int object_to_select_id);
extern "C" __declspec(dllexport) bool __cdecl CNC_Save_Load(bool save, const char *file_path_and_name, const char *game_type);
extern "C" __declspec(dllexport) void __cdecl CNC_Set_Difficulty(int difficulty);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Player_Switch_To_AI(uint64 player_id);
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Human_Team_Wins(uint64 player_id);
extern "C" __declspec(dllexport) void __cdecl CNC_Start_Mission_Timer(int time);



/*
** Class to implement the interface, and contain additional game state required by the conversion from peer/peer to client/server
** 
** 
** 
** 
** 
*/
class DLLExportClass {
	public:
	
		static void Init(void);
		static void Shutdown(void);
		static void Config(const CNCRulesDataStruct& rules);
		static void Add_Mod_Path(const char *mod_path);
		static void Set_Home_Cell(int x, int y, uint64 player_id);
		static void Set_Content_Directory(const char *dir);

		static bool Get_Layer_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size);
		static bool Get_Sidebar_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size);
		static bool Start_Construction(uint64 player_id, int buildable_type, int buildable_id);
		static bool Hold_Construction(uint64 player_id, int buildable_type, int buildable_id);
		static bool Cancel_Construction(uint64 player_id, int buildable_type, int buildable_id);
		static bool Start_Placement(uint64 player_id, int buildable_type, int buildable_id);
		static BuildingClass *Get_Pending_Placement_Object(uint64 player_id, int buildable_type, int buildable_id);
		static bool Get_Placement_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size);
		static void Convert_Type(const ObjectClass *object, CNCObjectStruct &object_out);
		static void DLL_Draw_Intercept(int shape_number, int x, int y, int width, int height, int flags, ObjectClass *object, const char *shape_file_name = NULL, char override_owner = HOUSE_NONE, int scale = 0x100);
		static void DLL_Draw_Pip_Intercept(const ObjectClass* object, int pip);
		static void DLL_Draw_Line_Intercept(int x, int y, int x1, int y1, unsigned char color, int frame);
		static bool Place(uint64 player_id, int buildable_type, int buildable_id, short cell_x, short cell_y);
		static bool Cancel_Placement(uint64 player_id, int buildable_type, int buildable_id);
		static bool Place_Super_Weapon(uint64 player_id, int buildable_type, int buildable_id, int x, int y);
		static bool Create_Control_Group(unsigned char control_group_index);
		static bool Add_To_Control_Group(unsigned char control_group_index);
		static bool Toggle_Control_Group_Selection(unsigned char control_group_index);
		static bool Construction_Action(SidebarRequestEnum construction_action, uint64 player_id, int buildable_type, int buildable_id);
		static bool MP_Construction_Action(SidebarRequestEnum construction_action, uint64 player_id, int buildable_type, int buildable_id);
		static bool Passes_Proximity_Check(CELL cell_in, BuildingTypeClass *placement_type, unsigned char* placement_distance);
		static void Calculate_Start_Positions(void);
		static void Computer_Message(bool last_player_taunt);

		static void Repair_Mode(uint64 player_id);
		static void Repair(uint64 player_id, int object_id);
		static void Sell_Mode(uint64 player_id);
		static void Sell(uint64 player_id, int object_id);
		static void Repair_Sell_Cancel(uint64 player_id);

		static void Scatter_Selected(uint64 player_id);
		static void Select_Next_Unit(uint64 player_id);
		static void Select_Previous_Unit(uint64 player_id);
		static void Selected_Guard_Mode(uint64 player_id);
		static void Selected_Stop(uint64 player_id);
		static void Team_Units_Formation_Toggle_On(uint64 player_id);
		static void Units_Queued_Movement_Toggle(uint64 player_id, bool toggle);

		static void Cell_Class_Draw_It(CNCDynamicMapStruct *dynamic_map, int &entry_index, CellClass *cell_ptr, int xpixel, int ypixel, bool debug_output);
		static bool Get_Dynamic_Map_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size);
		static bool Get_Shroud_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size);
		static bool Get_Occupier_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size);
		static bool Get_Player_Info_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size);


		static void Set_Event_Callback(CNC_Event_Callback_Type event_callback) {EventCallback = event_callback;}
		static void Debug_Spawn_Unit(const char *object_name, int x, int y, bool enemy = false);
		static void Debug_Spawn_All(int x, int y);
		static bool Try_Debug_Spawn_Unlimbo(TechnoClass *techno, int &cell_x, int &cell_y);
		static void Debug_Kill_Unit(int x, int y);
		static void Debug_Heal_Unit(int x, int y);

		static void On_Play_Movie(const char * movie_name, ThemeType theme, bool immediate);
		static void On_Display_Briefing_Text();

		static void On_Sound_Effect(const HouseClass* player_ptr, int sound_effect_index, const char* extension, int variation, COORDINATE coord);
		static void On_Speech(const HouseClass* player_ptr, int speech_index);

		static void On_Game_Over(uint64 glyphx_player_id, bool player_wins);
		static void On_Multiplayer_Game_Over(void);

		static void On_Message(const HouseClass* player_ptr, const char* message, float timeout_seconds, EventCallbackMessageEnum message_type, int64 message_id);
		
		static void On_Debug_Output(const char *debug_text);

		static void On_Achievement(const HouseClass* player_ptr, const char *achievement_type, const char *achievement_reason);

		static void On_Center_Camera(const HouseClass* player_ptr, int coord_x, int coord_y);

		static void On_Ping(const HouseClass* player_ptr, COORDINATE coord);

		static void Glyphx_Queue_AI();

		static void Force_Human_Team_Wins(uint64 quitting_player_id);

		/*
		** Player context switching for input/output
		*/
		static bool Set_Player_Context(uint64 glyphx_player, bool force = false);
		static void Reset_Player_Context(void);
		static void Adjust_Internal_View(bool force_ignore_view_constraints = false);
		static void Logic_Switch_Player_Context(ObjectClass *object);
		static void Logic_Switch_Player_Context(HouseClass *house);
		static __int64 Get_GlyphX_Player_ID(const HouseClass *house);

		static void Recalculate_Placement_Distances();
				
		static void Reset_Sidebars(void);

		static SidebarGlyphxClass *Get_Current_Context_Sidebar(HouseClass *player_ptr = NULL);

		static uint64 GlyphxPlayerIDs[MAX_PLAYERS];



		static const void *Get_Shadow_Shapes(void) {return Map.ShadowShapes;}
		static const unsigned char *Get_Shadow_Trans(void) {return &Map.ShadowTrans[0];}

		static bool Legacy_Render_Enabled(void);

		static bool Get_Input_Key_State(KeyNumType key);

		static void Set_Special_Key_Flags(unsigned char special_key_flags);
		static void Clear_Special_Key_Flags();

		static bool Load(FileClass & file);
		static bool Save(FileClass & file);
		static void Code_Pointers(void);
		static void Decode_Pointers(void);

		static bool Get_Game_Over()	{ return GameOver; }


	private:
		static void Calculate_Single_Player_Score(EventCallbackStruct&);

		static unsigned int TD_Calculate_Efficiency( unsigned int harvested_credits, unsigned int initial_credits, unsigned int available_credits );
		static unsigned int TD_Calculate_Leadership( int house, unsigned int units_lost, unsigned int buildings_lost );
		static unsigned int TD_Calculate_Score( unsigned int leadership, unsigned int efficiency, unsigned int build_level );

		static void Convert_Action_Type(ActionType type, ObjectClass* object, TARGET target, DllActionTypeEnum& dll_type);

		static void Calculate_Placement_Distances(BuildingTypeClass* placement_type, unsigned char* placement_distance);

		static int CurrentDrawCount;
		static int TotalObjectCount;
		static int SortOrder;
		static CNCObjectListStruct *ObjectList;

		static CNC_Event_Callback_Type EventCallback;


		static int CurrentLocalPlayerIndex;

		static bool GameOver;

		/*
		** Pseudo sidebars for players in multiplayer
		*/
		static SidebarGlyphxClass MultiplayerSidebars[MAX_PLAYERS];

		static CELL MultiplayerStartPositions[MAX_PLAYERS];

		static BuildingTypeClass *PlacementType[MAX_PLAYERS];

		static unsigned char PlacementDistance[MAX_PLAYERS][MAP_CELL_TOTAL];

		static unsigned char SpecialKeyFlags[MAX_PLAYERS];

		/*
		** Mod directories
		*/
		static DynamicVectorClass<char *> ModSearchPaths;

};


/*
** DLLExportClass static data
** 
** 
** 
** 
** 
*/
int DLLExportClass::CurrentDrawCount = 0;
int DLLExportClass::TotalObjectCount = 0;
int DLLExportClass::SortOrder = 0;
CNCObjectListStruct *DLLExportClass::ObjectList = NULL;
SidebarGlyphxClass DLLExportClass::MultiplayerSidebars [MAX_PLAYERS];
uint64 DLLExportClass::GlyphxPlayerIDs[MAX_PLAYERS] = {0xffffffffl};
int DLLExportClass::CurrentLocalPlayerIndex = -1;
CELL DLLExportClass::MultiplayerStartPositions[MAX_PLAYERS];
BuildingTypeClass *DLLExportClass::PlacementType[MAX_PLAYERS];
unsigned char DLLExportClass::PlacementDistance[MAX_PLAYERS][MAP_CELL_TOTAL];
unsigned char DLLExportClass::SpecialKeyFlags[MAX_PLAYERS] = { 0U };
DynamicVectorClass<char *> DLLExportClass::ModSearchPaths;
bool DLLExportClass::GameOver = false;


/*
** Global variables
** 
** 
** 
** 
** 
*/
int DLLForceMouseX = 0;
int DLLForceMouseY = 0;

CNC_Event_Callback_Type DLLExportClass::EventCallback = NULL;

// Needed to accomodate Glyphx client sidebar. ST - 4/12/2019 5:29PM
int GlyphXClientSidebarWidthInLeptons = 0;

bool MPlayerIsHuman[MAX_PLAYERS];
int MPlayerTeamIDs[MAX_PLAYERS];
int MPlayerStartLocations[MAX_PLAYERS];

bool ShareAllyVisibility = true;






void Play_Movie_GlyphX(const char * movie_name, ThemeType theme)
{
	if ((movie_name[0] == 'x' || movie_name[0] == 'X') && movie_name[1] == 0) {
		return;
	}

	DLLExportClass::On_Play_Movie(movie_name, theme, false);
}


void On_Sound_Effect(int sound_index, int variation, COORDINATE coord)
{
	// MBL 02.26.2019
	int voc = sound_index;
	if (voc == VOC_NONE)
	{
		return;
	}

	// MBL 02.26.2019 - Borrowed from AUDIO.CPP Sound_Effect()
	//
	#if 1 
		char const * ext = ""; // ".AUD";
		if (Special.IsJuvenile && SoundEffectName[voc].Where == IN_JUV) {
			ext = ".JUV";
		} else {
			if (SoundEffectName[voc].Where == IN_VAR) {
				/*
				**	For infantry, use a variation on the response. For vehicles, always
				**	use the vehicle response table.
				*/
				if (variation < 0) {
					if (ABS(variation) % 2) {
						ext = ".V00";
					} else {
						ext = ".V02";
					}
				} else {
					if (variation % 2) {
						ext = ".V01";
					} else {
						ext = ".V03";
					}
				}
			}
		}
	#endif
	// END MBL 

	DLLExportClass::On_Sound_Effect(PlayerPtr, sound_index, ext, variation, coord);
}

// MBL 02.06.2020
// void On_Speech(int speech_index)
void On_Speech(int speech_index, HouseClass *house)
{
	// DLLExportClass::On_Speech(PlayerPtr, speech_index); // MBL 02.06.2020
	if (house == NULL) {
		DLLExportClass::On_Speech(PlayerPtr, speech_index);
	}
	else
	{
		DLLExportClass::On_Speech(house, speech_index);
	}
}


void On_Ping(const HouseClass* player_ptr, COORDINATE coord)
{
	DLLExportClass::On_Ping(player_ptr, coord);
}

		  
void GlyphX_Debug_Print(const char *debug_text)
{
	DLLExportClass::On_Debug_Output(debug_text);
}
			  

void On_Achievement_Event(const HouseClass* player_ptr, const char *achievement_type, const char *achievement_reason)
{
	DLLExportClass::On_Achievement(player_ptr, achievement_type, achievement_reason);
}			  




/**************************************************************************************************
* CNC_Version -- Check DLL/Server version
*
* In:		Version expected
*
* Out:	Actual version
*
*
*
* History: 4/9/2020 2:12PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) unsigned int __cdecl CNC_Version(unsigned int version_in)
{
	// Unreferenced, but potentially useful to know which version the server is expecting
	version_in;

	return CNC_DLL_API_VERSION;
}




/**************************************************************************************************
* CNC_Init -- Initialize the .DLL
*
* In:   Command line
*
* Out:
*
*
*
* History: 1/3/2019 11:33AM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Init(const char *command_line, CNC_Event_Callback_Type event_callback)
{
	DLLExportClass::Set_Content_Directory(NULL);
	
	DLL_Startup(command_line);

	// MBL 
	DLLExportClass::Set_Event_Callback( event_callback );

	DLLExportClass::Init();
}



/**************************************************************************************************
* DLL_Shutdown -- Shutdown the .DLL
*
* In:   
*
* Out:
*
*
*
* History: 2/20/2020 1:58PM - ST
**************************************************************************************************/
void DLL_Shutdown(void)
{
	DLLExportClass::Shutdown();
}





/**************************************************************************************************
* CNC_Config -- Configure the plugin
*
* In:   Configuration data
*
* Out:
*
*
*
* History: 10/03/2019 - SKY
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Config(const CNCRulesDataStruct& rules)
{
	DLLExportClass::Config(rules);
}




/**************************************************************************************************
* CNC_Add_Mod_Path -- Add a path to load mod files from
*
* In:   Path to load mods from
*
* Out:
*
*
*
* History: 2/20/2020 2:04PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Add_Mod_Path(const char *mod_path)
{
	DLLExportClass::Add_Mod_Path(mod_path);
}





/**************************************************************************************************
* CNC_Get_Visible_Page -- Get the screen buffer 'SeenBuff' from the game
*
* In:   If buffer_in is null, just return info about page
*
* Out:  false if not changed since last call
*
*
*
* History: 1/3/2019 11:33AM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) bool __cdecl CNC_Get_Visible_Page(unsigned char *buffer_in, unsigned int &width, unsigned int &height)
{
	if (!DLLExportClass::Legacy_Render_Enabled() || (buffer_in == NULL)) {
		return false;
	}

	/*
	** Assume the seen page viewport is the same size as the page
	*/

	GraphicBufferClass *gbuffer = HidPage.Get_Graphic_Buffer();
	if (gbuffer == NULL) {
		return false;
	}

	int view_port_width = Map.MapCellWidth * CELL_PIXEL_W;
	int view_port_height = Map.MapCellHeight * CELL_PIXEL_H;

	if (view_port_width == 0 || view_port_height == 0) {
		return false;
	}

	unsigned char *raw_buffer = (unsigned char*) gbuffer->Get_Buffer();
	long raw_size = gbuffer->Get_Size();
	if (raw_buffer == NULL || gbuffer->Get_Width() < view_port_width || gbuffer->Get_Height() < view_port_height) {
		return false;
	}

	width = view_port_width;
	height = view_port_height;

	int pitch = gbuffer->Get_Width();
	for (int i = 0; i < view_port_height; ++i, buffer_in += view_port_width, raw_buffer += pitch) {
		memcpy(buffer_in, raw_buffer, view_port_width);
	}

	return true;
}




extern "C" __declspec(dllexport) bool __cdecl CNC_Get_Palette(unsigned char(&palette_in)[256][3])
{
	memcpy(palette_in, CurrentPalette, sizeof(palette_in));
	return true;
}




/**************************************************************************************************
* CNC_Set_Multiplayer_Data -- Set up for a multiplayer match
*
* In:   Multiplayer data
*
* Out:  false if data is bad
*
*
*
* History: 1/7/2019 5:20PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) bool __cdecl CNC_Set_Multiplayer_Data(int scenario_index, CNCMultiplayerOptionsStruct &game_options, int num_players, CNCPlayerInfoStruct *player_list, int max_players)
{
	
	if (num_players <= 0) {
		return false;
	}

	if (num_players > min(MAX_PLAYERS, max_players)) {
		return false;
	}

	DLLExportClass::Init();

	//MPlayerPrefColor;												// preferred color index for this player
	//MPlayerColorIdx;												// actual color index of this player
	//MPlayerHouse;													// House of this player (GDI/NOD)
	//MPlayerLocalID;													// ID of this player
	MPlayerCount = num_players;									// # of human players in this game
	MPlayerBases	 	= game_options.MPlayerBases;			// 1 = bases are on for this scenario
	MPlayerCredits	 	= game_options.MPlayerCredits;		// # credits everyone gets
	MPlayerTiberium	= game_options.MPlayerTiberium;		// 1 = tiberium enabled for this scenario
	MPlayerGoodies		= game_options.MPlayerGoodies;		// 1 = goodies enabled for this scenario
	MPlayerGhosts		= game_options.MPlayerGhosts;			// 1 = houses with no players will still play
	MPlayerSolo			= game_options.MPlayerSolo;			// 1 = allows a single-player net game
	MPlayerUnitCount	= game_options.MPlayerUnitCount;		// # units for non-base multiplayer scenarios
	
	Special.IsMCVDeploy = game_options.IsMCVDeploy;
	Special.IsVisceroids = game_options.SpawnVisceroids;
	Special.IsCaptureTheFlag = game_options.CaptureTheFlag;
	Special.IsEarlyWin = game_options.DestroyStructures;

	Rule.AllowSuperWeapons = game_options.EnableSuperweapons;	// Are superweapons available

	if (MPlayerTiberium) {
		Special.IsTGrowth = 1;
		Special.IsTSpread = 1;
	} else {
		Special.IsTGrowth = 0;
		Special.IsTSpread = 0;
	}

	Scenario = scenario_index;
	MPlayerCount = 0;

	for (int i=0 ; i<num_players ; i++) {
		CNCPlayerInfoStruct &player_info = player_list[i];
		MPlayerHouses[i] = (HousesType) player_info.House;
		strncpy(MPlayerNames[i], player_info.Name, MPLAYER_NAME_MAX);
		MPlayerNames[i][MPLAYER_NAME_MAX - 1] = 0;			// Make sure it's terminated

		MPlayerID[i] = Build_MPlayerID(player_info.ColorIndex, (HousesType)player_info.House);

		DLLExportClass::GlyphxPlayerIDs[i] = player_info.GlyphxPlayerID;

		MPlayerIsHuman[i] = !player_info.IsAI;
		if (player_info.IsAI) {
			MPlayerGhosts = true;
		}

		MPlayerTeamIDs[i] = player_info.Team;
		MPlayerStartLocations[i] = player_info.StartLocationIndex;

		/*
		** Temp fix for custom maps that don't have valid start positions set from matchmaking
		*/
		if (i > 0 && MPlayerStartLocations[i] == 0 && MPlayerStartLocations[0] == 0) {
			MPlayerStartLocations[i] = i;
		}

		MPlayerCount++;
	}

	/*
	** We need some default for the local ID in order to have a valid PlayerPtr during scenario load. ST - 4/24/2019 10:33AM
	*/
	MPlayerLocalID = MPlayerID[0];

	return true;
}

extern "C" __declspec(dllexport) bool __cdecl CNC_Clear_Object_Selection(uint64 player_id)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return false;
	}

	Unselect_All();

	return true;
}

extern "C" __declspec(dllexport) bool __cdecl CNC_Select_Object(uint64 player_id, int object_type_id, int object_to_select_id)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return false;
	}

	switch (object_type_id)
	{
		case INFANTRY:
		{
			for (int index = 0; index < Infantry.Count(); index++) {
				InfantryClass * obj = Infantry.Ptr(index);

				if (obj
					&& !obj->IsInLimbo
					&& obj->House == PlayerPtr
					&& Infantry.ID((InfantryClass*)obj) == object_to_select_id)
				{
					if (!obj->Is_Selected_By_Player())
					{
						obj->Select();
						AllowVoice = false;
					}
					return true;
				}
			}
		}
		break;
		case UNIT:
		{
			for (int index = 0; index < Units.Count(); index++) {
				UnitClass * obj = Units.Ptr(index);

				if (obj
					&& !obj->IsInLimbo
					&& obj->House == PlayerPtr
					&& Units.ID((UnitClass*)obj) == object_to_select_id)
				{
					if (!obj->Is_Selected_By_Player())
					{
						obj->Select();
						AllowVoice = false;
					}
					return true;
				}
			}
		}
		break;
		case AIRCRAFT:
		{
			for (int index = 0; index < Aircraft.Count(); index++) {
				AircraftClass * obj = Aircraft.Ptr(index);

				if (obj
					&& !obj->IsInLimbo
					&& obj->House == PlayerPtr
					&& Aircraft.ID((AircraftClass*)obj) == object_to_select_id)
				{
					if (!obj->Is_Selected_By_Player())
					{
						obj->Select();
						AllowVoice = false;
					}
					return true;
				}
			}
		}
		break;
		case BUILDING:
		{
			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass * obj = Buildings.Ptr(index);
				if (obj
					&& !obj->IsInLimbo
					&& obj->House == PlayerPtr
					&& Buildings.ID((BuildingClass*)obj) == object_to_select_id)
				{
					if (!obj->Is_Selected_By_Player())
					{
						obj->Select();
						AllowVoice = false;
					}
					return true;
				}
			}
		}
		break;
	}

	return false;
}


/**************************************************************************************************
* GlyphX_Assign_Houses -- Replacement for Assign_Houses in INI.CPP
*
* In:   
*
* Out:  
*
*
*
* History: 6/25/2019 11:09AM - ST
**************************************************************************************************/
void GlyphX_Assign_Houses(void)
{
	HousesType house;
	HousesType pref_house;
	HouseClass *housep;
	bool house_used[MAX_PLAYERS];	// true = this house is in use
	bool color_used[16];				// true = this color is in use. We have more than 6 color options now, so bumped this to 16. ST - 6/19/2019 5:18PM
	bool preassigned;
	int i,j,random_start_location;
	PlayerColorType color;
	HousesType house2;
	HouseClass *housep2;

	srand(timeGetTime());

	/*
	**	Init the 'used' flag for all houses & colors to 0
	*/
	for (i = 0; i < MAX_PLAYERS; i++) {
		house_used[i] = false;
	}
	for (i = 0; i < 16; i++) {
		color_used[i] = false;
	}

	/*
	** Assign random start positions if needed.
	*/
	int random_start_locations[26];
	int num_start_locations = 0;
	int num_random_start_locations = 0;
	for (i = 0; i < 26; i++) {
		if (Waypoint[i] != -1) {
			preassigned = false;
			for (j = 0; !preassigned && (j < MPlayerCount); j++) {
				if (MPlayerStartLocations[j] == num_start_locations) {
					preassigned = true;
				}
			}
			if (!preassigned) {
				random_start_locations[num_random_start_locations] = num_start_locations;
				num_random_start_locations++;
			}
			num_start_locations++;
		}
	}

	if (num_random_start_locations > 1)	{
		for (i = 0; i < num_random_start_locations - 1; i++) {
			j = i + rand() / (RAND_MAX / (num_random_start_locations - i) + 1);
			int t = random_start_locations[j];
			random_start_locations[j] = random_start_locations[i];
			random_start_locations[i] = t;
		}
	}

	/*
	**	For each player, randomly pick a house
	*/
	random_start_location = 0;
	for (i = 0; i < MPlayerCount; i++) {
		j = Random_Pick(0, MPlayerMax-1);

		/*
		**	If this house was already selected, decrement 'i' & keep looping.
		*/
		if (house_used[j]) {
			i--;
			continue;
		}

		/*
		**	Set the house, preferred house (GDI/NOD), color, and actual house;
		**	get a pointer to the house instance
		*/
		house = (HousesType)(j + (int)HOUSE_MULTI1);
		pref_house = MPlayerID_To_HousesType(MPlayerID[i]);
		color = MPlayerID_To_ColorIndex(MPlayerID[i]);
		housep = HouseClass::As_Pointer(house);
		MPlayerHouses[i] = house;

		/*
		**	Mark this house & color as used
		*/
		house_used[j] = true;
		color_used[color] = true;

		/*
		**	Set the house's IsHuman, Credits, ActLike, & RemapTable
		*/
		memset((char *)housep->Name, 0, MPLAYER_NAME_MAX);
		strncpy((char *)housep->Name, MPlayerNames[i], MPLAYER_NAME_MAX-1);
		housep->IsHuman = MPlayerIsHuman[i];
		housep->Init_Data(color, pref_house, MPlayerCredits);

		/*
		**	Set the start location override
		*/
		if (MPlayerStartLocations[i] != RANDOM_START_POSITION) {
			housep->StartLocationOverride = MPlayerStartLocations[i];
		} else {
			if (random_start_location < num_random_start_locations) {
				housep->StartLocationOverride = random_start_locations[random_start_location++];
			} else {
				housep->StartLocationOverride = -1;
			}
		}

		/*
		**	If this ID is for myself, set up PlayerPtr
		*/
		if (MPlayerID[i] == MPlayerLocalID) {
			PlayerPtr = housep;
		}
	}
	
	/*
	** From INI.CPP. Remove unused AI players.
	*/
	for (int i=0 ; i<MAX_PLAYERS ; i++) {
		
		if (house_used[i]) {
			continue;
		}

		house = (HousesType)(i + (int)HOUSE_MULTI1);
		housep = HouseClass::As_Pointer (house);
		if (housep && housep->IsHuman == false) {
			housep->Clobber_All();
		}
	}

	for (i = 0; i < MPlayerCount; i++) {
		
		house = MPlayerHouses[i];
		housep = HouseClass::As_Pointer(house);

		if (housep) {

			int team = MPlayerTeamIDs[i];

			for (int j=0 ; j<MPlayerCount ; j++) {
			
				if (i != j) {

					if (team == MPlayerTeamIDs[j]) {
						
						house2 = MPlayerHouses[j];
						housep2 = HouseClass::As_Pointer(house2);

						if (housep2) {
							housep->Make_Ally(house2);
						}
					}
				}
			}
		}
	}
}

/**************************************************************************************************
* CNC_Start_Instance -- Load and start a cnc map to use WITHOUT a sceanrio variation (SCEN_VAR) or scenarion direction (SCEN_DIR)
*
* In:   Map initialization parameters
*
* Out:  false if map load failed
*
*
*
* History: 7/10/2019 - LLL
**************************************************************************************************/
extern "C" __declspec(dllexport) bool __cdecl CNC_Start_Instance(int scenario_index, int build_level, const char *faction, const char *game_type, const char *content_directory, int sabotaged_structure, const char *override_map_name)
{
	return CNC_Start_Instance_Variation(scenario_index, (int)SCEN_VAR_NONE, (int)SCEN_DIR_EAST, build_level, faction, game_type, content_directory, sabotaged_structure, override_map_name);
}


/**************************************************************************************************
* HandleSabotagedStructure
* 
* A port of the code from the original game code which is suppose to remove the main previously sabatoged building.
* From what I can tell since it only stores the type it might remove a different building of the same type.
* Watching the GDI longplay on YouTube the player destroys the refinery and yet it exists in the next level. Perhaps there are 2 refineries. 
*
* History: 7/10/2019 - LLL
**************************************************************************************************/
void HandleSabotagedStructure(int structure_type)
{
	SabotagedType = (StructType) structure_type;

	int index;
	if (SabotagedType != STRUCT_NONE && Scenario == 7 && PlayerPtr->Class->House == HOUSE_GOOD) {
		for (index = 0; index < Buildings.Count(); index++) {
			BuildingClass * building = Buildings.Ptr(index);

			if (building && !building->IsInLimbo && building->House != PlayerPtr && building->Class->Type == SabotagedType) {
				building->Limbo();
				delete building;
				break;
			}
		}

		/*
		**	Remove the building from the prebuild list.
		*/
		for (index = 0; index < Base.Nodes.Count(); index++) {
			BaseNodeClass * node = Base.Get_Node(index);

			if (node && node->Type == SabotagedType) {
				Base.Nodes.Delete(index);
				break;
			}
		}
	}
	SabotagedType = STRUCT_NONE;
}



/**************************************************************************************************
* CNC_Read_INI -- Load an ini file into the supplied buffer
*
* In:   Map initialization parameters
*
* Out:  false if ini load failed
*
*
* History: 12/16/2019 11:44AM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) bool __cdecl CNC_Read_INI(int scenario_index, int scenario_variation, int scenario_direction, const char *content_directory, const char *override_map_name, char *ini_buffer, int _ini_buffer_size)
{
	if (content_directory == NULL) {
		return false;
	}

	DLLExportClass::Set_Content_Directory(content_directory);

	// Hack a fix for scenario 21 since the same mission number is used in Covert Ops and N64
	Scenario = (scenario_index == 81) ? 21 : scenario_index;

	ScenVar = (ScenarioVarType)scenario_variation;
	ScenDir = (ScenarioDirType)scenario_direction;

	GameToPlay = GAME_GLYPHX_MULTIPLAYER;
	ScenPlayer = SCEN_PLAYER_MPLAYER;

	if (override_map_name && strlen(override_map_name)) {
		strcpy(ScenarioName, override_map_name);
	} else {
		Set_Scenario_Name(ScenarioName, Scenario, ScenPlayer, (ScenarioDirType)scenario_direction, (ScenarioVarType)scenario_variation);
	}


	if (_ini_buffer_size < _ShapeBufferSize) {
		GlyphX_Debug_Print("INI file buffer may be too small");
		return false;
	}

	if (!ini_buffer) {
		GlyphX_Debug_Print("No INI file buffer");
		return false;
	}

	memset(ini_buffer, _ini_buffer_size, 0);

	char fname[_MAX_PATH];

	sprintf(fname,"%s.INI", ScenarioName);
	CCFileClass file(fname);
	if (!file.Is_Available()) {
		GlyphX_Debug_Print("Failed to find scenario file");
		GlyphX_Debug_Print(fname);
		return(false);

	} else {
		
		GlyphX_Debug_Print("Opened scenario file");
		GlyphX_Debug_Print(fname);
		
		int bytes_read = file.Read(ini_buffer, _ini_buffer_size-1);
		if (bytes_read == _ini_buffer_size - 1) {
			GlyphX_Debug_Print("INI file buffer is too small");
			return false;
		}
	}

	/*
	** Ini buffer should be zero terminated
	*/
	if ((int) strlen(ini_buffer) >= _ini_buffer_size) {
		GlyphX_Debug_Print("INI file buffer overrun");
		return false;
	}

	return true;
}			  


/**************************************************************************************************
* CNC_Set_Home_Cell -- Allows overriding the start position for the camera
*
*
* History: 2/14/2020 - LLL
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Set_Home_Cell(int x, int y, uint64 player_id)
{
	DLLExportClass::Set_Home_Cell(x, y, player_id);
}


/**************************************************************************************************
* CNC_Start_Instance -- Load and start a cnc map
*
* In:   Map initialization parameters
*
* Out:  false if map load failed
*
*
* Renamed and modified to accept a scenario variation 7/10/2019 - LLL
* Modified to accept a scenario direction 7/12/2019 - LLL
*
* History: 1/7/2019 5:20PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) bool __cdecl CNC_Start_Instance_Variation(int scenario_index, int scenario_variation, int scenario_direction, int build_level, const char *faction, const char *game_type, const char *content_directory, int sabotaged_structure, const char *override_map_name)
{
	if (game_type == NULL) {
		return false;
	}

	if (faction == NULL) {
		return false;
	}

	if (content_directory == NULL) {
		return false;
	}

	if (stricmp(faction, "GDI") == 0) {
		ScenPlayer = SCEN_PLAYER_GDI;
		Whom = HOUSE_GOOD;
	}
			  
	if (stricmp(faction, "NOD") == 0) {
		ScenPlayer = SCEN_PLAYER_NOD;
		Whom = HOUSE_BAD;
	}

	if (stricmp(faction, "Jurassic") == 0) {
		ScenPlayer = SCEN_PLAYER_JP;
		Whom = HOUSE_JP;
		Special.IsJurassic = true;
		AreThingiesEnabled = true;
	}

	DLLExportClass::Set_Content_Directory(content_directory);

	// Hack a fix for scenario 21 since the same mission number is used in Covert Ops and N64
	Scenario = (scenario_index == 81) ? 21 : scenario_index;
	BuildLevel = build_level;

	SabotagedType = (StructType)sabotaged_structure;

	ScenVar = (ScenarioVarType)scenario_variation;
	ScenDir = (ScenarioDirType)scenario_direction;

	if (stricmp(game_type, "GAME_NORMAL") == 0) {
		GameToPlay = GAME_NORMAL;
	} else {
		if (stricmp(game_type, "GAME_GLYPHX_MULTIPLAYER") == 0) {
			GameToPlay = GAME_GLYPHX_MULTIPLAYER;
			ScenPlayer = SCEN_PLAYER_MPLAYER;
		} else {
			return false;
		}
	}

	if (override_map_name && strlen(override_map_name)) {
		strcpy(ScenarioName, override_map_name);
	} else {
		Set_Scenario_Name(ScenarioName, Scenario, ScenPlayer, (ScenarioDirType)scenario_direction, (ScenarioVarType)scenario_variation);
	}

	HiddenPage.Clear();
	VisiblePage.Clear();

	/*
	** Set the mouse to some position where it's not going to scroll, or do something else wierd.
	*/
	DLLForceMouseX = 100;
	DLLForceMouseY = 100;
	_Kbd->MouseQX = 100;
	_Kbd->MouseQY = 100;

	GlyphXClientSidebarWidthInLeptons = 0;

	Seed = timeGetTime();

	if (!Start_Scenario(ScenarioName)) {
		return(false);
	}

	HandleSabotagedStructure(sabotaged_structure);

	DLLExportClass::Reset_Sidebars();
	DLLExportClass::Reset_Player_Context();

	/*
	** Make sure the scroll constraints are applied. This is important for GDI 1 where the map isn't wide enough for the screen
	*/
	COORDINATE origin_coord = Coord_Add(Map.TacticalCoord, XY_Coord(1, 0));
	Map.Set_Tactical_Position(origin_coord);
	origin_coord = Coord_Add(Map.TacticalCoord, XY_Coord(-1, 0));
	Map.Set_Tactical_Position(origin_coord);

	DLLExportClass::Calculate_Start_Positions();

	/*
	**	Hide the SeenBuff; force the map to render one frame.  The caller can
	**	then fade the palette in.
	**	(If we loaded a game, this step will fade out the title screen.  If we
	**	started a scenario, Start_Scenario() will have played a couple of VQ
	**	movies, which will have cleared the screen to black already.)
	*/

	//Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
	HiddenPage.Clear();
	VisiblePage.Clear();
	Set_Logic_Page(SeenBuff);
	Map.Flag_To_Redraw(true);
	Map.Render();

	Set_Palette(GamePalette);

	return true;
}





/**************************************************************************************************
* CNC_Start_Custom_Instance -- 
*
*
*
*
* History: 2019/10/17 - JAS
**************************************************************************************************/
extern "C" __declspec(dllexport) bool __cdecl CNC_Start_Custom_Instance(const char* content_directory, const char* directory_path, 
	const char* scenario_name, int build_level, bool multiplayer)
{
	if (content_directory == NULL) {
		return false;
	}

	DLLExportClass::Set_Content_Directory(content_directory);

	if (multiplayer) {
		GameToPlay = GAME_GLYPHX_MULTIPLAYER;
		ScenPlayer = SCEN_PLAYER_MPLAYER;
	} else {
		GameToPlay = GAME_NORMAL;
		ScenPlayer = SCEN_PLAYER_GDI;		// Don't think it matters since we are specifying the exact file to load
	}

	BuildLevel = build_level;

	const int MAX_FILE_PATH = 1024;
	char scenario_file_name[MAX_FILE_PATH];
	char bin_file_name[MAX_FILE_PATH];
	snprintf(scenario_file_name, MAX_FILE_PATH, "%s%s.INI", directory_path, scenario_name);
	snprintf(bin_file_name, MAX_FILE_PATH, "%s%s.BIN", directory_path, scenario_name);

	Seed = timeGetTime();

	Clear_Scenario();

	Read_Scenario_Ini_File(scenario_file_name, bin_file_name, scenario_name, true);

	HiddenPage.Clear();
	VisiblePage.Clear();

	/*
	** Set the mouse to some position where it's not going to scroll, or do something else wierd.
	*/
	DLLForceMouseX = 100;
	DLLForceMouseY = 100;
	_Kbd->MouseQX = 100;
	_Kbd->MouseQY = 100;

	GlyphXClientSidebarWidthInLeptons = 0;

	/*
	if (!Start_Scenario(ScenarioName)) {
		return(false);
	}
	*/

	DLLExportClass::Reset_Sidebars();
	DLLExportClass::Reset_Player_Context();

	/*
	** Make sure the scroll constraints are applied. This is important for GDI 1 where the map isn't wide enough for the screen
	*/
	COORDINATE origin_coord = Coord_Add(Map.TacticalCoord, XY_Coord(1, 0));
	Map.Set_Tactical_Position(origin_coord);
	origin_coord = Coord_Add(Map.TacticalCoord, XY_Coord(-1, 0));
	Map.Set_Tactical_Position(origin_coord);

	DLLExportClass::Calculate_Start_Positions();

	/*
	**	Hide the SeenBuff; force the map to render one frame.  The caller can
	**	then fade the palette in.
	**	(If we loaded a game, this step will fade out the title screen.  If we
	**	started a scenario, Start_Scenario() will have played a couple of VQ
	**	movies, which will have cleared the screen to black already.)
	*/

	//Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
	HiddenPage.Clear();
	VisiblePage.Clear();
	Set_Logic_Page(SeenBuff);
	Map.Flag_To_Redraw(true);
	Map.Render();

	Set_Palette(GamePalette);
	
	return true;
}


bool Debug_Write_Shape_Type(const ObjectTypeClass *type, int shapenum)
{		
	char	fullname[_MAX_FNAME+_MAX_EXT];
	char	buffer[_MAX_FNAME];
	CCFileClass	file;

	if (type->ImageData != NULL) {

		sprintf(buffer, "%s_%d", type->IniName, shapenum);
		_makepath(fullname, NULL, NULL, buffer, ".PCX");

		return Debug_Write_Shape(fullname, type->ImageData, shapenum);
	}

	return false;
}


bool Debug_Write_Shape(const char *file_name, void const * shapefile, int shapenum, int flags, void const * ghostdata)
{
	/*
	** Build frame returns a pointer now instead of the shapes length
	*/
	char *shape_pointer = (char*) Build_Frame(shapefile , shapenum , _ShapeBuffer);
	if (shape_pointer == NULL) {
		return false;;
	}
	if (Get_Last_Frame_Length() > _ShapeBufferSize) {
		return false;;
	}

	int width = Get_Build_Frame_Width(shapefile);
	int height = Get_Build_Frame_Height(shapefile);

	GraphicBufferClass temp_gbuffer(width, height);
	GraphicViewPortClass	temp_viewport(&temp_gbuffer, 0, 0, width, height);

	WindowList[WINDOW_CUSTOM][WINDOWX] = 0;
	WindowList[WINDOW_CUSTOM][WINDOWY] = 0;
	WindowList[WINDOW_CUSTOM][WINDOWWIDTH] = width;
	WindowList[WINDOW_CUSTOM][WINDOWHEIGHT] = height;

	static const char _shape_trans = 0x40;

	if (flags == 0) {
		Buffer_Frame_To_Page(0, 0, width, height, shape_pointer, temp_viewport, SHAPE_NORMAL|SHAPE_WIN_REL|_shape_trans);	//, ghostdata, predoffset);
	} else {
		Buffer_Frame_To_Page(0, 0, width, height, shape_pointer, temp_viewport, flags, ghostdata);
	}	
	Write_PCX_File((char*)file_name, temp_viewport, GamePalette);

	return true;
}



/**************************************************************************************************
* CNC_Advance_Instance -- Process one logic frame
*
* In:   
*
* Out:  Is game still playing?
*
*
*
* History: 1/7/2019 5:20PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) bool __cdecl CNC_Advance_Instance(uint64 player_id)
{
	//DLLExportClass::Set_Event_Callback(event_callback);
	
	InMainLoop = true;
	
	if (Frame <= 10) {		// Don't spam forever, but useful to know that we actually started advancing
		GlyphX_Debug_Print("CNC_Advance_Instance - TD");
	}

	/*
	** Shouldn't really need to do this, but I like the idea of always running the main loop in the context of the same player.
	** Might make tbe bugs more repeatable and consistent. ST - 3/15/2019 11:58AM
	*/
	if (player_id != 0) {
		DLLExportClass::Set_Player_Context(player_id);
	} else {
		DLLExportClass::Set_Player_Context(DLLExportClass::GlyphxPlayerIDs[0]);
	}	
			
	/*
	** Allocate extra memory for uncompressed shapes as needed
	*/
	Reallocate_Big_Shape_Buffer();

	/*
	**	If there is no theme playing, but it looks like one is required, then start one
	**	playing. This is usually the symptom of there being no transition score.
	*/
	//if (SampleType && Theme.What_Is_Playing() == THEME_NONE) {
	//	Theme.Queue_Song(THEME_PICK_ANOTHER);
	//}

	/*
	**	Update the display, unless we're inside a dialog.
	*/
	//if (SpecialDialog == SDLG_NONE && GameInFocus) {

		//WWMouse->Erase_Mouse(&HidPage, TRUE);
		//Map.Input(input, x, y);
		//if (input) {
		//	Keyboard_Process(input);
		//}
		/*
		** The main loop passes these in uninitialized. ST - 2/7/2019 4:36PM
		*/
		KeyNumType input = KN_NONE;					// Player input.
		int x = 0;
		int y = 0;
		Map.Input(input, x, y);
		//if (input) {
		//	Keyboard_Process(input);
		//}

		if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
			/*
			** Process the sidebar. ST - 4/18/2019 11:59AM
			*/
			HouseClass *old_player_ptr = PlayerPtr;
			for (int i=0 ; i<MPlayerCount ; i++) {
				HouseClass *player_ptr = HouseClass::As_Pointer(MPlayerHouses[i]);	//HouseClass::As_Pointer(HOUSE_MULTI2);
				DLLExportClass::Logic_Switch_Player_Context(player_ptr);
				Sidebar_Glyphx_AI(player_ptr, input);
			}
			DLLExportClass::Logic_Switch_Player_Context(old_player_ptr);
		}
	//}

	/*
	** Sort the map's ground layer by y-coordinate value.  This is done
	** outside the IsToRedraw check, for the purposes of game sync'ing
	** between machines; this way, all machines will sort the Map's
	** layer in the same way, and any processing done that's based on
	** the order of this layer will sync on different machines.
	*/
	Map.Layer[LAYER_GROUND].Sort();

	/*
	**	AI logic operations are performed here.
	*/
	//Skip this block of code on first update of single-player games. This helps prevents trigger generated messages on the first update from being lost during loading screen or movie. - LLL
	static bool FirstUpdate = GameToPlay != GAME_GLYPHX_MULTIPLAYER;;
	if (!FirstUpdate)
	{
		HouseClass *old_player_ptr = PlayerPtr;
		Logic.Clear_Recently_Created_Bits();
		Logic.AI();
		DLLExportClass::Logic_Switch_Player_Context(old_player_ptr);
	}
	FirstUpdate = false;

	/*
	**	Manage the inter-player message list.  If Manage() returns true, it means
	**	a message has expired & been removed, and the entire map must be updated.
	*/
	//if (Messages.Manage()) {
	//	HiddenPage.Clear();
	//	Map.Flag_To_Redraw(true);
	//}

	/*
	**	Process all commands that are ready to be processed.
	*/
	if (GameToPlay == GAME_NORMAL) {
		Queue_AI();
	} else {
		if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
			DLLExportClass::Glyphx_Queue_AI();

			/*
			** Process the sidebar. ST - 3/22/2019 2:07PM
			*/
			for (int i=0 ; i<MPlayerCount ; i++) {
				HouseClass *player_ptr = HouseClass::As_Pointer(MPlayerHouses[i]);	//HouseClass::As_Pointer(HOUSE_MULTI2);
				Sidebar_Glyphx_Recalc(player_ptr);
			}
		}
	}

	/*
	**	Keep track of elapsed time in the game.
	*/
	//Score.ElapsedTime += TIMER_SECOND / TICKS_PER_SECOND;

	/*
	**	Perform any win/lose code as indicated by the global control flags.
	*/
	if (EndCountDown) EndCountDown--;

	/*
	**	Check for player wins or loses according to global event flag.
	*/
	if (PlayerWins) {
		//WWMouse->Erase_Mouse(&HidPage, TRUE);
		PlayerLoses = false;
		PlayerWins = false;
		PlayerRestarts = false;
		Map.Help_Text(TXT_NONE);

		InMainLoop = false;

		GlyphX_Debug_Print("PlayerWins = true");

		if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
			DLLExportClass::On_Multiplayer_Game_Over();
		} else {
			DLLExportClass::On_Game_Over(player_id, true);
		}

		return false;
	}
	if (PlayerLoses) {

		//WWMouse->Erase_Mouse(&HidPage, TRUE);
		PlayerWins = false;
		PlayerLoses = false;
		PlayerRestarts = false;
		Map.Help_Text(TXT_NONE);

		//Do_Lose(); //Old C&C code
		GlyphX_Debug_Print("PlayerLoses = true");
		if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
			DLLExportClass::On_Multiplayer_Game_Over();
		} else {
			DLLExportClass::On_Game_Over(player_id, false);
		}

		InMainLoop = false;

		return false;
	}

	/*
	**	The frame logic has been completed. Increment the frame
	**	counter.
	*/
	Frame++;

	/*
	** Very rarely, the human players will get a message from the computer.
	*/
	if (GameToPlay != GAME_NORMAL && MPlayerGhosts && IRandom(0,10000) == 1) {
		DLLExportClass::Computer_Message(false);
	}

	/*
	** The code is often leaving dangling pointers in overlappers. We can afford the CPU time to just clean them up. I suspect
	** the underlying cause was probably fixed in RA.
	** ST - 4/14/2020 11:45AM
	*/
	Map.Clean();

#ifndef NDEBUG
	/*
	** Is there a memory trasher altering the map??
	*/
	if (!Map.Validate()) {
		GlyphX_Debug_Print("Map.Validate() failed");
			
		//if (CCMessageBox().Process ("Map Error!","Stop","Continue")==0) {
		//	GameActive = false;
		//}
		Map.Validate();		// give debugger a chance to catch it
	}
#endif NDEBUG

	InMainLoop = false;

	if (ProgEndCalled) {
		GlyphX_Debug_Print("ProgEndCalled - GameActive = false");
		GameActive = false;
	}

	if (DLLExportClass::Legacy_Render_Enabled()) {
		Map.Render();
	}

	//Sync_Delay();
	Color_Cycle();
	//DLLExportClass::Set_Event_Callback(NULL);
	return(GameActive);
}


/**************************************************************************************************
* CNC_Save_Load -- Process a save or load game action
*
* In:   
*
* Out:  Success?
*
*
*
* History: 1/7/2019 5:20PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) bool __cdecl CNC_Save_Load(bool save, const char *file_path_and_name, const char *game_type)
{
	bool result = false;

	if (save) {
		result = Save_Game(file_path_and_name, "internal");
	} else {
		
		if (game_type == NULL) {
			return false;
		}
	
		if (stricmp(game_type, "GAME_NORMAL") == 0) {
			GameToPlay = GAME_NORMAL;
		} else {
			if (stricmp(game_type, "GAME_GLYPHX_MULTIPLAYER") == 0) {
				GameToPlay = GAME_GLYPHX_MULTIPLAYER;
				ScenPlayer = SCEN_PLAYER_MPLAYER;
			} else {
				return false;
			}
		}
		
		result = Load_Game(file_path_and_name);
		
		DLLExportClass::Set_Player_Context(DLLExportClass::GlyphxPlayerIDs[0], true);
		Set_Logic_Page(SeenBuff);
		VisiblePage.Clear();
		Map.Flag_To_Redraw(true);
		if (DLLExportClass::Legacy_Render_Enabled()) {
			Map.Render();
		}
		Set_Palette(GamePalette);
	}

	return result;
}



/**************************************************************************************************
* CNC_Set_Difficulty -- Set game difficulty
*
* In:
*
* Out:
*
*
*
* History: 10/02/2019 - SKY
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Set_Difficulty(int difficulty)
{
	if (GameToPlay == GAME_NORMAL) {
		Set_Scenario_Difficulty(difficulty);
	}
}


/**************************************************************************************************
* CNC_Handle_Player_Switch_To_AI -- Renamed 3/9/20202 - LLL
* previously named: CNC_Handle_Player_Disconnect -- Handle player disconnected during multiplayuer game
*
* In:   
*
* Out:  
*
*
*
* History: 12/3/2019 1:46PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Player_Switch_To_AI(uint64 player_id)
{
	if (PlayerWins || PlayerLoses || DLLExportClass::Get_Game_Over()) {
		return;
	}

	HousesType house;
	HouseClass *ptr;
	
	GlyphX_Debug_Print("CNC_Handle_Player_Switch_To_AI");

	if (GameToPlay == GAME_NORMAL) {
		return;
	}
	
	if (player_id != 0) {
		DLLExportClass::Set_Player_Context(player_id);

		if (PlayerPtr) {
			PlayerPtr->WasHuman = true;
			PlayerPtr->IsHuman = false;
			PlayerPtr->IsStarted = true;
			PlayerPtr->IQ = Rule.MaxIQ;
			PlayerPtr->IsBaseBuilding = true;

			/*
			** Start the unload mission for MCVs
			*/
			for (int index = 0; index < Units.Count(); index++) {
				UnitClass * obj = Units.Ptr(index);

				if (obj && !obj->IsInLimbo && obj->House == PlayerPtr) {
					if (*obj == UNIT_MCV) {
						obj->Assign_Mission(MISSION_GUARD);
						obj->Assign_Target(TARGET_NONE);
						obj->Assign_Destination(TARGET_NONE);
						obj->Assign_Mission(MISSION_UNLOAD);
						obj->Commence();
					}
				}
			}

			DLLExportClass::On_Message(PlayerPtr, "", 60.0f, MESSAGE_TYPE_PLAYER_DISCONNECTED, -1);

			/*
			** Send the disconnect taunt message
			*/
			int human_count = 0;
			for (house = HOUSE_MULTI1; house < (HOUSE_MULTI1 + MPlayerMax); house++) {
				ptr = HouseClass::As_Pointer(house);

				if (ptr && ptr->IsHuman && !ptr->IsDefeated) {
					human_count++;
				}
			}

			if (human_count == 1) {
				DLLExportClass::Computer_Message(true);
			}
		}
	}
}


/**************************************************************************************************
* CNC_Handle_Human_Team_Wins
*
* History: 3/10/2020 - LLL
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Human_Team_Wins(uint64 quitting_player_id)
{
	GlyphX_Debug_Print("CNC_Handle_Human_Team_Wins");
	DLLExportClass::Force_Human_Team_Wins(quitting_player_id);
}


/**************************************************************************************************
* CNC_Start_Mission_Timer
*
* History: 11/25/2019 - LLL
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Start_Mission_Timer(int time)
{
	//Only implemented in Red Alert.
}



/**************************************************************************************************
* DLLExportClass::Init -- Init the class
*
* In:   
*
* Out:  
*
*
*
* History: 3/12/2019 10:52AM - ST
**************************************************************************************************/
void DLLExportClass::Init(void)
{
	for (int i=0 ; i<MAX_PLAYERS ; i++) {
		GlyphxPlayerIDs[i] = 0xffffffffull;
	}

	CurrentLocalPlayerIndex = 0;

}



/**************************************************************************************************
* DLLExportClass::Shutdown -- Shutdown
*
* In:   
*
* Out:  
*
*
*
* History: 2/20/2020 1:59PM - ST
**************************************************************************************************/
void DLLExportClass::Shutdown(void)
{
	for (int i=0 ; i<ModSearchPaths.Count() ; i++) {
		delete [] ModSearchPaths[i];
	}
	ModSearchPaths.Clear();
}




/**************************************************************************************************
* DLLExportClass::Add_Mod_Path -- Add a path to load mod files from
*
* In: Mod path  
*
* Out:  
*
*
*
* History: 2/20/2020 2:03PM - ST
**************************************************************************************************/
void DLLExportClass::Add_Mod_Path(const char *mod_path)
{
	char *copy_path = strdup(mod_path);
	ModSearchPaths.Add(copy_path);
}
	



/**************************************************************************************************
* DLLExportClass::Set_Content_Directory -- Update the locations that the original code will load from
*
* In: Main (official) content directory
*
* Out:  
*
*
*
* History: 2/20/2020 2:03PM - ST
**************************************************************************************************/
void DLLExportClass::Set_Content_Directory(const char *content_directory)
{
	CCFileClass::Clear_Search_Drives();
	CCFileClass::Reset_Raw_Path();

	if ((content_directory == NULL || strlen(content_directory) == 0) && ModSearchPaths.Count() == 0) {
		return;
	}

	char *all_paths = new char [_MAX_PATH * 100];
	*all_paths = 0;

	for (int i=0 ; i<ModSearchPaths.Count() ; i++) {
		if (i != 0) {
			strcat(all_paths, ";");
		}
		strcat(all_paths, ModSearchPaths[i]);
	}

	if (ModSearchPaths.Count() && content_directory && strlen(content_directory)) {
		strcat(all_paths, ";");
	}

	if (content_directory) {
		strcat(all_paths, content_directory);
	}

	CCFileClass::Set_Search_Drives(all_paths);
	delete [] all_paths;
}






/**************************************************************************************************
* DLLExportClass::Config
*
* History: 1/16/2020 - SKY
**************************************************************************************************/
void DLLExportClass::Config(const CNCRulesDataStruct& rules)
{
	for (int i = 0; i < 3; ++i)
	{
		Rule.Diff[i].FirepowerBias = rules.Difficulties[i].FirepowerBias;
		Rule.Diff[i].GroundspeedBias = rules.Difficulties[i].GroundspeedBias;
		Rule.Diff[i].AirspeedBias = rules.Difficulties[i].AirspeedBias;
		Rule.Diff[i].ArmorBias = rules.Difficulties[i].ArmorBias;
		Rule.Diff[i].ROFBias = rules.Difficulties[i].ROFBias;
		Rule.Diff[i].CostBias = rules.Difficulties[i].CostBias;
		Rule.Diff[i].BuildSpeedBias = rules.Difficulties[i].BuildSpeedBias;
		Rule.Diff[i].RepairDelay = rules.Difficulties[i].RepairDelay;
		Rule.Diff[i].BuildDelay = rules.Difficulties[i].BuildDelay;
		Rule.Diff[i].IsBuildSlowdown = rules.Difficulties[i].IsBuildSlowdown ? 1 : 0;
		Rule.Diff[i].IsWallDestroyer = rules.Difficulties[i].IsWallDestroyer ? 1 : 0;
		Rule.Diff[i].IsContentScan = rules.Difficulties[i].IsContentScan ? 1 : 0;
	}	
}


/**************************************************************************************************
* DLLExportClass::Set_Home_Cell
*
* History: 2/14/2020 - LLL
**************************************************************************************************/
extern CELL Views[4];
void DLLExportClass::Set_Home_Cell(int x, int y, uint64 player_id)
{
	if (GameToPlay == GAME_NORMAL) {
		MultiplayerStartPositions[0] = Views[0] = XY_Cell(x, y);
	}
	else {
		for (int i = 0; i < MPlayerCount && i < 4; i++) {
			if (GlyphxPlayerIDs[i] == player_id) {
				Views[i] = MultiplayerStartPositions[i] = XY_Cell(x, y);
			}
		}
	}
}


/**************************************************************************************************
* DLLExportClass::On_Play_Movie
*
* History: 7/23/2019 - LLL
**************************************************************************************************/
void DLLExportClass::On_Play_Movie(const char * movie_name, ThemeType theme, bool immediate)
{
	if (EventCallback == NULL) {
		return;
	}

	InMainLoop = false;

	EventCallbackStruct new_event;
	new_event.EventType = CALLBACK_EVENT_MOVIE;
	new_event.Movie.MovieName = movie_name;
	new_event.Movie.Theme = (int)theme;
	new_event.Movie.Immediate = immediate;

	EventCallback(new_event);
}

/**************************************************************************************************
* DLLExportClass::On_Display_Briefing_Text
*
* Called when Red Alert wants to display the mission breifing screen before a mission.
*
* History: 12/04/2019 - LLL
**************************************************************************************************/
void DLLExportClass::On_Display_Briefing_Text()
{
	//Only implemeneted for Red Alert - LLL
}


/**************************************************************************************************
* DLLExportClass::On_Sound_Effect -- Called when C&C wants to play a sound effect
*
* In:   
*
* Out:
*
*
*
* History: 2/20/2019 2:39PM - ST
**************************************************************************************************/
void DLLExportClass::On_Sound_Effect(const HouseClass* player_ptr, int sound_effect_index, const char* extension, int variation, COORDINATE coord)
{
	// player_ptr could be NULL

	if (EventCallback == NULL) {
		return;
	}

	EventCallbackStruct new_event;
	new_event.EventType = CALLBACK_EVENT_SOUND_EFFECT;
	new_event.SoundEffect.SFXIndex = sound_effect_index;
	new_event.SoundEffect.Variation = variation;

	new_event.GlyphXPlayerID = 0;
	if ( player_ptr != NULL ) 
	{
		new_event.GlyphXPlayerID = Get_GlyphX_Player_ID(player_ptr);
	}

	if ( coord == 0 ) 
	{
		new_event.SoundEffect.PixelX = -1;
		new_event.SoundEffect.PixelY = -1;
	}
	else
	{
		// Use world pixel coordinates
		new_event.SoundEffect.PixelX = Lepton_To_Pixel(Coord_X(coord));
		new_event.SoundEffect.PixelY = Lepton_To_Pixel(Coord_Y(coord));
	}

	if ( sound_effect_index >= VOC_FIRST && sound_effect_index < VOC_COUNT )	
	{
		strncpy( new_event.SoundEffect.SoundEffectName, SoundEffectName[ sound_effect_index ].Name, CNC_OBJECT_ASSET_NAME_LENGTH);
		new_event.SoundEffect.SoundEffectName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;  // strncpy can leave strings unterminated		
		if ( extension != NULL ) 
		{
			strncat( new_event.SoundEffect.SoundEffectName, extension, CNC_OBJECT_ASSET_NAME_LENGTH);
			new_event.SoundEffect.SoundEffectName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;	// strncat can leave strings unterminated
		}
		new_event.SoundEffect.SoundEffectPriority = SoundEffectName[ sound_effect_index ].Priority;
		new_event.SoundEffect.SoundEffectContext = SoundEffectName[ sound_effect_index ].Where;
	}
	else
	{
		strncpy( new_event.SoundEffect.SoundEffectName, "BADINDEX", CNC_OBJECT_ASSET_NAME_LENGTH);
		new_event.SoundEffect.SoundEffectPriority = -1;
		new_event.SoundEffect.SoundEffectContext = -1;
	}

	EventCallback(new_event);
}


	  
/**************************************************************************************************
* DLLExportClass::On_Speech -- Called when C&C wants to play a speech line
*
* In:   
*
* Out:
*
*
*
* History: 2/20/2019 2:39PM - ST
**************************************************************************************************/
void DLLExportClass::On_Speech(const HouseClass* player_ptr, int speech_index)
{
	// player_ptr could be NULL

	if (EventCallback == NULL) {
		return;
	}
	
	EventCallbackStruct new_event;
	new_event.EventType = CALLBACK_EVENT_SPEECH;
	new_event.Speech.SpeechIndex = speech_index;

	new_event.GlyphXPlayerID = 0;
	if ( player_ptr != NULL ) 
	{
		new_event.GlyphXPlayerID = Get_GlyphX_Player_ID(player_ptr);
	}
	
	if ( speech_index >= VOX_FIRST && speech_index < VOX_COUNT )	
	{
		strncpy( new_event.Speech.SpeechName, Speech[ speech_index ], CNC_OBJECT_ASSET_NAME_LENGTH);
		new_event.Speech.SpeechName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;	// strncpy can leave strings unterminated
	}
	else
	{
		strncpy( new_event.Speech.SpeechName, "BAD_SPEECH_INDEX", CNC_OBJECT_ASSET_NAME_LENGTH);
	}

	EventCallback(new_event);
}		

/**************************************************************************************************
* DLLExportClass::TD_Calculate_Efficiency --
*
* History: 10.29.2019 MBL (Based on LLL's Calculate_Single_Player_Score())
**************************************************************************************************/
unsigned int DLLExportClass::TD_Calculate_Efficiency( unsigned int harvested_credits, unsigned int initial_credits, unsigned int available_credits )
{
	unsigned efficiency = Cardinal_To_Fixed( harvested_credits + (initial_credits + 1), (available_credits + 1) );
	if ( efficiency == 0 ) {
		efficiency++;
	}

	efficiency = Fixed_To_Cardinal(100, efficiency);
	if (efficiency > 100) {
		efficiency = 100;
	}

	return efficiency;
}

/**************************************************************************************************
* DLLExportClass::TD_Calculate_Leadership --
*
* History: 10.29.2019 MBL (Based on LLL's Calculate_Single_Player_Score())
**************************************************************************************************/
unsigned int DLLExportClass::TD_Calculate_Leadership( int house, unsigned int units_lost, unsigned int buildings_lost )
{
	unsigned int leadership = 0;

	for (int index = 0; index < Logic.Count(); index++) {
		ObjectClass * object = Logic[index];
		if (object->Owner() == house) {
			leadership++;
		}
	}

	if (leadership == 0) {
		leadership = 1;
	}

	leadership = Cardinal_To_Fixed(units_lost + buildings_lost + leadership, leadership);

	leadership = Fixed_To_Cardinal(100, leadership);
	if (leadership > 100) {
		leadership = 100;
	}

	return leadership;
}

/**************************************************************************************************
* DLLExportClass::TD_Calculate_Score --
*
* History: 10.29.2019 MBL (Based on LLL's Calculate_Single_Player_Score())
**************************************************************************************************/
unsigned int DLLExportClass::TD_Calculate_Score( unsigned int leadership, unsigned int efficiency, unsigned int build_level )
{
	long total = ((leadership * 40) + (4600) + (efficiency * 14)) / 100;
	if (!total) total++;
	total *= (build_level + 1);

	return total;
}

void DLLExportClass::Calculate_Single_Player_Score(EventCallbackStruct& event)
{
	//Adapted from Tiberian Dawn SCORE.CPP Presentation() - LLL
	int house = PlayerPtr->Class->House;		// 0 or 1

	HouseClass *houses[3];
	for (int index = 0; index < 3; index++) {
		houses[index] = (HouseClass::As_Pointer((HousesType)(HOUSE_GOOD + index)));
	}

	int gdi_units_lost = (HouseClass::As_Pointer(HOUSE_GOOD))->UnitsLost;
	int nod_units_lost = (HouseClass::As_Pointer(HOUSE_BAD))->UnitsLost;
	int civilians_killed = (HouseClass::As_Pointer(HOUSE_NEUTRAL))->UnitsLost;
	int gdi_buildings_lost = (HouseClass::As_Pointer(HOUSE_GOOD))->BuildingsLost;
	int nod_buildings_lost = (HouseClass::As_Pointer(HOUSE_BAD))->BuildingsLost;
	int civilian_buildings_lost = (HouseClass::As_Pointer(HOUSE_NEUTRAL))->BuildingsLost;
	int gdi_harvested = (HouseClass::As_Pointer(HOUSE_GOOD))->HarvestedCredits;
	int nod_harvested = (HouseClass::As_Pointer(HOUSE_BAD))->HarvestedCredits;

	// unsigned leadership = 0;
	// int index;
	// for (index = 0; index < Logic.Count(); index++) {
	// 	ObjectClass * object = Logic[index];
	// 	if (object->Owner() == house) {
	// 		leadership++;
	// 	}
	// }
	// 
	// if (leadership == 0) {
	// 	leadership = 1;
	// }
	// 
	// leadership = Cardinal_To_Fixed(gdi_units_lost + gdi_buildings_lost + leadership, leadership);
	// leadership = Fixed_To_Cardinal(100, leadership);
	// if (leadership > 100) {
	// 	leadership = 100;
	// }
	//
	unsigned leadership = TD_Calculate_Leadership( house, (house == HOUSE_GOOD ? gdi_units_lost : nod_units_lost), (house == HOUSE_GOOD ? gdi_buildings_lost : nod_buildings_lost) );

	/*
	**	Determine efficiency rating.
	*/
	// int gharv = gdi_harvested;
	// int init = PlayerPtr->InitialCredits;
	// int cred = PlayerPtr->Available_Money();
	// 
	// unsigned efficiency = Cardinal_To_Fixed((house == HOUSE_GOOD ? gdi_harvested : nod_harvested) + (unsigned)PlayerPtr->InitialCredits + 1, (unsigned)PlayerPtr->Available_Money() + 1);
	// if (!efficiency) efficiency++;
	// efficiency = Fixed_To_Cardinal(100, efficiency);
	// if (efficiency > 100) {
	// 	efficiency = 100;
	// }
	//
	unsigned efficiency = TD_Calculate_Efficiency((house == HOUSE_GOOD ? gdi_harvested : nod_harvested), PlayerPtr->InitialCredits, PlayerPtr->Available_Money());

	/*
	** Calculate total score
	*/
	// long total_score = ((leadership * 40) + (4600) + (efficiency * 14)) / 100;
	// if (!total_score) total_score++;
	// total_score *= (BuildLevel + 1);
	//
	unsigned total_score = TD_Calculate_Score( leadership, efficiency, BuildLevel );

	//Score Stats
	event.GameOver.Leadership = leadership;
	event.GameOver.Efficiency = efficiency;
	event.GameOver.Score = total_score;
	event.GameOver.CategoryTotal = 0; //Only used in Red Alert
	event.GameOver.NODKilled = nod_units_lost;
	event.GameOver.GDIKilled = gdi_units_lost;
	event.GameOver.CiviliansKilled = civilians_killed;
	event.GameOver.NODBuildingsDestroyed = nod_buildings_lost;
	event.GameOver.GDIBuildingsDestroyed = gdi_buildings_lost;
	event.GameOver.CiviliansBuildingsDestroyed = civilian_buildings_lost;
	event.GameOver.RemainingCredits = PlayerPtr->Available_Money();
}

void DLLExportClass::Convert_Action_Type(ActionType type, ObjectClass* object, TARGET target, DllActionTypeEnum& dll_type)
{
	switch (type)
	{
		case ACTION_NONE:
		default:
			dll_type = DAT_NONE;
			break;
		case ACTION_MOVE:
			dll_type = DAT_MOVE;
			break;
		case ACTION_NOMOVE:
			dll_type = DAT_NOMOVE;
			break;
		case ACTION_ENTER:
			dll_type = DAT_ENTER;
			break;
		case ACTION_SELF:
			dll_type = DAT_SELF;
			break;
		case ACTION_ATTACK:
			if (Target_Legal(target) && (object != NULL) && object->Is_Techno() && ((TechnoClass*)object)->In_Range(target, 0)) {
				dll_type = DAT_ATTACK;
			}
			else {
				dll_type = DAT_ATTACK_OUT_OF_RANGE;
			}
			break;
		case ACTION_GUARD_AREA:
			dll_type = DAT_GUARD;
			break;
		case ACTION_HARVEST:
			dll_type = DAT_ATTACK;
			break;
		case ACTION_SELECT:
		case ACTION_TOGGLE_SELECT:
			dll_type = DAT_SELECT;
			break;
		case ACTION_CAPTURE:
			dll_type = DAT_CAPTURE;
			break;
		case ACTION_SABOTAGE:
			dll_type = DAT_SABOTAGE;
			break;
		case ACTION_TOGGLE_PRIMARY:
			dll_type = DAT_TOGGLE_PRIMARY;
			break;
		case ACTION_NO_DEPLOY:
			dll_type = DAT_CANT_DEPLOY;
			break;
	}
}




/**************************************************************************************************
* DLLExportClass::On_Game_Over -- Called when the C&C campaign game finishes
*
*
* History: 6/19/2019 - LLL
**************************************************************************************************/
void DLLExportClass::On_Game_Over(uint64 glyphx_Player_id, bool player_wins)
{
	if (EventCallback == NULL) {
		return;
	}

	GameOver = true;

	EventCallbackStruct new_event;
	new_event.EventType = CALLBACK_EVENT_GAME_OVER;
	new_event.GlyphXPlayerID = glyphx_Player_id;
	new_event.GameOver.PlayerWins = player_wins;
	new_event.GameOver.MovieName = player_wins ? WinMovie : LoseMovie;
	new_event.GameOver.AfterScoreMovieName = "";
	new_event.GameOver.Multiplayer = false;
	new_event.GameOver.MultiPlayerTotalPlayers = 0;

	Calculate_Single_Player_Score(new_event);

	if (player_wins)
	{
		if (PlayerPtr->Class->House == HOUSE_BAD && Scenario == 13) {
			//TODO: Nod_Ending() Also looks like it plays some audio that might need to be integrated.
			new_event.GameOver.MovieName = "";
			new_event.GameOver.AfterScoreMovieName = "NODFINAL";
		}
		else if (PlayerPtr->Class->House == HOUSE_GOOD && Scenario == 15) {
			if (TempleIoned) {
				new_event.GameOver.MovieName = "GDIFINB";
				new_event.GameOver.AfterScoreMovieName = "GDIEND2";
			}
			else {
				new_event.GameOver.MovieName = "GDIFINA";
				new_event.GameOver.AfterScoreMovieName = "GDIEND1";
			}
		}
	}

	if (strcmp(new_event.GameOver.MovieName, "x") == 0 || strcmp(new_event.GameOver.MovieName, "X") == 0) {
		new_event.GameOver.MovieName = "";
	}

	new_event.GameOver.MovieName2 = WinMovie2;
	if (strcmp(new_event.GameOver.MovieName2, "x") == 0 || strcmp(new_event.GameOver.MovieName2, "X") == 0) {
		new_event.GameOver.MovieName2 = "";
	}

	new_event.GameOver.MovieName3 = WinMovie3;
	if (strcmp(new_event.GameOver.MovieName3, "x") == 0 || strcmp(new_event.GameOver.MovieName3, "X") == 0) {
		new_event.GameOver.MovieName3 = "";
	}

	new_event.GameOver.MovieName4 = WinMovie4;
	if (strcmp(new_event.GameOver.MovieName4, "x") == 0 || strcmp(new_event.GameOver.MovieName4, "X") == 0) {
		new_event.GameOver.MovieName4 = "";
	}

	new_event.GameOver.SabotagedStructureType = SabotagedType;
	new_event.GameOver.TimerRemaining = -1; //Used in RA

	EventCallback(new_event);
}


/**************************************************************************************************
* DLLExportClass::On_Multiplayer_Game_Over -- Called when the C&C multiplayer game finishes
*
*
* History: 6/19/2019 - LLL
* History: 10/25/2019 - MBL - Adding the multi-player score stats support for debrief
**************************************************************************************************/
void DLLExportClass::On_Multiplayer_Game_Over(void)
{
	if (EventCallback == NULL) {
		return;
	}

	GameOver = true;

	EventCallbackStruct event;

	event.EventType = CALLBACK_EVENT_GAME_OVER;

	// Multiplayer players data for debrief stats

	event.GameOver.Multiplayer = true;
	event.GameOver.MultiPlayerTotalPlayers = MPlayerCount;

	for ( int player_index = 0; player_index < MPlayerCount; player_index ++ ) 
	{
		HouseClass* player_ptr = HouseClass::As_Pointer( MPlayerHouses[player_index] );	//HouseClass::As_Pointer(HOUSE_MULTI2);
		if ( player_ptr != NULL )
		{
			int house = player_ptr->Class->House;
			unsigned int leadership = TD_Calculate_Leadership( house, player_ptr->UnitsLost, player_ptr->BuildingsLost );

			unsigned int efficiency = TD_Calculate_Efficiency( player_ptr->HarvestedCredits, player_ptr->InitialCredits, player_ptr->Available_Money() );

		  	unsigned int total_score = TD_Calculate_Score( leadership, efficiency, BuildLevel );

			int units_killed = 0;
			int structures_killed = 0;
			for ( unsigned int house_index = 0; house_index < HOUSE_COUNT; house_index ++ ) 
			{
				units_killed += player_ptr->UnitsKilled[ house_index ];
				structures_killed += player_ptr->BuildingsKilled[ house_index ];
			}

			// Populate and copy the multiplayer player data structure 

			GameOverMultiPlayerStatsStruct multi_player_data;

			multi_player_data.GlyphXPlayerID = Get_GlyphX_Player_ID( player_ptr );
			multi_player_data.IsHuman = (player_ptr->IsHuman || player_ptr->WasHuman);
			multi_player_data.WasHuman = player_ptr->WasHuman;
			multi_player_data.IsWinner = !player_ptr->IsDefeated;
			multi_player_data.Efficiency = efficiency;
			multi_player_data.Score = total_score;
			multi_player_data.ResourcesGathered = player_ptr->HarvestedCredits;
			multi_player_data.TotalUnitsKilled = units_killed;
			multi_player_data.TotalStructuresKilled = structures_killed;

			if ( player_index < GAME_OVER_MULTIPLAYER_MAX_PLAYERS_TRACKED ) 
			{
				event.GameOver.MultiPlayerPlayersData[ player_index ] = multi_player_data;
			}
		}
	}
	for ( int player_index = MPlayerCount; player_index < GAME_OVER_MULTIPLAYER_MAX_PLAYERS_TRACKED; player_index ++ ) 
	{
		memset( &event.GameOver.MultiPlayerPlayersData[ player_index ], 0, sizeof( GameOverMultiPlayerStatsStruct ) );
	}

	// Single-player N/A stuff

	event.GameOver.MovieName = "";
	event.GameOver.MovieName2 = "";
	event.GameOver.MovieName3 = "";
	event.GameOver.MovieName4 = "";
	event.GameOver.AfterScoreMovieName = "";
	event.GameOver.Leadership = 0;
	event.GameOver.Efficiency = 0;
	event.GameOver.Score = 0;
	event.GameOver.NODKilled = 0;
	event.GameOver.GDIKilled = 0;
	event.GameOver.CiviliansKilled = 0;
	event.GameOver.NODBuildingsDestroyed = 0;
	event.GameOver.GDIBuildingsDestroyed = 0;
	event.GameOver.CiviliansBuildingsDestroyed = 0;
	event.GameOver.RemainingCredits = 0;
	event.GameOver.SabotagedStructureType = 0;
	event.GameOver.TimerRemaining = -1;

	// Trigger an event for each human player
	for (int i=0 ; i<MPlayerCount ; i++) {
		HouseClass* player_ptr = HouseClass::As_Pointer(MPlayerHouses[i]);	//HouseClass::As_Pointer(HOUSE_MULTI2);
		if ( player_ptr != NULL )
		{
			if ( player_ptr->IsHuman == true ) 
			{
				event.GlyphXPlayerID = Get_GlyphX_Player_ID(player_ptr);
				event.GameOver.PlayerWins = !player_ptr->IsDefeated;
				event.GameOver.RemainingCredits = player_ptr->Available_Money();
				EventCallback(event);
			}
		}
	}
}


/**************************************************************************************************
* DLLExportClass::On_Message -- Called when the game wants to display a message (ex. tutorial text)
*
* In:
*
* Out:
*
*
*
* History: 10/16/2019 - SKY
**************************************************************************************************/
void DLLExportClass::On_Message(const HouseClass* player_ptr, const char* message, float timeout_seconds, EventCallbackMessageEnum message_type, int64 message_id)
{
	if (EventCallback == NULL)
	{
		return;
	}

	const char* p_msg = message;
	if (message_id != -1) {
		if (message_id == TXT_LOW_POWER) {
			p_msg = "TEXT_LOW_POWER_MESSAGE_001";
		}
		else if (message_id == TXT_INSUFFICIENT_FUNDS) {
			p_msg = "TEXT_INSUFFICIENT_FUNDS_MESSAGE";
		}
	}

	EventCallbackStruct new_event;
	new_event.EventType = CALLBACK_EVENT_MESSAGE;
	new_event.Message.Message = p_msg;
	new_event.Message.TimeoutSeconds = timeout_seconds;
	new_event.Message.MessageType = message_type;
	new_event.Message.MessageParam1 = message_id;

	new_event.GlyphXPlayerID = 0;
	if (player_ptr != NULL)
	{
		new_event.GlyphXPlayerID = Get_GlyphX_Player_ID(player_ptr);
	}

	EventCallback(new_event);
}

void On_Message(const char* message, float timeout_seconds, EventCallbackMessageEnum message_type, int64 message_id)
{
	DLLExportClass::On_Message(PlayerPtr, message, timeout_seconds, message_type, message_id);
}

void On_Message(const char* message, float timeout_seconds, int64 message_id)
{
	DLLExportClass::On_Message(PlayerPtr, message, timeout_seconds, MESSAGE_TYPE_DIRECT, message_id);
}

void On_Defeated_Message(const char* message, float timeout_seconds)
{
	DLLExportClass::On_Message(PlayerPtr, message, timeout_seconds, MESSAGE_TYPE_PLAYER_DEFEATED, -1);
}



/**************************************************************************************************
* DLLExportClass::On_Achievement -- Called when something achievement-related happens
*
* In:   Type of achievement, reason this happened
*
* Out:
*
*
*
* History: 11/11/2019 11:37AM - ST
**************************************************************************************************/
void DLLExportClass::On_Achievement(const HouseClass* player_ptr, const char *achievement_type, const char *achievement_reason)
{
	if (EventCallback == NULL) {
		return;
	}

	EventCallbackStruct new_event;
	new_event.EventType = CALLBACK_EVENT_ACHIEVEMENT;
	new_event.Achievement.AchievementType = achievement_type;
	new_event.Achievement.AchievementReason = achievement_reason;

	new_event.GlyphXPlayerID = 0;
	if (player_ptr != NULL) {
		new_event.GlyphXPlayerID = Get_GlyphX_Player_ID(player_ptr);
	}

	EventCallback(new_event);
}			  


void DLLExportClass::On_Center_Camera(const HouseClass* player_ptr, int coord_x, int coord_y)
{
	if (EventCallback == NULL) {
		return;
	}

	EventCallbackStruct new_event;
	new_event.EventType = CALLBACK_EVENT_CENTER_CAMERA;
	new_event.CenterCamera.CoordX = coord_x;
	new_event.CenterCamera.CoordY = coord_y;

	new_event.GlyphXPlayerID = 0;
	if (player_ptr != NULL) {
		new_event.GlyphXPlayerID = Get_GlyphX_Player_ID(player_ptr);
	}

	EventCallback(new_event);
}


void DLLExportClass::On_Ping(const HouseClass* player_ptr, COORDINATE coord)
{
	if (EventCallback == NULL) {
		return;
	}

	EventCallbackStruct new_event;
	new_event.EventType = CALLBACK_EVENT_PING;
	new_event.Ping.CoordX = Coord_X(coord);
	new_event.Ping.CoordY = Coord_Y(coord);

	new_event.GlyphXPlayerID = 0;
	if (player_ptr != NULL) {
		new_event.GlyphXPlayerID = Get_GlyphX_Player_ID(player_ptr);
	}

	EventCallback(new_event);
}


/**************************************************************************************************
* DLLExportClass::On_Debug_Output -- Called when C&C wants to print debug output
*
* In:   String to print to GlyphX log system
*
* Out:
*
*
*
* History: 2/20/2019 2:39PM - ST
**************************************************************************************************/
void DLLExportClass::On_Debug_Output(const char *debug_text)
{
	if (EventCallback == NULL) {
		return;
	}

	EventCallbackStruct new_event;
	new_event.EventType = CALLBACK_EVENT_DEBUG_PRINT;
	new_event.DebugPrint.PrintString = debug_text;
	EventCallback(new_event);
}			  


/**************************************************************************************************
* DLLExportClass::Force_Human_Team_Wins
*
* History: 3/10/2020 - LL
**************************************************************************************************/
void DLLExportClass::Force_Human_Team_Wins(uint64 quitting_player_id)
{
	if (PlayerWins || PlayerLoses || GameOver) {
		return;
	}

	int winning_team = -1;

	//Find the first human's multiplayer team.
	for (int i = 0; i < MPlayerCount; i++)
	{
		if (GlyphxPlayerIDs[i] != quitting_player_id) {
			HousesType house_type = MPlayerHouses[i];
			HouseClass* house_class = HouseClass::As_Pointer(house_type);
			if (house_class && house_class->IsHuman && !house_class->IsDefeated) {
				winning_team = MPlayerTeamIDs[i];
				break;
			}
		}
	}

	//Mark all players not on that team as defeated.
	for (int i = 0; i < MPlayerCount; i++)
	{
		HousesType house_type = MPlayerHouses[i];
		HouseClass* house_class = HouseClass::As_Pointer(house_type);
		if (house_class) {
			house_class->IsDefeated = MPlayerTeamIDs[i] != winning_team;
		}
	}

	PlayerWins = true;
}


/**************************************************************************************************
* CNC_Get_Game_State -- Get game state
*
* In:   Type of state requested
*       Player perspective
*       Buffer to contain game state
*       Size of buffer
*
* Out:  Game state returned in buffer
*
*
*
* History: 1/7/2019 5:20PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) bool __cdecl CNC_Get_Game_State(GameStateRequestEnum state_type, uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size)
{
	bool got_state = false;

	switch (state_type) {
		
		case GAME_STATE_LAYERS:
		{
			got_state = DLLExportClass::Get_Layer_State(player_id, buffer_in, buffer_size);
			break;
		}		 

		case GAME_STATE_SIDEBAR:
		{	
			got_state = DLLExportClass::Get_Sidebar_State(player_id, buffer_in, buffer_size);
			break;
		}

		case GAME_STATE_PLACEMENT:
		{
			got_state = DLLExportClass::Get_Placement_State(player_id, buffer_in, buffer_size);
			break;
		}			
		
		case GAME_STATE_DYNAMIC_MAP:
			got_state = DLLExportClass::Get_Dynamic_Map_State(player_id, buffer_in, buffer_size);
			break;

		case GAME_STATE_SHROUD:
			got_state = DLLExportClass::Get_Shroud_State(player_id, buffer_in, buffer_size);
			break;

		case GAME_STATE_OCCUPIER:
			got_state = DLLExportClass::Get_Occupier_State(player_id, buffer_in, buffer_size);
			break;

		case GAME_STATE_PLAYER_INFO:
			got_state = DLLExportClass::Get_Player_Info_State(player_id, buffer_in, buffer_size);
			break;

		case GAME_STATE_STATIC_MAP:
		{	
			if (buffer_size < sizeof(CNCMapDataStruct)) {
				got_state = false;
				break;
			}

			int map_cell_x = Map.MapCellX;
			int map_cell_y = Map.MapCellY;
			int map_cell_width = Map.MapCellWidth;
			int map_cell_height = Map.MapCellHeight;

			CNCMapDataStruct *map_data = (CNCMapDataStruct *)buffer_in;
			
			map_data->OriginalMapCellX = map_cell_x;
			map_data->OriginalMapCellY = map_cell_y;
			map_data->OriginalMapCellWidth = map_cell_width;
			map_data->OriginalMapCellHeight = map_cell_height;

			if (map_cell_x > 0) {
				map_cell_x--;
				map_cell_width++;
			}

			if (map_cell_width < MAP_MAX_CELL_WIDTH) {
				map_cell_width++;
			}

			if (map_cell_y > 0) {
				map_cell_y--;
				map_cell_height++;
			}

			if (map_cell_height < MAP_MAX_CELL_HEIGHT) {
				map_cell_height++;
			}

			map_data->MapCellX = map_cell_x;
			map_data->MapCellY = map_cell_y;
			map_data->MapCellWidth = map_cell_width;
			map_data->MapCellHeight = map_cell_height;

			map_data->Theater = (CnCTheaterType) Map.Theater;

			// Hack a fix for scenario 21 since the same mission number is used in Covert Ops and N64
			memset(map_data->ScenarioName, 0, sizeof(map_data->ScenarioName));
			if ((Map.Theater == CNC_THEATER_DESERT) && (Scenario == 21)) {
				strncpy(map_data->ScenarioName, "SCB81EA", sizeof(map_data->ScenarioName) - 1);
			} else {
				strncpy(map_data->ScenarioName, ScenarioName, sizeof(map_data->ScenarioName) - 1);
			}
			
			int cell_index = 0;
			char cell_name[_MAX_PATH];
			char icon_number[32];
				
			for (int y = 0 ; y < map_cell_height ; y++) {
				for (int x = 0 ; x < map_cell_width ; x++) {
					CELL cell = XY_Cell(map_cell_x+x, map_cell_y+y);
					CellClass * cellptr = &Map[cell];

					cell_name[0] = 0;
					int icon = 0;
					void *image_data = 0;
					if (cellptr->Get_Template_Info(cell_name, icon, image_data)) {
						itoa(icon, icon_number, 10);
						strncat(cell_name, "_i", 32);
						strncat(cell_name, icon_number, 32);
						strncat(cell_name, ".tga", 32);
						cell_name[31] = 0;

						CNCStaticCellStruct &cell_info = map_data->StaticCells[cell_index++];
						strncpy(cell_info.TemplateTypeName, cell_name, 32);
						cell_info.TemplateTypeName[31] = 0;
						cell_info.IconNumber = icon;
					}
				}
			}
				
			got_state = true;
			break;
		}

		default:
			{
				got_state = false;
				break;
			}
	}

	return got_state;
}

/**************************************************************************************************
* CNC_Handle_Game_Request
*
* Callback for when the requested movie is done playing. 
*
* 7/23/2019 - LLL
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Game_Request(GameRequestEnum request_type)
{
	switch (request_type)
	{
		case INPUT_GAME_MOVIE_DONE:

		InMainLoop = true;
		break;
	}
}

extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Game_Settings_Request(int health_bar_display_mode, int resource_bar_display_mode)
{
	if (!DLLExportClass::Legacy_Render_Enabled()) {
		return;
	}

	SpecialClass::eHealthBarDisplayMode new_hb_mode = (SpecialClass::eHealthBarDisplayMode)health_bar_display_mode;
	if (new_hb_mode != Special.HealthBarDisplayMode) {
		Special.HealthBarDisplayMode = new_hb_mode;
		Map.Flag_To_Redraw(true);
	}

	SpecialClass::eResourceBarDisplayMode new_rb_mode = (SpecialClass::eResourceBarDisplayMode)resource_bar_display_mode;
	if (new_rb_mode != Special.ResourceBarDisplayMode) {
		Special.ResourceBarDisplayMode = new_rb_mode;
		Map.Flag_To_Redraw(true);
	}
}

	

	

void DLL_Draw_Intercept(int shape_number, int x, int y, int width, int height, int flags, ObjectClass *object, const char *shape_file_name = NULL, char override_owner = HOUSE_NONE, int scale = 0x100)
{
	DLLExportClass::DLL_Draw_Intercept(shape_number, x, y, width, height, flags, object, shape_file_name, override_owner, scale);
}			  


void DLL_Draw_Pip_Intercept(const ObjectClass* object, int pip)
{
	DLLExportClass::DLL_Draw_Pip_Intercept(object, pip);
}


void DLL_Draw_Line_Intercept(int x, int y, int x1, int y1, unsigned char color, int frame)
{
	DLLExportClass::DLL_Draw_Line_Intercept(x, y, x1, y1, color, frame);
}


void DLLExportClass::DLL_Draw_Intercept(int shape_number, int x, int y, int width, int height, int flags, ObjectClass *object, const char *shape_file_name, char override_owner, int scale)
{
	CNCObjectStruct& new_object = ObjectList->Objects[TotalObjectCount + CurrentDrawCount];
	Convert_Type(object, new_object);
	if (new_object.Type == UNKNOWN) {
		return;
	}

	CNCObjectStruct* base_object = NULL;
	for (int i = 0; i < CurrentDrawCount; ++i) {
		CNCObjectStruct& draw_object = ObjectList->Objects[TotalObjectCount + i];
		if (draw_object.CNCInternalObjectPointer == object) {
			base_object = &draw_object;
			break;
		}
	}

	new_object.CNCInternalObjectPointer = (void*)object;
	new_object.OccupyListLength = 0;
	new_object.SortOrder = SortOrder++;

	strncpy(new_object.TypeName, object->Class_Of().IniName, CNC_OBJECT_ASSET_NAME_LENGTH);

	if (shape_file_name != NULL) {
		strncpy(new_object.AssetName, shape_file_name, CNC_OBJECT_ASSET_NAME_LENGTH);
	}
	else {
		strncpy(new_object.AssetName, object->Class_Of().IniName, CNC_OBJECT_ASSET_NAME_LENGTH);
	}

	new_object.TypeName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
	new_object.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
	new_object.Owner = ((base_object != NULL) && (override_owner != HOUSE_NONE)) ? override_owner : (char)object->Owner();

	HouseClass* owner_house = nullptr;
	for (int i = 0; i < Houses.Count(); ++i) {
		HouseClass* hptr = Houses.Ptr(i);
		if ((hptr != nullptr) && (hptr->Class->House == new_object.Owner)) {
			owner_house = hptr;
			break;
		}
	}

	new_object.RemapColor = (owner_house != nullptr) ? owner_house->RemapColor : -1;

	if (base_object == NULL) {
		CNCObjectStruct& root_object = ObjectList->Objects[TotalObjectCount];

		if (new_object.Type == BUILDING) {
			BuildingClass *building = (BuildingClass*)object;
			if (building->BState == BSTATE_CONSTRUCTION) {
				strncat(new_object.AssetName, "MAKE", CNC_OBJECT_ASSET_NAME_LENGTH);
				new_object.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
			}
			const BuildingTypeClass *building_type = building->Class;
			short const *occupy_list = building_type->Occupy_List();
			if (occupy_list) {
				while (*occupy_list != REFRESH_EOL && new_object.OccupyListLength < MAX_OCCUPY_CELLS) {
					new_object.OccupyList[new_object.OccupyListLength] = *occupy_list;
					new_object.OccupyListLength++;
					occupy_list++;
				}
			}
		}

		COORDINATE coord = object->Render_Coord();
		CELL cell = Coord_Cell(coord);
		int dimx, dimy;
		object->Class_Of().Dimensions(dimx, dimy);

		short sim_lepton_x = 0;
		short sim_lepton_y = 0;

		if (new_object.Type == UNIT) {
			sim_lepton_x = ((DriveClass*)object)->SimLeptonX;
			sim_lepton_y = ((DriveClass*)object)->SimLeptonY;
		}

		new_object.PositionX = x;
		new_object.PositionY = y;
		new_object.Width = width;
		new_object.Height = height;
		new_object.Altitude = 0;
		new_object.DrawFlags = flags;
		new_object.SubObject = 0;
		new_object.ShapeIndex = (unsigned short)shape_number;
		new_object.IsTheaterSpecific = IsTheaterShape;
		new_object.Scale = scale;
		new_object.Rotation = 0;
		new_object.FlashingFlags = 0;
		new_object.Cloak = (CurrentDrawCount > 0) ? root_object.Cloak : UNCLOAKED;
		new_object.VisibleFlags = CNCObjectStruct::VISIBLE_FLAGS_ALL;
		new_object.SpiedByFlags = 0U;

		new_object.SortOrder = SortOrder++;
		new_object.IsSelectable = object->Class_Of().IsSelectable;
		new_object.IsSelectedMask = object->IsSelectedMask;
		new_object.MaxStrength = object->Class_Of().MaxStrength;
		new_object.Strength = object->Strength;
		new_object.CellX = (CurrentDrawCount > 0) ? root_object.CellX : Cell_X(cell);
		new_object.CellY = (CurrentDrawCount > 0) ? root_object.CellY : Cell_Y(cell);
		new_object.CenterCoordX = Coord_X(object->Center_Coord());
		new_object.CenterCoordY = Coord_Y(object->Center_Coord());
		new_object.DimensionX = dimx;
		new_object.DimensionY = dimy;
		new_object.SimLeptonX = (CurrentDrawCount > 0) ? root_object.SimLeptonX : sim_lepton_x;
		new_object.SimLeptonY = (CurrentDrawCount > 0) ? root_object.SimLeptonY : sim_lepton_y;
		new_object.BaseObjectID = ((CurrentDrawCount > 0) && (root_object.Type != BUILDING)) ? root_object.ID : 0;
		new_object.BaseObjectType = ((CurrentDrawCount > 0) && (root_object.Type != BUILDING)) ? root_object.Type : UNKNOWN;
		new_object.NumLines = 0;
		new_object.RecentlyCreated = object->IsRecentlyCreated;
		new_object.NumPips = 0;
		new_object.MaxPips = 0;
		new_object.CanDemolish = object->Can_Demolish();
		new_object.CanDemolishUnit = object->Can_Demolish_Unit();
		new_object.CanRepair = object->Can_Repair();
		memset(new_object.CanMove, false, sizeof(new_object.CanMove));
		memset(new_object.CanFire, false, sizeof(new_object.CanFire));
		memset(new_object.ActionWithSelected, DAT_NONE, sizeof(new_object.ActionWithSelected));

		HouseClass* old_player_ptr = PlayerPtr;
		for (int i = 0; i < Houses.Count(); ++i) {
			HouseClass* hptr = Houses.Ptr(i);
			if ((hptr != nullptr) && hptr->IsActive && hptr->IsHuman) {
				HousesType house = hptr->Class->House;
				DynamicVectorClass<ObjectClass*>& selected_objects = CurrentObject.Raw(house);
				if (selected_objects.Count() > 0) {
					Logic_Switch_Player_Context(hptr);
					Convert_Action_Type(Best_Object_Action(selected_objects, object), (selected_objects.Count() == 1) ? selected_objects[0] : NULL, object->As_Target(), new_object.ActionWithSelected[house]);
				}
			}
		}
		Logic_Switch_Player_Context(old_player_ptr);

		RTTIType what_is_object = object->What_Am_I();

		new_object.IsRepairing = false;
		new_object.IsDumping = false;
		new_object.IsALoaner = false;
		new_object.IsFactory = false;
		new_object.IsPrimaryFactory = false;
		new_object.IsAntiGround = false;
		new_object.IsAntiAircraft = false;
		new_object.IsSubSurface = false;
		new_object.IsNominal = false;
		new_object.IsDog = false;
		new_object.IsIronCurtain = false;
		new_object.IsInFormation = false;
		new_object.IsFake = false;
		new_object.ProductionAssetName[0] = '\0';
		new_object.OverrideDisplayName = "\0";

		bool is_building = what_is_object == RTTI_BUILDING;
		if (is_building) {
			BuildingClass* building = static_cast<BuildingClass*>(object);
			new_object.IsRepairing = building->IsRepairing;
			new_object.IsFactory = building->Class->IsFactory;
			new_object.IsPrimaryFactory = building->IsLeader;
		}

		if (object->Is_Techno()) {
			TechnoClass* techno_object = static_cast<TechnoClass*>(object);
			const TechnoTypeClass *ttype = techno_object->Techno_Type_Class();

			new_object.MaxSpeed = (unsigned char)ttype->MaxSpeed;
			new_object.IsALoaner = techno_object->IsALoaner;
			new_object.IsNominal = ttype->IsNominal;
			new_object.MaxPips = ttype->Max_Pips();
			new_object.IsAntiGround = ttype->Primary != WEAPON_NONE;
			new_object.IsAntiAircraft = (ttype->Primary != WEAPON_NONE) && (Weapons[ttype->Primary].Fires != BULLET_NONE) && BulletTypeClass::As_Reference(Weapons[ttype->Primary].Fires).IsAntiAircraft;

			HouseClass* old_player_ptr = PlayerPtr;
			for (int i = 0; i < Houses.Count(); ++i) {
				HouseClass* hptr = Houses.Ptr(i);
				if ((hptr != nullptr) && hptr->IsActive && hptr->IsHuman) {
					Logic_Switch_Player_Context(hptr);
					HousesType house = hptr->Class->House;
					new_object.CanMove[house] = techno_object->Can_Player_Move();
					new_object.CanFire[house] = techno_object->Can_Player_Fire();
				}
			}
			Logic_Switch_Player_Context(old_player_ptr);
		}

		new_object.ControlGroup = (unsigned char)(-1);
		new_object.CanPlaceBombs = false;
		bool is_infantry = what_is_object == RTTI_INFANTRY;
		if (is_infantry) {
			InfantryClass* infantry = static_cast<InfantryClass*>(object);
			new_object.ControlGroup = infantry->Group;
			new_object.CanPlaceBombs = infantry->Class->Type == INFANTRY_RAMBO;
		}

		new_object.CanHarvest = false;
		bool is_unit = what_is_object == RTTI_UNIT;
		if (is_unit) {
			UnitClass* unit = static_cast<UnitClass*>(object);
			if (unit->Class->Type == UNIT_HARVESTER) {
				new_object.CanHarvest = true;
			}

			new_object.ControlGroup = unit->Group;
		}

		new_object.IsFixedWingedAircraft = false;
		bool is_aircraft = what_is_object == RTTI_AIRCRAFT;
		if (is_aircraft) {
			AircraftClass* aircraft = static_cast<AircraftClass*>(object);
			new_object.Altitude = Pixel_To_Lepton(aircraft->Altitude);
			new_object.IsFixedWingedAircraft = aircraft->Class->IsFixedWing;
			new_object.ControlGroup = aircraft->Group;
		}

		switch (what_is_object)
		{
			case RTTI_INFANTRY:
			case RTTI_INFANTRYTYPE:
			case RTTI_UNIT:
			case RTTI_UNITTYPE:
			case RTTI_AIRCRAFT:
			case RTTI_AIRCRAFTTYPE:
			case RTTI_BUILDING:
			case RTTI_BUILDINGTYPE:
			{
				TechnoClass* techno_object = static_cast<TechnoClass*>(object);
				new_object.FlashingFlags = techno_object->Get_Flashing_Flags();
				new_object.Cloak = techno_object->Cloak;
			}
			break;

			case RTTI_ANIM:
			{
				AnimClass* anim_object = static_cast<AnimClass*>(object);
				new_object.VisibleFlags = anim_object->Get_Visible_Flags();
			}
			break;
		}
	}
	else {
		new_object.MaxStrength = 0;
		new_object.MaxSpeed = 0;
		new_object.Strength = 0;
		new_object.CellX = base_object->CellX;
		new_object.CellY = base_object->CellY;
		new_object.CenterCoordX = base_object->CenterCoordX;
		new_object.CenterCoordY = base_object->CenterCoordY;
		new_object.DimensionX = base_object->DimensionX;
		new_object.DimensionY = base_object->DimensionY;
		new_object.IsSelectable = false;
		new_object.IsSelectedMask = 0U;
		new_object.SimLeptonX = base_object->SimLeptonX;
		new_object.SimLeptonY = base_object->SimLeptonY;

		new_object.PositionX = x;
		new_object.PositionY = y;
		new_object.Width = width;
		new_object.Height = height;
		new_object.Altitude = base_object->Altitude;
		new_object.DrawFlags = flags;
		new_object.ShapeIndex = (unsigned short)shape_number;
		new_object.IsTheaterSpecific = IsTheaterShape;
		new_object.Scale = scale;
		new_object.Rotation = 0;
		new_object.SubObject = CurrentDrawCount;
		new_object.BaseObjectID = base_object->ID;
		new_object.BaseObjectType = base_object->Type;
		new_object.FlashingFlags = base_object->FlashingFlags;
		new_object.Cloak = base_object->Cloak;
		new_object.OccupyListLength = 0;
		new_object.NumPips = 0;
		new_object.MaxPips = 0;
		new_object.IsRepairing = false;
		new_object.IsDumping = false;
		new_object.IsALoaner = base_object->IsALoaner;
		new_object.NumLines = 0;
		new_object.CanDemolish = base_object->CanDemolish;
		new_object.CanDemolishUnit = base_object->CanDemolishUnit;
		new_object.CanRepair = base_object->CanRepair;
		new_object.RecentlyCreated = base_object->RecentlyCreated;
		new_object.IsFactory = base_object->IsFactory;
		new_object.IsPrimaryFactory = base_object->IsPrimaryFactory;
		new_object.IsAntiGround = base_object->IsAntiGround;
		new_object.IsAntiAircraft = base_object->IsAntiAircraft;
		new_object.IsSubSurface = base_object->IsSubSurface;
		new_object.IsNominal = base_object->IsNominal;
		new_object.IsDog = base_object->IsDog;
		new_object.IsIronCurtain = base_object->IsIronCurtain;
		new_object.IsInFormation = false;
		new_object.CanHarvest = base_object->CanHarvest;
		new_object.CanPlaceBombs = base_object->CanPlaceBombs;
		new_object.ControlGroup = base_object->ControlGroup;
		new_object.VisibleFlags = base_object->VisibleFlags;
		new_object.SpiedByFlags = base_object->SpiedByFlags;
		new_object.IsFixedWingedAircraft = base_object->IsFixedWingedAircraft;
		new_object.IsFake = base_object->IsFake;
		new_object.ProductionAssetName[0] = '\0';
		new_object.OverrideDisplayName = "\0";
		memset(new_object.CanMove, false, sizeof(new_object.CanMove));
		memset(new_object.CanFire, false, sizeof(new_object.CanFire));
		memset(new_object.ActionWithSelected, DAT_NONE, sizeof(new_object.ActionWithSelected));
	}

	CurrentDrawCount++;
}			  



void DLLExportClass::DLL_Draw_Pip_Intercept(const ObjectClass* object, int pip)
{
	CNCObjectStruct* base_object = NULL;
	for (int i = 0; i < CurrentDrawCount; ++i) {
		CNCObjectStruct& draw_object = ObjectList->Objects[TotalObjectCount + i];
		if (draw_object.CNCInternalObjectPointer == object) {
			base_object = &draw_object;
			break;
		}
	}

	if ((base_object != NULL) && (base_object->NumPips < MAX_OBJECT_PIPS)) {
		base_object->Pips[base_object->NumPips] = pip;
		base_object->NumPips++;
		base_object->MaxPips = max(base_object->MaxPips, base_object->NumPips);
	}
}



void DLLExportClass::DLL_Draw_Line_Intercept(int x, int y, int x1, int y1, unsigned char color, int frame)
{
	CNCObjectStruct& root_object = ObjectList->Objects[TotalObjectCount];
	if (root_object.NumLines < MAX_OBJECT_LINES) {
		root_object.Lines[root_object.NumLines].X = x;
		root_object.Lines[root_object.NumLines].Y = y;
		root_object.Lines[root_object.NumLines].X1 = x1;
		root_object.Lines[root_object.NumLines].Y1 = y1;
		root_object.Lines[root_object.NumLines].Frame = frame;
		root_object.Lines[root_object.NumLines].Color = color;

		SortOrder++;
		root_object.NumLines++;
	}
}



/**************************************************************************************************
* DLLExportClass::Get_Layer_State -- Get game objects from the layers
*
* In:   
*
* Out:  
*
*
*
* History: 1/29/2019 11:37AM - ST
**************************************************************************************************/
bool DLLExportClass::Get_Layer_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size)
{
	player_id;

	static int _export_count = 0;

	bool got_state = false;

	ObjectList = (CNCObjectListStruct*) buffer_in;

	TotalObjectCount = 0;

	/*
	** Get a reference draw coordinate for cells
	*/
	int map_cell_x = Map.MapCellX;
	int map_cell_y = Map.MapCellY;
	if (map_cell_x > 0) {
		map_cell_x--;
	}
	if (map_cell_y > 0) {
		map_cell_y--;
	}

	SortOrder = 0;

	/*
	**	Get the ground layer first and then followed by all the layers in increasing altitude.
	*/
	for (int layer = 0; layer < DLL_LAYER_COUNT; layer++) {
		
		for (int index = 0; index < Map.Layer[layer].Count(); index++) {
			
			ObjectClass *object = Map.Layer[layer][index];
			if (object->IsActive) {
				
				unsigned int memory_needed = sizeof(CNCObjectListStruct);
				memory_needed += (TotalObjectCount + 10) * sizeof(CNCObjectStruct);
				if (memory_needed >= buffer_size) {
					return false;
				}
				
				if (object->Is_Techno()) {
					/*
					**  Skip units tethered to buildings, since the building will draw them itself
					*/
					TechnoClass* techno_object = static_cast<TechnoClass*>(object);
					TechnoClass* contact_object = techno_object->In_Radio_Contact() ? techno_object->Contact_With_Whom() : nullptr;
					if ((contact_object != nullptr) && (contact_object->What_Am_I() == RTTI_BUILDING) && contact_object->IsTethered && *((BuildingClass*)contact_object) == STRUCT_WEAP) {
						continue;
					}
				}

				if (Debug_Map || Debug_Unshroud || (object->IsDown && !object->IsInLimbo)) {
					int	x, y;
					Map.Coord_To_Pixel(object->Render_Coord(), x, y);

					/*
					** Call to Draw_It can result in multiple callbacks to the draw intercept
					*/
					CurrentDrawCount = 0;
					object->Draw_It(x, y, WINDOW_VIRTUAL);

					/*
					** Shadows need to be rendered before the base object so they appear underneath,
					** even though they get drawn as sub-objects (after the base object)
					*/
					for (int i = 1; i < CurrentDrawCount; ++i) {
						CNCObjectStruct& sub_object = ObjectList->Objects[TotalObjectCount + i];
						if (!sub_object.SubObject) {
							continue;
						}
						static const int shadow_flags = SHAPE_PREDATOR | SHAPE_FADING;
						if (((sub_object.DrawFlags & shadow_flags) == shadow_flags) || (strncmp(sub_object.AssetName, "WAKE", CNC_OBJECT_ASSET_NAME_LENGTH) == 0)) {
							if ((strncmp(sub_object.AssetName, "RROTOR", CNC_OBJECT_ASSET_NAME_LENGTH) != 0) &&
								(strncmp(sub_object.AssetName, "LROTOR", CNC_OBJECT_ASSET_NAME_LENGTH) != 0)) {
								for (int j = i - 1; j >= 0; --j) {
									CNCObjectStruct& base_object = ObjectList->Objects[TotalObjectCount + j];
									if (!base_object.SubObject && (base_object.CNCInternalObjectPointer == sub_object.CNCInternalObjectPointer)) {
										int sort_order = base_object.SortOrder;
										base_object.SortOrder = sub_object.SortOrder;
										sub_object.SortOrder = sort_order;
										break;
									}
								}
							}
						}
					}

					TotalObjectCount += CurrentDrawCount;
				}
			}
		}
	}

	ObjectList->Count = TotalObjectCount;

	if (ObjectList->Count) {
		_export_count++;
		return true;
	}

	return false;
}




void DLLExportClass::Convert_Type(const ObjectClass *object, CNCObjectStruct &object_out)
{	
	object_out.Type = UNKNOWN;
	object_out.ID = -1;
	
	if (object == NULL) {
		return;
	}	  

	RTTIType type = object->What_Am_I();
	
	switch (type) {
		default:
			break;

		case RTTI_INFANTRY:
			object_out.Type = INFANTRY;
			object_out.ID = Infantry.ID((InfantryClass*)object);
			break;
						
		case RTTI_UNIT:
			object_out.Type = UNIT;
			object_out.ID = Units.ID((UnitClass*)object);
			break;
						
		case RTTI_AIRCRAFT:
			object_out.Type = AIRCRAFT;
			object_out.ID = Aircraft.ID((AircraftClass*)object);
			break;
					
		case RTTI_BUILDING:
			object_out.Type = BUILDING;
			object_out.ID = Buildings.ID((BuildingClass*)object);
			break;

		case RTTI_BULLET:
			object_out.Type = BULLET;
			object_out.ID = Bullets.ID((BulletClass*)object);
			break;

		case RTTI_ANIM:
			object_out.Type = ANIM;
			object_out.ID = Anims.ID((AnimClass*)object);
			break;

		case RTTI_SMUDGE:
			object_out.Type = SMUDGE;
			object_out.ID = Smudges.ID((SmudgeClass*)object);
			break;

		case RTTI_TERRAIN:
			object_out.Type = TERRAIN;
			object_out.ID = Terrains.ID((TerrainClass*)object);
			break;
	}
}






/**************************************************************************************************
* CNC_Handle_Input -- Process input to the game
*
* In:   
*       
*       
*       
*
* Out:  Game state returned in buffer
*
*
*
* History: 1/7/2019 5:20PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Input(InputRequestEnum input_event, unsigned char special_key_flags, uint64 player_id, int x1, int y1, int x2, int y2)
{
	
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	switch (input_event) {

		/*
		** Special keys have changed
		*/
		case INPUT_REQUEST_SPECIAL_KEYS:
		{
			DLLExportClass::Set_Special_Key_Flags(special_key_flags);
			break;
		}

		/*
		** The mouse is moving
		*/
		case INPUT_REQUEST_MOUSE_MOVE:
		{	
			if (!DLLExportClass::Legacy_Render_Enabled()) {
				break;
			}
			
			DLLForceMouseX = x1;
			DLLForceMouseY = y1;
			_Kbd->MouseQX = x1;
			_Kbd->MouseQY = y1;

			COORDINATE coord = Map.Pixel_To_Coord(x1, y1);
			CELL cell = Coord_Cell(coord);
			if (coord) {
				//x -= Map.TacPixelX;
				//y -= Map.TacPixelY;

				/*
				** Cause any displayed cursor to move along with the mouse cursor.
				*/
				if (cell != Map.ZoneCell) {
					Map.Set_Cursor_Pos(cell);
				}
			}
			break;
		}

		/*
		** Player left-clicked
		*/
		case INPUT_REQUEST_MOUSE_LEFT_CLICK:
		{
			DLLExportClass::Adjust_Internal_View();
			
			DLLForceMouseX = x1;
			DLLForceMouseY = y1;
			_Kbd->MouseQX = x1;
			_Kbd->MouseQY = y1;

			KeyNumType key = (KeyNumType)(KN_LMOUSE | KN_RLSE_BIT);

			if (Map.Pixel_To_Coord(x1, y1)) {
				//DisplayClass::TacButton.Clicked_On(key, GadgetClass::LEFTRELEASE, x1, y1);
				DisplayClass::TacButton.Clicked_On(key, GadgetClass::LEFTRELEASE, 100, 100);
			}
			break;
		}	
		
		/*
		** Player right-clicked (on up)
		*/
		case INPUT_REQUEST_MOUSE_RIGHT_CLICK:
		{
			DLLExportClass::Adjust_Internal_View();
			
			DLLForceMouseX = x1;
			DLLForceMouseY = y1;
			_Kbd->MouseQX = x1;
			_Kbd->MouseQY = y1;

			KeyNumType key = (KeyNumType)(KN_RMOUSE | KN_RLSE_BIT);

			if (Map.Pixel_To_Coord(x1, y1)) {
				//DisplayClass::TacButton.Clicked_On(key, GadgetClass::RIGHTRELEASE, x1, y1);
				DisplayClass::TacButton.Clicked_On(key, GadgetClass::RIGHTRELEASE, 100, 100);
			}
			break;
		}	

		/*
		** Player right button down
		*/
		case INPUT_REQUEST_MOUSE_RIGHT_DOWN:
		{
			DLLExportClass::Adjust_Internal_View();
			
			DLLForceMouseX = x1;
			DLLForceMouseY = y1;
			_Kbd->MouseQX = x1;
			_Kbd->MouseQY = y1;

			KeyNumType key = (KeyNumType)(KN_RMOUSE);

			if (Map.Pixel_To_Coord(x1, y1)) {
				//DisplayClass::TacButton.Clicked_On(key, GadgetClass::RIGHTPRESS, x1, y1);
				DisplayClass::TacButton.Clicked_On(key, GadgetClass::RIGHTPRESS, 100, 100);
			}
			break;
		}	

			
		/*
		** Player drag selected
		*/
		case INPUT_REQUEST_MOUSE_AREA:
		{
			DLLExportClass::Adjust_Internal_View();
			Map.Select_These(XYPixel_Coord(x1, y1), XYPixel_Coord(x2, y2), false);
			break;
		}

		case INPUT_REQUEST_MOUSE_AREA_ADDITIVE:
		{
			DLLExportClass::Adjust_Internal_View();
			Map.Select_These(XYPixel_Coord(x1, y1), XYPixel_Coord(x2, y2), true);
			break;
		}

		case INPUT_REQUEST_SELL_AT_POSITION:
		{
			DLLExportClass::Adjust_Internal_View();
			DLLForceMouseX = x1;
			DLLForceMouseY = y1;
			_Kbd->MouseQX = x1;
			_Kbd->MouseQY = y1;

			COORDINATE coord = Map.Pixel_To_Coord(x1, y1);
			CELL cell = Coord_Cell(coord);

			if (Map.Pixel_To_Coord(x1, y1))
			{
				PlayerPtr->Sell_Wall(cell);
			}

			break;
		}

		case INPUT_REQUEST_SELECT_AT_POSITION:
		{
			DLLExportClass::Adjust_Internal_View();
			DLLForceMouseX = x1;
			DLLForceMouseY = y1;
			_Kbd->MouseQX = x1;
			_Kbd->MouseQY = y1;

			COORDINATE coord = Map.Pixel_To_Coord(x1, y1);
			CELL cell = Coord_Cell(coord);

			if (Map.Pixel_To_Coord(x1, y1))
			{
				KeyNumType key = (KeyNumType)(KN_LMOUSE | KN_RLSE_BIT);

				DisplayClass::TacButton.Selection_At_Mouse(GadgetClass::LEFTRELEASE, key);
			}

			break;
		}

		case INPUT_REQUEST_COMMAND_AT_POSITION:
		{
			DLLExportClass::Adjust_Internal_View();
			DLLForceMouseX = x1;
			DLLForceMouseY = y1;
			_Kbd->MouseQX = x1;
			_Kbd->MouseQY = y1;

			COORDINATE coord = Map.Pixel_To_Coord(x1, y1);
			CELL cell = Coord_Cell(coord);

			if (Map.Pixel_To_Coord(x1, y1))
			{
				KeyNumType key = (KeyNumType)(KN_LMOUSE | KN_RLSE_BIT);
				DisplayClass::TacButton.Command_Object(GadgetClass::LEFTRELEASE, key);
			}

			break;
		}
		  		  
		default:
			break;
	}
}			




/**************************************************************************************************
* CNC_Handle_Structure_Request -- Process requests to repair and sell structures.
*
* In:
*
*
* Out:
*
*
*
* History: 4/29/2019 - LLL
**************************************************************************************************/

extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Structure_Request(StructureRequestEnum request_type, uint64 player_id, int object_id)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	switch (request_type) 
	{
	case INPUT_STRUCTURE_REPAIR_START: 
		DLLExportClass::Repair_Mode(player_id);
		break;
	case INPUT_STRUCTURE_REPAIR:
		DLLExportClass::Repair(player_id, object_id);
		break;
	case INPUT_STRUCTURE_SELL_START: 
		DLLExportClass::Sell_Mode(player_id);
		break;
	case INPUT_STRUCTURE_SELL:
		DLLExportClass::Sell(player_id, object_id);
		break;
	case INPUT_STRUCTURE_CANCEL:
		DLLExportClass::Repair_Sell_Cancel(player_id);
		break;
	default:
		break;
	}
}



/**************************************************************************************************
* CNC_Handle_Unit_Request -- Process requests on selected units.
*
* In:
*
*
* Out:
*
*
*
* History: 10/15/2019 - SKY
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Unit_Request(UnitRequestEnum request_type, uint64 player_id)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	switch (request_type)
	{
		case INPUT_UNIT_SCATTER:
			DLLExportClass::Scatter_Selected(player_id);
			break;
		case INPUT_UNIT_SELECT_NEXT:
			DLLExportClass::Select_Next_Unit(player_id);
			break;
		case INPUT_UNIT_SELECT_PREVIOUS:
			DLLExportClass::Select_Previous_Unit(player_id);
			break;
		case INPUT_UNIT_GUARD_MODE:
			DLLExportClass::Selected_Guard_Mode(player_id);
			break;
		case INPUT_UNIT_STOP:
			DLLExportClass::Selected_Stop(player_id);
			break;
		case INPUT_UNIT_FORMATION_TOGGLE:
			DLLExportClass::Team_Units_Formation_Toggle_On(player_id);
			break;
		case INPUT_UNIT_QUEUED_MOVEMENT_ON:
			// Red Alert Only
			DLLExportClass::Units_Queued_Movement_Toggle(player_id, true);
			break;
		case INPUT_UNIT_QUEUED_MOVEMENT_OFF:
			// Red Alert Only
			DLLExportClass::Units_Queued_Movement_Toggle(player_id, false);
			break;
		default:
			break;
	}
}



/**************************************************************************************************
* CNC_Handle_Sidebar_Request -- Process an input request to the sidebar
*
* In:   
*       
*       
* Out:
*
*
*
* History: 1/7/2019 5:20PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Sidebar_Request(SidebarRequestEnum request_type, uint64 player_id, int buildable_type, int buildable_id, short cell_x, short cell_y)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}
	
	switch (request_type) {
		
		case SIDEBAR_REQUEST_START_CONSTRUCTION:
			DLLExportClass::Start_Construction(player_id, buildable_type, buildable_id);
			break;
				
		case SIDEBAR_REQUEST_HOLD_CONSTRUCTION:
			DLLExportClass::Hold_Construction(player_id, buildable_type, buildable_id);
			break;
			
		case SIDEBAR_REQUEST_CANCEL_CONSTRUCTION:
			DLLExportClass::Cancel_Construction(player_id, buildable_type, buildable_id);
			break;

		case SIDEBAR_REQUEST_START_PLACEMENT:
			DLLExportClass::Start_Placement(player_id, buildable_type, buildable_id);
			break;
			
		case SIDEBAR_REQUEST_PLACE:
			DLLExportClass::Place(player_id, buildable_type, buildable_id, cell_x, cell_y);
			break;

		case SIDEBAR_CANCEL_PLACE:
			DLLExportClass::Cancel_Placement(player_id, buildable_type, buildable_id);
			break;

		default:
			break;
	}
}			  

/**************************************************************************************************
* CNC_Handle_SuperWeapon_Request
*
* In:
*
*
* Out:
*
*
*
* History: 
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_SuperWeapon_Request(SuperWeaponRequestEnum request_type, uint64 player_id, int buildable_type, int buildable_id, int x1, int y1)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	switch (request_type)
	{
	case SUPERWEAPON_REQUEST_PLACE_SUPER_WEAPON:
		DLLExportClass::Place_Super_Weapon(player_id, buildable_type, buildable_id, x1, y1);
		break;
	}
}

/**************************************************************************************************
* CNC_Handle_ControlGroup_Request
*
* In:
*
*
* Out:
*
*
*
* History:
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_ControlGroup_Request(ControlGroupRequestEnum request_type, uint64 player_id, unsigned char control_group_index)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	switch (request_type)
	{
	case CONTROL_GROUP_REQUEST_CREATE:
		DLLExportClass::Create_Control_Group(control_group_index);
		break;
	case CONTROL_GROUP_REQUEST_TOGGLE:
		DLLExportClass::Toggle_Control_Group_Selection(control_group_index);
		break;
	case CONTROL_GROUP_REQUEST_ADDITIVE_SELECTION:
		DLLExportClass::Add_To_Control_Group(control_group_index);
		break;

	}
}




/**************************************************************************************************
* DLLExportClass::Get_Layer_State -- Get a snapshot of the sidebar state
*
* In:   
*
* Out:  
*
*
*
* History: 1/29/2019 11:37AM - ST
**************************************************************************************************/
bool DLLExportClass::Get_Sidebar_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return false;
	}

	CNCSidebarStruct *sidebar = (CNCSidebarStruct*) buffer_in;
	
	unsigned int memory_needed = sizeof(*sidebar);	// Base amount needed. Will need more depending on how many entries there are
	
	int entry_index = 0;

	sidebar->Credits = 0;
	sidebar->CreditsCounter = 0;
	sidebar->Tiberium = 0;
	sidebar->MaxTiberium = 0;
	sidebar->PowerProduced = 0;
	sidebar->PowerDrained = 0;
	sidebar->RepairBtnEnabled = false;
	sidebar->SellBtnEnabled = false;
	sidebar->RadarMapActive = false;
	sidebar->MissionTimer = -1;

	sidebar->UnitsKilled = 0;
	sidebar->BuildingsKilled = 0;
	sidebar->UnitsLost = 0;
	sidebar->BuildingsLost = 0;
	sidebar->TotalHarvestedCredits = 0;

	if (PlayerPtr) {
		sidebar->Credits = PlayerPtr->Credits;
		sidebar->CreditsCounter = PlayerPtr->VisibleCredits.Current;		// Timed display
		// sidebar->CreditsCounter = PlayerPtr->VisibleCredits.Credits;	// Actual 
		sidebar->Tiberium = PlayerPtr->Tiberium;
		sidebar->MaxTiberium = PlayerPtr->Capacity;
		sidebar->PowerProduced = PlayerPtr->Power;
		sidebar->PowerDrained = PlayerPtr->Drain;

		sidebar->RepairBtnEnabled = PlayerPtr->BScan > 0;
		sidebar->SellBtnEnabled = PlayerPtr->BScan > 0;
		sidebar->RadarMapActive = PlayerPtr->Radar == RADAR_ON;


		// A. Get the DestroyedBuildings and DestroyedInfantry stats if they are available at this point
		if (PlayerPtr->DestroyedBuildings) {
			for ( int index = 0; index < PlayerPtr->DestroyedBuildings->Get_Unit_Count(); index ++ )
			{
				unsigned int count = (unsigned int) PlayerPtr->DestroyedBuildings->Get_Unit_Total( index );
				sidebar->BuildingsKilled += count;
			}
		}
		if (PlayerPtr->DestroyedInfantry) {
			for ( int index = 0; index < PlayerPtr->DestroyedInfantry->Get_Unit_Count(); index ++ )
			{
				unsigned int count = (unsigned int) PlayerPtr->DestroyedInfantry->Get_Unit_Total( index );
				sidebar->UnitsKilled += count; // Includes Infantry, Vehicles, Aircraft
			}
		}
		if (PlayerPtr->DestroyedUnits) {
			for ( int index = 0; index < PlayerPtr->DestroyedUnits->Get_Unit_Count(); index ++ )
			{
				unsigned int count = (unsigned int) PlayerPtr->DestroyedUnits->Get_Unit_Total( index );
				sidebar->UnitsKilled += count; // Includes Infantry, Vehicles, Aircraft
			}
		}
		if (PlayerPtr->DestroyedAircraft) {
			for ( int index = 0; index < PlayerPtr->DestroyedAircraft->Get_Unit_Count(); index ++ )
			{
				unsigned int count = (unsigned int) PlayerPtr->DestroyedAircraft->Get_Unit_Total( index );
				sidebar->UnitsKilled += count; // Includes Infantry, Vehicles, Aircraft
			}
		}

		// B. If the DestroyedBuildings and DestroyedInfantry stats seemed to be unvailable, this is another way to do it
		// Note that we need to do both of these depending on which type of match we are running, as well as for Replays/Observer and live stats reporting
		// We can't just do it this way for everything, as it does not work for all cases
		if (sidebar->BuildingsKilled == 0)
		{
			for (unsigned int house_index = 0; house_index < HOUSE_COUNT; house_index ++)
			{
				sidebar->BuildingsKilled += PlayerPtr->BuildingsKilled[ house_index ];
			}
		}
		if (sidebar->UnitsKilled == 0)
		{
			for (unsigned int house_index = 0; house_index < HOUSE_COUNT; house_index ++)
			{
				sidebar->UnitsKilled += PlayerPtr->UnitsKilled[ house_index ]; // Includes Infantry, Vehicles, Aircraft
			}
		}


		sidebar->UnitsLost = PlayerPtr->UnitsLost;
		sidebar->BuildingsLost = PlayerPtr->BuildingsLost;
		sidebar->TotalHarvestedCredits = PlayerPtr->HarvestedCredits;
	}

	if (GameToPlay == GAME_NORMAL) {

		/*
		** Get each sidebar column
		*/
		for (int c = 0 ; c < 2 ; c++) {
		
			sidebar->EntryCount[c] = Map.Column[c].BuildableCount;
				
			/*
			** Each production slot in the column
			*/
			for (int b=0 ; b < Map.Column[c].BuildableCount ; b++) {
			
				CNCSidebarEntryStruct &sidebar_entry = sidebar->Entries[entry_index++];
				if ((entry_index + 1) * sizeof(CNCSidebarEntryStruct) + memory_needed > buffer_size) {
					return false;
				}

				sidebar_entry.AssetName[0] = 0;
				sidebar_entry.Type = UNKNOWN;
				sidebar_entry.BuildableID = Map.Column[c].Buildables[b].BuildableID;
				sidebar_entry.BuildableType = Map.Column[c].Buildables[b].BuildableType;
				sidebar_entry.BuildableViaCapture = Map.Column[c].Buildables[b].BuildableViaCapture;
			   sidebar_entry.Fake = false;
			
				TechnoTypeClass const * tech = Fetch_Techno_Type(Map.Column[c].Buildables[b].BuildableType, Map.Column[c].Buildables[b].BuildableID);
				
				sidebar_entry.SuperWeaponType = SW_NONE;

				if (tech) {
					sidebar_entry.Cost = tech->Cost * PlayerPtr->CostBias;
					sidebar_entry.PowerProvided = 0;
					sidebar_entry.BuildTime = tech->Time_To_Build(PlayerPtr->Class->House);
					strncpy(sidebar_entry.AssetName, tech->IniName, CNC_OBJECT_ASSET_NAME_LENGTH);
					sidebar_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
				} else {
					sidebar_entry.Cost = 0;
					sidebar_entry.AssetName[0] = 0;
				}	
				
				SuperClass* super_weapon = nullptr;

				bool isbusy = false;

				switch (Map.Column[c].Buildables[b].BuildableType) {
					case RTTI_INFANTRYTYPE:
						sidebar_entry.Type = INFANTRY_TYPE;
						isbusy = (PlayerPtr->InfantryFactory != -1);
						isbusy |= Infantry.Avail() <= 0;
						break;
						
					case RTTI_UNITTYPE:
						isbusy = (PlayerPtr->UnitFactory != -1);
						sidebar_entry.Type = UNIT_TYPE;
						isbusy |= Units.Avail() <= 0;
						break;
						
					case RTTI_AIRCRAFTTYPE:
						isbusy = (PlayerPtr->AircraftFactory != -1);
						sidebar_entry.Type = AIRCRAFT_TYPE;
						isbusy |= Aircraft.Avail() <= 0;
						break;
					
					case RTTI_BUILDINGTYPE:
					{
						isbusy = (PlayerPtr->BuildingFactory != -1);
						isbusy |= Buildings.Avail() <= 0;
						sidebar_entry.Type = BUILDING_TYPE;

						const BuildingTypeClass* build_type = static_cast<const BuildingTypeClass*>(tech);
						sidebar_entry.PowerProvided = build_type->Power - build_type->Drain;
					}
						break;

					default:
						sidebar_entry.Type = UNKNOWN;
						break;

					case RTTI_SPECIAL:
						switch (Map.Column[c].Buildables[b].BuildableID) 
						{
						case SPC_ION_CANNON:
							sidebar_entry.SuperWeaponType = SW_ION_CANNON;
							sidebar_entry.Type = SPECIAL;
							strncpy(sidebar_entry.AssetName, "SW_Ion", CNC_OBJECT_ASSET_NAME_LENGTH);
							sidebar_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
							super_weapon = &PlayerPtr->IonCannon;
							break;

						case SPC_NUCLEAR_BOMB:
							sidebar_entry.SuperWeaponType = SW_NUKE;
							sidebar_entry.Type = SPECIAL;
							strncpy(sidebar_entry.AssetName, "SW_Nuke", CNC_OBJECT_ASSET_NAME_LENGTH);
							sidebar_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
							super_weapon = &PlayerPtr->NukeStrike;
							break;

						case SPC_AIR_STRIKE:
							sidebar_entry.SuperWeaponType = SW_AIR_STRIKE;
							sidebar_entry.Type = SPECIAL;
							strncpy(sidebar_entry.AssetName, "SW_AirStrike", CNC_OBJECT_ASSET_NAME_LENGTH);
							sidebar_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
							super_weapon = &PlayerPtr->AirStrike;
							break;

						default:
							sidebar_entry.SuperWeaponType = SW_UNKNOWN;
							sidebar_entry.Type = SPECIAL;
							break;
						}
						break;
				}

				int fnumber = Map.Column[c].Buildables[b].Factory;
				FactoryClass * factory = NULL;
				if (tech && fnumber != -1) {
					factory = Factories.Raw_Ptr(fnumber);
				}

				if (super_weapon != nullptr)
				{
					sidebar_entry.Progress = (float)super_weapon->Anim_Stage() / (float)SuperClass::ANIMATION_STAGES;
					sidebar_entry.Completed = super_weapon->Is_Ready();
					sidebar_entry.Constructing = super_weapon->Anim_Stage() != SuperClass::ANIMATION_STAGES;
					sidebar_entry.ConstructionOnHold = false;
					sidebar_entry.PlacementListLength = 0;
					sidebar_entry.PowerProvided = 0;
					sidebar_entry.BuildTime = super_weapon->Get_Recharge_Time();
				}
				else
				{

					sidebar_entry.Completed = false;
					sidebar_entry.Constructing = false;
					sidebar_entry.ConstructionOnHold = false;
					sidebar_entry.Progress = 0.0f;
					sidebar_entry.Busy = isbusy;
					sidebar_entry.PlacementListLength = 0;

					if (factory) {
						if (factory->Is_Building()) {
							sidebar_entry.Constructing = true;
							sidebar_entry.Progress = (float)factory->Completion() / (float)FactoryClass::STEP_COUNT;
							sidebar_entry.Completed = factory->Has_Completed();
						}
						else {
							sidebar_entry.Completed = factory->Has_Completed();
							if (!sidebar_entry.Completed)
							{
								sidebar_entry.ConstructionOnHold = true;
								sidebar_entry.Progress = (float)factory->Completion() / (float)FactoryClass::STEP_COUNT;
							}

							if (sidebar_entry.Completed && sidebar_entry.Type == BUILDING_TYPE) {
								if (tech) {
									BuildingTypeClass *building_type = (BuildingTypeClass*)tech;
									short const *occupy_list = building_type->Occupy_List(true);
									if (occupy_list) {
										while (*occupy_list != REFRESH_EOL && sidebar_entry.PlacementListLength < MAX_OCCUPY_CELLS) {
											sidebar_entry.PlacementList[sidebar_entry.PlacementListLength] = *occupy_list;
											sidebar_entry.PlacementListLength++;
											occupy_list++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	
	} else {
		
		
		if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
			
			SidebarGlyphxClass *context_sidebar = DLLExportClass::Get_Current_Context_Sidebar();
			
			/*
			** Get each sidebar column
			*/
			for (int c = 0 ; c < 2 ; c++) {
		
				sidebar->EntryCount[c] = context_sidebar->Column[c].BuildableCount;
				
				/*
				** Each production slot in the column
				*/
				for (int b=0 ; b < context_sidebar->Column[c].BuildableCount ; b++) {
			
					CNCSidebarEntryStruct &sidebar_entry = sidebar->Entries[entry_index++];
					if ((entry_index + 1) * sizeof(CNCSidebarEntryStruct) + memory_needed > buffer_size) {
						return false;
					}

					sidebar_entry.AssetName[0] = 0;
					sidebar_entry.Type = UNKNOWN;
					sidebar_entry.BuildableID = context_sidebar->Column[c].Buildables[b].BuildableID;
					sidebar_entry.BuildableType = context_sidebar->Column[c].Buildables[b].BuildableType;
					sidebar_entry.BuildableViaCapture = context_sidebar->Column[c].Buildables[b].BuildableViaCapture;
			   	sidebar_entry.Fake = false;
			
					TechnoTypeClass const * tech = Fetch_Techno_Type(context_sidebar->Column[c].Buildables[b].BuildableType, context_sidebar->Column[c].Buildables[b].BuildableID);
				
					sidebar_entry.SuperWeaponType = SW_NONE;

					if (tech) {
						sidebar_entry.Cost = tech->Cost * PlayerPtr->CostBias;
						sidebar_entry.PowerProvided = 0;
						sidebar_entry.BuildTime = tech->Time_To_Build(PlayerPtr->Class->House);
						strncpy(sidebar_entry.AssetName, tech->IniName, CNC_OBJECT_ASSET_NAME_LENGTH);
						sidebar_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
					} else {
						sidebar_entry.Cost = 0;
						sidebar_entry.AssetName[0] = 0;
					}	
			
					SuperClass* super_weapon = nullptr;

					bool isbusy = false;

					switch (context_sidebar->Column[c].Buildables[b].BuildableType) {
						case RTTI_INFANTRYTYPE:
							sidebar_entry.Type = INFANTRY_TYPE;
							isbusy = (PlayerPtr->InfantryFactory != -1);
							isbusy |= Infantry.Avail() <= 0;
							break;
						
						case RTTI_UNITTYPE:
							isbusy = (PlayerPtr->UnitFactory != -1);
							isbusy |= Units.Avail() <= 0;
							sidebar_entry.Type = UNIT_TYPE;
							break;
						
						case RTTI_AIRCRAFTTYPE:
							isbusy = (PlayerPtr->AircraftFactory != -1);
							isbusy |= Aircraft.Avail() <= 0;
							sidebar_entry.Type = AIRCRAFT_TYPE;
							break;
					
						case RTTI_BUILDINGTYPE:
						{
							isbusy = (PlayerPtr->BuildingFactory != -1);
							isbusy |= Buildings.Avail() <= 0;
							sidebar_entry.Type = BUILDING_TYPE;

							const BuildingTypeClass* build_type = static_cast<const BuildingTypeClass*>(tech);
							sidebar_entry.PowerProvided = build_type->Power - build_type->Drain;
						}
							break;

						default:
							sidebar_entry.Type = UNKNOWN;
							break;

						case RTTI_SPECIAL:
							switch (context_sidebar->Column[c].Buildables[b].BuildableID) {
								case SPC_ION_CANNON:
									sidebar_entry.SuperWeaponType = SW_ION_CANNON;
									sidebar_entry.Type = SPECIAL;
									strncpy(sidebar_entry.AssetName,  "SW_Ion", CNC_OBJECT_ASSET_NAME_LENGTH);
									sidebar_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
									super_weapon = &PlayerPtr->IonCannon;
									break;

								case SPC_NUCLEAR_BOMB:
									sidebar_entry.SuperWeaponType = SW_NUKE;
									sidebar_entry.Type = SPECIAL;
									strncpy(sidebar_entry.AssetName, "SW_Nuke", CNC_OBJECT_ASSET_NAME_LENGTH);
									sidebar_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
									super_weapon = &PlayerPtr->NukeStrike;
									break;

								case SPC_AIR_STRIKE:
									sidebar_entry.SuperWeaponType = SW_AIR_STRIKE;
									sidebar_entry.Type = SPECIAL;
									strncpy(sidebar_entry.AssetName, "SW_AirStrike", CNC_OBJECT_ASSET_NAME_LENGTH);
									sidebar_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
									super_weapon = &PlayerPtr->AirStrike;
									break;

								default:
									sidebar_entry.SuperWeaponType = SW_UNKNOWN;
									sidebar_entry.Type = SPECIAL;
									break;
							}
							break;
					}

					if (super_weapon != nullptr)
					{
						sidebar_entry.Progress = (float)super_weapon->Anim_Stage() / (float)SuperClass::ANIMATION_STAGES;
						sidebar_entry.Completed = super_weapon->Is_Ready();
						sidebar_entry.Constructing = super_weapon->Anim_Stage() != SuperClass::ANIMATION_STAGES;
						sidebar_entry.ConstructionOnHold = false;
						sidebar_entry.PlacementListLength = 0;
						sidebar_entry.PowerProvided = 0;
						sidebar_entry.BuildTime = super_weapon->Get_Recharge_Time();
					}
					else
					{

						int fnumber = context_sidebar->Column[c].Buildables[b].Factory;
						FactoryClass * factory = NULL;
						if (tech && fnumber != -1) {
							factory = Factories.Raw_Ptr(fnumber);
						}
			
						sidebar_entry.Completed = false;
						sidebar_entry.Constructing = false;
						sidebar_entry.ConstructionOnHold = false;
						sidebar_entry.Progress = 0.0f;
						sidebar_entry.Busy = isbusy;
						sidebar_entry.PlacementListLength = 0;
			  
						if (factory) {
							if (factory->Is_Building()) {
								sidebar_entry.Constructing = true;
								sidebar_entry.Progress = (float)factory->Completion() / (float)FactoryClass::STEP_COUNT;
								sidebar_entry.Completed = factory->Has_Completed();
							}
							else {
								sidebar_entry.Completed = factory->Has_Completed();
								if (!sidebar_entry.Completed)
								{
									sidebar_entry.ConstructionOnHold = true;
									sidebar_entry.Progress = (float)factory->Completion() / (float)FactoryClass::STEP_COUNT;
								}

								if (sidebar_entry.Completed && sidebar_entry.Type == BUILDING_TYPE) {
									if (tech) {
										BuildingTypeClass *building_type = (BuildingTypeClass*)tech;
										short const *occupy_list = building_type->Occupy_List(true);
										if (occupy_list) {
											while (*occupy_list != REFRESH_EOL && sidebar_entry.PlacementListLength < MAX_OCCUPY_CELLS) {
												sidebar_entry.PlacementList[sidebar_entry.PlacementListLength] = *occupy_list;
												sidebar_entry.PlacementListLength++;
												occupy_list++;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}



	return true;
}


void DLLExportClass::Calculate_Placement_Distances(BuildingTypeClass* placement_type, unsigned char* placement_distance)
{
	int map_cell_x = Map.MapCellX;
	int map_cell_y = Map.MapCellY;
	int map_cell_width = Map.MapCellWidth;
	int map_cell_height = Map.MapCellHeight;

	if (map_cell_x > 0) {
		map_cell_x--;
		map_cell_width++;
	}

	if (map_cell_width < MAP_MAX_CELL_WIDTH) {
		map_cell_width++;
	}

	if (map_cell_y > 0) {
		map_cell_y--;
		map_cell_height++;
	}

	if (map_cell_height < MAP_MAX_CELL_HEIGHT) {
		map_cell_height++;
	}

	memset(placement_distance, 255U, MAP_CELL_TOTAL);
	for (int y = 0; y < map_cell_height; y++) {
		for (int x = 0; x < map_cell_width; x++) {
			CELL cell = (CELL)map_cell_x + x + ((map_cell_y + y) << 6);
			BuildingClass* base = (BuildingClass*)Map[cell].Cell_Find_Object(RTTI_BUILDING);
			if ((base && base->House->Class->House == PlayerPtr->Class->House) || (Map[cell].Owner == PlayerPtr->Class->House)) {
				placement_distance[cell] = 0U;
				for (FacingType facing = FACING_N; facing < FACING_COUNT; facing++) {
					CELL adjcell = Adjacent_Cell(cell, facing);
					if (Map.In_Radar(adjcell)) {
						placement_distance[adjcell] = min(placement_distance[adjcell], 1U);
					}
				}
			}
		}
	}
}

void Recalculate_Placement_Distances()
{
	DLLExportClass::Recalculate_Placement_Distances();
}

void DLLExportClass::Recalculate_Placement_Distances()
{
	if (PlacementType[CurrentLocalPlayerIndex] != NULL) {
		Calculate_Placement_Distances(PlacementType[CurrentLocalPlayerIndex], PlacementDistance[CurrentLocalPlayerIndex]);
	}
}


/**************************************************************************************************
* DLLExportClass::Get_Placement_State -- Get a snapshot of legal validity of placing a structure on all map cells
*
* In:   
*
* Out:  
*
*
*
* History: 2/4/2019 3:11PM - ST
**************************************************************************************************/
bool DLLExportClass::Get_Placement_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return false;
	}

	if (PlacementType[CurrentLocalPlayerIndex] == NULL) {
		return false;
	}

	CNCPlacementInfoStruct *placement_info = (CNCPlacementInfoStruct*) buffer_in;
	
	unsigned int memory_needed = sizeof(*placement_info);	// Base amount needed. Will need more depending on how many entries there are

	int map_cell_x = Map.MapCellX;
	int map_cell_y = Map.MapCellY;
	int map_cell_width = Map.MapCellWidth;
	int map_cell_height = Map.MapCellHeight;

	if (map_cell_x > 0) {
		map_cell_x--;
		map_cell_width++;
	}

	if (map_cell_width < MAP_MAX_CELL_WIDTH) {
		map_cell_width++;
	}

	if (map_cell_y > 0) {
		map_cell_y--;
		map_cell_height++;
	}

	if (map_cell_height < MAP_MAX_CELL_HEIGHT) {
		map_cell_height++;
	}

	memory_needed += map_cell_width * map_cell_height * sizeof(CNCPlacementCellInfoStruct);

	if (memory_needed + 128 >= buffer_size) {
		return false;
	}

	placement_info->Count = map_cell_width * map_cell_height;

	int index = 0;
	for (int y=0 ; y < map_cell_height ; y++) {
		for (int x=0 ; x < map_cell_width ; x++) {

			CELL cell = (CELL) map_cell_x + x + ((map_cell_y + y) << 6);

			bool pass = Passes_Proximity_Check(cell, PlacementType[CurrentLocalPlayerIndex], PlacementDistance[CurrentLocalPlayerIndex]);

			CellClass * cellptr = &Map[cell];
			bool clear = cellptr->Is_Generally_Clear();

			CNCPlacementCellInfoStruct &placement_cell_info = placement_info->CellInfo[index++];
			placement_cell_info.PassesProximityCheck = pass;
			placement_cell_info.GenerallyClear = clear;
		}	
	}

	Map.ZoneOffset = 0;

	return true;
}


bool DLLExportClass::Passes_Proximity_Check(CELL cell_in, BuildingTypeClass *placement_type, unsigned char* placement_distance)
{
			  
	/*
	**	Scan through all cells that the building foundation would cover. If any adjacent
	**	cells to these are of friendly persuasion, then consider the proximity check to
	**	have been a success.
	*/
	short const *occupy_list = placement_type->Occupy_List(true);
	
	while (*occupy_list != REFRESH_EOL) {

		CELL center_cell = cell_in + *occupy_list++;

		if (!Map.In_Radar(center_cell)) {
			return false;
		}

		if (placement_distance[center_cell] <= 1U) {
			return true;
		}
	}

	return false;
}


/**************************************************************************************************
* DLLExportClass::Start_Construction -- Start sidebar construction
*
* In:   
*
* Out:  
*
*
*
* History: 1/29/2019 11:37AM - ST
**************************************************************************************************/
bool DLLExportClass::Start_Construction(uint64 player_id, int buildable_type, int buildable_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return false;
	}

	if (GameToPlay == GAME_NORMAL) {
		return Construction_Action(SIDEBAR_REQUEST_START_CONSTRUCTION, player_id, buildable_type, buildable_id);
	}
	return MP_Construction_Action(SIDEBAR_REQUEST_START_CONSTRUCTION, player_id, buildable_type, buildable_id);
}

/**************************************************************************************************
* DLLExportClass::Hold_Construction -- Pause sidebar construction
*
* In:
*
* Out:
*
*
*
* History: 6/12/2019 JAS
**************************************************************************************************/
bool DLLExportClass::Hold_Construction(uint64 player_id, int buildable_type, int buildable_id)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) 
	{
		return false;
	}

	if (GameToPlay == GAME_NORMAL) {
		return Construction_Action(SIDEBAR_REQUEST_HOLD_CONSTRUCTION, player_id, buildable_type, buildable_id);
	}
	return MP_Construction_Action(SIDEBAR_REQUEST_HOLD_CONSTRUCTION, player_id, buildable_type, buildable_id);
}

/**************************************************************************************************
* DLLExportClass::Cancel_Construction -- Stop sidebar construction
*
* In:
*
* Out:
*
*
*
* History: 6/12/2019 JAS
**************************************************************************************************/
bool DLLExportClass::Cancel_Construction(uint64 player_id, int buildable_type, int buildable_id)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) 
	{
		return false;
	}

	return Cancel_Placement(player_id, buildable_type, buildable_id) && 
		((GameToPlay == GAME_NORMAL) ? 
			Construction_Action(SIDEBAR_REQUEST_CANCEL_CONSTRUCTION, player_id, buildable_type, buildable_id) :
			MP_Construction_Action(SIDEBAR_REQUEST_CANCEL_CONSTRUCTION, player_id, buildable_type, buildable_id));
}

/**************************************************************************************************
* DLLExportClass::Construction_Action -- Reproduce actions on the sidebar
*
* In:   
*
* Out:  
*
*
*
* History: 1/29/2019 11:37AM - ST
**************************************************************************************************/
bool DLLExportClass::Construction_Action(SidebarRequestEnum construction_action, uint64 player_id, int buildable_type, int buildable_id)
{		
		
	/*
	** 
	** Based on SidebarClass::StripClass::SelectClass::Action
	** 
	** Most of this code is validating that the game is in the correct state to be able to act on a sidebar icon
	** 
	*/
			
	for (int c = 0 ; c < 2 ; c++) {
		
		/*
		** Each production slot in the column
		*/
		for (int b=0 ; b < Map.Column[c].BuildableCount ; b++) {
			if (Map.Column[c].Buildables[b].BuildableID == buildable_id) {
				if (Map.Column[c].Buildables[b].BuildableType == buildable_type) {
					
					
					int genfactory = -1;
					switch (buildable_type) {
						case RTTI_INFANTRYTYPE:
							genfactory = PlayerPtr->InfantryFactory;
							break;

						case RTTI_UNITTYPE:
							genfactory = PlayerPtr->UnitFactory;
							break;

						case RTTI_AIRCRAFTTYPE:
							genfactory = PlayerPtr->AircraftFactory;
							break;

						case RTTI_BUILDINGTYPE:
							genfactory = PlayerPtr->BuildingFactory;
							break;

						default:
							genfactory = -1;
							break;
					}

					int fnumber = Map.Column[c].Buildables[b].Factory;
					int spc = 0;
					ObjectTypeClass const * choice = NULL;

					if (buildable_type != RTTI_SPECIAL) {
						choice  = Fetch_Techno_Type((RTTIType)buildable_type, buildable_id);
					} else {
						spc = buildable_id;
					}

					FactoryClass * factory = NULL;
					if (fnumber != -1) {
						factory = Factories.Raw_Ptr(fnumber);
					}

					if (spc == 0 && choice) {
						if (fnumber == -1 && genfactory != -1) {
							return(false);
						}

						if (factory) {

							/*
							**	If this object is currently being built, then give a scold sound and text and then
							**	bail.
							*/
							switch (construction_action)
							{
							case SIDEBAR_REQUEST_CANCEL_CONSTRUCTION:
								On_Speech(PlayerPtr, VOX_CANCELED); // Speak(VOX_CANCELED);
								OutList.Add(EventClass(EventClass::ABANDON, (RTTIType)buildable_type, buildable_id));
								break;
							case SIDEBAR_REQUEST_HOLD_CONSTRUCTION:
								if (factory->Is_Building())
								{
									On_Speech(PlayerPtr, VOX_SUSPENDED); // Speak(VOX_SUSPENDED);
									OutList.Add(EventClass(EventClass::SUSPEND, (RTTIType)buildable_type, buildable_id));
								}
								break;

							default:
								if (factory->Is_Building()) {
									On_Speech(PlayerPtr, VOX_NO_FACTORY); //Speak(VOX_NO_FACTORY); // "Cannot Comply"
									return false;
								}
								else {

									/*
									**	If production has completed, then attempt to have the object exit
									**	the factory or go into placement mode.
									*/
									if (factory->Has_Completed()) {

										TechnoClass * pending = factory->Get_Object();
										if (!pending && factory->Get_Special_Item()) {
											// TO_DO
											//Map.IsTargettingMode = true;
										}
										else {

											BuildingClass * builder = pending->Who_Can_Build_Me(false, false);
											if (!builder) {
												OutList.Add(EventClass(EventClass::ABANDON, (RTTIType)buildable_type, buildable_id));
												On_Speech(PlayerPtr, VOX_NO_FACTORY); //Speak(VOX_NO_FACTORY); // "Cannot Comply"
											}
											else {

												/*
												**	If the completed object is a building, then change the
												**	game state into building placement mode. This fact is
												**	not transmitted to any linked computers until the moment
												**	the building is actually placed down.
												*/
												if (pending->What_Am_I() == RTTI_BUILDING) {
													if (construction_action == SIDEBAR_REQUEST_START_PLACEMENT) {
														PlayerPtr->Manual_Place(builder, (BuildingClass *)pending);
													}
												}
												else {

													/*
													**	For objects that can leave the factory under their own
													**	power, queue this event and process through normal house
													**	production channels.
													*/
													//OutList.Add(EventClass(EventClass::PLACE, otype, -1));
												}
											}
										}

									}
									else {

										/*
										**	The factory must have been in a suspended state. Resume construction
										**	normally.
										*/
										if (construction_action == SIDEBAR_REQUEST_START_CONSTRUCTION) {
											On_Speech(PlayerPtr, VOX_BUILDING); // Speak(VOX_BUILDING);
											OutList.Add(EventClass(EventClass::PRODUCE, (RTTIType)buildable_type, buildable_id));
											return true;
										}
									}
								}
								break;
							}

						} else {

							switch (construction_action)
							{
								case SIDEBAR_REQUEST_CANCEL_CONSTRUCTION:
								case SIDEBAR_REQUEST_HOLD_CONSTRUCTION:
									break;

								default:
									/*
									**	If this side strip is already busy with production, then ignore the
									**	input and announce this fact.
									*/
									On_Speech(PlayerPtr, VOX_BUILDING); // Speak(VOX_BUILDING);
									OutList.Add(EventClass(EventClass::PRODUCE, (RTTIType)buildable_type, buildable_id));

									/*
									** Execute immediately so we get the sidebar feedback
									*/
									Queue_AI();
									return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}			  




/**************************************************************************************************
* DLLExportClass::MP_Construction_Action -- Reproduce actions on the sidebar
*
* In:   
*
* Out:  
*
*
*
* History: 3/26/2019 1:02PM - ST
**************************************************************************************************/
bool DLLExportClass::MP_Construction_Action(SidebarRequestEnum construction_action, uint64 player_id, int buildable_type, int buildable_id)
{		
		
	/*
	** 
	** Based on SidebarClass::StripClass::SelectClass::Action
	** 
	** Most of this code is validating that the game is in the correct state to be able to act on a sidebar icon
	** 
	*/
			
	SidebarGlyphxClass *context_sidebar = DLLExportClass::Get_Current_Context_Sidebar();
	
	for (int c = 0 ; c < 2 ; c++) {
		
		/*
		** Each production slot in the column
		*/
		for (int b=0 ; b < context_sidebar->Column[c].BuildableCount ; b++) {
			if (context_sidebar->Column[c].Buildables[b].BuildableID == buildable_id) {
				if (context_sidebar->Column[c].Buildables[b].BuildableType == buildable_type) {
					
					int genfactory = -1;
					switch (buildable_type) {
						case RTTI_INFANTRYTYPE:
							genfactory = PlayerPtr->InfantryFactory;
							break;

						case RTTI_UNITTYPE:
							genfactory = PlayerPtr->UnitFactory;
							break;

						case RTTI_AIRCRAFTTYPE:
							genfactory = PlayerPtr->AircraftFactory;
							break;

						case RTTI_BUILDINGTYPE:
							genfactory = PlayerPtr->BuildingFactory;
							break;

						default:
							genfactory = -1;
							break;
					}

					int fnumber = context_sidebar->Column[c].Buildables[b].Factory;
					int spc = 0;
					ObjectTypeClass const * choice = NULL;

					if (buildable_type != RTTI_SPECIAL) {
						choice  = Fetch_Techno_Type((RTTIType)buildable_type, buildable_id);
					} else {
						spc = buildable_id;
					}

					FactoryClass * factory = NULL;
					if (fnumber != -1) {
						factory = Factories.Raw_Ptr(fnumber);
					}

					if (spc == 0 && choice) {
						/*
						**	If there is already a factory attached to this strip but the player didn't click
						**	on the icon that has the attached factory, then say that the factory is busy and
						**	ignore the click.
						*/
						if (fnumber == -1 && genfactory != -1) {
							On_Speech(PlayerPtr, VOX_NO_FACTORY); //Speak(VOX_NO_FACTORY); // "Cannot Comply"
							return(false);
						}

						if (factory) {
							switch (construction_action)
							{
							case SIDEBAR_REQUEST_CANCEL_CONSTRUCTION:
								On_Speech(PlayerPtr, VOX_CANCELED); // Speak(VOX_CANCELED);
								OutList.Add(EventClass(EventClass::ABANDON, (RTTIType)buildable_type, buildable_id));
								break;
							case SIDEBAR_REQUEST_HOLD_CONSTRUCTION:
								if (factory->Is_Building())
								{
									On_Speech(PlayerPtr, VOX_SUSPENDED); // Speak(VOX_SUSPENDED);
									OutList.Add(EventClass(EventClass::SUSPEND, (RTTIType)buildable_type, buildable_id));
								}
								break;

							default:
								/*
								**	If this object is currently being built, then give a scold sound and text and then
								**	bail.
								*/
								if (factory->Is_Building()) {
									On_Speech(PlayerPtr, VOX_NO_FACTORY); //Speak(VOX_NO_FACTORY); // "Cannot Comply"
									return false;
								}
								else {

									/*
									**	If production has completed, then attempt to have the object exit
									**	the factory or go into placement mode.
									*/
									if (factory->Has_Completed()) {

										TechnoClass * pending = factory->Get_Object();
										if (!pending && factory->Get_Special_Item()) {
											// TO_DO
											//Map.IsTargettingMode = true;
										}
										else {

											BuildingClass * builder = pending->Who_Can_Build_Me(false, false);
											if (!builder) {
												On_Speech(PlayerPtr, VOX_NO_FACTORY); //Speak(VOX_NO_FACTORY); // "Cannot Comply"
												OutList.Add(EventClass(EventClass::ABANDON, (RTTIType)buildable_type, buildable_id));
											}
											else {

												/*
												**	If the completed object is a building, then change the
												**	game state into building placement mode. This fact is
												**	not transmitted to any linked computers until the moment
												**	the building is actually placed down.
												*/
												if (pending->What_Am_I() == RTTI_BUILDING) {
													if (construction_action == SIDEBAR_REQUEST_START_PLACEMENT) {
														if (DLLExportClass::Legacy_Render_Enabled()) {
															PlayerPtr->Manual_Place(builder, (BuildingClass *)pending);
														} else {
															Unselect_All();
														}
													}
												}
												else {

													/*
													**	For objects that can leave the factory under their own
													**	power, queue this event and process through normal house
													**	production channels.
													*/
													//OutList.Add(EventClass(EventClass::PLACE, otype, -1));
												}
											}
										}

									}
									else {

										/*
										**	The factory must have been in a suspended state. Resume construction
										**	normally.
										*/
										if (construction_action == SIDEBAR_REQUEST_START_CONSTRUCTION) {
											On_Speech(PlayerPtr, VOX_BUILDING); // Speak(VOX_BUILDING);
											OutList.Add(EventClass(EventClass::PRODUCE, (RTTIType)buildable_type, buildable_id));
											return true;
										}
									}
								}
								break;
							}

						} else {

							switch (construction_action)
							{
								case SIDEBAR_REQUEST_CANCEL_CONSTRUCTION:
								case SIDEBAR_REQUEST_HOLD_CONSTRUCTION:
									break;

								default:
									/*
									**
									*/
									On_Speech(PlayerPtr, VOX_BUILDING); // Speak(VOX_BUILDING);
									OutList.Add(EventClass(EventClass::PRODUCE, (RTTIType)buildable_type, buildable_id));

									/*
									** Execute immediately so we get the sidebar feedback
									*/
									DLLExportClass::Glyphx_Queue_AI();
									return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}			  





/**************************************************************************************************
* DLLExportClass::Start_Placement -- Start placing a completed structure
*
* In:   
*
* Out:  
*
*
*
* History: 1/29/2019 11:37AM - ST
**************************************************************************************************/
bool DLLExportClass::Start_Placement(uint64 player_id, int buildable_type, int buildable_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return false;
	}

	BuildingClass *building = Get_Pending_Placement_Object(player_id, buildable_type, buildable_id);	

	if (building) {
		
		TechnoTypeClass const * tech = Fetch_Techno_Type((RTTIType)buildable_type, buildable_id);

		if (tech) {
			BuildingTypeClass *building_type = (BuildingTypeClass*) tech;
			//short const *occupy_list = building_type->Get_Occupy_List(true);

			PlacementType[CurrentLocalPlayerIndex] = building_type;
			Recalculate_Placement_Distances();

			if (GameToPlay == GAME_NORMAL) {
				return Construction_Action(SIDEBAR_REQUEST_START_PLACEMENT, player_id, buildable_type, buildable_id);
			}
			return MP_Construction_Action(SIDEBAR_REQUEST_START_PLACEMENT, player_id, buildable_type, buildable_id);
		}
	}
	return true;
}


/**************************************************************************************************
* DLLExportClass::Cancel_Placement -- Cancel placing a completed structure
*
* In:   
*
* Out:  
*
*
*
* History: 2/7/2019 10:52AM - ST
**************************************************************************************************/
bool DLLExportClass::Cancel_Placement(uint64 player_id, int buildable_type, int buildable_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return false;
	}

	PlacementType[CurrentLocalPlayerIndex] = NULL;

	Map.PendingObjectPtr = 0;
	Map.PendingObject = 0;
	Map.PendingHouse = HOUSE_NONE;
	Map.Set_Cursor_Shape(0);

	return true;
}
			


/**************************************************************************************************
* DLLExportClass::Place -- Place a completed structure down
*
* In:   
*
* Out:  
*
*
*
* History: 2/6/2019 11:51AM - ST
**************************************************************************************************/
bool DLLExportClass::Place(uint64 player_id, int buildable_type, int buildable_id, short cell_x, short cell_y)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return false;
	}
	
	/*
	** Need to check for proximity again here?
	*/
#if (0)
Map.Passes_Proximity_Check

							Map.Set_Cursor_Shape(Map.PendingObject->Occupy_List());


			OutList.Add(EventClass(EventClass::PLACE, PendingObjectPtr->What_Am_I(), cell + ZoneOffset));
#endif
	

	BuildingClass *building = Get_Pending_Placement_Object(player_id, buildable_type, buildable_id);	

	if (building) {
		
		TechnoTypeClass const * tech = Fetch_Techno_Type((RTTIType)buildable_type, buildable_id);

		if (tech) {
			BuildingTypeClass *building_type = (BuildingTypeClass*) tech;
			//short const *occupy_list = building_type->Get_Occupy_List(true);

			PlacementType[CurrentLocalPlayerIndex] = building_type;

			/*
			** The cell coordinates passed in will be relative to the playable area that the client knows about
			*/

			int map_cell_x = Map.MapCellX;
			int map_cell_y = Map.MapCellY;
			int map_cell_width = Map.MapCellWidth;
			int map_cell_height = Map.MapCellHeight;

			if (map_cell_x > 0) {
				map_cell_x--;
				map_cell_width++;
			}

			if (map_cell_y > 0) {
				map_cell_y--;
				map_cell_height++;
			}

			CELL cell = (CELL) (map_cell_x + cell_x) + ( (map_cell_y + cell_y) << 6 );

			/*
			** Call the place directly instead of queueing it, so we can evaluate the return code.
			*/
			if (PlayerPtr->Place_Object(building->What_Am_I(), cell + Map.ZoneOffset)) {
				PlacementType[CurrentLocalPlayerIndex] = NULL;
			}	
		}
	}
	return true;

}			  




BuildingClass *DLLExportClass::Get_Pending_Placement_Object(uint64 player_id, int buildable_type, int buildable_id)
{		
	/*
	** 
	** Based on SidebarClass::StripClass::SelectClass::Action
	** 
	** 
	*/
	if (GameToPlay == GAME_NORMAL) {
	
		for (int c = 0 ; c < 2 ; c++) {
		
			/*
			** Each production slot in the column
			*/
			for (int b=0 ; b < Map.Column[c].BuildableCount ; b++) {
				if (Map.Column[c].Buildables[b].BuildableID == buildable_id) {
					if (Map.Column[c].Buildables[b].BuildableType == buildable_type) {
					
					
						int genfactory = -1;
						switch (buildable_type) {
							case RTTI_INFANTRYTYPE:
								genfactory = PlayerPtr->InfantryFactory;
								break;

							case RTTI_UNITTYPE:
								genfactory = PlayerPtr->UnitFactory;
								break;

							case RTTI_AIRCRAFTTYPE:
								genfactory = PlayerPtr->AircraftFactory;
								break;

							case RTTI_BUILDINGTYPE:
								genfactory = PlayerPtr->BuildingFactory;
								break;

							default:
								genfactory = -1;
								break;
						}

						int fnumber = Map.Column[c].Buildables[b].Factory;
						int spc = 0;
						ObjectTypeClass const * choice = NULL;

						if (buildable_type != RTTI_SPECIAL) {
							choice  = Fetch_Techno_Type((RTTIType)buildable_type, buildable_id);
						} else {
							spc = buildable_id;
						}

						FactoryClass * factory = NULL;
						if (fnumber != -1) {
							factory = Factories.Raw_Ptr(fnumber);
						}

						if (spc == 0 && choice) {
							if (fnumber == -1 && genfactory != -1) {
								return(NULL);
							}

							if (factory) {

								/*
								**	If production has completed, then attempt to have the object exit
								**	the factory or go into placement mode.
								*/
								if (factory->Has_Completed()) {
								
									TechnoClass * pending = factory->Get_Object();
									if (!pending && factory->Get_Special_Item()) {
										//Map.IsTargettingMode = true;
									} else {
										BuildingClass * builder = pending->Who_Can_Build_Me(false, false);
										if (!builder) {
											OutList.Add(EventClass(EventClass::ABANDON, buildable_type, buildable_id));
											On_Speech(PlayerPtr, VOX_NO_FACTORY); // Speak(VOX_NO_FACTORY);
										} else {

											/*
											**	If the completed object is a building, then change the
											**	game state into building placement mode. This fact is
											**	not transmitted to any linked computers until the moment
											**	the building is actually placed down.
											*/
											if (pending->What_Am_I() == RTTI_BUILDING) {
												return (BuildingClass*)pending;
										 		//PlayerPtr->Manual_Place(builder, (BuildingClass *)pending);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

	} else {
		
		if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
			
			
			SidebarGlyphxClass *context_sidebar = DLLExportClass::Get_Current_Context_Sidebar();
		
			for (int c = 0 ; c < 2 ; c++) {
		
				/*
				** Each production slot in the column
				*/
				for (int b=0 ; b < context_sidebar->Column[c].BuildableCount ; b++) {
					if (context_sidebar->Column[c].Buildables[b].BuildableID == buildable_id) {
						if (context_sidebar->Column[c].Buildables[b].BuildableType == buildable_type) {
					
					
							int genfactory = -1;
							switch (buildable_type) {
								case RTTI_INFANTRYTYPE:
									genfactory = PlayerPtr->InfantryFactory;
									break;

								case RTTI_UNITTYPE:
									genfactory = PlayerPtr->UnitFactory;
									break;

								case RTTI_AIRCRAFTTYPE:
									genfactory = PlayerPtr->AircraftFactory;
									break;

								case RTTI_BUILDINGTYPE:
									genfactory = PlayerPtr->BuildingFactory;
									break;

								default:
									genfactory = -1;
									break;
							}

							int fnumber = context_sidebar->Column[c].Buildables[b].Factory;
							int spc = 0;
							ObjectTypeClass const * choice = NULL;

							if (buildable_type != RTTI_SPECIAL) {
								choice  = Fetch_Techno_Type((RTTIType)buildable_type, buildable_id);
							} else {
								spc = buildable_id;
							}

							FactoryClass * factory = NULL;
							if (fnumber != -1) {
								factory = Factories.Raw_Ptr(fnumber);
							}

							if (spc == 0 && choice) {
								if (fnumber == -1 && genfactory != -1) {
									return(NULL);
								}

								if (factory) {

									/*
									**	If production has completed, then attempt to have the object exit
									**	the factory or go into placement mode.
									*/
									if (factory->Has_Completed()) {
								
										TechnoClass * pending = factory->Get_Object();
										if (!pending && factory->Get_Special_Item()) {
											//Map.IsTargettingMode = true;
										} else {
											BuildingClass * builder = pending->Who_Can_Build_Me(false, false);
											if (!builder) {
												OutList.Add(EventClass(EventClass::ABANDON, buildable_type, buildable_id));
												On_Speech(PlayerPtr, VOX_NO_FACTORY); // Speak(VOX_NO_FACTORY);
											} else {

												/*
												**	If the completed object is a building, then change the
												**	game state into building placement mode. This fact is
												**	not transmitted to any linked computers until the moment
												**	the building is actually placed down.
												*/
												if (pending->What_Am_I() == RTTI_BUILDING) {
													return (BuildingClass*)pending;
											 		//PlayerPtr->Manual_Place(builder, (BuildingClass *)pending);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}	
	return NULL;
}		

/**************************************************************************************************
* DLLExportClass::Place_Super_Weapon 
*
* History: 
**************************************************************************************************/
bool DLLExportClass::Place_Super_Weapon(uint64 player_id, int buildable_type, int buildable_id, int x, int y)
{
	if (buildable_type != RTTI_SPECIAL)
	{
		return false;
	}

	COORDINATE coord = Map.Pixel_To_Coord(x, y);
	CELL cell = Coord_Cell(coord);

	SpecialWeaponType weapon_type = (SpecialWeaponType)buildable_id;

	OutList.Add(EventClass(EventClass::SPECIAL_PLACE, weapon_type, cell));

	return true;
}

/**************************************************************************************************
* DLLExportClass::Create_Control_Group
*
* History:
**************************************************************************************************/
bool DLLExportClass::Create_Control_Group(unsigned char control_group_index)
{
	Handle_Team(control_group_index, 2);

	return true;
}

/**************************************************************************************************
* DLLExportClass::Add_To_Control_Group
*
* History:
**************************************************************************************************/
bool DLLExportClass::Add_To_Control_Group(unsigned char control_group_index)
{
	Handle_Team(control_group_index, 1);

	return true;
}

/**************************************************************************************************
* DLLExportClass::Toggle_Control_Group_Selection
*
* History:
**************************************************************************************************/
bool DLLExportClass::Toggle_Control_Group_Selection(unsigned char control_group_index)
{
	Handle_Team(control_group_index, 0);

	return true;
}



/**************************************************************************************************
* DLLExportClass::Get_Shroud_State -- Get a snapshot of the shroud for the given player
*
* In:   
*
* Out:  
*
*
*
* History: 4/12/2019 3:44PM - ST
**************************************************************************************************/
bool DLLExportClass::Get_Shroud_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return false;
	}
	
	CNCShroudStruct *shroud = (CNCShroudStruct*) buffer_in;
	
	unsigned int memory_needed = sizeof(*shroud) + 256;		// Base amount needed. Will need more depending on how many entries there are

	int entry_index = 0;

	/*
	** 
	**  Based loosely on DisplayClass::Redraw_Icons
	** 
	** 
	*/
	int map_cell_x = Map.MapCellX;
	int map_cell_y = Map.MapCellY;
	int map_cell_width = Map.MapCellWidth;
	int map_cell_height = Map.MapCellHeight;

	if (map_cell_x > 0) {
		map_cell_x--;
		map_cell_width++;
	}

	if (map_cell_width < MAP_MAX_CELL_WIDTH) {
		map_cell_width++;
	}

	if (map_cell_y > 0) {
		map_cell_y--;
		map_cell_height++;
	}

	if (map_cell_height < MAP_MAX_CELL_HEIGHT) {
		map_cell_height++;
	}

	for (int y = 0 ; y < map_cell_height ; y++) {
		for (int x = 0 ; x < map_cell_width ; x++) {
			CELL cell = XY_Cell(map_cell_x+x, map_cell_y+y);
			COORDINATE coord = Cell_Coord(cell) & 0xFF00FF00L;

			memory_needed += sizeof(CNCShroudEntryStruct);
			if (memory_needed >= buffer_size) {
				return false;
			}

			int xpixel;
			int ypixel;

			Map.Coord_To_Pixel(coord, xpixel, ypixel);
			
			CellClass * cellptr = &Map[Coord_Cell(coord)];

			CNCShroudEntryStruct &shroud_entry = shroud->Entries[entry_index];

			shroud_entry.IsVisible = cellptr->Is_Visible(PlayerPtr);
			shroud_entry.IsMapped = cellptr->Is_Mapped(PlayerPtr);
			shroud_entry.IsJamming = false;
			shroud_entry.ShadowIndex = -1;

			if (!shroud_entry.IsMapped) {
				if (shroud_entry.IsVisible) {
					shroud_entry.ShadowIndex = (char) Map.Cell_Shadow(cell, PlayerPtr);
				}
			}

			entry_index++;
		}
	}

	shroud->Count = entry_index;

	return true;
}	




/**************************************************************************************************
* DLLExportClass::Get_Occupier_State -- Get the occupier state for this player
*
* In:
*
* Out:
*
*
*
* History: 10/25/2019 - SKY
**************************************************************************************************/
bool DLLExportClass::Get_Occupier_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size)
{
	UNREFERENCED_PARAMETER(player_id);

	CNCOccupierHeaderStruct* occupiers = (CNCOccupierHeaderStruct*)buffer_in;
	CNCOccupierEntryHeaderStruct* entry = reinterpret_cast<CNCOccupierEntryHeaderStruct*>(occupiers + 1U);
	occupiers->Count = 0;

	unsigned int memory_needed = sizeof(CNCOccupierHeaderStruct);

	int map_cell_x = Map.MapCellX;
	int map_cell_y = Map.MapCellY;
	int map_cell_width = Map.MapCellWidth;
	int map_cell_height = Map.MapCellHeight;

	if (map_cell_x > 0) {
		map_cell_x--;
		map_cell_width++;
	}

	if (map_cell_width < MAP_MAX_CELL_WIDTH) {
		map_cell_width++;
	}

	if (map_cell_y > 0) {
		map_cell_y--;
		map_cell_height++;
	}

	if (map_cell_height < MAP_MAX_CELL_HEIGHT) {
		map_cell_height++;
	}

	for (int y = 0; y < map_cell_height; y++) {
		for (int x = 0; x < map_cell_width; x++, occupiers->Count++) {
			CELL cell = XY_Cell(map_cell_x + x, map_cell_y + y);
			CellClass * cellptr = &Map[cell];

			int occupier_count = 0;
			ObjectClass* const cell_occupier = cellptr->Cell_Occupier();
			for (ObjectClass* optr = cell_occupier; optr != NULL; optr = optr->Next) {
				if (optr->IsActive) {
					occupier_count++;
				}
			}

			memory_needed += sizeof(CNCOccupierEntryHeaderStruct) + (sizeof(CNCOccupierObjectStruct) * occupier_count);
			if (memory_needed >= buffer_size) {
				return false;
			}

			CNCOccupierObjectStruct* occupier = reinterpret_cast<CNCOccupierObjectStruct*>(entry + 1U);
			entry->Count = occupier_count;

			for (ObjectClass* optr = cell_occupier; optr != NULL; optr = optr->Next) {
				if (optr->IsActive) {
					CNCObjectStruct object;
					Convert_Type(optr, object);
					occupier->Type = object.Type;
					occupier->ID = object.ID;
					occupier++;
				}
			}

			entry = reinterpret_cast<CNCOccupierEntryHeaderStruct*>(occupier + 1U);
		}
	}

	return true;
}




/**************************************************************************************************
* DLLExportClass::Get_Player_Info_State -- Get the multiplayer info for this player
*
* In:   
*
* Out:  
*
*
*
* History: 4/22/2019 10:33AM - ST
**************************************************************************************************/
bool DLLExportClass::Get_Player_Info_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return false;
	}
	
	CNCPlayerInfoStruct *player_info = (CNCPlayerInfoStruct*) buffer_in;
	
	unsigned int memory_needed = sizeof(*player_info) + 32;  // A little extra for no reason

	if (memory_needed >= buffer_size) {
		return false;
	}
	
	player_info->GlyphxPlayerID = 0;

	if (PlayerPtr == NULL) {
		return false;;
	}

	strncpy(&player_info->Name[0], MPlayerNames[CurrentLocalPlayerIndex], MPLAYER_NAME_MAX);
	player_info->Name[MPLAYER_NAME_MAX - 1] = 0;			// Make sure it's terminated
	player_info->House = PlayerPtr->Class->House;
	player_info->AllyFlags = PlayerPtr->Get_Ally_Flags();
	player_info->ColorIndex = MPlayerID_To_ColorIndex(MPlayerID[CurrentLocalPlayerIndex]);
	player_info->GlyphxPlayerID = player_id;
	player_info->HomeCellX = Cell_X(MultiplayerStartPositions[CurrentLocalPlayerIndex]);
	player_info->HomeCellY = Cell_Y(MultiplayerStartPositions[CurrentLocalPlayerIndex]);
	player_info->IsDefeated = PlayerPtr->IsDefeated;
	player_info->SpiedPowerFlags = 0U;
	player_info->SpiedMoneyFlags = 0U;
	player_info->IsRadarJammed = false;

	// Populate selection info
	if (CurrentObject.Count() > 0) {
		CNCObjectStruct object;
		Convert_Type(CurrentObject[0], object);
		player_info->SelectedID = object.ID;
		player_info->SelectedType = object.Type;

		const int left = Map.MapCellX;
		const int right = Map.MapCellX + Map.MapCellWidth - 1;
		const int top = Map.MapCellY;
		const int bottom = Map.MapCellY + Map.MapCellHeight - 1;

		// Use first object with a weapon, or first object if none
		ObjectClass* action_object = nullptr;
		for (int i = 0; i < CurrentObject.Count(); ++i) {
			ObjectClass* object = CurrentObject[i];
			if (object->Is_Techno()) {
				TechnoClass* techno = (TechnoClass*)object;
				if (techno->Techno_Type_Class()->Primary != WEAPON_NONE || techno->Techno_Type_Class()->Secondary != WEAPON_NONE) {
					action_object = object;
					break;
				}
			}
		}
		if (action_object == nullptr) {
			action_object = CurrentObject[0];
		}

		int index = 0;
		for (int y = top; y <= bottom; ++y) {
			for (int x = left; x <= right; ++x, ++index) {
				Convert_Action_Type(action_object->What_Action(XY_Cell(x, y)), (CurrentObject.Count() == 1) ? action_object : NULL, As_Target(XY_Cell(x, y)), player_info->ActionWithSelected[index]);
			}
		}

		player_info->ActionWithSelectedCount = Map.MapCellWidth * Map.MapCellHeight;
	}
	else {
		player_info->SelectedID = -1;
		player_info->SelectedType = UNKNOWN;
		player_info->ActionWithSelectedCount = 0U;
	}

	// Screen shake
	player_info->ScreenShake = PlayerPtr->ScreenShakeTime;

	return true;
};




/**************************************************************************************************
* DLLExportClass::Get_Dynamic_Map_State -- Get a snapshot of the smudges and overlays on the terrain
*
* In:   
*
* Out:  
*
*
*
* History: 2/8/2019 10:45AM - ST
**************************************************************************************************/
bool DLLExportClass::Get_Dynamic_Map_State(uint64 player_id, unsigned char *buffer_in, unsigned int buffer_size)
{
	/*
	** Get the player for this...
	*/
	player_id;

	static int _call_count = 0;

	CNCDynamicMapStruct *dynamic_map = (CNCDynamicMapStruct*) buffer_in;
	
	unsigned int memory_needed = sizeof(*dynamic_map) + 256;		// Base amount needed. Will need more depending on how many entries there are

	int entry_index = 0;

	/*
	** 
	**  Based loosely on DisplayClass::Redraw_Icons
	** 
	** 
	*/
	int map_cell_x = Map.MapCellX;
	int map_cell_y = Map.MapCellY;
	int map_cell_width = Map.MapCellWidth;
	int map_cell_height = Map.MapCellHeight;

	if (map_cell_x > 0) {
		map_cell_x--;
		map_cell_width++;
	}

	if (map_cell_width < MAP_MAX_CELL_WIDTH) {
		map_cell_width++;
	}

	if (map_cell_y > 0) {
		map_cell_y--;
		map_cell_height++;
	}

	if (map_cell_height < MAP_MAX_CELL_HEIGHT) {
		map_cell_height++;
	}

	int cell_index = 0;

	bool debug_output = false;
	//if (_call_count == 20) {
		//debug_output = true;
	//}
				
	// Need to ignore view constraints for dynamic map updates, so the radar map
	// has the latest tiberium state for cells outside the tactical view
	DLLExportClass::Adjust_Internal_View(true);

	for (int y = 0 ; y < map_cell_height ; y++) {
		for (int x = 0 ; x < map_cell_width ; x++) {
			CELL cell = XY_Cell(map_cell_x+x, map_cell_y+y);
			COORDINATE coord = Cell_Coord(cell) & 0xFF00FF00L;

			memory_needed += sizeof(CNCDynamicMapEntryStruct) * 2;
			if (memory_needed >= buffer_size) {
				return false;
			}

			/*
			**	Only cells flagged to be redraw are examined.
			*/
			//if (In_View(cell) && Is_Cell_Flagged(cell)) {
				int xpixel;
				int ypixel;

				if (Map.Coord_To_Pixel(coord, xpixel, ypixel)) {
					CellClass * cellptr = &Map[Coord_Cell(coord)];

					/*
					**	If there is a portion of the underlying icon that could be visible,
					**	then draw it.  Also draw the cell if the shroud is off.
					*/
					if (GameToPlay == GAME_GLYPHX_MULTIPLAYER || cellptr->IsVisible || Debug_Unshroud) {
						Cell_Class_Draw_It(dynamic_map, entry_index, cellptr, xpixel, ypixel, debug_output);
					}

					/*
					**	If any cell is not fully mapped, then flag it so that the shadow drawing
					**	process will occur.  Only draw the shadow if Debug_Unshroud is false.
					*/
					//if (!cellptr->IsMapped && !Debug_Unshroud) {
					//	IsShadowPresent = true;
					//}
				}
			//}
		}
	}

	if (entry_index) {
		_call_count++;
	}

	dynamic_map->Count = entry_index;
	dynamic_map->VortexActive = false;

	return true;
}






/**************************************************************************************************
* DLLExportClass::Cell_Class_Draw_It -- Go through the motions of drawing a cell to get the smudge and overlay info
*
* In:   
*
* Out:  
*
*
*
* History: 2/8/2019 11:09AM - ST
**************************************************************************************************/
void DLLExportClass::Cell_Class_Draw_It(CNCDynamicMapStruct *dynamic_map, int &entry_index, CellClass *cell_ptr, int xpixel, int ypixel, bool debug_output)
{
	/*
	** 
	**  Based on CellClass::Draw_It and SmudgeTypeClass::Draw_It
	** 
	** 
	*/

	CELL	cell = cell_ptr->Cell_Number();

	/*
	**	Redraw any smudge.
	*/
	if (cell_ptr->Smudge != SMUDGE_NONE) {
		//SmudgeTypeClass::As_Reference(Smudge).Draw_It(x, y, SmudgeData);
		
		const SmudgeTypeClass &smudge_type = SmudgeTypeClass::As_Reference(cell_ptr->Smudge);

		if (smudge_type.Get_Image_Data() != NULL) {

			if (debug_output) {
				IsTheaterShape = true;
				Debug_Write_Shape_Type(&smudge_type, 0);
				IsTheaterShape = false;
			}
			
			CNCDynamicMapEntryStruct &smudge_entry = dynamic_map->Entries[entry_index++];

			strncpy(smudge_entry.AssetName, smudge_type.IniName, CNC_OBJECT_ASSET_NAME_LENGTH);
			smudge_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
			smudge_entry.Type = (short) cell_ptr->Smudge;
			smudge_entry.Owner = (char)cell_ptr->Owner;
			smudge_entry.DrawFlags = SHAPE_WIN_REL;			// Looks like smudges are drawn top left
			smudge_entry.PositionX = xpixel;
			smudge_entry.PositionY = ypixel;
			smudge_entry.Width = Get_Build_Frame_Width(smudge_type.Get_Image_Data());
			smudge_entry.Height = Get_Build_Frame_Height(smudge_type.Get_Image_Data());
			smudge_entry.CellX = Cell_X(cell);
			smudge_entry.CellY = Cell_Y(cell);
			smudge_entry.ShapeIndex = cell_ptr->SmudgeData;
			smudge_entry.IsSmudge = true;
			smudge_entry.IsOverlay = false;
			smudge_entry.IsResource = false;
			smudge_entry.IsSellable = false;
			smudge_entry.IsTheaterShape = true;		// Smudges are always theater-specific
			smudge_entry.IsFlag = false;
		}
	}

	/*
	**	Draw the overlay object.
	*/
	if (cell_ptr->Overlay != OVERLAY_NONE) {
		//OverlayTypeClass const & otype = OverlayTypeClass::As_Reference(Overlay);
		//IsTheaterShape = (bool)otype.IsTheater;
		//CC_Draw_Shape(otype.Get_Image_Data(), OverlayData, (x+(CELL_PIXEL_W>>1)), (y+(CELL_PIXEL_H>>1)), WINDOW_TACTICAL, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_GHOST, NULL, Map.UnitShadow);
		//IsTheaterShape = false;
		
		const OverlayTypeClass &overlay_type = OverlayTypeClass::As_Reference(cell_ptr->Overlay);

		if (overlay_type.Get_Image_Data() != NULL) {
			
			CNCDynamicMapEntryStruct &overlay_entry = dynamic_map->Entries[entry_index++];


			if (debug_output) {
				IsTheaterShape = (bool)overlay_type.IsTheater;
				Debug_Write_Shape_Type(&overlay_type, 0);
				IsTheaterShape = false;
			}

			strncpy(overlay_entry.AssetName, overlay_type.IniName, CNC_OBJECT_ASSET_NAME_LENGTH);
			overlay_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
			overlay_entry.Type = (short)cell_ptr->Overlay;
			overlay_entry.Owner = (char) cell_ptr->Owner;
			overlay_entry.DrawFlags = SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_GHOST;		// Looks like overlays are drawn centered and translucent
			overlay_entry.PositionX = xpixel + (CELL_PIXEL_W>>1);
			overlay_entry.PositionY = ypixel + (CELL_PIXEL_H>>1);
			overlay_entry.Width = Get_Build_Frame_Width(overlay_type.Get_Image_Data());
			overlay_entry.Height = Get_Build_Frame_Height(overlay_type.Get_Image_Data());
			overlay_entry.CellX = Cell_X(cell);
			overlay_entry.CellY = Cell_Y(cell);
			overlay_entry.ShapeIndex = cell_ptr->OverlayData;
			overlay_entry.IsSmudge = false;
			overlay_entry.IsOverlay = true;
			overlay_entry.IsResource = overlay_entry.Type >= OVERLAY_TIBERIUM1 && overlay_entry.Type <= OVERLAY_TIBERIUM12;
			overlay_entry.IsSellable = overlay_entry.Type >= OVERLAY_SANDBAG_WALL && overlay_entry.Type <= OVERLAY_WOOD_WALL;
			overlay_entry.IsTheaterShape = (bool)overlay_type.IsTheater;
			overlay_entry.IsFlag = false;
		}
	}


	if (cell_ptr->IsFlagged) {

		const void* image_data = MixFileClass::Retrieve("FLAGFLY.SHP");
		if (image_data != NULL) {

			CNCDynamicMapEntryStruct &flag_entry = dynamic_map->Entries[entry_index++];

			strncpy(flag_entry.AssetName, "FLAGFLY", CNC_OBJECT_ASSET_NAME_LENGTH);
			flag_entry.AssetName[CNC_OBJECT_ASSET_NAME_LENGTH - 1] = 0;
			flag_entry.Type = -1;
			flag_entry.Owner = cell_ptr->Owner;
			flag_entry.DrawFlags = SHAPE_CENTER|SHAPE_GHOST|SHAPE_FADING;
			flag_entry.PositionX = xpixel + (ICON_PIXEL_W / 2);
			flag_entry.PositionY = ypixel + (ICON_PIXEL_H / 2);
			flag_entry.Width = Get_Build_Frame_Width(image_data);
			flag_entry.Height = Get_Build_Frame_Height(image_data);
			flag_entry.CellX = Cell_X(cell);
			flag_entry.CellY = Cell_Y(cell);
			flag_entry.ShapeIndex = Frame % 14;
			flag_entry.IsSmudge = false;
			flag_entry.IsOverlay = false;
			flag_entry.IsResource = false;
			flag_entry.IsSellable = false;
			flag_entry.IsTheaterShape = false;
			flag_entry.IsFlag = true;
		}

	}
		  
}			  




/**************************************************************************************************
* DLLExportClass::Glyphx_Queue_AI -- Special queue processing for Glyphx multiplayer mode
*
* In:   
*
* Out:  
*
*
*
* History: 3/12/2019 10:52AM - ST
**************************************************************************************************/
void DLLExportClass::Glyphx_Queue_AI(void)
{
	
	//------------------------------------------------------------------------
	//	Move events from the OutList (events generated by this player) into the
	//	DoList (the list of events to execute).
	//------------------------------------------------------------------------
	while (OutList.Count) {
		OutList.First().IsExecuted = false;
		if (!DoList.Add(OutList.First())) {
			;
		}
		OutList.Next();
	}

	/*
	** Based on Execute_DoList in queue.cpp
	** 
	** The events have the ID of the player encoded in them, so no special per-player processing should be needed.
	** When the event is created, the 'local player' is assumed to be the originator of the event, so PlayerPtr will need
	** to be swapped out to represent the real originating player prior to any events being created as a result of GlyphX input
	** 
	** ST - 3/12/2019 10:51AM
	*/

	for (int i = 0; i < MPlayerCount; i++) {

		HousesType house;
		HouseClass *housep;

		house = MPlayerHouses [i];
		housep= HouseClass::As_Pointer (house);

		//.....................................................................
		// If for some reason this house doesn't exist, skip it.
		// Also, if this house has exited the game, skip it.  (The user can
		// generate events after he exits, because the exit event is scheduled
		// at least FrameSendRate*3 frames ahead.  If one system gets these
		// packets & another system doesn't, they'll go out of sync because
		// they aren't checking the CommandCount for that house, since that
		// house isn't connected any more.)
		//.....................................................................
		if (!housep){
			continue;
		}

		if (!housep->IsHuman){
			continue;
		}

		//.....................................................................
		//	Loop through all events
		//.....................................................................
		for (int j = 0; j < DoList.Count; j++) {

			if (!DoList[j].IsExecuted && (unsigned)Frame >= DoList[j].Frame) {
				DoList[j].Execute();

				//...............................................................
				//	Mark this event as executed.
				//...............................................................
				DoList[j].IsExecuted = 1;
			}
		}
	}




	//------------------------------------------------------------------------
	//	Clean out the DoList
	//------------------------------------------------------------------------
	while (DoList.Count) {

		//.....................................................................
		//	Discard events that have been executed, OR it's too late to execute.
		//	(This happens if another player exits the game; he'll leave FRAMEINFO
		//	events lying around in my queue.  They won't have been "executed",
		//	because his IPX connection was destroyed.)
		//.....................................................................
		if ( (DoList.First().IsExecuted) || ((unsigned)Frame > DoList.First().Frame) ) {
			DoList.Next();
		}
		else {
			break;
		}
	}

}

			  



/**************************************************************************************************
* DLLExportClass::Reset_Sidebars -- Init the multiplayer sidebars
*
* In:   
*
* Out:  
*
*
*
* History: 3/14/2019 3:10PM - ST
**************************************************************************************************/
void DLLExportClass::Reset_Sidebars(void)
{
	for (int i=0 ; i<MPlayerCount ; i++) {
		HouseClass *player_ptr = HouseClass::As_Pointer(MPlayerHouses[i]);	//HouseClass::As_Pointer(HOUSE_MULTI2);
		MultiplayerSidebars[i].Init_Clear(player_ptr);
	}
}			  




/**************************************************************************************************
* DLLExportClass::Set_Player_Context -- Switch the C&C local player context
*
* In:   
*
* Out:  
*
*
*
* History: 3/14/2019 3:20PM - ST
**************************************************************************************************/
bool DLLExportClass::Set_Player_Context(uint64 glyphx_player_id, bool force)
{
	/*
	** Context never needs to change in single player
	*/
	if (GameToPlay == GAME_NORMAL) {
		if (PlayerPtr) {
			CurrentObject.Set_Active_Context(PlayerPtr->Class->House);
		}
		return true;
	}
		 
	/*
	** C&C relies a lot on PlayerPtr, which is a pointer to the 'local' player's house. Historically, in a peer-to-peer 
	** multiplayer game, each player's PlayerPtr pointed to their own local player.
	** 
	** Since much of the IO logic depends on PlayerPtr being the player performing the action, we need to set PlayerPtr
	** correctly depending on which player generated input or needs output
	*/

	for (int i=0 ; i<MPlayerCount ; i++) {
		if (GlyphxPlayerIDs[i] == glyphx_player_id) {
			
			if (!force && i == CurrentLocalPlayerIndex) {
				return true;
			}

			MPlayerLocalID = MPlayerID[i];
			PlayerPtr = HouseClass::As_Pointer(MPlayerHouses[i]);	//HouseClass::As_Pointer(HOUSE_MULTI2);
			CurrentObject.Set_Active_Context(PlayerPtr->Class->House);
			CurrentLocalPlayerIndex = i;

			return true;
		}
	}

	return false;
}



/**************************************************************************************************
* DLLExportClass::Reset_Player_Context -- Clear out old player context data
*
* In:   
*
* Out:  
*
*
*
* History: 4/16/2019 10:36AM - ST
**************************************************************************************************/
void DLLExportClass::Reset_Player_Context(void)
{
	for (int i=0 ; i<MAX_PLAYERS ; i++) {
		PlacementType[i] = NULL;
	}
	CurrentLocalPlayerIndex = 0;
	CurrentObject.Clear_All();
}



/**************************************************************************************************
* Logic_Switch_Player_Context -- Called when the internal game locic needs to switch player context
*
* In:   
*
* Out:  
*
*
*
* History: 4/17/2019 9:45AM - ST
**************************************************************************************************/
void Logic_Switch_Player_Context(ObjectClass *object)
{
	DLLExportClass::Logic_Switch_Player_Context(object);
}


/**************************************************************************************************
* DLLExportClass::Logic_Switch_Player_Context -- Called when the internal game locic needs to switch player context
*
* In:   
*
* Out:  
*
*
*
* History: 4/17/2019 9:45AM - ST
**************************************************************************************************/
void DLLExportClass::Logic_Switch_Player_Context(ObjectClass *object)
{
	if (object == NULL) {
		return;
	}
	 
	/*
	** If it's not a techno, it can't be owned.
	*/
	if (!object->Is_Techno()) {
		return;
	}
		
	TechnoClass *tech = static_cast<TechnoClass*>(object);

	//HousesType house = tech->House->Class->House;
	DLLExportClass::Logic_Switch_Player_Context(tech->House);
}	
		
		
		
/**************************************************************************************************
* Logic_Switch_Player_Context -- Called when the internal game locic needs to switch player context
*
* In:   
*
* Out:  
*
*
*
* History: 4/17/2019 9:45AM - ST
**************************************************************************************************/
void Logic_Switch_Player_Context(HouseClass *object)
{
	DLLExportClass::Logic_Switch_Player_Context(object);
}


/**************************************************************************************************
* DLLExportClass::Logic_Switch_Player_Context -- Called when the internal game locic needs to switch player context
*
* In:   
*
* Out:  
*
*
*
* History: 4/17/2019 9:45AM - ST
**************************************************************************************************/
void DLLExportClass::Logic_Switch_Player_Context(HouseClass *house)
{
	if (GameToPlay == GAME_NORMAL) {
		CurrentObject.Set_Active_Context(PlayerPtr->Class->House);
		return;
	}

	if (house == NULL) {
		return;
	}
	 
	/*
	** C&C relies a lot on PlayerPtr, which is a pointer to the 'local' player's house. Historically, in a peer-to-peer 
	** multiplayer game, each player's PlayerPtr pointed to their own local player.
	** 
	** Since much of the IO logic depends on PlayerPtr being the player performing the action, we need to set PlayerPtr
	** correctly depending on which player generated input or needs output
	*/

	HousesType house_type = house->Class->House;

	for (int i=0 ; i<MPlayerCount ; i++) {
		
		if (house_type == MPlayerHouses[i]) {
		
			if (i == CurrentLocalPlayerIndex) {
				return;
			}

			MPlayerLocalID = MPlayerID[i];
			PlayerPtr = HouseClass::As_Pointer(MPlayerHouses[i]);	//HouseClass::As_Pointer(HOUSE_MULTI2);
			CurrentObject.Set_Active_Context(PlayerPtr->Class->House);
			CurrentLocalPlayerIndex = i;

			return;
		}
	}
}


/**************************************************************************************************
* DLLExportClass::Calculate_Start_Positions -- Calculate the initial view positions for the players
*
* In:   
*
* Out:  
*
*
*
* History: 4/16/2019 6/12/2019 3:00PM - ST
**************************************************************************************************/
void DLLExportClass::Calculate_Start_Positions(void)
{
	if (GameToPlay == GAME_NORMAL) {
		MultiplayerStartPositions[0] = Views[0];
		return;
	}

	HouseClass *player_ptr = PlayerPtr;
	
	ScenarioInit++;
	COORDINATE old_tac = Map.TacticalCoord;
	for (int i=0 ; i<MPlayerCount ; i++) {
		PlayerPtr = HouseClass::As_Pointer(MPlayerHouses[i]);
		if (PlayerPtr) {
			long x, y;
			Map.Compute_Start_Pos(x, y);
			MultiplayerStartPositions[i] = XY_Cell(x, y);
		}
	}
	Map.TacticalCoord = old_tac;
	ScenarioInit--;

	PlayerPtr = player_ptr;
}


/**************************************************************************************************
* DLLExportClass::Get_GlyphX_Player_ID -- Get the external GlyphX player ID from the C&C house/player pointer
*                                         Returns 0 in single player or if player ID isn't found
*
* In:   
*
* Out:  
*
*
*
* History: 4/22/2019 6:23PM - ST
**************************************************************************************************/
__int64 DLLExportClass::Get_GlyphX_Player_ID(const HouseClass *house)
{
	/*
	** C&C relies a lot on PlayerPtr, which is a pointer to the 'local' player's house. Historically, in a peer-to-peer 
	** multiplayer game, each player's PlayerPtr pointed to their own local player.
	** 
	** Since much of the IO logic depends on PlayerPtr being the player performing the action, we need to set PlayerPtr
	** correctly depending on which player generated input or needs output
	*/
	
	if (GameToPlay == GAME_NORMAL) {
		return 0;
	}

	if (house == NULL) {
		return 0;
	}
	 
	HousesType house_type = house->Class->House;

	for (int i=0 ; i<MPlayerCount ; i++) {
		
		if (house_type == MPlayerHouses[i]) {
			
			return GlyphxPlayerIDs[i];
		}
	}

	/*
	** Failure case.
	*/
	return 0;
}




/**************************************************************************************************
* DLLExportClass::Adjust_Internal_View -- Set the internal tactical view to encompass the input coordinates
*
* In:   
*
* Out:  
*
*
*
* History: 4/16/2019 3:00PM - ST
**************************************************************************************************/
void DLLExportClass::Adjust_Internal_View(bool force_ignore_view_constraints)
{
	/*
	** When legacy rendering is disabled (especially in multiplayer) we can get input coordinates that
	** fall outside the engine's tactical view. In this case, we need to adjust the tactical view before the
	** input will behave as expected.
	*/
	
	if (!force_ignore_view_constraints && Legacy_Render_Enabled()) {
		/*
		** Render view should already be tracking the player's local view
		*/
		DisplayClass::IgnoreViewConstraints = false;
		return;
	}

	DisplayClass::IgnoreViewConstraints = true;
}			  





/**************************************************************************************************
* DLLExportClass::Get_Current_Context_Sidebar -- Get the sidebar data for the current player context
*
* In:   
*
* Out:  
*
*
*
* History: 3/14/2019 3:20PM - ST
**************************************************************************************************/
SidebarGlyphxClass *DLLExportClass::Get_Current_Context_Sidebar(HouseClass *player_ptr)
{
	if (player_ptr) {
		
		for (int i=0 ; i<MPlayerCount ; i++) {
			if (player_ptr == HouseClass::As_Pointer(MPlayerHouses[i])) {
				return &MultiplayerSidebars[i];
			}
		}
	}
	return &MultiplayerSidebars[CurrentLocalPlayerIndex];
}			  


SidebarGlyphxClass *Get_Current_Context_Sidebar(HouseClass *player_ptr)
{
	return DLLExportClass::Get_Current_Context_Sidebar(player_ptr);
}


/**************************************************************************************************
* DLLExportClass::Repair_Mode -- Starts the player's repair mode. All it does here is unselect all units.
*
* In:
*
* Out:
*
*
*
* History: 5/1/2019 - LLL
**************************************************************************************************/
void DLLExportClass::Repair_Mode(uint64 player_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	Unselect_All();
}

/**************************************************************************************************
* DLLExportClass::Repair -- Repairs a specific building
*
* In:
*
* Out:
*
*
*
* History: 5/1/2019 - LLL
**************************************************************************************************/
void DLLExportClass::Repair(uint64 player_id, int object_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	TARGET target = Build_Target(KIND_BUILDING, object_id);
	if (target != TARGET_NONE)
	{
		BuildingClass* building = As_Building(target);
		if (building) {
			if (!building->IsActive) {
				GlyphX_Debug_Print("DLLExportClass::Repair -- trying to repair a non-active building");
			} else {

				if (building && building->Can_Repair() && building->House && building->House->Class->House == PlayerPtr->Class->House)
				{
					building->Repair(-1);
				}
			}
		}
	}
}

/**************************************************************************************************
* DLLExportClass::Sell_Mode -- Starts the player's sell mode. All it does here is unselect all units.
*
* In:
*
* Out:
*
*
*
* History: 5/1/2019 - LLL
**************************************************************************************************/
void DLLExportClass::Sell_Mode(uint64 player_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	Unselect_All();
}

/**************************************************************************************************
* DLLExportClass::Sell -- Sell's a player's speceific building.
*
* In:
*
* Out:
*
*
*
* History: 5/1/2019 - LLL
**************************************************************************************************/
void DLLExportClass::Sell(uint64 player_id, int object_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	TARGET target = Build_Target(KIND_BUILDING, object_id);
	if (target != TARGET_NONE)
	{
		BuildingClass* building = As_Building(target);
		if (building) {
			if (!building->IsActive) {
				GlyphX_Debug_Print("DLLExportClass::Sell -- trying to sell a non-active building");
			} else {
				if (building->House && building->House->Class->House == PlayerPtr->Class->House)
				{
					building->Sell_Back(1);
				}
			}
		}
	}
}



/**************************************************************************************************
* DLLExportClass::Repair_Sell_Cancel -- Ends the player's repair or sell mode. Doesn't do anything right now.
*
* In:
*
* Out:
*
*
*
* History: 5/1/2019 - LLL
**************************************************************************************************/
void DLLExportClass::Repair_Sell_Cancel(uint64 player_id)
{
	//OutputDebugString("Repair_Sell_Cancel\n");
}

/**************************************************************************************************
* DLLExportClass::Scatter_Selected -- Scatter the selected units
*
* In:
*
* Out:
*
*
*
* History: 10/15/2019 - SKY
**************************************************************************************************/
void DLLExportClass::Scatter_Selected(uint64 player_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	if (CurrentObject.Count()) {
		for (int index = 0; index < CurrentObject.Count(); index++) {
			ObjectClass const * tech = CurrentObject[index];

			if (tech && tech->Can_Player_Move()) {
				OutList.Add(EventClass(EventClass::SCATTER, tech->As_Target()));
			}
		}
	}
}

/**************************************************************************************************
* DLLExportClass::Select_Next_Unit
*
* History: 03.02.2020 MBL
**************************************************************************************************/
void DLLExportClass::Select_Next_Unit(uint64 player_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	ObjectClass* obj = Map.Next_Object(CurrentObject.Count() ? CurrentObject[0] : NULL);
	if (obj) {
		Unselect_All();
		obj->Select();

		COORDINATE center = Map.Center_Map();
		Map.Flag_To_Redraw(true);
		if (center) {
			On_Center_Camera(PlayerPtr, Coord_X(center), Coord_Y(center));
		}
	}
}

/**************************************************************************************************
* DLLExportClass::Select_Previous_Unit
*
* History: 03.02.2020 MBL
**************************************************************************************************/
void DLLExportClass::Select_Previous_Unit(uint64 player_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	ObjectClass* obj = Map.Prev_Object(CurrentObject.Count() ? CurrentObject[0] : NULL);
	if (obj) {
		Unselect_All();
		obj->Select();

		COORDINATE center = Map.Center_Map();
		Map.Flag_To_Redraw(true);
		if (center) {
			On_Center_Camera(PlayerPtr, Coord_X(center), Coord_Y(center));
		}
	}
}

/**************************************************************************************************
* DLLExportClass::Selected_Guard_Mode
*
* History: 03.03.2020 MBL
**************************************************************************************************/
void DLLExportClass::Selected_Guard_Mode(uint64 player_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	if (CurrentObject.Count()) {
		for (int index = 0; index < CurrentObject.Count(); index++) {
			ObjectClass const * tech = CurrentObject[index];

			if (tech && tech->Can_Player_Move() && tech->Can_Player_Fire()) {
				OutList.Add(EventClass(tech->As_Target(), MISSION_GUARD_AREA));
			}
		}
	}
}

/**************************************************************************************************
* DLLExportClass::Selected_Stop
*
* History: 03.03.2020 MBL
**************************************************************************************************/
void DLLExportClass::Selected_Stop(uint64 player_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	// Copied from TiberianDawn/Conquer.cpp - Keyboard_Process() with VK_S
	if (CurrentObject.Count()) {
		for (int index = 0; index < CurrentObject.Count(); index++) {
			ObjectClass const * tech = CurrentObject[index];

			if (tech && (tech->Can_Player_Move() || (tech->Can_Player_Fire() &&
					tech->What_Am_I() != RTTI_BUILDING))) {
				OutList.Add(EventClass(EventClass::IDLE, tech->As_Target()));
			}
		}
	}
}


/**************************************************************************************************
* DLLExportClass::Units_Queued_Movement_Toggle
*
* History: 03.03.2020 MBL
**************************************************************************************************/
void DLLExportClass::Units_Queued_Movement_Toggle(uint64 /*player_id*/, bool /*toggle*/)
{
	// Currently Red Alert only but stubbed in support in case we add to Tiberian Dawn later
}

/**************************************************************************************************
* DLLExportClass::Team_Units_Formation_Toggle_On
*
* History: 03.03.2020 MBL
**************************************************************************************************/
void DLLExportClass::Team_Units_Formation_Toggle_On(uint64 player_id)
{
	/*
	** Get the player for this...
	*/
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}

	// Red Alert only at this time, unless we do some updates to support in Tiberian Dawn
	#if 0
		Toggle_Formation(); // Conquer.cpp
	#endif
}


/**************************************************************************************************
* CNC_Handle_Debug_Request -- Process a debug input request
*
* In:   
*       
*
* Out:
*
*
* History: 1/7/2019 5:20PM - ST
**************************************************************************************************/
extern "C" __declspec(dllexport) void __cdecl CNC_Handle_Debug_Request(DebugRequestEnum debug_request_type, uint64 player_id, const char *object_name, int x, int y, bool unshroud, bool enemy)
{
	if (!DLLExportClass::Set_Player_Context(player_id)) {
		return;
	}
	
	switch (debug_request_type) {

	case DEBUG_REQUEST_SPAWN_OBJECT:
		{
			DLLExportClass::Debug_Spawn_Unit(object_name, x, y, enemy);
		}
		break;

	case DEBUG_REQUEST_FORCE_CRASH:
		Debug_Force_Crash = true;
		break;

	case DEBUG_REQUEST_KILL_OBJECT:
		if (strcmp(object_name, "HEAL") == 0) {
			DLLExportClass::Debug_Heal_Unit(x, y);
		}
		else {
			DLLExportClass::Debug_Kill_Unit(x, y);
		}
		break;

	case DEBUG_REQUEST_END_GAME:
		{
			bool win = true;

			const char lose[] =  "LOSE";
			if (strcmp(lose, object_name) == 0) {
				win = false;
			}

			PlayerWins = win;
			PlayerLoses = !win;
		}
		break;

	case DEBUG_REQUEST_UNSHROUD:
		Debug_Unshroud = unshroud;
		Map.Flag_To_Redraw(true);
		break;

	case DEBUG_REQUEST_SUPERWEAPON_RECHARGE:
		PlayerPtr->IonCannon.Forced_Charge(true);
		PlayerPtr->NukeStrike.Forced_Charge(true);
		PlayerPtr->AirStrike.Forced_Charge(true);
		break;

	case DEBUG_REQUEST_END_PRODUCTION:
		{	
			for (int index = 0; index < Factories.Count(); index++) {
				FactoryClass* factory = Factories.Ptr(index);
				if (factory->Get_House()->IsHuman) {
					Factories.Ptr(index)->Force_Complete();
				}
			}
		}	
		break;

	case DEBUG_REQUEST_ADD_RESOURCES:
		{
			if (object_name) {
				int amount = atoi(object_name);
				PlayerPtr->Credits += amount;
				if (PlayerPtr->Credits < 0) {
					PlayerPtr->Credits = 0;
				}
			}			
		}		 
		break;
		
	case DEBUG_REQUEST_UNLOCK_BUILDABLES:
		PlayerPtr->DebugUnlockBuildables = !PlayerPtr->DebugUnlockBuildables;
		PlayerPtr->IsRecalcNeeded = true;
		break;

	default:
			break;
	}

	
}			  


/**************************************************************************************************
* DLLExportClass::Debug_Spawn_All -- Debug spawn all buildable units and structures
*
* In:   Object to unlimbo , x & y cell positions
*
*
* Out:  True if unlimbo succeeded
*
*
*
* History: 1/22/2020 2:57PM - ST
**************************************************************************************************/
bool DLLExportClass::Try_Debug_Spawn_Unlimbo(TechnoClass *techno, int &cell_x, int &cell_y)
{
	if (techno) {
		
		int map_cell_x = Map.MapCellX;
		int map_cell_y = Map.MapCellY;
		int map_cell_right = map_cell_x + Map.MapCellWidth;
		int map_cell_bottom = map_cell_y + Map.MapCellHeight;

		map_cell_right = min(map_cell_right, cell_x + 26);		// Generally try to prevent the objects from spawing off the right of the screen
		
		int try_x = cell_x;
		int try_y = cell_y;
					
		while (try_y < map_cell_bottom) {
					
			CELL cell = XY_Cell(try_x, try_y);
					
			if (techno->Unlimbo(Cell_Coord(cell))) {
				
				try_x++;
				if (try_x > map_cell_right - 2) {
					try_x = cell_x;	//map_cell_x + 2;
					try_y++;
				}

				cell_x = try_x;
				cell_y = try_y;
				return true;
			}

			try_x++;
			if (try_x > map_cell_right - 2) {
				try_x = cell_x;	//map_cell_x + 2;
				try_y++;
			}
		}

		cell_x = try_x;
		cell_y = try_y;
	}
	
	return false;
}			  


/**************************************************************************************************
* DLLExportClass::Debug_Spawn_All -- Debug spawn all buildable units and structures
*
* In:   
*
* Out:  
*
*
*
* History: 1/22/2020 2:57PM - ST
**************************************************************************************************/
void DLLExportClass::Debug_Spawn_All(int x, int y)
{
	int map_cell_x = Map.MapCellX;
	int map_cell_y = Map.MapCellY;

	int map_cell_bottom = map_cell_y + Map.MapCellHeight;

	int origin_x = map_cell_x + 2;
	int origin_y = map_cell_y + 2;

	if (x != 0 || y != 0) {
		CELL screen_cell = Coord_Cell(Map.Pixel_To_Coord(x, y));
		origin_x = Cell_X(screen_cell);
		origin_y = Cell_Y(screen_cell);
	}

	int try_x = origin_x;
	int try_y = origin_y;

	HousesType house = PlayerPtr->Class->House;
		
	for (StructType sindex = STRUCT_FIRST; sindex < STRUCT_COUNT; sindex++) {
		BuildingTypeClass const & building_type = BuildingTypeClass::As_Reference(sindex);

		if (building_type.IsBuildable) {
				
			BuildingClass * building = new BuildingClass(building_type, house);
			if (building) {
					
				try_x = origin_x;
				try_y = origin_y;
					  
				while (try_y < map_cell_bottom) {
					if (Try_Debug_Spawn_Unlimbo(building, try_x, try_y)) {
						break;
					}
				}
			}
		}	
	}


	for (UnitType index = UNIT_FIRST; index < UNIT_COUNT; index++) {
		UnitTypeClass const & unit_type = UnitTypeClass::As_Reference(index);

		/*
		**	Fetch the sidebar cameo image for this building.
		*/
		if (unit_type.IsBuildable) {
				
			UnitClass * unit = (UnitClass*) unit_type.Create_One_Of(PlayerPtr);
			if (unit) {
				
				try_x = origin_x;
				try_y = origin_y;
					  
				while (try_y < map_cell_bottom) {
					if (Try_Debug_Spawn_Unlimbo(unit, try_x, try_y)) {
						break;
					}
				}
			}		
		}
	}


	for (InfantryType index = INFANTRY_FIRST; index < INFANTRY_COUNT; index++) {
		InfantryTypeClass	const &infantry_type = InfantryTypeClass::As_Reference(index);

		/*
		**	Fetch the sidebar cameo image for this building.
		*/
		if (infantry_type.IsBuildable) {
				
			InfantryClass * inf = (InfantryClass*) infantry_type.Create_One_Of(PlayerPtr);
			if (inf) {
				
				try_x = origin_x;
				try_y = origin_y;
					  
				while (try_y < map_cell_bottom) {
					if (Try_Debug_Spawn_Unlimbo(inf, try_x, try_y)) {
						break;
					}
				}
			}		
		}
	}

	for (AircraftType index = AIRCRAFT_FIRST; index < AIRCRAFT_COUNT; index++) {
		AircraftTypeClass	const &aircraft_type = AircraftTypeClass::As_Reference(index);

		/*
		**	Fetch the sidebar cameo image for this building.
		*/
		if (aircraft_type.IsBuildable) {
				
			AircraftClass * air = (AircraftClass*) aircraft_type.Create_One_Of(PlayerPtr);
			if (air) {
				
				try_x = origin_x;
				try_y = origin_y;
					  
				while (try_y < map_cell_bottom) {
					if (Try_Debug_Spawn_Unlimbo(air, try_x, try_y)) {
						break;
					}
				}
			}		
		}
	}

}			  





/**************************************************************************************************
* DLLExportClass::Debug_Spawn_Unit -- Debug spawn a unit at the specified location
*
* In:   
*
* Out:  
*
*
*
* History: 3/14/2019 3:20PM - ST
**************************************************************************************************/
void DLLExportClass::Debug_Spawn_Unit(const char *object_name, int x, int y, bool enemy)
{
	if (object_name == NULL) {
		return;
	}
	
	if (strlen(object_name) == 0) {
		return;
	}

	COORDINATE coord = Map.Pixel_To_Coord(x, y);
	CELL cell = Coord_Cell(coord);

	HousesType house = PlayerPtr->Class->House;

	/*
	** Place all?
	*/
	if (stricmp(object_name, "ALLOBJECTS") == 0) {
		Debug_Spawn_All(x, y);
		return;
	}

	/*
	** If this is for the enemy, find the enemy with the most stuff
	*/

	if (enemy) {
		unsigned max_count = 0;
		for (int i = 0; i < Houses.Count(); ++i) {
			const HouseClass* player = Houses.Ptr(i);
			const unsigned count = player->CurUnits + player->CurBuildings + player->CurInfantry + player->CurAircraft;
			if (!PlayerPtr->Is_Ally(player) && (count >= max_count)) {
				house = player->Class->House;
				max_count = count;
			}
		}
	}

	/*
	** What is this thing?
	*/

	StructType structure_type = BuildingTypeClass::From_Name(object_name);
	if (structure_type != STRUCT_NONE) {
		
		BuildingClass * building = new BuildingClass(structure_type, house);
		if (building) {
			if (!building->Unlimbo(Cell_Coord(cell))) {
				delete building;
			}
		}
		
#if (0)		 
		Map.PendingObject = &BuildingTypeClass::As_Reference(structure_type);
		Map.PendingHouse = PlayerPtr->ActLike;
		Map.PendingObjectPtr = Map.PendingObject->Create_One_Of(PlayerPtr);
		if (Map.PendingObjectPtr) {
			Map.Set_Cursor_Pos();
			Map.Set_Cursor_Shape(Map.PendingObject->Occupy_List());

			//OutList.Add(EventClass(EventClass::PLACE, RTTI_BUILDING, (CELL)(cell + Map.ZoneOffset)));
		}
#endif		
		return;
	}

	
	UnitType unit_type = UnitTypeClass::From_Name(object_name);
	if (unit_type != UNIT_NONE) {
		
		UnitClass * unit = new UnitClass(unit_type, house);
		if (unit) {
			unit->Unlimbo(Map.Pixel_To_Coord(x, y), DIR_N);
		}
		
		return;
	}


	InfantryType infantry_type = InfantryTypeClass::From_Name(object_name);
	if (infantry_type != INFANTRY_NONE) {
		
		InfantryClass * inf = new InfantryClass(infantry_type, house);
		if (inf) {
			inf->Unlimbo(Map.Pixel_To_Coord(x, y), DIR_N);
		}
		return;
	}

	AircraftType aircraft_type = AircraftTypeClass::From_Name(object_name);
	if (aircraft_type != AIRCRAFT_NONE) {

		AircraftClass * air = new AircraftClass(aircraft_type, house);
		if (air) {
			air->Altitude = 0;
			air->Unlimbo(Map.Pixel_To_Coord(x, y), DIR_N);
		}
		return;
	}

	OverlayType overlay_type =  OverlayTypeClass::From_Name(object_name);
	if (overlay_type != OVERLAY_NONE)
	{
		new OverlayClass(overlay_type, cell);
		return;
	}
}			  


/**************************************************************************************************
* DLLExportClass::Debug_Kill_Unit -- Kill a unit at the specified location
*
* In:   
*
* Out:  
*
*
*
* History: 8/19/2019 3:09PM - ST
**************************************************************************************************/
void DLLExportClass::Debug_Kill_Unit(int x, int y)
{
	COORDINATE coord = Map.Pixel_To_Coord(x, y);
	CELL cell = Coord_Cell(coord);

	CellClass * cellptr = &Map[cell];

	if (cellptr) {
		ObjectClass *obj = cellptr->Cell_Object();
		static const int debug_damage = 1000;
		if (obj) {
			int damage = debug_damage;
			obj->Take_Damage(damage, 0, WARHEAD_HE, 0);
		} else {
			if (cellptr->Overlay != OVERLAY_NONE) {
				OverlayTypeClass const * optr = &OverlayTypeClass::As_Reference(cellptr->Overlay);
				if (optr->IsTiberium) {
					cellptr->Reduce_Tiberium(1);
				}
				if (optr->IsWall) {
					Map[cell].Reduce_Wall(debug_damage);
				}
			}
		}
	}
}			  

void DLLExportClass::Debug_Heal_Unit(int x, int y)
{
	COORDINATE coord = Map.Pixel_To_Coord(x, y);
	CELL cell = Coord_Cell(coord);

	CellClass * cellptr = &Map[cell];

	if (cellptr) {
		ObjectClass *obj = cellptr->Cell_Object();
		if (obj) {
			obj->Strength = obj->Class_Of().MaxStrength;
		}
		else {
			if (cellptr->Overlay != OVERLAY_NONE) {
				OverlayTypeClass const * optr = &OverlayTypeClass::As_Reference(cellptr->Overlay);
				if (optr->IsTiberium) {
					const int cellcount = (int)FACING_COUNT + 1;
					CellClass* cells[cellcount];
					cells[0] = cellptr;
					for (FacingType index = FACING_N; index < FACING_COUNT; index++) {
						cells[(int)index + 1] = &cellptr->Adjacent_Cell(index);
					}

					for (int index = 0; index < cellcount; index++) {
						CellClass *newcell = cells[index];

						if (newcell && newcell->Cell_Object() == NULL) {
							if (newcell->Land_Type() == LAND_CLEAR && newcell->Overlay == OVERLAY_NONE) {
								switch (newcell->TType) {
									case TEMPLATE_BRIDGE1:
									case TEMPLATE_BRIDGE2:
									case TEMPLATE_BRIDGE3:
									case TEMPLATE_BRIDGE4:
										break;

									default:
										new OverlayClass(Random_Pick(OVERLAY_TIBERIUM1, OVERLAY_TIBERIUM12), newcell->Cell_Number());
										newcell->OverlayData = 1;
										break;

								}
							}
							else if (newcell->Land_Type() == LAND_TIBERIUM) {
								newcell->OverlayData = MIN(newcell->OverlayData + 1, 11);
								newcell->Recalc_Attributes();
								newcell->Redraw_Objects();
							}
						}
					}
				}
			}
		}
	}
}




/**************************************************************************************************
* DLLExportClass::Legacy_Render_Enabled -- Is the legacy rendering enabled?
*
* In:   
*
* Out:  
*
*
*
* History: 4/15/2019 5:46PM - ST
**************************************************************************************************/
bool DLLExportClass::Legacy_Render_Enabled(void)
{
	if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
		unsigned int num_humans = 0U;
		for (int i = 0; i < MPlayerCount; ++i) {
			HouseClass *player_ptr = HouseClass::As_Pointer(MPlayerHouses[i]);
			if (player_ptr && player_ptr->IsHuman) {
				if (++num_humans > 1) break;
			}
		}
		return num_humans < 2;
	}

	//return false;
	return true;
}


/**************************************************************************************************
* DLLExportClass::Computer_Message -- Replacement for original Computer_Message function
*
* In:
*
* Out:
*
*
*
* History: 1/27/2020 1:42PM - ST
**************************************************************************************************/
void DLLExportClass::Computer_Message(bool last_player_taunt)
{
	HousesType house;
	HouseClass *ptr;

	HouseClass *ai_players[MAX_PLAYERS];
	int ai_player_count = 0;

	/*------------------------------------------------------------------------
	Find the computer house that the message will be from
	------------------------------------------------------------------------*/
	for (house = HOUSE_MULTI1; house < (HOUSE_MULTI1 + MPlayerMax); house++) {
		ptr = HouseClass::As_Pointer(house);

		if (!ptr || ptr->IsHuman || ptr->IsDefeated) {
			continue;
		}

		ai_players[ai_player_count++] = ptr;
	}

	if (ai_player_count) {
		int ai_player_index = 0;
		if (ai_player_count > 1) {
			ai_player_index = IRandom(0, ai_player_count - 1);
		}

		int taunt_index;
		if (last_player_taunt) {
			taunt_index = 13;
		} else {
			taunt_index = IRandom(0,12);
		}

		On_Message(ai_players[ai_player_index], "", 15.0f, MESSAGE_TYPE_COMPUTER_TAUNT, taunt_index);
	}
}


/**************************************************************************************************
* DLLExportClass::Set_Special_Key_Flags --
*
* In:
*
* Out:
*
*
*
* History: 6/27/2019 - JAS
**************************************************************************************************/
void DLLExportClass::Set_Special_Key_Flags(unsigned char special_key_flags)
{
	SpecialKeyFlags[CurrentLocalPlayerIndex] = special_key_flags;
}

/**************************************************************************************************
* DLLExportClass::Clear_Special_Key_Flags --
*
* In:
*
* Out:
*
*
*
* History: 6/27/2019 - JAS
**************************************************************************************************/
void DLLExportClass::Clear_Special_Key_Flags()
{
	SpecialKeyFlags[CurrentLocalPlayerIndex] = 0;
}

/**************************************************************************************************
* DLLExportClass::Get_Input_Key_State --
*
* In:
*
* Out:
*
*
*
* History: 6/27/2019 - JAS
**************************************************************************************************/
bool DLLExportClass::Get_Input_Key_State(KeyNumType key)
{
	switch (key)
	{
	case KN_LCTRL:
		return (SpecialKeyFlags[CurrentLocalPlayerIndex] & INPUT_SPECIAL_KEY_CTRL) != 0;
		break;
	case KN_LSHIFT:
		return (SpecialKeyFlags[CurrentLocalPlayerIndex] & INPUT_SPECIAL_KEY_SHIFT) != 0;
		break;
	case KN_LALT:
		return (SpecialKeyFlags[CurrentLocalPlayerIndex] & INPUT_SPECIAL_KEY_ALT) != 0;
		break;
	default:
		break;
	};

	return false;
	
}


/**************************************************************************************************
* Get_Input_Key_State
*
* History: 6/27/2019 - JAS
**************************************************************************************************/
bool DLL_Export_Get_Input_Key_State(KeyNumType key)
{
	return DLLExportClass::Get_Input_Key_State(key);
}



bool DLLSave(FileClass &file)
{
	return DLLExportClass::Save(file);
}

bool DLLLoad(FileClass &file)
{
	return DLLExportClass::Load(file);
}


/**************************************************************************************************
* DLLExportClass::Save --
*
* In:
*
* Out:
*
*
*
* History: 9/10/2019 10:24AM - ST
**************************************************************************************************/
bool DLLExportClass::Save(FileClass & file)
{
	/*
	** Version first
	*/
	unsigned int version = CNC_DLL_API_VERSION;
	if (file.Write(&version, sizeof(version)) != sizeof(version)) {
		return false;
	}
	
	if (file.Write(MultiplayerStartPositions, sizeof(MultiplayerStartPositions)) != sizeof(MultiplayerStartPositions)) {
		return false;
	}

	if (file.Write(GlyphxPlayerIDs, sizeof(GlyphxPlayerIDs)) != sizeof(GlyphxPlayerIDs)) {
		return false;
	}

	if (file.Write(&GlyphXClientSidebarWidthInLeptons, sizeof(GlyphXClientSidebarWidthInLeptons)) != sizeof(GlyphXClientSidebarWidthInLeptons)) {
		return false;
	}

	if (file.Write(MPlayerIsHuman, sizeof(MPlayerIsHuman)) != sizeof(MPlayerIsHuman)) {
		return false;
	}

	if (file.Write(MultiplayerStartPositions, sizeof(MultiplayerStartPositions)) != sizeof(MultiplayerStartPositions)) {
		return false;
	}

	if (file.Write(PlacementType, sizeof(PlacementType)) != sizeof(PlacementType)) {
		return false;
	}

	if (file.Write(&MPlayerCount, sizeof(MPlayerCount)) != sizeof(MPlayerCount)) {
		return false;
	}

	if (file.Write(&MPlayerBases, sizeof(MPlayerBases)) != sizeof(MPlayerBases)) {
		return false;
	}
	
	if (file.Write(&MPlayerCredits, sizeof(MPlayerCredits)) != sizeof(MPlayerCredits)) {
		return false;
	}
	
	if (file.Write(&MPlayerTiberium, sizeof(MPlayerTiberium)) != sizeof(MPlayerTiberium)) {
		return false;
	}
	
	if (file.Write(&MPlayerGoodies, sizeof(MPlayerGoodies)) != sizeof(MPlayerGoodies)) {
		return false;
	}
	
	if (file.Write(&MPlayerGhosts, sizeof(MPlayerGhosts)) != sizeof(MPlayerGhosts)) {
		return false;
	}
	
	if (file.Write(&MPlayerSolo, sizeof(MPlayerSolo)) != sizeof(MPlayerSolo)) {
		return false;
	}

	if (file.Write(&MPlayerUnitCount, sizeof(MPlayerUnitCount)) != sizeof(MPlayerUnitCount)) {
		return false;
	}

	if (file.Write(&MPlayerLocalID, sizeof(MPlayerLocalID)) != sizeof(MPlayerLocalID)) {
		return false;
	}

	if (file.Write(MPlayerHouses, sizeof(MPlayerHouses)) != sizeof(MPlayerHouses)) {
		return false;
	}

	if (file.Write(MPlayerNames, sizeof(MPlayerNames)) != sizeof(MPlayerNames)) {
		return false;
	}
	
	if (file.Write(MPlayerID, sizeof(MPlayerID)) != sizeof(MPlayerID)) {
		return false;
	}

	if (file.Write(MPlayerIsHuman, sizeof(MPlayerIsHuman)) != sizeof(MPlayerIsHuman)) {
		return false;
	}

	for (int i=0 ; i<MAX_PLAYERS ; i++) {
		Sidebar_Glyphx_Save(file, &MultiplayerSidebars[i]);
	}

	if (file.Write(&Special, sizeof(Special)) != sizeof(Special)) {
		return false;
	}

	/*
	** Room for save game expansion
	*/
	unsigned char padding[4096];
	memset(padding, 0, sizeof(padding));

	if (file.Write(padding, sizeof(padding)) != sizeof(padding)) {
		return false;
	}


	return true;
}
	 

/**************************************************************************************************
* DLLExportClass::Load --
*
* In:
*
* Out:
*
*
*
* History: 9/10/2019 10:24AM - ST
**************************************************************************************************/
bool DLLExportClass::Load(FileClass & file)
{
	unsigned int version = 0;

	if (file.Read(&version, sizeof(version)) != sizeof(version)) {
		return false;
	}
	
	if (file.Read(MultiplayerStartPositions, sizeof(MultiplayerStartPositions)) != sizeof(MultiplayerStartPositions)) {
		return false;
	}

	if (file.Read(GlyphxPlayerIDs, sizeof(GlyphxPlayerIDs)) != sizeof(GlyphxPlayerIDs)) {
		return false;
	}
	
	if (file.Read(&GlyphXClientSidebarWidthInLeptons, sizeof(GlyphXClientSidebarWidthInLeptons)) != sizeof(GlyphXClientSidebarWidthInLeptons)) {
		return false;
	}

	if (file.Read(MPlayerIsHuman, sizeof(MPlayerIsHuman)) != sizeof(MPlayerIsHuman)) {
		return false;
	}

	if (file.Read(MultiplayerStartPositions, sizeof(MultiplayerStartPositions)) != sizeof(MultiplayerStartPositions)) {
		return false;
	}

	if (file.Read(PlacementType, sizeof(PlacementType)) != sizeof(PlacementType)) {
		return false;
	}

	if (file.Read(&MPlayerCount, sizeof(MPlayerCount)) != sizeof(MPlayerCount)) {
		return false;
	}

	if (file.Read(&MPlayerBases, sizeof(MPlayerBases)) != sizeof(MPlayerBases)) {
		return false;
	}
	
	if (file.Read(&MPlayerCredits, sizeof(MPlayerCredits)) != sizeof(MPlayerCredits)) {
		return false;
	}
	
	if (file.Read(&MPlayerTiberium, sizeof(MPlayerTiberium)) != sizeof(MPlayerTiberium)) {
		return false;
	}
	
	if (file.Read(&MPlayerGoodies, sizeof(MPlayerGoodies)) != sizeof(MPlayerGoodies)) {
		return false;
	}
	
	if (file.Read(&MPlayerGhosts, sizeof(MPlayerGhosts)) != sizeof(MPlayerGhosts)) {
		return false;
	}
	
	if (file.Read(&MPlayerSolo, sizeof(MPlayerSolo)) != sizeof(MPlayerSolo)) {
		return false;
	}

	if (file.Read(&MPlayerUnitCount, sizeof(MPlayerUnitCount)) != sizeof(MPlayerUnitCount)) {
		return false;
	}

	if (file.Read(&MPlayerLocalID, sizeof(MPlayerLocalID)) != sizeof(MPlayerLocalID)) {
		return false;
	}

	if (file.Read(MPlayerHouses, sizeof(MPlayerHouses)) != sizeof(MPlayerHouses)) {
		return false;
	}

	if (file.Read(MPlayerNames, sizeof(MPlayerNames)) != sizeof(MPlayerNames)) {
		return false;
	}
	
	if (file.Read(MPlayerID, sizeof(MPlayerID)) != sizeof(MPlayerID)) {
		return false;
	}

	if (file.Read(MPlayerIsHuman, sizeof(MPlayerIsHuman)) != sizeof(MPlayerIsHuman)) {
		return false;
	}

	for (int i=0 ; i<MAX_PLAYERS ; i++) {
		Sidebar_Glyphx_Load(file, &MultiplayerSidebars[i]);
	}

	if (file.Read(&Special, sizeof(Special)) != sizeof(Special)) {
		return false;
	}

	unsigned char padding[4096];

	if (file.Read(padding, sizeof(padding)) != sizeof(padding)) {
		return false;
	}


	return true;
}



/**************************************************************************************************
* DLLExportClass::Code_Pointers --
*
* In:
*
* Out:
*
*
*
* History: 9/10/2019 10:24AM - ST
**************************************************************************************************/
void DLLExportClass::Code_Pointers(void)
{
	for (int i=0 ; i<MAX_PLAYERS ; i++) {
		Sidebar_Glyphx_Code_Pointers(&MultiplayerSidebars[i]);

		if (PlacementType[i]) {
			PlacementType[i] = (BuildingTypeClass *) PlacementType[i]->Type;
		}
	}
}

	 

/**************************************************************************************************
* DLLExportClass::Decode_Pointers --
*
* In:
*
* Out:
*
*
*
* History: 9/10/2019 10:24AM - ST
**************************************************************************************************/
void DLLExportClass::Decode_Pointers(void)
{
	for (int i=0 ; i<MAX_PLAYERS ; i++) {
		Sidebar_Glyphx_Decode_Pointers(&MultiplayerSidebars[i]);

		if (PlacementType[i]) {
			StructType type = (StructType) reinterpret_cast<unsigned int>(PlacementType[i]);
			PlacementType[i] = NULL;
			if (type >= STRUCT_FIRST && type < STRUCT_COUNT) {
				
				TechnoTypeClass const * tech = Fetch_Techno_Type(RTTI_BUILDINGTYPE, type);
				if (tech) {
					BuildingTypeClass* build_type = (BuildingTypeClass*)(tech);
					if (build_type) {
						PlacementType[i] = build_type;
					}
				}
			}
		}
	}
}


void DLL_Code_Pointers(void)
{
	DLLExportClass::Code_Pointers();
}

void DLL_Decode_Pointers(void)
{
	DLLExportClass::Decode_Pointers();
}