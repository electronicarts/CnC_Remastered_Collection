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

/* $Header: /CounterStrike/LISTNODE.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LISTNODE.H                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/16/96                                                     *
 *                                                                                             *
 *                  Last Update : May 16, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef LISTNODE_H
#define LISTNODE_H

#include	<stddef.h>
#include	<assert.h>

/*
**	The "bool" integral type was defined by the C++ comittee in
**	November of '94. Until the compiler supports this, use the following
**	definition.
*/
#ifndef __BORLANDC__
#ifndef TRUE_FALSE_DEFINED
#define TRUE_FALSE_DEFINED
enum {false=0,true=1};
typedef int bool;
#endif
#endif


//#pragma warn -inl

/*
**	This is a doubly linked list node. Typical use of this node is to derive
**	objects from this node. The interface class for this node can be used for
**	added convenience.
*/
class GenericList;
class GenericNode {
	public:
		GenericNode(void) : NextNode(0), PrevNode(0) {}
		~GenericNode(void) {Unlink();}
		GenericNode(GenericNode & node) {node.Link(this);}
		GenericNode & operator = (GenericNode & node) {
			if (&node != this) {
				node.Link(this);
			}
			return(*this);
		}

		void Unlink(void) {
			if (Is_Valid()) {
				PrevNode->NextNode = NextNode;
				NextNode->PrevNode = PrevNode;
				PrevNode = 0;
				NextNode = 0;
			}
		}

		GenericList * Main_List(void) const {
			GenericNode const * node = this;
			while (node->PrevNode) {
				node = PrevNode;
			}
			return((GenericList *)this);
		}
		void Link(GenericNode * node) {
			assert(node != NULL);
			node->Unlink();
			node->NextNode = NextNode;
			node->PrevNode = this;
			if (NextNode) NextNode->PrevNode = node;
			NextNode = node;
		}

		GenericNode * Next(void) const {return(NextNode);}
		GenericNode * Prev(void) const {return(PrevNode);}
		bool Is_Valid(void) const {return(this != NULL && NextNode != NULL && PrevNode != NULL);}

	protected:
		GenericNode * NextNode;
		GenericNode * PrevNode;
};


/*
**	This is a generic list handler. It manages N generic nodes. Use the interface class
**	to the generic list for added convenience.
*/
class GenericList {
	public:
		GenericList(void) {
			FirstNode.Link(&LastNode);
		}

		GenericNode * First(void) const {return(FirstNode.Next());}
		GenericNode * Last(void) const {return(LastNode.Prev());}
		bool Is_Empty(void) const {return(!FirstNode.Next()->Is_Valid());}
		void Add_Head(GenericNode * node) {FirstNode.Link(node);}
		void Add_Tail(GenericNode * node) {LastNode.Prev()->Link(node);}
		void Delete(void) {while (FirstNode.Next()->Is_Valid()) delete FirstNode.Next();}

	protected:
		GenericNode FirstNode;
		GenericNode LastNode;

	private:
		GenericList(GenericList & list);
		GenericList & operator = (GenericList const &);
};



/*
**	This node class serves only as an "interface class" for the normal node
**	object. In order to use this interface class you absolutely must be sure
**	that the node is the root base object of the "class T". If it is true that the
**	address of the node is the same as the address of the "class T", then this
**	interface class will work. You can usually ensure this by deriving the
**	class T object from this node.
*/
template<class T> class List;
template<class T>
class Node : public GenericNode {
	public:
		List<T> * Main_List(void) const {return((List<T> *)GenericNode::Main_List());}
		T * Next(void) const {return((T *)GenericNode::Next());}
		T * Prev(void) const {return((T *)GenericNode::Prev());}
		bool Is_Valid(void) const {return(GenericNode::Is_Valid());}
};


/*
**	This is an "interface class" for a list of nodes. The rules for the class T object
**	are the same as the requirements required of the node class.
*/
template<class T>
class List : public GenericList {
	public:
		T * First(void) const {return((T*)GenericList::First());}
		T * Last(void) const {return((T*)GenericList::Last());}
};


#endif
