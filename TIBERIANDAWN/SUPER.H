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

/* $Header:   F:\projects\c&c\vcs\code\super.h_v   1.5   16 Oct 1995 16:47:04   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : SUPER.H                                                      * 
 *                                                                                             * 
 *                   Programmer : Joe L. Bostic                                                * 
 *                                                                                             * 
 *                   Start Date : 07/28/95                                                     * 
 *                                                                                             * 
 *                  Last Update : July 28, 1995 [JLB]                                          * 
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef SUPER_H
#define SUPER_H

#include	"ftimer.h"

class SuperClass {
	public:
		SuperClass(int recharge=0, VoxType charging=VOX_NONE, VoxType ready=VOX_NONE, VoxType impatient=VOX_NONE, VoxType suspend=VOX_NONE);

		bool Suspend(bool on);
		bool Enable(bool onetime = false, bool player=false, bool quiet=false);
		void Forced_Charge(bool player=false);
		bool AI(bool player=false);
		bool Remove(bool forced=false);
		void Impatient_Click(void) const;
		int Anim_Stage(void) const;
		bool Discharged(bool player);
		bool Is_Ready(void) const {return(IsReady);};
		bool Is_Present(void) const {return(IsPresent);};
		bool Is_One_Time(void) const {return(IsOneTime && IsPresent);};

		//Needed access to recharge times for tooltips - 2019/08/14 Jason Scott 
		int Get_Recharge_Time() const { return(RechargeTime); };

	private:
		bool Recharge(bool player=false);

		unsigned IsPresent:1;
		unsigned IsOneTime:1;
		unsigned IsReady:1;
		unsigned IsSuspended:1;

		TCountDownTimerClass Control;
		int OldStage;
		int SuspendTime;

		VoxType VoxRecharge;
		VoxType VoxCharging;
		VoxType VoxImpatient;
		VoxType VoxSuspend;
		int RechargeTime;

		//Needed to make ANIMATION_STAGES public so the animation frame numbers could be turned into progress 
		//percentages - 2019/08/07 Jason Scott 
public:
		enum {
			ANIMATION_STAGES=102
		};
};



#endif
