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

/* $Header:   F:\projects\c&c0\vcs\code\queue.cpv   2.24   11 Oct 1995 13:47:40   JOE_BOSTIC  $ */
/***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : QUEUE.CPP                                *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 11/28/95                                 *
 *                                                                         *
 *                  Last Update : November 28, 1995 [BRR]                  *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions for Queueing Events:                                          *
 *   Queue_Mission -- Queue a mega mission event.                          *
 *   Queue_Options -- Queue the options event.                             *
 *   Queue_Exit -- Add the exit game event to the queue.                   *
 *                                                                         *
 * Functions for processing Queued Events:											*
 *   Queue_AI -- Process all queued events.                                *
 *   Queue_AI_Normal -- Process all queued events.                         *
 *   Queue_AI_Multiplayer -- Process all queued events.                    *
 *                                                                         *
 * Main Multiplayer Queue Logic:															*
 *   Wait_For_Players -- Waits for other systems to come on-line           *
 *   Generate_Timing_Event -- computes & queues a RESPONSE_TIME event      *
 *   Process_Send_Period -- timing for sending packets every 'n' frames    *
 *   Send_Packets -- sends out events from the OutList                     *
 *   Send_FrameSync -- Sends a FRAMESYNC packet                            *
 *   Process_Receive_Packet -- processes an incoming packet                *
 *   Process_Serial_Packet -- Handles an incoming serial packet            *
 *   Can_Advance -- determines if it's OK to advance to the next frame     *
 *   Process_Reconnect_Dialog -- processes the reconnection dialog         *
 *   Handle_Timeout -- attempts to reconnect; if fails, bails.             *
 *   Stop_Game -- stops the game															*
 *                                                                         *
 * Packet Compression / Decompression:													*
 *   Build_Send_Packet -- Builds a big packet from a bunch of little ones.	*
 *   Add_Uncompressed_Events -- adds uncompressed events to a packet       *
 *   Add_Compressed_Events -- adds compressed events to a packet        	*
 *   Breakup_Receive_Packet -- Splits a big packet into little ones.			*
 *   Extract_Uncompressed_Events -- extracts events from a packet				*
 *   Extract_Compressed_Events -- extracts events from a packet            *
 *                                                                         *
 * DoList Management:																		*
 *   Execute_DoList -- Executes commands from the DoList                   *
 *   Clean_DoList -- Cleans out old events from the DoList                 *
 *   Queue_Record -- Records the DoList to disk                            *
 *   Queue_Playback -- plays back queue entries from a record file         *
 *                                                                         *
 * Debugging:																					*
 *   Compute_Game_CRC -- Computes a CRC value of the entire game.				*
 *   Add_CRC -- Adds a value to a CRC                                      *
 *   Print_CRCs -- Prints a data file for finding Sync Bugs						*
 *   Init_Queue_Mono -- inits mono display                                 *
 *   Update_Queue_Mono -- updates mono display                             *
 *   Print_Framesync_Values -- displays frame-sync variables               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include	"function.h"
#include 	"tcpip.h"

/********************************** Defines *********************************/
#define SHOW_MONO	1

int tmp_flag = 0;

/********************************** Globals *********************************/
//---------------------------------------------------------------------------
//	GameCRC is the current computed CRC value for this frame.
//	CRC[] is a record of our last 32 game CRC's.
// ColorNames is for debug output in Print_CRCs
//---------------------------------------------------------------------------
#ifndef DEMO
static unsigned long GameCRC;
static unsigned long CRC[32] =
	{0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0};

static char *ColorNames[6] = {
	"Yellow",
	"Red",
	"BlueGreen",
	"Orange",
	"Green",
	"Blue",
};
#endif	//DEMO

//...........................................................................
// Mono debugging variables:
// NetMonoMode: 0 = show connection output, 1 = flowcount output
// NewMonoMode: set by anything that toggles NetMonoMode; re-inits screen
// IsMono: used for taking control of Mono screen away from the engine
//...........................................................................
#ifndef DEMO
int NetMonoMode = 1;
int NewMonoMode = 1;
static int IsMono = 0;
#endif	//DEMO

//---------------------------------------------------------------------------
// Several routines return various codes; here's an enum for all of them.
//---------------------------------------------------------------------------
typedef enum RetcodeEnum {
	RC_NORMAL,						// no news is good news
	RC_PLAYER_READY,				// a new player has been heard from
	RC_SCENARIO_MISMATCH,		// scenario mismatch
	RC_DOLIST_FULL,				// DoList is full
	RC_SERIAL_PROCESSED,			// modem: SERIAL packet was processed
	RC_PLAYER_LEFT,				// modem: other player left the game
	RC_HUNG_UP,						// modem has hung up
	RC_NOT_RESPONDING,			// other player not responding (timeout/hung up)
	RC_CANCEL,						// user cancelled
} RetcodeType;


/********************************* Prototypes *******************************/
//...........................................................................
// Main multiplayer queue logic
//...........................................................................
static void Queue_AI_Normal(void);
#ifndef DEMO
static void Queue_AI_Multiplayer(void);
static RetcodeType Wait_For_Players(int first_time, ConnManClass *net,
	int resend_delta, int dialog_time, int timeout, char *multi_packet_buf,
	int my_sent, long *their_frame,  unsigned short *their_sent,
	unsigned short *their_recv);
static void Generate_Timing_Event(ConnManClass *net, int my_sent);
static void Generate_Real_Timing_Event(ConnManClass *net, int my_sent);
static void Generate_Process_Time_Event(ConnManClass *net);
static int Process_Send_Period(ConnManClass *net);
static int Send_Packets(ConnManClass *net, char *multi_packet_buf,
	int multi_packet_max, int max_ahead, int my_sent);
static void Send_FrameSync(ConnManClass *net, int cmd_count);
static RetcodeType Process_Receive_Packet(ConnManClass *net,
	char *multi_packet_buf, int id, int packetlen, long *their_frame,
	unsigned short *their_sent, unsigned short *their_recv);
static RetcodeType Process_Serial_Packet(char *multi_packet_buf,
	int first_time);
static int Can_Advance(ConnManClass *net, int max_ahead, long *their_frame,
	unsigned short *their_sent, unsigned short *their_recv);
static int Process_Reconnect_Dialog(CountDownTimerClass *timeout_timer,
	long *their_frame, int num_conn, int reconn, int fresh);
static int Handle_Timeout(ConnManClass *net, long *their_frame,
	unsigned short *their_sent, unsigned short *their_recv);
static void Stop_Game(void);
#endif	//DEMO

//...........................................................................
// Packet compression/decompression:
//...........................................................................
#ifndef DEMO
static int Build_Send_Packet(void *buf, int bufsize, int frame_delay,
	int num_cmds, int cap);
static int Breakup_Receive_Packet(void *buf, int bufsize );
#endif	//DEMO
int Add_Uncompressed_Events(void *buf, int bufsize, int frame_delay, int size,
	int cap);
int Add_Compressed_Events(void *buf, int bufsize, int frame_delay, int size,
	int cap);
int Extract_Uncompressed_Events(void *buf, int bufsize);
int Extract_Compressed_Events(void *buf, int bufsize);

//...........................................................................
// DoList management:
//...........................................................................
static int Execute_DoList(int max_houses, HousesType base_house,
	ConnManClass *net, TCountDownTimerClass *skip_crc,
	long *their_frame, unsigned short *their_sent, unsigned short *their_recv);
static void Clean_DoList(ConnManClass *net);
#ifndef DEMO
static void Queue_Record(void);
static void Queue_Playback(void);
#endif	//DEMO

//...........................................................................
// Debugging:
//...........................................................................
#ifndef DEMO
static void Compute_Game_CRC(void);
static void Init_Queue_Mono(ConnManClass *net);
static void Update_Queue_Mono(ConnManClass *net, int flow_index);
static void Print_Framesync_Values(long curframe, unsigned long max_ahead,
	int num_connections, unsigned short *their_recv,
	unsigned short *their_sent, unsigned short my_sent);
#endif	//DEMO
void Add_CRC(unsigned long *crc, unsigned long val);
void Print_CRCs(EventClass *);

extern void Keyboard_Process(KeyNumType &input);
void Dump_Packet_Too_Late_Stuff(EventClass *event);

extern void Register_Game_End_Time(void);
extern void Send_Statistics_Packet(void);

/***************************************************************************
 * Queue_Mission -- Queue a mega mission event.                            *
 *                                                                         *
 * This routine is called when the player causes a change to a game unit. 	*
 * The event that initiates the change is queued to as a result of a call 	*
 * to this routine.                 													*
 *                                                                         *
 * INPUT:                                                                  *
 *		whom		Whom this mission request applies to (a friendly unit).     *
 *    mission	The mission to assign to this object.                       *
 *    target	The target of this mission (if any).                        *
 *    dest		The movement destination for this mission (if any).         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *		Was the mission request queued successfully?                         *
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.                                                                *
 *                                                                         *
 * HISTORY:                                                                *
 *   09/21/1995 JLB : Created.                                             *
 *=========================================================================*/
bool Queue_Mission(TARGET whom, MissionType mission, TARGET target,
	TARGET destination)
{
	if (! OutList.Add(EventClass(whom, mission, target, destination))) {
		return(false);
	}
	else {
		return(true);
	}

}	/* end of Queue_Mission */


/***************************************************************************
 * Queue_Options -- Queue the options event.                               *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		Was the options screen event queued successfully?                    *
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   09/21/1995 JLB : Created.                                             *
 *=========================================================================*/
bool Queue_Options(void)
{
	if (! OutList.Add(EventClass(EventClass::OPTIONS))) {
		return(false);
	}
	else {
		return(true);
	}

}		/* end of Queue_Options */


/***************************************************************************
 * Queue_Exit -- Add the exit game event to the queue.                     *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		Was the exit event queued successfully?                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   09/21/1995 JLB : Created.                                             *
 *=========================================================================*/
bool Queue_Exit(void)
{
	if (! OutList.Add(EventClass(EventClass::EXIT))) {
		return(false);
	}
	else {
		return(true);
	}

}		/* end of Queue_Exit */


/***************************************************************************
 * Queue_AI -- Process all queued events.                                  *
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
 *   09/21/1995 JLB : Created.                                             *
 *=========================================================================*/
void Queue_AI(void)
{
#ifdef DEMO
	Queue_AI_Normal();
#else	//DEMO

	if (PlaybackGame) {
		Queue_Playback();
	}

	else {

		switch (GameToPlay) {

			case GAME_NORMAL:
				Queue_AI_Normal();
				break;

			case GAME_MODEM:
			case GAME_NULL_MODEM:
			case GAME_IPX:
			case GAME_INTERNET:
				Queue_AI_Multiplayer();
				break;
		}
	}
#endif	//DEMO

}	/* end of Queue_AI */


/***************************************************************************
 * Queue_AI_Normal -- Process all queued events.                           *
 *                                                                         *
 * This is the "normal" version of the queue management routine.  It does 	*
 * the following:		  																		*
 * - Transfers items in the OutList to the DoList									*
 * - Executes any commands in the DoList that are supposed to be done on 	*
 *   this frame #			  																	*
 * - Cleans out the DoList																	*
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
 *   09/21/1995 JLB : Created.                                             *
 *=========================================================================*/
static void Queue_AI_Normal(void)
{
	//------------------------------------------------------------------------
	//	Move events from the OutList (events generated by this player) into the
	//	DoList (the list of events to execute).
	//------------------------------------------------------------------------
	while (OutList.Count) {
		OutList.First().IsExecuted = false;
		if (!DoList.Add(OutList.First())) {
			;
		}
		OutList.Next();
	}

	//------------------------------------------------------------------------
	// Save the DoList to disk, if we're in "Record" mode
	//------------------------------------------------------------------------
#ifndef DEMO
	if (RecordGame) {
		Queue_Record();
	}
#endif	//DEMO

	//------------------------------------------------------------------------
	// Execute the DoList
	//------------------------------------------------------------------------
	if (!Execute_DoList(1,PlayerPtr->Class->House, NULL, NULL, NULL,
		NULL, NULL)) {
		GameActive = 0;
		return;
	}
	//------------------------------------------------------------------------
	//	Clean out the DoList
	//------------------------------------------------------------------------
	Clean_DoList(NULL);

}	/* end of Queue_AI_Normal */

#ifndef DEMO

/***************************************************************************
 * Queue_AI_Multiplayer -- Process all queued events.                      *
 *                                                                         *
 * This is the network version of the queue management routine.  It does 	*
 * the following:																				*
 * - If this is the 1st frame, waits for other systems to signal ready		*
 * - Generates a timing event, to allow the connection time to be dynamic	*
 * - Handles timing related to sending packets every 'n' frames				*
 * - Sends outgoing events																	*
 * - Frame-syncs to the other systems (see below)									*
 * - Executes & cleans out the DoList													*
 *                                                                         *
 * The Frame-Sync'ing logic is the heart & soul of network play.  It works	*
 * by ensuring that any system won't out-run the other system by more than	*
 * 'MaxAhead' frames; this in turn ensures that a packet's 						*
 * execution frame # won't have been passed by the time that packet is 		*
 * received by all systems.																*
 *                                                                         *
 * To achieve this, the system must keep track of all other system's 		*
 * current frame #'s; these are stored in an array called 'their_frame[]'. *
 * However, because current frame #'s are sent in FRAMEINFO packets, which *
 * don't require an ACK, and command packets are sent in packets requiring *
 * an ACK, it's possible for a command packet to get lost, and the next 	*
 * frame's FRAMEINFO packet to not get lost; the other system may then 		*
 * advance past the frame # the command is to execute on!  So, to prevent 	*
 * this, all FRAMEINFO packets include a CommandCount field.  This value 	*
 * tells the other system how many events it should have received by this 	*
 * time.  This system can therefore keep track of how many commands it's	*
 * actually received, and compare it to the CommandCount field, to see if 	*
 * it's missed an event packet.  The # of events we've received from each 	*
 * system is stored in 'their_recv[]', and the # events they say they've 	*
 * sent is stored in 'their_sent[]'.													*
 *                                                                         *
 * Thus, two conditions must be met in order to advance to the next frame:	*
 * - Our current frame # must be <= their_frame + MaxAhead						*
 * - their_recv[i] must be >= their_sent[i]											*
 *                                                                         *
 * 'their_frame[] is updated by Process_Receive_Packet()							*
 * 'their_recv[] is updated by Process_Receive_Packet()							*
 * 'their_sent[] is updated by Process_Receive_Packet()							*
 * 'my_sent' is updated by this routine.												*
 *                                                                         *
 * The only routines allowed to pop up dialogs are:								*
 * 	Wait_For_Players() (only pops up the reconnect dialog)					*
 * 	Execute_DoList() (tells if out of sync, or packet recv'd too late)	*
 *                                                                         *
 * Sign-off's are detected by:															*
 * - Timing out while waiting for a packet											*
 * - Detecting that the other player is now at the score screen or 			*
 *   connection dialog (serial)															*
 * - If we see an EventClass::EXIT event on the private channel				*
 *                                                                         *
 * The current communications protocol, COMM_PROTOCOL_MULTI_E_COMP, has 	*
 * the following properites:																*
 * - It compresses packets, so that the minimum number of bytes are 			*
 *   transmitted.  Packets are compressed by extracting all info common to *
 *   the events into the packet header, and then sending only the bytes 	*
 *   relevant to each type of event.  For instance, if 100 infantry guys 	*
 *   are told to move to the same location, the command itself & the 		*
 *   location will be included in the 1st movement command only; after 		*
 *   that, there will be a rep count then 99 infantry TARGET numbers, 		*
 *   identifying all the infantry told to move.										*
 * - The protocol also only sends packets out every 'n' frames.  This cuts *
 *   the data rate dramatically.  It means that 'MaxAhead' must be 			*
 *   divisible by 'n'; also, the minimum value for 'MaxAhead' is 				*
 *   'n * 2', to give both sides some "breathing" room in case a FRAMEINFO	*
 *   packet gets missed.																	*
 *                                                                         *
 * Note:  For synchronization-waiting loops (like waiting to hear from all *
 * other players, waiting to advance to the next frame, etc), use 			*
 * Net.Num_Connections() rather than NumPlayers; this reflects the 			*
 * actual # of connections, and can be "faked" into playing even when 		*
 * there aren't any other players actually there.  A typical example of 	*
 * this is playing back a recorded game.  For command-execution loops, use *
 * NumPlayers.  This ensures all commands get executed, even if 				*
 * there isn't a human generating those commands.									*
 *                                                                         *
 * The modem works a little differently from the network in this respect:	*
 * - The connection has to stay "alive" even if the other player exits to 	*
 *   the join dialog.  This prevents each system from timing out & hanging *
 *   the modem up.  Thus, packets are sent back & forth & just thrown away,*
 *   but each system knows the other is still there.  Messages may be sent	*
 *   between systems, though. 															*
 * - Destroy_Null_Connection doesn't hang up the modem, so 						*
 *   Num_Connections() still reports a value of 1 even though the other 	*
 *   player has left.																		*
 * - Any waits on Num_Connections() must also check for 							*
 *   NumPlayers > 1, to keep from waiting forever if the other 				*
 *   guy has left																				*
 * - Packets sent to a player who's left require no ACK							*
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
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Queue_AI_Multiplayer(void)
{
	//........................................................................
	// Enums:
	//........................................................................
	enum {
		MIXFILE_RESEND_DELTA = 120,	// ticks b/w resends
		MIXFILE_TIMEOUT = 3600,			// timeout waiting for mixfiles
		FRAMESYNC_DLG_TIME = (3*60),	// time until displaying reconnect dialog
		FRAMESYNC_TIMEOUT = (25*60),	// timeout waiting for frame sync packet
	};

	int timeout_factor = (GameToPlay == GAME_INTERNET) ? 6 : 1;

	//........................................................................
	// Variables for sending, receiving & parsing packets:
	//........................................................................
	ConnManClass *net;					// ptr to access all multiplayer functions
	EventClass packet;					// for sending single frame-sync's
	char *multi_packet_buf;				// buffer for sending/receiving
	int multi_packet_max;				// max length of multi_packet_buf

	//........................................................................
	// Frame-sync'ing variables.  Values in these arrays are stored in the
	// order in which the connections are created.
	// (ie net->Connection_Index(id))
	//........................................................................
	static long
		their_frame[MAX_PLAYERS - 1];	// other players' frame #'s
	static unsigned short
		their_sent[MAX_PLAYERS - 1];	// # cmds other player claims to have sent
	static unsigned short
		their_recv[MAX_PLAYERS - 1];	// # cmds actually received from others
	static unsigned short
		my_sent;								// # cmds I've sent out

	//........................................................................
	// Other misc variables
	//........................................................................
	int i;
	RetcodeType rc;
	int reconnect_dlg = 0;				// 1 = the reconnect dialog is displayed

	//------------------------------------------------------------------------
	//	Initialize the packet buffer pointer & its max size
	//------------------------------------------------------------------------
	if (GameToPlay == GAME_MODEM
		|| GameToPlay == GAME_NULL_MODEM){
//PG_TO_FIX
#if (0)
		multi_packet_buf = NullModem.BuildBuf;
		multi_packet_max = NullModem.MaxLen - sizeof (CommHeaderType);
		net = &NullModem;
#endif
	}
	else if (GameToPlay == GAME_IPX || GameToPlay == GAME_INTERNET) {
		multi_packet_buf = MetaPacket;
		multi_packet_max = MetaSize;
		net = &Ipx;
	}

	//------------------------------------------------------------------------
	//	Debug stuff
	//------------------------------------------------------------------------
	Init_Queue_Mono(net);
	Update_Queue_Mono (net, 0);

	//------------------------------------------------------------------------
	//	If we've just started a game, or loaded a multiplayer game, we must
	// wait for all other systems to signal ready.
	//------------------------------------------------------------------------
	if (Frame==0) {
		//.....................................................................
		//	Initialize static locals
		//.....................................................................
		for (i = 0; i < MAX_PLAYERS - 1; i++) {
			their_frame[i] = -1;
			their_sent[i] = 0;
			their_recv[i] = 0;
			TheirProcessTime[i] = -1;
		}
		my_sent = 0;
		for (i = 0; i < 32; i++) {
			CRC[i] = 0;
		}

		//.....................................................................
		// Send our initial FRAMESYNC packet
		//.....................................................................
		Send_FrameSync(net, my_sent);

		//.....................................................................
		// Wait for the other guys
		//.....................................................................
		rc = Wait_For_Players (1, net, MIXFILE_RESEND_DELTA, FRAMESYNC_DLG_TIME*timeout_factor,
			MIXFILE_TIMEOUT, multi_packet_buf, my_sent, their_frame,
			their_sent, their_recv);

		if (rc != RC_NORMAL) {
			if (rc == RC_NOT_RESPONDING) {
				CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);
			}
			else if (rc == RC_SCENARIO_MISMATCH) {
				CCMessageBox().Process (TXT_SCENARIOS_DO_NOT_MATCH);
			}
			else if (rc == RC_DOLIST_FULL) {
				CCMessageBox().Process(TXT_QUEUE_FULL);
			}
			Stop_Game();
			return;
		}

		//.....................................................................
		//	Re-initialize frame numbers (in case somebody signed off while I was
		//	waiting for MIX files to load; we would have fallen through, but
		//	their frame # would still be -1).
		//.....................................................................
		for (i = 0; i < MAX_PLAYERS - 1; i++)
			their_frame[i] = 0;

		//.....................................................................
		//	Reset the network response time computation, now that we're both
		// sending data again  (loading MIX files will have introduced
		// deceptively large values).
		//.....................................................................
		net->Reset_Response_Time();

		//.....................................................................
		// Initialize the frame timers
		//.....................................................................
		if (CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
			Process_Send_Period(net);
		}

	} 	// end of Frame 0 wait

	//------------------------------------------------------------------------
	// Adjust connection timing parameters every 128 frames.
	//------------------------------------------------------------------------
	else if ( (Frame & 0x007f) == 0) {
		//
		// If we're using the new spiffy protocol, do proper timing handling.
		// If we're the net "master", compute our desired frame rate & new
		// 'MaxAhead' value.
		//
		if (CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {

			//
			// All systems will transmit their required process time.
			//
			Generate_Process_Time_Event(net);

			//
			// The game "host" will transmit timing adjustment events.
			//
			if (MPlayerLocalID == MPlayerID[0]) {
				Generate_Real_Timing_Event(net, my_sent);
			}
		} else {
			//
			// For the older protocols, do the old broken timing handling.
			//
			Generate_Timing_Event(net, my_sent);
		}
	}

	//------------------------------------------------------------------------
	//	Compute the Game's CRC
	//------------------------------------------------------------------------
	Compute_Game_CRC();
	CRC[Frame & 0x001f] = GameCRC;
	//unsigned long save_crc = GameCRC;
	//Print_CRCs((EventClass *)NULL);
	//GameCRC = save_crc;

	//------------------------------------------------------------------------
	// Only process every 'FrameSendRate' frames
	//------------------------------------------------------------------------
	if (CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		if (!Process_Send_Period(net)) {
			if (IsMono) {
				MonoClass::Disable();
			}
			return;
		}
	}

	//------------------------------------------------------------------------
	// Send our data packet(s); update my command-sent counter
	//------------------------------------------------------------------------
	my_sent += Send_Packets(net, multi_packet_buf, multi_packet_max,
		MPlayerMaxAhead, my_sent);

	//------------------------------------------------------------------------
	//	If this is our first time through, we're done.
	//------------------------------------------------------------------------
	if (Frame==0) {
		if (IsMono) {
			MonoClass::Disable();
		}
		return;
	}

	//------------------------------------------------------------------------
	//	Frame-sync'ing: wait until it's OK to advance to the next frame.
	//------------------------------------------------------------------------
	rc = Wait_For_Players (0, net,
		(MPlayerMaxAhead << 3),
		MAX ( net->Response_Time() * 3, (unsigned long)FRAMESYNC_DLG_TIME*timeout_factor ),
		FRAMESYNC_TIMEOUT * (timeout_factor*2),
		multi_packet_buf, my_sent, their_frame,
		their_sent, their_recv);

	if (rc != RC_NORMAL) {
		if (rc == RC_NOT_RESPONDING) {
			CCMessageBox().Process (TXT_SYSTEM_NOT_RESPONDING);
		}
		else if (rc == RC_SCENARIO_MISMATCH) {
			CCMessageBox().Process (TXT_SCENARIOS_DO_NOT_MATCH);
		}
		else if (rc == RC_DOLIST_FULL) {
			CCMessageBox().Process(TXT_QUEUE_FULL);
		}
		Stop_Game();
		return;
	}

	//------------------------------------------------------------------------
	//	Save the DoList to disk, if we're in "Record" mode
	//------------------------------------------------------------------------
	if (RecordGame) {
		Queue_Record();
	}

	//------------------------------------------------------------------------
	// Execute the DoList; if an error occurs, bail out.
	//------------------------------------------------------------------------
	if (!Execute_DoList(MPlayerMax, HOUSE_MULTI1, net, NULL,
		their_frame, their_sent, their_recv)) {
		Stop_Game();
		return;
	}

	//------------------------------------------------------------------------
	//	Clean out the DoList
	//------------------------------------------------------------------------
	Clean_DoList(net);

	if (IsMono) {
		MonoClass::Disable();
	}

}	// end of Queue_AI_Multiplayer


/***************************************************************************
 * Wait_For_Players -- Waits for other systems to come on-line             *
 *                                                                         *
 * This routine performs the most critical logic in multiplayer; that of	*
 * synchronizing my frame number with those of the other systems.				*
 *                                                                         *
 * INPUT:                                                                  *
 *		first_time				1 = 1st time this routine is called					*
 *		net						ptr to connection manager								*
 *		resend_delta			time (ticks) between FRAMESYNC resends				*
 *		dialog_time				time (ticks) until pop up a reconnect dialog		*
 *		timeout					time (ticks) until we give up the ghost			*
 *		multi_packet_buf		buffer to store packets in								*
 *		my_sent					# commands I've sent so far							*
 *		their_frame				array of their frame #'s								*
 *		their_sent				array of their CommandCount values					*
 *		their_recv				array of # cmds I've received from them			*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		RC_NORMAL				OK to advance to the next frame						*
 *		RC_CANCEL				user hit 'Cancel' at the timeout countdown dlg	*
 *		RC_NOT_RESPONDING		other player(s) not responding						*
 *		RC_SCENARIO_MISMATCH	scenario's don't match (first_time only)			*
 *		RC_DOLIST_FULL			DoList was full											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static RetcodeType Wait_For_Players(int first_time, ConnManClass *net,
	int resend_delta, int dialog_time, int timeout, char *multi_packet_buf,
	int my_sent, long *their_frame, unsigned short *their_sent,
	unsigned short *their_recv)
{
	//........................................................................
	// Variables for sending, receiving & parsing packets:
	//........................................................................
	EventClass *event;					// event ptr for parsing incoming packets
	int packetlen;							// size of meta-packet sent, & received
	int id;									// id of other player
	int messages_this_loop;				// to limit # messages processed each loop

	//........................................................................
	// Variables used only if 'first_time':
	//........................................................................
	int num_ready;							// # players signalling ready

	//........................................................................
	// Timing variables
	//........................................................................
	CountDownTimerClass retry_timer;		// time between FRAMESYNC packet resends
	CountDownTimerClass dialog_timer;	// time to pop up a dialog
	CountDownTimerClass timeout_timer;	// general-purpose timeout

	//........................................................................
	// Dialog variables
	//........................................................................
	int reconnect_dlg = 0;				// 1 = the reconnect dialog is displayed

	//........................................................................
	// Other misc variables
	//........................................................................
	KeyNumType input;						// for user input
	int x,y;									// for map input
	RetcodeType rc;

	//------------------------------------------------------------------------
	// Wait to hear from all other players
	//------------------------------------------------------------------------
	num_ready = 0;
	retry_timer.Set (resend_delta, true);	// time to retry
	dialog_timer.Set (dialog_time, true);	// time to show dlg
	timeout_timer.Set (timeout, true);		// time to bail out

	while (1) {

		Update_Queue_Mono (net, 2);

		//---------------------------------------------------------------------
		//	Resend a frame-sync packet if longer than one propogation delay goes
		// by; this prevents a "deadlock".  If he's waiting for me to advance,
		// but has missed my last few FRAMEINFO packets, I may be waiting for
		// him to advance.  Resending a FRAMESYNC ensures he knows what frame
		// number I'm on.
		//---------------------------------------------------------------------
		if (!retry_timer.Time()) {
			retry_timer.Set (resend_delta, true);		// time to retry
			Update_Queue_Mono (net, 3);
			Send_FrameSync(net, my_sent);
		}

		//---------------------------------------------------------------------
		//	Service the connections
		//---------------------------------------------------------------------
		net->Service();

		//---------------------------------------------------------------------
		//	Pop up a reconnect dialog if enough time goes by
		//---------------------------------------------------------------------
		if (!dialog_timer.Time() && SpecialDialog==SDLG_NONE) {
			if (Process_Reconnect_Dialog(&timeout_timer, their_frame,
				net->Num_Connections(), (first_time==0), (reconnect_dlg==0))) {
				return (RC_CANCEL);
			}
			reconnect_dlg = 1;
		}

		//---------------------------------------------------------------------
		//	Exit if too much time goes by (the other system has crashed or
		// bailed)
		//---------------------------------------------------------------------
		if (!timeout_timer.Time()) {
			//..................................................................
			// For the first-time run, just give up; something's wrong.
			//..................................................................
			if (first_time) {
				return (RC_NOT_RESPONDING);
			}
			//..................................................................
			// Otherwise, we're in the middle of a game; so, the modem &
			// network must deal with a timeout differently.
			//..................................................................
			else {
				Update_Queue_Mono (net, 4);

				if (Handle_Timeout(net, their_frame, their_sent, their_recv)) {
					Map.Flag_To_Redraw(true);	// erase modem reconnect dialog
					Map.Render();
					retry_timer.Set (resend_delta, true);
					dialog_timer.Set (dialog_time, true);
					timeout_timer.Set (timeout, true);
				}
				else {
					return (RC_NOT_RESPONDING);
				}
			}
		}

		//---------------------------------------------------------------------
		//	Check for an incoming message.  We must still process commands
		// even if 'first_time' is set, in case the other system got my 1st
		// FRAMESYNC, but I didn't get his; he'll be at the next frame, and
		// may be sending commands.
		// We have to limit the number of incoming messages we handle; it's
		// possible to go into an infinite loop processing modem messages.
		//---------------------------------------------------------------------
		messages_this_loop = 0;
		while ( (messages_this_loop++ < 5) &&
			net->Get_Private_Message (multi_packet_buf, &packetlen, &id) ) {
			Update_Queue_Mono (net, 5);

			/*..................................................................
			Get an event ptr to the incoming message
			..................................................................*/
			event = (EventClass *)multi_packet_buf;

			//------------------------------------------------------------------
			// Special processing for a modem game: process SERIAL packets
			//------------------------------------------------------------------
			if (GameToPlay == GAME_MODEM
				|| GameToPlay == GAME_NULL_MODEM){
				//|| GameToPlay == GAME_INTERNET) {
				rc = Process_Serial_Packet(multi_packet_buf, first_time);
				//...............................................................
				// SERIAL packet received & processed
				//...............................................................
				if (rc == RC_SERIAL_PROCESSED) {
					net->Service();
					retry_timer.Set (resend_delta, true);
					dialog_timer.Set (dialog_time, true);
					timeout_timer.Set (timeout, true);
					continue;
				}
				//...............................................................
				// other player has left the game
				//...............................................................
				else if (rc == RC_PLAYER_LEFT) {
					if (first_time) {
						num_ready++;
					}
					break;
				}
				//...............................................................
				// Connection was lost
				//...............................................................
				else if (rc == RC_HUNG_UP) {
					return (RC_NOT_RESPONDING);
				}
				//...............................................................
				// If it was any other type of serial packet, break
				//...............................................................
				else if (rc != RC_NORMAL) {
					break;
				}
			}

			//------------------------------------------------------------------
			//	Process the incoming packet
			//------------------------------------------------------------------
			rc = Process_Receive_Packet(net, multi_packet_buf, id, packetlen,
				their_frame, their_sent, their_recv);
			//..................................................................
			// New player heard from
			//..................................................................
			if (rc == RC_PLAYER_READY) {
				num_ready++;
			}
			//..................................................................
			// Scenario's don't match
			//..................................................................
			else if (rc == RC_SCENARIO_MISMATCH) {
				return (RC_SCENARIO_MISMATCH);
			}
			//..................................................................
			// DoList was full
			//..................................................................
			else if (rc == RC_DOLIST_FULL) {
				return (RC_DOLIST_FULL);
			}

			//..................................................................
			//	Service the connection, to clean out the receive queues
			//..................................................................
			net->Service();
		}

		//---------------------------------------------------------------------
		// Debug output
		//---------------------------------------------------------------------
		Print_Framesync_Values(Frame, MPlayerMaxAhead, net->Num_Connections(),
			their_recv, their_sent, my_sent);

		//---------------------------------------------------------------------
		//	Attempt to advance to the next frame.
		//---------------------------------------------------------------------
		//.....................................................................
		// For the first-time run, just check to see if we've heard from
		// everyone.
		//.....................................................................
		if (first_time) {
			if (num_ready >= net->Num_Connections()) 	break;
		}
		//.....................................................................
		// For in-game processing, we have to check their_sent, their_recv,
		// their_frame, etc.
		//.....................................................................
		else {
			if (Can_Advance(net, MPlayerMaxAhead, their_frame, their_sent,
				their_recv)) {
				break;
			}
		}

		//---------------------------------------------------------------------
		//	Service game stuff.  Servicing the map's input, and rendering the
		//	map, allows the map to scroll even though we're hung up waiting for
		//	packets.  Don't do this if 'first_time' is set, since users could be
		// waiting a very long time for all systems to load the scenario, and
		// it gets frustrating being able to scroll around without doing
		// anything.
		//---------------------------------------------------------------------
		Call_Back();
		if (!first_time && SpecialDialog == SDLG_NONE && reconnect_dlg==0) {
			WWMouse->Erase_Mouse(&HidPage, TRUE);
			Map.Input(input, x, y);
			if (input)
				Keyboard_Process(input);
			Map.Render();
		}

	}	/* end of while */

	//------------------------------------------------------------------------
	//	If the reconnect dialog was shown, force the map to redraw.
	//------------------------------------------------------------------------
	if (reconnect_dlg) {
		Map.Flag_To_Redraw(true);
		Map.Render();
	}

	return (RC_NORMAL);

}	// end of Wait_For_Players


/***************************************************************************
 * Generate_Timing_Event -- computes & queues a RESPONSE_TIME event        *
 *                                                                         *
 * This routine adjusts the connection timing on the local system; it also	*
 * optionally generates a RESPONSE_TIME event, to tell all systems to		*
 * dynamically adjust the current MaxAhead value.  This allows both the		*
 * MaxAhead & the connection retry logic to have dynamic timing, to adjust	*
 * to varying line conditions.															*
 *                                                                         *
 * INPUT:                                                                  *
 *		net			ptr to connection manager											*
 *		my_sent		# commands I've sent out so far									*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Generate_Timing_Event(ConnManClass *net, int my_sent)
{
	unsigned long resp_time;			// connection response time, in ticks
	EventClass ev;

	//------------------------------------------------------------------------
	// Measure the current connection response time.  This time will be in
	// 60ths of a second, and represents full round-trip time of a packet.
	// To convert to one-way packet time, divide by 2; to convert to game
	// frames, divide again by 4, assuming a game rate of 15 fps.
	//------------------------------------------------------------------------
	resp_time = net->Response_Time();

	//------------------------------------------------------------------------
	//	Adjust my connection retry timing; only do this if I've sent out more
	//	than 5 commands, so I know I have a measure of the response time.
	//------------------------------------------------------------------------
	if (my_sent > 5) {

		net->Set_Timing (resp_time + 10, -1, (resp_time * 4)+15);

		//.....................................................................
		// If I'm the network "master", I'm also responsible for updating the
		// MaxAhead value on all systems, so do that here too.
		//.....................................................................
		if (MPlayerLocalID == MPlayerID[0]) {
			ev.Type = EventClass::RESPONSE_TIME;
			//..................................................................
			// For multi-frame compressed events, the MaxAhead must be an even
			// multiple of the FrameSendRate.
			//..................................................................
			if (CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
				ev.Data.FrameInfo.Delay = MAX( ((((resp_time / 8) +
					(FrameSendRate - 1)) / FrameSendRate) *
					FrameSendRate), (FrameSendRate * 2) );
char flip[128];
sprintf (flip, "C&C95 - Generating timing packet - MaxAhead = %d frames\n", ev.Data.FrameInfo.Delay);
CCDebugString (flip);

			}
			//..................................................................
			// For sending packets every frame, just use the 1-way connection
			// response time.
			//..................................................................
			else {
				if (GameToPlay == GAME_MODEM
					|| GameToPlay == GAME_NULL_MODEM){
					//|| GameToPlay == GAME_INTERNET) {
					ev.Data.FrameInfo.Delay = MAX( (resp_time / 8),
						 (unsigned long)MODEM_MIN_MAX_AHEAD );
				}
				else if (GameToPlay == GAME_IPX || GameToPlay == GAME_INTERNET) {
					ev.Data.FrameInfo.Delay = MAX( (resp_time / 8),
						 (unsigned long)NETWORK_MIN_MAX_AHEAD );
				}
			}
			OutList.Add(ev);
		}
	}

}	// end of Generate_Timing_Event


/***************************************************************************
 * Generate_Real_Timing_Event -- Generates a TIMING event                  *
 *                                                                         *
 * INPUT:                                                                  *
 *		net			ptr to connection manager											*
 *		my_sent		# commands I've sent out so far									*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   07/02/1996 BRR : Created.                                             *
 *=========================================================================*/
static void Generate_Real_Timing_Event(ConnManClass *net, int my_sent)
{
	unsigned long resp_time;			// connection response time, in ticks
	EventClass ev;
	int highest_ticks;
	int i;
	int specified_frame_rate;
	int maxahead;

	//
	// If we haven't sent out at least 5 guaranteed-delivery packets, don't
	// bother trying to measure our connection response time; just return.
	//
	if (my_sent < 5) {
		return;
	}

	//
	// Find the highest processing time we have stored
	//
	highest_ticks = 0;
	for (i = 0; i < MPlayerCount; i++) {
		//
		// If we haven't heard from all systems yet, bail out.
		//
		if (TheirProcessTime[i] == -1) {
			return;
		}
		if (TheirProcessTime[i] > highest_ticks) {
			highest_ticks = TheirProcessTime[i];
		}
	}

	//
	// Compute our "desired" frame rate as the lower of:
	// - What the user has dialed into the options screen
	// - What we're really able to run at
	//
	if (highest_ticks == 0) {
		DesiredFrameRate = 60;
	} else {
		DesiredFrameRate = 60 / highest_ticks;
	}

	if (Options.GameSpeed == 0) {
		specified_frame_rate = 60;
	} else {
		specified_frame_rate = 60 / Options.GameSpeed;
	}

	DesiredFrameRate = MIN (DesiredFrameRate, specified_frame_rate);

	//
	// Measure the current connection response time.  This time will be in
	// 60ths of a second, and represents full round-trip time of a packet.
	// To convert to one-way packet time, divide by 2; to convert to game
	// frames, ....uh....
	//
	resp_time = net->Response_Time();

	//
	// Compute our new 'MaxAhead' value, based upon the response time of our
	// connection and our desired frame rate.
	// 'MaxAhead' in frames is:
	//
	// (resp_time / 2 ticks) * (1 sec/60 ticks) * (n Frames / sec)
	//
	// resp_time is divided by 2 because, as reported, it represents a round-
	// trip, and we only want to use a one-way trip.
	//
	maxahead = (resp_time * DesiredFrameRate) / (2 * 60);

	//
	// Now, we have to round 'maxahead' so it's an even multiple of our
	// send rate.  It also must be at least thrice the FrameSendRate.
	// (Isn't "thrice" a cool word?)
	//
	maxahead = ((maxahead + FrameSendRate - 1) / FrameSendRate) * FrameSendRate;
	maxahead = MAX (maxahead, (int)FrameSendRate * 3);

	ev.Type = EventClass::TIMING;
	ev.Data.Timing.DesiredFrameRate = DesiredFrameRate;
	ev.Data.Timing.MaxAhead = maxahead;

	OutList.Add(ev);

	//
	//	Adjust my connection retry timing.  These values set the retry timeout
	// to just over one round-trip time, the 'maxretries' to -1, and the
	// connection timeout to allow for about 4 retries.
	//
	net->Set_Timing (resp_time + 10, -1, (resp_time * 4)+15);

}


/***************************************************************************
 * Generate_Process_Time_Event -- Generates a PROCESS_TIME event           *
 *                                                                         *
 * INPUT:                                                                  *
 *		net			ptr to connection manager											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   07/02/1996 BRR : Created.                                             *
 *=========================================================================*/
static void Generate_Process_Time_Event(ConnManClass *net)
{
	EventClass ev;
	int avgticks;
	unsigned long resp_time;			// connection response time, in ticks

	//
	// Measure the current connection response time.  This time will be in
	// 60ths of a second, and represents full round-trip time of a packet.
	// To convert to one-way packet time, divide by 2; to convert to game
	// frames, ....uh....
	//
	resp_time = net->Response_Time();

	//
	//	Adjust my connection retry timing.  These values set the retry timeout
	// to just over one round-trip time, the 'maxretries' to -1, and the
	// connection timeout to allow for about 4 retries.
	//
	net->Set_Timing (resp_time + 10, -1, (resp_time * 4)+15);

	if (IsMono) {
		MonoClass::Enable();
		Mono_Set_Cursor(0,23);
		Mono_Printf("Processing Ticks:%03d Frames:%03d\n", ProcessTicks,ProcessFrames);
		MonoClass::Disable();
	}

	avgticks = ProcessTicks / ProcessFrames;

	ev.Type = EventClass::PROCESS_TIME;
	ev.Data.ProcessTime.AverageTicks = avgticks;
char flip[128];
sprintf (flip, "C&C95 - Sending PROCESS_TIME packet of %04x ticks\n", ev.Data.ProcessTime.AverageTicks);
CCDebugString (flip);

	OutList.Add(ev);

	ProcessTicks = 0;
	ProcessFrames = 0;
}


/***************************************************************************
 * Process_Send_Period -- timing for sending packets every 'n' frames      *
 *                                                                         *
 * This function is for a CommProtocol of COMM_PROTOCOL_MULTI_E_COMP only.	*
 * It determines if it's time to send a packet or not.							*
 *																									*
 * INPUT:                                                                  *
 *		net		ptr to connection manager												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = it's time to send a packet; 0 = don't send a packet this frame.	*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Process_Send_Period(ConnManClass *net)
{
	//------------------------------------------------------------------------
	// If the current frame # is not an even multiple of 'FrameSendRate', then
	// it's not time to send a packet; just return.
	//------------------------------------------------------------------------
	if (Frame != (((Frame + (FrameSendRate - 1)) /
		FrameSendRate) * FrameSendRate) ) {

		net->Service();

		if (IsMono) {
			MonoClass::Disable();
		}

		return (0);
	}

	return (1);

}	// end of Process_Send_Period


/***************************************************************************
 * Send_Packets -- sends out events from the OutList                       *
 *                                                                         *
 * This routine computes how many events can be sent this frame, and then	*
 * builds the "meta-packet" & sends it.												*
 *                                                                         *
 * The 'cap' value is the max # of events we can send.  Ideally, it should	*
 * be based upon the bandwidth of our connection.  Currently, it's just		*
 * hardcoded to prevent the modem from having to resend "too much" data,	*
 * which is about 200 bytes per frame.													*
 *                                                                         *
 * INPUT:                                                                  *
 *		net						ptr to connection manager								*
 *		multi_packet_buf		buffer to store packets in								*
 *		multi_packet_max		max size of multi_packet_buf							*
 *		max_ahead				current game MaxAhead value							*
 *		my_sent					# commands I've sent this game						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		# events sent, NOT including the FRAMEINFO event							*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Send_Packets(ConnManClass *net, char *multi_packet_buf,
	int multi_packet_max, int max_ahead, int my_sent)
{
	int cap;				// max # events to send, NOT including FRAMEINFO event
	int do_once;		// true: only go through packet loop once
	int ack_req;		// 0 = no ack required on outgoing packet
	int packetlen;		// size of meta-packet sent

	//------------------------------------------------------------------------
	//	Determine how many events it's OK to send this frame.
	//------------------------------------------------------------------------
	//........................................................................
	// If we have 4 or more packets queue'd for sending, don't add any more
	// this frame.
	//........................................................................
	if (net->Private_Num_Send() >= 4) {
		cap = 0;
		do_once = 1;
	}
	//........................................................................
	// If there are 2 or more packets queued, the entire packet we send must
	// fit within a single ComQueue buffer, so limit # events to 5.
	// (The Modem connection manager has a max buffer size of 200 bytes, which
	// is large enough for 6 uncompressed events, which leaves room for 5
	// events plus a FRAMEINFO.)
	//........................................................................
	else if (net->Private_Num_Send() >= 2) {
		cap = 5;
		do_once = 1;

	}
	//........................................................................
	// Otherwise, just send all events in the OutList
	//........................................................................
	else {
		cap = OutList.Count;
		do_once = 0;
	}
	//........................................................................
	// Make sure we aren't sending more events than are in the OutList
	//........................................................................
	if (cap > OutList.Count) {
		cap = OutList.Count;
	}

	//........................................................................
	// Make sure we don't send so many events that our DoList fills up
	//........................................................................
	if (cap > (MAX_EVENTS * 8) - DoList.Count) {
		cap = (MAX_EVENTS * 8) - DoList.Count;
	}

	/*
	** No cap for internet game
	**
	** Or for serial games for that matter ST - 5/31/96 4:00PM
	*/
	if (GameToPlay == GAME_INTERNET || GameToPlay == GAME_MODEM || GameToPlay == GAME_NULL_MODEM){
		cap = OutList.Count;
		do_once = 0;
	}

	//------------------------------------------------------------------------
	//	Build our meta-packet & transmit it.
	//------------------------------------------------------------------------
	while (1) {

		Update_Queue_Mono (net, 1);

		//.....................................................................
		//	If there are no commands this frame, we'll just be sending a FRAMEINFO
		//	packet; no ack is required.  For the modem's sake, check
		// MPlayerCount; no ACK is needed if we're just sending to someone
		// who's left the game.
		//.....................................................................
		if (cap == 0 || OutList.Count == 0 || MPlayerCount == 1) {
			ack_req = 0;
		}
		else {
			ack_req = 1;
		}

		//.....................................................................
		//	Build & send out our message
		//.....................................................................
		packetlen = Build_Send_Packet (multi_packet_buf, multi_packet_max,
			max_ahead, my_sent, cap);
		net->Send_Private_Message (multi_packet_buf, packetlen, ack_req);

		//.....................................................................
		//	Call Service() to actually send the packet
		//.....................................................................
		net->Service();

		//.....................................................................
		//	Stop if there's no more data to send, or if our send queue is
		// filling up.
		//.....................................................................
		if (OutList.Count == 0 || do_once) {
			break;
		}
	}

	return (cap);

}	// end of Send_Packets


/***************************************************************************
 * Send_FrameSync -- Sends a FRAMESYNC packet                              *
 *                                                                         *
 * This routine is used to periodically remind the other systems that 		*
 * we're still here, and to tell them what frame # we're on, in case			*
 * they've missed my FRAMEINFO packets.												*
 *                                                                         *
 * INPUT:                                                                  *
 *		net				ptr to connection manager										*
 *		cmd_count		# commands I've sent so far									*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Send_FrameSync(ConnManClass *net, int cmd_count)
{
	EventClass packet;

	//------------------------------------------------------------------------
	//	Build a frame-sync event to send.  FRAMESYNC packets contain a
	// scenario-based CRC rather than a game-state-based CRC, to let the
	// games compare scenario CRC's on startup.
	//------------------------------------------------------------------------
	memset (&packet, 0, sizeof(EventClass));
	packet.Type = EventClass::FRAMESYNC;
	if (CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		packet.Frame = ((Frame + MPlayerMaxAhead + (FrameSendRate - 1)) /
			 FrameSendRate) * FrameSendRate;
	}
	else {
		packet.Frame = Frame + MPlayerMaxAhead;
	}
	packet.ID = Houses.ID(PlayerPtr);
	packet.MPlayerID = MPlayerLocalID;
	packet.Data.FrameInfo.CRC = ScenarioCRC;
	packet.Data.FrameInfo.CommandCount = cmd_count;
	packet.Data.FrameInfo.Delay = MPlayerMaxAhead;

	//------------------------------------------------------------------------
	// Send the event.  For modem, this just sends to the other player;
	// for network, it sends to everyone we're connected to.
	//------------------------------------------------------------------------
	net->Send_Private_Message (&packet, (offsetof(EventClass, Data) +
		size_of(EventClass, Data.FrameInfo)), 0 );

	return;

}	// end of Send_FrameSync


/***************************************************************************
 * Process_Receive_Packet -- processes an incoming packet                  *
 *                                                                         *
 * This routine receives a packet from another system, adds it to our		*
 * execution queue (the DoList), and updates my arrays of their frame #,	*
 * their commands-sent, and their commands-received.								*
 *                                                                         *
 * INPUT:                                                                  *
 *		net					ptr to connection manager									*
 *		multi_packet_buf	buffer containing packet(s) to parse					*
 *		id						id of sender													*
 *		their_frame			array containing frame #'s of other players			*
 *		their_sent			array containing command count of other players		*
 *		their_recv			array containing # recv'd cmds from other players	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		RC_NORMAL:					nothing unusual happened, although 				*
 * 									their_sent or their_recv may have been 		*
 *										altered													*
 *		RC_PLAYER_READY:			player has been heard from for the 1st time; *
 * 									this presumes that his original 					*
 * 									'their_frame[]' value was -1 when this 		*
 * 									routine was called									*
 *		RC_SCENARIO_MISMATCH:	FRAMEINFO scenario CRC doesn't match; 			*
 * 									normally only applies after loading a new 	*
 * 									scenario or save-game								*
 *		RC_DOLIST_FULL:			fatal error; unable to add events to DoList	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static RetcodeType Process_Receive_Packet(ConnManClass *net,
	char *multi_packet_buf, int id, int packetlen, long *their_frame,
	unsigned short *their_sent, unsigned short *their_recv)
{
	EventClass *event;
	int index;
	RetcodeType retcode = RC_NORMAL;
	int i;

	//------------------------------------------------------------------------
	//	Get an event ptr to the incoming message
	//------------------------------------------------------------------------
	event = (EventClass *)multi_packet_buf;

	//------------------------------------------------------------------------
	//	Get the index of the sender
	//------------------------------------------------------------------------
	index = net->Connection_Index(id);

	//------------------------------------------------------------------------
	//	Compute the other player's frame # (at the time this packet was sent)
	//------------------------------------------------------------------------
	if (their_frame[index] <
		(int)(event->Frame - event->Data.FrameInfo.Delay)) {

		//.....................................................................
		// If the original frame # for this player is -1, it means we've heard
		// from this player for the 1st time; return the appropriate value.
		//.....................................................................
		if (their_frame[index]==-1) {
			retcode = RC_PLAYER_READY;
		}

		their_frame[index] = event->Frame - event->Data.FrameInfo.Delay;
	}

	//------------------------------------------------------------------------
	//	Extract the other player's CommandCount.  This count will include
	//	the commands in this packet, if there are any.
	//------------------------------------------------------------------------
	if (event->Data.FrameInfo.CommandCount > their_sent[index]) {
		their_sent[index] = event->Data.FrameInfo.CommandCount;
	}

	//------------------------------------------------------------------------
	//	If this packet was not a FRAMESYNC packet:
	//	- Add the events in it to our DoList
	//	- Increment our commands-received counter by the number of non-
	//	  FRAMEINFO packets received
	//------------------------------------------------------------------------
	if (event->Type != EventClass::FRAMESYNC) {
		//.....................................................................
		// Break up the packet into its component events.  A returned packet
		// count of -1 indicates a fatal queue-full error.
		//.....................................................................
		i = Breakup_Receive_Packet( multi_packet_buf, packetlen);
		if (i==-1) {
			return (RC_DOLIST_FULL);
		}
		//.....................................................................
		// Compute the actual # commands in the packet by subtracting off the
		// FRAMEINFO event
		//.....................................................................
		if ( (event->Type==EventClass::FRAMEINFO) && (i > 0)) {
			i--;
		}

		their_recv[index] += i;
	}

	//------------------------------------------------------------------------
	//	If the event was a FRAMESYNC packet, there will be no commands to add,
	//	but we must check the ScenarioCRC value.
	//------------------------------------------------------------------------
	else if (event->Data.FrameInfo.CRC != ScenarioCRC) {
		return (RC_SCENARIO_MISMATCH);
	}

	return (retcode);

}	// end of Process_Receive_Packet


/***************************************************************************
 * Process_Serial_Packet -- Handles an incoming serial packet              *
 *                                                                         *
 * This routine is needed because the modem classes don't support a 			*
 * "global channel" like the network classes do, but that functionality is	*
 * still needed for modem communications.  Specifically, the modem dialogs	*
 * transmit their own special packets back & forth, and messages are sent	*
 * using a special packet type.  Thus, we have to call this routine when	*
 * we receive a modem packet, to allow it to process messages & dialog		*
 * packets.																						*
 *                                                                         *
 * INPUT:                                                                  *
 *		multi_packet_buf		packet buffer to process								*
 *		first_time				1 = this is the 1st game frame						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		RC_NORMAL:				this wasn't a SERIAL-type packet						*
 *		RC_SERIAL_PROCESSED:	this was a SERIAL-type packet, and was 			*
 * 								processed; the other player is still connected,	*
 * 								even if he's not in the game.							*
 *		RC_PLAYER_LEFT:		other player has left the game						*
 *		RC_HUNG_UP:				we're getting our own packets back; thus, the 	*
 * 								modem is mirroring our packets, which means the *
 * 								modem hung up!												*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static RetcodeType Process_Serial_Packet(char *multi_packet_buf,
	int first_time)
{
	multi_packet_buf;
	first_time;
	return (RC_NORMAL);
#if (0)	// ST - 1/2/2019 5:28PM
	SerialPacketType 	*serial_packet;		// for parsing serial packets
	int 					player_gone;
	EventClass 			*event;
	char 					txt[MAX_MESSAGE_LENGTH+80];
	unsigned short		magic_number;
	unsigned short		crc;

	//------------------------------------------------------------------------
	//	Determine if this packet means that the other player has left the game
	//------------------------------------------------------------------------
	serial_packet = (SerialPacketType *)multi_packet_buf;
	player_gone = 0;
	//........................................................................
	// On Frame 0, only a SIGN_OFF means the other player left; the other
	// packet types may be left over from a previous session.
	//........................................................................
	if (first_time) {
		if (serial_packet->Command == SERIAL_SIGN_OFF) {
			player_gone = 1;
		}
	}
	//........................................................................
	// On subsequent frames, any of SIGN_OFF, TIMING, or SCORE_SCREEN means
	// the other player is gone.
	//........................................................................
	else {
		if (serial_packet->Command == SERIAL_SIGN_OFF ||
			serial_packet->Command == SERIAL_TIMING ||
			serial_packet->Command == SERIAL_SCORE_SCREEN ) {
			player_gone = 1;
		}
	}
	if (player_gone) {
		Destroy_Null_Connection(serial_packet->Color, 0);
		return (RC_PLAYER_LEFT);
	}

	//------------------------------------------------------------------------
	// Process an incoming message
	//------------------------------------------------------------------------
	if (serial_packet->Command == SERIAL_MESSAGE) {
		sprintf(txt, Text_String(TXT_FROM), serial_packet->Name,
			serial_packet->Message);

		magic_number = *((unsigned short*)(serial_packet->Message + COMPAT_MESSAGE_LENGTH-4));
		crc = *((unsigned short*)(serial_packet->Message + COMPAT_MESSAGE_LENGTH-2));

		Messages.Add_Message (txt,
			MPlayerTColors[MPlayerID_To_ColorIndex(serial_packet->ID)],
			TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 600, magic_number, crc);

		//.....................................................................
		//	Save this message in our last-message buffer
		//.....................................................................
		if (strlen (serial_packet->Message)) {
			strcpy (LastMessage, serial_packet->Message);
		}

		//.....................................................................
		//	Tell the map to do a partial update (just to force the
		// messages to redraw).
		//.....................................................................
		Map.Flag_To_Redraw(false);
		return (RC_SERIAL_PROCESSED);
	}

	//------------------------------------------------------------------------
	// Any other SERIAL-type packet means the other player is still there;
	// throw them away, but let the caller know the connection is OK.
	//------------------------------------------------------------------------
	if ( (serial_packet->Command >= SERIAL_CONNECT &&
		serial_packet->Command < SERIAL_LAST_COMMAND) ||
		MPlayerCount == 1) {
		return (RC_SERIAL_PROCESSED);
	}

	//........................................................................
	//	are we getting our own packets back??
	//........................................................................
	event = (EventClass *)multi_packet_buf;
	if (event->ID == Houses.ID(PlayerPtr)) {
		return (RC_HUNG_UP);
	}

	return (RC_NORMAL);
#endif
}	// end of Process_Serial_Packet


/***************************************************************************
 * Can_Advance -- determines if it's OK to advance to the next frame       *
 *                                                                         *
 * This routine uses the current values stored in their_frame[], 				*
 * their_send[], and their_recv[] to see if it's OK to advance to the next	*
 * game frame.  We must not advance if:												*
 * - If our frame # would be too far ahead of the slowest player (the 		*
 *   lowest their_frame[] value).  "Too far" means 								*
 *   (Frame > their_frame + MaxAhead).													*
 * - our current command count doesn't match the sent command count of one	*
 *   other player (meaning that we've missed a command packet from that 	*
 *   player, and thus the frame # we're receiving from him may be due to a	*
 *   FRAMEINFO packet sent later than the command, so we shouldn't use 		*
 *   this frame # to see if we should advance; we should wait until we 		*
 *   have all the commands before we advance.										*
 *                                                                         *
 * Of course, this routine assumes the values in their_frame[] etc are		*
 * kept current by the caller.															*
 *                                                                         *
 * INPUT:                                                                  *
 *		net				ptr to connection manager										*
 *		max_ahead		max frames ahead													*
 *		their_frame		array of their frame #'s										*
 *		their_sent		array of their sent command count							*
 *		their_recv		array of their # received commands							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK to advance; 0 = not OK														*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Can_Advance(ConnManClass *net, int max_ahead, long *their_frame,
	unsigned short *their_sent, unsigned short *their_recv)
{
	long their_oldest_frame;			// other players' oldest frame #
	int count_ok;							// true = my cmd count matches theirs
	int i;

	//------------------------------------------------------------------------
	// Special case for modem: if the other player has left, go ahead and
	// advance to the next frame; don't wait on him.
	//------------------------------------------------------------------------
	if (MPlayerCount == 1) {
		return (1);
	}
	//------------------------------------------------------------------------
	//	Find the oldest frame # in 'their_frame'
	//------------------------------------------------------------------------
	their_oldest_frame = Frame + 1000;
	for (i = 0; i < net->Num_Connections(); i++) {
		if (their_frame[i] < their_oldest_frame)
			their_oldest_frame = their_frame[i];
	}

	//------------------------------------------------------------------------
	//	I can advance to the next frame IF:
	//	1) I'm less than a one-way propogation delay ahead of the other
	//    players' frame numbers, AND
	//	2) their_recv[i] >= their_sent[i] (ie I've received all the commands
	//	   the other players have sent so far).
	//------------------------------------------------------------------------
	count_ok = 1;
	for (i = 0; i < net->Num_Connections(); i++) {
		if (their_recv[i] < their_sent[i]) {
			count_ok = 0;
			break;
		}
	}
	if (count_ok && (Frame < (their_oldest_frame + max_ahead))) {
		return (1);
	}

	return (0);

}	// end of Can_Advance


/***************************************************************************
 * Process_Reconnect_Dialog -- processes the reconnection dialog           *
 *                                                                         *
 * This routine [re]draws the reconnection dialog; if 'reconn' is set,		*
 * it tells the user who we're trying to reconnect to; otherwise, is just	*
 * says something generic like "Waiting for connections".						*
 *                                                                         *
 * INPUT:                                                                  *
 *		timeout_timer	ptr to count down timer, showing time remaining			*
 *		their_frame		array of other players' frame #'s							*
 *		num_conn			# connections in 'their_frame'								*
 *		reconn			1 = reconnect, 0 = waiting for first-time connection	*
 *		fresh				1 = draw from scratch, 0 = only update time counter	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = user wants to cancel, 0 = not												*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Process_Reconnect_Dialog(CountDownTimerClass *timeout_timer,
	long *their_frame, int num_conn, int reconn, int fresh)
{
	static int displayed_time = 0;	// time value currently displayed
	int new_time;
	int oldest_index;						// index of person requiring a reconnect
	int i,j;

	//------------------------------------------------------------------------
	// Convert the timer to seconds
	//------------------------------------------------------------------------
	new_time = timeout_timer->Time() / 60;

	//--------------------------------------------------------------------------------
	// If we have just received input focus again after running in the background then
	// we need to redraw the whole dialog.
	//--------------------------------------------------------------------------------
	if (AllSurfaces.SurfacesRestored){
		AllSurfaces.SurfacesRestored=FALSE;
		fresh = true;
	}

	//------------------------------------------------------------------------
	// If the timer has changed, or 'fresh' is set, redraw the dialog
	//------------------------------------------------------------------------
	if (fresh || (new_time != displayed_time)) {
		//.....................................................................
		// Find the index of the person we're trying to reconnect to
		//.....................................................................
		if (reconn) {
			j = 0x7fffffff;
			oldest_index = 0;
			for (i = 0; i < num_conn; i++) {
				if (their_frame[i] < j) {
					j = their_frame[i];
					oldest_index = i;
				}
			}
		}
		Net_Reconnect_Dialog(reconn, fresh, oldest_index, new_time);
	}
	displayed_time = new_time;

	//........................................................................
	//	If user hits ESC, bail out
	//........................................................................
	if (Check_Key()) {
		if (Get_Key_Num()==KN_ESC) {
			return (1);
		}
	}

	return (0);

}	// end of Process_Reconnect_Dialog


/***************************************************************************
 * Handle_Timeout -- handles a timeout in the wait-for-players loop			*
 *                                                                         *
 * This routine "gracefully" handles a timeout in the frame-sync loop.		*
 * The timeout must be handled differently by a modem game or network 		*
 * game.																							*
 *                                                                         *
 * The modem game must detect if the other player is still connected			*
 * physically, even if he's not playing the game any more; if so, this		*
 * routine returns an OK status.  If the other player isn't even 				*
 * physically connected, an error is returned.										*
 *                                                                         *
 * The network game must find the connection that's causing the timeout,	*
 * and destroy it.  The game continues, even if there are no more human		*
 * players left.																				*
 *                                                                         *
 * INPUT:                                                                  *
 *		net					ptr to connection manager									*
 *		their_frame			array containing frame #'s of other players			*
 *		their_sent			array containing command count of other players		*
 *		their_recv			array containing # recv'd cmds from other players	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = it's OK; reset timeout timers & keep processing						*
 *		0 = game over, man																	*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Handle_Timeout(ConnManClass *net, long *their_frame,
	unsigned short *their_sent, unsigned short *their_recv)
{
	int oldest_index;						// index of person requiring a reconnect
	int i,j;
	int id;

	//------------------------------------------------------------------------
	// For modem, attempt to reconnect; if that fails, save the game & bail.
	//------------------------------------------------------------------------
	if (GameToPlay == GAME_MODEM || GameToPlay == GAME_NULL_MODEM) {
#if (0)			//ST - 1/2/2019 5:28PM
		if ( net->Num_Connections() ) {
			if (!Reconnect_Modem()) {
				return (0);
			}
			else {
				return (1);
			}
		}
#endif
	}

	//------------------------------------------------------------------------
	//	For network, destroy the oldest connection
	//------------------------------------------------------------------------
	else if (GameToPlay == GAME_IPX || GameToPlay == GAME_INTERNET) {
		j = 0x7fffffff;
		oldest_index = 0;
		for (i = 0; i < net->Num_Connections(); i++) {
			if (their_frame[i] < j) {
				j = their_frame[i];
				oldest_index = i;
			}
		}

		id = net->Connection_ID(oldest_index);

		/*
		** Send the game statistics packet now if the game is effectivly over
		*/
		if (MPlayerCount == 2 &&
				GameToPlay == GAME_INTERNET &&
				!GameStatisticsPacketSent){
			Register_Game_End_Time();
			ConnectionLost = true;
			Send_Statistics_Packet();
		}

		if (id != ConnManClass::CONNECTION_NONE) {
			for (i = oldest_index; i < net->Num_Connections() - 1; i++) {
				their_frame[i] = their_frame[i+1];
				their_sent[i] = their_sent[i+1];
				their_recv[i] = their_recv[i+1];
			}
			CCDebugString ("C&C95 = Destroying connection due to time out\n");
			Destroy_Connection(id,1);
		}
	}

	return (1);

}	// end of Handle_Timeout


/***************************************************************************
 * Stop_Game -- stops the game															*
 *                                                                         *
 * This routine clears any global flags that need it, in preparation for	*
 * halting the game prematurely.															*
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
 *   11/22/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Stop_Game(void)
{
	CCDebugString ("C&C95 - In Stop_Game.\n");
	GameActive = 0;
	if (IsMono) {
		MonoClass::Disable();
	}

	if (GameToPlay == GAME_INTERNET){
		ConnectionLost = true;
		CCDebugString ("C&C95 - About to send statistics packet.\n");
		Register_Game_End_Time();
		Send_Statistics_Packet();
		CCDebugString ("C&C95 - Returned from sending stats packet.\n");
	}

	return;

}	// end of Stop_Game


/***************************************************************************
 * Build_Send_Packet -- Builds a big packet from a bunch of little ones.	*
 *                                                                         *
 * This routine takes events from the OutList, and puts them into a 			*
 * "meta-packet", which is transmitted to all systems we're connected to.	*
 * Also, these events are added to our own DoList.									*
 *                                                                         *
 * Every Meta-Packet we send uses a FRAMEINFO packet as a header; this 		*
 * tells the other systems what frame we're on, as well as serving as a 	*
 * standard packet header.																	*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf				buffer to store packet in										*
 *		bufsize			max size of buffer												*
 *		frame_delay		desired frame delay to attach to all outgoing packets	*
 *		num_cmds			value to use for the CommandCount field					*
 *		cap				max # events to send												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		new size of packet																	*
 *                                                                         *
 * WARNINGS:                                                               *
 * 'num_cmds' should be the total of of commands, including all those sent *
 * this frame!																					*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Build_Send_Packet(void *buf, int bufsize, int frame_delay,
	int num_cmds, int cap)
{
	int size = 0;
	EventClass *finfo;

	//------------------------------------------------------------------------
	// All events start with a FRAMEINFO event; fill this part in.
	//------------------------------------------------------------------------
	//........................................................................
	// Set the event type
	//........................................................................
	finfo = (EventClass *)buf;
	finfo->Type = EventClass::FRAMEINFO;
	//........................................................................
	// Set the frame to execute this event on; this is protocol-specific
	//........................................................................
	if (CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		finfo->Frame = ((Frame + frame_delay + (FrameSendRate - 1)) /
			 FrameSendRate) * FrameSendRate;
	}
	else {
		finfo->Frame = Frame + frame_delay;
	}
	//........................................................................
	// Fill in the rest of the event
	//........................................................................
	finfo->ID = Houses.ID(PlayerPtr);
	finfo->MPlayerID = MPlayerLocalID;
	finfo->Data.FrameInfo.CRC = GameCRC;
	finfo->Data.FrameInfo.CommandCount = num_cmds;
	finfo->Data.FrameInfo.Delay = frame_delay;

	//------------------------------------------------------------------------
	// Initialize the # of bytes processed; this is protocol-specific
	//------------------------------------------------------------------------
	if (CommProtocol==COMM_PROTOCOL_SINGLE_NO_COMP) {
		size += sizeof(EventClass);
	}
	else {
		size += (offsetof(EventClass, Data) +
			size_of(EventClass, Data.FrameInfo));
	}

	//------------------------------------------------------------------------
	//	Transfer all events from the OutList into the DoList, building our
	//	packet while we go.
	//------------------------------------------------------------------------
	switch (CommProtocol) {
		//.....................................................................
		// COMM_PROTOCOL_SINGLE_NO_COMP:
		// We'll send at least a FRAMEINFO every single frame, no compression
		//.....................................................................
		case (COMM_PROTOCOL_SINGLE_NO_COMP):
			size = Add_Uncompressed_Events(buf, bufsize, frame_delay, size, cap);
			break;

		//.....................................................................
		// COMM_PROTOCOL_SINGLE_E_COMP:
		//   Compress a group of packets into our send buffer; send out
		//   compressed packets every frame.
		// COMM_PROTOCOL_MULTI_E_COMP:
		//   Compress a group of packets into our send buffer; send out
		//   compressed packets every 'n' frames.
		//.....................................................................
		case (COMM_PROTOCOL_SINGLE_E_COMP):
		case (COMM_PROTOCOL_MULTI_E_COMP):
			size = Add_Compressed_Events(buf, bufsize, frame_delay, size, cap);
			break;

		//.....................................................................
		// Default: We have no idea what to do, so do nothing.
		//.....................................................................
		default:
			size = 0;
			break;
	}

	return( size );

}	/* end of Build_Send_Packet */


/***************************************************************************
 * Add_Uncompressed_Events -- adds uncompressed events to a packet         *
 *                                                                         *
 * INPUT:                                                                  *
 *		buf				buffer to store packet in										*
 *		bufsize			max size of buffer												*
 *		frame_delay		desired frame delay to attach to all outgoing packets	*
 *		size				current packet size												*
 *		cap				max # events to process											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		new size value																			*
 *                                                                         *
 * WARNINGS:                                                               *
 *		This routine MUST check to be sure it doesn't overflow the buffer.	*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 DRD : Created.                                             *
 *=========================================================================*/
static int Add_Uncompressed_Events(void *buf, int bufsize, int frame_delay,
	int size, int cap)
{
	int num = 0;			// # of events processed
	int ev_size;			// size of event we're adding

	//------------------------------------------------------------------------
	// Loop until there are no more events, or we've processed our max # of
	// events, or the buffer is full.
	//------------------------------------------------------------------------
	while (OutList.Count && (num < cap)) {
		ev_size = sizeof(EventClass);
		//.....................................................................
		// Will the next event exceed the size of the buffer?  If so, break.
		//.....................................................................
		if ( (size + ev_size) > bufsize ) {
			return (size);
		}

		//.....................................................................
		// Set the event's frame delay
		//.....................................................................
		OutList.First().Frame = Frame + frame_delay;

		//.....................................................................
		// Set the event's ID
		//.....................................................................
		OutList.First().ID = Houses.ID(PlayerPtr);
		OutList.First().MPlayerID = MPlayerLocalID;

		//.....................................................................
		// Transfer the event in OutList to DoList, un-queue the OutList
		// event.  If the DoList is full, stop transferring immediately.
		//.....................................................................
		OutList.First().IsExecuted = 0;
		if (!DoList.Add(OutList.First())) {
			return (size);
		}

		//.....................................................................
		// Add event to the send packet
		//.....................................................................
		memcpy ( ((char *)buf) + size, &OutList.First(), sizeof(EventClass) );
		size += sizeof(EventClass);

		//.....................................................................
		// Increment our event counter; delete the last event from the queue
		//.....................................................................
		num++;
		OutList.Next();
	}

	return (size);

}	// end of Add_Uncompressed_Events


/***************************************************************************
 * Add_Compressed_Events -- adds an compressed events to a packet          *
 *                                                                         *
 * INPUT:                                                                  *
 *		buf				buffer to store packet in										*
 *		bufsize			max size of buffer												*
 *		frame_delay		desired frame delay to attach to all outgoing packets	*
 *		size				reference to current packet size								*
 *		cap				max # events to process											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		new size value																			*
 *                                                                         *
 * WARNINGS:                                                               *
 *		This routine MUST check to be sure it doesn't overflow the buffer.	*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 DRD : Created.                                             *
 *=========================================================================*/
static int Add_Compressed_Events(void *buf, int bufsize, int frame_delay,
	int size, int cap)
{
	int num = 0;							// # of events processed
	EventClass::EventType eventtype;	// type of event being compressed
	EventClass prevevent;				// last event processed
	int datasize;							// size of element plucked from event union
	int storedsize;						// actual # bytes stored from event
	unsigned char *unitsptr = NULL;	// ptr to buffer pos to store mega. rep count
	unsigned char numunits = 0;		// megamission rep count value
	bool missiondup = false;			// flag: is this event a megamission repeat?

	//------------------------------------------------------------------------
	// clear previous event
	//------------------------------------------------------------------------
	memset (&prevevent, 0, sizeof(EventClass));

	//------------------------------------------------------------------------
	// Loop until there are no more events, we've processed our max # of
	// events, or the buffer is full.
	//------------------------------------------------------------------------
	while (OutList.Count && (num < cap)) {
		eventtype = OutList.First().Type;
		datasize = EventClass::EventLength[ eventtype ];
		//.....................................................................
		// For a variable-sized event, pull the size from the event; otherwise,
		// the size will be the data element size plus the event type value.
		//.....................................................................
		storedsize = datasize + sizeof (EventClass::EventType);

		//.....................................................................
		// MegaMission compression:  MegaMissions are stored as:
		//   EventType
		//   Rep Count
		//   MegaMission structure (event # 1 only)
		//   Whom #2
		//   Whom #3
		//   Whom #4
		//   ...
		//   Whom #n
		//.....................................................................
		if (prevevent.Type == EventClass::MEGAMISSION) {
			//..................................................................
			// If previous & current events are both MegaMissions:
			//..................................................................
			if (eventtype == EventClass::MEGAMISSION) {
				//...............................................................
				// If the Mission, Target, & Destination are the same, compress
				// the events into one:
				// - Change datasize to the size of the 'Whom' field only
				// - set total # bytes to store to the size of the 'Whom' only
				// - increment the MegaMission rep count
				// - set the MegaMission rep flag
				//...............................................................
				if (OutList.First().Data.MegaMission.Mission ==
					prevevent.Data.MegaMission.Mission &&
					OutList.First().Data.MegaMission.Target ==
						prevevent.Data.MegaMission.Target &&
					OutList.First().Data.MegaMission.Destination ==
						prevevent.Data.MegaMission.Destination) {

					datasize = sizeof(prevevent.Data.MegaMission.Whom);
					storedsize = datasize;
					numunits++;
					missiondup = true;
				}
				//...............................................................
				// Data doesn't match; start a new run of MegaMissions:
				// - Store previous MegaMission rep count
				// - Init 'unitsptr' to buffer pos after next EventType
				// - set total # bytes to store to 'datasize' + sizeof(EventType) +
				//   sizeof (numunits)
				// - init the MegaMission rep count to 1
				// - clear the MegaMission rep flag
				//...............................................................
				else {
					*unitsptr = numunits;
					unitsptr = ((unsigned char *)buf) + size +
						sizeof(EventClass::EventType);
					storedsize += sizeof(numunits);
					numunits = 1;
					missiondup = false;
				}
			}
			//..................................................................
			// Previous event was a MegaMission, but this one isn't: end the
			// run of MegaMissions:
			// - Store previous MegaMission rep count
			// - Clear variables
			//..................................................................
			else {
				*unitsptr = numunits;		// save # events in our run
				unitsptr = NULL;				// init other values
				numunits = 0;
				missiondup = false;
			}
		}

		//.....................................................................
		// The previous event is not a MEGAMISSION but the current event is:
		// Set up a new run of MegaMissions:
		// - Init 'unitsptr' to buffer pos after next EventType
		// - set total # bytes to store to 'datasize' + sizeof(EventType) +
		//   sizeof (numunits)
		// - init the MegaMission rep count to 1
		// - clear the MegaMission rep flag
		//.....................................................................
		else if (eventtype == EventClass::MEGAMISSION) {
			unitsptr = ((unsigned char *)buf) + size +
				sizeof(EventClass::EventType);
			storedsize += sizeof(numunits);
			numunits = 1;
			missiondup = false;
		}

		//.....................................................................
		// Will the next event exceed the size of the buffer?  If so,
		// stop compressing.
		//.....................................................................
		if ( (size + storedsize) > bufsize )
			break;

		//.....................................................................
		// Set the event's frame delay (this is protocol-dependent)
		//.....................................................................
		if (CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
			OutList.First().Frame = ((Frame + frame_delay +
				(FrameSendRate - 1)) / FrameSendRate) *
				FrameSendRate;
		}
		else {
			OutList.First().Frame = Frame + frame_delay;
		}

		//.....................................................................
		// Set the event's ID
		//.....................................................................
		OutList.First().ID = Houses.ID(PlayerPtr);
		OutList.First().MPlayerID = MPlayerLocalID;

		//.....................................................................
		// Transfer the event in OutList to DoList, un-queue the OutList event.
		// If the DoList is full, stop transferring immediately.
		//.....................................................................
		OutList.First().IsExecuted = 0;
		if ( !DoList.Add( OutList.First() ) ) {
			break;
		}

		//---------------------------------------------------------------------
		// Compress the event into the send packet buffer
		//---------------------------------------------------------------------
		switch ( eventtype ) {
			//..................................................................
			// RESPONSE_TIME: just use the Delay field of the FrameInfo union
			//..................................................................
			case (EventClass::RESPONSE_TIME):

				*(EventClass::EventType *)( ((char *)buf) + size) = eventtype;

				memcpy ( ((char *)buf) + size + sizeof(EventClass::EventType),
					&OutList.First().Data.FrameInfo.Delay, datasize );

				size += (datasize + sizeof(EventClass::EventType));
				break;

			//..................................................................
			// MEGAMISSION:
			//..................................................................
			case (EventClass::MEGAMISSION):
				//...............................................................
				// Repeated mission in a run:
				//   - Update the rep count (in case we break out)
				//   - Copy the Whom field only
				//...............................................................
				if (missiondup) {
					*unitsptr = numunits;

					memcpy ( ((char *)buf) + size,
						&OutList.First().Data.MegaMission.Whom, datasize );

					size += datasize;
				}
				//...............................................................
				// 1st mission in a run:
				//   - Init the rep count (in case we break out)
				//   - Set the EventType
				//   - Copy the MegaMission structure, leaving room for 'numunits'
				//...............................................................
				else {
					*unitsptr = numunits;

					*(EventClass::EventType *)( ((char *)buf) + size) = eventtype;

					memcpy ( ((char *)buf) + size +
						sizeof(EventClass::EventType) + sizeof(numunits),
						&OutList.First().Data.MegaMission, datasize );

					size += (datasize + sizeof(EventClass::EventType) + sizeof(numunits));
				}
				break;

			//..................................................................
			// Default case: Just copy over the data field from the union
			//..................................................................
			default:
				*(EventClass::EventType *)( ((char *)buf) + size) = eventtype;

				memcpy ( ((char *)buf) + size + sizeof(EventClass::EventType),
					&OutList.First().Data, datasize );

				size += (datasize + sizeof(EventClass::EventType));

				break;
		}

		//---------------------------------------------------------------------
		// update # events processed
		//---------------------------------------------------------------------
		num++;

		//---------------------------------------------------------------------
		// Update 'prevevent'
		//---------------------------------------------------------------------
		memcpy ( &prevevent, &OutList.First(), sizeof(EventClass) );

		//---------------------------------------------------------------------
		// Go to the next event to process
		//---------------------------------------------------------------------
		OutList.Next();
	}

	return (size);

}	// end of Add_Compressed_Events


/***************************************************************************
 * Breakup_Receive_Packet -- Splits a big packet into little ones.			*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer to break up													*
 *		bufsize		length of buffer														*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		# events added to queue, -1 if fatal error (queue is full)				*
 *    (return value includes any FRAMEINFO packets encountered; 				*
 *		FRAMESYNC's are ignored)		  													*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Breakup_Receive_Packet(void *buf, int bufsize )
{
	int count = 0;

	/*
	** is there enough leftover for another record
	*/
	switch (CommProtocol) {
		case (COMM_PROTOCOL_SINGLE_NO_COMP):
			count = Extract_Uncompressed_Events(buf, bufsize);
			break;

		default:
			count = Extract_Compressed_Events(buf, bufsize);
			break;
	}

	return (count);

}	/* end of Breakup_Receive_Packet */


/***************************************************************************
 * Extract_Uncompressed_Events -- extracts events from a packet            *
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer containing events to extract								*
 *		bufsize		length of 'buf'														*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		# events extracted																	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 DRD : Created.                                             *
 *=========================================================================*/
static int Extract_Uncompressed_Events(void *buf, int bufsize)
{
	int count = 0;
	int pos = 0;
	int leftover = bufsize;
	EventClass *event;

	//------------------------------------------------------------------------
	// Loop until there are no more events in the packet
	//------------------------------------------------------------------------
	while (leftover >= sizeof(EventClass) ) {
		event = (EventClass *)(((char *)buf) + pos);

		//.....................................................................
		// add event to the DoList, only if it's not a FRAMESYNC
		// (but FRAMEINFO's do get added.)
		//.....................................................................
		if (event->Type != EventClass::FRAMESYNC) {
			event->IsExecuted = 0;

			if (!DoList.Add( *event )) {
				return (-1);
			}

			//..................................................................
			// Keep count of how many events we add to the queue
			//..................................................................
			count++;
		}

		//.....................................................................
		// Point to the next position in the buffer; decrement our 'leftover'
		//.....................................................................
		pos += sizeof(EventClass);
		leftover -= sizeof(EventClass);
	}

	return (count);

}	// end of Extract_Uncompressed_Events


/***************************************************************************
 * Extract_Compressed_Events -- extracts events from a packet   				*
 *                                                                         *
 * INPUT:                                                                  *
 *		buf			buffer containing events to extract								*
 *		bufsize		length of 'buf'														*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		# events extracted																	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 DRD : Created.                                             *
 *=========================================================================*/
static int Extract_Compressed_Events(void *buf, int bufsize)
{
	int pos = 0;						// current buffer parsing position
	int leftover = bufsize;			// # bytes left to process
	EventClass *event;				// event ptr for parsing buffer
	int count = 0;						// # events processed
	int datasize = 0;					// size of data to copy
	EventClass eventdata;			// stores Frame, ID, etc
	unsigned char numunits = 0;	// # units stored in compressed MegaMissions
//int lasteventtype=0;
	//------------------------------------------------------------------------
	// Clear work event structure
	//------------------------------------------------------------------------
	memset (&eventdata, 0, sizeof(EventClass));

	//------------------------------------------------------------------------
	// Assume the first event is a FRAMEINFO event
	// Init 'datasize' to the amount of data to copy, minus the EventType value
	// For the 1st packet only, this will include all info before the Data
	// union, plus the size of the FrameInfo structure, minus the EventType size.
	//------------------------------------------------------------------------
	datasize = (offsetof(EventClass, Data) +
		size_of(EventClass, Data.FrameInfo)) - sizeof(EventClass::EventType);
	event = (EventClass *)(((char *)buf) + pos);

	while ((unsigned)leftover >= (datasize + sizeof(EventClass::EventType)) ) {
		//.....................................................................
		// add event to the DoList, only if it's not a FRAMESYNC
		// (but FRAMEINFO's do get added.)
		//.....................................................................
		if (event->Type != EventClass::FRAMESYNC) {
			//..................................................................
			// initialize the common data from the FRAMEINFO event
			// keeping IsExecuted 0
			//..................................................................
			if (event->Type == EventClass::FRAMEINFO) {
				eventdata.Frame = event->Frame;
				eventdata.ID = event->ID;
				eventdata.MPlayerID = event->MPlayerID;

				//...............................................................
				// Adjust position past the common data
				//...............................................................
				pos += (offsetof(EventClass, Data) -
						 sizeof(EventClass::EventType));
				leftover -= (offsetof(EventClass, Data) -
								sizeof(EventClass::EventType));
			}
			//..................................................................
			// if MEGAMISSION event get the number of units (events to generate)
			//..................................................................
			else if (event->Type == EventClass::MEGAMISSION) {
				numunits = *(((unsigned char *)buf) + pos + sizeof(eventdata.Type));
				pos += sizeof(numunits);
				leftover -= sizeof(numunits);
			}

			//..................................................................
			// clear the union data portion of the event
			//..................................................................
			memset (&eventdata.Data, 0, sizeof(eventdata.Data));
			eventdata.Type = event->Type;
			datasize = EventClass::EventLength[ eventdata.Type ];

			switch (eventdata.Type) {
				case (EventClass::RESPONSE_TIME):
					memcpy ( &eventdata.Data.FrameInfo.Delay,
						((char *)buf) + pos + sizeof(EventClass::EventType),
						datasize );
					break;

				case (EventClass::MEGAMISSION):
					memcpy ( &eventdata.Data.MegaMission,
						((char *)buf) + pos + sizeof(EventClass::EventType),
						datasize );

					if (numunits > 1) {
						pos += (datasize + sizeof(EventClass::EventType));
						leftover -= (datasize + sizeof(EventClass::EventType));
						datasize = sizeof(eventdata.Data.MegaMission.Whom);

						while (numunits) {
							if ( !DoList.Add( eventdata ) ) {
								return (-1);
							}

							//......................................................
							// Keep count of how many events we add to the queue
							//......................................................
							count++;
							numunits--;
							memcpy ( &eventdata.Data.MegaMission.Whom,
								((char *)buf) + pos, datasize );

							//......................................................
							// if one unit left fall thru to normal code
							//......................................................
							if (numunits == 1) {
								datasize -= sizeof(EventClass::EventType);
								break;
							}
							else {
								pos += datasize;
								leftover -= datasize;
							}
						}
					}
					break;

				default:
					memcpy ( &eventdata.Data,
						((char *)buf) + pos + sizeof(EventClass::EventType),
						datasize );
					break;
			}

char flip[128];
sprintf (flip, "C&C95 - Adding event type %d to queue\n", eventdata.Type);
CCDebugString (flip);

//if (lasteventtype == 11){
//	break;
//}

//lasteventtype = (int) eventdata.Type;


			if ( !DoList.Add( eventdata ) ) {
				return (-1);
			}

			//..................................................................
			// Keep count of how many events we add to the queue
			//..................................................................
			count++;

			pos += (datasize + sizeof(EventClass::EventType));
			leftover -= (datasize + sizeof(EventClass::EventType));

			if (leftover) {
				event = (EventClass *)(((char *)buf) + pos);
				datasize = EventClass::EventLength[ event->Type ];
				if (event->Type == EventClass::MEGAMISSION) {
					datasize += sizeof(numunits);
				}
			}
		}
		//.....................................................................
		// FRAMESYNC event: This >should< be the only event in the buffer,
		// and it will be uncompressed.
		//.....................................................................
		else {
			pos += (datasize + sizeof(EventClass::EventType));
			leftover -= (datasize + sizeof(EventClass::EventType));
			event = (EventClass *)(((char *)buf) + pos);

			//..................................................................
			// size of FRAMESYNC event - EventType size
			//..................................................................
			datasize = (offsetof(EventClass, Data) +
							size_of(EventClass, Data.FrameInfo)) -
							sizeof(EventClass::EventType);
		}
	}

	return (count);

}	// end of Extract_Compressed_Events

#endif	//DEMO

/***************************************************************************
 * Execute_DoList -- Executes commands from the DoList                     *
 *                                                                         *
 * This routine executes any events in the DoList that need to be executed	*
 * on the current game frame.  The events must be executed in a special		*
 * order, so that all systems execute all events in exactly the same			*
 * order.																						*
 *                                                                         *
 * This routine also handles checking the Game CRC sent by other systems	*
 * against my own, to be sure we're still in sync.									*
 *                                                                         *
 * INPUT:                                                                  *
 *		max_houses	# houses to execute commands for									*
 *		base_house	HousesType to start with											*
 *		net			ptr to connection manager; NULL if none						*
 *		skip_crc		a frame-based countdown timer; if it's non-zero, the		*
 *						CRC check will be skipped.  Ignored if NULL.					*
 *		their_frame	array of their frame #'s											*
 *		their_sent	array of # commands they've sent									*
 *		their_recv	array of # commands I've received from them					*
 *                                                                         *
 * (their_xxx are ignored if 'net' is NULL.)											*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = some error occurred (CRC error, packet rcv'd too late.)	*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Execute_DoList(int , HousesType ,
	ConnManClass *net, TCountDownTimerClass *,
	long *their_frame, unsigned short *their_sent, unsigned short *their_recv)
{
	int i,j,k,wibble;
	int index;

	//------------------------------------------------------------------------
	// For a single-player game, just execute all events in the queue.
	//------------------------------------------------------------------------
	if (GameToPlay == GAME_NORMAL) {
		for (i = 0; i < DoList.Count; i++) {
			if ((unsigned)Frame >= DoList[i].Frame && !DoList[i].IsExecuted) {
				DoList[i].Execute();				// execute it
				DoList[i].IsExecuted = true;	// mark as having been executed
			}
		}
		return (1);
	}

//#if(TIMING_FIX)
	//
	// If MPlayerMaxAhead is recomputed such that it increases, the systems
	// may try to free-run to the new MaxAhead value.  If so, they may miss
	// an event that was generated after the TIMING event was created, but
	// before it executed; this event will be scheduled with the older,
	// shorter MaxAhead value.  If a system doesn't receive this event, it
	// may execute past the frame it's scheduled to execute on, creating
	// a Packet-Recieved-Too-Late error.  To prevent this, find any events
	// that are scheduled to execute during this "period of vulnerability",
	// and re-schedule for the end of that period.
	//
	if (CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		for (j = 0; j < DoList.Count; j++) {
			if (DoList[j].Type != EventClass::FRAMEINFO &&
				DoList[j].Frame > (unsigned)NewMaxAheadFrame1 &&
				DoList[j].Frame < (unsigned)NewMaxAheadFrame2) {
				DoList[j].Frame = (unsigned)NewMaxAheadFrame2;
			}
		}
	}
//#endif

	//------------------------------------------------------------------------
	//	Execute the DoList.  Events must be executed in the same order on all
	//	systems; so, execute them in the order of the MPlayerID array.  This
	//	array is stored in the same order on all systems.
	//------------------------------------------------------------------------
	for (i = 0; i < MPlayerCount; i++) {

		HousesType house;
		HouseClass *housep;

		house = MPlayerHouses [i];
		housep= HouseClass::As_Pointer (house);

		//.....................................................................
		// If for some reason this house doesn't exist, skip it.
		// Also, if this house has exited the game, skip it.  (The user can
		// generate events after he exits, because the exit event is scheduled
		// at least FrameSendRate*3 frames ahead.  If one system gets these
		// packets & another system doesn't, they'll go out of sync because
		// they aren't checking the CommandCount for that house, since that
		// house isn't connected any more.)
		//.....................................................................
		if (!housep){
			continue;
		}

		if (!housep->IsHuman){
			continue;
		}

		//.....................................................................
		//	Loop through all events
		//.....................................................................
		for (j = 0; j < DoList.Count; j++) {

#ifndef DEMO
			if (net)
				Update_Queue_Mono (net, 6);
#endif	//DEMO

			//..................................................................
			//	If this event was from the currently-executing player ID, and it's
			//	time to execute it, execute it.
			//..................................................................
			if (DoList[j].MPlayerID == MPlayerID[i] && (unsigned)Frame >= DoList[j].Frame &&
				!DoList[j].IsExecuted) {

				//...............................................................
				//	Error if it's too late to execute this packet!
				//...............................................................
				if ((unsigned)Frame > DoList[j].Frame && DoList[j].Type !=
					EventClass::FRAMEINFO) {
#ifndef DEMO
					Dump_Packet_Too_Late_Stuff(&DoList[j]);
#endif	//DEMO
					CCMessageBox().Process (TXT_PACKET_TOO_LATE);
					return (0);
				}

				//...............................................................
				//	Only execute EXIT & OPTIONS commands if they're from myself.
				//...............................................................
				if (DoList[j].Type==EventClass::EXIT ||
						DoList[j].Type==EventClass::OPTIONS) {


					if (DoList[j].Type==EventClass::EXIT){
CCDebugString ("C&C95 -  Received EXIT packet\n");

						/*
						** Flag that this house lost because it quit. ST - 6/5/96 0:29AM
						*/
						for (wibble = 0; wibble < MPlayerCount; wibble++) {
							if (MPlayerID[wibble] == DoList[j].MPlayerID) {
								house = MPlayerHouses[wibble];
								housep = HouseClass::As_Pointer (house);
								housep->IGaveUp = true;
								break;
							}
						}

						/*
						** Send the game statistics packet now since the game is effectivly over
						*/
						if (MPlayerCount == 2 &&
								GameToPlay == GAME_INTERNET &&
								!GameStatisticsPacketSent){
							Register_Game_End_Time();
							Send_Statistics_Packet();
						}
					}



					if (DoList[j].ID == Houses.ID(PlayerPtr)) {
						DoList[j].Execute();
					}

					//............................................................
					//	If this EXIT event isn't from myself, destroy the connection
					//	for that player.  The HousesType for this event is the
					// connection ID.
					//............................................................
					else if (DoList[j].Type==EventClass::EXIT) {
						if (GameToPlay == GAME_MODEM
							|| GameToPlay == GAME_NULL_MODEM){
							//|| GameToPlay == GAME_INTERNET) {
							
							//ST - 1/2/2019 5:29PM
							//Destroy_Null_Connection( DoList[j].MPlayerID, 0 );
						}

						else if ((GameToPlay == GAME_IPX || GameToPlay == GAME_INTERNET) && net) {
							index = net->Connection_Index (DoList[j].MPlayerID);
							if (index != -1) {
								for (k = index; k < net->Num_Connections() - 1; k++) {
									their_frame[k] = their_frame[k+1];
									their_sent[k] = their_sent[k+1];
									their_recv[k] = their_recv[k+1];
								}
								CCDebugString ("C&C95 = Destroying connection due to exit event\n");
#ifndef DEMO
								Destroy_Connection(DoList[j].MPlayerID,0);
#endif	//DEMO
							}
						}
					}
				}

				//...............................................................
				//	For a FRAMEINFO event, check the CRC value.
				// This could be an old FRAMEINFO packet that was floating around
				// for awhile and just arrived; if so, its Frame value will be
				// old.  Ignore these packets.  (This created bogus sync bugs on
				// the Internet, when packets that were 35 frames old arrived.)
				//...............................................................
#ifndef DEMO
				else if (DoList[j].Type == EventClass::FRAMEINFO) {
					if (DoList[j].Frame == Frame &&
						DoList[j].Data.FrameInfo.Delay < 32) {
						index = ((DoList[j].Frame - DoList[j].Data.FrameInfo.Delay) &
							0x001f);
						if (CRC[index] != DoList[j].Data.FrameInfo.CRC) {
							Print_CRCs(&DoList[j]);
							if (CCMessageBox().Process (TXT_OUT_OF_SYNC,
								TXT_CONTINUE, TXT_STOP) == 0) {
								if (GameToPlay == GAME_MODEM ||
									GameToPlay == GAME_NULL_MODEM){
#if (0)//ST - 1/2/2019 5:29PM										
									Destroy_Null_Connection( DoList[j].MPlayerID, -1 );
									Shutdown_Modem();
#endif
									GameToPlay = GAME_NORMAL;
								}
								else if ((GameToPlay == GAME_IPX || GameToPlay == GAME_INTERNET) && net) {
									CCDebugString ("C&C95 = Destroying connections due to bad frame info packet\n");
									while (net->Num_Connections()) {
										Destroy_Connection (net->Connection_ID(0), -1);
									}
								}
								Map.Flag_To_Redraw(true);
							}
							else {
								return (0);
							}
							return (1);
						}
					}
				}
#endif	//DEMO
				//...............................................................
				//	Execute other commands
				//...............................................................
				else {
					DoList[j].Execute();
				}

				//...............................................................
				//	Mark this event as executed.
				//...............................................................
				DoList[j].IsExecuted = 1;
			}
		}
	}

	return (1);

}	// end of Execute_DoList


/***************************************************************************
 * Clean_DoList -- Cleans out old events from the DoList                   *
 *                                                                         *
 * Currently, an event can only be removed from the DoList if it's at the	*
 * head of the list; and event can't be removed from the middle.  So,		*
 * this routine loops as long as the next event in the DoList has been		*
 * executed, it's removed.																	*
 *                                                                         *
 * INPUT:                                                                  *
 *		net		ptr to connection manager; ignored if NULL						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Clean_DoList(ConnManClass *net)
{
	while (DoList.Count) {

#ifndef DEMO
		if (net)
			Update_Queue_Mono (net, 7);
#else
		net = net;
#endif	//DEMO

		//.....................................................................
		//	Discard events that have been executed, OR it's too late to execute.
		//	(This happens if another player exits the game; he'll leave FRAMEINFO
		//	events lying around in my queue.  They won't have been "executed",
		//	because his IPX connection was destroyed.)
		//.....................................................................
		if ( (DoList.First().IsExecuted) || ((unsigned)Frame > DoList.First().Frame) ) {
			DoList.Next();
		}
		else {
			break;
		}
	}

}	// end of Clean_DoList

#ifndef DEMO

/***************************************************************************
 * Queue_Record -- Records the DoList to disk                              *
 *                                                                         *
 * This routine just saves any events in the DoList to disk; we can later	*
 * "play back" the recording just be pulling events from disk rather than	*
 * from the network!																			*
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
 *   08/14/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Queue_Record(void)
{
	int i,j;

	//------------------------------------------------------------------------
	//	Compute # of events to save this frame
	//------------------------------------------------------------------------
	j = 0;
	for (i = 0; i < DoList.Count; i++) {
		if (Frame == DoList[i].Frame && !DoList[i].IsExecuted) {
			j++;
		}
	}

	//------------------------------------------------------------------------
	//	Save the # of events, then all events.
	//------------------------------------------------------------------------
	RecordFile.Write (&j,sizeof(j));
	for (i = 0; i < DoList.Count; i++) {
		if (Frame == DoList[i].Frame && !DoList[i].IsExecuted) {
			RecordFile.Write (&DoList[i],sizeof (EventClass));
			j--;
		}
	}

}	/* end of Queue_Record */


/***************************************************************************
 * Queue_Playback -- plays back queue entries from a record file           *
 *                                                                         *
 * This routine reads events from disk, putting them into the DoList;		*
 * it then executes the DoList just like the network version does.  The		*
 * result is that the game "plays back" like a recording.						*
 *																									*
 * This routine detects mouse motion and stops playback, so it can work		*
 * like an "attract" mode, showing a demo of the game itself.					*
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
 *   05/15/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Queue_Playback(void)
{
	int numevents;
	EventClass event;
	int i;
	int ok;
  	static int mx,my;
	int max_houses;
	HousesType base_house;
	int key;
	int testframe;

	//------------------------------------------------------------------------
	//	If the user hits ESC, stop the playback
	//------------------------------------------------------------------------
	if (Check_Key_Num()) {
		key = Get_Key();
		//
		// If the user hit ESC, end the recording.  If this is an Attract-mode
		// recording, end it no matter what the user does (any key or mouse).
		//
		if (key == KA_ESC || AllowAttract) {
			GameActive = 0;
			return;
		}
	}

	//------------------------------------------------------------------------
	// If we're in "Attact" mode, and the user moves the mouse, stop the
	// playback.
	//------------------------------------------------------------------------
	if (AllowAttract && Frame > 0 &&
		(mx != Get_Mouse_X() || my != Get_Mouse_Y())) {
		GameActive = 0;
		return;
	}
	mx = Get_Mouse_X();
	my = Get_Mouse_Y();

	//------------------------------------------------------------------------
	//	Compute the Game's CRC
	//------------------------------------------------------------------------
	Compute_Game_CRC();
	CRC[Frame & 0x001f] = GameCRC;

	//------------------------------------------------------------------------
	//	Don't read anything the first time through (since the Queue_AI_Network
	//	routine didn't write anything the first time through); do this after the
	//	CRC is computed, since we'll still need a CRC for Frame 0.
	//------------------------------------------------------------------------
	if (Frame==0 && GameToPlay!=GAME_NORMAL) {
		return;
	}

	//------------------------------------------------------------------------
	// Only process every 'FrameSendRate' frames
	//------------------------------------------------------------------------
	testframe = ((Frame + (FrameSendRate - 1)) / FrameSendRate) * FrameSendRate;

	if (GameToPlay != GAME_NORMAL &&
		CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		if (Frame != testframe) {
			return;
		}
	}

	//------------------------------------------------------------------------
	//	Read the DoList from disk
	//------------------------------------------------------------------------
	ok = 1;
	if (RecordFile.Read (&numevents, sizeof(numevents)) ==
		sizeof(numevents)) {
		for (i = 0; i < numevents; i++) {
			if (RecordFile.Read (&event, sizeof(EventClass)) ==
				sizeof(EventClass)) {
				event.IsExecuted = 0;
				DoList.Add (event);
			}
			else {
				ok = 0;
				break;
			}
		}
	}
	else {
		ok = 0;
	}

	if (!ok) {
		GameActive = 0;
		return;
	}


	//------------------------------------------------------------------------
	// Execute the DoList; if an error occurs, bail out.
	//------------------------------------------------------------------------
	if (GameToPlay == GAME_NORMAL) {
		max_houses = 1;
		base_house = PlayerPtr->Class->House;
	}
	else {
		max_houses = MPlayerMax;
		base_house = HOUSE_MULTI1;
	}
	if (!Execute_DoList(max_houses, base_house, NULL, NULL, NULL, NULL, NULL)) {
		GameActive = 0;
		return;
	}

	//------------------------------------------------------------------------
	//	Clean out the DoList
	//------------------------------------------------------------------------
	Clean_DoList(NULL);

}	/* end of Queue_Playback */


/***************************************************************************
 * Compute_Game_CRC -- Computes a CRC value of the entire game.				*
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
 *   05/09/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Compute_Game_CRC(void)
{
	int i;
	InfantryClass *infp;
	UnitClass *unitp;
	BuildingClass *bldgp;
	//ObjectClass *objp;

	GameCRC = 0;

	//------------------------------------------------------------------------
	//	Infantry
	//------------------------------------------------------------------------
	for (i = 0; i < Infantry.Count(); i++) {
		infp = (InfantryClass *)Infantry.Active_Ptr(i);
		Add_CRC (&GameCRC, (int)infp->Coord + (int)infp->PrimaryFacing);
	}

	//------------------------------------------------------------------------
	//	Units
	//------------------------------------------------------------------------
	for (i = 0; i < Units.Count(); i++) {
		unitp = (UnitClass *)Units.Active_Ptr(i);
		Add_CRC (&GameCRC, (int)unitp->Coord + (int)unitp->PrimaryFacing +
			(int)unitp->SecondaryFacing);
	}

	//------------------------------------------------------------------------
	//	Buildings
	//------------------------------------------------------------------------
	for (i = 0; i < Buildings.Count(); i++) {
		bldgp = (BuildingClass *)Buildings.Active_Ptr(i);
		Add_CRC (&GameCRC, (int)bldgp->Coord + (int)bldgp->PrimaryFacing);
	}

#if(0)
	//------------------------------------------------------------------------
	//	Map Layers
	//------------------------------------------------------------------------
	for (i = 0; i < LAYER_COUNT; i++) {
		for (j = 0; j < Map.Layer[i].Count(); j++) {
			objp = Map.Layer[i][j];
			Add_CRC (&GameCRC, (int)objp->Coord + (int)objp->What_Am_I());
		}
	}

	//------------------------------------------------------------------------
	//	Logic Layers
	//------------------------------------------------------------------------
	for (i = 0; i < Logic.Count(); i++) {
		objp = Logic[i];
		Add_CRC (&GameCRC, (int)objp->Coord + (int)objp->What_Am_I());
	}
#endif

	//------------------------------------------------------------------------
	//	A random #
	//------------------------------------------------------------------------
	Add_CRC(&GameCRC, rand());

}	/* end of Compute_Game_CRC */


/***************************************************************************
 * Add_CRC -- Adds a value to a CRC                                        *
 *                                                                         *
 * INPUT:                                                                  *
 *		crc		ptr to crc																	*
 *		val		value to add																*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/09/1995 BRR : Created.                                             *
 *=========================================================================*/
void Add_CRC(unsigned long *crc, unsigned long val)
{
	int hibit;

	if ( (*crc) & 0x80000000) {
		hibit = 1;
	}
	else {
		hibit = 0;
	}

	(*crc) <<= 1;
	(*crc) += val;
	(*crc) += hibit;

}	/* end of Add_CRC */


/***************************************************************************
 * Print_CRCs -- Prints a data file for finding Sync Bugs						*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/09/1995 BRR : Created.                                             *
 *=========================================================================*/
void Print_CRCs(EventClass *ev)
{
	ev=ev;

	int i;	//,j;
	InfantryClass *infp;
	UnitClass *unitp;
	BuildingClass *bldgp;
	//ObjectClass *objp;
	FILE *fp;
	int rnd;
	HouseClass *housep;
	//HousesType house;
	int color;

	Mono_Clear_Screen();
	Mono_Set_Cursor (0,0);

	char filename[80];
	sprintf (filename, "CRC%02d.TXT", Frame & 0x1f);

	fp = fopen(filename, "wt");	//"OUT.TXT","wt");
	if (fp==NULL) {
		return;
	}

	for (i = 0; i < 32; i++) {
		fprintf(fp,"CRC[%d]=%x\n",i,CRC[i]);
	}


	housep = HouseClass::As_Pointer (HOUSE_MULTI1);
	if (housep) {
		color = housep->RemapColor;
		fprintf(fp,"Multi1: IsHuman:%d  Color:%s\n",housep->IsHuman,
			ColorNames[color]);
	}

	housep = HouseClass::As_Pointer (HOUSE_MULTI2);
	if (housep) {
		color = housep->RemapColor;
		fprintf(fp,"Multi2: IsHuman:%d  Color:%s\n",housep->IsHuman,
			ColorNames[color]);
	}

	housep = HouseClass::As_Pointer (HOUSE_MULTI3);
	if (housep) {
		color = housep->RemapColor;
		fprintf(fp,"Multi3: IsHuman:%d  Color:%s\n",housep->IsHuman,
			ColorNames[color]);
	}

	housep = HouseClass::As_Pointer (HOUSE_MULTI4);
	if (housep) {
		color = housep->RemapColor;
		fprintf(fp,"Multi4: IsHuman:%d  Color:%s\n",housep->IsHuman,
			ColorNames[color]);
	}

	housep = HouseClass::As_Pointer (HOUSE_MULTI5);
	if (housep) {
		color = housep->RemapColor;
		fprintf(fp,"Multi5: IsHuman:%d  Color:%s\n",housep->IsHuman,
			ColorNames[color]);
	}

	housep = HouseClass::As_Pointer (HOUSE_MULTI6);
	if (housep) {
		color = housep->RemapColor;
		fprintf(fp,"Multi6: IsHuman:%d  Color:%s\n",housep->IsHuman,
			ColorNames[color]);
	}


	//------------------------------------------------------------------------
	//	Multi1 Infantry
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI1)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI1 INFANTRY -------------------\n");
		for (i = 0; i < Infantry.Count(); i++) {
			infp = (InfantryClass *)Infantry.Active_Ptr(i);
			if (infp->Owner()==HOUSE_MULTI1) {
				Add_CRC (&GameCRC, (int)infp->Coord + (int)infp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					infp->Coord,(int)infp->PrimaryFacing,infp->Get_Mission(),
					infp->Class->Type);
			}
		}
		Mono_Printf("Multi1 Infantry:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi2 Infantry
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI2)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI2 INFANTRY -------------------\n");
		for (i = 0; i < Infantry.Count(); i++) {
			infp = (InfantryClass *)Infantry.Active_Ptr(i);
			if (infp->Owner()==HOUSE_MULTI2) {
				Add_CRC (&GameCRC, (int)infp->Coord + (int)infp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					infp->Coord,(int)infp->PrimaryFacing,infp->Get_Mission(),
					infp->Class->Type);
			}
		}
		Mono_Printf("Multi2 Infantry:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi3 Infantry
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI3)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI3 INFANTRY -------------------\n");
		for (i = 0; i < Infantry.Count(); i++) {
			infp = (InfantryClass *)Infantry.Active_Ptr(i);
			if (infp->Owner()==HOUSE_MULTI3) {
				Add_CRC (&GameCRC, (int)infp->Coord + (int)infp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					infp->Coord,(int)infp->PrimaryFacing,infp->Get_Mission(),
					infp->Class->Type);
			}
		}
		Mono_Printf("Multi3 Infantry:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi4 Infantry
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI4)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI4 INFANTRY -------------------\n");
		for (i = 0; i < Infantry.Count(); i++) {
			infp = (InfantryClass *)Infantry.Active_Ptr(i);
			if (infp->Owner()==HOUSE_MULTI4) {
				Add_CRC (&GameCRC, (int)infp->Coord + (int)infp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					infp->Coord,(int)infp->PrimaryFacing,infp->Get_Mission(),
					infp->Class->Type);
			}
		}
		Mono_Printf("Multi4 Infantry:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi5 Infantry
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI5)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI5 INFANTRY -------------------\n");
		for (i = 0; i < Infantry.Count(); i++) {
			infp = (InfantryClass *)Infantry.Active_Ptr(i);
			if (infp->Owner()==HOUSE_MULTI5) {
				Add_CRC (&GameCRC, (int)infp->Coord + (int)infp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					infp->Coord,(int)infp->PrimaryFacing,infp->Get_Mission(),
					infp->Class->Type);
			}
		}
		Mono_Printf("Multi5 Infantry:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi6 Infantry
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI6)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI6 INFANTRY -------------------\n");
		for (i = 0; i < Infantry.Count(); i++) {
			infp = (InfantryClass *)Infantry.Active_Ptr(i);
			if (infp->Owner()==HOUSE_MULTI6) {
				Add_CRC (&GameCRC, (int)infp->Coord + (int)infp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					infp->Coord,(int)infp->PrimaryFacing,infp->Get_Mission(),
					infp->Class->Type);
			}
		}
		Mono_Printf("Multi6 Infantry:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi1 Units
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI1)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI1 UNITS -------------------\n");
		for (i = 0; i < Units.Count(); i++) {
			unitp = (UnitClass *)Units.Active_Ptr(i);
			if (unitp->Owner()==HOUSE_MULTI1) {
				Add_CRC (&GameCRC, (int)unitp->Coord + (int)unitp->PrimaryFacing);
				fprintf(fp,
					"COORD:%x   Facing:%d   Facing2:%d   Mission:%d   Type:%d\n",
					unitp->Coord,(int)unitp->PrimaryFacing,
					(int)unitp->SecondaryFacing,unitp->Get_Mission(),
					unitp->Class->Type);
			}
		}
		Mono_Printf("Multi1 Units:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi2 Units
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI2)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI2 UNITS -------------------\n");
		for (i = 0; i < Units.Count(); i++) {
			unitp = (UnitClass *)Units.Active_Ptr(i);
			if (unitp->Owner()==HOUSE_MULTI2) {
				Add_CRC (&GameCRC, (int)unitp->Coord + (int)unitp->PrimaryFacing);
				fprintf(fp,
					"COORD:%x   Facing:%d   Facing2:%d   Mission:%d   Type:%d\n",
					unitp->Coord,(int)unitp->PrimaryFacing,
					(int)unitp->SecondaryFacing,unitp->Get_Mission(),
					unitp->Class->Type);
			}
		}
		Mono_Printf("Multi2 Units:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi3 Units
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI3)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI3 UNITS -------------------\n");
		for (i = 0; i < Units.Count(); i++) {
			unitp = (UnitClass *)Units.Active_Ptr(i);
			if (unitp->Owner()==HOUSE_MULTI3) {
				Add_CRC (&GameCRC, (int)unitp->Coord + (int)unitp->PrimaryFacing);
				fprintf(fp,
					"COORD:%x   Facing:%d   Facing2:%d   Mission:%d   Type:%d\n",
					unitp->Coord,(int)unitp->PrimaryFacing,
					(int)unitp->SecondaryFacing,unitp->Get_Mission(),
					unitp->Class->Type);
			}
		}
		Mono_Printf("Multi3 Units:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi4 Units
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI4)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI4 UNITS -------------------\n");
		for (i = 0; i < Units.Count(); i++) {
			unitp = (UnitClass *)Units.Active_Ptr(i);
			if (unitp->Owner()==HOUSE_MULTI4) {
				Add_CRC (&GameCRC, (int)unitp->Coord + (int)unitp->PrimaryFacing);
				fprintf(fp,
					"COORD:%x   Facing:%d   Facing2:%d   Mission:%d   Type:%d\n",
					unitp->Coord,(int)unitp->PrimaryFacing,
					(int)unitp->SecondaryFacing,unitp->Get_Mission(),
					unitp->Class->Type);
			}
		}
		Mono_Printf("Multi4 Units:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi5 Units
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI5)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI5 UNITS -------------------\n");
		for (i = 0; i < Units.Count(); i++) {
			unitp = (UnitClass *)Units.Active_Ptr(i);
			if (unitp->Owner()==HOUSE_MULTI5) {
				Add_CRC (&GameCRC, (int)unitp->Coord + (int)unitp->PrimaryFacing);
				fprintf(fp,
					"COORD:%x   Facing:%d   Facing2:%d   Mission:%d   Type:%d\n",
					unitp->Coord,(int)unitp->PrimaryFacing,
					(int)unitp->SecondaryFacing,unitp->Get_Mission(),
					unitp->Class->Type);
			}
		}
		Mono_Printf("Multi5 Units:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi6 Units
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI6)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI6 UNITS -------------------\n");
		for (i = 0; i < Units.Count(); i++) {
			unitp = (UnitClass *)Units.Active_Ptr(i);
			if (unitp->Owner()==HOUSE_MULTI6) {
				Add_CRC (&GameCRC, (int)unitp->Coord + (int)unitp->PrimaryFacing);
				fprintf(fp,
					"COORD:%x   Facing:%d   Facing2:%d   Mission:%d   Type:%d\n",
					unitp->Coord,(int)unitp->PrimaryFacing,
					(int)unitp->SecondaryFacing,unitp->Get_Mission(),
					unitp->Class->Type);
			}
		}
		Mono_Printf("Multi6 Units:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi1 Buildings
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI1)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI1 BUILDINGS -------------------\n");
		for (i = 0; i < Buildings.Count(); i++) {
			bldgp = (BuildingClass *)Buildings.Active_Ptr(i);
			if (bldgp->Owner()==HOUSE_MULTI1) {
				Add_CRC (&GameCRC, (int)bldgp->Coord + (int)bldgp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					bldgp->Coord,(int)bldgp->PrimaryFacing,bldgp->Get_Mission(),
					bldgp->Class->Type);
			}
		}
		Mono_Printf("Multi1 Buildings:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi2 Buildings
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI2)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI2 BUILDINGS -------------------\n");
		for (i = 0; i < Buildings.Count(); i++) {
			bldgp = (BuildingClass *)Buildings.Active_Ptr(i);
			if (bldgp->Owner()==HOUSE_MULTI2) {
				Add_CRC (&GameCRC, (int)bldgp->Coord + (int)bldgp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					bldgp->Coord,(int)bldgp->PrimaryFacing,bldgp->Get_Mission(),
					bldgp->Class->Type);
			}
		}
		Mono_Printf("Multi2 Buildings:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi3 Buildings
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI3)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI3 BUILDINGS -------------------\n");
		for (i = 0; i < Buildings.Count(); i++) {
			bldgp = (BuildingClass *)Buildings.Active_Ptr(i);
			if (bldgp->Owner()==HOUSE_MULTI3) {
				Add_CRC (&GameCRC, (int)bldgp->Coord + (int)bldgp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					bldgp->Coord,(int)bldgp->PrimaryFacing,bldgp->Get_Mission(),
					bldgp->Class->Type);
			}
		}
		Mono_Printf("Multi3 Buildings:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi4 Buildings
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI4)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI4 BUILDINGS -------------------\n");
		for (i = 0; i < Buildings.Count(); i++) {
			bldgp = (BuildingClass *)Buildings.Active_Ptr(i);
			if (bldgp->Owner()==HOUSE_MULTI4) {
				Add_CRC (&GameCRC, (int)bldgp->Coord + (int)bldgp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					bldgp->Coord,(int)bldgp->PrimaryFacing,bldgp->Get_Mission(),
					bldgp->Class->Type);
			}
		}
		Mono_Printf("Multi4 Buildings:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi5 Buildings
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI5)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI5 BUILDINGS -------------------\n");
		for (i = 0; i < Buildings.Count(); i++) {
			bldgp = (BuildingClass *)Buildings.Active_Ptr(i);
			if (bldgp->Owner()==HOUSE_MULTI5) {
				Add_CRC (&GameCRC, (int)bldgp->Coord + (int)bldgp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					bldgp->Coord,(int)bldgp->PrimaryFacing,bldgp->Get_Mission(),
					bldgp->Class->Type);
			}
		}
		Mono_Printf("Multi5 Buildings:%d\n",GameCRC);
	}

	//------------------------------------------------------------------------
	//	Multi6 Buildings
	//------------------------------------------------------------------------
	if (HouseClass::As_Pointer(HOUSE_MULTI6)) {
		GameCRC = 0;
		fprintf(fp,"-------------------- MULTI6 BUILDINGS -------------------\n");
		for (i = 0; i < Buildings.Count(); i++) {
			bldgp = (BuildingClass *)Buildings.Active_Ptr(i);
			if (bldgp->Owner()==HOUSE_MULTI6) {
				Add_CRC (&GameCRC, (int)bldgp->Coord + (int)bldgp->PrimaryFacing);
				fprintf(fp,"COORD:%x   Facing:%d   Mission:%d   Type:%d\n",
					bldgp->Coord,(int)bldgp->PrimaryFacing,bldgp->Get_Mission(),
					bldgp->Class->Type);
			}
		}
		Mono_Printf("Multi6 Buildings:%d\n",GameCRC);
	}
#if (0)
	//------------------------------------------------------------------------
	//	Map Layers
	//------------------------------------------------------------------------
	GameCRC = 0;
	for (i = 0; i < LAYER_COUNT; i++) {
		fprintf(fp,">>>> MAP LAYER %d <<<<\n",i);
		for (j = 0; j < Map.Layer[i].Count(); j++) {
			objp = Map.Layer[i][j];
			Add_CRC (&GameCRC, (int)objp->Coord + (int)objp->What_Am_I());
			fprintf(fp,"Object %d: %x ",j,objp->Coord);

			if (objp->What_Am_I() == RTTI_AIRCRAFT)
				fprintf(fp,"Aircraft  (Type:%d) ",
					(AircraftType)(*((AircraftClass *)objp)));
			else if (objp->What_Am_I() == RTTI_ANIM)
				fprintf(fp,"Anim      (Type:%d) ",
					(AnimType)(*((AnimClass *)objp)));
			else if (objp->What_Am_I() == RTTI_BUILDING)
				fprintf(fp,"Building  (Type:%d) ",
					(StructType)(*((BuildingClass *)objp)));
			else if (objp->What_Am_I() == RTTI_BULLET)
				fprintf(fp,"Bullet    (Type:%d) ",
					(BulletType)(*((BulletClass *)objp)));
			else if (objp->What_Am_I() == RTTI_INFANTRY)
				fprintf(fp,"Infantry  (Type:%d) ",
					(InfantryType)(*((InfantryClass *)objp)));
			else if (objp->What_Am_I() == RTTI_OVERLAY)
				fprintf(fp,"Overlay   (Type:%d) ",
					(OverlayType)(*((OverlayClass *)objp)));
			else if (objp->What_Am_I() == RTTI_SMUDGE)
				fprintf(fp,"Smudge    (Type:%d) ",
					(SmudgeType)(*((SmudgeClass *)objp)));
			else if (objp->What_Am_I() == RTTI_TEMPLATE)
				fprintf(fp,"Template  (Type:%d) ",
					(TemplateType)(*((TemplateClass *)objp)));
			else if (objp->What_Am_I() == RTTI_TERRAIN)
				fprintf(fp,"Terrain   (Type:%d) ",
					(TerrainType)(*((TerrainClass *)objp)));
			else if (objp->What_Am_I() == RTTI_UNIT)
				fprintf(fp,"Unit      (Type:%d) ",
					(UnitType)(*((UnitClass *)objp)));

			house = objp->Owner();
			if (house!=HOUSE_NONE) {
				housep = HouseClass::As_Pointer (house);
				fprintf(fp,"Owner: %s\n",housep->Class->IniName);
			}
			else {
				fprintf(fp,"Owner: NONE\n");
			}
		}
	}
	Mono_Printf("Map Layers:%d\n",GameCRC);

	//------------------------------------------------------------------------
	//	Logic Layers
	//------------------------------------------------------------------------
	GameCRC = 0;
	fprintf(fp,">>>> LOGIC LAYER <<<<\n");
	for (i = 0; i < Logic.Count(); i++) {
		objp = Logic[i];
		Add_CRC (&GameCRC, (int)objp->Coord + (int)objp->What_Am_I());
		fprintf(fp,"Object %d: %x ",i,objp->Coord);

		if (objp->What_Am_I() == RTTI_AIRCRAFT)
			fprintf(fp,"Aircraft  (Type:%d) ",
				(AircraftType)(*((AircraftClass *)objp)));
		else if (objp->What_Am_I() == RTTI_ANIM)
			fprintf(fp,"Anim      (Type:%d) ",
				(AnimType)(*((AnimClass *)objp)));
		else if (objp->What_Am_I() == RTTI_BUILDING)
			fprintf(fp,"Building  (Type:%d) ",
				(StructType)(*((BuildingClass *)objp)));
		else if (objp->What_Am_I() == RTTI_BULLET)
			fprintf(fp,"Bullet    (Type:%d) ",
				(BulletType)(*((BulletClass *)objp)));
		else if (objp->What_Am_I() == RTTI_INFANTRY)
			fprintf(fp,"Infantry  (Type:%d) ",
				(InfantryType)(*((InfantryClass *)objp)));
		else if (objp->What_Am_I() == RTTI_OVERLAY)
			fprintf(fp,"Overlay   (Type:%d) ",
				(OverlayType)(*((OverlayClass *)objp)));
		else if (objp->What_Am_I() == RTTI_SMUDGE)
			fprintf(fp,"Smudge    (Type:%d) ",
				(SmudgeType)(*((SmudgeClass *)objp)));
		else if (objp->What_Am_I() == RTTI_TEMPLATE)
			fprintf(fp,"Template  (Type:%d) ",
				(TemplateType)(*((TemplateClass *)objp)));
		else if (objp->What_Am_I() == RTTI_TERRAIN)
			fprintf(fp,"Terrain   (Type:%d) ",
				(TerrainType)(*((TerrainClass *)objp)));
		else if (objp->What_Am_I() == RTTI_UNIT)
			fprintf(fp,"Unit      (Type:%d) ",
				(UnitType)(*((UnitClass *)objp)));

		house = objp->Owner();
		if (house!=HOUSE_NONE) {
			housep = HouseClass::As_Pointer (house);
			fprintf(fp,"Owner: %s\n",housep->Class->IniName);
		}
		else {
			fprintf(fp,"Owner: NONE\n");
		}
	}
	Mono_Printf("Logic:%d\n",GameCRC);
#endif

	//------------------------------------------------------------------------
	//	Random # generator, frame #
	//------------------------------------------------------------------------
	rnd = rand();

	Mono_Printf("Random Number:%d\n",rnd);
	fprintf(fp,"\nRandom Number:%d\n",rnd);

	Mono_Printf("My Frame:%d\n",Frame);
	fprintf(fp,"My Frame:%d\n",Frame);
#if (0)
	fprintf(fp,"-------------- Offending event: ----------------\n");
	fprintf(fp,"Type:         %d\n",ev->Type);
	fprintf(fp,"Frame:        %d\n",ev->Frame);
	fprintf(fp,"MPlayerID:    %04x\n",ev->MPlayerID);
	if (ev->Type == EventClass::FRAMEINFO) {
		fprintf(fp,"CRC:          %x\n",ev->Data.FrameInfo.CRC);
		fprintf(fp,"CommandCount: %x\n",ev->Data.FrameInfo.CommandCount);
		fprintf(fp,"Delay:        %x\n",ev->Data.FrameInfo.Delay);
	}
#endif
	fclose(fp);

}	/* end of Print_CRCs */


/***************************************************************************
 * Init_Queue_Mono -- inits mono display                                   *
 *                                                                         *
 * This routine steals control of the mono screen away from the rest of		*
 * the engine, by setting the global IsMono; if IsMono is set, the other	*
 * routines in this module turn off the Mono display when they're done		*
 * with it, so the rest of the engine won't over-write what we're writing.	*
 *                                                                         *
 * INPUT:                                                                  *
 *		net		ptr to connection manager												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Init_Queue_Mono(ConnManClass *net)
{
#if(SHOW_MONO)
	//------------------------------------------------------------------------
	// Set 'IsMono' so we can steal the mono screen from the engine
	//------------------------------------------------------------------------
	if (Frame==0 && MonoClass::Is_Enabled()) {
		IsMono = true;
	}

	//------------------------------------------------------------------------
	// Enable mono output for our stuff; we must Disable it before we return
	// control to the engine.
	//------------------------------------------------------------------------
	if (IsMono)
		MonoClass::Enable();

	if (net->Num_Connections() > 0) {
		//.....................................................................
		//	Network mono debugging screen
		//.....................................................................
		if (NetMonoMode==0) {
			if (Frame==0 || NewMonoMode) {
				net->Configure_Debug (0, sizeof (CommHeaderType),
					sizeof(EventClass::EventType), EventClass::EventNames, 0);
				net->Mono_Debug_Print (0,1);
				NewMonoMode = 0;
			}
			else {
				net->Mono_Debug_Print (0,0);
			}
		}
		//.....................................................................
		//	Flow control debugging output
		//.....................................................................
		else {
			if (NewMonoMode) {
				Mono_Clear_Screen();
				Mono_Printf("                         Queue AI:\n");	// flowcount[0]
				Mono_Printf("                Build Packet Loop:\n");	// flowcount[1]
				Mono_Printf("                       Frame Sync:\n");	// flowcount[2]
				Mono_Printf("                Frame Sync Resend:\n");	// flowcount[3]
				Mono_Printf("               Frame Sync Timeout:\n");	// flowcount[4]
				Mono_Printf("           Frame Sync New Message:\n");	// flowcount[5]
				Mono_Printf("                 DoList Execution:\n");	// flowcount[6]
				Mono_Printf("                  DoList Cleaning:\n");	// flowcount[7]
				Mono_Printf("\n");
				Mono_Printf("                            Frame:\n");
				Mono_Printf("                  MPlayerMaxAhead:\n");
				Mono_Printf("                       their_recv:\n");
				Mono_Printf("                       their_sent:\n");
				Mono_Printf("                          my_sent:\n");
				Mono_Printf("                 DesiredFrameRate:\n");
				NewMonoMode = 0;
			}
		}
	}
#else
	net = net;
#endif
}	// end of Init_Queue_Mono


/***************************************************************************
 * Update_Queue_Mono -- updates mono display                               *
 *                                                                         *
 * INPUT:                                                                  *
 *		net				ptr to connection manager										*
 *		flow_index		index # for flow-count updates								*
 *							-1: display															*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Update_Queue_Mono(ConnManClass *net, int flow_index)
{
#if(SHOW_MONO)
	static int flowcount[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	//------------------------------------------------------------------------
	// If 'NetMonoMode' is 1, display flowcount info
	//------------------------------------------------------------------------
	if (NetMonoMode==1) {
		if (flow_index >= 0 && flow_index < 20) {
			Mono_Set_Cursor(35,flow_index);
			flowcount[flow_index]++;
			Mono_Printf("%d",flowcount[flow_index]);
		}
	}
	//------------------------------------------------------------------------
	// Otherwise, display the connection debug screen
	//------------------------------------------------------------------------
	else {
		net->Mono_Debug_Print (0,0);
	}

#else
	flow_index = flow_index;
	net = net;
#endif

}	// end of Update_Queue_Mono


/***************************************************************************
 * Print_Framesync_Values -- displays frame-sync variables                 *
 *                                                                         *
 * INPUT:                                                                  *
 *		curframe					current game Frame #										*
 *		max_ahead				max-ahead value											*
 *		num_connections		# connections												*
 *		their_recv				# commands I've received from my connections		*
 *		their_sent				# commands each connection claims to have sent	*
 *		my_sent					# commands I've sent										*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Print_Framesync_Values(long curframe, unsigned long max_ahead,
	int num_connections, unsigned short *their_recv,
	unsigned short *their_sent, unsigned short my_sent)
{
#if(SHOW_MONO)
	int i;

	if (NetMonoMode==1) {
		Mono_Set_Cursor(35,9);
		Mono_Printf("%d",curframe);

		Mono_Set_Cursor(35,10);
		Mono_Printf("%d",max_ahead);

		for (i = 0; i < num_connections; i++) {
			Mono_Set_Cursor(35 + i*5,11);
			Mono_Printf("%4d",(int)their_recv[i]);
		}

		for (i = 0; i < num_connections; i++) {
			Mono_Set_Cursor(35 + i*5,12);
			Mono_Printf("%4d",(int)their_sent[i]);
		}

		Mono_Set_Cursor(35,13);
		Mono_Printf("%4d",(int)my_sent);

		Mono_Set_Cursor(35,14);
		Mono_Printf("%2d",(int)DesiredFrameRate);
	}
#else
	curframe = curframe;
	max_ahead = max_ahead;
	num_connections = num_connections;
	their_recv = their_recv;
	their_sent = their_sent;
	my_sent = my_sent;
#endif
}	// end of Print_Framesync_Values


/***************************************************************************
 * Dump_Packet_Too_Late_Stuff -- Dumps a debug file to disk                *
 *                                                                         *
 * INPUT:                                                                  *
 *		event		ptr to event to print													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/28/1996 BRR : Created.                                             *
 *=========================================================================*/
void Dump_Packet_Too_Late_Stuff(EventClass *event)
{
	FILE *fp;
	int i;

	fp = fopen("toolate.txt", "wt");
	if (!fp) {
		return;
	}
	fprintf(fp,"--------- Event data: -------------------\n");
	fprintf(fp,"Type:       %s\n",EventClass::EventNames[event->Type]);
	fprintf(fp,"Frame:      %d\n",event->Frame);
	fprintf(fp,"ID:         %d\n",event->ID);
	fprintf(fp,"MPlayerID:  %04x\n",event->MPlayerID);

	for (i = 0; i < MPlayerCount; i++) {
		if (event->MPlayerID == MPlayerID[i]) {
			fprintf(fp,"Player's Name: %s",MPlayerNames[i]);
		}
	}

	fprintf(fp,"----------- My data: ------------------\n");
	fprintf(fp,"Frame:%d\n",Frame);
	fprintf(fp,"MaxAhead:%d\n",MPlayerMaxAhead);

	fclose(fp);
}

#endif	//DEMO

/*************************** end of queue.cpp ******************************/