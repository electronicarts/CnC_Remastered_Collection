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
;*                    File Name : COORDA.ASM                               *
;*                                                                         *
;*                   Programmer : Barry W. Green                           *
;*                                                                         *
;*                   Start Date : February 17, 1995                        *
;*                                                                         *
;*                  Last Update : February 17, 1995  [BWG]                 *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Cardinal_To_Fixed -- Converts cardinal numbers into a fixed point number.                 *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *


IDEAL
P386
MODEL USE32 FLAT

global C	Cardinal_To_Fixed	:NEAR
global C	Fixed_To_Cardinal	:NEAR

	CODESEG

;***********************************************************************************************
;* Cardinal_To_Fixed -- Converts cardinal numbers into a fixed point number.                   *
;*                                                                                             *
;*    This utility function will convert cardinal numbers into a fixed point fraction. The     *
;*    use of fixed point numbers occurs throughout the product -- since it is a convenient     *
;*    tool. The fixed point number is based on the formula:                                    *
;*                                                                                             *
;*       result = cardinal / base                                                              *
;*                                                                                             *
;*    The accuracy of the fixed point number is limited to 1/256 as the lowest and up to       *
;*    256 as the largest.                                                                      *
;*                                                                                             *
;* INPUT:   base     -- The key number to base the fraction about.                             *
;*                                                                                             *
;*          cardinal -- The other number (hey -- what do you call it?)                         *
;*                                                                                             *
;* OUTPUT:  Returns with the fixed point number of the "cardinal" parameter as it relates      *
;*          to the "base" parameter.                                                           *
;*                                                                                             *
;* WARNINGS:   none                                                                            *
;*                                                                                             *
;* HISTORY:                                                                                    *
;*   02/17/1995 BWG : Created.                                                                 *
;*=============================================================================================*/
;unsigned int Cardinal_To_Fixed(unsigned base, unsigned cardinal);

	PROC	Cardinal_To_Fixed C near
	USES	ebx, edx

	ARG	base:DWORD
	ARG	cardinal:DWORD

	mov	eax,0FFFFh		; establish default return value

	mov	ebx,[base]
	or	ebx,ebx
	jz	near ??retneg1		; if base==0, return 65535

	mov	eax,[cardinal]		; otherwise, return (cardinal*256)/base
	shl	eax,8
	xor	edx,edx
	div	ebx

??retneg1:
	ret

	ENDP	Cardinal_To_Fixed


;***********************************************************************************************
;* Fixed_To_Cardinal -- Converts a fixed point number into a cardinal number.                  *
;*                                                                                             *
;*    Use this routine to convert a fixed point number into a cardinal number.                 *
;*                                                                                             *
;* INPUT:   base     -- The base number that the original fixed point number was created from. *
;*                                                                                             *
;*          fixed    -- The fixed point number to convert.                                     *
;*                                                                                             *
;* OUTPUT:  Returns with the reconverted number.                                               *
;*                                                                                             *
;* WARNINGS:   none                                                                            *
;*                                                                                             *
;* HISTORY:                                                                                    *
;*   02/17/1995 BWG : Created.                                                                 *
;*=============================================================================================*/
;unsigned int Fixed_To_Cardinal(unsigned base, unsigned fixed);
	PROC	Fixed_To_Cardinal C near
	USES	edx

	ARG	base:DWORD
	ARG	fixed:DWORD

	mov	eax,[base]
	mul	[fixed]
	add	eax,080h		; eax = (base * fixed) + 0x80

	test	eax,0FF000000h		; if high byte set, return FFFF
	jnz	??rneg1
	shr	eax,8			; else, return eax/256
	ret
??rneg1	:
	mov	eax,0FFFFh		; establish default return value
	ret

	ENDP	Fixed_To_Cardinal

	END
