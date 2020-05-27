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

/*************************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S                  **
 *************************************************************************************
 *                                                                                   *
 *                 Project Name : Command & Conquer - Red Alert                      *
 *                                                                                   *
 *                    File Name : INTERNET.CPP                                       *
 *                                                                                   *
 *                   Programmer : Steve Tall                                         *
 *                                                                                   *
 *                   Start Date : March 11th, 1996                                   *
 *                                                                                   *
 *                  Last Update : August 5th, 1996 [ST]                              *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Overview:                                                                         *
 *                                                                                   *
 *  Miscellaneous junk related to H2H internet connection.                           *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Functions:                                                             	 			*
 *  Check_From_WChat -- Interprets start game packet from WChat           	 			*
 *  Read_Game_Options -- Read the game setup options from the wchat packet	 			*
 *  Is_User_WChat_Registered -- retrieve the users wchat entry from registry 			*
 *  Spawn_WChat -- spawns or switches focus to wchat                         			*
 *  Spawn_Registration_App -- spawns the C&C/Planet westwood registration app			*
 *  Do_The_Internet_Menu_Thang -- Handle case where user clicks on 'Internet' button *
 *                                                                         				*
 *                                                                         				*
 *                                                                         				*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "function.h"
#include "tcpip.h"
#include "ccdde.h"



/***************************************************************************
** Internet specific globals
*/
char PlanetWestwoodHandle[] = {"Handle"};				//Planet WW user name
char PlanetWestwoodPassword[] = {"Password"};       //Planet WW password
char PlanetWestwoodIPAddress[IP_ADDRESS_MAX] = {"206.154.108.87"};	//IP of server or other player
long PlanetWestwoodPortNumber = 1234;					//Port number to send to
bool PlanetWestwoodIsHost = false;                  //Flag true if player has control of game options
unsigned long PlanetWestwoodGameID;                 //Game ID
unsigned long PlanetWestwoodStartTime;              //Time that game was started
HWND	WChatHWND = 0;                               //Handle to Wchat window.
bool UseVirtualSubnetServer;
int  InternetMaxPlayers;
int WChatMaxAhead;
int WChatSendRate;


int Read_Game_Options(void);

extern bool SpawnedFromWChat;






/***********************************************************************************************
 * Check_From_WChat -- This function reads in C&CSPAWN.INI and interprets it                   *
 *                     C&CSPAWN.INI is now sent to us by WCHAT via DDE                         *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Name of C&CSPAWN.INI file. If NULL then get file from DDE Server                  *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/9/96 1:44PM ST : Created                                                               *
 *=============================================================================================*/
void Check_From_WChat(char *wchat_name)
{
#ifndef DEMO

	char	default_string[] = {"Error"};
	char	key_string[256];
	char *ini_file;
	RawFileClass wchat_file;

	/*
	** Get a pointer to C&CSPAWN.INI either by reading it from disk or getting it from
	** the DDE server.
	*/
	if (wchat_name){
		ini_file = new char [8192];
	}else{
		ini_file = DDEServer.Get_MPlayer_Game_Info();
#if (0)
		/*
		** Save it to disk as well so I can see it
		*/
		RawFileClass anotherfile ("FROMCHAT.TXT");
		anotherfile.Write(ini_file, DDEServer.Get_MPlayer_Game_Info_Length());
#endif	//(0)
	}

	if (wchat_name){
		wchat_file.Set_Name(wchat_name);
	}

	if (!wchat_name || wchat_file.Is_Available()){

		/*
		** Read the ini file from disk if we founf it there
		*/
		if (wchat_name){
			wchat_file.Read(ini_file, wchat_file.Size());
		}

		/*
		** Get the IP address
		*/
		key_string[0] = 0;

		WWGetPrivateProfileString("Internet",
										"Address",
										default_string,
										key_string,
										sizeof(key_string),
										ini_file);


		if (!strcmp (key_string, default_string)) {
			if (wchat_name) delete ini_file;
			return;
		}
		strcpy (PlanetWestwoodIPAddress, key_string);



		/*
		** Get the port number
		*/
		key_string[0] = 0;

		WWGetPrivateProfileString("Internet",
										"Port",
										default_string,
										key_string,
										sizeof(key_string),
										ini_file);


		if (!strcmp (key_string, default_string)) {
			if (wchat_name) delete ini_file;
			return;
		}

		PlanetWestwoodPortNumber = atol(key_string);


		/*
		** Get host or client
		*/
		key_string[0] = 0;

		WWGetPrivateProfileString("Internet",
										"Host",
										default_string,
										key_string,
										sizeof(key_string),
										ini_file);


		if (!strcmp (key_string, default_string)) {
			if (wchat_name) delete ini_file;
			return;
		}

		if (strchr (key_string, '1')){
			PlanetWestwoodIsHost = true;
		}else{
			PlanetWestwoodIsHost = false;
		}

		UseVirtualSubnetServer = WWGetPrivateProfileInt("Internet", "UseVSS", 0, ini_file);

		Special.IsFromWChat = true;
	}

	if (wchat_name) delete ini_file;

#else	//DEMO

	wchat_name = wchat_name;

#endif	//DEMO

}

//EventClass Wibble;


/***************************************************************************
 * Read_Game_Options -- reads multiplayer game options from disk           *
 *                                                                         *
 * This routine is used for multiplayer games which read the game options	*
 * from disk, rather than through a connection dialog.							*
 *                                                                         *
 * INPUT:                                                                  *
 *		name of C&CSPAWN.INI file. Null if data should be got from DDE server*                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                          \                                     *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/11/1996 BRR : Created.                                             *
 *=========================================================================*/
int Read_Game_Options(char *name)
{
	char *buffer;

	char filename[256] = {"INVALID.123"};

	if (name){
		strcpy (filename, name);
	}

	/*------------------------------------------------------------------------
	Create filename and read the file.
	------------------------------------------------------------------------*/
	CCFileClass file (filename);

	if (name && !file.Is_Available()) {
		return(0);
	} else {
		if (name){
			buffer = new char [8192];							// INI staging buffer pointer.
			memset(buffer, '\0', 8192);
			file.Read(buffer, 8192-1);
			file.Close();
		}else{
			buffer = DDEServer.Get_MPlayer_Game_Info();
		}
	}

	/*------------------------------------------------------------------------
	Get the player's name
	------------------------------------------------------------------------*/
	WWGetPrivateProfileString("Options", "Handle", "Noname", MPlayerName,
		sizeof(MPlayerName), buffer);
	strcpy(MPlayerGameName, MPlayerName);
	MPlayerColorIdx = WWGetPrivateProfileInt("Options", "Color", 0, buffer);
	MPlayerPrefColor = MPlayerColorIdx;
	MPlayerHouse = (HousesType)WWGetPrivateProfileInt("Options", "Side",
		HOUSE_GOOD, buffer);

	MPlayerCredits = WWGetPrivateProfileInt("Options", "Credits", 0, buffer);
	MPlayerBases = WWGetPrivateProfileInt("Options", "Bases", 0, buffer);
	MPlayerTiberium = WWGetPrivateProfileInt("Options", "Tiberium", 0, buffer);
	MPlayerGoodies = WWGetPrivateProfileInt("Options", "Crates", 0, buffer);
	MPlayerGhosts = WWGetPrivateProfileInt("Options", "AI", 0, buffer);
	BuildLevel = WWGetPrivateProfileInt("Options", "BuildLevel", 0, buffer);
	MPlayerUnitCount = WWGetPrivateProfileInt("Options", "UnitCount", 0, buffer);
	Seed = WWGetPrivateProfileInt("Options", "Seed", 0, buffer);
	Special.IsCaptureTheFlag = WWGetPrivateProfileInt("Options", "CaptureTheFlag", 0, buffer);
	PlanetWestwoodGameID = WWGetPrivateProfileInt("Internet", "GameID", 0, buffer);
	PlanetWestwoodStartTime = WWGetPrivateProfileInt ("Internet", "StartTime", 0, buffer);
	WChatHWND = (HWND) WWGetPrivateProfileInt("Internet", "HWND", (int)FindWindow("OWL_Window", "Westwood Chat"), buffer);

	InternetMaxPlayers = WWGetPrivateProfileInt("Internet", "MaxPlayers", 2, buffer);


	if (MPlayerTiberium) {
		Special.IsTGrowth = 1;
		Special.IsTSpread = 1;
	} else {
		Special.IsTGrowth = 0;
		Special.IsTSpread = 0;
	}
	ScenarioIdx = WWGetPrivateProfileInt("Options", "Scenario", 0, buffer);
	Scenario = ScenarioIdx;	//MPlayerFilenum[ScenarioIdx];

	Options.GameSpeed = 0;

	MPlayerLocalID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);

	MPlayerMaxAhead = WChatMaxAhead = WWGetPrivateProfileInt("Timing", "MaxAhead", 9, buffer);
	FrameSendRate = WChatSendRate = WWGetPrivateProfileInt("Timing", "SendRate", 3, buffer);

	if (name) delete buffer;
	return (1);

}




/***********************************************************************************************
 * Get_Registry_Sub_Key -- search a registry key for a sub-key                                 *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    handle of key to search                                                           *
 *           text to search for                                                                *
 *           true if old key should be closed when new key opened                              *
 *                                                                                             *
 * OUTPUT:   handle to the key we found or 0                                                   *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    1/12/96 2:11PM ST : Created                                                              *
 *=============================================================================================*/

extern HKEY Get_Registry_Sub_Key (HKEY base_key, char *search_key, BOOL close);



void Just_Path(char *path, char *destpath)
{
	char *terminator = NULL;		//He'll be back.

	strcpy  (destpath, path);
	terminator = strrchr (destpath, '\\');
	if (terminator){
		*terminator = 0;
	}
}





/***********************************************************************************************
 * Is_User_WChat_Registered -- retrieve the users wchat entry from the registry                *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:  Nothing                                                                             *
 *                                                                                             *
 * OUTPUT:   TRUE if users wchat entry was found in the registry                               *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    1/12/96 2:13PM ST : Created                                                              *
 *=============================================================================================*/
bool Is_User_WChat_Registered(char *buffer, int buffer_len)
{
	buffer;
	buffer_len;
	return false;

#if (0)	//ST - 1/2/2019 5:54PM
	
	HKEY	key;
	char	user_handle[256];
	DWORD	user_handle_size = sizeof (user_handle);
	char	user_pword[256];
	DWORD	user_pword_size = sizeof (user_pword);


	/*
	** Check HKEY_CLASSES_ROOT first. Old versions of Wchat register there
	*/
	key = Get_Registry_Sub_Key (HKEY_CLASSES_ROOT, "Wchat", FALSE);

	if (key){
		key = Get_Registry_Sub_Key (key, "Nick1", TRUE);
		if (key){

			if (RegQueryValue(key, "Nick", user_handle, (long*)&user_handle_size) == ERROR_SUCCESS){

				if (RegQueryValue(key, "Pass", user_pword, (long*)&user_pword_size) == ERROR_SUCCESS){

					/*
					** If the first char of the users name is non-numberic and there is a password
					** then return success
					*/
					if ((user_handle[0] < '0' || user_handle[0] > '9') && user_pword[0]){
						RegCloseKey( key );
						return (TRUE);
					}
				}
			}
		}

		RegCloseKey ( key );
	}



	/*
	** Check HKEY_LOCAL_MACKINE/Software
	*/
	user_handle_size = sizeof (user_handle);

	key = Get_Registry_Sub_Key (HKEY_LOCAL_MACHINE, "SOFTWARE", FALSE);
	if (!key) return (FALSE);

	key = Get_Registry_Sub_Key (key, "Westwood", TRUE);
	if (!key) return (FALSE);

	key = Get_Registry_Sub_Key (key, "InetReg", TRUE);
	if (!key) return (FALSE);

	//key = Get_Registry_Sub_Key (key, "UserName", TRUE);
	//if (!key) return (FALSE);

	//key = Get_Registry_Sub_Key (key, "Nick", TRUE);
	//if (!key) return (FALSE);

	if (RegQueryValueEx(key, "UserName", NULL, NULL, (unsigned char*)user_handle, &user_handle_size) != ERROR_SUCCESS){
		RegCloseKey(key);
		return (FALSE);
	}

	RegCloseKey(key);
	memcpy (buffer, user_handle, min((unsigned)buffer_len, user_handle_size));

	/*
	** If the first char of the users name is non-numeric then return success
	*/
	if (user_handle[0] < '0' || user_handle[0] > '9'){
		return (TRUE);
	}else{
		return (FALSE);
	}
#endif
}



/***********************************************************************************************
 * Spawn_WChat -- spawns or switches focus to wchat                                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    can launch. If set then we are allowed to launch WChat if not already running     *
 *                                                                                             *
 * OUTPUT:   True if wchat was spawned                                                         *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/8/96 12:33PM ST : Created                                                              *
 *=============================================================================================*/
bool Poke_WChat(void);
bool Spawn_WChat(bool can_launch)
{
	can_launch;
	return false;

#if (0)
	CCDebugString ("C&C95 - In Spawn_WChat.\n");
	char packet[10] = {"Hello"};
	HWND chat_window = NULL;

	/*
	** See if WChat is already running...
	*/
	if (WChatHWND && IsWindow (WChatHWND) ){
		chat_window = WChatHWND;
	}else{
		chat_window = FindWindow ( "OWL_Window", "Westwood Chat" );
	}

	if (chat_window){
	/*
	** WChat is already running. Minimize myself then try to give it focus.
	*/
		Set_Palette(BlackPalette);
		VisiblePage.Clear();
		ShowWindow (MainWindow, SW_MINIMIZE);
		/*
		** Give windoze a couple of secs to sort itself out.
		*/
		CountDownTimerClass wibble_timer;
		wibble_timer.Set ( 60 * 3, true);

		while (wibble_timer.Time()){
			/*
			** Call our message loop to make sure we get all the messages that are sent to us
			** when we minimise.
			*/
			Keyboard::Check();
		}

		/*
		** Send chat a tickle message so it knows to send the game stats to the server.
		*/
		if (GameStatisticsPacketSent && !PlanetWestwoodIsHost) {
			Send_Data_To_DDE_Server (packet, strlen(packet), DDEServerClass::DDE_TICKLE);
		}
		/*
		** Give the focus to WChat
		*/
		SetForegroundWindow ( chat_window );
		ShowWindow ( chat_window, SW_RESTORE );
		return(true);
	}

	/*
	** Fail if we aren't allowed to launch wchat and we couldnt find its window.
	*/
	if (!can_launch) return (false);

	/*
	** Find where WChat was installed to
	*/

	HKEY	key;
	char	wchat_loc[256];
	DWORD	wchat_loc_size = 256;

	key = Get_Registry_Sub_Key (HKEY_LOCAL_MACHINE, "SOFTWARE", FALSE);
	if (!key) return (FALSE);

	key = Get_Registry_Sub_Key (key, "Westwood", TRUE);
	if (!key) return (FALSE);

	key = Get_Registry_Sub_Key (key, "WChat", TRUE);
	if (!key) return (FALSE);

	//key = Get_Registry_Sub_Key (key, "UserName", TRUE);
	//if (!key) return (FALSE);

	//key = Get_Registry_Sub_Key (key, "Nick", TRUE);
	//if (!key) return (FALSE);

	if (RegQueryValueEx(key, "InstallPath", NULL, NULL, (unsigned char*)wchat_loc, &wchat_loc_size) != ERROR_SUCCESS){
		RegCloseKey(key);
		return (FALSE);
	}

	RegCloseKey(key);

	PROCESS_INFORMATION process_info;
	STARTUPINFO start_info;
	memset ((void*)&start_info, 0, sizeof(start_info));
	start_info.cb = sizeof(start_info);
	char justpath [256];
	Just_Path(wchat_loc, justpath);

	/*
	** We found WChat in the registry. Minimize myself then try to spawn it.
	*/
	Set_Palette(BlackPalette);
	VisiblePage.Clear();
	ShowWindow (MainWindow, SW_MINIMIZE);
	/*
	** Give windoze a couple of secs to sort itself out.
	*/
	CountDownTimerClass wibble_timer;
	wibble_timer.Set ( 60 * 3, true);

	while (wibble_timer.Time()){
		/*
		** Call our message loop to make sure we get all the messages that are sent to us
		** when we minimise.
		*/
		Keyboard::Check();
	}
	bool success = CreateProcess (wchat_loc, NULL, NULL, NULL, false, 0, NULL, justpath, &start_info, &process_info);

	if (success){
		return (true);
	}else{
		ShowWindow (MainWindow, SW_RESTORE);
		while ( Keyboard::Check() ) {};
		return (false);
	}
#endif

}




/***********************************************************************************************
 * Spawn_Registration_App -- spawns the C&C/Planet westwood registration app                   *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   True if app was spawned                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/8/96 12:33PM ST : Created                                                              *
 *=============================================================================================*/
bool Spawn_Registration_App(void)
{
	return false;
#if (0)	//ST - 1/2/2019 5:53PM
	/*
	** Find where inetreg was installed to
	*/

	HKEY	key;
	char	inetreg_loc[256];
	DWORD	inetreg_loc_size = 256;

	key = Get_Registry_Sub_Key (HKEY_LOCAL_MACHINE, "SOFTWARE", FALSE);
	if (!key) return (FALSE);

	key = Get_Registry_Sub_Key (key, "Westwood", TRUE);
	if (!key) return (FALSE);

	key = Get_Registry_Sub_Key (key, "InetReg", TRUE);
	if (!key) return (FALSE);

	if (RegQueryValueEx(key, "InstallPath", NULL, NULL, (unsigned char*)inetreg_loc, &inetreg_loc_size) != ERROR_SUCCESS){
		RegCloseKey(key);
		return (FALSE);
	}

	RegCloseKey(key);

	PROCESS_INFORMATION process_info;
	STARTUPINFO start_info;
	char justpath 	[256];
	memset ((void*)&start_info, 0, sizeof(start_info));
	start_info.cb = sizeof(start_info);
	Just_Path(inetreg_loc, justpath);

	BOOL success = CreateProcess (inetreg_loc, NULL, NULL, NULL, false, 0, NULL, justpath, &start_info, &process_info);
	if (success){
		//WaitForSingleObject (process_info.hProcess, 1000*10000);
		//SetForegroundWindow ( MainWindow );
		//ShowWindow ( MainWindow, SW_RESTORE );
	}
	return (success);
#endif

}





/***********************************************************************************************
 * Do_The_Internet_Menu_Thang -- Handle case where user clicks on 'Internet' button            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 8:30PM ST : Created                                                               *
 *=============================================================================================*/
bool Do_The_Internet_Menu_Thang(void)
{
#ifndef DEMO

	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	/*
	** Dialog & button dimensions
	*/
	int d_dialog_w = 120 *factor;											// dialog width
	int d_dialog_h = 80*factor;											// dialog height
	int d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*factor - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_margin1=10;
	int d_txt6_h=15;

#if (GERMAN | FRENCH)
	int d_cancel_w = 50*factor;
#else
	int d_cancel_w = 40*factor;
#endif
	int d_cancel_h = 9*factor;
	int d_cancel_x = d_dialog_cx - d_cancel_w / 2;
	int d_cancel_y = d_dialog_y + d_dialog_h - 20*factor;

	char packet[10] = {"Hello"};

#if (GERMAN | FRENCH)
	int	width=160*factor;
	int	height=80*factor;
#else
	int	width=120*factor;
	int	height=80*factor;
#endif	//GERMAN | FRENCH

	Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String((char*)Text_String (TXT_CONNECTING), SeenBuff.Get_Height(), width, height);

#if (GERMAN | FRENCH)
	d_dialog_w = width + 25*factor;
	d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	d_cancel_x = d_dialog_cx - (d_cancel_w / 2);
#endif

	/*
	** Button Enumerations
	*/
	enum {
		BUTTON_CANCEL = 100,
	};

	/*
	** Buttons
	*/
	//TextButtonClass *buttons;										// button list

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
//#if (GERMAN | FRENCH)
//		d_cancel_x, d_cancel_y);
//#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
//#endif

	//buttons = &cancelbtn;

	Fancy_Text_Print(TXT_NONE, 0, 0, CC_GREEN, TBLACK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);


	char users_name[256];
	int  buffer_len = sizeof (users_name);
	bool process;
	bool display;
	KeyNumType input;


	if (!Special.IsFromWChat && !SpawnedFromWChat){
		/*
		** If the user is registered with Planet Westwood then spawn WChat.
		*/
		if (Is_User_WChat_Registered(users_name, buffer_len)){
			GameStatisticsPacketSent = false;
			if (!Spawn_WChat(true)){
				Set_Logic_Page(SeenBuff);
				Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
				Blit_Hid_Page_To_Seen_Buff();
				Set_Palette(Palette);
				CCMessageBox().Process(TXT_ERROR_UNABLE_TO_RUN_WCHAT, TXT_OK);
				LogicPage->Clear();
				return(false);
			}
		}else{
			Set_Logic_Page(SeenBuff);
			Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
			Blit_Hid_Page_To_Seen_Buff();
			Set_Palette(Palette);
			if (CCMessageBox().Process(TXT_EXPLAIN_REGISTRATION, TXT_REGISTER, TXT_CANCEL)){
				LogicPage->Clear();
				return(false);
			}else{
				LogicPage->Clear();
				Spawn_Registration_App();
				return(false);
			}
		}
	}

	/*
	**
	** User is registered and we spawned WChat. Wait for a game start message from WChat.
	**
	*/

	process = true;
	display = true;

	while (process){

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored = FALSE;
			display = true;
		}

		if (display) {

			Set_Logic_Page(SeenBuff);

			Hide_Mouse();
			/*
			** Redraw backgound & dialog box
			*/
			Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
			Blit_Hid_Page_To_Seen_Buff();
			Set_Palette(Palette);

			Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

			/*
			** Dialog & Field labels
			*/
			Draw_Caption (TXT_NONE, d_dialog_x, d_dialog_y, d_dialog_w);

			Fancy_Text_Print(TXT_CONNECTING, d_dialog_cx-width/2, d_dialog_y + 25*factor, CC_GREEN, TBLACK,
								TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

			//cancelbtn.Zap();
			//buttons = &cancelbtn;

			/*
			.................... Rebuild the button list ....................
			*/
			//buttons->Draw_All();
			cancelbtn.Draw_Me(true);

			Show_Mouse();
			display = false;
		}



		/*
		** See if the game start packet has arrived from wchat yet.
		*/
		if (DDEServer.Get_MPlayer_Game_Info()){
			//MessageBox (NULL, "About to restore focus to C&C95", "C&C95", MB_OK);
			//SetForegroundWindow ( MainWindow );
			//ShowWindow ( MainWindow, SW_SHOWMAXIMIZED	);
			return(true);
		}

		//input = buttons->Input();
		input = cancelbtn.Input();

		/*
		---------------------------- Process input ----------------------------
		*/
		switch (input) {

			/*
			** Cancel. Just return to the main menu
			*/
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				process = false;
				Send_Data_To_DDE_Server (packet, strlen(packet), DDEServerClass::DDE_CONNECTION_FAILED);
				GameStatisticsPacketSent = false;
				Spawn_WChat(false);
				break;
		}

	}

#endif	//DEMO

	return (false);


}






