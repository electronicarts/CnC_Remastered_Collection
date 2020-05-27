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
 *                  Last Update : October 26, 1995   []                    *
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
 *   Check_Key -- compatability routine for old 32 bit library             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "keyboard.h"
#include "timer.h"
#include "mono.h"

void Message_Loop(void);

WWKeyboardClass *_Kbd;

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
WWKeyboardClass::WWKeyboardClass(void)
{
	_Kbd = this;
	//
	// Initialize the keyboard remap table for our system (note it would be bad if someone
	// switched keyboard modes after this happened.
	//
	memset(VKRemap, 0, 256);
	memset(AsciiRemap, 0, 2048);
	short lp;
	for (lp = 31; lp < 255; lp ++) {
		if (isprint(lp)) {
			int vk_key = VkKeyScan((unsigned char)lp);
			if (vk_key > 0 && vk_key < 2048) {
				AsciiRemap[vk_key] = (unsigned char)lp;
				VKRemap[lp] 	 = (unsigned char)(vk_key & 0xFF);
			}
		}
	}

	//
	// Build a remap table of the different keys which are affected by the caps lock and
	// the num lock.
	//
	memset(ToggleKeys, 0, 256);
	for (lp = 0; lp < 255; lp++ ) {
		if (isalpha(lp) && isupper(lp)) {
			ToggleKeys[lp] = 1;
		}
		if (lp >= VK_NUMPAD0 && lp <= VK_DIVIDE) {
			ToggleKeys[lp] = 2;
		}
	}

	//
	// Our buffer should start devoid of keys.
	//
	memset(Buffer, 0, 256);
	Head				= 0;
	Tail				= 0;

	//
	// There should be no starting queued mouse events for us to have to worry
	// about.
	//
	MouseQX			= 0;
	MouseQY			= 0;
	MState			= 0;
	Conditional		= 0;
	CurrentCursor	= NULL;
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
int WWKeyboardClass::Buff_Get(void)
{
	while (!Check()) {}										// wait for key in buffer
	int 	temp		= Buffer[Head];						// get key out of the buffer
	int   newhead	= Head;									// save off head for manipulation
	if (Is_Mouse_Key(temp)) {								// if key is a mouse then
		MouseQX	= Buffer[(Head + 1) & 255];			//		get the x and y pos
		MouseQY	= Buffer[(Head + 2) & 255];			//		from the buffer
		newhead += 3;		  									//		adjust head forward
	} else {
		newhead += 1;		  									//		adjust head forward
	}
	newhead	&= 255;
	Head		 = newhead;
	return(temp);
}

BOOL WWKeyboardClass::Is_Mouse_Key(int key)
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
 *=============================================================================================*/
BOOL WWKeyboardClass::Check(void)
{
	Message_Loop();
	unsigned short temp;											// store temp holding spot for key
	if (Head == Tail) return(FALSE);							// if no keys in buff then get out
	temp = Buffer[Head];											// get key out of the buffer
	return(temp);													// send it back to main program
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
int WWKeyboardClass::Get(void)
{
	int temp,bits;										// store temp holding spot for key

	while (!Check()) {}								// wait for key in buffer
	temp = Buff_Get();								// get key from the buffer

	bits = temp & 0xFF00;							// save of keyboard bits

	if (!(bits & WWKEY_VK_BIT)) {					// if its not a virtual key
		temp = AsciiRemap[temp&0x1FF] | bits;	//   convert to ascii equivalent
	}
	return(temp);							// return the key that we pulled out
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
BOOL WWKeyboardClass::Put(int key)
{
	int	temp		= (Tail + 1) & 255;
	if (temp != Head)
	{
		Buffer[Tail]	= (short)key;

		//
		// Critical Line
		//
		Tail				= temp;
		return(TRUE);
	}
	return(FALSE);
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
BOOL WWKeyboardClass::Put_Key_Message(UINT vk_key, BOOL release, BOOL dbl)
{
	int bits = 0;
	//
	// Get the status of all of the different keyboard modifiers.  Note, only pay attention
	// to numlock and caps lock if we are dealing with a key that is affected by them.  Note
	// that we do not want to set the shift, ctrl and alt bits for Mouse keypresses as this
	// would be incompatible with the dos version.
	//
	if (vk_key != VK_LBUTTON && vk_key != VK_MBUTTON && vk_key != VK_RBUTTON) 	{
		int	shift		= (GetKeyState(VK_SHIFT) & 0xFF00) 		!= 0;
		int	ctrl 		= (GetKeyState(VK_CONTROL) & 0xFF00) 	!= 0;
		int	alt 		= (GetKeyState(VK_MENU) & 0xFF00) 		!= 0;
		int	caps		= ((GetKeyState(VK_CAPITAL) & 0x00FF) 	!= 0) && (ToggleKeys[vk_key] == 1);
		int	nums		= ((GetKeyState(VK_NUMLOCK) & 0x00FF) 	!= 0) && (ToggleKeys[vk_key] == 2);

		//
		// Set the proper bits for whatever the key we got is.
		//
		if (shift || caps || nums) {
			bits |= WWKEY_SHIFT_BIT;
		}
		if (ctrl) {
			bits |= WWKEY_CTRL_BIT;
		}
		if (alt) {
			bits |= WWKEY_ALT_BIT;
		}
		}
	if (!AsciiRemap[vk_key|bits]) {
		bits |= WWKEY_VK_BIT;
	}
	if (release) {
		bits |= WWKEY_RLS_BIT;
	}
	if (dbl) {
		bits |= WWKEY_DBL_BIT;
	}
	//
	// Finally use the put command to enter the key into the keyboard
	// system.
	//
	return(Put(vk_key|bits));
}

void WWKeyboardClass::Clear(void)
{
	Head = Tail;
}

int WWKeyboardClass::To_ASCII(int key)
{
	if ( key && WWKEY_RLS_BIT)
	  return(KN_NONE);
	return(key);
}

int WWKeyboardClass::Down(int key)
{
	return(GetAsyncKeyState(key&0xFF));
}

VOID WWKeyboardClass::Split(int &key, int &shift, int &ctrl, int &alt, int &rls, int &dbl)
{
	shift = (key & WWKEY_SHIFT_BIT)	!= 0;
	ctrl	= (key & WWKEY_CTRL_BIT)	!= 0;
	alt	= (key & WWKEY_ALT_BIT)		!= 0;
	rls	= (key & WWKEY_RLS_BIT)		!= 0;
	dbl	= (key & WWKEY_DBL_BIT)		!= 0;
	key	= (key & 0xFF);

}


extern "C" {
	void __cdecl Stop_Execution (void);
}

//#pragma off(unreferenced)
void WWKeyboardClass::Message_Handler(HWND , UINT message, UINT wParam, LONG lParam)
{
	return;
#if (0)	//   ST - 12/20/2018 11:27AM
	switch (message) {
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if ( wParam==VK_SCROLL ){
				Stop_Execution();
			} else {
				Put_Key_Message(wParam);
			}
			break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
			Put_Key_Message(wParam, TRUE);
			break;

		case WM_LBUTTONDOWN:
			Put_Key_Message(VK_LBUTTON);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_LBUTTONUP:
			Put_Key_Message(VK_LBUTTON, TRUE);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_LBUTTONDBLCLK:
			Put_Key_Message(VK_LBUTTON, TRUE, TRUE);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_MBUTTONDOWN:
			Put_Key_Message(VK_MBUTTON);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_MBUTTONUP:
			Put_Key_Message(VK_MBUTTON, TRUE);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_MBUTTONDBLCLK:
			Put_Key_Message(VK_MBUTTON, TRUE, TRUE);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_RBUTTONDOWN:
			Put_Key_Message(VK_RBUTTON);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_RBUTTONUP:
			Put_Key_Message(VK_RBUTTON, TRUE);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;

		case WM_RBUTTONDBLCLK:
			Put_Key_Message(VK_RBUTTON, TRUE, TRUE);
			Put(LOWORD(lParam));
			Put(HIWORD(lParam));
			break;
		case WM_MOUSEMOVE:
			if (CurrentCursor)
				SetCursor(CurrentCursor);
			break;
	}
#endif
}
//#pragma on(unreferenced)




void Message_Loop(void)
{

	MSG	msg;

	while (PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE )) {
	  	if( !GetMessage( &msg, NULL, 0, 0 ) ){
			return;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}




}



/***************************************************************************
 * CHECK_KEY -- compatability routine for old 32 bit library               *
 *                                                                         *
 * This routine checks to see if there is a key in the keyboard buffer     *
 * and returns it to the sender if there is.  It does not remove the key	*
 * from the buffer.																			*
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     The key that was pressed.                                   *
 *                                                                         *
 * WARNINGS:   You must declare a WWKeyboardClass object before calling    *
 *					this routine.																*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995     : Created.                                             *
 *=========================================================================*/
int Check_Key(void)
{
	if (!_Kbd) return(KA_NONE);
	return(_Kbd->Check() & ~WWKEY_SHIFT_BIT);
}

void Clear_KeyBuffer(void)
{
	if (!_Kbd) return;
	_Kbd->Clear();
}

int Check_Key_Num(void)
{
	if (!_Kbd) return(KN_NONE);
	int key		= _Kbd->Check();
	int flags	= key & 0xFF00;
	key		   = key & 0x00FF;

	if (isupper(key)) {
		key = tolower(key);
		if ( !(flags & WWKEY_VK_BIT) ) {
			flags |= WWKEY_SHIFT_BIT;
		}
	}

	return(key | flags);
}

int Get_Key_Num(void)
{
	if (!_Kbd) return(KN_NONE);
	int key		= _Kbd->Get();
	int flags	= key & 0xFF00;
	key		   = key & 0x00FF;

	if (isupper(key)) {
		key = tolower(key);
		if ( !(flags & WWKEY_VK_BIT) ) {
			flags |= WWKEY_SHIFT_BIT;
		}
	}
	return(key | flags);
}

int KN_To_KA(int key)
{
	if ( key & WWKEY_RLS_BIT) {
		return(KA_NONE);
	}
	if (!(key & WWKEY_VK_BIT)) {
		int flags = key & 0xFF00;
		key		 = key & 0x00FF;
		if (flags & WWKEY_SHIFT_BIT) {
			key		= toupper(key);
			flags	  &= ~WWKEY_SHIFT_BIT;
		}
	}else{
		/*
		** If its a numeric keypad key then fix it up
		*/
		if ((key & 0xff) >=VK_NUMPAD0 && (key & 0xff) <=VK_NUMPAD9){
			key = (key & 0xff) - VK_NUMPAD0 + KA_0;
		}
	}
	return(key);
}

int KN_To_VK(int key)
{
	if (!_Kbd) return(KN_NONE);
	if ( key & WWKEY_RLS_BIT) {
		return(VK_NONE);
	}

	int flags = key & 0xFF00;
	if (!(flags & WWKEY_VK_BIT)) {
		key 		 = _Kbd->VKRemap[key & 0x00FF] | flags;
	}
	key &= ~WWKEY_VK_BIT;
	return(key);
}

int Key_Down(int key)
{
	if (!_Kbd) return(FALSE);
	return(_Kbd->Down(key));
}

int Get_Key(void)
{
	int retval;

	if (!_Kbd) return(KN_NONE);
	retval = _Kbd->Get() & ~WWKEY_SHIFT_BIT;
	if (retval & WWKEY_RLS_BIT) {
		retval = KN_NONE;
	}
	return(retval);
}
