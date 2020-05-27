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

/* $Header:   F:\projects\c&c\vcs\code\radar.cpv   2.17   16 Oct 1995 16:49:28   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : November 17, 1995 [PWG]                                      *
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
 *   RadarClass::Init_Clear -- Sets the radar map to a known state                             *
 *   RadarClass::Map_Cell -- Updates radar map when a cell becomes mapped.                     *
 *   RadarClass::One_Time -- Handles one time processing for the radar map.                    *
 *   RadarClass::Player_Names -- toggles the Player-Names mode of the radar map                *
 *   RadarClass::Plot_Radar_Pixel -- Updates the radar map with a terrain pixel.               *
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
 *   RadarClass::Set_Radar_Position -- Sets the radar map coordinates.                         *
 *   RadarClass::Set_Radar_Position -- Sets the radar position to center around specified cell.*
 *   RadarClass::Set_Tactical_Position -- Called when setting the tactical display position.   *
 *   RadarClass::Set_Tactical_Position -- Called when setting the tactical display position.   *
 *   RadarClass::TacticalClass::Action -- I/O function for the radar map.                      *
 *   RadarClass::Zoom_Mode(void) -- Handles toggling zoom on the map                           *
 *   RadarClass::Set_Tactical_Position -- Sets the map's tactical position and adjusts radar to*
 *   RadarClass::Coord_To_Radar_Pixel -- Converts a coordinate to a radar pixel position       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include <stdlib.h>

//void const * RadarClass::CoverShape;
RadarClass::TacticalClass RadarClass::RadarButton;

void const * RadarClass::RadarAnim = NULL;

static bool FullRedraw = false;

#define _MAX_NAME 13

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
RadarClass::RadarClass(void)
{
	IsZoomed          = true;
	IsRadarActive		= false;
	IsToRedraw			= false;
	RadarCursorRedraw = false;
	PixelPtr				= 0;
	SpecialRadarFrame = 0;
	IsPlayerNames		= false;
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
	int factor = Get_Resolution_Factor();
	RadWidth		= 80 << factor;
	RadHeight 	= 70 << factor;
	RadX 			= SeenBuff.Get_Width() - RadWidth;
	RadY 			= Map.Get_Tab_Height() - (1 << factor);
	RadPWidth 	= 64 << factor;
	RadPHeight 	= 64 << factor;
	if ( factor ) {
		RadOffX 		= 16;
		RadOffY 		= 7;
		RadIWidth 	= 128;
		RadIHeight	= 128;
	} else {
		RadOffX 		= 4 << factor;
		RadOffY 		= 1 << factor;
		RadIWidth 	= 72 << factor;
		RadIHeight	= 69 << factor;
	}

	DisplayClass::One_Time();
	RadarButton.X			= RadX+RadOffX;
	RadarButton.Y 			= RadY+RadOffY;
	RadarButton.Width 	= RadIWidth;
	RadarButton.Height 	= RadIHeight;
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
	IsPlayerNames		= false;

	/*
	** If we have a valid map lets make sure that we set it correctly
	*/
	if (MapCellWidth || MapCellHeight) {
		IsZoomed = false;
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
			if (GameToPlay==GAME_NORMAL) {
				Map.Zoom.Disable();
			}
			IsRadarActive = false;
			IsRadarActivating = false;
			IsRadarDeactivating = false;
			break;

		case 3:
			if (GameToPlay==GAME_NORMAL) {
				Map.Zoom.Enable();
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
//	if (!In_Debugger)	while (!HidPage.Lock()) {}

#if (0)	                               // Legacy radar rendering not used. ST - 2/26/2020 3:53PM

	/*
	**	Don't perform any rendering if none is requested.
	*/
	if (!forced && !IsToRedraw && !FullRedraw) return;

	static HousesType _house = HOUSE_NONE;
	if (PlayerPtr->ActLike != _house) {
		char name[_MAX_NAME + _MAX_EXT];

		if (Special.IsJurassic && AreThingiesEnabled) {
			strcpy(name, "RADAR.JP");
		} else {
			_makepath(name, NULL, NULL, "RADAR", HouseTypeClass::As_Reference(PlayerPtr->ActLike).Suffix);
		}
		RadarAnim = Hires_Retrieve(name);
		_house = PlayerPtr->ActLike;
	}

	/*
	** If in player name mode, just draw player names
	*/
	if (IsPlayerNames) {
		Draw_Names();
		IsToRedraw = false;
		return;
	}

	if (IsRadarActivating || IsRadarDeactivating) {
		Radar_Anim();
		IsToRedraw = false;
		return;
	}

	if (Map.IsSidebarActive) {
		if (IsRadarActive) {

			//HidPage.Lock();
// ST 8/13/96 2:24PM
//forced = true;
			/*
			**	If only a few of the radar pixels need to be redrawn, then find and redraw
			**	only these.
			*/
			if (!forced && IsToRedraw && !FullRedraw) {
				IsToRedraw = false;

				if (PixelPtr) {

					/*
					**	Render all pixels in the "to redraw" stack.
					*/
					for (unsigned int index = 0; index < PixelPtr; index++) {
						CELL cell = PixelStack[index];
					  	if (Cell_On_Radar(cell)) {
							(*this)[cell].IsPlot = false;
							Plot_Radar_Pixel(cell);
							RadarCursorRedraw |= (*this)[cell].IsRadarCursor;
						}
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
				GraphicViewPortClass *oldpage	= Set_Logic_Page(HidPage);
//				if (LogicPage->Lock()) {
					CC_Draw_Shape(RadarAnim, RADAR_ACTIVATED_FRAME, RadX, RadY+1, WINDOW_MAIN, SHAPE_NORMAL);
					if (BaseX || BaseY) {
						LogicPage->Fill_Rect(	RadX + RadOffX,
														RadY + RadOffY,
														RadX + RadOffX + RadIWidth - 1,
														RadY + RadOffY + RadIHeight - 1,
														DKGREY);
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
					for (int index = 0; index < MAP_CELL_TOTAL; index++) {
						Plot_Radar_Pixel(index);
					}
					Radar_Cursor(true);
					FullRedraw = false;
					IsToRedraw = false;
					LogicPage->Unlock();
					if (oldpage == &SeenBuff) {
						Hide_Mouse();
						LogicPage->Blit(SeenBuff, RadX, RadY, RadX, RadY, RadWidth, RadHeight);
						Show_Mouse();
					}

//					Set_Logic_Page(oldpage);

//				}

			}

		} else {

			/*
			**	If the radar is not active, then only draw the cover plate if forced to do so.
			*/
//			if (forced) {
				int val = (DoesRadarExist) ?  MAX_RADAR_FRAMES : 0;
				CC_Draw_Shape(RadarAnim, val, RadX, RadY + 1, WINDOW_MAIN, SHAPE_NORMAL);
				FullRedraw = false;
				IsToRedraw = false;
//			}
		}

		//HidPage.Unlock();
//		Map.Activator.Draw_Me(true);
	}
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
	TerrainClass	*list[4];
	int				listidx = 0;
	int				lp,lp2;


	ObjectClass *obj = Map[cell].Cell_Occupier();

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
	for (lp = 0; lp < 3; lp ++) {
		obj = Map[cell].Overlapper[lp];
		if (obj && obj->What_Am_I() == RTTI_TERRAIN)
			list[listidx++] = (TerrainClass *)obj;
	}

	/*
	** If there are no entrys in our list then just get out.
	*/
	if (!listidx) return;

	/*
	**	If there is terrain in this cell then draw a dark pixel to
	** represent it.
	*/
	if (size == 1) {
		LogicPage->Put_Pixel(x, y, 60);
		return;
	}

	/*
	** Sort the list by its sort Y value so that we can render in the proper
	** order.
	*/
	for (lp = 0; lp < listidx - 1; lp ++) {
		for (lp2 = lp + 1; lp2 < listidx; lp2++) {
			if (list[lp]->Sort_Y() > list[lp2]->Sort_Y()) {
				TerrainClass *terrain = list[lp];
				list[lp] = list[lp2];
				list[lp2] = terrain;
			}
		}
	}

	/*
	** loop through the list and take care of rendering the correct icon.
	*/
	for (lp = 0; lp < listidx; lp ++) {
		unsigned char *icon = list[lp]->Radar_Icon(cell);
		if (!icon) continue;

		Buffer_To_Page(0, 0, 3, 3, icon, _IconStage);
		_IconStage.Scale(*LogicPage, 0, 0, x, y, 3, 3, ZoomFactor, ZoomFactor, TRUE, (char *)&FadingBrighten[0]);
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
	ObjectClass 	*obj;
	int 				xoff,yoff;

	obj = (ObjectClass *)Map[cell].Cell_Occupier();
	while (obj) {
		if (obj->Is_Techno() && (((TechnoClass *)obj)->Cloak != CLOAKED || ((TechnoClass *)obj)->House->Is_Ally(PlayerPtr))) {
			switch (obj->What_Am_I()) {
				case RTTI_INFANTRY:
					{
						//int divisor = 255 / ZoomFactor;
						int divisor = 86;
						if ( ZoomFactor >= 3 ) {
							xoff = Coord_XLepton(obj->Coord) / divisor;
							yoff = Coord_YLepton(obj->Coord) / divisor;
							if ( ZoomFactor >= 6 ) {
								xoff<<=1;
								yoff<<=1;
							}
						} else {
							xoff = 0;
							yoff = 0;
						}
						LogicPage->Put_Pixel(x+xoff, y+yoff, ((InfantryClass *)obj)->House->Class->BrightColor);
					}
					break;

				case RTTI_UNIT:
				case RTTI_AIRCRAFT:
					// PWG: Slowdown?
					//if (LogicPage->Lock()){
						Fat_Put_Pixel(x, y, ((UnitClass *)obj)->House->Class->BrightColor, size, *LogicPage);
						//LogicPage->Unlock();
					//}
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
	OverlayType overlay = (*this)[cell].Overlay;
 	if (overlay != OVERLAY_NONE) {
		OverlayTypeClass const * otype = &OverlayTypeClass::As_Reference(overlay);

		if (otype->IsRadarVisible) {
			unsigned char *icon = otype->Radar_Icon((*this)[cell].OverlayData);
			if (!icon) return;
			Buffer_To_Page(0, 0, 3, 3, icon, _IconStage);
			if (otype->IsTiberium) {
				_IconStage.Scale(*LogicPage, 0, 0, x, y, 3, 3, size, size, TRUE, (char *)&FadingGreen[0]);
			} else {
				_IconStage.Scale(*LogicPage, 0, 0, x, y, 3, 3, size, size, TRUE, (char *)&FadingBrighten[0]);
			}
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
	IsZoomed				= !IsZoomed;
	BaseX					= 0;
	BaseY					= 0;

	/*
	** Figure out exactly what size we need to zoom the map to.
	*/
	if ( !IsZoomed ) {
		int xfactor			= RadIWidth / MapCellWidth;
		int yfactor			= RadIHeight / MapCellHeight;
		ZoomFactor			= MIN(xfactor,yfactor);
		map_c_width			= MapCellWidth;
		map_c_height      = MapCellHeight;
	} else {
		ZoomFactor			= 6;
		map_c_width			= RadIWidth / ZoomFactor;
		map_c_height		= RadIHeight / ZoomFactor;
	}

	/*
	** Make sure we do not show more cell then are on the map.
	*/
	map_c_width			= MIN(map_c_width, 62);
	map_c_height		= MIN(map_c_height, 62);

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
	RadarHeight			= RadIWidth - rem_y;

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
	if (!IsRadarActive || (unsigned)cell > MAP_CELL_TOTAL) return;

	if (!In_Radar(cell) || !Cell_On_Radar(cell)) {
		return;
	}

	/*
	** If we are zoomed in then calculate the pixel based off of the portion
	** of the map the radar is viewing.
	*/
	x = Cell_X(cell) - RadarX;
	y = Cell_Y(cell) - RadarY;

	if (LogicPage->Lock()) {
		CellClass * cellptr = &(*this)[cell];
 		x = RadX + RadOffX + BaseX + (x * ZoomFactor);
 		y = RadY + RadOffY + BaseY + (y * ZoomFactor);

 		/*
 		**	Determine what (if any) vehicle or unit should be rendered in this blip.
 		*/
		int color=TBLACK;		// Color of the pixel to plot.
 		if ((*this)[cell].IsVisible || Debug_Unshroud) {
 			color = cellptr->Cell_Color(true);
 		} else  {
 			color = BLACK;
 		}

// ST 8/13/96 2:24PM
//if (cellptr->IsRadarCursor){
//	color = WHITE;
//}

 		/*
 		**	If no color override occurs for this cell, then render the underlying
 		**	terrain.
 		*/
		if (color == TBLACK) {
			if (ZoomFactor > 1) {
				void const *ptr;
				long offset;
				int icon;

				if (cellptr->TType != TEMPLATE_NONE) {
					ptr = TemplateTypeClass::As_Reference(cellptr->TType).Get_Image_Data();
					icon = cellptr->TIcon;
				} else {
					ptr = TemplateTypeClass::As_Reference(TEMPLATE_CLEAR1).Get_Image_Data();
					icon = cellptr->Clear_Icon();
				}

				/*
				**	Convert the logical icon number into the actual icon number.
				*/
				Mem_Copy(Add_Long_To_Pointer((void *)ptr, 28), &offset, sizeof(offset));
				Mem_Copy(Add_Long_To_Pointer((void *)ptr, offset+icon), &icon, sizeof(char));
				icon &= 0x00FF;

				Mem_Copy(Add_Long_To_Pointer((void *)ptr, 12), &offset, sizeof(offset));
				ptr = Add_Long_To_Pointer((void *)ptr, offset + icon*(24*24));

				unsigned char * data = (unsigned char *)ptr;
				Buffer_To_Page(0, 0, 24, 24, data, _TileStage);
				_TileStage.Scale(*LogicPage, 0, 0, x, y, 24, 24, ZoomFactor, ZoomFactor, TRUE);

			} else {
				if (LogicPage->Lock()){
					Fat_Put_Pixel(x, y, cellptr->Cell_Color(false), ZoomFactor, *LogicPage);
					LogicPage->Unlock();
				}
			}
		} else {
			if (LogicPage->Lock()){
				Fat_Put_Pixel(x, y, color, ZoomFactor, *LogicPage);
				LogicPage->Unlock();
			}
		}
		if (color != BLACK) {
			Render_Overlay(cell, x, y, ZoomFactor);
		 	Render_Terrain(cell, x, y, ZoomFactor);
			Render_Infantry(cell, x, y, ZoomFactor);
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
		IsToRedraw 						= true;
		(*this)[cell].IsPlot 		= true;
		if (PixelPtr < PIXELSTACK) {
			PixelStack[PixelPtr++] 	= cell;
		}
	}
}


/***********************************************************************************************
 * RadarClass::Click_In_Radar -- Converts a radar click into cell X and Y coordinate.          *
 *                                                                                             *
 *    This routine will examine the X and Y coordinate and convert them into the X and Y       *
 *    cell coordinate value that cooresponds to the location.                                  *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y moouse coordinate already normalized to the radar upper left  *
 *                   corner.                                                                   *
 *                                                                                             *
 * OUTPUT:  Returns with success rating in addition, the X and Y values will now hold the      *
 *          cell coordinates of the cell the pixel offsets indicated.                          *
 *             Result 1 = click was in radar region                                            *
 *             Result 0 = click was outside radar region completly                             *
 *             Result-1 = click in radar area but not on clickable reagion of radar.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/30/1995 PWG : Created.                                                                 *
 *   07/16/1995 JLB : Recognizes when sidebar is closed now.                                   *
 *=============================================================================================*/
int RadarClass::Click_In_Radar(int &ptr_x, int &ptr_y, bool change)
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

		if ((unsigned)x < RadarWidth && (unsigned)y < RadarHeight) {
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
CELL RadarClass::Click_Cell_Calc(int x, int y)
{
	int result = Click_In_Radar(x, y, true);
	switch (result) {
		case 1:
			return(XY_Cell(x, y));

		case -1:
			return(-1);
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
bool RadarClass::Map_Cell(CELL cell, HouseClass * house, bool and_for_allies)
{
	if (DisplayClass::Map_Cell(cell, house, and_for_allies)) {
		Radar_Pixel(cell);
		return(true);
	}
	return(false);
}

void RadarClass::Cursor_Cell(CELL cell, int value)
{
	int	temp			= (*this)[cell].IsRadarCursor;

	/*
	** If this cell is not on the radar don't botther doing anything.
	*/
	if (In_Radar(cell) && temp != value) {
		/*
		**	Record the new state of this cell.
		*/
		(*this)[cell].IsRadarCursor	= value;

		/*
		**	If we are erasing then erase the cell.
		*/
////// ST 8/13/96 2:23PM
		if (value == FALSE) 	{
			Plot_Radar_Pixel(cell);
//////
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
	barlen = (barlen / ZoomFactor)+1;

	/*
	** Now lets loop through and mark the map with the proper value.
	*/
	for (int lp = 0; lp <= barlen; lp++) {
		/*
		** Do Horizontal action to upper and lower left corners.
		*/
		x = x1 + lp;
		Cursor_Cell(XY_Cell(x,y1), value);
		Cursor_Cell(XY_Cell(x,y2), value);
		/*
		** Do Horizontal Action to upper and lower right corners
		*/
		x = x2 - lp;
		Cursor_Cell(XY_Cell(x,y1), value);
		Cursor_Cell(XY_Cell(x,y2), value);
		/*
		** Do Vertical Action to left and right upper corners
		*/
		y = y1 + lp;
		Cursor_Cell(XY_Cell(x1,y), value);
		Cursor_Cell(XY_Cell(x2,y), value);

		/*
		** Do Vertical action to left and right lower corners.
		*/
		y = y2 - lp;
		Cursor_Cell(XY_Cell(x1,y), value);
		Cursor_Cell(XY_Cell(x2,y), value);
	}
}



/***********************************************************************************************
 * RadarClass::Cell_XY_To_Radar_Pixel-- Adjust the position of the radar map cursor.                    *
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
	x     = (cellx - RadarX) * ZoomFactor;
	y     = (celly - RadarY) * ZoomFactor;
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
	static int 					_last_pos = -1;
	static int 					_last_frame = -1;
	GraphicViewPortClass 	*oldpage;
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
	** find the upper left and lower right corners of the radar cursor.  Remember to adjust x2 and y2 back
	** by one pixel as they will not be pointing to the right value otherwise.  They point one cell ahead
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
#if(0)
	draw_window.Draw_Rect(x1, y1, x2, y2, WHITE);
#endif

#if(FALSE)
	if (oldpage == &SeenBuff) {
		Hide_Mouse();
	   HidPage.Blit(	SeenBuff,
							(int)(RadX + RadOffX + BaseX),
							(int)(RadY + RadOffY + BaseY),
							(int)(RadX + RadOffX + BaseX),
							(int)(RadY + RadOffY + BaseY),
							(int)draw_window.Get_Width(),
							(int)draw_window.Get_Height(),
						 	(BOOL)FALSE);

		Show_Mouse();
	}
#endif
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

	GraphicViewPortClass *oldpage= Set_Logic_Page(HidPage);
	GraphicViewPortClass draw_window(LogicPage->Get_Graphic_Buffer(),
												RadX + RadOffX + LogicPage->Get_XPos(),
												RadY + RadOffY + LogicPage->Get_YPos(),
												RadIWidth,
												RadIHeight);

	Draw_Box(RadX+RadOffX-1, RadY+RadOffY-1, RadIWidth+2, RadIHeight+2, BOXSTYLE_RAISED, true);
	draw_window.Clear();
	CC_Draw_Shape(RadarAnim, RadarAnimFrame, RadX, RadY+1, WINDOW_MAIN, SHAPE_NORMAL);

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
	if (IsRadarDeactivating) {
		RadarAnimFrame++;
		if (RadarAnimFrame == MAX_RADAR_FRAMES) {
			IsRadarDeactivating = false;
		} else {
			IsToRedraw = true;
			Flag_To_Redraw(false);
		}
	}

	DisplayClass::AI(input, x, y);
}


/***********************************************************************************************
 * RadarClass::TacticalClass::Action -- I/O function for the radar map.                        *
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
int RadarClass::TacticalClass::Action(unsigned flags, KeyNumType & key)
{
	CELL			cell;							// cell num click happened over
	int			x,y;							// Sub cell pixel coordinates.
	int			cellx,celly;				// Sub cell pixel coordinates.
	bool			shadow;						// is the cell in shadow or not
	ObjectClass *object = 0;				// what object is in the cell
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
			Map.Override_Mouse_Shape(MOUSE_NORMAL, true);
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
		x = _Kbd->MouseQX;
		y = _Kbd->MouseQY;
	} else {
		x = Get_Mouse_X();
		y = Get_Mouse_Y();
	}

	int result = Map.RadarClass::Click_In_Radar(x, y, false);

	if (result == 1) {
		cell = Map.RadarClass::Click_Cell_Calc(x, y);
		if (cell != -1) {
			//using function for IsVisible so we have different results for different players - JAS 2019/09/30
			shadow	= (!Map[cell].Is_Visible(PlayerPtr) && !Debug_Unshroud);
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
					Map.Mouse_Left_Up(shadow, object, action, true);
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
						cellx = MAX(cellx, Map.MapCellX);
						celly -= Lepton_To_Cell(Map.TacLeptonHeight) / 2;
						celly = MAX(celly, Map.MapCellY);
						cell = XY_Cell(cellx, celly);
						shadow = (!Map[cell].Is_Visible(PlayerPtr) && !Debug_Unshroud);
						Map.Set_Tactical_Position(Cell_Coord(cell));
						cell = Coord_Cell(Map.DesiredTacticalCoord);
						Map.DisplayClass::IsToRedraw = true;
						//Map.Flag_To_Redraw(false);
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
void RadarClass::Refresh_Cells(CELL cell, short const *list)
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
	int oldx, oldy;
	int newx, newy;
	int newcell;

	if (ZoomFactor != 1) {
#if(FALSE)
		oldx = (Cell_X(cell) - MapCellX) - (RadarCellWidth / 2);
		oldy = (Cell_Y(cell) - MapCellY) - (RadarCellHeight / 2);
#else
		oldx = (Cell_X(cell) - MapCellX);
		oldy = (Cell_Y(cell) - MapCellY);
#endif
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

		if (Map.IsSidebarActive&& Map.IsRadarActive) {
			int radw = RadarCellWidth-ABS(radx);			// Replicable width.
			int radh = RadarCellHeight-ABS(rady);		// Replicable height.

			if (radw < 1) forced = true;
			if (radh < 1) forced = true;

			if (!forced && (radw != RadarWidth || radh != RadarHeight)) {
				/*
				** Blit the section that is actually overlapping.
				*/
				if (OverlappedVideoBlits || !HidPage.Get_IsDirectDraw()){
			  		HidPage.Blit(HidPage,
								(((radx < 0) ? -radx : 0) * ZoomFactor) + RadX + RadOffX + BaseX,
								(((rady < 0) ? -rady : 0) * ZoomFactor) + RadY + RadOffY + BaseY,
								(((radx < 0) ? 0 : radx) * ZoomFactor) + RadX+ RadOffX + BaseX,
								(((rady < 0) ? 0 : rady) * ZoomFactor) + RadY + RadOffY + BaseY,
								radw * ZoomFactor,
								radh * ZoomFactor);
				}else{
					/*
					** System does not support overlapped blitting of video surfaces.
					** Blit it in 2 stages using an intermediate buffer.
					*/
					GraphicBufferClass temp_surface;
					temp_surface.Init((RadarWidth + 16) & 0xfffffff0,
											(RadarHeight + 16) & 0xfffffff0,
											NULL, 0, (GBC_Enum) GBC_VIDEOMEM);

					HidPage.Blit(temp_surface, (((radx < 0) ? -radx : 0) * ZoomFactor) + RadX + RadOffX + BaseX,
														(((rady < 0) ? -rady : 0) * ZoomFactor) + RadY + RadOffY + BaseY,
														0,
														0,
														RadarWidth,
														RadarHeight);

					temp_surface.Blit (HidPage,0,
														0,
														(((radx < 0) ? 0 : radx) * ZoomFactor) + RadX + RadOffX + BaseX,
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
						for (unsigned int y = 0; y < RadarCellHeight; y++ ) {
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
						for ( unsigned int x = 0; x < RadarCellWidth; x++ ) {
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
	Set_Radar_Position(Coord_Cell(DesiredTacticalCoord));
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

	int x = Cell_X(cell) - RadarX;
	int y = Cell_Y(cell) - RadarY;
	return (!((unsigned)x >= RadarCellWidth || (unsigned)y >= RadarCellHeight));

//	if (!IsZoomed) {
//		return(true);
//	}
//	return(!(((Cell_X(cell) - RadarX) > RadarCellWidth) || ((Cell_Y(cell) - RadarY) > RadarCellHeight)));
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
	int c_idx;
	HousesType house;
	HouseClass *ptr;
	int y;
	char txt[40];
	unsigned char id;
	int i;
	HousesType h;
	int kills;
	int color;
	TextPrintType style;
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	/*
	** Do nothing if the sidebar isn't there
	*/
	if (!Map.IsSidebarActive) {
		return;
	}

	CC_Draw_Shape(RadarAnim, RADAR_ACTIVATED_FRAME, RadX, RadY+1, WINDOW_MAIN, SHAPE_NORMAL);
	LogicPage->Fill_Rect(	RadX + RadOffX,
									RadY + RadOffY,
									RadX + RadOffX + RadIWidth - 1,
									RadY + RadOffY + RadIHeight - 1,
									BLACK);

	y = RadY + RadOffY;

	Fancy_Text_Print (TXT_NAME_COLON, RadX + RadOffX, y, LTGREY, TBLACK,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);


	Fancy_Text_Print (TXT_KILLS_COLON, RadX + RadOffX + RadIWidth - 2, y,
		LTGREY, TBLACK, TPF_RIGHT | TPF_6PT_GRAD | TPF_NOSHADOW | TPF_USE_GRAD_PAL);

	y += 6*factor+1;

	LogicPage->Draw_Line(RadX + RadOffX, y,
		RadX + RadOffX + RadIWidth - 1, y, LTGREY);

	y += 2*factor;

	for (house = HOUSE_MULTI1; house < (HOUSE_MULTI1 + MPlayerMax); house++) {
		ptr = HouseClass::As_Pointer(house);

		if (!ptr)
			continue;

		/*
		**	Decode this house's color
		*/
		c_idx = ptr->RemapColor;

		if (ptr->IsDefeated) {
			color = GREY;
			style = TPF_6PT_GRAD | TPF_NOSHADOW | TPF_USE_GRAD_PAL;
		} else {
			color = MPlayerTColors[c_idx];
			style = TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW;
		}

		/*
		**	Initialize our message
		*/
		txt[0] = 0;

		/*
		**	If the house is non-human, generate the message
		*/
		if (!ptr->IsHuman) {
			sprintf(txt,"%s", Text_String(TXT_COMPUTER));
		} else {

			/*
			**	For a human house:
			**	- Compute the multiplayer ID for this house
			**	- find the name for this player
			*/
			id = Build_MPlayerID (c_idx,ptr->ActLike);
			for (i = 0; i < MPlayerCount; i++) {
				if (id == MPlayerID[i]) {
					sprintf(txt,"%s",MPlayerNames[i]);
					break;
				}
			}
		}

		/*
		**	Print the player name, and the # of kills
		*/
		if (strlen(txt)) {
			if (strlen(txt) > 9) {
				txt[9] = '.';
				txt[10] = '\0';
			}
			Fancy_Text_Print (txt, RadX + RadOffX, y, color, BLACK, style);

			kills = 0;
			for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
				kills += ptr->UnitsKilled[h];
				kills += ptr->BuildingsKilled[h];
			}
			sprintf(txt, "%2d", kills);
			Fancy_Text_Print (txt, RadX + RadOffX + RadIWidth - 2, y,
				color, BLACK, style | TPF_RIGHT);

			y += 6*factor+1;

		}
	}
}
