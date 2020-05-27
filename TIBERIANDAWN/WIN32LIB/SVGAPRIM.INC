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
;**      C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S      **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood 32 bit Library                  *
;*                                                                         *
;*                    File Name : SVGAPRIM.INC                             *
;*                                                                         *
;*                   Programmer : Phil W. Gorrow                           *
;*                                                                         *
;*                   Start Date : December 8, 1994                         *
;*                                                                         *
;*                  Last Update : December 8, 1994   [PWG]                 *
;*                                                                         *
;*-------------------------------------------------------------------------*

; Externs from VIDEO.CPP module of the VIDEO library
GLOBAL BankTable			:DWORD
GLOBAL VesaFunc			:DWORD
GLOBAL XRes					:DWORD
GLOBAL YRes					:DWORD
GLOBAL CurrentMode		:DWORD
global cpu_video_page	:dword
global cpu_page_limit   :dword


; Externs from VESA.ASM module of the SVGAPRIM library
GLOBAL Vesa_Asm_Set_Win		:NEAR
GLOBAL Vesa_Asm_Next_Win		:NEAR

; Externs from VGETPIX.ASM module of the SVGA/MCGAPRIM library
GLOBAL Vesa_Get_Pixel          	:NEAR

; Externs from VPUTTPIX.ASM module of the SVGA/MCGAPRIM library
GLOBAL Vesa_Put_Pixel 		:NEAR


; Externs from VCLEAR.ASM module of the MCGA/SVGAPRIM library
GLOBAL Vesa_Clear			:NEAR

; Externs from VBITBLIT.ASM module of the MCGA/SVGAPRIM library
GLOBAL Linear_Blit_To_Vesa		:NEAR
GLOBAL Vesa_Blit_To_Linear		:NEAR
GLOBAL Vesa_Blit_To_Vesa		:NEAR

; Externs from VTOBUFF.ASM module of the SVGA/MCGAPRIM library
GLOBAL Vesa_To_Buffer		:NEAR

; Externs from VTOPAGE.ASM module of the SVGA/MCGAPRIM library
GLOBAL Vesa_Buffer_To_Page		:NEAR

; Externs from VSCALE.ASM module of the SVGA/MCGAPRIM library
GLOBAL Linear_Scale_To_Vesa 		:NEAR
GLOBAL Vesa_Scale_To_Linear		:NEAR
GLOBAL Vesa_Scale_To_Vesa 		:NEAR


; Externs from VSCALE.ASM module of the SVGA/MCGAPRIM library
GLOBAL Vesa_Print	 		:NEAR
