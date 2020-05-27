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

/*
** 
** 
**  Misc asm functions from ww lib
**  ST - 12/19/2018 1:20PM
** 
** 
** 
** 
** 
** 
** 
** 
** 
** 
** 
*/

#include "gbuffer.h"
#include "MISC.H"
#include "WSA.H"

IconCacheClass::IconCacheClass (void)
{
	IsCached			=FALSE;
	SurfaceLost		=FALSE;
	DrawFrequency	=0;
	CacheSurface	=NULL;
	IconSource		=NULL;
}

IconCacheClass::~IconCacheClass (void)
{
}		  

IconCacheClass	CachedIcons[MAX_CACHED_ICONS];

extern "C"{
IconSetType		IconSetList[MAX_ICON_SETS];
short				IconCacheLookup[MAX_LOOKUP_ENTRIES];
}

int		CachedIconsDrawn=0;		//Counter of number of cache hits
int		UnCachedIconsDrawn=0;	//Counter of number of cache misses
BOOL	CacheMemoryExhausted;	//Flag set if we have run out of video RAM


void Invalidate_Cached_Icons (void) {}
void Restore_Cached_Icons (void) {}
void Register_Icon_Set (void *icon_data , BOOL pre_cache) {};

//
// Prototypes for assembly language procedures in STMPCACH.ASM
//
extern "C" void __cdecl Clear_Icon_Pointers (void) {};
extern "C" void __cdecl Cache_Copy_Icon (void const *icon_ptr ,void * , int) {};
extern "C" int __cdecl Is_Icon_Cached (void const *icon_data , int icon) {return -1;};
extern "C" int __cdecl Get_Icon_Index (void *icon_ptr) {return 0;};
extern "C" int __cdecl Get_Free_Index (void) {return 0;};
extern "C" BOOL __cdecl Cache_New_Icon (int icon_index, void *icon_ptr) {return -1;};
extern "C" int __cdecl Get_Free_Cache_Slot(void) {return -1;}

void IconCacheClass::Draw_It (LPDIRECTDRAWSURFACE dest_surface , int x_pixel, int y_pixel, int window_left , int window_top , int window_width , int window_height) {}



extern	int	CachedIconsDrawn;
extern	int	UnCachedIconsDrawn;


extern "C" void __cdecl Set_Font_Palette_Range(void const *palette, INT start_idx, INT end_idx)
{
}		  


/*
;***************************************************************************
;* VVC::DRAW_LINE -- Scales a virtual viewport to another virtual viewport *
;*                                                                         *
;* INPUT:	WORD sx_pixel 	- the starting x pixel position		   *
;*		WORD sy_pixel	- the starting y pixel position		   *
;*		WORD dx_pixel	- the destination x pixel position	   *
;*		WORD dy_pixel   - the destination y pixel position	   *
;*		WORD color      - the color of the line to draw		   *
;*                                                                         *
;* Bounds Checking: Compares sx_pixel, sy_pixel, dx_pixel and dy_pixel	   *
;*       with the graphic viewport it has been assigned to.		   *
;*                                                                         *
;* HISTORY:                                                                *
;*   06/16/1994 PWG : Created.                                             *
;*   08/30/1994 IML : Fixed clipping bug.				   *
;*=========================================================================*
	PROC	Buffer_Draw_Line C NEAR
	USES	eax,ebx,ecx,edx,esi,edi
*/

void __cdecl Buffer_Draw_Line(void *this_object, int sx, int sy, int dx, int dy, unsigned char color)
{
	unsigned int clip_min_x;
	unsigned int clip_max_x;
	unsigned int clip_min_y;
	unsigned int clip_max_y;
	unsigned int clip_var;
	unsigned int accum;
	unsigned int bpr;
	
	static int _one_time_init = 0;

	//clip_tbl	DD	nada,a_up,a_dwn,nada
	//		DD	a_lft,a_lft,a_dwn,nada
	//		DD	a_rgt,a_up,a_rgt,nada
	//		DD	nada,nada,nada,nada

	static void *_clip_table [4*4] = {0};

	unsigned int int_color = color;
	unsigned int x1_pixel = (unsigned int) sx;
	unsigned int y1_pixel = (unsigned int) sy;
	unsigned int x2_pixel = (unsigned int) dx;
	unsigned int y2_pixel = (unsigned int) dy;

	__asm {		  
		mov	eax,_one_time_init
		and	eax,eax
		jnz	init_done

		call	do_init

init_done:
		
		//;*==================================================================
		//;* Take care of find the clip minimum and maximums
		//;*==================================================================
		mov	ebx,[this_object]
		xor	eax,eax
		mov	[clip_min_x],eax
		mov	[clip_min_y],eax
		mov	eax,[ebx]GraphicViewPortClass.Width
		mov	[clip_max_x],eax
		add	eax,[ebx]GraphicViewPortClass.XAdd
		add	eax,[ebx]GraphicViewPortClass.Pitch
		mov	[bpr],eax
		mov	eax,[ebx]GraphicViewPortClass.Height
		mov	[clip_max_y],eax

		//;*==================================================================
		//;* Adjust max pixels as they are tested inclusively.
		//;*==================================================================
		dec	[clip_max_x]
		dec	[clip_max_y]

		//;*==================================================================
		//;* Set the registers with the data for drawing the line
		//;*==================================================================
		mov	eax,[x1_pixel]		//; eax = start x pixel position
		mov	ebx,[y1_pixel]		//; ebx = start y pixel position
		mov	ecx,[x2_pixel]		//; ecx = dest x pixel position
		mov	edx,[y2_pixel]		//; edx = dest y pixel position

		//;*==================================================================
		//;* This is the section that "pushes" the line into bounds.
		//;* I have marked the section with PORTABLE start and end to signify
		//;* how much of this routine is 100% portable between graphics modes.
		//;* It was just as easy to have variables as it would be for constants
		//;* so the global vars ClipMaxX,ClipMinY,ClipMaxX,ClipMinY are used
		//;* to clip the line (default is the screen)
		//;* PORTABLE start
		//;*==================================================================

		cmp	eax,[clip_min_x]
		jl	short clip_it
		cmp	eax,[clip_max_x]
		jg	short clip_it
		cmp	ebx,[clip_min_y]
		jl	short clip_it
		cmp	ebx,[clip_max_y]
		jg	short clip_it
		cmp	ecx,[clip_min_x]
		jl	short clip_it
		cmp	ecx,[clip_max_x]
		jg	short clip_it
		cmp	edx,[clip_min_y]
		jl	short clip_it
		cmp	edx,[clip_max_y]
		jle	short on_screen

		//;*==================================================================
		//;* Takes care off clipping the line.
		//;*==================================================================
	clip_it:
		call	set_bits
		xchg	eax,ecx
		xchg	ebx,edx
		mov	edi,esi
		call	set_bits
		mov	[clip_var],edi
		or	[clip_var],esi
		jz	short on_screen
		test	edi,esi
		jne	short off_screen
		shl	esi,2
		//call	[clip_tbl+esi]
		call	[_clip_table+esi]
		jc	clip_it
		xchg	eax,ecx
		xchg	ebx,edx
		shl	edi,2
		//call	[clip_tbl+edi]
		call	[_clip_table+edi]
		jmp	clip_it

	on_screen:
		jmp	draw_it

	off_screen:
		jmp	and_out

		//;*==================================================================
		//;* Jump table for clipping conditions
		//;*==================================================================
	//clip_tbl	DD	nada,a_up,a_dwn,nada
	//		DD	a_lft,a_lft,a_dwn,nada
	//		DD	a_rgt,a_up,a_rgt,nada
	//		DD	nada,nada,nada,nada

	nada:
		clc
		ret

	a_up:
		mov	esi,[clip_min_y]
		call	clip_vert
		stc
		ret

	a_dwn:
		mov	esi,[clip_max_y]
		neg	esi
		neg	ebx
		neg	edx
		call	clip_vert
		neg	ebx
		neg	edx
		stc
		ret

		//;*==================================================================
		//;* xa'=xa+[(miny-ya)(xb-xa)/(yb-ya)]
		//;*==================================================================
	clip_vert:
		push	edx
		push	eax
		mov	[clip_var],edx		//; clip_var = yb
		sub	[clip_var],ebx		//; clip_var = (yb-ya)
		neg	eax			//; eax=-xa
		add	eax,ecx			//; (ebx-xa)
		mov	edx,esi			//; edx=miny
		sub	edx,ebx			//; edx=(miny-ya)
		imul	edx
		idiv	[clip_var]
		pop	edx
		add	eax,edx
		pop	edx
		mov	ebx,esi
		ret

	a_lft:
		mov	esi,[clip_min_x]
		call	clip_horiz
		stc
		ret

	a_rgt:
		mov	esi,[clip_max_x]
		neg	eax
		neg	ecx
		neg	esi
		call	clip_horiz
		neg	eax
		neg	ecx
		stc
		ret

		//;*==================================================================
		//;* ya'=ya+[(minx-xa)(yb-ya)/(xb-xa)]
		//;*==================================================================
	clip_horiz:
		push	edx
		mov	[clip_var],ecx		//; clip_var = xb
		sub	[clip_var],eax		//; clip_var = (xb-xa)
		sub	edx,ebx			//; edx = (yb-ya)
		neg	eax			//; eax = -xa
		add	eax,esi			//; eax = (minx-xa)
		imul	edx			//; eax = (minx-xa)(yb-ya)
		idiv	[clip_var]		//; eax = (minx-xa)(yb-ya)/(xb-xa)
		add	ebx,eax			//; ebx = xa+[(minx-xa)(yb-ya)/(xb-xa)]
		pop	edx
		mov	eax,esi
		ret

		//;*==================================================================
		//;* Sets the condition bits
		//;*==================================================================
	set_bits:
		xor	esi,esi
		cmp	ebx,[clip_min_y]	//; if y >= top its not up
		jge	short a_not_up
		or	esi,1

	a_not_up:
		cmp	ebx,[clip_max_y]	//; if y <= bottom its not down
		jle	short a_not_down
		or	esi,2

	a_not_down:
		cmp	eax,[clip_min_x]   	//; if x >= left its not left
		jge	short a_not_left
		or	esi,4

	a_not_left:
		cmp	eax,[clip_max_x]	//; if x <= right its not right
		jle	short a_not_right
		or	esi,8

	a_not_right:
		ret

		//;*==================================================================
		//;* Draw the line to the screen.
		//;* PORTABLE end
		//;*==================================================================
	draw_it:
		sub	edx,ebx			//; see if line is being draw down
		jnz	short not_hline	//; if not then its not a hline
		jmp	short hline		//; do special case h line

	not_hline:
		jg	short down		//; if so there is no need to rev it
		neg	edx			//; negate for actual pixel length
		xchg	eax,ecx			//; swap x's to rev line draw
		sub	ebx,edx			//; get old edx

	down:
		push	edx
		push	eax
		mov	eax,[bpr]
		mul	ebx
		mov	ebx,eax
		mov	eax,[this_object]
		add	ebx,[eax]GraphicViewPortClass.Offset
		pop	eax
		pop	edx

		mov	esi,1			//; assume a right mover
		sub	ecx,eax			//; see if line is right
		jnz	short not_vline	//; see if its a vertical line
		jmp	vline

	not_vline:
		jg	short right		//; if so, the difference = length

	//left:
		neg	ecx			//; else negate for actual pixel length
		neg	esi			//; negate counter to move left

	right:
		cmp	ecx,edx			//; is it a horiz or vert line
		jge	short horiz		//; if ecx > edx then |x|>|y| or horiz

	//vert:
		xchg	ecx,edx			//; make ecx greater and edx lesser
		mov	edi,ecx			//; set greater
		mov	[accum],ecx		//; set accumulator to 1/2 greater
		shr	[accum],1

		//;*==================================================================
		//;* at this point ...
		//;* eax=xpos ; ebx=page line offset; ecx=counter; edx=lesser; edi=greater;
		//;* esi=adder; accum=accumulator
		//;* in a vertical loop the adder is conditional and the inc constant
		//;*==================================================================
	//vert_loop:
		add	ebx,eax
		mov	eax,[int_color]

	v_midloop:
		mov	[ebx],al
		dec	ecx
		jl	and_out
		add	ebx,[bpr]
		sub	[accum],edx		//; sub the lesser
		jge	v_midloop		//; any line could be new
		add	[accum],edi		//; add greater for new accum
		add	ebx,esi			//; next pixel over
		jmp	v_midloop

	horiz:
		mov	edi,ecx			//; set greater
		mov	[accum],ecx		//; set accumulator to 1/2 greater
		shr	[accum],1

		//;*==================================================================
		//;* at this point ...
		//;* eax=xpos ; ebx=page line offset; ecx=counter; edx=lesser; edi=greater;
		//;* esi=adder; accum=accumulator
		//;* in a vertical loop the adder is conditional and the inc constant
		//;*==================================================================
	//horiz_loop:
		add	ebx,eax
		mov	eax,[int_color]

	h_midloop:
		mov	[ebx],al
		dec	ecx				//; dec counter
		jl	and_out				//; end of line
		add	ebx,esi
		sub     [accum],edx			//; sub the lesser
		jge	h_midloop
		add	[accum],edi			//; add greater for new accum
		add	ebx,[bpr]			//; goto next line
		jmp	h_midloop

		//;*==================================================================
		//;* Special case routine for horizontal line draws
		//;*==================================================================
	hline:
		cmp	eax,ecx			//; make eax < ecx
		jl	short hl_ac
		xchg	eax,ecx

	hl_ac:
		sub	ecx,eax			//; get len
		inc	ecx

		push	edx
		push	eax
		mov	eax,[bpr]
		mul	ebx
		mov	ebx,eax
		mov	eax,[this_object]
		add	ebx,[eax]GraphicViewPortClass.Offset
		pop	eax
		pop	edx
		add	ebx,eax
		mov	edi,ebx
		cmp	ecx,15
		jg	big_line
		mov	al,[color]
		rep	stosb			//; write as many words as possible
		jmp	short and_out		//; get outt


	big_line:
		mov	al,[color]
		mov	ah,al
		mov     ebx,eax
		shl	eax,16
		mov	ax,bx
		test	edi,3
		jz	aligned
		mov	[edi],al
		inc	edi
		dec	ecx
		test	edi,3
		jz	aligned
		mov	[edi],al
		inc	edi
		dec	ecx
		test	edi,3
		jz	aligned
		mov	[edi],al
		inc	edi
		dec	ecx

	aligned:
		mov	ebx,ecx
		shr	ecx,2
		rep	stosd
		mov	ecx,ebx
		and	ecx,3
		rep	stosb
		jmp	and_out


		//;*==================================================================
		//;* a special case routine for vertical line draws
		//;*==================================================================
	vline:
		mov	ecx,edx			//; get length of line to draw
		inc	ecx
		add	ebx,eax
		mov	eax,[int_color]

	vl_loop:
		mov	[ebx],al		//; store bit
		add	ebx,[bpr]
		dec	ecx
		jnz	vl_loop
		jmp	and_out


do_init:
		mov	edi, offset _clip_table
		
		lea	esi, nada
		mov	[edi], esi
		mov	[edi+12], esi
		lea	esi, a_up
		mov	[edi+4], esi
		lea	esi, a_dwn
		mov	[edi+8], esi

		add	edi, 16
		
		lea	esi, a_lft
		mov	[edi], esi
		mov	[edi+4], esi
		lea	esi, a_dwn
		mov	[edi+8], esi
		lea	esi, nada
		mov	[edi+12], esi

		add	edi, 16

		lea	esi, a_rgt
		mov	[edi], esi
		mov	[edi+8], esi
		lea	esi, a_up
		mov	[edi+4], esi
		lea	esi, nada
		mov	[edi+12], esi

		add	edi, 16

		lea	esi, nada
		mov	[edi], esi
		mov	[edi+4], esi
		mov	[edi+8], esi
		mov	[edi+12], esi
		
		mov	[_one_time_init], 1
		ret

	and_out:
	}
}





/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood 32 bit Library                  *
;*                                                                         *
;*                    File Name : DRAWLINE.ASM                             *
;*                                                                         *
;*                   Programmer : Phil W. Gorrow                           *
;*                                                                         *
;*                   Start Date : June 16, 1994                            *
;*                                                                         *
;*                  Last Update : August 30, 1994   [IML]                  *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   VVC::Scale -- Scales a virtual viewport to another virtual viewport   *
;*   Normal_Draw -- jump loc for drawing  scaled line of normal pixel      *
;*   __DRAW_LINE -- Assembly routine to draw a line                        *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

INCLUDE ".\drawbuff.inc"
INCLUDE ".\gbuffer.inc"


CODESEG
*/


/*
;***************************************************************************
;* VVC::DRAW_LINE -- Scales a virtual viewport to another virtual viewport *
;*                                                                         *
;* INPUT:	WORD sx_pixel 	- the starting x pixel position		   *
;*		WORD sy_pixel	- the starting y pixel position		   *
;*		WORD dx_pixel	- the destination x pixel position	   *
;*		WORD dy_pixel   - the destination y pixel position	   *
;*		WORD color      - the color of the line to draw		   *
;*                                                                         *
;* Bounds Checking: Compares sx_pixel, sy_pixel, dx_pixel and dy_pixel	   *
;*       with the graphic viewport it has been assigned to.		   *
;*                                                                         *
;* HISTORY:                                                                *
;*   06/16/1994 PWG : Created.                                             *
;*   08/30/1994 IML : Fixed clipping bug.				   *
;*=========================================================================*
	PROC	Buffer_Draw_Line C NEAR
	USES	eax,ebx,ecx,edx,esi,edi

	;*==================================================================
	;* Define the arguements that the function takes.
	;*==================================================================
	ARG	this_object:DWORD	; associated graphic view port
	ARG	x1_pixel:DWORD		; the start x pixel position
	ARG	y1_pixel:DWORD		; the start y pixel position
	ARG	x2_pixel:DWORD		; the dest x pixel position
	ARG	y2_pixel:DWORD		; the dest y pixel position
	ARG	color:DWORD		; the color we are drawing

	;*==================================================================
	;* Define the local variables that we will use on the stack
	;*==================================================================
	LOCAL	clip_min_x:DWORD
	LOCAL	clip_max_x:DWORD
	LOCAL	clip_min_y:DWORD
	LOCAL	clip_max_y:DWORD
	LOCAL	clip_var:DWORD
	LOCAL	accum:DWORD
	LOCAL	bpr:DWORD

	;*==================================================================
	;* Take care of find the clip minimum and maximums
	;*==================================================================
	mov	ebx,[this_object]
	xor	eax,eax
	mov	[clip_min_x],eax
	mov	[clip_min_y],eax
	mov	eax,[(GraphicViewPort ebx).GVPWidth]
	mov	[clip_max_x],eax
	add	eax,[(GraphicViewPort ebx).GVPXAdd]
	add	eax,[(GraphicViewPort ebx).GVPPitch]
	mov	[bpr],eax
	mov	eax,[(GraphicViewPort ebx).GVPHeight]
	mov	[clip_max_y],eax

	;*==================================================================
	;* Adjust max pixels as they are tested inclusively.
	;*==================================================================
	dec	[clip_max_x]
	dec	[clip_max_y]

	;*==================================================================
	;* Set the registers with the data for drawing the line
	;*==================================================================
	mov	eax,[x1_pixel]		; eax = start x pixel position
	mov	ebx,[y1_pixel]		; ebx = start y pixel position
	mov	ecx,[x2_pixel]		; ecx = dest x pixel position
	mov	edx,[y2_pixel]		; edx = dest y pixel position

	;*==================================================================
	;* This is the section that "pushes" the line into bounds.
	;* I have marked the section with PORTABLE start and end to signify
	;* how much of this routine is 100% portable between graphics modes.
	;* It was just as easy to have variables as it would be for constants
	;* so the global vars ClipMaxX,ClipMinY,ClipMaxX,ClipMinY are used
	;* to clip the line (default is the screen)
	;* PORTABLE start
	;*==================================================================

	cmp	eax,[clip_min_x]
	jl	short ??clip_it
	cmp	eax,[clip_max_x]
	jg	short ??clip_it
	cmp	ebx,[clip_min_y]
	jl	short ??clip_it
	cmp	ebx,[clip_max_y]
	jg	short ??clip_it
	cmp	ecx,[clip_min_x]
	jl	short ??clip_it
	cmp	ecx,[clip_max_x]
	jg	short ??clip_it
	cmp	edx,[clip_min_y]
	jl	short ??clip_it
	cmp	edx,[clip_max_y]
	jle	short ??on_screen

	;*==================================================================
	;* Takes care off clipping the line.
	;*==================================================================
??clip_it:
	call	NEAR PTR ??set_bits
	xchg	eax,ecx
	xchg	ebx,edx
	mov	edi,esi
	call	NEAR PTR ??set_bits
	mov	[clip_var],edi
	or	[clip_var],esi
	jz	short ??on_screen
	test	edi,esi
	jne	short ??off_screen
	shl	esi,2
	call	[DWORD PTR cs:??clip_tbl+esi]
	jc	??clip_it
	xchg	eax,ecx
	xchg	ebx,edx
	shl	edi,2
	call	[DWORD PTR cs:??clip_tbl+edi]
	jmp	??clip_it

??on_screen:
	jmp	??draw_it

??off_screen:
	jmp	??out

	;*==================================================================
	;* Jump table for clipping conditions
	;*==================================================================
??clip_tbl	DD	??nada,??a_up,??a_dwn,??nada
		DD	??a_lft,??a_lft,??a_dwn,??nada
		DD	??a_rgt,??a_up,??a_rgt,??nada
		DD	??nada,??nada,??nada,??nada

??nada:
	clc
	retn

??a_up:
	mov	esi,[clip_min_y]
	call	NEAR PTR ??clip_vert
	stc
	retn

??a_dwn:
	mov	esi,[clip_max_y]
	neg	esi
	neg	ebx
	neg	edx
	call	NEAR PTR ??clip_vert
	neg	ebx
	neg	edx
	stc
	retn

	;*==================================================================
	;* xa'=xa+[(miny-ya)(xb-xa)/(yb-ya)]
	;*==================================================================
??clip_vert:
	push	edx
	push	eax
	mov	[clip_var],edx		; clip_var = yb
	sub	[clip_var],ebx		; clip_var = (yb-ya)
	neg	eax			; eax=-xa
	add	eax,ecx			; (ebx-xa)
	mov	edx,esi			; edx=miny
	sub	edx,ebx			; edx=(miny-ya)
	imul	edx
	idiv	[clip_var]
	pop	edx
	add	eax,edx
	pop	edx
	mov	ebx,esi
	retn

??a_lft:
	mov	esi,[clip_min_x]
	call	NEAR PTR ??clip_horiz
	stc
	retn

??a_rgt:
	mov	esi,[clip_max_x]
	neg	eax
	neg	ecx
	neg	esi
	call	NEAR PTR ??clip_horiz
	neg	eax
	neg	ecx
	stc
	retn

	;*==================================================================
	;* ya'=ya+[(minx-xa)(yb-ya)/(xb-xa)]
	;*==================================================================
??clip_horiz:
	push	edx
	mov	[clip_var],ecx		; clip_var = xb
	sub	[clip_var],eax		; clip_var = (xb-xa)
	sub	edx,ebx			; edx = (yb-ya)
	neg	eax			; eax = -xa
	add	eax,esi			; eax = (minx-xa)
	imul	edx			; eax = (minx-xa)(yb-ya)
	idiv	[clip_var]		; eax = (minx-xa)(yb-ya)/(xb-xa)
	add	ebx,eax			; ebx = xa+[(minx-xa)(yb-ya)/(xb-xa)]
	pop	edx
	mov	eax,esi
	retn

	;*==================================================================
	;* Sets the condition bits
	;*==================================================================
??set_bits:
	xor	esi,esi
	cmp	ebx,[clip_min_y]	; if y >= top its not up
	jge	short ??a_not_up
	or	esi,1

??a_not_up:
	cmp	ebx,[clip_max_y]	; if y <= bottom its not down
	jle	short ??a_not_down
	or	esi,2

??a_not_down:
	cmp	eax,[clip_min_x]   	; if x >= left its not left
	jge	short ??a_not_left
	or	esi,4

??a_not_left:
	cmp	eax,[clip_max_x]	; if x <= right its not right
	jle	short ??a_not_right
	or	esi,8

??a_not_right:
	retn

	;*==================================================================
	;* Draw the line to the screen.
	;* PORTABLE end
	;*==================================================================
??draw_it:
	sub	edx,ebx			; see if line is being draw down
	jnz	short ??not_hline	; if not then its not a hline
	jmp	short ??hline		; do special case h line

??not_hline:
	jg	short ??down		; if so there is no need to rev it
	neg	edx			; negate for actual pixel length
	xchg	eax,ecx			; swap x's to rev line draw
	sub	ebx,edx			; get old edx

??down:
	push	edx
	push	eax
	mov	eax,[bpr]
	mul	ebx
	mov	ebx,eax
	mov	eax,[this_object]
	add	ebx,[(GraphicViewPort eax).GVPOffset]
	pop	eax
	pop	edx

	mov	esi,1			; assume a right mover
	sub	ecx,eax			; see if line is right
	jnz	short ??not_vline	; see if its a vertical line
	jmp	??vline

??not_vline:
	jg	short ??right		; if so, the difference = length

??left:
	neg	ecx			; else negate for actual pixel length
	neg	esi			; negate counter to move left

??right:
	cmp	ecx,edx			; is it a horiz or vert line
	jge	short ??horiz		; if ecx > edx then |x|>|y| or horiz

??vert:
	xchg	ecx,edx			; make ecx greater and edx lesser
	mov	edi,ecx			; set greater
	mov	[accum],ecx		; set accumulator to 1/2 greater
	shr	[accum],1

	;*==================================================================
	;* at this point ...
	;* eax=xpos ; ebx=page line offset; ecx=counter; edx=lesser; edi=greater;
	;* esi=adder; accum=accumulator
	;* in a vertical loop the adder is conditional and the inc constant
	;*==================================================================
??vert_loop:
	add	ebx,eax
	mov	eax,[color]

??v_midloop:
	mov	[ebx],al
	dec	ecx
	jl	??out
	add	ebx,[bpr]
	sub	[accum],edx		; sub the lesser
	jge	??v_midloop		; any line could be new
	add	[accum],edi		; add greater for new accum
	add	ebx,esi			; next pixel over
	jmp	??v_midloop

??horiz:
	mov	edi,ecx			; set greater
	mov	[accum],ecx		; set accumulator to 1/2 greater
	shr	[accum],1

	;*==================================================================
	;* at this point ...
	;* eax=xpos ; ebx=page line offset; ecx=counter; edx=lesser; edi=greater;
	;* esi=adder; accum=accumulator
	;* in a vertical loop the adder is conditional and the inc constant
	;*==================================================================
??horiz_loop:
	add	ebx,eax
	mov	eax,[color]

??h_midloop:
	mov	[ebx],al
	dec	ecx				; dec counter
	jl	??out				; end of line
	add	ebx,esi
	sub     [accum],edx			; sub the lesser
	jge	??h_midloop
	add	[accum],edi			; add greater for new accum
	add	ebx,[bpr]			; goto next line
	jmp	??h_midloop

	;*==================================================================
	;* Special case routine for horizontal line draws
	;*==================================================================
??hline:
	cmp	eax,ecx			; make eax < ecx
	jl	short ??hl_ac
	xchg	eax,ecx

??hl_ac:
	sub	ecx,eax			; get len
	inc	ecx

	push	edx
	push	eax
	mov	eax,[bpr]
	mul	ebx
	mov	ebx,eax
	mov	eax,[this_object]
	add	ebx,[(GraphicViewPort eax).GVPOffset]
	pop	eax
	pop	edx
	add	ebx,eax
	mov	edi,ebx
	cmp	ecx,15
	jg	??big_line
	mov	al,[byte color]
	rep	stosb			; write as many words as possible
	jmp	short ??out		; get outt


??big_line:
	mov	al,[byte color]
	mov	ah,al
	mov     ebx,eax
	shl	eax,16
	mov	ax,bx
	test	edi,3
	jz	??aligned
	mov	[edi],al
	inc	edi
	dec	ecx
	test	edi,3
	jz	??aligned
	mov	[edi],al
	inc	edi
	dec	ecx
	test	edi,3
	jz	??aligned
	mov	[edi],al
	inc	edi
	dec	ecx

??aligned:
	mov	ebx,ecx
	shr	ecx,2
	rep	stosd
	mov	ecx,ebx
	and	ecx,3
	rep	stosb
	jmp	??out


	;*==================================================================
	;* a special case routine for vertical line draws
	;*==================================================================
??vline:
	mov	ecx,edx			; get length of line to draw
	inc	ecx
	add	ebx,eax
	mov	eax,[color]

??vl_loop:
	mov	[ebx],al		; store bit
	add	ebx,[bpr]
	dec	ecx
	jnz	??vl_loop

??out:
	ret
	ENDP	Buffer_Draw_Line


*/















/*

;***************************************************************************
;* GVPC::FILL_RECT -- Fills a rectangular region of a graphic view port	   *
;*                                                                         *
;* INPUT:	WORD the left hand x pixel position of region		   *
;*		WORD the upper x pixel position of region		   *
;*		WORD the right hand x pixel position of region		   *
;*		WORD the lower x pixel position of region		   *
;*		UBYTE the color (optional) to clear the view port to	   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* NOTE:	This function is optimized to handle viewport with no XAdd *
;*		value.  It also handles DWORD aligning the destination	   *
;*		when speed can be gained by doing it.			   *
;* HISTORY:                                                                *
;*   06/07/1994 PWG : Created.                                             *
;*=========================================================================*
*/ 

/*
;******************************************************************************
; Much testing was done to determine that only when there are 14 or more bytes
; being copied does it speed the time it takes to do copies in this algorithm.
; For this reason and because 1 and 2 byte copies crash, is the special case
; used.  SKB 4/21/94.  Tested on 486 66mhz.  Copied by PWG 6/7/04.
*/ 
#define OPTIMAL_BYTE_COPY	14


void __cdecl Buffer_Fill_Rect(void *thisptr, int sx, int sy, int dx, int dy, unsigned char color)
{
/*
	;*===================================================================
	;* define the arguements that our function takes.
	;*===================================================================
	ARG    	this_object:DWORD			; this is a member function
	ARG	x1_pixel:WORD
	ARG	y1_pixel:WORD
	ARG	x2_pixel:WORD
	ARG	y2_pixel:WORD
	ARG    	color:BYTE			; what color should we clear to
*/
	
	void *this_object = thisptr;
	int x1_pixel = sx;
	int y1_pixel = sy;
	int x2_pixel = dx;
	int y2_pixel = dy;
	
/*
	;*===================================================================
	; Define some locals so that we can handle things quickly
	;*===================================================================
	LOCAL	VPwidth:DWORD		; the width of the viewport
	LOCAL	VPheight:DWORD		; the height of the viewport
	LOCAL	VPxadd:DWORD		; the additional x offset of viewport
	LOCAL	VPbpr:DWORD		; the number of bytes per row of viewport
*/

	int VPwidth;
	int VPheight;
	int VPxadd;
	int VPbpr;

	int local_ebp;	                      // Can't use ebp

	__asm {

		;*===================================================================
		;* save off the viewport characteristics on the stack
		;*===================================================================
		mov	ebx,[this_object]				; get a pointer to viewport
		mov	eax,[ebx]GraphicViewPortClass.Width		; get width from viewport
		mov	ecx,[ebx]GraphicViewPortClass.Height	; get height from viewport
		mov	edx,[ebx]GraphicViewPortClass.XAdd		; get xadd from viewport
		add	edx,[ebx]GraphicViewPortClass.Pitch		; extra pitch of direct draw surface
		mov	[VPwidth],eax				; store the width of locally
		mov	[VPheight],ecx
		mov	[VPxadd],edx
		add	eax,edx
		mov	[VPbpr],eax

		;*===================================================================
		;* move the important parameters into local registers
		;*===================================================================
		mov	eax,[x1_pixel]
		mov	ebx,[y1_pixel]
		mov	ecx,[x2_pixel]
		mov	edx,[y2_pixel]

		;*===================================================================
		;* Convert the x2 and y2 pixel to a width and height
		;*===================================================================
		cmp	eax,ecx
		jl	no_swap_x
		xchg	eax,ecx

	no_swap_x:
		sub	ecx,eax
		cmp	ebx,edx
		jl	no_swap_y
		xchg	ebx,edx
	no_swap_y:
		sub	edx,ebx
		inc	ecx
		inc	edx

		;*===================================================================
		;* Bounds check source X.
		;*===================================================================
		cmp	eax, [VPwidth]			; compare with the max
		jge	done				; starts off screen, then later
		jb	short sx_done			; if it's not negative, it's ok

		;------ Clip source X to left edge of screen.
		add	ecx, eax			; Reduce width (add in negative src X).
		xor	eax, eax			; Clip to left of screen.
	sx_done:

		;*===================================================================
		;* Bounds check source Y.
		;*===================================================================
		cmp	ebx, [VPheight]			; compare with the max
		jge	done				; starts off screen, then later
		jb	short sy_done			; if it's not negative, it's ok

		;------ Clip source Y to top edge of screen.
		add	edx, ebx			; Reduce height (add in negative src Y).
		xor	ebx, ebx			; Clip to top of screen.

	sy_done:
		;*===================================================================
		;* Bounds check width versus width of source and dest view ports
		;*===================================================================
		push	ebx				; save off ebx for later use
		mov	ebx,[VPwidth]			; get the source width
		sub	ebx, eax			; Maximum allowed pixel width (given coordinates).
		sub	ebx, ecx			; Pixel width undershoot.
		jns	short width_ok		; if not signed no adjustment necessary
		add	ecx, ebx			; Reduce width to screen limits.

	width_ok:
		pop	ebx				; restore ebx to old value

		;*===================================================================
		;* Bounds check height versus height of source view port
		;*===================================================================
		push	eax				; save of eax for later use
		mov	eax, [VPheight]			; get the source height
		sub	eax, ebx			; Maximum allowed pixel height (given coordinates).
		sub	eax, edx			; Pixel height undershoot.
		jns	short height_ok		; if not signed no adjustment necessary
		add	edx, eax			; Reduce height to screen limits.
	height_ok:
		pop	eax				; restore eax to old value

		;*===================================================================
		;* Perform the last minute checks on the width and height
		;*===================================================================
		or	ecx,ecx
		jz	done

		or	edx,edx
		jz	done

		cmp	ecx,[VPwidth]
		ja	done
		cmp	edx,[VPheight]
		ja	done

		;*===================================================================
		;* Get the offset into the virtual viewport.
		;*===================================================================
		xchg	edi,eax			; save off the contents of eax
		xchg	esi,edx			;   and edx for size test
		mov	eax,ebx			; move the y pixel into eax
		mul	[VPbpr]			; multiply by bytes per row
		add	edi,eax			; add the result into the x position
		mov	ebx,[this_object]
		add	edi,[ebx]GraphicViewPortClass.Offset

		mov	edx,esi			; restore edx back to real value
		mov	eax,ecx			; store total width in ecx
		sub	eax,[VPwidth]		; modify xadd value to include clipped
		sub	[VPxadd],eax		;   width bytes (subtract a negative number)

		;*===================================================================
		; Convert the color byte to a DWORD for fast storing
		;*===================================================================
		mov	al,[color]				; get color to clear to
		mov	ah,al					; extend across WORD
		mov	ebx,eax					; extend across DWORD in
		shl	eax,16					;   several steps
		mov	ax,bx

		;*===================================================================
		; If there is no row offset then adjust the width to be the size of
		;   the entire viewport and adjust the height to be 1
		;*===================================================================
		mov	esi,[VPxadd]
		or	esi,esi					; set the flags for esi
		jnz	row_by_row_aligned			;   and act on them

		xchg	eax,ecx					; switch bit pattern and width
		mul	edx					; multiply by edx to get size
		xchg	eax,ecx					; switch size and bit pattern
		mov	edx,1					; only 1 line off view port size to do

		;*===================================================================
		; Find out if we should bother to align the row.
		;*===================================================================
	row_by_row_aligned:
		mov	[local_ebp],ecx					; width saved in ebp
		cmp	ecx,OPTIMAL_BYTE_COPY			; is it worth aligning them?
		jl	row_by_row				;   if not then skip

		;*===================================================================
		; Figure out the alignment offset if there is any
		;*===================================================================
		mov	ebx,edi					; get output position
		and	ebx,3					;   is there a remainder?
		jz	aligned_loop				;   if not we are aligned
		xor	ebx,3					; find number of align bytes
		inc	ebx					; this number is off by one
		sub	[local_ebp],ebx					; subtract from width

		;*===================================================================
		; Now that we have the alignment offset copy each row
		;*===================================================================
	aligned_loop:
		mov	ecx,ebx					; get number of bytes to align
		rep	stosb					;   and move them over
		mov	ecx,[local_ebp]					; get number of aligned bytes
		shr	ecx,2					;   convert to DWORDS
		rep	stosd					;   and move them over
		mov	ecx,[local_ebp]					; get number of aligned bytes
		and	ecx,3					;   find the remainder
		rep	stosb					;   and move it over
		add	edi,esi					; fix the line offset
		dec	edx					; decrement the height
		jnz	aligned_loop				; if more to do than do it
		jmp	done					; we are all done

		;*===================================================================
		; If not enough bytes to bother aligning copy each line across a byte
		;    at a time.
		;*===================================================================
	row_by_row:
		mov	ecx,[local_ebp]					; get total width in bytes
		rep	stosb					; store the width
		add	edi,esi					; handle the xadd
		dec	edx					; decrement the height
		jnz	row_by_row				; if any left then next line
	done:
	}
}




/*
;***************************************************************************
;* VVPC::CLEAR -- Clears a virtual viewport instance                       *
;*                                                                         *
;* INPUT:	UBYTE the color (optional) to clear the view port to	   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* NOTE:	This function is optimized to handle viewport with no XAdd *
;*		value.  It also handles DWORD aligning the destination	   *
;*		when speed can be gained by doing it.			   *
;* HISTORY:                                                                *
;*   06/07/1994 PWG : Created.                                             *
;*   08/23/1994 SKB : Clear the direction flag to always go forward.       *
;*=========================================================================*
*/
void	__cdecl Buffer_Clear(void *this_object, unsigned char color)
{
	unsigned int local_color = color;

	__asm {

		cld 		 				; always go forward

		mov	ebx,[this_object]			; get a pointer to viewport
		mov	edi,[ebx]GraphicViewPortClass.Offset	; get the correct offset
		mov	edx,[ebx]GraphicViewPortClass.Height	; get height from viewport
		mov	esi,[ebx]GraphicViewPortClass.Width		; get width from viewport
		//push	[dword (GraphicViewPort ebx).GVPPitch]	; extra pitch of direct draw surface
		push	[ebx]GraphicViewPortClass.Pitch

		mov	ebx,[ebx]GraphicViewPortClass.XAdd		; esi = add for each line
		add	ebx,[esp]				; Yes, I know its nasty but
		add	esp,4					;      it works!

		;*===================================================================
		; Convert the color byte to a DWORD for fast storing
		;*===================================================================
		mov	al,[color]				; get color to clear to
		mov	ah,al					; extend across WORD
		mov	ecx,eax					; extend across DWORD in
		shl	eax,16					;   several steps
		mov	ax,cx

		;*===================================================================
		; Find out if we should bother to align the row.
		;*===================================================================

		cmp	esi , OPTIMAL_BYTE_COPY			; is it worth aligning them?
		jl	byte_by_byte				;   if not then skip

		;*===================================================================
		; Figure out the alignment offset if there is any
		;*===================================================================
		push	ebx
	
	dword_aligned_loop:
		    mov	ecx , edi
		    mov	ebx , esi
		    neg	ecx
		    and	ecx , 3
		    sub	ebx , ecx
		    rep	stosb
		    mov	ecx , ebx
		    shr	ecx , 2
		    rep	stosd
		    mov	ecx , ebx
		    and	ecx , 3
		    rep	stosb
		    add	edi , [ esp ]
		    dec	edx					; decrement the height
		    jnz	dword_aligned_loop				; if more to do than do it
		    pop	eax
			 jmp	done
		    //ret

		;*===================================================================
		; If not enough bytes to bother aligning copy each line across a byte
		;    at a time.
		;*===================================================================
	byte_by_byte:
		mov	ecx,esi					; get total width in bytes
		rep	stosb					; store the width
		add	edi,ebx					; handle the xadd
		dec	edx					; decrement the height
		jnz	byte_by_byte				; if any left then next line
	done:
	}
}














BOOL __cdecl Linear_Blit_To_Linear(	void *this_object, void * dest, int x_pixel, int y_pixel, int dest_x0, int dest_y0, int pixel_width, int pixel_height, BOOL trans)
{
/*
	;*===================================================================
	;* define the arguements that our function takes.
	;*===================================================================
	ARG    	this_object :DWORD		; this is a member function
	ARG	dest        :DWORD		; what are we blitting to
	ARG	x_pixel     :DWORD		; x pixel position in source
	ARG	y_pixel     :DWORD		; y pixel position in source
	ARG	dest_x0     :dword
	ARG	dest_y0     :dword
	ARG	pixel_width :DWORD		; width of rectangle to blit
	ARG	pixel_height:DWORD		; height of rectangle to blit
	ARG	trans       :DWORD			; do we deal with transparents?

	;*===================================================================
	; Define some locals so that we can handle things quickly
	;*===================================================================
	LOCAL 	x1_pixel :dword
	LOCAL	y1_pixel :dword
	LOCAL	dest_x1 : dword
	LOCAL	dest_y1 : dword
	LOCAL	scr_ajust_width:DWORD
	LOCAL	dest_ajust_width:DWORD
        LOCAL	source_area :  dword
        LOCAL	dest_area :  dword
*/
	
	int	x1_pixel;
	int	y1_pixel;
	int	dest_x1;
	int	dest_y1;
	int	scr_adjust_width;
	int	dest_adjust_width;
	int	source_area;
	int	dest_area;
	
	__asm {	
	
		;This Clipping algorithm is a derivation of the very well known
		;Cohen-Sutherland Line-Clipping test. Due to its simplicity and efficiency
		;it is probably the most commontly implemented algorithm both in software
		;and hardware for clipping lines, rectangles, and convex polygons against
		;a rectagular clipping window. For reference see
		;"COMPUTER GRAPHICS principles and practice by Foley, Vandam, Feiner, Hughes
		; pages 113 to 177".
		; Briefly consist in computing the Sutherland code for both end point of
		; the rectangle to find out if the rectangle is:
		; - trivially accepted (no further clipping test, display rectangle)
		; - trivially rejected (return with no action)
		; - retangle must be iteratively clipped again edges of the clipping window
		;   and the remaining retangle is display.

		; Clip Source Rectangle against source Window boundaries.
			mov  	esi,[this_object]    ; get ptr to src
			xor 	ecx,ecx		    ; Set sutherland code to zero
			xor 	edx,edx		    ; Set sutherland code to zero

		   ; compute the difference in the X axis and get the bit signs into ecx , edx
			mov	edi,[esi]GraphicViewPortClass.Width  ; get width into register
			mov	ebx,[x_pixel]	    ; Get first end point x_pixel into register
			mov	eax,[x_pixel]	    ; Get second end point x_pixel into register
			add	ebx,[pixel_width]   ; second point x1_pixel = x + width
			shld	ecx, eax,1	    ; the sign bit of x_pixel is sutherland code0 bit4
			mov	[x1_pixel],ebx	    ; save second for future use
			inc	edi		    ; move the right edge by one unit
			shld	edx,ebx,1	    ; the sign bit of x1_pixel is sutherland code0 bit4
			sub	eax,edi		    ; compute the difference x0_pixel - width
			sub	ebx,edi		    ; compute the difference x1_pixel - width
			shld	ecx,eax,1	    ; the sign bit of the difference is sutherland code0 bit3
			shld	edx,ebx,1	    ; the sign bit of the difference is sutherland code0 bit3

		   ; the following code is just a repeticion of the above code
		   ; in the Y axis.
			mov	edi,[esi]GraphicViewPortClass.Height ; get height into register
			mov	ebx,[y_pixel]
			mov	eax,[y_pixel]
			add	ebx,[pixel_height]
			shld	ecx,eax,1
			mov	[y1_pixel ],ebx
			inc	edi
			shld	edx,ebx,1
			sub	eax,edi
			sub	ebx,edi
			shld	ecx,eax,1
			shld	edx,ebx,1

		    ; Here we have the to Sutherland code into cl and dl
			xor	cl,5		       ; bit 2 and 0 are complented, reverse then
			xor	dl,5		       ; bit 2 and 0 are complented, reverse then
			mov	al,cl		       ; save code1 in case we have to clip iteratively
			test	dl,cl		       ; if any bit in code0 and its counter bit
			jnz	real_out	       ; in code1 is set then the rectangle in outside
			or	al,dl		       ; if all bit of code0 the counter bit in
			jz	clip_against_dest    ; in code1 is set to zero, then all
						       ; end points of the rectangle are
						       ; inside the clipping window

		     ; if we are here the polygon have to be clip iteratively
			test	cl,1000b	       ; if bit 4 in code0 is set then
			jz	scr_left_ok	       ; x_pixel is smaller than zero
			mov	[x_pixel],0	       ; set x_pixel to cero.

		scr_left_ok:
			test	cl,0010b	       ; if bit 2 in code0 is set then
			jz	scr_bottom_ok	       ; y_pixel is smaller than zero
			mov	[ y_pixel ],0	       ; set y_pixel to cero.

		scr_bottom_ok:
			test	dl,0100b	       ; if bit 3 in code1 is set then
			jz	scr_right_ok	       ; x1_pixel is greater than the width
			mov	eax,[esi]GraphicViewPortClass.Width ; get width into register
			mov	[ x1_pixel ],eax       ; set x1_pixel to width.
		scr_right_ok:
			test	dl,0001b	       ; if bit 0 in code1 is set then
			jz	clip_against_dest    ; y1_pixel is greater than the width
			mov	eax,[esi]GraphicViewPortClass.Height  ; get height into register
			mov	[ y1_pixel ],eax       ; set y1_pixel to height.

		; Clip Source Rectangle against destination Window boundaries.
		clip_against_dest:

		   ; build the destination rectangle before clipping
		   ; dest_x1 = dest_x0 + ( x1_pixel - x_pixel )
		   ; dest_y1 = dest_y0 + ( y1_pixel - y_pixel )
			mov	eax,[dest_x0]	     ; get dest_x0 into eax
			mov	ebx,[dest_y0]	     ; get dest_y0 into ebx
			sub	eax,[x_pixel]	     ; subtract x_pixel from eax
			sub	ebx,[y_pixel]	     ; subtract y_pixel from ebx
			add	eax,[x1_pixel]	     ; add x1_pixel to eax
			add	ebx,[y1_pixel]	     ; add y1_pixel to ebx
			mov	[dest_x1],eax	     ; save eax into dest_x1
			mov	[dest_y1],ebx	     ; save eax into dest_y1


		  ; The followin code is a repeticion of the Sutherland clipping
		  ; descrived above.
			mov  	esi,[dest]	    ; get ptr to src
			xor 	ecx,ecx
			xor 	edx,edx
			mov	edi,[esi]GraphicViewPortClass.Width  ; get width into register
			mov	eax,[dest_x0]
			mov	ebx,[dest_x1]
			shld	ecx,eax,1
			inc	edi
			shld	edx,ebx,1
			sub	eax,edi
			sub	ebx,edi
			shld	ecx,eax,1
			shld	edx,ebx,1

			mov	edi,[esi]GraphicViewPortClass.Height ; get height into register
			mov	eax,[dest_y0]
			mov	ebx,[dest_y1]
			shld	ecx,eax,1
			inc	edi
			shld	edx,ebx,1
			sub	eax,edi
			sub	ebx,edi
			shld	ecx,eax,1
			shld	edx,ebx,1

			xor	cl,5
			xor	dl,5
			mov	al,cl
			test	dl,cl
			jnz	real_out
			or	al,dl
			jz	do_blit

			test	cl,1000b
			jz	dest_left_ok
			mov	eax,[ dest_x0 ]
			mov	[ dest_x0 ],0
			sub	[ x_pixel ],eax

		dest_left_ok:
			test	cl,0010b
			jz	dest_bottom_ok
			mov	eax,[ dest_y0 ]
			mov	[ dest_y0 ],0
			sub	[ y_pixel ],eax


		dest_bottom_ok:
			test	dl,0100b
			jz	dest_right_ok
			mov	ebx,[esi]GraphicViewPortClass.Width  ; get width into register
			mov	eax,[ dest_x1 ]
			mov	[ dest_x1 ],ebx
			sub	eax,ebx
			sub	[ x1_pixel ],eax

		dest_right_ok:
			test	dl,0001b
			jz	do_blit
			mov	ebx,[esi]GraphicViewPortClass.Height  ; get width into register
			mov	eax,[ dest_y1 ]
			mov	[ dest_y1 ],ebx
			sub	eax,ebx
			sub	[ y1_pixel ],eax


		; Here is where	we do the actual blit
		do_blit:
		       cld
		       mov	ebx,[this_object]
		       mov	esi,[ebx]GraphicViewPortClass.Offset
		       mov	eax,[ebx]GraphicViewPortClass.XAdd
		       add	eax,[ebx]GraphicViewPortClass.Width
		       add	eax,[ebx]GraphicViewPortClass.Pitch
		       mov	ecx,eax
		       mul	[y_pixel]
		       add	esi,[x_pixel]
		       mov	[source_area],ecx
		       add	esi,eax

		       add	ecx,[x_pixel ]
		       sub	ecx,[x1_pixel ]
		       mov	[scr_adjust_width ],ecx

		       mov	ebx,[dest]
		       mov	edi,[ebx]GraphicViewPortClass.Offset
		       mov	eax,[ebx]GraphicViewPortClass.XAdd
		       add	eax,[ebx]GraphicViewPortClass.Width
		       add	eax,[ebx]GraphicViewPortClass.Pitch
		       mov	ecx,eax
		       mul	[ dest_y0 ]
		       add	edi,[ dest_x0 ]
		       mov	[ dest_area ],ecx
		       add	edi,eax

		       mov	eax,[ dest_x1 ]
		       sub	eax,[ dest_x0 ]
		       jle	real_out
		       sub	ecx,eax
		       mov	[ dest_adjust_width ],ecx

		       mov	edx,[ dest_y1 ]
		       sub	edx,[ dest_y0 ]
		       jle	real_out

		       cmp	esi,edi
		       jz	real_out
		       jl	backupward_blit

		; ********************************************************************
		; Forward bitblit

		       test	[ trans ],1
		       jnz	forward_Blit_trans


		; the inner loop is so efficient that
		; the optimal consept no longer apply because
		; the optimal byte have to by a number greather than 9 bytes
		       cmp	eax,10
		       jl	forward_loop_bytes

		forward_loop_dword:
		       mov	ecx,edi
		       mov	ebx,eax
		       neg	ecx
		       and	ecx,3
		       sub	ebx,ecx
		       rep	movsb
		       mov	ecx,ebx
		       shr	ecx,2
		       rep	movsd
		       mov	ecx,ebx
		       and	ecx,3
		       rep	movsb
		       add	esi,[ scr_adjust_width ]
		       add	edi,[ dest_adjust_width ]
		       dec	edx
		       jnz	forward_loop_dword
		       jmp	real_out	//ret

		forward_loop_bytes:
		       mov	ecx,eax
		       rep	movsb
		       add	esi,[ scr_adjust_width ]
		       add	edi,[ dest_adjust_width ]
		       dec	edx
		       jnz	forward_loop_bytes
		       jmp	real_out

		forward_Blit_trans:
		       mov	ecx,eax
		       and	ecx,01fh
		       lea	ecx,[ ecx + ecx * 4 ]
		       neg	ecx
		       shr	eax,5
		       lea	ecx,[ transp_reference + ecx * 2 ]
		       mov	[ y1_pixel ],ecx

		forward_loop_trans:
		       mov	ecx,eax
		       jmp	[ y1_pixel ]
		forward_trans_line:
		       //REPT	32
		       //local	transp_pixel
				 //No REPT in msvc inline assembly.
				 // Save ECX and use as counter instead. ST - 12/19/2018 5:41PM
				 push	ecx
				 mov	ecx, 32

		rept_loop:
				mov	bl,[ esi ]
				test	bl,bl
				jz		transp_pixel
				mov	[ edi ],bl
		transp_pixel:
				inc	esi
				inc	edi

				dec	ecx			//ST - 12/19/2018 5:44PM
				jnz	rept_loop	//ST - 12/19/2018 5:44PM

				pop	ecx			//ST - 12/19/2018 5:44PM

			//ENDM
		    transp_reference:
		       dec	ecx
		       jge	forward_trans_line
		       add	esi,[ scr_adjust_width ]
		       add	edi,[ dest_adjust_width ]
		       dec	edx
		       jnz	forward_loop_trans
		       jmp	real_out		//ret


		; ************************************************************************
		; backward bitblit

		backupward_blit:

			mov	ebx,[ source_area ]
			dec	edx
			add	esi,eax
			imul    ebx,edx
			std
			lea	esi,[ esi + ebx - 1 ]

			mov	ebx,[ dest_area ]
			add	edi,eax
			imul    ebx,edx
			lea	edi,[ edi + ebx - 1]

		       test	[ trans ],1
		       jnz	backward_Blit_trans

		        cmp	eax,15
		        jl	backward_loop_bytes

		backward_loop_dword:
			push	edi
			push	esi
			lea	ecx,[edi+1]
			mov	ebx,eax
			and	ecx,3		; Get non aligned bytes.
			sub	ebx,ecx		; remove that from the total size to be copied later.
			rep	movsb		; do the copy.
			sub	esi,3
			mov	ecx,ebx		; Get number of bytes left.
		 	sub	edi,3
			shr	ecx,2		; Do 4 bytes at a time.
			rep	movsd		; do the dword copy.
			mov	ecx,ebx
			add	esi,3
			add	edi,3
			and	ecx,03h
			rep	movsb		; finnish the remaining bytes.
			pop	esi
			pop	edi
		        sub	esi,[ source_area ]
		        sub	edi,[ dest_area ]
			dec	edx
			jge	backward_loop_dword
			cld
			jmp	real_out		//ret

		backward_loop_bytes:
			push	edi
			mov	ecx,eax		; remove that from the total size to be copied later.
			push	esi
			rep	movsb		; do the copy.
			pop	esi
			pop	edi
		        sub	esi,[ source_area ]
		        sub	edi,[ dest_area ]
			dec	edx
			jge	backward_loop_bytes
			cld
			jmp	real_out		//ret

		backward_Blit_trans:
		       mov	ecx,eax
		       and	ecx,01fh
		       lea	ecx,[ ecx + ecx * 4 ]
		       neg	ecx
		       shr	eax,5
		       lea	ecx,[ back_transp_reference + ecx * 2 ]
		       mov	[ y1_pixel ],ecx

		backward_loop_trans:
		       mov	ecx,eax
		       push	edi
		       push	esi
		       jmp	[ y1_pixel ]
		backward_trans_line:
		       //REPT	32
		       //local	transp_pixel2
				 //No REPT in msvc inline assembly.
				 // Save ECX and use as counter instead. ST - 12/19/2018 5:41PM
				 push	ecx
				 mov	ecx, 32
		rept_loop2:
				 mov	bl,[ esi ]
				 test	bl,bl
				 jz	transp_pixel2
				 mov	[ edi ],bl
		transp_pixel2:
				 dec	esi
				 dec	edi

				 dec	ecx				//ST - 12/19/2018 5:44PM
				 jnz	rept_loop2		//ST - 12/19/2018 5:44PM

				 pop	ecx				//ST - 12/19/2018 5:44PM
			
			//ENDM
		    
			 back_transp_reference:
		       dec	ecx
		       jge	backward_trans_line
		       pop	esi
		       pop	edi
		       sub	esi,[ source_area ]
		       sub	edi,[ dest_area ]
		       dec	edx
		       jge	backward_loop_trans
		       cld
		       //ret

		real_out:
	}
}












/*
;***************************************************************************
;* VVC::SCALE -- Scales a virtual viewport to another virtual viewport     *
;*                                                                         *
;* INPUT:                                                                  *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* WARNINGS:                                                               *
;*                                                                         *
;* HISTORY:                                                                *
;*   06/16/1994 PWG : Created.                                             *
;*=========================================================================*
	PROC	Linear_Scale_To_Linear C NEAR
	USES	eax,ebx,ecx,edx,esi,edi
*/

// Ran out of registers so had to use ebp. ST - 12/19/2018 6:22PM
#pragma warning (push)
#pragma warning (disable : 4731)

BOOL __cdecl Linear_Scale_To_Linear(void *this_object, void *dest, int src_x, int src_y, int dst_x, int dst_y, int src_width, int src_height, int dst_width, int dst_height, BOOL trans, char *remap)
{
/*			  

	;*===================================================================
	;* Define the arguements that our function takes.
	;*===================================================================
	ARG	this_object:DWORD		; pointer to source view port
	ARG	dest:DWORD		; pointer to destination view port
	ARG	src_x:DWORD		; source x offset into view port
	ARG	src_y:DWORD		; source y offset into view port
	ARG	dst_x:DWORD		; dest x offset into view port
	ARG	dst_y:DWORD		; dest y offset into view port
	ARG	src_width:DWORD		; width of source rectangle
	ARG	src_height:DWORD	; height of source rectangle
	ARG	dst_width:DWORD		; width of dest rectangle
	ARG	dst_height:DWORD	; width of dest height
	ARG	trans:DWORD		; is this transparent?
	ARG	remap:DWORD		; pointer to table to remap source

	;*===================================================================
	;* Define local variables to hold the viewport characteristics
	;*===================================================================
	local	src_x0 : dword
	local	src_y0 : dword
	local	src_x1 : dword
	local	src_y1 : dword

	local	dst_x0 : dword
	local	dst_y0 : dword
	local	dst_x1 : dword
	local	dst_y1 : dword

	local	src_win_width : dword
	local	dst_win_width : dword
	local	dy_intr : dword
	local	dy_frac : dword
	local	dy_acc  : dword
	local	dx_frac : dword

	local	counter_x     : dword
	local	counter_y     : dword
	local	remap_counter :dword
	local	entry : dword
*/
	
	int src_x0;
	int src_y0;
	int src_x1;
	int src_y1;

	int dst_x0;
	int dst_y0;
	int dst_x1;
	int dst_y1;

	int src_win_width;
	int dst_win_width;
	int dy_intr;
	int dy_frac;
	int dy_acc;
	int dx_frac;

	int counter_x;
	int counter_y;
	int remap_counter;
	int entry;
	
	
	__asm {
		
		;*===================================================================
		;* Check for scale error when to or from size 0,0
		;*===================================================================
		cmp	[dst_width],0
		je	all_done
		cmp	[dst_height],0
		je	all_done
		cmp	[src_width],0
		je	all_done
		cmp	[src_height],0
		je	all_done

		mov	eax , [ src_x ]
		mov	ebx , [ src_y ]
		mov	[ src_x0 ] , eax
		mov	[ src_y0 ] , ebx
		add	eax , [ src_width ]
		add	ebx , [ src_height ]
		mov	[ src_x1 ] , eax
		mov	[ src_y1 ] , ebx

		mov	eax , [ dst_x ]
		mov	ebx , [ dst_y ]
		mov	[ dst_x0 ] , eax
		mov	[ dst_y0 ] , ebx
		add	eax , [ dst_width ]
		add	ebx , [ dst_height ]
		mov	[ dst_x1 ] , eax
		mov	[ dst_y1 ] , ebx

	; Clip Source Rectangle against source Window boundaries.
		mov  	esi , [ this_object ]	    ; get ptr to src
		xor 	ecx , ecx
		xor 	edx , edx
		mov	edi , [esi]GraphicViewPortClass.Width  ; get width into register
		mov	eax , [ src_x0 ]
		mov	ebx , [ src_x1 ]
		shld	ecx , eax , 1
		inc	edi
		shld	edx , ebx , 1
		sub	eax , edi
		sub	ebx , edi
		shld	ecx , eax , 1
		shld	edx , ebx , 1

		mov	edi,[esi]GraphicViewPortClass.Height ; get height into register
		mov	eax , [ src_y0 ]
		mov	ebx , [ src_y1 ]
		shld	ecx , eax , 1
		inc	edi
		shld	edx , ebx , 1
		sub	eax , edi
		sub	ebx , edi
		shld	ecx , eax , 1
		shld	edx , ebx , 1

		xor	cl , 5
		xor	dl , 5
		mov	al , cl
		test	dl , cl
		jnz	all_done
		or	al , dl
		jz	clip_against_dest
		mov	bl , dl
		test	cl , 1000b
		jz	src_left_ok
		xor	eax , eax
		mov	[ src_x0 ] , eax
		sub	eax , [ src_x ]
		imul	[ dst_width ]
		idiv	[ src_width ]
		add	eax , [ dst_x ]
		mov	[ dst_x0 ] , eax

	src_left_ok:
		test	cl , 0010b
		jz	src_bottom_ok
		xor	eax , eax
		mov	[ src_y0 ] , eax
		sub	eax , [ src_y ]
		imul	[ dst_height ]
		idiv	[ src_height ]
		add	eax , [ dst_y ]
		mov	[ dst_y0 ] , eax

	src_bottom_ok:
		test	bl , 0100b
		jz	src_right_ok
		mov	eax , [esi]GraphicViewPortClass.Width  ; get width into register
		mov	[ src_x1 ] , eax
		sub	eax , [ src_x ]
		imul	[ dst_width ]
		idiv	[ src_width ]
		add	eax , [ dst_x ]
		mov	[ dst_x1 ] , eax

	src_right_ok:
		test	bl , 0001b
		jz	clip_against_dest
		mov	eax , [esi]GraphicViewPortClass.Height  ; get width into register
		mov	[ src_y1 ] , eax
		sub	eax , [ src_y ]
		imul	[ dst_height ]
		idiv	[ src_height ]
		add	eax , [ dst_y ]
		mov	[ dst_y1 ] , eax

	; Clip destination Rectangle against source Window boundaries.
	clip_against_dest:
		mov  	esi , [ dest ]	    ; get ptr to src
		xor 	ecx , ecx
		xor 	edx , edx
		mov	edi , [esi]GraphicViewPortClass.Width  ; get width into register
		mov	eax , [ dst_x0 ]
		mov	ebx , [ dst_x1 ]
		shld	ecx , eax , 1
		inc	edi
		shld	edx , ebx , 1
		sub	eax , edi
		sub	ebx , edi
		shld	ecx , eax , 1
		shld	edx , ebx , 1

		mov	edi,[esi]GraphicViewPortClass.Height ; get height into register
		mov	eax , [ dst_y0 ]
		mov	ebx , [ dst_y1 ]
		shld	ecx , eax , 1
		inc	edi
		shld	edx , ebx , 1
		sub	eax , edi
		sub	ebx , edi
		shld	ecx , eax , 1
		shld	edx , ebx , 1

		xor	cl , 5
		xor	dl , 5
		mov	al , cl
		test	dl , cl
		jnz	all_done
		or	al , dl
		jz	do_scaling
		mov	bl , dl
		test	cl , 1000b
		jz	dst_left_ok
		xor	eax , eax
		mov	[ dst_x0 ] , eax
		sub	eax , [ dst_x ]
		imul	[ src_width ]
		idiv	[ dst_width ]
		add	eax , [ src_x ]
		mov	[ src_x0 ] , eax

	dst_left_ok:
		test	cl , 0010b
		jz	dst_bottom_ok
		xor	eax , eax
		mov	[ dst_y0 ] , eax
		sub	eax , [ dst_y ]
		imul	[ src_height ]
		idiv	[ dst_height ]
		add	eax , [ src_y ]
		mov	[ src_y0 ] , eax

	dst_bottom_ok:
		test	bl , 0100b
		jz	dst_right_ok
		mov	eax , [esi]GraphicViewPortClass.Width  ; get width into register
		mov	[ dst_x1 ] , eax
		sub	eax , [ dst_x ]
		imul	[ src_width ]
		idiv	[ dst_width ]
		add	eax , [ src_x ]
		mov	[ src_x1 ] , eax

	dst_right_ok:
		test	bl , 0001b
		jz	do_scaling

		mov	eax , [esi]GraphicViewPortClass.Height  ; get width into register
		mov	[ dst_y1 ] , eax
		sub	eax , [ dst_y ]
		imul	[ src_height ]
		idiv	[ dst_height ]
		add	eax , [ src_y ]
		mov	[ src_y1 ] , eax

	do_scaling:

   	    cld
   	    mov	ebx , [ this_object ]
   	    mov	esi , [ebx]GraphicViewPortClass. Offset
   	    mov	eax , [ebx]GraphicViewPortClass. XAdd
   	    add	eax , [ebx]GraphicViewPortClass. Width
   	    add	eax , [ebx]GraphicViewPortClass. Pitch
   	    mov	[ src_win_width ] , eax
   	    mul	[ src_y0 ]
   	    add	esi , [ src_x0 ]
   	    add	esi , eax

   	    mov	ebx , [ dest ]
   	    mov	edi , [ebx]GraphicViewPortClass. Offset
   	    mov	eax , [ebx]GraphicViewPortClass. XAdd
   	    add	eax , [ebx]GraphicViewPortClass. Width
   	    add	eax , [ebx]GraphicViewPortClass. Pitch
   	    mov	[ dst_win_width ] , eax
   	    mul	[ dst_y0 ]
   	    add	edi , [ dst_x0 ]
   	    add	edi , eax

   	    mov	eax , [ src_height ]
   	    xor	edx , edx
   	    mov	ebx , [ dst_height ]
   	    idiv	[ dst_height ]
   	    imul	eax , [ src_win_width ]
   	    neg	ebx
   	    mov	[ dy_intr ] , eax
   	    mov	[ dy_frac ] , edx
   	    mov	[ dy_acc ]  , ebx

   	    mov	eax , [ src_width ]
   	    xor	edx , edx
   	    shl	eax , 16
   	    idiv	[ dst_width ]
   	    xor	edx , edx
   	    shld	edx , eax , 16
   	    shl	eax , 16

   	    mov	ecx , [ dst_y1 ]
   	    mov	ebx , [ dst_x1 ]
   	    sub	ecx , [ dst_y0 ]
   	    jle	all_done
   	    sub	ebx , [ dst_x0 ]
   	    jle	all_done

   	    mov	[ counter_y ] , ecx

   	    cmp	[ trans ] , 0
   	    jnz	transparency

   	    cmp	[ remap ] , 0
   	    jnz	normal_remap

	; *************************************************************************
	; normal scale
   	    mov	ecx , ebx
   	    and	ecx , 01fh
   	    lea	ecx , [ ecx + ecx * 2 ]
   	    shr	ebx , 5
   	    neg	ecx
   	    mov	[ counter_x ] , ebx
   	    lea	ecx , [ ref_point + ecx + ecx * 2 ]
   	    mov	[ entry ] , ecx

 	outter_loop:
   	    push	esi
   	    push	edi
   	    xor	ecx , ecx
   	    mov	ebx , [ counter_x ]
   	    jmp	[ entry ]
 	inner_loop:
   	    // REPT not supported for inline asm. ST - 12/19/2018 6:11PM
			 //REPT	32
			 	push ebx		//ST - 12/19/2018 6:11PM
				mov ebx,32	//ST - 12/19/2018 6:11PM
rept_loop:
		       mov	cl , [ esi ]
		       add	ecx , eax
		       adc	esi , edx
		       mov	[ edi ] , cl
		       inc	edi

				dec ebx				//ST - 12/19/2018 6:11PM
				jnz rept_loop		//ST - 12/19/2018 6:11PM
				pop ebx				//ST - 12/19/2018 6:11PM
   	    //ENDM
 	ref_point:
   	    dec	ebx
   	    jge	inner_loop

   	    pop	edi
   	    pop	esi
   	    add	edi , [ dst_win_width ]
   	    add	esi , [ dy_intr ]

   	    mov	ebx , [ dy_acc ]
   	    add	ebx , [ dy_frac ]
   	    jle	skip_line
   	    add	esi , [ src_win_width ]
   	    sub	ebx , [ dst_height ]
	skip_line:
		dec	[ counter_y ]
		mov	[ dy_acc ] , ebx
		jnz	outter_loop
		jmp	all_done	//ret


	; *************************************************************************
	; normal scale with remap

	normal_remap:
   	    mov	ecx , ebx
   	    mov	[ dx_frac ], eax
   	    and	ecx , 01fh
   	    mov	eax , [ remap ]
   	    shr	ebx , 5
   	    imul	ecx , - 13
   	    mov	[ counter_x ] , ebx
   	    lea	ecx , [ remapref_point + ecx ]
   	    mov	[ entry ] , ecx

 	remapoutter_loop:
   	    mov	ebx , [ counter_x ]
   	    push	esi
   	    mov	[ remap_counter ] , ebx
   	    push	edi
   	    xor	ecx , ecx
   	    xor	ebx , ebx
   	    jmp	[ entry ]
 	remapinner_loop:
   	    // REPT not supported for inline asm. ST - 12/19/2018 6:11PM
			 //REPT	32
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi
		       mov	bl , [ esi ]
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		       inc	edi

   	    //ENDM
 	remapref_point:
   	    dec	[ remap_counter ]
   	    jge	remapinner_loop

   	    pop	edi
   	    pop	esi
   	    add	edi , [ dst_win_width ]
   	    add	esi , [ dy_intr ]

   	    mov	ebx , [ dy_acc ]
   	    add	ebx , [ dy_frac ]
   	    jle	remapskip_line
   	    add	esi , [ src_win_width ]
   	    sub	ebx , [ dst_height ]
	remapskip_line:
		dec	[ counter_y ]
		mov	[ dy_acc ] , ebx
		jnz	remapoutter_loop
		jmp	all_done	//ret


	;****************************************************************************
	; scale with trnsparency

	transparency:
   	    cmp	[ remap ] , 0
   	    jnz	trans_remap

	; *************************************************************************
	; normal scale with transparency
   	    mov	ecx , ebx
   	    and	ecx , 01fh
   	    imul	ecx , -13
   	    shr	ebx , 5
   	    mov	[ counter_x ] , ebx
   	    lea	ecx , [ trans_ref_point + ecx ]
   	    mov	[ entry ] , ecx

 	trans_outter_loop:
   	    xor	ecx , ecx
   	    push	esi
   	    push	edi
   	    mov	ebx , [ counter_x ]
   	    jmp	[ entry ]
 	trans_inner_loop:
   	    
   	    // REPT not supported for inline asm. ST - 12/19/2018 6:11PM
			 //REPT	32
			 	push ebx		//ST - 12/19/2018 6:11PM
				mov ebx,32	//ST - 12/19/2018 6:11PM
rept_loop2:
			 
		       mov	cl , [ esi ]
		       test	cl , cl
		       jz	trans_pixel
		       mov	[ edi ] , cl
   	    trans_pixel:
		       add	ecx , eax
		       adc	esi , edx
		       inc	edi
				
				dec ebx				//ST - 12/19/2018 6:11PM
				jnz rept_loop2		//ST - 12/19/2018 6:11PM
				pop ebx				//ST - 12/19/2018 6:11PM
   	    
			 //ENDM
 	trans_ref_point:
   	    dec	ebx
   	    jge	trans_inner_loop

   	    pop	edi
   	    pop	esi
   	    add	edi , [ dst_win_width ]
   	    add	esi , [ dy_intr ]

   	    mov	ebx , [ dy_acc ]
   	    add	ebx , [ dy_frac ]
   	    jle	trans_skip_line
   	    add	esi , [ src_win_width ]
   	    sub	ebx , [ dst_height ]
	trans_skip_line:
		dec	[ counter_y ]
		mov	[ dy_acc ] , ebx
		jnz	trans_outter_loop
		jmp	all_done	//ret


	; *************************************************************************
	; normal scale with remap

	trans_remap:
   	    mov	ecx , ebx
   	    mov	[ dx_frac ], eax
   	    and	ecx , 01fh
   	    mov	eax , [ remap ]
   	    shr	ebx , 5
   	    imul	ecx , - 17
   	    mov	[ counter_x ] , ebx
   	    lea	ecx , [ trans_remapref_point + ecx ]
   	    mov	[ entry ] , ecx

 	trans_remapoutter_loop:
   	    mov	ebx , [ counter_x ]
   	    push	esi
   	    mov	[ remap_counter ] , ebx
   	    push	edi
   	    xor	ecx , ecx
   	    xor	ebx , ebx
   	    jmp	[ entry ]
 	
	
	trans_remapinner_loop:
   	    // REPT not supported for inline asm. ST - 12/19/2018 6:11PM
			 //REPT	32
			 // Run out of registers so use ebp
			 	push ebp		//ST - 12/19/2018 6:11PM
				mov ebp,32	//ST - 12/19/2018 6:11PM
rept_loop3:
		       mov	bl , [ esi ]
		       test	bl , bl
		       jz	trans_pixel2
		       mov	cl , [ eax + ebx ]
		       mov	[ edi ] , cl
		  trans_pixel2:
		       add	ecx , [ dx_frac ]
		       adc	esi , edx
		       inc	edi
				
				dec ebp				//ST - 12/19/2018 6:11PM
				jnz rept_loop3		//ST - 12/19/2018 6:11PM
				pop ebp				//ST - 12/19/2018 6:11PM

   	    //ENDM

 	trans_remapref_point:
   	    dec	[ remap_counter ]
   	    jge	trans_remapinner_loop

   	    pop	edi
   	    pop	esi
   	    add	edi , [ dst_win_width ]
   	    add	esi , [ dy_intr ]

   	    mov	ebx , [ dy_acc ]
   	    add	ebx , [ dy_frac ]
   	    jle	trans_remapskip_line
   	    add	esi , [ src_win_width ]
   	    sub	ebx , [ dst_height ]
	trans_remapskip_line:
		dec	[ counter_y ]
		mov	[ dy_acc ] , ebx
		jnz	trans_remapoutter_loop
		//ret


	all_done:
	}
}


#pragma warning (pop)




















unsigned int LastIconset = 0;
unsigned int StampPtr = 0;	//	DD	0	; Pointer to icon data.

unsigned int IsTrans = 0;	//		DD	0	; Pointer to transparent icon flag table.

unsigned int MapPtr = 0;	//		DD	0	; Pointer to icon map.
unsigned int IconWidth = 0;	//	DD	0	; Width of icon in pixels.
unsigned int IconHeight = 0;	//	DD	0	; Height of icon in pixels.
unsigned int IconSize = 0;		//	DD	0	; Number of bytes for each icon data.
unsigned int IconCount = 0;	//	DD	0	; Number of icons in the set.



#if (0)
LastIconset	DD	0	; Pointer to last iconset initialized.
StampPtr	DD	0	; Pointer to icon data.

IsTrans		DD	0	; Pointer to transparent icon flag table.

MapPtr		DD	0	; Pointer to icon map.
IconWidth	DD	0	; Width of icon in pixels.
IconHeight	DD	0	; Height of icon in pixels.
IconSize	DD	0	; Number of bytes for each icon data.
IconCount	DD	0	; Number of icons in the set.


GLOBAL C	Buffer_Draw_Stamp:near
GLOBAL C	Buffer_Draw_Stamp_Clip:near

; 256 color icon system.
#endif


/*
;***********************************************************
; INIT_STAMPS
;
; VOID cdecl Init_Stamps(VOID *icondata);
;
; This routine initializes the stamp data.
; Bounds Checking: NONE
;
;*
*/ 
extern "C" void __cdecl Init_Stamps(unsigned int icondata)
{

	__asm {
		pushad										// ST - 12/20/2018 10:30AM
		
		; Verify legality of parameter.
		cmp	[icondata],0
		je	short fini

		; Don't initialize if already initialized to this set (speed reasons).
		mov	edi,[icondata]
		cmp	[LastIconset],edi
		je	short fini
		mov	[LastIconset],edi

		; Record number of icons in set.
		movzx	eax,[edi]IControl_Type.Count
		mov	[IconCount],eax

		; Record width of icon.
		movzx	eax,[edi]IControl_Type.Width
		mov	[IconWidth],eax

		; Record height of icon.
		movzx	ebx,[edi]IControl_Type.Height
		mov	[IconHeight],ebx

		; Record size of icon (in bytes).
		mul	ebx
		mov	[IconSize],eax

		; Record hard pointer to icon map data.
		mov	eax,[edi]IControl_Type.Map
		add	eax,edi
		mov	[MapPtr],eax

//nomap:
		; Record hard pointer to icon data.
		mov	eax,edi
		add	eax,[edi]IControl_Type.Icons
		mov	[StampPtr],eax

		; Record the transparent table.
		mov	eax,edi
		add	eax,[edi]IControl_Type.TransFlag
		mov	[IsTrans],eax

fini:
		popad										// ST - 12/20/2018 10:30AM

	}
}


/*
;***********************************************************

;***********************************************************
; DRAW_STAMP
;
; VOID cdecl Buffer_Draw_Stamp(VOID *icondata, WORD icon, WORD x_pixel, WORD y_pixel, VOID *remap);
;
; This routine renders the icon at the given coordinate.
;
; The remap table is a 256 byte simple pixel translation table to use when
; drawing the icon.  Transparency check is performed AFTER the remap so it is possible to
; remap valid colors to be invisible (for special effect reasons).
; This routine is fastest when no remap table is passed in.
;*
*/

void __cdecl Buffer_Draw_Stamp(void const *this_object, void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap)
{
	unsigned int	modulo = 0;
	unsigned int	iwidth = 0;
	unsigned char	doremap = 0;


/*
		PROC	Buffer_Draw_Stamp C near

		ARG	this_object:DWORD		; this is a member function
		ARG	icondata:DWORD		; Pointer to icondata.
		ARG	icon:DWORD		; Icon number to draw.
		ARG	x_pixel:DWORD		; X coordinate of icon.
		ARG	y_pixel:DWORD		; Y coordinate of icon.
		ARG	remap:DWORD 		; Remap table.

		LOCAL	modulo:DWORD		; Modulo to get to next row.
		LOCAL	iwidth:DWORD		; Icon width (here for speedy access).
		LOCAL	doremap:BYTE		; Should remapping occur?
*/
		
	__asm {

			pushad
			cmp	[icondata],0
			je	proc_out

			; Initialize the stamp data if necessary.
			mov	eax,[icondata]
			cmp	[LastIconset],eax
			je		short noreset
			push	eax
			call	Init_Stamps
			pop	eax			             // Clean up stack. ST - 12/20/2018 10:42AM
noreset:

			; Determine if the icon number requested is actually in the set.
			; Perform the logical icon to actual icon number remap if necessary.
			mov	ebx,[icon]
			cmp	[MapPtr],0
			je	short notmap
			mov	edi,[MapPtr]
			mov	bl,[edi+ebx]
notmap:
			cmp	ebx,[IconCount]
			jae	proc_out
			mov	[icon],ebx		; Updated icon number.

			; If the remap table pointer passed in is NULL, then flag this condition
			; so that the faster (non-remapping) icon draw loop will be used.
			cmp	[remap],0
			setne	[doremap]

			; Get pointer to position to render icon. EDI = ptr to destination page.
			mov	ebx,[this_object]
			mov	edi,[ebx]GraphicViewPortClass.Offset
			mov	eax,[ebx]GraphicViewPortClass.Width
			add	eax,[ebx]GraphicViewPortClass.XAdd
			add	eax,[ebx]GraphicViewPortClass.Pitch
			push	eax			; save viewport full width for lower
			mul	[y_pixel]
			add	edi,eax
			add	edi,[x_pixel]

			; Determine row modulo for advancing to next line.
			pop	eax			; retrieve viewport width
			sub	eax,[IconWidth]
			mov	[modulo],eax

			; Setup some working variables.
			mov	ecx,[IconHeight]	; Row counter.
			mov	eax,[IconWidth]
			mov	[iwidth],eax		; Stack copy of byte width for easy BP access.

			; Fetch pointer to start of icon's data.  ESI = ptr to icon data.
			mov	eax,[icon]
			mul	[IconSize]
			mov	esi,[StampPtr]
			add	esi,eax

			; Determine whether simple icon draw is sufficient or whether the
			; extra remapping icon draw is needed.
			cmp	[BYTE PTR doremap],0
			je	short istranscheck

			;************************************************************
			; Complex icon draw -- extended remap.
			; EBX = Palette pointer (ready for XLAT instruction).
			; EDI = Pointer to icon destination in page.
			; ESI = Pointer to icon data.
			; ECX = Number of pixel rows.
		;;;	mov	edx,[remap]
		 mov ebx,[remap]
			xor	eax,eax
xrowloop:
			push	ecx
			mov	ecx,[iwidth]

xcolumnloop:
			lodsb
		;;;	mov	ebx,edx
		;;;	add	ebx,eax
		;;;	mov	al,[ebx]		; New real color to draw.
		 xlatb
			or	al,al
			jz	short xskip1		; Transparency skip check.
			mov	[edi],al
xskip1:
			inc	edi
			loop	xcolumnloop

			pop	ecx
			add	edi,[modulo]
			loop	xrowloop
			jmp	short proc_out


			;************************************************************
			; Check to see if transparent or generic draw is necessary.
istranscheck:
			mov	ebx,[IsTrans]
			add	ebx,[icon]
			cmp	[BYTE PTR ebx],0
			jne	short rowloop

			;************************************************************
			; Fast non-transparent icon draw routine.
			; ES:DI = Pointer to icon destination in page.
			; DS:SI = Pointer to icon data.
			; CX = Number of pixel rows.
			mov	ebx,ecx
			shr	ebx,2
			mov	edx,[modulo]
			mov	eax,[iwidth]
			shr	eax,2
loop1:
			mov	ecx,eax
			rep movsd
			add	edi,edx

			mov	ecx,eax
			rep movsd
			add	edi,edx

			mov	ecx,eax
			rep movsd
			add	edi,edx

			mov	ecx,eax
			rep movsd
			add	edi,edx

			dec	ebx
			jnz	loop1
			jmp	short proc_out

			;************************************************************
			; Transparent icon draw routine -- no extended remap.
			; ES:DI = Pointer to icon destination in page.
			; DS:SI = Pointer to icon data.
			; CX = Number of pixel rows.
rowloop:
			push	ecx
			mov	ecx,[iwidth]

columnloop:
			lodsb
			or	al,al
			jz	short skip1		; Transparency check.
			mov	[edi],al
skip1:
			inc	edi
			loop	columnloop

			pop	ecx
			add	edi,[modulo]
			loop	rowloop

			; Cleanup and exit icon drawing routine.
proc_out:
			popad
			//ret
	}
}




/*
;***********************************************************
; DRAW_STAMP_CLIP
;
; VOID cdecl MCGA_Draw_Stamp_Clip(VOID *icondata, WORD icon, WORD x_pixel, WORD y_pixel, VOID *remap, LONG min_x, LONG min_y, LONG max_x, LONG max_y);
;
; This routine renders the icon at the given coordinate.
;
; The remap table is a 256 byte simple pixel translation table to use when
; drawing the icon.  Transparency check is performed AFTER the remap so it is possible to
; remap valid colors to be invisible (for special effect reasons).
; This routine is fastest when no remap table is passed in.
;*
*/	
void __cdecl Buffer_Draw_Stamp_Clip(void const *this_object, void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap, int min_x, int min_y, int max_x, int max_y)
{
	
	
	unsigned int	modulo = 0;
	unsigned int	iwidth = 0;
	unsigned int	skip = 0;
	unsigned char	doremap = 0;
	
		
/*		
	ARG	this_object:DWORD	; this is a member function
	ARG	icondata:DWORD		; Pointer to icondata.
	ARG	icon:DWORD		; Icon number to draw.
	ARG	x_pixel:DWORD		; X coordinate of icon.
	ARG	y_pixel:DWORD		; Y coordinate of icon.
	ARG	remap:DWORD 		; Remap table.
	ARG	min_x:DWORD		; Clipping rectangle boundary
	ARG	min_y:DWORD		; Clipping rectangle boundary
	ARG	max_x:DWORD		; Clipping rectangle boundary
	ARG	max_y:DWORD		; Clipping rectangle boundary

	LOCAL	modulo:DWORD		; Modulo to get to next row.
	LOCAL	iwidth:DWORD		; Icon width (here for speedy access).
	LOCAL	skip:DWORD		; amount to skip per row of icon data
	LOCAL	doremap:BYTE		; Should remapping occur?
*/
	__asm {
			pushad
			cmp	[icondata],0
			je	proc_out

			; Initialize the stamp data if necessary.
			mov	eax,[icondata]
			cmp	[LastIconset],eax
			je		short noreset2
			push	eax
			call	Init_Stamps
			pop	eax			             // Clean up stack. ST - 12/20/2018 10:42AM
noreset2:

			; Determine if the icon number requested is actually in the set.
			; Perform the logical icon to actual icon number remap if necessary.
			mov	ebx,[icon]
			cmp	[MapPtr],0
			je	short notmap2
			mov	edi,[MapPtr]
			mov	bl,[edi+ebx]
notmap2:
			cmp	ebx,[IconCount]
			jae	proc_out
			mov	[icon],ebx		; Updated icon number.

			; Setup some working variables.
			mov	ecx,[IconHeight]	; Row counter.
			mov	eax,[IconWidth]
			mov	[iwidth],eax		; Stack copy of byte width for easy BP access.

			; Fetch pointer to start of icon's data.  ESI = ptr to icon data.
			mov	eax,[icon]
			mul	[IconSize]
			mov	esi,[StampPtr]
			add	esi,eax

			; Update the clipping window coordinates to be valid maxes instead of width & height
			; , and change the coordinates to be window-relative
			mov	ebx,[min_x]
			add	[max_x],ebx
			add	[x_pixel],ebx		; make it window-relative
			mov	ebx,[min_y]
			add	[max_y],ebx
			add	[y_pixel],ebx		; make it window-relative

			; See if the icon is within the clipping window
			; First, verify that the icon position is less than the maximums
			mov	ebx,[x_pixel]
			cmp	ebx,[max_x]
			jge	proc_out
			mov	ebx,[y_pixel]
			cmp	ebx,[max_y]
			jge	proc_out
			; Now verify that the icon position is >= the minimums
			add	ebx,[IconHeight]
			cmp	ebx,[min_y]
			jle	proc_out
			mov	ebx,[x_pixel]
			add	ebx,[IconWidth]
			cmp	ebx,[min_x]
			jle	proc_out

			; Now, clip the x, y, width, and height variables to be within the
			; clipping rectangle
			mov	ebx,[x_pixel]
			cmp	ebx,[min_x]
			jge	nominxclip
			; x < minx, so must clip
			mov	ebx,[min_x]
			sub	ebx,[x_pixel]
			add	esi,ebx		; source ptr += (minx - x)
			sub	[iwidth],ebx	; icon width -= (minx - x)
			mov	ebx,[min_x]
			mov	[x_pixel],ebx

nominxclip:
			mov	eax,[IconWidth]
			sub	eax,[iwidth]
			mov	[skip],eax

			; Check for x+width > max_x
			mov	eax,[x_pixel]
			add	eax,[iwidth]
			cmp	eax,[max_x]
			jle	nomaxxclip
			; x+width is greater than max_x, so must clip width down
			mov	eax,[iwidth]	; eax = old width
			mov	ebx,[max_x]
			sub	ebx,[x_pixel]
			mov	[iwidth],ebx	; iwidth = max_x - xpixel
			sub	eax,ebx
			add	[skip],eax	; skip += (old width - iwidth)
nomaxxclip:
			; check if y < miny
			mov	eax,[min_y]
			cmp	eax,[y_pixel]	; if(miny <= y_pixel), no clip needed
			jle	nominyclip
			sub	eax,[y_pixel]
			sub	ecx,eax		; height -= (miny - y)
			mul	[IconWidth]
			add	esi,eax		; icon source ptr += (width * (miny - y))
			mov	eax,[min_y]
			mov	[y_pixel],eax	; y = miny
nominyclip:
			; check if (y+height) > max y
			mov	eax,[y_pixel]
			add	eax,ecx
			cmp	eax,[max_y]	; if (y + height <= max_y), no clip needed
			jle	nomaxyclip
			mov	ecx,[max_y]	; height = max_y - y_pixel
			sub	ecx,[y_pixel]
nomaxyclip:

			; If the remap table pointer passed in is NULL, then flag this condition
			; so that the faster (non-remapping) icon draw loop will be used.
			cmp	[remap],0
			setne	[doremap]

			; Get pointer to position to render icon. EDI = ptr to destination page.
			mov	ebx,[this_object]
			mov	edi,[ebx]GraphicViewPortClass.Offset
			mov	eax,[ebx]GraphicViewPortClass.Width
			add	eax,[ebx]GraphicViewPortClass.XAdd
			add	eax,[ebx]GraphicViewPortClass.Pitch
			push	eax			; save viewport full width for lower
			mul	[y_pixel]
			add	edi,eax
			add	edi,[x_pixel]

			; Determine row modulo for advancing to next line.
			pop	eax			; retrieve viewport width
			sub	eax,[iwidth]
			mov	[modulo],eax

			; Determine whether simple icon draw is sufficient or whether the
			; extra remapping icon draw is needed.
			cmp	[BYTE PTR doremap],0
			je	short istranscheck2

			;************************************************************
			; Complex icon draw -- extended remap.
			; EBX = Palette pointer (ready for XLAT instruction).
			; EDI = Pointer to icon destination in page.
			; ESI = Pointer to icon data.
			; ECX = Number of pixel rows.
			mov	ebx,[remap]
			xor	eax,eax
xrowloopc:
			push	ecx
			mov	ecx,[iwidth]

xcolumnloopc:
			lodsb
			xlatb
			or	al,al
			jz	short xskip1c		; Transparency skip check.
			mov	[edi],al
xskip1c:
			inc	edi
			loop	xcolumnloopc

			pop	ecx
			add	edi,[modulo]
 		add esi,[skip]
			loop	xrowloopc
			jmp	short proc_out


			;************************************************************
			; Check to see if transparent or generic draw is necessary.
istranscheck2:
			mov	ebx,[IsTrans]
			add	ebx,[icon]
			cmp	[BYTE PTR ebx],0
			jne	short rowloopc

			;************************************************************
			; Fast non-transparent icon draw routine.
			; ES:DI = Pointer to icon destination in page.
			; DS:SI = Pointer to icon data.
			; CX = Number of pixel rows.
			mov	ebx,ecx
			mov	edx,[modulo]
			mov	eax,[iwidth]

			;
			; Optimise copy by dword aligning the destination
			;
loop1c:
			push	eax
 		//rept 3					// No rept in inline asm. ST - 12/20/2018 10:43AM
			test	edi,3
			jz	aligned
			movsb
			dec	eax
			jz	finishedit

			test	edi,3
			jz	aligned
			movsb
			dec	eax
			jz	finishedit

			test	edi,3
			jz	aligned
			movsb
			dec	eax
			jz	finishedit

 		//endm
aligned:
			mov	ecx,eax
			shr	ecx,2
			rep	movsd
			mov	ecx,eax
			and	ecx,3
			rep	movsb

finishedit:
			add	edi,edx
			add	esi,[skip]
			pop	eax

			dec	ebx
			jnz	loop1c
			jmp	short proc_out

			;************************************************************
			; Transparent icon draw routine -- no extended remap.
			; ES:DI = Pointer to icon destination in page.
			; DS:SI = Pointer to icon data.
			; CX = Number of pixel rows.
rowloopc:
			push	ecx
			mov	ecx,[iwidth]

columnloopc:
			lodsb
			or	al,al
			jz	short skip1c		; Transparency check.
			mov	[edi],al
skip1c:
			inc	edi
			loop	columnloopc

			pop	ecx
			add	edi,[modulo]
 		add esi,[skip]
			loop	rowloopc

			; Cleanup and exit icon drawing routine.
proc_out:
			popad
			//ret
	}
}















	 VOID __cdecl Buffer_Draw_Line(void *thisptr, int sx, int sy, int dx, int dy, unsigned char color);
	 VOID __cdecl Buffer_Fill_Rect(void *thisptr, int sx, int sy, int dx, int dy, unsigned char color);
	 VOID __cdecl Buffer_Remap(void * thisptr, int sx, int sy, int width, int height, void *remap);
	 VOID __cdecl Buffer_Fill_Quad(void * thisptr, VOID *span_buff, int x0, int y0, int x1, int y1,
							 	int x2, int y2, int x3, int y3, int color);
	 void __cdecl Buffer_Draw_Stamp(void const *thisptr, void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap);
	 void __cdecl Buffer_Draw_Stamp_Clip(void const *thisptr, void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap, int ,int,int,int);
	 void * __cdecl Get_Font_Palette_Ptr ( void );


/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood 32 bit Library                  *
;*                                                                         *
;*                    File Name : REMAP.ASM                                *
;*                                                                         *
;*                   Programmer : Phil W. Gorrow                           *
;*                                                                         *
;*                   Start Date : July 1, 1994                             *
;*                                                                         *
;*                  Last Update : July 1, 1994   [PWG]                     *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
*/


VOID __cdecl Buffer_Remap(void * this_object, int sx, int sy, int width, int height, void *remap)
{
/*
	PROC	Buffer_Remap C NEAR
	USES	eax,ebx,ecx,edx,esi,edi

	;*===================================================================
	;* Define the arguements that our function takes.
	;*===================================================================
	ARG	this_object:DWORD
	ARG	x0_pixel:DWORD
	ARG	y0_pixel:DWORD
	ARG	region_width:DWORD
	ARG	region_height:DWORD
	ARG	remap	:DWORD

	;*===================================================================
	; Define some locals so that we can handle things quickly
	;*===================================================================
	local	x1_pixel  : DWORD
	local	y1_pixel  : DWORD
	local	win_width : dword
	local	counter_x : dword
*/

	unsigned int x0_pixel = (unsigned int) sx;
	unsigned int y0_pixel = (unsigned int) sy;
	unsigned int region_width = (unsigned int) width;
	unsigned int region_height = (unsigned int) height;

	unsigned int x1_pixel = 0;
	unsigned int y1_pixel = 0;
	unsigned int win_width = 0;
	unsigned int counter_x = 0;

	__asm {
		
		cmp	[ remap ] , 0
		jz		real_out

	; Clip Source Rectangle against source Window boundaries.
		mov  	esi , [ this_object ]	    ; get ptr to src
		xor 	ecx , ecx
		xor 	edx , edx
		mov	edi , [esi]GraphicViewPortClass.Width  ; get width into register
		mov	ebx , [ x0_pixel ]
		mov	eax , [ x0_pixel ]
		add	ebx , [ region_width ]
		shld	ecx , eax , 1
		mov	[ x1_pixel ] , ebx
		inc	edi
		shld	edx , ebx , 1
		sub	eax , edi
		sub	ebx , edi
		shld	ecx , eax , 1
		shld	edx , ebx , 1

		mov	edi,[esi]GraphicViewPortClass.Height ; get height into register
		mov	ebx , [ y0_pixel ]
		mov	eax , [ y0_pixel ]
		add	ebx , [ region_height ]
		shld	ecx , eax , 1
		mov	[ y1_pixel ] , ebx
		inc	edi
		shld	edx , ebx , 1
		sub	eax , edi
		sub	ebx , edi
		shld	ecx , eax , 1
		shld	edx , ebx , 1

		xor	cl , 5
		xor	dl , 5
		mov	al , cl
		test	dl , cl
		jnz	real_out
		or	al , dl
		jz		do_remap

		test	cl , 1000b
		jz		scr_left_ok
		mov	[ x0_pixel ] , 0

scr_left_ok:
		test	cl , 0010b
		jz		scr_bottom_ok
		mov	[ y0_pixel ] , 0

scr_bottom_ok:
		test	dl , 0100b
		jz		scr_right_ok
		mov	eax , [esi]GraphicViewPortClass.Width  ; get width into register
		mov	[ x1_pixel ] , eax
scr_right_ok:
		test	dl , 0001b
		jz		do_remap
		mov	eax , [esi]GraphicViewPortClass.Height  ; get width into register
		mov	[ y1_pixel ] , eax


do_remap:
      	 cld
      	 mov	edi , [esi]GraphicViewPortClass.Offset
      	 mov	eax , [esi]GraphicViewPortClass.XAdd
      	 mov	ebx , [ x1_pixel ]
      	 add	eax , [esi]GraphicViewPortClass.Width
      	 add	eax , [esi]GraphicViewPortClass.Pitch
      	 mov	esi , eax
      	 mul	[ y0_pixel ]
      	 add	edi , [ x0_pixel ]
      	 sub	ebx , [ x0_pixel ]
      	 jle	real_out
      	 add	edi , eax
      	 sub	esi , ebx

      	 mov	ecx , [ y1_pixel ]
      	 sub	ecx , [ y0_pixel ]
      	 jle	real_out
      	 mov	eax , [ remap ]
      	 mov	[ counter_x ] , ebx
      	 xor	edx , edx

outer_loop:
      	 mov	ebx , [ counter_x ]
inner_loop:
      	 mov	dl , [ edi ]
      	 mov	dl , [ eax + edx ]
      	 mov	[ edi ] , dl
      	 inc	edi
      	 dec	ebx
      	 jnz	inner_loop
      	 add	edi , esi
      	 dec	ecx
      	 jnz	outer_loop




real_out:
//		ret
	}
}















/*
; **************************************************************************
; **   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   *
; **************************************************************************
; *                                                                        *
; *                 Project Name : WSA Support routines			   *
; *                                                                        *
; *                    File Name : XORDELTA.ASM                            *
; *                                                                        *
; *                   Programmer : Scott K. Bowen			   *
; *                                                                        *
; *                  Last Update :May 23, 1994   [SKB]                     *
; *                                                                        *
; *------------------------------------------------------------------------*
; * Functions:                                                             *
;*   Apply_XOR_Delta -- Apply XOR delta data to a buffer.                  *
;*   Apply_XOR_Delta_To_Page_Or_Viewport -- Calls the copy or the XOR funti*
;*   Copy_Delta_buffer -- Copies XOR Delta Data to a section of a page.    *
;*   XOR_Delta_Buffer -- Xor's the data in a XOR Delta format to a page.   *
; * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*

IDEAL
P386
MODEL USE32 FLAT
*/


/*
LOCALS ??

; These are used to call Apply_XOR_Delta_To_Page_Or_Viewport() to setup flags parameter.  If
; These change, make sure and change their values in wsa.cpp.
DO_XOR		equ	0
DO_COPY		equ	1
TO_VIEWPORT	equ	0
TO_PAGE		equ	2

;
; Routines defined in this module
;
;
; UWORD Apply_XOR_Delta(UWORD page_seg, BYTE *delta_ptr);
; PUBLIC Apply_XOR_Delta_To_Page_Or_Viewport(UWORD page_seg, BYTE *delta_ptr, WORD width, WORD copy)
;
;	PROC	C XOR_Delta_Buffer
;	PROC	C Copy_Delta_Buffer
;

GLOBAL 	C Apply_XOR_Delta:NEAR
GLOBAL 	C Apply_XOR_Delta_To_Page_Or_Viewport:NEAR
*/

#define DO_XOR			0
#define DO_COPY		1
#define TO_VIEWPORT	0
#define TO_PAGE		2

void __cdecl XOR_Delta_Buffer(int nextrow);
void __cdecl Copy_Delta_Buffer(int nextrow);


/*
;***************************************************************************
;* APPLY_XOR_DELTA -- Apply XOR delta data to a linear buffer.             *
;*   AN example of this in C is at the botton of the file commented out.   *
;*                                                                         *
;* INPUT:  BYTE *target - destination buffer.                              *
;*         BYTE *delta - xor data to be delta uncompress.                  *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* WARNINGS:                                                               *
;*                                                                         *
;* HISTORY:                                                                *
;*   05/23/1994 SKB : Created.                                             *
;*=========================================================================*
*/
unsigned int __cdecl Apply_XOR_Delta(char *target, char *delta)
{
/* 
PROC	Apply_XOR_Delta C near
	USES 	ebx,ecx,edx,edi,esi
	ARG	target:DWORD 		; pointers.
	ARG	delta:DWORD		; pointers.
*/
	
	__asm {
		
			; Optimized for 486/pentium by rearanging instructions.
			mov	edi,[target]		; get our pointers into offset registers.
			mov	esi,[delta]

			cld				; make sure we go forward
			xor	ecx,ecx			; use cx for loop

		top_loop:
			xor	eax,eax			; clear out eax.
			mov	al,[esi]		; get delta source byte
			inc	esi

			test	al,al			; check for a SHORTDUMP ; check al incase of sign value.
			je	short_run
			js	check_others

		;
		; SHORTDUMP
		;
			mov	ecx,eax			; stick count in cx

		dump_loop:
			mov	al,[esi]		;get delta XOR byte
			xor	[edi],al		; xor that byte on the dest
			inc	esi
			inc	edi
			dec	ecx
			jnz	dump_loop
			jmp	top_loop

		;
		; SHORTRUN
		;

		short_run:
			mov	cl,[esi]		; get count
			inc	esi			; inc delta source

		do_run:
			mov	al,[esi]		; get XOR byte
			inc	esi

		run_loop:
			xor	[edi],al		; xor that byte.

			inc	edi			; go to next dest pixel
			dec	ecx			; one less to go.
			jnz	run_loop
			jmp	top_loop

		;
		; By now, we know it must be a LONGDUMP, SHORTSKIP, LONGRUN, or LONGSKIP
		;

		check_others:
			sub	eax,080h		; opcode -= 0x80
			jnz	do_skip		; if zero then get next word, otherwise use remainder.

			mov	ax,[esi]
			lea	esi,[esi+2]		; get word code in ax
			test	ax,ax			; set flags. (not 32bit register so neg flag works)
			jle	not_long_skip

		;
		; SHORTSKIP AND LONGSKIP
		;
		do_skip:
			add	edi,eax			; do the skip.
			jmp	top_loop


		not_long_skip:
			jz	stop			; long count of zero means stop
			sub	eax,08000h     		; opcode -= 0x8000
			test	eax,04000h		; is it a LONGRUN (code & 0x4000)?
			je	long_dump

		;
		; LONGRUN
		;
			sub	eax,04000h		; opcode -= 0x4000
			mov	ecx,eax			; use cx as loop count
			jmp	do_run		; jump to run code.


		;
		; LONGDUMP
		;

		long_dump:
			mov	ecx,eax			; use cx as loop count
			jmp	dump_loop		; go to the dump loop.

		stop:
	}
}


/*
;----------------------------------------------------------------------------

;***************************************************************************
;* APPLY_XOR_DELTA_To_Page_Or_Viewport -- Calls the copy or the XOR funtion.           *
;*                                                                         *
;*									   *
;* 	This funtion is call to either xor or copy XOR_Delta data onto a   *
;*	page instead of a buffer.  The routine will set up the registers   *
;*	need for the actual routines that will perform the copy or xor.	   *
;*									   *
;*	The registers are setup as follows :				   *
;*		es:edi - destination segment:offset onto page.		   *
;*		ds:esi - source buffer segment:offset of delta data.	   *
;*		dx,cx,ax - are all zeroed out before entry.		   *
;*                                                                         *
;* INPUT:                                                                  *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* WARNINGS:                                                               *
;*                                                                         *
;* HISTORY:                                                                *
;*   03/09/1992  SB : Created.                                             *
;*=========================================================================*
*/

void __cdecl Apply_XOR_Delta_To_Page_Or_Viewport(void *target, void *delta, int width, int nextrow, int copy)
{
	/*
	USES 	ebx,ecx,edx,edi,esi
	ARG	target:DWORD		; pointer to the destination buffer.
	ARG	delta:DWORD		; pointer to the delta buffer.
	ARG	width:DWORD		; width of animation.
	ARG	nextrow:DWORD		; Page/Buffer width - anim width.
	ARG	copy:DWORD		; should it be copied or xor'd?
	*/
	
	__asm {

		mov	edi,[target]		; Get the target pointer.
		mov	esi,[delta]		; Get the destination pointer.

		xor	eax,eax			; clear eax, later put them into ecx and edx.

		cld				; make sure we go forward

		mov	ebx,[nextrow]		; get the amount to add to get to next row from end.  push it later...

		mov	ecx,eax			; use cx for loop
		mov	edx,eax			; use dx to count the relative column.

		push	ebx			; push nextrow onto the stack for Copy/XOR_Delta_Buffer.
		mov	ebx,[width]		; bx will hold the max column for speed compares

	; At this point, all the registers have been set up.  Now call the correct function
	; to either copy or xor the data.

		cmp	[copy],DO_XOR		; Do we want to copy or XOR
		je	xorfunct		; Jump to XOR if not copy
		call	Copy_Delta_Buffer	; Call the function to copy the delta buffer.
		jmp	didcopy		; jump past XOR
	xorfunct:
		call	XOR_Delta_Buffer	; Call funtion to XOR the deltat buffer.
	didcopy:
		pop	ebx			; remove the push done to pass a value.
	}
}


/*
;----------------------------------------------------------------------------


;***************************************************************************
;* XOR_DELTA_BUFFER -- Xor's the data in a XOR Delta format to a page.     *
;*	This will only work right if the page has the previous data on it. *
;*	This function should only be called by XOR_Delta_Buffer_To_Page_Or_Viewport.   *
;*      The registers must be setup as follows :                           *
;*                                                                         *
;* INPUT:                                                                  *
;*	es:edi - destination segment:offset onto page.		 	   *
;*	ds:esi - source buffer segment:offset of delta data.	 	   *
;*	edx,ecx,eax - are all zeroed out before entry.		 	   *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* WARNINGS:                                                               *
;*                                                                         *
;* HISTORY:                                                                *
;*   03/09/1992  SB : Created.                                             *
;*=========================================================================*
*/
void __cdecl XOR_Delta_Buffer(int nextrow)
{
	/*		  
	ARG	nextrow:DWORD
	*/
	
	__asm {

		top_loop:
			xor	eax,eax			; clear out eax.
			mov	al,[esi]		; get delta source byte
			inc	esi

			test	al,al			; check for a SHORTDUMP ; check al incase of sign value.
			je	short_run
			js	check_others

		;
		; SHORTDUMP
		;
			mov	ecx,eax			; stick count in cx

		dump_loop:
			mov	al,[esi]		; get delta XOR byte
			xor	[edi],al		; xor that byte on the dest
			inc	esi
			inc	edx			; increment our count on current column
			inc	edi
			cmp	edx,ebx			; are we at the final column
			jne	end_col1		; if not the jmp over the code

			sub	edi,edx			; get our column back to the beginning.
			xor	edx,edx			; zero out our column counter
			add	edi,[nextrow]		; jump to start of next row
		end_col1:

			dec	ecx
			jnz	dump_loop
			jmp	top_loop

		;
		; SHORTRUN
		;

		short_run:
			mov	cl,[esi]		; get count
			inc	esi			; inc delta source

		do_run:
			mov	al,[esi]		; get XOR byte
			inc	esi

		run_loop:
			xor	[edi],al		; xor that byte.

			inc	edx			; increment our count on current column
			inc	edi			; go to next dest pixel
			cmp	edx,ebx			; are we at the final column
			jne	end_col2		; if not the jmp over the code

			sub	edi,ebx			; get our column back to the beginning.
			xor	edx,edx			; zero out our column counter
			add	edi,[nextrow]		; jump to start of next row
		end_col2:


			dec	ecx
			jnz	run_loop
			jmp	top_loop

		;
		; By now, we know it must be a LONGDUMP, SHORTSKIP, LONGRUN, or LONGSKIP
		;

		check_others:
			sub	eax,080h		; opcode -= 0x80
			jnz	do_skip		; if zero then get next word, otherwise use remainder.

			mov	ax,[esi]		; get word code in ax
			lea	esi,[esi+2]
			test	ax,ax			; set flags. (not 32bit register so neg flag works)
			jle	not_long_skip

		;
		; SHORTSKIP AND LONGSKIP
		;
		do_skip:
			sub	edi,edx			; go back to beginning or row.
			add	edx,eax			; incriment our count on current row
		recheck3:
			cmp	edx,ebx			; are we past the end of the row
			jb	end_col3  		; if not the jmp over the code

			sub	edx,ebx			; Subtract width from the col counter
			add	edi,[nextrow]  		; jump to start of next row
			jmp	recheck3		; jump up to see if we are at the right row
		end_col3:
			add	edi,edx			; get to correct position in row.
			jmp	top_loop


		not_long_skip:
			jz	stop			; long count of zero means stop
			sub	eax,08000h     		; opcode -= 0x8000
			test	eax,04000h		; is it a LONGRUN (code & 0x4000)?
			je	long_dump

		;
		; LONGRUN
		;
			sub	eax,04000h		; opcode -= 0x4000
			mov	ecx,eax			; use cx as loop count
			jmp	do_run		; jump to run code.


		;
		; LONGDUMP
		;

		long_dump:
			mov	ecx,eax			; use cx as loop count
			jmp	dump_loop		; go to the dump loop.

		stop:

	}
}


/*
;----------------------------------------------------------------------------


;***************************************************************************
;* COPY_DELTA_BUFFER -- Copies XOR Delta Data to a section of a page.      *
;*	This function should only be called by XOR_Delta_Buffer_To_Page_Or_Viewport.   *
;*      The registers must be setup as follows :                           *
;*                                                                         *
;* INPUT:                                                                  *
;*	es:edi - destination segment:offset onto page.		 	   *
;*	ds:esi - source buffer segment:offset of delta data.	 	   *
;*	dx,cx,ax - are all zeroed out before entry.		 	   *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* WARNINGS:                                                               *
;*                                                                         *
;* HISTORY:                                                                *
;*   03/09/1992  SB : Created.                                             *
;*=========================================================================*
*/
void __cdecl Copy_Delta_Buffer(int nextrow)
{
	/*		  
	ARG	nextrow:DWORD
	*/
	
	__asm {
		
		top_loop:
			xor	eax,eax			; clear out eax.
			mov	al,[esi]		; get delta source byte
			inc	esi

			test	al,al			; check for a SHORTDUMP ; check al incase of sign value.
			je	short_run
			js	check_others

		;
		; SHORTDUMP
		;
			mov	ecx,eax			; stick count in cx

		dump_loop:
			mov	al,[esi]		; get delta XOR byte

			mov	[edi],al		; store that byte on the dest

			inc	edx			; increment our count on current column
			inc	esi
			inc	edi
			cmp	edx,ebx			; are we at the final column
			jne	end_col1		; if not the jmp over the code

			sub	edi,edx			; get our column back to the beginning.
			xor	edx,edx			; zero out our column counter
			add	edi,[nextrow]		; jump to start of next row
		end_col1:

			dec	ecx
			jnz	dump_loop
			jmp	top_loop

		;
		; SHORTRUN
		;

		short_run:
			mov	cl,[esi]		; get count
			inc	esi			; inc delta source

		do_run:
			mov	al,[esi]		; get XOR byte
			inc	esi

		run_loop:
			mov	[edi],al		; store the byte (instead of XOR against current color)

			inc	edx			; increment our count on current column
			inc	edi			; go to next dest pixel
			cmp	edx,ebx			; are we at the final column
			jne	end_col2		; if not the jmp over the code

			sub	edi,ebx			; get our column back to the beginning.
			xor	edx,edx			; zero out our column counter
			add	edi,[nextrow]		; jump to start of next row
		end_col2:


			dec	ecx
			jnz	run_loop
			jmp	top_loop

		;
		; By now, we know it must be a LONGDUMP, SHORTSKIP, LONGRUN, or LONGSKIP
		;

		check_others:
			sub	eax,080h		; opcode -= 0x80
			jnz	do_skip		; if zero then get next word, otherwise use remainder.

			mov	ax,[esi]		; get word code in ax
			lea	esi,[esi+2]
			test	ax,ax			; set flags. (not 32bit register so neg flag works)
			jle	not_long_skip

		;
		; SHORTSKIP AND LONGSKIP
		;
		do_skip:
			sub	edi,edx			; go back to beginning or row.
			add	edx,eax			; incriment our count on current row
		recheck3:
			cmp	edx,ebx			; are we past the end of the row
			jb	end_col3  		; if not the jmp over the code

			sub	edx,ebx			; Subtract width from the col counter
			add	edi,[nextrow]  		; jump to start of next row
			jmp	recheck3		; jump up to see if we are at the right row
		end_col3:
			add	edi,edx			; get to correct position in row.
			jmp	top_loop


		not_long_skip:
			jz	stop			; long count of zero means stop
			sub	eax,08000h     		; opcode -= 0x8000
			test	eax,04000h		; is it a LONGRUN (code & 0x4000)?
			je	long_dump

		;
		; LONGRUN
		;
			sub	eax,04000h		; opcode -= 0x4000
			mov	ecx,eax			; use cx as loop count
			jmp	do_run		; jump to run code.


		;
		; LONGDUMP
		;

		long_dump:
			mov	ecx,eax			; use cx as loop count
			jmp	dump_loop		; go to the dump loop.

		stop:

	}
}
/*
;----------------------------------------------------------------------------
*/






















/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood Library                         *
;*                                                                         *
;*                    File Name : FADING.ASM                               *
;*                                                                         *
;*                   Programmer : Joe L. Bostic                            *
;*                                                                         *
;*                   Start Date : August 20, 1993                          *
;*                                                                         *
;*                  Last Update : August 20, 1993   [JLB]                  *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

GLOBAL	C Build_Fading_Table	:NEAR

	CODESEG

;***********************************************************
; BUILD_FADING_TABLE
;
; void *Build_Fading_Table(void *palette, void *dest, long int color, long int frac);
;
; This routine will create the fading effect table used to coerce colors
; from toward a common value.  This table is used when Fading_Effect is
; active.
;
; Bounds Checking: None
;*
*/
void * __cdecl Build_Fading_Table(void const *palette, void const *dest, long int color, long int frac)
{
	/*
	PROC	Build_Fading_Table C near
	USES	ebx, ecx, edi, esi
	ARG	palette:DWORD
	ARG	dest:DWORD
	ARG	color:DWORD
	ARG	frac:DWORD

	LOCAL	matchvalue:DWORD	; Last recorded match value.
	LOCAL	targetred:BYTE		; Target gun red.
	LOCAL	targetgreen:BYTE	; Target gun green.
	LOCAL	targetblue:BYTE		; Target gun blue.
	LOCAL	idealred:BYTE
	LOCAL	idealgreen:BYTE
	LOCAL	idealblue:BYTE
	LOCAL	matchcolor:BYTE		; Tentative match color.
	*/
	
	int matchvalue = 0;	//:DWORD	; Last recorded match value.
	unsigned char targetred = 0;		//BYTE		; Target gun red.
	unsigned char targetgreen = 0;	//BYTE		; Target gun green.
	unsigned char targetblue = 0;		//BYTE		; Target gun blue.
	unsigned char idealred = 0;		//BYTE	
	unsigned char idealgreen = 0;		//BYTE	
	unsigned char idealblue = 0;		//BYTE	
	unsigned char matchcolor = 0;		//:BYTE		; Tentative match color.
	
	__asm {
		cld

		; If the source palette is NULL, then just return with current fading table pointer.
		cmp	[palette],0
		je	fini
		cmp	[dest],0
		je	fini

		; Fractions above 255 become 255.
		mov	eax,[frac]
		cmp	eax,0100h
		jb	short ok
		mov	[frac],0FFh
	ok:

		; Record the target gun values.
		mov	esi,[palette]
		mov	ebx,[color]
		add	esi,ebx
		add	esi,ebx
		add	esi,ebx
		lodsb
		mov	[targetred],al
		lodsb
		mov	[targetgreen],al
		lodsb
		mov	[targetblue],al

		; Main loop.
		xor	ebx,ebx			; Remap table index.

		; Transparent black never gets remapped.
		mov	edi,[dest]
		mov	[edi],bl
		inc	edi

		; EBX = source palette logical number (1..255).
		; EDI = running pointer into dest remap table.
	mainloop:
		inc	ebx
		mov	esi,[palette]
		add	esi,ebx
		add	esi,ebx
		add	esi,ebx

		mov	edx,[frac]
		shr	edx,1
		; new = orig - ((orig-target) * fraction);

		lodsb				; orig
		mov	dh,al			; preserve it for later.
		sub	al,[targetred]		; al = (orig-target)
		imul	dl			; ax = (orig-target)*fraction
		shl	ax,1
		sub	dh,ah			; dh = orig - ((orig-target) * fraction)
		mov	[idealred],dh		; preserve ideal color gun value.

		lodsb				; orig
		mov	dh,al			; preserve it for later.
		sub	al,[targetgreen]	; al = (orig-target)
		imul	dl			; ax = (orig-target)*fraction
		shl	ax,1
		sub	dh,ah			; dh = orig - ((orig-target) * fraction)
		mov	[idealgreen],dh		; preserve ideal color gun value.

		lodsb				; orig
		mov	dh,al			; preserve it for later.
		sub	al,[targetblue]		; al = (orig-target)
		imul	dl			; ax = (orig-target)*fraction
		shl	ax,1
		sub	dh,ah			; dh = orig - ((orig-target) * fraction)
		mov	[idealblue],dh		; preserve ideal color gun value.

		; Sweep through the entire existing palette to find the closest
		; matching color.  Never matches with color 0.

		mov	eax,[color]
		mov	[matchcolor],al		; Default color (self).
		mov	[matchvalue],-1		; Ridiculous match value init.
		mov	ecx,255

		mov	esi,[palette]		; Pointer to original palette.
		add	esi,3

		; BH = color index.
		mov	bh,1
	innerloop:

		; Recursion through the fading table won't work if a color is allowed
		; to remap to itself.  Prevent this from occuring.
		add	esi,3
		cmp	bh,bl
		je	short notclose
		sub	esi,3

		xor	edx,edx			; Comparison value starts null.
		mov	eax,edx
		; Build the comparison value based on the sum of the differences of the color
		; guns squared.
		lodsb
		sub	al,[idealred]
		mov	ah,al
		imul	ah
		add	edx,eax

		lodsb
		sub	al,[idealgreen]
		mov	ah,al
		imul	ah
		add	edx,eax

		lodsb
		sub	al,[idealblue]
		mov	ah,al
		imul	ah
		add	edx,eax
		jz	short perfect		; If perfect match found then quit early.

		cmp	edx,[matchvalue]
		ja	short notclose
		mov	[matchvalue],edx	; Record new possible color.
		mov	[matchcolor],bh
	notclose:
		inc	bh			; Checking color index.
		loop	innerloop
		mov	bh,[matchcolor]
	perfect:
		mov	[matchcolor],bh
		xor	bh,bh			; Make BX valid main index again.

		; When the loop exits, we have found the closest match.
		mov	al,[matchcolor]
		stosb
		cmp	ebx,255
		jne	mainloop

	fini:
		mov	eax,[dest]
//			ret


	}
}
























/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood Library                         *
;*                                                                         *
;*                    File Name : PAL.ASM                                  *
;*                                                                         *
;*                   Programmer : Joe L. Bostic                            *
;*                                                                         *
;*                   Start Date : May 30, 1992                             *
;*                                                                         *
;*                  Last Update : April 27, 1994   [BR]                    *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Set_Palette_Range -- Sets changed values in the palette.              *
;*   Bump_Color -- adjusts specified color in specified palette            *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
;********************** Model & Processor Directives ************************
IDEAL
P386
MODEL USE32 FLAT


;include "keyboard.inc"
FALSE = 0
TRUE  = 1

;****************************** Declarations ********************************
GLOBAL 		C Set_Palette_Range:NEAR
GLOBAL 		C Bump_Color:NEAR
GLOBAL  	C CurrentPalette:BYTE:768
GLOBAL		C PaletteTable:byte:1024


;********************************** Data ************************************
LOCALS ??

	DATASEG

CurrentPalette	DB	768 DUP(255)	; copy of current values of DAC regs
PaletteTable	DB	1024 DUP(0)

IFNDEF LIB_EXTERNS_RESOLVED
VertBlank	DW	0		; !!!! this should go away
ENDIF


;********************************** Code ************************************
	CODESEG
*/

extern "C" unsigned char CurrentPalette[768] = {255};	//	DB	768 DUP(255)	; copy of current values of DAC regs
extern "C" unsigned char PaletteTable[1024] = {0};		//	DB	1024 DUP(0)


/*
;***************************************************************************
;* SET_PALETTE_RANGE -- Sets a palette range to the new pal                *
;*                                                                         *
;* INPUT:                                                                  *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* PROTO:                                                                  *
;*                                                                         *
;* WARNINGS:	This routine is optimized for changing a small number of   *
;*		colors in the palette.
;*                                                                         *
;* HISTORY:                                                                *
;*   03/07/1995 PWG : Created.                                             *
;*=========================================================================*
*/
void __cdecl Set_Palette_Range(void *palette)
{
	memcpy(CurrentPalette, palette, 768);
	Set_DD_Palette(palette);

	/*
	PROC	Set_Palette_Range C NEAR
	ARG	palette:DWORD

	GLOBAL	Set_DD_Palette_:near
	GLOBAL	Wait_Vert_Blank_:near
	
	pushad
	mov	esi,[palette]
	mov	ecx,768/4
	mov	edi,offset CurrentPalette
	cld
	rep	movsd
	;call	Wait_Vert_Blank_
	mov	eax,[palette]
	push	eax
	call	Set_DD_Palette_
	pop	eax
	popad
	ret
	*/
}


/*
;***************************************************************************
;* Bump_Color -- adjusts specified color in specified palette              *
;*                                                                         *
;* INPUT:                                                                  *
;*	VOID *palette	- palette to modify				   *
;*	WORD changable	- color # to change				   *
;*	WORD target	- color to bend toward				   *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* WARNINGS:                                                               *
;*                                                                         *
;* HISTORY:                                                                *
;*   04/27/1994 BR : Converted to 32-bit.                                  *
;*=========================================================================*
; BOOL cdecl Bump_Color(VOID *palette, WORD changable, WORD target);
*/ 
BOOL __cdecl Bump_Color(void *pal, int color, int desired)
{
	/*		  
PROC Bump_Color C NEAR
	USES ebx,ecx,edi,esi
	ARG	pal:DWORD, color:WORD, desired:WORD
	LOCAL	changed:WORD		; Has palette changed?
	 */ 
	
	short short_color = (short) color;
	short short_desired = (short) desired;
	bool changed = false;
	
	__asm {
		mov	edi,[pal]		; Original palette pointer.
		mov	esi,edi
		mov	eax,0
		mov	ax,[short_color]
		add	edi,eax
		add	edi,eax
		add	edi,eax			; Offset to changable color.
		mov	ax,[short_desired]
		add	esi,eax
		add	esi,eax
		add	esi,eax			; Offset to target color.

		mov	[changed],FALSE		; Presume no change.
		mov	ecx,3			; Three color guns.

		; Check the color gun.
	colorloop:
		mov	al,[BYTE PTR esi]
		sub	al,[BYTE PTR edi]	; Carry flag is set if subtraction needed.
		jz	short gotit
		mov	[changed],TRUE
		inc	[BYTE PTR edi]		; Presume addition.
		jnc	short gotit		; oops, subtraction needed so dec twice.
		dec	[BYTE PTR edi]
		dec	[BYTE PTR edi]
	gotit:
		inc	edi
		inc	esi
		loop	colorloop

		movzx	eax,[changed]
	}
}















/*
;***************************************************************************
;**     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
;***************************************************************************
;*                                                                         *
;*                 Project Name : GraphicViewPortClass			   *
;*                                                                         *
;*                    File Name : PUTPIXEL.ASM                             *
;*                                                                         *
;*                   Programmer : Phil Gorrow				   *
;*                                                                         *
;*                   Start Date : June 7, 1994				   *
;*                                                                         *
;*                  Last Update : June 8, 1994   [PWG]                     *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   VVPC::Put_Pixel -- Puts a pixel on a virtual viewport                 *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

INCLUDE ".\drawbuff.inc"
INCLUDE ".\gbuffer.inc"


CODESEG
*/

/*
;***************************************************************************
;* VVPC::PUT_PIXEL -- Puts a pixel on a virtual viewport                   *
;*                                                                         *
;* INPUT:	WORD the x position for the pixel relative to the upper    *
;*			left corner of the viewport			   *
;*		WORD the y pos for the pixel relative to the upper left	   *
;*			corner of the viewport				   *
;*		UBYTE the color of the pixel to write			   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* WARNING:	If pixel is to be placed outside of the viewport then	   *
;*		this routine will abort.				   *
;*									   *
;* HISTORY:                                                                *
;*   06/08/1994 PWG : Created.                                             *
;*=========================================================================*
	PROC	Buffer_Put_Pixel C near
	USES	eax,ebx,ecx,edx,edi
*/

void __cdecl Buffer_Put_Pixel(void * this_object, int x_pixel, int y_pixel, unsigned char color)
{
			  
	/*
	ARG    	this_object:DWORD				; this is a member function
	ARG	x_pixel:DWORD				; x position of pixel to set
	ARG	y_pixel:DWORD				; y position of pixel to set
	ARG    	color:BYTE				; what color should we clear to
	*/
	
	__asm {
		
	
			;*===================================================================
			; Get the viewport information and put bytes per row in ecx
			;*===================================================================
			mov	ebx,[this_object]				; get a pointer to viewport
			xor	eax,eax
			mov	edi,[ebx]GraphicViewPortClass.Offset	; get the correct offset
			mov	ecx,[ebx]GraphicViewPortClass.Height	; edx = height of viewport
			mov	edx,[ebx]GraphicViewPortClass.Width	; ecx = width of viewport

			;*===================================================================
			; Verify that the X pixel offset if legal
			;*===================================================================
			mov	eax,[x_pixel]				; find the x position
			cmp	eax,edx					;   is it out of bounds
			jae	short done				; if so then get out
			add	edi,eax					; otherwise add in offset

			;*===================================================================
			; Verify that the Y pixel offset if legal
			;*===================================================================
			mov	eax,[y_pixel]				; get the y position
			cmp	eax,ecx					;  is it out of bounds
			jae	done					; if so then get out
			add	edx,[ebx]GraphicViewPortClass.XAdd	; otherwise find bytes per row
			add	edx,[ebx]GraphicViewPortClass.Pitch	; add in direct draw pitch
			mul	edx					; offset = bytes per row * y
			add	edi,eax					; add it into the offset

			;*===================================================================
			; Write the pixel to the screen
			;*===================================================================
			mov	al,[color]				; read in color value
			mov	[edi],al				; write it to the screen
		done:
	}
}








/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Support Library                          *
;*                                                                         *
;*                    File Name : cliprect.asm                             *
;*                                                                         *
;*                   Programmer : Julio R Jerez                            *
;*                                                                         *
;*                   Start Date : Mar, 2 1995                              *
;*                                                                         *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* int Clip_Rect ( int * x , int * y , int * dw , int * dh , 		   *
;*	       	   int width , int height ) ;          			   *
;* int Confine_Rect ( int * x , int * y , int * dw , int * dh , 	   *
;*	       	      int width , int height ) ;          		   *
;*									   *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *


IDEAL
P386
MODEL USE32 FLAT

GLOBAL	 C Clip_Rect	:NEAR
GLOBAL	 C Confine_Rect	:NEAR

CODESEG

;***************************************************************************
;* Clip_Rect -- clip a given rectangle against a given window		   *
;*                                                                         *
;* INPUT:   &x , &y , &w , &h  -> Pointer to rectangle being clipped       *
;*          width , height     -> dimension of clipping window             *
;*                                                                         *
;* OUTPUT: a) Zero if the rectangle is totally contained by the 	   *
;*	      clipping window.						   *
;*	   b) A negative value if the rectangle is totally outside the     *
;*            the clipping window					   *
;*	   c) A positive value if the rectangle	was clipped against the	   *
;*	      clipping window, also the values pointed by x, y, w, h will  *
;*	      be modified to new clipped values	 			   *
;*									   *
;*   05/03/1995 JRJ : added comment                                        *
;*=========================================================================*
; int Clip_Rect (int* x, int* y, int* dw, int* dh, int width, int height);          			   *
*/

extern "C" int __cdecl Clip_Rect ( int * x , int * y , int * w , int * h , int width , int height )
{		

/*
	PROC	Clip_Rect C near
	uses	ebx,ecx,edx,esi,edi
	arg	x:dword
	arg	y:dword
	arg	w:dword
	arg	h:dword
	arg	width:dword
	arg	height:dword
*/

	__asm {

		;This Clipping algorithm is a derivation of the very well known
		;Cohen-Sutherland Line-Clipping test. Due to its simplicity and efficiency
		;it is probably the most commontly implemented algorithm both in software
		;and hardware for clipping lines, rectangles, and convex polygons against
		;a rectagular clipping window. For reference see
		;"COMPUTER GRAPHICS principles and practice by Foley, Vandam, Feiner, Hughes
		; pages 113 to 177".
		; Briefly consist in computing the Sutherland code for both end point of
		; the rectangle to find out if the rectangle is:
		; - trivially accepted (no further clipping test, return the oroginal data)
		; - trivially rejected (return with no action, return error code)
		; - retangle must be iteratively clipped again edges of the clipping window
		;   and return the clipped rectangle

			; get all four pointer into regisnters
			mov	esi,[x]		; esi = pointer to x
			mov	edi,[y]		; edi = pointer to x
			mov	eax,[w]		; eax = pointer to dw
			mov	ebx,[h]		; ebx = pointer to dh

			; load the actual data into reg
			mov	esi,[esi]	; esi = x0
			mov	edi,[edi]	; edi = y0
			mov	eax,[eax]	; eax = dw
			mov	ebx,[ebx]	; ebx = dh

			; create a wire frame of the type [x0,y0] , [x1,y1]
			add	eax,esi		; eax = x1 = x0 + dw
			add	ebx,edi		; ebx = y1 = y0 + dh

			; we start we suthenland code0 and code1 set to zero
			xor 	ecx,ecx		; cl = sutherland boolean code0
			xor 	edx,edx		; dl = sutherland boolean code0

			; now we start computing the to suthenland boolean code for x0 , x1
			shld	ecx,esi,1	; bit3 of code0 = sign bit of (x0 - 0)
			shld	edx,eax,1 	; bit3 of code1 = sign bit of (x1 - 0)
			sub	esi,[width]	; get the difference (x0 - (width + 1))
			sub	eax,[width]	; get the difference (x1 - (width + 1))
			dec	esi
			dec	eax
			shld	ecx,esi,1	; bit2 of code0 = sign bit of (x0 - (width + 1))
			shld	edx,eax,1	; bit2 of code1 = sign bit of (x0 - (width + 1))

			; now we start computing the to suthenland boolean code for y0 , y1
			shld	ecx,edi,1   	; bit1 of code0 = sign bit of (y0 - 0)
			shld	edx,ebx,1	; bit1 of code1 = sign bit of (y0 - 0)
			sub	edi,[height]	; get the difference (y0 - (height + 1))
			sub	ebx,[height]	; get the difference (y1 - (height + 1))
			dec	edi
			dec	ebx
			shld	ecx,edi,1	; bit0 of code0 = sign bit of (y0 - (height + 1))
			shld	edx,ebx,1	; bit0 of code1 = sign bit of (y1 - (height + 1))

			; Bit 2 and 0 of cl and bl are complemented
			xor	cl,5		; reverse bit2 and bit0 in code0
			xor	dl,5 		; reverse bit2 and bit0 in code1

			; now perform the rejection test
			mov	eax,-1		; set return code to false
			mov	bl,cl 		; save code0 for future use
			test	dl,cl  		; if any two pair of bit in code0 and code1 is set
			jnz	clip_out	; then rectangle is outside the window

			; now perform the aceptance test
			xor	eax,eax		; set return code to true
			or	bl,dl		; if all pair of bits in code0 and code1 are reset
			jz	clip_out	; then rectangle is insize the window.								      '

			; we need to clip the rectangle iteratively
			mov	eax,-1		; set return code to false
			test	cl,1000b	; if bit3 of code0 is set then the rectangle
			jz	left_ok	; spill out the left edge of the window
			mov	edi,[x]		; edi = a pointer to x0
			mov	ebx,[w]		; ebx = a pointer to dw
			mov	esi,[edi]	; esi = x0
			mov	[dword ptr edi],0 ; set x0 to 0 "this the left edge value"
			add	[ebx],esi	; adjust dw by x0, since x0 must be negative

		left_ok:
			test	cl,0010b	; if bit1 of code0 is set then the rectangle
			jz	bottom_ok	; spill out the bottom edge of the window
			mov	edi,[y]		; edi = a pointer to y0
			mov	ebx,[h]		; ebx = a pointer to dh
			mov	esi,[edi]	; esi = y0
			mov	[dword ptr edi],0 ; set y0 to 0 "this the bottom edge value"
			add	[ebx],esi	; adjust dh by y0, since y0 must be negative

		bottom_ok:
			test	dl,0100b	; if bit2 of code1 is set then the rectangle
			jz	right_ok	; spill out the right edge of the window
			mov	edi,[w] 	; edi = a pointer to dw
			mov	esi,[x]		; esi = a pointer to x
			mov	ebx,[width]	; ebx = the width of the window
			sub	ebx,[esi] 	; the new dw is the difference (width-x0)
			mov	[edi],ebx	; adjust dw to (width - x0)
			jle	clip_out	; if (width-x0) = 0 then the clipped retangle
						; has no width we are done
		right_ok:
			test	dl,0001b	; if bit0 of code1 is set then the rectangle
			jz	clip_ok	; spill out the top edge of the window
			mov	edi,[h] 	; edi = a pointer to dh
			mov	esi,[y]		; esi = a pointer to y0
			mov	ebx,[height]	; ebx = the height of the window
			sub	ebx,[esi] 	; the new dh is the difference (height-y0)
			mov	[edi],ebx	; adjust dh to (height-y0)
			jle	clip_out	; if (width-x0) = 0 then the clipped retangle
						; has no width we are done
		clip_ok:
			mov	eax,1  	; signal the calling program that the rectangle was modify
		clip_out:
		//ret
	}

	//ENDP	Clip_Rect
}

/*
;***************************************************************************
;* Confine_Rect -- clip a given rectangle against a given window	   *
;*                                                                         *
;* INPUT:   &x,&y,w,h    -> Pointer to rectangle being clipped       *
;*          width,height     -> dimension of clipping window             *
;*                                                                         *
;* OUTPUT: a) Zero if the rectangle is totally contained by the 	   *
;*	      clipping window.						   *
;*	   c) A positive value if the rectangle	was shifted in position    *
;*	      to fix inside the clipping window, also the values pointed   *
;*	      by x, y, will adjusted to a new values	 		   *
;*									   *
;*  NOTE:  this function make not attempt to verify if the rectangle is	   *
;*	   bigger than the clipping window and at the same time wrap around*
;*	   it. If that is the case the result is meaningless		   *
;*=========================================================================*
; int Confine_Rect (int* x, int* y, int dw, int dh, int width, int height);          			   *
*/

extern "C" int __cdecl Confine_Rect ( int * x , int * y , int w , int h , int width , int height )
{
	
/*
	PROC	Confine_Rect C near
	uses	ebx, esi,edi
	arg	x:dword
	arg	y:dword
	arg	w:dword
	arg	h:dword
	arg	width :dword
	arg	height:dword
*/
	__asm {		  
	
			xor	eax,eax
			mov	ebx,[x]
			mov	edi,[w]

			mov	esi,[ebx]
			add	edi,[ebx]

			sub	edi,[width]
			neg	esi
			dec	edi

			test	esi,edi
			jl	x_axix_ok
			mov	eax,1

			test	esi,esi
			jl	shift_right
			mov	[dword ptr ebx],0
			jmp	x_axix_ok
		shift_right:
			inc	edi
			sub	[ebx],edi
		x_axix_ok:
			mov	ebx,[y]
			mov	edi,[h]

			mov	esi,[ebx]
			add	edi,[ebx]

			sub	edi,[height]
			neg	esi
			dec	edi

			test	esi,edi
			jl	confi_out
			mov	eax,1

			test	esi,esi
			jl	shift_top
			mov	[dword ptr ebx],0

			//ret
			jmp confi_out
		shift_top:
			inc	edi
			sub	[ebx],edi
		confi_out:
			//ret

			//ENDP	Confine_Rect
	}
}









/*
; $Header: //depot/Projects/Mobius/QA/Project/Run/SOURCECODE/REDALERT/WIN32LIB/DrawMisc.cpp#33 $
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Library routine                          *
;*                                                                         *
;*                    File Name : UNCOMP.ASM                               *
;*                                                                         *
;*                   Programmer : Christopher Yates                        *
;*                                                                         *
;*                  Last Update : 20 August, 1990   [CY]                   *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*                                                                         *
; ULONG LCW_Uncompress(BYTE *source, BYTE *dest, ULONG length);		   *
;*                                                                         *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

GLOBAL            C LCW_Uncompress          :NEAR

CODESEG

; ----------------------------------------------------------------
;
; Here are prototypes for the routines defined within this module:
;
; ULONG LCW_Uncompress(BYTE *source, BYTE *dest, ULONG length);
;
; ----------------------------------------------------------------
*/
#if (0)//ST 5/10/2019
extern "C" unsigned long __cdecl LCW_Uncompress(void *source, void *dest, unsigned long length_)
{
//PROC	LCW_Uncompress C near
//
//	USES ebx,ecx,edx,edi,esi
//
//	ARG	source:DWORD
//	ARG	dest:DWORD
//	ARG	length:DWORD
//;LOCALS
//	LOCAL a1stdest:DWORD
//	LOCAL maxlen:DWORD
//	LOCAL lastbyte:DWORD
//	LOCAL lastcom:DWORD
//	LOCAL lastcom1:DWORD
		
	unsigned long a1stdest;
	unsigned long  maxlen;
	unsigned long lastbyte;
	//unsigned long lastcom;
	//unsigned long lastcom1;

	__asm {


		mov	edi,[dest]
		mov	esi,[source]
		mov	edx,[length_]

	;
	;
	; uncompress data to the following codes in the format b = byte, w = word
	; n = byte code pulled from compressed data
	;   Bit field of n		command		description
	; n=0xxxyyyy,yyyyyyyy		short run	back y bytes and run x+3
	; n=10xxxxxx,n1,n2,...,nx+1	med length	copy the next x+1 bytes
	; n=11xxxxxx,w1			med run		run x+3 bytes from offset w1
	; n=11111111,w1,w2		long copy	copy w1 bytes from offset w2
	; n=11111110,w1,b1		long run	run byte b1 for w1 bytes
	; n=10000000			end		end of data reached
	;

		mov	[a1stdest],edi
		add	edx,edi
		mov	[lastbyte],edx
		cld			; make sure all lod and sto are forward
		mov	ebx,esi		; save the source offset

	loop_label:
		mov	eax,[lastbyte]
		sub	eax,edi		; get the remaining byte to uncomp
		jz	short out_label		; were done

		mov	[maxlen],eax	; save for string commands
		mov	esi,ebx		; mov in the source index

		xor	eax,eax
		mov	al,[esi]
		inc	esi
		test	al,al		; see if its a short run
		js	short notshort

		mov	ecx,eax		;put count nibble in cl

		mov	ah,al		; put rel offset high nibble in ah
		and	ah,0Fh		; only 4 bits count

		shr	cl,4		; get run -3
		add	ecx,3		; get actual run length

		cmp	ecx,[maxlen]	; is it too big to fit?
		jbe	short rsok		; if not, its ok

		mov	ecx,[maxlen]	; if so, max it out so it dosen't overrun

	rsok:
		mov	al,[esi]	; get rel offset low byte
		lea	ebx,[esi+1]	; save the source offset
		mov	esi,edi		; get the current dest
		sub	esi,eax		; get relative offset

		rep	movsb

		jmp	loop_label

	notshort:
		test	al,40h		; is it a length?
		jne	short notlength	; if not it could be med or long run

		cmp	al,80h		; is it the end?
		je	short out_label		; if so its over

		mov	cl,al		; put the byte in count register
		and	ecx,3Fh		; and off the extra bits

		cmp	ecx,[maxlen]	; is it too big to fit?
		jbe	short lenok		; if not, its ok

		mov	ecx,[maxlen]	; if so, max it out so it dosen't overrun

	lenok:
		rep movsb

		mov	ebx,esi		; save the source offset
		jmp	loop_label

	out_label:
	      	mov	eax,edi
		sub	eax,[a1stdest]
		jmp	label_exit

	notlength:
		mov	cl,al		; get the entire code
		and	ecx,3Fh		; and off all but the size -3
		add	ecx,3		; add 3 for byte count

		cmp	al,0FEh
		jne	short notrunlength

		xor	ecx,ecx
		mov	cx,[esi]

		xor	eax,eax
		mov	al,[esi+2]
		lea	ebx,[esi+3]	;save the source offset

		cmp	ecx,[maxlen]	; is it too big to fit?
		jbe	short runlenok		; if not, its ok

		mov	ecx,[maxlen]	; if so, max it out so it dosen't overrun

	runlenok:
		test	ecx,0ffe0h
		jnz	dont_use_stosb
		rep	stosb
		jmp	loop_label


	dont_use_stosb:
		mov	ah,al
		mov	edx,eax
		shl	eax,16
		or	eax,edx

		test	edi,3
		jz	aligned

		mov	[edi],eax
		mov	edx,edi
		and	edi,0fffffffch
		lea	edi,[edi+4]
		and	edx,3
		dec	dl
		xor	dl,3
		sub	ecx,edx

	aligned:
		mov	edx,ecx
		shr	ecx,2
		rep	stosd

		and	edx,3
		jz	loop_label
		mov	ecx,edx
		rep	stosb
		jmp	loop_label






	notrunlength:
		cmp	al,0FFh		; is it a long run?
		jne	short notlong	; if not use the code as the size

		xor     ecx,ecx
		xor	eax,eax
		mov	cx,[esi]	; if so, get the size
		lea	esi,[esi+2]

	notlong:
		mov	ax,[esi]	;get the real index
		add	eax,[a1stdest]	;add in the 1st index
		lea	ebx,[esi+2]	;save the source offset
		cmp	ecx,[maxlen]	;compare for overrun
		mov	esi,eax		;use eax as new source
		jbe	short runok	; if not, its ok

		mov	ecx,[maxlen]	; if so, max it out so it dosen't overrun

	runok:
		test	ecx,0ffe0h
		jnz	dont_use_movsb
		rep	movsb
		jmp	loop_label




	dont_use_movsb:
		lea	edx,[edi+0fffffffch]
		cmp	esi,edx
		ja	use_movsb

		test	edi,3
		jz	aligned2

		mov	eax,[esi]
		mov	[edi],eax
		mov	edx,edi
		and	edi,0fffffffch
		lea	edi,[edi+4]
		and	edx,3
		dec	dl
		xor	dl,3
		sub	ecx,edx
		add	esi,edx

	aligned2:
		mov	edx,ecx
		shr	ecx,2
		and	edx,3
		rep	movsd
		mov	ecx,edx
	use_movsb:
		rep	movsb
		jmp	loop_label




	label_exit:
		mov	eax,edi
		mov	ebx,[dest]
		sub	eax,ebx

		//ret

	}
}
#endif













/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood 32 bit Library                  *
;*                                                                         *
;*                    File Name : TOPAGE.ASM                               *
;*                                                                         *
;*                   Programmer : Phil W. Gorrow                           *
;*                                                                         *
;*                   Start Date : June 8, 1994                             *
;*                                                                         *
;*                  Last Update : June 15, 1994   [PWG]                    *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Buffer_To_Page -- Copies a linear buffer to a virtual viewport	   *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

TRANSP	equ  0


INCLUDE ".\drawbuff.inc"
INCLUDE ".\gbuffer.inc"

CODESEG

;***************************************************************************
;* VVC::TOPAGE -- Copies a linear buffer to a virtual viewport		   *
;*                                                                         *
;* INPUT:	WORD	x_pixel		- x pixel on viewport to copy from *
;*		WORD	y_pixel 	- y pixel on viewport to copy from *
;*		WORD	pixel_width	- the width of copy region	   *
;*		WORD	pixel_height	- the height of copy region	   *
;*		BYTE *	src		- buffer to copy from		   *
;*		VVPC *  dest		- virtual viewport to copy to	   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* WARNINGS:    Coordinates and dimensions will be adjusted if they exceed *
;*	        the boundaries.  In the event that no adjustment is 	   *
;*	        possible this routine will abort.  If the size of the 	   *
;*		region to copy exceeds the size passed in for the buffer   *
;*		the routine will automatically abort.			   *
;*									   *
;* HISTORY:                                                                *
;*   06/15/1994 PWG : Created.                                             *
;*=========================================================================*
 */ 

extern "C" long __cdecl Buffer_To_Page(int x_pixel, int y_pixel, int pixel_width, int pixel_height, void *src, void *dest)
{

/*
	PROC	Buffer_To_Page C near
	USES	eax,ebx,ecx,edx,esi,edi

	;*===================================================================
	;* define the arguements that our function takes.
	;*===================================================================
	ARG	x_pixel     :DWORD		; x pixel position in source
	ARG	y_pixel     :DWORD		; y pixel position in source
	ARG	pixel_width :DWORD		; width of rectangle to blit
	ARG	pixel_height:DWORD		; height of rectangle to blit
	ARG    	src         :DWORD		; this is a member function
	ARG	dest        :DWORD		; what are we blitting to

;	ARG	trans       :DWORD			; do we deal with transparents?

	;*===================================================================
	; Define some locals so that we can handle things quickly
	;*===================================================================
	LOCAL 	x1_pixel :dword
	LOCAL	y1_pixel :dword
	local	scr_x 	: dword
	local	scr_y 	: dword
	LOCAL	dest_ajust_width:DWORD
	LOCAL	scr_ajust_width:DWORD
	LOCAL	dest_area   :  dword
*/

	unsigned long x1_pixel;
	unsigned long y1_pixel;
	unsigned long scr_x;
	unsigned long scr_y;
	unsigned long dest_ajust_width;
	unsigned long scr_ajust_width;
	//unsigned long dest_area;

	__asm {
	
			cmp	[ src ] , 0
			jz	real_out


		; Clip dest Rectangle against source Window boundaries.

			mov	[ scr_x ] , 0
			mov	[ scr_y ] , 0
			mov  	esi , [ dest ]	    ; get ptr to dest
			xor 	ecx , ecx
			xor 	edx , edx
			mov	edi , [esi]GraphicViewPortClass.Width  ; get width into register
			mov	ebx , [ x_pixel ]
			mov	eax , [ x_pixel ]
			add	ebx , [ pixel_width ]
			shld	ecx , eax , 1
			mov	[ x1_pixel ] , ebx
			inc	edi
			shld	edx , ebx , 1
			sub	eax , edi
			sub	ebx , edi
			shld	ecx , eax , 1
			shld	edx , ebx , 1

			mov	edi, [esi]GraphicViewPortClass.Height ; get height into register
			mov	ebx , [ y_pixel ]
			mov	eax , [ y_pixel ]
			add	ebx , [ pixel_height ]
			shld	ecx , eax , 1
			mov	[ y1_pixel ] , ebx
			inc	edi
			shld	edx , ebx , 1
			sub	eax , edi
			sub	ebx , edi
			shld	ecx , eax , 1
			shld	edx , ebx , 1

			xor	cl , 5
			xor	dl , 5
			mov	al , cl
			test	dl , cl
			jnz	real_out
			or	al , dl
			jz	do_blit

			test	cl , 1000b
			jz	dest_left_ok
			mov	eax , [ x_pixel ]
			neg	eax
			mov	[ x_pixel ] , 0
			mov	[ scr_x ] , eax

		dest_left_ok:
			test	cl , 0010b
			jz	dest_bottom_ok
			mov	eax , [ y_pixel ]
			neg	eax
			mov	[ y_pixel ] , 0
			mov	[ scr_y ] , eax

		dest_bottom_ok:
			test	dl , 0100b
			jz	dest_right_ok
			mov	eax , [esi]GraphicViewPortClass.Width  ; get width into register
			mov	[ x1_pixel ] , eax
		dest_right_ok:
			test	dl , 0001b
			jz	do_blit
			mov	eax , [esi]GraphicViewPortClass.Height  ; get width into register
			mov	[ y1_pixel ] , eax

		do_blit:

   		    cld

   		    mov	eax , [esi]GraphicViewPortClass.XAdd
   		    add	eax , [esi]GraphicViewPortClass.Width
   		    add	eax , [esi]GraphicViewPortClass.Pitch
   		    mov	edi , [esi]GraphicViewPortClass.Offset

   		    mov	ecx , eax
   		    mul	[ y_pixel ]
   		    add	edi , [ x_pixel ]
   		    add	edi , eax

   		    add	ecx , [ x_pixel ]
   		    sub	ecx , [ x1_pixel ]
   		    mov	[ dest_ajust_width ] , ecx


   		    mov	esi , [ src ]
   		    mov	eax , [ pixel_width ]
   		    sub	eax , [ x1_pixel ]
   		    add	eax , [ x_pixel ]
   		    mov	[ scr_ajust_width ] , eax

   		    mov	eax , [ scr_y ]
   		    mul 	[ pixel_width ]
   		    add	eax , [ scr_x ]
   		    add	esi , eax

   		    mov	edx , [ y1_pixel ]
   		    mov	eax , [ x1_pixel ]

   		    sub	edx , [ y_pixel ]
   		    jle	real_out
   		    sub	eax , [ x_pixel ]
   		    jle	real_out


		; ********************************************************************
		; Forward bitblit only

		//IF TRANSP
   	//	    test	[ trans ] , 1
   	//	    jnz	forward_Blit_trans
		//ENDIF


		; the inner loop is so efficient that
		; the optimal consept no longer apply because
		; the optimal byte have to by a number greather than 9 bytes
   		    cmp	eax , 10
   		    jl	forward_loop_bytes

		forward_loop_dword:
   		    mov	ecx , edi
   		    mov	ebx , eax
   		    neg	ecx
   		    and	ecx , 3
   		    sub	ebx , ecx
   		    rep	movsb
   		    mov	ecx , ebx
   		    shr	ecx , 2
   		    rep	movsd
   		    mov	ecx , ebx
   		    and	ecx , 3
   		    rep	movsb
   		    add	esi , [ scr_ajust_width ]
   		    add	edi , [ dest_ajust_width ]
   		    dec	edx
   		    jnz	forward_loop_dword
   		    jmp	real_out	//ret

		forward_loop_bytes:
   		    mov	ecx , eax
   		    rep	movsb
   		    add	esi , [ scr_ajust_width ]
   		    add	edi , [ dest_ajust_width ]
   		    dec	edx					; decrement the height
   		    jnz	forward_loop_bytes
   		  //  ret

		//IF  TRANSP
		//
		//
		//forward_Blit_trans:
		//
		//
   	//	    mov	ecx , eax
   	//	    and	ecx , 01fh
   	//	    lea	ecx , [ ecx + ecx * 4 ]
   	//	    neg	ecx
   	//	    shr	eax , 5
   	//	    lea	ecx , [ transp_reference + ecx * 2 ]
   	//	    mov	[ y1_pixel ] , ecx
		//
		//
		//forward_loop_trans:
   	//	    mov	ecx , eax
   	//	    jmp	[ y1_pixel ]
		//forward_trans_line:
   	//	    REPT	32
   	//	    local	transp_pixel
   	//	    		mov	bl , [ esi ]
   	//	    		inc	esi
   	//	    		test	bl , bl
   	//	    		jz	transp_pixel
   	//	    		mov	[ edi ] , bl
   	//	 	    transp_pixel:
   	//	    		inc	edi
		//	ENDM
   	//	 transp_reference:
   	//	    dec	ecx
   	//	    jge	forward_trans_line
   	//	    add	esi , [ scr_ajust_width ]
   	//	    add	edi , [ dest_ajust_width ]
   	//	    dec	edx
   	//	    jnz	forward_loop_trans
   	//	    ret
		//ENDIF

		real_out:
   		    //ret
	}
}

			//ENDP	Buffer_To_Page
		//END









/*

;***************************************************************************
;* VVPC::GET_PIXEL -- Gets a pixel from the current view port		   *
;*                                                                         *
;* INPUT:	WORD the x pixel on the screen.				   *
;*		WORD the y pixel on the screen.				   *
;*                                                                         *
;* OUTPUT:      UBYTE the pixel at the specified location		   *
;*                                                                         *
;* WARNING:	If pixel is to be placed outside of the viewport then	   *
;*		this routine will abort.				   *
;*                                                                         *
;* HISTORY:                                                                *
;*   06/07/1994 PWG : Created.                                             *
;*=========================================================================*
	PROC	Buffer_Get_Pixel C near
	USES	ebx,ecx,edx,edi

	ARG    	this_object:DWORD				; this is a member function
	ARG	x_pixel:DWORD				; x position of pixel to set
	ARG	y_pixel:DWORD				; y position of pixel to set
*/

extern "C" int __cdecl Buffer_Get_Pixel(void * this_object, int x_pixel, int y_pixel)
{
	__asm {		  

		;*===================================================================
		; Get the viewport information and put bytes per row in ecx
		;*===================================================================
		mov	ebx,[this_object]				; get a pointer to viewport
		xor	eax,eax
		mov	edi,[ebx]GraphicViewPortClass.Offset	; get the correct offset
		mov	ecx,[ebx]GraphicViewPortClass.Height	; edx = height of viewport
		mov	edx,[ebx]GraphicViewPortClass.Width	; ecx = width of viewport

		;*===================================================================
		; Verify that the X pixel offset if legal
		;*===================================================================
		mov	eax,[x_pixel]				; find the x position
		cmp	eax,edx					;   is it out of bounds
		jae	short exit_label				; if so then get out
		add	edi,eax					; otherwise add in offset

		;*===================================================================
		; Verify that the Y pixel offset if legal
		;*===================================================================
		mov	eax,[y_pixel]				; get the y position
		cmp	eax,ecx					;  is it out of bounds
		jae	exit_label					; if so then get out
		add	edx,[ebx]GraphicViewPortClass.XAdd	; otherwise find bytes per row
		add	edx,[ebx]GraphicViewPortClass.Pitch	; otherwise find bytes per row
		mul	edx					; offset = bytes per row * y
		add	edi,eax					; add it into the offset

		;*===================================================================
		; Write the pixel to the screen
		;*===================================================================
		xor	eax,eax					; clear the word
		mov	al,[edi]				; read in the pixel
	exit_label:
		//ret
		//ENDP	Buffer_Get_Pixel

	}
}


