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
;*                 Project Name : Westwood 32 bit Library                  *
;*                                                                         *
;*                    File Name : KEYSTRUC.INC                             *
;*                                                                         *
;*                   Programmer : Phil W. Gorrow                           *
;*                                                                         *
;*                   Start Date : July 13, 1994                            *
;*                                                                         *
;*                  Last Update : July 13, 1994   [PWG]                    *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

STRUC	KeyboardType
	SoundOn			DW	?	; toggled by alt S
	MusicOn			DW	?	; toggled by alt M
	KeyFlags			DD	?	; all but repeat for now
	Break				DW	?
	KeyMouseMove	DB	6  DUP(?)
	ScreenEdge		DW	18 DUP (?)
	Bits				DB	8  DUP (?)
	CondPassKey		DW	17 DUP (?) 
	CondPassCond	DW	17 DUP (?)
	EscRoutine		DD	?
	ExtCodes			DB	16 DUP (?)
	ExtNums			DB	16 DUP (?)
	ExtRemap			DB	16 DUP (?)
	ExtRemapEnd		DB	?
	ExtKeyboard		DB	?
	KeyBuffer		DW	128 DUP(?) ; set to empty
	KeyBufferHead	DD	?	; set to first entry
	KeyBufferTail	DD	?	; set to head for empty buffer
	KeyLock			DW	?	; num and caps lock bits
	KeyNums			DB	89 DUP (?)
	KeysCapsLock	DB	16 DUP (?)
	KeysNumLock		DB	16 DUP (?)
	KeysUpDown		DB	16 DUP (?)
	KeyStream		DB	16 DUP (?)
	PassCount		DW	?
	KeyStreamIndex	DW	?
	LastKeyE0		DB	?
	LastKeyE1		DB	?
	PassAlways		DB	10 DUP (?)
	PassAlwaysEnd	DB	?		; invalid code to END PassAlways
	CtrlFlags		DB	?
	Buffer			DW	?
	Time				DW	?
	XYAdjust			DB	26 DUP (?)
	EdgeConv			DW	16 DUP (?)
	MouseUpdate		DW	?
	MouseX			DD	?
	LocalMouseX		DW	?
	MouseY			DD	?
	LocalMouseY		DW	?
	IsExtKey			DB	?
	ExtIndex			DW	?

	KeyOldRMI			DD	?	; The origianl RM interrupt seg:off.
	KeyOldPMIOffset	DD	?	; The origianl PM interrupt offset		
	KeyOldPMISelector	DD	?	; The original PM interrupt segment.

	KeyCodeOffset			DW	?	; Offset of the code in the RM stuff.
	CallKeyRMIntOffset	DW	?	; Offset of function to call DOS timer interrupt.
	CallKeyRMIntAddr		DD	?	; PM address of CallRealIntOffset for speed.
	PMIssuedKeyInt			DD	?	; did Protected mode pass this through?

	BrkOldRMI				DD	?	; The origianl RM interrupt seg:off.
	BrkOldPMIOffset		DD	?	; The origianl PM interrupt offset		
	BrkOldPMISelector		DD	?	; The original PM interrupt segment.

	BrkCodeOffset			DW	?	; Offset of the code in the RM stuff.
	CallBrkRMIntOffset	DW	?	; Offset of function to call DOS timer interrupt.
	CallBrkRMIntAddr		DD	?	; PM address of CallRealIntOffset for speed.
	PMIssuedBrkInt			DD	?	; did Protected mode pass this through?
	KeyIntDisabled			DD	?

	DbgOldPMIOffset		DD	?	; The origianl PM interrupt offset		
	DbgOldPMISelector		DD	?	; The original PM interrupt segment.

	;---------------------------------------------------------------------------
	; Begin definition of Mouse Specific Variables for real mode
	;---------------------------------------------------------------------------
	Button				DB	?	; current value of the mouse button
	MDisabled			DB	?	; Is the mouse driver disabled
	MInput				DB	?	; Defaults to mouse input allowed.
	Adjust				DW	?	; flag to adjust coordinates if necessary
	MouseStepX			DW	?	; step values if the mouse moves at
	MouseStepY			DW	?	;   more than one pixel at a time
	MouseOffsetX		DW	?	; Fractional step values used if a mouse
	MouseOffsetY		DW	?	;   moves at less than one pixel at a time
	MState				DD	?	; Tracks if mouse is hidden (TRUE) or not (FALSE)
	MouseXOld     		DW	?	; Holds last MouseX and MouseY to determine if 
	MouseYOld     		DW	?	;   mouse needs to be redrawn
	MCState				DW	?	; Tracks if mouse conditional hidden (TRUE) or not
	MouseCXLeft			DD	?	; Conditional hide mouse left x position
	MouseCYUpper		DD	?	; Conditional hide mouse top y position
	MouseCXRight		DD	?	; Conditional hide mouse right x position
	MouseCYLower		DD	?	; Conditional hide mouse lower y position
	MouseCursor   		DD	?	; Pointer to the mouse cursor to draw
	MouseCursorSize	DW	?	; Size of mouse cursor draw area
	MouseBuffer   		DD	?	; Pointer to buffer mouse is saved in
	MouseXHot     		DD	?	; Offset to mouse's x hot spot
	MouseYHot     		DD	?	; Offset to mouse's y hot spot
	MouseBuffX			DD	?	; X position background was saved at
	MouseBuffY			DD	?	; Y position background was saved at
	MouseBuffW			DD	?	; Width of the region saved for mouse
	MouseBuffH			DD	?	; Height of the region saved for mouse
	MouseWidth			DD	?	; Mouse cursor theoretical width
	MouseHeight			DD	?	; Mouse cursor theoretical height
	MouseCodeOffset	DW	?	; Offset to the real mode code offset
	MouseRight			DD	?	; Right hand side of the screen
	MouseBottom			DD	?	; Bottom of the screen


	ShadowPtr			dw	?
	DrawMousePtr 		dw	?

	VGAMouseDraw		dw	?
	VGAMouseShadow		dw	?

	VESAMouseDraw		dw	?
	VESAMouseShadow	dw	?

	VesaPtr				dd ?	
	VesaBankTable	   DD	8 dup (?)
	Adjust_XPos			dd ?
	Adjust_YPos			dd ?


ENDS

; InitFlags that are set to have a fully functional interrupt.
IF_ALLOC_RM			equ	1		; Allocation of RM was successful.
IF_SET_VECTORS		equ	2		; Vectors have been set.
IF_LOCKED_PM_CODE	equ	4		; Locked PM code for DPMI.
IF_LOCKED_PM_DATA	equ	8		; Locked PM code for DPMI.
IF_RATE_CHANGE		equ	10		; Timer rate was changed.
IF_FUNCTIONAL		equ	20		; Timer is in and functional.
IF_LOCKED_RM_CODE	equ	40
