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

#ifdef WIN32

#include	"function.h"
#include "tcpip.h"
#include "packet.h"
#include "ccdde.h"

#define FIELD_PACKET_TYPE						"TYPE"
#define FIELD_GAME_ID							"IDNO"
#define FIELD_START_CREDITS						"CRED"
#define FIELD_BASES								"BASE"
#define FIELD_TIBERIUM							"TIBR"
#define FIELD_CRATES							"CRAT"
#define FIELD_AI_PLAYERS						"AIPL"
#define FIELD_CAPTURE_THE_FLAG					"FLAG"
#define FIELD_START_UNIT_COUNT					"UNIT"
#define FIELD_TECH_LEVEL						"TECH"
#define FIELD_SCENARIO							"SCEN"
#define FIELD_COMPLETION						"CMPL"
#define FIELD_START_TIME						"TIME"
#define FIELD_GAME_DURATION						"DURA"
#define FIELD_FRAME_RATE						"AFPS"
#define FIELD_SPEED_SETTING						"SPED"
#define FIELD_GAME_VERSION						"VERS"
#define FIELD_GAME_BUILD_DATE					"DATE"
#define FIELD_COVERT_PRESENT					"COVT"
#define FIELD_CPU_TYPE							"PROC"
#define FIELD_MEMORY							"MEMO"
#define FIELD_VIDEO_MEMORY						"VIDM"
#define FIELD_SHADOW_REGROWS					"SHAD"

#ifdef WOLAPI_INTEGRATION
#define FIELD_HOSTORNOT							"SDFX"
#define FIELD_TOURNAMENT						"TRNY"
#define FIELD_NUM_INITIAL_PLAYERS				"NUMP"
#define FIELD_NUM_REMAINING_PLAYERS				"REMN"
#define FIELD_DISCONNECT_PINGS					"PING"
#define FIELD_COMPUTERTOOKOVER					"QUIT"
//#define FIELD_HARDWARE_GUID					"GUID"
#define FIELD_PLAYER1_IP						"ADR1"
#define FIELD_PLAYER2_IP						"ADR2"
#endif

//	ajw The following were never used (thank god).
#define FIELD_PLAYER1_HANDLE					"NAM1"
#define FIELD_PLAYER2_HANDLE					"NAM2"
#define FIELD_PLAYER1_TEAM						"SID1"
#define FIELD_PLAYER2_TEAM						"SID2"
#define FIELD_PLAYER1_COLOR						"COL1"
#define FIELD_PLAYER2_COLOR						"COL2"
#define FIELD_PLAYER1_CREDITS					"CRD1"
#define FIELD_PLAYER2_CREDITS					"CRD2"

#define FIELD_PLAYER1_UNITS_LEFT				"UNL1"
#define FIELD_PLAYER2_UNITS_LEFT				"UNL2"
#define FIELD_PLAYER1_INFANTRY_LEFT				"INL1"
#define FIELD_PLAYER2_INFANTRY_LEFT				"INL2"
#define FIELD_PLAYER1_PLANES_LEFT				"PLL1"
#define FIELD_PLAYER2_PLANES_LEFT				"PLL2"
#define FIELD_PLAYER1_BUILDINGS_LEFT			"BLL1"
#define FIELD_PLAYER2_BUILDINGS_LEFT			"BLL2"
#define FIELD_PLAYER1_VESSELS_LEFT				"VSL1"
#define FIELD_PLAYER2_VESSELS_LEFT				"VSL2"

#define FIELD_PLAYER1_UNITS_BOUGHT				"UNB1"
#define FIELD_PLAYER2_UNITS_BOUGHT				"UNB2"
#define FIELD_PLAYER1_INFANTRY_BOUGHT			"INB1"
#define FIELD_PLAYER2_INFANTRY_BOUGHT			"INB2"
#define FIELD_PLAYER1_PLANES_BOUGHT				"PLB1"
#define FIELD_PLAYER2_PLANES_BOUGHT				"PLB2"
#define FIELD_PLAYER1_BUILDINGS_BOUGHT			"BLB1"
#define FIELD_PLAYER2_BUILDINGS_BOUGHT			"BLB2"
#define FIELD_PLAYER1_VESSELS_BOUGHT			"VSB1"
#define FIELD_PLAYER2_VESSELS_BOUGHT			"VSB2"

#define FIELD_PLAYER1_UNITS_KILLED				"UNK1"
#define FIELD_PLAYER2_UNITS_KILLED				"UNK2"
#define FIELD_PLAYER1_INFANTRY_KILLED			"INK1"
#define FIELD_PLAYER2_INFANTRY_KILLED			"INK2"
#define FIELD_PLAYER1_PLANES_KILLED				"PLK1"
#define FIELD_PLAYER2_PLANES_KILLED				"PLK2"
#define FIELD_PLAYER1_BUILDINGS_KILLED			"BLK1"
#define FIELD_PLAYER2_BUILDINGS_KILLED			"BLK2"
#define FIELD_PLAYER1_VESSELS_KILLED			"VSK1"
#define FIELD_PLAYER2_VESSELS_KILLED			"VSK2"

#define FIELD_PLAYER1_BUILDINGS_CAPTURED		"BLC1"
#define FIELD_PLAYER2_BUILDINGS_CAPTURED		"BLC2"

#define FIELD_PLAYER1_CRATES_FOUND				"CRA1"
#define FIELD_PLAYER2_CRATES_FOUND				"CRA2"
#define FIELD_PLAYER1_HARVESTED					"HRV1"
#define FIELD_PLAYER2_HARVESTED					"HRV2"


#define	PACKET_TYPE_HOST_GAME_INFO		(unsigned char) 50
#define	PACKET_TYPE_GUEST_GAME_INFO		(unsigned char) 51

//	Note: These enums match those in the game results server code.
enum {
	COMPLETION_CONNECTION_LOST,
	COMPLETION_PLAYER_1_WON,
	COMPLETION_PLAYER_1_WON_BY_RESIGNATION,
	COMPLETION_PLAYER_1_WON_BY_DISCONNECTION,
	COMPLETION_PLAYER_2_WON,
	COMPLETION_PLAYER_2_WON_BY_RESIGNATION,
	COMPLETION_PLAYER_2_WON_BY_DISCONNECTION,
#ifdef FIXIT_VERSION_3		//	Stalemate games.
	COMPLETION_WASH = 64,
#endif
};


extern unsigned long 	PlanetWestwoodGameID;
extern HINSTANCE		ProgramInstance;
extern unsigned long 	PlanetWestwoodStartTime;

extern "C" char	CPUType;


bool			GameTimerInUse = false;
TimerClass	GameTimer;
long			GameEndTime;
void			*PacketLater = NULL;

//#include "WolDebug.h"

#ifdef WOLAPI_INTEGRATION
#include "WolapiOb.h"
extern WolapiObject* pWolapi;

extern bool bReconnectDialogCancelled;
#endif

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
#if (0)//PG
//	debugprint( "Stats: Send_Statistics_Packet() called.\n" );
#ifndef INTERNET_OFF // Denzil 5/4/98

#ifdef WOLAPI_INTEGRATION
	if( !pWolapi )			//	Should no longer ever happen.
		return;
#endif

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
	static char field_player_vessels_left[5]		= { "VSL?" };
	static char field_player_units_bought[5] 		= { "UNB?" };
	static char field_player_infantry_bought[5] 	= { "INB?" };
	static char field_player_planes_bought[5]		= { "PLB?" };
	static char field_player_buildings_bought[5]	= { "BLB?" };
	static char field_player_vessels_bought[5]	= { "VSB?" };
	static char field_player_units_killed[5] 		= { "UNK?" };
	static char field_player_infantry_killed[5] 	= { "INK?" };
	static char field_player_planes_killed[5]		= { "PLK?" };
	static char field_player_buildings_killed[5]	= { "BLK?" };
	static char field_player_vessels_killed[5]	= { "VSK?" };
	static char field_player_buildings_captured[5] = { "BLC?" };
	static char field_player_crates_found[5]		= { "CRA?" };
	static char field_player_harvested[5]			= { "HRV?" };

	static char *houses[] = {
		"SPA",
		"GRE",
		"USS",
		"ENG",
		"ITA",
		"GER",
		"FRA",
		"TKY",
		"GUD",
		"BAD",
		"CIV",
		"JP ",
		"M01",
		"M02",
		"M03",
		"M04",
		"M05",
		"M06",
		"M07",
		"M08"
	};

	if (!PacketLater){

		/*
		** Field to identify this as C&C 95 internet game statistics packet
		*/
#ifdef WOLAPI_INTEGRATION
		if( pWolapi->bGameServer )
		{
			stats.Add_Field( FIELD_HOSTORNOT, (unsigned char)0 );		//	Reversed meaning of this for Neal.
		}else{
			stats.Add_Field( FIELD_HOSTORNOT, (unsigned char)1 );
		}
#else
		if (Server){
			stats.Add_Field(FIELD_PACKET_TYPE, PACKET_TYPE_HOST_GAME_INFO);
		}else{
			stats.Add_Field(FIELD_PACKET_TYPE, PACKET_TYPE_GUEST_GAME_INFO);
		}
#endif

		/*
		** Game ID. A unique game identifier assigned by WChat.
		*/
		stats.Add_Field(FIELD_GAME_ID, PlanetWestwoodGameID);

#ifdef WOLAPI_INTEGRATION

		//	Number of players initially in game.
		stats.Add_Field( FIELD_NUM_INITIAL_PLAYERS, (unsigned long)pWolapi->GameInfoCurrent.iPlayerCount );
//debugprint( "Stats: number of initial players is %i\n", pWolapi->GameInfoCurrent.iPlayerCount );

		//	Number of players remaining in game. Not sure of what use this will be statistically...
		stats.Add_Field( FIELD_NUM_REMAINING_PLAYERS, (unsigned long)Session.Players.Count() );
//debugprint( "Stats: number of remaining players is %i\n", Session.Players.Count() );

		//	Whether or not this was a tournament game.
		stats.Add_Field( FIELD_TOURNAMENT, (unsigned char)( pWolapi->GameInfoCurrent.bTournament ? 1 : 0 ) );

//	ajw This is now in WOLAPI...
//		//	A unique value that identifies the machine that the game was played on.
//		HW_PROFILE_INFO hwinfo;
//		::GetCurrentHwProfile( &hwinfo );
//		stats.Add_Field( FIELD_HARDWARE_GUID, hwinfo.szHwProfileGuid );
#endif

		/*
		** Start credits.
		*/
		stats.Add_Field(FIELD_START_CREDITS, (unsigned long)Session.Options.Credits);

		/*
		** Bases (On/Off)
		*/
		stats.Add_Field(FIELD_BASES, Session.Options.Bases ? "ON" : "OFF");

		/*
		** Tiberium (On/Off)
		*/
		stats.Add_Field(FIELD_TIBERIUM, Session.Options.Tiberium ? "ON" : "OFF");

		/*
		** Crates (On/Off)
		*/
		stats.Add_Field(FIELD_CRATES, Session.Options.Goodies ? "ON" : "OFF");

		/*
		** AI Players (On/Off)
		*/
		stats.Add_Field(FIELD_AI_PLAYERS, (unsigned long) Session.Options.AIPlayers);

		/*
		** Shadow regrowth enabled
		*/
		stats.Add_Field(FIELD_SHADOW_REGROWS, Special.IsShadowGrow ? "ON" : "OFF" );

		/*
		** Capture the flag mode (On/Off)
		*/
		stats.Add_Field(FIELD_CAPTURE_THE_FLAG, Special.IsCaptureTheFlag ? "ON" : "OFF");

		/*
		** Start unit count
		*/
		stats.Add_Field(FIELD_START_UNIT_COUNT, (unsigned long)Session.Options.UnitCount);

		/*
		** Tech level.
		*/
		stats.Add_Field(FIELD_TECH_LEVEL, (unsigned long)BuildLevel);

		/*
		** Scenario
		*/
#if (1)

		stats.Add_Field(FIELD_SCENARIO, Session.Options.ScenarioDescription);

#else //(1)
		char fname[128];
		char namebuffer[40];
		char *abuffer = (char *)_ShapeBuffer;
		memset(abuffer, '\0', _ShapeBufferSize);
		sprintf(fname,"%s.INI",Scen.ScenarioName);
		CCFileClass fileo;
		fileo.Set_Name (fname);
		fileo.Read(abuffer, _ShapeBufferSize-1);
		fileo.Close();
		WWGetPrivateProfileString("Basic", "Name", "Nulls-Ville", namebuffer, 40, abuffer);
		stats.Add_Field(FIELD_SCENARIO, namebuffer);
		//stats.Add_Field(FIELD_SCENARIO, MPlayerScenarios[ScenarioIdx]);
#endif	//(1)

#ifdef WOLAPI_INTEGRATION
		//	Completion status is set for Tournament games only - ajw.
		if( pWolapi->GameInfoCurrent.bTournament )
		{
#endif

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
			**	Game was a draw
			*/
			HouseClass *player1 = NULL;
			HouseClass *player2 = NULL;
			for (int h=0 ; h<Session.Players.Count(); h++){
				HouseClass *ptr = HouseClass::As_Pointer ((HousesType)(h + HOUSE_MULTI1));
				if (ptr->IsHuman){
					if (player1){
						player2 = ptr;
						break;
					}else{
						player1 = ptr;
					}
				}
			}

			int completion = -1;

			if (player1 && player2){			//	Can this ever fail?		ajw
#ifdef FIXIT_VERSION_3
				//	Send IP addresses of both players.
				NetNumType net;
				NetNodeType node;
				char szIPAddress[ 30 ];
				Session.Players[ 0 ]->Address.Get_Address( net, node );
				sprintf( szIPAddress, "%i.%i.%i.%i", node[0], node[1], node[2], node[3] );
				if( strcmp( szIPAddress, "255.255.255.255" ) == 0 )
				{

					//	Ok. It's not set. Let's try to get it ourselves...
					char szHostName[ 512 ];
					int iRes = gethostname( szHostName, 512 );
					if( iRes != SOCKET_ERROR )	//	else forget about trying
					{
//						debugprint( "gethostname got me %s\n", szHostName );
						struct hostent* pHostent = gethostbyname( szHostName );
						if( pHostent )	//	else forget about trying
						{
							int i = 0;
							int* piAddress = (int*)pHostent->h_addr_list[ i ];
							while( piAddress )
							{
								//	There is a non-null value for this h_addr_list entry.
								char szAsciiIP[ 30 ];
								strcpy( szAsciiIP, inet_ntoa( *( (struct in_addr*)piAddress ) ) );
								//	We have an address in the right form.
								//	Now, is it an address in a private network? If so we should ignore it.
								unsigned char q1 = ( (char*)piAddress )[ 0 ];	//	First digit.
								unsigned char q2 = ( (char*)piAddress )[ 1 ];	//	Second digit.
//								debugprint( "ip: %s\n", szAsciiIP );
								if( q1 == 10 || ( q1 == 172 && ( q2 >= 16 && q2 <= 31 ) ) || ( q1 == 192 && q2 == 168 ) )
								{
									//	This is a private network address - ignore it and go on to next.
								}
								else
								{
									strcpy( szIPAddress, szAsciiIP );
									break;
								}
								piAddress = (int*)pHostent->h_addr_list[ ++i ];
							}
						}
//						else
//							debugprint( "gethostbyname failed. Error %i\n", WSAGetLastError() );
					}
//					else
//						debugprint( "gethostname failed with %i, error %i\n", iRes, WSAGetLastError() );
				}
				stats.Add_Field( FIELD_PLAYER1_IP, (char*)szIPAddress );
				Session.Players[ 1 ]->Address.Get_Address( net, node );
				sprintf( szIPAddress, "%i.%i.%i.%i", node[0], node[1], node[2], node[3] );
				stats.Add_Field( FIELD_PLAYER2_IP, (char*)szIPAddress );
#endif				
#ifdef FIXIT_VERSION_3		//	Stalemate games.
				if( Scen.bLocalProposesDraw && Scen.bOtherProposesDraw )
				{
					completion = COMPLETION_WASH;
				}
				else
				{
#endif
				if (ConnectionLost){
#ifdef WOLAPI_INTEGRATION
					if( bReconnectDialogCancelled )
					{
						if( Session.Players[ 0 ]->Player.ID == HOUSE_MULTI1 )
							//	I am player1.
							completion = COMPLETION_PLAYER_2_WON_BY_DISCONNECTION;
						else
							completion = COMPLETION_PLAYER_1_WON_BY_DISCONNECTION;
					}
					else
					{
						completion = COMPLETION_CONNECTION_LOST;
						if( pWolapi->bDisconnectPingingCompleted )
						{
							char szPingResult[ 8 ];		//	Format is "x/y a/b", e.g., "3/5 4/5"
							pWolapi->DisconnectPingResultsString( szPingResult );
							stats.Add_Field( FIELD_DISCONNECT_PINGS, (char*)szPingResult );
						}
//						else
//							debugprint( "Stats: bDisconnectPingingCompleted is false! Should be finished!!!!!!!!!!!!!!!\n" );
					}
#else
					completion = COMPLETION_CONNECTION_LOST;
#endif
				}else{

					if (player1->IsGiverUpper){
						completion = COMPLETION_PLAYER_2_WON_BY_DISCONNECTION;
					}

					if (player2->IsGiverUpper){
						completion = COMPLETION_PLAYER_1_WON_BY_DISCONNECTION;
					}


					if (player2->IsDefeated){
						/*
						** Player 1 won. Find out how.
						*/
						completion = COMPLETION_PLAYER_1_WON;
						if (player2->IsResigner){
							completion = COMPLETION_PLAYER_1_WON_BY_RESIGNATION;
						}else{
							if (player2->IsGiverUpper){
								completion = COMPLETION_PLAYER_1_WON_BY_DISCONNECTION;
							}
						}


					}else{

						if (player1->IsDefeated){
							/*
							** Player 2 won. Find out how.
							*/
							completion = COMPLETION_PLAYER_2_WON;
							if (player1->IsResigner){
								completion = COMPLETION_PLAYER_2_WON_BY_RESIGNATION;
							}else{
								if (player1->IsGiverUpper){
									completion = COMPLETION_PLAYER_2_WON_BY_DISCONNECTION;
								}
							}
						}
					}
				}
#ifdef FIXIT_VERSION_3		//	Stalemate games.
				}
#endif
			}

			stats.Add_Field (FIELD_COMPLETION, (char) completion);
//debugprint( "Stats: Tournament game completion value: %i\n", completion );

#ifdef WOLAPI_INTEGRATION
		}
#endif


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
#ifdef FIXIT_IP_CRASH
		long divisor = GameEndTime / 60;
		if (divisor != 0) {
			stats.Add_Field (FIELD_FRAME_RATE, (long) Frame / (GameEndTime/60) );
		} else {
			stats.Add_Field (FIELD_FRAME_RATE, 0l);
		}
#else
		stats.Add_Field (FIELD_FRAME_RATE, (long) Frame / (GameEndTime/60) );
#endif


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

		/*
		** Game speed setting.
		*/
		stats.Add_Field (FIELD_SPEED_SETTING, (char)Options.GameSpeed);

		/*
		** Red Alert version/build date
		*/
		char	version[128];
		sprintf (version, "V%s", VerNum.Version_Name() );
		stats.Add_Field (FIELD_GAME_VERSION, (char*)version);

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
		//stats.Add_Field(FIELD_COVERT_PRESENT, (char) Expansion_Present());

		/*
		** Build the player specific statistics
		**
		*/
#ifdef WOLAPI_INTEGRATION
		for (int house = 0 ; house < 8 ; house++){
#else
		for (int house = 0 ; house < 2 ; house++){
#endif
			player = HouseClass::As_Pointer((HousesType) (house + HOUSE_MULTI1));

#ifdef WOLAPI_INTEGRATION
			if( !player )
				continue;
#endif

			/*
			** Player handle.
			*/
			field_player_handle[3] = '1' + (char)house;
#ifdef WOLAPI_INTEGRATION
			stats.Add_Field (field_player_handle, (char*) player->InitialName);
//debugprint( "Stats: Player %i name %s\n", house, (char*) player->InitialName );
//debugprint( "Stats: Player %i ending name %s\n", house, (char*) player->IniName );
#else
			stats.Add_Field (field_player_handle, (char*) player->IniName);
#endif

#ifdef WOLAPI_INTEGRATION
			//	Whether or not this player was taken over by the computer, due to his quitting the game.
			if( strcmp( player->IniName, player->InitialName ) )
				stats.Add_Field( FIELD_COMPUTERTOOKOVER, (unsigned char)1 );
			else
				stats.Add_Field( FIELD_COMPUTERTOOKOVER, (unsigned char)0 );
#endif
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
			field_player_vessels_bought[3] = '1' + (char)house;

			player->InfantryTotals->To_Network_Format();
			player->UnitTotals->To_Network_Format();
			player->AircraftTotals->To_Network_Format();
			player->BuildingTotals->To_Network_Format();
			player->VesselTotals->To_Network_Format();

			stats.Add_Field (field_player_infantry_bought, (void*) player->InfantryTotals->Get_All_Totals(), player->InfantryTotals->Get_Unit_Count()*4);
			stats.Add_Field (field_player_units_bought, (void*) player->UnitTotals->Get_All_Totals(), player->UnitTotals->Get_Unit_Count()*4);
			stats.Add_Field (field_player_planes_bought, (void*) player->AircraftTotals->Get_All_Totals(), player->AircraftTotals->Get_Unit_Count()*4);
			stats.Add_Field (field_player_buildings_bought, (void*) player->BuildingTotals->Get_All_Totals(), player->BuildingTotals->Get_Unit_Count()*4);
			stats.Add_Field (field_player_vessels_bought, (void*) player->VesselTotals->Get_All_Totals(), player->VesselTotals->Get_Unit_Count()*4);

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
			player->VesselTotals->Clear_Unit_Total();

			/*
			** Number of units remaining to player
			*/
			for (index = 0; index < Units.Count(); index++) {
				UnitClass const * unit = Units.Ptr(index);
				if (player == unit->House){
					player->UnitTotals->Increment_Unit_Total (unit->Class->Type);
				}
			}

			for (index = 0; index < Infantry.Count(); index++) {
			InfantryClass const * infantry = Infantry.Ptr(index);
				if (player == infantry->House && !infantry->Class->IsCivilian){
					player->InfantryTotals->Increment_Unit_Total (infantry->Class->Type);
				}
			}

			for (index = 0; index < Aircraft.Count(); index++) {
			AircraftClass const * aircraft = Aircraft.Ptr(index);
				if (player == aircraft->House){	// &&	aircraft->Class->Type != AIRCRAFT_CARGO){
					player->AircraftTotals->Increment_Unit_Total (aircraft->Class->Type);
				}
			}

			for (index = 0; index < Buildings.Count(); index++) {
			BuildingClass const * building = Buildings.Ptr(index);
				if (player == building->House){
					player->BuildingTotals->Increment_Unit_Total (building->Class->Type);
				}
			}

			for (index = 0; index < Vessels.Count(); index++) {
			VesselClass const * vessel = Vessels.Ptr(index);
				if (player == vessel->House){
					player->VesselTotals->Increment_Unit_Total (vessel->Class->Type);
				}
			}


			player->InfantryTotals->To_Network_Format();
			player->UnitTotals->To_Network_Format();
			player->AircraftTotals->To_Network_Format();
			player->BuildingTotals->To_Network_Format();
			player->VesselTotals->To_Network_Format();

			field_player_infantry_left[3] = '1' + (char)house;
			field_player_units_left[3] = '1' + (char)house;
			field_player_planes_left[3] = '1' + (char)house;
			field_player_buildings_left[3] = '1' + (char)house;
#ifdef FIXIT_IP_STATS
			field_player_vessels_left[3] = '1' + (char)house;
#endif
			stats.Add_Field (field_player_infantry_left, (void*) player->InfantryTotals->Get_All_Totals(), player->InfantryTotals->Get_Unit_Count()*4);
			stats.Add_Field (field_player_units_left, (void*) player->UnitTotals->Get_All_Totals(), player->UnitTotals->Get_Unit_Count()*4);
			stats.Add_Field (field_player_planes_left, (void*) player->AircraftTotals->Get_All_Totals(), player->AircraftTotals->Get_Unit_Count()*4);
			stats.Add_Field (field_player_buildings_left, (void*) player->BuildingTotals->Get_All_Totals(), player->BuildingTotals->Get_Unit_Count()*4);
			stats.Add_Field (field_player_vessels_left, (void*) player->VesselTotals->Get_All_Totals(), player->VesselTotals->Get_Unit_Count()*4);


			/*
			** Number of enemy units/buildings of each type destroyed.
			*/

			player->DestroyedInfantry->To_Network_Format();
			player->DestroyedUnits->To_Network_Format();
			player->DestroyedAircraft->To_Network_Format();
			player->DestroyedBuildings->To_Network_Format();
			player->DestroyedVessels->To_Network_Format();

			field_player_infantry_killed[3] = '1' + (char)house;
			field_player_units_killed[3] = '1' + (char)house;
			field_player_planes_killed[3] = '1' + (char)house;
			field_player_buildings_killed[3] = '1' + (char)house;
			field_player_vessels_killed[3] = '1' + (char)house;

			stats.Add_Field (field_player_infantry_killed, (void*) player->DestroyedInfantry->Get_All_Totals(), player->DestroyedInfantry->Get_Unit_Count()*4);
			stats.Add_Field (field_player_units_killed, (void*) player->DestroyedUnits->Get_All_Totals(), player->DestroyedUnits->Get_Unit_Count()*4);
			stats.Add_Field (field_player_planes_killed, (void*) player->DestroyedAircraft->Get_All_Totals(), player->DestroyedAircraft->Get_Unit_Count()*4);
			stats.Add_Field (field_player_buildings_killed, (void*) player->DestroyedBuildings->Get_All_Totals(), player->DestroyedBuildings->Get_Unit_Count()*4);
#ifdef FIXIT_VERSION_3
			stats.Add_Field (field_player_vessels_killed, (void*) player->DestroyedVessels->Get_All_Totals(), player->DestroyedVessels->Get_Unit_Count()*4);
#else
			stats.Add_Field (field_player_vessels_killed, (void*) player->DestroyedVessels->Get_All_Totals(), player->DestroyedBuildings->Get_Unit_Count()*4);
#endif


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

		/*
		** Create the comms packet to be sent
		*/
		packet = stats.Create_Comms_Packet(packet_size);

#ifndef WOLAPI_INTEGRATION	//	ajw - 'PacketLater' is no longer ever used.
		/*
		** If a player disconnected then dont send the packet at this time - save it for later
		*/
		if (completion == COMPLETION_PLAYER_1_WON_BY_DISCONNECTION
			|| completion == COMPLETION_PLAYER_2_WON_BY_DISCONNECTION){
			PacketLater = packet;
			return;
		}
#endif

	}else{		//else for if (!PacketLater)

		/*
		** Send the packet we calculated earlier when the disconnect occurred
		*/
		packet = PacketLater;
		PacketLater = NULL;
	}

	/*
	** Send it.....
	*/
	const char* szGameResServer;
	int iPort;
	if( pWolapi->GameInfoCurrent.GameKind == CREATEGAMEINFO::AMGAME )
	{
		szGameResServer = pWolapi->szGameResServerHost2;
		iPort = pWolapi->iGameResServerPort2;
	}
	else
	{
		szGameResServer = pWolapi->szGameResServerHost1;
		iPort = pWolapi->iGameResServerPort1;
	}

	if( *szGameResServer )
	{
		if( pWolapi->pNetUtil->RequestGameresSend( szGameResServer, iPort, (unsigned char*)packet, packet_size ) != S_OK )
			//debugprint( "RequestGameresSend( %s, %i ) failed!!!\n", szGameResServer, iPort );
			;
	}

	/*
	** Save it to disk as well so I can see it
	*/
//	RawFileClass anotherfile ("packet.net");
//	anotherfile.Write(packet, packet_size);
//debugprint( "Wrote out packet.net\n" );

	/*
	** Tidy up
	*/
	delete [] packet;

	GameStatisticsPacketSent = true;
#endif // INTERNET_OFF
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


#endif	//WIN32
