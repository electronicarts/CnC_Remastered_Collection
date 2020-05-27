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

#include <stdio.h>
//#include <iostream>
//#include <fstream>

#include	"function.h"
#include "externs.h"
#include "DLLInterface.h"
#include "Gadget.h"
#include "defines.h" // VOC_COUNT, VOX_COUNT
#include "SidebarGlyphx.h"
#include "TEMPLATE.H"

extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Startup();
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Cleanup();
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Load_Map(char* cncdata_directory, char* house_name, int scenario_index, char* east_west, char* variant);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Load_Map_By_Scenario_Name(char* cncdata_directory, char* scenario_name);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Clear_Map();
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Map_Stats(int& map_width, int& map_height, int& theater);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Cell_Data_By_Index(int cell_index, char* cell_name, unsigned long cell_name_size, int& template_type, int& template_icon_index);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Cell_Data(int x, int y, char* cell_name, unsigned long cell_name_size, int& template_type, int& template_icon_index);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Cell_Texture_Buffer(int x, int y, int& out_width, int& out_height, SAFEARRAY*& out_texture_array);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Scenario_Names(char* cncdata_directory, int CD);
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Template_Data(int template_type_index, SAFEARRAY*& template_points);

char EditorMapINIBuffer[SHAPE_BUFFER_SIZE];
bool EditorMapInitialized = false;

const static int EDITOR_COMMMAND_SUCCESS = 0;
const static int EDITOR_COMMMAND_FAILURE = 1;

const static char* MixFileNames[] =
{
	"GENERAL.MIX",
	"SC-000.MIX",
	"SC-001.MIX",
	"DESERT.MIX",
	"TEMPERAT.MIX",
	"WINTER.MIX"
};

extern MixFileClass *TheaterIcons;
extern bool Read_Movies_From_Scenario_Ini(char *root, bool fresh);

/**************************************************************************************************
* CNC_Editor_Startup
* Initializes the system to allow map loading for the editor
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Startup()
{
	BlackPalette = new(MEM_CLEAR | MEM_REAL) unsigned char[768];
	GamePalette = new(MEM_CLEAR | MEM_REAL) unsigned char[768];
	OriginalPalette = new(MEM_CLEAR | MEM_REAL) unsigned char[768];
	WhitePalette = new(MEM_CLEAR | MEM_REAL) unsigned char[768];
	memset(WhitePalette, 63, 768);


	Set_Palette(GamePalette);

	TheaterData = 0;
	TheaterIcons = 0;
	LowTheaterData = 0;

	return EDITOR_COMMMAND_SUCCESS;
}

/**************************************************************************************************
* CNC_Editor_Cleanup
* Cleans up systems initialized by the editor
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Cleanup()
{
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

	return EDITOR_COMMMAND_SUCCESS;
}
/**************************************************************************************************
* CNC_Editor_Load_Mix_Files
* Loads all the Mix files for Tiberian Dawn
**************************************************************************************************/
void CNC_Editor_Load_Mix_Files()
{
	int count = sizeof(MixFileNames) / sizeof(MixFileNames[0]);

	for (int i = count - 1; i >= 0; --i)
	{
		MixFileClass::Free(MixFileNames[i]);
	}

	for (int i = 0; i < count; ++i)
	{
		new MixFileClass(MixFileNames[i]);
		MixFileClass::Cache(MixFileNames[i]);
	}
}

/**************************************************************************************************
* CNC_Editor_Setup_Content_Directory
* Sets up where the system should load map data from.
**************************************************************************************************/
void CNC_Editor_Setup_Content_Directory(char* cncdata_directory, char* cd_directory)
{
	char content_directory[_MAX_PATH];

	sprintf(content_directory, "%s\\TIBERIAN_DAWN\\%s\\", cncdata_directory, cd_directory);

	if (strlen(content_directory) != 0) {
		CCFileClass::Clear_Search_Drives();
		CCFileClass::Reset_Raw_Path();
		char *dll_dir = strdup(content_directory);
		CCFileClass::Set_Search_Drives(dll_dir);
		free(dll_dir);
	}
}

void CNC_Editor_Setup_Content_Directory(char* cncdata_directory, int CD_index)
{
	char cd_name[_MAX_PATH];
	sprintf(cd_name, "CD%i", CD_index);
	CNC_Editor_Setup_Content_Directory(cncdata_directory, cd_name);
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

	int CD = 1;
	if (stricmp(house_name, "GDI") == 0) {
		ScenPlayer = SCEN_PLAYER_GDI;
		GameToPlay = GAME_NORMAL;

		if (scenario_index == 30 || scenario_index == 90) {
			CD = 5;
		}
		else if (scenario_index >= 60 && scenario_index <= 72) {
			CD = 4;
		}
		else if (scenario_index >= 20 && scenario_index < 60) {
			CD = 3;
		}
	}

	if (stricmp(house_name, "NOD") == 0) {
		ScenPlayer = SCEN_PLAYER_NOD;
		GameToPlay = GAME_NORMAL;
		CD = 2;

		// Hack a fix for scenario 21 since the same mission number is used in Covert Ops and N64
		if (scenario_index == 81 || scenario_index == 22) {
			CD = 5;
			if (scenario_index == 81) {
				scenario_index = 21;
			}
		}
		else if (scenario_index >= 60 && scenario_index <= 61) {
			CD = 4;
		}
		else if (scenario_index >= 20 && scenario_index < 60) {
			CD = 3;
		}
	}

	if (stricmp(house_name, "MULTI") == 0)
	{
		ScenPlayer = SCEN_PLAYER_MPLAYER;
		GameToPlay = GAME_GLYPHX_MULTIPLAYER;
	}

	if (stricmp(house_name, "JUR") == 0)
	{
		ScenPlayer = SCEN_PLAYER_JP;
		GameToPlay = GAME_NORMAL;
	}

	switch (CD)
	{
		case 4:
			CNC_Editor_Setup_Content_Directory(cncdata_directory, "CONSOLE_1");
			break;
		case 5:
			CNC_Editor_Setup_Content_Directory(cncdata_directory, "CONSOLE_2");
			break;
		default:
			CNC_Editor_Setup_Content_Directory(cncdata_directory, CD);
			break;
	}

	CNC_Editor_Load_Mix_Files();

	Scenario = scenario_index;
	BuildLevel = 7;

	if (stricmp(east_west, "w") == 0)
	{
		ScenDir = SCEN_DIR_WEST;
	}
	else
	{
		ScenDir = SCEN_DIR_EAST;
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

	Set_Scenario_Name(ScenarioName, Scenario, ScenPlayer, ScenDir, variant_enum);

	char fname[_MAX_FNAME + _MAX_EXT];
	sprintf(fname, "%s.INI", ScenarioName);
	CCFileClass file(fname);
	if (!file.Is_Available())
	{
		return(EDITOR_COMMMAND_FAILURE);
	}
	else
	{
		memset(EditorMapINIBuffer, 0, SHAPE_BUFFER_SIZE);
		file.Read(EditorMapINIBuffer, SHAPE_BUFFER_SIZE - 1);
	}

	Map.One_Time_Editor();
	Map.Read_INI(EditorMapINIBuffer);
	if (Map.Read_Binary(ScenarioName, &ScenarioCRC))
	{
		EditorMapInitialized = true;
		return EDITOR_COMMMAND_SUCCESS;
	}
	else
	{
		return EDITOR_COMMMAND_FAILURE;
	}
}


extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Load_Map_By_Scenario_Name(char* cncdata_directory, char* scenario_name)
{
	return EDITOR_COMMMAND_FAILURE;
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
		
		int count = sizeof(MixFileNames) / sizeof(MixFileNames[0]);
		for (int i = count - 1; i >= 0; --i)
		{
			MixFileClass::Free(MixFileNames[i]);
		}

		EditorMapInitialized = false;
		TheaterData = nullptr;
		TheaterIcons = nullptr;

		return EDITOR_COMMMAND_SUCCESS;
	}
	else
	{
		return EDITOR_COMMMAND_FAILURE;
	}

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
		theater = Map.Theater;
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
	CellClass * cellptr = &Map[cell_index];

	cell_name[0] = 0;
	int icon = 0;
	void *image_data = 0;

	char template_name[10];
	if (cellptr->Get_Template_Info(template_name, icon, image_data))
	{
		snprintf(cell_name, cell_name_size, "%s-%04d", template_name, icon);

		template_type = cellptr->TType;
		template_icon_index = icon;

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

					int red_palette_index = (*pallete_index_ptr) * COLOR_SIZE;
					out_buffer_ptr[0] = GamePalette[red_palette_index] << 2;
					out_buffer_ptr[1] = GamePalette[red_palette_index + 1] << 2;
					out_buffer_ptr[2] = GamePalette[red_palette_index + 2] << 2;
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
* template_points: Out buffer to be filled with the list of positions of the tiles as offsets from the origin of the template.
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


/**************************************************************************************************
* CNC_Editor_Get_Scenario_Names
* 
**************************************************************************************************/
extern "C" __declspec(dllexport) int __cdecl CNC_Editor_Get_Scenario_Names(char* cncdata_directory, int CD)
{
	unsigned char read_buffer[SHAPE_BUFFER_SIZE];
	Set_Shape_Buffer(read_buffer, SHAPE_BUFFER_SIZE);

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
		'b',
		'm',
		'j'
	};
	*/
	const int team_count = sizeof(team_ids) / sizeof(char);

	char direction_ids[] =
	{
		'e',
		'w'
	};
	const int direction_count = sizeof(direction_ids) / sizeof(char);

	char variant_ids[] =
	{
	'a',
	'b',
	'c',
	'd'
	};

	const int variant_count = sizeof(variant_ids) / sizeof(char);

	const int min_scenario_index = 1;
	const int max_scenario_index = 99;

	char scenario_name[_MAX_FNAME + _MAX_EXT];
	char file_name[_MAX_FNAME + _MAX_EXT];

	CNC_Editor_Setup_Content_Directory(cncdata_directory, CD);
	CNC_Editor_Load_Mix_Files();

	char output_file_name[256];
	snprintf(output_file_name, 256, "d:\\TD_Disk%d_Names.txt", CD);

	FILE * names_file = fopen(output_file_name, "w+");

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
						if (Read_Movies_From_Scenario_Ini(scenario_name, false))
						{
							//fprintf(names_file, "%s - \t$Intro:%s \t\t$Brief:%s \t\t$Win:%s \t\t$Lose:%s \t\t$Action:%s \t\t$Theme:%s", scenario_name, IntroMovie, BriefMovie, WinMovie, LoseMovie, ActionMovie, MovieThemeName);
							fprintf(names_file, "------------------%s-------------- %s \n", scenario_name, MovieThemeName);
							
							if (stricmp("x", IntroMovie) != 0)
							{
								fprintf(names_file, "<IntroMovieName network=\"client\">%s</IntroMovieName>\n", IntroMovie);
							}
							if (stricmp("x", BriefMovie) != 0)
							{
								fprintf(names_file, "<BriefMovieName network=\"client\">%s</BriefMovieName>\n", BriefMovie);
							}
							if (stricmp("x", ActionMovie) != 0)
							{
								fprintf(names_file, "<ActionMovieName network=\"client\">%s</ActionMovieName>\n", ActionMovie);
							}
							if (stricmp("x", WinMovie) != 0)
							{
								fprintf(names_file, "<WinMovieName network=\"client\">%s</WinMovieName>\n", WinMovie);
							}
						}

						fprintf(names_file, "\n");
					}


					
				}
			}
		}
	}

	fclose(names_file);

	return EDITOR_COMMMAND_SUCCESS;
}
