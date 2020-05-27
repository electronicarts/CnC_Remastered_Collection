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
 *  Copyright (C) 1994-1995 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       dplay.h
 *  Content:    DirectPlay include file
 *
 ***************************************************************************/

#ifndef __DPLAY_INCLUDED__
#define __DPLAY_INCLUDED__
#ifdef _WIN32
/* for DECLARE_INTERFACE and HRESULT. */
#include <ole2.h>
#endif

#define _FACDP  0x877
#define MAKE_DPHRESULT( code )    MAKE_HRESULT( 1, _FACDP, code )

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)


/*============================================================================
 *
 * DirectPlay Structures
 *
 * Various structures used to invoke DirectPlay.
 *
 *==========================================================================*/

#ifdef __cplusplus
/* 'struct' not 'class' per the way DECLARE_INTERFACE_ is defined */
struct IDirectPlay;
typedef struct IDirectPlay        FAR *LPDIRECTPLAY;
#else                  
typedef struct IDirectPlay        FAR *LPDIRECTPLAY;
#endif

typedef DWORD DPID, FAR *LPDPID;

typedef struct _DPCAPS
{
    DWORD   dwSize;
    DWORD   dwFlags;
    DWORD   dwMaxBufferSize;
    DWORD   dwMaxQueueSize;             // Function of DPlay, not SP.
    DWORD   dwMaxPlayers;
    DWORD   dwHundredBaud;              // 24 is 2400, 96 is 9600, etc.
    DWORD   dwLatency;
} DPCAPS;

typedef DPCAPS        FAR *LPDPCAPS;

#define DPLONGNAMELEN           52
#define DPSHORTNAMELEN          20
#define DPSESSIONNAMELEN        32
#define DPPASSWORDLEN           16
#define DPUSERRESERVED          16

typedef struct
{
    DWORD   dwSize;
    GUID    guidSession;                  // Id for Game.  Null is all games.
    DWORD   dwSession;                    // session identifier
    DWORD   dwMaxPlayers;                 // Maximum players allowed in game.
    DWORD   dwCurrentPlayers;             // Current  players in Game.
    DWORD   dwFlags;                      // DPOPEN_* flags
    char    szSessionName[DPSESSIONNAMELEN];// Human readable name for Game
    char    szUserField[DPUSERRESERVED];
    DWORD   dwReserved1;                  // Reserved for future MS use.
    char    szPassword[DPPASSWORDLEN];    // Password to be allowed into game.
    DWORD   dwReserved2;                  // Reserved for future MS use. 
    DWORD   dwUser1;
    DWORD   dwUser2;
    DWORD   dwUser3;
    DWORD   dwUser4;
} DPSESSIONDESC;
typedef DPSESSIONDESC FAR *LPDPSESSIONDESC;


/*
 * Create API
 */
typedef BOOL (FAR PASCAL * LPDPENUMDPCALLBACK)(
    LPGUID              lpSPGuid,
    LPSTR               lpFriendlyName,
    DWORD               dwMajorVersion,
    DWORD               dwMinorVersion,
    LPVOID              lpContext);

typedef BOOL (FAR PASCAL * LPDPENUMSESSIONSCALLBACK)(
    LPDPSESSIONDESC     lpDPSGameDesc,
    LPVOID              lpContext,
    LPDWORD             lpdwTimeOut,
    DWORD               dwFlags);



extern HRESULT WINAPI DirectPlayCreate( LPGUID lpGUID, LPDIRECTPLAY FAR *lplpDP, IUnknown FAR *pUnk);
extern HRESULT WINAPI DirectPlayEnumerate( LPDPENUMDPCALLBACK, LPVOID );


/* Player enumeration callback prototype */
typedef BOOL (FAR PASCAL *LPDPENUMPLAYERSCALLBACK)(
    DPID    dpId,
    LPSTR   lpFriendlyName,
    LPSTR   lpFormalName,
    DWORD   dwFlags,
    LPVOID  lpContext );

/*
 * IDirectPlay
 */
#undef INTERFACE
#define INTERFACE IDirectPlay
#ifdef _WIN32
DECLARE_INTERFACE_( IDirectPlay, IUnknown )
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)       (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS)  PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;
    /*** IDirectPlay methods ***/
    STDMETHOD(AddPlayerToGroup)     (THIS_ DPID, DPID) PURE;
    STDMETHOD(Close)                (THIS) PURE;
    STDMETHOD(CreatePlayer)         (THIS_ LPDPID,LPSTR,LPSTR,LPHANDLE) PURE;
    STDMETHOD(CreateGroup)          (THIS_ LPDPID,LPSTR,LPSTR) PURE;
    STDMETHOD(DeletePlayerFromGroup)(THIS_ DPID,DPID) PURE;
    STDMETHOD(DestroyPlayer)        (THIS_ DPID) PURE;
    STDMETHOD(DestroyGroup)         (THIS_ DPID) PURE;
    STDMETHOD(EnableNewPlayers)     (THIS_ BOOL) PURE;
    STDMETHOD(EnumGroupPlayers)     (THIS_ DPID, LPDPENUMPLAYERSCALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(EnumGroups)           (THIS_ DWORD, LPDPENUMPLAYERSCALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(EnumPlayers)          (THIS_ DWORD, LPDPENUMPLAYERSCALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(EnumSessions)         (THIS_ LPDPSESSIONDESC,DWORD,LPDPENUMSESSIONSCALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(GetCaps)              (THIS_ LPDPCAPS) PURE;
    STDMETHOD(GetMessageCount)      (THIS_ DPID, LPDWORD) PURE;
    STDMETHOD(GetPlayerCaps)        (THIS_ DPID, LPDPCAPS) PURE;
    STDMETHOD(GetPlayerName)        (THIS_ DPID,LPSTR,LPDWORD,LPSTR,LPDWORD) PURE;
    STDMETHOD(Initialize)           (THIS_ LPGUID) PURE;
    STDMETHOD(Open)                 (THIS_ LPDPSESSIONDESC) PURE;
    STDMETHOD(Receive)              (THIS_ LPDPID,LPDPID,DWORD,LPVOID,LPDWORD) PURE;
    STDMETHOD(SaveSession)          (THIS_ LPSTR) PURE;
    STDMETHOD(Send)                 (THIS_ DPID, DPID, DWORD, LPVOID, DWORD) PURE;
    STDMETHOD(SetPlayerName)        (THIS_ DPID,LPSTR,LPSTR) PURE;
};
#endif




/****************************************************************************
 *
 * DIRECTPLAY ERRORS
 *
 * Errors are represented by negative values and cannot be combined.
 *
 ****************************************************************************/
#define DP_OK                           0
#define DPERR_ALREADYINITIALIZED        MAKE_DPHRESULT(   5 )
#define DPERR_ACCESSDENIED              MAKE_DPHRESULT(  10 )
#define DPERR_ACTIVEPLAYERS             MAKE_DPHRESULT(  20 )
#define DPERR_BUFFERTOOSMALL            MAKE_DPHRESULT(  30 )
#define DPERR_CANTADDPLAYER             MAKE_DPHRESULT(  40 )
#define DPERR_CANTCREATEGROUP           MAKE_DPHRESULT(  50 )
#define DPERR_CANTCREATEPLAYER          MAKE_DPHRESULT(  60 )
#define DPERR_CANTCREATESESSION         MAKE_DPHRESULT(  70 )
#define DPERR_CAPSNOTAVAILABLEYET       MAKE_DPHRESULT(  80 )
#define DPERR_EXCEPTION                 MAKE_DPHRESULT(  90 )
#define DPERR_GENERIC                   E_FAIL

#define DPERR_INVALIDFLAGS              MAKE_DPHRESULT( 120 )
#define DPERR_INVALIDOBJECT             MAKE_DPHRESULT( 130 )
#define DPERR_INVALIDPARAM              E_INVALIDARG
#define DPERR_INVALIDPARAMS             DPERR_INVALIDPARAM
#define DPERR_INVALIDPLAYER             MAKE_DPHRESULT( 150 )
#define DPERR_NOCAPS                    MAKE_DPHRESULT( 160 )
#define DPERR_NOCONNECTION              MAKE_DPHRESULT( 170 )
#define DPERR_NOMEMORY                  E_OUTOFMEMORY
#define DPERR_OUTOFMEMORY               DPERR_NOMEMORY
#define DPERR_NOMESSAGES                MAKE_DPHRESULT( 190 )
#define DPERR_NONAMESERVERFOUND         MAKE_DPHRESULT( 200 )
#define DPERR_NOPLAYERS                 MAKE_DPHRESULT( 210 )
#define DPERR_NOSESSIONS                MAKE_DPHRESULT( 220 )
#define DPERR_SENDTOOBIG                MAKE_DPHRESULT( 230 )
#define DPERR_TIMEOUT                   MAKE_DPHRESULT( 240 )
#define DPERR_UNAVAILABLE               MAKE_DPHRESULT( 250 )
#define DPERR_UNSUPPORTED               E_NOTIMPL
#define DPERR_BUSY                      MAKE_DPHRESULT( 270 )
#define DPERR_USERCANCEL                MAKE_DPHRESULT( 280 ) 


#define DPOPEN_OPENSESSION          0x00000001
#define DPOPEN_CREATESESSION        0x00000002

#define DPSEND_GUARANTEE            0x00000001
#define DPSEND_HIGHPRIORITY         0x00000002
#define DPSEND_TRYONCE              0x00000004

#define DPRECEIVE_ALL               0x00000001
#define DPRECEIVE_TOPLAYER          0x00000002
#define DPRECEIVE_FROMPLAYER        0x00000004
#define DPRECEIVE_PEEK              0x00000008

#define DPCAPS_NAMESERVICE          0x00000001 // A name server is supported.
#define DPCAPS_NAMESERVER           0x00000002 // You are the name server.
#define DPCAPS_GUARANTEED           0x00000004 // SP's don't have to implement guarantees.

#define DPENUMSESSIONS_AVAILABLE    0x00000001 // All games that match password (if given)
                                               // and have openings.
#define DPENUMSESSIONS_ALL          0x00000002
#define DPENUMSESSIONS_PREVIOUS     0x00000004

#define DPENUMPLAYERS_ALL           0x00000000
#define DPENUMPLAYERS_PREVIOUS      0x00000004
#define DPENUMPLAYERS_LOCAL         0x00000008
#define DPENUMPLAYERS_REMOTE        0x00000010
#define DPENUMPLAYERS_GROUP         0x00000020
#define DPENUMPLAYERS_SESSION       0x00000080

//
// This flag is set on the enumsessions callback when the time out has occured.
// This means that there is no session data for this callback.
// If lpdwTimeOut is set to a non-zero value and the EnumSessionsCallback returns
// TRUE then EnumSessions will continue until the next timeout occurs.
// Timeouts are in milliseconds.
 
#define DPESC_TIMEDOUT              0x00000001


//
// System message structures and types.
//
// System messages have a leading 4 byte type code to identify the message.
// an app knows it is a system message because it is addressed 'To' player 0.
//

                                    
#define DPSYS_ADDPLAYER               0x0003  // DPMSG_ADDPLAYER
#define DPSYS_DELETEPLAYER            0x0005  // DPMSG_DELETEPLAYER

#define DPSYS_ADDPLAYERTOGROUP        0x0007  // DPMSG_GROUPADD

#define DPSYS_INVITE                  0x000e  // DPMSG_INVITE, Net only.

#define DPSYS_DELETEGROUP             0x0020  // DPMSG_DELETEPLAYER
#define DPSYS_DELETEPLAYERFROMGRP     0x0021  // DPMSG_GROUPDELETE
#define DPSYS_SESSIONLOST             0x0031

#define DPSYS_CONNECT                 0x484b  // DPMSG_GENERIC



typedef struct
{
    DWORD       dwType;
    DWORD       dwPlayerType;
    DPID        dpId;
    char        szLongName[DPLONGNAMELEN];
    char        szShortName[DPSHORTNAMELEN];
    DWORD       dwCurrentPlayers;
}   DPMSG_ADDPLAYER;

typedef DPMSG_ADDPLAYER DPMSG_ADDGROUP;

typedef struct
{
    DWORD       dwType;
    DPID        dpIdGroup;
    DPID        dpIdPlayer;
} DPMSG_GROUPADD;

typedef DPMSG_GROUPADD DPMSG_GROUPDELETE;
typedef struct
{
    DWORD       dwType;
    DPID        dpId;
} DPMSG_DELETEPLAYER;

typedef struct
{
    DWORD           dwType;
    DPSESSIONDESC   dpsDesc;
} DPMSG_INVITE;



typedef struct
{
    DWORD           dwType;
} DPMSG_GENERIC;

#pragma pack(pop)


DEFINE_GUID( IID_IDirectPlay, 0x5454e9a0, 0xdb65, 0x11ce, 0x92, 0x1c, 0x00, 0xaa, 0x00, 0x6c, 0x49, 0x72);



#ifdef __cplusplus
};
#endif

#endif
