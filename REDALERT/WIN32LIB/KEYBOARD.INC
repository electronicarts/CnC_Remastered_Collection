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
;*                    File Name : KEYBOARD.INC                             *
;*                                                                         *
;*                   Programmer : Phil W. Gorrow                           *
;*                                                                         *
;*                   Start Date : July 12, 1994                            *
;*                                                                         *
;*                  Last Update : July 12, 1994   [PWG]                    *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Contains all the defines used by the keyboard interrupt for assembly    *
;*   includes.                                                             *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

TRUE		EQU	1			; numerical true
FALSE		EQU	0			; numerical false
DEBUG		EQU	1

MAX_X_PIXEL		EQU	319
MAX_Y_PIXEL		EQU	199


KN_RESERVED1		EQU	14
KN_RESERVED2		EQU	45
KN_RESERVED3		EQU	56


; these two are reserved for AMIGA open and close.
KN_LCOMM		EQU	59
KN_RCOMM		EQU	63

KN_LMOUSE		EQU	65
KN_RMOUSE		EQU	66
KN_JBUTTON1		EQU	67
KN_JBUTTON2		EQU	68
KN_J_UP			EQU	69
KN_J_RIGHT		EQU	70
KN_J_DOWN		EQU	71
KN_J_LEFT		EQU	72

KN_LEFT			EQU	92
KN_UPLEFT		EQU	91
KN_UP			EQU	96
KN_UPRIGHT		EQU	101
KN_RIGHT		EQU	102
KN_DOWNRIGHT		EQU	103
KN_DOWN			EQU	98
KN_DOWNLEFT		EQU	93
KN_CENTER		EQU	97

KN_INSERT		EQU	99
KN_DELETE		EQU	104

KN_RETURN		EQU	43
KN_SPACE		EQU	61
KN_KEYPAD_RETURN	EQU	108


; these two are reserved for AMIGA K left and right paren
KN_KLPAREN		EQU	87
KN_KRPAREN		EQU	88


KN_NUMLOCK		EQU	90

KN_SCROLLOCK		EQU	125		; key ignored by the logging system

KN_MOUSE_MOVE	EQU	45	; Indicate a mouse move (for playback of logged data)

; ----------------------------------------------------------------
; flags used in Keyflags to customize keystroke interrupt.

REPEATON	EQU	0001H	; 1:all makes into buffer, 0:only 1st make
TRACKEXT	EQU	0002H	; 1:Home != keypad Home, 0:Home=keypad Home
FILTERONLY	EQU	0004H	; 1:Normal BIOS operation with filter
CTRLSON		EQU	0008H	; 1:pass scroll lock sequence into BIOS
CTRLALTTURBO	EQU	0010H	; 1:Allow turbo up and down in application
CTRLCON		EQU	0020H	; 1:pass stop code to BIOS
SCROLLLOCKON	EQU	0040H	; 1:pass scroll lock key into BIOS
PAUSEON		EQU	0080H	; 1:pass the pause key and seq to BIOS
BREAKON		EQU	0100H	; 1:pass the ctrl break seq to BIOS
NONUMLOCK	EQU	0200H	; 1:do NOT remap keypad to numbers
TASKSWITCHABLE	EQU	0400H	; 1:allows task switching keys thru ALT-TAB,
				;   ALT-ESC,CTRL-ESC
PASSBREAKS	EQU	0800H	; 1:Pass all break codes to keyboard buffer.
KEYMOUSE	EQU	1000H	; 1:Numeric keypad moves mouse
SIMLBUTTON	EQU	2000H	; 1:have space and enter keys simulate Left
DEBUGINT	EQU	4000H	;   mouse button when KEYMOUSE is set


SHIFTPRESS	EQU	001H	; bit 0 for shift key pressed
CTRLPRESS	EQU	002H	; bit 1 for ctrl key pressed
ALTPRESS	EQU	004H	; bit 2 for alt key pressed
KEYRELEASE	EQU	008H	; bit 3 for key released
NOTKEYRELEASE	EQU	0F7H	; not of key released

CAPSLOCK	EQU	00001H	; bit 0 for caps locked
NUMLOCK	EQU	00002H	; bit 1 for num locked



CLEARISR	EQU	020H	; value to clear In Service Register
DOS		EQU 	021H
INTCHIP0	EQU	020H	; 8259 interrupt chip controller 0
KEYCTRL		EQU	061H	; control bits for KB sense data
KEYDATA		EQU	060H	; keyboard scan code port
