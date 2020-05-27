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

/*==========================================================================;
 *
 *  Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       dsound.h
 *  Content:    DirectSound include file
 *
 ***************************************************************************/

#ifndef __DSOUND_INCLUDED__
#define __DSOUND_INCLUDED__

#ifdef _WIN32
#define COM_NO_WINDOWS_H
#include <objbase.h>
#endif

#define _FACDS  0x878
#define MAKE_DSHRESULT( code )  MAKE_HRESULT( 1, _FACDS, code )

#ifdef __cplusplus
extern "C" {
#endif

// Direct Sound Component GUID    {47D4D946-62E8-11cf-93BC-444553540000}
DEFINE_GUID(CLSID_DirectSound,
0x47d4d946, 0x62e8, 0x11cf, 0x93, 0xbc, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0);

// DirectSound 279afa83-4981-11ce-a521-0020af0be560
DEFINE_GUID(IID_IDirectSound,0x279AFA83,0x4981,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60);
// DirectSoundBuffer 279afa85-4981-11ce-a521-0020af0be560
DEFINE_GUID(IID_IDirectSoundBuffer,0x279AFA85,0x4981,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60);



//==========================================================================;
//
//                            Structures...
//
//==========================================================================;
#ifdef __cplusplus
/* 'struct' not 'class' per the way DECLARE_INTERFACE_ is defined */
struct IDirectSound;
struct IDirectSoundBuffer;
#endif

typedef struct IDirectSound           *LPDIRECTSOUND;        
typedef struct IDirectSoundBuffer     *LPDIRECTSOUNDBUFFER;  
typedef struct IDirectSoundBuffer    **LPLPDIRECTSOUNDBUFFER;  


typedef struct _DSCAPS
{
    DWORD       dwSize;
    DWORD       dwFlags;
    DWORD       dwMinSecondarySampleRate;
    DWORD       dwMaxSecondarySampleRate;
    DWORD       dwPrimaryBuffers;
    DWORD       dwMaxHwMixingAllBuffers;
    DWORD       dwMaxHwMixingStaticBuffers;
    DWORD       dwMaxHwMixingStreamingBuffers;
    DWORD       dwFreeHwMixingAllBuffers;
    DWORD       dwFreeHwMixingStaticBuffers;
    DWORD       dwFreeHwMixingStreamingBuffers;
    DWORD       dwMaxHw3DAllBuffers;
    DWORD       dwMaxHw3DStaticBuffers;
    DWORD       dwMaxHw3DStreamingBuffers;
    DWORD       dwFreeHw3DAllBuffers;
    DWORD       dwFreeHw3DStaticBuffers;
    DWORD       dwFreeHw3DStreamingBuffers;
    DWORD       dwTotalHwMemBytes;
    DWORD       dwFreeHwMemBytes;
    DWORD       dwMaxContigFreeHwMemBytes;
    DWORD       dwUnlockTransferRateHwBuffers;
    DWORD       dwPlayCpuOverheadSwBuffers;
    DWORD       dwReserved1;
    DWORD       dwReserved2;
} DSCAPS, *LPDSCAPS;

typedef struct _DSBCAPS
{
    
    DWORD       dwSize;
    DWORD       dwFlags;
    DWORD       dwBufferBytes;
    DWORD       dwUnlockTransferRate;
    DWORD       dwPlayCpuOverhead;
} DSBCAPS, *LPDSBCAPS;

typedef struct _DSBUFFERDESC
{
    DWORD                   dwSize;
    DWORD                   dwFlags;
    DWORD                   dwBufferBytes;
    DWORD                   dwReserved;
    LPWAVEFORMATEX          lpwfxFormat;
} DSBUFFERDESC, *LPDSBUFFERDESC;



typedef LPVOID* LPLPVOID;


typedef BOOL (FAR PASCAL * LPDSENUMCALLBACKW)(GUID FAR *, LPWSTR, LPWSTR, LPVOID);
typedef BOOL (FAR PASCAL * LPDSENUMCALLBACKA)(GUID FAR *, LPSTR, LPSTR, LPVOID);

extern HRESULT WINAPI DirectSoundCreate(GUID FAR * lpGUID, LPDIRECTSOUND * ppDS, IUnknown FAR *pUnkOuter );
extern HRESULT WINAPI DirectSoundEnumerateW(LPDSENUMCALLBACKW lpCallback, LPVOID lpContext );
extern HRESULT WINAPI DirectSoundEnumerateA(LPDSENUMCALLBACKA lpCallback, LPVOID lpContext );

#ifdef UNICODE
#define LPDSENUMCALLBACK        LPDSENUMCALLBACKW
#define DirectSoundEnumerate    DirectSoundEnumerateW
#else
#define LPDSENUMCALLBACK        LPDSENUMCALLBACKA
#define DirectSoundEnumerate    DirectSoundEnumerateA
#endif

//
// IDirectSound
//
#undef INTERFACE
#define INTERFACE IDirectSound
#ifdef _WIN32
DECLARE_INTERFACE_( IDirectSound, IUnknown )
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;
    /*** IDirectSound methods ***/

    STDMETHOD( CreateSoundBuffer)(THIS_ LPDSBUFFERDESC, LPLPDIRECTSOUNDBUFFER, IUnknown FAR *) PURE;
    STDMETHOD( GetCaps)(THIS_ LPDSCAPS ) PURE;
    STDMETHOD( DuplicateSoundBuffer)(THIS_ LPDIRECTSOUNDBUFFER, LPLPDIRECTSOUNDBUFFER ) PURE;
    STDMETHOD( SetCooperativeLevel)(THIS_ HWND, DWORD ) PURE;
    STDMETHOD( Compact)(THIS ) PURE;
    STDMETHOD( GetSpeakerConfig)(THIS_ LPDWORD ) PURE;
    STDMETHOD( SetSpeakerConfig)(THIS_ DWORD ) PURE;
    STDMETHOD( Initialize)(THIS_ GUID FAR * ) PURE;
};

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSound_QueryInterface(p,a,b)       (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectSound_AddRef(p)                   (p)->lpVtbl->AddRef(p)
#define IDirectSound_Release(p)                  (p)->lpVtbl->Release(p)
#define IDirectSound_CreateSoundBuffer(p,a,b,c)  (p)->lpVtbl->CreateSoundBuffer(p,a,b,c)
#define IDirectSound_GetCaps(p,a)                (p)->lpVtbl->GetCaps(p,a)
#define IDirectSound_DuplicateSoundBuffer(p,a,b) (p)->lpVtbl->DuplicateSoundBuffer(p,a,b)
#define IDirectSound_SetCooperativeLevel(p,a,b)  (p)->lpVtbl->SetCooperativeLevel(p,a,b)
#define IDirectSound_Compact(p)                  (p)->lpVtbl->Compact(p)
#define IDirectSound_GetSpeakerConfig(p,a)       (p)->lpVtbl->GetSpeakerConfig(p,a)
#define IDirectSound_SetSpeakerConfig(p,b)       (p)->lpVtbl->SetSpeakerConfig(p,b)
#define IDirectSound_Initialize(p,a)             (p)->lpVtbl->Initialize(p,a)
#endif

#endif

//
// IDirectSoundBuffer
//
#undef INTERFACE
#define INTERFACE IDirectSoundBuffer
#ifdef _WIN32
DECLARE_INTERFACE_( IDirectSoundBuffer, IUnknown )
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;
    /*** IDirectSoundBuffer methods ***/

    STDMETHOD(           GetCaps)(THIS_ LPDSBCAPS ) PURE;
    STDMETHOD(GetCurrentPosition)(THIS_ LPDWORD,LPDWORD ) PURE;
    STDMETHOD(         GetFormat)(THIS_ LPWAVEFORMATEX, DWORD, LPDWORD ) PURE;
    STDMETHOD(         GetVolume)(THIS_ LPLONG ) PURE;
    STDMETHOD(            GetPan)(THIS_ LPLONG ) PURE;
    STDMETHOD(      GetFrequency)(THIS_ LPDWORD ) PURE;
    STDMETHOD(         GetStatus)(THIS_ LPDWORD ) PURE;
    STDMETHOD(        Initialize)(THIS_ LPDIRECTSOUND, LPDSBUFFERDESC ) PURE;
    STDMETHOD(              Lock)(THIS_ DWORD,DWORD,LPVOID,LPDWORD,LPVOID,LPDWORD,DWORD ) PURE;
    STDMETHOD(              Play)(THIS_ DWORD,DWORD,DWORD ) PURE;
    STDMETHOD(SetCurrentPosition)(THIS_ DWORD ) PURE;
    STDMETHOD(         SetFormat)(THIS_ LPWAVEFORMATEX ) PURE;
    STDMETHOD(         SetVolume)(THIS_ LONG ) PURE;
    STDMETHOD(            SetPan)(THIS_ LONG ) PURE;
    STDMETHOD(      SetFrequency)(THIS_ DWORD ) PURE;
    STDMETHOD(              Stop)(THIS  ) PURE;
    STDMETHOD(            Unlock)(THIS_ LPVOID,DWORD,LPVOID,DWORD ) PURE;
    STDMETHOD(           Restore)(THIS  ) PURE;
};

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundBuffer_QueryInterface(p,a,b)        (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectSoundBuffer_AddRef(p)                    (p)->lpVtbl->AddRef(p)
#define IDirectSoundBuffer_Release(p)                   (p)->lpVtbl->Release(p)
#define IDirectSoundBuffer_GetCaps(p,a)                 (p)->lpVtbl->GetCaps(p,a)
#define IDirectSoundBuffer_GetCurrentPosition(p,a,b)    (p)->lpVtbl->GetCurrentPosition(p,a,b)
#define IDirectSoundBuffer_GetFormat(p,a,b,c)           (p)->lpVtbl->GetFormat(p,a,b,c)
#define IDirectSoundBuffer_GetVolume(p,a)               (p)->lpVtbl->GetVolume(p,a)
#define IDirectSoundBuffer_GetPan(p,a)                  (p)->lpVtbl->GetPan(p,a)
#define IDirectSoundBuffer_GetFrequency(p,a)            (p)->lpVtbl->GetFrequency(p,a)
#define IDirectSoundBuffer_GetStatus(p,a)               (p)->lpVtbl->GetStatus(p,a)
#define IDirectSoundBuffer_Initialize(p,a,b)            (p)->lpVtbl->Initialize(p,a,b)
#define IDirectSoundBuffer_Lock(p,a,b,c,d,e,f,g)        (p)->lpVtbl->Lock(p,a,b,c,d,e,f,g)
#define IDirectSoundBuffer_Play(p,a,b,c)                (p)->lpVtbl->Play(p,a,b,c)
#define IDirectSoundBuffer_SetCurrentPosition(p,a)      (p)->lpVtbl->SetCurrentPosition(p,a)
#define IDirectSoundBuffer_SetFormat(p,a)               (p)->lpVtbl->SetFormat(p,a)
#define IDirectSoundBuffer_SetVolume(p,a)               (p)->lpVtbl->SetVolume(p,a)
#define IDirectSoundBuffer_SetPan(p,a)                  (p)->lpVtbl->SetPan(p,a)
#define IDirectSoundBuffer_SetFrequency(p,a)            (p)->lpVtbl->SetFrequency(p,a)
#define IDirectSoundBuffer_Stop(p)                      (p)->lpVtbl->Stop(p)
#define IDirectSoundBuffer_Unlock(p,a,b,c,d)            (p)->lpVtbl->Unlock(p,a,b,c,d)
#define IDirectSoundBuffer_Restore(p)                   (p)->lpVtbl->Restore(p)
#endif

#endif



/*
 * Return Codes
 */

#define DS_OK                           0

/*
 * The call failed because resources (such as a priority level)
 *  were already being used by another caller.
 */
#define DSERR_ALLOCATED                 MAKE_DSHRESULT( 10 )
/*
 * The control (vol,pan,etc.) requested by the caller is not available.
 */
#define DSERR_CONTROLUNAVAIL            MAKE_DSHRESULT( 30 )
/*
 * An invalid parameter was passed to the returning function
 */
#define DSERR_INVALIDPARAM              E_INVALIDARG
/*
 * This call is not valid for the current state of this object
 */
#define DSERR_INVALIDCALL               MAKE_DSHRESULT( 50 )
/*
 * An undetermined error occured inside the DSound subsystem
 */
#define DSERR_GENERIC                   E_FAIL
/*
 * The caller does not have the priority level required for the function to
 * succeed.
 */
#define DSERR_PRIOLEVELNEEDED           MAKE_DSHRESULT( 70 )
/*
 * The DSound subsystem couldn't allocate sufficient memory to complete the
 * caller's request.
 */
#define DSERR_OUTOFMEMORY               E_OUTOFMEMORY
/*
 * The specified WAVE format is not supported
 */
#define DSERR_BADFORMAT                 MAKE_DSHRESULT( 100 )
/*
 * The function called is not supported at this time
 */
#define DSERR_UNSUPPORTED               E_NOTIMPL
/*
 * No sound driver is available for use
 */
#define DSERR_NODRIVER                  MAKE_DSHRESULT( 120 )
/*
 * This object is already initialized
 */
#define DSERR_ALREADYINITIALIZED        MAKE_DSHRESULT( 130 )
/*
 * This object does not support aggregation
 */
#define DSERR_NOAGGREGATION             CLASS_E_NOAGGREGATION
/*
 * The buffer memory has been lost, and must be Restored.
 */
#define DSERR_BUFFERLOST                MAKE_DSHRESULT( 150 )
/*
 * Another app has a higher priority level, preventing this call from
 * succeeding.
 */
#define DSERR_OTHERAPPHASPRIO           MAKE_DSHRESULT( 160 )
/*
 * The Initialize() member on the Direct Sound Object has not been
 * called or called successfully before calls to other members.
 */
#define DSERR_UNINITIALIZED             MAKE_DSHRESULT( 170 )




//==========================================================================;
//
//                               Flags...
//
//==========================================================================;

#define DSCAPS_PRIMARYMONO          0x00000001
#define DSCAPS_PRIMARYSTEREO        0x00000002
#define DSCAPS_PRIMARY8BIT          0x00000004
#define DSCAPS_PRIMARY16BIT         0x00000008
#define DSCAPS_CONTINUOUSRATE       0x00000010
#define DSCAPS_EMULDRIVER           0x00000020
#define DSCAPS_CERTIFIED            0x00000040
#define DSCAPS_SECONDARYMONO        0x00000100
#define DSCAPS_SECONDARYSTEREO      0x00000200
#define DSCAPS_SECONDARY8BIT        0x00000400
#define DSCAPS_SECONDARY16BIT       0x00000800



#define DSBPLAY_LOOPING         0x00000001


          
#define DSBSTATUS_PLAYING           0x00000001
#define DSBSTATUS_BUFFERLOST        0x00000002
#define DSBSTATUS_LOOPING           0x00000004
         

#define DSBLOCK_FROMWRITECURSOR         0x00000001



#define DSSCL_NORMAL                1
#define DSSCL_PRIORITY              2
#define DSSCL_EXCLUSIVE             3
#define DSSCL_WRITEPRIMARY          4



#define DSBCAPS_PRIMARYBUFFER       0x00000001
#define DSBCAPS_STATIC              0x00000002
#define DSBCAPS_LOCHARDWARE         0x00000004
#define DSBCAPS_LOCSOFTWARE         0x00000008
#define DSBCAPS_CTRLFREQUENCY       0x00000020
#define DSBCAPS_CTRLPAN             0x00000040
#define DSBCAPS_CTRLVOLUME          0x00000080
#define DSBCAPS_CTRLDEFAULT         0x000000E0  // Pan + volume + frequency.
#define DSBCAPS_CTRLALL             0x000000E0  // All control capabilities
#define DSBCAPS_STICKYFOCUS         0x00004000




#define DSSPEAKER_HEADPHONE     1
#define DSSPEAKER_MONO          2
#define DSSPEAKER_QUAD          3
#define DSSPEAKER_STEREO        4
#define DSSPEAKER_SURROUND      5






#ifdef __cplusplus
};
#endif

#endif  /* __DSOUND_INCLUDED__ */
