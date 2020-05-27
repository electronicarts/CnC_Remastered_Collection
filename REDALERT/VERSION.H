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

/* $Header: /counterstrike/VERSION.H 2     3/10/97 6:22p Steve_tall $ */
/***************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : VERSION.H                                *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 10/26/95                                 *
 *                                                                         *
 *                  Last Update : October 26, 1995 [BRR]                   *
 *                                                                         *
 * This class maintains version information, and communications protocol	*
 * information.																				*
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef VERSION_H
#define VERSION_H

#define	VERSION_RED_ALERT_104		0x00010000	//Shipped US version number
#define	VERSION_RED_ALERT_107		0x00011000	//Shipped Counterstrike number
#define	VERSION_RED_ALERT_106		0x00010500	//Patch without CS installed
#define	VERSION_RED_ALERT_108		0x00011500	//Patch with CS installed
#define	VERSION_AFTERMATH_CS			0x00011FFF	//Aftermath with CS installed
#define	VERSION_AFTERMATH				0x00012000	//Aftermath

//	Aftermath has, in a sense, used version 2.00. (Because of the text on title screen.) Call ourselves version 3.
#define VERSION_RA_300				0x00030000	//	RA, CS, AM executables unified into one. All are now the same version. -ajw
//	It seems that extra information, that didn't belong there, was being stuffed into version number. Namely, whether or not
//	Counterstrike is installed. I'm going to change things back to the way they should be, as I see it. Version will describe
//	the version of the executable only. When it comes to communicating whether or not a player has expansions present, separate
//	data will be transmitted.

//	However, having said that, a caveat. I'm going to have to use the same communication method that was used previously, because
//	I need to have prior versions of the game recognize that they can't play against this version. What I'll do is encode
//	"does player have aftermath" (which is actually the only fact that matters, in multiplayer) in the communicated version
//	number, as a high bit set/unset. This version of the game will receive this communicated value and pull out the Aftermath
//	bit. Older version will reject us as a possible opponent, because, whether or not AM is installed, our version number will
//	be too high for them.

//	These horrible things are no longer used. ajw
#define	CS_MAJOR_VERSION_MODIFIER	0x0000
#define	CS_MINOR_VERSION_MODIFIER	0x1000

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
typedef enum CommProtocolEnum {
	COMM_PROTOCOL_SINGLE_NO_COMP = 0,	// single frame with no compression
	COMM_PROTOCOL_SINGLE_E_COMP,			// single frame with event compression
	COMM_PROTOCOL_MULTI_E_COMP,			// multiple frame with event compression
	COMM_PROTOCOL_COUNT,
	DEFAULT_COMM_PROTOCOL = COMM_PROTOCOL_MULTI_E_COMP
} CommProtocolType;

typedef struct {
	unsigned long Version;
	CommProtocolType Protocol;
} VersionProtocolType;

class VersionClass {
	public:
		//.....................................................................
		// Constructor/Destructor
		//.....................................................................
		VersionClass(void);
		virtual ~VersionClass() {};

		//.....................................................................
		// These routines return the current version number.  The long version
		// number contains the major version in the high word, and the minor
		// version in the low word.  They should be interpreted in hex.
		//.....................................................................
		unsigned long Version_Number(void);
		unsigned short Major_Version(void);
		unsigned short Minor_Version(void);

		//.....................................................................
		// Retrieves a pointer to the version # as a text string (#.#), with
		// the trailing 0's trimmed off.
		//.....................................................................
		char *Version_Name(void);

		//.....................................................................
		// Retrieves a pointer to the current version text.
		//.....................................................................
		char *Version_Text() {return (VersionText);}

		//.....................................................................
		// Returns the default comm protocol for a given version number.
		//.....................................................................
		CommProtocolType Version_Protocol(unsigned long version);

		//.....................................................................
		// These routines support "version clipping".
		//.....................................................................
		void Init_Clipping(void);
		unsigned long Clip_Version(unsigned long minver, unsigned long maxver);
		unsigned long Get_Clipped_Version(void) {return (MaxClipVer);}

		//.....................................................................
		// These routines return the theoretical lowest & highest version #'s
		// that this program will connect to; this does not take any previous
		// version clipping into account.
		//.....................................................................
		unsigned long Min_Version(void);
		unsigned long Max_Version(void);

	private:
		//.....................................................................
		// Fills in a 'VersionText' with a descriptive version name.
		//.....................................................................
		void Read_Text_String(void);

		//.....................................................................
		// These values define the major & minor version #'s for the current
		// version.  Change these values to change the game's version #!
		//.....................................................................
		enum VersionEnum {
#ifdef FIXIT_VERSION_3
			MAJOR_VERSION = 0x0003,
			MINOR_VERSION = 0x0000
#else
			MAJOR_VERSION = 0x0001,
			MINOR_VERSION = 0x2000
#endif
		};

		//.....................................................................
		// These values control which other versions this program will connect
		// to.  Keep them current!
		// If CHEAT is defined, the program will only connect to itself; these
		// values aren't used.
		//.....................................................................
		enum VersionRangeEnum {
#ifdef FIXIT_VERSION_3
			//	ajw - We can only play against same version.
			MIN_VERSION = VERSION_RA_300,
			MAX_VERSION = VERSION_RA_300
#else
			MIN_VERSION = VERSION_RED_ALERT_104, //0x00010000,	// Version: 1.0
			MAX_VERSION = VERSION_AFTERMATH		 //0x00012000	// Version: 1.2
#endif
		};

		//.....................................................................
		// This is the program's version number, stored internally.
		//.....................................................................
		unsigned long Version;
		unsigned short MajorVer;
		unsigned short MinorVer;

		//.....................................................................
		// This array is used for formatting the version # as a string
		//.....................................................................
		char VersionName[30];

		//.....................................................................
		// This array contains special version labels (such as "Beta"), stored
		// in the file VERSION.TXT.  If the file isn't present, no label is
		// shown.
		//.....................................................................
		char VersionText[16];

		//.....................................................................
		// Values used for "Version Clipping"
		//.....................................................................
		unsigned long MinClipVer;
		unsigned long MaxClipVer;

		//.....................................................................
		// Bitfield Flags
		// IsInitialized: is set if the VERSION.TXT file has been read
		//.....................................................................
		unsigned VersionInit :		1;
		unsigned MajorInit :		1;
		unsigned MinorInit :		1;
		unsigned TextInit :			1;
};
#endif
/************************** end of version.h *******************************/

