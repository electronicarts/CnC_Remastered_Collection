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

/* $Header: /CounterStrike/VERSION.CPP 14    3/16/97 10:16p Joe_b $ */
/***************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : VERSION.CPP                              *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 10/26/95                                 *
 *                                                                         *
 *                  Last Update : September 17, 1996 [JLB]                 *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   VersionClass::VersionClass -- Class constructor                       *
 *   VersionClass::Version_Number -- Returns program version number        *
 *   VersionClass::Major_Version -- returns major version #                *
 *   VersionClass::Minor_Version -- returns minor version (revision) number*
 *   VersionClass::Version_Name -- returns version # as char string        *
 *   VersionClass::Read_Text_String -- reads version text string from disk *
 *   VersionClass::Version_Protocol -- returns default protocol for version*
 *   VersionClass::Init_Clipping -- Initializes version clipping           *
 *   VersionClass::Clip_Version -- "clips" the given version range         *
 *   VersionClass::Min_Version -- returns lowest version # to connect to   *
 *   VersionClass::Max_Version -- returns highest version # to connect to  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if (0)//PG
#include "function.h"

#ifdef FIXIT_VERSION_3
#include "rawolapi.h"		//	For version number.
#endif

/****************************** Globals ************************************/
//---------------------------------------------------------------------------
// This is a table of version numbers # the communications protocol used for
// that version number.  It's used by the game owner to determine the
// protocol to be used for a given session.
//
// This table needs to be updated every time a new communications protocol
// is implemented, not every time a new version is created.
//
// A given protocol is used from its corresponding version #, up to (but not
// including) the next version number in the table.  The last protocol in
// the table is the default protocol for this version.
//---------------------------------------------------------------------------
static VersionProtocolType VersionProtocol[] = {
	{0x00001000,COMM_PROTOCOL_SINGLE_NO_COMP},	// (obsolete)
	{0x00002000,COMM_PROTOCOL_SINGLE_E_COMP},		// (obsolete)
	{0x00010000,COMM_PROTOCOL_MULTI_E_COMP},
};


/***************************************************************************
 * VersionClass::VersionClass -- Class constructor                         *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995 BRR : Created.                                             *
 *   09/17/1996 JLB : Converted to used initializer list.                  *
 *=========================================================================*/
VersionClass::VersionClass(void) :
	Version(0),
	MajorVer(0),
	MinorVer(0),
	MinClipVer(0),
	MaxClipVer(0),
	VersionInit(false),
	MajorInit(false),
	MinorInit(false),
	TextInit(false)
{
	VersionText[0] = '\0';
	VersionName[0] = '\0';
}


/***************************************************************************
 * VersionClass::Version_Number -- Returns program version number          *
 *                                                                         *
 * Version Number Format:																	*
 * Non-CHEAT format:																			*
 *   Byte 3,2: major version (printed to the left of a decimal)				*
 *   Byte 1,0: minor version (printed to the right of a decimal)				*
 *   Thus, version 1.07 would appear as 0x0001 0700								*
 *                                                                         *
 *   This format guarantees that a greater-than or less-than comparison		*
 *   will work on version numbers.														*
 *                                                                         *
 * CHEAT format:																				*
 *   Byte 3: Month #																			*
 *   Byte 2: Day #																			*
 *   Byte 1: Hour #																			*
 *   Byte 0: Minute #																		*
 *                                                                         *
 * This format guarantees a unique version number for each compile (as		*
 * long as they're a minute or more apart), with increasing version #'s		*
 * for later times.																			*
 *                                                                         *
 * Either format should be printed in hex.                                 *
 *                                                                         *
 * This routine also fills in a text string (retrieved with Version_Text), *
 * which may contain a custom string (such as "Beta"); this string is		*
 * read from the file VERSION.TXT.														*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		Version number																			*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Don't call this function until the file system has been init'd!		*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995 BRR : Created.                                             *
 *=========================================================================*/

//ajw Note: This function is no longer called. MIN_VERSION is now incorrect, but I don't have time
//	for a full rebuild (3 hrs!), and as MIN_VERSION is no longer referred to, I'm going to leave it.
//	Really, it should be deleted or commented out.
//	Version number used is now GAME_VERSION.
//	Note also that VERSION_RA_300 is wrong, but not used.

unsigned long VersionClass::Version_Number(void)
{
	//------------------------------------------------------------------------
	// Read the text description, if there is one
	//------------------------------------------------------------------------
	if (!TextInit) {
		Read_Text_String();
		TextInit = 1;
	}

	//------------------------------------------------------------------------
	// If the version has already been set, just return it.
	//------------------------------------------------------------------------
	if (VersionInit) {
		return (Version);
	}

	//------------------------------------------------------------------------
	// Generate the version #
	//------------------------------------------------------------------------
	Version = ((Major_Version() << 16) | Minor_Version());
	VersionInit = 1;

	return (Version);

}	/* end of Version_Number */


/***************************************************************************
 * VersionClass::Major_Version -- returns major version #                  *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		Major Version number																	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Don't call this function until the file system has been init'd!		*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned short VersionClass::Major_Version(void)
{
#ifdef DEV_VERSION
	static char * date = __DATE__;		// format: Mmm dd yyyy
	static char const * months = "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC";
	char buf[10];
	char * ptr;
	char * tok;
	int monthnum;
	int daynum;
#endif

	//------------------------------------------------------------------------
	// Read the text description, if there is one
	//------------------------------------------------------------------------
	if (!TextInit) {
		Read_Text_String();
		TextInit = 1;
	}

	//------------------------------------------------------------------------
	// If the major version # is already set, just return it.
	//------------------------------------------------------------------------
	if (MajorInit) {
		return (MajorVer);
	}

	//------------------------------------------------------------------------
	// For a development version, use the date (month & day) as the major
	// version number.
	//------------------------------------------------------------------------
#ifdef DEV_VERSION
	//........................................................................
	//	Fetch the month and place in the high byte.
	//........................................................................
	strupr(date);
	tok = strtok(date, " ");
	ptr = strstr(months, tok);
	if (ptr) {
		monthnum = (((ptr - months) / 3) + 1);
	} else {
		monthnum = 0;
	}

	//........................................................................
	// Convert the month number to a hex counterpart (so, when it's printed
	// in hex, it will read correctly.)
	//........................................................................
	sprintf(buf,"%d",monthnum);
	sscanf(buf,"%x",&monthnum);

	//........................................................................
	//	Fetch the date and place that in the low byte.
	//........................................................................
	tok = strtok(NULL, " ");
	if (tok) {
		daynum = atoi(tok);
	} else {
		daynum = 0;
	}

	//........................................................................
	// Convert the day number to a hex counterpart
	//........................................................................
	sprintf(buf,"%d",daynum);
	sscanf(buf,"%x",&daynum);

	MajorVer = ((monthnum << 8) | daynum);

	//------------------------------------------------------------------------
	// For a non-development version, use the hard-coded minor version number.
	//------------------------------------------------------------------------
#else
	MajorVer = MAJOR_VERSION;
#endif

	MajorInit = 1;

	return (MajorVer);

}	/* end of Major_Version */


/***************************************************************************
 * VersionClass::Minor_Version -- returns minor version (revision) number  *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		Minor Version number																	*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Don't call this function until the file system has been init'd!		*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned short VersionClass::Minor_Version(void)
{
#ifdef DEV_VERSION
	static char * time = __TIME__;		// format: hh:mm:ss
	char * tok;
	char buf[10];
	int hournum;
	int minnum;
#endif

	//------------------------------------------------------------------------
	// Read the text description, if there is one
	//------------------------------------------------------------------------
	if (!TextInit) {
		Read_Text_String();
		TextInit = 1;
	}

	//------------------------------------------------------------------------
	// If the minor version # is already set, just return it.
	//------------------------------------------------------------------------
	if (MinorInit) {
		return (MinorVer);
	}

	//------------------------------------------------------------------------
	// For in-development versions, use the time (hour & min) as the minor
	// version
	//------------------------------------------------------------------------
#ifdef DEV_VERSION
	//........................................................................
	//	Fetch the hour and place that in the last two digit positions.
	//........................................................................
	tok = strtok(time, ": ");
	if (tok) {
		hournum = atoi(tok);
	} else {
		hournum = 0;
	}

	//........................................................................
	// Convert the hour number to a hex counterpart (so, when it's printed
	// in hex, it will read correctly.)
	//........................................................................
	sprintf(buf,"%d",hournum);
	sscanf(buf,"%x",&hournum);

	//........................................................................
	//	Fetch the minute and place that in the last two digit positions.
	//........................................................................
	tok = strtok(NULL, ": ");
	if (tok) {
		minnum = atoi(tok);
	} else {
		minnum = 0;
	}

	//........................................................................
	// Convert the minute number to a hex counterpart
	//........................................................................
	sprintf(buf,"%d",minnum);
	sscanf(buf,"%x",&minnum);

	MinorVer = ((hournum << 8) | minnum);

	//------------------------------------------------------------------------
	// For a non-development version, use the hard-coded minor revision number.
	//------------------------------------------------------------------------
#else

#ifdef FIXIT_VERSION_3			//	Insanity. CS installation should not have affected version number. ajw

	MinorVer = MINOR_VERSION;

#else	//	FIXIT_VERSION_3

#ifdef FIXIT_CSII
	MinorVer = MINOR_VERSION;
	if (Is_Counterstrike_Installed()) {
		MinorVer = MINOR_VERSION - 1;
	}
#else
	#ifdef FIXIT_VERSION
	/* If counterstrike is not installed then we report version 1.06
	 * otherwise we report ourselves as 1.08
	 */
	if (Is_Counterstrike_Installed() == false) {
		MinorVer = (MINOR_VERSION - CS_MINOR_VERSION_MODIFIER);
	} else {
		MinorVer = MINOR_VERSION;
	}
	#else
	MinorVer = MINOR_VERSION;
	#endif
#endif

#endif	//	FIXIT_VERSION_3

#endif

	MinorInit = 1;

	return (MinorVer);

}	/* end of Minor_Version */


/***************************************************************************
 * VersionClass::Version_Name -- returns version # as char string          *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		ptr to name																				*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/30/1995 BRR : Created.                                             *
 *=========================================================================*/
char * VersionClass::Version_Name(void)
{
	//------------------------------------------------------------------------
	// For developmental versions, just use the major & minor version #'s
	//------------------------------------------------------------------------
#ifdef DEV_VERSION
	sprintf(VersionName, "%x.%x", VerNum.Major_Version(), VerNum.Minor_Version());

	//------------------------------------------------------------------------
	// For final versions, trim 0's off the minor version
	//------------------------------------------------------------------------
#else
	unsigned short adjusted_minor;
	int i;

	adjusted_minor = Minor_Version();
	for (i = 0; i < 4; i++) {
		if ( (adjusted_minor & 0x000f) != 0) {
			break;
		}
		adjusted_minor >>= 4;
	}

	sprintf(VersionName, "%x.%x", VerNum.Major_Version(), adjusted_minor);
#endif

	return (VersionName);

}	/* end of Version_Name */


/***************************************************************************
 * VersionClass::Read_Text_String -- reads version # text string from disk *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Don't call this function until the file system has been init'd!		*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995 BRR : Created.                                             *
 *=========================================================================*/
void VersionClass::Read_Text_String(void)
{
	RawFileClass file("VERSION.TXT");

	if (file.Is_Available()) {
		file.Read(VersionText, sizeof(VersionText));
		VersionText[sizeof(VersionText)-1] = '\0';
		while (VersionText[strlen(VersionText)-1] == '\r') {
			VersionText[strlen(VersionText)-1] = '\0';
		}
	} else {
		VersionText[0] = '\0';
	}

}	/* end of Read_Text_String */


/***************************************************************************
 * VersionClass::Version_Protocol -- returns default protocol for version  *
 *                                                                         *
 * INPUT:                                                                  *
 *		version		version # to look up													*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		protocol value to use for that version #										*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995 BRR : Created.                                             *
 *=========================================================================*/
CommProtocolType VersionClass::Version_Protocol(unsigned long version)
{
	int i,j;

	//------------------------------------------------------------------------
	// Compute # entries in the VersionProtocol table
	//------------------------------------------------------------------------
	j = sizeof (VersionProtocol) / sizeof(VersionProtocolType);

	//------------------------------------------------------------------------
	// Search backwards through the table, finding the first entry for which
	// the given version # is >= the table's; this is the range containing
	// the given version number.
	//------------------------------------------------------------------------
	for (i = j - 1; i >= 0; i--) {
		if (version >= VersionProtocol[i].Version) {
			return (VersionProtocol[i].Protocol);
		}
	}

	//------------------------------------------------------------------------
	// If no range was found for the given version, return the highest
	// possible protocol.  (If version clipping is being done properly, this
	// case should never happen, but never say never.)
	//------------------------------------------------------------------------
	return (VersionProtocol[j-1].Protocol);

}	/* end of Version_Protocol */


/***************************************************************************
 * VersionClass::Init_Clipping -- Initializes version clipping             *
 *                                                                         *
 * Initializes the Min & Max clip version #'s to the min & max values		*
 * defined for this program.  This sets the initial range for use by			*
 * the Clip_Version routine.																*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		The DEV_VERSION version of this routine calls Version_Number(), so 	*
 *		don't	call this routine until the file system has been initialized!	*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995 BRR : Created.                                             *
 *=========================================================================*/
void VersionClass::Init_Clipping(void)
{
	MinClipVer = Min_Version();
	MaxClipVer = Max_Version();

}	/* end of Init_Clipping */


/***************************************************************************
 * VersionClass::Clip_Version -- "clips" the given version range           *
 *                                                                         *
 * This routine compares another program's supported min/max version 		*
 * range with the range currently defined by 'MinClipVer' and 'MaxClipVer'.*
 * If there is overlap in the two ranges, Min & MaxClipVer are adjusted		*
 * to the bounds of the overlap. The routine returns the largest version	*
 * number shared by the ranges (MaxClipVer).											*
 *																									*
 * Thus, by calling Init_Clipping(), then a series of Clip_Version() calls,*
 * a mutually-acceptable range of version #'s may be negotiated between		*
 * different versions of this program.  The max shared version may then		*
 * be used to decide upon a communications protocol that all programs		*
 * support.																						*
 *                                                                         *
 * INPUT:                                                                  *
 *		minver		min version to clip to												*
 *		maxver		max version to clip to												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		highest clipped version #															*
 *		0 = given range is below our current range									*
 *		0xFFFFFFFF = given range is above our current range						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		Be sure Init_Clipping() was called before performing a clipping		*
 *		session.																					*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned long VersionClass::Clip_Version(unsigned long minver,
	unsigned long maxver)
{
	//------------------------------------------------------------------------
	// If the given range is outside & above our own, return an error.
	//------------------------------------------------------------------------
	if (minver > MaxClipVer)
		return (0xffffffff);

	//------------------------------------------------------------------------
	// If the given range is outside & below our own, return an error.
	//------------------------------------------------------------------------
	if (maxver < MinClipVer)
		return (0);

	//------------------------------------------------------------------------
	// Clip the lower range value
	//------------------------------------------------------------------------
	if (minver > MinClipVer)
		MinClipVer = minver;

	//------------------------------------------------------------------------
	// Clip the upper range value
	//------------------------------------------------------------------------
	if (maxver < MaxClipVer)
		MaxClipVer = maxver;

	//------------------------------------------------------------------------
	// Return the highest version supported by the newly-adjusted range.
	//------------------------------------------------------------------------
	return (MaxClipVer);

}	/* end of Clip_Version */


/***************************************************************************
 * VersionClass::Min_Version -- returns lowest version # to connect to     *
 *                                                                         *
 * Returns the minimum version # this program will connect to.					*
 *                                                                         *
 * If DEV_VERSION is defined, this routine returns the current version, so *
 * this program will only connect to an exact copy of itself.					*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		min version #																			*
 *                                                                         *
 * WARNINGS:                                                               *
 *		The DEV_VERSION version of this routine calls Version_Number(), so 	*
 *		don't	call this routine until the file system has been initialized!	*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned long VersionClass::Min_Version(void)
{
#ifdef DEV_VERSION
	return (Version_Number());
#else

#ifdef FIXIT_VERSION_3

	//	Note! I'm no longer using MIN_VERSION, MAX_VERSION, or VERSION_RA_300!
	//	But no time to do three full rebuilds right now, so I'm not deleting them from the header file...   ajw
	return GAME_VERSION;

#else	//	FIXIT_VERSION_3

#ifdef FIXIT_VERSION
	if ( Is_Counterstrike_Installed() ) {
		return (MIN_VERSION - 1);
	}
	return (MIN_VERSION);
#else
	if ( Is_Counterstrike_Installed() ){
		return (MIN_VERSION - CS_MINOR_VERSION_MODIFIER);
	}else{
		return (MIN_VERSION);
	}
#endif

#endif	//	FIXIT_VERSION_3

#endif

}	/* end of Min_Version */


/***************************************************************************
 * VersionClass::Max_Version -- returns highest version # to connect to    *
 *                                                                         *
 * Returns the maximum version # this program will connect to.					*
 *                                                                         *
 * If DEV_VERSION is defined, this routine returns the current version, so *
 * this program will only connect to an exact copy of itself.					*
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		max version #																			*
 *                                                                         *
 * WARNINGS:                                                               *
 *		The DEV_VERSION version of this routine calls Version_Number(), so 	*
 *		don't	call this routine until the file system has been initialized!	*
 *                                                                         *
 * HISTORY:                                                                *
 *   10/26/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned long VersionClass::Max_Version(void)
{
#ifdef DEV_VERSION
	return (Version_Number());
#else

#ifdef FIXIT_VERSION_3

	//	Note! I'm no longer using MIN_VERSION, MAX_VERSION, or VERSION_RA_300!
	//	But no time to do three full rebuilds right now, so I'm not deleting them from the header file...   ajw
	return GAME_VERSION;

#else

#ifdef FIXIT_CSII	//	checked - ajw
	return (MAX_VERSION);
#else
 #ifdef FIXIT_VERSION
	if (Is_Counterstrike_Installed() == false) {
		return (MAX_VERSION - CS_MINOR_VERSION_MODIFIER);
	} else {
		return (MAX_VERSION);
	}
 #else
	if ( Is_Counterstrike_Installed() ){
		return (MAX_VERSION + CS_MINOR_VERSION_MODIFIER);
	}else{
		return (MAX_VERSION);
	}
 #endif
#endif
#endif

#endif	//	FIXIT_VERSION_3

}	/* end of Max_Version */


char const * Version_Name(void)
{
#ifdef NEVER
	static char buffer[32];

	/*
	**	Fetch the day and month components from the current
	**	build date.
	*/
	static char * date = __DATE__;		// format: Mmm dd yyyy
	strupr(date);
	char const * tok = strtok(date, " ");
	static char const * months = "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC";
	char const * ptr = strstr(months, tok);
	int monthnum = 0;
	if (ptr != NULL) {
		monthnum = (((ptr - months) / 3) + 1);
	}

	tok = strtok(NULL, " ");
	int daynum = 0;
	if (tok != NULL) {
		daynum = atoi(tok);
	}

	/*
	**	Fetch the time components from the current build time.
	*/
	static char * time = __TIME__;		// format: hh:mm:ss
	tok = strtok(time, ": ");
	int hournum = 0;
	if (tok != NULL) {
		hournum = atoi(tok);
	}

	tok = strtok(NULL, ": ");
	int minnum = 0;
	if (tok != NULL) {
		minnum = atoi(tok);
	}

	sprintf(buffer, "%02d%02d%02d", monthnum, daynum, (hournum*4) + (minnum / 15));
	return(buffer);
#else

	static char buffer[128];

	memset(buffer, '\0', sizeof(buffer));

#ifdef FIXIT_VERSION_3
	strcpy( buffer, "3.03" );

	#ifdef ENGLISH
	strcat(buffer, "E");
	#else
		#ifdef GERMAN
		strcat(buffer, "G");
		#else
			#ifdef FRENCH
			strcat(buffer, "F");
			#endif
		#endif
	#endif

#else	//	FIXIT_VERSION_3

#ifdef FIXIT_PATCH_108
	//strcpy(buffer, "1.08PE");
    strcpy(buffer, "1.08P");

#ifdef FIXIT_CSII
	strcpy(buffer,"2.00");
#ifdef DEV_VERSION
	strcpy(buffer,VerNum.Version_Name());
#endif
#ifdef DEV_VER_NAME
	strcpy(buffer,__DATE__);		// format: Mmm dd yyyy
#endif
#endif

	#ifdef ENGLISH
	strcat(buffer, "E");
	#else
		#ifdef GERMAN
		strcat(buffer, "G");
		#else
			#ifdef FRENCH
			strcat(buffer, "F");
			#endif
		#endif
	#endif

#else
	strcpy(buffer, "1.07E");
#endif

#endif //	FIXIT_VERSION_3

	if (Is_Counterstrike_Installed ()){
		strcat (buffer, "CS");
	}
	if (Is_Aftermath_Installed()) {
		strcat (buffer, "AM");
	}

#if(TEN)
	strcat(buffer, "Ten");		// Ten version
#endif

#if(MPATH)
	strcat(buffer, "MPath");	// MPath version
#endif

	RawFileClass file("VERSION.TXT");
	if (file.Is_Available()) {
		strcat(buffer, "\r");
		file.Read(&buffer[strlen(buffer)], 25);
	}
	return(buffer);
#endif
}
#endif