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

/* $Header:   F:\projects\c&c\vcs\code\saveload.cpv   2.18   16 Oct 1995 16:48:44   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SAVELOAD.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 23, 1994                                              *
 *                                                                                             *
 *                  Last Update : June 24, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Code_All_Pointers -- Code all pointers.                                                   *
 *   Decode_All_Pointers -- Decodes all pointers.                                              *
 *   Get_Savefile_Info -- gets description, scenario #, house                                  *
 *   Load_Game -- loads a saved game                                                           *
 *   Load_Misc_Values -- Loads miscellaneous variables.                                        *
 *   Load_Misc_Values -- loads miscellaneous variables                                         *
 *   Read_Object -- reads an object from disk, in a safe way                                   *
 *   Save_Game -- saves a game to disk                                                         *
 *   Save_Misc_Values -- saves miscellaneous variables                                         *
 *   Target_To_TechnoType -- converts TARGET to TechnoTypeClass                                *
 *   TechnoType_To_Target -- converts TechnoTypeClass to TARGET                                *
 *   Write_Object -- reads an object from disk, in a safe way                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

extern bool DLLSave(FileClass &file);
extern bool DLLLoad(FileClass &file);

		
/*
********************************** Defines **********************************
*/
#define	SAVEGAME_VERSION		(DESCRIP_MAX +						\
										0x01000003 + ( 					\
										sizeof(AircraftClass) + 		\
										sizeof(AircraftTypeClass) + 	\
										sizeof(AnimClass) + 				\
										sizeof(AnimTypeClass) + 		\
										sizeof(BuildingClass) + 		\
										sizeof(BuildingTypeClass) + 	\
										sizeof(BulletClass) + 			\
										sizeof(BulletTypeClass) + 		\
										sizeof(HouseClass) + 			\
										sizeof(HouseTypeClass) + 		\
										sizeof(InfantryClass) + 		\
										sizeof(InfantryTypeClass) + 	\
										sizeof(OverlayClass) + 			\
										sizeof(OverlayTypeClass) + 	\
										sizeof(SmudgeClass) + 			\
										sizeof(SmudgeTypeClass) + 		\
										sizeof(TeamClass) + 				\
										sizeof(TeamTypeClass) + 		\
										sizeof(TemplateClass) + 		\
										sizeof(TemplateTypeClass) + 	\
										sizeof(TerrainClass) + 			\
										sizeof(TerrainTypeClass) + 	\
										sizeof(UnitClass) + 				\
										sizeof(UnitTypeClass) + 		\
										sizeof(MouseClass) + 			\
										sizeof(CellClass) + 				\
										sizeof(FactoryClass) + 			\
										sizeof(BaseClass) + 				\
										sizeof(LayerClass) +				\
										sizeof(BriefingText) + \
										sizeof(Waypoint)))


/***************************************************************************
 * Save_Game -- saves a game to disk                                       *
 *                                                                         *
 * Saving the Map:                                                         *
 *     DisplayClass::Save() invokes CellClass's Write() for every cell     *
 *     that needs to be saved.  A cell needs to be saved if it contains    *
 *     any special data at all, such as a TIcon, or an Occupier.           *
 *   The cell saves its own CellTrigger pointer, converted to a TARGET.    *
 *                                                                         *
 * Saving game objects:                                                    *
 *   - Any object stored in an ArrayOf class needs to be saved.  The ArrayOf*
 *     Save() routine invokes each object's Write() routine, if that       *
 *     object's IsActive is set.                                           *
 *                                                                         *
 * Saving the layers:                                                      *
 *   The Map's Layers (Ground, Air, etc) of things that are on the map,    *
 *     and the Logic's Layer of things to process both need to be saved.   *
 *     LayerClass::Save() writes the entire layer array to disk            *
 *                                                                         *
 * Saving the houses:                                                      *
 *   Each house needs to be saved, to record its Credits, Power, etc.      *
 *                                                                         *
 * Saving miscellaneous data:                                              *
 *   There are a lot of miscellaneous variables to save, such as the       *
 *     map's dimensions, the player's house, etc.                          *
 *                                                                         *
 * INPUT:                                                                  *
 *      id      numerical ID, for the file extension                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = OK, false = error                                           *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/28/1994 BR : Created.                                              *
 *=========================================================================*/
bool Save_Game(int id,char *descr)
{
	char name[_MAX_FNAME+_MAX_EXT];

	/*
	**	Generate the filename to save
	*/
	sprintf(name, "SAVEGAME.%03d", id);

	return Save_Game(name, descr);
}


/*
** Version that takes file name. ST - 9/9/2019 11:10AM
*/
bool Save_Game(const char *file_name, const char *descr)
{
	RawFileClass file;
	int i;
	unsigned long version;
	unsigned scenario;
	HousesType house;
	char descr_buf[DESCRIP_MAX];

	scenario = Scenario;								// get current scenario #
	house = PlayerPtr->Class->House;				// get current house

	/*
	**	Code everybody's pointers
	*/
	Code_All_Pointers();

	/*
	**	Open the file
	*/
	if (!file.Open(file_name, WRITE)) {
		Decode_All_Pointers();
		return(false);
	}

	/*
	** Save the DLLs variables first, so we can do a version check in the DLL when we begin the load
	*/
	if (RunningAsDLL) {
		if (!DLLSave(file)) {
			file.Close();
			Decode_All_Pointers();
			return false;
		}
	}

	/*
	**	Save the description, scenario #, and house
	**	(scenario # & house are saved separately from the actual Scenario &
	**	PlayerPtr globals for convenience; we can quickly find out which
	**	house & scenario this save-game file is for by reading these values.
	**	Also, PlayerPtr is stored in a coded form in Save_Misc_Values(),
	**	which may or may not be a HousesType number; so, saving 'house'
	**	here ensures we can always pull out the house for this file.)
	*/
	sprintf(descr_buf, "%s\r\n",descr);			// put CR-LF after text
	descr_buf[strlen(descr_buf) + 1] = 26;		// put CTRL-Z after NULL

	if (file.Write(descr_buf, DESCRIP_MAX) != DESCRIP_MAX) {
		file.Close();
		return(false);
	}

	if (file.Write(&scenario, sizeof(scenario)) != sizeof(scenario)) {
		file.Close();
		return(false);
	}

	if (file.Write(&house, sizeof(house)) != sizeof(house)) {
		file.Close();
		return(false);
	}

	/*
	**	Save the save-game version, for loading verification
	*/
	version = SAVEGAME_VERSION;

	if (file.Write(&version, sizeof(version)) != sizeof(version)) {
		file.Close();
		return(false);
	}

	Call_Back();
	/*
	**	Save the map.  The map must be saved first, since it saves the Theater.
	*/
	Map.Save(file);

	Call_Back();
	/*
	**	Save all game objects.  This code saves every object that's stored in a
	**	TFixedIHeap class.
	*/
	if (!Houses.Save(file)			||
		!TeamTypes.Save(file)		||
		!Teams.Save(file)				||
		!Triggers.Save(file)			||
		!Aircraft.Save(file)			||
		!Anims.Save(file)				||
		!Buildings.Save(file)		||
		!Bullets.Save(file)			||
		!Infantry.Save(file)			||
		!Overlays.Save(file)			||
		!Smudges.Save(file)			||
		!Templates.Save(file)		||
		!Terrains.Save(file)			||
		!Units.Save(file)				||
		!Factories.Save(file)) {
		file.Close();

		Decode_All_Pointers();

		return(false);
	}

	Call_Back();
	/*
	**	Save the Logic & Map layers
	*/
	if (!Logic.Save(file)) {
		file.Close();
		Decode_All_Pointers();
		return(false);
	}

	for (i = 0; i < LAYER_COUNT; i++) {
		if (!Map.Layer[i].Save(file)) {
			file.Close();
			Decode_All_Pointers();
			return(false);
		}
	}

	/*
	**	Save the Score
	*/
	if (!Score.Save(file)) {
		file.Close();
		Decode_All_Pointers();
		return(false);
	}

	/*
	**	Save the AI Base
	*/
	if (!Base.Save(file)) {
		file.Close();
		Decode_All_Pointers();
		return(false);
	}

	/*
	**	Save miscellaneous variables.
	*/
	if (!Save_Misc_Values(file)) {
		file.Close();
		Decode_All_Pointers();
		return(false);
	}

	Call_Back();

	/*
	**	Close the file; we're done
	*/
	file.Close();
	Decode_All_Pointers();

	return(true);
}


/***************************************************************************
 * Load_Game -- loads a saved game                                         *
 *                                                                         *
 * This routine loads the data in the same way it was saved out.           *
 *                                                                         *
 * Loading the Map:                                                        *
 *   - DisplayClass::Load() invokes CellClass's Load() for every cell      *
 *     that was saved.                                                     *
 * - The cell loads its own CellTrigger pointer.                           *
 *                                                                         *
 * Loading game objects:                                                   *
 * - IHeap's Load() routine loads the # of objects stored, and loads       *
 *   each object.                                                          *
 * - Triggers: Add themselves to the HouseTriggers if they're associated   *
 *   with a house                                                          *
 *                                                                         *
 * Loading the layers:                                                     *
 *     LayerClass::Load() reads the entire layer array to disk             *
 *                                                                         *
 * Loading the houses:                                                     *
 *   Each house is loaded in its entirety.                                 *
 *                                                                         *
 * Loading miscellaneous data:                                             *
 *   There are a lot of miscellaneous variables to load, such as the       *
 *     map's dimensions, the player's house, etc.                          *
 *                                                                         *
 * INPUT:                                                                  *
 *      id         numerical ID, for the file extension                    *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = OK, false = error                                           *
 *                                                                         *
 * WARNINGS:                                                               *
 *      If this routine returns false, the entire game will be in an       *
 *      unknown state, so the scenario will have to be re-initialized.     *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/28/1994 BR : Created.                                              *
 *=========================================================================*/
bool Load_Game(int id)
{
	char name[_MAX_FNAME+_MAX_EXT];
	
	/*
	**	Generate the filename to load
	*/
	sprintf(name, "SAVEGAME.%03d", id);
	
	return Load_Game(name);
}

/*
** Version that takes a file name instead. ST - 9/9/2019 11:13AM
*/
bool Load_Game(const char *file_name)
{
	RawFileClass file;
	int i;
	unsigned long version;
	unsigned scenario;
	HousesType house;
	char descr_buf[DESCRIP_MAX];

	/*
	**	Open the file
	*/
	if (!file.Open(file_name, READ)) {
		return(false);
	}

	/*
	** Load the DLLs variables first, in case we need to do something different based on version
	*/
	if (RunningAsDLL) {
		if (!DLLLoad(file)) {
			file.Close();
			return false;
		}
	}

	/*
	**	Read & discard the save-game's header info
	*/
	if (file.Read(descr_buf, DESCRIP_MAX) != DESCRIP_MAX) {
		file.Close();
		return(false);
	}

	if (file.Read(&scenario, sizeof(scenario)) != sizeof(scenario)) {
		file.Close();
		return(false);
	}

	if (file.Read(&house, sizeof(house)) != sizeof(house)) {
		file.Close();
		return(false);
	}

	Call_Back();
	/*
	**	Clear the scenario so we start fresh; this calls the Init_Clear() routine
	**	for the Map, and all object arrays.  It has the following important
	**	effects:
	**	- Every cell is cleared to 0's, via MapClass::Init_Clear()
	**	- All heap elements' are cleared
	**	- The Houses are Initialized, which also clears their HouseTriggers
	**	  array
	**	- The map's Layers & Logic Layer are cleared to empty
	**	- The list of currently-selected objects is cleared
	*/
	Clear_Scenario();

	/*
	**	Read in & verify the save-game ID code
	*/
	if (file.Read(&version,sizeof(version)) != sizeof(version)) {
		file.Close();
		return(false);
	}

	if (version != SAVEGAME_VERSION) {
		file.Close();
		return(false);
	}

	Call_Back();
	/*
	**	Set the required CD to be in the drive according to the scenario
	**	loaded.
	*/
	if (RequiredCD != -2) {
		if (scenario >= 20 && scenario <60 && GameToPlay == GAME_NORMAL) {
			RequiredCD = 2;
		} else {
			if (scenario >= 60){
				/*
				** This is a gateway bonus scenario
				*/
				RequiredCD = -1;
			}else{
				if (house == HOUSE_GOOD) {
					RequiredCD = 0;
				} else {
					RequiredCD = 1;
				}
			}
		}
	}
	if(!Force_CD_Available(RequiredCD)) {
		Prog_End("Load_Game - CD not found", true);
		if (!RunningAsDLL) {
			exit(EXIT_FAILURE);
		}
		return false;
	}

	Call_Back();

	/*
	**	Load the map.  The map comes first, since it loads the Theater & init's
	**	mixfiles.  The map calls all the type-class's Init routines, telling them
	**	what the Theater is; this must be done before any objects are created, so
	**	they'll be properly created.
	*/
	Map.Load(file);

	Call_Back();
	/*
	**	Load the object data.
	*/
	if (!Houses.Load(file)			||
		!TeamTypes.Load(file)		||
		!Teams.Load(file)				||
		!Triggers.Load(file)			||
		!Aircraft.Load(file)			||
		!Anims.Load(file)				||
		!Buildings.Load(file)		||
		!Bullets.Load(file)			||
		!Infantry.Load(file)			||
		!Overlays.Load(file)			||
		!Smudges.Load(file)			||
		!Templates.Load(file)		||
		!Terrains.Load(file)			||
		!Units.Load(file)				||
		!Factories.Load(file)) {
		file.Close();
		return(false);
	}

	Call_Back();
	/*
	**	Load the Logic & Map Layers
	*/
	if (!Logic.Load(file)) {
		file.Close();
		return(false);
	}
	for (i = 0; i < LAYER_COUNT; i++) {
		if (!Map.Layer[i].Load(file)) {
			file.Close();
			return(false);
		}
	}

	Call_Back();
	/*
	**	Load the Score
	*/
	if (!Score.Load(file)) {
		file.Close();
		return(false);
	}

	/*
	**	Load the AI Base
	*/
	if (!Base.Load(file)) {
		file.Close();
		return(false);
	}

	/*
	**	Load miscellaneous variables, including the map size & the Theater
	*/
	if (!Load_Misc_Values(file)) {
		file.Close();
		return(false);
	}

	file.Close();
	Decode_All_Pointers();
	Map.Init_IO();
	Map.Flag_To_Redraw(true);

	ScenarioInit = 0;

	/*
	** Fixup remap tables. ST - 2/28/2020 1:50PM
	*/
	for (HousesType house = HOUSE_MULTI1; house < HOUSE_COUNT; house++) {
		HouseClass * hptr = HouseClass::As_Pointer(house);
		if (hptr && hptr->IsActive) {
			hptr->Init_Data(hptr->RemapColor, hptr->ActLike, hptr->Credits);
		}
	}

	/*
	** Re-init unit trackers. They will be garbage pointers after the load
	*/
	for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		HouseClass * hptr = HouseClass::As_Pointer(house);
		if (hptr && hptr->IsActive) {
			hptr->Init_Unit_Trackers();
		}
	}

#ifdef DEMO
	if (Scenario != 10 && Scenario != 1 && Scenario != 6) {
		Clear_Scenario();
		return(false);
	}
#endif

	Call_Back();
	return(true);
}


/***************************************************************************
 * Save_Misc_Values -- saves miscellaneous variables                       *
 *                                                                         *
 * INPUT:                                                                  *
 *      file      file to use for writing                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = success, false = failure                                    *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/29/1994 BR : Created.                                              *
 *=========================================================================*/
bool Save_Misc_Values(FileClass &file)
{
	int i, j;
	int count;								// # ptrs in 'CurrentObject'
	ObjectClass * ptr;					// for saving 'CurrentObject' ptrs

	/*
	**	Player's House.
	*/
	if (file.Write(&PlayerPtr, sizeof(PlayerPtr)) != sizeof(PlayerPtr)) {
		return(false);
	}

	/*
	**	Save this scenario number.
	*/
	if (file.Write(&Scenario, sizeof(Scenario)) != sizeof(Scenario)) {
		return(false);
	}

	/*
	**	Save frame #.
	*/
	if (file.Write(&Frame, sizeof(Frame)) != sizeof(Frame)) {
		return(false);
	}

	/*
	**	Save VQ Movie names.
	*/
	if (file.Write(WinMovie, sizeof(WinMovie)) != sizeof(WinMovie)) {
		return(false);
	}

	if (file.Write(LoseMovie, sizeof(LoseMovie)) != sizeof(LoseMovie)) {
		return(false);
	}

	/*
	**	Save currently-selected objects list.
	**	Save the # of ptrs in the list.
	*/
	for (i = 0; i < SelectedObjectsType::COUNT; i++) {
		DynamicVectorClass<ObjectClass *>& selection = CurrentObject.Raw(i);
		count = selection.Count();
		if (file.Write(&count, sizeof(count)) != sizeof(count)) {
			return(false);
		}

		/*
		**	Save the pointers.
		*/
		for (j = 0; j < count; j++) {
			ptr = selection[j];
			if (file.Write(&ptr, sizeof(ptr)) != sizeof(ptr)) {
				return(false);
			}
		}
	}

	/*
	**	Save the list of waypoints.
	*/
	if (file.Write(Waypoint, sizeof(Waypoint)) != sizeof(Waypoint)) {
		return(false);
	}

	file.Write(&ScenDir, sizeof(ScenDir));
	file.Write(&ScenVar, sizeof(ScenVar));
	file.Write(&CarryOverMoney, sizeof(CarryOverMoney));
	file.Write(&CarryOverPercent, sizeof(CarryOverPercent));
	file.Write(&BuildLevel, sizeof(BuildLevel));
	file.Write(BriefMovie, sizeof(BriefMovie));
	file.Write(Views, sizeof(Views));
	file.Write(&EndCountDown, sizeof(EndCountDown));
	file.Write(BriefingText, sizeof(BriefingText));

	// This is new...
	file.Write(ActionMovie, sizeof(ActionMovie));

	return(true);
}


/***********************************************************************************************
 * Load_Misc_Values -- Loads miscellaneous variables.                                          *
 *                                                                                             *
 * INPUT:   file  -- The file to load the misc values from.                                    *
 *                                                                                             *
 * OUTPUT:  Was the misc load process successful?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
bool Load_Misc_Values(FileClass &file)
{
	int i, j;
	int count;								// # ptrs in 'CurrentObject'
	ObjectClass * ptr;					// for loading 'CurrentObject' ptrs

	/*
	**	Player's House.
	*/
	if (file.Read(&PlayerPtr, sizeof(PlayerPtr)) != sizeof(PlayerPtr)) {
		return(false);
	}

	/*
	**	Read this scenario number.
	*/
	if (file.Read(&Scenario,sizeof(Scenario)) != sizeof(Scenario)) {
		return(false);
	}

	/*
	**	Load frame #.
	*/
	if (file.Read(&Frame, sizeof(Frame)) != sizeof(Frame)) {
		return(false);
	}

	/*
	**	Load VQ Movie names.
	*/
	if (file.Read(WinMovie, sizeof(WinMovie)) != sizeof(WinMovie)) {
		return(false);
	}

	if (file.Read(LoseMovie, sizeof(LoseMovie)) != sizeof(LoseMovie)) {
		return(false);
	}

	for (i = 0; i < SelectedObjectsType::COUNT; i++) {
		/*
		**	Load currently-selected objects list.
		**	Load the # of ptrs in the list.
		*/
		DynamicVectorClass<ObjectClass *>& selection = CurrentObject.Raw(i);
		if (file.Read(&count, sizeof(count)) != sizeof(count)) {
			return(false);
		}

		/*
		**	Load the pointers.
		*/
		for (j = 0; j < count; j++) {
			if (file.Read(&ptr, sizeof(ptr)) != sizeof(ptr)) {
				return(false);
			}
			selection.Add(ptr);	// add to the list
		}
	}

	/*
	**	Save the list of waypoints.
	*/
	if (file.Read(Waypoint, sizeof(Waypoint)) != sizeof(Waypoint)) {
		return(false);
	}

	file.Read(&ScenDir, sizeof(ScenDir));
	file.Read(&ScenVar, sizeof(ScenVar));
	file.Read(&CarryOverMoney, sizeof(CarryOverMoney));
	file.Read(&CarryOverPercent, sizeof(CarryOverPercent));
	file.Read(&BuildLevel, sizeof(BuildLevel));
	file.Read(BriefMovie, sizeof(BriefMovie));
	file.Read(Views, sizeof(Views));
	file.Read(&EndCountDown, sizeof(EndCountDown));
	file.Read(BriefingText, sizeof(BriefingText));

	if (file.Seek(0, SEEK_CUR) < file.Size()) {
		file.Read(ActionMovie, sizeof(ActionMovie));
	}

	return(true);
}


/*
** ST - 9/26/2019 11:43AM
*/
extern void DLL_Code_Pointers(void);
extern void DLL_Decode_Pointers(void);


/***********************************************************************************************
 * Code_All_Pointers -- Code all pointers.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void Code_All_Pointers(void)
{
	int i, j;

	/*
	**	The Map.
	*/
	Map.Code_Pointers();

	/*
	**	The ArrayOf's.
	*/
	TeamTypes.Code_Pointers();
	Teams.Code_Pointers();
	Triggers.Code_Pointers();
	Aircraft.Code_Pointers();
	Anims.Code_Pointers();
	Buildings.Code_Pointers();
	Bullets.Code_Pointers();
	Infantry.Code_Pointers();
	Overlays.Code_Pointers();
	Smudges.Code_Pointers();
	Templates.Code_Pointers();
	Terrains.Code_Pointers();
	Units.Code_Pointers();
	Factories.Code_Pointers();

	/*
	**	The Layers.
	*/
	Logic.Code_Pointers();
	for (i = 0; i < LAYER_COUNT; i++) {
		Map.Layer[i].Code_Pointers();
	}

	/*
	**	The Score.
	*/
	Score.Code_Pointers();

	/*
	**	The Base.
	*/
	Base.Code_Pointers();

	/*
	**	PlayerPtr.
	*/
	PlayerPtr = (HouseClass *)(PlayerPtr->Class->House);

	/*
	**	Currently-selected objects.
	*/
	for (i = 0; i < SelectedObjectsType::COUNT; i++) {
		DynamicVectorClass<ObjectClass *>& selection = CurrentObject.Raw(i);
		for (j = 0; j < selection.Count(); j++) {
			selection[j] = (ObjectClass *)selection[j]->As_Target();
		}
	}

	/*
	** DLL data
	*/
	DLL_Code_Pointers();

	/*
	** Houses must be coded last, because the Class->House member of the HouseClass
	** is used to code HouseClass pointers for all other objects, and if Class is
	** coded, it will point to a meaningless value.
	*/
	Houses.Code_Pointers();
}


/***********************************************************************************************
 * Decode_All_Pointers -- Decodes all pointers.                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void Decode_All_Pointers(void)
{
	int i, j;

	/*
	**	The Map.
	*/
	Map.Decode_Pointers();

	/*
	** Decode houses first, so we can properly decode all other objects'
	** House pointers
	*/
	Houses.Decode_Pointers();

	/*
	** DLL data
	*/
	DLL_Decode_Pointers();

	/*
	**	The ArrayOf's.
	*/
	TeamTypes.Decode_Pointers();
	Teams.Decode_Pointers();
	Triggers.Decode_Pointers();
	Aircraft.Decode_Pointers();
	Anims.Decode_Pointers();
	Buildings.Decode_Pointers();
	Bullets.Decode_Pointers();
	Infantry.Decode_Pointers();
	Overlays.Decode_Pointers();
	Smudges.Decode_Pointers();
	Templates.Decode_Pointers();
	Terrains.Decode_Pointers();
	Units.Decode_Pointers();
	Factories.Decode_Pointers();

	/*
	**	The Layers.
	*/
	Logic.Decode_Pointers();
	for (i = 0; i < LAYER_COUNT; i++) {
		Map.Layer[i].Decode_Pointers();
	}

	/*
	**	The Score.
	*/
	Score.Decode_Pointers();

	/*
	**	The Base.
	*/
	Base.Decode_Pointers();

	/*
	**	PlayerPtr.
	*/
	PlayerPtr = HouseClass::As_Pointer(*((HousesType*)&PlayerPtr));
	Whom = PlayerPtr->Class->House;
	switch (PlayerPtr->Class->House) {
		case HOUSE_GOOD:
			ScenPlayer = SCEN_PLAYER_GDI;
			break;

		case HOUSE_BAD:
			ScenPlayer = SCEN_PLAYER_NOD;
			break;

		case HOUSE_JP:
			ScenPlayer = SCEN_PLAYER_JP;
			break;
	}
	Check_Ptr(PlayerPtr,__FILE__,__LINE__);
	
	if (PlayerPtr->ActLike == HOUSE_JP) {
		ScenPlayer = SCEN_PLAYER_JP;
	}
	Set_Scenario_Name(ScenarioName, Scenario, ScenPlayer, ScenDir, ScenVar);

	/*
	**	Currently-selected objects.
	*/
	for (i = 0; i < SelectedObjectsType::COUNT; i++) {
		DynamicVectorClass<ObjectClass *>& selection = CurrentObject.Raw(i);
		for (j = 0; j < selection.Count(); j++) {
			unsigned long target_as_object_ptr = reinterpret_cast<unsigned long>(selection[j]);
			TARGET target = (TARGET)target_as_object_ptr;
			selection[j] = As_Object(target);
			Check_Ptr(selection[j],__FILE__,__LINE__);
		}
	}

	/*
	**	Last-Minute Fixups; to resolve these pointers properly requires all other
	**	pointers to be loaded & decoded.
	*/
	if (Map.PendingObjectPtr) {
		Map.PendingObject = &Map.PendingObjectPtr->Class_Of();
		Check_Ptr((void *)Map.PendingObject, __FILE__, __LINE__);
		Map.Set_Cursor_Shape(Map.PendingObject->Occupy_List(true));
	} else {
		Map.PendingObject = 0;
		Map.Set_Cursor_Shape(0);
	}
}


/***********************************************************************************************
 * Read_Object -- reads an object from a file                                                  *
 *                                                                                             *
 *    Replacement for the original code below, which doesn't work with MSVC                    *
 *    We now assume that the vtable is 4 bytes, and is at the beginning of the class           *
 *    It's the caller's responsibility to make sure the VTable is correct                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 BR : Created.                                                                  *
 *   9/10/2019 12:34PM ST : Updated for MS compiler                                            *
 *=============================================================================================*/
bool Read_Object(void *ptr, int class_size, FileClass & file, bool has_vtable)
{
	int size;

	/*
	**	Read size of this chunk.
	*/
	if (file.Read(&size,sizeof(size)) != sizeof(size)) {
		return(false);
	}
	
	/*
	**	Error if incorrect size.
	*/
	if (size != class_size) {
		return false;
	}
	
	int vtable_adjust = has_vtable ? 4 : 0;
	unsigned char *object_ptr = static_cast<unsigned char *>(ptr);

	if (has_vtable) {
		/*
		** Need to skip the vtable read.
		*/
		int dummy;
		file.Read(&dummy, vtable_adjust);
	}

	/*
	**	Read object data.
	*/
	if (file.Read(object_ptr + vtable_adjust, class_size - vtable_adjust) != (class_size - vtable_adjust)) {
		return(false);
	}

	return true;
}	
		
		
#if (0)
/***********************************************************************************************
 * Write_Object -- writes an object to a file                                                  *
 *                                                                                             *
 * This routine writes an object, skipping the embedded virtual function table pointer.        *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      ptr            pointer to object to write                                              *
 *      class_size      size of the class itself                                               *
 *      file            file to use for I/O                                                    *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      true = OK, false = error                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      This routine ASSUMES the program modules are compiled with:                            *
 *      -Vb-      Always make the virtual function table ptr 2 bytes long                      *
 *      -Vt      Put the virtual function table after the 1st class's data                     *
 *                                                                                             *
 *    Also see warnings for Read_Object().                                                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 BR : Created.                                                                  *
 *   9/10/2019 12:34PM ST : Updated for MS compiler                                            *
 *=============================================================================================*/
bool Write_Object(void *ptr, int class_size, FileClass & file)
{
	/*
	** Test assumptions about class size.
	*/
	class TestClass {
		virtual void Test(void) = 0;
	};	

	if (sizeof(TestClass) != 4) {
		/*
		** Crash.
		*/
		*((int*)0) = 0;
	}
	
	/*
	**	Save size of this chunk.
	*/
	if (file.Write(&class_size,sizeof(class_size)) != sizeof(class_size)) {
		return(false);
	}

	/*
	**	Save object data.
	*/
	if (file.Write(ptr, class_size) != (class_size)) {
		return(false);
	}

	return(true);
}


#endif
		
		
#if (0)		//ST - 9/10/2019 12:43PM
		  
/***********************************************************************************************
 * Read_Object -- reads an object from disk                                                    *
 *                                                                                             *
 * This routine reads in an object and fills in the virtual function table pointer.            *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      ptr            pointer to object to read                                               *
 *      base_size      size of object's absolute base class                                    *
 *      class_size      size of the class itself                                               *
 *      file            file to use for I/O                                                    *
 *      vtable         virtual function table pointer value, NULL if none                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      true = OK, false = error                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      This routine ASSUMES the program modules are compiled with:                            *
 *      -Vb-      Always make the virtual function table ptr 2 bytes long                      *
 *      -Vt      Put the virtual function table after the 1st class's data                     *
 *                                                                                             *
 *      ALSO, the class used to compute 'base_size' must come first in a multiple-inheritence  *
 *      hierarchy.  AND, if your class multiply-inherits from other classes, only ONE of those *
 *      classes can contain virtual functions!  If you include virtual functions in the other  *
 *      classes, the compiler will generate multiple virtual function tables, and this load/save *
 *      technique will fail.                                                                   *
 *                                                                                             *
 *      Each class hierarchy is stored in memory as a chain: first the data for the base-est   *
 *      class, then the virtual function table pointer for this hierarchy, then the data for   *
 *      all derived classes.  If any of these derived classes multiply-inherit, the base class *
 *      for the multiple inheritance is stored as a separate chain following this chain.  The  *
 *      new chain will contain its own virtual function table pointer, if the multiply-        *
 *      inherited hierarchy contains any virtual functions.  Thus, the declaration             *
 *         class A                                                                             *
 *         class B: public A                                                                   *
 *         class C: public B, X                                                                *
 *      is stored as:                                                                          *
 *         A data                                                                              *
 *         A's Virtual Table Pointer                                                           *
 *         B data                                                                              *
 *         X data                                                                              *
 *         [X's Virtual Table Pointer]                                                         *
 *         C data                                                                              *
 *                                                                                             *
 *      and                                                                                    *
 *         class A                                                                             *
 *         class B: public A                                                                   *
 *         class C: public X, B                                                                *
 *      is stored in memory as:                                                                *
 *         X data                                                                              *
 *         [X's Virtual Table Pointer]                                                         *
 *         A data                                                                              *
 *         A's Virtual Table Pointer                                                           *
 *         B data                                                                              *
 *         C data                                                                              *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 BR : Created.                                                                  *
 *=============================================================================================*/
bool Read_Object(void *ptr, int base_size, int class_size, FileClass & file, void * vtable)
{
	int size;					// object size in bytes

	/*
	**	Read size of this chunk.
	*/
	if (file.Read(&size,sizeof(size)) != sizeof(size)) {
		return(false);
	}

	/*
	**	Error if incorrect size.
	*/
	if (size != class_size) {
		return(false);
	}

	/*
	**	Read object data.
	*/
	if (file.Read(ptr, class_size) != (class_size)) {
		return(false);
	}

	/*
	**	Fill in VTable.
	*/
	if (vtable) {
		((void **)(((char *)ptr) + base_size - 4))[0] = vtable;
	}

	return(true);
}

#endif



/***********************************************************************************************
 * Write_Object -- reads an object from disk, in a safe way                                    *
 *                                                                                             *
 * This routine writes an object in 2 pieces, skipping the embedded                            *
 * virtual function table pointer.                                                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      ptr            pointer to object to write                                              *
 *      class_size      size of the class itself                                               *
 *      file            file to use for I/O                                                    *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      true = OK, false = error                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      This routine ASSUMES the program modules are compiled with:                            *
 *      -Vb-      Always make the virtual function table ptr 2 bytes long                      *
 *      -Vt      Put the virtual function table after the 1st class's data                     *
 *                                                                                             *
 *    Also see warnings for Read_Object().                                                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 BR : Created.                                                                  *
 *=============================================================================================*/
bool Write_Object(void *ptr, int class_size, FileClass & file)
{
	/*
	**	Save size of this chunk.
	*/
	if (file.Write(&class_size,sizeof(class_size)) != sizeof(class_size)) {
		return(false);
	}

	/*
	**	Save object data.
	*/
	if (file.Write(ptr, class_size) != (class_size)) {
		return(false);
	}

	return(true);
}


/***************************************************************************
 * Get_Savefile_Info -- gets description, scenario #, house                *
 *                                                                         *
 * INPUT:                                                                  *
 *      id         numerical ID, for the file extension                    *
 *      buf      buffer to store description in                            *
 *      scenp      ptr to variable to hold scenario                        *
 *      housep   ptr to variable to hold house                             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = OK, false = error (save-game file invalid)                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/12/1995 BR : Created.                                              *
 *=========================================================================*/
bool Get_Savefile_Info(int id, char *buf, unsigned *scenp, HousesType *housep)
{
	RawFileClass file;
	char name[_MAX_FNAME+_MAX_EXT];
	unsigned long version;
	char descr_buf[DESCRIP_MAX];

	/*
	**	Generate the filename to load
	*/
	sprintf(name, "SAVEGAME.%03d", id);

	/*
	**	If the file opens OK, read the file
	*/
	if (file.Open(name, READ)) {

		/*
		**	Read in the description, scenario #, and the house
		*/
		if (file.Read(descr_buf, DESCRIP_MAX) != DESCRIP_MAX) {
			file.Close();
			return(false);
		}

		descr_buf[strlen(descr_buf) - 2] = '\0';	// trim off CR/LF
		strcpy(buf, descr_buf);

		if (file.Read(scenp, sizeof(unsigned)) != sizeof(unsigned)) {
			file.Close();
			return(false);
		}

		if (file.Read(housep, sizeof(HousesType)) != sizeof(HousesType)) {
			file.Close();
			return(false);
		}

		/*
		**	Read & verify the save-game version #
		*/
		if (file.Read(&version,sizeof(version)) != sizeof(version)) {
			file.Close();
			return(false);
		}

		if (version!=SAVEGAME_VERSION) {
			file.Close();
			return(false);
		}

		file.Close();

		return(true);
	}
	return(false);
}


/***************************************************************************
 * TechnoType_To_Target -- converts TechnoTypeClass to TARGET              *
 *                                                                         *
 * INPUT:                                                                  *
 *      ptr      pointer to convert                                        *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      target value                                                       *
 *                                                                         *
 * WARNINGS:                                                               *
 *      Be certain that you only use the returned target value by passing  *
 *      it to Target_To_TechnoType; do NOT call As_Techno, or you'll get   *
 *      a totally invalid pointer.                                         *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/12/1995 BR : Created.                                              *
 *=========================================================================*/
TARGET TechnoType_To_Target(TechnoTypeClass const * ptr)
{
	TARGET target;

	switch (ptr->What_Am_I()) {
		case RTTI_INFANTRYTYPE:
			target = Build_Target(KIND_INFANTRY, ((InfantryTypeClass const *)ptr)->Type);
			break;

		case RTTI_UNITTYPE:
			target = Build_Target(KIND_UNIT, ((UnitTypeClass const *)ptr)->Type);
			break;

		case RTTI_AIRCRAFTTYPE:
			target = Build_Target(KIND_AIRCRAFT, ((AircraftTypeClass const *)ptr)->Type);
			break;

		case RTTI_BUILDINGTYPE:
			target = Build_Target(KIND_BUILDING, ((BuildingTypeClass const *)ptr)->Type);
			break;

		default:
			target = 0;
			break;
	}

	return(target);
}


/***************************************************************************
 * Target_To_TechnoType -- converts TARGET to TechnoTypeClass              *
 *                                                                         *
 * INPUT:                                                                  *
 *      target      TARGET value to convert                                *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      pointer to the TechnoTypeClass for this target value               *
 *                                                                         *
 * WARNINGS:                                                               *
 *      The TARGET value MUST have been generated with TechnoType_To_Target;*
 *      If you give this routine a target generated by an As_Target()      *
 *      routine, it will return a bogus pointer.                           *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/12/1995 BR : Created.                                              *
 *=========================================================================*/
TechnoTypeClass const * Target_To_TechnoType(TARGET target)
{
	switch (Target_Kind(target)) {
		case KIND_INFANTRY:
			return(&InfantryTypeClass::As_Reference((InfantryType)Target_Value(target)));

		case KIND_UNIT:
			return(&UnitTypeClass::As_Reference((UnitType)Target_Value(target)));

		case KIND_AIRCRAFT:
			return(&AircraftTypeClass::As_Reference((AircraftType)Target_Value(target)));

		case KIND_BUILDING:
			return(&BuildingTypeClass::As_Reference((StructType)Target_Value(target)));
	}
	return(NULL);
}


/***************************************************************************
 * Get_VTable -- gets the VTable pointer for the given object              *
 *                                                                         *
 * INPUT:                                                                  *
 *      ptr      pointer to check                                          *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none                                                               *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/12/1995 BR : Created.                                              *
 *=========================================================================*/
void * Get_VTable(void *ptr, int base_size)
{
	return(((void **)(((char *)ptr) + base_size - 4))[0]);
}


/***************************************************************************
 * Set_VTable -- sets the VTable pointer for the given object              *
 *                                                                         *
 * INPUT:                                                                  *
 *      ptr         pointer to check                                       *
 *      base_size   size of base class                                     *
 *      vtable      value of VTable to plug in                             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none                                                               *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/12/1995 BR : Created.                                              *
 *=========================================================================*/
void Set_VTable(void *ptr, int base_size, void *vtable)
{
	((void **)(((char *)ptr) + base_size - 4))[0] = vtable;
}


#if 0
/****************************************************************************
Dump routine: prints everything about everything related to the Save/Load
process (OK, not exactly everything, but lots of stuff)
****************************************************************************/
void Dump(void)
{
	int i,j;
	FILE *fp;
	char *layername[] = {
		"Ground",
		"Air",
		"Top"
	};

	/*
	------------------------------- Open file --------------------------------
	*/
	fp = fopen("dump.txt","wt");

	/*
	------------------------------ Logic Layer -------------------------------
	*/
	fprintf(fp,"--------------------- Logic Layer ---------------------\n");
	fprintf(fp,"Count: %d\n",Logic.Count());
	for (j = 0; j < Logic.Count(); j++) {
		fprintf(fp, "Entry %d: %x \n",j,Logic[j]);
	}
	fprintf(fp,"\n");

	/*
	------------------------------- Map Layers -------------------------------
	*/
	for (i = 0; i < LAYER_COUNT; i++) {
		fprintf(fp,"----------------- Map Layer %s ---------------------\n",
			layername[i]);
		fprintf(fp,"Count: %d\n",Map.Layer[i].Count());
		for (j = 0; j < Map.Layer[i].Count(); j++) {
			fprintf(fp, "Entry %d: %x \n",j,Map.Layer[i][j]);
		}
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ TeamTypes --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",TeamTypes.ActiveCount);
	for (i = 0; i < TEAMTYPE_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d Name:%s\n",i,TeamTypes[i].IsActive,
			TeamTypes[i].Get_Name());
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Teams --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Teams.ActiveCount);
	for (i = 0; i < TEAM_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d Name:%s\n",i,Teams[i].IsActive,
			Teams[i].Class->Get_Name());
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Triggers --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Triggers.ActiveCount);
	for (i = 0; i < TRIGGER_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d Name:%s\n",i,Triggers[i].IsActive,
			Triggers[i].Get_Name());
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Aircraft --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Aircraft.ActiveCount);
	for (i = 0; i < AIRCRAFT_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Aircraft[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Anims --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Anims.ActiveCount);
	for (i = 0; i < ANIM_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Anims[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Buildings --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Buildings.ActiveCount);
	for (i = 0; i < BUILDING_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Buildings[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Bullets --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Bullets.ActiveCount);
	for (i = 0; i < BULLET_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Bullets[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Infantry --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Infantry.ActiveCount);
	for (i = 0; i < INFANTRY_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Infantry[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Overlays --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Overlays.ActiveCount);
	for (i = 0; i < OVERLAY_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Overlays[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Reinforcements --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Reinforcements.ActiveCount);
	for (i = 0; i < REINFORCEMENT_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Reinforcements[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Smudges --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Smudges.ActiveCount);
	for (i = 0; i < SMUDGE_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Smudges[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Templates --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Templates.ActiveCount);
	for (i = 0; i < TEMPLATE_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Templates[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Terrains --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Terrains.ActiveCount);
	for (i = 0; i < TERRAIN_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Terrains[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Units --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Units.ActiveCount);
	for (i = 0; i < UNIT_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Units[i].IsActive);
	}
	fprintf(fp,"\n");

	fprintf(fp,"------------------ Factories --------------------------\n");
	fprintf(fp,"ActiveCount: %d\n",Factories.ActiveCount);
	for (i = 0; i < FACTORY_MAX; i++) {
		fprintf(fp,"Entry %d: Active:%d \n",i,Factories[i].IsActive);
	}
	fprintf(fp,"\n");

	fclose(fp);

	/*
	---------------------------- Flush the cache -----------------------------
	*/
	fp = fopen("dummy.bin","wt");
	for (i = 0; i < 100; i++) {
		fprintf(fp,"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	}
	fclose(fp);
}
#endif