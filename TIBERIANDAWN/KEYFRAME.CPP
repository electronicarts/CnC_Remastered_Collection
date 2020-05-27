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

/* $Header:   F:\projects\c&c\vcs\code\keyframe.cpv   2.14   16 Oct 1995 16:48:54   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : KEYFRAME.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/25/95                                                     *
 *                                                                                             *
 *                  Last Update : June 25, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Get_Build_Frame_Count -- Fetches the number of frames in data block.                      *
 *   Get_Build_Frame_Width -- Fetches the width of the shape image.                            *
 *   Get_Build_Frame_Height -- Fetches the height of the shape image.                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "function.h"

#define SUBFRAMEOFFS			7	// 3 1/2 frame offsets loaded (2 offsets/frame)


#define	Apply_Delta(buffer, delta)		Apply_XOR_Delta((char*)(buffer), (char*)(delta))

typedef struct {
	unsigned short frames;
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
	unsigned short largest_frame_size;
	short				flags;
} KeyFrameHeaderType;

#define	INITIAL_BIG_SHAPE_BUFFER_SIZE	12000*1024
#define	THEATER_BIG_SHAPE_BUFFER_SIZE 1000*1024
#define	UNCOMPRESS_MAGIC_NUMBER			56789

unsigned	BigShapeBufferLength = INITIAL_BIG_SHAPE_BUFFER_SIZE;
unsigned	TheaterShapeBufferLength = THEATER_BIG_SHAPE_BUFFER_SIZE;
extern "C"{
	char		*BigShapeBufferStart = NULL;
	char		*TheaterShapeBufferStart = NULL;
	BOOL		UseBigShapeBuffer = FALSE;
	bool		IsTheaterShape = false;
}
char		*BigShapeBufferPtr = NULL;
int			TotalBigShapes=0;
BOOL		ReallocShapeBufferFlag = FALSE;
bool		OriginalUseBigShapeBuffer = false;

char		*TheaterShapeBufferPtr = NULL;
int			TotalTheaterShapes = 0;



#define MAX_SLOTS 1500
#define THEATER_SLOT_START 1000

char	**KeyFrameSlots [MAX_SLOTS];
int 	TotalSlotsUsed=0;
int		TheaterSlotsUsed = THEATER_SLOT_START;


typedef struct tShapeHeaderType{
	unsigned draw_flags;
	char		*shape_data;
	int		shape_buffer;		//1 if shape is in theater buffer
} ShapeHeaderType;

static int Length;

void *Get_Shape_Header_Data(void *ptr)
{
	if (UseBigShapeBuffer){

		ShapeHeaderType *header = (ShapeHeaderType*) ptr;
		return ((void*)  (header->shape_data + (long)(header->shape_buffer ? TheaterShapeBufferStart : BigShapeBufferStart) ) );

	}else{
		return (ptr);
	}
}

int Get_Last_Frame_Length(void)
{
	return(Length);
}



void Reset_Theater_Shapes (void)
{
	/*
	** Delete any previously allocated slots
	*/
	for (int i=THEATER_SLOT_START ; i<TheaterSlotsUsed ; i++){
		delete [] KeyFrameSlots [i];
	}

	TheaterShapeBufferPtr = TheaterShapeBufferStart;
	TotalTheaterShapes = 0;
	TheaterSlotsUsed = THEATER_SLOT_START;
}



void Reallocate_Big_Shape_Buffer(void)
{
	if (ReallocShapeBufferFlag){
		BigShapeBufferLength += 200 * 1024;							//Extra 2 Mb of uncompressed shape space
		BigShapeBufferPtr -= (unsigned)BigShapeBufferStart;
		Memory_Error = NULL;
		BigShapeBufferStart = (char*)Resize_Alloc(BigShapeBufferStart, BigShapeBufferLength);
		Memory_Error = &Memory_Error_Handler;
		/*
		** If we have run out of memory then disable the uncompressed shapes
		** It may still be possible to continue with compressed shapes
		*/
		if (!BigShapeBufferStart){
			UseBigShapeBuffer = false;
			return;
		}
		BigShapeBufferPtr += (unsigned)BigShapeBufferStart;
		ReallocShapeBufferFlag = FALSE;
	}
}




void Check_Use_Compressed_Shapes (void)
{
	MEMORYSTATUS	mem_info;

	mem_info.dwLength=sizeof(mem_info);
	GlobalMemoryStatus(&mem_info);

	UseBigShapeBuffer = (mem_info.dwTotalPhys > 16*1024*1024) ? TRUE : FALSE;
	OriginalUseBigShapeBuffer = UseBigShapeBuffer;

	// UseBigShapeBuffer = false;
}




/***********************************************************************************************
 * Disable_Uncompressed_Shapes -- Temporarily turns off shape decompression                    *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/19/96 2:37PM ST : Created                                                             *
 *=============================================================================================*/
void Disable_Uncompressed_Shapes (void)
{
	UseBigShapeBuffer = false;
}



/***********************************************************************************************
 * Enable_Uncompressed_Shapes -- Restores state of shape decompression before it was disabled  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/19/96 2:37PM ST : Created                                                             *
 *=============================================================================================*/
void Enable_Uncompressed_Shapes (void)
{
	UseBigShapeBuffer = OriginalUseBigShapeBuffer;
}


#define FIXIT_SCORE_CRASH

unsigned long Build_Frame(void const *dataptr, unsigned short framenumber, void *buffptr)
{
#ifdef FIXIT_SCORE_CRASH
	char * ptr;
	unsigned long offcurr, offdiff;
#else
	char * ptr, * lockptr;
	unsigned long offcurr, off16, offdiff;
#endif
	unsigned long offset[SUBFRAMEOFFS];
	KeyFrameHeaderType *keyfr;
	unsigned short buffsize, currframe, subframe;
	unsigned long length = 0;
	char frameflags;
	unsigned long return_value;
	char *temp_shape_ptr;

	//
	// valid pointer??
	//
	Length = 0;
	if ( !dataptr || !buffptr ) {
		return(0);
	}

	//
	// look at header then check that frame to build is not greater
	// than total frames
	//
	keyfr = (KeyFrameHeaderType *) dataptr;

	if ( framenumber >= keyfr->frames ) {
		return(0);
	}


	if (UseBigShapeBuffer){
		/*
		** If we havnt yet allocated memory for uncompressed shapes then do so now.
		**
		*/
		if (!BigShapeBufferStart){
			BigShapeBufferStart = (char*)Alloc(BigShapeBufferLength, MEM_NORMAL);
			BigShapeBufferPtr = BigShapeBufferStart;
			/*
			** Allocate memory for theater specific uncompressed shapes
			*/
			TheaterShapeBufferStart = (char*) Alloc (TheaterShapeBufferLength, MEM_NORMAL);
			TheaterShapeBufferPtr = TheaterShapeBufferStart;
		}


		/*
		** Track memory usage in uncompressed shape buffers.
		*/
		static bool show_info = true;

		if ((Frame & 0xff) == 0){

			if (show_info){

				char debugstr [128];
				sprintf (debugstr, "C&C95 - Big shape buffer is now %d Kb.\n", BigShapeBufferLength / 1024);
				CCDebugString (debugstr);

				sprintf (debugstr, "C&C95 - %d Kb Used in big shape buffer.\n", (unsigned)((unsigned)BigShapeBufferPtr - (unsigned)BigShapeBufferStart)/1024);
				CCDebugString (debugstr);

				sprintf (debugstr, "C&C95 - %d Kb Used in theater shape buffer.\n", (unsigned)((unsigned)TheaterShapeBufferPtr - (unsigned)TheaterShapeBufferStart)/1024);
				CCDebugString (debugstr);
				show_info = false;
			}

			}else{
			show_info = true;

		}


		/*
		** If we are running out of memory (<128k left) for uncompressed shapes
		** then allocate some more.
		*/
		if (( (unsigned)BigShapeBufferStart + BigShapeBufferLength) - (unsigned)BigShapeBufferPtr < 128*1024){
			ReallocShapeBufferFlag = TRUE;
		}

		/*
		** If this animation was not previously uncompressed then
		** allocate memory to keep the pointers to the uncompressed data
		** for these animation frames
		*/
		if (keyfr->x != UNCOMPRESS_MAGIC_NUMBER){
			keyfr->x = UNCOMPRESS_MAGIC_NUMBER;
			if (IsTheaterShape){
				keyfr->y = TheaterSlotsUsed;
				TheaterSlotsUsed++;
			}else{
				keyfr->y = TotalSlotsUsed;
				TotalSlotsUsed++;
			}
			/*
			** Allocate and clear the memory for the shape info
			*/
			KeyFrameSlots[keyfr->y]= new char *[keyfr->frames];
			memset (KeyFrameSlots[keyfr->y] , 0 , keyfr->frames*4);
		}

		/*
		** If this frame was previously uncompressed then just return
		** a pointer to the raw data
		*/
		if (*(KeyFrameSlots[keyfr->y]+framenumber)){
			if (IsTheaterShape){
				return ((unsigned long)TheaterShapeBufferStart + (unsigned long)*(KeyFrameSlots[keyfr->y]+framenumber));
			}else{
				return ((unsigned long)BigShapeBufferStart + (unsigned long)*(KeyFrameSlots[keyfr->y]+framenumber));
			}
		}
	}

	// calc buff size
	buffsize = keyfr->width * keyfr->height;

	// get offset into data
	ptr = (char *)Add_Long_To_Pointer( dataptr, (((unsigned long)framenumber << 3) + sizeof(KeyFrameHeaderType)) );
	Mem_Copy( ptr, &offset[0], 12L );
	frameflags = (char)(offset[0] >> 24);


	if ( (frameflags & KF_KEYFRAME) ) {

		ptr = (char *)Add_Long_To_Pointer( dataptr, (offset[0] & 0x00FFFFFFL) );

		if (keyfr->flags & 1 ) {
			ptr = (char *)Add_Long_To_Pointer( ptr, 768L );
		}
		length = LCW_Uncompress( ptr, buffptr, buffsize );
	} else {	// key delta or delta

		if ( (frameflags & KF_DELTA) ) {
			currframe = (unsigned short)offset[1];

			ptr = (char *)Add_Long_To_Pointer( dataptr, (((unsigned long)currframe << 3) + sizeof(KeyFrameHeaderType)) );
			Mem_Copy( ptr, &offset[0], (long)(SUBFRAMEOFFS * sizeof(unsigned long)) );
		}

		// key frame
		offcurr = offset[1] & 0x00FFFFFFL;

		// key delta
		offdiff = (offset[0] & 0x00FFFFFFL) - offcurr;

		ptr = (char *)Add_Long_To_Pointer( dataptr, offcurr );

		if (keyfr->flags & 1 ) {
			ptr = (char *)Add_Long_To_Pointer( ptr, 768L );
		}

#ifndef FIXIT_SCORE_CRASH
		off16 = (unsigned long)lockptr & 0x00003FFFL;
#endif
		length = LCW_Uncompress( ptr, buffptr, buffsize );

		if (length > buffsize) {
			return(0);
		}

#ifndef FIXIT_SCORE_CRASH
		if ( ((offset[2] & 0x00FFFFFFL) - offcurr) >= (0x00010000L - off16) ) {

			ptr = (char *)Add_Long_To_Pointer( ptr, offdiff );
			off16 = (unsigned long)ptr & 0x00003FFFL;

			offcurr += offdiff;
			offdiff = 0;
		}
#endif
		length = buffsize;
		Apply_Delta(buffptr, Add_Long_To_Pointer(ptr, offdiff));

		if ( (frameflags & KF_DELTA) ) {
			// adjust to delta after the keydelta

			currframe++;
			subframe = 2;

			while (currframe <= framenumber) {
				offdiff = (offset[subframe] & 0x00FFFFFFL) - offcurr;

#ifndef FIXIT_SCORE_CRASH
				if ( ((offset[subframe+2] & 0x00FFFFFFL) - offcurr) >= (0x00010000L - off16) ) {

					ptr = (char *)Add_Long_To_Pointer( ptr, offdiff );
					off16 = (unsigned long)lockptr & 0x00003FFFL;

					offcurr += offdiff;
					offdiff = 0;
				}
#endif

				length = buffsize;
				Apply_Delta(buffptr, Add_Long_To_Pointer(ptr, offdiff));

				currframe++;
				subframe += 2;

				if ( subframe >= (SUBFRAMEOFFS - 1) &&
					currframe <= framenumber ) {
					Mem_Copy( Add_Long_To_Pointer( dataptr,
									(((unsigned long)currframe << 3) +
									sizeof(KeyFrameHeaderType)) ),
						&offset[0], (long)(SUBFRAMEOFFS * sizeof(unsigned long)) );
					subframe = 0;
				}
			}
		}
	}

	if (UseBigShapeBuffer){
		/*
		** Save the uncompressed shape data so we dont have to uncompress it
		** again next time its drawn.
		** We keep a space free before the raw shape data so we can add line
		** header info before the shape is drawn for the first time
		*/

		if (IsTheaterShape){
			/*
			** Shape is a theater specific shape
			*/
			return_value = (unsigned long) TheaterShapeBufferPtr;
			temp_shape_ptr = TheaterShapeBufferPtr + keyfr->height+sizeof(ShapeHeaderType);
			/*
			** align the actual shape data
			*/
			if (3 & (unsigned)temp_shape_ptr){
				temp_shape_ptr = (char *) ((unsigned)(temp_shape_ptr + 3) & 0xfffffffc);
			}

			memcpy (temp_shape_ptr , buffptr , length);
			((ShapeHeaderType *)TheaterShapeBufferPtr)->draw_flags = -1;						//Flag that headers need to be generated
			((ShapeHeaderType *)TheaterShapeBufferPtr)->shape_data = temp_shape_ptr - (unsigned)TheaterShapeBufferStart;		//pointer to old raw shape data
			((ShapeHeaderType *)TheaterShapeBufferPtr)->shape_buffer = 1;	//Theater buffer
			*(KeyFrameSlots[keyfr->y]+framenumber) = TheaterShapeBufferPtr - (unsigned)TheaterShapeBufferStart;
			TheaterShapeBufferPtr = (char*)(length + (unsigned)temp_shape_ptr);
			/*
			** Align the next shape
			*/
			if (3 & (unsigned)TheaterShapeBufferPtr){
				TheaterShapeBufferPtr = (char *)((unsigned)(TheaterShapeBufferPtr + 3) & 0xfffffffc);
			}
			Length = length;
			return (return_value);

		}else{


			return_value=(unsigned long)BigShapeBufferPtr;
			temp_shape_ptr = BigShapeBufferPtr + keyfr->height+sizeof(ShapeHeaderType);
			/*
			** align the actual shape data
			*/
			if (3 & (unsigned)temp_shape_ptr){
				temp_shape_ptr = (char *) ((unsigned)(temp_shape_ptr + 3) & 0xfffffffc);
			}
			memcpy (temp_shape_ptr , buffptr , length);
			((ShapeHeaderType *)BigShapeBufferPtr)->draw_flags = -1;						//Flag that headers need to be generated
			((ShapeHeaderType *)BigShapeBufferPtr)->shape_data = temp_shape_ptr - (unsigned)BigShapeBufferStart;		//pointer to old raw shape data
			((ShapeHeaderType *)BigShapeBufferPtr)->shape_buffer = 0;	//Normal Big Shape Buffer
			*(KeyFrameSlots[keyfr->y]+framenumber) = BigShapeBufferPtr - (unsigned)BigShapeBufferStart;
			BigShapeBufferPtr = (char*)(length + (unsigned)temp_shape_ptr);
			// Align the next shape
			if (3 & (unsigned)BigShapeBufferPtr){
				BigShapeBufferPtr = (char *)((unsigned)(BigShapeBufferPtr + 3) & 0xfffffffc);
			}
			Length = length;
			return (return_value);
		}

	}else{
		return ((unsigned long)buffptr);
	}
}


/***********************************************************************************************
 * Get_Build_Frame_Count -- Fetches the number of frames in data block.                        *
 *                                                                                             *
 *    Use this routine to determine the number of shapes within the data block.                *
 *                                                                                             *
 * INPUT:   dataptr  -- Pointer to the keyframe shape data block.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the number of shapes in the data block.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Commented.                                                               *
 *=============================================================================================*/
unsigned short Get_Build_Frame_Count(void const *dataptr)
{
	if (dataptr) {
		return(((KeyFrameHeaderType const *)dataptr)->frames);
	}
	return(0);
}


unsigned short Get_Build_Frame_X(void const *dataptr)
{
	if (dataptr) {
		return(((KeyFrameHeaderType const *)dataptr)->x);
	}
	return(0);
}


unsigned short Get_Build_Frame_Y(void const *dataptr)
{
	if (dataptr) {
		return(((KeyFrameHeaderType const *)dataptr)->y);
	}
	return(0);
}


/***********************************************************************************************
 * Get_Build_Frame_Width -- Fetches the width of the shape image.                              *
 *                                                                                             *
 *    Use this routine to fetch the width of the shapes within the keyframe shape data block.  *
 *    All shapes within the block have the same width.                                         *
 *                                                                                             *
 * INPUT:   dataptr  -- Pointer to the keyframe shape data block.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the width of the shapes in the block -- expressed in pixels.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Commented                                                                *
 *=============================================================================================*/
unsigned short Get_Build_Frame_Width(void const *dataptr)
{
	if (dataptr) {
		return(((KeyFrameHeaderType const *)dataptr)->width);
	}
	return(0);
}


/***********************************************************************************************
 * Get_Build_Frame_Height -- Fetches the height of the shape image.                            *
 *                                                                                             *
 *    Use this routine to fetch the height of the shapes within the keyframe shape data block. *
 *    All shapes within the block have the same height.                                        *
 *                                                                                             *
 * INPUT:   dataptr  -- Pointer to the keyframe shape data block.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the height of the shapes in the block -- expressed in pixels.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Commented                                                                *
 *=============================================================================================*/
unsigned short Get_Build_Frame_Height(void const *dataptr)
{
	if (dataptr) {
		return(((KeyFrameHeaderType const *)dataptr)->height);
	}
	return(0);
}


bool Get_Build_Frame_Palette(void const * dataptr, void * palette)
{
	if (dataptr && (((KeyFrameHeaderType const *)dataptr)->flags & 1)) {
		char const * ptr = (char const *)Add_Long_To_Pointer( dataptr,
							( (( (long)sizeof(unsigned long) << 1) *
								((KeyFrameHeaderType *) dataptr)->frames ) +
							16 + sizeof(KeyFrameHeaderType) ) );

		memcpy(palette, ptr, 768L);
		return(true);
	}
	return(false);
}