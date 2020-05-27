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

/* $Header: /CounterStrike/IPXADDR.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPXADDR.CPP                              *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : December 19, 1994   [BR]                 *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   IPXAddressClass::IPXAddressClass -- class constructor                 *
 *   IPXAddressClass::IPXAddressClass -- class constructor form 2				*
 *   IPXAddressClass::IPXAddressClass -- class constructor form 3				*
 *   IPXAddressClass::Set_Address -- sets the IPX address values        	*
 *   IPXAddressClass::Set_Address -- sets the IPX values from a header		*
 *   IPXAddressClass::Get_Address -- retrieves the IPX address values      *
 *   IPXAddressClass::Get_Address -- copies address into an IPX header		*
 *   IPXAddressClass::Is_Broadcast -- tells if this is a broadcast address *
 *   IPXAddressClass::operator== -- overloaded comparison operator         *
 *   IPXAddressClass::operator!= -- overloaded comparison operator         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "function.h"
#include <stdio.h>
//#include <mem.h>
#include "ipxaddr.h"

#ifdef WINSOCK_IPX
#include	"WSProto.h"
#endif	//WINSOCK_IPX


/***************************************************************************
 * IPXAddressClass::IPXAddressClass -- class constructor                   *
 *                                                                         *
 * This default constructor generates a broadcast address.						*
 *                                                                         *
 * INPUT:                                                                  *
 *		net		Network Number for this address										*
 *		node		Node Address for this address											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
IPXAddressClass::IPXAddressClass(void)
{
	NetworkNumber[0] = 0xff;
	NetworkNumber[1] = 0xff;
	NetworkNumber[2] = 0xff;
	NetworkNumber[3] = 0xff;
	NodeAddress[0] = 0xff;
	NodeAddress[1] = 0xff;
	NodeAddress[2] = 0xff;
	NodeAddress[3] = 0xff;
	NodeAddress[4] = 0xff;
	NodeAddress[5] = 0xff;

}	/* end of IPXAddressClass */


/***************************************************************************
 * IPXAddressClass::IPXAddressClass -- class constructor form 2				*
 *                                                                         *
 * INPUT:                                                                  *
 *		net		Network Number for this address										*
 *		node		Node Address for this address											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
IPXAddressClass::IPXAddressClass(NetNumType net, NetNodeType node)
{
	memcpy(NetworkNumber, net, 4);
	memcpy(NodeAddress, node, 6);

}	/* end of IPXAddressClass */


/***************************************************************************
 * IPXAddressClass::IPXAddressClass -- class constructor form 3				*
 *                                                                         *
 * This form of the constructor takes an IPX header as an argument.  It		*
 * extracts the address from the Source address fields in the header.		*
 *                                                                         *
 * INPUT:                                                                  *
 *		header	Header from which to extract the address							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
IPXAddressClass::IPXAddressClass(IPXHeaderType *header)
{
	memcpy(NetworkNumber,header->SourceNetworkNumber,4);
	memcpy(NodeAddress,header->SourceNetworkNode,6);

}	/* end of IPXAddressClass */


/***************************************************************************
 * IPXAddressClass::Set_Address -- sets the IPX address values        		*
 *                                                                         *
 * INPUT:                                                                  *
 *		net		Network Number for this address										*
 *		node		Node Address for this address											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
void IPXAddressClass::Set_Address(NetNumType net, NetNodeType node)
{
	memcpy(NetworkNumber,net,4);
	memcpy(NodeAddress,node,6);

}	/* end of Set_Address */


/***************************************************************************
 * IPXAddressClass::Set_Address -- sets the IPX values from a header			*
 *                                                                         *
 * This routine extracts the source addresses from the given IPX header.	*
 *                                                                         *
 * INPUT:                                                                  *
 *		net		Network Number for this address										*
 *		node		Node Address for this address											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
void IPXAddressClass::Set_Address(IPXHeaderType *header)
{
#ifdef WINSOCK_IPX
	ProtocolEnum protocol = PROTOCOL_IPX;
	if ( PacketTransport ) protocol = PacketTransport->Get_Protocol();

	switch ( protocol ) {

		case PROTOCOL_IPX:
			memcpy(NetworkNumber,header->SourceNetworkNumber,4);
			memcpy(NodeAddress,header->SourceNetworkNode,6);
			break;

		case PROTOCOL_UDP:
			unsigned char *addr = (unsigned char*) header;
			memset (NodeAddress, 0, 6);
			memcpy (NodeAddress, addr, 4);
			memset (NetworkNumber, 0, 4);
			break;
	}
#else	//WINSOCK_IPX

	if (header) {
		memcpy(NetworkNumber,header->SourceNetworkNumber,4);
		memcpy(NodeAddress,header->SourceNetworkNode,6);
	} else {
		/*
		** Address is meaningless when using winsock
		*/
		memset(NetworkNumber, 1 ,4);
		memset(NodeAddress, 1, 6);
	}

#endif	//WINSOCK_IPX

}	/* end of Set_Address */


/***************************************************************************
 * IPXAddressClass::Get_Address -- retrieves the IPX address values        *
 *                                                                         *
 * INPUT:                                                                  *
 *		net		Network Number for this address										*
 *		node		Node Address for this address											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
void IPXAddressClass::Get_Address(NetNumType net, NetNodeType node)
{
	memcpy(net,NetworkNumber,4);
	memcpy(node,NodeAddress,6);

}	/* end of Get_Address */


/***************************************************************************
 * IPXAddressClass::Get_Address -- copies address into an IPX header			*
 *                                                                         *
 * INPUT:                                                                  *
 *		net		Network Number for this address										*
 *		node		Node Address for this address											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
void IPXAddressClass::Get_Address(IPXHeaderType *header)
{
	memcpy(header->DestNetworkNumber,NetworkNumber,4);
	memcpy(header->DestNetworkNode,NodeAddress,6);

}	/* end of Get_Address */


/***************************************************************************
 * IPXAddressClass::Is_Broadcast -- tells if this is a broadcast address   *
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
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXAddressClass::Is_Broadcast(void)
{
	if (	NetworkNumber[0] == 0xff &&
			NetworkNumber[1] == 0xff &&
			NetworkNumber[2] == 0xff &&
			NetworkNumber[3] == 0xff &&
			NodeAddress[0] == 0xff &&
			NodeAddress[1] == 0xff &&
			NodeAddress[2] == 0xff &&
			NodeAddress[3] == 0xff &&
			NodeAddress[4] == 0xff &&
			NodeAddress[5] == 0xff) {
		return(1);
	}
	else {
		return(0);
	}

}	/* end of Is_Broadcast */


/***************************************************************************
 * IPXAddressClass::operator== -- overloaded comparison operator           *
 *                                                                         *
 * Since, if NETX isn't running, the network number on a received packet	*
 * can be bogus (all 0's), only the node address is used for comparison 	*
 * purposes here.																				*
 *                                                                         *
 * INPUT:                                                                  *
 *		addr		address to compare to													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = not equal, 1 = equal															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXAddressClass::operator == (IPXAddressClass & addr)
{
	//------------------------------------------------------------------------
	//	If either Network Number is all 0's (which can happen if the system is
	//	not running NETX), compare only the Node Addresses.
	//------------------------------------------------------------------------
	if ( (NetworkNumber[0]==0 &&
			NetworkNumber[1]==0 &&
			NetworkNumber[2]==0 &&
			NetworkNumber[3]==0) ||
		  (addr.NetworkNumber[0]==0 &&
			addr.NetworkNumber[1]==0 &&
			addr.NetworkNumber[2]==0 &&
			addr.NetworkNumber[3]==0) ) {

		if (memcmp(NodeAddress,addr.NodeAddress,6)==0) {
			return(1);
		}
		else {
			return(0);
		}

	}
	//------------------------------------------------------------------------
	//	Otherwise, compare both the Network Numbers and Node Addresses
	//------------------------------------------------------------------------
	else {
		if (memcmp(NodeAddress,addr.NodeAddress,6)==0 &&
			memcmp(NetworkNumber,addr.NetworkNumber,4)==0) {
			return(1);
		}
		else {
			return(0);
		}
	}

}	/* end of operator== */


/***************************************************************************
 * IPXAddressClass::operator!= -- overloaded comparison operator           *
 *                                                                         *
 * Since, if NETX isn't running, the network number on a received packet	*
 * can be bogus (all 0's), only the node address is used for comparison 	*
 * purposes here.																				*
 *                                                                         *
 * INPUT:                                                                  *
 *		addr		address to compare to													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		0 = equal, 1 = not equal															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXAddressClass::operator != (IPXAddressClass & addr)
{
	//------------------------------------------------------------------------
	//	If either Network Number is all 0's (which can happen if the system is
	//	not running NETX), compare only the Node Addresses.
	//------------------------------------------------------------------------
	if ( (NetworkNumber[0]==0 &&
			NetworkNumber[1]==0 &&
			NetworkNumber[2]==0 &&
			NetworkNumber[3]==0) ||
		  (addr.NetworkNumber[0]==0 &&
			addr.NetworkNumber[1]==0 &&
			addr.NetworkNumber[2]==0 &&
			addr.NetworkNumber[3]==0) ) {

		if (memcmp(NodeAddress,addr.NodeAddress,6)==0) {
			return(0);
		}
		else {
			return(1);
		}
	}
	//------------------------------------------------------------------------
	//	Otherwise, compare both the Network Numbers and Node Addresses
	//------------------------------------------------------------------------
	else {
		if (memcmp(NodeAddress,addr.NodeAddress,6)==0 &&
			memcmp(NetworkNumber,addr.NetworkNumber,4)==0) {
			return(0);
		}
		else {
			return(1);
		}
	}

}	/* end of operator!= */


/***************************************************************************
 * IPXAddressClass::operator > -- overloaded comparison operator           *
 *                                                                         *
 * INPUT:                                                                  *
 *		addr		address to compare to													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		TRUE = greater, FALSE = not														*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXAddressClass::operator > (IPXAddressClass & addr)
{
	return(memcmp(this, &addr, 10) > 0);

}	/* end of operator> */


/***************************************************************************
 * IPXAddressClass::operator < -- overloaded comparison operator           *
 *                                                                         *
 * INPUT:                                                                  *
 *		addr		address to compare to													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		TRUE = less, FALSE = not															*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXAddressClass::operator < (IPXAddressClass & addr)
{
	return(memcmp(this, &addr, 10) < 0);

}	/* end of operator< */


/***************************************************************************
 * IPXAddressClass::operator >= -- overloaded comparison operator          *
 *                                                                         *
 * INPUT:                                                                  *
 *		addr		address to compare to													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		TRUE = greater or equal, FALSE = not											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXAddressClass::operator >= (IPXAddressClass & addr)
{
	return(memcmp(this, &addr, 10) >= 0);

}	/* end of operator>= */


/***************************************************************************
 * IPXAddressClass::operator <= -- overloaded comparison operator          *
 *                                                                         *
 * INPUT:                                                                  *
 *		addr		address to compare to													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		TRUE = less or equal, FALSE = not												*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXAddressClass::operator <= (IPXAddressClass & addr)
{
	return(memcmp(this, &addr, 10) <= 0);

}	/* end of operator<= */

/************************** end of ipxaddr.cpp *****************************/
