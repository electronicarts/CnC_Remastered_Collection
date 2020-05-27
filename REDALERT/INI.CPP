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

/* $Header: /CounterStrike/INI.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INI.CPP                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : November 2, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   INIClass::Clear -- Clears out a section (or all sections) of the INI data.                *
 *   INIClass::Entry_Count -- Fetches the number of entries in a specified section.            *
 *   INIClass::Find_Entry -- Find specified entry within section.                              *
 *   INIClass::Find_Section -- Find the specified section within the INI data.                 *
 *   INIClass::Get_Bool -- Fetch a boolean value for the section and entry specified.          *
 *   INIClass::Get_Entry -- Get the entry identifier name given ordinal number and section name*
 *   INIClass::Get_Fixed -- Fetch a fixed point number from the section & entry.               *
 *   INIClass::Put_Fixed -- Store a fixed point number to the INI database.                    *
 *   INIClass::Get_Hex -- Fetches integer [hex format] from the section and entry specified.   *
 *   INIClass::Get_Int -- Fetch an integer entry from the specified section.                   *
 *   INIClass::Get_PKey -- Fetch a key from the ini database.                                  *
 *   INIClass::Get_String -- Fetch the value of a particular entry in a specified section.     *
 *   INIClass::Get_TextBlock -- Fetch a block of normal text.                                  *
 *   INIClass::Get_UUBlock -- Fetch an encoded block from the section specified.               *
 *   INIClass::INISection::Find_Entry -- Finds a specified entry and returns pointer to it.    *
 *   INIClass::Load -- Load INI data from the file specified.                                  *
 *   INIClass::Load -- Load the INI data from the data stream (straw).                         *
 *   INIClass::Put_Bool -- Store a boolean value into the INI database.                        *
 *   INIClass::Put_Hex -- Store an integer into the INI database, but use a hex format.        *
 *   INIClass::Put_Int -- Stores a signed integer into the INI data base.                      *
 *   INIClass::Put_PKey -- Stores the key to the INI database.                                 *
 *   INIClass::Put_String -- Output a string to the section and entry specified.               *
 *   INIClass::Put_TextBlock -- Stores a block of text into an INI section.                    *
 *   INIClass::Put_UUBlock -- Store a binary encoded data block into the INI database.         *
 *   INIClass::Save -- Save the ini data to the file specified.                                *
 *   INIClass::Save -- Saves the INI data to a pipe stream.                                    *
 *   INIClass::Section_Count -- Counts the number of sections in the INI data.                 *
 *   INIClass::Strip_Comments -- Strips comments of the specified text line.                   *
 *   INIClass::~INIClass -- Destructor for INI handler.                                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	<string.h>
#include	<stdlib.h>
#include	<stddef.h>
#include	<stdio.h>
#include	<ctype.h>
#include	"ini.h"
#include	"readline.h"
#include	"xpipe.h"
#include	"b64pipe.h"
#include	"xstraw.h"
#include	"b64straw.h"


#ifdef FIXIT_FAST_LOAD
#include	"cstraw.h"
#endif



// Disable the "temporary object used to initialize a non-constant reference" warning.
//#pragma warning 665 9


/***********************************************************************************************
 * INIClass::~INIClass -- Destructor for INI handler.                                          *
 *                                                                                             *
 *    This is the destructor for the INI class. It handles deleting all of the allocations     *
 *    it might have done.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
INIClass::~INIClass(void)
{
	Clear();
}


/***********************************************************************************************
 * INIClass::Clear -- Clears out a section (or all sections) of the INI data.                  *
 *                                                                                             *
 *    This routine is used to clear out the section specified. If no section is specified,     *
 *    then the entire INI data is cleared out. Optionally, this routine can be used to clear   *
 *    out just an individual entry in the specified section.                                   *
 *                                                                                             *
 * INPUT:   section  -- Pointer to the section to clear out [pass NULL to clear all].          *
 *                                                                                             *
 *          entry    -- Pointer to optional entry specifier. If this parameter is specified,   *
 *                      then only this specific entry (if found) will be cleared. Otherwise,   *
 *                      the entire section specified will be cleared.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *   08/21/1996 JLB : Optionally clears section too.                                           *
 *   11/02/1996 JLB : Updates the index list.                                                  *
 *=============================================================================================*/
bool INIClass::Clear(char const * section, char const * entry)
{
	if (section == NULL) {
		SectionList.Delete();
		SectionIndex.Clear();
	} else {
		INISection * secptr = Find_Section(section);
		if (secptr != NULL) {
			if (entry != NULL) {
				INIEntry * entptr = secptr->Find_Entry(entry);
				if (entptr != NULL) {
					/*
					**	Remove the entry from the entry index list.
					*/
					secptr->EntryIndex.Remove_Index(entptr->Index_ID());

					delete entptr;
				}
			} else {
				/*
				**	Remove this section index from the section index list.
				*/
				SectionIndex.Remove_Index(secptr->Index_ID());

				delete secptr;
			}
		}
	}
	return(true);
}


/***********************************************************************************************
 * INIClass::Load -- Load INI data from the file specified.                                    *
 *                                                                                             *
 *    Use this routine to load the INI class with the data from the specified file.            *
 *                                                                                             *
 * INPUT:   file  -- Reference to the file that will be used to fill up this INI manager.      *
 *                                                                                             *
 * OUTPUT:  bool; Was the file loaded successfully?                                            *
 *                                                                                             *
 * WARNINGS:   This routine allocates memory.                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool INIClass::Load(FileClass & file)
{
	return(Load(FileStraw(file)));
}


/***********************************************************************************************
 * INIClass::Load -- Load the INI data from the data stream (straw).                           *
 *                                                                                             *
 *    This will fetch data from the straw and build an INI database from it.                   *
 *                                                                                             *
 * INPUT:   straw -- The straw that the data will be provided from.                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the database loaded ok?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef FIXIT_FAST_LOAD
bool INIClass::Load(Straw & ffile)
#else
bool INIClass::Load(Straw & file)
#endif
{
	bool end_of_file = false;
	char buffer[MAX_LINE_LENGTH];

#ifdef FIXIT_FAST_LOAD
	CacheStraw file;
	file.Get_From(ffile);
#endif

	/*
	**	Prescan until the first section is found.
	*/
	while (!end_of_file) {
		Read_Line(file, buffer, sizeof(buffer), end_of_file);
		if (end_of_file) return(false);
		if (buffer[0] == '[' && strchr(buffer, ']') != NULL) break;
	}

	/*
	**	Process a section. The buffer is prefilled with the section name line.
	*/
	while (!end_of_file) {

		buffer[0] = ' ';
		char * ptr = strchr(buffer, ']');
		if (ptr) *ptr = '\0';
		strtrim(buffer);
		INISection * secptr = new INISection(strdup(buffer));
		if (secptr == NULL) {
			Clear();
			return(false);
		}

		/*
		**	Read in the entries of this section.
		*/
		while (!end_of_file) {

			/*
			**	If this line is the start of another section, then bail out
			**	of the entry loop and let the outer section loop take
			**	care of it.
			*/
			int len = Read_Line(file, buffer, sizeof(buffer), end_of_file);
			if (buffer[0] == '[' && strchr(buffer, ']') != NULL) break;

			/*
			**	Determine if this line is a comment or blank line. Throw it out if it is.
			*/
			Strip_Comments(buffer);
			if (len == 0 || buffer[0] == ';' || buffer[0] == '=') continue;

			/*
			**	The line isn't an obvious comment. Make sure that there is the "=" character
			**	at an appropriate spot.
			*/
			char * divider = strchr(buffer, '=');
			if (!divider) continue;

			/*
			**	Split the line into entry and value sections. Be sure to catch the
			**	"=foobar" and "foobar=" cases. These lines are ignored.
			*/
			*divider++ = '\0';
			strtrim(buffer);
			if (!strlen(buffer)) continue;

			strtrim(divider);
			if (!strlen(divider)) continue;

			INIEntry * entryptr = new INIEntry(strdup(buffer), strdup(divider));
			if (entryptr == NULL) {
				delete secptr;
				Clear();
				return(false);
			}

			secptr->EntryIndex.Add_Index(entryptr->Index_ID(), entryptr);
			secptr->EntryList.Add_Tail(entryptr);
		}

		/*
		**	All the entries for this section have been parsed. If this section is blank, then
		**	don't bother storing it.
		*/
		if (secptr->EntryList.Is_Empty()) {
			delete secptr;
		} else {
			SectionIndex.Add_Index(secptr->Index_ID(), secptr);
			SectionList.Add_Tail(secptr);
		}
	}
	return(true);
}


/***********************************************************************************************
 * INIClass::Save -- Save the ini data to the file specified.                                  *
 *                                                                                             *
 *    Use this routine to save the ini data to the file specified. All existing data in the    *
 *    file, if it was present, is replaced.                                                    *
 *                                                                                             *
 * INPUT:   file  -- Reference to the file to write the INI data to.                           *
 *                                                                                             *
 * OUTPUT:  bool; Was the data written to the file?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int INIClass::Save(FileClass & file) const
{
	return(Save(FilePipe(file)));
}


/***********************************************************************************************
 * INIClass::Save -- Saves the INI data to a pipe stream.                                      *
 *                                                                                             *
 *    This routine will output the data of the INI file to a pipe stream.                      *
 *                                                                                             *
 * INPUT:   pipe  -- Reference to the pipe stream to pump the INI image to.                    *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes output to the pipe.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int INIClass::Save(Pipe & pipe) const
{
	int total = 0;

	INISection * secptr = SectionList.First();
	while (secptr && secptr->Is_Valid()) {

		/*
		**	Output the section identifier.
		*/
		total += pipe.Put("[", 1);
		total += pipe.Put(secptr->Section, strlen(secptr->Section));
		total += pipe.Put("]", 1);
		total += pipe.Put("\r\n", strlen("\r\n"));

		/*
		**	Output all the entries and values in this section.
		*/
		INIEntry * entryptr = secptr->EntryList.First();
		while (entryptr && entryptr->Is_Valid()) {
			total += pipe.Put(entryptr->Entry, strlen(entryptr->Entry));
			total += pipe.Put("=", 1);
			total += pipe.Put(entryptr->Value, strlen(entryptr->Value));
			total += pipe.Put("\r\n", strlen("\r\n"));

			entryptr = entryptr->Next();
		}

		/*
		**	After the last entry in this section, output an extra
		**	blank line for readability purposes.
		*/
		total += pipe.Put("\r\n", strlen("\r\n"));

		secptr = secptr->Next();
	}
	total += pipe.End();

	return(total);
}


/***********************************************************************************************
 * INIClass::Find_Section -- Find the specified section within the INI data.                   *
 *                                                                                             *
 *    This routine will scan through the INI data looking for the section specified. If the    *
 *    section could be found, then a pointer to the section control data is returned.          *
 *                                                                                             *
 * INPUT:   section  -- The name of the section to search for. Don't enclose the name in       *
 *                      brackets. Case is NOT sensitive in the search.                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the INI section control structure if the section was     *
 *          found. Otherwise, NULL is returned.                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *   11/02/1996 JLB : Uses index manager.                                                      *
 *=============================================================================================*/
INIClass::INISection * INIClass::Find_Section(char const * section) const
{
	if (section != NULL) {

		long crc = CRCEngine()(section, strlen(section));

		if (SectionIndex.Is_Present(crc)) {
			return(SectionIndex.Fetch_Index(crc));
		}
	}
	return(NULL);
}


/***********************************************************************************************
 * INIClass::Section_Count -- Counts the number of sections in the INI data.                   *
 *                                                                                             *
 *    This routine will scan through all the sections in the INI data and return a count       *
 *    of the number it found.                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of sections recorded in the INI data.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *   11/02/1996 JLB : Uses index manager.                                                      *
 *=============================================================================================*/
int INIClass::Section_Count(void) const
{
	return(SectionIndex.Count());
}


/***********************************************************************************************
 * INIClass::Entry_Count -- Fetches the number of entries in a specified section.              *
 *                                                                                             *
 *    This routine will examine the section specified and return with the number of entries    *
 *    associated with it.                                                                      *
 *                                                                                             *
 * INPUT:   section  -- Pointer to the section that will be examined.                          *
 *                                                                                             *
 * OUTPUT:  Returns with the number entries in the specified section.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *   11/02/1996 JLB : Uses index manager.                                                      *
 *=============================================================================================*/
int INIClass::Entry_Count(char const * section) const
{
	INISection * secptr = Find_Section(section);
	if (secptr != NULL) {
		return(secptr->EntryIndex.Count());
	}
	return(0);
}


/***********************************************************************************************
 * INIClass::Find_Entry -- Find specified entry within section.                                *
 *                                                                                             *
 *    This support routine will find the specified entry in the specified section. If found,   *
 *    a pointer to the entry control structure will be returned.                               *
 *                                                                                             *
 * INPUT:   section  -- Pointer to the section name to search under.                           *
 *                                                                                             *
 *          entry    -- Pointer to the entry name to search for.                               *
 *                                                                                             *
 * OUTPUT:  If the entry was found, then a pointer to the entry control structure will be      *
 *          returned. Otherwise, NULL will be returned.                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
INIClass::INIEntry * INIClass::Find_Entry(char const * section, char const * entry) const
{
	INISection * secptr = Find_Section(section);
	if (secptr != NULL) {
		return(secptr->Find_Entry(entry));
	}
	return(NULL);
}


/***********************************************************************************************
 * INIClass::Get_Entry -- Get the entry identifier name given ordinal number and section name. *
 *                                                                                             *
 *    This will return the identifier name for the entry under the section specified. The      *
 *    ordinal number specified is used to determine which entry to retrieve. The entry         *
 *    identifier is the text that appears to the left of the "=" character.                    *
 *                                                                                             *
 * INPUT:   section  -- The section to use.                                                    *
 *                                                                                             *
 *          index    -- The ordinal number to use when fetching an entry name.                 *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the entry name.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * INIClass::Get_Entry(char const * section, int index) const
{
	INISection * secptr = Find_Section(section);

	if (secptr != NULL && index < secptr->EntryIndex.Count()) {
		INIEntry * entryptr = secptr->EntryList.First();

		while (entryptr != NULL && entryptr->Is_Valid()) {
			if (index == 0) return(entryptr->Entry);
			index--;
			entryptr = entryptr->Next();
		}
	}
	return(NULL);
}


/***********************************************************************************************
 * INIClass::Put_UUBlock -- Store a binary encoded data block into the INI database.           *
 *                                                                                             *
 *    Use this routine to store an arbitrary length binary block of data into the INI database.*
 *    This routine will covert the data into displayable form and then break it into lines     *
 *    that are stored in sequence to the section. A section used to store data in this         *
 *    fashion can not be used for any other entries.                                           *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to place the data into.                         *
 *                                                                                             *
 *          block    -- Pointer to the block of binary data to store.                          *
 *                                                                                             *
 *          len      -- The length of the binary data.                                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the data stored to the database?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool INIClass::Put_UUBlock(char const * section, void const * block, int len)
{
	if (section == NULL || block == NULL || len < 1) return(false);

	Clear(section);

	BufferStraw straw(block, len);
	Base64Straw bstraw(Base64Straw::ENCODE);
	bstraw.Get_From(straw);

	int counter = 1;

	for (;;) {
		char buffer[71];
		char sbuffer[32];

		int length = bstraw.Get(buffer, sizeof(buffer)-1);
		buffer[length] = '\0';
		if (length == 0) break;

		sprintf(sbuffer, "%d", counter);
		Put_String(section, sbuffer, buffer);
		counter++;
	}
	return(true);
}


/***********************************************************************************************
 * INIClass::Get_UUBlock -- Fetch an encoded block from the section specified.                 *
 *                                                                                             *
 *    This routine will take all the entries in the specified section and decompose them into  *
 *    a binary block of data that will be stored into the buffer specified. By using this      *
 *    routine [and the Put_UUBLock counterpart], arbitrary blocks of binary data may be        *
 *    stored in the INI file. A section processed by this routine can contain no other         *
 *    entries than those put there by a previous call to Put_UUBlock.                          *
 *                                                                                             *
 * INPUT:   section  -- The section name to process.                                           *
 *                                                                                             *
 *          block    -- Pointer to the buffer that will hold the retrieved data.               *
 *                                                                                             *
 *          len      -- The length of the buffer. The retrieved data will not fill past this   *
 *                      limit.                                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes decoded into the buffer specified.                *
 *                                                                                             *
 * WARNINGS:   If the number of bytes retrieved exactly matches the length of the buffer       *
 *             specified, then you might have a condition of buffer "overflow".                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int INIClass::Get_UUBlock(char const * section, void * block, int len) const
{
	if (section == NULL) return(0);

	Base64Pipe b64pipe(Base64Pipe::DECODE);
	BufferPipe bpipe(block, len);

	b64pipe.Put_To(&bpipe);

	int total = 0;
	int counter = Entry_Count(section);
	for (int index = 0; index < counter; index++) {
		char buffer[128];

		int length = Get_String(section, Get_Entry(section, index), "=", buffer, sizeof(buffer));
		int outcount = b64pipe.Put(buffer, length);
		total += outcount;
	}
	total += b64pipe.End();
	return(total);
}


/***********************************************************************************************
 * INIClass::Put_TextBlock -- Stores a block of text into an INI section.                      *
 *                                                                                             *
 *    This routine will take an arbitrarily long block of text and store it into the INI       *
 *    database. The text is broken up into lines and each line is then stored as a numbered    *
 *    entry in the specified section. A section used to store text in this way can not be used *
 *    to hold any other entries. The text is presumed to contain space characters scattered    *
 *    throughout it and that one space between words and sentences is natural.                 *
 *                                                                                             *
 * INPUT:   section  -- The section to place the text block into.                              *
 *                                                                                             *
 *          text     -- Pointer to a null terminated text string that holds the block of       *
 *                      text. The length can be arbitrary.                                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the text block placed into the database?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool INIClass::Put_TextBlock(char const * section, char const * text)
{
	if (section == NULL) return(false);

	Clear(section);

	int index = 1;
	while (text != NULL && *text != NULL) {

		char buffer[128];

		strncpy(buffer, text, 75);
		buffer[75] = '\0';

		char b[32];
		sprintf(b, "%d", index);

		/*
		**	Scan backward looking for a good break position.
		*/
		int count = strlen(buffer);
		if (count > 0) {
			if (count >= 75) {
				while (count) {
					char c = buffer[count];

					if (isspace(c)) break;
					count--;
				}

				if (count == 0) {
					break;
				} else {
					buffer[count] = '\0';
				}
			}

			strtrim(buffer);
			Put_String(section, b, buffer);
			index++;
			text = ((char  *)text) + count;
		} else {
			break;
		}
	}
	return(true);
}


/***********************************************************************************************
 * INIClass::Get_TextBlock -- Fetch a block of normal text.                                    *
 *                                                                                             *
 *    This will take all entries in the specified section and format them into a block of      *
 *    normalized text. That is, text with single spaces between each concatenated line. All    *
 *    entries in the specified section are processed by this routine. Use Put_TextBlock to     *
 *    build the entries in the section.                                                        *
 *                                                                                             *
 * INPUT:   section  -- The section name to process.                                           *
 *                                                                                             *
 *          buffer   -- Pointer to the buffer that will hold the complete text.                *
 *                                                                                             *
 *          len      -- The length of the buffer specified. The text will, at most, fill this  *
 *                      buffer with the last character being forced to null.                   *
 *                                                                                             *
 * OUTPUT:  Returns with the number of characters placed into the buffer. The trailing null    *
 *          is not counted.                                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int INIClass::Get_TextBlock(char const * section, char * buffer, int len) const
{
	if (len <= 0) return(0);

	buffer[0] = '\0';
	if (len <= 1) return(0);

	int elen = Entry_Count(section);
	int total = 0;
	for (int index = 0; index < elen; index++) {

		/*
		**	Add spacers between lines of fetched text.
		*/
		if (index > 0) {
			*buffer++ = ' ';
			len--;
			total++;
		}

		Get_String(section, Get_Entry(section, index), "", buffer, len);

		int partial = strlen(buffer);
		total += partial;
		buffer += partial;
		len -= partial;
		if (len <= 1) break;
	}
	return(total);
}


/***********************************************************************************************
 * INIClass::Put_Int -- Stores a signed integer into the INI data base.                        *
 *                                                                                             *
 *    Use this routine to store an integer value into the section and entry specified.         *
 *                                                                                             *
 * INPUT:   section  -- The identifier for the section that the entry will be placed in.       *
 *                                                                                             *
 *          entry    -- The entry identifier used for the integer number.                      *
 *                                                                                             *
 *          number   -- The integer number to store in the database.                           *
 *                                                                                             *
 *          format   -- The format to store the integer. The format is generally only a        *
 *                      cosmetic affect. The Get_Int operation will interpret the value the    *
 *                      same regardless of what format was used to store the integer.          *
 *                                                                                             *
 *                      0  : plain decimal digit                                               *
 *                      1  : hexadecimal digit (trailing "h")                                  *
 *                      2  : hexadecimal digit (leading "$")                                   *
 *                                                                                             *
 * OUTPUT:  bool; Was the number stored?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *   07/10/1996 JLB : Handles multiple integer formats.                                        *
 *=============================================================================================*/
bool INIClass::Put_Int(char const * section, char const * entry, int number, int format)
{
	char buffer[MAX_LINE_LENGTH];

	switch (format) {
		default:
		case 0:
			sprintf(buffer, "%d", number);
			break;

		case 1:
			sprintf(buffer, "%Xh", number);
			break;

		case 2:
			sprintf(buffer, "$%X", number);
			break;
	}
	return(Put_String(section, entry, buffer));
}


/***********************************************************************************************
 * INIClass::Get_Int -- Fetch an integer entry from the specified section.                     *
 *                                                                                             *
 *    This routine will fetch an integer value from the entry and section specified. If no     *
 *    entry could be found, then the default value will be returned instead.                   *
 *                                                                                             *
 * INPUT:   section  -- The section name to search under.                                      *
 *                                                                                             *
 *          entry    -- The entry name to search for.                                          *
 *                                                                                             *
 *          defvalue -- The default value to use if the specified entry could not be found.    *
 *                                                                                             *
 * OUTPUT:  Returns with the integer value specified in the INI database or else returns the   *
 *          default value.                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *   07/10/1996 JLB : Handles multiple integer formats.                                        *
 *=============================================================================================*/
int INIClass::Get_Int(char const * section, char const * entry, int defvalue) const
{
	/*
	**	Verify that the parameters are nominally correct.
	*/
	if (section == NULL || entry == NULL) return(defvalue);

	INIEntry * entryptr = Find_Entry(section, entry);
	if (entryptr && entryptr->Value != NULL) {

		if (*entryptr->Value == '$') {
			sscanf(entryptr->Value, "$%x", &defvalue);
		} else {
			if (tolower(entryptr->Value[strlen(entryptr->Value)-1]) == 'h') {
				sscanf(entryptr->Value, "%xh", &defvalue);
			} else {
				defvalue = atoi(entryptr->Value);
			}
		}
	}
	return(defvalue);
}


/***********************************************************************************************
 * INIClass::Put_Hex -- Store an integer into the INI database, but use a hex format.          *
 *                                                                                             *
 *    This routine is similar to the Put_Int routine, but the number is stored as a hexadecimal*
 *    number.                                                                                  *
 *                                                                                             *
 * INPUT:   section  -- The identifier for the section that the entry will be placed in.       *
 *                                                                                             *
 *          entry    -- The entry identifier to tag to the integer number specified.           *
 *                                                                                             *
 *          number   -- The number to assign the the specified entry and placed in the         *
 *                      specified section.                                                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the number placed into the INI database?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool INIClass::Put_Hex(char const * section, char const * entry, int number)
{
	char buffer[MAX_LINE_LENGTH];

	sprintf(buffer, "%X", number);
	return(Put_String(section, entry, buffer));
}


/***********************************************************************************************
 * INIClass::Get_Hex -- Fetches integer [hex format] from the section and entry specified.     *
 *                                                                                             *
 *    This routine will search under the section specified, looking for a matching entry. The  *
 *    value is interpreted as a hexadecimal number and then returned. If no entry could be     *
 *    found, then the default value is returned instead.                                       *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to search under.                                *
 *                                                                                             *
 *          entry    -- The entry identifier to search for.                                    *
 *                                                                                             *
 *          defvalue -- The default value to use if the entry could not be located.            *
 *                                                                                             *
 * OUTPUT:  Returns with the integer value from the specified section and entry. If no entry   *
 *          could be found, then the default value will be returned instead.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int INIClass::Get_Hex(char const * section, char const * entry, int defvalue) const
{
	/*
	**	Verify that the parameters are nominally correct.
	*/
	if (section == NULL || entry == NULL) return(defvalue);

	INIEntry * entryptr = Find_Entry(section, entry);
	if (entryptr && entryptr->Value != NULL) {
		sscanf(entryptr->Value, "%x", &defvalue);
	}
	return(defvalue);
}


/***********************************************************************************************
 * INIClass::Put_String -- Output a string to the section and entry specified.                 *
 *                                                                                             *
 *    This routine will put an arbitrary string to the section and entry specified. Any        *
 *    previous matching entry will be replaced.                                                *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to place the string under.                      *
 *                                                                                             *
 *          entry    -- The entry identifier to identify this string [placed under the section]*
 *                                                                                             *
 *          string   -- Pointer to the string to assign to this entry.                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the entry assigned without error?                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *   11/02/1996 JLB : Uses index handler.                                                      *
 *=============================================================================================*/
bool INIClass::Put_String(char const * section, char const * entry, char const * string)
{
	if (section == NULL || entry == NULL) return(false);

	INISection * secptr = Find_Section(section);

	if (secptr == NULL) {
		secptr = new INISection(strdup(section));
		if (secptr == NULL) return(false);
		SectionList.Add_Tail(secptr);
		SectionIndex.Add_Index(secptr->Index_ID(), secptr);
	}

	/*
	**	Remove the old entry if found.
	*/
	INIEntry * entryptr = secptr->Find_Entry(entry);
	if (entryptr != NULL) {
		secptr->EntryIndex.Remove_Index(entryptr->Index_ID());
		delete entryptr;
	}

	/*
	**	Create and add the new entry.
	*/
	if (string != NULL && strlen(string) > 0) {
		entryptr = new INIEntry(strdup(entry), strdup(string));

		if (entryptr == NULL) {
			return(false);
		}
		secptr->EntryList.Add_Tail(entryptr);
		secptr->EntryIndex.Add_Index(entryptr->Index_ID(), entryptr);
	}
	return(true);
}


/***********************************************************************************************
 * INIClass::Get_String -- Fetch the value of a particular entry in a specified section.       *
 *                                                                                             *
 *    This will retrieve the entire text to the right of the "=" character. The text is        *
 *    found by finding a matching entry in the section specified. If no matching entry could   *
 *    be found, then the default value will be stored in the output string buffer.             *
 *                                                                                             *
 * INPUT:   section  -- Pointer to the section name to search under.                           *
 *                                                                                             *
 *          entry    -- The entry identifier to search for.                                    *
 *                                                                                             *
 *          defvalue -- If no entry could be found, then this text will be returned.           *
 *                                                                                             *
 *          buffer   -- Output buffer to store the retrieved string into.                      *
 *                                                                                             *
 *          size     -- The size of the output buffer. The maximum string length that could    *
 *                      be retrieved will be one less than this length. This is due to the     *
 *                      forced trailing zero added to the end of the string.                   *
 *                                                                                             *
 * OUTPUT:  Returns with the length of the string retrieved.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int INIClass::Get_String(char const * section, char const * entry, char const * defvalue, char * buffer, int size) const
{
	/*
	**	Verify that the parameters are nominally legal.
	*/
	if (section == NULL || entry == NULL) {
		if (buffer != NULL && size > 0) {
			buffer[0] = '\0';
		}
		return(0);
	}

	/*
	**	Fetch the entry string if it is present. If not, then the normal default
	**	value will be used as the entry value.
	*/
	INIEntry * entryptr = Find_Entry(section, entry);
	if (entryptr) {
		if (entryptr->Value) {
			defvalue = entryptr->Value;
		}
	}

	/*
	**	Fill in the buffer with the entry value and return with the length of the string.
	*/
	if (buffer == NULL || !size) {
		return(0);
	} else if (defvalue == NULL) {
		buffer[0] = '\0';
		return(0);
	} else if (buffer == defvalue) {
		return(strlen(buffer));
	} else {
		strncpy(buffer, defvalue, size);
		buffer[size-1] = '\0';
		strtrim(buffer);
		return(strlen(buffer));
	}
}


/***********************************************************************************************
 * INIClass::Put_Bool -- Store a boolean value into the INI database.                          *
 *                                                                                             *
 *    Use this routine to place a boolean value into the INI database. The boolean value will  *
 *    be stored as "yes" or "no".                                                              *
 *                                                                                             *
 * INPUT:   section  -- The section to place the entry and boolean value into.                 *
 *                                                                                             *
 *          entry    -- The entry identifier to tag to the boolean value.                      *
 *                                                                                             *
 *          value    -- The boolean value to place into the database.                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the boolean value placed into the database?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool INIClass::Put_Bool(char const * section, char const * entry, bool value)
{
	if (value) {
		return(Put_String(section, entry, "yes"));
	} else {
		return(Put_String(section, entry, "no"));
	}
}


/***********************************************************************************************
 * INIClass::Get_Bool -- Fetch a boolean value for the section and entry specified.            *
 *                                                                                             *
 *    This routine will search under the section specified, looking for a matching entry. If   *
 *    one is found, the value is interpreted as a boolean value and then returned. In the case *
 *    of no matching entry, the default value will be returned instead. The boolean value      *
 *    is interpreted using the standard boolean conventions. e.g., "Yes", "Y", "1", "True",    *
 *    "T" are all consider to be a TRUE boolean value.                                         *
 *                                                                                             *
 * INPUT:   section  -- The section to search under.                                           *
 *                                                                                             *
 *          entry    -- The entry to search for.                                               *
 *                                                                                             *
 *          defvalue -- The default value to use if no matching entry could be located.        *
 *                                                                                             *
 * OUTPUT:  Returns with the boolean value of the specified section and entry. If no match     *
 *          then the default boolean value is returned.                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool INIClass::Get_Bool(char const * section, char const * entry, bool defvalue) const
{
	/*
	**	Verify that the parameters are nominally correct.
	*/
	if (section == NULL || entry == NULL) return(defvalue);

	INIEntry * entryptr = Find_Entry(section, entry);
	if (entryptr && entryptr->Value != NULL) {
		switch (toupper(*entryptr->Value)) {
			case 'Y':
			case 'T':
			case '1':
				return(true);

			case 'N':
			case 'F':
			case '0':
				return(false);
		}
	}
	return(defvalue);
}


/***********************************************************************************************
 * INIClass::INISection::Find_Entry -- Finds a specified entry and returns pointer to it.      *
 *                                                                                             *
 *    This routine scans the supplied entry for the section specified. This is used for        *
 *    internal database maintenance.                                                           *
 *                                                                                             *
 * INPUT:   entry -- The entry to scan for.                                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the entry control structure if the entry was found.      *
 *          Otherwise it returns NULL.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *   11/02/1996 JLB : Uses index handler.                                                      *
 *=============================================================================================*/
INIClass::INIEntry * INIClass::INISection::Find_Entry(char const * entry) const
{
	if (entry != NULL) {
		int crc = CRCEngine()(entry, strlen(entry));
		if (EntryIndex.Is_Present(crc)) {
			return(EntryIndex.Fetch_Index(crc));
		}
	}
	return(NULL);
}


/***********************************************************************************************
 * INIClass::Put_PKey -- Stores the key to the INI database.                                   *
 *                                                                                             *
 *    The key stored to the database will have both the exponent and modulus portions saved.   *
 *    Since the fast key only requires the modulus, it is only necessary to save the slow      *
 *    key to the database. However, storing the slow key stores the information necessary to   *
 *    generate the fast and slow keys. Because public key encryption requires one key to be    *
 *    completely secure, only store the fast key in situations where the INI database will     *
 *    be made public.                                                                          *
 *                                                                                             *
 * INPUT:   key   -- The key to store the INI database.                                        *
 *                                                                                             *
 * OUTPUT:  bool; Was the key stored to the database?                                          *
 *                                                                                             *
 * WARNINGS:   Store the fast key for public INI database availability. Store the slow key if  *
 *             the INI database is secure.                                                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool INIClass::Put_PKey(PKey const & key)
{
	char buffer[512];

	int len = key.Encode_Modulus(buffer);
	Put_UUBlock("PublicKey", buffer, len);

	len = key.Encode_Exponent(buffer);
	Put_UUBlock("PrivateKey", buffer, len);
	return(true);
}


/***********************************************************************************************
 * INIClass::Get_PKey -- Fetch a key from the ini database.                                    *
 *                                                                                             *
 *    This routine will fetch the key from the INI database. The key fetched is controlled by  *
 *    the parameter. There are two choices of key -- the fast or slow key.                     *
 *                                                                                             *
 * INPUT:   fast  -- Should the fast key be retrieved? The fast key has the advantage of       *
 *                   requiring only the modulus value.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the key retrieved.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
PKey INIClass::Get_PKey(bool fast) const
{
	PKey key;
	char buffer[512];

	/*
	**	When retrieving the fast key, the exponent is a known constant. Don't parse the
	**	exponent from the database.
	*/
	if (fast) {
		BigInt exp = PKey::Fast_Exponent();
		exp.DEREncode((unsigned char *)buffer);
		key.Decode_Exponent(buffer);
	} else {
		Get_UUBlock("PrivateKey", buffer, sizeof(buffer));
		key.Decode_Exponent(buffer);
	}

	Get_UUBlock("PublicKey", buffer, sizeof(buffer));
	key.Decode_Modulus(buffer);

	return(key);
}


/***********************************************************************************************
 * INIClass::Get_Fixed -- Fetch a fixed point number from the section & entry.                 *
 *                                                                                             *
 *    This routine will examine the section and entry specified and interpret the value        *
 *    as if it were a fixed point number. The format of the fixed point number can be          *
 *    percentage (e.g. 100%) or a floating point number (e.g., 1.7).                           *
 *                                                                                             *
 * INPUT:   section  -- Pointer to the section identifier to look under.                       *
 *                                                                                             *
 *          entry    -- Pointer to the entry identifier to examine.                            *
 *                                                                                             *
 *          defvalue -- If the section and entry could not be found, then this value will      *
 *                      be returned.                                                           *
 *                                                                                             *
 * OUTPUT:  Returns with the fixed point number that occurs at the section and entry           *
 *          specified. If it could not be found, then the default value is returned.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
fixed INIClass::Get_Fixed(char const * section, char const * entry, fixed defvalue) const
{
	char buffer[128];
	fixed retval = defvalue;

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		retval = fixed(buffer);
	}
	return(retval);
}


/***********************************************************************************************
 * INIClass::Put_Fixed -- Store a fixed point number to the INI database.                      *
 *                                                                                             *
 *    Use this routine to output a fixed point number to the database. The entry will be       *
 *    placed in the section and entry specified. If there was any existing entry, it will      *
 *    be replaced.                                                                             *
 *                                                                                             *
 * INPUT:   section  -- Pointer to the section identifier.                                     *
 *                                                                                             *
 *          entry    -- Pointer to the entry identifier to use for this value.                 *
 *                                                                                             *
 *          value    -- The value to store in the database.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the data stored?                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool INIClass::Put_Fixed(char const * section, char const * entry, fixed value)
{
	return(Put_String(section, entry, value.As_ASCII()));
}


/***********************************************************************************************
 * INIClass::Strip_Comments -- Strips comments of the specified text line.                     *
 *                                                                                             *
 *    This routine will scan the string (text line) supplied and if any comment portions are   *
 *    found, they will be trimmed off. Leading and trailing blanks are also removed.           *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the null terminate string to be processed.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void INIClass::Strip_Comments(char * buffer)
{
	if (buffer != NULL) {
		char * comment = strchr(buffer, ';');
		if (comment) {
			*comment = '\0';
			strtrim(buffer);
		}
	}
}
