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

//	WolapiOb.h
//	ajw 07/10/98

//	Class WolapiObject is mainly a container so I can avoid globals and keep things clean.
//	All WOLAPI interfacing will be done through this object. It's lifetime will begin when
//	API functions are first needed and end when we are finished with the API - this will
//	presumably parallel the duration of the user's connection to WOL.

#ifndef WOLAPI_H
#define WOLAPI_H

#include "RAWolapi.h"
#include "dibapi.h"
#include "IconList.h"

//***********************************************************************************************
class IconListClass;
class WOL_GameSetupDialog;
class ToolTipClass;

#define PUMPSLEEPDURATION	300			//	Milliseconds between PumpMessages() calls.
#define EMERGENCY_TIMEOUT	40000		//	Longest we wait for a wolapi response before terminating everything.

//	Milliseconds between automatic behaviors.
#define WOLAPIPUMPWAIT				300
#define CHANNELUPDATEWAIT			45000

#define WOL_NAME_LEN_MAX		10			//	Includes null-terminator.
#define WOL_PASSWORD_LEN		9			//	Includes null-terminator.
#define WOL_CHANNAME_LEN_MAX	17			//	Includes null-terminator.
#define WOL_CHANKEY_LEN_MAX		9			//	Includes null-terminator.

#define CHAT_USER_SQUELCHED		0x0004		//	Will theoretically be added to the api and implemented.


#define LOBBYPASSWORD "not_a_valid_password"  // password removed per Security requirements - 8/27/2018      

#define USERCANCELLED	1
#define PATCHDOWNLOADED	2
#define PATCHAVOIDED	3

//	Special hidden descriptors added to channel list items.
//	These serve double-duty as tooltip help text.
#define CHANNELTYPE_TOP				TXT_WOL_CHANNELTYPE_TOP
#define CHANNELTYPE_OFFICIALCHAT	TXT_WOL_CHANNELTYPE_OFFICIALCHAT
#define CHANNELTYPE_USERCHAT		TXT_WOL_CHANNELTYPE_USERCHAT
#define CHANNELTYPE_GAMES			TXT_WOL_CHANNELTYPE_GAMES
#define CHANNELTYPE_GAMESOFTYPE     "GamesOfType"	//	Not seen.
#define CHANNELTYPE_CHATCHANNEL		"ChatChannel"	//	Not seen.
#define CHANNELTYPE_GAMECHANNEL		"GameChannel"	//	Not seen.
#define CHANNELTYPE_LOADING			TXT_WOL_CHANNELTYPE_LOADING
#define CHANNELTYPE_LOBBIES			TXT_WOL_CHANNELTYPE_LOBBIES
#define CHANNELTYPE_LOBBYCHANNEL	"LobbyChannel"	//	Not seen.

enum WOL_LEVEL
{
	WOL_LEVEL_TOP,					//	Viewing top level menu choices.
	WOL_LEVEL_OFFICIALCHAT,			//	Viewing official chat channels.
	WOL_LEVEL_USERCHAT,				//	Viewing user chat channels.
	WOL_LEVEL_INCHATCHANNEL,		//	In a chat channel.
	WOL_LEVEL_GAMES,				//	Viewing types (skus) of games.
	WOL_LEVEL_GAMESOFTYPE,			//	Viewing game channels of a type.
	WOL_LEVEL_INGAMECHANNEL,		//	In a game channel.
	WOL_LEVEL_LOBBIES,				//	Viewing the game lobbies.
	WOL_LEVEL_INLOBBY,				//	In a "lobby" chat channel.
	WOL_LEVEL_INVALID
};

struct WOL_GAMETYPEINFO
{
	int		iGameType;
	char	szName[128];
	char	szURL[256];
	HDIB	hDIB;				//	DIB handle.
	const char*	pDIB;				//	What you get when you GlobalLock hDIB.
};

//	Header values for game options messages. Note that 0 is not used!
enum WOL_GAMEOPT
{
	WOL_GAMEOPT_REQCOLOR = 1,	//	REQuest = guest asks game host for a color
	WOL_GAMEOPT_INFCOLOR,		//	INForm = game host tells guests color of a single player (not "accept-canceling")
	WOL_GAMEOPT_INFPARAMS,		//	host tells guests all common game params
	WOL_GAMEOPT_REQHOUSE,		//	guest tells host he's changed house (REQ because it's guest->host)
	WOL_GAMEOPT_INFHOUSE,		//	host tells guests about new house of a single player
	WOL_GAMEOPT_REQACCEPT,		//	guest tells host he accepts current params
	WOL_GAMEOPT_INFACCEPT,		//	host tells guests that a player accepted
	WOL_GAMEOPT_INFSTART,		//	host tell guests to go into wait for start mode
	WOL_GAMEOPT_REQSTART,		//	guest acknowledges WOL_GAMEOPT_INFSTART
	WOL_GAMEOPT_REQSTART_BUTNEEDSCENARIO,	//	guests acks WOL_GAMEOPT_INFSTART and asks for scenario download
	WOL_GAMEOPT_INFCANCELSTART,	//	host tells guests to cancel game start, as a change arrived or player joined/left
	WOL_GAMEOPT_INFGO,			//	host tells everyone to start
	WOL_GAMEOPT_INFNEWGUESTPLAYERINFO,	//	host tells new guest a lot of stuff about everyone that's in the game
};
enum DIBICON
{
	DIBICON_OWNER,
	DIBICON_SQUELCH,
	DIBICON_LATENCY,
	DIBICON_ACCEPT,
	DIBICON_NOTACCEPT,
	DIBICON_USER,
	DIBICON_PRIVATE,
	DIBICON_TOURNAMENT,
	DIBICON_VOICE,
};
#define NUMDIBICONS		9

struct DIBICONINFO
{
	char		szFile[50];
	HDIB		hDIB;
	const char*	pDIB;
};

//	See SaveChat()...
#define SAVECHATWIDTH	150			//	Wider than text that will fit in the chat list window.
struct CHATSAVE			//	What we save about each individual list item.
{
	char					szText[ SAVECHATWIDTH + 1 ];
	IconList_ItemExtras		ItemExtras;			//	Only color is used.
	CHATSAVE*				next;
};

struct CREATEGAMEINFO
{
	enum GAMEKIND		//	Gets or'ed with lobby number in channel 'reserved' field.
	{
		RAGAME = 0x01000000,
		CSGAME = 0x02000000,
		AMGAME = 0x04000000,
	};

	bool	bCreateGame;	//	True if user confirms game creation.
	int		iPlayerMax;		//	NOT number of players, but maximum number allowed into game channel.
	int		iPlayerCount;	//	Number of initial human players in game. Set at game launch, used for stats.
	bool	bTournament;
	bool	bPrivate;
	GAMEKIND GameKind;
	char	szPassword[ WOL_CHANKEY_LEN_MAX ];		//	If not blank, key for private game.
};

//***********************************************************************************************
HPALETTE	GetCurrentScreenPalette();
void		RemapDIBToPalette( HPALETTE hPal, const char* pDIB );		//	Note: pDIB is treated as non-const.
//char*		LoadFileIntoMemory( const char* szFileName, int& iLength );

//***********************************************************************************************
class WolapiObject
{
public:
	WolapiObject();
	virtual ~WolapiObject();

	IChat*					pChat;
	IDownload*				pDownload;
	INetUtil*				pNetUtil;
	DWORD					dwChatAdvise;			//	Value that identifies the "connection" from chat to chatsink.
	DWORD					dwDownloadAdvise;
	DWORD					dwNetUtilAdvise;

	RAChatEventSink*		pChatSink;
	RADownloadEventSink*	pDownloadSink;
	RANetUtilEventSink*		pNetUtilSink;

	bool					bChatShownBefore;

	char					szLadderServerHost[ 150 ];
	int						iLadderServerPort;
	char					szGameResServerHost1[ 150 ];
	int						iGameResServerPort1;
	char					szGameResServerHost2[ 150 ];
	int						iGameResServerPort2;

	bool		bFindEnabled;		//	I have to maintain these, though wolapi should do it for me...
	bool		bPageEnabled;		//	Note they are initialized true, as is currently the case in wol.
	bool		bLangFilter;		//
	bool		bAllGamesShown;

	bool		bEggSounds;			//	Easter egg related. True = user actions trigger sounds.
	bool		bEgg8Player;		//	True = 8 player games can be created. This is hidden so that we don't really have to support the feature...

	WOL_LEVEL	CurrentLevel;
	WOL_LEVEL	LastUpdateChannelCallLevel;
	char		szMyName[WOL_NAME_LEN_MAX];		//	Local user's name, valid while connected.
	char		szMyRecord[ WOL_NAME_LEN_MAX + 80 ];
	char		szMyRecordAM[ WOL_NAME_LEN_MAX + 80 ];
	bool		bMyRecordUpdated;				//	True when szMyRecord has changed and not yet recognized by chat dialog.
	char		szChannelListTitle[ 100 ];
	bool		bChannelListTitleUpdated;
	char		szChannelNameCurrent[WOL_CHANNAME_LEN_MAX];
	bool		bChannelOwner;
	char		szChannelReturnOnGameEnterFail[WOL_CHANNAME_LEN_MAX];

	int			iLobbyReturnAfterGame;			//	When in game channel, part of the value of the channel's 'reserved' field.
	
	bool		bReturningAfterGame;

	int			iLobbyLast;						//	Number of last lobby we personally were in.

//	CREATEGAMEINFO::GAMEKIND GameKindCurrent;	//	Kind of game (Red Alert, CS, AM) we are in game setup for.
	CREATEGAMEINFO GameInfoCurrent;				//	Kind of game (Red Alert, CS, AM, tournament, private) we are in game setup for.
	bool		bEnableNewAftermathUnits;		//	Used to pass game parameter back to init only.

	DWORD		dwTimeNextWolapiPump;
	DWORD		dwTimeNextChannelUpdate;

	DIBICONINFO DibIconInfos[ NUMDIBICONS ];

	HRESULT		hresPatchResults;				//	Used when a patch has been downloaded or cancelled.

	WOL_GameSetupDialog*	pGSupDlg;			//	When in a game channel, setting up a game; ptr to the dialog.

	bool		bInGame;				//	True while playing a game.
	bool		bConnectionDown;		//	Flag used while in a game, set to true if connection goes down.
	bool		bGameServer;			//	Flag used while in a game, true if game server (host).
	
	unsigned long	TournamentOpponentIP;	//	Valid while playing a tournament game. IP address of opponent.

	bool		bPump_In_Call_Back;		//	Used to enable PumpMessages during Call_Back(), for when we're in a modal dialog.

	bool		bSelfDestruct;			//	If set true, causes logout and deletion of wolapi object.

	char		szWebBrowser[ _MAX_PATH + 1 ];

	//	For "disconnect pinging".
	bool		bDoingDisconnectPinging;
	bool		bDisconnectPingingCompleted;
	int			iDisconnectPingCurrent;
	DISCONNECT_PING_STATUS	DisconnectPingResult_Server[ DISCONNECT_PING_COUNT ];
	DISCONNECT_PING_STATUS	DisconnectPingResult_Opponent[ DISCONNECT_PING_COUNT ];

	//	Used for in-game paging and responding.
	char		szExternalPager[ WOL_NAME_LEN_MAX ];	//	Last person to page me from outside the game, or blank for none.
	bool		bFreezeExternalPager;

	bool		bShowRankRA;		//	true = view RA rankings, false = view AM rankings
	bool		bShowRankUpdated;	//	set true when bShowRankRA value changes

	//	Standard wol buttons.
	char*		pShpDiscon;
	char*		pShpLeave;
	char*		pShpRefresh;
	char*		pShpSquelch;
	char*		pShpBan;
	char*		pShpKick;
	char*		pShpFindpage;
	char*		pShpOptions;
	char*		pShpLadder;
	char*		pShpHelp;
	ShapeButtonClass*	pShpBtnDiscon;
	ShapeButtonClass*	pShpBtnLeave;
	ShapeButtonClass*	pShpBtnRefresh;
	ShapeButtonClass*	pShpBtnSquelch;
	ShapeButtonClass*	pShpBtnBan;
	ShapeButtonClass*	pShpBtnKick;
	ShapeButtonClass*	pShpBtnFindpage;
	ShapeButtonClass*	pShpBtnOptions;
	ShapeButtonClass*	pShpBtnLadder;
	ShapeButtonClass*	pShpBtnHelp;
	ToolTipClass*		pTTipDiscon;
	ToolTipClass*		pTTipLeave;
	ToolTipClass*		pTTipRefresh;
	ToolTipClass*		pTTipSquelch;
	ToolTipClass*		pTTipBan;
	ToolTipClass*		pTTipKick;
	ToolTipClass*		pTTipFindpage;
	ToolTipClass*		pTTipOptions;
	ToolTipClass*		pTTipLadder;
	ToolTipClass*		pTTipHelp;

	WOL_GAMETYPEINFO		OldRAGameTypeInfos[ 3 ];	//	Used for storing old red alert icons only.

public:
	bool		bLoggedIn();

	void		LinkToChatDlg( IconListClass* pILChat, IconListClass* pILChannels, IconListClass* pILUsers, StaticButtonClass* pStaticUsers );
	void		ClearListPtrs();
	void		LinkToGameDlg( IconListClass* pILDisc, IconListClass* pILPlayers );

	void		PrepareButtonsAndIcons();

	bool		bSetupCOMStuff();
	void		UnsetupCOMStuff();

	void		PrintMessage( const char* szText, PlayerColorType iColorRemap = PCOLOR_NONE );
	void		PrintMessage( const char* szText, RemapControlType* pColorRemap );

	HRESULT		GetChatServer();
	HRESULT 	AttemptLogin( const char* szName, const char* szPass, bool bPassIsMangled );
	void		Logout();
	bool		UpdateChannels( int iChannelType, CHANNELFILTER ChannelFilter, bool bAutoping );
	void		OnChannelList();
	void		ListChannels();
	HRESULT		ChannelJoin( const char* szChannelName, const char* szKey );
	HRESULT		ChannelJoin( Channel* pChannelToJoin );
	bool		ChannelLeave();
//	bool		UserList();
	bool		ListChannelUsers();

	bool		bItemMarkedAccepted( int iIndex );
	bool		MarkItemAccepted( int iIndex, bool bAccept );
	bool		bItemMarkedReadyToGo( int iIndex );
	void		MarkItemReadyToGo( int iIndex, const char* szReadyState );
	bool		bItemMarkedNeedScenario( int iIndex );
	void		PullPlayerName_Into_From( char* szDest, const char* szSource );
	HousesType	PullPlayerHouse_From( const char* szSource );
	void		WritePlayerListItem( char* szDest, const char* szName, HousesType House );

	void		RequestPlayerPings();
	
	void		SendMessage( const char* szMessage, IconListClass& ILUsers, bool bAction );
	bool		ChannelCreate( const char* szChannelName, const char* szKey, bool bGame = false, int iMaxPlayers = 0, 
								bool bTournament = false, int iLobby = 0, CREATEGAMEINFO::GAMEKIND GameKind = CREATEGAMEINFO::RAGAME );
	void		DoFindPage();
	HRESULT		Locate( const char* szUser );
	HRESULT		Page( const char* szUser, const char* szSend, bool bWaitForResult );
	void		DoKick( IconListClass* pILUsersOrPlayers, bool bAndBan );
	bool		Kick( User* pUserToKick );
	bool		Ban( User* pUserToKick );
	void		DoSquelch( IconListClass* pILUsersOrPlayers );
	bool		Squelch( User* pUserToSquelch );
	void		DoOptions();
	bool		DoLadder();
	bool		DoHelp();
	bool		DoWebRegistration();
	bool		DoGameAdvertising( const Channel* pChannel );
	bool		SpawnBrowser( const char* szURL );
	
	void		ChannelListTitle( const char* szTitle );
	bool		EnterLevel_Top();
	bool		EnterLevel_OfficialChat();
	bool		EnterLevel_UserChat();
	bool		EnterLevel_Games();
	bool		EnterLevel_GamesOfType( WOL_GAMETYPEINFO* pGameTypeInfo );
	bool		EnterLevel_Lobbies();
	bool		OnEnteringChatChannel( const char* szChannelName, bool bICreatedChannel, int iLobby );
	void		OnExitingChatChannel();
	bool		ExitChatChannelForGameChannel();
	bool		OnEnteringGameChannel( const char* szChannelName, bool bICreatedChannel, const CREATEGAMEINFO& CreateGameInfo );
	bool		OnEnteringGameSetup();
	void		OnFailedToEnterGameChannel();
	void		OnExitingGameChannel();
	void		RejoinLobbyAfterGame();
	
	bool		RequestLadders( const char* szName );
	bool		RequestIPs( const char* szName );

	void		SaveChat();
	void		RestoreChat();
	void		AddHostLeftMessageToSavedChat( const char* szName );
	void		AddMessageToSavedChat( const char* szMessage );
	void		DeleteSavedChat();
	void		GenericErrorMessage();

	bool		GetNameOfBeginningLobby( char* szNameToSet );
	bool		GetLobbyChannels();

	const char* pGameHostName();
	User*		pGameHost();
	bool		SendGameOpt( const char* szSend, User* pUserPriv );
	bool		RequestGameStart();
	bool		SendGo( const char* szSend );

	void		Init_DisconnectPinging();
	bool		Pump_DisconnectPinging();
	void		DisconnectPingResultsString( char* szStringToSet );

	void		SetOptionDefaults();
	void		SetOptions( bool bEnableFind, bool bEnablePage, bool bLangFilterOn, bool bShowAllGames );

protected:
	void		GetGameTypeInfo( int iGameType, WOL_GAMETYPEINFO& GameTypeInfo, HPALETTE hPal );
	void*		IconForGameType( int iGameType );
	const char* NameOfGameType( int iGameType ) const;
	const char* URLForGameType( int iGameType ) const;

protected:
	//	Used by the general chat dialog.
	IconListClass*			pILChat;			//	Main messages list.
	IconListClass*			pILChannels;		//	Channels list.
	IconListClass*			pILUsers;			//	Users list.

//	IconListClass*			pILDisc;			//	Main messages list.		(pILChat is used.)
	IconListClass*			pILPlayers;			//	Players list.

	StaticButtonClass*		pStaticUsers;		//	Title for a users list. Used by main chat dialog only, not by game setup.

	WOL_GAMETYPEINFO*		GameTypeInfos;
	unsigned int			nGameTypeInfos;

	float					fLatencyToIconWidth;

	CHATSAVE*				pChatSaveList;
	CHATSAVE*				pChatSaveLast;
};

#endif

#endif
