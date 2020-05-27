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

/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Westwood Library                         *
 *                                                                         *
 *                    File Name : MEM.C                                    *
 *                                                                         *
 *                   Programmer : Joe L. Bostic                            *
 *                                Scott K. Bowen                           *
 *                                                                         *
 *                   Start Date : March 31, 1993                           *
 *                                                                         *
 *                  Last Update : September 8, 1994   [IML]                *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Mem_Free -- Free a block of memory from system.                       *
 *   Mem_Alloc -- Allocate a block of memory from the special memory pool. *
 *   Mem_Init -- Initialize the private memory allocation pool.            *
 *   Mem_Reference -- Updates the reference time for the specified memory blo*
 *   Mem_Find -- Returns with pointer to specified memory block.           *
 *   Mem_Find_Oldest -- Returns with the memory block with the oldest time st*
 *   Mem_Free_Oldest -- Find and free the oldest memory block.             *
 *   Mem_Avail -- Returns the amount of free memory available in the cache.*
 *   Mem_Cleanup -- Performes a garbage collection on the memory cache.    *
 *   MemNode_Unlink -- Unlinks a node from the cache.                      *
 *   MemNode_Insert -- Inserts a node into a cache chain.                  *
 *   Mem_Largest_Avail -- Largest free block available.                    *
 *   Mem_Lock_Block -- Locks a block so that it cannot be moved in cleanup.*
 *   Mem_In_Use -- Makes it so a block will never be returned as oldest*
 *   Mem_Pool_Size -- Returns total amount of memory in pool.              *
 *   Mem_Get_ID -- Returns ID of node.                                     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <wwstd.h>
#include "wwmem.h"
#include <timer.h>

#include	<stddef.h>
//#include	<mem.h>

#define DEBUG_FILL FALSE

////////////////////////////////////////////////////////////////////////////



/*******************************************************************************
** A allocated block may have one of three meanings in the Time field.  The first
** is the time stamp of the last time it was referenced.  The other two values
** are defined below.  MEM_BLOCK_IN_USE means that it will never be returned as the
** oldest since there is no valid time stamp.  LOCKED_BLOCK has the same meaning as
** MEM_BLOCK_IN_USE with the added feature that the block will not be moved in a
** Mem_Cleanup().  Therefore, there may be some fragmentation after the cleanup
** if any blocks are LOCKED.  It would be good practice to seldomly lock blocks,
** for instance, only when a sample is being played.
** WARNING: If these values change to anything else, logic will need to be changed
**          in Mem_Find_Oldest since it relies on these being small values.
*/
#define MEM_BLOCK_IN_USE	0x00
#define MEM_BLOCK_LOCKED	0x01

/*
**	Each block of memory in the pool is headed by this structure.
*/
typedef struct MemChain {
	struct MemChain	*Next;	// Pointer to next memory chain node.
	struct MemChain	*Prev;	// Pointer to previous memory chain node.
	unsigned long		ID;		// ID number of block.
	unsigned short		Time;		// TickCount of latest reference.
	unsigned long		Size;		// Size of memory block (in paragraphs).
} MemChain_Type;


/*
**	Holding tank memory management data.
*/
typedef struct MemPool {
	MemChain_Type	*FreeChain;	// Pointer to first node in free chain.
	MemChain_Type	*UsedChain;	// Pointer to first node in used chain.
	unsigned long	FreeMem;		// Current amount of free ram (in paragraphs).
	unsigned long	TotalMem;	// Total quantity of memory.
	long				pad2;
} MemPool_Type;


/*=========================================================================*/
/* The following PRIVATE functions are in this file:                       */
/*=========================================================================*/

PRIVATE void MemNode_Unlink(MemPool_Type *pool, int freechain, MemChain_Type *node);
PRIVATE void MemNode_Insert(MemPool_Type *pool, int freechain, MemChain_Type *node, unsigned int size, unsigned long id, int merge);

/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/


/***************************************************************************
 * Mem_Init -- Initialize the private memory allocation pool.              *
 *                                                                         *
 *    This routine is used to initialize the private memory allocation     *
 *    pool.                                                                *
 *                                                                         *
 * INPUT:   buffer   -- Pointer to the buffer that is the allocation pool. *
 *                                                                         *
 *          size     -- Size of the buffer in bytes.                       *
 *                                                                         *
 * OUTPUT:  TRUE/FALSE; Was it initialized successfully?                   *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   03/31/1993 JLB : Created.                                             *
 *   04/13/1994 SKB : Update for 32 bit library, removed XMS calls, 			*
 *							 optimized for low memory only.								*
 *=========================================================================*/
int Mem_Init(void *buffer, long size)
{
	MemChain_Type	*mem;			// Working memory chain node.
	MemPool_Type	*pool;		// Memory pool control structure.

	/*
	**	The buffer is rounded down to the nearest paragraph.
	*/
	size = size & 0xFFFFFFF0L;

	if (!buffer || !size) return(FALSE);

	/*
	**	Initialize the pool control structure.
	*/
	pool = (MemPool_Type *)buffer;
	pool->FreeMem = (size - sizeof(MemPool_Type)) >> 4;
	pool->UsedChain = NULL;
	pool->TotalMem = pool->FreeMem;
	mem = pool->FreeChain = (MemChain_Type *) (pool + 1);

	/*
	**	Initialize the free memory chain.
	*/
	mem->Next = NULL;
	mem->Prev = NULL;
	mem->Size = pool->FreeMem;
	mem->ID = -1;
	mem->Time = 0;

	return(TRUE);
}


/***************************************************************************
 * Mem_Alloc -- Allocate a block of memory from the special memory pool.   *
 *                                                                         *
 *    This routine will allocate a block of memory from the special        *
 *    memory allocation pool.                                              *
 *                                                                         *
 * INPUT:   poolptr	-- Pointer to the memory pool base address.           *
 *                                                                         *
 *          size  -- The size of the memory block to allocate.             *
 *                                                                         *
 *          id    -- ID number to give this memory block.                  *
 *                                                                         *
 * OUTPUT:  Returns with a pointer to the allocated block.  If there was   *
 *          insufficient room, then NULL is returned.                      *
 *                                                                         *
 * WARNINGS:   Be sure to check for the NULL return case.                  *
 *                                                                         *
 * HISTORY:                                                                *
 *   03/31/1993 JLB : Created.                                             *
 *   08/06/1993 JLB : Optimized for low memory caches.                     *
 *   04/13/1994 SKB : Update for 32 bit library, removed XMS calls, 			*
 *							 optimized for low memory only.								*
 *=========================================================================*/
void *Mem_Alloc(void *poolptr, long lsize, unsigned long id)
{
	MemPool_Type	*pool;
	MemChain_Type	*node;			// Pointer to current memory node.
	unsigned int	remainder=0;	// Remaining bytes that are still free.
	int				found;
	unsigned int	size;				// Paragraph size of allocation.


	/*
	**	If there is no free memory then the allocation will
	**	always fail.
	*/
	if (!poolptr || !lsize) return(NULL);
	pool = (MemPool_Type *) poolptr;

	/*
	**	Allocations are forced to be paragraph sized.
	*/
	lsize += sizeof(MemChain_Type);	// Account for header.
	lsize = (lsize + 0x0FL) & 0xFFFFFFF0L;
	size = lsize >> 4;

	/*
	**	If the total free is less than the size of the desired allocation,
	**	then we KNOW that an allocation will fail -- just return.
	*/
	if (pool->TotalMem < size) {
		return(NULL);
	}

	/*
	**	Walk down free chain looking for the first block that will
	**	accomodate the allocation.
	*/
	node = pool->FreeChain;
	found = FALSE;
	while (!found && node) {

		/*
		**	Fetch free memory chunk block and see if it is big enough.
		*/
		if (node->Size >= size) {
			found = TRUE;
			break;
		}
		node = node->Next;
	}
	if (!found) {
		return(NULL);
	}

	/*
	**	Determine if this allocation would split the block.
	*/
	remainder = node->Size - size;

	/*
	**	If only a very small free chunk would remain, just tack it on
	**	to the current allocation.
	*/
	if (remainder <= 2) {
		remainder = 0;
		size = node->Size;
	}

	/*
	**	Remove the primary block from the free memory list.
	*/
	MemNode_Unlink(pool, TRUE, node);

	/*
	**	If a smaller block remains, then link it back into
	**	the free memory list.
	*/
	if (remainder) {
		MemNode_Insert(pool, TRUE, (MemChain_Type *)Add_Long_To_Pointer(node, (long)size << 4), remainder, -1, FALSE);
	}

	/*
	**	Link in the allocated node into the used memory list.
	*/
	MemNode_Insert(pool, FALSE, node, size, id, FALSE);

	/*
	**	Reflect the change to the total free count.
	*/
	pool->FreeMem -= size;

	/*
	**	Return a pointer to the block of allocated memory just past
	**	the header.
	*/

#if DEBUG_FILL
	memset(node + 1, id, (size-1) << 4);
#endif
	return((void *) (node + 1));
}


/***************************************************************************
 * Mem_Free -- Free a block of memory from system.                         *
 *                                                                         *
 *    This routine will free a block of memory from the special memory     *
 *    buffer.                                                              *
 *                                                                         *
 * INPUT:   poolptr	-- Pointer to the memory pool base address.           *
 *                                                                         *
 *          buffer   -- Pointer to memory block to free.                   *
 *                                                                         *
 * OUTPUT:  TRUE/FALSE; Was the deallocation successful?                   *
 *                                                                         *
 * WARNINGS:   Be sure to only pass in to this routine a buffer that was   *
 *             returned from Mem_Alloc().                                  *
 *                                                                         *
 * HISTORY:                                                                *
 *   03/31/1993 JLB : Created.                                             *
 *   08/06/1993 JLB : Optimized for low memory caches.                     *
 *   04/13/1994 SKB : Update for 32 bit library, removed XMS calls, 			*
 *							 optimized for low memory only.								*
 *=========================================================================*/
int Mem_Free(void *poolptr, void *buffer)
{
	MemPool_Type	*pool;			// pointer to structure.
	MemChain_Type	*node;			// Copy of current memory node.
	unsigned int	size;				// Size of the block being freed.

	/*
	**	One can't free what isn't there.
	*/
	if (!buffer || !poolptr) {
		return(FALSE);
	}
	pool = (MemPool_Type *) poolptr;

	/*
	**	The node pointer is actually back a bit from the "normal" pointer.
	*/
	node = (MemChain_Type *) buffer;
	node--;

	/*
	**	Get pointer to actual allocated node and unlink it from the used
	**	memory chain.
	*/
	size = node->Size;
	MemNode_Unlink(pool, FALSE, node);
	MemNode_Insert(pool, TRUE, node, size, -1, TRUE);

	/*
	**	Reflect the new free memory into the total memory count.
	*/
	pool->FreeMem += size;

	return(TRUE);
}


/***************************************************************************
 * Mem_Reference -- Updates the reference time for the specified memory blo*
 *                                                                         *
 *    This routine is used to update the memory reference time for the     *
 *    specified memory node.  Typically, this is called every time a       *
 *    memory block is used in order to make sure the memory block time     *
 *    tracking (Last Recently Used) system works properly.                 *
 *                                                                         *
 * INPUT:   node  -- Pointer to memory block returned from Mem_Find.       *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   The node pointer must be valid.  For maximum safety this    *
 *             routine should be called right after Mem_Find().            *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/06/1993 JLB : Created.                                             *
 *   04/13/1994 SKB : Update for 32 bit library, removed XMS calls, 			*
 *							 optimized for low memory only.								*
 *=========================================================================*/
void Mem_Reference(void *node)
{
	MemChain_Type	*nodeptr;		// Pointer of current memory node.

	if (!node) return;

	// Get to the node header.
	nodeptr = (MemChain_Type *) node;
	nodeptr--;

	nodeptr->Time = (unsigned short)(TickCount.Time() >> 4);

}


/***************************************************************************
 * MEM_LOCK_BLOCK -- Locks a block so that it cannot be moved in cleanup.  *
 * 	By marking a memory block in use, the memory system will never return*
 *    it as the oldest memory block.  It also makes it so that the block	*
 *    will never be moved during a Cleanup process.                        *
 *                                                                         *
 * INPUT:   node  -- Pointer to memory block returned from Mem_Find.       *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:  	If one or more blocks are locked in a heap, Mem_Avail might *
 *             not equal Mem_Largest_Avail after a call to Mem_Cleanup.    *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/15/1994 SKB : Created.                                             *
 *=========================================================================*/
void Mem_Lock_Block(void *node)
{
	MemChain_Type	*nodeptr;		// Pointer of current memory node.

	if (!node) return;

	// Get to the node header.
	nodeptr = (MemChain_Type *) node;
	nodeptr--;
	nodeptr->Time = MEM_BLOCK_LOCKED;
}


/***************************************************************************
 * MEM_IN_USE -- Makes it so a block will never be returned as oldest		*
 * 	By marking a memory block in use, the memory system will never return*
 *    it as the oldest memory block.  It still can be moved in the Cleanup *
 *    code.                                                                *
 *                                                                         *
 * INPUT:   node  -- Pointer to memory block returned from Mem_Find.       *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS: Mem_Find_Oldest() will return NULL if only IN_USE blocks are  *
 *           in memory.                                                    *
 * HISTORY:                                                                *
 *   04/15/1994 SKB : Created.                                             *
 *=========================================================================*/
void Mem_In_Use(void *node)
{
	MemChain_Type	*nodeptr;		// Pointer of current memory node.

	if (!node) return;

	// Get to the node header.
	nodeptr = (MemChain_Type *) node - 1;
	nodeptr->Time = MEM_BLOCK_IN_USE;
}


/***************************************************************************
 * Mem_Find -- Returns with pointer to specified memory block.             *
 *                                                                         *
 *    Use this routine to convert a memory ID value into an actual memory  *
 *    pointer.  It sweeps through all of the 'cached' memory blocks and    *
 *    returns with the matching block pointer.                             *
 *                                                                         *
 * INPUT:   poolptr  -- Pointer to the memory cache block.                 *
 *                                                                         *
 *          id       -- The ID of the block desired.                       *
 *                                                                         *
 * OUTPUT:  Returns with the pointer to the memory block.  If NULL is      *
 *          returned then the desired block is not in the memory cache.    *
 *                                                                         *
 * WARNINGS:   This routine may return NULL if the memory block is not     *
 *             present in the cache.                                       *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/06/1993 JLB : Created.                                             *
 *   08/06/1993 JLB : Optimized for low memory caches.                     *
 *   04/13/1994 SKB : Update for 32 bit library, removed XMS calls, 			*
 *							 optimized for low memory only.								*
 *=========================================================================*/
void *Mem_Find(void *poolptr, unsigned long id)
{
	MemPool_Type	*pool;			// pointer to structure.
	MemChain_Type	*node;			// Working node structure.

	if (!poolptr) return(NULL);

	pool = (MemPool_Type *) poolptr;

	/*
	** Cannot free a node that is not on the UsedChain list.
	*/
	if (!pool->UsedChain) {
		return(NULL);
	}

	/*
	**	Sweep through entire allocation chain to find
	**	the one with the matching ID.
	*/
	node = pool->UsedChain;
	while (node) {

		if (node->ID == id) {
			return(node + 1);
		}
		node = node->Next;
	}
	return(NULL);
}


/***************************************************************************
 * MEM_GET_ID -- Returns ID of node.                                       *
 *                                                                         *
 * INPUT:  void *node - pointer to node. 												*
 *                                                                         *
 * OUTPUT: The ID of the node that was supplied by user during Mem_Alloc().*
 *                                                                         *
 * WARNINGS: pointer to node must be one that Mem_Alloc or                	*
 *           Mem_Find returned.                                           **
 *                                                                         *
 * HISTORY:                                                                *
 *   04/18/1994 SKB : Created.                                             *
 *=========================================================================*/
unsigned long Mem_Get_ID(void *node)
{
	MemChain_Type	*nodeptr;		// Pointer of current memory node.

	if (!node) return (0L);

	// Get to the node header.
	nodeptr = (MemChain_Type *) node - 1;
	return (nodeptr->ID);
}


/***************************************************************************
 * Mem_Find_Oldest -- Returns with the memory block with the oldest time st*
 *                                                                         *
 *    Use this routine to find the memory block with the oldest time stamp *
 *    value.  Typically, this is used when freeing memory blocks in the    *
 *    cache in order to make room for a new memory block.                  *
 *                                                                         *
 * INPUT:   poolptr  -- Pointer to the memory cache.                       *
 *                                                                         *
 * OUTPUT:  Returns with the pointer to the oldest memory block.  If NULL  *
 *          is returned, then the memory cache is empty.                   *
 *                                                                         *
 * WARNINGS:   This routine could return NULL.                             *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/06/1993 JLB : Created.                                             *
 *   08/06/1993 JLB : Optimized for low memory caches.                     *
 *   04/13/1994 SKB : Update for 32 bit library, removed XMS calls, 			*
 *							 optimized for low memory only.								*
 *   04/15/1994 SKB : Handle time wrap, locked blocks, and no_refenece blocks*
 *=========================================================================*/
void *Mem_Find_Oldest(void *poolptr)
{
	MemChain_Type	*node; 				// Working node pointer.
	MemChain_Type	*oldnode;			// Pointer to oldest block.
	unsigned int	oldtime;				// Time of oldest block.
	unsigned int	basetime;			// Time to mark our base time with.
	unsigned int	time;					// basetime + time of node.

	if (!poolptr) return(NULL);

	/*
	**	Sweep through entire allocation chain to find
	**	the oldest referenced memory block.
	*/
	oldnode = NULL;
	oldtime = 0;
	node = ((MemPool_Type*) poolptr)->UsedChain;

  basetime = (unsigned int)(TickCount.Time() >> 4);

	while (node) {

		/*
		** Don't allow MEM_BLOCK_IN_USE or MEM_BLOCK_LOCKED to be returned.
		*/
		if (node->Time > MEM_BLOCK_LOCKED) {

			/*
			** Adjust time for wrap around (after about 5 hrs).
			** times less then the base time will wrap up high while
			** and times greater then base time will then be lower since
			** any time greater has been on the thing a long time.
			*/
			time = node->Time - basetime ;

			if (time < oldtime || !oldnode) {
				oldtime = time;
				oldnode = node;
			}
		}
		node = node->Next;
	}

	/*
	**	Return with the value that matches the pointer that
	**	was allocated by the system previously.
	*/
	if (oldnode) {
		oldnode++;
	}
	return(oldnode);
}


/***************************************************************************
 * Mem_Free_Oldest -- Find and free the oldest memory block.               *
 *                                                                         *
 *    This routine is used to free the oldest memory block in the memory   *
 *    cache.  This routine is typcially used in order to create more room  *
 *    in the cache for a new allocation.                                   *
 *                                                                         *
 * INPUT:   poolptr  -- Pointer to the memory cache.                       *
 *                                                                         *
 * OUTPUT:  Returns with the node that it freed.  Although this node is    *
 *          is no longer valid, it may be used to mark that pointer as     *
 *          invalid in the main code.                                      *
 *                                                                         *
 * WARNINGS:   If this routine returns NULL, then no memory was freed.     *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/06/1993 JLB : Created.                                             *
 *   04/13/1994 SKB : Update for 32 bit library, removed XMS calls, 			*
 *							 optimized for low memory only.								*
 *=========================================================================*/
void *Mem_Free_Oldest(void *poolptr)
{
	MemChain_Type	*node;		// Copy of pointer to oldest node.

	if (!poolptr) return(NULL);
	node = (MemChain *) Mem_Find_Oldest(poolptr);
	if (Mem_Free(poolptr, node)) {
		return(node);
	}
	return(NULL);
}

/***************************************************************************
 * MEM_POOL_SIZE -- Returns total amount of memory in pool.                *
 *                                                                         *
 * INPUT:   poolptr  -- Pointer to the memory cache.                       *
 *                                                                         *
 * OUTPUT:  long total size of pool. i.e. largest possible allocation if   *
 *          no memory was allocated.                                       *
 *                                                                         *
 * WARNINGS: 																					*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/18/1994 SKB : Created.                                             *
 *=========================================================================*/
long Mem_Pool_Size(void *poolptr)
{
	MemPool_Type	*pool;			// Memory pool control structure.
	long				memtotal;		// Total amount of memory free.

	if (!poolptr) return(NULL);
	pool = (MemPool_Type *) poolptr;

	memtotal = ((long)pool->TotalMem) << 4;
	memtotal -= sizeof(MemChain_Type);
	memtotal = MAX(memtotal, (long)0);
	return(memtotal);
}

/***************************************************************************
 * Mem_Avail -- Returns the amount of free memory available in the cache.  *
 *                                                                         *
 *    This routine examines the memory cache and returns the amount of     *
 *    free memory available.  This memory total MAY be fragmented but      *
 *    after Mem_Cleanup() is called, an allocation of the amount returned  *
 *    by this function is guaranteed.                                      *
 *                                                                         *
 * INPUT:   poolptr  -- Pointer to the memory cache.                       *
 *                                                                         *
 * OUTPUT:  Returns the largest allocation possible from the memory cache. *
 *                                                                         *
 * WARNINGS:   The value returned may represent the FRAGMENTED total       *
 *             amount of memory free in the cache.                         *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/06/1993 JLB : Created.                                             *
 *   04/13/1994 SKB : Update for 32 bit library, removed XMS calls, 			*
 *							 optimized for low memory only.								*
 *=========================================================================*/
long Mem_Avail(void *poolptr)
{
	MemPool_Type	*pool;		// Memory pool control structure.
	long				memtotal;	// Total amount of memory free.

	if (!poolptr) return(NULL);
	pool = (MemPool_Type *) poolptr;

	memtotal = ((long)pool->FreeMem) << 4;
	memtotal -= sizeof(MemChain_Type);
	//memtotal -= sizeof(MemChain_Type) + 15;
	memtotal = MAX(memtotal, (long)0);
	return(memtotal);
}


/***************************************************************************
 * MEM_LARGEST_AVAIL -- Largest free block available.                      *
 * 	This routine examines the free node list to find the largest block   *
 *		available.  User can Mem_Alloc() this return size successfully.      *
 *                                                                         *
 * INPUT:   poolptr  -- Pointer to the memory cache.                       *
 *                                                                         *
 * OUTPUT:  Returns largest allocation currently possible from the cache. 	*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/15/1994 SKB : Created.                                             *
 *=========================================================================*/
long Mem_Largest_Avail(void *poolptr)
{
	MemChain_Type	*node;			// Pointer to current memory node.
	unsigned int	size;
	long				truesize;

	/*
	** Make sure that it is a buffer.
	*/
	if (!poolptr) return(NULL);

	/*
	** Go through the entire free chain looking for the largest block.
	*/
	node = ((MemPool_Type *)poolptr)->FreeChain;
	size = 0;
	while (node) {

		/*
		**	Fetch free memory chunk block and see if it is big enough.
		*/
		if (node->Size >= size) {
			size = node->Size;
		}
		node = node->Next;
	}

	truesize = (long)size << 4;
	truesize -= sizeof(MemChain_Type);
	truesize = MAX(truesize, 0L);
	return (truesize);
}


/***************************************************************************
 * Mem_Cleanup -- Performs a garbage collection on the memory cache.       *
 *                                                                         *
 *    This routine is used to coalesce all adjacent free blocks of         *
 *    memory in the specified cache.  As a result, all previous pointers   *
 *    provided by Mem_Find() are invalidated.  This routine consumes a     *
 *    fair amount of time and should be called as infrequently as          *
 *    possible.                                                            *
 *                                                                         *
 * INPUT:   poolptr  -- Pointer to the memory cache.                       *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   This routine takes a significant amount of time!            *
 *             If there are locked block in memory, the pool may still     *
 *             be fragmented.                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/06/1993 JLB : Created.                                             *
 *   08/06/1993 JLB : Updated for low memory caches.                       *
 *   04/13/1994 SKB : Update for 32 bit library, removed XMS calls, 			*
 *							 optimized for low memory only.								*
 *=========================================================================*/
void Mem_Cleanup(void *poolptr)
{
	MemPool_Type	*pool;  	// Memory pool control structure.
	MemChain_Type	*free,	// Pointer to first free area.
						*cur;		// Pointer to first used block that is after free.
	unsigned long	size;
	unsigned long	freesize;// Size of free heap at the end of the block.

	if (!poolptr) return;

	/*
	**	Fetch working copy of pool control structure.
	*/
	pool = (MemPool_Type *) poolptr;

	/*
	**	Basic parameter and condition legality checks.  If the memory pool
	**	has no free space, no free blocks, or no allocated blocks, then
	**	memory cleanup is unnecessary -- just exit.
	*/
	if (!pool->FreeMem || !pool->FreeChain || !pool->UsedChain) return;

	freesize = pool->FreeMem;
	free = pool->FreeChain;
	pool->FreeChain = NULL;
	cur = pool->UsedChain;
	while (TRUE) {

		/*
		** Setup pointers so that free points to the first free block and cur
		** points to the next used block after the free block.
		*/
		while (cur < free && cur) {
	 		cur = cur->Next;
		}

		// All used blocks are at the front of the free.  We are done.
		if (!cur) {
	 		break;
		}

		/*
		** Do not allow a locked block to be moved.
		*/
		if (cur->Time == MEM_BLOCK_LOCKED) {
			/*
			** Figure the size of the new free block that we are creating.
			** Subtract off the total block size.
			** Add the node to the free list.
			*/
			size = (char *) cur - (char  *) free;
			size >>= 4;
		 	freesize -= size;
			MemNode_Insert(pool, TRUE, free, (unsigned int) size, -1, FALSE);

			/*
			** Time to find a new free position to start working from.
			** Cur will be in the position just following.
			*/
			free = (MemChain_Type *) Add_Long_To_Pointer(cur, (unsigned long)cur->Size << 4);
			cur = cur->Next;
			while (free == cur) {
				free = (MemChain_Type *) Add_Long_To_Pointer(cur, (unsigned long)cur->Size << 4);
				cur = cur->Next;
			}

			// All used blocks are at the front of the free.  We are done.
			if (!cur) {
			 	break;
			}
		} else {

			// Copy the block up.
			size = (unsigned long)cur->Size << 4;
			Mem_Copy(cur, free, size);
			cur = free;

			// Change pointers of surrounding blocks.
			if (cur->Next) {
		 		cur->Next->Prev = cur;
			}
			if (cur->Prev) {
		 		cur->Prev->Next = cur;
			} else {
		 		pool->UsedChain = cur;
			}

			// Change to next new free area.
			free = (MemChain_Type *) Add_Long_To_Pointer(cur, size);
		}
	}

	/*
	**	Now build the single free chunk.
	*/
	MemNode_Insert(pool, TRUE, free, freesize, -1, FALSE);
}


/***************************************************************************
 * MemNode_Unlink -- Unlinks a node from the cache.                        *
 *                                                                         *
 *    A private routine the actually unlinks a memory block from the       *
 *    memory cache.  It doesn't perform a complete update of the memory    *
 *    cache.                                                               *
 *                                                                         *
 * INPUT:   pool     -- Pointer to the memory cache header (copy in real   *
 *                      memory).                                           *
 *                                                                         *
 *          freechain-- Is the block part of the free memory chain?        *
 *                                                                         *
 *          node     -- Pointer to the node that will be unlinked.         *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   This routine doesn't update memory totals.  It is a support *
 *             function.                                                   *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/06/1993 JLB : Created.                                             *
 *   04/13/1994 SKB : Update for 32 bit library, removed XMS calls, 			*
 *							 optimized for low memory only.								*
 *=========================================================================*/
PRIVATE void MemNode_Unlink(MemPool_Type *pool, int freechain, MemChain_Type *node)
{
	MemChain_Type	*other; 		// Copy of node data to unlink.
	MemChain_Type	**chain;		// A pointer to one of the chains pointer.

	/*
	**	Check for parameter validity.
	*/
	if (!pool || !node) return;

	/*
	**	Setup working pointer for the particular chain desired.
	*/
	if (freechain) {
		chain = &pool->FreeChain;
	} else {
		chain = &pool->UsedChain;
	}

	/*
	**	Make adjustments to the previous node.  If the pointer
	**	to the previous node is NULL then this indicates the
	**	first node in the list and thus the chain pointer needs
	**	to be updated instead.
	*/
	if (node->Prev) {
		other = node->Prev;
		other->Next = node->Next;
	} else {
		*chain = node->Next;
	}

	if (node->Next) {
		other = node->Next;
		other->Prev = node->Prev;
	}
}


/***************************************************************************
 * MemNode_Insert -- Inserts a node into a cache chain.                    *
 *                                                                         *
 *    This routine is used to add a node to a cache chain.  Since nodes    *
 *    do not contain double links, they must be placed in sequence.        *
 *                                                                         *
 * INPUT:   pool     -- Pointer to memory pool (must be in real memory).   *
 *                                                                         *
 *          freechain-- Is the node to be inserted into the free chain?    *
 *                                                                         *
 *          node     -- Pointer to the node to insert.                     *
 *                                                                         *
 *          size     -- Size of the memory block (in paragraphs).          *
 *                                                                         *
 *          id       -- The ID number to associate with this block.        *
 *                                                                         *
 *          merge    -- Merge inserted block with adjacent blocks.         *
 *                                                                         *
 * OUTPUT:  return                                                         *
 *                                                                         *
 * WARNINGS:   This is a support routine.                                  *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/06/1993 JLB : Created.                                             *
 *=========================================================================*/
PRIVATE void MemNode_Insert(MemPool_Type *pool, int freechain, MemChain_Type *node, unsigned int size, unsigned long id, int merge)
{
	MemChain_Type 	**chain;			// Pointer to chain that will be linked.
	MemChain_Type 	*prev,			// Successor node pointer.
						*next;			// Predecessor node pointer.
	int				doit=TRUE;		// Link the node into the list.


	/*
	**	Determine if the parameters are valid.
	*/
	if (!pool || !node || !size) return;

	/*
	**	Setup working pointer for the particular chain desired.
	*/
	if (freechain) {
		chain = &pool->FreeChain;
	} else {
		chain = &pool->UsedChain;
	}

	/*
	**	Handle the "no node in list" condition (easiest).
	*/
	if (!*chain) {
		node->Next = NULL;
		node->Prev = NULL;
		node->Size = size;
		node->Time = (unsigned short)(TickCount.Time() >> 4);
		node->ID = id;
		*chain = node;
		return;
	}

	/*
	**	Sweep through the memory chain looking for a likely spot
	**	to insert the new node.  It will stop with "next" pointing
	**	to the node to come after the block to be inserted and "prev"
	** will point to the node right before.
	*/
	prev = NULL;
	next = *chain;
	while (next && (next < node)) {

		/*
		**	Move up the memory chain.
		*/
		prev = next;
		next = next->Next;
	}

	/*
	**	Coallescing of adjacent blocks (if requested).
	*/
	if (merge) {

		/*
		**	If the previous block is touching the block to insert
		**	then merely adjust the size of the previous block and
		**	that is all that is necessary.
		*/
		if (prev) {
			if (((char *)prev + ((long)prev->Size << 4)) == ((char *) node)) {
				prev->Size += size;
				size = prev->Size;
				node = prev;
				prev = prev->Prev;
				doit = FALSE;
			}
		}

		/*
		**	If the following block is touching the block to insert
		**	then remove the following block and increase the size of
		**	the original insertion block by the size of the other
		**	block.
		*/
		if (next) {
			if (((char *)node + ((long)size << 4)) == (char *)next) {

				if (!doit) {

					/*
					**	If the node was already merged with the previous block
					**	then merely increase the previous block's size
					**	and adjust it's next pointer appropriately.
					*/
					node->Size += next->Size;
					node->Next = next->Next;
					next = next->Next;
				} else {

					/*
					**	Increase the size of the current block and adjust
					**	the "next" pointer so that it gets fixed up
					**	accordingly.
					*/
					size += next->Size;
					next = next->Next;
				}
			}
		}
	}

#if DEBUG_FILL
	if (doit) {
		memset(node + 1, 0xFF, (size - 1) << 4);
	} else {
		memset(node + 1, 0xFF, (node->Size - 1) << 4);
	}
#endif

	/*
	**	Fixup the node pointers.
	*/
	if (prev) {
		prev->Next = node;
	}else{
		*chain = node;
	}

	if (next) {
	 	next->Prev = node;
	}

	if (doit) {
		node->Prev = prev;
		node->Next = next;
		node->Size = size;
		node->Time = (unsigned short)(TickCount.Time() >> 4);
		node->ID = id;
	}
}






