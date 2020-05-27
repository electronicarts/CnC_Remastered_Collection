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

//	wol_chat.cpp
//	ajw 7/8/98

#include "function.h"
#include "iconlist.h"
#include "WolapiOb.h"
#include "SEditDlg.h"
#include "WolStrng.h"
#include "ToolTip.h"

//#include "WolDebug.h"

void WOL_PrintMessage( IconListClass& ILTarget, const char* szText, PlayerColorType iColorRemap = PCOLOR_NONE );
void WOL_PrintMessage( IconListClass& ILTarget, const char* szText, RemapControlType* pColorRemap );
bool EnterChannel( WolapiObject* pWO, IconListClass& chatlist, Channel* pChannel, char* szChannelName, bool bGame );
bool EnterChannel( WolapiObject* pWO, IconListClass& chatlist, IconListClass& chanlist, int iIndex, bool bGame );
bool ExitChatChannel( WolapiObject* pWO );
void CreateChatChannel( WolapiObject* pWO );
bool CreateGameChannel( WolapiObject* pWO, const CREATEGAMEINFO& cgi );
bool ProcessChannelListSelection( WolapiObject* pWO, IconListClass& chatlist, IconListClass& chanlist, int iIndex );

enum LIST_EXPAND_STATE
{
	LES_NORMAL,
	LES_CHANNELS_EXPANDED,
	LES_USERS_EXPANDED,
};
static LIST_EXPAND_STATE	lesCurrent = LES_NORMAL;

bool OnExpandChannelList( IconListClass& chanlist, IconListClass& userlist );
bool OnExpandUserList( IconListClass& chanlist, IconListClass& userlist );
void ResizeChannelList( IconListClass& chanlist, bool bExpand );
void ResizeUserList( IconListClass& userlist, bool bExpand );

bool bLinkInList( const LinkClass* pListHead, const LinkClass* pLinkToFind );

extern CREATEGAMEINFO WOL_CreateGame_Dialog( WolapiObject* pWO );

static int d_chanlist_w;
static int d_chanlist_h;
static int d_chanlist_x;
static int d_chanlist_y;

static int d_userlist_w;
static int d_userlist_h;
static int d_userlist_x;
static int d_userlist_y;

#define DRAWTOGDOWN		Turn_Off()
#define DRAWTOGUP		Turn_On()

//***********************************************************************************************
int WOL_Chat_Dialog( WolapiObject* pWO )
{
	int rc;
	bool bFirsttime = true;
	bool bHackFocus = true;

	//------------------------------------------------------------------------
	//	Dialog & button dimensions
	//------------------------------------------------------------------------
	int d_dialog_w = 320 *RESFACTOR;											// dialog width
	int d_dialog_h = 200 *RESFACTOR;											// dialog height
	int d_dialog_x = ((320*RESFACTOR - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*RESFACTOR - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_text_h = 12;
	int d_margin1 = 34;															// large margin
	int d_margin2 = 14;															// small margin

	int d_chatlist_w = 340;
	int d_chatlist_x = d_dialog_x + d_margin1;
	int d_chatlist_y = d_dialog_y + d_margin2 + d_margin1 + 27;
	int d_chatlist_h = 337 - d_chatlist_y;

	d_chanlist_w = 227;
	d_chanlist_h = 50 * RESFACTOR;
	d_chanlist_x = d_dialog_x + d_dialog_w - (d_margin1 + d_chanlist_w);
	d_chanlist_y = d_chatlist_y;

	d_userlist_w = d_chanlist_w;
//		int d_userlist_h = ((10 * 6) + 3) *RESFACTOR;
	d_userlist_x = d_chanlist_x;
	d_userlist_y = d_chanlist_y + d_chanlist_h + 14 + 5;

	d_userlist_h = d_chatlist_y + d_chatlist_h - d_userlist_y;

	int d_action_w = 100;
	int d_action_h = 9 *RESFACTOR;
	int d_action_x = d_dialog_x + 500;
	int d_action_y = 365;

//	int d_chanpriv_w = 60;
//	int d_chanpriv_h = 9 *RESFACTOR;
//	int d_chanpriv_x = d_dialog_x + 150;
//	int d_chanpriv_y = d_action_y;

//	int d_cgame_w = 60;
//	int d_cgame_h = 9 *RESFACTOR;
//	int d_cgame_x = d_dialog_x + 390; //d_dialog_cx - d_cgame_w / 2;
//	int d_cgame_y = d_action_y;

	int d_back_w = 100;
	int d_back_h = 9 *RESFACTOR;
	int d_back_x = d_dialog_x + 100;
	int d_back_y = d_action_y;

	int d_join_w = 100;
	int d_join_h = 9 *RESFACTOR;
	int d_join_x = d_dialog_x + 210;
	int d_join_y = d_action_y;

	int d_create_w = 100;
	int d_create_h = 9 *RESFACTOR;
	int d_create_x = d_dialog_x + 320;	//((d_dialog_w * 5) / 6) - (d_create_w / 2);
	int d_create_y = d_action_y;

	int d_send_w = d_chanlist_x + d_chanlist_w - d_chatlist_x;
	int d_send_h = 9 *RESFACTOR;
	int d_send_x = d_chatlist_x;
	int d_send_y = d_chatlist_y + d_chatlist_h + 5;

	//------------------------------------------------------------------------
	//	Button Enumerations
	//------------------------------------------------------------------------
	enum 
	{
		BUTTON_DISCONNECT = 100,	//	Note: standard WOL button IDs must match values in WolapiObject::PrepareButtonsAndIcons().
		BUTTON_LEAVE,
		BUTTON_REFRESH,
		BUTTON_SQUELCH,
		BUTTON_BAN,
		BUTTON_KICK,
		BUTTON_FINDPAGE,
		BUTTON_OPTIONS,
		BUTTON_LADDER,
		BUTTON_HELP,

		BUTTON_CHATLIST,
		BUTTON_CHANLIST,
		BUTTON_USERLIST,
		BUTTON_SENDEDIT,
		BUTTON_ACTION,
//		BUTTON_CGAME,
		BUTTON_CREATE,
		BUTTON_JOIN,
		BUTTON_BACK,
		BUTTON_EXPANDCHANNELS,
		BUTTON_EXPANDUSERS,
		BUTTON_RANKRA,
		BUTTON_RANKAM
	};

	//------------------------------------------------------------------------
	//	Redraw values: in order from "top" to "bottom" layer of the dialog
	//------------------------------------------------------------------------
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	//------------------------------------------------------------------------
	//	Dialog variables
	//------------------------------------------------------------------------
	RedrawType display = REDRAW_ALL;		// redraw level
	bool process = true;						// process while true
	KeyNumType input;

	TTimerClass<SystemTimerClass> lastclick_timer;
	int lastclick_idx = 0;					// index of item last clicked on
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	//------------------------------------------------------------------------
	//	Buttons
	//------------------------------------------------------------------------
	GadgetClass *commands;										// button list

	char* pShpExpand = (char*)MFCD::Retrieve( "exp.shp" );
	char* pShpUnexpand = (char*)MFCD::Retrieve( "unexp.shp" );

	IconListClass chatlist( BUTTON_CHATLIST, d_chatlist_x, d_chatlist_y, d_chatlist_w, d_chatlist_h, TPF_TYPE, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"), true, 0, 500 );
	ShapeButtonClass ExpandChanBtn( BUTTON_EXPANDCHANNELS, pShpExpand, d_chanlist_x + d_chanlist_w - 17, d_chanlist_y - 14 );
	IconListClass chanlist( BUTTON_CHANLIST, d_chanlist_x, d_chanlist_y, d_chanlist_w, d_chanlist_h, TPF_TYPE, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"), true, 1 );
	ShapeButtonClass ExpandUserBtn( BUTTON_EXPANDUSERS, pShpExpand, d_userlist_x + d_userlist_w - 17, d_userlist_y - 14 );
	IconListClass userlist( BUTTON_USERLIST, d_userlist_x, d_userlist_y, d_userlist_w, d_userlist_h, TPF_TYPE, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"), true, 2 );
	TextButtonClass ActionBtn( BUTTON_ACTION, TXT_WOL_ACTION, TPF_BUTTON, d_action_x, d_action_y, d_action_w );
	TextButtonClass CreateBtn( BUTTON_CREATE, TXT_WOL_NEWSOMETHING, TPF_BUTTON, d_create_x, d_create_y, d_create_w );
	TextButtonClass JoinBtn( BUTTON_JOIN, TXT_WOL_JOIN, TPF_BUTTON, d_join_x, d_join_y, d_join_w );
	TextButtonClass BackBtn( BUTTON_BACK, TXT_WOL_BACK, TPF_BUTTON, d_back_x, d_back_y, d_back_w );
	char* szRecordToStartWith;
	if( pWO->bShowRankRA )
		szRecordToStartWith = pWO->szMyRecord;
	else
		szRecordToStartWith = pWO->szMyRecordAM;
	StaticButtonClass chatlistTitle( 0, szRecordToStartWith, TPF_TYPE, d_chatlist_x + 2, d_chatlist_y - 13, d_chatlist_w - 4, 12 );
	StaticButtonClass chanlistTitle( 0, "", TPF_TYPE, d_chanlist_x + 2, d_chanlist_y - 16 + 4, d_chanlist_w - 4 - 16, 12 );
	StaticButtonClass userlistTitle( 0, TXT_WOL_NOUSERLIST, TPF_TYPE, d_userlist_x + 2, d_userlist_y - 16 + 4, d_userlist_w - 4 - 16 * 4, 12 );

	char szSendBuffer[MAXCHATSENDLENGTH] = "";
	EditClass sendedit( BUTTON_SENDEDIT, szSendBuffer, MAXCHATSENDLENGTH, TPF_TEXT, d_send_x, d_send_y, d_send_w, d_send_h );

	char* pShpRankRA = (char*)MFCD::Retrieve( "rank_ra.shp" );
	char* pShpRankAM = (char*)MFCD::Retrieve( "rank_am.shp" );
	ShapeButtonClass RankRABtn( BUTTON_RANKRA, pShpRankRA, d_userlist_x + d_userlist_w - ( 16 * 4 + 1 ), d_userlist_y - 14 );
	ShapeButtonClass RankAMBtn( BUTTON_RANKAM, pShpRankAM, d_userlist_x + d_userlist_w - ( 16 * 3 + 1 ), d_userlist_y - 14 );
	//	Change draw behavior of toggle buttons.
	RankRABtn.ReflectButtonState = true;
	RankAMBtn.ReflectButtonState = true;

	//	Build the button list.
	commands = pWO->pShpBtnDiscon;
	pWO->pShpBtnLeave->Add_Tail(*commands);
	pWO->pShpBtnRefresh->Add_Tail(*commands);
	pWO->pShpBtnSquelch->Add_Tail(*commands);
	pWO->pShpBtnBan->Add_Tail(*commands);
	pWO->pShpBtnKick->Add_Tail(*commands);
	pWO->pShpBtnFindpage->Add_Tail(*commands);
	pWO->pShpBtnOptions->Add_Tail(*commands);
	pWO->pShpBtnLadder->Add_Tail(*commands);
	pWO->pShpBtnHelp->Add_Tail(*commands);
	chatlist.Add_Tail(*commands);
	ExpandChanBtn.Add_Tail(*commands);
	chanlist.Add_Tail(*commands);
	ExpandUserBtn.Add_Tail(*commands);
	userlist.Add_Tail(*commands);
	ActionBtn.Add_Tail(*commands);
	CreateBtn.Add_Tail(*commands);
//	CreatePrivBtn.Add_Tail(*commands);
	JoinBtn.Add_Tail(*commands);
	BackBtn.Add_Tail(*commands);
//	CGameBtn.Add_Tail(*commands);
	chatlistTitle.Add_Tail(*commands);
	chanlistTitle.Add_Tail(*commands);
	userlistTitle.Add_Tail(*commands);
	sendedit.Add_Tail(*commands);

	//	Tooltips...
	DWORD				timeToolTipAppear;
	ToolTipClass*		pToolTipHead = NULL;		//	Head of list of ToolTips that parallels gadget list.
	ToolTipClass*		pToolTipHitLast = NULL;		//	ToolTip the mouse was last over, or null.

	ToolTipClass* pToolTip = pToolTipHead = pWO->pTTipDiscon;
	pToolTip->next = pWO->pTTipLeave;
	pToolTip = pToolTip->next;
	pToolTip->next = pWO->pTTipRefresh;
	pToolTip = pToolTip->next;
	pToolTip->next = pWO->pTTipSquelch;
	pToolTip = pToolTip->next;
	pToolTip->next = pWO->pTTipBan;
	pToolTip = pToolTip->next;
	pToolTip->next = pWO->pTTipKick;
	pToolTip = pToolTip->next;
	pToolTip->next = pWO->pTTipFindpage;
	pToolTip = pToolTip->next;
	pToolTip->next = pWO->pTTipOptions;
	pToolTip = pToolTip->next;
	pToolTip->next = pWO->pTTipLadder;
	pToolTip = pToolTip->next;
	pToolTip->next = pWO->pTTipHelp;
	pToolTip = pToolTip->next;
	ToolTipClass TTipChanExpand( &ExpandChanBtn, TXT_WOL_TTIP_EXPANDLIST, ExpandChanBtn.X + 8, ExpandChanBtn.Y - 9, true );
	pToolTip->next = &TTipChanExpand;
	pToolTip = pToolTip->next;
	ToolTipClass TTipUserExpand( &ExpandUserBtn, TXT_WOL_TTIP_EXPANDLIST, ExpandUserBtn.X + 8, ExpandUserBtn.Y - 9, true );
	pToolTip->next = &TTipUserExpand;
	pToolTip = pToolTip->next;
	ToolTipClass TTipChanList( &chanlist, 0, chanlist.X + 1, chanlist.Y + 1, true, true );
	pToolTip->next = &TTipChanList;
	pToolTip = pToolTip->next;
	ToolTipClass TTipJoin( &JoinBtn, TXT_WOL_TTIP_JOIN, d_join_x + d_join_w/2, d_join_y - 6 );
	pToolTip->next = &TTipJoin;
	pToolTip = pToolTip->next;
	ToolTipClass TTipBack( &BackBtn, TXT_WOL_TTIP_BACK, d_back_x + d_back_w/2, d_back_y - 6 );
	pToolTip->next = &TTipBack;
	pToolTip = pToolTip->next;
	ToolTipClass TTipCreate( &CreateBtn, TXT_WOL_TTIP_CREATE, d_create_x + d_create_w/2, d_create_y - 6 );
	pToolTip->next = &TTipCreate;
	pToolTip = pToolTip->next;
	ToolTipClass TTipAction( &ActionBtn, TXT_WOL_TTIP_ACTION, d_action_x + d_action_w/2, d_action_y - 6, true );
	pToolTip->next = &TTipAction;
	pToolTip = pToolTip->next;
	ToolTipClass TTipRankRA( &RankRABtn, TXT_WOL_TTIP_RANKRA, RankRABtn.X + 8, RankRABtn.Y - 9, true );
	pToolTip->next = &TTipRankRA;
	pToolTip = pToolTip->next;
	ToolTipClass TTipRankAM( &RankAMBtn, TXT_WOL_TTIP_RANKAM, RankAMBtn.X + 8, RankAMBtn.Y - 9, true );
	pToolTip->next = &TTipRankAM;
	pToolTip = pToolTip->next;
	pToolTip->next = NULL;

	//........................................................................
	// List boxes
	//........................................................................
	int tabs[] = { 150 };			//	tabs for channel list
	chanlist.Set_Tabs( tabs );

//	Fancy_Text_Print("", 0, 0, scheme, TBLACK, TPF_TEXT);

	lastclick_timer = 0;

	//	Tell WolapiObject about lists to use for output.
	//	(Sure wish I'd gone against the grain and made this dialog a class...)
	pWO->LinkToChatDlg( &chatlist, &chanlist, &userlist, &userlistTitle );

	if( !pWO->bChatShownBefore )
	{
		//	Print message of the day.
		chatlist.Add_Item( pWO->pChatSink->szMotd, NULL, NULL, ICON_SHAPE );
	}
	else
	{
		//	We have returned to the chat dialog after being in either game setup or an actual game.
		pWO->RestoreChat();
		pWO->DeleteSavedChat();

		if( pWO->bReturningAfterGame )
			pWO->RejoinLobbyAfterGame();
		else
		{
			if( pWO->pChatSink->bGotKickedTrigger )
			{
				//	We got kicked out of a game setup.
				WOL_PrintMessage( chatlist, TXT_WOL_YOUWEREKICKEDFROMGAME, WOLCOLORREMAP_KICKORBAN );
				pWO->pChatSink->bGotKickedTrigger = false;
			}
		}

		if( pWO->iLobbyReturnAfterGame != -1 )
		{
			char szChannelToJoin[ WOL_CHANNAME_LEN_MAX ];
			//sprintf( szChannelToJoin, "Lob_%i_%i", GAME_TYPE, pWO->iLobbyReturnAfterGame );
			sprintf( szChannelToJoin, "%s%i", LOB_PREFIX, pWO->iLobbyReturnAfterGame );
			pWO->OnEnteringChatChannel( szChannelToJoin, false, iChannelLobbyNumber( (unsigned char*)szChannelToJoin ) );
		}
		else
			//	Will never happen presumably, if games are always entered via a lobby chat channel.
			pWO->EnterLevel_Top();

		pWO->iLobbyReturnAfterGame = -1;

		if( pWO->bReturningAfterGame )
		{
			Sound_Effect( WOLSOUND_LOGIN );
			pWO->bReturningAfterGame = false;
		}
		else
			Sound_Effect( WOLSOUND_EXITGAME );
	}

	//	Cause a refresh of szMyRecord, the string showing my win/loss record.
	pWO->RequestLadders( pWO->szMyName );

	//------------------------------------------------------------------------
	//	Init Mono Output
	//------------------------------------------------------------------------
	#if(SHOW_MONO)
 	Ipx.Configure_Debug(-1, sizeof (GlobalHeaderType), sizeof(NetCommandType), GlobalPacketNames, 0, 13);
	Ipx.Mono_Debug_Print(-1,1);
	#endif

	//------------------------------------------------------------------------
	//	Processing loop
	//------------------------------------------------------------------------
	while (process) 
	{
		#if(SHOW_MONO)
		Ipx.Mono_Debug_Print(-1,0);
		#endif

		//	Regularly check for incoming messages from wolapi.
		if( ::timeGetTime() > pWO->dwTimeNextWolapiPump )
		{
/*
			if( pToolTipHitLast && pToolTipHitLast->bShowing )	//	Lame hack. Problem is draws that occur in callbacks.
			{
				pToolTipHitLast->Unshow();
				pWO->pChat->PumpMessages();
				pWO->pNetUtil->PumpMessages();
				pToolTipHitLast->Show();
			}
			else
			{
				pWO->pChat->PumpMessages();
				pWO->pNetUtil->PumpMessages();
			}
*/
			pWO->pChat->PumpMessages();
			pWO->pNetUtil->PumpMessages();

			//	Special post-callback processing...
			if( pWO->bSelfDestruct )
			{
				if( pWO->pChatSink->bConnected )
					pWO->Logout();
				rc = -1;		//	As if the user logged himself out.
				process = false;
				break;
			}
			if( pWO->pChatSink->bGotKickedTrigger )
			{
				if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL )
				{
					pWO->OnExitingChatChannel();
					pWO->EnterLevel_UserChat();
				}
				else if( pWO->CurrentLevel == WOL_LEVEL_INLOBBY )
				{
					pWO->OnExitingChatChannel();
					pWO->EnterLevel_Lobbies();
				}
				else		//	Must be WOL_LEVEL_INOFFICIALCHATCHANNEL.
				{
					pWO->OnExitingChatChannel();
					pWO->EnterLevel_OfficialChat();
				}
				pWO->pChatSink->bGotKickedTrigger = false;
				display = REDRAW_ALL;
			}
			if( pWO->bMyRecordUpdated )
			{
				if( pWO->bShowRankRA )
					chatlistTitle.Set_Text( pWO->szMyRecord );
				else
					chatlistTitle.Set_Text( pWO->szMyRecordAM );
				pWO->bMyRecordUpdated = false;
			}
			if( pWO->bChannelListTitleUpdated )
			{
				chanlistTitle.Set_Text( pWO->szChannelListTitle );
				pWO->bChannelListTitleUpdated = false;
			}
			pWO->dwTimeNextWolapiPump = ::timeGetTime() + WOLAPIPUMPWAIT;
		}

		//	Synch rank toggle buttons state.
		if( BackBtn.Get_Prev() == &RankAMBtn )
		{
			if( pWO->CurrentLevel != WOL_LEVEL_INLOBBY )
			{
				//	Rank buttons are there and shouldn't be.
				RankRABtn.Remove();
				RankAMBtn.Remove();
				display = REDRAW_ALL;
			}
			else
			{
				if( pWO->bShowRankUpdated )
				{
					if( pWO->bShowRankRA )
					{
						RankRABtn.DRAWTOGDOWN;
						RankAMBtn.DRAWTOGUP;
					}
					else
					{
						RankRABtn.DRAWTOGUP;
						RankAMBtn.DRAWTOGDOWN;
					}
					//	Buttons have been refreshed.
					pWO->bShowRankUpdated = false;
					//	Cause my own record to get refreshed.
					pWO->bMyRecordUpdated = true;
					//	Refresh list to show different rankings type.
					pWO->ListChannelUsers();
				}
			}
		}
		else
		{
			if( pWO->CurrentLevel == WOL_LEVEL_INLOBBY )
			{
				//	Rank buttons aren't there and should be.
				RankRABtn.Add( JoinBtn );
				RankAMBtn.Add( RankRABtn );
				RankRABtn.Flag_To_Redraw();
				RankAMBtn.Flag_To_Redraw();
				if( pWO->bShowRankRA )
				{
					RankRABtn.DRAWTOGDOWN;
					RankAMBtn.DRAWTOGUP;
				}
				else
				{
					RankRABtn.DRAWTOGUP;
					RankAMBtn.DRAWTOGDOWN;
				}
				//	Buttons have been refreshed.
				pWO->bShowRankUpdated = false;
			}
		}

		//	Regularly update the channels list in certain cases.
		if( ( pWO->CurrentLevel == WOL_LEVEL_OFFICIALCHAT || pWO->CurrentLevel == WOL_LEVEL_USERCHAT || 
				pWO->CurrentLevel == WOL_LEVEL_LOBBIES || pWO->CurrentLevel == WOL_LEVEL_INLOBBY ) && 
				::timeGetTime() > pWO->dwTimeNextChannelUpdate )
		{
			switch( pWO->CurrentLevel )
			{
			case WOL_LEVEL_OFFICIALCHAT:
				pWO->UpdateChannels( 0, CHANNELFILTER_OFFICIAL, false );
				break;
			case WOL_LEVEL_USERCHAT:
				pWO->UpdateChannels( 0, CHANNELFILTER_UNOFFICIAL, false );
				break;
			case WOL_LEVEL_LOBBIES:		//	Overkill in this case to update so often...
				pWO->UpdateChannels( 0, CHANNELFILTER_LOBBIES, false );
				break;
			case WOL_LEVEL_INLOBBY:
				pWO->UpdateChannels( GAME_TYPE, CHANNELFILTER_LOCALLOBBYGAMES, true );
				break;
			}
			pWO->dwTimeNextChannelUpdate = ::timeGetTime() + CHANNELUPDATEWAIT;
		}

#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}
#endif

		if( bFirsttime && !pWO->bChatShownBefore )
		{
			WWMessageBox().Process( TXT_WOL_FINDINGLOBBY, TXT_NONE );
			char szLobbyName[ WOL_CHANNAME_LEN_MAX ];
			if( pWO->GetNameOfBeginningLobby( szLobbyName ) )
			{
//				debugprint( "Found lobby to go into: '%s'\n", szLobbyName );
				if( !EnterChannel( pWO, chatlist, NULL, szLobbyName, false ) )
				{
					//	Could not enter channel for some reason. Go to top instead.
					pWO->EnterLevel_Top();
				}
			}
			else
			{
				//	Could not find name of a lobby for some reason. Go to top instead.
				pWO->EnterLevel_Top();
			}
			pWO->bChatShownBefore = true;
			display = REDRAW_ALL;
			//	Play login sound.
			Sound_Effect( WOLSOUND_LOGIN );
		}
		bFirsttime = false;

		//.....................................................................
		//	Refresh display if needed
		//.....................................................................
		if (display) 
		{
			Hide_Mouse();

			//..................................................................
			//	Redraw backgound & dialog box
			//..................................................................
			if (display >= REDRAW_BACKGROUND) 
			{
				if( pToolTipHitLast && pToolTipHitLast->bShowing )
				{
					pToolTipHitLast->Unshow();
				}


				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				//...............................................................
				//	Dialog & Field labels
				//...............................................................

				commands->Draw_All();

				//	Draw title bar above channel list.
				Draw_Box( d_chanlist_x, d_chanlist_y - 15, d_chanlist_w, 16, BOXSTYLE_BOX, false );
				switch( lesCurrent )
				{
				case LES_CHANNELS_EXPANDED:
					//	Draw users title bar at bottom.
					Draw_Box( d_userlist_x, d_userlist_y + d_userlist_h - 16, d_userlist_w, 16, BOXSTYLE_BOX, false );
					break;
				case LES_USERS_EXPANDED:
					//	Draw users title bar at top.
					Draw_Box( d_chanlist_x, d_chanlist_y, d_chanlist_w, 16, BOXSTYLE_BOX, false );
					break;
				default:
					//	Draw users title bar in middle.
					Draw_Box( d_userlist_x, d_userlist_y - 15, d_userlist_w, 16, BOXSTYLE_BOX, false );
					break;
				}
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		//	Force mouse visible, as some beta testers report unexplicable disappearing cursors.
		while( Get_Mouse_State() )
			Show_Mouse();
		//	Be nice to other apps.
		Sleep( 50 );

		//.....................................................................
		//	Get user input
		//.....................................................................
		if( ( ::GetAsyncKeyState( KN_LMOUSE ) & 0x8000 ) || ( ::GetAsyncKeyState( KN_RMOUSE ) & 0x8000 ) )
		{
			//	Mouse button is down.
			timeToolTipAppear = ::timeGetTime() + TOOLTIPDELAY;
			if( pToolTipHitLast && pToolTipHitLast->bShowing )
			{
				pToolTipHitLast->Unshow();
			}
		}

		//	If anything currently on the controls list is set to redraw, hide tooltip.
		if( pToolTipHitLast && pToolTipHitLast->bShowing && commands->Is_List_To_Redraw() )
		{
			pToolTipHitLast->Unshow();
		}

		input = commands->Input();

		//	This hack, used elsewhere in this form, appears to be the standard dodge around GadgetClass::Input's 
		//	tendency to remove any focus the first time it runs for a 'commands' list.

		//	ajw - Perhaps I could try doing this every cycle regardless - would avoid stupid non-focused editbox key reactions bug.
		if( bHackFocus )
		{
			sendedit.Set_Focus();
			sendedit.Flag_To_Redraw();
			input = commands->Input();
			bHackFocus = false;
		}

		//	Tooltips...
		if( pToolTipHead )
		{
			ToolTipClass* pToolTipHit = pToolTipHead->GetToolTipHit();
			if( pToolTipHit == pToolTipHitLast )
			{
				if( pToolTipHit && bLinkInList( commands, pToolTipHit->pGadget ) )	//	(Gadget must be in controls list.)
				{
					if( !pToolTipHit->bShowing && ::timeGetTime() > timeToolTipAppear && 
						!( ( ::GetAsyncKeyState( KN_LMOUSE ) & 0x8000 ) || ( ::GetAsyncKeyState( KN_RMOUSE ) & 0x8000 ) ) )
					{
						pToolTipHit->Show();
					}
					else if( pToolTipHit->bIconList && pToolTipHit->bOverDifferentLine() )
					{
						pToolTipHit->Unshow();
						pToolTipHit->Show();
					}
				}
			}
			else
			{
				if( pToolTipHitLast && pToolTipHitLast->bShowing )
					pToolTipHitLast->Unshow();
				pToolTipHitLast = pToolTipHit;
				timeToolTipAppear = ::timeGetTime() + TOOLTIPDELAY;
			}
		}

		//.....................................................................
		//	Process input
		//.....................................................................
		switch (input) 
		{
			case ( BUTTON_SENDEDIT | KN_BUTTON ):
				//	Enter has been pressed - was caught by sendedit control.
				if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL || pWO->CurrentLevel == WOL_LEVEL_INLOBBY )
				{
					pWO->SendMessage( sendedit.Get_Text(), userlist, false );
					//	Clear sendedit, reset focus.
					szSendBuffer[0] = 0;
					sendedit.Set_Focus();
					//	Mark for redraw.
					sendedit.Flag_To_Redraw();
				}
				else
				{
					WOL_PrintMessage( chatlist, TXT_WOL_YOURENOTINCHANNEL, WOLCOLORREMAP_LOCALMACHINEMESS );
					Sound_Effect( WOLSOUND_ERROR );
					sendedit.Set_Focus();
					//	Mark for redraw.
					sendedit.Flag_To_Redraw();
				}
				break;

			case KN_LMOUSE:
				break;

			case ( BUTTON_EXPANDCHANNELS | KN_BUTTON ):
				if( OnExpandChannelList( chanlist, userlist ) )
				{
					//	Hide userlist.
					if( ExpandUserBtn.Get_Next() == &userlist )
						userlist.Remove();
					//	Ensure chanlist is visible.
					if( ExpandChanBtn.Get_Next() != &chanlist )
						chanlist.Add( ExpandChanBtn );
					//	Move userlist expand button.
					ExpandUserBtn.Set_Position( ExpandUserBtn.X, chanlist.Y + chanlist.Height );
					TTipUserExpand.Move( ExpandUserBtn.X + 8, ExpandUserBtn.Y - 16 );
					userlistTitle.Set_Position( userlistTitle.X, chanlist.Y + chanlist.Height + 2 );
					//	Set buttons.
					ExpandChanBtn.Set_Shape( pShpUnexpand );
					ExpandUserBtn.Set_Shape( pShpExpand );
				}
				else
				{
					//	Show userlist.
					if( ExpandUserBtn.Get_Next() != &userlist )
						userlist.Add( ExpandUserBtn );
					//	Move userlist expand button.
					ExpandUserBtn.Set_Position( ExpandUserBtn.X, userlist.Y - 14 );
					TTipUserExpand.Move( ExpandUserBtn.X + 8, ExpandUserBtn.Y - 16 );
					userlistTitle.Set_Position( userlistTitle.X, userlist.Y - 16 + 4 );
					//	Set buttons.
					ExpandChanBtn.Set_Shape( pShpExpand );
					ExpandUserBtn.Set_Shape( pShpExpand );
				}
				//	Move rank buttons.
				RankRABtn.Set_Position( RankRABtn.X, ExpandUserBtn.Y );
				RankAMBtn.Set_Position( RankAMBtn.X, ExpandUserBtn.Y );
				TTipRankRA.Move( RankRABtn.X + 8, RankRABtn.Y - 16 );
				TTipRankAM.Move( RankAMBtn.X + 8, RankAMBtn.Y - 16 );
				display = REDRAW_ALL;
				break;

			case ( BUTTON_EXPANDUSERS | KN_BUTTON ):
				if( OnExpandUserList( chanlist, userlist ) )
				{
					//	Hide chanlist controls.
					if( ExpandChanBtn.Get_Next() == &chanlist )
						chanlist.Remove();
					//	Ensure userlist is visible.
					if( ExpandUserBtn.Get_Next() != &userlist )
						userlist.Add( ExpandUserBtn );
					//	Set buttons.
					ExpandChanBtn.Set_Shape( pShpExpand );
					ExpandUserBtn.Set_Shape( pShpUnexpand );
				}
				else
				{
					//	Show chanlist.
					if( ExpandChanBtn.Get_Next() != &chanlist )
						chanlist.Add( ExpandChanBtn );
					//	Set buttons.
					ExpandChanBtn.Set_Shape( pShpExpand );
					ExpandUserBtn.Set_Shape( pShpExpand );
				}
				//	Move userlist expand button.
				ExpandUserBtn.Set_Position( ExpandUserBtn.X, userlist.Y - 14 );
				TTipUserExpand.Move( ExpandUserBtn.X + 8, ExpandUserBtn.Y - 16 );
				userlistTitle.Set_Position( userlistTitle.X, userlist.Y - 16 + 4 );
				//	Move rank buttons.
				RankRABtn.Set_Position( RankRABtn.X, ExpandUserBtn.Y );
				RankAMBtn.Set_Position( RankAMBtn.X, ExpandUserBtn.Y );
				TTipRankRA.Move( RankRABtn.X + 8, RankRABtn.Y - 16 );
				TTipRankAM.Move( RankAMBtn.X + 8, RankAMBtn.Y - 16 );
				display = REDRAW_ALL;
				break;

			case ( BUTTON_CHANLIST | KN_BUTTON ):
				//	User clicks on the game list.
				//...............................................................
				// Handle a double-click
				//...............................................................
				if( lastclick_timer < 30 && chanlist.Current_Index() == lastclick_idx )
				{
					//	Doubleclick on channel list.
					if( ProcessChannelListSelection( pWO, chatlist, chanlist, lastclick_idx ) )
					{
						//	Exit the chat dialog, go to game dialog.
						rc = 2;
						process = false;
					}
					display = REDRAW_ALL;
					bHackFocus = true;
				}
				else
				{
					//...............................................................
					// Handle a single-click
					//...............................................................
					//............................................................
					// If no double-click occurred, set the last-clicked index
					// & double-click timer.
					//............................................................
					lastclick_timer = 0;
					lastclick_idx = chanlist.Current_Index();

				}
				break;

			case ( BUTTON_JOIN | KN_BUTTON ):
				//	Pressing the join button is exactly like doubleclicking on the selected index in chanlist, except:
				//		if the first item is selected, ignore, unless we are at the top level
				if( pWO->CurrentLevel == WOL_LEVEL_TOP || chanlist.Current_Index() != 0 )
				{
					if( ProcessChannelListSelection( pWO, chatlist, chanlist, chanlist.Current_Index() ) )
					{
						//	Exit the chat dialog, go to game dialog.
						rc = 2;
						process = false;
					}
					display = REDRAW_ALL;
					bHackFocus = true;
				}
				break;

			case ( BUTTON_USERLIST | KN_BUTTON ):
				//	User clicks on user list.
				break;
				
			case ( BUTTON_CREATE | KN_BUTTON ):
				switch( pWO->CurrentLevel )
				{
				case WOL_LEVEL_INCHATCHANNEL:
//					debugprint( "%s\n", TXT_WOL_CANTCREATEINCHANNEL );
					WOL_PrintMessage( chatlist, TXT_WOL_CANTCREATEINCHANNEL, WOLCOLORREMAP_LOCALMACHINEMESS );
					Sound_Effect( WOLSOUND_ERROR );
					break;
				case WOL_LEVEL_INLOBBY:
				{
					pWO->bPump_In_Call_Back = true;
					CREATEGAMEINFO CreateGameInfo = WOL_CreateGame_Dialog( pWO );
					pWO->bPump_In_Call_Back = false;
					if( CreateGameInfo.bCreateGame )
					{
						if( CreateGameChannel( pWO, CreateGameInfo ) )
						{
							rc = 1;
							process = false;
						}
					}
					break;
				}
				case WOL_LEVEL_GAMES:
				case WOL_LEVEL_GAMESOFTYPE:
				case WOL_LEVEL_LOBBIES:
					WOL_PrintMessage( chatlist, TXT_WOL_CANTCREATEHERE, WOLCOLORREMAP_LOCALMACHINEMESS );
					Sound_Effect( WOLSOUND_ERROR );
					break;
				default:
					CreateChatChannel( pWO );
				}

				display = REDRAW_ALL;
				bHackFocus = true;
				break;

			case ( BUTTON_LEAVE | KN_BUTTON ):
				//	Because of the way things are set up, this is exactly like selecting the first item in chanlist.
				//	(Button is disabled when this is not appropriate.)
				ProcessChannelListSelection( pWO, chatlist, chanlist, 0 );
				display = REDRAW_ALL;
				break;

			case ( BUTTON_REFRESH | KN_BUTTON ):
				pWO->dwTimeNextChannelUpdate = ::timeGetTime();
				break;

			case ( BUTTON_SQUELCH | KN_BUTTON ):
				pWO->DoSquelch( &userlist );
				break;

			case ( BUTTON_BAN | KN_BUTTON ):
				pWO->DoKick( &userlist, true );
//				display = REDRAW_ALL;
				break;

			case ( BUTTON_KICK | KN_BUTTON ):
				pWO->DoKick( &userlist, false );
//				display = REDRAW_ALL;
				break;

			case ( BUTTON_FINDPAGE | KN_BUTTON ):
				pWO->DoFindPage();
				display = REDRAW_ALL;
				bHackFocus = true;
				break;

			case ( BUTTON_OPTIONS | KN_BUTTON ):
				pWO->DoOptions();
				display = REDRAW_ALL;
				bHackFocus = true;
				break;

			case ( KN_ESC ):
//				break;			ajw		Put back in?

			case ( BUTTON_BACK | KN_BUTTON ):
				//	Pressing the back button is exactly like doubleclicking on the top item in chanlist, except
				//	when we're at the top level.
				if( pWO->CurrentLevel != WOL_LEVEL_TOP )
				{
					ProcessChannelListSelection( pWO, chatlist, chanlist, 0 );
					display = REDRAW_ALL;
					break;
				}
				//	Note no break; here. Fall through if at top level.
			case ( BUTTON_DISCONNECT | KN_BUTTON ):
				if( WWMessageBox().Process( TXT_WOL_CONFIRMLOGOUT, TXT_YES, TXT_NO ) == 0 )
				{
					if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL || pWO->CurrentLevel == WOL_LEVEL_INLOBBY )
						ExitChatChannel( pWO );
					pWO->Logout();
					rc = -1;
					process = false;
				}
				display = REDRAW_ALL;
				bHackFocus = true;
				break;

			case ( BUTTON_ACTION | KN_BUTTON ):
				if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL || pWO->CurrentLevel == WOL_LEVEL_INLOBBY )
				{
					pWO->SendMessage( sendedit.Get_Text(), userlist, true );
					//	Clear sendedit, reset focus.
					szSendBuffer[0] = 0;
					sendedit.Set_Focus();
					//	Mark for redraw.
					//chatlist.Flag_To_Redraw();
					sendedit.Flag_To_Redraw();
				}
				else
				{
					WOL_PrintMessage( chatlist, TXT_WOL_YOURENOTINCHANNEL, WOLCOLORREMAP_LOCALMACHINEMESS );
					Sound_Effect( WOLSOUND_ERROR );
					sendedit.Set_Focus();
					//	Mark for redraw.
					sendedit.Flag_To_Redraw();
				}
				break;

			case ( BUTTON_LADDER | KN_BUTTON ):
				pWO->DoLadder();
				display = REDRAW_ALL;
				bHackFocus = true;
				break;

			case ( BUTTON_HELP | KN_BUTTON ):
				pWO->DoHelp();
				display = REDRAW_ALL;
				bHackFocus = true;
				break;

			case ( BUTTON_RANKRA | KN_BUTTON ):
				pWO->bShowRankRA = true;
				pWO->bShowRankUpdated = true;
				break;
			
			case ( BUTTON_RANKAM | KN_BUTTON ):
				pWO->bShowRankRA = false;
				pWO->bShowRankUpdated = true;
				break;

			default:

				break;
		}

		//.....................................................................
		//	Service the sounds & score; GameActive must be false at this point,
		//	so Call_Back() doesn't intercept global messages from me!
		//.....................................................................
		Call_Back();

	}	// end of while

	if( pToolTipHitLast && pToolTipHitLast->bShowing )
		pToolTipHitLast->Unshow();

/*
	//------------------------------------------------------------------------
	//	Restore screen
	//------------------------------------------------------------------------
	Hide_Mouse();
	Load_Title_Page(true);
	Show_Mouse();
*/

	pWO->SaveChat();

	pWO->ClearListPtrs();

	return rc;
}

//***********************************************************************************************
void WOL_PrintMessage( IconListClass& ILTarget, const char* szText, PlayerColorType iColorRemap /* = PCOLOR_NONE */ )
{
	RemapControlType* pColorRemap = ( iColorRemap == PCOLOR_NONE ? NULL : &ColorRemaps[ iColorRemap ] );
	WOL_PrintMessage( ILTarget, szText, pColorRemap );
}

//***********************************************************************************************
void WOL_PrintMessage( IconListClass& ILTarget, const char* szText, RemapControlType* pColorRemap )
{
	ILTarget.Add_Item( szText, NULL, NULL, ICON_SHAPE, NULL, NULL, pColorRemap );
	if( !ILTarget.bScrollBeingDragged() )
		ILTarget.Show_Last_Item();
}

//***********************************************************************************************
bool OnExpandChannelList( IconListClass& chanlist, IconListClass& userlist )
{
	//	Expand channel list button was pressed.
	//	Returns true if userlist controls are to be hidden, false if they are to be shown.
	switch( lesCurrent )
	{
	case LES_NORMAL:
		ResizeChannelList( chanlist, true );
		lesCurrent = LES_CHANNELS_EXPANDED;
		break;
	case LES_USERS_EXPANDED:
		ResizeUserList( userlist, false );
		ResizeChannelList( chanlist, true );
		lesCurrent = LES_CHANNELS_EXPANDED;
		break;
	case LES_CHANNELS_EXPANDED:
		ResizeChannelList( chanlist, false );
		lesCurrent = LES_NORMAL;
		return false;
	}
	return true;
}

//***********************************************************************************************
bool OnExpandUserList( IconListClass& chanlist, IconListClass& userlist )
{
	//	Expand user list button was pressed.
	//	Returns true if chanlist controls are to be hidden, false if they are to be shown.
	switch( lesCurrent )
	{
	case LES_NORMAL:
		ResizeUserList( userlist, true );
		lesCurrent = LES_USERS_EXPANDED;
		break;
	case LES_CHANNELS_EXPANDED:
		ResizeChannelList( chanlist, false );
		ResizeUserList( userlist, true );
		lesCurrent = LES_USERS_EXPANDED;
		break;
	case LES_USERS_EXPANDED:
		ResizeUserList( userlist, false );
		lesCurrent = LES_NORMAL;
		return false;
	}
	return true;
}

//***********************************************************************************************
void ResizeChannelList( IconListClass& chanlist, bool bExpand )
{
	//	If bExpand, makes list big, else normal size.
	if( bExpand )
		chanlist.Resize( d_chanlist_x, d_chanlist_y, d_chanlist_w, d_userlist_y + d_userlist_h - 15 - d_chanlist_y );
	else
		chanlist.Resize( d_chanlist_x, d_chanlist_y, d_chanlist_w, d_chanlist_h );
}

//***********************************************************************************************
void ResizeUserList( IconListClass& userlist, bool bExpand )
{
	//	If bExpand, makes list big, else normal size.
	if( bExpand )
		userlist.Resize( d_userlist_x, d_chanlist_y + 15, d_userlist_w, d_userlist_y + d_userlist_h - ( d_chanlist_y + 15 ) );
	else
		userlist.Resize( d_userlist_x, d_userlist_y, d_userlist_w, d_userlist_h );
}

//***********************************************************************************************
bool EnterChannel( WolapiObject* pWO, IconListClass& chatlist, IconListClass& chanlist, int iIndex, bool bGame )
{
	//	Enter the channel specified in chanlist at iIndex.
	//	Called to enter chat channels, "lobbies", and game channels.

	//	We've stored the channel pointer in the hidden extra data field.
	//	( Be careful about calling RAChatEventSink::DeleteChannelList()! )
	Channel* pChannel = (Channel*)chanlist.Get_Item_ExtraDataPtr( iIndex );
	return EnterChannel( pWO, chatlist, pChannel, NULL, bGame );
}

//***********************************************************************************************
bool EnterChannel( WolapiObject* pWO, IconListClass& chatlist, Channel* pChannel, char* szChannelName, bool bGame )
{
	//	Called to cause the user to enter a channel (chat, lobby, or game).
	//	If pChannel is NULL, szChannelName will be used.

	Channel ChannelWhenNameOnly;
	if( !pChannel )
	{
		if( !szChannelName )
		{
//			debugprint( "pChannel and szChannelName null in EnterChannel" );
			pWO->bSelfDestruct = true;
			return false;
		}
		pChannel = &ChannelWhenNameOnly;
		strcpy( (char*)pChannel->name, szChannelName );
	}

	if( bGame && pChannel->currentUsers >= pChannel->maxUsers )			//	Pre-emptive fullness check.
	{
		WOL_PrintMessage( chatlist, TXT_WOL_CHANNELFULL, WOLCOLORREMAP_LOCALMACHINEMESS );
		Sound_Effect( WOLSOUND_ERROR );
		return false;
	}

	if( bGame )
	{
		//	It is possible to enter a game channel while currently in a chat channel. (A lobby, presumably.)
		if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL || pWO->CurrentLevel == WOL_LEVEL_INLOBBY )
			if( !pWO->ExitChatChannelForGameChannel() )
			{
				*pWO->szChannelReturnOnGameEnterFail = 0;
//				debugprint( "ExitChatChannelForGameChannel on join failed" );
				pWO->bSelfDestruct = true;
				return false;
			}
	}

/*	The following doesn't work because the needpw field is not currently being set in wolapi for chat channels.
	Instead, we wait for a fail on join, then present this dialog and try again.
	if( pChannel->needpw )
	{
		SimpleEditDlgClass* pEditDlg = new SimpleEditDlgClass( 300, TXT_WOL_JOINPRIVATETITLE, TXT_WOL_JOINPRIVATEPROMPT,
											WOL_CHANKEY_LEN_MAX );
		if( !pEditDlg->Show() || !*pEditDlg->szEdit )
			return false;

		strcpy( (char*)pChannel->key, pEditDlg->szEdit );
	}
*/
	bool bKeepTrying = true;

	//	Set password automatically for our lobbies, if trying to join one.
	int iLobby = iChannelLobbyNumber( pChannel->name );
	if( iLobby != -1 )
		strcpy( (char*)pChannel->key, LOBBYPASSWORD );

	char szSuccessfulPassword[ WOL_PASSWORD_LEN + 5 ];
	*szSuccessfulPassword = 0;

	HRESULT hRes;
	while( bKeepTrying )
	{
		hRes = pWO->ChannelJoin( pChannel );
		switch( hRes )
		{
		case CHAT_E_BADCHANNELPASSWORD:
		{
			Fancy_Text_Print( TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_TEXT );	//	Required before String_Pixel_Width() call, for god's sake.
#ifdef ENGLISH
			SimpleEditDlgClass* pEditDlg = new SimpleEditDlgClass( 300, TXT_WOL_JOINPRIVATETITLE, TXT_WOL_JOINPRIVATEPROMPT,
												WOL_CHANKEY_LEN_MAX );
#else
#ifdef GERMAN
			SimpleEditDlgClass* pEditDlg = new SimpleEditDlgClass( 400, TXT_WOL_JOINPRIVATETITLE, TXT_WOL_JOINPRIVATEPROMPT,
												WOL_CHANKEY_LEN_MAX );
#else
			SimpleEditDlgClass* pEditDlg = new SimpleEditDlgClass( 500, TXT_WOL_JOINPRIVATETITLE, TXT_WOL_JOINPRIVATEPROMPT,
												WOL_CHANKEY_LEN_MAX );
#endif
#endif
			pWO->bPump_In_Call_Back = true;
			if( strcmp( pEditDlg->Show(), Text_String( TXT_OK ) ) != 0 || !*pEditDlg->szEdit )
			{
				pWO->bPump_In_Call_Back = false;
				delete pEditDlg;
				bKeepTrying = false;
				break;
			}
			pWO->bPump_In_Call_Back = false;
			strcpy( (char*)pChannel->key, pEditDlg->szEdit );
			strcpy( szSuccessfulPassword, pEditDlg->szEdit );
			delete pEditDlg;
			break;
		}
		case CHAT_E_TIMEOUT:
			pWO->bPump_In_Call_Back = true;
			WWMessageBox().Process( TXT_WOL_TIMEOUT );
			pWO->bPump_In_Call_Back = false;
			bKeepTrying = false;
			break;
		case CHAT_E_CHANNELDOESNOTEXIST:
			pWO->bPump_In_Call_Back = true;
			WWMessageBox().Process( TXT_WOL_CHANNELGONE );
			pWO->bPump_In_Call_Back = false;
			bKeepTrying = false;
			break;
		case CHAT_E_BANNED:
			WOL_PrintMessage( chatlist, TXT_WOL_YOUREBANNED, WOLCOLORREMAP_LOCALMACHINEMESS );
			Sound_Effect( WOLSOUND_ERROR );
			bKeepTrying = false;
			break;
		case CHAT_E_CHANNELFULL:
			WOL_PrintMessage( chatlist, TXT_WOL_CHANNELFULL, WOLCOLORREMAP_LOCALMACHINEMESS );
			Sound_Effect( WOLSOUND_ERROR );
			bKeepTrying = false;
			break;
		case E_FAIL:
			pWO->GenericErrorMessage();
			bKeepTrying = false;
			break;
		case S_OK:
			bKeepTrying = false;
			break;
		}
	}

	if( !bGame )
	{
		if( hRes == S_OK )
			return pWO->OnEnteringChatChannel( (char*)pChannel->name, false, iLobby );
		else
			return false;
	}
	else
	{
		if( hRes == S_OK )
		{
			*pWO->szChannelReturnOnGameEnterFail = 0;
			//	Return later to the lobby of the channel creator - which was saved in the channel itself.
			pWO->iLobbyReturnAfterGame = pChannel->reserved & 0x00FFFFFF;
			if( pWO->iLobbyReturnAfterGame == 0x00FFFFFF )
				pWO->iLobbyReturnAfterGame = -1;
			CREATEGAMEINFO CreateGameInfo;
			//	Not all of these values are currently used during setup.
			CreateGameInfo.bCreateGame = false;
			CreateGameInfo.iPlayerMax = pChannel->maxUsers;
			CreateGameInfo.bTournament = pChannel->tournament;
			if( *szSuccessfulPassword )
			{
				CreateGameInfo.bPrivate = true;
				strcpy( CreateGameInfo.szPassword, szSuccessfulPassword );
			}
			else
			{
				CreateGameInfo.bPrivate = false;
				*CreateGameInfo.szPassword = 0;
			}
			CreateGameInfo.GameKind = (CREATEGAMEINFO::GAMEKIND)( pChannel->reserved & 0xFF000000 );
			return pWO->OnEnteringGameChannel( (char*)pChannel->name, false, CreateGameInfo );
		}
		else
		{
			pWO->OnFailedToEnterGameChannel();
			*pWO->szChannelReturnOnGameEnterFail = 0;
			return false;
		}
	}
}

//***********************************************************************************************
bool ExitChatChannel( WolapiObject* pWO )
{
	//	Called to cause user to leave current chat or lobby channel.

//	debugprint( "ExitChatChannel\n" );
	if( !pWO->ChannelLeave() )
	{
		pWO->GenericErrorMessage();
		return false;
	}

	pWO->OnExitingChatChannel();

	return true;
}

//***********************************************************************************************
//void CreateChatChannel( WolapiObject* pWO, bool bPrivate )
void CreateChatChannel( WolapiObject* pWO )
{
	SimpleEditDlgClass* pEditDlg;
/*	if( !bPrivate )
	{
		pEditDlg = new SimpleEditDlgClass( 300, TXT_WOL_CREATECHANNELTITLE, TXT_WOL_CREATECHANNELPROMPT,
											WOL_CHANNAME_LEN_MAX );
		if( strcmp( pEditDlg->Show(), Text_String( TXT_OK ) ) == 0 && *pEditDlg->szEdit )
		{
			if( pWO->ChannelCreate( pEditDlg->szEdit, NULL ) )
				pWO->OnEnteringChatChannel( pEditDlg->szEdit, true, -1 );
		}
	}
	else
*/	{
		Fancy_Text_Print( TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_TEXT );	//	Required before String_Pixel_Width() call, for god's sake.
		pEditDlg = new SimpleEditDlgClass( 350, TXT_WOL_CREATECHANNELTITLE, TXT_WOL_CREATECHANNELPROMPT,
											WOL_CHANNAME_LEN_MAX, TXT_WOL_OPTIONALPASSPROMPT, WOL_CHANKEY_LEN_MAX );
		pWO->bPump_In_Call_Back = true;
		if( strcmp( pEditDlg->Show(), Text_String( TXT_OK ) ) == 0 && *pEditDlg->szEdit )
		{
			pWO->bPump_In_Call_Back = false;
			if( *pEditDlg->szEdit2 )
			{
				if( pWO->ChannelCreate( pEditDlg->szEdit, pEditDlg->szEdit2 ) )
					pWO->OnEnteringChatChannel( pEditDlg->szEdit, true, -1 );
			}
			else
			{
				//	Create public channel.
				if( pWO->ChannelCreate( pEditDlg->szEdit, NULL ) )
					pWO->OnEnteringChatChannel( pEditDlg->szEdit, true, -1 );
			}
		}
		pWO->bPump_In_Call_Back = false;
	}

	delete pEditDlg;
}

//***********************************************************************************************
bool CreateGameChannel( WolapiObject* pWO, const CREATEGAMEINFO& cgi )
{
	char szNewChannelName[ WOL_CHANNAME_LEN_MAX ];
	sprintf( szNewChannelName, "%s's_game", pWO->szMyName );

	if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL || pWO->CurrentLevel == WOL_LEVEL_INLOBBY )
		if( !pWO->ExitChatChannelForGameChannel() )
		{
			*pWO->szChannelReturnOnGameEnterFail = 0;
//			debugprint( "ExitChatChannelForGameChannel in CreateGameChannel() error" );
			pWO->bSelfDestruct = true;
			return false;
		}

	const char* szKey;
	if( *cgi.szPassword )
		szKey = cgi.szPassword;
	else
		szKey = NULL;

	if( pWO->ChannelCreate( szNewChannelName, szKey, true, cgi.iPlayerMax, cgi.bTournament, pWO->iLobbyLast, cgi.GameKind ) )
		pWO->OnEnteringGameChannel( szNewChannelName, true, cgi );
	else
	{
		pWO->OnFailedToEnterGameChannel();
//		debugprint( "CreateGameChannel fail" );
		return false;
	}
	*pWO->szChannelReturnOnGameEnterFail = 0;

	return true;
}

//***********************************************************************************************
bool ProcessChannelListSelection( WolapiObject* pWO, IconListClass& chatlist, IconListClass& chanlist, int iIndex )
{
	//	Takes whatever action necessary due to user selecting iIndex from chanlist.
	//	Returns true if user selected to enter a game channel, else false.
	if( iIndex < 0 )
		return false;

//	debugprint( "iIndex %i\n", iIndex );

	const char* szChannelType = chanlist.Get_Item_ExtraDataString( iIndex );
	if( szChannelType )
	{
//debugprint( "szChannelType %s\n", szChannelType );
		if( strcmp( szChannelType, CHANNELTYPE_OFFICIALCHAT ) == 0 )
		{
			if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL )
			{
				if( !ExitChatChannel( pWO ) )
				{
					pWO->bSelfDestruct = true;
					return false;
				}
			}
			pWO->EnterLevel_OfficialChat();
		}
		else if( strcmp( szChannelType, CHANNELTYPE_USERCHAT ) == 0 )
		{
			if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL )
			{
				if( !ExitChatChannel( pWO ) )
				{
					pWO->bSelfDestruct = true;
					return false;
				}
			}
			pWO->EnterLevel_UserChat();
		}
		else if( strcmp( szChannelType, CHANNELTYPE_TOP ) == 0 )
		{
			if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL )
			{
				//	Now not possible.
				if( !ExitChatChannel( pWO ) )
				{
					pWO->bSelfDestruct = true;
					return false;
				}
			}
			pWO->EnterLevel_Top();
		}
		else if( strcmp( szChannelType, CHANNELTYPE_GAMES ) == 0 )
		{
			if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL )
			{
				//	Now not possible.
				if( !ExitChatChannel( pWO ) )
				{
					pWO->bSelfDestruct = true;
					return false;
				}
			}
			pWO->EnterLevel_Games();
		}
		else if( strcmp( szChannelType, CHANNELTYPE_GAMESOFTYPE ) == 0 )
		{
			if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL )
			{
				//	Now not possible.
				if( !ExitChatChannel( pWO ) )
				{
					pWO->bSelfDestruct = true;
					return false;
				}
			}
			void* pExtraData = chanlist.Get_Item_ExtraDataPtr( iIndex );
			pWO->EnterLevel_GamesOfType( (WOL_GAMETYPEINFO*)pExtraData );
		}
		else if( strcmp( szChannelType, CHANNELTYPE_CHATCHANNEL ) == 0 )
		{
			if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL )
			{
				//	Not currently possible.
//				debugprint( "Trying to jump from channel to channel?!\n" );
				pWO->bSelfDestruct = true;
				return false;
			}
			//	Join the chat channel.
			EnterChannel( pWO, chatlist, chanlist, iIndex, false );		//	Can fail.
		}
		else if( strcmp( szChannelType, CHANNELTYPE_GAMECHANNEL ) == 0 )
		{
			//	User attempting to join game channel.
			if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL )
			{
				//	Not currently possible.
//				debugprint( "Trying to jump from channel to channel?!\n" );
				pWO->bSelfDestruct = true;
				return false;
			}
			//	Check if local user is allowed to join GameKind.
			Channel* pChannel = (Channel*)chanlist.Get_Item_ExtraDataPtr( iIndex );
			if( pChannel->type == GAME_TYPE )
			{
				//	It is a game of our type, at least.
				CREATEGAMEINFO::GAMEKIND GameKind = (CREATEGAMEINFO::GAMEKIND)( pChannel->reserved & 0xFF000000 );
				switch( GameKind )
				{
				case CREATEGAMEINFO::RAGAME:
					break;
				case CREATEGAMEINFO::CSGAME:
					if( !Is_Counterstrike_Installed() )
					{
						WOL_PrintMessage( chatlist, TXT_WOL_NEEDCOUNTERSTRIKE, WOLCOLORREMAP_LOCALMACHINEMESS );
						Sound_Effect( WOLSOUND_ERROR );
						return false;
					}
					break;
				case CREATEGAMEINFO::AMGAME:
					if( !Is_Aftermath_Installed() )
					{
						WOL_PrintMessage( chatlist, TXT_WOL_NEEDAFTERMATH, WOLCOLORREMAP_LOCALMACHINEMESS );
						Sound_Effect( WOLSOUND_ERROR );
						return false;
					}
					break;
				default:
//					debugprint( "Illegal value for GameKind channel reserved field: %s\n", (char*)pChannel->name );
					Sound_Effect( WOLSOUND_ERROR );
					return false;
				}
				//	Join the game channel.
				if( EnterChannel( pWO, chatlist, chanlist, iIndex, true ) )
				{
					//	Exit the chat dialog, go to game dialog.
					return true;
				}
			}
			else
			{
				//	User doubleclicked on a game that is of a different type.
				//	Offer to take them to a web page regarding the game type.
				pWO->DoGameAdvertising( pChannel );
			}
		}
		else if( strcmp( szChannelType, CHANNELTYPE_LOBBIES ) == 0 )
		{
			if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL )
			{
				//	Not currently possible.
//				debugprint( "Chat channel to lobbies level?!\n" );
				pWO->bSelfDestruct = true;
				return false;
			}
			if( pWO->CurrentLevel == WOL_LEVEL_INLOBBY )
			{
				if( !ExitChatChannel( pWO ) )
				{
					pWO->bSelfDestruct = true;
					return false;
				}
			}
			pWO->EnterLevel_Lobbies();
		}
		else if( strcmp( szChannelType, CHANNELTYPE_LOBBYCHANNEL ) == 0 )
		{
			if( pWO->CurrentLevel == WOL_LEVEL_INCHATCHANNEL ||
				pWO->CurrentLevel == WOL_LEVEL_INLOBBY )
			{
				//	Not currently possible.
//				debugprint( "Chat or lobby channel to lobby channel?!\n" );
				pWO->bSelfDestruct = true;
				return false;
			}
			//	Join the lobby chat channel.
			EnterChannel( pWO, chatlist, chanlist, iIndex, false );		//	Can fail.
		}
		else if( strcmp( szChannelType, CHANNELTYPE_LOADING ) == 0 )
		{
			//	User clicked on the channel list loading notification - do nothing.
		}
		else
		{
//			debugprint( "Item selected in channel list unidentifiable from extradata field\n" );
			pWO->bSelfDestruct = true;
			return false;
		}

	}
	return false;
}
	
//***********************************************************************************************
bool bLinkInList( const LinkClass* pListHead, const LinkClass* pLinkToFind )
{
	const LinkClass* pLink = pListHead;
	while( pLink )
	{
		if( pLink == pLinkToFind )
			return true;
		pLink = pLink->Get_Next();
	}
	return false;
}

#endif
