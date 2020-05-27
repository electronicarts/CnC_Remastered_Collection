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
 **      C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S      **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Westwood 32 bit Library                  *
 *                                                                         *
 *                    File Name : SOUNDINT.H                               *
 *                                                                         *
 *                   Programmer : Phil W. Gorrow                           *
 *                                                                         *
 *                   Start Date : June 23, 1995                            *
 *                                                                         *
 *                  Last Update : June 23, 1995   [PWG]                    *
 *                                                                         *
 * This file is the include file for the Westwood Sound Sytem defines and  *
 * routines that are handled in an interrupt.
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "sound.h"

/*
** Defines for true and false.  These are included because we do not allow
** the sound int to include any of the westwood standard headers.  If we
** did, there might be too much temptation to call another library function.
** this would be bad, because then that function would not be locked.
*/
#define	FALSE		0
#define	TRUE		1

/*
** Define the different type of sound compression avaliable to the westwood
** library.
*/
typedef enum {
	SCOMP_NONE=0,			// No compression -- raw data.
	SCOMP_WESTWOOD=1,		// Special sliding window delta compression.
	SCOMP_SONARC=33,		// Sonarc frame compression.
	SCOMP_SOS=99			// SOS frame compression.
} SCompressType;

/*
**	This is the safety overrun margin for the sonarc compressed
** data frames.  This value should be equal the maximum 'order' times
**	the maximum number of bytes per sample.  It should be evenly divisible
**	by 16 to aid paragraph alignment.
*/
#define	SONARC_MARGIN				32


/*
** Define the sample control structure which helps us to handle feeding
** data to the sound interrupt.
*/
#pragma pack(1);
typedef struct {
	/*
	**	This flags whether this sample structure is active or not.
	*/
	unsigned Active;
	//unsigned Active:1;

	/*
	**	This flags whether the sample is loading or has been started.
	*/
	//unsigned Loading:1;
	unsigned Loading;

	/*
	**	This semaphore ensures that simultaneous update of this structure won't
	**	occur.  This is necessary since both interrupt and regular code can modify
	**	this structure.
	*/
	//unsigned DontTouch:1;
	unsigned DontTouch;

	/*
	**	If this sample is really to be considered a score rather than
	**	a sound effect, then special rules apply.  These largely fall into
	**	the area of volume control.
	*/
	//unsigned IsScore:1;
	unsigned IsScore;

	/*
	**	This is the original sample pointer. It is used to control the sample based on
	**	pointer rather than handle. The handle method is necessary when more than one
	**	sample could be playing simultaneously. The pointer method is necessary when
	**	the dealing with a sample that may have stopped behind the programmer's back and
	**	this occurance is not otherwise determinable.  It is also used in
	** conjunction with original size to unlock a sample which has been DPMI
	** locked.
	*/
	void const *Original;
	long OriginalSize;

	/*
	**	These are pointers to the double buffers.
	*/
	LPDIRECTSOUNDBUFFER PlayBuffer;

	/*
	** Variable to keep track of the playback rate of this buffer
	*/
	int	PlaybackRate;

	/*
	** Variable to keep track of the sample type ( 8 or 16 bit ) of this buffer
	*/
	int	BitSize;

	/*
	** Variable to keep track of the stereo ability of this buffer
	*/
	int	Stereo;

	/*
	**	The number of bytes in the buffer that has been filled but is not
	**	yet playing.  This value is normally the size of the buffer,
	**	except for the case of the last bit of the sample.
	*/
	LONG DataLength;

	/*
	**	This is the buffer index for the low buffer that
	**	has been filled with data but not yet being
	**	played.
	*/
//	short int Index;

	/*
	**	Pointer into the play buffer for writing the next
	**  chunk of sample to
	**
	*/
	VOID *DestPtr;

	/*
	**	This flag indicates that there is more source data
	**  to copy to the play buffer
	**
	*/
	BOOL MoreSource;

	/*
	**	This flag indicates that the entire sample fitted inside the
	** direct sound secondary buffer
	**
	*/
	BOOL OneShot;

	/*
	**	Pointer to the sound data that has not yet been copied
	**	to the playback buffers.
	*/
	VOID *Source;

	/*
	**	This is the number of bytes remaining in the source data as
	**	pointed to by the "Source" element.
	*/
	LONG Remainder;

	/*
	**	Object to use with Enter/LeaveCriticalSection
	**
	*/
	CRITICAL_SECTION AudioCriticalSection;

	/*
	**	Samples maintain a priority which is used to determine
	**	which sounds live or die when the maximum number of
	**	sounds are being played.
	*/
	int Priority;

	/*
	**	This is the handle as returned by sosDIGIStartSample function.
	*/
	short int Handle;

	/*
	**	This is the current volume of the sample as it is being played.
	*/
	int Volume;
	int Reducer;		// Amount to reduce volume per tick.

	/*
	**	This is the compression that the sound data is using.
	*/
	SCompressType Compression;
	short int TrailerLen;						// Number of trailer bytes in buffer.
	BYTE Trailer[SONARC_MARGIN];		// Maximum number of 'order' samples needed.


	DWORD Pitch;
	WORD Flags;

	/*
	**	This flag indicates whether this sample needs servicing.
	**	Servicing entails filling one of the empty low buffers.
	*/
	short int Service;

	/*
	**	This flag is TRUE when the sample has stopped playing,
	**	BUT there is more data available.  The sample must be
	**	restarted upon filling the low buffer.
	*/
	BOOL Restart;

	/*
	**	Streaming control handlers.
	*/
	BOOL (*Callback)(short int id, short int *odd, VOID **buffer, LONG *size);
	VOID	*QueueBuffer;	// Pointer to continued sample data.
	LONG	QueueSize;		// Size of queue buffer attached.
	short int	Odd;				// Block number tracker (0..StreamBufferCount-1).
	int	FilePending;	// Number of buffers already filled ahead.
	long	FilePendingSize;	// Number of bytes in last filled buffer.

	/*
	**	The file variables are used when streaming directly off of the
	**	hard drive.
	*/
	int	FileHandle;		// Streaming file handle (ERROR = not in use).
	VOID	*FileBuffer;	// Temporary streaming buffer (allowed to be freed).
	/*
	** The following structure is used if the sample if compressed using
	** the sos 16 bit compression Codec.
	*/

	_SOS_COMPRESS_INFO sosinfo;


} SampleTrackerType;


typedef struct LockedData {
	unsigned int 		DigiHandle; 			// = -1;
	BOOL 					ServiceSomething;		// = FALSE;
	long 					MagicNumber; 			// = 0xDEAF;
	VOID 					*UncompBuffer;			// = NULL;
	long 					StreamBufferSize; 	// = (2*SECONDARY_BUFFER_SIZE)+128;
	short 				StreamBufferCount; 	// = 32;
	SampleTrackerType SampleTracker[MAX_SFX];
	unsigned int		SoundVolume;
	unsigned int		ScoreVolume;
	BOOL					_int;
} LockedDataType;

extern LockedDataType LockedData;
#pragma pack(4);

void Init_Locked_Data(void);
long Simple_Copy(void ** source, long * ssize, void ** alternate, long * altsize, void **dest, long size);
long Sample_Copy(SampleTrackerType *st, void ** source, long * ssize, void ** alternate, long * altsize, void * dest, long size, SCompressType scomp, void * trailer, short int *trailersize);
VOID far __cdecl maintenance_callback(VOID);
VOID __cdecl far DigiCallback(unsigned int driverhandle, unsigned int callsource, unsigned int sampleid);
void far HMI_TimerCallback(void);
void *Audio_Add_Long_To_Pointer(void const *ptr, long size);
void	DPMI_Unlock(VOID const *ptr, long const size);
extern "C" {
	void __cdecl Audio_Mem_Set(void const *ptr, unsigned char value, long size);
//	void	Mem_Copy(void *source, void *dest, unsigned long bytes_to_copy);
	long  __cdecl Decompress_Frame(void * source, void * dest, long size);
	int	__cdecl Decompress_Frame_Lock(void);
	int	__cdecl Decompress_Frame_Unlock(void);
	int	__cdecl sosCODEC_Lock(void);
	int	__cdecl sosCODEC_Unlock(void);
	void	__GETDS(void);
}
