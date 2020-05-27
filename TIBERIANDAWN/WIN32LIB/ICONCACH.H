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

/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Drawbuff - Westwood win95 library                            *
 *                                                                                             *
 *                    File Name : Iconcach.H                                                   *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : November 8th, 1995                                           *
 *                                                                                             *
 *                  Last Update : November 16th, 1995 [ST]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview: This file cantains definition of the IconCacheClass and associated non member     *
 *           function prototypes.                                                              *
 *                                                                                             *
 * Functions:                                                                                  *
 *  IconCacheClass::Get_Is_Cached -- member to allow access to private IsCached flag           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */




#ifndef	ICONCACH_H
#define	ICONCACH_H

#include	<tile.h>

#define	ICON_WIDTH			24			// Icons must be this width to be cached
#define	ICON_HEIGHT			24			// Icons must be this height to be cached
#define	MAX_CACHED_ICONS	500		// Maximum number of icons that can be cached
#define	MAX_ICON_SETS		100		// Maximum number of icon sets that can be registered
#define	MAX_LOOKUP_ENTRIES 3000		// Size of icon index table


/*
** IconCacheClass for tracking individual icons cached into video memory
**
** Use Register_Icon_Set to identify a set of icons as cachable. Once registered, the icons
** will be cached automatically when drawn.
** Use Invalidate_Cached_Icons at the end of icon drawing to release the video memory used by the
** caching system.
** Restore_Cached_Icons may be used to reload the icons into video memory after a focus loss.
**
*/

class IconCacheClass {

	public:

		IconCacheClass (void);					// class constructor
		~IconCacheClass (void);					// class destructor

		void Restore(void);						// restore the surface
		BOOL Cache_It (void * icon_ptr);		// Cache the icon to video memory
		void Uncache_It (void);					// Restore the video memory and flag the icon as uncached
		void Draw_It (LPDIRECTDRAWSURFACE dest_surface , int x_pixel, int y_pixel, int window_left , int window_top , int window_width , int window_height);
		inline BOOL Get_Is_Cached(void);		// Return the IsCached member

		int						TimesDrawn;		// counter of times cached icon has been drawn
		int						TimesFailed;	// counter of times cached icon has failed to draw


	private:

		LPDIRECTDRAWSURFACE	CacheSurface;	// Ptr to direct draw surface where icon resides
		BOOL						IsCached;		// Flag to say whether an icon is cached
		BOOL						SurfaceLost;	// Flag to indicate that our icons surface has been lost
		int						DrawFrequency;	// Number of times icon has been drawn
		void						*IconSource;	// Ptr to original icon data in system memory

};



/*
** Structure to keep track of registered icon sets
**
*/

typedef struct tIconSetType{
	IControl_Type	*IconSetPtr;				// Ptr to icon set data
	int				IconListOffset;			// Offset into icon index table for this icon set
}IconSetType;


extern	IconCacheClass	CachedIcons[MAX_CACHED_ICONS];

extern void Invalidate_Cached_Icons (void);
extern void Restore_Cached_Icons (void);
extern void Register_Icon_Set (void *icon_data , BOOL pre_cache);

//
// Prototypes for assembly language procedures in STMPCACH.ASM
//
extern "C" void Clear_Icon_Pointers (void);
extern "C" void Cache_Copy_Icon (void const *icon_ptr ,void * , int);
extern "C" int Is_Icon_Cached (void const *icon_data , int icon);
extern "C" int Get_Icon_Index (void *icon_ptr);
extern "C" int Get_Free_Index (void);
extern "C" BOOL Cache_New_Icon (int icon_index, void *icon_ptr);
extern "C" int Get_Free_Cache_Slot(void);


extern	int	CachedIconsDrawn;
extern	int	UnCachedIconsDrawn;


/***********************************************************************************************
 * ICC::Get_Is_Cached -- member to allow access to the private IsCached flag                   *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   IsCached                                                                          *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/13/95 9:42AM ST : Created                                                             *
 *=============================================================================================*/
inline BOOL IconCacheClass::Get_Is_Cached (void)
{
	return (IsCached);
}





#endif	//ICONCACH_H

