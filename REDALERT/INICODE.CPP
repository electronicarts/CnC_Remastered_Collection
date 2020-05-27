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

/* $Header:   F:\projects\c&c0\vcs\code\inicode.cpv   4.38   03 Jul 1996 05:14:04   JOE_BOSTIC  $ */
/***************************************************************************
 **     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : INICODE.CPP                              *
 *                                                                         *
 *                   Programmer : David R Dettmer                          *
 *                                                                         *
 *                   Start Date : November 7, 1995                         *
 *                                                                         *
 *                  Last Update : February 20, 1996 [JLB]                  *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


#ifdef TOFIX

void Get_Scenario_Digest(char * digest, char * buffer)
{
	char buf[128];	// Working string staging buffer.
	char stage[sizeof(BigInt)*2];

	char * stage_ptr = &stage[0];
	int len = strlen(buffer) + 2;
	char * tbuffer = buffer + len;

	WWGetPrivateProfileString("DIGEST", NULL, NULL, tbuffer, sizeof(_staging_buffer)-len, buffer);
	stage[0] = '\0';
	while (*tbuffer != '\0') {

		WWGetPrivateProfileString("DIGEST", tbuffer, NULL, buf, sizeof(buf)-1, buffer);
		strcat(stage, buf);
		tbuffer += strlen(tbuffer)+1;
	}

	len = strlen(stage);
	char * dbuffer = &stage[0];
	tbuffer = &stage[0];
	for (int index = 0; index < len/2; index++) {
		int c;
		if (isdigit(*tbuffer)) {
			c = (*tbuffer) - '0';
		} else {
			c = 10 + (toupper(*tbuffer) - 'A');
		}
		tbuffer++;
		c <<= 4;
		if (isdigit(*tbuffer)) {
			c |= (*tbuffer) - '0';
		} else {
			c |= 10 + (toupper(*tbuffer) - 'A');
		}
		tbuffer++;
		*dbuffer++ = c;
	}

	/*
	**	Decode and decrypt the number.
	*/
	BigInt hash = 0;
	hash.DERDecode((unsigned char*)stage);

	BigInt d;
	d = d.Decode_ASCII(KEY_D);
	BigInt n;
	n = n.Decode_ASCII(KEY_N);

	hash = hash.exp_b_mod_c(d, n);

	memcpy(digest, &hash, 20);

	buffer = strstr(buffer, "[DIGEST]");
	if (buffer) {
		*buffer = '\0';
	}
}


bool Read_Scenario_INI_Write_INB( char *root, bool fresh)
{
	char *buffer;				// Scenario.ini staging buffer pointer.
	char *binbuf;				// Scenario.inb staging buffer pointer.
	char fname[_MAX_FNAME+_MAX_EXT];			// full INI filename
	char buf[256];				// Working string staging buffer.
	char scenarioname[40];
	int len;
	unsigned char val;

	/*
	**	Fetch working pointer to the INI staging buffer. Make sure that the buffer
	**	is cleared out before proceeding.  (Don't use the HidPage for this, since
	**	the HidPage may be needed for various uncompressions during the INI
	**	parsing.)
	*/
	buffer = (char *)_staging_buffer;
	memset(buffer, '\0', sizeof(_staging_buffer));

	/*
	**	Create scenario filename and read the file.
	** The previous routine verifies that the file is available.
	*/

	sprintf(fname,"%s.INI",root);
	CCFileClass file(fname);

	file.Read(buffer, sizeof(_staging_buffer)-1);

	/*
	**	Fetch and slice off any message digest attached.
	*/
	char digest[20];
	Get_Scenario_Digest(digest, buffer);

	char real_digest[20];
	SHAEngine digest_engine;
	digest_engine.Hash(buffer, strlen(buffer));
	digest_engine.Result(real_digest);

	if (memcmp(digest, real_digest, sizeof(real_digest)) != 0) {
		WWMessageBox().Process(TXT_SCENARIO_ERROR, TXT_OK);
	}

	/*
	** Init the Scenario CRC value
	*/
	ScenarioCRC = 0;
	len = strlen(buffer);
	for (int i = 0; i < len; i++) {
		val = (unsigned char)buffer[i];
		Add_CRC(&ScenarioCRC, (unsigned long)val);
	}

	sprintf(fname,"%s.INB",root);
	file.Set_Name(fname);
	file.Cache(16384);
	file.Open(WRITE);

	unsigned long crc = Ini_Binary_Version();

	file.Write( (char *)&crc, sizeof(crc) );

	binbuf = (char *)Alloc( sizeof(_staging_buffer), MEM_NORMAL );

	if (binbuf) {
		Write_Bin_Init( binbuf, sizeof(_staging_buffer) );
	} else {
		Print_Error_End_Exit( "Unable to alloc space for writing INB" );
	}

	/*
	**	Fetch the appropriate movie names from the INI file.
	*/
	WWGetPrivateProfileString("Basic", "Name", "", scenarioname, sizeof(scenarioname), buffer);
	WWGetPrivateProfileString("Basic", "Intro", "x", Scen.IntroMovie, sizeof(Scen.IntroMovie), buffer);
	WWGetPrivateProfileString("Basic", "Brief", "x", Scen.BriefMovie, sizeof(Scen.BriefMovie), buffer);
	WWGetPrivateProfileString("Basic", "Win", "x", Scen.WinMovie, sizeof(Scen.WinMovie), buffer);
	WWGetPrivateProfileString("Basic", "Lose", "x", Scen.LoseMovie, sizeof(Scen.LoseMovie), buffer);
	WWGetPrivateProfileString("Basic", "Action", "x", Scen.ActionMovie, sizeof(Scen.ActionMovie), buffer);
	Scen.IsToCarryOver = WWGetPrivateProfileInt("Basic", "ToCarryOver", 0, buffer);
	Scen.IsToInherit = WWGetPrivateProfileInt("Basic", "ToInherit", 0, buffer);

	Write_Bin_String( scenarioname, strlen(scenarioname), binbuf );
	Write_Bin_String( Scen.IntroMovie, strlen(Scen.IntroMovie), binbuf );
	Write_Bin_String( Scen.BriefMovie, strlen(Scen.BriefMovie), binbuf );
	Write_Bin_String( Scen.WinMovie, strlen(Scen.WinMovie), binbuf );
	Write_Bin_String( Scen.LoseMovie, strlen(Scen.LoseMovie), binbuf );
	Write_Bin_String( Scen.ActionMovie, strlen(Scen.ActionMovie), binbuf );

	/*
	**	Fetch the transition theme for this scenario.
	*/
	Scen.TransitTheme = THEME_NONE;
	WWGetPrivateProfileString("Basic", "Theme", "No Theme", buf, sizeof(buf), buffer);
	Scen.TransitTheme = Theme.From_Name(buf);

	WWGetPrivateProfileString( "Basic", "Player", "Greece", buf, 127, buffer);
	Scen.PlayerHouse = HouseTypeClass::From_Name(buf);
	if (Scen.PlayerHouse >= HOUSE_MULTI1) {
		Scen.PlayerHouse = HOUSE_GREECE;
	}
// TCTC To Fix?
//	Scen.CarryOverPercent = WWGetPrivateProfileInt( "Basic", "CarryOverMoney", 100, buffer);
	Scen.CarryOverCap = WWGetPrivateProfileInt( "Basic", "CarryOverCap", -1, buffer);
	Scen.Percent = WWGetPrivateProfileInt( "Basic", "Percent", 0, buffer);

	Write_Bin_Num( &Scen.TransitTheme, sizeof(Scen.TransitTheme), binbuf );
	Write_Bin_Num( &Scen.PlayerHouse, sizeof(Scen.PlayerHouse), binbuf );
	Write_Bin_Num( &Scen.CarryOverPercent, 1, binbuf );
	Write_Bin_Num( &Scen.CarryOverCap, 2, binbuf );
	Write_Bin_Num( &Scen.Percent, 1, binbuf );

	/*
	**	Read in the specific information for each of the house types.  This creates
	**	the houses of different types.
	*/
	HouseClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the team-type data. The team types must be created before any
	**	triggers can be created.
	*/
	TeamTypeClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Assign PlayerPtr by reading the player's house from the INI;
	**	Must be done before any TechnoClass objects are created.
	*/
	if (Session.Type == GAME_NORMAL) {
		Scen.CarryOverPercent = Cardinal_To_Fixed(100, Scen.CarryOverPercent);

		PlayerPtr = HouseClass::As_Pointer( Scen.PlayerHouse );
		assert(PlayerPtr != NULL);
		PlayerPtr->IsHuman = true;

		int carryover;
		if (Scen.CarryOverCap != -1) {
			carryover = MIN(Fixed_To_Cardinal(Scen.CarryOverMoney, Scen.CarryOverPercent), (Scen.CarryOverCap * 100) );
		} else {
			carryover = Fixed_To_Cardinal(Scen.CarryOverMoney, Scen.CarryOverPercent);
		}
		PlayerPtr->Credits += carryover;
		PlayerPtr->Control.InitialCredits += carryover;
	} else {

		Assign_Houses();
	}

	/*
	**	Read in the trigger data. The triggers must be created before any other
	**	objects can be initialized.
	*/
	TriggerClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the map control values. This includes dimensions
	**	as well as theater information.
	*/
	Map.Read_INI(buffer);
	Call_Back();

	/*
	**	Attempt to read the map's binary image file; if fails, read the
	**	template data from the INI, for backward compatibility
	*/
	if (fresh) {
		if (!Map.Read_Binary(root, &ScenarioCRC)) {
			return( false );
		}
	}
	Call_Back();

	/*
	**	Read in and place the 3D terrain objects.
	*/
	TerrainClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in and place the units (all sides).
	*/
	UnitClass::Read_INI(buffer);
	Call_Back();

	VesselClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in and place the infantry units (all sides).
	*/
	InfantryClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in and place all the buildings on the map.
	*/
	BuildingClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the AI's base information.
	*/
	Base.Read_INI(buffer);
	Call_Back();

	/*
	**	Read in any normal overlay objects.
	*/
	OverlayClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in any smudge overlays.
	*/
	SmudgeClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in any briefing text.
	*/
	char * stage = &Scen.BriefingText[0];
	*stage = '\0';
	int index = 1;

	/*
	**	Build the full text of the mission objective.
	*/
	for (;;) {
		char buff[16];

		sprintf(buff, "%d", index++);
		*stage = '\0';
		WWGetPrivateProfileString("Briefing", buff, "", stage, 255, buffer);
		if (strlen(stage) == 0) break;
		strcat(stage, " ");
		stage += strlen(stage);
	}

	len = Write_Bin_Length( binbuf );
	if (len != -1) {
		file.Write( binbuf, len );
	}

	Free( binbuf );
	file.Close();

	return(true);
}

#endif









