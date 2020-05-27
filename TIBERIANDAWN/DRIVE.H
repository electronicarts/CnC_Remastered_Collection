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

/* $Header:   F:\projects\c&c\vcs\code\drive.h_v   2.19   16 Oct 1995 16:47:44   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DRIVE.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 14, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 14, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef DRIVE_H
#define DRIVE_H

#include	"foot.h"

/****************************************************************************
**	Movable objects are handled by this class definition. Moveable objects
**	cover everything except buildings.
*/
class DriveClass : public FootClass
{
	public:
		/*
		**	This points to the static control data that gives 'this' unit its characteristics.
		*/
		UnitTypeClass const * const Class;


		/*
		** Simulated sub-pixel offset to find out where the object would be if it could make a sub-pixel move. Only used by
		** GlyphX client for more accurate positioning.
		** 
		** ST - 4/30/2019 8:05AM
		*/
		short SimLeptonX;
		short SimLeptonY;

		/*
		**	This records the number of "loads" of Tiberium the unit is carrying. Only
		**	harvesters use this field.
		*/
		unsigned char Tiberium;

		/*
		**	If this unit performing harvesting action, then this flag is true. The flag
		**	is located here because the other bit flags here give it a free place to
		**	reside.
		*/
		unsigned IsHarvesting:1;

		/*
		**	This flags when a transport vehicle could not unload at its designated location
		**	and is heading off the map to try again later. When this flag is true, the
		**	transport unit is allowed to disappear when it reaches the edge of the map.
		*/
		unsigned IsReturning:1;

		/*
		**	Some units must have their turret locked down to face their body direction.
		**	When this flag is set, this condition is in effect. This flag is a more
		**	accurate check than examining the TrackNumber since the turret may be
		**	rotating into position so that a pending track may start. During this process
		**	the track number does not indicate anything.
		*/
		unsigned IsTurretLockedDown:1;

		/*
		**	This vehicle could be processing a "short track". A short track is one that
		**	doesn't actually go anywhere. Kind of like turning in place.
		*/
		unsigned IsOnShortTrack:1;

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		DriveClass(void);
		DriveClass(UnitType classid, HousesType house);
		virtual ~DriveClass(void) {};
		operator UnitType(void) const {return Class->Type;};

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		virtual int Offload_Tiberium_Bail(void);
		void Do_Turn(DirType dir);
		virtual void Approach_Target(void);
		virtual ObjectTypeClass const & Class_Of(void) const;
		virtual void Overrun_Square(CELL cell, bool threaten=true);
		virtual void Assign_Destination(TARGET target);
		virtual void Per_Cell_Process(bool center);
		virtual bool Ok_To_Move(DirType ) const;
		virtual void AI(void);
		#ifdef CHEAT_KEYS
		virtual void Debug_Dump(MonoClass *mono) const;
		#endif
		void Force_Track(int track, COORDINATE coord);
		virtual int Tiberium_Load(void) const;

		void Exit_Map(void);
		void Mark_Track(COORDINATE headto, MarkType type);
		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		/**********************************************************************
		**	These enumerations are used as working constants that exist only
		**	in the DriveClass namespace.
		*/
		enum DriveClassEnum {
			BACKUP_INTO_REFINERY=64,		// Track to backup into refinery.
			OUT_OF_REFINERY,					// Track to leave refinery.
			OUT_OF_WEAPON_FACTORY			// Track to leave weapons factory.
		};

	private:

		/****************************************************************************
		**	Smooth turning tracks are controlled by this structure and these
		**	processing bits.
		*/
		typedef enum TrackControlType : unsigned char {
			F_=0x00,		// No translation necessary?
			F_T=0x01,	// Transpose X and Y components?
			F_X=0x02,	// Reverse X component sign?
			F_Y=0x04,	// Reverse Y component sign?
			F_D=0x08		// Two cell consumption?
		} TrackControlType;
		//#define	F_S	0x10	// Is this a 90 degree turn?

		typedef struct {
			char					Track;		// Which track to use.
			char					StartTrack;	// Track when starting from stand-still.
			DirType				Facing;		// Facing when track has been completed.
			DriveClass::TrackControlType	Flag;			// List processing flag bits.
		} TurnTrackType;

		typedef struct {
			COORDINATE		Offset;		// Offset to origin coordinate.
			DirType	Facing;		// Facing (primary track).
		} TrackType;

		typedef struct {
			DriveClass::TrackType const * Track;	// Pointer to track list.
			int	Jump;		// Index where track jumping is allowed.
			int	Entry;	// Entry point if jumping to this track.
			int	Cell;		// Per cell process should occur at this index.
		} RawTrackType;

		/*
		**	These speed values are used to accumulate movement and then
		**	convert them into pixel "steps" that are then translated through
		**	the currently running track so that the unit will move.
		*/
		unsigned char SpeedAccum;

		/*
		**	This the track control logic (used for ground vehicles only). The 'Track'
		**	variable holds the track being followed (0 == not following track). The
		**	'TrackIndex' variable holds the current index into the specified track
		**	(starts at 0).
		*/
		char TrackNumber;
		char TrackIndex;

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		virtual void Fixup_Path(PathType *path);
		bool While_Moving(void);
		bool Start_Of_Move(void);
		void Lay_Track(void);
		COORDINATE Smooth_Turn(COORDINATE adj, DirType *dir);

		static TurnTrackType const TrackControl[67];
		static RawTrackType const RawTracks[13];
		static TrackType const Track13[];
		static TrackType const Track12[];
		static TrackType const Track11[];
		static TrackType const Track10[];
		static TrackType const Track9[];
		static TrackType const Track8[];
		static TrackType const Track7[];
		static TrackType const Track6[];
		static TrackType const Track5[];
		static TrackType const Track4[];
		static TrackType const Track3[];
		static TrackType const Track2[];
		static TrackType const Track1[24];
};

//PG_TO_FIX
//inline DriveClass::TrackControlType operator |(DriveClass::TrackControlType, DriveClass::TrackControlType);
//inline DriveClass::TrackControlType operator &(DriveClass::TrackControlType, DriveClass::TrackControlType);
//inline DriveClass::TrackControlType operator ~(DriveClass::TrackControlType);


#endif