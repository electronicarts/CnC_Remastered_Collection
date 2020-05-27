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

/* $Header:   F:\projects\c&c\vcs\code\link.cpv   2.18   16 Oct 1995 16:52:18   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : LINK.CPP                                                     * 
 *                                                                                             * 
 *                   Programmer : Joe L. Bostic                                                * 
 *                                                                                             * 
 *                   Start Date : 01/15/95                                                     * 
 *                                                                                             * 
 *                  Last Update : January 19, 1995 [JLB]                                       * 
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 *   LinkClass::LinkClass -- Default constructor for linked list object.                       * 
 *   LinkClass::LinkClass -- Copy constructor for linked list object.                          * 
 *   LinkClass::~LinkClass -- Default destructor for linked list object.                       * 
 *   LinkClass::Zap -- Forces the link pointers to NULL.                                       * 
 *   LinkClass::operator= -- Assignment operator for linked list class object.                 * 
 *   LinkClass::Get_Next -- Fetches the next object in list.                                   * 
 *   LinkClass::Get_Prev -- Fetches previous object in linked list.                            * 
 *   LinkClass::Head_Of_List -- Finds the head of the list.                                    * 
 *   LinkClass::Tail_Of_List -- Scans for the object at the end of the list.                   * 
 *   LinkClass::Add -- This object adds itself to the given list                               *
 *   LinkClass::Add_Head -- This gadget makes itself the head of the given list.               *
 *   LinkClass::Add_Tail -- Add myself to the end of the given list.                           *
 *   LinkClass::Remove -- Removes the specified object from the list.                          * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "link.h"


/*********************************************************************************************** 
 * LinkClass::LinkClass -- Default constructor for linked list object.                         * 
 *                                                                                             * 
 *    This is the default constructor for a linked list object. It merely initializes the      * 
 *    next and previous pointers to NULL.                                                      * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/15/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass::LinkClass(void) 
{
	LinkClass::Zap();
}


/*********************************************************************************************** 
 * LinkClass::LinkClass -- Copy constructor for linked list object.                            * 
 *                                                                                             * 
 *    This copy constructor, unlike the assigment operator, doesn't have to deal with an       * 
 *    already initialized and legal link object to the left of the "=". It merely puts the     * 
 *    destination object into the same list as the source object.                              * 
 *                                                                                             * 
 * INPUT:   link  -- Reference to the object on the right of the "=".                          * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/16/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass::LinkClass(LinkClass & link)
{
	LinkClass::Zap();
	Add(link);
}	


/*********************************************************************************************** 
 * LinkClass::~LinkClass -- Default destructor for linked list object.                         * 
 *                                                                                             * 
 *    This default destructor will remove the object from any linked list it may be part of.   * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/15/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass::~LinkClass(void) 
{
	Remove();
}


/*********************************************************************************************** 
 * LinkClass::Zap -- Forces the link pointers to NULL.                                         * 
 *                                                                                             * 
 *    This routine will "zap" out the link pointers. This is usually necessary when the link   * 
 *    pointers start in an undefined state, but we KNOW that they aren't pointing to anything  * 
 *    valid. In such a case it becomes necessary to zap them so that when the object is added  * 
 *    to a list, it will be added corrrectly.                                                  * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/19/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
void LinkClass::Zap(void)
{
	Next = 0;
	Prev = 0;
}	


/*********************************************************************************************** 
 * LinkClass::operator= -- Assignment operator for linked list class object.                   * 
 *                                                                                             * 
 *    The assignment operator makes sure that the previous and next pointers remain valid.     * 
 *    Because this class only consists of pointers, the assignment operator doesn't actually   * 
 *    transfer any data from the source object. It merely makes the destination object part    * 
 *    of the same list as the source object. In essence, this is transferring information      * 
 *    but not the actual values.                                                               * 
 *                                                                                             * 
 *    If the destination object is already part of another list, it is removed from that list  * 
 *    before being added to the source object's list. This ensures that either list remains    * 
 *    in a valid condition.                                                                    * 
 *                                                                                             * 
 * INPUT:   link  -- The object to the right of the "=" operator.                              * 
 *                                                                                             * 
 * OUTPUT:  Returns a reference to the rightmost object -- per standard assigment rules.       * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/16/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass & LinkClass::operator=(LinkClass & link)
{
	Remove();
	Add(link);
	return(link);
}	


/*********************************************************************************************** 
 * LinkClass::Get_Next -- Fetches the next object in list.                                     * 
 *                                                                                             * 
 *    This routine will return with a pointer to the next object in the list. If there are     * 
 *    no more objects, then NULL is returned.                                                  * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns with pointer to next object in list or NULL if at end of list.             * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/15/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass * LinkClass::Get_Next(void) const
{
	return(Next);
}


/*********************************************************************************************** 
 * LinkClass::Get_Prev -- Fetches previous object in linked list.                              * 
 *                                                                                             * 
 *    Use this routine to get a pointer to the previous object in the linked list. If there    * 
 *    are no previous objects (such as at the head of the list), then NULL is returned.        * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns with a pointer to the previous object in the list or NULL if none.         * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/15/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass * LinkClass::Get_Prev(void) const
{
	return(Prev);
}


/*********************************************************************************************** 
 * LinkClass::Head_Of_List -- Finds the head of the list.                                      * 
 *                                                                                             * 
 *    Use this routine to scan for and return a reference to the object at the head of the     * 
 *    list.                                                                                    * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns with a reference to the object at the head of the list.                    * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/19/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass const & LinkClass::Head_Of_List(void) const
{
	LinkClass const * link = this;
	while (link->Prev) {
		link = link->Prev;
		if (link == this) break;		// Safety check
	}
	return(*link);
}	


/*********************************************************************************************** 
 * LinkClass::Tail_Of_List -- Scans for the object at the end of the list.                     * 
 *                                                                                             * 
 *    Use this routine to scan for and return a reference to the object at the end of the      * 
 *    list.                                                                                    * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns with a reference to the object at the end of the list.                     * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/19/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass const & LinkClass::Tail_Of_List(void) const
{
	LinkClass const * link = this;
	while (link->Next) {
		link = link->Next;
		if (link == this) break;		// Safety check
	}
	return(*link);
}	


/*********************************************************************************************** 
 * LinkClass::Add -- This object adds itself to the given list                                 *
 *                                                                                             * 
 *    Use this routine to add a link object to the list, but to be added right after the       * 
 *    given link. This allows inserting a link in the middle of the chain. A quite necessary   * 
 *    ability if the chain is order dependant (e.g., the gadget system).                       * 
 *                                                                                             * 
 * INPUT:   list -- gadget object to add this one to                                           *
 *                                                                                             * 
 * OUTPUT:  Returns with a pointer to the head of the list.                                    * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/19/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass & LinkClass::Add(LinkClass & list)
{
	LinkClass *ptr;

	/*
	**	Save ptr to next gadget.
	*/
	ptr = list.Next;

	/*
	**	Link myself in after 'list'.
	*/
	list.Next = this;
	Prev = &list;

	/*
	**	Link myself to next gadget, if there is one.
	*/
	Next = ptr;
	if (ptr) {
		ptr->Prev = this;
	}

	return(Head_Of_List());
}	


/*********************************************************************************************** 
 * LinkClass::Add_Head -- This gadget makes itself the head of the given list.                 *
 *                                                                                             * 
 * INPUT:   list -- the list to make myself the head of                                        *
 *                                                                                             * 
 * OUTPUT:  Returns with a reference to the object at the head of the list. This should be     * 
 *          the same object that is passed in.                                                 * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/19/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass & LinkClass::Add_Head(LinkClass & list) 
{
	LinkClass *ptr;

	/*
	**	Get head of given list.
	*/
	ptr = &list.Head_Of_List();

	/*
	**	Link myself in front of it.
	*/
	ptr->Prev = this;
	Next = ptr;
	Prev = NULL;

	return(*this);
}


/*********************************************************************************************** 
 * LinkClass::Add_Tail -- Add myself to the end of the given list.                             *
 *                                                                                             * 
 * INPUT:   list -- list to add myself to                                                      *
 *                                                                                             * 
 * OUTPUT:  the head of the list                                                               *
 *                                                                                             * 
 * WARNINGS:   The previous and next pointers for the added object MUST have been properly     * 
 *             initialized for this routine to work correctly.                                 * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/15/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass & LinkClass::Add_Tail(LinkClass & list) 
{
	LinkClass *ptr;

	/*
	**	Get head of given list.
	*/
	ptr = &list.Tail_Of_List();

	/*
	**	Link myself in front of it.
	*/
	ptr->Next = this;
	Prev = ptr;
	Next = NULL;

	return(Head_Of_List());
}


/*********************************************************************************************** 
 * LinkClass::Remove -- Removes the specified object from the list.                            * 
 *                                                                                             * 
 *    This routine will remove the specified object from the list of objects. Because of the   * 
 *    previous and next pointers, it is possible to remove an object from the list without     * 
 *    knowing the head of the list. To do this, just call Remove() with the parameter of       * 
 *    "this".                                                                                  * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns with the new head of list.                                                 * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/15/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass * LinkClass::Remove(void) 
{
	LinkClass * head = &Head_Of_List();
	LinkClass * tail = &Tail_Of_List();

	if (Prev) {
		Prev->Next = Next;
	}
	if (Next) {
		Next->Prev = Prev;
	}
	Prev = 0;
	Next = 0;

	if (head==this) {
		if (tail==this) {
			return(0);
		}
		return(&tail->Head_Of_List());
	}
	return(head);
}


