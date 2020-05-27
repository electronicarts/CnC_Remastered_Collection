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

/* $Header: /CounterStrike/THEME.CPP 3     3/11/97 4:03p Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : THEME.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 14, 1994                                              *
 *                                                                                             *
 *                  Last Update : August 12, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ThemeClass::AI -- Process the theme engine and restart songs.                             *
 *   ThemeClass::Base_Name -- Fetches the base filename for the theme specified.               *
 *   ThemeClass::From_Name -- Determines theme number from specified name.                     *
 *   ThemeClass::Full_Name -- Retrieves the full score name.                                   *
 *   ThemeClass::Is_Allowed -- Checks to see if the specified theme is legal.                  *
 *   ThemeClass::Next_Song -- Calculates the next song number to play.                         *
 *   ThemeClass::Play_Song -- Starts the specified song play NOW.                              *
 *   ThemeClass::Queue_Song -- Queues the song to the play queue.                              *
 *   ThemeClass::Scan -- Scans all scores for availability.                                    *
 *   ThemeClass::Set_Theme_Data -- Set the theme data for scenario and owner.                  *
 *   ThemeClass::Still_Playing -- Determines if music is still playing.                        *
 *   ThemeClass::Stop -- Stops the current theme from playing.                                 *
 *   ThemeClass::ThemeClass -- Default constructor for the theme manager class.                *
 *   ThemeClass::Theme_File_Name -- Constructs a filename for the specified theme.             *
 *   ThemeClass::Track_Length -- Calculates the length of the song (in seconds).               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"theme.h"

#ifndef WIN32
extern short StreamLowImpact;
#endif  //WIN32

/*
**	These are the actual filename list for the theme sample files.
*/
ThemeClass::ThemeControl ThemeClass::_themes[THEME_COUNT] = {
	{"BIGF226M",	TXT_THEME_BIGF,		0,		307,	true,		false,	true,	HOUSEF_ALLIES},
	{"CRUS226M",	TXT_THEME_CRUS,		0,		222,	true,		false,	true,	HOUSEF_SOVIET},
	{"FAC1226M",	TXT_THEME_FAC1,		0,		271,	true,		false,	true,	HOUSEF_ALLIES},
	{"FAC2226M",	TXT_THEME_FAC2,		0,		328,	true,		false,	true,	HOUSEF_SOVIET},
	{"HELL226M",	TXT_THEME_HELL,		0,		375,	true,		false,	true,	HOUSEF_ALLIES},
	{"RUN1226M",	TXT_THEME_RUN1,		0,		312,	true,		false,	true,	HOUSEF_SOVIET},
	{"SMSH226M",	TXT_THEME_SMSH,		0,		272,	true,		false,	true,	HOUSEF_ALLIES},
	{"TREN226M",	TXT_THEME_TREN,		0,		312,	true,		false,	true,	HOUSEF_SOVIET},
	{"WORK226M",	TXT_THEME_WORK,		0,		277,	true,		false,	true,	HOUSEF_ALLIES},
	{"AWAIT",		TXT_THEME_AWAIT,		0,		259,	true,		false,	true,	HOUSEF_ALLIES},
	{"DENSE_R",		TXT_THEME_DENSE_R,	0,		294,	true,		false,	true,	HOUSEF_ALLIES},
	{"FOGGER1A",	TXT_THEME_FOGGER1A,	0,		297,	true,		false,	true,	HOUSEF_ALLIES},
	{"MUD1A",		TXT_THEME_MUD1A,		0,		280,	true,		false,	true,	HOUSEF_ALLIES},
	{"RADIO2",		TXT_THEME_RADIO2,		0,		237,	true,		false,	true,	HOUSEF_ALLIES},
	{"ROLLOUT",		TXT_THEME_ROLLOUT,	0,		227,	true,		false,	true,	HOUSEF_ALLIES},
	{"SNAKE",		TXT_THEME_SNAKE,		0,		277,	true,		false,	true,	HOUSEF_ALLIES},
	{"TERMINAT",	TXT_THEME_TERMINAT,	0,		310,	true,		false,	true,	HOUSEF_ALLIES},
	{"TWIN",			TXT_THEME_TWIN,		0,		229,	true,		false,	true,	HOUSEF_ALLIES},
	{"VECTOR1A",	TXT_THEME_VECTOR1A,	0,		252,	true,		false,	true,	HOUSEF_ALLIES},
  	{"MAP",			TXT_THEME_MAP,			0,		63,	false,	true,		true,	HOUSEF_NONE},
	{"SCORE",		TXT_THEME_SCORE,		0,		106,	false,	true,		true,	HOUSEF_NONE},
	{"INTRO",		TXT_THEME_INTRO,		0,		205,	false,	true,		true,	HOUSEF_NONE},
	{"CREDITS",		TXT_THEME_CREDITS,	0,		163,	false,	true,		true,	HOUSEF_NONE},

	{"2ND_HAND",	TXT_THEME_2ND_HAND,	0,		268,	true,		false,	true,	HOUSEF_ALLIES|HOUSEF_SPAIN},
	{"ARAZOID",		TXT_THEME_ARAZOID,	0,		257,	true,		false,	true,	HOUSEF_SOVIET|HOUSEF_SPAIN},
	{"BACKSTAB",	TXT_THEME_BACKSTAB,	0,		278,	true,		false,	true,	HOUSEF_ALLIES|HOUSEF_SPAIN},
	{"CHAOS2",		TXT_THEME_CHAOS2,		0,		250,	true,		false,	true,	HOUSEF_SOVIET|HOUSEF_SPAIN},
	{"SHUT_IT",		TXT_THEME_SHUT_IT,	0,		261,	true,		false,	true,	HOUSEF_ALLIES|HOUSEF_SPAIN},
	{"TWINMIX1",	TXT_THEME_TWINMIX1,	0,		222,	true,		false,	true,	HOUSEF_SOVIET|HOUSEF_SPAIN},
	{"UNDER3",		TXT_THEME_UNDER3,		0,		246,	true,		false,	true,	HOUSEF_ALLIES|HOUSEF_SPAIN},
	{"VR2",			TXT_THEME_VR2,			0,		255,	true,		false,	true,	HOUSEF_SOVIET|HOUSEF_SPAIN},
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	{"BOG",			TXT_THEME_BOG,			0,		212,	true,		false,	true,	HOUSEF_ALLIES|HOUSEF_SPAIN},
	{"FLOAT_V2",	TXT_THEME_FLOAT_V2,	0,		274,	true,		false,	true,	HOUSEF_SOVIET|HOUSEF_SPAIN},
	{"GLOOM",		TXT_THEME_GLOOM,		0,		236,	true,		false,	true,	HOUSEF_ALLIES|HOUSEF_SPAIN},
	{"GRNDWIRE",	TXT_THEME_GRNDWIRE,	0,		228,	true,		false,	true,	HOUSEF_SOVIET|HOUSEF_SPAIN},
	{"RPT",			TXT_THEME_RPT,			0,		275,	true,		false,	true,	HOUSEF_ALLIES|HOUSEF_SPAIN},
	{"SEARCH",		TXT_THEME_SEARCH,		0,		276,	true,		false,	true,	HOUSEF_SOVIET|HOUSEF_SPAIN},
	{"TRACTION",	TXT_THEME_TRACTION,	0,		237,	true,		false,	true,	HOUSEF_ALLIES|HOUSEF_SPAIN},
	{"WASTELND",	TXT_THEME_WASTELND,	0,		242,	true,		false,	true,	HOUSEF_SOVIET|HOUSEF_SPAIN},
#endif
};


/***********************************************************************************************
 * ThemeClass::Base_Name -- Fetches the base filename for the theme specified.                 *
 *                                                                                             *
 *    This routine is used to retrieve a pointer to the base filename for the theme            *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   theme -- The theme number to convert into a base filename.                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the base filename for the theme specified. If the        *
 *          theme number is invalid, then a pointer to "No Theme" is returned instead.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * ThemeClass::Base_Name(ThemeType theme) const
{
	if (theme != THEME_NONE) {
		return(_themes[theme].Name);
	}
	return("No theme");
}


/***********************************************************************************************
 * ThemeClass::ThemeClass -- Default constructor for the theme manager class.                  *
 *                                                                                             *
 *    This is the default constructor for the theme class object.                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ThemeClass::ThemeClass(void) :
	Current(-1),
	Score(THEME_NONE),
	Pending(THEME_NONE)
{
}


/***********************************************************************************************
 * ThemeClass::Full_Name -- Retrieves the full score name.                                     *
 *                                                                                             *
 *    This routine will fetch and return with a pointer to the full name of the theme          *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   theme -- The theme to fetch the full name for.                                     *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the full name for this score. This pointer may point to  *
 *          EMS memory.                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * ThemeClass::Full_Name(ThemeType theme) const
{
	if (theme >= THEME_FIRST && theme < THEME_COUNT) {
		return(Text_String(_themes[theme].Fullname));
	}
	return(NULL);
}


/***********************************************************************************************
 * ThemeClass::AI -- Process the theme engine and restart songs.                               *
 *                                                                                             *
 *    This is a maintenance function that will restart an appropriate theme if the current one *
 *    has finished. This routine should be called frequently.                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *   01/23/1995 JLB : Picks new song just as it is about to play it.                           *
 *=============================================================================================*/
void ThemeClass::AI(void)
{
	if (SampleType && !Debug_Quiet) {
		if (ScoresPresent && Options.ScoreVolume != 0 && !Still_Playing() && Pending != THEME_NONE) {

			/*
			**	If the pending song needs to be picked, then pick it now.
			*/
			if (Pending == THEME_PICK_ANOTHER) {
				Pending = Next_Song(Score);
			}

			/*
			**	Start the song playing and then flag it so that a new song will
			**	be picked when this one ends.
			*/
			Play_Song(Pending);
			Pending = THEME_PICK_ANOTHER;
		}
		Sound_Callback();
	}
}


/***********************************************************************************************
 * ThemeClass::Next_Song -- Calculates the next song number to play.                           *
 *                                                                                             *
 *    use this routine to figure out what song number to play. It examines the option settings *
 *    for repeat and shuffle so that it can return the correct value.                          *
 *                                                                                             *
 * INPUT:   theme -- The origin (last) index. The new value is related to this for all but     *
 *                   the shuffling method of play.                                             *
 *                                                                                             *
 * OUTPUT:  Returns with the song number for the next song to play.                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *   01/19/1995 JLB : Will not play the same song twice when in shuffle mode.                  *
 *=============================================================================================*/
ThemeType ThemeClass::Next_Song(ThemeType theme) const
{
	if (theme == THEME_NONE || theme == THEME_PICK_ANOTHER || (theme != THEME_QUIET && !_themes[theme].Repeat && !Options.IsScoreRepeat)) {
		if (Options.IsScoreShuffle) {

			/*
			**	Shuffle the theme, but never pick the same theme that was just
			**	playing.
			*/
			ThemeType newtheme;
			do {
				newtheme = Sim_Random_Pick(THEME_FIRST, THEME_LAST);
			} while (newtheme == theme || !Is_Allowed(newtheme));
			theme = newtheme;

		} else {

			/*
			**	Sequential score playing.
			*/
			do {
				theme++;
				if (theme > THEME_LAST) {
					theme = THEME_FIRST;
				}
			} while (!Is_Allowed(theme));
		}
	}
	return(theme);
}


/***********************************************************************************************
 * ThemeClass::Queue_Song -- Queues the song to the play queue.                                *
 *                                                                                             *
 *    This routine will cause the current song to fade and the specified song to start. This   *
 *    is the normal and friendly method of changing the current song.                          *
 *                                                                                             *
 * INPUT:   theme -- The song to start playing. If -1 is passed in, then just the current song.*
 *                   is faded.                                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ThemeClass::Queue_Song(ThemeType theme)
{
	/*
	**	If there is no score file present, then abort.
	*/
	if (!ScoresPresent) return;

	/*
	**	If there is no sound driver or sounds have been specifically
	**	turned off, then abort.
	*/
	if (SampleType == 0 || Debug_Quiet) return;

	/*
	**	If the current score volumne is set to silent, then there is no need to play the
	**	specified theme.
	*/
	if (Options.ScoreVolume == 0) return;

	/*
	**	If the pending theme is available to be set and the specified theme is valid, then
	**	set the queued theme accordingly.
	*/
	if (Pending == THEME_NONE || Pending == THEME_PICK_ANOTHER || theme == THEME_NONE || theme == THEME_QUIET) {
		Pending = theme;
		if (Still_Playing()) {
			Fade_Sample(Current, THEME_DELAY);
		}
	}
}


/***********************************************************************************************
 * ThemeClass::Play_Song -- Starts the specified song play NOW.                                *
 *                                                                                             *
 *    This routine is used to start the specified theme playing right now. If there is already *
 *    a theme playing, it is cut short so that this one may start.                             *
 *                                                                                             *
 * INPUT:   theme -- The theme number to start playing.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the sample play handle.                                               *
 *                                                                                             *
 * WARNINGS:   This cuts off any current song in a abrupt manner. Only use this routine when   *
 *             necessary.                                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ThemeClass::Play_Song(ThemeType theme)
{
	if (ScoresPresent && SampleType && !Debug_Quiet && Options.ScoreVolume != 0) {
		Stop();
		Score = theme;
		if (theme != THEME_NONE && theme != THEME_QUIET) {
			//PG StreamLowImpact = true;
			Current = File_Stream_Sample_Vol(Theme_File_Name(theme), 0xFF, true);
			//PG StreamLowImpact = false;
		}
	}
	return(Current);
}


/***********************************************************************************************
 * ThemeClass::Theme_File_Name -- Constructs a filename for the specified theme.               *
 *                                                                                             *
 *    This routine will construct (into a static buffer) a filename that matches the theme     *
 *    number specified. This constructed filename is returned as a pointer. The filename will  *
 *    remain valid until the next call to this routine.                                        *
 *                                                                                             *
 * INPUT:   theme -- The theme number to convert to a filename.                                *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the constructed filename for the specified theme number. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *   05/09/1995 JLB : Theme variation support.                                                 *
 *=============================================================================================*/
char const * ThemeClass::Theme_File_Name(ThemeType theme)
{
	static char name[_MAX_FNAME+_MAX_EXT];

	if (theme >= THEME_FIRST && theme < THEME_COUNT) {
		_makepath(name, NULL, NULL, _themes[theme].Name, ".AUD");
		return((char const *)(&name[0]));
	}

	return("");
}


/***********************************************************************************************
 * ThemeClass::Track_Length -- Calculates the length of the song (in seconds).                 *
 *                                                                                             *
 *    Use this routine to calculate the length of the song. The length is determined by        *
 *    reading the header of the song and dividing the sample rate into the sample length.      *
 *                                                                                             *
 * INPUT:   theme -- The song number to examine to find its length.                            *
 *                                                                                             *
 * OUTPUT:  Returns with the length of the specified theme. This length is in the form of      *
 *          seconds.                                                                           *
 *                                                                                             *
 * WARNINGS:   This routine goes to disk to fetch this information. Don't call frivolously.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ThemeClass::Track_Length(ThemeType theme) const
{
	if ((unsigned)theme < THEME_COUNT) {
		return(_themes[theme].Duration);
	}
	return(0);
}


/***********************************************************************************************
 * ThemeClass::Stop -- Stops the current theme from playing.                                   *
 *                                                                                             *
 *    Use this routine to stop the current theme. After this routine is called, no more music  *
 *    will play until the Start() function is called.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void ThemeClass::Stop(void)
{
	if (ScoresPresent && SampleType && !Debug_Quiet && Current != -1) {
		Stop_Sample(Current);
		Current = -1;
		Score = THEME_NONE;
		Pending = THEME_NONE;
	}
}


void ThemeClass::Suspend(void)
{
	if (ScoresPresent && SampleType && !Debug_Quiet && Current != -1) {
		Stop_Sample(Current);
		Current = -1;
		Pending = Score;
		Score = THEME_NONE;
	}
}


/***********************************************************************************************
 * ThemeClass::Still_Playing -- Determines if music is still playing.                          *
 *                                                                                             *
 *    Use this routine to determine if music is still playing.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the music still audible?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/20/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int ThemeClass::Still_Playing(void) const
{
	if (ScoresPresent && SampleType && Current != -1 && !Debug_Quiet) {
		return(Sample_Status(Current));
	}
	return(false);
}


/***********************************************************************************************
 * ThemeClass::Is_Allowed -- Checks to see if the specified theme is legal.                    *
 *                                                                                             *
 *    Use this routine to determine if a theme is allowed to be played. A theme is not allowed *
 *    if the scenario is too early for that score, or the score only is allowed in special     *
 *    cases.                                                                                   *
 *                                                                                             *
 * INPUT:   index -- The score the check to see if it is allowed to play.                      *
 *                                                                                             *
 * OUTPUT:  Is the specified score allowed to play in the normal score playlist?               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/09/1995 JLB : Created.                                                                 *
 *   07/04/1996 JLB : Handles alternate playlist checking.                                     *
 *=============================================================================================*/
bool ThemeClass::Is_Allowed(ThemeType index) const
{
	if ((unsigned)index >= THEME_COUNT) return(true);

	/*
	**	If the theme is not present, then it certainly isn't allowed.
	*/
	if (!_themes[index].Available) return(false);

	/*
	**	Only normal themes (playable during battle) are considered allowed.
	*/
	if (!_themes[index].Normal) return(false);

	/*
	**	If the theme is not allowed to be played by the player's house, then don't allow
	**	it. If the player's house hasn't yet been determined, then presume this test
	**	passes.
	*/
	if (PlayerPtr != NULL && ((1 << PlayerPtr->ActLike) & _themes[index].Owner) == 0) return(false);

	/*
	**	If the scenario doesn't allow this theme yet, then return the failure flag. The
	**	scenario check only makes sense for solo play.
	*/
	if (Session.Type == GAME_NORMAL && Scen.Scenario < _themes[index].Scenario) return(false);

	/*
	**	Since all tests passed, return with the "is allowed" flag.
	*/
	return(true);
}


/***********************************************************************************************
 * ThemeClass::From_Name -- Determines theme number from specified name.                       *
 *                                                                                             *
 *    Use this routine to convert a name (either the base filename of the theme, or a partial  *
 *    substring of the full name) into the matching ThemeType value. Typical use of this is    *
 *    when parsing the INI file for theme control values.                                      *
 *                                                                                             *
 * INPUT:   name  -- Pointer to base filename of theme or a partial substring of the full      *
 *                   theme name.                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the matching theme number. If no match could be found, then           *
 *          THEME_NONE is returned.                                                            *
 *                                                                                             *
 * WARNINGS:   If a filename is specified the comparison is case insensitive. When scanning    *
 *             the full theme name, the comparison is case sensitive.                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ThemeType ThemeClass::From_Name(char const * name) const
{
	if (name && strlen(name) > 0) {
		/*
		**	First search for an exact name match with the filename
		**	of the theme. This is guaranteed to be unique.
		*/
		for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; theme++) {
			if (stricmp(_themes[theme].Name, name) == 0) {
				return(theme);
			}
		}

		/*
		**	If the filename scan failed to find a match, then scan for
		**	a substring within the full name of the score. This might
		**	yield a match, but is not guaranteed to be unique.
		*/
		for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; theme++) {
			if (strstr(Text_String(_themes[theme].Fullname), name) != NULL) {
				return(theme);
			}
		}
	}

	return(THEME_NONE);
}


/***********************************************************************************************
 * ThemeClass::Scan -- Scans all scores for availability.                                      *
 *                                                                                             *
 *    This routine should be called whenever a score mixfile is registered. It will scan       *
 *    to see if any score is unavailable. If this is the case, then the score will be so       *
 *    flagged in order not to appear on the play list. This condition is likely to occur       *
 *    when expansion mission disks contain a different score mix than the release version.     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void ThemeClass::Scan(void)
{
	for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; theme++) {
		_themes[theme].Available = CCFileClass(Theme_File_Name(theme)).Is_Available();
	}
}



/***********************************************************************************************
 * ThemeClass::Set_Theme_Data -- Set the theme data for scenario and owner.                    *
 *                                                                                             *
 *    This is an override function used to set a particular theme's initial scenario and       *
 *    owner values. Typically, the rules control file will be the source of calling this       *
 *    routine.                                                                                 *
 *                                                                                             *
 * INPUT:   theme    -- The theme to set these override values for.                            *
 *                                                                                             *
 *          scenario -- The first scenario when this theme becomes available on the play list. *
 *                                                                                             *
 *          owners   -- A bitfield representing the owners allowed to play this song.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void ThemeClass::Set_Theme_Data(ThemeType theme, int scenario, int owners)
{
	if (theme != THEME_NONE) {
		_themes[theme].Normal = true;
		_themes[theme].Scenario = scenario;
		_themes[theme].Owner = owners;
	}
}

