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

/* $Header:   F:\projects\c&c\vcs\code\led.h_v   2.17   16 Oct 1995 16:45:50   JOE_BOSTIC  $ */

#ifndef LED_H
#define LED_H

class LEDClass
{
	public:
		typedef enum ControlType {
			LED_NOCHANGE,			// Do nothing (just query).
			LED_OFF,					// Turn LED off.
			LED_ON,					// Turn LED on.
			LED_TOGGLE				// Toggle LED state.
		} ControlType;

	protected:
		static int Shift_Control(ControlType control, char bit);

	public:
		static int Scroll_Lock(ControlType control=LED_TOGGLE) {return Shift_Control(control, 0x01);};
		static int Caps_Lock(ControlType control=LED_TOGGLE) {return Shift_Control(control, 0x02);};
		static int Num_Lock(ControlType control=LED_TOGGLE) {return Shift_Control(control, 0x04);};

	private:
		static void Send_To_Keyboard(unsigned char val);
};

#endif
