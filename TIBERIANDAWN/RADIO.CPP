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

/* $Header:   F:\projects\c&c\vcs\code\radio.cpv   2.19   16 Oct 1995 16:50:12   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RADIO.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : June 25, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   RadioClass::Debug_Dump -- Displays the current status of the radio to the mono monitor.   *
 *   RadioClass::Receive_Message -- Handles receipt of a radio message.                        *
 *   RadioClass::Transmit_Message -- Transmit message from one object to another.              * 
 *   RadioClass::Limbo -- When limboing a unit will always break radio contact.                * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/*
**	These are the text representations of the radio messages that can be transmitted.
*/
char const * RadioClass::Messages[RADIO_COUNT] = {
	"hisssss",
	"Roger.",
	"Come in.",
	"Over and out.",
	"Requesting transport.",
	"Attach to transport.",
	"I've got a delivery for you.",
	"I'm performing load/unload maneuver. Be careful.",
	"I'm clear.",
	"You are clear to unload. Driving away now.",
	"Am unable to comply.",
	"I'm starting construction now... act busy.",
	"I've finished construction. You are free.",
	"We bumped, redraw yourself please.",
	"I'm trying to load up now.",
	"May I become a passenger?",
	"Are you ready to receive shipment?",
	"Are you trying to become a passenger?",
	"Move to location X.",
	"Do you need to move?",
	"All right already. Now what?",
	"I'm a passenger now.",
	"Backup into refinery now.",
	"Run away!",
	"Tether established.",
	"Tether broken.",
	"Repair one step.",
	"Are you prepared to fight?",
	"Attack this target please.",
	"Reload one step.",
	"Take this kick! You... You...",
	"Take this punch! You... You...",
	"Fancy a little fisticuffs, eh?"
};


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * RadioClass::Debug_Dump -- Displays the current status of the radio to the mono monitor.     *
 *                                                                                             *
 *    This displays the radio connection value to the monochrome monitor.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void RadioClass::Debug_Dump(MonoClass *mono) const
{
	mono->Set_Cursor(34, 5);mono->Printf(Messages[LastMessage]);
	if (Radio) {
		mono->Set_Cursor(50, 1);mono->Printf("%04X", Radio->As_Target());
	}
	MissionClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * RadioClass::Receive_Message -- Handles receipt of a radio message.                          *
 *                                                                                             *
 *    This is the base version of what should happen when a radio message is received. It      *
 *    turns the radio off when the "OVER_OUT" message is received. All other messages are      *
 *    merely acknowledged with a "ROGER".                                                      *
 *                                                                                             *
 * INPUT:   from     -- The object that is initiating this radio message (always valid).        *
 *                                                                                             *
 *          message  -- The radio message received.                                            *
 *                                                                                             *
 *          param    -- Reference to optional value that might be used to return more          *
 *                      information than can be conveyed in the simple radio response          *
 *                      messages.                                                              *
 *                                                                                             *
 * OUTPUT:  Returns with the response radio message.                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *   09/24/1994 JLB : Streamlined to be only a communications carrier.                         *
 *   05/22/1995 JLB : Recognized who is sending the message                                    * 
 *=============================================================================================*/
RadioMessageType RadioClass::Receive_Message(RadioClass * from, RadioMessageType message, long & param)
{
	/*
	**	Keep a record of the last message received by this radio.
	*/
	LastMessage = message;

	/*
	**	When this message is received, it means that the other object
	**	has already turned its radio off. Turn this radio off as well.
	**	This only applies if the message is coming from the object that
	**	has an established conversation with this object.
	*/
	if (from == Radio && message == RADIO_OVER_OUT) {
		MissionClass::Receive_Message(from, message, param);
		Radio_Off();
		return(RADIO_ROGER);
	}

	/*
	**	The "hello" message is an attempt to establish contact. If this radio
	**	is already in an established conversation with another object, then 
	**	return with "negative". If all is well, return with "roger".
	*/
	if (message == RADIO_HELLO && Strength) {
		if (Radio == from || !Radio) {
			Radio = from;
			return(RADIO_ROGER);
		}
		return(RADIO_NEGATIVE);
	}

	return(MissionClass::Receive_Message(from, message, param));
}


/*********************************************************************************************** 
 * RadioClass::Transmit_Message -- Transmit message from one object to another.                * 
 *                                                                                             * 
 *    This routine is used to transmit a radio message from this object to another. Most       * 
 *    inter object coordination is handled through this mechanism.                             * 
 *                                                                                             * 
 * INPUT:   to       -- Pointer to the object that will receive the radio message.             * 
 *                                                                                             * 
 *          message  -- The message itself (see RadioType).                                    * 
 *                                                                                             * 
 *          param    -- Optional reference to parameter that might be used to pass or          * 
 *                      receive additional information.                                        * 
 *                                                                                             * 
 * OUTPUT:  Returns with the response radio message from the receiving object.                 * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   05/22/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
RadioMessageType RadioClass::Transmit_Message(RadioMessageType message, long & param, RadioClass * to)
{
	if (!to) {
		to = Contact_With_Whom();
	}

	/*
	**	If there is no target for the radio message, then always return static.
	*/
	if (!to) return(RADIO_STATIC);

	/*
	**	Handle some special case processing that occurs when certain messages
	**	are transmitted.
	*/
	if (to == Radio && message == RADIO_OVER_OUT) {
		Radio = 0;
	}

	/*
	**	If this object is not in radio contact but the message
	**	indicates that radio contact should be established, then
	**	try to do so. If the other party agrees then contact
	**	is established.
	*/
	if (/*!Radio &&*/ message == RADIO_HELLO) {
		Transmit_Message(RADIO_OVER_OUT);
		if (to->Receive_Message(this, message, param) == RADIO_ROGER) {
			Radio = to;
			return(RADIO_ROGER);
		}
		return(RADIO_NEGATIVE);
	}

	return(to->Receive_Message(this, message, param));
}


/*********************************************************************************************** 
 * RadioClass::Limbo -- When limboing a unit will always break radio contact.                  * 
 *                                                                                             * 
 *    This routine will break radio contact as the object is entering limbo state.             * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Was the object successfully limboed?                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/25/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
bool RadioClass::Limbo(void)
{
	if (!IsInLimbo) {
		Transmit_Message(RADIO_OVER_OUT);
	}
	return(MissionClass::Limbo());
}


RadioMessageType RadioClass::Transmit_Message(RadioMessageType message, RadioClass * to) {return(Transmit_Message(message, LParam, to));};
