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

/* $Header:   F:\projects\c&c0\vcs\code\wwalloc.h_v   4.9   07 May 1996 17:14:00   JOE_BOSTIC  $ */


#ifdef __cplusplus
extern "C" {
#endif


/*
**	This should be located in the wwlib32.h file, but is located here for
**	test purposes.
*/
#ifdef __FLAT__
#define	PRIVATE	static
#endif

typedef enum MemoryFlagType {
	MEM_NORMAL = 0x0000,		// Default memory (normal).
	MEM_PUBLIC = 0x0000,		// Default memory (normal).
	MEM_CHIP   = 0x0000,		// Graphic & sound buffer memory (Amiga).
	MEM_UNUSED = 0x0001,		// <unused>
	MEM_SYSTEM = 0x0002,		// Allocate out of system heap (XMS or EMS only).
	MEM_RELAXED= 0x0004,		// Don't worry about page conservation in EMS.
	MEM_TEMP   = 0x0008,		// Temporary allocation (used by library only).
	MEM_CLEAR  = 0x0010,		// Fill memory with '\0' characters.
	MEM_PARA   = 0x0020,		// Paragraph aligned (IBM only).
	MEM_XMS    = 0x0040,		// XMS memory.
	MEM_EMS    = 0x0080,		// EMS memory (not implemented).
	MEM_X      = 0x8000		// Here to force this enum to be unsigned sized.
} MemoryFlagType;
MemoryFlagType operator |(MemoryFlagType, MemoryFlagType);
MemoryFlagType operator &(MemoryFlagType, MemoryFlagType);
MemoryFlagType operator ~(MemoryFlagType);


/* Prototypes for functions defined in this file */
void *cdecl Alloc(unsigned long bytes_to_alloc, MemoryFlagType flags);
void cdecl Free(void const *pointer);
void *cdecl Resize_Alloc(void const *original_ptr, unsigned long new_size_in_bytes);
long cdecl Ram_Free(MemoryFlagType flag);
long cdecl Total_Ram_Free(MemoryFlagType flag);
long cdecl Heap_Size(MemoryFlagType flag);

extern unsigned long cdecl MinRam;		// Record of least memory at worst case.
extern unsigned long cdecl MaxRam;		// Record of total allocated at worst case.

#ifdef __cplusplus
}
#endif

