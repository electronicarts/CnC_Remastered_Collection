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
 *                    File Name : PACKET.CPP                               *
 *                                                                         *
 *                   Programmer : Philip W. Gorrow                         *
 *                                                                         *
 *                   Start Date : 04/22/96                                 *
 *                                                                         *
 *                  Last Update : April 24, 1996 [PWG]                     *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   *PacketClass::Find_Field -- Finds a field if it exists in the packets *
 *   Get_Field -- Find specified name and returns data                     *
 *   PacketClass::~PacketClass -- destroys a packet class be freeing list  *
 *   PacketClass::Add_Field -- Adds a FieldClass entry to head of packet li*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if (0) //PG

#include <stdlib.h>
#include <mem.h>
#include <string.h>

enum {false=0,true=1};
typedef int bool;

#include "packet.h"


/**************************************************************************
 * PACKETCLASS::~PACKETCLASS -- destroys a packet class be freeing list   *
 *                                                                        *
 * INPUT:		none                                                       *
 *                                                                        *
 * OUTPUT:     none																		  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/24/1996 PWG : Created.                                            *
 *========================================================================*/
PacketClass::~PacketClass(void)
{
	FieldClass *current;
	FieldClass *next;
	//
	// Loop through the entire field list and delete each entry.
	//
	for (current = Head; current; current = next) {
		next = current->Next;
		delete current;
	}
}


/**************************************************************************
 * PACKETCLASS::ADD_FIELD -- Adds a FieldClass entry to head of packet li *
 *                                                                        *
 * INPUT:		FieldClass * - a properly constructed field class entry.	  *
 *                                                                        *
 * OUTPUT:     none                                                       *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/24/1996 PWG : Created.                                            *
 *========================================================================*/
void PacketClass::Add_Field(FieldClass *field)
{
	field->Next = Head;
	Head = field;
}

/**************************************************************************
 * PACKETCLASS::PACKETCLASS -- Creates a Packet object from a COMMS packe *
 *                                                                        *
 * INPUT:                                                                 *
 *                                                                        *
 * OUTPUT:                                                                *
 *                                                                        *
 * WARNINGS:                                                              *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/22/1996 PWG : Created.                                            *
 *========================================================================*/
PacketClass::PacketClass(char *curbuf)
{
	int remaining_size;
	//
	// Pull the size and packet ID out of the linear packet stream.
	//
	Size = *(unsigned short *)curbuf;
	curbuf += sizeof (unsigned short);
	Size = ntohs(Size);
	ID	  = *(short *)curbuf;
	curbuf += sizeof (short);
	ID   = ntohs(ID);
	Head = NULL;

	//
	// Calculate the remaining size so that we can loop through the
	//   packets and extract them.
	//
	remaining_size = Size - 4;

	//
	// Loop through the linear packet until we run out of room and
	// create a field for each.
	//
	while (remaining_size > 0) {
		FieldClass *field = new FieldClass;

		//
		// Copy the adjusted header into the buffer and then advance the buffer
		//
		memcpy(field, curbuf, FIELD_HEADER_SIZE);
		curbuf += FIELD_HEADER_SIZE;
		remaining_size   -= FIELD_HEADER_SIZE;

		//
		// Copy the data into the buffer
		//
		int size			= ntohs(field->Size);
		field->Data		= new char[size];
		memcpy(field->Data, curbuf, size);
		curbuf			+= size;
		remaining_size	-= size;
		//
		// Make sure we allow for the pad bytes.
		//
		int pad = (4 - (ntohs(field->Size) & 3)) & 3;
		curbuf += pad;
		remaining_size   -= pad;

		//
		// Convert the field back to the host format
		//
		field->Net_To_Host();

		//
		// Finally add the field to the field list in the packet
		// structure.
		//
		Add_Field(field);
	}
}

/**************************************************************************
 * CREATE_COMMS_PACKET -- Walks field list creating a packet              *
 *                                                                        *
 * INPUT:		short - the id of the packet so the server can identify it *
 *					unsigned short & - the size of the packet returned here    *
 *                                                                        *
 * OUTPUT:     void * pointer to the linear packet data                   *
 *                                                                        *
 * WARNINGS: 	This routine allocates memory that the user is responsible *
 *  				for freeing.															  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/22/1996 PWG : Created.                                            *
 *========================================================================*/
char *PacketClass::Create_Comms_Packet(int &size)
{
	FieldClass *current;

	//
	// Size starts at four because that is the size of the packet header.
	//
	size = 4;

	//
	// Take a quick spin through and calculate the size of the packet we
	//   are building.
	//
	for (current = Head; current; current=current->Next) {
		size += (unsigned short)FIELD_HEADER_SIZE;			// add in packet header size
		size += current->Size;				// add in data size
		size += (4 - (size & 3)) & 3; 	// add in pad value to dword align next packet
	}

	//
	// Now that we know the size allocate a buffer big enough to hold the
	// packet.
	//
	char *retval = new char[size];
	char *curbuf = retval;

	//
	// write the size into the packet header
	//
	*(unsigned short *)curbuf = (unsigned short)htons((unsigned short)size);
	curbuf += sizeof (unsigned short);
	*(short *)curbuf = htons(ID);
	curbuf += sizeof (short);

	//
	// Ok now that the actual header information has been written we need to write out
	// field information.
	//
	for (current = Head; current; current = current->Next) {
		//
		// Temporarily convert the packet to net format (this saves alot of
		//   effort, and seems safe...)
		//
		current->Host_To_Net();

		//
		// Copy the adjusted header into the buffer and then advance the buffer
		//
		memcpy(curbuf, current, FIELD_HEADER_SIZE);
		curbuf += FIELD_HEADER_SIZE;

		//
		// Copy the data into the buffer and then advance the buffer
		//
		memcpy(curbuf, current->Data, ntohs(current->Size));
		curbuf += ntohs(current->Size);

		//
		// Finally take care of any pad bytes by setting them to 0
		//
		int pad = (4 - (ntohs(current->Size) & 3)) & 3;

		//
		//	If there is any pad left over, make sure you memset it
		// to zeros, so it looks like a pad.
		//
		if (pad) {
			memset(curbuf, 0, pad);
			curbuf += pad;
		}

		current->Net_To_Host();
	}
	return(retval);
}


/**************************************************************************
 * PACKETCLASS::FIND_FIELD -- Finds a field if it exists in the packets   *
 *                                                                        *
 * INPUT:		char *  - the id of the field we are looking for.			  *
 *                                                                        *
 * OUTPUT:     FieldClass * pointer to the field class                    *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
FieldClass *PacketClass::Find_Field(char *id)
{
	for (FieldClass *current = Head; current; current = current->Next) {
		if ( strncmp(id, current->ID, 4) == 0)
			return current;
	}
	return NULL;
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:		char *   - the id of the field that holds the data.		  *
 *					char &   - the reference to store the data into				  *
 *                                                                        *
 * OUTPUT:		true if the field was found, false if it was not.			  *
 *                                                                        *
 * WARNINGS:	The data reference is not changed if the field is not		  *
 *					found.																	  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
bool PacketClass::Get_Field(char *id, char &data)
{
	FieldClass *field = Find_Field(id);
	if (field) {
		data = *((char *)field->Data);
	}
	return((field) ? true : false);
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:		char *   - the id of the field that holds the data.		  *
 *					unsigned char &   - the reference to store the data into	  *
 *                                                                        *
 * OUTPUT:		true if the field was found, false if it was not.			  *
 *                                                                        *
 * WARNINGS:	The data reference is not changed if the field is not		  *
 *					found.																	  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
bool PacketClass::Get_Field(char *id, unsigned char &data)
{
	FieldClass *field = Find_Field(id);
	if (field) {
		data = *((unsigned char *)field->Data);
	}
	return((field) ? true : false);
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:		char *   - the id of the field that holds the data.		  *
 *					short &   - the reference to store the data into	        *
 *                                                                        *
 * OUTPUT:		true if the field was found, false if it was not.			  *
 *                                                                        *
 * WARNINGS:	The data reference is not changed if the field is not		  *
 *					found.																	  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
bool PacketClass::Get_Field(char *id, short &data)
{
	FieldClass *field = Find_Field(id);
	if (field) {
		data = *((short *)field->Data);
	}
	return((field) ? true : false);
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:		char *   - the id of the field that holds the data.		  *
 *					unsigned short &   - the reference to store the data into  *
 *                                                                        *
 * OUTPUT:		true if the field was found, false if it was not.			  *
 *                                                                        *
 * WARNINGS:	The data reference is not changed if the field is not		  *
 *					found.																	  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
bool PacketClass::Get_Field(char *id, unsigned short &data)
{
	FieldClass *field = Find_Field(id);
	if (field) {
		data = *((unsigned short *)field->Data);
	}
	return((field) ? true : false);
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:		char *   - the id of the field that holds the data.		  *
 *					long &   - the reference to store the data into  			  *
 *                                                                        *
 * OUTPUT:		true if the field was found, false if it was not.			  *
 *                                                                        *
 * WARNINGS:	The data reference is not changed if the field is not		  *
 *					found.																	  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
bool PacketClass::Get_Field(char *id, long &data)
{
	FieldClass *field = Find_Field(id);
	if (field) {
		data = *((long *)field->Data);
	}
	return((field) ? true : false);
}

/**************************************************************************
 * GET_FIELD -- Find specified name and returns data as a string          *
 *                                                                        *
 * INPUT:		char *   - the id of the field that holds the data.		  *
 *					char *   - the string to store the data into					  *
 *                                                                        *
 * OUTPUT:		true if the field was found, false if it was not.			  *
 *                                                                        *
 * WARNINGS:	The string is not changed if the field is not found.  It   *
 *					is assumed that the string variabled specified by the      *
 *					pointer is large enough to hold the data.						  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
bool PacketClass::Get_Field(char *id, char *data)
{
	FieldClass *field = Find_Field(id);
	if (field) {
		strcpy(data, (char *)field->Data);
	}
	return((field) ? true : false);
}

/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:		char *   - the id of the field that holds the data.		  *
 *					unsigned long &   - the reference to store the data into   *
 *                                                                        *
 * OUTPUT:		true if the field was found, false if it was not.			  *
 *                                                                        *
 * WARNINGS:	The data reference is not changed if the field is not		  *
 *					found.																	  *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
bool PacketClass::Get_Field(char *id, unsigned long &data)
{
	FieldClass *field = Find_Field(id);
	if (field) {
		data = *((unsigned long *)field->Data);
	}
	return((field) ? true : false);
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:		char *   - the id of the field that holds the data.		    *
 *				void * - the reference to store the data into                *
 *           int    - the length of the buffer passed in                  *
 *                                                                        *
 * OUTPUT:		true if the field was found, false if it was not.			 *
 *                                                                        *
 * WARNINGS:	The data reference is not changed if the field is not		    *
 *					found.																	 *
 *                                                                        *
 * HISTORY:                                                               *
 *   6/4/96 4:46PM ST : Created                                           *
 *========================================================================*/
bool PacketClass::Get_Field(char *id, void *data, int &length)
{
	FieldClass *field = Find_Field(id);
	if (field) {
		memcpy (data, field->Data, min(field->Size, length));
		length = (int) field->Size;
	}
	return((field) ? true : false);
}
#endif