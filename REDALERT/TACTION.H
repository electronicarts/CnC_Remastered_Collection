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

/* $Header: /CounterStrike/TACTION.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : ACTION.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/28/95                                                     *
 *                                                                                             *
 *                  Last Update : November 28, 1995 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef ACTION_H
#define ACTION_H

typedef enum TActionType : unsigned char {
	TACTION_NONE,

	TACTION_WIN,								// player wins!
	TACTION_LOSE,								// player loses.
	TACTION_BEGIN_PRODUCTION,				// computer begins factory production.
	TACTION_CREATE_TEAM,						// computer creates a certain type of team
	TACTION_DESTROY_TEAM,
	TACTION_ALL_HUNT,							// all enemy units go into hunt mode (teams destroyed).
	TACTION_REINFORCEMENTS,					// player gets reinforcements
													// (house that gets them is determined by
													// the Reinforcement instance)
	TACTION_DZ,									// Deploy drop zone smoke.
	TACTION_FIRE_SALE,						// Sell all buildings and go on rampage.
	TACTION_PLAY_MOVIE,						//	Play movie (temporarily suspend game).
	TACTION_TEXT_TRIGGER,					// Triggers a text message display.
	TACTION_DESTROY_TRIGGER,				// Destroy specified trigger.
	TACTION_AUTOCREATE,						// Computer to autocreate teams.
	TACTION_WINLOSE,							// Win if captured, lose if destroyed.
	TACTION_ALLOWWIN,							// Allows winning if triggered.

	TACTION_REVEAL_ALL,						// Reveal the entire map.
	TACTION_REVEAL_SOME,						// Reveal map around cell #.
	TACTION_REVEAL_ZONE,						// Reveal all of specified zone.
	TACTION_PLAY_SOUND,						// Play sound effect.
	TACTION_PLAY_MUSIC,						// Play musical score.
	TACTION_PLAY_SPEECH,						// Play EVA speech.
	TACTION_FORCE_TRIGGER,					// Force trigger to activate.
	TACTION_START_TIMER,						// Start mission timer.
	TACTION_STOP_TIMER,						// Stop mission timer.
	TACTION_ADD_TIMER,						// Increase mission timer time.
	TACTION_SUB_TIMER,						// Decrease mission timer time.
	TACTION_SET_TIMER,						// Set and start the mission timer.
	TACTION_SET_GLOBAL,						// Set global variable.
	TACTION_CLEAR_GLOBAL,					// Clear global variable.
	TACTION_BASE_BUILDING,					// Automated base building.
	TACTION_CREEP_SHADOW,					// Shadow grows back one 'step'.

	TACTION_DESTROY_OBJECT,					// Destroys the building this trigger is attached to.
	TACTION_1_SPECIAL,						// Add a one-time special weapon ability to house.
	TACTION_FULL_SPECIAL,					// Add a repeating special weapon ability to house.

	TACTION_PREFERRED_TARGET,				// Designates preferred target for house.
	TACTION_LAUNCH_NUKES,					// Launch fake nuclear missiles from all silos

	TACTION_COUNT,
	TACTION_FIRST=0
} TActionType;

TActionType Action_From_Name(char const * name);
char const * Name_From_Action(TActionType action);
NeedType Action_Needs(TActionType action);

class TriggerTypeClass;
class TeamTypeClass;

/*
**	This elaborates the information necessary to carry out
**	a trigger's action.
*/
struct TActionClass {
	TActionType		Action;	// Action to perform.

	CCPtr<TeamTypeClass> Team;	// Team type pointer for this action (if needed).

	CCPtr<TriggerTypeClass> Trigger;	// Trigger type pointer for this action (if needed).

	union {
		ThemeType			Theme;		// Musical theme.
		VocType				Sound;		// Sound effect.
		VoxType				Speech;		// Speech identifier.
		HousesType			House;		// House to be affected.
		SpecialWeaponType Special;		// Special weapon ability.
		QuarryType			Quarry;		// Preferred target for attack.
		VQType				Movie;		// The movie to play.
		bool					Bool;			// Boolean value.
		long					Value;
	} Data;

	TActionClass(void) : Action(TACTION_NONE) {
		Data.Theme = THEME_NONE;
		Data.Value = -1;
	};
	TActionClass(NoInitClass const & x) : Team(x), Trigger(x) {};

	void Detach(TARGET target);
	void Code_Pointers(void);
	void Decode_Pointers(void);
	void Read_INI(void);
	void Build_INI_Entry(char * buffer) const;

	bool operator() (HousesType house, ObjectClass * object, int id, CELL cell);
};


class ActionChoiceClass {
	public:
		ActionChoiceClass(TActionType event=TACTION_NONE) : Action(event) {}

		operator TActionType (void) const {return(Action);}
		bool operator == (ActionChoiceClass const & rvalue) const {return(Action == rvalue.Action);}
		bool operator != (ActionChoiceClass const & rvalue) const {return(Action != rvalue.Action);}
		bool operator > (ActionChoiceClass const & rvalue) const {return(stricmp(Description(), rvalue.Description()) > 0);}
		bool operator < (ActionChoiceClass const & rvalue) const {return(stricmp(Description(), rvalue.Description()) < 0);}
		bool operator <= (ActionChoiceClass const & rvalue) const {return(Action == rvalue.Action || stricmp(Description(), rvalue.Description()) < 0);}
		bool operator >= (ActionChoiceClass const & rvalue) const {return(Action == rvalue.Action || stricmp(Description(), rvalue.Description()) > 0);}
		char const * Description(void) const {return(Name_From_Action(Action));}
		void Draw_It(int index, int x, int y, int width, int height, bool selected, TextPrintType flags) const;

		TActionType Action;
};


extern ActionChoiceClass ActionChoices[TACTION_COUNT];

#endif
