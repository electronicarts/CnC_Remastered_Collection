//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection


/*
** 
**  Replacement for Shape asm files
** 
**  ST - 12/19/2018 10:15AM
** 
** 
** 
*/


extern "C" unsigned char *_ShapeBuffer = 0;
extern "C" long _ShapeBufferSize = 0;

		

/*
;***************************************************************************
;* SET_SHAPE_BUFFER -- Sets the shape buffer to the given pointer          *
;*                                                                         *
;* This routine will set the shape buffer to the given value and make sure *
;* that the system does not try to compress any shapes that will be larger *
;* than the shape buffer.						   *
;*                                                                         *
;* INPUT:	void * - pointer to the shape buffer                       *
;*		int    - size of the buffer which has been passed in	   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* PROTO:       VOID *Set_Shape_Bufer(void *buffer, int size);		   *
;*                                                                         *
;* HISTORY:                                                                *
;*   10/26/1994 PWG : Created.                                             *
;*=========================================================================*
*/ 
extern "C" void __cdecl Set_Shape_Buffer(void const *buffer, int size)
{
	_ShapeBuffer = (unsigned char *)buffer;
	_ShapeBufferSize = size;
}



#if (0)
global	C ShapeBuffer		:dword
global	C ShapeBufferSize	:dword
global	C _ShapeBuffer		:dword
global	C _ShapeBufferSize	:dword
global	C Set_Shape_Buffer	:near

DATASEG
label	ShapeBuffer	dword
_ShapeBuffer		dd	0

label	ShapeBufferSize	dword
_ShapeBufferSize	dd	0

CODESEG

;***************************************************************************
;* SET_SHAPE_BUFFER -- Sets the shape buffer to the given pointer          *
;*                                                                         *
;* This routine will set the shape buffer to the given value and make sure *
;* that the system does not try to compress any shapes that will be larger *
;* than the shape buffer.						   *
;*                                                                         *
;* INPUT:	void * - pointer to the shape buffer                       *
;*		int    - size of the buffer which has been passed in	   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* PROTO:       VOID *Set_Shape_Bufer(void *buffer, int size);		   *
;*                                                                         *
;* HISTORY:                                                                *
;*   10/26/1994 PWG : Created.                                             *
;*=========================================================================*
GLOBAL	C Set_Shape_Buffer:NEAR

PROC	Set_Shape_Buffer C near
	USES	eax

	ARG	buff:DWORD
	ARG	size:DWORD

	mov	eax,[size]
	mov	[_ShapeBufferSize],eax

	mov	eax,[buff]
	mov	[_ShapeBuffer],eax
	ret

	ENDP	Set_Shape_Buffer
END

#endif