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

/*
 *   wsnwlink.h
 *
 *
 *  Microsoft Windows
 *  Copyright (C) Microsoft Corporation, 1992-1996.
 *   Microsoft-specific extensions to the Windows NT IPX/SPX Windows
 *   Sockets interface.  These extensions are provided for use as
 *   necessary for compatibility with existing applications.  They are
 *   otherwise not recommended for use, as they are only guaranteed to
 *   work *   over the Microsoft IPX/SPX stack.  An application which
 *   uses these *   extensions may not work over other IPX/SPX
 *   implementations.  Include this header file after winsock.h and
 *   wsipx.h.
 *
 *   To open an IPX socket where a particular packet type is sent in
 *   the IPX header, specify NSPROTO_IPX + n as the protocol parameter
 *   of the socket() API.  For example, to open an IPX socket that
 *   sets the packet type to 34, use the following socket() call:
 *
 *       s = socket(AF_IPX, SOCK_DGRAM, NSPROTO_IPX + 34);
 *
 *   Below are socket option that may be set or retrieved by specifying
 *   the appropriate manifest in the "optname" parameter of getsockopt()
 *   or setsockopt().  Use NSPROTO_IPX as the "level" argument for the
 *   call.
 *
 */

#ifndef _WSNWLINK_
#define _WSNWLINK_


/*
 *   Set/get the IPX packet type.  The value specified in the
 *   optval argument will be set as the packet type on every IPX
 *   packet sent from this socket.  The optval parameter of
 *   getsockopt()/setsockopt() points to an int.
 *
 */

#define IPX_PTYPE               0x4000


/*
 *   Set/get the receive filter packet type.  Only IPX packets with
 *   a packet type equal to the value specified in the optval
 *   argument will be returned; packets with a packet type that
 *   does not match are discarded.  optval points to an int.
 *
 */

#define IPX_FILTERPTYPE         0x4001


/*
 *   Stop filtering on packet type set with IPX_FILTERPTYPE.
 *
 */

#define IPX_STOPFILTERPTYPE     0x4003


/*
 *   Set/get the value of the datastream field in the SPX header on
 *   every packet sent.  optval points to an int.
 *
 */

#define IPX_DSTYPE              0x4002


/*
 *   Enable extended addressing.  On sends, adds the element
 *   "unsigned char sa_ptype" to the SOCKADDR_IPX structure,
 *   making the total length 15 bytes.  On receives, add both
 *   the sa_ptype and "unsigned char sa_flags" to the SOCKADDR_IPX
 *   structure, making the total length 16 bytes.  The current
 *   bits defined in sa_flags are:
 *
 *   0x01 - the received frame was sent as a broadcast
 *   0x02 - the received frame was sent from this machine
 *
 *   optval points to a BOOL.
 *
 */

#define IPX_EXTENDED_ADDRESS    0x4004


/*
 *   Send protocol header up on all receive packets.  optval points
 *   to a BOOL.
 *
 */

#define IPX_RECVHDR             0x4005


/*
 *   Get the maximum data size that can be sent.  Not valid with
 *   setsockopt().  optval points to an int where the value is
 *   returned.
 *
 */

#define IPX_MAXSIZE             0x4006


/*
 *   Query information about a specific adapter that IPX is bound
 *   to.  In a system with n adapters they are numbered 0 through n-1.
 *   Callers can issue the IPX_MAX_ADAPTER_NUM getsockopt() to find
 *   out the number of adapters present, or call IPX_ADDRESS with
 *   increasing values of adapternum until it fails.  Not valid
 *   with setsockopt().  optval points to an instance of the
 *   IPX_ADDRESS_DATA structure with the adapternum filled in.
 *
 */

#define IPX_ADDRESS             0x4007

typedef struct _IPX_ADDRESS_DATA {
    INT   adapternum;  /* input: 0-based adapter number */
    UCHAR netnum[4];   /* output: IPX network number */
    UCHAR nodenum[6];  /* output: IPX node address */
    BOOLEAN wan;       /* output: TRUE = adapter is on a wan link */
    BOOLEAN status;    /* output: TRUE = wan link is up (or adapter is not wan) */
    INT   maxpkt;      /* output: max packet size, not including IPX header */
    ULONG linkspeed;   /* output: link speed in 100 bytes/sec (i.e. 96 == 9600 bps) */
} IPX_ADDRESS_DATA, *PIPX_ADDRESS_DATA;


/*
 *   Query information about a specific IPX network number.  If the
 *   network is in IPX's cache it will return the information directly,
 *   otherwise it will issue RIP requests to find it.  Not valid with
 *   setsockopt().  optval points to an instance of the IPX_NETNUM_DATA
 *   structure with the netnum filled in.
 *
 */

#define IPX_GETNETINFO          0x4008

typedef struct _IPX_NETNUM_DATA {
    UCHAR  netnum[4];  /* input: IPX network number */
    USHORT hopcount;   /* output: hop count to this network, in machine order */
    USHORT netdelay;   /* output: tick count to this network, in machine order */
    INT    cardnum;    /* output: 0-based adapter number used to route to this net */
                       /*         can be used as adapternum input to IPX_ADDRESS */
    UCHAR  router[6];  /* output: MAC address of the next hop router, zeroed if */
                       /*         the network is directly attached */
} IPX_NETNUM_DATA, *PIPX_NETNUM_DATA;


/*
 *   Like IPX_GETNETINFO except it *does not* issue RIP requests. If the
 *   network is in IPX's cache it will return the information, otherwise
 *   it will fail (see also IPX_RERIPNETNUMBER which *always* forces a
 *   re-RIP). Not valid with setsockopt().  optval points to an instance of
 *   the IPX_NETNUM_DATA structure with the netnum filled in.
 *
 */

#define IPX_GETNETINFO_NORIP    0x4009


/*
 *   Get information on a connected SPX socket.  optval points
 *   to an instance of the IPX_SPXCONNSTATUS_DATA structure.
 *
 *   All numbers are in Novell (high-low) order.
 *
 */

#define IPX_SPXGETCONNECTIONSTATUS 0x400B

typedef struct _IPX_SPXCONNSTATUS_DATA {
    UCHAR  ConnectionState;
    UCHAR  WatchDogActive;
    USHORT LocalConnectionId;
    USHORT RemoteConnectionId;
    USHORT LocalSequenceNumber;
    USHORT LocalAckNumber;
    USHORT LocalAllocNumber;
    USHORT RemoteAckNumber;
    USHORT RemoteAllocNumber;
    USHORT LocalSocket;
    UCHAR  ImmediateAddress[6];
    UCHAR  RemoteNetwork[4];
    UCHAR  RemoteNode[6];
    USHORT RemoteSocket;
    USHORT RetransmissionCount;
    USHORT EstimatedRoundTripDelay; /* In milliseconds */
    USHORT RetransmittedPackets;
    USHORT SuppressedPacket;
} IPX_SPXCONNSTATUS_DATA, *PIPX_SPXCONNSTATUS_DATA;


/*
 *   Get notification when the status of an adapter that IPX is
 *   bound to changes.  Typically this will happen when a wan line
 *   goes up or down.  Not valid with setsockopt().  optval points
 *   to a buffer which contains an IPX_ADDRESS_DATA structure
 *   followed immediately by a HANDLE to an unsignaled event.
 *
 *   When the getsockopt() query is submitted, it will complete
 *   successfully.  However, the IPX_ADDRESS_DATA pointed to by
 *   optval will not be updated at that point.  Instead the
 *   request is queued internally inside the transport.
 *
 *   When the status of an adapter changes, IPX will locate a
 *   queued getsockopt() query and fill in all the fields in the
 *   IPX_ADDRESS_DATA structure.  It will then signal the event
 *   pointed to by the HANDLE in the optval buffer.  This handle
 *   should be obtained before calling getsockopt() by calling
 *   CreateEvent().  If multiple getsockopts() are submitted at
 *   once, different events must be used.
 *
 *   The event is used because the call needs to be asynchronous
 *   but currently getsockopt() does not support this.
 *
 *   WARNING: In the current implementation, the transport will
 *   only signal one queued query for each status change.  Therefore
 *   only one service which uses this query should be running at
 *   once.
 *
 */

#define IPX_ADDRESS_NOTIFY      0x400C


/*
 *   Get the maximum number of adapters present.  If this call returns
 *   n then the adapters are numbered 0 through n-1.  Not valid
 *   with setsockopt().  optval points to an int where the value
 *   is returned.
 *
 */

#define IPX_MAX_ADAPTER_NUM     0x400D


/*
 *   Like IPX_GETNETINFO except it forces IPX to re-RIP even if the
 *   network is in its cache (but not if it is directly attached to).
 *   Not valid with setsockopt().  optval points to an instance of
 *   the IPX_NETNUM_DATA structure with the netnum filled in.
 *
 */

#define IPX_RERIPNETNUMBER      0x400E


/*
 *   A hint that broadcast packets may be received.  The default is
 *   TRUE.  Applications that do not need to receive broadcast packets
 *   should set this sockopt to FALSE which may cause better system
 *   performance (note that it does not necessarily cause broadcasts
 *   to be filtered for the application).  Not valid with getsockopt().
 *   optval points to a BOOL.
 *
 */

#define IPX_RECEIVE_BROADCAST   0x400F


/*
 *   On SPX connections, don't delay before sending ack.  Applications
 *   that do not tend to have back-and-forth traffic over SPX should
 *   set this; it will increase the number of acks sent but will remove
 *   delays in sending acks.  optval points to a BOOL.
 *
 */

#define IPX_IMMEDIATESPXACK     0x4010

#endif

