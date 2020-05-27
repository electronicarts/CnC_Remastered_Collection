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

/* $Header: /CounterStrike/DYNAVEC.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***************************************************************************
 *                                                                         *
 *                 Project Name : Red Alert                                *
 *                                                                         *
 *                    File Name : DYNAVEC.CPP                              *
 *                                                                         *
 *                   Programmer : Bill R Randolph                          *
 *                                                                         *
 *                   Start Date : 09/18/96                                 *
 *                                                                         *
 *                  Last Update : September 18, 1996 [BRR]                 *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   DynamicVectorClass<T>::Add -- Add an element to the vector.           *
 *   DynamicVectorClass<T>::Add_Head -- Adds element to head of the list.  *
 *   DynamicVectorClass<T>::Add_Head -- Adds element to head of the list.  *
 *   DynamicVectorClass<T>::Delete -- Deletes specified index from vector. *
 *   DynamicVectorClass<T>::Delete -- Remove specified object from vector. *
 *   DynamicVectorClass<T>::DynamicVectorClass -- Constructor              *
 *   DynamicVectorClass<T>::ID -- Find matching value in dynamic vector.   *
 *   DynamicVectorClass<T>::Resize -- Changes size of a dynamic vector.    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if (0)
#include	"function.h"
#include "vector.h"
#ifdef WINSOCK_IPX
#include	"WSProto.h"
#include	"WSPUDP.h"
#endif	//WINSOCK_IPX
//#include	<mem.h>
#include	<stdio.h>




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
 *          array    -- Optional pointer to a previously allocated memory array.               *
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
 *    resized to accomodate the new element IF the vector was allocated previously and the     *
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
	(*this)[ActiveCount++] = object;
	return(true);
}


/***********************************************************************************************
 * DynamicVectorClass<T>::Add_Head -- Adds element to head of the list.                        *
 *                                                                                             *
 *    This routine will add the specified element to the head of the vector. If necessary,     *
 *    the vector will be expanded accordingly.                                                 *
 *                                                                                             *
 * INPUT:   object   -- Reference to the object to add to the head of this vector.             *
 *                                                                                             *
 * OUTPUT:  bool; Was the object added without error?                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
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
	return(Delete(ID(object)));
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

/************************** end of dynavec.cpp *****************************/
#endif