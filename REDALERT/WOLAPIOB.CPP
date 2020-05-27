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

//	WolapiOb.cpp - Implementation of class WolapiObject.
//	ajw 07/10/98

#include "WolapiOb.h"
#include "RAWolapi.h"
#include "WolStrng.h"
#include "SEditDlg.h"
#include "ToolTip.h"
#include "Wol_GSup.h"

#include "WolDebug.h"

extern void WOL_PrintMessage( IconListClass& ILTarget, const char* szText, PlayerColorType iColorRemap = PCOLOR_NONE );
extern void WOL_PrintMessage( IconListClass& ILTarget, const char* szText, RemapControlType* pColorRemap );
void HostNameFromGameChannelName( char* szNameToSet, const char* szChannelName );

bool WOL_Options_Dialog( WolapiObject* pWO, bool bCalledFromGame );

extern bool cancel_current_msgbox;

const char* Game_Registry_Key();

//***********************************************************************************************
WolapiObject::WolapiObject() : pChat( NULL ), pDownload( NULL ), pChatSink( NULL ), pDownloadSink( NULL ),
								dwChatAdvise( 0 ), dwDownloadAdvise( 0 ), pILChat( NULL ), pILUsers( NULL ),
								CurrentLevel( WOL_LEVEL_TOP ), bChannelOwner( false ), GameTypeInfos( NULL ),
								nGameTypeInfos( 0 ), bChatShownBefore( false ),
								pILPlayers( NULL ), pChatSaveList( NULL ), iLobbyReturnAfterGame( -1 ), iLobbyLast( -1 ),
								bFindEnabled( true ),
								bPageEnabled( true ),
								bLangFilter( true ),
								bAllGamesShown( true ),
								pGSupDlg( NULL ),
								pShpDiscon( NULL ),
								pShpLeave( NULL ),
								pShpRefresh( NULL ),
								pShpSquelch( NULL ),
								pShpBan( NULL ),
								pShpKick( NULL ),
								pShpFindpage( NULL ),
								pShpOptions( NULL ),
								pShpLadder( NULL ),
								pShpHelp( NULL ),
								pShpBtnDiscon( NULL ),
								pShpBtnLeave( NULL ),
								pShpBtnRefresh( NULL ),
								pShpBtnSquelch( NULL ),
								pShpBtnBan( NULL ),
								pShpBtnKick( NULL ),
								pShpBtnFindpage( NULL ),
								pShpBtnOptions( NULL ),
								pShpBtnLadder( NULL ),
								pShpBtnHelp( NULL ),
								bReturningAfterGame( false ),
								pTTipDiscon( NULL ),
								pTTipLeave( NULL ),
								pTTipRefresh( NULL ),
								pTTipSquelch( NULL ),
								pTTipBan( NULL ),
								pTTipKick( NULL ),
								pTTipFindpage( NULL ),
								pTTipOptions( NULL ),
								pTTipLadder( NULL ),
								pTTipHelp( NULL ),
								bMyRecordUpdated( false ),
								bChannelListTitleUpdated( false ),
								bInGame( false ),
								pStaticUsers( NULL ),
								bPump_In_Call_Back( false ),
								bFreezeExternalPager( false ),
								bDoingDisconnectPinging( false ),
								bSelfDestruct( false ),
								bEggSounds( true ),
								bEgg8Player( false ),
								bShowRankRA( true ),
								bShowRankUpdated( false )
{
	*szMyName = 0;
	*szMyRecord = 0;
	*szMyRecordAM = 0;
	*szChannelListTitle = 0;
	*szChannelNameCurrent = 0;
	*szChannelReturnOnGameEnterFail = 0;
	dwTimeNextWolapiPump = ::timeGetTime() + WOLAPIPUMPWAIT;
	dwTimeNextChannelUpdate = 0;
	*szLadderServerHost = 0;
	*szGameResServerHost1 = 0;
	*szGameResServerHost2 = 0;

	strcpy( DibIconInfos[ DIBICON_OWNER ].szFile, "dib_own.bmp" );
	strcpy( DibIconInfos[ DIBICON_SQUELCH ].szFile, "dib_sqel.bmp" );
	strcpy( DibIconInfos[ DIBICON_LATENCY ].szFile, "latency.bmp" );
	strcpy( DibIconInfos[ DIBICON_ACCEPT ].szFile, "dib_acpt.bmp" );
	strcpy( DibIconInfos[ DIBICON_NOTACCEPT ].szFile, "dib_acp2.bmp" );
	strcpy( DibIconInfos[ DIBICON_USER ].szFile, "dib_user.bmp" );
	strcpy( DibIconInfos[ DIBICON_PRIVATE ].szFile, "privgame.bmp" );
	strcpy( DibIconInfos[ DIBICON_TOURNAMENT ].szFile, "tourgame.bmp" );
	strcpy( DibIconInfos[ DIBICON_VOICE ].szFile, "voice.bmp" );
	for( int i = 0; i != NUMDIBICONS; i++ )
	{
		DibIconInfos[ i ].hDIB = 0;
		DibIconInfos[ i ].pDIB = NULL;
	}

	//	Determine name of executable of user's web browser.
	//	This seems to be the "correct" way to do this, but it's bloody stupid.
	*szWebBrowser = 0;
	char szFile[] = "\\name_unlikely_to_conflict_77.html";	//"\\it is really dumb for me to have to create this file.html";
	HANDLE hFile = ::CreateFile( szFile, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( hFile );
		HINSTANCE hExecutable = ::FindExecutable( szFile, "", szWebBrowser );
//		if( (int)hExecutable <= 32 )
//		{
//			debugprint( "error %i getting browser\n", hExecutable );
//		}
//		else
//			debugprint( "szWebBrowser is %s\n", szWebBrowser );
		::DeleteFile( szFile );
	}
}

//***********************************************************************************************
WolapiObject::~WolapiObject()
{
	DeleteSavedChat();

	if( nGameTypeInfos )
	{
		//	Delete DIBs that were created, and the wol_gametypeinfos themselves
		for( unsigned int n = 0; n != nGameTypeInfos; n++ )
		{
			if( GameTypeInfos[ n ].hDIB )
			{
				GlobalUnlock( GameTypeInfos[ n ].hDIB );		//	Release pDIB.
				DestroyDIB( GameTypeInfos[ n ].hDIB );			//	Destroy mem alloc'ed for dib bits data.
			}
		}
		delete [] GameTypeInfos;
		GameTypeInfos = NULL;
	}

	for( int i = 0; i != NUMDIBICONS; i++ )
	{
		if( DibIconInfos[ i ].pDIB )
		{
			GlobalUnlock( DibIconInfos[ i ].hDIB );
			DestroyDIB( DibIconInfos[ i ].hDIB );
		}
	}

	if( pChatSink )
		UnsetupCOMStuff();

	//	Delete buttons, etc., shared by dialogs.
	delete pShpBtnDiscon;
	delete pShpBtnLeave;
	delete pShpBtnRefresh;
	delete pShpBtnSquelch;
	delete pShpBtnBan;
	delete pShpBtnKick;
	delete pShpBtnFindpage;
	delete pShpBtnOptions;
	delete pShpBtnLadder;
	delete pShpBtnHelp;
	//	Delete shared tooltips.
	delete pTTipDiscon;
	delete pTTipLeave;
	delete pTTipRefresh;
	delete pTTipSquelch;
	delete pTTipBan;
	delete pTTipKick;
	delete pTTipFindpage;
	delete pTTipOptions;
	delete pTTipLadder;
	delete pTTipHelp;
}

//***********************************************************************************************
bool WolapiObject::bSetupCOMStuff()
{
//	debugprint( "++++Begin WolapiObject::bSetupCOMStuff\n" );

	HRESULT	hRes;

	//	Grab IChat, INetUtil, set up "sinks".
//	debugprint( "CoCreateInstance\n" );
	CoCreateInstance( CLSID_Chat, NULL, CLSCTX_INPROC_SERVER, IID_IChat, (void**)&pChat );
	if( !pChat )
		return false;		//	Severe, essentially fatal.
	CoCreateInstance( CLSID_NetUtil, NULL, CLSCTX_INPROC_SERVER, IID_INetUtil, (void**)&pNetUtil );
	if( !pNetUtil )
		return false;		//	Severe, essentially fatal.

	//	Set up RAChatEventSink.
	pChatSink = new RAChatEventSink( this );
	pChatSink->AddRef();
	//	Set up RANetUtilEventSink.
	pNetUtilSink = new RANetUtilEventSink( this );
	pNetUtilSink->AddRef();

	//	If we could use ATL stuff, this would be different. (We'd use AtlAdvise.)

	IConnectionPoint*			pConnectionPoint = NULL;
	IConnectionPointContainer*	pContainer = NULL;
	
	// Get a connection point from the chat class for the chatsink.
//	debugprint( "QueryInterface\n" );
	hRes = pChat->QueryInterface( IID_IConnectionPointContainer, (void**)&pContainer );
	if( !SUCCEEDED(hRes) )
		return false;		//	Severe, essentially fatal.
//	debugprint( "FindConnectionPoint\n" );
	hRes = pContainer->FindConnectionPoint( IID_IChatEvent, &pConnectionPoint );
	if( !SUCCEEDED(hRes) )
		return false;		//	Severe, essentially fatal.
	//	Connect chat to chatsink.
//	debugprint( "Advise. pChatSink = %i, pConnectionPoint = %i\n", pChatSink, pConnectionPoint );
	hRes = pConnectionPoint->Advise( (IChatEvent*)pChatSink, &dwChatAdvise );
	if( !SUCCEEDED(hRes) )
		return false;		//	Severe, essentially fatal.
	
	pContainer->Release();
	pConnectionPoint->Release();

	pConnectionPoint = NULL;
	pContainer = NULL;
	// Get a connection point from the netutil class for the netutilsink.
//	debugprint( "QueryInterface\n" );
	hRes = pNetUtil->QueryInterface( IID_IConnectionPointContainer, (void**)&pContainer );
	if( !SUCCEEDED(hRes) )
		return false;		//	Severe, essentially fatal.
//	debugprint( "FindConnectionPoint\n" );
	hRes = pContainer->FindConnectionPoint( IID_INetUtilEvent, &pConnectionPoint );
	if( !SUCCEEDED(hRes) )
		return false;		//	Severe, essentially fatal.
	//	Connect netutil to netutilsink.
//	debugprint( "Advise. pChatSink = %i, pConnectionPoint = %i\n", pChatSink, pConnectionPoint );
	hRes = pConnectionPoint->Advise( (INetUtilEvent*)pNetUtilSink, &dwNetUtilAdvise );
	if( !SUCCEEDED(hRes) )
		return false;		//	Severe, essentially fatal.

	pContainer->Release();
	pConnectionPoint->Release();

//	debugprint( "++++End WolapiObject::bSetupCOMStuff\n" );
	return true;
}

//***********************************************************************************************
void WolapiObject::UnsetupCOMStuff()
{
//	debugprint( "----Begin WolapiObject::UnsetupCOMStuff\n" );

	HRESULT	hRes;

	//	If we could use ATL stuff, this would be different. (We'd use AtlUnadvise.)

	//	Unsetup RAChatEventSink and RANetUtilEventSink, release IChat.
	IConnectionPoint*			pConnectionPoint = NULL;
	IConnectionPointContainer*	pContainer = NULL;

//	debugprint( "QueryInterface\n" );
	hRes = pChat->QueryInterface( IID_IConnectionPointContainer, (void**)&pContainer );
	_ASSERTE(SUCCEEDED(hRes));
//	debugprint( "FindConnectionPoint\n" );
	hRes = pContainer->FindConnectionPoint( IID_IChatEvent, &pConnectionPoint );
	_ASSERTE(SUCCEEDED(hRes));
//	debugprint( "Unadvise: %i\n", dwChatAdvise );
	pConnectionPoint->Unadvise( dwChatAdvise );

	pContainer->Release();
	pConnectionPoint->Release();

	pConnectionPoint = NULL;
	pContainer = NULL;
//	debugprint( "QueryInterface\n" );
	hRes = pNetUtil->QueryInterface( IID_IConnectionPointContainer, (void**)&pContainer );
	_ASSERTE(SUCCEEDED(hRes));
//	debugprint( "FindConnectionPoint\n" );
	hRes = pContainer->FindConnectionPoint( IID_INetUtilEvent, &pConnectionPoint );
	_ASSERTE(SUCCEEDED(hRes));
//	debugprint( "Unadvise: %i\n", dwNetUtilAdvise );
	pConnectionPoint->Unadvise( dwNetUtilAdvise );

	pContainer->Release();
	pConnectionPoint->Release();

//	debugprint( "pChat->Release\n" );
	pChat->Release();

//	debugprint( "pChatSink->Release\n" );
	pChatSink->Release();	//	This results in pChatSink deleting itself for us.
	pChatSink = NULL;

//	debugprint( "pNetUtil->Release\n" );
	pNetUtil->Release();

//	debugprint( "pNetUtilSink->Release\n" );
	pNetUtilSink->Release();	//	This results in pChatSink deleting itself for us.
	pNetUtilSink = NULL;

//	debugprint( "----End WolapiObject::UnsetupCOMStuff\n" );
}

//***********************************************************************************************
void WolapiObject::LinkToChatDlg( IconListClass* pILChat, IconListClass* pILChannels, IconListClass* pILUsers, StaticButtonClass* pStaticUsers )
{
	//	Called to initialize this before the chat dialog is shown.

	//	Set pointers to lists in dialog.
	this->pILChat = pILChat;
	this->pILChannels = pILChannels;
	this->pILUsers = pILUsers;

	this->pStaticUsers = pStaticUsers;
}

//***********************************************************************************************
void WolapiObject::ClearListPtrs()
{
	//	Called to clear list pointers when chat or gamesetup dialog goes away, for safety.
	pILChat = NULL;
	pILChannels = NULL;
	pILUsers = NULL;
	
	pILPlayers = NULL;

	pStaticUsers = NULL;
}

//***********************************************************************************************
void WolapiObject::LinkToGameDlg( IconListClass* pILDisc, IconListClass* pILPlayers )
{
	//	Called to initialize this before the gamesetup dialog is shown.

	//	Set pointers to lists in dialog.
	pILChat = pILDisc;
	this->pILPlayers = pILPlayers;
}

//***********************************************************************************************
void WolapiObject::PrepareButtonsAndIcons()
{
	//	Load shapes for buttons. Store images in this order: up, down, disabled.
	//pShpDiscon = LoadShpFile( "discon.shp" ); etc
	pShpDiscon = (char*)MFCD::Retrieve( "discon.shp" );
	pShpLeave = (char*)MFCD::Retrieve( "leave.shp" );
	pShpRefresh = (char*)MFCD::Retrieve( "refresh.shp" );
	pShpSquelch = (char*)MFCD::Retrieve( "squelch.shp" );
	pShpBan = (char*)MFCD::Retrieve( "ban.shp" );
	pShpKick = (char*)MFCD::Retrieve( "kick.shp" );
	pShpFindpage = (char*)MFCD::Retrieve( "findpage.shp" );
	pShpOptions = (char*)MFCD::Retrieve( "ops.shp" );
	pShpLadder = (char*)MFCD::Retrieve( "ladder.shp" );
	pShpHelp = (char*)MFCD::Retrieve( "help.shp" );

	//	Set up standard wol buttons, used by both main dialogs. Note hardcoded ID values: must match values in dialog.
	int iWolButtons_x = 34;
	int iWolButtons_y = 20;
	int iWolButtons_dx = 53;
	int xWolButton = iWolButtons_x;
	int xTTip = 10;			//	Offset for tooltip.
	int yTTip = - 5;		//	Offset for tooltip.
	pShpBtnDiscon = new ShapeButtonClass( 100, pShpDiscon, xWolButton, iWolButtons_y );
	pTTipDiscon = new ToolTipClass( pShpBtnDiscon, TXT_WOL_TTIP_DISCON, xWolButton + xTTip, iWolButtons_y + yTTip );
	xWolButton += iWolButtons_dx;
	pShpBtnLeave = new ShapeButtonClass( 101, pShpLeave, xWolButton, iWolButtons_y );
	pTTipLeave = new ToolTipClass( pShpBtnLeave, TXT_WOL_TTIP_LEAVE, xWolButton + xTTip, iWolButtons_y + yTTip );
	xWolButton += iWolButtons_dx;
	pShpBtnRefresh = new ShapeButtonClass( 102, pShpRefresh, xWolButton, iWolButtons_y );
	pTTipRefresh = new ToolTipClass( pShpBtnRefresh, TXT_WOL_TTIP_REFRESH, xWolButton + xTTip, iWolButtons_y + yTTip );
	xWolButton += iWolButtons_dx;
	pShpBtnSquelch = new ShapeButtonClass( 103, pShpSquelch, xWolButton, iWolButtons_y );
	pTTipSquelch = new ToolTipClass( pShpBtnSquelch, TXT_WOL_TTIP_SQUELCH, xWolButton + xTTip, iWolButtons_y + yTTip );
	xWolButton += iWolButtons_dx;
	pShpBtnBan = new ShapeButtonClass( 104, pShpBan, xWolButton, iWolButtons_y );
	pTTipBan = new ToolTipClass( pShpBtnBan, TXT_WOL_TTIP_BAN, xWolButton + xTTip, iWolButtons_y + yTTip );
	xWolButton += iWolButtons_dx;
	pShpBtnKick = new ShapeButtonClass( 105, pShpKick, xWolButton, iWolButtons_y );
	pTTipKick = new ToolTipClass( pShpBtnKick, TXT_WOL_TTIP_KICK, xWolButton + xTTip, iWolButtons_y + yTTip );
	xWolButton += iWolButtons_dx;
	pShpBtnFindpage = new ShapeButtonClass( 106, pShpFindpage, xWolButton, iWolButtons_y );
	pTTipFindpage = new ToolTipClass( pShpBtnFindpage, TXT_WOL_TTIP_FINDPAGE, xWolButton + xTTip, iWolButtons_y + yTTip );
	xWolButton = 452;
	pShpBtnOptions = new ShapeButtonClass( 107, pShpOptions, xWolButton, iWolButtons_y );
	pTTipOptions = new ToolTipClass( pShpBtnOptions, TXT_WOL_TTIP_OPTIONS, xWolButton + xTTip, iWolButtons_y + yTTip, true );
	xWolButton += iWolButtons_dx;
	pShpBtnLadder = new ShapeButtonClass( 108, pShpLadder, xWolButton, iWolButtons_y );
	pTTipLadder = new ToolTipClass( pShpBtnLadder, TXT_WOL_TTIP_LADDER, xWolButton + xTTip, iWolButtons_y + yTTip, true );
	xWolButton += iWolButtons_dx;
	pShpBtnHelp = new ShapeButtonClass( 109, pShpHelp, xWolButton, iWolButtons_y );
	pTTipHelp = new ToolTipClass( pShpBtnHelp, TXT_WOL_TTIP_HELP, xWolButton + xTTip, iWolButtons_y + yTTip, true );

	//	Load standard hard-coded icons.
	HPALETTE hPal = GetCurrentScreenPalette();

	int iFileLength;
	const char* pFileData;
	for( int iDibIcon = 0; iDibIcon != NUMDIBICONS; iDibIcon++ )
	{
		//pFileData = LoadFileIntoMemory( DibIconInfos[ iDibIcon ].szFile, iFileLength );
		pFileData = (char*)MFCD::Retrieve( DibIconInfos[ iDibIcon ].szFile );
		if( pFileData )
		{
			CCFileClass ccfileDib( DibIconInfos[ iDibIcon ].szFile );
			iFileLength = ccfileDib.Size();
			//debugprint( "Loaded %s, size is %i.\n", DibIconInfos[ iDibIcon ].szFile, iFileLength );
			DibIconInfos[ iDibIcon ].hDIB = LoadDIB_FromMemory( (unsigned char*)pFileData, iFileLength );
			if( DibIconInfos[ iDibIcon ].hDIB )
			{
				DibIconInfos[ iDibIcon ].pDIB = (char*)GlobalLock( DibIconInfos[ iDibIcon ].hDIB );
				RemapDIBToPalette( hPal, DibIconInfos[ iDibIcon ].pDIB );
			}
//			else
//				debugprint( "LoadDIB_FromMemory failed!\n" );
		}
//		else
//			debugprint( "Couldn't find %s in mix.\n", DibIconInfos[ iDibIcon ].szFile );
	}

	if( DibIconInfos[ DIBICON_LATENCY ].pDIB )
		fLatencyToIconWidth = (float)DIBWidth( DibIconInfos[ DIBICON_LATENCY ].pDIB ) / 1000;
	else
		fLatencyToIconWidth = 0;

	//	All of the following is for the list of game icons...

	//	Load game icons from the wol api.
	LPCSTR szSkus;
	if( pChat->GetGametypeList( &szSkus ) == S_OK )
	{
		//	Make two copies of szSkus because strtok insists on messing with them.
		char* szSkus1 = new char[ strlen( szSkus ) + 1 ];
		char* szSkus2 = new char[ strlen( szSkus ) + 1 ];
		strcpy( szSkus1, szSkus );
		strcpy( szSkus2, szSkus );
		//	Count commas.
		char seps[] = ",";
		char* token;
		nGameTypeInfos = 0;
		token = strtok( szSkus1, seps );
		while( token != NULL )
		{
			nGameTypeInfos++;
			token = strtok( NULL, seps );
		}
		//	There are actually 2 additional game types available in wolapi - 0 (ws icon) and -1 (wwonline icon).
		nGameTypeInfos += 2;
		//	Create structs to hold infos.
//		debugprint( "Creating %i gametypeinfos\n", nGameTypeInfos );
		GameTypeInfos = new WOL_GAMETYPEINFO[ nGameTypeInfos ];
		int iMyIndex = 0;
		token = strtok( szSkus2, seps );
		while( token != NULL )
		{
			GetGameTypeInfo( atoi( token ), GameTypeInfos[ iMyIndex ], hPal );
			token = strtok( NULL, seps );
			iMyIndex++;
		}
		//	Get the two extra game type infos...
		GetGameTypeInfo( -1, GameTypeInfos[ iMyIndex++ ], hPal );
		GetGameTypeInfo( 0, GameTypeInfos[ iMyIndex++ ], hPal );
	}
//	else
//		debugprint( "GetGametypeList() failed.\n" );

	//	Load icons that we'll need to represent Red Alert GameKinds.
	//	These are available in wolapi at their old sku number locations.
	GetGameTypeInfo( 2, OldRAGameTypeInfos[ 0 ], hPal );	//	RA
	GetGameTypeInfo( 3, OldRAGameTypeInfos[ 1 ], hPal );	//	CS
	GetGameTypeInfo( 4, OldRAGameTypeInfos[ 2 ], hPal );	//	AM

	if( hPal )
		DeleteObject( hPal );
}

//***********************************************************************************************
void WolapiObject::GetGameTypeInfo( int iGameType, WOL_GAMETYPEINFO& GameTypeInfo, HPALETTE hPal )
{
	unsigned char* pVirtualFile;
	int	iFileLength;
//	debugprint( "GetGametypeInfo, type %i\n", iGameType );
	LPCSTR szName;
	LPCSTR szURL;
	pChat->GetGametypeInfo( iGameType, 12, &pVirtualFile, &iFileLength, &szName, &szURL );
	GameTypeInfo.iGameType = iGameType;
	if( szName )
		strcpy( GameTypeInfo.szName, szName );
	else
		*GameTypeInfo.szName = 0;
	if( szURL )
		strcpy( GameTypeInfo.szURL, szURL );
	else
		*GameTypeInfo.szURL = 0;

//	debugprint( "LoadDIB_FromMemory( %i, %i )\n", pVirtualFile, iFileLength );
	//	Create a DIB by "loading" (as if it was a file) the bitmap data.
	GameTypeInfo.hDIB = LoadDIB_FromMemory( pVirtualFile, iFileLength );
//	debugprint( "hDIB is %i\n", GameTypeInfo.hDIB );
	if( !GameTypeInfo.hDIB )
	{
		GameTypeInfo.pDIB = NULL;
		return;			//	Load failed. Should not ever happen.
	}
	GameTypeInfo.pDIB = (const char*)GlobalLock( GameTypeInfo.hDIB );

//	debugprint( "@@@@@ Created gametypeinfo #%i: name %s, pDIB = %i\n", iIndex, GameTypeInfo.szName, GameTypeInfo.pDIB );

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GameTypeInfo.pDIB;
	if( lpbi->biBitCount != 8 )
	{
		//	Do not use this loaded bmp, as it's not 256 color.
		GlobalUnlock( GameTypeInfo.hDIB );		//	Release pDIB.
		GameTypeInfo.pDIB = NULL;
		DestroyDIB( GameTypeInfo.hDIB );		//	Destroy mem alloc'ed for dib bits data.
		GameTypeInfo.hDIB = 0;
		return;
	}

	//	Remap colors...
	RemapDIBToPalette( hPal, GameTypeInfo.pDIB );

}

//***********************************************************************************************
void* WolapiObject::IconForGameType( int iGameType )
{
	//	Returns a GameTypeInfos entry by gametype, instead of our (potentially arbitrary) index.
	//	Returns NULL if type not found in list, which will of course never happen...
	for( int i = 0; i != nGameTypeInfos; i++ )
	{
		if( GameTypeInfos[i].iGameType == iGameType )
			return (void*)GameTypeInfos[i].pDIB;
	}
	return NULL;
}

//***********************************************************************************************
const char* WolapiObject::NameOfGameType( int iGameType ) const
{
	//	Returns the name of a sku by gametype, instead of our (potentially arbitrary) index.
	//	Returns NULL if type not found in list, which will of course never happen...
	for( int i = 0; i != nGameTypeInfos; i++ )
	{
		if( GameTypeInfos[i].iGameType == iGameType )
			return GameTypeInfos[i].szName;
	}
	return NULL;
}

//***********************************************************************************************
const char* WolapiObject::URLForGameType( int iGameType ) const
{
	//	Returns NULL if type not found in list, which will of course never happen...
	for( int i = 0; i != nGameTypeInfos; i++ )
	{
		if( GameTypeInfos[i].iGameType == iGameType )
			return GameTypeInfos[i].szURL;
	}
	return NULL;
}

//***********************************************************************************************
void WolapiObject::PrintMessage( const char* szText, PlayerColorType iColorRemap /* = PCOLOR_NONE */ )
{
	if( pILChat )
		WOL_PrintMessage( *pILChat, szText, iColorRemap );
}

//***********************************************************************************************
void WolapiObject::PrintMessage( const char* szText, RemapControlType* pColorRemap )
{
	if( pILChat )
		WOL_PrintMessage( *pILChat, szText, pColorRemap );
}

//***********************************************************************************************
HRESULT WolapiObject::GetChatServer()
{
	//	Calls RequestServerList() to get a chat server ready for us to login to.
	//	Returns S_OK and sets pChatSink->pServer if successful.
	WWMessageBox().Process( TXT_WOL_CONNECTING, TXT_NONE );

//if( !( ::GetAsyncKeyState( VK_SHIFT ) & 0x8000 ) )			//	ajw - allow use of test servers
//{

	//	Request chat server address from server server.
	pChatSink->bRequestServerListWait = true;
//	debugprint( "Calling RequestServerList...\n" );
	if( !SUCCEEDED( pChat->RequestServerList( GAME_SKU, GAME_VERSION, "unused", "unused", 5 ) ) )
	{
//		debugprint( "RequestServerList call failed\n" );
		return E_FAIL;
	}
	DWORD dwTimeLimit = timeGetTime();		//	ajw My own extra timeout at one minute, in case wolapi chokes.
//	debugprint( "Called RequestServerList...\n" );
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	::GetAsyncKeyState( VK_ESCAPE );	//	Set up for escape key checking.
	bool bCancel = false;
	hresPatchResults = 0;
	while( pChatSink->bRequestServerListWait && timeGetTime() - dwTimeLimit < 60000 )
	{
		while( timeGetTime() < dwTimeNextPump )
		{
			Call_Back();
			if( ::GetAsyncKeyState( VK_ESCAPE ) & 1 )
			{
				bCancel = true;
				break;
			}
		}
//		debugprint( "PumpMessages after RequestServerList...\n" );
		pChat->PumpMessages();
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
//        Sleep( PUMPSLEEPDURATION );	//	Can't do because we want to Call_Back()
		//	If an "update list" of patches has been received, instead of a server list, this flag will have been set
		//	for us describing the results. We'll either cancel log in or trigger game exit.
		if( hresPatchResults )
		{
			pChatSink->bRequestServerListWait = false;
			return hresPatchResults;
		}
	}
//	debugprint( "RequestServerList wait finished\n" );
	if( bCancel )
	{
		Keyboard->Clear();
		return USERCANCELLED;
	}
	if( pChatSink->pServer )
		return S_OK;
	else
		return E_FAIL;

/*
}
else
{
	//	Test using local server on LAN.

	//	Bypass RequestServerList, as it is unnecessary and may not be possible if serverserver can't be reached.
	//	Set SKU manually because normally RequestServerList does this for you.
	pChat->SetProductSKU( GAME_SKU );
	if( pChatSink->pServer )
		delete pChatSink->pServer;
	pChatSink->pServer = new Server;
	if( !( ::GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) )
		strcpy( (char*)pChatSink->pServer->conndata, "TCP;irc.westwood.com;9000" );
	else
		//	Control key down as well.
		strcpy( (char*)pChatSink->pServer->conndata, "TCP;10.2.20.28;4000" );
	strcpy( (char*)pChatSink->pServer->connlabel, "IRC" );
	strcpy( (char*)pChatSink->pServer->name, "Chat");
	return S_OK;
}
*/

}

//***********************************************************************************************
HRESULT WolapiObject::AttemptLogin( const char* szName, const char* szPass, bool bPassIsMangled )
{
	//	If RequestConnection() succeeds, sets pChatSink->bConnected true and returns S_OK.
	//	Else returns RequestConnection() error result.
	WWMessageBox().Process( TXT_WOL_ATTEMPTLOGIN, TXT_NONE );

//	debugprint( "~1\n" );
	strcpy( (char*)pChatSink->pServer->login, szName );
	strcpy( (char*)pChatSink->pServer->password, szPass );

/*
//	debugprint( "RequestConnection with:\n%s,%s,%s,%s,%s - %s\n",
					pChatSink->pServer->name,
					pChatSink->pServer->connlabel,
					pChatSink->pServer->conndata,
					pChatSink->pServer->login,
					pChatSink->pServer->password,
					bPassIsMangled ? "(mangled)" : "(unmangled)" );
*/
	pChatSink->bRequestConnectionWait = true;
	pChatSink->hresRequestConnectionError = 0;

	if( !SUCCEEDED( pChat->RequestConnection( pChatSink->pServer, 15, !bPassIsMangled ) ) )
	{
//		debugprint( "RequestConnection call failed\n" );
		return CHAT_E_CON_ERROR;
	}

	DWORD dwTimeStart = timeGetTime();
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	::GetAsyncKeyState( VK_ESCAPE );	//	Set up for escape key checking.
	bool bCancel = false;
	while( pChatSink->bRequestConnectionWait && timeGetTime() - dwTimeStart < EMERGENCY_TIMEOUT )
	{
		while( timeGetTime() < dwTimeNextPump )
		{
			Call_Back();
			if( ::GetAsyncKeyState( VK_ESCAPE ) & 1 )
			{
				bCancel = true;
				break;
			}
		}
		if( bCancel )
			break;
		pChat->PumpMessages();
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
//        Sleep( PUMPSLEEPDURATION );	//	Can't do because we want to Call_Back()
	}
	if( bCancel )
	{
		Keyboard->Clear();
		return USERCANCELLED;
	}
	if( pChatSink->bRequestConnectionWait )
		return CHAT_E_CON_ERROR;

	if( pChatSink->bConnected )
	{
		strcpy( szMyName, szName );
		strcpy( szMyRecord, szName );
		strcpy( szMyRecordAM, szName );
		return S_OK;
	}
	else
		return pChatSink->hresRequestConnectionError;
}

//***********************************************************************************************
bool WolapiObject::bLoggedIn()
{
	return pChatSink->bConnected;
}

//***********************************************************************************************
void WolapiObject::Logout()
{
	//	Requests logout from wolapi. Doesn't return any error values, as what we would do if it
	//	failed - force the user to stay connected?

	if( bSelfDestruct )
		WWMessageBox().Process( TXT_WOL_ERRORLOGOUT, TXT_NONE );
	else
		WWMessageBox().Process( TXT_WOL_ATTEMPTLOGOUT, TXT_NONE );

//	debugprint( "RequestLogout()\n" );

	pChatSink->bRequestLogoutWait = true;

	if( !SUCCEEDED( pChat->RequestLogout() ) )
	{
//		debugprint( "RequestLogout() call failed\n" );
	}

	DWORD dwTimePatience = timeGetTime();		//	After 5 seconds we run out of patience and bail.
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	while( pChatSink->bRequestLogoutWait && timeGetTime() - dwTimePatience < 5000 )
	{
		while( timeGetTime() < dwTimeNextPump )
			Call_Back();
		pChat->PumpMessages();
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	}

	pChatSink->bConnected = false;
	*szMyName = 0;

	Sound_Effect( WOLSOUND_LOGOUT );
}

//***********************************************************************************************
bool WolapiObject::UpdateChannels( int iChannelType, CHANNELFILTER ChannelFilter, bool bAutoping )
{
	//	This is now non-modal.
	//	Sends off a request for a new channels list.

//	//	Returns false upon total failure.
//	WWMessageBox().Process( TXT_WOL_WAIT, TXT_NONE );

//	pChatSink->bRequestChannelListWait = true;
	pChatSink->ChannelFilter = ChannelFilter;

//	debugprint( "RequestChannelList(), iChannelType = %i, filter = %i\n", iChannelType, ChannelFilter );
	if( !SUCCEEDED( pChat->RequestChannelList( iChannelType, bAutoping ) ) )
	{
//		debugprint( "RequestChannelList() call failed\n" );
		return false;
	}
/*
	DWORD dwTimeStart = timeGetTime();
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	while( pChatSink->bRequestChannelListWait && timeGetTime() - dwTimeStart < EMERGENCY_TIMEOUT )
	{
		while( timeGetTime() < dwTimeNextPump )
			Call_Back();
		pChat->PumpMessages();
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	}

	if( pChatSink->bRequestChannelListWait )
		return false;
*/
	LastUpdateChannelCallLevel = CurrentLevel;

	return true;
}

//***********************************************************************************************
void WolapiObject::OnChannelList()
{
	//	The chatsink calls this when its OnChannelList() is called, and it has remade its internal channel list.
	//	The question here is: should we display the values now in the chatsink?
	//	As UpdateChannels() is now non-modal, there is the danger that we have moved away from the place in
	//	the channel heirarchy where we originally called RequestChannelList().
	//	To help ensure we're getting this where we expect to get it, we check the value of CurrentLevel against
	//	what it was when we called UpdateChannels().
	if( CurrentLevel == LastUpdateChannelCallLevel )
		ListChannels();
}

//***********************************************************************************************
void WolapiObject::ListChannels()
{
	//	Show pChatSink's pChannelList in pILChannels.
	//	The extra data ptr hidden in each list item will hold a void pointer to the channel described.
//	debugprint( "ListChannels(), pChannelList = %i\n", pChatSink->pChannelList );

	static WOL_LEVEL	LevelLastListed = WOL_LEVEL_INVALID;

	int iListViewIndex = 0;

	//	If redrawing the same list as before, preserve the view position.
	if( LevelLastListed == CurrentLevel )
		iListViewIndex = pILChannels->Get_View_Index();
	else
		LevelLastListed = CurrentLevel;

	pILChannels->Clear();
	switch( CurrentLevel )
	{
	case WOL_LEVEL_GAMESOFTYPE:
		pILChannels->Add_Item( TXT_WOL_CHANNEL_BACK, CHANNELTYPE_GAMES, NULL, ICON_SHAPE, CHANNELTYPE_GAMES );
		break;
	case WOL_LEVEL_LOBBIES:
		pILChannels->Add_Item( TXT_WOL_CHANNEL_BACK, CHANNELTYPE_GAMES, NULL, ICON_SHAPE, CHANNELTYPE_GAMES );
		break;
	case WOL_LEVEL_INLOBBY:
		pILChannels->Add_Item( TXT_WOL_CHANNEL_BACK, CHANNELTYPE_LOBBIES, NULL, ICON_SHAPE, CHANNELTYPE_LOBBIES );
		break;
	default:
		pILChannels->Add_Item( TXT_WOL_CHANNEL_TOP, CHANNELTYPE_TOP, NULL, ICON_SHAPE, CHANNELTYPE_TOP );
		break;
	}

	Channel* pChannel = pChatSink->pChannelList;
	while( pChannel )
	{
		if( pChannel->type == 0 )
		{
			//	Show chat channel.
			char* pShow;
			int iLobby = iChannelLobbyNumber( pChannel->name );
			if( iLobby == - 1 )
			{
				//	Regular chat channel.
				pShow = new char[ strlen( (char*)pChannel->name ) + 10 ];
				sprintf( pShow, "%s\t%-3u", (char*)pChannel->name, pChannel->currentUsers );
				char szHelp[ 200 ];
				sprintf( szHelp, TXT_WOL_TTIP_CHANLIST_CHAT, (char*)pChannel->name, pChannel->currentUsers );
				pILChannels->Add_Item( pShow, szHelp, (void*)DibIconInfos[ DIBICON_USER ].pDIB, ICON_DIB, CHANNELTYPE_CHATCHANNEL, (void*)pChannel );
			}
			else
			{
				//	Channel is a lobby.
				char szLobbyName[ REASONABLELOBBYINTERPRETEDNAMELEN ];
				InterpretLobbyNumber( szLobbyName, iLobby );
				pShow = new char[ REASONABLELOBBYINTERPRETEDNAMELEN + 10 ];
				sprintf( pShow, "%s\t%-3u", szLobbyName, pChannel->currentUsers );
				char szHelp[ 200 ];
				sprintf( szHelp, TXT_WOL_TTIP_CHANLIST_LOBBY, szLobbyName, pChannel->currentUsers );
				pILChannels->Add_Item( pShow, szHelp, IconForGameType( -1 ), ICON_DIB, CHANNELTYPE_LOBBYCHANNEL, (void*)pChannel );
//				debugprint( ":::::added pChannel %i, name %s, as %s\n", pChannel, pChannel->name, pShow );
			}
			delete [] pShow;
		}
		else
		{
			//	Show game channel.
			char* pShow = new char[ strlen( (char*)pChannel->name ) + 10 ];
			char szHelp[ 200 ];
			void* pGameKindIcon;
			if( pChannel->type == GAME_TYPE )
			{
				//	Get RedAlert GameKind.
				CREATEGAMEINFO::GAMEKIND GameKind = (CREATEGAMEINFO::GAMEKIND)( pChannel->reserved & 0xFF000000 );
				switch( GameKind )
				{
				case CREATEGAMEINFO::RAGAME:
					pGameKindIcon = (void*)OldRAGameTypeInfos[ 0 ].pDIB;		//	Red Alert icon
					sprintf( szHelp, TXT_WOL_TTIP_CHANLIST_RAGAME, TXT_WOL_TTIP_REDALERT, 
								pChannel->currentUsers, pChannel->maxUsers );
					break;
				case CREATEGAMEINFO::CSGAME:
					pGameKindIcon = (void*)OldRAGameTypeInfos[ 1 ].pDIB;		//	CS icon
					sprintf( szHelp, TXT_WOL_TTIP_CHANLIST_RAGAME, TXT_WOL_TTIP_COUNTERSTRIKE, 
								pChannel->currentUsers, pChannel->maxUsers );
					break;
				case CREATEGAMEINFO::AMGAME:
					pGameKindIcon = (void*)OldRAGameTypeInfos[ 2 ].pDIB;		//	AM icon
					sprintf( szHelp, TXT_WOL_TTIP_CHANLIST_RAGAME, TXT_WOL_TTIP_AFTERMATH, 
								pChannel->currentUsers, pChannel->maxUsers );
					break;
				default:
//					debugprint( "Illegal value for GameKind channel reserved field: %s\n", (char*)pChannel->name );
					pGameKindIcon = NULL;
					break;
				}
				sprintf( pShow, "%s\t%u/%u", (char*)pChannel->name, pChannel->currentUsers, pChannel->maxUsers );
			}
			else
			{
				pGameKindIcon = IconForGameType( pChannel->type );
				sprintf( pShow, "%s\t%-2u", (char*)pChannel->name, pChannel->currentUsers );
				sprintf( szHelp, TXT_WOL_TTIP_CHANLIST_GAME, NameOfGameType( pChannel->type ), 
							pChannel->currentUsers );
			}
			void* pPrivateIcon = NULL;
			if( pChannel->flags & CHAN_MODE_KEY )
			{
				//	Game is private.
				pPrivateIcon = (void*)DibIconInfos[ DIBICON_PRIVATE ].pDIB;
				strcat( szHelp, TXT_WOL_TTIP_PRIVATEGAME );
			}

			void* pTournamentIcon = NULL;
			if( pChannel->tournament )
			{
				//	Game is tournament.
				pTournamentIcon = (void*)DibIconInfos[ DIBICON_TOURNAMENT ].pDIB;
				strcat( szHelp, TXT_WOL_TTIP_TOURNAMENTGAME );
			}

			int iLatencyUse = pChannel->latency;
			if( iLatencyUse == -1 )
				iLatencyUse = 0;

			static int iLatencyBarX = 227 - DIBWidth( DibIconInfos[ DIBICON_LATENCY ].pDIB ) - 19;

			pILChannels->Add_Item( pShow, szHelp, pGameKindIcon, ICON_DIB,
									CHANNELTYPE_GAMECHANNEL, (void*)pChannel, NULL,
									pPrivateIcon, ICON_DIB, pTournamentIcon, ICON_DIB,
									(void*)DibIconInfos[ DIBICON_LATENCY ].pDIB, ICON_DIB, 
									iLatencyBarX, 0, iLatencyUse * fLatencyToIconWidth );
			delete [] pShow;
		}
		pChannel = pChannel->next;
	}
	if( iListViewIndex )
		pILChannels->Set_View_Index( iListViewIndex );	//	Not perfect but should keep list pretty stable on updates.
}

//***********************************************************************************************
HRESULT WolapiObject::ChannelJoin( const char* szChannelName, const char* szKey )
{
	//	Used for CHAT channels (or lobbies) only. Channel type is set to 0.
	Channel ChannelTemp;
	memset( &ChannelTemp, 0, sizeof( ChannelTemp ) );
	strcpy( (char*)ChannelTemp.name, szChannelName );
	strcpy( (char*)ChannelTemp.key, szKey );
	return ChannelJoin( &ChannelTemp );
}

//***********************************************************************************************
HRESULT WolapiObject::ChannelJoin( Channel* pChannelToJoin )
{
	//	Returns an HRESULT, the meaning of which is totally customized for my own uses.

	WWMessageBox().Process( TXT_WOL_WAIT, TXT_NONE );

	pChatSink->bRequestChannelJoinWait = true;
	pChatSink->hresRequestJoinResult = 0;

//	debugprint( "RequestChannelJoin(), %s\n", pChannelToJoin->name );
	HRESULT hRes = pChat->RequestChannelJoin( pChannelToJoin );
	if( !SUCCEEDED( hRes ) )
	{
//		debugprint( "RequestChannelJoin() call failed, result %i ", hRes );
		DebugChatDef( hRes );
		return E_FAIL;
	}
	pChatSink->bIgnoreChannelLists = true;		//	Turn off response to channel lists.
	DWORD dwTimeStart = timeGetTime();
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	while( pChatSink->bRequestChannelJoinWait && timeGetTime() - dwTimeStart < EMERGENCY_TIMEOUT )
	{
		while( timeGetTime() < dwTimeNextPump )
			Call_Back();
		pChat->PumpMessages();
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	}
	pChatSink->bIgnoreChannelLists = false;		//	Turn on response to channel lists.

	if( pChatSink->bRequestChannelJoinWait )
		return CHAT_E_TIMEOUT;

	switch( pChatSink->hresRequestJoinResult )
	{
	case CHAT_E_CHANNELDOESNOTEXIST:
	case CHAT_E_BADCHANNELPASSWORD:
	case CHAT_E_BANNED:
	case CHAT_E_CHANNELFULL:
		return pChatSink->hresRequestJoinResult;
	}

	if( !pChatSink->bJoined )
		return E_FAIL;

	return S_OK;
}

//***********************************************************************************************
bool WolapiObject::ChannelLeave()
{
	//	Returns false upon total failure.
	WWMessageBox().Process( TXT_WOL_WAIT, TXT_NONE );

	pChatSink->bRequestChannelLeaveWait = true;
	pChatSink->DeleteUserList();
	
//	debugprint( "RequestChannelLeave()\n" );
	if( !SUCCEEDED( pChat->RequestChannelLeave() ) )
	{
//		debugprint( "RequestChannelLeave() call failed\n" );
		return false;
	}
	pChatSink->bIgnoreChannelLists = true;		//	Turn off response to channel lists.
	DWORD dwTimeStart = timeGetTime();
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	while( pChatSink->bRequestChannelLeaveWait && timeGetTime() - dwTimeStart < EMERGENCY_TIMEOUT )
	{
		while( timeGetTime() < dwTimeNextPump )
			Call_Back();
		pChat->PumpMessages();
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	}
	pChatSink->bIgnoreChannelLists = false;

	if( pChatSink->bRequestChannelLeaveWait || pChatSink->bJoined )
		return false;

	return true;
}

/*
//***********************************************************************************************
bool WolapiObject::UserList()
{
	//	Returns false upon total failure.
	WWMessageBox().Process( TXT_WOL_WAIT, TXT_NONE );

	pChatSink->bRequestUserListWait = true;

	//	I no longer request a user list, as this was being done only when entering a channel, and it turns out wolapi gives
	//	us a user list automatically when joining. This function is used as a blocker that waits until the user list has
	//	definitely arrived.
//	debugprint( "RequestUserList()\n" );
//	if( !SUCCEEDED( pChat->RequestUserList() ) )
//	{
//		debugprint( "RequestUserList() call failed\n" );
//		return false;
//	}

	DWORD dwTimeStart = timeGetTime();
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	while( pChatSink->bRequestUserListWait && timeGetTime() - dwTimeStart < EMERGENCY_TIMEOUT )
	{
		while( timeGetTime() < dwTimeNextPump )
			Call_Back();
		pChat->PumpMessages();
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	}

	if( pChatSink->bRequestUserListWait )
	{
		pChatSink->bRequestUserListWait = false;
		return false;
	}

	return true;
}
*/

//***********************************************************************************************
//typedef char CHANNELUSERNAME[ WOL_NAME_LEN_MAX ];
struct CHANNELUSERINFO
{
	char				szName[ WOL_NAME_LEN_MAX ];
	bool				bFlagged;
	RemapControlType*	pColorRemap;
	HousesType			House;			//	Only used if game channel.
	bool				bAccept;		//	Only used if game channel.
	char				szExtra[ 50 ];	//	Only used if game channel.
};

//***********************************************************************************************
bool WolapiObject::ListChannelUsers()
{
	//	Show pChatSink's pUserList in pILUsers or pILPlayers (depending on CurrentLevel), after clearing it.
	//	The extra data ptr hidden in each list item will hold a void pointer to the user described.
	//	For simplicity, I destroy the old list and write a new one, even though possibly only one item
	//	may have changed.
	//	In order for the multiselect flags in the list to persist, I record the names that are flagged
	//	before clearing the list, then reset them (if found) in the new list.
	//	This is inefficient, but should be fine in this non-time-critical situation.
	//	(I also save item color here, and save all items. Now it's really inefficient.)
	//	(Oh, boy. Now I've added the persistence of house info when this is a game channel...)
	//	The idea was to avoid duplication of player data, and not have any dependence on the integrity of the chatsink's
	//	players list. Now it is a case of it working "well enough" to not require time to elegantize it.

	//	Extra bonus, if useful: returns true if an operator (channel owner) is found in the channel, false otherwise.

	bool bChannelOwnerFound = false;

	bool bInLobby;
	IconListClass* pListToUse;
	if( CurrentLevel == WOL_LEVEL_INGAMECHANNEL )
	{
		bInLobby = false;
		pListToUse = pILPlayers;
	}
	else
	{
		bInLobby = ( iChannelLobbyNumber( (unsigned char*)szChannelNameCurrent ) != -1 );
		pListToUse = pILUsers;
	}

	if( pListToUse &&				//	Fails in rare cases when list draw is triggered before it is fully set up.
		*szChannelNameCurrent )		//	No users to list if not in a channel.
	{
		//	If redrawing the same list as before, preserve the view position.
		static char szChannelLastListed[ WOL_CHANNAME_LEN_MAX ] = { 0 };
//debugprint( "szChannelLastListed '%s', szChannelNameCurrent '%s'\n", szChannelLastListed, szChannelNameCurrent );
		int iListViewIndex = 0;
		if( strcmp( szChannelLastListed, szChannelNameCurrent ) == 0 )
			iListViewIndex = pListToUse->Get_View_Index();
		else
			strcpy( szChannelLastListed, szChannelNameCurrent );

//debugprint( "ListChannelUsers(), pUserList = %i\n", pChatSink->pUserList );
		//	Save users in current list.
		int iCount = pListToUse->Count();
		CHANNELUSERINFO* pUsersSaved = NULL;
		int iUsersSaved = 0;
		if( iCount )
		{
			pUsersSaved = new CHANNELUSERINFO[ iCount ];
			for( int i = 0; i != iCount; i++ )
			{
				PullPlayerName_Into_From( pUsersSaved[ iUsersSaved ].szName, pListToUse->Get_Item( i ) );
				pUsersSaved[ iUsersSaved ].bFlagged = pListToUse->bItemIsMultiSelected( i );
				pUsersSaved[ iUsersSaved ].pColorRemap = pListToUse->Get_Item_Color( i );
//				debugprint( "  Saving color of %s as %i.\n", pUsersSaved[ iUsersSaved ].szName, pUsersSaved[ iUsersSaved ].pColorRemap );
				if( CurrentLevel == WOL_LEVEL_INGAMECHANNEL )
				{
					pUsersSaved[ iUsersSaved ].House = PullPlayerHouse_From( pListToUse->Get_Item( i ) );
					pUsersSaved[ iUsersSaved ].bAccept = bItemMarkedAccepted( i );
					const char* szExtra = pListToUse->Get_Item_ExtraDataString( i );
					if( szExtra )
						strcpy( pUsersSaved[ iUsersSaved ].szExtra, szExtra );
					else
						*pUsersSaved[ iUsersSaved ].szExtra = 0;
				}
				iUsersSaved++;
			}
		}
		//	Clear list and recreate with new users list.
		pListToUse->Clear();
		User* pUser = pChatSink->pUserList;
		int iUserCount = 0;
		while( pUser )
		{
			++iUserCount;
			void* pIcon1 = NULL;
			void* pIcon2 = NULL;
			if( pUser->flags & CHAT_USER_CHANNELOWNER )
			{
				pIcon1 = (void*)DibIconInfos[ DIBICON_OWNER ].pDIB;
				bChannelOwnerFound = true;
			}
			else
			{
				if( CurrentLevel == WOL_LEVEL_INGAMECHANNEL )				
					pIcon1 = (void*)DibIconInfos[ DIBICON_NOTACCEPT ].pDIB;
				else
				{
					if( pUser->flags & CHAT_USER_VOICE )
						pIcon1 = (void*)DibIconInfos[ DIBICON_VOICE ].pDIB;
					else
						pIcon1 = (void*)DibIconInfos[ DIBICON_USER ].pDIB;
				}
			}
			if( pUser->flags & CHAT_USER_SQUELCHED )
				pIcon2 = (void*)DibIconInfos[ DIBICON_SQUELCH ].pDIB;

			if( CurrentLevel == WOL_LEVEL_INGAMECHANNEL || bInLobby )
			{				
				int iRank = pNetUtilSink->GetUserRank( (char*)pUser->name, bShowRankRA );
				char szNameToShow[ WOL_NAME_LEN_MAX + 40 ];
				if( iRank )
				{
//					debugprint("  Found %s has rank %u\n", (char*)pUser->name, iRank );
					sprintf( szNameToShow, TXT_WOL_USERRANK, (char*)pUser->name, iRank );
				}
				else
					strcpy( szNameToShow, (char*)pUser->name );

				static int iLatencyBarX = 124*RESFACTOR - DIBWidth( DibIconInfos[ DIBICON_LATENCY ].pDIB ) - 5 - 16;

				//	If we have had a chance to request pings to the player, there'll be some avg. results waiting for us.
				int iLatencyBarWidth = 0;
				int iLatency;
				if( CurrentLevel == WOL_LEVEL_INGAMECHANNEL )
				{
					unsigned long UserIP = pChatSink->GetUserIP( (char*)pUser->name );
//					debugprint( "player %s ip address %i\n", szNameToShow, UserIP );
					if( UserIP && pNetUtil->GetAvgPing( UserIP, &iLatency ) == S_OK )
					{
//						debugprint( "player %s latency %i\n", szNameToShow, iLatency );
						if( iLatency == -1 )
							iLatency = 0;
						iLatencyBarWidth = iLatency * fLatencyToIconWidth;
					}
				}
				pListToUse->Add_Item( szNameToShow, NULL, pIcon1, ICON_DIB, NULL, (void*)pUser, NULL, pIcon2, ICON_DIB, NULL, ICON_DIB, 
						(void*)DibIconInfos[ DIBICON_LATENCY ].pDIB, ICON_DIB, iLatencyBarX, 2, iLatencyBarWidth );
			}
			else
				pListToUse->Add_Item( (char*)pUser->name, NULL, pIcon1, ICON_DIB, NULL, (void*)pUser, NULL, pIcon2, ICON_DIB );

			pUser = pUser->next;
		}
		if( pStaticUsers )
		{
			//	Display number of users in channel.
			char szCount[100];
			sprintf( szCount, TXT_WOL_USERLIST, iUserCount );
			pStaticUsers->Set_Text( szCount );
		}
		//	Reset multiselectedness, color, and item text for a user. Slow.
		//	(What a bloody, bloody hack.)
		for( int iUser = 0; iUser != iUsersSaved; iUser++ )
		{
			int iFind = pListToUse->Find( pUsersSaved[ iUser ].szName );		//	Finds any item beginning with szName...
			if( iFind != -1 )
			{
				if( CurrentLevel == WOL_LEVEL_INGAMECHANNEL )
				{
					if( pUsersSaved[ iUser ].House != HOUSE_NONE )
					{
						//	Append house text to item string, as we found a valid house name after the name, above.
						char szItem[ 120 ];
						WritePlayerListItem( szItem, pUsersSaved[ iUser ].szName, pUsersSaved[ iUser ].House );
						pListToUse->Set_Item( iFind, szItem );
					}
					if( pUsersSaved[ iUser ].bAccept )
					{
						//	Player was marked "accepted" before. If he has one now, it's because he is the host.
						//	Else it was an accepted icon before, so put one in again now. (a-hacking-we-will-go)
						if( !bItemMarkedAccepted( iFind ) )
							MarkItemAccepted( iFind, true );
					}
					if( *pUsersSaved[ iUser ].szExtra )
						pListToUse->Set_Item_ExtraDataString( iFind, pUsersSaved[ iUser ].szExtra );
				}
				if( pUsersSaved[ iUser ].bFlagged )
					pListToUse->MultiSelect( iFind, true );
//				debugprint( "  Restoring color of %s as %i.\n", pUsersSaved[ iUser ].szName, pUsersSaved[ iUser ].pColorRemap );
				pListToUse->Set_Item_Color( iFind, pUsersSaved[ iUser ].pColorRemap );
			}
//			else
//				debugprint( "ListChannelUsers() - Couldn't find %s!\n", pUsersSaved[ iUser ].szName );
		}
		delete [] pUsersSaved;
		if( iListViewIndex )
			pListToUse->Set_View_Index( iListViewIndex );	//	Not perfect but should keep list pretty stable on updates.
	}
	return bChannelOwnerFound;
}

//***********************************************************************************************
bool WolapiObject::bItemMarkedAccepted( int iIndex )
{
	//	Returns true if the iIndex'th entry in pILPlayers has an icon pointer in position 0 that
	//	is either the host icon or the accepted icon.
	const IconList_ItemExtras* pItemExtras = pILPlayers->Get_ItemExtras( iIndex );
	return ( pItemExtras->pIcon[0] == (void*)DibIconInfos[ DIBICON_OWNER ].pDIB || 
				pItemExtras->pIcon[0] == (void*)DibIconInfos[ DIBICON_ACCEPT ].pDIB );
}

//***********************************************************************************************
bool WolapiObject::MarkItemAccepted( int iIndex, bool bAccept )
{
	pILPlayers->Flag_To_Redraw();
	if( bAccept )
		return pILPlayers->Set_Icon( iIndex, 0, (void*)DibIconInfos[ DIBICON_ACCEPT ].pDIB, ICON_DIB );
	else
		//return pILPlayers->Set_Icon( iIndex, 0, NULL, ICON_DIB );
		return pILPlayers->Set_Icon( iIndex, 0, (void*)DibIconInfos[ DIBICON_NOTACCEPT ].pDIB, ICON_DIB );		
}

//***********************************************************************************************
bool WolapiObject::bItemMarkedReadyToGo( int iIndex )
{
	//	Returns true if the iIndex'th entry in pILPlayers marks the player as "ready to go".
	//	This is true if the player is marked as "ready" or "need scenario".
	const char* szItem = pILPlayers->Get_Item_ExtraDataString( iIndex );
	if( !szItem )
		return false;
//	debugprint( "szItem is %s\n", szItem );
	return ( strcmp( szItem, "ready" ) == 0 || strcmp( szItem, "need scenario" ) == 0 );
}

//***********************************************************************************************
void WolapiObject::MarkItemReadyToGo( int iIndex, const char* szReadyState )
{
	//	Set szReadyState to "ready", "need scenario", or NULL.
	//	First two cases are regarded as player being ready to go.
	pILPlayers->Flag_To_Redraw();
	pILPlayers->Set_Item_ExtraDataString( iIndex, szReadyState );
}

//***********************************************************************************************
bool WolapiObject::bItemMarkedNeedScenario( int iIndex )
{
	//	Returns true if the iIndex'th entry in pILPlayers marks the player as ready to go, but needing scenario download.
	const char* szItem = pILPlayers->Get_Item_ExtraDataString( iIndex );
	if( !szItem )
		return false;
	return ( strcmp( szItem, "need scenario" ) == 0 );
}

//***********************************************************************************************
void WolapiObject::PullPlayerName_Into_From( char* szDest, const char* szSource )
{
	//	Sets szDest to the "player name" found in szSource.
	//	Called "player" name because this is mainly designed for use in game channels.
	//	Player name appears first in item, separated by a space from anything later.

	char* pSpace = strstr( szSource, " " );
	if( !pSpace )
	{
		//	No space character. Use entire item.	
		strcpy( szDest, szSource );
	}
	else
	{
		int iSpacePosition = pSpace - szSource;
		strncpy( szDest, szSource, iSpacePosition );
		szDest[ iSpacePosition ] = 0;		//	terminate
	}
//	debugprint( "PullPlayerName_Into_From: '%s' from '%s', ok?\n", szDest, szSource );
}

//***********************************************************************************************
HousesType WolapiObject::PullPlayerHouse_From( const char* szSource )
{
	//	Pulls the house value out of a player list item in a game channel.
	//	House appears as the last word, and it's in <>.
//	char* pChar = strrchr( szSource, ' ' );		//	Last space character.		was failing on roy. uni cause of space
//	if( !pChar )
//		return HOUSE_NONE;
//	++pChar;
//	if( *pChar++ != '<' )		//	We know house has to be last, so if not the case, no house in item.
//		return HOUSE_NONE;
	char* pChar = strrchr( szSource, '<' );		//	Last < character.
	if( !pChar )
		return HOUSE_NONE;
	++pChar;
	int iLen = strlen( pChar );	//	Remaining: "housename>"
	//	Copy remaining string.
	char szHouse[ 30 ];
	strcpy( szHouse, pChar );
	*( szHouse + iLen - 1 ) = 0;	//	Terminate to remove ">"
//	debugprint( "PullPlayerHouse_From: '%s' from '%s', ok?\n", szHouse, szSource );
	//	pChar is now a valid house name.

//	return HouseTypeClass::From_Name( szHouse );
#ifdef ENGLISH
	//	Bloody bloody hell I can't believe there are bugs in RA like the one I deal with here...
	if( strcmp( szHouse, "Russia" ) == 0 )
		return HOUSE_USSR;
	else
		return HouseTypeClass::From_Name( szHouse );	//	Fails on "Russia". (Thinks "USSR".)
#else
	for( HousesType house = HOUSE_USSR; house <= HOUSE_FRANCE; house++ )
		if( strcmp( Text_String(HouseTypeClass::As_Reference(house).Full_Name()), szHouse ) == 0 )
			return house;
//	debugprint( "dohfus" );		//	should never happen
//	Fatal( "" );
	return HOUSE_USSR;
#endif
}

//***********************************************************************************************
void WolapiObject::WritePlayerListItem( char* szDest, const char* szName, HousesType House )
{
	//	Sets szDest to the way a player list item appears in a game channel.
	char szHouse[ 50 ];
	strcpy( szHouse, Text_String( HouseTypeClass::As_Reference( House ).Full_Name() ) );

	int iRank = pNetUtilSink->GetUserRank( szName, bShowRankRA );	//	Horrendous inefficiency here, when called for relisting players...
	if( iRank )
		sprintf( szDest, TXT_WOL_USERRANKHOUSE, szName, iRank, szHouse );
	else
		sprintf( szDest, TXT_WOL_USERHOUSE, szName, szHouse );
//	debugprint( "WritePlayerListItem: '%s', ok?\n", szDest );
}

//***********************************************************************************************
void WolapiObject::RequestPlayerPings()
{
	//	Does a RequestPing for every other player listed in pILPlayers.
	for( int i = 0; i < pILPlayers->Count(); i++ )
	{
		User* pUser = (User*)pILPlayers->Get_Item_ExtraDataPtr( i );
		if( pUser && !( pUser->flags & CHAT_USER_MYSELF ) )
		{
			unsigned long UserIP = pChatSink->GetUserIP( (char*)pUser->name );
			if( UserIP )
			{
				int iUnused;
				in_addr inaddrUser;
				inaddrUser.s_addr = UserIP;
				char* szIP = inet_ntoa( inaddrUser );
//				debugprint( "RequestPing of %s, ipaddr of %i, aka %s\n", (char*)pUser->name, UserIP, szIP );
				pNetUtil->RequestPing( szIP, 1000, &iUnused );
			}
		}
	}
}

//***********************************************************************************************
void WolapiObject::SendMessage( const char* szMessage, IconListClass& ILUsers, bool bAction )
{
	//	Send regular chat message.

	if( *szMessage == 0 )
		return;

	if( strlen( szMessage ) > 4 && szMessage[0] == 63 && szMessage[1] == 97 && szMessage[2] == 106 && szMessage[3] == 119 )
	{
		int i = atoi( szMessage + 4 );
		if( i >= VOX_ACCOMPLISHED && i <= VOX_LOAD1 )
			Speak( (VoxType)i );
		return;
	}
	if( strlen( szMessage ) > 4 && szMessage[0] == 35 && szMessage[1] == 97 && szMessage[2] == 106 && szMessage[3] == 119 )
	{
		int i = atoi( szMessage + 4 );
		if( i >= VOX_ACCOMPLISHED && i <= VOX_LOAD1 )
			Speak( (VoxType)i );
	}

	//	Iterate through ILUsers looking for selected entries. Build up a users list of selected
	//	items. If the list turns out to be blank, send message publicly.
	User* pUserListSend = NULL;
	User* pUserNew;
	User* pUserTail = NULL;
	int iCount = ILUsers.Count();
	int iPrivatePrintLen = 1;
	for( int i = 0; i != iCount; i++ )
	{
		if( ILUsers.bItemIsMultiSelected( i ) )
		{
			pUserNew = new User;
			*pUserNew = *( (User*)ILUsers.Get_Item_ExtraDataPtr( i ) );
//			debugprint( "Copied %s for sendmessage.\n", pUserNew->name );
			pUserNew->next = NULL;			//	(We don't want the value that was just copied!)
			if( !pUserTail )
			{
				//	First User in list.
				pUserListSend = pUserNew;
			}
			else
			{
				pUserTail->next = pUserNew;
			}
			pUserTail = pUserNew;
			iPrivatePrintLen += ( strlen( (char*)pUserNew->name ) + 2 );		//	Extra space and comma.
		}
	}
	if( pUserListSend )
	{
		//	Send private message.
		if( !bAction )
			pChat->RequestPrivateMessage( pUserListSend, szMessage );
		else
			pChat->RequestPrivateAction( pUserListSend, szMessage );
		char* szPrint = 0;
		if( iPrivatePrintLen > 50 )
		{
			//	Too many users specified to print out. Just say "multiple users".
			if( !bAction )
			{
				szPrint = new char[ strlen( szMessage ) + 135 ];
				sprintf( szPrint, "%s %s", TXT_WOL_PRIVATETOMULTIPLE, szMessage );
			}
			else
			{
				szPrint = new char[ strlen( szMessage ) + strlen( szMyName ) + 138 ];
				sprintf( szPrint, "%s %s %s", TXT_WOL_PRIVATETOMULTIPLE, szMyName, szMessage );
			}
		}
		else
		{
			if( !bAction )
				szPrint = new char[ strlen( szMessage ) + iPrivatePrintLen + 120 ];
			else
				szPrint = new char[ strlen( szMessage ) + iPrivatePrintLen + 125 + strlen( szMyName ) ];
			//strcpy( szPrint, "<Private to " );
			sprintf( szPrint, "<%s ", TXT_WOL_PRIVATETO );
			User* pUserPrint = pUserListSend;
			while( pUserPrint )
			{
				strcat( szPrint, (char*)pUserPrint->name );
				if( pUserPrint->next )
					strcat( szPrint, ", " );
				else
					strcat( szPrint, ">: " );
				pUserPrint = pUserPrint->next;
			}
			if( bAction )
			{
				strcat( szPrint, szMyName );
				strcat( szPrint, " " );
			}
			strcat( szPrint, szMessage );
		}
		if( !bAction )
			PrintMessage( szPrint, WOLCOLORREMAP_SELFSPEAKING );
		else
		{
			PrintMessage( szPrint, WOLCOLORREMAP_ACTION );
			pChatSink->ActionEggSound( szMessage );
		}
		delete [] szPrint;
	}
	else
	{
		//	Send public message.
		if( !bAction )
		{
			//	Easter egg related.
			if( _stricmp( szMessage, "/nousersounds" ) == 0 )
			{
				bEggSounds = false;
				return;
			}
			else if( _stricmp( szMessage, "/usersounds" ) == 0 )		//	Left as obvious text in the exe, for someone to find... :-)
			{
				bEggSounds = true;
				return;
			}
			else if( _stricmp( szMessage, "/8playergames" ) == 0 )		//	Left as obvious text in the exe, for someone to find... :-)
			{
				bEgg8Player = true;
				return;
			}
			HRESULT hRes = pChat->RequestPublicMessage( szMessage );
			if( hRes != S_OK )
			{
//				debugprint( "                           RequestPublicMessage() failed with: " );
//				DebugChatDef( hRes );
			}
		}
		else
		{
			HRESULT hRes = pChat->RequestPublicAction( szMessage );
			if( hRes != S_OK )
			{
//				debugprint( "                           RequestPublicAction() failed with: " );
//				DebugChatDef( hRes );
			}
		}
		char* szPrint = new char[ strlen( szMessage ) + strlen( szMyName ) + 10 ];
		if( !bAction )
		{
			sprintf( szPrint, "%s: %s", szMyName, szMessage );
			PrintMessage( szPrint, WOLCOLORREMAP_SELFSPEAKING );
		}
		else
		{
			sprintf( szPrint, "%s %s", szMyName, szMessage );
			PrintMessage( szPrint, WOLCOLORREMAP_ACTION );
			pChatSink->ActionEggSound( szMessage );
		}
		delete [] szPrint;
	}
}

//***********************************************************************************************
bool WolapiObject::ChannelCreate( const char* szChannelName, const char* szKey, bool bGame /* = false */, 
									int iMaxPlayers /* = 0 */, bool bTournament /* = false */, int iLobby /* = 0 */,
									CREATEGAMEINFO::GAMEKIND GameKind /* = red alert */ )
{
	//	Create a channel.
	//	szKey is NULL if a public channel is to be created, else channel password.

	//	Returns true if everything goes okay.

	if( pChatSink->bJoined )
	{
		//	This never happens. Here just in case.
//		debugprint( "WolapiObject::ChannelCreate called when bJoined is true!\n" );
		return false;
//		Fatal( "WolapiObject::ChannelCreate called when bJoined is true!" );
	}
	
	Channel ChannelNew;

	//	Prepare the struct.
	memset( &ChannelNew, 0, sizeof( ChannelNew ) );
	
	if( !bGame )
	{
		//	ChannelNew.type = 0;	0 for chat channel.
		strcpy( (char*)ChannelNew.name, szChannelName );
	}
	else
	{
		ChannelNew.type = GAME_TYPE;
		ChannelNew.maxUsers = iMaxPlayers;
		ChannelNew.tournament = bTournament;
		//	Channel 'reserved' stores GameKind in the highest byte, and
		//	lobby number to return to in the lower three bytes.
		//	Note: If lobby number is -1 (no lobby to return to), it's encoded as 0x00FFFFFF
		ChannelNew.reserved = ( iLobby & 0x00FFFFFF ) | GameKind;
		strcpy( (char*)ChannelNew.name, szChannelName );
	}

//	debugprint( "RequestChannelCreate(), channel name: '%s'\n", szChannelName );

	if( szKey )
		strcpy( (char*)ChannelNew.key, szKey );

	WWMessageBox().Process( TXT_WOL_WAIT, TXT_NONE );

	pChatSink->bRequestChannelCreateWait = true;
	
	HRESULT hRes = pChat->RequestChannelCreate( &ChannelNew );
	if( !SUCCEEDED( hRes ) )
	{
//		debugprint( "RequestChannelCreate() call failed:" );
		DebugChatDef( hRes );
		return false;
	}
	pChatSink->bIgnoreChannelLists = true;		//	Turn off response to channel lists.
	DWORD dwTimeStart = timeGetTime();
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	while( pChatSink->bRequestChannelCreateWait && timeGetTime() - dwTimeStart < EMERGENCY_TIMEOUT )
	{
		while( timeGetTime() < dwTimeNextPump )
			Call_Back();
		pChat->PumpMessages();
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	}
	pChatSink->bIgnoreChannelLists = false;

	if( pChatSink->bRequestChannelCreateWait || !pChatSink->bJoined )
		return false;		//	Timed out or callback got fail value.

	if( bGame )
		iLobbyReturnAfterGame = iLobby;

	return true;
}

//***********************************************************************************************
void WolapiObject::DoFindPage()
{
	//	User presses find/page button.
	SimpleEditDlgClass* pFindPageDlg;

	//	Ask user for user desired.
	Fancy_Text_Print( TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_TEXT );	//	Required before String_Pixel_Width() call, for god's sake.
	pFindPageDlg = new SimpleEditDlgClass( 400, TXT_WOL_PAGELOCATE, TXT_WOL_USERNAMEPROMPT, WOL_NAME_LEN_MAX );
	pFindPageDlg->SetButtons( TXT_WOL_LOCATE, Text_String( TXT_CANCEL ), TXT_WOL_PAGE );
	bPump_In_Call_Back = true;
	const char* szNameDlgResult = pFindPageDlg->Show();
	bPump_In_Call_Back = false;

	if( strcmp( szNameDlgResult, Text_String( TXT_CANCEL ) ) == 0 || !*pFindPageDlg->szEdit )
	{
		delete pFindPageDlg;
		return;
	}

	if( strcmp( szNameDlgResult, TXT_WOL_LOCATE ) == 0 )
	{
		//	Locate user.
		HRESULT hRes = Locate( pFindPageDlg->szEdit );
		switch( hRes )
		{
		case CHAT_S_FIND_NOTHERE:
			bPump_In_Call_Back = true;
			WWMessageBox().Process( TXT_WOL_FIND_NOTHERE );
			bPump_In_Call_Back = false;
			break;
		case CHAT_S_FIND_NOCHAN:
			bPump_In_Call_Back = true;
			WWMessageBox().Process( TXT_WOL_FIND_NOCHAN );
			bPump_In_Call_Back = false;
			break;
		case CHAT_S_FIND_OFF:
			bPump_In_Call_Back = true;
			WWMessageBox().Process( TXT_WOL_FIND_OFF );
			bPump_In_Call_Back = false;
			break;
		case CHAT_E_TIMEOUT:
			bPump_In_Call_Back = true;
			WWMessageBox().Process( TXT_WOL_TIMEOUT );
			bPump_In_Call_Back = false;
			break;
		case E_FAIL:
			GenericErrorMessage();
			break;
		case S_OK:
		{
			char* szChannel = (char*)pChatSink->OnFindChannel.name;
			int iLobby = iChannelLobbyNumber( (unsigned char*)szChannel );
			char* szFound;
			if( iLobby != -1 )
			{
				char szLobbyName[ REASONABLELOBBYINTERPRETEDNAMELEN ];
				InterpretLobbyNumber( szLobbyName, iLobby );
				szFound = new char[ strlen( TXT_WOL_FOUNDIN ) + strlen( szLobbyName ) + 5 ];
				sprintf( szFound, TXT_WOL_FOUNDIN, szLobbyName );
			}
			else
			{
				szFound = new char[ strlen( TXT_WOL_FOUNDIN ) + strlen( szChannel ) + 5 ];
				sprintf( szFound, TXT_WOL_FOUNDIN, szChannel );
			}
			bPump_In_Call_Back = true;
			WWMessageBox().Process( szFound );
			bPump_In_Call_Back = false;
			delete [] szFound;
			break;
		}
		}
	}
	else
	{
		//	Page user.
		//	Ask user for text to send.
		SimpleEditDlgClass* pMessDlg = new SimpleEditDlgClass( 600, TXT_WOL_PAGEMESSAGETITLE, 
																TXT_WOL_PAGEMESSAGEPROMPT, MAXCHATSENDLENGTH );
		bPump_In_Call_Back = true;
		if( strcmp( pMessDlg->Show(), Text_String( TXT_OK ) ) == 0 && *pMessDlg->szEdit )
		{
			switch( Page( pFindPageDlg->szEdit, pMessDlg->szEdit, true ) )
			{
			case CHAT_S_PAGE_NOTHERE:
				WWMessageBox().Process( TXT_WOL_PAGE_NOTHERE );
				break;
			case CHAT_S_PAGE_OFF:
				WWMessageBox().Process( TXT_WOL_PAGE_OFF );
				break;
			case CHAT_E_TIMEOUT:
				WWMessageBox().Process( TXT_WOL_TIMEOUT );
				break;
			case E_FAIL:
				GenericErrorMessage();
				break;
			case S_OK:
				char szMessage[ WOL_NAME_LEN_MAX + 30 ];
				sprintf( szMessage, TXT_WOL_WASPAGED, pFindPageDlg->szEdit );
				PrintMessage( szMessage, WOLCOLORREMAP_LOCALMACHINEMESS );
				break;
			}
		}
		bPump_In_Call_Back = false;
	}

	delete pFindPageDlg;
}

//***********************************************************************************************
HRESULT WolapiObject::Locate( const char* szUser )
{
	//	Returns HRESULT with possibly customized meanings.

	char* szMessage = new char[ strlen( TXT_WOL_LOCATING ) + strlen( szUser ) + 5 ];
	sprintf( szMessage, TXT_WOL_LOCATING, szUser );
	WWMessageBox().Process( szMessage, TXT_NONE );
	delete [] szMessage;

	pChatSink->bRequestFindWait = true;
	
	User userFind;
	strcpy( (char*)userFind.name, szUser );

//	debugprint( "RequestFind()\n" );
	if( !SUCCEEDED( pChat->RequestFind( &userFind ) ) )
	{
//		debugprint( "RequestFind() call failed\n" );
		return 0;
	}
	DWORD dwTimeStart = timeGetTime();
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	while( pChatSink->bRequestFindWait && timeGetTime() - dwTimeStart < EMERGENCY_TIMEOUT )
	{
		while( timeGetTime() < dwTimeNextPump )
			Call_Back();
		pChat->PumpMessages();
//		debugprint( ">Find pump\n" );
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	}

	if( pChatSink->bRequestFindWait )
		return CHAT_E_TIMEOUT;

	return pChatSink->hresRequestFindResult;
}

//***********************************************************************************************
HRESULT WolapiObject::Page( const char* szUser, const char* szSend, bool bWaitForResult )
{
	//	Returns HRESULT with possibly customized meanings.

	if( bWaitForResult )
	{
		char* szMessage = new char[ strlen( TXT_WOL_PAGING ) + strlen( szUser ) + 5 ];
		sprintf( szMessage, TXT_WOL_PAGING, szUser );
		WWMessageBox().Process( szMessage, TXT_NONE );
		delete [] szMessage;
	}

	pChatSink->bRequestPageWait = true;
	
	User userFind;
	strcpy( (char*)userFind.name, szUser );

//	debugprint( "RequestPage()\n" );
	if( !SUCCEEDED( pChat->RequestPage( &userFind, szSend ) ) )
	{
//		debugprint( "RequestPage() call failed\n" );
		return 0;
	}
	if( !bWaitForResult )
		return 0;
	DWORD dwTimeStart = timeGetTime();
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	while( pChatSink->bRequestPageWait && timeGetTime() - dwTimeStart < EMERGENCY_TIMEOUT )
	{
		while( timeGetTime() < dwTimeNextPump )
			Call_Back();
		pChat->PumpMessages();
//		debugprint( ">Page pump\n" );
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	}

	if( pChatSink->bRequestPageWait )
		return CHAT_E_TIMEOUT;

	return pChatSink->hresRequestPageResult;
}

//***********************************************************************************************
void WolapiObject::DoKick( IconListClass* pILUsersOrPlayers, bool bAndBan )
{
	//	Kick selected users.

	if( CurrentLevel != WOL_LEVEL_INCHATCHANNEL && CurrentLevel != WOL_LEVEL_INLOBBY && CurrentLevel != WOL_LEVEL_INGAMECHANNEL )
	{
		PrintMessage( TXT_WOL_YOURENOTINCHANNEL, WOLCOLORREMAP_LOCALMACHINEMESS );
		Sound_Effect( WOLSOUND_ERROR );
	}
	else if( !bChannelOwner )
	{
		PrintMessage( TXT_WOL_ONLYOWNERCANKICK, WOLCOLORREMAP_LOCALMACHINEMESS );
		Sound_Effect( WOLSOUND_ERROR );
	}
	else
	{
		int iFound = 0;
		for( int i = 0; i < pILUsersOrPlayers->Count(); i++ )
		{
			if( pILUsersOrPlayers->bItemIsMultiSelected( i ) )
			{
				User* pUser = (User*)pILUsersOrPlayers->Get_Item_ExtraDataPtr( i );
				if( pUser && strcmp( (char*)pUser->name, szMyName ) != 0 )			//	Don't kick yourself.
				{
					Kick( pUser );
					if( bAndBan )
						Ban( pUser );
					iFound++;
					if( iFound < 5 )
						Sound_Effect( (VocType)( VOC_SCREAM1 + ( rand() % 9 ) ) );
				}
			}
		}
		if( !iFound )
		{
			PrintMessage( TXT_WOL_NOONETOKICK, WOLCOLORREMAP_LOCALMACHINEMESS );
			Sound_Effect( WOLSOUND_ERROR );
		}
	}
}

//***********************************************************************************************
bool WolapiObject::Kick( User* pUserToKick )
{
	//	Returns false if something terrible happens.
//	debugprint( "RequestUserKick()\n" );
	if( !SUCCEEDED( pChat->RequestUserKick( pUserToKick ) ) )
	{
//		debugprint( "RequestUserKick() call failed\n" );
		return false;
	}
	return true;
}

//***********************************************************************************************
bool WolapiObject::Ban( User* pUserToKick )
{
	//	Returns false if something terrible happens.
//	debugprint( "RequestChannelBan()\n" );
	if( !SUCCEEDED( pChat->RequestChannelBan( (char*)pUserToKick->name, true ) ) )
	{
//		debugprint( "RequestUserKick() call failed\n" );
		return false;
	}
	return true;
}

//***********************************************************************************************
void WolapiObject::DoSquelch( IconListClass* pILUsersOrPlayers )
{
	//	Squelch/unsquelch selected users.
	bool bFound = false;
	for( int i = 0; i < pILUsersOrPlayers->Count(); i++ )
	{
		if( pILUsersOrPlayers->bItemIsMultiSelected( i ) )
		{
			User* pUser = (User*)pILUsersOrPlayers->Get_Item_ExtraDataPtr( i );
			if( pUser )
			{
				if( strcmp( (char*)pUser->name, szMyName ) != 0 )		//	Don't squelch yourself.
				{
					Squelch( pUser );
//					char szMess[ 150 ];
//					if( Squelch( pUser ) )
//						sprintf( szMess, TXT_WOL_USERISSQUELCHED, (char*)pUser->name );
//					else
//						sprintf( szMess, TXT_WOL_USERISNOTSQUELCHED, (char*)pUser->name );
//					WOL_PrintMessage( chatlist, szMess, WOLCOLORREMAP_LOCALMACHINEMESS );

					bFound = true;
					pILUsersOrPlayers->Flag_To_Redraw();
				}
				else
					PrintMessage( TXT_WOL_CANTSQUELCHSELF, WOLCOLORREMAP_LOCALMACHINEMESS );
			}
		}
	}
	if( bFound )
	{
		Sound_Effect( VOC_SQUISH );
		ListChannelUsers();		//	Refresh displayed user list.
	}
}

//***********************************************************************************************
bool WolapiObject::Squelch( User* pUserToSquelch )
{
	//	Returns true if user is now squelched, false if not squelched.
	//	Sets User pointer flags value.
//	debugprint( "Squelch:: pUser is %i, flags is %i\n", pUserToSquelch, pUserToSquelch->flags );

	if( pUserToSquelch->flags & CHAT_USER_SQUELCHED )
	{
		pChat->SetSquelch( pUserToSquelch, false );
		pUserToSquelch->flags &= ~CHAT_USER_SQUELCHED;
		return false;
	}
	pChat->SetSquelch( pUserToSquelch, true );
	pUserToSquelch->flags |= CHAT_USER_SQUELCHED;
	return true;
}

//***********************************************************************************************
void WolapiObject::DoOptions()
{
	//	Show options dialog.
	bPump_In_Call_Back = true;
	WOL_Options_Dialog( this, false );
	bPump_In_Call_Back = false;
	//	Set trigger for an immediate channel list update, in case local lobby games filter was changed.
	dwTimeNextChannelUpdate = ::timeGetTime();
}

//***********************************************************************************************
bool WolapiObject::DoLadder()
{
	bPump_In_Call_Back = true;
	if( WWMessageBox().Process( TXT_WOL_LADDERSHELL, TXT_YES, TXT_NO ) == 0 )
	{
		bPump_In_Call_Back = false;
#ifdef ENGLISH 
		//return SpawnBrowser( "http://www.westwood.com/ra_ladders.html" );
		return SpawnBrowser( "http://www.westwood.com/westwoodonline/tournaments/redalert/index.html" );
#else
#ifdef GERMAN
//		return SpawnBrowser( "http://www.westwood.com/ra_ladders_german.html" );
		return SpawnBrowser( "http://www.westwood.com/westwoodonline/tournaments/redalert/index.html" );
#else
//		return SpawnBrowser( "http://www.westwood.com/ra_ladders_french.html" );
		return SpawnBrowser( "http://www.westwood.com/westwoodonline/tournaments/redalert/index.html" );
#endif
#endif
	}

	bPump_In_Call_Back = false;
	return false;
}

//***********************************************************************************************
bool WolapiObject::DoHelp()
{
	bPump_In_Call_Back = true;
	if( WWMessageBox().Process( TXT_WOL_HELPSHELL, TXT_YES, TXT_NO ) == 0 )
	{
		bPump_In_Call_Back = false;
		const char* szURL;
		if( pChat->GetHelpURL( &szURL ) == S_OK )
			return SpawnBrowser( szURL );
		GenericErrorMessage();
	}

	bPump_In_Call_Back = false;
	return false;
}

//***********************************************************************************************
bool WolapiObject::DoWebRegistration()
{
	//	Get the executable name from the registry.
	HKEY hKey;
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, "SOFTWARE\\Westwood\\Register", 0, KEY_READ, &hKey ) != ERROR_SUCCESS )
	{
		GenericErrorMessage();
		return false;
	}
	char szPath[ _MAX_PATH + 1 ];
	DWORD dwBufSize = _MAX_PATH;
	if( RegQueryValueEx( hKey, "InstallPath", 0, NULL, (LPBYTE)szPath, &dwBufSize ) != ERROR_SUCCESS )
	{
		GenericErrorMessage();
		return false;
	}
	RegCloseKey( hKey );
//	debugprint( "Registration app is '%s'\n", szPath );

	bPump_In_Call_Back = true;
	if( WWMessageBox().Process( TXT_WOL_WEBREGISTRATIONSHELL, TXT_YES, TXT_NO ) == 0 )
	{
		bPump_In_Call_Back = false;
		::ShellExecute( NULL, "open", szPath, NULL, ".", SW_SHOW );
		return true;
	}

	bPump_In_Call_Back = false;
	return false;
}

//***********************************************************************************************
bool WolapiObject::DoGameAdvertising( const Channel* pChannel )
{
	const char* szURL = URLForGameType( pChannel->type );
	if( !szURL )
	{
		GenericErrorMessage();
		return false;
	}

	char szQuestion[512];
	sprintf( szQuestion, TXT_WOL_GAMEADVERTSHELL, NameOfGameType( pChannel->type ) );
	bPump_In_Call_Back = true;
	if( WWMessageBox().Process( szQuestion, TXT_YES, TXT_NO ) == 0 )
	{
		bPump_In_Call_Back = false;
		return SpawnBrowser( szURL );
	}

	bPump_In_Call_Back = false;
	return false;
}

//***********************************************************************************************
bool WolapiObject::SpawnBrowser( const char* szURL )
{
	//	Attempts to launch user's web browser, and monitors it, waiting for user to close it, at which
	//	point we bring focus back to the game.

	//	Loosely based on Dune2000 example.

	bool bSuccess = false;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);

	if( *szWebBrowser )
	{
		char szCommandLine[ _MAX_PATH + 300 ];
		sprintf( szCommandLine, "\"%s\" %s", szWebBrowser, szURL );
//		debugprint( "About to CreateProcess: '%s'\n", szCommandLine );
		Hide_Mouse();
		BlackPalette.Set( FADE_PALETTE_FAST, Call_Back );
//		::ShowWindow( MainWindow, SW_SHOWMINIMIZED );
		SeenPage.Clear();
		if(	::CreateProcess(	NULL,
								szCommandLine,	//	Command line.
								NULL,			//	Process handle not inheritable.
								NULL,			//	Thread handle not inheritable.
								FALSE,			//	Set handle inheritance to FALSE.
								0,				//	No creation flags.
								NULL,			//	Use parent’s environment block.
								NULL,			//	Use parent’s starting directory.
								&si,			//	Pointer to STARTUPINFO structure.
								&pi ) )			//	Pointer to PROCESS_INFORMATION structure.
		{
			if( pi.hProcess )
			{
//				debugprint( "CreateProcess: '%s'\n", szCommandLine );
				bSuccess = true;
				::WaitForInputIdle( pi.hProcess, 5000 );
				bPump_In_Call_Back = true;
				for( ; ; )
				{
					DWORD dwActive;
					Call_Back();
					Sleep( 200 );
					::GetExitCodeProcess( pi.hProcess, &dwActive );
					if( dwActive != STILL_ACTIVE || cancel_current_msgbox )
					{
						//	Either user closed the browser app, or game is starting and we should return focus to game.
						cancel_current_msgbox = false;
						::SetForegroundWindow( MainWindow );
						::ShowWindow( MainWindow, SW_RESTORE );
						break;
					}
					if( ::GetTopWindow( NULL ) == MainWindow )
					{
						::ShowWindow( MainWindow, SW_RESTORE );		//	In case it was topmost but minimized.
					   break;
					}
				}
				bPump_In_Call_Back = false;
				GamePalette.Set( FADE_PALETTE_FAST, Call_Back );
				Show_Mouse();
			}
		}
	}

	if( !bSuccess )
	{
		//	This was the old way - does not pop you back into game when finished...
		if( (int)::ShellExecute( NULL, NULL, szURL, NULL, ".", SW_SHOW ) <= 32 )
		{
//			debugprint( "ShellExecute\n" );
			//	ShellExecute failed as well. Just print a message instead.
			GamePalette.Set();
			::ShowWindow( MainWindow, SW_RESTORE );
			char szError[ 300 ];
			sprintf( szError, TXT_WOL_CANTLAUNCHBROWSER, szURL );
			Show_Mouse();
			WWMessageBox().Process( szError );
			return false;
		}
		//	(We return immediately after launching in this case.)
		GamePalette.Set();
		Show_Mouse();
	}
	return true;
}

//***********************************************************************************************
void WolapiObject::ChannelListTitle( const char* szTitle )
{
	strcpy( szChannelListTitle, szTitle );
	bChannelListTitleUpdated = true;
}

//***********************************************************************************************
bool WolapiObject::EnterLevel_Top()
{
	//	<Showing the top level choices.>

//	debugprint( "*** EnterLevel_Top\n" );
	//	(Might as well hardcode the channels tree.)

	ChannelListTitle( TXT_WOL_TOPLEVELTITLE );
	pILChannels->Clear();
	//void* pTopIcon = (void*)DibIconInfos[ DIBICON_ACCEPT ].pDIB;
	void* pTopIcon = IconForGameType( 0 );
	pILChannels->Add_Item( TXT_WOL_OFFICIALCHAT, CHANNELTYPE_OFFICIALCHAT, pTopIcon, ICON_DIB, CHANNELTYPE_OFFICIALCHAT );
	pILChannels->Add_Item( TXT_WOL_USERCHAT, CHANNELTYPE_USERCHAT, pTopIcon, ICON_DIB, CHANNELTYPE_USERCHAT );
	pILChannels->Add_Item( TXT_WOL_GAMECHANNELS, CHANNELTYPE_GAMES, pTopIcon, ICON_DIB, CHANNELTYPE_GAMES );

	//	Set wol buttons enabled/disabled.
	pShpBtnLeave->Disable();
	pShpBtnRefresh->Disable();
	pShpBtnSquelch->Disable();
	pShpBtnBan->Disable();
	pShpBtnKick->Disable();
	
	CurrentLevel = WOL_LEVEL_TOP;

	return true;
}

//***********************************************************************************************
bool WolapiObject::EnterLevel_OfficialChat()
{
	//	<Showing available official chat channels.>

//	debugprint( "*** EnterLevel_OfficialChat\n" );
	//	(Might as well hardcode the channels tree.)

	CurrentLevel = WOL_LEVEL_OFFICIALCHAT;
	if( !UpdateChannels( 0, CHANNELFILTER_OFFICIAL, false ) )
	{
		GenericErrorMessage();
		return false;
	}

	ChannelListTitle( TXT_WOL_OFFICIALCHAT );
	pILChannels->Clear();
	pILChannels->Add_Item( TXT_WOL_CHANNELLISTLOADING, CHANNELTYPE_LOADING, NULL, ICON_SHAPE, CHANNELTYPE_LOADING );
	dwTimeNextChannelUpdate = ::timeGetTime();		//	Set trigger for an immediate channel list update.

	//	Set wol buttons enabled/disabled.
	pShpBtnLeave->Disable();
	pShpBtnRefresh->Enable();
	pShpBtnSquelch->Disable();
	pShpBtnBan->Disable();
	pShpBtnKick->Disable();
	
	return true;
}

//***********************************************************************************************
bool WolapiObject::EnterLevel_UserChat()
{
	//	<Showing available user chat channels.>

//	debugprint( "*** EnterLevel_UserChat\n" );
	//	(Might as well hardcode the channels tree.)

	CurrentLevel = WOL_LEVEL_USERCHAT;
	if( !UpdateChannels( 0, CHANNELFILTER_UNOFFICIAL, false ) )
	{
		GenericErrorMessage();
		return false;
	}

	ChannelListTitle( TXT_WOL_USERCHAT );
	pILChannels->Clear();
	pILChannels->Add_Item( TXT_WOL_CHANNELLISTLOADING, CHANNELTYPE_LOADING, NULL, ICON_SHAPE, CHANNELTYPE_LOADING );
	dwTimeNextChannelUpdate = ::timeGetTime();		//	Set trigger for an immediate channel list update.

	//	Set wol buttons enabled/disabled.
	pShpBtnLeave->Disable();
	pShpBtnRefresh->Enable();
	pShpBtnSquelch->Disable();
	pShpBtnBan->Disable();
	pShpBtnKick->Disable();
	
	return true;
}

//***********************************************************************************************
bool WolapiObject::EnterLevel_Games()
{
	//	<Showing each westwood game type.>

//	debugprint( "*** EnterLevel_Games\n" );
	//	(Might as well hardcode the channels tree.)

	CurrentLevel = WOL_LEVEL_GAMES;

	ChannelListTitle( TXT_WOL_GAMECHANNELS );
	pILChannels->Clear();
	pILChannels->Add_Item( TXT_WOL_CHANNEL_TOP, CHANNELTYPE_TOP, NULL, ICON_SHAPE, CHANNELTYPE_TOP );

	//	Create entry for our lobbies at the top.
	bool bFound = false;
	//	(There are actually 2 additional game types at the end of GameTypeInfos - for ws icon and wwonline icon.)
	for( int i = 0; i < nGameTypeInfos - 2; i++ )
	{
		if( GameTypeInfos[ i ].iGameType == GAME_TYPE )
		{
			//pILChannels->Add_Item( GameTypeInfos[ i ].szName, CHANNELTYPE_LOBBIES, (void*)GameTypeInfos[ i ].pDIB, ICON_DIB, CHANNELTYPE_LOBBIES );
			pILChannels->Add_Item( TXT_WOL_REDALERTLOBBIES, CHANNELTYPE_LOBBIES, (void*)GameTypeInfos[ i ].pDIB, ICON_DIB, CHANNELTYPE_LOBBIES );
			bFound = true;
			break;
		}
	}
	if( !bFound )
	{
		//	In the production version, this should never happen, as there should always be a gametypeinfo created that matches
		//	our game type. It depends on the recentness of the WOR file accompanying the wolapi.dll.
		pILChannels->Add_Item( TXT_WOL_REDALERTLOBBIES, CHANNELTYPE_LOBBIES, (void*)OldRAGameTypeInfos[ 0 ].pDIB, ICON_DIB, CHANNELTYPE_LOBBIES );
	}

	//	A pointer to the GameTypeInfos entry is stored in the item for convenience later.
	for( i = 0; i < nGameTypeInfos - 2; i++ )
	{
		int iType = GameTypeInfos[ i ].iGameType;
		if( iType != GAME_TYPE )		//	Else it is our game - skip it here since we put it at the top.
		{
			if( iType != 2 && iType != 3 && iType != 4 )	//	Hack needed for the time being, to prevent the old ra games from being seen.
			{
				char szHelp[ 200 ];
				sprintf( szHelp, TXT_WOL_TTIP_CHANNELTYPE_GAMESOFTYPE, GameTypeInfos[ i ].szName );
				pILChannels->Add_Item( GameTypeInfos[ i ].szName, szHelp, (void*)GameTypeInfos[ i ].pDIB, ICON_DIB, CHANNELTYPE_GAMESOFTYPE, (void*)&GameTypeInfos[ i ] );
			}
		}
	}

	//	Set wol buttons enabled/disabled.
	pShpBtnLeave->Disable();
	pShpBtnRefresh->Disable();
	pShpBtnSquelch->Disable();
	pShpBtnBan->Disable();
	pShpBtnKick->Disable();
	
	return true;
}

//***********************************************************************************************
bool WolapiObject::EnterLevel_GamesOfType( WOL_GAMETYPEINFO* pGameTypeInfo )
{
	//	<Showing current game channels of a specific type - not our own game type.>

//	debugprint( "*** EnterLevel_GamesOfType: pGameTypeInfo->szName %s, iGameType %i, URL %s\n", pGameTypeInfo->szName, pGameTypeInfo->iGameType, pGameTypeInfo->szURL );

	CurrentLevel = WOL_LEVEL_GAMESOFTYPE;
	if( !UpdateChannels( pGameTypeInfo->iGameType, CHANNELFILTER_NO, true ) )
	{
		GenericErrorMessage();
		return false;
	}

	ChannelListTitle( pGameTypeInfo->szName );
	pILChannels->Clear();
	pILChannels->Add_Item( TXT_WOL_CHANNELLISTLOADING, CHANNELTYPE_LOADING, NULL, ICON_SHAPE, CHANNELTYPE_LOADING );
	dwTimeNextChannelUpdate = ::timeGetTime();		//	Set trigger for an immediate channel list update.

	//	Set wol buttons enabled/disabled.
	pShpBtnLeave->Disable();
	pShpBtnRefresh->Enable();
	pShpBtnSquelch->Disable();
	pShpBtnBan->Disable();
	pShpBtnKick->Disable();
	
	return true;
}

//***********************************************************************************************
bool WolapiObject::EnterLevel_Lobbies()
{
	//	<Showing available lobbies.>

//	debugprint( "*** EnterLevel_Lobbies\n" );

	CurrentLevel = WOL_LEVEL_LOBBIES;
	if( !UpdateChannels( 0, CHANNELFILTER_LOBBIES, false ) )
	{
		GenericErrorMessage();
		return false;
	}

	ChannelListTitle( TXT_WOL_REDALERTLOBBIES );
	pILChannels->Clear();
	pILChannels->Add_Item( TXT_WOL_CHANNELLISTLOADING, CHANNELTYPE_LOADING, NULL, ICON_SHAPE, CHANNELTYPE_LOADING );
	dwTimeNextChannelUpdate = ::timeGetTime();		//	Set trigger for an immediate channel list update.

	//	Set wol buttons enabled/disabled.
	pShpBtnLeave->Disable();
	pShpBtnRefresh->Enable();
	pShpBtnSquelch->Disable();
	pShpBtnBan->Disable();
	pShpBtnKick->Disable();
	
	return true;
}

//***********************************************************************************************
bool WolapiObject::OnEnteringChatChannel( const char* szChannelName, bool bICreatedChannel, int iLobby )
{
	//	Called when a chat channel (or lobby) has been successfully joined.
//	debugprint( "*** OnEnteringChatChannel '%s'\n", szChannelName );
	
//	//	Block until we have a userlist.		- Not necessary - always comes immediately following OnJoin.
//	if( !UserList() )
//		return false;

	//	Request ladders if this is a lobby.
	if( iLobby != -1 )
		RequestLadders( NULL );

	//	Set channels list.
	pILChannels->Clear();
	//	Add a "return" choice at the top of the channel list, based on where we want to go 'back' to...
	if( iLobby == -1 )
	{
		switch( CurrentLevel )
		{
		case WOL_LEVEL_OFFICIALCHAT:
			pILChannels->Add_Item( TXT_WOL_CHANNEL_BACK, CHANNELTYPE_OFFICIALCHAT, NULL, ICON_SHAPE, CHANNELTYPE_OFFICIALCHAT );
			break;
		case WOL_LEVEL_USERCHAT:
			pILChannels->Add_Item( TXT_WOL_CHANNEL_BACK, CHANNELTYPE_USERCHAT, NULL, ICON_SHAPE, CHANNELTYPE_USERCHAT );
			break;
		default:
			//	If entering a channel from anywhere else, user must have created the channel.
			//	Make "back" take them to user channels list.
//			if( bICreatedChannel )		//	ajw  just verifying
				pILChannels->Add_Item( TXT_WOL_CHANNEL_BACK, CHANNELTYPE_USERCHAT, NULL, ICON_SHAPE, CHANNELTYPE_USERCHAT );
/*			else
			{			
//				debugprint( "Case that should not occur in OnEnteringChatChannel. CurrentLevel %i\n", CurrentLevel );
				pILChannels->Add_Item( "ERROR in OnEnteringChatChannel", NULL, NULL, ICON_SHAPE, CHANNELTYPE_TOP );
			}
*/
			break;
		}
	}
	else
	{
		pILChannels->Add_Item( TXT_WOL_CHANNEL_BACK, CHANNELTYPE_LOBBIES, NULL, ICON_SHAPE, CHANNELTYPE_LOBBIES );
	}

	char* szMess;
	if( iLobby == -1 )
	{
		CurrentLevel = WOL_LEVEL_INCHATCHANNEL;
		szMess = new char[ strlen( TXT_WOL_YOUJOINED ) + strlen( szChannelName ) + 5 ];
		sprintf( szMess, TXT_WOL_YOUJOINED, szChannelName );
		ChannelListTitle( szChannelName );
	}
	else
	{
		CurrentLevel = WOL_LEVEL_INLOBBY;
		char szLobbyName[ REASONABLELOBBYINTERPRETEDNAMELEN ];
		InterpretLobbyNumber( szLobbyName, iLobby );
		szMess = new char[ strlen( TXT_WOL_YOUJOINEDLOBBY ) + REASONABLELOBBYINTERPRETEDNAMELEN + 10 ];
		sprintf( szMess, TXT_WOL_YOUJOINEDLOBBY, szLobbyName );
		ChannelListTitle( szLobbyName );
		iLobbyLast = iLobby;
		dwTimeNextChannelUpdate = ::timeGetTime();		//	Set trigger for an immediate channel list update.
	}

	strcpy( szChannelNameCurrent, szChannelName );

	bChannelOwner = bICreatedChannel;

	//	Set users list.
	ListChannelUsers();

	PrintMessage( szMess, WOLCOLORREMAP_LOCALMACHINEMESS );
	delete [] szMess;

	Sound_Effect( WOLSOUND_ENTERCHAN );

	//	Set wol buttons enabled/disabled.
	pShpBtnLeave->Enable();
	if( CurrentLevel == WOL_LEVEL_INLOBBY )
		pShpBtnRefresh->Enable();
	else
		pShpBtnRefresh->Disable();
	pShpBtnSquelch->Enable();
	if( bChannelOwner )
	{
		pShpBtnBan->Enable();
		pShpBtnKick->Enable();
	}
	else
	{
		pShpBtnBan->Disable();
		pShpBtnKick->Disable();
	}

	return true;
}

//***********************************************************************************************
void WolapiObject::OnExitingChatChannel()
{
	//	Called when we successfully ExitChannel, or we get kicked out. (Lobbies included.)

	//	Clear users list.
	pILUsers->Clear();
	if( pStaticUsers )
		pStaticUsers->Set_Text( TXT_WOL_NOUSERLIST );

//	debugprint( "*** OnExitingChatChannel() - szChannelNameCurrent '%s', CurrentLevel %i\n", szChannelNameCurrent, CurrentLevel );
	int iLobby = iChannelLobbyNumber( (unsigned char*)szChannelNameCurrent );
	char* szMess;
	if( iLobby == -1 )
	{
		szMess = new char[ strlen( TXT_WOL_YOULEFT ) + strlen( szChannelNameCurrent ) + 5 ];
		sprintf( szMess, TXT_WOL_YOULEFT, szChannelNameCurrent );
	}
	else
	{
		//	Channel is a lobby.
		char szLobbyName[ REASONABLELOBBYINTERPRETEDNAMELEN ];
		InterpretLobbyNumber( szLobbyName, iLobby );
		szMess = new char[ strlen( TXT_WOL_YOULEFTLOBBY ) + REASONABLELOBBYINTERPRETEDNAMELEN + 10 ];
		sprintf( szMess, TXT_WOL_YOULEFTLOBBY, szLobbyName );
	}
	PrintMessage( szMess, WOLCOLORREMAP_LOCALMACHINEMESS );
	delete [] szMess;

	*szChannelNameCurrent = 0;
	CurrentLevel = WOL_LEVEL_INVALID;

	Sound_Effect( WOLSOUND_EXITCHAN );
}

//***********************************************************************************************
bool WolapiObject::ExitChatChannelForGameChannel()
{
	//	We are about to try and join/create a game channel, and are currently in a chat channel.

	//	Save this channel name, so we can come back to it if game channel join/create fails.
	strcpy( szChannelReturnOnGameEnterFail, szChannelNameCurrent );

	if( !ChannelLeave() )
	{
		GenericErrorMessage();
		return false;
	}
	return true;
}

//***********************************************************************************************
bool WolapiObject::OnEnteringGameChannel( const char* szChannelName, bool bICreatedChannel, 
											const CREATEGAMEINFO& CreateGameInfo )
{
	//	Called when a game channel has been successfully joined, while still in chat dialog,
	//	before game dialog has been created.
	//	CreateGameInfo is copied to GameInfoCurrent, so that we know what kind of a game we're in during setup.

//	debugprint( "*** OnEnteringGameChannel() - %s\n", szChannelName );

	CurrentLevel = WOL_LEVEL_INGAMECHANNEL;
	strcpy( szChannelNameCurrent, szChannelName );

	bChannelOwner = bICreatedChannel;
//	GameKindCurrent = GameKind;
	GameInfoCurrent = CreateGameInfo;
	strcpy( GameInfoCurrent.szPassword, CreateGameInfo.szPassword );

	//	Remove shared buttons from wolchat's command list.
	pShpBtnDiscon->Zap();
	pShpBtnLeave->Zap();
	pShpBtnRefresh->Zap();
	pShpBtnSquelch->Zap();
	pShpBtnBan->Zap();
	pShpBtnKick->Zap();
	pShpBtnFindpage->Zap();
	pShpBtnOptions->Zap();
	pShpBtnLadder->Zap();
	pShpBtnHelp->Zap();

	//	Set wol buttons enabled/disabled.
	pShpBtnLeave->Enable();
	pShpBtnRefresh->Disable();
	pShpBtnSquelch->Enable();
	if( bChannelOwner )
	{
		pShpBtnBan->Enable();
		pShpBtnKick->Enable();
	}
	else
	{
		pShpBtnBan->Disable();
		pShpBtnKick->Disable();
	}

	if( CreateGameInfo.GameKind == CREATEGAMEINFO::AMGAME )
	{
		if( bShowRankRA )
		{
			//	Switch to "show AM rankings" mode.
			bShowRankRA = false;
			bMyRecordUpdated = true;
			bShowRankUpdated = true;
		}
	}
	else
	{
		if( !bShowRankRA )
		{
			//	Switch to "show RA rankings" mode.
			bShowRankRA = true;
			bMyRecordUpdated = true;
			bShowRankUpdated = true;
		}
	}

	return true;
}

//***********************************************************************************************
bool WolapiObject::OnEnteringGameSetup()
{
	//	Called when entering the game setup screen. Controls are initialized. OnEnteringGameChannel
	//	has just been called earlier.

	//	Returns false only if we find there is not host - he must have simultaneously left.

//	//	Block until we have a userlist.		- Not necessary - always comes immediately following OnJoin.
//	if( !UserList() )
//		return false;

	//	Request ladders.
	RequestLadders( NULL );

	//	Request IP addresses.
	RequestIPs( NULL );

	//	Set users list.
	if( !ListChannelUsers() )
	{
		//	No host was found currently in channel!
		return false;
	}

	if( !pGSupDlg->bHost )
	{
		char* szMess = new char[ strlen( TXT_WOL_YOUJOINEDGAME ) + WOL_NAME_LEN_MAX + 5 ];
		char szHostName[ WOL_NAME_LEN_MAX ];
		HostNameFromGameChannelName( szHostName, szChannelNameCurrent );
		sprintf( szMess, TXT_WOL_YOUJOINEDGAME, szHostName );
		PrintMessage( szMess, WOLCOLORREMAP_LOCALMACHINEMESS );
		delete [] szMess;
	}
	else
		PrintMessage( TXT_WOL_YOUCREATEDGAME, WOLCOLORREMAP_LOCALMACHINEMESS );

	return true;
}

//***********************************************************************************************
void WolapiObject::OnFailedToEnterGameChannel()
{
	if( *szChannelReturnOnGameEnterFail == 0 )
		return;

	//	This is called when we fail to join/create a game channel.
	*szChannelNameCurrent = 0;

	//	Because we don't save the channel key as well, assume the usual lobby password. If we fail, we'll return to top level.
	HRESULT hRes = ChannelJoin( szChannelReturnOnGameEnterFail, LOBBYPASSWORD );
	switch( hRes )
	{
	case S_OK:
		OnEnteringChatChannel( szChannelReturnOnGameEnterFail, false, iChannelLobbyNumber( (unsigned char*)szChannelReturnOnGameEnterFail ) );
		break;
	default:
		//	ChannelJoin returned fail value.
		//	(Now only applies if you could ever enter a game channel from a non-lobby.)
		//	There is the possibility that the channel we were in disappeared in the instant between leaving it and
		//	failing to join the game channel. <sigh> Or, the channel has a password, that we didn't record. In either
		//	case, go back to the top level.
		GenericErrorMessage();
		EnterLevel_Top();
	}
}

//***********************************************************************************************
void WolapiObject::OnExitingGameChannel()
{
	//	This is called after we leave a game channel, while still in the game setup dialog.

	//	Remove shared buttons from wolgsup's command list.
	pShpBtnDiscon->Zap();
	pShpBtnLeave->Zap();
	pShpBtnRefresh->Zap();
	pShpBtnSquelch->Zap();
	pShpBtnBan->Zap();
	pShpBtnKick->Zap();
	pShpBtnFindpage->Zap();
	pShpBtnOptions->Zap();
	pShpBtnLadder->Zap();
	pShpBtnHelp->Zap();

	CurrentLevel = WOL_LEVEL_INVALID;
	*szChannelNameCurrent = 0;
}

//***********************************************************************************************
void WolapiObject::RejoinLobbyAfterGame()
{
	//	Called to rejoin lobby after EITHER a game, or the game setup dialog.
//debugprint( "RejoinLobbyAfterGame, iLobbyReturnAfterGame is %i\n", iLobbyReturnAfterGame );

	if( iLobbyReturnAfterGame == -1 )
	{
		//	Will never happen presumably, if games are always entered via a lobby chat channel.
		//	We will naturally reenter the top level.
	}
	else
	{
		char szChannelToJoin[ WOL_CHANNAME_LEN_MAX ];
		//sprintf( szChannelToJoin, "Lob_%i_%i", GAME_TYPE, iLobbyReturnAfterGame );
		sprintf( szChannelToJoin, "%s%i", LOB_PREFIX, iLobbyReturnAfterGame );
//debugprint( "RejoinLobbyAfterGame, channel is %s\n", szChannelToJoin );

		HRESULT hRes = ChannelJoin( szChannelToJoin, LOBBYPASSWORD );
		switch( hRes )
		{
		case S_OK:
			//OnEnteringChatChannel( szChannelToJoin, false );		Done automatically now in wol_chat.
			break;
		default:
			//	Something went wrong when trying to rejoin the lobby we were in.
			//	We'll go back to the top level instead, which happens automatically if we do this...
			iLobbyReturnAfterGame = -1;
			break;
		}
	}
}

//***********************************************************************************************
bool WolapiObject::RequestLadders( const char* szName )
{
	//	If szName is NULL, calls RequestLadderList() until all ladder structs for all users in pChatSink's current 
	//	list have been asked for. Does not wait for results - these come in asynchronously. The previous list is
	//	erased before new results come in.
	//	If szName is valid, asks for specific name only. Result is appended to current ladder list.
	//	This function does not block.

	if( szName && *szName )
	{
//		debugprint( "RequestLadderList( %s )\n", szName );
		if( !SUCCEEDED( pNetUtil->RequestLadderList( szLadderServerHost, iLadderServerPort, szName, LADDER_CODE_RA, -1, 0, 0 ) ) )
		{
//			debugprint( "RequestLadderList() call failed\n" );
			return false;
		}
		if( !SUCCEEDED( pNetUtil->RequestLadderList( szLadderServerHost, iLadderServerPort, szName, LADDER_CODE_AM, -1, 0, 0 ) ) )
		{
//			debugprint( "RequestLadderList() call failed\n" );
			return false;
		}
		return true;
	}

	char szNames[ ( WOL_NAME_LEN_MAX + 1 ) * 30 ];			//	Neal says max is actually 25 names requested at once. Do 24...

	pNetUtilSink->DeleteLadderList();

	//	Do not request more than this number of times, to prevent overloads to ladder server.
	//	If we have that many people in the channel, forget about doing ladders for all of them.
	//	Probably this will never come into play (except while testing), because lobbies will be limited in # of users.
	int iCallLimit = 4;

	User* pUser = pChatSink->pUserList;
	while( pUser )
	{
		//	Reset names string.
		*szNames = 0;
		//	Get 24 users from list and add names to string.
		for( int i = 0; i != 24; ++i )
		{
			strcat( szNames, (char*)pUser->name );
			strcat( szNames, ":" );
			pUser = pUser->next;
			if( !pUser )
				break;
		}
		//	Remove last colon.
		szNames[ strlen( szNames ) - 1 ] = 0;

//		debugprint( "RequestLadderList( %s )\n", szNames );
		if( !SUCCEEDED( pNetUtil->RequestLadderList( szLadderServerHost, iLadderServerPort, szNames, LADDER_CODE_RA, -1, 0, 0 ) ) )
		{
//			debugprint( "RequestLadderList() call failed\n" );
			return false;
		}
		if( !SUCCEEDED( pNetUtil->RequestLadderList( szLadderServerHost, iLadderServerPort, szNames, LADDER_CODE_AM, -1, 0, 0 ) ) )
		{
//			debugprint( "RequestLadderList() call failed\n" );
			return false;
		}
		if( --iCallLimit == 0 )
			return false;
	}
	return true;
}

//***********************************************************************************************
bool WolapiObject::RequestIPs( const char* szName )
{
	//	If szName is NULL, calls RequestUserIP() until IPs for all users in pChatSink's current 
	//	list have been asked for. Does not wait for results - these come in asynchronously. The previous list is
	//	erased before new results come in.
	//	If szName is valid, asks for specific name only. Result is appended to current IP list.
	//	This function does not block.

	if( szName && *szName )
	{
		User user;
		strcpy( (char*)user.name, szName );
//		debugprint( "RequestUserIP( %s )\n", szName );
		if( !SUCCEEDED( pChat->RequestUserIP( &user ) ) )
		{
//			debugprint( "RequestUserIP() call failed\n" );
			return false;
		}
		return true;
	}

	//	Do all users in current chatsink list.

	pChatSink->DeleteUserIPList();	//	Clear old user IPs. (To keep searches fast, if we go in and out of game channels a lot.)

	User* pUser = pChatSink->pUserList;
	while( pUser )
	{
		if( !( pUser->flags & CHAT_USER_MYSELF ) )
		{
			if( !SUCCEEDED( pChat->RequestUserIP( pUser ) ) )
			{
//				debugprint( "RequestUserIP() call failed\n" );
				return false;
			}
		}
		pUser = pUser->next;
	}
	return true;
}

//***********************************************************************************************
void WolapiObject::SaveChat()
{
	//	Basically, a big hack to avoiding restructuring things so that the dialogs are persistent
	//	objects. Save the contents of the chat list in the chat dialog so that we can refresh it
	//	after returning from the game setup dialog (if necessary).
	//	This turns out to be the easiest and most straightforward way to implement this.
	pChatSaveLast = pChatSaveList = NULL;
	CHATSAVE* pChatSaveNew;

	for( int i = 0; i != pILChat->Count(); i++ )
	{
		pChatSaveNew = new CHATSAVE;
		const char* szItem = pILChat->Get_Item( i );
		if( strlen( szItem ) < SAVECHATWIDTH )
			strcpy( pChatSaveNew->szText, szItem );
		const IconList_ItemExtras* pItemExtras = pILChat->Get_ItemExtras( i );
		pChatSaveNew->ItemExtras.pColorRemap = pItemExtras->pColorRemap;			
		pChatSaveNew->next = NULL;

		if( pChatSaveLast )
			pChatSaveLast->next = pChatSaveNew;
		else
			pChatSaveList = pChatSaveNew;
		pChatSaveLast = pChatSaveNew;
	}
}

//***********************************************************************************************
void WolapiObject::RestoreChat()
{
	//	See SaveChat()...
	CHATSAVE* pChatSave = pChatSaveList;
	while( pChatSave )
	{
		PrintMessage( pChatSave->szText, pChatSave->ItemExtras.pColorRemap );
		pChatSave = pChatSave->next;
	}
}

//***********************************************************************************************
void WolapiObject::AddHostLeftMessageToSavedChat( const char* szName )
{
	CHATSAVE* pChatSaveNew;
	pChatSaveNew = new CHATSAVE;
	sprintf( pChatSaveNew->szText, TXT_WOL_HOSTLEFTGAME, szName );
	pChatSaveNew->ItemExtras.pColorRemap = &ColorRemaps[ WOLCOLORREMAP_LOCALMACHINEMESS ];
	pChatSaveNew->next = NULL;
	if( pChatSaveLast )
		pChatSaveLast->next = pChatSaveNew;
	else
		pChatSaveList = pChatSaveNew;
	pChatSaveLast = pChatSaveNew;
}

//***********************************************************************************************
void WolapiObject::AddMessageToSavedChat( const char* szMessage )
{
	CHATSAVE* pChatSaveNew;
	pChatSaveNew = new CHATSAVE;
	strcpy( pChatSaveNew->szText, szMessage );
	pChatSaveNew->ItemExtras.pColorRemap = &ColorRemaps[ WOLCOLORREMAP_LOCALMACHINEMESS ];
	pChatSaveNew->next = NULL;
	if( pChatSaveLast )
		pChatSaveLast->next = pChatSaveNew;
	else
		pChatSaveList = pChatSaveNew;
	pChatSaveLast = pChatSaveNew;
}

//***********************************************************************************************
void WolapiObject::DeleteSavedChat()
{
	//	See SaveChat()...
	CHATSAVE* pChatSaveNext;
	while( pChatSaveList )
	{
		pChatSaveNext = pChatSaveList->next;
		delete pChatSaveList;
		pChatSaveList = pChatSaveNext;
	}
}

//***********************************************************************************************
void WolapiObject::GenericErrorMessage()
{
	//	Displays generic "something bad happened" error message.
	bPump_In_Call_Back = true;
	WWMessageBox().Process( TXT_WOL_ERRORMESSAGE );
	bPump_In_Call_Back = false;
}

//***********************************************************************************************
bool WolapiObject::GetNameOfBeginningLobby( char* szNameToSet )
{
	//	Checks for game lobbies, sets szNameToSet to the channel name that the new user should enter and returns true if succeeds.
	if( !GetLobbyChannels() )
		return false;

	//	Chatsink should now have a list of lobbies.
	int iCount = 0;
	Channel* pChannel = pChatSink->pChannelList;
	if( !pChannel )
		//	List is empty.
		return false;

	//	Return the name of the first lobby with less than 50 users.
	while( pChannel )
	{
		if( pChannel->currentUsers < 50 )
		{
			strcpy( szNameToSet, (char*)pChannel->name );
			return true;
		}
		++iCount;
		pChannel = pChannel->next;
	}

	//	All lobbies have 50 or more users. So just choose a random one.
	int iChoice = ( rand() % iCount );
	pChannel = pChatSink->pChannelList;
	for( int i = 0; i != iChoice; i++ )
		pChannel = pChannel->next;

	strcpy( szNameToSet, (char*)pChannel->name );

	return true;
}

//***********************************************************************************************
bool WolapiObject::GetLobbyChannels()
{
	//	Modal version of UpdateChannels, for fetching lobby names.

//	//	Returns false upon total failure.	ajxxx do same for other calls
//	WWMessageBox().Process( TXT_WOL_WAIT, TXT_NONE );

	pChatSink->bRequestChannelListForLobbiesWait = true;
	pChatSink->ChannelFilter = CHANNELFILTER_LOBBIES;

//	debugprint( "RequestChannelList() for lobbies\n" );
	if( !SUCCEEDED( pChat->RequestChannelList( 0, false ) ) )
	{
//		debugprint( "RequestChannelList() call failed\n" );
		return false;
	}

	DWORD dwTimeStart = timeGetTime();
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	while( pChatSink->bRequestChannelListForLobbiesWait && timeGetTime() - dwTimeStart < EMERGENCY_TIMEOUT )
	{
		while( timeGetTime() < dwTimeNextPump )
			Call_Back();
		pChat->PumpMessages();
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	}

	if( pChatSink->bRequestChannelListForLobbiesWait )
		return false;

	return true;
}

//***********************************************************************************************
const char* WolapiObject::pGameHostName()
{
	//	Returns a POINTER (careful - temporary!) to the name of the creator of the game channel we're in, or null.
	//	Uses players' list as its means of reference.
	if( pILPlayers )
	{
		for( int i = 0; i != pILPlayers->Count(); i++ )
		{
			User* pUser = (User*)pILPlayers->Get_Item_ExtraDataPtr( i );
			if( pUser && pUser->flags & CHAT_USER_CHANNELOWNER )
				return (char*)pUser->name;
		}
	}
	return NULL;
}

//***********************************************************************************************
User* WolapiObject::pGameHost()
{
	//	Returns a POINTER (careful - temporary!) to the creator of the game channel we're in, or null.
	//	Uses players' list as its means of reference.
	if( pILPlayers )
	{
		for( int i = 0; i != pILPlayers->Count(); i++ )
		{
			User* pUser = (User*)pILPlayers->Get_Item_ExtraDataPtr( i );
			if( pUser && pUser->flags & CHAT_USER_CHANNELOWNER )
				return pUser;
		}
	}
	return NULL;
}

//***********************************************************************************************
bool WolapiObject::SendGameOpt( const char* szSend, User* pUserPriv )
{
	//	Used during game setup to send public or private game options string.
	//	If pUserPriv is NULL, message is public, else private to pUserPriv.
	if( !pUserPriv )
	{
//		debugprint( "Send public game opt: '%s'\n", szSend );
		if( !SUCCEEDED( pChat->RequestPublicGameOptions( szSend ) ) )
		{
//			debugprint( "RequestPublicGameOptions() call failed\n" );
			return false;
		}
	}
	else
	{
//		debugprint( "Send private game opt to %s: '%s'\n", (char*)pUserPriv->name, szSend );
		if( !SUCCEEDED( pChat->RequestPrivateGameOptions( pUserPriv, szSend ) ) )
		{
//			debugprint( "RequestPrivateGameOptions() call failed\n" );
			return false;
		}
	}
	return true;
}

//***********************************************************************************************
bool WolapiObject::RequestGameStart()
{
	//	Host is starting a game.

/*
	//	Block any users that join the channel in the next microsecond from becoming involved, and
	//	block any users that leave from being recognized as having left.
	//what if someone leaves?
	//	This is done to preserve the integrity of the ChatSink's user list
	pWO->pChatSink->bIgnoreJoin = true;
*/
	pChatSink->bRequestGameStartWait = true;

//	debugprint( "RequestGameStart()\n" );
	if( !SUCCEEDED( pChat->RequestGameStart( pChatSink->pUserList ) ) )
	{
//		debugprint( "RequestGameStart() call failed\n" );
		return false;
	}

	DWORD dwTimeStart = timeGetTime();
	DWORD dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	while( pChatSink->bRequestGameStartWait && timeGetTime() - dwTimeStart < EMERGENCY_TIMEOUT )
	{
		while( timeGetTime() < dwTimeNextPump )
			Call_Back();
		pChat->PumpMessages();
		dwTimeNextPump = timeGetTime() + PUMPSLEEPDURATION;
	}

	if( pChatSink->bRequestGameStartWait )
	{
//		debugprint( "WolapiObject::RequestGameStart returning false\n" );
		pChatSink->bRequestGameStartWait = false;
		return false;
	}

//	debugprint( "WolapiObject::RequestGameStart returning true\n" );
	return true;
}

//***********************************************************************************************
bool WolapiObject::SendGo( const char* szSend )
{
	//	Send a "GO" message to all players included in the list that came back from OnGameStart.
	//	(Don't just broadcast it. We don't want to include any users that may have joined the channel
	//	in the last microsecond.)
//	debugprint( "SendGo()\n" );

	User* pUser = pChatSink->pGameUserList;

	while( pUser )
	{
//		if( !( pUser->flags & CHAT_USER_MYSELF ) )			Method changed. I now wait for go message to bounce back to me.
//		{
//			debugprint( "Send private game opt to %s: '%s'\n", (char*)pUser->name, szSend );
			if( !SUCCEEDED( pChat->RequestPrivateGameOptions( pUser, szSend ) ) )
			{
//				debugprint( "RequestPrivateGameOptions() call failed\n" );
				return false;
			}
//		}
		pUser = pUser->next;
	}
	return true;
}

//***********************************************************************************************
void WolapiObject::Init_DisconnectPinging()
{
	//	Sets us up to begin "disconnect pinging" - the pinging that occurs when connection is broken
	//	during a tournament game. The idea is to try and figure out who is responsible for the connection
	//	going down. We do this by repeatedly pinging the opponent and the game results server. The number
	//	of successful pings is sent in the game results package.
	iDisconnectPingCurrent = 0;
	for( int i = 0; i != DISCONNECT_PING_COUNT; ++i )
	{
		DisconnectPingResult_Server[ i ] = PING_UNSTARTED;
		DisconnectPingResult_Opponent[ i ] = PING_UNSTARTED;
	}
	bDisconnectPingingCompleted = false;
	bDoingDisconnectPinging = true;
}

//***********************************************************************************************
bool WolapiObject::Pump_DisconnectPinging()
{
	//	Called repeatedly and continuously when it seems a tournament game connection with the opponent
	//	has been broken. Does PumpMessages() and requests new pings when previous results have been received.
	//	Returns true when the required number of pings have been completed.

	if( ::timeGetTime() > dwTimeNextWolapiPump )
	{
		pChat->PumpMessages();
		pNetUtil->PumpMessages();
		dwTimeNextWolapiPump = ::timeGetTime() + WOLAPIPUMPWAIT;
	}

	switch( DisconnectPingResult_Server[ iDisconnectPingCurrent ] )
	{
	case PING_UNSTARTED:
		//	Pings have yet to be requested.
		//	Ping game results server.
		int iUnused;
		if( *szGameResServerHost1 )
		{
//			debugprint( "RequestPing ( gameres server )\n" );
			if( pNetUtil->RequestPing( szGameResServerHost1, 1000, &iUnused ) != S_OK )
			{
//				debugprint( "RequestPing() ( gameres server ) failed\n" );
				DisconnectPingResult_Server[ iDisconnectPingCurrent ] = PING_BAD;
			}
			DisconnectPingResult_Server[ iDisconnectPingCurrent ] = PING_WAITING;
		}
		else
			//	We never got an address for the gameresults server. Fake fail result.
			DisconnectPingResult_Server[ iDisconnectPingCurrent ] = PING_BAD;

		//	Ping opponent.
		in_addr inaddr;
		char* szIP;
		inaddr.s_addr = TournamentOpponentIP;
		szIP = inet_ntoa( inaddr );
//		debugprint( "RequestPing ( opponent )\n" );
		if( pNetUtil->RequestPing( szIP, 1000, &iUnused ) != S_OK )
		{
//			debugprint( "RequestPing() ( opponent ) failed\n" );
			DisconnectPingResult_Opponent[ iDisconnectPingCurrent ] = PING_BAD;
		}
		else
			DisconnectPingResult_Opponent[ iDisconnectPingCurrent ] = PING_WAITING;
		break;
	case PING_WAITING:
		//	Ping results still pending. (Callback will set vars when results arrive.)
		break;
	default:
		//	Ping result for server is in.
		if( DisconnectPingResult_Opponent[ iDisconnectPingCurrent ] == PING_WAITING )
			break;
		//	Both results are in. Begin new ping, or end disconnect pinging.
		iDisconnectPingCurrent++;
		if( iDisconnectPingCurrent == DISCONNECT_PING_COUNT )
		{
			bDisconnectPingingCompleted = true;
			bDoingDisconnectPinging = false;
			return true;
		}
		break;
	}
	return false;
}

//***********************************************************************************************
void WolapiObject::DisconnectPingResultsString( char* szStringToSet )
{
	int iGoodServerPings = 0;
	int iGoodPlayerPings = 0;
	for( int i = 0; i < DISCONNECT_PING_COUNT; ++i )
	{
		if( DisconnectPingResult_Server[ i ] == PING_GOOD )			++iGoodServerPings;
		if( DisconnectPingResult_Opponent[ i ] == PING_GOOD )		++iGoodPlayerPings;
	}

	sprintf( szStringToSet, "%1u/%1u %1u/%1u", iGoodServerPings, DISCONNECT_PING_COUNT, iGoodPlayerPings, DISCONNECT_PING_COUNT );
}

//***********************************************************************************************
void WolapiObject::SetOptionDefaults()
{
	//	Get stored defaults for options.
	HKEY hKey;
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, Game_Registry_Key(), 0, KEY_READ, &hKey ) == ERROR_SUCCESS )
	{
		DWORD dwValue;
		DWORD dwBufSize = sizeof( DWORD );
		if( RegQueryValueEx( hKey, "WOLAPI Find Enabled", 0, NULL, (LPBYTE)&dwValue, &dwBufSize ) != ERROR_SUCCESS )
			bFindEnabled = true;
		else
			bFindEnabled = (bool)dwValue;
		if( RegQueryValueEx( hKey, "WOLAPI Page Enabled", 0, NULL, (LPBYTE)&dwValue, &dwBufSize ) != ERROR_SUCCESS )
			bPageEnabled = true;
		else
			bPageEnabled = (bool)dwValue;
		if( RegQueryValueEx( hKey, "WOLAPI Lang Filter", 0, NULL, (LPBYTE)&dwValue, &dwBufSize ) != ERROR_SUCCESS )
			bLangFilter = true;
		else
			bLangFilter = (bool)dwValue;
		if( RegQueryValueEx( hKey, "WOLAPI Show All Games", 0, NULL, (LPBYTE)&dwValue, &dwBufSize ) != ERROR_SUCCESS )
			bAllGamesShown = true;
		else
			bAllGamesShown = (bool)dwValue;

		RegCloseKey( hKey );
	}
	pChat->SetFindPage( bFindEnabled, bPageEnabled );
	pChat->SetLangFilter( bLangFilter );
}

//***********************************************************************************************
void WolapiObject::SetOptions( bool bEnableFind, bool bEnablePage, bool bLangFilterOn, bool bShowAllGames )
{
	//	Set options and remember them in registry.

	bFindEnabled = bEnableFind;
	bPageEnabled = bEnablePage;
	bLangFilter = bLangFilterOn;
	bAllGamesShown = bShowAllGames;

	HKEY hKey;
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, Game_Registry_Key(), 0, KEY_WRITE, &hKey ) == ERROR_SUCCESS )
	{
		DWORD dwValue = bFindEnabled ? 1 : 0;
		RegSetValueEx( hKey, "WOLAPI Find Enabled", 0, REG_DWORD, (LPBYTE)&dwValue, sizeof( dwValue ) );
		dwValue = bPageEnabled ? 1 : 0;
		RegSetValueEx( hKey, "WOLAPI Page Enabled", 0, REG_DWORD, (LPBYTE)&dwValue, sizeof( dwValue ) );
		dwValue = bLangFilter ? 1 : 0;
		RegSetValueEx( hKey, "WOLAPI Lang Filter", 0, REG_DWORD, (LPBYTE)&dwValue, sizeof( dwValue ) );
		dwValue = bAllGamesShown ? 1 : 0;
		RegSetValueEx( hKey, "WOLAPI Show All Games", 0, REG_DWORD, (LPBYTE)&dwValue, sizeof( dwValue ) );

		RegCloseKey( hKey );
	}
	pChat->SetFindPage( bFindEnabled, bPageEnabled );
	pChat->SetLangFilter( bLangFilter );
}

//***********************************************************************************************
HPALETTE GetCurrentScreenPalette()
{
	//	Get the palette of the current screen.
	//	Returns 0 if can't get palette.
	//	Remember to DeleteObject the HPALETTE if non-zero.
	GraphicViewPortClass draw_window(	LogicPage->Get_Graphic_Buffer(),
										WindowList[WINDOW_MAIN][WINDOWX] + LogicPage->Get_XPos(),
										WindowList[WINDOW_MAIN][WINDOWY] + LogicPage->Get_YPos(),
										WindowList[WINDOW_MAIN][WINDOWWIDTH],
										WindowList[WINDOW_MAIN][WINDOWHEIGHT] );
	LPDIRECTDRAWSURFACE lpDDS = draw_window.Get_Graphic_Buffer()->Get_DD_Surface();
	LPDIRECTDRAWPALETTE lpDDP;
	HPALETTE hPal = 0;
	if( lpDDS->GetPalette( &lpDDP ) == DD_OK )
	{
		PALETTEENTRY pe[256];
		if( lpDDP->GetEntries( 0, 0, 256, pe ) == DD_OK )
		{
			LOGPALETTE* pLogPal = (LOGPALETTE*)new char[ sizeof( LOGPALETTE ) + sizeof( PALETTEENTRY ) * 255 ];
			pLogPal->palVersion = 0x300;
			pLogPal->palNumEntries = 256;
			for( int i = 0; i != 256; i++ )
			{
				pLogPal->palPalEntry[i].peRed = pe[i].peRed;
				pLogPal->palPalEntry[i].peGreen = pe[i].peGreen;
				pLogPal->palPalEntry[i].peBlue = pe[i].peBlue;
				pLogPal->palPalEntry[i].peFlags = 0;
//				debugprint( "DD Palette %03u: %03u, %03u, %03u\n", i, pe[i].peRed, pe[i].peGreen, pe[i].peBlue );
			}
			hPal = CreatePalette( pLogPal );
			delete [] pLogPal;
		}
//		else
//			debugprint( "DD GetEntries failed.\n" );
	}
//	else
//		debugprint( "DD GetPalette failed.\n" );
	
	return hPal;
}

//***********************************************************************************************
void RemapDIBToPalette( HPALETTE hPal, const char* pDIB )		//	Note: pDIB is treated as non-const.
{
	//	Converts pDIB's actual pixel data to proper values for a different palette (hPal).
	//	Obeys convention that index 0 in the DIB's palette should map to transparent. For our purposes, make it black.
	
	//	Set the values of the qNewPalette array to hold the new destination palette index we want 
	//	a bmp palette entry to map to.
	unsigned char qNewPalette[ 256 ];

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)pDIB;

	RGBQUAD* pRGBEntry = (RGBQUAD*)( (char*)lpbi + lpbi->biSize );		//	This now points to the first entry in the bmp's palette table.

//	debugprint( "Starting rgbquads at %i\n", pRGBEntry );

	//	Index zero is supposed to be transparent. In our case, that means make it black.
	qNewPalette[0] = GetNearestPaletteIndex( hPal, RGB( 0, 0, 0 ) );
	pRGBEntry++;

	for( int i = 1; i != 256; i++ )
	{
		qNewPalette[i] = GetNearestPaletteIndex( hPal, RGB( pRGBEntry->rgbRed, pRGBEntry->rgbGreen, pRGBEntry->rgbBlue ) );
//		if( iIndex == 1 )
//			debugprint( "Remapping bmp %03u to new %03u\n", i, qNewPalette[i] );
		pRGBEntry++;
	}

	//	Convert the data to values that match game palette.
	int iWidth = DIBWidth( pDIB );
	int iHeight = DIBHeight( pDIB );
	int iSrcPitch = ( iWidth + 3 ) & ~3;
	int iLength = iSrcPitch * iHeight;
	unsigned char* pBits = (unsigned char*)FindDIBBits( pDIB );
//	debugprint( "First Byte value %03u will become %03u\n", *pBits, qNewPalette[ *pBits ] );
	for( i = 0; i != iLength; i++ )
	{
		*pBits++ = qNewPalette[ *pBits ];
	}
}

/*
//***********************************************************************************************
char* LoadFileIntoMemory( const char* szFileName, int& iLength )
{
	//	Loads a file into a buffer.
	//	Delete[] the pointer when you're done with the buffer.
	HANDLE hFile;
	hFile = CreateFile( szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return NULL;
	iLength = GetFileSize( hFile, NULL );
	char* pData = new char[ iLength ];
	DWORD dwBytesRead;
	ReadFile( hFile, pData, iLength, &dwBytesRead, NULL );
	if( dwBytesRead != iLength )
//		debugprint( "######LoadFileIntoMemory expected %i bytes and got %i\n", iLength, dwBytesRead );
	CloseHandle( hFile );
	return pData;
}
*/

//***********************************************************************************************
void HostNameFromGameChannelName( char* szNameToSet, const char* szChannelName )
{
	int iApostrophe = strcspn( szChannelName, "'" );
	memcpy( szNameToSet, szChannelName, iApostrophe );
	szNameToSet[ iApostrophe ] = 0;
}

#endif
