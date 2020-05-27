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

/* $Header:   F:\projects\c&c\vcs\code\options.cpv   2.17   16 Oct 1995 16:51:28   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : June 30, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   OptionsClass::Adjust_Palette -- Adjusts the palette according to the settings specified.  *
 *   OptionsClass::Get_Brightness -- Fetches the current brightness setting.                   *
 *   OptionsClass::Get_Color -- Fetches the current color setting.                             *
 *   OptionsClass::Get_Contrast -- Gets the current contrast setting.                          *
 *   OptionsClass::Get_Game_Speed -- Fetches the current game speed setting.                   *
 *   OptionsClass::Get_Scroll_Rate -- Fetches the current scroll rate setting.                 *
 *   OptionsClass::Get_Tint -- Fetches the current tint setting.                               *
 *   OptionsClass::Load_Settings -- reads options settings from the INI file                   *
 *   OptionsClass::Normalize_Delay -- Normalizes delay factor to keep rate constant.           *
 *   OptionsClass::One_Time -- This performs any one time initialization for the options class.*
 *   OptionsClass::OptionsClass -- The default constructor for the options class.              *
 *   OptionsClass::Process -- Handles all the options graphic interface.                       *
 *   OptionsClass::Save_Settings -- writes options settings to the INI file                    *
 *   OptionsClass::Set -- Sets options based on current settings                               *
 *   OptionsClass::Set_Brightness -- Sets the brightness level to that specified.              *
 *   OptionsClass::Set_Color -- Sets the color to the value specified.                         *
 *   OptionsClass::Set_Contrast -- Sets the contrast to the value specified.                   *
 *   OptionsClass::Set_Game_Speed -- Sets the game speed as specified.                         *
 *   OptionsClass::Set_Repeat -- Controls the score repeat option.                             *
 *   OptionsClass::Set_Score_Volume -- Sets the global score volume to that specified.         *
 *   OptionsClass::Set_Scroll_Rate -- Sets the scroll rate as specified.                       *
 *   OptionsClass::Set_Shuffle -- Controls the play shuffle setting.                           *
 *   OptionsClass::Set_Sound_Volume -- Sets the sound effects volume level.                    *
 *   OptionsClass::Set_Tint -- Sets the tint setting.                                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"options.h"


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
OptionsClass::OptionsClass(void)
{
	GameSpeed = TIMER_SECOND / TICKS_PER_SECOND;
	ScrollRate = TIMER_SECOND / TICKS_PER_SECOND;
	Volume = 0xE0;
	ScoreVolume = 0x90;
	Contrast = 0x80;
	Color = 0x80;
	Contrast = 0x80;
	Tint = 0x80;
	Brightness = 0x80;
	AutoScroll = true;
#if (GERMAN | FRENCH)
	IsDeathAnnounce = true;
#else
	IsDeathAnnounce = false;
#endif
	IsScoreRepeat = false;
	IsScoreShuffle = false;
	IsFreeScroll = false;
}


/***********************************************************************************************
 * OptionsClass::One_Time -- This performs any one time initialization for the options class.  *
 *                                                                                             *
 *    This routine should be called only once and it will perform any inializations for the    *
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
	Set_Score_Vol(ScoreVolume);
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
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Score_Volume(int volume)
{
	volume = Bound(volume, 0, 255);
	ScoreVolume = volume;
	Set_Score_Vol(ScoreVolume);
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
void OptionsClass::Set_Sound_Volume(int volume, int feedback)
{
	volume = Bound(volume, 0, 255);
	Volume = volume;
	if (feedback) {
		Sound_Effect(VOC_BLEEPY3, NULL);
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
void OptionsClass::Set_Brightness(int brightness)
{
	Brightness = 0x40 + Fixed_To_Cardinal(0x80, brightness);
		Adjust_Palette(OriginalPalette, GamePalette, Brightness, Color, Tint, Contrast);
	if (InMainLoop){
		Set_Palette(GamePalette);
	}
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
int OptionsClass::Get_Brightness(void) const
{
	return(Cardinal_To_Fixed(0x80, Brightness-0x40));
}


/***********************************************************************************************
 * OptionsClass::Set_Color -- Sets the color to the value specified.                           *
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
void OptionsClass::Set_Color(int color)
{
	Color = color;
	Adjust_Palette(OriginalPalette, GamePalette, Brightness, Color, Tint, Contrast);
	if (InMainLoop){
		Set_Palette(GamePalette);
	}
}


/***********************************************************************************************
 * OptionsClass::Get_Color -- Fetches the current color setting.                               *
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
int OptionsClass::Get_Color(void) const
{
	return(Color);
}


/***********************************************************************************************
 * OptionsClass::Set_Contrast -- Sets the contrast to the value specified.                     *
 *                                                                                             *
 *    This routine will set the constrast to the setting specified. This setting ranges from   *
 *    zero to 255. The value o 128 is the normal default value.                                *
 *                                                                                             *
 * INPUT:   contrast -- The constrast setting to make as the current setting.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Contrast(int contrast)
{
	Contrast = 0x40 + Fixed_To_Cardinal(0x80, contrast);
	Adjust_Palette(OriginalPalette, GamePalette, Brightness, Color, Tint, Contrast);
	if (InMainLoop){
		Set_Palette(GamePalette);
	}
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
int OptionsClass::Get_Contrast(void) const
{
	return(Cardinal_To_Fixed(0x80, Contrast-0x40));
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
void OptionsClass::Set_Tint(int tint)
{
	Tint = tint;
	Adjust_Palette(OriginalPalette, GamePalette, Brightness, Color, Tint, Contrast);
	if (InMainLoop){
		Set_Palette(GamePalette);
	}
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
int OptionsClass::Get_Tint(void) const
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
 *=============================================================================================*/
void OptionsClass::Adjust_Palette(void *oldpal, void *newpal, unsigned char brightness, unsigned char color, unsigned char tint, unsigned char contrast) const
{
// ST - 1/3/2019 10:49AM
#if (0)	
	int	index;
	unsigned	h,s,v;
	unsigned	r,g,b;

	if (!oldpal || !newpal) return;

	/*
	**	Adjust for palette.
	*/
	for (index = 0; index < 256; index++) {
		if (/*index == LTGREEN ||*/ index == 255) {
			memcpy(&((char*)newpal)[index*3], &((char*)oldpal)[index*3], 3);
		} else {
			r = ((char*)oldpal)[(index*3)+0];
			g = ((char*)oldpal)[(index*3)+1];
			b = ((char*)oldpal)[(index*3)+2];
			Convert_RGB_To_HSV(r, g, b, &h, &s, &v);

			/*
			**	Adjust contrast by moving the value toward the center according to the
			**	percentage indicated.
			*/
			int temp;

			temp = (v * brightness) / 0x80;		// Brightness
			temp = Bound(temp, 0, 0xFF);
			v = temp;
			temp = (((((int)v) - 0x80) * contrast) / 0x80) + 0x80;	// Contrast
			temp = Bound(temp, 0, 0xFF);
			v = temp;
			temp = (s * color) / 0x80;				// Color
			temp = Bound(temp, 0, 0xFF);
			s = temp;
			temp = (h * tint) / 0x80;				// Tint
			temp = Bound(temp, 0, 0xFF);
			h = temp;
			Convert_HSV_To_RGB(h, s, v, &r, &g, &b);
			((char*)newpal)[(index*3)+0] = r;
			((char*)newpal)[(index*3)+1] = g;
			((char*)newpal)[(index*3)+2] = b;
		}
	}
#endif
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
 *=============================================================================================*/
void OptionsClass::Load_Settings (void)
{
	char *buffer;							// INI staging buffer pointer.

	/*
	**	Fetch working pointer to the INI staging buffer. Make sure that the buffer
	**	is cleared out before proceeding.  (Don't use the HidPage for this, since
	**	the HidPage may be needed for various uncompressions during the INI
	**	parsing.)
	*/
	buffer = (char *)_ShapeBuffer;
	memset(buffer, '\0', _ShapeBufferSize);

	/*
	**	Create filename and read the file.
	*/
	CCFileClass file ("CONQUER.INI");
	if (!file.Is_Available()) {
		return;
	} else {
		file.Read(buffer, _ShapeBufferSize-1);
	}
	file.Close();

	/*
	**	Read in the Options values
	*/
	GameSpeed = WWGetPrivateProfileInt("Options", "GameSpeed", 4, buffer);
	ScrollRate = WWGetPrivateProfileInt("Options", "ScrollRate", 4, buffer);
	Set_Brightness(WWGetPrivateProfileInt("Options", "Brightness", 0x80, buffer));
	Set_Sound_Volume(WWGetPrivateProfileInt("Options", "Volume", 0xA0, buffer),false);
	Set_Score_Volume(WWGetPrivateProfileInt("Options", "ScoreVolume", 0xFF, buffer));
	Set_Contrast(WWGetPrivateProfileInt("Options", "Contrast", 0x80, buffer));
	Set_Color(WWGetPrivateProfileInt("Options", "Color", 0x80, buffer));
	Set_Tint(WWGetPrivateProfileInt("Options", "Tint", 0x80, buffer));
	AutoScroll = WWGetPrivateProfileInt("Options", "AutoScroll", 1, buffer);
	Set_Repeat(WWGetPrivateProfileInt("Options", "IsScoreRepeat", 0, buffer));
	Set_Shuffle(WWGetPrivateProfileInt("Options", "IsScoreShuffle", 0, buffer));
	IsDeathAnnounce = WWGetPrivateProfileInt("Options", "DeathAnnounce", 0, buffer);
	IsFreeScroll = WWGetPrivateProfileInt("Options", "FreeScrolling", 0, buffer);
	SlowPalette = WWGetPrivateProfileInt("Options", "SlowPalette", 1, buffer);

	char workbuf[128];

	/*
	**	Check for and possible enable true object names.
	*/
	WWGetPrivateProfileString("Options", "TrueNames", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_TRUENAME) {
		Special.IsNamed = true;
	}

	/*
	**	Enable 6 player games if special flag is detected.
	*/
	WWGetPrivateProfileString("Options", "Players", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_6PLAYER) {
		MPlayerMax = 6;
	}

	/*
	**	Enable three point turning logic as indicated.
	*/
	WWGetPrivateProfileString("Options", "Rotation", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_3POINT) {
		Special.IsThreePoint = true;
	}

	/*
	**	Allow purchase of the helipad separately from the helicopter.
	*/
	WWGetPrivateProfileString("Options", "Helipad", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_HELIPAD) {
		Special.IsSeparate = true;
	}

	/*
	**	Allow the MCV to undeploy rather than sell.
	*/
	WWGetPrivateProfileString("Options", "MCV", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_MCV) {
		Special.IsMCVDeploy = true;
	}

	/*
	**	Allow disabling of building bibs so that tigher building packing can occur.
	*/
	WWGetPrivateProfileString("Options", "Bibs", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_BIB) {
		Special.IsRoad = true;
	}

	/*
	**	Allow targeting of trees without having to hold down the shift key.
	*/
	WWGetPrivateProfileString("Options", "TreeTarget", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_TREETARGET) {
		Special.IsTreeTarget = true;
	}

	/*
	**	Allow infantry to fire while moving. Attacker gets advantage with this flag.
	*/
	WWGetPrivateProfileString("Options", "Combat", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_COMBAT) {
		Special.IsDefenderAdvantage = false;
	}

	/*
	**	Allow custom scores.
	*/
	WWGetPrivateProfileString("Options", "Scores", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_SCORE) {
		Special.IsVariation = true;
	}

	/*
	**	Smarter self defense logic. Tanks will try to run over adjacent infantry. Buildings
	**	will automatically return fire if they are fired upon. Infantry will run from an
	**	incoming explosive (grenade or napalm) or damage that can't be directly addressed.
	*/
	WWGetPrivateProfileString("Options", "CombatIQ", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_IQ) {
		Special.IsSmartDefense = true;
		Special.IsScatter = true;
	}

	/*
	**	Enable the infantry squish marks when run over by a vehicle.
	*/
	WWGetPrivateProfileString("Options", "Overrun", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_SQUISH) {
		Special.IsGross = true;
	}

	/*
	**	Enable the human generated sound effects.
	*/
	WWGetPrivateProfileString("Options", "Sounds", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_HUMAN) {
		Special.IsJuvenile = true;
	}

	/*
	**	Scrolling is disabled over the tabs with this option.
	*/
	WWGetPrivateProfileString("Options", "Scrolling", "", workbuf, sizeof(workbuf), buffer);
	if (Obfuscate(workbuf) == PARM_SCROLLING) {
		Special.IsScrollMod = true;
	}
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
 *=============================================================================================*/
void OptionsClass::Save_Settings (void)
{
	char * buffer;			// INI staging buffer pointer.
	CCFileClass file;

	/*
	**	Get a working pointer to the INI staging buffer. Make sure that the buffer
	**	starts cleared out of any data.
	*/
	buffer = (char *)_ShapeBuffer;
	memset(buffer, '\0', _ShapeBufferSize);

	file.Set_Name("CONQUER.INI");
	if (file.Is_Available()) {
		file.Read(buffer, _ShapeBufferSize-1);
	}

	/*
	**	Save Options settings
	*/
	WWWritePrivateProfileInt("Options", "GameSpeed", GameSpeed, buffer);
	WWWritePrivateProfileInt("Options", "ScrollRate", ScrollRate, buffer);
	WWWritePrivateProfileInt("Options", "Brightness", Brightness, buffer);
	WWWritePrivateProfileInt("Options", "Volume", Volume, buffer);
	WWWritePrivateProfileInt("Options", "ScoreVolume", ScoreVolume, buffer);
	WWWritePrivateProfileInt("Options", "Contrast", Contrast, buffer);
	WWWritePrivateProfileInt("Options", "Color", Color, buffer);
	WWWritePrivateProfileInt("Options", "Tint", Tint, buffer);
	WWWritePrivateProfileInt("Options", "AutoScroll", AutoScroll, buffer);
	WWWritePrivateProfileInt("Options", "IsScoreRepeat", IsScoreRepeat, buffer);
	WWWritePrivateProfileInt("Options", "IsScoreShuffle", IsScoreShuffle, buffer);
	WWWritePrivateProfileInt("Options", "DeathAnnounce", IsDeathAnnounce, buffer);
	WWWritePrivateProfileInt("Options", "FreeScrolling", IsFreeScroll, buffer);

	/*
	**	Write the INI data out to a file.
	*/
	file.Write(buffer,strlen(buffer));
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
	Set_Color(Color);
	Set_Tint(Tint);
	Set_Sound_Volume(Volume,false);
	Set_Score_Volume(ScoreVolume);
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



void OptionsClass::Fixup_Palette(void) const
{
	Adjust_Palette(OriginalPalette, GamePalette, Brightness, Color, Tint, Contrast);
}


int OptionsClass::Normalize_Sound(int volume) const
{
	return(Fixed_To_Cardinal(volume, Volume));
}