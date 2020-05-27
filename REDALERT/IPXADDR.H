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

/* $Header: /CounterStrike/IPXADDR.H 1     3/03/97 10:24a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPXADDR.H                                *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : December 19, 1994   [BR]                 *
 *                                                                         *
 * This class is useful for any IPX-related code.  It's just a utility		*
 * to help manage those annoying IPX address fields.  This class lets you	*
 * compare addresses, copy addresses to & from the IPX header, etc.			*
 *                                                                         *
 * The class has no virtual functions, so you can treat this class just		*
 * like a data structure; it can be loaded & saved, and even transmitted	*
 * across the net.																			*
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef IPXADDR_H
#define IPXADDR_H

#include "ipx.h"				// for NetNumType & NetNodeType


/*
***************************** Class Declaration *****************************
*/
class IPXAddressClass
{
	/*
	---------------------------- Public Interface ----------------------------
	*/
	public:
		/*.....................................................................
		Constructors:
		.....................................................................*/
		IPXAddressClass(void);
		IPXAddressClass(NetNumType net, NetNodeType node);
		IPXAddressClass(IPXHeaderType *header);

		/*.....................................................................
		Set the address from explicit variables, or from the SOURCE values
		in an IPX packet header.
		.....................................................................*/
		void Set_Address(NetNumType net, NetNodeType node);
		void Set_Address(IPXHeaderType *header);
		/*.....................................................................
		Get the address values explicitly, or copy them into the DESTINATION
		values in an IPX packet header.
		.....................................................................*/
		void Get_Address (NetNumType net, NetNodeType node);
		void Get_Address(IPXHeaderType *header);

		/*.....................................................................
		Tells if this address is a broadcast address
		.....................................................................*/
		int Is_Broadcast(void);

		/*.....................................................................
		Overloaded operators:
		.....................................................................*/
		int operator == (IPXAddressClass & addr);
		int operator != (IPXAddressClass & addr);
		int operator > (IPXAddressClass &addr);
		int operator < (IPXAddressClass &addr);
		int operator >= (IPXAddressClass &addr);
		int operator <= (IPXAddressClass &addr);
	/*
	-------------------------- Protected Interface ---------------------------
	*/
	protected:
	/*
	--------------------------- Private Interface ----------------------------
	*/
	private:
		NetNumType NetworkNumber;
		NetNodeType NodeAddress;
};

#endif

/**************************** end of ipxaddr.h *****************************/
