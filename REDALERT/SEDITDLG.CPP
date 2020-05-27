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

//	SEditDlg.cpp - "SimpleEditDlgClass": An ok/cancel type dialog with 1 or 2 edit boxes.
//					Mostly a hack for what I need right now - not necessarily very flexible.
//					Still - I can't believe there isn't a set of dialog classes in here already.
//	ajw 07/21/98

#include "SEditDlg.h"
#include "WOLEdit.h"

extern bool cancel_current_msgbox;
bool disable_current_msgbox = false;

//***********************************************************************************************
SimpleEditDlgClass::SimpleEditDlgClass( int iDialogWidth, const char* szTitle, const char* szPrompt, int iEditCharsAccept,
											const char* szPrompt2 /* = NULL */, int iEditCharsAccept2 /* = 0 */ )
	: iDialogWidth( iDialogWidth ), iEditCharsAccept( iEditCharsAccept ), iEditCharsAccept2( iEditCharsAccept2 )
{
	//	Copy strings.
	if( szTitle )
	{
		this->szTitle = new char[ strlen( szTitle ) + 1 ];
		strcpy( this->szTitle, szTitle );
	}
	else
		this->szTitle = NULL;

	if( szPrompt )
	{
		this->szPrompt = new char[ strlen( szPrompt ) + 1 ];
		strcpy( this->szPrompt, szPrompt );
	}
	else
		this->szPrompt = NULL;		//	I wouldn't try this ... not totally implemented.

	if( szPrompt2 )
	{
		this->szPrompt2 = new char[ strlen( szPrompt2 ) + 1 ];
		strcpy( this->szPrompt2, szPrompt2 );
	}
	else
		this->szPrompt2 = NULL;		//	This is the flag for whether or not there is a second edit box.

	*szEdit = 0;
	*szEdit2 = 0;

	szOkButton = Text_String( TXT_OK );
	szCancelButton = Text_String( TXT_CANCEL );
	szMiddleButton = NULL;
}

//***********************************************************************************************
SimpleEditDlgClass::~SimpleEditDlgClass()
{
	delete [] szTitle;
	delete [] szPrompt;
	delete [] szPrompt2;
}

//***********************************************************************************************
void SimpleEditDlgClass::SetButtons( const char* szOk, const char* szCancel, const char* szMiddle /*= NULL*/ )
{
	szOkButton = szOk;
	szCancelButton = szCancel;
	szMiddleButton = szMiddle;
}

//***********************************************************************************************
const char* SimpleEditDlgClass::Show()
{
	//	Shows dialog, returns text of button pressed.
	//	Unless SetButtons() is used, value will be TXT_OK or TXT_CANCEL string values.

	bool bEscapeDown = false;
	bool bReturnDown = false;

	/*
	**	Dialog & button dimensions
	*/
	int x_margin = 18 * RESFACTOR;												// margin width/height
	int y_margin = 10 * RESFACTOR;												// margin width/height
	int d_gap_y = 5 * RESFACTOR;

	int d_dialog_w = iDialogWidth;
	int d_dialog_h = szPrompt2 ? ( 29 * RESFACTOR ) + 2 * d_gap_y + 2 * y_margin : ( 19 * RESFACTOR ) + d_gap_y + 2 * y_margin;
	if( szTitle )
		d_dialog_h += 10 * RESFACTOR + 2 * d_gap_y;
	int d_dialog_x = (((320 * RESFACTOR) - d_dialog_w) / 2);
	int d_dialog_y = (((200 * RESFACTOR) - d_dialog_h) / 2);
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// coord of x-center

/*
	if( szTitle )
	{
		d_title_w = String_Pixel_Width( szTitle );
		d_title_h = 10 * RESFACTOR;
		d_title_x = d_dialog_cx - d_title_w / 2;
		d_title_y = d_dialog_y + d_gap_y;
	}
*/

	int d_prompt_w = String_Pixel_Width( szPrompt );
	int d_prompt_h = 10 * RESFACTOR;
	int d_prompt_x = d_dialog_x + x_margin;
	int d_prompt_y = szTitle ? ( d_dialog_y + 3 * d_gap_y + 10 * RESFACTOR ) : ( d_dialog_y + d_gap_y );

	int d_edit_w = d_dialog_w - d_prompt_w - 2 * x_margin;
	int d_edit_h = 10 * RESFACTOR;
	int d_edit_x = d_dialog_x + d_prompt_w + x_margin;
	int d_edit_y = d_prompt_y;

	int d_prompt2_w = szPrompt2 ? String_Pixel_Width( szPrompt2 ) : 0;
	int d_prompt2_h = 10 * RESFACTOR;
	int d_prompt2_x = d_dialog_x + x_margin;
	int d_prompt2_y = d_prompt_y + d_prompt2_h + d_gap_y;

	int d_edit2_w = d_dialog_w - d_prompt2_w - 2 * x_margin;
	int d_edit2_h = 10 * RESFACTOR;
	int d_edit2_x = d_dialog_x + d_prompt2_w + x_margin;
	int d_edit2_y = d_prompt2_y;

	int d_ok_w, d_ok_h,	d_ok_x, d_ok_y, d_cancel_w, d_cancel_h, d_cancel_x, d_cancel_y, d_mid_x, d_mid_y, d_mid_w, d_mid_h;

	if( !szMiddleButton )
	{
		d_ok_w = 40 * RESFACTOR;
		d_ok_h = 9 * RESFACTOR;
		d_ok_x = d_dialog_cx - d_ok_w - 10 * RESFACTOR;
		d_ok_y = d_dialog_y + d_dialog_h - d_ok_h - y_margin;

		d_cancel_w = 40 * RESFACTOR;
		d_cancel_h = 9 * RESFACTOR;
		d_cancel_x = d_dialog_cx + 10 * RESFACTOR;
		d_cancel_y = d_ok_y;
	}
	else
	{
		d_ok_w = 40 * RESFACTOR;
		d_ok_h = 9 * RESFACTOR;
		d_ok_x = d_dialog_cx - d_ok_w - 30 * RESFACTOR;
		d_ok_y = d_dialog_y + d_dialog_h - d_ok_h - y_margin;

		d_mid_w = 40 * RESFACTOR;
		d_mid_h = 9 * RESFACTOR;
		d_mid_x = d_dialog_cx - ( d_mid_w / 2 );
		d_mid_y = d_ok_y;

		d_cancel_w = 40 * RESFACTOR;
		d_cancel_h = 9 * RESFACTOR;
		d_cancel_x = d_dialog_cx + 30 * RESFACTOR;
		d_cancel_y = d_ok_y;
	}

	/*
	**	Button enumerations
	*/
	enum {
		BUTTON_OK = 100,
		BUTTON_CANCEL,
		BUTTON_MIDDLE,
		BUTTON_EDIT,
		BUTTON_EDIT2
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
	const char* szReturn = NULL;

	/*
	**	Buttons
	*/
	ControlClass* commands = NULL;		// the button list

	TextButtonClass OkBtn( BUTTON_OK, szOkButton, TPF_BUTTON, d_ok_x, d_ok_y, d_ok_w );
	TextButtonClass CancelBtn( BUTTON_CANCEL, szCancelButton, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w );
	TextButtonClass MiddleBtn( BUTTON_MIDDLE, szMiddleButton, TPF_BUTTON, d_mid_x, d_mid_y, d_mid_w );

	WOLEditClass EditBox( BUTTON_EDIT, szEdit, min( sizeof(szEdit), iEditCharsAccept ), TPF_6PT_GRAD|TPF_NOSHADOW, 
							d_edit_x, d_edit_y, d_edit_w, -1, EditClass::ALPHANUMERIC );
	WOLEditClass EditBox2( BUTTON_EDIT2, szEdit2, min( sizeof(szEdit2), iEditCharsAccept2 ), TPF_6PT_GRAD|TPF_NOSHADOW, 
							d_edit2_x, d_edit2_y, d_edit2_w, -1, EditClass::ALPHANUMERIC );

	/*
	**	Initialize.
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Create the button list.
	*/
	commands = &OkBtn;
	CancelBtn.Add_Tail(*commands);
	if( szMiddleButton )
		MiddleBtn.Add_Tail(*commands);
	EditBox.Add_Tail(*commands);
	if( szPrompt2 )
		EditBox2.Add_Tail(*commands);
	EditBox.Set_Focus();

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

			/*
			**	Display the dialog box.
			*/
			Hide_Mouse();
			if (display) {
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);
				if( szTitle )
					Draw_Caption( szTitle, d_dialog_x, d_dialog_y, d_dialog_w );
			}

			/*
			**	Redraw the buttons.
			*/
			if (display) {
				Fancy_Text_Print( szPrompt, d_prompt_x, d_prompt_y,
									GadgetClass::Get_Color_Scheme(), TBLACK, TPF_TEXT );
				if( szPrompt2 )
					Fancy_Text_Print( szPrompt2, d_prompt2_x, d_prompt2_y,
										GadgetClass::Get_Color_Scheme(), TBLACK, TPF_TEXT );

				commands->Flag_List_To_Redraw();
			}
			Show_Mouse();
			display = false;
		}

		/*
		**	Get user input.
		*/
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
			EditBox.Set_Focus();
			EditBox.Flag_To_Redraw();
		}

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

		//	I really hate to do this, but...      ajw
		if( cancel_current_msgbox )
		{
			cancel_current_msgbox = false;
			input = (KeyNumType)( BUTTON_CANCEL | KN_BUTTON );
		}

		if( disable_current_msgbox )
		{
			disable_current_msgbox = false;
			EditBox.Disable();
			//	These do not actually draw. I am actually clearing the "draw" flag!
			//	Problem is Disable sets them to redraw, and I don't want to, and there is no Flag_To_Redraw( false ).
			EditBox.GadgetClass::Draw_Me( true );
			if( szPrompt2 )
			{
				EditBox2.Disable();
				EditBox2.GadgetClass::Draw_Me( true );
			}
			OkBtn.Disable();
			OkBtn.GadgetClass::Draw_Me( true );
			CancelBtn.Disable();
			CancelBtn.GadgetClass::Draw_Me( true );
			if( szMiddleButton )
			{
				MiddleBtn.Disable();
				MiddleBtn.GadgetClass::Draw_Me( true );
			}
		}

		/*
		**	Process input.
		*/
		switch( input )
		{
//		case ( KN_ESC ):
		case ( BUTTON_CANCEL | KN_BUTTON ):
			szReturn = szCancelButton;
			process = false;
			break;

//		case KN_RETURN:
		case ( BUTTON_EDIT | KN_BUTTON ):			//	(Return pressed while on edit.)
		case ( BUTTON_OK | KN_BUTTON ):
			szReturn = szOkButton;
			process = false;
			break;

		case ( BUTTON_MIDDLE | KN_BUTTON ):
			szReturn = szMiddleButton;
			process = false;
			break;

		default:
			break;
		}
	}

	return szReturn;
}

#endif
