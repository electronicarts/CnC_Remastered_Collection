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

/* $Header: /CounterStrike/CCPTR.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CCPTR.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/07/96                                                     *
 *                                                                                             *
 *                  Last Update : June 7, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef CCPTR_H
#define CCPTR_H

/*
**	The CCPtr class is designed for a specific purpose. It functions like a pointer except that
**	it requires no fixups for saving and loading. If pointer fixups are not an issue, than using
**	regular pointers would be more efficient.
*/
template<class T>
class CCPtr
{
	public:
		CCPtr(void) : ID(-1) {};
		CCPtr(NoInitClass const & ) {};
		CCPtr(T * ptr);

		operator T * (void) const {
			if (ID == -1) return(NULL);
			assert(Heap != NULL && ID < Heap->Length());
			return((T*) (*Heap)[ID]);
		}
		T & operator * (void) const {
			assert(Heap != NULL && ID < Heap->Length());
			return(*(T*)(*Heap)[ID]);
		}
		T * operator -> (void) const {
			if (ID == -1) return(NULL);
			assert(Heap != NULL && ID < Heap->Length());
			return((T*) (*Heap)[ID]);
		}

		bool Is_Valid(void) const {return(ID != -1);}

		bool operator == (CCPtr<T> const & rvalue) const {return(ID == rvalue.ID);}
		bool operator != (CCPtr<T> const & rvalue) const {return(ID != rvalue.ID);}
		bool operator > (CCPtr<T> const & rvalue) const;
		bool operator <= (CCPtr<T> const & rvalue) const {return (rvalue > *this);}
		bool operator < (CCPtr<T> const & rvalue) const {return (*this != rvalue && rvalue > *this);}
		bool operator >= (CCPtr<T> const & rvalue) const {return (*this == rvalue || rvalue > *this);}

		long Raw(void) const {return(ID);}
		void Set_Raw(long value) {ID = value;}

		static void Set_Heap(FixedIHeapClass *heap) {Heap = heap;}

	private:

		static FixedIHeapClass * Heap;

		/*
		**	This is the ID number of the object it refers to. By using an ID number, this class can
		**	be saved and loaded without pointer fixups.
		*/
		int ID;
};

/*
**	These template helper functions tell the compiler what to do in the
**	ambiguous case of a CCPtr on one side and a regular type pointer on the
**	other side. In such a case the compiler could create a temp CCPtr object
**	OR call the conversion operator on the existing CCPtr object. Either way
**	is technically valid, but the compiler doesn't know which is better so it
**	generates an error. These routines force the conversion operator rather than
**	creating a temporary object. This presumes that the conversion operator is
**	cheaper than constructing a temporary and that cheaper solutions are desirable.
*/
template<class T>
int operator == (CCPtr<T> & lvalue, T * rvalue)
{
	return((T*)lvalue == rvalue);
}

template<class T>
int operator == (T * lvalue, CCPtr<T> & rvalue)
{
	return(lvalue == (T*)rvalue);
}

#endif