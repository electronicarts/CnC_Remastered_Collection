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

; $Header:   F:\projects\c&c0\vcs\code\lcwcomp.asv   5.0   11 Nov 1996 09:40:34   JOE_BOSTIC  $
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Library routine                          *
;*                                                                         *
;*                    File Name : COMPRESS.ASM                             *
;*                                                                         *
;*                   Programmer : Louis Castle                             *
;*                                                                         *
;*                  Last Update : 20 August, 1990   [CY]                   *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*                                                                         *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

;IDEAL
;P386
;MODEL USE32 FLAT

.MODEL FLAT

;GLOBAL    C LCW_Comp          :NEAR
externdef C LCW_Comp:near

;CODESEG
.code

; ----------------------------------------------------------------
;
; Here are prototypes for the routines defined within this module:
;
; ULONG LCW_Compress(BYTE *source,BYTE *dest, ULONG length);
;
; ----------------------------------------------------------------



;***********************************************************
;
; ULONG LCW_Compress(BYTE *source, BYTE *dest, ULONG length)
;
; returns the size of the compressed data in bytes
;
;*
LCW_Comp proc C source:DWORD, dest:DWORD, datasize:DWORD

	;USES ebx,ecx,edx,edi,esi

	;ARG	source:DWORD
	;ARG	dest:DWORD
	;ARG	datasize:DWORD

	LOCAL inlen:DWORD
	LOCAL a1stdest:DWORD
	LOCAL a1stsrc:DWORD
	LOCAL lenoff:DWORD
	LOCAL ndest:DWORD
	LOCAL count:DWORD
	LOCAL matchoff:DWORD
	LOCAL end_of_data:DWORD

	pushad

	cld
 	mov	edi,[dest]
 	mov	esi,[source]
 	mov	edx,[datasize]		; get length of data to compress

;	mov	ax,ds
;	mov	es,ax

;
; compress data to the following codes in the format b = byte, w = word
; n = byte code pulled from compressed data
;   Bit field of n		command		description
; n=0xxxyyyy,yyyyyyyy		short run	back y bytes and run x+3
; n=10xxxxxx,n1,n2,...,nx+1	med length	copy the next x+1 bytes
; n=11xxxxxx,w1			med run		run x+3 bytes from offset w1
; n=11111111,w1,w2		long run	run w1 bytes from offset w2
; n=10000000			end		end of data reached
;
	cld			; make sure all string commands are forward
	mov	ebx,esi
	add	ebx,edx
	mov	[end_of_data],ebx
	mov	[inlen],1	; set the in-length flag
	mov	[a1stdest],edi	; save original dest offset for size calc
	mov	[a1stsrc],esi	; save offset of first byte of data
	mov	[lenoff],edi	; save the offset of the legth of this len
	sub	eax,eax
	mov	al,081h		; the first byte is always a len
	stosb			; write out a len of 1
	lodsb			; get the byte
	stosb			; save it
_loop:
	mov	[ndest],edi	; save offset of compressed data
	mov	edi,[a1stsrc]	; get the offset to the first byte of data
	mov	[count],1	; set the count of run to 0
searchloop:
	sub	eax,eax
	mov	al,[esi]	; get the current byte of data
	cmp	al,[esi+64]
	jne	short notrunlength

	mov	ebx,edi

	mov	edi,esi
	mov	ecx,[end_of_data]
	sub	ecx,edi
   	repe	scasb
	dec	edi
	mov	ecx,edi
	sub	ecx,esi
	cmp	ecx,65
	jb	short notlongenough

	mov	[DWORD PTR inlen],0	; clear the in-length flag
	mov	esi,edi
	mov	edi,[ndest]	; get the offset of our compressed data

	mov	ah,al
	mov	al,0FEh
	stosb
	xchg	ecx,eax
	stosw
	mov	al,ch
	stosb

	mov	[ndest],edi	; save offset of compressed data
	mov	edi,ebx
	jmp	searchloop
notlongenough:
	mov	edi,ebx
notrunlength:

oploop:
	mov	ecx,esi		; get the address of the last byte +1
	sub	ecx,edi		; get the total number of bytes left to comp
	jz	short searchdone

	repne	scasb		; look for a match
	jne	short searchdone	; if we don't find one we're done

	mov	ebx,[count]
	mov	ah,[esi+ebx-1]
	cmp	ah,[edi+ebx-2]

	jne	oploop

	mov	edx,esi		; save this spot for the next search
	mov	ebx,edi		; save this spot for the length calc
	dec	edi		; back up one for compare
	mov	ecx,[end_of_data]		; get the end of data
	sub	ecx,esi		; sub current source for max len

	repe	cmpsb		; see how many bytes match

; start of change MH 9-24-91
	jne	short notend	; if found mismatch then di - bx = match count

	inc	edi		; else cx = 0 and di + 1 - bx = match count

notend:
; end of change MH 9-24-91

	mov	esi,edx		; restore si
	mov	eax,edi		; get the dest
	sub	eax,ebx		; sub the start for total bytes that match
	mov	edi,ebx		; restore dest
	cmp	eax,[count]	; see if its better than before
	jb	searchloop	; if not keep looking

	mov	[count],eax	; if so keep the count
	dec	ebx		; back it up for the actual match offset
	mov	[matchoff],ebx ; save the offset for later
	jmp	searchloop	; loop until we searched it all

searchdone:

	mov	ecx,[count]	; get the count of the longest run
	mov	edi,[ndest]	; get the offset of our compressed data
	cmp	ecx,2		; see if its not enough run to matter
	jbe	short lenin		; if its 0,1, or 2 its too small

	cmp	ecx,10		; if not, see if it would fit in a short
	ja	short medrun	; if not, see if its a medium run

	mov	eax,esi		; if its short get the current address
	sub	eax,[matchoff] ; sub the offset of the match
	cmp	eax,0FFFh	; if its less than 12 bits its a short
	ja	short medrun	; if its not, its a medium

shortrun:
	sub	ebx,ebx
	mov	bl,cl		; get the length (3-10)
	sub	bl,3		; sub 3 for a 3 bit number 0-7
	shl	bl,4		; shift it left 4
	add	ah,bl		; add in the length for the high nibble
	xchg	ah,al		; reverse the bytes for a word store
	jmp	short srunnxt	; do the run fixup code

medrun:
	cmp	ecx,64		; see if its a short run
	ja	short longrun	; if not, oh well at least its long

	sub	cl,3		; back down 3 to keep it in 6 bits
	or	cl,0C0h		; the highest bits are always on
	mov	al,cl		; put it in al for the stosb
	stosb			; store it
	jmp	short medrunnxt ; do the run fixup code

lenin:
	cmp	[DWORD PTR inlen],0	; is it doing a length?
	jnz	short len	; if so, skip code

lenin1:
	mov	[lenoff],edi	; save the length code offset
	mov	al,80h		; set the length to 0
	stosb			; save it

len:
	mov	ebx,[lenoff]	; get the offset of the length code
	cmp	BYTE PTR [ebx],0BFh	; see if its maxed out
	je	lenin1	; if so put out a new len code

stolen:
	inc	BYTE PTR [ebx] ; inc the count code
	lodsb			; get the byte
	stosb			; store it
	mov	DWORD PTR [inlen],1	; we are now in a length so save it
	jmp	short nxt	; do the next code

longrun:
	mov	al,0ffh		; its a long so set a code of FF
	stosb			; store it

	mov	eax,[count]	; send out the count
	stosw			; store it
medrunnxt:
	mov	eax,[matchoff] ; get the offset
	sub	eax,[a1stsrc]	; make it relative tot he start of data
srunnxt:
	stosw			; store it
; this code common to all runs
	add	esi,[count]	; add in the length of the run to the source
	mov	[DWORD PTR inlen],0	; set the in leght flag to false

;=======================================================================

nxt:
	cmp	esi,[end_of_data]		; see if we did the whole pic
	jae	short _out		; if so, cool! were done

	jmp	_loop

_out:
	mov	ax,080h		; remember to send an end of data code
	stosb			; store it
	mov	eax,edi		; get the last compressed address
	sub	eax,[a1stdest]	; sub the first for the compressed size

	popad
	ret

LCW_Comp endp


END
