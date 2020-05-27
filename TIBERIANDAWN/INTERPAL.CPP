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
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INTERPAL.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : December 7th 1995                                            *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *  This module contains functions to allow use of old 320x200 animations on a 640x400 screen  *
 *                                                                                             *
 * Functions:                                                                                  *
 *  Read_Interpolation_Palette -- reads an interpolation palette table from disk               *
 *  Write_Interpolation_Palette -- writes an interpolation palette to disk                     *
 *  Create_Palette_Interpolation_Table -- build the palette interpolation table                *
 *  Increase_Palette_Luminance -- increase the contrast of a palette                           *
 *  Interpolate_2X_Scale -- Stretch a 320x200 graphic buffer into 640x400                      *
 *                                                                                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

BOOL	InterpolationPaletteChanged = FALSE;
extern "C" {
extern void __cdecl Asm_Interpolate (unsigned char* src_ptr ,
						 						unsigned char* dest_ptr ,
												int				lines ,
												int				src_width ,
												int				dest_width);

extern void __cdecl Asm_Interpolate_Line_Double (unsigned char* src_ptr ,
						 						unsigned char* dest_ptr ,
												int				lines ,
												int				src_width ,
												int				dest_width);

extern void __cdecl Asm_Interpolate_Line_Interpolate (unsigned char* src_ptr ,
						 						unsigned char* dest_ptr ,
												int				lines ,
												int				src_width ,
												int				dest_width);

}

extern "C"{
	unsigned char PaletteInterpolationTable[SIZE_OF_PALETTE][SIZE_OF_PALETTE];
	unsigned char *InterpolationPalette;
}



/***********************************************************************************************
 * Read_Interpolatioin_Palette -- reads an interpolation palette table from disk               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    name of palette file                                                              *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    12/12/95 12:15PM ST : Created                                                            *
 *=============================================================================================*/

void Read_Interpolation_Palette (char const *palette_file_name)
{
	CCFileClass palette_file(palette_file_name);

	if (palette_file.Is_Available()){
		palette_file.Open(READ);
		palette_file.Read(&PaletteInterpolationTable[0][0],256*256);
		palette_file.Close();
		InterpolationPaletteChanged = FALSE;
	}
}


/***********************************************************************************************
 * Write_Interpolatioin_Palette -- writes an interpolation palette table to disk               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    name of palette file                                                              *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    12/12/95 12:15PM ST : Created                                                            *
 *=============================================================================================*/

void Write_Interpolation_Palette (char const *palette_file_name)
{
	CCFileClass palette_file(palette_file_name);

	if (!palette_file.Is_Available()){
		palette_file.Open(WRITE);
		palette_file.Write(&PaletteInterpolationTable[0][0],256*256);
		palette_file.Close();
	}
}





/***************************************************************************
 * CREATE_PALETTE_INTERPOLATION_TABLE                                      *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/06/1995  MG : Created.                                             *
 *=========================================================================*/
void Create_Palette_Interpolation_Table( void )
{

//Don't think we need this. ST - 12/20/2018 2:25PM
	//Asm_Create_Palette_Interpolation_Table();

	#if (0)

	int 				i;
	int 				j;
	int 				p;
	unsigned char	*first_palette_ptr;
	unsigned char	*second_palette_ptr;
	unsigned char	*match_pal_ptr;
	int				first_r;
	int				first_g;
	int				first_b;
	int				second_r;
	int				second_g;
	int				second_b;
	int				diff_r;
	int				diff_g;
	int				diff_b;
	int				dest_r;
	int				dest_g;
	int				dest_b;
	int 				distance;
	int 				closest_distance;
	int 				index_of_closest_color;

	//
	// Create an interpolation table for the current palette.
	//
	first_palette_ptr = (unsigned char *) InterpolationPalette;
	for ( i = 0; i < SIZE_OF_PALETTE; i ++ ) {

		//
		// Get the first palette entry's RGB.
		//
		first_r = *first_palette_ptr;
		first_palette_ptr ++;
		first_g = *first_palette_ptr;
		first_palette_ptr ++;
		first_b = *first_palette_ptr;
		first_palette_ptr ++;

		second_palette_ptr = (unsigned char *) InterpolationPalette;
		for  ( j = 0; j < SIZE_OF_PALETTE; j ++ ) {
			//
			// Get the second palette entry's RGB.
			//
			second_r = *second_palette_ptr;
			second_palette_ptr ++;
			second_g = *second_palette_ptr;
			second_palette_ptr ++;
			second_b = *second_palette_ptr;
			second_palette_ptr ++;

			//
			// Now calculate the RGB halfway between the first and second colors.
			//
			dest_r = ( first_r + second_r ) >> 1;
			dest_g = ( first_g + second_g ) >> 1;
			dest_b = ( first_b + second_b ) >> 1;

			//
			// Now find the color in the palette that most closely matches the interpolated color.
			//
			index_of_closest_color = 0;
//			closest_distance = (256 * 256) * 3;
			closest_distance = 500000;
			match_pal_ptr = (unsigned char *) InterpolationPalette;
			for ( p = 0; p < SIZE_OF_PALETTE; p ++ ) {
				diff_r = ( ((int) (*match_pal_ptr)) - dest_r );
				match_pal_ptr ++;
				diff_g = ( ((int) (*match_pal_ptr)) - dest_g );
				match_pal_ptr ++;
				diff_b = ( ((int) (*match_pal_ptr)) - dest_b );
				match_pal_ptr ++;

				distance = ( diff_r * diff_r ) + ( diff_g * diff_g ) + ( diff_b * diff_b );
				if ( distance < closest_distance ) {
					closest_distance = distance;
					index_of_closest_color = p;
				}
			}

			PaletteInterpolationTable[ i ][ j ] = (unsigned char) index_of_closest_color;
		}
	}

	#endif
	InterpolationPaletteChanged = FALSE;
	return;

}









/***********************************************************************************************
 * Increase_Palette_Luminance -- increase contrast of colours in a palette                     *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to palette                                                                    *
 *           percentage increase of red                                                        *
 *           percentage increase of green                                                      *
 *           percentage increase of blue                                                       *
 *           cap value for colours                                                             *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    12/12/95 12:16PM ST : Created                                                            *
 *=============================================================================================*/

void Increase_Palette_Luminance (unsigned char *palette , int red_percentage , int green_percentage , int blue_percentage ,int cap)
{

	unsigned	int	red;
	unsigned	int	green;
	unsigned	int	blue;
	for (int i=0 ; i<SIZE_OF_PALETTE*3 ; i+=3){

		red 	= (unsigned)*(palette+i);
		green = (unsigned)*(palette+i+1);
		blue	= (unsigned)*(palette+i+2);

		red 	+= red*red_percentage/100;
		green	+= green*green_percentage/100;
		blue	+= blue*blue_percentage/100;

		red 	= MIN (cap,(int)red);
		green	= MIN (cap,(int)green);
		blue	= MIN (cap,(int)blue);

		*(palette+i)	=(unsigned char)red;
		*(palette+i+1)	=(unsigned char)green;
		*(palette+i+2)	=(unsigned char)blue;

	}

}












int	CopyType	=0;

#if( 1 )
/***************************************************************************
 * INTERPOLATE_2X_SCALE                                                    *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/06/1995  MG : Created.                                             *
 *=========================================================================*/
void Interpolate_2X_Scale( GraphicBufferClass *source, GraphicViewPortClass *dest ,char const *palette_file_name)
{
//Don't think we need this. ST - 12/20/2018 2:25PM
#if (0)	
	unsigned char	*src_ptr;
	unsigned char	*dest_ptr;
	unsigned char	*last_dest_ptr;
	unsigned char	*end_of_source;
	int	src_width;
	int	dest_width;
//	int	width_counter;
	BOOL	source_locked = FALSE;
	BOOL	dest_locked = FALSE;


	/*
	**If a palette table exists on disk then read it in otherwise create it
	*/
	if (InterpolationPaletteChanged){
		if (palette_file_name){
			Read_Interpolation_Palette(palette_file_name);
		}
		if (InterpolationPaletteChanged){
			Create_Palette_Interpolation_Table();
		}
	}

	/*
	** Write the palette table to disk so we dont have to create it again next time
	*/
	if (palette_file_name){
		Write_Interpolation_Palette(palette_file_name);
	}
	if ( dest == &SeenBuff ) Hide_Mouse();

	Wait_Blit();
	/*
	** Lock video surfaces if requred
	*/
	if (source->Get_IsDirectDraw()){
		if (!source->Lock()){
			if (dest == &SeenBuff) Show_Mouse();
			return;
		}
		source_locked = TRUE;
	}
	if (dest->Get_IsDirectDraw()){
		if (!dest->Lock()) {
			if (source_locked){
				source->Unlock();
			}
			if (dest == &SeenBuff) Show_Mouse();
			return;
		}
		dest_locked = TRUE;
	}


	//
	// Get pointers to the source and destination buffers.
	//
	src_ptr = (unsigned char *) source->Get_Offset();
	dest_ptr = (unsigned char *) dest->Get_Offset();
	end_of_source = src_ptr + ( source->Get_Width() * source->Get_Height() );

	//
	// Get width of source and dest buffers.
	//
	src_width = source->Get_Width();
	dest_width = 2*(dest->Get_Width() + dest->Get_XAdd() + dest->Get_Pitch());
	last_dest_ptr = dest_ptr;

	/*
	** Call the appropriate assembly language copy routine
	*/
#if (1)
	switch (CopyType){
		case 0:
			Asm_Interpolate ( src_ptr , dest_ptr , source->Get_Height() , src_width , dest_width);
			break;

		case 1:
			Asm_Interpolate_Line_Double( src_ptr , dest_ptr , source->Get_Height() , src_width , dest_width);
			break;

		case 2:
			Asm_Interpolate_Line_Interpolate( src_ptr , dest_ptr , source->Get_Height() , src_width , dest_width);
			break;
	}
#endif

#if (0)
	//
	// Copy over the first pixel (upper left).
	//
	*dest_ptr = *src_ptr;
	src_ptr ++;
	dest_ptr ++;

	//
	// Scale copy.
	//
	width_counter = 0;
	while ( src_ptr < end_of_source ) {

		//
		// Blend this pixel with the one to the left and place this new color in the dest buffer.
		//
		*dest_ptr = PaletteInterpolationTable[ (*src_ptr) ][ (*( src_ptr - 1 )) ];
		dest_ptr ++;

		//
		// Now place the source pixel into the dest buffer.
		//
		*dest_ptr = *src_ptr;

		src_ptr ++;
		dest_ptr ++;

		width_counter ++;
		if ( width_counter == src_width ) {
			width_counter = 0;
			last_dest_ptr += dest_width;
			dest_ptr = last_dest_ptr;
		}
	}

#endif
	if (source_locked) source->Unlock();
	if (dest_locked) dest->Unlock();
	if (dest == &SeenBuff) Show_Mouse();
#endif
}
#endif



