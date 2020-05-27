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

/* $Header:   F:\projects\c&c0\vcs\code\conquer.cpv   4.74   23 Sep 1996 12:36:00   JOE_BOSTIC  $ */
/***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Westwood Keyboard Library                                    *
 *                                                                                             *
 *                    File Name : KEYBOARD.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Philip W. Gorrow                                             *
 *                                                                                             *
 *                   Start Date : 10/16/95                                                     *
 *                                                                                             *
 *                  Last Update : September 24, 1996 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   WWKeyboardClass::Put -- Logic to insert a key into the keybuffer]                         *
 *   WWKeyboardClass::Get -- Logic to get a metakey from the buffer                            *
 *   WWKeyboardClass::Check -- Checks to see if a key is in the buffer                         *
 *   WWKeyboardClass::Put_Key_Message -- Translates and inserts wParam into Keyboard Buffer    *
 *   WWKeyboardClass::Buff_Get -- Lowlevel function to get a key from key buffer               *
 *   WWKeyboardClass::Get_Mouse_X -- Returns the mouses current x position in pixels           *
 *   WWKeyboardClass::Get_Mouse_Y -- returns the mouses current y position in pixels           *
 *   WWKeyboardClass::Get_Mouse_XY -- Returns the mouses x,y position via reference vars       *
 *   Check_Key -- compatability routine for old 32 bit library                                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "keyboard.h"

#include	"monoc.h"

//void Message_Loop(void);

WWKeyboardClass * _Kbd = NULL;


#define	ARRAY_SIZE(x)		int(sizeof(x)/sizeof(x[0]))


/***********************************************************************************************
 * WWKeyboardClass::WWKeyBoardClass -- Construction for Westwood Keyboard Class                *
 *                                                                                             *
 * INPUT:		none							                                                        *
 *                                                                                             *
 * OUTPUT:     none							                                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
WWKeyboardClass::WWKeyboardClass(void) :
	MouseQX(0),
	MouseQY(0),
	Head(0),
	Tail(0)
//	MState(0),
//	Conditional(0),
//	CurrentCursor(0)
{
	_Kbd = this;

	memset(KeyState, '\0', sizeof(KeyState));
}


/***********************************************************************************************
 * WWKeyboardClass::Buff_Get -- Lowlevel function to get a key from key buffer                 *
 *                                                                                             *
 * INPUT:		none                                                        						  *
 *                                                                                             *
 * OUTPUT:     int		- the key value that was pulled from buffer (includes bits)				  *                                                                                *
 *                                                                                             *
 * WARNINGS:   If the key was a mouse event MouseQX and MouseQY will be updated                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
unsigned short WWKeyboardClass::Buff_Get(void)
{
	while (!Check()) {}										// wait for key in buffer

	unsigned short temp = Fetch_Element();
	if (Is_Mouse_Key(temp)) {
		MouseQX = Fetch_Element();
		MouseQY = Fetch_Element();
	}
	return(temp);
}


bool WWKeyboardClass::Is_Mouse_Key(unsigned short key)
{
	key &= 0xFF;
	return (key == VK_LBUTTON || key == VK_MBUTTON || key == VK_RBUTTON);
}


/***********************************************************************************************
 * WWKeyboardClass::Check -- Checks to see if a key is in the buffer                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 PWG : Created.                                                                 *
 *   09/24/1996 JLB : Converted to new style keyboard system.                                  *
 *=============================================================================================*/
unsigned short WWKeyboardClass::Check(void) const
{
	((WWKeyboardClass *)this)->Fill_Buffer_From_System();
	if (Is_Buffer_Empty()) return(false);
	return(Peek_Element());
}


/***********************************************************************************************
 * WWKeyboardClass::Get -- Logic to get a metakey from the buffer                              *
 *                                                                                             *
 * INPUT:		none                                                        						  *
 *                                                                                             *
 * OUTPUT:     int		- the meta key taken from the buffer.											  *
 *                                                                                             *
 * WARNINGS:	This routine will not return until a keypress is received							  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
unsigned short WWKeyboardClass::Get(void)
{
	while (!Check()) {}								// wait for key in buffer
	return (Buff_Get());
}


/***********************************************************************************************
 * WWKeyboardClass::Put -- Logic to insert a key into the keybuffer]                           *
 *                                                                                             *
 * INPUT:		int	 	- the key to insert into the buffer          								  *
 *                                                                                             *
 * OUTPUT:     bool		- true if key is sucessfuly inserted.							              *
 *                                                                                             *
 * WARNINGS:   none							                                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
bool WWKeyboardClass::Put(unsigned short key)
{
	if (!Is_Buffer_Full()) {
		Put_Element(key);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * WWKeyboardClass::Put_Key_Message -- Translates and inserts wParam into Keyboard Buffer      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
bool WWKeyboardClass::Put_Key_Message(unsigned short vk_key, bool release)
{
	//
	// Get the status of all of the different keyboard modifiers.  Note, only pay attention
	// to numlock and caps lock if we are dealing with a key that is affected by them.  Note
	// that we do not want to set the shift, ctrl and alt bits for Mouse keypresses as this
	// would be incompatible with the dos version.
	//
	if (!Is_Mouse_Key(vk_key)) {
		if (((GetKeyState(VK_SHIFT) & 0x8000) != 0) ||
			((GetKeyState(VK_CAPITAL) & 0x0008) != 0) ||
			((GetKeyState(VK_NUMLOCK) & 0x0008) != 0)) {

			vk_key |= WWKEY_SHIFT_BIT;
		}
		if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) {
			vk_key |= WWKEY_CTRL_BIT;
		}
		if ((GetKeyState(VK_MENU) & 0x8000) != 0) {
			vk_key |= WWKEY_ALT_BIT;
		}
	}

	if (release) {
		vk_key |= WWKEY_RLS_BIT;
	}

	//
	// Finally use the put command to enter the key into the keyboard
	// system.
	//
	return(Put(vk_key));



}

#pragma warning (disable : 4065)
char WWKeyboardClass::To_ASCII(unsigned short key)
{
	/*
	**	Released keys never translate into an ASCII value.
	*/
	if (key & WWKEY_RLS_BIT) {
		return('\0');
	}

	/*
	**	Set the KeyState buffer to reflect the shift bits stored in the key value.
	*/
	if (key & WWKEY_SHIFT_BIT) {
		KeyState[VK_SHIFT] = 0x80;
	}
	if (key & WWKEY_CTRL_BIT) {
		KeyState[VK_CONTROL] = 0x80;
	}
	if (key & WWKEY_ALT_BIT) {
		KeyState[VK_MENU] = 0x80;
	}

	/*
	**	Ask windows to translate the key into an ASCII equivalent.
	*/
	char buffer[10];
	int result = 1;
	int scancode = 0;
	char override = '\0';

	switch (key & 0xFF) {
//		case KN_RETURN:
//			override = KA_RETURN;
//			break;

//		case KN_BACKSPACE:
//			override = KA_BACKSPACE;
//			break;

		default:
			scancode = MapVirtualKey(key & 0xFF, 0);
			result = ToAscii((UINT)(key & 0xFF), (UINT)scancode, (PBYTE)KeyState, (LPWORD)buffer, (UINT)0);
			break;
	}

	/*
	**	Restore the KeyState buffer back to pristine condition.
	*/
	if (key & WWKEY_SHIFT_BIT) {
		KeyState[VK_SHIFT] = 0;
	}
	if (key & WWKEY_CTRL_BIT) {
		KeyState[VK_CONTROL] = 0;
	}
	if (key & WWKEY_ALT_BIT) {
		KeyState[VK_MENU] = 0;
	}

	/*
	**	If Windows could not perform the translation as expected, then
	**	return with a null ASCII value.
	*/
	if (result != 1) {
		return('\0');
	}

	if (override != 0) {
		return(override);
	}

	return(buffer[0]);
}


bool WWKeyboardClass::Down(unsigned short key)
{
	return(GetAsyncKeyState(key & 0xFF) == 0 ? false : true);
}


extern "C" {
	void __cdecl Stop_Execution (void);
}



unsigned short WWKeyboardClass::Fetch_Element(void)
{
	unsigned short val = 0;
	if (Head != Tail) {
		val = Buffer[Head];

		Head = (Head + 1) % ARRAY_SIZE(Buffer);
	}
	return(val);
}


unsigned short WWKeyboardClass::Peek_Element(void) const
{
	if (!Is_Buffer_Empty()) {
		return(Buffer[Head]);
	}
	return(0);
}


bool WWKeyboardClass::Put_Element(unsigned short val)
{
	if (!Is_Buffer_Full()) {
		int temp = (Tail+1) % ARRAY_SIZE(Buffer);
		Buffer[Tail] = val;
		Tail = temp;
		return(true);
	}
	return(false);
}


bool WWKeyboardClass::Is_Buffer_Full(void) const
{
	if ((Tail + 1) % ARRAY_SIZE(Buffer) == Head) {
		return(true);
	}
	return(false);
}


bool WWKeyboardClass::Is_Buffer_Empty(void) const
{
	if (Head == Tail) {
		return(true);
	}
	return(false);
}


void WWKeyboardClass::Fill_Buffer_From_System(void)
{
	if (!Is_Buffer_Full()) {
		MSG	msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		  	if (!GetMessage( &msg, NULL, 0, 0 )) {
				return;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


void WWKeyboardClass::Clear(void)
{
	Head = Tail;
}


void WWKeyboardClass::Message_Handler(HWND , UINT message, UINT wParam, LONG lParam)
{
	switch (message) {
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if ( wParam==VK_SCROLL ){
				Stop_Execution();
			} else {
				Put_Key_Message((unsigned short)wParam);
			}
			break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
			Put_Key_Message((unsigned short)wParam, true);
			break;

		case WM_LBUTTONDOWN:
			Put_Key_Message(VK_LBUTTON);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_LBUTTONUP:
			Put_Key_Message(VK_LBUTTON, true);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_LBUTTONDBLCLK:
			Put_Key_Message(VK_LBUTTON, true);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			Put_Key_Message(VK_LBUTTON, true);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_MBUTTONDOWN:
			Put_Key_Message(VK_MBUTTON);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_MBUTTONUP:
			Put_Key_Message(VK_MBUTTON, true);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_MBUTTONDBLCLK:
			Put_Key_Message(VK_MBUTTON, true);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			Put_Key_Message(VK_MBUTTON, true);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_RBUTTONDOWN:
			Put_Key_Message(VK_RBUTTON);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_RBUTTONUP:
			Put_Key_Message(VK_RBUTTON, true);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_RBUTTONDBLCLK:
			Put_Key_Message(VK_RBUTTON, true);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			Put_Key_Message(VK_RBUTTON, true);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

//		case WM_MOUSEMOVE:
//			if (CurrentCursor)
//				SetCursor(CurrentCursor);
//			break;
	}
}
