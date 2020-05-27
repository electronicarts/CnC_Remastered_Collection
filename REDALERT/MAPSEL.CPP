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

/* $Header: /CounterStrike/MAPSEL.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MAPSEL.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Barry W. Green                                               *
 *                                                                                             *
 *                   Start Date : April 17, 1995                                               *
 *                                                                                             *
 *                  Last Update : April 27, 1995   [BWG]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Bit_It_In -- Pixel fade graphic copy.                                                     *
 *   Map_Selection -- Starts the whole process of selecting next map to go to                  *
 *   Print_Statistics -- Prints statistics on country selected                                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

void Cycle_Call_Back_Delay(int time, PaletteClass &pal);
extern int ControlQ;

int Mouse_Over_Spot(int house, int scenario);
void Set_Mouse(MouseType shape, int &start, int &count, int &delay, int &xspot, int &yspot);
//VG for ant mission progression
const char* antmission[] = {NULL, "SCA01EA.INI", "SCA02EA.INI", "SCA03EA.INI", "SCA04EA.INI"};

struct point {
	int x;
	int y;
} const MapCoords[2][14][3] = {
 {
 {{185,123},{ -1, -1},{ -1, -1}},
 {{173,112},{ -1, -1},{ -1, -1}},
 {{196,100},{200,112},{ -1, -1}},
 {{175,113},{ -1, -1},{ -1, -1}},
 {{187, 91},{202, 93},{206,105}},
 {{207,161},{212,172},{ -1, -1}},
 {{172, 92},{ -1, -1},{ -1, -1}},
 {{132,119},{146,125},{ -1, -1}},
 {{199, 73},{205, 86},{ -1, -1}},
 {{236,114},{ -1, -1},{ -1, -1}},
 {{219, 64},{225, 76},{ -1, -1}},
 {{256, 69},{ -1, -1},{ -1, -1}},
 {{262, 77},{ -1, -1},{ -1, -1}},
 {{249, 97},{ -1, -1},{ -1, -1}}
 },
// Soviet coords
 {
 {{178,105},{ -1, -1},{ -1, -1}},
 {{163,101},{163,113},{ -1, -1}},
 {{160, 89},{ -1, -1},{ -1, -1}},
 {{142,101},{142,117},{ -1, -1}},
 {{212,163},{ -1, -1},{ -1, -1}},
 {{155,133},{171,144},{ -1, -1}},
 {{216,103},{ -1, -1},{ -1, -1}},
 {{132,145},{154,154},{ -1, -1}},
 {{122,117},{ -1, -1},{ -1, -1}},
 {{117,130},{ -1, -1},{ -1, -1}},
 {{ 99,107},{109,146},{ -1, -1}},
 {{134,125},{ -1, -1},{ -1, -1}},
 {{ 32,156},{ 46,171},{ -1, -1}},
 {{108, 97},{ -1, -1},{ -1, -1}}
 }
};

#ifndef WIN32
extern short StreamLowImpact;
#endif  //WIN32

/***********************************************************************************************
 * Map_Selection -- Starts the whole process of selecting next map to go to                    *
 *                                                                                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
extern int CopyType;
#ifndef WIN32
extern short StreamLowImpact;
#endif
char const * Map_Selection(void)
{
	return NULL;
#if (0)//PG
	static char scenarioname[_MAX_FNAME+_MAX_EXT];

#ifdef FIXIT_ANTS
	if (AntsEnabled)  {
		strcpy(scenarioname, Scen.ScenarioName);
		char buf[10];
		sprintf(buf, "%02d", Scen.Scenario+1);
		memcpy(&scenarioname[3], buf, 2);
		return(scenarioname);
	}

#endif
	char _filename[]="MSAA.WSA";
	int house = (PlayerPtr->Class->House == HOUSE_USSR || PlayerPtr->Class->House == HOUSE_UKRAINE);

	_filename[2] = house ? 'S' : 'A';
	_filename[3] = Scen.Scenario + 'A';
	PaletteClass mappalette;

	int scenario = Scen.Scenario;
	int selection;
	static CDTimerClass<SystemTimerClass> timer;
	int start = 0;
	int count = 0;
	int delay = 0;
	int xspot = 0;
	int yspot = 0;

	void const * appear1 =  MFCD::Retrieve("MAPWIPE2.AUD");
	void const * bleep11  = MFCD::Retrieve("BLEEP11.AUD");
	void const * country4 = MFCD::Retrieve("MAPWIPE5.AUD");
	void const * toney7 =   MFCD::Retrieve("TONEY7.AUD");
	void const * bleep17  = MFCD::Retrieve("BLEEP17.AUD");

	void const * scold1 =   MFCD::Retrieve("TONEY4.AUD");
	void const * country1 = MFCD::Retrieve("TONEY10.AUD");

#ifdef WIN32
	GraphicBufferClass *pseudoseenbuff = new GraphicBufferClass(320, 200, (void*)NULL);
#endif

//	fixed oldvolume = Options.ScoreVolume;
//	Options.Set_Score_Volume(fixed(4, 10));
	Theme.Queue_Song(THEME_MAP);

	void *anim = Open_Animation(_filename, NULL, 0L, (WSAOpenType)(WSA_OPEN_FROM_MEM | WSA_OPEN_TO_PAGE), mappalette);

	Keyboard->Clear();
	SeenPage.Clear();
	mappalette.Set(FADE_PALETTE_FAST, Call_Back);

#ifdef WIN32
	pseudoseenbuff->Clear();
	Animate_Frame(anim, *pseudoseenbuff, 1);
	for(int x=0; x<256; x++) memset(&PaletteInterpolationTable[x][0],x,256);
	CopyType = 1;
	Interpolate_2X_Scale(pseudoseenbuff , &SeenBuff , 0);
#else
	HidPage.Clear();
	Animate_Frame(anim, HidPage, 1);
	HidPage.Blit(SeenPage);
#endif


	int frame = 1;
	StreamLowImpact = true;
#ifdef WIN32
	Play_Sample(appear1, 255, Options.Normalize_Volume(170));
#else
	Play_Sample(appear1, 255, Options.Normalize_Volume(55));
#endif
	while (frame < Get_Animation_Frame_Count(anim)) {
#ifdef WIN32
		CopyType = 1;
		Animate_Frame(anim, *pseudoseenbuff, frame++);
		Interpolate_2X_Scale(pseudoseenbuff , &SeenBuff , NULL);
		CopyType = 0;
#else
		Animate_Frame(anim, SeenPage, frame++);
#endif
		Call_Back_Delay(2);
		switch(frame) {
			case 16:
#ifdef WIN32
				Play_Sample(bleep11, 255, Options.Normalize_Volume(170));
#else
				Play_Sample(bleep11, 255, Options.Normalize_Volume(55));
#endif
				break;
			case 30:
#ifdef WIN32
				Play_Sample(country4, 255, Options.Normalize_Volume(170));
#else
				Play_Sample(country4, 255, Options.Normalize_Volume(55));
#endif
				break;
			case 51:
#ifdef WIN32
				Play_Sample(toney7, 255, Options.Normalize_Volume(170));
#else
				Play_Sample(toney7, 255, Options.Normalize_Volume(55));
#endif
				break;
			case 61:
#ifdef WIN32
				Play_Sample(bleep17, 255, Options.Normalize_Volume(170));
#else
				Play_Sample(bleep17, 255, Options.Normalize_Volume(55));
#endif
				break;
		}
	}
	StreamLowImpact = false;
	Call_Back();
	Close_Animation(anim);
	Show_Mouse();
	Keyboard->Clear();

	bool done = 0;
	MouseType shape = MOUSE_NORMAL;
	while (!done) {
#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			CopyType = 1;
			Interpolate_2X_Scale(pseudoseenbuff , &SeenBuff , NULL);
			CopyType = 0;
		}
#endif
		Cycle_Call_Back_Delay(1, mappalette);
		int choice = Mouse_Over_Spot(house, scenario);
		if (choice == -1) {
			shape = MOUSE_NORMAL;
		} else {
			shape = MOUSE_CAN_ATTACK;
		}

		Set_Mouse(shape, start, count, delay, xspot, yspot);
		if (timer == 0) {
			frame++;
			frame %= count;
			timer = delay;
			Set_Mouse_Cursor(xspot, yspot, Extract_Shape(MouseClass::MouseShapes, start + frame));
		}
		if (Keyboard->Check()) {
			if ((Keyboard->Get() & 0x10FF) == KN_LMOUSE) {
				if (choice != -1) {
					done = 1;
					selection = choice;
#ifdef WIN32
					Play_Sample(country1, 255, Options.Normalize_Volume(170));
#else
					Play_Sample(country1, 255, Options.Normalize_Volume(50));
#endif
				} else {
#ifdef WIN32
					Play_Sample(scold1, 255, Options.Normalize_Volume(170));
#else
					Play_Sample(scold1, 255, Options.Normalize_Volume(50));
#endif
				}
			}
		}
	}

	Hide_Mouse();

	/*
	** Restore the mouse to normal shape before leaving this routine.
	*/
	Set_Mouse(MOUSE_NORMAL, start, count, delay, xspot, yspot);
	Set_Mouse_Cursor(xspot, yspot, Extract_Shape(MouseClass::MouseShapes, start));

	Keyboard->Clear();
//	BlackPalette.Set(FADE_PALETTE_SLOW, Call_Back);
//	SeenPage.Clear();

	Fancy_Text_Print(TXT_STAND_BY, 160 * RESFACTOR, 190 * RESFACTOR, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_CENTER|TPF_6PT_GRAD|TPF_DROPSHADOW);

	/*
	**	Create the new scenario filename from the selection. The filename is
	**	derived from the previous filename but it has the scenario number
	**	incremented and the chosen variation set.
	*/

	//V.G. added so Ant Missions would progress
	if(Scen.ScenarioName[2] == 'A'){
	   int antnum = Scen.Scenario++;	
	   if(antnum > 4) antnum = 1;	
	   strcpy(scenarioname, antmission[antnum]);
	}
	else{
		strcpy(scenarioname, Scen.ScenarioName);
		char buf[10];
		sprintf(buf, "%02d", Scen.Scenario+1);
		memcpy(&scenarioname[3], buf, 2);
		scenarioname[6] = 'A' + selection;
	}
	Theme.Fade_Out();
//	Options.Set_Score_Volume(oldvolume);

//	Scen.ScenVar = (ScenarioVarType)selection;
//Mono_Printf("Chose variant %d  \n", selection);
	return(scenarioname);
#endif
}

int Mouse_Over_Spot(int house, int scenario)
{
	int retval = -1;
	for (int selection = 0; selection < 3 && MapCoords[house][scenario][selection].x != -1; selection++) {
		int mousex = Get_Mouse_X() / RESFACTOR;
		int mousey = Get_Mouse_Y() / RESFACTOR;
		if (mousex >= MapCoords[house][scenario][selection].x &&
			 mousey >= MapCoords[house][scenario][selection].y &&
			 mousex <= MapCoords[house][scenario][selection].x+11 &&
			 mousey <= MapCoords[house][scenario][selection].y+9) {

			retval = selection;
			break;
		}
	}
	return(retval);
}
void Cycle_Call_Back_Delay(int time, PaletteClass &pal)
{
	static CDTimerClass<SystemTimerClass> _ftimer;
	static bool _up = false;
	static int val = 255;

	while(time--) {
		/*
		**	Process the fading white color.
		*/
		if (!_ftimer) {
			_ftimer = TIMER_SECOND/6;

			#define	STEP_RATE	20
			if (_up) {
				val += STEP_RATE;
				if (val > 150) {
					val = 150;
					_up = false;
				}
			} else {
				val -= STEP_RATE;
				if (val < 0x20) {
					val = 0x20;
					_up = true;
				}
			}

			/*
			**	Set the pulse color as the proportional value between white and the
			**	minimum value for pulsing.
			*/
			pal[254] = GamePalette[WHITE];
			pal[254].Adjust(val, BlackColor);

			pal.Set();
		}
		Call_Back_Delay(1);
	}
}

void Set_Mouse(MouseType shape, int &start, int &count, int &delay, int &xspot, int &yspot)
{
	switch(shape) {
		case MOUSE_NORMAL:
			start = 0;
			count = 1;
			delay = 0;
			xspot = 0;
			yspot = 0;
			break;
		default:
			start = 21;
			count = 8;
			delay = 4;
			xspot = 14;
			yspot = 11;
			break;
	}
}
