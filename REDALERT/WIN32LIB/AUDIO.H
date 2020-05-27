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

/***************************************************************************
 **      C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S      **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Westwood 32 bit Library                  *
 *                                                                         *
 *                    File Name : AUDIO.H                                  *
 *                                                                         *
 *                   Programmer : Phil W. Gorrow                           *
 *                                                                         *
 *                   Start Date : March 10, 1995                           *
 *                                                                         *
 *                  Last Update : March 10, 1995   [PWG]                   *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "wwstd.h"

/*=========================================================================*/
/* AUD file header type																		*/
/*=========================================================================*/
#define	AUD_FLAG_STEREO	1
#define	AUD_FLAG_16BIT		2

// PWG 3-14-95: This structure used to have bit fields defined for Stereo
//   and Bits.  These were removed because watcom packs them into a 32 bit
//   flag entry even though they could have fit in a 8 bit entry.
//#pragma pack(1);
#pragma pack(push,1)
typedef struct {
	unsigned short int	Rate;				// Playback rate (hertz).
	long	Size;				// Size of data (bytes).
	long	UncompSize;				// Size of data (bytes).
	unsigned char Flags;	// Holds flags for info
								//  1: Is the sample stereo?
								//  2: Is the sample 16 bits?
	unsigned char Compression;	// What kind of compression for this sample?
} AUDHeaderType;
#pragma pack(pop)


/*=========================================================================*/
/*	There can be a different sound driver for sound effects, digitized		*/
/*	samples, and musical scores.  Each one must be of these specified			*/
/*	types.																						*/
/*=========================================================================*/
typedef enum {
	SAMPLE_NONE,		// No digitized sounds will be played.
	SAMPLE_SB,			// Sound Blaster digitized driver.
	SAMPLE_SBPRO,		// Sound Blaster Pro digitized driver.
	SAMPLE_PAS,			// Pro-Audio Spectrum digitized driver.
	SAMPLE_ADLIBG,		// Adlib-Gold digitized driver.
	SAMPLE_TANDY,		// Tandy 'compatible' driver.
	SAMPLE_PCSPKR,		// PC speaker digitized driver (The Audio Solution driver).
	SAMPLE_ADLIB,		// Adlib digitized driver (The Audio Solution driver).
	SAMPLE_TEMP=0x1000,
	SAMPLE_LAST
} Sample_Type;

typedef enum {
	SCORE_NONE,			// No scores will be played.
	SCORE_ALFX,			// Westwood's ALFX adlib compatable driver.
	SCORE_WWPCSPKR,	// Westwood's PC-speaker driver (obsolete).
	SCORE_WWTANDY,		// Westwood's PC-speaker driver with Tandy mod (obsolete).
	SCORE_PCSPKR,		// PC speaker XMIDI driver.
	SCORE_TANDY,		// Tandy XMIDI driver.
	SCORE_MT32,			// MT-32 / LAPC-1 Roland XMIDI driver.
	SCORE_CANVAS,		// Sound Canvas SC-55.
	SCORE_ADLIB,		// Adlib XMIDI driver.
	SCORE_ADLIBG,		// Adlib Gold XMIDI driver.
	SCORE_PASFM,		// Pro Audio Spectrum XMIDI driver.
	SCORE_SBFM,			// Sound Blaster XMIDI driver.
	SCORE_SBP1FM,		// Sound Blaster Pro (YM3812) XMIDI driver.
	SCORE_SBP2FM,		// Sound Blaster Pro (OPL3) XMIDI driver (Can't use with SFX_ALFX).
	SCORE_TEMP=0x1000,
	SCORE_LAST
} Score_Type;

typedef enum {
	SFX_NONE,			// No sound effects will be played.
	SFX_ALFX,			// Westwood's ALFX adlib compatable driver.
	SFX_WWPCSPKR,		// Westwood's PC-speaker driver.
	SFX_WWTANDY,		// Westwood's PC-speaker driver with Tandy mod.
	SFX_PCSPKR,			// PC speaker XMIDI driver.
	SFX_TANDY,			// Tandy XMIDI driver.
	SFX_MT32,			// MT-32 / LAPC-1 Roland XMIDI driver.
	SFX_CANVAS,			// Sound Canvas SC-55.
	SFX_ADLIB,			// Adlib XMIDI driver.
	SFX_ADLIBG,			// Adlib Gold XMIDI driver.
	SFX_PASFM,			// Pro Audio Spectrum XMIDI driver.
	SFX_SBFM,			// Sound Blaster XMIDI driver.
	SFX_SBP1FM,			// Sound Blaster Pro (YM3812) XMIDI driver.
	SFX_SBP2FM,			// Sound Blaster Pro (OPL3) XMIDI driver.
	SFX_TEMP=0x1000,
	SFX_LAST
} SFX_Type;



/*=========================================================================*/
/* The following prototypes are for the file: SOUNDIO.CPP						*/
/*=========================================================================*/
int File_Stream_Sample(char const *filename, BOOL real_time_start = FALSE);
int File_Stream_Sample_Vol(char const *filename, int volume, BOOL real_time_start = FALSE);
void __cdecl Sound_Callback(void);
void __cdecl far maintenance_callback(void);
void *Load_Sample(char const *filename);
long Load_Sample_Into_Buffer(char const *filename, void *buffer, long size);
long Sample_Read(int fh, void *buffer, long size);
void Free_Sample(void const *sample);
BOOL Audio_Init( HWND window , int bits_per_sample, BOOL stereo , int rate , int reverse_channels);
void Sound_End(void);
void Stop_Sample(int handle);
BOOL Sample_Status(int handle);
BOOL Is_Sample_Playing(void const * sample);
void Stop_Sample_Playing(void const * sample);
int Play_Sample(void const *sample, int priority=0xFF, int volume=0xFF, signed short panloc = 0x0);
int Play_Sample_Handle(void const *sample, int priority, int volume, signed short panloc, int id);
int Set_Sound_Vol(int volume);
int Set_Score_Vol(int volume);
void Fade_Sample(int handle, int ticks);
int Get_Free_Sample_Handle(int priority);
int Get_Digi_Handle(void);
long Sample_Length(void const *sample);
void Restore_Sound_Buffers (void);
BOOL Set_Primary_Buffer_Format(void);
BOOL Start_Primary_Sound_Buffer (BOOL forced);
void Stop_Primary_Sound_Buffer (void);

/*
** Function to call if we detect focus loss
*/
extern	void (*Audio_Focus_Loss_Function)(void);


extern int Misc;
extern SFX_Type SoundType;
extern Sample_Type SampleType;

extern CRITICAL_SECTION	GlobalAudioCriticalSection;

extern int StreamLowImpact;