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

/* $Header:   F:\projects\c&c\vcs\code\vector.cpv   2.17   16 Oct 1995 16:49:26   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : VECTOR.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 02/19/95                                                     *
 *                                                                                             *
 *                  Last Update : July 18, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BooleanVectorClass::BooleanVectorClass -- Copy constructor fo boolean array.              *
 *   BooleanVectorClass::BooleanVectorClass -- Explicit data buffer constructor.               *
 *   BooleanVectorClass::Clear -- Resets boolean vector to empty state.                        *
 *   BooleanVectorClass::Fixup -- Updates the boolean vector to a known state.                 *
 *   BooleanVectorClass::Reset -- Clear all boolean values in array.                           *
 *   BooleanVectorClass::Resize -- Resizes a boolean vector object.                            *
 *   BooleanVectorClass::Set -- Forces all boolean elements to true.                           *
 *   BooleanVectorClass::operator = -- Assignment operator.                                    *
 *   BooleanVectorClass::operator == -- Comparison operator for boolean vector.                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include "vector.h"
//#include	<mem.h>
#include	<stdio.h>

/*
**	The following template function can be located here ONLY if all the instatiations are
**	declared in a header file this module includes. By placing the template functions here,
**	it speeds up compiler operation and reduces object module size.
*/
#if (0)	                               // Moved to header

/***********************************************************************************************
 * VectorClass<T>::VectorClass -- Constructor for vector class.                                *
 *                                                                                             *
 *    This constructor for the vector class is passed the initial size of the vector and an    *
 *    optional pointer to a preallocated block of memory that the vector will be placed in.    *
 *    If this optional pointer is NULL (or not provided), then the vector is allocated out     *
 *    of free store (with the "new" operator).                                                 *
 *                                                                                             *
 * INPUT:   size  -- The number of elements to initialize this vector to.                      *
 *                                                                                             *
 *          array -- Optional pointer to a previously allocated memory block to hold the       *
 *                   vector.                                                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
VectorClass<T>::VectorClass(unsigned size, T const * array)
{
	Vector = 0;
	VectorMax = size;
	IsAllocated = false;

	/*
	**	Allocate the vector. The default constructor will be called for every
	**	object in this vector.
	*/
	if (size) {
		if (array) {
			Vector = new((void*)array) T[size];
		} else {
			Vector = new T[size];
			IsAllocated = true;
		}
	}
}


/***********************************************************************************************
 * VectorClass<T>::~VectorClass -- Default destructor for vector class.                        *
 *                                                                                             *
 *    This is the default destructor for the vector class. It will deallocate any memory       *
 *    that it may have allocated.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
VectorClass<T>::~VectorClass(void)
{
	VectorClass<T>::Clear();
}


/***********************************************************************************************
 * VectorClass<T>::VectorClass -- Copy constructor for vector object.                          *
 *                                                                                             *
 *    This is the copy constructor for the vector class. It will duplicate the provided        *
 *    vector into the new vector being created.                                                *
 *                                                                                             *
 * INPUT:   vector   -- Reference to the vector to use as a copy.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
VectorClass<T>::VectorClass(VectorClass<T> const & vector)
{
	VectorMax = 0;
	IsAllocated = false;
	Vector = 0;
	*this = vector;
}


/***********************************************************************************************
 * VectorClass<T>::operator = -- The assignment operator.                                      *
 *                                                                                             *
 *    This the the assignment operator for vector objects. It will alter the existing lvalue   *
 *    vector to duplicate the rvalue one.                                                      *
 *                                                                                             *
 * INPUT:   vector   -- The rvalue vector to copy into the lvalue one.                         *
 *                                                                                             *
 * OUTPUT:  Returns with reference to the newly copied vector.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
VectorClass<T> & VectorClass<T>::operator =(VectorClass<T> const & vector)
{
	Clear();
	VectorMax = vector.Length();
	if (VectorMax) {
		Vector = new T[VectorMax];
		if (Vector) {
			IsAllocated = true;
			for (int index = 0; index < (int)VectorMax; index++) {
				Vector[index] = vector[index];
			}
		}
	} else {
		Vector = 0;
		IsAllocated = false;
	}
	return(*this);
}


/***********************************************************************************************
 * VectorClass<T>::operator == -- Equality operator for vector objects.                        *
 *                                                                                             *
 *    This operator compares two vectors for equality. It does this by performing an object    *
 *    by object comparison between the two vectors.                                            *
 *                                                                                             *
 * INPUT:   vector   -- The right vector expression.                                           *
 *                                                                                             *
 * OUTPUT:  bool; Are the two vectors essentially equal? (do they contain comparable elements  *
 *                in the same order?)                                                          *
 *                                                                                             *
 * WARNINGS:   The equality operator must exist for the objects that this vector contains.     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
int VectorClass<T>::operator == (VectorClass<T> const & vector) const
{
	if (VectorMax == vector.Length()) {
		for (int index = 0; index < (int)VectorMax; index++) {
			if (Vector[index] != vector[index]) {
				return(false);
			}
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * VectorClass<T>::ID -- Pointer based conversion to index number.                             *
 *                                                                                             *
 *    Use this routine to convert a pointer to an element in the vector back into the index    *
 *    number of that object. This routine ONLY works with actual pointers to object within     *
 *    the vector. For "equivalent" object index number (such as with similar integral values)  *
 *    then use the "by value" index number ID function.                                        *
 *                                                                                             *
 * INPUT:   pointer  -- Pointer to an actual object in the vector.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the index number for the object pointed to by the parameter.          *
 *                                                                                             *
 * WARNINGS:   This routine is only valid for actual pointers to object that exist within      *
 *             the vector. All other object pointers will yield undefined results.             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
inline int VectorClass<T>::ID(T const * ptr)
{
	return(((unsigned long)ptr - (unsigned long)&(*this)[0]) / sizeof(T));
}


/***********************************************************************************************
 * VectorClass<T>::ID -- Finds object ID based on value.                                       *
 *                                                                                             *
 *    Use this routine to find the index value of an object with equivalent value in the       *
 *    vector. Typical use of this would be for integral types.                                 *
 *                                                                                             *
 * INPUT:   object   -- Reference to the object that is to be looked up in the vector.         *
 *                                                                                             *
 * OUTPUT:  Returns with the index number of the object that is equivalent to the one          *
 *          specified. If no matching value could be found then -1 is returned.                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
int VectorClass<T>::ID(T const & object)
{
	for (int index = 0; index < (int)VectorMax; index++) {
		if ((*this)[index] == object) {
			return(index);
		}
	}
	return(-1);
}


/***********************************************************************************************
 * VectorClass<T>::Clear -- Frees and clears the vector.                                       *
 *                                                                                             *
 *    Use this routine to reset the vector to an empty (non-allocated) state. A vector will    *
 *    free all allocated memory when this routine is called. In order for the vector to be     *
 *    useful after this point, the Resize function must be called to give it element space.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
void VectorClass<T>::Clear(void)
{
	if (Vector && IsAllocated) {
		delete[] Vector;
		Vector = 0;
	}
	IsAllocated = false;
	VectorMax = 0;
}


/***********************************************************************************************
 * VectorClass<T>::Resize -- Changes the size of the vector.                                   *
 *                                                                                             *
 *    This routine is used to change the size (usually to increase) the size of a vector. This *
 *    is the only way to increase the vector's working room (number of elements).              *
 *                                                                                             *
 * INPUT:   newsize  -- The desired size of the vector.                                        *
 *                                                                                             *
 *          array    -- Optional pointer to a previously allocated memory block that the       *
 *                      array will be located in. If this parameter is not supplied, then      *
 *                      the array will be allocated from free store.                           *
 *                                                                                             *
 * OUTPUT:  bool; Was the array resized successfully?                                          *
 *                                                                                             *
 * WARNINGS:   Failure to succeed could be the result of running out of memory.                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
int VectorClass<T>::Resize(unsigned newsize, T const * array)
{
	if (newsize) {

		/*
		**	Allocate a new vector of the size specified. The default constructor
		**	will be called for every object in this vector.
		*/
		T * newptr;
		if (!array) {
			newptr = new T[newsize];
		} else {
			newptr = new((void*)array) T[newsize];
		}
		if (!newptr) {
			return(false);
		}

		/*
		**	If there is an old vector, then it must be copied (as much as is feasable)
		**	to the new vector.
		*/
		if (Vector) {

			/*
			**	Copy as much of the old vector into the new vector as possible. This
			**	presumes that there is a functional assignment operator for each
			**	of the objects in the vector.
			*/
			int copycount = (newsize < VectorMax) ? newsize : VectorMax;
			for (int index = 0; index < copycount; index++) {
				newptr[index] = Vector[index];
			}

			/*
			**	Delete the old vector. This might cause the destructors to be called
			**	for all of the old elements. This makes the implementation of suitable
			**	assignment operator very important. The default assigment operator will
			**	only work for the simplist of objects.
			*/
			if (IsAllocated) {
				delete[] Vector;
				Vector = 0;
			}
		}

		/*
		**	Assign the new vector data to this class.
		*/
		Vector = newptr;
		VectorMax = newsize;
		IsAllocated = (Vector && !array);

	} else {

		/*
		**	Resizing to zero is the same as clearing the vector.
		*/
		Clear();
	}
	return(true);
}


/***********************************************************************************************
 * DynamicVectorClass<T>::DynamicVectorClass -- Constructor for dynamic vector.                *
 *                                                                                             *
 *    This is the normal constructor for the dynamic vector class. It is similar to the normal *
 *    vector class constructor. The vector is initialized to contain the number of elements    *
 *    specified in the "size" parameter. The memory is allocated from free store unless the    *
 *    optional array parameter is provided. In this case it will place the vector at the       *
 *    memory location specified.                                                               *
 *                                                                                             *
 * INPUT:   size  -- The maximum number of objects allowed in this vector.                     *
 *                                                                                             *
 *          array -- Optional pointer to the memory area to place the vector at.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
DynamicVectorClass<T>::DynamicVectorClass(unsigned size, T const * array)
	: VectorClass<T>(size, array)
{
	GrowthStep = 10;
	ActiveCount = 0;
}


/***********************************************************************************************
 * DynamicVectorClass<T>::Resize -- Changes the size of a dynamic vector.                      *
 *                                                                                             *
 *    Use this routine to change the size of the vector. The size changed is the maximum       *
 *    number of allocated objects within this vector. If a memory buffer is provided, then     *
 *    the vector will be located there. Otherwise, the memory will be allocated out of free    *
 *    store.                                                                                   *
 *                                                                                             *
 * INPUT:   newsize  -- The desired maximum size of this vector.                               *
 *                                                                                             *
 *          array    -- Optional pointer to a previosly allocated memory array.                *
 *                                                                                             *
 * OUTPUT:  bool; Was vector successfully resized according to specifications?                 *
 *                                                                                             *
 * WARNINGS:   Failure to resize the vector could be the result of lack of free store.         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
int DynamicVectorClass<T>::Resize(unsigned newsize, T const * array)
{
	if (VectorClass<T>::Resize(newsize, array)) {
		if (Length() < (unsigned)ActiveCount) ActiveCount = Length();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * DynamicVectorClass<T>::ID -- Find matching value in the dynamic vector.                     *
 *                                                                                             *
 *    Use this routine to find a matching object (by value) in the vector. Unlike the base     *
 *    class ID function of similar name, this one restricts the scan to the current number     *
 *    of valid objects.                                                                        *
 *                                                                                             *
 * INPUT:   object   -- A reference to the object that a match is to be found in the           *
 *                      vector.                                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the index number of the object that is equivalent to the one          *
 *          specified. If no equivalent object could be found then -1 is returned.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
int DynamicVectorClass<T>::ID(T const & object)
{
	for (int index = 0; index < Count(); index++) {
		if ((*this)[index] == object) return(index);
	}
	return(-1);
}


/***********************************************************************************************
 * DynamicVectorClass<T>::Add -- Add an element to the vector.                                 *
 *                                                                                             *
 *    Use this routine to add an element to the vector. The vector will automatically be       *
 *    resized to accomodate the new element IF the vector was allocated previosly and the      *
 *    growth rate is not zero.                                                                 *
 *                                                                                             *
 * INPUT:   object   -- Reference to the object that will be added to the vector.              *
 *                                                                                             *
 * OUTPUT:  bool; Was the object added successfully? If so, the object is added to the end     *
 *                of the vector.                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
int DynamicVectorClass<T>::Add(T const & object)
{
	if ((unsigned)ActiveCount >= Length()) {
		if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
			if (!Resize(Length() + GrowthStep)) {

				/*
				**	Failure to increase the size of the vector is an error condition.
				**	Return with the error flag.
				*/
				return(false);
			}
		} else {

			/*
			**	Increasing the size of this vector is not allowed! Bail this
			**	routine with the error code.
			*/
			return(false);
		}
	}

	/*
	**	There is room for the new object now. Add it to the end of the object vector.
	*/
	(*this)[ActiveCount++] = object;
	return(true);
}


template<class T>
int DynamicVectorClass<T>::Add_Head(T const & object)
{
	if (ActiveCount >= Length()) {
		if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
			if (!Resize(Length() + GrowthStep)) {

				/*
				**	Failure to increase the size of the vector is an error condition.
				**	Return with the error flag.
				*/
				return(false);
			}
		} else {

			/*
			**	Increasing the size of this vector is not allowed! Bail this
			**	routine with the error code.
			*/
			return(false);
		}
	}

	/*
	**	There is room for the new object now. Add it to the end of the object vector.
	*/
	if (ActiveCount) {
		memmove(&(*this)[1], &(*this)[0], ActiveCount * sizeof(T));
	}
	(*this)[0] = object;
	ActiveCount++;
//	(*this)[ActiveCount++] = object;
	return(true);
}


/***********************************************************************************************
 * DynamicVectorClass<T>::Delete -- Remove the specified object from the vector.               *
 *                                                                                             *
 *    This routine will delete the object referenced from the vector. All objects in the       *
 *    vector that follow the one deleted will be moved "down" to fill the hole.                *
 *                                                                                             *
 * INPUT:   object   -- Reference to the object in this vector that is to be deleted.          *
 *                                                                                             *
 * OUTPUT:  bool; Was the object deleted successfully? This should always be true.             *
 *                                                                                             *
 * WARNINGS:   Do no pass a reference to an object that is NOT part of this vector. The        *
 *             results of this are undefined and probably catastrophic.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
int DynamicVectorClass<T>::Delete(T const & object)
{
	int index = ID(object);
	if (index != -1){
		return(Delete(index));
	}else{
		return (false);
	}
}


/***********************************************************************************************
 * DynamicVectorClass<T>::Delete -- Deletes the specified index from the vector.               *
 *                                                                                             *
 *    Use this routine to delete the object at the specified index from the objects in the     *
 *    vector. This routine will move all the remaining objects "down" in order to fill the     *
 *    hole.                                                                                    *
 *                                                                                             *
 * INPUT:   index -- The index number of the object in the vector that is to be deleted.       *
 *                                                                                             *
 * OUTPUT:  bool; Was the object index deleted successfully? Failure might mean that the index *
 *                specified was out of bounds.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T>
int DynamicVectorClass<T>::Delete(int index)
{
	if ((unsigned)index < ActiveCount) {
		ActiveCount--;

		/*
		**	If there are any objects past the index that was deleted, copy those
		**	objects down in order to fill the hole. A simple memory copy is
		**	not sufficient since the vector could contain class objects that
		**	need to use the assignment operator for movement.
		*/
		for (int i = index; i < ActiveCount; i++) {
			(*this)[i] = (*this)[i+1];
		}
		return(true);
	}
	return(false);
}

#endif //Moved to header


//----------------------------------------------------------------------------------------------

/***********************************************************************************************
 * BooleanVectorClass::BooleanVectorClass -- Explicit data buffer constructor.                 *
 *                                                                                             *
 *    This is the constructor for a boolean array. This constructor takes the memory pointer   *
 *    provided as assigns that as the array data pointer.                                      *
 *                                                                                             *
 * INPUT:   size  -- The size of the array (in bits).                                          *
 *                                                                                             *
 *          array -- Pointer to the memory that the array is to use.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   You must make sure that the memory specified is large enough to contain the     *
 *             bits specified.                                                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BooleanVectorClass::BooleanVectorClass(unsigned size, unsigned char * array)
{
	BitArray.Resize(((size + (8-1)) / 8), array);
	LastIndex = -1;
	BitCount = size;
}


/***********************************************************************************************
 * BooleanVectorClass::BooleanVectorClass -- Copy constructor fo boolean array.                *
 *                                                                                             *
 *    This is the copy constructor for a boolean array. It is used to make a duplicate of the  *
 *    boolean array.                                                                           *
 *                                                                                             *
 * INPUT:   vector   -- Reference to the vector to be duplicated.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BooleanVectorClass::BooleanVectorClass(BooleanVectorClass const & vector)
{
	LastIndex = -1;
	*this = vector;
}


/***********************************************************************************************
 * BooleanVectorClass::operator = -- Assignment operator.                                      *
 *                                                                                             *
 *    This routine will make a copy of the specifed boolean vector array. The vector is        *
 *    copied into an already constructed existing vector. The values from the existing vector  *
 *    are destroyed by this copy.                                                              *
 *                                                                                             *
 * INPUT:   vector   -- Reference to the vector to make a copy of.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BooleanVectorClass & BooleanVectorClass::operator =(BooleanVectorClass const & vector)
{
	Fixup();
	Copy = vector.Copy;
	LastIndex = vector.LastIndex;
	BitArray = vector.BitArray;
	BitCount = vector.BitCount;
	return(*this);
}


/***********************************************************************************************
 * BooleanVectorClass::operator == -- Comparison operator for boolean vector.                  *
 *                                                                                             *
 *    This is the comparison operator for a boolean vector class. Boolean vectors are equal    *
 *    if the bit count and bit values are identical.                                           *
 *                                                                                             *
 * INPUT:   vector   -- Reference to the vector to compare to.                                 *
 *                                                                                             *
 * OUTPUT:  Are the boolean vectors identical?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BooleanVectorClass::operator == (const BooleanVectorClass & vector)
{
	Fixup(LastIndex);
	return(BitCount == vector.BitCount && BitArray == vector.BitArray);
}


/***********************************************************************************************
 * BooleanVectorClass::Resize -- Resizes a boolean vector object.                              *
 *                                                                                             *
 *    This routine will resize the boolean vector object. An index value used with a boolean   *
 *    vector must be less than the value specified in as the new size.                         *
 *                                                                                             *
 * INPUT:   size  -- The new maximum size of this boolean vector.                              *
 *                                                                                             *
 * OUTPUT:  Was the boolean vector sized successfully?                                         *
 *                                                                                             *
 * WARNINGS:   The boolean array might be reallocated or even deleted by this routine.         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BooleanVectorClass::Resize(unsigned size)
{
	Fixup();

	if (size) {

		/*
		**	Record the previous bit count of the boolean vector. This is used
		**	to determine if the array has grown in size and thus clearing is
		**	necessary.
		*/
		int oldsize = BitCount;

		/*
		**	Actually resize the bit array. Since this is a bit packed array,
		**	there are 8 elements per byte (rounded up).
		*/
		int success = BitArray.Resize(((size + (8-1)) / 8));

		/*
		**	Since there is no default constructor for bit packed integers, a manual
		**	clearing of the bits is required.
		*/
		BitCount = size;
		if (success && oldsize < (int)size) {
			for (int index = oldsize; index < (int)size; index++) {
				(*this)[index] = 0;
			}
		}

		return(success);
	}

	/*
	**	Resizing to zero is the same as clearing and deallocating the array.
	**	This is always successful.
	*/
	Clear();
	return(true);
}


/***********************************************************************************************
 * BooleanVectorClass::Clear -- Resets boolean vector to empty state.                          *
 *                                                                                             *
 *    This routine will clear out the boolean array. This will free any allocated memory and   *
 *    result in the boolean vector being unusable until the Resize function is subsiquently    *
 *    called.                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The boolean vector cannot be used until it is resized to a non null condition.  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BooleanVectorClass::Clear(void)
{
	Fixup();
	BitCount = 0;
	BitArray.Clear();
}


/***********************************************************************************************
 * BooleanVectorClass::Reset -- Clear all boolean values in array.                             *
 *                                                                                             *
 *    This is the preferred (and quick) method to clear the boolean array to a false condition.*
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BooleanVectorClass::Reset(void)
{
	LastIndex = -1;
	if (BitArray.Length()) {
		memset(&BitArray[0], '\0', BitArray.Length());
	}
}


/***********************************************************************************************
 * BooleanVectorClass::Set -- Forces all boolean elements to true.                             *
 *                                                                                             *
 *    This is the preferred (and fast) way to set all boolean elements to true.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BooleanVectorClass::Set(void)
{
	LastIndex = -1;
	if (BitArray.Length()) {
		memset(&BitArray[0], '\xFF', BitArray.Length());
	}
}


/***********************************************************************************************
 * BooleanVectorClass::Fixup -- Updates the boolean vector to a known state.                   *
 *                                                                                             *
 *    Use this routine to set the boolean value copy to match the appropriate bit in the       *
 *    boolean array. The boolean array will be updated with any changes from the last time     *
 *    a value was fetched from the boolean vector. By using this update method, the boolean    *
 *    array can be treated as a normal array even though the elements are composed of          *
 *    otherwise inaccessable bits.                                                             *
 *                                                                                             *
 * INPUT:   index -- The index to set the new copy value to. If the index is -1, then the      *
 *                   previous value will be updated into the vector array, but no new value    *
 *                   will be fetched from it.                                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Always call this routine with "-1" if any direct manipulation of the bit        *
 *             array is to occur. This ensures that the bit array is accurate.                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BooleanVectorClass::Fixup(int index) const
{
	/*
	**	If the requested index value is illegal, then force the index
	**	to be -1. This is the default non-index value.
	*/
	if (index >= BitCount) {
		index = -1;
	}

	/*
	**	If the new index is different than the previous index, there might
	**	be some fixing up required.
	*/
	if (index != LastIndex) {

		/*
		**	If the previously fetched boolean value was changed, then update
		**	the boolean array accordingly.
		*/
		if (LastIndex != -1) {
			Set_Bit((void*)&BitArray[0], LastIndex, Copy);
		}

		/*
		**	If this new current index is valid, then fill in the reference boolean
		**	value with the approriate data from the bit array.
		*/
		if (index != -1) {
			((unsigned char&)Copy) = Get_Bit(&BitArray[0], index);
		}

		((int &)LastIndex) = index;
	}
}

