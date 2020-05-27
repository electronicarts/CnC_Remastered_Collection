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
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood 32 bit Library                  *
;*                                                                         *
;*                    File Name : TOBUFFER.ASM                             *
;*                                                                         *
;*                   Programmer : Phil W. Gorrow                           *
;*                                                                         *
;*                   Start Date : June 8, 1994                             *
;*                  Last Update : Feb 10, 1995   [jrj]                     *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   VVC::TOBUFFER -- Copies a virtual viewport to a linear buffer         *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

;IDEAL
;P386
;MODEL USE32 FLAT
;LOCALS ??
.model flat


TRANSP equ 0


;INCLUDE "drawbuff.inc"
INCLUDE gbuffer.inc


;CODESEG
.code


;***************************************************************************
;* VIVC::TOBUFFER -- Copies a virtual viewport to a linear buffer          *
;*                                                                         *
;* INPUT:	BYTE *	dest		- buffer to copy to		   *
;*			size		- size of the buffer to copy to	   *
;*			x_pixel		- x pixel on viewport to copy from *
;*			y_pixel 	- y pixel on viewport to copy from *
;*			pixel_width	- the width of copy region	   *
;*			pixel_height	- the height of copy region	   *
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
Buffer_To_Buffer proc C public USES ebx ecx edx esi edi this_object:DWORD, x_pixel:DWORD, y_pixel:DWORD, pixel_width:DWORD, pixel_height:DWORD, dest:DWORD, buffer_size:DWORD

	;PROC	Buffer_To_Buffer C near
	;USES	ebx,ecx,edx,esi,edi

	;*===================================================================
	;* define the arguements that our function takes.
	;*===================================================================
	;ARG	this_object:DWORD		; this is a class member function
	;ARG	x_pixel:DWORD		; Page X pixel coordinate.
	;ARG	y_pixel:DWORD		; Page Y pixel coordinate.
	;ARG	pixel_width:DWORD	; Width of region in pixels.
	;ARG	pixel_height:DWORD	; Height of region in pixels.
	;ARG	dest:DWORD		; the buffer to copy to
	;ARG	buffer_size:DWORD	; the size of the buffer

	;*===================================================================
	; Define some locals so that we can handle things quickly
	;*===================================================================
	LOCAL 	x1_pixel :dword
	LOCAL	y1_pixel :dword
	LOCAL	dest_x1 : dword
	LOCAL	dest_y1 : dword
	LOCAL	dest_ajust_width:DWORD
	LOCAL	scr_ajust_width:DWORD
	;LOCAL	dest_area   :  dword

; Clip dest Rectangle against source Window boundaries.

	mov	[ dest_x1 ] , 0
	mov	[ dest_y1 ] , 0

	mov  	esi , [ this_object ]	    ; get ptr to dest
	xor 	ecx , ecx
	xor 	edx , edx
	mov	edi , [esi].GraphicViewPort.GVPWidth  ; get width into register
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

	mov	edi,[esi].GraphicViewPort.GVPHeight ; get height into register
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
	jz	scr_left_ok
	mov	eax , [ x_pixel ]
	neg	eax
	mov	[ x_pixel ] , 0
	mov	[ dest_x1 ] , eax

scr_left_ok:
	test	cl , 0010b
	jz	scr_bottom_ok
 	mov	eax , [ y_pixel ]
	neg	eax
	mov	[ y_pixel ] , 0
	mov	[ dest_y1 ] , eax

scr_bottom_ok:
	test	dl , 0100b
	jz	scr_right_ok
	mov	eax , [esi].GraphicViewPort.GVPWidth  ; get width into register
	mov	[ x1_pixel ] , eax
scr_right_ok:
	test	dl , 0001b
	jz	do_blit
	mov	eax , [esi].GraphicViewPort.GVPHeight  ; get width into register
	mov	[ y1_pixel ] , eax

do_blit:

       cld

       mov	eax , [esi].GraphicViewPort.GVPXAdd
       add	eax , [esi].GraphicViewPort.GVPWidth
       add	eax , [esi].GraphicViewPort.GVPPitch
       mov	esi , [esi].GraphicViewPort.GVPOffset

       mov	ecx , eax
       mul	[ y_pixel ]
       add	esi , [ x_pixel ]
       add	esi , eax

       add	ecx , [ x_pixel ]
       sub	ecx , [ x1_pixel ]
       mov	[ scr_ajust_width ] , ecx

       mov	edi , [ dest ]
       mov	eax , [ pixel_width ]
       sub	eax , [ x1_pixel ]
       add	eax , [ x_pixel ]
       mov	[ dest_ajust_width ] , eax

       mov	eax , [ dest_y1 ]
       mul 	[ pixel_width ]
       add	eax , [ dest_x1 ]
       add	edi , eax

       mov	edx , [ y1_pixel ]
       mov	eax , [ x1_pixel ]
       sub	edx , [ y_pixel ]
       jle	real_out
       sub	eax , [ x_pixel ]
       jle	real_out

       mov	ebx , [ pixel_width ]
       imul	ebx , edx
       cmp	ebx , [ buffer_size ]
       jg	real_out


; ********************************************************************
; Forward bitblit only

IF TRANSP
       cmp	[ transp ] , 0
       jnz	forward_Blit_trans
ENDIF

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
       ret

forward_loop_bytes:
       mov	ecx , eax
       rep	movsb
       add	esi , [ scr_ajust_width ]
       add	edi , [ dest_ajust_width ]
       dec	edx					; decrement the height
       jnz	forward_loop_bytes
       ret


IF  TRANSP

forward_Blit_trans:
       mov	ecx , eax
       and	ecx , 01fh
       lea	ecx , [ ecx + ecx * 4 ]
       neg	ecx
       shr	eax , 5
       lea	ecx , [ transp_reference + ecx * 2 ]
       mov	[ y1_pixel ] , ecx

forward_loop_trans:
       mov	ecx , eax
       jmp	[ y1_pixel ]
forward_trans_line:
       REPT	32
       local	transp_pixel
       		mov	bl , [ esi ]
       		test	bl , bl
       		jz	transp_pixel
       		mov	[ edi ] , bl
    	    transp_pixel:
       		inc	esi
	    	inc	edi
	ENDM
    transp_reference:
       dec	ecx
       jge	forward_trans_line
       add	esi , [ scr_ajust_width ]
       add	edi , [ dest_ajust_width ]
       dec	edx
       jnz	forward_loop_trans
       ret
ENDIF

real_out:
       ret

;ENDP	Buffer_To_Buffer
Buffer_To_Buffer endp

END