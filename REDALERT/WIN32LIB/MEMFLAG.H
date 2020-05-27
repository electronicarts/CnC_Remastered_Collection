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

/***************************************************************************
 **     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Memory System                            *
 *                                                                         *
 *                    File Name : MEMFLAG.H                                *
 *                                                                         *
 *                   Programmer : Jeff Wilson                              *
 *                                                                         *
 *                   Start Date : April 4, 1994                            *
 *                                                                         *
 *                  Last Update : September 8, 1994   [IML]                *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef MEMFLAG_H
#define MEMFLAG_H
// Memory Flags
/*
**	Memory allocation flags.  These are the flags that are passed into Alloc
**	in order to control the type of memory allocated.
*/
typedef enum {
	MEM_NORMAL = 0x0000,		// Default memory (normal).
	MEM_NEW	  = 0x0001,		// Called by the operator new and was overloaded.
	MEM_CLEAR  = 0x0002,		// Clear memory before returning.
	MEM_REAL   = 0x0004,		// Clear memory before returning.
	MEM_TEMP   = 0x0008,		// Clear memory before returning.
	MEM_LOCK   = 0x0010,		// Lock the memory that we allocated
} MemoryFlagType;


/*
** Prototypes for VMPAGEIN.ASM
*/
extern "C"{
	void __cdecl Force_VM_Page_In (void *buffer, int length);
}

/*=========================================================================*/
/* The following prototypes are for the file: ALLOC.CPP							*/
/*=========================================================================*/

void * operator new(size_t size, MemoryFlagType flag);
void * operator new[] (size_t size, MemoryFlagType flag);
void	*Alloc(unsigned long bytes_to_alloc, MemoryFlagType flags);
void	Free(void const *pointer);
void	DPMI_Lock(VOID const *ptr, long const size);
void	DPMI_Unlock(void const *ptr, long const size);
void	*Resize_Alloc(void *original_ptr, unsigned long new_size_in_bytes);
long	Ram_Free(MemoryFlagType flag);
long	Heap_Size(MemoryFlagType flag);
long	Total_Ram_Free(MemoryFlagType flag);

//#pragma option -Jgd

inline void * operator new(size_t size, MemoryFlagType flag)
{
	return(Alloc(size, flag));
}
inline void * operator new[] (size_t size, MemoryFlagType flag)
{
	return(Alloc(size, flag));
}

//#pragma option -Jgd

/*=========================================================================*/
/* The following prototypes are for the file: MEM_COPY.ASM						*/
/*=========================================================================*/

extern "C" {
	void __cdecl Mem_Copy(void const *source, void *dest, unsigned long bytes_to_copy);
}


inline void *Add_Long_To_Pointer(void const *ptr, long size)
{
 	return ((void *) ( (char const *) ptr + size));
}

extern void (*Memory_Error)(void);
extern void (*Memory_Error_Exit)(char *string);

extern unsigned long MinRam;		// Record of least memory at worst case.
extern unsigned long MaxRam;		// Record of total allocated at worst case.


#endif
