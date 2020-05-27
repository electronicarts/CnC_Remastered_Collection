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

/* $Header: /CounterStrike/RADIO.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RADIO.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 23, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 23, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef RADIO_H
#define RADIO_H

#include	"mission.h"

class ObjectClass;
class TechnoClass;


/****************************************************************************
**	Radio contact is controlled by this class. It handles the mundane chore
**	of keeping the radio contact alive as well as broadcasting messages
**	to the receiving radio. Radio contact is primarily used when one object
**	is in "command" of another.
*/
class RadioClass : public MissionClass {
	private:

		/*
		**	This is a record of the last message received by this receiver.
		*/
		RadioMessageType Old[3];

		/*
		**	This is the object that radio communication has been established
		**	with. Although is is only a one-way reference, it is required that
		**	the receiving radio is also tuned to the object that contains this
		**	radio set.
		*/
		RadioClass * Radio;

		/*
		**	This is a text representation of all the possible radio messages. This
		**	text is used for monochrome debug printing.
		*/
		static char const * Messages[RADIO_COUNT];

	public:

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		RadioClass(RTTIType rtti, int id) : MissionClass(rtti, id), Radio(0) {};
		RadioClass(NoInitClass const & x) : MissionClass(x) {};
		virtual ~RadioClass(void) {Radio=0;};

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		bool In_Radio_Contact(void) const {return (Radio != 0);};
		void Radio_Off(void) {Radio = 0;};
		TechnoClass * Contact_With_Whom(void) const {return (TechnoClass *)Radio;};

		// Inherited from base class(es).
		virtual RadioMessageType Receive_Message(RadioClass * from, RadioMessageType message, long & param);
		virtual RadioMessageType Transmit_Message(RadioMessageType message, long & param=LParam, RadioClass * to=NULL);
		virtual RadioMessageType Transmit_Message(RadioMessageType message, RadioClass * to);
		#ifdef CHEAT_KEYS
		virtual void Debug_Dump(MonoClass *mono) const;
		#endif
		virtual bool Limbo(void);

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);
};

#endif
