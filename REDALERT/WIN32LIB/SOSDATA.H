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

   File              : sosdata.h

   Programmer(s)     : Don Fowler, Nick Skrepetos
   Date              :

   Purpose           : Include Files For Zortech C++ Compiler

   Last Updated      :

****************************************************************************
               Copyright(c) 1993,1994 Human Machine Interfaces
                            All Rights Reserved
****************************************************************************/

#ifndef  _SOS_DATA
#define  _SOS_DATA

#include <stddef.h>

#pragma pack(4)
extern   WORD     _sosDIGIData_Start;
extern   WORD     _sosDIGIData_End;
extern   WORD     _wSOSDriverLinear[];
extern   WORD     _wSOSTimerLinear[];
extern   LPSTR    _lpSOSDriver[];
extern   LPSTR    _lpSOSTimer[];
extern   LPSTR    _lpSOSDriverCS[];
extern   LPSTR    _lpSOSTimerCS[];
extern   BOOL     _wSOSDriverLoaded[];
extern   BOOL     _wSOSTimerLoaded[];
extern   BOOL     _wSOSDriverInitialized[];
extern   WORD     _wSOSOutputRate[];
extern   WORD     _wSOSDMABuffersize[];
extern   LONG     _dwSOSDMABufferPhysical[];
extern   LPSTR    _lpSOSDMABuffer[];
extern   BOOL     _wTimerUsed;
extern   VOID ( far *_lpSOSFillHandler[] )( VOID );
extern   WORD     _wSOSTimerType[];
extern   WORD     _wSOSDriverType[];
extern   _SOS_SAMPLE far * _lpSOSSampleList[][ _MAX_VOICES ];
extern   LPWORD      _lpSOSDMAIrqCount[];
extern   LPWORD      _lpSOSDMAFillCount[];
extern   WORD        _wSOSTmrNextCount;
extern   VOID ( interrupt far *_lpSOSOldTimer )( VOID );
extern   WORD     _wSOSDriverID[];
extern   _SOS_CAPABILITIES _sSOSDriverCaps[];
extern   WORD        _wSOSDMAPortList[];
extern   BYTE        _bSOSDMAChannel[];
extern   _SOS_INIT_DRIVER     _sSOSDIGIInitDriver[];
extern   BYTE  _pSOSDriverPath[];
extern   BYTE  _pSOSTempDriverPath[];
extern   BOOL  _wTIMERUsed;
extern   WORD  _wTIMERValue;
extern   VOID ( far * _lpTIMEREvents[] )( VOID );
extern   WORD _wTIMEREventRate[];
extern   WORD _dwTIMEREventFraction[];
extern   WORD _dwTIMEREventFractionCurrent[];
extern   BYTE  _bSOSMIDITimerSongHandler[];
extern   BYTE  _bSOSMIDISongHandle;
extern   WORD  _wSOSTimerMemHandle[];
extern   WORD  _wSOSDriverMemHandle[];
extern   WORD  _wSOSRealSeg[];

extern   _FILEHEADER    _sDETFileHeader;
extern   _DRIVERHEADER  _sDETDriverHeader;
extern   _FILEHEADER    sLOADFileHeader;
extern   _DRIVERHEADER  sLOADDriverHeader;
extern   BOOL           _wDETInitialized;
extern   WORD           _wDETLinear;
extern   LPSTR          _lpDETDriverBuffer;
extern   LPSTR          _lpDETDriverBufferCS;
extern   WORD           _hDETFile;
extern   DWORD          _dwDETDriverIndex;
extern   WORD           _wDETDriverIndexCur;
extern   WORD           _wDETMemHandle;
extern   LPSOSDEVICECAPS _lpDETDeviceCaps;
extern   _SOS_CAPABILITIES _sDETCaps;
extern   PSTR _pSOSErrorStrings[];
extern   BOOL _wSOSBufferAllocated[];
extern   BOOL _wSOSSystemInitialized;
extern   VDS_STRUCT _sSOSVDSInfo;
extern   _SOS_FILL_INFO _sSOSFillInfo;
extern   WORD    _wSOSTimerEventIndex;
extern   WORD  _wSOSTimerEntered;
extern   WORD  _wSOSDriverSize[];
extern   WORD  _wSOSTimerSize[];

#ifdef __cplusplus
extern "C" {
#endif
extern   WORD  _sosDIGIData1_Start;
extern   WORD  _sosDIGIData1_End;
extern   WORD  _sosDIGIData2_Start;
extern   WORD  _sosDIGIData2_End;
extern   BYTE  _bTIMERInstalled;
extern   BYTE  _bTIMERDPMI;
extern   WORD  wDetectPort;
extern   WORD  wDetectIRQ;
extern   WORD  wDetectDMA;
extern   WORD  wDetectParam;
#ifdef __cplusplus
}
#endif

#pragma pack()

#endif

