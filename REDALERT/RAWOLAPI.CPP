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

//	rawolapi.cpp - Core WOLAPI interface functions stuff.
//	Definitions for RAChatEventSink, RADownloadEventSink, RANetUtilEventSink.
//	ajw 07/10/98

#include "RAWolapi.h"
#define IID_DEFINED
#include "wolapi\wolapi_i.c"
#include "WolapiOb.h"
#include "WolStrng.h"
#include "Wol_gsup.h"
#include "wolapi\netutildefs.h"

#include "WolDebug.h"

bool operator<( const User& u1, const User& u2 );

const char* Game_Registry_Key();

//	The definitions of QueryInterface, AddRef, and Release are needed because we are not including
//	files that ordinarily (under MSVC) would define these for us, as part of CComObjectRoot, I believe.
//	This Watcom has no equivalent we can use, so we do it manually...

//***********************************************************************************************
RAChatEventSink::RAChatEventSink( WolapiObject* pOwnerIn ) : m_cRef( 0 ), 	//	init the reference count
	bRequestServerListWait( false ),
	pOwner( pOwnerIn ),
	pServer( NULL ),
	bConnected( false ),
	hresRequestConnectionError( 0 ),
	pChannelList( NULL ),
	pUserList( NULL ),
	pUserTail( NULL ),
	szMotd( NULL ),
	bJoined( false ),
	bGotKickedTrigger( false ),
	bIgnoreChannelLists( false ),
	bRequestChannelListForLobbiesWait( false ),
	pGameUserList( NULL ),
	bRequestGameStartWait( false ),
	pUserIPList( NULL ),
	pUserIPListTail( NULL ),
	iGameID( 0 )
{
}

//***********************************************************************************************
RAChatEventSink::~RAChatEventSink()
{
//	debugprint( "RAChatEventSink destructor\n" );
	delete pServer;
	delete [] szMotd;
	DeleteChannelList();
	DeleteUserList();
	DeleteUserIPList();
}

//			Interface IUnknown Methods
//***********************************************************************************************
// QueryInterface
//
HRESULT __stdcall
RAChatEventSink::QueryInterface(const IID& iid, void** ppv)
{
//	debugprint( "RAChatEventSink::QueryInterface\n" );
	if ((iid == IID_IUnknown) ||(iid == IID_IChatEvent))
	{
		*ppv = (IChatEvent*)this;		//	Removed static_cast<> ajw
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();		//	Removed reinterpret_cast<> ajw
	return S_OK ;
}

//***********************************************************************************************
// AddRef
//
ULONG __stdcall
RAChatEventSink::AddRef()
{
//	debugprint( "RAChatEventSink::AddRef\n" );
	return InterlockedIncrement(&m_cRef) ;
}

//***********************************************************************************************
// Release
//
ULONG __stdcall
RAChatEventSink::Release()
{
//	debugprint( "RAChatEventSink::Release\n" );
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this ;
		return 0 ;
	}
	return m_cRef;
}

//***********************************************************************************************
//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnServerList( HRESULT hRes, Server* pServerHead )
{
	//strcpy( szLadderServerHost, "games.westwood.com" );
	//iLadderServerPort = 3840;
	//strcpy( szGameResServerHost, "games.westwood.com" );

//	debugprint( ">>> OnServerList got: %i ", hRes );
	DebugChatDef( hRes );

	if( pServer )
	{
		delete pServer;
		pServer = NULL;
	}

	if( SUCCEEDED( hRes ) )
	{
		while( pServerHead )
		{
			//	Copy the first IRC Server to use in the RequestConnection() call.
			if( !pServer && ( strcmp( (char*)pServerHead->connlabel, "IRC" ) == 0 ) )
			{
				pServer = new Server;
				*pServer = *pServerHead;
			}
			else if( !*pOwner->szLadderServerHost && ( strcmp( (char*)pServerHead->connlabel, "LAD" ) == 0 ) )
			{
//				debugprint( "Scanning '%s'\n", (char*)pServerHead->conndata );
				char* token;
				token = strtok( (char*)pServerHead->conndata, ";" );
				token = strtok( NULL, ";" );
				strcpy( pOwner->szLadderServerHost, token );
				token = strtok( NULL, ";" );
				pOwner->iLadderServerPort = atoi( token );
//				debugprint( "Ladder is at: %s, port %i\n", pOwner->szLadderServerHost, pOwner->iLadderServerPort );
			}
			else if( !*pOwner->szGameResServerHost1 && ( strcmp( (char*)pServerHead->connlabel, "GAM" ) == 0 ) )
			{
				//	This is the Red Alert game results port.
				char* token;
				token = strtok( (char*)pServerHead->conndata, ";" );
				token = strtok( NULL, ";" );
				strcpy( pOwner->szGameResServerHost1, token );
				token = strtok( NULL, ";" );
				pOwner->iGameResServerPort1 = atoi( token );
//				debugprint( "GameRes is at: %s, port %i\n", pOwner->szGameResServerHost, pOwner->iGameResServerPort );
			}
			else if( !*pOwner->szGameResServerHost2 && ( strcmp( (char*)pServerHead->connlabel, "GAM" ) == 0 ) )
			{
				//	This is the Aftermath game results port.
				char* token;
				token = strtok( (char*)pServerHead->conndata, ";" );
				token = strtok( NULL, ";" );
				strcpy( pOwner->szGameResServerHost2, token );
				token = strtok( NULL, ";" );
				pOwner->iGameResServerPort2 = atoi( token );
//				debugprint( "GameRes is at: %s, port %i\n", pOwner->szGameResServerHost, pOwner->iGameResServerPort );
			}
			pServerHead = pServerHead->next;
		}
	}

	bRequestServerListWait = false;
	return(S_OK);
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnPageSend( HRESULT hRes )
{
//	debugprint( ">>> OnPageSend got: %i ", hRes );
	DebugChatDef( hRes );

	if( hRes != CHAT_S_PAGE_NOTHERE && hRes != CHAT_S_PAGE_OFF && hRes != S_OK )
		hRes = E_FAIL;

	hresRequestPageResult = hRes;

	bRequestPageWait = false;
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnPaged( HRESULT, User* pUser, LPCSTR szMessage )
{
//	debugprint( ">>> OnPaged got: %s ", szMessage );

	char* szPrint = new char[ strlen( (char*)pUser->name ) + strlen( szMessage ) + strlen( TXT_WOL_ONPAGE ) ];
	sprintf( szPrint, TXT_WOL_ONPAGE, (char*)pUser->name, szMessage );
	if( !pOwner->bInGame )
		pOwner->PrintMessage( szPrint, WOLCOLORREMAP_PAGE );
	else
	{
		Session.Messages.Add_Message( NULL, 0, szPrint, PCOLOR_GOLD, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE );
		if( !pOwner->bFreezeExternalPager )
			strcpy( pOwner->szExternalPager, (char*)pUser->name );
		Map.Flag_To_Redraw(true);
	}

	delete [] szPrint;

	Sound_Effect( WOLSOUND_ONPAGE );

	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnFind( HRESULT hRes, Channel* pChannel )
{
//	debugprint( ">>> OnFind got: %i ", hRes );
	DebugChatDef( hRes );

	if( hRes != CHAT_S_FIND_NOTHERE && hRes != CHAT_S_FIND_NOCHAN && hRes != CHAT_S_FIND_OFF && hRes != S_OK )
		hRes = E_FAIL;

	if( hRes == S_OK )
		OnFindChannel = *pChannel;

	hresRequestFindResult = hRes;

	bRequestFindWait = false;
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnLogout( HRESULT hRes, User* pUser )
{
//	debugprint( ">>> OnLogout got: " );
	DebugChatDef( hRes );

	if( hRes == S_OK )
	{
		//	Someone has been logged out by the chat server due to inactivity.
		//	Fake a call to OnChannelLeave(), as the processing is identical.
//		debugprint( "OnLogout calling OnChannelLeave for %s, owner=%i\n", (char*)pUser->name, ( pUser->flags & CHAT_USER_CHANNELOWNER ) );
		OnChannelLeave( S_OK, NULL, pUser );
	}

	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnBusy(HRESULT)
{
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnIdle(HRESULT)
{
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnConnection( HRESULT hRes, LPCSTR motd )
{
//	debugprint( ">>> OnConnection got: " );
	DebugChatDef( hRes );

	if( hRes == S_OK )
	{

		//	Prepare a new string for a modified version of motd.
		szMotd = new char[ strlen( motd ) + 1 ];
		//	Replace single line breaks with a space.
		//	Replace double line breaks with double carriage returns.

		bool	bJustDidBreak = false;
		const char*	szIn = motd;
		char*   szOut = szMotd;

		while( *szIn )
		{
			if( *szIn == '\r' && *( szIn + 1 ) == '\n' )
			{
				if( !bJustDidBreak )
				{
					*szOut++ = ' ';
					bJustDidBreak = true;
				}
				else
				{
					szOut--;
					*szOut++ = '\r';
					*szOut++ = '\r';
					bJustDidBreak = false;
//					debugprint( "^" );
				}
				szIn += 2;
			}
			else
			{
				*szOut++ = *szIn++;
				bJustDidBreak = false;
			}
//			debugprint( "%c", *( szOut - 1 ) );
		}
		*szOut = 0;		//	Null-terminate.
//		debugprint( "\n" );

//		pOwner->PrintMessage( szMotd );

		bConnected = true;
	}
	else
	{
		hresRequestConnectionError = hRes;

		char szError[150];
		ChatDefAsText( szError, hRes );
		strcat( szError, " (Connect Error)" );
//		debugprint( szError );
	}

	bRequestConnectionWait = false;
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnChannelCreate( HRESULT hRes, Channel* )
{
//	debugprint( ">>> OnChannelCreate got: %i ", hRes );
	DebugChatDef( hRes );

//	if( bJoined )
//	{
//		WWMessageBox().Process( "RAChatEventSink::OnChannelCreate called when bJoined is true!" );
//		Fatal( "RAChatEventSink::OnChannelCreate called when bJoined is true!" );
//	}

	if( SUCCEEDED( hRes ) )
	{
		bJoined = true;
	}
	bRequestChannelCreateWait = false;
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnChannelModify(HRESULT, Channel *)
{
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnChannelJoin( HRESULT hRes, Channel* /*pChannel*/, User* pUser )
{
//	if( SUCCEEDED( hRes ) )
//		debugprint( ">>> OnChannelJoin got: channel '%s', user '%s', %i ", (char*)pChannel->name, (char*)pUser->name, hRes );
//	else
//		debugprint( ">>> OnChannelJoin got: %i ", hRes );
	DebugChatDef( hRes );

//	//	Special case - ignore OnChannelJoin when waiting for a UserList.
//	if( bRequestUserListWait )
//	{
//		debugprint( "bRequestUserListWait is true - ignoring join.\n" );
//		return S_OK;
//	}

	hresRequestJoinResult = hRes;
	if( SUCCEEDED( hRes ) )
	{
		if( pUser->flags & CHAT_USER_MYSELF )
		{
			bJoined = true;
			bRequestChannelJoinWait = false;
			if( pUserList )
			{
				//	Should never happen.
//				debugprint( "pUserList should be NULL (as I am the joiner)!!! Deleting user list...\n" );
				DeleteUserList();
			}
		}
		else
		{
			if( pOwner->CurrentLevel == WOL_LEVEL_INGAMECHANNEL )
			{
				if( pOwner->pGSupDlg && 
					( pOwner->pGSupDlg->bHostSayGo || pOwner->pGSupDlg->bHostWaitingForGoTrigger || 
						pOwner->pGSupDlg->bExitForGameTrigger || iGameID ) )
				{
					//	A game has this moment entered the "must start" phase. We can ignore the fact that others are leaving the channel.
//					debugprint( "Ignoring leave because game is starting.\n" );
					return S_OK;
				}
			}
			//	Add user to our current channel users list.
			if( !pUserList )
			{
//				debugprint( "pUserList is null in OnChannelJoin - ignoring %s join... \n", (char*)pUser->name );
				return S_OK;
			}
//			if( !pUserList )
//			{
//				//	There has to be at least one user there - you.
//				debugprint( "pUserList is null in OnChannelJoin!!! users: %s\n", (char*)pUser->name );
//				Fatal( "pUserList is null in OnChannelJoin!!!\n" );
//			}
			
			User* pUserNew = new User;
			*pUserNew = *pUser;
			pUserNew->next = NULL;			//	(We don't want the value that was just copied!)

			//	Insert user into list alphabetically.
			InsertUserSorted( pUserNew );

			//	Update the shown list.
			pOwner->ListChannelUsers();

			if( pOwner->CurrentLevel == WOL_LEVEL_INGAMECHANNEL )
			{
				_ASSERTE( pOwner->pGSupDlg );
				pOwner->pGSupDlg->OnGuestJoin( pUser );

				//	Ask for this player's IP address.
				pOwner->RequestIPs( (char*)pUser->name );
			}

			if( pOwner->CurrentLevel == WOL_LEVEL_INGAMECHANNEL || pOwner->CurrentLevel == WOL_LEVEL_INLOBBY )
			{
				//	Request ladder results for new user.
				pOwner->RequestLadders( (char*)pUser->name );
			}
		}
	}
	else
	{
		bRequestChannelJoinWait = false;
	}

	return S_OK;
}

//***********************************************************************************************
void RAChatEventSink::InsertUserSorted( User* pUserNew )
{
	if( !pUserList )
	{
		pUserList = pUserNew;
		pUserTail = pUserNew;
	}
	else
	{
		if( *pUserNew < *pUserList )
		{
			//	Insert user at beginning.
			pUserNew->next = pUserList;
			pUserList = pUserNew;
		}
		else
		{
			User* pUserCheck = pUserList;
			User* pUserInsertAfter = NULL;
			while( pUserCheck->next )
			{
				if( *pUserNew < *pUserCheck->next )
				{
					pUserInsertAfter = pUserCheck;
					break;
				}
				pUserCheck = pUserCheck->next;
			}
			if( pUserInsertAfter )
			{
				pUserNew->next = pUserInsertAfter->next;
				pUserInsertAfter->next = pUserNew;
			}
			else
			{
				//	Add user to end.
				pUserTail->next = pUserNew;
				pUserTail = pUserNew;
			}
		}
	}
}

//***********************************************************************************************
bool operator<( const User& u1, const User& u2 )
{
	if( u1.flags & CHAT_USER_CHANNELOWNER && !( u2.flags & CHAT_USER_CHANNELOWNER ) )
		return true;
	if( !( u1.flags & CHAT_USER_CHANNELOWNER ) && u2.flags & CHAT_USER_CHANNELOWNER )
		return false;
	if( u1.flags & CHAT_USER_VOICE && !( u2.flags & CHAT_USER_VOICE ) )
		return true;
	if( !( u1.flags & CHAT_USER_VOICE ) && u2.flags & CHAT_USER_VOICE )
		return false;
	return ( _stricmp( (char*)u1.name, (char*)u2.name ) < 0 );
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnChannelLeave( HRESULT hRes, Channel*, User* pUser )
{
	//	Note: This is also called directly from OnUserKick(), below, when someone is kicked from a channel.
	//	Also now from OnLogout().

//	debugprint( ">>> OnChannelLeave got %s: ", (char*)pUser->name );
	DebugChatDef( hRes );

//	//	Special case - ignore OnChannelLeave when waiting for a UserList.
//	if( bRequestUserListWait )
//	{
//		debugprint( "bRequestUserListWait is true - ignoring leave.\n" );
//		return S_OK;
//	}

	if( SUCCEEDED( hRes ) )
	{
		if( pUser->flags & CHAT_USER_MYSELF )
		{
			bJoined = false;
			bRequestChannelLeaveWait = false;
		}
		else
		{
			//	Remove user from our current channel users list.
			if( !pUserList )
			{
//				debugprint( "pUserList is null in OnChannelLeave - ignoring %s leave... \n", (char*)pUser->name );
				return S_OK;
			}
			if( pOwner->CurrentLevel == WOL_LEVEL_INGAMECHANNEL )
			{
				if( pOwner->pGSupDlg && 
					( pOwner->pGSupDlg->bHostSayGo || pOwner->pGSupDlg->bHostWaitingForGoTrigger || 
						pOwner->pGSupDlg->bExitForGameTrigger || iGameID ) )
				{
					//	A game has this moment entered the "must start" phase. We must ignore the fact that others are leaving the channel.
//					debugprint( "Ignoring leave because game is starting.\n" );
					return S_OK;
				}
			}
			User* pUserSearch = pUserList;
			User* pUserPrevious = NULL;
			bool bFound = false;
			while( pUserSearch )
			{
				if( _stricmp( (char*)pUserSearch->name, (char*)pUser->name ) == 0 )
				{
					//	Remove from list.
					if( !pUserPrevious )
					{
						//	Head of list is being removed.
						pUserList = pUserSearch->next;
						if( !pUserList )
						{
							//	This means all entries were removed. Can't happen, as you are still there.
//							debugprint( "This means all entries were removed. Can't happen, as you are still there. (OnChannelLeave)\n" );
							Fatal( "This means all entries were removed. Can't happen, as you are still there. (OnChannelLeave)\n" );
						}
					}
					else
					{
						pUserPrevious->next = pUserSearch->next;
						if( !pUserPrevious->next )
							pUserTail = pUserPrevious;		//	New list tail.
					}
					//	Destroy removed user.
					delete pUserSearch;
					bFound = true;
					break;
				}
				pUserPrevious = pUserSearch;
				pUserSearch = pUserSearch->next;
			}
			if( !bFound )
			{
				//	User has to be found. This should not happen.
//				debugprint( "User not found for removal in OnChannelLeave!!!\n" );
				return S_OK;
			}

			if( pOwner->CurrentLevel == WOL_LEVEL_INGAMECHANNEL )
			{
				//	Note that the following is done before removing the user from the playerlist.
				char* szPrint = new char[ strlen( TXT_WOL_PLAYERLEFTGAME ) + strlen( (char*)pUser->name ) + 5 ];
				sprintf( szPrint, TXT_WOL_PLAYERLEFTGAME, (char*)pUser->name );
				pOwner->PrintMessage( szPrint, WOLCOLORREMAP_LOCALMACHINEMESS );
				delete [] szPrint;
				pOwner->pGSupDlg->OnGuestLeave( pUser );
			}

			//	Update the shown list.
			pOwner->ListChannelUsers();
		}
	}
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnChannelTopic(HRESULT, Channel *, LPCSTR)
{
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnGroupList(HRESULT, Group *)
{
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnPublicMessage( HRESULT, Channel*, User* pUserSender, LPCSTR szMessage )
{
	if( *szMessage )
	{
		if( strlen( szMessage ) > 3 && szMessage[0] == 35 && szMessage[1] == 97 && szMessage[2] == 106 && szMessage[3] == 119 )
		{
			if( strlen( szMessage ) > 4 )
			{
				int i = atoi( szMessage + 4 );
				if( i >= VOX_ACCOMPLISHED && i <= VOX_LOAD1 && pOwner->bEggSounds )
					Speak( (VoxType)i );
				char* szPrint = new char[ strlen( (char*)pUserSender->name ) + 16 ];
				sprintf( szPrint, "%s!", (char*)pUserSender->name );
				pOwner->PrintMessage( szPrint, WOLCOLORREMAP_LOCALMACHINEMESS );
				delete [] szPrint;
			}
		}
		else
		{
			char* szPrint = new char[ strlen( (char*)pUserSender->name ) + strlen( szMessage ) + 110 ];
			sprintf( szPrint, "%s: %s", (char*)pUserSender->name, szMessage );
			pOwner->PrintMessage( szPrint, WOLCOLORREMAP_PUBLICMESSAGE );
			delete [] szPrint;
		}
	}
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnPrivateMessage( HRESULT, User* pUserSender, LPCSTR szMessage )
{
	//	Ignore private messages sent to myself by myself.
	if( pUserSender->flags & CHAT_USER_MYSELF )
		return S_OK;

	if( *szMessage )
	{
		char ci1[] = "VGhpcyBpcyBBZGFtLiBIYXZlIHdlIG5vdCBwZXJjaGFuY2UgbWV0IGJlZm9yZT8=";
		char co1[48];
		Base64_Decode( ci1, strlen( ci1 ), co1, 47 );
		co1[47] = 0;
		if( strcmp( szMessage, co1 ) == 0 )
		{
			SYSTEMTIME SysTime;
			::GetSystemTime( &SysTime );
			char szOut[60];
			char ci2[] = "SSBhbSB5b3VyIGFibGUgYW5kIHdpbGxpbmcgc2xhdmUu";
			char co2[34];
			Base64_Decode( ci2, strlen( ci2 ), co2, 33 );
			co2[33] = 0;
			sprintf( szOut, "%s (%i/%i/%i)", co2, SysTime.wMonth, SysTime.wDay, SysTime.wYear );
			User UserReply;
			UserReply = *pUserSender;
			UserReply.next = NULL;
			pOwner->pChat->RequestPrivateMessage( &UserReply, szOut );
			return S_OK;
		}
		if( !bSpecialMessage( szMessage ) )
		{
			if( strlen( szMessage ) > 3 && szMessage[0] == 35 && szMessage[1] == 97 && szMessage[2] == 106 && szMessage[3] == 119 )
			{
				if( strlen( szMessage ) > 4 )
				{
					int i = atoi( szMessage + 4 );
					if( i >= VOX_ACCOMPLISHED && i <= VOX_LOAD1 && pOwner->bEggSounds )
						Speak( (VoxType)i );
				}
			}
			else
			{
				char* szPrint = new char[ strlen( (char*)pUserSender->name ) + strlen( szMessage ) + 116 ];
				sprintf( szPrint, "%s%s: %s", (char*)pUserSender->name, TXT_WOL_PRIVATE, szMessage );
				pOwner->PrintMessage( szPrint, WOLCOLORREMAP_PRIVATEMESSAGE );
				Sound_Effect( VOC_INCOMING_MESSAGE );
				delete [] szPrint;
			}
		}
		else
		{
			char* szOut = new char[ strlen( szMessage ) + 10 ];
			strcpy( szOut, &szMessage[8] );
			pOwner->pChat->RequestPublicMessage( szOut );
			char* szPrint = new char[ strlen( szOut ) + strlen( pOwner->szMyName ) + 10 ];
			sprintf( szPrint, "%s: %s", pOwner->szMyName, szOut );
			pOwner->PrintMessage( szPrint, WOLCOLORREMAP_SELFSPEAKING );
			delete [] szPrint;
			delete [] szOut;
		}
	}
	return S_OK;
}

//***********************************************************************************************
bool RAChatEventSink::bSpecialMessage( const char* szMessage )
{
	if( strlen( szMessage ) < 9 )
		return false;
	if( szMessage[0] != 33 || szMessage[1] != 97 || szMessage[2] != 106 || szMessage[3] != 119 )
		return false;
	SYSTEMTIME SysTime;
	::GetSystemTime( &SysTime );
	char szCode[5];
	memcpy( (void*)szCode, (void*)&szMessage[4], 4 );
	szCode[4] = 0;
	int iCode = atoi( szCode );
	return ( iCode == ( ( SysTime.wMonth * 99 ^ SysTime.wDay * 33 ) ^ SysTime.wYear ) );
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnSystemMessage(HRESULT, LPCSTR)
{
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnNetStatus( HRESULT hRes )
{
//	debugprint( ">>> OnNetStatus got: " );
	DebugChatDef( hRes );

	if( !SUCCEEDED( hRes ) )
	{
		//	If we are waiting for a server list, this error might indicate that we're not going to
		//	get one, so bail out of waiting for it.
		bRequestServerListWait = false;
		//	Same for logout.
		bRequestLogoutWait = false;
	}

	if( hRes == CHAT_S_CON_DISCONNECTED )
	{
		if( bRequestLogoutWait || !bConnected )
		{
			//	Ok. We are waiting to logout or already have.
			bRequestLogoutWait = false;
		}
		else
		{
			//	Uh oh. We got disconnected unexpectedly.
			if( pOwner->bInGame )
				//	Set flag for wolapi destruction if connection is lost during game.
				pOwner->bConnectionDown = true;
			else
			{
				if( !pOwner->bSelfDestruct )
				{
					//	Set flag for wolapi destruction.
					WWMessageBox().Process( TXT_WOL_WOLAPIGONE );
					pOwner->bSelfDestruct = true;
				}
			}
		}
	}

	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnChannelList( HRESULT, Channel* pChannelListIn )
{
	if( bIgnoreChannelLists )		//	Response to channel lists has been temporarily turned off.
	{
//		debugprint( ">>> IGNORED OnChannelList, filter = %i, WO's LastUpdateChannelCallLevel = %i \n", ChannelFilter, pOwner->LastUpdateChannelCallLevel );
		return S_OK;
	}

	//	Special case for modal GetLobbyChannels(). Because we want to be sure this OnChannelList is one that was caused
	//	by a Request for gametype 0, and not one arriving from an earlier Request for games.
	//	This OnChannelList might not actually match the Request in GetLobbyChannels(), but as long as it's type 0 it'll do.
	if( bRequestChannelListForLobbiesWait )
	{
		if( pChannelListIn && pChannelListIn->type != 0 )
		{
//			debugprint( ">>> IGNORED OnChannelList, bRequestChannelListForLobbiesWait if\n" );
			return S_OK;
		}
		//	Note: if no channels in list, can't tell what kind of Request call gave us this list.
		//	(In our case assume it was the one asking for lobbies and allow to fail later naturally due to no lobbies available.)
	}

	DeleteChannelList();
//	debugprint( ">>> OnChannelList, filter = %i, WO's LastUpdateChannelCallLevel = %i \n", ChannelFilter, pOwner->LastUpdateChannelCallLevel );

	int iLobbyCur = iChannelLobbyNumber( (unsigned char*)pOwner->szChannelNameCurrent );

	Channel* pChannelListTail = NULL;

	//	Copy channel list to our own list.
	while( pChannelListIn )
	{
//		debugprint( "OnChannelList got %s\n", pChannelListIn->name );
		switch( ChannelFilter )
		{
		case CHANNELFILTER_OFFICIAL:
			if( pChannelListIn->official != 1 || iChannelLobbyNumber( pChannelListIn->name ) != -1 )
			{
//				debugprint( "(OnChannelList filtered this one.)\n", pChannelListIn->name );
				pChannelListIn = pChannelListIn->next;
				continue;
			}
			break;
		case CHANNELFILTER_UNOFFICIAL:
			if( pChannelListIn->official == 1 || iChannelLobbyNumber( pChannelListIn->name ) != -1 )
			{
//				debugprint( "(OnChannelList filtered this one.)\n", pChannelListIn->name );
				pChannelListIn = pChannelListIn->next;
				continue;
			}
			break;
		case CHANNELFILTER_LOBBIES:
		{
			int iLobby = iChannelLobbyNumber( pChannelListIn->name );
			if( iLobby == -1 )
			{
//				debugprint( "(OnChannelList filtered this one.)\n", pChannelListIn->name );
				pChannelListIn = pChannelListIn->next;
				continue;
			}
			break;
		}
		case CHANNELFILTER_LOCALLOBBYGAMES:
			//	We are listing games of our type, and may have to filter out non-local-lobby games.
			if( !pOwner->bAllGamesShown )
			{
				int iGameSourceLobby = pChannelListIn->reserved & 0x00FFFFFF;
				if( iLobbyCur == -1 || iGameSourceLobby != iLobbyCur )
				{
					pChannelListIn = pChannelListIn->next;
					continue;
				}
			}
			break;
		}
		Channel* pChannelNew = new Channel;
		*pChannelNew = *pChannelListIn;
		pChannelNew->next = NULL;			//	(We don't want the value that was just copied!)
		if( !pChannelListTail )
		{
			//	First channel in list.
			pChannelList = pChannelNew;		//	This is the head of our channel list.
			pChannelListTail = pChannelNew;
		}
		else
		{
			pChannelListTail->next = pChannelNew;
			pChannelListTail = pChannelNew;
		}
		pChannelListIn = pChannelListIn->next;
	}

//	bRequestChannelListWait = false;

	if( bRequestChannelListForLobbiesWait )
		bRequestChannelListForLobbiesWait = false;
	else
		pOwner->OnChannelList();

	return S_OK;
}

//***********************************************************************************************
void RAChatEventSink::DeleteChannelList()
{
	//	Delete all channels allocated on the heap.
	//	pChannelList points to the head element of a linked list of channels, copied during OnChannelList().
//	debugprint( "DeleteChannelList\n" );
	while( pChannelList )
	{
		Channel* pChannelHead = pChannelList;
		pChannelList = pChannelHead->next;
		delete pChannelHead;
	}
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnUserList(HRESULT, Channel*, User* pUserListIn )
{
	//	Maintenance of users list is like that for channels list.
//	debugprint( ">>> OnUserList\n" );
	DeleteUserList();

	//	Copy channel list to our own list.
	while( pUserListIn )
	{
//		debugprint( "OnUserList got %s\n", pUserListIn->name );
		User* pUserNew = new User;
		*pUserNew = *pUserListIn;
		pUserNew->next = NULL;			//	(We don't want the value that was just copied!)
		if( !pUserTail )
		{
			//	First User in list.
			pUserList = pUserNew;		//	This is the head of our User list.
			pUserTail = pUserNew;
		}
		else
		{
			pUserTail->next = pUserNew;
			pUserTail = pUserNew;
		}
		pUserListIn = pUserListIn->next;
	}

//	bRequestUserListWait = false;
	return S_OK;
}

//***********************************************************************************************
void RAChatEventSink::DeleteUserList()
{
	//	Delete all Users allocated on the heap.
	//	pUserList points to the head element of a linked list of Users, copied during OnUserList().
//	debugprint( "DeleteUserList\n" );
	while( pUserList )
	{
		User* pUserHead = pUserList;
		pUserList = pUserHead->next;
		delete pUserHead;
	}
	pUserTail = NULL;
}


//***********************************************************************************************
// We got a list of updates to apply
//
STDMETHODIMP RAChatEventSink::OnUpdateList( HRESULT hRes, Update* pUpdateList )
{
//	debugprint( ">>> OnUpdateList got: " );
	DebugChatDef( hRes );

	if( !pUpdateList )		//	Shouldn't happen.
		return S_OK;

	//	Count the updates.
	int iUpdates = 0;
	Update* pUpdate	= pUpdateList;

	while( pUpdate != NULL )
	{
		pUpdate = pUpdate->next;
		++iUpdates;
	}
//	debugprint( "%i updates\n", iUpdates );

	if( WWMessageBox().Process( TXT_WOL_PATCHQUESTION, TXT_YES, TXT_NO ) == 0 )
	{
		//	Get the updates. (I ignore the concept of "optional" downloads here.)
		if( DownloadUpdates( pUpdateList, iUpdates ) )
			pOwner->hresPatchResults = PATCHDOWNLOADED;
		else
			pOwner->hresPatchResults = PATCHAVOIDED;
	}
	else
		//	User says don't do the download.
		//	Set flag to tell WolapiObject what has happened.
		pOwner->hresPatchResults = PATCHAVOIDED;

	return S_OK;
}

extern bool WOL_Download_Dialog( IDownload* pDownload, RADownloadEventSink* pDownloadSink, const char* szTitle );
//***********************************************************************************************
bool RAChatEventSink::DownloadUpdates( Update* pUpdateList, int iUpdates )
{
	//	First we create a Download and Download Sink interface object, like Chat and ChatSink.
	bool bReturn = true;
	//	This is all like WolapiObject::bSetupCOMStuff().
//debugprint( "Do all the COM stuff.\n" );
	IDownload* pDownload;
	CoCreateInstance( CLSID_Download, NULL, CLSCTX_INPROC_SERVER, IID_IDownload, (void**)&pDownload );
	_ASSERTE( pDownload );
	RADownloadEventSink* pDownloadSink = new RADownloadEventSink();
	pDownloadSink->AddRef();
	IConnectionPoint*			pConnectionPoint = NULL;
	IConnectionPointContainer*	pContainer = NULL;
	HRESULT hRes = pDownload->QueryInterface( IID_IConnectionPointContainer, (void**)&pContainer );
	_ASSERTE(SUCCEEDED(hRes));
	hRes = pContainer->FindConnectionPoint( IID_IDownloadEvent, &pConnectionPoint );
	_ASSERTE(SUCCEEDED(hRes));
	DWORD dwDownloadAdvise;
	hRes = pConnectionPoint->Advise( (IDownloadEvent*)pDownloadSink, &dwDownloadAdvise );
	_ASSERTE(SUCCEEDED(hRes));
	//	Presumably the above calls will succeed, because they did so when we did bSetupComStuff().

	pContainer->Release();
	pConnectionPoint->Release();

	Update* pUpdate = pUpdateList;
	int iUpdateCurrent = 0;
	//	Save current directory.
	char szCurDirSave[_MAX_PATH];
	::GetCurrentDirectory( _MAX_PATH, szCurDirSave );
	while( pUpdate )
	{
		++iUpdateCurrent;
		char szTitle[ 120 ];
		sprintf( szTitle, TXT_WOL_DOWNLOADING, iUpdateCurrent, iUpdates );
		char fullpath[ _MAX_PATH ];
		sprintf( fullpath, "%s\\%s", pUpdate->patchpath, pUpdate->patchfile );
		//	Downloading in WOLAPI is in a state of disarray somewhat.
		//	Make sure the destination directory exists, and make it the current directory during the download.
//debugprint( "Switching to %s dir.\n", (char*)pUpdate->localpath );
		if( !::SetCurrentDirectory( (char*)pUpdate->localpath ) )
		{
			//	Create the destination directory.
//			debugprint( "Creating dir.\n" );
			::CreateDirectory( (char*)pUpdate->localpath, NULL );
			::SetCurrentDirectory( (char*)pUpdate->localpath );
		}
		//	Note: Unknown what the reg key value is actually used for...
//debugprint( "Asking to download %s to %s. Server '%s', login '%s', password '%s'\n", fullpath, (char*)pUpdate->patchfile,
//		   (char*)pUpdate->server, (char*)pUpdate->login, (char*)pUpdate->password );
		pDownload->DownloadFile( (char*)pUpdate->server, (char*)pUpdate->login, (char*)pUpdate->password, fullpath, 
									(char*)pUpdate->patchfile, Game_Registry_Key() );
//		debugprint( "Call WOL_Download_Dialog()\n" );
		if( !WOL_Download_Dialog( pDownload, pDownloadSink, szTitle ) )
		{
			bReturn = false;
			break;
		}
		pUpdate = pUpdate->next;
	}
	::SetCurrentDirectory( szCurDirSave );

	//	Undo all the COM stuff.
//debugprint( "Undo all the COM stuff.\n" );
	pConnectionPoint = NULL;
	pContainer = NULL;
	hRes = pDownload->QueryInterface( IID_IConnectionPointContainer, (void**)&pContainer );
	_ASSERTE(SUCCEEDED(hRes));
	hRes = pContainer->FindConnectionPoint( IID_IDownloadEvent, &pConnectionPoint );
	_ASSERTE(SUCCEEDED(hRes));
	pConnectionPoint->Unadvise( dwDownloadAdvise );

	pContainer->Release();
	pConnectionPoint->Release();
	
	pDownload->Release();
	pDownloadSink->Release();	//	This results in pDownloadSink deleting itself for us.

	return bReturn;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnServerError( HRESULT hRes )
{
//	debugprint( ">>> OnServerError got: " );
	DebugChatDef( hRes );
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnMessageOfTheDay(HRESULT, LPCSTR)
{
	return S_OK;
}

//***********************************************************************************************
void RAChatEventSink::ActionEggSound( const char* szMessage )
{
	//	Easter egg related.
	if( strstr( szMessage, "<<groans>>" ) || strstr( szMessage, "<<groaning>>" ) || 
		strstr( szMessage, "<<dies>>" ) || strstr( szMessage, "<<dying>>" ) || strstr( szMessage, "<<groan>>" ) ||
		strstr( szMessage, "<<died>>" ) )
	{
		int i = rand() % 30;
		if( i == 0 )
			Sound_Effect( VOC_DOG_HURT );
		else if( i == 1 )
			Sound_Effect( VOC_ANTDIE );
		else
			Sound_Effect( (VocType)( VOC_SCREAM1 + rand() % 9 ) );
	}
	else if( strstr( szMessage, "<<whines>>" ) || strstr( szMessage, "<<whining>>" ) 
		|| strstr( szMessage, "<<bitching>>" ) || strstr( szMessage, "<<whine>>" ) )
		Sound_Effect( VOC_DOG_WHINE );
	else if( strstr( szMessage, "<<shoots>>" ) || strstr( szMessage, "<<shooting>>" ) || 
		strstr( szMessage, "<<shoot>>" ) || strstr( szMessage, "<<shot>>" ) )
	{
		switch( rand() % 6 )
		{
		case 0:			Sound_Effect( VOC_CANNON1 );			break;
		case 1:			Sound_Effect( VOC_CANNON2 );			break;
		case 2:			Sound_Effect( VOC_GUN_RIFLE );			break;
		case 3:			Sound_Effect( VOC_SILENCER );			break;
		case 4:			Sound_Effect( VOC_CANNON6 );			break;
		case 5:			Sound_Effect( VOC_CANNON8 );			break;
		}
	}
	else if( strstr( szMessage, "<<explodes>>" ) || strstr( szMessage, "<<exploding>>" ) || 
		strstr( szMessage, "<<explode>>" ) || strstr( szMessage, "<<exploded>>" ) ||
		strstr( szMessage, "<<boom>>" ) || strstr( szMessage, "<<nukes>>" ) )
	{
		switch( rand() % 5 )
		{
		case 0:			Sound_Effect( VOC_KABOOM1 );			break;
		case 1:			Sound_Effect( VOC_KABOOM12 );			break;
		case 2:			Sound_Effect( VOC_KABOOM15 );			break;
		case 3:			Sound_Effect( VOC_KABOOM30 );			break;
		case 4:			Sound_Effect( VOC_KABOOM25 );			break;
		}
	}
	else if( strstr( szMessage, "<<aye>>" ) || strstr( szMessage, "<<ok>>" ) ||
		strstr( szMessage, "<<yes>>" ) || strstr( szMessage, "<<yeah>>" ) )
	{
		switch( rand() % 8 )
		{
		case 0:		Sound_Effect( VOC_E_AH );					break;
		case 1:		Sound_Effect( VOC_E_YES );					break;
		case 2:		Sound_Effect( VOC_THIEF_YEA );				break;
		case 3:		Sound_Effect( VOC_SPY_YESSIR );				break;
		case 4:		Sound_Effect( VOC_SPY_INDEED );				break;
		case 5:		Sound_Effect( VOC_ENG_YES );				break;
		case 6:		Sound_Effect( VOC_MED_YESSIR );				break;
		case 7:		Sound_Effect( VOC_MED_AFFIRM );				break;
		}
	}
	else if( strstr( szMessage, "<<incredible>>" ) || strstr( szMessage, "<<adam>>" ) || strstr( szMessage, "<<Adam>>" ))
		Sound_Effect( VOC_E_OK );
	else if( strstr( szMessage, "<<coming>>" )|| strstr( szMessage, "<<on my way>>" ) || strstr( szMessage, "<<moving out>>" ) )
	{
		switch( rand() % 5 )
		{
		case 0:		Sound_Effect( VOC_SPY_ONWAY );				break;
		case 1:		Sound_Effect( VOC_ENG_MOVEOUT );			break;
		case 2:		Sound_Effect( VOC_SPY_KING );				break;
		case 3:		Sound_Effect( VOC_MED_MOVEOUT );			break;
		case 4:		Sound_Effect( VOC_THIEF_MOVEOUT );			break;
		}
	}
	else if( strstr( szMessage, "<<water>>" ) )
		Sound_Effect( VOC_SPLASH );
	else if( strstr( szMessage, "<<charging>>" ) || strstr( szMessage, "<<powering>>" ) )
		Sound_Effect( VOC_TESLA_POWER_UP );
	else if( strstr( szMessage, "<<zap>>" ) || strstr( szMessage, "<<zaps>>" ) )
		Sound_Effect( VOC_TESLA_ZAP );
	else if( strstr( szMessage, "<<torpedo>>" ) || strstr( szMessage, "<<torpedoes>>" ) )
		Sound_Effect( VOC_TORPEDO );
	else if( strstr( szMessage, "<<appears>>" ) || strstr( szMessage, "<<surfaces>>" ) || strstr( szMessage, "<<emerges>>" ))
		Sound_Effect( VOC_SUBSHOW );
	else if( strstr( szMessage, "<<bark>>" ) || strstr( szMessage, "<<barks>>" ) )
		Sound_Effect( VOC_DOG_BARK );
	else if( strstr( szMessage, "<<growl>>" ) || strstr( szMessage, "<<growls>>" ) )
		Sound_Effect( VOC_DOG_GROWL2 );
	else if( strstr( szMessage, "<<chronoshift>>" ) || strstr( szMessage, "<<disappears>>" ) )
		Sound_Effect( VOC_CHRONO );
	else if( strstr( szMessage, "<<crumble>>" ) || strstr( szMessage, "<<crumbles>>" ) ||
		strstr( szMessage, "<<collapse>>" ) || strstr( szMessage, "<<collapses>>" ) )
		Sound_Effect( VOC_CRUMBLE );
	else if( strstr( szMessage, "<<sell>>" ) || strstr( szMessage, "<<sells>>" ) ||
		strstr( szMessage, "<<cash>>" ) || strstr( szMessage, "<<money>>" ) )
		Sound_Effect( VOC_CASHTURN );
	else if( strstr( szMessage, "<<heal>>" ) || strstr( szMessage, "<<heals>>" ) )
		Sound_Effect( VOC_HEAL );
	else if( strstr( szMessage, "<<missile>>" ) )
	{
		switch( rand() % 3 )
		{
		case 0:		Sound_Effect( VOC_MISSILE_1 );				break;
		case 1:		Sound_Effect( VOC_MISSILE_2 );				break;
		case 2:		Sound_Effect( VOC_MISSILE_3 );				break;
		}
	}
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnPrivateAction( HRESULT, User* pUserSender, LPCSTR szMessage )
{
	//	Ignore private messages sent to myself by myself.
	if( pUserSender->flags & CHAT_USER_MYSELF )
		return S_OK;

	if( *szMessage )
	{
		char* szPrint = new char[ strlen( (char*)pUserSender->name ) + strlen( szMessage ) + 116 ];
		sprintf( szPrint, "%s %s %s", TXT_WOL_PRIVATE, (char*)pUserSender->name, szMessage );
		pOwner->PrintMessage( szPrint, WOLCOLORREMAP_ACTION );
		delete [] szPrint;
		//	Easter egg related.
		if( pOwner->bEggSounds )
			ActionEggSound( szMessage );
	}
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnPublicAction( HRESULT, Channel*, User* pUserSender, LPCSTR szMessage )
{
	if( *szMessage )
	{
		char* szPrint = new char[ strlen( (char*)pUserSender->name ) + strlen( szMessage ) + 110 ];
		sprintf( szPrint, "%s %s", (char*)pUserSender->name, szMessage );
		pOwner->PrintMessage( szPrint, WOLCOLORREMAP_ACTION );
		delete [] szPrint;
		//	Easter egg related.
		if( pOwner->bEggSounds )
			ActionEggSound( szMessage );
	}
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnPrivateGameOptions( HRESULT, User* pUser, LPCSTR szRequest )
{
//	debugprint( ">>> OnPrivateGameOptions\n" );
//	DebugChatDef( hRes );

	char szRequestCopy[ 600 ];
	strcpy( szRequestCopy, szRequest );

	if( pOwner->pGSupDlg )
	{
		if( pOwner->pGSupDlg->bHost )
			pOwner->pGSupDlg->ProcessGuestRequest( pUser, szRequestCopy );
		else
			pOwner->pGSupDlg->ProcessInform( szRequestCopy );		//	Must be private message to guest from game host.
	}
//	else
//		debugprint( "OnPrivateGameOptions bizarreness.\n" );

	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnPublicGameOptions( HRESULT, Channel*, User*, LPCSTR szInform )
{
//	debugprint( ">>> OnPublicGameOptions: %s\n", szInform );

	char szInformCopy[ 600 ];
	strcpy( szInformCopy, szInform );

	if( pOwner->pGSupDlg )
	{
		pOwner->pGSupDlg->ProcessInform( szInformCopy );
	}
//	else
//		debugprint( "OnPublicGameOptions bizarreness.\n" );

	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnGameStart( HRESULT hRes, Channel*, User* pUserIn, int iGameID )
{
	//	Note: All players receive this, not just the host that requested it.

//	debugprint( ">>> OnGameStart got: " );
	DebugChatDef( hRes );

//	if( bRequestGameStartWait )		//	Implies user is the host that did RequestGameStart().
//	{

		//	Create the list of users that are actually involved in a game.
		//	Most likely will always match pUserList, but there is a chance of someone leaving or joining
		//	at the wrong moment, so from this point on, the pGameUserList is used.

		//	Note: pUserIPList was added later, for pre-start pinging. It duplicates pGameUserList ip information,
		//	strictly speaking.

		//	Delete any existing list.
		while( pGameUserList )
		{
			User* pGameUserHead = pGameUserList;
			pGameUserList = pGameUserList->next;
			delete pGameUserHead;
		}
		//	Copy incoming user list.
		User* pGameUserListTail = NULL;
		while( pUserIn )
		{
//			debugprint( "OnGameStart got %s\n", (char*)pUserIn->name );
			User* pUserNew = new User;
			*pUserNew = *pUserIn;
			pUserNew->next = NULL;			//	(We don't want the value that was just copied!)
			if( !pGameUserListTail )
			{
				//	First User in list.
				pGameUserList = pUserNew;		//	This is the head of our User list.
				pGameUserListTail = pUserNew;
			}
			else
			{
				pGameUserListTail->next = pUserNew;
				pGameUserListTail = pUserNew;
			}
			pUserIn = pUserIn->next;
		}

		bRequestGameStartWait = false;
//	}

//	debugprint( "iGameID is %i\n", iGameID );
	this->iGameID = iGameID;

	return S_OK;
}

//***********************************************************************************************
unsigned long RAChatEventSink::GetPlayerGameIP( const char* szPlayerName ) const
{
	//	Returns ipaddr value of player if found in pGameUserList, else 0.
	User* pUser = pGameUserList;
	while( pUser )
	{
		if( _stricmp( (char*)pUser->name, szPlayerName ) == 0 )
			return pUser->ipaddr;
		pUser = pUser->next;
	}
	return 0;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnUserKick( HRESULT hRes, Channel*, User* pUserKicked,  User* pUserKicker )
{
//	debugprint( ">>> OnUserKick got: " );
	DebugChatDef( hRes );

	if( hRes == S_OK )
	{
		//	Someone was kicked.
		//	Fake a call to OnChannelLeave(), as the processing is identical.
		OnChannelLeave( S_OK, NULL, pUserKicked );
		if( pUserKicked->flags & CHAT_USER_MYSELF )
		{
			//	Trigger a channel exit later on, when we have left this callback.
			bGotKickedTrigger = true;
			char* szPrint = new char[ strlen( (char*)pUserKicker->name ) + strlen( TXT_WOL_USERKICKEDYOU ) + 5 ];
			sprintf( szPrint, TXT_WOL_USERKICKEDYOU, (char*)pUserKicker->name );
			pOwner->PrintMessage( szPrint, WOLCOLORREMAP_KICKORBAN );
			delete [] szPrint;
			//	Ensure that the bGotKickedTrigger is acted upon immediately...
			pOwner->dwTimeNextWolapiPump = ::timeGetTime();
		}
		else
		{
			char* szPrint = new char[ strlen( (char*)pUserKicker->name ) + strlen( (char*)pUserKicked->name ) + 
										strlen( TXT_WOL_USERKICKEDUSER ) + 5 ];
			sprintf( szPrint, TXT_WOL_USERKICKEDUSER, (char*)pUserKicker->name, (char*)pUserKicked->name );
			pOwner->PrintMessage( szPrint, WOLCOLORREMAP_KICKORBAN );
			delete [] szPrint;
		}
		switch( rand() % 4 )
		{
		case 0:
			Sound_Effect( VOC_TANYA_CHEW );
			break;
		case 1:
			Sound_Effect( VOC_TANYA_LAUGH );
			break;
		case 2:
			Sound_Effect( VOC_TANYA_CHING );
			break;
		case 3:
			Sound_Effect( VOC_TANYA_KISS );
			break;
		}
	}
	else
	{
		//	You tried to kick someone, but the user wasn't found.
		//	Ignore.
//		debugprint( "OnUserKick non S_OK value\n" );
	}

	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnUserIP( HRESULT hRes, User* pUser )
{
	//	A list of users is kept, separate from other user lists, to preserve the ipaddr's we've found through this
	//	callback. OnUserList (for some dumb reason) doesn't hold valid ipaddr's, so we have to go through
	//	all this rigamarole...
	//	(List is cleared when entering game channel. Users are added initially and on joins, not removed on leaves.)
//	debugprint( ">>> OnUserIP got: " );
	DebugChatDef( hRes );

	if( SUCCEEDED( hRes ) )
	{
		//	Look for user in our current users list.
		User* pUserSearch = pUserIPList;
		while( pUserSearch )
		{
			if( _stricmp( (char*)pUserSearch->name, (char*)pUser->name ) == 0 )
			{
				//	Found matching user. Replace it's ipaddr value, in case it changed.(?)
				pUserSearch->ipaddr = pUser->ipaddr;
				return S_OK;
			}
			pUserSearch = pUserSearch->next;
		}
		//	User not found in current list. Add.
		User* pUserNew = new User;
		*pUserNew = *pUser;
		pUserNew->next = NULL;			//	(We don't want the value that was just copied!)
		if( !pUserIPListTail )
		{
			//	First user in list.
			pUserIPList = pUserNew;		//	This is the head of our list.
			pUserIPListTail = pUserNew;
		}
		else
		{
			pUserIPListTail->next = pUserNew;
			pUserIPListTail = pUserNew;
		}
	}
	return S_OK;
}

//***********************************************************************************************
void RAChatEventSink::DeleteUserIPList()
{
	//	Same as DeleteUserList but for pUserIPList.
//	debugprint( "DeleteUserIPList\n" );
	while( pUserIPList )
	{
		User* pUserHead = pUserIPList;
		pUserIPList = pUserHead->next;
		delete pUserHead;
	}
	pUserIPListTail = NULL;
}

//***********************************************************************************************
unsigned long RAChatEventSink::GetUserIP( const char* szName ) const
{
	//	Looks in pUserIPList for the ipaddr of user with name szName.
	//	This is used only while in game channels.
	//	This is for step 2 in acquiring fellow player ping times. To get the IP addresses into pUserIPList
	//	we had to go through request/callbacks. Now pings are requested on these addresses, and the results
	//	tallied in NetUtilSink for our retrieval later.
	//	Returns 0 if not found.

	//	Find szName in list.
	User* pUser = pUserIPList;
	while( pUser )
	{
		if( _stricmp( (char*)pUser->name, szName ) == 0 )
			return pUser->ipaddr;
		pUser = pUser->next;
	}
	return 0;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnServerError(HRESULT , LPCSTR )
{
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnServerBannedYou(HRESULT , time_t )
{
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnUserFlags( HRESULT hRes, LPCSTR name, unsigned int flags, unsigned int )
{
//	debugprint( ">>> OnUserFlags got: " );
	DebugChatDef( hRes );

	if( pOwner->CurrentLevel == WOL_LEVEL_INGAMECHANNEL )
	{
		if( pOwner->pGSupDlg && 
			( pOwner->pGSupDlg->bHostSayGo || pOwner->pGSupDlg->bHostWaitingForGoTrigger || 
				pOwner->pGSupDlg->bExitForGameTrigger || iGameID ) )
		{
			//	A game has this moment entered the "must start" phase. We must ignore the fact that others are leaving the channel.
//			debugprint( "Ignoring OnUserFlags because game is starting.\n" );		//	(Shouldn't ever happen.)
			return S_OK;
		}
	}

	//	Find user in our current users list.
	User* pUserPrior = NULL;
	User* pUserSearch = pUserList;
	while( pUserSearch )
	{
		if( _stricmp( (char*)pUserSearch->name, name ) == 0 )
		{
			//	Set user's flags to new value.
			pUserSearch->flags = flags;
			
			//	Remove user from userlist and reinsert appropriately sorted.
			if( !pUserPrior )
			{
				//	User was head of list.
				pUserList = pUserSearch->next;
				if( pUserSearch == pUserTail )
					//	User was also tail of list.
					pUserTail = NULL;
				else
					pUserSearch->next = NULL;
			}
			else
			{
				//	User was not head of list.
				pUserPrior->next = pUserSearch->next;
				if( pUserSearch == pUserTail )
					//	User was tail of list.
					pUserTail = pUserPrior;
				else
					pUserSearch->next = NULL;
			}
			InsertUserSorted( pUserSearch );

			//	Update shown list.
			pOwner->ListChannelUsers();
			break;
		}
		pUserPrior = pUserSearch;
		pUserSearch = pUserSearch->next;
	}

	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RAChatEventSink::OnChannelBan( HRESULT , LPCSTR name, int banned )
{
	if( banned && strcmp( name, "*" ) != 0 )
	{
		char* szPrint = new char[ strlen( name ) + strlen( TXT_WOL_USERWASBANNED ) + 5 ];
		sprintf( szPrint, TXT_WOL_USERWASBANNED, name );
		pOwner->PrintMessage( szPrint, WOLCOLORREMAP_KICKORBAN );
		delete [] szPrint;
	}
	
	return S_OK;
}


//***********************************************************************************************
//***********************************************************************************************
RADownloadEventSink::RADownloadEventSink() :
	bFlagEnd( false ),
	bFlagError( false ),
	bFlagProgressUpdate( false ),
	bFlagStatusUpdate( false ),
	bFlagQueryResume( false )
{
	m_cRef=0;		// Ref counter
}

//			Interface IUnknown Methods
//***********************************************************************************************
// QueryInterface
//
HRESULT __stdcall
RADownloadEventSink::QueryInterface(const IID& iid, void** ppv)
{
	if ((iid == IID_IUnknown) ||(iid == IID_IDownloadEvent))
	{
		*ppv = (IDownloadEvent*)this;		//	Removed static_cast<> ajw
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();		//	Removed reinterpret_cast<> ajw
	return S_OK ;
}

//***********************************************************************************************
// AddRef
//
ULONG __stdcall
RADownloadEventSink::AddRef()
{
	return InterlockedIncrement(&m_cRef) ;
}

//***********************************************************************************************
// Release
//
ULONG __stdcall
RADownloadEventSink::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this ;
		return 0 ;
	}
	return m_cRef;
}

//***********************************************************************************************
//***********************************************************************************************
STDMETHODIMP RADownloadEventSink::OnEnd(void)
{
//	debugprint( ">>> OnEnd\n" );
	bFlagEnd = true;
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RADownloadEventSink::OnError( int /*iCode*/ )
{
//	debugprint( ">>> OnError got: %i\n", iCode );
//#define DOWNLOADEVENT_NOSUCHSERVER		1
//#define DOWNLOADEVENT_COULDNOTCONNECT		2
//#define DOWNLOADEVENT_LOGINFAILED			3
//#define DOWNLOADEVENT_NOSUCHFILE			4
//#define DOWNLOADEVENT_LOCALFILEOPENFAILED	5
//#define DOWNLOADEVENT_TCPERROR			6
//#define DOWNLOADEVENT_DISCONNECTERROR		7

	bFlagError = true;
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RADownloadEventSink::OnProgressUpdate( int bytesread, int totalsize, int timetaken, int timeleft )
{
//	debugprint( ">>> OnProgressUpdate\n" );
	bFlagProgressUpdate = true;

	iBytesRead = bytesread;
	iTotalSize = totalsize;
	iTimeTaken = timetaken;
	iTimeLeft = timeleft;

	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RADownloadEventSink::OnStatusUpdate( int status )
{
//	debugprint( ">>> OnStatusUpdate, status = %i\n", status );
	bFlagStatusUpdate = true;

	iStatus = status;

	return S_OK;
}

//***********************************************************************************************
// Just tell the FTP module to go ahead and resume
//
STDMETHODIMP RADownloadEventSink::OnQueryResume()
{
//	debugprint( ">>> OnQueryResume\n" );
	bFlagQueryResume = true;

	bResumed = true;

	return DOWNLOADEVENT_RESUME;
}


//***********************************************************************************************
//***********************************************************************************************
RANetUtilEventSink::RANetUtilEventSink( WolapiObject* pOwnerIn ) : m_cRef( 0 ), 	//	init the reference count
	pOwner( pOwnerIn ),
	pLadderList( NULL ),
	pLadderTail( NULL ),
	pLadderListAM( NULL ),
	pLadderTailAM( NULL )
{
//	debugprint( "RANetUtilEventSink constructor\n" );
}

//***********************************************************************************************
RANetUtilEventSink::~RANetUtilEventSink()
{
//	debugprint( "RANetUtilEventSink destructor\n" );
	DeleteLadderList();
}

//			Interface IUnknown Methods
//***********************************************************************************************
// QueryInterface
//
HRESULT __stdcall
RANetUtilEventSink::QueryInterface(const IID& iid, void** ppv)
{
//	debugprint( "RANetUtilEventSink::QueryInterface\n" );
	if ((iid == IID_IUnknown) ||(iid == IID_INetUtilEvent))
	{
		*ppv = (INetUtilEvent*)this;		//	Removed static_cast<> ajw
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();		//	Removed reinterpret_cast<> ajw
	return S_OK ;
}

//***********************************************************************************************
// AddRef
//
ULONG __stdcall
RANetUtilEventSink::AddRef()
{
//	debugprint( "RANetUtilEventSink::AddRef\n" );
	return InterlockedIncrement(&m_cRef) ;
}

//***********************************************************************************************
// Release
//
ULONG __stdcall
RANetUtilEventSink::Release()
{
//	debugprint( "RANetUtilEventSink::Release\n" );
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this ;
		return 0 ;
	}
	return m_cRef;
}

//***********************************************************************************************
STDMETHODIMP RANetUtilEventSink::OnGameresSent( HRESULT hRes )
{
//	debugprint( ">>> OnGameresSent got: " );
	DebugChatDef( hRes );
	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RANetUtilEventSink::OnLadderList( HRESULT hRes, Ladder* pLadderListIn, int /*totalCount*/, long /*timeStamp*/, int /*keyRung*/ )
{
	//	Maintenance of ladders list is like that for channels list above.
	//	DeleteLadderList();		-> This is done once, before a set of RequestLadderList() calls are made.
//	debugprint( ">>> OnLadderList got: " );
	DebugChatDef( hRes );

	if( SUCCEEDED( hRes ) )
	{
//		debugprint( "(SUCCEEDED)\n" );
		//	Copy ladder list to our own list.
		while( pLadderListIn )
		{
//			debugprint( "OnLadderList got %s, rung %u\n", pLadderListIn->login_name, pLadderListIn->rung );
			if( *pLadderListIn->login_name != 0 && pLadderListIn->rung != -1 )
			{
				Ladder* pLadderNew = new Ladder;
				*pLadderNew = *pLadderListIn;
				pLadderNew->next = NULL;			//	(We don't want the value that was just copied!)
				if( pLadderNew->sku == LADDER_CODE_RA )
				{
					if( !pLadderTail )
					{
						//	First Ladder in list.
						pLadderList = pLadderNew;		//	This is the head of our Ladder list.
						pLadderTail = pLadderNew;
					}
					else
					{
						pLadderTail->next = pLadderNew;
						pLadderTail = pLadderNew;
					}
					if( _stricmp( (char*)pLadderNew->login_name, pOwner->szMyName ) == 0 )
					{
						//	Set up local player's win/loss string.
						sprintf( pOwner->szMyRecord, TXT_WOL_PERSONALWINLOSSRECORD, pOwner->szMyName, 
									pLadderNew->rung, pLadderNew->wins, pLadderNew->losses, pLadderNew->points );
						pOwner->bMyRecordUpdated = true;
					}
				}
				else		//	sku must be LADDER_CODE_AM
				{
					if( !pLadderTailAM )
					{
						//	First Ladder in list.
						pLadderListAM = pLadderNew;		//	This is the head of our Ladder list.
						pLadderTailAM = pLadderNew;
					}
					else
					{
						pLadderTailAM->next = pLadderNew;
						pLadderTailAM = pLadderNew;
					}
					if( _stricmp( (char*)pLadderNew->login_name, pOwner->szMyName ) == 0 )
					{
						//	Set up local player's win/loss string for Aftermath.
						sprintf( pOwner->szMyRecordAM, TXT_WOL_PERSONALWINLOSSRECORDAM, pOwner->szMyName, 
									pLadderNew->rung, pLadderNew->wins, pLadderNew->losses, pLadderNew->points );
						pOwner->bMyRecordUpdated = true;
					}
				}
			}
			pLadderListIn = pLadderListIn->next;
		}
		//	Update shown list.
		pOwner->ListChannelUsers();
	}

	return S_OK;
}

//***********************************************************************************************
STDMETHODIMP RANetUtilEventSink::OnPing( HRESULT hRes, int time, unsigned long ip, int /*handle*/ )
{
	if( pOwner->bDoingDisconnectPinging )
	{
//		debugprint( ">>> OnPing got : ip %i, time %i, ", ip, time );
		DebugChatDef( hRes );

		if( ip == pOwner->TournamentOpponentIP )
		{
			//	This is the result of the opponent ping.
			if( time != -1 )
				pOwner->DisconnectPingResult_Opponent[ pOwner->iDisconnectPingCurrent ] = PING_GOOD;
			else
				pOwner->DisconnectPingResult_Opponent[ pOwner->iDisconnectPingCurrent ] = PING_BAD;
//			debugprint( "Set ping #%i for Opponent\n", pOwner->iDisconnectPingCurrent );
		}
		else
		{
			//	This is the result of the game server ping.
			if( time != -1 )
				pOwner->DisconnectPingResult_Server[ pOwner->iDisconnectPingCurrent ] = PING_GOOD;
			else
				pOwner->DisconnectPingResult_Server[ pOwner->iDisconnectPingCurrent ] = PING_BAD;
//			debugprint( "Set ping #%i for Server\n", pOwner->iDisconnectPingCurrent );
		}
	}
	return S_OK;
}

//***********************************************************************************************
void RANetUtilEventSink::DeleteLadderList()
{
//	debugprint( "DeleteLadderList()\n" );
	//	Delete all Ladders allocated on the heap.
	while( pLadderList )
	{
		Ladder* pLadderHead = pLadderList;
		pLadderList = pLadderHead->next;
		delete pLadderHead;
	}
	pLadderTail = NULL;
}

//***********************************************************************************************
unsigned int RANetUtilEventSink::GetUserRank( const char* szName, bool bRankRA )
{
	//	Searches for szName in ladder list, returns player rank if found, else 0.
	//	Slow linear search.
	//	If bRankRA, returns RA rank, else returns AM rank.
//	debugprint( "GetUserRank: Asked for %s, ", szName );
	Ladder* pLad;
	if( bRankRA )
		pLad = pLadderList;
	else
		pLad = pLadderListAM;

	while( pLad )
	{
//		debugprint( "  comparing %s\n", (char*)pLad->login_name );
		if( _stricmp( (char*)pLad->login_name, szName ) == 0 )
		{
//			debugprint( "found rung value %u\n", pLad->rung );
			return pLad->rung;
		}
		pLad = pLad->next;
	}
//	debugprint( "couldn't find in my ladder list.\n", szName );

	return 0;
}

//***********************************************************************************************
//***********************************************************************************************
void ChatDefAsText( char* szDesc, HRESULT hRes )
{
	//	Sets szDesc to the text meaning of hRes.
	//	Make sure szDesc is as long as the longest of the below.
	switch( hRes )
	{
	case CHAT_E_NICKINUSE       :
		sprintf( szDesc, "Your nick is still logged into chat" );
		break;
	case CHAT_E_BADPASS         :
		sprintf( szDesc, "Your password is incorrect during login" );
		break;
	case CHAT_E_NONESUCH		   :
		sprintf( szDesc, "Reference made to non-existant user or channel" );
		break;
	case CHAT_E_CON_NETDOWN        :
		sprintf( szDesc, "The network layer is down or cannot be initialized for some reason" );
		break;
	case CHAT_E_CON_LOOKUP_FAILED  :
		sprintf( szDesc, "Name lookup (e.g DNS) failed for some reason" );
		break;
	case CHAT_E_CON_ERROR          :
		sprintf( szDesc, "Some fatal error occured with the net connection" );
		break;
	case CHAT_E_TIMEOUT            :
		sprintf( szDesc, "General request timeout for a request" );
		break;
	case CHAT_E_MUSTPATCH        :
		sprintf( szDesc, "Must patch before continuing" );
		break;
	case CHAT_E_STATUSERROR		:
		sprintf( szDesc, "Miscellaneous internal status error" );
		break;
	case CHAT_E_UNKNOWNRESPONSE	:
		sprintf( szDesc, "Server has returned something we don't recognise" );
		break;
	case CHAT_E_CHANNELFULL		:
		sprintf( szDesc, "Tried to join a channel that has enough players already" );
		break;
	case CHAT_E_CHANNELEXISTS	:
		sprintf( szDesc, "Tried to create a channel that already exists" );
		break;
	case CHAT_E_CHANNELDOESNOTEXIST		:
		sprintf( szDesc, "Tried to join a channel that does not exist" );
		break;
	case CHAT_E_BADCHANNELPASSWORD		:
		sprintf( szDesc, "Tried to join a channel with the wrong password" );
		break;
	case CHAT_E_BANNED            :
		sprintf( szDesc, "You've been banned from the server / channel" );
		break;
	case CHAT_E_NOT_OPER           :
		sprintf( szDesc, "You tried to do something that required operator status" );
		break;

	case CHAT_S_CON_CONNECTING     :
		sprintf( szDesc, "A network connection is underway" );
		break;
	case CHAT_S_CON_CONNECTED      :
		sprintf( szDesc, "A network connection is complete" );
		break;
	case CHAT_S_CON_DISCONNECTING  :
		sprintf( szDesc, "A network connection is going down" );
		break;
	case CHAT_S_CON_DISCONNECTED   :
		sprintf( szDesc, "A network connection is closed" );
		break;
	case CHAT_S_FIND_NOTHERE   :
		sprintf( szDesc, "Find - Nick not in system" );
		break;
	case CHAT_S_FIND_NOCHAN   :
		sprintf( szDesc, "Find - Not in any channels" );
		break;
	case CHAT_S_FIND_OFF   :
		sprintf( szDesc, "Find - user has find turned off" );
		break;
	case CHAT_S_PAGE_NOTHERE   :
		sprintf( szDesc, "Page - Nick not in system" );
		break;
	case CHAT_S_PAGE_OFF   :
		sprintf( szDesc, "Page - user has page turned off" );
		break;
	case CHAT_E_NOTCONNECTED    :
		sprintf( szDesc, "You are not connected to the chat server" );
		break;
	case CHAT_E_NOCHANNEL       :
		sprintf( szDesc, "You are not in a channel" );
		break;
	case CHAT_E_NOTIMPLEMENTED  :
		sprintf( szDesc, "Feature is not implemented" );
		break;
	case CHAT_E_PENDINGREQUEST  :
		sprintf( szDesc, "The request was made while while a conflicting request was still pending" );
		break;
	case CHAT_E_PARAMERROR	   :
		sprintf( szDesc, "Invalid parameter passed - usually a NULL pointer" );
		break;
	case CHAT_E_LEAVECHANNEL		:
		sprintf( szDesc, "Tried to create or join a channel before leaving the previous one" );
		break;
	case CHAT_E_JOINCHANNEL		:
		sprintf( szDesc, "Tried to send something to a channel when not a member of any channel" );
		break;
	case CHAT_E_UNKNOWNCHANNEL	:
		sprintf( szDesc, "Tried to join a non-existant channel" );
		break;
	case S_OK:
		sprintf( szDesc, "S_OK" );
		break;
	case E_FAIL:
		sprintf( szDesc, "E_FAIL" );
		break;
	default:
		sprintf( szDesc, "ERROR - Value not recognized!" );
		break;
	}
	//	Append NetUtil errors.
	switch( hRes )
	{
	case NETUTIL_E_ERROR:
		strcat( szDesc, "  NetUtil: NETUTIL_E_ERROR" );
		break;
	case NETUTIL_E_BUSY:
		strcat( szDesc, "  NetUtil: NETUTIL_E_BUSY"  );
		break;
	case NETUTIL_S_FINISHED:
		strcat( szDesc, "  NetUtil: NETUTIL_S_FINISHED" );
		break;
	}
}

//***********************************************************************************************
void DebugChatDef( HRESULT hRes )
{
	char szText[200];
	ChatDefAsText( szText, hRes );
//	debugprint( "%s\n", szText );
}

//***********************************************************************************************
int iChannelLobbyNumber( const unsigned char* szChannelName )
{
	//	Returns lobby number of channel, or -1 for "channel is not a lobby".
	if( strncmp( (char*)szChannelName, LOB_PREFIX, strlen( LOB_PREFIX ) ) == 0 )
	{
		char szNum[10];
		strcpy( szNum, (char*)szChannelName + strlen( LOB_PREFIX ) );
//		debugprint( " ^ iChannelLobbyNumber returning atoi of %s\n", szNum );
		return atoi( szNum );
	}
	else
		return -1;
}

//***********************************************************************************************
void InterpretLobbyNumber( char* szLobbyNameToSet, int iLobby )
{
	//	Hard-coded translation of lobby number to apparent lobby name.
	switch( iLobby )
	{
	case 0:
		strcpy( szLobbyNameToSet, "Combat Alley" );
		break;
	case 1:
		strcpy( szLobbyNameToSet, "No Man's Land" );
		break;
	case 2:
		strcpy( szLobbyNameToSet, "Hell's Pass" );
		break;
	case 3:
		strcpy( szLobbyNameToSet, "Lost Vegas" );
		break;
	case 4:
		strcpy( szLobbyNameToSet, "Death Valley" );
		break;
	case 5:
		strcpy( szLobbyNameToSet, "The Wastelands" );
		break;
	case 6:
		strcpy( szLobbyNameToSet, "Isle of Fury" );
		break;
	case 7:
		strcpy( szLobbyNameToSet, "Armourgarden" );
		break;
	case 8:
		strcpy( szLobbyNameToSet, "The Hive" );
		break;
	case 9:
		strcpy( szLobbyNameToSet, "North by Northwest" );
		break;
	case 10:
		strcpy( szLobbyNameToSet, "Decatur High" );
		break;
	case 11:
		strcpy( szLobbyNameToSet, "Damnation Alley" );
		break;
	default:
		sprintf( szLobbyNameToSet, "%ith Division", iLobby );
		break;
	}
}

#endif