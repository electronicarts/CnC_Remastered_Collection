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

/* $Header:   F:\projects\c&c\vcs\code\heap.cpv   2.18   16 Oct 1995 16:49:56   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : HEAP.CPP                                                     * 
 *                                                                                             * 
 *                   Programmer : Joe L. Bostic                                                * 
 *                                                                                             * 
 *                   Start Date : 02/18/95                                                     * 
 *                                                                                             * 
 *                  Last Update : May 22, 1995 [JLB]                                           * 
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 *   FixedIHeapClass::Free -- Frees an object in the heap.                                     * 
 *   FixedHeapClass::FixedHeapClass -- Normal constructor for heap management class.           * 
 *   FixedHeapClass::~FixedHeapClass -- Destructor for the heap manager class.                 * 
 *   FixedHeapClass::Set_Heap -- Assigns a memory block for this heap manager.                 * 
 *   FixedHeapClass::Allocate -- Allocate a sub-block from the heap.                           * 
 *   FixedHeapClass::Free -- Frees a sub-block in the heap.                                    * 
 *   FixedHeapClass::ID -- Converts a pointer to a sub-block index number.                     * 
 *   FixedHeapClass::Clear -- Clears (and frees) the heap manager memory.                      * 
 *   TFixedIHeapClass::Save -- Saves all active objects                                        *
 *   TFixedIHeapClass::Load -- Loads all active objects                                        *
 *   TFixedIHeapClass::Code_Pointers -- codes pointers for every object, to prepare for save   *
 *   TFixedIHeapClass::Decode_Pointers -- Decodes all object pointers, for after loading       *
 *   FixedHeapClass::Free_All -- Frees all objects in the fixed heap.                          * 
 *   FixedIHeapClass::Free_All -- Frees all objects out of the indexed heap.                   * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"
#include	"heap.h"
//#include	<mem.h>
#include	<stdio.h>
#include	<stddef.h>
#include	<conio.h>
#include	<string.h>


/*********************************************************************************************** 
 * FixedHeapClass::FixedHeapClass -- Normal constructor for heap management class.             * 
 *                                                                                             * 
 *    This is the normal constructor used for the heap manager class. This initializes         * 
 *    the class but doesn't yet assign actual heap memory to this manager. That is handled     * 
 *    by the Set_Heap() function.                                                              * 
 *                                                                                             * 
 * INPUT:   size  -- The size of the individual sub-blocks in this heap. This value is         * 
 *                   typically the size of some class or structure.                            * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   The heap must first be assigned a block of memory to manage before it can       * 
 *             be used.                                                                        * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   02/21/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
FixedHeapClass::FixedHeapClass(int size)
{
	Size = size;
	Buffer = 0;
	IsAllocated = false;
	TotalCount = 0;
	ActiveCount = 0;
}	


/*********************************************************************************************** 
 * FixedHeapClass::~FixedHeapClass -- Destructor for the heap manager class.                   * 
 *                                                                                             * 
 *    This is the default constructor for the heap manager class. It handles freeing the       * 
 *    memory assigned to this heap.                                                            * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   02/21/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
FixedHeapClass::~FixedHeapClass(void)
{
	FixedHeapClass::Clear();
}	


/*********************************************************************************************** 
 * FixedHeapClass::Set_Heap -- Assigns a memory block for this heap manager.                   * 
 *                                                                                             * 
 *    This routine is used to assign a memory heap to this object. A memory heap so assigned   * 
 *    will start with all sub-blocks unallocated. After this routine is called, normal         * 
 *    allocation and freeing may occur. This routine will allocate necessary memory if the     * 
 *    buffer parameter is NULL.                                                                * 
 *                                                                                             * 
 * INPUT:   count    -- The number of objects that this heap should manage.                    * 
 *                                                                                             * 
 *          buffer   -- Pointer to pre-allocated buffer that this manager will use. If this    * 
 *                      parameter is NULL, then memory will be automatically allocated.        * 
 *                                                                                             * 
 * OUTPUT:  bool; Was the heap successfully initialized?                                       * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   02/21/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
int FixedHeapClass::Set_Heap(int count, void * buffer)
{
	/*
	**	Clear out the old heap data.
	*/
	Clear();

	/*
	**	If there is no size to the objects in the heap, then this block memory
	**	handler can NEVER function. Return with a failure condition.
	*/
	if (!Size) return(false);

	/*
	**	If there is no count specified, then this indicates that the heap should
	**	be disabled.
	*/
	if (!count) return(true);

	/*
	**	Initialize the free boolean vector and the buffer for the actual
	**	allocation objects.
	*/
	if (FreeFlag.Resize(count)) {
		if (!buffer) {
			buffer = new char[count * Size];
			if (!buffer) {
				FreeFlag.Clear();
				return(false);
			}
			IsAllocated = true;
		}
		Buffer = buffer;
		TotalCount = count;

		memset((unsigned char *)Buffer, 0x00, count * Size);  // Added. Needed to identify bad states in saved games. ST - 10/3/2019 11:49AM		

		return(true);
	}
	return(false);
}	


/*********************************************************************************************** 
 * FixedHeapClass::Allocate -- Allocate a sub-block from the heap.                             * 
 *                                                                                             * 
 *    Finds the first available sub-block in the heap and returns a pointer to it. The sub-    * 
 *    block is marked as allocated by this routine. If there are no more sub-blocks            * 
 *    available, then this routine will return NULL.                                           * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns with a pointer to the newly allocated sub-block.                           * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   02/21/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
void * FixedHeapClass::Allocate(void)
{
	if (ActiveCount < TotalCount) {
		int index = FreeFlag.First_False();

		if (index != -1) {
			ActiveCount++;
			FreeFlag[index] = true;
			return((*this)[index]);
		}
	}
	return(0);
}	


/*********************************************************************************************** 
 * FixedHeapClass::Free -- Frees a sub-block in the heap.                                      * 
 *                                                                                             * 
 *    Use this routine to free a previously allocated sub-block in the heap.                   * 
 *                                                                                             * 
 * INPUT:   pointer  -- A pointer to the sub-block to free. This is the same pointer that      * 
 *                      was returned from the Allocate() function.                             * 
 *                                                                                             * 
 * OUTPUT:  bool; Was the deallocation successful? Failure could indicate a pointer that       * 
 *                doesn't refer to this heap or a null pointer.                                * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   02/21/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
int FixedHeapClass::Free(void * pointer)
{
	if (pointer && ActiveCount) {
		int index = ID(pointer);

		if (index < TotalCount) {
			if (FreeFlag[index]) {
				ActiveCount--;
				FreeFlag[index] = false;

				return(true);	 
			}
		}
	}
	return(false);
}	


/*********************************************************************************************** 
 * FixedHeapClass::ID -- Converts a pointer to a sub-block index number.                       * 
 *                                                                                             * 
 *    Use this routine to convert a pointer (returned by Allocate) into the sub-block          * 
 *    index number. This index number can be used as a form of identifier for the block.       * 
 *                                                                                             * 
 * INPUT:   pointer  -- A pointer to the sub-block to conver into an ID number.                * 
 *                                                                                             * 
 * OUTPUT:  Returns with the index (ID) number for the sub-block specified. This number will   * 
 *          range between 0 and the sub-block max -1. If -1 is returned, then the pointer      * 
 *          was invalid.                                                                       * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   02/21/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
int FixedHeapClass::ID(void const * pointer)
{
	if (pointer && Size) {
		return((int)(((char *)pointer - (char *)Buffer) / Size));
	}
	return(-1);
}	


/*********************************************************************************************** 
 * FixedHeapClass::Clear -- Clears (and frees) the heap manager memory.                        * 
 *                                                                                             * 
 *    This routine is used to bring the heap manager back into a non-functioning state. All    * 
 *    memory allocated by this manager is freeed. Any previous pointers to allocated blocks    * 
 *    from this heap are now invalid.                                                          * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   02/21/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
void FixedHeapClass::Clear(void)
{
	/*
	**	Free the old buffer (if present).
	*/
	if (Buffer && IsAllocated) {
		delete[] Buffer;
	}
	Buffer = 0;
	IsAllocated = false;
	ActiveCount = 0;
	TotalCount = 0;
	FreeFlag.Clear();
}	


/*********************************************************************************************** 
 * FixedHeapClass::Free_All -- Frees all objects in the fixed heap.                            * 
 *                                                                                             * 
 *    This routine will free all previously allocated objects out of the heap. Use this        * 
 *    routine to ensure that the heap is empty.                                                * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Was the heap successfully cleared of all objects?                                  * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   05/22/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
int FixedHeapClass::Free_All(void)
{
	ActiveCount = 0;
	FreeFlag.Reset();
	return(true);
}	


/////////////////////////////////////////////////////////////////////


/*********************************************************************************************** 
 * FixedIHeapClass::Free_All -- Frees all objects out of the indexed heap.                     * 
 *                                                                                             * 
 *    Use this routine to free all previously allocated objects in the heap. This routine will * 
 *    also clear out the allocated object vector as well.                                      * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Was the heap successfully cleared of objects?                                      * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   05/22/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
int FixedIHeapClass::Free_All(void)
{
	ActivePointers.Delete_All();
	return(FixedHeapClass::Free_All());
}


void FixedIHeapClass::Clear(void)
{
	FixedHeapClass::Clear();
	ActivePointers.Clear();
}	


int FixedIHeapClass::Set_Heap(int count, void * buffer)
{
	Clear();
	if (FixedHeapClass::Set_Heap(count, buffer)) {
		ActivePointers.Resize(count);
		return(true);
	}
	return(false);
}	


void * FixedIHeapClass::Allocate(void)
{
	void * ptr = FixedHeapClass::Allocate();
	if (ptr)	{
		ActivePointers.Add(ptr);
		memset (ptr, 0, Size);
	}
	return(ptr);
}	


/*********************************************************************************************** 
 * FixedIHeapClass::Free -- Frees an object in the heap.                                       * 
 *                                                                                             * 
 *    This routine is used to free an object in the heap. Freeing is accomplished by marking   * 
 *    the object's memory as free to be reallocated. The object is also removed from the       * 
 *    allocated object pointer vector.                                                         * 
 *                                                                                             * 
 * INPUT:   pointer  -- Pointer to the object that is to be removed from the heap.             * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   02/21/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
int FixedIHeapClass::Free(void * pointer)
{
	if (FixedHeapClass::Free(pointer)) {
		ActivePointers.Delete(pointer);
	}
	return(false);
}	

#if (0)
/*********************************************************************************************** 
 * TFixedIHeapClass::Save -- Saves all active objects                                          *
 *                                                                                             * 
 * INPUT:   file      file to write to                                                         *
 *                                                                                             * 
 * OUTPUT:  true = OK, false = error                                                           *
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   03/15/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
template<class T>
int TFixedIHeapClass<T>::Save(FileClass &file)
{
	int i;				// loop counter
	int idx;				// object index

	/*
	** Save the number of instances of this class
	*/
	if (file.Write(&ActiveCount, sizeof(ActiveCount)) != sizeof(ActiveCount)) {
		return(false);
	}

	/*
	** Save each instance of this class
	*/
	for (i = 0; i < ActiveCount; i++) {
		/*
		** Save the array index of the object, so it can be loaded back into the
		** same array location (so TARGET translations will work)
		*/
		idx = ID(Ptr(i));
		if (file.Write(&idx, sizeof(idx)) != sizeof(idx)) {
			return(false);
		}

		/*
		** Save the object itself
		*/
		if (!Ptr(i)->Save(file)) {
			return(false);
		}
	}

	return(true);
}	


/*********************************************************************************************** 
 * TFixedIHeapClass::Load -- Loads all active objects                                          *
 *                                                                                             * 
 * INPUT:   file      file to read from                                                        *
 *                                                                                             * 
 * OUTPUT:  true = OK, false = error                                                           *
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   03/15/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
template<class T>
int TFixedIHeapClass<T>::Load(FileClass &file)
{
	int i;			// loop counter
	int idx;			// object index
	T *ptr;			// object pointer
	int a_count;

	/*
	** Read the number of instances of this class
	*/
	if (file.Read(&a_count, sizeof(a_count)) != sizeof(a_count)) {
		return(false);
	}

	/*
	** Error if more objects than we can hold
	*/
	if (a_count > TotalCount) {
		return(false);
	}

	/*
	** Read each class instance
	*/
	for (i = 0; i < a_count; i++) {
		/*
		** Read the object's array index
		*/
		if (file.Read (&idx, sizeof(idx)) != sizeof(idx)) {
			return(false);
		}

		/*
		** Get a pointer to the object, activate that object
		*/
		ptr = (T *)(*this)[idx];
		FreeFlag[idx] = true;
		ActiveCount++;
		ActivePointers.Add(ptr);

		/*
		** Load the object
		*/
		if (!ptr->Load(file)) {
			return(false);
		}
	}

	return(true);
}	


/*********************************************************************************************** 
 * TFixedIHeapClass::Code_Pointers -- codes pointers for every object, to prepare for save     *
 *                                                                                             * 
 * INPUT:   file      file to read from                                                        *
 *                                                                                             * 
 * OUTPUT:  true = OK, false = error                                                           *
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   03/15/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
template<class T>
void TFixedIHeapClass<T>::Code_Pointers(void)
{
	int i;

	for (i = 0; i < ActiveCount; i++) {
		Ptr(i)->Code_Pointers();
	}
}


/*********************************************************************************************** 
 * TFixedIHeapClass::Decode_Pointers -- Decodes all object pointers, for after loading         *
 *                                                                                             * 
 * INPUT:   file      file to read from                                                        *
 *                                                                                             * 
 * OUTPUT:  true = OK, false = error                                                           *
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   03/15/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
template<class T>
void TFixedIHeapClass<T>::Decode_Pointers(void)
{
	int i;

	for (i = 0; i < ActiveCount; i++) {
		Ptr(i)->Decode_Pointers();
	}
}

#endif