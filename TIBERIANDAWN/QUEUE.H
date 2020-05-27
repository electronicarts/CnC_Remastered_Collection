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

/* $Header:   F:\projects\c&c\vcs\code\queue.h_v   2.16   16 Oct 1995 16:45:50   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : QUEUE.H                                                      * 
 *                                                                                             * 
 *                   Programmer : Joe L. Bostic                                                * 
 *                                                                                             * 
 *                   Start Date : 12/08/94                                                     * 
 *                                                                                             * 
 *                  Last Update : December 9, 1994 [JLB]                                       * 
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 *   QueueClass<T,size>::Add -- Add object to queue.                                           * 
 *   QueueClass<T,size>::First -- Fetches reference to first object in list.                   * 
 *   QueueClass<T,size>::Init -- Initializes queue to empty state.                             * 
 *   QueueClass<T,size>::Next -- Throws out the head of the line.                              * 
 *   QueueClass<T,size>::operator[] -- Fetches reference to sub object in queue.               * 
 *   QueueClass<T,size>::QueueClass -- Default constructor for QueueClass objects.             * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef QUEUE_H
#define QUEUE_H

#include	"mission.h"
#include	"target.h"
#include	"defines.h"

//#pragma warn -inl

/*
**	This class implements a classic FIFO queue (also known as - standing in line). Objects 
**	are added to the end (tail) of the line. Objects are removed from the start (first) of 
**	the line. A keyboard buffer is a good example of a common computer use of a queue. There
**	is no provision for "taking cuts" or leaving the line once an object has been added.
**
**	The implementation uses a circular list of objects. This allows adding and deleting of
**	elements without any maintenance moves of remaining objects that would otherwise be
**	necessary in a simple array storage method. A side effect of this means that accessing the
**	internal array directly is not allowed. Supporting functions are provided for this purpose.
**
**	WARNING WARNING WARNING WARNING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
**	The size parameter MUST be an exact power of two (2, 4, 8, 16, etc.) otherwise the internal
**	indexing algorithm will fail.
*/
template<class T, int size> 
class QueueClass
{
	public:
		/*
		**	This is the count of the number of objects in the queue. If this count is zero,
		**	then the operator[], First(), and Next() functions are undefined. Check this
		**	value BEFORE calling these functions.
		*/
		const int Count;

		//-------------- Functions --------------------
		QueueClass(void);						// Default constructor.

		/*
		**	The bracket subscript operator functions similarly to the way a normal subscript
		**	operator works except that entry [0] matches the first-in-line and entry
		**	[Count-1] matches the last-in-line. This is ensured regardless of the actual position
		**	of the object in the circular internal list.
		*/
		T & operator[](int);

		/*
		**	This function will return a reference to the "head of the line" object.
		*/
		T & First(void);

		/*
		**	This function clears the list of objects.
		*/
		void Init(void);

		/*
		**	This function discards the head-of-the-line object and advances all the remaining
		**	objects up by one. Mnemonic: Imagine a broadway audition and the director yells
		**	"NEXT!"
		*/
		int Next(void);

		/*
		**	This will add an object to the tail of the line. If there is no more room to add
		**	the object, then false will be returned.
		*/
		int Add(T const &);

	private:
		int Head;								// Index of element in list the longest.
		int Tail;								// Index where next new addition will go.

		T Array[size];							// Raw array of objects.
};


/*********************************************************************************************** 
 * QueueClass<T,size>::QueueClass -- Default constructor for QueueClass objects.               * 
 *                                                                                             * 
 *    This default constructor for QueueClass objects initializes the queue to an empty        * 
 *    state.                                                                                   * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   12/09/1994 JLB : Created.                                                                 * 
 *=============================================================================================*/
template<class T, int size> 
inline QueueClass<T,size>::QueueClass(void) : Count(0)
{
	Init();
}	


/*********************************************************************************************** 
 * QueueClass<T,size>::Init -- Initializes queue to empty state.                               * 
 *                                                                                             * 
 *    This function resets the queue to an empty state.                                        * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   12/09/1994 JLB : Created.                                                                 * 
 *=============================================================================================*/
template<class T, int size> 
inline void QueueClass<T,size>::Init(void)
{
	((int &)Count) = 0;
	Head = 0;
	Tail = 0;
}	


/*********************************************************************************************** 
 * QueueClass<T,size>::Add -- Add object to queue.                                             * 
 *                                                                                             * 
 *    This function is used to add an object to the tail of the line. If the queue cannot      * 
 *    accept any more entries, then the object won't be added and false will be returned.      * 
 *                                                                                             * 
 * INPUT:   object   -- The object that is to be added to the queue.                           * 
 *                                                                                             * 
 * OUTPUT:  bool; Was the object added successfully?                                           * 
 *                                                                                             * 
 * WARNINGS:   If the queue is full, then the object won't be added. Be sure to check for this.* 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   12/09/1994 JLB : Created.                                                                 * 
 *=============================================================================================*/
template<class T, int size>
inline int QueueClass<T,size>::Add(T const &q)
{
	if (Count < size) {
		Array[Tail] = q;
		Tail = (Tail + 1) & (size-1);
		((int &)Count) = Count + 1;
		return(true);
	}
 Mono_Printf( "Queue Add failed Count %d size %d tail %d head %d \n",
	Count, size, Tail, Head );
	return (false);
}


/*********************************************************************************************** 
 * QueueClass<T,size>::Next -- Throws out the head of the line.                                * 
 *                                                                                             * 
 *    This routine is used to discard the object at the head of the line. All remaining        * 
 *    objects "move up" one. No actual movement occurs, merely the index is adjusted, but      * 
 *    the affect is that the next oldest object in the queue will now be returned with the     * 
 *    next call to the First() function.                                                       * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns the number of object remaining in the queue.                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   12/09/1994 JLB : Created.                                                                 * 
 *=============================================================================================*/
template<class T, int size>
inline int QueueClass<T,size>::Next(void)
{
	if (Count) {
		Head = (Head + 1) & (size-1);
		((int &)Count) = Count - 1;
	}
	return (Count);
}	


/*********************************************************************************************** 
 * QueueClass<T,size>::operator[] -- Fetches reference to sub object in queue.                 * 
 *                                                                                             * 
 *    Use this routine to examine individual objects within the queue. The oldest object in    * 
 *    the queue is referenced by an index value of zero. The newest object in the queue is     * 
 *    referenced by a value of Count-1. If there are no objects in the queue, then this        * 
 *    operator is undefined. Although this operator allows examination of the queue, there is  * 
 *    no corresponding ability to insert or delete objects from the middle of the queue.       * 
 *                                                                                             * 
 * INPUT:   index -- The index into the queue of objects. Valid values range from zero to      * 
 *                   Count-1. All other values return an undefined reference!                  * 
 *                                                                                             * 
 * OUTPUT:  Returns with a reference to the object indicated by the index.                     * 
 *                                                                                             * 
 * WARNINGS:   Check to make sure that Count is not zero before using this operator. Failure   * 
 *             to do so will return a reference to an undefined object.                        * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   12/09/1994 JLB : Created.                                                                 * 
 *=============================================================================================*/
template<class T, int size>
inline T & QueueClass<T,size>::operator[](int index)
{
	return Array[(Head + index) & (size-1)];
}	


/*********************************************************************************************** 
 * QueueClass<T,size>::First -- Fetches reference to first object in list.                     * 
 *                                                                                             * 
 *    This routine is used to fetch the first object in the list (head of the line). This      * 
 *    object is the oldest in the list. Typical use of this function is to get and process     * 
 *    the first object so that it may be discarded with the Next() function in order to bring  * 
 *    subsequent objects to the first position.                                                * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns with a reference to the oldest object in the queue.                        * 
 *                                                                                             * 
 * WARNINGS:   If there are no objects in the queue, then this function returns an undefined   * 
 *             reference. Be sure to check Count against zero before calling this function.    * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   12/09/1994 JLB : Created.                                                                 * 
 *=============================================================================================*/
template<class T, int size>
inline T & QueueClass<T,size>::First(void)
{
	return Array[Head];
}	

#endif
