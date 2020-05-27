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

/* $Header:   F:\projects\c&c\vcs\code\phone.h_v   1.9   16 Oct 1995 16:47:58   JOE_BOSTIC  $ */
/*************************************************************************** 
 *                                                                         * 
 *                 Project Name : Command & Conquer                        * 
 *                                                                         * 
 *                    File Name : PHONE.H                                  * 
 *                                                                         * 
 *                   Programmer : Bill R. Randolph                         * 
 *                                                                         * 
 *                   Start Date : 04/28/95                                 * 
 *                                                                         * 
 *                  Last Update : April 28, 1995 [BRR]                     * 
 *                                                                         * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef PHONE_H
#define PHONE_H

/*
***************************** Class Declaration *****************************
*/
class PhoneEntryClass
{
	public:
		enum PhoneEntryEnum {
			PHONE_MAX_NAME = 21,
			PHONE_MAX_NUM = 21
		};

		PhoneEntryClass(void) {};
		~PhoneEntryClass() {};

		operator == (PhoneEntryClass &obj) 
			{ return (memcmp (Name,obj.Name,strlen(Name))==0);}
		operator != (PhoneEntryClass &obj)
			{ return (memcmp (Name,obj.Name,strlen(Name))!=0);}
		operator > (PhoneEntryClass &obj)
			{ return (memcmp(Name, obj.Name, strlen(Name)) > 0);}
		operator < (PhoneEntryClass &obj)
			{ return (memcmp(Name, obj.Name, strlen(Name)) < 0);}
		operator >= (PhoneEntryClass &obj)
			{ return (memcmp(Name, obj.Name, strlen(Name)) >= 0);}
		operator <= (PhoneEntryClass &obj)
			{ return (memcmp(Name, obj.Name, strlen(Name)) <= 0);}

		SerialSettingsType Settings;
		char Name[ PHONE_MAX_NAME ];		// destination person's name
		char Number[ PHONE_MAX_NUM ];		// phone #
};

#endif
