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

/* $Header: /CounterStrike/RADAR.CPP 3     3/12/97 2:35p Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RADAR.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/15/94                                                     *
 *                                                                                             *
 *                  Last Update : September 16, 1996 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Get_Multi_Color -- Get the multi color offset number                                      *
 *   RadarClass::AI -- Processes radar input (non-tactical).                                   *
 *   RadarClass::Cell_On_Radar -- Determines if a cell is currently visible on radar.          *
 *   RadarClass::Click_Cell_Calc -- Determines what cell the pixel coordinate is over.         *
 *   RadarClass::Click_In_Radar -- Check to see if a click is in radar map                     *
 *   RadarClass::Click_In_Radar -- Converts a radar click into cell X and Y coordinate.        *
 *   RadarClass::Draw_It -- Displays the radar map of the terrain.                             *
 *   RadarClass::Draw_Names -- draws players' names on the radar map                           *
 *   RadarClass::Get_Jammed -- Fetch the current radar jammed state for the player.            *
 *   RadarClass::Init_Clear -- Sets the radar map to a known state                             *
 *   RadarClass::Is_Radar_Active -- Determines if the radar map is currently being displayed.  *
 *   RadarClass::Is_Radar_Existing -- Queries to see if radar map is available.                *
 *   RadarClass::Is_Zoomable -- Determines if the map can be zoomed.                           *
 *   RadarClass::Map_Cell -- Updates radar map when a cell becomes mapped.                     *
 *   RadarClass::One_Time -- Handles one time processing for the radar map.                    *
 *   RadarClass::Player_Names -- toggles the Player-Names mode of the radar map                *
 *   RadarClass::Plot_Radar_Pixel -- Updates the radar map with a terrain pixel.               *
 *   RadarClass::RTacticalClass::Action -- I/O function for the radar map.                     *
 *   RadarClass::RadarClass -- Default constructor for RadarClass object.                      *
 *   RadarClass::Radar_Activate -- Controls radar activation.                                  *
 *   RadarClass::Radar_Anim -- Renders current frame of radar animation                        *
 *   RadarClass::Radar_Cursor -- Adjust the position of the radar map cursor.                  *
 *   RadarClass::Radar_Pixel -- Mark a cell to be rerendered on the radar map.                 *
 *   RadarClass::Radar_Position -- Returns with the current position of the radar map.         *
 *   RadarClass::Refresh_Cells -- Intercepts refresh request and updates radar if needed       *
 *   RadarClass::Render_Infantry -- Displays objects on the radar map.                         *
 *   RadarClass::Render_Overlay -- Renders an icon for given overlay                           *
 *   RadarClass::Render_Terrain -- Render the terrain over the given cell                      *
 *   RadarClass::Set_Map_Dimensions -- Sets the tactical map dimensions.                       *
 *   RadarClass::Set_Radar_Position -- Sets the radar position to center around specified cell.*
 *   RadarClass::Set_Tactical_Position -- Called when setting the tactical display position.   *
 *   RadarClass::Set_Tactical_Position -- Called when setting the tactical display position.   *
 *   RadarClass::Set_Tactical_Position -- Sets the map's tactical position and adjusts radar to*
 *   RadarClass::Zoom_Mode(void) -- Handles toggling zoom on the map                           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


//void const * RadarClass::CoverShape;
RadarClass::RTacticalClass RadarClass::RadarButton;

void const * RadarClass::RadarAnim  = NULL;
void const * RadarClass::RadarPulse = NULL;
void const * RadarClass::RadarFrame = NULL;

static bool FullRedraw = false;

static GraphicBufferClass _IconStage(3,3);
static GraphicBufferClass _TileStage(24,24);


/***********************************************************************************************
 * RadarClass::RadarClass -- Default constructor for RadarClass object.                        *
 *                                                                                             *
 *    This default constructor merely sets the radar specific values to default settings. The  *
 *    radar must be deliberately activated in order for it to be displayed.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/16/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
RadarClass::RadarClass(void) :
	IsToRedraw(false),
	RadarCursorRedraw(false),
	IsPulseActive(false),
	RadarPulseFrame(0),
	DoesRadarExist(false),
	IsRadarActive(false),
	IsRadarActivating(false),
	IsRadarDeactivating(false),
	IsRadarJammedByPlayerMask(0U),
	SpecialRadarFrame(0),
	RadarAnimFrame(0),
	RadarX(0),
	RadarY(0),
	RadarCellWidth(0),
	RadarCellHeight(0),
	RadarCell(0),
	BaseX(0),
	BaseY(0),
	RadarWidth(0),
	RadarHeight(0),
	IsZoomed(true),
	ZoomFactor(0),
	IsPlayerNames(false),
	IsHouseSpy(false),
	SpyingOn(HOUSE_SPAIN),
	PixelPtr(0)
{
}


/***********************************************************************************************
 * RadarClass::One_Time -- Handles one time processing for the radar map.                      *
 *                                                                                             *
 *    This routine handles any one time processing required in order for the radar map to      *
 *    function. This actually only requires an allocation of the radar staging buffer. This    *
 *    buffer is needed for those cases where the radar area of the page is being destroyed     *
 *    and it needs to be destroyed.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure to call this routine only ONCE.                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::One_Time(void)
{
	RadWidth		= 80 * RESFACTOR;
	RadHeight 	= 70 * RESFACTOR;
	RadX 			= SeenBuff.Get_Width() - RadWidth;
	RadY 			= 7 * RESFACTOR;
	RadPWidth 	= 64 * RESFACTOR;
	RadPHeight 	= 64 * RESFACTOR;
	#ifdef WIN32
		RadOffX 		= 6;
		RadOffY 		= 7;
		RadIWidth 	= 128+18;//************
		RadIHeight	= 128+2;//************
	#else
		RadOffX 		= 4;
		RadOffY 		= 1;
		RadIWidth 	= 72;
		RadIHeight	= 69;
	#endif

	DisplayClass::One_Time();
#ifdef OBSOLETE
	RadarButton.X			= RadX+RadOffX;
	RadarButton.Y 			= RadY+RadOffY;
	RadarButton.Width 	= RadIWidth;
	RadarButton.Height 	= RadIHeight;
#else
	RadarButton.X			= RadX;
	RadarButton.Y 			= RadY;
	RadarButton.Width 	= RadWidth;
	RadarButton.Height 	= RadHeight;
#endif
}


/***********************************************************************************************
 * RadarClass::Init_Clear -- Sets the radar map to a known state.                              *
 *                                                                                             *
 *    This routine is used to initialize the radar map at the start of the scenario. It        *
 *    sets the radar map position and starts it in the disabled state.                         *
 *                                                                                             *
 * INPUT:   theater  -- The theater that the scenario is starting (unused by this routine).    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::Init_Clear(void)
{
	DisplayClass::Init_Clear();
	IsRadarActive			= false;
	IsToRedraw 				= true;
	RadarCursorRedraw    = true;
	IsRadarActivating 	= false;
	IsRadarDeactivating 	= false;
	DoesRadarExist 		= false;
	PixelPtr 				= 0;
	IsPlayerNames			= false;

	/*
	** If we have a valid map lets make sure that we set it correctly
	*/
	if (MapCellWidth || MapCellHeight) {
#ifdef WIN32
		IsZoomed = false;
#else
		IsZoomed = true;
#endif
		Zoom_Mode(Coord_Cell(Map.TacticalCoord));
	}
}


/***********************************************************************************************
 * RadarClass::Radar_Activate -- Controls radar activation.                                    *
 *                                                                                             *
 *    Use this routine to turn the radar map on or off.                                        *
 *                                                                                             *
 * INPUT:   control  -- What to do with the radar map:                                         *
 *                      0 = Turn radar off.                                                    *
 *                      1 = Turn radar on.                                                     *
 *                      2 = Remove Radar Gadgets                                               *
 *                      3 = Add Radar Gadgets                                                  *
 *                      4 = Remove radar.                                                      *
 *                      -1= Toggle radar on or off.                                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the radar map already on?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/11/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Radar_Activate(int control)
{
	bool old = IsRadarActive;

	switch (control) {

		/*
		** Toggle the state of the radar map on or off.
		*/
		case -1:
			{
				int temp = (IsRadarActive == false);
				if (temp) {
					Radar_Activate(1);
				} else {
					Radar_Activate(0);
				}
			}
			break;

		/*
		** Turn the radar map off properly.
		*/
		case 0:
			if (Map.IsSidebarActive) {
				if (IsRadarActive && !IsRadarDeactivating) {
					Sound_Effect(VOC_RADAR_OFF);
					IsRadarDeactivating = true;
					IsRadarActive = false;
					if (IsRadarActivating == true) {
						IsRadarActivating = false;
					} else {
						RadarAnimFrame = RADAR_ACTIVATED_FRAME;
					}
				}
			} else {
				Radar_Activate(2);
			}
			return(old);

		case 1:
			if (Map.IsSidebarActive) {
				if (!IsRadarActivating && !IsRadarActive) {
					Sound_Effect(VOC_RADAR_ON);
					IsRadarActivating = true;
					if (IsRadarDeactivating == true) {
						IsRadarDeactivating = false;
					} else {
						if (DoesRadarExist) {
							RadarAnimFrame = MAX_RADAR_FRAMES;
						} else {
							RadarAnimFrame = 0;
						}
					}
				}
			} else {
				Radar_Activate(3);
			}
			return(old);

		case 2:
			if (Session.Type==GAME_NORMAL) {
				SidebarClass::Zoom.Disable();
			} else {
				SidebarClass::Zoom.Enable();
			}
			IsRadarActive = false;
			IsRadarActivating = false;
			IsRadarDeactivating = false;
			break;

		case 3:
			if (Session.Type == GAME_NORMAL && Is_Zoomable()) {
				SidebarClass::Zoom.Enable();
			}
			IsRadarActive = true;
			IsRadarActivating = false;
			IsRadarDeactivating = false;
			break;

		case 4:
			IsRadarActive			= false;
			IsRadarActivating		= false;
			IsRadarDeactivating	= false;
			DoesRadarExist 		= false;
			Flag_To_Redraw(false);
			IsToRedraw = true;
			break;

		default:
			break;
	}

	if (IsRadarActive != old) {
		IsToRedraw = true;
		Flag_To_Redraw(false);
	}
	FullRedraw = IsRadarActive;
	return(old);
}


/***********************************************************************************************
 * RadarClass::Draw_It -- Displays the radar map of the terrain.                               *
 *                                                                                             *
 *    This is used to display the radar map that appears in the lower                          *
 *    right corner. The main changes to this map are the vehicles and                          *
 *    structure pixels.                                                                        *
 *                                                                                             *
 * INPUT:      none                                                                            *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/24/1991 JLB : Created.                                                                 *
 *   05/08/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
void RadarClass::Draw_It(bool forced)
{
	DisplayClass::Draw_It(forced);

#if (0)	                               // Legacy radar rendering not used. ST - 2/26/2020 3:53PM

	static char * _hiresradarnames[]={
		"natoradr.shp",	//HOUSE_SPAIN,
		"natoradr.shp",   //HOUSE_GREECE,
		"ussrradr.shp",   //HOUSE_USSR,
		"natoradr.shp",   //HOUSE_ENGLAND,
		"ussrradr.shp",   //HOUSE_UKRAINE,
		"natoradr.shp",   //HOUSE_GERMANY,
		"natoradr.shp",   //HOUSE_FRANCE,
		"natoradr.shp",	//HOUSE_TURKEY,
		"natoradr.shp",	//HOUSE_GOOD
		"ussrradr.shp",	//HOUSE_BAD
	};
	static char * _frames[]={
		"nradrfrm.shp",   //HOUSE_SPAIN,
		"nradrfrm.shp",   //HOUSE_GREECE,
		"uradrfrm.shp",   //HOUSE_USSR,
		"nradrfrm.shp",   //HOUSE_ENGLAND,
		"uradrfrm.shp",   //HOUSE_UKRAINE,
		"nradrfrm.shp",   //HOUSE_GERMANY,
		"nradrfrm.shp",   //HOUSE_FRANCE,
		"nradrfrm.shp",   //HOUSE_TURKEY,
		"nradrfrm.shp",	//HOUSE_GOOD
		"uradrfrm.shp",	//HOUSE_BAD
	};

	int radarforced = 0;

	/*
	**	Don't perform any rendering if none is requested.
	*/
	if (!forced && !IsToRedraw && !FullRedraw) return;

	BStart(BENCH_RADAR);

	static HousesType _house = HOUSE_NONE;

	if (PlayerPtr->ActLike != _house) {
		char name[_MAX_FNAME + _MAX_EXT];

//		strcpy(name, "NATORADR.SHP" );
//		if (Session.Type == GAME_NORMAL) {
			strcpy(name, _hiresradarnames[PlayerPtr->ActLike]);
//		}
		#ifndef NDEBUG
			RawFileClass file(name);
			if (file.Is_Available()) {
				RadarAnim = Load_Alloc_Data(file);
			} else {
				RadarAnim = MFCD::Retrieve(name);
			}
			strcpy(name, "PULSE.SHP");
			RawFileClass file2(name);
			if (file2.Is_Available()) {
				RadarPulse = Load_Alloc_Data(file2);
			} else {
				RadarPulse = MFCD::Retrieve(name);
			}
			strcpy(name, _frames[PlayerPtr->ActLike]);
			RawFileClass file3(name);
			if (file3.Is_Available()) {
				RadarFrame = Load_Alloc_Data(file3);
			} else {
				RadarFrame = MFCD::Retrieve(_frames[PlayerPtr->ActLike]);
			}
		#else
			RadarAnim = MFCD::Retrieve(name);
			strcpy(name, "PULSE.SHP");
			RawFileClass file3(name);
			if (file3.Is_Available()) {
				RadarPulse = Load_Alloc_Data(file3);
			} else {
				RadarPulse = MFCD::Retrieve(name);
			}
		RadarFrame = MFCD::Retrieve(_frames[PlayerPtr->ActLike]);
		#endif
		_house = PlayerPtr->ActLike;
	}

	/*
	** If in player name mode, just draw player names
	*/
	if (IsPlayerNames) {
		Draw_Names();
		IsToRedraw = false;
		BEnd(BENCH_RADAR);
		return;
	}

	/*
	** If in spy-on-radar facility mode, draw the appropriate info.
	*/
	if (IsHouseSpy) {
		IsToRedraw = false;
		if (Draw_House_Info()) {
			BEnd(BENCH_RADAR);
			return;
		}
	}

	if (IsRadarActivating || IsRadarDeactivating || IsRadarJammed) {
		Radar_Anim();
		Map.Repair.Draw_Me(true);
		Map.Upgrade.Draw_Me(true);
		Map.Zoom.Draw_Me(true);
		IsToRedraw = false;
		BEnd(BENCH_RADAR);
		return;
	}

	if (Map.IsSidebarActive) {
		if (IsRadarActive) {
			
			if (RunningAsDLL) {
				BEnd(BENCH_RADAR);
				return;
			}

			/*
			**	If only a few of the radar pixels need to be redrawn, then find and redraw
			**	only these.
			*/
			if (!forced && IsToRedraw && !FullRedraw && !IsPulseActive) {
				IsToRedraw = false;

				if (PixelPtr) {

					/*
					**	Render all pixels in the "to redraw" stack.
					*/
					if (LogicPage->Lock()) {
						for (int index = 0; index < (int)PixelPtr; index++) {
							CELL cell = PixelStack[index];
					  		if (Cell_On_Radar(cell)) {
								(*this)[cell].IsPlot = false;
								Plot_Radar_Pixel(cell);
								RadarCursorRedraw |= (*this)[cell].IsRadarCursor;
							}
						}
						LogicPage->Unlock();
					}

					/*
					**	Refill the stack if there is pending pixels yet to be plotted.
					**	This should only process in sections for speed reasons
					*/
					if (PixelPtr == PIXELSTACK) {
						PixelPtr = 0;

						for (int y = 0; y < MapCellHeight; y++) {
							for (int x = 0; x < MapCellWidth; x++) {
								CELL cell = XY_Cell(MapCellX + x, MapCellY + y);
							  	if (Cell_On_Radar(cell)) {

									if ((*this)[cell].IsPlot) {
										PixelStack[PixelPtr++] = cell;
										IsToRedraw = true;
										if	(PixelPtr == PIXELSTACK) break;
									}
								}
							}
							if	(PixelPtr == PIXELSTACK) break;
						}
					} else {
						PixelPtr = 0;
					}
				}

				Radar_Cursor(RadarCursorRedraw);

			} else {

#ifdef WIN32
				GraphicViewPortClass * oldpage	= Set_Logic_Page(HidPage);
#else
				GraphicBufferClass * oldpage	= Set_Logic_Page(HidPage);
#endif

				CC_Draw_Shape(RadarFrame, 1, RadX, RadY+(1 * RESFACTOR), WINDOW_MAIN, SHAPE_NORMAL);
				if (BaseX || BaseY) {

					if (!IsZoomed && BaseX && BaseY && (int)RadarWidth< (RadIWidth-1) && (int)RadarHeight < (RadIHeight-1)) {
#ifdef WIN32
						LogicPage->Draw_Rect(RadX + RadOffX + BaseX -1,
													RadY + RadOffY + BaseY -1,
													RadX + RadOffX + BaseX + RadarWidth,
//													RadX + RadOffX + BaseX + RadarWidth +1,
													RadY + RadOffY + BaseY + RadarHeight,
//													RadY + RadOffY + BaseY + RadarHeight +1,
													WHITE);
#endif
					}
				} else {
					LogicPage->Fill_Rect(	RadX + RadOffX,
													RadY + RadOffY,
													RadX + RadOffX + RadIWidth - 1,
													RadY + RadOffY + RadIHeight - 1,
													BLACK);
				}

				/*
				** Draw the entire radar map.
				*/
				if (LogicPage->Lock()) {
					for (int index = 0; index < MAP_CELL_TOTAL; index++) {
						if (In_Radar(index) && Cell_On_Radar(index)) {
							Plot_Radar_Pixel(index);
						}
					}
					if (IsPulseActive) {
						CC_Draw_Shape(RadarPulse, RadarPulseFrame++, RadX + RadOffX, RadY+1*RESFACTOR, WINDOW_MAIN, SHAPE_NORMAL);
					}
					LogicPage->Unlock();
				}

				Radar_Cursor(true);
				FullRedraw = false;
				IsToRedraw = false;

				Map.Repair.Draw_Me(true);
				Map.Upgrade.Draw_Me(true);
				Map.Zoom.Draw_Me(true);

				if (oldpage == &SeenBuff) {
					Hide_Mouse();
					LogicPage->Blit(SeenBuff, RadX, RadY, RadX, RadY, RadWidth, RadHeight);
					Show_Mouse();
				}

				Set_Logic_Page(oldpage);
			}

		} else {

			/*
			**	If the radar is not active, then only draw the cover plate if forced to do so.
			*/
			int val = (DoesRadarExist) ?  MAX_RADAR_FRAMES : 0;
			CC_Draw_Shape(RadarAnim, val, RadX, RadY + (1 * RESFACTOR), WINDOW_MAIN, SHAPE_NORMAL);
			FullRedraw = false;
			IsToRedraw = false;

			if (RunningAsDLL) {
				BEnd(BENCH_RADAR);
				return;
			}

			/*
			**	Display the country name on the cover plate when in multi play only.
			*/
			if (Session.Type != GAME_NORMAL) {
				Fancy_Text_Print(Text_String(HouseTypeClass::As_Reference(PlayerPtr->ActLike).Full_Name()), RadX+RadWidth/2, RadY+RadHeight-10*RESFACTOR, &ColorRemaps[PlayerPtr->RemapColor], TBLACK, TPF_CENTER|TPF_TEXT|TPF_DROPSHADOW);
			}

			Map.Repair.Draw_Me(true);
			Map.Upgrade.Draw_Me(true);
			Map.Zoom.Draw_Me(true);

		}

	}
	BEnd(BENCH_RADAR);
#endif
}


/***************************************************************************
 * RadarClass::Render_Terrain -- Render the terrain over the given cell    *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/12/1995 PWG : Created.                                             *
 *=========================================================================*/
void RadarClass::Render_Terrain(CELL cell, int x, int y, int size)
{
	TerrainClass	* list[4] = {0,0,0,0};
	int				listidx = 0;
	int				lp,lp2;


	ObjectClass * obj = Map[cell].Cell_Occupier();

	/*
	** If the cell is occupied by a terrain type, add it to the sortable
	** list.
	*/
	if (obj && obj->What_Am_I() == RTTI_TERRAIN)
		list[listidx++] = (TerrainClass *)obj;

	/*
	** Now loop through all the occupiers and add them to the list if they
	** are terrain type.
	*/
	for (lp = 0; lp < ARRAY_SIZE(Map[cell].Overlapper); lp ++) {
		obj = Map[cell].Overlapper[lp];
		if (obj && obj->What_Am_I() == RTTI_TERRAIN)
			list[listidx++] = (TerrainClass *)obj;
	}

	/*
	** If there are no entries in our list then just get out.
	*/
	if (!listidx) return;

	/*
	**	If there is terrain in this cell then draw a dark pixel to
	** represent it.
	*/
	if (size == 1) {
		LogicPage->Put_Pixel(x, y, 21);
//		LogicPage->Put_Pixel(x, y, 60);
		return;
	}

	/*
	** Sort the list by its sort Y value so that we can render in the proper
	** order.
	*/
	for (lp = 0; lp < listidx - 1; lp ++) {
		for (lp2 = lp + 1; lp2 < listidx; lp2++) {
			if (list[lp]->Sort_Y() > list[lp2]->Sort_Y()) {
				TerrainClass * terrain = list[lp];
				list[lp] = list[lp2];
				list[lp2] = terrain;
			}
		}
	}

	/*
	** loop through the list and take care of rendering the correct icon.
	*/
	for (lp = 0; lp < listidx; lp ++) {
		unsigned char * icon = list[lp]->Radar_Icon(cell);
		if (!icon) continue;
#ifdef WIN32
		Buffer_To_Page(0, 0, 3, 3, icon, _IconStage);
		_IconStage.Scale(*LogicPage, 0, 0, x, y, 3, 3, ZoomFactor, ZoomFactor, TRUE, (char *)&FadingBrighten[0]);
#else
		for (int lpy = 0; lpy < 3; lpy++) {
			for (int lpx = 0; lpx < 3; lpx++) {
				if (*icon) {
					LogicPage->Put_Pixel(x + lpx, y + lpy, FadingBrighten[*icon]);
				}
				icon++;
			}
		}
#endif	//WIN32
	}
}


/***********************************************************************************************
 * RadarClass::Render_Infantry -- Displays objects on the radar map.                           *
 *                                                                                             *
 *    This routine will display an object imagery at the location specified according to the   *
 *    condition of the specified cell.                                                         *
 *                                                                                             *
 * INPUT:   cell  -- The cell to use as reference when drawing the radar pixel.                *
 *                                                                                             *
 *          x,y   -- The pixel coordinate to render the radar "pixel" at.                      *
 *                                                                                             *
 *          size  -- The size of the "pixel". When zoomed in, this value will be "3".          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/17/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::Render_Infantry(CELL cell, int x, int y, int size)
{
	ObjectClass * obj;

	obj = (ObjectClass *)Map[cell].Cell_Occupier();
	while (obj) {
		if (obj->Is_Techno() && ((TechnoClass *)obj)->Is_Visible_On_Radar()) {
			int color = ColorRemaps[((InfantryClass *)obj)->House->RemapColor].Bar;
//			int color = ColorRemaps[((InfantryClass *)obj)->House->RemapColor].BrightColor;
			int xoff;
			int yoff;
			int subsize = max(1, size/3);

			switch (obj->What_Am_I()) {
				case RTTI_INFANTRY:
					xoff = (Coord_XLepton(obj->Coord) / (CELL_LEPTON_W/(size+1))) - subsize/2;
					xoff = max(xoff, 0);
					xoff = min(xoff, size-subsize);
					yoff = (Coord_YLepton(obj->Coord) / (CELL_LEPTON_H/(size+1))) - subsize/2;
					yoff = max(yoff, 0);
					yoff = min(yoff, size-subsize);

					/*
					** Draw the infantryman's pixel.  If he's a spy, draw in my house color
					*/
					if (*(InfantryClass *)obj == INFANTRY_SPY) {
						color = ColorRemaps[PlayerPtr->RemapColor].Bar;
//						color = ColorRemaps[PlayerPtr->RemapColor].BrightColor;
					}
					LogicPage->Fill_Rect(x+xoff, y+yoff, x+xoff+(subsize-1), y+yoff+(subsize-1), color);
					break;

				case RTTI_UNIT:
				case RTTI_VESSEL:
				case RTTI_AIRCRAFT:
					LogicPage->Fill_Rect(x, y, x+size-1, y+size-1, color);
					break;

				default:
					break;
			}
		}
		obj = obj->Next;
	}

}


/***************************************************************************
 * RadarClass::Render_Overlay -- Renders an icon for given overlay         *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/18/1995 PWG : Created.                                             *
 *=========================================================================*/
void RadarClass::Render_Overlay(CELL cell, int x, int y, int size)
{
	//int lpx,lpy;

	OverlayType overlay = (*this)[cell].Overlay;
 	if (overlay != OVERLAY_NONE) {
		OverlayTypeClass const * otype = &OverlayTypeClass::As_Reference(overlay);

		if (otype->IsRadarVisible) {
			unsigned char * icon = otype->Radar_Icon((*this)[cell].OverlayData);
			if (!icon) return;
#ifdef WIN32
			Buffer_To_Page(0, 0, 3, 3, icon, _IconStage);
			if (otype->IsTiberium) {
				if (size == 1) {
					LogicPage->Put_Pixel(x, y, DKGREY);

//					_IconStage.Scale(*LogicPage, 0, 0, x, y, 3, 3, size, size, TRUE, (char *)&FadingShade[0]);
				} else {
					_IconStage.Scale(*LogicPage, 0, 0, x, y, 3, 3, size, size, TRUE, (char *)&FadingYellow[0]);
				}
//				_IconStage.Scale(*LogicPage, 0, 0, x, y, 3, 3, size, size, TRUE, (char *)&FadingGreen[0]);
//			} else {
//				_IconStage.Scale(*LogicPage, 0, 0, x, y, 3, 3, size, size, TRUE, (char *)&FadingBrighten[0]);
			}

#else
			for (int lpy = 0; lpy < size; lpy++) {
				for (int lpx = 0; lpx < size; lpx++) {
					if (size == 1) icon+=4;
					if (*icon) {
						if (otype->IsTiberium) {
							if (size == 1) {
								LogicPage->Put_Pixel(x + lpx, y + lpy, DKGREY);
//								LogicPage->Put_Pixel(x + lpx, y + lpy, FadingShade[*icon]);
							} else {
								LogicPage->Put_Pixel(x + lpx, y + lpy, FadingYellow[*icon]);
							}
//							LogicPage->Put_Pixel(x + lpx, y + lpy, FadingGreen[*icon]);
//						} else {
//							LogicPage->Put_Pixel(x + lpx, y + lpy, FadingBrighten[*icon]);
						}
					}
					if (size == 1) {
						icon+=5;
					} else {
						icon++;
					}
					icon++;
				}
			}
#endif	//WIN32
		}
	}
}


/***************************************************************************
 * RadarClass::Zoom_Mode -- Handles toggling zoom on the map               *
 *                                                                         *
 * INPUT:      none                                                        *
 *                                                                         *
 * OUTPUT:  	none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/29/1995 PWG : Created.                                             *
 *=========================================================================*/
void RadarClass::Zoom_Mode(CELL cell)
{
	int map_c_width;
	int map_c_height;

	/*
	** Set all of the initial zoom mode variables to the correct
	** setting.
	*/
#ifdef WIN32
	if (Is_Zoomable()) {
		IsZoomed				= !IsZoomed;
	} else {
		IsZoomed = true;
	}
#else
	IsZoomed				= false;
#endif
	BaseX					= 0;
	BaseY					= 0;

	/*
	** Figure out exactly what size we need to zoom the map to.
	*/
	if (!IsZoomed) {
		int xfactor			= RadIWidth / MapCellWidth;
		int yfactor			= RadIHeight / MapCellHeight;
		ZoomFactor			= max(min(xfactor, yfactor) , 1);
		map_c_width			= MapCellWidth;
		map_c_height      = MapCellHeight;
	} else {
		ZoomFactor			= 3;
//		ZoomFactor			= 6;
		map_c_width			= RadIWidth / ZoomFactor;
		map_c_height		= RadIHeight / ZoomFactor;
	}

	/*
	** Make sure we do not show more cells than are on the map.
	*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
#ifdef WIN32
	map_c_width			= min(map_c_width, RadIWidth);
	map_c_width			= min(map_c_width, MapCellWidth);
	map_c_height		= min(map_c_height, RadIHeight);
	map_c_height		= min(map_c_height, MapCellHeight);
#else
	map_c_width			= min(map_c_width, 62 * RESFACTOR);
	map_c_width			= min(map_c_width, MapCellWidth);
	map_c_height		= min(map_c_height, 62 * RESFACTOR);
	map_c_height		= min(map_c_height, MapCellHeight);
#endif
#else
	map_c_width			= min(map_c_width, 62 * RESFACTOR);
	map_c_width			= min(map_c_width, MapCellWidth);
	map_c_height		= min(map_c_height, 62 * RESFACTOR);
	map_c_height		= min(map_c_height, MapCellHeight);
#endif

	/*
	** Find the amount of remainder because this will let us calculate
	** how to center the thing.
	*/
	int rem_x 			= RadIWidth - (map_c_width * ZoomFactor);
	int rem_y 			= RadIHeight - (map_c_height * ZoomFactor);

	/*
	** Finally mark the map so it shows just as much as it is supposed
	** to.
	*/
	BaseX					= rem_x / 2;
	BaseY					= rem_y / 2;
	RadarCellWidth 	= map_c_width;
	RadarCellHeight 	= map_c_height;
	RadarWidth			= RadIWidth - rem_x;
	RadarHeight			= RadIHeight - rem_y;

	/*
	** Set the radar position to the current cell.
	*/
	Set_Radar_Position(cell);

	/*
	** When zoom mode changes then we need to redraw the radar
	** area.
	*/
	IsToRedraw = true;

	/*
	** Notify the map that we need to redraw a portion
	*/
	Flag_To_Redraw(false);

	/*
	** Since we have made a vast change we must redraw everything
	*/
	FullRedraw = true;
}


/***********************************************************************************************
 * RadarClass::Is_Zoomable -- Determines if the map can be zoomed.                             *
 *                                                                                             *
 *    This will check to see if the zoomed mode of the map would be just the same size as      *
 *    the non-zoomed mode. If this is true, then zooming would have no effect, so return       *
 *    false indicating that zooming is not allowed.                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is zooming allowed?                                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Is_Zoomable(void) const
{
	int xfactor = RadIWidth / MapCellWidth;
	int yfactor = RadIHeight / MapCellHeight;
	int factor = max(min(xfactor, yfactor) , 1);
	if (factor == 3) {
		return(false);
	}
	return(true);
}


/***********************************************************************************************
 * RadarClass::Plot_Radar_Pixel -- Updates the radar map with a terrain pixel.                 *
 *                                                                                             *
 *    This will update the radar map with a pixel. It is used to display                       *
 *    vehicle positions on the radar map.                                                      *
 *                                                                                             *
 * INPUT:   unit  -- Pointer to unit to render at the given position. If                       *
 *                   NULL is passed in, then the underlying terrain is                         *
 *                   displayed instead.                                                        *
 *                                                                                             *
 *          pos   -- Position on the map to update.                                            *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   This routine does NOT hide the mouse. It is up to you to                        *
 *             do so.                                                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1991 JLB : Created.                                                                 *
 *   06/21/1991 JLB : Large blips for units & buildings.                                       *
 *   02/14/1994 JLB : Revamped.                                                                *
 *   04/17/1995 PWG : Created.                                                                 *
 *   04/18/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::Plot_Radar_Pixel(CELL cell)
{
	if (cell == -1) cell = 1;

	int	x,y;					// Coordinate of cell location.

	/*
	**	Perform any clipping on the cell coordinate.
	*/
	if (!IsRadarActive || (unsigned)cell > MAP_CELL_TOTAL) {
		return;
	}

	if (!In_Radar(cell) || !Cell_On_Radar(cell)) {
		return;
	}

	/*
	** If we are zoomed in then calculate the pixel based off of the portion
	** of the map the radar is viewing.
	*/
	x = Cell_X(cell) - RadarX;
	y = Cell_Y(cell) - RadarY;
	if ((unsigned)x >= RadarCellWidth || (unsigned)y >= RadarCellHeight) {
		return;
	}

	bool usjamming = false;
	if (LogicPage->Lock()) {
		CellClass * cellptr = &(*this)[cell];
 		x = RadX + RadOffX + BaseX + (x * ZoomFactor);
 		y = RadY + RadOffY + BaseY + (y * ZoomFactor);

 		/*
 		**	Determine what (if any) vehicle or unit should be rendered in this blip.
 		*/
		int color=TBLACK;		// Color of the pixel to plot.
		int housebit = (1 << PlayerPtr->Class->House);
		int celljammed = (*this)[cell].Jammed;
		int jammed = celljammed & (0xFFFF - housebit);
 		if (!jammed && ((*this)[cell].IsMapped || Debug_Unshroud)) {
// 		if (!jammed && ((*this)[cell].IsVisible || Debug_Unshroud)) {
 			color = cellptr->Cell_Color(true);
			if ( (celljammed & housebit) && (color == TBLACK) ) {
				color = BLACK;//FadingWayDark[color];
				usjamming = true;
			}
 		} else  {
 			color = BLACK;
 		}

 		/*
 		**	If no color override occurs for this cell, then render the underlying
 		**	terrain.
 		*/
		if (color == TBLACK) {
			if (ZoomFactor > 1) {
				void const * ptr = NULL;
				int icon;

				/*
				**	Fetch the template pointer and template icon number for the
				**	specified cell.
				*/
				if (cellptr->TType != TEMPLATE_NONE && cellptr->TType != 255) {
					ptr = TemplateTypeClass::As_Reference(cellptr->TType).Get_Image_Data();
					icon = cellptr->TIcon;
				}

				/*
				**	If the template pointer is still NULL, then this means either a clear
				**	template or an illegal one. Setup for a clear template.
				*/
				if (ptr == NULL) {
					ptr = TemplateTypeClass::As_Reference(TEMPLATE_CLEAR1).Get_Image_Data();
					icon = cellptr->Clear_Icon();
				}

				IconsetClass const * iconset = (IconsetClass const *)ptr;
				unsigned char const * icondata = iconset->Icon_Data();


				/*
				**	Convert the logical icon number into the actual icon number.
				*/
				icon &= 0x00FF;
				icon = *(iconset->Map_Data() + icon);

				unsigned char * data = (unsigned char *)icondata + icon*(24*24);
				Buffer_To_Page(0, 0, 24, 24, data, _TileStage);
				_TileStage.Scale(*LogicPage, 0, 0, x, y, 24, 24, ZoomFactor, ZoomFactor, TRUE);
			} else {
//				LogicPage->Fill_Rect(x, y, x+ZoomFactor-1, y+ZoomFactor-1, cellptr->Cell_Color(false));
/*BG*/		LogicPage->Put_Pixel(x, y, cellptr->Cell_Color(false));
			}
		} else {
			LogicPage->Fill_Rect(x, y, x+ZoomFactor-1, y+ZoomFactor-1, color);
///*BG*/		LogicPage->Put_Pixel(x, y, color);
		}
		if (color != BLACK) {
			Render_Overlay(cell, x, y, ZoomFactor);
	 		Render_Terrain(cell, x, y, ZoomFactor);
			Render_Infantry(cell, x, y, ZoomFactor);
		} else {
			if(usjamming) {
				Render_Infantry(cell, x, y, ZoomFactor);
			}
		}
		LogicPage->Unlock();
	}
}


/***********************************************************************************************
 * RadarClass::Radar_Pixel -- Mark a cell to be rerendered on the radar map.                   *
 *                                                                                             *
 *    This routine is used to inform the system that a pixel needs to be                       *
 *    rerendered on the radar map. The pixel(s) will be rendered the                           *
 *    next time the map is refreshed.                                                          *
 *                                                                                             *
 * INPUT:   cell  -- The map cell to be rerendered.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/12/1992 JLB : Created.                                                                 *
 *   05/08/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
void RadarClass::Radar_Pixel(CELL cell)
{
	if (IsRadarActive && Map.IsSidebarActive && Cell_On_Radar(cell)) {
		IsToRedraw = true;
		(*this)[cell].IsPlot = true;
		if (PixelPtr < PIXELSTACK) {
			PixelStack[PixelPtr++] = cell;
		}
	}
}


/***********************************************************************************************
 * RadarClass::Click_In_Radar -- Converts a radar click into cell X and Y coordinate.          *
 *                                                                                             *
 *    This routine will examine the X and Y coordinate and convert them into the X and Y       *
 *    cell coordinate value that corresponds to the location.                                  *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y mouse coordinate already normalized to the radar upper left   *
 *                   corner.                                                                   *
 *                                                                                             *
 * OUTPUT:  Returns with success rating in addition, the X and Y values will now hold the      *
 *          cell coordinates of the cell the pixel offsets indicated.                          *
 *             Result 1 = click was in radar region                                            *
 *             Result 0 = click was outside radar region completely                            *
 *             Result-1 = click in radar area but not on clickable region of radar.            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/30/1995 PWG : Created.                                                                 *
 *   07/16/1995 JLB : Recognizes when sidebar is closed now.                                   *
 *=============================================================================================*/
int RadarClass::Click_In_Radar(int &ptr_x, int &ptr_y, bool change) const
{
	int x	= ptr_x;
	int y	= ptr_y;

	/*
	** If radar is not active the click could have been on a radar point
	*/
	if (!IsRadarActive || !Map.IsSidebarActive) return(0);

	x -= (RadX + RadOffX);
	y -= (RadY + RadOffY);
	if (x < RadIWidth && y < RadIHeight) {
		x -= BaseX;
		y -= BaseY;

		if ((unsigned)x < RadarWidth + (ZoomFactor-1) && (unsigned)y < RadarHeight + (ZoomFactor-1)) {
//		if ((unsigned)x < RadarWidth && (unsigned)y < RadarHeight) {
			x = RadarX + (x / ZoomFactor);
			y = RadarY + (y / ZoomFactor);
			if (change) {
				ptr_x = x;
				ptr_y = y;
			}
			return(1);
		}
		return(-1);
	}
	return(0);
}


/***********************************************************************************************
 * RadarClass::Click_Cell_Calc -- Determines what cell the pixel coordinate is over.           *
 *                                                                                             *
 *    This routine will examine the pixel coordinate provided and determine what cell it       *
 *    represents. If the radar map is not active or the coordinates are not positioned over    *
 *    the radar map, then it will fall into the base class corresponding routine.              *
 *                                                                                             *
 * INPUT:   x,y   -- The pixel coordinate to convert into a cell number.                       *
 *                                                                                             *
 * OUTPUT:  Returns with the cell number that the coordinate is over or -1 if not over any     *
 *          cell.                                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL RadarClass::Click_Cell_Calc(int x, int y) const
{
	int result = Click_In_Radar(x, y, true);
	switch (result) {
		case 1:
			return(XY_Cell(x, y));

		case -1:
			return(-1);

		default:
			break;
	}
	return(DisplayClass::Click_Cell_Calc(x, y));
}


/***********************************************************************************************
 * RadarClass::Map_Cell -- Updates radar map when a cell becomes mapped.                       *
 *                                                                                             *
 *    This routine will update the radar map if a cell becomes mapped.                         *
 *                                                                                             *
 * INPUT:   cell  -- The cell that is being mapped.                                            *
 *                                                                                             *
 *          house -- The house that is doing the mapping.                                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the cell mapped (for the first time) by this routine?                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Map_Cell(CELL cell, HouseClass * house, bool check_radar_spied, bool and_for_allies)
{
	if (DisplayClass::Map_Cell(cell, house, check_radar_spied, and_for_allies)) {
		Radar_Pixel(cell);
		return(true);
	}
	return(false);
}



void RadarClass::Cursor_Cell(CELL cell, int value)
{
	/*
	** If this cell is not on the radar don't bother doing anything.
	*/
	if (Cell_On_Radar(cell)) {

		int temp = (*this)[cell].IsRadarCursor;

		if (temp != value) {

			/*
			**	Record the new state of this cell.
			*/
			(*this)[cell].IsRadarCursor	= value;

			/*
			**	If we are erasing then erase the cell.
			*/
			if (value == FALSE) 	{
				Plot_Radar_Pixel(cell);
			}
		}
	}
}


void RadarClass::Mark_Radar(int x1, int y1, int x2, int y2, int value, int barlen)
{
	int x, y;
	/*
	** First step is to convert pixel coordinates back to a CellX and CellY.
	*/
	x1 = RadarX + (x1 / ZoomFactor);
	y1 = RadarY + (y1 / ZoomFactor);
	x2 = RadarX + (x2 / ZoomFactor);
	y2 = RadarY + (y2 / ZoomFactor);

	/*
	** Now we need to convert the Pixel length to a cell length.
	*/
	barlen = (barlen / ZoomFactor) + 1;

	/*
	** Now lets loop through and mark the map with the proper value.
	*/
	for (int lp = 0; lp <= barlen; lp++) {
		/*
		** Do Horizontal action to upper and lower left corners.
		*/
		x = x1 + lp;
		Cursor_Cell(XY_Cell(x, y1), value);
		Cursor_Cell(XY_Cell(x, y2), value);
		/*
		** Do Horizontal Action to upper and lower right corners
		*/
		x = x2 - lp;
		Cursor_Cell(XY_Cell(x, y1), value);
		Cursor_Cell(XY_Cell(x, y2), value);
		/*
		** Do Vertical Action to left and right upper corners
		*/
		y = y1 + lp;
		Cursor_Cell(XY_Cell(x1, y), value);
		Cursor_Cell(XY_Cell(x2, y), value);

		/*
		** Do Vertical action to left and right lower corners.
		*/
		y = y2 - lp;
		Cursor_Cell(XY_Cell(x1, y), value);
		Cursor_Cell(XY_Cell(x2, y), value);
	}
}



/***********************************************************************************************
 * RadarClass::Cell_XY_To_Radar_Pixel-- Adjust the position of the radar map cursor.           *
 *                                                                                             *
 *    This routine will adjust the location (and visibility) of the radar                      *
 *    map cursor. It handles all restoration, drawing, and flashing.                           *
 *                                                                                             *
 * INPUT:   pos   - Cell position for the cursor. If the value is -1 then                      *
 *                  the cursor will be hidden. If the value is equal to                        *
 *                  the last value passed in then cursor flashing will                         *
 *                  be maintained.                                                             *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1991 JLB : Created.                                                                 *
 *   11/17/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::Cell_XY_To_Radar_Pixel(int cellx, int celly, int &x, int &y)
{
	x = (cellx - RadarX) * ZoomFactor;
	y = (celly - RadarY) * ZoomFactor;
}


/***********************************************************************************************
 * RadarClass::Jam_Cell -- Updates radar map when a cell becomes jammed.                       *
 *                                                                                             *
 *    This routine will update the radar map if a cell becomes jammed.                         *
 *                                                                                             *
 * INPUT:   cell  -- The cell that is being jammed.                                            *
 *                                                                                             *
 *          house -- The house that is doing the jamming.                                      *
 *                                                                                             *
 * OUTPUT:  																											  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/09/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Jam_Cell(CELL cell, HouseClass * house/*KO, bool shadeit*/)
{
	unsigned short jam = 1 << house->Class->House;
	(*this)[cell].Jammed |= jam;
	
	/*
	** Updated for client/server multiplayer. ST - 8/12/2019 11:00AM
	*/
	if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {
		if (house != PlayerPtr) Shroud_Cell(cell, PlayerPtr/*KO, shadeit*/);
	
	} else {
		
		for (int i = 0; i < Session.Players.Count(); i++) {
			HouseClass *player_house = HouseClass::As_Pointer(Session.Players[i]->Player.ID);
			if (player_house->IsHuman && player_house != house) {
				Shroud_Cell(cell, player_house);
			}
		}
	}	

	Radar_Pixel(cell);
	return(true);
}


/***********************************************************************************************
 * RadarClass::UnJam_Cell -- Updates radar map when a cell becomes jammed.                     *
 *                                                                                             *
 *    This routine will update the radar map if a cell becomes jammed.                         *
 *                                                                                             *
 * INPUT:   cell  -- The cell that is being jammed.                                            *
 *                                                                                             *
 *          house -- The house that is doing the jamming.                                      *
 *                                                                                             *
 * OUTPUT:  																											  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/09/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::UnJam_Cell(CELL cell, HouseClass * house)
{
	unsigned short jam = 1 << house->Class->House;
	(*this)[cell].Redraw_Objects();
	(*this)[cell].Jammed &= (0xFFFF - jam);
	Radar_Pixel(cell);
	return(true);
}


/***********************************************************************************************
 * RadarClass::Radar_Cursor -- Adjust the position of the radar map cursor.                    *
 *                                                                                             *
 *    This routine will adjust the location (and visibility) of the radar                      *
 *    map cursor. It handles all restoration, drawing, and flashing.                           *
 *                                                                                             *
 * INPUT:   pos   - Cell position for the cursor. If the value is -1 then                      *
 *                  the cursor will be hidden. If the value is equal to                        *
 *                  the last value passed in then cursor flashing will                         *
 *                  be maintained.                                                             *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1991 JLB : Created.                                                                 *
 *   11/17/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
//#pragma argsused
void RadarClass::Radar_Cursor(int forced)
{
	static 					int _last_pos = -1;
	static 					int _last_frame = -1;
#ifdef WIN32
	GraphicViewPortClass 	* oldpage;
#else
	GraphicBufferClass 	* oldpage;
#endif
	int						x1, y1, x2, y2;

	/*
	** figure out these function calls as we will need to call them multiple times.
	*/
	int	tac_cell    = Coord_Cell(TacticalCoord);
	int	tac_cell_x	= Cell_X(tac_cell);
	int	tac_cell_y	= Cell_Y(tac_cell);
	int	barlen      = 6;

	/*
	** If the current tactical cell is invalid or we haven't moved and we are not forced to redraw then
	** just skip the redraw process.
	*/
	if (tac_cell != -1 && _last_pos == tac_cell && _last_frame == SpecialRadarFrame && !forced) return;

	if ( _last_pos != -1 ) {
		/*
		** The first thing we need to do is take care of erasing the last radar cell position.  We do this
		** by converting to pixel coordinates, then adjusting for the pixel coords for the current frame and
		** finally taking care of calling the erase procedure which will convert the pixel coordinates back
		** to the cells that need to be redraw.
		**/
		int last_cell_x	= Cell_X(_last_pos);
		int last_cell_y	= Cell_Y(_last_pos);

		Cell_XY_To_Radar_Pixel(last_cell_x, last_cell_y, x1, y1);
		Cell_XY_To_Radar_Pixel(last_cell_x + Lepton_To_Cell(TacLeptonWidth), last_cell_y + Lepton_To_Cell(TacLeptonHeight), x2, y2);
		x2--;
		y2--;

		/*
		** Adjust the current coordinates based on the last animation frame.
		*/
		x1-= _last_frame;
		y1-= _last_frame;
		x2+= _last_frame;
		y2+= _last_frame;

		/*
		** Finally mark the map (actually remove the marks that indicate the radar cursor was there
		*/
		Mark_Radar(x1, y1, x2, y2, FALSE, barlen);
	}


	/*
	** Find the upper left and lower right corners of the radar cursor.
	** Remember to adjust x2 and y2 back by one pixel as they will not be
	** pointing to the right value otherwise.  They point one cell ahead
	** of where they should.
	*/
	Cell_XY_To_Radar_Pixel(tac_cell_x, tac_cell_y, x1, y1);
	Cell_XY_To_Radar_Pixel(tac_cell_x + Lepton_To_Cell(TacLeptonWidth), tac_cell_y + Lepton_To_Cell(TacLeptonHeight), x2, y2);
	x2--;
	y2--;

	/*
	** Adjust the coordinates based on the current frame of radar animation.
	*/
	x1-= SpecialRadarFrame;
	y1-= SpecialRadarFrame;
	x2+= SpecialRadarFrame;
	y2+= SpecialRadarFrame;

	Mark_Radar(x1, y1, x2, y2, TRUE, barlen);

	/*
	** setup a graphic view port class so we can write all the pixels relative
	** to 0,0 rather than relative to full screen coordinates.
	*/
	oldpage = Set_Logic_Page(HidPage);
	GraphicViewPortClass draw_window(LogicPage->Get_Graphic_Buffer(),
												RadX + RadOffX + BaseX + LogicPage->Get_XPos(),
												RadY + RadOffY + BaseY + LogicPage->Get_YPos(),
												RadarWidth,
												RadarHeight);

	draw_window.Draw_Line(x1, 	y1,	x1 + barlen,	y1, 	LTGREEN);
	draw_window.Draw_Line(x1,	y1,	x1,	y1 + barlen,	LTGREEN);

	// Draw upper right hand corner
	draw_window.Draw_Line(x2 - barlen,	y1,	x2,	y1, 	LTGREEN);
	draw_window.Draw_Line(x2, 	y1, 	x2, 	y1 + barlen,	LTGREEN);

	// Draw lower left hand corner
	draw_window.Draw_Line(x1,	y2 - barlen,	x1,	y2,	LTGREEN);
	draw_window.Draw_Line(x1,	y2,	x1 + barlen,	y2, 	LTGREEN);

	// Draw lower right hand corner
	draw_window.Draw_Line(x2,	y2 - barlen,	x2, 	y2, 	LTGREEN);
	draw_window.Draw_Line(x2 - barlen, 	y2,	x2,	y2, 	LTGREEN);

	Set_Logic_Page(oldpage);
	_last_pos = tac_cell;
	_last_frame = SpecialRadarFrame;
	RadarCursorRedraw = FALSE;
}


/***************************************************************************
 * RadarClass::Radar_Anim -- Renders current frame of radar animation      *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/19/1995 PWG : Created.                                             *
 *=========================================================================*/
void RadarClass::Radar_Anim(void)
{
	/*
	** Do nothing if we're in player-name mode
	*/
	if (IsPlayerNames)
		return;

	if (!Map.IsSidebarActive) return;

#ifdef WIN32
	GraphicViewPortClass 	* oldpage= Set_Logic_Page(HidPage);
	GraphicViewPortClass draw_window(LogicPage->Get_Graphic_Buffer(),
												RadX + RadOffX + LogicPage->Get_XPos(),
												RadY + RadOffY + LogicPage->Get_YPos(),
												RadIWidth,
												RadIHeight);
#else
	GraphicBufferClass 	* oldpage= Set_Logic_Page(HidPage);
	GraphicViewPortClass draw_window(LogicPage,
												RadX + RadOffX,
												RadY + RadOffY,
												RadIWidth,
												RadIHeight-2);
#endif
//Mono_Set_Cursor(0,0);
#ifdef WIN32
	Draw_Box(RadX+RadOffX-1, RadY+RadOffY-1, RadIWidth+2, RadIHeight+2, BOXSTYLE_RAISED, true);
#endif
	draw_window.Clear();
	CC_Draw_Shape(RadarAnim, RadarAnimFrame, RadX, RadY+1*RESFACTOR, WINDOW_MAIN, SHAPE_NORMAL);
	Flag_To_Redraw(false);
	Set_Logic_Page(oldpage);
}


/***********************************************************************************************
 * RadarClass::AI -- Processes radar input (non-tactical).                                     *
 *                                                                                             *
 *    This routine intercepts any player input that concerns the radar map, but not those      *
 *    areas that represent the tactical map. These are handled by the tactical map AI          *
 *    processor. Primarily, this routine handles the little buttons that border the radar      *
 *    map.                                                                                     *
 *                                                                                             *
 * INPUT:   input -- The player input code.                                                    *
 *                                                                                             *
 *          x,y   -- Mouse coordinate parameters to use.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *   12/26/1994 JLB : Moves tactical map with click or drag.                                   *
 *   12/31/1994 JLB : Uses mouse coordinate parameters.                                        *
 *=============================================================================================*/
void RadarClass::AI(KeyNumType & input, int x, int y)
{
	/*
	** Check to see if we need to animate the radar cursor
	*/
	if (IsRadarActive && Map.IsSidebarActive && SpecialRadarFrame) {
		SpecialRadarFrame--;
		RadarCursorRedraw = TRUE;
		IsToRedraw = TRUE;
		Flag_To_Redraw(FALSE);
	}

	/*
	** Check goes here to see if there is enough power to run the radar
	*/
	if (IsRadarActivating) {
		if (!DoesRadarExist) {
			RadarAnimFrame++;
			if (RadarAnimFrame < RADAR_ACTIVATED_FRAME) {
				IsToRedraw = true;
				Flag_To_Redraw(false);
			} else {
				DoesRadarExist = true;
				Radar_Activate(3);
			}
		} else {
			RadarAnimFrame--;
			if (RadarAnimFrame > RADAR_ACTIVATED_FRAME) {
				IsToRedraw = true;
				Flag_To_Redraw(false);
			} else {
				Radar_Activate(3);
			}
		}
	}

	/*
	** Check goes here to see if there is enough power to run the radar
	*/
	if (IsRadarDeactivating) {
		RadarAnimFrame++;
		if (RadarAnimFrame == MAX_RADAR_FRAMES) {
			IsRadarDeactivating = false;
		} else {
			IsToRedraw = true;
			Flag_To_Redraw(false);
		}
	}

	/*
	** Check here to see if radar is being jammed, so we can update the
	** animation with snow.
	*/
	if (!IsRadarActivating && !IsRadarDeactivating) {
		RadarAnimFrame++;
		if (RadarAnimFrame < RADAR_ACTIVATED_FRAME) RadarAnimFrame = RADAR_ACTIVATED_FRAME;
		if (RadarAnimFrame > (3 + RADAR_ACTIVATED_FRAME)) RadarAnimFrame = RADAR_ACTIVATED_FRAME;
		IsToRedraw = true;
		Flag_To_Redraw(false);
	}

	/*
	** Check here to see if the sonar pulse is active, and if it is, flag the
	** radar to redraw so the pulse ping will display.
	*/
	if (IsPulseActive) {
		Flag_To_Redraw(true);
		IsToRedraw = true;
		if (RadarPulseFrame >= 8) {
			RadarPulseFrame = 0;
			IsPulseActive = false;
		}
	}

	DisplayClass::AI(input, x, y);
}


/***********************************************************************************************
 * RadarClass::RTacticalClass::Action -- I/O function for the radar map.                       *
 *                                                                                             *
 *    This is the main action function for handling player I/O on the radar map. It processes  *
 *    mouse clicks as well as mouse moves.                                                     *
 *                                                                                             *
 * INPUT:   flags -- The event flags that trigger this function call.                          *
 *                                                                                             *
 *          key   -- Reference the keyboard event that applies to the trigger event.           *
 *                                                                                             *
 * OUTPUT:  Should further processing of the input list be aborted?                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int RadarClass::RTacticalClass::Action(unsigned flags, KeyNumType & key)
{
	CELL			cell;							// cell num click happened over
	int			x,y;							// Sub cell pixel coordinates.
	int			cellx,celly;				// Sub cell pixel coordinates.
	bool			shadow;						// is the cell in shadow or not
	ObjectClass * object = 0;				// what object is in the cell
	ActionType 	action = ACTION_NONE;	// Action possible with currently selected object.

	/*
	**	Force any help label to disappear when the mouse is held over the
	**	radar map.
	*/
	if (Map.IsSidebarActive) {
		Map.Help_Text(TXT_NONE);
	}

	if (!Map.IsRadarActive) {
		if (Map.IsSidebarActive) {
			Map.Override_Mouse_Shape(MOUSE_NORMAL);
//			Map.Override_Mouse_Shape(MOUSE_NORMAL, true);
		}
		return(false);
	}

	/*
	** Disable processing if the player names are up
	*/
	if (Map.Is_Player_Names()) {
		GadgetClass::Action(0, key);
		return(true);
	}

	/*
	**	Set some working variables that depend on the mouse position. For the press
	**	or release event, special mouse queuing storage variables are used. Other
	**	events must use the current mouse position globals.
	*/
	if (flags & (LEFTPRESS|LEFTRELEASE|RIGHTPRESS|RIGHTRELEASE)) {
		x = Keyboard->MouseQX;
		y = Keyboard->MouseQY;
	} else {
		x = Get_Mouse_X();
		y = Get_Mouse_Y();
	}

	/*
	**	See if the mouse is over the radar general area, but not yet
	**	over the active region of the radar map. In such a case, the
	**	mouse is overridden to be the normal cursor and no other
	**	action is performed.
	*/
	if (x < Map.RadX+Map.RadOffX || x >= Map.RadX+Map.RadIWidth || y < Map.RadY+Map.RadOffY || y >= Map.RadY+Map.RadIHeight) {
		Map.Override_Mouse_Shape(MOUSE_NORMAL);
		return(false);
	}

	int result = Map.RadarClass::Click_In_Radar(x, y, false);

	if (result == 1) {
		cell = Map.RadarClass::Click_Cell_Calc(x, y);
		if (cell != -1 && Map.In_Radar(cell)) {
			shadow	= (!Map[cell].IsMapped && !Debug_Unshroud);
//			shadow	= (!Map[cell].IsVisible && !Debug_Unshroud);
			cellx	   = 12;
			celly	   = 12;

			/*
			**	Determine the object that the mouse is currently over.
			*/
			if (!shadow) {
				object = Map.Cell_Object(cell, cellx, celly);
			}

			/*
			**	If there is a currently selected object, then the action to perform if
			**	the left mouse button were clicked must be determined.
			*/
			if (CurrentObject.Count()) {
				if (object) {
					action = CurrentObject[0]->What_Action(object);
				} else {
					action = CurrentObject[0]->What_Action(cell);
				}

				/*
				** If this is not a valid radar map action then we are not going to do
				** anything.
				*/
				switch (action) {
					case ACTION_MOVE:
					case ACTION_NOMOVE:
					case ACTION_ATTACK:
					case ACTION_ENTER:
					case ACTION_CAPTURE:
					case ACTION_SABOTAGE:
						break;

					default:
						action = ACTION_NONE;
						object = NULL;
						break;
				}

				/*
				** On the radar map the only reason we would want the normal cursor to
				** appear is if we were over one of our own selected units.  Otherwise
				** we can't move there.
				**/
				if (action == ACTION_NONE) {
					if (object && object->Is_Selected_By_Player()) {
						object = NULL;
					} else {
						action = ACTION_NOMOVE;
					}
				}

				/*
				**	A right mouse button press toggles the zoom mode.
				*/
				if (flags & RIGHTPRESS) {
					Map.Mouse_Right_Press();
				}

				/*
				**	When the mouse buttons aren't pressed, only the mouse cursor shape is processed.
				**	The shape changes depending on what object the mouse is currently over and what
				**	object is currently selected.
				*/
				if (flags & LEFTUP) {
					Map.Mouse_Left_Up(-1, shadow, object, action, true);
				}

				/*
				**	Normal actions occur when the mouse button is released. The press event is
				**	intercepted and possible rubber-band mode is flagged.
				*/
				if (flags & LEFTPRESS) {
					Map.Mouse_Left_Release(cell, cellx, celly, object, action, true);
				}

			} else {

				Map.Set_Default_Mouse(MOUSE_RADAR_CURSOR, !Map.IsZoomed);

				if (flags & LEFTPRESS) {

					cell = Map.RadarClass::Click_Cell_Calc(x, y);
					if (cell != -1) {
						int cellx = Cell_X(cell);
						int celly = Cell_Y(cell);
						cellx -= Lepton_To_Cell(Map.TacLeptonWidth) / 2;
						cellx = max(cellx, Map.MapCellX);
						celly -= Lepton_To_Cell(Map.TacLeptonHeight) / 2;
						celly = max(celly, Map.MapCellY);
						cell = XY_Cell(cellx, celly);
						shadow = (!Map[cell].IsMapped && !Debug_Unshroud);
//						shadow = (!Map[cell].IsVisible && !Debug_Unshroud);
						Map.Set_Tactical_Position(Cell_Coord(cell));
						cell = Coord_Cell(Map.DesiredTacticalCoord);
						Map.DisplayClass::IsToRedraw = true;
						Map.Flag_To_Redraw(true);
						Map.SpecialRadarFrame = 4;
					}
				}

				/*
				**	A right mouse button press toggles the zoom mode.
				*/
				if (flags & RIGHTPRESS) {
					Map.Zoom_Mode(cell);
				}
			}
		}
	}
	if (result == -1) {
		Map.Override_Mouse_Shape(MOUSE_NORMAL, true);
	}
	GadgetClass::Action(0, key);
	return(true);
}


/***********************************************************************************************
 * RadarClass::Refresh_Cells -- Intercepts refresh request and updates radar if needed         *
 *                                                                                             *
 *    This routine intercepts the refresh cells request and if it detects that the sidebar     *
 *    should be rerendered, it flags the radar map to redraw during the next draw operation.   *
 *                                                                                             *
 * INPUT:   cell  -- The origin cell that the refresh cell offset list is based upon.          *
 *                                                                                             *
 *          list  -- Pointer to the list of offsets from the origin cell that specifies the    *
 *                   cells to be flagged for redraw. If the list starts with the special       *
 *                   code to refresh the sidebar, then this routine recognizes it and flags    *
 *                   the radar map to be redrawn accordingly.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::Refresh_Cells(CELL cell, short const * list)
{
	if (*list == REFRESH_SIDEBAR) {
		IsToRedraw = true;
		Flag_To_Redraw(false);
	}
	DisplayClass::Refresh_Cells(cell, list);
}


/***********************************************************************************************
 * RadarClass::Set_Radar_Position -- Sets the radar position to center around specified cell.  *
 *                                                                                             *
 *    This routine will try to center the radar map around the cell position specified.        *
 *                                                                                             *
 * INPUT:   cell  -- The cell to try and position the radar map around.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::Set_Radar_Position(CELL cell)
{
#ifdef WIN32
	int oldx, oldy;
	int newx, newy;
	int newcell;

	if (ZoomFactor != 1) {
		oldx = (Cell_X(cell) - MapCellX);
		oldy = (Cell_Y(cell) - MapCellY);
	} else {
		oldx = 0;
		oldy = 0;
	}

	Confine_Rect(&oldx, &oldy, RadarCellWidth, RadarCellHeight, MapCellWidth, MapCellHeight);

	newx 		= oldx + MapCellX;
	newy 		= oldy + MapCellY;
	newcell	= XY_Cell(newx, newy);

	if (RadarCell != newcell) {
   	int forced = FALSE;
		int xmod = newx;
		int ymod = newy;

		int radx = (Cell_X(RadarCell)) - xmod;
		int rady = (Cell_Y(RadarCell)) - ymod;

		RadarX 		= newx;
		RadarY 		= newy;
		RadarCell 	= newcell;

		if (Map.IsSidebarActive && Map.IsRadarActive) {
			int radw = RadarCellWidth-ABS(radx);			// Replicable width.
			int radh = RadarCellHeight-ABS(rady);		// Replicable height.

			if (radw < 1) forced = true;
			if (radh < 1) forced = true;

			if (!forced && (radw != RadarWidth || radh != RadarHeight)) {
				/*
				** Blit the section that is actually overlapping.
				**
				** If the video card isnt able to blit overlapped regions then we have
				** to do the blit in two stages via an intermediate buffer. The test to allow
				** overlapped blits is done in the library at the time of setting the video mode.
				*/
				if (OverlappedVideoBlits || !HidPage.Get_IsDirectDraw()) {

					/*
					** Overlapped blits are OK or we dont have a video memory hid page so blits are
					** always done in software by the library anyway.
					*/
					HidPage.Blit(HidPage,
									(((radx < 0) ? -radx : 0) * ZoomFactor) + RadX + RadOffX + BaseX,
									(((rady < 0) ? -rady : 0) * ZoomFactor) + RadY + RadOffY + BaseY,
									(((radx < 0) ? 0 : radx) * ZoomFactor) + RadX+ RadOffX + BaseX,
									(((rady < 0) ? 0 : rady) * ZoomFactor) + RadY + RadOffY + BaseY,
									radw * ZoomFactor,
									radh * ZoomFactor);

				} else {
					/*
					** Create a temporary intermediate surface
					*/
					GraphicBufferClass temp_surface;
					temp_surface.Init((RadarWidth + 16) & 0xfffffff0,
											(RadarHeight + 16) & 0xfffffff0,
											NULL, 0, (GBC_Enum) GBC_VIDEOMEM);

					/*
					** Do the blit in 2 stages.
					*/
					HidPage.Blit(temp_surface,
									(((radx < 0) ? -radx : 0) * ZoomFactor) + RadX + RadOffX + BaseX,
									(((rady < 0) ? -rady : 0) * ZoomFactor) + RadY + RadOffY + BaseY,
									0,
									0,
									RadarWidth,
									RadarHeight);

					temp_surface.Blit(HidPage,
									0,
									0,
									(((radx < 0) ? 0 : radx) * ZoomFactor) + RadX+ RadOffX + BaseX,
									(((rady < 0) ? 0 : rady) * ZoomFactor) + RadY + RadOffY + BaseY,
									radw * ZoomFactor,
									radh * ZoomFactor);
				}

				/*
				** Now we need to flag the section of the map that is going to redraw.
				*/
				if ( radx != 0 ) {
					int min;
					int max;
					if ( radx < 0 ) {  								// this mean regen the right edge
						min = radw;
						max = radw+ABS(radx);
					} else {												//	this mean regen the left edge
						min = 0;
						max = radx;
					}
					for (int x = min; x < max; x++ ) {
						for (int y = 0; y < (int)RadarCellHeight; y++ ) {
							Radar_Pixel(XY_Cell(newx + x, newy + y));
						}
					}
				}
				if ( newy != 0 ) {
					int min;
					int max;
					if ( rady < 0 ) {  								// this mean regen the bottom edge
						min = radh;
						max = radh+ABS(rady);
					} else {												// this mean regen the top edge
						min = 0;
						max = rady;
					}
					for (int y = min; y < max; y++ ) {
						for ( int x = 0; x < (int)RadarCellWidth; x++ ) {
							Radar_Pixel(XY_Cell(newx + x, newy + y));
						}
					}
				}
			}
		}
		RadarCursorRedraw = IsRadarActive;
		IsToRedraw 	= IsRadarActive;
		Flag_To_Redraw(false);
		if (ZoomFactor > 4) {
			FullRedraw = forced;
		}
	} else {
		RadarCursorRedraw = IsRadarActive;
		IsToRedraw 	= IsRadarActive;
		Flag_To_Redraw(false);
	}
#else

	if (cell != RadarCell) {
		int oldx = RadarX;
		int oldy = RadarY;
		CELL oldcell = RadarCell;
		int x = Cell_X(cell);
		int y = Cell_Y(cell);

		/*
		**	If the new radar position is not too close to the edge of the
		**	current radar display, then don't bother to change the radar position.
		*/
		if ((unsigned)(x - (RadarX+10)) > RadarCellWidth-20) {
			oldx = (Cell_X(cell)-MapCellX)-RadarCellWidth/2;
		} else {
			oldx = Cell_X(RadarCell)-MapCellX;
		}
		if ((unsigned)(y - (RadarY+10)) > RadarCellHeight-20) {
			oldy = (Cell_Y(cell)-MapCellY)-RadarCellHeight/2;
		} else {
			oldy = Cell_Y(RadarCell)-MapCellY;
		}

#ifdef NEVER
		if ((unsigned)(x - (RadarX+10)) > RadarWidth-20 || (unsigned)(y - (RadarY+10)) > RadarHeight-20) {
			oldx = (Cell_X(cell)-MapCellX)-RadarCellWidth/2;
			oldy = (Cell_Y(cell)-MapCellY)-RadarCellHeight/2;
		} else {
			oldx = Cell_X(RadarCell)-MapCellX;
			oldy = Cell_Y(RadarCell)-MapCellY;
		}
#endif

		Confine_Rect(&oldx, &oldy, RadarCellWidth, RadarCellHeight, MapCellWidth, MapCellHeight);
		RadarX = oldx + MapCellX;
		RadarY = oldy + MapCellY;
		RadarCell = XY_Cell(RadarX, RadarY);
		IsToRedraw = true;
		Flag_To_Redraw(false);
		if (oldcell != RadarCell) {
			FullRedraw = IsRadarActive;
		}
	}
#endif
}


/***********************************************************************************************
 * RadarClass::Radar_Position -- Returns with the current position of the radar map.           *
 *                                                                                             *
 *    This returns the cell number of the upper left corner of the radar map.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the radar map upper left corner cell position.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL RadarClass::Radar_Position(void)
{
	return(RadarCell);
}


/***********************************************************************************************
 * RadarClass::Set_Map_Dimensions -- Sets the tactical map dimensions.                         *
 *                                                                                             *
 *    This routine is called when the tactical map changes its dimensions. This occurs when    *
 *    the tactical map moves and when the sidebar pops on or off.                              *
 *                                                                                             *
 * INPUT:   x,y   -- The cell coordinate of the upper left corner of the tactical map.         *
 *                                                                                             *
 *          w,y   -- The cell width and height of the tactical map.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::Set_Map_Dimensions(int x, int y, int w, int h)
{
	Set_Radar_Position(XY_Cell(x, y));
	DisplayClass::Set_Map_Dimensions(x, y, w, h);
}


/***********************************************************************************************
 * RadarClass::Set_Tactical_Position -- Sets the map's tactical position and adjusts radar to  *
 *                                                                                             *
 *    This routine is called when the tactical map is to change position. The radar map might  *
 *    be adjusted as well by this routine.                                                     *
 *                                                                                             *
 * INPUT:   coord -- The new coordinate to use for the upper left corner of the tactical       *
 *                   map.                                                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/17/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::Set_Tactical_Position(COORDINATE coord)
{
	DisplayClass::Set_Tactical_Position(coord);
	Set_Radar_Position(Coord_Cell(TacticalCoord));
}


/***********************************************************************************************
 * RadarClass::Cell_On_Radar -- Determines if a cell is currently visible on radar.            *
 *                                                                                             *
 *    This routine will examine the specified cell number and return whether it is visible     *
 *    on the radar map. This depends on the radar map position.                                *
 *                                                                                             *
 * INPUT:   cell  -- The cell number to check.                                                 *
 *                                                                                             *
 * OUTPUT:  Is the specified cell visible on the radar map currently?                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/03/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Cell_On_Radar(CELL cell)
{
	if ((unsigned)cell > MAP_CELL_TOTAL)
		return(false);

	if (!IsZoomed) {
		return(true);
	}
	return(!(((Cell_X(cell) - RadarX) > RadarCellWidth) || ((Cell_Y(cell) - RadarY) > RadarCellHeight)));
}


/***********************************************************************************************
 * RadarClass::Player_Names -- toggles the Player-Names mode of the radar map                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      on         true = turn on; false = turn off                                            *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/07/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::Player_Names(bool on)
{
	IsPlayerNames = on;
	IsToRedraw = true;
	if (on) {
		Flag_To_Redraw(true);
//		Flag_To_Redraw(false);
	} else {
		Flag_To_Redraw(true);		// force drawing of the plate
	}
}


/***********************************************************************************************
 * RadarClass::Spy_Next_House -- advances to the next house we're spying on, or returns NULL	  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      0 = no house to spy on, 1 = found house to spy on                                      *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Spy_Next_House(void)
{
	bool	tospy = false;
	int spiedby = (1<<(PlayerPtr->Class->House));

	IsPlayerNames = false;
	IsToRedraw = true;

	HousesType maxhouse;
	HousesType firsthouse;
	HousesType house;

	if (Session.Type == GAME_NORMAL) {
		firsthouse = HOUSE_SPAIN;
		maxhouse = HOUSE_GOOD;
	} else {
		firsthouse = HOUSE_MULTI1;
		maxhouse = HOUSE_COUNT;
	}

	if (IsHouseSpy) {
	 	house = (HousesType)(SpyingOn+1);
	} else {
		house = firsthouse;
	}

	if (house < firsthouse) house = firsthouse;

	while (house < maxhouse && !tospy) {
		HouseClass * hptr = HouseClass::As_Pointer(house);
		if (hptr && hptr->IsActive && hptr != PlayerPtr) {
			if (hptr->RadarSpied & spiedby) {
				tospy = true;
				SpyingOn = house;
				break;
			}
		}
		house++;
	}

	IsHouseSpy = tospy;

	Flag_To_Redraw(true);		// force drawing of the plate
	return(tospy);
}


/***********************************************************************************************
 * Draw_House_Info -- Print house statistics on the radar map											  *
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
 *   03/20/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Draw_House_Info(void)
{
	int y;
	char txt[40];
	/*
	** Do nothing if the sidebar isn't there
	*/
	if (!Map.IsSidebarActive) {
		return false;
	}
	CC_Draw_Shape(RadarFrame, 1, RadX, RadY + 1*RESFACTOR, WINDOW_MAIN, SHAPE_NORMAL);
	y = RadY + RadOffY + (2*RESFACTOR);

	Map.Repair.Draw_Me(true);
	Map.Upgrade.Draw_Me(true);
	Map.Zoom.Draw_Me(true);

	Fancy_Text_Print (TXT_SPY_INFO, RadX + RadOffX + (6 * RESFACTOR), y,
		&ColorRemaps[PCOLOR_GREY], TBLACK,
		TPF_6PT_GRAD | TPF_NOSHADOW);
	y += 7*RESFACTOR;

	HouseClass * ptr = HouseClass::As_Pointer(SpyingOn);
	if (ptr && (ptr->RadarSpied & (1<<(PlayerPtr->Class->House))) ) {
		PlayerColorType c_idx;
		RemapControlType * color;
		TextPrintType style;

		c_idx = ptr->RemapColor;
		color = &ColorRemaps[c_idx];
		style = TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW;

		/*
		** Print house's name below 'spy report'
		*/
		txt[0] = 0;
		sprintf(txt, "%s", ptr->IniName);//Text_String(ptr->Class->FullName));
//		sprintf(txt, "%s", ptr->Name());//Text_String(ptr->Class->FullName));
		if (strlen(txt)) {
			if (strlen(txt) > 9) {
				txt[9] = '.';
				txt[10] = '\0';
			}
			Fancy_Text_Print (txt, RadX + RadOffX + (6 * RESFACTOR), y, color, BLACK, style);
		} else {
			strcpy(txt,"________");
		}
		y += (6 * RESFACTOR) + 1;

		Fancy_Text_Print(TXT_BUILDNGS, RadX + RadOffX + (6 * RESFACTOR), y, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
		y += (6 * RESFACTOR) + 1;

// count & print buildings
		itoa(ptr->CurBuildings, txt, 10);
		Fancy_Text_Print(txt, RadX + RadOffX + (6 * RESFACTOR), y, color, BLACK, style);
		y += (6 * RESFACTOR) + 1;

		Fancy_Text_Print(TXT_UNITS, RadX + RadOffX + (6 * RESFACTOR), y, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
		y += (6 * RESFACTOR) + 1;
// count & print units
		itoa(ptr->CurUnits, txt, 10);
		Fancy_Text_Print(txt, RadX + RadOffX + 6 * RESFACTOR, y, color, BLACK, style);
		y += (6 * RESFACTOR) + 1;

		Fancy_Text_Print(TXT_INFANTRY, RadX + RadOffX + (6 * RESFACTOR), y, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
		y += (6 * RESFACTOR) + 1;
// count & print infantry
		itoa(ptr->CurInfantry, txt, 10);
		Fancy_Text_Print(txt, RadX + RadOffX + (6 * RESFACTOR), y, color, BLACK, style);
#if(0)
		y += (6 * RESFACTOR) + 1;

		Fancy_Text_Print(TXT_AIRCRAFT, RADAR_X + RADAR_OFF_X + 6, y,
								&ColorRemaps[PCOLOR_GREY], TBLACK,
								TPF_6PT_GRAD | TPF_NOSHADOW);
		y += (6 * RESFACTOR) + 1;
// count & print aircraft
		for (i = AIRCRAFT_NONE+1, count = 0; i < AIRCRAFT_COUNT; i++) {
			count += ptr->AQuantity[i];
		}
		itoa(count, txt, 10);
		Fancy_Text_Print(txt, RadX + RadOffX, y,
				color, BLACK, style);
#endif
		return(true);
	} else {
		return(false);
	}
}


/***********************************************************************************************
 * Draw_Names -- draws players' names on the radar map                                         *
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
 *   06/07/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void RadarClass::Draw_Names(void)
{
	PlayerColorType c_idx;
	HousesType house;
	HouseClass * ptr;
	int y;
	char txt[40];
	HousesType h;
	int kills;
	RemapControlType * color;
	TextPrintType style;

	/*
	** Do nothing if the sidebar isn't there
	*/
	if (!Map.IsSidebarActive) {
		return;
	}

//	CC_Draw_Shape(RadarAnim, RADAR_ACTIVATED_FRAME, RADAR_X, RADAR_Y+1,
//		WINDOW_MAIN, SHAPE_NORMAL);
	CC_Draw_Shape(RadarFrame, 1, RadX, RadY + 1*RESFACTOR, WINDOW_MAIN, SHAPE_NORMAL);

	y = RadY + RadOffY+(2*RESFACTOR);

	Fancy_Text_Print (TXT_NAME_COLON, RadX + RadOffX, y, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
	Fancy_Text_Print (TXT_KILLS_COLON, RadX + RadOffX + RadIWidth - 2, y, &ColorRemaps[PCOLOR_GREY], TBLACK, TPF_RIGHT | TPF_6PT_GRAD | TPF_NOSHADOW);
	y += 6*RESFACTOR+1;

	LogicPage->Draw_Line(RadX + RadOffX, y, RadX + RadOffX + RadIWidth - 1, y, LTGREY);
	y += 2*RESFACTOR;

	for (house = HOUSE_MULTI1; house < (HOUSE_MULTI1 + Session.MaxPlayers); house++) {
		ptr = HouseClass::As_Pointer(house);

		if (!ptr) continue;

		/*
		**	Decode this house's color
		*/
		c_idx = ptr->RemapColor;

		if (ptr->IsDefeated) {
			color = &GreyScheme;
			style = TPF_6PT_GRAD | TPF_NOSHADOW;
		} else {
			color = &ColorRemaps[c_idx];
			style = TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW;
		}

		/*
		**	Initialize our message
		*/
		txt[0] = 0;
//		sprintf(txt, "%s", ptr->Name());
		sprintf(txt, "%s", ptr->IsHuman ? ptr->IniName : Text_String(TXT_COMPUTER));

		if (strlen(txt) == 0) {
			strcpy(txt,"________");
		}

		/*
		**	Print the player name, and the # of kills
		*/
#ifdef WIN32
		if (strlen(txt) > 9) {
			txt[9] = '.';
			txt[10] = '\0';
		}
#else
		if (strlen(txt) > 8) {
			txt[8] = '.';
			txt[9] = '\0';
		}
#endif
		Fancy_Text_Print (txt, RadX + RadOffX, y, color, TBLACK, style);

		kills = 0;
		for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
			kills += ptr->UnitsKilled[h];
			kills += ptr->BuildingsKilled[h];
		}
		sprintf(txt, "%2d", kills);
		Fancy_Text_Print (txt, RadX + RadOffX + RadIWidth - 2, y, color, TBLACK, style | TPF_RIGHT);

		y += 6*RESFACTOR+1;

	}

	Map.Repair.Draw_Me(true);
	Map.Upgrade.Draw_Me(true);
	Map.Zoom.Draw_Me(true);
}


void RadarClass::Activate_Pulse(void)
{
	if (IsRadarActive || PlayerPtr->IsGPSActive) {
		IsPulseActive = true;
		RadarPulseFrame = 0;
	}
}


/***********************************************************************************************
 * RadarClass::Is_Radar_Active -- Determines if the radar map is currently being displayed.    *
 *                                                                                             *
 *    Determines if the radar map is currently being displayed.                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the radar map currently being displayed as active?                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Is_Radar_Active(void)
{
	return(IsRadarActive || PlayerPtr->IsGPSActive);
//	return IsRadarActive || PlayerPtr->IsGPSActive;
}


/***********************************************************************************************
 * RadarClass::Is_Radar_Activating -- Determines if the radar map is being activated.          *
 *                                                                                             *
 *    Determines if the radar map is being activated.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the radar map currently being activated?                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/2019 SKY : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Is_Radar_Activating(void)
{
	return IsRadarActivating;
}


/***********************************************************************************************
 * RadarClass::Is_Radar_Existing -- Queries to see if radar map is available.                  *
 *                                                                                             *
 *    This will determine if the radar map is available. If available, the radar will show     *
 *    representations of terrain, units, and buildings.                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the radar map available to be displayed?                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Is_Radar_Existing(void)
{
	return(DoesRadarExist || PlayerPtr->IsGPSActive);
}


/***********************************************************************************************
 * RadarClass::Get_Jammed -- Fetch the current radar jammed state for the player.              *
 *                                                                                             *
 *    This will fetch the current state of the radar jamming for the player.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the radar currently jammed?                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RadarClass::Get_Jammed(HousesType house) const
{
	for (int i = 0; i < Houses.Count(); ++i) {
		HouseClass* player = Houses.Ptr(i);
		if (player && player->Class->House == house) {
			return Get_Jammed(player);
		}
	}
	return false;
}

bool RadarClass::Get_Jammed(HouseClass *player) const
{
	assert(player);
	if (player->IsGPSActive) return(false);
	int shift = (int)player->Class->House;
	return (IsRadarJammedByPlayerMask & (1 << shift)) ? true : false;
}

void RadarClass::Set_Jammed(HousesType house, bool jam)
{
	int shift = (int)house;
	if (jam) {
		IsRadarJammedByPlayerMask |= (1 << shift);
	}
	else {
		IsRadarJammedByPlayerMask &= ~(1 << shift);
	}
}

void RadarClass::Set_Jammed(HouseClass *player, bool jam)
{
	if (player && player->Class) {
		Set_Jammed(player->Class->House, jam);
	}
}

void RadarClass::Flag_Cell(CELL cell)
{
//	Radar_Pixel(cell);
	DisplayClass::Flag_Cell(cell);
}