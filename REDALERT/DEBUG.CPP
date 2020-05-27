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

/* $Header: /CounterStrike/DEBUG.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DEBUG.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : July 18, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Self_Regulate -- Regulates the logic timer to result in smooth animation.                 *
 *   Debug_Key -- Debug mode keyboard processing.                                              *
 *   Bench_Time -- Convert benchmark timer into descriptive string.                            *
 *   Benchmarks -- Display the performance tracking benchmarks.                                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "vortex.h"
#include	<stdarg.h>


#ifdef CHEAT_KEYS


static CDTimerClass<SystemTimerClass> DebugTimer;

int VortexFrame = -1;

/***********************************************************************************************
 * Debug_Key -- Debug mode keyboard processing.                                                *
 *                                                                                             *
 *    If debugging is enabled, then this routine will be called for every keystroke that the   *
 *    game doesn't recognize. These extra keys usually perform some debugging function.        *
 *                                                                                             *
 * INPUT:   input -- The key code that was pressed.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
void Debug_Key(unsigned input)
{
	static int map_x = -1;
	static int map_y = -1;
	static int map_width = -1;
	static int map_height = -1;

	if (!input || input & KN_BUTTON) return;

	/*
	**	Processing of normal keystrokes.
	*/
	if (Debug_Flag) {

		switch (input) {

			case KN_BACKSPACE:

				if (ChronalVortex.Is_Active()) {
					ChronalVortex.Disappear();
				} else {
					int xxxx = Get_Mouse_X() + Map.TacPixelX;
					int yyyy = Get_Mouse_Y() + Map.TacPixelY;
					CELL cell = Map.DisplayClass::Click_Cell_Calc(xxxx,yyyy);
					ChronalVortex.Appear ( Cell_Coord (cell) );
				}
				break;

#ifdef WIN32
			case KN_J:
				Debug_MotionCapture = true;
				break;

#ifdef OBSOLETE
			case KN_K:
				/*
				** time to create a screen shot using the PCX code (if it works)
				*/
				if (!Debug_MotionCapture) {
					GraphicBufferClass temp_page(	SeenBuff.Get_Width(),
															SeenBuff.Get_Height(),
															NULL,
															SeenBuff.Get_Width() * SeenBuff.Get_Height());
					CDFileClass file;
					char filename[30];

					SeenBuff.Blit(temp_page);
					for (int lp = 0; lp < 99; lp ++) {
						sprintf(filename, "scrsht%02d.pcx", lp);
						file.Set_Name(filename);
						if (!file.Is_Available()) break;
					}

					file.Cache(200000);
					Write_PCX_File(file, temp_page, & GamePalette);
					Sound_Effect(VOC_BEEP);
				}
				break;
#endif
#endif

			case KN_P:
				{
					for (SpecialWeaponType spc = SPC_FIRST; spc < SPC_COUNT; spc++) {
						PlayerPtr->SuperWeapon[spc].Enable(true, true);
						PlayerPtr->SuperWeapon[spc].Forced_Charge(true);
						Map.Add(RTTI_SPECIAL, spc);
						Map.Column[1].Flag_To_Redraw();
					}
				}
				break;

			case KN_I:
				{
					Map.Flash_Power();
					Map.Flash_Money();
				}
				break;

			case KN_O:
				{
					AircraftClass * air = new AircraftClass(AIRCRAFT_HIND, PlayerPtr->Class->House);
					if (air) {
						air->Height = 0;
						air->Unlimbo(Map.Pixel_To_Coord(Get_Mouse_X(), Get_Mouse_Y()), DIR_N);
					}
				}
				break;

			case KN_B:
				{
					AircraftClass * air = new AircraftClass(AIRCRAFT_LONGBOW, PlayerPtr->Class->House);
					if (air) {
						air->Height = 0;
						air->Unlimbo(Map.Pixel_To_Coord(Get_Mouse_X(), Get_Mouse_Y()), DIR_N);
					}
				}
				break;

			case KN_GRAVE:
				{
					WarheadType warhead = Random_Pick(WARHEAD_HE, WARHEAD_FIRE);
					COORDINATE coord = Map.Pixel_To_Coord(Get_Mouse_X(), Get_Mouse_Y());
					int damage = 1000;
					new AnimClass(Combat_Anim(damage, warhead, Map[coord].Land_Type()), coord);
					Explosion_Damage(coord, damage, NULL, warhead);
				}
				break;

			case KN_C:
				Debug_Cheat = (Debug_Cheat == false);
				PlayerPtr->IsRecalcNeeded = true;

				/*
				**	This placement might affect any prerequisite requirements for construction
				**	lists. Update the buildable options accordingly.
				*/
				if (!ScenarioInit) {
					Map.Recalc();
					for (int index = 0; index < Buildings.Count(); index++) {
						Buildings.Ptr(index)->Update_Buildables();
					}
				}
				break;

			case (int)KN_Z|(int)KN_ALT_BIT:
				if (map_x == -1) {
					map_x = Map.MapCellX;
					map_y = Map.MapCellY;
					map_width = Map.MapCellWidth;
					map_height = Map.MapCellHeight;
					Map.MapCellX = 1;
					Map.MapCellY = 1;
					Map.MapCellWidth = MAP_CELL_W-2;
					Map.MapCellHeight = MAP_CELL_H-2;
				} else {
					Map.MapCellX = map_x;
					Map.MapCellY = map_y;
					Map.MapCellWidth = map_width;
					Map.MapCellHeight = map_height;
					map_x = -1;
					map_y = -1;
					map_width = -1;
					map_height = -1;
				}
				break;

			case KN_M:
				if (Debug_Flag) {
					if (MonoClass::Is_Enabled()) {
						MonoClass::Disable();
					} else {
						MonoClass::Enable();
					}
				}
				break;

			case (int)KN_W|(int)KN_ALT_BIT:
				PlayerPtr->Flag_To_Win();
				break;

			case (int)KN_L|(int)KN_ALT_BIT:
				PlayerPtr->Flag_To_Lose();
				break;

			case KN_DELETE:
				if (CurrentObject.Count()) {
					Map.Recalc();
					//CurrentObject[0]->Detach_All();
					if (CurrentObject[0]->What_Am_I() == RTTI_BUILDING) {
						((BuildingClass *)CurrentObject[0])->Sell_Back(1);
					} else {
						ObjectClass * object = CurrentObject[0];
						object->Unselect();
						object->Limbo();
						delete object;
					}
				}
				break;

			case (int)KN_DELETE|(int)KN_SHIFT_BIT:
				if (CurrentObject.Count()) {
					Map.Recalc();
					int damage = 50;
					CurrentObject[0]->Take_Damage(damage, 0, WARHEAD_SA);
				}
				break;

			case KN_INSERT:
				if (CurrentObject.Count()) {
					Map.PendingObject = &CurrentObject[0]->Class_Of();
					if (Map.PendingObject) {
						Map.PendingHouse = CurrentObject[0]->Owner();
						Map.PendingObjectPtr = Map.PendingObject->Create_One_Of(HouseClass::As_Pointer(Map.PendingHouse));
						if (Map.PendingObjectPtr) {
							Map.Set_Cursor_Pos();
							Map.Set_Cursor_Shape(Map.PendingObject->Occupy_List());
						}
					}
				}
				break;

			case KN_LBRACKET:
			case KN_F11:
				if (MonoPage == DMONO_FIRST) {
					MonoPage = DMonoType(DMONO_COUNT-1);
				} else {
					MonoPage = DMonoType(MonoPage - 1);
				}
				DebugTimer = 0;
				break;

			case KN_RBRACKET:
			case KN_F12:
				MonoPage = DMonoType(MonoPage + 1);
				if (MonoPage == DMONO_COUNT) {
					MonoPage = DMONO_FIRST;
				}
				DebugTimer = 0;
				break;

			case KN_V:
			case KN_F3:
				Debug_Icon = (Debug_Icon == false);
				Map.Flag_To_Redraw(true);
				break;

			/*
			**	Reveal entire map to player.
			*/
//			case KN_F4:
//				if (Session.Type == GAME_NORMAL) {
//					Debug_Unshroud = (Debug_Unshroud == false);
//					Map.Flag_To_Redraw(true);
//				}
//				break;

			/*
			**	Shows sight and fire range in the form of circles emanating from the currently
			**	selected unit. The white circle is for sight range, the red circle is for
			**	fire range.
			*/
			case KN_F7:
				if (CurrentObject.Count() && CurrentObject[0]->Is_Techno()) {
					TechnoTypeClass const & ttype = (TechnoTypeClass const &)CurrentObject[0]->Class_Of();
					int sight = ((int)ttype.SightRange)<<8;
					int weapon = 0;
					if (ttype.PrimaryWeapon != NULL) weapon = ttype.PrimaryWeapon->Range;
					Set_Logic_Page(SeenBuff);
					COORDINATE center = CurrentObject[0]->Center_Coord();
					COORDINATE center2 = CurrentObject[0]->Fire_Coord(0);

					for (int r = 0; r < 255; r += 10) {
						int	x,y,x1,y1;
						DirType r1 = (DirType)r;
						DirType r2 = (DirType)((r+10) & 0xFF);

						if (Map.Coord_To_Pixel(Coord_Move(center, r1, sight), x, y)) {
							Map.Coord_To_Pixel(Coord_Move(center, r2, sight), x1, y1);
							LogicPage->Draw_Line(x, y+8, x1, y1+8, WHITE);
						}
						if (Map.Coord_To_Pixel(Coord_Move(center2, r1, weapon), x, y)) {
							Map.Coord_To_Pixel(Coord_Move(center2, r2, weapon), x1, y1);
							LogicPage->Draw_Line(x, y+8, x1, y1+8, RED);
						}
					}
				}
				break;

			case ((int)KN_F4 | (int)KN_CTRL_BIT):
				Debug_Unshroud = (Debug_Unshroud == false);
				Map.Flag_To_Redraw(true);
				break;

			default:
				break;
		}
	}
}


/***********************************************************************************************
 * Bench_Time -- Convert benchmark timer into descriptive string.                              *
 *                                                                                             *
 *    This routine will take the values of the benchmark timer specified and build a string    *
 *    that displays the average time each event consumed as well as the ranking of how much    *
 *    time that event took (total) during the tracking duration (one second?).                 *
 *                                                                                             *
 * INPUT:   btype -- The benchmark to convert to a descriptive string.                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the descriptive string of the benchmark specified.       *
 *                                                                                             *
 * WARNINGS:   The value returned is a pointer to a static buffer. As such, it is only valid   *
 *             until the next time that this routine is called.                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static char const * Bench_Time(BenchType btype)
{
	static char buffer[32];

	int rootcount = Benches[BENCH_GAME_FRAME].Count();
	if (rootcount == 0) rootcount = 1;
	int roottime = Benches[BENCH_GAME_FRAME].Value();
	int count = Benches[btype].Count();
	int time = Benches[btype].Value();
	if (count > 0 && count * time > roottime * rootcount) time = roottime / count;
	int percent = 0;
	if (roottime != 0 && rootcount != 0) {
		percent = ((count * time) * 99) / (roottime * rootcount);
	}
	if (percent > 99) percent = 99;
	sprintf(buffer, "%-2d%% %7d", percent, time);
	return(buffer);
}


/***********************************************************************************************
 * Benchmarks -- Display the performance tracking benchmarks.                                  *
 *                                                                                             *
 *    This will display the benchmarks for the various processes that are being tracked. The   *
 *    display will indicate the fraction that each process is consuming out of the entire      *
 *    process time as well as the time consumed by each individual event. The total fraction   *
 *    is useful for determing what should be optimized. The individual time is useful for      *
 *    guaging the effectiveness of optimization changes.                                       *
 *                                                                                             *
 * INPUT:   mono  -- Pointer to the monochrome screen that the display will use.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static void Benchmarks(MonoClass * mono)
{
	static bool _first = true;
	if (_first) {
		_first = false;
		mono->Clear();
		mono->Set_Cursor(0, 0);
		mono->Print(Text_String(TXT_DEBUG_PERFORMANCE));
		if (Benches == NULL) {
			mono->Set_Cursor(20, 15);
			mono->Printf(TXT_NO_PENTIUM);
		}
	}

	if (Benches != NULL) {
		mono->Set_Cursor(1, 2);mono->Printf("%s", Bench_Time(BENCH_FINDPATH));
		mono->Set_Cursor(1, 4);mono->Printf("%s", Bench_Time(BENCH_GREATEST_THREAT));
		mono->Set_Cursor(1, 6);mono->Printf("%s", Bench_Time(BENCH_AI));
		mono->Set_Cursor(1, 8);mono->Printf("%s", Bench_Time(BENCH_PCP));
		mono->Set_Cursor(1, 10);mono->Printf("%s", Bench_Time(BENCH_EVAL_OBJECT));
		mono->Set_Cursor(1, 12);mono->Printf("%s", Bench_Time(BENCH_EVAL_CELL));
		mono->Set_Cursor(1, 14);mono->Printf("%s", Bench_Time(BENCH_EVAL_WALL));
		mono->Set_Cursor(1, 16);mono->Printf("%s", Bench_Time(BENCH_MISSION));

		mono->Set_Cursor(14, 2);mono->Printf("%s", Bench_Time(BENCH_CELL));
		mono->Set_Cursor(14, 4);mono->Printf("%s", Bench_Time(BENCH_OBJECTS));
		mono->Set_Cursor(14, 6);mono->Printf("%s", Bench_Time(BENCH_ANIMS));

		mono->Set_Cursor(27, 2);mono->Printf("%s", Bench_Time(BENCH_PALETTE));

		mono->Set_Cursor(40, 2);mono->Printf("%s", Bench_Time(BENCH_GSCREEN_RENDER));
		mono->Set_Cursor(40, 4);mono->Printf("%s", Bench_Time(BENCH_SIDEBAR));
		mono->Set_Cursor(40, 6);mono->Printf("%s", Bench_Time(BENCH_RADAR));
		mono->Set_Cursor(40, 8);mono->Printf("%s", Bench_Time(BENCH_TACTICAL));
		mono->Set_Cursor(40, 10);mono->Printf("%s", Bench_Time(BENCH_POWER));
		mono->Set_Cursor(40, 12);mono->Printf("%s", Bench_Time(BENCH_SHROUD));
		mono->Set_Cursor(40, 14);mono->Printf("%s", Bench_Time(BENCH_TABS));
		mono->Set_Cursor(40, 16);mono->Printf("%s", Bench_Time(BENCH_BLIT_DISPLAY));

		mono->Set_Cursor(66, 2);mono->Printf("%7d", Benches[BENCH_RULES].Value());
		mono->Set_Cursor(66, 4);mono->Printf("%7d", Benches[BENCH_SCENARIO].Value());

		for (BenchType index = BENCH_FIRST; index < BENCH_COUNT; index++) {
			if (index != BENCH_RULES && index != BENCH_SCENARIO) Benches[index].Reset();
		}
	}
}


/***********************************************************************************************
 * Self_Regulate -- Regulates the logic timer to result in smooth animation                    *
 *                                                                                             *
 *    The self regulation process checks the number of frames displayed                        *
 *    per second and from this determines the amount of time to devote                         *
 *    to internal logic processing. By adjusting the time allotted to                          *
 *    internal processing, smooth animation can be maintained.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   In order for this routine to work properly it MUST be                           *
 *             called every display loop.                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1991 JLB : Created.                                                                 *
 *   07/05/1994 JLB : Handles new monochrome system.                                           *
 *=============================================================================================*/
#define	UPDATE_INTERVAL	TIMER_SECOND
void Self_Regulate(void)
{
	static ObjectClass * _lastobject = 0;
	static bool _first=true;

	if (DebugTimer == 0) {
		DebugTimer = UPDATE_INTERVAL;

		if (MonoClass::Is_Enabled()) {
			if (_first) {
				_first = false;
				for (DMonoType index = DMONO_FIRST; index < DMONO_COUNT; index++) {
					MonoArray[index].Clear();
				}
			}

			/*
			**	Always update the stress tracking mono display even if it
			**	currently isn't visible.
			*/
			Logic.Debug_Dump(&MonoArray[DMONO_STRESS]);

			MonoClass * mono = &MonoArray[MonoPage];
			mono->Set_Default_Attribute(MonoClass::NORMAL);
			mono->View();

			switch (MonoPage) {
				case DMONO_EVENTS:
					Benchmarks(mono);
					break;

				case DMONO_OBJECT:
					mono->Clear();

					/*
					**	Display the status of the currently selected object.
					*/
					if (CurrentObject.Count()) {
						_lastobject = CurrentObject[0];
					}
					if (_lastobject && !_lastobject->IsActive) {
						_lastobject = 0;
					}
					if (_lastobject) {
						_lastobject->Debug_Dump(mono);
					}
					break;

				case DMONO_STRESS:
#ifdef OBSOLETE
					mono->Set_Cursor(0, 20);
					mono->Printf(
						"Heap size:%10ld \r"
						"Largest:  %10ld \r"
						"Ttl Free: %10ld \r"
						"Frag:     %10ld \r",
						Heap_Size(MEM_NORMAL),
						Ram_Free(MEM_NORMAL),
						Total_Ram_Free(MEM_NORMAL),
						Total_Ram_Free(MEM_NORMAL)-Ram_Free(MEM_NORMAL)
					);
#endif
					break;

				case DMONO_HOUSE:
					mono->Clear();

					if (CurrentObject.Count()) {
						_lastobject = CurrentObject[0];
					}
					if (_lastobject && !_lastobject->IsActive) {
						_lastobject = 0;
					}
					if (_lastobject && _lastobject->Is_Techno()) {
						((TechnoClass *)_lastobject)->House->Debug_Dump(mono);
					}
					break;

				default:
					break;
			}

			mono->Set_Cursor(0, 0);
		}
	}
}
#endif
