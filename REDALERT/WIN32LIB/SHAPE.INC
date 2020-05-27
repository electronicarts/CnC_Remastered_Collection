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
;*                 Project Name : WWLIB32                                  *
;*                                                                         *
;*                    File Name : SHAPE.INC                                *
;*                                                                         *
;*                   Programmer : Scott Bowen				   *
;*                                                                         *
;*                   Start Date : May 25, 1994                             *
;*                                                                         *
;*                  Last Update : September 14, 1994   [IML]               *
;*                                                                         *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
;****************************** Equates ************************************
;






;............................ Shape Types ..................................
;
TRUE	equ	1			; Boolean 'true' value
FALSE	equ	0			; Boolean 'false' value

MAKESHAPE_NORMAL		EQU	0	; 256-color compressed shape
MAKESHAPE_COMPACT		EQU	1	; 16-color shape (built-in color table)
MAKESHAPE_NOCOMP		EQU	2	; non-wwcomped shape
MAKESHAPE_VARIABLE	EQU	4	; <16-color shape with variable #
					; of colors (ColorTable[0] = # of colors)
; old names:
;COLOR_SHAPE		EQU	1	; flag which determines a color shape
;NORM_SHAPE			EQU	2	; flag that indicates non wwcomped shp
;NORM_SHAPE_16		EQU	4	; flag that tells us if we have a variable sized table
					;  variable sized table
;
;...........................................................................
; Drawing flags:
; The low byte is for coordinate transformations.
; The high byte is for drawing effects.
;...........................................................................
;
SHAPE_NORMAL			EQU	0000h	; no options; just a copy
SHAPE_HORZ_REV			EQU	0001h	; reverse horizontally
SHAPE_VERT_REV			EQU	0002h	; reverse vertically
SHAPE_SCALING			EQU	0004h	; scale
SHAPE_VIEWPORT_REL	EQU	0010h	; viewport-relative coordinates
SHAPE_CENTER			EQU	0020h	; use centered coordinates
SHAPE_BOTTOM			EQU 0040h	; Y coord is based on shape bottom pt
SHAPE_FADING			EQU	0100h	; fading effect shape
SHAPE_PREDATOR			EQU	0200h	; predator effect shape
SHAPE_COMPACT			EQU	0400h	; shape is in 16 colors
SHAPE_PRIORITY			EQU	0800h	; priority draw shape
SHAPE_GHOST				EQU	1000h	; ghosting effect
SHAPE_SHADOW			EQU	2000h	; shadow effect
SHAPE_PARTIAL			EQU	4000h	; partial predator effect
SHAPE_COLOR				EQU	8000h	; use alternative color table effect

SHAPE_EFFECTS			EQU    03F00h	; shape effect flags

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
PRED_MASK		EQU	0007h	; mask used for predator pixel puts

;---------------------------------------------------------------------------
; This table is a list of the local stack variables in the function
; Draw_Shape.  Many other functions in other modules access these variables
; on the stack.  Since the BP is not changed when these other functions are
; called by Draw_Shape (possibly indirectly), they can also access these
; stack varibles.  When adding or removing from the table, one must be very
; careful to change the offsets.
;---------------------------------------------------------------------------
;.......................... proc addresses .................................
LSkipRout	EQU DWORD PTR ebp - 04h		;DWORD  pointer to the skip routine
RSkipRout	EQU DWORD PTR ebp - 08h		;DWORD  pointer to the skip routine
DrawRout	EQU DWORD PTR ebp - 0Ch			;DWORD  pointer to the draw routine
;........................ optional arguments ...............................
ColorTable	EQU DWORD PTR ebp - 10h		;DWORD	ptr to the shapes color table
FadingTable	EQU DWORD PTR ebp - 14h		;DWORD  ptr to the fading table

FadingNum	EQU DWORD PTR ebp - 18h		;DWORD  number of times to fade
IsTranslucent	EQU DWORD PTR ebp - 1Ch	;DWORD	ptr to is_translucent table
Translucent	EQU DWORD PTR ebp - 20h		;DWORD	ptr to actual translucent tbl
PriLevel	EQU BYTE  PTR ebp - 24h			;BYTE	priority level of the object
ScaleX		EQU DWORD PTR ebp - 28h		;DWORD  the x increment to scale by
ScaleY		EQU DWORD PTR ebp - 2Ch		;DWORD  the y increment to scale by
ShadowingTable	EQU DWORD PTR ebp - 30h	;DWORD	ptr to the shadowing table
;........................ Shape header values ..............................
ShapeType	EQU DWORD PTR ebp - 34h 	;DWORD	shape type
ShapeWidth	EQU DWORD PTR ebp - 38h 	;DWORD	shape's unscaled width
ShapeHeight	EQU DWORD PTR ebp - 3Ch 	;DWORD	shape's unscaled height
UncompDataLen	EQU DWORD PTR ebp - 40h ;DWORD	uncompressed data length
ShapeData	EQU DWORD PTR ebp - 44h 	;DWORD	pointer to shape data
;...................... Scaled shape dimensions ............................
ScaledWidth	EQU DWORD PTR ebp - 48h 	;DWORD	shape's scaled width
ScaledHeight	EQU DWORD PTR ebp - 4Ch ;DWORD	shape's scaled height
;...................... Pixel clipping variables ...........................
LeftClipPixels	EQU DWORD PTR ebp - 50h ;DWORD	# left-clipped pixels
RightClipPixels	EQU DWORD PTR ebp - 54h ;DWORD	# right-clipped pixels
TopClipPixels	EQU DWORD PTR ebp - 58h ;DWORD	# top-clipped pixels
BotClipPixels	EQU DWORD PTR ebp - 5Ch ;DWORD	# bottom-clipped pixels
PixelWidth	EQU DWORD PTR ebp - 60h 	;DWORD	drawable area in pixels
PixelHeight	EQU DWORD PTR ebp - 64h 	;DWORD	drawable area in pixels
;......................... Drawing variables ...............................
NumColors	EQU DWORD PTR ebp - 68h ;DWORD	# colors for 16-color shapes
StartDraw	EQU DWORD PTR ebp - 6Ch ;DWORD	offset of drawing start pos
NextLine	EQU DWORD PTR ebp - 70h ;DWORD	offset of next drawing line
LeftClipBytes	EQU DWORD PTR ebp - 74h ;DWORD	# left-clipped bytes
XTotal		EQU DWORD PTR ebp - 78h ;DWORD	accumulated x-pixels
XTotalInit	EQU DWORD PTR ebp - 7Ch ;DWORD	initial roundoff for XTotal
YTotal		EQU DWORD PTR ebp - 80h ;DWORD	accumulated y-pixels
HeightCount	EQU DWORD PTR ebp - 84h ;DWORD	ht counter for drawing lines
LineStart	EQU DWORD PTR ebp - 88h ;DWORD	address of start of line
WidthCount	EQU DWORD PTR ebp - 8Ch ;DWORD	counts down # bytes skipped
StashReg	EQU DWORD PTR ebp - 90h 	;DWORD	temp variable for draw routines
MaskAdjust	EQU DWORD PTR ebp - 94h ;DWORD  priority buffer offset
BackAdjust	EQU DWORD PTR ebp - 98h ;DWORD  background buffer offset
StashECX	EQU DWORD PTR ebp - 9Ch 	;DWORD  temp variable for ECX register
StashEDX	EQU DWORD PTR ebp -0A0h 	;DWORD  temp variable for EDX register

Local_Size	EQU 00A4h		; Amt of data on stack: 4+last offset

;****************************** Declarations *******************************
;---------------------------------------------------------------------------
; Global variables used by the shape routines, defined in drawshp.asm
;---------------------------------------------------------------------------
GLOBAL C ShapeBuffer:DWORD
GLOBAL C ShapeBufferSize:DWORD
GLOBAL C _MaskPage:DWORD
GLOBAL C _BackGroundPage:DWORD
GLOBAL C PredCount:DWORD
GLOBAL C PredTable:BYTE
GLOBAL C PredValue:DWORD
GLOBAL C PartialPred:DWORD
GLOBAL C PartialCount:DWORD
GLOBAL C Flags:DWORD

;---------------------------------------------------------------------------
; External tables that are defined in ds_table.asm.
;---------------------------------------------------------------------------
GLOBAL LSkipTable:DWORD
GLOBAL RSkipTable:DWORD
GLOBAL DrawTable:DWORD

;------------------------------------------------------------------------------
; Public functions, declared in the order they appear in the function tables.
;--------------------------------------------------------------------------------
GLOBAL	C Not_Supported:NEAR
;	LSkipTable:
GLOBAL Left_Skip:NEAR						; ds_ls
GLOBAL Left_Reverse_Skip:NEAR				; ds_lrs
GLOBAL Left_Skip:NEAR						; ds_ls
GLOBAL Left_Reverse_Skip:NEAR				; ds_lrs
GLOBAL Left_Scale_Skip:NEAR				; ds_lss
GLOBAL Left_Scale_Reverse_Skip:NEAR		; ds_lsrs
GLOBAL Left_Scale_Skip:NEAR				; ds_lss
GLOBAL Left_Scale_Reverse_Skip:NEAR		; ds_lsrs

;	RSkipTable:
GLOBAL Right_Skip:NEAR					   ; ds_rs
GLOBAL Right_Reverse_Skip:NEAR			; ds_rrs
GLOBAL Right_Skip:NEAR					   ; ds_rs
GLOBAL Right_Reverse_Skip:NEAR			; ds_rrs
GLOBAL Right_Scale_Skip:NEAR				; ds_rss
GLOBAL Right_Scale_Reverse_Skip:NEAR	; ds_rsrs
GLOBAL Right_Scale_Skip:NEAR				; ds_rss
GLOBAL Right_Scale_Reverse_Skip:NEAR	; ds_rsrs

;	DrawTable:
GLOBAL Draw_Normal:NEAR					; ds_dn
GLOBAL Draw_Reverse:NEAR				; ds_dr
GLOBAL Draw_Normal:NEAR					; ds_dn
GLOBAL Draw_Reverse:NEAR				; ds_dr
GLOBAL Draw_Scale:NEAR					; ds_ds
GLOBAL Draw_Scale_Reverse:NEAR		; ds_dsr
GLOBAL Draw_Scale:NEAR					; ds_ds
GLOBAL Draw_Scale_Reverse:NEAR		; ds_dsr


;************************* End of shape.inc ********************************

