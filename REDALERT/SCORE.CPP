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

/* $Header: /counterstrike/SCORE.CPP 3     3/14/97 12:02a Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SCORE.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 19, 1994                                               *
 *                                                                                             *
 *                  Last Update : May 3, 1995   [BWG]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Call_Back_Delay -- Combines Call_Back() and Delay() functions                             *
 *   Draw_Bar_Graphs -- Draw "Casualties" bar graphs                                           *
 *   Draw_InfantryMan -- Draw one guy in score screen, update animation                        *
 *   Draw_Infantrymen -- Draw all the guys on the score screen                                 *
 *   New_Infantry_Anim -- Start up a new animation for one of the infantrymen                  *
 *   ScoreClass::Count_Up_Print -- Prints a number (up to its max) into a string, cleanly      *
 *   ScoreClass::DO_GDI_GRAPH -- Show # of people or buildings killed on GDI score screen      *
 *   ScoreClass::Delay -- Pauses waiting for keypress.                                         *
 *   ScoreClass::Presentation -- Main routine to display score screen.                         *
 *   ScoreClass::Print_Graph_Title -- Prints title on score screen.                            *
 *   ScoreClass::Print_Minutes -- Print out hours/minutes up to max                            *
 *   ScoreClass::Pulse_Bar_Graph -- Pulses the bargraph color.                                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef WIN32
extern short StreamLowImpact;
#endif

#include	"function.h"

#define SCORETEXT_X		184
#define SCORETEXT_Y		8
#define CASUALTY_Y		88
#define BUILDING_X		256
#define BUILDING_Y		128
#define BARGRAPH_X		266
#define MAX_BAR_X			318		// max possible is 319 because of bar's right shadow
#define SIZEGBAR			118
#define HALLFAME_X		11
#define HALLFAME_Y		120

#define	MULTISCOREX		30

#define TEDIT_FAME				1
#define NUMINFANTRYMEN			10
#define NUMFAMENAMES				7
#define MAX_FAMENAME_LENGTH	11

#ifndef WIN32
extern short StreamLowImpact;
#endif  //WIN32

GraphicBufferClass *PseudoSeenBuff;

struct InfantryAnim {
	int xpos;
	int ypos;
	void const *shapefile;
	void const *remap;
	int anim;
	int stage;
	char delay;
	InfantryTypeClass const *Class;
} InfantryMan[NUMINFANTRYMEN];
void Draw_InfantryMen(void);
void Draw_InfantryMan(int index);
void New_Infantry_Anim(int index, int anim);
void Draw_Bar_Graphs(int i, int gkilled, int nkilled);
void Animate_Cursor(int pos, int ypos);
void Animate_Score_Objs(void);
void Cycle_Wait_Click(bool cycle=true);

#ifdef FIXIT_SCORE_CRASH
//void Disable_Uncompressed_Shapes (void);
//void Enable_Uncompressed_Shapes (void);
#endif	//FIXIT

void const * Beepy6;
int ControlQ;	// cheat key to skip past score/mapsel screens
bool StillUpdating;

#ifdef WIN32
char *ScreenNames[2]={"ALIBACKH.PCX", "SOVBACKH.PCX"};
#else
char *ScreenNames[2]={"ALI-TRAN.WSA", "SOV-TRAN.WSA"};
#endif

//#ifdef WIN32
//TextBlitClass BlitList;
//#endif


struct Fame {
	char	name[MAX_FAMENAME_LENGTH];
	int	score;
	int	level;
	int	side;
};

ScoreAnimClass *ScoreObjs[MAXSCOREOBJS];


ScoreAnimClass::ScoreAnimClass(int x, int y, void const * data)
{
	XPos = x * RESFACTOR;
	YPos = y * RESFACTOR;
	Timer = 0;
	DataPtr = data;
}


ScoreTimeClass::ScoreTimeClass(int xpos, int ypos, void const * data, int maxval, int xtimer) :
	ScoreAnimClass(xpos, ypos, data)
{
	Stage = 0;
	MaxStage = maxval;
	TimerReset = xtimer;
}

void ScoreTimeClass::Update(void)
{
#ifdef WIN32
	GraphicViewPortClass *oldpage;
#else
	GraphicBufferClass *oldpage;
#endif
	if (!Timer) {
		Timer = TimerReset;
		if (++Stage >= MaxStage) Stage = 0;
		oldpage = LogicPage;
		Set_Logic_Page(SeenBuff);
		CC_Draw_Shape(DataPtr, Stage, XPos, YPos, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
#ifdef WIN32
		Set_Logic_Page(*PseudoSeenBuff);
		CC_Draw_Shape(DataPtr, Stage, XPos, YPos, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
#endif
		Set_Logic_Page(oldpage);
	}
}

ScoreCredsClass::ScoreCredsClass(int xpos, int ypos, void const * data, int maxval, int xtimer) :
	ScoreAnimClass(xpos, ypos, data)
{
	Stage = 0;
	MaxStage = maxval;
	TimerReset = xtimer;
	Clock1 = MFCD::Retrieve("CLOCK1.AUD");
	CashTurn = MFCD::Retrieve("CASHTURN.AUD");
}


void ScoreCredsClass::Update(void)
{
#ifdef WIN32
	GraphicViewPortClass *oldpage;
#else
	GraphicBufferClass *oldpage;
#endif
	if (!Timer) {
		Timer = TimerReset;
		if (++Stage >= MaxStage) Stage = 0;
		oldpage = LogicPage;
		Set_Logic_Page(SeenBuff);
#ifdef WIN32
		Play_Sample(Clock1, 255, Options.Normalize_Volume(130));
#else
		Play_Sample(Clock1, 255, Options.Normalize_Volume(50));
#endif
		CC_Draw_Shape(DataPtr, Stage, XPos, YPos, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
#ifdef WIN32
		Set_Logic_Page(*PseudoSeenBuff);
		CC_Draw_Shape(DataPtr, Stage, XPos, YPos, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
#endif
		Set_Logic_Page(oldpage);
	}
}


ScorePrintClass::ScorePrintClass(int string, int xpos, int ypos, void const * palette, int background) :
	ScoreAnimClass(xpos, ypos, Text_String(string))
{
	Background = background;
	PrimaryPalette = palette;
	Stage = 0;
}


ScorePrintClass::ScorePrintClass(void const * string, int xpos, int ypos, void const * palette, int background) :
	ScoreAnimClass(xpos, ypos, string)
{
	Background = background;
	PrimaryPalette = palette;
	Stage = 0;
}


void ScorePrintClass::Update(void)
{
	static char localstr[2]={0,0};
	static char _whitepal[]={0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F};

	if (Stage && (((char *)DataPtr)[Stage-1]==0) ) {
		for (int i = 0; i < MAXSCOREOBJS; i++) {
			if (ScoreObjs[i] == this) {
				ScoreObjs[i] = 0;
			}
		}
		delete this;
		return;
	}

#ifdef WIN32
	StillUpdating = true;
#endif
	if (!Timer) {
		Timer = 1;

		int pos = XPos+(Stage*(6*RESFACTOR));
// print the letter properly
		if (Stage) {
			Set_Font_Palette(PrimaryPalette);
			localstr[0]=((char *)DataPtr)[Stage-1];
			HidPage.Print(localstr, pos-6*RESFACTOR, YPos,   TBLACK, TBLACK);
			HidPage.Blit(SeenPage, pos-6*RESFACTOR, YPos-1*RESFACTOR, pos-6*RESFACTOR, YPos-1*RESFACTOR, 7*RESFACTOR, 8*RESFACTOR);
#ifdef WIN32
			HidPage.Blit(*PseudoSeenBuff, pos-6*RESFACTOR, YPos-1*RESFACTOR, pos-6*RESFACTOR, YPos-1*RESFACTOR, 7*RESFACTOR, 8*RESFACTOR);
			PseudoSeenBuff->Print(localstr, pos-6*RESFACTOR, YPos,   TBLACK, TBLACK);
#endif
		}
		if (((char *)DataPtr)[Stage]) {
			localstr[0]=((char *)DataPtr)[Stage];
			Set_Font_Palette(_whitepal);
			SeenPage.Print(localstr, pos,  YPos-1, TBLACK, TBLACK);
			SeenPage.Print(localstr, pos,  YPos+1, TBLACK, TBLACK);
			SeenPage.Print(localstr, pos+1, YPos  , TBLACK, TBLACK);
#ifdef WIN32
			PseudoSeenBuff->Print(localstr, pos,  YPos-1, TBLACK, TBLACK);
			PseudoSeenBuff->Print(localstr, pos,  YPos+1, TBLACK, TBLACK);
			PseudoSeenBuff->Print(localstr, pos+1, YPos  , TBLACK, TBLACK);
#endif
		}
		Stage++;
	}
}


ScoreScaleClass::ScoreScaleClass(void const * string, int xpos, int ypos, char const palette[]) :
	ScoreAnimClass(xpos, ypos, string)
{
	Palette = &palette[0];
#ifdef WIN32
	Stage = 0;
#else
	Stage = 5;
#endif
}


void ScoreScaleClass::Update(void)
{
	static int _destx[]={0,80,107,134,180,228};
	static int _destw[]={6,20, 30, 40, 60, 80};

	/*
	** Restore the background for the scaled-up letter
	*/
	if (!Timer) {
		Timer = 1;
#ifndef WIN32
		if (Stage != 5) {
			int destx = _destx[Stage+1]*RESFACTOR;
			int destw = _destw[Stage+1]*RESFACTOR;
			HidPage.Blit(SeenPage, destx, YPos, destx, YPos, (destx + destw) <= 320 * RESFACTOR ? destw : (320 * RESFACTOR) - destx, (YPos + destw) <= 200 * RESFACTOR ? destw : (200 * RESFACTOR) - YPos);
		}
#endif
		if (Stage) {
			Set_Font_Palette(Palette);
			HidPage.Fill_Rect(0, 0, 7*RESFACTOR, 7*RESFACTOR, TBLACK);
			HidPage.Print((char *)DataPtr, 0, 0,   TBLACK, TBLACK);
			HidPage.Scale(SeenPage, 0, 0, _destx[Stage]*RESFACTOR, YPos, 5*RESFACTOR, 6*RESFACTOR, _destw[Stage]*RESFACTOR, _destw[Stage]*RESFACTOR, true);
			Stage--;
		} else {
			Set_Font_Palette(Palette);
			for (int i = 0; i < MAXSCOREOBJS; i++) {
				if (ScoreObjs[i]==this) ScoreObjs[i] = 0;
			}
			HidPage.Print((char *)DataPtr, XPos, YPos,   TBLACK, TBLACK);
			HidPage.Blit(SeenPage, XPos, YPos, XPos, YPos, 6*RESFACTOR, 6*RESFACTOR);
#ifdef WIN32
			HidPage.Blit(*PseudoSeenBuff, XPos, YPos, XPos, YPos, 6*RESFACTOR, 6*RESFACTOR);
#endif
			delete this;
			return;
		}
	}
}

int Alloc_Object(ScoreAnimClass *obj)
{
	int i,ret;

	for (i = ret = 0; i < MAXSCOREOBJS; i++) {
		if (!ScoreObjs[i]) {
			ScoreObjs[i] = obj;
			ret = i;
			break;
		}
	}
	return(ret);
}



/***********************************************************************************************
 * ScoreClass::Presentation -- Main routine to display score screen.                           *
 *                                                                                             *
 *    This is the main routine that displays the score screen graphics.                        *
 *    It gets called at the end of each scenario and is used to present                        *
 *    the results and a rating of the player's battle.                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994     : Created.                                                                 *
 *=============================================================================================*/
static unsigned char const  _bluepal[]={0xC0,0xC1,0xC1,0xC3,0xC2,0xC5,0xC3,0xC7,0xC4,0xC9,0xCA,0xCB,0xCC,0xCD,0xC0,0xCF};
static unsigned char const _greenpal[]={0x70,0x71,0x7C,0x73,0x7D,0x75,0x7E,0x77,0x7F,0x79,0x7A,0x7B,0x7C,0x7D,0x7C,0x7F};
static unsigned char const   _redpal[]={0xD0,0xD1,0xD7,0xD3,0xD9,0xD5,0xDA,0xD7,0xDB,0xD9,0xDA,0xDB,0xDC,0xDD,0xD6,0xDF};
static unsigned char const _yellowpal[]={0x0,0x0,0xEC,0x0,0xEB,0x0,0xEA,0x0,0xE9,0x0,0x0,0x0,0x0,0x0,0xED,0x0};
void ScoreClass::Presentation(void)
{
#if (0)//PG
#ifdef WIN32
//	if (Keyboard != NULL) return;
#endif
	static int const _casuax[2]={144,150};
	static int const _casuay[2]={ 78, 78};
	static int const _gditxy[2]={ 90, 90};

#if defined(FRENCH) || defined(GERMAN)
	static int const _gditxx[2]={130,150};
	static int const _nodtxx[2]={130,150};
#else
	static int const _gditxx[2]={135,150};
	static int const _nodtxx[2]={135,150};
#endif
	static int const _nodtxy[2]={102,102};
	static int const _bldggy[2]={138,138};
	static int const _bldgny[2]={150,150};

#ifdef WIN32
#ifdef FIXIT_SCORE_CRASH
	/*
	** Fix for the score screen crash due to uncompressed shape buffer overflow.
	*/
	Disable_Uncompressed_Shapes();
#endif	//FIXIT
	PseudoSeenBuff = new GraphicBufferClass(SeenBuff.Get_Width(),SeenBuff.Get_Height(),(void*)NULL);
#endif
	int i;
	void const * yellowptr;
	void const * redptr;
	CCFileClass file(FAME_FILE_NAME);
	struct Fame hallfame[NUMFAMENAMES];
	void *oldfont;
	int oldfontxspacing = FontXSpacing;
	int house = (PlayerPtr->Class->House == HOUSE_USSR || PlayerPtr->Class->House == HOUSE_UKRAINE);		// 0 or 1
#ifdef WIN32
	char inter_pal[15];
	sprintf(inter_pal, "SCORPAL1.PAL");
#endif

	ControlQ = 0;
	FontXSpacing = 0;
	Map.Override_Mouse_Shape(MOUSE_NORMAL);
	Theme.Queue_Song(THEME_SCORE);

#ifdef WIN32
	VisiblePage.Clear();
	SysMemPage.Clear();
	WWMouse->Erase_Mouse(&HidPage, TRUE);
	HiddenPage.Clear();
	Set_Logic_Page(SysMemPage);
#else
	SeenPage.Clear();
	HidPage.Clear();
	Set_Logic_Page(HidPage);
#endif
	BlackPalette.Set();


	void const * country4 = MFCD::Retrieve("COUNTRY4.AUD");
	void const * sfx4 = MFCD::Retrieve("SFX4.AUD");
	Beepy6 = MFCD::Retrieve("BEEPY6.AUD");

	/*
	** Load the background for the score screen
	*/
#ifndef WIN32
	void *anim = Open_Animation(ScreenNames[house], NULL, 0L, (WSAOpenType)(WSA_OPEN_FROM_MEM | WSA_OPEN_TO_PAGE), ScorePalette);
#endif

	unsigned minutes = (unsigned)((ElapsedTime / (long)TIMER_MINUTE))+1;

// Load up the shapes for the Nod score screen
#ifdef WIN32
	yellowptr = MFCD::Retrieve("BAR3BHR.SHP");
	redptr = MFCD::Retrieve("BAR3RHR.SHP");
#else
	if (!house) {
		yellowptr = MFCD::Retrieve("BAR3BLU.SHP");
		redptr = MFCD::Retrieve("BAR3RED.SHP");
	}
#endif

/* Change to the six-point font for Text_Print */
	oldfont = Set_Font(ScoreFontPtr);
	Call_Back();

/* --- Now display the background animation --- */
	Hide_Mouse();
#ifdef WIN32
	Load_Title_Screen(ScreenNames[house], &HidPage, ScorePalette);
	Increase_Palette_Luminance (ScorePalette , 30, 30, 30, 63);
	HidPage.Blit(SeenPage);
	HidPage.Blit(*PseudoSeenBuff);
#else
	Animate_Frame(anim, HidPage, 1);
	HidPage.Blit(SeenPage);
#endif
	ScorePalette.Set(FADE_PALETTE_FAST, Call_Back);
#ifdef WIN32
	Play_Sample(country4, 255, Options.Normalize_Volume(150));
#else
	Play_Sample(country4, 255, Options.Normalize_Volume(60));
#endif

#ifndef WIN32
	int frame = 1;
	StreamLowImpact = true;
	while (frame < Get_Animation_Frame_Count(anim)) {
		Animate_Frame(anim, SeenPage, frame++);
		Call_Back_Delay(2);
	}
	StreamLowImpact = false;
	Call_Back();
	Close_Animation(anim);
#endif

	/*
	** Background's up, so now load various shapes and animations
	*/
#ifdef WIN32
	void const * timeshape = 	  MFCD::Retrieve("TIMEHR.SHP");
	void const * hiscore1shape = MFCD::Retrieve("HISC1-HR.SHP");
	void const * hiscore2shape = MFCD::Retrieve("HISC2-HR.SHP");
#else
	void const * timeshape = 	  MFCD::Retrieve("TIME.SHP");
	void const * hiscore1shape = MFCD::Retrieve("HISCORE1.SHP");
	void const * hiscore2shape = MFCD::Retrieve("HISCORE2.SHP");
#endif
	ScoreObjs[0] = new ScoreTimeClass(238, 2, timeshape, 30, 4);
	ScoreObjs[1] = new ScoreTimeClass(4, 89, hiscore1shape, 10, 4);
	ScoreObjs[2] = new ScoreTimeClass(4, 180, hiscore2shape, 10, 4);

	/* Now display the stuff */
#ifdef WIN32
#else
	SeenPage.Blit(HidPage);
#endif
	Set_Logic_Page(SeenBuff);


#ifdef FRENCH
	Alloc_Object(new ScorePrintClass(TXT_SCORE_TIME, 198,  9, _greenpal));
#else
	Alloc_Object(new ScorePrintClass(TXT_SCORE_TIME, 204,  9, _greenpal));
#endif
	Alloc_Object(new ScorePrintClass(TXT_SCORE_LEAD, 164, 26, _greenpal));
	Alloc_Object(new ScorePrintClass(TXT_SCORE_EFFI, 164, 38, _greenpal));
	Alloc_Object(new ScorePrintClass(TXT_SCORE_TOTA, 164, 50, _greenpal));
#ifdef WIN32
	Play_Sample(sfx4, 255, Options.Normalize_Volume(150));
#else
	Play_Sample(sfx4, 255, Options.Normalize_Volume(60));
#endif
	Call_Back_Delay(13);

	int scorecounter = 0;

	Keyboard->Clear();

	/*
	**	Determine leadership rating.
	*/
	int leadership = 0;
	for (int index = 0; index < Logic.Count(); index++) {
		ObjectClass * object = Logic[index];
		HousesType owner = object->Owner();
		if ( (house) && (owner == HOUSE_USSR || owner == HOUSE_BAD || owner == HOUSE_UKRAINE) ) {
			leadership++;
		} else {
			if ( (!house) && (object->Owner() == HOUSE_GREECE) ) {
				leadership++;
			}
		}
	}
	int uspoints = 0;

	for (HousesType hous = HOUSE_SPAIN; hous <= HOUSE_BAD; hous++) {
		HouseClass *hows = HouseClass::As_Pointer(hous);
		if (hous == HOUSE_USSR || hous == HOUSE_BAD || hous == HOUSE_UKRAINE) {
			NKilled += hows->UnitsLost;
			NBKilled += hows->BuildingsLost;
		} else {
			GKilled += hows->UnitsLost;
			GBKilled += hows->BuildingsLost;
		}
		if (PlayerPtr->Is_Ally(hous) ) {
			uspoints += hows->PointTotal;
		}
	}
//	if(uspoints < 0) uspoints = 0;
//	uspoints += 1000; //BG 1000 bonus points for winning mission

	/*
	**	Bias the base score upward according to the difficulty level.
	*/
	switch (PlayerPtr->Difficulty) {
		case DIFF_EASY:
			uspoints += 500;
			break;

		case DIFF_NORMAL:
			uspoints += 1500;
			break;

		case DIFF_HARD:
			uspoints += 3500;
			break;
	}


	if (!leadership) leadership++;
	leadership = 100*fixed(leadership, (house ? NKilled+NBKilled+leadership : GKilled+GBKilled+leadership));
	leadership = min(150,leadership);

	/*
	**	Determine economy rating.
	*/
	int init = PlayerPtr->Control.InitialCredits;
	int cred = PlayerPtr->Available_Money();

	int economy = 100*fixed((unsigned)PlayerPtr->Available_Money()+1+PlayerPtr->StolenBuildingsCredits, PlayerPtr->HarvestedCredits + (unsigned)PlayerPtr->Control.InitialCredits+1);
	economy=min(economy,150);

	int total = ((uspoints * leadership) / 100) + ((uspoints * economy) / 100);
	if (total < -9999) total = -9999;
	total = min(total, 99999);

Keyboard->Clear();
	for (i = 0; i <= 130; i++) {
		Set_Font_Palette(_greenpal);
		int lead = (leadership * i) / 100;
		Count_Up_Print("%3d%%", lead, leadership,     244, 26);
		if (i>=30) {
			int econo = (economy * (i-30)) / 100;
			Count_Up_Print("%3d%%", econo, economy, 244, 38);
		}
		Print_Minutes(minutes);
		Call_Back_Delay(1);
#ifdef WIN32
		Play_Sample(Beepy6, 255, Options.Normalize_Volume(100));
#else
		Play_Sample(Beepy6, 255, Options.Normalize_Volume(40));
#endif
		if ( (i >= 30) && (i >= leadership) && ((i-30) >= economy) ) break;
//BG		if (Keyboard->Check()) break;
	}
	Count_Up_Print("%3d%%", leadership, leadership, 244, 26);
	Count_Up_Print("%3d%%", economy,    economy,    244, 38);

	char buffer[16];
	sprintf(buffer, "x %5d",uspoints);
	Alloc_Object(new ScorePrintClass(buffer, 274,  26, _greenpal));
	Alloc_Object(new ScorePrintClass(buffer, 274,  38, _greenpal));
	Call_Back_Delay(8);
	SeenBuff.Draw_Line(274*RESFACTOR, 48*RESFACTOR, 313*RESFACTOR, 48*RESFACTOR, WHITE);
	Call_Back_Delay(1);
	SeenBuff.Draw_Line(274*RESFACTOR, 48*RESFACTOR, 313*RESFACTOR, 48*RESFACTOR, GREEN);

	sprintf(buffer,"%5d", total);
	Alloc_Object(new ScorePrintClass(buffer, 286,  50, _greenpal));

//BG	if (!Keyboard->Check()) {
		Call_Back_Delay(60);
//BG	}

	if (house) Show_Credits(house, _greenpal);

/*BG	if (!Keyboard->Check()) */ Call_Back_Delay(60);

	/*
	** Show stats on # of units killed
	*/
	Set_Logic_Page(SeenBuff);
#ifdef WIN32
	Play_Sample(sfx4, 255, Options.Normalize_Volume(150));
#else
	Play_Sample(sfx4, 255, Options.Normalize_Volume(60));
#endif
	int indx = house;
#ifdef WIN32
	indx = 0;
#endif
	Alloc_Object(new ScorePrintClass(TXT_SCORE_CASU, _casuax[indx], _casuay[indx], _greenpal));
	Call_Back_Delay(9);
	if (house) {
		Alloc_Object(new ScorePrintClass(TXT_SOVIET, _nodtxx[indx], _gditxy[indx], _redpal));
		Alloc_Object(new ScorePrintClass(TXT_ALLIES, _gditxx[indx], _nodtxy[indx], _bluepal));
	} else {
		Alloc_Object(new ScorePrintClass(TXT_ALLIES, _gditxx[indx], _gditxy[indx], _bluepal));
		Alloc_Object(new ScorePrintClass(TXT_SOVIET, _nodtxx[indx], _nodtxy[indx], _redpal));
	}
	Call_Back_Delay(6);

	Set_Font_Palette(_redpal);
#ifdef WIN32
		Do_GDI_Graph(yellowptr, redptr, GKilled + CKilled, NKilled, 89);
#else
	if (house) {
		Do_Nod_Casualties_Graph();
	} else {
		Do_GDI_Graph(yellowptr, redptr, GKilled + CKilled, NKilled, 89);
	}
#endif

	Set_Logic_Page(SeenBuff);

	/*
	** Print out stats on buildings destroyed
	*/
#ifdef WIN32
	Play_Sample(sfx4, 255, Options.Normalize_Volume(150));
#else
	Play_Sample(sfx4, 255, Options.Normalize_Volume(60));
#endif
#ifdef WIN32
		Alloc_Object(new ScorePrintClass(TXT_SCORE_BUIL, 144, 126, _greenpal));
		Call_Back_Delay(9);
#else
	if (!house) {
		Alloc_Object(new ScorePrintClass(TXT_SCORE_BUIL, 144, 126, _greenpal));
		Call_Back_Delay(9);
	} else {
		Alloc_Object(new ScorePrintClass(TXT_SCORE_BUIL1, 150, 118, _greenpal));
		Alloc_Object(new ScorePrintClass(TXT_SCORE_BUIL2, 150, 126, _greenpal));
		Call_Back_Delay(13);
	}
#endif
	if(house) {
		Alloc_Object(new ScorePrintClass(TXT_SOVIET, _gditxx[indx], _bldggy[indx], _redpal));
		Alloc_Object(new ScorePrintClass(TXT_ALLIES, _gditxx[indx], _bldgny[indx], _bluepal));
	} else {
		Alloc_Object(new ScorePrintClass(TXT_ALLIES, _gditxx[indx], _bldggy[indx], _bluepal));
		Alloc_Object(new ScorePrintClass(TXT_SOVIET, _gditxx[indx], _bldgny[indx], _redpal));
	}
	Call_Back_Delay(7);
#ifdef WIN32
		Do_GDI_Graph(yellowptr, redptr, GBKilled + CBKilled, NBKilled, 137);
#else
	if (house) {
		Call_Back_Delay(6);
		Set_Font_Palette(_greenpal);
		Do_Nod_Buildings_Graph();
	} else {
		Do_GDI_Graph(yellowptr, redptr, GBKilled + CBKilled, NBKilled, 137);
	}
#endif

#ifdef WIN32
	// Wait for text printing to complete
	while (StillUpdating) {
		Call_Back_Delay(1);
	}
#endif

	Keyboard->Clear();

	if (!house) Show_Credits(house, _greenpal);
	/*
	** Hall of fame display and processing
	*/
#ifdef WIN32
	Play_Sample(sfx4, 255, Options.Normalize_Volume(150));
#else
	Play_Sample(sfx4, 255, Options.Normalize_Volume(60));
#endif
	Alloc_Object(new ScorePrintClass(TXT_SCORE_TOP, 28, 110, _greenpal));
	Call_Back_Delay(9);

	/*
	** First check for the existence of the file, and if there isn't one,
	** make a new one filled with blanks.
	*/
	if (!file.Is_Available()) {

		// hall of fame doesn't exist, so blank it out & write it
		file.Open(WRITE);

		for (i = 0; i < NUMFAMENAMES; i++) {
			hallfame[i].name[0] =
			hallfame[i].score   =
			hallfame[i].level   = 0;
			hallfame[i].side	  = 0;
			file.Write(&hallfame[i], sizeof(struct Fame));
		}

		file.Close();
	}

	file.Open(READ);
	for (i = 0; i < NUMFAMENAMES; i++) {
		file.Read(&hallfame[i], sizeof(struct Fame));
	}
	file.Close();

	/*
	** If the player's score is good enough to bump someone off the list,
	** remove their data, move everyone down a notch, and set index = where
	** their info goes
	*/
	if (hallfame[NUMFAMENAMES-1].score >= total)
								hallfame[NUMFAMENAMES-1].score = 0;
	int index;
	for (index = 0; index < NUMFAMENAMES; index++) {
		if (total > hallfame[index].score) {
			if (index < (NUMFAMENAMES-1)) for (i = (NUMFAMENAMES-1); i > index; i--) hallfame[i] = hallfame[i-1];
			hallfame[index].score = total;
			hallfame[index].level = Scen.Scenario;
			hallfame[index].name[0] = 0;	// blank out the name
			hallfame[index].side = house;
			break;
		}
	}

	/*
	** Now display the hall of fame
	*/
	Set_Logic_Page(SeenBuff);

#ifdef WIN32
	char maststr[NUMFAMENAMES*32];
#endif
	char const *pal;
	for (i = 0; i < NUMFAMENAMES; i++) {
		pal = hallfame[i].side ? _redpal : _bluepal;
		Alloc_Object(new ScorePrintClass(hallfame[i].name, HALLFAME_X, HALLFAME_Y + (i*8), pal));
		if (hallfame[i].score) {
#ifdef WIN32
			char *str = maststr + i*32;
#else
			char *str = (char *)(HidPage.Get_Buffer()) + i*32;
#endif
			sprintf(str, "%d", hallfame[i].score);
			Alloc_Object(new ScorePrintClass(str, HALLFAME_X+(6*14), HALLFAME_Y + (i*8), pal, BLACK));
			if (hallfame[i].level < 20) {
				sprintf(str+16, "%d", hallfame[i].level);
			} else {
				strcpy(str+16, "**");
			}
			Alloc_Object(new ScorePrintClass(str+16, HALLFAME_X+(6*11), HALLFAME_Y + (i*8), pal, BLACK));
			Call_Back_Delay(13);
		}
	}
#ifdef WIN32
	// Wait for text printing to complete
	while (StillUpdating) {
		Call_Back_Delay(1);
	}
#endif
	/*
	** If the player's on the hall of fame, have him enter his name now
	*/
	Keyboard->Clear();

	if (index < NUMFAMENAMES) {
		pal = hallfame[index].side ? _redpal : _bluepal;
		Input_Name(hallfame[index].name, HALLFAME_X, HALLFAME_Y + (index*8), pal);

		file.Open(WRITE);
		for (i = 0; i < NUMFAMENAMES; i++) {
			file.Write(&hallfame[i], sizeof(struct Fame));
		}
		file.Close();
	} else {
		Alloc_Object(new ScorePrintClass(TXT_CLICK_CONTINUE, 149, 190, _yellowpal));
		ControlQ = false;
		Cycle_Wait_Click();
	}

	Keyboard->Clear();

/* get rid of all the animating objects */
	for (i = 0; i < MAXSCOREOBJS; i++) if (ScoreObjs[i]) {
		delete ScoreObjs[i];
		ScoreObjs[i] = 0;
	}
	BlackPalette.Set(FADE_PALETTE_FAST, NULL);
#ifdef WIN32
	VisiblePage.Clear();
#else
	SeenPage.Clear();
#endif
	Show_Mouse();
//	Map_Selection();
//	Scen.ScenVar = SCEN_VAR_A;
//	Scen.ScenDir = SCEN_DIR_EAST;

	Theme.Queue_Song(THEME_NONE);

	BlackPalette.Set(FADE_PALETTE_FAST, NULL);
#ifdef WIN32
	VisiblePage.Clear();
#else
	SeenPage.Clear();
#endif
	GamePalette.Set();

	Set_Font(oldfont);
	FontXSpacing = oldfontxspacing;
	ControlQ = 0;

#ifdef WIN32
	delete PseudoSeenBuff;
#ifdef FIXIT_SCORE_CRASH
	/*
	** Fix for the score screen crash due to uncompressed shape buffer overflow.
	*/
	Enable_Uncompressed_Shapes();
#endif	//FIXIT

#endif
#endif
}


void Cycle_Wait_Click(bool cycle)
{
	int counter = 0;
	int minclicks = 20;
	unsigned long timingtime = TickCount;
	//PG SerialPacketType sendpacket;
	//PG SerialPacketType receivepacket;
	//PG int packetlen;


	Keyboard->Clear();
	while (minclicks || (!Keyboard->Check() && !ControlQ) ) {
#if (0)	//PG
		if (Session.Type == GAME_NULL_MODEM ||
			Session.Type == GAME_MODEM) {

			//
			// send a timing packet if enough time has gone by.
			//
			if ( (TickCount - timingtime) > PACKET_TIMING_TIMEOUT) {
				memset (&sendpacket, 0, sizeof(SerialPacketType));
				sendpacket.Command = SERIAL_SCORE_SCREEN;
				sendpacket.ScenarioInfo.ResponseTime = NullModem.Response_Time();
				sendpacket.ID = Session.ModemType;

				NullModem.Send_Message (&sendpacket, sizeof(sendpacket), 0);
				timingtime = TickCount;
			}

			if (NullModem.Get_Message (&receivepacket, &packetlen) > 0) {
				// throw packet away
				packetlen = packetlen;
			}

			NullModem.Service();
		}
#endif
		Call_Back_Delay(1);
		if (minclicks) {
			minclicks--;
			Keyboard->Clear();
		}

		if(cycle) {
			counter = ((++counter) & 7);
			if (counter == 0 && Options.IsPaletteScroll) {
				RGBClass rgb = ScorePalette[233];
				for (int i = 233; i < 237; i++) {
					ScorePalette[i] = ScorePalette[i+1];
				}
				ScorePalette[237] = rgb;
				ScorePalette.Set();
			}
		}
	}
	Keyboard->Clear();
}

void ScoreClass::Do_Nod_Buildings_Graph(void)
{
	int shapenum;
	InfantryTypeClass const *ramboclass;

	void const * factptr   = MFCD::Retrieve("POWR.SHP");
	void const * rmboptr   = MFCD::Retrieve("E7.SHP");
	void const * fball1ptr = MFCD::Retrieve("FBALL1.SHP");
	ramboclass = &InfantryTypeClass::As_Reference(INFANTRY_TANYA);

	/*
	** Print the # of buildings on the hidpage so we only need to do it once
	*/
	SeenPage.Blit(HidPage);
	Set_Logic_Page(HidPage);
	Call_Back_Delay(30);
	Set_Font_Palette(_redpal);
	HidPage.Print( 0, BUILDING_X + 16, BUILDING_Y + 10, TBLACK, TBLACK);
	Set_Font_Palette(_bluepal);
	HidPage.Print( 0, BUILDING_X + 16, BUILDING_Y + 22, TBLACK, TBLACK);

	/*
	** Here's the animation/draw loop for blowing up the factory
	*/
	int i;
	for (i=0; i<98; i++) {
		HidPage.Blit(HidPage, BUILDING_X, BUILDING_Y, 0, 0, 320-BUILDING_X, 48);
		shapenum = 0;	// no damage
		if (i >= 60) {
			shapenum = Extract_Shape_Count(factptr) - 2;	// some damage
			if (i == 60) {
				Shake_The_Screen(6);
				Sound_Effect(VOC_CRUMBLE);
			}
			if (i > 65) {
				shapenum = Extract_Shape_Count(factptr) - 1;	// mega damage
			}
		}

		/*
		** Draw the building before Rambo
		*/
		if (i < 68) {
			CC_Draw_Shape(factptr, shapenum, 0, 0, WINDOW_MAIN,
					  SHAPE_GHOST|SHAPE_FADING|SHAPE_WIN_REL, ColorRemaps[PCOLOR_GOLD].RemapTable, DisplayClass::UnitShadow);

		}

		/*
		** Now draw some fires, if appropriate
		*/
		if (i >= 61) {
			int firecount = Extract_Shape_Count(fball1ptr);
			int shapeindex = (i-61) / 2;
			if (shapeindex < firecount) {
				CC_Draw_Shape(fball1ptr, shapeindex, 10, 10, WINDOW_MAIN,
						  SHAPE_CENTER|SHAPE_WIN_REL);
			}
			if (i > 64) {
				shapeindex = (i-64) / 2;
				if (shapeindex < firecount) {
					CC_Draw_Shape(fball1ptr, shapeindex, 50, 30, WINDOW_MAIN,
						  SHAPE_CENTER|SHAPE_WIN_REL);
				}
			}
		}
		/*
		** Draw the Tanya character running away from the building
		*/
		CC_Draw_Shape(rmboptr, (ramboclass->DoControls[DO_WALK].Frame + ramboclass->DoControls[DO_WALK].Jump*6) + ((unsigned(i)>>1)%ramboclass->DoControls[DO_WALK].Count),
						  i+32, 40, WINDOW_MAIN,
						  SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_GHOST,
						  ColorRemaps[PCOLOR_RED].RemapTable, DisplayClass::UnitShadow);
		HidPage.Blit(SeenPage, 0, 0, BUILDING_X, BUILDING_Y, 320-BUILDING_X, 48);
/*BG		if (!Keyboard->Check()) */ Call_Back_Delay(1);
	}

	i = max(GBKilled, NBKilled);
	for (int q = 0; q <= i; q++) {
		Set_Font_Palette(_redpal);
		Count_Up_Print( "%d", q, NBKilled, BUILDING_X + 16, BUILDING_Y + 10);
		Set_Font_Palette(_bluepal);
		Count_Up_Print( "%d", q, GBKilled, BUILDING_X + 16, BUILDING_Y + 22);
//BG		if (!Keyboard->Check()) {
#ifdef WIN32
			Play_Sample(Beepy6, 255, Options.Normalize_Volume(150));
#else
			Play_Sample(Beepy6, 255, Options.Normalize_Volume(60));
#endif
			Call_Back_Delay(1);
//BG		}
	}
	Set_Font_Palette(_redpal);
	Count_Up_Print( "%d", NBKilled, NBKilled, BUILDING_X + 16, BUILDING_Y + 10);
	Set_Font_Palette(_bluepal);
	Count_Up_Print( "%d", GBKilled, GBKilled, BUILDING_X + 16, BUILDING_Y + 22);
}


/***************************************************************************
 * DO_GDI_GRAPH -- Show # of people or buildings killed on GDI score screen*
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:   yellowptr, redptr = pointers to shape file for graphs          *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/03/1995 BWG : Created.                                             *
 *=========================================================================*/

void ScoreClass::Do_GDI_Graph(void const * yellowptr, void const * redptr, int gkilled, int nkilled, int ypos)
{
	int i, maxval;
#ifdef WIN32
	int xpos = 174;
	int house = (PlayerPtr->Class->House == HOUSE_USSR || PlayerPtr->Class->House == HOUSE_UKRAINE);		// 0 or 1
	if(house) {
		int temp = gkilled;
		gkilled = nkilled;
		nkilled = temp;
		void const *tempptr = yellowptr;
		yellowptr = redptr;
		redptr = tempptr;
	}
#else
	int xpos = 173;
#endif
	int gdikilled = gkilled, nodkilled=nkilled;

	maxval = max(gdikilled, nodkilled);
	if (!maxval) maxval=1;

	gdikilled = (gdikilled * SIZEGBAR) / maxval;
	nodkilled = (nodkilled * SIZEGBAR) / maxval;
	if (maxval < 20) {
		gdikilled = gkilled * 5;
		nodkilled = nkilled * 5;
	}

	maxval = max(gdikilled, nodkilled);
	if (!maxval) maxval=1;

	// Draw the white-flash shape on the hidpage
	Set_Logic_Page(HidPage);
	HidPage.Fill_Rect(0, 0, 124*RESFACTOR, 9*RESFACTOR, TBLACK);
	CC_Draw_Shape(redptr, 119, 0, 0, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
	Set_Logic_Page(SeenBuff);
#ifdef WIN32
	Set_Font_Palette(house ? _redpal : _bluepal);
#else
	Set_Font_Palette(_bluepal);
#endif

	for (i = 1; i <= gdikilled; i++) {
		if (i != gdikilled) {
#ifdef WIN32
			Set_Logic_Page(*PseudoSeenBuff);
			CC_Draw_Shape(yellowptr, i, xpos*RESFACTOR, ypos*RESFACTOR, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
			Set_Logic_Page(SeenBuff);
#endif
			CC_Draw_Shape(yellowptr, i, xpos*RESFACTOR, ypos*RESFACTOR, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
		} else {
			HidPage.Blit(SeenPage, 0, 0, xpos*RESFACTOR, ypos*RESFACTOR, (3+gdikilled)*RESFACTOR, 8*RESFACTOR);
#ifdef WIN32
			HidPage.Blit(*PseudoSeenBuff, 0, 0, xpos*RESFACTOR, ypos*RESFACTOR, (3+gdikilled)*RESFACTOR, 8*RESFACTOR);
#endif
		}

		Count_Up_Print("%d", (i*gkilled) / maxval, gkilled, 297, ypos+2);
//BG		if (!Keyboard->Check()) {
#ifdef WIN32
			Play_Sample(Beepy6, 255, Options.Normalize_Volume(150));
#else
			Play_Sample(Beepy6, 255, Options.Normalize_Volume(60));
#endif
			Call_Back_Delay(2);
//BG		}
	}
	CC_Draw_Shape(yellowptr, gdikilled, xpos*RESFACTOR, ypos*RESFACTOR   , WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
#ifdef WIN32
	Set_Logic_Page(*PseudoSeenBuff);
	CC_Draw_Shape(yellowptr, gdikilled, xpos*RESFACTOR, ypos*RESFACTOR   , WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
	Set_Logic_Page(SeenBuff);
#endif
	Count_Up_Print("%d", gkilled, gkilled, 297, ypos+ 2);
/*BG	if (!Keyboard->Check()) */ Call_Back_Delay(40);

#ifdef WIN32
	Set_Font_Palette(house ? _bluepal : _redpal);
#else
	Set_Font_Palette(_redpal);
#endif
	for (i = 1; i <= nodkilled; i++) {
		if (i != nodkilled) {
#ifdef WIN32
			Set_Logic_Page(*PseudoSeenBuff);
			CC_Draw_Shape(redptr, i, xpos*RESFACTOR, (ypos+12)*RESFACTOR, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
			Set_Logic_Page(SeenBuff);
#endif
			CC_Draw_Shape(redptr, i, xpos*RESFACTOR, (ypos+12)*RESFACTOR, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
		} else {
			HidPage.Blit(SeenPage, 0, 0, xpos*RESFACTOR, (ypos+12)*RESFACTOR, (3+nodkilled)*RESFACTOR, 8*RESFACTOR);
#ifdef WIN32
			HidPage.Blit(*PseudoSeenBuff, 0, 0, xpos*RESFACTOR, (ypos+12)*RESFACTOR, (3+nodkilled)*RESFACTOR, 8*RESFACTOR);
#endif
		}

		Count_Up_Print("%d", (i*nkilled) / maxval, nkilled, 297, ypos+14);
//BG		if (!Keyboard->Check()) {
#ifdef WIN32
			Play_Sample(Beepy6, 255, Options.Normalize_Volume(150));
#else
			Play_Sample(Beepy6, 255, Options.Normalize_Volume(60));
#endif
			Call_Back_Delay(2);
//BG		}
	}

//	if (Keyboard::Check()) Keyboard::Clear();

	/*
	** Make sure accurate count is printed at end
	*/
#ifdef WIN32
	Set_Logic_Page(*PseudoSeenBuff);
	CC_Draw_Shape(   redptr, nodkilled, xpos*RESFACTOR, (ypos+12)*RESFACTOR, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
	Set_Logic_Page(SeenBuff);
#endif
	CC_Draw_Shape(   redptr, nodkilled, xpos*RESFACTOR, (ypos+12)*RESFACTOR, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
	Count_Up_Print("%d", nkilled, nkilled, 297, ypos+14);
/*BG	if (!Keyboard->Check()) */ Call_Back_Delay(40);
}


void ScoreClass::Do_Nod_Casualties_Graph(void)
{
	int i, gdikilled, nodkilled, maxval;

	void const * e1ptr = MFCD::Retrieve("E1.SHP");

	gdikilled = GKilled;
	nodkilled = NKilled;
	maxval = max(gdikilled, nodkilled);

	if (!maxval) maxval=1;
	if ((gdikilled > (MAX_BAR_X - BARGRAPH_X)) || (nodkilled > (MAX_BAR_X - BARGRAPH_X)) ) {
		gdikilled = (gdikilled * (MAX_BAR_X - BARGRAPH_X)) / maxval;
		nodkilled = (nodkilled * (MAX_BAR_X - BARGRAPH_X)) / maxval;
	}

	maxval = max(gdikilled, nodkilled);
	if (!maxval) maxval=1;

	/*
	** Initialize a bunch of objects for the infantrymen who pose for the bar
	** graphs of casualties.
	*/
	int r = NUMINFANTRYMEN/2;
	for (i = 0; i < NUMINFANTRYMEN/2; i++) {
		InfantryMan[i+0].xpos =
		InfantryMan[i+r].xpos =  (i*10) + 7;
		InfantryMan[i+0].ypos = 11;
		InfantryMan[i+r].ypos = 21;
		InfantryMan[i+0].shapefile =
		InfantryMan[i+r].shapefile = e1ptr;
		InfantryMan[i+0].remap = ColorRemaps[PCOLOR_RED].RemapTable;
		InfantryMan[i+r].remap = ColorRemaps[PCOLOR_BLUE].RemapTable;
		InfantryMan[i+0].anim  =
		InfantryMan[i+r].anim  = 0;
		InfantryMan[i+0].stage =
		InfantryMan[i+r].stage = 0;
		InfantryMan[i+0].delay =
		InfantryMan[i+r].delay = NonCriticalRandomNumber & 0x1F;
		InfantryMan[i+0].Class =
		InfantryMan[i+r].Class = &InfantryTypeClass::As_Reference(INFANTRY_E1);
	}

	/*
	** Draw the infantrymen and pause briefly before running the graph
	*/
	Draw_InfantryMen();
	HidPage.Blit(SeenPage, 0, 0, BARGRAPH_X, CASUALTY_Y, 320-BARGRAPH_X, 34);
	Call_Back_Delay(40);

	for (i = 1; i <= maxval; i++) {
		// Draw & update infantrymen 3 times for every tick on the graph (i)
		for (int index = 0; index < 3; index++) {
			Draw_InfantryMen();
			Draw_Bar_Graphs(i, nodkilled, gdikilled);
			HidPage.Blit(SeenPage, 0, 0, BARGRAPH_X, CASUALTY_Y, 320-BARGRAPH_X, 34);

			Set_Font_Palette(_redpal);
			Count_Up_Print("%d", (i*NKilled) / maxval, NKilled, SCORETEXT_X+64, CASUALTY_Y +  2);
			Set_Font_Palette(_bluepal);
			Count_Up_Print("%d", (i*GKilled) / maxval, GKilled, SCORETEXT_X+64, CASUALTY_Y + 14);
/*BG			if (!Keyboard->Check()) */ Call_Back_Delay(3);
		}
#ifdef WIN32
		Play_Sample(Beepy6, 255, Options.Normalize_Volume(150));
#else
		Play_Sample(Beepy6, 255, Options.Normalize_Volume(60));
#endif
	}
//BG	if (Keyboard->Check()) Keyboard->Clear();

	/*
	** Make sure accurate count is printed at end
	*/
	Set_Font_Palette(_redpal);
	Count_Up_Print("%d", NKilled, NKilled, SCORETEXT_X+64, CASUALTY_Y +  2);
	Set_Font_Palette(_bluepal);
	Count_Up_Print("%d", GKilled, GKilled, SCORETEXT_X+64, CASUALTY_Y + 14);

	/*
	** Finish up death animations, if there are any active
	*/
	int k = 1;
	while (k) {
		for (i=k=0; i<NUMINFANTRYMEN; i++) {
			if (InfantryMan[i].anim >= DO_GUN_DEATH) {
				k=1;
			}
		}
		if (k) {
			Draw_InfantryMen();
		}
		Draw_Bar_Graphs(maxval, nodkilled, gdikilled);
		HidPage.Blit(SeenPage, 0, 0, BARGRAPH_X, CASUALTY_Y, 320-BARGRAPH_X, 34);
		Call_Back_Delay(1);
	}
}


void ScoreClass::Show_Credits(int house, char const pal[])
{
	static int _credsx[2]={276,276};
	static int _credsy[2]={173,58};
	static int _credpx[2]={228,236};
#ifdef GERMAN
	static int _credpy[2]={181, 74};
	static int _credtx[2]={162,162};
	static int _credty[2]={173, 62};
#else
	static int _credpy[2]={189-12, 74};
	static int _credtx[2]={182,182};
	static int _credty[2]={179-12, 62};
#endif

	int credobj,i;
	int minval,add;

#ifdef WIN32
	void const * credshape = MFCD::Retrieve(house ? "CREDSUHR.SHP" : "CREDSAHR.SHP");
#else
	void const * credshape = MFCD::Retrieve(house ? "CREDSU.SHP" : "CREDSA.SHP");
#endif

	Alloc_Object(new ScorePrintClass(TXT_SCORE_ENDCRED, _credtx[house], _credty[house], pal));
	Call_Back_Delay(15);

	credobj = Alloc_Object(new ScoreCredsClass(_credsx[house], _credsy[house], credshape, 32, 2));
	minval = PlayerPtr->Available_Money() / 100;

	/*
	** Print out total credits left at end of scenario
	*/
	i = -50;

	do {
		add = 5;
		if ((PlayerPtr->Available_Money() - i) > 100 ) add += 15;
		if ((PlayerPtr->Available_Money() - i) > 500 ) add += 30;
		if ((PlayerPtr->Available_Money() - i) > 1000) add += PlayerPtr->Available_Money() / 40;
		if (add < minval) add = minval;
		i += add;

		if (i < 0) i=0;

		Set_Font_Palette(pal);
		Count_Up_Print("%d", i, PlayerPtr->Available_Money(), _credpx[house], _credpy[house]);
		Call_Back_Delay(2);
/*BG		if (Keyboard->Check()) {
			Count_Up_Print("%d", PlayerPtr->Available_Money(), PlayerPtr->Available_Money(), _credpx[house], _credpy[house]);
			Keyboard->Clear();
			break;
		}*/
	} while (i < PlayerPtr->Available_Money()) ;

	delete ScoreObjs[credobj];
	ScoreObjs[credobj] = 0;
}


/***************************************************************************
 * SCORECLASS::PRINT_MINUTES -- Print out hours/minutes up to max          *
 *                                                                         *
 *    Same as count-up-print, but for the time                             *
 *                                                                         *
 * INPUT:   current minute count and maximum                               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/13/1995 BWG : Created.                                             *
 *=========================================================================*/
void ScoreClass::Print_Minutes(int minutes)
{
	char str[20];
	if (minutes >= 60) {
		if ((minutes/60) > 9) minutes = (9*60 + 59);
		sprintf(str, Text_String(TXT_SCORE_TIMEFORMAT1), (minutes / 60), (minutes % 60));
	} else {
		sprintf(str, Text_String(TXT_SCORE_TIMEFORMAT2), minutes);
	}
	SeenPage.Print(str, 275*RESFACTOR, 9*RESFACTOR, TBLACK, TBLACK);
#ifdef WIN32
	PseudoSeenBuff->Print(str, 275*RESFACTOR, 9*RESFACTOR, TBLACK, TBLACK);
#endif
}


/***********************************************************************************************
 * ScoreClass::Count_Up_Print -- Prints a number (up to its max) into a string, cleanly.       *
 *                                                                                             *
 *    This routine prints out a number (like 70) or its maximum number, into a string,   onto  *
 *    the screen, on a clean section of the screen, and blits it forward to the seenpage so you*
 *    can print without flashing and can print over something (to count up %'s).               *
 *                                                                                             *
 * INPUT:   str = string to print into                                                         *
 *            percent = # to print                                                             *
 *            max = # to print if percent > max                                                *
 *            xpos = x pixel coord                                                             *
 *            ypos = y pixel coord                                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/07/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
void ScoreClass::Count_Up_Print(char *str, int percent, int maxval, int xpos, int ypos)
{
	char destbuf[64];

	sprintf(destbuf, str, percent <= maxval ? percent : maxval);
	SeenPage.Print(	destbuf, xpos * RESFACTOR, ypos * RESFACTOR, TBLACK, BLACK);
#ifdef WIN32
	PseudoSeenBuff->Print(	destbuf, xpos * RESFACTOR, ypos * RESFACTOR, TBLACK, BLACK);
#endif
}


/***********************************************************************************************
 * ScoreClass::Input_Name -- Gets the name from the keyboard                                   *
 *                                                                                             *
 *      This routine handles keyboard input, and does a nifty zooming letter effect too.       *
 *                                                                                             *
 * INPUT:   str = string to put user's typing into                                             *
 *            xpos = x pixel coord                                                             *
 *            ypos = y pixel coord                                                             *
 *            pal  = text remapping palette to print using                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/15/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
void ScoreClass::Input_Name(char str[], int xpos, int ypos, char const pal[])
{
	int key = 0;
	int ascii, index=0;

	void const * keystrok = MFCD::Retrieve("KEYSTROK.AUD");

	/*
	** Ready the hidpage so it can restore background under zoomed letters
	*/
	SeenPage.Blit(HidPage);

	/*
	** Put a copy of the high score area on a spare area of the hidpage, so
	** we can use it to restore the letter's background instead of filling
	** with black.
	*/
	HidPage.Blit(HidPage, 0, 100*RESFACTOR, 0, 0, 100*RESFACTOR, 100*RESFACTOR);

	do {
		Call_Back();
		Animate_Score_Objs();
		Animate_Cursor(index, ypos);
		if (Keyboard->Check()) {
			key = Keyboard->To_ASCII(Keyboard->Get()) & 0xFF;
			Call_Back();

			if (index == MAX_FAMENAME_LENGTH-2) {
				while (Keyboard->Check()) {
					Keyboard->Get();
				}
			}

			/*
			** If they hit 'backspace' when they're on the last letter,
			** turn it into a space instead.
			*/
			if ((key == KA_BACKSPACE) && (index == MAX_FAMENAME_LENGTH-2) ) {
				if (str[index] && str[index]!=32) key = 32;
			}
			if (key == KA_BACKSPACE) {							//if (key == KN_BACKSPACE) {
				if (index) {
					str[--index] = 0;

					int xposindex6 = (xpos+(index*6))*RESFACTOR;
					HidPage.Blit(SeenPage, xposindex6, (ypos-100)*RESFACTOR, xposindex6, ypos*RESFACTOR, 6*RESFACTOR, 6*RESFACTOR);
		#ifdef WIN32
					HidPage.Blit(*PseudoSeenBuff, xposindex6, (ypos-100)*RESFACTOR, xposindex6, ypos*RESFACTOR, 6*RESFACTOR, 6*RESFACTOR);
		#endif
					HidPage.Blit(HidPage,  xposindex6, (ypos-100)*RESFACTOR, xposindex6, ypos*RESFACTOR, 6*RESFACTOR, 6*RESFACTOR);
				}

			} else if (key != KA_RETURN) {			//else if (key != KN_RETURN && key!=KN_KEYPAD_RETURN) {
				ascii = key;					//ascii = KN_To_KA(key);
				if (ascii >= 'a' && ascii <= 'z') ascii -= ('a' - 'A');
				if ( (ascii >= '!' && ascii <= KA_TILDA) || ascii == ' ') {
					HidPage.Blit(SeenPage, (xpos + (index*6))*RESFACTOR, (ypos-100)*RESFACTOR, (xpos + (index*6))*RESFACTOR, ypos*RESFACTOR, 6*RESFACTOR, 6*RESFACTOR);
		#ifdef WIN32
					HidPage.Blit(*PseudoSeenBuff, (xpos + (index*6))*RESFACTOR, (ypos-100)*RESFACTOR, (xpos + (index*6))*RESFACTOR, ypos*RESFACTOR, 6*RESFACTOR, 6*RESFACTOR);
		#endif
					HidPage.Blit(HidPage, (xpos + (index*6))*RESFACTOR, (ypos-100)*RESFACTOR, (xpos + (index*6))*RESFACTOR, ypos*RESFACTOR, 6*RESFACTOR, 6*RESFACTOR);
					str[index] = ascii;
					str[index+1] = 0;

					int objindex;
#ifdef WIN32
					Play_Sample(keystrok, 255, Options.Normalize_Volume(150));
#else
					Play_Sample(keystrok, 255, Options.Normalize_Volume(105));
#endif
					objindex = Alloc_Object(new ScoreScaleClass(str+index, xpos+(index*6), ypos, pal));
					while (ScoreObjs[objindex]) Call_Back_Delay(1);

					if (index < (MAX_FAMENAME_LENGTH-2) ) index++;
				}
			}
		}
	} while (key != KA_RETURN);					//	} while(key != KN_RETURN && key!=KN_KEYPAD_RETURN);
}


void Animate_Cursor(int pos, int ypos)
{
	static int _lastpos = 0, _state;
	static CDTimerClass<SystemTimerClass> _timer;

	ypos += 6;	// move cursor to bottom of letter

	ypos *= RESFACTOR;

	// If they moved the cursor, erase old one and force state=0, to make green draw right away
	if (pos != _lastpos) {
		HidPage.Blit(SeenPage, (HALLFAME_X + (_lastpos*6))*RESFACTOR, ypos-100*RESFACTOR, (HALLFAME_X + (_lastpos*6))*RESFACTOR, ypos, 6*RESFACTOR, 1*RESFACTOR);
#ifdef WIN32
		HidPage.Blit(*PseudoSeenBuff, (HALLFAME_X + (_lastpos*6))*RESFACTOR, ypos-100*RESFACTOR, (HALLFAME_X + (_lastpos*6))*RESFACTOR, ypos, 6*RESFACTOR, 1*RESFACTOR);
#endif
		_lastpos = pos;
		_state = 0;
	}
	SeenBuff.Draw_Line((HALLFAME_X + (pos*6))*RESFACTOR, ypos, (HALLFAME_X + (pos*6)+5)*RESFACTOR, ypos, _state ? LTBLUE : TBLACK);
#ifdef WIN32
	PseudoSeenBuff->Draw_Line((HALLFAME_X + (pos*6))*RESFACTOR, ypos, (HALLFAME_X + (pos*6)+5)*RESFACTOR, ypos, _state ? LTBLUE : TBLACK);
#endif
	/*
	** Toggle the color of the cursor, green or black, if it's time to do so.
	*/
	if (!_timer) {
		_state ^= 1;
		_timer = 5;
	}
}


/***************************************************************************
 * Draw_InfantryMen -- Draw all the guys on the score screen               *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/13/1995 BWG : Created.                                             *
 *=========================================================================*/
void Draw_InfantryMen()
{
	int k;

// Only draw the infantrymen if we're playing USSR... Allies wouldn't execute
//	people like that.

	/*
	** First restore the background
	*/
	HidPage.Blit(HidPage, BARGRAPH_X, CASUALTY_Y, 0, 0, 320-BARGRAPH_X, 34);
	Set_Logic_Page(HidPage);

	/*
	** Then draw all the infantrymen on the clean hidpage
	*/
	for (k = 0; k < NUMINFANTRYMEN; k++) Draw_InfantryMan(k);
	/*
	** They'll all be blitted over to the seenpage after the graphs are drawn
	*/
}

/***************************************************************************
 * Draw_InfantryMan -- Draw one guy in score screen, update animation      *
 *                                                                         *
 *    This routine draws one of the infantrymen in the "Casualties" area   *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/13/1995 BWG : Created.                                             *
 *=========================================================================*/
void Draw_InfantryMan(int index)
{
	int stage;

	/* If the infantryman's dead, just abort this function */
	if (InfantryMan[index].anim == -1) return;

	stage = InfantryMan[index].stage + InfantryMan[index].Class->DoControls[InfantryMan[index].anim].Frame;

	CC_Draw_Shape(InfantryMan[index].shapefile,
					  stage,
					  InfantryMan[index].xpos,
					  InfantryMan[index].ypos,
					  WINDOW_MAIN,
					  SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_GHOST,
					  InfantryMan[index].remap,
					  DisplayClass::UnitShadow);
	/*
	** see if it's time to run a new anim
	*/
	if (--InfantryMan[index].delay <= 0) {
		InfantryMan[index].delay = 3;
		if (++InfantryMan[index].stage >= InfantryMan[index].Class->DoControls[InfantryMan[index].anim].Count) {

			/*
			** was he playing a death anim? If so, and it's done, erase him
			*/
			if (InfantryMan[index].anim >= DO_GUN_DEATH) {
				InfantryMan[index].anim = -1;
			} else {
				New_Infantry_Anim(index, DO_STAND_READY);
			}
		}
	}
}


/***************************************************************************
 * New_Infantry_Anim -- Start up a new animation for one of the infantrymen*
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:   index: which of the 30 infantrymen to affect                   *
 *          anim:  which animation sequence to start him into              *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/13/1995 BWG : Created.                                             *
 *=========================================================================*/
void New_Infantry_Anim(int index, int anim)
{
	InfantryMan[index].anim = anim;
	InfantryMan[index].stage = 0;
	if (anim >= DO_GUN_DEATH) {
		InfantryMan[index].delay = 1;	// start right away
	} else {
		InfantryMan[index].delay = NonCriticalRandomNumber & 15;
	}
}


/***************************************************************************
 * Draw_Bar_Graphs -- Draw "Casualties" bar graphs                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:   i = current count of how far to draw graph                     *
 *          gkilled = # of GDI forces killed (adjusted to fit in space)    *
 *          nkilled = # of Nod forces killed (adjusted to fit in space)    *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/13/1995 BWG : Created.                                             *
 *   07/02/1996 BWG : Removed references to civilians.                     *
 *=========================================================================*/
void Draw_Bar_Graphs(int i, int gkilled, int nkilled)
{

	if (gkilled) {
		LogicPage->Fill_Rect(0,   0+4*RESFACTOR, 0+min(i, gkilled)*RESFACTOR,   0+5*RESFACTOR, RED);
		LogicPage->Draw_Line(0+1*RESFACTOR, 0+6*RESFACTOR, (0+min(i, gkilled)+1)*RESFACTOR, 0+6*RESFACTOR, TBLACK);
		LogicPage->Draw_Line((0+MIN(i, gkilled)+1)*RESFACTOR, 0+5*RESFACTOR, (0+min(i, gkilled)+1)*RESFACTOR, 0+5*RESFACTOR, TBLACK);
		if (i <= gkilled) {
			int anim = InfantryMan[i/11].anim;
			if (anim!=-1 && anim < DO_GUN_DEATH) {
				if (i/11) {
					New_Infantry_Anim(i/11, DO_GUN_DEATH + (NonCriticalRandomNumber & 3));
				} else {
					New_Infantry_Anim(i/11, DO_GUN_DEATH);
				}
//				Sound_Effect(Random_Pick(VOC_SCREAM1, VOC_SCREAM5));
			}
		}
	}
	if (nkilled) {
		LogicPage->Fill_Rect( 0,          0+16*RESFACTOR,  0+min(i, nkilled)*RESFACTOR,    0+17*RESFACTOR, LTCYAN);
		LogicPage->Draw_Line( 0+1*RESFACTOR, 0+18*RESFACTOR, (0+min(i, nkilled)+1)*RESFACTOR, 0+18*RESFACTOR, TBLACK);
		LogicPage->Draw_Line((0+MIN(i, nkilled)+1)*RESFACTOR, 0+17*RESFACTOR, (0+min(i, nkilled)+1)*RESFACTOR, 0+17*RESFACTOR, TBLACK);
		if (i <= nkilled) {
			int anim = InfantryMan[(NUMINFANTRYMEN/2)+(i/11)].anim;
			if (anim!=-1 && anim < DO_GUN_DEATH) {
				if (i/11) {
					New_Infantry_Anim((NUMINFANTRYMEN/2)+(i/11), DO_GUN_DEATH + (NonCriticalRandomNumber & 3));
				} else {
					New_Infantry_Anim((NUMINFANTRYMEN/2)+(i/11), DO_GUN_DEATH);
				}
//				Sound_Effect(Random_Pick(VOC_SCREAM1, VOC_SCREAM5));
			}
		}
	}
}


/***************************************************************************
 * Call_Back_Delay -- Combines Call_Back() and Delay() functions           *
 *                                                                         *
 *    This is just to cut down on code size and typing a little.           *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/13/1995 BWG : Created.                                             *
 *=========================================================================*/
void Call_Back_Delay(int time)
{
	time;
#if (0)//PG
	if (time < 0 ) time = 0;
	if (time > 60) time = 60;
	CDTimerClass<SystemTimerClass> cd;
	CDTimerClass<SystemTimerClass> callbackcd = 0;

	if (!ControlQ) {
		if (Keyboard->Down(KN_LCTRL) && Keyboard->Down(KN_Q)) {
			ControlQ = 1;
			Keyboard->Clear();
		}
	}
	if (ControlQ) time=0;

	cd = time;
	StreamLowImpact = true;
	do {
		if (callbackcd == 0) {
			Call_Back();
			callbackcd = TIMER_SECOND/4;
		}
		Animate_Score_Objs();
	} while (cd);
	StreamLowImpact = false;
#endif
}


void Animate_Score_Objs()
{
#ifdef WIN32
	StillUpdating = false;
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			PseudoSeenBuff->Blit(SeenPage);
		}
#endif
	for (int i = 0; i < MAXSCOREOBJS; i++) {
		if (ScoreObjs[i]) {
			ScoreObjs[i]->Update();
		}
	}
}

char *Int_Print(int a)
{
	static char str[10];

	sprintf(str, "%d", a);
	return str;
}


/***********************************************************************************************
 * Multi_Score_Presentation -- Multiplayer routine to display score screen.                    *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/11/1995  BWG: Created.                                                                 *
 *=============================================================================================*/
extern int CopyType;

void Multi_Score_Presentation(void)
{
#if (0)//PG
	char remap[16];
#ifdef WIN32
	GraphicBufferClass *pseudoseenbuff = new GraphicBufferClass(320, 200, (void*)NULL);
	PseudoSeenBuff = new GraphicBufferClass(SeenBuff.Get_Width(),SeenBuff.Get_Height(),(void*)NULL);
#endif

	int i,k;
	void *oldfont;
	int oldfontxspacing = FontXSpacing;

	FontXSpacing = 0;
	Map.Override_Mouse_Shape(MOUSE_NORMAL);
//	Theme.Queue_Song(THEME_WIN);

	BlackPalette.Set();
	SeenPage.Clear();
	HidPage.Clear();
	Hide_Mouse();
	void *anim = Open_Animation("MLTIPLYR.WSA", NULL, 0L, (WSAOpenType)(WSA_OPEN_FROM_MEM | WSA_OPEN_TO_PAGE), ScorePalette);
	/*
	** Display the background animation
	*/
#ifdef WIN32
	pseudoseenbuff->Clear();
	Animate_Frame(anim, *pseudoseenbuff, 1);
for(int x=0; x<256; x++) memset(&PaletteInterpolationTable[x][0],x,256);
CopyType = 1;
	Interpolate_2X_Scale(pseudoseenbuff , &SeenBuff , 0);
#else
	Animate_Frame(anim, HidPage, 1);
	HidPage.Blit(SeenPage);
#endif
	ScorePalette.Set(FADE_PALETTE_FAST, Call_Back);

	int frame = 1;
	while (frame < Get_Animation_Frame_Count(anim)) {
#ifdef WIN32
		Animate_Frame(anim, *pseudoseenbuff, frame++);
		CopyType = 1;
		Interpolate_2X_Scale(pseudoseenbuff , &SeenBuff , NULL);
		CopyType = 0;
#else
		Animate_Frame(anim, SeenPage, frame++);
#endif
		Call_Back_Delay(2);
	}
	Close_Animation(anim);

#ifdef WIN32
	CopyType = 1;
	Interpolate_2X_Scale(pseudoseenbuff , PseudoSeenBuff , NULL);
	CopyType = 0;
#endif

	/* Change to the six-point font for Text_Print */
	oldfont = Set_Font(ScoreFontPtr);
	Call_Back();

	Set_Logic_Page(SeenBuff);

#ifdef FRENCH
	Alloc_Object(new ScorePrintClass(TXT_SCORE_TOP,   113,  13, _greenpal));
#else
	Alloc_Object(new ScorePrintClass(TXT_SCORE_TOP,   130,  13, _greenpal));
#endif
	Call_Back_Delay(5);
	Alloc_Object(new ScorePrintClass(TXT_COMMANDER,    27,  31, _greenpal));
	Call_Back_Delay(10);
#ifdef FRENCH
	Alloc_Object(new ScorePrintClass(TXT_BATTLES_WON, 113,  31, _greenpal));
#endif
#ifdef GERMAN
	Alloc_Object(new ScorePrintClass(TXT_BATTLES_WON, 118,  31, _greenpal));
#endif
#ifdef ENGLISH
	Alloc_Object(new ScorePrintClass(TXT_BATTLES_WON, 126,  31, _greenpal));
#endif
	Call_Back_Delay(13);
	Alloc_Object(new ScorePrintClass(TXT_KILLS_COLON, 249,  31, _greenpal));
	Call_Back_Delay(6);

	/*
	** Move all the scores over a notch if there's more games than can be
	** shown (which is known by Session.CurGame == MAX_MULTI_GAMES-1);
	*/
	if (Session.CurGame == MAX_MULTI_GAMES-1) {
		for (i = 0; i < MAX_MULTI_NAMES; i++) {
			for (k = 0; k < MAX_MULTI_GAMES-1; k++) {
				Session.Score[i].Kills[k] = Session.Score[i].Kills[k+1];
			}
		}
	}

	int y = 41;
	for (i = 0; i < MAX_MULTI_NAMES; i++) {
		if (strlen(Session.Score[i].Name)) {
			int color = Session.Score[i].Color;
			remap[ 8] = ColorRemaps[color].FontRemap[11];
			remap[ 6] = ColorRemaps[color].FontRemap[12];
			remap[ 4] = ColorRemaps[color].FontRemap[13];
			remap[ 2] = ColorRemaps[color].FontRemap[14];
			remap[14] = ColorRemaps[color].FontRemap[15];

			Alloc_Object(new ScorePrintClass(Session.Score[i].Name, 15,  y, remap));
			Call_Back_Delay(20);

			Alloc_Object(new ScorePrintClass(Int_Print(Session.Score[i].Wins), 118, y, remap));
			Call_Back_Delay(6);

			for (k = 0; k <= min(Session.CurGame, MAX_MULTI_GAMES-2); k++) {
				if (Session.Score[i].Kills[k] >= 0) {
					Alloc_Object(new ScorePrintClass(Int_Print(Session.Score[i].Kills[k]), 225+(24*k), y, remap));
					Call_Back_Delay(6);
				}
			}
			y += 12;
		}
	}

#if defined(GERMAN) || defined(FRENCH)
	Alloc_Object(new ScorePrintClass(TXT_CLICK_CONTINUE, 95 /*(320-strlen(Text_String(TXT_MAP_CLICK2)))/2*/, 190, _yellowpal));
#else
	Alloc_Object(new ScorePrintClass(TXT_CLICK_CONTINUE, 109 /*(320-strlen(Text_String(TXT_MAP_CLICK2)))/2*/, 190, _yellowpal));
#endif
	Cycle_Wait_Click(false);

/* get rid of all the animating objects */
	for (i = 0; i < MAXSCOREOBJS; i++) if (ScoreObjs[i]) {
		delete ScoreObjs[i];
		ScoreObjs[i] = 0;
	}

	Theme.Queue_Song(THEME_NONE);

	BlackPalette.Set(FADE_PALETTE_FAST, NULL);
	SeenPage.Clear();
	GamePalette.Set();
#ifdef WIN32
	delete PseudoSeenBuff;
#endif
	Set_Font(oldfont);
	FontXSpacing = oldfontxspacing;
	ControlQ = 0;
	Show_Mouse();
#endif
}

void ScoreClass::Init(void)
{
	Score = 0;
	NKilled = 0;
	GKilled = 0;
	CKilled = 0;
	NBKilled = 0;
	GBKilled = 0;
	CBKilled = 0;
	NHarvested = 0;
	GHarvested = 0;
	CHarvested = 0;
	ElapsedTime = 0;
	RealTime = 0;
	ChangingGun = 0;
}
