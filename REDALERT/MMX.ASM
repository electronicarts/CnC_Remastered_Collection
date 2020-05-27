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
;*                    File Name : MMX.ASM                                  *
;*                                                                         *
;*                   Programmer : Steve Tall                               *
;*                                                                         *
;*                   Start Date : May 19th, 1996                           *
;*                                                                         *
;*                  Last Update : May 19th 1996 [ST]                       *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *


 ;		include	<mmx.inc>


		.model	flat
		;.686


		externdef	C Detect_MMX_Availability:near
		externdef	C Single_Line_Trans_Entry:near
		externdef	C Next_Line:near
		externdef	C Init_MMX:near
		externdef	C MMX_Done:near

		externdef	EndNewShapeJumpTable:byte
		externdef	NewShapeJumpTable:dword
		externdef	C Single_Line_Trans:near
		externdef	MMX_Single_Line_Trans:near


		.code

externdef	C CPUType:byte
externdef	C VendorID:byte



;*********************************************************************************************
;* Detect_MMX_Availability -- Detect the presence of MMX technology.                         *
;*                                                                                           *
;*                                                                                           *
;* INPUT:	Nothing                                                                      *
;*                                                                                           *
;* OUTPUT:      True if MMX technology is available.                                         *
;*                                                                                           *
;* Warnings:                                                                                 *
;*                                                                                           *
;* Note: Based in part on CPUID32.ASM by Intel                                               *
;*                                                                                           *
;* HISTORY:                                                                                  *
;*   05/19/96 ST : Created.                                                                  *
;*===========================================================================================*

Detect_MMX_Availability proc C

		local	idflag:byte
		local	local_cputype:byte

		; MMX always available now. ST - 1/3/2019 1:31PM
		mov	[CPUType], 5
		mov	eax, 1
		ret


;assume processor is at least 386
;
;check whether AC bit in eflags can be toggled.
;If not then processor is 386

		mov	[idflag],0

		pushfd			;get Eflags in EAX
		pop	eax
		mov	ecx,eax		;save eflags
		xor	eax,40000h	;toggle AC bit in eflags
		push	eax		;new eflags on stack
		popfd			;move new value into eflags
		pushfd			;get new eflags back into eax
		pop	eax
		xor	eax,ecx		;if AC bit not toggled then CPU=386
		mov	[local_cputype],3
		jz	@@end_get_cpu	;cpu is 386

		push	ecx
		popfd			;restore AC bit in eflags


;processor is at least 486
;
;Check for ability to set/clear ID flag in EFLAGS
;ID flag indicates ability of processor to execute the CPUID instruction.
;486 not guaranteed to have CPUID inst?
;
		mov	[local_cputype],4
		mov	eax,ecx		;original EFLAGS
		xor	eax,200000h	;toggle ID bit
		push	eax
		popfd
		pushfd
		pop	eax
		xor	eax,ecx		;check if still toggled
		jz	@@end_get_cpu


;       Execute CPUID instruction to determine vendor, family,
;       model and stepping.
;

		mov	[idflag],1	;flag ID is available

		xor	eax,eax
		cpuid

		mov	dword ptr [VendorID],ebx
		mov	dword ptr [VendorID+4],edx
		mov	dword ptr [VendorID+8],ecx
		mov	dword ptr [VendorID+12]," "

		cmp	eax,1		;check if 1 is valid
		jl	@@end_get_cpu	;inp for cpuid inst.

		xor	eax,eax
		inc	eax

		cpuid		;get stepping, model and family

		and     ax,0f00H
		shr     ax,08H

		mov	[local_cputype],al

@@end_get_cpu:	mov	al,[local_cputype]
		mov	[CPUType],al


;
; We have the CPU type in al now.
; If we arent on at least a pentium then we can assume there is no MMX
;
		cmp	al,5
		jl	@@no_mmx

		mov	eax,1
		cpuid
		test	edx,00800000h
		jz	@@no_mmx

;
; MMX detected - return true
;
		mov	eax,1
		ret


@@no_mmx:	xor	eax,eax
		ret


Detect_MMX_Availability endp



;*********************************************************************************************
;* Init_MMX -- Do any special inits required for MMX support                                 *
;*                                                                                           *
;*                                                                                           *
;* INPUT:	Nothing                                                                      *
;*                                                                                           *
;* OUTPUT:      None                                                                         *
;*                                                                                           *
;* Warnings:                                                                                 *
;*                                                                                           *
;* HISTORY:                                                                                  *
;*   05/19/96 ST : Created.                                                                  *
;*===========================================================================================*

Init_MMX	proc C

		mov	edi,offset NewShapeJumpTable
		mov	ecx,offset EndNewShapeJumpTable
		sub	ecx,edi
		shr	ecx,2
		mov	eax,offset Single_Line_Trans
		mov	ebx,offset MMX_Single_Line_Trans
		cld


@@patch_loop:   repnz	scasd
		jnz	@@done
		mov	[edi-4],ebx
		test	ecx,ecx
		jnz	@@patch_loop

@@done:		ret

Init_MMX	endp






;*********************************************************************************************
;* MMX_Done -- Restores floating point capability after MMX usage                            *
;*                                                                                           *
;*                                                                                           *
;* INPUT:	Nothing                                                                      *
;*                                                                                           *
;* OUTPUT:      None                                                                         *
;*                                                                                           *
;* Warnings:                                                                                 *
;*                                                                                           *
;* HISTORY:                                                                                  *
;*   05/19/96 ST : Created.                                                                  *
;*===========================================================================================*

MMX_Done	proc C

		emms
		ret

MMX_Done	endp







		code segment page public use32 'code'	; Need stricter segment alignment
							; for pentium optimisations


;*********************************************************************************************
;* MMX_Single_Line_Trans -- draw a single line of transparent pixels using MMX technology    *
;*                                                                                           *
;*                                                                                           *
;* INPUT:	Esi - ptr to source data                                                     *
;*              Edi - ptr to destination data                                                *
;*              Ecx - width to draw in bytes                                                 *
;*                                                                                           *
;* OUTPUT:      None                                                                         *
;*                                                                                           *
;* Warnings:                                                                                 *
;*                                                                                           *
;* HISTORY:                                                                                  *
;*   05/19/96 ST : Created.                                                                  *
;*===========================================================================================*

		align	16

MMX_Single_Line_Trans proc near

;
; If we are doing less than 8 bytes then dont use MMX
;
		cmp	ecx,8
		jge	@@mmx_loop
		push	offset Single_Line_Trans_Entry
		ret

;
; Use MMX instructions to mask 8 bytes at once
;
; Creates a bitmask based on the source bytes equality with zero and then uses this to mask
; out the source bytes in the destination data. The advatage that MMX gives us is that there is
; no 'test for zero then jump' required to mask.
;
		align	64		;MMX instructions like 64 byte alignment!

@@mmx_loop:
		movq	mm0,[esi]	; move 8 bytes of source into mm0
		pxor	mm1,mm1		; zero out mm1
		pcmpeqb mm1,mm0		; compare mm0 with 0. Bits get set in mm1
		lea	esi,[esi+8]     ; adjust the source data pointer
		pand	mm1,[edi]	; and in the destination data to throw away the bytes which arent zero in the source
		sub	ecx,8		; adjust the byte counter
		por	mm1,mm0         ; or in the source with the destination data
		movq	[edi],mm1       ; write back the destination data
		lea	edi,[edi+8]	; adjust the destination pointer

		cmp	ecx,8
		jg	@@mmx_loop

;
; Jump to the approprite code for drawing the end of this line or going to the next one
;
		push	offset Next_Line
		jcxz   	@@next_line
		push	offset Single_Line_Trans_Entry
@@next_line:	ret


MMX_Single_Line_Trans endp


code ends

		.data

CPUType		db	0
VendorID	db	"Not available",0,0,0,0,0,0


end