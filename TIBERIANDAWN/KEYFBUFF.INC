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



;**************************************************************
;
; Macro to fetch the header of the next line and jump to the appropriate routine
;
next_line	macro

		add	edi,[dest_adjust_width]		;add in dest modulo
		dec	edx				;line counter
		jz	real_out			;return
		mov	ecx,[save_ecx]			;ecx is pixel count
		mov	eax,[header_pointer]		;ptr to next header byte
		mov	al,[eax]
		inc	[header_pointer]
		and	eax,BLIT_ALL			;Make sure we dont jump to some spurious address
							; if the header is wrong then its better to draw with the wrong
							; shape routine than to die
		shl	eax,2
		add	eax,[ShapeJumpTableAddress]	;get the address to jump to
		jmp	[dword eax]			;do the jump

		endm

