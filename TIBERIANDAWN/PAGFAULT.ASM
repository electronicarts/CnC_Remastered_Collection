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
;*                 Project Name : Library                                  *
;*                                                                         *
;*                    File Name : PAGFAULT.ASM                             *
;*                                                                         *
;*                   Programmer : Julio R Jerez 	                   *
;*                                                                         *
;*                   	   Date : April 25,1995                            *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;
; Here are prototypes for the routines defined within this module:
; VOID Install_Page_Fault_Handle (void) ;
;
; ----------------------------------------------------------------

IDEAL				; the product runs in ideal mode
P386				; use 386 real mode instructions
MODEL USE32 FLAT
LOCALS ??			; ?? is the symbol for a local
WARN				; generate all warnings we can
JUMPS				; optimize jumps if possible

;---------------------------------------------------------------------------
; Make some general equates for easy compatability
;---------------------------------------------------------------------------
DPMI_INTR	 EQU	31h
PAGE_FAULT	 equ	0eh
DIVIDE_ERROR	 equ	00h
RESET_VIDEO_MODE  equ	-1

global C	 Install_Page_Fault_Handle : NEAR
global C	 Set_Video_Mode	     : NEAR
global C	 Remove_Mouse		     : NEAR
global C	 Remove_Keyboard_Interrupt : NEAR
global C	 Remove_Timer_Interrupt    : NEAR

DATASEG
	Old_Page_Fault_handle DF ?
	Page_Fault_SS 	DD	?
	Page_Fault_ESP	DD	?

	Old_Div_Error_handle DF	?
	Div_Error_SS 	DD	?
	Div_Error_ESP	DD	?


CODESEG
;***************************************************************************
;* INSTALL_PAGE_FAULT_HANDLE -- Installs new page fault and div Error	   *
;* 				handles.				   *
;*	This function will install a new page fault handle and Div Error   *
;*	so in the event that we have a program crash these handles will	   *
;*	remove all interrupts and then will chain to the default Exception *
;*	Handle							   	   *
;*									   *
;* INPUT:	none							   *
;*									   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* PROTO:	VOID Install_Page_Fault_Handle( void);  		   *
;*                                                                         *
;* HISTORY:  04/25/96 Created                                              *
;*=========================================================================*
	PROC	Install_Page_Fault_Handle C NEAR
	USES	eax,ebx,ecx,edx,esi,edi

	; Install_Page_Fault_Handle
	mov	eax,0202h 	; get address of exception handle
	mov	bl,PAGE_FAULT
	int	DPMI_INTR
	jc	??exit		; not action is taken

	; save addrees of default handle
	mov	[dword ptr Old_Page_Fault_handle],edx
	mov	[word ptr  Old_Page_Fault_handle+4],cx

	; redirect default handle to a new Page Fault Handle
	mov eax,0203h
	mov bl,PAGE_FAULT
	mov cx,cs
	lea edx,[Page_Fault_Handle]
	int DPMI_INTR


	; Install_Divide_Error_Handle
	mov	eax,0202h 	; get address of exception handle
	mov	bl,DIVIDE_ERROR
	int	DPMI_INTR
	jc	??exit		; not action is taken

	; save addrees of default fault handle
	mov	[dword ptr Old_Div_Error_handle],edx
	mov	[word ptr  Old_Div_Error_handle+4],cx

	; redirect default handle to a new Divede Handle
	mov eax,0203h
	mov bl,DIVIDE_ERROR
	mov cx,cs
	lea edx,[Divide_Error_Handle]
	int DPMI_INTR

  ??exit:


	ret
	ENDP	Install_Page_Fault_Handle


;***************************************************************************
;* PAGE_FAULT_HANDLE --  This           		   		   *
;*                                                                         *
;*                                                                         *
;*                                                                         *
;* HISTORY:  04/25/96 Created                                              *
;*=========================================================================*
PROC	Page_Fault_Handle far

	; preserve used  registers
	push	eax
	push	ebx
	; save Page Fault satck frame
	mov	ax,ss
	mov	[Page_Fault_SS],eax
	mov	[Page_Fault_ESP],esp

	; retrieve application original stack frame
	mov eax , [ esp + ( 6 + 2 ) * 4 ]
	mov ebx , [ esp + ( 7 + 2 ) * 4 ]
	mov ss , bx
	mov esp , eax

	; set video mode to standard text mode
	push	RESET_VIDEO_MODE
	call 	Set_Video_Mode
	pop	eax
	call 	Remove_Mouse
	call 	Remove_Keyboard_Interrupt
	call	Remove_Timer_Interrupt

	; restore Page Fault stack frame
	mov	eax,[Page_Fault_SS]
	mov 	ss , ax
	mov	esp, [Page_Fault_ESP]

	; restore used registers and chain to default Page Fault Handle
	pop	ebx
	pop	eax
	jmp	[fword Old_Page_Fault_handle]

	ENDP	Page_Fault_Handle



;***************************************************************************
;* Divide_Error --  							   *
;*                                                                         *
;*                                                                         *
;*                                                                         *
;* HISTORY:  04/25/96 Created                                              *
;*=========================================================================*
PROC	Divide_Error_Handle far

	; preserve used  registers
	push	eax
	push	ebx
	; save Page Fault satck frame
	mov	ax,ss
	mov	[Div_Error_SS],eax
	mov	[Div_Error_ESP],esp

	; retrieve application original stack frame
	mov eax , [ esp + ( 6 + 2 ) * 4 ]
	mov ebx , [ esp + ( 7 + 2 ) * 4 ]
	mov ss , bx
	mov esp , eax

	; set video mode to standard text mode
	push	RESET_VIDEO_MODE
	call 	Set_Video_Mode
	pop	eax
	call 	Remove_Mouse
	call 	Remove_Keyboard_Interrupt
	call	Remove_Timer_Interrupt

	; restore Fault stack frame
	mov	eax,[Div_Error_SS]
	mov 	ss , ax
	mov	esp, [Div_Error_ESP]

	; restore used registers and chain to default Page Fault Handle
	pop	ebx
	pop	eax
	jmp	[fword Old_Div_Error_handle]

	ENDP	Divide_Error_Handle


;***************************************************************************
;* End of File.								   *
;***************************************************************************
END
