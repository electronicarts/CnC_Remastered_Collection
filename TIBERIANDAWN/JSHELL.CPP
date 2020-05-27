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

/* $Header:   F:\projects\c&c\vcs\code\jshell.cpv   2.18   16 Oct 1995 16:51:12   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : JSHELL.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 2, 1994                                                *
 *                                                                                             *
 *                  Last Update : May 11, 1995 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Build_Translucent_Table -- Creates a translucent control table.                           *
 *   Translucent_Table_Size -- Determines the size of a translucent table.                     *
 *   Conquer_Build_Translucent_Table -- Builds fading table for shadow colors only.            *
 *   Load_Alloc_Data -- Allocates a buffer and loads the file into it.                         *
 *   Load_Uncompress -- Loads and uncompresses data to a buffer.                               *
 *   Fatal -- General purpose fatal error handler.                                             *
 *   Set_Window -- Sets the window dimensions to that specified.                               * 
 *   Small_Icon -- Create a small icon from a big one.                                         * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"wwfile.h"

#include <assert.h>

/*********************************************************************************************** 
 * Small_Icon -- Create a small icon from a big one.                                           * 
 *                                                                                             * 
 *    This routine will extract the specified icon from the icon data file and convert that    * 
 *    incon into a small (3x3) representation. Typicall use of this mini-icon is for the radar * 
 *    map.                                                                                     * 
 *                                                                                             * 
 * INPUT:   iconptr  -- Pointer to the icon data file.                                         * 
 *                                                                                             * 
 *          iconnum  -- The embedded icon number to convert into a small image.                * 
 *                                                                                             * 
 * OUTPUT:  Returns with a pointer to the small icon imagery. This is exactly 9 bytes long.    * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   05/11/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
void * Small_Icon(void const * iconptr, int iconnum)
{
	static unsigned char _icon[9];
	IControl_Type const * iptr = (IControl_Type const *)iconptr;
	unsigned char * data;

	if (iconptr) {
		iconnum = iptr->Map[iconnum];
		data = &iptr->Icons[iconnum*(24*24)];

		for (int index = 0; index < 9; index++) {
			int _offsets[9] = {
				4+4*24,
				12+4*24,
				20+4*24,
				4+12*24,
				12+12*24,
				20+12*24,
				4+20*24,
				12+20*24,
				20+20*24
			};
			_icon[index] = data[_offsets[index]];
		}
	}

	return(_icon);
}	


/*********************************************************************************************** 
 * Set_Window -- Sets the window dimensions to that specified.                                 * 
 *                                                                                             * 
 *    Use this routine to set the windows dimensions to the coordinates and dimensions         * 
 *    specified.                                                                               * 
 *                                                                                             * 
 * INPUT:   x     -- Window X pixel position.                                                  * 
 *                                                                                             * 
 *          y     -- Window Y pixel position.                                                  * 
 *                                                                                             * 
 *          w     -- Window width in pixels.                                                   * 
 *                                                                                             * 
 *          h     -- Window height in pixels.                                                  * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   The X and width values are truncated to an even 8 pixel boundary. This is       * 
 *             the same as stripping off the lower 3 bits.                                     * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/15/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
void Set_Window(int window, int x, int y, int w, int h)
{
	WindowList[window][WINDOWWIDTH] = w >> 3; 
	WindowList[window][WINDOWHEIGHT] = h;
	WindowList[window][WINDOWX] = x >> 3;
	WindowList[window][WINDOWY] = y;
}	


/***********************************************************************************************
 * Fatal -- General purpose fatal error handler.                                               *
 *                                                                                             *
 *    This is a very simple general purpose fatal error handler. It goes directly to text      *
 *    mode, prints the error, and then aborts with a failure code.                             *
 *                                                                                             *
 * INPUT:   message  -- The text message to display.                                           *
 *                                                                                             *
 *          ...      -- Any optional parameters that are used in formatting the message.       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine never returns. The game exits immediately.                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void Fatal(char const *message, ...)
{
	va_list	va;

	va_start(va, message);
	Prog_End();
	vfprintf(stderr, message, va);
	Mono_Printf(message);
	GlyphX_Debug_Print("Fatal");
	GlyphX_Debug_Print(message);

	if (!RunningAsDLL) {
		exit(EXIT_FAILURE);
	} else {
		*((int*)0) = 0;
	}	
}


#ifdef NEVER
void File_Fatal(char const *message)
{
	Prog_End();
	perror(message);
	if (!RunningAsDLL) {
		exit(EXIT_FAILURE);
	}
}
#endif



/***********************************************************************************************
 * Load_Uncompress -- Loads and uncompresses data to a buffer.                                 *
 *                                                                                             *
 *    This is the C++ counterpart to the Load_Uncompress function. It will load the file       *
 *    specified into the graphic buffer indicated and uncompress it.                           *
 *                                                                                             *
 * INPUT:   file     -- The file to load and uncompress.                                       *
 *                                                                                             *
 *          uncomp_buff -- The graphic buffer that initial loading will use.                   *
 *                                                                                             *
 *          dest_buff   -- The buffer that will hold the uncompressed data.                    *
 *                                                                                             *
 *          reserved_data  -- This is an optional pointer to a buffer that will hold any       *
 *                            reserved data the compressed file may contain. This is           *
 *                            typically a palette.                                             *
 *                                                                                             *
 * OUTPUT:  Returns with the size of the uncompressed data in the destination buffer.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
long Load_Uncompress(FileClass &file, BuffType &uncomp_buff, BuffType &dest_buff, void *reserved_data)
{
	unsigned short	size;
	void	*sptr = uncomp_buff.Get_Buffer();
	void	*dptr = dest_buff.Get_Buffer();
	int	opened = false;
	CompHeaderType	header;

	/*
	**	The file must be opened in order to be read from. If the file 
	**	isn't opened, then open it. Record this fact so that it can be
	**	restored to its closed state at the end.
	*/
	if (!file.Is_Open()) {
		if (!file.Open()) {
			return(0);
		}
		opened = true;
	}

	/*
	**	Read in the size of the file (supposedly).
	*/
	file.Read(&size, sizeof(size));

	/*
	**	Read in the header block. This block contains the compression type
	**	and skip data (among other things).
	*/
	file.Read(&header, sizeof(header));
	size -= sizeof(header);

	/*
	**	If there are skip bytes then they must be processed. Either read
	**	them into the buffer provided or skip past them. No check is made
	**	to ensure that the reserved data buffer is big enough (watch out!).
	*/
	if (header.Skip) {
		size -= header.Skip;
		if (reserved_data) {
			file.Read(reserved_data, header.Skip);
		} else {
			file.Seek(header.Skip, SEEK_CUR);
		}
		header.Skip = 0;
	}

	/*
	**	Determine where is the proper place to load the data. If both buffers
	**	specified are identical, then the data should be loaded at the end of
	**	the buffer and decompressed at the beginning.
	*/
	if (uncomp_buff.Get_Buffer() == dest_buff.Get_Buffer()) {
		sptr = Add_Long_To_Pointer(sptr, uncomp_buff.Get_Size()-(size+sizeof(header)));
	}

	/*
	**	Read in the bulk of the data.
	*/
	Mem_Copy(&header, sptr, sizeof(header));
	file.Read(Add_Long_To_Pointer(sptr, sizeof(header)), size);

	/*
	**	Decompress the data.
	*/
	size = (unsigned int) Uncompress_Data(sptr, dptr);

	/*
	**	Close the file if necessary.
	*/
	if (opened) {
		file.Close();
	}
	return((long)size);
}


/***********************************************************************************************
 * Load_Alloc_Data -- Allocates a buffer and loads the file into it.                           *
 *                                                                                             *
 *    This is the C++ replacement for the Load_Alloc_Data function. It will allocate the       *
 *    memory big enough to hold the file and then read the file into it.                       *
 *                                                                                             *
 * INPUT:   file  -- The file to read.                                                         *
 *                                                                                             *
 *          mem   -- The memory system to use for allocation.                                  *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the allocated and filled memory block.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void * Load_Alloc_Data(FileClass &file)
{
	void *ptr = 0;
	long size = file.Size();

	ptr = new char [size];
	if (ptr) {
		file.Read(ptr, size);
	}
	return(ptr);
}


/***********************************************************************************************
 * Translucent_Table_Size -- Determines the size of a translucent table.                       *
 *                                                                                             *
 *    Use this routine to determine how big the translucent table needs                        *
 *    to be given the specified number of colors. This value is typically                      *
 *    used when allocating the buffer for the translucent table.                               *
 *                                                                                             *
 * INPUT:   count -- The number of colors that are translucent.                                *
 *                                                                                             *
 * OUTPUT:  Returns the size of the translucent table.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
long Translucent_Table_Size(int count)
{
	return(256L + (256L * count));
}


/***********************************************************************************************
 * Build_Translucent_Table -- Creates a translucent control table.                             *
 *                                                                                             *
 *    The table created by this routine is used by Draw_Shape (GHOST) to                       *
 *    achieve a translucent affect. The original color of the shape will                       *
 *    show through. This differs from the fading effect, since that                            *
 *    affect only alters the background color toward a single destination                      *
 *    color.                                                                                   *
 *                                                                                             *
 * INPUT:   palette  -- Pointer to the control palette.                                        *
 *                                                                                             *
 *          control  -- Pointer to array of structures that control how                        *
 *                      the translucent table will be built.                                   *
 *                                                                                             *
 *          count    -- The number of entries in the control array.                            *
 *                                                                                             *
 *          buffer   -- Pointer to buffer to place the translucent table.                      *
 *                      If NULL is passed in, then the buffer will be                          *
 *                      allocated.                                                             *
 *                                                                                             *
 * OUTPUT:  Returns with pointer to the translucent table.                                     *
 *                                                                                             *
 * WARNINGS:   This routine is exceedingly slow. Use sparingly.                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void *Build_Translucent_Table(void const *palette, TLucentType const *control, int count, void *buffer)
{
	unsigned char	const *table;		// Remap table pointer.
	int			index;		// Working color index.

	if (count && control && palette) {
		if (!buffer) {
			buffer = new char [Translucent_Table_Size(count)];
		}

		if (buffer) {
			memset(buffer, -1, 256);
			table = (unsigned char*)Add_Long_To_Pointer((void*)buffer, 256);

			/*
			**	Build the individual remap tables for each translucent color.
			*/
			for (index = 0; index < count; index++) {
				((unsigned char*)buffer)[control[index].SourceColor] = index;
				Build_Fading_Table(palette, (void*)table, control[index].DestColor, control[index].Fading);
				table = (unsigned char*)Add_Long_To_Pointer((void*)table, 256);
			}
		}
	}
	return(buffer);
}


/***********************************************************************************************
 * Conquer_Build_Translucent_Table -- Builds fading table for shadow colors only.              *
 *                                                                                             *
 *    This routine will build a translucent (fading) table to remap colors into the shadow     *
 *    color region of the palette. Shadow colors are not affected by this translucent table.   *
 *    This means that a shape can be overlapped any number of times and the imagery will       *
 *    remain deterministic (and constant).                                                     *
 *                                                                                             *
 * INPUT:   palette  -- Pointer to the palette to base the translucent process on.             *
 *                                                                                             *
 *          control  -- Pointer to special control structure that specifies the                *
 *                      target color, and percentage of fade.                                  *
 *                                                                                             *
 *          count    -- The number of colors to be remapped (entries in the control array).    *
 *                                                                                             *
 *          buffer   -- Pointer to the staging buffer that will hold the translucent table     *
 *                      data. If this parameter is NULL, then an appropriate sized table       *
 *                      will be allocated.                                                     *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the translucent table data.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void *Conquer_Build_Translucent_Table(void const *palette, TLucentType const *control, int count, void *buffer)
{
	unsigned char	const *table;		// Remap table pointer.
	int			index;		// Working color index.

	if (count && control && palette) {
		if (!buffer) {
			buffer = new char [Translucent_Table_Size(count)];
		}

		if (buffer) {
			memset(buffer, -1, 256);
			table = (unsigned char*)Add_Long_To_Pointer((void*)buffer, 256);

			/*
			**	Build the individual remap tables for each translucent color.
			*/
			for (index = 0; index < count; index++) {
				((unsigned char*)buffer)[control[index].SourceColor] = index;
				Conquer_Build_Fading_Table(palette, (void*)table, control[index].DestColor, control[index].Fading);
				table = (unsigned char*)Add_Long_To_Pointer((void*)table, 256);
			}
		}
	}
	return(buffer);
}