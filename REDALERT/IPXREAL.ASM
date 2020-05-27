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
;**   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
;***************************************************************************
;*                                                                         *
;*                 Project Name : VQLIB                                    *
;*                                                                         *
;*                    File Name : HANDLER.ASM                              *
;*                                                                         *
;*                   Programmer : Bill Randolph                            *
;*                                                                         *
;*                   Start Date : April 7, 1995                            *
;*                                                                         *
;*                  Last Update : April 7, 1995   [BRR]                    *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   IPXHandler -- callback routine for IPX                                *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
;********************* Model & Processor Directives ************************
IDEAL
MODEL LARGE
P386N
LOCALS ??


;******************************** Includes *********************************


;******************************** Defines ***********************************


;****************************** Declarations ********************************
GLOBAL	IPXHandler:FAR


;********************************* Code ************************************
	CODESEG


;---------------------------------------------------------------------------
; The markers let the application verify that it's mapping this memory
; correctly.
;---------------------------------------------------------------------------
Marker1			DW 1111h	; placeholder to find data start

;---------------------------------------------------------------------------
; This is the IPX Event Control Block:
;---------------------------------------------------------------------------
ECB_LinkAddress		DD ?
ECB_EventServiceRoutine	DD ?		; Event Handler ptr
ECB_InUse		DB ?		; 0 = event is complete
ECB_CompletionCode	DB ?		; 0 = OK, IPX error otherwise
ECB_SocketNumber	DW ?		; socket to listen/send on
ECB_ConnectionID	DW ?
ECB_RestOfWorkspace	DW ?
ECB_DriverWorkSpace	DB 12 DUP (?)
ECB_ImmediateAddress	DB 6 DUP (?)	; bridge address
ECB_PacketCount		DW ?		; # data areas (2)
ECB_HeaderAddress	DD ?		; ptr to IPX header buffer
ECB_HeaderLength	DW ?		; length of IPX header buffer
ECB_PacketAddress	DD ? 		; ptr to packet buffer
ECB_PacketLength	DW ?		; length of packet buffer

;---------------------------------------------------------------------------
; The rest of the variables are for telling IPX which buffer to store the
; next incoming packet in.  They must be initialized by the application.
;---------------------------------------------------------------------------
NumBufs		DW 0			; # buffers provided by app
BufferFlags	DD 0			; array of in-use flags (1 = in use)
PacketSize	DW 0			; total size of 1 buf (incl IPX hdr)
FirstPacketBuf	DD 0			; ptr to 1st packet buffer
CurIndex	DW 0			; current packet/flag index
CurPacketBuf	DD 0			; ptr to current packet buf
FuncOffset	DW StartLabel		; offset of our routine

;---------------------------------------------------------------------------
; These values are for preventing re-entrancy; they're currently not used.
;---------------------------------------------------------------------------
Semaphore	DB 0			; prevents re-entrancy
ReEntrantCount	DW 0			; times we've been called re-entrantly

;---------------------------------------------------------------------------
; Local stack space
;---------------------------------------------------------------------------
StackPtr	DW 0			; saved copy of stack ptr
StackSeg	DW 0			; saved copy of stack seg
StackPtr_int	DW 0			; our internal stack ptr
StackSeg_int	DW 0			; our internal stack seg
StackCheck	DW 1234h		; check for stack overflow
		DW 256 DUP (0)		; stack storage space
StackSpace	DW 0			; label for our stack space

;---------------------------------------------------------------------------
; These bytes mark the end of the real-mode data area
;---------------------------------------------------------------------------
Marker2		DW 2222h		; placeholder to find data end


;***************************************************************************
;* IPXHandler -- IPX callback routine					   *
;*                                                                         *
;* This routine is assembled as a stand-alone executable, then loaded      *
;* into low DOS memory by a protected-mode application.                    *
;*                                                                         *
;* INPUT:                                                                  *
;*	none.								   *
;*									   *
;* OUTPUT: 			                                           *
;*	none.								   *
;*                                                                         *
;* WARNINGS:                                                               *
;*	none.								   *
;* 									   *
;* HISTORY:                                                                *
;*   04/07/1995 BRR : Created.                                             *
;*=========================================================================*
	label StartLabel
	PROC	IPXHandler C FAR USES

	;...................................................................
	; Turn off interrupts; make sure memory copies go forward
	;...................................................................
	pushf
	cli
	cld

	;...................................................................
	; Set up segment registers to point DS to CS
	;...................................................................
	push	ds
	push	ax
	mov	ax,cs
	mov	ds,ax

	;...................................................................
	; Set up our local stack; save SS & SP first.
	;...................................................................
	mov	[StackSeg],ss
	mov	[StackPtr],sp
	mov	[StackPtr_int], OFFSET StackSpace
	mov	[StackSeg_int], SEG StackSpace
	lss	sp, [DWORD PTR StackPtr_int]
	

	;...................................................................
	; Save all registers
	;...................................................................
	pushad
	push	es

	;...................................................................
	; If we've been called re-entrantly, just exit
	;...................................................................
	cmp	[Semaphore],0
	jz	??Start_Handler
	add	[ReEntrantCount],1
	jmp	??Exit_Handler

??Start_Handler:
	;...................................................................
	; Set our semaphore
	;...................................................................
	mov	[Semaphore],1

	;-------------------------------------------------------------------
	; Set 'CurIndex' to the index of the next-available receive buffer,
	; and 'CurPacketBuf to the next-available packet buffer
	;-------------------------------------------------------------------
	;...................................................................
	; Get 'CurIndex' & increment it.  Wrap to 0 if we reach 'NumBufs'
	; Since I'm treating 'CurPacketBuf' as a long integer (and not as
	; a segment:offset), the entire data area can't be larger than 64K.
	;...................................................................
	mov	dx,[CurIndex]		; DX = CurIndex
	mov	eax,[CurPacketBuf]	; EAX = current packet buffer addr
	inc	dx			; DX = next index
	add	ax,[PacketSize]		; EAX = next buffer ptr
	cmp	dx,[NumBufs]		; see if DX is past # buffers
	jb	??Get_Flag
	mov	dx,0			; wrap to 1st index
	mov	eax,[FirstPacketBuf]	; wrap to 1st packet buffer

??Get_Flag:
	;...................................................................
	; Get the next buffer-in-use flag; if it's 0, load [CurIndex] with 
	; the value of SI (the next index).  If it's 1, skip the updating of
	; the index, flag & buffer ptr.
	; DX = new CurIndex
	; EAX = new CurPacketBuf
	;...................................................................
	les	di,[BufferFlags]	; ES:DI = BufferFlags address
	mov	bx,di			; BX = DI + new CurIndex
	add	bx,dx

	cmp	[BYTE PTR es:bx],0	; compare next flag to 0 (avail)
	jne	??Set_ECB		; if not avail, skip setting new values

	;...................................................................
	; The next buffer is available; so, set this buffer's In-Use flag
	; to 1, and move on to the next buffer.  Do not set this buffer's
	; flag to 1 until we move on to the next buffer, to prevent the
	; application from reading the currently-in-use packet buffer.
	; DX = new CurIndex
	; EAX = new CurPacketBuf
	; ES:DI = BufferFlags address
	;...................................................................
	mov	bx,di			; BX = DI + old CurIndex
	add	bx,[CurIndex]
	mov	[BYTE PTR es:bx],1	; set old BufferFlags value to in-use

	mov	[CurIndex],dx		; save new index
	mov	[CurPacketBuf],eax	; save new packet address

	;-------------------------------------------------------------------
	; Set up the Event Control Block to tell IPX to start listening.
	; The following entries are filled in by the app, and should be left
	; alone:
	; - EventServiceRoutine
	; - SocketNumber
	; The rest should be re-initialized.  Note that EBX is now pointing
	; to an unavailable buffer if the next buffer was already in use;
	; so it must be reloaded with the correct buffer address from
	; [CurPacketBuf].
	;-------------------------------------------------------------------
??Set_ECB:
	mov	[ECB_LinkAddress],0		; default
	mov	[ECB_InUse],0			; default
	mov	[ECB_CompletionCode],0		; default
	mov	[ECB_ConnectionID],0		; default
	mov	[ECB_RestOfWorkspace],0		; default
	mov	[ECB_DriverWorkSpace],0		; default
	mov	[ECB_ImmediateAddress],0	; default
	mov	[ECB_PacketCount],2		; use 2 data areas
	mov	ebx,[CurPacketBuf]		; get current buffer address
	mov	[ECB_HeaderAddress],ebx		; set header address
	mov	[ECB_HeaderLength],30		; size of IPX header
	add	ebx,30				; point to past the header
	mov	[ECB_PacketAddress],ebx		; set packet data address
	mov	ax,[PacketSize]			; get size of one buffer
	sub	ax,30				; remove size of IPX header
	mov	[ECB_PacketLength],ax		; set size of packet data

	;-------------------------------------------------------------------
	; Clear the IPX header for this packet
	;-------------------------------------------------------------------
	les	di,[ECB_HeaderAddress]		; ES:DI = IPX Listen Header
	mov	cx,30				; (30 bytes = size of header)
	mov	al,0
	rep	stosb				; clear to 0's

	;-------------------------------------------------------------------
	; Command IPX to start listening again.
	;-------------------------------------------------------------------
	mov	bx,4				; IPX code for Listen
	mov	ax,ds				; ES = segment of ListenECB
	mov	es,ax
	mov	ax,OFFSET ECB_LinkAddress	
	mov	si,ax				; ES:SI = address of ECB
	int	07ah				; call IPX interrupt

	;...................................................................
	; Clear our semaphore
	;...................................................................
	mov	[Semaphore],0

??Exit_Handler:
	;...................................................................
	; Pop values from our local stack
	;...................................................................
	pop	es
	popad

	;...................................................................
	; Check our stack-check value; if the stack has overflowed, generate
	; a debugger break.
	;...................................................................
	cmp	[StackCheck],1234h
	je	??Restore_Stack
	int	3

	;...................................................................
	; Restore the stack to its previous value
	;...................................................................
??Restore_Stack:
	lss	sp, [DWORD PTR StackPtr]

	;...................................................................
	; Pop the rest of the registers
	;...................................................................
	pop	ax
	pop	ds

	popf

	ret

ENDP		IPXHandler

END

;************************** End of handler.asm *****************************
