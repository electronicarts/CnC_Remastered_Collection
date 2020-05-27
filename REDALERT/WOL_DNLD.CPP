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

//	Wol_Dnld.cpp - WW online patch download dialog.
//	ajw 10/12/98

#include "function.h"
#include "WolapiOb.h"
#include "WolStrng.h"

//***********************************************************************************************
bool WOL_Download_Dialog( IDownload* pDownload, RADownloadEventSink* pDownloadSink, const char* szTitle )
{
	//	This dialog is presented for each file that is to be downloaded during a WOLAPI patch.

	bool bReturn = true;
	DWORD dwTimeNextPump = ::timeGetTime() + WOLAPIPUMPWAIT;

	/*
	** Dialog & button dimensions
	*/
	int d_dialog_w = 200*RESFACTOR;										// dialog width
	int d_dialog_h = 90*RESFACTOR;										// dialog height
	int d_dialog_x = ((320*RESFACTOR - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*RESFACTOR - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_margin = 34;
	int d_txt6_h = 15;

#if (GERMAN | FRENCH)
	int d_cancel_w = 50*RESFACTOR;
#else
	int d_cancel_w = 40*RESFACTOR;
#endif
	int d_cancel_h = 9*RESFACTOR;
	int d_cancel_x = d_dialog_cx - d_cancel_w / 2;
	int d_cancel_y = d_dialog_y + d_dialog_h - 20*RESFACTOR;

	int d_progress_w = 100*RESFACTOR;
	int d_progress_h = 10*RESFACTOR;
	int d_progress_x = (SeenBuff.Get_Width()/2) - d_progress_w/2;
	int d_progress_y = d_dialog_y + 45*RESFACTOR;

//	int	width;
//	int	height;
//	char* info_string = (char*)szTitle;

	Fancy_Text_Print( TXT_NONE, 0, 0, GadgetClass::Get_Color_Scheme(),
						TBLACK, TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW );

//	Format_Window_String( info_string, SeenBuff.Get_Height(), width, height );


	/*
	** Button Enumerations
	*/
	enum {
		BUTTON_CANCEL = 100,
		BUTTON_PROGRESS
	};

	/*
	** Buttons
	*/
	TextButtonClass cancelbtn( BUTTON_CANCEL, TXT_CANCEL, TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
#if (GERMAN | FRENCH)
									d_cancel_x, d_cancel_y );
#else
									d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h );
#endif

	GaugeClass progress_meter( BUTTON_PROGRESS, d_progress_x, d_progress_y, d_progress_w, d_progress_h );
	progress_meter.Use_Thumb( 0 );

	StaticButtonClass StatTitle( 0, szTitle, TPF_CENTER|TPF_TEXT, d_dialog_x + d_margin, d_dialog_y + 28, d_dialog_w - 2 * d_margin, d_txt6_h );
	StaticButtonClass StatStatus( 0, "", TPF_CENTER|TPF_TEXT, d_dialog_x + d_margin, d_dialog_y + 49, d_dialog_w - 2 * d_margin, d_txt6_h );
	StaticButtonClass StatBytes( 0, "", TPF_CENTER|TPF_TEXT, d_dialog_x + d_margin, d_dialog_y + 71, d_dialog_w - 2 * d_margin, d_txt6_h );
	StaticButtonClass StatTime( 0, "", TPF_CENTER|TPF_TEXT, d_dialog_x + d_margin, d_dialog_y + 117, d_dialog_w - 2 * d_margin, d_txt6_h );

	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_PROGRESS,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;


	bool 		process = true;
	RedrawType 	display = REDRAW_ALL;		// redraw level
	KeyNumType 	input;
	GadgetClass* commands;					// button list

	commands = &cancelbtn;
	progress_meter.Add_Tail(*commands);
	StatTitle.Add_Tail(*commands);
	StatBytes.Add_Tail(*commands);
	StatTime.Add_Tail(*commands);
	StatStatus.Add_Tail(*commands);

	progress_meter.Set_Maximum(100);		// Max is 100%
	progress_meter.Set_Value(0);			// Current is 0%

	do	{
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

		if (display){

			if (display >= REDRAW_BACKGROUND){

				Hide_Mouse();
				/*
				** Redraw backgound & dialog box
				*/
				Load_Title_Page(true);
				Set_Palette(CCPalette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				/*
				** Dialog & Field labels
				*/
				Draw_Caption (TXT_NONE, d_dialog_x, d_dialog_y, d_dialog_w);

//				Fancy_Text_Print(info_string, d_dialog_cx-width/2, d_dialog_y + 25*RESFACTOR,
//									GadgetClass::Get_Color_Scheme(), TBLACK,
//									TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Show_Mouse();

			}

			if (display >= REDRAW_BUTTONS){

				commands->Draw_All();

			}

			if (display >= REDRAW_PROGRESS){
				progress_meter.Draw_Me(true);
			}

			display = REDRAW_NONE;
		}

		if (process){
			input = cancelbtn.Input();
			switch (input) {

				/*
				** Cancel. Just return to the main menu
				*/
				case (KN_ESC):
				case (BUTTON_CANCEL | KN_BUTTON):
					pDownload->Abort();
					process = false;
					bReturn = false;
					break;
			}
		}

		if( ::timeGetTime() > dwTimeNextPump )
		{
			pDownload->PumpMessages();
			if( pDownloadSink->bFlagEnd )
			{
				pDownloadSink->bFlagEnd = false;
				process = false;
				break;
			}
			if( pDownloadSink->bFlagError )
			{
				WWMessageBox().Process( TXT_WOL_DOWNLOADERROR );
				pDownloadSink->bFlagError = false;
				process = false;
				bReturn = false;
				break;
			}
			if( pDownloadSink->bFlagProgressUpdate )
			{
				pDownloadSink->bFlagProgressUpdate = false;
				progress_meter.Set_Value( ( pDownloadSink->iBytesRead * 100 ) / pDownloadSink->iTotalSize );
				char szText[200];
				sprintf( szText, TXT_WOL_DOWNLOADBYTES, pDownloadSink->iBytesRead, pDownloadSink->iTotalSize,
							( pDownloadSink->iBytesRead * 100 ) / pDownloadSink->iTotalSize );
				StatBytes.Set_Text( szText );
				sprintf( szText, TXT_WOL_DOWNLOADTIME, pDownloadSink->iTimeLeft / 60, pDownloadSink->iTimeLeft % 60 );
				StatTime.Set_Text( szText );
				if( display < REDRAW_BUTTONS ) display = REDRAW_BUTTONS;
			}
			if( pDownloadSink->bFlagStatusUpdate )
			{
				pDownloadSink->bFlagStatusUpdate = false;
				switch( pDownloadSink->iStatus )
				{
				case DOWNLOADSTATUS_CONNECTING:
					StatStatus.Set_Text( TXT_WOL_DOWNLOADCONNECTING );
					break;

				case DOWNLOADSTATUS_FINDINGFILE:
					StatStatus.Set_Text( TXT_WOL_DOWNLOADLOCATING );
					break;

				case DOWNLOADSTATUS_DOWNLOADING:
					StatStatus.Set_Text( TXT_WOL_DOWNLOADDOWNLOADING );
					break;

				default:
//					debugprint( "Unknown status update!\n" );
					break;
				}
				if( display < REDRAW_BUTTONS ) display = REDRAW_BUTTONS;
			}
			if( pDownloadSink->bFlagQueryResume )
			{
				if( pDownloadSink->bResumed )
				{
					char szTitleNew[200];
					sprintf( szTitleNew, TXT_WOL_DOWNLOADRESUMED, szTitle );
					StatTitle.Set_Text( szTitleNew );
					if( display < REDRAW_BUTTONS ) display = REDRAW_BUTTONS;
				}
			}

			dwTimeNextPump = ::timeGetTime() + WOLAPIPUMPWAIT;
		}

		//	Invoke game callback
		Call_Back();

	} while ( process );

	return bReturn;
}

#endif
