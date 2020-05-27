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

#ifdef WOLAPI_INTEGRATION

/***************************************************************************
 * WOLEditClass -- Derived from EditClass, includes changes I wanted for
 *                 wolapi integration stuff.
 *					Note: An editbox of this class cannot be made read-only. See comment below.
 * HISTORY:    07/17/1998 ajw : Created.
 *=========================================================================*/

#include "WOLEdit.h"

//#include "WolDebug.h"

bool bTabKeyPressedHack = false;

//***********************************************************************************************
void WOLEditClass::Draw_Text( char const * text )
{
	//	Only difference between this and EditClass: cursor shows up when
	//	string is at MaxLength.

	TextPrintType flags;

	if (Has_Focus()) {
		flags = TPF_BRIGHT_COLOR;
	} else {
		flags = (TextPrintType)0;
	}

	Conquer_Clip_Text_Print(text, X+1, Y+1, Color, TBLACK, TextFlags | flags, Width-2);

	if (Has_Focus() &&		//	strlen(text) < MaxLength &&
		(String_Pixel_Width(text) + String_Pixel_Width ("_") < Width-2) ) {
		Conquer_Clip_Text_Print( "_", X+1+String_Pixel_Width(text), Y+1, Color, TBLACK, TextFlags | flags);
	}
}

//***********************************************************************************************
//	Override of EditClass::Action, because the base class does not behave correctly in certain circumstances.
//	(Escape key is being processed as enter key.)
//	Again, I'm not about to change the base class directly, as I'm trying to have as minimal an affect as possible on
//	the current game code. -ajw
int WOLEditClass::Action(unsigned flags, KeyNumType & key)
{
	//	(Mostly duplicated from base class ::Action)
/*	For some painful reason, IsReadOnly is private in the base class, so I can't do the following.
	For this reason, don't make a WOLEditClass edit box read-only.

	//
	// If this is a read-only edit box, it's a display-only device
	//
	if (IsReadOnly) {
		return(false);
	}
*/

	//debugprint( "WOLEditClass::Action this=%i, flags=0x%x, key=0x%x\n", this, flags, key );
	//
	//	If the left mouse button is pressed over this gadget, then set the focus to
	//	this gadget. The event flag is cleared so that no button ID number is returned.
	//
	if ((flags & LEFTPRESS)) {
		flags &= ~LEFTPRESS;
		Set_Focus();
		Flag_To_Redraw();		// force to draw cursor
	}

	//
	//	Handle keyboard events here. Normally, the key is added to the string, but if the
	//	RETURN key is pressed, then the button ID number is returned from the Input()
	//	function.
	//
	if ((flags & KEYBOARD) && Has_Focus()) {

		//
		//	Process the keyboard character. If indicated, consume this keyboard event
		//	so that the edit gadget ID number is not returned.
		//
		if (key == KN_ESC) {

			Clear_Focus();
			flags = 0;

		} else {
#ifdef WIN32

			KeyASCIIType ascii = (KeyASCIIType)(Keyboard->To_ASCII(key) & 0xff);

			//
			// Allow numeric keypad presses to map to ascii numbers
			//
			if ((key & WWKEY_VK_BIT) && ascii >='0' && ascii <= '9') {

				key = (KeyNumType)(key & ~WWKEY_VK_BIT);
				if ( (!(flags & LEFTRELEASE)) && (!(flags & RIGHTRELEASE))) {
					if (Handle_Key (ascii) ) {
						flags &= ~KEYBOARD;
						key = KN_NONE;
					}
				}
			} else {
				//
				// Filter out all special keys except return and backspace
				//  	
				if ((!(key & WWKEY_VK_BIT) && ascii >= ' ' && ascii <= 255)
					|| key == KN_RETURN || key == KN_BACKSPACE) {



					if ((!(flags & LEFTRELEASE)) && (!(flags & RIGHTRELEASE))) {
						if (Handle_Key(Keyboard->To_ASCII(key))) {
							flags &= ~KEYBOARD;
							key = KN_NONE;
						}
					}
				} else {
					if( key == KN_TAB )
					{
						bTabKeyPressedHack = true;
					}
					flags &= ~KEYBOARD;
					key = KN_NONE;
				}
			}
		}

#else	//WIN32
			if (Handle_Key(Keyboard->To_ASCII(key))) {
				flags &= ~KEYBOARD;
				key = KN_NONE;
			}
		}
#endif	//WIN32
	}
	else
	{
		//	ajw added
//		if( key == ( KN_ESC | WWKEY_RLS_BIT ) && ( key & WWKEY_ALT_BIT ) )
//		{
			//Clear_Focus();
			flags = 0;
			key = KN_NONE;
//		}
	}

	return(ControlClass::Action(flags, key));
}

#endif
