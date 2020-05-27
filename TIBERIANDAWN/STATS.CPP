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


/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : WINSTUB.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 05/29/1996                                                   *
 *                                                                                             *
 *                  Last Update : May 29th 1996 [ST]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *  Internet game statistics to collect and upload to the server                               *
 *                                                                                             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include "tcpip.h"
#include "packet.h"
#include "ccdde.h"

#define FIELD_PACKET_TYPE					"TYPE"
#define FIELD_GAME_ID							"IDNO"
#define FIELD_START_CREDITS					"CRED"
#define FIELD_BASES							"BASE"
#define FIELD_TIBERIUM						"TIBR"
#define FIELD_CRATES							"CRAT"
#define FIELD_AI_PLAYERS						"AIPL"
#define FIELD_CAPTURE_THE_FLAG				"FLAG"
#define FIELD_START_UNIT_COUNT				"UNIT"
#define FIELD_TECH_LEVEL						"TECH"
#define FIELD_SCENARIO						"SCEN"
#define FIELD_COMPLETION						"CMPL"
#define FIELD_START_TIME						"TIME"
#define FIELD_GAME_DURATION					"DURA"
#define FIELD_FRAME_RATE						"AFPS"
#define FIELD_SPEED_SETTING					"SPED"
#define FIELD_GAME_VERSION					"VERS"
#define FIELD_GAME_BUILD_DATE				"DATE"
#define FIELD_COVERT_PRESENT				"COVT"
#define FIELD_CPU_TYPE						"PROC"
#define FIELD_MEMORY							"MEMO"
#define FIELD_VIDEO_MEMORY					"VIDM"
#define FIELD_PLAYER1_HANDLE				"NAM1"
#define FIELD_PLAYER2_HANDLE				"NAM2"
#define FIELD_PLAYER1_TEAM					"SID1"
#define FIELD_PLAYER2_TEAM					"SID2"
#define FIELD_PLAYER1_COLOR					"COL1"
#define FIELD_PLAYER2_COLOR					"COL2"
#define FIELD_PLAYER1_CREDITS				"CRD1"
#define FIELD_PLAYER2_CREDITS				"CRD2"

#define FIELD_PLAYER1_UNITS_LEFT			"UNL1"
#define FIELD_PLAYER2_UNITS_LEFT			"UNL2"
#define FIELD_PLAYER1_INFANTRY_LEFT		"INL1"
#define FIELD_PLAYER2_INFANTRY_LEFT		"INL2"
#define FIELD_PLAYER1_PLANES_LEFT			"PLL1"
#define FIELD_PLAYER2_PLANES_LEFT			"PLL2"
#define FIELD_PLAYER1_BUILDINGS_LEFT		"BLL1"
#define FIELD_PLAYER2_BUILDINGS_LEFT		"BLL2"

#define FIELD_PLAYER1_UNITS_BOUGHT		"UNB1"
#define FIELD_PLAYER2_UNITS_BOUGHT		"UNB2"
#define FIELD_PLAYER1_INFANTRY_BOUGHT	"INB1"
#define FIELD_PLAYER2_INFANTRY_BOUGHT	"INB2"
#define FIELD_PLAYER1_PLANES_BOUGHT		"PLB1"
#define FIELD_PLAYER2_PLANES_BOUGHT		"PLB2"
#define FIELD_PLAYER1_BUILDINGS_BOUGHT	"BLB1"
#define FIELD_PLAYER2_BUILDINGS_BOUGHT	"BLB2"

#define FIELD_PLAYER1_UNITS_KILLED		"UNK1"
#define FIELD_PLAYER2_UNITS_KILLED		"UNK2"
#define FIELD_PLAYER1_INFANTRY_KILLED	"INK1"
#define FIELD_PLAYER2_INFANTRY_KILLED	"INK2"
#define FIELD_PLAYER1_PLANES_KILLED		"PLK1"
#define FIELD_PLAYER2_PLANES_KILLED		"PLK2"
#define FIELD_PLAYER1_BUILDINGS_KILLED	"BLK1"
#define FIELD_PLAYER2_BUILDINGS_KILLED	"BLK2"

#define FIELD_PLAYER1_BUILDINGS_CAPTURED "BLC1"
#define FIELD_PLAYER2_BUILDINGS_CAPTURED "BLC2"

#define FIELD_PLAYER1_CRATES_FOUND		"CRA1"
#define FIELD_PLAYER2_CRATES_FOUND		"CRA2"
#define FIELD_PLAYER1_HARVESTED			"HRV1"
#define FIELD_PLAYER2_HARVESTED			"HRV2"


#define	PACKET_TYPE_HOST_GAME_INFO		(unsigned char) 50
#define	PACKET_TYPE_GUEST_GAME_INFO	(unsigned char) 51

enum {
	COMPLETION_CONNECTION_LOST,
	COMPLETION_PLAYER_1_WON,
	COMPLETION_PLAYER_1_WON_BY_RESIGNATION,
	COMPLETION_PLAYER_1_WON_BY_DISCONNECTION,
	COMPLETION_PLAYER_2_WON,
	COMPLETION_PLAYER_2_WON_BY_RESIGNATION,
	COMPLETION_PLAYER_2_WON_BY_DISCONNECTION
};


extern unsigned long 	PlanetWestwoodGameID;
extern HINSTANCE		ProgramInstance;
extern unsigned long 	PlanetWestwoodStartTime;

extern "C" char	CPUType;


bool			GameTimerInUse = false;
TimerClass	GameTimer;
long			GameEndTime;
void			*PacketLater = NULL;

/***********************************************************************************************
 * Send_Statistics_To_Server -- sends internet game statistics to the Westeood server          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/29/96 12:38PM ST : Created                                                             *
 *=============================================================================================*/

void Send_Statistics_Packet(void)
{
//PG_TO_FIX
#if (0)
#ifndef DEMO

	PacketClass	stats;
	HouseClass	*player;
	static int	packet_size;
	int			index;
	bool			packet_later = false;	// Should the packet be sent later
	void			*packet;

	static char	field_player_handle[5] 				= { "NAM?" };
	static char	field_player_team[5] 				= { "SID?" };
	static char	field_player_color[5]				= { "COL?" };
	static char field_player_credits[5]				= { "CRD?" };
	static char field_player_units_left[5]			= { "UNL?" };
	static char field_player_infantry_left[5]		= { "INL?" };
	static char field_player_planes_left[5]		= { "PLL?" };
	static char field_player_buildings_left[5]	= { "BLL?" };
	static char field_player_units_bought[5] 		= { "UNB?" };
	static char field_player_infantry_bought[5] 	= { "INB?" };
	static char field_player_planes_bought[5]		= { "PLB?" };
	static char field_player_buildings_bought[5]	= { "BLB?" };
	static char field_player_units_killed[5] 		= { "UNK?" };
	static char field_player_infantry_killed[5] 	= { "INK?" };
	static char field_player_planes_killed[5]		= { "PLK?" };
	static char field_player_buildings_killed[5]	= { "BLK?" };
	static char field_player_buildings_captured[5] = { "BLC?" };
	static char field_player_crates_found[5]		= { "CRA?" };
	static char field_player_harvested[5]			= { "HRV?" };

	static char *houses[] = {
		"GDI",
		"NOD",
		"NUT",
		"JUR",
		"M01",
		"M02",
		"M03",
		"M04",
		"M05",
		"M06"
	};

	CCDebugString ("C&C95 - In Send_Statistics_Packet.\n");


	if (!PacketLater){

		CCDebugString ("C&C95 - PacketLater is false.\n");


		/*
		** Field to identify this as C&C 95 internet game statistics packet
		*/
		if (Server){
			stats.Add_Field(FIELD_PACKET_TYPE, PACKET_TYPE_HOST_GAME_INFO);
		}else{
			stats.Add_Field(FIELD_PACKET_TYPE, PACKET_TYPE_GUEST_GAME_INFO);
		}

		/*
		** Game ID. A unique game identifier assigned by WChat.
		*/
		stats.Add_Field(FIELD_GAME_ID, PlanetWestwoodGameID);

		/*
		** Start credits.
		*/
		stats.Add_Field(FIELD_START_CREDITS, (unsigned long)MPlayerCredits);

		/*
		** Bases (On/Off)
		*/
		stats.Add_Field(FIELD_BASES, MPlayerBases ? "ON" : "OFF");

		/*
		** Tiberium (On/Off)
		*/
		stats.Add_Field(FIELD_TIBERIUM, MPlayerTiberium ? "ON" : "OFF");

		/*
		** Crates (On/Off)
		*/
		stats.Add_Field(FIELD_CRATES, MPlayerGoodies ? "ON" : "OFF");

		/*
		** AI Players (On/Off/Capture the flag)
		*/
		stats.Add_Field(FIELD_AI_PLAYERS, MPlayerGhosts ? "ON" : "OFF");
		stats.Add_Field(FIELD_CAPTURE_THE_FLAG, Special.IsCaptureTheFlag ? "ON" : "OFF");

		/*
		** Start unit count
		*/
		stats.Add_Field(FIELD_START_UNIT_COUNT, (unsigned long)MPlayerUnitCount);

		/*
		** Tech level.
		*/
		stats.Add_Field(FIELD_TECH_LEVEL, (unsigned long)BuildLevel);

		CCDebugString ("C&C95 - Adding stats field for scenario.\n");
		/*
		** Scenario
		*/
		char fname[128];
		char namebuffer[40];
		char *abuffer = (char *)_ShapeBuffer;
		memset(abuffer, '\0', _ShapeBufferSize);
		sprintf(fname,"%s.INI",ScenarioName);
		CCFileClass fileo;
		fileo.Set_Name (fname);
		fileo.Read(abuffer, _ShapeBufferSize-1);
		fileo.Close();
		WWGetPrivateProfileString("Basic", "Name", "Nulls-Ville", namebuffer, 40, abuffer);
		stats.Add_Field(FIELD_SCENARIO, namebuffer);
		//stats.Add_Field(FIELD_SCENARIO, MPlayerScenarios[ScenarioIdx]);


		/*
		** Game completion status.
		**
		**  Connection lost.
		**  Player 1 won
		**  Player 2 won
		**  Player 1 won by resignation
		**  Player 2 won by resignation
		**  Player 1 aborted
		**  Player 2 aborted
		**
		*/
		CCDebugString ("C&C95 - Adding stats field for completion status.\n");
		HouseClass *player1 = HouseClass::As_Pointer(MPlayerHouses[0]);
		HouseClass *player2 = HouseClass::As_Pointer(MPlayerHouses[1]);

		int completion = -1;

		if (ConnectionLost){
			completion = COMPLETION_CONNECTION_LOST;
			CCDebugString ("C&C95 - Completion status is connection lost.\n");
		}else{

			if (player1 && player2){
				if (player1->IGaveUp){
					completion = COMPLETION_PLAYER_2_WON_BY_DISCONNECTION;
					CCDebugString ("C&C95 - Completion status is player 1 disconnected.\n");
				}

				if (player2->IGaveUp){
					completion = COMPLETION_PLAYER_1_WON_BY_DISCONNECTION;
					CCDebugString ("C&C95 - Completion status is player 2 disconnected.\n");
				}


				if (player2->IsDefeated){
					/*
					** Player 1 won. Find out how.
					*/
					completion = COMPLETION_PLAYER_1_WON;
					if (player2->Resigned){
						completion = COMPLETION_PLAYER_1_WON_BY_RESIGNATION;
						CCDebugString ("C&C95 - Completion status is player 2 resigned.\n");
					}else{
						if (player2->IGaveUp){
							completion = COMPLETION_PLAYER_1_WON_BY_DISCONNECTION;
							CCDebugString ("C&C95 - Completion status is player 2 disconnected.\n");
						}
					}


				}else{

					if (player1->IsDefeated){
						/*
						** Player 2 won. Find out how.
						*/
						completion = COMPLETION_PLAYER_2_WON;
						if (player1->Resigned){
							completion = COMPLETION_PLAYER_2_WON_BY_RESIGNATION;
							CCDebugString ("C&C95 - Completion status is player 1 resigned.\n");
						}else{
							if (player1->IGaveUp){
								completion = COMPLETION_PLAYER_2_WON_BY_DISCONNECTION;
								CCDebugString ("C&C95 - Completion status is player 1 disconnected.\n");
							}
						}
					}
				}
			}
		}

		stats.Add_Field (FIELD_COMPLETION, (char) completion);






		/*
		** Game start time (GMT or Pacific?)
		**
		** Passed from WChat
		*/
		stats.Add_Field (FIELD_START_TIME, (long) PlanetWestwoodStartTime);

		/*
		** Game duration (seconds).
		*/
		stats.Add_Field (FIELD_GAME_DURATION, (long) GameEndTime/60);

		/*
		** Avg. frame rate.
		*/
		if (GameEndTime/60 == 0){
			stats.Add_Field (FIELD_FRAME_RATE, 0L );
		}else{
			stats.Add_Field (FIELD_FRAME_RATE, (long) Frame / (GameEndTime/60) );
		}


		CCDebugString ("C&C95 - Adding hardware info stats.\n");
		/*
		** CPU type
		*/
		stats.Add_Field (FIELD_CPU_TYPE, (char)CPUType);

		/*
		** Memory
		*/
		MEMORYSTATUS	mem_info;
		mem_info.dwLength=sizeof(mem_info);
		GlobalMemoryStatus(&mem_info);
		stats.Add_Field (FIELD_MEMORY, (long)mem_info.dwTotalPhys);

		/*
		** Video memory
		*/
		DDCAPS	video_capabilities;
		long		video_memory;

		if (DirectDrawObject){
			video_capabilities.dwSize = sizeof (video_capabilities);
			if (DD_OK == DirectDrawObject->GetCaps (&video_capabilities , NULL)){
				video_memory = video_capabilities.dwVidMemTotal;
				video_memory += 1024*1024 -1;
				video_memory &= 0xfff00000;
				stats.Add_Field (FIELD_VIDEO_MEMORY, (long) video_memory);
			}
		}

		CCDebugString ("C&C95 - Adding game info stats.\n");
		/*
		** Game speed setting.
		*/
		stats.Add_Field (FIELD_SPEED_SETTING, (char)Options.GameSpeed);

		/*
		** C&C 95 version/build date
		*/
		char	version[128];
		sprintf (version, "%d%s", Version_Number(), VersionText);
		stats.Add_Field (FIELD_GAME_VERSION, version);

		char path_to_exe[280];
		FILETIME write_time;		//File time is 64 bits

		GetModuleFileName (ProgramInstance, path_to_exe, 280);
		RawFileClass file;
		file.Set_Name(path_to_exe);
		file.Open();
		HANDLE handle = file.Get_File_Handle();

		if (handle != INVALID_HANDLE_VALUE){
			if (GetFileTime (handle, NULL, NULL, &write_time)){
				write_time.dwLowDateTime = htonl (write_time.dwLowDateTime);
				write_time.dwHighDateTime = htonl (write_time.dwHighDateTime);
				stats.Add_Field (FIELD_GAME_BUILD_DATE, (void*)&write_time, sizeof (write_time));
			}
		}

		/*
		** Covert installed? (Yes/No)
		*/
		stats.Add_Field(FIELD_COVERT_PRESENT, (char) Expansion_Present());

		CCDebugString ("C&C95 - Adding house specific stats.\n");
		/*
		** Build the player specific statistics
		**
		*/
		for (int house = 0 ; house < 2 ; house++){

			player = HouseClass::As_Pointer(MPlayerHouses[house]);

			if (player){
				/*
				** Player handle.
				*/
				field_player_handle[3] = '1' + (char)house;
				stats.Add_Field (field_player_handle, (char*) MPlayerNames[house]);

				/*
				** Player team. (NOD or GDI)
				*/
				field_player_team[3] = '1' + (char)house;
				stats.Add_Field (field_player_team, houses[player->ActLike]);

				/*
				** Player color
				*/
				field_player_color[3] = '1' + (char)house;
				stats.Add_Field (field_player_color, (unsigned char) (player->Class->House - HOUSE_MULTI1));

				/*
				** Player end credits.
				*/
				field_player_credits[3] = '1' + (char)house;
				stats.Add_Field (field_player_credits, player->Credits + player->Tiberium);

				/*
				** Number of each unit/building type built
				*/
				field_player_infantry_bought[3] = '1' + (char)house;
				field_player_units_bought[3] = '1' + (char)house;
				field_player_planes_bought[3] = '1' + (char)house;
				field_player_buildings_bought[3] = '1' + (char)house;

				player->InfantryTotals->To_Network_Format();
				player->UnitTotals->To_Network_Format();
				player->AircraftTotals->To_Network_Format();
				player->BuildingTotals->To_Network_Format();

				stats.Add_Field (field_player_infantry_bought, (void*) player->InfantryTotals->Get_All_Totals(), player->InfantryTotals->Get_Unit_Count()*4);
				stats.Add_Field (field_player_units_bought, (void*) player->UnitTotals->Get_All_Totals(), player->UnitTotals->Get_Unit_Count()*4);
				stats.Add_Field (field_player_planes_bought, (void*) player->AircraftTotals->Get_All_Totals(), player->AircraftTotals->Get_Unit_Count()*4);
				stats.Add_Field (field_player_buildings_bought, (void*) player->BuildingTotals->Get_All_Totals(), player->BuildingTotals->Get_Unit_Count()*4);

				player->InfantryTotals->To_PC_Format();
				player->UnitTotals->To_PC_Format();
				player->AircraftTotals->To_PC_Format();
				player->BuildingTotals->To_PC_Format();

				/*
				** Clear out the counts and use the space to count up the current number of units/buildings
				*/
				player->InfantryTotals->Clear_Unit_Total();
				player->AircraftTotals->Clear_Unit_Total();
				player->UnitTotals->Clear_Unit_Total();
				player->BuildingTotals->Clear_Unit_Total();

				/*
				** Number of units remaining to player
				*/
				for (index = 0; index < Units.Count(); index++) {
					UnitClass const * unit = Units.Ptr(index);
					if (unit->House == player){
						player->UnitTotals->Increment_Unit_Total (unit->Class->Type);
					}
				}

				for (index = 0; index < Infantry.Count(); index++) {
				InfantryClass const * infantry = Infantry.Ptr(index);
					if (infantry->House == player && !infantry->Class->IsCivilian){
						player->InfantryTotals->Increment_Unit_Total (infantry->Class->Type);
					}
				}

				for (index = 0; index < Aircraft.Count(); index++) {
				AircraftClass const * aircraft = Aircraft.Ptr(index);
					if (aircraft->House == player &&	aircraft->Class->Type != AIRCRAFT_CARGO){
						player->AircraftTotals->Increment_Unit_Total (aircraft->Class->Type);
					}
				}

				for (index = 0; index < Buildings.Count(); index++) {
				BuildingClass const * building = Buildings.Ptr(index);
					if (building->House == player){
						player->BuildingTotals->Increment_Unit_Total (building->Class->Type);
					}
				}

				player->InfantryTotals->To_Network_Format();
				player->UnitTotals->To_Network_Format();
				player->AircraftTotals->To_Network_Format();
				player->BuildingTotals->To_Network_Format();

				field_player_infantry_left[3] = '1' + (char)house;
				field_player_units_left[3] = '1' + (char)house;
				field_player_planes_left[3] = '1' + (char)house;
				field_player_buildings_left[3] = '1' + (char)house;
				stats.Add_Field (field_player_infantry_left, (void*) player->InfantryTotals->Get_All_Totals(), player->InfantryTotals->Get_Unit_Count()*4);
				stats.Add_Field (field_player_units_left, (void*) player->UnitTotals->Get_All_Totals(), player->UnitTotals->Get_Unit_Count()*4);
				stats.Add_Field (field_player_planes_left, (void*) player->AircraftTotals->Get_All_Totals(), player->AircraftTotals->Get_Unit_Count()*4);
				stats.Add_Field (field_player_buildings_left, (void*) player->BuildingTotals->Get_All_Totals(), player->BuildingTotals->Get_Unit_Count()*4);


				/*
				** Number of enemy units/buildings of each type destroyed.
				*/

				player->DestroyedInfantry->To_Network_Format();
				player->DestroyedUnits->To_Network_Format();
				player->DestroyedAircraft->To_Network_Format();
				player->DestroyedBuildings->To_Network_Format();

				field_player_infantry_killed[3] = '1' + (char)house;
				field_player_units_killed[3] = '1' + (char)house;
				field_player_planes_killed[3] = '1' + (char)house;
				field_player_buildings_killed[3] = '1' + (char)house;
				stats.Add_Field (field_player_infantry_killed, (void*) player->DestroyedInfantry->Get_All_Totals(), player->DestroyedInfantry->Get_Unit_Count()*4);
				stats.Add_Field (field_player_units_killed, (void*) player->DestroyedUnits->Get_All_Totals(), player->DestroyedUnits->Get_Unit_Count()*4);
				stats.Add_Field (field_player_planes_killed, (void*) player->DestroyedAircraft->Get_All_Totals(), player->DestroyedAircraft->Get_Unit_Count()*4);
				stats.Add_Field (field_player_buildings_killed, (void*) player->DestroyedBuildings->Get_All_Totals(), player->DestroyedBuildings->Get_Unit_Count()*4);


				/*
				** Number and type of enemy buildings captured
				*/
				field_player_buildings_captured[3] = '1' + (char)house;
				player->CapturedBuildings->To_Network_Format();
				stats.Add_Field (field_player_buildings_captured, (void*) player->CapturedBuildings->Get_All_Totals(), player->CapturedBuildings->Get_Unit_Count()*4);

				/*
				** Number of crates discovered and their contents
				*/
				field_player_crates_found[3] = '1' + (char)house;
				player->TotalCrates->To_Network_Format();
				stats.Add_Field (field_player_crates_found, (void*) player->TotalCrates->Get_All_Totals(), player->TotalCrates->Get_Unit_Count()*4);

				/*
				** Amount of tiberium turned into credits
				*/
				field_player_harvested[3] = '1' + (char)house;
				stats.Add_Field (field_player_harvested, (unsigned long) player->HarvestedCredits);
			}
		}

		CCDebugString ("C&C95 - Calling Create_Comms_Packet.\n");
		/*
		** Create the comms packet to be sent
		*/
		packet = stats.Create_Comms_Packet(packet_size);
		CCDebugString ("C&C95 - Returned from Create_Comms_Packet.\n");

		/*
		** If a player disconnected then dont send the packet at this time - save it for later
		*/
		if (completion == COMPLETION_PLAYER_1_WON_BY_DISCONNECTION
			|| completion == COMPLETION_PLAYER_2_WON_BY_DISCONNECTION){
			PacketLater = packet;
			CCDebugString ("C&C95 - Flagging to send the packet later.\n");
			return;
		}

	}else{		//else for if (!PacketLater)

		CCDebugString ("C&C95 - PacketLater is true.\n");

		/*
		** Send the packet we calculated earlier when the disconnect occurred
		*/
		packet = PacketLater;
		PacketLater = NULL;
	}

	/*
	** Send it.....
	*/
	int times = 100;		//100 times max
	CountDownTimerClass	send_timer;

	CCDebugString ("C&C95 - About to send stats packet to DDE server.\n");
	while ( ! Send_Data_To_DDE_Server ((char*)packet, packet_size, DDEServerClass::DDE_PACKET_GAME_RESULTS)){
		CCDebugString ("C&C95 - Stats packet send failed.\n");
		send_timer.Set (60, true);
		while (send_timer.Time()){};
	}


	/*
	** Save it to disk as well so I can see it
	*/
#if (0)
	RawFileClass anotherfile ("packet.net");
	anotherfile.Write(packet, packet_size);
#endif	//(0)
	/*
	** Tidy up
	*/
	CCDebugString ("C&C95 - About to delete packet memory.\n");
	delete [] packet;

	GameStatisticsPacketSent = true;
	CCDebugString ("C&C95 - Returning from Send_Statistics_Packet.\n");
#endif	//DEMO
#endif
}









void Register_Game_Start_Time(void)
{

	GameTimer.Set (0, true);
	GameTimerInUse = true;
}


extern void Register_Game_End_Time(void)
{
	GameEndTime = GameTimer.Time();
	GameTimerInUse = false;
}

