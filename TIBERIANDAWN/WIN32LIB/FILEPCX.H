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
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : iff                                      *
;*                                                                         *
;*                    File Name : FILEPCX.H                                *
;*                                                                         *
;*                   Programmer : Julio R. Jerez                           *
;*                                                                         *
;*                   Start Date : May 2, 1995                              *
;*                                                                         *
;*                  Last Update : May 2, 1995   [JRJ]                      *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* GraphicBufferClass* Read_PCX_File (char* name, BYTE* palette,void *buff, long size);
;* GraphicBufferClass* Read_PCX_File (char* name, BYTE* palette, BufferClass& Buff); 
;* int Write_PCX_File (char* name, GraphicViewPortClass& pic, BYTE* palette );*
;*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/
#include <gbuffer.h>
#include <string.h>
#include <buffer.h>
#include	<file.h>

#ifndef PCX_H
#define PCX_H


typedef	struct {
						  char	red ;
						  char	green ;
						  char	blue ;
					} RGB ;

typedef	struct	{
						  char      id ;
						  char	   version ;
						  char	   encoding ;
						  char	   pixelsize ;
						  short 	   x ;
						  short		y ;
						  short     width ;
						  short		height ;
						  short 	   xres ;
						  short		yres ;
						  RGB       ega_palette [ 16 ] ;
						  char	   nothing ;
						  char      color_planes ;
						  short 		byte_per_line ;
						  short 	 	palette_type ;
						  char	   filler [ 58 ] ;
						} PCX_HEADER ;

GraphicBufferClass* Read_PCX_File (char* name, char* palette= NULL,void *buff=NULL, long size=0);
GraphicBufferClass* Read_PCX_File (char* name, BufferClass& Buff,char* palette= NULL) ;
int Write_PCX_File (char* name, GraphicViewPortClass& pic, unsigned char* palette );

#endif
