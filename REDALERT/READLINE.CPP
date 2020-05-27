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
#include	<ctype.h>
#include	<string.h>
#include	"wwfile.h"
#include	"xstraw.h"
#include	"readline.h"


// Disable the "temporary object used to initialize a non-constant reference" warning.
//#pragma warning 665 9


void strtrimcpp(char * buffer)
{
	if (buffer) {

		/*
		**	Strip leading white space from the string.
		*/
		char * source = buffer;
		while (isspace(*source)) {
			source++;
		}
		if (source != buffer) {
			strcpy(buffer, source);
		}

		/*
		**	Clip trailing white space from the string.
		*/
		for (int index = strlen(buffer)-1; index >= 0; index--) {
			if (isspace(buffer[index])) {
				buffer[index] = '\0';
			} else {
				break;
			}
		}
	}
}



int Read_Line(FileClass & file, char * buffer, int len, bool & eof)
{
	return(Read_Line(FileStraw(file), buffer, len, eof));
}


int Read_Line(Straw & file, char * buffer, int len, bool & eof)
{
	if (len == 0 || buffer == NULL) return(0);

	int count = 0;
	for (;;) {
		char c;
		if (file.Get(&c, sizeof(c)) != sizeof(c)) {
			eof = true;
			buffer[0] = '\0';
			break;
		}

		if (c == '\x0A') break;
		if (c != '\x0D' && count+1 < len) {
			buffer[count++] = c;
		}
	}
	buffer[count] = '\0';

	strtrimcpp(buffer);
	return(strlen(buffer));
}
