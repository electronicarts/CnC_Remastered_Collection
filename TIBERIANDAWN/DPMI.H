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

/* $Header:   F:\projects\c&c\vcs\code\dpmi.h_v   2.17   16 Oct 1995 16:44:52   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DPMI.H                                                       *
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

#ifndef DPMI_H
#define DPMI_H
#include	<dos.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<mem.h>


extern void output(short port, short data);


class DOSSegmentClass {
		/*
		**	This is the selector/segment value. In real mode it is the segment, in protected
		**	mode it is the selector (also 16 bits). This value is moved into DS or ES when
		**	accessing memory.
		** Note: in Watcom flat addressing, Selector == Segment<<4 (ex: 0A0000h)
		*/
		unsigned int Selector;

		/*
		**	These are C equivalents for pushing and popping the DS segment register. By using
		**	these, it is possible to create very small code that uses a segment and 
		**	offset without damaging the DS register. These are especially useful in protected
		**	mode, but they are legal in real mode as well.
		*/
		void Push_DS(void) {/*__emit__(0x1E);*/};
		void Pop_DS(void) {/*__emit__(0x1F);*/};

	public:
		DOSSegmentClass(void);
		~DOSSegmentClass(void);
		DOSSegmentClass(unsigned short segment, long size=(1024L*64L));

		unsigned int Get_Selector(void);

		/*
		**	This routine is used to assign where the descriptor actually points to in
		**	low DOS memory. In real mode, this is a simple segment assignment and the size
		**	is always 64K regardless of what is specified. In protected mode, the segment
		**	is used to update the selector and the size can be any length.
		** In Watcom flat mode, it sets Selector == segment<<4
		*/
		void Assign(unsigned short segment, long size=(1024L*64L));

		/*
		**	These routines will move the data to/from regular memory and the segment/descriptor
		**	memory.
		*/
		void Copy_To(void *source, int dest, int size);
		void Copy_From(void *dest, int source, int size);
		void Copy_Word_To(short data, int dest);
		void Copy_Byte_To(char data, int dest);
		void Copy_DWord_To(long data, int dest);
		short Copy_Word_From(int source);
		char Copy_Byte_From(int source);
		long Copy_DWord_From(int source);

		/*
		**	These routines move data around between sections of segmented (descriptor) memory.
		**	Typically, this is used when accessing DOS memory in protected mode or when dealing
		**	with hard memory areas such as the screen.
		*/
		static void Copy(DOSSegmentClass &src, int soffset, DOSSegmentClass &dest, int doffset, int size);
		static void Swap(DOSSegmentClass &src, int soffset, DOSSegmentClass &dest, int doffset, int size);
};


inline DOSSegmentClass::DOSSegmentClass(void)
{
	Selector = 0xB0000;
}

inline DOSSegmentClass::~DOSSegmentClass(void)
{
}

inline void DOSSegmentClass::Copy_Word_To(short data, int dest)
{
	*(short *)(Selector+dest) = data;
}

inline void DOSSegmentClass::Copy_Byte_To(char data, int dest)
{
	*(char *)(Selector+dest) = data;
}

inline void DOSSegmentClass::Copy_DWord_To(long data, int dest)
{
	*(long *)(Selector+dest) = data;
}

inline DOSSegmentClass::DOSSegmentClass(unsigned short segment, long)
{
	Assign(segment);
}

inline void DOSSegmentClass::Assign(unsigned short segment, long)
{
	Selector = (long)(segment)<<4L;
}

inline void DOSSegmentClass::Copy_To(void *source, int dest, int size)
{
	memmove((void*)(Selector+dest), source, size);
}

inline void DOSSegmentClass::Copy_From(void *dest, int source, int size)
{
	memmove(dest, (void*)(Selector+source), size);
}
		
inline void DOSSegmentClass::Copy(DOSSegmentClass &src, int soffset, DOSSegmentClass &dest, int doffset, int size) {
	memmove((void*)(dest.Selector+doffset), (void*)(src.Selector+soffset), size);
}

inline short DOSSegmentClass::Copy_Word_From(int source)
{
	return *(short*)(Selector+source);
}

inline char DOSSegmentClass::Copy_Byte_From(int source)
{
	return *(char*)(Selector+source);
}

inline long DOSSegmentClass::Copy_DWord_From(int source)
{
	return *(long*)(Selector+source);
}

inline unsigned int DOSSegmentClass::Get_Selector(void)
{
	return Selector;
}
#endif


