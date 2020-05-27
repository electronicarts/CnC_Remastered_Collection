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

;****************************************************************************
;* bNybbleTablexxxx - ADPCM Lookup table for nybbles
;****************************************************************************

		align	4

bNybbleTableLow	DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2
		DB	00h*2,01h*2,02h*2,03h*2,04h*2,05h*2,06h*2,07h*2,08h*2,09h*2,0Ah*2,0Bh*2,0Ch*2,0Dh*2,0Eh*2,0Fh*2

		align 4

bNybbleTableHigh	DB	00h*2,00h*2,00h*2,00h*2,00h*2,00h*2,00h*2,00h*2,00h*2,00h*2,00h*2,00h*2,00h*2,00h*2,00h*2,00h*2
		DB	01h*2,01h*2,01h*2,01h*2,01h*2,01h*2,01h*2,01h*2,01h*2,01h*2,01h*2,01h*2,01h*2,01h*2,01h*2,01h*2
		DB	02h*2,02h*2,02h*2,02h*2,02h*2,02h*2,02h*2,02h*2,02h*2,02h*2,02h*2,02h*2,02h*2,02h*2,02h*2,02h*2
		DB	03h*2,03h*2,03h*2,03h*2,03h*2,03h*2,03h*2,03h*2,03h*2,03h*2,03h*2,03h*2,03h*2,03h*2,03h*2,03h*2
		DB	04h*2,04h*2,04h*2,04h*2,04h*2,04h*2,04h*2,04h*2,04h*2,04h*2,04h*2,04h*2,04h*2,04h*2,04h*2,04h*2
		DB	05h*2,05h*2,05h*2,05h*2,05h*2,05h*2,05h*2,05h*2,05h*2,05h*2,05h*2,05h*2,05h*2,05h*2,05h*2,05h*2
		DB	06h*2,06h*2,06h*2,06h*2,06h*2,06h*2,06h*2,06h*2,06h*2,06h*2,06h*2,06h*2,06h*2,06h*2,06h*2,06h*2
		DB	07h*2,07h*2,07h*2,07h*2,07h*2,07h*2,07h*2,07h*2,07h*2,07h*2,07h*2,07h*2,07h*2,07h*2,07h*2,07h*2
		DB	08h*2,08h*2,08h*2,08h*2,08h*2,08h*2,08h*2,08h*2,08h*2,08h*2,08h*2,08h*2,08h*2,08h*2,08h*2,08h*2
		DB	09h*2,09h*2,09h*2,09h*2,09h*2,09h*2,09h*2,09h*2,09h*2,09h*2,09h*2,09h*2,09h*2,09h*2,09h*2,09h*2
		DB	0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2,0Ah*2
		DB	0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2,0Bh*2
		DB	0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2,0Ch*2
		DB	0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2,0Dh*2
		DB	0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2,0Eh*2
		DB	0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2,0Fh*2
