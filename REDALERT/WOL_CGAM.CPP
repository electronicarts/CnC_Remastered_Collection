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

#ifdef WOLAPI_INTEGRATION			//	Implies FIXIT_CSII.

//	Wol_CGam.cpp - Create game dialog.
//	ajw 09/9/98

#include "function.h"

#ifndef FIXIT_CSII
#error FIXIT_CSII must be defined.
#endif

#include "IconList.h"
#include "WolapiOb.h"
#include "WolStrng.h"
#include "SEditDlg.h"
#include "BigCheck.h"

//extern char* LoadShpFile( const char* szShpFile );
void SetPlayerCountList( IconListClass& PlayerCountList, int iPlayerMax, char* pShpBoxCheck, char* pShpBoxEmpty );

//***********************************************************************************************
CREATEGAMEINFO WOL_CreateGame_Dialog( WolapiObject* pWO )
{
	CREATEGAMEINFO cgiReturn;
	cgiReturn.bCreateGame = false;
	cgiReturn.iPlayerMax = 2;
	cgiReturn.bTournament = false;
	cgiReturn.bPrivate = false;
	cgiReturn.GameKind = CREATEGAMEINFO::RAGAME;
	*cgiReturn.szPassword = 0;

	bool bEscapeDown = false;
	bool bReturnDown = false;

	/*
	**	Dialog & button dimensions
	*/
	int d_dialog_w = 150 * RESFACTOR;											// dialog width
	int d_dialog_h = 135 * RESFACTOR;											// dialog height
	int d_dialog_x = (((320 * RESFACTOR) - d_dialog_w) / 2);
	int d_dialog_y = (((200 * RESFACTOR) - d_dialog_h) / 2);
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// coord of x-center

	int d_txt8_h = 11 * RESFACTOR;												// ht of 8-pt text
	int d_margin = 7 * RESFACTOR;												// margin width/height
	int x_margin = 16 * RESFACTOR;												// margin width/height

	int top_margin = 0;

	int d_gaugeplayers_w = 70 * RESFACTOR;
	int d_gaugeplayers_h = 9 * RESFACTOR;
	int d_gaugeplayers_x = d_dialog_cx - d_gaugeplayers_w / 2;
	int d_gaugeplayers_y = d_dialog_y + d_margin + 42;

	int d_checktourn_w = 75 * RESFACTOR;
	int d_checktourn_h = 9 * RESFACTOR;
	int d_checktourn_x = d_dialog_cx - d_checktourn_w / 2;
	int d_checktourn_y = d_gaugeplayers_y + d_gaugeplayers_h + 10;

	int d_checkpriv_w = d_checktourn_w;
	int d_checkpriv_h = 9 * RESFACTOR;
	int d_checkpriv_x = d_checktourn_x;
	int d_checkpriv_y = d_checktourn_y + d_checktourn_h + 10;

	int d_checkra_w = d_checktourn_w;
	int d_checkra_h = 9 * RESFACTOR;
	int d_checkra_x = d_checktourn_x;
	int d_checkra_y = d_checkpriv_y + d_checkpriv_h + 20;

	int d_checkcs_w = d_checktourn_w;
	int d_checkcs_h = 9 * RESFACTOR;
	int d_checkcs_x = d_checktourn_x;
	int d_checkcs_y = d_checkra_y + d_checkra_h + 5;

	int d_checkam_w = d_checktourn_w;
	int d_checkam_h = 9 * RESFACTOR;
	int d_checkam_x = d_checktourn_x;
	int d_checkam_y = d_checkcs_y + d_checkcs_h + 5;

#if (GERMAN | FRENCH)
	int d_ok_w = 30 * RESFACTOR;
#else
	int d_ok_w = 30 * RESFACTOR;
#endif
	int d_ok_h = 13 * RESFACTOR;
	int d_ok_x = d_dialog_x + ( d_dialog_w / 3 ) - ( d_ok_w / 2 );
	int d_ok_y = d_dialog_y + d_dialog_h - d_ok_h - d_margin;

	int d_cancel_w = 40 * RESFACTOR;
	int d_cancel_h = 9 * RESFACTOR;
	int d_cancel_x = d_dialog_x + ( ( d_dialog_w * 2 ) / 3 ) - ( d_cancel_w / 2 );
	int d_cancel_y = d_ok_y;

	/*
	**	Button enumerations
	*/
	enum {
		BUTTON_OK = 100,
		BUTTON_CANCEL,
		GAUGE_PLAYERCOUNT,
		CHECK_TOURNAMENT,
		CHECK_PRIVACY,
		CHECK_RA,
		CHECK_CS,
		CHECK_AM,
	};

	/*
	**	Buttons
	*/
	ControlClass* commands = NULL;		// the button list

	TextButtonClass OkBtn( BUTTON_OK, TXT_OK, TPF_BUTTON, d_ok_x, d_ok_y, d_ok_w );
	TextButtonClass CancelBtn( BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w );

	StaticButtonClass PlayerCountStatic( 0, "               ", TPF_TEXT, d_gaugeplayers_x, d_gaugeplayers_y - 16 );
	GaugeClass PlayerCountGauge( GAUGE_PLAYERCOUNT, d_gaugeplayers_x, d_gaugeplayers_y, d_gaugeplayers_w, d_gaugeplayers_h );

	if( pWO->bEgg8Player )
		PlayerCountGauge.Set_Maximum( 6 );
	else
		PlayerCountGauge.Set_Maximum( 2 );
	PlayerCountGauge.Set_Value( cgiReturn.iPlayerMax - 2 );

	BigCheckBoxClass TournamentCheck( CHECK_TOURNAMENT, d_checktourn_x, d_checktourn_y, d_checktourn_w, d_checktourn_h,
										TXT_WOL_CG_TOURNAMENT, TPF_6PT_GRAD | TPF_NOSHADOW, cgiReturn.bTournament );

	BigCheckBoxClass PrivacyCheck( CHECK_PRIVACY, d_checkpriv_x, d_checkpriv_y, d_checkpriv_w, d_checkpriv_h,
										TXT_WOL_CG_PRIVACY, TPF_6PT_GRAD | TPF_NOSHADOW, cgiReturn.bPrivate );

	BigCheckBoxClass RA_Check( CHECK_RA, d_checkra_x, d_checkra_y, d_checkra_w, d_checkra_h,
										TXT_WOL_CG_RAGAME, TPF_6PT_GRAD | TPF_NOSHADOW, cgiReturn.GameKind == CREATEGAMEINFO::RAGAME );
	BigCheckBoxClass CS_Check( CHECK_CS, d_checkcs_x, d_checkcs_y, d_checkcs_w, d_checkcs_h,
										TXT_WOL_CG_CSGAME, TPF_6PT_GRAD | TPF_NOSHADOW, cgiReturn.GameKind == CREATEGAMEINFO::CSGAME );
	BigCheckBoxClass AM_Check( CHECK_AM, d_checkam_x, d_checkam_y, d_checkam_w, d_checkam_h,
										TXT_WOL_CG_AMGAME, TPF_6PT_GRAD | TPF_NOSHADOW, cgiReturn.GameKind == CREATEGAMEINFO::AMGAME );

	if( !Is_Counterstrike_Installed() )
		CS_Check.Disable();

	if( !Is_Aftermath_Installed() )
		AM_Check.Disable();

	/*
	**	Initialize.
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Create the button list.
	*/
	commands = &OkBtn;
	CancelBtn.Add_Tail(*commands);
	PlayerCountStatic.Add_Tail(*commands);
	PlayerCountGauge.Add_Tail(*commands);
	TournamentCheck.Add_Tail(*commands);
	PrivacyCheck.Add_Tail(*commands);
	RA_Check.Add_Tail(*commands);
	CS_Check.Add_Tail(*commands);
	AM_Check.Add_Tail(*commands);

	char szPlayerCount[ 100 ];
	sprintf( szPlayerCount, TXT_WOL_CG_PLAYERS, cgiReturn.iPlayerMax );
	PlayerCountStatic.Set_Text( szPlayerCount );

	/*
	**	Main Processing Loop.
	*/
	Keyboard->Clear();
	bool display = true;
	bool process = true;
	while (process) {

		/*
		**	Invoke game callback.
		*/
		Call_Back();

		#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = true;
		}
		#endif

		/*
		**	Refresh display if needed.
		*/
		if (display) {

			/*
			**	Display the dialog box.
			*/
			Hide_Mouse();
			Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);
			Draw_Caption( TXT_WOL_CG_TITLE, d_dialog_x, d_dialog_y, d_dialog_w );
//			Fancy_Text_Print( TXT_WOL_CG_PLAYERS, d_gaugeplayers_x - 2*RESFACTOR, d_gaugeplayers_y, 
//								GadgetClass::Get_Color_Scheme(), TBLACK, TPF_TEXT | TPF_RIGHT );
			commands->Flag_List_To_Redraw();
			Show_Mouse();
			display = false;
		}

		//	Force mouse visible, as some beta testers report unexplicable disappearing cursors.
		while( Get_Mouse_State() )
			Show_Mouse();
		//	Be nice to other apps.
		Sleep( 50 );

		/*
		**	Get user input.
		*/
		KeyNumType input = commands->Input();

		//	My hack for triggering escape and return on key up instead of down...
		//	The problem that was occurring was that the calling dialog would act on the key up,
		//	though this dialog handled the key down. ajw
		if( ( ::GetAsyncKeyState( VK_ESCAPE ) & 0x8000 ) )
		{
			bEscapeDown = true;
		}
		else if( bEscapeDown )
		{
			input = (KeyNumType)( BUTTON_CANCEL | KN_BUTTON );
			bEscapeDown = false;
		}
		if( ( ::GetAsyncKeyState( VK_RETURN ) & 0x8000 ) )
		{
			bReturnDown = true;
		}
		else if( bReturnDown )
		{
			input = (KeyNumType)( BUTTON_OK | KN_BUTTON );
			bReturnDown = false;
		}

		/*
		**	Process input.
		*/

		switch( input )
		{
			case ( BUTTON_OK | KN_BUTTON ):
				cgiReturn.bCreateGame = true;
				process = false;
				break;

			case ( BUTTON_CANCEL | KN_BUTTON ):
				process = false;
				break;

			case ( GAUGE_PLAYERCOUNT | KN_BUTTON ):
				if( PlayerCountGauge.Get_Value() != 0 && cgiReturn.bTournament )
				{
					WWMessageBox().Process( TXT_WOL_TOURNAMENTPLAYERLIMIT );
					PlayerCountGauge.Set_Value( 0 );
					display = true;
				}
				cgiReturn.iPlayerMax = PlayerCountGauge.Get_Value() + 2;
				sprintf( szPlayerCount, TXT_WOL_CG_PLAYERS, cgiReturn.iPlayerMax );
				PlayerCountStatic.Set_Text( szPlayerCount );
				PlayerCountStatic.Draw_Me();
				break;

			case ( CHECK_TOURNAMENT | KN_BUTTON ):
				cgiReturn.bTournament = TournamentCheck.IsOn;
				if( cgiReturn.bTournament )
				{
					PlayerCountGauge.Set_Value( 0 );
//					PlayerCountGauge.Disable();
					cgiReturn.iPlayerMax = 2;
					sprintf( szPlayerCount, TXT_WOL_CG_PLAYERS, cgiReturn.iPlayerMax );
					PlayerCountStatic.Set_Text( szPlayerCount );
					PlayerCountStatic.Draw_Me();
				}
//				else
//					PlayerCountGauge.Enable();
				break;

			case ( CHECK_PRIVACY | KN_BUTTON ):
				cgiReturn.bPrivate = PrivacyCheck.IsOn;
				break;

			case ( CHECK_RA | KN_BUTTON ):
				if( RA_Check.IsOn )
				{
					//	Box was checked.
					CS_Check.Turn_Off();
					AM_Check.Turn_Off();
					cgiReturn.GameKind = CREATEGAMEINFO::RAGAME;
				}
				else
					//	Box was unchecked. Has no effect.
					RA_Check.Turn_On();
				break;
			case ( CHECK_CS | KN_BUTTON ):
				if( CS_Check.IsOn )
				{
					//	Box was checked.
					RA_Check.Turn_Off();
					AM_Check.Turn_Off();
					cgiReturn.GameKind = CREATEGAMEINFO::CSGAME;
				}
				else
					//	Box was unchecked. Has no effect.
					CS_Check.Turn_On();
				break;
			case ( CHECK_AM | KN_BUTTON ):
				if( AM_Check.IsOn )
				{
					//	Box was checked.
					RA_Check.Turn_Off();
					CS_Check.Turn_Off();
					cgiReturn.GameKind = CREATEGAMEINFO::AMGAME;
				}
				else
					//	Box was unchecked. Has no effect.
					AM_Check.Turn_On();
				break;

			default:
				break;
		}
	}

	if( cgiReturn.bCreateGame && cgiReturn.bPrivate )
	{
		//	Get a password for the channel.
		Fancy_Text_Print( TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_TEXT );	//	Required before String_Pixel_Width() call, for god's sake.
		SimpleEditDlgClass* pEditDlg = new SimpleEditDlgClass( 300, TXT_WOL_CREATEPRIVGAMETITLE, 
												TXT_WOL_PASSPROMPT, WOL_CHANKEY_LEN_MAX );
		pWO->bPump_In_Call_Back = true;
		if( strcmp( pEditDlg->Show(), Text_String( TXT_OK ) ) == 0 && *pEditDlg->szEdit )
			strcpy( cgiReturn.szPassword, pEditDlg->szEdit );
		else
			cgiReturn.bCreateGame = false;		//	Cancel creation.
		pWO->bPump_In_Call_Back = false;
	}

	return cgiReturn;
}

//***********************************************************************************************
void SetPlayerCountList( IconListClass& PlayerCountList, int iPlayerMax, char* pShpBoxCheck, char* pShpBoxEmpty )
{
	//	Checks appropriate list item based on iPlayerMax.
	switch( iPlayerMax )
	{
	case 2:
		PlayerCountList.Set_Icon( 0, 0, (void*)pShpBoxCheck, ICON_SHAPE );
		PlayerCountList.Set_Icon( 1, 0, (void*)pShpBoxEmpty, ICON_SHAPE );
		PlayerCountList.Set_Icon( 2, 0, (void*)pShpBoxEmpty, ICON_SHAPE );
		break;
	case 3:
		PlayerCountList.Set_Icon( 0, 0, (void*)pShpBoxEmpty, ICON_SHAPE );
		PlayerCountList.Set_Icon( 1, 0, (void*)pShpBoxCheck, ICON_SHAPE );
		PlayerCountList.Set_Icon( 2, 0, (void*)pShpBoxEmpty, ICON_SHAPE );
		break;
	case 4:
		PlayerCountList.Set_Icon( 0, 0, (void*)pShpBoxEmpty, ICON_SHAPE );
		PlayerCountList.Set_Icon( 1, 0, (void*)pShpBoxEmpty, ICON_SHAPE );
		PlayerCountList.Set_Icon( 2, 0, (void*)pShpBoxCheck, ICON_SHAPE );
		break;
	}
}

#endif
