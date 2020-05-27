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

/* $Header:   F:\projects\c&c0\vcs\code\audio.h_v   4.43   05 Jul 1996 17:58:10   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : AUDIO.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 21, 1994                                                *
 *                                                                                             *
 *                  Last Update : June 21, 1994   [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef AUDIO_H
#define AUDIO_H

#include	"memory.h"

class AudioClass {
		char const * Name;	// Name of audio asset.
		void const * Data;	// Loaded audio data.
		int Handle;				// Handle of asset (as it is playing).
		MemoryClass *Mem;		// Pointer to memory handler class.
		unsigned IsMIDI:1;	// Is this a midi file?

	public:
		AudioClass(void);
		AudioClass(char const *name, MemoryClass &mem);
		virtual ~AudioClass(void);

		bool Load(char const *name = 0);
		bool Free(void);
		bool Play(int volume = 0xFF);
		bool Stop(void);
		bool Pause(void);
		bool Resume(void);
		bool Set_Name(char const *name);
		bool Is_Playing(void) const;
		bool Is_Loaded(void) const;
		bool Is_MIDI(void) const;
};

inline AudioClass::AudioClass(void)
{
	Name = 0; 
	Data = 0; 
	Mem = 0;
	Handle = -1;
};

inline AudioClass::AudioClass(char const *name, MemoryClass &mem) 
{
	if (mem) {
		Mem = &mem;
	} else {
		Mem = &::Mem;		// Uses global default memory handler.
	}
	Name = strdup(name);
	Data = 0;
	Handle = -1;
};

inline AudioClass::~AudioClass(void)
{
	if (GameActive) {
		if (Name) free(Name);
		if (Data) Mem->Free(Data);
		Name = 0;
		Data = 0;
		Handle = -1;
	}
};


#endif
