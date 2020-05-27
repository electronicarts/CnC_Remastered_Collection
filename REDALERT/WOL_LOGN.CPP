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

//	Wol_Logn.cpp - WW online name/password dialog.
//	ajw 07/16/98

#include "function.h"

#include "IconList.h"
#include "WolapiOb.h"
#include "PassEdit.h"
#include "WolStrng.h"
#include "BigCheck.h"

bool ReadSavedNicks( WolapiObject* pWO, IconListClass& NickList, char* szNameBuffer, char* szPassBuffer );
bool bSaveNick( WolapiObject* pWO, const char* szNickToSave, const char* szPassToSave, bool bPassIsMangled );
void DeleteNick( WolapiObject* pWO, int iOneBasedEntryToDelete );
//char* LoadShpFile( const char* szShpFile );

void DebugChatDef( HRESULT hRes );

extern bool bTabKeyPressedHack;

//#include "WolDebug.h"

//***********************************************************************************************
int WOL_Login_Dialog( WolapiObject* pWO )
{
	//	Return values: 0 = user cancels, 1 = success, -1 = force game exit

	if( pWO->bLoggedIn() )
	{
		pWO->bReturningAfterGame = true;	//	Set trigger for chat dialog.
		return 1;		//	We are already logged in, and have just come back from a game.
	}

	/*
	**	Dialog & button dimensions
	*/
#ifdef FRENCH
	int d_dialog_w = 160 * RESFACTOR;											// dialog width
#else
	int d_dialog_w = 150 * RESFACTOR;											// dialog width
#endif
	int d_dialog_h = 85 * RESFACTOR;											// dialog height
	int d_dialog_x = (((320 * RESFACTOR) - d_dialog_w) / 2);
	int d_dialog_y = (((255 * RESFACTOR) - d_dialog_h) / 2);
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// coord of x-center

	int d_txt8_h = 11 * RESFACTOR;												// ht of 8-pt text
	int d_margin = 7 * RESFACTOR;												// margin width/height
	int x_margin = 16 * RESFACTOR;												// margin width/height

	int top_margin = 0;

	int d_name_w = 66 * RESFACTOR;
	int d_name_h = 10 * RESFACTOR;
#ifdef FRENCH
	int d_name_x = d_dialog_x + 25 * RESFACTOR;
#else
	int d_name_x = d_dialog_x + 20 * RESFACTOR;
#endif
	int d_name_y = d_dialog_y + top_margin + 25 * RESFACTOR;

	int d_pass_w = 36 * RESFACTOR;
	int d_pass_h = d_name_h;
	int d_pass_x = d_name_x + d_name_w + 6 * RESFACTOR;
	int d_pass_y = d_name_y;

	int d_list_w = d_name_w;
	int d_list_h = 20 * RESFACTOR;
	int d_list_x = d_name_x;
	int d_list_y = d_dialog_y + top_margin + 40 * RESFACTOR;

//	int d_save_w = d_pass_w;
	int d_save_h = 9 * RESFACTOR;
	int d_save_x = d_pass_x + ( d_pass_w / 2 ) - ( d_pass_w / 2 );
	int d_save_y = d_list_y;	// + ( d_list_h / 2 ) - ( d_save_h / 2 );

	int d_delete_w = d_pass_w;
	int d_delete_h = 10 * RESFACTOR;
	int d_delete_x = d_save_x;
	int d_delete_y = d_list_y + d_list_h - d_delete_h;

#ifdef FRENCH
	int d_connect_w = 45 * RESFACTOR;
#else
	int d_connect_w = 40 * RESFACTOR;
#endif
	int d_connect_h = 13 * RESFACTOR;
	int d_connect_x = d_name_x + d_name_w/2 - d_connect_w/2;
	int d_connect_y = d_dialog_y + top_margin + 65 * RESFACTOR;	//d_dialog_y + d_dialog_h - d_connect_h - d_margin;

#if defined(GERMAN) || defined(FRENCH)
	int d_cancel_w = 40 * RESFACTOR;//BG:40
#else
	int d_cancel_w = 40 * RESFACTOR;
#endif
	int d_cancel_h = 13 * RESFACTOR;
	int d_cancel_x = d_pass_x + d_pass_w/2 - d_cancel_w/2;	//d_dialog_cx + d_margin;
	int d_cancel_y = d_connect_y;

	/*
	**	Button enumerations
	*/
	enum {
		BUTTON_CONNECT = 100,
		BUTTON_CANCEL,
		LISTBOX_NICKS,
		EDITBOX_NAME,
		EDITBOX_PASS,
		BUTTON_SAVECHECK,
		BUTTON_DELETE,
	};

	/*
	**	Redraw values: in order from "top" to "bottom" layer of the dialog
	*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*
	**	Dialog variables
	*/
	int iReturn = 1;		//	0 = user cancels, 1 = success, -1 = force game exit

	/*
	**	Other Variables
	*/
	char szNameBuffer[ WOL_NAME_LEN_MAX ] = {0};				//	User name.
	char szPassBuffer[ WOL_PASSWORD_LEN ] = {0};				//	User password.

	/*
	**	Buttons
	*/
	ControlClass* commands = NULL;		// the button list

	TextButtonClass ConnectBtn( BUTTON_CONNECT, TXT_WOL_CONNECT, TPF_BUTTON, d_connect_x, d_connect_y, d_connect_w );
	TextButtonClass CancelBtn( BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w );

	IconListClass NickList( LISTBOX_NICKS, d_list_x, d_list_y, d_list_w, d_list_h, TPF_6PT_GRAD | TPF_NOSHADOW,
								MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"), true, 1, 0 );

	WOLEditClass NameEdit( EDITBOX_NAME, szNameBuffer, sizeof(szNameBuffer), TPF_6PT_GRAD|TPF_NOSHADOW, 
							d_name_x, d_name_y, d_name_w, -1, EditClass::ALPHANUMERIC );

	PassEditClass PassEdit( EDITBOX_PASS, szPassBuffer, sizeof(szPassBuffer), TPF_6PT_GRAD|TPF_NOSHADOW, 
							d_pass_x, d_pass_y, d_pass_w, -1, EditClass::ALPHANUMERIC );

	//	Just making sure globals are set right before String_Pixel_Width() call... sigh
	Fancy_Text_Print( TXT_NONE, 0, 0, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW );
	int iSaveTextWidth = String_Pixel_Width( TXT_WOL_SAVELOGIN ) + BIGCHECK_OFFSETX;
	BigCheckBoxClass SaveCheckBox( BUTTON_SAVECHECK, d_save_x, d_save_y, iSaveTextWidth, d_save_h, 
										TXT_WOL_SAVELOGIN, TPF_6PT_GRAD | TPF_NOSHADOW, true );

	TextButtonClass DeleteBtn( BUTTON_DELETE, TXT_DELETE_BUTTON, TPF_BUTTON, d_delete_x, d_delete_y, d_delete_w );

	/*
	**	Initialize.
	*/
	Set_Logic_Page(SeenBuff);

	//	Get saved nickname/passwords from the registry.
	if( ReadSavedNicks( pWO, NickList, szNameBuffer, szPassBuffer ) )
	{
		PassEdit.bClearOnNextSetFocus = true;
	}
	else
	{
		//	Offer user the chance to go to web site now to get a nick.
		if( pWO->DoWebRegistration() )
		{
			//	User chose to go to web page. Leave function so that we'll re-read nicks when they return.
			return 0;
		}
	}

	/*
	**	Create the button list.
	*/
	commands = &ConnectBtn;
	CancelBtn.Add_Tail(*commands);
	NickList.Add_Tail(*commands);
	NameEdit.Add_Tail(*commands);
	PassEdit.Add_Tail(*commands);
	SaveCheckBox.Add_Tail(*commands);
	DeleteBtn.Add_Tail(*commands);
	NameEdit.Set_Focus();

	if( NickList.Count() == 0 )
		DeleteBtn.Disable();

	/*
	**	Main Processing Loop.
	*/
	Keyboard->Clear();
	bool firsttime = true;
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

			//------------------------------------------------------------------------
			//	Clear screen
			//------------------------------------------------------------------------
			Hide_Mouse();
			Load_Title_Page(true);
//			Show_Mouse();

			/*
			**	Display the dialog box.
			*/
//			Hide_Mouse();
			if (display) {
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);
				Draw_Caption(TXT_WOL_LOGINDIALOG, d_dialog_x, d_dialog_y, d_dialog_w);
			}

			/*
			**	Redraw the buttons.
			*/
			if (display) {
				Fancy_Text_Print( TXT_WOL_NAME, d_name_x + ( d_name_w / 2 ), d_name_y - 14,
									GadgetClass::Get_Color_Scheme(), TBLACK, TPF_TEXT | TPF_CENTER );
				Fancy_Text_Print( TXT_WOL_PASSWORD, d_pass_x + ( d_pass_w / 2 ), d_pass_y - 14,
									GadgetClass::Get_Color_Scheme(), TBLACK, TPF_TEXT | TPF_CENTER );
				commands->Flag_List_To_Redraw();
			}
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
		bTabKeyPressedHack = false;
		KeyNumType input = commands->Input();

		/*
		**	The first time through the processing loop, set the edit
		**	gadget to have the focus. The
		**	focus must be set here since the gadget list has changed
		**	and this change will cause any previous focus setting to be
		**	cleared by the input processing routine.
		*/
		if (firsttime ) {
			firsttime = false;
			NameEdit.Set_Focus();
			NameEdit.Flag_To_Redraw();
		}

//		/*
//		**	If the <RETURN> key was pressed, then default to the appropriate
//		**	action button according to the style of this dialog box.
//		*/
/*		if (input == KN_RETURN || input == (BUTTON_CONNECT|KN_BUTTON)) {
			ToggleClass * toggle = NULL;
			input = (KeyNumType)(BUTTON_CONNECT|KN_BUTTON);
			CancelBtn.Turn_Off();
			toggle = (ToggleClass*)commands->Extract_Gadget(BUTTON_CONNECT);
			if (toggle != NULL) {
				toggle->Turn_On();
				toggle->IsPressed = true;
			}

			Hide_Mouse();
			commands->Draw_All(true);
			Show_Mouse();
		}
*/
		/*
		**	Process input.
		*/
//		if( input )
//			debugprint( "input: %i\n", input );

		if( bTabKeyPressedHack )
		{
			if( NameEdit.Has_Focus() )
				PassEdit.Set_Focus();
			else
				NameEdit.Set_Focus();
			NameEdit.Flag_To_Redraw();
			PassEdit.Flag_To_Redraw();
		}

		switch( input )
		{
			/*
			** ESC/Cancel: break
			*/
			case ( KN_ESC ):
			case ( BUTTON_CANCEL | KN_BUTTON ):
				iReturn = 0;
				process = false;
				break;

			case KN_RETURN:
			case ( EDITBOX_NAME | KN_BUTTON ):
			case ( EDITBOX_PASS | KN_BUTTON ):
			case ( BUTTON_CONNECT | KN_BUTTON ):
			{
				if( !strlen( szNameBuffer ) )
				{
					WWMessageBox().Process( TXT_WOL_MISSINGNAME );
					firsttime = true;		//	Bloody hack.
					NameEdit.Set_Focus();
					Keyboard->Clear();
					display = true;
					break;
				}
				if( !strlen( szPassBuffer ) )
				{
					WWMessageBox().Process( TXT_WOL_MISSINGPASSWORD );
					firsttime = true;		//	Bloody hack.
					PassEdit.Set_Focus();
					Keyboard->Clear();
					display = true;
					break;
				}
				
				//	If we have not done RequestServerList() yet, do it now.
				if( !pWO->pChatSink->pServer )
				{
					bool bBreak = false;
					HRESULT hRes = pWO->GetChatServer();
					switch( hRes )
					{
					case E_FAIL:
						bBreak = true;
						WWMessageBox().Process( TXT_WOL_CANTCONNECT );
						firsttime = true;		//	Bloody hack.
						NameEdit.Set_Focus();
						Keyboard->Clear();
						display = true;
						break;
					case USERCANCELLED:
						bBreak = true;
						WWMessageBox().Process( TXT_WOL_LOGINCANCEL );
						firsttime = true;		//	Bloody hack.
						NameEdit.Set_Focus();
						Keyboard->Clear();
						display = true;
						break;
					case PATCHAVOIDED:
						bBreak = true;
						firsttime = true;		//	Bloody hack.
						NameEdit.Set_Focus();
						Keyboard->Clear();
						display = true;
						break;
					case PATCHDOWNLOADED:
						bBreak = true;
						process = false;
						iReturn = -1;
						break;
					}
					if( bBreak )
						break;
				}
				
				//	RequestConnection()...
				HRESULT hRes = pWO->AttemptLogin( szNameBuffer, szPassBuffer, PassEdit.bClearOnNextSetFocus );
				if( hRes == S_OK )
				{
					if( SaveCheckBox.IsOn && !bSaveNick( pWO, szNameBuffer, szPassBuffer, PassEdit.bClearOnNextSetFocus ) )
					{
						//	Nick/pass save failed.
						WWMessageBox().Process( TXT_WOL_CANTSAVENICK );
					}
					process = false;
				}
				else
				{
					switch( hRes )
					{
					case USERCANCELLED:
						WWMessageBox().Process( TXT_WOL_LOGINCANCEL );
						break;
					case CHAT_E_TIMEOUT:
						WWMessageBox().Process( TXT_WOL_TIMEOUT );
						break;
					case CHAT_E_BADPASS:
						WWMessageBox().Process( TXT_WOL_BADPASS );
						break;
					case CHAT_E_NICKINUSE:
						WWMessageBox().Process( TXT_WOL_NICKINUSE );
						break;
					case CHAT_E_CON_ERROR:
						//	This error value I pass back myself, when the emergency timeout is hit.
						WWMessageBox().Process( TXT_WOL_TIMEOUT );
						break;
					}
					firsttime = true;		//	Bloody hack.
					NameEdit.Set_Focus();
					Keyboard->Clear();
					display = true;
				}
				break;
			}

/*
			case( EDITBOX_PASS | KN_BUTTON ):
			{
				//	Message with delay so that user has time to read it...
				CDTimerClass<SystemTimerClass> timer;
				timer = TICKS_PER_SECOND*4;
				WWMessageBox().Process(TXT_WOL_DEBUG2, TXT_NONE);
				while (timer > 0) {
					Call_Back();
				}
				Keyboard->Clear();

				display = true;
				break;
			}
*/
			case ( LISTBOX_NICKS | KN_BUTTON ):
				strcpy( szNameBuffer, NickList.Get_Item( NickList.Current_Index() ) );
				strcpy( szPassBuffer, NickList.Get_Item_ExtraDataString( NickList.Current_Index() ) );
				NameEdit.Flag_To_Redraw();
				PassEdit.Flag_To_Redraw();
				//	Because the password is mangled, if the user begins to edit it now, we clear it.
				//	Otherwise we could get a half-mangled, half-unmangled password field.
				//	PassEdit.bClearOnNextSetFocus also acts as a flag telling us whether or not the
				//	password field is mangled or not.
				PassEdit.bClearOnNextSetFocus = true;
//				display = true;
				break;

			case ( BUTTON_SAVECHECK | KN_BUTTON ):
				break;

			case ( BUTTON_DELETE | KN_BUTTON ):
				if( NickList.Count() > 0 )
				{
					DeleteNick( pWO, NickList.Current_Index() + 1 );
					NickList.Remove_Item( NickList.Current_Index() );
					NickList.Flag_To_Redraw();
					if( NickList.Count() == 0 )
					{
						DeleteBtn.Disable();
						DeleteBtn.Flag_To_Redraw();
					}
				}
				break;

			default:
				break;
		}
	}

	return iReturn;
}

//***********************************************************************************************
bool ReadSavedNicks( WolapiObject* pWO, IconListClass& NickList, char* szNameBuffer, char* szPassBuffer )
{
	//	Read saved nickname/passwords from the registry.
	//	Set up the list of nick/passwords.
	//	Copy the first nick into the nick/password edits.

	//	Returns true if edits are set with a default nick/pass because a nick was found.

	LPCSTR szNick;
	LPCSTR szPass;
	bool bReturn = false;

	for( int i = 1; i != 3; i++ )
	{
		if( pWO->pChat->GetNick( i, &szNick, &szPass ) == S_OK )
		{
			if( *szNick )
			{
				NickList.Add_Item( szNick, NULL, NULL, ICON_SHAPE, szPass );
				if( i == 1 )
				{
					strcpy( szNameBuffer, szNick );
					strcpy( szPassBuffer, szPass );
					bReturn = true;
				}
			}
		}
	}
	return bReturn;
}

//***********************************************************************************************
bool bSaveNick( WolapiObject* pWO, const char* szNickToSave, const char* szPassToSave, bool bPassIsMangled )
{
	//	Saves specified nick and password in the registry, using SetNick.
	//	Returns false if nick can't be saved.
	
	//	If slot 1 empty, use slot 1.
	//	Else push nick 1 down to second slot and save new nick in slot 1, unless
	//	nick 1 name matches new entry.

	LPCSTR szNick;
	LPCSTR szPass;
	bool bPushSlot1 = true;

	switch( pWO->pChat->GetNick( 1, &szNick, &szPass ) )
	{
	case E_FAIL:
		//	Assume that this is because there is no registry entry. We can use this slot.
		bPushSlot1 = false;
		break;
	case S_OK:
		if( *szNick == 0 )
			bPushSlot1 = false;			//	We can use this blank slot.
		else
			if( strcmp( szNick, szNickToSave ) == 0 )
				bPushSlot1 = false;		//	We can use this slot as the name is the same.
		break;
	}

	if( bPushSlot1 )
	{
		//	Move nick in slot 1 to slot 2.
		pWO->pChat->SetNick( 2, szNick, szPass, false );	//	(Already mangled.)
	}

	//	Save new nick in slot 1.
	return ( pWO->pChat->SetNick( 1, szNickToSave, szPassToSave, !bPassIsMangled ) == S_OK );

/*
	int iSlot;
	bool bStop = false;
	for( iSlot = 1; iSlot != 3; iSlot++ )
	{
		switch( pWO->pChat->GetNick( iSlot, &szNick, &szPass ) )
		{
		case E_FAIL:
			//	Assume that this is because there is no registry entry. We can use this slot.
			bStop = true;
			break;
		case S_OK:
			if( *szNick == 0 )
				bStop = true;		//	We can use this blank slot.
			else
				if( strcmp( szNick, szNickToSave ) == 0 )
					bStop = true;	//	We can use this slot as the name is the same.
			break;
		}
		if( bStop )
			break;
	}
	if( iSlot == 3 )
	{
		//	No open slots were found.
		//	Get nick 1.
		pWO->pChat->GetNick( 1, &szNick, &szPass );
		//	Save as nick 2.
		pWO->pChat->SetNick( 2, szNick, szPass, false );	//	(Already mangled.)
		//	Save new nick 1.
		return ( pWO->pChat->SetNick( 1, szNickToSave, szPassToSave, !bPassIsMangled ) == S_OK );
	}
	else
	{
		//	iSlot points to an open slot.
		return ( pWO->pChat->SetNick( iSlot, szNickToSave, szPassToSave, !bPassIsMangled ) == S_OK );
	}
*/
}

//***********************************************************************************************
void DeleteNick( WolapiObject* pWO, int iOneBasedEntryToDelete )
{
	//	Delete a nick from the registry via wolapi SetNick.
	//	If nick to delete is in position one, and there is a second nick, move the second nick into position one.
	if( iOneBasedEntryToDelete == 1 )
	{
		//	Check for nick 2.
		LPCSTR szNick;
		LPCSTR szPass;
		if( pWO->pChat->GetNick( 2, &szNick, &szPass ) == S_OK && *szNick != 0 )
		{
			//	Copy nick in slot 2 to slot 1.
			pWO->pChat->SetNick( 1, szNick, szPass, false );	//	(Already mangled.)
			//	Delete slot 2.
			HRESULT hRes = pWO->pChat->SetNick( 2, "", "", false );
			DebugChatDef( hRes );
		}
		else
		{
			//	No second nick.
			HRESULT hRes = pWO->pChat->SetNick( 1, "", "", false );
			DebugChatDef( hRes );
		}
	}
	else
	{
		HRESULT hRes = pWO->pChat->SetNick( 2, "", "", false );
		DebugChatDef( hRes );
	}
}

/*
//***********************************************************************************************
char* LoadShpFile( const char* szShpFile )
{
	//	Returns pointer to shp data that has been new'ed (and must be delete[]d), or NULL if failure.
	//	ajw: No longer needed - I used this before putting new resources into a mix file.
	HANDLE hFile;
	hFile = CreateFile( szShpFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return NULL;
	DWORD dwFileSize = GetFileSize( hFile, NULL );
	char* pShp = new char[ dwFileSize ];
	DWORD dwBytesRead;
	ReadFile( hFile, pShp, dwFileSize, &dwBytesRead, NULL );
//	debugprint( "~~ LoadShpFile() - Read %i bytes out of %i from shp file.\n", dwBytesRead, dwFileSize );
	CloseHandle( hFile );
	return pShp;
}
*/

#endif
