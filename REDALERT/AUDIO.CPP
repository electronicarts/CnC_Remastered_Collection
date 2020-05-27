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

/* $Header: /CounterStrike/AUDIO.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : AUDIO.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : November 1, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Is_Speaking -- Checks to see if the eva voice is still playing.                           *
 *   Sound_Effect -- General purpose sound player.                                             *
 *   Sound_Effect -- Plays a sound effect in the tactical map.                                 *
 *   Speak -- Computer speaks to the player.                                                   *
 *   Speak_AI -- Handles starting the EVA voices.                                              *
 *   Speech_Name -- Fetches the name for the voice specified.                                  *
 *   Stop_Speaking -- Forces the EVA voice to stop talking.                                    *
 *   Voc_From_Name -- Fetch VocType from ASCII name specified.                                 *
 *   Voc_Name -- Fetches the name for the sound effect.                                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
 /*
 **
 **
 ** Win32lib stubs
 **
 **
 **
 */


SFX_Type					SoundType;
Sample_Type				SampleType;

int File_Stream_Sample(char const *filename, BOOL real_time_start) { return 1; };
int File_Stream_Sample_Vol(char const *filename, int volume, BOOL real_time_start) { return 1; };
void __cdecl Sound_Callback(void) {};
void __cdecl far maintenance_callback(void) {};
void *Load_Sample(char const *filename) { return NULL; };
long Load_Sample_Into_Buffer(char const *filename, void *buffer, long size) { return 0; }
long Sample_Read(int fh, void *buffer, long size) { return 0; };
void Free_Sample(void const *sample) {};
BOOL Audio_Init(HWND window, int bits_per_sample, BOOL stereo, int rate, int reverse_channels) { return 0; };
void Sound_End(void) {};
void Stop_Sample(int handle) {};
BOOL Sample_Status(int handle) { return 0; };
BOOL Is_Sample_Playing(void const * sample) { return 0; };
void Stop_Sample_Playing(void const * sample) {};
int Play_Sample(void const *sample, int priority, int volume, signed short panloc) { return 1; };
int Play_Sample_Handle(void const *sample, int priority, int volume, signed short panloc, int id) { return 1; };
int Set_Sound_Vol(int volume) { return 0; };
int Set_Score_Vol(int volume) { return 0; };
void Fade_Sample(int handle, int ticks) {};
int Get_Free_Sample_Handle(int priority) { return 1; };
int Get_Digi_Handle(void) { return 1; }
long Sample_Length(void const *sample) { return 0; };
void Restore_Sound_Buffers(void) {};
BOOL Set_Primary_Buffer_Format(void) { return 0; };
BOOL Start_Primary_Sound_Buffer(BOOL forced) { return 0; };
void Stop_Primary_Sound_Buffer(void) {};


/***************************************************************************
**	Controls what special effects may occur on the sound effect.
*/
typedef enum {
	IN_NOVAR,			// No variation or alterations allowed.
	IN_VAR				// Infantry variance response modification.
} ContextType;

// static struct { // MBL 02.21.2019
// Had to name the struct for VS 2017 distributed build. ST - 4/10/2019 3:59PM
struct SoundEffectNameStruct {
	char const *	Name;		// Digitized voice file name.
	int				Priority;	// Playback priority of this sample.
	ContextType		Where;		// In what game context does this sample exist.
} SoundEffectName[VOC_COUNT] = {

	/*
	**	Civilian voices (technicians too).
	*/
	{"GIRLOKAY",	20,	IN_NOVAR},		//	VOC_GIRL_OKAY
	{"GIRLYEAH",	20,	IN_NOVAR},		//	VOC_GIRL_YEAH
	{"GUYOKAY1",	20,	IN_NOVAR},		//	VOC_GUY_OKAY
	{"GUYYEAH1",	20,	IN_NOVAR},		//	VOC_GUY_YEAH

	{"MINELAY1",	5,		IN_VAR},			// VOC_MINELAY1

	/*
	**	Infantry and vehicle responses.
	*/
	{"ACKNO",		20,	IN_VAR},	//	VOC_ACKNOWL			"acknowledged"
	{"AFFIRM1",		20,	IN_VAR},	//	VOC_AFFIRM			"affirmative"
	{"AWAIT1",		20,	IN_VAR},	//	VOC_AWAIT1			"awaiting orders"
	{"EAFFIRM1",	20,	IN_NOVAR},	// VOC_ENG_AFFIRM	Engineer: "affirmative"
	{"EENGIN1",		20,	IN_NOVAR},	//	VOC_ENG_ENG			Engineer: "engineering"
	{"NOPROB",		20,	IN_VAR},	// VOC_NO_PROB			"not a problem"
	{"READY",		20,	IN_VAR},	// VOC_READY			"ready and waiting"
	{"REPORT1",		20,	IN_VAR},	//	VOC_REPORT			"reporting"
	{"RITAWAY",		20,	IN_VAR},	// VOC_RIGHT_AWAY		"right away sir"
	{"ROGER",		20,	IN_VAR},	// VOC_ROGER			"roger"
	{"UGOTIT",		20,	IN_VAR},	// VOC_UGOTIT			"you got it"
	{"VEHIC1",		20,	IN_VAR},	//	VOC_VEHIC1			"vehicle reporting"
	{"YESSIR1",		20,	IN_VAR},	//	VOC_YESSIR			"yes sir"

	{"DEDMAN1", 	10,	IN_NOVAR},	// VOC_SCREAM1			short infantry scream
	{"DEDMAN2", 	10,	IN_NOVAR},	// VOC_SCREAM3			short infantry scream
	{"DEDMAN3", 	10,	IN_NOVAR},	// VOC_SCREAM4			short infantry scream
	{"DEDMAN4", 	10,	IN_NOVAR},	// VOC_SCREAM5			short infantry scream
	{"DEDMAN5", 	10,	IN_NOVAR},	// VOC_SCREAM6			short infantry scream
	{"DEDMAN6", 	10,	IN_NOVAR},	// VOC_SCREAM7			short infantry scream
	{"DEDMAN7", 	10,	IN_NOVAR},	// VOC_SCREAM10		short infantry scream
	{"DEDMAN8", 	10,	IN_NOVAR},	// VOC_SCREAM11		short infantry scream
	{"DEDMAN10", 	10,	IN_NOVAR},	// VOC_YELL1			long infantry scream

	{"CHRONO2",		5,		IN_NOVAR},	//	VOC_CHRONO			Chronosphere sound
	{"CANNON1",		1,		IN_NOVAR},	//	VOC_CANNON1			Cannon sound (medium).
	{"CANNON2",		1,		IN_NOVAR},	//	VOC_CANNON2			Cannon sound (short).
	{"IRONCUR9",	10,	IN_NOVAR},	// VOC_IRON1
	{"EMOVOUT1",	20,	IN_NOVAR},	//	VOC_ENG_MOVEOUT	Engineer: "movin' out"
	{"SONPULSE",	10,	IN_NOVAR},	//	VOC_SONAR
	{"SANDBAG2",	5,		IN_NOVAR},	// VOC_SANDBAG			sand bag crunch
	{"MINEBLO1",	5,		IN_NOVAR},	// VOC_MINEBLOW		weird mine explosion
	{"CHUTE1",		1,		IN_NOVAR},	//	VOC_CHUTE1			Wind swoosh sound.
	{"DOGY1",		5,		IN_NOVAR},	//	VOC_DOG_BARK		Dog bark.
	{"DOGW5",		10,	IN_NOVAR},	//	VOC_DOG_WHINE		Dog whine.
	{"DOGG5P",		10,	IN_NOVAR},	//	VOC_DOG_GROWL2		Strong dog growl.
	{"FIREBL3",		1,		IN_NOVAR},	//	VOC_FIRE_LAUNCH	Fireball launch sound.
	{"FIRETRT1",	1,		IN_NOVAR},	//	VOC_FIRE_EXPLODE	Fireball explode sound.
	{"GRENADE1",	1,		IN_NOVAR},	//	VOC_GRENADE_TOSS	Grenade toss.
	{"GUN11",		1,		IN_NOVAR},	//	VOC_GUN_5			5 round gun burst (slow).
	{"GUN13",		1,		IN_NOVAR},	//	VOC_GUN_7			7 round gun burst (fast).
	{"EYESSIR1",	20,	IN_NOVAR},	//	VOC_ENG_YES,		Engineer: "yes sir"
	{"GUN27",		1,		IN_NOVAR},	//	VOC_GUN_RIFLE		Rifle shot.
	{"HEAL2",		1,		IN_NOVAR},	//	VOC_HEAL				Healing effect.
	{"HYDROD1",		1,		IN_NOVAR},	//	VOC_DOOR				Hyrdrolic door.
	{"INVUL2",		1,		IN_NOVAR},	//	VOC_INVULNERABLE	Invulnerability effect.
	{"KABOOM1",		1,		IN_NOVAR},	//	VOC_KABOOM1			Long explosion (muffled).
	{"KABOOM12",	1,		IN_NOVAR},	//	VOC_KABOOM12		Very long explosion (muffled).
	{"KABOOM15",	1,		IN_NOVAR},	//	VOC_KABOOM15		Very long explosion (muffled).
	{"SPLASH9",		5,		IN_NOVAR},	// VOC_SPLASH			water splash
	{"KABOOM22",	1,		IN_NOVAR},	//	VOC_KABOOM22		Long explosion (sharp).
	{"AACANON3",	1,		IN_NOVAR},
	{"TANDETH1",	10,	IN_NOVAR},
	{"MGUNINF1",	1,		IN_NOVAR},	//	VOC_GUN_5F			5 round gun burst (fast).
	{"MISSILE1",	1,		IN_NOVAR},	//	VOC_MISSILE_1		Missile with high tech effect.
	{"MISSILE6",	1,		IN_NOVAR},	//	VOC_MISSILE_2		Long missile launch.
	{"MISSILE7",	1,		IN_NOVAR},	//	VOC_MISSILE_3		Short missile launch.
	{"x",				1,		IN_NOVAR},
	{"PILLBOX1",	1,		IN_NOVAR},	//	VOC_GUN_5R			5 round gun burst (rattles).
	{"RABEEP1",		1,		IN_NOVAR},	//	VOC_BEEP				Generic beep sound.
	{"RAMENU1",		1,		IN_NOVAR},	//	VOC_CLICK			Generic click sound.
	{"SILENCER",	1,		IN_NOVAR},	//	VOC_SILENCER		Silencer.
	{"TANK5",		1,		IN_NOVAR},	//	VOC_CANNON6			Long muffled cannon shot.
	{"TANK6",		1,		IN_NOVAR},	//	VOC_CANNON7			Sharp mechanical cannon fire.
	{"TORPEDO1",	1,		IN_NOVAR},	//	VOC_TORPEDO			Torpedo launch.
	{"TURRET1",		1,		IN_NOVAR},	//	VOC_CANNON8			Sharp cannon fire.
	{"TSLACHG2",	10,	IN_NOVAR},	//	VOC_TESLA_POWER_UP	Hum charge up.
	{"TESLA1",		10,	IN_NOVAR},	//	VOC_TESLA_ZAP		Tesla zap effect.
	{"SQUISHY2",	10,	IN_NOVAR},	//	VOC_SQUISH			Squish effect.
	{"SCOLDY1",		10,	IN_NOVAR},	//	VOC_SCOLD			Scold bleep.
	{"RADARON2",	20,	IN_NOVAR},	//	VOC_RADAR_ON		Powering up electronics.
	{"RADARDN1",	10,	IN_NOVAR},	//	VOC_RADAR_OFF		B movie power down effect.
	{"PLACBLDG",	10,	IN_NOVAR},	//	VOC_PLACE_BUILDING_DOWN	Building slam down sound.
	{"KABOOM30",	1,		IN_NOVAR},	//	VOC_KABOOM30		Short explosion (HE).
	{"KABOOM25",	10,	IN_NOVAR},	//	VOC_KABOOM25		Short growling explosion.
	{"x",				10,	IN_NOVAR},
	{"DOGW7",		10,	IN_NOVAR},	//	VOC_DOG_HURT		Dog whine (loud).
	{"DOGW3PX",		10,	IN_NOVAR},	//	VOC_DOG_YES			Dog 'yes sir'.
	{"CRMBLE2",		10,	IN_NOVAR},	//	VOC_CRUMBLE			Building crumble.
	{"CASHUP1",		10,	IN_NOVAR},	//	VOC_MONEY_UP		Rising money tick.
	{"CASHDN1",		10,	IN_NOVAR},	//	VOC_MONEY_DOWN		Falling money tick.
	{"BUILD5",		10,	IN_NOVAR},	//	VOC_CONSTRUCTION	Building construction sound.
	{"BLEEP9",		10,	IN_NOVAR},	//	VOC_GAME_CLOSED	Long bleep.
	{"BLEEP6",		10,	IN_NOVAR},	//	VOC_INCOMING_MESSAGE	Soft happy warble.
	{"BLEEP5",		10,	IN_NOVAR},	//	VOC_SYS_ERROR		Sharp soft warble.
	{"BLEEP17",		10,	IN_NOVAR},	//	VOC_OPTIONS_CHANGED	Mid range soft warble.
	{"BLEEP13",		10,	IN_NOVAR},	//	VOC_GAME_FORMING	Long warble.
	{"BLEEP12",		10,	IN_NOVAR},	//	VOC_PLAYER_LEFT	Chirp sequence.
	{"BLEEP11",		10,	IN_NOVAR},	//	VOC_PLAYER_JOINED	Reverse chirp sequence.
	{"H2OBOMB2",	10,	IN_NOVAR},	//	VOC_DEPTH_CHARGE	Distant explosion sound.
	{"CASHTURN",	10,	IN_NOVAR},	//	VOC_CASHTURN		Airbrake.
	{"TUFFGUY1",	20,	IN_NOVAR},	//	VOC_TANYA_CHEW			Tanya: "Chew on this"
	{"ROKROLL1",	20,	IN_NOVAR},	//	VOC_TANYA_ROCK			Tanya: "Let's rock"
	{"LAUGH1",		20,	IN_NOVAR},	//	VOC_TANYA_LAUGH		Tanya: "ha ha ha"
	{"CMON1",		20,	IN_NOVAR},	//	VOC_TANYA_SHAKE		Tanya: "Shake it baby"
	{"BOMBIT1",		20,	IN_NOVAR},	//	VOC_TANYA_CHING		Tanya: "Cha Ching"
	{"GOTIT1",		20,	IN_NOVAR},	//	VOC_TANYA_GOT			Tanya: "That's all you got"
	{"KEEPEM1",		20,	IN_NOVAR},	//	VOC_TANYA_KISS			Tanya: "Kiss it bye bye"
	{"ONIT1",		20,	IN_NOVAR},	//	VOC_TANYA_THERE		Tanya: "I'm there"
	{"LEFTY1",		20,	IN_NOVAR},	//	VOC_TANYA_GIVE			Tanya: "Give it to me"
	{"YEAH1",		20,	IN_NOVAR},	//	VOC_TANYA_YEA			Tanya: "Yea?"
	{"YES1",			20,	IN_NOVAR},	//	VOC_TANYA_YES			Tanya: "Yes sir?"
	{"YO1",			20,	IN_NOVAR},	//	VOC_TANYA_WHATS		Tanya: "What's up."
	{"WALLKIL2",	5,		IN_NOVAR},	//	VOC_WALLKILL2			Crushing wall sound.
	{"x",				10,	IN_NOVAR},
	{"GUN5",			5,		IN_NOVAR},	//	VOC_TRIPLE_SHOT		Three quick shots in succession.
	{"SUBSHOW1",	5,		IN_NOVAR},	//	VOC_SUBSHOW				Submarine surface sound.
	{"EINAH1",		20,	IN_NOVAR},	//	VOC_E_AH,				Einstien "ah"
	{"EINOK1",		20,	IN_NOVAR},	//	VOC_E_OK,				Einstien "ok"
	{"EINYES1",		20,	IN_NOVAR},	//	VOC_E_YES,				Einstien "yes"
	{"MINE1",		10,	IN_NOVAR},	//	VOC_TRIP_MINE			mine explosion sound

	{"SCOMND1",		20,	IN_NOVAR},	//	VOC_SPY_COMMANDER		Spy: "commander?"
	{"SYESSIR1",	20,	IN_NOVAR},	//	VOC_SPY_YESSIR			Spy: "yes sir"
	{"SINDEED1",	20,	IN_NOVAR},	//	VOC_SPY_INDEED			Spy: "indeed"
	{"SONWAY1",		20,	IN_NOVAR},	//	VOC_SPY_ONWAY			Spy: "on my way"
	{"SKING1",		20,	IN_NOVAR},	//	VOC_SPY_KING			Spy: "for king and country"
	{"MRESPON1",	20,	IN_NOVAR},	//	VOC_MED_REPORTING		Medic: "reporting"
	{"MYESSIR1",	20,	IN_NOVAR},	//	VOC_MED_YESSIR			Medic: "yes sir"
	{"MAFFIRM1",	20,	IN_NOVAR},	//	VOC_MED_AFFIRM			Medic: "affirmative"
	{"MMOVOUT1",	20,	IN_NOVAR},	//	VOC_MED_MOVEOUT		Medic: "movin' out"
	{"BEEPSLCT",	10,	IN_NOVAR},	//	VOC_BEEP_SELECT		map selection beep

	{"SYEAH1",		20,	IN_NOVAR},	//	VOC_THIEF_YEA			Thief: "yea?"
	{"ANTDIE",		20,	IN_NOVAR},	//	VOC_ANTDIE
	{"ANTBITE",		20,	IN_NOVAR},	//	VOC_ANTBITE
	{"SMOUT1",		20,	IN_NOVAR},	//	VOC_THIEF_MOVEOUT		Thief: "movin' out"
	{"SOKAY1",		20,	IN_NOVAR},	//	VOC_THIEF_OKAY			Thief: "ok"
	{"x",				20,	IN_NOVAR},
	{"SWHAT1",		20,	IN_NOVAR},	//	VOC_THIEF_WHAT			Thief: "what"
	{"SAFFIRM1",	20,	IN_NOVAR},	//	VOC_THIEF_AFFIRM		Thief: "affirmative"
//ADDED VG 2/24/97
	{"STAVCMDR",	20,	IN_NOVAR},	
	{"STAVCRSE",	20,	IN_NOVAR},	
	{"STAVYES",		20,	IN_NOVAR},	
	{"STAVMOV",		20,	IN_NOVAR},	
	{"BUZZY1",		20,	IN_NOVAR},	
	{"RAMBO1",     20,   IN_NOVAR},			
	{"RAMBO2", 		20,   IN_NOVAR}, 
	{"RAMBO3",     20,   IN_NOVAR},
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	{"MYES1",		20,	IN_NOVAR},	// VOC_MECHYES1			Mechanic: "Yes sir!"
	{"MHOWDY1",		20,	IN_NOVAR},	// VOC_MECHHOWDY1			Mechanic: "Howdy!"
	{"MRISE1",		20,	IN_NOVAR},	// VOC_MECHRISE1			Mechanic: "Rise 'n shine!"
	{"MHUH1",		20,	IN_NOVAR},	// VOC_MECHHUH1			Mechanic: "Huh?"
	{"MHEAR1",		20,	IN_NOVAR},	// VOC_MECHHEAR1			Mechanic: "I Hear Ya!"
	{"MLAFF1",		20,	IN_NOVAR},	// VOC_MECHLAFF1			Mechanic: guffaw
	{"MBOSS1",		20,	IN_NOVAR},	// VOC_MECHBOSS1			Mechanic: "Sure Thing, Boss!"
	{"MYEEHAW1",	20,	IN_NOVAR},	// VOC_MECHYEEHAW1		Mechanic: "Yee Haw!"
	{"MHOTDIG1",	20,	IN_NOVAR},	// VOC_MECHHOTDIG1		Mechanic: "Hot Diggity Dog!"
	{"MWRENCH1",	20,	IN_NOVAR},	// VOC_MECHWRENCH1		Mechanic: "I'll get my wrench."

	{"JBURN1",		20,	IN_NOVAR},	//	VOC_STBURN1				Shock Trooper: "Burn baby burn!"
	{"JCHRGE1",		20,	IN_NOVAR},	//	VOC_STCHRGE1			Shock Trooper: "Fully charged!"
	{"JCRISP1",		20,	IN_NOVAR},	//	VOC_STCRISP1			Shock Trooper: "Extra Crispy!"
	{"JDANCE1",		20,	IN_NOVAR},	//	VOC_STDANCE1			Shock Trooper: "Let's Dance!"
	{"JJUICE1",		20,	IN_NOVAR},	//	VOC_STJUICE1			Shock Trooper: "Got juice?"
	{"JJUMP1",		20,	IN_NOVAR},	//	VOC_STJUMP1				Shock Trooper: "Need a jump?"
	{"JLIGHT1",		20,	IN_NOVAR},	//	VOC_STLIGHT1			Shock Trooper: "Lights out!"
	{"JPOWER1",		20,	IN_NOVAR},	//	VOC_STPOWER1			Shock Trooper: "Power on!"
	{"JSHOCK1",		20,	IN_NOVAR},	//	VOC_STSHOCK1			Shock Trooper: "Shocking!"
	{"JYES1",		20,	IN_NOVAR},	//	VOC_STYES1				Shock Trooper: "Yesssss!"

	{"CHROTNK1",	20,	IN_NOVAR},	// VOC_CHRONOTANK1		Chrono tank teleport
	{"FIXIT1",		20,	IN_NOVAR},	// VOC_MECH_FIXIT1		Mechanic fixes something
	{"MADCHRG2",	20,	IN_NOVAR},	// VOC_MAD_CHARGE			MAD tank charges up
	{"MADEXPLO",	20,	IN_NOVAR},	// VOC_MAD_EXPLODE		MAD tank explodes
	{"SHKTROP1",	20,	IN_NOVAR},	// VOC_SHOCK_TROOP1		Shock Trooper fires

#endif
};


//
// External handlers. MBL 06.17.2019
//
extern void On_Sound_Effect(int sound_index, int variation, COORDINATE coord, int house);
// extern void On_Speech(int speech_index); MBL 02.06.2020
extern void On_Speech(int speech_index, HouseClass *house);
extern void On_Ping(const HouseClass* player_ptr, COORDINATE coord);



/***********************************************************************************************
 * Voc_From_Name -- Fetch VocType from ASCII name specified.                                   *
 *                                                                                             *
 *    This will find the corresponding VocType from the ASCII string specified. It does this   *
 *    by finding a root filename that matches the string.                                      *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII string that will be converted into a VocType.        *
 *                                                                                             *
 * OUTPUT:  Returns with the VocType that matches the string specified. If no match could be   *
 *          found, then VOC_NONE is returned.                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VocType Voc_From_Name(char const * name)
{
	if (name == NULL) return(VOC_NONE);

	for (VocType voc = VOC_FIRST; voc < VOC_COUNT; voc++) {
		if (stricmp(name, SoundEffectName[voc].Name) == 0) {
			return(voc);
		}
	}

	return(VOC_NONE);
}


/***********************************************************************************************
 * Voc_Name -- Fetches the name for the sound effect.                                          *
 *                                                                                             *
 *    This routine returns the descriptive name of the sound effect. Currently, this is just   *
 *    the root of the file name.                                                               *
 *                                                                                             *
 * INPUT:   voc   -- The VocType that the corresponding name is requested.                     *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the text string the represents the sound effect.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * Voc_Name(VocType voc)
{
	if (voc == VOC_NONE) return("none");
	return(SoundEffectName[voc].Name);
}


/***********************************************************************************************
 * Sound_Effect -- Plays a sound effect in the tactical map.                                   *
 *                                                                                             *
 *    This routine is used when a sound effect occurs in the game world. It handles fading     *
 *    the sound according to distance.                                                         *
 *                                                                                             *
 * INPUT:   voc   -- The sound effect number to play.                                          *
 *                                                                                             *
 *          coord -- The world location that the sound originates from.                        *
 *                                                                                             *
 *          variation   -- This is the optional variation number to use when playing special   *
 *                         sound effects that have variations. For normal sound effects, this  *
 *                         parameter is ignored.                                               *
 *                                                                                             *
 *          house -- This specifies the optional house override value to use when playing      *
 *                   sound effects that have a variation. If not specified, then the current   *
 *                   player is examined for the house variation to use.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/12/1994 JLB : Created.                                                                 *
 *   01/05/1995 JLB : Reduces sound more dramatically when off screen.                         *
 *   09/15/1996 JLB : Revamped volume logic.                                                   *
 *   11/01/1996 JLB : House override control.                                                  *
 *=============================================================================================*/
void Sound_Effect(VocType voc, COORDINATE coord, int variation, HousesType house)
{
	//
	// Intercept sound effect calls. MBL 06.17.2019
	//
	On_Sound_Effect((int)voc, variation, coord, (int)house);

#if 0
	CELL cell_pos = 0;
	int pan_value;

	if (Debug_Quiet || Options.Volume == 0 || voc == VOC_NONE || !SoundOn || SampleType == SAMPLE_NONE) {
		return;
	}
	if (coord) {
		cell_pos = Coord_Cell(coord);
	}

	fixed volume = 1;
	pan_value = 0;
	if (coord && !Map.In_View(cell_pos)) {
		int distance = Distance(coord, Map.TacticalCoord) / CELL_LEPTON_W;
		fixed dfixed = fixed(distance, 128+64);
		dfixed.Sub_Saturate(1);
		volume = fixed(1) - dfixed;

		pan_value  = Cell_X(cell_pos);
		pan_value -= Coord_XCell(Map.TacticalCoord) + (Lepton_To_Cell(Map.TacLeptonWidth) / 2);
		if (ABS(pan_value) > Lepton_To_Cell(Map.TacLeptonWidth / 2)) {
			pan_value *= 0x8000;
			pan_value /= (MAP_CELL_W >> 2);
			pan_value = Bound(pan_value, -0x7FFF, 0x7FFF);
		} else {
			pan_value  = 0;
		}
	}

	Sound_Effect(voc, volume, variation, pan_value, house);
#endif
}


/***********************************************************************************************
 * Sound_Effect -- General purpose sound player.                                               *
 *                                                                                             *
 *    This is used for general purpose sound effects. These are sounds that occur outside      *
 *    of the game world. They do not have a corresponding game world location as their source. *
 *                                                                                             *
 * INPUT:   voc      -- The sound effect number to play.                                       *
 *                                                                                             *
 *          volume   -- The volume to assign to this sound effect.                             *
 *                                                                                             *
 *          variation   -- This is the optional variation number to use when playing special   *
 *                         sound effects that have variations. For normal sound effects, this  *
 *                         parameter is ignored.                                               *
 *                                                                                             *
 *          house -- This specifies the optional house override value to use when playing      *
 *                   sound effects that have a variation. If not specified, then the current   *
 *                   player is examined for the house variation to use.                        *
 *                                                                                             *
 * OUTPUT:  Returns with the sound handle (-1 if no sound was played).                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/12/1994 JLB : Created.                                                                 *
 *   11/12/1994 JLB : Handles cache logic.                                                     *
 *   05/04/1995 JLB : Variation adjustments.                                                   *
 *   11/01/1996 JLB : House override control.                                                  *
 *=============================================================================================*/
int Sound_Effect(VocType voc, fixed volume, int variation, signed short pan_value, HousesType house)
{
	//
	// Intercept sound effect calls. MBL 06.17.2019
	//
	pan_value;
	COORDINATE coord = 0;
	On_Sound_Effect((int)voc, variation, coord, (int)house);

#if 0
	char name[_MAX_FNAME+_MAX_EXT];				// Working filename of sound effect.

	if (Debug_Quiet || Options.Volume == 0 || voc == VOC_NONE || !SoundOn || SampleType == SAMPLE_NONE) {
		return(-1);
	}

	/*
	**	Alter the volume according to the game volume setting.
	*/
	volume = volume * Options.Volume;

	/*
	**	Fetch a pointer to the sound effect data. Modify the sound as appropriate and desired.
	*/
	char const * ext = ".AUD";
	if (SoundEffectName[voc].Where == IN_VAR) {

		/*
		**	If there is no forced house, then use the current player
		**	act like house.
		*/
		if (house == HOUSE_NONE) {
			house = PlayerPtr->ActLike;
		}

		/*
		**	Change the extension based on the variation and house accent requested.
		*/
		if (((1 << house) & HOUSEF_ALLIES) != 0) {

			/*
			**	For infantry, use a variation on the response. For vehicles, always
			**	use the vehicle response table.
			*/
			if (variation < 0) {
				if (ABS(variation) % 2) {
					ext = ".V00";
				} else {
					ext = ".V02";
				}
			} else {
				if (variation % 2) {
					ext = ".V01";
				} else {
					ext = ".V03";
				}
			}
		} else {
			if (variation < 0) {
				if (ABS(variation) % 2) {
					ext = ".R00";
				} else {
					ext = ".R02";
				}
			} else {
				if (variation % 2) {
					ext = ".R01";
				} else {
					ext = ".R03";
				}
			}
		}
	}
	_makepath(name, NULL, NULL, SoundEffectName[voc].Name, ext);
	void const * ptr = MFCD::Retrieve(name);

	/*
	**	If the sound data pointer is not null, then presume that it is valid.
	*/
	if (ptr != NULL) {
		volume.Sub_Saturate(1);
		return(Play_Sample(ptr, SoundEffectName[voc].Priority * volume, volume*256, pan_value));
	}
#endif 

	return(-1);
}


/*
**	This elaborates all the EVA speech voices.
*/
/*static PG*/ char const * Speech[VOX_COUNT] =  {
	"MISNWON1",		//	VOX_ACCOMPLISHED					mission accomplished
	"MISNLST1",		//	VOX_FAIL								your mission has failed
	"PROGRES1",		//	VOX_NO_FACTORY						unable to comply, building in progress
	"CONSCMP1",		//	VOX_CONSTRUCTION					construction complete
	"UNITRDY1",		//	VOX_UNIT_READY						unit ready
	"NEWOPT1",		//	VOX_NEW_CONSTRUCT					new construction options
	"NODEPLY1",		//	VOX_DEPLOY							cannot deploy here
	"STRCKIL1",		//	VOX_STRUCTURE_DESTROYED,		structure destroyed
	"NOPOWR1",		//	VOX_INSUFFICIENT_POWER,			insufficient power
	"NOFUNDS1",		//	VOX_NO_CASH							insufficient funds
	"BCT1",			//	VOX_CONTROL_EXIT					battle control terminated
	"REINFOR1",		//	VOX_REINFORCEMENTS				reinforcements have arrived
	"CANCLD1",		//	VOX_CANCELED						canceled
	"ABLDGIN1",		//	VOX_BUILDING						building
	"LOPOWER1",		//	VOX_LOW_POWER						low power
	"NOFUNDS1",		//	VOX_NEED_MO_MONEY					insufficent funds
	"BASEATK1",		//	VOX_BASE_UNDER_ATTACK			our base is under attack
	"NOBUILD1",		//	VOX_UNABLE_TO_BUILD				unable to build more
	"PRIBLDG1",		//	VOX_PRIMARY_SELECTED				primary building selected
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
#ifdef ENGLISH
	"TANK01",		// VOX_MADTANK_DEPLOYED				M.A.D. Tank Deployed
#else
	"none",
#endif
#else
	"none",
#endif
	"none",			//	VOX_SOVIET_CAPTURED				Allied building captured
	"UNITLST1",		// VOX_UNIT_LOST						unit lost
	"SLCTTGT1",		// VOX_SELECT_TARGET					select target
	"ENMYAPP1",		//	VOX_PREPARE							enemy approaching
	"SILOND1",		//	VOX_NEED_MO_CAPACITY				silos needed
	"ONHOLD1",		//	VOX_SUSPENDED						on hold
	"REPAIR1",		//	VOX_REPAIRING						repairing
	"none",
	"none",
	"AUNITL1",		//	VOX_AIRCRAFT_LOST					airborne unit lost
	"none",
	"AAPPRO1",		//	VOX_ALLIED_FORCES_APPROACHING	allied forces approaching
	"AARRIVE1",		// VOX_ALLIED_APPROACHING			allied reinforcements have arrived
	"none",
	"none",
	"BLDGINF1",		// VOX_BUILDING_INFILTRATED		building infiltrated
	"CHROCHR1",		// VOX_CHRONO_CHARGING				chronosphere charging
	"CHRORDY1",		// VOX_CHRONO_READY					chronosphere ready
	"CHROYES1",		// VOX_CHRONO_TEST					chronosphere test successful
	"CMDCNTR1",		//	VOX_HQ_UNDER_ATTACK				command center under attack
	"CNTLDED1",		//	VOX_CENTER_DEACTIVATED			control center deactivated
	"CONVYAP1",		//	VOX_CONVOY_APPROACHING			convoy approaching
	"CONVLST1",		// VOX_CONVOY_UNIT_LOST				convoy unit lost
	"XPLOPLC1",		//	VOX_EXPLOSIVE_PLACED				explosive charge placed
	"CREDIT1",		// VOX_MONEY_STOLEN					credits stolen
	"NAVYLST1",		// VOX_SHIP_LOST						naval unit lost
	"SATLNCH1",		//	VOX_SATALITE_LAUNCHED			satalite launched
	"PULSE1",		//	VOX_SONAR_AVAILABLE				sonar pulse available
	"none",
	"SOVFAPP1",		//	VOX_SOVIET_FORCES_APPROACHING	soviet forces approaching
	"SOVREIN1",		// VOX_SOVIET_REINFROCEMENTS		soviet reinforcements have arrived
	"TRAIN1",		//	VOX_TRAINING						training
	"AREADY1",		//	VOX_ABOMB_READY
	"ALAUNCH1",		//	VOX_ABOMB_LAUNCH
	"AARRIVN1",		//	VOX_ALLIES_N
	"AARRIVS1",		//	VOX_ALLIES_S
	"AARIVE1",		//	VOX_ALLIES_E
	"AARRIVW1",		//	VOX_ALLIES_W
	"1OBJMET1",		//	VOX_OBJECTIVE1
	"2OBJMET1",		//	VOX_OBJECTIVE2
	"3OBJMET1",		//	VOX_OBJECTIVE3
	"IRONCHG1",		//	VOX_IRON_CHARGING
	"IRONRDY1",		//	VOX_IRON_READY
	"KOSYRES1",		//	VOX_RESCUED
	"OBJNMET1",		//	VOX_OBJECTIVE_NOT
	"FLAREN1",		//	VOX_SIGNAL_N
	"FLARES1",		//	VOX_SIGNAL_S
	"FLAREE1",		//	VOX_SIGNAL_E
	"FLAREW1",		//	VOX_SIGNAL_W
	"SPYPLN1",		//	VOX_SPY_PLANE
	"TANYAF1",		//	VOX_FREED
	"ARMORUP1",		//	VOX_UPGRADE_ARMOR
	"FIREPO1",		//	VOX_UPGRADE_FIREPOWER
	"UNITSPD1",		//	VOX_UPGRADE_SPEED
	"MTIMEIN1",		//	VOX_MISSION_TIMER
	"UNITFUL1",		//	VOX_UNIT_FULL
	"UNITREP1",		//	VOX_UNIT_REPAIRED
	"40MINR",		//	VOX_TIME_40
	"30MINR",		//	VOX_TIME_30
	"20MINR",		//	VOX_TIME_20
	"10MINR",		//	VOX_TIME_10
	"5MINR",			//	VOX_TIME_5
	"4MINR",			//	VOX_TIME_4
	"3MINR",			//	VOX_TIME_3
	"2MINR",			//	VOX_TIME_2
	"1MINR",			//	VOX_TIME_1
	"TIMERNO1",		//	VOX_TIME_STOP
	"UNITSLD1",		//	VOX_UNIT_SOLD
	"TIMERGO1",		//	VOX_TIMER_STARTED
	"TARGRES1",		//	VOX_TARGET_RESCUED
	"TARGFRE1",		//	VOX_TARGET_FREED
	"TANYAR1",		//	VOX_TANYA_RESCUED
	"STRUSLD1",		//	VOX_STRUCTURE_SOLD
	"SOVFORC1",		//	VOX_SOVIET_FORCES_FALLEN
	"SOVEMP1",		//	VOX_SOVIET_SELECTED
	"SOVEFAL1",		//	VOX_SOVIET_EMPIRE_FALLEN
	"OPTERM1",		//	VOX_OPERATION_TERMINATED
	"OBJRCH1",		//	VOX_OBJECTIVE_REACHED
	"OBJNRCH1",		//	VOX_OBJECTIVE_NOT_REACHED
	"OBJMET1",		//	VOX_OBJECTIVE_MET
	"MERCR1",		//	VOX_MERCENARY_RESCUED
	"MERCF1",		//	VOX_MERCENARY_FREED
	"KOSYFRE1",		//	VOX_KOSOYGEN_FREED
	"FLARE1",		//	VOX_FLARE_DETECTED
	"COMNDOR1",		//	VOX_COMMANDO_RESCUED
	"COMNDOF1",		//	VOX_COMMANDO_FREED
	"BLDGPRG1",		//	VOX_BUILDING_IN_PROGRESS
	"ATPREP1",		//	VOX_ATOM_PREPPING
	"ASELECT1",		//	VOX_ALLIED_SELECTED
	"APREP1",		//	VOX_ABOMB_PREPPING
	"ATLNCH1",		//	VOX_ATOM_LAUNCHED
	"AFALLEN1",		//	VOX_ALLIED_FORCES_FALLEN
	"AAVAIL1",		//	VOX_ABOMB_AVAILABLE
	"AARRIVE1",		//	VOX_ALLIED_REINFORCEMENTS
	"SAVE1",			//	VOX_MISSION_SAVED
	"LOAD1"			//	VOX_MISSION_LOADED
};


static VoxType CurrentVoice = VOX_NONE;


/***********************************************************************************************
 * Speech_Name -- Fetches the name for the voice specified.                                    *
 *                                                                                             *
 *    Use this routine to fetch the ASCII name of the speech id specified. Typical use of this *
 *    would be to build a displayable list of the speech types. The trigger system uses this   *
 *    so that a speech type can be selected.                                                   *
 *                                                                                             *
 * INPUT:   speech   -- The speech type id to convert to ASCII string.                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the speech ASCII representation of the speech id type.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * Speech_Name(VoxType speech)
{
	if (speech == VOX_NONE) return("none");
	return(Speech[speech]);
}


/***********************************************************************************************
 * Speak -- Computer speaks to the player.                                                     *
 *                                                                                             *
 *    This routine is used to have the game computer (EVA) speak to the player.                *
 *                                                                                             *
 * INPUT:   voice -- The voice number to speak (see defines.h).                                *
 *                                                                                             *
 * OUTPUT:  Returns with the handle of the playing speech (-1 if no voice started).            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// void Speak(VoxType voice) // MBL 02.06.2020
void Speak(VoxType voice, HouseClass *house, COORDINATE coord)
{
	// MBL 06.17.2019
	if (voice == VOX_NONE)
	{
		return;
	}

	//
	// Intercept speech calls. MBL 06.17.2019
	//

	// On_Speech((int)voice); // MBL 02.06.2020
	On_Speech((int)voice, house);
	if (coord) {
		On_Ping(house, coord);
	}

#if 0
	if (!Debug_Quiet && Options.Volume != 0 && SampleType != 0 && voice != VOX_NONE && voice != SpeakQueue && voice != CurrentVoice && SpeakQueue == VOX_NONE) {
		SpeakQueue = voice;
		Speak_AI();
	}
#endif
}


/***********************************************************************************************
 * Speak_AI -- Handles starting the EVA voices.                                                *
 *                                                                                             *
 *    This starts the EVA voice talking as well. If there is any speech request in the queue,  *
 *    it will be started when the current voice is finished. Call this routine as often as     *
 *    possible (once per game tick is sufficient).                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *   10/11/1996 JLB : Handles multiple speech buffers.                                         *
 *=============================================================================================*/
void Speak_AI(void)
{
// MBL 06.17.2019 KO
#if 0
	static int _index = 0;
	if (Debug_Quiet || SampleType == 0) return;

	if (!Is_Sample_Playing(SpeechBuffer[_index])) {
		CurrentVoice = VOX_NONE;
		if (SpeakQueue != VOX_NONE) {

			/*
			**	Try to find a previously loaded copy of the EVA speech in one of the
			**	speech buffers.
			*/
			void const * speech = NULL;
			for (int index = 0; index < ARRAY_SIZE(SpeechRecord); index++) {
				if (SpeechRecord[index] == SpeakQueue) break;
			}

			/*
			**	If a previous copy could not be located, then load the requested
			**	voice into the oldest buffer available.
			*/
			if (speech == NULL) {
				_index = (_index + 1) % ARRAY_SIZE(SpeechRecord);

				char name[_MAX_FNAME+_MAX_EXT];

				_makepath(name, NULL, NULL, Speech[SpeakQueue], ".AUD");
				CCFileClass file(name);
				if (file.Is_Available() && file.Read(SpeechBuffer[_index], SPEECH_BUFFER_SIZE)) {
					speech = SpeechBuffer[_index];
					SpeechRecord[_index] = SpeakQueue;
				}
			}

			/*
			**	Since the speech file was loaded, play it.
			*/
			if (speech != NULL) {
				Play_Sample(speech, 254, Options.Volume * 256);
				CurrentVoice = SpeakQueue;
			}

			SpeakQueue = VOX_NONE;
		}
	}
#endif

	// MBL 06.18.2019
	if (SpeakQueue != VOX_NONE) 
	{
		// On_Speech((int)SpeakQueue); // MBL 02.06.2020
		On_Speech((int)SpeakQueue, NULL);
		SpeakQueue = VOX_NONE;
	}
}


/***********************************************************************************************
 * Stop_Speaking -- Forces the EVA voice to stop talking.                                      *
 *                                                                                             *
 *    Use this routine to immediately stop the EVA voice from speaking. It also clears out     *
 *    the pending voice queue.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void Stop_Speaking(void)
{
	SpeakQueue = VOX_NONE;
	Stop_Sample_Playing(SpeechBuffer);
}


/***********************************************************************************************
 * Is_Speaking -- Checks to see if the eva voice is still playing.                             *
 *                                                                                             *
 *    Call this routine when the EVA voice being played needs to be checked. A typical use     *
 *    of this would be when some action needs to be delayed until the voice has finished --    *
 *    say the end of the game.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the EVA voice still playing?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Is_Speaking(void)
{
	Speak_AI();
	if (!Debug_Quiet && SampleType != 0 && (SpeakQueue != VOX_NONE || Is_Sample_Playing(SpeechBuffer))) {
		return(true);
	}
	return(false);
}
