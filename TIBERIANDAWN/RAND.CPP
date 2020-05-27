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

/* $Header:   F:\projects\c&c\vcs\code\rand.cpv   1.6   16 Oct 1995 16:51:10   JOE_BOSTIC  $ */
/*************************************************************************** 
 *                                                                         * 
 *                 Project Name : Command & Conquer                        * 
 *                                                                         * 
 *                    File Name : RAND.CPP                                 * 
 *                                                                         * 
 *                   Programmer : Bill R. Randolph                         * 
 *                                                                         * 
 *                   Start Date : 04/25/95                                 * 
 *                                                                         * 
 *                  Last Update : June 17, 1995 [JLB]                      * 
 *                                                                         * 
 *-------------------------------------------------------------------------* 
 * Functions:                                                              * 
 *   Sim_Random -- Returns 0 - 255                                         * 
 *   Sim_IRandom -- Returns minval to maxval, inclusive                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

int SimRandIndex = 0;

/*************************************************************************** 
 * Sim_Random -- Returns 0 - 255                                           * 
 *                                                                         * 
 * INPUT:                                                                  * 
 *      none.                                                              *
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *      0 - 255, at random                                                 *
 *                                                                         * 
 * WARNINGS:                                                               * 
 *      none.                                                              *
 *                                                                         * 
 * HISTORY:                                                                * 
 *   04/25/1995 BRR : Created.                                             * 
 *   06/17/1995 JLB : Takes advantage of character math wrap.              * 
 *=========================================================================*/
int Sim_Random(void)
{
	static unsigned char _randvals[] = {
		0x47, 0xce, 0xc6, 0x6e, 0xd7, 0x9f, 0x98, 0x29, 0x92, 0x0c, 0x74, 0xa2, 
		0x65, 0x20, 0x4b, 0x4f, 0x1e, 0xed, 0x3a, 0xdf, 0xa5, 0x7d, 0xb5, 0xc8,
		0x86, 0x01, 0x81, 0xca, 0xf1, 0x17, 0xd6, 0x23, 0xe1, 0xbd, 0x0e, 0xe4, 
		0x62, 0xfa, 0xd9, 0x5c, 0x68, 0xf5, 0x7f, 0xdc, 0xe7, 0xb9, 0xc4, 0xb3,
		0x7a, 0xd8, 0x06, 0x3e, 0xeb, 0x09, 0x1a, 0x31, 0x3f, 0x46, 0x28, 0x12, 
		0xf0, 0x10, 0x84, 0x76, 0x3b, 0xc5, 0x53, 0x18, 0x14, 0x73, 0x7e, 0x59,
		0x48, 0x93, 0xaa, 0x1d, 0x5d, 0x79, 0x24, 0x61, 0x1b, 0xfd, 0x2b, 0xa8, 
		0xc2, 0xdb, 0xe8, 0x2a, 0xb0, 0x25, 0x95, 0xab, 0x96, 0x83, 0xfc, 0x5f,
		0x9c, 0x32, 0x78, 0x9a, 0x9e, 0xe2, 0x8e, 0x35, 0x4c, 0x41, 0xa1, 0x69, 
		0x5a, 0xfe, 0xa7, 0xa4, 0xf6, 0x6d, 0xc1, 0x58, 0x0a, 0xcf, 0xea, 0xc3,
		0xba, 0x85, 0x99, 0x8d, 0x36, 0xb6, 0xdd, 0xd3, 0x04, 0xe6, 0x45, 0x0d,
		0x60, 0xae, 0xa3, 0x22, 0x4d, 0xe9, 0xc9, 0x9b, 0xb7, 0x0f, 0x02, 0x42,
		0xf9, 0x0b, 0x8f, 0x43, 0x44, 0x87, 0x70, 0xbe, 0xe3, 0xf8, 0xee, 0xa9, 
		0xbc, 0xc0, 0x67, 0x33, 0x16, 0x37, 0x57, 0xad, 0x5e, 0x9d, 0x64, 0x40,
		0x54, 0x05, 0x2c, 0xe0, 0xb2, 0x97, 0x08, 0xaf, 0x75, 0x8a, 0x5b, 0xfb,
		0x4e, 0xbf, 0x91, 0xf3, 0xcb, 0x7c, 0x63, 0xef, 0x89, 0x52, 0x6c, 0x2f,
		0x21, 0x4a, 0xf7, 0xcd, 0x2e, 0xf4, 0xc7, 0x6f, 0x19, 0xb1, 0x66, 0xcc,
		0x90, 0x8c, 0x50, 0x51, 0x26, 0x7b, 0xda, 0x49, 0x80, 0x30, 0x55, 0x1f, 
		0xd2, 0xb4, 0xd1, 0xd5, 0x6b, 0xf2, 0x72, 0xbb, 0x13, 0x3d, 0xff, 0x15,
		0x38, 0xe5, 0xd4, 0xde, 0x2d, 0x27, 0x94, 0xa0, 0xd0, 0x39, 0x82, 0x8b,
		0x03, 0xac, 0x3c, 0x34, 0x77, 0xb8, 0xec, 0x00, 0x07, 0x1c, 0x88, 0xa6,
		0x56, 0x11, 0x71, 0x6a,
	};

	((unsigned char&)SimRandIndex)++;
//	SimRandIndex &= 0xff;
	return(_randvals[SimRandIndex]);
}


/*************************************************************************** 
 * Sim_IRandom -- returns minval to maxval, inclusive                      *
 *                                                                         * 
 * INPUT:                                                                  * 
 *      minval,maxval      range limits                                    *
 *                                                                         * 
 * OUTPUT:                                                                 * 
 *      random value                                                       *
 *                                                                         * 
 * WARNINGS:                                                               * 
 *      none.                                                              *
 *                                                                         * 
 * HISTORY:                                                                * 
 *   04/25/1995 BRR : Created.                                             * 
 *   06/17/1995 JLB : Uses fixed point math helper routine.                * 
 *=========================================================================*/
int Sim_IRandom(int minval, int maxval)
{
	return(Fixed_To_Cardinal((maxval-minval), Sim_Random()) + minval);
}


/*---------------------------------------------------------------------------
This routine can be used to create the _RandVals[] table.  It will be 
different every time it's run.
---------------------------------------------------------------------------*/
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void main(void);

void main(void)
{
	int i;
	FILE *fp;
	int r;
	char is_used[256];
	char rand_val[256];

	srand ( time ( NULL ) ) ;

	for (i = 0; i < 256; i++) {
		is_used[i] = 0;
	}

	/*
	** Store the digits 0 to 255 in a random order within the 'rand_val' array
	** This ensures our random number sequence represents every value.
	*/
	for (i = 0; i < 256; i++) {
		if (kbhit()!=0) {
			printf("ABORTED!\n");
			break;
		}
		r = (rand() * 256) / RAND_MAX;		// r is the index into array
		if (is_used[r]==0) {
			is_used[r] = 1;
			rand_val[r] = i;
		} else {
			i--;
		}
	}

	fp = fopen("rand.cpp","wt");
	if (fp==NULL) {
		printf("File error\n");
		exit(0);
	}

	fprintf(fp, "unsigned char _RandVals[] = {\n");
	for (i = 0; i < 256; i++) {
		fprintf(fp,"0x%02x,\n",rand_val[i]);
	}
	fprintf(fp,"};\n");
	fclose(fp);

}

#endif

