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

/* $Header: /CounterStrike/BENCH.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BENCH.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/17/96                                                     *
 *                                                                                             *
 *                  Last Update : July 18, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Benchmark::Begin -- Start the benchmark operation.                                        *
 *   Benchmark::Benchmark -- Constructor for the benchmark object.                             *
 *   Benchmark::End -- Mark the end of a benchmarked operation                                 *
 *   Benchmark::Reset -- Clear out the benchmark statistics.                                   *
 *   Benchmark::Value -- Fetch the current average benchmark time.                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if (0)
#include	"bench.h"
#include	"mpu.h"


/***********************************************************************************************
 * Benchmark::Benchmark -- Constructor for the benchmark object.                               *
 *                                                                                             *
 *    This will construct the benchmark object.                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
Benchmark::Benchmark(void) :
	Average(0),
	Counter(0),
	TotalCount(0)
{
}


/***********************************************************************************************
 * Benchmark::Reset -- Clear out the benchmark statistics.                                     *
 *                                                                                             *
 *    Use this routine to clear out all the accumulated statistics within this benchmark       *
 *    object. The object is set just as if it was freshly constructed.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Benchmark::Reset(void)
{
	Average = 0;
	Counter = 0;
	TotalCount = 0;
}


/***********************************************************************************************
 * Benchmark::Begin -- Start the benchmark operation.                                          *
 *                                                                                             *
 *    Call this routine before the operation to be benchmarked is begun. The corresponding     *
 *    End() function must be called after the operation has completed.                         *
 *                                                                                             *
 * INPUT:   reset -- Should the entire benchmark object be reset at this time as well?         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The Begin() and End() functions are NOT nestable.                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Benchmark::Begin(bool reset)
{
	if (reset) Reset();
	Clock = 0;
}


/***********************************************************************************************
 * Benchmark::End -- Mark the end of a benchmarked operation                                   *
 *                                                                                             *
 *    This routine is called at the end of the operation that is being benchmarked. It is      *
 *    important to call this routine as soon as possible after the event being benchmarked     *
 *    has completed.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The Being() and End() functions are NOT nestable.                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Benchmark::End(void)
{
	unsigned long value = Clock;

	if (Counter == MAXIMUM_EVENT_COUNT) {
		Average -= Average / MAXIMUM_EVENT_COUNT;
		Average += value;
	} else {
		Average += value;
		Counter++;
	}
	TotalCount++;
}


/***********************************************************************************************
 * Benchmark::Value -- Fetch the current average benchmark time.                               *
 *                                                                                             *
 *    This routine will take the statistics already accumulated and determine the average      *
 *    time recorded. This value will be returned.                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the average time that all events tracked by this object.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
unsigned long Benchmark::Value(void) const
{
	if (Counter) {
		return(Average / Counter);
	}
	return(0);
}
#endif