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
 **      C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S      **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : WWLIB 32                                 *
 *                                                                         *
 *                    File Name : REGIONSZ.CPP                             *
 *                                                                         *
 *                   Programmer : Phil W. Gorrow                           *
 *                                                                         *
 *                   Start Date : November 3, 1994                         *
 *                                                                         *
 *                  Last Update : November 3, 1994   [PWG]                 *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Size_Of_Region -- Calculates the size of a given region               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*=========================================================================*/
/* The following PRIVATE functions are in this file:                       */
/*=========================================================================*/


/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/


/***************************************************************************
 * SIZE_OF_REGION -- Calculates the size of a given region                 *
 *                                                                         *
 * INPUT:	int width  - the width of the region                           *
 *				int height - the height of the region									*
 *                                                                         *
 * OUTPUT:  long - the size of the region                                  *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/03/1994 PWG : Created.                                             *
 *=========================================================================*/
long Size_Of_Region(int width, int height)                                 
{
	return(width * height);
}
