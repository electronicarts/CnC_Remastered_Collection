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

/* $Header:   F:\projects\c&c\vcs\code\dpmi.cpv   2.17   16 Oct 1995 16:49:36   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DPMI.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : July 2, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 2, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

//PG_TO_FIX
#if (0)

#ifdef __FLAT__
#pragma inline
#endif

#include	"function.h"
#include "dpmi.h"

#ifndef __FLAT__

void DOSSegmentClass::Swap(DOSSegmentClass &src, int soffset, DOSSegmentClass &dest, int doffset, int size)
{
	if (!size) return;

	unsigned short ssel = src.Selector;
	unsigned short dsel = dest.Selector;

	asm {
		push	es
		push	ds

		mov	si,soffset
		mov	di,doffset
		mov	cx,size
		mov	ax,ssel
		mov	dx,dsel
		mov	ds,ax
		mov	es,dx
	}
again:
	asm {
		mov	al,ds:[si]
		mov	ah,es:[di]
		mov	ds:[si],ah
		mov	es:[di],al
		inc	di
		inc	si
		dec	cx
		jnz	again

		pop	ds
		pop	es
	}
}
#endif


void DOSSegmentClass::Swap(DOSSegmentClass &src, int soffset, DOSSegmentClass &dest, int doffset, int size)
{
	extern void dss_swap(char *src, char *dest, int size);

	#pragma aux dss_swap = 		\
	"again: mov	al,[esi]"	\
		"mov	ah,[edi]"		\
		"mov	[esi],ah"		\
		"stosb"					\
		"inc	esi"				\
		"loop	again"			\
		parm	[esi] [edi] [ecx]	\
		modify [ax];

	if (!size) return;
	dss_swap((char *)(src.Selector + soffset), (char *)(dest.Selector + doffset), size);
}

#ifdef OBSOLETE
void DOSSegmentClass::Copy(DOSSegmentClass &src, int soffset, DOSSegmentClass &dest, int doffset, int size)
{
	extern void dss_copy(char *src, char *dest, int size);
	#pragma aux dss_copy = 		\
		"mov		ebx,ecx"			\
		"shr		ecx,2"			\
		"jecxz	copskip1"		\
		"rep 		movsd"			\
"copskip1: mov ecx,ebx"			\
		"and		ecx,3"			\
		"jecxz	copskip2"		\
		"rep		movsb"			\
"copskip2:"							\
		parm	[esi edi ecx]		\
		modify [ebx];

	if (!size) return;
	dss_copy((char *)(src.Selector + soffset), (char *)(dest.Selector + doffset), size);
}
#endif

#ifdef OBSOLETE
void DOSSegmentClass::Copy_To(void *source, int dest, int size)
{
	extern void dss_copy_to(void *src, (void *)dest, int size);

	#pragma aux dss_copy_to =	\
		"mov		ebx,ecx"			\
		"shr		ecx,2"			\
		"jecxz	cop2skip1"		\
		"rep		movsd"			\
"cop2skip1: mov ecx,ebx"		\
		"and		ecx,3"			\
		"jecxz	cop2skip2"		\
		"rep		movsb"			\
"cop2skip2:"						\
		parm	[esi edi ecx]		\
		modify [ebx];

	if (!size) return;
	dss_copy_to(src, (void *)(Selector + dest), size);

}
#endif

#ifdef OBSOLETE
void DOSSegmentClass::Copy_From(void *dest, int source, int size)
{
	extern void dss_copy_from(void *dest, (void *)source, int size);

	#pragma aux dss_copy_from =	\
		"mov		ebx,ecx"			\
		"shr		ecx,2"			\
		"jecxz	copfskip1"		\
		"rep		movsd"			\
"copfskip1: mov ecx,ebx"		\
		"and		ecx,3"			\
		"jecxz	copfskip2"		\
		"rep		movsb"			\
"copfskip2:"						\
		parm	[edi esi ecx]		\
		modify [ebx];

	if (!size) return;
	dss_copy_from(dest, (void *)(Selector + source), size);
}
#endif


#endif //PG_TO_FIX