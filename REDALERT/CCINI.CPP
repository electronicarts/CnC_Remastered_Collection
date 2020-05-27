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

/* $Header: /CounterStrike/CCINI.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CCINI.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/24/96                                                     *
 *                                                                                             *
 *                  Last Update : November 1, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CCINIClass::Calculate_Message_Digest -- Calculate a message digest for the current databas*
 *   CCINIClass::Get_AnimType -- Fetch an animation type number from the INI database.         *
 *   CCINIClass::Get_ArmorType -- Fetches the armor type from the INI database.                *
 *   CCINIClass::Get_Buildings -- Fetch a building bitfield from the INI database.             *
 *   CCINIClass::Get_BulletType -- Fetch the bullet identifier from the INI database.          *
 *   CCINIClass::Get_CrateType -- Fetches a crate type value from the INI database.            *
 *   CCINIClass::Get_HousesType -- Fetch a house identifier from the INI database.             *
 *   CCINIClass::Get_Lepton -- Fetches a lepton value from the INI database.                   *
 *   CCINIClass::Get_MPHType -- Fetches the speed value as a number from 0 to 100.             *
 *   CCINIClass::Get_OverlayType -- Fetch the overlay identifier from the INI database.        *
 *   CCINIClass::Get_Owners -- Fetch the owners (list of house bits).                          *
 *   CCINIClass::Get_SourceType -- Fetch the source (edge) type from the INI database.         *
 *   CCINIClass::Get_TerrainType -- Fetch the terrain type identifier from the INI database.   *
 *   CCINIClass::Get_TheaterType -- Fetch the theater type from the INI database.              *
 *   CCINIClass::Get_ThemeType -- Fetch the theme identifier.                                  *
 *   CCINIClass::Get_TriggerType -- Fetch the trigger type identifier from the INI database.   *
 *   CCINIClass::Get_Unique_ID -- Fetch a unique identifier number for the INI file.           *
 *   CCINIClass::Get_VQType -- Fetch the VQ movie identifier from the INI database.            *
 *   CCINIClass::Get_VocType -- Fetch a voc (sound effect) from the INI database.              *
 *   CCINIClass::Get_WarheadType -- Fetch the warhead type from the INI database.              *
 *   CCINIClass::Get_WeaponType -- Fetches the weapon type from the INI database.              *
 *   CCINIClass::Invalidate_Message_Digest -- Flag message digest as being invalid.            *
 *   CCINIClass::Load -- Load the INI database from the data stream specified.                 *
 *   CCINIClass::Load -- Load the INI database from the file specified.                        *
 *   CCINIClass::Put_AnimType -- Stores the animation identifier to the INI database.          *
 *   CCINIClass::Put_ArmorType -- Store the armor type to the INI database.                    *
 *   CCINIClass::Put_Buildings -- Store a building list to the INI database.                   *
 *   CCINIClass::Put_BulletType -- Store the projectile identifier into the INI database.      *
 *   CCINIClass::Put_CrateType -- Stores the crate value in the section and entry specified.   *
 *   CCINIClass::Put_HousesType -- Store a house identifier to the INI database.               *
 *   CCINIClass::Put_Lepton -- Stores a lepton value to the INI database.                      *
 *   CCINIClass::Put_MPHType -- Stores the speed value to the section & entry specified.       *
 *   CCINIClass::Put_OverlayType -- Store the overlay identifier into the INI database.        *
 *   CCINIClass::Put_Owners -- Store the house bitfield to the INI database.                   *
 *   CCINIClass::Put_SourceType -- Store the source (edge) identifier to the INI database.     *
 *   CCINIClass::Put_TerrainType -- Store the terrain type number to the INI database.         *
 *   CCINIClass::Put_TheaterType -- Store the theater identifier to the INI database.          *
 *   CCINIClass::Put_ThemeType -- Store the theme identifier to the INI database.              *
 *   CCINIClass::Put_TriggerType -- Store the trigger identifier to the INI database.          *
 *   CCINIClass::Put_VQType -- Store the VQ movie identifier into the INI database.            *
 *   CCINIClass::Put_VocType -- Store a sound effect identifier into the INI database.         *
 *   CCINIClass::Put_WarheadType -- Stores the warhead identifier to the INI database.         *
 *   CCINIClass::Put_WeaponType -- Store the weapon identifier to the INI database.            *
 *   CCINIClass::Save -- Pipes the INI database to the pipe specified.                         *
 *   CCINIClass::Save -- Save the INI data to the file specified.                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"


/***********************************************************************************************
 * CCINIClass::Load -- Load the INI database from the file specified.                          *
 *                                                                                             *
 *    This routine will load the database from the file specified in much the same manner      *
 *    that the INIClass load function works. However, this class will examine the message      *
 *    digest (if present) and compare it to the actual digest. If they differ, a special       *
 *    return value is used. This will allow verification of the integrity of the ini data.     *
 *                                                                                             *
 * INPUT:   file  -- Reference to the file that will be read from.                             *
 *                                                                                             *
 *          withdigest  -- Should a message digest be examined when loaded. If there is a      *
 *                         mismatch detected, then an error will be returned.                  *
 *                                                                                             *
 * OUTPUT:  If the file was not read, returns 0. If the file was read ok, returns 1. If the    *
 *          file was read ok, but the digest doesn't verify, returns 2.                        *
 *                                                                                             *
 * WARNINGS:   If no message digest was present in the INI file, then no verification can      *
 *             be performed.                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *   08/21/1996 JLB : Handles digest control.                                                  *
 *=============================================================================================*/
bool CCINIClass::Load(FileClass & file, bool withdigest)
{
	return(Load(FileStraw(file), withdigest));
}


/***********************************************************************************************
 * CCINIClass::Load -- Load the INI database from the data stream specified.                   *
 *                                                                                             *
 *    This will load the INI database and in the process, it will fetch and verify any         *
 *    message digest present.                                                                  *
 *                                                                                             *
 * INPUT:   straw -- The data stream to fetch the INI data from.                               *
 *                                                                                             *
 *          withdigest  -- Should a message digest be examined when loaded. If there is a      *
 *                         mismatch detected, then an error will be returned.                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the database loaded ok? (hack: returns "2" if digest doesn't match).     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *   08/21/1996 JLB : Handles message digest control.                                          *
 *=============================================================================================*/
int CCINIClass::Load(Straw & file, bool withdigest)
{
	bool ok = INIClass::Load(file);

	Invalidate_Message_Digest();
	if (ok && withdigest) {

		/*
		**	If a digest is present, fetch it.
		*/
		unsigned char digest[20];
		int len = Get_UUBlock("Digest", digest, sizeof(digest));
		if (len > 0) {
			Clear("Digest");

			/*
			**	Calculate the message digest for the INI data that was read.
			*/
			Calculate_Message_Digest();

			/*
			**	If the message digests don't match, then return with the special error code.
			*/
			if (memcmp(digest, Digest, sizeof(digest)) != 0) {
				return(2);
			}
		}
	}
	return(ok);
}


/***********************************************************************************************
 * CCINIClass::Save -- Save the INI data to the file specified.                                *
 *                                                                                             *
 *    This routine will save the INI data to the file. It will add a message digest so that    *
 *    validity check can be performed when the INI data is subsequently read.                  *
 *                                                                                             *
 * INPUT:   file  -- Reference to the file to write the INI data to.                           *
 *                                                                                             *
 *          withdigest  -- Should a message digest be generated and saved with the INI         *
 *                         data file?                                                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the INI data saved?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *   08/21/1996 JLB : Handles message digest control.                                          *
 *=============================================================================================*/
int CCINIClass::Save(FileClass & file, bool withdigest) const
{
	return(Save(FilePipe(file), withdigest));
}


/***********************************************************************************************
 * CCINIClass::Save -- Pipes the INI database to the pipe specified.                           *
 *                                                                                             *
 *    This routine will pipe the INI data to the pipe segment specified. It is functionally    *
 *    the same as the save operation. A message digest is added to the output data so that     *
 *    validity check can occur during a subsequent read.                                       *
 *                                                                                             *
 * INPUT:   straw -- Reference to the pipe that will receive the output ini data stream.       *
 *                                                                                             *
 *          withdigest  -- Should a message digest be generated and saved with the INI         *
 *                         data file?                                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes output to the pipe.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *   08/21/1996 JLB : Handles message digest control.                                          *
 *=============================================================================================*/
int CCINIClass::Save(Pipe & pipe, bool withdigest) const
{
	if (!withdigest) {
		return(INIClass::Save(pipe));
	}

	/*
	**	Just in case these entries are present, clear them out.
	*/
	((CCINIClass *)this)->Clear("Digest");

	/*
	**	Calculate what the new digest should be.
	*/
	((CCINIClass *)this)->Calculate_Message_Digest();

	/*
	**	Store the actual digest into the INI database.
	*/
	((CCINIClass *)this)->Put_UUBlock("Digest", Digest, sizeof(Digest));

	/*
	**	Output the database to the pipe specified.
	*/
	int length = INIClass::Save(pipe);

	/*
	**	Remove the digest from the database. It shouldn't stick around as if it were real data
	**	since it isn't really part of the INI database proper.
	*/
	((CCINIClass *)this)->Clear("Digest");

	/*
	**	Finally, return with the total number of bytes send out the pipe.
	*/
	return(length);
}


static inline int _Scale_To_256(int val)
{
	val = min(val, 100);
	val = max(val, 0);
	val = ((val * 256) / 100);
	val = min(val, 255);
	return(val);
}


/***********************************************************************************************
 * CCINIClass::Get_Lepton -- Fetches a lepton value from the INI database.                     *
 *                                                                                             *
 *    This routine will fetch the lepton value as if it were expressed as cells. Example;      *
 *    a value of 1 would mean 256 in leptons.                                                  *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to look under.                                  *
 *                                                                                             *
 *          entry    -- The entry identifier to find.                                          *
 *                                                                                             *
 *          defvalue -- The default value to use if the specified section and entry could      *
 *                      not be located.                                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the lepton value of the section & entry specified. If not found, then *
 *          the default value is returned.                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
LEPTON CCINIClass::Get_Lepton(char const * section, char const * entry, LEPTON defvalue) const
{
	fixed result = Get_Fixed(section, entry, fixed(defvalue, CELL_LEPTON_W));
	return(result * CELL_LEPTON_W);
}


/***********************************************************************************************
 * CCINIClass::Put_Lepton -- Stores a lepton value to the INI database.                        *
 *                                                                                             *
 *    This routine will store the lepton value as if it were expressed in cells. Example;      *
 *    A lepton of 128 will be stored as ".5".                                                  *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to store the value under.                       *
 *                                                                                             *
 *          entry    -- The entry to store the lepton value at.                                *
 *                                                                                             *
 *          value    -- The lepton value to store.                                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the lepton value stored?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_Lepton(char const * section, char const * entry, LEPTON value)
{
	return(Put_Fixed(section, entry, fixed(value, CELL_LEPTON_W)));
}


/***********************************************************************************************
 * CCINIClass::Get_MPHType -- Fetches the speed value as a number from 0 to 100.               *
 *                                                                                             *
 *    This routine will fetch the speed value as if it were expressed as a number from 0       *
 *    to 100. The value of 100 would translate into a speed of 256 leptons per game frame.     *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to search for the entry under.                  *
 *                                                                                             *
 *          entry    -- The entry identifier to find.                                          *
 *                                                                                             *
 *          defvalue -- The default speed value to use if the entry could not be located.      *
 *                                                                                             *
 * OUTPUT:  Returns with the speed value. If no entry could be found, then the default value   *
 *          will be returned.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
MPHType CCINIClass::Get_MPHType(char const * section, char const * entry, MPHType defvalue) const
{
	int val = Get_Int(section, entry, ((int)defvalue * 100) / 256);
	return (MPHType(_Scale_To_256(val)));
}


/***********************************************************************************************
 * CCINIClass::Put_MPHType -- Stores the speed value to the section & entry specified.         *
 *                                                                                             *
 *    Use this routine to store a speed value into the INI database. The number stored will    *
 *    be in a 0..100 format. A speed of 256 leptons per tick would be stored as 100.           *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to store the entry under.                       *
 *                                                                                             *
 *          entry    -- The entry identifier to store the speed value to.                      *
 *                                                                                             *
 *          value    -- The speed value to store.                                              *
 *                                                                                             *
 * OUTPUT:  bool; Was the speed value stored?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_MPHType(char const * section, char const * entry, MPHType value)
{
	return(Put_Int(section, entry, ((int)value * 100) / 256));
}


/***********************************************************************************************
 * CCINIClass::Get_Owners -- Fetch the owners (list of house bits).                            *
 *                                                                                             *
 *    Use this to fetch a house bit array value from the INI database. This value will be      *
 *    various bit positions set (1 << house#) for each house specified in the database.        *
 *    Houses can be specified in series by the house name separated by commas or by the        *
 *    special group names of "soviet", and "allies" to cover the houses that are members of    *
 *    these groups.                                                                            *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to search for the entry under.                  *
 *                                                                                             *
 *          entry    -- The entry identifier to search for.                                    *
 *                                                                                             *
 *          defvalue -- The default house bitfield to use if the entry could not be found.     *
 *                                                                                             *
 * OUTPUT:  Returns with the house bitfield value. If the entry could not be found, then the   *
 *          default value is returned.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
long CCINIClass::Get_Owners(char const * section, char const * entry, long defvalue) const
{
	char buffer[128];
	long ownable = defvalue;

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {

		ownable = 0;
		char * name = strtok(buffer, ",");

		while (name) {
			ownable |= Owner_From_Name(name);
			name = strtok(NULL, ",");
		}
	}
	return(ownable);
}


/***********************************************************************************************
 * CCINIClass::Put_Owners -- Store the house bitfield to the INI database.                     *
 *                                                                                             *
 *    Use this routine to store the house bitfield data into the database. The bitfield format *
 *    matches the format used by the Get_Owners function. Example; if both England and         *
 *    Spain were specified in the bitfield, the entry would be stored as "England,Spain".      *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to store the entry under.                       *
 *                                                                                             *
 *          entry    -- The entry identifier that is assigned the value.                       *
 *                                                                                             *
 *          value    -- The value to assign to the entry.                                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the entry stored in the INI database?                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_Owners(char const * section, char const * entry, long value)
{
	char buffer[128];

	buffer[0] = '\0';

	if ((value & HOUSEF_ALLIES) == HOUSEF_ALLIES) {
		strcat(buffer, "allies");
		value &= ~HOUSEF_ALLIES;
	}
	if ((value & HOUSEF_SOVIET) == HOUSEF_SOVIET) {
		if (buffer[0] != '\0') {
			strcat(buffer, ",");
		}
		strcat(buffer, "soviet");
		value &= ~HOUSEF_SOVIET;
	}

	for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		if ((value & (1 << house)) != 0) {
			if (buffer[0] != '\0') {
				strcat(buffer, ",");
			}
			strcat(buffer, HouseTypeClass::As_Reference(house).Name());
		}
	}

	if (buffer[0] != '\0') {
		return(Put_String(section, entry, buffer));
	}
	return(true);
}


/***********************************************************************************************
 * CCINIClass::Get_ArmorType -- Fetches the armor type from the INI database.                  *
 *                                                                                             *
 *    This routine will fetch the armor type from the database.                                *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search for the entry under.              *
 *                                                                                             *
 *          entry    -- Th identifier for the entry to search for.                             *
 *                                                                                             *
 *          defvalue -- The default value to use if the entry could not be located.            *
 *                                                                                             *
 * OUTPUT:  Returns with the armor type specified in the INI database. If it could not be      *
 *          found, then the default value is returned instead.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ArmorType CCINIClass::Get_ArmorType(char const * section, char const * entry, ArmorType defvalue) const
{
	char buffer[128];

	Get_String(section, entry, ArmorName[defvalue], buffer, sizeof(buffer));
	return(Armor_From_Name(buffer));
}


/***********************************************************************************************
 * CCINIClass::Put_ArmorType -- Store the armor type to the INI database.                      *
 *                                                                                             *
 *    Use this routine to store the specified armor type to the INI database.                  *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to store the entry under.                       *
 *                                                                                             *
 *          entry    -- The entry to store the value at.                                       *
 *                                                                                             *
 *          value    -- The value to store in the database.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the entry stored in the database?                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_ArmorType(char const * section, char const * entry, ArmorType value)
{
	return(Put_String(section, entry, ArmorName[value]));
}


/***********************************************************************************************
 * CCINIClass::Get_VocType -- Fetch a voc (sound effect) from the INI database.                *
 *                                                                                             *
 *    This routine will fetch a voc number from the database. The voc number will either       *
 *    be a valid sound effect or VOC_NONE if no match could be found.                          *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search for the entry under.              *
 *                                                                                             *
 *          entry    -- The entry to search for.                                               *
 *                                                                                             *
 *          defvalue -- The default value to return if the entry could not be located.         *
 *                                                                                             *
 * OUTPUT:  Returns with the sound effect (VocType) from the INI database. If the entry could  *
 *          not be located, then the default value is returned.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VocType CCINIClass::Get_VocType(char const * section, char const * entry, VocType defvalue) const
{
	char buffer[128];

	Get_String(section, entry, Voc_Name(defvalue), buffer, sizeof(buffer));
	return(Voc_From_Name(buffer));
}


/***********************************************************************************************
 * CCINIClass::Put_VocType -- Store a sound effect identifier into the INI database.           *
 *                                                                                             *
 *    Use this routine to store a voc identifier (stored a the text name of the sound) into    *
 *    the INI database.                                                                        *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to store the entry under.                   *
 *                                                                                             *
 *          entry    -- The entry to assign the value to.                                      *
 *                                                                                             *
 *          value    -- The sound effect to store to the entry.                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the sound effect entry stored?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_VocType(char const * section, char const * entry, VocType value)
{
	if (value == VOC_NONE) {
		return(Put_String(section, entry, "<none>"));
	}
	return(Put_String(section, entry, Voc_Name(value)));
}


/***********************************************************************************************
 * CCINIClass::Get_AnimType -- Fetch an animation type number from the INI database.           *
 *                                                                                             *
 *    This will fetch an AnimType number from the INI database. The anim is stored as a text   *
 *    name of the art file used for that anim.                                                 *
 *                                                                                             *
 * INPUT:   section  -- The section to search for the entry under.                             *
 *                                                                                             *
 *          entry    -- The entry to search for.                                               *
 *                                                                                             *
 *          defvalue -- The default AnimType to use if the entry could not be located.         *
 *                                                                                             *
 * OUTPUT:  Returns with the anim type specified in the database. If it could not be found,    *
 *          then the default value is returned.                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
AnimType CCINIClass::Get_AnimType(char const * section, char const * entry, AnimType defvalue) const
{
	char buffer[128];

	Get_String(section, entry, Anim_Name(defvalue), buffer, sizeof(buffer));
	return(Anim_From_Name(buffer));
}


/***********************************************************************************************
 * CCINIClass::Put_AnimType -- Stores the animation identifier to the INI database.            *
 *                                                                                             *
 *    This routine will store the animation identifier (stored as the text name of the art     *
 *    file it uses) to the INI database.                                                       *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to place the entry under.                       *
 *                                                                                             *
 *          entry    -- The entry identifier to assign the animation number to.                *
 *                                                                                             *
 *          value    -- The animation identifier to store with the entry.                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the animation identifier stored?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_AnimType(char const * section, char const * entry, AnimType value)
{
	if (value == ANIM_NONE) {
		return(Put_String(section, entry, "<none>"));
	}
	return(Put_String(section, entry, Anim_Name(value)));
}


UnitType CCINIClass::Get_UnitType(char const * section, char const * entry, UnitType defvalue) const
{
	char buffer[128];

	char const * def = "<none>";
	if (defvalue != UNIT_NONE) {
		def = UnitTypeClass::As_Reference(defvalue).Name();
	}
	Get_String(section, entry, def, buffer, sizeof(buffer));
	return(UnitTypeClass::From_Name(buffer));
}


bool CCINIClass::Put_UnitType(char const * section, char const * entry, UnitType value)
{
	if (value == UNIT_NONE) {
		return(Put_String(section, entry, "<none>"));
	}
	return(Put_String(section, entry, UnitTypeClass::As_Reference(value).Name()));
}



/***********************************************************************************************
 * CCINIClass::Get_WeaponType -- Fetches the weapon type from the INI database.                *
 *                                                                                             *
 *    This routine will fetch the weapon type from the INI database. The weapon type is        *
 *    stored as a custom identifier string.                                                    *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to search for the entry under.                  *
 *                                                                                             *
 *          entry    -- The entry identifier to search for.                                    *
 *                                                                                             *
 *          defvalue -- The default weapon value to return if the entry could not be located.  *
 *                                                                                             *
 * OUTPUT:  Returns with the weapon type specified by the entry. If the entry could not be     *
 *          found then the default value is returned.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
WeaponType CCINIClass::Get_WeaponType(char const * section, char const * entry, WeaponType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		return(Weapon_From_Name(buffer));
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_WeaponType -- Store the weapon identifier to the INI database.              *
 *                                                                                             *
 *    Store the weapon identifier (as custom string name) to the INI database.                 *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to store the entry under.                   *
 *                                                                                             *
 *          entry    -- Identifier to store the weapon identifier with.                        *
 *                                                                                             *
 *          value    -- The weapon identifier to store.                                        *
 *                                                                                             *
 * OUTPUT:  bool; Was the weapon identifier stored?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_WeaponType(char const * section, char const * entry, WeaponType value)
{
	if (value == WEAPON_NONE) {
		return(Put_String(section, entry, "<none>"));
	}
	return(Put_String(section, entry, WeaponTypeClass::As_Pointer(value)->Name()));
}


/***********************************************************************************************
 * CCINIClass::Get_WarheadType -- Fetch the warhead type from the INI database.                *
 *                                                                                             *
 *    Will fetch the warhead identifier from the INI database.                                 *
 *                                                                                             *
 * INPUT:   section  -- The identifier for the section to search for the entry under.          *
 *                                                                                             *
 *          entry    -- The entry to search for.                                               *
 *                                                                                             *
 *          defvalue -- The default return value to use if the entry could not be located.     *
 *                                                                                             *
 * OUTPUT:  Returns with the found warhead type. If the entry could not be found then the      *
 *          default warhead value is returned.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
WarheadType CCINIClass::Get_WarheadType(char const * section, char const * entry, WarheadType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		for (WarheadType wh = WARHEAD_FIRST; wh < WARHEAD_COUNT; wh++) {
			if (stricmp(WarheadTypeClass::As_Pointer(wh)->Name(), buffer) == 0) {
				return(wh);
			}
		}
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_WarheadType -- Stores the warhead identifier to the INI database.           *
 *                                                                                             *
 *    This will store the weapon identifier specified to the INI database.                     *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to store the entry under.                       *
 *                                                                                             *
 *          entry    -- The entry to store the warhead identifier.                             *
 *                                                                                             *
 *          value    -- The warhead identifier to store.                                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the warhead identifier stored to the database?                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_WarheadType(char const * section, char const * entry, WarheadType value)
{
	if (value == WARHEAD_NONE) {
		return(Put_String(section, entry, "<none>"));
	}
	return(Put_String(section, entry, WarheadTypeClass::As_Pointer(value)->Name()));
}


/***********************************************************************************************
 * CCINIClass::Get_OverlayType -- Fetch the overlay identifier from the INI database.          *
 *                                                                                             *
 *    This routine will fetch the overlay identifier from the database.                        *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search for the entry under.              *
 *                                                                                             *
 *          entry    -- The entry to search for.                                               *
 *                                                                                             *
 *          defvalue -- The default value to use if the entry could not be located.            *
 *                                                                                             *
 * OUTPUT:  Returns with the overlay identifier found. If it could not be found, then the      *
 *          default value is returned.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
OverlayType CCINIClass::Get_OverlayType(char const * section, char const * entry, OverlayType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		return(OverlayTypeClass::From_Name(buffer));
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_OverlayType -- Store the overlay identifier into the INI database.          *
 *                                                                                             *
 *    Use this routine to store the overlay identifier into the INI database.                  *
 *                                                                                             *
 * INPUT:   section  -- Identifier for to search for the entry under.                          *
 *                                                                                             *
 *          entry    -- The entry to search for.                                               *
 *                                                                                             *
 *          value    -- The overlay type value to store with the entry.                        *
 *                                                                                             *
 * OUTPUT:  bool; Was the overlay value stored?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_OverlayType(char const * section, char const * entry, OverlayType value)
{
	assert(value != OVERLAY_NONE);
	return(Put_String(section, entry, OverlayTypeClass::As_Reference(value).Name()));
}


/***********************************************************************************************
 * CCINIClass::Get_BulletType -- Fetch the bullet identifier from the INI database.            *
 *                                                                                             *
 *    Use this routine to fetch the bullet type identifier from the INI database.              *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to search for the entry under.                  *
 *                                                                                             *
 *          entry    -- The entry to search for.                                               *
 *                                                                                             *
 *          defvalue -- The default bullet type value to return if the entry could not be      *
 *                      located.                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the bullet type identifier found. If the entry could not be found     *
 *          then the default value is returned.                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletType CCINIClass::Get_BulletType(char const * section, char const * entry, BulletType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		for (BulletType proj = BULLET_FIRST; proj < BULLET_COUNT; proj++) {
			if (stricmp(BulletTypeClass::As_Reference(proj).Name(), buffer) == 0) {
//			if (stricmp(ProjectileNames[proj], buffer) == 0) {
				return(proj);
			}
		}
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_BulletType -- Store the projectile identifier into the INI database.        *
 *                                                                                             *
 *    This routine will store the projectile name (as the identifier) to the INI database.     *
 *                                                                                             *
 * INPUT:   section  -- The section to store the entry under.                                  *
 *                                                                                             *
 *          entry    -- The entry identifier to store the projectile value with.               *
 *                                                                                             *
 *          value    -- The projectile identifier to store.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the projectile identifier stored?                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_BulletType(char const * section, char const * entry, BulletType value)
{
	if (value == BULLET_NONE) {
		return(Put_String(section, entry, "<none>"));
	}
	return(Put_String(section, entry, BulletTypeClass::As_Reference(value).Name()));
//	return(Put_String(section, entry, ProjectileNames[value]));
}


/***********************************************************************************************
 * CCINIClass::Get_HousesType -- Fetch a house identifier from the INI database.               *
 *                                                                                             *
 *    Use this routine to fetch an individual house identifier from the INI database. This is  *
 *    somewhat similar to the Get_Owners function but is limited to a single house.            *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search for the entry under.              *
 *                                                                                             *
 *          entry    -- Identifier for the entry to search for.                                *
 *                                                                                             *
 *          defvalue -- The default value to use if the entry could not be located.            *
 *                                                                                             *
 * OUTPUT:  Returns with the house identifier if it was found. If not found, then the default  *
 *          value is returned.                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
HousesType CCINIClass::Get_HousesType(char const * section, char const * entry, HousesType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		return(HouseTypeClass::From_Name(buffer));
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_HousesType -- Store a house identifier to the INI database.                 *
 *                                                                                             *
 *    Use this routine to store the specified house identifier to the INI database.            *
 *                                                                                             *
 * INPUT:   section  -- The section to store the entry under.                                  *
 *                                                                                             *
 *          entry    -- Identifier for the entry to search for.                                *
 *                                                                                             *
 *          value    -- The house identifier to store in the database.                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the house identifier stored?                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_HousesType(char const * section, char const * entry, HousesType value)
{
	return(Put_String(section, entry, HouseTypeClass::As_Reference(value).Name()));
}


/***********************************************************************************************
 * CCINIClass::Get_VQType -- Fetch the VQ movie identifier from the INI database.              *
 *                                                                                             *
 *    Fetches the VQ movie name (identifier) from the INI database.                            *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search for the entry under.              *
 *                                                                                             *
 *          entry    -- Identifier for the entry to search for.                                *
 *                                                                                             *
 *          defvalue -- The default value to use if the entry could not be located.            *
 *                                                                                             *
 * OUTPUT:  Returns with the VQ movie identifier found. If the entry could not be located,     *
 *          then the default value is returned.                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VQType CCINIClass::Get_VQType(char const * section, char const * entry, VQType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		for (VQType vq = VQ_FIRST; vq < VQ_COUNT; vq++) {
			if (stricmp(buffer, VQName[vq]) == 0) {
				return(vq);
			}
		}
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_VQType -- Store the VQ movie identifier into the INI database.              *
 *                                                                                             *
 *    Use this routine to store the VQ movie identifier into the INI database.                 *
 *                                                                                             *
 * INPUT:   section  -- The section to store the entry under.                                  *
 *                                                                                             *
 *          entry    -- Identifier for the entry to store.                                     *
 *                                                                                             *
 *          value    -- The VQ movie identifier to store to the INI database.                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the VQ identifier stored?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_VQType(char const * section, char const * entry, VQType value)
{
	if (value == VQ_NONE) {
		return(Put_String(section, entry, "<none>"));
	}
	return(Put_String(section, entry, VQName[value]));
}


/***********************************************************************************************
 * CCINIClass::Get_TheaterType -- Fetch the theater type from the INI database.                *
 *                                                                                             *
 *    This will fetch the theater identifier from the INI database.                            *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search for the entry under.              *
 *                                                                                             *
 *          entry    -- Identifier for the entry to search for.                                *
 *                                                                                             *
 *          defvalue -- The default value to use if the entry could not be located.            *
 *                                                                                             *
 * OUTPUT:  Returns with the theater type found. If the entry could not be found, then the     *
 *          default value is returned.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TheaterType CCINIClass::Get_TheaterType(char const * section, char const * entry, TheaterType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		return(Theater_From_Name(buffer));
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_TheaterType -- Store the theater identifier to the INI database.            *
 *                                                                                             *
 *    Use this routine to store the theater name to the INI database.                          *
 *                                                                                             *
 * INPUT:   section  -- The section to store the entry under.                                  *
 *                                                                                             *
 *          entry    -- Identifier for the entry to store.                                     *
 *                                                                                             *
 *          value    -- The theater identifier to store.                                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the theater identifier stored?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_TheaterType(char const * section, char const * entry, TheaterType value)
{
	return(Put_String(section, entry, Theaters[value].Name));
}


/***********************************************************************************************
 * CCINIClass::Get_TriggerType -- Fetch the trigger type identifier from the INI database.     *
 *                                                                                             *
 *    This routine will fetch the trigger type identifier from the INI database.               *
 *                                                                                             *
 * INPUT:   section  -- The section to search for the entry under.                             *
 *                                                                                             *
 *          entry    -- Identifier of the entry to search for.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the trigger type pointer if a match was found. No match found will    *
 *          return a NULL.                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerTypeClass * CCINIClass::Get_TriggerType(char const * section, char const * entry) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		return(TriggerTypeClass::From_Name(buffer));
	}
	return(NULL);
}


/***********************************************************************************************
 * CCINIClass::Put_TriggerType -- Store the trigger identifier to the INI database.            *
 *                                                                                             *
 *    This routine will store the trigger (as its name) to the INI database.                   *
 *                                                                                             *
 * INPUT:   section  -- The section to store the entry under.                                  *
 *                                                                                             *
 *          entry    -- The entry name to store the trigger identifier to.                     *
 *                                                                                             *
 *          value    -- The trigger type to store. The trigger name will be stored.            *
 *                                                                                             *
 * OUTPUT:  bool; Was the trigger name stored?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_TriggerType(char const * section, char const * entry, TriggerTypeClass * value)
{
	return(Put_String(section, entry, value->Name()));
}


/***********************************************************************************************
 * CCINIClass::Get_ThemeType -- Fetch the theme identifier.                                    *
 *                                                                                             *
 *    This routine will fetch the theme identifier from the INI database.                      *
 *                                                                                             *
 * INPUT:   section  -- The section to search for the entry under.                             *
 *                                                                                             *
 *          entry    -- Identifier of the entry to search for.                                 *
 *                                                                                             *
 *          defvalue -- The default theme identifier to return if the entry could not be found.*
 *                                                                                             *
 * OUTPUT:  Returns with the theme identifier if it was found. If not found, then the default  *
 *          value is returned instead.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ThemeType CCINIClass::Get_ThemeType(char const * section, char const * entry, ThemeType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		return(Theme.From_Name(buffer));
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_ThemeType -- Store the theme identifier to the INI database.                *
 *                                                                                             *
 *    This routine will store the specified theme identifier to the INI database.              *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to store the entry under.                   *
 *                                                                                             *
 *          entry    -- Identifier for the entry to store the value to.                        *
 *                                                                                             *
 *          value    -- The theme identifier to store.                                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the theme identifier stored.                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_ThemeType(char const * section, char const * entry, ThemeType value)
{
	return(Put_String(section, entry, Theme.Base_Name(value)));
}


/***********************************************************************************************
 * CCINIClass::Get_SourceType -- Fetch the source (edge) type from the INI database.           *
 *                                                                                             *
 *    This routine will fetch the source (reinforcement edge) identifier from the INI          *
 *    database.                                                                                *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section that the entry will be searched under.      *
 *                                                                                             *
 *          entry    -- Identifier for the entry that will be searched for.                    *
 *                                                                                             *
 *          defvalue -- The default value to return if the entry could not be located.         *
 *                                                                                             *
 * OUTPUT:  Returns with the source type of the entry if found. If not found, then the         *
 *          default value is returned.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
SourceType CCINIClass::Get_SourceType(char const * section, char const * entry, SourceType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		return(Source_From_Name(buffer));
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_SourceType -- Store the source (edge) identifier to the INI database.       *
 *                                                                                             *
 *    This will store the source type (reinforcement edge) to the INI database.                *
 *                                                                                             *
 * INPUT:   section  -- The section to store the entry under.                                  *
 *                                                                                             *
 *          entry    -- Identifier of the entry to store the source identifier to.             *
 *                                                                                             *
 *          value    -- The source (edge) value to store.                                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the source identifier stored?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_SourceType(char const * section, char const * entry, SourceType value)
{
	return(Put_String(section, entry, SourceName[value]));
}


/***********************************************************************************************
 * CCINIClass::Get_CrateType -- Fetches a crate type value from the INI database.              *
 *                                                                                             *
 *    This will return with the crate type specified in the INI database.                      *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search under.                            *
 *                                                                                             *
 *          entry    -- The entry to find the matching crate value for.                        *
 *                                                                                             *
 *          defvalue -- The default crate value to return if the entry could not be found.     *
 *                                                                                             *
 * OUTPUT:  Returns with the crate type identified with the specified entry. If the entry      *
 *          could not be located, then the default value is returned.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CrateType CCINIClass::Get_CrateType(char const * section, char const * entry, CrateType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		return(Crate_From_Name(buffer));
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_CrateType -- Stores the crate value in the section and entry specified.     *
 *                                                                                             *
 *    This will store the specified crate value to the section and entry specified.            *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to store the entry under.                       *
 *                                                                                             *
 *          entry    -- The entry identifier to store the crate value with.                    *
 *                                                                                             *
 *          value    -- The crate value to store.                                              *
 *                                                                                             *
 * OUTPUT:  bool; Was the crate value stored to the INI database?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_CrateType(char const * section, char const * entry, CrateType value)
{
	return(Put_String(section, entry, CrateNames[value]));
}


/***********************************************************************************************
 * CCINIClass::Get_TerrainType -- Fetch the terrain type identifier from the INI database.     *
 *                                                                                             *
 *    Fetches the terrain type number from the INI database.                                   *
 *                                                                                             *
 * INPUT:   section  -- The section to search for the entry under.                             *
 *                                                                                             *
 *          entry    -- Identifier for the entry to search for.                                *
 *                                                                                             *
 *          defvalue -- The default value to use if the entry could not be located.            *
 *                                                                                             *
 * OUTPUT:  Returns with the terrain type if found. If the entry wasn't found, then the        *
 *          default value will be returned.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TerrainType CCINIClass::Get_TerrainType(char const * section, char const * entry, TerrainType defvalue) const
{
	char buffer[128];

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
		return(TerrainTypeClass::From_Name(strtok(buffer, ",")));
	}
	return(defvalue);
}


/***********************************************************************************************
 * CCINIClass::Put_TerrainType -- Store the terrain type number to the INI database.           *
 *                                                                                             *
 *    This will store the terrain type identifier to the INI database.                         *
 *                                                                                             *
 * INPUT:   section  -- The section to store the entry under.                                  *
 *                                                                                             *
 *          entry    -- Identifier that the terrain number will be stored to.                  *
 *                                                                                             *
 *          value    -- The terrain type identifier to store.                                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the terrain identifier stored?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_TerrainType(char const * section, char const * entry, TerrainType value)
{
	return(Put_String(section, entry, TerrainTypeClass::As_Reference(value).Name()));
}


/***********************************************************************************************
 * CCINIClass::Get_Buildings -- Fetch a building bitfield from the INI database.               *
 *                                                                                             *
 *    This routing will fetch the a list of buildings from the INI database. The buildings     *
 *    are expressed as a comma separated list of building identifiers. The return value is     *
 *    a composite of bits that represent these buildings -- one bit per building type.         *
 *                                                                                             *
 * INPUT:   section  -- The section to search for the entry under.                             *
 *                                                                                             *
 *          entry    -- The entry to fetch the building list from.                             *
 *                                                                                             *
 *          defvalue -- The default value to return if the section and entry could not be      *
 *                      located.                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the building list (as a bitfield). If the entry could not be          *
 *          found, the the default value is returned instead.                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
long CCINIClass::Get_Buildings(char const * section, char const * entry, long defvalue) const
{
	char buffer[128];
	long pre;

	if (Get_String(section, entry, "", buffer, sizeof(buffer))) {

		pre = 0;
		char * token = strtok(buffer, ",");
		while (token != NULL && *token != '\0') {
			StructType building = BuildingTypeClass::From_Name(token);
			if (building != STRUCT_NONE) {
				pre |= (1L << building);
			}
			token = strtok(NULL, ",");
		}
	} else {
		pre = defvalue;
	}

	return(pre);
}


/***********************************************************************************************
 * CCINIClass::Put_Buildings -- Store a building list to the INI database.                     *
 *                                                                                             *
 *    This will store a list of buildings to the INI database. The buildings are listed by     *
 *    their identifier names separated by commas.                                              *
 *                                                                                             *
 * INPUT:   section  -- The identifier for the section to store the entry under.               *
 *                                                                                             *
 *          entry    -- The entry to store the building list to.                               *
 *                                                                                             *
 *          value    -- A list of buildings (in the form of a bit field -- one bit per         *
 *                      building type).                                                        *
 *                                                                                             *
 * OUTPUT:  Was the building list stored to the INI file?                                      *
 *                                                                                             *
 * WARNINGS:   This is limited to the buildings that can be expressed in a bitfield long.      *
 *             Which means, there can be only a maximum of 32 building types listed and        *
 *             even then, the total line length generated must not exceed 128 bytes.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CCINIClass::Put_Buildings(char const * section, char const * entry, long value)
{
	char buffer[128] = "";
	int maxi = (32 < STRUCT_COUNT) ? 32 : STRUCT_COUNT;

	for (StructType index = STRUCT_FIRST; index < maxi; index++) {
		if ((value & (1L << index)) != 0) {

			if (buffer[0] != '\0') {
				strcat(buffer, ",");
			}
			strcat(buffer, BuildingTypeClass::As_Reference(index).IniName);
		}
	}

	return(Put_String(section, entry, buffer));
}


/***********************************************************************************************
 * CCINIClass::Get_Unique_ID -- Fetch a unique identifier number for the INI file.             *
 *                                                                                             *
 *    This is a shorthand version of the message digest. It calculates the ID number from the  *
 *    message digest itself.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a 32 bit unique identifier number for the INI database.               *
 *                                                                                             *
 * WARNINGS:   Since the return value is only 32 bits, it is much less secure than the         *
 *             complete message digest.                                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int CCINIClass::Get_Unique_ID(void) const
{
	if (!IsDigestPresent) {
		((CCINIClass *)this)->Calculate_Message_Digest();
	}

	return(CRCEngine()(&Digest[0], sizeof(Digest)));
}


/***********************************************************************************************
 * CCINIClass::Calculate_Message_Digest -- Calculate a message digest for the current database *
 *                                                                                             *
 *    This will calculate a new message digest according to the current state of the INI       *
 *    database.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   If the database is changed in any fashion, this message digest will be rendered *
 *             obsolete.                                                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void CCINIClass::Calculate_Message_Digest(void)
{
	/*
	**	Calculate the message digest for the INI data that was read.
	*/
	SHAPipe sha;
	INIClass::Save(sha);
	sha.Result(Digest);
	IsDigestPresent = true;
}


/***********************************************************************************************
 * CCINIClass::Invalidate_Message_Digest -- Flag message digest as being invalid.              *
 *                                                                                             *
 *    This flags the message digest as being invalid so that it will be recalculated when      *
 *    needed.                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void CCINIClass::Invalidate_Message_Digest(void)
{
	IsDigestPresent = false;
}
