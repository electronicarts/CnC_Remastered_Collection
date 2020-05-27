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

/* $Header:   F:\projects\c&c\vcs\code\ioobj.cpv   2.18   16 Oct 1995 16:51:22   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : IOOBJ.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Bill Randolph                                                *
 *                                                                                             *
 *                   Start Date : January 16, 1995                                             *
 *                                                                                             *
 *                  Last Update : January 16, 1995   [BR]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * All object-related loading/saving routines should go in this module, so it can be overlayed.*
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TeamTypeClass::Load -- Reads from a save game file.                                       *
 *   TeamTypeClass::Save -- Write to a save game file.                                         *
 *   TeamTypeClass::Code_Pointers -- codes class's pointers for load/save                      *
 *   TeamTypeClass::Decode_Pointers -- decodes pointers for load/save                          *
 *   TeamClass::Load -- Reads from a save game file.                                           *
 *   TeamClass::Save -- Write to a save game file.                                             *
 *   TeamClass::Code_Pointers -- codes class's pointers for load/save                          *
 *   TeamClass::Decode_Pointers -- decodes pointers for load/save                              *
 *   TriggerClass::Load -- Reads from a save game file.                                        *
 *   TriggerClass::Save -- Write to a save game file.                                          *
 *   TriggerClass::Code_Pointers -- codes class's pointers for load/save                       *
 *   TriggerClass::Decode_Pointers -- decodes pointers for load/save                           *
 *   AircraftClass::Load -- Reads from a save game file.                                       *
 *   AircraftClass::Save -- Write to a save game file.                                         *
 *   AircraftClass::Code_Pointers -- codes class's pointers for load/save                      *
 *   AircraftClass::Decode_Pointers -- decodes pointers for load/save                          *
 *   AnimClass::Load -- Reads from a save game file.                                           *
 *   AnimClass::Save -- Write to a save game file.                                             *
 *   AnimClass::Code_Pointers -- codes class's pointers for load/save                          *
 *   AnimClass::Decode_Pointers -- decodes pointers for load/save                              *
 *   BuildingClass::Load -- Reads from a save game file.                                       *
 *   BuildingClass::Save -- Write to a save game file.                                         *
 *   BuildingClass::Code_Pointers -- codes class's pointers for load/save                      *
 *   BuildingClass::Decode_Pointers -- decodes pointers for load/save                          *
 *   BulletClass::Load -- Reads from a save game file.                                         *
 *   BulletClass::Save -- Write to a save game file.                                           *
 *   BulletClass::Code_Pointers -- codes class's pointers for load/save                        *
 *   BulletClass::Decode_Pointers -- decodes pointers for load/save                            *
 *   InfantryClass::Load -- Reads from a save game file.                                       *
 *   InfantryClass::Save -- Write to a save game file.                                         *
 *   InfantryClass::Code_Pointers -- codes class's pointers for load/save                      *
 *   InfantryClass::Decode_Pointers -- decodes pointers for load/save                          *
 *   OverlayClass::Load -- Reads from a save game file.                                        *
 *   OverlayClass::Save -- Write to a save game file.                                          *
 *   OverlayClass::Code_Pointers -- codes class's pointers for load/save                       *
 *   OverlayClass::Decode_Pointers -- decodes pointers for load/save                           *
 *   ReinforcementClass::Load -- Reads from a save game file.                                  *
 *   ReinforcementClass::Save -- Write to a save game file.                                    *
 *   ReinforcementClass::Code_Pointers -- codes class's pointers for load/save                 *
 *   ReinforcementClass::Decode_Pointers -- decodes pointers for load/save                     *
 *   SmudgeClass::Load -- Reads from a save game file.                                         *
 *   SmudgeClass::Save -- Write to a save game file.                                           *
 *   SmudgeClass::Code_Pointers -- codes class's pointers for load/save                        *
 *   SmudgeClass::Decode_Pointers -- decodes pointers for load/save                            *
 *   TemplateClass::Load -- Reads from a save game file.                                       *
 *   TemplateClass::Save -- Write to a save game file.                                         *
 *   TemplateClass::Code_Pointers -- codes class's pointers for load/save                      *
 *   TemplateClass::Decode_Pointers -- decodes pointers for load/save                          *
 *   TerrainClass::Load -- Reads from a save game file.                                        *
 *   TerrainClass::Save -- Write to a save game file.                                          *
 *   TerrainClass::Code_Pointers -- codes class's pointers for load/save                       *
 *   TerrainClass::Decode_Pointers -- decodes pointers for load/save                           *
 *   UnitClass::Load -- Reads from a save game file.                                           *
 *   UnitClass::Save -- Write to a save game file.                                             *
 *   UnitClass::Code_Pointers -- codes class's pointers for load/save                          *
 *   UnitClass::Decode_Pointers -- decodes pointers for load/save                              *
 *   FactoryClass::Load -- Reads from a save game file.                                        *
 *   FactoryClass::Save -- Write to a save game file.                                          *
 *   FactoryClass::Code_Pointers -- codes class's pointers for load/save                       *
 *   FactoryClass::Decode_Pointers -- decodes pointers for load/save                           *
 *   LayerClass::Load -- Reads from a save game file.                                          *
 *   LayerClass::Save -- Write to a save game file.                                            *
 *   LayerClass::Code_Pointers -- codes class's pointers for load/save                         *
 *   LayerClass::Decode_Pointers -- decodes pointers for load/save                             *
 *   HouseClass::Load -- Reads from a save game file.                                          *
 *   HouseClass::Save -- Write to a save game file.                                            *
 *   HouseClass::Code_Pointers -- codes class's pointers for load/save                         *
 *   HouseClass::Decode_Pointers -- decodes pointers for load/save                             *
 *   ScoreClass::Load -- Reads from a save game file.                                          *
 *   ScoreClass::Save -- Write to a save game file.                                            *
 *   ScoreClass::Code_Pointers -- codes class's pointers for load/save                         *
 *   ScoreClass::Decode_Pointers -- decodes pointers for load/save                             *
 *   FlyClass::Code_Pointers -- codes class's pointers for load/save                           *
 *   FlyClass::Decode_Pointers -- decodes pointers for load/save                               *
 *   FuseClass::Code_Pointers -- codes class's pointers for load/save                          *
 *   FuseClass::Decode_Pointers -- decodes pointers for load/save                              *
 *   TarComClass::Code_Pointers -- codes class's pointers for load/save                        *
 *   TarComClass::Decode_Pointers -- decodes pointers for load/save                            *
 *   TurretClass::Code_Pointers -- codes class's pointers for load/save                        *
 *   TurretClass::Decode_Pointers -- decodes pointers for load/save                            *
 *   DriveClass::Code_Pointers -- codes class's pointers for load/save                         *
 *   DriveClass::Decode_Pointers -- decodes pointers for load/save                             *
 *   FootClass::Code_Pointers -- codes class's pointers for load/save                          *
 *   FootClass::Decode_Pointers -- decodes pointers for load/save                              *
 *   RadioClass::Code_Pointers -- codes class's pointers for load/save                         *
 *   RadioClass::Decode_Pointers -- decodes pointers for load/save                             *
 *   TechnoClass::Code_Pointers -- codes class's pointers for load/save                        *
 *   TechnoClass::Decode_Pointers -- decodes pointers for load/save                            *
 *   FlasherClass::Code_Pointers -- codes class's pointers for load/save                       *
 *   FlasherClass::Decode_Pointers -- decodes pointers for load/save                           *
 *   CargoClass::Code_Pointers -- codes class's pointers for load/save                         *
 *   CargoClass::Decode_Pointers -- decodes pointers for load/save                             *
 *   MissionClass::Code_Pointers -- codes class's pointers for load/save                       *
 *   MissionClass::Decode_Pointers -- decodes pointers for load/save                           *
 *   ObjectClass::Code_Pointers -- codes class's pointers for load/save                        *
 *   ObjectClass::Decode_Pointers -- decodes pointers for load/save                            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

#pragma warning (disable : 4302)			// Truncation from pointer to TARGET


/***********************************************************************************************
 * TeamTypeClass::Load -- Loads from a save game file.                                         *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TeamTypeClass::Load(FileClass & file)
{
	::new (this) TeamTypeClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractTypeClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * TeamTypeClass::Save -- Write to a save game file.                                           *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TeamTypeClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * TeamTypeClass::Code_Pointers -- codes class's pointers for load/save                        *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TeamTypeClass::Code_Pointers(void)
{
	/*
	-------------------------- Code the Class array --------------------------
	*/
	for (int i = 0; i < ClassCount; i++) {
		Class[i] = (TechnoTypeClass *)TechnoType_To_Target(Class[i]);
	}
}


/***********************************************************************************************
 * TeamTypeClass::Decode_Pointers -- decodes pointers for load/save                            *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TeamTypeClass::Decode_Pointers(void)
{
	/*
	------------------------- Decode the Class array -------------------------
	*/
	for (int i = 0; i < ClassCount; i++) {
		Class[i] =  Target_To_TechnoType((TARGET)Class[i]);
		Check_Ptr((void *)Class[i],__FILE__,__LINE__);
	}
}


/***********************************************************************************************
 * TeamClass::Load -- Loads from a save game file.                                             *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TeamClass::Load(FileClass & file)
{
	::new (this) TeamClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * TeamClass::Save -- Write to a save game file.                                               *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TeamClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * TeamClass::Code_Pointers -- codes class's pointers for load/save                            *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TeamClass::Code_Pointers(void)
{
	TeamTypeClass const * cls;

	/*
	-------------------- Code Class & House for this team --------------------
	*/
	cls = Class;
	((TeamTypeClass *&)Class) = (TeamTypeClass *)cls->As_Target();
	((HouseClass *&)House) = (HouseClass *)House->Class->House;

	/*
	--------------------------- Code the 'Member' ----------------------------
	*/
	if (Member) {
		Member = (FootClass *)Member->As_Target();
	}
}


/***********************************************************************************************
 * TeamClass::Decode_Pointers -- decodes pointers for load/save                                *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TeamClass::Decode_Pointers(void)
{
	/*
	------------------- Decode Class & House for this team -------------------
	*/
	((TeamTypeClass *&)Class) = As_TeamType((TARGET)Class);
	Check_Ptr((void *)Class,__FILE__,__LINE__);
	((HouseClass *&)House) = HouseClass::As_Pointer(*((HousesType*)&House));
	Check_Ptr((void *)House,__FILE__,__LINE__);

	/*
	-------------------------- Decode the 'Member' ---------------------------
	*/
	if (Member) {
		switch (Target_Kind((TARGET)Member)) {
			case KIND_INFANTRY:
				Member = As_Infantry((TARGET)Member, false);
				break;

			case KIND_UNIT:
				Member = As_Unit((TARGET)Member, false);
				break;

			case KIND_AIRCRAFT:
				Member = As_Aircraft((TARGET)Member, false);
				break;

			default:
				Member = 0;
				break;
		}

		Check_Ptr((void *)Member,__FILE__,__LINE__);
	}
}


/***********************************************************************************************
 * TriggerClass::Load -- Loads from a save game file.                                          *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TriggerClass::Load(FileClass & file)
{
	::new (this) TriggerClass();
	int rc = Read_Object(this, sizeof(*this), file, false);
	
	//int rc = Read_Object(this, sizeof(*this), sizeof(*this), file, 0);

	/*
	-------------------------- Add to HouseTriggers --------------------------
	*/
	if (rc) {
		if (House != HOUSE_NONE) {
			HouseTriggers[House].Add(this);
		}
	}

	return(rc);
}


/***********************************************************************************************
 * TriggerClass::Save -- Write to a save game file.                                            *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TriggerClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * TriggerClass::Code_Pointers -- codes class's pointers for load/save                         *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TriggerClass::Code_Pointers(void)
{
	if (Team) {
		Team = (TeamTypeClass *)Team->As_Target();
	}
}


/***********************************************************************************************
 * TriggerClass::Decode_Pointers -- decodes pointers for load/save                             *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TriggerClass::Decode_Pointers(void)
{
	if (Team) {
		Team = As_TeamType((TARGET)Team);
		Check_Ptr((void *)Team,__FILE__,__LINE__);
	}
}


/***********************************************************************************************
 * AircraftClass::Load -- Loads from a save game file.                                         *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Load(FileClass & file)
{
	::new (this) AircraftClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * AircraftClass::Save -- Write to a save game file.                                           *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * AircraftClass::Code_Pointers -- codes class's pointers for load/save                        *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void AircraftClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((AircraftTypeClass *&)Class) = (AircraftTypeClass *)Class->Type;

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	FootClass::Code_Pointers();
	FlyClass::Code_Pointers();
}


/***********************************************************************************************
 * AircraftClass::Decode_Pointers -- decodes pointers for load/save                            *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void AircraftClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((AircraftTypeClass const *&)Class) = &AircraftTypeClass::As_Reference(*((AircraftType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	FootClass::Decode_Pointers();
	FlyClass::Decode_Pointers();
}


/***********************************************************************************************
 * AnimClass::Load -- Loads from a save game file.                                             *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AnimClass::Load(FileClass & file)
{
	::new (this) AnimClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * AnimClass::Save -- Write to a save game file.                                               *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AnimClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * AnimClass::Code_Pointers -- codes class's pointers for load/save                            *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void AnimClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((AnimTypeClass *&)Class) = (AnimTypeClass *)Class->Type;

	/*
	----------------------------- Code 'Object' ------------------------------
	*/
	if (Object) {
		Object = (ObjectClass *)Object->As_Target();
	}

	/*
	----------------------------- Code 'VirtualAnim' -------------------------
	*/
	if (VirtualAnim) {
		VirtualAnim = (AnimClass *)VirtualAnim->As_Target();
	}

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Code_Pointers();
	StageClass::Code_Pointers();
}


/***********************************************************************************************
 * AnimClass::Decode_Pointers -- decodes pointers for load/save                                *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void AnimClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((AnimTypeClass const *&)Class) = &AnimTypeClass::As_Reference(*((AnimType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);

	/*
	---------------------------- Decode 'Object' -----------------------------
	*/
	if (Object) {
		Object = As_Object((TARGET)Object, false);
		Check_Ptr((void *)Object,__FILE__,__LINE__);
	}

	/*
	---------------------------- Decode 'VirtualAnim' ------------------------
	*/
	if (VirtualAnim) {
		VirtualAnim = As_Animation((TARGET)VirtualAnim, false);
		Check_Ptr((void *)VirtualAnim, __FILE__, __LINE__);
	}

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Decode_Pointers();
	StageClass::Decode_Pointers();
}


/***********************************************************************************************
 * BuildingClass::Load -- Loads from a save game file.                                         *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Load(FileClass & file)
{
	::new (this) BuildingClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * BuildingClass::Save -- Write to a save game file.                                           *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * BuildingClass::Code_Pointers -- codes class's pointers for load/save                        *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void BuildingClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((BuildingTypeClass const *&)Class) = (BuildingTypeClass *)Class->Type;

	/*------------------------------------------------------------------------
	Code the Factory value; there's not target conversion routine for factories, 
	so just use its Array ID, plus 1 so it doesn't look like a NULL value when 
	it's converted back
	------------------------------------------------------------------------*/
	if (Factory) {
		Factory = (FactoryClass *)(Factories.ID(Factory) + 1);
	}

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	TechnoClass::Code_Pointers();
}


/***********************************************************************************************
 * BuildingClass::Decode_Pointers -- decodes pointers for load/save                            *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void BuildingClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((BuildingTypeClass const *&)Class) = &BuildingTypeClass::As_Reference(*((StructType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);

	/*------------------------------------------------------------------------
	Decode the Factory value, subtracting off the '1' we added when coding it
	------------------------------------------------------------------------*/
	if (Factory) {
		Factory = Factories.Raw_Ptr((int)Factory - 1);
		Check_Ptr((void *)Factory,__FILE__,__LINE__);
	}

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	TechnoClass::Decode_Pointers();
}


/***********************************************************************************************
 * BulletClass::Load -- Loads from a save game file.                                           *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BulletClass::Load(FileClass & file)
{
	::new (this) BulletClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * BulletClass::Save -- Write to a save game file.                                             *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BulletClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * BulletClass::Code_Pointers -- codes class's pointers for load/save                          *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void BulletClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((BulletTypeClass *&)Class) = (BulletTypeClass *)Class->Type;

	/*
	----------------------------- Code 'Payback' -----------------------------
	*/
	if (Payback)
		Payback = (TechnoClass *)Payback->As_Target();

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Code_Pointers();
	FlyClass::Code_Pointers();
	FuseClass::Code_Pointers();
}


/***********************************************************************************************
 * BulletClass::Decode_Pointers -- decodes pointers for load/save                              *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void BulletClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((BulletTypeClass const *&)Class) = &BulletTypeClass::As_Reference(*((BulletType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);

	/*
	---------------------------- Decode 'Payback' ----------------------------
	*/
	if (Payback) {
		Payback = As_Techno((TARGET)Payback, false);
		Check_Ptr((void *)Payback,__FILE__,__LINE__);
	}

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Decode_Pointers();
	FlyClass::Decode_Pointers();
	FuseClass::Decode_Pointers();
}


/***********************************************************************************************
 * InfantryClass::Load -- Loads from a save game file.                                         *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Load(FileClass & file)
{
	::new (this) InfantryClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * InfantryClass::Save -- Write to a save game file.                                           *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * InfantryClass::Code_Pointers -- codes class's pointers for load/save                        *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void InfantryClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((InfantryTypeClass *&)Class) = (InfantryTypeClass *)Class->Type;

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	FootClass::Code_Pointers();
}


/***********************************************************************************************
 * InfantryClass::Decode_Pointers -- decodes pointers for load/save                            *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void InfantryClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((InfantryTypeClass const *&)Class) = &InfantryTypeClass::As_Reference(*((InfantryType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	FootClass::Decode_Pointers();
}


/***********************************************************************************************
 * OverlayClass::Load -- Loads from a save game file.                                          *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool OverlayClass::Load(FileClass & file)
{
	::new (this) OverlayClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * OverlayClass::Save -- Write to a save game file.                                            *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool OverlayClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * OverlayClass::Code_Pointers -- codes class's pointers for load/save                         *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void OverlayClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((OverlayTypeClass *&)Class) = (OverlayTypeClass *)Class->Type;

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Code_Pointers();
}


/***********************************************************************************************
 * OverlayClass::Decode_Pointers -- decodes pointers for load/save                             *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void OverlayClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((OverlayTypeClass const *&)Class) = &OverlayTypeClass::As_Reference(*((OverlayType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Decode_Pointers();
}


/***********************************************************************************************
 * SmudgeClass::Load -- Loads from a save game file.                                           *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SmudgeClass::Load(FileClass & file)
{
	::new (this) SmudgeClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * SmudgeClass::Save -- Write to a save game file.                                             *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SmudgeClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * SmudgeClass::Code_Pointers -- codes class's pointers for load/save                          *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void SmudgeClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((SmudgeTypeClass const *&)Class) = (SmudgeTypeClass *)Class->Type;

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Code_Pointers();
}


/***********************************************************************************************
 * SmudgeClass::Decode_Pointers -- decodes pointers for load/save                              *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void SmudgeClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((SmudgeTypeClass const *&)Class) = &SmudgeTypeClass::As_Reference(*((SmudgeType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Decode_Pointers();
}


/***********************************************************************************************
 * TemplateClass::Load -- Loads from a save game file.                                         *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TemplateClass::Load(FileClass & file)
{
	::new (this) TemplateClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * TemplateClass::Save -- Write to a save game file.                                           *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TemplateClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * TemplateClass::Code_Pointers -- codes class's pointers for load/save                        *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TemplateClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((TemplateTypeClass *&)Class) = (TemplateTypeClass *)Class->Type;

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Code_Pointers();
}


/***********************************************************************************************
 * TemplateClass::Decode_Pointers -- decodes pointers for load/save                            *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TemplateClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((TemplateTypeClass const *&)Class) = &TemplateTypeClass::As_Reference(*((TemplateType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Decode_Pointers();
}


/***********************************************************************************************
 * TerrainClass::Load -- Loads from a save game file.                                          *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TerrainClass::Load(FileClass & file)
{
	::new (this) TerrainClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * TerrainClass::Save -- Write to a save game file.                                            *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TerrainClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * TerrainClass::Code_Pointers -- codes class's pointers for load/save                         *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TerrainClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((TerrainTypeClass *&)Class) = (TerrainTypeClass *)Class->Type;

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Code_Pointers();
	StageClass::Code_Pointers();
}


/***********************************************************************************************
 * TerrainClass::Decode_Pointers -- decodes pointers for load/save                             *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TerrainClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((TerrainTypeClass const *&)Class) = &TerrainTypeClass::As_Reference(*((TerrainType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	ObjectClass::Decode_Pointers();
	StageClass::Decode_Pointers();
}


/***********************************************************************************************
 * UnitClass::Load -- Loads from a save game file.                                             *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Load(FileClass & file)
{
	::new (this) UnitClass();
	return(Read_Object(this, sizeof(*this), file, true));
	
	//return(Read_Object(this, sizeof(AbstractClass), sizeof(*this), file, VTable));
}


/***********************************************************************************************
 * UnitClass::Save -- Write to a save game file.                                               *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * UnitClass::Code_Pointers -- codes class's pointers for load/save                            *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void UnitClass::Code_Pointers(void)
{
	TarComClass::Code_Pointers();
}


/***********************************************************************************************
 * UnitClass::Decode_Pointers -- decodes pointers for load/save                                *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void UnitClass::Decode_Pointers(void)
{
	TarComClass::Decode_Pointers();
}


/***********************************************************************************************
 * FactoryClass::Load -- Loads from a save game file.                                          *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FactoryClass::Load(FileClass & file)
{
	::new (this) FactoryClass();
	return(Read_Object(this, sizeof(*this), file, false));
	
	//return(Read_Object(this, sizeof(StageClass), sizeof(*this), file, 0));
}


/***********************************************************************************************
 * FactoryClass::Save -- Write to a save game file.                                            *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FactoryClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * FactoryClass::Code_Pointers -- codes class's pointers for load/save                         *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void FactoryClass::Code_Pointers(void)
{
	if (Object) {
		Object = (TechnoClass *)Object->As_Target();
	}

	((HouseClass *&)House) = (HouseClass *)House->Class->House;

	StageClass::Code_Pointers();
}


/***********************************************************************************************
 * FactoryClass::Decode_Pointers -- decodes pointers for load/save                             *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void FactoryClass::Decode_Pointers(void)
{
	if (Object) {
		Object = As_Techno((TARGET)Object, false);
		Check_Ptr((void *)Object,__FILE__,__LINE__);
	}

	((HouseClass *&)House) = HouseClass::As_Pointer(*((HousesType*)&House));
	Check_Ptr((void *)House,__FILE__,__LINE__);

	StageClass::Decode_Pointers();
}


/***********************************************************************************************
 * LayerClass::Load -- Loads from a save game file.                                            *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool LayerClass::Load(FileClass & file)
{
	int count;
	int i;
	ObjectClass * ptr;

	/*
	---------------------- Read # elements in the layer ----------------------
	*/
	if (file.Read(&count,sizeof(count)) != sizeof(count)) {
		return(false);
	}

	/*
	---------------------------- Clear the array -----------------------------
	*/
	Clear();

	/*
	----------------------- Read in all array elements -----------------------
	*/
	for (i = 0; i < count; i++) {
		if (file.Read(&ptr, sizeof(ObjectClass *)) != sizeof(ObjectClass *)) {
			return(false);
		}
		Add(ptr);
	}
	
	return(true);
}


/***********************************************************************************************
 * LayerClass::Save -- Write to a save game file.                                              *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool LayerClass::Save(FileClass & file)
{
	int count;
	int i;
	ObjectClass * ptr;

	/*
	------------------------- Save # array elements --------------------------
	*/
	count = Count();
	if (file.Write(&count, sizeof(count)) != sizeof(count))
		return(false);

	/*
	--------------------------- Save all elements ----------------------------
	*/
	for (i = 0; i < count; i++) {
		ptr = (*this)[i];
		if (file.Write(&ptr, sizeof(ObjectClass *)) != sizeof(ObjectClass *))
			return(false);
	}

	return(true);
}


/***********************************************************************************************
 * LayerClass::Code_Pointers -- codes class's pointers for load/save                           *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void LayerClass::Code_Pointers(void)
{
	ObjectClass *obj;

	for (int i = 0; i < Count(); i++) {
		obj = (*this)[i];
		(*this)[i] = (ObjectClass *)(obj->As_Target());
	}
}


/***********************************************************************************************
 * LayerClass::Decode_Pointers -- decodes pointers for load/save                               *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void LayerClass::Decode_Pointers(void)
{
	TARGET target;

	for (int i = 0; i < Count(); i++) {
		target = (TARGET)(*this)[i];
		(*this)[i] = (ObjectClass *)As_Object(target, false);
		Check_Ptr((*this)[i],__FILE__,__LINE__);
	}
}


/***********************************************************************************************
 * HouseClass::Load -- Loads from a save game file.                                            *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Load(FileClass & file)
{
	::new (this) HouseClass();
	return(Read_Object(this, sizeof(*this), file, false));
	
	//return(Read_Object(this, sizeof(*this), sizeof(*this), file, 0));
}


/***********************************************************************************************
 * HouseClass::Save -- Write to a save game file.                                              *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * HouseClass::Code_Pointers -- codes class's pointers for load/save                           *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void HouseClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((HouseTypeClass const *&)Class) = (HouseTypeClass const *)Class->House;
}


/***********************************************************************************************
 * HouseClass::Decode_Pointers -- decodes pointers for load/save                               *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void HouseClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((HouseTypeClass const *&)Class) = &HouseTypeClass::As_Reference(*((HousesType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);
}


/***********************************************************************************************
 * ScoreClass::Load -- Loads from a save game file.                                            *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ScoreClass::Load(FileClass & file)
{
	::new (this) ScoreClass();
	return(Read_Object(this, sizeof(*this), file, false));
	
	//return(Read_Object(this, sizeof(*this), sizeof(*this), file, 0));
}


/***********************************************************************************************
 * ScoreClass::Save -- Write to a save game file.                                              *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ScoreClass::Save(FileClass & file)
{
	return(Write_Object(this, sizeof(*this), file));
}


/***********************************************************************************************
 * ScoreClass::Code_Pointers -- codes class's pointers for load/save                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void ScoreClass::Code_Pointers(void)
{
}


/***********************************************************************************************
 * ScoreClass::Decode_Pointers -- decodes pointers for load/save                               *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void ScoreClass::Decode_Pointers(void)
{
}


/***********************************************************************************************
 * FlyClass::Code_Pointers -- codes class's pointers for load/save                             *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void FlyClass::Code_Pointers(void)
{
}


/***********************************************************************************************
 * FlyClass::Decode_Pointers -- decodes pointers for load/save                                 *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void FlyClass::Decode_Pointers(void)
{
}


/***********************************************************************************************
 * FuseClass::Code_Pointers -- codes class's pointers for load/save                            *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void FuseClass::Code_Pointers(void)
{
}


/***********************************************************************************************
 * FuseClass::Decode_Pointers -- decodes pointers for load/save                                *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void FuseClass::Decode_Pointers(void)
{
}


/***********************************************************************************************
 * TarComClass::Code_Pointers -- codes class's pointers for load/save                          *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TarComClass::Code_Pointers(void)
{
	TurretClass::Code_Pointers();
}


/***********************************************************************************************
 * TarComClass::Decode_Pointers -- decodes pointers for load/save                              *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TarComClass::Decode_Pointers(void)
{
	TurretClass::Decode_Pointers();
}


/***********************************************************************************************
 * TurretClass::Code_Pointers -- codes class's pointers for load/save                          *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TurretClass::Code_Pointers(void)
{
	DriveClass::Code_Pointers();
}


/***********************************************************************************************
 * TurretClass::Decode_Pointers -- decodes pointers for load/save                              *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TurretClass::Decode_Pointers(void)
{
	DriveClass::Decode_Pointers();
}


/***********************************************************************************************
 * DriveClass::Code_Pointers -- codes class's pointers for load/save                           *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void DriveClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Class' ------------------------------
	*/
	((UnitTypeClass *&)Class) = (UnitTypeClass *)Class->Type;

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	FootClass::Code_Pointers();
}


/***********************************************************************************************
 * DriveClass::Decode_Pointers -- decodes pointers for load/save                               *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void DriveClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Class' -----------------------------
	*/
	((UnitTypeClass const *&)Class) = &UnitTypeClass::As_Reference(*((UnitType*)&Class));
	Check_Ptr((void *)Class,__FILE__,__LINE__);

	/*
	---------------------------- Chain to parent -----------------------------
	*/
	FootClass::Decode_Pointers();
}


/***********************************************************************************************
 * FootClass::Code_Pointers -- codes class's pointers for load/save                            *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void FootClass::Code_Pointers(void)
{
	if (Team)
		Team = (TeamClass *)Team->As_Target();

	if (Member) {
		Member = (FootClass *)Member->As_Target();
	}

	TechnoClass::Code_Pointers();
}


/***********************************************************************************************
 * FootClass::Decode_Pointers -- decodes pointers for load/save                                *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void FootClass::Decode_Pointers(void)
{
	if (Team) {
		Team = As_Team((TARGET)Team, false);
		Check_Ptr((void *)Team,__FILE__,__LINE__);
	}

	if (Member) {
		Member = (FootClass *)As_Techno((TARGET)Member, false);
		Check_Ptr((void *)Member,__FILE__,__LINE__);
	}

	TechnoClass::Decode_Pointers();
}


/***********************************************************************************************
 * RadioClass::Code_Pointers -- codes class's pointers for load/save                           *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void RadioClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'Radio' ------------------------------
	*/
	if (Radio) {
		Radio = (RadioClass *)Radio->As_Target();
	}

	MissionClass::Code_Pointers();
}


/***********************************************************************************************
 * RadioClass::Decode_Pointers -- decodes pointers for load/save                               *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void RadioClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'Radio' -----------------------------
	*/
	if (Radio) {
		Radio = As_Techno((TARGET)Radio, false);
		Check_Ptr((void *)Radio,__FILE__,__LINE__);
	}

	MissionClass::Decode_Pointers();
}


/***********************************************************************************************
 * TechnoClass::Code_Pointers -- codes class's pointers for load/save                          *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TechnoClass::Code_Pointers(void)
{
	/*
	------------------------------ Code 'House' ------------------------------
	*/
	((HouseClass *&)House) = (HouseClass *)(House->Class->House);

	FlasherClass::Code_Pointers();
	StageClass::Code_Pointers();
	CargoClass::Code_Pointers();
	DoorClass::Code_Pointers();

	RadioClass::Code_Pointers();
}


/***********************************************************************************************
 * TechnoClass::Decode_Pointers -- decodes pointers for load/save                              *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void TechnoClass::Decode_Pointers(void)
{
	/*
	----------------------------- Decode 'House' -----------------------------
	*/
	((HouseClass *&)House) = HouseClass::As_Pointer(*((HousesType*)&House));
	Check_Ptr((void *)House,__FILE__,__LINE__);

	FlasherClass::Decode_Pointers();
	StageClass::Decode_Pointers();
	CargoClass::Decode_Pointers();
	DoorClass::Decode_Pointers();

	RadioClass::Decode_Pointers();
}


/***********************************************************************************************
 * FlasherClass::Code_Pointers -- codes class's pointers for load/save                         *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void FlasherClass::Code_Pointers(void)
{
}


/***********************************************************************************************
 * FlasherClass::Decode_Pointers -- decodes pointers for load/save                             *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void FlasherClass::Decode_Pointers(void)
{
}


/***********************************************************************************************
 * CargoClass::Code_Pointers -- codes class's pointers for load/save                           *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void CargoClass::Code_Pointers(void)
{
	/*
	---------------------------- Code 'CargoHold' ----------------------------
	*/
	if (CargoHold) {
		CargoHold = (FootClass *)CargoHold->As_Target();
	}
}


/***********************************************************************************************
 * CargoClass::Decode_Pointers -- decodes pointers for load/save                               *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void CargoClass::Decode_Pointers(void)
{
	/*
	--------------------------- Decode 'CargoHold' ---------------------------
	*/
	if (CargoHold) {
		CargoHold = (FootClass *)As_Techno((TARGET)CargoHold, false);
		Check_Ptr((void *)CargoHold,__FILE__,__LINE__);
	}
}


/***********************************************************************************************
 * MissionClass::Code_Pointers -- codes class's pointers for load/save                         *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void MissionClass::Code_Pointers(void)
{
	ObjectClass::Code_Pointers();
}


/***********************************************************************************************
 * MissionClass::Decode_Pointers -- decodes pointers for load/save                             *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void MissionClass::Decode_Pointers(void)
{
	ObjectClass::Decode_Pointers();
}


/***********************************************************************************************
 * ObjectClass::Code_Pointers -- codes class's pointers for load/save                          *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void ObjectClass::Code_Pointers(void)
{
	if (Next) {
		Next = (ObjectClass *)Next->As_Target();
	}

	if (Trigger) {
		Trigger = (TriggerClass *)Trigger->As_Target();
	}
}


/***********************************************************************************************
 * ObjectClass::Decode_Pointers -- decodes pointers for load/save                              *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
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
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void ObjectClass::Decode_Pointers(void)
{
	if (Next) {
		Next = As_Object((TARGET)Next, false);
		Check_Ptr((void *)Next,__FILE__,__LINE__);
	}

	if (Trigger) {
		Trigger = As_Trigger((TARGET)Trigger, false);
		Check_Ptr((void *)Trigger,__FILE__,__LINE__);
	}
}
