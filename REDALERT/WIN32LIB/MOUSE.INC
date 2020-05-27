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

;*********************************************************************************************** 
;*                                                                                             * 
;*                 Project Name : Westwood 32 bit Library                                      * 
;*                                                                                             * 
;*                    File Name : MOUSE.INC                                                    * 
;*                                                                                             * 
;*                   Programmer : Philip W. Gorrow                                             * 
;*                                                                                             * 
;*                   Start Date : 12/12/95                                                     * 
;*                                                                                             * 
;*                  Last Update : December 12, 1995 [PWG]                                      * 
;*                                                                                             * 
;*---------------------------------------------------------------------------------------------* 
;* Functions:                                                                                  * 
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *


STRUC MouseType
MouseCursor		DD		?			; pointer to the mouse cursor in memory
MouseXHot		DD		?			; X hot spot of the current mouse cursor
MouseYHot		DD		?			; Y hot spot of the current mouse cursor
CursorWidth		DD		?			; Width of mouse cursor in pixels
CursorHeight	DD		?			; Height of the mouse cursor in pixels

MouseBuffer		DD		?			; pointer to background buffer in memory
MouseBuffX		DD		?			; pixel x mouse buffer was preserved at
MouseBuffY		DD		?			; pixel y mouse buffer was preserved at
MaxWidth			DD		?			; Maximum possible width of the background buffer
MaxHeight		DD		?			; Maximum possible height of the background buffer

MouseCXLeft		DD		?			; left hand x position if conditional hide mouse in effect
MouseCYUpper	DD		?			; upper y position if conditional hide mouse in effect
MouseCXRight	DD		?			; right hand x position if conditional hide mouse in effect
MouseCYLower	DD		?			; lower y position if conditional hide mouse in effect
MCFlags			DB		?			; conditional hide mouse flags
MCCount			DB		?			; nesting count for conditional hide mouse

Screen			DD		?			; pointer to the surface mouse was init'd with
PrevCursor		DD		?			; pointer to the prev cursor shape
MouseUpdate		DD		?			; is the mouse being currently updated
State				DD		?

EraseBuffer		DD		?
EraseBuffX		DD		?
EraseBuffY		DD		?
EraseBuffHotX	DD		?
EraseBuffHotY	DD		?
EraseFlags		DD		?
ENDS
