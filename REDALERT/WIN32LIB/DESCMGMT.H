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
 **     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : WWLIB32 library                          *
 *                                                                         *
 *                    File Name : DESCMGMT.H                               *
 *                                                                         *
 *                   Programmer : Scott K. Bowen                           *
 *                                                                         *
 *                   Start Date : August 3, 1994                           *
 *                                                                         *
 *                  Last Update : August 3, 1994   [SKB]                   *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef DESCMGMT_H
#define DESCMGMT_H


#ifndef WWSTD_H
#include "wwstd.h"
#endif	   

//=====================================================================
// C type include files
#ifdef __cplusplus
extern "C" {
#endif

#include <dos.h>
#include <bios.h>

#ifdef __cplusplus
}
#endif

// ====================================================================


// types
// These where taken from dos.h
//==========================================

// external functions
// ===================================================
extern ULONG Map_Segment_To_Address(ULONG address, ULONG length);

extern "C" {
	// Assemble functions
	extern UWORD FixSelector(UWORD sel);
	extern UWORD GetDs(void);
	extern UWORD GetCs(void);
	extern VOID  GetDefaultSelectors(VOID);
	extern UWORD Get_Standard_Selector(void);

	
	// Assembly data variables
	extern UWORD CodeSelector;
	extern UWORD DataSelector;
	extern UWORD ScreenSelector;
	extern UWORD GraphicsSelector;
	extern UWORD PspSelector;
	extern UWORD EnvSelector;
	extern UWORD DosMemSelector;
	extern UWORD Fp1167Selector;
	extern UWORD FpWeitekSelector;
	extern UWORD FpCyrixSelector;
}

#endif  // DESCMGMT_H


