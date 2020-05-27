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

; This is the control structure at the start of a loaded icon set.  It must match
; the structure in ICONSET.C!  This structure MUST be a multiple of 16 bytes long.

ifdef NO_WAY_THIS_WILL_BE_DEFINED_HAHAHAHA

	STRUC	IControl_Type
Width		DW	?	; Width in pixels (per icon).
Height		DW	?	; Height in pixels (per icon).
Count		DW	?	; Number of icons in this set.
Allocated	DW	?	; Was this iconset allocated?
Size		DD	?	; Size of entire iconset memory block.
Icons		DD	?	; Offset from buffer start to icon data.
Palettes	DD	?	; Offset from buffer start to palette data.
Remaps		DD	?	; Offset from buffer start to remap index data.
TransFlag	DD	?	; Offset for transparency flag data.
Map		DD	?	; Icon map offset.
	ENDS

else


	STRUC	IControl_Type

Width		DW	?	; Width in pixels (per icon).
Height		DW	?	; Height in pixels (per icon).
Count		DW	?	; Number of icons in this set.
Allocated	DW	?	; Was this iconset allocated?
MapWidth	DW	?
MapHeight	DW	?
Size		DD	?	; Size of entire iconset memory block.
Icons		DD	?	; Offset from buffer start to icon data.
Palettes	DD	?	; Offset from buffer start to palette data.
Remaps		DD	?	; Offset from buffer start to remap index data.
TransFlag	DD	?	; Offset for transparency flag data.
ColorMap	DD	?
Map		DD	?	; Icon map offset.
	ENDS

endif

ICON256	EQU	1
