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
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : UTRACKER.CPP                             *
 *                                                                         *
 *                   Programmer : Steve Tall                               *
 *                                                                         *
 *                   Start Date : June 3rd, 1996                           *
 *                                                                         *
 *                  Last Update : June 7th, 1996 [ST]                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *  The UnitTracker class exists to track the various statistics           *
 *   required for internet games.                                          *
 *                                                                         *
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 *                                                                         *
 *  Functions:                                                             *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



#include "function.h"

/*
** Define host to network to host functions for DOS
*/
#ifndef WIN32

#define htonl(val) 0
#define ntohl(val) 0

#endif	//WIN32


/***********************************************************************************************
 * UTC::UnitTrackerClass -- Class constructor                                                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Number of unit types to reserve space for                                         *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:10AM ST : Created                                                               *
 *=============================================================================================*/
UnitTrackerClass::UnitTrackerClass (int unit_count)
{
	UnitTotals = new long [unit_count];		// Allocate memory for the unit totals
	UnitCount = unit_count;						// Keep a record of how many unit entries there are
	InNetworkFormat = 0;							// The unit entries are in host format
	Clear_Unit_Total();							// Clear each entry
}


/***********************************************************************************************
 * UTC::~UnitTrackerClass -- Class destructor                                                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:10AM ST : Created                                                               *
 *=============================================================================================*/
UnitTrackerClass::~UnitTrackerClass (void)
{
	delete UnitTotals;
}



/***********************************************************************************************
 * UTC::Increment_Unit_Total -- Increment the total for the specefied unit                     *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Unit number                                                                       *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:12AM ST : Created                                                               *
 *=============================================================================================*/
void UnitTrackerClass::Increment_Unit_Total(int unit_type)
{
	UnitTotals[unit_type]++;
}


/***********************************************************************************************
 * UTC::Decrement_Unit_Total -- Decrement the total for the specefied unit                     *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Unit number                                                                       *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:13AM ST : Created                                                               *
 *=============================================================================================*/
void UnitTrackerClass::Decrement_Unit_Total(int unit_type)
{
	UnitTotals[unit_type]--;
}


/***********************************************************************************************
 * UTC::Get_All_Totals -- Returns a pointer to the start of the unit totals list               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Ptr to unit totals list                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:13AM ST : Created                                                               *
 *=============================================================================================*/
long *UnitTrackerClass::Get_All_Totals (void)
{
	return (UnitTotals);
}


/***********************************************************************************************
 * UTC::Clear_Unit_Total -- Clear out all the unit totals                                      *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:14AM ST : Created                                                               *
 *=============================================================================================*/
void UnitTrackerClass::Clear_Unit_Total (void)
{
	memset (UnitTotals, 0, UnitCount * sizeof(long) );
}



/***********************************************************************************************
 * UTC::To_Network_Format -- Changes all unit totals to network format for the internet        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:15AM ST : Created                                                               *
 *=============================================================================================*/
void UnitTrackerClass::To_Network_Format (void)
{
	if (!InNetworkFormat){
		for (int i=0 ; i<UnitCount ; i++){
			UnitTotals[i] = htonl (UnitTotals[i]);
		}
	}
	InNetworkFormat = 1;		// Flag that data is now in network format
}


/***********************************************************************************************
 * UTC::To_PC_Format -- Changes all unit totals to PC format from network format               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:15AM ST : Created                                                               *
 *=============================================================================================*/
void UnitTrackerClass::To_PC_Format (void)
{
	if (InNetworkFormat){
		for (int i=0 ; i<UnitCount ; i++){
			UnitTotals[i] = ntohl (UnitTotals[i]);
		}
	}
	InNetworkFormat = 0;		// Flag that data is now in PC format
}


// MBL 01.17.2020: Defined in .h, but not implemented
int UnitTrackerClass::Get_Unit_Total (int unit_type)
{
	if (UnitTotals == NULL) 
	{
		return 0;
	}
	if (unit_type >= 0 && unit_type < UnitCount)
	{
		return UnitTotals[unit_type];
	}
	return 0;
}

