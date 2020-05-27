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

/* $Header: /CounterStrike/HEAP.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
**	This is a block memory management handler. It is used when memory is to
**	be treated as a series of blocks of fixed size. This is similar to an
**	array of integral types, but unlike such an array, the memory blocks
**	are anonymously. This facilitates the use of this class when overloading
**	the new and delete operators for a normal class object.
*/
class FixedHeapClass
{
	public:
		FixedHeapClass(int size);
		virtual ~FixedHeapClass(void);

		int Count(void) const {return ActiveCount;};
		int Length(void) const {return TotalCount;};
		int Avail(void) const {return TotalCount-ActiveCount;};

		virtual int ID(void const * pointer) const;
		virtual int Set_Heap(int count, void * buffer=0);
		virtual void * Allocate(void);
		virtual void Clear(void);
		virtual int Free(void * pointer);
		virtual int Free_All(void);

		void * operator[](int index) {return ((char *)Buffer) + (index * Size);};
		void const * operator[](int index) const {return ((char *)Buffer) + (index * Size);};

	protected:
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


		virtual int ID(T const * pointer) const {return FixedHeapClass::ID(pointer);};
		virtual T * Alloc(void) {return (T*)FixedHeapClass::Allocate();};
		virtual int Free(T * pointer) {return(FixedHeapClass::Free(pointer));};

		T & operator[](int index) {return *(T *)(((char *)Buffer) + (index * Size));};
		T const & operator[](int index) const {return *(T*)(((char *)Buffer) + (index * Size));};
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
		virtual int Logical_ID(void const * pointer) const;
		virtual int Logical_ID(int id) const {return(Logical_ID((*this)[id]));}

		virtual void * Active_Ptr(int index) {return ActivePointers[index];};
		virtual void const * Active_Ptr(int index) const {return ActivePointers[index];};

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
class FileClass;
template<class T>
class TFixedIHeapClass : public FixedIHeapClass
{
	public:
		TFixedIHeapClass(void) : FixedIHeapClass(sizeof(T)) {};
		virtual ~TFixedIHeapClass(void) {};

		virtual int ID(T const * pointer) const {return FixedIHeapClass::ID(pointer);};
		virtual int Logical_ID(T const * pointer) const {return(FixedIHeapClass::Logical_ID(pointer));}
		virtual int Logical_ID(int id) const {return(FixedIHeapClass::Logical_ID(id));}
		virtual T * Alloc(void) {return (T*)FixedIHeapClass::Allocate();};
		virtual int Free(T * pointer) {return FixedIHeapClass::Free(pointer);};
		virtual int Free(void * pointer) {return FixedIHeapClass::Free(pointer);};
		virtual int Save(Pipe & file) const;
		virtual int Load(Straw & file);
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		virtual T * Ptr(int index) const {return (T*)FixedIHeapClass::ActivePointers[index];};
		virtual T * Raw_Ptr(int index) {return (index >= 0 && index < Length()) ? (T*)((*this)[index]) : NULL;};
};


#endif


