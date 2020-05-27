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

/* $Header:   F:\projects\c&c\vcs\code\mapedit.h_v   2.19   16 Oct 1995 16:46:36   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MAPEDIT.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 14, 1994                                                 *
 *                                                                                             *
 *                  Last Update : May 14, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *	This class is derived from the normal display map class. It exists 		                    *
 * only to allow editing and adding items to the map.								                    *
 *---------------------------------------------------------------------------------------------*
 * House-setting functions: The editor contains several house maintenance routines:				  *
 * Verify_House: tells if the given ObjectType can be owned by the given HousesType				  *
 * Cycle_House: Finds the next valid house for the given ObjectType; used when a new object	  *
 *              can't be owned by the current editor HousesType.										  *
 * Change_House: attempts to change the owner of the currently-selected object					  *
 * Toggle_House: cycles the HousesType of a pending placement object									  *
 * Set_House_Buttons: sets house buttons in accordance with the given HousesType					  *
 *---------------------------------------------------------------------------------------------*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MAPEDIT_H
#define MAPEDIT_H

/*
********************************* Includes **********************************
*/
#include	"function.h"

/*
********************************** Defines **********************************
*/
/*...........................................................................
This is the maximum # of ObjectTypeClasses the editor has to deal with.
...........................................................................*/
enum MapEdit1Enum {
	MAX_EDIT_OBJECTS =				// max # of ObjectTypeClasses allowed
		(int)TEMPLATE_COUNT +
		(int)OVERLAY_COUNT +
		(int)SMUDGE_COUNT +
		(int)TERRAIN_COUNT +
		(int)UNIT_COUNT +
		(int)INFANTRY_COUNT +
		(int)AIRCRAFT_COUNT +
		(int)STRUCT_COUNT,

	MAX_TEAM_CLASSES =				// max # ObjectTypeClasses for a team
		(int)UNIT_COUNT +
		(int)INFANTRY_COUNT +
		(int)AIRCRAFT_COUNT,

//	NUM_EDIT_MISSIONS = 6,			// # missions that can be assigned an object

	NUM_EDIT_CLASSES = 8,			// # different classes (templates, terrain, etc)

	MAX_MAIN_MENU_NUM = 8,
	MAX_MAIN_MENU_LEN = 20,

	MAX_AI_MENU_NUM = 6,
	MAX_AI_MENU_LEN = 20,

	POPUP_GDI_W = 100,
	POPUP_GDI_H = 18,
	POPUP_GDI_X = 20,
	POPUP_GDI_Y = 320,

	POPUP_NOD_W = 100,
	POPUP_NOD_H = 18,
	POPUP_NOD_X = 20,
	POPUP_NOD_Y = 338,

	POPUP_NEUTRAL_W = 100,
	POPUP_NEUTRAL_H = 18,
	POPUP_NEUTRAL_X = 20,
	POPUP_NEUTRAL_Y = 356,

	POPUP_MULTI1_W = 50,
	POPUP_MULTI1_H = 18,
	POPUP_MULTI1_X = 20,
	POPUP_MULTI1_Y = 320,

	POPUP_MULTI2_W = 50,
	POPUP_MULTI2_H = 18,
	POPUP_MULTI2_X = 70,
	POPUP_MULTI2_Y = 320,

	POPUP_MULTI3_W = 50,
	POPUP_MULTI3_H = 18,
	POPUP_MULTI3_X = 20,
	POPUP_MULTI3_Y = 330,

	POPUP_MULTI4_W = 50,
	POPUP_MULTI4_H = 18,
	POPUP_MULTI4_X = 70,
	POPUP_MULTI4_Y = 338,

	POPUP_MISSION_W = 160,
	POPUP_MISSION_H = 80,
	POPUP_MISSION_X = 140,
	POPUP_MISSION_Y = 300,

	POPUP_FACEBOX_W = 60,
	POPUP_FACEBOX_H = 60,
	POPUP_FACEBOX_X = 320,
	POPUP_FACEBOX_Y = 320,

	POPUP_HEALTH_W = 100,
	POPUP_HEALTH_H = 20,
	POPUP_HEALTH_X = 400,
	POPUP_HEALTH_Y = 340,

	POPUP_BASE_W = 100,
	POPUP_BASE_H = 16,
	POPUP_BASE_X = 600 - POPUP_BASE_W,
	POPUP_BASE_Y = 0,
};

/*...........................................................................
These are the button ID's for the pop-up object-editing gizmos.
The house button ID's must be sequential, with a 1-to-1 correspondence to
the HousesType values.
...........................................................................*/
enum MapEditButtonIDEnum{
	POPUP_GDI=500,				// GDI house button
	POPUP_NOD,					// NOD house button
	POPUP_NEUTRAL,				// Neutral house button
	POPUP_HOUSE_JP,			// not used
	POPUP_MULTI1,				// Multiplayer 1 house button
	POPUP_MULTI2,				// Multiplayer 2 house button
	POPUP_MULTI3,				// Multiplayer 3 house button
	POPUP_MULTI4,				// Multiplayer 4 house button
	POPUP_MULTI5,				// Multiplayer 4 house button
	POPUP_MULTI6,				// Multiplayer 4 house button
	POPUP_MISSIONLIST,		// list box for missions
	POPUP_HEALTHGAUGE,		// health of object
	POPUP_FACINGDIAL,			// object's facing
	POPUP_BASEPERCENT,		// Base's percent-built slider
	MAP_AREA,					// map as a click-able thingy
	BUTTON_FLAG=0x8000
};


/*
******************************* Declarations ********************************
*/
class TeamTypeClass;

/*
***************************** Class Declaration *****************************
*/
class MapEditClass : public MouseClass 
{
	/*
	---------------------------- Public Interface ----------------------------
	*/
	public:
		/*
		............................. mapedit.cpp .............................
		*/
		MapEditClass(void);
		virtual void One_Time(void);							// One-time init
		virtual void Init_IO(void);							// Inits button list
		virtual void AI(KeyNumType &input, int x, int y);
		virtual void Draw_It(bool forced = true);
		virtual bool Scroll_Map(DirType facing, int & distance, bool really=true);
//		virtual void Flag_To_Redraw(bool complete);
		virtual void Read_INI(char *buffer);
		virtual void Write_INI(char *buffer);
		virtual void Detach(ObjectClass * object);
		void Clear_List(void);
		bool Add_To_List(ObjectTypeClass const *object);
		void Main_Menu(void);
		void AI_Menu(void);
		bool Mouse_Moved(void);
		bool Verify_House(HousesType house, ObjectTypeClass const * objtype);
		HousesType Cycle_House(HousesType curhouse, ObjectTypeClass const * objtype);
//		int Trigger_Needs_Team(TriggerClass *trigger);
		void Fatal(int txt);

		/*
		............................ mapeddlg.cpp .............................
		*/
		int New_Scenario(void);
		int Load_Scenario(void);
		int Save_Scenario(void);
		int Pick_Scenario(char const * caption, int *scen_nump, 
			ScenarioPlayerType *playerp, ScenarioDirType *dirp, 
			ScenarioVarType *varp, int multi);
		int Size_Map(int x, int y, int w, int h);
		int Scenario_Dialog(void);
		void Handle_Triggers(void);
		int Select_Trigger(void);
		int Edit_Trigger(void);
		int Import_Triggers(void);
		int Import_Teams(void);
		/*
		............................ mapedplc.cpp .............................
		*/
		int Placement_Dialog(void);
		void Start_Placement(void);
		int Place_Object(void);
		void Cancel_Placement(void);
		void Place_Next(void);
		void Place_Prev(void);
		void Place_Next_Category(void);
		void Place_Prev_Category(void);
		void Place_Home(void);
		void Toggle_House(void);
		void Set_House_Buttons(HousesType house, GadgetClass *btnlist, int base_id);
		void Start_Trigger_Placement(void);
		void Stop_Trigger_Placement(void);
		void Place_Trigger(void);
		void Start_Base_Building(void);
		void Cancel_Base_Building(void);
		void Build_Base_To(int percent);
		
		/*
		............................ mapedsel.cpp .............................
		*/
		int Select_Object(void);
		void Select_Next(void);
		void Popup_Controls(void);
		void Grab_Object(void);
		int Move_Grabbed_Object(void);
		bool Change_House(HousesType newhouse);
		
		/*
		............................. mapedtm.cpp .............................
		*/
		void Draw_Member(TechnoTypeClass const * ptr, int index, 
			int quant, HousesType house, int pic_x, int pic_y);
		void Handle_Teams(char const * caption);
		int Select_Team(char const * caption);
		int Edit_Team(void);
		int Team_Members(HousesType house);
		void Build_Mission_List(int missioncount, TeamMissionStruct *missions, 
			char missionbuf[TeamTypeClass::MAX_TEAM_MISSIONS][20], ListClass *list);

	/*
	--------------------------- Private Interface ----------------------------
	*/
	private:
		/*.....................................................................
		This is the last-requested variation of a loaded/saved/new scenario.
		.....................................................................*/
		ScenarioVarType ScenVar;

		/*.....................................................................
		Array of all TypeClasses the user can add to the map; cleared by 
		Clear_List(), added to by Add_To_List()
		.....................................................................*/
		ObjectTypeClass const * Objects[MAX_EDIT_OBJECTS];
		int ObjCount;							// # of objects in the Objects array

		/*.....................................................................
		Last-selected object to place, and last-selected house of object
		.....................................................................*/
		int LastChoice;						// index of item user picked last
		HousesType LastHouse;				// house of last item picked

		/*.....................................................................
		Variables for grabbing/moving objects
		.....................................................................*/
		ObjectClass * GrabbedObject;		// object "grabbed" with mouse
		CELL GrabOffset;						// offset to grabbed obj's upper-left
		unsigned long LastClickTime;		// time of last LMOUSE click

		/*.....................................................................
		Number of each type of object in Objects, so we can switch categories
		.....................................................................*/
		int NumType[NUM_EDIT_CLASSES];		// # of each type of class:
														// 0 = Template
														// 1 = Overlay
														// 2 = Smudge
														// 3 = Terrain
														// 4 = Unit
														// 5 = Infantry
														// 6 = Aircraft
														// 7 = Building

		/*.....................................................................
		The offset of each type of object within the Objects[] array
		.....................................................................*/
		int TypeOffset[NUM_EDIT_CLASSES];	// offsets within Objects[]

		/*.....................................................................
		The "current" trigger for point-and-click trigger setting
		.....................................................................*/
		TriggerClass * CurTrigger;				// current trigger

		/*.....................................................................
		The "current" team type for editing & associating with a trigger
		.....................................................................*/
		TeamTypeClass * CurTeam;				// current team

		/*.....................................................................
		Bitfields for flags & such
		.....................................................................*/
		int Changed : 1;						// 1 = changes are unsaved
		int LMouseDown : 1;					// 1 = left mouse is held down
		int BaseBuilding : 1;				// 1 = we're in base-building mode

		/*.....................................................................
		Variables for pre-building a base
		.....................................................................*/
		int BasePercent;						// Percentage the base will be built

		/*.....................................................................
		Variables for supporting the object-editing controls at screen bottom
		.....................................................................*/
		TextButtonClass *GDIButton;
		TextButtonClass *NODButton;
		TextButtonClass *NeutralButton;
		TextButtonClass *Multi1Button;
		TextButtonClass *Multi2Button;
		TextButtonClass *Multi3Button;
		TextButtonClass *Multi4Button;
		ListClass *MissionList;
		TriColorGaugeClass *HealthGauge;
		Dial8Class *FacingDial;
		ControlClass *MapArea;
		TextLabelClass *HealthText;
		static char HealthBuf[20];
		GaugeClass *BaseGauge;
		TextLabelClass *BaseLabel;
		static MissionType MapEditMissions[];
};

#endif
