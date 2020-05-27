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
 *                 Project Name : iff                                      *
 *                                                                         *
 *                    File Name : WRITEPCX.CPP                             *
 *                                                                         *
 *                   Programmer : Julio R. Jerez                           *
 *                                                                         *
 *                   Start Date : May 2, 1995                              *
 *                                                                         *
 *                  Last Update : May 2, 1995   [JRJ]                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * int Save_PCX_File (char* name, GraphicViewPortClass& pic, char* palette)*
 *= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

#include  <wwlib32.h>
#include "filepcx.h"
#include <wwmem.h>
static void Write_Pcx_ScanLine ( int file_handle , int scansize , unsigned char * ptr );


/***************************************************************************
 * WRITE_PCX_FILE -- Write the data in ViewPort to a pcx file              *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:  name is a NULL terminated string of the fromat [xxxx.pcx]			*
 *			  pic	 is a pointer to a GraphicViewPortClass or to a 				*
 *					 GraphicBufferClass holding the picture. 							*
 *        palette is a pointer the the memry block holding the color 		*														*
 *              palette of the picture.                                    *
 *                                                                         *
 * OUTPUT: FALSE  if the function fails zero otherwise             			*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/04/1995 JRJ : Created.                                             *
 *   08/01/1995 SKB : Copy the palette so it is not modified.              *
 *=========================================================================*/
int Write_PCX_File (char* name, GraphicViewPortClass& pic, unsigned char* palette )
{
 	unsigned char palcopy[256 * 3];
	unsigned     i ;
	//unsigned	   width ;
	int 	      file_handle ;
	int				VP_Scan_Line ;
	char	     * ptr ;
	RGB  		  * pal ;
	GraphicBufferClass	* Graphic_Buffer ;
	PCX_HEADER   header = { 10 , 5 , 1 , 8 , 0 , 0 , 319 , 199 ,
									320 , 200 , { 0 } , 0 , 1 , 320 , 1 , {0} } ;

	// Open file name
	file_handle = Open_File ( name , WRITE ) ;
	if ( file_handle == WW_ERROR ) return FALSE ;


	header.width  = pic.Get_Width() - 1 ;
	header.height = pic.Get_Height() - 1 ;
	header.byte_per_line = pic.Get_Width() ;
	Write_File ( file_handle, & header , sizeof (PCX_HEADER)) ;

	VP_Scan_Line = pic.Get_Width()	+ pic.Get_XAdd();
	Graphic_Buffer = pic.Get_Graphic_Buffer() ;
	ptr = ( char * ) Graphic_Buffer->Get_Buffer() ;
	ptr += ( (pic.Get_YPos() * VP_Scan_Line) + pic.Get_XPos() );

	for ( i = 0 ; i < (unsigned)header.height + 1 ; i ++ )
   	Write_Pcx_ScanLine ( file_handle , header.byte_per_line, (unsigned char*)ptr + i * VP_Scan_Line ) ;

	Mem_Copy(palette, palcopy, 256 * 3);
	pal = ( RGB * ) palcopy ;
	for ( i = 0 ; i < 256 ; i ++ ) {
		pal -> red <<= 2 ;
		pal -> green <<= 2 ;
		pal -> blue <<= 2 ;
		pal ++ ;
	}
	i = 0x0c ;
	Write_File ( file_handle, & i , 1 ) ;
	Write_File ( file_handle, palcopy , 256 * sizeof (RGB) ) ;
	Close_File (file_handle) ;
	return 0 ;
}




/***************************************************************************
 * WRITE_PCX_SCANLINE -- function to write a single pcx scanline to a file *
 *                                                                         *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/04/1995 JRJ : Created.                                             *
 *=========================================================================*/

#define	POOL_SIZE 2048
#define WRITE_CHAR(x) {	\
								* file_ptr ++ = x ; \
							   if ( file_ptr >= & pool [ POOL_SIZE ]	) { \
								  Write_File ( file_handle, pool , POOL_SIZE ) ; \
								  file_ptr = pool ; \
							 }	}


void Write_Pcx_ScanLine ( int file_handle , int scansize , unsigned char * ptr )
{
	unsigned     i ;
	unsigned     rle ;
	unsigned     color ;
	unsigned		last ;
	unsigned char		  * file_ptr ;
	unsigned char		 	pool [ POOL_SIZE ] ;

	file_ptr = pool ;
	last = * ptr ;
	rle = 1 ;

	for ( i = 1 ; i < (unsigned)scansize ; i ++ ) {
   	color = 0xff & * ++ ptr ;
		if ( color == last ) {
			rle ++ ;
			if ( rle == 63 )	 {
				WRITE_CHAR ( 255 ) ;
				WRITE_CHAR ( color ) ;
				rle = 0 ;
			}
		} else {
	  		if ( rle ) {
		 		if ( rle == 1 && ( 192 != ( 192 & last ))) {
			   	WRITE_CHAR ( last ) ;
		     	} else {
		      	WRITE_CHAR ( rle | 192 ) ;
	     	   	WRITE_CHAR ( last ) ;
				}
			}
			last = color ;
			rle = 1 ;
		}
	}
	if ( rle ) {
		if ( rle == 1 && ( 192 != ( 192 & last ))) {
			WRITE_CHAR ( last ) ;
		} else {
			WRITE_CHAR ( rle | 192 ) ;
	   	WRITE_CHAR ( last) ;
		}
	}

	Write_File ( file_handle, pool , ( int ) file_ptr - ( int ) pool ) ;
}
