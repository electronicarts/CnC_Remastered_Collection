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

//	wol_gsup.h			"WOL Game Setup Dialog"
//	ajw 08/06/98

//	Class WOL_GameSetupDialog is a move away from doing dialogs purely in C, a format that I've maintained in the login and
//	chat dialogs, mimicking how dialogs are done elsewhere in the code, but became frustrated with.
//	Though I'll follow the same pattern as before, things will be centralized a little cleaner through the use of this object.
//	Why a standard dialog class that would handle common input behavior and so forth (one that this class could derive from)
//	wasn't written 5 years ago, I don't know...

#include "WolapiOb.h"
//class WolapiObject;
class IconListClass;
class EditClass;
class GaugeClass;
class CheckListClass;
class TextButtonClass;
class StaticButtonClass;
class DropListClass;
class ShapeButtonClass;
class BigCheckBoxClass;
class ToolTipClass;

//***********************************************************************************************
enum RESULT_WOLGSUP
{
	RESULT_WOLGSUP_BACKTOCHAT,
	RESULT_WOLGSUP_FATALERROR,
	RESULT_WOLGSUP_HOSTLEFT,
	RESULT_WOLGSUP_STARTGAMEHOST,
	RESULT_WOLGSUP_STARTGAME,
	RESULT_WOLGSUP_RULESMISMATCH,
	RESULT_WOLGSUP_LOGOUT,
};

struct GAMEPARAMS
{
	GlobalPacketType	GPacket;
	//	My additions to game params. - ajw
	bool				bAftermathUnits;
	bool				bSlowUnitBuildRate;
};

//***********************************************************************************************
class WOL_GameSetupDialog
{
public:
	WOL_GameSetupDialog( WolapiObject* pWO, bool bHost );
	~WOL_GameSetupDialog();

	RESULT_WOLGSUP	Run();

public:
	bool	bHost;				//	True when I created the game channel and am the host.

	bool	bHostSayGo;					//	Trigger host instructing all to start game immediately.
	bool	bHostWaitingForGoTrigger;	//	True while host is waiting for go message to bounce back to him and trigger start.
	bool	bExitForGameTrigger;		//	Trigger exiting dialog for game.


	void	ProcessGuestRequest( User* pUser, const char* szRequest );
	void	ProcessInform( char* szRequest );
	void	OnGuestJoin( User* pUser );
	void	OnGuestLeave( User* pUser );

protected:
	void	Initialize();
	RESULT_WOLGSUP	Show();
	void	SetSpecialControlStates();
	void	BindControls( bool bBind );
	bool	ExitGameChannel();
	void	DrawScenarioDescripIcon( const char* pDIB ) const;
	void	SetPlayerColor( const char* szName, PlayerColorType Color );
	PlayerColorType		GetPlayerColor( const char* szName );
	void	SetPlayerHouse( const char* szName, HousesType House );
	HousesType	GetPlayerHouse( const char* szName );
	bool	SetPlayerAccepted( const char* szName, bool bAccepted );
	bool	IveAccepted();
	bool	SetPlayerReadyToGo( const char* szName, const char* szReadyState );
	void	ResetReadyToGo();
	bool	bPlayerReadyToGo( const char* szName );
	bool	bAllPlayersReadyToGo();

	bool	bParamsUnfresh();
	void	SendParams();
	bool	AcceptParams( char* szParams );
	void	SetGParamsToCurrent( GAMEPARAMS& GParams );

	void	AcceptNewGuestPlayerInfo( char* szMsg );

	bool	RequestPlayerColor( PlayerColorType Color );
	bool	InformAboutPlayerColor( const char* szName, PlayerColorType Color, User* pUserPriv );
	bool	InformAboutPlayerHouse( const char* szName, HousesType House, User* pUserPriv );
	bool	InformAboutPlayerAccept( const char* szName, User* pUserPriv );
	bool	InformAboutStart();
	bool	InformAboutCancelStart();

	void	ClearAllAccepts();
	bool	bAllGuestsAccept();

	PlayerColorType ColorNextAvailable();

	void	GuestIsReadyToPlay( const char* szName, const char* szReadyState );
	bool	bNeedScenarioDownload();
	void	HostSaysGo();
	void	TriggerGameStart( char* szGoMessage );

	enum SCENARIO_GAMEKIND
	{
		SCENARIO_RA = 0,
		SCENARIO_CS,
		SCENARIO_AM,
		SCENARIO_USER,
		SCENARIO_UNINITIALIZED,
	};
	void	ScenarioDisplayMode( SCENARIO_GAMEKIND ScenKind );

//	bool	bSpecialAftermathScenario( const char* szScenarioDescription );

public:
	int d_dialog_w;
	int d_dialog_h;
	int d_dialog_x;
	int d_dialog_y;
	int d_dialog_cx;

	int d_txt6_h;
	int d_margin1;

	int d_house_w;
	int d_house_h;
	int d_house_x;
	int d_house_y;

	int d_color_w;
	int d_color_h;
	int d_color_x;
	int d_color_y;

	int d_playerlist_w;
	int d_playerlist_h;
	int d_playerlist_x;
	int d_playerlist_y;

	int d_scenariolist_w;
	int d_scenariolist_h;
	int d_scenariolist_x;
	int d_scenariolist_y;

	int d_gamekind_w;
	int d_gamekind_h;
	int d_gamekind_x;
	int d_gamekind_y;

	int d_count_w;
	int d_count_h;
	int d_count_x;
	int d_count_y;

	int d_level_w;
	int d_level_h;
	int d_level_x;
	int d_level_y;

	int d_credits_w;
	int d_credits_h;
	int d_credits_x;
	int d_credits_y;

	int d_aiplayers_w;
	int d_aiplayers_h;
	int d_aiplayers_x;
	int d_aiplayers_y;

	int d_options_w;
	int d_options_h;
	int d_options_x;
	int d_options_y;

	int d_disc_w;
	int d_disc_h;
	int d_disc_x;
	int d_disc_y;

	int d_send_w;
	int d_send_h;
	int d_send_x;
	int d_send_y;

	int d_ok_w;
	int d_ok_h;
	int d_ok_x;
	int d_ok_y;

	int d_cancel_w;
	int d_cancel_h;
	int d_cancel_x;
	int d_cancel_y;

	int	d_accept_w;
	int	d_accept_h;
	int	d_accept_x;
	int	d_accept_y;

	int	d_amunits_w;
	int	d_amunits_h;
	int	d_amunits_x;
	int	d_amunits_y;

	int	d_action_w;
	int	d_action_h;
	int	d_action_x;
	int	d_action_y;

protected:
	GadgetClass*		commands;				//	The controls list.

	IconListClass*		pILPlayers;
	IconListClass*		pILScens;
	IconListClass*		pILDisc;
	char szSendBuffer[ MAXCHATSENDLENGTH ];
	EditClass*			pEditSend;
	GaugeClass*			pGaugeCount;
	GaugeClass*			pGaugeLevel;
	GaugeClass*			pGaugeCredits;
	GaugeClass*			pGaugeAIPlayers;
	CheckListClass*		pCheckListOptions;
	TextButtonClass*	pTextBtnOk;
	TextButtonClass*	pTextBtnCancel;
	TextButtonClass*	pTextBtnAcceptStart;
	TextButtonClass*	pTextBtnAction;
	StaticButtonClass*	pStaticDescrip;
	StaticButtonClass*	pStaticUnit;
	StaticButtonClass*	pStaticLevel;
	StaticButtonClass*	pStaticCredits;
	StaticButtonClass*	pStaticAIPlayers;
	char szHouseBuffer[25];						// buffer for house droplist
	DropListClass*		pDropListHouse;
	BigCheckBoxClass*	pCheckAftermathUnits;
	ShapeButtonClass*	pShpBtnScenarioRA;
	ShapeButtonClass*	pShpBtnScenarioCS;
	ShapeButtonClass*	pShpBtnScenarioAM;
	ShapeButtonClass*	pShpBtnScenarioUser;

	ToolTipClass*		pTTipAcceptStart;
	ToolTipClass*		pTTipCancel;
	ToolTipClass*		pTTipAction;

	WolapiObject*		pWO;


	GAMEPARAMS			GParamsLastSent;		//	Used merely as a handy container for the vars I need to set.

	DWORD				dwTimeNextParamRefresh;		//	Param changes are sent by host at certain interval.

	HousesType			HousePrevious;

	unsigned int		nHostLastParamID;		//	Host's send update tracking packet ID.
	unsigned int		nGuestLastParamID;		//	Guest's record of last ID received from host.

	bool				bWaitingToStart;

	bool				bProcess;				//	True means continue doing input loop.
	RESULT_WOLGSUP		ResultReturn;			//	Value that will be returned from Show().
	char				szNameOfHostWhoJustBailedOnUs[ WOL_NAME_LEN_MAX ];	//	If set, triggers setup cancellation.

	bool				bParamsReceived;		//	True after any WOL_GAMEOPT_INFPARAMS messages have been received from a host.

	bool				bLeaveDueToRulesMismatchTrigger;

	bool				bRequestedScenarioDownload;

	char				szTriggerGameStartInfo[ ( WOL_NAME_LEN_MAX + 10 ) * 4 + 60 ];

	//	Tooltips...
	DWORD				timeToolTipAppear;
	ToolTipClass*		pToolTipHead;			//	Head of list of ToolTips that parallels gadget list.
	ToolTipClass*		pToolTipHitLast;		//	ToolTip the mouse was last over, or null.

	//	Extra game params...
	bool				bAftermathUnits;		//	True if aftermath units are to be used in the game.
	bool				bSlowUnitBuildRate;

	SCENARIO_GAMEKIND	ScenKindCurrent;		//	Describes what gamekind of scenarios we are viewing, if host.
	DynamicVectorClass< const char* >	ar_szScenarios[ 4 ];	//	Lists of scenarios, by SCENARIO_GAMEKIND.
	//	ar_szScenIndexes parallels ar_szScenarios, holds ScenarioIndex. It's actually an int, but I'm using void* to avoid
	//	template instantiation problems and the need to change defines.h.
	DynamicVectorClass< void* >			ar_szScenIndexes[ 4 ];

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

		BUTTON_PLAYERLIST,
		BUTTON_HOUSE,
		BUTTON_SCENARIOLIST,
		BUTTON_DISCLIST,
		BUTTON_SENDEDIT,
		BUTTON_COUNT,
		BUTTON_LEVEL,
		BUTTON_CREDITS,
		BUTTON_AIPLAYERS,
		BUTTON_PARAMS,
//		BUTTON_OK,
		BUTTON_CANCEL,
		BUTTON_ACCEPTSTART,
		BUTTON_ACTION,
		BUTTON_AFTERMATHUNITS,
		BUTTON_SCENARIO_RA,
		BUTTON_SCENARIO_CS,
		BUTTON_SCENARIO_AM,
		BUTTON_SCENARIO_USER,
	};

	//------------------------------------------------------------------------
	//	Redraw values: in order from "top" to "bottom" layer of the dialog
	//------------------------------------------------------------------------
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_PARMS,
		REDRAW_MESSAGE,
		REDRAW_COLORS,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	RedrawType			display;

};

#endif

