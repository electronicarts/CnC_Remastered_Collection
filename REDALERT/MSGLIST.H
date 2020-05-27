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

/* $Header: /CounterStrike/MSGLIST.H 2     3/04/97 2:53p Joe_bostic $ */
/***************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : MSGLIST.H                                *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 05/22/95                                 *
 *                                                                         *
 *                  Last Update : May 22, 1995 [BRR]                       *
 *                                                                         *
 * Initializing:																				*
 *   Call Init(), giving it the coords of the upper-left corner to display	*
 *   the messages, the coordinates to display the editable message, max # 	*
 *   messages allowed, max # chars per message, and the pixel height of 	*
 *   the font.  MaxChars should be less than or equal to the define 			*
 *   MAX_MESSAGE_LENGTH.																	*
 * 																								*
 *	Displaying a message:																	*
 *   Call Add_Message(); the buffer you pass in is copied into a static		*
 *   buffer in this class.  Each message is given a timeout; after this		*
 *   time, the message is removed from the displayed list.						*
 *   Each message also has a 2-byte ID, which can be used to identify the
 *   sender.
 *																									*
 * Editing a message:																		*
 *   Call Add_Edit(), giving it the color & style to print in.  An 			*
 *   optional "To xxx" prefix is allowed.  The edit message can appear in 	*
 *   the message list itself (it's placed first), or at a separate screen 	*
 *   location, depending on the values passed to Init().							*
 *																									*
 *	Updating messages:																		*
 *   The Input() routine passes new keys to the editable message; this 		*
 *   routine's return code tells the app whether to redraw the messages,	*
 *   and whether to send it the edit message.  As the user types 				*
 *   characters into the edit field, if he overflows the max storage			*
 *   for the edit field, the field is parsed & the first few words 			*
 *   removed; these words are put into the Overflow buffer, and the app		*
 *   is told so; the app should send this buffer across to the destination.*
 *																									*
 * - MessageList is a gadget list of all current messages						*
 * - MessageX & Y are the upper left corner of the 1st message					*
 * - MaxMessages is the max # of messages allowed, including the editable 	*
 *   message; 0 = no limit.																*
 * - MaxChars is the max # of chars allowed per message							*
 * - Height is the pixel height of a single line									*
 * - EditLabel points to the textmessage gadget for the current editable	*
 *   field.  EditBuf points to the char buffer being edited.  EditInitPos	*
 *   & EditCurPos define buffer index positions.									*
 * - EditSendAddress is the IPX Address to send the message to when RETURN	*
 *   is pressed.																				*
 *																									*
 * The low word in the UserData field in the TextLabelClass tells what the *
 * timeout for each message is (0 = none); the high byte of the UserData	*
 * field is used for the text message ID.												*
 * When a message's timeout expires, it's deleted.  When a new message		*
 * is added, the top message is deleted if MPlayerMaxMessages is exceeded.	*
 *                                                                         *
 * The Edit-able message is never deleted until ESC or RETURN is pressed.	*
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MSGLIST_H
#define MSGLIST_H

//***************************************************************************
// Defines
//***************************************************************************
//---------------------------------------------------------------------------
//	Max length of inter-player message buffers.  Messages.Init() should specify
// a value <= this.  For editing messages, the "to" field length is added to
// this length to generate the entire editable message length.  For displayed
// messages, a "From" prefix length should be added to this value to generate
// the entire max displayable message length.
//---------------------------------------------------------------------------
#define	MAX_MESSAGE_LENGTH		120

//---------------------------------------------------------------------------
// Max # of allowed messages at one time
//---------------------------------------------------------------------------
#define	MAX_NUM_MESSAGES			14

//***************************************************************************
// Forward declarations
//***************************************************************************
class TextLabelClass;

//***************************************************************************
// Class declaration
//***************************************************************************
class MessageListClass {
	public:
		//.....................................................................
		// Constructor/Destructor
		//.....................................................................
		MessageListClass (void);
		~MessageListClass ();

		//.....................................................................
		// Initialization
		//.....................................................................
		void Reset(void);
		void Init (int x, int y, int max_msg, int maxchars, int height,
			int edit_x, int edit_y, int overflow_on, int over_start,
			int over_end, int width = 640);
		TextLabelClass * Add_Message (char const * name, int id, char const * txt,
			PlayerColorType color, TextPrintType style, int timeout);
		int Concat_Message (char const * name, int id, char const * txt, int timeout);

		//.....................................................................
		// Message access utility routines
		//.....................................................................
		char * Get_Message (int id);
		TextLabelClass * Get_Label (int id);

		//.....................................................................
		// Message-editing support routines
		//.....................................................................
		TextLabelClass * Add_Edit(PlayerColorType color, TextPrintType style,
			char *to, char cursor = 0, int width = 640);
		void Remove_Edit (void);
		char * Get_Edit_Buf (void);
		char * Get_Overflow_Buf (void) {return (OverflowBuf);}
		void Clear_Overflow_Buf (void) {OverflowBuf[0] = 0;}
		int Is_Edit(void) {return (IsEdit);}
		void Set_Edit_Color(PlayerColorType color);

		//.....................................................................
		// Maintenance routines
		//.....................................................................
		int Manage (void);
		int Input (KeyNumType &input);
		void Draw(void);
		int Num_Messages(void);
		void Set_Width(int width);
		void Set_Edit_Focus(void);
		bool Has_Edit_Focus(void);

	private:

		//.....................................................................
		// Message parsing
		//.....................................................................
		int Trim_Message(char *dest, char *src, int min_chars, int max_chars,
			int scandir);

		//.....................................................................
		// Compute the y-coord of the message list
		//.....................................................................
		void Compute_Y(void);

		//.....................................................................
		// Private Data
		//.....................................................................
		TextLabelClass * MessageList;				// list of messages
		int MessageX;									// x-coord of upper-left
		int MessageY;									// y-coord of upper-left
		int MaxMessages;								// max messages allowed
		int MaxChars;									// max allowed chars per message
		int Height;										// height in pixels

		//.....................................................................
		// Data for the edit field: the edit field will either appear at
		// exact coordinates specified by the application, or it will appear
		// vertically above the other messages.
		//.....................................................................
		unsigned EnableOverflow	: 1;					// 1 = enable overflow feature
		unsigned IsEdit				: 1;					// 1 = there's an edit field
		unsigned AdjustEdit			: 1;					// 1 = edit field appears over msgs
		int EditX;										// x-coord of edit field
		int EditY;										// y-coord of edit field
		TextLabelClass *EditLabel;					// ptr to current edit label
		char EditBuf[MAX_MESSAGE_LENGTH + 30];	// buffer for editable message
		char OverflowBuf[MAX_MESSAGE_LENGTH + 30];	// overflow area
		int EditCurPos;								// current edit position
		int EditInitPos;								// initial edit position
		char CursorChar;								// character to use a cursor
		int OverflowStart;							// 1st index for overflow trimming
		int OverflowEnd;								// last index for overflow trimming
		int Width;										// Maximum width in pixels of editable string

		//.....................................................................
		// Buffers provided for messages.  They must be long enough for
		// both the message, and for the "To" prefix on edited messages, or
		// the "Name:" prefix on received messages.
		//.....................................................................
		char MessageBuffers[MAX_NUM_MESSAGES][MAX_MESSAGE_LENGTH + 30];
		char BufferAvail[MAX_NUM_MESSAGES];
};

#endif
/**************************** end of msglist.h *****************************/

