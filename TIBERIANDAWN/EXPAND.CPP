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

/* $Header$ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : EXPAND.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/03/95                                                     *
 *                                                                                             *
 *                  Last Update : November 3, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

#ifdef NEWMENU

bool Expansion_Present(void)
{
	CCFileClass file("EXPAND.DAT");

	return(file.Is_Available());
}




class EListClass : public ListClass
{
	public:
		EListClass(int id, int x, int y, int w, int h, TextPrintType flags, void const * up, void const * down) :
			ListClass(id, x, y, w, h, flags, up, down) {};

	protected:
		virtual void Draw_Entry(int index, int x, int y, int width, int selected);
};


void EListClass::Draw_Entry(int index, int x, int y, int width, int selected)
{
	if (TextFlags & TPF_6PT_GRAD) {
		TextPrintType flags = TextFlags;

		if (selected) {
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect (x, y, x + width - 1, y + LineHeight - 1, CC_GREEN_SHADOW);
		} else {
			if (!(flags & TPF_USE_GRAD_PAL)) {
				flags = flags | TPF_MEDIUM_COLOR;
			}
		}

		Conquer_Clip_Text_Print(List[index]+sizeof(int), x, y, CC_GREEN, TBLACK, flags, width, Tabs);

	} else {
		Conquer_Clip_Text_Print(List[index]+sizeof(int), x, y, (selected ? BLUE : WHITE), TBLACK, TextFlags, width, Tabs);
	}
}

bool Expansion_Dialog(void)
{
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	int	option_width 	= 236 * factor;
	int	option_height	= 162 * factor;
	int	option_x			= (320*factor - option_width) /2;
	int	option_y			= (200*factor - option_height) /2;

	GadgetClass * buttons = NULL;

	void const *up_button;
	void const *down_button;

	if (InMainLoop){
		up_button = Hires_Retrieve("BTN-UP.SHP");
		down_button = Hires_Retrieve("BTN-DN.SHP");
	}else{
		up_button = Hires_Retrieve("BTN-UP2.SHP");
		down_button = Hires_Retrieve("BTN-DN2.SHP");
	}

	TextButtonClass ok(200, TXT_OK, TPF_6PT_GRAD|TPF_NOSHADOW, option_x+25*factor, option_y+option_height-15*factor);
	TextButtonClass cancel(201, TXT_CANCEL, TPF_6PT_GRAD|TPF_NOSHADOW, option_x+option_width-50*factor, option_y+option_height-15*factor);
	EListClass list(202, option_x+10*factor, option_y+20*factor, option_width-20*factor, option_height-40*factor, TPF_6PT_GRAD|TPF_NOSHADOW, up_button, down_button);

	buttons = &ok;
	cancel.Add(*buttons);
	list.Add(*buttons);


	/*
	**	Add in all the expansion scenarios.
	*/
	char * sbuffer = (char*)_ShapeBuffer;
	int index;
	for (index = 20; index < 60; index++) {
		char buffer[128];
		CCFileClass file;

		Set_Scenario_Name(buffer, index, SCEN_PLAYER_GDI, SCEN_DIR_EAST, SCEN_VAR_A);
		strcat(buffer, ".INI");
		file.Set_Name(buffer);
		if (file.Is_Available()) {
			file.Read(sbuffer, 1000);
			sbuffer[1000] = '\r';
			sbuffer[1000+1] = '\n';
			sbuffer[1000+2] = '\0';

			WWGetPrivateProfileString("Basic", "Name", "x", buffer, sizeof(buffer), sbuffer);
			char * data = new char [strlen(buffer)+1+sizeof(int)+25];
			*((int*)&data[0]) = index;
			strcpy(&data[sizeof(int)], "GDI: ");
			strcat(&data[sizeof(int)], buffer);
			list.Add_Item(data);
		}
	}

	for (index = 20; index < 60; index++) {
		char buffer[128];
		CCFileClass file;

		Set_Scenario_Name(buffer, index, SCEN_PLAYER_NOD, SCEN_DIR_EAST, SCEN_VAR_A);
		strcat(buffer, ".INI");
		file.Set_Name(buffer);
		if (file.Is_Available()) {
			file.Read(sbuffer, 1000);
			sbuffer[1000] = '\r';
			sbuffer[1000+1] = '\n';
			sbuffer[1000+2] = '\0';

			WWGetPrivateProfileString("Basic", "Name", "x", buffer, sizeof(buffer), sbuffer);
			char * data = new char [strlen(buffer)+1+sizeof(int)+25];
			*((int*)&data[0]) = index;
			strcpy(&data[sizeof(int)], "NOD: ");
			strcat(&data[sizeof(int)], buffer);
			list.Add_Item(data);
		}
	}

	Set_Logic_Page(SeenBuff);
	bool recalc = true;
	bool display = true;
	bool process = true;
	bool okval = true;
	while (process) {

		Call_Back();

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=TRUE;
		}

		if (display) {
			display = false;

			Hide_Mouse();

			/*
			**	Load the background picture.
			*/
			Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
			Blit_Hid_Page_To_Seen_Buff();

			Dialog_Box(option_x, option_y, option_width, option_height);
			Draw_Caption(TXT_MISSION_DESCRIPTION, option_x, option_y, option_width);
			buttons->Draw_All();
			Show_Mouse();
		}

		KeyNumType input = buttons->Input();
		switch (input) {
			case KN_RETURN:
			case 200|KN_BUTTON:
				if (list.Current_Item()[sizeof(int)] == 'G') {
					ScenPlayer = SCEN_PLAYER_GDI;
				} else {
					ScenPlayer = SCEN_PLAYER_NOD;
				}
				ScenDir = SCEN_DIR_EAST;
				Whom = HOUSE_GOOD;
				Scenario = *(int *)list.Current_Item();
				process = false;
				okval = true;
				break;

			case KN_ESC:
			case 201|KN_BUTTON:
				ScenPlayer = SCEN_PLAYER_GDI;
				ScenDir = SCEN_DIR_EAST;
				Whom = HOUSE_GOOD;
				Scenario = *(int *)list.Current_Item();
				process = false;
				okval = false;
				break;

			default:
				break;
		}
	}

	/*
	**	Free up the allocations for the text lines in the list box.
	*/
	for (index = 0; index < list.Count(); index++) {
		delete [] (char *)list.Get_Item(index);
	}

	return(okval);
}












/***********************************************************************************************
 * Bonus_Dialog -- Asks the user which bonus mission he wants to play                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/26/97 11:07AM ST : Created                                                             *
 *=============================================================================================*/
bool Bonus_Dialog(void)
{
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	int	option_width 	= 236 * factor;
	int	option_height	= 162 * factor;
	int	option_x			= (320*factor - option_width) /2;
	int	option_y			= (200*factor - option_height) /2;

	GadgetClass * buttons = NULL;

	void const *up_button;
	void const *down_button;

	if (InMainLoop){
		up_button = Hires_Retrieve("BTN-UP.SHP");
		down_button = Hires_Retrieve("BTN-DN.SHP");
	}else{
		up_button = Hires_Retrieve("BTN-UP2.SHP");
		down_button = Hires_Retrieve("BTN-DN2.SHP");
	}

	TextButtonClass ok(200, TXT_OK, TPF_6PT_GRAD|TPF_NOSHADOW, option_x+25*factor, option_y+option_height-15*factor);
	TextButtonClass cancel(201, TXT_CANCEL, TPF_6PT_GRAD|TPF_NOSHADOW, option_x+option_width-50*factor, option_y+option_height-15*factor);
	EListClass list(202, option_x+10*factor, option_y+20*factor, option_width-20*factor, option_height-40*factor, TPF_6PT_GRAD|TPF_NOSHADOW, up_button, down_button);

	buttons = &ok;
	cancel.Add(*buttons);
	list.Add(*buttons);


	/*
	**	Add in all the expansion scenarios.
	*/
	char * sbuffer = (char*)_ShapeBuffer;
	int gdi_scen_names[3]={
		TXT_BONUS_MISSION_1,
		TXT_BONUS_MISSION_2,
		TXT_BONUS_MISSION_3
	};

	int nod_scen_names[2]={
		TXT_BONUS_MISSION_4,
		TXT_BONUS_MISSION_5
	};

	int index;
	for (index = 60; index < 63; index++) {
		char buffer[128];
		CCFileClass file;

		Set_Scenario_Name(buffer, index, SCEN_PLAYER_GDI, SCEN_DIR_EAST, SCEN_VAR_A);
		strcat(buffer, ".INI");
		file.Set_Name(buffer);
		if (file.Is_Available()) {
			memcpy (buffer, Text_String (gdi_scen_names[index-60]), 1+strlen(Text_String (gdi_scen_names[index-60])));
			char * data = new char [strlen(buffer)+1+sizeof(int)+25];
			*((int*)&data[0]) = index;
			strcpy(&data[sizeof(int)], "GDI: ");
			strcat(&data[sizeof(int)], buffer);
			list.Add_Item(data);
		}
	}

	for (index = 60; index < 62; index++) {
		char buffer[128];
		CCFileClass file;

		Set_Scenario_Name(buffer, index, SCEN_PLAYER_NOD, SCEN_DIR_EAST, SCEN_VAR_A);
		strcat(buffer, ".INI");
		file.Set_Name(buffer);
		if (file.Is_Available()) {
			memcpy (buffer, Text_String (nod_scen_names[index-60]), 1+strlen(Text_String (nod_scen_names[index-60])));
			char * data = new char [strlen(buffer)+1+sizeof(int)+25];
			*((int*)&data[0]) = index;
			strcpy(&data[sizeof(int)], "NOD: ");
			strcat(&data[sizeof(int)], buffer);
			list.Add_Item(data);
		}
	}

	Set_Logic_Page(SeenBuff);
	bool recalc = true;
	bool display = true;
	bool process = true;
	bool okval = true;
	while (process) {

		Call_Back();

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=TRUE;
		}

		if (display) {
			display = false;

			Hide_Mouse();

			/*
			**	Load the background picture.
			*/
			Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
			Blit_Hid_Page_To_Seen_Buff();

			Dialog_Box(option_x, option_y, option_width, option_height);
			Draw_Caption(TXT_BONUS_MISSIONS, option_x, option_y, option_width);
			buttons->Draw_All();
			Show_Mouse();
		}

		KeyNumType input = buttons->Input();
		switch (input) {
			case KN_RETURN:
			case 200|KN_BUTTON:
				if (list.Current_Item()[sizeof(int)] == 'G') {
					ScenPlayer = SCEN_PLAYER_GDI;
				} else {
					ScenPlayer = SCEN_PLAYER_NOD;
				}
				ScenDir = SCEN_DIR_EAST;
				Whom = HOUSE_GOOD;
				Scenario = *(int *)list.Current_Item();
				process = false;
				okval = true;
				break;

			case KN_ESC:
			case 201|KN_BUTTON:
				ScenPlayer = SCEN_PLAYER_GDI;
				ScenDir = SCEN_DIR_EAST;
				Whom = HOUSE_GOOD;
				Scenario = *(int *)list.Current_Item();
				process = false;
				okval = false;
				break;

			default:
				break;
		}
	}

	/*
	**	Free up the allocations for the text lines in the list box.
	*/
	for (index = 0; index < list.Count(); index++) {
		delete [] (char *)list.Get_Item(index);
	}

	return(okval);
}


#endif