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

/*;***************************************************************************
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
*/

#ifndef FACINGFF_H
#define FACINGFF_H

//IDEAL
//P386
//MODEL USE32 FLAT

//GLOBAL	C Desired_Facing256	:NEAR
//;	INCLUDE	"wwlib.i"
//INCLUDE "..\include\gbuffer.inc"

//	CODESEG
/*
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

int __cdecl Desired_Facing256(LONG srcx, LONG srcy, LONG dstx, LONG dsty);


#if (0)
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
#endif



















/*

;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Support Library                          *
;*                                                                         *
;*                    File Name : FACING8.ASM                              *
;*                                                                         *
;*                   Programmer : Joe L. Bostic                            *
;*                                                                         *
;*                   Start Date : May 8, 1991                              *
;*                                                                         *
;*                  Last Update : February 6, 1995  [BWG]                  *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Desired_Facing8 -- Determines facing to reach a position.             *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *


IDEAL
P386
MODEL USE32 FLAT

GLOBAL	 C Desired_Facing8	:NEAR
;	INCLUDE	"wwlib.i"

	DATASEG

; 8 direction desired facing lookup table.  Build the index according
; to the following bits:
;
; bit 3 = Is y2 < y1?
; bit 2 = Is x2 < x1?
; bit 1 = Is the ABS(x2-x1) < ABS(y2-y1)?
; bit 0 = Is the facing closer to a major axis?
//NewFacing8	DB	1,2,1,0,7,6,7,0,3,2,3,4,5,6,5,4

//	CODESEG
*/

/*
;***************************************************************************
;* DESIRED_FACING8 -- Determines facing to reach a position.               *
;*                                                                         *
;*    This routine will return with the most desirable facing to reach     *
;*    one position from another.  It is accurate to a resolution of 0 to   *
;*    7.                                                                   *
;*                                                                         *
;* INPUT:       x1,y1   -- Position of origin point.                       *
;*                                                                         *
;*              x2,y2   -- Position of target.                             *
;*                                                                         *
;* OUTPUT:      Returns desired facing as a number from 0..255 with an     *
;*              accuracy of 32 degree increments.                          *
;*                                                                         *
;* WARNINGS:    If the two coordinates are the same, then -1 will be       *
;*              returned.  It is up to you to handle this case.            *
;*                                                                         *
;* HISTORY:                                                                *
;*   07/15/1991 JLB : Documented.                                          *
;*   08/08/1991 JLB : Same position check.                                 *
;*   08/14/1991 JLB : New algorithm                                        *
;*   02/06/1995 BWG : Convert to 32-bit                                    *
;*=========================================================================*
*/
int __cdecl Desired_Facing8(long x1, long y1, long x2, long y2);

#if (0)
	PROC	Desired_Facing8 C near
	USES	ebx, ecx, edx

	ARG	x1:DWORD
	ARG	y1:DWORD
	ARG	x2:DWORD
	ARG	y2:DWORD

	xor	ebx,ebx			; Index byte (built).

	; Determine Y axis difference.
	mov	edx,[y1]
	mov	ecx,[y2]
	sub	edx,ecx			; DX = Y axis (signed).
	jns	short ??absy
	inc	ebx			; Set the signed bit.
	neg	edx			; ABS(y)
??absy:

	; Determine X axis difference.
	shl	ebx,1
	mov	eax,[x1]
	mov	ecx,[x2]
	sub	ecx,eax			; CX = X axis (signed).
	jns	short ??absx
	inc	ebx			; Set the signed bit.
	neg	ecx			; ABS(x)
??absx:

	; Determine the greater axis.
	cmp	ecx,edx
	jb	short ??dxisbig
	xchg	ecx,edx
??dxisbig:
	rcl	ebx,1			; Y > X flag bit.

	; Determine the closeness or farness of lesser axis.
	mov	eax,edx
	inc	eax			; Round up.
	shr	eax,1

	cmp	ecx,eax
	rcl	ebx,1			; Close to major axis bit.

	xor	eax,eax
	mov	al,[NewFacing8+ebx]

	; Normalize to 0..FF range.
	shl	eax,5

	ret

	ENDP	Desired_Facing8


	END

#endif




/*
	; $Header: //depot/Projects/Mobius/QA/Project/Run/SOURCECODE/TIBERIANDAWN/WIN32LIB/FACINGFF.h#33 $
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Support Library                          *
;*                                                                         *
;*                    File Name : FACING16.ASM                             *
;*                                                                         *
;*                   Programmer : Joe L. Bostic                            *
;*                                                                         *
;*                   Start Date : May 8, 1991                              *
;*                                                                         *
;*                  Last Update : February 6, 1995  [BWG]                  *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Desired_Facing16 -- Converts coordinates into a facing number.        *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *


IDEAL
P386
MODEL USE32 FLAT

GLOBAL	 C Desired_Facing16	:NEAR
;	INCLUDE	"wwlib.i"

	DATASEG

; 16 direction desired facing lookup table.  Build the index according
; to the following bits:
;
; bit 4 = Is y2 < y1?
; bit 3 = Is x2 < x1?
; bit 2 = Is the ABS(x2-x1) < ABS(y2-y1)?
; bit 1 = Is the lesser absolute difference very close to zero?
; bit 0 = Is the lesser absolute difference very close to the greater dist?
NewFacing16	DB	 3, 2, 4,-1, 1, 2,0,-1
		DB	13,14,12,-1,15,14,0,-1
		DB	 5, 6, 4,-1, 7, 6,8,-1
		DB	11,10,12,-1, 9,10,8,-1

	CODESEG

;***************************************************************************
;* DESIRED_FACING16 -- Converts coordinates into a facing number.          *
;*                                                                         *
;*      This converts coordinates into a desired facing number that ranges *
;*      from 0 to 15 (0 equals North and going clockwise).                 *
;*                                                                         *
;* INPUT:       x1,y1   -- Position of origin point.                       *
;*                                                                         *
;*              x2,y2   -- Position of target.                             *
;*                                                                         *
;* OUTPUT:      Returns desired facing as a number from 0 to 255 but       *
;*              accurate to 22.5 degree increments.                        *
;*                                                                         *
;* WARNINGS:    If the two coordinates are the same, then -1 will be       *
;*              returned.  It is up to you to handle this case.            *
;*                                                                         *
;* HISTORY:                                                                *
;*   08/14/1991 JLB : Created.                                             *
;*=========================================================================*
*/
long __cdecl Desired_Facing16(long x1, long y1, long x2, long y2);
	
#if (0)
	PROC	Desired_Facing16 C near
	USES	ebx, ecx, edx

	ARG	x1:DWORD
	ARG	y1:DWORD
	ARG	x2:DWORD
	ARG	y2:DWORD

	xor	ebx,ebx			; Index byte (built).

	; Determine Y axis difference.
	mov	edx,[y1]
	mov	ecx,[y2]
	sub	edx,ecx			; DX = Y axis (signed).
	jns	short ??absy
	inc	ebx			; Set the signed bit.
	neg	edx			; ABS(y)
??absy:

	; Determine X axis difference.
	shl	ebx,1
	mov	eax,[x1]
	mov	ecx,[x2]
	sub	ecx,eax			; CX = X axis (signed).
	jns	short ??absx
	inc	ebx			; Set the signed bit.
	neg	ecx			; ABS(x)
??absx:

	; Determine the greater axis.
	cmp	ecx,edx
	jb	short ??dxisbig
	xchg	ecx,edx
??dxisbig:
	rcl	ebx,1			; Y > X flag bit.

	; Determine the closeness or farness of lesser axis.
	mov	eax,edx
	inc	eax			; Round up.
	shr	eax,1
	inc	eax			; Round up.
	shr	eax,1			; 1/4 of greater axis.

	cmp	ecx,eax
	rcl	ebx,1			; Very close to major axis bit.

	sub	edx,eax
	cmp	edx,ecx
	rcl	ebx,1			; Very far from major axis bit.

	xor	eax,eax
	mov	al,[NewFacing16+ebx]

	; Normalize to 0..FF range.
	shl	eax,4

	ret

	ENDP	Desired_Facing16

	END
#endif




#endif FACINGFF_H
