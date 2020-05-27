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
 **     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : 32 bit library                           *
 *                                                                         *
 *                    File Name : MISC.H                                   *
 *                                                                         *
 *                   Programmer : Scott K. Bowen                           *
 *                                                                         *
 *                   Start Date : August 3, 1994                           *
 *                                                                         *
 *                  Last Update : August 3, 1994   [SKB]                   *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MISC_H
#define MISC_H

#define WIN32_LEAN_AND_MEAN	// eliminates unecessary definitions in windows.h
#ifndef _WIN32 // Denzil 6/2/98 Watcom 11.0 complains without this check
#define _WIN32
#endif // _WIN32
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

extern	LPDIRECTDRAWSURFACE	PaletteSurface;

/*========================= C++ Routines ==================================*/


/*=========================================================================*/
/* The following prototypes are for the file: DDRAW.CPP							*/
/*=========================================================================*/
void 		Process_DD_Result(HRESULT result, int display_ok_msg);
BOOL 		Set_Video_Mode(HWND hwnd, int w, int h, int bits_per_pixel);
void 		Reset_Video_Mode(void);
unsigned 	Get_Free_Video_Memory(void);
void 		Wait_Blit(void);
unsigned 	Get_Video_Hardware_Capabilities(void);

extern "C" void Wait_Vert_Blank(void);
extern "C" void Set_DD_Palette (void *palette);

/*
** Pointer to function to call if we detect a focus loss
*/
extern	void (*Misc_Focus_Loss_Function)(void);
/*
** Pointer to function to call if we detect a surface restore
*/
extern	void (*Misc_Focus_Restore_Function)(void);


/*
 *  Flags returned by Get_Video_Hardware_Capabilities
 */
/* Hardware blits supported? */
#define	VIDEO_BLITTER					1

/* Hardware blits asyncronous? */
#define	VIDEO_BLITTER_ASYNC  		2

/* Can palette changes be synced to vertical refresh? */
#define	VIDEO_SYNC_PALETTE			4

/* Is the video cards memory bank switched? */
#define	VIDEO_BANK_SWITCHED			8

/* Can the blitter do filled rectangles? */
#define	VIDEO_COLOR_FILL				16

/* Is there no hardware assistance avaailable at all? */
#define	VIDEO_NO_HARDWARE_ASSIST	32



/*
 * Definition of surface monitor class
 *
 * This class keeps track of all the graphic buffers we generate in video memory so they
 *  can be restored after a focus switch.
*/

#define	MAX_SURFACES	20

class SurfaceMonitorClass {

	public:

		SurfaceMonitorClass();

		void	Add_DD_Surface (LPDIRECTDRAWSURFACE);
		void	Remove_DD_Surface (LPDIRECTDRAWSURFACE);
		BOOL	Got_Surface_Already (LPDIRECTDRAWSURFACE);
		void	Restore_Surfaces (void);
		void	Set_Surface_Focus ( BOOL in_focus );
		void	Release(void);

		BOOL	SurfacesRestored;

	private:

		LPDIRECTDRAWSURFACE	Surface[MAX_SURFACES];
		BOOL						InFocus;

};

extern	SurfaceMonitorClass	AllSurfaces;				//List of all direct draw surfaces


/*=========================================================================*/
/* The following variables are declared in: DDRAW.CPP								*/
/*=========================================================================*/
extern	LPDIRECTDRAW	DirectDrawObject;
extern	LPDIRECTDRAW2	DirectDraw2Interface;
extern	HWND				MainWindow;
extern BOOL 				SystemToVideoBlits;
extern BOOL				VideoToSystemBlits;
extern BOOL				SystemToSystemBlits;
extern BOOL				OverlappedVideoBlits;	// Can video driver blit overlapped regions?

/*=========================================================================*/
/* The following prototypes are for the file: EXIT.CPP							*/
/* Prog_End Must be supplied by the user program in startup.cpp				*/
/*=========================================================================*/
VOID __cdecl Prog_End(const char *why = NULL, bool fatal = false);	// Added why and fatal parameters. ST - 6/27/2019 10:10PM		 
VOID __cdecl Exit(INT errorval, const BYTE *message, ...);

/*=========================================================================*/
/* The following prototypes are for the file: DELAY.CPP							*/
/*=========================================================================*/
void Delay(int duration);
void Vsync(void);


/*=========================================================================*/
/* The following prototypes are for the file: FINDARGV.CPP						*/
/*=========================================================================*/

BYTE  __cdecl Find_Argv(BYTE const *str);

/*=========================================================================*/
/* The following prototypes are for the file: LIB.CPP								*/
/*=========================================================================*/
char *Find_Argv(char const *str);
void Mono_Mem_Dump(void const *databuf, int bytes, int y);
void Convert_RGB_To_HSV(unsigned int r, unsigned int g, unsigned int b, unsigned int *h, unsigned int *s, unsigned int *v);
void Convert_HSV_To_RGB(unsigned int h, unsigned int s, unsigned int v, unsigned int *r, unsigned int *g, unsigned int *b);


/*=========================================================================*/
/* The following prototypes are for the file: VERSION.CPP						*/
/*=========================================================================*/

BYTE  __cdecl Version(VOID);


/*=========================================================================*/
/* The following prototypes are for the file: IRANDOM.CPP						*/
/*=========================================================================*/
int IRandom(int minval, int maxval);


/*========================= Assembly Routines ==============================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=========================================================================*/
/* The following prototypes are for the file: RANDOM.ASM							*/
/*=========================================================================*/

unsigned char __cdecl Random(void);
int __cdecl Get_Random_Mask(int maxval);

/*=========================================================================*/
/* The following prototype is for the file: SHAKESCR.ASM							*/
/*=========================================================================*/

void __cdecl Shake_Screen(int shakes);

/*=========================================================================*/
/* The following prototypes are for the file: REVERSE.ASM						*/
/*=========================================================================*/

long __cdecl Reverse_Long(long number);
short __cdecl Reverse_Short(short number);
long __cdecl Swap_Long(long number);
#if (0)
/*=========================================================================*/
/* The following prototype is for the file: FACING8.ASM							*/
/*=========================================================================*/

int __cdecl Desired_Facing8(int x1, int y1, int x2, int y2);

/*=========================================================================*/
/* The following prototype is for the file: FACING16.ASM							*/
/*=========================================================================*/

int __cdecl Desired_Facing16(int x1, int y1, int x2, int y2);

/*=========================================================================*/
/* The following prototype is for the file: FACINGFF.ASM							*/
/*=========================================================================*/

int __cdecl Desired_Facing256(int x1, int y1, int x2, int y2);

/*=========================================================================*/
/* The following prototype is for the file: FADING.ASM							*/
/*=========================================================================*/
#endif

void * __cdecl Build_Fading_Table(void const *palette, void const *dest, long int color, long int frac);
/*=========================================================================*/
/* The following prototype is for the file: CRC.ASM							*/
/*=========================================================================*/

long __cdecl Calculate_CRC(void *buffer, long length);

/*=========================================================================*/
/* The following prototypes are for the file: DETPROC.ASM						*/
/*=========================================================================*/

extern WORD __cdecl  Processor(void);
extern WORD __cdecl Operating_System(void);
extern unsigned long random ( unsigned long mod )  ;
//extern void  randomize ( void ) ;

extern int __cdecl Clip_Rect ( int * x , int * y , int * dw , int * dh ,
	       	   			 int width , int height ) ;
extern int __cdecl Confine_Rect ( int * x , int * y , int dw , int dh ,
	      	      			 int width , int height ) ;



/*=========================================================================*/
/* The following prototypes are for the file: OPSYS.ASM							*/
/*=========================================================================*/

extern WORD OperationgSystem;

#ifdef __cplusplus
}
#endif

/*=========================================================================*/

#endif // MISC_H
