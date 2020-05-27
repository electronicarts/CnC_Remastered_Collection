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

/* $Header: /CounterStrike/INTRO.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INTRO.H                                                      *
 *                                                                                             *
 *                   Programmer : Barry W. Green                                               *
 *                                                                                             *
 *                   Start Date : May 8, 1995                                                  *
 *                                                                                             *
 *                  Last Update : May 8, 1995  [BWG]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


#if (0) //PG
VQAHandle * Open_Movie(char * name);
VQAHandle * Open_Movie(char * name)
{
	if (!Debug_Quiet && Get_Digi_Handle() != -1) {
		AnimControl.OptionFlags |= VQAOPTF_AUDIO;
	} else {
		AnimControl.OptionFlags &= ~VQAOPTF_AUDIO;
	}

	VQAHandle * vqa = VQA_Alloc();
	if (vqa) {
		VQA_Init(vqa, MixFileHandler);

		if (VQA_Open(vqa, name, &AnimControl) != 0) {
			VQA_Free(vqa);
			vqa = 0;
		}
	}
	return(vqa);
}
#endif

/***********************************************************************************************
 * Choose_Side -- play the introduction movies, select house                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   5/08/1995 BWG : Created.                                                                  *
 *=============================================================================================*/
void Choose_Side(void)		//	ajw - In RA, all this did was play a movie. Denzil is using it in its original sense.
{

	Whom = HOUSE_GOOD;

#if (0)	//PG
	if (Special.IsFromInstall) {
		#ifdef DVD // Denzil
		if( Using_DVD() )
		{
			Hide_Mouse();
			Load_Title_Page();
			GamePalette = CCPalette;
			HidPage.Blit(SeenPage);
			CCPalette.Set();
			Set_Logic_Page(SeenBuff);
			Show_Mouse();
			
			switch (WWMessageBox().Process(TXT_CHOOSE, TXT_ALLIES, TXT_SOVIET))
				{
				case 0:
					CurrentCD = 0;
				break;

				case 1:
					CurrentCD = 1;
				break;
				}

			Hide_Mouse();
			BlackPalette.Set(FADE_PALETTE_SLOW);
			SeenPage.Clear();
		}
		#endif
		
		Play_Movie(VQ_INTRO_MOVIE, THEME_NONE, false);
	}
#endif
//	Scen.ScenPlayer = SCEN_PLAYER_GREECE;

#ifdef OBSOLETE
	static char const _yellowpal[]={0x0,0x0,0xC9,0x0,0xBA,0x0,0x93,0x0,0x61,0x0,0x0,0x0,0x0,0x0,0xEE,0x0};
	static char const _redpal[]   ={0x0,0x0,0xA8,0x0,0xD9,0x0,0xDA,0x0,0xE1,0x0,0x0,0x0,0x0,0x0,0xD4,0x0};
	static char const _graypal[]  ={0x0,0x0,0x17,0x0,0x10,0x0,0x12,0x0,0x14,0x0,0x0,0x0,0x0,0x0,0x1C,0x0};

	void * anim;
	VQAHandle * gdibrief=0, * nodbrief=0;
	void const * staticaud, * oldfont;
	void const * speechg, * speechn, * speech;
	int statichandle, speechhandle, speechplaying = 0;
	int oldfontxspacing = FontXSpacing;
	int setpalette = 0;

	int frame = 0, endframe = 255, selection = 0, lettersdone = 0;

	Hide_Mouse();
/* Change to the six-point font for Text_Print */
	oldfont = Set_Font(ScoreFontPtr);

	Call_Back();

	staticaud = Load_Alloc_Data(CCFileClass("STRUGGLE.AUD"));
	speechg = Load_Alloc_Data(CCFileClass("GDI_SLCT.AUD"));
	speechn = Load_Alloc_Data(CCFileClass("NOD_SLCT.AUD"));

//	staticaud = MixFileClass::Retrieve("STRUGGLE.AUD");
//	speechg = MixFileClass::Retrieve("GDI_SLCT.AUD");
//	speechn = MixFileClass::Retrieve("NOD_SLCT.AUD");

	anim = Open_Animation("CHOOSE.WSA", NULL, 0L, (WSAOpenType)(WSA_OPEN_FROM_MEM | WSA_OPEN_TO_PAGE), Palette);
	Call_Back();

	nodbrief = Open_Movie("NOD1PRE.VQA");
	Call_Back();
	gdibrief = Open_Movie("GDI1.VQA");

	if (Special.IsFromInstall) {
		Set_Video_Mode(MCGA_MODE);
		PreserveVQAScreen = 1;
//		Hide_Mouse();
		Play_Movie("INTRO2", THEME_NONE, false);
		Show_Mouse();
	}

	HidPage.Clear();
	if (!Special.IsFromInstall) {
		SeenPage.Clear();
//		Set_Palette(Palette);
		Palette.Set();
	} else {
		setpalette = 1;
	}

	statichandle = Play_Sample(staticaud, 255, 64);
	Alloc_Object(new ScorePrintClass(TXT_GDI_NAME,   0, 180, _yellowpal));
#ifdef FRENCH
	Alloc_Object(new ScorePrintClass(TXT_GDI_NAME2,  0, 187, _yellowpal));
#endif
	Alloc_Object(new ScorePrintClass(TXT_NOD_NAME, 180, 180, _redpal));

#ifdef GERMAN
	Alloc_Object(new ScorePrintClass(TXT_SEL_TRANS, 57, 190, _graypal));
#else
#ifdef FRENCH
	Alloc_Object(new ScorePrintClass(TXT_SEL_TRANS, 103, 194, _graypal));
#else
	Alloc_Object(new ScorePrintClass(TXT_SEL_TRANS, 103, 190, _graypal));
#endif
#endif
	Keyboard->Clear();

	while (endframe != frame || (speechplaying && Is_Sample_Playing(speech)) ) {
		Animate_Frame(anim, HidPage, frame++);
		Hide_Mouse();
		if (setpalette) {
			Wait_Vert_Blank(VertBlank);
			//Set_Palette(Palette);
			Palette.Set();
			setpalette = 0;
		}
		HidPage.Blit(SeenPage, 0, 22, 0, 22, 320, 156);
		Show_Mouse();

		if (!Is_Sample_Playing(staticaud)) statichandle = Play_Sample(staticaud, 255, 64);
		Call_Back_Delay(3);	// delay only if haven't clicked

		/* keep the mouse hidden until the letters are thru printing */
		if (!lettersdone) {
			lettersdone = true;
			for (int i=0; i < MAXSCOREOBJS; i++) if (ScoreObjs[i]) lettersdone = 0;
			if (lettersdone) {
				Show_Mouse();
			}
		}
		if (frame >= Get_Animation_Frame_Count(anim)) frame = 0;
		if (Keyboard->Check() && endframe == 255) {
			if ((Keyboard->Get() & 0xFF) == KN_LMOUSE) {
				if ((MouseQY > 48) && (MouseQY < 150)) {
					if ((MouseQX > 18) && (MouseQX < 148)) {

					// Chose GDI
						Whom = HOUSE_GOOD;
						ScenPlayer = SCEN_PLAYER_GDI;
						endframe = 0;
						speechhandle = Play_Sample(speechg);
						speechplaying = true;
						speech = speechg;
					} else if ((MouseQX > 160) && (MouseQX < 300)) {
					// Chose Nod
						selection = 1;
						endframe = 14;
						Whom = HOUSE_BAD;
						ScenPlayer = SCEN_PLAYER_NOD;
						speechhandle = Play_Sample(speechn);
						speechplaying = true;
						speech = speechn;
					}
				}
			}
		}
	}

	Hide_Mouse();
	Close_Animation(anim);

	// erase the "choose side" text
	SeenBuff.Fill_Rect(0, 180, 319, 199, 0);

	Keyboard->Clear();

	/* play the scenario 1 briefing movie */
	if (Whom == HOUSE_GOOD) {
		if (nodbrief) {
			VQA_Close(nodbrief);
			VQA_Free(nodbrief);
		}
		if (gdibrief) {
#ifdef CHEAT_KEYS
#else
			VQA_Play(gdibrief, VQAMODE_RUN);
#endif
			VQA_Close(gdibrief);
			VQA_Free(gdibrief);
		}
	} else {
		if (gdibrief) {
			VQA_Close(gdibrief);
			VQA_Free(gdibrief);
		}
		if (nodbrief) {
#ifdef CHEAT_KEYS
#else
			VQA_Play(nodbrief, VQAMODE_RUN);
#endif
			VQA_Close(nodbrief);
			VQA_Free(nodbrief);
		}
	}

/* get rid of all the animating objects */
	for (int i = 0; i < MAXSCOREOBJS; i++) if (ScoreObjs[i]) {
		delete ScoreObjs[i];
		ScoreObjs[i] = 0;
	}

	if (Whom == HOUSE_GOOD) {
		/*
		** Make sure the screen's fully clear after the movie plays
		*/
		SeenPage.Clear();

		BlackPalette.Adjust(WhitePalette, 0x08);
		BlackPalette.Set();
		BlackPalette.Adjust(0xFF);
		BlackPalette.Set();

//		memset(BlackPalette, 0x01, 768);
//		Set_Palette(BlackPalette);
//		memset(BlackPalette, 0x00, 768);
	} else {
		PreserveVQAScreen = 1;
	}
	Free(staticaud);
	Free(speechg);
	Free(speechn);

	Set_Font(oldfont);
	FontXSpacing = oldfontxspacing;
#endif
}
