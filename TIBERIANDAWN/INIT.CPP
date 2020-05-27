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

/* $Header:   F:\projects\c&c\vcs\code\init.cpv   2.18   16 Oct 1995 16:50:16   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INIT.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : January 20, 1992                                             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Anim_Init -- Initialize the VQ animation control structure.                               *
 *   Init_Game -- Main game initialization routine.                                            *
 *   Load_Recording_Values -- Loads recording values from recording file                       *
 *   Obfuscate -- Sufficiently transform parameter to thwart casual hackers.                   *
 *   Parse_Command_Line -- Parses the command line parameters.                                 *
 *   Parse_INI_File -- Parses CONQUER.INI for special options                                  *
 *   Play_Intro -- plays the introduction & logo movies                                        *
 *   Save_Recording_Values -- Saves recording values to a recording file                       *
 *   Select_Game -- The game's main menu                                                       *
 *   Version_Number -- Determines the version number.                                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"loaddlg.h"
#include	"tcpip.h"
#include	<conio.h>
#include	<dos.h>
#include  "ccdde.h"

static HANDLE			hCCLibrary;

/****************************************
**	Function prototypes for this module **
*****************************************/
static void Play_Intro(bool for_real = false);

extern "C" {
extern long RandNumb;
}

extern int SimRandIndex;

#define ATTRACT_MODE_TIMEOUT	3600		// timeout for attract mode
#if(0)

long FAR PASCAL _export Start_Game_Proc(HWND hwnd, UINT message, UINT wParam, LONG lParam)
{
	switch (message) {
		case WM_CREATE:
			break;

		case WM_COMMAND:
			EndDialog(hwnd, TRUE);
			AllDone = TRUE;
			break;

		case WM_DESTROY:
			EndDialog(hwnd, TRUE);
			break;
	}
	return(DefWindowProc(hwnd, message, wParam, lParam));
}
#endif


extern bool Server_Remote_Connect(void);
extern bool Client_Remote_Connect(void);
extern bool SpawnedFromWChat;


/***********************************************************************************************
 * Init_Game -- Main game initialization routine.                                              *
 *                                                                                             *
 *    Perform all one-time game initializations here. This includes all                        *
 *    allocations and table setups. The intro and other one-time startup                       *
 *    tasks are also performed here.                                                           *
 *                                                                                             *
 * INPUT:   argc,argv   -- Command line arguments.                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this ONCE!                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Init_Game(int , char *[])
{
	void const *temp_mouse_shapes;

	CCDebugString ("C&C95 - About to load reslib.dll\n");
	hCCLibrary = LoadLibrary("reslib.dll");

	/*
	**	Initialize the game object heaps.
	*/
	CCDebugString ("C&C95 - About to enter Units.Set_Heap\n");
	Units.Set_Heap(UNIT_MAX);
	CCDebugString ("C&C95 - About to enter Factories.Set_Heap\n");
	Factories.Set_Heap(FACTORY_MAX);
	CCDebugString ("C&C95 - About to enter Terrains.Set_Heap\n");
	Terrains.Set_Heap(TERRAIN_MAX);
	CCDebugString ("C&C95 - About to enter Templates.Set_Heap\n");
	Templates.Set_Heap(TEMPLATE_MAX);
	CCDebugString ("C&C95 - About to enter Smudges.Set_Heap\n");
	Smudges.Set_Heap(SMUDGE_MAX);
	CCDebugString ("C&C95 - About to enter Overlays.Set_Heap\n");
	Overlays.Set_Heap(OVERLAY_MAX);
	CCDebugString ("C&C95 - About to enter Infantry.Set_Heap\n");
	Infantry.Set_Heap(INFANTRY_MAX);
	CCDebugString ("C&C95 - About to enter Bullets.Set_Heap\n");
	Bullets.Set_Heap(BULLET_MAX);
	CCDebugString ("C&C95 - About to enter Buildings.Set_Heap\n");
	Buildings.Set_Heap(BUILDING_MAX);
	CCDebugString ("C&C95 - About to enter Anims.Set_Heap\n");
	Anims.Set_Heap(ANIM_MAX);
	CCDebugString ("C&C95 - About to enter Aircraft.Set_Heap\n");
	Aircraft.Set_Heap(AIRCRAFT_MAX);
	CCDebugString ("C&C95 - About to enter Triggers.Set_Heap\n");
	Triggers.Set_Heap(TRIGGER_MAX);
	CCDebugString ("C&C95 - About to enter TeamTypes.Set_Heap\n");
	TeamTypes.Set_Heap(TEAMTYPE_MAX);
	CCDebugString ("C&C95 - About to enter Teams.Set_Heap\n");
	Teams.Set_Heap(TEAM_MAX);
	CCDebugString ("C&C95 - About to enter Houses.Set_Heap\n");
	Houses.Set_Heap(HOUSE_MAX);

	/*
	**	Initialize all the waypoints to invalid values.
	*/
	CCDebugString ("C&C95 - About to clear waypoints\n");
	memset(Waypoint, 0xFF, sizeof(Waypoint));

	/*
	**	Setup the keyboard processor in preparation for the game.
	*/
	CCDebugString ("C&C95 - About to do various keyboard inits\n");
#ifdef FIX_ME_LATER
	Keyboard_Attributes_Off(TRACKEXT | PAUSEON | BREAKON | SCROLLLOCKON | CTRLSON | CTRLCON | PASSBREAKS | FILTERONLY | TASKSWITCHABLE);
#endif //FIX_ME_LATER
	Keyboard::Clear();
	Kbd.Clear();

	/*
	**	This is the shape staging buffer. It must always be available, so it is
	**	allocated here and never freed. The library sets the globals ShapeBuffer
	**	and ShapeBufferSize to these values, so it can be accessed for other
	**	purposes.
	*/
	CCDebugString ("C&C95 - About to call Set_Shape_Buffer\n");
	Set_Shape_Buffer(new unsigned char[SHAPE_BUFFER_SIZE], SHAPE_BUFFER_SIZE);

	/*
	**	Bootstrap enough of the system so that the error dialog box can sucessfully
	**	be displayed.
	*/
	CCDebugString ("C&C95 - About to register CCLOCAL.MIX\n");
#ifdef DEMO
	new MixFileClass("DEMOL.MIX");
	MixFileClass::Cache("DEMOL.MIX");
#else
	int temp = RequiredCD;
	RequiredCD = -2;
	new MixFileClass("CCLOCAL.MIX");			// Cached.
	MixFileClass::Cache("CCLOCAL.MIX");
	CCDebugString ("C&C95 - About to register UPDATE.MIX\n");
	new MixFileClass("UPDATE.MIX");			// Cached.
	new MixFileClass("UPDATA.MIX");			// Cached.
	CCDebugString ("C&C95 - About to register UPDATEC.MIX\n");
	new MixFileClass("UPDATEC.MIX");			// Cached.
	MixFileClass::Cache("UPDATEC.MIX");
#ifdef JAPANESE
	CCDebugString ("C&C95 - About to register LANGUAGE.MIX\n");
	new MixFileClass("LANGUAGE.MIX");
#endif	//JAPANESE

	RequiredCD = temp;

#endif
	CCDebugString ("C&C95 - About to load fonts\n");
	Green12FontPtr = Load_Alloc_Data(CCFileClass("12GREEN.FNT"));
	Green12GradFontPtr = Load_Alloc_Data(CCFileClass("12GRNGRD.FNT"));
	MapFontPtr = Load_Alloc_Data(CCFileClass("8FAT.FNT"));
	Font8Ptr = MixFileClass::Retrieve(FONT8);
	FontPtr = (char *)Font8Ptr;
	Set_Font(FontPtr);
	Font3Ptr = MixFileClass::Retrieve(FONT3);
//	Font6Ptr = MixFileClass::Retrieve(FONT6);
   	 Font6Ptr = Load_Alloc_Data(CCFileClass("6POINT.FNT"));
	//ScoreFontPtr = MixFileClass::Retrieve("12GRNGRD.FNT");	//GRAD12FN");	//("SCOREFNT.FNT");
	ScoreFontPtr = Load_Alloc_Data(CCFileClass("12GRNGRD.FNT"));
	FontLEDPtr = MixFileClass::Retrieve("LED.FNT");
	VCRFontPtr = MixFileClass::Retrieve("VCR.FNT");
//	GradFont6Ptr = MixFileClass::Retrieve("GRAD6FNT.FNT");
	GradFont6Ptr = Load_Alloc_Data(CCFileClass("GRAD6FNT.FNT"));
	BlackPalette = new(MEM_CLEAR|MEM_REAL) unsigned char[768];
	GamePalette = new(MEM_CLEAR|MEM_REAL) unsigned char[768];
	OriginalPalette = new(MEM_CLEAR|MEM_REAL) unsigned char[768];
	WhitePalette = new(MEM_CLEAR|MEM_REAL) unsigned char[768];
	memset(WhitePalette, 63, 768);

	CCDebugString ("C&C95 - About to set palette\n");
	memset(BlackPalette, 0x01, 768);
	if (!Special.IsFromInstall) Set_Palette(BlackPalette);
	memset(BlackPalette, 0, 768);
	if (!Special.IsFromInstall) {
		Set_Palette(BlackPalette);
		CCDebugString ("C&C95 - About to clear visible page\n");
		VisiblePage.Clear();
	}

	Set_Palette(GamePalette);

	CCDebugString ("C&C95 - About to set the mouse shape\n");
	/*
	** Since there is no mouse shape currently available we need'
	** to set one of our own.
	*/
	ShowCursor (FALSE);
	if (MouseInstalled) {
		temp_mouse_shapes = MixFileClass::Retrieve("MOUSE.SHP");
		if (temp_mouse_shapes) {
			Set_Mouse_Cursor(0, 0, Extract_Shape(temp_mouse_shapes,0));
			while (Get_Mouse_State() > 1) {
				Show_Mouse();
			}
		}
	}

	CCDebugString ("C&C95 - About to enter wait for focus loop\n");
	/*
	** Process the message loop until we are in focus.
	*/
	do {
		CCDebugString ("C&C95 - About to call Keyboard::Check\n");
		Keyboard::Check();
	}while (!GameInFocus);
	AllSurfaces.SurfacesRestored=FALSE;

	CCDebugString ("C&C95 - About to load the language file\n");
	/*
	**	Fetch the language text from the hard drive first. If it cannot be
	**	found on the hard drive, then look for it in the mixfile.
	*/
	if (RawFileClass(Language_Name("CONQUER")).Is_Available()) {
		SystemStrings = (char const *)Load_Alloc_Data(RawFileClass(Language_Name("CONQUER")));
	} else {
		SystemStrings = (char const *)MixFileClass::Retrieve(Language_Name("CONQUER"));
	}

	/*
	**	Default palette initialization. Uses the desert palette for convenience,
	**	but only the non terrain specific colors matter.
	*/
	//Mem_Copy((void *)MixFileClass::Retrieve("TEMPERAT.PAL"), GamePalette, 768L);
	CCFileClass palfile ("TEMPERAT.PAL");
	palfile.Read (GamePalette, 768L);

	if (!MouseInstalled) {
		char buffer[255];
		Set_Palette(GamePalette);
#ifdef GERMAN
		sprintf(buffer, "Command & Conquer kann Ihren Maustreiber nicht finden..");
#else
#ifdef FRENCH
		sprintf(buffer, "Command & Conquer ne peut pas dÇtecter votre gestionnaire de souris.");
#else
		sprintf(buffer, "Command & Conquer is unable to detect your mouse driver.");
#endif
#endif
		CCMessageBox().Process(buffer, TXT_OK);
		Prog_End();
		exit(1);
	}

#ifdef DEMO
	/*
	**	Add in any override path specified in the conquer.ini file.
	*/
	if (strlen(OverridePath)) {
		CCFileClass::Set_Search_Drives(OverridePath);
	}
#endif

#if (0)	//ST - 1/2/2019 5:49PM			
	CCDebugString ("C&C95 - About to search for CD drives\n");
	/*
	**	Always try to look at the CD-ROM for data files.
	*/
	if (!CCFileClass::Is_There_Search_Drives()) {

		/*
		** If there are no search drives specified then we must be playing
		** off cd, so read files from there.
		*/
		int error;

		do {
			if (!CDList.Get_Number_Of_Drives()){
				Set_Palette(GamePalette);
				Show_Mouse();
				CCMessageBox().Process(TXT_CD_ERROR1, TXT_OK);
				Prog_End();
				exit(EXIT_FAILURE);
			}
			CCFileClass::Set_CD_Drive( CDList.Get_First_CD_Drive() );

			error = CCFileClass::Set_Search_Drives("?:\\");
			switch(error) {
				case 1:
					Set_Palette(GamePalette);
					Show_Mouse();
					CCMessageBox().Process(TXT_CD_ERROR1, TXT_OK);
					Prog_End();
					exit(EXIT_FAILURE);

				case 2:
					Set_Palette(GamePalette);
					Show_Mouse();
					if (CCMessageBox().Process(TXT_CD_DIALOG_1, TXT_OK, TXT_CANCEL) == 1) {
						Prog_End();
						exit(EXIT_FAILURE);
					}
					Hide_Mouse();
					break;

				default:
					Show_Mouse();
					if (!Force_CD_Available(RequiredCD)) {
						Prog_End();
						exit(EXIT_FAILURE);
					}
					Hide_Mouse();
					break;
			}
		} while (error);

#ifdef DEMO
		RequiredCD = -2;
#else
		RequiredCD = -1;
#endif
	} else {

		/*
		** If there are search drives specified then all files are to be
		** considered local.
		*/
		RequiredCD = -2;
	}
#endif


#ifndef DEMO
	CCDebugString ("C&C95 - About to register addon mixfiles\n");
	/*
	**	Before all else, cache any additional mixfiles.
	*/

	/*
	** Need to search the search paths. ST - 3/15/2019 2:18PM
	*/
	const char *path = ".\\";
	char search_path[_MAX_PATH];
	char scan_path[_MAX_PATH];

	for (int p=0 ; p<100 ; p++) {
		
		strcpy(search_path, path);
		if (search_path[strlen(search_path) - 1] != '\\') {
			strcat(search_path, "\\");
		}

		strcpy(scan_path, search_path);
		strcat(scan_path, "SC*.MIX");

		WIN32_FIND_DATA find_data;
		memset(&find_data, 0, sizeof(find_data));
		HANDLE file_handle = FindFirstFile(scan_path, &find_data);
		if (file_handle != INVALID_HANDLE_VALUE)
		{
			do
			{
				char *ptr = strdup(find_data.cFileName);
				new MixFileClass(ptr);
				MixFileClass::Cache(ptr);
			} while (FindNextFile(file_handle, &find_data));
			FindClose(file_handle);
		}
		
		memset(&find_data, 0, sizeof(find_data));
		strcpy(scan_path, search_path);
		strcat(scan_path, "Ss*.MIX");
		file_handle = FindFirstFile(scan_path, &find_data);
		if (file_handle != INVALID_HANDLE_VALUE)
		{
			do
			{
				char *ptr = strdup(find_data.cFileName);
				new MixFileClass(ptr);
				MixFileClass::Cache(ptr);
			} while (FindNextFile(file_handle, &find_data));
			FindClose(file_handle);
		}

		path = CDFileClass::Get_Search_Path(p);

		if (path == NULL) {
			break;
		}
	}

#if (0)
	struct find_t ff;		// for _dos_findfirst
	if (!_dos_findfirst("SC*.MIX", _A_NORMAL, &ff)) {
		char * ptr;
		do {
			ptr = strdup(ff.name);
			new MixFileClass(ptr);
			MixFileClass::Cache(ptr);
//			free(ptr);
		} while(!_dos_findnext(&ff));
	}
	if (!_dos_findfirst("SS*.MIX", _A_NORMAL, &ff)) {
		char * ptr;
		do {
			ptr = strdup(ff.name);
			new MixFileClass(ptr);
//			free(ptr);
		} while(!_dos_findnext(&ff));
	}
#endif
#endif	//DEMO

	CCDebugString ("C&C95 - About to register GENERAL.MIX\n");
	//
	// This is a problem because registering the mix file can call Force_CD_Available which will try to load General.Mix
	// Might as well just cut to the chase and call it directly.
	// ST - 1/3/2019 5:19PM
	//
	//if (GeneralMix) delete GeneralMix;
	//GeneralMix = new MixFileClass("GENERAL.MIX");
	Force_CD_Available(RequiredCD);

//	if (!_dos_findfirst("SC*.MIX", _A_NORMAL, &ff)) {
//		do {
//			new MixFileClass(ff.name);
//			MixFileClass::Cache(ff.name);
//		} while(!_dos_findnext(&ff));
//	}

	/*
	**	Inform the file system of the various MIX files.
	*/
#ifdef DEMO
	new MixFileClass("DEMO.MIX");
	if (CCFileClass("DEMOM.MIX").Is_Available()) {
		if (!MoviesMix) MoviesMix = new MixFileClass("DEMOM.MIX");
		ScoresPresent = true;
		ThemeClass::Scan();
	}

#else
	CCDebugString ("C&C95 - About to register CONQUER.MIX\n");
	new MixFileClass("CONQUER.MIX");			// Cached.
	CCDebugString ("C&C95 - About to register TRANSIT.MIX\n");
	new MixFileClass("TRANSIT.MIX");

	CCDebugString ("C&C95 - About to register GENERAL.MIX\n");
	if (!GeneralMix) GeneralMix = new MixFileClass("GENERAL.MIX");			// Never cached.

//	if (CCFileClass("MOVIES.MIX").Is_Available()) {
	CCDebugString ("C&C95 - About to register MOVIES.MIX\n");
		if (!MoviesMix) MoviesMix = new MixFileClass("MOVIES.MIX");			// Never cached.
//	}



#if (0)

	/*
	** Extract a movie from a mixfile.
	*/
	char *file_ptr = (char*)Alloc (32 * 1024 * 1024, MEM_NORMAL);
	CCFileClass whatever ("PINTLE.VQA");

	int len = whatever.Size();

	whatever.Open();

	DWORD	actual;
		HANDLE sfile = CreateFile("c:\\temp\\PINTLE.VQA", GENERIC_WRITE, 0,
												NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (sfile != INVALID_HANDLE_VALUE){
		SetFilePointer (sfile, 0, NULL, FILE_END);

		do{
			whatever.Read (file_ptr, MIN (len, 1024*64));
			WriteFile(sfile, file_ptr, MIN (len, 1024*64), &actual, NULL);
			len -= MIN (len, 1024*64);
		}while ( len >0 );

		CloseHandle (sfile);
	}

	whatever.Close();

	Free (file_ptr);

#endif	//(0)



	/*
	**	Register the score mixfile.
	*/
	CCDebugString ("C&C95 - About to register SCORES.MIX\n");
	ScoresPresent = false;
//	if (CCFileClass("SCORES.MIX").Is_Available()) {
		ScoresPresent = true;
		if (!ScoreMix) {
			ScoreMix = new MixFileClass("SCORES.MIX");
			ThemeClass::Scan();
		}
//	}
#endif

	/*
	**	These are sound card specific, but the install program would have
	**	copied the coorect versions to the hard drive.
	*/
	CCDebugString ("C&C95 - About to register SPEECH.MIX\n");
	if (CCFileClass("SPEECH.MIX").Is_Available()) {
		new MixFileClass("SPEECH.MIX");			// Never cached.
	}
	CCDebugString ("C&C95 - About to register SOUNDS.MIX\n");
	new MixFileClass("SOUNDS.MIX");			// Cached.

	/*
	**	Initialize the animation system.
	*/
	CCDebugString ("C&C95 - About to initialise the animation system\n");
	Anim_Init();

	if (SpawnedFromWChat){
		Special.IsFromWChat = true;
	}

	/*
	**	Play the introduction movies.
	*/
	CCDebugString ("C&C95 - About to play the intro movie\n");
	if (!Special.IsFromInstall && !Special.IsFromWChat) Play_Intro(true);

	/*
	**	Wait for a VSync; during the vertical blank, set the game palette & blit
	**	the title screen.  We must ensure no RGB values in the game palette match
	**	those in the WWLIB's 'CurrentPalette', or the WWLIB palette-set routine
	**	will skip that color; the VQ player will have changed that color (behind
	**	WWLIB's back), so it will be incorrect.
	*/
	memset(CurrentPalette, 0x01, 768);

	if (!Special.IsFromInstall) {
		Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
		Blit_Hid_Page_To_Seen_Buff();
	}

	Hide_Mouse();
	Wait_Vert_Blank();
	if (!Special.IsFromInstall) {
		Set_Palette(Palette);
		Blit_Hid_Page_To_Seen_Buff();
		Show_Mouse();
	}
	Call_Back();
//	Window_Dialog_Box(hCCLibrary, "DIALOG_1", MainWindow, MakeProcInstance((FARPROC)Start_Game_Proc, hInstance));
//	if (hCCLibrary) FreeLibrary(hCCLibrary);

#ifdef DEMO
	MixFileClass::Cache("DEMO.MIX");
	MixFileClass::Cache("SOUNDS.MIX");
#else
	/*
	**	Cache the main game data. This operation can take a very long time.
	*/
	MixFileClass::Cache("CONQUER.MIX");
	if (SampleType != 0 && !Debug_Quiet) {
		MixFileClass::Cache("SOUNDS.MIX");
		if (Special.IsJuvenile) {
			new MixFileClass("ZOUNDS.MIX");		// Cached.
			MixFileClass::Cache("ZOUNDS.MIX");
		}
	}
	Call_Back();
#endif

//	malloc(2);

	/*
	**	Perform any special debug-only processing. This includes preparing the
	**	monochrome screen.
	*/
	Mono_Clear_Screen();

#ifdef ONHOLD
	/*
	** Check for addition options not specified on the command-line.  This must
	** be done before the One_Time calls, but after the shape buffer is set up.
	*/
	Parse_INI_File();
#endif

	/*
	**	Perform one-time game system initializations.
	*/
	Call_Back();
//	malloc(3);
	Map.One_Time();
//	malloc(4);
	Logic.One_Time();
//	malloc(5);
	Options.One_Time();

//	malloc(6);

	ObjectTypeClass::One_Time();
	BuildingTypeClass::One_Time();
	BulletTypeClass::One_Time();
	HouseTypeClass::One_Time();

	TemplateTypeClass::One_Time();
	OverlayTypeClass::One_Time();
	SmudgeTypeClass::One_Time();
	TerrainTypeClass::One_Time();
	UnitTypeClass::One_Time();

	InfantryTypeClass::One_Time();
	AnimTypeClass::One_Time();
	AircraftTypeClass::One_Time();
	HouseClass::One_Time();

	/*
	**	Speech holding tank buffer. Since speech does not mix, it can be placed
	**	into a custom holding tank only as large as the largest speech file to
	**	be played.
	*/
	SpeechBuffer = new char [SPEECH_BUFFER_SIZE];
	Call_Back();

	/*
	**	WWLIB bug: MouseState is in some undefined state; show the mouse until
	**	it really shows.
	*/
	Map.Set_Default_Mouse(MOUSE_NORMAL, false);
	Show_Mouse();
//#ifdef FIX_ME_LATER
	while (Get_Mouse_State() > 0) Show_Mouse();
//#endif //FIX_ME_LATER
	Call_Back();

#ifndef DEMO
	/*
	**	Load multiplayer scenario descriptions
	*/
	Read_Scenario_Descriptions();
#endif

	/*
	**	Initialize the multiplayer score values
	*/
	MPlayerGamesPlayed = 0;
	MPlayerNumScores = 0;
	MPlayerCurGame = 0;
	for (int i = 0; i < MAX_MULTI_NAMES; i++) {
		MPlayerScore[i].Name[0] = '\0';
		MPlayerScore[i].Wins = 0;
		for (int j = 0; j < MAX_MULTI_GAMES; j++) {
			MPlayerScore[i].Kills[j] = -1;	// -1 = this player didn't play this round
		}
	}

	/*
	** Copy the title screen's palette into the GamePalette & OriginalPalette,
	** because the options Load routine uses these palettes to set the brightness, etc.
	*/
	memcpy (GamePalette, Palette, 768);
	memcpy (OriginalPalette, Palette, 768);

	/*
	**	Read game options, so the GameSpeed is initialized when multiplayer
	** dialogs are invoked.  (GameSpeed must be synchronized between systems.)
	*/
	Options.Load_Settings();

	return(true);
}


//#ifndef NOMEMCHECK
void Uninit_Game(void)
{
	delete Map.ShadowPage;
	Map.ShadowPage = NULL;
	Map.Free_Cells();

	delete [] SpeechBuffer;

	CCFileClass::Clear_Search_Drives();
	MixFileClass::Free_All();

	Units.Set_Heap(0);
	Factories.Set_Heap(0);
	Terrains.Set_Heap(0);
	Templates.Set_Heap(0);
	Smudges.Set_Heap(0);
	Overlays.Set_Heap(0);
	Infantry.Set_Heap(0);
	Bullets.Set_Heap(0);
	Buildings.Set_Heap(0);
	Anims.Set_Heap(0);
	Aircraft.Set_Heap(0);
	Triggers.Set_Heap(0);
	TeamTypes.Set_Heap(0);
	Teams.Set_Heap(0);
	Houses.Set_Heap(0);

	delete [] _ShapeBuffer;
	Set_Shape_Buffer(NULL, 0);
	delete [] BlackPalette;
	delete [] GamePalette;
	delete [] OriginalPalette;
	delete [] WhitePalette;

	WWDOS_Shutdown();
	delete [] Palette;
}
//#endif

extern bool Do_The_Internet_Menu_Thang(void);
extern int ShowCommand;

/***********************************************************************************************
 * Select_Game -- The game's main menu                                                         *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		fade		if true, will fade the palette in gradually												  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		none.																												  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		none.																												  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/05/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
extern int Com_Fake_Scenario_Dialog(void);
extern int Com_Show_Fake_Scenario_Dialog(void);
extern int WChatMaxAhead;
extern int WChatSendRate;
void Check_From_WChat(char *wchat_name);

bool Select_Game(bool fade)
{
	enum {
		SEL_TIMEOUT = -1,				// main menu timeout--go into attract mode
#ifdef NEWMENU
		SEL_NEW_SCENARIO,				// Expansion scenario to play.
#endif
		SEL_START_NEW_GAME,			// start a new game
#ifdef BONUS_MISSIONS
		SEL_BONUS_MISSIONS,
#endif	//BONUS_MISSIONS
		SEL_INTERNET,
		SEL_LOAD_MISSION,				// load a saved game
		SEL_MULTIPLAYER_GAME,		// play modem/null-modem/network game
		SEL_INTRO,						// replay the intro
		SEL_EXIT,						// exit to DOS
		SEL_FAME,						// view the hall of fame
		SEL_NONE,						// placeholder default value
	};
	bool gameloaded=false;			// Has the game been loaded from the menu?
	int selection;						// the default selection
	bool process = true;				// false = break out of while loop
	bool display = true;
	CountDownTimerClass count;
	int cd_index;

	MEMORYSTATUS	mem_info;
	mem_info.dwLength=sizeof(mem_info);
	GlobalMemoryStatus(&mem_info);

	/*
	** Enable the DDE Server so we can get internet start game packets from WChat
	*/
	DDEServer.Enable();

	if (Special.IsFromInstall) {
		/*
		** Special case for machines with 12 megs or less - just play intro, no choose side screen
		*/
		if (mem_info.dwTotalPhys < 12*1024*1024){
			VisiblePage.Clear();
			Play_Movie("INTRO2", THEME_NONE, false);
			BreakoutAllowed = true;
			Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
			fade = true;
			VisiblePage.Clear();
		}else{
			display = false;
			Show_Mouse();
		}
	}

	/*
	**	[Re]set any globals that need it, in preparation for a new scenario
	*/
	GameActive = true;
	DoList.Init();
	OutList.Init();
	Frame = 0;
	PlayerWins = false;
	PlayerLoses = false;
	MPlayerObiWan = false;
	Debug_Unshroud = false;
	Map.Set_Cursor_Shape(0);
	Map.PendingObjectPtr = 0;
	Map.PendingObject = 0;
	Map.PendingHouse = HOUSE_NONE;

	/*
	** Initialize multiplayer-protocol-specific variables:
	** If CommProtocol MULTI_E_COMP is used, you must:
	** Init FrameSendRate to a sensible value (3 is good)
	** Init MPlayerMaxAhead to an even multiple of FrameSendRate, and it must
	**   be at least 2 * MPlayerMaxAhead
	*/
	CommProtocol = COMM_PROTOCOL_SINGLE_NO_COMP;
	if (!Special.IsFromWChat){
		FrameSendRate = 3;
	}

	ProcessTicks = 0;
	ProcessFrames = 0;
	DesiredFrameRate = 30;
//#if(TIMING_FIX)
	NewMaxAheadFrame1 = 0;
	NewMaxAheadFrame2 = 0;
//#endif

	/*
	**	Init multiplayer game scores.  Let Wins accumulate; just init the current
	** Kills for this game.  Kills of -1 means this player didn't play this round.
	*/
	for (int i = 0 ; i < MAX_MULTI_GAMES; i++) {
		MPlayerScore[i].Kills[MPlayerCurGame] = -1;
	}

	/*
	**	Set default mouse shape
	*/
	Map.Set_Default_Mouse(MOUSE_NORMAL, false);

	/*
	**	If the last game we played was a multiplayer game, jump right to that
	**	menu by pre-setting 'selection'.
	*/
	if (GameToPlay == GAME_NORMAL) {
		selection = SEL_NONE;
	} else {
		selection = SEL_MULTIPLAYER_GAME;
	}

	/*
	**	Main menu processing; only do this if we're not in editor mode.
	*/
	if (!Debug_Map) {

		/*
		**	Menu selection processing loop
		*/
		ScenarioInit++;
		Theme.Queue_Song(THEME_MAP1);
		ScenarioInit--;

		/*
		** If we're playing back a recording, load all pertinant values & skip
		** the menu loop.  Hide the now-useless mouse pointer.
		*/
		if (PlaybackGame && RecordFile.Is_Available()) {
			if (RecordFile.Open(READ)) {
				Load_Recording_Values();
				process = false;
				Theme.Fade_Out();
			} else
				PlaybackGame = false;
		}


		/*
		** Handle case where we were spawned from Wchat
		*/
		if (SpawnedFromWChat){
			Special.IsFromInstall = false;	//Dont play intro if we were spawned from wchat
			selection = SEL_INTERNET;
			Theme.Queue_Song(THEME_NONE);
			GameToPlay = GAME_INTERNET;
			display = false;
			Set_Logic_Page(SeenBuff);
		}


		while (process) {

			/*
			** If we have just received input focus again after running in the background then
			** we need to redraw.
			*/
			if (AllSurfaces.SurfacesRestored){
				AllSurfaces.SurfacesRestored=FALSE;
				display=TRUE;
			}

			/*
			**	Redraw the title page if needed
			*/
			if (display) {
				Hide_Mouse();

				/*
				**	Display the title page; fade it in if this is the first time
				**	through the loop, and the 'fade' flag is true
				*/
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				memcpy (GamePalette, Palette, 768);
				Blit_Hid_Page_To_Seen_Buff();

				if (fade) {
					Fade_Palette_To(Palette, FADE_PALETTE_SLOW, Call_Back);
					fade = false;
				}

				Set_Logic_Page(SeenBuff);
#ifdef VIRGIN_CHEAT_KEYS
				Fancy_Text_Print("V.%d%s", SeenBuff.Get_Width() - 1, SeenBuff.Get_Height() - 10, DKGREY, TBLACK, TPF_6POINT|TPF_FULLSHADOW|TPF_RIGHT, Version_Number(), VersionText, FOREIGN_VERSION_NUMBER);
//				Fancy_Text_Print("V.%d%s%02d", 319, 190, DKGREY, TBLACK, TPF_6POINT|TPF_FULLSHADOW|TPF_RIGHT, Version_Number(), VersionText, FOREIGN_VERSION_NUMBER);
#else

#ifdef DEMO
				Version_Number();
				Fancy_Text_Print("DEMO V%s", SeenBuff.Get_Width() - 1, SeenBuff.Get_Height() - 10, DKGREY, TBLACK, TPF_6POINT|TPF_FULLSHADOW|TPF_RIGHT, VersionText);
#else
				Fancy_Text_Print("V.%d%s", SeenBuff.Get_Width() - 1, SeenBuff.Get_Height() - 10, DKGREY, TBLACK, TPF_6POINT|TPF_FULLSHADOW|TPF_RIGHT, Version_Number(), VersionText);
#endif
#endif
				display = false;
				Show_Mouse();
			} else {
				if (RunningAsDLL) {
					return true;;
				}
			}
			/*
			**	Display menu and fetch selection from player.
			*/
			if (Special.IsFromInstall && mem_info.dwTotalPhys >= 12*1024*1024){
				selection = SEL_START_NEW_GAME;
				Theme.Queue_Song(THEME_NONE);
			}

			/*
			** Handle case where we were spawned from Wchat
			*/
			if (Special.IsFromWChat && DDEServer.Get_MPlayer_Game_Info()){
				Check_From_WChat(NULL);
				selection = SEL_MULTIPLAYER_GAME;
				Theme.Queue_Song(THEME_NONE);
				GameToPlay = GAME_INTERNET;
			}else{
				/*
				** We werent spawned but we could still receive a DDE packet from wchat
				*/
				if (DDEServer.Get_MPlayer_Game_Info()){
					Check_From_WChat(NULL);
					/*
					** Make sure top and bottom of screen are clear in 640x480 mode
					*/
					if (ScreenHeight == 480){
						VisiblePage.Fill_Rect (0, 0, 639, 40, 0);
						VisiblePage.Fill_Rect (0, 440, 639, 479, 0);
					}
				}
			}

			if (selection == SEL_NONE) {
//				selection = Main_Menu(0);
				selection = Main_Menu(ATTRACT_MODE_TIMEOUT);
			}
			Call_Back();

			switch (selection) {

#ifdef NEWMENU

				case SEL_INTERNET:
					/*
					** Only call up the internet menu code if we dont already have connect info from WChat
					*/
					if (!DDEServer.Get_MPlayer_Game_Info()){
						CCDebugString ("C&C95 - About to call Internet Menu.\n");
						if (Do_The_Internet_Menu_Thang() && DDEServer.Get_MPlayer_Game_Info()){
							CCDebugString ("C&C95 - About to call Check_From_WChat.\n");
							Check_From_WChat(NULL);
							selection = SEL_MULTIPLAYER_GAME;
							display = false;
							GameToPlay = GAME_INTERNET;
						}else{
							selection = SEL_NONE;
							display = true;
						}
					}else{
						CCDebugString ("C&C95 - About to call Check_From_WChat.\n");
						Check_From_WChat(NULL);
						display = false;
						GameToPlay = GAME_INTERNET;
						selection = SEL_MULTIPLAYER_GAME;
					}
					break;


				/*
				**	Pick an expansion scenario.
				*/
				case SEL_NEW_SCENARIO:
					CarryOverMoney = 0;
					if (Expansion_Dialog()) {
						Theme.Fade_Out();
//						Theme.Queue_Song(THEME_AOI);
						GameToPlay = GAME_NORMAL;
						process = false;
					} else {
						display = true;
						selection = SEL_NONE;
					}
					break;


#ifdef BONUS_MISSIONS

				/*
				**	User selected to play a bonus scenario.
				*/
				case SEL_BONUS_MISSIONS:
					CarryOverMoney = 0;

					/*
					** Ensure that CD1 or CD2 is in the drive. These missions
					** are not on the covert CD.
					*/
					cd_index = Get_CD_Index(CCFileClass::Get_CD_Drive(), 1*60);
					/*
					** If cd_index == 2 then its a covert CD
					*/
					if (cd_index == 2){
						RequiredCD = 0;
						if (!Force_CD_Available (RequiredCD)){
							Prog_End("Select_Game - CD not found", true);
							exit(EXIT_FAILURE);
						}
					}

					if (Bonus_Dialog()) {
						Theme.Fade_Out();
						GameToPlay = GAME_NORMAL;
						process = false;
					} else {
						display = true;
						selection = SEL_NONE;
					}
					break;


#endif	//BONUS_MISSIONS

#endif

				/*
				**	SEL_START_NEW_GAME: Play the game
				*/
				case SEL_START_NEW_GAME:
					CarryOverMoney = 0;

#ifdef DEMO
					Hide_Mouse();
					Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
					Load_Title_Screen("PREPICK.PCX", &HidPage, Palette);
					Blit_Hid_Page_To_Seen_Buff();
					Fade_Palette_To(Palette, FADE_PALETTE_MEDIUM, Call_Back);
					Clear_KeyBuffer();
					while (!Check_Key_Num()) {
						Call_Back();
					}
					Get_Key_Num();
					Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
					Show_Mouse();

					Scenario = 1;
					BuildLevel = 1;
#else
					Scenario = 1;
					BuildLevel = 1;
#endif
					ScenPlayer = SCEN_PLAYER_GDI;
					ScenDir = SCEN_DIR_EAST;
					Whom = HOUSE_GOOD;

#ifndef DEMO
					Theme.Fade_Out();
					Choose_Side();
#endif

					/*
					** If user is playing special mode, do NOT change Whom; leave it set to
					** GDI or NOD.  Ini.cpp will set the player's ActLike to mirror the
					** Whom value.
					*/
					if (Special.IsJurassic && AreThingiesEnabled) {
						ScenPlayer = SCEN_PLAYER_JP;
						ScenDir = SCEN_DIR_EAST;
					}

					GameToPlay = GAME_NORMAL;
					process = false;
					break;

				/*
				**	Load a saved game.
				*/
				case SEL_LOAD_MISSION:
					if (LoadOptionsClass(LoadOptionsClass::LOAD).Process()) {
						//Theme.Fade_Out();
						Theme.Queue_Song(THEME_AOI);
						process = false;
						gameloaded = true;
					} else {
						display = true;
						selection = SEL_NONE;
					}
					break;

				/*
				**	SEL_MULTIPLAYER_GAME: set 'GameToPlay' to NULL-modem, modem, or
				**	network play.
				*/
				case SEL_MULTIPLAYER_GAME:

#ifdef DEMO
					Hide_Mouse();
					Set_Palette(BlackPalette);
					Load_Title_Screen("DEMOPIC.PCX", &HidPage, Palette);
					Blit_Hid_Page_To_Seen_Buff();
					Fade_Palette_To(Palette, FADE_PALETTE_MEDIUM, Call_Back);
					Clear_KeyBuffer();
					while (!Check_Key()) {
						Call_Back();
					}
					Get_Key();
					Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
					Show_Mouse();
					display = true;
					fade = true;
					selection = SEL_NONE;
#else
					switch (GameToPlay) {

						/*
						**	If 'GameToPlay' isn't already set up for a multiplayer game,
						**	we must prompt the user for which type of multiplayer game
						**	they want.
						*/
						case GAME_NORMAL:
							GameToPlay = Select_MPlayer_Game();
							if (GameToPlay == GAME_NORMAL) {		// 'Cancel'
								display = true;
								selection = SEL_NONE;
							}
							break;

						case GAME_NULL_MODEM:
						case GAME_MODEM:
#if (0)
							if ( NullModem.Num_Connections() ) {
								NullModem.Init_Send_Queue();

								if ((GameToPlay == GAME_NULL_MODEM &&
									ModemGameToPlay == MODEM_NULL_HOST) ||
									(GameToPlay == GAME_MODEM &&
									ModemGameToPlay == MODEM_DIALER) ) {

									if ( !Com_Scenario_Dialog() ) {
										GameToPlay = Select_Serial_Dialog();
										if (GameToPlay == GAME_NORMAL) {		// user hit Cancel
											display = true;
											selection = SEL_NONE;
										}
									}
								} else {
									if ( !Com_Show_Scenario_Dialog() ) {
										GameToPlay = Select_Serial_Dialog();
										if (GameToPlay == GAME_NORMAL) {		// user hit Cancel
											display = true;
											selection = SEL_NONE;
										}
									}
								}
							} else {
								GameToPlay = Select_MPlayer_Game();
								if (GameToPlay == GAME_NORMAL) {		// 'Cancel'
									display = true;
									selection = SEL_NONE;
								}
							}
#endif
							break;



#ifdef FORCE_WINSOCK
						/*
						** Handle being spawned from WChat. Intermnet play based on IPX code now.
						*/
						case GAME_INTERNET:
							CCDebugString ("C&C95 - case GAME_INTERNET:\n");
							if (Special.IsFromWChat){
								//MessageBox (NULL, "About to restore focus to C&C95", "C&C95", MB_OK);
								CCDebugString ("C&C95 - About to give myself focus.\n");
								SetForegroundWindow ( MainWindow );
								ShowWindow ( MainWindow, ShowCommand );

								CCDebugString ("C&C95 - About to initialise Winsock.\n");
								if (Winsock.Init()){
									CCDebugString ("C&C95 - About to read multiplayer settings.\n");
									Read_MultiPlayer_Settings ();
									Server = PlanetWestwoodIsHost;

									CCDebugString ("C&C95 - About to set addresses.\n");
									Winsock.Set_Host_Address(PlanetWestwoodIPAddress);

									CCDebugString ("C&C95 - About to call Start_Server or Start_Client.\n");
									if (Server){
										ModemGameToPlay = INTERNET_HOST;
										Winsock.Start_Server();
									}else{
										ModemGameToPlay = INTERNET_JOIN;
										Winsock.Start_Client();
									}


//#if (0)
									/*
									** Flush out any pending packets from a previous game.
									*/
									CCDebugString ("C&C95 - About to flush packet queue.\n");
									CCDebugString ("C&C95 - Allocating scrap memory.\n");
									char *temp_buffer = new char[1024];

									CCDebugString ("C&C95 - Creating timer class instance.\n");
									CountDownTimerClass ptimer;

									CCDebugString ("C&C95 - Entering read loop.\n");
									while (Winsock.Read(temp_buffer, 1024)){

										CCDebugString ("C&C95 - Discarding a packet.\n");
										ptimer.Set (30, true);
										while (ptimer.Time()){};
										CCDebugString ("C&C95 - Ready to check for more packets.\n");

									}
									CCDebugString ("C&C95 - About to delete scrap memory.\n");
									delete temp_buffer;
//#endif	//(0)



								}else{
									CCDebugString ("C&C95 - Winsock failed to initialise.\n");
									GameToPlay = GAME_NORMAL;
									selection = SEL_EXIT;
									Special.IsFromWChat = false;
									break;
								}

								CCDebugString ("C&C95 - About to call Init_Network.\n");
								Init_Network();

								if (DDEServer.Get_MPlayer_Game_Info()){
									CCDebugString ("C&C95 - About to call Read_Game_Options.\n");
									Read_Game_Options( NULL );
								}else{
									Read_Game_Options( "C&CSPAWN.INI" );
								}

								if (Server){

									CCDebugString ("C&C95 - About to call Server_Remote_Connect.\n");
									if (Server_Remote_Connect()){
										CCDebugString ("C&C95 - Server_Remote_Connect returned success.\n");
										break;
									}else{
										/*
										** We failed to connect to the other player
										 *
										 *   SEND FAILURE PACKET TO WCHAT HERE !!!!!
										 *
										*/
										Winsock.Close();
										GameToPlay = GAME_NORMAL;
										selection = SEL_NONE;
										DDEServer.Delete_MPlayer_Game_Info();	// Make sure we dont go round in an infinite loop
										//Special.IsFromWChat = false;
										break;
									}
								}else{
									CCDebugString ("C&C95 - About to call Client_Remote_Connect.\n");
									if (Client_Remote_Connect()){
										CCDebugString ("C&C95 - Client_Remote_Connect returned success.\n");
										break;
									}else{
										/*
										** We failed to connect to the other player
										 *
										 *   SEND FAILURE PACKET TO WCHAT HERE !!!!!
										 *
										*/
										Winsock.Close();
										GameToPlay = GAME_NORMAL;
										selection = SEL_NONE;
										DDEServer.Delete_MPlayer_Game_Info();  // Make sure we dont go round in an infinite loop
										//Special.IsFromWChat = false;
										break;
									}
								}

							}else{
								GameToPlay = Select_MPlayer_Game();
								if (GameToPlay == GAME_NORMAL) {		// 'Cancel'
									display = true;
									selection = SEL_NONE;
								}
							}
							break;

#endif	//FORCE_WINSOCK

					}


					switch (GameToPlay) {
						/*
						**	Internet, Modem or Null-Modem
						*/
						case GAME_MODEM:
						case GAME_NULL_MODEM:
						case GAME_INTERNET:
							Theme.Fade_Out();
							ScenPlayer = SCEN_PLAYER_2PLAYER;
							ScenDir = SCEN_DIR_EAST;
							process = false;
							Options.ScoreVolume = 0;
							break;


						/*
						**	Network (IPX): start a new network game.
						*/
						case GAME_IPX:
							/*
							** Init network system & remote-connect
							*/
							if (Init_Network() && Remote_Connect()) {
#if (0)
								char seed[80];
								sprintf (seed, "Seed: %08x\n", Seed);
								CCDebugString (seed);

								sprintf (seed, "rand: %04x\n", rand());
								CCDebugString (seed);

								sprintf (seed, "rand: %04x\n", rand());
								CCDebugString (seed);

								sprintf (seed, "rand: %04x\n", rand());
								CCDebugString (seed);
#endif
								Options.ScoreVolume = 0;
								ScenPlayer = SCEN_PLAYER_MPLAYER;
								ScenDir = SCEN_DIR_EAST;
								process = false;
								Theme.Fade_Out();
							} else {						// user hit cancel, or init failed
								GameToPlay = GAME_NORMAL;
								display = true;
								selection = SEL_NONE;
							}
							break;
					}
#endif
					break;

				/*
				**	Play a VQ
				*/
				case SEL_INTRO:
					Theme.Fade_Out();
					Theme.Stop();
					Call_Back();

					Force_CD_Available(-1);
					Play_Intro(false);
					Hide_Mouse();

// verify existance of movie file before playing this sequence.
					if (CCFileClass("TRAILER.VQA").Is_Available()) {
						Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
						VisiblePage.Clear();
						if (CCFileClass("ATTRACT2.CPS").Is_Available()){
							Load_Uncompress(CCFileClass("ATTRACT2.CPS"), SysMemPage, SysMemPage, Palette);
							SysMemPage.Scale(SeenBuff, 0, 0, 0, 0, 320, 199, 640, 398);
							Fade_Palette_To(Palette, FADE_PALETTE_MEDIUM, Call_Back);
						}
						Clear_KeyBuffer();
						count.Set(TIMER_SECOND*3);
						while (count.Time()) {
							Call_Back();
						}
						Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);

						Play_Movie("TRAILER");	// Red Alert teaser.
					}

					if (CCFileClass("SIZZLE.VQA").Is_Available()) {
						Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
						VisiblePage.Clear();
						if (CCFileClass("ATTRACT2.CPS").Is_Available()){
							Load_Uncompress(CCFileClass("ATTRACT2.CPS"), SysMemPage, SysMemPage, Palette);
							SysMemPage.Scale(SeenBuff, 0, 0, 0, 0, 320, 199, 640, 398);
							Fade_Palette_To(Palette, FADE_PALETTE_MEDIUM, Call_Back);
						}
						Clear_KeyBuffer();
						count.Set(TIMER_SECOND*3);
						while (count.Time()) {
							Call_Back();
						}
						Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);

						Play_Movie("SIZZLE");	// Red Alert teaser.
					}

					if (CCFileClass("SIZZLE2.VQA").Is_Available()) {
						Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
						VisiblePage.Clear();
						if (CCFileClass("ATTRACT2.CPS").Is_Available()){
							Load_Uncompress(CCFileClass("ATTRACT2.CPS"), SysMemPage, SysMemPage, Palette);
							SysMemPage.Scale(SeenBuff, 0, 0, 0, 0, 320, 199, 640, 398);
							Fade_Palette_To(Palette, FADE_PALETTE_MEDIUM, Call_Back);
						}
						Clear_KeyBuffer();
						count.Set(TIMER_SECOND*3);
						while (count.Time()) {
							Call_Back();
						}
						Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);

						Play_Movie("SIZZLE2");	// Red Alert teaser.
					}

					Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
					VisiblePage.Clear();
					if (CCFileClass("ATTRACT2.CPS").Is_Available()){
						Load_Uncompress(CCFileClass("ATTRACT2.CPS"), SysMemPage, SysMemPage, Palette);
						SysMemPage.Scale(SeenBuff, 0, 0, 0, 0, 320, 199, 640, 398);
						Fade_Palette_To(Palette, FADE_PALETTE_MEDIUM, Call_Back);
					}
					Clear_KeyBuffer();
					count.Set(TIMER_SECOND*3);
					while (count.Time()) {
						Call_Back();
					}
					Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);

					Play_Movie("CC2TEASE");
					Show_Mouse();

					ScenarioInit++;
					Theme.Play_Song (THEME_MAP1);
					ScenarioInit--;
					display = true;
					fade = true;
					selection = SEL_NONE;
					break;

				/*
				**	Exit to DOS.
				*/
				case SEL_EXIT:
#ifdef JAPANESE
	Hide_Mouse();
#endif
					Theme.Fade_Out();
					Fade_Palette_To(BlackPalette, FADE_PALETTE_SLOW, NULL);
#ifdef JAPANESE
	VisiblePage.Clear();
#endif
					return(false);

				/*
				**	Display the hall of fame.
				*/
				case SEL_FAME:
					break;

				case SEL_TIMEOUT:
					if (AllowAttract && RecordFile.Is_Available()) {
						PlaybackGame = true;
						if (RecordFile.Open(READ)) {
							Load_Recording_Values();
							process = false;
							Theme.Fade_Out();
						} else {
							PlaybackGame = false;
							selection = SEL_NONE;
						}
					} else {
						selection = SEL_NONE;
					}
					break;

				default:
					break;
			}
		}
	} else {

		/*
		** For Debug_Map (editor) mode, if JP option is on, set to load that scenario
		*/
		Scenario = 1;
		if (Special.IsJurassic && AreThingiesEnabled) {
			ScenPlayer = SCEN_PLAYER_JP;
			ScenDir = SCEN_DIR_EAST;
		}
	}
	CCDebugString ("C&C95 - About to start game initialisation.\n");
#ifdef FORCE_WINSOCK
	if (GameToPlay == GAME_INTERNET){
		CommProtocol = COMM_PROTOCOL_MULTI_E_COMP;
		if (!Special.IsFromWChat){
			FrameSendRate = 5;	//3;
		}
	}
#endif	//FORCE_WINSOCK
	/*
	**	Don't carry stray keystrokes into game.
	*/
	Kbd.Clear();

	/*
	** Get a pointer to the compiler's random number seed.
	**	the Get_EAX() must follow immediately after the srand(0) in order to save
	**	the address of the random seed.  (Currently not used.)
	*/
	srand(0);
	//RandSeedPtr = (long *)Get_EAX();	 // ST - 1/2/2019 5:26PM

	/*
	**	Initialize the random number Seed.  For multiplayer, this will have been done
	** in the connection dialogs.  For single-player games, AND if we're not playing
	** back a recording, init the Seed to a random value.
	*/
	if (GameToPlay == GAME_NORMAL && !PlaybackGame) {
		srand(timeGetTime());
		//randomize();
		Seed = rand();
	}

	/*
	** If user has specified a desired random number seed, use it for multiplayer games
	*/
	if (CustomSeed != 0) {
		Seed = CustomSeed;
	}

	/*
	** Save initialization values if we're recording this game.
	** This must be done after 'Seed' has been initialized.
	*/
	if (RecordGame) {
		if (RecordFile.Open(WRITE)) {
			Save_Recording_Values();
		} else {
			RecordGame = false;
		}
	}

	/*
	**	Initialize the random-number generator.
	*/
	//Seed = 1;

	srand(Seed);
	RandNumb = Seed;
	SimRandIndex = 0;
#if (0)
	DWORD	actual;
		HANDLE sfile = CreateFile("random.txt", GENERIC_WRITE, 0,
												NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (sfile != INVALID_HANDLE_VALUE){
		SetFilePointer (sfile, 0, NULL, FILE_END);

		int minimum;
		int maximum;
		char whatever[80];
		for ( int i=0 ; i<1000 ; i++){
			minimum = rand();
			maximum = rand();

			sprintf (whatever, "%04x\n", Random_Pick(minimum, maximum));
			WriteFile(sfile, whatever, strlen(whatever), &actual, NULL);
		}
		CloseHandle (sfile);
	}
#endif



	/*
	**	Load the scenario.  Specify variation 'A' for the editor; for the game,
	**	don't specify a variation, to make 'Set_Scenario_Name()' pick a random one.
	**	Skip this if we've already loaded a save-game.
	*/
	if (!gameloaded) {
		if (Debug_Map) {
			Set_Scenario_Name(ScenarioName, Scenario, ScenPlayer, ScenDir, SCEN_VAR_A);
		} else {
			Set_Scenario_Name(ScenarioName, Scenario, ScenPlayer, ScenDir);
		}

		/*
		** Start_Scenario() changes the palette; so, fade out & clear the screen
		** before calling it.
		*/
		Hide_Mouse();

		if (selection != SEL_START_NEW_GAME) {
			Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
			HiddenPage.Clear();
			VisiblePage.Clear();
		}
		Show_Mouse();

		Special.IsFromInstall = 0;
		CCDebugString ("C&C95 - Starting scenario.\n");
		if (!Start_Scenario(ScenarioName)) {
			return(false);
		}
		CCDebugString ("C&C95 - Scenario started OK.\n");
	}

	/*
	**	For multiplayer games, initialize the inter-player message system.
	**	Do this after loading the scenario, so the map's upper-left corner is
	**	properly set.
	*/
	CCDebugString ("C&C95 - Initialising message system.\n");
	int factor = (SeenBuff.Get_Width() == 320) ? 1 : 2;
	Messages.Init(Map.TacPixelX, Map.TacPixelY, 6, MAX_MESSAGE_LENGTH, 6*factor+1);

	/*
	**	Hide the SeenBuff; force the map to render one frame.  The caller can
	**	then fade the palette in.
	**	(If we loaded a game, this step will fade out the title screen.  If we
	**	started a scenario, Start_Scenario() will have played a couple of VQ
	**	movies, which will have cleared the screen to black already.)
	*/
	CCDebugString ("C&C95 - About to call Call_Back.\n");
	Call_Back();

	/*
	** This is desperately sad isnt it?
	*/
	Hide_Mouse();
	Hide_Mouse();
	Hide_Mouse();
	Hide_Mouse();
	WWMouse->Erase_Mouse(&HidPage, TRUE);


	Fade_Palette_To(BlackPalette, FADE_PALETTE_MEDIUM, Call_Back);
	HiddenPage.Clear();
	VisiblePage.Clear();
	Set_Logic_Page(SeenBuff);
	Map.Flag_To_Redraw();
	Call_Back();
	Map.Render();
	//Show_Mouse();

	/*
	** Special hack initialization of 'MPlayerMaxAhead' to accommodate the
	** compression protocol technology.
	*/
#ifdef FORCE_WINSOCK
	if (CommProtocol==COMM_PROTOCOL_MULTI_E_COMP && GameToPlay!=GAME_NORMAL) {
		if (!Special.IsFromWChat){
			MPlayerMaxAhead = FrameSendRate * 3;	//2;
		}else{
			MPlayerMaxAhead = WChatMaxAhead;
			FrameSendRate = WChatSendRate;
		}
	}
#endif	//FORCE_WINSOCK

	if (Debug_Map) {
		while (Get_Mouse_State() > 1) {
			Show_Mouse();
		}
	}

	return(true);
}


/***********************************************************************************************
 * Play_Intro -- plays the introduction & logo movies                                          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		for_real			if true, this function plays the "real" intro; otherwise, it plays		  *
 *							a delicious smorgasbord of visual delights, guaranteed to titillate		  *
 *							the ocular & auditory nerve pathways.												  *
 *							Well, it plays movies, anyway.														  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		none.																												  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		none.																												  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/06/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static void Play_Intro(bool for_real)
{
	return; // No game intro movies. - LLL

	bool playright = !Key_Down(KN_LCTRL) || !Key_Down(KN_RCTRL);
	static int _counter = -1;
	static char * _names[] = {
#ifdef DEMO
		"LOGO",

#else

		"INTRO2",
//#ifdef CHEAT_KEYS
		"GDIEND1",
		"GDIEND2",
		"GDIFINA",
		"GDIFINB",
		"AIRSTRK",
		"AKIRA",
		"BANNER",
		"BCANYON",
		"BKGROUND",
		"BOMBAWAY",
		"BOMBFLEE",
		"BURDET1",
		"BURDET2",
		"CC2TEASE",
		"CONSYARD",
		"DESFLEES",
		"DESKILL",
		"DESOLAT",
		"DESSWEEP",
		"FLAG",
		"FLYY",
		"FORESTKL",
		"GAMEOVER",
		"GDI1",
		"GDI10",
		"GDI11",
		"GDI12",
		"GDI13",
		"GDI14",
		"GDI15",
		"GDI2",
		"GDI3",
		"GDI3LOSE",
		"GDI4A",
		"GDI4B",
		"GDI5",
		"GDI6",
		"GDI7",
		"GDI8A",
		"GDI8B",
		"GDI9",
		"GDILOSE",
		"GUNBOAT",
		"HELLVALY",
		"INSITES",
		"KANEPRE",
		"LANDING",
		"LOGO",
		"NAPALM",
		"NITEJUMP",
		"NOD1",
		"NOD10A",
		"NOD10B",
		"NOD11",
		"NOD12",
		"NOD13",
		"NOD1PRE",
		"NOD2",
		"NOD3",
		"NOD4A",
		"NOD4B",
		"NOD5",
		"NOD6",
		"NOD7A",
		"NOD7B",
		"NOD8",
		"NOD9",
		"NODEND1",
		"NODEND2",
		"NODEND3",
		"NODEND4",
		"NODFINAL",
		"NODFLEES",
		"NODLOSE",
		"NODSWEEP",
		"NUKE",
		"OBEL",
		"PARATROP",
		"PINTLE",
		"PLANECRA",
		"PODIUM",
		"REFINT",
		"RETRO",
		"SABOTAGE",
		"SAMDIE",
		"SAMSITE",
		"SEIGE",
		"SETHPRE",
		"SPYCRASH",
		"STEALTH",
		"SUNDIAL",
		"TANKGO",
		"TANKKILL",
		"TBRINFO1",
		"TBRINFO2",
		"TBRINFO3",
		"TIBERFX",
		"TRTKIL_D",
		"TURTKILL",
		"VISOR",
//#endif
#endif
		NULL
	};

	Keyboard::Clear();
	if (for_real) {
		Hide_Mouse();
		Play_Movie("LOGO", THEME_NONE, false);
		Show_Mouse();
	} else {
		if (!Debug_Flag) {
			_counter = 0;
		} else {
			if (playright) _counter++;
			if (_counter == -1) _counter = 0;
		}
		Hide_Mouse();
		Play_Movie(_names[_counter], THEME_NONE);
		Show_Mouse();
		if (!_names[_counter]) {
			_counter = -1;
		}
	}
}


/***********************************************************************************************
 * Anim_Init -- Initialize the VQ animation control structure.                                 *
 *                                                                                             *
 *    VQ animations are controlled by a structure passed to the VQ player. This routine        *
 *    initializes the structure to values required by C&C.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only need to call this routine once at the beginning of the game.               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/20/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
//extern LPDIRECTSOUND			SoundObject;
//extern	LPDIRECTSOUNDBUFFER  PrimaryBufferPtr;
void Anim_Init(void)
{
//PG_TO_FIX
#if (0)
	/* Configure player with INI file */
	VQA_DefaultConfig(&AnimControl);
//	void const * font = Load_Font(FONT8);
//	AnimControl.EVAFont = (char *)font;
//	AnimControl.CapFont = (char *)font;

	AnimControl.DrawFlags = VQACFGF_TOPLEFT;
	AnimControl.DrawFlags |= VQACFGF_BUFFER;

	AnimControl.DrawFlags |= VQACFGF_NOSKIP;

	//AnimControl.X1 =0;
	//AnimControl.Y1 =0;
	AnimControl.FrameRate = -1;
	AnimControl.DrawRate = -1;

	AnimControl.DrawerCallback = VQ_Call_Back;
	AnimControl.ImageWidth = 320;
	AnimControl.ImageHeight = 200;
	AnimControl.Vmode = 0;
	AnimControl.ImageBuf = (unsigned char *)SysMemPage.Get_Offset();
	//AnimControl.VBIBit = VertBlank;
	//AnimControl.DrawFlags |= VQACFGF_TOPLEFT;
	AnimControl.OptionFlags |= VQAOPTF_CAPTIONS|VQAOPTF_EVA;

	if (SlowPalette) {
		AnimControl.OptionFlags |= VQAOPTF_SLOWPAL;
	}
#endif
//	AnimControl.AudioBuf = (unsigned char *)HidPage.Get_Buffer();
//	AnimControl.AudioBufSize = 32768U;
	//AnimControl.DigiCard = NewConfig.DigitCard;
	//AnimControl.HMIBufSize = 8192;
	//AnimControl.DigiHandle = Get_Digi_Handle();
	//AnimControl.Volume = 0x00FF;
	//AnimControl.AudioRate = 22050;
//	if (NewConfig.Speed) AnimControl.AudioRate = 11025;
	
	
	//PG_TO_FIX
#if (0)
	AnimControl.SoundObject = SoundObject;	//Get_Sound_Object();
	AnimControl.PrimaryBufferPtr = PrimaryBufferPtr;	//Get_Primart_Buffer();
#endif
	
	//if (!Debug_Quiet && Get_Digi_Handle() != -1) {
		//AnimControl.OptionFlags |= VQAOPTF_AUDIO;
	//}

#if (0)
	if (MonoClass::Is_Enabled()) {
		AnimControl.OptionFlags |= VQAOPTF_MONO;
	}
#endif
}


/***********************************************************************************************
 * Parse_Command_Line -- Parses the command line parameters.                                   *
 *                                                                                             *
 *    This routine should be called before the graphic mode is initialized. It examines the    *
 *    command line parameters and sets the appropriate globals. If there is an error, then     *
 *    it outputs a command summary and then returns false.                                     *
 *                                                                                             *
 * INPUT:   argc  -- The number of command line arguments.                                     *
 *                                                                                             *
 *          argv  -- Pointer to character string array that holds the individual arguments.    *
 *                                                                                             *
 * OUTPUT:  bool; Was the command line parsed successfully?                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Parse_Command_Line(int argc, char *argv[])
{
	/*
	**	Parse the command line and set globals to reflect the parameters
	**	passed in.
	*/
#ifdef DEMO
	Scenario = 3;
#else
	Scenario = 1;
#endif
	ScenPlayer = SCEN_PLAYER_GDI;
	ScenDir = SCEN_DIR_EAST;
	Whom = HOUSE_GOOD;
	Special.Init();

	Debug_Map = false;
//	Debug_Play_Map = false;
	Debug_Unshroud = false;

	for (int index = 1; index < argc; index++) {
		char * string;		// Pointer to argument.
		long code = 0;

		char arg_string[512];
		int str_len = strlen(argv[index]);
		char *src = argv[index];
		char *dest = arg_string; 
		for (int i=0 ; i<str_len ; i++) {
			if (*src == '\"') {
				src++;
			} else {
				*dest++ = *src++;
			}
		}
		*dest++ = 0;
		string = arg_string;
		strupr(string);

		//string = strupr(argv[index]);

		/*
		**	Print usage text only if requested.
		*/
		if (stricmp("/?", string) == 0 || stricmp("-?", string) == 0 || stricmp("-h", string) == 0 || stricmp("/h", string) == 0) {
			/*
			**	Unrecognized command line parameter... Display usage
			**	and then exit.
			*/
	#ifdef GERMAN
			puts("Command & Conquer (c) 1995,1996 Westwood Studios\r\n"
						"Parameter:\r\n"
//						"  -CD<Pfad> = Suchpfad fÅr Daten-Dateien festlegen.\r\n"
						"  -DESTNET  = Netzwerkkennung des Zielrechners festlegen\r\n"
						"              (Syntax: DESTNETxx.xx.xx.xx)\r\n"
						"  -SOCKET   = Kennung des Netzwerk-Sockets (0 - 16383)\n"
						"  -STEALTH  = Namen im Mehrspieler-Modus verstecken (\"Boss-Modus\")\r\n"
						"  -MESSAGES = Mitteilungen von au·erhalb des Spiels zulassen\r\n"
	//					"  -ELITE    = Fortgeschrittene KI und Gefechtstechniken.\r\n"
						"\r\n");
	#else
	#ifdef FRENCH
			puts("Command & Conquer (c) 1995, Westwood Studios\r\n"
						"Paramätres:\r\n"
//						"  -CD<chemin d'accäs> = Recherche des fichiers dans le\r\n"
//						"                        rÇpertoire indiquÇ.\r\n"
						"  -DESTNET  = SpÇcifier le numÇro de rÇseau du systäme de destination\r\n"
						"              (Syntaxe: DESTNETxx.xx.xx.xx)\r\n"
						"  -SOCKET   = ID poste rÇseau (0 Ö 16383)\r\n"
						"  -STEALTH  = Cacher les noms en mode multijoueurs (\"Mode Boss\")\r\n"
						"  -MESSAGES = Autorise les messages extÇrieurs Ö ce jeu.\r\n"
						"\r\n");
	#else
			puts("Command & Conquer (c) 1995, 1996 Westwood Studios\r\n"
						"Parameters:\r\n"
	#ifdef NEVER
						"  CHEAT     = Enable debug keys.\r\n"
						"  -EDITOR    = Enable scenario editor.\r\n"
	#endif
//						"  -CD<path> = Set search path for data files.\r\n"
						"  -DESTNET  = Specify Network Number of destination system\r\n"
						"              (Syntax: DESTNETxx.xx.xx.xx)\r\n"
						"  -SOCKET   = Network Socket ID (0 - 16383)\n"
						"  -STEALTH  = Hide multiplayer names (\"Boss mode\")\r\n"
						"  -MESSAGES = Allow messages from outside this game.\r\n"
						"  -o        = Enable compatability with version 1.07.\r\n"
#ifdef JAPANESE
						"  -ENGLISH  = Enable English keyboard compatibility.\r\n"
#endif
	//					"  -ELITE    = Advanced AI and combat characteristics.\r\n"
	#ifdef NEVER
						"  -O[options]= Special control options;\r\n"
						"     1 : Tiberium grows.\r\n"
						"     2 : Tiberium grows and spreads.\r\n"
						"     A : Aggressive player unit defense enabled.\r\n"
						"     B : Bargraphs always displayed.\r\n"
						"     C : Capture the flag mode.\r\n"
						"     E : Elite defense mode disable (attacker advantage).\r\n"
						"     D : Deploy reversal allowed for construction yard.\r\n"
						"     F : Fleeing from direct immediate threats is enabled.\r\n"
						"     H : Hussled recharge time.\r\n"
						"     G : Growth for Tiberium slowed in multiplay.\r\n"
						"     I : Inert weapons -- no damage occurs.\r\n"
						"     J : 7th grade sound effects.\r\n"
						"     M : Monochrome debug messages.\r\n"
						"     N : Name the civilians and buildings.\r\n"
						"     P : Path algorithm displayed as it works.\r\n"
						"     Q : Quiet mode (no sound).\r\n"
						"     R : Road pieces are not added to buildings.\r\n"
						"     T : Three point turns for wheeled vehicles.\r\n"
						"     U : U can target and burn trees.\r\n"
						"     V : Show target selection by opponent.\r\n"
						"     X : Make a recording of a multiplayer game.\r\n"
						"     Y : Play a recording of a multiplayer game.\r\n"
						"     Z : Disaster containment team.\r\n"
	#endif
						"\r\n");
	#endif
	#endif
			return(false);
		}


		bool processed = true;
		switch (Obfuscate(string)) {

			/*
			**	Signal that easy mode is active.
			*/
			case PARM_EASY:
				Special.IsEasy = true;
				Special.IsDifficult = false;
				break;

			/*
			**	Signal that hard mode is active.
			*/
			case PARM_HARD:
				Special.IsEasy = false;
				Special.IsDifficult = true;
				break;

#ifdef VIRGIN_CHEAT_KEYS
			case PARM_PLAYTEST:
				Debug_Playtest = true;
				break;
#endif

#ifdef PARM_CHEATERIK
			case PARM_CHEATERIK:
				Debug_Playtest = true;
				Debug_Flag = true;
				break;
#endif

#ifdef PARM_CHEATADAM
			case PARM_CHEATADAM:
				Debug_Playtest = true;
				Debug_Flag = true;
				break;
#endif

#ifdef PARM_CHEATMIKE
			case PARM_CHEATMIKE:
				Debug_Playtest = true;
				Debug_Flag = true;
				break;
#endif

#ifdef PARM_CHEATDAVID
			case PARM_CHEATDAVID:
				Debug_Playtest = true;
				Debug_Flag = true;
				break;
#endif

#ifdef PARM_CHEATPHIL
			case PARM_CHEATPHIL:
				Debug_Playtest = true;
				Debug_Flag = true;
				break;
#endif

#ifdef PARM_CHEATBILL
			case PARM_CHEATBILL:
				Debug_Playtest = true;
				Debug_Flag = true;
				break;
#endif

#ifdef PARM_CHEAT_STEVET

			case PARM_CHEAT_STEVET:
				Debug_Playtest = true;
				Debug_Flag = true;
				break;

#endif

#ifdef PARM_EDITORBILL
			case PARM_EDITORBILL:
				Debug_Map = true;
				Debug_Unshroud = true;
				Debug_Flag = true;
				break;
#endif

#ifdef PARM_EDITORERIK
			case PARM_EDITORERIK:
				Debug_Map = true;
				Debug_Unshroud = true;
				Debug_Flag = true;
				break;
#endif

			case PARM_SPECIAL:
				Special.IsJurassic = true;
				AreThingiesEnabled = true;
				break;

			/*
			** Special flag - is C&C being run from the install program?
			*/
			case PARM_INSTALL:
#ifndef DEMO
				Special.IsFromInstall = true;
#endif
				break;

			default:
				processed = false;
				break;
		}
		if (processed) continue;


#ifdef CHEAT_KEYS
		/*
		**	Scenario Editor Mode
		*/
		if (stricmp(string, "-CHECKMAP") == 0) {
			Debug_Check_Map = true;
			continue;
		}

#endif

		/*
		**	Older version override.
		*/
		if (stricmp(string, "-O") == 0 || stricmp(string, "-0") == 0) {
			IsV107 = true;
			continue;
		}

		/*
		**	File search path override.
		*/
		if (strstr(string, "-CD")) {
			CCFileClass::Set_Search_Drives(&string[3]);
			continue;
		}
#ifdef JAPANESE
		/*
		** Enable english-compatible keyboard
		*/
		if (!stricmp(string,"-ENGLISH")) {
			ForceEnglish = true;
			continue;
		}
#endif

		/*
		**	Specify destination connection for network play
		*/
		if (strstr(string, "-DESTNET")) {
			NetNumType net;
			NetNodeType node;

			/*
			** Scan the command-line string, pulling off each address piece
			*/
			int i = 0;
			char * p = strtok(string + 8,".");
			while (p) {
				int x;

				sscanf(p,"%x",&x);			// convert from hex string to int
				if (i < 4) {
					net[i] = (char)x;			// fill NetNum
				} else {
					node[i-4] = (char)x;		// fill NetNode
				}
				i++;
				p = strtok(NULL,".");
			}

			/*
			** If all the address components were successfully read, fill in the
			** BridgeNet with a broadcast address to the network across the bridge.
			*/
			if (i >= 4) {
				IsBridge = 1;
				memset(node, 0xff, 6);
				BridgeNet = IPXAddressClass(net, node);
			}
			continue;
		}

		/*
		**	Specify socket ID, as an offset from 0x4000.
		*/
		if (strstr(string, "-SOCKET")) {
			unsigned short socket;

			socket = (unsigned short)(atoi(string + strlen("SOCKET")));
			socket += 0x4000;
			if (socket >= 0x4000 && socket < 0x8000) {
				Ipx.Set_Socket (socket);
			}
			continue;
		}

		/*
		**	Set the Net Stealth option
		*/
		if (strstr(string, "-STEALTH")) {
			NetStealth = true;
			continue;
		}

		/*
		**	Set the Net Protection option
		*/
		if (strstr(string, "-MESSAGES")) {
			NetProtect = false;
			continue;
		}

		/*
		**	Allow "attract" mode
		*/
		if (strstr(string, "-ATTRACT")) {
			AllowAttract = true;
			continue;
		}

		/*
		** Set screen to 640x480 instead of 640x400
		*/
		if (strstr(string,"-480")){
			ScreenHeight = 480;
			continue;
		}

		/*
		** Check for spawn from WChat
		*/
		if (strstr(string,"-WCHAT")){
			SpawnedFromWChat = true;
		}

		/*
		** Allow use of MMX instructions
		*/
		if (strstr(string,"-MMX")){
			MMXAvailable = true;
			continue;
		}


#ifdef CHEAT_KEYS
		/*
		**	Allow solo net play
		*/
		if (stricmp(string, "-HANSOLO") == 0) {
			MPlayerSolo = true;
			continue;
		}

		/*
		**	Specify the random number seed (for debugging)
		*/
		if (strstr(string, "-SEED")) {
			CustomSeed = (unsigned short)(atoi(string + strlen("SEED")));
			continue;
		}
#endif

#ifdef NEVER
		/*
		**	Handle the prog init differently in this case.
		*/
		if (strstr(string, "-V")) {
			continue;
		}
#endif

#ifdef FIX_ME_LATER
		/*
		** look for passed-in video mode to default to
		*/
		if (strnicmp(string, "-V", strlen("-V")) == 0) {
			Set_Video_Mode(MCGA_MODE);	// do this to get around first_time variable...
			Set_Original_Video_Mode(atoi(string+2));
			continue;
		}
#endif //FIX_ME_LATER

		/*
		**	Special command line control parsing.
		*/
		if (strnicmp(string, "-X", strlen("-O")) == 0) {
			string += strlen("-X");
			while (*string) {
				char code = *string++;
				switch (toupper(code)) {

#ifdef ONHOLD
					/*
					**	Should human generated sound effects be used?
					*/
					case 'J':
						Special.IsJuvenile = true;
						break;
#endif

#ifdef  CHEAT_KEYS
					/*
					**	Monochrome debug screen enable.
					*/
					case 'M':
						Special.IsMonoEnabled = true;
						break;

					/*
					**	Inert weapons -- no units take damage.
					*/
					case 'I':
						Special.IsInert = true;
						break;
#endif

#ifdef CHEAT_KEYS
					/*
					**	Hussled recharge timer.
					*/
					case 'H':
						Special.IsSpeedBuild = true;
						break;

					/*
					**	Turn on super-record mode, which thrashes your disk terribly,
					** but is really really cool.  Well, sometimes it is, anyway.
					** At least, it can be.  Once in a while.
					** This flag tells the recording system to re-open the file for
					** each write, so the recording survives a crash.
					*/
					case 'S':
						SuperRecord = 1;
						break;

					/*
					**	"Record" a multi-player game
					*/
					case 'X':
						RecordGame = 1;
						break;

					/*
					**	"Play Back" a multi-player game
					*/
					case 'Y':
						PlaybackGame = 1;
						break;
#endif

#ifdef ONHOLD
					/*
					**	Bonus scenario enable.
					*/
					case 'Z':
						Special.IsJurassic = true;
						break;
#endif

					/*
					**	Quiet mode override control.
					*/
					case 'Q':
						Debug_Quiet = true;
						break;

#ifdef CHEAT_KEYS
					/*
					**	Target selection by human opponent (network/modem play) will
					**	be visible to the player?
					*/
					case 'V':
						Special.IsVisibleTarget = true;
						break;
#endif

					default:
#ifdef GERMAN
						puts("UngÅltiger Parameter.\n");
#else
#ifdef FRENCH
						puts("Commande d'option invalide.\n");
#else
						puts("Invalid option switch.\n");
#endif
#endif
						return(false);
				}

			}

			if (Special.IsMonoEnabled) {
				MonoClass::Enable();
			}
			continue;
		}
	}
	return(true);
}


#ifdef ONHOLD
/***********************************************************************************************
 * Parse_INI_File -- Parses CONQUER.INI for certain options                                    *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		none.																												  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		none.																												  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		none.																												  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/18/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void Parse_INI_File(void)
{
	char *buffer;							// INI staging buffer pointer.
	char buf[128];
	static char section[40];
	static char entry[40];
	static char name[40];
	int len;
	int i;

	/*
	** These arrays store the coded version of the names Geologic, Period, & Jurassic.
	** Decode them by subtracting 83.  For you curious types, the names look like:
	** ö∏¬ø¬∫º∂
	** £∏≈º¬∑
	** ù»≈¥∆∆º∂
	** If these INI entries aren't found, the IsJurassic flag does nothing.
	*/
	static char coded_section[] = {154,184,194,191,194,186,188,182,0};
	static char coded_entry[] = {163,184,197,188,194,183,0};
	static char coded_name[] = {157,200,197,180,198,198,188,182,0};

	/*------------------------------------------------------------------------
	Fetch working pointer to the INI staging buffer. Make sure that the buffer
	is cleared out before proceeding.
	------------------------------------------------------------------------*/
	buffer = (char *)_ShapeBuffer;
	memset(buffer, '\0', _ShapeBufferSize);

	/*------------------------------------------------------------------------
	Decode the desired section, entry, & name
	------------------------------------------------------------------------*/
	strcpy (section,coded_section);
	len = strlen(coded_section);
	for (i = 0; i < len; i++) {
		section[i] -= 83;
	}

	strcpy (entry,coded_entry);
	len = strlen(coded_entry);
	for (i = 0; i < len; i++) {
		entry[i] -= 83;
	}

	strcpy (name,coded_name);
	len = strlen(coded_name);
	for (i = 0; i < len; i++) {
		name[i] -= 83;
	}

	/*------------------------------------------------------------------------
	Create filename and read the file.
	------------------------------------------------------------------------*/
	CCFileClass file ("CONQUER.INI");
	if (!file.Is_Available()) {
		return;
	} else {
		file.Read(buffer, _ShapeBufferSize-1);
	}
	file.Close();

	WWGetPrivateProfileString(section, entry, "", buf, sizeof(buf), buffer);

	if (!stricmp (buf,name))
		AreThingiesEnabled = true;

	memset (section, 0, sizeof(section));
	memset (entry, 0, sizeof(entry));
	memset (name, 0, sizeof(name));
}
#endif


/***********************************************************************************************
 * Version_Number -- Determines the version number.                                            *
 *                                                                                             *
 *    This routine will determine the version number by analyzing the date and teim that the   *
 *    program was compiled and then generating a unique version number based on it. The        *
 *    version numbers are guaranteed to be larger for later dates.                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the version number.                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int Version_Number(void)
{
#ifdef OBSOLETE
	static bool initialized = false;
	static int version;
	static char * date = __DATE__;
	static char * time = __TIME__;
	static char const * months = "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC";

	if (!initialized) {
		char * ptr;
		char * tok;

		/*
		**	Fetch the month and place in the first two digit positions.
		*/
		strupr(date);
		tok = strtok(date, " ");
		ptr = strstr(months, tok);
		if (ptr) {
			version = (((ptr - months) / 3)+1) * 10000;
		}

		/*
		**	Fetch the date and place that in the next two digit positions.
		*/
		tok = strtok(NULL, " ");
		if (tok) {
			version += atoi(tok) * 100;
		}

		/*
		**	Fetch the time and place that in the last two digit positions.
		*/
		tok = strtok(time, ": ");
		if (tok) {
			version += atoi(tok);
		}


		/*
		**	Fetch the virgin text file (if present).
		*/
		RawFileClass file("VERSION.TXT");
		if (file.Is_Available()) {
			file.Read(VersionText, sizeof(VersionText));
			VersionText[sizeof(VersionText)-1] = '\0';
			while (VersionText[sizeof(VersionText)-1] == '\r') {
				VersionText[sizeof(VersionText)-1] = '\0';
			}
		} else {
			VersionText[0] = '\0';
		}

		initialized = true;
	}
	return(version);
#endif

#ifdef WIN32

#if (FRENCH)
		sprintf(VersionText, ".02");            // Win95 french version number
#endif	//FRENCH

#if (GERMAN)
		sprintf(VersionText, ".01");            // Win95 german version number
#endif	//GERMAN

#if (JAPANESE)
		sprintf(VersionText, ".01");            // Win95 german version number
#endif	//GERMAN

#if !(FRENCH | GERMAN | JAPANESE)
		sprintf(VersionText, ".07");            // Win95 USA version number
#endif	//FRENCH | GERMAN

		RawFileClass file("VERSION.TXT");
		char version [16];
		memset (version, 0, sizeof (version));
		if (file.Is_Available()){
			file.Read (version, sizeof (version));
		}
		strncat (VersionText, version, sizeof (VersionText) - strlen (VersionText) - 1);

#if (FRENCH)
		return (1);            						// Win95 french version number
#endif	//FRENCH

#if (GERMAN)
		return (1);										// Win95 german version number
#endif	//GERMAN

#if (JAPANESE)
		return (1);										// Win95 german version number
#endif	//GERMAN

#if !(FRENCH | GERMAN | JAPANESE)
		return (1);										// Win95 USA version number
#endif	//FRENCH | GERMAN

#else


#ifdef PATCH

	#ifdef DEMO
		sprintf(VersionText, " 1.0a");		// Demo version.
	#else
		strcpy(VersionText, ".34 ");
	#endif
	return(1);

#else

	#ifdef DEMO
		sprintf(VersionText, " 1.0a");		// Demo version.
	#else
	//	sprintf(VersionText, ".%02dp", 13);			// Patch version.
		sprintf(VersionText, ".%02d", 14);			// Master version.
	#endif
	return(1);
#endif

#endif	//WIN32
}


/***************************************************************************
 * Save_Recording_Values -- Saves recording values to a recording file     *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/15/1995 BRR : Created.                                             *
 *=========================================================================*/
void Save_Recording_Values(void)
{
	RecordFile.Write(&GameToPlay, sizeof(GameToPlay));
	RecordFile.Write(&ModemGameToPlay, sizeof(ModemGameToPlay));
	RecordFile.Write(&BuildLevel, sizeof(BuildLevel));
	RecordFile.Write(MPlayerName, sizeof(MPlayerName));
	RecordFile.Write(&MPlayerPrefColor, sizeof(MPlayerPrefColor));
	RecordFile.Write(&MPlayerColorIdx, sizeof(MPlayerColorIdx));
	RecordFile.Write(&MPlayerHouse, sizeof(MPlayerHouse));
	RecordFile.Write(&MPlayerLocalID, sizeof(MPlayerLocalID));
	RecordFile.Write(&MPlayerCount, sizeof(MPlayerCount));
	RecordFile.Write(&MPlayerBases, sizeof(MPlayerBases));
	RecordFile.Write(&MPlayerCredits, sizeof(MPlayerCredits));
	RecordFile.Write(&MPlayerTiberium, sizeof(MPlayerTiberium));
	RecordFile.Write(&MPlayerGoodies, sizeof(MPlayerGoodies));
	RecordFile.Write(&MPlayerGhosts, sizeof(MPlayerGhosts));
	RecordFile.Write(&MPlayerUnitCount, sizeof(MPlayerUnitCount));
	RecordFile.Write(MPlayerID, sizeof(MPlayerID));
	RecordFile.Write(MPlayerHouses, sizeof(MPlayerHouses));
	RecordFile.Write(&Seed, sizeof(Seed));
	RecordFile.Write(&Scenario, sizeof(Scenario));
	RecordFile.Write(&ScenPlayer, sizeof(ScenPlayer));
	RecordFile.Write(&ScenDir, sizeof(ScenDir));
	RecordFile.Write(&Whom, sizeof(Whom));
	RecordFile.Write(&Special, sizeof(SpecialClass));
	RecordFile.Write(&Options, sizeof(GameOptionsClass));
	RecordFile.Write(&FrameSendRate, sizeof(FrameSendRate));
	RecordFile.Write(&CommProtocol, sizeof(CommProtocol));

	if (SuperRecord) {
		RecordFile.Close();
	}
}


/***************************************************************************
 * Load_Recording_Values -- Loads recording values from recording file     *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/15/1995 BRR : Created.                                             *
 *=========================================================================*/
void Load_Recording_Values(void)
{
	Read_MultiPlayer_Settings();

	RecordFile.Read(&GameToPlay, sizeof(GameToPlay));
	RecordFile.Read(&ModemGameToPlay, sizeof(ModemGameToPlay));
	RecordFile.Read(&BuildLevel, sizeof(BuildLevel));
	RecordFile.Read(MPlayerName, sizeof(MPlayerName));
	RecordFile.Read(&MPlayerPrefColor, sizeof(MPlayerPrefColor));
	RecordFile.Read(&MPlayerColorIdx, sizeof(MPlayerColorIdx));
	RecordFile.Read(&MPlayerHouse, sizeof(MPlayerHouse));
	RecordFile.Read(&MPlayerLocalID, sizeof(MPlayerLocalID));
	RecordFile.Read(&MPlayerCount, sizeof(MPlayerCount));
	RecordFile.Read(&MPlayerBases, sizeof(MPlayerBases));
	RecordFile.Read(&MPlayerCredits, sizeof(MPlayerCredits));
	RecordFile.Read(&MPlayerTiberium, sizeof(MPlayerTiberium));
	RecordFile.Read(&MPlayerGoodies, sizeof(MPlayerGoodies));
	RecordFile.Read(&MPlayerGhosts, sizeof(MPlayerGhosts));
	RecordFile.Read(&MPlayerUnitCount, sizeof(MPlayerUnitCount));
	RecordFile.Read(MPlayerID, sizeof(MPlayerID));
	RecordFile.Read(MPlayerHouses, sizeof(MPlayerHouses));
	RecordFile.Read(&Seed, sizeof(Seed));
	RecordFile.Read(&Scenario, sizeof(Scenario));
	RecordFile.Read(&ScenPlayer, sizeof(ScenPlayer));
	RecordFile.Read(&ScenDir, sizeof(ScenDir));
	RecordFile.Read(&Whom, sizeof(Whom));
	RecordFile.Read(&Special, sizeof(SpecialClass));
	RecordFile.Read(&Options, sizeof(GameOptionsClass));
	RecordFile.Read(&FrameSendRate, sizeof(FrameSendRate));
	RecordFile.Read(&CommProtocol, sizeof(CommProtocol));
}


/***********************************************************************************************
 * Obfuscate -- Sufficiently transform parameter to thwart casual hackers.                     *
 *                                                                                             *
 *    This routine borrows from CRC and PGP technology to sufficiently alter the parameter     *
 *    in order to make it difficult to reverse engineer the key phrase. This is designed to    *
 *    be used for hidden game options that will be released at a later time over Westwood's    *
 *    Web page or through magazine hint articles.                                              *
 *                                                                                             *
 *    Since this is a one way transformation, it becomes much more difficult to reverse        *
 *    engineer the pass phrase even if the resultant pass code is known. This has an added     *
 *    benefit of making this algorithm immune to traditional cyrptographic attacks.            *
 *                                                                                             *
 *    The largest strength of this transformation algorithm lies in the restriction on the     *
 *    source vector being legal ASCII uppercase characters. This restriction alone makes even  *
 *    a simple CRC transformation practically impossible to reverse engineer. This algorithm   *
 *    uses far more than a simple CRC transformation to achieve added strength from advanced   *
 *    attack methods.                                                                          *
 *                                                                                             *
 * INPUT:   string   -- Pointer to the key phrase that will be transformed into a code.        *
 *                                                                                             *
 * OUTPUT:  Returns with the code that the key phrase is translated into.                      *
 *                                                                                             *
 * WARNINGS:   A zero length pass phrase results in a 0x00000000 result code.                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
long Obfuscate(char const * string)
{
	char buffer[1024];

	if (!string) return(0);
	memset(buffer, '\xA5', sizeof(buffer));

	/*
	**	Copy key phrase into a working buffer. This hides any transformation done
	**	to the string.
	*/
	strncpy(buffer, string, sizeof(buffer));
	buffer[sizeof(buffer)-1] = '\0';
	int length = strlen(buffer);

	/*
	**	Only upper case letters are significant.
	*/
	strupr(buffer);

	/*
	**	Ensure that only visible ASCII characters compose the key phrase. This
	**	discourages the direct forced illegal character input method of attack.
	*/
	int index;
	for (index = 0; index < length; index++) {
		if (!isgraph(buffer[index])) {
			buffer[index] = 'A' + (index%26);
		}
	}

	/*
	**	Increase the strength of even short pass phrases by extending the
	**	length to be at least a minimum number of characters. This helps prevent
	**	a weak pass phrase from compromising the obfuscation process. This
	**	process also forces the key phrase to be an even multiple of four.
	**	This is necessary to support the cypher process that occurs later.
	*/
	if (length < 16 || (length & 0x03)) {
		int maxlen = 16;
		if (((length+3) & 0x00FC) > maxlen) {
			maxlen = ((length+3) & 0x00FC);
		}
		for (index = length; index < maxlen; index++) {
			buffer[index] = 'A' + ((('?' ^ buffer[index-length]) + index) % 26);
		}
		length = index;
		buffer[length] = '\0';
	}

	/*
	**	Transform the buffer into a number. This transformation is character
	**	order dependant.
	*/
	long code = Calculate_CRC(buffer, length);

	/*
	**	Record a copy of this initial transformation to be used in a later
	**	self referential transformation.
	*/
	long copy = code;

	/*
	**	Reverse the character string and combine with the previous transformation.
	**	This doubles the workload of trying to reverse engineer the CRC calculation.
	*/
	strrev(buffer);
	code ^= Calculate_CRC(buffer, length);

	/*
	**	Perform a self referential transformation. This makes a reverse engineering
	**	by using a cause and effect attack more difficult.
	*/
	code = code ^ copy;

	/*
	**	Unroll and combine the code value into the pass phrase and then perform
	**	another self referential transformation. Although this is a trivial cypher
	**	process, it gives the sophisticated hacker false hope since the strong
	**	cypher process occurs later.
	*/
	strrev(buffer);		// Restore original string order.
	for (index = 0; index < length; index++) {
		code ^= (unsigned char)buffer[index];
		unsigned char temp = (unsigned char)code;
		buffer[index] ^= temp;
		code >>= 8;
		code |= (((long)temp)<<24);
	}

	/*
	**	Introduce loss into the vector. This strengthens the key against traditional
	**	cryptographic attack engines. Since this also weakens the key against
	**	unconventional attacks, the loss is limited to less than 10%.
	*/
	for (index = 0; index < length; index++) {
		static unsigned char _lossbits[] = {0x00,0x08,0x00,0x20,0x00,0x04,0x10,0x00};
		static unsigned char _addbits[] = {0x10,0x00,0x00,0x80,0x40,0x00,0x00,0x04};

		buffer[index] |= _addbits[index % (sizeof(_addbits)/sizeof(_addbits[0]))];
		buffer[index] &= ~_lossbits[index % (sizeof(_lossbits)/sizeof(_lossbits[0]))];
	}

	/*
	**	Perform a general cypher transformation on the vector
	**	and use the vector itself as the cypher key. This is a variation on the
	**	cypher process used in PGP. It is a very strong cypher process with no known
	**	weaknesses. However, in this case, the cypher key is the vector itself and this
	**	opens up a weakness against attacks that have access to this transformation
	**	algorithm. The sheer workload of reversing this transformation should be enough
	**	to discourage even the most determined hackers.
	*/
	for (index = 0; index < length; index += 4) {
		short key1 = buffer[index];
		short key2 = buffer[index+1];
		short key3 = buffer[index+2];
		short key4 = buffer[index+3];
		short val1 = key1;
		short val2 = key2;
		short val3 = key3;
		short val4 = key4;

		val1 *= key1;
		val2 += key2;
		val3 += key3;
		val4 *= key4;

		short s3 = val3;
		val3 ^= val1;
		val3 *= key1;
		short s2 = val2;
		val2 ^= val4;
		val2 += val3;
		val2 *= key3;
		val3 += val2;

		val1 ^= val2;
		val4 ^= val3;

		val2 ^= s3;
		val3 ^= s2;

		buffer[index] = val1;
		buffer[index+1] = val2;
		buffer[index+2] = val3;
		buffer[index+3] = val4;
	}

	/*
	**	Convert this final vector into a cypher key code to be
	**	returned by this routine.
	*/
	code = Calculate_CRC(buffer, length);

	/*
	**	Return the final code value.
	*/
	return(code);
}