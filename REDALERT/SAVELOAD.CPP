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

/* $Header: /counterstrike/SAVELOAD.CPP 9     3/17/97 1:04a Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *                  Last Update : July 8, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Code_All_Pointers -- Code all pointers.                                                   *
 *   Decode_All_Pointers -- Decodes all pointers.                                              *
 *   Get_Savefile_Info -- gets description, scenario #, house                                  *
 *   Load_Game -- loads a saved game                                                           *
 *   Load_MPlayer_Values -- Loads multiplayer-specific values                                  *
 *   Load_Misc_Values -- loads miscellaneous variables                                         *
 *   MPlayer_Save_Message -- pops up a "saving..." message                                     *
 *   Put_All -- Store all save game data to the pipe.                                          *
 *   Reconcile_Players -- Reconciles loaded data with the 'Players' vector							  *
 *   Save_Game -- saves a game to disk                                                         *
 *   Save_MPlayer_Values -- Saves multiplayer-specific values                                  *
 *   Save_Misc_Values -- saves miscellaneous variables                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"vortex.h"
#ifdef WIN32
#include "tcpip.h"
#include "ccdde.h"

//#include "WolDebug.h"

extern bool DLLSave(Pipe &file);
extern bool DLLLoad(Straw &file);

extern bool SpawnedFromWChat;
#endif

//#define	SAVE_BLOCK_SIZE	512
#define	SAVE_BLOCK_SIZE	4096
//#define	SAVE_BLOCK_SIZE	1024

/*
********************************** Defines **********************************
*/
#define	SAVEGAME_VERSION		(DESCRIP_MAX + \
										0x01000006 + ( \
										sizeof(AircraftClass) + \
										sizeof(AircraftTypeClass) + \
										sizeof(AnimClass) + \
										sizeof(AnimTypeClass) + \
										sizeof(BaseClass) + \
										sizeof(BuildingClass) + \
										sizeof(BuildingTypeClass) + \
										sizeof(BulletClass) + \
										sizeof(BulletTypeClass) + \
										sizeof(CellClass) + \
										sizeof(FactoryClass) + \
										sizeof(HouseClass) + \
										sizeof(HouseTypeClass) + \
										sizeof(InfantryClass) + \
										sizeof(InfantryTypeClass) + \
										sizeof(LayerClass) + \
										sizeof(MouseClass) + \
										sizeof(OverlayClass) + \
										sizeof(OverlayTypeClass) + \
										sizeof(SmudgeClass) + \
										sizeof(SmudgeTypeClass) + \
										sizeof(TeamClass) + \
										sizeof(TeamTypeClass) + \
										sizeof(TemplateClass) + \
										sizeof(TemplateTypeClass) + \
										sizeof(TerrainClass) + \
										sizeof(TerrainTypeClass) + \
										sizeof(TriggerClass) + \
										sizeof(TriggerTypeClass) + \
										sizeof(UnitClass) + \
										sizeof(UnitTypeClass) + \
										sizeof(VesselClass) + \
										sizeof(ScenarioClass) + \
										sizeof(ChronalVortexClass)))
//										sizeof(Waypoint)))


static int Reconcile_Players(void);
extern bool Is_Mission_Counterstrike (char *file_name);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
extern bool Is_Mission_Aftermath (char *file_name);
#endif

/***********************************************************************************************
 * Put_All -- Store all save game data to the pipe.                                            *
 *                                                                                             *
 *    This is the bulk processor of the game related save game data. All the game object       *
 *    and state data is stored to the pipe specified.                                          *
 *                                                                                             *
 * INPUT:   pipe  -- Reference to the pipe that will receive the save game data.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static void Put_All(Pipe & pipe, int save_net)
{
	/*
	**	Save the scenario global information.
	*/
	pipe.Put(&Scen, sizeof(Scen));

	/*
	**	Save the map.  The map must be saved first, since it saves the Theater.
	*/
	if (!save_net) Call_Back();
	Map.Save(pipe);

	if (!save_net) Call_Back();

	/*
	**	Save all game objects.  This code saves every object that's stored in a
	**	TFixedIHeap class.
	*/
	Houses.Save(pipe);
	if (!save_net) Call_Back();
	TeamTypes.Save(pipe);
	if (!save_net) Call_Back();
	Teams.Save(pipe);
	if (!save_net) Call_Back();
	TriggerTypes.Save(pipe);
	if (!save_net) Call_Back();
	Triggers.Save(pipe);
	if (!save_net) Call_Back();
	Aircraft.Save(pipe);
	if (!save_net) Call_Back();
	Anims.Save(pipe);

	if (!save_net) Call_Back();

	Buildings.Save(pipe);
	if (!save_net) Call_Back();
	Bullets.Save(pipe);
	if (!save_net) Call_Back();
	Infantry.Save(pipe);
	if (!save_net) Call_Back();
	Overlays.Save(pipe);
	if (!save_net) Call_Back();
	Smudges.Save(pipe);
	if (!save_net) Call_Back();
	Templates.Save(pipe);
	if (!save_net) Call_Back();
	Terrains.Save(pipe);
	if (!save_net) Call_Back();
	Units.Save(pipe);
	if (!save_net) Call_Back();
	Factories.Save(pipe);
	if (!save_net) Call_Back();
	Vessels.Save(pipe);

	if (!save_net) Call_Back();

	/*
	**	Save the Logic & Map layers
	*/
	Logic.Save(pipe);

	int count = MapTriggers.Count();
	pipe.Put(&count, sizeof(count));
	int index;
	for (int index = 0; index < MapTriggers.Count(); index++) {
		TARGET target = MapTriggers[index]->As_Target();
		pipe.Put(&target, sizeof(target));
	}
	if (!save_net) Call_Back();
	count = LogicTriggers.Count();
	pipe.Put(&count, sizeof(count));
	for (index = 0; index < LogicTriggers.Count(); index++) {
		TARGET target = LogicTriggers[index]->As_Target();
		pipe.Put(&target, sizeof(target));
	}
	if (!save_net) Call_Back();
	for (HousesType h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
		count = HouseTriggers[h].Count();
		pipe.Put(&count, sizeof(count));
		for (index = 0; index < HouseTriggers[h].Count(); index++) {
			TARGET target = HouseTriggers[h][index]->As_Target();
			pipe.Put(&target, sizeof(target));
		}
	}
	if (!save_net) Call_Back();

	for (int i = 0; i < LAYER_COUNT; i++) {
		Map.Layer[i].Save(pipe);
	}

	if (!save_net) Call_Back();

	/*
	**	Save the Score
	*/
	pipe.Put(&Score, sizeof(Score));
	if (!save_net) Call_Back();

	/*
	**	Save the AI Base
	*/
	Base.Save(pipe);
	if (!save_net) Call_Back();

	/*
	**	Save out the carry over list (if present). First see how
	**	many carry over objects are in the list.
	*/
	int carry_count = 0;
	CarryoverClass const * cptr = Carryover;
	while (cptr != NULL) {
		carry_count++;
		cptr = (CarryoverClass const *)cptr->Get_Next();
	}

	if (!save_net) Call_Back();

	/*
	**	Save out the number of objects in the list.
	*/
	pipe.Put(&carry_count, sizeof(carry_count));
	if (!save_net) Call_Back();

	/*
	**	Now write out the objects themselves.
	*/
	CarryoverClass const * object_to_write = Carryover;
	while (object_to_write != NULL) {
		pipe.Put(object_to_write, sizeof(*object_to_write));
		object_to_write = (CarryoverClass const *)object_to_write->Get_Next();
	}
	if (!save_net) Call_Back();

	/*
	**	Save miscellaneous variables.
	*/
	Save_Misc_Values(pipe);

	if (!save_net) Call_Back();

	/*
	**	Save multiplayer values
	*/
	pipe.Put(&save_net, sizeof(save_net));		// Write out whether we saved the net values so we know if we have to load them again. ST - 10/22/2019 2:10PM
	if (save_net) {
		Save_MPlayer_Values(pipe);
	}

	pipe.Flush();
}


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
 *   02/27/1996 JLB : Uses simpler game control value save operation.      *
 *=========================================================================*/
bool Save_Game(int id, char const * descr, bool )
{
	char name[_MAX_FNAME+_MAX_EXT];

	/*
	**	Generate the filename to save.  If 'id' is -1, it means save a
	** network/modem game; otherwise, use 'id' as the file extension.
	*/
	if (id==-1) {
		strcpy(name, NET_SAVE_FILE_NAME);
		//save_net = 1;
	} else {
		sprintf(name, "SAVEGAME.%03d", id);
	}

	return Save_Game(name, descr);
}



/*
** Version that takes file name. ST - 9/9/2019 11:10AM
*/
bool NowSavingGame = false; // TEMP MBL: Need to discuss better solution with Steve
bool Save_Game(const char *file_name, const char *descr)
{
	NowSavingGame = true; // TEMP MBL: Need to discuss better solution with Steve

	int save_net = 0;									// 1 = save network/modem game
	
	if (Session.Type  == GAME_GLYPHX_MULTIPLAYER) {
		save_net = 1;
	}
	
	unsigned scenario;
	HousesType house;

	scenario = Scen.Scenario;						// get current scenario #
	house = PlayerPtr->Class->House;				// get current house

	/*
	**	Code everybody's pointers
	*/
	Code_All_Pointers();

	/*
	**	Open the file
	*/
	BufferIOFileClass file(file_name);

	FilePipe fpipe(&file);

	/*
	** Save the DLLs variables first, so we can do a version check in the DLL when we begin the load
	*/
	if (RunningAsDLL) {
		DLLSave(fpipe);
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
	char descr_buf[DESCRIP_MAX];
	memset(descr_buf, '\0', sizeof(descr_buf));
	sprintf(descr_buf, "%s\r\n", descr);			// put CR-LF after text
	//descr_buf[strlen(descr_buf) + 1] = 26;		// put CTRL-Z after NULL
	fpipe.Put(descr_buf, DESCRIP_MAX);

	fpipe.Put(&scenario, sizeof(scenario));

	fpipe.Put(&house, sizeof(house));

	/*
	**	Save the save-game version, for loading verification
	*/
	unsigned long version = SAVEGAME_VERSION;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	version++;
#endif
	fpipe.Put(&version, sizeof(version));

	int pos = file.Seek(0, SEEK_CUR);

	/*
	**	Store a dummy message digest.
	*/
	char digest[20];
	fpipe.Put(digest, sizeof(digest));


	/*
	**	Dump the save game data to the file. The data is compressed
	**	and then encrypted. The message digest is calculated in the
	**	process by using the data just as it is written to disk.
	*/
	SHAPipe sha;
	BlowPipe bpipe(BlowPipe::ENCRYPT);
	LZOPipe pipe(LZOPipe::COMPRESS, SAVE_BLOCK_SIZE);
//	LZWPipe pipe(LZWPipe::COMPRESS, SAVE_BLOCK_SIZE);
//	LCWPipe pipe(LCWPipe::COMPRESS, SAVE_BLOCK_SIZE);
	bpipe.Key(&FastKey, BlowfishEngine::MAX_KEY_LENGTH);

	sha.Put_To(fpipe);
	bpipe.Put_To(sha);
	pipe.Put_To(bpipe);
	Put_All(pipe, save_net);

	/*
	**	Output the real final message digest. This is the one that is of
	**	the data image as it exists on the disk.
	*/
	pipe.Flush();
	file.Seek(pos, SEEK_SET);
	sha.Result(digest);
	fpipe.Put(digest, sizeof(digest));

	pipe.End();

	Decode_All_Pointers();

	NowSavingGame = false; // TEMP MBL: Need to discuss better solution with Steve

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
 *   12/28/1994 BR : Created. 						   								*
 *   1/20/97  V.Grippi Added expansion CD check                            *
 *=========================================================================*/
bool Load_Game(int id)
{
	char name[_MAX_FNAME+_MAX_EXT];

	/*
	**	Generate the filename to load.  If 'id' is -1, it means save a
	** network/modem game; otherwise, use 'id' as the file extension.
	*/
	if (id == -1) {
		strcpy(name, NET_SAVE_FILE_NAME);
		//load_net = 1;
	} else {
		sprintf(name, "SAVEGAME.%03d", id);
	}
	return Load_Game(name);
}


/*
** Version that takes a file name instead. ST - 9/9/2019 11:13AM
*/
bool Load_Game(const char *file_name)
{		
	int i;
	unsigned scenario;
	HousesType house;
	char descr_buf[DESCRIP_MAX];
	int load_net = 0;									// 1 = save network/modem game
	
	/*
	**	Open the file
	*/
	RawFileClass file(file_name);
	if (!file.Is_Available()) {
		return(false);
	}

	FileStraw fstraw(file);

	Call_Back();

	/*
	** Load the DLLs variables first, in case we need to do something different based on version
	*/
	if (RunningAsDLL) {
		DLLLoad(fstraw);
	}

	/*
	**	Read & discard the save-game's header info
	*/
	if (fstraw.Get(descr_buf, DESCRIP_MAX) != DESCRIP_MAX) {
		return(false);
	}

	if (fstraw.Get(&scenario, sizeof(scenario)) != sizeof(scenario)) {
		return(false);
	}

	if (fstraw.Get(&house, sizeof(house)) != sizeof(house)) {
		return(false);
	}

	/*
	**	Read in & verify the save-game ID code
	*/
	unsigned long version;
	if (fstraw.Get(&version, sizeof(version)) != sizeof(version)) {
		return(false);
	}
	GameVersion = version;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (version != SAVEGAME_VERSION && ((version-1) != SAVEGAME_VERSION) ) {
	      	return(false);
	}
#else
	if (version != SAVEGAME_VERSION /*&& version != 0x0100616D*/){
	      	return(false);
	}
#endif
	/*
	**	Get the message digest that is embedded in the file.
	*/
	char digest[20];
	fstraw.Get(digest, sizeof(digest));

	/*
	**	Remember the file position since we must seek back here to
	**	perform the real saved game read.
	*/
	long pos = file.Seek(0, SEEK_CUR);

	/*
	**	Pass the rest of the file through the hash straw so that
	**	the digest can be compaired to the one in the file.
	*/
	SHAStraw sha;
	sha.Get_From(fstraw);
	for (;;) {
		if (sha.Get(_staging_buffer, sizeof(_staging_buffer)) != sizeof(_staging_buffer)) break;
	}
	char actual[20];
	sha.Result(actual);
	sha.Get_From(NULL);

	Call_Back();

	/*
	**	Compare the two digests. If they differ then return a failure condition
	**	before any damage could be done.
	*/
	if (memcmp(actual, digest, sizeof(digest)) != 0) {
		return(false);
	}

	/*
	**	Set up the pipe so that the scenario data can be read.
	*/
	file.Seek(pos, SEEK_SET);
	BlowStraw bstraw(BlowStraw::DECRYPT);
	LZOStraw straw(LZOStraw::DECOMPRESS, SAVE_BLOCK_SIZE);
//	LZWStraw straw(LZWStraw::DECOMPRESS, SAVE_BLOCK_SIZE);
//	LCWStraw straw(LCWStraw::DECOMPRESS, SAVE_BLOCK_SIZE);

	bstraw.Key(&FastKey, BlowfishEngine::MAX_KEY_LENGTH);
	bstraw.Get_From(fstraw);
	straw.Get_From(bstraw);

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
	**	Load the scenario global information.
	*/
	straw.Get(&Scen, sizeof(Scen));

	/*
	**	Fixup the Sessionclass scenario info so we can work out which
	** CD to request later
	*/
	if ( load_net ){

		CCFileClass scenario_file (Scen.ScenarioName);
		if ( !scenario_file.Is_Available() ){

			int cd = -1;
			if (Is_Mission_Counterstrike (Scen.ScenarioName)) {
				cd = 2;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if (Expansion_AM_Present()) {
					int current_drive = CCFileClass::Get_CD_Drive();
					int index = Get_CD_Index(current_drive, 1*60);
					if (index == 3) cd = 3;
				}
#endif
			}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if (Is_Mission_Aftermath (Scen.ScenarioName)) {
				cd = 3;
#ifdef BOGUSCD
	cd = -1;
#endif
			}
#endif
			RequiredCD = cd;
			if (!Force_CD_Available (RequiredCD)) {
				Emergency_Exit(EXIT_FAILURE);
			}

			/*
			** Update the internal list of scenarios to include the counterstrike
			** list.
			*/
			Session.Read_Scenario_Descriptions();
		} else {
			/*
			** The scenario is available so set RequiredCD to whatever is currently
			** in the drive.
			*/
			int current_drive = CCFileClass::Get_CD_Drive();
			RequiredCD = Get_CD_Index(current_drive, 1*60);
		}
	}

	/*
	**	Load the map.  The map comes first, since it loads the Theater & init's
	**	mixfiles.  The map calls all the type-class's Init routines, telling them
	**	what the Theater is; this must be done before any objects are created, so
	**	they'll be properly created.
	*/
	Map.Load(straw);

	Call_Back();

	/*
	**	Load the object data.
	*/
	Houses.Load(straw);
	TeamTypes.Load(straw);
	Teams.Load(straw);
	TriggerTypes.Load(straw);
	Triggers.Load(straw);
	Aircraft.Load(straw);
	Anims.Load(straw);
	Buildings.Load(straw);
	Bullets.Load(straw);

	Call_Back();

	Infantry.Load(straw);
	Overlays.Load(straw);
	Smudges.Load(straw);
	Templates.Load(straw);
	Terrains.Load(straw);
	Units.Load(straw);
	Factories.Load(straw);
	Vessels.Load(straw);

	/*
	**	Load the Logic & Map Layers
	*/
	Logic.Load(straw);

	int count;
	straw.Get(&count, sizeof(count));
	MapTriggers.Clear();
	int index;
	for (index = 0; index < count; index++) {
		TARGET target;
		straw.Get(&target, sizeof(target));
		MapTriggers.Add(As_Trigger(target));
	}

	straw.Get(&count, sizeof(count));
	LogicTriggers.Clear();
	for (index = 0; index < count; index++) {
		TARGET target;
		straw.Get(&target, sizeof(target));
		LogicTriggers.Add(As_Trigger(target));
	}

	for (HousesType h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
		straw.Get(&count, sizeof(count));
		HouseTriggers[h].Clear();
		for (index = 0; index < count; index++) {
			TARGET target;
			straw.Get(&target, sizeof(target));
			HouseTriggers[h].Add(As_Trigger(target));
		}
	}

	for (i = 0; i < LAYER_COUNT; i++) {
		Map.Layer[i].Load(straw);
	}

	Call_Back();

	/*
	**	Load the Score
	*/
	straw.Get(&Score, sizeof(Score));
	new(&Score) ScoreClass(NoInitClass());

	/*
	**	Load the AI Base
	*/
	Base.Load(straw);

	/*
	**	Delete any carryover pseudo-saved game list.
	*/
	while (Carryover != NULL) {
		CarryoverClass * cptr = (CarryoverClass *)Carryover->Get_Next();
		Carryover->Remove();
		delete Carryover;
		Carryover = cptr;
	}

	/*
	**	Load any carryover pseudo-saved game list.
	*/
	int carry_count = 0;
	straw.Get(&carry_count, sizeof(carry_count));
	while (carry_count) {
		CarryoverClass * cptr = new CarryoverClass;
		assert(cptr != NULL);

		straw.Get(cptr, sizeof(CarryoverClass));
		new (cptr) CarryoverClass(NoInitClass());
		cptr->Zap();

		if (!Carryover) {
			Carryover = cptr;
		} else {
			cptr->Add_Tail(*Carryover);
		}
		carry_count--;
	}

	Call_Back();

	/*
	**	Load miscellaneous variables, including the map size & the Theater
	*/
	Load_Misc_Values(straw);

	/*
	**	Load multiplayer values
	*/
	straw.Get(&load_net, sizeof(load_net));
	if (load_net) {
		Load_MPlayer_Values(straw);
	}

	file.Close();
	Decode_All_Pointers();
	Map.Init_IO();
	Map.Flag_To_Redraw(true);

	/*
	**	Fixup any expediency data that can be inferred from the physical
	**	data loaded.
	*/
	Post_Load_Game(load_net);

	/*
	** Re-init unit trackers. They will be garbage pointers after the load
	*/
	for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		HouseClass * hptr = HouseClass::As_Pointer(house);
		if (hptr && hptr->IsActive) {
			hptr->Init_Unit_Trackers();
		}
	}

	Call_Back();

	/*
	**	Set the required CD to be in the drive according to the scenario
	**	loaded.
	*/
	if (RequiredCD != -2 && !load_net) {

#ifdef FIXIT_ANTS
		/*
		**	Determines if this an ant mission. Since the ant mission looks no different from
		**	a regular mission, examining of the scenario name is the only way to tell.
		*/
		if (toupper(Scen.ScenarioName[0]) == 'S' &&
			toupper(Scen.ScenarioName[1]) == 'C' &&
			toupper(Scen.ScenarioName[2]) == 'A' &&
			toupper(Scen.ScenarioName[3]) == '0' &&
			toupper(Scen.ScenarioName[5]) == 'E' &&
			toupper(Scen.ScenarioName[6]) == 'A') {

			AntsEnabled = true;
		} else{
			AntsEnabled = false;
		}
#endif

		if (Scen.Scenario == 1) {
			RequiredCD = -1;
		} else {
#ifdef FIXIT_ANTS
			if (Scen.Scenario > 19 || AntsEnabled) {
			   RequiredCD = 2;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if(Scen.Scenario >= 36) {
					RequiredCD = 3;
#ifdef BOGUSCD
	RequiredCD = -1;
#endif
				}
#endif
			} else {
#endif	//FIXIT_ANTS
				if (PlayerPtr->Class->House != HOUSE_USSR  && PlayerPtr->Class->House != HOUSE_UKRAINE) {
					RequiredCD = 0;
				} else {
					RequiredCD = 1;
				}
#ifdef FIXIT_ANTS
			}
#endif	//FIXIT_ANTS
		}

	}else{

		if ( load_net ){

			CCFileClass scenario_file (Scen.ScenarioName);

			/*
			** Fix up the session class variables
			*/
			for ( int s=0 ; s<Session.Scenarios.Count() ; s++ ) {
				if (Session.Scenarios[s]->Description() == Scen.Description){

					memcpy (Session.Options.ScenarioDescription, Scen.Description,
								sizeof (Session.Options.ScenarioDescription));
					memcpy (Session.ScenarioFileName, Scen.ScenarioName,
								sizeof (Session.ScenarioFileName));
					Session.ScenarioFileLength = scenario_file.Size();
					memcpy (Session.ScenarioDigest, Session.Scenarios[s]->Get_Digest(),
								sizeof (Session.ScenarioDigest));
					Session.ScenarioIsOfficial = Session.Scenarios[s]->Get_Official();
					Scen.Scenario = s;
					Session.Options.ScenarioIndex = s;
					break;
				}
			}
		}
	}

	if (!Force_CD_Available(RequiredCD)) {
		Prog_End("Load_Game Force_CD_Available failed", true);
		Emergency_Exit(EXIT_FAILURE);
	}

	ScenarioInit = 0;

	if (load_net) {

		// Removed as this is ensured by the GlyphX & DLL save/load code. ST - 10/22/2019 5:20PM
		//if (!Reconcile_Players()) {	// (must do after Decode pointers)
		//	return(false);
		//}
		
		//!!!!!!!!!! put Fixup_Player_Units() here
		Session.LoadGame = true;
	}

	Map.Reload_Sidebar();	// re-load sidebar art.

	/*
	**	Rescan the scenario file for any rules updates.
	*/
	CCINIClass ini;
	int result = ini.Load(CCFileClass(Scen.ScenarioName), true);

	/*
	**	Reset the rules values to their initial settings.
	*/
	Rule.General(RuleINI);
	Rule.Recharge(RuleINI);
	Rule.AI(RuleINI);
	Rule.Powerups(RuleINI);
	Rule.Land_Types(RuleINI);
	Rule.Themes(RuleINI);
	Rule.IQ(RuleINI);
	Rule.Objects(RuleINI);
	Rule.Difficulty(RuleINI);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98 - But does this incorporate *changes*? - NO.
	Rule.General(AftermathINI);
	Rule.Recharge(AftermathINI);
	Rule.AI(AftermathINI);
	Rule.Powerups(AftermathINI);
	Rule.Land_Types(AftermathINI);
	Rule.Themes(AftermathINI);
	Rule.IQ(AftermathINI);
	Rule.Objects(AftermathINI);
	Rule.Difficulty(AftermathINI);
#endif

	/*
	**	Override any rules values specified in this
	**	particular scenario file.
	*/
	Rule.General(ini);
	Rule.Recharge(ini);
	Rule.AI(ini);
	Rule.Powerups(ini);
	Rule.Land_Types(ini);                               \
	Rule.Themes(ini);
	Rule.IQ(ini);
	Rule.Objects(ini);
	Rule.Difficulty(ini);
#ifdef FIXIT_CSII	//	ajw - Added runtime check for Aftermath to skirmish mode.
	if (load_net) {
		bool readini = false;
		switch(Session.Type) {
			case GAME_NORMAL:
				readini = false;
				break;
			case GAME_SKIRMISH:
				readini = Is_Aftermath_Installed();
				break;
			default:
#ifdef FIXIT_VERSION_3
				readini = bAftermathMultiplayer;
#else
				if (PlayingAgainstVersion >= VERSION_AFTERMATH_CS) {
					readini = true;
				}
#endif
				break;
		}
		if(readini) {
			/*
			** Find out if the CD in the current drive is the Aftermath disc.
		  	*/
			if(Get_CD_Index(CCFileClass::Get_CD_Drive(), 1*60) != 3) {
				GamePalette.Set(FADE_PALETTE_FAST, Call_Back);
				if (!Force_CD_Available(3)) {	// force Aftermath CD in drive.
					Prog_End("Load_Game Force_CD_Available(3) failed", true);
#ifndef FIXIT_VERSION_3			//	WChat eliminated.
#ifdef WIN32
if(Special.IsFromWChat || SpawnedFromWChat) {
	char packet[10] = {"Hello"};
	Send_Data_To_DDE_Server (packet, strlen(packet), DDEServerClass::DDE_CONNECTION_FAILED);
}
#endif
#endif
					Emergency_Exit(EXIT_FAILURE);
		     	}
			}
			CCINIClass mpini;
			if (mpini.Load(CCFileClass("MPLAYER.INI"), false)) {
				Rule.General(mpini);
				Rule.Recharge(mpini);
				Rule.AI(mpini);
				Rule.Powerups(mpini);
				Rule.Land_Types(mpini);
				Rule.Themes(mpini);
				Rule.IQ(mpini);
				Rule.Objects(mpini);
				Rule.Difficulty(mpini);
			}
		}

	}
#endif

	if (Scen.TransitTheme == THEME_NONE) {
		Theme.Queue_Song(THEME_FIRST);
	} else {
		Theme.Queue_Song(Scen.TransitTheme);
	}
	
	if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
		Rule.IsSmartDefense = true;
	}
	
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
 *   03/12/1996 JLB : Simplified.                                          *
 *=========================================================================*/
bool Save_Misc_Values(Pipe & file)
{
	int i, j;
	int count;								// # ptrs in 'CurrentObject'
	ObjectClass * ptr;					// for saving 'CurrentObject' ptrs

	/*
	**	Player's House.
	*/
	int x = PlayerPtr->Class->House;
	file.Put(&x, sizeof(x));

	/*
	**	Save frame #.
	*/
	file.Put(&Frame, sizeof(Frame));

	/*
	**	Save currently-selected objects list.
	**	Save the # of ptrs in the list.
	*/
	for (i = 0; i < SelectedObjectsType::COUNT; i++) {
		DynamicVectorClass<ObjectClass *>& selection = CurrentObject.Raw(i);
		count = selection.Count();
		file.Put(&count, sizeof(count));

		/*
		**	Save the pointers.
		*/
		for (j = 0; j < count; j++) {
			ptr = selection[j];
			file.Put(&ptr, sizeof(ptr));
		}
	}

	/*
	** Save the chronal vortex
	*/
	ChronalVortex.Save(file);

	/*
	**	Save Tanya flags.
	*/
	file.Put(&IsTanyaDead, sizeof(IsTanyaDead));
	file.Put(&SaveTanya, sizeof(SaveTanya));

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
 *   03/12/1996 JLB : Simplified.                                                              *
 *=============================================================================================*/
bool Load_Misc_Values(Straw & file)
{
	ObjectClass * ptr;					// for loading 'CurrentObject' ptrs

	/*
	**	Player's House.
	*/
	int x;
	file.Get(&x, sizeof(x));
//	file.Get(&PlayerPtr, sizeof(PlayerPtr));
	PlayerPtr = HouseClass::As_Pointer((HousesType)x);

	/*
	**	Load frame #.
	*/
	file.Get(&Frame, sizeof(Frame));

	for (int i = 0; i < SelectedObjectsType::COUNT; i++) {
		/*
		**	Load currently-selected objects list.
		**	Load the # of ptrs in the list.
		*/
		DynamicVectorClass<ObjectClass *>& selection = CurrentObject.Raw(i);
		int count;								// # ptrs in 'CurrentObject'
		file.Get(&count, sizeof(count));

		/*
		**	Load the pointers.
		*/
		for (int j = 0; j < count; j++) {
			file.Get(&ptr, sizeof(ptr));
			selection.Add(ptr);	// add to the list
		}
	}

	/*
	** Load the chronal vortex
	*/
	ChronalVortex.Load(file);

	/*
	**	Save Tanya flags.
	*/
	file.Get(&IsTanyaDead, sizeof(IsTanyaDead));
	file.Get(&SaveTanya, sizeof(SaveTanya));

	return(true);
}


/***************************************************************************
 * Save_MPlayer_Values -- Saves multiplayer-specific values                *
 *                                                                         *
 * This routine saves multiplayer values that need to be restored for a		*
 * save game.  In addition to saving the random # seed for this scenario, 	*
 * it saves the contents of the actual random number generator; this 		*
 * ensures that the random # sequencer will pick up where it left off when	*
 * the game was saved.																		*
 * This routine also saves the header for a Recording file, so it must 		*
 * save some data not needed specifically by a save-game file (ie Seed).	*
 *                                                                         *
 * INPUT:                                                                  *
 *		file		file to save to															*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		true = success, false = failure													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   09/28/1995 BRR : Created.                                             *
 *=========================================================================*/
bool Save_MPlayer_Values(Pipe & file)
{
	Session.Save(file);
	file.Put(&BuildLevel, sizeof(BuildLevel));
	file.Put(&Debug_Unshroud, sizeof(Debug_Unshroud));
	file.Put(&Seed, sizeof(Seed));
	file.Put(&Whom, sizeof(Whom));
	file.Put(&Special, sizeof(SpecialClass));
	file.Put(&Options, sizeof(GameOptionsClass));

	return (true);
}


/***************************************************************************
 * Load_MPlayer_Values -- Loads multiplayer-specific values                *
 *                                                                         *
 * INPUT:                                                                  *
 *		file			file to load from														*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		true = success, false = failure													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   09/28/1995 BRR : Created.                                             *
 *=========================================================================*/
bool Load_MPlayer_Values(Straw & file)
{
	Session.Load(file);
	file.Get(&BuildLevel, sizeof(BuildLevel));
	file.Get(&Debug_Unshroud, sizeof(Debug_Unshroud));
	file.Get(&Seed, sizeof(Seed));
	file.Get(&Whom, sizeof(Whom));
	file.Get(&Special, sizeof(SpecialClass));
	file.Get(&Options, sizeof(GameOptionsClass));

	return (true);
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
	Vessels.Code_Pointers();

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
//	PlayerPtr = (HouseClass *)(PlayerPtr->Class->House);

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
	Vessels.Decode_Pointers();

	/*
	**	The Layers.
	*/
	Logic.Decode_Pointers();
	for (int i = 0; i < LAYER_COUNT; i++) {
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
//	PlayerPtr = HouseClass::As_Pointer((HousesType)PlayerPtr);
	Whom = PlayerPtr->Class->House;
	assert(PlayerPtr != NULL);

	/*
	**	Currently-selected objects.
	*/
	for (int index = 0; index < SelectedObjectsType::COUNT; index++) {
		DynamicVectorClass<ObjectClass *>& selection = CurrentObject.Raw(index);
		for (int j = 0; j < selection.Count(); j++) {
			selection[j] = As_Object((TARGET)selection[j]);
			assert(selection[j] != NULL);
		}
	}

	/*
	**	Last-Minute Fixups; to resolve these pointers properly requires all other
	**	pointers to be loaded & decoded.
	*/
	if (Map.PendingObjectPtr) {
		Map.PendingObject = &Map.PendingObjectPtr->Class_Of();
		assert(Map.PendingObject != NULL);
		Map.Set_Cursor_Shape(Map.PendingObject->Occupy_List(true));
#ifdef BG
		Map.Set_Placement_List(Map.PendingObject->Placement_List(true));
#endif
	} else {
		Map.PendingObject = 0;
		Map.Set_Cursor_Shape(0);
	}
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
bool Get_Savefile_Info(int id, char * buf, unsigned * scenp, HousesType * housep)
{
	char name[_MAX_FNAME+_MAX_EXT];
	unsigned long version;
	char descr_buf[DESCRIP_MAX];

	/*
	**	Generate the filename to load
	*/
	sprintf(name, "SAVEGAME.%03d", id);
	BufferIOFileClass file(name);

	FileStraw straw(file);

	/*
	**	Read in the description, scenario #, and the house
	*/
	if (straw.Get(descr_buf, DESCRIP_MAX) != DESCRIP_MAX) {
		return(false);
	}

	descr_buf[strlen(descr_buf) - 2] = '\0';	// trim off CR/LF
	strcpy(buf, descr_buf);

	if (straw.Get(scenp, sizeof(unsigned)) != sizeof(unsigned)) {
		return(false);
	}

	if (straw.Get(housep, sizeof(HousesType)) != sizeof(HousesType)) {
		return(false);
	}

	/*
	**	Read & verify the save-game version #
	*/
	if (straw.Get(&version, sizeof(version)) != sizeof(version)) {
		return(false);
	}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (version != SAVEGAME_VERSION && ((version-1 != SAVEGAME_VERSION)) ) {
#else
	if (version != SAVEGAME_VERSION) {
#endif
		return(false);
	}
	return(true);
}


/***************************************************************************
 * Reconcile_Players -- Reconciles loaded data with the 'Players' vector	*
 *                                                                         *
 * This function is for supporting loading a saved multiplayer game.  		*
 * When the game is loaded, we have to figure out which house goes with		*
 * which entry in the Players vector.  We also have to figure out if 		*
 * everyone who was originally in the game is still with us, and if not, 	*
 * turn their stuff over to the computer.												*
 *                                                                         *
 * So, this function does the following:												*
 * - For every name in 'Players', makes sure that name is in the House		*
 *   array; if not, it's a fatal error.												*
 * - For every human-controlled house, makes sure there's a player	 		*
 *   with that name; if not, it turns that house over to the computer.		*
 * - Fills in the Player's house ID														*
 *                                                                         *
 * This assumes that each player MUST keep their name the same as it was	*
 * when the game was saved!  It's also assumed that the network 				*
 * connections have not been formed yet, since Player[i]->Player.ID will	*
 * be invalid until this routine has been called.									*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		true = OK, false = error															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   09/29/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Reconcile_Players(void)
{
	int i;
	int found;
	HousesType house;
	HouseClass * housep;

	/*
	**	If there are no players, there's nothing to do.
	*/
	if (Session.Players.Count()==0)
		return (true);

	/*
	**	Make sure every name we're connected to can be found in a House
	*/
	for (i = 0; i < Session.Players.Count(); i++) {
		found = 0;
		for (house = HOUSE_MULTI1; house < HOUSE_MULTI1 +
			Session.MaxPlayers; house++) {

			housep = HouseClass::As_Pointer(house);
			if (!housep) {
				continue;
			}

			if (!stricmp(Session.Players[i]->Name, housep->IniName)) {
				found = 1;
				break;
			}
		}
		if (!found)
			return (false);
	}

	//
	// Loop through all Houses; if we find a human-owned house that we're
	// not connected to, turn it over to the computer.
	//
	for (house = HOUSE_MULTI1; house < HOUSE_MULTI1 +
		Session.MaxPlayers; house++) {
		housep = HouseClass::As_Pointer(house);
		if (!housep) {
			continue;
		}

		//
		// Skip this house if it wasn't human to start with.
		//
		if (!housep->IsHuman) {
			continue;
		}

		//
		// Try to find this name in the Players vector; if it's found, set
		// its ID to this house.
		//
		found = 0;
		for (i = 0; i < Session.Players.Count(); i++) {
			if (!stricmp(Session.Players[i]->Name, housep->IniName)) {
				found = 1;
				Session.Players[i]->Player.ID = house;
				break;
			}
		}

		/*
		**	If this name wasn't found, remove it
		*/
		if (!found) {

			/*
			**	Turn the player's house over to the computer's AI
			*/
			housep->IsHuman = false;
			housep->IsStarted = true;
//			housep->Smartness = IQ_MENSA;
			housep->IQ = Rule.MaxIQ;
			strcpy (housep->IniName, Text_String(TXT_COMPUTER));

			Session.NumPlayers--;
		}
	}

	//
	// If all went well, our Session.NumPlayers value should now equal the value
	// from the saved game, minus any players we removed.
	//
	if (Session.NumPlayers == Session.Players.Count()) {
		return (true);
	} else {
		return (false);
	}
}


/***************************************************************************
 * MPlayer_Save_Message -- pops up a "saving..." message                   *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/30/1995 BRR : Created.                                             *
 *=========================================================================*/
void MPlayer_Save_Message(void)
{
	//char *txt = Text_String(
}