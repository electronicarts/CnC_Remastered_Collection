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

/* $Header: /CounterStrike/MSGLIST.CPP 2     3/04/97 2:52p Joe_bostic $ */
/***************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : MSGLIST.CPP                              *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 05/22/95                                 *
 *                                                                         *
 *                  Last Update : March 4, 1997 [JLB]                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   MessageListClass::MessageListClass -- constructor                     *
 *   MessageListClass::~MessageListClass -- destructor                     *
 *   MessageListClass::Init -- Inits message system, sets options          *
 *   MessageListClass::Add_Message -- displays the given message           *
 *   MessageListClass::Get_Message -- retrieves given message              *
 *   MessageListClass::Get_Label -- retrieves given text label					*
 *   MessageListClass::Concat_Message -- concats the given message         *
 *   MessageListClass::Add_Edit -- Adds editable string to message list    *
 *   MessageListClass::Remove_Edit -- removes the edit field               *
 *   MessageListClass::Get_Edit_Buf -- gets edit buffer                    *
 *   MessageListClass::Set_Edit_Color -- sets color of edit gizmo          *
 *   MessageListClass::Manage -- Manages multiplayer messages              *
 *   MessageListClass::Input -- Handles input for sending messages         *
 *   MessageListClass::Draw -- Draws the messages                          *
 *   MessageListClass::Num_Messages -- returns # messages in the list      *
 *   MessageListClass::Set_Width -- sets allowable width of messages       *
 *   MessageListClass::Trim_Message -- trims chars off start of message    *
 *   MessageListClass::Compute_Y -- recomputes y-coord for all messages    *
 *   MessageListClass::Reset -- Reset so no messages are visible.          * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


/**************************** Globals **************************************/


/***************************************************************************
 * MessageListClass::MessageListClass -- constructor                       *
 *                                                                         *
 * INPUT:                                                                  *
 *    x,y			coord of upper-left of top message                    	*
 *    max_msg		max messages allowed, including edit message          	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *    none.                                                              	*
 *                                                                         *
 * WARNINGS:                                                               *
 *    none.                                                              	*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/21/1995 BRR : Created.                                             *
 *=========================================================================*/
MessageListClass::MessageListClass(void)
{
	int i;

	//------------------------------------------------------------------------
	// Init all data members
	//------------------------------------------------------------------------
	MessageList = 0;
	MessageX = 0;
	MessageY = 0;
	MaxMessages = 0;
	MaxChars = 0;
	Height = 0;

	EnableOverflow = 0;
	AdjustEdit = 0;
	IsEdit = 0;
	EditX = 0;
	EditY = 0;
	EditLabel = 0;
	EditBuf[0] = 0;
	OverflowBuf[0] = 0;
	EditCurPos = 0;
	EditInitPos = 0;
	CursorChar = 0;
	OverflowStart = 0;
	OverflowEnd = 0;

	for (i = 0; i < MAX_NUM_MESSAGES; i++) {
		BufferAvail[i] = 1;
	}

}	// end of MessageListClass


/***************************************************************************
 * MessageListClass::~MessageListClass -- destructor                       *
 *                                                                         *
 * INPUT:                                                                  *
 *    x,y			coord of upper-left of top message                     	*
 *    max_msg		max messages allowed, including edit message          	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *    none.                                                              	*
 *                                                                         *
 * WARNINGS:                                                               *
 *    none.                                                              	*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/21/1995 BRR : Created.                                             *
 *=========================================================================*/
MessageListClass::~MessageListClass()
{
	Init(0,0,0,0,0,0,0,0,0,0);

}	// end of ~MessageListClass


/***************************************************************************
 * MessageListClass::Init -- Inits message system, sets options            *
 *                                                                         *
 * INPUT:                                                                  *
 *    x,y				coord of upper-left of top message                    *
 *    max_msg			max messages allowed, NOT including edit message      *
 *    maxchars			max # characters allowed per message                	*
 *		height			pixel height of a line of text								*
 *		edit_x			x-coord of edit field; -1 = put at the top of the		*
 *							other messages														*
 *		edit_y			y-coord of edit field; -1 = put at the top of the		*
 *							other messages														*
 *		overflow_on		true = enable the overflow typing feature					*
 *		over_start		start index for overflow processing							*
 *		over_end			end index for overflow processing							*
 *     width          pixel width of message buffer                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *    none.                                                              	*
 *                                                                         *
 * WARNINGS:                                                               *
 *    none.                                                              	*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/21/1995 BRR : Created.                                             *
 *=========================================================================*/
void MessageListClass::Init(int x, int y, int max_msg, int maxchars,
	int height, int edit_x, int edit_y, int overflow_on, int over_start,
	int over_end, int width)
{
	TextLabelClass * txtlabel;
	int i;

	Width = width;

	//------------------------------------------------------------------------
	//	Remove every entry in the list
	//------------------------------------------------------------------------
	txtlabel = MessageList;
	while (txtlabel) {
		MessageList = (TextLabelClass *)txtlabel->Remove();
		delete txtlabel;
		txtlabel = MessageList;
	}

	//------------------------------------------------------------------------
	//	Mark all buffers as available
	//------------------------------------------------------------------------
	for (i = 0; i < MAX_NUM_MESSAGES; i++) {
		BufferAvail[i] = 1;
	}

	//------------------------------------------------------------------------
	//	Remove the editable message
	//------------------------------------------------------------------------
	if (IsEdit) {
		delete EditLabel;
		EditLabel = 0;
	}

	//------------------------------------------------------------------------
	//	Init variables
	//------------------------------------------------------------------------
	MessageList = 0;
	MessageX = x;
	MessageY = y;

	MaxMessages = max_msg;
	if (MaxMessages > MAX_NUM_MESSAGES)
		MaxMessages = MAX_NUM_MESSAGES;

	MaxChars = maxchars;
	if (MaxChars > MAX_MESSAGE_LENGTH)
		MaxChars = MAX_MESSAGE_LENGTH;

	Height = height;

	//------------------------------------------------------------------------
	// Init the edit field variables.  If edit_x or edit_y is -1, place the
	// edit field above the other messages; otherwise, place it at the desired
	// coords.
	//------------------------------------------------------------------------
	EnableOverflow = overflow_on;
	IsEdit = 0;
	if (edit_x == -1 || edit_y == -1) {
		AdjustEdit = 1;
		EditX = x;
		EditY = y;
	}
	else {
		AdjustEdit = 0;
		EditX = edit_x;
		EditY = edit_y;
	}
	EditLabel = 0;
	EditBuf[0] = 0;
	OverflowBuf[0] = 0;
	EditCurPos = 0;
	EditInitPos = 0;
	CursorChar = 0;

	//------------------------------------------------------------------------
	// Init the overflow processing indices
	//------------------------------------------------------------------------
	OverflowStart = over_start;
	OverflowEnd = over_end;
	if (OverflowEnd >= MaxChars) {
		OverflowEnd = MaxChars - 1;
	}
	if (OverflowStart >= OverflowEnd) {
		OverflowStart = OverflowEnd - 1;
	}

}	// end of Init


/*********************************************************************************************** 
 * MessageListClass::Reset -- Reset so no messages are visible.                                * 
 *                                                                                             * 
 *    This routine will reset the message list tracker so that any displayed messages are      * 
 *    cleared.                                                                                 * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   03/04/1997 JLB : Created.                                                                 * 
 *=============================================================================================*/
void MessageListClass::Reset(void)
{
	//------------------------------------------------------------------------
	//	Remove every entry in the list
	//------------------------------------------------------------------------
	TextLabelClass * txtlabel = MessageList;
	while (txtlabel) {
		MessageList = (TextLabelClass *)txtlabel->Remove();
		delete txtlabel;
		txtlabel = MessageList;
	}

	//------------------------------------------------------------------------
	//	Mark all buffers as available
	//------------------------------------------------------------------------
	for (int index = 0; index < MAX_NUM_MESSAGES; index++) {
		BufferAvail[index] = 1;
	}

	//------------------------------------------------------------------------
	//	Remove the editable message
	//------------------------------------------------------------------------
	if (IsEdit) {
		delete EditLabel;
		EditLabel = 0;
	}

	//------------------------------------------------------------------------
	//	Init variables
	//------------------------------------------------------------------------
	MessageList = 0;
	EditLabel = 0;
	IsEdit = 0;
}

extern void On_Message(const char* message, float timeout_seconds, long long message_id);

/***************************************************************************
 * MessageListClass::Add_Message -- displays the given message             *
 *                                                                         *
 * INPUT:                                                                  *
 *		name			name of sender, NULL = none										*
 *		id				numerical ID for this message										*
 *    txt         text to display                                        	*
 *    color       color to draw text in                                		*
 *    style       style to use                                         		*
 *    timeout     # of ticks the thing is supposed to last (-1 = forever)	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *    ptr to new TextLabelClass object.                                  	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/05/1995 BRR : Created.                                             *
 *   10/16/1996 JLB : Audio feedback added.                                *
 *=========================================================================*/
TextLabelClass * MessageListClass::Add_Message(char const * name, int id, char const * txt,
	PlayerColorType color, TextPrintType style, int timeout)
{
	TextLabelClass * txtlabel = NULL;
	char message[MAX_MESSAGE_LENGTH + 30];

	//------------------------------------------------------------------------
	// Combine the name & message text, if there's a name given
	//------------------------------------------------------------------------
	if (name) {
		sprintf(message, "%s:%s", name, txt);
		id = -1;
	} else {
		strcpy(message, txt);
	}

#if (0)

	int i;
	int found;
	char temp[MAX_MESSAGE_LENGTH + 30];
	int print_this_pass;
	char save = 0;
	int mess_start;

	//------------------------------------------------------------------------
	//	Check that printing this wont overrun the width of the print area on screen
	//------------------------------------------------------------------------

	print_this_pass = 0;
	Fancy_Text_Print(TXT_NONE, 0, 0, &ColorRemaps[color], TBLACK, style);
	int wid = String_Pixel_Width(message);
	if (wid >= Width-8) {
		//------------------------------------------------------------------------
		//	Bugger. Its too long. Loop through and find out how many chars we can print
		//------------------------------------------------------------------------
		if (name) {
			sprintf (temp, "%s:", name);
			mess_start = strlen (name)+1;
		} else {
			mess_start = 0;
		}
		for (int i=1 ; i<(int)strlen(txt) ; i++) {
			strncpy (&temp[mess_start], txt, i);
			temp [mess_start + i] = 0;
			wid = String_Pixel_Width(temp);
			if (wid >= Width-8) {
				print_this_pass = mess_start + i-1;
				break;
			}
		}

		//------------------------------------------------------------------------
		//	Prematurely terminate the string so it doesn't all print.
		// We will re-enter at the end to print the rest.
		//------------------------------------------------------------------------
		if (print_this_pass) {
			save = message [print_this_pass];
			message [print_this_pass] = 0;
		}
	}



	//------------------------------------------------------------------------
	//	Remove the top-most message if we're about to exceed the max allowed
	//------------------------------------------------------------------------
	if ( (MaxMessages > 0) && ((Num_Messages() + 1) > MaxMessages)) {
		txtlabel = MessageList;

		if (txtlabel==NULL)
			return(NULL);

		//.....................................................................
		//	Remove this message from the list; mark its buffer as being available.
		//.....................................................................
		MessageList = (TextLabelClass *)txtlabel->Remove();
		for (i = 0; i < MAX_NUM_MESSAGES; i++) {
			if (txtlabel->Text == MessageBuffers[i])
				BufferAvail[i] = 1;
		}
		delete txtlabel;
	}

	//------------------------------------------------------------------------
	//	Create the message
	//------------------------------------------------------------------------
	txtlabel = new TextLabelClass (message, MessageX, MessageY,
		&ColorRemaps[color], style);
	if (timeout==-1) {
		txtlabel->UserData1 = 0;
	}
	else {
		txtlabel->UserData1 = TickCount + timeout;
	}
	txtlabel->UserData2 = id;

	//------------------------------------------------------------------------
	//	Find a buffer to store our message in; if there are none, don't add the
	//	message.
	//------------------------------------------------------------------------
	found = 0;
	for (i = 0; i < MAX_NUM_MESSAGES; i++) {
		if (BufferAvail[i]) {
			BufferAvail[i] = 0;
			memset (MessageBuffers[i],0,MAX_MESSAGE_LENGTH + 30);
			strcpy (MessageBuffers[i],message);
			txtlabel->Text = MessageBuffers[i];
			found = 1;
			break;
		}
	}
	if (!found) {
		delete txtlabel;
		return (NULL);
	}

#endif

	On_Message(message, timeout * 60.0f / TICKS_PER_MINUTE, id);
	//Sound_Effect(VOC_INCOMING_MESSAGE);

#if (0)

	//------------------------------------------------------------------------
	//	Attach the message to our list
	//------------------------------------------------------------------------
	if (MessageList) {
		txtlabel->Add_Tail (*MessageList);
	}
	else {
		MessageList = txtlabel;
	}

	//------------------------------------------------------------------------
	//	Recompute all messages' y-coordinate values
	//------------------------------------------------------------------------
	Compute_Y();

	//------------------------------------------------------------------------
	//	If we terminated the string before the end then we need to reenter to
	// add a new message with the rest of the string.
	//------------------------------------------------------------------------
	if (save) {
		message [print_this_pass] = save;
		Add_Message (name, id, &message [print_this_pass], color, style, timeout);
	}

#endif

	return(txtlabel);

}	// end of Add_Message


/***************************************************************************
 * MessageListClass::Get_Message -- retrieves given message                *
 *                                                                         *
 * INPUT:                                                                  *
 *		id			ID of message to get														*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ptr to message text, NULL if not found											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/07/1995 BRR : Created.                                             *
 *=========================================================================*/
char * MessageListClass::Get_Message(int id)
{
	TextLabelClass * gadg;

	//------------------------------------------------------------------------
	// Scan the message list, searching for the given ID
	//------------------------------------------------------------------------
	if (MessageList) {
		gadg = MessageList;
		while (gadg) {
			if (gadg->UserData2 == id) {
				return (gadg->Text);
			}
			gadg = (TextLabelClass *)gadg->Get_Next();
		}
	}

	return (NULL);

}	// end of Get_Message


/***************************************************************************
 * MessageListClass::Get_Label -- retrieves given text label					*
 *                                                                         *
 * INPUT:                                                                  *
 *		id			ID of message to get														*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ptr to message text, NULL if not found											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/07/1995 BRR : Created.                                             *
 *=========================================================================*/
TextLabelClass * MessageListClass::Get_Label(int id)
{
	TextLabelClass * gadg;

	//------------------------------------------------------------------------
	// Scan the message list, searching for the given ID
	//------------------------------------------------------------------------
	if (MessageList) {
		gadg = MessageList;
		while (gadg) {
			if (gadg->UserData2 == id) {
				return (gadg);
			}
			gadg = (TextLabelClass *)gadg->Get_Next();
		}
	}

	return (NULL);

}	// end of Get_Label


/***************************************************************************
 * MessageListClass::Concat_Message -- concats the given message           *
 *                                                                         *
 * INPUT:                                                                  *
 *		name			name of sender; NULL = none										*
 *		id				ID of message to concatenate to									*
 *		txt			text to concatenate onto existing message						*
 *		timeout		new timeout for message												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error (id or name not found)										*
 *                                                                         *
 * WARNINGS:                                                               *
 *		If the required message doesn't exist, this routine does nothing.		*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/07/1995 BRR : Created.                                             *
 *=========================================================================*/
int MessageListClass::Concat_Message(char const * name, int id, char const * txt, int timeout)
{
	int min_chars;
	int max_chars;
	char * msg;
	TextLabelClass * tlabel;
	int found;

	//------------------------------------------------------------------------
	// If no name is given, or the concatenation feature is turned off,
	// don't concatenate the message
	//------------------------------------------------------------------------
	if (!name || !EnableOverflow) {
		return (0);
	}

	//------------------------------------------------------------------------
	// Scan through all active messages, searching for one with a matching
	// name & ID
	//------------------------------------------------------------------------
	found = 0;
	if (MessageList) {
		tlabel = MessageList;
		while (tlabel) {
			if (tlabel->UserData2 == id &&
				!memcmp(tlabel->Text,name,strlen(name))) {
				found = 1;
				break;
			}
			tlabel = (TextLabelClass *)tlabel->Get_Next();
		}
	}

	//------------------------------------------------------------------------
	// name and ID not found; return
	//------------------------------------------------------------------------
	if (!found) {
		return (0);
	}

	//------------------------------------------------------------------------
	// set a pointer to the text string, plus the name and colon
	//------------------------------------------------------------------------
	msg = tlabel->Text + strlen(name) + 1;

	//------------------------------------------------------------------------
	// If there's room enough in the message, just add the given string
	//------------------------------------------------------------------------
	if ( (int)(strlen(msg) + strlen(txt)) < MaxChars) {

		//---------------------------------------------------------------------
		// We need to trim the message if there is no room to draw it
		//---------------------------------------------------------------------
		char *concat_test = new char [MaxChars+1];
		Fancy_Text_Print(TXT_NONE, 0, 0, tlabel->Color, TBLACK, tlabel->Style);
		int name_width = String_Pixel_Width(tlabel->Text) - String_Pixel_Width(msg);
		int width;

		strcpy (concat_test, msg);
		strcat (concat_test, txt);
		width = String_Pixel_Width(concat_test) + name_width;
		min_chars = 10;

		while (width >= Width-8){

			max_chars = strlen (msg);
			if (max_chars < min_chars) {
				max_chars = min_chars;
			}

			Trim_Message (NULL, msg, min_chars, max_chars, 0);

			strcpy (concat_test, msg);
			strcat (concat_test, txt);

			width = String_Pixel_Width(concat_test) + name_width;
		};

		delete [] concat_test;

		strcat (msg,txt);
	}

	//------------------------------------------------------------------------
	// Otherwise, trim off some characters from the beginning of the
	// message.  Trim off at least enough to leave room for the new text.
	// Trim from left to right to remove the minimum required text.
	//------------------------------------------------------------------------
	else {
		min_chars = (strlen(msg) + strlen(txt)) - MaxChars;
		max_chars = strlen(msg);
		if (max_chars < min_chars) {
			max_chars = min_chars;
		}
		Trim_Message (NULL, msg, min_chars, max_chars, 0);
		strcat (msg, txt);
	}

	//------------------------------------------------------------------------
	// Set the new timeout value for the message
	//------------------------------------------------------------------------
	if (timeout==-1) {
		tlabel->UserData1 = 0;
	}
	else {
		tlabel->UserData1 = TickCount + timeout;
	}

	return (1);

}	// end of Concat_Message



/***********************************************************************************************
 * MessageListClass::Set_Edit_Focus -- Give the gadget system focus to the edit box            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/19/96 4:41PM ST : Created                                                             *
 *=============================================================================================*/
void MessageListClass::Set_Edit_Focus (void)
{
	if (IsEdit) EditLabel->Set_Focus();
}


/***********************************************************************************************
 * MessageListClass::Has_Edit_Focus -- Find out if the edit box has the input focus            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/19/96 4:41PM ST : Created                                                             *
 *=============================================================================================*/
bool MessageListClass::Has_Edit_Focus (void)
{
	if (IsEdit){
		return (EditLabel->Has_Focus());
	}else{
		return(false);
	}
}



/***************************************************************************
 * MessageListClass::Add_Edit -- Adds editable string to message list      *
 *                                                                         *
 * INPUT:                                                                  *
 *    color			color of edit message                                		*
 *    style			style of edit message                                		*
 *    to				string: who to send to; NULL = none								*
 *		cursor		character to use as a cursor; 0 = none							*
 *                                                                         *
 * OUTPUT:                                                                 *
 *    ptr to new TextLabelClass                                          	*
 *                                                                         *
 * WARNINGS:                                                               *
 *    none.                                                              	*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/22/1995 BRR : Created.                                             *
 *=========================================================================*/
TextLabelClass * MessageListClass::Add_Edit(PlayerColorType color,
	TextPrintType style, char * to, char cursor, int width)
{
	int i;
	TextLabelClass * txtlabel;

	//------------------------------------------------------------------------
	//	Do nothing if we're already in "edit" mode
	//------------------------------------------------------------------------
	if (IsEdit) {
		EditLabel->Set_Focus();
		return(NULL);
	}

	//------------------------------------------------------------------------
	//	Remove the top-most message if we're about to exceed the max allowed
	//------------------------------------------------------------------------
	if (AdjustEdit && ((Num_Messages() + 1) > MaxMessages)) {
		txtlabel = MessageList;
		MessageList = (TextLabelClass *)txtlabel->Remove();
		for (i = 0; i < MAX_NUM_MESSAGES; i++) {
			if (txtlabel->Text == MessageBuffers[i])
				BufferAvail[i] = 1;
		}
		delete txtlabel;
	}

	//------------------------------------------------------------------------
	// If no 'to' field was passed in, ignore it
	//------------------------------------------------------------------------
	if (!to) {
		to = "";
	}

	//------------------------------------------------------------------------
	// Set the cursor character
	//------------------------------------------------------------------------
	CursorChar = cursor;

	//------------------------------------------------------------------------
	//	Initialize the buffer positions; create a new text label object
	//------------------------------------------------------------------------
	memset (EditBuf, 0, sizeof(EditBuf));
	strcpy (EditBuf, to);
	OverflowBuf[0] = 0;
	EditCurPos = EditInitPos = strlen(to);
	EditLabel = new TextLabelClass (EditBuf, EditX, EditY,
		&ColorRemaps[color], style);

	Width = width;

	if (EditLabel) {
		IsEdit = 1;
		EditLabel->Set_Focus();
	}
	else {
		IsEdit = 0;
	}

	//------------------------------------------------------------------------
	// If the edit field appears over the message list, recompute the y-value
	// for all messages.  Also, adjust MaxMessages down by one, since there
	// is now one less slot available.
	//------------------------------------------------------------------------
	if (AdjustEdit) {
		Compute_Y();
		MaxMessages--;
	}

	return(EditLabel);

}	// end of Add_Edit


/***************************************************************************
 * MessageListClass::Remove_Edit -- removes the edit field                 *
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
 *   11/06/1995 BRR : Created.                                             *
 *=========================================================================*/
void MessageListClass::Remove_Edit(void)
{
	//------------------------------------------------------------------------
	// If the edit field is active, delete it
	//------------------------------------------------------------------------
	if (IsEdit) {
		IsEdit = 0;
		delete EditLabel;

		//.....................................................................
		// If the edit field appears over the message list, recompute the
		// y-value for all messages.  Adjust MaxMessages back up, since there
		// is now a new available slot.
		//.....................................................................
		if (AdjustEdit) {
			Compute_Y();
			MaxMessages++;
		}
	}

}	// end if Remove_Edit


/***************************************************************************
 * MessageListClass::Get_Edit_Buf -- gets edit buffer                      *
 *                                                                         *
 * INPUT:                                                                  *
 *    none.                                                              	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *    ptr to edit buffer, minus the "To:" header                         	*
 *                                                                         *
 * WARNINGS:                                                               *
 *    none.                                                              	*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/21/1995 BRR : Created.                                             *
 *=========================================================================*/
char * MessageListClass::Get_Edit_Buf(void)
{
	return(EditBuf + EditInitPos);

}	// end of Get_Edit_Buf


/***************************************************************************
 * MessageListClass::Set_Edit_Color -- sets color of edit gizmo            *
 *                                                                         *
 * INPUT:                                                                  *
 *		color		color to set edit label to												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/08/1995 BRR : Created.                                             *
 *=========================================================================*/
void MessageListClass::Set_Edit_Color(PlayerColorType color)
{
	if (IsEdit) {
		EditLabel->Color = &ColorRemaps[color];
	}

}	// end of Set_Edit_Color


/***************************************************************************
 * MessageListClass::Manage -- Manages multiplayer messages                *
 *                                                                         *
 * If this routine returns TRUE, the caller should update the display.     *
 *                                                                         *
 * INPUT:                                                                  *
 *    none.                                                              	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *    none.                                                              	*
 *                                                                         *
 * WARNINGS:                                                               *
 *    0 = no change has occurred, 1 = changed                            	*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/05/1995 BRR : Created.                                             *
 *=========================================================================*/
int MessageListClass::Manage (void)
{
	TextLabelClass * txtlabel;
	TextLabelClass * next;
	int changed = 0;
	int i;

	//------------------------------------------------------------------------
	//	Loop through all messages
	//------------------------------------------------------------------------
	txtlabel = MessageList;
	while (txtlabel) {

		//.....................................................................
		//	If this message's time is up, remove it from the list
		//.....................................................................
		if (txtlabel->UserData1 != 0 && TickCount > txtlabel->UserData1) {

			//..................................................................
			//	Save the next ptr in the list; remove this entry
			//..................................................................
			next = (TextLabelClass *)txtlabel->Get_Next();
			MessageList = (TextLabelClass *)txtlabel->Remove();
			for (i = 0; i < MAX_NUM_MESSAGES; i++) {
				if (txtlabel->Text == MessageBuffers[i]) {
					BufferAvail[i] = 1;
				}
			}
			delete txtlabel;
			changed = 1;
			txtlabel = next;
		}
		else {
			txtlabel = (TextLabelClass *)txtlabel->Get_Next();
		}
	}

	//------------------------------------------------------------------------
	//	If a changed has been made, recompute the y-coord of all messages
	//------------------------------------------------------------------------
	if (changed) {
		Compute_Y();
	}

	return(changed);

}	// end of Manage


/***************************************************************************
 * MessageListClass::Input -- Handles input for sending messages           *
 *                                                                         *
 * INPUT:                                                                  *
 *    input         key value to process                                 	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *    1 = caller should redraw the message list (no need to complete     	*
 *        refresh, though)                                                 *
 *    2 = caller should completely refresh the display.                  	*
 *    3 = caller should send the edit message.                           	*
 *        (sets 'input' to 0 if it processes it.)                          *
 *		4 = caller should send the Overflow buffer									*
 *                                                                         *
 * WARNINGS:                                                               *
 *    none.                                                              	*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/05/1995 BRR : Created.                                             *
 *=========================================================================*/
int MessageListClass::Input(KeyNumType &input)
{
	KeyASCIIType ascii;
	int retcode = 0;
	int numchars;

	//------------------------------------------------------------------------
	//	Do nothing if nothing to do.
	//------------------------------------------------------------------------
	if (input == KN_NONE) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	Leave mouse events alone.
	//------------------------------------------------------------------------
	if ( (input & (~KN_RLSE_BIT))==KN_LMOUSE ||
		(input & (~KN_RLSE_BIT))==KN_RMOUSE) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	If we're in 'edit mode', handle keys
	//------------------------------------------------------------------------
	if (IsEdit) {


		ascii = (KeyASCIIType)(Keyboard->To_ASCII(input) & 0x00ff);

#ifdef WIN32
		/*
		** Allow numeric keypad presses to map to ascii numbers
		*/
		if ((input & WWKEY_VK_BIT) && ascii >='0' && ascii <= '9') {

			input = (KeyNumType)(input & ~WWKEY_VK_BIT);

		} else {
			/*
			** Filter out all special keys except return, escape and backspace
			*/
			if ((!(input & WWKEY_VK_BIT) && !(input & KN_BUTTON)
					&& ascii >= ' ' && ascii <= 127)
				|| (input & 0xff)== (KN_RETURN & 0xff)
				|| (input & 0xff)== (KN_BACKSPACE & 0xff)
				|| (input & 0xff)== (KN_ESC & 0xff) ) {

				//ascii = (KeyASCIIType)(Keyboard->To_ASCII(input));
			} else {
				input = KN_NONE;
				return (0);
			}
		}
#endif	//WIN32



		switch (ascii) {
			//..................................................................
			//	ESC = abort message
			//..................................................................
			case KA_ESC & 0xff:
				Remove_Edit();
				retcode = 2;
				input = KN_NONE;
				break;

			//..................................................................
			//	RETURN = send the message.
			// Add a space to the end, in case another message gets concatenated
			// onto this one after we send it; then, they won't be mushed
			// together.
			//..................................................................
			case KA_RETURN & 0xff:
				if (EditCurPos == EditInitPos) {
					retcode = 0;
					input = KN_NONE;
					break;
				}
				if ( (EditCurPos - EditInitPos) < (MaxChars - 1) ) {
					EditBuf[EditCurPos] = ' ';
					EditCurPos++;
					EditBuf[EditCurPos] = 0;
				}
				Remove_Edit();
				retcode = 3;
				input = KN_NONE;
				break;

			//..................................................................
			//	BACKSPACE = remove a character
			//..................................................................
			case KA_BACKSPACE & 0xff:
				if (EditCurPos > EditInitPos) {
					EditCurPos--;
					EditBuf[EditCurPos] = 0;
					retcode = 2;
				}
				input = KN_NONE;
				EditLabel->Set_Focus();
				break;

			//..................................................................
			// default: add a character.  Reserve the last buffer position for
			// null.  (EditCurPos - EditInitPos) is the buffer index # of the
			// next character, after the "To:" prefix.
			//..................................................................
			default:
				EditLabel->Set_Focus();
				bool overflowed = false;
				if (ascii >= ' ' && ascii <= 127) {
					if ( (EditCurPos - EditInitPos) < (MaxChars - 1) ) {

						EditBuf[EditCurPos] = ascii;
						EditCurPos++;
						EditBuf[EditCurPos] = 0;
						retcode = 1;

						/*
						** Verify that the additional character would not overrun the on screen edit box.
						*/
						Fancy_Text_Print(TXT_NONE, 0, 0, EditLabel->Color, TBLACK, EditLabel->Style);
						int width = String_Pixel_Width(EditBuf);
						if (width >= Width-10) {
							overflowed = true;
							EditCurPos--;
							EditBuf[EditCurPos] = 0;
							retcode = 0;
						}
					} else {
					//............................................................
					// If there's no room in the buffer, and overflow is enabled,
					// trim the extra characters off (from right to left, to
					// remove the max possible characters), and then add the new
					// character in.
					//............................................................
						overflowed = true;
					}

					if (/*BGEnableOverflow &&*/ overflowed) {
						numchars = Trim_Message (OverflowBuf, EditBuf + EditInitPos,
							OverflowStart,OverflowEnd, 1);
						EditCurPos -= numchars;
						EditBuf[EditCurPos] = ascii;
						EditCurPos++;
						EditBuf[EditCurPos] = 0;
						retcode = 4;
					}
				}
				input = KN_NONE;
				break;
		}
	}

	return(retcode);

}	// end of Input


/***************************************************************************
 * MessageListClass::Draw -- draws messages                                *
 *                                                                         *
 * INPUT:                                                                  *
 *    none                                                               	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *    none.                                                              	*
 *                                                                         *
 * WARNINGS:                                                               *
 *    none.                                                              	*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/22/1995 BRR : Created.                                             *
 *=========================================================================*/
void MessageListClass::Draw(void)
{
	char txt[2] = {0,0};

	if (IsEdit) {
		if (LogicPage == &SeenBuff) {
			Hide_Mouse();
		}
		EditLabel->Draw_Me(true);

		if (CursorChar && (EditCurPos - EditInitPos) < (MaxChars - 1) && EditLabel->Has_Focus()) {
			txt[0] = CursorChar;
			Fancy_Text_Print(txt,
				EditLabel->X + String_Pixel_Width(EditLabel->Text),
				EditLabel->Y,
				EditLabel->Color,
				TBLACK,
				EditLabel->Style);
		}

		if (LogicPage == &SeenBuff) {
			Show_Mouse();
		}
	}
	if (MessageList) {
		if (LogicPage == &SeenBuff) {
			Hide_Mouse();
		}
		MessageList->Draw_All();
		if (LogicPage == &SeenBuff) {
			Show_Mouse();
		}
	}

}	// end of Draw


/***************************************************************************
 * MessageListClass::Num_Messages -- returns # messages in the list        *
 *                                                                         *
 * INPUT:                                                                  *
 *    none.                                                              	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *    # of messages, including the edit field if it's above the messages	*
 *                                                                         *
 * WARNINGS:                                                               *
 *    none.                                                              	*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/26/1995 BRR : Created.                                             *
 *=========================================================================*/
int MessageListClass::Num_Messages(void)
{
	GadgetClass * gadg;
	int num;

	num = 0;

	if (MessageList) {
		gadg = MessageList;
		while (gadg) {
			num++;
			gadg = gadg->Get_Next();
		}
	}

	if (IsEdit && AdjustEdit) {
		num++;
	}

	return (num);

}	// end of Num_Messages


/***************************************************************************
 * MessageListClass::Set_Width -- sets allowable width of messages         *
 *                                                                         *
 * INPUT:                                                                  *
 *    width      pixel width                                             	*
 *                                                                         *
 * OUTPUT:                                                                 *
 *    none.                                                              	*
 *                                                                         *
 * WARNINGS:                                                               *
 *    none.                                                              	*
 *                                                                         *
 * HISTORY:                                                                *
 *   06/26/1995 BRR : Created.                                             *
 *=========================================================================*/
void MessageListClass::Set_Width(int width)
{
	GadgetClass * gadg;

	if (MessageList) {
		gadg = MessageList;
		while (gadg) {
			((TextLabelClass *)gadg)->PixWidth = width;
			gadg = gadg->Get_Next();
		}
	}

	if (IsEdit) {
		EditLabel->PixWidth = width;
	}

}	// end of Set_Width


/***************************************************************************
 * MessageListClass::Trim_Message -- trims chars off start of message      *
 *                                                                         *
 * INPUT:                                                                  *
 *		dest				buffer to store removed characters in; NULL = none		*
 *		src				text buffer to trim												*
 *		min_chars		min # chars that must be trimmed off						*
 *		max_chars		max # chars allowed to trim									*
 *		scandir			0 = left-to-right, 1 = right-to-left						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		# characters removed																	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/07/1995 BRR : Created.                                             *
 *=========================================================================*/
int MessageListClass::Trim_Message(char * dest, char * src, int min_chars,
	int max_chars, int scandir)
{
	int i;
	int len;
	int found;

	//------------------------------------------------------------------------
	// validate parameters
	//------------------------------------------------------------------------
	if (min_chars <= 0) {
		return(0);
	}

	len = strlen (src);
	if (max_chars > len) {
		max_chars = len;
	}

	//------------------------------------------------------------------------
	// find 1st available white space; if there is none, just trim off
	// 'min_chars' characters.  'i' will be the number of chars to trim.
	// The chars removed will include the white space.
	//------------------------------------------------------------------------
	found = 0;
	//........................................................................
	// scan from left to right
	//........................................................................
	if (scandir == 0) {
		for (i = min_chars; i <= max_chars; i++) {
			if (isspace(src[i - 1])) {
				found = 1;
				break;
			}
		}
	}
	//........................................................................
	// scan from right to left
	//........................................................................
	else {
		for (i = max_chars; i >= min_chars; i--) {
			if (isspace(src[i - 1])) {
				found = 1;
				break;
			}
		}
	}
	//........................................................................
	// If no whitespace was found, just set 'i' to the min # characters
	//........................................................................
	if (!found) {
		i = min_chars;
	}

	//------------------------------------------------------------------------
	// Save trimmed characters in the dest buffer, if there is one
	//------------------------------------------------------------------------
	if (dest) {
		memcpy (dest, src, i);
		dest[i] ='\0';
	}

	//------------------------------------------------------------------------
	// Shift characters over in the source buffer
	//------------------------------------------------------------------------
	memmove (src, src + i, len - i + 1);

	return (i);

}	// end of Trim_Message


/***************************************************************************
 * MessageListClass::Compute_Y -- recomputes y-coord for all messages      *
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
 *   11/07/1995 BRR : Created.                                             *
 *=========================================================================*/
void MessageListClass::Compute_Y(void)
{
	GadgetClass * gadg;
	int y;

	//------------------------------------------------------------------------
	// If the editable message is attached to the message list, 'AdjustEdit'
	// will be set; so, adjust all y-values downward one line.  Otherwise,
	// the editable message has its own screen coordinates.
	//------------------------------------------------------------------------
	if (IsEdit && AdjustEdit) {
		y = MessageY + Height;
	}
	else {
		y = MessageY;
	}
	if (MessageList) {
		gadg = MessageList;
		while (gadg) {
			gadg->Y = y;
			gadg = gadg->Get_Next();
			y += Height;
		}
	}

}	// end of Compute_Y


/*************************** end of msglist.cpp ****************************/
