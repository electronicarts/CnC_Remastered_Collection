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

; $Header:   F:\projects\c&c0\vcs\code\cpuid.asv   5.0   11 Nov 1996 09:40:28   JOE_BOSTIC  $
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


		.586
		.model	flat

;
; Variables externs
;
GLOBAL	C CPUType:byte
;externdef	C CPUType:byte
GLOBAL	C VendorID:byte
;externdef	C VendorID:byte

;
; Function externs
;
GLOBAL	C Detect_MMX_Availability:near
;externdef	C Detect_MMX_Availability:near


		.code


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
		local	cputype:byte

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
		mov	[cputype],3
		jz	@@end_get_cpu	;cpu is 386

		push	ecx
		popfd			;restore AC bit in eflags


;processor is at least 486
;
;Check for ability to set/clear ID flag in EFLAGS
;ID flag indicates ability of processor to execute the CPUID instruction.
;486 not guaranteed to have CPUID inst?
;
		mov	[cputype],4
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

		mov	[cputype],al

@@end_get_cpu:	mov	al,[cputype]
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


		.data

CPUType		db	0
VendorID	db	"Not available",0,0,0,0,0,0


end

