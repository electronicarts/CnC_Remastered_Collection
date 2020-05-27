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

/* $Header:   F:\projects\c&c\vcs\code\sidebar.h_v   2.18   16 Oct 1995 16:45:24   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SIDEBAR.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : October 20, 1994                                             *
 *                                                                                             *
 *                  Last Update : October 20, 1994   [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef SIDEBAR_GLYPHX_H
#define SIDEBAR_GLYPHX_H

#pragma once

#include	"function.h"
#include	"power.h"
#include	"factory.h"


/*
**  ST - 3/14/2019 10:49AM
** 
**  We are going to need one sidebar per player for multiplayer with GlyphX. We can't have different maps / cell arrays per 
**  player though, so SidebarClass being in the middle of the map/display class hierarchy is a problem.
** 
**  All the class static data will have to be made non-static so we can have multiple instances.
** 
**  So, this is a stub sidebar class with the functionality we need just to support the exporting of production data to the
**  GlyphX client.
** 
** 
*/




class SidebarGlyphxClass
{
	public:
		
		enum SideBarClassEnums {
			COLUMNS=2							// Number of side strips on sidebar.
		};
	
		SidebarGlyphxClass(void);

		/*
		** Initialization
		*/
		void Init_Clear(HouseClass *player_ptr);						// Clears all to known state
		void Init_IO(void);							// Inits button list

		void AI(KeyNumType & input, int x, int y);

		bool  Abandon_Production(RTTIType type, int factory);
		bool  Add(RTTIType type, int ID, bool via_capture = false);
		void  Recalc(void);
		bool  Factory_Link(int factory, RTTIType type, int id);

		/*
		**	File I/O.
		*/
		void Code_Pointers(void);
	  	void Decode_Pointers(void);
		bool Load(Straw &file);
		bool Save(Pipe &pipe);

		/*
		**	Each side strip is managed by this class. It handles all strip specific
		**	actions.
		*/
		class StripClass : public StageClass
		{
			class SelectClass : public ControlClass
			{
				public:
					SelectClass(void);

					void Set_Owner(StripClass & strip, int index);
					StripClass * Strip;
					int Index;

				protected:
					int Action(unsigned flags, KeyNumType & key);
			};

			public:

				StripClass(void);
				bool  Add(RTTIType type, int ID, bool via_capture);
				bool  Abandon_Production(int factory);
				bool  AI(KeyNumType & input, int x, int y);
				void  One_Time(int id);
				void  Init_Clear(void);
				void  Init_IO(int id);
				void  Init_Theater(TheaterType theater);
				bool  Recalc(void);
				bool  Factory_Link(int factory, RTTIType type, int id);
				void const * Get_Special_Cameo(int type);

				void Set_Parent_Sidebar(SidebarGlyphxClass *parent) {ParentSidebar = parent;}

				/*
				**	File I/O.
				*/
				bool  Load(FileClass & file);
				bool  Save(FileClass & file);
				//void  Code_Pointers(void);
				//void  Decode_Pointers(void);


				/*
				**	Working numbers used when rendering and processing the side strip.
				*/
				enum SideBarGeneralEnums {
					MAX_BUILDABLES = 75			// Maximum number of object types in sidebar.
				};

				SidebarGlyphxClass *ParentSidebar;


				/*
				**	This is a unique identifier for the sidebar strip. Using this identifier,
				**	it is possible to differentiate the button messages that arrive from the
				**	common input button list.  It >MUST< be equal to the strip's index into
				** the Column[] array, because the strip uses it to access the stripclass
				** buttons.
				*/
				int ID;

				/*
				**	If construction is in progress (no other objects in this strip can
				**	be started), then this flag will be true. It will be cleared when
				**	the strip is free to start production again.
				*/
				unsigned IsBuilding:1;

				/*
				**	This is the count of the number of sidebar slots that are active.
				*/
				int BuildableCount;

				/*
				**	This is the array of buildable object types. This array is sorted in the order
				**	that it is to be displayed. This array keeps track of which objects are building
				**	and ready to be placed. The very nature of this method precludes simultaneous
				**	construction of the same object type.
				*/
				typedef struct BuildType {
					int BuildableID;
					RTTIType BuildableType;
					int Factory;								// Production manager.
					bool BuildableViaCapture;							// Added for new sidebar functionality. ST - 9/24/2019 3:10PM
				} BuildType;
				BuildType Buildables[MAX_BUILDABLES];

		} Column[COLUMNS];


	private:
		int  Which_Column(RTTIType type);

		HouseClass *SidebarPlayerPtr;
};



void Sidebar_Glyphx_Init_Clear(HouseClass *player_ptr = NULL);
void Sidebar_Glyphx_Init_IO(HouseClass *player_ptr = NULL);							// Inits button list
bool Sidebar_Glyphx_Abandon_Production(RTTIType type, int factory, HouseClass *player_ptr = NULL);
bool Sidebar_Glyphx_Add(RTTIType type, int ID, HouseClass *player_ptr = NULL, bool via_capture = false);
void Sidebar_Glyphx_Recalc(HouseClass *player_ptr = NULL);
bool Sidebar_Glyphx_Factory_Link(int factory, RTTIType type, int id, HouseClass *player_ptr = NULL);
void Sidebar_Glyphx_AI(HouseClass *player_ptr, KeyNumType & input);
bool Sidebar_Glyphx_Save(Pipe &file, SidebarGlyphxClass *sidebar);
bool Sidebar_Glyphx_Load(Straw &file, SidebarGlyphxClass *sidebar);
void Sidebar_Glyphx_Code_Pointers(SidebarGlyphxClass *sidebar);
void Sidebar_Glyphx_Decode_Pointers(SidebarGlyphxClass *sidebar);





#endif //SIDEBAR_GLYPHX_H