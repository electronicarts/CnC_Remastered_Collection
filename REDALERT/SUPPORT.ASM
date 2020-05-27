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

; $Header:   F:\projects\c&c0\vcs\code\support.asv   5.0   11 Nov 1996 09:40:30   JOE_BOSTIC  $
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Command & Conquer                        *
;*                                                                         *
;*                    File Name : SUPPORT.ASM                              *
;*                                                                         *
;*                   Programmer : Joe L. Bostic                            *
;*                                                                         *
;*                   Start Date : September 23, 1993                       *
;*                                                                         *
;*                  Last Update : May 10, 1994   [JLB]                     *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   strtrim -- Remove the trailing white space from a string.             *
;*   Fat_Put_Pixel -- Draws a fat pixel.                                   *
;*   Conquer_Build_Fading_Table -- Builds custom shadow/light fading table.*
;*   Remove_From_List -- Removes a pointer from a list of pointers.        *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

INCLUDE "gbuffer.inc"
	DISPLAY	"Command & Conquer assembly support routines."

	CODESEG


;***************************************************************************
;* Fat_Put_Pixel -- Draws a fat pixel.                                     *
;*                                                                         *
;*    Use this routine to draw a "pixel" that is bigger than 1 pixel       *
;*    across.  This routine is faster than drawing a similar small shape   *
;*    and faster than calling Fill_Rect.                                   *
;*                                                                         *
;* INPUT:   x,y       -- Screen coordinates to draw the pixel's upper      *
;*                       left corner.                                      *
;*                                                                         *
;*          color     -- The color to render the pixel in.                 *
;*                                                                         *
;*          size      -- The number of pixels width of the big "pixel".    *
;*                                                                         *
;*          page      -- The pointer to a GraphicBuffer class or something *
;*                                                                         *
;* OUTPUT:  none                                                           *
;*                                                                         *
;* WARNINGS:   none                                                        *
;*                                                                         *
;* HISTORY:                                                                *
;*   03/17/1994 JLB : Created.                                             *
;*=========================================================================*
; VOID cdecl Fat_Put_Pixel(long x, long y, long color, long size, void *page)
	GLOBAL	C Fat_Put_Pixel:NEAR
	PROC	Fat_Put_Pixel C near
	USES	eax, ebx, ecx, edx, edi, esi

	ARG	x:DWORD		; X coordinate of upper left pixel corner.
	ARG	y:DWORD		; Y coordinate of upper left pixel corner.
	ARG	color:DWORD	; Color to use for the "pixel".
	ARG	siz:DWORD	; Size of "pixel" to plot (square).
	ARG	gpage:DWORD	; graphic page address to plot onto

	cmp	[siz],0
	je	short ??exit

	; Set EDI to point to start of logical page memory.
	;*===================================================================
	; Get the viewport information and put bytes per row in ecx
	;*===================================================================
	mov	ebx,[gpage]				; get a pointer to viewport
	mov	edi,[(GraphicViewPort ebx).GVPOffset]	; get the correct offset

	; Verify the the Y pixel offset is legal.
	mov	eax,[y]
	cmp	eax,[(GraphicViewPort ebx).GVPHeight]	;YPIXEL_MAX
	jae	short ??exit
	mul	[(GraphicViewPort ebx).GVPWidth]
	add	edi,eax

	; Verify the the X pixel offset is legal.
	mov	edx,[(GraphicViewPort ebx).GVPWidth]
	cmp	edx,[x]
	jbe	short ??exit
	add	edi,[x]

	; Write the pixel to the screen.
	mov	ebx,[siz]		; Copy of pixel size.
	sub	edx,ebx			; Modulo to reach start of next row.
	mov	eax,[color]
??again:
	mov	ecx,ebx
	rep stosb
	add	edi,edx			; EDI points to start of next row.
	dec	[siz]
	jnz	short ??again

??exit:
	ret

	ENDP	Fat_Put_Pixel


if 0

;***************************************************************************
;* strtrim -- Remove the trailing white space from a string.               *
;*                                                                         *
;*    Use this routine to remove white space characters from the beginning *
;*    and end of the string.        The string is modified in place by     *
;*    this routine.                                                        *
;*                                                                         *
;* INPUT:   buffer   -- Pointer to the string to modify.                   *
;*                                                                         *
;* OUTPUT:     none                                                        *
;*                                                                         *
;* WARNINGS:   none                                                        *
;*                                                                         *
;* HISTORY:                                                                *
;*   10/07/1992 JLB : Created.                                             *
;*=========================================================================*
; VOID cdecl strtrim(BYTE *buffer);
	GLOBAL	C strtrim :NEAR
	PROC	strtrim C near
	USES	ax, edi, esi

	ARG	buffer:DWORD		; Pointer to string to modify.

	cmp	[buffer],0
	je	short ??fini

	; Prepare for string scanning by loading pointers.
	cld
	mov	esi,[buffer]
	mov	edi,esi

	; Strip white space from the start of the string.
??looper:
	lodsb
	cmp	al,20h			; Space
	je	short ??looper
	cmp	al,9			; TAB
	je	short ??looper
	stosb

	; Copy the rest of the string.
??gruntloop:
	lodsb
	stosb
	or	al,al
	jnz	short ??gruntloop
	dec	edi
	; Strip the white space from the end of the string.
??looper2:
	mov	[edi],al
	dec	edi
	mov	ah,[edi]
	cmp	ah,20h
	je	short ??looper2
	cmp	ah,9
	je	short ??looper2

??fini:
	ret

	ENDP	strtrim


;***************************************************************************
;* Conquer_Build_Fading_Table -- Builds custom shadow/light fading table.  *
;*                                                                         *
;*    This routine is used to build a special fading table for C&C.  There *
;*    are certain colors that get faded to and cannot be faded again.      *
;*    With this rule, it is possible to draw a shadow multiple times and   *
;*    not have it get any lighter or darker.                               *
;*                                                                         *
;* INPUT:   palette  -- Pointer to the 768 byte IBM palette to build from. *
;*                                                                         *
;*          dest     -- Pointer to the 256 byte remap table.               *
;*                                                                         *
;*          color    -- Color index of the color to "fade to".             *
;*                                                                         *
;*          frac     -- The fraction to fade to the specified color        *
;*                                                                         *
;* OUTPUT:  Returns with pointer to the remap table.                       *
;*                                                                         *
;* WARNINGS:   none                                                        *
;*                                                                         *
;* HISTORY:                                                                *
;*   10/07/1992 JLB : Created.                                             *
;*=========================================================================*/
;VOID * cdecl Conquer_Build_Fading_Table(VOID *palette, VOID *dest, long color, long frac);
	GLOBAL	C Conquer_Build_Fading_Table : NEAR
	PROC	Conquer_Build_Fading_Table C near
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

ALLOWED_COUNT	EQU	16
ALLOWED_START	EQU	256-ALLOWED_COUNT

	cld

	; If the source palette is NULL, then just return with current fading table pointer.
	cmp	[palette],0
	je	??fini1
	cmp	[dest],0
	je	??fini1

	; Fractions above 255 become 255.
	mov	eax,[frac]
	cmp	eax,0100h
	jb	short ??ok
	mov	[frac],0FFh
??ok:

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
??mainloop:
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
	shl	eax,1
	sub	dh,ah			; dh = orig - ((orig-target) * fraction)
	mov	[idealred],dh		; preserve ideal color gun value.

	lodsb				; orig
	mov	dh,al			; preserve it for later.
	sub	al,[targetgreen]	; al = (orig-target)
	imul	dl			; ax = (orig-target)*fraction
	shl	eax,1
	sub	dh,ah			; dh = orig - ((orig-target) * fraction)
	mov	[idealgreen],dh		; preserve ideal color gun value.

	lodsb				; orig
	mov	dh,al			; preserve it for later.
	sub	al,[targetblue]		; al = (orig-target)
	imul	dl			; ax = (orig-target)*fraction
	shl	eax,1
	sub	dh,ah			; dh = orig - ((orig-target) * fraction)
	mov	[idealblue],dh		; preserve ideal color gun value.

	; Sweep through a limited set of existing colors to find the closest
	; matching color.

	mov	eax,[color]
	mov	[matchcolor],al		; Default color (self).
	mov	[matchvalue],-1		; Ridiculous match value init.
	mov	ecx,ALLOWED_COUNT

	mov	esi,[palette]		; Pointer to original palette.
	add	esi,(ALLOWED_START)*3

	; BH = color index.
	mov	bh,ALLOWED_START
??innerloop:

	xor	edx,edx			; Comparison value starts null.

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
	jz	short ??perfect		; If perfect match found then quit early.

	cmp	edx,[matchvalue]
	jae	short ??notclose
	mov	[matchvalue],edx	; Record new possible color.
	mov	[matchcolor],bh
??notclose:
	inc	bh			; Checking color index.
	loop	??innerloop
	mov	bh,[matchcolor]
??perfect:
	mov	[matchcolor],bh
	xor	bh,bh			; Make BX valid main index again.

	; When the loop exits, we have found the closest match.
	mov	al,[matchcolor]
	stosb
	cmp	ebx,ALLOWED_START-1
	jne	??mainloop

	; Fill the remainder of the remap table with values
	; that will remap the color to itself.
	mov	ecx,ALLOWED_COUNT
??fillerloop:
	inc	bl
	mov	al,bl
	stosb
	loop	??fillerloop

??fini1:
	mov	esi,[dest]
	mov	eax,esi
	ret

	ENDP	Conquer_Build_Fading_Table


;***************************************************************************
;* Remove_From_List -- Removes a pointer from a list of pointers.          *
;*                                                                         *
;*    This low level routine is used to remove a pointer from a list of    *
;*    pointers.  The trailing pointers are moved downward to fill the      *
;*    hole.                                                                *
;*                                                                         *
;* INPUT:   list     -- Pointer to list of pointer.                        *
;*                                                                         *
;*          index    -- Pointer to length of pointer list.                 *
;*                                                                         *
;*          ptr      -- The pointer value to search for and remove.        *
;*                                                                         *
;* OUTPUT:  none                                                           *
;*                                                                         *
;* WARNINGS:   none                                                        *
;*                                                                         *
;* HISTORY:                                                                *
;*   04/11/1994 JLB : Created.                                             *
;*   04/22/1994 JLB : Convert to assembly language.                        *
;*   05/10/1994 JLB : Short pointers now.                                  *
;*=========================================================================*/
;VOID cdecl Remove_From_List(VOID **list, long *index, long ptr);
	GLOBAL	C Remove_From_List:NEAR
	PROC	Remove_From_List C near
	USES	edi, esi, ecx, eax
	ARG	list:DWORD		; Pointer to list.
	ARG	index:DWORD		; Pointer to count.
	ARG	element:DWORD		; Element to remove.

	; Fetch the number of elements in the list.  If there are no
	; elements, then just exit quickly.
	mov	edi,[index]
	mov	ecx,[edi]
	jcxz	short ??fini2

	; Fetch pointer to list.
	cmp	[list],0
	je	short ??fini2
	mov	edi,[list]

	; Loop through all elements searching for a match.
	mov	eax,[element]
	repne scasd
	jne	short ??fini2		; No match found.

	; Copy all remaining elements down.  If this is the
	; last element in the list then nothing needs to be
	; copied -- just decrement the list size.
	jcxz	short ??nocopy		; No copy necessary.
	mov	esi,edi
	sub	edi,4
	rep movsd

	; Reduce the list count by one.
??nocopy:
	mov	edi,[index]
	dec	[DWORD PTR edi]

??fini2:
	ret

	ENDP	Remove_From_List


; long cdecl Get_EAX();
	GLOBAL	C Get_EAX :NEAR
	PROC	Get_EAX C near
	ret

	ENDP	Get_EAX
endif


	DATASEG

TabA	DD 6949350
	DD 4913933
	DD 3474675
	DD 2456966
	DD 1737338
	DD 1228483
	DD 868669
	DD 614242
	DD 434334
	DD 307121
	DD 217167
	DD 153560
	DD 108584
	DD 76780
	DD 54292
	DD 38390
	DD 27146
	DD 19195
	DD 13573
	DD 9598
	DD 6786
	DD 4799
	DD 3393
	DD 2399
	DD 1697
	DD 1200
	DD 848
	DD 600
	DD 424
	DD 300
	DD 212
	DD 150
	DD 106

TabB	DD 154
	DD 218
	DD 309
	DD 437
	DD 618
	DD 874
	DD 1236
	DD 1748
	DD 2472
	DD 3496
	DD 4944
	DD 6992
	DD 9888
	DD 13983
	DD 19775
	DD 27967
	DD 39551
	DD 55933
	DD 79101
	DD 111866
	DD 158203
	DD 223732
	DD 316405
	DD 447465
	DD 632811
	DD 894929
	DD 1265621
	DD 1789859
	DD 2531243
	DD 3579718
	DD 5062486
	DD 7159436
	DD 10124971

	CODESEG

;***********************************************************************************************
;* Square_Root -- Finds the square root of the fixed pointer parameter.                        *
;*                                                                                             *
;* INPUT:   val   -- The fixed point (16:16) value to find the square root of.                 *
;*                                                                                             *
;* OUTPUT:  Returns with the square root of the fixed pointer parameter.                       *
;*                                                                                             *
;* WARNINGS:   none                                                                            *
;*                                                                                             *
;* HISTORY:                                                                                    *
;*   10/04/1995 JLB : Adapted.                                                                 *
;*=============================================================================================*/
;unsigned Square_Root(unsigned val);
	GLOBAL	C Square_Root :NEAR
	PROC	Square_Root C near
	USES	ebx,edx

	bsr ebx,eax
	jz ??zero

	mul [DWORD 4*ebx + OFFSET TabA]
	shrd eax,edx,10h
	add eax, [4*ebx + OFFSET TabB]
??zero:
	ret

	ENDP	Square_Root

;----------------------------------------------------------------------------

	END

