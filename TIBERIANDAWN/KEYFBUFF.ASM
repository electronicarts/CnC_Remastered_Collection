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
;*                 Project Name : Command & Conquer                        *
;*                                                                         *
;*                    File Name : KEYFBUFF.ASM                             *
;*                                                                         *
;*                   Programmer : David R. Dettmer                         *
;*                                                                         *
;*                   Start Date : March 3, 1995                            *
;*                                                                         *
;*                  Last Update :                                          *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Buffer_Frame_To_Page -- Copies a linear buffer to a virtual viewport  *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

;********************** Model & Processor Directives ***********************
;IDEAL
;P386
;MODEL USE32 FLAT
;jumps

.MODEL FLAT
;.386

;******************************** Includes *********************************
;INCLUDE "gbuffer.inc"
;include	"profile.inc"

OPTIMAL_BYTE_COPY	equ	14

GraphicViewPort STRUCT 
GVPOffset		DD		?		; offset to virtual viewport
GVPWidth			DD		?		; width of virtual viewport
GVPHeight		DD		?		; height of virtual viewport
GVPXAdd			DD		?		; x mod to get to next line
GVPXPos			DD		?		; x pos relative to Graphic Buff
GVPYPos			DD		?		; y pos relative to Graphic Buff
GVPPitch		dd		?		; modulo of graphic view port
GVPBuffPtr		DD		?		; ptr to associated Graphic Buff
GraphicViewPort ENDS

;******************************** Equates **********************************

TRUE	equ	1			; Boolean 'true' value
FALSE	equ	0			; Boolean 'false' value

;*=========================================================================*/
;* The following are defines used to control what functions are linked	   *
;* in for Buffer_Frame_To_Page.						   *
;*=========================================================================*/
;USE_NORMAL		EQU	TRUE
;USE_HORZ_REV 		EQU	TRUE
;USE_VERT_REV 		EQU	TRUE
;USE_SCALING 		EQU	TRUE


FLAG_NORMAL		EQU	0
FLAG_TRANS		EQU	1
FLAG_GHOST		EQU	2
FLAG_FADING		EQU	4
FLAG_PREDATOR		EQU	8

FLAG_MASK		EQU	0Fh


SHAPE_NORMAL 		EQU	0000h		; Standard shape
;SHAPE_HORZ_REV 		EQU	0001h		; Flipped horizontally
;SHAPE_VERT_REV 		EQU	0002h		; Flipped vertically
;SHAPE_SCALING 		EQU	0004h		; Scaled (WORD scale_x, WORD scale_y)
;SHAPE_VIEWPORT_REL 	EQU	0010h		; Coords are window-relative
;SHAPE_WIN_REL 		EQU	0010h		; Coordinates are window relative instead of absolute.
SHAPE_CENTER 		EQU	0020h		; Coords are based on shape's center pt
SHAPE_TRANS		EQU	0040h		; has transparency

SHAPE_FADING 		EQU	0100h		; Fading effect (VOID * fading_table,
						;   WORD fading_num)
SHAPE_PREDATOR 		EQU	0200h		; Transparent warping effect
;SHAPE_COMPACT 		EQU	0400h		; Never use this bit
;SHAPE_PRIORITY 		EQU	0800h		; Use priority system when drawing
SHAPE_GHOST		EQU	1000h		; Shape is drawn ghosted
;SHAPE_SHADOW		EQU	2000h
SHAPE_PARTIAL  		EQU	4000h
;SHAPE_COLOR 		EQU	8000h		; Remap the shape's colors
						;   (VOID * color_table)


;
;.......................... Shadow Effect ..................................
;
SHADOW_COL		EQU	00FFh	; magic number for shadows

;......................... Priority System .................................
;
CLEAR_UNUSED_BITS  	EQU	0007h	; and with 0000-0111 to clear
					;  non-walkable high bit and
					;  scaling id bits
NON_WALKABLE_BIT  	EQU	0080h	; and with 1000-0000 to clear all
					;  but non-walkable bit
;
;......................... Predator Effect .................................
;
;PRED_MASK		EQU	0007h	; mask used for predator pixel puts
PRED_MASK		EQU	000Eh	; mask used for predator pixel puts


;---------------------------------------------------------------------------
;
; Use a macro to make code a little cleaner.
; The parameter varname is optional.
; Syntax to use macro is :
;  WANT equ expression
;  USE func [,varname]
; If the 'varname' is defined, a table declaration is created like:
;	GLOBAL	TableName:DWORD
; Then, the table entry is created:
;  If WANT is true, the table entry is created for the given function:
;	varname	DD	func
;  If WANT is not TRUE, a Not_Supported entry is put in the table:
;	varname	DD	Not_Supported
; The resulting tables look like:
;
;	GLOBAL	ExampTable:DWORD
;	ExampTable	DD	routine1
;			DD	routine2
;			DD	routine3
;			...
; Thus, each table is an array of function pointers.
;
;---------------------------------------------------------------------------
USE MACRO  func, varname
 IF WANT
  varname 	DD	func
 ELSE
  varname	DD	Not_Supported
 ENDIF
ENDM

prologue	macro
endm



epilogue macro 
endm


; IFNB <varname>
;	GLOBAL	varname:DWORD
; ENDIF

;---------------------------------------------------------------------------


.DATA

;---------------------------------------------------------------------------
; Predator effect variables
;---------------------------------------------------------------------------
; make table for negative offset and use the used space for other variables

BFPredNegTable	DW	-1, -3, -2, -5, -2, -4, -3, -1
	; 8 words below calculated
		DW	0, 0, 0, 0, 0, 0, 0, 0	; index ffffff00
		DD	0, 0, 0, 0		; index ffffff10
BFPredOffset	DD	0, 0, 0, 0		; index ffffff20
		DD	0, 0, 0, 0		; index ffffff30
	; partially faded predator effect value
BFPartialPred	DD	0, 0, 0, 0		; index ffffff40
BFPartialCount	DD	0, 0, 0, 0		; index ffffff50
		DD	0, 0, 0, 0		; index ffffff60
		DD	0, 0, 0, 0		; index ffffff70
		DD	0, 0, 0, 0		; index ffffff80
		DD	0, 0, 0, 0		; index ffffff90
		DD	0, 0, 0, 0		; index ffffffa0
		DD	0, 0, 0, 0		; index ffffffb0
		DD	0, 0, 0, 0		; index ffffffc0
		DD	0, 0, 0, 0		; index ffffffd0
		DD	0, 0, 0, 0		; index ffffffe0
		DD	0, 0, 0, 0		; index fffffff0
BFPredTable	DW	1, 3, 2, 5, 2, 3, 4, 1
;BFPredTable	DB	1, 3, 2, 5, 4, 3, 2, 1






		extern	C BigShapeBufferStart:dword
		extern	C UseBigShapeBuffer:dword
		extern	C TheaterShapeBufferStart:dword
		extern	C IsTheaterShape:dword
		;extern	C Single_Line_Trans_Entry:near
		;extern	C Next_Line:near
		extern	C MMX_Done:near
		extern	C MMXAvailable:dword
		;extern	C EndNewShapeJumpTable:byte
		;extern	C NewShapeJumpTable:dword


;**********************************************************************************
;
; Jump tables for new line header system
;
; Each line of shape data now has a header byte which describes the data on the line.
;

;
; Header byte control bits
;
BLIT_TRANSPARENT	=1
BLIT_GHOST		=2
BLIT_FADING		=4
BLIT_PREDATOR		=8
BLIT_SKIP		=16
BLIT_ALL		=BLIT_TRANSPARENT or BLIT_GHOST or BLIT_FADING or BLIT_PREDATOR or BLIT_SKIP


ShapeHeaderType	STRUCT

		draw_flags	dd	?
		shape_data	dd	?
		shape_buffer	dd	?

ShapeHeaderType	ENDS



;
; Global definitions for routines that draw a single line of a shape
;
		;extern	Short_Single_Line_Copy:near
		;extern	Single_Line_Trans:near
		;extern	Single_Line_Ghost:near
		;extern	Single_Line_Ghost_Trans:near
		;extern	Single_Line_Fading:near
		;extern	Single_Line_Fading_Trans:near
		;extern	Single_Line_Ghost_Fading:near
		;extern	Single_Line_Ghost_Fading_Trans:near
		;extern	Single_Line_Predator:near
		;extern	Single_Line_Predator_Trans:near
		;extern	Single_Line_Predator_Ghost:near
		;extern	Single_Line_Predator_Ghost_Trans:near
		;extern	Single_Line_Predator_Fading:near
		;extern	Single_Line_Predator_Fading_Trans:near
		;extern	Single_Line_Predator_Ghost_Fading:near
		;extern	Single_Line_Predator_Ghost_Fading_Trans:near
		;extern	Single_Line_Skip:near

		;extern	Single_Line_Single_Fade:near
		;extern	Single_Line_Single_Fade_Trans:near

;externdef AllFlagsJumpTable:dword
;
externdef NewShapeJumpTable:dword
externdef EndNewShapeJumpTable:byte
externdef CriticalFadeRedirections:dword
;externdef BufferFrameTable:dword

;externdef CriticalFadeRedirections:dword
;externdef CriticalFadeRedirections:dword
;externdef CriticalFadeRedirections:dword
;externdef BF_Copy:far ptr
;externdef BF_Trans:dword
;externdef BF_Ghost:dword
;externdef BF_Ghost_Trans:dword
;externdef BF_Fading:dword
;externdef BF_Fading_Trans:dword
;externdef BF_Ghost_Fading:dword
;externdef BF_Ghost_Fading_Trans:dword
;externdef BF_Predator:dword
;externdef BF_Predator_Trans:dword
;externdef BF_Predator_Ghost:dword
;externdef BF_Predator_Ghost_Trans:dword
;externdef BF_Predator_Fading:dword
;externdef BF_Predator_Fading_Trans:dword
;externdef BF_Predator_Ghost_Fading:dword
;externdef BF_Predator_Ghost_Fading_Trans:dword

externdef	C Single_Line_Trans:near
externdef	C Single_Line_Trans_Entry:near
externdef	C Next_Line:near


.CODE


;---------------------------------------------------------------------------
; Code Segment Tables:
; This code uses the USE macro to set up tables of function addresses.
; The tables have the following format:
; Tables defined are:
;	BufferFrameTable
;---------------------------------------------------------------------------

WANT	equ 	<TRUE>
USE	BF_Copy, BufferFrameTable

WANT	equ 	<TRUE>
USE	BF_Trans

WANT	equ 	<TRUE>
USE	BF_Ghost

WANT	equ 	<TRUE>
USE	BF_Ghost_Trans

WANT	equ 	<TRUE>
USE	BF_Fading

WANT	equ 	<TRUE>
USE	BF_Fading_Trans

WANT	equ 	<TRUE>
USE	BF_Ghost_Fading

WANT	equ 	<TRUE>
USE	BF_Ghost_Fading_Trans

WANT	equ 	<TRUE>
USE	BF_Predator

WANT	equ 	<TRUE>
USE	BF_Predator_Trans

WANT	equ 	<TRUE>
USE	BF_Predator_Ghost

WANT	equ 	<TRUE>
USE	BF_Predator_Ghost_Trans

WANT	equ 	<TRUE>
USE	BF_Predator_Fading

WANT	equ 	<TRUE>
USE	BF_Predator_Fading_Trans

WANT	equ 	<TRUE>
USE	BF_Predator_Ghost_Fading

WANT	equ 	<TRUE>
USE	BF_Predator_Ghost_Fading_Trans



.DATA

;NewShapeJumpTable label	near ptr		dword

;
; Jumptable for shape line drawing with no flags set
;

NewShapeJumpTable	dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
;CriticalFadeRedirections label		 dword
CriticalFadeRedirections		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


;
; Jumptable for shape line drawing routines with transparent flags set
;
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


;
; Jumptable for shape line drawing routines with ghost flags set
;

		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


;
; Jumptable for shape line drawing routines with ghost and transparent flags set
;

		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost_Trans
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip





;
; Jumptable for shape line drawing routines with fading flag set
;

		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Single_Fade
		dd	Single_Line_Single_Fade
		dd	Single_Line_Fading
		dd	Single_Line_Fading
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Fading
		dd	Single_Line_Fading
		dd	Single_Line_Fading
		dd	Single_Line_Fading
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


;
; Jumptable for shape line drawing routines with fading and transparent flags set
;

		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Single_Fade
		dd	Single_Line_Single_Fade_Trans
		dd	Single_Line_Fading
		dd	Single_Line_Fading_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Fading
		dd	Single_Line_Fading_Trans
		dd	Single_Line_Fading
		dd	Single_Line_Fading_Trans
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


;
; Jumptable for shape line drawing routines with fading and ghost flags set
;

		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost
		dd	Single_Line_Single_Fade
		dd	Single_Line_Single_Fade
		dd	Single_Line_Ghost_Fading
		dd	Single_Line_Ghost_Fading
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost
		dd	Single_Line_Fading
		dd	Single_Line_Fading
		dd	Single_Line_Ghost_Fading
		dd	Single_Line_Ghost_Fading
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip



;
; Jumptable for shape line drawing routines with fading, transparent and ghost flags set
;

		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost_Trans
		dd	Single_Line_Single_Fade
		dd	Single_Line_Single_Fade_Trans
		dd	Single_Line_Ghost_Fading
		dd	Single_Line_Ghost_Fading_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost_Trans
		dd	Single_Line_Fading
		dd	Single_Line_Fading_Trans
		dd	Single_Line_Ghost_Fading
		dd	Single_Line_Ghost_Fading_Trans
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip






;
; Jumptable for shape line drawing with predator flag set
;

		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


;
; Jumptable for shape line drawing routines with transparent and predator flags set
;
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Trans
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Trans
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Trans
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Trans
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


;
; Jumptable for shape line drawing routines with ghost and predator flags set
;

		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Ghost
		dd	Single_Line_Predator_Ghost
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Ghost
		dd	Single_Line_Predator_Ghost
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


;
; Jumptable for shape line drawing routines with ghost and transparent and predator flags set
;

		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost_Trans
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Trans
		dd	Single_Line_Predator_Ghost
		dd	Single_Line_Predator_Ghost_Trans
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Trans
		dd	Single_Line_Predator_Ghost
		dd	Single_Line_Predator_Ghost_Trans
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip





;
; Jumptable for shape line drawing routines with fading and predator flags set
;

		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Single_Fade
		dd	Single_Line_Single_Fade
		dd	Single_Line_Fading
		dd	Single_Line_Fading
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Fading
		dd	Single_Line_Predator_Fading
		dd	Single_Line_Predator_Fading
		dd	Single_Line_Predator_Fading
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


;
; Jumptable for shape line drawing routines with fading and transparent and predator flags set
;

		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Single_Fade
		dd	Single_Line_Single_Fade_Trans
		dd	Single_Line_Fading
		dd	Single_Line_Fading_Trans
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Trans
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Trans
		dd	Single_Line_Predator_Fading
		dd	Single_Line_Predator_Fading_Trans
		dd	Single_Line_Predator_Fading
		dd	Single_Line_Predator_Fading_Trans
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


;
; Jumptable for shape line drawing routines with fading and ghost and predator flags set
;

		dd	Short_Single_Line_Copy
		dd	Short_Single_Line_Copy
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost
		dd	Single_Line_Single_Fade
		dd	Single_Line_Single_Fade
		dd	Single_Line_Ghost_Fading
		dd	Single_Line_Ghost_Fading
		dd	Single_Line_Predator
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Ghost
		dd	Single_Line_Predator_Ghost
		dd	Single_Line_Predator_Fading
		dd	Single_Line_Predator_Fading
		dd	Single_Line_Predator_Ghost_Fading
		dd	Single_Line_Predator_Ghost_Fading
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip







;
; Jumptable for shape line drawing routines with all flags set
;

AllFlagsJumpTable label dword
		dd	Short_Single_Line_Copy
		dd	Single_Line_Trans
		dd	Single_Line_Ghost
		dd	Single_Line_Ghost_Trans
		dd	Single_Line_Single_Fade
		dd	Single_Line_Single_Fade_Trans
		dd	Single_Line_Ghost_Fading
		dd	Single_Line_Ghost_Fading_Trans
		dd	Single_Line_Predator
		dd	Single_Line_Predator_Trans
		dd	Single_Line_Predator_Ghost
		dd	Single_Line_Predator_Ghost_Trans
		dd	Single_Line_Predator_Fading
		dd	Single_Line_Predator_Fading_Trans
		dd	Single_Line_Predator_Ghost_Fading
		dd	Single_Line_Predator_Ghost_Fading_Trans
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip
		dd	Single_Line_Skip


EndNewShapeJumpTable	db 0

.CODE



;---------------------------------------------------------------------------




;*********************************************************************************************
;* Set_Shape_Header -- create the line header bytes for a shape                              *
;*                                                                                           *
;* INPUT:	Shape width                                                                  *
;*              Shape height                                                                 *
;*              ptr to raw shape data                                                        *
;*              ptr to shape headers                                                         *
;*              shape flags                                                                  *
;*              ptr to translucency table                                                    *
;*              IsTranslucent                                                                *
;*                                                                                           *
;* OUTPUT:      none                                                                         *
;*                                                                                           *
;* Warnings:                                                                                 *
;*                                                                                           *
;* HISTORY:                                                                                  *
;*   11/29/95 10:09AM ST : Created.                                                          *
;*===========================================================================================*

		Setup_Shape_Header proc	C pixel_width:DWORD, pixel_height:DWORD, src:DWORD, headers:DWORD, flags:DWORD, Translucent:DWORD, IsTranslucent:DWORD

		;ARG	pixel_width 	:DWORD		; width of rectangle to blit
		;ARG	pixel_height	:DWORD		; height of rectangle to blit
		;ARG  	src         	:DWORD		; this is a member function
		;ARG	headers	    	:DWORD
		;ARG	flags		:DWORD
		;ARG	Translucent	:DWORD
		;ARG	IsTranslucent	:DWORD
		LOCAL	trans_count	:DWORD

		pushad


		mov	esi,[src]		;ptr to raw shape data
		mov	edi,[headers]		;ptr to headers we are going to set up
		mov	eax,[flags]
		and	eax,SHAPE_TRANS or SHAPE_FADING or SHAPE_PREDATOR or SHAPE_GHOST
		mov	[edi].ShapeHeaderType.draw_flags,eax 	;save old flags in header
		add	edi,size ShapeHeaderType
		mov	edx,[pixel_height]	;number of shape lines to scan

outer_loop:	mov	ecx,[pixel_width]	;number of pixels in shape line
		xor	bl,bl			;flag the we dont know anything about this line yet
		mov	[trans_count],0		;we havnt scanned any transparent pixels yet

;
; Scan one shape line to see what kind of data it contains
;
inner_loop:	xor	eax,eax
		mov	al,[esi]
		inc	esi

;
; Check for transparent pixel
;
		test	al,al
		jnz	not_transp
		test	[flags],SHAPE_TRANS
		jz	not_transp
		or	bl,BLIT_TRANSPARENT	;flag that pixel is transparent
		inc	[trans_count]		;keep count of the number of transparent pixels on the line
		jmp	end_lp

;
; Check for predator effect on this line
;
not_transp:	test	[flags],SHAPE_PREDATOR
		jz	not_pred
		or	bl,BLIT_PREDATOR

;
; Check for ghost effects
;
not_pred:	test	[flags],SHAPE_GHOST
		jz	not_ghost
		push	edi
		mov	edi,[IsTranslucent]
		cmp	byte ptr [edi+eax],-1
		pop	edi
		jz	not_ghost
		or	bl,BLIT_GHOST

;
; Check if fading is required
;
not_ghost:	test	[flags],SHAPE_FADING
		jz	end_lp
		or	bl,BLIT_FADING

end_lp:       dec	ecx
		jnz	inner_loop


;
; Interpret the info we have collected and decide which routine will be
; used to draw this line
;
		xor	bh,bh

		test	bl,BLIT_TRANSPARENT
		jz	no_transparencies
		or	bh,BLIT_TRANSPARENT
		mov	ecx,[pixel_width]
		cmp	ecx,[trans_count]
		jnz	not_all_trans

; all pixels in the line were transparent so we dont need to draw it at all
		mov	bh,BLIT_SKIP
		jmp	got_line_type

not_all_trans:
no_transparencies:
		mov	al,bl
		and	al,BLIT_PREDATOR
		or	bh,al
		mov	al,bl
		and	al,BLIT_GHOST
		or	bh,al
		mov	al,bl
		and	al,BLIT_FADING
		or	bh,al

;
; Save the line header and do the next line
;
got_line_type:mov	[edi],bh
		inc	edi

		dec	edx
		jnz	outer_loop


		popad
		ret

Setup_Shape_Header	endp	




;**************************************************************
;
; Macro to fetch the header of the next line and jump to the appropriate routine
;
next_line	macro

		add	edi , [ dest_adjust_width ]	;add in dest modulo
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
		jmp	dword ptr [eax]			;do the jump

		endm






;***************************************************************************
;* VVC::TOPAGE -- Copies a linear buffer to a virtual viewport		   *
;*                                                                         *
;* INPUT:	WORD	x_pixel		- x pixel on viewport to copy from *
;*		WORD	y_pixel 	- y pixel on viewport to copy from *
;*		WORD	pixel_width	- the width of copy region	   *
;*		WORD	pixel_height	- the height of copy region	   *
;*		BYTE *	src		- buffer to copy from		   *
;*		VVPC *  dest		- virtual viewport to copy to	   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* WARNINGS:    Coordinates and dimensions will be adjusted if they exceed *
;*	        the boundaries.  In the event that no adjustment is 	   *
;*	        possible this routine will abort.  If the size of the 	   *
;*		region to copy exceeds the size passed in for the buffer   *
;*		the routine will automatically abort.			   *
;*									   *
;* HISTORY:                                                                *
;*   06/15/1994 PWG : Created.                                             *
;*=========================================================================*
Buffer_Frame_To_Page proc C public USES eax ebx ecx edx esi edi x_pixel:DWORD, y_pixel:DWORD, pixel_width:DWORD, pixel_height:DWORD, src:DWORD, dest:DWORD, flags:DWORD

	;USES	eax,ebx,ecx,edx,esi,edi

	;*===================================================================
	;* define the arguements that our function takes.
	;*===================================================================
	;ARG	x_pixel     :DWORD		; x pixel position in source
	;ARG	y_pixel     :DWORD		; y pixel position in source
	;ARG	pixel_width :DWORD		; width of rectangle to blit
	;ARG	pixel_height:DWORD		; height of rectangle to blit
	;ARG    	src         :DWORD		; this is a member function
	;ARG	dest        :DWORD		; what are we blitting to

	;ARG	flags       :DWORD		; flags passed

	;*===================================================================
	; Define some locals so that we can handle things quickly
	;*===================================================================
	LOCAL	IsTranslucent		:DWORD	; ptr to the is_translucent table
	LOCAL	Translucent		:DWORD	; ptr to the actual translucent table
	LOCAL	FadingTable		:DWORD	; extracted fading table pointer
	LOCAL	FadingNum		:DWORD	; get the number of times to fade

	LOCAL	StashECX		:DWORD	; temp variable for ECX register

	LOCAL	jflags			:DWORD	; flags used to goto correct buff frame routine
	LOCAL	BufferFrameRout		:DWORD	; ptr to the buffer frame routine

	LOCAL	jmp_loc			:DWORD	; calculated jump location
	LOCAL	loop_cnt		:DWORD

	LOCAL 	x1_pixel		:DWORD
	LOCAL	y1_pixel		:DWORD
	LOCAL	scr_x			:DWORD
	LOCAL	scr_y			:DWORD
	LOCAL	dest_adjust_width	:DWORD
	LOCAL	scr_adjust_width	:DWORD
	LOCAL	header_pointer		:DWORD
	LOCAL	use_old_draw		:DWORD
	LOCAL	save_ecx		:DWORD
	LOCAL	ShapeJumpTableAddress	:DWORD
	LOCAL	shape_buffer_start	:DWORD

	prologue
	cmp	[ src ] , 0
	jz	real_out

	;
	; Save the line attributes pointers and
	; Modify the src pointer to point to the actual image
	;
	cmp	[UseBigShapeBuffer],0
	jz	do_args			;just use the old shape drawing system

	mov	edi,[src]
	mov	[header_pointer],edi

	mov	eax,[BigShapeBufferStart]
	cmp	[edi].ShapeHeaderType.shape_buffer,0
	jz	is_ordinary_shape
	mov	eax,[TheaterShapeBufferStart]
is_ordinary_shape:
	mov	[shape_buffer_start],eax

	mov	edi,[edi].ShapeHeaderType.shape_data
	add	edi,[shape_buffer_start]
	mov	[src],edi
	mov	[use_old_draw],0


	;====================================================================
	; Pull off optional arguments:
	; EDI is used as an offset from the 'flags' parameter, to point
	; to the optional argument currently being processed.
	;====================================================================
do_args:
	mov	edi , 4	 			; optional params start past flags
	mov	[ jflags ] , 0			; clear jump flags

check_centering:
	;-------------------------------------------------------------------
	; See if we need to center the frame
	;-------------------------------------------------------------------
	test	[ flags ] , SHAPE_CENTER	; does this need to be centered?
	je	check_trans			; if not the skip over this stuff

	mov	eax , [ pixel_width ]
	mov	ebx , [ pixel_height ]
	sar	eax , 1
	sar	ebx , 1
	sub	[ x_pixel ] , eax
	sub	[ y_pixel ] , ebx

check_trans:
	test	[ flags ] , SHAPE_TRANS
	jz	check_ghost

	or	[ jflags ] , FLAG_TRANS

	;--------------------------------------------------------------------
	; SHAPE_GHOST: DWORD is_translucent tbl
	;--------------------------------------------------------------------
check_ghost:
	test	[ flags ] , SHAPE_GHOST		; are we ghosting this shape
	jz	check_fading

	mov	eax , [ flags + edi ]
	or	[ jflags ] , FLAG_GHOST
	mov	[ IsTranslucent ] , eax		; save ptr to is_trans. tbl
	add	eax , 0100h			; add 256 for first table
	add	edi , 4				; next argument
	mov	[ Translucent ] , eax		; save ptr to translucent tbl



check_fading:
	;______________________________________________________________________
	; If this is the first time through for this shape then
	; set up the shape header
	;______________________________________________________________________
	pushad

	cmp	[UseBigShapeBuffer],0
	jz	new_shape

	mov	edi,[header_pointer]
	cmp	[edi].ShapeHeaderType.draw_flags,-1
	jz	setup_headers
	mov	eax,[flags]							 ;Redo the shape headers if this shape was
	and	eax,SHAPE_TRANS or SHAPE_FADING or SHAPE_PREDATOR or SHAPE_GHOST ;initially set up with different flags
	cmp	eax,[edi].ShapeHeaderType.draw_flags
	jz	no_header_setup
new_shape:
	mov	[use_old_draw],1
	jmp	no_header_setup

setup_headers:
	push	[IsTranslucent]
	push	[Translucent]
	push	[flags]
	push	[header_pointer]
	push	[src]
	push	[pixel_height]
	push	[pixel_width]
	call	Setup_Shape_Header
	add	esp,7*4
	mov	[ShapeJumpTableAddress], offset AllFlagsJumpTable
	jmp	headers_set
no_header_setup:

	xor	eax,eax
	test	[flags],SHAPE_PREDATOR
	jz	not_shape_predator
	or	al,BLIT_PREDATOR

not_shape_predator:
	test	[flags],SHAPE_FADING
	jz	not_shape_fading
	or	al,BLIT_FADING

not_shape_fading:

	test	[flags],SHAPE_TRANS
	jz	not_shape_transparent
	or	al,BLIT_TRANSPARENT

not_shape_transparent:

	test	[flags],SHAPE_GHOST
	jz	not_shape_ghost
	or	al,BLIT_GHOST

not_shape_ghost:

	 
      	shl	eax,7
       	add	eax, offset NewShapeJumpTable
	mov	[ShapeJumpTableAddress],eax
	;call	Init_New_Shape_Jump_Table_Address


headers_set:
	popad

	;--------------------------------------------------------------------
	; SHAPE_FADING: DWORD fade_table[256], DWORD fade_count
	;--------------------------------------------------------------------
	test	[ flags ] , SHAPE_FADING	; are we fading this shape
	jz	check_predator

	mov	eax , [ flags + edi ]
	mov	[ FadingTable ] , eax		; save address of fading tbl
	mov	eax , [ flags + edi + 4 ]	; get fade num
	or	[ jflags ] , FLAG_FADING
	and	eax , 03fh			; no need for more than 63
	add	edi , 8				; next argument
	cmp	eax , 0				; check if it's 0
	jnz	set_fading			; if not, store fade num

	and	[ flags ] , NOT SHAPE_FADING	; otherwise, don't fade

set_fading:
	mov	[ FadingNum ] , eax

	mov	ebx,[ShapeJumpTableAddress]
	mov	dword ptr [CriticalFadeRedirections-NewShapeJumpTable+ebx],offset Single_Line_Single_Fade
	mov	dword ptr [CriticalFadeRedirections-NewShapeJumpTable+ebx+4],offset Single_Line_Single_Fade_Trans
	cmp	eax,1
	jz	single_fade
	mov	dword ptr [CriticalFadeRedirections-NewShapeJumpTable+ebx],offset Single_Line_Fading
	mov	dword ptr [CriticalFadeRedirections-NewShapeJumpTable+ebx+4],offset Single_Line_Fading_Trans

single_fade:

	;--------------------------------------------------------------------
	; SHAPE_PREDATOR: DWORD init_pred_lookup_offset (0-7)
	;--------------------------------------------------------------------
check_predator:
	test	[ flags ] , SHAPE_PREDATOR	; is predator effect on
	jz	check_partial

	mov	eax , [ flags + edi ]		; pull the partial value
	or	[ jflags ] , FLAG_PREDATOR
	shl	eax , 1
	cmp	eax , 0
	jge	check_range

	neg	eax
	mov	ebx , -1
	and	eax , PRED_MASK			; keep entries within bounds
	mov	bl , al
	mov	eax , ebx			; will be ffffff00-ffffff07
	jmp	pred_cont

check_range:
	and	eax , PRED_MASK			; keep entries within bounds

pred_cont:
	add	edi , 4				; next argument
	mov	[ BFPredOffset ] , eax
	mov	[ BFPartialCount ] , 0		; clear the partial count
	mov	[ BFPartialPred ] , 100h	; init partial to off

pred_neg_init:
	mov  	esi , [ dest ]	    ; get ptr to dest
	mov	ebx, 7 * 2

pred_loop:
	movzx	eax , [ WORD PTR BFPredNegTable + ebx ]
	add	eax , [esi].GraphicViewPort.GVPWidth	; add width
	add	eax , [esi].GraphicViewPort.GVPXAdd	; add x add
	add	eax , [esi].GraphicViewPort.GVPPitch	; extra pitch of DD surface	ST - 9/29/95 1:08PM
	mov	[ WORD PTR BFPredNegTable + 16 + ebx ] , ax
	dec	ebx
	dec	ebx
	jge	pred_loop

	;--------------------------------------------------------------------
	; SHAPE_PARTIAL: DWORD partial_pred_value (0-255)
	;--------------------------------------------------------------------
check_partial:
	test	[ flags ] , SHAPE_PARTIAL		; is this a partial pred?
	jz	setupfunc

	mov	eax , [ flags + edi ]		; pull the partial value
	add	edi , 4				; next argument
	and	eax , 0ffh			; make sure 0-255
	mov	[ BFPartialPred ] , eax		; store it off

setupfunc:
	mov	ebx , [ jflags ]		; load flags value
	and	ebx , FLAG_MASK			; clip high bits
	add	ebx , ebx			; mult by 4 to get DWORD offset
	add	ebx , ebx
	mov	ebx , dword ptr [ BufferFrameTable + ebx ]	; get table value
	mov	dword ptr [ BufferFrameRout ] , ebx		; store it in the function pointer

; Clip dest Rectangle against source Window boundaries.

	mov	[ scr_x ] , 0
	mov	[ scr_y ] , 0
	mov  	esi , [ dest ]	    ; get ptr to dest
	xor 	ecx , ecx
	xor 	edx , edx
	mov	edi , [esi].GraphicViewPort.GVPWidth  ; get width into register
	mov	ebx , [ x_pixel ]
	mov	eax , [ x_pixel ]
	add	ebx , [ pixel_width ]
	shld	ecx , eax , 1
	mov	[ x1_pixel ] , ebx
	inc	edi
	shld	edx , ebx , 1
	sub	eax , edi
	sub	ebx , edi
	shld	ecx , eax , 1
	shld	edx , ebx , 1

	mov	edi,[esi].GraphicViewPort.GVPHeight ; get height into register
	mov	ebx , [ y_pixel ]
	mov	eax , [ y_pixel ]
	add	ebx , [ pixel_height ]
	shld	ecx , eax , 1
	mov	[ y1_pixel ] , ebx
	inc	edi
	shld	edx , ebx , 1
	sub	eax , edi
	sub	ebx , edi
	shld	ecx , eax , 1
	shld	edx , ebx , 1

	xor	cl , 5
	xor	dl , 5
	mov	al , cl
	test	dl , cl
	jnz	real_out

	or	al , dl
	jz	do_blit

	mov	[use_old_draw],1
	test	cl , 1000b
	jz	dest_left_ok

	mov	eax , [ x_pixel ]
	neg	eax
	mov	[ x_pixel ] , 0
	mov	[ scr_x ] , eax

dest_left_ok:
	test	cl , 0010b
	jz	dest_bottom_ok

	mov	eax , [ y_pixel ]
	neg	eax
	mov	[ y_pixel ] , 0
	mov	[ scr_y ] , eax

dest_bottom_ok:
	test	dl , 0100b
	jz	dest_right_ok

	mov	eax , [esi].GraphicViewPort.GVPWidth  ; get width into register
	mov	[ x1_pixel ] , eax

dest_right_ok:
	test	dl , 0001b
	jz	do_blit

	mov	eax , [esi].GraphicViewPort.GVPHeight  ; get width into register
	mov	[ y1_pixel ] , eax

do_blit:
	cld
	mov	eax , [esi].GraphicViewPort.GVPXAdd
	add	eax , [esi].GraphicViewPort.GVPPitch
	add	eax , [esi].GraphicViewPort.GVPWidth
	mov	edi , [esi].GraphicViewPort.GVPOffset

	mov	ecx , eax
	mul	[ y_pixel ]
	add	edi , [ x_pixel ]
	add	edi , eax

	add	ecx , [ x_pixel ]
	sub	ecx , [ x1_pixel ]
	mov	[ dest_adjust_width ] , ecx

	mov	esi , [ src ]
	mov	eax , [ pixel_width ]
	sub	eax , [ x1_pixel ]
	add	eax , [ x_pixel ]
	mov	[ scr_adjust_width ] , eax

	mov	eax , [ scr_y ]
	mul	[ pixel_width ]
	add	eax , [ scr_x ]
	add	esi , eax

;
; If the shape needs to be clipped then we cant handle it with the new header systen
; so draw it with the old shape drawer
;
	cmp	[use_old_draw],0
	jnz	use_old_stuff

	add	[header_pointer],size ShapeHeaderType
	mov	edx,[pixel_height]
	mov	ecx,[pixel_width]
	mov	eax,[header_pointer]
	mov	al,[eax]
	mov	[save_ecx],ecx
	inc	[header_pointer]
	and	eax,BLIT_ALL
	shl	eax,2
	add	eax,[ShapeJumpTableAddress]
	jmp	dword ptr [eax]


use_old_stuff:
	mov	edx , [ y1_pixel ]
	mov	eax , [ x1_pixel ]

	sub	edx , [ y_pixel ]
	jle	real_out

	sub	eax , [ x_pixel ]
	jle	real_out

	jmp	[ BufferFrameRout ]	; buffer frame to viewport routine

real_out:

	cmp	[MMXAvailable],0
	jz	no_mmx_cleanup
	call	MMX_Done

no_mmx_cleanup:
	epilogue

	ret


; ********************************************************************
; Forward bitblit only
; the inner loop is so efficient that
; the optimal consept no longer apply because
; the optimal byte have to by a number greather than 9 bytes
; ********************************************************************
;extern	BF_Copy:near

BF_Copy:
	prologue

	cmp	eax , 10
	jl	forward_loop_bytes

forward_loop_dword:
	mov	ecx , edi
	mov	ebx , eax
	neg	ecx
	and	ecx , 3
	sub	ebx , ecx
	rep	movsb
	mov	ecx , ebx
	shr	ecx , 2
	rep	movsd
	mov	ecx , ebx
	and	ecx , 3
	rep	movsb

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	forward_loop_dword

	ret

forward_loop_bytes:
	mov	ecx , eax
	rep	movsb
	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx					; decrement the height
	jnz	forward_loop_bytes
	
	epilogue

	ret


;********************************************************************
;********************************************************************

;		segment code page public use32 'code'	; Need stricter segment alignment
							; for pentium optimisations


;
; Expand the 'next_line' macro so we can jump to it
;
;
;  ST - 12/20/2018 3:48PM
Next_Line::	next_line



;*****************************************************************************
; Draw a single line with transparent pixels
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Trans::
		prologue

Single_Line_Trans_Entry::

slt_mask_map_lp:				; Pentium pipeline usage
						;Pipe	Cycles
		mov	al,[esi]		;U	1
		inc	esi			;Vee	1

		test	al,al			;U	1
		jz	slt_skip		;Vee	1/5

slt_not_trans:mov	[edi],al		;u 	1

		inc	edi			;vee	1
		dec	ecx			;u	1

		jnz	slt_mask_map_lp	;vee  (maybe)	1

slt_end_line:	epilogue
		next_line

		;align	32

slt_skip:	inc	edi
		dec	ecx
		jz	slt_skip_end_line

		mov	al,[esi]
		inc	esi
		test	al,al
		jz	slt_skip2
		mov	[edi],al
		inc	edi
		dec	ecx
		jnz	slt_mask_map_lp

		epilogue
		next_line

		;align	32

slt_skip2:	inc	edi
		dec	ecx
		jz	slt_end_line

;
; If we have hit two transparent pixels in a row then we go into
; the transparent optimised bit
;
slt_round_again:
	rept	64
		mov	al,[esi]   ;	;pipe 1
		inc	esi	   ;1	;pipe 2
		test	al,al	   ;	;pipe 1
		jnz	slt_not_trans;pipe 2 (not pairable in 1)
				   ;2
		inc	edi	   ;	;pipe 1
		dec	ecx	   ;3	;pipe 2
		jz	slt_end_line ;4	;pipe 1 (not pairable)
	endm			   ; best case is 4 cycles per iteration
		jmp	slt_round_again



slt_skip_end_line:
		epilogue
		next_line



;*****************************************************************************
; Draw a single line with no transparent pixels
;
; 11/29/95 10:21AM - ST
;
; We have to align the destination for cards that dont bankswitch correctly
; when you write non-aligned data.
;
		;align	32
Long_Single_Line_Copy:
		prologue

 rept 3
		test	edi,3
		jz	LSLC_aligned
		movsb
		dec	ecx
 endm

LSLC_aligned:
		mov	ebx,ecx

		shr	ecx,2
		rep	movsd
		and	ebx,3
		jz	proc_out
		movsb
		dec	bl
		jz	proc_out
		movsb
		dec	bl
		jz	proc_out
		movsb
proc_out:		epilogue
		next_line



;*****************************************************************************
; Draw a single short line with no transparent pixels
;
; 11/29/95 10:21AM - ST
;
		;align	32
Short_Single_Line_Copy:
		prologue
		cmp	ecx,16
		jge	Long_Single_Line_Copy
		mov	ebx,ecx
		rep	movsb
		mov	ecx,ebx
		epilogue
		next_line


;*****************************************************************************
; Skip a line of source that is all transparent
;
; 11/29/95 10:21AM - ST
;

		;align	32
Single_Line_Skip:
		prologue
		add	esi,ecx
		add	edi,ecx
		epilogue
		next_line



;*****************************************************************************
; Draw a single line with ghosting
;
; 11/29/95 10:21AM - ST
;
		;align	32
Single_Line_Ghost:

		prologue
		xor	eax,eax
slg_loop:	mov	al,[esi]
		mov	ebx,[IsTranslucent]
		inc	esi
		mov	bh,[eax+ebx]
		cmp	bh,-1
		jz	slg_store_pixel

		and	ebx,0ff00h
		mov	al,[edi]
		add	ebx,[Translucent]
		mov	al,[eax+ebx]

slg_store_pixel:
		mov	[edi],al

		inc	edi
		dec	ecx
		jnz	slg_loop
		epilogue
		next_line



;*****************************************************************************
; Draw a single line with transparent pixels and ghosting
;
; 11/29/95 10:21AM - ST
;
		;align	32
Single_Line_Ghost_Trans:
		prologue
		xor	eax,eax
;		cmp	ecx,3
;		ja	slgt4

slgt_loop:	mov	al,[esi]
		inc	esi
		test	al,al
		jz	slgt_transparent

slgt_not_transparent:
		mov	ebx,[IsTranslucent]
		mov	bh,[eax+ebx]
		cmp	bh,-1
		jz	slgt_store_pixel

		and	ebx,0ff00h
		mov	al,[edi]
		add	ebx,[Translucent]
		mov	al,[eax+ebx]

slgt_store_pixel:
		mov	[edi],al
		inc	edi
		dec	ecx
		jnz	slgt_loop
		epilogue
		next_line


		;align	32

slgt_transparent:
		inc	edi		;1
		dec	ecx		;2
		jz	slgt_out	;1 (not pairable)

slgt_round_again:
	rept	64
		mov	al,[esi]   ;	;pipe 1
		inc	esi	   ;1	;pipe 2
		test	al,al	   ;	;pipe 1
		jnz	slgt_not_transparent	;pipe 2 (not pairable in 1)
				   ;2
		inc	edi	   ;	;pipe 1
		dec	ecx	   ;3	;pipe 2
		jz	slgt_out ;4	;pipe 1 (not pairable)
	endm			   ; best case is 4 cycles per iteration
		jmp	slgt_round_again

slgt_out:	epilogue
		next_line



;
; Optimised video memory access version
;
		;align 	32

slgt4:        push	edx
		mov	edx,[edi]

	rept	4
	local	slgt4_store1
	local	slgt4_trans1
		mov	al,[esi]
		inc	esi
		test	al,al
		jz	slgt4_trans1

		mov	ebx,[IsTranslucent]
		mov	bh,[eax+ebx]
		cmp	bh,-1
		jz	slgt4_store1

		and	ebx,0ff00h
		mov	al,dl
		add	ebx,[Translucent]
		mov	al,[eax+ebx]

slgt4_store1:	mov	dl,al

slgt4_trans1:	ror	edx,8
	endm
		mov	[edi],edx
		pop	edx
		lea	edi,[edi+4]
		lea	ecx,[ecx+0fffffffch]
		cmp	ecx,3
		ja	slgt4
		test	ecx,ecx
		jnz	slgt_loop

		epilogue
		next_line










;*****************************************************************************
; Draw a single line with fading (colour remapping)
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Fading:
		prologue
		xor	eax,eax
		mov	ebx,[FadingTable]
		push	ebp
		mov	ebp,[FadingNum]
		push	ebp

slf_loop:	mov	al,[esi]
		inc	esi

		mov	ebp,[esp]

slf_fade_loop:mov	al,[ebx+eax]
		dec	ebp
		jnz	slf_fade_loop

		mov	[edi],al
		inc	edi

		dec	ecx
		jnz	slf_loop
		add	esp,4
		pop	ebp
		epilogue
		next_line


;*****************************************************************************
; Draw a single line with transparent pixels and fading (colour remapping)
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Fading_Trans:
		prologue
		xor	eax,eax
		mov	ebx,[FadingTable]
		push	ebp
		mov	ebp,[FadingNum]
		push	ebp

slft_loop:	mov	al,[esi]
		inc	esi
		test	al,al
		jz	slft_transparent

		mov	ebp,[esp]

slft_fade_loop:
		mov	al,[ebx+eax]
		dec	ebp
		jnz	slft_fade_loop

		mov	[edi],al
slft_transparent:
		inc	edi

		dec	ecx
		jnz	slft_loop
		add	esp,4
		pop	ebp
		epilogue
		next_line





;*****************************************************************************
; Draw a single line with a single fade level (colour remap)
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Single_Fade:
		prologue
		xor	eax,eax
		mov	ebx,[FadingTable]

slsf_loop:	mov	al,[esi]
		mov	al,[ebx+eax]
		mov	[edi],al
		inc	esi
		inc	edi

		dec	ecx
		jnz	slsf_loop
		epilogue
		next_line



;*****************************************************************************
; Draw a single line with transparent pixels and a single fade level (colour remap)
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Single_Fade_Trans:
		prologue
		xor	eax,eax
		mov	ebx,[FadingTable]

slsft_loop:	mov	al,[esi]
		inc	esi
		test	al,al
		jz	slsft_transparent
		mov	al,[ebx+eax]
		mov	[edi],al
		inc	edi
		dec	ecx
		jnz	slsft_loop
		epilogue
		next_line

		;align	32

slsft_transparent:
		inc	edi

		dec	ecx
		jz	slsft_next_line
		mov	al,[esi]
		inc	esi
		test	al,al
		jz	slsft_transparent
		mov	al,[ebx+eax]
		mov	[edi],al
		inc	edi
		dec	ecx
		jnz	slsft_loop

slsft_next_line:
		epilogue
		next_line





;*****************************************************************************
; Draw a single line with ghosting and fading (colour remapping)
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Ghost_Fading:

		prologue
		mov	[StashECX],ecx

SLGF_loop:	xor	eax,eax
		mov	al,[esi]
		mov	ebx,[IsTranslucent]
		mov	bh,[eax+ebx]
		cmp	bh,-1
		jz	slgf_do_fading

		and	ebx,0ff00h

		mov	al,[edi]
		add	ebx,[Translucent]
		mov	al,[ebx+eax]

slgf_do_fading:
		mov	ebx,[FadingTable]
		mov	ecx,[FadingNum]

slgf_fade_loop:
		mov	al,[eax+ebx]
		dec	ecx
		jnz	slgf_fade_loop

		mov	[edi],al
		inc	esi
		inc	edi

		dec	[StashECX]
		jnz	SLGF_loop
		epilogue
		next_line


;*****************************************************************************
; Draw a single line with transparent pixels, ghosting and fading
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Ghost_Fading_Trans:
		prologue
		mov	[StashECX],ecx
		xor	eax,eax

;		cmp	ecx,3
;		ja	slgft4

SLGFT_loop:	mov	al,[esi]
		inc	esi
		test	al,al
		jz	slgft_trans_pixel
		mov	ebx,[IsTranslucent]
		mov	bh,[eax+ebx]
		cmp	bh,-1
		jz	slgft_do_fading

		and	ebx,0ff00h

		mov	al,[edi]
		add	ebx,[Translucent]
		mov	al,[ebx+eax]

slgft_do_fading:
		mov	ebx,[FadingTable]
		mov	ecx,[FadingNum]

slgft_fade_loop:
		mov	al,[eax+ebx]
		dec	ecx
		jnz	slgft_fade_loop

		mov	[edi],al
slgft_trans_pixel:
		inc	edi

		dec	[StashECX]
		jnz	SLGFT_loop
		epilogue
		next_line


		;align	32

slgft4:	push	edx
		mov	edx,[edi]

	rept	4
	local	slgft4_fade
	local	slgft4_fade_lp
	local	slgft4_trans
		mov	al,[esi]
		inc	esi
		test	al,al
		jz	slgft4_trans
		mov	ebx,[IsTranslucent]
		mov	bh,[eax+ebx]
		cmp	bh,-1
		jz	slgft4_fade

		and	ebx,0ff00h

		mov	al,dl
		add	ebx,[Translucent]
		mov	al,[ebx+eax]

slgft4_fade:	mov	ebx,[FadingTable]
		mov	ecx,[FadingNum]

slgft4_fade_lp:	mov	al,[eax+ebx]
		dec	ecx
		jnz	slgft4_fade_lp

		mov	dl,al

slgft4_trans:	ror	edx,8
	endm
		mov	[edi],edx
		pop	edx
		lea	edi,[edi+4]
		sub	[StashECX],4
		jz	slgft4_out
		cmp	[StashECX],3
		ja	slgft4
		jmp	SLGFT_loop

slgft4_out:	epilogue
		next_line


;*****************************************************************************
; Draw a single line with predator effect
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Predator:

		prologue

slp_loop:	mov	al,[esi]

		mov	ebx,[BFPartialCount]
		add	ebx,[BFPartialPred]
		or	bh,bh
		jnz	slp_get_pred

		mov	[BFPartialCount] , ebx
		jmp	slp_skip_pixel

slp_get_pred:	xor	bh , bh
		mov	eax,[BFPredOffset]
		mov	[BFPartialCount] , ebx
		add	byte ptr [BFPredOffset],2
		mov	eax, dword ptr [BFPredTable+eax]
		and	byte ptr [BFPredOffset],PRED_MASK
		and	eax,0ffffh

		mov	al,[edi+eax]
		mov	[edi],al

slp_skip_pixel:
		inc	esi
		inc	edi

		dec	ecx
		jnz	slp_loop

		epilogue
		next_line




;*****************************************************************************
; Draw a single line with transparent pixels and predator effect
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Predator_Trans:

		prologue

slpt_loop:	mov	al,[esi]
		inc	esi
		test	al,al
		jz	slpt_skip_pixel

		mov	ebx,[BFPartialCount]
		add	ebx,[BFPartialPred]
		or	bh,bh
		jnz	slpt_get_pred

		mov	[BFPartialCount] , ebx
		jmp	slpt_skip_pixel

slpt_get_pred:xor	bh , bh
		mov	eax,[BFPredOffset]
		mov	[BFPartialCount] , ebx
		add	byte ptr [BFPredOffset],2
		mov	eax,dword ptr [BFPredTable+eax]
		and	byte ptr [BFPredOffset ] , PRED_MASK
		and	eax,0ffffh

		mov	al,[edi+eax]
		mov	[edi],al

slpt_skip_pixel:
		inc	edi

		dec	ecx
		jnz	slpt_loop

		epilogue
		next_line


;*****************************************************************************
; Draw a single line with predator and ghosting
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Predator_Ghost:

		prologue

slpg_loop:	mov	al,[esi]
		mov	ebx,[BFPartialCount]
		add	ebx,[BFPartialPred]
		test	bh,bh
		jnz	slpg_get_pred		; is this a predator pixel?

		mov	[BFPartialCount],ebx
		jmp	slpg_check_ghost

slpg_get_pred:
		xor	bh,bh
		mov	eax,[BFPredOffset]
		mov	[BFPartialCount],ebx
		add	byte ptr [BFPredOffset],2
		mov	eax,dword ptr [BFPredTable+eax ]
		and	byte ptr [BFPredOffset],PRED_MASK
		and	eax,0ffffh
		mov	al,[edi+eax]

slpg_check_ghost:
		mov	ebx,[IsTranslucent]
		mov	bh,[ebx+eax]
		cmp	bh,0ffh
		je	slpg_store_pixel

		xor	eax,eax
		and	ebx,0FF00h

		mov	al,[edi]
		add	ebx,[Translucent]

		mov	al,[ebx+eax]

slpg_store_pixel:
		mov	[edi],al
		inc	esi
		inc	edi

		dec	ecx
		jnz	slpg_loop

		epilogue
		next_line



;*****************************************************************************
; Draw a single line with transparent pixels, predator and ghosting
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Predator_Ghost_Trans:
		prologue

slpgt_loop:	mov	al,[esi]
		inc	esi
		test	al,al
		jz	slpgt_transparent

		mov	ebx,[BFPartialCount]
		add	ebx,[BFPartialPred]
		test	bh,bh
		jnz	slpgt_get_pred		; is this a predator pixel?

		mov	[BFPartialCount],ebx
		jmp	slpgt_check_ghost

slpgt_get_pred:
		xor	bh,bh
		mov	eax,[BFPredOffset]
		mov	[BFPartialCount],ebx
		add	byte ptr [BFPredOffset],2
		mov	eax,dword ptr [BFPredTable+eax ]
		and	byte ptr [BFPredOffset],PRED_MASK
		and	eax,0ffffh
		mov	al,[edi+eax]

slpgt_check_ghost:
		mov	ebx,[IsTranslucent]
		mov	bh,[ebx+eax]
		cmp	bh,0ffh
		je	slpgt_store_pixel

		xor	eax,eax
		and	ebx,0FF00h

		mov	al,[edi]
		add	ebx,[Translucent]

		mov	al,[ebx+eax]

slpgt_store_pixel:
		mov	[edi],al
slpgt_transparent:
		inc	edi

		dec	ecx
		jnz	slpgt_loop

		pop	ecx
		epilogue
		next_line


;*****************************************************************************
; Draw a single line with predator and fading
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Predator_Fading:

		prologue
		mov	[StashECX],ecx

slpf_loop:	mov	al,[esi]
		mov	ebx,[BFPartialCount]
		inc	esi
		add	ebx,[BFPartialPred]
		test	bh,bh
		jnz	slpf_get_pred

		mov	[BFPartialCount],ebx
		jmp	slpf_do_fading

slpf_get_pred:xor	bh,bh
		mov	eax,[BFPredOffset]
		mov	[BFPartialCount],ebx
		and	byte ptr [BFPredOffset],2
		mov	eax,dword ptr [BFPredTable+eax]
		and	byte ptr [BFPredOffset],PRED_MASK

		and	eax,0ffffh
		mov	al,[eax+edi]

slpf_do_fading:
		and	eax,255
		mov	ebx,[FadingTable]
		mov	ecx,[FadingNum]

slpf_fade_loop:
		mov	al,[eax+ebx]
		dec	ecx
		jnz	slpf_fade_loop

		mov	[edi],al
		inc	edi

		dec	[StashECX]
		jnz	slpf_loop

		epilogue
		next_line



;*****************************************************************************
; Draw a single line with transparent pixels, fading and predator
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Predator_Fading_Trans:
		prologue
		mov	[StashECX],ecx

slpft_loop:	mov	al,[esi]
		inc	esi
		test	al,al
		jz	slpft_transparent
		mov	ebx,[BFPartialCount]
		add	ebx,[BFPartialPred]
		test	bh,bh
		jnz	slpft_get_pred

		mov	[BFPartialCount],ebx
		jmp	slpft_do_fading

slpft_get_pred:
		xor	bh,bh
		mov	eax,[BFPredOffset]
		mov	[BFPartialCount],ebx
		and	byte ptr [BFPredOffset],2
		mov	eax,dword ptr [BFPredTable+eax]
		and	byte ptr [BFPredOffset],PRED_MASK

		and	eax,0ffffh
		mov	al,[eax+edi]

slpft_do_fading:
		and	eax,255
		mov	ebx,[FadingTable]
		mov	ecx,[FadingNum]

slpft_fade_loop:
		mov	al,[eax+ebx]
		dec	ecx
		jnz	slpft_fade_loop

		mov	[edi],al
slpft_transparent:
		inc	edi

		dec	[StashECX]
		jnz	slpft_loop

		epilogue
		next_line



;*****************************************************************************
; Draw a single line with predator, ghosting and fading
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Predator_Ghost_Fading:

		prologue
		mov	[StashECX],ecx

slpgf_loop:	mov	al,[esi]
		mov	ebx,[BFPartialCount]
		inc	esi
		add	ebx,[BFPartialPred]
		test	bh , bh
		jnz	slpgf_get_pred		; is this a predator pixel?

		mov	[BFPartialCount],ebx
		jmp	slpgf_check_ghost

slpgf_get_pred:
		xor	bh,bh
		mov	eax,[BFPredOffset]
		mov	[BFPartialCount],ebx
		add	byte ptr [BFPredOffset],2
		mov	eax,dword ptr [BFPredTable+eax]
		and	byte ptr [BFPredOffset],PRED_MASK
		and	eax,0ffffh

		mov	al,[edi+eax]

slpgf_check_ghost:
		and	eax,255
		mov	ebx,[IsTranslucent]
		mov	bh,[ebx+eax]
		cmp	bh,0ffh
		je	slpgf_do_fading

		and	ebx , 0FF00h

		mov	al,[edi]
		add	ebx,[Translucent]

		mov	al,[ebx+eax]

slpgf_do_fading:
		xor	eax,eax
		mov	ebx,[FadingTable]
		mov	ecx,[FadingNum]

slpgf_fade_loop:
		mov	al,[ebx+eax]
		dec	ecx
		jnz	slpgf_fade_loop

slpgf_store_pixel:
		mov	[edi],al
		inc	edi

		dec	[StashECX]
		jnz	slpgf_loop

		epilogue
		next_line



;*****************************************************************************
; Draw a single line with transparent pixels, predator, ghosting and fading
;
; 11/29/95 10:21AM - ST
;
		;align	32

Single_Line_Predator_Ghost_Fading_Trans:

		prologue
		mov	[StashECX],ecx

slpgft_loop:	mov	al,[esi]
		inc	esi
		test	al,al
		jz	slpgft_transparent

		mov	ebx,[BFPartialCount]
		add	ebx,[BFPartialPred]
		test	bh , bh
		jnz	slpgft_get_pred		; is this a predator pixel?

		mov	[BFPartialCount],ebx
		jmp	slpgft_check_ghost

slpgft_get_pred:
		xor	bh,bh
		mov	eax,[BFPredOffset]
		mov	[BFPartialCount],ebx
		add	byte ptr [BFPredOffset],2
		mov	eax,dword ptr [BFPredTable+eax]
		and	byte ptr [BFPredOffset],PRED_MASK
		and	eax,0ffffh

		mov	al,[edi+eax]

slpgft_check_ghost:
		and	eax,255
		mov	ebx,[IsTranslucent]
		mov	bh,[ebx+eax]
		cmp	bh,0ffh
		je	slpgft_do_fading

		and	ebx , 0FF00h

		mov	al,[edi]
		add	ebx,[Translucent]

		mov	al,[ebx+eax]

slpgft_do_fading:
		xor	eax,eax
		mov	ebx,[FadingTable]
		mov	ecx,[FadingNum]

slpgft_fade_loop:
		mov	al,[ebx+eax]
		dec	ecx
		jnz	slpgft_fade_loop

slpgft_store_pixel:
		mov	[edi],al
slpgft_transparent:
		inc	edi

		dec	[StashECX]
		jnz	slpgft_loop

		epilogue
		next_line




;		ends		;end of strict alignment segment

;	       	codeseg



;extern	BF_Trans:near

BF_Trans:

	prologue
; calc the code location to skip to 10 bytes per REPT below!!!!
	mov	ecx , eax
	and	ecx , 01fh
	lea	ecx , [ ecx + ecx * 4 ]		; quick multiply by 5
	neg	ecx
	shr	eax , 5
	lea	ecx , [ trans_reference + ecx * 2 ] ; next multiply by 2
	mov	[ loop_cnt ] , eax
	mov	[ jmp_loc ] , ecx

trans_loop:
	mov	ecx , [ loop_cnt ]
	jmp	[ jmp_loc ]

; the following code should NOT be changed without changing the calculation
; above!!!!!!

trans_line:

	REPT	32
	local	trans_pixel
		mov	bl , [ esi ]
		inc	esi
		test	bl , bl
		jz	trans_pixel

		mov	[ edi ] , bl

	trans_pixel:
		inc	edi
	ENDM

trans_reference:
	dec	ecx
	jge	trans_line

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	trans_loop
	epilogue

	ret

;********************************************************************
;********************************************************************

;extern	BF_Ghost:near
BF_Ghost:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx, [offset ghost_reference]
	sub	ecx, [offset ghost_line]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ ghost_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

ghost_loop:
	mov	ecx , [ loop_cnt ]
	jmp	[ jmp_loc ]

ghost_line:

	REPT	32
	local	store_pixel
		mov	al , [ esi ]
		inc	esi
		mov	ebx , [ IsTranslucent ]		; is it a translucent color?
		mov	bh , BYTE PTR [ ebx + eax ]
		cmp	bh , 0ffh
		je	store_pixel

		and	ebx , 0FF00h			; clear all of ebx except bh
							; we have the index to the translation table
							; ((trans_colour * 256) + dest colour)
		mov	al , [ edi ]			; mov pixel at destination to al
		add	ebx , [ Translucent ]		; get the ptr to it!
							; Add the (trans_color * 256) of the translation equ.
		mov	al , BYTE PTR [ ebx + eax ]	; get new pixel in al

	store_pixel:
		mov	[ edi ] , al
		inc	edi

	ENDM

ghost_reference:
	dec	ecx
	jge	ghost_line

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	ghost_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Ghost_Trans:near
BF_Ghost_Trans:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset ghost_t_reference ]
	sub	ecx, [ offset ghost_t_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ ghost_t_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

ghost_t_loop:
	mov	ecx , [ loop_cnt ]
	jmp	[ jmp_loc ]

ghost_t_line:

	REPT	32
	local	transp_pixel
	local	store_pixel
		mov	al , [ esi ]
		inc	esi
		test	al , al
		jz	transp_pixel

		mov	ebx , [ IsTranslucent ]		; is it a translucent color?
		mov	bh , BYTE PTR [ ebx + eax ]
		cmp	bh , 0ffh
		je	store_pixel

		and	ebx , 0FF00h			; clear all of ebx except bh
							; we have the index to the translation table
							; ((trans_colour * 256) + dest colour)
		mov	al , [ edi ]			; mov pixel at destination to al
		add	ebx , [ Translucent ]		; get the ptr to it!
							; Add the (trans_color * 256) of the translation equ.
		mov	al , BYTE PTR [ ebx + eax ]	; get new pixel in al

	store_pixel:
		mov	[ edi ] , al

	transp_pixel:
		inc	edi

	ENDM

ghost_t_reference:
	dec	ecx
	jge	ghost_t_line

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	ghost_t_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Fading:near
BF_Fading:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset fading_reference ]
	sub	ecx, [ offset fading_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ fading_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

fading_loop:
	mov	ecx , [ loop_cnt ]
	mov	[ StashECX ] , ecx		; preserve ecx for later
	mov	ebx , [ FadingTable ]		; run color through fading table
	jmp	[ jmp_loc ]

fading_line_begin:
	mov	[ StashECX ] , ecx		; preserve ecx for later

fading_line:

	REPT	32
	local	fade_loop
		mov	al , [ esi ]
		inc	esi
		mov	ecx , [ FadingNum ]

	fade_loop:
		mov	al, byte ptr [ebx + eax]
		dec	ecx
		jnz	fade_loop

		mov	[ edi ] , al
		inc	edi

	ENDM

fading_reference:
	mov	ecx , [ StashECX ]		; restore ecx for main draw loop
	dec	ecx
	jge	fading_line_begin

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	fading_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Fading_Trans:near
BF_Fading_Trans:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset fading_t_reference ]
	sub	ecx, [ offset fading_t_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ fading_t_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

fading_t_loop:
	mov	ecx , [ loop_cnt ]
	mov	[ StashECX ] , ecx		; preserve ecx for later
	mov	ebx , [ FadingTable ]		; run color through fading table
	jmp	[ jmp_loc ]

fading_t_line_begin:
	mov	[ StashECX ] , ecx		; preserve ecx for later

fading_t_line:

	REPT	32
	local	transp_pixel
	local	fade_loop
		mov	al , [ esi ]
		inc	esi
		test	al , al
		jz	transp_pixel

		mov	ecx , [ FadingNum ]

	fade_loop:
		mov	al, byte ptr [ebx + eax]
		dec	ecx
		jnz	fade_loop

		mov	[ edi ] , al

	transp_pixel:
		inc	edi

	ENDM

fading_t_reference:
	mov	ecx , [ StashECX ]		; restore ecx for main draw loop
	dec	ecx
	jge	fading_t_line_begin

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	fading_t_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Ghost_Fading:near
BF_Ghost_Fading:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset ghost_f_reference ]
	sub	ecx, [ offset ghost_f_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ ghost_f_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

ghost_f_loop:
	mov	ecx , [ loop_cnt ]
	mov	[ StashECX ] , ecx		; preserve ecx for later
	jmp	[ jmp_loc ]

ghost_f_line_begin:
	mov	[ StashECX ] , ecx		; preserve ecx for later

ghost_f_line:

	REPT	32
	local	store_pixel
	local	do_fading
	local	fade_loop
		mov	al , [ esi ]
		inc	esi
		mov	ebx , [ IsTranslucent ]		; is it a lucent color?
		mov	bh , byte ptr [ ebx + eax ]
		cmp	bh , 0ffh
		je	do_fading

		and	ebx , 0FF00h			; clear all of ebx except bh
							; we have the index to the lation table
							; ((_colour * 256) + dest colour)
		mov	al , [ edi ]			; mov pixel at destination to al
		add	ebx , [ Translucent ]		; get the ptr to it!
							; Add the (_color * 256) of the lation equ.
		mov	al , byte ptr [ ebx + eax ]	; get new pixel in al
; DRD		jmp	store_pixel

	do_fading:
		mov	ebx , [ FadingTable ]		; run color through fading table
		mov	ecx , [ FadingNum ]

	fade_loop:
		mov	al, byte ptr [ebx + eax]
		dec	ecx
		jnz	fade_loop

	store_pixel:
		mov	[ edi ] , al
		inc	edi

	ENDM

ghost_f_reference:
	mov	ecx , [ StashECX ]		; restore ecx for main draw loop
	dec	ecx
	jge	ghost_f_line_begin

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	ghost_f_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Ghost_Fading_Trans:near
BF_Ghost_Fading_Trans:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset ghost_f_t_reference ]
	sub	ecx, [ offset ghost_f_t_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ ghost_f_t_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

ghost_f_t_loop:
	mov	ecx , [ loop_cnt ]
	mov	[ StashECX ] , ecx		; preserve ecx for later
	jmp	[ jmp_loc ]

ghost_f_t_line_begin:
	mov	[ StashECX ] , ecx		; preserve ecx for later

ghost_f_t_line:

	REPT	32
	local	transp_pixel
	local	store_pixel
	local	do_fading
	local	fade_loop
		mov	al , [ esi ]
		inc	esi
		test	al , al
		jz	transp_pixel

		mov	ebx , [ IsTranslucent ]		; is it a translucent color?
		mov	bh , byte ptr [ ebx + eax ]
		cmp	bh , 0ffh
		je	do_fading

		and	ebx , 0FF00h			; clear all of ebx except bh
							; we have the index to the translation table
							; ((trans_colour * 256) + dest colour)
		mov	al , [ edi ]			; mov pixel at destination to al
		add	ebx , [ Translucent ]		; get the ptr to it!
							; Add the (trans_color * 256) of the translation equ.
		mov	al , byte ptr [ ebx + eax ]	; get new pixel in al
; DRD		jmp	store_pixel

	do_fading:
		mov	ebx , [ FadingTable ]		; run color through fading table
		mov	ecx , [ FadingNum ]

	fade_loop:
		mov	al, byte ptr [ebx + eax]
		dec	ecx
		jnz	fade_loop

	store_pixel:
		mov	[ edi ] , al

	transp_pixel:
		inc	edi

	ENDM

ghost_f_t_reference:
	mov	ecx , [ StashECX ]		; restore ecx for main draw loop
	dec	ecx
	jge	ghost_f_t_line_begin

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	ghost_f_t_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Predator:near
BF_Predator:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset predator_reference ]
	sub	ecx, [offset predator_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ predator_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

predator_loop:
	mov	ecx , [ loop_cnt ]
	jmp	[ jmp_loc ]

predator_line:

	REPT	32
	local	get_pred
	local	skip_pixel
		mov	al , [ esi ]
		inc	esi
		mov	ebx , [ BFPartialCount ]
		add	ebx , [ BFPartialPred ]
		or	bh , bh
		jnz	get_pred		; is this a predator pixel?

		mov	[ BFPartialCount ] , ebx
		jmp	skip_pixel

	get_pred:
		xor	bh , bh
		mov	eax, [ BFPredOffset ]
		mov	[ BFPartialCount ] , ebx
		add	BYTE PTR [ BFPredOffset ] , 2
		movzx	eax , WORD PTR [ BFPredTable + eax ]
		and	BYTE PTR [ BFPredOffset ] , PRED_MASK
						; pick up a color offset a pseudo-
						;  random amount from the current
		movzx	eax , BYTE PTR [ edi + eax ]	;  viewport address

;		xor	bh , bh
;		mov	eax , [ BFPredValue ]	; pick up a color offset a pseudo-
;						;  random amount from the current
;		mov	[ BFPartialCount ] , ebx
;		mov	al , [ edi + eax ]	;  viewport address

		mov	[ edi ] , al

	skip_pixel:
		inc	edi

	ENDM

predator_reference:
	dec	ecx
	jge	predator_line

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	predator_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Predator_Trans:near
BF_Predator_Trans:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset predator_t_reference ]
	sub	ecx, [ offset predator_t_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ predator_t_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

predator_t_loop:
	mov	ecx , [ loop_cnt ]
	jmp	[ jmp_loc ]

predator_t_line:

	REPT	32
	local	trans_pixel
	local	get_pred
	local	store_pixel
		mov	al , [ esi ]
		inc	esi
		test	al , al
		jz	trans_pixel

		mov	ebx , [ BFPartialCount ]
		add	ebx , [ BFPartialPred ]
		or	bh , bh
		jnz	get_pred		; is this a predator pixel?

		mov	[ BFPartialCount ] , ebx
		jmp	store_pixel

	get_pred:
		xor	bh , bh
		mov	eax, [ BFPredOffset ]
		mov	[ BFPartialCount ] , ebx
		add	BYTE PTR [ BFPredOffset ] , 2
		movzx	eax , WORD PTR [ BFPredTable + eax ]
		and	BYTE PTR [ BFPredOffset ] , PRED_MASK
						; pick up a color offset a pseudo-
						;  random amount from the current
		movzx	eax , BYTE PTR [ edi + eax ]	;  viewport address

	store_pixel:
		mov	[ edi ] , al

	trans_pixel:
		inc	edi

	ENDM

predator_t_reference:
	dec	ecx
	jge	predator_t_line

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	predator_t_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Predator_Ghost:near
BF_Predator_Ghost:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset predator_g_reference ]
	sub	ecx, [ offset predator_g_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ predator_g_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

predator_g_loop:
	mov	ecx , [ loop_cnt ]
	jmp	[ jmp_loc ]

predator_g_line:

	REPT	32
	local	get_pred
	local	check_ghost
	local	store_pixel
		mov	al , [ esi ]
		mov	ebx , [ BFPartialCount ]
		inc	esi
		add	ebx , [ BFPartialPred ]
		or	bh , bh
		jnz	get_pred		; is this a predator pixel?

		mov	[ BFPartialCount ] , ebx
		jmp	check_ghost

	get_pred:
		xor	bh , bh
		mov	eax, [ BFPredOffset ]
		mov	[ BFPartialCount ] , ebx
		add	BYTE PTR [ BFPredOffset ] , 2
		movzx	eax , WORD PTR [ BFPredTable + eax ]
		and	BYTE PTR [ BFPredOffset ] , PRED_MASK
						; pick up a color offset a pseudo-
						;  random amount from the current
		movzx	eax , BYTE PTR [ edi + eax ]	;  viewport address

	check_ghost:
		mov	ebx , [ IsTranslucent ]		; is it a translucent color?
		mov	bh , BYTE PTR [ ebx + eax ]
		cmp	bh , 0ffh
		je	store_pixel

		and	ebx , 0FF00h			; clear all of ebx except bh
							; we have the index to the translation table
							; ((trans_colour * 256) + dest colour)
		mov	al , [ edi ]			; mov pixel at destination to al
		add	ebx , [ Translucent ]		; get the ptr to it!
							; Add the (trans_color * 256) of the translation equ.
		mov	al , BYTE PTR [ ebx + eax ]	; get new pixel in al

	store_pixel:
		mov	[ edi ] , al
		inc	edi

	ENDM

predator_g_reference:
	dec	ecx
	jge	predator_g_line

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	predator_g_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Predator_Ghost_Trans:near
BF_Predator_Ghost_Trans:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset predator_g_t_reference ]
	sub	ecx, [ offset predator_g_t_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ predator_g_t_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

predator_g_t_loop:
	mov	ecx , [ loop_cnt ]
	jmp	[ jmp_loc ]

predator_g_t_line:

	REPT	32
	local	trans_pixel
	local	get_pred
	local	check_ghost
	local	store_pixel
		mov	al , [ esi ]
		inc	esi
		test	al , al
		jz	trans_pixel

		mov	ebx , [ BFPartialCount ]
		add	ebx , [ BFPartialPred ]
		or	bh , bh
		jnz	get_pred		; is this a predator pixel?

		mov	[ BFPartialCount ] , ebx
		jmp	check_ghost

	get_pred:
		xor	bh , bh
		mov	eax, [ BFPredOffset ]
		mov	[ BFPartialCount ] , ebx
		add	BYTE PTR [ BFPredOffset ] , 2
		movzx	eax , WORD PTR [ BFPredTable + eax ]
		and	BYTE PTR [ BFPredOffset ] , PRED_MASK
						; pick up a color offset a pseudo-
						;  random amount from the current
		movzx	eax , BYTE PTR [ edi + eax ]	;  viewport address

	check_ghost:
		mov	ebx , [ IsTranslucent ]		; is it a translucent color?
		mov	bh , BYTE PTR [ ebx + eax ]
		cmp	bh , 0ffh
		je	store_pixel

		and	ebx , 0FF00h			; clear all of ebx except bh
							; we have the index to the translation table
							; ((trans_colour * 256) + dest colour)
		mov	al , [ edi ]			; mov pixel at destination to al
		add	ebx , [ Translucent ]		; get the ptr to it!
							; Add the (trans_color * 256) of the translation equ.
		mov	al , BYTE PTR [ ebx + eax ]	; get new pixel in al

	store_pixel:
		mov	[ edi ] , al

	trans_pixel:
		inc	edi

	ENDM

predator_g_t_reference:
	dec	ecx
	jge	predator_g_t_line

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	predator_g_t_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Predator_Fading:near
BF_Predator_Fading:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset predator_f_reference ]
	sub	ecx, [ offset predator_f_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ predator_f_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

predator_f_loop:
	mov	ecx , [ loop_cnt ]
	mov	[ StashECX ] , ecx		; preserve ecx for later
	jmp	[ jmp_loc ]

predator_f_line_begin:
	mov	[ StashECX ] , ecx		; preserve ecx for later

predator_f_line:

	REPT	32
	local	get_pred
	local	do_fading
	local	fade_loop
		mov	al , [ esi ]
		mov	ebx , [ BFPartialCount ]
		inc	esi
		add	ebx , [ BFPartialPred ]
		or	bh , bh
		jnz	get_pred		; is this a predator pixel?

		mov	[ BFPartialCount ] , ebx
		jmp	do_fading

	get_pred:
		xor	bh , bh
		mov	eax, [ BFPredOffset ]
		mov	[ BFPartialCount ] , ebx
		add	BYTE PTR [ BFPredOffset ] , 2
		movzx	eax , WORD PTR [ BFPredTable + eax ]
		and	BYTE PTR [ BFPredOffset ] , PRED_MASK
						; pick up a color offset a pseudo-
						;  random amount from the current
		movzx	eax , BYTE PTR [ edi + eax ]	;  viewport address

	do_fading:
		mov	ebx , [ FadingTable ]		; run color through fading table
		mov	ecx , [ FadingNum ]

	fade_loop:
		mov	al, byte ptr [ebx + eax]
		dec	ecx
		jnz	fade_loop

		mov	[ edi ] , al
		inc	edi

	ENDM

predator_f_reference:
	mov	ecx , [ StashECX ]		; restore ecx for main draw loop
	dec	ecx
	jge	predator_f_line_begin

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	predator_f_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Predator_Fading_Trans:near
BF_Predator_Fading_Trans:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset predator_f_t_reference ]
	sub	ecx, [ offset predator_f_t_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ predator_f_t_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

predator_f_t_loop:
	mov	ecx , [ loop_cnt ]
	mov	[ StashECX ] , ecx		; preserve ecx for later
	jmp	[ jmp_loc ]

predator_f_t_line_begin:
	mov	[ StashECX ] , ecx		; preserve ecx for later

predator_f_t_line:

	REPT	32
	local	trans_pixel
	local	get_pred
	local	do_fading
	local	fade_loop
		mov	al , [ esi ]
		inc	esi
		test	al , al
		jz	trans_pixel

		mov	ebx , [ BFPartialCount ]
		add	ebx , [ BFPartialPred ]
		or	bh , bh
		jnz	get_pred		; is this a predator pixel?

		mov	[ BFPartialCount ] , ebx
		jmp	do_fading

	get_pred:
		xor	bh , bh
		mov	eax, [ BFPredOffset ]
		mov	[ BFPartialCount ] , ebx
		add	BYTE PTR [ BFPredOffset ] , 2
		movzx	eax , WORD PTR [ BFPredTable + eax ]
		and	BYTE PTR [ BFPredOffset ] , PRED_MASK
						; pick up a color offset a pseudo-
						;  random amount from the current
		movzx	eax , BYTE PTR [ edi + eax ]	;  viewport address

	do_fading:
		mov	ebx , [ FadingTable ]		; run color through fading table
		mov	ecx , [ FadingNum ]

	fade_loop:
		mov	al, byte ptr [ebx + eax]
		dec	ecx
		jnz	fade_loop

		mov	[ edi ] , al

	trans_pixel:
		inc	edi

	ENDM

predator_f_t_reference:
	mov	ecx , [ StashECX ]		; restore ecx for main draw loop
	dec	ecx
	jge	predator_f_t_line_begin

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	predator_f_t_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Predator_Ghost_Fading:near
BF_Predator_Ghost_Fading:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset predator_g_f_reference ]
	sub	ecx, [ offset predator_g_f_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ predator_g_f_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

predator_g_f_loop:
	mov	ecx , [ loop_cnt ]
	mov	[ StashECX ] , ecx		; preserve ecx for later
	jmp	[ jmp_loc ]

predator_g_f_line_begin:
	mov	[ StashECX ] , ecx		; preserve ecx for later

predator_g_f_line:

	REPT	32
	local	get_pred
	local	check_ghost
	local	store_pixel
	local	do_fading
	local	fade_loop
		mov	al , [ esi ]
		mov	ebx , [ BFPartialCount ]
		inc	esi
		add	ebx , [ BFPartialPred ]
		or	bh , bh
		jnz	get_pred		; is this a predator pixel?

		mov	[ BFPartialCount ] , ebx
		jmp	check_ghost

	get_pred:
		xor	bh , bh
		mov	eax, [ BFPredOffset ]
		mov	[ BFPartialCount ] , ebx
		add	BYTE PTR [ BFPredOffset ] , 2
		movzx	eax , WORD PTR [ BFPredTable + eax ]
		and	BYTE PTR [ BFPredOffset ] , PRED_MASK
						; pick up a color offset a pseudo-
						;  random amount from the current
		movzx	eax , BYTE PTR [ edi + eax ]	;  viewport address

	check_ghost:
		mov	ebx , [ IsTranslucent ]		; is it a translucent color?
		mov	bh , BYTE PTR [ ebx + eax ]
		cmp	bh , 0ffh
		je	do_fading

		and	ebx , 0FF00h			; clear all of ebx except bh
							; we have the index to the translation table
							; ((trans_colour * 256) + dest colour)
		mov	al , [ edi ]			; mov pixel at destination to al
		add	ebx , [ Translucent ]		; get the ptr to it!
							; Add the (trans_color * 256) of the translation equ.
		mov	al , BYTE PTR [ ebx + eax ]	; get new pixel in al
; DRD		jmp	store_pixel

	do_fading:
		mov	ebx , [ FadingTable ]		; run color through fading table
		mov	ecx , [ FadingNum ]

	fade_loop:
		mov	al, byte ptr [ebx + eax]
		dec	ecx
		jnz	fade_loop

	store_pixel:
		mov	[ edi ] , al
		inc	edi

	ENDM

predator_g_f_reference:
	mov	ecx , [ StashECX ]		; restore ecx for main draw loop
	dec	ecx
	jge	predator_g_f_line_begin

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	predator_g_f_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

;extern	BF_Predator_Ghost_Fading_Trans:near
BF_Predator_Ghost_Fading_Trans:

	prologue
	mov	ebx , eax			; width

	; NOTE: the below calculation assumes a group of instructions is
	;	less than 256 bytes

	; get length of the 32 groups of instructions

	lea	ecx , [ offset predator_g_f_t_reference ]
	sub	ecx, [ offset predator_g_f_t_line ]

	shr	ebx , 5				; width / 32
	shr	ecx , 5				; length of instructions / 32
	and	eax , 01fh			; mod of width / 32
	mul	cl				; calc offset to start of group
	neg	eax				; inverse of width
	mov	[ loop_cnt ] , ebx		; save width / 32
	lea	ecx , [ predator_g_f_t_reference + eax ]
	mov	eax , 0
	mov	[ jmp_loc ] , ecx

predator_g_f_t_loop:
	mov	ecx , [ loop_cnt ]
	mov	[ StashECX ] , ecx		; preserve ecx for later
	jmp	[ jmp_loc ]

predator_g_f_t_line_begin:
	mov	[ StashECX ] , ecx		; preserve ecx for later

predator_g_f_t_line:

	REPT	32
	local	trans_pixel
	local	get_pred
	local	check_ghost
	local	store_pixel
	local	do_fading
	local	fade_loop
		mov	al , [ esi ]
		inc	esi
		test	al , al
		jz	trans_pixel

		mov	ebx , [ BFPartialCount ]
		add	ebx , [ BFPartialPred ]
		or	bh , bh
		jnz	get_pred		; is this a predator pixel?

		mov	[ BFPartialCount ] , ebx
		jmp	check_ghost

	get_pred:
		xor	bh , bh
		mov	eax, [ BFPredOffset ]
		mov	[ BFPartialCount ] , ebx
		add	BYTE PTR [ BFPredOffset ] , 2
		movzx	eax , WORD PTR [ BFPredTable + eax ]
		and	BYTE PTR [ BFPredOffset ] , PRED_MASK
						; pick up a color offset a pseudo-
						;  random amount from the current
		movzx	eax , BYTE PTR [ edi + eax ]	;  viewport address

	check_ghost:
		mov	ebx , [ IsTranslucent ]		; is it a translucent color?
		mov	bh , BYTE PTR [ ebx + eax ]
		cmp	bh , 0ffh
		je	do_fading

		and	ebx , 0FF00h			; clear all of ebx except bh
							; we have the index to the translation table
							; ((trans_colour * 256) + dest colour)
		mov	al , [ edi ]			; mov pixel at destination to al
		add	ebx , [ Translucent ]		; get the ptr to it!
							; Add the (trans_color * 256) of the translation equ.
		mov	al , BYTE PTR [ ebx + eax ]	; get new pixel in al
; DRD		jmp	store_pixel

	do_fading:
		mov	ebx , [ FadingTable ]		; run color through fading table
		mov	ecx , [ FadingNum ]

	fade_loop:
		mov	al, byte ptr [ebx + eax]
		dec	ecx
		jnz	fade_loop

	store_pixel:
		mov	[ edi ] , al

	trans_pixel:
		inc	edi

	ENDM

predator_g_f_t_reference:
	mov	ecx , [ StashECX ]		; restore ecx for main draw loop
	dec	ecx
	jge	predator_g_f_t_line_begin

	add	esi , [ scr_adjust_width ]
	add	edi , [ dest_adjust_width ]
	dec	edx
	jnz	predator_g_f_t_loop

	epilogue
	ret


;********************************************************************
;********************************************************************

Not_Supported:
	ret

Buffer_Frame_To_Page	ENDP	


end

