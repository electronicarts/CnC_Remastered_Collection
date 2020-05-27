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

/* $Header: /CounterStrike/HEAP.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *                  Last Update : May 6, 1996 [JLB]                                            *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   FixedHeapClass::Allocate -- Allocate a sub-block from the heap.                           *
 *   FixedHeapClass::Clear -- Clears (and frees) the heap manager memory.                      *
 *   FixedHeapClass::FixedHeapClass -- Normal constructor for heap management class.           *
 *   FixedHeapClass::Free -- Frees a sub-block in the heap.                                    *
 *   FixedHeapClass::Free_All -- Frees all objects in the fixed heap.                          *
 *   FixedHeapClass::ID -- Converts a pointer to a sub-block index number.                     *
 *   FixedHeapClass::Set_Heap -- Assigns a memory block for this heap manager.                 *
 *   FixedHeapClass::~FixedHeapClass -- Destructor for the heap manager class.                 *
 *   FixedIHeapClass::Allocate -- Allocate an object from the heap.                            *
 *   FixedIHeapClass::Clear -- Clears the fixed heap of all entries.                           *
 *   FixedIHeapClass::Free -- Frees an object in the heap.                                     *
 *   FixedIHeapClass::Free_All -- Frees all objects out of the indexed heap.                   *
 *   FixedIHeapClass::Logical_ID -- Fetches the logical ID number.                             *
 *   FixedIHeapClass::Set_Heap -- Set the heap to the buffer provided.                         *
 *   TFixedIHeapClass::Code_Pointers -- codes pointers for every object, to prepare for save   *
 *   TFixedIHeapClass::Decode_Pointers -- Decodes all object pointers, for after loading       *
 *   TFixedIHeapClass::Load -- Loads all active objects                                        *
 *   TFixedIHeapClass::Save -- Saves all active objects                                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"
#include	"heap.h"
//#include	<mem.h>
#include	<stdio.h>
#include	<stddef.h>
#include	<conio.h>
#include	<string.h>


template class TFixedIHeapClass<AircraftClass>;
template class TFixedIHeapClass<AircraftTypeClass>;
template class TFixedIHeapClass<AnimClass>;
template class TFixedIHeapClass<AnimTypeClass>;
template class TFixedIHeapClass<BuildingClass>;
template class TFixedIHeapClass<BuildingTypeClass>;
template class TFixedIHeapClass<BulletClass>;
template class TFixedIHeapClass<BulletTypeClass>;
template class TFixedIHeapClass<FactoryClass>;
template class TFixedIHeapClass<HouseClass>;
template class TFixedIHeapClass<HouseTypeClass>;
template class TFixedIHeapClass<InfantryClass>;
template class TFixedIHeapClass<InfantryTypeClass>;
template class TFixedIHeapClass<OverlayClass>;
template class TFixedIHeapClass<OverlayTypeClass>;
template class TFixedIHeapClass<SmudgeClass>;
template class TFixedIHeapClass<SmudgeTypeClass>;
template class TFixedIHeapClass<TeamClass>;
template class TFixedIHeapClass<TeamClass>;
template class TFixedIHeapClass<TeamTypeClass>;
template class TFixedIHeapClass<TemplateClass>;
template class TFixedIHeapClass<TemplateTypeClass>;
template class TFixedIHeapClass<TerrainClass>;
template class TFixedIHeapClass<TerrainTypeClass>;
template class TFixedIHeapClass<TriggerClass>;
template class TFixedIHeapClass<TriggerTypeClass>;
template class TFixedIHeapClass<UnitClass>;
template class TFixedIHeapClass<UnitTypeClass>;
template class TFixedIHeapClass<VesselClass>;
template class TFixedIHeapClass<VesselTypeClass>;
template class TFixedIHeapClass<WarheadTypeClass>;
template class TFixedIHeapClass<WeaponTypeClass>;


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
FixedHeapClass::FixedHeapClass(int size) :
	IsAllocated(false),
	Size(size),
	TotalCount(0),
	ActiveCount(0),
	Buffer(0)
{
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
 * INPUT:   pointer  -- A pointer to the sub-block to convert into an ID number.               *
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
int FixedHeapClass::ID(void const * pointer) const
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
 *    memory allocated by this manager is freed. Any previous pointers to allocated blocks     *
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


/***********************************************************************************************
 * FixedIHeapClass::Clear -- Clears the fixed heap of all entries.                             *
 *                                                                                             *
 *    This routine will clear the entire heap. All memory that was allocation, will be freed   *
 *    by this routine. After calling this routine, the heap must either be resized or          *
 *    a new heap memory block specifically attached, before it can be used again.              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FixedIHeapClass::Clear(void)
{
	FixedHeapClass::Clear();
	ActivePointers.Clear();
}


/***********************************************************************************************
 * FixedIHeapClass::Set_Heap -- Set the heap to the buffer provided.                           *
 *                                                                                             *
 *    This routine will set the heap to use the buffer specified. Use this routine when a      *
 *    pre-allocated buffer is to be used for the heap. A heap that is assigned in this         *
 *    manner cannot be resized.                                                                *
 *                                                                                             *
 * INPUT:   count -- The number of objects that the buffer pointer can be used to track.       *
 *                                                                                             *
 *          buffer   -- Pointer to the buffer to use when keeping track of the objects.        *
 *                                                                                             *
 * OUTPUT:  Was the heap assigned successfully?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int FixedIHeapClass::Set_Heap(int count, void * buffer)
{
	Clear();
	if (FixedHeapClass::Set_Heap(count, buffer)) {
		ActivePointers.Resize(count);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * FixedIHeapClass::Allocate -- Allocate an object from the heap.                              *
 *                                                                                             *
 *    This routine will allocate an object located in the heap. If no free object space        *
 *    could be found, then NULL is returned.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the allocated object memory block.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
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


/***********************************************************************************************
 * FixedIHeapClass::Logical_ID -- Fetches the logical ID number.                               *
 *                                                                                             *
 *    Ths logical ID number of a memory block is the index number of the block as if the       *
 *    heap consisted only of valid allocated blocks. This knowledge comes in handy when        *
 *    the real index number must be anticipated before a memory block packing process.         *
 *                                                                                             *
 * INPUT:   pointer  -- Pointer to an allocated block in the heap.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the logical index number of this block. The number returned must not  *
 *          be used as a regular index into the heap until such time as the heap has been      *
 *          compacted (by some means or another) without modifying the block order.            *
 *                                                                                             *
 * WARNINGS:   Runs in linear time.                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int FixedIHeapClass::Logical_ID(void const * pointer) const
{
	if (pointer != NULL) {
		for (int index = 0; index < Count(); index++) {
			if (Active_Ptr(index) == pointer) {
				return(index);
			}
		}
	}
	return(-1);
}


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
 *   03/12/1996 JLB : Uses in-place new operator for virtual table control.                    *
 *=============================================================================================*/
template<class T>
int TFixedIHeapClass<T>::Save(Pipe & file) const
{
	/*
	** Save the number of instances of this class
	*/
	file.Put(&ActiveCount, sizeof(ActiveCount));

	/*
	** Save each instance of this class
	*/
	for (int i = 0; i < ActiveCount; i++) {

		/*
		** Save the array index of the object, so it can be loaded back into the
		** same array location (so TARGET translations will work)
		*/
		int idx = ID(Ptr(i));
		file.Put(&idx, sizeof(idx));

		/*
		** Save the object itself
		*/
		file.Put(Ptr(i), sizeof(T));
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
int TFixedIHeapClass<T>::Load(Straw & file)
{
	int i;			// loop counter
	int idx;			// object index
	T * ptr;			// object pointer
	int a_count;

	/*
	** Read the number of instances of this class
	*/
	if (file.Get(&a_count, sizeof(a_count)) != sizeof(a_count)) {
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
		if (file.Get(&idx, sizeof(idx)) != sizeof(idx)) {
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
		file.Get(ptr, sizeof(T));
		new(ptr) T(NoInitClass());
//		if (!ptr->Load(file)) {
//			return(false);
//		}
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
