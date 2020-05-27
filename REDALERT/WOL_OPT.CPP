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

//	Wol_Opt.cpp - WW online options dialog.
//	ajw 09/1/98

#include "function.h"

#include "IconList.h"
#include "WolapiOb.h"
#include "WolStrng.h"
#include "BigCheck.h"
//#include "WolDebug.h"

extern bool cancel_current_msgbox;

//***********************************************************************************************
bool WOL_Options_Dialog( WolapiObject* pWO, bool bCalledFromGame )
{
	//	Returns true only if called from inside game, and the game ended on us unexpectedly.
	bool bReturn = false;

	bool bEscapeDown = false;
	bool bReturnDown = false;

	bool bIgnoreReturnDown = false;
	if( ( ::GetAsyncKeyState( VK_RETURN ) & 0x8000 ) )
	{
		//	The return key is already down, as we enter the dialog.
		//	Until it comes up again, ignore this fact, so that we don't act on a return press that's not valid.
		bIgnoreReturnDown = true;
	}

	/*
	**	Dialog & button dimensions
	*/
#ifdef GERMAN
	int d_list_w = 180 * RESFACTOR;
#else
#ifdef FRENCH
	int d_list_w = 165 * RESFACTOR;
#else
	int d_list_w = 165 * RESFACTOR;
#endif
#endif

	int d_dialog_w = d_list_w + 40 * RESFACTOR;											// dialog width
	int d_dialog_h = 90 * RESFACTOR;											// dialog height
	int d_dialog_x = (((320 * RESFACTOR) - d_dialog_w) / 2);
	int d_dialog_y = (((200 * RESFACTOR) - d_dialog_h) / 2);
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// coord of x-center

	int d_txt8_h = 11 * RESFACTOR;												// ht of 8-pt text
	int d_margin = 7 * RESFACTOR;												// margin width/height
	int x_margin = 16 * RESFACTOR;												// margin width/height

	int top_margin = 0;

//	int d_list_w = 100 * RESFACTOR;
	int d_list_h = 7 * RESFACTOR;
	int d_list_x = d_dialog_cx - d_list_w / 2;
	int d_list_y = d_dialog_y + d_margin + 24;

#if (GERMAN | FRENCH)
	int d_ok_w = 40 * RESFACTOR;
#else
	int d_ok_w = 40 * RESFACTOR;
#endif
	int d_ok_h = 13 * RESFACTOR;
	int d_ok_x = d_dialog_cx - d_ok_w / 2;
	int d_ok_y = d_dialog_y + d_dialog_h - d_ok_h - d_margin;

	/*
	**	Button enumerations
	*/
	enum {
		BUTTON_OK = 100,
		CHECK_FIND,
		CHECK_PAGE,
		CHECK_LANGUAGE,
		CHECK_ALLGAMES,
		CHECK_RANKAM,
	};

	/*
	**	Buttons
	*/
	ControlClass* commands = NULL;		// the button list

	TextButtonClass OkBtn( BUTTON_OK, TXT_OK, TPF_BUTTON, d_ok_x, d_ok_y, d_ok_w );

	BigCheckBoxClass FindCheck( CHECK_FIND, d_list_x, d_list_y, d_list_w, d_list_h, 
								TXT_WOL_OPTFIND, TPF_6PT_GRAD | TPF_NOSHADOW, pWO->bFindEnabled );
	BigCheckBoxClass PageCheck( CHECK_PAGE, d_list_x, d_list_y + d_list_h + 2, d_list_w, d_list_h, 
								TXT_WOL_OPTPAGE, TPF_6PT_GRAD | TPF_NOSHADOW, pWO->bPageEnabled );
	BigCheckBoxClass LanguageCheck( CHECK_LANGUAGE, d_list_x, d_list_y + 2 * ( d_list_h + 2 ), d_list_w, d_list_h, 
									TXT_WOL_OPTLANGUAGE, TPF_6PT_GRAD | TPF_NOSHADOW, pWO->bLangFilter );
	BigCheckBoxClass GamescopeCheck( CHECK_ALLGAMES, d_list_x, d_list_y + 3 * ( d_list_h + 2 ), d_list_w, d_list_h, 
									TXT_WOL_OPTGAMESCOPE, TPF_6PT_GRAD | TPF_NOSHADOW, !pWO->bAllGamesShown );
	BigCheckBoxClass RankAMCheck( CHECK_RANKAM, d_list_x, d_list_y + 4 * ( d_list_h + 2 ), d_list_w, d_list_h, 
									TXT_WOL_OPTRANKAM, TPF_6PT_GRAD | TPF_NOSHADOW, !pWO->bShowRankRA );

	/*
	**	Initialize.
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Create the button list.
	*/
	commands = &OkBtn;
	FindCheck.Add_Tail(*commands);
	PageCheck.Add_Tail(*commands);
	LanguageCheck.Add_Tail(*commands);
	GamescopeCheck.Add_Tail(*commands);
	RankAMCheck.Add_Tail(*commands);

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
		if( !bCalledFromGame )
			Call_Back();
		else
		{
			if( Main_Loop() )	//	Game ended on us in the background.
			{
				process = false;
				bReturn = true;
			}
		}

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
			Draw_Caption( TXT_WOL_OPTTITLE, d_dialog_x, d_dialog_y, d_dialog_w );
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
			input = (KeyNumType)( BUTTON_OK | KN_BUTTON );
			bEscapeDown = false;
		}
		if( ( ::GetAsyncKeyState( VK_RETURN ) & 0x8000 ) )
		{
			if( !bIgnoreReturnDown )
				bReturnDown = true;
		}
		else
		{
			bIgnoreReturnDown = false;
			if( bReturnDown )
			{
				input = (KeyNumType)( BUTTON_OK | KN_BUTTON );
				bReturnDown = false;
			}
		}

		/*
		**	Process input.
		*/

		if( cancel_current_msgbox )
		{
			cancel_current_msgbox = false;
			input = (KeyNumType)( BUTTON_OK | KN_BUTTON );
		}
		switch( input )
		{
			case ( BUTTON_OK | KN_BUTTON ):
				process = false;
				break;

			case ( CHECK_FIND | KN_BUTTON ):
			case ( CHECK_PAGE | KN_BUTTON ):
			case ( CHECK_LANGUAGE | KN_BUTTON ):
			case ( CHECK_ALLGAMES | KN_BUTTON ):
				pWO->SetOptions( FindCheck.IsOn, PageCheck.IsOn, LanguageCheck.IsOn, !GamescopeCheck.IsOn );
				break;

			case ( CHECK_RANKAM | KN_BUTTON ):
				pWO->bShowRankRA = !RankAMCheck.IsOn;
				pWO->bMyRecordUpdated = true;
				pWO->bShowRankUpdated = true;
				break;

			default:
				break;
		}
	}
	return bReturn;
}

#endif
