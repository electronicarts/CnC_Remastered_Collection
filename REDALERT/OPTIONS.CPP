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

/* $Header: /CounterStrike/OPTIONS.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OPTIONS.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 8, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 30, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   OptionsClass::Adjust_Palette -- Adjusts the palette according to the settings specified.  *
 *   OptionsClass::Fixup_Palette -- Adjusts the real palette to match the palette sliders.     *
 *   OptionsClass::Get_Brightness -- Fetches the current brightness setting.                   *
 *   OptionsClass::Get_Contrast -- Gets the current contrast setting.                          *
 *   OptionsClass::Get_Game_Speed -- Fetches the current game speed setting.                   *
 *   OptionsClass::Get_Saturation -- Fetches the current color setting.                        *
 *   OptionsClass::Get_Scroll_Rate -- Fetches the current scroll rate setting.                 *
 *   OptionsClass::Get_Tint -- Fetches the current tint setting.                               *
 *   OptionsClass::Load_Settings -- reads options settings from the INI file                   *
 *   OptionsClass::Normalize_Delay -- Normalizes delay factor to keep rate constant.           *
 *   OptionsClass::Normalize_Volume -- Convert to a real volume value.                         *
 *   OptionsClass::One_Time -- This performs any one time initialization for the options class.*
 *   OptionsClass::OptionsClass -- The default constructor for the options class.              *
 *   OptionsClass::Process -- Handles all the options graphic interface.                       *
 *   OptionsClass::Save_Settings -- writes options settings to the INI file                    *
 *   OptionsClass::Set -- Sets options based on current settings                               *
 *   OptionsClass::Set_Brightness -- Sets the brightness level to that specified.              *
 *   OptionsClass::Set_Contrast -- Sets the contrast to the value specified.                   *
 *   OptionsClass::Set_Game_Speed -- Sets the game speed as specified.                         *
 *   OptionsClass::Set_Repeat -- Controls the score repeat option.                             *
 *   OptionsClass::Set_Saturation -- Sets the color to the value specified.                    *
 *   OptionsClass::Set_Score_Volume -- Sets the global score volume to that specified.         *
 *   OptionsClass::Set_Scroll_Rate -- Sets the scroll rate as specified.                       *
 *   OptionsClass::Set_Shuffle -- Controls the play shuffle setting.                           *
 *   OptionsClass::Set_Sound_Volume -- Sets the sound effects volume level.                    *
 *   OptionsClass::Set_Tint -- Sets the tint setting.                                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"options.h"


#ifdef WIN32
char const * const OptionsClass::HotkeyName = "WinHotkeys";
#else
char const * const OptionsClass::HotkeyName = "DOSHotkeys";
#endif


/***********************************************************************************************
 * OptionsClass::OptionsClass -- The default constructor for the options class.                *
 *                                                                                             *
 *    This is the constructor for the options class. It handles setting up all the globals     *
 *    necessary for the options. This includes setting them to their default state.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
OptionsClass::OptionsClass(void) :
	GameSpeed(3),
	ScrollRate(3),
#ifdef WIN32
	Volume(".40"),			// was .295
	ScoreVolume(".25"),
#ifdef FIXIT_VERSION_3
	MultiScoreVolume("0"),
#endif
#else
	Volume(".8"),
	ScoreVolume(".6"),
#endif
	Brightness(fixed::_1_2),
	Tint(fixed::_1_2),
	Saturation(fixed::_1_2),
	Contrast(fixed::_1_2),
	AutoScroll(true),
	IsScoreRepeat(false),
	IsScoreShuffle(false),
	IsPaletteScroll(true),

	KeyForceMove1(KN_LALT),
	KeyForceMove2(KN_RALT),
	KeyForceAttack1(KN_LCTRL),
	KeyForceAttack2(KN_RCTRL),
	KeySelect1(KN_LSHIFT),
	KeySelect2(KN_RSHIFT),
	KeyScatter(KN_X),
	KeyStop(KN_S),
	KeyGuard(KN_G),
	KeyNext(KN_N),
	KeyPrevious(KN_B),
	KeyFormation(KN_F),
	KeyHome1(KN_HOME),
	KeyHome2(KN_E_HOME),
	KeyBase(KN_H),
	KeyResign(KN_R),
	KeyAlliance(KN_A),
	KeyBookmark1(KN_F9),
	KeyBookmark2(KN_F10),
	KeyBookmark3(KN_F11),
	KeyBookmark4(KN_F12),
	KeySelectView(KN_E),
	KeyRepair(KN_T),
	KeyRepairOn(KN_NONE),
	KeyRepairOff(KN_NONE),
	KeySell(KN_Y),
	KeySellOn(KN_NONE),
	KeySellOff(KN_NONE),
	KeyMap(KN_U),
	KeySidebarUp(KN_UP),
	KeySidebarDown(KN_DOWN),
	KeyOption1(KN_ESC),
	KeyOption2(KN_SPACE),
	KeyScrollLeft(KN_NONE),
	KeyScrollRight(KN_NONE),
	KeyScrollUp(KN_NONE),
	KeyScrollDown(KN_NONE),
	KeyQueueMove1(KN_Q),
	KeyQueueMove2(KN_Q),
	KeyTeam1(KN_1),
	KeyTeam2(KN_2),
	KeyTeam3(KN_3),
	KeyTeam4(KN_4),
	KeyTeam5(KN_5),
	KeyTeam6(KN_6),
	KeyTeam7(KN_7),
	KeyTeam8(KN_8),
	KeyTeam9(KN_9),
	KeyTeam10(KN_0)
{
}


/***********************************************************************************************
 * OptionsClass::One_Time -- This performs any one time initialization for the options class.  *
 *                                                                                             *
 *    This routine should be called only once and it will perform any initializations for the  *
 *    options class that is needed. This may include things like file loading and memory       *
 *    allocation.                                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once.                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::One_Time(void)
{
	Set_Score_Vol(ScoreVolume * 256);
}


/***********************************************************************************************
 * OptionsClass::Process -- Handles all the options graphic interface.                         *
 *                                                                                             *
 *    This routine is the main control for the visual representation of the options            *
 *    screen. It handles the visual overlay and the player input.                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Process(void)
{
}


/***********************************************************************************************
 * OptionsClass::Set_Shuffle -- Controls the play shuffle setting.                             *
 *                                                                                             *
 *    This routine will control the score shuffle flag. The setting to use is provided as      *
 *    a parameter. When shuffling is on, the score play order is scrambled.                    *
 *                                                                                             *
 * INPUT:   on -- Should the shuffle option be activated?                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Shuffle(int on)
{
	IsScoreShuffle = on;
}


/***********************************************************************************************
 * OptionsClass::Set_Repeat -- Controls the score repeat option.                               *
 *                                                                                             *
 *    This routine is used to control whether scores repeat or not. The setting to use for     *
 *    the repeat flag is provided as a parameter.                                              *
 *                                                                                             *
 * INPUT:   on -- Should the scores repeat?                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Repeat(int on)
{
	IsScoreRepeat = on;
}


/***********************************************************************************************
 * OptionsClass::Set_Score_Volume -- Sets the global score volume to that specified.           *
 *                                                                                             *
 *    This routine will set the global score volume to the value specified. The value ranges   *
 *    from zero to 255.                                                                        *
 *                                                                                             *
 * INPUT:   volume   -- The new volume setting to use for scores.                              *
 *                                                                                             *
 *          feedback -- Should a feedback sound effect be generated?                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Score_Volume(fixed volume, bool feedback)
{
	ScoreVolume = Sub_Saturate(volume, 1);
	Set_Score_Vol(ScoreVolume * 256);
	if (feedback && !Theme.Still_Playing()) {
		Sound_Effect(VOC_BEEP, ScoreVolume);
	}
}


/***********************************************************************************************
 * OptionsClass::Set_Sound_Volume -- Sets the sound effects volume level.                      *
 *                                                                                             *
 *    This routine will set the sound effect volume level as indicated. It can generate a      *
 *    sound effect for feedback purposes if desired. The volume setting can range from zero    *
 *    to 255. The value of 255 is the loudest.                                                 *
 *                                                                                             *
 * INPUT:   volume   -- The volume setting to use for the new value. 0 to 255.                 *
 *                                                                                             *
 *          feedback -- Should a feedback sound effect be generated?                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Sound_Volume(fixed volume, bool feedback)
{
	Volume = Sub_Saturate(volume, 1);
	if (feedback) {
		Sound_Effect(VOC_BEEP);
	}
}


/***********************************************************************************************
 * OptionsClass::Set_Brightness -- Sets the brightness level to that specified.                *
 *                                                                                             *
 *    This routine will set the current brightness level to the value specified. This value    *
 *    can range from zero to 255, with 128 being the normal (default) brightness level.        *
 *                                                                                             *
 * INPUT:   brightness  -- The brightness level to set as current.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Brightness(fixed brightness)
{
	Brightness = fixed::_1_4 + (fixed::_1_2 * brightness);
	Adjust_Palette(OriginalPalette, InGamePalette, Brightness, Saturation, Tint, Contrast);
	InGamePalette.Set();
}


/***********************************************************************************************
 * OptionsClass::Get_Brightness -- Fetches the current brightness setting.                     *
 *                                                                                             *
 *    This routine will fetch the current setting for the brightness level. The value ranges   *
 *    from zero to 255, with 128 being the normal (default) value.                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current brightness setting.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
fixed OptionsClass::Get_Brightness(void) const
{
	return((Brightness - fixed::_1_4) / fixed::_1_2);
}


/***********************************************************************************************
 * OptionsClass::Set_Saturation -- Sets the color to the value specified.                      *
 *                                                                                             *
 *    This routine will set the color value to that specified. The value specified can range   *
 *    from zero to 255. The value of 128 is the normal default color setting.                  *
 *                                                                                             *
 * INPUT:   color -- The new color value to set as current.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Saturation(fixed color)
{
	Saturation = color;
	Adjust_Palette(OriginalPalette, InGamePalette, Brightness, Saturation, Tint, Contrast);
	InGamePalette.Set();
}


/***********************************************************************************************
 * OptionsClass::Get_Saturation -- Fetches the current color setting.                          *
 *                                                                                             *
 *    This routine will fetch the current color setting. This value ranges from zero to        *
 *    255.                                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current color setting. The value of 128 is the normal (default)   *
 *          color setting.                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
fixed OptionsClass::Get_Saturation(void) const
{
	return(Saturation);
}


/***********************************************************************************************
 * OptionsClass::Set_Contrast -- Sets the contrast to the value specified.                     *
 *                                                                                             *
 *    This routine will set the contrast to the setting specified. This setting ranges from    *
 *    zero to 255. The value o 128 is the normal default value.                                *
 *                                                                                             *
 * INPUT:   contrast -- The contrast setting to make as the current setting.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Contrast(fixed contrast)
{
	Contrast = fixed::_1_4 + (fixed::_1_2 * contrast);
	Adjust_Palette(OriginalPalette, InGamePalette, Brightness, Saturation, Tint, Contrast);
	InGamePalette.Set();
}


/***********************************************************************************************
 * OptionsClass::Get_Contrast -- Gets the current contrast setting.                            *
 *                                                                                             *
 *    This routine will get the current contrast setting. The value returned is in the range   *
 *    of zero to 255.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the current contrast setting. A setting of 128 is the normal default value.*
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
fixed OptionsClass::Get_Contrast(void) const
{
	return((Contrast - fixed::_1_4) / fixed::_1_2);
}


/***********************************************************************************************
 * OptionsClass::Set_Tint -- Sets the tint setting.                                            *
 *                                                                                             *
 *    This routine will change the current tint setting according to the value specified.      *
 *                                                                                             *
 * INPUT:   tint  -- The desired tint setting. This value ranges from zero to 255.             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The value of 128 is the default (normal) tint setting.                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Tint(fixed tint)
{
	Tint = tint;
	Adjust_Palette(OriginalPalette, InGamePalette, Brightness, Saturation, Tint, Contrast);
	InGamePalette.Set();
}


/***********************************************************************************************
 * OptionsClass::Get_Tint -- Fetches the current tint setting.                                 *
 *                                                                                             *
 *    This fetches the current tint setting. The value is returned as a number between         *
 *    zero and 255. This has been adjusted for the valid range allowed.                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current tint setting. Normal tint setting is 128.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
fixed OptionsClass::Get_Tint(void) const
{
	return(Tint);
}


/***********************************************************************************************
 * OptionsClass::Adjust_Palette -- Adjusts the palette according to the settings specified.    *
 *                                                                                             *
 *    This routine is used to adjust the palette according to the settings provided. It is     *
 *    used by the options class to monkey with the palette.                                    *
 *                                                                                             *
 * INPUT:   oldpal      -- Pointer to the original (unmodified) palette.                       *
 *                                                                                             *
 *          newpal      -- The new palette to create according to the settings provided.       *
 *                                                                                             *
 *          brightness  -- The brightness level (0..255).                                      *
 *                                                                                             *
 *          color       -- The color level (0..255).                                           *
 *                                                                                             *
 *          tint        -- The tint (hue) level (0..255).                                      *
 *                                                                                             *
 *          contrast    -- The contrast level (0..255).                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/21/1994 JLB : Created.                                                                 *
 *   12/02/1995 JLB : Uses palette class objects.                                              *
 *=============================================================================================*/
void OptionsClass::Adjust_Palette(PaletteClass const & oldpal, PaletteClass & newpal, fixed brightness, fixed color, fixed tint, fixed contrast) const
{
	//if (!oldpal || !newpal) return;	Can't be NULL... ST - 5/9/2019

	/*
	**	Adjust for palette.
	*/
	for (int index = 0; index < PaletteClass::COLOR_COUNT; index++) {
		if (index == CC_MOUSE_COLOR) {
			newpal[index] = oldpal[index];
		} else {
			/*
			**	Convert the working palette entry into an HSV format for
			**	manipulation.
			*/
			HSVClass hsv = oldpal[index];

			/*
			**	Adjust contrast by moving the value toward the center according to the
			**	percentage indicated.
			*/
			int temp;
			temp = (hsv.Value_Component() * (brightness * 256)) / 0x80;		// Brightness
			temp = Bound(temp, 0, 0xFF);
			int v = temp;
			temp = (((((int)v) - 0x80) * (contrast * 256)) / 0x80) + 0x80;	// Contrast
			temp = Bound(temp, 0, 0xFF);
			v = temp;
			temp = (hsv.Saturation_Component() * (color * 256)) / 0x80;		// Color
			temp = Bound(temp, 0, 0xFF);
			int s = temp;
			temp = (hsv.Hue_Component() * (tint * 256)) / 0x80;					// Tint
			temp = Bound(temp, 0, 0xFF);
			int h = temp;

			/*
			**	Replace the working palette entry according to the newly calculated
			**	hue, saturation, and value.
			*/
			newpal[index] = HSVClass(h, s, v);
		}
	}
}


/***********************************************************************************************
 * OptionsClass::Load_Settings -- reads options settings from the INI file                     *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *   07/03/1996 JLB : Reworked to use new INI handler.                                         *
 *   07/30/1996 JLB : Handles hotkeys.                                                         *
 *=============================================================================================*/
void OptionsClass::Load_Settings(void)
{
	/*
	**	Create filename and read the file.
	*/
	CCFileClass file(CONFIG_FILE_NAME);
	INIClass ini;
	ini.Load(file);

	/*
	**	Read in the Options values
	*/
	static char const * const OPTIONS = "Options";
	GameSpeed = ini.Get_Int(OPTIONS, "GameSpeed", GameSpeed);
	ScrollRate = ini.Get_Int(OPTIONS, "ScrollRate", ScrollRate);
	Set_Brightness(ini.Get_Fixed(OPTIONS, "Brightness", Brightness));
	Set_Sound_Volume(ini.Get_Fixed(OPTIONS, "Volume", Volume), false);
	Set_Score_Volume(ini.Get_Fixed(OPTIONS, "ScoreVolume", ScoreVolume), false);
#ifdef FIXIT_VERSION_3
	MultiScoreVolume = ini.Get_Fixed(OPTIONS, "MultiplayerScoreVolume", MultiScoreVolume);
#endif
	Set_Contrast(ini.Get_Fixed(OPTIONS, "Contrast", Contrast));
	Set_Saturation(ini.Get_Fixed(OPTIONS, "Color", Saturation));
	Set_Tint(ini.Get_Fixed(OPTIONS, "Tint", Tint));
	AutoScroll = ini.Get_Bool(OPTIONS, "AutoScroll", AutoScroll);
	Set_Repeat(ini.Get_Bool(OPTIONS, "IsScoreRepeat", IsScoreRepeat));
	Set_Shuffle(ini.Get_Bool(OPTIONS, "IsScoreShuffle", IsScoreShuffle));
	SlowPalette = ini.Get_Bool(OPTIONS, "SlowPalette", SlowPalette);
	IsPaletteScroll = ini.Get_Bool(OPTIONS, "PaletteScroll", IsPaletteScroll);

	KeyForceMove1 = (KeyNumType)ini.Get_Int(HotkeyName, "KeyForceMove1", KeyForceMove1);
	KeyForceMove2 = (KeyNumType)ini.Get_Int(HotkeyName, "KeyForceMove2", KeyForceMove2);
	KeyForceAttack1 = (KeyNumType)ini.Get_Int(HotkeyName, "KeyForceAttack1", KeyForceAttack1);
	KeyForceAttack2 = (KeyNumType)ini.Get_Int(HotkeyName, "KeyForceAttack2", KeyForceAttack2);
	KeySelect1 = (KeyNumType)ini.Get_Int(HotkeyName, "KeySelect1", KeySelect1);
	KeySelect2 = (KeyNumType)ini.Get_Int(HotkeyName, "KeySelect2", KeySelect2);
	KeyScatter = (KeyNumType)ini.Get_Int(HotkeyName, "KeyScatter", KeyScatter);
	KeyStop = (KeyNumType)ini.Get_Int(HotkeyName, "KeyStop", KeyStop);
	KeyGuard = (KeyNumType)ini.Get_Int(HotkeyName, "KeyGuard", KeyGuard);
	KeyNext = (KeyNumType)ini.Get_Int(HotkeyName, "KeyNext", KeyNext);
	KeyPrevious = (KeyNumType)ini.Get_Int(HotkeyName, "KeyPrevious", KeyPrevious);
	KeyFormation = (KeyNumType)ini.Get_Int(HotkeyName, "KeyFormation", KeyFormation);
	KeyHome1 = (KeyNumType)ini.Get_Int(HotkeyName, "KeyHome1", KeyHome1);
	KeyHome2 = (KeyNumType)ini.Get_Int(HotkeyName, "KeyHome2", KeyHome2);
	KeyBase = (KeyNumType)ini.Get_Int(HotkeyName, "KeyBase", KeyBase);
	KeyResign = (KeyNumType)ini.Get_Int(HotkeyName, "KeyResign", KeyResign);
	KeyAlliance = (KeyNumType)ini.Get_Int(HotkeyName, "KeyAlliance", KeyAlliance);
	KeyBookmark1 = (KeyNumType)ini.Get_Int(HotkeyName, "KeyBookmark1", KeyBookmark1);
	KeyBookmark2 = (KeyNumType)ini.Get_Int(HotkeyName, "KeyBookmark2", KeyBookmark2);
	KeyBookmark3 = (KeyNumType)ini.Get_Int(HotkeyName, "KeyBookmark3", KeyBookmark3);
	KeyBookmark4 = (KeyNumType)ini.Get_Int(HotkeyName, "KeyBookmark4", KeyBookmark4);
	KeySelectView =  (KeyNumType)ini.Get_Int(HotkeyName, "KeySelectView", KeySelectView);
	KeyRepair =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyRepairToggle", KeyRepair);
	KeyRepairOn = (KeyNumType)ini.Get_Int(HotkeyName, "KeyRepairOn", KeyRepairOn);
	KeyRepairOff = (KeyNumType)ini.Get_Int(HotkeyName, "KeyRepairOff", KeyRepairOff);
	KeySell =  (KeyNumType)ini.Get_Int(HotkeyName, "KeySellToggle", KeySell);
	KeySellOn = (KeyNumType)ini.Get_Int(HotkeyName, "KeySellOn", KeySellOn);
	KeySellOff = (KeyNumType)ini.Get_Int(HotkeyName, "KeySellOff", KeySellOff);
	KeyMap =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyMapToggle", KeyMap);
	KeySidebarUp =  (KeyNumType)ini.Get_Int(HotkeyName, "KeySidebarUp", KeySidebarUp);
	KeySidebarDown =  (KeyNumType)ini.Get_Int(HotkeyName, "KeySidebarDown", KeySidebarDown);
	KeyOption1 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyOption1", KeyOption1);
	KeyOption2 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyOption2", KeyOption2);
	KeyScrollLeft =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyScrollLeft", KeyScrollLeft);
	KeyScrollRight =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyScrollRight", KeyScrollRight);
	KeyScrollUp =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyScrollUp", KeyScrollUp);
	KeyScrollDown =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyScrollDown", KeyScrollDown);
	KeyQueueMove1 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyQueueMove1", KeyQueueMove1);
	KeyQueueMove2 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyQueueMove2", KeyQueueMove2);
	KeyTeam1 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyTeam1", KeyTeam1);
	KeyTeam2 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyTeam2", KeyTeam2);
	KeyTeam3 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyTeam3", KeyTeam3);
	KeyTeam4 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyTeam4", KeyTeam4);
	KeyTeam5 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyTeam5", KeyTeam5);
	KeyTeam6 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyTeam6", KeyTeam6);
	KeyTeam7 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyTeam7", KeyTeam7);
	KeyTeam8 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyTeam8", KeyTeam8);
	KeyTeam9 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyTeam9", KeyTeam9);
	KeyTeam10 =  (KeyNumType)ini.Get_Int(HotkeyName, "KeyTeam10", KeyTeam10);


#ifdef WIN32
	KeyForceMove1 = (KeyNumType)(KeyForceMove1 & ~WWKEY_VK_BIT);
	KeyForceMove2 = (KeyNumType)(KeyForceMove2 & ~WWKEY_VK_BIT);
	KeyForceAttack1 = (KeyNumType)(KeyForceAttack1 & ~WWKEY_VK_BIT);
	KeyForceAttack2 = (KeyNumType)(KeyForceAttack2 & ~WWKEY_VK_BIT);
	KeySelect1 = (KeyNumType)(KeySelect1 & ~WWKEY_VK_BIT);
	KeySelect2 = (KeyNumType)(KeySelect2 & ~WWKEY_VK_BIT);
	KeyScatter = (KeyNumType)(KeyScatter & ~WWKEY_VK_BIT);
	KeyStop = (KeyNumType)(KeyStop & ~WWKEY_VK_BIT);
	KeyGuard = (KeyNumType)(KeyGuard & ~WWKEY_VK_BIT);
	KeyNext = (KeyNumType)(KeyNext & ~WWKEY_VK_BIT);
	KeyPrevious = (KeyNumType)(KeyPrevious & ~WWKEY_VK_BIT);
	KeyFormation = (KeyNumType)(KeyFormation & ~WWKEY_VK_BIT);
	KeyHome1 = (KeyNumType)(KeyHome1 & ~WWKEY_VK_BIT);
	KeyHome2 = (KeyNumType)(KeyHome2 & ~WWKEY_VK_BIT);
	KeyBase = (KeyNumType)(KeyBase & ~WWKEY_VK_BIT);
	KeyResign = (KeyNumType)(KeyResign & ~WWKEY_VK_BIT);
	KeyAlliance = (KeyNumType)(KeyAlliance & ~WWKEY_VK_BIT);
	KeyBookmark1 = (KeyNumType)(KeyBookmark1 & ~WWKEY_VK_BIT);
	KeyBookmark2 = (KeyNumType)(KeyBookmark2 & ~WWKEY_VK_BIT);
	KeyBookmark3 = (KeyNumType)(KeyBookmark3 & ~WWKEY_VK_BIT);
	KeyBookmark4 = (KeyNumType)(KeyBookmark4 & ~WWKEY_VK_BIT);
	KeySelectView = (KeyNumType)(KeySelectView & ~WWKEY_VK_BIT);
	KeyRepair = (KeyNumType)(KeyRepair & ~WWKEY_VK_BIT);
	KeyRepairOn = (KeyNumType)(KeyRepairOn & ~WWKEY_VK_BIT);
	KeyRepairOff = (KeyNumType)(KeyRepairOff & ~WWKEY_VK_BIT);
	KeySell = (KeyNumType)(KeySell & ~WWKEY_VK_BIT);
	KeySellOn = (KeyNumType)(KeySellOn & ~WWKEY_VK_BIT);
	KeySellOff = (KeyNumType)(KeySellOff & ~WWKEY_VK_BIT);
	KeyMap = (KeyNumType)(KeyMap & ~WWKEY_VK_BIT);
	KeySidebarUp = (KeyNumType)(KeySidebarUp & ~WWKEY_VK_BIT);
	KeySidebarDown = (KeyNumType)(KeySidebarDown & ~WWKEY_VK_BIT);
	KeyOption1 = (KeyNumType)(KeyOption1 & ~WWKEY_VK_BIT);
	KeyOption2 = (KeyNumType)(KeyOption2 & ~WWKEY_VK_BIT);
	KeyScrollLeft = (KeyNumType)(KeyScrollLeft & ~WWKEY_VK_BIT);
	KeyScrollRight = (KeyNumType)(KeyScrollRight & ~WWKEY_VK_BIT);
	KeyScrollUp = (KeyNumType)(KeyScrollUp & ~WWKEY_VK_BIT);
	KeyScrollDown = (KeyNumType)(KeyScrollDown & ~WWKEY_VK_BIT);
	KeyQueueMove1 = (KeyNumType)(KeyQueueMove1 & ~WWKEY_VK_BIT);
	KeyQueueMove2 = (KeyNumType)(KeyQueueMove2 & ~WWKEY_VK_BIT);
	KeyTeam1 = (KeyNumType)(KeyTeam1 & ~WWKEY_VK_BIT);
	KeyTeam2 = (KeyNumType)(KeyTeam2 & ~WWKEY_VK_BIT);
	KeyTeam3 = (KeyNumType)(KeyTeam3 & ~WWKEY_VK_BIT);
	KeyTeam4 = (KeyNumType)(KeyTeam4 & ~WWKEY_VK_BIT);
	KeyTeam5 = (KeyNumType)(KeyTeam5 & ~WWKEY_VK_BIT);
	KeyTeam6 = (KeyNumType)(KeyTeam6 & ~WWKEY_VK_BIT);
	KeyTeam7 = (KeyNumType)(KeyTeam7 & ~WWKEY_VK_BIT);
	KeyTeam8 = (KeyNumType)(KeyTeam8 & ~WWKEY_VK_BIT);
	KeyTeam9 = (KeyNumType)(KeyTeam9 & ~WWKEY_VK_BIT);
	KeyTeam10 = (KeyNumType)(KeyTeam10 & ~WWKEY_VK_BIT);
#endif
}


/***********************************************************************************************
 * OptionsClass::Save_Settings -- writes options settings to the INI file                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *   07/03/1996 JLB : Revamped and tightened up.                                               *
 *   07/30/1996 JLB : Handles hotkeys.                                                         *
 *=============================================================================================*/
void OptionsClass::Save_Settings (void)
{
	CCFileClass file(CONFIG_FILE_NAME);
	INIClass ini;

	/*
	**	Load any existing options file because it may contain entries that won't be
	**	explicitly written out by this routine. By preloading the database, these entries
	**	will be carried over.
	*/
	if (file.Is_Available()) {
		ini.Load(file);
	}

	/*
	**	Save Options settings
	*/
	static char const * const OPTIONS = "Options";
	ini.Put_Int(OPTIONS, "GameSpeed", GameSpeed);
	ini.Put_Int(OPTIONS, "ScrollRate", ScrollRate);
	ini.Put_Fixed(OPTIONS, "Brightness", Brightness);
	ini.Put_Fixed(OPTIONS, "Volume", Volume);
#ifdef FIXIT_VERSION_3
	if( Session.Type == GAME_NORMAL )		//	Save only when non-multiplayer.
		ini.Put_Fixed(OPTIONS, "ScoreVolume", ScoreVolume);
#else
	ini.Put_Fixed(OPTIONS, "ScoreVolume", ScoreVolume);
#endif
#ifdef FIXIT_VERSION_3
	ini.Put_Fixed(OPTIONS, "MultiplayerScoreVolume", MultiScoreVolume);
#endif
	ini.Put_Fixed(OPTIONS, "Contrast", Contrast);
	ini.Put_Fixed(OPTIONS, "Color", Saturation);
	ini.Put_Fixed(OPTIONS, "Tint", Tint);
	ini.Put_Bool(OPTIONS, "AutoScroll", AutoScroll);
	ini.Put_Bool(OPTIONS, "IsScoreRepeat", IsScoreRepeat);
	ini.Put_Bool(OPTIONS, "IsScoreShuffle", IsScoreShuffle);
	ini.Put_Bool(OPTIONS, "PaletteScroll", IsPaletteScroll);

	ini.Put_Int(HotkeyName, "KeyForceMove1", KeyForceMove1);
	ini.Put_Int(HotkeyName, "KeyForceMove2", KeyForceMove2);
	ini.Put_Int(HotkeyName, "KeyForceAttack1", KeyForceAttack1);
	ini.Put_Int(HotkeyName, "KeyForceAttack2", KeyForceAttack2);
	ini.Put_Int(HotkeyName, "KeySelect1", KeySelect1);
	ini.Put_Int(HotkeyName, "KeySelect2", KeySelect2);
	ini.Put_Int(HotkeyName, "KeyScatter", KeyScatter);
	ini.Put_Int(HotkeyName, "KeyStop", KeyStop);
	ini.Put_Int(HotkeyName, "KeyGuard", KeyGuard);
	ini.Put_Int(HotkeyName, "KeyNext", KeyNext);
	ini.Put_Int(HotkeyName, "KeyPrevious", KeyPrevious);
	ini.Put_Int(HotkeyName, "KeyFormation", KeyFormation);
	ini.Put_Int(HotkeyName, "KeyHome1", KeyHome1);
	ini.Put_Int(HotkeyName, "KeyHome2", KeyHome2);
	ini.Put_Int(HotkeyName, "KeyBase", KeyBase);
	ini.Put_Int(HotkeyName, "KeyResign", KeyResign);
	ini.Put_Int(HotkeyName, "KeyAlliance", KeyAlliance);
	ini.Put_Int(HotkeyName, "KeyBookmark1", KeyBookmark1);
	ini.Put_Int(HotkeyName, "KeyBookmark2", KeyBookmark2);
	ini.Put_Int(HotkeyName, "KeyBookmark3", KeyBookmark3);
	ini.Put_Int(HotkeyName, "KeyBookmark4", KeyBookmark4);
	ini.Put_Int(HotkeyName, "KeySelectView", KeySelectView);
	ini.Put_Int(HotkeyName, "KeyRepairToggle", KeyRepair);
	ini.Put_Int(HotkeyName, "KeyRepairOn", KeyRepairOn);
	ini.Put_Int(HotkeyName, "KeyRepairOff", KeyRepairOff);
	ini.Put_Int(HotkeyName, "KeySellToggle", KeySell);
	ini.Put_Int(HotkeyName, "KeySellOn", KeySellOn);
	ini.Put_Int(HotkeyName, "KeySellOff", KeySellOff);
	ini.Put_Int(HotkeyName, "KeyMapToggle", KeyMap);
	ini.Put_Int(HotkeyName, "KeySidebarUp", KeySidebarUp);
	ini.Put_Int(HotkeyName, "KeySidebarDown", KeySidebarDown);
	ini.Put_Int(HotkeyName, "KeyOption1", KeyOption1);
	ini.Put_Int(HotkeyName, "KeyOption2", KeyOption2);
	ini.Put_Int(HotkeyName, "KeyScrollLeft", KeyScrollLeft);
	ini.Put_Int(HotkeyName, "KeyScrollRight", KeyScrollRight);
	ini.Put_Int(HotkeyName, "KeyScrollUp", KeyScrollUp);
	ini.Put_Int(HotkeyName, "KeyScrollDown", KeyScrollDown);
	ini.Put_Int(HotkeyName, "KeyQueueMove1", KeyQueueMove1);
	ini.Put_Int(HotkeyName, "KeyQueueMove2", KeyQueueMove2);
	ini.Put_Int(HotkeyName, "KeyTeam1", KeyTeam1);
	ini.Put_Int(HotkeyName, "KeyTeam2", KeyTeam2);
	ini.Put_Int(HotkeyName, "KeyTeam3", KeyTeam3);
	ini.Put_Int(HotkeyName, "KeyTeam4", KeyTeam4);
	ini.Put_Int(HotkeyName, "KeyTeam5", KeyTeam5);
	ini.Put_Int(HotkeyName, "KeyTeam6", KeyTeam6);
	ini.Put_Int(HotkeyName, "KeyTeam7", KeyTeam7);
	ini.Put_Int(HotkeyName, "KeyTeam8", KeyTeam8);
	ini.Put_Int(HotkeyName, "KeyTeam9", KeyTeam9);
	ini.Put_Int(HotkeyName, "KeyTeam10", KeyTeam10);

	/*
	**	Write the INI data out to a file.
	*/
	ini.Save(file);
}


/***********************************************************************************************
 * OptionsClass::Set -- Sets options based on current settings                                 *
 *                                                                                             *
 * Use this routine to adjust the palette or sound settings after a fresh scenario load.       *
 * It assumes the values needed are already loaded into OptionsClass.                          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set(void)
{
	Set_Brightness(Brightness);
	Set_Contrast(Contrast);
	Set_Saturation(Saturation);
	Set_Tint(Tint);
	Set_Sound_Volume(Volume, false);
	Set_Score_Volume(ScoreVolume, false);
	Set_Repeat(IsScoreRepeat);
	Set_Shuffle(IsScoreShuffle);
}


/***********************************************************************************************
 * OptionsClass::Normalize_Delay -- Normalizes delay factor to keep rate constant.             *
 *                                                                                             *
 *    This routine is used to adjust delay factors that MUST be synchronized on all machines   *
 *    but should maintain a speed as close to constant as possible. Building animations are    *
 *    a good example of this.                                                                  *
 *                                                                                             *
 * INPUT:   delay -- The normal delay factor.                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the delay to use that has been modified so that a reasonably constant *
 *          rate will result.                                                                  *
 *                                                                                             *
 * WARNINGS:   This calculation is crude due to the coarse resolution that a 1/15 second timer *
 *             allows.                                                                         *
 *                                                                                             *
 *             Use of this routine ASSUMES that the GameSpeed is synchronized on all machines. *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1995 JLB : Created.                                                                 *
 *   06/30/1995 JLB : Handles low values in a more consistent manner.                          *
 *=============================================================================================*/
int OptionsClass::Normalize_Delay(int delay) const
{
	static int _adjust[][8] = {
		{2,2,1,1,1,1,1,1},
		{3,3,3,2,2,2,1,1},
		{5,4,4,3,3,2,2,1},
		{7,6,5,4,4,4,3,2}
	};
	if (delay) {
		if (delay < 5) {
			delay = _adjust[delay-1][GameSpeed];
		} else {
			delay = ((delay * 8) / (GameSpeed+1));
		}
	}
	return(delay);
}


/***********************************************************************************************
 * OptionsClass::Fixup_Palette -- Adjusts the real palette to match the palette sliders.       *
 *                                                                                             *
 *    This routine is used to adjust the real palette to match the values for the palette      *
 *    control. The adjusted palette is placed into the palette buffer specified.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The palette is not actually set by this routine.                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Fixup_Palette(void) const
{
	Adjust_Palette(OriginalPalette, InGamePalette, Brightness, Saturation, Tint, Contrast);
	CCPalette = InGamePalette;
}


/***********************************************************************************************
 * OptionsClass::Normalize_Volume -- Convert to a real volume value.                           *
 *                                                                                             *
 *    This routine will take a relative volume value and convert it to the real volume value   *
 *    to use. This allows all the game volumes to be corrected to the correct global volume.   *
 *                                                                                             *
 * INPUT:   volume   -- Requested volume level.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the actual volume level to use.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int OptionsClass::Normalize_Volume(int volume) const
{
	return(volume * Volume);
}
