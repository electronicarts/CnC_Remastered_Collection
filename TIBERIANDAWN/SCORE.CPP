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

/* $Header:   F:\projects\c&c\vcs\code\score.cpv   2.17   16 Oct 1995 16:49:54   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *   ScoreClass::Delay -- Pauses waiting for keypress.                                         *
 *   ScoreClass::DO_GDI_GRAPH -- Show # of people or buildings killed on GDI score screen      *
 *   ScoreClass::Presentation -- Main routine to display score screen.                         *
 *   ScoreClass::Print_Graph_Title -- Prints title on score screen.                            *
 *   ScoreClass::Print_Minutes -- Print out hours/minutes up to max                            *
 *   ScoreClass::Pulse_Bar_Graph -- Pulses the bargraph color.                                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include  "textblit.h"
#define SCORETEXT_X		184
#define SCORETEXT_Y		8
#define CASUALTY_Y		88
#define BUILDING_X		256
#define BUILDING_Y		128
#define BARGRAPH_X		266
#define MAX_BAR_X			318		// max possible is 319 because of bar's right shadow
#define SIZEGBAR			119
#define HALLFAME_X		19
#define HALLFAME_Y		120

#define	MULTISCOREX		30

#define TEDIT_FAME				1
#define NUMINFANTRYMEN			15
#define NUMFAMENAMES				7
#define MAX_FAMENAME_LENGTH	12

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
void Draw_Bar_Graphs(int i, int gkilled, int nkilled, int ckilled);
void Animate_Cursor(int pos, int ypos);
void Animate_Score_Objs(void);
void Cycle_Wait_Click(void);
int  ScorePass;

void const * Beepy6;
int ControlQ;	// cheat key to skip past score/mapsel screens
bool StillUpdating;

GraphicBufferClass *PseudoSeenBuff;
GraphicBufferClass *TextPrintBuffer;

#ifdef WRITE_LBM
PUBLIC bool CCWrite_LBM_File(CCFileClass &lbmhandle, BufferClass& buff, short bitplanes, unsigned char *palette);
#endif

unsigned char RemapCiv[256]={
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0xD0,0x18,0x19,0xD1,0xD2,0xD3,0xD4,0x1E,0xD5,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0xD6,0xD7,0xD8,0xD9,0x7E,0xDA,
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
	0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xDB,0xDC,0xDD,0xDE,0xDB,0xDC,0xDD,0xDE,0xDF,
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF};

unsigned char const ScoreRemapGrey[256] = {
	  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,	// 0..15
	 16, 17, 18, 19, 20,176, 22,208, 24, 25,209,210,211,212, 30,213,	// 16..31
	 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,	// 32..47
	 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,	// 48..63
	 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,	// 64..79
	 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,	// 80..95
	 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,	// 96..111
	112,113,114,115,116,117,118,119,120,121,214,215,216,217,149,218,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,108,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,219,220,221,222,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const ScoreRemapYellow[256] = {
	  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,	// 0..15
	 16, 17, 18, 19, 20,176, 22,208, 24, 25,209,210,211,212, 30,213,	// 16..31
	 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,	// 32..47
	 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,	// 48..63
	 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,	// 64..79
	 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,	// 80..95
	 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,	// 96..111
	112,113,114,115,116,117,118,119,120,121,214,215,216,217,149,218,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,108,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,219,220,221,222,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const ScoreRemapBldg[256]={
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
	0x70,0x71,0xCE,0xC5,0x49,0x48,0x47,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
	0x80,0x81,0xB8,0x83,0x7C,0x7A,0x76,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
	0x74,0x91,0x92,0x93,0x94,0x95,0xB5,0x97,0x98,0xCF,0x4B,0x7F,0x9C,0x9D,0x9E,0x9F,
	0xA0,0xA1,0xA2,0x83,0x79,0xA5,0xA6,0xA7,0x43,0x99,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xBB,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF
};
unsigned char const ScoreRemapFBall[256]={
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0xB0,0xE5,0x82,0xE4,0xE3,0xE2,0xB1,0xD0,0xE1,0xE0,0xD1,0xD2,0xD3,0xD4,0xDF,0xD5,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0xE6,0xE7,0xA0,0xE8,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
	0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF
};

TextBlitClass BlitList;


char *ScreenNames[2]={"S-GDIIN2.WSA","SCRSCN1.WSA"};

//extern short StreamLowImpact;

// ST - 1/3/2019 10:38AM
int StreamLowImpact = FALSE;

struct Fame {
	char	name[MAX_FAMENAME_LENGTH];
	int	score;
	int	level;
};

ScoreAnimClass *ScoreObjs[MAXSCOREOBJS];


ScoreAnimClass::ScoreAnimClass(int x, int y, void const * data)
{
	BlitList.Add (x*2, y*2, x*2, y*2, 2* String_Pixel_Width ( (char*)data ) , 16);
	XPos = x;
	YPos = y;
	Timer.Set(0);
	Timer.Start();
	DataPtr = data;
}


ScoreTimeClass::ScoreTimeClass(int xpos, int ypos, void const * data, int max, int timer) :
	ScoreAnimClass(xpos, ypos, data)
{
	Stage = 0;
	MaxStage = max;
	TimerReset = timer;
}

void ScoreTimeClass::Update(void)
{
	GraphicViewPortClass *oldpage;
	if (!Timer.Time()) {
		Timer.Set(TimerReset);
		if (++Stage >= MaxStage) Stage = 0;
		oldpage = LogicPage;
		Set_Logic_Page(PseudoSeenBuff);
		CC_Draw_Shape(DataPtr, Stage, XPos, YPos, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);
		Set_Logic_Page(oldpage);
	}
}

ScoreCredsClass::ScoreCredsClass(int xpos, int ypos, void const * data, int max, int timer) :
	ScoreAnimClass(xpos, ypos, data)
{
	Stage = 0;
	MaxStage = max;
	TimerReset = timer;
	Clock1 = MixFileClass::Retrieve("CLOCK1.AUD");
	CashTurn = MixFileClass::Retrieve("CASHTURN.AUD");
}


void ScoreCredsClass::Update(void)
{
	GraphicViewPortClass *oldpage;
	if (!Timer.Time()) {
		Timer.Set(TimerReset);
		if (++Stage >= MaxStage) Stage = 0;
		oldpage = LogicPage;
		Set_Logic_Page(PseudoSeenBuff);
		if (Stage <22) {
			Play_Sample(Clock1, 255, Options.Normalize_Sound(70));
		} else {
			if (Stage==24) {
				Play_Sample(CashTurn, 255, Options.Normalize_Sound(70));
			}
		}
		CC_Draw_Shape(DataPtr,Stage, XPos, YPos, WINDOW_MAIN,SHAPE_WIN_REL, 0, 0);
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
		BlitList.Add (XPos*2, YPos*2, XPos*2, YPos*2,(Stage*6)+14, 8*2);
		delete this;
		return;
	}

	StillUpdating = true;

	if (!Timer.Time()) {
		Timer.Set(1);

		int pos = XPos+(Stage*6);
		if (Stage) {

			localstr[0]=((char *)DataPtr)[Stage-1];

			/*
			** Clear out the white letter overlay
			*/
			static char const _blackpal[]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK};
			Set_Font_Palette(_blackpal);
			TextPrintBuffer->Print(localstr, 2*(pos-6),2*(YPos-1),   TBLACK, TBLACK);
			TextPrintBuffer->Print(localstr, 2*(pos-6),2*(YPos+1),   TBLACK, TBLACK);
			TextPrintBuffer->Print(localstr, 2*(pos-6+1),2*(YPos),   TBLACK, TBLACK);

			Set_Font_Palette(PrimaryPalette);
			TextPrintBuffer->Print(localstr, 2*(pos-6),2*YPos,   TBLACK, TBLACK);
		}
		if (((char *)DataPtr)[Stage]) {
			localstr[0]=((char *)DataPtr)[Stage];
			Set_Font_Palette(_whitepal);
			TextPrintBuffer->Print(localstr, pos*2,  2*(YPos-1), TBLACK, TBLACK);
			TextPrintBuffer->Print(localstr, pos*2,  2*(YPos+1), TBLACK, TBLACK);
			TextPrintBuffer->Print(localstr, (pos+1)*2,2*YPos  , TBLACK, TBLACK);
		}
		Stage++;
	}
}











MultiStagePrintClass::MultiStagePrintClass(int string, int xpos, int ypos, void const * palette, int background) :
	ScoreAnimClass(xpos, ypos, Text_String(string))
{
	Background = background;
	PrimaryPalette = palette;
	Stage = 0;
}


MultiStagePrintClass::MultiStagePrintClass(void const * string, int xpos, int ypos, void const * palette, int background) :
	ScoreAnimClass(xpos, ypos, string)
{
	Background = background;
	PrimaryPalette = palette;
	Stage = 0;
}


void MultiStagePrintClass::Update(void)
{
	static char localstr[2]={0,0};
	static char _whitepal[]={0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F};

	if (Stage && (((char *)DataPtr)[Stage-1]==0) ) {
		for (int i = 0; i < MAXSCOREOBJS; i++) {
			if (ScoreObjs[i] == this) {
				ScoreObjs[i] = 0;
			}
		}
		BlitList.Add (XPos*2, YPos*2, XPos*2, YPos*2,(Stage*6)+14, 8*2);
		delete this;
		return;
	}

	StillUpdating = true;

	if (!Timer.Time()) {
		Timer.Set(1);

		/*
		** Do 10 stages at once
		*/
		for (int wibble = 0 ; wibble < 10 ; wibble ++){

			int pos = XPos+(Stage*6);
			if (Stage) {

				localstr[0]=((char *)DataPtr)[Stage-1];

				/*
				** Clear out the white letter overlay
				*/
				static char const _blackpal[]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK};
				Set_Font_Palette(_blackpal);
				TextPrintBuffer->Print(localstr, 2*(pos-6),2*(YPos-1),   TBLACK, TBLACK);
				TextPrintBuffer->Print(localstr, 2*(pos-6),2*(YPos+1),   TBLACK, TBLACK);
				TextPrintBuffer->Print(localstr, 2*(pos-6+1),2*(YPos),   TBLACK, TBLACK);

				Set_Font_Palette(PrimaryPalette);
				TextPrintBuffer->Print(localstr, 2*(pos-6),2*YPos,   TBLACK, TBLACK);
			}
			if (((char *)DataPtr)[Stage]) {
				localstr[0]=((char *)DataPtr)[Stage];
				Set_Font_Palette(_whitepal);
				TextPrintBuffer->Print(localstr, pos*2,  2*(YPos-1), TBLACK, TBLACK);
				TextPrintBuffer->Print(localstr, pos*2,  2*(YPos+1), TBLACK, TBLACK);
				TextPrintBuffer->Print(localstr, (pos+1)*2,2*YPos  , TBLACK, TBLACK);
			}
			Stage++;

			if ( ( (char *) DataPtr) [Stage-1] == 0 ) break;
		}
	}
}






ScoreScaleClass::ScoreScaleClass(void const * string, int xpos, int ypos, char const palette[]) :
	ScoreAnimClass(xpos, ypos, string)
{
	Palette = &palette[0];
	Stage = 5;
}


void ScoreScaleClass::Update(void)
{
	static int _destx[]={0,80,107,134,180,228};
	static int _destw[]={6,20, 30, 40, 60, 80};

	/*
	** Restore the background for the scaled-up letter
	*/
	if (!Timer.Time()) {
		Timer.Set(1);
		if (Stage != 5) {
			TextPrintBuffer->Blit(HidPage, _destx[Stage+1]*2, YPos*2, _destx[Stage+1]*2, YPos*2, _destw[Stage+1]*2, _destw[Stage+1]*2);
			//SysMemPage.Blit(*PseudoSeenBuff, _destx[Stage+1], YPos, _destx[Stage+1], YPos, _destw[Stage+1], _destw[Stage+1]);
		}
		if (Stage) {
			Set_Font_Palette(Palette);
			TextPrintBuffer->Fill_Rect(0,0, 7*2,7*2, TBLACK);
			TextPrintBuffer->Print((char *)DataPtr, 0,0,   TBLACK, TBLACK);
			TextPrintBuffer->Scale(HidPage, 0,0, _destx[Stage]*2, YPos*2, 5*2,5*2, _destw[Stage]*2, _destw[Stage]*2, true);

			//SysMemPage.Fill_Rect(0,0, 7,7, TBLACK);
			//SysMemPage.Print((char *)DataPtr, 0,0,   TBLACK, TBLACK);
			//SysMemPage.Scale(*PseudoSeenBuff, 0,0, _destx[Stage], YPos, 5,5, _destw[Stage], _destw[Stage], true);
			Stage--;
		} else {
			Set_Font_Palette(Palette);
			for (int i = 0; i < MAXSCOREOBJS; i++) {
				if (ScoreObjs[i]==this) ScoreObjs[i] = 0;
			}
			TextPrintBuffer->Print((char *)DataPtr, XPos*2,YPos*2,   TBLACK, TBLACK);
			//TextPrintBuffer->Blit(HidPage, XPos*2, YPos*2, XPos*2, YPos*2,2*6,2*6);
			//BlitList.Add (XPos, YPos, XPos, YPos, 6,6);

			//SysMemPage.Print((char *)DataPtr, XPos,YPos,   TBLACK, TBLACK);
			//SysMemPage.Blit(*PseudoSeenBuff, XPos,YPos, XPos, YPos, 6,6);
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





TextBlitClass::TextBlitClass (void)
{
	Clear();
}



void TextBlitClass::Add (int x, int y, int dx, int dy, int w, int h)
{
	if ( Count < MAX_ENTRIES ){

		BlitListo [Count].SourceX	= x;
		BlitListo [Count].SourceY	= y;
		BlitListo [Count].DestX 	= dx;
		BlitListo [Count].DestY 	= dy;
		BlitListo [Count].Width		= w;
		BlitListo [Count].Height	= h;

		Count++;
	}
}


void TextBlitClass::Clear(void)
{
	Count = 0;
}




void TextBlitClass::Update(void)
{
	if (TextPrintBuffer){

		if (HidPage.Lock()){

			for (int i=0 ; i<Count ; i++){

				TextPrintBuffer->Blit (HidPage, BlitListo[i].SourceX,
													BlitListo[i].SourceY,
													BlitListo[i].DestX,
													BlitListo[i].DestY,
													BlitListo[i].Width,
													BlitListo[i].Height,
													true);
			}

			HidPage.Unlock();
		}
	}
}



void Disable_Uncompressed_Shapes (void);
void Enable_Uncompressed_Shapes (void);

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
void ScoreClass::Presentation(void)
{
	//static char const _redpal[]={0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x21,0x2F};
	static char const _redpal[]={0x20,0x22,0x24,0x26,0x28,0x28,0x28,0x28,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x21,0x2F};
	static char const _greenpal[]={0x10,0x12,0x14,0x16,0x18,0x18,0x18,0x18,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x10,0x1F};
	static char const _bluepal[]={0x60,0x62,0x64,0x66,0x68,0x68,0x68,0x68,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x61,0x6F};
	//static char const _bluepal[]={0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x61,0x6F};
	static unsigned char const _yellowpal[]={0x0,0x0,0xEC,0x0,0xEB,0x0,0xEA,0x0,0xE9,0x0,0x0,0x0,0x0,0x0,0xED,0x0};
	static int const _casuax[2]={144,146};
	static int const _casuay[2]={ 78, 90};
	static int const _gditxx[2]={150,224};
	static int const _gditxy[2]={ 90, 90};
	static int const _nodtxx[2]={150,224};
	static int const _nodtxy[2]={102,102};
//	static int _bargrx[2]={297,SCORETEXT_X+64};
//	static int _bargry[2]={ 90,CASUALTY_Y + 2};
	static int const _bldggy[2]={138,128};
	static int const _bldgny[2]={150,140};

//	int gdikilled, nodkilled, civkilled, max, i, k, shapenum;
	int i;
	int max;
	void const * yellowptr;
	void const * redptr;
	CCFileClass file(FAME_FILE_NAME);
	struct Fame hallfame[NUMFAMENAMES];
	void *anim, *oldfont;
	int oldfontxspacing = FontXSpacing;
	int house = PlayerPtr->Class->House;		// 0 or 1
	char inter_pal[15];

	/*
	** Choose an appropriate palette file for the interpolation
	*/
	if (house == HOUSE_GOOD){
		sprintf(inter_pal,"SCORPAL1.PAL");
	}else{
		sprintf(inter_pal,"SNODPAL1.PAL");
	}


	if (Special.IsJurassic && AreThingiesEnabled) return;

	PseudoSeenBuff = new GraphicBufferClass(320,200,(void*)NULL);
	TextPrintBuffer = new GraphicBufferClass(SeenBuff.Get_Width(), SeenBuff.Get_Height(), (void*)NULL);
	TextPrintBuffer->Clear();
	BlitList.Clear();
	Disable_Uncompressed_Shapes ();

	ControlQ = 0;
	FontXSpacing = 0;
	Map.Override_Mouse_Shape(MOUSE_NORMAL);
	Theme.Queue_Song(THEME_WIN1);

	VisiblePage.Clear();
	PseudoSeenBuff->Clear();
	SysMemPage.Clear();
	WWMouse->Erase_Mouse(&HidPage, TRUE);
	HiddenPage.Clear();
	Set_Palette(BlackPalette);

	Set_Logic_Page(SysMemPage);

	void const * country4 = MixFileClass::Retrieve("COUNTRY4.AUD");
	void const * sfx4 = MixFileClass::Retrieve("SFX4.AUD");
	Beepy6 = MixFileClass::Retrieve("BEEPY6.AUD");

	/*
	** Load the background for the score screen
	*/
	anim = Open_Animation(ScreenNames[house],NULL,0L,(WSAOpenType)(WSA_OPEN_FROM_MEM | WSA_OPEN_TO_PAGE),Palette);

	unsigned minutes = (unsigned)((ElapsedTime / (long)TIMER_MINUTE))+1;

	/*
	**	Determine leadership rating.
	*/
	unsigned leadership = 0;
	int index;
	for (index = 0; index < Logic.Count(); index++) {
		ObjectClass * object = Logic[index];
		if (object->Owner() == house) {
			leadership++;
		}
	}

	HouseClass *houses[3];
	for (index = 0; index < 3; index++) {
		houses[index] =(HouseClass::As_Pointer((HousesType)(HOUSE_GOOD+index)));
	}

	GKilled  = (HouseClass::As_Pointer(HOUSE_GOOD))->UnitsLost;
	NKilled  = (HouseClass::As_Pointer(HOUSE_BAD))->UnitsLost;
	CKilled  = (HouseClass::As_Pointer(HOUSE_NEUTRAL))->UnitsLost;
	GBKilled = (HouseClass::As_Pointer(HOUSE_GOOD))->BuildingsLost;
	NBKilled = (HouseClass::As_Pointer(HOUSE_BAD))->BuildingsLost;
	CBKilled = (HouseClass::As_Pointer(HOUSE_NEUTRAL))->BuildingsLost;

	/*
	** New  - ST 6/12/96 2:40PM
	*/
	GHarvested = (HouseClass::As_Pointer(HOUSE_GOOD))->HarvestedCredits;
	NHarvested = (HouseClass::As_Pointer(HOUSE_BAD))->HarvestedCredits;

	if (!leadership) leadership++;
	leadership = Cardinal_To_Fixed(GKilled+GBKilled+leadership, leadership);
	leadership = Fixed_To_Cardinal(100, leadership);
	if (leadership > 100) leadership = 100;

	/*
	**	Determine efficiency rating.
	*/
	int gharv = GHarvested;
	int init = PlayerPtr->InitialCredits;
	int cred = PlayerPtr->Available_Money();

	unsigned efficiency = Cardinal_To_Fixed( (house == HOUSE_GOOD ? GHarvested : NHarvested) + (unsigned)PlayerPtr->InitialCredits+1, (unsigned)PlayerPtr->Available_Money()+1);
	if (!efficiency) efficiency++;
	efficiency = Fixed_To_Cardinal(100, efficiency);

	if (efficiency > 100) efficiency = 100;
	/*
	** Calculate total score
	*/
	long total = ((leadership * 40) + (4600) + (efficiency * 14))/100;
	if (!total) total++;
	total *= (BuildLevel+1);

// Load up the shapes for the Nod score screen
	if (house == HOUSE_GOOD) {
		yellowptr = MixFileClass::Retrieve("BAR3YLW.SHP");
		redptr = MixFileClass::Retrieve("BAR3RED.SHP");
	}

/* Change to the six-point font for Text_Print */
	oldfont = Set_Font(ScoreFontPtr);
	Call_Back();

/* --- Now display the background animation --- */
	Hide_Mouse();
	Animate_Frame(anim, SysMemPage, 1);
	SysMemPage.Blit(*PseudoSeenBuff);
	Increase_Palette_Luminance (Palette , 30,30,30,63);
	InterpolationPalette = Palette;
	InterpolationPaletteChanged = TRUE;
	Read_Interpolation_Palette(inter_pal);
	Interpolate_2X_Scale( PseudoSeenBuff , &SeenBuff , inter_pal);
	Fade_Palette_To(Palette, FADE_PALETTE_FAST, Call_Back);

	Play_Sample(country4, 255, Options.Normalize_Sound(90));

	int frame = 1;
	StreamLowImpact = true;
	while (frame < Get_Animation_Frame_Count(anim)) {
		Animate_Frame(anim, *PseudoSeenBuff, frame++);
		////////////////Interpolate_2X_Scale( PseudoSeenBuff , &SeenBuff , NULL);
		Call_Back_Delay(2);
	}
	StreamLowImpact = false;
	Call_Back();
	Close_Animation(anim);

	/*
	** Background's up, so now load various shapes and animations
	*/
	void const * timeshape = MixFileClass::Retrieve("TIME.SHP");
	ScoreObjs[0] = new ScoreTimeClass(233, 2, timeshape, 30, 4);

	void const * hiscore1shape = MixFileClass::Retrieve("HISCORE1.SHP");
	void const * hiscore2shape = MixFileClass::Retrieve("HISCORE2.SHP");
	ScoreObjs[1] = new ScoreTimeClass(4, 97, hiscore1shape, 10, 4);
	ScoreObjs[2] = new ScoreTimeClass(8, 172, hiscore2shape, 10, 4);

	/* Now display the stuff */
	PseudoSeenBuff->Blit(SysMemPage);


	if (house == HOUSE_BAD) {

		/*
		** load the logo
		*/
		void const * logoptr = MixFileClass::Retrieve("LOGOS.SHP");
		CC_Draw_Shape(logoptr, 1, 0, 0, WINDOW_MAIN, SHAPE_WIN_REL, 0, 0);

		Bit_It_In_Scale(0,0, 128,104-16, &SysMemPage, PseudoSeenBuff, &SeenBuff , 1);
	}

	Set_Logic_Page(PseudoSeenBuff);

#ifdef FRENCH
	Alloc_Object(new ScorePrintClass(TXT_SCORE_TIME, 200,  3,_greenpal));
#else
	Alloc_Object(new ScorePrintClass(TXT_SCORE_TIME, 206,  3,_greenpal));
#endif
	Alloc_Object(new ScorePrintClass(TXT_SCORE_LEAD, 182, 26,_greenpal));
	Alloc_Object(new ScorePrintClass(TXT_SCORE_EFFI, 182, 38,_greenpal));
	Alloc_Object(new ScorePrintClass(TXT_SCORE_TOTA, 182, 50,_greenpal));
	Play_Sample(sfx4, 255, Options.Normalize_Sound(120));
	Call_Back_Delay(13);

	max = MAX((long)leadership, (long)efficiency);
	int scorecounter = 0;
	Keyboard::Clear();

	BlitList.Add (264*2, 26*2, 264*2, 26*2, 4*12 , 12);
	BlitList.Add (264*2, 38*2, 264*2, 38*2, 4*12 , 12);
	BlitList.Add (264*2, 50*2, 264*2, 50*2, 4*12 , 12);
	BlitList.Add (275*2, 9*2, 275*2, 9*2, 64, 12 );	//Minutes
	for (i = 0; i <= 160; i++) {
		Set_Font_Palette(_greenpal);
		Count_Up_Print("%3d%%", i,    leadership,         264, 26);
		if (i>=30) Count_Up_Print("%3d%%", i-30,    efficiency,         264, 38);
		if (i>=60) {
			Count_Up_Print("%3d",scorecounter, total,         264, 50);
			scorecounter += total/100;
		}
		Print_Minutes(minutes);
		Call_Back_Delay(1);
		Play_Sample(Beepy6, 255, Options.Normalize_Sound(60));
		if (Check_Key() && i < (max-5) ) {
			i=158;
			Keyboard::Clear();
		}
	}
	Count_Up_Print("%3d", total, total, 264, 50);

	Call_Back_Delay(60);

	if (house == HOUSE_BAD) Show_Credits(house, _greenpal);

	Call_Back_Delay(60);

	/*
	** Show stats on # of units killed
	*/
	Set_Logic_Page(*PseudoSeenBuff);
	Play_Sample(sfx4, 255, Options.Normalize_Sound(90));
	Alloc_Object(new ScorePrintClass(TXT_SCORE_CASU, _casuax[house], _casuay[house],_redpal));
	Call_Back_Delay(9);
	if (house == HOUSE_BAD) {
		Alloc_Object(new ScorePrintClass(TXT_SCORE_NEUT, 200, 114,_redpal));
		Call_Back_Delay(4);
	}

	Alloc_Object(new ScorePrintClass(TXT_SCORE_GDI, _gditxx[house], _gditxy[house],_redpal));
	Alloc_Object(new ScorePrintClass(TXT_SCORE_NOD, _nodtxx[house], _nodtxy[house],_redpal));
	Call_Back_Delay(6);

	Set_Font_Palette(_redpal);
	if (house == HOUSE_BAD) {
		Do_Nod_Casualties_Graph();
	} else {
		Do_GDI_Graph(yellowptr, redptr, GKilled + CKilled, NKilled, 88);
	}

	Set_Logic_Page(*PseudoSeenBuff);

	/*
	** Print out stats on buildings destroyed
	*/
	Play_Sample(sfx4, 255, Options.Normalize_Sound(90));
	if (house == HOUSE_GOOD) {
		Alloc_Object(new ScorePrintClass(TXT_SCORE_BUIL, 144, 126,_greenpal));
		Call_Back_Delay(9);
	} else {
		Alloc_Object(new ScorePrintClass(TXT_SCORE_BUIL1, 146, 128,_greenpal));
		Alloc_Object(new ScorePrintClass(TXT_SCORE_BUIL2, 146, 136,_greenpal));
		Call_Back_Delay(9);
		Alloc_Object(new ScorePrintClass(TXT_SCORE_NEUT, 200, 152,_greenpal));
		Call_Back_Delay(4);
	}
	Alloc_Object(new ScorePrintClass(TXT_SCORE_GDI, _gditxx[house], _bldggy[house],_greenpal));
	Alloc_Object(new ScorePrintClass(TXT_SCORE_NOD, _gditxx[house], _bldgny[house],_greenpal));
	Call_Back_Delay(7);

	if (house==HOUSE_BAD) {
		Call_Back_Delay(6);
		Set_Font_Palette(_greenpal);
		Do_Nod_Buildings_Graph();
	} else {
		Do_GDI_Graph(yellowptr, redptr, GBKilled + CBKilled, NBKilled, 136);
	}

	// Wait for text printing to complete
	while (StillUpdating){
		Call_Back_Delay(1);
	}

	if (Keyboard::Check()) Keyboard::Clear();

	if (house == HOUSE_GOOD) Show_Credits(house, _greenpal);

	/*
	** Hall of fame display and processing
	*/
	Play_Sample(sfx4, 255, Options.Normalize_Sound(90));
	Alloc_Object(new ScorePrintClass(TXT_SCORE_TOP, 28, 110,_bluepal));
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
	if(hallfame[NUMFAMENAMES-1].score >= total)
								hallfame[NUMFAMENAMES-1].score = 0;
	for (index = 0; index < NUMFAMENAMES; index++) {
		if (total > hallfame[index].score) {
			if (index < (NUMFAMENAMES-1)) for (i = (NUMFAMENAMES-1); i > index; i--) hallfame[i] = hallfame[i-1];
			hallfame[index].score = total;
			hallfame[index].level = Scenario;
//			hallfame[index].level = BuildLevel;
			//hallfame[index].name[0] = 0;	// blank out the name
			memset (hallfame[index].name, ' ', sizeof (hallfame[index].name) -1);
			hallfame[index].name[sizeof (hallfame[index].name)-1] = 0;
			break;
		}
	}

	/*
	** Now display the hall of fame
	*/
	Set_Logic_Page(*PseudoSeenBuff);
	for (i = 0; i < NUMFAMENAMES; i++) {
		Alloc_Object(new ScorePrintClass(hallfame[i].name, HALLFAME_X, HALLFAME_Y + (i*8), _bluepal));
		if (hallfame[i].score) {
			char *str = (char *)(SysMemPage.Get_Buffer()) + i*32;
			sprintf(str,"%d", hallfame[i].score);
			Alloc_Object(new ScorePrintClass(str, HALLFAME_X+(6*15), HALLFAME_Y + (i*8), _bluepal, BLACK));
			if (hallfame[i].level < 20) {
				sprintf(str+16,"%d", hallfame[i].level);
			} else {
				strcpy(str+16, "**");
			}
			Alloc_Object(new ScorePrintClass(str+16, HALLFAME_X+(6*12), HALLFAME_Y + (i*8), _bluepal, BLACK));
			Call_Back_Delay(13);
		}
	}

	// Wait for text printing to complete
	while (StillUpdating){
		Call_Back_Delay(1);
	}

	/*
	** If the player's on the hall of fame, have him enter his name now
	*/
	Keyboard::Clear();
	if (index < NUMFAMENAMES) {
		Input_Name(hallfame[index].name, HALLFAME_X, HALLFAME_Y + (index*8), _bluepal);

		file.Open(WRITE);
		for (i = 0; i < NUMFAMENAMES; i++) {
			file.Write(&hallfame[i], sizeof(struct Fame));
		}
		file.Close();
	} else {
#ifdef FRENCH
		Alloc_Object(new ScorePrintClass(TXT_MAP_CLICK2, 145, 190, _yellowpal));
#else
		Alloc_Object(new ScorePrintClass(TXT_MAP_CLICK2, 149, 190, _yellowpal));
#endif
		Cycle_Wait_Click();
	}

#ifdef WRITE_LBM
	file.Open("e:scorscrn.lbm",WRITE);
	SeenBuff.Blit(SysMemPage,0,0, 0,0, 320,200);
	CCWrite_LBM_File(file, SysMemPage, 8, Palette);
	file.Close();
#endif

	Keyboard::Clear();

/* get rid of all the animating objects */
	for (i = 0; i < MAXSCOREOBJS; i++) if (ScoreObjs[i]) {
		delete ScoreObjs[i];
		ScoreObjs[i] = 0;
	}
	Fade_Palette_To(BlackPalette, FADE_PALETTE_FAST, NULL);
	VisiblePage.Clear();

	Show_Mouse();
//	Map_Selection();

	Theme.Queue_Song(THEME_NONE);

	Fade_Palette_To(BlackPalette, FADE_PALETTE_FAST, NULL);
	VisiblePage.Clear();
	Set_Palette(GamePalette);

	Set_Font(oldfont);
	FontXSpacing = oldfontxspacing;
	ControlQ = 0;

	Set_Logic_Page (SeenBuff);

	delete PseudoSeenBuff;
	delete TextPrintBuffer;
	TextPrintBuffer = NULL;
	BlitList.Clear();
	Enable_Uncompressed_Shapes();
}

// ST = 12/17/2018 5:44PM
#ifndef TickCount
extern TimerClass					TickCount;
#endif

void Cycle_Wait_Click(void)
{
	int counter = 0;
	int minclicks = 20;
	unsigned long timingtime = TickCount.Time();
	//SerialPacketType sendpacket;
	//SerialPacketType receivepacket;
	//int packetlen;


	Keyboard::Clear();
	while (minclicks || (!Check_Key() && !ControlQ) ) {

		if (GameToPlay == GAME_NULL_MODEM ||
			GameToPlay == GAME_MODEM){
			//GameToPlay == GAME_INTERNET) {
#if (0)	//PG_TO_FIX
			//
			// send a timing packet if enough time has gone by.
			//
			if ( (TickCount.Time() - timingtime) > PACKET_TIMING_TIMEOUT) {
				sendpacket.Command = SERIAL_SCORE_SCREEN;
				sendpacket.ResponseTime = NullModem.Response_Time();
				sendpacket.ID = ModemGameToPlay;

				NullModem.Send_Message (&sendpacket, sizeof(sendpacket), 0);
				timingtime = TickCount.Time();
			}

			if (NullModem.Get_Message (&receivepacket, &packetlen) > 0) {
				// throw packet away
				packetlen = packetlen;
			}

			NullModem.Service();
#endif
		}

		Call_Back_Delay(1);
		if (minclicks) {
			minclicks--;
			Keyboard::Clear();
		}

		counter = ((++counter) & 7);

		if (!counter) {
			unsigned char r,g,b;

			r = Palette[233*3+0];
			g = Palette[233*3+1];
			b = Palette[233*3+2];

			for (int i = 233; i < 237; i++) {
				Palette[i*3+0] = Palette[(i+1)*3+0];
				Palette[i*3+1] = Palette[(i+1)*3+1];
				Palette[i*3+2] = Palette[(i+1)*3+2];
			}
			Palette[237*3+0] = r;
			Palette[237*3+1] = g;
			Palette[237*3+2] = b;

			Set_Palette(Palette);
		}
	}
	Keyboard::Clear();
}

void ScoreClass::Do_Nod_Buildings_Graph(void)
{
	int shapenum;
	InfantryTypeClass const *ramboclass;

	void const * factptr   = MixFileClass::Retrieve("FACT.SHP");
	void const * rmboptr   = MixFileClass::Retrieve("RMBO.SHP");
	void const * fball1ptr = MixFileClass::Retrieve("FBALL1.SHP");
	ramboclass = &InfantryTypeClass::As_Reference(INFANTRY_E5);

	/*
	** Print the # of buildings on the hidpage so we only need to do it once
	*/
	PseudoSeenBuff->Blit(SysMemPage);
	Set_Logic_Page(SysMemPage);
	Call_Back_Delay(30);
	BlitList.Add (2* (BUILDING_X + 8), 2* (BUILDING_Y), 2* (BUILDING_X+8), 2* (BUILDING_Y), 5*12 , 12);
	BlitList.Add (2* (BUILDING_X + 8), 2* (BUILDING_Y + 12), 2* (BUILDING_X+8), 2* (BUILDING_Y + 12), 5*12 , 12);
	BlitList.Add (2* (BUILDING_X + 8), 2* (BUILDING_Y + 24), 2* (BUILDING_X+8), 2* (BUILDING_Y + 24), 5*12 , 12);

	TextPrintBuffer->Print( 0, (BUILDING_X + 8)*2, BUILDING_Y*2,      TBLACK, TBLACK);
	TextPrintBuffer->Print( 0, (BUILDING_X + 8)*2, (BUILDING_Y + 12)*2, TBLACK, TBLACK);
	TextPrintBuffer->Print( 0, (BUILDING_X + 8)*2, (BUILDING_Y + 24)*2, TBLACK, TBLACK);

	/*
	** Here's the animation/draw loop for blowing up the factory
	*/
	int i;
	for (i=0; i<98; i++) {
		SysMemPage.Blit(SysMemPage, BUILDING_X, BUILDING_Y, 0, 0, 320-BUILDING_X,48);
		shapenum = 0;	// no damage
		if (i >= 60) {
			shapenum = Extract_Shape_Count(factptr) - 2;	// some damage
			if (i == 60) {
				Shake_The_Screen(6);
				Sound_Effect(VOC_CRUMBLE, VOL_FULL, 0);
			}
			if (i > 65) {
				shapenum = Extract_Shape_Count(factptr) - 1;	// mega damage
			}
		}

		/*
		** Draw the building before Rambo
		*/
		if (i < 68) {
			CC_Draw_Shape(factptr,shapenum, 0, 0, WINDOW_MAIN,
					  SHAPE_FADING|SHAPE_WIN_REL, ScoreRemapBldg,Map.UnitShadow);

		}

		/*
		** Now draw some fires, if appropriate
		*/
		if (i >= 61) {
			int firecount = Extract_Shape_Count(fball1ptr);
			int shapeindex = (i-61)>>1;
			if (shapeindex < firecount) {
				CC_Draw_Shape(fball1ptr, shapeindex, 10, 10, WINDOW_MAIN,
					  SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL,//|SHAPE_GHOST,
					  ScoreRemapFBall,0);
			}
			if (i > 64) {
				shapeindex = (i-64)>>1;
				if (shapeindex < firecount) {
					CC_Draw_Shape(fball1ptr, shapeindex, 50, 30, WINDOW_MAIN,
						  SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL,//|SHAPE_GHOST,
						  ScoreRemapFBall,0);
				}
			}
		}
		/*
		** Draw the Rambo character running away from the building
		*/
		CC_Draw_Shape(rmboptr, (ramboclass->DoControls[DO_WALK].Frame + ramboclass->DoControls[DO_WALK].Jump*6) + ((i>>1)%ramboclass->DoControls[DO_WALK].Count),
						  i+32, 40, WINDOW_MAIN,
						  SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL,//|SHAPE_GHOST,
						  ScoreRemapYellow,Map.UnitShadow);
		SysMemPage.Blit(*PseudoSeenBuff, 0, 0, BUILDING_X, BUILDING_Y, 320-BUILDING_X,48);

		/*
		** Extra font related stuff. ST - 7/29/96 2:22PM
		*/
		Interpolate_2X_Scale(PseudoSeenBuff , &HidPage ,NULL);
		BlitList.Update();
		WWMouse->Draw_Mouse(&HidPage);
		Blit_Hid_Page_To_Seen_Buff();
		WWMouse->Erase_Mouse(&HidPage, TRUE);
		//Interpolate_2X_Scale( PseudoSeenBuff , &SeenBuff , NULL);

		if (!Check_Key()) Call_Back_Delay(1);
	}

	i = MAX(GBKilled, NBKilled);
	i = MAX(i, CBKilled);

	for (int q = 0; q <= i; q++) {
		Count_Up_Print( "%d", q, GBKilled,BUILDING_X + 8,BUILDING_Y     );
		Count_Up_Print( "%d", q, NBKilled,BUILDING_X + 8,BUILDING_Y + 12);
		Count_Up_Print( "%d", q, CBKilled,BUILDING_X + 8,BUILDING_Y + 24);
		if (!Check_Key()) {
			Play_Sample(Beepy6, 255, Options.Normalize_Sound(110));
			Call_Back_Delay(1);
		}
	}
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
	int i, max;
	int gdikilled = gkilled, nodkilled=nkilled;

	max = MAX(gdikilled, nodkilled);
	if (!max) max=1;

	gdikilled = (gdikilled * SIZEGBAR) / max;
	nodkilled = (nodkilled * SIZEGBAR) / max;
	if (max < 20) {
		gdikilled = gkilled * 5;
		nodkilled = nkilled * 5;
	}

	max = MAX(gdikilled, nodkilled);
	if (!max) max=1;

	// Draw the white-flash shape on the hidpage
	Set_Logic_Page(SysMemPage);
	SysMemPage.Fill_Rect(0,0, 124,9, TBLACK);
	CC_Draw_Shape(redptr, 120, 0,0, WINDOW_MAIN,SHAPE_WIN_REL, 0, 0);
	Set_Logic_Page(PseudoSeenBuff);

	BlitList.Add (2* 297, 2* (ypos+2), 2* 297, 2* (ypos+2), 5*12 , 12);

	for (i = 1; i <= gdikilled; i++) {
		if (i != gdikilled) {
			CC_Draw_Shape(yellowptr,i, 172, ypos, WINDOW_MAIN,SHAPE_WIN_REL, 0, 0);
		} else {
			SysMemPage.Blit(*PseudoSeenBuff,0,0, 172, ypos, 3+gdikilled,9);
		}

		Count_Up_Print("%d", (i*gkilled) / max, gkilled, 297, ypos+2);
		if (!Check_Key()) {
			Play_Sample(Beepy6, 255, Options.Normalize_Sound(110));
			Call_Back_Delay(2);
		}
	}
	CC_Draw_Shape(yellowptr,gdikilled, 172,ypos   , WINDOW_MAIN,SHAPE_WIN_REL, 0, 0);
	Count_Up_Print("%d", gkilled, gkilled, 297, ypos+ 2);
	if (!Check_Key()) Call_Back_Delay(40);

	BlitList.Add (2* 297, 2* (ypos+14), 2* 297, 2* (ypos+14), 5*12 , 12);
	for (i = 1; i <= nodkilled; i++) {
		if (i != nodkilled) {
			CC_Draw_Shape(redptr, i, 172, ypos+12, WINDOW_MAIN,SHAPE_WIN_REL, 0, 0);
		} else {
			SysMemPage.Blit(*PseudoSeenBuff,0,0, 172,ypos+12, 3+nodkilled,9);
		}

		Count_Up_Print("%d", (i*nkilled) / max, nkilled, 297, ypos+14);
		if (!Check_Key()) {
			Play_Sample(Beepy6, 255, Options.Normalize_Sound(110));
			Call_Back_Delay(2);
		}
	}

//	if (Keyboard::Check()) Keyboard::Clear();

	/*
	** Make sure accurate count is printed at end
	*/
	CC_Draw_Shape(   redptr,nodkilled, 172,ypos+12, WINDOW_MAIN,SHAPE_WIN_REL, 0, 0);
	Count_Up_Print("%d", nkilled, nkilled, 297, ypos+14);
	if (!Check_Key()) Call_Back_Delay(40);
}


void ScoreClass::Do_Nod_Casualties_Graph(void)
{
	int i, gdikilled, nodkilled, civkilled, max;

	void const * e1ptr = MixFileClass::Retrieve("E1.SHP");
	void const * c1ptr = MixFileClass::Retrieve("C1.SHP");

	gdikilled = GKilled;
	nodkilled = NKilled;
	civkilled = CKilled;
	max = MAX(gdikilled, nodkilled);
	max = MAX(max, civkilled);

	if (!max) max=1;
	if ((gdikilled > (MAX_BAR_X - BARGRAPH_X)) || (nodkilled > (MAX_BAR_X - BARGRAPH_X)) || (civkilled > (MAX_BAR_X - BARGRAPH_X))) {
		gdikilled = (gdikilled * (MAX_BAR_X - BARGRAPH_X)) / max;
		nodkilled = (nodkilled * (MAX_BAR_X - BARGRAPH_X)) / max;
		civkilled = (civkilled * (MAX_BAR_X - BARGRAPH_X)) / max;
	}

	max = MAX(gdikilled, nodkilled);
	max = MAX(max, civkilled);
	if (!max) max=1;

	/*
	** Initialize a bunch of objects for the infantrymen who pose for the bar
	** graphs of casualties.
	*/
	int q = NUMINFANTRYMEN/3;
	int r = q*2;
	for (i = 0; i < NUMINFANTRYMEN/3; i++) {
		InfantryMan[i+0].xpos =
		InfantryMan[i+q].xpos =
		InfantryMan[i+r].xpos =  (i*10) + 7;
		InfantryMan[i+0].ypos = 11;
		InfantryMan[i+q].ypos = 21;
		InfantryMan[i+r].ypos = 31;
		InfantryMan[i+0].shapefile =
		InfantryMan[i+q].shapefile = e1ptr;
		InfantryMan[i+r].shapefile = c1ptr;
		InfantryMan[i+0].remap = ScoreRemapYellow;
		InfantryMan[i+q].remap = ScoreRemapGrey;
		InfantryMan[i+r].remap = RemapCiv;
		InfantryMan[i+0].anim  =
		InfantryMan[i+q].anim  =
		InfantryMan[i+r].anim  = 0;
		InfantryMan[i+0].stage =
		InfantryMan[i+q].stage =
		InfantryMan[i+r].stage = 0;
		InfantryMan[i+0].delay =
		InfantryMan[i+q].delay =
		InfantryMan[i+r].delay = Random() & 0x1F;
		InfantryMan[i+0].Class =
		InfantryMan[i+q].Class = &InfantryTypeClass::As_Reference(INFANTRY_E1);
		InfantryMan[i+r].Class = &InfantryTypeClass::As_Reference(INFANTRY_C1);

	}

	/*
	** Draw the infantrymen and pause briefly before running the graph
	*/
	Draw_InfantryMen();
	SysMemPage.Blit(*PseudoSeenBuff, 0, 0, BARGRAPH_X, CASUALTY_Y, 320-BARGRAPH_X, 34);
	//Interpolate_2X_Scale( PseudoSeenBuff , &SeenBuff, NULL);
	/*
	** Extra font related stuff. ST - 7/29/96 2:22PM
	*/
	Interpolate_2X_Scale(PseudoSeenBuff , &HidPage ,NULL);
	BlitList.Update();
	WWMouse->Draw_Mouse(&HidPage);
	Blit_Hid_Page_To_Seen_Buff();
	WWMouse->Erase_Mouse(&HidPage, TRUE);

	Call_Back_Delay(40);

	BlitList.Add (2* (SCORETEXT_X + 64), 2* (CASUALTY_Y + 2), 2* (SCORETEXT_X + 64), 2* (CASUALTY_Y + 2), 5*12 , 12);
	BlitList.Add (2* (SCORETEXT_X + 64), 2* (CASUALTY_Y + 14), 2* (SCORETEXT_X + 64), 2* (CASUALTY_Y + 14), 5*12 , 12);
	BlitList.Add (2* (SCORETEXT_X + 64), 2* (CASUALTY_Y + 26), 2* (SCORETEXT_X + 64), 2* (CASUALTY_Y + 26), 5*12 , 12);

	for (i = 1; i <= max; i++) {
		// Draw & update infantrymen 3 times for every tick on the graph (i)
		for (int q = 0; q < 3; q++) {
			Draw_InfantryMen();
			Draw_Bar_Graphs(i, gdikilled, nodkilled, civkilled);
			SysMemPage.Blit(*PseudoSeenBuff, 0, 0, BARGRAPH_X, CASUALTY_Y, 320-BARGRAPH_X, 34);

			Count_Up_Print("%d", (i*GKilled) / max, GKilled, SCORETEXT_X+64, CASUALTY_Y +  2);
			Count_Up_Print("%d", (i*NKilled) / max, NKilled, SCORETEXT_X+64, CASUALTY_Y + 14);
			Count_Up_Print("%d", (i*CKilled) / max, CKilled, SCORETEXT_X+64, CASUALTY_Y + 26);
			if (!Check_Key()) Call_Back_Delay(3);
		}
		Play_Sample(Beepy6, 255, Options.Normalize_Sound(110));
	}
	if (Check_Key()) Keyboard::Clear();

	/*
	** Make sure accurate count is printed at end
	*/
	Count_Up_Print("%d", GKilled, GKilled, SCORETEXT_X+64, CASUALTY_Y +  2);
	Count_Up_Print("%d", NKilled, NKilled, SCORETEXT_X+64, CASUALTY_Y + 14);
	Count_Up_Print("%d", CKilled, CKilled, SCORETEXT_X+64, CASUALTY_Y + 26);

	/*
	** Finish up death animations, if there are any active
	*/
	int k = 1;
	while (k) {
		for (i=k=0; i<NUMINFANTRYMEN; i++) if (InfantryMan[i].anim >= DO_GUN_DEATH) k=1;
		if (k) Draw_InfantryMen();
		Draw_Bar_Graphs(max, gdikilled, nodkilled, civkilled);
		SysMemPage.Blit(*PseudoSeenBuff, 0, 0, BARGRAPH_X, CASUALTY_Y, 320-BARGRAPH_X, 34);
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
	int min,add;

	void const * credshape = MixFileClass::Retrieve("CREDS.SHP");

	Alloc_Object(new ScorePrintClass(TXT_SCORE_ENDCRED, _credtx[house], _credty[house],pal));
	Call_Back_Delay(15);

	credobj = Alloc_Object(new ScoreCredsClass(_credsx[house], _credsy[house], credshape, 32, 2));
	min = PlayerPtr->Available_Money() / 100;

	/*
	** Print out total credits left at end of scenario
	*/
	i = -50;

	BlitList.Add (2* (_credpx[house]), 2* (_credpy[house]), 2* (_credpx[house]), 2* (_credpy[house]), 5*12 , 12);

	do {
		add = 5;
		if ((PlayerPtr->Available_Money() - i) > 100 ) add += 15;
		if ((PlayerPtr->Available_Money() - i) > 1000) add += 30;
		if (add < min) add = min;
		i += add;

		if (i < 0) i=0;

		Set_Font_Palette(pal);
		Count_Up_Print("%d", i, PlayerPtr->Available_Money(), _credpx[house], _credpy[house]);
		Call_Back_Delay(2);
		if (Check_Key()) {
			i=PlayerPtr->Available_Money() - 5;
			Keyboard::Clear();
		}
	} while (i < PlayerPtr->Available_Money()) ;

	// Make sure the credits object doesn't freeze on the white stage
	while (((ScoreTimeClass *)ScoreObjs[credobj])->Stage >= 20 && !ControlQ) {
		Call_Back_Delay(1);
	}
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
		sprintf(str,Text_String(TXT_SCORE_TIMEFORMAT1), (minutes / 60),(minutes % 60));
	} else {
		sprintf(str,Text_String(TXT_SCORE_TIMEFORMAT2), minutes);
	}
	TextPrintBuffer->Print(str, 275*2, 9*2, TBLACK, TBLACK);
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
void ScoreClass::Count_Up_Print(char *str, int percent, int max, int xpos, int ypos)
{
	char destbuf[64];
	int width;

	sprintf(destbuf, str, percent <= max ? percent : max);
	width = strlen(destbuf) * 7;




//	HidPage.Blit(HidPage, xpos, ypos, 0, 0, width, 8);
//	Set_Logic_Page(HidPage);
//	LogicPage->Print(	destbuf, 0, 0, WHITE, TBLACK);
//	HidPage.Blit(SeenBuff, 0, 0, xpos, ypos, width, 8);

	TextPrintBuffer->Fill_Rect (xpos*2, ypos*2, (xpos + width)*2, (ypos+7)*2, BLACK);
	TextPrintBuffer->Print (destbuf, xpos*2, ypos*2, WHITE, TBLACK);


	//TextPrintBuffer->Blit(*TextPrintBuffer, xpos*2, ypos*2, 0, 0, width*2, 8*2);
	//TextPrintBuffer->Print(destbuf, 0, 0, WHITE, TBLACK);
	//TextPrintBuffer->Blit(SeenBuff, 0, 0, xpos*2, ypos*2, width*2, 8*2);

//	PseudoSeenBuff->Print(	destbuf, xpos, ypos, TBLACK, BLACK);
//	Interpolate_2X_Scale( PseudoSeenBuff , &SeenBuff , NULL);

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
	int key, ascii, index=0;

	void const * keystrok = MixFileClass::Retrieve("KEYSTROK.AUD");

	/*
	** Ready the hidpage so it can restore background under zoomed letters
	*/
	PseudoSeenBuff->Blit(SysMemPage);

	do {
		Call_Back();
		Animate_Score_Objs();
		Animate_Cursor(index, ypos);
		/*
		** Extra font related stuff. ST - 7/29/96 2:22PM
		*/
		Interpolate_2X_Scale (PseudoSeenBuff , &HidPage ,NULL);
		BlitList.Update();
		Blit_Hid_Page_To_Seen_Buff();

		if (Check_Key()) {						//if (Keyboard::Check()) {
			key = Get_Key();						//key = Keyboard::Get();

			if (index == MAX_FAMENAME_LENGTH-2) {
				while (Check_Key()) {
					Get_Key();
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

					int xposindex6 = xpos+(index*6);

					PseudoSeenBuff->Fill_Rect(xposindex6,ypos,xposindex6+6,ypos+6,TBLACK);
					SysMemPage.Fill_Rect(xposindex6,ypos,xposindex6+6,ypos+6,TBLACK);
					TextPrintBuffer->Fill_Rect(xposindex6*2,ypos*2,(xposindex6+6)*2,(ypos+6)*2, BLACK);
				}

			} else if (key!=KA_RETURN) {				//else if (key != KN_RETURN && key!=KN_KEYPAD_RETURN) {
				ascii = key;					//ascii = KN_To_KA(key);
				if (ascii >= 'a' && ascii <= 'z') ascii -= ('a' - 'A');
//if (ascii >='A' && ascii<='Z' || ascii == ' ') {
if ( (ascii >= '!' && ascii <= KA_TILDA) || ascii == ' ') {
					PseudoSeenBuff->Fill_Rect(xpos + (index*6), ypos, xpos + (index*6)+6, ypos+5, TBLACK);
					 SysMemPage.Fill_Rect(xpos + (index*6), ypos, xpos + (index*6)+6, ypos+5, TBLACK);
					TextPrintBuffer->Fill_Rect(2*(xpos + (index*6)), ypos*2, 2*(xpos + (index*6)+6), 2*(ypos+6), BLACK);
					str[index] = ascii;
					str[index+1] = 0;

					int objindex;
					Play_Sample(keystrok, 255, Options.Normalize_Sound(255));
					objindex = Alloc_Object(new ScoreScaleClass(str+index,xpos+(index*6), ypos, pal));
					while (ScoreObjs[objindex]) Call_Back_Delay(1);

					if (index < (MAX_FAMENAME_LENGTH-2) ) index++;
				}
			}
		}
	} while(key!=KA_RETURN);							//	} while(key != KN_RETURN && key!=KN_KEYPAD_RETURN);
}


void Animate_Cursor(int pos, int ypos)
{
	static int _lastpos, _state;
	static CountDownTimerClass _timer;

	ypos += 7;	// move cursor to bottom of letter

	// If they moved the cursor, erase old one and force state=0, to make green draw right away
	if (pos != _lastpos) {
		PseudoSeenBuff->Draw_Line(HALLFAME_X + (_lastpos*6),ypos, HALLFAME_X + (_lastpos*6) + 5, ypos, TBLACK);
		TextPrintBuffer->Fill_Rect(2*(HALLFAME_X + (_lastpos*6)),2*ypos, 2*(HALLFAME_X + (_lastpos*6) + 5), 2*ypos+1, BLACK);
		_lastpos = pos;
		_state = 0;
	}

	PseudoSeenBuff->Draw_Line(HALLFAME_X + (pos*6),ypos, HALLFAME_X + (pos*6)+5, ypos, _state ? LTBLUE : TBLACK);
	TextPrintBuffer->Fill_Rect(2*(HALLFAME_X + (pos*6)),2*ypos, 2*(HALLFAME_X + (pos*6)+5), 2*ypos+1, _state ? LTBLUE : BLACK);

	/*
	** Toggle the color of the cursor, green or black, if it's time to do so.
	*/
	if (!_timer.Time()) {
		_state ^= 1;
		_timer.Set(5);
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

// Only draw the infantrymen if we're playing Nod... GDI wouldn't execute
//	people like that.

	/*
	** First restore the background
	*/
	SysMemPage.Blit(SysMemPage, BARGRAPH_X, CASUALTY_Y, 0, 0, 320-BARGRAPH_X, 34);
	Set_Logic_Page(SysMemPage);

	/*
	** Then draw all the infantrymen on the clean SysMemPage
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
					  SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL,//|SHAPE_GHOST,
					  InfantryMan[index].remap,
					  Map.UnitShadow);
	/*
	** see if it's time to run a new anim
	*/
	if (--InfantryMan[index].delay < 0) {
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
		InfantryMan[index].delay = Random() & 15;
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
 *          ckilled = # of civilians killed (adjusted to fit in space)     *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/13/1995 BWG : Created.                                             *
 *=========================================================================*/
void Draw_Bar_Graphs(int i, int gkilled, int nkilled, int ckilled)
{
	if (gkilled) {
		LogicPage->Fill_Rect(0,   0+4, 0+MIN(i,gkilled),   0+5, LTCYAN);
		LogicPage->Draw_Line(0+1, 0+6, 0+MIN(i,gkilled)+1, 0+6, TBLACK);
		LogicPage->Draw_Line(0+MIN(i,gkilled)+1,0+5, 0+MIN(i,gkilled)+1, 0+5, TBLACK);
		if (i <= gkilled) {
			int anim = InfantryMan[i/11].anim;
			if (anim!=-1 && anim < DO_GUN_DEATH) {
				if (i/11) {
					New_Infantry_Anim(i/11,DO_GUN_DEATH + (Random() & 3));
				} else {
					New_Infantry_Anim(i/11,DO_GUN_DEATH);
				}
//				Sound_Effect(Random_Pick(VOC_SCREAM1, VOC_SCREAM5));
			}
		}
	}
	if (nkilled) {
		LogicPage->Fill_Rect(0,   0+16, 0+MIN(i,nkilled),   0+17, RED);
		LogicPage->Draw_Line(0+1, 0+18, 0+MIN(i,nkilled)+1, 0+18, TBLACK);
		LogicPage->Draw_Line(0+MIN(i,nkilled)+1, 0+17, 0+MIN(i,nkilled)+1, 0+17, TBLACK);
		if (i <= nkilled) {
			int anim = InfantryMan[(NUMINFANTRYMEN/3)+(i/11)].anim;
			if (anim!=-1 && anim < DO_GUN_DEATH) {
				if (i/11) {
					New_Infantry_Anim((NUMINFANTRYMEN/3)+(i/11),DO_GUN_DEATH + (Random() & 3));
				} else {
					New_Infantry_Anim((NUMINFANTRYMEN/3)+(i/11),DO_GUN_DEATH);
				}
//				Sound_Effect(Random_Pick(VOC_SCREAM1, VOC_SCREAM5));
			}
		}
	}

	if (ckilled) {
		LogicPage->Fill_Rect(0,   0+28, 0+MIN(i,ckilled),   0+29, RED);
		LogicPage->Draw_Line(0+1, 0+30, 0+MIN(i,ckilled)+1, 0+30, TBLACK);
		LogicPage->Draw_Line(0+MIN(i,ckilled)+1, 0+29, 0+MIN(i,ckilled)+1, 0+29, TBLACK);
		if (i <= ckilled) {
			int anim = InfantryMan[((NUMINFANTRYMEN*2)/3)+(i/11)].anim;
			if (anim!=-1 && anim < DO_GUN_DEATH) {
				if (i/11) {
					New_Infantry_Anim(((NUMINFANTRYMEN*2)/3)+(i/11),DO_GUN_DEATH + (Random() & 3));
				} else {
					New_Infantry_Anim(((NUMINFANTRYMEN*2)/3)+(i/11),DO_GUN_DEATH);
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
	CountDownTimerClass cd;

	if (!ControlQ) {
		if (Keyboard::Down(KN_LCTRL) && Keyboard::Down(KN_Q)) {
			ControlQ = 1;
			Keyboard::Clear();
		}
	}
	if (ControlQ) time=0;

	cd.Set(time);
	StreamLowImpact = true;
	do {
		Call_Back();
		//Animate_Score_Objs();
		//Interpolate_2X_Scale(PseudoSeenBuff , &SeenBuff ,NULL);

		//if (Get_Mouse_State()){
		//	Interpolate_2X_Scale(PseudoSeenBuff , &SeenBuff ,NULL);
			//BlitList.Update();
		//}else{
			Animate_Score_Objs();
			Interpolate_2X_Scale(PseudoSeenBuff , &HidPage ,NULL);
			BlitList.Update();
			WWMouse->Draw_Mouse(&HidPage);
			Blit_Hid_Page_To_Seen_Buff();
			WWMouse->Erase_Mouse(&HidPage, TRUE);
		//}
	} while(cd.Time());
	StreamLowImpact = false;
}


void Animate_Score_Objs()
{
	StillUpdating = false;
	for (int i = 0; i < MAXSCOREOBJS; i++) {
		if (ScoreObjs[i]) {
			ScoreObjs[i]->Update();
		}
	}
}

char *Int_Print(int a)
{
	static char str[10];

	sprintf(str,"%d",a);
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
void Multi_Score_Presentation(void)
{
	static unsigned char const _cycleyellowpal[]={0x0,0xec,0xEb,0xea,0xE9,0xe9,0xE9,0x0,0xE9,0x0,0x0,0x0,0x0,0x0,0xED,0x0};

	static unsigned char const _greenpal[]= {0x0,0x12,0x14,0x16,0x18,0x18,0x18,0x0,0x18,0x0,0x0,0x0,0x0,0x0,0x10,0x0};
	static unsigned char const _redpal[]=   {0x0,0x22,0x24,0x26,0x28,0x28,0x28,0x0,0x28,0x0,0x0,0x0,0x0,0x0,0x20,0x0};
	static unsigned char const _graypal[]=  {0x0,0xca,0xCb,0xcc,0xCd,0xcd,0xCd,0x0,0xCD,0x0,0x0,0x0,0x0,0x0,0xC8,0x0};
	static unsigned char const _orangepal[]={0x0,0xd1,0xD2,0xd3,0xD4,0xd4,0xD4,0x0,0xD4,0x0,0x0,0x0,0x0,0x0,0xD0,0x0};
	static unsigned char const _bluepal[]=  {0x0,0x2,0x0a,0xb,0x0b,0xb,0x0B,0x0,0x0B,0x0,0x0,0x0,0x0,0x0,0x09,0x0};
	static unsigned char const _yellowpal[]={0x0,0x5,0xee,0xf1,0xf2,0xf2,0xF2,0xf2,0xF2,0x0,0x0,0x0,0x0,0x0,0x7D,0x0};

	//static char const _greenpal[]= {0x0,0x0,0x12,0x0,0x14,0x0,0x16,0x0,0x18,0x0,0x0,0x0,0x0,0x0,0x10,0x0};
	//static char const _redpal[]=   {0x0,0x0,0x22,0x0,0x24,0x0,0x26,0x0,0x28,0x0,0x0,0x0,0x0,0x0,0x20,0x0};
	//static char const _graypal[]=  {0x0,0x0,0xCA,0x0,0xCB,0x0,0xCC,0x0,0xCD,0x0,0x0,0x0,0x0,0x0,0xC8,0x0};
	//static char const _orangepal[]={0x0,0x0,0xD1,0x0,0xD2,0x0,0xD3,0x0,0xD4,0x0,0x0,0x0,0x0,0x0,0xD0,0x0};
	//static char const _bluepal[]=  {0x0,0x0,0x02,0x0,0x0A,0x0,0x0B,0x0,0x0B,0x0,0x0,0x0,0x0,0x0,0x09,0x0};
	//static char const _yellowpal[]={0x0,0x0,0x05,0x0,0xEE,0x0,0xF1,0x0,0xF2,0x0,0x0,0x0,0x0,0x0,0x7D,0x0};

	static unsigned char const *_colors[]={_yellowpal,_redpal,_bluepal,_orangepal,_greenpal,_graypal};

	int i,k;
	void *oldfont, *anim;
	int oldfontxspacing = FontXSpacing;
	char const *pal;


	FontXSpacing = 0;
	Map.Override_Mouse_Shape(MOUSE_NORMAL);
	Theme.Queue_Song(THEME_WIN1);

	PseudoSeenBuff = new GraphicBufferClass(320,200,(void*)NULL);
	TextPrintBuffer = new GraphicBufferClass(SeenBuff.Get_Width(), SeenBuff.Get_Height() ,(void*)NULL);
	BlitList.Clear();

   	SysMemPage.Clear();
	PseudoSeenBuff->Clear();
	HiddenPage.Clear();
	TextPrintBuffer->Clear();

	Set_Palette(BlackPalette);

	anim = Open_Animation("MLTIPLYR.WSA",NULL,0L,(WSAOpenType)(WSA_OPEN_FROM_MEM | WSA_OPEN_TO_PAGE),Palette);
	Hide_Mouse();

	/*
	** Display the background animation
	*/
	VisiblePage.Clear();
	InterpolationPaletteChanged = TRUE;
	InterpolationPalette = Palette;
	Increase_Palette_Luminance (Palette , 30,30,30,63);
	Animate_Frame(anim, *PseudoSeenBuff, 1);
	Interpolate_2X_Scale( PseudoSeenBuff , &SeenBuff , "MULTSCOR.PAL");
	Fade_Palette_To(Palette, FADE_PALETTE_FAST, Call_Back);

	int frame = 1;
	while (frame < Get_Animation_Frame_Count(anim)) {
		Animate_Frame(anim, *PseudoSeenBuff, frame++);
		Call_Back_Delay(2);
	}
	Close_Animation(anim);

	/* Change to the six-point font for Text_Print */
	oldfont = Set_Font(ScoreFontPtr);
	Call_Back();

	Set_Logic_Page(*PseudoSeenBuff);

	/*
	** Move all the scores over a notch if there's more games than can be
	** shown (which is known by MPlayerCurGame == MAX_MULTI_GAMES-1);
	*/
	if (MPlayerCurGame == MAX_MULTI_GAMES-1) {
		for(i = 0; i < MAX_MULTI_NAMES; i++) {
			for(k = 0; k < MAX_MULTI_GAMES-1; k++) {
				MPlayerScore[i].Kills[k] = MPlayerScore[i].Kills[k+1];
			}
		}
	}

	int y = 41;
	for(i = 0; i < MAX_MULTI_NAMES; i++) {
		if (strlen(MPlayerScore[i].Name)) {
			pal = (const char*)_colors[MPlayerScore[i].Color];

			Alloc_Object(new ScorePrintClass(MPlayerScore[i].Name, 15,  y,pal));
			Call_Back_Delay(20);

			Alloc_Object(new ScorePrintClass(Int_Print(MPlayerScore[i].Wins), 118, y, pal));
			Call_Back_Delay(6);

			for(k = 0; k <= MIN(MPlayerCurGame, MAX_MULTI_GAMES-2); k++) {
				if (MPlayerScore[i].Kills[k] >= 0) {
					Alloc_Object(new ScorePrintClass(Int_Print(MPlayerScore[i].Kills[k]), 225+(24*k), y, pal));
					Call_Back_Delay(6);
				}
			}
			y += 12;
		}
	}

#if (FRENCH)
	Alloc_Object(new ScorePrintClass(TXT_MAP_CLICK2, 90 /*(320-strlen(Text_String(TXT_MAP_CLICK2)))/2*/, 185, _cycleyellowpal));
#else
	Alloc_Object(new ScorePrintClass(TXT_MAP_CLICK2, 109 /*(320-strlen(Text_String(TXT_MAP_CLICK2)))/2*/, 185, _cycleyellowpal));
#endif
	Cycle_Wait_Click();

/* get rid of all the animating objects */
	for (i = 0; i < MAXSCOREOBJS; i++) if (ScoreObjs[i]) {
		delete ScoreObjs[i];
		ScoreObjs[i] = 0;
	}

	Theme.Queue_Song(THEME_NONE);

	Fade_Palette_To(BlackPalette, FADE_PALETTE_FAST, NULL);
	VisiblePage.Clear();
	Set_Palette(GamePalette);

	Set_Logic_Page (SeenBuff);

	delete PseudoSeenBuff;
	delete TextPrintBuffer;
	BlitList.Clear();

	Set_Font(oldfont);
	FontXSpacing = oldfontxspacing;
	ControlQ = 0;
	Show_Mouse();
}

#ifdef WRITE_LBM
	// A BitMapHeader is stored in a BMHD chunk.  This structure MUST be an even size
typedef struct {
    unsigned short w, h;							// raster width & height in pixels
    unsigned short x, y;							// position for this image
    unsigned char planes;							// # source bitplanes
    unsigned char masking;						// masking technique
    unsigned char compression;					// compression algoithm
    unsigned char pad1;							// UNUSED.  For consistency, put 0 here.
    unsigned short transcolor;					// transparent "color number"
    unsigned char xaspect, yaspect;			// aspect ratio, a rational number x/y
    unsigned short pagewidth, pageheight;		// source "page" size in pixels
} BitMapHeaderType;

// All values in LocalHeader are always the same except planes.  This is set in Write_BMHD
// the WORD values must be in low-high order for compatibility.

static BitMapHeaderType LocalHeader = {
	0x4001, 0xc800, 0, 0, 0, 0,		// width, height, x, y, planes, mask
	1,   0, 0xFF00, 5, 6, 				// compress, pad1, transcolor, xasptect, yaspect
	0x4001,	0xC800 };					// pagewidth, pageheight

#define BM_HEADER_SIZE		(((sizeof(BitMapHeaderType) + 1) & 0xFFFE) + 8L)

/*=========================================================================*/
/* The following static functions are in this file:                       */
/*=========================================================================*/

static long CCWrite_BMHD(CCFileClass &lbmhandle, short bitplanes);
static long CCWrite_CMAP(CCFileClass &lbmhandle, unsigned char * palette, short bitplanes);
static long CCWrite_BODY(CCFileClass &lbmhandle, BufferClass& buff, short bitplanes);
static long CCWrite_Row(CCFileClass &lbmhandle, unsigned char *buffer);


/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

/***************************************************************************
 * CCWrite_LBM_File -- Writes out a file in LBM format                     *
 *                                                                         *
 * INPUT:  short lbmhandle -- lbm file handle already opened by caller     *
 *         BufferClass buff -- buff where MCGA picture is                  *
 *         short bitplane -- number of bitplanes to convert to             *
 *         char *palette -- pointer to palette for buff                    *
 *                                                                         *
 * OUTPUT: Returns bool -- successfull or not                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/18/1991  SB : Created.                                             *
 *=========================================================================*/
bool CCWrite_LBM_File(CCFileClass &lbmhandle, BufferClass& buff, short bitplanes, unsigned char *palette)
{
	long filesize;


	lbmhandle.Seek(0L, SEEK_SET);						// goto beginning of file

	lbmhandle.Write("FORM????ILBM", 12L);			// First 12 bytes of all .lbm files
																		// size is unkown so write ????
	filesize = 12L; 												// 4 bytes for "ILBM"

	filesize += CCWrite_BMHD(lbmhandle, bitplanes);		// write out BMHD (fixed size)
	filesize += CCWrite_CMAP(lbmhandle, palette, bitplanes);	// write out CMAP

		// Write out the body, or compressed picture image.  This size will depend
		// on the compression, but the value passed back is what the compressor
		// assumed was written to file

	filesize += CCWrite_BODY(lbmhandle, buff, bitplanes);

		// Verify that we were able to write out the file without running out of space
	if (lbmhandle.Seek(0L, SEEK_END) != filesize) {
		return(false);
	}

	lbmhandle.Seek(4L, SEEK_SET);						// goto beginning of file
	filesize = Reverse_LONG(filesize - 8L);				// - 8 because of "FORM" + short (size)
	lbmhandle.Write( (char *) &filesize, 4L);		// patch in filesize

	return(true);
}


/***************************************************************************
 * CCWrite_BMHD -- writes out the bit map header (LocalHeader)             *
 *                                                                         *
 * INPUT:  short lbmhandle -- file handle for lbm file                     *
 *         short pitplanes -- number of bitplanes to write out             *
 *                                                                         *
 * OUTPUT: LONG number of bytes hopefully written out to .LBM file         *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/19/1991  SB : Created.                                             *
 *=========================================================================*/
static long CCWrite_BMHD(CCFileClass &lbmhandle, short bitplanes)
{
	long size;

	lbmhandle.Write("BMHD", 4L);					// write out chunk title
	size = Reverse_LONG(sizeof(LocalHeader));			// write out size of LocalHeader chunk
	lbmhandle.Write((char *) &size, 4L);

	LocalHeader.planes	= bitplanes;					// only nonconstant value in LocalHeader

		// Make sure size is even. Return 8 = "BMHD" + size of the bitmap header structure

	return(lbmhandle.Write((char *) &LocalHeader,
	                 (sizeof(LocalHeader) + 1) & 0xFFFE) + 8L);
}


/***************************************************************************
 * CCWrite_CMAP -- Writes out CMAP (palette) information                   *
 *                                                                         *
 *                                                                         *
 * INPUT:  short lbmhandle -- file handle of lbm file                      *
 *         char * palette -- pointer to paletter information               *
 *         short bitplanes -- used to figure out size of palette           *
 *                                                                         *
 * OUTPUT: long number of bytes that should have been written out to .LBM. *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/19/1991  SB : Created.                                             *
 *=========================================================================*/
static long CCWrite_CMAP(CCFileClass &lbmhandle, unsigned char * palette, short bitplanes)
{
	short color, r, g, b, colors;
	long size;
	unsigned char *pal_ptr;
	char rgb[3];


	lbmhandle.Write("CMAP", 4L);						// write out palette info
	colors = 1 << bitplanes;									// colors = 2 to the bitplanes
	size = Reverse_LONG(colors * 3L);						// size = colors * 3 guns

 	lbmhandle.Write((char *) &size, 4L);

	for (pal_ptr = palette, color = 0; color < colors; color++) { // for each color

		if ((r = *pal_ptr++) != 0) {			// DPaint changes allows 0 - 100 for gun values
			r = (r << 2) | 0x03;					// this must be converted to 0 - 256 for LBM
		}												// so LBM_val = (DP_val * 4) | 3 if DP_val != 0
		if ((g = *pal_ptr++) != 0) {
			g = (g << 2) | 0x03;
		}
		if ((b = *pal_ptr++) != 0) {
			b = (b << 2) | 0x03;
		}
		rgb[0] = r;									// assign gun values to an array to write out
		rgb[1] = g;
		rgb[2] = b;

		lbmhandle.Write(rgb, 3L);
	}
														// size = colors * 3
	return(((colors << 1) + colors) + 8L);	// total size of CMAP 8 = "CMAP" + short (size)
}


/***************************************************************************
 * CCWrite_Body -- writes out compressed data in an LBM file               *
 *                                                                         *
 * INPUT: short lbmhandle -- file handle of lbm file                       *
 *                                                                         *
 * OUTPUT: long - number of byte written                                   *
 *  	                                                                     *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/19/1991  SB : Created.                                             *
 *=========================================================================*/
static long CCWrite_BODY(CCFileClass &lbmhandle, BufferClass& buff, short bitplanes)
{
	long bodysize = 0;
	long actualsize;
	long size;
	short planebit;
	short line, plane;
	unsigned char buffer[40];
	unsigned char *buffptr;

	lbmhandle.Write("BODY????", 8L);		// BODY chunk ID, ???? reserved for chuncksize

	buffptr = (unsigned char *) buff.Get_Buffer();						// point to beginning of buff

	for (line = 0; line < 200; line++) {
		planebit = 1;											// start with bit 1 set

		for (plane = 0; plane < bitplanes; plane++) {
			Pack_2_Plane(buffer, buffptr, planebit);	// convert to planar
			bodysize += CCWrite_Row(lbmhandle, buffer); // write to to the BODY in the LBM

			planebit <<= 1;									// set next bit
		}

		buffptr += 320;										// row size is 320
	}

	actualsize = bodysize + (bodysize&0x01);

	if (actualsize != bodysize) {
		lbmhandle.Write(buffer, 1);		// Padd the block.
	}

	lbmhandle.Seek( -(actualsize + 4L), SEEK_CUR);		// Patch in chunksize
	size = Reverse_LONG(bodysize);
	lbmhandle.Write( (char *) &size ,4L);

	return(actualsize + 8L);		// total size of BODY,  "BODY????" = 8 bytes
}


/***************************************************************************
 * CCWrite_Row -- compresses and writes a row plane to .lbm file           *
 *                                                                         *
 * INPUT:  short lbmhandle -- lbm file handle                              *
 *         unsigned char *buffer -- pointer to buffer to be written out    *
 *                                                                         *
 * OUTPUT: long size of chunk that should have been written out            *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/19/1991  SB : Created.                                             *
 *=========================================================================*/
static long CCWrite_Row(CCFileClass &lbmhandle, unsigned char *buffer)
{
	short i;
	short chunksize = 0;
	short dataLength = 40;	  					// 320 rows / 8 ( 1 plane per row)
	unsigned char repCode, current, curr_plus_2;
	unsigned char *buffptr;

	while (dataLength) {

  		// If at least 2 more bytes and they are equal, then replicate

		if ((dataLength >= 2) && (buffer[0] == buffer[1])) {
			buffptr = buffer;
			for (i = 0; (i <= 128) &&  (i < (dataLength - 1)); i++) {
				if (*buffptr != buffptr[1]) {
					break;
				}
				buffptr++;
			}
			i++;
			repCode = -i + 1;
			lbmhandle.Write(&repCode, 1L);			// Write count as -count+1
			lbmhandle.Write(buffer,   1L);			// Write byte to replicate
			buffer += i;
			dataLength -= i;
			chunksize  += 2;

		} else { // Copy literally till 3 byte run or two 2 byte runs found

			for (i = 0; (i <= 128) && (i < dataLength); i++) {
				current     = buffer[i];
				curr_plus_2 = buffer[i + 2];

				if (i == dataLength - 1)
					continue;
				if (current != buffer[i + 1])
					continue;
				if (i == dataLength - 2)
					continue;
				if (current == curr_plus_2)
					break;
				if (i == dataLength - 3)
					continue;
				if (curr_plus_2 == buffer[i + 3])
					 break;
			}
			repCode = i - 1;
			lbmhandle.Write(&repCode, 1L);			// Write count as count-1
			lbmhandle.Write(buffer, (long) i);		// Write 'count' bytes
			buffer += i;
			dataLength -= i;
			chunksize += i + 1;
		}
	}  // end while

	return(chunksize);
}
#endif