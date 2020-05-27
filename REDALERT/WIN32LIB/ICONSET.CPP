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
 **   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Library                                  *
 *                                                                         *
 *                    File Name : ICONSET.C                                *
 *                                                                         *
 *                   Programmer : Joe L. Bostic                            *
 *                                                                         *
 *                   Start Date : June 9, 1991                             *
 *                                                                         *
 *                  Last Update : September 15, 1993   [JLB]               *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Load_Icon_Set -- Loads an icons set and initializes it.               *
 *   Free_Icon_Set -- Frees allocations made by Load_Icon_Set().           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

//#include	"function.h"

//#define		_WIN32
//#define		WIN32_LEAN_AND_MEAN

//#include	<dos.h>
#include	<stdlib.h>
#include	<stdio.h>
//#include	<mem.h>
#include	<wwstd.h>
#include	<file.h>
#include	"tile.h"
#include	<iff.h>


// Misc? ST - 1/3/2019 10:40AM
//extern int Misc;
int Misc;


void *  Load_Icon_Set(char const *filename, void *iconsetptr, long buffsize);
void  Free_Icon_Set(void const *iconset);
long  Get_Icon_Set_Size(void const *iconset);
int  Get_Icon_Set_Width(void const *iconset);
int  Get_Icon_Set_Height(void const *iconset);
void *  Get_Icon_Set_Icondata(void const *iconset);
void *  Get_Icon_Set_Trans(void const *iconset);
void *  Get_Icon_Set_Remapdata(void const *iconset);
void *  Get_Icon_Set_Palettedata(void const *iconset);
int  Get_Icon_Set_Count(void const *iconset);
void *  Get_Icon_Set_Map(void const *iconset);


//#define	ICON_PALETTE_BYTES	16
//#define	ICON_MAX					256

/***************************************************************************
**	The terrain is rendered by using icons.  These are the buffers that hold
**	the icon data, remap tables, and remap index arrays.
*/
//PRIVATE char *IconPalette = NULL;		// MCGA only.
//PRIVATE char *IconRemap = NULL;			// MCGA only.

#define	FORM_RPAL	MAKE_ID('R','P','A','L')
#define	FORM_RTBL	MAKE_ID('R','T','B','L')
#define	FORM_SSET	MAKE_ID('S','S','E','T')
#define	FORM_SINF	MAKE_ID('S','I','N','F')
#define	FORM_ICON	MAKE_ID('I','C','O','N')
#define	FORM_TRNS	MAKE_ID('T','R','N','S')
#define	FORM_MAP		MAKE_ID('M','A','P',' ')



/***************************************************************************
 * LOAD_ICON_SET -- Loads an icons set and initializes it.                 *
 *                                                                         *
 *    This routine will load an IFF icon set from disk.  It handles all    *
 *    of the necessary allocations.                                        *
 *                                                                         *
 * INPUT:   filename -- Name of the icon file.                             *
 *                                                                         *
 *          buffer   -- Pointer to paragraph aligned buffer to hold data.  *
 *                                                                         *
 *          size     -- Size of the buffer (in bytes).                     *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * WARNINGS:   In EEGA mode the iconset buffer will be free because the    *
 *             icons will have been transferred to card ram.               *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/21/1991 JLB : Created.                                             *
 *   07/01/1991 JLB : Determines icon size from file.                      *
 *   07/15/1991 JLB : Load and uncompress onto the same buffer.            *
 *   09/15/1993 JLB : Added EMS support.                                   *
 *=========================================================================*/
void *  Load_Icon_Set(char const *filename, void *iconsetptr, long buffsize)
{
	int	fh;				// File handle of iconset.
	int	bytespericon;	// The number of bytes per icon.
	unsigned long	icons=0;			// Number of icons loaded.
	unsigned long	size;				// Size of the icon chunk (raw).

	unsigned long	transsize;
	void	*transptr=NULL;

	unsigned long	mapsize;				// Icon map chunk size.
	void	*mapptr=NULL;		// Icon map pointer.
	void	*returnptr=NULL;	// Iconset pointer returned by routine.
	BOOL	allocated=FALSE;	// Was the iconset block allocated?
	IControl_Type	*idata=NULL;	// Icon data loaded.
	long	id;				// ID of file openned.
	struct {
		char	Width;		// Width of icon in bytes.
		char	Height;		// Height of icon in bytes.
		char	Format;		// Graphic mode.
								//lint -esym(754,Format)
		char	Bitplanes;	// Number of bitplanes per icon.
	} sinf;

	/*
	**	Open the icon set for loading.  If it is not a legal icon set
	**	data file, then abort.
	*/
	fh = Open_Iff_File(filename);
	if (fh != WW_ERROR) {
		Read_File(fh, &id, sizeof(long));
		if (id == FORM_ICON) {

			/*
			**	Determine the size of the icons and set up the graphic
			**	system accordingly.  Also get the sizes of the various
			**	data blocks that have to be loaded.
			*/
			Read_Iff_Chunk(fh, FORM_SINF, &sinf, sizeof(sinf));
			bytespericon = ((((int)sinf.Width)<<3)*(((int)sinf.Height)<<3)*(int)sinf.Bitplanes)>>3;

			size = Get_Iff_Chunk_Size(fh,FORM_SSET);
				transsize = Get_Iff_Chunk_Size(fh, FORM_TRNS);
			mapsize = Get_Iff_Chunk_Size(fh, FORM_MAP);

			/*
			**	Allocate the icon buffer if one isn't provided.  First try EMS and
			**	then try conventional RAM.
			*/
			allocated = FALSE;
			if (!iconsetptr) {
					buffsize = size + transsize + mapsize + sizeof(IControl_Type);

				Misc = buffsize;
				iconsetptr = Alloc(buffsize, MEM_NORMAL);
				allocated = (iconsetptr != NULL);
			}

			if (iconsetptr && (size+transsize+mapsize+sizeof(IControl_Type)) <= (unsigned long)buffsize) {

				idata = (IControl_Type *)iconsetptr;

				memset(idata, 0, sizeof(IControl_Type));

				/*
				**	Initialize the iconset header structure.
				*/
				idata->Width = (short)(((short)sinf.Width)<<3);
				idata->Height = (short)(((short)sinf.Height)<<3);
				idata->Allocated = (short)allocated;
				idata->Icons = (long)iconsetptr + sizeof(IControl_Type);
				idata->Map = idata->Icons + size;
					idata->TransFlag = sizeof(IControl_Type) + size + mapsize;
				idata->Size = buffsize;

					{
						long	val;

						val = Read_Iff_Chunk(fh, FORM_SSET, Add_Long_To_Pointer(iconsetptr, sizeof(IControl_Type)), size);
						icons = (int)(val/(long)bytespericon);
						idata = (IControl_Type *)iconsetptr;
					}

				if (mapsize) {
					icons = mapsize;
				}
				idata->Count = (short)icons;

				/*
				**	Limit buffer to only the size needed.  This is done AFTER loading of the
				**	raw icon data because it might have been compressed and thus need any
				**	extra space to perform an overlapped decompression.
				*/
					if ((unsigned long)buffsize > size + transsize + mapsize + sizeof(IControl_Type)) {
						buffsize = size + transsize + mapsize + sizeof(IControl_Type);
					}

					transptr = Add_Long_To_Pointer(iconsetptr, idata->TransFlag);
					Read_Iff_Chunk(fh, FORM_TRNS, transptr, transsize);
					idata = (IControl_Type *)iconsetptr;

				mapptr = (void*)idata->Map;
				Read_Iff_Chunk(fh, FORM_MAP, mapptr, mapsize);

				/*
				**	Let the graphic overlay know of the icon data.  This could involve
				**	translation and other data manipulations.
				*/
				//Init_Stamps(iconsetptr);

				returnptr = iconsetptr;
			}
		}
		Close_Iff_File(fh);
	}

	return (returnptr);		// Return with icon pointer.
}


/***************************************************************************
 * FREE_ICON_SET -- Frees allocations made by Load_Icon_Set().             *
 *                                                                         *
 *    This routine is used to free up any allocations by Load_Icon_Set().  *
 *    Use this routine when a new icon set is to be loaded.                *
 *                                                                         *
 * INPUT:   none                                                           *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/21/1991 JLB : Created.                                             *
 *=========================================================================*/
void  Free_Icon_Set(void const *iconset)
{
	IControl_Type	*icontrol;

	icontrol = (IControl_Type *)iconset;
	if (icontrol) {
		if (icontrol->Allocated) {
			Free((void *)iconset);
		}
	}
}


long  Get_Icon_Set_Size(void const *iconset)
{
	IControl_Type	*icontrol;
	long				size=0;

	icontrol = (IControl_Type *)iconset;
	if (icontrol) {
		size = icontrol->Size;
	}
	return(size);
}


int  Get_Icon_Set_Width(void const *iconset)
{
	IControl_Type	*icontrol;
	int				width=0;

	icontrol = (IControl_Type *)iconset;
	if (icontrol) {
		width = icontrol->Width;
	}
	return(width);
}


int  Get_Icon_Set_Height(void const *iconset)
{
	IControl_Type	*icontrol;
	int				height=0;

	icontrol = (IControl_Type *)iconset;
	if (icontrol) {
		height = icontrol->Height;
	}
	return(height);
}


void *  Get_Icon_Set_Icondata(void const *iconset)
{
	IControl_Type	*icontrol;
	icontrol = (IControl_Type *)iconset;
	if (icontrol)
		return(Add_Long_To_Pointer(iconset, (LONG)icontrol->Icons));
	return(NULL);
}

void *  Get_Icon_Set_Trans(void const *iconset)
{
	IControl_Type	*icontrol;
	void				*ptr=NULL;

	icontrol = (IControl_Type *)iconset;
	if (icontrol) {
		ptr = Add_Long_To_Pointer((void *)iconset, icontrol->TransFlag);
	}
	return(ptr);
}


int  Get_Icon_Set_Count(void const *iconset)
{
	IControl_Type	*icontrol;
	int				count;

	icontrol = (IControl_Type *)iconset;
	if (icontrol) {
		count = icontrol->Count;
	}
	return(count);
}


void *  Get_Icon_Set_Map(void const *iconset)
{
	IControl_Type	*icontrol;
	icontrol = (IControl_Type *)iconset;
	if (icontrol)
		return(Add_Long_To_Pointer(iconset, (LONG)icontrol->Map));
	return(NULL);
}
