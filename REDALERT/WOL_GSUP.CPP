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


#ifdef WOLAPI_INTEGRATION			//	Now implies also WINSOCK_IPX, WIN32, and FIXIT_CSII must be true

#include "wol_gsup.h"
#include "function.h"
#include "IconList.h"
#include <time.h>
#include "WolStrng.h"
#include "wsproto.h"
#include "BigCheck.h"
#include "ToolTip.h"

extern char const* EngMisStr[];

bool Is_Mission_126x126 (char *file_name);
bool Is_Mission_Aftermath (char *file_name);
bool Is_Mission_Counterstrike (char *file_name);

bool Force_Scenario_Available( const char* szName );

int ScenarioIndex_From_Filename( const char* szScenarioFilename );

bool bSpecialAftermathScenario( const char* szScenarioDescription );

#include "WolDebug.h"

#define PARAMREFRESHWAIT	2000

#define PING_AND_DISPLAY_WAIT		5000

void WOL_PrintMessage( IconListClass& ILTarget, const char* szText, PlayerColorType iColorRemap = PCOLOR_NONE );
void WOL_PrintMessage( IconListClass& ILTarget, const char* szText, RemapControlType* pColorRemap );
void CC_Draw_DIB( const char* pDIB, int xDest, int yDest, int iWidth, WindowNumberType window );

bool operator==( const GAMEPARAMS& gp1, const GAMEPARAMS& gp2 );
bool operator==( const GlobalPacketType& gp1, const GlobalPacketType& gp2 );
PlayerColorType PlayerColorTypeOf( RemapControlType* pColorRemap );

extern unsigned long PlanetWestwoodStartTime;					//Time that game was started

extern bool cancel_current_msgbox;
extern bool disable_current_msgbox;

void Debug_GlobalPacketType( const GlobalPacketType& gp1 );

//***********************************************************************************************
/*	Game startup logic:
When a guest joins the channel, the host sends all current setup info.
When a guest changes house, he tells the host, who informs everyone. A house change can't be denied.
When a guest changes color, he asks the host for the new color, and assumes the request will go through. If it doesn't, the
host sends the guest a messages setting him back to the old color, otherwise, everyone gets the new color for the guest.
When the host changes a game param, the change will be noticed within PARAMREFRESHWAIT milliseconds and then transmitted to
all guests.
All player specific information is stored within the players list (IconListClass), and nowhere else. Though this storage
method got a bit inelegant, it gets the job done.
All "informs" (messages sent from host to guest) except color changes (WOL_GAMEOPT_INFCOLOR) and player "accept" status
changes (WOL_GAMEOPT_INFACCEPT) are assigned a ID, which
increments on each send. Color informs are: a) not a condition for removing a guest's "accept" status, and b) sent out to
individual guests in certain cases, and would thus complicate the ID tracking scheme.
When a guest "accepts" the game setup (with a WOL_GAMEOPT_REQACCEPT), the latest received ID is included in the message.
If the host receives a WOL_GAMEOPT_REQACCEPT with an out-of-date ID, the accept is ignored. Though the guest sets himself to
"accepted" when he sends the WOL_GAMEOPT_REQACCEPT, if it gets ignored by the host it is because a new inform is already on its
way, which will reset the guest's own status to "not accepted" when it arrives.
Host clears all accepteds when params are changed, player joins or leaves, or house change message arrives. Though a
WOL_GAMEOPT_INFACCEPT is sent out when a guest accepts, there is no corresponding "unaccept all" message sent out at this point,
as all guests will naturally clear their recorded accept status for everyone when the change arrives there.
Guest clears own accepted when an inform from the server other than color change arrives, player joins or leaves, or own
house is directly changed.
When all players are "accepted", and the host says "start", a WOL_GAMEOPT_INFSTART is sent to all guests and the host goes into
"waiting to start" mode. If a house change or other arrives in this phase, host clears waiting mode and (naturally) marks guests
unaccepted and sends out the house change to the guests. (The same applies to player joins/leaves.)
While in this mode, the host user is locked out of making any changes to game params.
If a change does come in during this phase, a WOL_GAMEOPT_INFCANCELSTART is sent to the guests, and the waiting mode cancelled.
When a guest receives WOL_GAMEOPT_INFSTART: If he does not see himself as accepted, it is because a change has just been sent
to the host, or some other event occurred that the host will learn about, so the guest can simply ignore the WOL_GAMEOPT_INFSTART.
If the guest does see himself as accepted when WOL_GAMEOPT_INFSTART arrives, he responds with WOL_GAMEOPT_REQSTART and goes into
"waiting to start" mode. When in this mode, the user is locked out of making any changes.
Note that the host is not really asking for any sort of "confirmation that it's ok" to start from the guest here. (Hence no
"cancel" response is possible.) It's really just a way of telling the guests not to make any more changes to the setup; it's
like we're making sure we've "flushed the queue" of game changes.
If a guest receives a WOL_GAMEOPT_INFCANCELSTART, he cancels out of "waiting to start" mode. Note that while in the waiting
mode, normal processing of messages is allowed to take place - the user is simply blocked from doing anything.
Presumably (hopefully) this phase will last less than a second.
When the host receives a WOL_GAMEOPT_REQSTART from everyone, he tells everyone to start with a WOL_GAMEOPT_INFGO.
Because there is a chance of color changes being out of sync, all player colors are sent to all guests at this point.
There is no chance of other data being out of sync.
*/

//--------------------------------------------------------------------------
//	Pieced together from Net_New_Dialog() and Net_Join_Dialog().
//--------------------------------------------------------------------------
//***********************************************************************************************
WOL_GameSetupDialog::WOL_GameSetupDialog( WolapiObject* pWO, bool bHost ) : pWO( pWO ), bHost( bHost ),
		HousePrevious( HOUSE_NONE ),
		pILPlayers( NULL ),
		pILScens( NULL ),
		pILDisc( NULL ),
		pEditSend( NULL ),
		pGaugeCount( NULL ),
		pGaugeLevel( NULL ),
		pGaugeCredits( NULL ),
		pGaugeAIPlayers( NULL ),
		pCheckListOptions( NULL ),
//		pTextBtnOk( NULL ),
		pTextBtnCancel( NULL ),
		pTextBtnAcceptStart( NULL ),
		pTextBtnAction( NULL ),
		pStaticDescrip( NULL ),
		pStaticUnit( NULL ),
		pStaticLevel( NULL ),
		pStaticCredits( NULL ),
		pStaticAIPlayers( NULL ),
		pDropListHouse( NULL ),
		pCheckAftermathUnits( NULL ),
		nHostLastParamID( 0 ),
		nGuestLastParamID( 0 ),
		bWaitingToStart( false ),
		bParamsReceived( false ),
		bHostSayGo( false ),
		bExitForGameTrigger( false ),
		pToolTipHead( NULL ),
		pToolTipHitLast( NULL ),
		pTTipAcceptStart( NULL ),
		pTTipCancel( NULL ),
		pTTipAction( NULL ),
		ScenKindCurrent( SCENARIO_UNINITIALIZED ),
		pShpBtnScenarioRA( NULL ),
		pShpBtnScenarioCS( NULL ),
		pShpBtnScenarioAM( NULL ),
		pShpBtnScenarioUser( NULL ),
		bLeaveDueToRulesMismatchTrigger( false ),
		bHostWaitingForGoTrigger( false )
{
	*szSendBuffer = 0;
	*szHouseBuffer = 0;
	memset( &GParamsLastSent, 0, sizeof( GAMEPARAMS ) );
	*szNameOfHostWhoJustBailedOnUs = 0;
	*szTriggerGameStartInfo = 0;
}

//***********************************************************************************************
WOL_GameSetupDialog::~WOL_GameSetupDialog()
{
	delete pILPlayers;
	delete pILScens;
	delete pILDisc;
	delete pEditSend;
	delete pGaugeCount;
	delete pGaugeLevel;
	delete pGaugeCredits;
	delete pGaugeAIPlayers;
	delete pCheckListOptions;
//	delete pTextBtnOk;
	delete pTextBtnCancel;
	delete pTextBtnAcceptStart;
	delete pTextBtnAction;
//	delete pStaticDescrip;
	delete pStaticUnit;
	delete pStaticLevel;
	delete pStaticCredits;
	delete pStaticAIPlayers;
	delete pDropListHouse;
	delete pCheckAftermathUnits;
	delete pTTipAcceptStart;
	delete pTTipCancel;
	delete pTTipAction;
	delete pShpBtnScenarioRA;
	delete pShpBtnScenarioCS;
	delete pShpBtnScenarioAM;
	delete pShpBtnScenarioUser;
}

//***********************************************************************************************
RESULT_WOLGSUP WOL_GameSetupDialog::Run()
{
	Initialize();
	return Show();
}

//***********************************************************************************************
void WOL_GameSetupDialog::Initialize()
{
	//------------------------------------------------------------------------
	//	Dialog & button dimensions
	//------------------------------------------------------------------------
	d_dialog_w = 640;											// dialog width
	d_dialog_h = 400;											// dialog height
	d_dialog_x = 0;
	d_dialog_y = 0;
	d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	d_txt6_h = 6*RESFACTOR+1;												// ht of 6-pt text
	int d_text_h = 12;
	d_margin1 = 34;															// large margin

	d_color_w = 10 *RESFACTOR;
	d_color_h = 9 *RESFACTOR;
	d_color_x = 294;	//54;		//d_dialog_x + ((d_dialog_w / 4) * 3) - (d_color_w * 3);
	d_color_y = 89;		//142;	//d_house_y;

	d_house_w = 60 *RESFACTOR;
	d_house_h = (8 * 5 *RESFACTOR);
	d_house_x = 466;	//65;	//d_color_x;	//d_dialog_cx - (d_house_w / 2);
	d_house_y = d_color_y;	// + 36;	//d_dialog_y + ( 7 * RESFACTOR ) + d_txt6_h + (2*RESFACTOR);

	if( bHost )
		d_disc_w = 365;	//d_dialog_w - (d_margin1 * 2) - 20*RESFACTOR;
	else
		d_disc_w = d_dialog_w - ( d_margin1 * 2 );
	d_disc_x = d_dialog_x + d_margin1;
	d_disc_y = 205;	//d_dialog_y + d_dialog_h - ( 65 + d_disc_h );
	d_disc_h = 337 - d_disc_y;

	d_playerlist_w = 124*RESFACTOR;
	d_playerlist_h = d_text_h * 4 + 4;
	d_playerlist_x = d_dialog_x + d_margin1;
	d_playerlist_y = 75;	//d_dialog_y + d_margin1 + d_txt6_h + 3*RESFACTOR + 18 * RESFACTOR;

	int d_tab_h = 19;

	d_scenariolist_w = 200;
//	d_scenariolist_h = (4 * d_txt6_h) + 3*RESFACTOR;		// 4 rows high
	d_scenariolist_x = d_dialog_x + d_dialog_w - d_margin1 - d_scenariolist_w;
	d_scenariolist_y = d_disc_y + d_tab_h;
	d_scenariolist_h = d_disc_y + d_disc_h - d_scenariolist_y;

	d_gamekind_w = 182;
	d_gamekind_h = 30;
	d_gamekind_x = 52;		//	370;
	d_gamekind_y = 153;		//	d_playerlist_y + d_text_h + 3;	// + d_playerlist_h - d_text_h;

	d_count_w = 25*RESFACTOR;
	d_count_h = d_txt6_h;
	d_count_x = 310;
	d_count_y = 138;	//d_playerlist_y + d_playerlist_h + d_margin2 + 9*RESFACTOR;

	d_level_w = 25*RESFACTOR;
	d_level_h = d_txt6_h;
	d_level_x = d_count_x;
	d_level_y = d_count_y + d_count_h;

	d_credits_w = 25*RESFACTOR;
	d_credits_h = d_txt6_h;
	d_credits_x = d_count_x;
	d_credits_y = d_level_y + d_level_h;

	d_aiplayers_w = 25*RESFACTOR;
	d_aiplayers_h = d_txt6_h;
	d_aiplayers_x = d_count_x;
	d_aiplayers_y = d_credits_y + d_credits_h;

#ifdef GERMAN
	d_options_w = 186;
#else
	d_options_w = 180;
#endif
	d_options_h = ((6 * 6) + 4)*RESFACTOR + 1;
	d_options_x = d_dialog_x + d_dialog_w - d_options_w - d_margin1;
	d_options_y = 127 - d_txt6_h + 2;

	d_send_w = d_dialog_w - ( d_margin1 * 2 );
	d_send_h = 9*RESFACTOR;
	d_send_x = d_dialog_x + d_margin1;
	d_send_y = d_disc_y + d_disc_h + 5;

//	d_ok_w = 50*RESFACTOR;
//	d_ok_h = 9*RESFACTOR;
//	d_ok_x = d_dialog_x + (d_dialog_w / 6) - (d_ok_w / 2);
//	d_ok_y = d_dialog_y + d_dialog_h - d_ok_h - d_margin1 - 3*RESFACTOR;

	d_cancel_w = 100;
	d_cancel_h = 9*RESFACTOR;
	d_cancel_x = d_dialog_x + 100; //d_dialog_cx - (d_cancel_w / 2);
	d_cancel_y = 365;	//d_dialog_y + d_dialog_h - d_cancel_h - d_margin1 - 3*RESFACTOR;

	d_accept_w = 100;
	d_accept_h = 9 *RESFACTOR;
	d_accept_x = d_dialog_x + 210;
	d_accept_y = d_cancel_y;	//d_dialog_y + d_dialog_h - 17*RESFACTOR;

	d_action_w = 100;
	d_action_h = 9 *RESFACTOR;
	d_action_x = d_dialog_x + 500;
	d_action_y = d_cancel_y;

/*	int d_load_w = 50*RESFACTOR;
	int d_load_h = 9*RESFACTOR;
	int d_load_x = d_dialog_x + ((d_dialog_w * 5) / 6) - (d_load_w / 2);
	int d_load_y = d_dialog_y + d_dialog_h - d_load_h - d_margin1 - 3*RESFACTOR;
*/
	d_amunits_w = 160;
	d_amunits_h = 18;
	d_amunits_x = d_dialog_x + (d_dialog_w / 6) - (d_amunits_w / 2);
	d_amunits_y = 365;

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

	pILPlayers = new IconListClass( BUTTON_PLAYERLIST, d_playerlist_x, d_playerlist_y, d_playerlist_w, d_playerlist_h, TPF_TYPE, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"), true, 2 );
//	ListClass scenariolist(BUTTON_SCENARIOLIST, d_scenariolist_x, d_scenariolist_y, d_scenariolist_w, d_scenariolist_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
	pILScens = new IconListClass( BUTTON_SCENARIOLIST, d_scenariolist_x, d_scenariolist_y, d_scenariolist_w, d_scenariolist_h, TPF_TYPE, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"), true, 1 );
	pILDisc = new IconListClass( BUTTON_DISCLIST, d_disc_x, d_disc_y, d_disc_w, d_disc_h, TPF_TYPE, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"), true, 0, 300 );

	pEditSend = new EditClass( BUTTON_SENDEDIT, szSendBuffer, MAXCHATSENDLENGTH, TPF_TEXT, d_send_x, d_send_y, d_send_w, d_send_h );

//	TextButtonClass rejectbtn( BUTTON_REJECT, TXT_REJECT, TPF_BUTTON, d_reject_x, d_reject_y );
	pGaugeCount = new GaugeClass( BUTTON_COUNT, d_count_x, d_count_y, d_count_w, d_count_h );
	pGaugeLevel = new GaugeClass( BUTTON_LEVEL, d_level_x, d_level_y, d_level_w, d_level_h );
	pGaugeCredits = new GaugeClass( BUTTON_CREDITS, d_credits_x, d_credits_y, d_credits_w, d_credits_h );
	pGaugeAIPlayers = new GaugeClass( BUTTON_AIPLAYERS, d_aiplayers_x, d_aiplayers_y, d_aiplayers_w, d_aiplayers_h );
	pCheckListOptions = new CheckListClass( BUTTON_PARAMS, d_options_x, d_options_y, d_options_w, d_options_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP") );
//	pTextBtnOk = new TextButtonClass( BUTTON_OK, TXT_OK, TPF_BUTTON, d_ok_x, d_ok_y, 60*RESFACTOR );
//	TextButtonClass loadbtn(BUTTON_LOAD, TXT_LOAD_BUTTON, TPF_BUTTON, d_load_x, d_load_y, 60*RESFACTOR);
	pTextBtnCancel = new TextButtonClass( BUTTON_CANCEL, TXT_WOL_CANCELGAME, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w );
	pTTipCancel = new ToolTipClass( pTextBtnCancel, TXT_WOL_TTIP_CANCELGAME, d_cancel_x + d_cancel_w/2, d_cancel_y - 6 );

	if( bHost )
	{
		pTextBtnAcceptStart = new TextButtonClass( BUTTON_ACCEPTSTART, TXT_WOL_STARTBUTTON, TPF_BUTTON, d_accept_x, d_accept_y,  d_accept_w );
		pTTipAcceptStart = new ToolTipClass( pTextBtnAcceptStart, TXT_WOL_TTIP_START, d_accept_x + d_accept_w/2, d_accept_y - 6 );
	}
	else
	{
		pTextBtnAcceptStart = new TextButtonClass( BUTTON_ACCEPTSTART, TXT_WOL_ACCEPTBUTTON, TPF_BUTTON, d_accept_x, d_accept_y, d_accept_w );
		pTTipAcceptStart = new ToolTipClass( pTextBtnAcceptStart, TXT_WOL_TTIP_ACCEPT, d_accept_x + d_accept_w/2, d_accept_y - 6 );
	}

	pTextBtnAction = new TextButtonClass( BUTTON_ACTION, TXT_WOL_ACTION, TPF_BUTTON, d_action_x, d_action_y, d_action_w );
	pTTipAction = new ToolTipClass( pTextBtnAction, TXT_WOL_TTIP_ACTION, d_action_x + d_action_w/2, d_action_y - 6, true );

	pToolTip = pToolTip->next;
	pToolTip->next = pTTipCancel;
	pToolTip = pToolTip->next;
	pToolTip->next = pTTipAcceptStart;
	pToolTip = pToolTip->next;
	pToolTip->next = pTTipAction;
	pToolTip = pToolTip->next;
	pToolTip->next = NULL;

	if( bHost )
		pTextBtnAcceptStart->Disable();

	//	pStaticDescrip is no longer used - can't get the bloody thing to clip text. You'd think a StaticButton control would.
//	pStaticDescrip = new StaticButtonClass( 0, "", TPF_TYPE, d_gamekind_x, d_gamekind_y, d_gamekind_w, d_gamekind_h );
	pStaticUnit = new StaticButtonClass( 0, "    ", TPF_TEXT, d_count_x + d_count_w + 2*RESFACTOR, d_count_y );
	pStaticLevel = new StaticButtonClass( 0, "    ", TPF_TEXT, d_level_x + d_level_w + 2*RESFACTOR, d_level_y );
	pStaticCredits = new StaticButtonClass( 0, "         ", TPF_TEXT, d_credits_x + d_credits_w + 2*RESFACTOR, d_credits_y );
	pStaticAIPlayers = new StaticButtonClass( 0, "   ", TPF_TEXT, d_aiplayers_x + d_aiplayers_w + 2*RESFACTOR, d_aiplayers_y );

	Fancy_Text_Print("", 0, 0, 0, 0, TPF_TEXT);
	pDropListHouse = new DropListClass( BUTTON_HOUSE, szHouseBuffer, sizeof( szHouseBuffer ),
		TPF_TEXT,
		d_house_x, d_house_y, d_house_w, d_house_h,
		MFCD::Retrieve("BTN-UP.SHP"),
		MFCD::Retrieve("BTN-DN.SHP") );

	//	ajw - This checkbox is not used. Could be turned on, though.
	pCheckAftermathUnits = new BigCheckBoxClass( BUTTON_AFTERMATHUNITS, d_amunits_x, d_amunits_y, d_amunits_w, d_amunits_h,
													"Aftermath units enabled", TPF_TEXT, false );

	if( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::AMGAME )
	{
		bAftermathUnits = true;
		int current_drive = CCFileClass::Get_CD_Drive();
		int cd_index = Get_CD_Index(current_drive, 1*60);
		if( cd_index != 3 && cd_index != 5 )
		{
			WOL_PrintMessage( *pILDisc, TXT_WOL_AMDISCNEEDED, WOLCOLORREMAP_LOCALMACHINEMESS );
		}
	}
	else
	{
		bAftermathUnits = false;
		pCheckAftermathUnits->Disable();
	}

	bSlowUnitBuildRate = true;

#define TABSPACING 38
	pShpBtnScenarioRA = new ShapeButtonClass( BUTTON_SCENARIO_RA, MFCD::Retrieve( "tabra.shp" ), d_scenariolist_x, d_scenariolist_y - d_tab_h );
	pShpBtnScenarioCS = new ShapeButtonClass( BUTTON_SCENARIO_CS, MFCD::Retrieve( "tabcs.shp" ), d_scenariolist_x + TABSPACING, d_scenariolist_y - d_tab_h );
	pShpBtnScenarioAM = new ShapeButtonClass( BUTTON_SCENARIO_AM, MFCD::Retrieve( "tabam.shp" ), d_scenariolist_x + TABSPACING, d_scenariolist_y - d_tab_h );

	int iScenarioUserTabPos;
	if( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::CSGAME || pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::AMGAME ||
		( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::RAGAME && Is_Aftermath_Installed() && !pWO->GameInfoCurrent.bTournament ) )
		//	Place user tab in the third tab position. (It may still not be present.)
		iScenarioUserTabPos = d_scenariolist_x + TABSPACING * 2;
	else
		iScenarioUserTabPos = d_scenariolist_x + TABSPACING;

	pShpBtnScenarioUser = new ShapeButtonClass( BUTTON_SCENARIO_USER, MFCD::Retrieve( "tabus.shp" ), iScenarioUserTabPos, d_scenariolist_y - d_tab_h );

	//	Change draw behavior of tab buttons.
	pShpBtnScenarioRA->ReflectButtonState = true;
	pShpBtnScenarioCS->ReflectButtonState = true;
	pShpBtnScenarioAM->ReflectButtonState = true;
	pShpBtnScenarioUser->ReflectButtonState = true;

	if( !bHost )
	{
		pILScens->Disable();
		pGaugeCount->Disable();
		pGaugeLevel->Disable();
		pGaugeCredits->Disable();
		pGaugeAIPlayers->Disable();
		pCheckListOptions->Disable();
		pCheckAftermathUnits->Disable();
	}

	pWO->LinkToGameDlg( pILDisc, pILPlayers );
	pWO->pGSupDlg = this;

	dwTimeNextParamRefresh = ::timeGetTime();
}

//***********************************************************************************************
RESULT_WOLGSUP WOL_GameSetupDialog::Show()
{
	//	Returns: -1 == return to chat dialog.

	//------------------------------------------------------------------------
	//	Dialog variables
	//------------------------------------------------------------------------
	bool bHackFocus = true;

	display = REDRAW_ALL;		// redraw level
	bProcess = true;						// process while true
	KeyNumType input;

	DWORD timeWaitingToStartTimeout;

	char szScenarioNameDisplay[ 300 ];

	bool	bInformParamChange;

	long ok_timer = 0;						// for timing OK button
	int i;
	int tabs[] = {77*RESFACTOR};			// tabs for player list box
	int optiontabs[] = {8*RESFACTOR};		// tabs for option list box

	CCFileClass loadfile ("SAVEGAME.NET");
	int load_game = 0;						// 1 = load a saved game
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	int cbox_x[] = {
							d_color_x,
							d_color_x + d_color_w,
							d_color_x + (d_color_w * 2),
							d_color_x + (d_color_w * 3),
							d_color_x + (d_color_w * 4),
							d_color_x + (d_color_w * 5),
							d_color_x + (d_color_w * 6),
							d_color_x + (d_color_w * 7),
					};

	bool bRetractHouseDropDown = false;

	if( !pWO->OnEnteringGameSetup() )		//	Gets a userlist setup, among other things.
		strcpy( szNameOfHostWhoJustBailedOnUs, TXT_WOL_THEGAMEHOST );		//	Will cause immediate exit.

	//	If I'm not already listed with a color, give myself a color.
	//	(I may have already received my assigned color from the game host.)
	int iItem = pILPlayers->Find( pWO->szMyName );		//	(I must be in the list I just got.)
	_ASSERTE( iItem != -1 );
	RemapControlType* pColorRemap = pILPlayers->Get_Item_Color( iItem );
	PlayerColorType Color = PlayerColorTypeOf( pColorRemap );

//	debugprint( "Starting up, I see myself as color %i\n", Color );
	if( Color == PCOLOR_NONE )
		SetPlayerColor( pWO->szMyName, ColorNextAvailable() );	//	Unless I'm host, this will be changed quite immediately,
																//	but nice to have it be a valid value until then.

	DWORD dwTimeNextPlayerPing = ::timeGetTime() + PING_AND_DISPLAY_WAIT;
	DWORD dwTimeNextPingDisplay = dwTimeNextPlayerPing + 1500;		//	Stagger ping and ping display periods.

	//------------------------------------------------------------------------
	//	Build the button list
	//------------------------------------------------------------------------
	BindControls( true );

	pILPlayers->Set_Tabs(tabs);

	//	If we have not already received a param update from a host, set default param values.
	if( !bParamsReceived )
	{
		if( !bHost )
			//	Accept button disabled until first params arrive.
			pTextBtnAcceptStart->Disable();

		Special.IsCaptureTheFlag = Rule.IsMPCaptureTheFlag;		//	Ugh. Use of "Special" global.
		if( bHost ) {
			Session.Options.Credits = Rule.MPDefaultMoney;			// init credits & credit buffer
			Session.Options.Bases = Rule.IsMPBasesOn;					// init scenario parameters
			Session.Options.Tiberium = Rule.IsMPTiberiumGrow;
			Session.Options.Goodies = Rule.IsMPCrates;
			Session.Options.AIPlayers = 0;
			Session.Options.UnitCount = (SessionClass::CountMax[Session.Options.Bases] + SessionClass::CountMin[Session.Options.Bases]) / 2;
			//first_time = 0;
		}
		//------------------------------------------------------------------------
		//	Init other scenario parameters
		//------------------------------------------------------------------------
		Special.IsTGrowth = Session.Options.Tiberium;		//	Ugh. Use of "Special" global.
		Rule.IsTGrowth = Session.Options.Tiberium;
		Special.IsTSpread = Session.Options.Tiberium;		//	Ugh. Use of "Special" global.
		Rule.IsTSpread = Session.Options.Tiberium;

		if( bHost )
		{
			//------------------------------------------------------------------------
			//	Set up array of lists of available scenarios.
			//------------------------------------------------------------------------
			for( i = 0; i < Session.Scenarios.Count(); i++ )
			{
				//	Reworking of the loop previously used for language translation. (What a hack I have inherited...)
				MultiMission* pMMission = Session.Scenarios[ i ];
				const char* szScenarioNameShow = pMMission->Description();
	#if defined( GERMAN ) || defined( FRENCH )
				for( int j = 0; EngMisStr[j] != NULL; j++ )
				{
					if( !strcmp( szScenarioNameShow, EngMisStr[j] ) )
					{
						//	Found description in translation array that matches mission.
						szScenarioNameShow = EngMisStr[ j + 1 ];
						break;
					}
				}
				//	(If no match found, defaults to English description.)
	#endif
				//	Place scenario name in a specific scenario list.
				if( pMMission->Get_Official() )
				{
					if( Is_Mission_Counterstrike( (char*)( Session.Scenarios[i]->Get_Filename() ) ) )
					{
	//					debugprint( " ---------------- Adding scenario %s as CS\n", szScenarioNameShow );
						ar_szScenarios[ SCENARIO_CS ].Add( szScenarioNameShow );
						ar_szScenIndexes[ SCENARIO_CS ].Add( (void*)i );
					}
					else if( Is_Mission_Aftermath( (char*)( Session.Scenarios[i]->Get_Filename() ) ) )
					{
	//					debugprint( " ---------------- Adding scenario %s as AM\n", szScenarioNameShow );
						//	If this is not an Aftermath game channel, we must filter out any AM maps that have
						//	special AM units on them.
						if( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::AMGAME ||
							!bSpecialAftermathScenario( pMMission->Description() ) )
						{
							ar_szScenarios[ SCENARIO_AM ].Add( szScenarioNameShow );
							ar_szScenIndexes[ SCENARIO_AM ].Add( (void*)i );
						}
					}
					else
					{
	//					debugprint( " ---------------- Adding scenario %s as RA\n", szScenarioNameShow );
						ar_szScenarios[ SCENARIO_RA ].Add( szScenarioNameShow );
						ar_szScenIndexes[ SCENARIO_RA ].Add( (void*)i );
					}
				}
				else
				{
	//				debugprint( " ---------------- Adding scenario %s as User\n", szScenarioNameShow );
					ar_szScenarios[ SCENARIO_USER ].Add( szScenarioNameShow );
					ar_szScenIndexes[ SCENARIO_USER ].Add( (void*)i );
				}
			}

			//	Set default scenario list viewing mode and prepare tab buttons.
	/*		switch( pWO->GameInfoCurrent.GameKind )
			{
			case CREATEGAMEINFO::RAGAME:
				ScenarioDisplayMode( SCENARIO_RA );
				break;
			case CREATEGAMEINFO::CSGAME:
				ScenarioDisplayMode( SCENARIO_CS );
				break;
			case CREATEGAMEINFO::AMGAME:
				ScenarioDisplayMode( SCENARIO_AM );
				break;
			default:
//				debugprint( "Illegal GameInfoCurrent value." );
				Fatal( "Illegal GameInfoCurrent value." );
			}
	*/
			Session.Options.ScenarioIndex = 0;		// 1st scenario is selected

			ScenarioDisplayMode( SCENARIO_RA );		//	Always start on RedAlert tab. Next line depends on selected item in
													//	list matching selected scenario.
//			pStaticDescrip->Set_Text( pILScens->Get_Item( pILScens->Current_Index() ), false );
			strcpy( szScenarioNameDisplay, pILScens->Get_Item( pILScens->Current_Index() ) );
		}

		Seed = rand();
	}

	//------------------------------------------------------------------------
	//	Init button states
	//------------------------------------------------------------------------
	pCheckListOptions->Set_Tabs(optiontabs);
	pCheckListOptions->Set_Read_Only(0);

	pCheckListOptions->Add_Item(Text_String(TXT_BASES));
	pCheckListOptions->Add_Item(Text_String(TXT_ORE_SPREADS));
	pCheckListOptions->Add_Item(Text_String(TXT_CRATES));
	pCheckListOptions->Add_Item(Text_String(TXT_CAPTURE_THE_FLAG));
	pCheckListOptions->Add_Item(Text_String(TXT_SHADOW_REGROWS));
	pCheckListOptions->Add_Item(TXT_WOL_SLOWUNITBUILD);

	SetSpecialControlStates();

	//........................................................................
	// House buttons
	//........................................................................
	for (HousesType house = HOUSE_USSR; house <= HOUSE_FRANCE; house++) {
		pDropListHouse->Add_Item(Text_String(HouseTypeClass::As_Reference(house).Full_Name()));
	}
	pDropListHouse->Set_Selected_Index(Session.House - HOUSE_USSR);
	pDropListHouse->Set_Read_Only (true);


	bInformParamChange = false;

//	PlayingAgainstVersion = VerNum.Version_Number();

	//------------------------------------------------------------------------
	//	Init random-number generator, & create a seed to be used for all random
	//	numbers from here on out
	//------------------------------------------------------------------------
	srand(time(NULL));

	//------------------------------------------------------------------------
	//	Init the version-clipping system
	//------------------------------------------------------------------------
	VerNum.Init_Clipping();


//	Load_Title_Page(true);
//	CCPalette.Set();	//GamePalette.Set();

	//
	// Now init the max range of the AI players slider.
	//
//	pGaugeAIPlayers->Set_Maximum(Rule.MaxPlayers-Session.Players.Count());
//	pGaugeAIPlayers->Set_Value(Session.Options.AIPlayers);

	Sound_Effect( WOLSOUND_ENTERGAME );

	//------------------------------------------------------------------------
	//	Processing loop
	//------------------------------------------------------------------------
	while (bProcess) {
		#if(SHOW_MONO)
		Ipx.Mono_Debug_Print(-1,0);
		#endif

		//	Check for change of house. Occurs on first loop and when user changes house.
		if( HousePrevious != Session.House )
		{
			if( bHost )
			{
				//	Host changed house.
				//	Do processing as if we'd received a message from a guest.

				//	Set house in our own list.
				SetPlayerHouse( pWO->szMyName, Session.House );
				//	Tell guests.
				nHostLastParamID++;
				InformAboutPlayerHouse( pWO->szMyName, Session.House, NULL );
				HousePrevious = Session.House;
				ClearAllAccepts();
			}
			else
			{
				User* pUserHost = pWO->pGameHost();
				if( pUserHost )		//	Else we have not received the user list yet and don't know who the host is.
									//	We'll keep trying this until we get a host - HousePrevious keeps us triggering until then.
				{
//					debugprint( "Session.House changed.\n" );
					//	Tell host we changed our house.
					char szSend[ 20 ];
					sprintf( szSend, "%02u %02u", WOL_GAMEOPT_REQHOUSE, Session.House );
					pWO->SendGameOpt( szSend, pUserHost );
					//	Set house in our own list. This is fine because we know that the change must be affirmed by the host.
					SetPlayerHouse( pWO->szMyName, Session.House );
					HousePrevious = Session.House;
				}
			}
		}

		//	Regularly ping other players to assess latencies.
		//	Display of ping results is on a parallel timer, slightly behind the ping so that we'll be likely to have
		//	a fresh average to show. Not too big a deal if OnPings haven't arrived by then (though they should have).
		if( ::timeGetTime() > dwTimeNextPlayerPing )
		{
			pWO->RequestPlayerPings();
			dwTimeNextPlayerPing = ::timeGetTime() + PING_AND_DISPLAY_WAIT;
		}
		if( ::timeGetTime() > dwTimeNextPingDisplay )
		{
			pWO->ListChannelUsers();
			dwTimeNextPingDisplay = ::timeGetTime() + PING_AND_DISPLAY_WAIT;
		}

		if( pWO->bShowRankUpdated )
		{
			pWO->bShowRankUpdated = false;
			pWO->ListChannelUsers();
		}

		//	Regularly check for incoming messages from wolapi.
		if( ::timeGetTime() > pWO->dwTimeNextWolapiPump )
		{
			pWO->pChat->PumpMessages();
			pWO->pNetUtil->PumpMessages();
			//	Special post-callback processing...
			if( pWO->pChatSink->bGotKickedTrigger )
			{
				//	We got kicked out of the channel.
				pWO->OnExitingGameChannel();
				pWO->RejoinLobbyAfterGame();
				bProcess = false;
				ResultReturn = RESULT_WOLGSUP_BACKTOCHAT;	//	Return to chat.
				//	Leave the bGotKickedTrigger flag so we can react to it upon reentering the chat dialog.
				//pWO->pChatSink->bGotKickedTrigger = false;
				display = REDRAW_ALL;
			}
			pWO->dwTimeNextWolapiPump = ::timeGetTime() + WOLAPIPUMPWAIT;
		}

		if( bHostSayGo )
		{
//			debugprint( "bHostSayGo trigger\n" );
			//	We are the host, now ready to tell everyone to GO and start our game.
			HostSaysGo();
/*	Part of old method of game start.
			bProcess = false;
			ResultReturn = RESULT_WOLGSUP_STARTGAMEHOST;
//			debugprint( "Host about to exit game setup dialog for game.\n" );
			if( !ExitGameChannel() )
				ResultReturn = RESULT_WOLGSUP_FATALERROR;	//	Return with an error value.
			break;
*/
		}
		else if( *szNameOfHostWhoJustBailedOnUs )		//	Host left channel - cancel setup.
		{
//			debugprint( "Guest about to exit game setup dialog because host bailed on us.\n" );
			if( ExitGameChannel() )
			{
				pWO->RejoinLobbyAfterGame();
				bProcess = false;
				ResultReturn = RESULT_WOLGSUP_HOSTLEFT;	//	Return to chat.
				//	Add a message explaining what happened to the saved chat that will be restored in the chat dialog.
				pWO->AddHostLeftMessageToSavedChat( szNameOfHostWhoJustBailedOnUs );
			}
			else
			{
				bProcess = false;
				ResultReturn = RESULT_WOLGSUP_FATALERROR;	//	Return with an error value.
			}
			break;
		}
		else if( bLeaveDueToRulesMismatchTrigger )
		{
//			debugprint( "Guest about to exit game setup dialog because of rules.ini mismatch.\n" );
			if( ExitGameChannel() )
			{
				pWO->RejoinLobbyAfterGame();
				bProcess = false;
				ResultReturn = RESULT_WOLGSUP_RULESMISMATCH;	//	Return to chat.
				//	Add a message explaining what happened to the saved chat that will be restored in the chat dialog.
				pWO->AddMessageToSavedChat( TXT_WOL_RULESMISMATCH );
			}
			else
			{
				bProcess = false;
				ResultReturn = RESULT_WOLGSUP_FATALERROR;	//	Return with an error value.
			}
			break;
		}
		else if( *szTriggerGameStartInfo )
		{
//			debugprint( "About to trigger game start.\n" );
			TriggerGameStart( szTriggerGameStartInfo );
		}
		else if( pWO->bSelfDestruct )
		{
			if( pWO->pChatSink->bConnected )
				pWO->Logout();
			bProcess = false;
			ResultReturn = RESULT_WOLGSUP_LOGOUT;		//	As if the user logged himself out.
		}

		if( bExitForGameTrigger )
		{
			//	We are now exiting to go into a game.
			bProcess = false;
			if( bHost )
				ResultReturn = RESULT_WOLGSUP_STARTGAMEHOST;
			else
				ResultReturn = RESULT_WOLGSUP_STARTGAME;
//			debugprint( "About to exit game setup dialog for game.\n" );
			if( !ExitGameChannel() )
				ResultReturn = RESULT_WOLGSUP_FATALERROR;	//	Return with an error value.
			break;
		}

		if( bHost && bWaitingToStart && !bExitForGameTrigger && timeGetTime() > timeWaitingToStartTimeout )
		{
			ClearAllAccepts();		//	Results in all required steps to cancel game start.
			WOL_PrintMessage( *pILDisc, TXT_WOL_STARTTIMEOUT, WOLCOLORREMAP_LOCALMACHINEMESS );
			Sound_Effect( WOLSOUND_ERROR );
		}

		//	Regularly send game param changes if I'm the host.
		if( bHost && !bHostSayGo && !bExitForGameTrigger && ::timeGetTime() > dwTimeNextParamRefresh )
		{
			if( bParamsUnfresh() )
			{
				nHostLastParamID++;
				SendParams();
				ClearAllAccepts();
			}
			dwTimeNextParamRefresh = ::timeGetTime() + PARAMREFRESHWAIT;
		}

		if( !bProcess )			//	Avoid redrawing if we're about to bail.
			break;

		//
		// If we have just received input focus again after running in the background then
		// we need to redraw.
		//
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}

		//.....................................................................
		//	Refresh display if needed
		//.....................................................................

		if( bRetractHouseDropDown )	//|| pCheckListOptions->Is_To_Redraw() )
		{
			bRetractHouseDropDown = false;
			if( pDropListHouse->IsDropped )
			{
				pDropListHouse->Collapse();
				if (display < REDRAW_BACKGROUND) display = REDRAW_BACKGROUND;
			}
		}
//		if( pILDisc->Is_To_Redraw() )
//		{
//			pDropListHouse->Flag_To_Redraw();
//		}

		if( display )
		{
			Hide_Mouse();

			/*
			** Collapse the country list if we are going to redraw the game list
			*/
//			if (pILScens->Is_To_Redraw() && pDropListHouse->IsDropped) {
//				pDropListHouse->Collapse();
//				if (display < REDRAW_BACKGROUND) display = REDRAW_BACKGROUND;
//			}

			//..................................................................
			//	Redraw backgound & dialog box
			//..................................................................
			if (display >= REDRAW_BACKGROUND) {
				if( pToolTipHitLast && pToolTipHitLast->bShowing )
					pToolTipHitLast->Unshow();

				if( pDropListHouse->IsDropped )
					pDropListHouse->Collapse();

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				//...............................................................
				//	Dialog & Field labels
				//...............................................................
				Fancy_Text_Print(TXT_PLAYERS, d_playerlist_x + (d_playerlist_w / 2), d_playerlist_y - d_txt6_h, scheme, TBLACK, TPF_TEXT | TPF_CENTER);
				if( bHost )
					Fancy_Text_Print( TXT_SCENARIOS, d_scenariolist_x + d_scenariolist_w, d_scenariolist_y - 12, scheme, TBLACK, TPF_TYPE | TPF_RIGHT );
//				else
//					Fancy_Text_Print( TXT_SCENARIO_COLON, d_scenariolist_x + (d_scenariolist_w / 2), d_scenariolist_y - d_txt6_h, scheme, TBLACK, TPF_TEXT | TPF_CENTER);
				Fancy_Text_Print(TXT_COUNT, d_count_x - 2*RESFACTOR, d_count_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
				Fancy_Text_Print(TXT_LEVEL, d_level_x - 2*RESFACTOR, d_level_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
				Fancy_Text_Print(TXT_CREDITS_COLON, d_credits_x - 2*RESFACTOR, d_credits_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
				Fancy_Text_Print(TXT_AI_PLAYERS_COLON, d_aiplayers_x - 2*RESFACTOR, d_aiplayers_y, scheme, TBLACK, TPF_TEXT | TPF_RIGHT);
				Fancy_Text_Print(TXT_SIDE_COLON,
//					d_house_x + (d_house_w / 2),
					d_house_x + ( ( d_house_w + 16 ) / 2 ),
					d_house_y - d_txt6_h,
					scheme, TBLACK,
					TPF_CENTER | TPF_TEXT);
				Fancy_Text_Print(TXT_COLOR_COLON,
					d_color_x + d_color_w * 4,
					d_color_y - d_txt6_h,
					scheme, TBLACK,
					TPF_CENTER | TPF_TEXT);

				const char* szGameKind;
				const char* pDIB;
				switch( pWO->GameInfoCurrent.GameKind )
				{
				case CREATEGAMEINFO::RAGAME:
					szGameKind = TXT_WOL_CG_RAGAME;
					pDIB = pWO->OldRAGameTypeInfos[ 0 ].pDIB;
					break;
				case CREATEGAMEINFO::CSGAME:
					szGameKind = TXT_WOL_CG_CSGAME;
					pDIB = pWO->OldRAGameTypeInfos[ 1 ].pDIB;
					break;
				case CREATEGAMEINFO::AMGAME:
					szGameKind = TXT_WOL_CG_AMGAME;
					pDIB = pWO->OldRAGameTypeInfos[ 2 ].pDIB;
					break;
				default:
//					debugprint( "Illegal GameInfoCurrent value." );
					//Fatal( "Illegal GameInfoCurrent value." );
					szGameKind = TXT_WOL_CG_AMGAME;
					pDIB = NULL;
					pWO->bSelfDestruct = true;
					break;
				}
				int iGameInfoSpacingY = 14;
				int iGameInfoSecondColumnX = 0;	//170;
				//	Game kind.
				Fancy_Text_Print( szGameKind, d_gamekind_x, d_gamekind_y - iGameInfoSpacingY * 1, scheme, TBLACK, TPF_TYPE );
				//	Game kind icon.
				CC_Draw_DIB( pDIB, d_gamekind_x - 16, d_gamekind_y - iGameInfoSpacingY * 1 - 2, 100, WINDOW_MAIN );
				//	"Tournament."
				if( pWO->GameInfoCurrent.bTournament )
				{
					Fancy_Text_Print( TXT_WOL_CG_TOURNAMENT, d_gamekind_x + iGameInfoSecondColumnX,
										d_gamekind_y + iGameInfoSpacingY * 1, scheme, TBLACK, TPF_TYPE );
					CC_Draw_DIB( pWO->DibIconInfos[ DIBICON_TOURNAMENT ].pDIB, d_gamekind_x + iGameInfoSecondColumnX - 16,
										d_gamekind_y + iGameInfoSpacingY * 1 - 2, 100, WINDOW_MAIN );
				}
				//	"Password: ..."
				if( pWO->GameInfoCurrent.bPrivate )
				{
					char szPrivatePassword[ 100 ];
					sprintf( szPrivatePassword, TXT_WOL_PRIVATEPASSWORD, pWO->GameInfoCurrent.szPassword );
					Fancy_Text_Print( szPrivatePassword, d_gamekind_x + iGameInfoSecondColumnX,
										d_gamekind_y + iGameInfoSpacingY * 2, scheme, TBLACK, TPF_TYPE );
					CC_Draw_DIB( pWO->DibIconInfos[ DIBICON_PRIVATE ].pDIB, d_gamekind_x + iGameInfoSecondColumnX - 16,
										d_gamekind_y + iGameInfoSpacingY * 2 - 2, 100, WINDOW_MAIN );
				}
				//	"Scenario:" - scenario name is drawn separately.
				//Fancy_Text_Print( TXT_SCENARIO_COLON, d_gamekind_x, d_gamekind_y - iGameInfoSpacingY, scheme, TBLACK, TPF_TYPE );
			}

			//..................................................................
			//	Redraw buttons
			//..................................................................
			if (display >= REDRAW_BUTTONS) {

				commands->Draw_All();
			}

			//..................................................................
			//	Draw the color boxes
			//..................................................................
			if (display >= REDRAW_COLORS ) {
				for (i = 0; i < MAX_MPLAYER_COLORS; i++) {
					LogicPage->Fill_Rect (cbox_x[i] + 1, d_color_y + 1,
						cbox_x[i] + 1 + d_color_w - 2 *RESFACTOR, d_color_y + 1 + d_color_h - 2,
						ColorRemaps[i].Box);

					if (i == Session.ColorIdx) {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h, BOXSTYLE_DOWN, false);
					} else {
						Draw_Box(cbox_x[i], d_color_y, d_color_w, d_color_h, BOXSTYLE_RAISED, false);
					}
				}
			}

			//..................................................................
			//	Draw the messages:
			//	- Erase an old message first
			//	- If we're in a game, print the game options (if they've been
			//	  received)
			//	- If we've been rejected from a game, print that message
			//..................................................................
			if (display >= REDRAW_MESSAGE) {
//				Draw_Box(d_disc_x, d_disc_y, d_disc_w, d_disc_h, BOXSTYLE_BOX, true);
//				Draw_Box(d_send_x, d_send_y, d_send_w, d_send_h, BOXSTYLE_BOX, true);
//				Session.Messages.Draw();
			}

			//..................................................................
			// Update game parameter labels
			//..................................................................
			if (display >= REDRAW_PARMS)
			{
				char txt[80];

				char* szScenarioDesc;
				bool bOfficial;
				char* szScenarioFileName;
				if( !bHost )
				{
					szScenarioDesc = Session.Options.ScenarioDescription;
					bOfficial = Session.ScenarioIsOfficial;
					szScenarioFileName = Session.ScenarioFileName;
				}
				else
				{
					szScenarioDesc = (char*)Session.Scenarios[ Session.Options.ScenarioIndex ]->Description();
					bOfficial = Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Official();
					szScenarioFileName = (char*)Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Filename();
				}

				if( *szScenarioDesc )
				{
					//	Language translation.
					for (int ii = 0; EngMisStr[ii] != NULL;  ii++) {
						if (!strcmp( szScenarioDesc, EngMisStr[ii]) ) {
							#if defined(GERMAN) || defined(FRENCH)
								//sprintf(txt, "%s %s", Text_String(TXT_SCENARIO_COLON), EngMisStr[ii+1]);
								sprintf(txt, "%s", EngMisStr[ii+1]);
							#else
								//sprintf(txt, "%s %s", Text_String(TXT_SCENARIO_COLON), Session.Options.ScenarioDescription);
								sprintf(txt, "%s", szScenarioDesc);
							#endif
							break;
						}
					}
					if (EngMisStr[ii] == NULL) {
						sprintf(txt, "%s", szScenarioDesc);
					}
//					pStaticDescrip->Set_Text( txt, false );
					strcpy( szScenarioNameDisplay, txt );

					//	Show icon for gamekind of scenario.
					const char* pDIB;

					if( bOfficial )
					{
						if( Is_Mission_Counterstrike( szScenarioFileName ) )
							pDIB = pWO->OldRAGameTypeInfos[ 1 ].pDIB;
						else if( Is_Mission_Aftermath( szScenarioFileName ) )
							pDIB = pWO->OldRAGameTypeInfos[ 2 ].pDIB;
						else
							pDIB = pWO->OldRAGameTypeInfos[ 0 ].pDIB;
					}
					else
						pDIB = pWO->DibIconInfos[ DIBICON_USER ].pDIB;

					DrawScenarioDescripIcon( pDIB );
				}
				else
				{
					//sprintf(txt, "%s %s", Text_String(TXT_SCENARIO_COLON), Text_String(TXT_NOT_FOUND));
					sprintf(txt, "%s", TXT_WOL_SCENARIONAMEWAIT );
//					pStaticDescrip->Set_Text( txt, false );
					strcpy( szScenarioNameDisplay, txt );
				}

				//	Print scenario name.
				Conquer_Clip_Text_Print( szScenarioNameDisplay, d_gamekind_x, d_gamekind_y, scheme, TBLACK, TPF_TYPE, d_gamekind_w );
//				pStaticDescrip->Draw_Me();

				sprintf(txt,"%d",Session.Options.UnitCount);
				pStaticUnit->Set_Text(txt);
				pStaticUnit->Draw_Me();

				if (BuildLevel <= MPLAYER_BUILD_LEVEL_MAX) {
					sprintf(txt,"%d",BuildLevel);
				} else {
					sprintf(txt, "**");
				}
				pStaticLevel->Set_Text(txt);
				pStaticLevel->Draw_Me();

				sprintf(txt,"%d",Session.Options.Credits);
				pStaticCredits->Set_Text(txt);
				pStaticCredits->Draw_Me();

				sprintf(txt,"%d",Session.Options.AIPlayers);
				pStaticAIPlayers->Set_Text(txt);
				pStaticAIPlayers->Draw_Me();
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
			timeToolTipAppear = ::timeGetTime() + TOOLTIPDELAY;
			if( pToolTipHitLast && pToolTipHitLast->bShowing )
				pToolTipHitLast->Unshow();
		}

		input = commands->Input();

		if( bHackFocus )
		{
			pEditSend->Set_Focus();
			pEditSend->Flag_To_Redraw();
			input = commands->Input();
			bHackFocus = false;
		}

		//	Tooltips...
		if( pToolTipHead )
		{
			ToolTipClass* pToolTipHit = pToolTipHead->GetToolTipHit();
			if( pToolTipHit == pToolTipHitLast )
			{
				if( pToolTipHit && !pToolTipHit->bShowing && ::timeGetTime() > timeToolTipAppear && !( ( ::GetAsyncKeyState( KN_LMOUSE ) & 0x8000 ) || ( ::GetAsyncKeyState( KN_RMOUSE ) & 0x8000 ) ) )
				{
					pToolTipHit->Show();
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

		//	Yummy special hack. Luv' that UI system.
		if( input == 2049 )		//	Left mouse released, not on control that captured it.
		{
			//	Redraw the tabs in case they were what were pressed down on.
			pShpBtnScenarioRA->Flag_To_Redraw();
			pShpBtnScenarioCS->Flag_To_Redraw();
			pShpBtnScenarioAM->Flag_To_Redraw();
			pShpBtnScenarioUser->Flag_To_Redraw();
		}

		//.....................................................................
		//	Process input
		//.....................................................................
		switch( input )
		{
			case KN_LMOUSE:
				if( !bWaitingToStart )
				{
					//	Check for mouse down on a control when player is not host.
					if( !bHost )
					{
						if ( (
								Get_Mouse_X() >= d_count_x &&
								Get_Mouse_X() <= d_count_x + d_count_w &&
								Get_Mouse_Y() >= d_count_y &&
								Get_Mouse_Y() <= d_aiplayers_y + d_aiplayers_h
							) || (
								Get_Mouse_X() >= d_options_x &&
								Get_Mouse_X() <= d_options_x + d_options_w &&
								Get_Mouse_Y() >= d_options_y &&
								Get_Mouse_Y() <= d_options_y + d_options_h
							) || (
								Get_Mouse_X() >= d_scenariolist_x &&
								Get_Mouse_X() <= d_scenariolist_x + d_scenariolist_w &&
								Get_Mouse_Y() >= d_scenariolist_y &&
								Get_Mouse_Y() <= d_scenariolist_y + d_scenariolist_h
							) )
						{
							//Session.Messages.Add_Message(NULL, 0, (char *)Text_String(TXT_ONLY_HOST_CAN_MODIFY), PCOLOR_BROWN, TPF_TEXT, 1200);
							WOL_PrintMessage( *pILDisc, Text_String( TXT_ONLY_HOST_CAN_MODIFY ), WOLCOLORREMAP_LOCALMACHINEMESS );
							Sound_Effect( WOLSOUND_ERROR );
							if (display < REDRAW_MESSAGE) display = REDRAW_MESSAGE;
							break;
						}
					}

					if (Keyboard->MouseQX > cbox_x[0] &&
						Keyboard->MouseQX < (cbox_x[MAX_MPLAYER_COLORS - 1] + d_color_w) &&
						Keyboard->MouseQY > d_color_y &&
						Keyboard->MouseQY < (d_color_y + d_color_h))
					{
						Session.PrefColor = (PlayerColorType)
							((Keyboard->MouseQX - cbox_x[0]) / d_color_w);

						//	Ensure that no one is using this color (to our knowledge).
						if( pILPlayers->FindColor( &ColorRemaps[ Session.PrefColor == PCOLOR_DIALOG_BLUE ? PCOLOR_REALLY_BLUE : Session.PrefColor ] ) == -1 )
						{
							//	Show me as the new color.
//							debugprint( "Color box pressed - " );
							SetPlayerColor( pWO->szMyName, Session.PrefColor );
							if( bHost )
							{
								//	Tell all guests about the color change.
								InformAboutPlayerColor( pWO->szMyName, Session.PrefColor, NULL );
							}
							else
							{
								RequestPlayerColor( Session.PrefColor );
							}
						}

					}
				}
				break;

			case ( BUTTON_DISCONNECT | KN_BUTTON ):
				if( WWMessageBox().Process( TXT_WOL_CONFIRMLOGOUT, TXT_YES, TXT_NO ) == 0 )
				{
//					debugprint( "Logging out from gsup.\n" );
					ExitGameChannel();
					pWO->Logout();
					bProcess = false;
					ResultReturn = RESULT_WOLGSUP_LOGOUT;
				}
				display = REDRAW_ALL;
				bHackFocus = true;
				break;

			case ( BUTTON_REFRESH | KN_BUTTON ):		//	Always disabled.
				break;

			case ( BUTTON_SQUELCH | KN_BUTTON ):
				pWO->DoSquelch( pILPlayers );
				break;

			case ( BUTTON_BAN | KN_BUTTON ):
				pWO->DoKick( pILPlayers, true );
//				display = REDRAW_ALL;
				break;

			case ( BUTTON_KICK | KN_BUTTON ):
				pWO->DoKick( pILPlayers, false );
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

			case ( BUTTON_HOUSE | KN_BUTTON ):
				Session.House = (HousesType)( pDropListHouse->Current_Index() + HOUSE_USSR );
/*
				//	Bloody bloody hell I can't believe there are bugs in RA like the one I deal with here...
				if( strcmp( pDropListHouse->Current_Item(), "Russia" ) == 0 )
					Session.House = HOUSE_USSR;
				else
				{
					Session.House = HouseTypeClass::From_Name( pDropListHouse->Current_Item() );	//	Fails on "Russia". (Thinks "USSR".)
					if( Session.House == HOUSE_NONE )
					{
//						debugprint( "Couldn't find house from selected '%s'.\n", pDropListHouse->Current_Item() );
					}
				}
*/
				if( pDropListHouse->IsDropped )
					bRetractHouseDropDown = true;
				else
					if (display < REDRAW_BACKGROUND) display = REDRAW_BACKGROUND;		//	Droplist already got contracted.

				Sound_Effect(VOC_OPTIONS_CHANGED);
				break;

			case ( BUTTON_AFTERMATHUNITS | KN_BUTTON ):
				bAftermathUnits = pCheckAftermathUnits->IsOn;
				break;

			case ( BUTTON_SENDEDIT | KN_BUTTON ):
				//	Enter has been pressed - was caught by pEditSend control.
				pWO->SendMessage( pEditSend->Get_Text(), *pILPlayers, false );
				//	Clear pEditSend, reset focus.
				szSendBuffer[0] = 0;
				pEditSend->Set_Focus();
				//	Mark for redraw.
				pEditSend->Flag_To_Redraw();
				break;

			case ( BUTTON_ACTION | KN_BUTTON ):
				//	Enter has been pressed - was caught by pEditSend control.
				pWO->SendMessage( pEditSend->Get_Text(), *pILPlayers, true );
				//	Clear pEditSend, reset focus.
				szSendBuffer[0] = 0;
				pEditSend->Set_Focus();
				//	Mark for redraw.
				pEditSend->Flag_To_Redraw();
				break;

			//..................................................................
			//	New Scenario selected.
			//..................................................................
			case (BUTTON_SCENARIOLIST | KN_BUTTON):
			{
				if( pILScens->Count() )
				{
					int iSelectedScenIndex = (int)pILScens->Get_Item_ExtraDataPtr( pILScens->Current_Index() );
					if( iSelectedScenIndex != Session.Options.ScenarioIndex )
					{
						Session.Options.ScenarioIndex = iSelectedScenIndex;
						bInformParamChange = true;
						if( !pILScens->SetSelectType( 1 ) )	//	Hack to deal with ListClass "highlighting nothing" problem.
							//	SelectType was 0 before call.
							pILScens->Flag_To_Redraw();
//						pStaticDescrip->Set_Text( pILScens->Get_Item( pILScens->Current_Index() ), false );
						strcpy( szScenarioNameDisplay, pILScens->Get_Item( pILScens->Current_Index() ) );
						//if (display < REDRAW_PARMS) display = REDRAW_PARMS;
						display = REDRAW_ALL;
						Sound_Effect(VOC_OPTIONS_CHANGED);
					}
				}
				break;
			}
			//..................................................................
			//	User adjusts max # units
			//..................................................................
			case (BUTTON_COUNT | KN_BUTTON):
				Session.Options.UnitCount = pGaugeCount->Get_Value() +
					SessionClass::CountMin[Session.Options.Bases];
				bInformParamChange = true;
				if (display < REDRAW_PARMS) display = REDRAW_PARMS;
				Sound_Effect(VOC_OPTIONS_CHANGED);
				break;

			//..................................................................
			//	User adjusts build level
			//..................................................................
			case (BUTTON_LEVEL | KN_BUTTON):
				BuildLevel = pGaugeLevel->Get_Value() + 1;
				if (BuildLevel > MPLAYER_BUILD_LEVEL_MAX)	// if it's pegged, max it out
					BuildLevel = MPLAYER_BUILD_LEVEL_MAX;
				bInformParamChange = true;
				if (display < REDRAW_PARMS) display = REDRAW_PARMS;
				Sound_Effect(VOC_OPTIONS_CHANGED);
				break;

			//..................................................................
			//	User edits the credits value; retransmit new game options
			// Round the credits to the nearest 500.
			//..................................................................
			case (BUTTON_CREDITS | KN_BUTTON):
				Session.Options.Credits = pGaugeCredits->Get_Value();
				Session.Options.Credits =
					((Session.Options.Credits + 250) / 500) * 500;
				bInformParamChange = true;
				if (display < REDRAW_PARMS) display = REDRAW_PARMS;
				Sound_Effect(VOC_OPTIONS_CHANGED);
				break;

			//..................................................................
			//	User adjusts # of AI players
			//..................................................................
			case (BUTTON_AIPLAYERS | KN_BUTTON):
				Session.Options.AIPlayers = pGaugeAIPlayers->Get_Value();
//				if (Session.Options.AIPlayers+Session.Players.Count() > Rule.MaxPlayers) {	// if it's pegged, max it out
				if (Session.Options.AIPlayers + pWO->GameInfoCurrent.iPlayerMax > Rule.MaxPlayers) {	// if it's pegged, max it out
					Session.Options.AIPlayers = Rule.MaxPlayers - pWO->GameInfoCurrent.iPlayerMax;
					pGaugeAIPlayers->Set_Value(Session.Options.AIPlayers);
				}
				bInformParamChange = true;
				if (display < REDRAW_PARMS) display = REDRAW_PARMS;
				Sound_Effect(VOC_OPTIONS_CHANGED);
				break;

			//..................................................................
			// Toggle-able options:
			// If 'Bases' gets toggled, we have to change the range of the
			// UnitCount slider.
			// Also, if Tiberium gets toggled, we have to set the flags
			// in SpecialClass.
			//..................................................................
			case ( BUTTON_PARAMS | KN_BUTTON ):
				bRetractHouseDropDown = true;
				if (Special.IsCaptureTheFlag != pCheckListOptions->Is_Checked(3) && !Special.IsCaptureTheFlag) {
					pCheckListOptions->Check_Item(0, true);
				}
				if (Session.Options.Bases != pCheckListOptions->Is_Checked(0)) {
					Session.Options.Bases = pCheckListOptions->Is_Checked(0);
					if (Session.Options.Bases) {
						Session.Options.UnitCount = Fixed_To_Cardinal (
							SessionClass::CountMax[1] -
							SessionClass::CountMin[1],
							Cardinal_To_Fixed(
								SessionClass::CountMax[0]-SessionClass::CountMin[0],
								Session.Options.UnitCount-SessionClass::CountMin[0])) +
							SessionClass::CountMin[1];
					} else {
						pCheckListOptions->Check_Item(3, false);
						Session.Options.UnitCount = Fixed_To_Cardinal (
							SessionClass::CountMax[0] -
							SessionClass::CountMin[0],
							Cardinal_To_Fixed(
								SessionClass::CountMax[1]-SessionClass::CountMin[1],
								Session.Options.UnitCount - SessionClass::CountMin[1])) +
							SessionClass::CountMin[0];
					}
					pGaugeCount->Set_Maximum(
						SessionClass::CountMax[Session.Options.Bases] -
						SessionClass::CountMin[Session.Options.Bases]);
					pGaugeCount->Set_Value(Session.Options.UnitCount -
						SessionClass::CountMin[Session.Options.Bases]);
				}
				Session.Options.Tiberium = pCheckListOptions->Is_Checked(1);
				Special.IsTGrowth = Session.Options.Tiberium;			//	Ugh. Use of "Special" global.
				Rule.IsTGrowth = Session.Options.Tiberium;
				Special.IsTSpread = Session.Options.Tiberium;			//	Ugh. Use of "Special" global.
				Rule.IsTSpread = Session.Options.Tiberium;

				Session.Options.Goodies = pCheckListOptions->Is_Checked(2);
				Special.IsCaptureTheFlag = pCheckListOptions->Is_Checked(3);	//	Ugh. Use of "Special" global.
				Special.IsShadowGrow = pCheckListOptions->Is_Checked(4);		//	Ugh. Use of "Special" global.

				bSlowUnitBuildRate = pCheckListOptions->Is_Checked(5);

				bInformParamChange = true;
				if (display < REDRAW_PARMS) display = REDRAW_PARMS;
				Sound_Effect(VOC_OPTIONS_CHANGED);
				break;

			case ( BUTTON_ACCEPTSTART | KN_BUTTON ):		//	'Accept' or 'Start Game' button.
				if( !bHost )
				{
					//	Guest wishes to accept game params.
					User* pUserHost = pWO->pGameHost();
					if( pUserHost )			//	Else it's too early to even be thinking about accepting, anyway.
					{
						//	Set ourself as accepted. We want to do this immediately so the user has feedback for pressing button.
						//	Besides, the host is guaranteed to allow this and set us to "accepted", unless it is simultaneously
						//	sending us a new update we haven't received yet. If this is the case, we will get that update in a
						//	second and will set ourselves to unaccepted, which will match the unaccepted state on the server.
						//	Upshot - We can ignore messages from the server telling us that we, ourself, accepted.
						if( pToolTipHitLast && pToolTipHitLast->bShowing )
							pToolTipHitLast->Unshow();
						pTextBtnAcceptStart->Disable();
						if( SetPlayerAccepted( pWO->szMyName, true ) )	//	Else we didn't find ourself in list!
						{
//							debugprint( "Sending accept.\n" );
							//	Tell host we accept.
							char szSend[ 20 ];
							sprintf( szSend, "%02u %06u", WOL_GAMEOPT_REQACCEPT, nGuestLastParamID );
							pWO->SendGameOpt( szSend, pUserHost );
						}
					}
				}
				else
				{
					//	Host says start the game.
					//	If we have changes just made and not yet sent, don't say start, because we're about to send changes
					//	that will unaccept everyone.
					if( !bParamsUnfresh() )
					{
						//	Force user to put the correct disk in before proceeding. (Not crucial, but can lead to ugly
						//	timeouts if the scenario has to be downloaded before game start.)
						if( !Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Official() ||
							Force_Scenario_Available( Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Filename() ) )
						{
							//	Go into "waiting to start" mode, tell guests to, and wait for responses.
							bWaitingToStart = true;
							timeWaitingToStartTimeout = ::timeGetTime() + 30000;
							nHostLastParamID++;
							InformAboutStart();
							WWMessageBox().Process( TXT_WOL_WAITINGTOSTART, TXT_NONE );
							BindControls( false );
							SetPlayerReadyToGo( pWO->szMyName, "ready" );
							Sound_Effect( VOC_GAME_CLOSED );
						}
					}
				}
				break;

			case ( BUTTON_SCENARIO_RA | KN_BUTTON ):
				ScenarioDisplayMode( SCENARIO_RA );
				break;
			case ( BUTTON_SCENARIO_CS | KN_BUTTON ):
				ScenarioDisplayMode( SCENARIO_CS );
				break;
			case ( BUTTON_SCENARIO_AM | KN_BUTTON ):
				ScenarioDisplayMode( SCENARIO_AM );
				break;
			case ( BUTTON_SCENARIO_USER | KN_BUTTON ):
				ScenarioDisplayMode( SCENARIO_USER );
				break;

//			case (BUTTON_LOAD | KN_BUTTON):
//			case (BUTTON_OK | KN_BUTTON):
//				break;

			case (KN_ESC):
				if( pDropListHouse->IsDropped )
					bRetractHouseDropDown = true;
				break;

			case ( BUTTON_LEAVE | KN_BUTTON ):
			case ( BUTTON_CANCEL | KN_BUTTON ):
				if( ExitGameChannel() )
				{
					pWO->RejoinLobbyAfterGame();
					bProcess = false;
					ResultReturn = RESULT_WOLGSUP_BACKTOCHAT;	//	Return to chat.
				}
				else
				{
					bProcess = false;
					ResultReturn = RESULT_WOLGSUP_FATALERROR;	//	Return with an error value.
				}
				break;
		}

		Call_Back();
	}

	if( pToolTipHitLast && pToolTipHitLast->bShowing )
		pToolTipHitLast->Unshow();

	pWO->ClearListPtrs();
	pWO->pGSupDlg = NULL;

	return ResultReturn;
}

//***********************************************************************************************
void WOL_GameSetupDialog::SetSpecialControlStates()
{
	//	Set gauges and checklist.

	pCheckListOptions->Check_Item(0, Session.Options.Bases);
	pCheckListOptions->Check_Item(1, Session.Options.Tiberium);
	pCheckListOptions->Check_Item(2, Session.Options.Goodies);
	pCheckListOptions->Check_Item(3, Special.IsCaptureTheFlag);		//	Ugh. Use of "Special" global.
	pCheckListOptions->Check_Item(4, Special.IsShadowGrow);			//	Ugh. Use of "Special" global.

	pCheckListOptions->Check_Item(5, bSlowUnitBuildRate);			//	Ugh. Use of "Special" global.

	pGaugeCount->Set_Maximum(SessionClass::CountMax[Session.Options.Bases] - SessionClass::CountMin[Session.Options.Bases]);
	pGaugeCount->Set_Value(Session.Options.UnitCount - SessionClass::CountMin[Session.Options.Bases]);

	pGaugeLevel->Set_Maximum(MPLAYER_BUILD_LEVEL_MAX - 1);
	pGaugeLevel->Set_Value(BuildLevel - 1);

	pGaugeCredits->Set_Maximum(Rule.MPMaxMoney);
	pGaugeCredits->Set_Value(Session.Options.Credits);

	if( pWO->GameInfoCurrent.bTournament )
	{
		pGaugeAIPlayers->Set_Maximum( 0 );
	}
	else
	{
		//	Note dependency of AIPlayers on number of human players.
	//	pGaugeAIPlayers->Set_Maximum(Rule.MaxPlayers-Session.Players.Count());
		pGaugeAIPlayers->Set_Maximum( Rule.MaxPlayers - pWO->GameInfoCurrent.iPlayerMax );
	}
	pGaugeAIPlayers->Set_Value(Session.Options.AIPlayers);

	if( bAftermathUnits )
		pCheckAftermathUnits->Turn_On();
	else
		pCheckAftermathUnits->Turn_Off();
}

#define DRAWTABDOWN		Turn_On()
#define DRAWTABUP		Turn_Off()

//***********************************************************************************************
void WOL_GameSetupDialog::BindControls( bool bBind )
{
	if( bBind )
	{
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
		pILPlayers->Add_Tail(*commands);
		if( bHost )
		{
			//	Draw order of tabs depends on which one is selected, as we want them to overlap appropriately.
			//	Also, the selected tab must appear over the scenario list, while the unselected tabs are below it.
			//	This assures that that bottom of the tab overlaps the scenario list border, making it look connected.

			//	ajw I could make all maps always available now that they're downloadable. Playing CS map with AM rules
			//	would mean switching CDs, though.
			//	Would mean no difference between RA and CS games.

			switch( ScenKindCurrent )
			{
			case SCENARIO_RA:
				if( !pWO->GameInfoCurrent.bTournament )
				{
					pShpBtnScenarioUser->Add_Tail(*commands);
					pShpBtnScenarioUser->DRAWTABDOWN;
				}
				if( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::CSGAME )
				{
					pShpBtnScenarioCS->Add_Tail(*commands);
					pShpBtnScenarioCS->DRAWTABDOWN;
				}
				else if( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::AMGAME ||
						( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::RAGAME && Is_Aftermath_Installed() && !pWO->GameInfoCurrent.bTournament )	)
				{
					pShpBtnScenarioAM->Add_Tail(*commands);
					pShpBtnScenarioAM->DRAWTABDOWN;
				}
				pILScens->Add_Tail(*commands);
				pShpBtnScenarioRA->Add_Tail(*commands);
				pShpBtnScenarioRA->DRAWTABUP;
				break;
			case SCENARIO_CS:		//	pWO->GameInfoCurrent.GameKind must be CREATEGAMEINFO::CSGAME.
				if( !pWO->GameInfoCurrent.bTournament )
				{
					pShpBtnScenarioUser->Add_Tail(*commands);
					pShpBtnScenarioUser->DRAWTABDOWN;
				}
				pShpBtnScenarioRA->Add_Tail(*commands);
				pShpBtnScenarioRA->DRAWTABDOWN;
				pILScens->Add_Tail(*commands);
				pShpBtnScenarioCS->Add_Tail(*commands);
				pShpBtnScenarioCS->DRAWTABUP;
				break;
			case SCENARIO_AM:		//	pWO->GameInfoCurrent.GameKind must be CREATEGAMEINFO::AMGAME, or RAGAME with AM installed.
				if( !pWO->GameInfoCurrent.bTournament )
				{
					pShpBtnScenarioUser->Add_Tail(*commands);
					pShpBtnScenarioUser->DRAWTABDOWN;
				}
				pShpBtnScenarioRA->Add_Tail(*commands);
				pShpBtnScenarioRA->DRAWTABDOWN;
				pILScens->Add_Tail(*commands);
				pShpBtnScenarioAM->Add_Tail(*commands);
				pShpBtnScenarioAM->DRAWTABUP;
				break;
			case SCENARIO_USER:		//	pWO->GameInfoCurrent.bTournament must be false.
				pShpBtnScenarioRA->Add_Tail(*commands);
				pShpBtnScenarioRA->DRAWTABDOWN;
				if( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::CSGAME )
				{
					pShpBtnScenarioCS->Add_Tail(*commands);
					pShpBtnScenarioCS->DRAWTABDOWN;
				}
				else if( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::AMGAME ||
						( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::RAGAME && Is_Aftermath_Installed() && !pWO->GameInfoCurrent.bTournament ) )
				{
					pShpBtnScenarioAM->Add_Tail(*commands);
					pShpBtnScenarioAM->DRAWTABDOWN;
				}
				pILScens->Add_Tail(*commands);
				pShpBtnScenarioUser->Add_Tail(*commands);
				pShpBtnScenarioUser->DRAWTABUP;
				break;
			}
		}
//		pStaticDescrip->Add_Tail(*commands);
		pEditSend->Add_Tail(*commands);
		pStaticUnit->Add_Tail(*commands);
		pStaticLevel->Add_Tail(*commands);
		pStaticCredits->Add_Tail(*commands);
		pStaticAIPlayers->Add_Tail(*commands);
		pGaugeCount->Add_Tail(*commands);
		pGaugeCredits->Add_Tail(*commands);
		pGaugeAIPlayers->Add_Tail(*commands);
		pGaugeLevel->Add_Tail(*commands);
		pCheckListOptions->Add_Tail(*commands);
		pTextBtnCancel->Add_Tail(*commands);
		pTextBtnAcceptStart->Add_Tail(*commands);
		pTextBtnAction->Add_Tail(*commands);
		pILDisc->Add_Tail(*commands);
		pDropListHouse->Add_Tail(*commands);
//		pCheckAftermathUnits->Add_Tail(*commands);
	}
	else
	{
		pWO->pShpBtnDiscon->Zap();
		pWO->pShpBtnLeave->Zap();
		pWO->pShpBtnRefresh->Zap();
		pWO->pShpBtnSquelch->Zap();
		pWO->pShpBtnBan->Zap();
		pWO->pShpBtnKick->Zap();
		pWO->pShpBtnFindpage->Zap();
		pWO->pShpBtnOptions->Zap();
		pWO->pShpBtnLadder->Zap();
		pWO->pShpBtnHelp->Zap();
		pILPlayers->Zap();
		if( bHost )
		{
			pShpBtnScenarioRA->Zap();
			if( !pWO->GameInfoCurrent.bTournament )
			{
				pShpBtnScenarioUser->Zap();
			}
			if( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::CSGAME )
				pShpBtnScenarioCS->Zap();
			else if( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::AMGAME ||
					( pWO->GameInfoCurrent.GameKind == CREATEGAMEINFO::RAGAME && Is_Aftermath_Installed() && !pWO->GameInfoCurrent.bTournament ) )
				pShpBtnScenarioAM->Zap();
			pILScens->Zap();
		}
//		pStaticDescrip->Zap();
		pEditSend->Zap();
		pStaticUnit->Zap();
		pStaticLevel->Zap();
		pStaticCredits->Zap();
		pStaticAIPlayers->Zap();
		pGaugeCount->Zap();
		pGaugeCredits->Zap();
		pGaugeAIPlayers->Zap();
		pGaugeLevel->Zap();
		pCheckListOptions->Zap();
		pTextBtnCancel->Zap();
		pTextBtnAcceptStart->Zap();
		pTextBtnAction->Zap();
		pILDisc->Zap();
		pDropListHouse->Zap();
//		pCheckAftermathUnits->Zap();
	}
}

//***********************************************************************************************
void WOL_GameSetupDialog::ScenarioDisplayMode( SCENARIO_GAMEKIND ScenKind )
{
//	debugprint( "ScenarioDisplayMode, from %i going into %i mode.\n", ScenKindCurrent, ScenKind );

	//	Puts us into mode where we are viewing a particular gamekind of scenario list.
	if( ScenKindCurrent == ScenKind )
		return;

	//	Reorder tab buttons.
	BindControls( false );
	ScenKindCurrent = ScenKind;
	BindControls( true );

	//	Reset list items.
	pILScens->Clear();
//	debugprint( "  *  *  *  *  *  *  ScenKind %i has %i items.\n", ScenKind, ar_szScenarios[ ScenKind ].Count() );
	//	Check for the currently selected scenario.
	bool bFoundCurrentSelection = false;
	int iSelect;
	for( int i = 0; i != ar_szScenarios[ ScenKind ].Count(); i++ )
	{
		//	Put ScenarioIndex in as extradata to list item.
		int iScenIndex = (int)ar_szScenIndexes[ ScenKind ][ i ];
		int iIndexNew = pILScens->Add_Item( ar_szScenarios[ ScenKind ][ i ], NULL, NULL, ICON_DIB, NULL, (void*)iScenIndex );
		if( iScenIndex == Session.Options.ScenarioIndex && !bFoundCurrentSelection )
		{
			//	(Choose first line of what can be multiline description of currently selected scenario.)
			bFoundCurrentSelection = true;
			iSelect = i;
		}
	}
	//	If the current scenario selection is in this list, enable the list to show the selection, otherwise,
	//	make the listclass selection invisible, because it doesn't indicate the real scenario selection.
	//	Basically, problem is that I can't have no selection in ListClass, and I don't want to risk changing
	//	it as I'll affect the rest of the code. So I do this horrible hack.
	if( bFoundCurrentSelection )
	{
		pILScens->SetSelectType( 1 );		//	Regular selection.
		pILScens->Set_Selected_Index( iSelect );
	}
	else
		pILScens->SetSelectType( 0 );		//	Invisible selection.

//	display = REDRAW_ALL;
	pILScens->Flag_To_Redraw();
	pShpBtnScenarioRA->Flag_To_Redraw();
	pShpBtnScenarioCS->Flag_To_Redraw();
	pShpBtnScenarioAM->Flag_To_Redraw();
	pShpBtnScenarioUser->Flag_To_Redraw();
}

//***********************************************************************************************
bool WOL_GameSetupDialog::ExitGameChannel()
{
//	debugprint( "ExitGameChannel \n" );
	if( pDropListHouse->IsDropped )
	{
		pDropListHouse->Collapse();
		if (display < REDRAW_BACKGROUND) display = REDRAW_BACKGROUND;
	}
	if( !pWO->ChannelLeave() )
	{
		pWO->GenericErrorMessage();
		return false;
	}

	pWO->OnExitingGameChannel();

	return true;
}

//***********************************************************************************************
void WOL_GameSetupDialog::DrawScenarioDescripIcon( const char* pDIB ) const
{
	CC_Draw_DIB( pDIB, d_gamekind_x - 16, d_gamekind_y - 2, 100, WINDOW_MAIN );
}

//***********************************************************************************************
void WOL_GameSetupDialog::SetPlayerColor( const char* szName, PlayerColorType Color )
{
	//	Sets player color - does not verify if it is "okay" to do so.
//	debugprint( "SetPlayerColor %s to %i\n", szName, Color );

	int iItem = pILPlayers->Find( szName );
	if( iItem == -1 )
	{
		//	Player name was not found in list.
		//	This can happen when player color Informs arrive from the host before I have gotten a userlist.
		//	Insert an entry for user - color will be maintained when the userlist arrives.
		//	"early insertion"
//		debugprint( "SetPlayerColor could not find name '%s'! Inserting...\n", szName );
		iItem = pILPlayers->Add_Item( szName );
	}

	if( strcmp( pWO->szMyName, szName ) == 0 )
	{
		//	I am the player involved.
		Session.ColorIdx = Color;
		if( display < REDRAW_COLORS ) display = REDRAW_COLORS;
	}
	pILPlayers->Set_Item_Color( iItem, &ColorRemaps[ Color == PCOLOR_DIALOG_BLUE ? PCOLOR_REALLY_BLUE : Color ] );
	pILPlayers->Flag_To_Redraw();

}

//***********************************************************************************************
PlayerColorType WOL_GameSetupDialog::GetPlayerColor( const char* szName )
{
	//	Returns player color, if player is found in list, else PCOLOR_NONE.
	int iItem = pILPlayers->Find( szName );
	if( iItem == -1 )
	{
		//	Player name was not found in list.
		return PCOLOR_NONE;
	}
	RemapControlType* pRemap = pILPlayers->Get_Item_Color( iItem );
	return PlayerColorTypeOf( pRemap );
}

//***********************************************************************************************
void WOL_GameSetupDialog::SetPlayerHouse( const char* szName, HousesType House )
{
	//	Sets player house - does not verify if it is "okay" to do so.
//	debugprint( "SetPlayerHouse %s to %i\n", szName, House );

	int iItem = pILPlayers->Find( szName );
	if( iItem == -1 )
	{
		//	Player name was not found in list.
		//	This can happen when player house Informs arrive from the host before I have gotten a userlist.
		//	Insert an entry for user - house will be maintained when the userlist arrives.
		//	"early insertion"
//		debugprint( "SetPlayerHouse could not find name '%s'! Inserting...\n", szName );
		iItem = pILPlayers->Add_Item( szName );
	}

	//	Reset item text.
	char szItem[ 100 ];
	pWO->WritePlayerListItem( szItem, szName, House );
//	debugprint ( "%i, %s\n", iItem, szItem );
	pILPlayers->Set_Item( iItem, szItem );
	pILPlayers->Flag_To_Redraw();
}

//***********************************************************************************************
HousesType WOL_GameSetupDialog::GetPlayerHouse( const char* szName )
{
	//	Returns player house for user if found in list, else HOUSE_NONE.
	int iItem = pILPlayers->Find( szName );
	if( iItem == -1 )
	{
		//	Player name was not found in list.
		return HOUSE_NONE;
	}

	return pWO->PullPlayerHouse_From( pILPlayers->Get_Item( iItem ) );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::SetPlayerAccepted( const char* szName, bool bAccepted )
{
	//	Sets player's 'accepted' state to true or false.
	//	Value is stored in the player list: if there is an accepted icon, player has accepted.
	int iItem = pILPlayers->Find( szName );
	if( iItem == -1 )
	{
		//	Player name was not found in list.
//		debugprint( "SetPlayerAccepted() - could not find '%s'.\n", szName );
		return false;
	}
//	debugprint( "SetPlayerAccepted() - set '%s' to %s.\n", szName, bAccepted ? "accepted" : "NOT accepted" );
	return pWO->MarkItemAccepted( iItem, bAccepted );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::IveAccepted()
{
	//	Returns true if I am marked as "accepted".
	int iItem = pILPlayers->Find( pWO->szMyName );
	if( iItem == -1 )
		return false;
	return pWO->bItemMarkedAccepted( iItem );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::SetPlayerReadyToGo( const char* szName, const char* szReadyState )
{
	//	Sets player's 'ready to go' state.
	//	Set szReadyState to "ready", "need scenario", or NULL.
	//	First two cases are regarded as player being ready to go.
	//	Value is stored in the player list in the hidden string field.
	int iItem = pILPlayers->Find( szName );
	if( iItem == -1 )
	{
		//	Player name was not found in list.
//		debugprint( "SetPlayerReadyToGo() - could not find '%s'.\n", szName );
		return false;
	}
//	debugprint( "SetPlayerReadyToGo() - set '%s' to %s.\n", szName, szReadyState );
	pWO->MarkItemReadyToGo( iItem, szReadyState );

	return true;
}

//***********************************************************************************************
void WOL_GameSetupDialog::ResetReadyToGo()
{
	//	Resets all users to "not ready to go".
	for( int i = 0; i < pILPlayers->Count(); i++ )
	{
		pWO->MarkItemReadyToGo( i, NULL );
	}
}

//***********************************************************************************************
bool WOL_GameSetupDialog::bPlayerReadyToGo( const char* szName )
{
	//	Returns true if player is marked as "ready to go".
	int iItem = pILPlayers->Find( szName );
	if( iItem == -1 )
		return false;
	return pWO->bItemMarkedReadyToGo( iItem );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::bAllPlayersReadyToGo()
{
	//	Returns true if all players are marked as "ready to go".
//	debugprint( "Checking for all players ready - there are %i\n", pILPlayers->Count() );
	for( int i = 0; i < pILPlayers->Count(); i++ )
	{
		if( !pWO->bItemMarkedReadyToGo( i ) )
		{
//			debugprint( "Item %i NOT ready\n", i );
			return false;
		}
//		debugprint( "Item %i is ready\n", i );
	}
//	debugprint( "Return true\n" );
	return true;
}

//***********************************************************************************************
void WOL_GameSetupDialog::ProcessGuestRequest( User* pUser, const char* szRequest )
{
	//	Game host processes a request that arrived as a privategameopt from one of the guests.
	//	WOL_GAMEOPT_REQCOLOR format:
	//	2		WOL_GAMEOPT
	//	1		space
	//	2		color
	//	1		null-terminator
//	debugprint( "ProcessGuestRequest. szRequest is '%s', len %i.\n", szRequest, strlen( szRequest ) );
	WOL_GAMEOPT opt = (WOL_GAMEOPT)atoi( szRequest );
	szRequest += 3;

	switch( opt )
	{
	case WOL_GAMEOPT_REQCOLOR:
	{
		PlayerColorType ColorDesired = (PlayerColorType)( atoi( szRequest ) );
		if( pILPlayers->FindColor( &ColorRemaps[ ColorDesired == PCOLOR_DIALOG_BLUE ? PCOLOR_REALLY_BLUE : ColorDesired ] ) == -1 )
		{
			//	Color is available.
			SetPlayerColor( (char*)pUser->name, ColorDesired );
			//	Tell all guests about the color change.
			InformAboutPlayerColor( (char*)pUser->name, ColorDesired, NULL );
		}
		else
		{
			//	Color is not available.
//			debugprint( "Color %i denied to %s\n", ColorDesired, (char*)pUser->name );
			//	Tell requestor that his color is still the same.
			RemapControlType* pColorRemapCurrent = pILPlayers->Get_Item_Color( pILPlayers->Find( (char*)pUser->name ) );
			InformAboutPlayerColor( (char*)pUser->name, PlayerColorTypeOf( pColorRemapCurrent ), pUser );
		}
		break;
	}
	case WOL_GAMEOPT_REQHOUSE:
	{
		HousesType HouseChoice = (HousesType)( atoi( szRequest ) );
//		debugprint( "Host received: '%s' changed house to %u.\n", (char*)pUser->name, HouseChoice );
		SetPlayerHouse( (char*)pUser->name, HouseChoice );
		nHostLastParamID++;
		InformAboutPlayerHouse( (char*)pUser->name, HouseChoice, NULL );
		ClearAllAccepts();
		break;
	}
	case WOL_GAMEOPT_REQACCEPT:
		//	Does Param ID of accept request match the last param change ID sent? See notes at top.
		if( atoi( szRequest ) == nHostLastParamID )
		{
//			debugprint( "Host received valid accept from '%s'.\n", (char*)pUser->name );
			SetPlayerAccepted( (char*)pUser->name, true );
			InformAboutPlayerAccept( (char*)pUser->name, NULL );
			//	We may be ready to start a game now.
			if( bAllGuestsAccept() )
			{
				if( pToolTipHitLast && pToolTipHitLast->bShowing )
					pToolTipHitLast->Unshow();
				pTextBtnAcceptStart->Enable();
			}
		}
		else
		{
//			debugprint( "______________Host received invalid accept from '%s'. ID = %i when it should be %i.\n", (char*)pUser->name,
//				atoi( szRequest ), nHostLastParamID );
		}
		break;
	case WOL_GAMEOPT_REQSTART:
		//	Does Param ID of accept request match the last param change ID sent? See notes at top.
		if( atoi( szRequest ) == nHostLastParamID )		//	Otherwise ignore - it's old and we don't care. (Incredibly unlikely to happen, actually.)
		{
//			debugprint( "Host received valid WOL_GAMEOPT_REQSTART from '%s'.\n", (char*)pUser->name );
//			WOL_PrintMessage( *pILDisc, "WOL_GAMEOPT_REQSTART response", WOLCOLORREMAP_LOCALMACHINEMESS );
//			WOL_PrintMessage( *pILDisc, (char*)pUser->name, WOLCOLORREMAP_LOCALMACHINEMESS );
			if( bWaitingToStart )
				//	If all responses are in, start the game!
				GuestIsReadyToPlay( (char*)pUser->name, "ready" );
//			else
//				debugprint( "Ignoring - I am no longer waiting to start a game.\n" );
		}
		break;
	case WOL_GAMEOPT_REQSTART_BUTNEEDSCENARIO:
		//	Does Param ID of accept request match the last param change ID sent? See notes at top.
		if( atoi( szRequest ) == nHostLastParamID )		//	Otherwise ignore - it's old and we don't care. (Incredibly unlikely to happen, actually.)
		{
//			debugprint( "Host received valid WOL_GAMEOPT_REQSTART_BUTNEEDSCENARIO from '%s'.\n", (char*)pUser->name );
			if( bWaitingToStart )
				//	If all responses are in, start the game!
				GuestIsReadyToPlay( (char*)pUser->name, "need scenario" );
//			else
//				debugprint( "Ignoring - I am no longer waiting to start a game.\n" );
		}
		break;
	case WOL_GAMEOPT_INFGO:
		//	I have told myself to start game right now.
		//	This is the new method. Avoids apparent "private messages/gameopts are getting delayed" problem in chatserver...
		//	(I don't want to end up leaving the channel before guests get my go message.)
		strcpy( szTriggerGameStartInfo, szRequest );
		break;
	default:
//		debugprint( "Unhandled value of %i in ProcessGuestRequest!!!\n", opt );
		break;
	}
}

//***********************************************************************************************
void WOL_GameSetupDialog::ProcessInform( char* szInform )
{
	//	Process inform message arriving from game host.
//	debugprint( "ProcessInform: '%s'\n", szInform );
	if( !bHost )
	{
		WOL_GAMEOPT opt = (WOL_GAMEOPT)atoi( szInform );
		szInform += 3;
		switch( opt )
		{
		case WOL_GAMEOPT_INFCOLOR:
		{
			//	WOL_GAMEOPT_INFCOLOR format:
			//	2		WOL_GAMEOPT
			//	1		space
			//	2		color
			//	1		space
			//	string	name of player
			PlayerColorType Color = (PlayerColorType)( atoi( szInform ) );
			szInform += 3;
			SetPlayerColor( szInform, Color );		//	(szInform is now sitting at the start of the name string.)
			Sound_Effect(VOC_OPTIONS_CHANGED);
			break;
		}
		case WOL_GAMEOPT_INFHOUSE:		//	Note: In theory, I could ignore this if it refers to me. I've already set my own house.
		{
			nGuestLastParamID = atoi( szInform );
			szInform += 7;
			HousesType House = (HousesType)( atoi( szInform ) );
			szInform += 3;
			SetPlayerHouse( szInform, House );		//	(szInform is now sitting at the start of the name string.)
			ClearAllAccepts();
			Sound_Effect(VOC_OPTIONS_CHANGED);
			break;
		}
		case WOL_GAMEOPT_INFACCEPT:		//	Note: In theory, I could ignore this if it refers to me.
			//	A guest has accepted.
			SetPlayerAccepted( szInform, true );		//	(szInform is now sitting at the start of the name string.)
			break;
		case WOL_GAMEOPT_INFPARAMS:
			//	Game params have changed.
			bParamsReceived = true;
			if( !AcceptParams( szInform ) )
				bLeaveDueToRulesMismatchTrigger = true;
			SetSpecialControlStates();
			//pILScens->Set_Selected_Index( Session.Options.ScenarioIndex );
			display = REDRAW_ALL;
			ClearAllAccepts();
			Sound_Effect(VOC_OPTIONS_CHANGED);
			break;
		case WOL_GAMEOPT_INFNEWGUESTPLAYERINFO:
			//	I have just joined and have received a message with info on all players in game.
			AcceptNewGuestPlayerInfo( szInform );
			Sound_Effect(VOC_OPTIONS_CHANGED);
			break;
		case WOL_GAMEOPT_INFSTART:
		{
			//	Host tells us to wait for start of game.
//			debugprint( "Guest received WOL_GAMEOPT_INFSTART.\n" );
			nGuestLastParamID = atoi( szInform );
//	The following check is not necessary. Rules.ini, if manually replaced by a cheater, is not reloaded.
//	So prior checks (that occur on game params receives) are sufficient.
//			szInform += 7;
//			//	Check rules.ini compatibility.
//			int iRulesID = atoi( szInform );
//			if( RuleINI.Get_Unique_ID() != iRulesID )
//			{
//				//	Rules.ini incompatible. Don't respond to call for start.
//				bLeaveDueToRulesMismatchTrigger = true;
//				break;
//			}
			User* pUserHost = pWO->pGameHost();
			if( pUserHost )		//	This better'd be true.
			{
				//	Send ack back to host.
				char szSend[ 20 ];
				//	Make sure we have the scenario.
				if( !bNeedScenarioDownload() )
					sprintf( szSend, "%02u %06u", WOL_GAMEOPT_REQSTART, nGuestLastParamID );
				else
					sprintf( szSend, "%02u %06u", WOL_GAMEOPT_REQSTART_BUTNEEDSCENARIO, nGuestLastParamID );
				pWO->SendGameOpt( szSend, pUserHost );
				//	Enter waiting mode.
				bWaitingToStart = true;
				WWMessageBox().Process( TXT_WOL_WAITINGTOSTART, TXT_NONE );
				BindControls( false );
				//	If we are in a modal dialog, we must have arrived here through Call_Back()'s PumpMessages.
				//	Set global that will force edit dialogs to stop accepting characters.
				//	This is to fix a minor glitch: guests can keep typing into a "page" dialog editbox after the
				//	"Launching game..." message has appeared on top of it.
				if( pWO->bPump_In_Call_Back )
					disable_current_msgbox = true;
			}
			else
			{
//				debugprint( "Impossible arose on WOL_GAMEOPT_INFSTART.\n" );
				Fatal( "Impossible arose on WOL_GAMEOPT_INFSTART.\n" );
			}
			Sound_Effect( VOC_GAME_CLOSED );
			break;
		}
		case WOL_GAMEOPT_INFCANCELSTART:
			//	Host tells us to cancel waiting for start of game.
			bWaitingToStart = false;
			ClearAllAccepts();
			BindControls( true );
			WOL_PrintMessage( *pILDisc, TXT_WOL_STARTCANCELLED, WOLCOLORREMAP_LOCALMACHINEMESS );
			Sound_Effect( VOC_SYS_ERROR );
			display = REDRAW_ALL;
			//	If we are in a modal dialog, we must have arrived here through Call_Back()'s PumpMessages. Set global that will
			//	force a cancel out of the dialog.
			if( pWO->bPump_In_Call_Back )
				cancel_current_msgbox = true;
			break;
		case WOL_GAMEOPT_INFGO:
			//	Host says start game right now.
			strcpy( szTriggerGameStartInfo, szInform );
			//	If we are in a modal dialog, we must have arrived here through Call_Back()'s PumpMessages. Set global that will
			//	force a cancel out of the dialog.
			if( pWO->bPump_In_Call_Back )
				cancel_current_msgbox = true;
			break;
		default:
//			debugprint( "Unhandled value of %i in ProcessInform!!!\n", opt );
			//WOL_PrintMessage( *pILDisc, "Error - Unhandled value in ProcessInform!!!", WOLCOLORREMAP_LOCALMACHINEMESS );
			Fatal( "Error - Unhandled value in ProcessInform!" );
			break;
		}
	}
//	debugprint( "* END of ProcessInform: '%s'\n", szInform );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::bParamsUnfresh()
{
	//	Returns true if game setup parameters do not match what they were last time they were sent.
	GAMEPARAMS GParamsNow;
	SetGParamsToCurrent( GParamsNow );

//	if( !( GParamsNow == GParamsLastSent ) )
//	{
//		debugprint( "---- Params Unfresh: OLD...\n" );
//		Debug_GlobalPacketType( GParamsLastSent.GPacket );
//		debugprint( "-------------------  NEW...\n" );
//		Debug_GlobalPacketType( GParamsNow.GPacket );
//		debugprint( "old bAftermathUnits = %i\n", GParamsLastSent.bAftermathUnits );
//		debugprint( "new bAftermathUnits = %i\n", GParamsNow.bAftermathUnits );
//	}

	return !( GParamsNow == GParamsLastSent );
}

//***********************************************************************************************
void WOL_GameSetupDialog::SendParams()
{
	//	Host only.

	SetGParamsToCurrent( GParamsLastSent );

	char szSend[ 130 + DESCRIP_MAX + 12 + 32 + 100 ];
	sprintf( szSend,
		"%01u "
		"%06u "
		"%03u "
		"%s "
		"%u "
		"%s "
		"%01u "
		"%.32s "	//	No null-terminator on digest. There may be nothing at all inserted here.
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		"%u "
		,
		WOL_GAMEOPT_INFPARAMS,
		nHostLastParamID,
		strlen( GParamsLastSent.GPacket.ScenarioInfo.Scenario ),
		GParamsLastSent.GPacket.ScenarioInfo.Scenario,
		GParamsLastSent.GPacket.ScenarioInfo.FileLength,
		GParamsLastSent.GPacket.ScenarioInfo.ShortFileName,
//		strlen( (char*)GParamsLastSent.GPacket.ScenarioInfo.FileDigest ),		not null-terminated!
		GParamsLastSent.GPacket.ScenarioInfo.FileDigest[0] ? 1 : 0,
		GParamsLastSent.GPacket.ScenarioInfo.FileDigest,
		GParamsLastSent.GPacket.ScenarioInfo.OfficialScenario,
		GParamsLastSent.GPacket.ScenarioInfo.Credits,
		GParamsLastSent.GPacket.ScenarioInfo.IsBases,
		GParamsLastSent.GPacket.ScenarioInfo.IsTiberium,
		GParamsLastSent.GPacket.ScenarioInfo.IsGoodies,
		GParamsLastSent.GPacket.ScenarioInfo.BuildLevel,
		GParamsLastSent.GPacket.ScenarioInfo.UnitCount,
		GParamsLastSent.GPacket.ScenarioInfo.AIPlayers,
		GParamsLastSent.GPacket.ScenarioInfo.Seed,
		GParamsLastSent.GPacket.ScenarioInfo.Special.IsShadowGrow,
		GParamsLastSent.GPacket.ScenarioInfo.Special.IsSpeedBuild,
		GParamsLastSent.GPacket.ScenarioInfo.Special.IsFromInstall,
		GParamsLastSent.GPacket.ScenarioInfo.Special.IsCaptureTheFlag,
		GParamsLastSent.GPacket.ScenarioInfo.Special.IsInert,
		GParamsLastSent.GPacket.ScenarioInfo.Special.IsThreePoint,
		GParamsLastSent.GPacket.ScenarioInfo.Special.IsTGrowth,
		GParamsLastSent.GPacket.ScenarioInfo.Special.IsTSpread,
		GParamsLastSent.GPacket.ScenarioInfo.GameSpeed,
		GParamsLastSent.GPacket.ScenarioInfo.Version,
		GParamsLastSent.bAftermathUnits,			//	Not currently used.
		GParamsLastSent.bSlowUnitBuildRate,
		RuleINI.Get_Unique_ID()		//	Used to verify rules.ini files match.
		);

	pWO->SendGameOpt( szSend, NULL );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::AcceptParams( char* szParams )
{
	//	Reverse of SendParams() process. szParams has already been stripped of 2 bytes header. Guest only.

	//	Returns false if rules.ini doesn't match that of the host.
	//	(Or if an error occurs due to the packet being incorrect - which happened once in test...)

	char szDelimiter[] = " ";
	char* szToken;
	char* szRemaining;

	szToken = strtok( szParams, szDelimiter );
	nGuestLastParamID = atoi( szToken );

	//	Read in length of following string.
	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	int iLen = atoi( szToken );
	//	Set string pointer to start of string (length is 3 digits).
	szRemaining = szToken + 4;
	//	Read in string.
	memcpy( Session.Options.ScenarioDescription, szRemaining, iLen );
	//	Null-terminate.
	Session.Options.ScenarioDescription[ iLen ] = 0;
	//	Advance string pointer to next param.
	szRemaining += iLen + 1;

//debugprint( "scenario description is '%s'\n", Session.Options.ScenarioDescription );
//debugprint( "remaining: '%s'\n", szRemaining );

	szToken = strtok( szRemaining, szDelimiter );
	if( !szToken )	return false;
	Session.ScenarioFileLength = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	strcpy( Session.ScenarioFileName, szToken );

//	//	Read in length of following string.
//	szToken = strtok( NULL, szDelimiter );
//	iLen = atoi( szToken );
//	//	Set string pointer to start of string (length is 3 digits).
//	szRemaining = szToken + 4;
	//	Method changed.
	//	Check if there is a digest.
	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	iLen = atoi( szToken );		//	1 or 0, indicating if there is a digest following.
	if( iLen )
	{
//		//	Set string pointer to start of string (previous field is 1 digit).
//		szRemaining = szToken + 2;
//		iLen = sizeof( Session.ScenarioDigest );
//		//	Read in string.
//		memcpy( Session.ScenarioDigest, szRemaining, iLen );
//		//	//	Null-terminate.
//		//	Session.ScenarioDigest[ iLen ] = 0;			Digest has no null-terminator!
//		//	Advance string pointer to next param.
//		szRemaining += iLen + 1;
		//	There is a digest.
		szToken = strtok( NULL, szDelimiter );			//	(Digests can't have spaces in the them.)
//debugprint( "digest: '%s'\n", szToken );
		if( !szToken )	return false;
		strncpy( Session.ScenarioDigest, szToken, sizeof( Session.ScenarioDigest ) );
	}

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Session.ScenarioIsOfficial = (bool)atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Session.Options.Credits = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Session.Options.Bases = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Session.Options.Tiberium = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Session.Options.Goodies = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	BuildLevel = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Session.Options.UnitCount = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Session.Options.AIPlayers = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Seed = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Special.IsShadowGrow = ( atoi( szToken ) == 0 ) ? 0 : 1;

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Special.IsSpeedBuild = ( atoi( szToken ) == 0 ) ? 0 : 1;

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Special.IsFromInstall = ( atoi( szToken ) == 0 ) ? 0 : 1;

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Special.IsCaptureTheFlag = ( atoi( szToken ) == 0 ) ? 0 : 1;

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Special.IsInert = ( atoi( szToken ) == 0 ) ? 0 : 1;

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Special.IsThreePoint = ( atoi( szToken ) == 0 ) ? 0 : 1;

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Special.IsTGrowth = ( atoi( szToken ) == 0 ) ? 0 : 1;

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Special.IsTSpread = ( atoi( szToken ) == 0 ) ? 0 : 1;

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	Options.GameSpeed = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
//	"Version"	= atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	bAftermathUnits = (bool)atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	bSlowUnitBuildRate = (bool)atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
	if( !szToken )	return false;
	int iRulesID = atoi( szToken );

	szToken = strtok( NULL, szDelimiter );
//	if( szToken )
//		debugprint( "szToken should be NULL!!!!!!!!\n" );

	return ( RuleINI.Get_Unique_ID() == iRulesID );
}

//***********************************************************************************************
void WOL_GameSetupDialog::SetGParamsToCurrent( GAMEPARAMS& GParams )
{
	//	Sets values in a GAMEPARAMS to the current game settings.

	strcpy( GParams.GPacket.ScenarioInfo.Scenario, Session.Scenarios[ Session.Options.ScenarioIndex ]->Description() );
	CCFileClass file( Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Filename() );
	GParams.GPacket.ScenarioInfo.FileLength = file.Size();
	strcpy( GParams.GPacket.ScenarioInfo.ShortFileName, Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Filename() );
	//	Digest is not null-terminated.
	strncpy( (char*)GParams.GPacket.ScenarioInfo.FileDigest, Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Digest(), sizeof( GParams.GPacket.ScenarioInfo.FileDigest ) );
	GParams.GPacket.ScenarioInfo.OfficialScenario = Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Official();
	GParams.GPacket.ScenarioInfo.Credits = Session.Options.Credits;
	GParams.GPacket.ScenarioInfo.IsBases = Session.Options.Bases;
	GParams.GPacket.ScenarioInfo.IsTiberium = Session.Options.Tiberium;
	GParams.GPacket.ScenarioInfo.IsGoodies = Session.Options.Goodies;
	GParams.GPacket.ScenarioInfo.BuildLevel = BuildLevel;
	GParams.GPacket.ScenarioInfo.UnitCount = Session.Options.UnitCount;
	GParams.GPacket.ScenarioInfo.AIPlayers = Session.Options.AIPlayers;
	GParams.GPacket.ScenarioInfo.Seed = Seed;
	GParams.GPacket.ScenarioInfo.Special = Special;
	GParams.GPacket.ScenarioInfo.GameSpeed = Options.GameSpeed;
	GParams.GPacket.ScenarioInfo.Version = VerNum.Get_Clipped_Version();

	GParams.bAftermathUnits = bAftermathUnits;
	GParams.bSlowUnitBuildRate = bSlowUnitBuildRate;
}

//***********************************************************************************************
bool operator==( const GAMEPARAMS& gp1, const GAMEPARAMS& gp2 )
{
	return	gp1.GPacket == gp2.GPacket &&
			gp1.bAftermathUnits == gp2.bAftermathUnits && gp1.bSlowUnitBuildRate == gp2.bSlowUnitBuildRate;
}

//***********************************************************************************************
bool operator==( const GlobalPacketType& gp1, const GlobalPacketType& gp2 )
{
	if( strcmp( gp1.ScenarioInfo.Scenario, gp2.ScenarioInfo.Scenario ) != 0	)		return false;
	if( strcmp( gp1.ScenarioInfo.ShortFileName, gp2.ScenarioInfo.ShortFileName ) != 0 )		return false;
	//	Digest is not null-terminated...
	if( strncmp( (const char*)gp1.ScenarioInfo.FileDigest, (const char*)gp2.ScenarioInfo.FileDigest, sizeof( gp1.ScenarioInfo.FileDigest ) ) != 0 )		return false;

	if(	gp1.ScenarioInfo.FileLength == gp2.ScenarioInfo.FileLength &&
		gp1.ScenarioInfo.OfficialScenario == gp2.ScenarioInfo.OfficialScenario &&
		gp1.ScenarioInfo.Credits == gp2.ScenarioInfo.Credits &&
		gp1.ScenarioInfo.IsBases == gp2.ScenarioInfo.IsBases &&
		gp1.ScenarioInfo.IsTiberium == gp2.ScenarioInfo.IsTiberium &&
		gp1.ScenarioInfo.IsGoodies == gp2.ScenarioInfo.IsGoodies &&
		gp1.ScenarioInfo.BuildLevel == gp2.ScenarioInfo.BuildLevel &&
		gp1.ScenarioInfo.UnitCount == gp2.ScenarioInfo.UnitCount &&
		gp1.ScenarioInfo.AIPlayers == gp2.ScenarioInfo.AIPlayers &&
		gp1.ScenarioInfo.Seed == gp2.ScenarioInfo.Seed &&
		gp1.ScenarioInfo.Special.IsShadowGrow == gp2.ScenarioInfo.Special.IsShadowGrow &&
		gp1.ScenarioInfo.Special.IsSpeedBuild == gp2.ScenarioInfo.Special.IsSpeedBuild &&
		gp1.ScenarioInfo.Special.IsFromInstall == gp2.ScenarioInfo.Special.IsFromInstall &&
		gp1.ScenarioInfo.Special.IsCaptureTheFlag == gp2.ScenarioInfo.Special.IsCaptureTheFlag &&
		gp1.ScenarioInfo.Special.IsInert == gp2.ScenarioInfo.Special.IsInert &&
		gp1.ScenarioInfo.Special.IsThreePoint == gp2.ScenarioInfo.Special.IsThreePoint &&
		gp1.ScenarioInfo.Special.IsTGrowth == gp2.ScenarioInfo.Special.IsTGrowth &&
		gp1.ScenarioInfo.Special.IsTSpread == gp2.ScenarioInfo.Special.IsTSpread &&
		gp1.ScenarioInfo.GameSpeed == gp2.ScenarioInfo.GameSpeed &&
		gp1.ScenarioInfo.Version == gp2.ScenarioInfo.Version )
		return true;

	return false;
}

//***********************************************************************************************
/*
void Debug_GlobalPacketType( const GlobalPacketType& gp1 )			//	ajw  debugging
{
	if( *gp1.ScenarioInfo.Scenario )
//		debugprint( "Scenario = %s\n", (char*)gp1.ScenarioInfo.Scenario );
	else
//		debugprint( "!Scenario string is null\n" );
	if( *gp1.ScenarioInfo.ShortFileName )
//		debugprint( "ShortFileName = %s\n", (char*)gp1.ScenarioInfo.ShortFileName );
	else
//		debugprint( "!ShortFileName string is null\n" );
	//	Remember ShortFileName is not null-terminated...
	if( *gp1.ScenarioInfo.FileDigest )
//		debugprint( "FileDigest = %.32s\n", (char*)gp1.ScenarioInfo.FileDigest );
	else
//		debugprint( "!FileDigest string is null\n" );

//	debugprint(	"FileLength = %i\n"
				"OfficialScenario = %i\n"
				"Credits = %i\n"
				"IsBases = %i\n"
				"IsTiberium = %i\n"
				"IsGoodies = %i\n"
				"BuildLevel = %i\n"
				"UnitCount = %i\n"
				"AIPlayers = %i\n"
				"Seed = %i\n"
				"Special.IsShadowGrow = %i\n"
				"Special.IsSpeedBuild = %i\n"
				"Special.IsFromInstall = %i\n"
				"Special.IsCaptureTheFlag = %i\n"
				"Special.IsInert = %i\n"
				"Special.IsThreePoint = %i\n"
				"Special.IsTGrowth = %i\n"
				"Special.IsTSpread = %i\n"
				"GameSpeed = %i\n"
				"Version = %i\n",
				gp1.ScenarioInfo.FileLength,
				gp1.ScenarioInfo.OfficialScenario,
				gp1.ScenarioInfo.Credits,
				gp1.ScenarioInfo.IsBases,
				gp1.ScenarioInfo.IsTiberium,
				gp1.ScenarioInfo.IsGoodies,
				gp1.ScenarioInfo.BuildLevel,
				gp1.ScenarioInfo.UnitCount,
				gp1.ScenarioInfo.AIPlayers,
				gp1.ScenarioInfo.Seed,
				gp1.ScenarioInfo.Special.IsShadowGrow,
				gp1.ScenarioInfo.Special.IsSpeedBuild,
				gp1.ScenarioInfo.Special.IsFromInstall,
				gp1.ScenarioInfo.Special.IsCaptureTheFlag,
				gp1.ScenarioInfo.Special.IsInert,
				gp1.ScenarioInfo.Special.IsThreePoint,
				gp1.ScenarioInfo.Special.IsTGrowth,
				gp1.ScenarioInfo.Special.IsTSpread,
				gp1.ScenarioInfo.GameSpeed,
				gp1.ScenarioInfo.Version );
}
*/

//***********************************************************************************************
PlayerColorType PlayerColorTypeOf( RemapControlType* pColorRemap )
{
	for( PlayerColorType pcolor = PCOLOR_FIRST; pcolor < PCOLOR_COUNT; pcolor++ )
	{
		if( &ColorRemaps[ pcolor ] == pColorRemap )
			return pcolor;
	}
	return PCOLOR_NONE;
}

//***********************************************************************************************
bool WOL_GameSetupDialog::RequestPlayerColor( PlayerColorType Color )
{
	//	Local player sends a request to the game host asking for a particular color.
	char szSend[ 20 ];

	//	WOL_GAMEOPT_REQCOLOR format:
	//	2	WOL_GAMEOPT
	//	1	space
	//	2	color
	sprintf( szSend, "%02u %02u", WOL_GAMEOPT_REQCOLOR, Color );

	_ASSERTE( pWO->pGameHost() );

	return pWO->SendGameOpt( szSend, pWO->pGameHost() );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::InformAboutPlayerColor( const char* szName, PlayerColorType Color, User* pUserPriv )
{
	//	Game host tells guests about a player color assignment.
	//	If pUserPriv is not null, indicates user to send message as private to.
	char szSend[ 20 + WOL_NAME_LEN_MAX ];

	//	WOL_GAMEOPT_INFCOLOR format:
	//	2		WOL_GAMEOPT
	//	1		space
	//	2		color
	//	1		space
	//	string	name of player

	if( Color == PCOLOR_NONE )
	{
//		debugprint( "Bad Color for %s in InformAboutPlayerColor.\n", szName );
		*szSend = 0;
	}
	else
		sprintf( szSend, "%02u %02u %s", WOL_GAMEOPT_INFCOLOR, Color, szName );

	return pWO->SendGameOpt( szSend, pUserPriv );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::InformAboutPlayerHouse( const char* szName, HousesType House, User* pUserPriv )
{
	//	Game host tells guests about a player house assignment.
	//	If pUserPriv is not null, indicates user to send message as private to.
	char szSend[ 28 + WOL_NAME_LEN_MAX ];
	//	WOL_GAMEOPT_INFHOUSE format:
	//	2		WOL_GAMEOPT
	//	1		space
	//	6		param ID
	//	1		space
	//	2		house
	//	1		space
	//	string	name of player

	if( House == HOUSE_NONE )
	{
//		debugprint( "Bad House for %s in InformAboutPlayerHouse.\n", szName );
		*szSend = 0;
	}
	else
		sprintf( szSend, "%02u %06u %02u %s", WOL_GAMEOPT_INFHOUSE, nHostLastParamID, (short)House, szName );

	return pWO->SendGameOpt( szSend, pUserPriv );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::InformAboutPlayerAccept( const char* szName, User* pUserPriv )
{
	//	Game host tells guests about player accepting game params.
	//	If pUserPriv is not null, indicates user to send message as private to.
	char szSend[ 6 + WOL_NAME_LEN_MAX ];
	sprintf( szSend, "%02u %s", WOL_GAMEOPT_INFACCEPT, szName );

	return pWO->SendGameOpt( szSend, pUserPriv );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::InformAboutStart()
{
	//	Game host tells all guests that he wants to start the game.
	//	Note that nHostLastParamID is involved here. We want to make sure that guest responses apply
	//	to the latest WOL_GAMEOPT_INFSTART, and not to an earlier one we canceled out of.
	char szSend[ 10 ];

	sprintf( szSend, "%02u %06u", WOL_GAMEOPT_INFSTART, nHostLastParamID );

	return pWO->SendGameOpt( szSend, NULL );
}

//***********************************************************************************************
bool WOL_GameSetupDialog::InformAboutCancelStart()
{
	//	Game host tells all guests that he wants to start the game.
	//	Note that nHostLastParamID is involved here. We want to make sure that guest responses apply
	//	to the latest WOL_GAMEOPT_INFSTART, and not to an earlier one we canceled out of.
//	debugprint( "InformAboutCancelStart!\n" );

	char szSend[ 10 ];

	sprintf( szSend, "%02u", WOL_GAMEOPT_INFCANCELSTART );

	return pWO->SendGameOpt( szSend, NULL );
}

//***********************************************************************************************
void WOL_GameSetupDialog::OnGuestJoin( User* pUser )
{
	//	A guest (not myself) has entered the game channel.
//	debugprint( "OnGuestJoin()\n" );
	char* szPrint = new char[ strlen( TXT_WOL_PLAYERJOINEDGAME ) + strlen( (char*)pUser->name ) + 5 ];
	sprintf( szPrint, TXT_WOL_PLAYERJOINEDGAME, (char*)pUser->name );
	WOL_PrintMessage( *pILDisc, szPrint, WOLCOLORREMAP_LOCALMACHINEMESS );
	delete [] szPrint;

	ClearAllAccepts();

	if( bHost )
	{
		//	Send the new guest the current setup.	Note that nHostLastParamID doesn't change here.

		//	Assign color to new guest.
		PlayerColorType Color = ColorNextAvailable();
		SetPlayerColor( (char*)pUser->name, Color );

		//	Previously, I was sending an individual color, house, and acceptedstate message for each other guest.
		//	Though convenient code-wise, this causes the initial info to arrive at the new guest in a very slow
		//	manner. This is because of the wonderful "anti-flood" feature of the chat server, which prevents a
		//	series of messages from a client from being passed on faster than a certain rate.
		//	For this reason, a new message that contains all of the info about all of the other guests has been
		//	created (WOL_GAMEOPT_INFNEWGUESTPLAYERINFO).

		//	WOL_GAMEOPT_INFNEWGUESTPLAYERINFO format (items separated by spaces):
		//		WOL_GAMEOPT
		//		number of players
		//		for each player {
		//			length of player name string
		//			player name
		//			color
		//			bool - is there a house field following?
		//			(house)
		//			// (removed) acceptedness - true for set player accepted, false for do nothing
		//		}

		//	Build up a big WOL_GAMEOPT_INFNEWGUESTPLAYERINFO message.
		char szSend[ 500 ];
		sprintf( szSend, "%02u %02u", WOL_GAMEOPT_INFNEWGUESTPLAYERINFO, pILPlayers->Count() );
		//	Send color and house of all players (including himself) to the new guest.
		for( int i = 0; i < pILPlayers->Count(); i++ )
		{
			char szSendPiece[ 100 ];
			char szPlayerName[ WOL_NAME_LEN_MAX ];
			pWO->PullPlayerName_Into_From( szPlayerName, pILPlayers->Get_Item( i ) );
//			InformAboutPlayerColor( szPlayerName, PlayerColorTypeOf( pILPlayers->Get_Item_Color( i ) ), pUser );
			sprintf( szSendPiece, " %02u %s %02u", strlen( szPlayerName ), szPlayerName,
							PlayerColorTypeOf( pILPlayers->Get_Item_Color( i ) ) );

			if( strcmp( szPlayerName, (char*)pUser->name ) != 0 )
			{
				HousesType House = pWO->PullPlayerHouse_From( pILPlayers->Get_Item( i ) );
				if( House != HOUSE_NONE )
				{
	//				InformAboutPlayerHouse( szPlayerName, House, pUser );
					char szSendHouse[ 50 ];
					sprintf( szSendHouse, " 1 %02u", (short)House );
					strcat( szSendPiece, szSendHouse );
				}
				else
				{
					//	Player must not have told me what house he is yet. Don't send house value.
					strcat( szSendPiece, " 0" );
				}
			}
			else
			{
				//	Player is the new guest himself. Don't send house value.
					strcat( szSendPiece, " 0" );
			}

//	Acceptedness must be false! No need to send.
//			//	Send "accepted" status of player. Ignore myself, as I'm the host.
//			if( strcmp( szPlayerName, pWO->szMyName ) != 0 && pWO->bItemMarkedAccepted( i ) )
//				strcat( szSendPiece, " 1" );
//			else
//				strcat( szSendPiece, " 0" );

			strcat( szSend, szSendPiece );
		}
		pWO->SendGameOpt( szSend, pUser );

		//	Send everyone the color of the new guest.
		InformAboutPlayerColor( (char*)pUser->name, Color, NULL );

		//	Send game params.
		//	This is done last because it contains a param ID value, and we need to ensure that the new guest has
		//	received everything we are sending him here before he tries to send me an accept.
		//	If game params were sent first, he could theoretically receive them, then send an accept, even though he
		//	has not received the WOL_GAMEOPT_INFNEWGUESTPLAYERINFO.
		//	By doing this I avoid having to have a param ID in WOL_GAMEOPT_INFNEWGUESTPLAYERINFO, which would be hard,
		//	since it is a private message.
		//	For simplicity, send public.
		SendParams();
	}
}

//***********************************************************************************************
void WOL_GameSetupDialog::AcceptNewGuestPlayerInfo( char* szMsg )
{
	//	Process a received WOL_GAMEOPT_INFNEWGUESTPLAYERINFO message.
	//	szMsg has already been stripped of 2 bytes header.
	char szDelimiter[] = " ";
	char* szToken;
	char* szRemaining;

	szToken = strtok( szMsg, szDelimiter );
	unsigned int nPlayers = atoi( szToken );

	//	We have to assist strtok a bit because of calls below that may also call strtok()...
	szRemaining = szMsg + 3;

	for( unsigned int nPlayer = 0; nPlayer != nPlayers; ++nPlayer )
	{
		//	Read in length of following string.
		szToken = strtok( szRemaining, szDelimiter );
		int iLen = atoi( szToken );

		//	Set string pointer to start of string (length is 2 digits).
		szRemaining = szToken + 3;
		//	Read in string.
		char szPlayerName[ 50 ];
		memcpy( szPlayerName, szRemaining, iLen );
		//	Null-terminate.
		szPlayerName[ iLen ] = 0;

		//	Advance string pointer to next param.
		szRemaining += iLen + 1;

		//	Read color.
		szToken = strtok( szRemaining, szDelimiter );
		PlayerColorType Color = (PlayerColorType)atoi( szToken );
		SetPlayerColor( szPlayerName, Color );

		//	SetPlayerColor may call strtok, so we can't use the strtok( NULL, option... in the next call.
		szRemaining += 3;

		//	Read whether there is a house field.
		szToken = strtok( szRemaining, szDelimiter );
		bool bHouseField = (bool)atoi( szToken );

		if( bHouseField )
		{
			//	Read house.
			szToken = strtok( NULL, szDelimiter );
			HousesType House = (HousesType)atoi( szToken );
			SetPlayerHouse( szPlayerName, House );
			//	SetPlayerHouse may call strtok, so we can't use the strtok( NULL, option... in the next call.
			szRemaining += 5;
		}
		else
			szRemaining += 2;		//	Advance past "0 ".

//	Acceptedness must be false. No need for it in message.
//		//	Read acceptedness.
//		szToken = strtok( NULL, szDelimiter );
//		bool bAccepted = (bool)atoi( szToken );
//		if( bAccepted )
//			SetPlayerAccepted( szPlayerName, true );
	}

	szToken = strtok( NULL, szDelimiter );
//	if( szToken )
//		debugprint( "szToken should be NULL!!!!!!!!\n" );

	ClearAllAccepts();		//	Most likely a pointless precaution.
}

//***********************************************************************************************
void WOL_GameSetupDialog::OnGuestLeave( User* pUser )
{
	//	pUser is about to leave but is still in our player list.
	if( pUser->flags & CHAT_USER_CHANNELOWNER  )
	{
		//	Host is leaving the channel. We must be a guest, and so must leave also. This will trigger exit.
		strcpy( szNameOfHostWhoJustBailedOnUs, (char*)pUser->name );
	}
	else
	{
		ClearAllAccepts();
	}
}

//***********************************************************************************************
void WOL_GameSetupDialog::ClearAllAccepts()
{
	//	Clears all "player has accepted" marks.
//debugprint( "ClearAllAccepts()\n" );
	for( int i = 0; i < pILPlayers->Count(); i++ )
	{
		User* pUser = (User*)pILPlayers->Get_Item_ExtraDataPtr( i );
		if( pUser && !( pUser->flags & CHAT_USER_CHANNELOWNER ) )	//	pUser null if this is an "early insertion" entry on startup
			pWO->MarkItemAccepted( i, false );
	}

	if( pToolTipHitLast && pToolTipHitLast->bShowing )
		pToolTipHitLast->Unshow();

	if( bHost )
	{
		pTextBtnAcceptStart->Disable();
		if( bWaitingToStart )
		{
			//	Something has happened that makes starting a game not possible now.
			//	Cancel out of waiting mode and tell guests to do the same.
			bWaitingToStart = false;
			InformAboutCancelStart();
			BindControls( true );
			ResetReadyToGo();
			display = REDRAW_ALL;
		}
		else
			pTextBtnAcceptStart->Flag_To_Redraw();
	}
	else
	{
		pTextBtnAcceptStart->Enable();
		pTextBtnAcceptStart->Flag_To_Redraw();
	}
}

//***********************************************************************************************
bool WOL_GameSetupDialog::bAllGuestsAccept()
{
	for( int i = 0; i < pILPlayers->Count(); i++ )
	{
		if( !pWO->bItemMarkedAccepted( i ) )
			return false;
	}
	return true;
}

//***********************************************************************************************
PlayerColorType WOL_GameSetupDialog::ColorNextAvailable()
{
	//	Returns the first free player color available.

	//	(Totally unoptimized, but hardly ever called.)

	for( int i = 0; i < MAX_MPLAYER_COLORS; i++ )
	{
		if( pILPlayers->FindColor( &ColorRemaps[ i ] ) == -1 )
		{
			return (PlayerColorType)i;
		}
	}
//	debugprint( "ColorNextAvailable is NONE!\n" );
	return PCOLOR_NONE;
}

//***********************************************************************************************
void WOL_GameSetupDialog::GuestIsReadyToPlay( const char* szName, const char* szReadyState )
{
	//	A guest has responded to a game start request.
	SetPlayerReadyToGo( szName, szReadyState );

	if( bAllPlayersReadyToGo() )
	{
//		debugprint( "All players ready to go.\n" );
		//	We can start the game.
		bHostSayGo = true;				//	Set trigger to fire function after we're out of callback.
	}
}

//***********************************************************************************************
bool WOL_GameSetupDialog::bNeedScenarioDownload()
{
	//	Returns true if we don't have the scenario and it is allowable as a download.
	if( !bHost )
	{
		if( Find_Local_Scenario(	Session.Options.ScenarioDescription,
									Session.ScenarioFileName,
									Session.ScenarioFileLength,
									Session.ScenarioDigest,
					 				Session.ScenarioIsOfficial ) )
		{
//			debugprint( "bNeedScenarioDownload() returning false.\n" );
			bRequestedScenarioDownload = false;
			return false;
		}
		else
		{
/*			if( !Session.ScenarioIsOfficial )
			{
				bRequestedScenarioDownload = true;
				return true;
			}
			else
			{
//				debugprint( "bNeedScenarioDownload fatal\n" );
				Fatal( "" );
			}
*/
//			debugprint( "Requesting download\n" );		//	ajw  Shouldn't be happening with am maps when i have am...?
			bRequestedScenarioDownload = true;			//	All maps are downloadable.
			return true;
		}
	}

	bRequestedScenarioDownload = false;
	return false;
}

//***********************************************************************************************
void WOL_GameSetupDialog::HostSaysGo()
{
	bHostSayGo = false;
	bHostWaitingForGoTrigger = true;

//	debugprint( "HostSaysGo()\n" );

	if( !pWO->RequestGameStart() )
	{
//		debugprint( "Call to RequestGameStart() failed.\n" );
		//Fatal( "Call to RequestGameStart() failed.\n" );
		pWO->bSelfDestruct = true;
		return;
	}

	//	Tell guests to start game.
//	debugprint( "Telling guests to start game.\n" );

	//	Create WOL_GAMEOPT_INFGO message.
	//	This contains the color for each player, which can change about haphazardly at the end of setup,
	//	without causing "unacceptedness". This means that the colors everyone thinks everyone else is might not
	//	be sync'ed. Host sets everyone straight here.
	char szSend[ ( WOL_NAME_LEN_MAX + 10 ) * 4 + 50 ] = "";
	sprintf( szSend, "%02u", WOL_GAMEOPT_INFGO );

	User* pUser = pWO->pChatSink->pGameUserList;
	while( pUser )
	{
		char szUser[ WOL_NAME_LEN_MAX + 10 ];
		PlayerColorType Color = GetPlayerColor( (char*)pUser->name );
		sprintf( szUser, " %s %02u", (char*)pUser->name, Color );	//	What if player left just now, and got removed from list. Ok to continue and fail on game start?
		strcat( szSend, szUser );
		pUser = pUser->next;
	}
	if( !pWO->SendGo( szSend ) )
	{
//		debugprint( "Call to SendGo() failed.\n" );
		//Fatal( "Call to SendGo() failed.\n" );
		pWO->bSelfDestruct = true;
		return;
	}

/*	...Method changed. It appears that my channelleave may appear to guests before the "go" privategameopt.
	For this reason, I'll wait until I receive a copy of the "go" message sent to myself, before proceeding.

	pWO->pChat->PumpMessages();		//	Flush the send out.
	//	(ajw - Note: An apparent bug in wolapi means that this pump does not necessarily flush the go gameopts.
	//	This is ok in practice, because the host hits pumps later that will flush them.)

	//	Pretend we just received szSend, and processed it like a guest would.
	char* szFakeGoMessage = szSend + 3;

	TriggerGameStart( szFakeGoMessage );
*/
}

//***********************************************************************************************
void WOL_GameSetupDialog::TriggerGameStart( char* szGoMessage )
{
	//	Last function before dialog is exited for game start. (Which must occur now.)
	//	Host or guest is about to start a game using final data in szGoMessage.

//	debugprint( "TriggerGameStart( %s )\n", szGoMessage );

	//	If we are in a modal dialog, we must have arrived here through Call_Back()'s PumpMessages. Set global that will
	//	force a cancel out of the dialog.
	if( pWO->bPump_In_Call_Back )
		cancel_current_msgbox = true;

	bHostWaitingForGoTrigger = false;
	bExitForGameTrigger = true;

	//	The following is based on Read_Game_Options()...

//	WWGetPrivateProfileString("Options", "Handle", "Noname", Session.Handle, sizeof(Session.Handle), buffer);

	strcpy( Session.Handle, pWO->szMyName );

	//	GameName will be the host's name...
	strcpy( Session.GameName, pWO->pGameHostName() );
//	debugprint( "Session.GameName is %s\n", Session.GameName );

//	gotit	Session.ColorIdx = (PlayerColorType) WWGetPrivateProfileInt("Options", "Color", 0, buffer);
//	gotit	Session.PrefColor = Session.ColorIdx;
//	gotit	int temp = WWGetPrivateProfileInt("Options", "Side", 0, buffer);
//	gotit	Session.House = (HousesType) ((int)HOUSE_USSR + temp);

//	gotit	Session.Options.Credits = WWGetPrivateProfileInt("Options", "Credits", 0, buffer);
//	gotit	Session.Options.Bases = WWGetPrivateProfileInt("Options", "Bases", 0, buffer);
//	gotit	Session.Options.Tiberium = WWGetPrivateProfileInt("Options", "Tiberium", 0, buffer);
//	gotit	Session.Options.Goodies = WWGetPrivateProfileInt("Options", "Crates", 0, buffer);
//	gotit	Special.IsShadowGrow = WWGetPrivateProfileInt ("Options", "Shadow", 0, buffer);
//	gotit	BuildLevel = WWGetPrivateProfileInt("Options", "BuildLevel", 0, buffer);
//	gotit	Session.Options.UnitCount = WWGetPrivateProfileInt("Options", "UnitCount", 0, buffer);
//	gotit	Seed = WWGetPrivateProfileInt("Options", "Seed", 0, buffer);
//	gotit	Special.IsCaptureTheFlag = WWGetPrivateProfileInt("Options", "CapFlag", 0, buffer);

//	UnitBuildPenalty = WWGetPrivateProfileInt ("Options", "BuildRate", 100, buffer);
	if( bSlowUnitBuildRate )
		UnitBuildPenalty = 250;
	else
		UnitBuildPenalty = 100;

	//PlanetWestwoodGameID = WWGetPrivateProfileInt("Internet", "GameID", 0, buffer);
	PlanetWestwoodGameID = pWO->pChatSink->iGameID;

	//	Reset ChatSink's iGameID.
	pWO->pChatSink->iGameID = 0;

	//PlanetWestwoodStartTime = WWGetPrivateProfileInt ("Internet", "StartTime", 0, buffer);
	PlanetWestwoodStartTime = time( NULL );
	//WChatHWND = (HWND) WWGetPrivateProfileInt("Internet", "HWND", (int)FindWindow("OWL_Window", "Westwood Chat"), buffer);

//	gotit	Session.Options.AIPlayers = WWGetPrivateProfileInt("Options", "AI", 0, buffer);		//Number of AI players
	if (Session.Options.AIPlayers){
		Session.Options.Ghosts = 1;
	}

	if (Session.Options.Tiberium) {
		Special.IsTGrowth = 1;
		Special.IsTSpread = 1;
	} else {
		Special.IsTGrowth = 0;
		Special.IsTSpread = 0;
	}

	//	The preceding was based on Read_Game_Options()...


	//	Now do whatever we've left out that the horrific Net_Fake_New_Dialog() and Net_Fake_Join_Dialog() used to do for us...

	//	Set up the Session.Players list.
	//	I think there is dependence on the local player being first, so put him there.
	//	Else put them in order listed in the szGoMessage.
	//	I will set "ID" based on a player's color, though it seems unclear if this is even used in the game, or what it should be.

	Clear_Vector( &Session.Players );

	//	Make the pILPlayers a valid list of players in the game.
	//	Players might (incredibly rarely) have joined in the last split-second, and we only want the players listed in
	//	the szGoMessage. To test for whether they're in this list, first wipe the colors from all list items.
	//	Then we fill them in from info in szGoMessage.
	//	We can ignore any list items then that have no color assigned.
	//	Also, we'll know that the colors assigned to valid players indeed match up with what every other client has.
	//	Remember, all other data should already be sync'ed because it has been implemented in such a way that changes would
	//	cause "unacceptedness" of guests to occur.

	//	Clear colors in list.
	for( int iItem = 0; iItem < pILPlayers->Count(); iItem++ )
		pILPlayers->Set_Item_Color( iItem, &ColorRemaps[ PCOLOR_NONE ] );

	//	Parse szGoMessage to iterate through players.
	char szDelimiter[] = " ";
	char* szToken;
	char szPlayerName[ WOL_NAME_LEN_MAX ];

	szToken = strtok( szGoMessage, szDelimiter );

	while( szToken )
	{
		strcpy( szPlayerName, szToken );
		szToken = strtok( NULL, szDelimiter );

		PlayerColorType Color = (PlayerColorType)atoi( szToken );
		SetPlayerColor( szPlayerName, Color );		//	ajw note: inserts if not found.
		szToken = strtok( NULL, szDelimiter );
	}

	//	Add myself to Session.Players list.
	_ASSERTE( pILPlayers->Find( pWO->szMyName ) != -1 );

	NodeNameType* pPlayerNew = new NodeNameType;
	strcpy( pPlayerNew->Name, pWO->szMyName );			//	"Name" is 12 chars max.
	//pPlayerNew->Address = Session.GAddress;
	pPlayerNew->Player.House = GetPlayerHouse( pWO->szMyName );
//debugprint( "ME: pPlayerNew->Player.House = %i\n", pPlayerNew->Player.House );
	pPlayerNew->Player.Color = GetPlayerColor( pWO->szMyName );
//	This gets done later.
//	pPlayerNew->Player.ID = (HousesType)( pPlayerNew->Player.Color + HOUSE_MULTI1 );

	Session.Players.Add( pPlayerNew );

	char szHostName[ WOL_NAME_LEN_MAX ] = "Game host";

	//	Add all other players to Session.Players list (if they have a valid color - see just above).
	//	Also in this step - build the scenario download requests array (used by hosts only).
	memset( Session.ScenarioRequests, 0, sizeof( Session.ScenarioRequests ) );
	Session.RequestCount = 0;
	for( iItem = 0; iItem < pILPlayers->Count(); iItem++ )
	{
		//	The following is not very efficient, but doesn't have to be. Better in this case to keep it clear and simple.
		pWO->PullPlayerName_Into_From( szPlayerName, pILPlayers->Get_Item( iItem ) );
		if( strcmp( szPlayerName, pWO->szMyName ) != 0 && GetPlayerColor( szPlayerName ) != PCOLOR_NONE )
		{
//			debugprint( "Creating player node '%s'\n", szPlayerName );
			pPlayerNew = new NodeNameType;
			strcpy( pPlayerNew->Name, szPlayerName );
			//	Get player's IP address from pChatSink...
			unsigned long lAddress = ( pWO->pChatSink->GetPlayerGameIP( szPlayerName ) );	//ntohl(
//			debugprint( "IP address is %i, or 0x%x\n", lAddress, lAddress );
			if( pWO->GameInfoCurrent.bTournament )
			{
				//	This is a tournament game, and I therefore have only one opponent: this one.
				//	for convenience, save a copy of his IP address in case I need it later for disconnect pinging.
				pWO->TournamentOpponentIP = lAddress;
				pWO->bDisconnectPingingCompleted = false;
			}
			NetNumType net;
			NetNodeType node;
			memset( net, 0, 4 );
			memset( node, 0, 6 );
			memcpy( node, &lAddress, 4 );
			//memcpy( node + 2, &lAddress, 4 );
			pPlayerNew->Address.Set_Address( net, node );
			//pPlayerNew->Address = Session.GAddress;
			pPlayerNew->Player.House = GetPlayerHouse( szPlayerName );
//debugprint( "Player %i: pPlayerNew->Player.House = %i\n", iItem, pPlayerNew->Player.House );
			pPlayerNew->Player.Color = GetPlayerColor( szPlayerName );
			//	This gets done later.
			//pPlayerNew->Player.ID = (HousesType)( pPlayerNew->Player.Color + HOUSE_MULTI1 );

			Session.Players.Add( pPlayerNew );

			//	If player is the game host, set HostAddress. This global is used when downloading scenarios; who knows where else.
			User* pUser = (User*)pILPlayers->Get_Item_ExtraDataPtr( iItem );
			if( pUser && pUser->flags & CHAT_USER_CHANNELOWNER )
			{
				Session.HostAddress = pPlayerNew->Address;
				strcpy( szHostName, (char*)pUser->name );
/*
				//	debugging
				NetNumType netxxx;
				NetNodeType nodexxx;
				Session.HostAddress.Get_Address( netxxx, nodexxx );
//				debugprint( "Host, ip %i.%i.%i.%i.%i.%i\n", nodexxx[0], nodexxx[1], nodexxx[2], nodexxx[3], nodexxx[4], nodexxx[5] );
*/
			}
/*
			else
			{
				NetNumType netxxx;
				NetNodeType nodexxx;
				pPlayerNew->Address.Get_Address( netxxx, nodexxx );
//				debugprint( "Player ip %i.%i.%i.%i.%i.%i\n", nodexxx[0], nodexxx[1], nodexxx[2], nodexxx[3], nodexxx[4], nodexxx[5] );
			}
*/

			if( bHost && pWO->bItemMarkedNeedScenario( iItem ) )
			{
//				debugprint( "%s has requested scenario download.\n", szPlayerName );
				Session.ScenarioRequests[ Session.RequestCount++ ] = Session.Players.Count() - 1;
			}
		}
//		else
//			debugprint( "%s excluded from Session.Players\n", szPlayerName );
	}


	//	From Init...
//	debugprint( "About to call Open_Socket().\n");
	PacketTransport->Open_Socket( 0 );

//	debugprint( "RA95 - About to call Start_Listening.\n" );
	PacketTransport->Start_Listening();

	/*
	** Flush out any pending packets from a previous game.
	*/
	PacketTransport->Discard_In_Buffers();
	PacketTransport->Discard_Out_Buffers();

	WWDebugString ("RA95 - About to call Init_Network.\n");
	Init_Network();

	Ipx.Set_Timing (	30, 		// retry 2 times per second
						-1, 		// ignore max retries
						600);		// give up after 10 seconds

//	debugprint( "Session.ScenarioFileName is %s.\n", Session.ScenarioFileName );


	/*
	** Read the scenario name from the .INI and try to match it with a scenario file in our list.
	*/
//	gotit		WWGetPrivateProfileString("Options", "Scenario", "SCM01EA.INI",
//										Session.Options.ScenarioDescription,
//										sizeof (Session.Options.ScenarioDescription),
//										buffer);
	//WWDebugString ("RA95I - Scenario is ");
	//WWDebugString (Session.Options.ScenarioDescription);
	//WWDebugString ("\n");

	if( !bHost )		//	Else ScenarioIndex is already set.
	{
		if( bRequestedScenarioDownload )
		{
			Session.Options.ScenarioIndex = 1;
			if( bSpecialAftermathScenario( Session.Options.ScenarioDescription ) )
			{
				//	Shouldn't ever happen. We should never have the opportunity to ask for one of these maps to be downloaded.
				bExitForGameTrigger = false;
				*szTriggerGameStartInfo = 0;
				//	Trigger the "our host just left the channel" code...
				strcpy( szNameOfHostWhoJustBailedOnUs, szHostName );
				return;
			}
			//	Wait for download from game host.
//debugprint( "Wait for download from game host.\n" );
			if( !Get_Scenario_File_From_Host( Session.ScenarioFileName, 1 ) )
			{
//				debugprint( "Get_Scenario_File_From_Host failed!\n" );
				bExitForGameTrigger = false;
				*szTriggerGameStartInfo = 0;
				//	Trigger the "our host just left the channel" code...
				strcpy( szNameOfHostWhoJustBailedOnUs, szHostName );
				return;
			}
			Scen.Scenario = Session.Options.ScenarioIndex;
//			debugprint( "Scen.Scenario = %i\n", Scen.Scenario );
			strcpy( Scen.ScenarioName, Session.ScenarioFileName );
//			debugprint( "Scen.ScenarioName = %s\n", Scen.ScenarioName );
		}
		else
		{
			//	Match ScenarioDescription to a ScenarioIndex.
/*	This is how the same code existed previously. Insufficient because Description may match on many scenarios.
			Session.Options.ScenarioIndex = -1;
			for (int i = 0; i < Session.Scenarios.Count(); i++) {
				if (!strcmp (Session.Scenarios[i]->Description(), Session.Options.ScenarioDescription) ){
					Session.Options.ScenarioIndex = i;
					break;
				}
			}
*/
			//	(We have already done the lookup, in Find_Local_Scenario, above.)
			Session.Options.ScenarioIndex = ScenarioIndex_From_Filename( Session.ScenarioFileName );
			_ASSERTE( Session.Options.ScenarioIndex != -1 );
			Scen.Scenario = Session.Options.ScenarioIndex;
//			debugprint( "Scen.Scenario = %i\n", Scen.Scenario );
			strcpy( Scen.ScenarioName, Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Filename() );
//			debugprint( "Scen.ScenarioName = %s\n", Scen.ScenarioName );
		}
	}
	else		//	bHost
	{
		Scen.Scenario = Session.Options.ScenarioIndex;
//		debugprint( "Scen.Scenario = %i\n", Scen.Scenario );
		strcpy( Scen.ScenarioName, Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Filename() );
//		debugprint( "Scen.ScenarioName = %s\n", Scen.ScenarioName );
		strcpy( Session.Options.ScenarioDescription, (char*)Session.Scenarios[ Session.Options.ScenarioIndex ]->Description() );
	}

	Options.GameSpeed = 0;

	//Session.MaxAhead = WChatMaxAhead = WWGetPrivateProfileInt("Timing", "MaxAhead", 9, buffer);
	//Session.FrameSendRate = WChatSendRate = WWGetPrivateProfileInt("Timing", "SendRate", 3, buffer);
	Session.MaxAhead = 15;	//9;
	//Session.FrameSendRate = 5;	//3;
	Session.FrameSendRate = 3;	//3;

	//	This is from NETDLG processing...
	Session.NumPlayers = Session.Players.Count();

	pWO->GameInfoCurrent.iPlayerCount = Session.Players.Count();

	Ipx.Set_Timing (25, (unsigned long) -1, 1000);


	if( bHost )
	{
		if( Session.Scenarios[ Session.Options.ScenarioIndex ]->Get_Official() )
		{
			if( !Force_Scenario_Available( Scen.ScenarioName ) )
			{
				bExitForGameTrigger = false;
				*szTriggerGameStartInfo = 0;
				pWO->bSelfDestruct = true;
				return;
			}
		}
		if( Session.RequestCount )
		{
			//	Send the scenario to any guests that requested a download.
//debugprint( "Send the scenario to any guests that requested a download.\n" );
			Send_Remote_File( Scen.ScenarioName, 1 );
		}
	}

	Session.CommProtocol = COMM_PROTOCOL_MULTI_E_COMP;
	Ipx.Set_Timing (30, (unsigned long) -1, 600);

	pWO->bEnableNewAftermathUnits = bAftermathUnits;
	bAftermathMultiplayer = bAftermathUnits;

	*pWO->szExternalPager = 0;

	pWO->bDoingDisconnectPinging = false;	//	Pointlessly making sure.

	*szTriggerGameStartInfo = 0;	//	This was set in order to trigger my coming here.
}

#endif

#include <string.h>
//***********************************************************************************************
bool bSpecialAftermathScenario( const char* szScenarioDescription )
{
	//	Returns true if szScenarioDescription matches one of the descriptions for Aftermath multiplayer
	//	scenarios that have special Aftermath-only units *embedded* within them.
	if( strcmp( szScenarioDescription, "Booby Traps (Mega 8 players)" ) == 0 ||
		strcmp( szScenarioDescription, "Central Conflict Extreme (Mega 8 players)" ) == 0 ||
		strcmp( szScenarioDescription, "Circles of Death (Mega 8 players)" ) == 0 ||
		strcmp( szScenarioDescription, "Holy Grounds (Mega 8 players)" ) == 0 ||
		strcmp( szScenarioDescription, "Island Wars Extreme (Mega 8 players)" ) == 0 ||
		strcmp( szScenarioDescription, "King of the Hills Extreme (Mega 8 players)" ) == 0 ||
		strcmp( szScenarioDescription, "The Hills Have Eyes (Mega 8 players)" ) == 0 )
		return true;
	return false;
}

//***********************************************************************************************
int ScenarioIndex_From_Filename( const char* szScenarioFilename )
{
#if (0)//PG
	//	Returns the scenario index that matches the scenario filename, or -1 if no match found.
	for( int index = 0; index < Session.Scenarios.Count(); index++ )
	{
		if( _stricmp( szScenarioFilename, Session.Scenarios[index]->Get_Filename() ) == 0 )
			return index;
	}
#endif
	return -1;
}
