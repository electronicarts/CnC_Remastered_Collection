;
; Copyright 2020 Electronic Arts Inc.
;
; TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
; software: you can redistribute it and/or modify it under the terms of 
; the GNU General Public License as published by the Free Software Foundation, 
; either version 3 of the License, or (at your option) any later version.

; TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
; in the hope that it will be useful, but with permitted additional restrictions 
; under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
; distributed with this program. You should have received a copy of the 
; GNU General Public License along with permitted additional restrictions 
; with this program. If not, see [https://github.com/electronicarts/CnC_Remastered_Collection]>.


;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S  I N C  **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Command & Conquer                        *
;*                                                                         *
;*                    File Name : WINSAM.ASM                               *
;*                                                                         *
;*                   Programmer : Steve Tall                               *
;*                                                                         *
;*                   Start Date : October 26th, 1995                       *
;*                                                                         *
;*                  Last Update : October 26th, 1995  [ST]                 *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *


IDEAL
P386
MODEL USE32 FLAT

global	C _AbortModemFunctionPtr:dword
global	C Memory_Error_Exit	:dword
global	C MouseQX		:dword
global	C MouseQY		:dword

global	FastGetPortHardware_	:near
global	FastSetPortHardware_	:near
global	PortOpenGreenleafFast_	:near
global	HMWaitForOK_		:near
global	HMSetDialingMethod_	:near
global	HMDial_			:near
global	HMInputLine_		:near
global	HMAnswer_		:near
global	PortKillTime_		:near
global	HMSendStringNoWait_	:near
global	HMSetUpEchoRoutine_	:near
global	HMSetUpAbortKey_	:near
global	SetAbortModemFunctionPtr_:near
global	Change8259Priority_	:near
global	HMSendString_		:near
global	C Stop_Execution	:near


global	_IPX_Initialise:near
global	_ASM_IPX_Initialise:near


		codeseg

proc _ASM_IPX_Initialise near

		int	3
		jmp	_IPX_Initialise

endp




global _Int3:near
proc _Int3 near
	;int	3
	ret
endp


proc Stop_Execution C near

	nop
	ret

endp



;
; Stuff needed from the shape library
;
;
;


INCLUDE "shape.inc"




;***************************************************************************
;* ModeX_Blit -- Copy a 320x200 graphic view port to a modex screen        *
;*                                                                         *
;*                                                                         *
;* INPUT:	eax - graphic view port                                    *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* PROTO:       extern "C" void ModeX_Blit (GraphicBufferClass *source);   *
;*                                                                         *
;* HISTORY:                                                                *
;*   10/26/1994 PWG : Created.                                             *
;*=========================================================================*

SEQUENCER	=3c4h	; sequencer port
MAP_MASK	=2	; map mask register

		INCLUDE "gbuffer.inc"
		global	ModeX_Blit_:near


proc        	ModeX_Blit_ NEAR

		pushad
		mov	ebx,eax

		mov	esi,[(GraphicViewPort ebx).GVPOffset]
		mov	eax,[(GraphicViewPort ebx).GVPXAdd]
		add	eax,[(GraphicViewPort ebx).GVPPitch]
		mov	edi,0a0000h
		mov	ebx,eax

		mov	al,MAP_MASK

		mov	ebp,200

??each_line_lp:	mov	ah,1		;1st plane
		push	ebx
		push	esi

??each_plane_lp:mov	edx,SEQUENCER
		out	dx,ax

		push	esi
		push	edi
		push	eax

		rept	10
		mov	al,[esi]
		mov	bl,[esi+8]
		mov	cl,[esi+16]
		mov	dl,[esi+24]
		mov	ah,[esi+4]
		mov	bh,[esi+12]
		mov	ch,[esi+20]
		mov	dh,[esi+28]
		shl	ebx,16
		shl	edx,16
		or	ebx,eax
		or	edx,ecx
		mov	[edi],ebx
		mov	[edi+4],edx
		add	esi,32
		add	edi,8
		endm

		pop	eax
		pop	edi
		pop	esi
		inc	esi
		shl	ah,1
		cmp	ah,16
		jl	??each_plane_lp


		pop	esi
		pop	ebx
		lea	esi,[esi+ebx+320]
		add	edi,80
		dec	ebp
		jnz	??each_line_lp

		popad

		ret

endp		ModeX_Blit_






ifdef cuts
		pushad
		mov	ebx,eax

		mov	esi,[(GraphicViewPort ebx).GVPOffset]
		mov	eax,[(GraphicViewPort ebx).GVPXAdd]
		add	eax,[(GraphicViewPort ebx).GVPPitch]
		mov	edi,0a0000h
		mov	ebx,eax

		mov	al,MAP_MASK
		mov	ah,1		;1st plane

??each_plane_lp:mov	edx,SEQUENCER
		out	dx,ax
		mov	ebp,200		;do 200 lines
		push	esi
		push	edi

??each_line_lp:	mov	ecx,320/4

??each_pixel_lp:mov	dl,[esi]
		mov	[edi],dl
		add	esi,4
		inc	edi
		dec	ecx
		jnz	??each_pixel_lp

		add	esi,ebx
		dec	ebp
		jnz	??each_line_lp

		pop	edi
		pop	esi
		inc	esi
		shl	ah,1

		cmp	ah,16
		jl	??each_plane_lp
endif










proc FastGetPortHardware_ NEAR
endp

proc FastSetPortHardware_ NEAR
endp

proc PortOpenGreenleafFast_ NEAR
endp

proc HMWaitForOK_ NEAR
endp

proc HMSetDialingMethod_ NEAR
endp

proc HMDial_ NEAR
endp

proc HMInputLine_ NEAR
endp

proc HMAnswer_ NEAR
endp

proc PortKillTime_ NEAR
endp

proc HMSendStringNoWait_ NEAR
endp

proc HMSetUpEchoRoutine_ NEAR
endp

proc HMSetUpAbortKey_ NEAR
endp

proc SetAbortModemFunctionPtr_ NEAR
endp

proc Change8259Priority_ NEAR
endp

proc HMSendString_ NEAR
endp

		ret







	masm
;
; Change a DAC colour register directly
;
; register number in al
;
; bh=red bl=green cl=blue
;

set_dac_col proc near
		pushad
		cli
		push	eax
		mov	dx,03dah
		in	al,dx
		jmp	@@1
@@1:		mov	dx,03c8h
		pop	eax
		out	dx,al
		jmp	@@2
@@2:		inc	dl
		mov	al,bh
		out	dx,al
		jmp	@@3
@@3:		mov	al,bl
		out	dx,al
		jmp	@@4
@@4:		mov	al,cl
		out	dx,al
		jmp	@@5
@@5:		sti
		popad
		ret
set_dac_col endp

	ideal


global	Set_Palette_Register_:near


proc Set_Palette_Register_ near

		pushad
		and	cl,63
		mov	bh,dl
		and	bh,63
		and	bl,63
		call	set_dac_col
		popad
		ret

endp Set_Palette_Register_




		locals	??



		ends

		dataseg

LineBuffer		dd	640 dup (?)
		ends



		segment mycode page public use32 'code'	; Need stricter segment alignment

global		C	Asm_Interpolate:near
global		C	Asm_Interpolate_Line_Double:near
global		C	Asm_Interpolate_Line_Interpolate:near
global		C	PaletteInterpolationTable:byte


;*********************************************************************************************
;* Asm_Interpolate -- interpolate a 320x200 buffer to a 640x400 screen                       *
;*                                                                                           *
;* INPUT:	ptr to source buffer (320x200 image)                                         *
;*              ptr to dest buffer (640x400)                                                 *
;*              height of source buffer                                                      *
;*              width of source buffer                                                        *
;*              width of dest buffer                                                         *
;*                                                                                           *
;*                                                                                           *
;* OUTPUT:      none                                                                         *
;*                                                                                           *
;* Warnings:                                                                                 *
;*                                                                                           *
;* HISTORY:                                                                                  *
;*   12/15/95 ST : Created.                                                                  *
;*===========================================================================================*

PROC		Asm_Interpolate C near

		ARG	src_ptr:dword
		ARG	dest_ptr:dword
		ARG	source_height:dword
		ARG	source_width:dword
		ARG	dest_width:dword

		LOCAL	old_dest:dword

		pushad

		mov	eax,[dest_ptr]
		mov	[old_dest],eax

		mov	esi,[src_ptr]

??each_line_loop:
		mov	ecx,[source_width]
		sub	ecx,2
		shr	ecx,1
		mov	edi,[old_dest]
		jmp	??interpolate_loop

		align	32
;
; convert 2 pixels of source into 4 pixels of destination
; so we can write to video memory with dwords
;
??interpolate_loop:
		mov	eax,[esi]
		lea	esi,[esi+2]
		mov	edx,eax
		mov	ebx,eax
		and	edx,65535
		ror	ebx,8
		mov	bl,[edx+PaletteInterpolationTable]
		mov	bh,ah
		and	eax,000ffff00h
		ror	ebx,8

;1st 3 pixels now in ebx

		shr	eax,8
		mov	bh,[eax+PaletteInterpolationTable]
		ror	ebx,16
		mov	[edi],ebx
		add	edi,4

		dec	ecx
		jnz	??interpolate_loop

; do the last three pixels and a blank on the end of a row
		xor	eax,eax
		mov	ax,[esi]
		mov	[edi],al
		inc	edi
		lea	esi,[esi+2]
		mov	al,[eax+PaletteInterpolationTable]
		mov	[edi],al
		inc	edi
		mov	[edi],ah
		inc	edi
		mov	[byte edi],0

		mov	edi,[dest_width]
		add	[old_dest],edi

		dec	[source_height]
		jnz	??each_line_loop

		popad
		ret

endp		Asm_Interpolate














PROC		Asm_Interpolate_Line_Double C near
		ARG	src_ptr:dword
		ARG	dest_ptr:dword
		ARG	source_height:dword
		ARG	source_width:dword
		ARG	dest_width:dword

		LOCAL	old_dest:dword
		LOCAL	width_counter:dword
		LOCAL	pixel_count:dword

		pushad

		mov	eax,[dest_ptr]
		mov	[old_dest],eax

		mov	esi,[src_ptr]
		mov	edi,[dest_ptr]

??each_line_loop:
		mov	[width_counter],0
		mov	ecx,[source_width]
		sub	ecx,2
		shr	ecx,1
		mov	[pixel_count],ecx
		mov	ecx,offset LineBuffer
		mov	edi,[old_dest]
		jmp	??interpolate_loop
		align	16

; convert 2 pixels of source into 4 pixels of destination
??interpolate_loop:
		mov	eax,[esi]
		lea	esi,[esi+2]
		mov	edx,eax
		mov	ebx,eax
		and	edx,65535
		ror	ebx,8
		mov	bl,[edx+PaletteInterpolationTable]
		mov	bh,ah
		and	eax,000ffff00h
		ror	ebx,8

		;1st 3 pixels now in ebx
		shr	eax,8
		mov	bh,[eax+PaletteInterpolationTable]
		ror	ebx,16
		mov	[edi],ebx
		mov	[ecx],ebx
		add	edi,4
		add	ecx,4

		dec	[pixel_count]
		jnz	??interpolate_loop

; do the last three pixels and a blank

		xor	eax,eax
		mov	ax,[esi]
		mov	[edi],al
		mov	[ecx],al
		inc	edi
		inc	ecx
		lea	esi,[esi+2]
		mov	al,[eax+PaletteInterpolationTable]
		mov	[edi],al
		mov	[ecx],al
		inc	edi
		inc	ecx
		mov	[edi],ah
		mov	[ecx],ah
		inc	edi
		inc	ecx
		mov	[byte edi],0
		mov	[byte ecx],0

		mov	edi,[dest_width]
		shr	edi,1
		add	[old_dest],edi
		push	esi
		push	edi
		mov	esi,offset LineBuffer
		mov	edi,[old_dest]
		mov	ecx,[source_width]
		shr	ecx,1
		rep	movsd
		pop	edi
		pop	esi
		add	[old_dest],edi
		mov	edi,[old_dest]

		dec	[source_height]
		jnz	??each_line_loop

		popad
		ret

endp		Asm_Interpolate_Line_Double









		ends

		dataseg

TopLine		dd	640 dup (?)
BottomLine	dd	640 dup (?)


		segment mycode page public use32 'code'	; Need stricter segment alignment


proc		Interpolate_Single_Line C near

		ARG	source_ptr:dword
		ARG	dest_ptr:dword
		ARG	source_width:dword

		pushad

		mov	ecx,[source_width]
		sub	ecx,2
		shr	ecx,1

		mov	esi,[source_ptr]
		mov	edi,[dest_ptr]

??interpolate_loop:
		mov	eax,[esi]
		lea	esi,[esi+2]
		mov	edx,eax
		mov	ebx,eax
		and	edx,65535
		ror	ebx,8
		mov	bl,[edx+PaletteInterpolationTable]
		mov	bh,ah
		and	eax,000ffff00h
		ror	ebx,8

		;1st 3 pixels now in ebx
		shr	eax,8
		mov	bh,[eax+PaletteInterpolationTable]
		ror	ebx,16
		mov	[edi],ebx
		add	edi,4

		dec	ecx
		jnz	??interpolate_loop

; do the last three pixels and a blank

		xor	eax,eax
		mov	ax,[esi]
		mov	[edi],al
		inc	edi
		mov	al,[eax+PaletteInterpolationTable]
		mov	[edi],al
		inc	edi
		mov	[edi],ah
		inc	edi
		mov	[byte edi],0

		popad
		ret


endp		Interpolate_Single_Line


proc		Interpolate_Between_Lines C near

		ARG	source1:dword
		ARG	source2:dword
		ARG	destination:dword
		ARG	source_width:dword

		pushad
		mov	esi,[source1]
		mov	edi,[destination]
		mov	ebx,[source2]
		xor	eax,eax
		mov	ecx,[source_width]
		add	ecx,ecx

??interpolate_each_pixel_loop:
		mov	al,[esi]
		mov	ah,[ebx]
		inc	esi
		inc	ebx
		mov	dl,[eax+PaletteInterpolationTable]
		mov	[edi],dl
		inc	edi
		dec	ecx
		jnz	??interpolate_each_pixel_loop

		popad
		ret

endp		Interpolate_Between_Lines




macro		Lineswp
		push	[next_line]
		push	[last_line]
		pop	[next_line]
		pop	[last_line]
endm


PROC		Asm_Interpolate_Line_Interpolate C near


		ARG	src_ptr:dword
		ARG	dest_ptr:dword
		ARG	source_lines:dword
		ARG	source_width:dword
		ARG	dest_width:dword

		LOCAL	old_dest:dword
		LOCAL	pixel_count:dword
		LOCAL	next_line:dword
		LOCAL	last_line:dword

		pushad

		mov	eax,[dest_ptr]
		mov	[old_dest],eax

		mov	[next_line],offset TopLine
		mov	[last_line],offset BottomLine
		mov	ecx,[source_width]
		shr	ecx,1
		mov	[pixel_count],ecx
		shr	[dest_width],1

		call	Interpolate_Single_Line C,[src_ptr],[next_line],[source_width]
		mov	esi,[source_width]
		Lineswp
		add	[src_ptr],esi
		dec	[source_lines]


??each_line_pair_loop:
		call	Interpolate_Single_Line C,[src_ptr],[next_line],[source_width]
		call	Interpolate_Between_Lines C,[last_line],[next_line],offset LineBuffer,[source_width]

		mov	esi,[last_line]
		mov	edi,[old_dest]
		mov	ecx,[pixel_count]
		rep	movsd

		mov	edi,[old_dest]
		mov	esi,offset LineBuffer
		add	edi,[dest_width]
		mov	ecx,[pixel_count]
		mov	[old_dest],edi
		rep	movsd

		mov	edi,[old_dest]
		mov	esi,[source_width]
		add	edi,[dest_width]
		add	[src_ptr],esi
		mov	[old_dest],edi

		Lineswp
		dec	[source_lines]
		jnz	??each_line_pair_loop

		call	Interpolate_Single_Line C,[src_ptr],[next_line],[source_width]
		mov	esi,[next_line]
		mov	edi,[old_dest]
		mov	ecx,[pixel_count]
		rep	movsd

		popad
		ret


endp		Asm_Interpolate_Line_Interpolate

		ends	mycode




global		C Asm_Create_Palette_Interpolation_Table:near
global		C InterpolationPalette:dword

		codeseg


proc		Asm_Create_Palette_Interpolation_Table C near

		LOCAL	palette_counter:dword
		LOCAL	first_palette:dword
		LOCAL	second_palette:dword
		LOCAL	dest_ptr:dword
		LOCAL	count:dword
		LOCAL	closest_colour:dword
		LOCAL	distance_of_closest:dword

		pushad

		mov	[dest_ptr],0
		mov	[palette_counter],256
		mov	esi,[InterpolationPalette]

??palette_outer_loop:
		mov	edi,[InterpolationPalette]
		mov	ecx,256

??palette_inner_loop:
		mov	bl,[esi]
		add	bl,[edi]
		shr	bl,1

		mov	bh,[esi+1]
		add	bh,[edi+1]
		shr	bh,1

		mov	dl,[esi+2]
		add	dl,[edi+2]
		shr	dl,1

		mov	[closest_colour],0
		mov	[distance_of_closest],-1

		push	edi
		push	ecx
		mov	edi,[InterpolationPalette]
		mov	[count],0

??cmp_pal_lp:	xor	eax,eax
		xor	ecx,ecx
		mov	al,[edi]
		sub	al,bl
		imul	al
		mov	ecx,eax
		mov	al,[edi+1]
		sub	al,bh
		imul	al
		add	ecx,eax
		mov	al,[edi+2]
		sub	al,dl
		imul	al
		add	ecx,eax

		cmp	ecx,[distance_of_closest]
		ja	??end_cmp_lp
		mov	[distance_of_closest],ecx
		mov	eax,[count]
		mov	[closest_colour],eax
		test	ecx,ecx
		jz	??got_perfect

??end_cmp_lp:	lea	edi,[edi+3]
		inc	[count]
		cmp	[count],256
		jb	??cmp_pal_lp


??got_perfect:	mov	edi,[dest_ptr]
		mov	eax,[closest_colour]
		mov	[edi+PaletteInterpolationTable],al
		inc	[dest_ptr]

		pop	ecx
		pop	edi
		lea	edi,[edi+3]
		dec	ecx
		jnz	??palette_inner_loop

		lea	esi,[esi+3]
		dec	[palette_counter]
		jnz	??palette_outer_loop

		popad
		ret

endp		Asm_Create_Palette_Interpolation_Table




	DATASEG

_AbortModemFunctionPtr	dd	0
Memory_Error_Exit	dd	0
MouseQX			dd	0
MouseQY			dd	0

end
