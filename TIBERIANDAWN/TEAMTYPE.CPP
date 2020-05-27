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

/* $Header:   F:\projects\c&c\vcs\code\teamtype.cpv   2.17   16 Oct 1995 16:48:52   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : TEAMTYPE.CPP                             *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 7, 1994                         *
 *                                                                         *
 *                  Last Update : July 21, 1995 [JLB]                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   TeamTypeClass::TeamTypeClass -- class constructor                     *
 *   TeamTypeClass::~TeamTypeClass -- class destructor                     *
 *   TeamTypeClass::Init -- pre-scenario initialization                    *
 *   TeamTypeClass::Read_INI -- reads INI data                             *
 *   TeamTypeClass::Write_INI -- writes INI data                           *
 *   TeamTypeClass::Read_Old_INI -- reads old INI format                   *
 *   TeamTypeClass::As_Pointer -- gets ptr for team type with given name   *
 *   TeamTypeClass::Remove -- removes this team from the system            *
 *   TeamTypeClass::Mission_From_Name -- returns mission for given name    *
 *   TeamTypeClass::Name_From_Mission -- returns name for given mission    *
 *   TeamTypeClass::operator new -- 'new' operator                         *
 *   TeamTypeClass::operator delete -- 'delete' operator                   *
 *   TeamTypeClass::Suggested_New_Team -- Suggests a new team to create.   *
 *   TeamTypeClass::Validate -- validates teamtype pointer						*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


/*
********************************** Globals **********************************
*/
char const * TeamTypeClass::TMissions[TMISSION_COUNT] = {
	"Attack Base",
	"Attack Units",
	"Attack Civil.",
	"Rampage",
	"Defend Base",
//	"Harvest",
	"Move",
	"Move to Cell",
	"Retreat",
	"Guard",
	"Loop",
	"Attack Tarcom",
	"Unload",
};

/*
** This contains the value of the Virtual Function Table Pointer
*/
void * TeamTypeClass::VTable;


/***********************************************************************************************
 * TeamTypeClass::Validate -- validates teamtype pointer													  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		none.																												  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		1 = ok, 0 = error																								  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		none.																												  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/09/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
#ifdef CHEAT_KEYS
int TeamTypeClass::Validate(void) const
{
	int num;

	num = TeamTypes.ID(this);
	if (num < 0 || num >= TEAMTYPE_MAX) {
		Validate_Error("TEAMTYPE");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


/***************************************************************************
 * TeamTypeClass::TeamTypeClass -- class constructor                       *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *=========================================================================*/
TeamTypeClass::TeamTypeClass(void)
{
	IsPrebuilt = true;
	IsReinforcable = true;
	IsRoundAbout = false;
	IsLearning = false;
	IsSuicide = false;
	IsAutocreate = false;
	IsTransient = false;
	IsMercenary = false;
	RecruitPriority = 7;
	MaxAllowed = 0;
	Fear = 0;
	InitNum = 0;
	House = HOUSE_NONE;
	MissionCount = 0;
	IniName[0] = '\0';
	ClassCount = 0;
	for (int i = 0; i < MAX_TEAM_CLASSCOUNT; i++) {
		Class[i] = NULL;
		DesiredNum[i] = 0;
	}
}


/***************************************************************************
 * TeamTypeClass::Init -- pre-scenario initialization                      *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *=========================================================================*/
void TeamTypeClass::Init(void)
{
	TeamTypeClass *ptr;

	TeamTypes.Free_All();

	ptr = new TeamTypeClass();
	VTable = ((void **)(((char *)ptr) + sizeof(AbstractTypeClass) - 4))[0];
	delete ptr;
}


/***************************************************************************
 * TeamTypeClass::Read_INI -- reads INI data                               *
 *                                                                         *
 * INI entry format:                                                       *
 *      TeamName = Housename,Roundabout,Learning,Suicide,Spy,Mercenary,    *
 *       RecruitPriority,MaxAllowed,InitNum,Fear,                          *
 *       ClassCount,Class:Num,Class:Num,...,                               *
 *       MissionCount,Mission:Arg,Mission:Arg,Mission:Arg,...              *
 *                                                                         *
 * INPUT:                                                                  *
 *      buffer      buffer to hold the INI data                            *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *   02/01/1995 BR : No del team if no classes (editor needs empty teams!) *
 *=========================================================================*/
void TeamTypeClass::Read_INI(char *buffer)
{
	TeamTypeClass * team;					// Working team pointer.
	char *tbuffer;								// Accumulation buffer of team names.
	int len;										// Length of data in buffer.
	char buf[500];								// INI entry buffer

	/*------------------------------------------------------------------------
	Set 'tbuffer' to point just past the INI buffer
	------------------------------------------------------------------------*/
	len = strlen(buffer) + 2;
	tbuffer = buffer + len;

	/*------------------------------------------------------------------------
	Read all TeamType entry names into 'tbuffer'
	------------------------------------------------------------------------*/
	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-len, buffer);

	/*
	----------------------- Loop for all team entries ------------------------
	*/
	while (*tbuffer != '\0') {
		/*
		....................... Create a new team type ........................
		*/
		team = new TeamTypeClass();

		/*
		......................... Get the team entry ..........................
		*/
		WWGetPrivateProfileString(INI_Name(), tbuffer, NULL, buf, sizeof(buf)-1, buffer);

		/*
		.......................... Fill the team in ...........................
		*/
		team->Fill_In(tbuffer, buf);

		/*
		...................... Go to the next INI entry .......................
		*/
		tbuffer += strlen(tbuffer)+1;
	}

	/*
	** If no teams were read in, try reading the old INI format.
	*/
	if (TeamTypes.Count()==0) {
		Read_Old_INI(buffer);
	}
}


/***********************************************************************************************
 * TeamTypeClass::Fill_In -- fills in trigger from the given INI entry                         *
 *                                                                                             *
 * This routine fills in the given teamtype with the given name, and values from               *
 * the given INI entry.                                                                        *
 *                                                                                             *
 * (This routine is used by the scenario editor, to import teams from the MASTER.INI file.)    *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      TeamName = Housename,Roundabout,Learning,Suicide,Spy,Mercenary,                        *
 *       RecruitPriority,MaxAllowed,InitNum,Fear,                                              *
 *       ClassCount,Class:Num,Class:Num,...,                                                   *
 *       MissionCount,Mission:Arg,Mission:Arg,Mission:Arg,...                                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      name      mnemonic for the desired trigger                                             *
 *      entry      INI entry to parse                                                          *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void TeamTypeClass::Fill_In(char * name, char *entry)
{
	Validate();
	int num_classes;
	char *p1;									// parsing pointer
	char *p2;									// parsing pointer
	int i;										// loop counter
	TechnoTypeClass const *otype;			// ptr to type of object
	InfantryType i_id;						// infantry ID
	UnitType u_id;								// unit ID
	AircraftType a_id;						// aircraft ID
	TeamMissionStruct mission;

	/*
	------------------------------ Set its name ------------------------------
	*/
	Set_Name(name);

	/*
	---------------------------- 1st token: House ----------------------------
	*/
	House = HouseTypeClass::From_Name(strtok(entry,","));

	/*
	-------------------------- 2nd token: RoundAbout -------------------------
	*/
	IsRoundAbout = atoi(strtok(NULL,","));

	/*
	--------------------------- 3rd token: Learning --------------------------
	*/
	IsLearning = atoi(strtok(NULL,","));

	/*
	--------------------------- 4th token: Suicide ---------------------------
	*/
	IsSuicide = atoi(strtok(NULL,","));

	/*
	----------------------------- 5th token: Spy -----------------------------
	*/
	IsAutocreate = atoi(strtok(NULL,","));

	/*
	-------------------------- 6th token: Mercenary --------------------------
	*/
	IsMercenary = atoi(strtok(NULL,","));

	/*
	----------------------- 7th token: RecruitPriority -----------------------
	*/
	RecruitPriority = atoi(strtok(NULL,","));

	/*
	-------------------------- 8th token: MaxAllowed -------------------------
	*/
	MaxAllowed = atoi(strtok(NULL,","));

	/*
	--------------------------- 9th token: InitNum ---------------------------
	*/
	InitNum = atoi(strtok(NULL,","));

	/*
	------------------------- 10th token: Fear level -------------------------
	*/
	Fear = atoi(strtok(NULL,","));

	/*
	------------------------ 11th token: Class count -------------------------
	*/
	num_classes = atoi(strtok(NULL,","));

	/*
	-------------- Loop through entries, setting class ptr & num -------------
	*/
	ClassCount = 0;
	for (i = 0; i < num_classes; i++) {
		p1 = strtok(NULL,",:");
		p2 = strtok(NULL,",:");
		otype = NULL;

		/*
		------------------- See if this is an infantry name -------------------
		*/
		i_id = InfantryTypeClass::From_Name(p1);
		if (i_id != INFANTRY_NONE) {
			otype = &InfantryTypeClass::As_Reference(i_id);
		}

		/*
		---------------------- See if this is a unit name ---------------------
		*/
		u_id = UnitTypeClass::From_Name(p1);
		if (u_id != UNIT_NONE) {
			otype = &UnitTypeClass::As_Reference(u_id);
		}

		/*
		------------------- See if this is an aircraft name -------------------
		*/
		a_id = AircraftTypeClass::From_Name(p1);
		if (a_id != AIRCRAFT_NONE) {
			otype = &AircraftTypeClass::As_Reference(a_id);
		}

		/*
		--------------- If the name was resolved, add this class --------------
		*/
		if (otype) {
			if (ClassCount < MAX_TEAM_CLASSCOUNT) {
				Class[ClassCount] = otype;
				DesiredNum[ClassCount] = atoi(p2);
				ClassCount++;
			}
		}
	}

	/*
	----------------------- next token: Mission count ------------------------
	*/
	MissionCount = atoi(strtok(NULL,","));

	for (i = 0; i < MissionCount; i++) {
		p1 = strtok(NULL,",:");
		p2 = strtok(NULL,",:");
		mission.Mission = Mission_From_Name(p1);
		mission.Argument = atoi(p2);
		MissionList[i] = mission;
	}

	char * ptr = strtok(NULL, ",");
	if (ptr) {
		IsReinforcable = atoi(ptr);
	}
	ptr = strtok(NULL, ",");
	if (ptr) {
		IsPrebuilt = atoi(ptr);
	}
}


/***************************************************************************
 * TeamTypeClass::Write_INI -- writes INI data                             *
 *                                                                         *
 * INI entry format:                                                       *
 *      TeamName = Housename,Roundabout,Learning,Suicide,Spy,Mercenary,    *
 *       RecruitPriority,MaxAllowed,InitNum,Fear,                          *
 *       ClassCount,Class:Num,Class:Num,...,                               *
 *       MissionCount,Mission,Arg,Mission,Arg,Mission,Arg,...              *
 *                                                                         *
 * INPUT:                                                                  *
 *      buffer      buffer to store INI data in                            *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *=========================================================================*/
void TeamTypeClass::Write_INI(char *buffer, bool refresh)
{
	int index;
	int i;
	char buf[500];
	TeamTypeClass * team;
	char const *hname;

	/*------------------------------------------------------------------------
	First, clear out all existing teamtypes in the old-style format.
	------------------------------------------------------------------------*/
	WWWritePrivateProfileString("Teams", NULL, NULL, buffer);

	/*------------------------------------------------------------------------
	Clear out all existing teamtype data from the INI file.
	------------------------------------------------------------------------*/
	if (refresh) {
		WWWritePrivateProfileString(INI_Name(), NULL, NULL, buffer);
	}

	/*------------------------------------------------------------------------
	Now write all the team data out
	------------------------------------------------------------------------*/
	buf[0] = 0;
	for (index = 0; index < TeamTypes.Count(); index++) {
		/*
		.................. Get ptr to next active teamtype ....................
		*/
		team = TeamTypes.Ptr(index);

		/*
		.......................... Find house's name ..........................
		*/
		if (team->House==HOUSE_NONE) {
			hname = "None";
		} else {
			hname = HouseClass::As_Pointer(team->House)->Class->IniName;
		}

		/*
		......................... Generate INI entry ..........................
		*/
		sprintf(buf,"%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			hname,
			team->IsRoundAbout,
			team->IsLearning,
			team->IsSuicide,
			team->IsAutocreate,
			team->IsMercenary,
			team->RecruitPriority,
			team->MaxAllowed,
			team->InitNum,
			team->Fear,
			team->ClassCount);

		/*.....................................................................
		For every class in the team, record the class's name & desired count
		.....................................................................*/
		for (i = 0; i < team->ClassCount; i++) {
			sprintf (buf + strlen(buf), ",%s:%d",
				team->Class[i]->IniName,
				team->DesiredNum[i]);
		}

		/*.....................................................................
		Record the # of missions, and each mission name & argument value.
		.....................................................................*/
		sprintf(buf + strlen(buf),",%d",team->MissionCount);
		for (i = 0; i < team->MissionCount; i++) {
			sprintf (buf + strlen(buf), ",%s:%d",
				Name_From_Mission(team->MissionList[i].Mission),
				team->MissionList[i].Argument);
		}

		if (team->IsReinforcable) {
			strcat(buf, ",1");
		} else {
			strcat(buf, ",0");
		}
		if (team->IsPrebuilt) {
			strcat(buf, ",1");
		} else {
			strcat(buf, ",0");
		}

		WWWritePrivateProfileString(INI_Name(), team->IniName, buf, buffer);
	}
}


/***************************************************************************
 * TeamTypeClass::Read_Old_INI -- reads old INI format                     *
 *                                                                         *
 *    INI entry format:                                                    *
 *      TeamName = Housename,Roundabout,Learning,Suicide,Spy,Mercenary,    *
 *       RecruitPriority,MaxAllowed,InitNum,Class:Num,Class:Num,...,Fear   *
 *                                                                         *
 * INPUT:                                                                  *
 *      buffer      buffer to hold the INI data                            *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *   02/01/1995 BR : No del team if no classes (editor needs empty teams!) *
 *=========================================================================*/
void TeamTypeClass::Read_Old_INI(char *buffer)
{
	TeamTypeClass * team;					// Working team pointer.
	char *tbuffer;								// Accumulation buffer of team names.
	int len;										// Length of data in buffer.
	char buf[256];								// INI entry buffer
	char *p1;									// parsing pointer
	char *p2;									// parsing pointer
	int index;
	TechnoTypeClass const *otype;			// ptr to type of object
	InfantryType i_id;						// infantry ID
	UnitType u_id;								// unit ID
	AircraftType a_id;						// infantry ID

	/*------------------------------------------------------------------------
	Set 'tbuffer' to point just past the INI buffer
	------------------------------------------------------------------------*/
	len = strlen(buffer) + 2;
	tbuffer = buffer + len;

	/*------------------------------------------------------------------------
	Read all TeamType entry names into 'tbuffer'
	------------------------------------------------------------------------*/
	WWGetPrivateProfileString("Teams", NULL, NULL, tbuffer, ShapeBufferSize-len, buffer);

	/*
	----------------------- Loop for all team entries ------------------------
	*/
	while (*tbuffer != '\0') {
		/*
		........................ Create a new trigger .........................
		*/
		team = new TeamTypeClass();

		/*
		............................ Set its name .............................
		*/
		team->Set_Name(tbuffer);

		/*
		......................... Get the team entry ..........................
		*/
		WWGetPrivateProfileString("Teams", tbuffer, NULL, buf, sizeof(buf)-1, buffer);

		/*
		.......................... 1st token: House ...........................
		*/
		team->House = HouseTypeClass::From_Name(strtok(buf,","));

		/*
		........................ 2nd token: RoundAbout ........................
		*/
		team->IsRoundAbout = atoi(strtok(NULL,","));

		/*
		......................... 3rd token: Learning .........................
		*/
		team->IsLearning = atoi(strtok(NULL,","));

		/*
		......................... 4th token: Suicide ..........................
		*/
		team->IsSuicide = atoi(strtok(NULL,","));

		/*
		........................... 5th token: Spy ............................
		*/
		team->IsAutocreate = atoi(strtok(NULL,","));

		/*
		........................ 6th token: Mercenary .........................
		*/
		team->IsMercenary = atoi(strtok(NULL,","));

		/*
		..................... 7th token: RecruitPriority ......................
		*/
		team->RecruitPriority = atoi(strtok(NULL,","));

		/*
		........................ 8th token: MaxAllowed ........................
		*/
		team->MaxAllowed = atoi(strtok(NULL,","));

		/*
		......................... 9th token: InitNum ..........................
		*/
		team->InitNum = atoi(strtok(NULL,","));

		/*
		....................... 10th token: Mission name ......................
		*/
		strtok(NULL,",");	// just throw it away

		/*
		............ Loop through entries, setting class ptr & num ............
		*/
		index = 0;
		p1 = strtok(NULL,",:");
		p2 = strtok(NULL,",:");
		while (p1 && p2) {
			otype = NULL;

			/*
			................. See if this is an infantry name ..................
			*/
			i_id = InfantryTypeClass::From_Name(p1);
			if (i_id!=INFANTRY_NONE) {
				otype = &InfantryTypeClass::As_Reference(i_id);
			}

			/*
			.................... See if this is a unit name ....................
			*/
			u_id = UnitTypeClass::From_Name(p1);
			if (u_id != UNIT_NONE) {
				otype = &UnitTypeClass::As_Reference(u_id);
			}

			/*
			................. See if this is an aircraft name ..................
			*/
			a_id = AircraftTypeClass::From_Name(p1);
			if (a_id != AIRCRAFT_NONE) {
				otype = &AircraftTypeClass::As_Reference(a_id);
			}

			/*
			............. If the name was resolved, add this class .............
			*/
			if (otype) {
				team->Class[index] = otype;
				team->DesiredNum[index] = atoi(p2);
				index++;
				team->ClassCount = index;
			}

			/*
			................. Go to the next entry on the line .................
			*/
			p1 = strtok(NULL,",:");
			p2 = strtok(NULL,",:");
		}

		team->Fear = 0;

		/*
		...................... Go to the next INI entry .......................
		*/
		tbuffer += strlen(tbuffer)+1;
	}
}


/***************************************************************************
 * TeamTypeClass::As_Pointer -- gets ptr for team type with given name     *
 *                                                                         *
 * INPUT:                                                                  *
 *      name      name of teamtype                                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ptr to TeamType with that name                                     *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *=========================================================================*/
TeamTypeClass * TeamTypeClass::As_Pointer(char *name)
{
	int i;

	if (name == NULL) {
		return(NULL);
	}

	for (i = 0; i < TeamTypes.Count(); i++) {
		if (!stricmp(name, TeamTypes.Ptr(i)->IniName)) {
			return(TeamTypes.Ptr(i));
		}
	}

	return(NULL);
}


/***************************************************************************
 * TeamTypeClass::Remove -- removes this team from the system              *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/09/1994 BR : Created.                                              *
 *=========================================================================*/
void TeamTypeClass::Remove(void)
{
	Validate();
	int i;
	TriggerClass * trigger;

	/*
	**	Remove all trigger references to this team.
	*/
	for (i = 0; i < Triggers.Count(); i++) {
		trigger = Triggers.Ptr(i);
		if (trigger->Team == this) {
			trigger->Team = NULL;
		}
	}

	/*
	**	Delete myself.
	*/
	delete this;
}


/***************************************************************************
 * TeamTypeClass::Mission_From_Name -- returns team mission for given name *
 *                                                                         *
 * INPUT:                                                                  *
 *      name         name to compare                                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      mission for that name                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/13/1994 BR : Created.                                              *
 *=========================================================================*/
TeamMissionType TeamTypeClass::Mission_From_Name(char const *name)
{
	int order;

	if (name) {
		for (order = TMISSION_FIRST; order < TMISSION_COUNT; order++) {
			if (stricmp(TMissions[order], name) == 0) {
				return((TeamMissionType) order);
			}
		}
	}

	return(TMISSION_NONE);
}


/***************************************************************************
 * TeamTypeClass::Name_From_Mission -- returns name for given mission      *
 *                                                                         *
 * INPUT:                                                                  *
 *      order      mission to get name for                                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      name of mission                                                    *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/13/1994 BR : Created.                                              *
 *=========================================================================*/
char const * TeamTypeClass::Name_From_Mission(TeamMissionType order)
{
	if (order <= TMISSION_NONE || order >= TMISSION_COUNT) {
		return("None");
	} else {
		return(TMissions[order]);
	}
}


/***************************************************************************
 * TeamTypeClass::operator new -- 'new' operator                           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      pointer to new TeamType                                            *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/28/1994 BR : Created.                                              *
 *=========================================================================*/
void * TeamTypeClass::operator new(size_t )
{
	void * ptr = TeamTypes.Allocate();
	if (ptr) {
		((TeamTypeClass *)ptr)->IsActive = true;
	}
	return(ptr);
}


/***************************************************************************
 * TeamTypeClass::operator delete -- 'delete' operator                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      ptr      pointer to delete                                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/28/1994 BR : Created.                                              *
 *=========================================================================*/
void TeamTypeClass::operator delete(void *ptr)
{
	if (ptr) {
		((TeamTypeClass *)ptr)->IsActive = false;
	}
	TeamTypes.Free((TeamTypeClass *)ptr);
}



TeamClass * TeamTypeClass::Create_One_Of(void) const
{
	if (ScenarioInit || TeamClass::Number[TeamTypes.ID(this)] < MaxAllowed) {
		return(new TeamClass(this, HouseClass::As_Pointer(House)));
	}
	return(NULL);
}


TARGET TeamTypeClass::As_Target(void) const
{
	Validate();
	return(Build_Target(KIND_TEAMTYPE, TeamTypes.ID(this)));
}


void TeamTypeClass::Destroy_All_Of(void) const
{
	for (int index = 0; index < Teams.Count(); index++) {
		TeamClass * team = Teams.Ptr(index);

		if (team->Class == this) {
			delete team;
			index--;
		}
	}
}


/***********************************************************************************************
 * TeamTypeClass::Suggested_New_Team -- Suggests a new team to create.                         *
 *                                                                                             *
 *    This routine will scan through the team types available and create teams of the          *
 *    type that can best utilize the existing unit mix.                                        *
 *                                                                                             *
 * INPUT:   house    -- Pointer to the house that this team is to be created for.              *
 *                                                                                             *
 *          utype    -- A bit mask of the unit types available for this house.                 *
 *                                                                                             *
 *          itypes   -- A bit mask of the infantry types available for this house.             *
 *                                                                                             *
 *          alerted  -- Is this house alerted? If true, then the Autocreate teams will be      *
 *                      considered in the selection process.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team type that should be created. If no team should  *
 *          be created, then it returns NULL.                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/13/1995 JLB : Created.                                                                 *
 *   07/21/1995 JLB : Will autocreate team even if no members in field.                        *
 *=============================================================================================*/
TeamTypeClass const * TeamTypeClass::Suggested_New_Team(HouseClass * house, long utypes, long itypes, bool alerted)
{
	TeamTypeClass const * best = NULL;
	int bestvalue = 0;

	for (int index = 0; index < TeamTypes.Count(); index++) {
		TeamTypeClass const * ttype = TeamTypes.Ptr(index);

		if (ttype &&
				ttype->House == house->Class->House &&
				TeamClass::Number[index] < ((alerted || !ttype->IsAutocreate) ? ttype->MaxAllowed : 0)) {

			/*
			**	Determine what kind of units this team requires.
			*/
			long uneeded = 0;
			long ineeded = 0;
			for (int ctype = 0; ctype < ttype->ClassCount; ctype++) {
				switch (ttype->Class[ctype]->What_Am_I()) {
					case RTTI_INFANTRYTYPE:
						ineeded |= (1 << ((InfantryTypeClass *)ttype->Class[ctype])->Type);
						break;

					case RTTI_UNITTYPE:
						uneeded |= (1 << ((UnitTypeClass *)ttype->Class[ctype])->Type);
						break;
				}
			}

			/*
			**	If this team can use the types required, then consider it a possible
			**	team type to create.
			*/
			int value = 0;
			if ((ineeded & itypes) || (uneeded & utypes)) {
				value = ttype->RecruitPriority;
			} else {
				value = ttype->RecruitPriority/2;
			}

			if (!best || bestvalue < value) {
				bestvalue = value;
				best = ttype;
			}
		}
	}

	return(best);
}
