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


;USE_PROFILER	=1


macro	prologue
Ifdef USE_PROFILER
		global	__PRO:near

		call		__PRO
endif ;USE_PROFILER
endm



macro epilogue
ifdef USE_PROFILER
		global	__EPI:near

		push		ebp
		call		__EPI
		pop			ebp
endif ;USE_PROFILER
endm



