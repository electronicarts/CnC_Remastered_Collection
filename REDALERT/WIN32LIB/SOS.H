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

/****************************************************************************

   File              : sos.h

   Programmer(s)     : Don Fowler, Nick Skrepetos
   Date              :

   Purpose           : Include Files For Zortech C++ Compiler

   Last Updated      :

****************************************************************************
               Copyright(c) 1993,1994 Human Machine Interfaces
                            All Rights Reserved
****************************************************************************/

#ifndef  _SOS_DEFINED
#define  _SOS_DEFINED
#include "sosdefs.h"

#pragma pack(4)

// error definition for sound operating system
#define  _SOS_ERR          -1

// number of drivers allowed to be open at one time
#define  _SOS_MAX_DRIVERS  5

// structure definition for the capabilities
typedef struct _tagCAPABILITIES
{
   BYTE  szDeviceName[ 32 ];  // device name
   WORD  wDeviceVersion;      // device version
   WORD  wBitsPerSample;      // bits per sound sample
   WORD  wChannels;           // stereo/mono sound card
   WORD  wMinRate;            // minimum rate
   WORD  wMaxRate;            // maximum rate
   WORD  wMixerOnBoard;       // board contains mixer
   WORD  wMixerFlags;         // mixer capabilities
   WORD  wFlags;              // miscellaneous flags
   short far * lpPortList;         // list of usable ports
   short far * lpDMAList;          // list of usable dma channels
   short far * lpIRQList;          // list of usable irq channels
   short far * lpRateList;         // list of usable rates, -1 if any in min to max
   WORD  fBackground;         // foreground or background driver
   WORD  wDeviceID;           // ID for the device
   WORD  wTimerID;            // ID for the timer

} _SOS_CAPABILITIES;

// far pointer to the device capabilities structure
typedef _SOS_CAPABILITIES far *  LPSOSDEVICECAPS;

// flag types for driver
#define  _FLAGS_SIGNED              0x8000

// devices that can be loaded
#define  _SOUND_BLASTER_8_MONO      0xe000
#define  _SOUND_BLASTER_8_ST        0xe001
#define  _SBPRO_8_ST                _SOUND_BLASTER_8_ST
#define  _SBPRO_8_MONO              0xe00f
#define  _SOUND_MASTER_II_8_MONO    0xe002
#define  _MV_PAS_8_MONO             0xe003
#define  _MV_PAS_16_MONO            0xe004
#define  _MV_PAS_8_ST               0xe005
#define  _MV_PAS_16_ST              0xe006
#define  _ADLIB_GOLD_8_ST           0xe007
#define  _ADLIB_GOLD_16_ST          0xe008
#define  _ADLIB_GOLD_8_MONO         0xe009
#define  _ADLIB_GOLD_16_MONO        0xe00a
#define  _MICROSOFT_8_MONO          0xe00b
#define  _MICROSOFT_8_ST            0xe00c
#define  _MICROSOFT_16_MONO         0xe00d
#define  _MICROSOFT_16_ST           0xe00e
#define  _SOUND_SOURCE_8_MONO_PC    0xe010
#define  _SOUND_SOURCE_8_MONO_TANDY 0xe011
#define  _GENERAL_PORT_8_MONO       0xe012
#define  _GENERAL_PORT_8_MONO_R     0xe013
#define  _SIERRA_8_MONO             0xe014
#define  _SB16_8_MONO               0xe015
#define  _SB16_8_ST                 0xe016
#define  _SB16_16_MONO              0xe017
#define  _SB16_16_ST                0xe018
#define  _ESS_AUDIODRIVE_8_MONO     0xe019
#define  _ESS_AUDIODRIVE_8_ST       0xe01a
#define  _ESS_AUDIODRIVE_16_MONO    0xe01b
#define  _ESS_AUDIODRIVE_16_ST      0xe01c
#define  _SOUNDSCAPE_8_MONO         0xe01d
#define  _SOUNDSCAPE_8_ST           0xe01e
#define  _SOUNDSCAPE_16_MONO        0xe01f
#define  _SOUNDSCAPE_16_ST          0xe020
#define  _RAP10_8_MONO              0xe021
#define  _RAP10_16_MONO             0xe022
#define  _GUS_8_MONO                0xe023
#define  _GUS_8_ST                  0xe024
#define  _GUS_16_MONO               0xe025
#define  _GUS_16_ST                 0xe026
#define  _GUS_MAX_8_MONO            0xe027
#define  _GUS_MAX_8_ST              0xe028
#define  _GUS_MAX_16_MONO           0xe029
#define  _GUS_MAX_16_ST             0xe02a
#define  _WAVEJAMMER_8_MONO         0xe02b
#define  _WAVEJAMMER_8_ST           0xe02c
#define  _WAVEJAMMER_16_MONO        0xe02d
#define  _WAVEJAMMER_16_ST          0xe02e
#define  _TEMPOCS_8_MONO            0xe02f
#define  _TEMPOCS_8_ST              0xe030
#define  _TEMPOCS_16_MONO           0xe031
#define  _TEMPOCS_16_ST             0xe032
#define  _WAVEJAMMERCD_8_MONO       0xe033
#define  _WAVEJAMMERCD_8_ST         0xe034
#define  _WAVEJAMMERCD_16_MONO      0xe035
#define  _WAVEJAMMERCD_16_ST        0xe036
#define  _SOUND_BLASTER_8_MONO_R    0xe050
#define  _MICROSOFT_8_MONO_R        0xe051
#define  _SOUND_MASTER_II_8_MONO_R  0xe052
#define  _ADLIB_GOLD_8_MONO_R       0xe053
#define  _MV_PAS_8_MONO_R           0xe054
#define  _RAP10_8_MONO_R            0xe058
#define  _RAP10_16_MONO_R           0xe059
#define  _SB16_8_MONO_R             0xe05a
#define  _SB16_8_ST_R               0xe05b
#define  _SB16_16_MONO_R            0xe05c
#define  _SB16_16_ST_R              0xe05d
#define  _MV_PAS_16_MONO_R          0xe060
#define  _SOUNDSCAPE_8_MONO_R       0xe061
#define  _SOUNDSCAPE_8_ST_R         0xe062
#define  _SOUNDSCAPE_16_MONO_R      0xe063
#define  _SOUNDSCAPE_16_ST_R        0xe064
#define  _ESS_AUDIODRIVE_8_MONO_R   0xe065
#define  _ESS_AUDIODRIVE_8_ST_R     0xe066
#define  _ESS_AUDIODRIVE_16_MONO_R  0xe067
#define  _ESS_AUDIODRIVE_16_ST_R    0xe068
#define  _SPEECH_THING_8_MONO       0xe090
#define  _YAMAHA_8_MONO             0xe106
#define  _INT_SPEAKER_8_MONO        0xe107

// call indexes for the loadable drivers
enum
{
   _DRV_INIT,
   _DRV_UNINIT,
   _DRV_SETRATE,
   _DRV_SETACTION,
   _DRV_START,
   _DRV_STOP,
   _DRV_PAUSE,
   _DRV_RESUME,
   _DRV_CAPABILITIES,
   _DRV_PLAY_FOREGROUND,
   _DRV_GET_FILL_INFO,
   _DRV_GET_CALL_FUNCTIONS,
   _DRV_SET_CALL_FUNCTIONS
};

// fill info
typedef  struct   _tagFillInfo
         {

            LPSTR lpFillHandler;                   // pointer to fill handler
            LPWORD lpDMAFillCount;                 // pointer to dma count
            LPSTR lpSampleList;                    // pointer to sample list
            LPWORD lpDMAMasterVolume;              // pointer to dma count

         } _SOS_FILL_INFO;

// caps info structure
typedef  struct   _tagCapsInfo
         {

            LPSTR    lpPortList;                   // pointer to port list
            LPSTR    lpDMAList;                    // pointer to DMA list
            LPSTR    lpIRQList;                    // pointer to IRQ list
            LPSTR    lpRateList;                   // pointer to rate list

         } _SOS_CAPS_INFO;

// maximum number of available voice
#define  _MAX_VOICES    32

// structure definition
typedef  struct   _tagSAMPLE
{
   LPSTR    samplePtr;                 // pointer to data buffer
   LPSTR    sampleData;                // pointer to active data
   LPSTR    sampleLoopPtr;             // pointer for loop back

   WORD     sampleLength;              // length of sample
   WORD     sampleIndex;               // index into sample
   WORD     sampleLoopLength;          // length of loop

   WORD     sampleBytesLeft;           // bytes left to play in sample

   WORD     sampleLoopPoint;           // byte count for loop point
   WORD     sampleLoopEndLength;       // length of remaining chunk

   short    sampleFlags;               // control sample
   short    sampleVolume;              // volume control
   short    sampleID;                  // sample ID

   short    sampleChannel;             // channel to play sample on
   short    sampleLoopCount;           // loop count
   short    sampleLastFill;            // last fill position
   VOID ( far __cdecl * sampleCallback )( WORD, WORD, WORD ); // callback function for sample

   WORD     samplePitchAdd;
   short    samplePitchFraction;

   short    samplePort;                // port to use for non-dma digitized

   WORD     sampleTotalBytes;
   WORD     sampleByteLength;

   short    samplePanLocation;
   short    samplePanSpeed;
   short    samplePanDirection;
   short    samplePanStart;
   short    samplePanEnd;

   short    sampleDelayBytes;
   short    sampleDelayRepeat;

   WORD     sampleADPCMPredicted;
   short    sampleADPCMIndex;

   short    sampleRootNoteMIDI;

   WORD  sampleTemp1;

} _SOS_SAMPLE;

// enumeration for left or right channel
enum
{
   _LEFT_CHANNEL,
   _RIGHT_CHANNEL,
   _CENTER_CHANNEL,
   _INTERLEAVED
};

// enumeration for foreground and background
enum
{
   _FOREGROUND,
   _BACKGROUND
};

// defines for the sample flags
#define  _ACTIVE           0x8000
#define  _LOOPING          0x4000
#define  _FIRST_TIME       0x2000
#define  _PENDING_RELEASE  0x1000
#define  _CONTINUE_BLOCK   0x0800
#define  _PITCH_SHIFT      0x0400
#define  _PANNING          0x0200
#define  _VOLUME           0x0100
#define  _TRANSLATE16TO8   0x0080
#define  _STAGE_LOOP       0x0040
#define  _TRANSLATE8TO16   0x0020
#define  _STEREOTOMONO     0x0010

// defines for the wParam flags
#define  _SINGLE_SAMPLE 0x01

#define  _SOS_DCAPS_AUTO_REINIT     0x01
#define  _SOS_DCAPS_MPU_401         0x02
#define  _SOS_DCAPS_OPL2            0x04
#define  _SOS_DCAPS_OPL3            0x08
#define  _SOS_DCAPS_OPL4            0x10
#define  _SOS_DCAPS_WAVETABLE       0x20
#define  _SOS_DCAPS_DL_SAMPLES      0x40
#define  _SOS_DCAPS_FIFO_DEVICE     0x80
#define  _SOS_DCAPS_ENV_NEEDED      0x100
#define  _SOS_DCAPS_PSEUDO_DMA1     0x200
#define  _SOS_DCAPS_SIGNED_DATA     0x8000

// file header structure
typedef struct
{
   // name ID
   BYTE  szName[ 32 ];

   // number of drivers in the file
   WORD  wDrivers;

   // offset of first driver
   WORD  lOffset;

   // size of the file
   WORD  lFileSize;

} _FILEHEADER;

// driver header structure
typedef struct
{
   // name ID
   BYTE  szName[ 32 ];

   // offset of next driver
   WORD  lNextDriver;

   // size of current driver
   WORD  wSize;

   // id for the current device
   WORD  wDeviceID;

   // id for the type of DOS extender
   WORD  wExtenderType;

} _DRIVERHEADER;

// device hardware information
typedef struct
{
   // port to be used
   WORD  wPort;

   // irq to use
   WORD  wIRQ;

   // dma channel to se
   WORD  wDMA;

   // extra parameter
   WORD  wParam;

} _SOS_HARDWARE;

// structure definition for start sample
typedef struct
{
   // pointer to sample
   LPSTR lpSamplePtr;

   // size of the sample
   WORD  dwSampleSize;

   // number of times to loop the sample -1 is infinite
   WORD  wLoopCount;

   // channel to play sample on
   WORD  wChannel;

   // volume to play sample at
   WORD wVolume;

   // id for the sample
   WORD  wSampleID;

   // far pointer to the callback function
   VOID ( far __cdecl *lpCallback )( WORD, WORD, WORD );

   // port to use if driver is a non-dma background driver
   WORD  wSamplePort;

   // flags field
   WORD  wSampleFlags;

   // total length of sample including loops, etc..
   WORD     dwSampleByteLength;

   // loop point for the sample
   WORD  dwSampleLoopPoint;
   WORD  dwSampleLoopLength;

   // pitch shifting components
   WORD     dwSamplePitchAdd;
   WORD    wSamplePitchFraction;

   // pan components
   WORD    wSamplePanLocation;
   WORD    wSamplePanSpeed;
   WORD    wSamplePanDirection;
   WORD    wSamplePanStart;
   WORD    wSamplePanEnd;

   // delay parts
   WORD    wSampleDelayBytes;
   WORD    wSampleDelayRepeat;

   // compression components
   WORD     dwSampleADPCMPredicted;
   WORD    wSampleADPCMIndex;

   // root note for pitch shifting
   WORD    wSampleRootNoteMIDI;

   // filler for future upgrades
   WORD  dwSampleTemp1;
   WORD  dwSampleTemp2;
   WORD  dwSampleTemp3;

} _SOS_START_SAMPLE;

// structure for initializing a driver
typedef struct
{
   WORD  wBufferSize;
   LPSTR lpBuffer;
   BOOL  wAllocateBuffer;
   WORD  wSampleRate;
   WORD  wParam;
   LONG  dwParam;
   VOID ( far *lpFillHandler )( VOID );
   LPSTR lpDriverMemory;
   LPSTR lpDriverMemoryCS;
   LPSTR lpTimerMemory;
   LPSTR lpTimerMemoryCS;
   WORD  wTimerID;
   WORD  wPhysical;

} _SOS_INIT_DRIVER;

// define for the timer types to use
#define  _SOS_NORMAL_TIMER          0x00

// enumeration for the timer types
enum
{
   _TIMER_8_MONO   =     0x1000,
   _TIMER_8_ST,
   _TIMER_16_MONO,
   _TIMER_16_ST,
   _TIMER_8_MONO_ULAW,
   _TIMER_8_ST_ULAW,
   _TIMER_16_MONO_ULAW,
   _TIMER_16_ST_ULAW,
   _TIMER_8_MONO_REC,
   _TIMER_8_MONO_ULAW_REC,
   _TIMER_UNDEFINED_1,
   _TIMER_UNDEFINED_2,
   _TIMER_UNDEFINED_3,
   _TIMER_UNDEFINED_4,
   _TIMER_UNDEFINED_5,
   _TIMER_UNDEFINED_6,
   _TIMER_UNDEFINED_7,
   _TIMER_UNDEFINED_8,
   _TIMER_UNDEFINED_9,
   _TIMER_UNDEFINED_A,
   _TIMER_UNDEFINED_B,
   _TIMER_UNDEFINED_C,
   _TIMER_UNDEFINED_D,
   _TIMER_UNDEFINED_E,
   _TIMER_UNDEFINED_F,
   _TIMER_UNDEFINED_10,
   _TIMER_UNDEFINED_11,
   _TIMER_UNDEFINED_12,
   _TIMER_UNDEFINED_13,
   _TIMER_UNDEFINED_14,
   _TIMER_UNDEFINED_15,
   _TIMER_UNDEFINED_16,
   _TIMER_8_SOUND_SOURCE,
   _TIMER_8_SOUND_SOURCE_TANDY,
   _TIMER_8_GENERAL_PORT,
   _TIMER_8_GENERAL_PORT_REC
};

// define for no slots available
#define  _ERR_NO_SLOTS ( WORD )-1

// error codes for the system
enum
{
   _ERR_NO_ERROR,
   _ERR_DRIVER_NOT_LOADED,
   _ERR_INVALID_POINTER,
   _ERR_DETECT_INITIALIZED,
   _ERR_FAIL_ON_FILE_OPEN,
   _ERR_MEMORY_FAIL,
   _ERR_INVALID_DRIVER_ID,
   _ERR_NO_DRIVER_FOUND,
   _ERR_DETECTION_FAILURE,
   _ERR_DRIVER_LOADED,
   _ERR_INVALID_HANDLE,
   _ERR_NO_HANDLES,
   _ERR_PAUSED,
   _ERR_NOT_PAUSED,
   _ERR_INVALID_DATA,
   _ERR_DRV_FILE_FAIL,
   _ERR_INVALID_PORT,
   _ERR_INVALID_IRQ,
   _ERR_INVALID_DMA,
   _ERR_INVALID_DMA_IRQ
};

// maximum number of timer events that can be registered
#define  _TIMER_MAX_EVENTS    0x10

// flags for the debugging system
#define  _SOS_DEBUG_NORMAL       0x0000
#define  _SOS_DEBUG_NO_TIMER     0x0001
#define  _SOS_TIMER_DPMI         0x0002

// define for types of DOS extenders
#define  _SOS_RATIONAL           0x8000
#define  _SOS_FLASHTECK          0x4000

// defines for the types of timers for different
// dos extenders
#define  _SOS_TIMER_NEAR         0x8000
#define  _SOS_TIMER_FAR          0x4000

// values for callback information
enum
{
   _SAMPLE_PROCESSED,
   _SAMPLE_LOOPING,
   _SAMPLE_DONE
};

// define for special 18.2 callback rate to dos
#define  _TIMER_DOS_RATE   0xff00

#pragma pack()

#pragma aux int_3 = "int 3"

#pragma pack( 1 )
typedef struct
{
	unsigned       region_size;
	unsigned       offset;
	unsigned       segment;
	unsigned short	number_available;
	unsigned short number_used;
	unsigned       page0;

} EVDS_STRUCT;

typedef struct
{
	unsigned       region_size;
	unsigned       offset;
	unsigned short segment;
	unsigned short	ID;
	unsigned       physical;

} VDS_STRUCT;

#pragma pack()

#include "sosdata.h"
#include "sosfnct.h"

#endif