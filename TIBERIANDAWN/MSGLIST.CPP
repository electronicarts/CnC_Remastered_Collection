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

/* $Header:   F:\projects\c&c\vcs\code\msglist.cpv   1.4   16 Oct 1995 16:48:20   JOE_BOSTIC  $ */
/***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : MSGLIST.CPP                              *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 05/22/95                                 *
 *                                                                         *
 *                  Last Update : June 26, 1995 [BRR]                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   MessageListClass::Add_Edit -- Adds editable string to message list    *
 *   MessageListClass::Add_Message -- displays the given message           *
 *   MessageListClass::Draw -- Draws the messages                          *
 *   MessageListClass::Get_Edit_Buf -- gets edit buffer                    *
 *   MessageListClass::Init -- Inits message system, sets options          *
 *   MessageListClass::Input -- Handles input for sending messages         *
 *   MessageListClass::Manage -- Manages multiplayer messages              *
 *   MessageListClass::MessageListClass -- constructor                     *
 *   MessageListClass::~MessageListClass -- destructor                     *
 *   MessageListClass::Num_Messages -- returns # messages in the list      *
 *   MessageListClass::Set_Width -- sets allowable width of messages       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

// ST = 12/17/2018 5:44PM
#ifndef TickCount
extern TimerClass					TickCount;
#endif

char MessageListClass::MessageBuffers[MAX_NUM_MESSAGES][MAX_MESSAGE_LENGTH + 30];
char MessageListClass::BufferAvail[MAX_NUM_MESSAGES];

/***************************************************************************
 * MessageListClass::MessageListClass -- constructor                       *
 *                                                                         *
 * INPUT:                                                                  *
 *      x,y         coord of upper-left of top message                     *
 *      max_msg      max messages allowed, including edit message          *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/21/1995 BRR : Created.                                             *
 *=========================================================================*/
MessageListClass::MessageListClass(void)
{
	int i;

	MessageList = 0;
	MessageX = 0;
	MessageY = 0;
	MaxMessages = 0;
	MaxChars = 0;
	Height = 0;
	EditLabel = 0;
	EditBuf = 0;
	EditCurPos = 0;
	EditInitPos = 0;

	for (i = 0; i < MAX_NUM_MESSAGES; i++) {
		BufferAvail[i] = 1;
	}

}


/***************************************************************************
 * MessageListClass::~MessageListClass -- destructor                       *
 *                                                                         *
 * INPUT:                                                                  *
 *      x,y         coord of upper-left of top message                     *
 *      max_msg      max messages allowed, including edit message          *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/21/1995 BRR : Created.                                             *
 *=========================================================================*/
MessageListClass::~MessageListClass()
{
	Init(0,0,0,0,0);
}


/***************************************************************************
 * MessageListClass::Init -- Inits message system, sets options            *
 *                                                                         *
 * INPUT:                                                                  *
 *      x,y         coord of upper-left of top message                     *
 *      max_msg      max messages allowed, including edit message          *
 *      maxchars      max # characters allowed per message                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/21/1995 BRR : Created.                                             *
 *=========================================================================*/
void MessageListClass::Init(int x, int y, int max_msg, int maxchars, int height)
{
	TextLabelClass *txtlabel;
	int i;

	/*------------------------------------------------------------------------
	Remove every entry in the list
	------------------------------------------------------------------------*/
	txtlabel = MessageList;
	while (txtlabel) {
		MessageList = (TextLabelClass *)txtlabel->Remove();
		delete txtlabel;
		txtlabel = MessageList;
	}

	/*------------------------------------------------------------------------
	Mark all buffers as available
	------------------------------------------------------------------------*/
	for (i = 0; i < MAX_NUM_MESSAGES; i++) {
		BufferAvail[i] = 1;
	}

	/*------------------------------------------------------------------------
	Init variables
	------------------------------------------------------------------------*/
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
	EditLabel = 0;
	EditBuf = 0;
	EditCurPos = 0;
	EditInitPos = 0;
}


/***************************************************************************
 * MessageListClass::Add_Message -- displays the given message             *
 *                                                                         *
 * INPUT:                                                                  *
 *      txt         text to display                                        *
 *      color         color to draw text in                                *
 *      style         style to use                                         *
 *      timeout      # of ticks the thing is supposed to last (-1 = forever)*
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ptr to new TextLabelClass object.                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 * The TextLabelClass's text buffer is free'd when the class is free'd,    *
 * so never pass it a static buffer.                                       *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/05/1995 BRR : Created.                                             *
 *=========================================================================*/
TextLabelClass * MessageListClass::Add_Message(char *txt, int color,
	TextPrintType style, int timeout, unsigned short magic_number, unsigned short crc)
{
	int num_msg;
	TextLabelClass *txtlabel;
	int x,y;
	GadgetClass *gadg;
	int i,j;
	int found;
	int	position;
	char	*raw_string;
	char	*current_string;
	char	*s1,*s2;
	bool	same;
#if (0)
#if (GERMAN)
			static int from_adjust = -1;
#else
#if (FRENCH)
			static int from_adjust = -2;
#else
			static int from_adjust = 0;
#endif
#endif
#endif	//(0)

	/*------------------------------------------------------------------------
	Prevent a duplicate message.  (The IPXManager Global Channel cannot detect
	a resend of a packet, so sometimes two identical messages appear in a row.)
	------------------------------------------------------------------------*/
	if (MessageList) {
		txtlabel = MessageList;
		while (txtlabel) {
			/*
			** Dont check for duplicates in multi-segment strings
			*/
			if (!txtlabel->Segments){
				if (!strcmp (txtlabel->Text,txt) && txtlabel->Color == color &&
					txtlabel->Style == style) {
					return(txtlabel);
				}
			}
			txtlabel = (TextLabelClass *)txtlabel->Get_Next();
		}
	}


	/*
	** If the magic number is a valid message tail then see if we
	** can add this message to the tail of an existing message (crc's must also match)
	*/
	if (magic_number > MESSAGE_HEAD_MAGIC_NUMBER &&
	    magic_number < MESSAGE_HEAD_MAGIC_NUMBER+MAX_MESSAGE_SEGMENTS){

		position = magic_number - MESSAGE_HEAD_MAGIC_NUMBER;
		txtlabel = MessageList;

		while (txtlabel) {
			if (txtlabel->Color == color && txtlabel->Style == style && txtlabel->CRC == crc) {

				same = true;

				s1 = strchr(txtlabel->Text, ':');
				s2 = strchr(txt, ':');

				if (s1 && s2){
					*s1 = 0;
					*s2 = 0;

					same = !strcmp (txtlabel->Text, txt);

					*s1 = ':';
					*s2 = ':';
				}

				if (same){

					/*
					** If this message segment hasnt already come through then add it to the existing text
					*/
					if (! (txtlabel->Segments & (1 << position)) ) {
						/*
						** Search for the ':' to find the actual message after the players name
						*/
						raw_string = s2;
						current_string = s1;
						if (raw_string++ && current_string++){
							memcpy (current_string + (position*(COMPAT_MESSAGE_LENGTH-5))/*+from_adjust*/, raw_string, COMPAT_MESSAGE_LENGTH-4);
							/*
							** Flag this string segment as complete
							*/
							txtlabel->Segments |= 1<<position;
							return(txtlabel);
						}
					}else{
						/*
						** This segment has already come through for this string so discard it.
						*/
						return (txtlabel);
					}
				}
			}
			txtlabel = (TextLabelClass *)txtlabel->Get_Next();
		}
	}



	/*------------------------------------------------------------------------
	Count the # of messages; if MaxMessages is going to be exceeded, remove
	the top-most message.
	------------------------------------------------------------------------*/
	num_msg = 0;
	if (MessageList) {
		gadg = MessageList;
		while (gadg) {
			num_msg++;
			gadg = gadg->Get_Next();
		}
	}
	/*........................................................................
	Remove the top-most message, but don't remove the edit message.
	........................................................................*/
	if ( (MaxMessages > 0) && ((num_msg + 1) > MaxMessages)) {
		txtlabel = MessageList;
		/*.....................................................................
		If the top label is the edit label, go to the next one; if there is
		no next one, just return.
		.....................................................................*/
		if (txtlabel == EditLabel)
			txtlabel = (TextLabelClass *)txtlabel->Get_Next();
		if (txtlabel==NULL)
			return(NULL);

		/*.....................................................................
		Remove this message from the list; mark its buffer as being available.
		.....................................................................*/
		MessageList = (TextLabelClass *)txtlabel->Remove();
		for (i = 0; i < MAX_NUM_MESSAGES; i++) {
			if (txtlabel->Text == MessageBuffers[i])
				BufferAvail[i] = 1;
		}
		delete txtlabel;

		/*.....................................................................
		Recompute everyone's y-coordinate
		.....................................................................*/
		y = MessageY;
		if (MessageList) {
			gadg = MessageList;
			while (gadg) {
				gadg->Y = y;
				gadg = gadg->Get_Next();
				y += Height;
			}
		}
	}

	/*------------------------------------------------------------------------
	Figure out the message's y-coordinate; put it below the other messages
	------------------------------------------------------------------------*/
	x = MessageX;
	y = MessageY;
	if (MessageList) {
		gadg = MessageList;
		while (gadg) {
			gadg = gadg->Get_Next();
			y += Height;
		}
	}

	/*------------------------------------------------------------------------
	Create the message
	------------------------------------------------------------------------*/
	txtlabel = new TextLabelClass (txt, x, y, color, style);
	if (timeout==-1) {
		txtlabel->UserData = 0;
	} else {
		txtlabel->UserData = TickCount.Time() + timeout;
	}

	/*------------------------------------------------------------------------
	Find a buffer to store our message in; if there are none, don't add the
	message.
	------------------------------------------------------------------------*/
	found = 0;
	txtlabel->Segments = 0;
	txtlabel->CRC = crc;

	for (i = 0; i < MAX_NUM_MESSAGES; i++) {
		if (BufferAvail[i]) {
			BufferAvail[i] = 0;
			memset (MessageBuffers[i],0,MAX_MESSAGE_LENGTH + 30);
			strcpy (MessageBuffers[i],txt);

			/*
			** If this is a segment from a larger message then put it in the right place
			** in the buffer and clear out the rest with spaces
			*/
			if (magic_number >= MESSAGE_HEAD_MAGIC_NUMBER &&
			    magic_number < MESSAGE_HEAD_MAGIC_NUMBER+MAX_MESSAGE_SEGMENTS){
				raw_string = strchr(txt, ':');
				char *dest_str = strchr(MessageBuffers[i], ':');
				if (dest_str){
					dest_str++;
				}else{
					dest_str = MessageBuffers[i];
				}

				if (raw_string++){
					for (j=0 ; j<3 ; j++){
						if (! ((magic_number - j) == MESSAGE_HEAD_MAGIC_NUMBER)){
							memset (dest_str + j*(COMPAT_MESSAGE_LENGTH-4)/*+from_adjust*/, 32, COMPAT_MESSAGE_LENGTH-4);
						}else{
							strcpy(dest_str + j*(COMPAT_MESSAGE_LENGTH-4)/*+from_adjust*/, raw_string);
						}
					}
				*(dest_str +((COMPAT_MESSAGE_LENGTH-4)*MAX_MESSAGE_SEGMENTS-1)) = 0;

				}
				position = magic_number - MESSAGE_HEAD_MAGIC_NUMBER;
				txtlabel->Segments = 1<<position;
			}

			txtlabel->Text = MessageBuffers[i];
			found = 1;
			break;
		}
	}
	if (!found) {
		delete txtlabel;
		return (NULL);
	}

	/*------------------------------------------------------------------------
	Attach the message to our list
	------------------------------------------------------------------------*/
	if (MessageList) {
		txtlabel->Add_Tail (*MessageList);
	} else {
		MessageList = txtlabel;
	}

	return(txtlabel);
}


/***************************************************************************
 * MessageListClass::Add_Edit -- Adds editable string to message list      *
 *                                                                         *
 * INPUT:                                                                  *
 *      color         color of edit message                                *
 *      style         style of edit message                                *
 *      to            string: who to send to                               *
 *      width			width of editbox in pixels                           *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ptr to new TextLabelClass                                          *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/22/1995 BRR : Created.                                             *
 *=========================================================================*/
TextLabelClass * MessageListClass::Add_Edit(int color, TextPrintType style,
	char *to, int width)
{
	/*------------------------------------------------------------------------
	Do nothing if we're already in "edit" mode
	------------------------------------------------------------------------*/
	if (EditLabel)
		return(NULL);

	/*------------------------------------------------------------------------
	Initialize the buffer positions; add a new label to the label list.
	------------------------------------------------------------------------*/
	EditCurPos = EditInitPos = strlen(to);
	EditLabel = Add_Message (to, color, style, -1, 0, 0);
	Width = width;

	/*------------------------------------------------------------------------
	Save our edit buffer pointer.
	------------------------------------------------------------------------*/
	if (EditLabel)
		EditBuf = EditLabel->Text;
	else
		EditBuf = NULL;

	return(EditLabel);
}


/***************************************************************************
 * MessageListClass::Get_Edit_Buf -- gets edit buffer                      *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ptr to edit buffer, minus the "To:" header                         *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/21/1995 BRR : Created.                                             *
 *=========================================================================*/
char * MessageListClass::Get_Edit_Buf(void)
{
	if (!EditBuf)
		return(NULL);

	return(EditBuf + EditInitPos);
}


/***************************************************************************
 * MessageListClass::Manage -- Manages multiplayer messages                *
 *                                                                         *
 * If this routine returns TRUE, the caller should update the display.     *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      0 = no change has occurred, 1 = changed                            *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/05/1995 BRR : Created.                                             *
 *=========================================================================*/
int MessageListClass::Manage (void)
{
	TextLabelClass *txtlabel;
	TextLabelClass *next;
	int changed = 0;
	int y;
	GadgetClass *gadg;
	int i;

	/*------------------------------------------------------------------------
	Loop through all messages
	------------------------------------------------------------------------*/
	txtlabel = MessageList;
	while (txtlabel) {
		/*.....................................................................
		If this message's time is up, remove it from the list
		.....................................................................*/
		if (txtlabel->UserData != 0 && (unsigned)TickCount.Time() > txtlabel->UserData) {
			/*..................................................................
			If we're about to delete the edit message, clear our edit message
			values.
			..................................................................*/
			if (txtlabel == EditLabel) {
				EditLabel = 0;
				EditBuf = 0;
			}
			/*..................................................................
			Save the next ptr in the list; remove this entry
			..................................................................*/
			next = (TextLabelClass *)txtlabel->Get_Next();
			MessageList = (TextLabelClass *)txtlabel->Remove();
			for (i = 0; i < MAX_NUM_MESSAGES; i++) {
				if (txtlabel->Text == MessageBuffers[i])
					BufferAvail[i] = 1;
			}
			delete txtlabel;
			changed = 1;
			txtlabel = next;
		} else {
			txtlabel = (TextLabelClass *)txtlabel->Get_Next();
		}
	}

	/*------------------------------------------------------------------------
	If a changed has been made, recompute the y-coord of all messages
	------------------------------------------------------------------------*/
	if (changed) {

		y = MessageY;
		if (MessageList) {
			gadg = MessageList;
			while (gadg) {
				gadg->Y = y;
				gadg = gadg->Get_Next();
				y += Height;
			}
		}
	}

	return(changed);
}


/***************************************************************************
 * MessageListClass::Input -- Handles input for sending messages           *
 *                                                                         *
 * INPUT:                                                                  *
 *      input         key value to process                                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = caller should redraw the message list (no need to complete     *
 *        refresh, though)                                                 *
 *      2 = caller should completely refresh the display.                  *
 *      3 = caller should send the edit message.                           *
 *      (sets 'input' to 0 if it processes it.)                            *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/05/1995 BRR : Created.                                             *
 *=========================================================================*/
int MessageListClass::Input(KeyNumType &input)
{
	KeyASCIIType ascii;
	int retcode = 0;

	/*------------------------------------------------------------------------
	Do nothing if nothing to do.
	------------------------------------------------------------------------*/
	if (input == KN_NONE)
		return(0);

	/*------------------------------------------------------------------------
	Leave mouse events alone.
	------------------------------------------------------------------------*/
	if ( (input & (~KN_RLSE_BIT))==KN_LMOUSE ||
		(input & (~KN_RLSE_BIT))==KN_RMOUSE)
		return(0);

	/*------------------------------------------------------------------------
	If we're in 'edit mode', handle keys
	------------------------------------------------------------------------*/
	if (EditLabel) {
		ascii = (KeyASCIIType)(Keyboard::To_ASCII(input) & 0x00ff);

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


		switch (ascii) {
			/*------------------------------------------------------------------
			ESC = abort message
			------------------------------------------------------------------*/
			case KA_ESC & 0xff:
				EditLabel->UserData = 1;		// force it to be removed
				input = KN_NONE;
				break;

			/*------------------------------------------------------------------
			RETURN = send the message
			------------------------------------------------------------------*/
			case KA_RETURN & 0xff:
				EditLabel->UserData = 1;		// force it to be removed
				retcode = 3;
				input = KN_NONE;
				break;

			/*------------------------------------------------------------------
			BACKSPACE = remove a character
			------------------------------------------------------------------*/
			case KA_BACKSPACE & 0xff:
				if (EditCurPos > EditInitPos) {
					EditCurPos--;
					EditBuf[EditCurPos] = 0;
					retcode = 2;
				}
				input = KN_NONE;
				break;

			/*------------------------------------------------------------------
			default: add a character.  Reserve the last buffer position for null.
			(EditCurPos - EditInitPos) is the buffer index # of the next
			character, after the "To:" prefix.
			------------------------------------------------------------------*/
			default:
				if ( (EditCurPos - EditInitPos) < (MaxChars - 1) ) {
					if (!(input & WWKEY_VK_BIT) && ascii >= ' ' && ascii <= 127) {
						EditBuf[EditCurPos] = ascii;
						EditCurPos++;
						retcode = 1;

						/*
						** Verify that the additional character would not overrun the on screen edit box.
						*/
						Fancy_Text_Print(TXT_NONE, 0, 0, EditLabel->Color, TBLACK, EditLabel->Style);
						int width = String_Pixel_Width(EditBuf);
						if (width >= Width){
							EditBuf[EditCurPos--] = 0;
							retcode = 0;
						}

					}
				}
				input = KN_NONE;
				break;
		}
	}

	return(retcode);
}


/***************************************************************************
 * MessageListClass::Draw -- draws messages                                *
 *                                                                         *
 * INPUT:                                                                  *
 *      none                                                               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/22/1995 BRR : Created.                                             *
 *=========================================================================*/
void MessageListClass::Draw(void)
{
	if (MessageList) {
		Hide_Mouse();
		MessageList->Draw_All();
		Show_Mouse();
	}
}


/***************************************************************************
 * MessageListClass::Num_Messages -- returns # messages in the list        *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      # of messages                                                      *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/26/1995 BRR : Created.                                             *
 *=========================================================================*/
int MessageListClass::Num_Messages(void)
{
	GadgetClass *gadg;
	int num;

	num = 0;

	if (MessageList) {
		gadg = MessageList;
		while (gadg) {
			num++;
			gadg = gadg->Get_Next();
		}
	}

	return (num);
}


/***************************************************************************
 * MessageListClass::Set_Width -- sets allowable width of messages         *
 *                                                                         *
 * INPUT:                                                                  *
 *      width      pixel width                                             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/26/1995 BRR : Created.                                             *
 *=========================================================================*/
void MessageListClass::Set_Width(int width)
{
	GadgetClass *gadg;

	if (MessageList) {
		gadg = MessageList;
		while (gadg) {
			((TextLabelClass *)gadg)->PixWidth = width;
			gadg = gadg->Get_Next();
		}
	}
}

