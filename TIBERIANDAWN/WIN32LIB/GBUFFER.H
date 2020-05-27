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
 *                 Project Name : Westwood 32 Bit Library						*
 *                                                                         *
 *                    File Name : GBUFFER.H                                *
 *                                                                         *
 *                   Programmer : Phil W. Gorrow                           *
 *                                                                         *
 *                   Start Date : May 26, 1994                             *
 *                                                                         *
 *                  Last Update : October 9, 1995   []                     *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *	This module contains the definition for the graphic buffer class.  The	*
 * primary functionality of the graphic buffer class is handled by inline	*
 * functions that make a call through function pointers to the correct		*
 * routine.  This has two benefits:														*
 *																									*
 *																									*
 *		1) C++ name mangling is not a big deal since the function pointers	*
 *		   point to functions in standard C format.									*
 *		2) The function pointers can be changed when we set a different		*
 *       graphic mode.  This allows us to have both supervga and mcga		*
 *			routines present in memory at once.											*
 *																									*
 * In the basic library, these functions point to stub routines which just	*
 * return.  This makes a product that just uses a graphic buffer take the	*
 * minimum amount of code space.  For programs that require MCGA or VESA	*
 * support, all that is necessary to do is link either the MCGA or VESA		*
 * specific libraries in, previous to WWLIB32.  The linker will then 		*
 * overide the the necessary stub functions automatically.						*
 *																									*
 * In addition, there are helpful inline function calls for parameter		*
 * ellimination.  This header file gives the defintion for all					*
 * GraphicViewPort and GraphicBuffer classes.  										*
 *																									*
 * Terminology:																				*
 *																									*
 *	Buffer Class - A class which consists of a pointer to an allocated		*
 *		buffer and the size of the buffer that was allocated.						*
 *																									*
 *	Graphic ViewPort - The Graphic ViewPort defines a window into a 			*
 *		Graphic Buffer.  This means that although a Graphic Buffer 				*
 *		represents linear memory, this may not be true with a Graphic			*
 *		Viewport.  All low level functions that act directly on a graphic		*
 *		viewport are included within this class.  This includes but is not	*
 *		limited to most of the functions which can act on a Video Viewport	*
 *		Video Buffer.																			*
 *																									*
 * Graphic Buffer - A Graphic Buffer is an instance of an allocated buffer	*
 *		used to represent a rectangular region of graphics memory.				*
 *		The HidBuff	and BackBuff are excellent examples of a Graphic Buffer.	*
 *																									*
 * Below is a tree which shows the relationship of the VideoBuffer and 		*
 * Buffer classes to the GraphicBuffer class:										*
 *																									*
 *	  BUFFER.H				 GBUFFER.H			  BUFFER.H				 VBUFFER.H	*
 *  ----------          ----------         ----------          ----------	*
 * |  Buffer  |        | Graphic  |       |  Buffer  |        |  Video   |	*
 * |  Class   |        | ViewPort |       |  Class   |        | ViewPort |	*
 *  ----------          ----------         ----------          ----------	*
 *            \        /                             \        /				*
 *             \      /                               \      /					*
 *            ----------                             ----------				*
 *           |  Graphic |                           |  Video   |				*
 *           |  Buffer  |                           |  Buffer  |				*
 *            ----------                             ---------- 				*
 *	  			  GBUFFER.H			                       VBUFFER.H					*
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   GBC::GraphicBufferClass -- inline constructor for GraphicBufferClass  *
 *   GVPC::Remap -- Short form to remap an entire graphic view port        *
 *   GVPC::Get_XPos -- Returns x offset for a graphic viewport class       *
 *   GVPC::Get_Ypos -- Return y offset in a GraphicViewPortClass           *
 *   VVPC::Get_XPos -- Get the x pos of the VP on the Video                *
 *   VVPC::Get_YPos -- Get the y pos of the VP on the video                *
 *   GBC::Get_Graphic_Buffer -- Get the graphic buffer of the VP.          *
 *   GVPC::Draw_Line -- Stub function to draw line in Graphic Viewport Class*
 *   GVPC::Fill_Rect -- Stub function to fill rectangle in a GVPC          *
 *   GVPC::Remap -- Stub function to remap a GVPC                          *
 *   GVPC::Print -- stub func to print a text string                       *
 *   GVPC::Print -- Stub function to print an integer                      *
 *   GVPC::Print -- Stub function to print a short to a graphic viewport   *
 *   GVPC::Print -- stub function to print a long on a graphic view port   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef _WIN32 // Denzil 6/2/98 Watcom 11.0 complains without this check
#define _WIN32
#endif // _WIN32

#define WIN32_LEAN_AND_MEAN
#include <ddraw.h>


#ifndef GBUFFER_H
#define GBUFFER_H

/*=========================================================================*/
/* If we have not already loaded the standard library header, than we can	*/
/*		load it.																					*/
/*=========================================================================*/

#ifndef WWSTD_H
#include "wwstd.h"
#endif

#ifndef DRAWBUFF_H
#include "drawbuff.h"
#endif

//#ifndef BUFFER_H
#include "buffer.h"
//#endif

#ifndef WINDOWS_H
#include "ww_win.h"
#endif
#include <stdlib.h>

#include "iconcach.h"



//#ifndef FUNCTION_H

//#pragma off (unreferenced)

//#ifndef BITMAPCLASS
//#define BITMAPCLASS
class BitmapClass
{
	public:
		BitmapClass(int w, int h, unsigned char * data) :
			Width(w), Height(h), Data(data) {};

		int Width;
		int Height;
		unsigned char * Data;
};

class TPoint2D
{
	public:
		TPoint2D(int xx, int yy) : x(xx), y(yy) {};
		TPoint2D(void) : x(0), y(0) {};

		int x;
		int y;
};
//#endif

//#pragma on (unreferenced)
//#endif


//////////////////////////////////////////////////////////////////////////
//
// Defines for direct draw
//
//
extern	LPDIRECTDRAW	DirectDrawObject;	//pointer to direct draw object
extern	HWND			MainWindow;			//handle to programs main window

/*
** Pointer to function to call if we detect a focus loss
*/
extern	void (*Gbuffer_Focus_Loss_Function)(void);

enum GBC_Enum {
	GBC_NONE				= 0,
	GBC_VIDEOMEM		= 1,
	GBC_VISIBLE			= 2,
};

#define	NOT_LOCKED		NULL

/*=========================================================================*/
/* Define the screen width and height to make portability to other modules	*/
/*		easier.																					*/
/*=========================================================================*/
#define	DEFAULT_SCREEN_WIDTH		768
#define	DEFAULT_SCREEN_HEIGHT	768

/*=========================================================================*/
/* Let the compiler know that a GraphicBufferClass exists so that it can	*/
/*		keep a pointer to it in a VideoViewPortClass.								*/
/*=========================================================================*/
class GraphicViewPortClass;
class GraphicBufferClass;
class VideoViewPortClass;
class VideoBufferClass;

GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass *ptr);
GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass &ptr);

/*=========================================================================*/
/* GraphicViewPortClass - Holds viewport information on a viewport which	*/
/*		has been attached to a GraphicBuffer.  A viewport is effectively a	*/
/*		rectangular subset of the full buffer which is used for clipping and	*/
/*		the like.																				*/
/*																									*/
/*			char	  	*Buffer	-		is the offset to view port buffer			*/
/*			int	  	Width		-		is the width of view port						*/
/*			int	  	Height	-		is the height of view port						*/
/*			int	  	XAdd		-		is add value to go from the end of a line	*/
/*											to the beginning of the next line			*/
/*			int		XPos;		- 		x offset into its associated VideoBuffer	*/
/*			int		YPos;		-		y offset into its associated VideoBuffer	*/
/*=========================================================================*/
class GraphicViewPortClass {
	public:

		/*===================================================================*/
		/* Define the base constructor and destructors for the class			*/
		/*===================================================================*/
		GraphicViewPortClass(GraphicBufferClass* graphic_buff, int x, int y, int w, int h);
		GraphicViewPortClass();
		~GraphicViewPortClass();

		/*===================================================================*/
		/* define functions to get at the private data members					*/
		/*===================================================================*/
		long	Get_Offset(void);
		int	Get_Height(void);
		int	Get_Width(void);
		int	Get_XAdd(void);
		int	Get_XPos(void);
		int	Get_YPos(void);
		int	Get_Pitch(void);
		inline BOOL	Get_IsDirectDraw(void);
		GraphicBufferClass	*Get_Graphic_Buffer(void);

		/*===================================================================*/
		/* Define a function which allows us to change a video viewport on	*/
		/*		the fly.																			*/
		/*===================================================================*/
		BOOL Change(int x, int y, int w, int h);

		/*===================================================================*/
		/* Define the set of common graphic functions that are supported by	*/
		/*		both Graphic ViewPorts and VideoViewPorts.							*/
		/*===================================================================*/
		long	Size_Of_Region(int w, int h);
		void	Put_Pixel(int x, int y, unsigned char color);
		int	Get_Pixel(int x, int y);
		void	Clear(unsigned char color = 0);
		long	To_Buffer(int x, int y, int w, int h, void *buff, long size);
		long	To_Buffer(int x, int y, int w, int h, BufferClass *buff);
		long	To_Buffer(BufferClass *buff);
		HRESULT	Blit(	GraphicViewPortClass& dest, int x_pixel, int y_pixel, int dx_pixel,
						int dy_pixel, int pixel_width, int pixel_height, BOOL trans = FALSE);
		HRESULT	Blit(	GraphicViewPortClass& dest, int dx, int dy, BOOL trans = FALSE);
		HRESULT	Blit(	GraphicViewPortClass& dest, BOOL trans = FALSE);
		HRESULT	Blit(	VideoViewPortClass& dest, int x_pixel, int y_pixel, int dx_pixel,
						int dy_pixel, int pixel_width, int pixel_height, BOOL trans = FALSE);
		HRESULT	Blit(	VideoViewPortClass& dest, int dx, int dy, BOOL trans = FALSE);
		HRESULT	Blit(	VideoViewPortClass& dest, BOOL trans = FALSE);
		BOOL	Scale(	GraphicViewPortClass &dest, int src_x, int src_y, int dst_x,
							int dst_y, int src_w, int src_h, int dst_w, int dst_h, BOOL trans = FALSE, char *remap = NULL);
		BOOL	Scale(	GraphicViewPortClass &dest, int src_x, int src_y, int dst_x,
							int dst_y, int src_w, int src_h, int dst_w, int dst_h, char *remap);
		BOOL	Scale(	GraphicViewPortClass &dest, BOOL trans = FALSE, char *remap = NULL);
		BOOL	Scale(	GraphicViewPortClass &dest, char *remap);
		BOOL	Scale(	VideoViewPortClass &dest, int src_x, int src_y, int dst_x,
							int dst_y, int src_w, int src_h, int dst_w, int dst_h, BOOL trans = FALSE, char *remap = NULL);
		BOOL	Scale(	VideoViewPortClass &dest, int src_x, int src_y, int dst_x,
							int dst_y, int src_w, int src_h, int dst_w, int dst_h, char *remap);
		BOOL	Scale(	VideoViewPortClass &dest, BOOL trans = FALSE, char *remap = NULL);
		BOOL	Scale(	VideoViewPortClass &dest, char *remap);
		unsigned long	Print(char const *string, int x_pixel, int y_pixel, int fcolor, int bcolor);
		unsigned long	Print(short num, int x_pixel, int y_pixel, int fcol, int bcol);
		unsigned long	Print(int num, int x_pixel, int y_pixel, int fcol, int bcol);
		unsigned long	Print(long num, int x_pixel, int y_pixel, int fcol, int bcol);

		/*===================================================================*/
		/* Define the list of graphic functions which work only with a 		*/
		/*		graphic buffer.																*/
		/*===================================================================*/
		VOID Draw_Line(int sx, int sy, int dx, int dy, unsigned char color);
		VOID Draw_Rect(int sx, int sy, int dx, int dy, unsigned char color);
		VOID Fill_Rect(int sx, int sy, int dx, int dy, unsigned char color);
		VOID Fill_Quad(VOID *span_buff, int x0, int y0, int x1, int y1,
							int x2, int y2, int x3, int y3, int color);
		VOID Remap(int sx, int sy, int width, int height, VOID *remap);
		VOID Remap(VOID *remap);
		void Draw_Stamp(void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap);
		void Draw_Stamp(void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap, int clip_window);

// PG_TO_FIX
// This seems to be missing. Might not be needed anyway since it looks like it's only used for UI drawing. ST - 12/17/2018 6:11PM
		void Texture_Fill_Rect (int xpos, int ypos, int width, int height, void const *shape_pointer, int source_width, int source_height) {
			return;
		}

//	This doesnt seem to exist anywhere?? - Steve T 9/26/95 6:05PM
//		VOID Grey_Out_Region(int x, int y, int width, int height, int color);

		//
		// New members to lock and unlock the direct draw video memory
		//
		inline BOOL 	Lock ();
		inline BOOL 	Unlock();
		inline int		Get_LockCount();

		// Member to blit using direct draw access to hardware blitter
		HRESULT DD_Linear_Blit_To_Linear ( GraphicViewPortClass &dest, int source_x, int source_y, int dest_x, int dest_y, int width	, int height, BOOL mask );

		/*===================================================================*/
		/* Define functions to attach the viewport to a graphicbuffer			*/
		/*===================================================================*/
		VOID Attach(GraphicBufferClass *graphic_buff, int x, int y, int w, int h);
		void Attach(GraphicBufferClass *video_buff, int w, int h);

	protected:

		/*===================================================================*/
		/* Define the data used by a GraphicViewPortClass							*/
		/*===================================================================*/
		long					Offset;			// offset to graphic page
		int						Width;			// width of graphic page
		int						Height;			// height of graphic page
		int						XAdd;			// xadd for graphic page (0)
		int						XPos;			// x offset in relation to graphicbuff
		int						YPos;			// y offset in relation to graphicbuff
		long					Pitch;			//Distance from one line to the next
		GraphicBufferClass		*GraphicBuff;	// related graphic buff
		BOOL					IsDirectDraw;	//Flag to let us know if it is a direct draw surface
		int						LockCount;		// Count for stacking locks if non-zero the buffer
};                                              //   is a locked DD surface

/*=========================================================================*/
/* GraphicBufferClass - A GraphicBuffer refers to an actual instance of an	*/
/*		allocated buffer.  The GraphicBuffer may be drawn to directly 			*/
/*		becuase it inherits a ViewPort which represents its physcial size.	*/
/*																									*/
/*			BYTE	  	*Buffer	-		is the offset to graphic buffer				*/
/*			int	  	Width		-		is the width of graphic buffer				*/
/*			int	  	Height	-		is the height of graphic buffer				*/
/*			int	  	XAdd		-		is the xadd of graphic buffer					*/
/*			int		XPos;		- 		will be 0 because it is graphicbuff			*/
/*			int		YPos;		-		will be 0 because it is graphicbuff			*/
/*			long	Pitch		-		modulo of buffer for reading and writing																						*/
/*			BOOL	IsDirectDraw - 		flag if its a direct draw surface																						*/
/*=========================================================================*/
class GraphicBufferClass : public GraphicViewPortClass, public BufferClass {

	public:
		GraphicBufferClass(int w, int h, GBC_Enum flags);
		GraphicBufferClass(int w, int h,	void *buffer, long size);
		GraphicBufferClass(int w, int h, void *buffer = 0);
		GraphicBufferClass(void);
		~GraphicBufferClass();

		void DD_Init(GBC_Enum flags);
		void Init(int w, int h, void *buffer, long size, GBC_Enum flags);
		void Un_Init(void);
		void GraphicBufferClass::Attach_DD_Surface (GraphicBufferClass * attach_buffer);
		BOOL Lock(void);
		BOOL Unlock(void);

		void Scale_Rotate(BitmapClass &bmp,TPoint2D const &pt,long scale,unsigned char angle);

		// Member to get a pointer to a direct draw surface
		LPDIRECTDRAWSURFACE Get_DD_Surface ( void );

	protected:
		LPDIRECTDRAWSURFACE	VideoSurfacePtr;		//Pointer to the related direct draw surface
		DDSURFACEDESC			VideoSurfaceDescription;//Description of the said surface

};



inline int GraphicViewPortClass::Get_LockCount(void)
{
	return (LockCount);
}



/***********************************************************************************************
 * GVPC::Get_IsDirectDraw -- provide read access to the IsDirectDraw flag                      *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   IsDirectDraw                                                                      *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/29/95 1:02PM ST : Created                                                             *
 *=============================================================================================*/
inline BOOL GraphicViewPortClass::Get_IsDirectDraw(void)
{
	return (IsDirectDraw);
}



/***********************************************************************************************
 * GBC::Get_DD_Surface -- returns a pointer to the buffer direct draw surface                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   ptr to direct draw surface                                                        *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/29/95 9:43AM ST : Created                                                              *
 *=============================================================================================*/
inline LPDIRECTDRAWSURFACE GraphicBufferClass::Get_DD_Surface ( void )
{
	return ( VideoSurfacePtr );

}



/***********************************************************************************************
 * GVPC::Lock -- lock the graphics buffer for reading or writing                               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   TRUE if surface was successfully locked                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09-19-95 12:33pm ST : Created                                                             *
 *   10/09/1995     : Moved actually functionality to GraphicBuffer                            *
 *=============================================================================================*/
inline BOOL GraphicViewPortClass::Lock(void)
{
	BOOL lock = GraphicBuff->Lock();
	if ( !lock ) return(FALSE);

	if (this != GraphicBuff) {
		Attach(GraphicBuff, XPos, YPos,  Width, Height);
	}
	return(TRUE);
}

/***********************************************************************************************
 * GVPC::Unlock -- unlock the video buffer                                                     *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   TRUE if surface was successfully unlocked                                         *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09-19-95 02:20pm ST : Created                                                             *
 *   10/09/1995     : Moved actually functionality to GraphicBuffer                            *
 *=============================================================================================*/
inline BOOL GraphicViewPortClass::Unlock(void)
{
	BOOL unlock = GraphicBuff->Unlock();
	if (!unlock) return(FALSE);
	if (this != GraphicBuff && IsDirectDraw && !GraphicBuff->LockCount) {
		Offset = 0;
	}
	return(TRUE);
}


/***************************************************************************
 * GVPC::GET_OFFSET -- Get offset for virtual view port class instance     *
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     long the offset for the virtual viewport instance           *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/07/1994 PWG : Created.                                             *
 *=========================================================================*/
inline long GraphicViewPortClass::Get_Offset(void)
{
	return(Offset);
}

/***************************************************************************
 * GVPC::GET_HEIGHT -- Gets the height of a virtual viewport instance      *
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     WORD the height of the virtual viewport instance            *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/07/1994 PWG : Created.                                             *
 *=========================================================================*/
inline int GraphicViewPortClass::Get_Height(void)
{
	return(Height);
}

/***************************************************************************
 * GVPC::GET_WIDTH -- Get the width of a virtual viewport instance			*
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     WORD the width of the virtual viewport instance             *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/07/1994 PWG : Created.                                             *
 *=========================================================================*/
inline int GraphicViewPortClass::Get_Width(void)
{
	return(Width);
}


/***************************************************************************
 * GVPC::GET_XADD -- Get the X add offset for virtual viewport instance    *
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     WORD the xadd for a virtual viewport instance               *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/07/1994 PWG : Created.                                             *
 *=========================================================================*/
inline int GraphicViewPortClass::Get_XAdd(void)
{
	return(XAdd);
}
/***************************************************************************
 * GVPC::GET_XPOS -- Get the x pos of the VP on the Video                  *
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     WORD the x offset to VideoBufferClass								*
 *                                                                         *
 * HISTORY:                                                                *
 *   08/22/1994 SKB : Created.                                             *
 *=========================================================================*/
inline int GraphicViewPortClass::Get_XPos(void)
{
	return(XPos);
}


/***************************************************************************
 * GVPC::GET_YPOS -- Get the y pos of the VP on the video                  *
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     WORD the x offset to VideoBufferClass								*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/22/1994 SKB : Created.                                             *
 *=========================================================================*/
inline int GraphicViewPortClass::Get_YPos(void)
{
	return(YPos);
}

/***************************************************************************
 * GVPC::GET_GRAPHIC_BUFFER -- Get the graphic buffer of the VP.            *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/22/1994 SKB : Created.                                             *
 *=========================================================================*/
inline GraphicBufferClass *GraphicViewPortClass::Get_Graphic_Buffer(void)
{
	return (GraphicBuff);
}

/***************************************************************************
 * GVPC::SIZE_OF_REGION -- stub to call curr graphic mode Size_Of_Region	*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   03/01/1995 BWG : Created.                                             *
 *=========================================================================*/
inline long	GraphicViewPortClass::Size_Of_Region(int w, int h)
{
	return Buffer_Size_Of_Region(this, w, h);
}


/***************************************************************************
 * GVPC::PUT_PIXEL -- stub to call curr graphic mode Put_Pixel					*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline void	GraphicViewPortClass::Put_Pixel(int x, int y, unsigned char color)
{

	if (Lock()){
		Buffer_Put_Pixel(this, x, y, color);
	}
	Unlock();


}

/***************************************************************************
 * GVPC::GET_PIXEL -- stub to call curr graphic mode Get_Pixel          	*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline int	GraphicViewPortClass::Get_Pixel(int x, int y)
{
	int		return_code=0;

	if (Lock()){
		return_code=(Buffer_Get_Pixel(this, x, y));
	}
	Unlock();
	return(return_code);

}

/***************************************************************************
 * GVPC::CLEAR -- stub to call curr graphic mode Clear	                  *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline void	GraphicViewPortClass::Clear(unsigned char color)
{
	if (Lock()){
		Buffer_Clear(this, color);
	}
	Unlock();

}

/***************************************************************************
 * GVPC::TO_BUFFER -- stub 1 to call curr graphic mode To_Buffer				*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline long	GraphicViewPortClass::To_Buffer(int x, int y, int w, int h, void *buff, long size)
{
	long	return_code=0;
	if (Lock()){
		return_code = (Buffer_To_Buffer(this, x, y, w, h, buff, size));
	}
	Unlock();
	return ( return_code );
}

/***************************************************************************
 * GVPC::TO_BUFFER -- stub 2 to call curr graphic mode To_Buffer 				*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline long	GraphicViewPortClass::To_Buffer(int x, int y, int w, int h, BufferClass *buff)
{
	long	return_code=0;
	if (Lock()){
		return_code = (Buffer_To_Buffer(this, x, y, w, h, buff->Get_Buffer(), buff->Get_Size()));
	}
	Unlock();
	return ( return_code );
}

/***************************************************************************
 * GVPC::TO_BUFFER -- stub 3 to call curr graphic mode To_Buffer 				*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline long	GraphicViewPortClass::To_Buffer(BufferClass *buff)
{
	long	return_code=0;
	if (Lock()){
		return_code = (Buffer_To_Buffer(this, 0, 0, Width, Height, buff->Get_Buffer(), buff->Get_Size()));
	}
	Unlock();
	return ( return_code );
}

/***************************************************************************
 * GVPC::BLIT -- stub 1 to call curr graphic mode Blit to GVPC					*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline HRESULT	GraphicViewPortClass::Blit(	GraphicViewPortClass& dest, int x_pixel, int y_pixel, int dx_pixel,
				int dy_pixel, int pixel_width, int pixel_height, BOOL trans)
{
	HRESULT		return_code=0;

	if ( IsDirectDraw && dest.IsDirectDraw ){
	return(DD_Linear_Blit_To_Linear( dest, XPos+x_pixel, YPos+y_pixel
								, dest.Get_XPos()+dx_pixel, dest.Get_YPos()+dy_pixel
								, pixel_width, pixel_height, trans));
	} else {

		if (Lock()){
			if (dest.Lock()){
				return_code=(Linear_Blit_To_Linear(this, &dest, x_pixel, y_pixel
														, dx_pixel, dy_pixel
														, pixel_width, pixel_height, trans));
			}
			dest.Unlock();
		}
		Unlock();
	}

	return ( return_code );
}

/***************************************************************************
 * GVPC::BLIT -- Stub 2 to call curr graphic mode Blit to GVPC					*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline HRESULT	GraphicViewPortClass::Blit(	GraphicViewPortClass& dest, int dx, int dy, BOOL trans)
{
	HRESULT		return_code=0;


	if ( IsDirectDraw && dest.IsDirectDraw ){
	return(DD_Linear_Blit_To_Linear( dest, XPos, YPos
								, dest.Get_XPos()+dx, dest.Get_YPos()+dy
								, Width, Height, trans));
	} else {

		if (Lock()){
			if (dest.Lock()){
				return_code=(Linear_Blit_To_Linear(this, &dest, 0, 0
														, dx, dy
														, Width, Height, trans));
			}
			dest.Unlock();
		}
		Unlock();
	}

	return (return_code);

}

/***************************************************************************
 * GVPC::BLIT -- stub 3 to call curr graphic mode Blit to GVPC					*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline HRESULT	GraphicViewPortClass::Blit(	GraphicViewPortClass& dest, BOOL trans)
{
	HRESULT		return_code=0;


	if ( IsDirectDraw && dest.IsDirectDraw ){
	return(DD_Linear_Blit_To_Linear( dest, XPos, YPos
								, dest.Get_XPos(), dest.Get_YPos()
								, MAX( Width, dest.Get_Width())
								, MAX( Height, dest.Get_Height())
								, trans));
	} else {

		if (Lock()){
			if (dest.Lock()){
				return_code=(Linear_Blit_To_Linear(this, &dest, 0, 0
														, 0, 0
														, Width, Height, trans));
			}
			dest.Unlock();
		}
		Unlock();
	}

	return (return_code);

}


/***************************************************************************
 * GVPC::SCALE -- stub 1 to call curr graphic mode Scale to GVPC				*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline BOOL	GraphicViewPortClass::Scale(	GraphicViewPortClass &dest, int src_x, int src_y, int dst_x,
					int dst_y, int src_w, int src_h, int dst_w, int dst_h, BOOL trans, char *remap)
{
	BOOL	return_code=0;
	if (Lock()){
		if (dest.Lock()){
			return_code = (Linear_Scale_To_Linear(this, &dest, src_x, src_y, dst_x, dst_y, src_w, src_h, dst_w, dst_h, trans, remap));
		}
		dest.Unlock();
	}
	Unlock();
	return ( return_code );
}

/***************************************************************************
 * GVPC::SCALE -- stub 2 to call curr graphic mode Scale to GVPC				*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline BOOL	GraphicViewPortClass::Scale(	GraphicViewPortClass &dest, int src_x, int src_y, int dst_x,
					int dst_y, int src_w, int src_h, int dst_w, int dst_h, char *remap)
{
	BOOL	return_code=0;
	if (Lock()){
		if (dest.Lock()){
			return_code = (Linear_Scale_To_Linear(this, &dest, src_x, src_y, dst_x, dst_y, src_w, src_h, dst_w, dst_h, FALSE, remap));
		}
		dest.Unlock();
	}
	Unlock();
	return ( return_code );
}

/***************************************************************************
 * GVPC::SCALE -- stub 3 to call curr graphic mode Scale to GVPC				*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline BOOL	GraphicViewPortClass::Scale(	GraphicViewPortClass &dest, BOOL trans, char *remap)
{
	BOOL	return_code=0;
	if (Lock()){
		if (dest.Lock()){
			return_code = (Linear_Scale_To_Linear(this,	&dest, 0, 0, 0, 0, Width, Height, dest.Get_Width(), dest.Get_Height(), trans, remap));
		}
		dest.Unlock();
	}
	Unlock();
	return ( return_code );
}

/***************************************************************************
 * GVPC::SCALE -- stub 4 to call curr graphic mode Scale to GVPC				*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/06/1995 PWG : Created.                                             *
 *=========================================================================*/
inline BOOL	GraphicViewPortClass::Scale(	GraphicViewPortClass &dest, char *remap)
{
	BOOL	return_code=0;
	if (Lock()){
		if (dest.Lock()){
			return_code = (Linear_Scale_To_Linear(this, &dest, 0, 0, 0, 0, Width, Height, dest.Get_Width(), dest.Get_Height(), FALSE, remap));
		}
		dest.Unlock();
	}
	Unlock();
	return ( return_code );
}
/***************************************************************************
 * GVPC::PRINT -- stub func to print a text string                         *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/17/1995 PWG : Created.                                             *
 *=========================================================================*/
inline unsigned long	GraphicViewPortClass::Print(char const *str, int x, int y, int fcol, int bcol)
{
	unsigned long	return_code=0;
	if (Lock()){
		return_code = (Buffer_Print(this, str, x, y, fcol, bcol));
	}
	Unlock();
	return ( return_code );
}

#pragma warning (disable:4996)

/***************************************************************************
 * GVPC::PRINT -- Stub function to print an integer                        *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *=========================================================================*/
inline unsigned long	GraphicViewPortClass::Print(int num, int x, int y, int fcol, int bcol)
{
	char str[17];

	unsigned long	return_code=0;
	if (Lock()){
		return_code = (Buffer_Print(this, itoa(num, str, 10), x, y, fcol, bcol));
	}
	Unlock();
	return ( return_code );
}

/***************************************************************************
 * GVPC::PRINT -- Stub function to print a short to a graphic viewport     *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *=========================================================================*/
inline unsigned long	GraphicViewPortClass::Print(short num, int x, int y, int fcol, int bcol)
{
	char str[17];

	unsigned long	return_code=0;
	if (Lock()){
		return_code = (Buffer_Print(this, itoa(num, str, 10), x, y, fcol, bcol));
	}
	Unlock();
	return ( return_code );
}

/***************************************************************************
 * GVPC::PRINT -- stub function to print a long on a graphic view port     *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *=========================================================================*/
inline unsigned long	GraphicViewPortClass::Print(long num, int x, int y, int fcol, int bcol)
{
	char str[33];

	unsigned long	return_code=0;
	if (Lock()){
		return_code = (Buffer_Print(this, ltoa(num, str,10), x, y, fcol, bcol));
	}
	Unlock();
	return ( return_code );
}

/***************************************************************************
 * GVPC::DRAW_STAMP -- stub function to draw a tile on a graphic view port *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *=========================================================================*/
inline void GraphicViewPortClass::Draw_Stamp(void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap)
{
	if (Lock()){
		Buffer_Draw_Stamp(this, icondata, icon, x_pixel, y_pixel, remap);
	}
	Unlock();
}



/***************************************************************************
 * GVPC::DRAW_STAMP -- stub function to draw a tile on a graphic view port *
 *                     This version clips the tile to a window             *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *    07/31/1995 BWG : Created.                                            *
 *=========================================================================*/
extern BOOL IconCacheAllowed;
inline void GraphicViewPortClass::Draw_Stamp(void const * icondata, int icon, int x_pixel, int y_pixel, void const * remap, int clip_window)
{
	int	cache_index=-1;

   int drewit = 0;
	if (IconCacheAllowed){
		if (IsDirectDraw){
			if (!remap){
				cache_index = Is_Icon_Cached(icondata,icon);
			}

			if (cache_index != -1){
				if (CachedIcons[cache_index].Get_Is_Cached()  ){
					CachedIcons[cache_index].Draw_It (GraphicBuff->Get_DD_Surface() , x_pixel, y_pixel,
																WindowList[clip_window][WINDOWX] + XPos,
																WindowList[clip_window][WINDOWY] +YPos,
																WindowList[clip_window][WINDOWWIDTH],
																WindowList[clip_window][WINDOWHEIGHT]);
					CachedIconsDrawn++;
               drewit = 1;
				}
			}
		}
	}


   if (drewit == 0) {
		if (Lock()){
			UnCachedIconsDrawn++;
			Buffer_Draw_Stamp_Clip(this, icondata, icon, x_pixel, y_pixel, remap, WindowList[clip_window][WINDOWX], WindowList[clip_window][WINDOWY], WindowList[clip_window][WINDOWWIDTH], WindowList[clip_window][WINDOWHEIGHT]);
		}
   }
	Unlock();
}


/***************************************************************************
 * GVPC::DRAW_LINE -- Stub function to draw line in Graphic Viewport Class *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/16/1995 PWG : Created.                                             *
 *=========================================================================*/
inline VOID GraphicViewPortClass::Draw_Line(int sx, int sy, int dx, int dy, unsigned char color)
{
	if (Lock()){
		Buffer_Draw_Line(this, sx, sy, dx, dy, color);
	}
	Unlock();
}

/***************************************************************************
 * GVPC::FILL_RECT -- Stub function to fill rectangle in a GVPC            *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/16/1995 PWG : Created.                                             *
 *=========================================================================*/
inline VOID GraphicViewPortClass::Fill_Rect(int sx, int sy, int dx, int dy, unsigned char color)
{
	if (  AllowHardwareBlitFills
			&& IsDirectDraw
			&& ( (dx-sx) * (dy-sy) >= (32*32) )
			&& GraphicBuff->Get_DD_Surface()->GetBltStatus(DDGBS_CANBLT) == DD_OK){
		DDBLTFX	blit_effects;
		RECT	dest_rectangle;

		dest_rectangle.left	=sx+XPos;
		dest_rectangle.top	=sy+YPos;
		dest_rectangle.right	=dx+XPos;
		dest_rectangle.bottom=dy+YPos;

		if (dest_rectangle.left<XPos){
			dest_rectangle.left=XPos;
		}

		if (dest_rectangle.right >= Width + XPos){
			dest_rectangle.right = Width +XPos -1;
		}

		if (dest_rectangle.top<YPos){
			dest_rectangle.top=YPos;
		}

		if (dest_rectangle.bottom >= Height + YPos){
			dest_rectangle.bottom = Height + YPos -1;
		}

		if (dest_rectangle.left >= dest_rectangle.right)  return;
		if (dest_rectangle.top  >= dest_rectangle.bottom) return;

		dest_rectangle.right++;
		dest_rectangle.bottom++;

		blit_effects.dwSize=sizeof(blit_effects);
		blit_effects.dwFillColor = color;
		GraphicBuff->Get_DD_Surface()->Blt(&dest_rectangle,
														NULL,
														NULL,
														DDBLT_WAIT | DDBLT_ASYNC | DDBLT_COLORFILL,
														&blit_effects);
	} else {
		if (Lock()){
			Buffer_Fill_Rect(this, sx, sy, dx, dy, color);
			Unlock();
		}
	}
}


/***************************************************************************
 * GVPC::REMAP -- Stub function to remap a GVPC                            *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/16/1995 PWG : Created.                                             *
 *=========================================================================*/
inline VOID GraphicViewPortClass::Remap(int sx, int sy, int width, int height, VOID *remap)
{
	if (Lock()){
		Buffer_Remap(this, sx, sy, width, height, remap);
	}
	Unlock();
}


inline VOID GraphicViewPortClass::Fill_Quad(VOID *span_buff, int x0, int y0, int x1, int y1,
							int x2, int y2, int x3, int y3, int color)
{
	if (Lock()){
		Buffer_Fill_Quad(this, span_buff, x0, y0, x1, y1, x2, y2, x3, y3, color);
	}
	Unlock();
}

/***************************************************************************
 * GVPC::REMAP -- Short form to remap an entire graphic view port          *
 *                                                                         *
 * INPUT:		BYTE * to the remap table to use										*
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/01/1994 PWG : Created.                                             *
 *=========================================================================*/
inline VOID GraphicViewPortClass::Remap(VOID *remap)
{
	if (Lock()){
		Buffer_Remap(this, 0, 0, Width, Height, remap);
	}
	Unlock();
}

inline int GraphicViewPortClass::Get_Pitch(void)
{
	return(Pitch);
}
/*=========================================================================*/
/* The following BufferClass functions are defined here because they act	*/
/*		on graphic viewports.																*/
/*=========================================================================*/


/***************************************************************************
 * BUFFER_TO_PAGE -- Generic 'c' callable form of Buffer_To_Page           *
 *                                                                         *
 * INPUT: 																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/12/1995 PWG : Created.                                             *
 *=========================================================================*/
inline long Buffer_To_Page(int x, int y, int w, int h, void *Buffer, GraphicViewPortClass &view)
{
	long	return_code=0;
	if (view.Lock()){
		return_code = (Buffer_To_Page(x, y, w, h, Buffer, &view));
	}
	view.Unlock();
	return ( return_code );
}

/***************************************************************************
 * BC::TO_PAGE -- Copys a buffer class to a page with definable w, h 		*
 *                                                                         *
 * INPUT:		int	width		- the width of copy region							*
 *					int	height	- the height of copy region						*
 *					GVPC&	dest		- virtual viewport to copy to						*
 *                                                                         *
 * OUTPUT:		none                                                        *
 *																									*
 * WARNINGS:	x and y position are the upper left corner of the dest		*
 *						viewport																	*
 *                                                                         *
 * HISTORY:                                                                *
 *   07/01/1994 PWG : Created.                                             *
 *=========================================================================*/
inline long BufferClass::To_Page(int w, int h, GraphicViewPortClass &view)
{
	long	return_code=0;
	if (view.Lock()){
		return_code = (Buffer_To_Page(0, 0, w, h, Buffer, &view));
	}
	view.Unlock();
	return ( return_code );
}
/***************************************************************************
 * BC::TO_PAGE -- Copys a buffer class to a page with definable w, h 		*
 *                                                                         *
 * INPUT:		GVPC&	dest		- virtual viewport to copy to						*
 *                                                                         *
 * OUTPUT:		none                                                        *
 *																									*
 * WARNINGS:	x and y position are the upper left corner of the dest		*
 *						viewport.  width and height are assumed to be the			*
 *						viewport's width and height.										*
 *                                                                         *
 * HISTORY:                                                                *
 *   07/01/1994 PWG : Created.                                             *
 *=========================================================================*/
inline long BufferClass::To_Page(GraphicViewPortClass &view)
{
	long	return_code=0;
	if (view.Lock()){
		return_code = (Buffer_To_Page(0, 0, view.Get_Width(), view.Get_Height(), Buffer, &view));
	}
	view.Unlock();
	return ( return_code );
}
/***************************************************************************
 * BC::TO_PAGE -- Copys a buffer class to a page with definable x, y, w, h *
 *                                                                         *
 * INPUT:	int	x			- x pixel on viewport to copy from					*
 *				int	y			- y pixel on viewport to copy from					*
 *				int	width		- the width of copy region								*
 *				int	height	- the height of copy region							*
 *				GVPC&	dest		- virtual viewport to copy to							*
 *                                                                         *
 * OUTPUT:	none                                                           *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/01/1994 PWG : Created.                                             *
 *=========================================================================*/
inline long BufferClass::To_Page(int x, int y, int w, int h, GraphicViewPortClass &view)
{
	long	return_code=0;
	if (view.Lock()){
		return_code = (Buffer_To_Page(x, y, w, h, Buffer, &view));
	}
	view.Unlock();
	return ( return_code );
}


#endif