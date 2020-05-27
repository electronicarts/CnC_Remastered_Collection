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

/* $Header:   F:\projects\c&c\vcs\code\event.h_v   2.19   16 Oct 1995 16:46:14   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : EVENT.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/09/94                                                     *
 *                                                                                             *
 *                  Last Update : December 9, 1994 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef EVENT_H
#define EVENT_H

/*
**	This event class is used to contain all external game events (things that the player can
**	do at any time) so that these events can be transported between linked computers. This
**	encapsulation is required in order to ensure that each event affects all computers at the
**	same time (same game frame).
*/
class EventClass
{
	public:

		/*
		**	All external events are identified by these labels.
		*/
		typedef enum EventType : unsigned char {
			EMPTY,

			ALLY,					// Make allie of specified house.
			MEGAMISSION,		// Full change of mission with target and destination.
			IDLE,					// Request to enter idle mode.
			SCATTER,				// Request to scatter from current location.
			DESTRUCT,			// Self destruct request (surrender action).
			DEPLOY,				// MCV is to deploy at current location.
			PLACE,				// Place building at location specified.
			OPTIONS,				// Bring up options screen.
			GAMESPEED,			// Set game speed
			PRODUCE,				// Start or Resume production.
			SUSPEND,				// Suspend production.
			ABANDON,				// Abandon production.
			PRIMARY,				// Primary factory selected.
			SPECIAL_PLACE,		// Special target location selected
			EXIT,					// Exit game.
			ANIMATION,			// Flash ground as movement feedback.
			REPAIR,				// Repair specified object.
			SELL,					//	Sell specified object.
			SPECIAL,				// Special options control.

			// Private events.
			FRAMESYNC,			// Game-connection packet; includes Scenario CRC & sender's frame #
									// Used to initiate game connection phase & to reconnect;
									// When one of these is received, the receiver knows there are
									// no associated commands in this packet.
			MESSAGE,				// Message to another player (The message is the 40 bytes
									// after the event class).
			RESPONSE_TIME,		// use a new propogation delay value
			FRAMEINFO,			// Game-heartbeat packet; includes Game CRC & command count
									// All packets sent for a frame are prefixed with one of these
			ARCHIVE,				// Updates archive target on specified object.
			TIMING,				// new timing values for all systems to use
			PROCESS_TIME,		// a system's average processing time, in ticks per frame
			LAST_EVENT,			// one past the last event
		} EventType;

		EventType Type;		// Type of queue command object.

		/*
		** 'Frame' is the frame that the command should execute on.
		** 27 bits gives over 25 days of playing time without wrapping,
		** at 30 frames per second, so it should be plenty!
		*/
		unsigned Frame : 27;

		/*
		** House index of the player originating this event
		*/
		unsigned ID : 4;

		/*
		** This bit tells us if we've already executed this event.
		*/
		unsigned IsExecuted: 1;

		/*
		** Multiplayer ID of the player originating this event.
		** High nybble: the color index of this player.
		** Low nybble: the HousesType this player is "acting like" (GDI/NOD)
		*/
		unsigned char MPlayerID;

		/*
		**	This union contains the specific data that the event requires.
		*/
		union {
			struct {
				SpecialClass	Data;		// The special option flags.
			} Options;
			struct {
				TARGET		Whom;			// The object to apply the event to.
			} Target;
			struct {
				AnimType		What;			// The animation to create.
				HousesType	Owner;		// The owner of the animation (when it matters).
				COORDINATE			Where;		// The location to place the animation.
			} Anim;
			struct {
				int 			Value;		// general-purpose data
			} General;
			struct {
				TARGET		Whom;			// Whom to apply mission to.
				MissionType	Mission;		// What mission to apply.
				TARGET		Target;		// Target to assign.
				TARGET		Destination;// Destination to assign.
			} MegaMission;
			struct {
				TARGET		Whom;			// Whom to apply mission to.
				MissionType Mission;		// What mission to apply.
			} Mission;
			struct {
				TARGET		Whom;			// Whom to apply movement change to.
				TARGET		Where;		// Where to set NavCom to.
			} NavCom;
			struct {
				TARGET		Whom;			// Whom to apply attack change to.
				TARGET		Target;		// What to set TarCom to.
			} TarCom;
			struct {
				RTTIType		Type;
				int			ID;
			} Specific;
			struct {
				RTTIType		Type;
				CELL			Cell;
			} Place;
			struct {
				int			ID;
				CELL			Cell;
			} Special;
			/*
			** This structure is used for FRAMEINFO, FRAMESYNC, and RESPONSE_TIME
			** events; exactly one of these will be sent each frame, whether there's
			** data that frame or not.
			** CRC: the game CRC when this packet was generated; used to detect sync errors
			** CommandCount: # of commands the sender has sent; used to detect missed packets
			** Delay: sender's propogation delay value for this frame
			*/
			struct {
				unsigned long CRC;
				unsigned short CommandCount;	// # commands sent so far
				unsigned char Delay;				// propogation delay used this frame
														// (Frame - Delay = sender's current frame #)
			} FrameInfo;

			//
			// This structure sets new timing values for all systems in a multiplayer
			// game.  This structure replaces the RESPONSE_TIME event for
			// the COMM_MULTI_E_COMP protocol.
			//
			struct {
				unsigned short DesiredFrameRate;
				unsigned short MaxAhead;
			} Timing;

			//
			// This structure is transmitted by all systems, and is used to compute
			// the "desired" frame rate for the game.
			//
			struct {
				unsigned short AverageTicks;
			} ProcessTime;

		} Data;

		//-------------- Functions ---------------------
		EventClass(void) {Type = EMPTY;};
		EventClass(SpecialClass data);
		EventClass(EventType type, TARGET target);
		EventClass(EventType type);
		EventClass(EventType type, int val);
		EventClass(EventType type, TARGET src, TARGET dest);
//		EventClass(TARGET src, MissionType mission);
		EventClass(TARGET src, MissionType mission, TARGET target=TARGET_NONE, TARGET destination=TARGET_NONE);
		EventClass(EventType type, RTTIType object, int id);
		EventClass(EventType type, RTTIType object, CELL cell);
		EventClass(EventType type, int id, CELL cell);
		EventClass(AnimType anim, HousesType owner, COORDINATE coord);

		// Process the event.
		void Execute(void);

		int operator == (EventClass & q) {
			return memcmp(this, &q, sizeof(q)) == 0;
		};

		static unsigned char EventLength[LAST_EVENT];
		static char * EventNames[LAST_EVENT];
};

#endif
