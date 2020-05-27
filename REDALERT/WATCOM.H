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

/* $Header: /CounterStrike/WATCOM.H 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : WATCOM.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 03/12/95                                                     *
 *                                                                                             *
 *                  Last Update : March 12, 1995 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef WATCOM_H
#define WATCOM_H
#if (0)

// Turn all warnings into errors.
#pragma warning * 0

// Disables warning when "sizeof" is used on an object with virtual functions.
#pragma warning 549 9

// Disable the "Integral value may be truncated during assignment or initialization".
#pragma warning 389 9

// Allow constructing a temporary to be used as a parameter.
#pragma warning 604 9

// Disable the construct resolved as an expression warning.
#pragma warning 595 9

// Disable the strange "construct resolved as a declaration/type" warning.
#pragma warning 594 9

// Disable the "pre-compiled header file cannot be used" warning.
#pragma warning 698 9

// Disable the "temporary object used to initialize a non-constant reference" warning.
#pragma warning 665 9

// Disable the "pointer or reference truncated by cast. Cast is supposed to REMOVE warnings, not create them.
#pragma warning 579 9

// Disable the warning that suggests a null destructor be placed in class definition.
#pragma warning 656 9

// Disable the warning about moving empty constructors/destructors to the class declaration.
#pragma warning 657 9

// No virtual destructor is not an error in C&C.
#pragma warning 004 9

// Turns off unreferenced function parameter warning.
//#pragma off(unreferenced)


// Fix deficiency in Watcom so that true/false will be defined.
#ifndef __BORLANDC__
#ifndef TRUE_FALSE_DEFINED
#define TRUE_FALSE_DEFINED
enum {false=0,true=1};
typedef int bool;
#endif
#endif

#endif

#endif
