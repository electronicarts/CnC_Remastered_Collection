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

#ifndef WIN32
#error WOLAPI_INTEGRATION can't be specified for non WIN32 version!
#endif

//	rawolapi.h - WOLAPI sinks declarations.
//	ajw 07/10/98

//	Based somewhat on Neal's Borlandized version, "chatapi.h".

#ifndef RAWOLAPI_H
#define RAWOLAPI_H

#include "function.h"

//#include "cominit.h"
#include <stdio.h>

//	From OBJBASE.H
#define interface struct

//	From RPCNDR.H
#define DECLSPEC_UUID(x)

#include <commctrl.h>

//namespace WOL		//	namespace is workaround due to the use of "Server" as a global in Red Alert.
//	ajw - Can't use namespaces in Watcom 10.5 it seems...
//{
	#include "wolapi\wolapi.h"
	#define IID_DEFINED
	//#include "wlib/wdebug.h"
	#include "wolapi\chatdefs.h"
	#include "wolapi\downloaddefs.h"
	#include "wolapi\ftpdefs.h"
//};
//using namespace WOL;
#include <winerror.h>
//#include <ocidl.h>
#include <olectl.h>

//***********************************************************************************************
//	For debugging chat defined hresults...
void ChatDefAsText( char* szDesc, HRESULT hRes );
void DebugChatDef( HRESULT hRes );

int iChannelLobbyNumber( const unsigned char* szChannelName );
#define REASONABLELOBBYINTERPRETEDNAMELEN	50
void InterpretLobbyNumber( char* szLobbyNameToSet, int iLobby );

class WolapiObject;

#define MAXCHATSENDLENGTH 71	//	Mainly aesthetic, and because of the length of edit line.

enum CHANNELFILTER
{
	CHANNELFILTER_NO,
	CHANNELFILTER_OFFICIAL,
	CHANNELFILTER_UNOFFICIAL,
	CHANNELFILTER_LOBBIES,
	CHANNELFILTER_LOCALLOBBYGAMES,
};

#define WOLCOLORREMAP_ACTION			PCOLOR_GREY
#define WOLCOLORREMAP_SELFSPEAKING		PCOLOR_RED
#define WOLCOLORREMAP_LOCALMACHINEMESS	PCOLOR_REALLY_BLUE		//	Color of system messages that originate locally.
#define WOLCOLORREMAP_PAGE				PCOLOR_GOLD
#define WOLCOLORREMAP_KICKORBAN			PCOLOR_GREEN	//LTBLUE
#define WOLCOLORREMAP_PUBLICMESSAGE		PCOLOR_NONE
#define WOLCOLORREMAP_PRIVATEMESSAGE	PCOLOR_ORANGE

#define WOLSOUND_ERROR					VOC_SYS_ERROR
#define WOLSOUND_LOGIN					VOC_RADAR_ON
#define WOLSOUND_LOGOUT					VOC_RADAR_OFF
#define WOLSOUND_ENTERCHAN				VOC_PLAYER_JOINED
#define WOLSOUND_EXITCHAN				VOC_PLAYER_LEFT
#define WOLSOUND_ONPAGE					VOC_INCOMING_MESSAGE
#define WOLSOUND_KICKORBAN				VOC_TANYA_KISS
#define WOLSOUND_ENTERGAME				VOC_INVULNERABLE
#define WOLSOUND_EXITGAME				VOC_DOOR

enum DISCONNECT_PING_STATUS
{
	PING_UNSTARTED,
	PING_WAITING,
	PING_GOOD,
	PING_BAD,
};
#define DISCONNECT_PING_COUNT		5

//***********************************************************************************************
class RAChatEventSink : /////public CComObjectRoot, /////public IConnectionPoint,
	public IChatEvent
{
 public:
	RAChatEventSink( WolapiObject* pOwner );
	virtual ~RAChatEventSink();

//    BEGIN_COM_MAP(RAChatEventSink)
//	  COM_INTERFACE_ENTRY(IChatEvent)
//    END_COM_MAP()

	// IUnknown
	STDMETHOD(QueryInterface)(const IID& iid, void **ppv);
	STDMETHOD_(ULONG,AddRef)();
	STDMETHOD_(ULONG,Release)();

	// IChatEvent
	STDMETHOD(OnServerList)(HRESULT res, Server* servers);
	STDMETHOD(OnLogout)(HRESULT r, User *user);
	STDMETHOD(OnBusy)(HRESULT r);
	STDMETHOD(OnIdle)(HRESULT r);
	STDMETHOD(OnPageSend)(HRESULT r);
	STDMETHOD(OnPaged)(HRESULT r, User *, LPCSTR);
	STDMETHOD(OnFind)(HRESULT r, Channel *);
	STDMETHOD(OnConnection)(HRESULT r,LPCSTR motd);
	STDMETHOD(OnChannelCreate)(HRESULT r, Channel *channel);
	STDMETHOD(OnChannelModify)(HRESULT r, Channel *channel);
	STDMETHOD(OnChannelJoin)(HRESULT r,  Channel *channel,  User *user);
	STDMETHOD(OnChannelLeave)(HRESULT r, Channel *channel,  User *user);
	STDMETHOD(OnChannelTopic)(HRESULT r, Channel *channel,  LPCSTR topic);
	STDMETHOD(OnGroupList)(HRESULT r,  Group *);
	STDMETHOD(OnPublicMessage)(HRESULT r,  Channel *channel, User *user, LPCSTR text);
	STDMETHOD(OnPrivateMessage)(HRESULT r,  User *user,LPCSTR text);
	STDMETHOD(OnSystemMessage)(HRESULT r, LPCSTR);
	STDMETHOD(OnNetStatus)(HRESULT r);
	STDMETHOD(OnChannelList)(HRESULT r, Channel* channels);
	STDMETHOD(OnUserList)(HRESULT r, Channel* channel, User* users);
	STDMETHOD(OnUpdateList)(HRESULT res, Update *);
	STDMETHOD(OnServerError)(HRESULT res);
	STDMETHOD(OnMessageOfTheDay)(HRESULT res, LPCSTR);
	STDMETHOD(OnPrivateAction)(HRESULT r, User *,  LPCSTR);
	STDMETHOD(OnPublicAction)(HRESULT r, Channel *, User *,  LPCSTR);
	STDMETHOD(OnPrivateGameOptions)(HRESULT r, User *,  LPCSTR);
	STDMETHOD(OnPublicGameOptions)(HRESULT r, Channel *, User *,  LPCSTR);
	STDMETHOD(OnGameStart)(HRESULT r, Channel *, User *,  int);
	STDMETHOD(OnUserKick)(HRESULT r, Channel *, User *,  User *);
	STDMETHOD(OnUserIP)(HRESULT r, User *);
	STDMETHOD(OnServerError)(HRESULT res, LPCSTR ircmsg);
	STDMETHOD(OnServerBannedYou)(HRESULT r, time_t bannedTill);
	STDMETHOD(OnUserFlags)(HRESULT r, LPCSTR name, unsigned int flags, unsigned int mask);
	STDMETHOD(OnChannelBan)(HRESULT r, LPCSTR name, int banned);

	unsigned long	GetPlayerGameIP( const char* szPlayerName ) const;
	void			DeleteUserList();		//	Deletes from heap all users pointed to through pUserList.
	void			DeleteUserIPList();
	unsigned long	GetUserIP( const char* szName ) const;
	
	void			ActionEggSound( const char* szMessage );

public:
	//	These vars are rather hackish. Basically, they are set before a callback is expected to be fired, and
	//	then checked immediately afterwards. The rest of the time, their values are meaningless.
	//	The idea is to force wolapi act in a modal way. In many places I "block" until a callback response to a 
	//	wolapi request has been received.
	bool	bRequestServerListWait;
	bool	bRequestConnectionWait;
	bool	bRequestLogoutWait;
//	bool	bRequestChannelListWait;
	bool	bRequestChannelJoinWait;
	bool	bRequestChannelLeaveWait;
	bool	bRequestUserListWait;
	bool	bRequestChannelCreateWait;
	bool	bRequestFindWait;
	bool	bRequestPageWait;

	bool	bRequestChannelListForLobbiesWait;

	bool	bIgnoreChannelLists;			//	Used to temporarily turn off response to channel lists, when we are in the midst
											//	of some processing that depends on pChannelList remaining constant.

	bool	bRequestGameStartWait;

	Server*			pServer;				//	Server to connect to, acquired from OnServerList.
	bool			bConnected;				//	True when user is logged in to chat server.
	bool			bJoined;				//	True when user has joined a channel.

	Channel*		pChannelList;			//	First element of channel list, or null.
	CHANNELFILTER	ChannelFilter;			//	Affects what channels are included in channel list when built.

	User*			pUserList;				//	First element of user list, or null.
	User*			pUserTail;				//	Last element of user list, or null.

	char*			szMotd;					//	Message of the day.
	HRESULT			hresRequestConnectionError;		//	Used to pass error hresult.
	
	HRESULT			hresRequestFindResult;	//	Used to pass hresult.
	Channel			OnFindChannel;

	HRESULT			hresRequestPageResult;	//	Used to pass hresult.

	HRESULT			hresRequestJoinResult;	//	Used to pass hresult.

	bool			bGotKickedTrigger;		//	Special flag meaning do some more processing after callback has exited.

	User*			pGameUserList;			//	First element of start game user list, or null.
	int				iGameID;				//	WW Online game id received from OnGameStart.
											//	Is also a flag indicating "OnGameStart() called, TriggerGameStart() not yet called".

	User*			pUserIPList;			//	List that holds user IP's, used for pinging in game channel.
	User*			pUserIPListTail;

protected:
	WolapiObject*	pOwner;					//	Link back to the object that contains me.

	void			DeleteChannelList();	//	Deletes from heap all channels pointed to through pChannelList.
	bool			DownloadUpdates( Update* pUpdateList, int iUpdates );
	bool			bSpecialMessage( const char* szMessage );
	void			InsertUserSorted( User* pUserNew );

private:
	long m_cRef;  // Reference Count

};

//***********************************************************************************************
class RADownloadEventSink : 
///////////	public CComObjectRoot,
	public IDownloadEvent
{
public:
	RADownloadEventSink();
	virtual ~RADownloadEventSink() {};

//  BEGIN_COM_MAP(RADownloadEventSink)
//    COM_INTERFACE_ENTRY(IDownloadEvent)
//  END_COM_MAP()

	// IUnknown
	STDMETHOD(QueryInterface)(const IID& iid, void **ppv);
	STDMETHOD_(ULONG,AddRef)();
	STDMETHOD_(ULONG,Release)();

	// IDownloadEvent
	STDMETHOD(OnEnd)(void);
	STDMETHOD(OnError)(int error);
	STDMETHOD(OnProgressUpdate)(int bytesread, int totalsize, int timetaken, int timeleft);
	STDMETHOD(OnStatusUpdate)(int status);
	STDMETHOD(OnQueryResume)(void);

public:
	bool		bFlagEnd;
	bool		bFlagError;
	bool		bFlagProgressUpdate;
	bool		bFlagStatusUpdate;
	bool		bFlagQueryResume;
	int			iBytesRead;
	int			iTotalSize;
	int			iTimeTaken;
	int			iTimeLeft;
	int			iStatus;
	bool		bResumed;

private:
	long m_cRef;  // Ref count
};

//***********************************************************************************************
class RANetUtilEventSink :
//    public CComObjectRoot,
    public INetUtilEvent
{
public:
	RANetUtilEventSink( WolapiObject* pOwner );
	virtual ~RANetUtilEventSink();

//BEGIN_COM_MAP(CNetUtilEventSink)
//	COM_INTERFACE_ENTRY(INetUtilEvent)
//END_COM_MAP()

	// IUnknown
	STDMETHOD(QueryInterface)(const IID& iid, void **ppv);
	STDMETHOD_(ULONG,AddRef)();
	STDMETHOD_(ULONG,Release)();

	// INetUtilEvent

	STDMETHOD(OnGameresSent)(HRESULT res);
	STDMETHOD(OnLadderList)(HRESULT res, Ladder *list, int totalCount, long timeStamp, int keyRung);
	STDMETHOD(OnPing)(HRESULT res, int time, unsigned long ip, int handle);


	void			DeleteLadderList();		//	Deletes from heap all users pointed to through pUserList.
	unsigned int	GetUserRank( const char* szName, bool bRankRA );

	Ladder*			pLadderList;				//	First element of Ladder list, or null.
	Ladder*			pLadderTail;				//	Last element of Ladder list, or null.
	Ladder*			pLadderListAM;				//	First element of Aftermath Ladder list, or null.
	Ladder*			pLadderTailAM;				//	Last element of Aftermath Ladder list, or null.

protected:
	WolapiObject*	pOwner;					//	Link back to the object that contains me.

private:
	long m_cRef;  // Reference Count
};

//***********************************************************************************************

//	SKU, reported to WOLAPI for the purpose of finding patches.
#ifdef ENGLISH
#define GAME_SKU		0x1500
#else
#ifdef GERMAN
#define GAME_SKU		0x1502
#else
#define GAME_SKU		0x1503
#endif
#endif

#define GAME_VERSION	0x00030003
#define GAME_TYPE		21
#define LOB_PREFIX		"Lob_21_"

//	Sent to gameres server in order to receive Red Alert or Aftermath ladder rankings. (Sent in RequestLadderList.)
#define LADDER_CODE_RA		1005
#define LADDER_CODE_AM		500

#endif

#endif
