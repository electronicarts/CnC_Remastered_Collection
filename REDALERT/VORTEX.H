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
 *                 Project Name : Command & Conquer - Red Alert                                *
 *                                                                                             *
 *                    File Name : VORTEX.H                                                     *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 8/12/96                                                      *
 *                                                                                             *
 *                  Last Update : August 29th, 1996 [ST]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *  Overview:                                                                                  *
 *    Definition of ChronalVortexClass. The Chronal vortex sometimes appears when the          *
 *  chronosphere is used.                                                                      *
 *                                                                                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef VORTEX_H
#define VORTEX_H


#define MAX_REMAP_SHADES 16		//Number of lookup tables required for vortex shading.
#define VORTEX_FRAMES		16		//Number of frames in one complete rotation of the vortex.



class ChronalVortexClass {

	public:

		/*
		** Constructor and destructor.
		*/
		ChronalVortexClass(void);
		~ChronalVortexClass(void);

		void Detach(TARGET target);

		/*
		** Makes the vortex appear at the specified coordinate.
		*/
		void Appear (COORDINATE coordinate);

		/*
		** Makes the vortex go away.
		*/
		void Disappear (void);

		/*
		** Call this every frame.
		*/
		void AI (void);

		/*
		** Render the vortex
		*/
		void Render (void);

		/*
		** Flags cells under the vortex to be redrawn
		*/
		void Set_Redraw (void);

		/*
		** Call whenever the theater changes to recalculate the shading lookup tables
		*/
		void Setup_Remap_Tables (TheaterType theater);

		/*
		** Functions to load and save the vortex.
		*/
		void Load(Straw &file);
		void Save(Pipe  &file);

		/*
		** Returns true of vortex is currently active.
		*/
		bool Is_Active(void) {return (Active);};

		/*
		** Makes the vortex attack the specified target. Target must be in range of the vortex.
		*/
		void Set_Target (ObjectClass *target);

		/*
		** Disables the vortex.
		*/
		void Stop(void);

		/*
		** Members to allow read access to private data
		*/
		COORDINATE Get_Position  (void) {return (Position);};
		int Get_Range  (void) {return (Range);};
		int Get_Speed  (void) {return (Speed);};
		int Get_Damage (void) {return (Damage);};

		/*
		** Members to allow write access to private data.
		*/
		void Set_Range  (int range) {Range = range;};
		void Set_Speed  (int speed) {Speed = speed;};
		void Set_Damage (int damage) {Damage = damage;};

		/*
		** Possible states the vortex can be in.
		*/
		typedef enum AnimStateType : unsigned char {
			STATE_GROW,			//Vortex has just appeared and is growing larger
			STATE_ROTATE,		//Vortex is rotating
			STATE_SHRINK		//Vortex is shrinking and about to disappear
		}AnimStateType;

	private:

		/*
		** Members for setting up the lookup tables.
		*/
		void Build_Fading_Table (PaletteClass const & palette, void * dest, int color, int frac);
		void Coordinate_Remap ( GraphicViewPortClass *inbuffer, int x, int y, int width, int height, unsigned char *remap_table);

		/*
		** Misc internal functions
		*/
		void Attack(void);
		void Zap_Target(void);
		void Movement(void);
		void Hide(void);
		void Show(void);

		/*
		** Position of the top left of the vortex
		*/
		COORDINATE		Position;

		/*
		** Direction of rotation
		*/
		int         	AnimateDir;

		/*
		** Current frame of animation
		*/
		int				AnimateFrame;

		/*
		** Animation flag. When 0 vortex will animate 1 frame.
		*/
		int				Animate;

		/*
		** State of vortex. See ENUM for info.
		*/
		AnimStateType	State;

		/*
		** Color lookup tables for shading on vortex.
		*/
		unsigned char 	VortexRemapTables [MAX_REMAP_SHADES][256];

		/*
		** Color lookup table to make the blue lightning orange.
		*/
		unsigned char	LightningRemap[256];

		/*
		** Is vortex currently active?
		*/
		int				Active 			: 1;

		/*
		** Is the vortex winding down?
		*/
		int				StartShutdown 	: 1;

		/*
		** Is the vortex about to hide from view?
		*/
		int				StartHiding		: 1;

		/*
		** Is the vortex active but hidden?
		*/
		int				Hidden			: 1;

		/*
		** Theater that lookup table is good for.
		*/
		TheaterType		Theater;

		/*
		** Last frame that vortex attacked on
		*/
		int				LastAttackFrame;

		/*
		** How many times lightning has zapped on this attack
		*/
		int				ZapFrame;

		/*
		** Ptr to object that the vortex is zapping
		*/
		TARGET	TargetObject;
//		ObjectClass		*TargetObject;

		/*
		** Distance to the target object
		*/
		int				TargetDistance;

		/*
		** Game frame that vortex hid on.
		*/
		int				HiddenFrame;

		/*
		** Direction vortex is going in.
		*/
		int				XDir;
		int				YDir;

		/*
		** Direction vortex should be going in
		*/
		int				DesiredXDir;
		int				DesiredYDir;

		/*
		** Range in cells of the vortex lightning
		*/
		int				Range;

		/*
		** Max speed in leptons per frame of the vortex.
		*/
		int				Speed;

		/*
		** Damge of vortex lightning zap.
		*/
		int				Damage;

		/*
		** Offscreen buffer to render vortex into. This is needed so we can handle clipping.
		*/
		GraphicBufferClass *RenderBuffer;
};


#endif
