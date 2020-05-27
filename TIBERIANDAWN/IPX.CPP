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

/* $Header:   F:\projects\c&c\vcs\code\ipx.cpv   2.17   16 Oct 1995 16:49:34   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPX.CPP                                  *
 *                                                                         *
 *                   Programmer : Barry Nance										*
 * 										 from Client/Server LAN Programming			*
 *											 Westwood-ized by Bill Randolph				*
 *                                                                         *
 *                   Start Date : December 14, 1994                        *
 *                                                                         *
 *                  Last Update : December 15, 1994   [BR]                 *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Pitfalls:																					*
 * - Never try to use a closed socket; always check the return code from	*
 *   IPX_Open_Socket().																		*
 * - Always give IPX an outstanding ECB for listening, before you send.		*
 * - It turns out that IPX is pretty bad about saving registers, so if		*
 *   you have any register variables in your program, they may get			*
 *   trashed.  To circumvent this, all functions in this module save &		*
 *   restore the registers before invoking any IPX or NETX function.			*
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   IPX_SPX_Installed -- checks for installation of IPX/SPX               *
 *   IPX_Open_Socket -- opens an IPX socket for sending or receiving       *
 *   IPX_Close_Socket -- closes an open socket                             *
 *   IPX_Get_Connection_Number -- gets local Connection Number					*
 *   IPX_Get_1st_Connection_Num -- gets 1st Connect Number for given user  *
 *   IPX_Get_Internet_Address -- gets Network Number & Node Address			*
 *   IPX_Get_User_ID -- gets user ID from Connection Number                *
 *   IPX_Listen_For_Packet -- commands IPX to listen for a packet          *
 *   IPX_Send_Packet -- commands IPX to send a packet                      *
 *   IPX_Get_Local_Target -- fills in ImmediateAddress field of ECB        *
 *   IPX_Cancel_Event -- cancels an operation in progress                  *
 *   Let_IPX_Breath -- gives IPX some CPU time                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "ipx95.h"


/***************************************************************************
 * IPX_SPX_Installed -- checks for installation of IPX/SPX                 *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = not installed; 1 = IPX only, 2 = IPX and SPX are installed			*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/14/1994 BR : Created.                                              *
 *=========================================================================*/
int IPX_SPX_Installed(void)
{

#ifndef NOT_FOR_WIN95

	return (IPX_Initialise());

#else	//NOT_FOR_WIN95

	union REGS regs;
	struct SREGS sregs;
	RMIType rmi;

	/*------------------------------------------------------------------------
	Init all registers to 0's
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));

	/*------------------------------------------------------------------------
	Fill in registers for the DPMI call, function 0x300
	------------------------------------------------------------------------*/
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = 0x002f;					// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);

	/*------------------------------------------------------------------------
	Fill in registers for the real-mode interrupt handler.
	To test for the presence of IPX, set AH to 0x7a, AL to 0, and invoke
	interrupt 0x2f (the "multiplex" interrupt).  If IPX is installed,
	AL will be 0xff, and ES:DI will contain the IPX/SPX function address.
	------------------------------------------------------------------------*/
	rmi.eax = 0x00007a00;

	/*------------------------------------------------------------------------
	call DPMI
	------------------------------------------------------------------------*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

	/*------------------------------------------------------------------------
	If IPX isn't there, return 0
	------------------------------------------------------------------------*/
	if ( (rmi.eax & 0x00ff) != 0xff) {
		return(0);
	}

	/*------------------------------------------------------------------------
	Test for SPX by invoking the IPX_SPX function with BX = 0x10, and AL = 0.
	If SPX is present, AL will be 0xff.
	------------------------------------------------------------------------*/
	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = IPX_INT;					// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.ebx = 0x00000010;
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

	/*------------------------------------------------------------------------
	SPX is installed; return '2'
	------------------------------------------------------------------------*/
	if ( (rmi.eax & 0x00ff) == 0xff) {
		return(2);
	}

	/*------------------------------------------------------------------------
	SPX is not installed; return '1'
	------------------------------------------------------------------------*/
	return(1);
#endif	//NOT_FOR_WIN95
}	/* end of IPX_SPX_Installed */


/***************************************************************************
 * IPX_Open_Socket -- opens an IPX socket for sending or receiving         *
 *                                                                         *
 * INPUT:                                                                  *
 *		socket		the socket number to open											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = OK																					*
 *		-1 = IPX not installed																*
 *		0xfe = socket table is full														*
 *		0xff = socket is already open														*
 *                                                                         *
 * WARNINGS:                                                               *
 *		The application must define its socket number carefully.  Use			*
 *		values from 0x4000 to 0x8000 for custom socket numbers.  The app		*
 *		must know its own socket number as well as the socket number of		*
 *		a destination workstation.															*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
int IPX_Open_Socket(unsigned short socket)
{
	union REGS regs;
	struct SREGS sregs;
	RMIType rmi;
	int rc;

	/*------------------------------------------------------------------------
	Open the socket:
	DX = socket number
	AL = 0 for short-lived socket, 0xff for long-lived socket
	------------------------------------------------------------------------*/
	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = IPX_INT;					// interrupt # to invoke
	sregs.es = FP_SEG (&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF (&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.ebx = IPX_OPEN_SOCKET;			// function code
	rmi.edx = socket;						// desired socket #
	rmi.eax = 0x00ff;						// make this a long-lived socket
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x (DPMI_INT, &regs, &regs, &sregs);

	rc = (rmi.eax & 0xff);

	return(rc);

}	/* end of IPX_Open_Socket */

#endif
/***************************************************************************
 * IPX_Close_Socket -- closes an open socket                               *
 *                                                                         *
 * INPUT:                                                                  *
 *		socket		socket number to close												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = ok, -1 = error																	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
int IPX_Close_Socket(unsigned short socket)
{
	union REGS regs;
	struct SREGS sregs;
	RMIType rmi;

	/*------------------------------------------------------------------------
	Close the socket:
	DX = socket number
	------------------------------------------------------------------------*/
	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = IPX_INT;					// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.ebx = IPX_CLOSE_SOCKET;
	rmi.edx = socket;
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

	return(0);

}	/* end of IPX_Close_Socket */
#endif	//NOT_FOR_WIN95

/***************************************************************************
 * IPX_Get_Connection_Number -- gets local Connection Number					*
 *                                                                         *
 * This Novell call will the return the user's local "Connection Number".	*
 * This value will be 0 if the user isn't logged into Novell, so this		*
 * routine can be used to detect if other calls (such as Get_Local_Target)	*
 * will be OK.																					*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		Connection Number, 0 = none														*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
int IPX_Get_Connection_Number(void)
{
	union REGS regs;
	struct SREGS sregs;
	RMIType rmi;
	int num;

	/*------------------------------------------------------------------------
	Call Interrupt 0x21, with AH = 0xdc.  This tells Novell to put the local
	connection number into AL.
	------------------------------------------------------------------------*/
	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = 0x21;						// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.eax = 0x0000dc00;
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

	num = rmi.eax & 0x00ff;

	return(num);

}	/* end of IPX_Get_Connection_Number */

#endif //NOT_FOR_WIN95

/***************************************************************************
 * IPX_Get_1st_Connection_Num -- gets 1st Connect Number for given user    *
 *                                                                         *
 * This gets the Connection Number for the given User ID.  Since a user		*
 * may be logged in more than once, this just returns the first connection	*
 * found and ignores the others.															*
 *                                                                         *
 * INPUT:                                                                  *
 *		username		name of the user to get the Connection Number for			*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		first-found Connection Number for that user, 0 if user not logged in	*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
int IPX_Get_1st_Connection_Num (char *username)
{
	struct request_buffer {
		unsigned short len;							// username length + 5
		unsigned char buffer_type;					// ConnectionNum = 0x15
		unsigned short object_type;				// set ot 0x0100
		unsigned char name_len;						// length of username
		char name [48];								// copy of username
		unsigned short reserved;
	};
	struct reply_buffer {
		unsigned short  len;
		unsigned char   number_connections;		// will be 0 - 100
		unsigned char   connection_num [100];	// array of connection numbers
		unsigned short  reserved[2];
	};
	union REGS regs;
	struct SREGS sregs;
	RMIType rmi;
	struct request_buffer *reqbuf;
	struct reply_buffer *replybuf;
	unsigned short segment;							// for DOS allocation
	unsigned short selector;						// for DOS allocation
	int num_conns;										// # connections returned
	int conn_num;										// connection number
	int rc;

	/*------------------------------------------------------------------------
	Allocate DOS memory to store the buffers passed to the interrupt
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_ALLOC_DOS_MEM;						// DPMI function to call
	regs.x.ebx = (sizeof(struct request_buffer) + 	// # paragraphs to allocate
		sizeof(struct reply_buffer) + 15) >> 4;
	int386x (DPMI_INT, &regs, &regs, &sregs);			// allocate the memory
	/*........................................................................
	If the carry flag is set, DPMI is indicating an error.
	........................................................................*/
	if (regs.x.cflag) {
		return(0);
	}

	/*........................................................................
	Get pointers to allocated memory.
	'reqbuf' is just the returned real-mode segment, multiplied by 16.
	'replybuf' is an offset from 'reqbuf'.
	........................................................................*/
	segment = regs.w.ax;
	selector = regs.w.dx;
	reqbuf = (struct request_buffer *)(segment << 4);
	replybuf = (struct reply_buffer *)
		(((char *)reqbuf) + sizeof (struct request_buffer));

	/*------------------------------------------------------------------------
	Init the contents of the request & reply buffers
	------------------------------------------------------------------------*/
	reqbuf->len = (unsigned short)(strlen(username) + 5);
	reqbuf->buffer_type = 0x15;
	reqbuf->object_type = 0x0100;
	reqbuf->name_len    = (unsigned char) strlen(username);
	strcpy(reqbuf->name, username);
	reqbuf->reserved = reqbuf->reserved;				// prevent compiler warning
	replybuf->len = 101;
	replybuf->reserved[0] = replybuf->reserved[0];	// prevent compiler warning
	replybuf->reserved[0] = replybuf->reserved[1];	// prevent compiler warning

	/*------------------------------------------------------------------------
	Invoke Int 21 with AH=0xe3, DS:SI=&request_buffer, ES:DI=&reply_buffer
	------------------------------------------------------------------------*/
	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = 0x21;						// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.eax = 0x0000e300;
	rmi.ds = segment;
	rmi.esi = 0;
	rmi.es = segment;
	rmi.edi = sizeof(struct request_buffer);
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

	/*------------------------------------------------------------------------
	Stash the 1st connection number
	------------------------------------------------------------------------*/
	rc = (rmi.eax & 0x00ff);								// if AL !=0, error
	num_conns = replybuf->number_connections;			// # times user is logged in
	conn_num = (int )replybuf->connection_num[0];	// 1st connection #

	/*------------------------------------------------------------------------
	Free DOS memory
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_FREE_DOS_MEM;						// DPMI function to call
	regs.x.edx = selector;									// ptr to free
	int386x (DPMI_INT, &regs, &regs, &sregs);			// allocate the memory

	/*------------------------------------------------------------------------
	Return error if function failed, or user not logged in
	------------------------------------------------------------------------*/
	if (rc != 0 || num_conns==0) {
		return(0);
	} else {
		return(conn_num);
	}
}
#endif	//NOT_FOR_WIN95

/***************************************************************************
 * IPX_Get_Internet_Address -- gets Network Number & Node Address				*
 *                                                                         *
 * Once you've obtained a Connection Number from IPX_Get_Connection_Number	*
 * or IPX_Get_1st_Connection_Num, use this function to translate it into	*
 * a Network Number and Node Address; then, place those numbers in the		*
 * IPX header for outgoing packets.														*
 *                                                                         *
 * INPUT:                                                                  *
 *		connection_number		Connection Number to translate						*
 *		network_number			ptr: will hold Network Number							*
 *		physical_node			ptr: will hold Node Address							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = OK, -1 = error																	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		If connection_number is 0 and NETX isn't running, this routine			*
 *		will just put garbage into the network_number and physical_node.		*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
int IPX_Get_Internet_Address(int connection_number,
	unsigned char *network_number, unsigned char *physical_node)
{
	struct request_buffer {
		unsigned short  len;
		unsigned char   buffer_type;				// Internet = 0x13
		unsigned char   connection_number;		// Conn. Number to translate
	};
	struct reply_buffer {
		unsigned short  len;
		unsigned char   network_number [4];		// filled in by IPX
		unsigned char   physical_node  [6];		// filled in by IPX
		unsigned short  server_socket;			// filled in by IPX, but don't use!
	};
	union REGS regs;
	struct SREGS sregs;
	RMIType rmi;
	struct request_buffer *reqbuf;
	struct reply_buffer *replybuf;
	unsigned short segment;							// for DOS allocation
	unsigned short selector;						// for DOS allocation

	/*------------------------------------------------------------------------
	Error if invalid connection is given
	------------------------------------------------------------------------*/
	if (connection_number==0)
		return(-1);

	/*------------------------------------------------------------------------
	Allocate DOS memory to store the buffers passed to the interrupt
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_ALLOC_DOS_MEM;						// DPMI function to call
	regs.x.ebx = (sizeof(struct request_buffer) + 	// # paragraphs to allocate
		sizeof(struct reply_buffer) + 15) >> 4;
	int386x (DPMI_INT, &regs, &regs, &sregs);			// allocate the memory
	/*........................................................................
	If the carry flag is set, DPMI is indicating an error.
	........................................................................*/
	if (regs.x.cflag) {
		return(-1);
	}

	/*........................................................................
	Get pointers to allocated memory.
	'reqbuf' is just the returned real-mode segment, multiplied by 16.
	'replybuf' is an offset from 'reqbuf'.
	........................................................................*/
	segment = regs.w.ax;
	selector = regs.w.dx;
	reqbuf = (struct request_buffer *)(segment << 4);
	replybuf = (struct reply_buffer *)
		(((char *)reqbuf) + sizeof (struct request_buffer));

	/*------------------------------------------------------------------------
	Init the contents of the request & reply buffers
	------------------------------------------------------------------------*/
	reqbuf->len = 2;
	reqbuf->buffer_type = 0x13;
	reqbuf->connection_number = (unsigned char)connection_number;
	replybuf->len = 12;
	replybuf->network_number[0] = replybuf->network_number[0];	// suppress warning
	replybuf->physical_node[0] = replybuf->physical_node[0];		// suppress warning
	replybuf->server_socket = replybuf->server_socket;				// suppress warning

	/*------------------------------------------------------------------------
	Invoke Int 21 with AH=0xe3, DS:SI=&request_buffer, ES:DI=&reply_buffer
	------------------------------------------------------------------------*/
	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = 0x21;						// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.eax = 0x0000e300;
	rmi.ds = segment;
	rmi.esi = 0;
	rmi.es = segment;
	rmi.edi = sizeof(struct request_buffer);
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

	memcpy(network_number, replybuf->network_number, 4);
	memcpy(physical_node,  replybuf->physical_node,  6);

	/*------------------------------------------------------------------------
	Free DOS memory
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_FREE_DOS_MEM;						// DPMI function to call
	regs.x.edx = selector;									// ptr to free
	int386x (DPMI_INT, &regs, &regs, &sregs);			// allocate the memory

	return(0);

}	/* end of IPX_Get_Internet_Address */

#endif	// NOT_FOR_WIN95

/***************************************************************************
 * IPX_Get_User_ID -- gets user ID from Connection Number                  *
 *                                                                         *
 * INPUT:                                                                  *
 *		connection_number		Connection Number to get User ID for				*
 *		user_id					ptr to buffer to put User ID into; 					*
 *									size must be >= 48 chars								*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = OK, -1 = error																	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
int IPX_Get_User_ID(int connection_number, char *user_id)
{
	struct request_buffer {
		unsigned short  len;
		unsigned char   buffer_type;			// 0x16 = UserID buffer type
		unsigned char   connection_number;	// Connection Number to get ID for
	};
	struct reply_buffer {
		unsigned short  len;
		unsigned char   object_id[4];
		unsigned char   object_type[2];
		char            object_name[48];
		char            login_time[7];
		unsigned short  reserved;
	};
	union REGS      regs;
	struct SREGS    sregs;
	RMIType rmi;
	struct request_buffer *reqbuf;
	struct reply_buffer *replybuf;
	unsigned short segment;							// for DOS allocation
	unsigned short selector;						// for DOS allocation

	/*------------------------------------------------------------------------
	Error if invalid connection is given
	------------------------------------------------------------------------*/
	if (connection_number==0)
		return(-1);

	/*------------------------------------------------------------------------
	Allocate DOS memory to store the buffers passed to the interrupt
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_ALLOC_DOS_MEM;						// DPMI function to call
	regs.x.ebx = (sizeof(struct request_buffer) + 	// # paragraphs to allocate
		sizeof(struct reply_buffer) + 15) >> 4;
	int386x (DPMI_INT, &regs, &regs, &sregs);			// allocate the memory
	/*........................................................................
	If the carry flag is set, DPMI is indicating an error.
	........................................................................*/
	if (regs.x.cflag) {
		return(-1);
	}

	/*........................................................................
	Get pointers to allocated memory.
	'reqbuf' is just the returned real-mode segment, multiplied by 16.
	'replybuf' is an offset from 'reqbuf'.
	........................................................................*/
	segment = regs.w.ax;
	selector = regs.w.dx;
	reqbuf = (struct request_buffer *)(segment << 4);
	replybuf = (struct reply_buffer *)
		(((char *)reqbuf) + sizeof (struct request_buffer));

	/*------------------------------------------------------------------------
	Init the contents of the request & reply buffers
	------------------------------------------------------------------------*/
	reqbuf->len = 2;
	reqbuf->buffer_type = 0x16;
	reqbuf->connection_number = (unsigned char)connection_number;
	replybuf->len = sizeof(struct reply_buffer) - 2;
	replybuf->object_id[0] = replybuf->object_id[0];		// suppress warnings
	replybuf->object_type[0] = replybuf->object_type[0];	// suppress warnings
	replybuf->login_time[0] = replybuf->login_time[0];		// suppress warnings
	replybuf->reserved = replybuf->reserved;					// suppress warnings

	/*------------------------------------------------------------------------
	Invoke Int 21 with AH=0xe3, DS:SI=&request_buffer, ES:DI=&reply_buffer
	------------------------------------------------------------------------*/
	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = 0x21;						// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.eax = 0x0000e300;
	rmi.ds = segment;
	rmi.esi = 0;
	rmi.es = segment;
	rmi.edi = sizeof(struct request_buffer);
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

	/*------------------------------------------------------------------------
	Fill in the caller's buffer with the user name
	------------------------------------------------------------------------*/
	strncpy(user_id, replybuf->object_name, 48);

	/*------------------------------------------------------------------------
	Free DOS memory
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_FREE_DOS_MEM;						// DPMI function to call
	regs.x.edx = selector;									// ptr to free
	int386x (DPMI_INT, &regs, &regs, &sregs);			// allocate the memory

	return(0);

}	/* end of IPX_Get_User_ID */
#endif	//NOT_FOR_WIN95


/***************************************************************************
 * IPX_Listen_For_Packet -- commands IPX to listen for a packet            *
 *                                                                         *
 * Before calling this function, you must fill in an ECB:						*
 *		SocketNumber:				must contain the socket you've opened,			*
 *										and are "listening" on								*
 *		Event_Service_Routine:	optionally points to a callback routine		*
 *		PacketCount:				set to 2, to tell IPX there are 2 areas to	*
 *										store the incoming data in							*
 *		Packet[0].Address:		set to the address of an IPXHeaderType			*
 *		Packet[0].Length:			sizeof(IPXHeaderType)								*
 *		Packet[1].Address:		address of data buffer, for the packet			*
 *		Packet[1].Length:			size of the data buffer								*
 *                                                                         *
 * When the packet is received, ECBType.CompletionCode will be 0 if 			*
 * successful.  Otherwise, some error occurred.										*
 *                                                                         *
 * You should initialize the ECB to 0's before filling it in.					*
 *                                                                         *
 * INPUT:                                                                  *
 *		ecb_ptr		pointer to a filled-in ECB; MUST be real-mode memory		*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = OK, IPX error otherwise														*
 *                                                                         *
 * WARNINGS:                                                               *
 *		The ECB must be located in real-mode memory, as well as the values	*
 *		pointed to in the ECB (the IPX Header, the buffer to send, etc.)		*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
int IPX_Listen_For_Packet(struct ECB *ecb_ptr)
{
	union REGS      regs;
	struct SREGS    sregs;
	RMIType rmi;

	/*------------------------------------------------------------------------
	Call IPX with ES:SI=ecb_ptr
	------------------------------------------------------------------------*/
	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = IPX_INT;					// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.ebx = IPX_LISTEN_FOR_PACKET;
	rmi.es = (short)((long)ecb_ptr >> 4);
	rmi.esi = (long)((long)ecb_ptr & 0x000f);
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

	return(rmi.eax & 0x00ff);

}	/* end of IPX_Listen_For_Packet */
#endif	//NOT_FOR_WIN95

/***************************************************************************
 * IPX_Send_Packet -- commands IPX to send a packet                        *
 *                                                                         *
 * Before calling this function, you must fill in an ECB:						*
 *		SocketNumber:				must contain the socket you've opened,			*
 *										and are sending on									*
 *		Event_Service_Routine:	optionally points to a callback routine		*
 *		PacketCount:				set to 2, to tell IPX there are 2 areas the	*
 *										outgoing data is stored in							*
 *		Packet[0].Address:		set to the address of an IPXHeaderType			*
 *		Packet[0].Length:			sizeof(IPXHeaderType)								*
 *		Packet[1].Address:		address of buffer containing data to send		*
 *		Packet[1].Length:			size of the data buffer								*
 *		ImmediateAddress:			must be filled in with the node address of	*
 *										the bridge that will route the message;		*
 *										fill this in by calling IPX_Get_Local_Target	*
 *                                                                         *
 * Also, you must fill in the IPXHeaderType with certain values:				*
 *		PacketType:					set to 4 for IPX										*
 *		DestNetworkNumber:		Network Number of the destination system		*
 *		DestNetworkNode:			Node Address of the destination system			*
 *		DestNetworkSocket:		the destination system's socket to send to;	*
 *										this doesn't have to be the same as the		*
 *										socket opened on the local machine.				*
 *                                                                         *
 * You should initialize the ECB & IPXHeader to 0's before filling them in.*
 *                                                                         *
 * INPUT:                                                                  *
 *		ecb_ptr		pointer to a filled-in ECB											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.  This function doesn't return anything; the caller must check	*
 *    its ECB.CompletionCode for: 0 = OK, IPX Error otherwise.					*
 *                                                                         *
 * WARNINGS:                                                               *
 *		The ECB must be located in real-mode memory, as well as the values	*
 *		pointed to in the ECB (the IPX Header, the buffer to send, etc.)		*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
void IPX_Send_Packet(struct ECB *ecb_ptr)
{
	union REGS      regs;
	struct SREGS    sregs;
	RMIType rmi;

	/*------------------------------------------------------------------------
	Call IPX with ES:SI=ecb_ptr
	------------------------------------------------------------------------*/
	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = IPX_INT;					// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.ebx = IPX_SEND_PACKET;
	rmi.es = (short)((long)ecb_ptr >> 4);
	rmi.esi = (long)((long)ecb_ptr & 0x000f);
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

}	/* end of IPX_Send_Packet */
#endif	//NOT_FOR_WIN95

/***************************************************************************
 * IPX_Get_Local_Target -- fills in ImmediateAddress field of ECB          *
 *                                                                         *
 * Use this function to fill in the ECB's ImmediateAddress field when		*
 * sending a packet.  The Immediate Address is the node address of the		*
 * bridge that must route the message.  If there is no bridge, it's 			*
 * filled in with the destination Node Address.  In either case, it			*
 * will contain the proper value for sending.										*
 *                                                                         *
 * INPUT:                                                                  *
 *		dest_network			destination Network Number								*
 *		dest_node				destination Node Address								*
 *		dest_socket				destination Socket Number								*
 *		bridge_address			field to fill in with Immediate Address			*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = OK, error otherwise																*
 *                                                                         *
 * WARNINGS:                                                               *
 *		If the Connection Number is 0 (user not logged in), dest_network		*
 *		and dest_node will be garbage, and this routine will probably crash.	*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
int IPX_Get_Local_Target(unsigned char *dest_network, unsigned char *dest_node,
	unsigned short dest_socket, unsigned char *bridge_address)
{
	struct request_buffer {
		unsigned char   network_number [4];
		unsigned char   physical_node  [6];
		unsigned short  socket;
	};
	struct reply_buffer {
		unsigned char   local_target [6];
	};
	unsigned int rc;
	union REGS regs;
	struct SREGS sregs;
	RMIType rmi;
	struct request_buffer *reqbuf;
	struct reply_buffer *replybuf;
	unsigned short segment;							// for DOS allocation
	unsigned short selector;						// for DOS allocation

	/*------------------------------------------------------------------------
	Allocate DOS memory to store the buffers passed to the interrupt
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_ALLOC_DOS_MEM;						// DPMI function to call
	regs.x.ebx = (sizeof(struct request_buffer) + 	// # paragraphs to allocate
		sizeof(struct reply_buffer) + 15) >> 4;
	int386x (DPMI_INT, &regs, &regs, &sregs);			// allocate the memory
	/*........................................................................
	If the carry flag is set, DPMI is indicating an error.
	........................................................................*/
	if (regs.x.cflag) {
		return(-1);
	}

	/*........................................................................
	Get pointers to allocated memory.
	'reqbuf' is just the returned real-mode segment, multiplied by 16.
	'replybuf' is an offset from 'reqbuf'.
	........................................................................*/
	segment = regs.w.ax;
	selector = regs.w.dx;
	reqbuf = (struct request_buffer *)(segment << 4);
	replybuf = (struct reply_buffer *)
		(((char *)reqbuf) + sizeof (struct request_buffer));

	/*------------------------------------------------------------------------
	Init the contents of the request & reply buffers
	------------------------------------------------------------------------*/
	memcpy(reqbuf->network_number, dest_network, 4);
	memcpy(reqbuf->physical_node, dest_node, 6);
	reqbuf->socket = dest_socket;

	/*------------------------------------------------------------------------
	Invoke IPX with DS:SI=&request_buffer, ES:DI=&reply_buffer
	------------------------------------------------------------------------*/
	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = IPX_INT;					// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.ebx = IPX_GET_LOCAL_TARGET;
	rmi.ds = segment;
	rmi.esi = 0;
	rmi.es = segment;
	rmi.edi = sizeof(struct request_buffer);
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

	rc = (rmi.eax & 0x00ff);
	memcpy(bridge_address, replybuf->local_target, 6);

	/*------------------------------------------------------------------------
	Free DOS memory
	------------------------------------------------------------------------*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	regs.x.eax = DPMI_FREE_DOS_MEM;						// DPMI function to call
	regs.x.edx = selector;									// ptr to free
	int386x (DPMI_INT, &regs, &regs, &sregs);			// allocate the memory

	return(rc);

}	/* end of IPX_Get_Local_Target */

#endif	//NOT_FOR_WIN95

/***************************************************************************
 * IPX_Cancel_Event -- cancels an operation in progress                    *
 *                                                                         *
 * INPUT:                                                                  *
 *		ecb_ptr		pointer to ECB event to cancel									*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		???																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		The ECB must be located in real-mode memory, as well as the values	*
 *		pointed to in the ECB (the IPX Header, the buffer to send, etc.)		*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
int IPX_Cancel_Event(struct ECB *ecb_ptr)
{
	union REGS      regs;
	struct SREGS    sregs;
	RMIType rmi;
	unsigned int rc;

	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = IPX_INT;					// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.ebx = IPX_CANCEL_EVENT;
	rmi.es = (short)((long)ecb_ptr >> 4);
	rmi.esi = (long)((long)ecb_ptr & 0x000f);
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

	rc = (rmi.eax & 0x00ff);

	return(rc);

}	/* end of IPX_Cancel_Event */
#endif	//NOT_FOR_WIN95

/***************************************************************************
 * Let_IPX_Breath -- gives IPX some CPU time                               *
 *                                                                         *
 * Use this function if you're polling the ECB's InUse flag, waiting			*
 * for it to go to 0:																		*
 *                                                                         *
 *		while ECBType.InUse																	*
 *			Let_IPX_Breath();																	*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/15/1994 BR : Created.                                              *
 *=========================================================================*/
#ifdef NOT_FOR_WIN95
void Let_IPX_Breath(void)
{
	union REGS      regs;
	struct SREGS    sregs;
	RMIType rmi;

	/*........................................................................
	Fill in registers for the DPMI call
	........................................................................*/
	memset (&regs, 0 ,sizeof(regs));
	segread (&sregs);
	memset (&rmi, 0 ,sizeof(rmi));
	regs.w.ax = DPMI_CALL_REAL_INT;	// DPMI function to call
	regs.w.bx = IPX_INT;					// interrupt # to invoke
	sregs.es = FP_SEG(&rmi);			// tell DPMI where the RMI is
	regs.x.edi = FP_OFF(&rmi);
	/*........................................................................
	Fill in registers for the interrupt call
	........................................................................*/
	rmi.ebx = IPX_RELINQUISH_CONTROL;
	/*........................................................................
	call DPMI
	........................................................................*/
	int386x(DPMI_INT, &regs, &regs, &sregs);

}	/* end of Let_IPX_Breath */
#endif	//NOT_FOR_WIN95
