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
 *                    File Name : MEM.H                                    *
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
#ifndef WWMEM_H
#define WWMEM_H

#include "wwstd.h"
#include "new.h"
#include "memflag.h"

// Defines
//==============




/*=========================================================================*/
/* The following prototypes are for the file: MEM.CPP								*/
/*=========================================================================*/

int  Mem_Init(void *buffer, long size);
void *Mem_Alloc(void *poolptr, long lsize, unsigned long id);
int  Mem_Free(void *poolptr, void *buffer);
void Mem_Reference(void *node);
void Mem_Lock_Block(void *node);
void Mem_In_Use(void *node);
void *Mem_Find(void *poolptr, unsigned long id);
unsigned long Mem_Get_ID(void *node);
void *Mem_Find_Oldest(void *poolptr);
void *Mem_Free_Oldest(void *poolptr);
long Mem_Pool_Size(void *poolptr);
long Mem_Avail(void *poolptr);
long Mem_Largest_Avail(void *poolptr);
void Mem_Cleanup(void *poolptr);


#endif

