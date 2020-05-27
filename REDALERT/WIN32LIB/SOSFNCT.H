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

   File              : sosfnct.h

   Programmer(s)     : Don Fowler, Nick Skrepetos
   Date              :

   Purpose           : Include Files For Zortech C++ Compiler

   Last Updated      :

****************************************************************************
               Copyright(c) 1993,1994 Human Machine Interfaces
                            All Rights Reserved
****************************************************************************/

#ifndef  _SOS_FUNCTIONS
#define  _SOS_FUNCTIONS

#pragma pack(4)

WORD sosDIGILockMemory          (  VOID  );
WORD sosDIGIUnLockMemory        (  VOID  );
WORD sosDIGIInitSystem           (  LPSTR, WORD  );
WORD sosDIGIUnInitSystem         (  VOID  );
WORD sosDIGIInitDriver           (  WORD, _SOS_HARDWARE far *,
                                    _SOS_INIT_DRIVER far *, WORD far *   );
WORD sosDIGIUnInitDriver         (  WORD, BOOL, BOOL  );
WORD sosDIGILoadDriver          (  WORD, WORD, LPSTR far *, LPSTR far *, PSTR, PSTR, WORD * );
WORD sosDIGIUnLoadDriver         (  WORD  );
WORD sosDIGIGetDeviceCaps        (  WORD, LPSOSDEVICECAPS  );

#ifdef PHARLAP
LPSTR sosDIGIAllocateBuffer      (  WORD , WORD *, WORD * );
#else
LPSTR sosDIGIAllocateBuffer      (  WORD , WORD *, WORD * );
#endif

WORD sosDIGIStopSample           (  WORD, WORD  );
WORD sosDIGISamplesPlaying       (  WORD  );
BOOL sosDIGISampleDone           (  WORD, WORD  );
BOOL sosDIGISampleFilling        (  WORD, WORD  );
WORD sosDIGIStartSample          (  WORD, _SOS_START_SAMPLE far *  );
WORD sosDIGIContinueSample       (  WORD, WORD, _SOS_START_SAMPLE far * );


WORD  sosDIGIDetectInit          (  LPSTR  );
WORD  sosDIGIDetectUnInit        (  VOID  );
WORD  sosDIGIDetectFindHardware  (  WORD, _SOS_CAPABILITIES far *, WORD far * );
WORD  sosDIGIDetectFindFirst     (  _SOS_CAPABILITIES far *, WORD far *  );
WORD  sosDIGIDetectFindNext      (  _SOS_CAPABILITIES far *, WORD far *  );
WORD  sosDIGIDetectGetSettings   (  _SOS_HARDWARE far *  );
WORD  sosDIGIDetectGetCaps       (  WORD, _SOS_CAPABILITIES far *  );
WORD  sosDIGIDetectVerifySettings(  _SOS_HARDWARE far *  );
PSTR  sosGetErrorString          (  WORD  );

WORD  sosDIGILoadTimer           (  WORD , LPSTR far *, LPSTR far *, PSTR, PSTR, WORD * );
WORD  sosDIGIUnLoadTimer         (  WORD  );

WORD  sosTIMERRegisterEvent      ( WORD wCallRate, VOID ( far * lpTimerEvent )( VOID ), WORD far *lpTimerHandle );
WORD  sosTIMERInitSystem               (  WORD, WORD  );
WORD  sosTIMERUnInitSystem             (  WORD  );
WORD  sosTIMERSetRate                  (  WORD  );
WORD  sosTIMERRemoveEvent              (  WORD );
WORD  sosTIMERAlterEventRate           (  WORD, WORD  );
WORD  sosTIMERGetEventRate             (  WORD  );
VOID far sosTIMEROldHandler            (  VOID  );
VOID far sosTIMERHandler               (  VOID  );

// functions in soscntl.c
WORD  sosDIGISetSampleVolume     ( WORD, WORD, WORD );
WORD  sosDIGIGetSampleVolume     ( WORD, WORD );
WORD  sosDIGISetChannel          ( WORD, WORD, WORD );
WORD  sosDIGIGetChannel          ( WORD, WORD );
WORD  sosDIGIGetBytesProcessed   ( WORD, WORD );
WORD  sosDIGIGetLoopCount        ( WORD, WORD );
WORD  sosDIGISetPanLocation      ( WORD, WORD, WORD );
WORD  sosDIGIGetPanLocation      ( WORD, WORD );
DWORD sosDIGISetPitch            ( WORD, WORD, DWORD );
DWORD sosDIGIGetPitch            ( WORD, WORD );
WORD  sosDIGIGetDMAPosition      ( WORD );
WORD  sosDIGISetPanSpeed         ( WORD, WORD, WORD );
WORD  sosDIGIGetPanSpeed         ( WORD, WORD );
WORD  sosDIGIGetSampleID         ( WORD, WORD );
WORD  sosDIGIGetSampleHandle     ( WORD, WORD );
WORD  sosDIGISetMasterVolume     ( WORD, WORD );
#ifdef PHARLAP
VOID  sosFreeVDSPage          ( unsigned short, unsigned short, DWORD );
WORD  sosAllocVDSPage         ( unsigned short *, unsigned short *, DWORD * );
#else
WORD  sosAllocVDSPage            ( LPSTR *, WORD *, WORD * );
VOID  sosFreeVDSPage             ( WORD, WORD, LONG );
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PHARLAP
extern   int   __cdecl sosRealFree          ( int );
extern   BOOL  __cdecl  _sos_read( WORD, LPSTR, WORD, WORD * );
extern   int   __cdecl sosRealAlloc( int, int *, int * );
extern   void  __cdecl sosDRVFarMemCopy( LPSTR, LPSTR, WORD );
extern   int   __cdecl sosGetCS( VOID );
extern   int   __cdecl sosGetES( VOID );
#else
extern   int   __cdecl sosRealAlloc         ( int, int *, int * );
extern   int   __cdecl sosRealFree          ( int );
#endif

// sos driver functions
extern   WORD  __cdecl sosDRVLockMemory     ( DWORD, DWORD );
extern   WORD  __cdecl sosDRVUnLockMemory   ( DWORD, DWORD );
extern   void  __cdecl sosDRVGetCapsInfo    ( LPSTR, LPSTR, _SOS_CAPABILITIES far * );
extern   void  __cdecl sosDetDRVGetCapsInfo ( LPSTR, LPSTR, _SOS_CAPABILITIES far * );
extern   void  __cdecl sosDRVGetCapsPtr     ( LPSTR, LPSTR, _SOS_CAPABILITIES far * );
extern   void  __cdecl sosDRVInit           ( LPSTR, LPSTR, int, int, int, int, int, int );
extern   void  __cdecl sosDRVStart          ( LPSTR, LPSTR, int, int );
extern   void  __cdecl sosDRVSetRate        ( LPSTR, LPSTR, int );
extern   void  __cdecl sosDRVSetAction      ( LPSTR, LPSTR );
extern   void  __cdecl sosDRVStop           ( LPSTR, LPSTR );
extern   void  __cdecl sosDRVUnInit         ( LPSTR, LPSTR );
extern   void  __cdecl sosDRVGetFillInfo    ( LPSTR, LPSTR, LPSTR, int, int, int, _SOS_FILL_INFO * );
extern   void  __cdecl sosFillSampleStructs ( PSTR, LPSTR );
extern   WORD  __cdecl sosDetDRVExist       ( LPSTR, LPSTR );
extern   WORD  __cdecl sosDetDRVGetSettings ( LPSTR, LPSTR );
extern   WORD  __cdecl sosDetDRVVerifySettings( LPSTR, WORD, WORD, WORD, LPSTR );
extern   WORD  __cdecl sosDIGIInitForWindows( WORD );
extern   WORD  __cdecl sosDIGIUnInitForWindows( WORD );
extern   LPSTR __cdecl sosAllocateFarMem      ( WORD, PSTR, WORD * );
extern   LPSTR __cdecl sosCreateAliasCS       ( LPSTR );
extern   VOID  __cdecl sosFreeSelector        ( LPSTR, DWORD );
extern   LPSTR __cdecl sosMAKEDOSPtr          ( PSTR );
extern   VOID  __cdecl sosDetDRVSetEnvString  ( DWORD, PSTR );
extern   PSTR  __cdecl sosDetDRVGetEnvString  ( DWORD );
extern   VOID  __cdecl sosDetDRVEnvStringInit ( LPSTR, LPSTR );
extern   VOID  __cdecl sosDRVSetupCallFunctions( LPSTR, LPSTR, LPSTR, LPSTR );
extern   WORD  __cdecl sosDRVGetFreeMemory     ( VOID );
extern   WORD  __cdecl sosDRVAllocVDSStruct    ( WORD, WORD *, WORD * );
extern   WORD  __cdecl sosDRVFreeVDSStruct     ( WORD, WORD );
extern   WORD  __cdecl sosDRVIsWindowsActive   ( VOID );
extern   WORD  __cdecl sosDRVVDSGetBuffer    ( WORD );
extern   WORD  __cdecl sosDRVVDSFreeBuffer   ( WORD );
extern   WORD  __cdecl getDS( VOID );
extern   WORD  __cdecl sosDRVMakeDMASelector   ( WORD );
extern   WORD  __cdecl sosDRVFreeDMASelector   ( WORD );


extern   void  __cdecl sosTIMERDRVInit( int wRate, void ( far * )( void ) );
extern   void  __cdecl sosTIMERDRVUnInit( void );
extern   void  __cdecl sosTIMERDRVHandler( void );
extern   void  __cdecl sosTIMERDRVFHandler( void );
extern   void  __cdecl sosTIMERDRVEnable( void );
extern   void  __cdecl sosTIMERDRVDisable( void );
extern   void  __cdecl sosTIMERDRVCallOld( void );
extern   void  __cdecl sosTIMERDRVSetRate( WORD );
extern   void  __cdecl sosDIGITimer_Start( void );
extern   void  __cdecl sosDIGITimer_End( void );
extern   void  __cdecl sosDIGIDrv_Start( void );
extern   void  __cdecl sosDIGIDrv_End( void );
#ifdef __cplusplus
}
#endif

// external functions for handling system initialization and
// uninitialization
WORD  sosEXDIGInitDriver               (  WORD, WORD, WORD, LPSTR,
                                          _SOS_HARDWARE far *, WORD *  );
WORD  sosEXDIGIUnInitDriver            (  VOID  );

WORD  sosEXDETFindDriver               (  WORD, LPSTR, _SOS_HARDWARE far *,
                                          _SOS_CAPABILITIES far *  );

// memory locking prototypes
VOID  sosDIGICaps_Start( VOID );
VOID  sosDIGICaps_End( VOID );
VOID  sosDIGIErr_Start( VOID );
VOID  sosDIGIErr_End( VOID );
VOID  sosDIGITmr_Start( VOID );
VOID  sosDIGITmr_End( VOID );
VOID  sosDIGIStart_Start( VOID );
VOID  sosDIGIStart_End( VOID );
VOID  sosDIGIPlyng_Start( VOID );
VOID  sosDIGIPlyng_End( VOID );
VOID  sosDIGIRate_Start( VOID );
VOID  sosDIGIRate_End( VOID );
VOID  sosDIGIDone_Start( VOID );
VOID  sosDIGIDone_End( VOID );
VOID  sosDIGIDetec_Start( VOID );
VOID  sosDIGIDetec_End( VOID );
VOID  sosDIGIInit_Start( VOID );
VOID  sosDIGIInit_End( VOID );
VOID  sosDIGILoad_Start( VOID );
VOID  sosDIGILoad_End( VOID );
VOID  sosDIGICntl_Start( VOID );
VOID  sosDIGICntl_End( VOID );

#pragma pack()

#endif

