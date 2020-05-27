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

#ifdef WOLAPI_INTEGRATION

//	Wol_Main.cpp - Bottom level wolapi-stuff function.
//	ajw 07/16/98

#include "function.h"
#include "WolapiOb.h"
#include "wol_gsup.h"
#include "WolStrng.h"

int WOL_Login_Dialog( WolapiObject* pWolapi );
int WOL_Chat_Dialog( WolapiObject* pWolapi );
const char* Game_Registry_Key();

bool ReregisterWolapiDLL();
void HandleDLLFail();

WolapiObject* pWolapi = NULL;

#include "WolDebug.h"

//***********************************************************************************************
//	The first time through, pWolapi is NULL thus wolapi gets set up. WOL_Login_Dialog presents the user
//	with the login dialog and attempts to log us on to the server. If the user continues on all the
//	way to a game start, we will drop out of here with pWolapi still pointing to a valid WolapiObject,
//	and with pWolapi's iLobbyReturnAfterGame set to the number of the lobby to return to automatically 
//	after the game ends.
//	Init() automatically brings us here if pWolapi is non-null.
//***********************************************************************************************
int WOL_Main()
{
	//	Return values:
	//		0 = cancel
	//		1 = start game
	//		-1 = patch downloaded, shut down app
	int iReturn = 0;

	if( pWolapi )
	{
		//	We have returned from a game started through ww online.
		
		//	Start theme up again.
		Theme.Play_Song( THEME_INTRO );

		//	Verify that we are still connected. If we aren't, kill WolapiObject and start over.
		//	(This will likely occur during the game, if connection is lost. Ensure that it is done here.)
		pWolapi->pChat->PumpMessages();			//	Causes OnNetStatus() call if no longer connected.
		if( pWolapi->bConnectionDown )
		{
//debugprint( "Re-entering WOL_Main(), pWolapi->bConnectionDown is true. Deleting old WolapiObject...\n" );
			WWMessageBox().Process( TXT_WOL_WOLAPIREINIT );
			//	Kill wolapi.
			pWolapi->UnsetupCOMStuff();
			delete pWolapi;
			pWolapi = NULL;
		}
	}

	if( !pWolapi )
	{
		//	Start up wolapi.
		pWolapi = new WolapiObject;
		if( !pWolapi->bSetupCOMStuff() )
		{
			//	Things are really bad if this happens. A COM call failed.
			
			//	We first assume that their wolapi.dll failed to register during wolsetup.exe, part of the patch process.
			//	This happens if they have an outdated oleaut32.dll, such as the one that comes with original
			//	version of Windows 95.

//			debugprint( "bSetupCOMStuff failed. Attemping to reregister wolapi.dll...\n" );
			//	Attempt to re-register wolapi.dll...
			if( ReregisterWolapiDLL() )
			{
				if( !pWolapi->bSetupCOMStuff() )
				{
					//	Still failed after reregistering seemed to work.
					HandleDLLFail();
					return 0;
				}
			}
			else
			{
				HandleDLLFail();
				return 0;
			}
		}
		pWolapi->PrepareButtonsAndIcons();
		//	Undocumented hack needed for patch downloading, per Neal.
		pWolapi->pChat->SetAttributeValue( "RegPath", Game_Registry_Key() );
		//	(Not that anything's really "documented".)
	}

	pWolapi->bInGame = false;

	int iLoginResult = WOL_Login_Dialog( pWolapi );
	if( iLoginResult == 1 )
	{
		pWolapi->SetOptionDefaults();
		bool bKeepGoing = true;
		while( bKeepGoing )
		{
			bool bCreator;
			switch( WOL_Chat_Dialog( pWolapi ) )
			{
			case -1:
				bKeepGoing = false;
				break;
			case 1:
				//	User created game channel.
				bCreator = true;
				break;
			case 2:
				//	User joined game channel.
				bCreator = false;
				break;
			}
			if( bKeepGoing )
			{
				WOL_GameSetupDialog GSupDlg( pWolapi, bCreator );
				switch( GSupDlg.Run() )
				{
				case RESULT_WOLGSUP_LOGOUT:
					//	User logged out.
					bKeepGoing = false;
					break;
				case RESULT_WOLGSUP_BACKTOCHAT:
				case RESULT_WOLGSUP_HOSTLEFT:
				case RESULT_WOLGSUP_RULESMISMATCH:
					//	Return to chat.
					break;
				case RESULT_WOLGSUP_STARTGAMEHOST:
					//	Proceed with game.
					bKeepGoing = false;
					iReturn = 1;
					pWolapi->bGameServer = true;
					break;
				case RESULT_WOLGSUP_STARTGAME:
					//	Proceed with game.
					bKeepGoing = false;
					iReturn = 1;
					pWolapi->bGameServer = false;
					break;
				case RESULT_WOLGSUP_FATALERROR:
//					debugprint( "RESULT_WOLGSUP_FATALERROR from game setup dialog.\n" );
//					Fatal( "RESULT_WOLGSUP_FATALERROR from game setup dialog.\n" );
					if( pWolapi->pChatSink->bConnected )
						pWolapi->Logout();
					bKeepGoing = false;
					break;
				}
			}
		}
	}

	if( iReturn != 1 )
	{
		//	Kill wolapi.
		pWolapi->UnsetupCOMStuff();
		delete pWolapi;
		pWolapi = NULL;
	}
	else
	{
		pWolapi->bInGame = true;
		pWolapi->bConnectionDown = false;
	}

	if( iLoginResult == -1 )
	{
		WWMessageBox().Process( TXT_WOL_DOWNLOADEXITWARNING );
		iReturn = -1;
	}

	return iReturn;
}

//***********************************************************************************************
bool ReregisterWolapiDLL()
{
	//	Attempt to reregister wolapi.dll.
	//	Returns true if we think we succeeded.
	HKEY hKey;
	char szInstallPath[ _MAX_PATH ];
	if( ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Software\\Westwood\\WOLAPI", 0, KEY_READ, &hKey ) == ERROR_SUCCESS )
	{
		DWORD dwBufSize = _MAX_PATH;
		if( ::RegQueryValueEx( hKey, "InstallPath", 0, NULL, (LPBYTE)szInstallPath, &dwBufSize ) == ERROR_SUCCESS )
		{
			WIN32_FIND_DATA wfd;
			HANDLE handle = FindFirstFile( szInstallPath, &wfd );
			if( handle == INVALID_HANDLE_VALUE )
			{
				//	File is not there.
				FindClose( handle );
				::RegCloseKey( hKey );
				return false;
			}
//			debugprint( "Found dll -> %s\n", szInstallPath );
			//	Get the DLL to register itself.
			HINSTANCE hLib = LoadLibrary( szInstallPath );
			if( !hLib )
			{
//				debugprint( "LoadLibrary failed, GetLastError is %i\n", GetLastError() );
				::RegCloseKey( hKey );
				return false;
			}
			FARPROC lpDllRegisterFunction = GetProcAddress( hLib, "DllRegisterServer" );
			if( !lpDllRegisterFunction )
			{
				::RegCloseKey( hKey );
				return false;
			}
			if( lpDllRegisterFunction() != S_OK )
			{
				::RegCloseKey( hKey );
				return false;
			}
			//	There is a bug in wolapi.dll that makes the following delay necessary.
			//	Something about Neal's extra threads only getting half-way set up before they get deleted.
			//	(The extra threads shouldn't really be created in this case, anyway...)
			::Sleep( 1000 );
			FreeLibrary( hLib );
			FindClose( handle );
		}
		else
		{
			::RegCloseKey( hKey );
			return false;
		}
		::RegCloseKey( hKey );
	}
	else
	{
		return false;
	}
	return true;
}

//***********************************************************************************************
void HandleDLLFail()
{
	//	The DLL failed to load. Either we failed to reregister it, or we think we succeeded at this but it
	//	still is not working. Show an error message and delete pWolapi.
	//	We show either "call tech support" or "download IE3", depending on whether oleaut32.dll looks out of date.

	char szPath[ _MAX_PATH + 1 ];
	::GetSystemDirectory( szPath, _MAX_PATH );
	if( *szPath && szPath[ strlen( szPath ) - 1 ] != '\\' )
		strcat( szPath, "\\" );

	strcat( szPath, "oleaut32.dll" );

	WIN32_FIND_DATA wfd;
	HANDLE handle = FindFirstFile( szPath, &wfd );

//	debugprint( "HandleDLLFail(): filesize of oleaut32 is %i\n", wfd.nFileSizeLow );
	if( handle != INVALID_HANDLE_VALUE && wfd.nFileSizeLow <= 232720 )
		WWMessageBox().Process( TXT_WOL_DLLERROR_GETIE3 );
	else
		WWMessageBox().Process( TXT_WOL_DLLERROR_CALLUS );

	FindClose( handle );

	delete pWolapi;
	pWolapi = NULL;
}

#endif
