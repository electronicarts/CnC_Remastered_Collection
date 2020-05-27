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

/* $Header:   F:\projects\c&c\vcs\code\audio.cpv   2.17   16 Oct 1995 16:50:20   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : May 4, 1995 [JLB]                                            *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Is_Speaking -- Checks to see if the eva voice is still playing.                           *
 *   Sound_Effect -- General purpose sound player.                                             *
 *   Sound_Effect -- Plays a sound effect in the tactical map.                                 *
 *   Speak -- Computer speaks to the player.                                                   *
 *   Speak_AI -- Handles starting the EVA voices.                                              *
 *   Stop_Speaking -- Forces the EVA voice to stop talking.                                    *
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

int File_Stream_Sample(char const *filename, BOOL real_time_start) {return 1;};
int File_Stream_Sample_Vol(char const *filename, int volume, BOOL real_time_start)  {return 1;};
void __cdecl Sound_Callback(void) {};
void __cdecl far maintenance_callback(void) {};
void *Load_Sample(char const *filename) {return NULL;};
long Load_Sample_Into_Buffer(char const *filename, void *buffer, long size) {return 0;}
long Sample_Read(int fh, void *buffer, long size) {return 0;};
void Free_Sample(void const *sample) {};
BOOL Audio_Init( HWND window , int bits_per_sample, BOOL stereo , int rate , int reverse_channels) {return 0;};
void Sound_End(void) {};
void Stop_Sample(int handle) {};
BOOL Sample_Status(int handle) {return 0;};
BOOL Is_Sample_Playing(void const * sample) {return 0;};
void Stop_Sample_Playing(void const * sample) {};
int Play_Sample(void const *sample, int priority, int volume, signed short panloc) {return 1;};
int Play_Sample_Handle(void const *sample, int priority, int volume, signed short panloc, int id) {return 1;};
int Set_Sound_Vol(int volume) {return 0;};
int Set_Score_Vol(int volume) {return 0;};
void Fade_Sample(int handle, int ticks) {};
int Get_Free_Sample_Handle(int priority) {return 1;};
int Get_Digi_Handle(void) {return 1;}
long Sample_Length(void const *sample) {return 0;};
void Restore_Sound_Buffers (void) {};
BOOL Set_Primary_Buffer_Format(void) {return 0;};
BOOL Start_Primary_Sound_Buffer (BOOL forced) {return 0;};
void Stop_Primary_Sound_Buffer (void) {};












/***************************************************************************
**	Controls what special effects may occur on the sound effect.
*/
typedef enum {
	IN_NOVAR,			// No variation or alterations allowed.
	IN_JUV,				// Juvenile sound effect alternate option.
	IN_VAR,				// Infantry variance response modification.
} ContextType;

// static struct { // MBL 02.21.2019
// Had to name the struct for VS 2017 distributed build. ST - 4/10/2019 3:59PM
struct SoundEffectNameStruct {
	char const *Name;		// Digitized voice file name.
	int			Priority;	// Playback priority of this sample.
	ContextType	Where;		// In what game context does this sample exist.
} SoundEffectName[VOC_COUNT] = {

	/*
	**	Special voices (typically associated with the commando).
	*/
	{"BOMBIT1",		20, IN_NOVAR},		//	VOC_RAMBO_PRESENT		"I've got a present for	ya"
	{"CMON1",		20, IN_NOVAR},		//	VOC_RAMBO_CMON			"c'mon"
	{"GOTIT1",		20, IN_NOVAR},		//	VOC_RAMBO_UGOTIT		"you got it" *
	{"KEEPEM1",		20, IN_NOVAR},		//	VOC_RAMBO_COMIN		"keep 'em commin'"
	{"LAUGH1",		20, IN_NOVAR},		//	VOC_RAMBO_LAUGH		"hahaha"
	{"LEFTY1",		20, IN_NOVAR},		//	VOC_RAMBO_LEFTY		"that was left handed" *
	{"NOPRBLM1",	20, IN_NOVAR},		//	VOC_RAMBO_NOPROB		"no problem"
//	{"OHSH1",		20, IN_NOVAR},		//	VOC_RAMBO_OHSH			"oh shiiiiii...."
	{"ONIT1",		20, IN_NOVAR},		//	VOC_RAMBO_ONIT			"I'm on it"
	{"RAMYELL1",	20, IN_NOVAR},		//	VOC_RAMBO_YELL			"ahhhhhhh"
	{"ROKROLL1",	20, IN_NOVAR},		//	VOC_RAMBO_ROCK			"time to rock and roll"
	{"TUFFGUY1",	20, IN_NOVAR},		//	VOC_RAMBO_TUFF			"real tuff guy" *
	{"YEAH1",		20, IN_NOVAR},		//	VOC_RAMBO_YEA			"yea" *
	{"YES1",			20, IN_NOVAR},		//	VOC_RAMBO_YES			"yes" *
	{"YO1",			20, IN_NOVAR},		//	VOC_RAMBO_YO			"yo"

	/*
	**	Civilian voices (technicians too).
	*/
	{"GIRLOKAY",	20, IN_NOVAR},		//	VOC_GIRL_OKAY
	{"GIRLYEAH",	20, IN_NOVAR},		//	VOC_GIRL_YEAH
	{"GUYOKAY1",	20, IN_NOVAR},		//	VOC_GUY_OKAY
	{"GUYYEAH1",	20, IN_NOVAR},		//	VOC_GUY_YEAH

	/*
	**	Infantry and vehicle responses.
	*/
	{"2DANGR1",		10,	IN_VAR},	//	VOC_2DANGER			"negative, too dangerous"
	{"ACKNO",		10,	IN_VAR},	//	VOC_ACKNOWL			"acknowledged"
	{"AFFIRM1",		10,	IN_VAR},	//	VOC_AFFIRM			"affirmative"
	{"AWAIT1",		10,	IN_VAR},	//	VOC_AWAIT1			"awaiting orders"
//	{"BACKUP",		10,	IN_VAR},	// VOC_BACKUP			"send backup"
//	{"HELP",			10,	IN_VAR},	// VOC_HELP				"send help"
	{"MOVOUT1",		10,	IN_VAR},	//	VOC_MOVEOUT			"movin' out"
	{"NEGATV1",		10,	IN_VAR},	//	VOC_NEGATIVE		"negative"
	{"NOPROB",		10,	IN_VAR},	// VOC_NO_PROB			"not a problem"
	{"READY",		10,	IN_VAR},	// VOC_READY			"ready and waiting"
	{"REPORT1",		10,	IN_VAR},	//	VOC_REPORT			"reporting"
	{"RITAWAY",		10,	IN_VAR},	// VOC_RIGHT_AWAY		"right away sir"
	{"ROGER",		10,	IN_VAR},	// VOC_ROGER			"roger"
//	{"SIR1",			10,	IN_VAR},	//	VOC_SIR1				"sir?"
//	{"SQUAD1",		10,	IN_VAR},	//	VOC_SQUAD1			"squad reporting"
//	{"TARGET1",		10,	IN_VAR},	//	VOC_PRACTICE		"target practice"
	{"UGOTIT",		10,	IN_VAR},	// VOC_UGOTIT			"you got it"
	{"UNIT1",		10,	IN_VAR},	//	VOC_UNIT1			"unit reporting"
	{"VEHIC1",		10,	IN_VAR},	//	VOC_VEHIC1			"vehicle reporting"
	{"YESSIR1",		10,	IN_VAR},	//	VOC_YESSIR			"yes sir"

	/*
	**	Sound effects that have a juvenile counterpart.
	*/
	{"BAZOOK1", 	1,		IN_JUV},	// VOC_BAZOOKA			Gunfire
	{"BLEEP2", 		1,		IN_JUV},	// VOC_BLEEP			Clean metal bing
	{"BOMB1", 		1,		IN_JUV},	// VOC_BOMB1			Crunchy parachute bomb type explosion
	{"BUTTON", 		1,		IN_JUV},	// VOC_BUTTON			Dungeon Master button click
	{"COMCNTR1",	10,	IN_JUV},	// VOC_RADAR_ON		Elecronic static with beeps
	{"CONSTRU2",	10,	IN_JUV},	// VOC_CONSTRUCTION	construction sounds
	{"CRUMBLE", 	1,		IN_JUV},	// VOC_CRUMBLE			muffled crumble sound
	{"FLAMER2", 	4,		IN_JUV},	// VOC_FLAMER1			flame thrower
	{"GUN18",		4, 	IN_JUV},	// VOC_RIFLE			rifle shot
	{"GUN19",		4, 	IN_JUV},	// VOC_M60				machine gun burst -- 6 rounds
	{"GUN20",		4, 	IN_JUV},	// VOC_GUN20			bat hitting heavy metal door
	{"GUN5",			4, 	IN_JUV},	// VOC_M60A				medium machine gun burst
	{"GUN8",			4, 	IN_JUV},	// VOC_MINI				mini gun burst
	{"GUNCLIP1",	1, 	IN_JUV},	// VOC_RELOAD			gun clip reload
	{"HVYDOOR1", 	5,		IN_JUV},	// VOC_SLAM				metal plates slamming together
	{"HVYGUN10",	1,		IN_JUV},	//	VOC_HVYGUN10		loud sharp cannon
	{"ION1", 		1,		IN_JUV},	// VOC_ION_CANNON		partical beam
	{"MGUN11", 		1,		IN_JUV},	// VOC_MGUN11			alternate tripple burst
	{"MGUN2", 		1,		IN_JUV},	// VOC_MGUN2			M-16 tripple burst
	{"NUKEMISL",	1,		IN_JUV},	//	VOC_NUKE_FIRE		long missile sound
	{"NUKEXPLO",	1,		IN_JUV},	//	VOC_NUKE_EXPLODE	long but not loud explosion
	{"OBELRAY1",	1,		IN_JUV},	//	VOC_LASER			humming laser beam
	{"OBELPOWR",	1,		IN_JUV},	// VOC_LASER_POWER	warming-up sound of laser beam
	{"POWRDN1",		1,		IN_JUV},	//	VOC_RADAR_OFF		doom door slide
	{"RAMGUN2",		1,		IN_JUV},	//	VOC_SNIPER			silenced rifle fire
	{"ROCKET1",		1,		IN_JUV},	//	VOC_ROCKET1			rocket launch variation #1
	{"ROCKET2",		1,		IN_JUV},	//	VOC_ROCKET2			rocket launch variation #2
	{"SAMMOTR2",	1, 	IN_JUV},	// VOC_MOTOR			dentists drill
	{"SCOLD2",		1, 	IN_JUV},	// VOC_SCOLD			cannot perform action feedback tone
	{"SIDBAR1C", 	1,		IN_JUV},	// VOC_SIDEBAR_OPEN	xylophone clink
	{"SIDBAR2C", 	1,		IN_JUV},	// VOC_SIDEBAR_CLOSE	xylophone clink
	{"SQUISH2", 	1,		IN_JUV},	// VOC_SQUISH2			crushing infantry
	{"TNKFIRE2",	1, 	IN_JUV},	// VOC_TANK1			sharp tank fire with recoil
	{"TNKFIRE3",	1, 	IN_JUV},	// VOC_TANK2			sharp tank fire
	{"TNKFIRE4",	1, 	IN_JUV},	// VOC_TANK3			sharp tank fire
	{"TNKFIRE6",	1, 	IN_JUV},	// VOC_TANK4			big gun tank fire
	{"TONE15",		0, 	IN_JUV},	// VOC_UP				credits counting up
	{"TONE16",		0, 	IN_JUV},	// VOC_DOWN				credits counting down
	{"TONE2",		1, 	IN_JUV},	// VOC_TARGET			target sound
	{"TONE5",		10,	IN_JUV},	// VOC_SONAR			sonar echo
	{"TOSS",			1, 	IN_JUV},	// VOC_TOSS				air swish
	{"TRANS1",		1, 	IN_JUV},	// VOC_CLOAK			stealth tank
	{"TREEBRN1",	1, 	IN_JUV},	// VOC_BURN				burning crackle
	{"TURRFIR5",	1, 	IN_JUV},	// VOC_TURRET			muffled gunfire
	{"XPLOBIG4",	5,		IN_JUV},	//	VOC_XPLOBIG4		very long muffled explosion
	{"XPLOBIG6",	5,		IN_JUV},	//	VOC_XPLOBIG6		very long muffled explosion
	{"XPLOBIG7",	5,		IN_JUV},	//	VOC_XPLOBIG7		very long muffled explosion
	{"XPLODE",		1,		IN_JUV},	// VOC_XPLODE			long soft muffled explosion
	{"XPLOS",		4, 	IN_JUV},	// VOC_XPLOS			short crunchy explosion
	{"XPLOSML2",	5,		IN_JUV},	//	VOC_XPLOSML2		muffled mechanical explosion

	/*
	**	Generic sound effects (no variations).
	*/
	{"NUYELL1", 	10,	IN_NOVAR},	// VOC_SCREAM1			short infantry scream
	{"NUYELL3", 	10,	IN_NOVAR},	// VOC_SCREAM3			short infantry scream
	{"NUYELL4", 	10,	IN_NOVAR},	// VOC_SCREAM4			short infantry scream
	{"NUYELL5", 	10,	IN_NOVAR},	// VOC_SCREAM5			short infantry scream
	{"NUYELL6", 	10,	IN_NOVAR},	// VOC_SCREAM6			short infantry scream
	{"NUYELL7", 	10,	IN_NOVAR},	// VOC_SCREAM7			short infantry scream
	{"NUYELL10", 	10,	IN_NOVAR},	// VOC_SCREAM10		short infantry scream
	{"NUYELL11", 	10,	IN_NOVAR},	// VOC_SCREAM11		short infantry scream
	{"NUYELL12", 	10,	IN_NOVAR},	// VOC_SCREAM12		short infantry scream
	{"YELL1", 		1,		IN_NOVAR},	// VOC_YELL1			long infantry scream

	{"MYES1", 		10,	IN_NOVAR},	// VOC_YES				"Yes?"
	{"MCOMND1", 	10,	IN_NOVAR},	// VOC_COMMANDER		"Commander?"
	{"MHELLO1", 	10,	IN_NOVAR},	//	VOC_HELLO			"Hello?"
	{"MHMMM1", 		10,	IN_NOVAR},	//	VOC_HMMM				"Hmmm?"
//	{"MHASTE1", 	10,	IN_NOVAR},	//	VOC_PROCEED1		"I will proceed, post haste."
//	{"MONCE1", 		10,	IN_NOVAR},	//	VOC_PROCEED2		"I will proceed, at once."
//	{"MIMMD1", 		10,	IN_NOVAR},	//	VOC_PROCEED3		"I will proceed, immediately."
//	{"MPLAN1", 		10,	IN_NOVAR},	//	VOC_EXCELLENT1		"That is an excellent plan."
//	{"MPLAN2", 		10,	IN_NOVAR},	//	VOC_EXCELLENT2		"Yes, that is an excellent plan."
	{"MPLAN3", 		10,	IN_NOVAR},	//	VOC_EXCELLENT3		"A wonderful plan."
//	{"MACTION1", 	10,	IN_NOVAR},	//	VOC_EXCELLENT4		"Astounding plan of action commander."
//	{"MREMARK1", 	10,	IN_NOVAR},	// VOC_EXCELLENT5		"Remarkable contrivance."
	{"MCOURSE1", 	10,	IN_NOVAR},	// VOC_OF_COURSE		"Of course."
	{"MYESYES1", 	10,	IN_NOVAR},	// VOC_YESYES			"Yes yes yes."
	{"MTIBER1", 	10,	IN_NOVAR},	//	VOC_QUIP1			"Mind the Tiberium."
//	{"MMG1", 		10,	IN_NOVAR},	//	VOC_QUIP2			"A most remarkable  Metasequoia Glyptostroboides."
	{"MTHANKS1", 	10,	IN_NOVAR},	//	VOC_THANKS			"Thank you."

	{"CASHTURN", 	1,		IN_NOVAR},	//	VOC_CASHTURN		Sound of money being piled up.
	{"BLEEP2", 		10,	IN_NOVAR},	//	VOC_BLEEPY3			Clean computer bleep sound.
	{"DINOMOUT", 	10,	IN_NOVAR},	//	VOC_DINOMOUT		Movin' out in dino-speak.
	{"DINOYES", 	10,	IN_NOVAR},	//	VOC_DINOYES			Yes Sir in dino-speak.
	{"DINOATK1", 	10,	IN_NOVAR},	//	VOC_DINOATK1		Dino attack sound.
	{"DINODIE1", 	10,	IN_NOVAR},	//	VOC_DINODIE1		Dino die sound.

#ifdef PETROGLYPH_EXAMPLE_MOD
	{"NUKE_LOB", 	10,	IN_NOVAR}	//	VOC_NUKE_LOB		Mod expansion unit firing sound
#endif //PETROGLYPH_EXAMPLE_MOD

};


//
// External handlers. ST - 2/20/2019 3:41PM
//
extern void On_Sound_Effect(int sound_index, int variation, COORDINATE coord);
// extern void On_Speech(int speech_index); // MBL 02.06.2020
extern void On_Speech(int speech_index, HouseClass *house);
extern void On_Ping(const HouseClass* player_ptr, COORDINATE coord);

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
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/12/1994 JLB : Created.                                                                 *
 *   01/05/1995 JLB : Reduces sound more dramatically when off screen.                         *
 *=============================================================================================*/
void Sound_Effect(VocType voc, COORDINATE coord, int variation)
{
	//
	// Intercept sound effect calls. ST - 2/20/2019 3:37PM
	//
	On_Sound_Effect((int)voc, variation, coord);
			
#if (0)			
	unsigned			distance;
	CELL				cell_pos;
	int pan_value;

	if (!Options.Volume || voc == VOC_NONE || !SoundOn || SampleType == SAMPLE_NONE) {
		return;
	}
	if (coord) {
		cell_pos = Coord_Cell(coord);
	}

	distance  = 0xFF;
	pan_value = 0;
	if (coord && !Map.In_View(cell_pos)) {
		distance = Map.Cell_Distance(cell_pos, Coord_Cell(Map.TacticalCoord));
		distance = (unsigned int)MIN((int)distance, (int)MAP_CELL_W);
		distance = Cardinal_To_Fixed(MAP_CELL_W, distance);
		distance = MIN(distance, 0xFFu);
		distance ^= 0xFF;

		distance /= 2;
		distance = MAX(distance, 25U);

		pan_value  = Cell_X(cell_pos);
		pan_value -= Coord_XCell(Map.TacticalCoord) + (Lepton_To_Cell(Map.TacLeptonWidth) >> 1);
		if (ABS(pan_value) > Lepton_To_Cell(Map.TacLeptonWidth >> 1)) {
			pan_value *= 0x8000;
			pan_value /= (MAP_CELL_W >> 2);
			pan_value = Bound(pan_value, -0x7FFF, 0x7FFF);
//			pan_value  = MAX((int)pan_value, (int)-0x7FFF);
//			pan_value  = MIN((int)pan_value, 0x7FFF);
		} else {
			pan_value  = 0;
		}
	}

	Sound_Effect(voc, (VolType)Fixed_To_Cardinal(distance, Options.Volume), variation, pan_value);
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
 * OUTPUT:  Returns with the sound handle (-1 if no sound was played).                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/12/1994 JLB : Created.                                                                 *
 *   11/12/1994 JLB : Handles cache logic.                                                     *
 *   05/04/1995 JLB : Variation adjustments.                                                   *
 *=============================================================================================*/
int Sound_Effect(VocType voc, VolType volume, int variation, signed short pan_value)
{
	char name[_MAX_FNAME+_MAX_EXT];				// Working filename of sound effect.

	if (!Options.Volume || voc == VOC_NONE || !SoundOn || SampleType == SAMPLE_NONE) {
		return(-1);
	}

	/*
	**	Fetch a pointer to the sound effect data. Modify the sound as appropriate and desired.
	*/
	char const * ext = ".AUD";
	if (Special.IsJuvenile && SoundEffectName[voc].Where == IN_JUV) {
		ext = ".JUV";
	} else {
		if (SoundEffectName[voc].Where == IN_VAR) {

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
		}
	}
	_makepath(name, NULL, NULL, SoundEffectName[voc].Name, ext);
	void const * ptr = MixFileClass::Retrieve(name);

	/*
	**	If the sound data pointer is not null, then presume that it is valid.
	*/
	if (ptr) {
		return(Play_Sample(ptr, Fixed_To_Cardinal(SoundEffectName[voc].Priority, (int)volume), (int)volume, pan_value));
	}
	return(-1);
}


/*
**	This elaborates all the EVA speech voices.
*/
char const * Speech[VOX_COUNT] =  {
	"ACCOM1",		//	mission accomplished
	"FAIL1",			//	your mission has failed
	"BLDG1",			//	unable to comply, building in progress
	"CONSTRU1",		//	construction complete
	"UNITREDY",		//	unit ready
	"NEWOPT1",		//	new construction options
	"DEPLOY1",		//	cannot deploy here
	"GDIDEAD1",		//	GDI unit destroyed
	"NODDEAD1",		//	Nod unit destroyed
	"CIVDEAD1",		//	civilian killed
//	"EVAYES1",		//	affirmative
//	"EVANO1",		//	negative
//	"UPUNIT1",		//	upgrade complete, new unit available
//	"UPSTRUC1",		//	upgrade complete, new structure available
	"NOCASH1",		//	insufficient funds
	"BATLCON1",		//	battle control terminated
	"REINFOR1",		//	reinforcements have arrived
	"CANCEL1",		//	canceled
	"BLDGING1",		//	building
	"LOPOWER1",		//	low power
	"NOPOWER1",		//	insufficient power
	"MOCASH1",		//	need more funds
	"BASEATK1",		//	our base is under attack
	"INCOME1",		//	incoming missile
	"ENEMYA",		//	enemy planes approaching
	"NUKE1",			//	nuclear warhead approaching - VOX_INCOMING_NUKE
//	"RADOK1",		//	radiation levels are acceptable
//	"RADFATL1",		//	radiation levels are fatal
	"NOBUILD1",		//	unable to build more
	"PRIBLDG1",		//	primary building selected
//	"REPDONE1",		//	repairs completed
	"NODCAPT1",		//	Nod building captured
	"GDICAPT1",		//	GDI building captured
//	"SOLD1",			//	structure sold
	"IONCHRG1",		//	ion cannon charging
	"IONREDY1",		//	ion cannon ready
	"NUKAVAIL",		//	nuclear weapon available
	"NUKLNCH1",		//	nuclear weapon launched - VOX_NUKE_LAUNCHED
	"UNITLOST",		// unit lost
	"STRCLOST",		// structure lost
	"NEEDHARV",		//	need harvester
	"SELECT1",		// select target
	"AIRREDY1",		// airstrike ready
	"NOREDY1",		//	not ready
	"TRANSSEE",		// Nod transport sighted
	"TRANLOAD",		// Nod transport loaded
	"ENMYAPP1",		//	enemy approaching
	"SILOS1",		//	silos needed
	"ONHOLD1",		//	on hold
	"REPAIR1",		//	repairing
	"ESTRUCX",		//	enemy structure destroyed
	"GSTRUC1",		//	GDI structure destroyed
	"NSTRUC1",		//	NOD structure destroyed
	"ENMYUNIT",		// Enemy unit destroyed
//	"GUKILL1",		//	gold unit destroyed
//	"GSTRUD1",		//	gold structure destroyed
//	"GONLINE1",		//	gold player online
//	"GLEFT1",		//	gold player has departed
//	"GOLDKILT",		//	gold player destroyed
//	"GOLDWIN",		//	gold player is victorious
//	"RUKILL1",		//	red unit destroyed
//	"RSTRUD1",		//	red structure destroyed
//	"RONLINE1",		//	red player online
//	"RLEFT1",		//	red player has departed
//	"REDKILT",		//	red player destroyed
//	"REDWIN",		//	red player is victorious
//	"GYUKILL1",		//	grey unit destroyed
//	"GYSTRUD1",		//	grey structure destroyed
//	"GYONLINE",		//	grey player online
//	"GYLEFT1",		//	grey player has departed
//	"GREYKILT",		//	grey player destroyed
//	"GREYWIN",		//	grey player is victorious
//	"OUKILL1",		//	orange unit destroyed
//	"OSTRUD1",		//	orange structure destroyed
//	"OONLINE1",		//	orange player online
//	"OLEFT1",		//	orange player has departed
//	"ORANKILT",		//	orange player destroyed
//	"ORANWIN",		//	orange player is victorious
//	"GNUKILL1",		//	green unit destroyed
//	"GNSTRUD1",		//	green structure destroyed
//	"GNONLINE",		//	green player online
//	"GNLEFT1",		//	green player has departed
//	"GRENKILT",		//	green player destroyed
//	"GRENWIN",		//	green player is victorious
//	"BUKILL1",		//	blue unit destroyed
//	"BSTRUD1",		//	blue structure destroyed
//	"BONLINE1",		//	blue player online
//	"BLEFT1",		//	blue player has departed
//	"BLUEKILT",		//	blue player destroyed
//	"BLUEWIN"		//	blue player is victorious
};
static VoxType CurrentVoice = VOX_NONE;


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
void Speak(VoxType voice, HouseClass *house, COORDINATE coord)
{
	// MBL 02.22.2019
	if (voice == VOX_NONE)
	{
		return;
	}

	//
	// Intercept speech calls. ST - 2/20/2019 3:37PM
	//
	// On_Speech((int)voice); // MBL 02.06.2020
	On_Speech((int)voice, house);
	if (coord) {
		On_Ping(house, coord);
	}

#if (0)	
	if (Options.Volume && SampleType != 0 && voice != VOX_NONE && voice != SpeakQueue && voice != CurrentVoice && SpeakQueue == VOX_NONE) {
		SpeakQueue = voice;
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
 *=============================================================================================*/
void Speak_AI(void)
{
// MBL 06.17.2019 KO
#if 0
	static VoxType _last = VOX_NONE;
	if (SampleType == 0) return;

	if (!Is_Sample_Playing(SpeechBuffer)) {
		CurrentVoice = VOX_NONE;
		if (SpeakQueue != VOX_NONE) {
			if (SpeakQueue != _last) {
				char name[_MAX_FNAME+_MAX_EXT];

				_makepath(name, NULL, NULL, Speech[SpeakQueue], ".AUD");
				if (CCFileClass(name).Read(SpeechBuffer, SPEECH_BUFFER_SIZE)) {
					Play_Sample(SpeechBuffer, 254, Options.Volume);
				}
				_last = SpeakQueue;
			} else {
				Play_Sample(SpeechBuffer, 254, Options.Volume);
			}
			SpeakQueue = VOX_NONE;
		}
	}
#endif
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
	if (SampleType != 0) {
		Stop_Sample_Playing(SpeechBuffer);
	}
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
	if (SampleType != 0 && (SpeakQueue != VOX_NONE || Is_Sample_Playing(SpeechBuffer))) {
		return(true);
	}
	return(false);
}