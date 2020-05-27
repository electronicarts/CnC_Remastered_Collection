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
 * How the messages work:																	*
 * - MPlayerMessageList is a gadget list of all current messages				*
 * - MPlayerMessageX & Y are the upper left corner of the 1st message		*
 * - MPlayerMaxMessages is the max # of messages allowed, including			*
 *   the editable message; 0 = no limit.												*
 * - EditLabel points to the textmessage gadget for the current editable	*
 *   field.  EditBuf points to the char buffer being edited.  EditInitPos	*
 *   & EditCurPos define buffer index positions.									*
 * - EditSendAddress is the IPX Address to send the message to when RETURN	*
 *   is pressed.																				*
 *																									*
 * The UserData field in the TextLabelClass tells what the timeout for		*
 * each message is (0 = none).															*
 * When a message's timeout expires, it's deleted.  When a new message		*
 * is added, the top message is deleted if MPlayerMaxMessages is exceeded.	*
 *                                                                         *
 * The Edit-able message is never deleted until ESC or RETURN is pressed.	*
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MSGLIST_H
#define MSGLIST_H

/*
**	Class declaration
*/
class MessageListClass {
	public:
		/*
		**	Constructor/Destructor
		*/
		MessageListClass (void);
		~MessageListClass ();

		/*
		**	Initialization
		*/
		void Init (int x, int y, int max_msg, int maxchars, int height);
		TextLabelClass * Add_Message (char *txt, int color, TextPrintType style, int timeout,
												unsigned short magic_number, unsigned short crc);

		/*
		**	Message-editing routines
		*/
		TextLabelClass * Add_Edit (int color, TextPrintType style, char *to, int width);
		char * Get_Edit_Buf (void);

		/*
		**	Maintenance routines
		*/
		int Manage (void);
		int Input (KeyNumType &input);
		void Draw(void);
		int Num_Messages(void);
		void Set_Width(int width);

	private:
		TextLabelClass * MessageList;				// list of messages
		int MessageX;									// x-coord of upper-left
		int MessageY;									// y-coord of upper-left
		int MaxMessages;								// max messages allowed
		int MaxChars;									// max allowed chars per message
		int Height;										// height in pixels
		TextLabelClass *EditLabel;					// ptr to current edit label
		char *EditBuf;									// ptr to current edit buffer
		int EditCurPos;								// current edit position
		int EditInitPos;								// initial edit position
		int Width;										// Maximum width in pixels of editable string

		/*
		** Static buffers provided for messages.  They must be long enough for
		** both the message, and for the "To" prefix on edited messages, or
		** the "From:" prefix on received messages.
		*/
		static char MessageBuffers[MAX_NUM_MESSAGES][MAX_MESSAGE_LENGTH + 30];
		static char BufferAvail[MAX_NUM_MESSAGES];
};

#endif
