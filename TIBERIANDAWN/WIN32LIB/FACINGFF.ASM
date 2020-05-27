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
;*                 Project Name : Support Library                          *
;*                                                                         *
;*                    File Name : FACINGFF.ASM                             *
;*                                                                         *
;*                   Programmer : Joe L. Bostic                            *
;*                                                                         *
;*                   Start Date : May 8, 1991                              *
;*                                                                         *
;*                  Last Update : February 6, 1995  [BWG]                  *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Desired_Facing256 -- Determines facing to reach a position.           *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *


;IDEAL
;P386
;MODEL USE32 FLAT

GLOBAL	C Desired_Facing256	:NEAR
;	INCLUDE	"wwlib.i"
INCLUDE "..\include\gbuffer.inc"

	CODESEG

;***************************************************************************
;* Desired_Facing256 -- Desired facing algorithm 0..255 resolution.        *
;*                                                                         *
;*    This is a desired facing algorithm that has a resolution of 0        *
;*    through 255.                                                         *
;*                                                                         *
;* INPUT:   srcx,srcy   -- Source coordinate.                              *
;*                                                                         *
;*          dstx,dsty   -- Destination coordinate.                         *
;*                                                                         *
;* OUTPUT:  Returns with the desired facing to face the destination        *
;*          coordinate from the position of the source coordinate.  North  *
;*          is 0, East is 64, etc.                                         *
;*                                                                         *
;* WARNINGS:   This routine is slower than the other forms of desired      *
;*             facing calculation.  Use this routine when accuracy is      *
;*             required.                                                   *
;*                                                                         *
;* HISTORY:                                                                *
;*   12/24/1991 JLB : Adapted.                                             *
;*=========================================================================*/
; LONG cdecl Desired_Facing256(LONG srcx, LONG srcy, LONG dstx, LONG dsty)
	PROC	Desired_Facing256 C near
	USES	ebx, ecx, edx

	ARG	srcx:DWORD
	ARG	srcy:DWORD
	ARG	dstx:DWORD
	ARG	dsty:DWORD

	xor	ebx,ebx			; Facing number.

	; Determine absolute X delta and left/right direction.
	mov	ecx,[dstx]
	sub	ecx,[srcx]
	jge	short ??xnotneg
	neg	ecx
	mov	ebx,11000000b		; Set bit 7 and 6 for leftward.
??xnotneg:

	; Determine absolute Y delta and top/bottom direction.
	mov	eax,[srcy]
	sub	eax,[dsty]
	jge	short ??ynotneg
	xor	ebx,01000000b		; Complement bit 6 for downward.
	neg	eax
??ynotneg:

	; Set DX=64 for quadrants 0 and 2.
	mov	edx,ebx
	and	edx,01000000b
	xor	edx,01000000b

	; Determine if the direction is closer to the Y axis and make sure that
	; CX holds the larger of the two deltas.  This is in preparation for the
	; divide.
	cmp	eax,ecx
	jb	short ??gotaxis
	xchg	eax,ecx
	xor	edx,01000000b		; Closer to Y axis so make DX=64 for quad 0 and 2.
??gotaxis:

	; If closer to the X axis then add 64 for quadrants 0 and 2.  If
	; closer to the Y axis then add 64 for quadrants 1 and 3.  Determined
	; add value is in DX and save on stack.
	push	edx

	; Make sure that the division won't overflow.  Reduce precision until
	; the larger number is less than 256 if it appears that an overflow
	; will occur.  If the high byte of the divisor is not zero, then this
	; guarantees no overflow, so just abort shift operation.
	test	eax,0FFFFFF00h
	jnz	short ??nooverflow
??again:
	test	ecx,0FFFFFF00h
	jz	short ??nooverflow
	shr	ecx,1
	shr	eax,1
	jmp	short ??again
??nooverflow:

	; Make sure that the division won't underflow (divide by zero).  If
	; this would occur, then set the quotient to $FF and skip divide.
	or	ecx,ecx
	jnz	short ??nounderflow
	mov	eax,0FFFFFFFFh
	jmp	short ??divcomplete

	; Derive a pseudo angle number for the octant.  The angle is based
	; on $00 = angle matches long axis, $00 = angle matches $FF degrees.
??nounderflow:
	xor	edx,edx
	shld	edx,eax,8	; shift high byte of eax into dl
	shl	eax,8
	div	ecx
??divcomplete:

	; Integrate the 5 most significant bits into the angle index.  If DX
	; is not zero, then it is 64.  This means that the dividend must be negated
	; before it is added into the final angle value.
	shr	eax,3
	pop	edx
	or	edx,edx
	je	short ??noneg
	dec	edx
	neg	eax
??noneg:
	add	eax,edx
	add	eax,ebx
	and	eax,0FFH
	ret

	ENDP	Desired_Facing256



	END