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

/* $Header:   F:\projects\c&c\vcs\code\heap.h_v   2.15   16 Oct 1995 16:47:08   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : HEAP.H                                                       * 
 *                                                                                             * 
 *                   Programmer : Joe L. Bostic                                                * 
 *                                                                                             * 
 *                   Start Date : 02/18/95                                                     * 
 *                                                                                             * 
 *                  Last Update : February 18, 1995 [JLB]                                      * 
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef HEAP_H
#define HEAP_H

#include "vector.h"

/**************************************************************************
**	This is a block memory managment handler. It is used when memory is to
**	be treated as a series of blocks of fixed size. This is similar to an
**	array of integral types, but unlike such an array, the memory blocks
**	are annonymous. This facilitates the use of this class when overloading
**	the new and delete operators for a normal class object.
*/
class FixedHeapClass
{
	public:
		FixedHeapClass(int size);
		virtual ~FixedHeapClass(void);

		int ID(void const * pointer);
		int Count(void) {return ActiveCount;};
		int Length(void) {return TotalCount;};
		int Avail(void) {return TotalCount-ActiveCount;};

		virtual int Set_Heap(int count, void * buffer=0);
		virtual void * Allocate(void);
		virtual void Clear(void);
		virtual int Free(void * pointer);
		virtual int Free_All(void);

	protected:
		void * operator[](int index) {return ((char *)Buffer) + (index * Size);};

		/*
		**	If the memory block buffer was allocated by this class, then this flag
		**	will be true. The block must be deallocated by this class if true.
		*/
		unsigned IsAllocated:1;

		/*
		**	This is the size of each sub-block within the buffer.
		*/
		int Size;

		/*
		**	This records the absolute number of sub-blocks in the buffer.
		*/
		int TotalCount;

		/*
		**	This is the total blocks allocated out of the heap. This number
		**	will never exceed Count.
		*/
		int ActiveCount;

		/*
		**	Pointer to the heap's memory buffer.
		*/
		void * Buffer;

		/*
		**	This is a boolean vector array of allocation flag bits.
		*/
		BooleanVectorClass FreeFlag;

	private:
		// The assignment operator is not supported.
		FixedHeapClass & operator = (FixedHeapClass const &);

		// The copy constructor is not supported.
		FixedHeapClass(FixedHeapClass const &);
};


/**************************************************************************
**	This template serves only as an interface to the heap manager class. By
**	using this template, the object pointers are automatically converted
**	to the correct type without any code overhead.
*/
template<class T>
class TFixedHeapClass : public FixedHeapClass
{
	public:
		TFixedHeapClass(void) : FixedHeapClass(sizeof(T)) {};
		virtual ~TFixedHeapClass(void) {};

		int ID(T const * pointer) {return FixedHeapClass::ID(pointer);};

		virtual T * Alloc(void) {return (T*)FixedHeapClass::Allocate();};
		virtual int Free(T * pointer) {FixedHeapClass::Free(pointer);};

	protected:
		T & operator[](int index) {return *(((char *)Buffer) + (index * Size));};
};


/**************************************************************************
**	This is a derivative of the fixed heap class. This class adds the 
**	ability to quickly iterate through the active (allocated) objects. Since the
**	active array is a sequence of pointers, the overhead of this class
**	is 4 bytes per potential allocated object (be warned).
*/
class FixedIHeapClass : public FixedHeapClass
{
	public:
		FixedIHeapClass(int size) : FixedHeapClass(size) {};
		virtual ~FixedIHeapClass(void) {};

		virtual int Set_Heap(int count, void * buffer=0);
		virtual void * Allocate(void);
		virtual void Clear(void);
		virtual int Free(void * pointer);
		virtual int Free_All(void);

		virtual void * Active_Ptr(int index) {return ActivePointers[index];};

		/*
		**	This is an array of pointers to allocated objects. Using this array
		**	to control iteration through the objects ensures a minimum of processing.
		**	It also allows access to this array so that custom sorting can be
		**	performed.
		*/
		DynamicVectorClass<void *> ActivePointers;
};


/**************************************************************************
**	This template serves only as an interface to the iteratable heap manager
**	class. By using this template, the object pointers are automatically converted
**	to the correct type without any code overhead.
*/
template<class T>
class TFixedIHeapClass : public FixedIHeapClass
{
	public:
		TFixedIHeapClass(void) : FixedIHeapClass(sizeof(T)) {};
		virtual ~TFixedIHeapClass(void) {};

		int ID(T const * pointer) {return FixedIHeapClass::ID(pointer);};
		virtual T * Alloc(void) {return (T*)FixedIHeapClass::Allocate();};
		virtual int Free(T * pointer) {return FixedIHeapClass::Free(pointer);};
		virtual int Free(void * pointer) {return FixedIHeapClass::Free(pointer);};
		virtual int Save(FileClass & );
		virtual int Load(FileClass & );
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		virtual T * Ptr(int index) {return (T*)FixedIHeapClass::ActivePointers[index];};
		virtual T * Raw_Ptr(int index) {return (index >= 0 && index < Length()) ? (T*)((*this)[index]) : NULL;};
};


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

	