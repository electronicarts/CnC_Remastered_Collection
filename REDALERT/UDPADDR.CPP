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



#include "function.h"
//#include "_WSProto.h"
#include "WSPUDP.h"


bool Get_Broadcast_Addresses (void)
{
	return false;
#if (0)//PG

	int d_dialog_w = 320 *RESFACTOR;											// dialog width
	int d_dialog_h = 160 *RESFACTOR;											// dialog height
	int d_dialog_x = ((320*RESFACTOR - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*RESFACTOR - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_txt6_h = 6 *RESFACTOR+1;												// ht of 6-pt text
	int d_margin1 = 5 *RESFACTOR;												// large margin
	int d_margin2 = 7 *RESFACTOR;												// small margin

	int d_ip_address_list_w = 300 *RESFACTOR;
	int d_ip_address_list_h = ((20 * 6) + 3) *RESFACTOR;		// 6 rows high
	int d_ip_address_list_x = d_dialog_cx - d_ip_address_list_w / 2;
	int d_ip_address_list_y = d_margin2 + d_dialog_y;


	int d_ok_w = 40*RESFACTOR;
	int d_ok_h = 9*RESFACTOR;
	int d_ok_x = d_dialog_cx + d_dialog_w / 4;
	int d_ok_y = d_dialog_y + d_dialog_h - 20*RESFACTOR;

#if (GERMAN | FRENCH)
	int d_cancel_w = 50*RESFACTOR;
#else
	int d_cancel_w = 40*RESFACTOR;
#endif
	int d_cancel_h = 9*RESFACTOR;
	int d_cancel_x = d_dialog_cx - d_dialog_w / 4;
	int d_cancel_y = d_dialog_y + d_dialog_h - 20*RESFACTOR;

	//------------------------------------------------------------------------
	//	Button Enumerations
	//------------------------------------------------------------------------
	enum {
		BUTTON_IPLIST=100,
		BUTTON_OK,
		BUTTON_CANCEL,
	};

	//------------------------------------------------------------------------
	//	Redraw values: in order from "top" to "bottom" layer of the dialog
	//------------------------------------------------------------------------
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_PARMS,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	//------------------------------------------------------------------------
	//	Dialog variables
	//------------------------------------------------------------------------
	RedrawType display = REDRAW_ALL;		// redraw level
	bool process = true;						// process while true
	KeyNumType input;

	int	width;
	int	height;

	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	//PG_TO_FIX
	//Fancy_Text_Print(TXT_NONE,0,0,TBLACK,TBLACK,TPF_6PT_GRAD | TPF_NOSHADOW);
	Format_Window_String("IP Addresses", SeenBuff.Get_Height(), width, height);


	GadgetClass *commands;										// button list
	ColorListClass ip_address_list(BUTTON_IPLIST, d_ip_address_list_x, d_ip_address_list_y, d_ip_address_list_w, d_ip_address_list_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));

	TextButtonClass okbtn(BUTTON_OK, TXT_OK, TPF_BUTTON, d_ok_x, d_ok_y, d_ok_w, d_ok_h);
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON, d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);

	ip_address_list.Set_Selected_Style(ColorListClass::SELECT_NORMAL);

	Fancy_Text_Print("", 0, 0, scheme, TBLACK, TPF_CENTER | TPF_TEXT);

	Load_Title_Page(true);
	CCPalette.Set();	//GamePalette.Set();



	/*
	** Add all the ip addresses from the ini file to the list box.
	*/
	CCINIClass ip_ini;
	int res=0;

	if (ip_ini.Load(CCFileClass("IP.INI"), false)) {
		int entry=0;
		char entry_name[16];
		do {
			entry++;
			char *temp = new char [128];
			sprintf (entry_name, "%d", entry);
			res = ip_ini.Get_String("IP_ADDRESSES", entry_name, "", temp, 128);
			if ( res ) {
				ip_address_list.Add_Item (temp);
char debug[128];
sprintf (debug, "RA95 - Adding address %s\n", temp);
WWDebugString (debug);
			}
		}while (res);
	}
	ip_address_list.Flag_To_Redraw();


	//------------------------------------------------------------------------
	//	Processing loop
	//------------------------------------------------------------------------
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}
		//.....................................................................
		//	Refresh display if needed
		//.....................................................................
		if (display) {
			Hide_Mouse();

			//..................................................................
			//	Redraw backgound & dialog box
			//..................................................................
			if (display >= REDRAW_BACKGROUND) {
				Load_Title_Page(true);
				CCPalette.Set();	//GamePalette.Set();
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				//...............................................................
				//	Dialog & Field labels
				//...............................................................
				Fancy_Text_Print("IP Addresses", d_dialog_cx-width/2, d_dialog_y + 25*RESFACTOR, scheme, TBLACK, TPF_TEXT);

				//...............................................................
				//	Rebuild the button list
				//...............................................................
				okbtn.Zap();
				cancelbtn.Zap();
				ip_address_list.Zap();

				commands = &okbtn;
				cancelbtn.Add_Tail(*commands);
				ip_address_list.Add_Tail(*commands);

			}

			//..................................................................
			//	Redraw buttons
			//..................................................................
			if (display >= REDRAW_BUTTONS) {
				commands->Draw_All();
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		//.....................................................................
		//	Get user input
		//.....................................................................
		input = commands->Input();

		//.....................................................................
		//	Process input
		//.....................................................................
		switch (input) {

			//..................................................................
			// ESC / CANCEL: send a SIGN_OFF
			// - If we're part of a game, stay in this dialog; otherwise, exit
			//..................................................................
			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				process = false;
				return (false);

			case (BUTTON_OK | KN_BUTTON):
				process = false;
				break;
		}
	}


	//------------------------------------------------------------------------
	//	Restore screen
	//------------------------------------------------------------------------
	Hide_Mouse();
	Load_Title_Page(true);
	CCPalette.Set();	//GamePalette.Set();
	Show_Mouse();

	for ( int i=0 ; i<ip_address_list.Count() ; i++ ) {
		char const *temp = ip_address_list.Get_Item(i);
		char *cut = (char*)strchr (temp, '#');
		if ( cut ) *cut = 0;
		PacketTransport->Set_Broadcast_Address ((char*)temp);
	}

	return (true);
#endif
}