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

//#include <string>
#include <stdio.h>

#include	"function.h"
#include "externs.h"
#include "DLLInterface.h"
#include "Gadget.h"
#include "defines.h" // VOC_COUNT, VOX_COUNT
#include "SidebarGlyphx.h"
#include	"mixfile.h"
#include "ccini.H"

extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Startup();
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Cleanup();
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Load_Map(char* cncdata_directory, char* house_name, int scenario_index, char* east_west, char* variant);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Load_Map_By_Scenario_Name(char* cncdata_directory, char* scenario_name);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Clear_Map();
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Map_Stats(int& map_width, int& map_height, int& theater);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Cell_Data_By_Index(int cell_index, char* cell_name, unsigned long cell_name_size, int& template_type, int& template_icon_index);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Cell_Data(int x, int y, char* cell_name, unsigned long cell_name_size, int& template_type, int& template_icon_index);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Cell_Texture_Buffer(int x, int y, int& out_width, int& out_height, SAFEARRAY*& out_texture_array);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Scenario_Names(char* cncdata_directory);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Template_Data(int template_type_index, SAFEARRAY*& template_points);

int LoadScenario();

extern int DLL_Startup(const char * command_line);

char EditorMapINIBuffer[SHAPE_BUFFER_SIZE];
bool EditorMapInitialized = false;

const static int EDITOR_COMMMAND_SUCCESS = 0;
const static int EDITOR_COMMMAND_FAILURE = 1;

const char* CD1Path = "\\RED_ALERT\\CD1\\";
const char* CD2Path = "\\RED_ALERT\\COUNTERSTRIKE\\";
const char* CD3Path = "\\RED_ALERT\\AFTERMATH\\";

char RedAlertINI[_MAX_PATH];

/**************************************************************************************************
* CNC_Editor_Startup
* Initializes the system to allow map loading for the editor
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Startup()
{
	/*
	BlackPalette = new(MEM_CLEAR | MEM_REAL) unsigned char[768];
	GamePalette = new(MEM_CLEAR | MEM_REAL) unsigned char[768];
	OriginalPalette = new(MEM_CLEAR | MEM_REAL) unsigned char[768];
	WhitePalette = new(MEM_CLEAR | MEM_REAL) unsigned char[768];
	memset(WhitePalette, 63, 768);


	Set_Palette(GamePalette);

	TheaterData = 0;
	TheaterIcons = 0;
	LowTheaterData = 0;
	*/

	RunningFromEditor = true;

	return EDITOR_COMMMAND_FAILURE;
}

/**************************************************************************************************
* CNC_Editor_Cleanup
* Cleans up systems initialized by the editor
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Cleanup()
{
	/*
	if (BlackPalette)
	{
		delete[] BlackPalette;
	}

	if (GamePalette)
	{
		delete[] GamePalette;
	}

	if (OriginalPalette)
	{
		delete[] OriginalPalette;
	}
	if (WhitePalette)
	{
		delete[] WhitePalette;
	}
	*/

	return EDITOR_COMMMAND_SUCCESS;
}
/**************************************************************************************************
* CNC_Editor_Load_Mix_Files
* Loads all the Mix files for Tiberian Dawn
**************************************************************************************************/
void CNC_Editor_Load_Mix_Files()
{
	const char* MixFileNames[] =
	{
		"MAIN.MIX",
		"REDALERT.MIX",
		"EXPAND2.MIX",
		"EXPAND.MIX",
		"HIRES1.MIX",
		//"LORES1.MIX"
		
		"GENERAL.MIX",		
		
		"LOCAL.MIX",
		"HIRES.MIX",
		"NCHIRES.MIX",
		"CONQUER.MIX",
		
		"RUSSIAN.MIX",
		"ALLIES.MIX",

		"SNOW.MIX",
		"TEMPERAT.MIX",
		"INTERIOR.MIX",
	};

	int count = sizeof(MixFileNames) / sizeof(MixFileNames[0]);

	for (int i = count - 1; i >= 0; --i)
	{
		MFCD::Free(MixFileNames[i]);
	}

	for (int i = 0; i < count; ++i)
	{
		MFCD* file = new MFCD(MixFileNames[i], &FastKey);
		file->Cache();
		
	}
	
}

/**************************************************************************************************
* CNC_Editor_Setup_Content_Directory
* Sets up where the system should load map data from.
*
* cncdata_directory: path of the base CNC data directory
* CD1: if true, consider this disc 1, otherwise consider this disc 2.
**************************************************************************************************/
void CNC_Editor_Setup_Content_Directory(char* cncdata_directory, int CD, char (&content_directory)[_MAX_PATH])
{

	switch (CD)
	{
	default:
	case 1:
		sprintf(content_directory, "%s%s", cncdata_directory, CD1Path);
		break;
	case 2:
		sprintf(content_directory, "%s%s", cncdata_directory, CD2Path);
		break;
	case 3:
		sprintf(content_directory, "%s%s", cncdata_directory, CD3Path);
		break;
	}

	//Setup red alert path
	sprintf(RedAlertINI, "%sREDALERT.INI",content_directory);

	if (strlen(content_directory) != 0) {
		CCFileClass::Clear_Search_Drives();
		CCFileClass::Reset_Raw_Path();
		char *dll_dir = strdup(content_directory);
		CCFileClass::Set_Search_Drives(dll_dir);
		free(dll_dir);
	}
}

/**************************************************************************************************
* CNC_Editor_Load_Map
* Loads the map with the given parameters.
*
* cncdata_directory: path of the base CNC data directory
* faction: the name of the faction we are loading the map for
* scenario_index: int scenario index
* east_west:
* variant:
*
* returns EDITOR_COMMMAND_SUCCESS on success, all other values are failure
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Load_Map(
	char* cncdata_directory,
	char* house_name,
	int scenario_index,
	char* east_west,
	char* variant)
{

	CNC_Editor_Clear_Map();

	ScenarioPlayerType scen_player;
	int CD = 1;
	if (stricmp(house_name, "SPAIN") == 0) {
		scen_player = SCEN_PLAYER_SPAIN;
		Whom = HOUSE_GOOD;

		if (scenario_index >= 40)
		{
			CD = 3;
		}
		else if (scenario_index >= 15)
		{
			CD = 2;
		}
	}

	if (stricmp(house_name, "GREECE") == 0 || stricmp(house_name, "ALLY") == 0) {
		scen_player = SCEN_PLAYER_GREECE;
		Whom = HOUSE_GOOD;

		if (scenario_index >= 40)
		{
			CD = 3;
		}
		else if (scenario_index >= 15)
		{
			CD = 2;
		}
	}

	if (stricmp(house_name, "USSR") == 0) {
		scen_player = SCEN_PLAYER_USSR;
		Whom = HOUSE_BAD;

		if (scenario_index >= 40)
		{
			CD = 3;
		}
		else if (scenario_index >= 15)
		{
			CD = 2;
		}
	}

	if (stricmp(house_name, "MULTI") == 0)
	{
		scen_player = SCEN_PLAYER_MPLAYER;
		Whom = HOUSE_MULTI1;

		if (scenario_index >= 25)
		{
			CD = 3;
		}

	}

	if (stricmp(house_name, "JAPAN") == 0)
	{
		scen_player = SCEN_PLAYER_JP;
		Whom = HOUSE_JP;
	}

	char content_directory[_MAX_PATH];

	CNC_Editor_Setup_Content_Directory(cncdata_directory, CD, content_directory);

	char command_line[_MAX_PATH];
	sprintf(command_line, "-CD%s", content_directory);

	DLL_Startup(command_line);

	Scen.Scenario = scenario_index;
	ScenarioDirType scen_dir;
	BuildLevel = 7;

	if (stricmp(east_west, "w") == 0)
	{
		scen_dir = SCEN_DIR_WEST;
	}
	else
	{
		scen_dir = SCEN_DIR_EAST;
	}

	ScenarioVarType variant_enum;

	if (stricmp(variant, "b") == 0)
	{
		variant_enum = SCEN_VAR_B;
	}
	else if (stricmp(variant, "c") == 0)
	{
		variant_enum = SCEN_VAR_C;
	}
	else if (stricmp(variant, "d") == 0)
	{
		variant_enum = SCEN_VAR_D;
	}
	else
	{
		variant_enum = SCEN_VAR_A;
	}

	Scen.Set_Scenario_Name(Scen.Scenario, scen_player, scen_dir, (ScenarioVarType)variant_enum);

	return LoadScenario();
}

/**************************************************************************************************
* CNC_Editor_Load_Map_By_Scenario_Name
* Loads the map with the given parameters.
*
* cncdata_directory: path of the base CNC data directory
* scenario_name: name of the scnario to load
*
* returns EDITOR_COMMMAND_SUCCESS on success, all other values are failure
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Load_Map_By_Scenario_Name(
	char* cncdata_directory,
	char* scenario_name)
{
	int CD = 3; // Always use the aftermath cd
	if (strnlen(scenario_name, _MAX_PATH) >= 3 && (scenario_name[2] == 'a' || scenario_name[2] == 'A'))
	{
		// Ant missions are CD 2
		CD = 2;
	}

	CNC_Editor_Clear_Map();

	char content_directory[_MAX_PATH];

	CNC_Editor_Setup_Content_Directory(cncdata_directory, CD, content_directory);

	char command_line[_MAX_PATH];
	sprintf(command_line, "-CD%s", content_directory);

	DLL_Startup(command_line);

	snprintf(Scen.ScenarioName, _MAX_FNAME + _MAX_EXT, "%s.ini", scenario_name);

	return LoadScenario();
}

int LoadScenario()
{
	CCFileClass file(Scen.ScenarioName);
	
	if (!file.Is_Available())
	{
		return(EDITOR_COMMMAND_FAILURE);
	}


	CCINIClass ini;
	int result = ini.Load(file,true);
	if (result == 0)
	{
		return(EDITOR_COMMMAND_FAILURE);
	}

	if (result == 2) 
	{
		//		if (Session.Type == GAME_NORMAL || Session.ScenarioIsOfficial) {
					/*
					**	Make a special exception so that multiplayer maps from 1 through
					**	24 will not care if the message digest is in error. All other
					**	maps will abort the scenario load.
					*/
		if (Scen.ScenarioName[2] != 'M' || Scen.Scenario >= 25) 
		{
			return(EDITOR_COMMMAND_FAILURE);
		}
	}

	const char * const BASIC = "Basic";
	NewINIFormat = ini.Get_Int(BASIC, "NewINIFormat", 0);

	Map.One_Time();
	Map.Read_INI(ini);

	EditorMapInitialized = true;
	
	return EDITOR_COMMMAND_SUCCESS;
}

/**************************************************************************************************
*	CNC_Editor_Clear_Map
*	Deletes the data for the currently loaded map.
*
* returns EDITOR_COMMMAND_SUCCESS on success, all other values are failure
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Clear_Map()
{
	if (EditorMapInitialized)
	{
		Map.Init_Clear();
		EditorMapInitialized = false;

		return EDITOR_COMMMAND_SUCCESS;
	}
	else
	{
		return EDITOR_COMMMAND_FAILURE;
	}

	return EDITOR_COMMMAND_FAILURE;
}

/**************************************************************************************************
* CNC_Editor_Get_Map_Stats
* Gets the stats for the currently loaded map
*
* map_width: out parameter storing the width of the map
* map_height: out parameter storing the height of the map
* theater: out paramter storing the theater of the map
*
* returns EDITOR_COMMMAND_SUCCESS on success, all other values are failure
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Map_Stats(int& map_width, int& map_height, int& theater)
{
	if (EditorMapInitialized)
	{
		map_width = Map.MapCellWidth + 1;
		map_height = Map.MapCellHeight + 1;
		theater = Scen.Theater;
		return EDITOR_COMMMAND_SUCCESS;
	}

	map_width = -1;
	map_height = -1;
	theater = -1;
	return EDITOR_COMMMAND_FAILURE;
}

/**************************************************************************************************
* CNC_Editor_Get_Cell_Data_By_Index
* Get the data from the given cell.
*
* cell_index: The index of the desired cell.
* cell_name: out buffer to be filled with the name of the given cell.
* cell_name_size: the size of the cell name buffer.
*
*
* returns EDITOR_COMMMAND_SUCCESS on success, all other values are failure
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Cell_Data_By_Index(int cell_index, char* cell_name, unsigned long cell_name_size, int& template_type, int& template_icon_index)
{
	CELL index = (CELL)cell_index;

	CellClass * cellptr = &Map[index];

	cell_name[0] = 0;
	int icon = 0;
	void *image_data = 0;

	char template_name[10];
	if (cellptr->Get_Template_Info(template_name, icon, image_data))
	{
		snprintf(cell_name, cell_name_size, "%s-%04d", template_name, icon);

		template_type = cellptr->TType;
		template_icon_index = icon;

		//TemplateTypeClass::As_Reference(ptr->TType).

		return EDITOR_COMMMAND_SUCCESS;
	}

	return EDITOR_COMMMAND_FAILURE;
}

/**************************************************************************************************
* CNC_Editor_Get_Cell_Data
* Get the data from the given cell.
*
* x,y: The corrdinates of the desired cell.
* cell_name: out buffer to be filled with the name of the given cell.
* cell_name_size: the size of the cell name buffer.
*
* returns EDITOR_COMMMAND_SUCCESS on success, all other values are failure
**************************************************************************************************/

extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Cell_Data(int x, int y, char* cell_name, unsigned long cell_name_size, int& template_type, int& template_icon_index)
{
	if (!EditorMapInitialized)
	{
		return EDITOR_COMMMAND_FAILURE;
	}


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

	CELL cell = XY_Cell(map_cell_x + x, map_cell_y + y);

	return CNC_Editor_Get_Cell_Data_By_Index((int)cell, cell_name, cell_name_size, template_type, template_icon_index);
}

/**************************************************************************************************
* CNC_Editor_Get_Cell_Texture_Buffer
*
* x,y:
* out_width, out_height: dimensions of the outputed texture array
* out_texture_array: output array of unsigned chars storing the color data for the requested object,
*							every 3 chars is a set of RGB values
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Cell_Texture_Buffer(int x, int y, int& out_width, int& out_height, SAFEARRAY*& out_texture_array)
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

	CELL cell = XY_Cell(map_cell_x + x, map_cell_y + y);
	CellClass * cellptr = &Map[cell];

	char cell_name[_MAX_PATH];

	int icon = 0;
	void *image_data = 0;
	if (cellptr->Get_Template_Info(cell_name, icon, image_data))
	{
		GraphicBufferClass temp_gbuffer(24, 24);
		GraphicViewPortClass	temp_viewport(&temp_gbuffer, 0, 0, 24, 24);

		WindowList[WINDOW_CUSTOM][WINDOWX] = 0;
		WindowList[WINDOW_CUSTOM][WINDOWY] = 0;
		WindowList[WINDOW_CUSTOM][WINDOWWIDTH] = 24;
		WindowList[WINDOW_CUSTOM][WINDOWHEIGHT] = 24;

		temp_viewport.Draw_Stamp(image_data, icon, 0, 0, NULL, WINDOW_CUSTOM);

		out_width = temp_viewport.Get_Width();
		out_height = temp_viewport.Get_Height();

		const int COLOR_SIZE = 3;

		SAFEARRAYBOUND  Bound;
		Bound.lLbound = 0;
		Bound.cElements = out_width * out_height * COLOR_SIZE;

		out_texture_array = SafeArrayCreate(VT_UI1, 1, &Bound);

		unsigned char* out_buffer;

		HRESULT hr = SafeArrayAccessData(out_texture_array, (void **)&out_buffer);
		if (SUCCEEDED(hr))
		{
			GraphicBufferClass* Graphic_Buffer = temp_viewport.Get_Graphic_Buffer();

			int VP_Scan_Line = temp_viewport.Get_Width() + temp_viewport.Get_XAdd();

			char * start_ptr;
			start_ptr = (char *)Graphic_Buffer->Get_Buffer();
			start_ptr += ((temp_viewport.Get_YPos() * VP_Scan_Line) + temp_viewport.Get_XPos());

			for (int y = 0; y < out_height; ++y)
			{
				unsigned char* scanline_ptr = (unsigned char*)start_ptr + y * VP_Scan_Line;
				unsigned char* out_buffer_y_ptr = out_buffer + (y * out_width * COLOR_SIZE);
				for (int x = 0; x < out_width; ++x)
				{
					unsigned char* pallete_index_ptr = scanline_ptr + x;
					unsigned char* out_buffer_ptr = out_buffer_y_ptr + (x * COLOR_SIZE);

					int palette_index = (*pallete_index_ptr);
					out_buffer_ptr[0] = ((unsigned char)GamePalette[palette_index].Red_Component());
					out_buffer_ptr[1] = ((unsigned char)GamePalette[palette_index].Green_Component());
					out_buffer_ptr[2] = ((unsigned char)GamePalette[palette_index].Blue_Component());
				}
			}

			SafeArrayUnaccessData(out_texture_array);

			return EDITOR_COMMMAND_SUCCESS;
		}
	}

	return EDITOR_COMMMAND_FAILURE;
}

/**************************************************************************************************
* CNC_Editor_Get_Template_Data
* Get the data from the given tile template type.
*
* template_type_index: The index of the template type to use. should come from the Get_Cell_Data function.
* template_positions: Out buffer to be filled with the list of positions of the tiles as offsets from the origin of the template.
*							This data is store is an X, Y, X, Y, X, Y format.
*
* returns EDITOR_COMMMAND_SUCCESS on success, all other values are failure
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Template_Data(int template_type_index, SAFEARRAY*& template_points)
{
	if (template_type_index >= TEMPLATE_COUNT || template_type_index == TEMPLATE_NONE)
	{
		return EDITOR_COMMMAND_FAILURE;
	}

	const TemplateTypeClass& template_type = TemplateTypeClass::As_Reference((TemplateType)template_type_index);
	if (template_type.Get_Image_Data() == nullptr)
	{
		return EDITOR_COMMMAND_FAILURE;
	}


	short const * occupy_list = template_type.Occupy_List();

	short const * counter = occupy_list;
	while (counter && *counter != REFRESH_EOL)
	{
		counter++;
	}

	int occupy_list_size = counter - occupy_list;


	SAFEARRAYBOUND  bounds;
	bounds.lLbound = 0;
	bounds.cElements = occupy_list_size * 2;
	template_points = SafeArrayCreate(VT_I4, 1, &bounds);

	int *pData;
	HRESULT hr = SafeArrayAccessData(template_points, (void **)&pData);
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < occupy_list_size; i++)
		{
			CELL cell = occupy_list[i];

			int x = Cell_X(cell);
			int y = Cell_Y(cell);

			pData[i * 2] = x;
			pData[i * 2 + 1] = y;
		}

		SafeArrayUnaccessData(template_points);

		return EDITOR_COMMMAND_SUCCESS;
	}

	return EDITOR_COMMMAND_FAILURE;
}

extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Scenario_Names(char* cncdata_directory)
{
	char content_directory[_MAX_PATH];

	CNC_Editor_Setup_Content_Directory(cncdata_directory,false, content_directory);

	char command_line[_MAX_PATH];
	sprintf(command_line, "-CD%s", content_directory);
	DLL_Startup(command_line);

	char team_ids[] =
	{
		'a',
		'b',
		'c',
		'd',
		'e',
		'f',
		'g',
		'h',
		'i',
		'j',
		'k',
		'l',
		'm',
		'n',
		'o',
		'p',
		'q',
		'r',
		's',
		't',
		'u',
		'v',
		'w',
		'x',
		'y',
		'z',
	};
	/*
	{
		'g',
		'a',
		'u',
		'm'
	};
	*/
	const int team_count = sizeof(team_ids) / sizeof(char);

	char direction_ids[] =
	{
		'e',
		'w',
	};
	const int direction_count = sizeof(direction_ids) / sizeof(char);

	char variant_ids[] =
	{
		'a',
		'b',
		'c',
		'd',
		'e',
		'f',
		'g',
		'h',
		'i',
		'j',
		'k',
		'l',
		'm',
		'n',
		'o',
		'p',
		'q',
		'r',
		's',
		't',
		'u',
		'v',
		'w',
		'x',
		'y',
		'z',
	};
	/*
	{
		'a',
		'b',
		'c',
		'd'
	};
	*/
	const int variant_count = sizeof(variant_ids) / sizeof(char);

	const int min_scenario_index = 1;
	const int max_scenario_index = 99;

	char scenario_name[_MAX_FNAME + _MAX_EXT];
	char file_name[_MAX_FNAME + _MAX_EXT];

	FILE * names_file = fopen("d:\\RA_Disk2.txt", "w+");

	for (int team_index = 0; team_index < team_count; team_index++)
	{
		for (int scenario_index = min_scenario_index; scenario_index <= max_scenario_index; ++scenario_index)
		{
			for (int direction_index = 0; direction_index < direction_count; direction_index++)
			{
				for (int variant_index = 0; variant_index < variant_count; variant_index++)
				{
					sprintf(scenario_name, "sc%c%.2d%c%c",
						team_ids[team_index],
						scenario_index,
						direction_ids[direction_index],
						variant_ids[variant_index]);

					sprintf(file_name, "%s.INI", scenario_name);
					CCFileClass file(file_name);
					if (file.Is_Available())
					{
						fprintf(names_file, "%s\n", scenario_name);
					}
				}
			}
		}
	}

	fclose(names_file);

	return EDITOR_COMMMAND_SUCCESS;
}