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

/* $Header:   F:\projects\c&c\vcs\code\intro.cpv   1.6   16 Oct 1995 16:50:18   JOE_BOSTIC  $ */
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
#include 	"textblit.h"

#ifndef DEMO


#if (0)	//PG_TO_FIX
VQAHandle *Open_Movie(char *name);
VQAHandle *Open_Movie(char *name)
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
#endif //(0)

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
void Choose_Side(void)
{
	//static char const _yellowpal[]={0x0,0x0,0xC9,0x0,0xBA,0x0,0x93,0x0,0x61,0x0,0x0,0x0,0x0,0x0,0xEE,0x0};
	//static char const _redpal[]   ={0x0,0x0,0xA8,0x0,0xD9,0x0,0xDA,0x0,0xE1,0x0,0x0,0x0,0x0,0x0,0xD4,0x0};
	//static char const _graypal[]  ={0x0,0x0,0x17,0x0,0x10,0x0,0x12,0x0,0x14,0x0,0x0,0x0,0x0,0x0,0x1C,0x0};

	// PG_TO_FIX
	// Chose GDI
	Whom = HOUSE_GOOD;
	ScenPlayer = SCEN_PLAYER_GDI;
	return;

#if (0)


	static char const _yellowpal[]={0x0,0xC9,0xBA,0x93,0x61,0xEE,0xee,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
	static char const _redpal[]   ={0x0,0xa8,0xd9,0xda,0xe1,0xd4,0xDA,0x0,0xE1,0x0,0x0,0x0,0x0,0x0,0xD4,0x0};
	static char const _graypal[]  ={0x0,0x17,0x10,0x12,0x14,0x1c,0x12,0x1c,0x14,0x0,0x0,0x0,0x0,0x0,0x1C,0x0};


	void *anim;
	VQAHandle *gdibrief=0, *nodbrief=0;
	void const *staticaud, *oldfont;
	void const *speechg, *speechn, *speech;
	int statichandle, speechhandle, speechplaying = 0;
	int oldfontxspacing = FontXSpacing;
	int setpalette = 0;
	int	gdi_start_palette;

	MEMORYSTATUS	mem_info;
	mem_info.dwLength=sizeof(mem_info);
	GlobalMemoryStatus(&mem_info);

	TextPrintBuffer = new GraphicBufferClass(SeenBuff.Get_Width(), SeenBuff.Get_Height(), (void*)NULL);
	TextPrintBuffer->Clear();
	BlitList.Clear();
	PseudoSeenBuff = new GraphicBufferClass(320,200,(void*)NULL);
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

	if (Special.IsFromInstall){
		if (mem_info.dwTotalPhys >= 12*1024*1024){
			VisiblePage.Clear();
			PreserveVQAScreen = 1;
			Play_Movie("INTRO2", THEME_NONE, false);
		}
		BreakoutAllowed = true;
	}

	//anim = Open_Animation("CHOOSE.WSA",NULL,0L,(WSAOpenType)(WSA_OPEN_FROM_MEM | WSA_OPEN_TO_PAGE),Palette);
	anim = Open_Animation("CHOOSE.WSA",NULL,0L,(WSAOpenType)(WSA_OPEN_FROM_DISK | WSA_OPEN_TO_PAGE),Palette);
	Call_Back();
	InterpolationPaletteChanged = TRUE;
	InterpolationPalette = Palette;
	Read_Interpolation_Palette("SIDES.PAL");

	nodbrief = Open_Movie("NOD1PRE.VQA");
	gdi_start_palette = Load_Interpolated_Palettes("NOD1PRE.VQP");
	Call_Back();
	gdibrief = Open_Movie("GDI1.VQA");
	Load_Interpolated_Palettes("GDI1.VQP" , TRUE);

	WWMouse->Erase_Mouse(&HidPage, TRUE);
	HiddenPage.Clear();
	PseudoSeenBuff->Clear();
	SysMemPage.Clear();
	//if (!Special.IsFromInstall) {
		VisiblePage.Clear();
		Set_Palette(Palette);
	//} else {
		//setpalette = 1;
	//}

	statichandle = Play_Sample(staticaud,255,64);
	CountDownTimerClass sample_timer;
	sample_timer.Set(0x3f);
	Alloc_Object(new ScorePrintClass(TXT_GDI_NAME,   0, 180,_yellowpal));
#ifdef FRENCH
	Alloc_Object(new ScorePrintClass(TXT_GDI_NAME2,  0, 187,_yellowpal));
#endif
	Alloc_Object(new ScorePrintClass(TXT_NOD_NAME, 180, 180,_redpal));

#ifdef GERMAN
	Alloc_Object(new ScorePrintClass(TXT_SEL_TRANS,57, 190,_graypal));
#else
#ifdef FRENCH
	Alloc_Object(new ScorePrintClass(TXT_SEL_TRANS,103, 194,_graypal));
#else
	Alloc_Object(new ScorePrintClass(TXT_SEL_TRANS,103, 190,_graypal));
#endif
#endif
	Keyboard::Clear();

	while (Get_Mouse_State()) Show_Mouse();

	while (endframe != frame || (speechplaying && Is_Sample_Playing(speech)) ) {
		Animate_Frame(anim, SysMemPage, frame++);
		if (setpalette) {
			Wait_Vert_Blank();
			Set_Palette(Palette);
			setpalette = 0;
		}
		SysMemPage.Blit(*PseudoSeenBuff,0,22, 0,22, 320,156);

		/*
		** If the sample has stopped or is about to then restart it
		*/
		if (!Is_Sample_Playing(staticaud) || !sample_timer.Time()) {
			Stop_Sample(statichandle);
			statichandle = Play_Sample(staticaud,255,64);
			sample_timer.Set(0x3f);
		}
		Call_Back_Delay(3);	// delay only if haven't clicked

		/* keep the mouse hidden until the letters are thru printing */
		if (!lettersdone) {
			lettersdone = true;
			for(int i=0; i < MAXSCOREOBJS; i++) if (ScoreObjs[i]) lettersdone = 0;
			if (lettersdone) {
				Show_Mouse();
			}
		}
		if (frame >= Get_Animation_Frame_Count(anim)) frame = 0;
		if (Keyboard::Check() && endframe == 255) {
			if ((Keyboard::Get() & 0x10FF) == KN_LMOUSE) {
				if ((_Kbd->MouseQY > 48*2) && (_Kbd->MouseQY < 150*2)) {
					if ((_Kbd->MouseQX > 18*2) && (_Kbd->MouseQX < 148*2)) {

					// Chose GDI
						Whom = HOUSE_GOOD;
						ScenPlayer = SCEN_PLAYER_GDI;
						endframe = 0;
						speechhandle = Play_Sample(speechg);
						speechplaying = true;
						speech = speechg;

					} else if ((_Kbd->MouseQX > 160*2) && (_Kbd->MouseQX < 300*2)) {
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
	PseudoSeenBuff->Fill_Rect(0,180,319,199,0);
	SeenBuff.Fill_Rect(0,180*2, 319*2, 199*2, 0);
	Interpolate_2X_Scale (PseudoSeenBuff , &SeenBuff ,"SIDES.PAL");
	Keyboard::Clear();
	SysMemPage.Clear();

	/*
	** Skip the briefings if we're in special mode.
	*/
	if (Special.IsJurassic && AreThingiesEnabled) {
		if (nodbrief) {
			VQA_Close(nodbrief);
			VQA_Free(nodbrief);
			nodbrief = NULL;
		}
		if (gdibrief) {
			VQA_Close(gdibrief);
			VQA_Free(gdibrief);
			gdibrief = NULL;
		}
	}

	/* play the scenario 1 briefing movie */
	if (Whom == HOUSE_GOOD) {
		if (nodbrief) {
			VQA_Close(nodbrief);
			VQA_Free(nodbrief);
		}
		if (gdibrief) {
			PaletteCounter = gdi_start_palette;
			VQA_Play(gdibrief, VQAMODE_RUN);
			VQA_Close(gdibrief);
			VQA_Free(gdibrief);
		}
	} else {
		if (gdibrief) {
			VQA_Close(gdibrief);
			VQA_Free(gdibrief);
		}
		if (nodbrief) {
			VQA_Play(nodbrief, VQAMODE_RUN);
			VQA_Close(nodbrief);
			VQA_Free(nodbrief);
		}
	}

	Free_Interpolated_Palettes();
	Set_Primary_Buffer_Format();
/* get rid of all the animating objects */
	for (int i = 0; i < MAXSCOREOBJS; i++) if (ScoreObjs[i]) {
		delete ScoreObjs[i];
		ScoreObjs[i] = 0;
	}

	if (Whom == HOUSE_GOOD) {
		/*
		** Make sure the screen's fully clear after the movie plays
		*/
		VisiblePage.Clear();
		memset(BlackPalette, 0x01, 768);
		Set_Palette(BlackPalette);
		memset(BlackPalette, 0x00, 768);
	} else {
		PreserveVQAScreen = 1;
	}
	Free(staticaud);
	Free(speechg);
	Free(speechn);

	Set_Font(oldfont);
	FontXSpacing = oldfontxspacing;

	delete PseudoSeenBuff;
	delete TextPrintBuffer;
	TextPrintBuffer = NULL;
	BlitList.Clear();

#endif
}
#endif
