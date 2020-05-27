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

/* $Header: /CounterStrike/PROFILE.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : PROFILE.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : September 10, 1993   [JLB]                                   *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   WWGetPrivateProfileInt -- Fetches integer value from INI.                                 *
 *   WWGetPrivateProfileString -- Fetch string from INI.                                       *
 *   WWWritePrivateProfileInt -- Write a profile int to the profile data block.                *
 *   WWWritePrivateProfileString -- Write a string to the profile data block.                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


static char * WriteBinBuffer = NULL;
static int WriteBinBufferLen = 0;
static int WriteBinBufferPos = 0;
static int WriteBinBufferMax = 0;
static char * ReadBinBuffer = NULL;
static int ReadBinBufferLen = 0;
static int ReadBinBufferPos = 0;
static int ReadBinBufferMax = 0;


/***************************************************************************
 * Read_Private_Config_Struct -- Fetches override integer value.           *
 *                                                                         *
 * INPUT:                                                                  *
 * OUTPUT:                                                                 *
 * WARNINGS:                                                               *
 * HISTORY:                                                                *
 *   08/05/1992 JLB : Created.                                             *
 *=========================================================================*/
bool Read_Private_Config_Struct(FileClass & file, NewConfigType * config)
{
	INIClass ini;
	ini.Load(file);

	config->DigitCard 	= ini.Get_Hex("Sound", "Card", 0);
	config->IRQ 			= ini.Get_Int("Sound", "IRQ", 0);
	config->DMA 			= ini.Get_Int("Sound", "DMA", 0);
	config->Port 			= ini.Get_Hex("Sound", "Port", 0);
	config->BitsPerSample= ini.Get_Int("Sound", "BitsPerSample", 0);
	config->Channels 		= ini.Get_Int("Sound", "Channels", 0);
	config->Reverse      = ini.Get_Int("Sound", "Reverse", 0);
	config->Speed        = ini.Get_Int("Sound", "Speed", 0);
	ini.Get_String("Language", "Language", NULL, config->Language, sizeof(config->Language));


//	config->DigitCard 	= WWGetPrivateProfileHex("Sound", "Card", 			 profile);
//	config->IRQ 			= WWGetPrivateProfileInt("Sound", "IRQ", 			 0,profile);
//	config->DMA 			= WWGetPrivateProfileInt("Sound", "DMA", 			 0,profile);
//	config->Port 			= WWGetPrivateProfileHex("Sound", "Port", 			 profile);
//	config->BitsPerSample= WWGetPrivateProfileInt("Sound", "BitsPerSample",0,profile);
//	config->Channels 		= WWGetPrivateProfileInt("Sound", "Channels",		 0,profile);
//	config->Reverse      = WWGetPrivateProfileInt("Sound", "Reverse", 		 0,profile);
//	config->Speed        = WWGetPrivateProfileInt("Sound", "Speed", 		 0,profile);
//	WWGetPrivateProfileString("Language", "Language", NULL, config->Language, 3, profile);

	return((config->DigitCard == 0) && (config->IRQ == 0) && (config->DMA == 0));
}


/***************************************************************************
 * Get_Private_Profile_Hex -- Fetches override integer value.              *
 *                                                                         *
 * INPUT:                                                                  *
 * OUTPUT:                                                                 *
 * WARNINGS:                                                               *
 * HISTORY:                                                                *
 *   08/05/1992 MML : Created.                                             *
 *=========================================================================*/
unsigned WWGetPrivateProfileHex (char const * section, char const * entry, char * profile)
{
	char	buffer[16];					// Integer staging buffer.
	unsigned card;

	memset (buffer, '0', sizeof(buffer));		// MAX_ENTRY_SIZE = 15
	buffer[sizeof(buffer)-1] = '\0';

	WWGetPrivateProfileString(section, entry, "0", buffer, sizeof(buffer), profile);

	if (strlen (buffer) > 0) {
		sscanf (buffer, "%x", &card);
	} else {
		card = 0;
	}

	return(card);
}


/***********************************************************************************************
 * WWGetPrivateProfileInt -- Fetches integer value.                                            *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      section      section to read from                                                      *
 *                                                                                             *
 *      entry         name of entry to read                                                    *
 *                                                                                             *
 *      def         default value, if entry isn't found                                        *
 *                                                                                             *
 *      profile      buffer containing INI data                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      integer requested                                                                      *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
int WWGetPrivateProfileInt(char const * section, char const * entry, int def, char * profile)
{
	char	buffer[16];			// Integer staging buffer.

	/*
	**	Store the default in the buffer.
	*/
	sprintf(buffer, "%d", def);

	/*
	**	Get the buffer; use itself as the default.
	*/
	WWGetPrivateProfileString(section, entry, buffer, buffer, sizeof(buffer)-1, profile);

	/*
	**	Convert to int & return.
	*/
	return(atoi(buffer));
}


/***********************************************************************************************
 * WWWritePrivateProfileInt -- Write a profile int to the profile data block.                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      section      section name to write to                                                  *
 *                                                                                             *
 *      entry         name of entry to write; if NULL, the entire section is deleted           *
 *                                                                                             *
 *      value         value to write                                                           *
 *                                                                                             *
 *      profile      INI buffer                                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      true = success, false = failure                                                        *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
bool WWWritePrivateProfileInt(char const * section, char const * entry, int value, char * profile)
{
	char	buffer[250];			// Working section buffer.

	/*
	**	Just return if nothing to do.
	*/
	if (!profile || !section) {
		return(true);
	}

	/*
	**	Generate string to save.
	*/
	sprintf(buffer, "%d", value);

	/*
	**	Save the string.
	*/
	return(WWWritePrivateProfileString(section, entry, buffer, profile));
}


/***********************************************************************************************
 * WWGetPrivateProfileString -- Fetch game override string.                                    *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      section      section name to read from                                                 *
 *                                                                                             *
 *      entry         name of entry to read; if NULL, all entry names are returned             *
 *                                                                                             *
 *      def         default string to use if not found; can be NULL                            *
 *                                                                                             *
 *      retbuffer   buffer to store result in                                                  *
 *                                                                                             *
 *      retlen      max length of return buffer                                                *
 *                                                                                             *
 *      profile      INI buffer                                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      ptr to entry found in INI buf; NULL if not found                                       *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      On the PC, the "\n" (10) is translated to "\r\n" (13,10) when it's written             *
 *      to disk. This routine must take this into consideration, by searching                  *
 *      for \n when scanning backward, and for \r when scanning forward.                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
char * WWGetPrivateProfileString(char const * section, char const * entry, char const * def, char * retbuffer, int retlen, char const * profile)
{
	char const * workptr;		// Working pointer into profile block.
	char * altworkptr;	// Alternate work pointer.
	char	sec[50];			// Working section buffer.
	char	const * retval;			// Start of section or entry pointer.
	char	* next;			// Pointer to start of next section (or EOF).
	char	c,c2;				// Working character values.
	int	len;				// Working substring length value.
	int	entrylen;		// Byte length of specified entry.
	char * orig_retbuf;	// original retbuffer ptr

//	if (!retlen) return(NULL);

	/*
	**	Fill in the default value just in case the entry could not be found.
	*/
	if (retbuffer) {
		retbuffer[0] = '\0';
		if (retlen > 1 || retlen == 0) retbuffer[1] = '\0';
		if (def) {
			strncpy(retbuffer, def, retlen);
		}
		retbuffer[retlen-1] = '\0';
		orig_retbuf = retbuffer;
	}

	/*
	**	Make sure a profile string was passed in
	*/
	if (!profile || !section) {
		return(retbuffer);
	}

	/*
	**	Build section string to match file image.
	*/
	sprintf(sec, "[%s]", section);	// sec = section name including []'s
	strupr(sec);
	len = strlen(sec);					// len = section name length, incl []'s

	/*
	**	Scan for a matching section
	*/
	retval = profile;
	workptr = profile;
	for (;;) {

		/*
		**	'workptr' = start of next section
		*/
		workptr = strchr(workptr, '[');

		/*
		**	If the end has been reached without finding the desired section
		**	then abort with a failure flag.
		*/
		if (!workptr) {
			return(NULL);
		}

		/*
		**	'c' = character just before the '['
		*/
		if (workptr==profile) {
			c = '\n';
		} else {
			c = *(workptr-1);
		}

		/*
		**	If this is the section name & the character before is a newline,
		**	process this section
		*/
		if (memicmp(workptr, sec, len) == 0 && (c == '\n')) {

			/*
			**	Skip work pointer to start of first valid entry.
			*/
			workptr += len;
			while (isspace(*workptr)) {
				workptr++;
			}

			/*
			**	If the section name is empty, we will have stepped onto the start
			**	of the next section name; inserting new entries here will leave
			**	a blank line between this section's name & 1st entry. So, check
			**	for 2 newlines in a row & step backward.
			*/
			if (workptr - profile > 4) {
				if ( *(workptr-1)=='\n' && *(workptr-3)=='\n')
					workptr -= 2;
			}

			/*
			**	'next = end of section or end of file.
			*/
			next = (char*)strchr(workptr, '[');
			for (;;) {
				if (next) {

					c = *(next-1);

					/*
					**	If character before '[' is newline, this is the start of the
					**	next section
					*/
					if (c == '\n') {
						if (*(next-1)=='\n' && *(next-3)=='\n') {
							next -= 2;
						}
						break;
					}

					/*
					**	This bracket was in the section; keep looking
					*/
					next = strchr(next+1, '[');
				} else {

					/*
					**	No bracket found; set 'next' to the end of the file
					*/
					next = (char*)workptr + strlen(workptr)-1;
					break;
				}
			}

			/*
			**	If a specific entry was specified then return with the associated
			**	string.
			*/
			if (entry) {
				retval = workptr;
				entrylen = strlen(entry);

				for (;;) {
					/*
					** Search for the 1st character of the entry
					*/
					workptr = strchr(workptr, *entry);

					/*
					**	If the end of the file has been reached or we have spilled
					**	into the next section, then abort
					*/
					if (!workptr || workptr >= next) {
						return(NULL);
					}

					/*
					**	'c' = character before possible entry; must be a newline
					**	'c2' = character after possible entry; must be '=' or space
					*/
					c = *(workptr-1);
					c2 = *(workptr+entrylen);

					/*
					**	Entry found; extract it
					*/
					if (memicmp(workptr, entry, entrylen) == 0 && (c == '\n') &&
						(c2 == '=' || isspace(c2))) {
						retval = workptr;
						workptr += entrylen;							// skip entry name
						workptr = strchr(workptr, '=');			// find '='

						/*
						** 'altworkptr' = next newline; \r is used here since we're
						** scanning forward!
						*/
						if (workptr) {
							altworkptr = (char*)strchr(workptr, '\r');	// find next newline
						}

						/*
						**	Return if there was no '=', or if the newline is before
						**	the next '='
						*/
						if (workptr == NULL || altworkptr < workptr) {
							return((char *)retval);
						}

						/*
						**	Skip any white space after the '=' and before the first
						**	valid character of the parameter.
						*/
						workptr++;										// Skip the '='.
						while (isspace(*workptr)) {

							/*
							**	Just return if there's no entry past the '='.
							*/
							if (workptr >= altworkptr)
								return((char*)retval);

							workptr++;									// Skip the whitespace
						}

						/*
						**	Copy the entry into the return buffer.
						*/
						len = (int)(altworkptr - workptr);
						if (len > retlen-1) {
							len = retlen-1;
						}

						if (retbuffer) {
							memcpy(retbuffer, workptr, len);
							*(retbuffer + len) = '\0';		// Insert trailing null.
							strtrim(retbuffer);
						}
						return((char*)retval);
					}

					/*
					**	Entry was not found; go to the next one
					*/
					workptr++;
				}
			} else {

				/*
				**	No entry was specified, so build a list of all entries.
				**	'workptr' is at 1st entry after section name
				**	'next' is next bracket, or end of file
				*/
				retval = workptr;

				if (retbuffer) {

					/*
					**	Keep accumulating the identifier strings in the retbuffer.
					*/
					while (workptr && workptr < next) {
						altworkptr = (char*)strchr(workptr, '=');		// find '='

						if (altworkptr && altworkptr < next) {
							int	length;								// Length of ID string.

							length = (int)(altworkptr - workptr);

							/*
							**	Make sure we don't write past the end of the retbuffer;
							**	add '3' for the 3 NULL's at the end
							*/
							if (retbuffer - orig_retbuf + length + 3 < retlen) {
								memcpy(retbuffer, workptr, length);	// copy entry name
								*(retbuffer+length) = '\0';			// NULL-terminate it
								strtrim(retbuffer);						// trim spaces
								retbuffer += strlen(retbuffer)+1;	// next pos in dest buf
							} else {
								break;
							}

							/*
							**	Advance the work pointer to the start of the next line
							**	by skipping the end of line character.
							*/
							workptr = strchr(altworkptr, '\n');
							if (!workptr) {
								break;
							}
							workptr++;
						} else {

							/*
							**	If no '=', break out of loop
							*/
							break;
						}
					}

					/*
					**	Final trailing terminator. Make double sure the double
					**	trailing null is added.
					*/
					*retbuffer++ = '\0';
					*retbuffer++ = '\0';
				}
				break;
			}
		} else {

			/*
			**	Section name not found; go to the next bracket & try again
			**	Advance past '[' and keep scanning.
			*/
			workptr++;
		}
	}

	return((char*)retval);
}


/***********************************************************************************************
 * WritePrivateProfileString -- Write a string to the profile data block.                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      section      section name to write to                                                  *
 *      entry         name of entry to write; if NULL, the section is deleted                  *
 *      string      string to write; if NULL, the entry is deleted                             *
 *      profile      INI buffer                                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      true = success, false = failure                                                        *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      This function has to translate newlines into \r\n sequences.                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
bool WWWritePrivateProfileString(char const * section, char const * entry, char const * string, char * profile)
{
	char	buffer[250];			// Working section buffer
	char const * offset;
	char const * next;						// ptr to next section
	char	c;							// Working character value

	/*
	**	Just return if nothing to do.
	*/
	if (!profile || !section) {
		return(true);
	}

	/*
	**	Try to find the section. WWGetPrivateProfileString with NULL entry name
	**	will return all entry names in the given buffer, truncated to the given
	**	buffer length. 'offset' will point to 1st entry in the section, NULL if
	**	section not found.
	*/
	offset = WWGetPrivateProfileString(section, NULL, NULL, NULL, 0, profile);

	/*
	**	If the section could not be found, then add it to the end. Don't add
	**	anything if a removal of an entry is requested (it is obviously already
	**	non-existent). Make sure two newlines precede the section name.
	*/
	if (!offset && entry) {
		sprintf(buffer, "\r\n[%s]\r\n", section);
		strcat(profile, buffer);
	}

	/*
	**	If the section is there and 'entry' is NULL, remove the entire section
	*/
	if (offset && !entry) {

		/*
		**	'next = end of section or end of file.
		*/
		next = strchr(offset, '[');
		for (;;) {
			if (next) {
				c = *(next-1);

				/*
				**	If character before '[' is newline, this is the start of the
				**	next section
				*/
				if (c == '\n') {
					if ( *(next-1)=='\n' && *(next-3)=='\n') {
						next -= 2;
					}
					break;
				}

				/*
				**	This bracket was in the section; keep looking
				*/
				next = strchr(next+1, '[');
			} else {

				/*
				**	No bracket found; set 'next' to the end of the file
				*/
				next = offset + strlen(offset);
				break;
			}
		}

		/*
		**	Remove the section
		*/
		strcpy((char*)offset, (char*)next);

		return(true);
	}

	/*
	**	Find the matching entry within the desired section. A NULL return buffer
	**	with 0 length will just return the offset of the found entry, NULL if
	**	entry not found.
	*/
	offset = WWGetPrivateProfileString(section, entry, NULL, NULL, 0, profile);

	/*
	**	Remove any existing entry
	*/
	if (offset) {
		int	eol;			// Working EOL offset.

		/*
		**	Get # characters up to newline; \n is used since we're after the end
		**	of this line
		*/
		eol = strcspn(offset, "\n");

		/*
		**	Erase the entry by strcpy'ing the entire INI file over this entry
		*/
		if (eol) {
			strcpy((char*)offset, offset + eol + 1);
		}
	} else {

		/*
		**	Entry doesn't exist, so point 'offset' to the 1st entry position in
		**	the section.
		*/
		offset = WWGetPrivateProfileString(section, NULL, NULL, NULL, 0, profile);
	}

	/*
	**	Add the desired entry.
	*/
	if (entry && string) {

		/*
		**	Generate entry string.
		*/
		sprintf(buffer, "%s=%s\r\n", entry, string);

		/*
		**	Make room for new entry.
		*/
		memmove((char*)offset+strlen(buffer), offset, strlen(offset)+1);

		/*
		**	Copy the entry into the INI buffer.
		*/
		memcpy((char*)offset, buffer, strlen(buffer));
	}

	return(true);
}


char * Read_Bin_Buffer( void )
{
	return( ReadBinBuffer );
}


bool Read_Bin_Init( char * buffer, int length )
{
	ReadBinBuffer = buffer;
	ReadBinBufferLen = length;
	ReadBinBufferPos = 0;
	ReadBinBufferMax = 0;
	return( true );
}


int Read_Bin_Length( char * buffer )
{
	if (buffer != ReadBinBuffer) {
		return( -1 );
	} else {
		return( ReadBinBufferMax );
	}
}


bool Read_Bin_Num( void * num, int length, char * buffer )
{
	char * ptr;

	if (buffer != ReadBinBuffer || length <= 0 || length > 4 ||
		 (ReadBinBufferPos + length) >= ReadBinBufferLen) {
		return( false );
	} else {
		ptr = ReadBinBuffer + ReadBinBufferPos;
		memcpy( num, ptr, length );
		ReadBinBufferPos += length;

		if (ReadBinBufferPos > ReadBinBufferMax) {
			ReadBinBufferMax = ReadBinBufferPos;
		}

		return( true );
	}
}


int Read_Bin_Pos( char * buffer )
{
	if (buffer != ReadBinBuffer) {
		return( -1 );
	} else {
		return( ReadBinBufferPos );
	}
}


int Read_Bin_PosSet( unsigned int pos, char * buffer )
{
	if (buffer != ReadBinBuffer) {
		return( -1 );
	} else {
		ReadBinBufferPos = pos;
		return( ReadBinBufferPos );
	}
}


bool Read_Bin_String( char * string, char * buffer )
{
	char * ptr;
	unsigned char length;

	if (buffer != ReadBinBuffer ||
		 ReadBinBufferPos >= ReadBinBufferLen) {
		return( false );
	} else {
		ptr = ReadBinBuffer + ReadBinBufferPos;
		length = (unsigned char)*ptr++;
		if ( (ReadBinBufferPos + length + 2) <= ReadBinBufferLen) {
			memcpy( string, ptr, (unsigned int)(length + 1) );
			ReadBinBufferPos += (length + 2);

			if (ReadBinBufferPos > ReadBinBufferMax) {
				ReadBinBufferMax = ReadBinBufferPos;
			}

			return( true );
		} else {
			return( false );
		}
	}
}


char * Write_Bin_Buffer( void )
{
	return( WriteBinBuffer );
}


bool Write_Bin_Init( char * buffer, int length )
{
	WriteBinBuffer = buffer;
	WriteBinBufferLen = length;
	WriteBinBufferPos = 0;
	WriteBinBufferMax = 0;
	return( true );
}


int Write_Bin_Length( char * buffer )
{
	if (buffer != WriteBinBuffer) {
		return( -1 );
	} else {
		return( WriteBinBufferMax );
	}
}


bool Write_Bin_Num( void * num, int length, char * buffer )
{
	char * ptr;

	if (buffer != WriteBinBuffer || length <= 0 || length > 4 ||
		 (WriteBinBufferPos + length) > WriteBinBufferLen) {
		return( false );
	} else {
		ptr = WriteBinBuffer + WriteBinBufferPos;
		memcpy( ptr, num, length );
		WriteBinBufferPos += length;

		if (WriteBinBufferPos > WriteBinBufferMax) {
			WriteBinBufferMax = WriteBinBufferPos;
		}

		return( true );
	}
}


int Write_Bin_Pos( char * buffer )
{
	if (buffer != WriteBinBuffer) {
		return( -1 );
	} else {
		return( WriteBinBufferPos );
	}
}


int Write_Bin_PosSet( unsigned int pos, char * buffer )
{
	if (buffer != WriteBinBuffer) {
		return( -1 );
	} else {
		WriteBinBufferPos = pos;
		return( WriteBinBufferPos );
	}
}


bool Write_Bin_String( char * string, int length, char * buffer )
{
	char * ptr;

	if (buffer != WriteBinBuffer || length < 0 || length > 255 ||
		 (WriteBinBufferPos + length + 2) > WriteBinBufferLen) {
		return( false );
	} else {
		ptr = WriteBinBuffer + WriteBinBufferPos;
		*ptr++ = (char)length;
		memcpy( ptr, string, (length + 1) );
		WriteBinBufferPos += (length + 2);

		if (WriteBinBufferPos > WriteBinBufferMax) {
			WriteBinBufferMax = WriteBinBufferPos;
		}

		return( true );
	}
}

