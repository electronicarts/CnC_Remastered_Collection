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

#include "function.h"

extern "C" {
#include "soscomp.h"
#include "itable.cpp"
#include "dtable.cpp"


void sosCODECInitStream(_SOS_COMPRESS_INFO* info)
{
	info->dwSampleIndex = 0;
	info->dwPredicted = 0;
}


unsigned long sosCODECDecompressData(_SOS_COMPRESS_INFO* info, unsigned long numbytes)
{
	unsigned long token;
	long sample;
	unsigned int fastindex;
	unsigned char *inbuff;
	unsigned short *outbuff;

	inbuff = (unsigned char *)info->lpSource;
	outbuff = (unsigned short *)info->lpDest;

	// Preload variables before the big loop
	fastindex = (unsigned int)info->dwSampleIndex;
	sample = info->dwPredicted;

	if (!numbytes)
		goto SkipLoop;

	do {
		// First nibble
		token = *inbuff++;
		fastindex += token & 0x0f;
		sample += DiffTable[fastindex];
		fastindex = IndexTable[fastindex];
		if (sample > 32767L)
			sample = 32767L;
		if (sample < -32768L)
			sample = -32768L;
		*outbuff++ = (unsigned short)sample;

		// Second nibble
		fastindex += token >> 4;
		sample += DiffTable[fastindex];
		fastindex = IndexTable[fastindex];
		if (sample > 32767L)
			sample = 32767L;
		if (sample < -32768L)
			sample = -32768L;
		*outbuff++ = (unsigned short)sample;
	} while(--numbytes);

SkipLoop:

	// Put local vars back
	info->dwSampleIndex = (unsigned long)fastindex;
	info->dwPredicted = sample;
	return(numbytes << 2);
}

}
