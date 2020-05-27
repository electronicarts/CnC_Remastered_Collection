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
 *                                                                         *
 *                 Project Name : Westwood Auto Registration App           *
 *                                                                         *
 *                    File Name : FIELD.CPP                                *
 *                                                                         *
 *                   Programmer : Philip W. Gorrow                         *
 *                                                                         *
 *                   Start Date : 04/22/96                                 *
 *                                                                         *
 *                  Last Update : April 22, 1996 [PWG]                     *
 *                                                                         *
 *  Actual member function for the field class.                            *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <string.h>
#include "field.h"

FieldClass::FieldClass(char *id, char data)
{
	strncpy(ID, id, sizeof(ID));
	DataType = TYPE_CHAR;
	Size		= sizeof(data);
	Data		= new char[Size];
	memcpy(Data, &data, Size);
	Next		= NULL;
}

FieldClass::FieldClass(char *id, unsigned char data)
{
	strncpy(ID, id, sizeof(ID));
	DataType = TYPE_UNSIGNED_CHAR;
	Size		= sizeof(data);
	Data		= new char[Size];
	memcpy(Data, &data, Size);
	Next		= NULL;
}

FieldClass::FieldClass(char *id, short data)
{
	strncpy(ID, id, sizeof(ID));
	DataType = TYPE_SHORT;
	Size		= sizeof(data);
	Data		= new char[Size];
	memcpy(Data, &data, Size);
	Next		= NULL;
}

FieldClass::FieldClass(char *id, unsigned short data)
{
	strncpy(ID, id, sizeof(ID));
	DataType = TYPE_UNSIGNED_SHORT;
	Size		= sizeof(data);
	Data		= new char[Size];
	memcpy(Data, &data, Size);
	Next		= NULL;
}

FieldClass::FieldClass(char *id, long data)
{
	strncpy(ID, id, sizeof(ID));
	DataType = TYPE_LONG;
	Size		= sizeof(data);
	Data		= new char[Size];
	memcpy(Data, &data, Size);
	Next		= NULL;
}

FieldClass::FieldClass(char *id, unsigned long data)
{
	strncpy(ID, id, sizeof(ID));
	DataType = TYPE_UNSIGNED_LONG;
	Size		= sizeof(data);
	Data		= new char[Size];
	memcpy(Data, &data, Size);
	Next		= NULL;
}

FieldClass::FieldClass(char *id, char *data)
{
	strncpy(ID, id, sizeof(ID));
	DataType = TYPE_STRING;
	Size		= (unsigned short)(strlen(data)+1);
	Data		= new char[Size];
	memcpy(Data, data, Size);
	Next		= NULL;
}

FieldClass::FieldClass(char *id, void *data, int length)
{
	strncpy(ID, id, sizeof(ID));
	DataType = TYPE_CHUNK;
	Size		= (unsigned short)length;
	Data		= new char[Size];
	memcpy(Data, data, Size);
	Next		= NULL;
}


/**************************************************************************
 * PACKETCLASS::HOST_TO_NET_FIELD -- Converts host field to net format    *
 *                                                                        *
 * INPUT:		FIELD 	* to the data field we need to convert				  *
 *                                                                        *
 * OUTPUT:     none																		  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/22/1996 PWG : Created.                                            *
 *========================================================================*/
void FieldClass::Host_To_Net(void)
{
	//
	// Before we convert the data type, we should convert the actual data
	//  sent.
	//
	switch (DataType) {
		case TYPE_CHAR:
		case TYPE_UNSIGNED_CHAR:
		case TYPE_STRING:
		case TYPE_CHUNK:
			break;

		case TYPE_SHORT:
		case TYPE_UNSIGNED_SHORT:
			*((unsigned short *)Data) = htons(*((unsigned short *)Data));
			break;

		case TYPE_LONG:
		case TYPE_UNSIGNED_LONG:
			*((unsigned long *)Data) = htonl(*((unsigned long *)Data));
			break;

		//
		// Might be good to insert some type of error message here for unknown
		//   datatypes -- but will leave that for later.
		//
		default:
			break;
	}
	//
	// Finally convert over the data type and the size of the packet.
	//
	DataType = htons(DataType);
	Size 	 	= htons(Size);
}
/**************************************************************************
 * PACKETCLASS::NET_TO_HOST_FIELD -- Converts net field to host format    *
 *                                                                        *
 * INPUT:		FIELD 	* to the data field we need to convert				  *
 *                                                                        *
 * OUTPUT:     none																		  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/22/1996 PWG : Created.                                            *
 *========================================================================*/
void FieldClass::Net_To_Host(void)
{
	//
	// Convert the variables to host order.  This needs to be converted so
	// the switch statement does compares on the data that follows.
	//
	Size 	 	= ntohs(Size);

	DataType = ntohs(DataType);

	//
	// Before we convert the data type, we should convert the actual data
	//  sent.
	//
	switch (DataType) {
		case TYPE_CHAR:
		case TYPE_UNSIGNED_CHAR:
		case TYPE_STRING:
		case TYPE_CHUNK:
			break;

		case TYPE_SHORT:
		case TYPE_UNSIGNED_SHORT:
			*((unsigned short *)Data) = ntohs(*((unsigned short *)Data));
			break;

		case TYPE_LONG:
		case TYPE_UNSIGNED_LONG:
			*((unsigned long *)Data) = ntohl(*((unsigned long *)Data));
			break;

		//
		// Might be good to insert some type of error message here for unknown
		//   datatypes -- but will leave that for later.
		//
		default:
			break;
	}
}

