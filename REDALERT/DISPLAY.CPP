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

/* $Header: /CounterStrike/DISPLAY.CPP 3     3/09/97 8:04p Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DISPLAY.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : October 20, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   DisplayClass::Compute_Start_Pos -- Computes player's start pos from unit coords.          *
 *   DisplayClass::AI -- Handles the maintenance tasks for the map display.                    *
 *   DisplayClass::All_To_Look -- Direct all objects to look around for the player.            *
 *   DisplayClass::Calculated_Cell -- Fetch a map cell based on specified method.              *
 *   DisplayClass::Cell_Object -- Determines what has been clicked on.                         *
 *   DisplayClass::Cell_Shadow   -- Determine what shadow icon to use for the cell.            *
 *   DisplayClass::Center_Map -- Centers the map about the currently selected objects          *
 *   DisplayClass::Click_Cell_Calc -- Determines cell from screen X & Y.                       *
 *   DisplayClass::Closest_Free_Spot -- Finds the closest cell sub spot that is free.          *
 *   DisplayClass::Coord_To_Pixel -- Determines X and Y pixel coordinates.                     *
 *   DisplayClass::Cursor_Mark -- Set or resets the cursor display flag bits.                  *
 *   DisplayClass::DisplayClass -- Default constructor for display class.                      *
 *   DisplayClass::Draw_It -- Draws the tactical map.                                          *
 *   DisplayClass::Encroach_Shadow -- Causes the shadow to creep back by one cell.             *
 *   DisplayClass::Flag_Cell -- Flag the specified cell to be redrawn.                         *
 *   DisplayClass::Flag_To_Redraw -- Flags the display so that it will be redrawn as soon as poss*
 *   DisplayClass::Get_Occupy_Dimensions -- computes width & height of the given occupy list   *
 *   DisplayClass::Good_Reinforcement_Cell -- Checks cell for renforcement legality.           *
 *   DisplayClass::In_View -- Determines if cell is visible on screen.                         *
 *   DisplayClass::Init_Clear -- Clears the display to a known state.                          *
 *   DisplayClass::Init_IO -- Creates the map's button list                                    *
 *   DisplayClass::Init_Theater -- Theater-specific initialization                             *
 *   DisplayClass::Is_Spot_Free -- Determines if cell sub spot is free of occupation.          *
 *   DisplayClass::Map_Cell -- Mark specified cell as having been mapped.                      *
 *   DisplayClass::Mouse_Left_Held -- Handles the left button held down.                       *
 *   DisplayClass::Mouse_Left_Press -- Handles the left mouse button press.                    *
 *   DisplayClass::Mouse_Left_Release -- Handles the left mouse button release.                *
 *   DisplayClass::Mouse_Left_Up -- Handles the left mouse "cruising" over the map.            *
 *   DisplayClass::Mouse_Right_Press -- Handles the right mouse button press.                  *
 *   DisplayClass::Next_Object -- Searches for next object on display.                         *
 *   DisplayClass::One_Time -- Performs any special one time initializations.                  *
 *   DisplayClass::Passes_Proximity_Check -- Determines if building placement is near friendly sq*
 *   DisplayClass::Pixel_To_Coord -- converts screen coord to COORDINATE                       *
 *   DisplayClass::Prev_Object -- Searches for the previous object on the map.                 *
 *   DisplayClass::Read_INI -- Reads map control data from INI file.                           *
 *   DisplayClass::Redraw_Icons -- Draws all terrain icons necessary.                          *
 *   DisplayClass::Redraw_Shadow -- Draw the shadow overlay.                                   *
 *   DisplayClass::Refresh_Band -- Causes all cells under the rubber band to be redrawn.       *
 *   DisplayClass::Refresh_Cells -- Redraws all cells in list.                                 *
 *   DisplayClass::Remove -- Removes a game object from the rendering system.                  *
 *   DisplayClass::Repair_Mode_Control -- Controls the repair mode.                            *
 *   DisplayClass::Scroll_Map -- Scroll the tactical map in desired direction.                 *
 *   DisplayClass::Select_These -- All selectable objects in region are selected.              *
 *   DisplayClass::Sell_Mode_Control -- Controls the sell mode.                                *
 *   DisplayClass::Set_Cursor_Pos -- Controls the display and animation of the tac cursor.     *
 *   DisplayClass::Set_Cursor_Shape -- Changes the shape of the terrain square cursor.         *
 *   DisplayClass::Set_Tactical_Position -- Sets the tactical view position.                   *
 *   DisplayClass::Set_View_Dimensions -- Sets the tactical display screen coordinates.        *
 *   DisplayClass::Shroud_Cell -- Returns the specified cell into the shrouded condition.      *
 *   DisplayClass::Submit -- Adds a game object to the map rendering system.                   *
 *   DisplayClass::TacticalClass::Action -- Processes input for the tactical map.              *
 *   DisplayClass::Text_Overlap_List -- Creates cell overlap list for specified text string.   *
 *   DisplayClass::Write_INI -- Write the map data to the INI file specified.                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"vortex.h"

/*
**	These layer control elements are used to group the displayable objects
**	so that proper overlap can be obtained.
*/
LayerClass DisplayClass::Layer[LAYER_COUNT];

/*
** Fading tables
*/
unsigned char DisplayClass::FadingBrighten[256];
unsigned char DisplayClass::FadingShade[256];
unsigned char DisplayClass::FadingWayDark[256];
unsigned char DisplayClass::FadingLight[256];
unsigned char DisplayClass::FadingGreen[256];
unsigned char DisplayClass::FadingYellow[256];
unsigned char DisplayClass::FadingRed[256];
unsigned char DisplayClass::TranslucentTable[(MAGIC_COL_COUNT+1)*256];
unsigned char DisplayClass::WhiteTranslucentTable[(1+1)*256];
unsigned char DisplayClass::MouseTranslucentTable[(4+1)*256];
void const * DisplayClass::TransIconset;
unsigned char DisplayClass::UnitShadow[(USHADOW_COL_COUNT+1)*256];
unsigned char DisplayClass::UnitShadowAir[(USHADOW_COL_COUNT+1)*256];
unsigned char DisplayClass::SpecialGhost[2*256];

void const * DisplayClass::ShadowShapes;
unsigned char DisplayClass::ShadowTrans[(SHADOW_COL_COUNT+1)*256];

/*
** Bit array of cell redraw flags
*/
BooleanVectorClass DisplayClass::CellRedraw;

/*
** The main button that intercepts user input to the map
*/
DisplayClass::TacticalClass DisplayClass::TacButton;

//
// We need a way to bypass visible view checks when we are running in the context of GlyphX without using the
// internal C&C renderer. We shouldn't know or care what the user is actually looking at
// ST - 4/17/2019 9:01AM
//
bool DisplayClass::IgnoreViewConstraints = false;


static int const TEX_X = 0;
static int const TEX_Y = 6;
static int const TEX_W = 14;

//Added for getting the input for special character keys from the client 
// - 6/26/2019 JAS 
extern bool DLL_Export_Get_Input_Key_State(KeyNumType key);


/***********************************************************************************************
 * DisplayClass::DisplayClass -- Default constructor for display class.                        *
 *                                                                                             *
 *    This constructor for the display class just initializes some of the display settings.    *
 *    Most settings are initialized with the correct values at the time that the Init function *
 *    is called. There are some cases where default values are wise and this routine fills     *
 *    those particular ones in.                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/06/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
DisplayClass::DisplayClass(void) :
	TacticalCoord(0),
	TacLeptonWidth(0),
	TacLeptonHeight(0),
	ZoneCell(0),
	ZoneOffset(0),
	CursorSize(0),
	ProximityCheck(false),
	PendingObjectPtr(0),
	PendingObject(0),
	PendingHouse(HOUSE_NONE),
	TacPixelX(0),
	TacPixelY(0),
	DesiredTacticalCoord(0),
	IsToRedraw(true),
	IsRepairMode(false),
	IsSellMode(false),
	IsTargettingMode(SPC_NONE),
	IsRubberBand(false),
	IsTentative(false),
	IsShadowPresent(false),
	BandX(0),
	BandY(0),
	NewX(0),
	NewY(0)
{
	ShadowShapes = 0;
	TransIconset = 0;

	Set_View_Dimensions(0, 8, 320/CELL_PIXEL_W, 200/CELL_PIXEL_H);
}


/***********************************************************************************************
 * DisplayClass::One_Time -- Performs any special one time initializations.                    *
 *                                                                                             *
 *    This routine is called from the game initialization process. It is to perform any one    *
 *    time initializations necessary for the map display system. It allocates the staging      *
 *    buffer needed for the radar map.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine must be called ONCE and only once.                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   05/31/1994 JLB : Handles layer system now.                                                *
 *   06/02/1994 JLB : Takes care of misc display tables and data allocation.                   *
 *=============================================================================================*/
void DisplayClass::One_Time(void)
{
	MapClass::One_Time();

	/*
	** Init the CellRedraw bit array.  Do not do this in the constructor, since the
	** BooleanVector may not have been constructed yet.
	*/
	CellRedraw.Resize(MAP_CELL_TOTAL);

	for (LayerType layer = LAYER_FIRST; layer < LAYER_COUNT; layer++) {
		Layer[layer].One_Time();
	}

	/*
	**	Load the generic transparent icon set.
	*/
	TransIconset = MFCD::Retrieve("TRANS.ICN");

	#ifndef NDEBUG
		RawFileClass file("SHADOW.SHP");
		if (file.Is_Available()) {
			ShadowShapes = Load_Alloc_Data(file);
		} else {
			ShadowShapes = MFCD::Retrieve("SHADOW.SHP");
		}
	#else
		ShadowShapes = MFCD::Retrieve("SHADOW.SHP");
	#endif

	//PG Set_View_Dimensions(0, 8 * RESFACTOR);
	Set_View_Dimensions(0, 0);
}


/***********************************************************************************************
 * DisplayClass::Init_Clear -- clears the display to a known state                             *
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
 *   03/17/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Init_Clear(void)
{
	MapClass::Init_Clear();

	/*
	** Clear any object being placed
	*/
	PendingObjectPtr = 0;
	PendingObject = 0;
	PendingHouse = HOUSE_NONE;
	CursorSize = 0;
	IsTargettingMode = SPC_NONE;
	IsRepairMode = false;
	IsRubberBand = false;
	IsTentative = false;
	IsSellMode = false;

	/*
	** Empty all the display's layers
	*/
	for (LayerType layer = LAYER_FIRST; layer < LAYER_COUNT; layer++) {
		Layer[layer].Init();
	}
}


/***********************************************************************************************
 * DisplayClass::Init_IO -- clears & re-builds the map's button list                           *
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
 *   03/17/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Init_IO(void)
{
	MapClass::Init_IO();
	/*
	** Re-attach our buttons to the main map button list, only in non-edit mode.
	*/
	if (!Debug_Map) {
		TacButton.Zap();
		Add_A_Button(TacButton);
	}
}


/***********************************************************************************************
 * DisplayClass::Init_Theater -- Performs theater-specific initialization (mixfiles, etc)      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      theater         new theater                                                            *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 BRR : Created.                                                                 *
 *   05/07/1996 JLB : Added translucent tables.                                                *
 *=============================================================================================*/
void DisplayClass::Init_Theater(TheaterType theater)
{
	char			fullname[16];
	static TLucentType const MouseCols[4] = {
		{BLACK, BLACK, 110, 0},
		{WHITE, WHITE, 110, 0},
		{LTGREY, LTGREY, 110, 0},
		{DKGREY, DKGREY, 110, 0}
	};
	static TLucentType const MagicCols[MAGIC_COL_COUNT] = {
		{32,32,110,0},
		{33,33,110,0},
		{34,34,110,0},
		{35,35,110,0},
		{36,36,110,0},
		{37,37,110,0},
		{38,38,110,0},
		{39,39,110,0},
		{BLACK, BLACK, 200, 0},
		{WHITE, BLACK, 40, 0},
		{LTGREY, BLACK, 80, 0},
		{DKGREY, BLACK, 140, 0},
		{LTGREEN,	BLACK,130,0}
	};
	static TLucentType const WhiteCols[1] = {
		{1, WHITE, 80, 0}
	};
	static TLucentType const ShadowCols[SHADOW_COL_COUNT] = {
		{WHITE+1,	BLACK,130,0},
		{WHITE,		BLACK,170,0},
		{LTGRAY,		BLACK,250,0},
		{DKGRAY,		BLACK,250,0}
	};
	static TLucentType const UShadowCols[USHADOW_COL_COUNT] = {
		{LTGREEN,	BLACK,130,0}
	};
	static TLucentType const UShadowColsAir[USHADOW_COL_COUNT] = {
		{LTGREEN,	WHITE,0,0}
	};
	static TLucentType const UShadowColsSnow[USHADOW_COL_COUNT] = {
		{LTGREEN,	BLACK,75,0}
	};

	/*
	**	Invoke parent's init routine.
	*/
	MapClass::Init_Theater(theater);

	/*
	** Save the new theater value
	*/
	Scen.Theater = theater;

	/*
	** Unload old mixfiles, and cache the new ones
	*/
	sprintf(fullname, "%s.MIX", Theaters[theater].Root);

#ifndef WIN32
LastTheater = THEATER_NONE;
#endif

	if (Scen.Theater != LastTheater) {
		if (TheaterData != NULL) {
			delete TheaterData;
		}
		TheaterData = new MFCD(fullname, &FastKey);
		assert(TheaterData != NULL);

		bool theaterload = TheaterData->Cache(TheaterBuffer);
		assert(theaterload);
//		LastTheater = Scen.Theater;
	}

	/*
	**	Load the custom palette associated with this theater.
	**	The fading palettes will have to be generated as well.
	*/
	sprintf(fullname, "%s.PAL", Theaters[theater].Root);
	PaletteClass const * ptr = (PaletteClass *)MFCD::Retrieve(fullname);

	GamePalette = * ptr;

	OriginalPalette = GamePalette;

	Build_Fading_Table(GamePalette.Get_Data(), FadingGreen, GREEN, 110);

	Build_Fading_Table(GamePalette.Get_Data(), FadingYellow, YELLOW, 140);

	Build_Fading_Table(GamePalette.Get_Data(), FadingRed, RED, 140);

	Build_Translucent_Table(GamePalette, &MouseCols[0], 4, MouseTranslucentTable);

	Build_Translucent_Table(GamePalette, &MagicCols[0], MAGIC_COL_COUNT, TranslucentTable);

	Build_Translucent_Table(GamePalette, &WhiteCols[0], 1, WhiteTranslucentTable);

	Build_Translucent_Table(GamePalette, &ShadowCols[0], SHADOW_COL_COUNT, ShadowTrans);

	Conquer_Build_Translucent_Table(GamePalette, &UShadowColsAir[0], USHADOW_COL_COUNT, UnitShadowAir);
	memcpy(&UnitShadowAir[256], ColorRemaps[PCOLOR_GOLD].RemapTable, sizeof(ColorRemaps[PCOLOR_GOLD].RemapTable));
	if (theater == THEATER_SNOW) {
		Conquer_Build_Translucent_Table(GamePalette, &UShadowColsSnow[0], USHADOW_COL_COUNT, UnitShadow);
	} else {
		Conquer_Build_Translucent_Table(GamePalette, &UShadowCols[0], USHADOW_COL_COUNT, UnitShadow);
	}

	if (theater == THEATER_SNOW) {
		Conquer_Build_Fading_Table(GamePalette, FadingShade, BLACK, 75);
	} else {
		Conquer_Build_Fading_Table(GamePalette, FadingShade, BLACK, 130);
	}

	Conquer_Build_Fading_Table(GamePalette, FadingLight, WHITE, 85);

	/*
	**	Create the shadow color used by aircraft.
	*/
	Conquer_Build_Fading_Table(GamePalette, &SpecialGhost[256], BLACK, 100);
	for (int index = 0; index < 256; index++) {
		SpecialGhost[index] = 0;
	}

	Make_Fading_Table(GamePalette, FadingBrighten, WHITE, 25);

	Make_Fading_Table(GamePalette, FadingWayDark, DKGRAY, 192);

	/*
	**	Adjust the palette according to the visual control option settings.
	*/
	Options.Fixup_Palette();
}


/***********************************************************************************************
 * DisplayClass::Text_Overlap_List -- Creates cell overlap list for specified text string.     *
 *                                                                                             *
 *    This routine is used to create an overlap list that specifies all the cells that are     *
 *    covered by the specified text string. This overlap list is used to handle map  refresh   *
 *    logic.                                                                                   *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text that would appear on the map and must have an         *
 *                   overlap list generated.                                                   *
 *                                                                                             *
 *          x,y   -- The coordinates that the text would appear (upper left corner).           *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to an overlap list that covers all cells "under" the text   *
 *          if were displayed at the coordinates specified. The list is actually a series of   *
 *          offsets from the display's upper left corner cell number.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/06/1994 JLB : Created.                                                                 *
 *   12/07/1994 JLB : Sidebar fixup.                                                           *
 *   08/13/1995 JLB : Optimized for variable sized help text.                                  *
 *=============================================================================================*/
short const * DisplayClass::Text_Overlap_List(char const * text, int x, int y) const
{
	static short _list[60];
	int count = ARRAY_SIZE(_list);

	if (text != NULL) {
		short * ptr = &_list[0];
		int len = String_Pixel_Width(text)+CELL_PIXEL_W;
		int right = TacPixelX + Lepton_To_Pixel(TacLeptonWidth);

		/*
		**	If the help text would spill into the sidebar, then flag this fact, but
		**	shorten the apparent length so that the icon list calculation will
		**	function correctly.
		*/
		if (x+len >= TacPixelX+Lepton_To_Pixel(TacLeptonWidth)) {
			len = right-x;
			*ptr++ = REFRESH_SIDEBAR;
			count--;
		}

		/*
		**	Build the list of overlap cell offset values according to the text
		**	coordinate and the length.
		*/
		if (x <= right) {
			CELL ul = Click_Cell_Calc(x, y-1);
			CELL lr = Click_Cell_Calc(x+len-1, Bound(y+24, TacPixelY, TacPixelY+Lepton_To_Pixel(TacLeptonHeight) - 1));

			if (ul == -1) ul = Click_Cell_Calc(x, y);

			if (ul != -1 && lr != -1) {
				for (int yy = Cell_Y(ul); yy <= Cell_Y(lr); yy++) {
					for (int xx = Cell_X(ul); xx <= Cell_X(lr); xx++) {
						*ptr++ = XY_Cell(xx, yy) - Coord_Cell(TacticalCoord);
						count--;
						if (count < 2) break;
					}
					if (count < 2) break;
				}
			}
		}

		*ptr = REFRESH_EOL;
	}
	return(_list);
}


/***********************************************************************************************
 * DisplayClass::Set_View_Dimensions -- Sets the tactical display screen coordinates.          *
 *                                                                                             *
 *    Use this routine to set the tactical map screen coordinates and dimensions. This routine *
 *    is typically used when the screen size or position changes as a result of the sidebar    *
 *    changing position or appearance.                                                         *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y pixel position on the screen for the tactical map upper left  *
 *                   corner.                                                                   *
 *                                                                                             *
 *          width -- The width of the tactical display (in icons). If this parameter is        *
 *                   omitted, then the width will be as wide as the screen will allow.         *
 *                                                                                             *
 *          height-- The height of the tactical display (in icons). If this parameter is       *
 *                   omitted, then the width will be as wide as the screen will allow.         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/06/1994 JLB : Created.                                                                 *
 *   06/27/1995 JLB : Adjusts tactical map position if necessary.                              *
 *=============================================================================================*/
void DisplayClass::Set_View_Dimensions(int x, int y, int width, int height)
{
	if (width == -1) {
		TacLeptonWidth = Pixel_To_Lepton(SeenBuff.Get_Width()-x);
	} else {
		TacLeptonWidth = width * CELL_LEPTON_W;
	}

	// ST - 3/1/2019 12:05PM
	// Made the below code more consistent with the width calculation. This is needed if we aren't going to draw the tabs at the top of the screen
	//
	if (height == -1) {
		TacLeptonHeight = Pixel_To_Lepton(SeenBuff.Get_Height() - y);
		//height = (SeenBuff.Get_Height()-y) / CELL_PIXEL_H;
	}
	else {
		TacLeptonHeight = height * CELL_LEPTON_H;
	}
	//TacLeptonHeight = height * CELL_LEPTON_H;

	/*
	**	Adjust the tactical cell if it is now in an invalid position
	**	because of the changed dimensions.
	*/
	int xx = 0;// Coord_X(TacticalCoord) - (MapCellX * CELL_LEPTON_W);
	int yy = 0;// Coord_Y(TacticalCoord) - (MapCellY * CELL_LEPTON_H);

	Confine_Rect(&xx, &yy, TacLeptonWidth, TacLeptonHeight, MapCellWidth * CELL_LEPTON_W, MapCellHeight * CELL_LEPTON_H);

	Set_Tactical_Position(XY_Coord(xx + (MapCellX * CELL_LEPTON_W), yy + (MapCellY * CELL_LEPTON_H)));

	TacPixelX = x;
	TacPixelY = y;
	WindowList[WINDOW_TACTICAL][WINDOWX] = x;
	WindowList[WINDOW_TACTICAL][WINDOWY] = y;
	WindowList[WINDOW_TACTICAL][WINDOWWIDTH] = Lepton_To_Pixel(TacLeptonWidth);
	WindowList[WINDOW_TACTICAL][WINDOWHEIGHT] = Lepton_To_Pixel(TacLeptonHeight);
	if (Window == WINDOW_TACTICAL) {
		Change_Window(0);
		Change_Window(Window);
	}
	IsToRedraw = true;
	Flag_To_Redraw(false);

	TacButton.X = TacPixelX;
	TacButton.Y = TacPixelY;
	TacButton.Width = Lepton_To_Pixel(TacLeptonWidth);
	TacButton.Height = Lepton_To_Pixel(TacLeptonHeight);
}


/***********************************************************************************************
 * DisplayClass::Set_Cursor_Shape -- Changes the shape of the terrain square cursor.           *
 *                                                                                             *
 *    This routine is used to set up the terrain cursor according to the size of the object    *
 *    that is to be placed down. The terrain cursor looks like an arbitrary collection of      *
 *    hatched square overlays. Typical use is when placing buildings.                          *
 *                                                                                             *
 * INPUT:   list  -- A pointer to the list that contains offsets to the cells that are to      *
 *                   be marked.                                                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/03/1994 JLB : Created.                                                                 *
 *   06/26/1995 JLB : Puts placement cursor into static buffer.                                *
 *=============================================================================================*/
void DisplayClass::Set_Cursor_Shape(short const * list)
{
	if (CursorSize) {
		Cursor_Mark(ZoneCell+ZoneOffset, false);
	}
	ZoneOffset = 0;

	if (list) {
		int	w,h;
		static short _list[50];

		memcpy(_list, list, sizeof(_list));
		CursorSize = _list;
		Get_Occupy_Dimensions (w, h, CursorSize);
		ZoneOffset = -(((h/2)*MAP_CELL_W)+(w/2));
		Cursor_Mark(ZoneCell+ZoneOffset, true);
	} else {
		CursorSize = 0;
	}
}


/***********************************************************************************************
 * DisplayClass::Passes_Proximity_Check -- Determines if building placement is near friendly sq*
 *                                                                                             *
 *    This routine is used by the building placement cursor logic to determine whether the     *
 *    at the current cursor position if the building would be adjacent to another friendly     *
 *    building. In cases where this is not true, then the building cannot be placed at all.    *
 *    This determination is returned by the function.                                          *
 *                                                                                             *
 * INPUT:   object   -- The building object that the current placement system is examining.    *
 *                                                                                             *
 *          house    -- The house to base the proximity check upon. Typically this is the      *
 *                      player's house, but in multiplay, the computer needs to check for      *
 *                      proximity as well.                                                     *
 *                                                                                             *
 *          list     -- Pointer to the building's offset list.                                 *
 *                                                                                             *
 *          trycell  -- The cell to base the offset list on.                                   *
 *                                                                                             *
 * OUTPUT:  bool; Can the pending building object be placed at the present cursor location     *
 *                checking only for proximity to friendly buildings?  If this isn't for a      *
 *                building type object, then this routine always returns true.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/06/1994 JLB : Created.                                                                 *
 *   06/07/1994 JLB : Handles concrete check.                                                  *
 *   10/11/1994 BWG : Added IsProximate check for ore refineries                               *
 *=============================================================================================*/
bool DisplayClass::Passes_Proximity_Check(ObjectTypeClass const * object, HousesType house, short const * list, CELL trycell) const
{
	short const * ptr;
	int retval = -1;
	bool noradar = false;
	//bool nomapped = false;			    // Not used. ST - 8/6/2019 10:51AM
	bool shipyard = false;

	if (house == PlayerPtr->Class->House) {
		PassedProximity = false;
	}

	/*
	** In editor mode, the proximity check always passes.
	*/
	if (Debug_Map) {
		return(true);
	}

	if (list == NULL || trycell == 0) {
		return(true);
	}

	if (object == NULL || object->What_Am_I() != RTTI_BUILDINGTYPE) {
		return(true);
	}

	BuildingTypeClass const * building = (BuildingTypeClass const *)object;

	/*
	**	Scan through all cells that the building foundation would cover. If any adjacent
	**	cells to these are of friendly persuasion, then consider the proximity check to
	**	have been a success.
	*/
	ptr = list;
//	ptr = CursorSize;
	CELL cell = trycell;
//	CELL cell = ZoneCell;
	if (building->Adjacent == 1) {
		while (*ptr != REFRESH_EOL && (retval == -1) ) {
			cell = trycell + *ptr++;
//			cell = ZoneCell + ZoneOffset + *ptr++;

			if (!In_Radar(cell)) {
				retval = false;
				noradar = true;
				break;
			}

			for (FacingType facing = FACING_FIRST; facing < FACING_COUNT; facing++) {
				CELL	newcell = Adjacent_Cell(cell, facing);

				if (!In_Radar(newcell)) continue;

				// Code has no effect. ST - 8/6/2019 10:51AM
				//if (!(*this)[newcell].IsMapped) {
				//	nomapped = true;
				//}
				BuildingClass * base = (*this)[newcell].Cell_Building();

				/*
				**	The special cell ownership flag allows building adjacent
				**	to friendly walls and bibs even though there is no official
				**	building located there.
				*/
				//BG: Modified so only walls can be placed next to walls - buildings can't.
				//JLB: Except for bibs, in which case buildings can be placed next to these.
				if (building->IsWall ||
					((*this)[newcell].Smudge != SMUDGE_NONE && SmudgeTypeClass::As_Reference((*this)[newcell].Smudge).IsBib)) {

					if ((*this)[newcell].Owner == house) {
						retval = true;
						break;
					}
				}

				// we've found a building...
				if (base != NULL && base->House->Class->House == house && base->Class->IsBase) {
					retval = true;
					break;
				}

				/* BG: modifications to allow buildings one cell away from other buildings.
				** This is done by scanning each cell that fails the check (hence getting
				** to this point) and looking at the n/s/e/w adjacent cells to see if they
				** have buildings in them.  If they do, and they match us, then succeed.
				*/
				if (retval != -1) break;

				for (FacingType newface = FACING_N; newface < FACING_COUNT; newface++) {
					CELL	newercell = Adjacent_Cell(newcell, newface);

					if (building->IsWall ||
									((*this)[newercell].Smudge != SMUDGE_NONE && SmudgeTypeClass::As_Reference((*this)[newercell].Smudge).IsBib)) {

						if ((*this)[newercell].Owner == house) {
							retval = true;
							break;
						}
					}

					TechnoClass * newbase = (*this)[newercell].Cell_Techno();

					// we've found a building...
					if (newbase != NULL && newbase->What_Am_I() == RTTI_BUILDING && newbase->House->Class->House == house && ((BuildingClass const *)newbase)->Class->IsBase) {
						retval = true;
						break;
					}
				}
				if (retval != -1) break;
			}
		}
	}

	if (retval == -1) retval = false;

	if (house == PlayerPtr->Class->House) {
		PassedProximity = (retval != false);
	}

	/*
	**	If this object has special dispensation to be placed further than one cell from
	**	other regular buildings, then check for this case now. Only bother to check if
	**	it hasn't already been given permission to be placed down.
	*/
	if (!retval && !noradar && object->What_Am_I() == RTTI_BUILDINGTYPE) {

		// For land mines, let's make it check proximity within 10 squares
		if (building->Adjacent > 1) {
			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass * obj = Buildings.Ptr(index);
				if (obj != NULL && !obj->IsInLimbo && obj->House->Class->House == house && obj->Class->IsBase) {
					int centdist = ::Distance(obj->Center_Coord(), Cell_Coord(cell));
					centdist /= CELL_LEPTON_W;
					centdist -= (obj->Class->Width() + obj->Class->Height()) / 2;
					if (centdist <= building->Adjacent) {
						retval = true;
						break;
					}
				}
			}
		}
	}

	return((bool)retval);
}


/***********************************************************************************************
 * DisplayClass::Set_Cursor_Pos -- Controls the display and animation of the tac cursor.       *
 *                                                                                             *
 *    This routine controls the location, display, and animation of the                        *
 *    tactical map cursor.                                                                     *
 *                                                                                             *
 * INPUT:   pos   -- Position to move the cursor do. If -1 is passed then                      *
 *                   the cursor will just be hidden. If the position                           *
 *                   passed is the same as the last position passed in,                        *
 *                   then animation could occur (based on timers).                             *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1991 JLB : Created.                                                                 *
 *   06/02/1994 JLB : Converted to member function.                                            *
 *   06/08/1994 JLB : If position is -1, then follow mouse.                                    *
 *   02/28/1995 JLB : Forces placement cursor to fit on map.                                   *
 *=============================================================================================*/
CELL DisplayClass::Set_Cursor_Pos(CELL pos)
{
	CELL	prevpos;			// Last position of cursor (for jump-back reasons).

	/*
	**	Follow the mouse position if no cell number is provided.
	*/
	if (pos == -1) {
		pos = Click_Cell_Calc(Get_Mouse_X(), Get_Mouse_Y());
	}

	if (CursorSize == NULL) {
		prevpos = ZoneCell;
		ZoneCell = pos;
		return(prevpos);
	}

	/*
	**	Adjusts the position so that the placement cursor is never part way off the
	**	tactical map.
	*/
	int w,h;
	Get_Occupy_Dimensions(w, h, CursorSize);

	int x = Cell_X(pos + ZoneOffset);
	int y = Cell_Y(pos + ZoneOffset);

	if (x < Coord_XCell(TacticalCoord)) x = Coord_XCell(TacticalCoord);
	if (y < Coord_YCell(TacticalCoord)) y = Coord_YCell(TacticalCoord);
	if (x+w >= Coord_XCell(TacticalCoord) + Lepton_To_Cell(TacLeptonWidth)) x = Coord_XCell(TacticalCoord)+Lepton_To_Cell(TacLeptonWidth)-w;
	if (y+h >= Coord_YCell(TacticalCoord) + Lepton_To_Cell(TacLeptonHeight)) y = Coord_YCell(TacticalCoord)+Lepton_To_Cell(TacLeptonHeight)-h;
	pos = XY_Cell(x, y) - ZoneOffset;

	/*
	** This checks to see if NO animation or drawing is to occur and, if so,
	**	exits.
	*/
	if (pos == ZoneCell) return(pos);

	prevpos = ZoneCell;

	/*
	**	If the cursor is visible, then handle the graphic update.
	**	Otherwise, just update the global position of the cursor.
	*/
	if (CursorSize != NULL) {

		/*
		** Erase the old cursor (if it exists) AND the cursor is moving.
		*/
		if (pos != ZoneCell && ZoneCell != -1) {
			Cursor_Mark(ZoneCell+ZoneOffset, false);
		}

		/*
		** Render the cursor (could just be animation).
		*/
		if (pos != -1) {
			Cursor_Mark(pos+ZoneOffset, true);
		}
	}
	ZoneCell = pos;
	ProximityCheck = Passes_Proximity_Check(PendingObject, PendingHouse, CursorSize, ZoneCell+ZoneOffset);

	return(prevpos);
}


/***********************************************************************************************
 * DisplayClass::Get_Occupy_Dimensions -- computes width & height of the given occupy list     *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      w      ptr to fill in with height                                                      *
 *      h      ptr to fill in with width                                                       *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/31/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Get_Occupy_Dimensions(int & w, int & h, short const * list) const
{
	int min_x = MAP_CELL_W;
	int max_x = -MAP_CELL_W;
	int min_y = MAP_CELL_H;
	int max_y = -MAP_CELL_H;
	int x,y;

	w = 0;
	h = 0;

	if (!list) {
		/*
		** Loop through all cell offsets, accumulating max & min x- & y-coords
		*/
		while (*list != REFRESH_EOL) {
			/*
			** Compute x & y coords of the current cell offset.  We can't use Cell_X()
			** & Cell_Y(), because they use shifts to compute the values, and if the
			** offset is negative we'll get a bogus coordinate!
			*/
			x = (*list) % MAP_CELL_W;
			y = (*list) / MAP_CELL_H;

			max_x = max(max_x, x);
			min_x = min(min_x, x);
			max_y = max(max_y, y);
			min_y = min(min_y, y);

			list++;
		}

		w = max(1, max_x - min_x + 1);
		h = min(1, max_y - min_y + 1);
	}
}


/***********************************************************************************************
 * DisplayClass::Cursor_Mark -- Set or resets the cursor display flag bits.                    *
 *                                                                                             *
 *    This routine will clear or set the cursor display bits on the map.                       *
 *    If the bit is set, then the cursor will be rendered on that map                          *
 *    icon.                                                                                    *
 *                                                                                             *
 * INPUT:   pos   -- Position of the upper left corner of the cursor.                          *
 *                                                                                             *
 *          on    -- Should the bit be turned on?                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure that every call to set the bits is matched by a                         *
 *             corresponding call to clear the bits.                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/04/1991 JLB : Created.                                                                 *
 *   06/02/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
void DisplayClass::Cursor_Mark(CELL pos, bool on)
{
	CELL const * ptr;
	CellClass * cellptr;

	if (pos == -1) return;

	/*
	**	For every cell in the CursorSize list, invoke its Redraw_Objects and
	**	toggle its IsCursorHere flag
	*/
	ptr = CursorSize;
	while (*ptr != REFRESH_EOL) {
		CELL cell = pos + *ptr++;
		if (In_Radar(cell)) {
			cellptr = &(*this)[cell];
			cellptr->Redraw_Objects();
			if (on) {
				cellptr->IsCursorHere = true;
			} else {
				cellptr->IsCursorHere = false;
			}
		}
	}

	/*
	**	For every cell in the PendingObjectPtr's Overlap_List, invoke its
	**	Redraw_Objects routine.
	*/
	if (PendingObjectPtr && PendingObjectPtr->IsActive) {
		ptr = PendingObjectPtr->Overlap_List();
		while (*ptr != REFRESH_EOL) {
			CELL cell = pos + *ptr++;
			if (In_Radar(cell)) {
				cellptr = &(*this)[cell];
				cellptr->Redraw_Objects();
			}
		}
	}
}


/***********************************************************************************************
 * DisplayClass::AI -- Handles the maintenance tasks for the map display.                      *
 *                                                                                             *
 *    This routine is called once per game display frame (15 times per second). It handles     *
 *    the mouse shape tracking and map scrolling as necessary.                                 *
 *                                                                                             *
 * INPUT:   input -- The next key just fetched from the input queue.                           *
 *                                                                                             *
 *          x,y   -- Mouse coordinates.                                                        *
 *                                                                                             *
 * OUTPUT:  Modifies the input code if necessary. When the input code is consumed, it gets     *
 *          set to 0.                                                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/01/1994 JLB : Created.                                                                 *
 *   06/02/1994 JLB : Filters mouse click input.                                               *
 *   06/07/1994 JLB : Fixed so template click will behave right.                               *
 *   10/14/1994 JLB : Changing cursor shape over target.                                       *
 *   12/31/1994 JLB : Takes mouse coordinates as parameters.                                   *
 *   06/27/1995 JLB : Breaks out of rubber band mode if mouse leaves map.                      *
 *=============================================================================================*/
void DisplayClass::AI(KeyNumType & input, int x, int y)
{
	if (
		IsRubberBand &&
		(Get_Mouse_X() < TacPixelX ||
		Get_Mouse_Y() < TacPixelY ||
		Get_Mouse_X() >= (TacPixelX + Lepton_To_Pixel(TacLeptonWidth)) ||
		Get_Mouse_Y() >= (TacPixelY + Lepton_To_Pixel(TacLeptonHeight)))) {
			Mouse_Left_Release(-1, Get_Mouse_X(), Get_Mouse_Y(), NULL, ACTION_NONE);
	}

	MapClass::AI(input, x, y);
}


/***********************************************************************************************
 * DisplayClass::Submit -- Adds a game object to the map rendering system.                     *
 *                                                                                             *
 *    This routine is used to add an arbitrary (but tangible) game object to the map. It will  *
 *    be rendered (made visible) once it is submitted to this function. This function builds   *
 *    the list of game objects that get rendered each frame as necessary. It is possible to    *
 *    submit the game object to different rendering layers. All objects in a layer get drawn   *
 *    at the same time. Using this layer method it becomes possible to have objects "below"    *
 *    other objects.                                                                           *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object to add.                                          *
 *                                                                                             *
 *          layer    -- The layer to add the object to.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   05/31/1994 JLB : Improved layer system.                                                   *
 *   05/31/1994 JLB : Sorts object position if this is for the ground layer.                   *
 *=============================================================================================*/
void DisplayClass::Submit(ObjectClass const * object, LayerType layer)
{
	if (object) {
		Layer[layer].Submit(object, (layer == LAYER_GROUND));
	}
}


/***********************************************************************************************
 * DisplayClass::Remove -- Removes a game object from the rendering system.                    *
 *                                                                                             *
 *    Every object that is to disappear from the map must be removed from the rendering        *
 *    system.                                                                                  *
 *                                                                                             *
 * INPUT:   object   -- The object to remove.                                                  *
 *                                                                                             *
 *          layer    -- The layer to remove it from.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   05/31/1994 JLB : Improved layer system.                                                   *
 *=============================================================================================*/
void DisplayClass::Remove(ObjectClass const * object, LayerType layer)
{
	assert(object != 0);
	assert(object->IsActive);

	if (object) {
		Layer[layer].Delete((ObjectClass *)object);
	}
}


/***********************************************************************************************
 * DisplayClass::Click_Cell_Calc -- Determines cell from screen X & Y.                         *
 *                                                                                             *
 *    This routine is used to determine the cell that is located at the                        *
 *    screen pixel coordinates given. Typical use is when the player                           *
 *    clicks with the mouse on the tactical map.                                               *
 *                                                                                             *
 * INPUT:   x,y   -- Screen pixel coordinates.                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with cell that is under the coordinates specified.                         *
 *          If the coordinate specified is outside of the tactical                             *
 *          map, then -1 is returned.                                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL DisplayClass::Click_Cell_Calc(int x, int y) const
{
	x -= TacPixelX;
	x = Pixel_To_Lepton(x);
	y -= TacPixelY;
	y = Pixel_To_Lepton(y);

	// Possibly ignore the view constraints if we aren't using the internal renderer. ST - 8/5/2019 11:56AM
	if (IgnoreViewConstraints || (unsigned)x < TacLeptonWidth && (unsigned)y < TacLeptonHeight) {
		COORDINATE tcoord = XY_Coord(Pixel_To_Lepton(Lepton_To_Pixel(Coord_X(TacticalCoord))), Pixel_To_Lepton(Lepton_To_Pixel(Coord_Y(TacticalCoord))));
		return(Coord_Cell(Coord_Add(tcoord, XY_Coord(x, y))));
	}
	return(-1);
}


/***********************************************************************************************
 * DisplayClass::Scroll_Map -- Scroll the tactical map in desired direction.                   *
 *                                                                                             *
 *    This routine is used to scroll the tactical map view in the desired                      *
 *    direction. It can also be used to determine if scrolling would be                        *
 *    legal without actually performing any scrolling action.                                  *
 *                                                                                             *
 * INPUT:   facing   -- The direction to scroll the tactical map.                              *
 *                                                                                             *
 *          distance -- The distance in leptons to scroll the map.                             *
 *                                                                                             *
 *          really   -- Should the map actually be scrolled?  If false,                        *
 *                      then only the legality of a scroll is checked.                         *
 *                                                                                             *
 * OUTPUT:  bool; Would scrolling in the desired direction be possible?                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   05/20/1994 JLB : Converted to member function.                                            *
 *   08/09/1995 JLB : Added distance parameter.                                                *
 *   08/10/1995 JLB : Any direction scrolling.                                                 *
 *=============================================================================================*/
bool DisplayClass::Scroll_Map(DirType facing, int & distance, bool really)
{
	/*
	**	If the distance is invalid then no further checking is required. Bail
	**	with a no-can-do flag.
	*/
	if (distance == 0) return(false);
	FacingType crude = Dir_Facing(facing);

	if (Coord_X(TacticalCoord) == Cell_To_Lepton(MapCellX) && crude != FACING_W) {
		if (crude == FACING_SW) facing = DIR_S;
		if (crude == FACING_NW) facing = DIR_N;
	}
	if (Coord_Y(TacticalCoord) == Cell_To_Lepton(MapCellY) && crude != FACING_N) {
		if (crude == FACING_NW) facing = DIR_W;
		if (crude == FACING_NE) facing = DIR_E;
	}
	if (Coord_X(TacticalCoord) + TacLeptonWidth == Cell_To_Lepton(MapCellX+MapCellWidth) && crude != FACING_E) {
		if (crude == FACING_NE) facing = DIR_N;
		if (crude == FACING_SE) facing = DIR_S;
	}
	if (Coord_Y(TacticalCoord) + TacLeptonHeight == Cell_To_Lepton(MapCellY+MapCellHeight) && crude != FACING_S) {
		if (crude == FACING_SE) facing = DIR_E;
		if (crude == FACING_SW) facing = DIR_W;
	}

	/*
	**	Determine the coordinate that it wants to scroll to.
	*/
	COORDINATE coord = Coord_Move(TacticalCoord, facing, distance);

	/*
	**	Clip the new coordinate to the edges of the game world.
	*/
	int xx = (int)(short)Coord_X(coord) - (short)Cell_To_Lepton(MapCellX);
	int yy = (int)(short)Coord_Y(coord) - (short)Cell_To_Lepton(MapCellY);
	bool shifted = Confine_Rect(&xx, &yy, TacLeptonWidth, TacLeptonHeight, Cell_To_Lepton(MapCellWidth), Cell_To_Lepton(MapCellHeight));
	if (xx < 0) {
		xx = 0;
		shifted = true;
	}
	if (yy < 0) {
		yy = 0;
		shifted = true;
	}
	coord = XY_Coord(xx + Cell_To_Lepton(MapCellX), yy + Cell_To_Lepton(MapCellY));

	/*
	**	If the desired scroll was bound by the edge of the map, then adjust the distance to more accurately
	**	reflect the actual distance moved.
	*/
	if (shifted) {
		distance = Distance(TacticalCoord, coord);
	}

	/*
	**	If the new coordinate is the same as the old, then no scrolling would occur.
	*/
	if (!distance || coord == TacticalCoord) return(false);

	/*
	**	Since the new coordinate is different than the old one, possibly adjust the real
	**	tactical map accordingly.
	*/
	if (really) {
		Set_Tactical_Position(coord);
		IsToRedraw = true;
		Flag_To_Redraw(false);

		/*
		**	Scrolled map REQUIRES all top layer units to be redrawn.
		*/
		int index;
		for (index = 0; index < Layer[LAYER_TOP].Count(); index++) {
			Layer[LAYER_TOP][index]->Mark(MARK_CHANGE);
		}
		for (index = 0; index < Layer[LAYER_AIR].Count(); index++) {
			Layer[LAYER_AIR][index]->Mark(MARK_CHANGE);
		}


	}
	return(true);
}


/***********************************************************************************************
 * DisplayClass::Refresh_Cells -- Redraws all cells in list.                                   *
 *                                                                                             *
 *    This routine is used to flag all cells in the specified list for                         *
 *    redrawing.                                                                               *
 *                                                                                             *
 * INPUT:   cell  -- The origin cell that the list is offset from.                             *
 *                                                                                             *
 *          list  -- Pointer to a list of offsets from the origin cell.                        *
 *                   Each cell so specified is flagged for redraw.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine is rather slow (by definition).                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *   08/01/1994 JLB : Simplified.                                                              *
 *=============================================================================================*/
void DisplayClass::Refresh_Cells(CELL cell, short const * list)
{
	short tlist[36];

	if (*list == REFRESH_SIDEBAR) {
		list++;
	}

	List_Copy(list, ARRAY_SIZE(tlist), tlist);
	short * tt = tlist;
	int count = 0;
	while (*tt != REFRESH_EOL) {
		if (count >= ARRAY_SIZE(tlist)) {  // Added overrun check. ST - 8/14/2019 3:14PM		
			break;
		}
		CELL newcell = cell + *tt++;
		if (In_Radar(newcell)) {
			(*this)[newcell].Redraw_Objects();
		}
		count++;
	}
}


/***********************************************************************************************
 * DisplayClass::Cell_Shadow   -- Determine what shadow icon to use for the cell.              *
 *                                                                                             *
 *    This routine will examine the specified cell and adjacent cells to                       *
 *    determine what shadow icon to use.                                                       *
 *                                                                                             *
 * INPUT:   cell     -- The cell to examine.                                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the shadow icon to use. -2= all black.                                *
 *                                                -1= map cell.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/01/1994 JLB : Created.                                                                 *
 *   04/04/1994 JLB : Revamped for new shadow icon method.                                     *
 *   04/30/1994 JLB : Converted to member function.                                            *
 *   08/05/2019  ST : Added house parameter so we can do this per player                       ** 
 *=============================================================================================*/
int DisplayClass::Cell_Shadow(CELL cell, HouseClass *house) const
{
	static char const _shadow[256]={
		-1,33, 2, 2,34,37, 2, 2,
		 4,26, 6, 6, 4,26, 6, 6,
		35,45,17,17,38,41,17,17,
		 4,26, 6, 6, 4,26, 6, 6,
		 8,21,10,10,27,31,10,10,
		12,23,14,14,12,23,14,14,
		 8,21,10,10,27,31,10,10,
		12,23,14,14,12,23,14,14,

		32,36,25,25,44,40,25,25,
		19,30,20,20,19,30,20,20,
		39,43,29,29,42,46,29,29,
		19,30,20,20,19,30,20,20,
		 8,21,10,10,27,31,10,10,
		12,23,14,14,12,23,14,14,
		 8,21,10,10,27,31,10,10,
		12,23,14,14,12,23,14,14,

		 1, 1, 3, 3,16,16, 3, 3,
		 5, 5, 7, 7, 5, 5, 7, 7,
		24,24,18,18,28,28,18,18,
		 5, 5, 7, 7, 5, 5, 7, 7,
		 9, 9,11,11,22,22,11,11,
		13,13,-2,-2,13,13,-2,-2,
		 9, 9,11,11,22,22,11,11,
		13,13,-2,-2,13,13,-2,-2,

		 1, 1, 3, 3,16,16, 3, 3,
		 5, 5, 7, 7, 5, 5, 7, 7,
		24,24,18,18,28,28,18,18,
		 5, 5, 7, 7, 5, 5, 7, 7,
		 9, 9,11,11,22,22,11,11,
		13,13,-2,-2,13,13,-2,-2,
		 9, 9,11,11,22,22,11,11,
		13,13,-2,-2,13,13,-2,-2
	};

	int index = 0, value = -1;

	/*
	**	Don't map cells that are at the edges. This solves
	**	problem of accessing cells off the bounds of map and into
	**	who-knows-what memory.
	*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if ((unsigned)(Cell_X(cell)-1) >= MAP_CELL_W-2) return(-1);
	if ((unsigned)(Cell_Y(cell)-1) >= MAP_CELL_H-2) return(-1);
#else
	if ((unsigned)(Cell_Y(cell)-1) > MAP_CELL_H-2) return(-1);
#endif
	//if ((unsigned)(Cell_Y(cell)-1) > MAP_CELL_H-2) return(-2);

	CellClass const * cellptr = &(*this)[cell];

	/*
	**	Presume solid black if that is what is here already.
	*/
	if (!cellptr->Is_Visible(house) && !cellptr->Is_Mapped(house)) value = -2;

	if (cellptr->Is_Mapped(house) /*&& !cellptr->IsVisible*/) {
		/*
		** Build an index into the lookup table using all 8 surrounding cells.
		** We're mapping a revealed cell and we only care about the existence
		** of black cells.  Bit numbering starts at the upper-right corner and
		** goes around the cell clockwise, so 0x80 = directly north.
		*/
		cell -= MAP_CELL_W + 1; cellptr -= MAP_CELL_W + 1;
		if (!cellptr->Is_Mapped(house) && In_Radar(cell)) index |= 0x40;
		cell++; cellptr++;
		if (!cellptr->Is_Mapped(house) && In_Radar(cell)) index |= 0x80;
		cell++; cellptr++;
		if (!cellptr->Is_Mapped(house) && In_Radar(cell)) index |= 0x01;
		cell += MAP_CELL_W - 2; cellptr += MAP_CELL_W - 2;
		if (!cellptr->Is_Mapped(house) && In_Radar(cell)) index |= 0x20;
		cell += 2; cellptr += 2;
		if (!cellptr->Is_Mapped(house) && In_Radar(cell)) index |= 0x02;
		cell += MAP_CELL_W - 2; cellptr += MAP_CELL_W - 2;
		if (!cellptr->Is_Mapped(house) && In_Radar(cell)) index |= 0x10;
		cell++; cellptr++;
		if (!cellptr->Is_Mapped(house) && In_Radar(cell)) index |= 0x08;
		cell++; cellptr++;
		if (!cellptr->Is_Mapped(house) && In_Radar(cell)) index |= 0x04;

		value = _shadow[index];
	}
	return(value);
}


#if (0)		                            // Old code for reference. ST - 8/15/2019 10:25AM
/***********************************************************************************************
 * DisplayClass::Cell_Shadow   -- Determine what shadow icon to use for the cell.              *
 *                                                                                             *
 *    This routine will examine the specified cell and adjacent cells to                       *
 *    determine what shadow icon to use.                                                       *
 *                                                                                             *
 * INPUT:   cell     -- The cell to examine.                                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the shadow icon to use. -2= all black.                                *
 *                                                -1= map cell.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/01/1994 JLB : Created.                                                                 *
 *   04/04/1994 JLB : Revamped for new shadow icon method.                                     *
 *   04/30/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
int DisplayClass::Cell_Shadow(CELL cell) const
{
	static char const _shadow[256]={
		-1,33, 2, 2,34,37, 2, 2,
		 4,26, 6, 6, 4,26, 6, 6,
		35,45,17,17,38,41,17,17,
		 4,26, 6, 6, 4,26, 6, 6,
		 8,21,10,10,27,31,10,10,
		12,23,14,14,12,23,14,14,
		 8,21,10,10,27,31,10,10,
		12,23,14,14,12,23,14,14,

		32,36,25,25,44,40,25,25,
		19,30,20,20,19,30,20,20,
		39,43,29,29,42,46,29,29,
		19,30,20,20,19,30,20,20,
		 8,21,10,10,27,31,10,10,
		12,23,14,14,12,23,14,14,
		 8,21,10,10,27,31,10,10,
		12,23,14,14,12,23,14,14,

		 1, 1, 3, 3,16,16, 3, 3,
		 5, 5, 7, 7, 5, 5, 7, 7,
		24,24,18,18,28,28,18,18,
		 5, 5, 7, 7, 5, 5, 7, 7,
		 9, 9,11,11,22,22,11,11,
		13,13,-2,-2,13,13,-2,-2,
		 9, 9,11,11,22,22,11,11,
		13,13,-2,-2,13,13,-2,-2,

		 1, 1, 3, 3,16,16, 3, 3,
		 5, 5, 7, 7, 5, 5, 7, 7,
		24,24,18,18,28,28,18,18,
		 5, 5, 7, 7, 5, 5, 7, 7,
		 9, 9,11,11,22,22,11,11,
		13,13,-2,-2,13,13,-2,-2,
		 9, 9,11,11,22,22,11,11,
		13,13,-2,-2,13,13,-2,-2
	};

	int index = 0, value = -1;

	/*
	**	Don't map cells that are at the top or bottom edge. This solves
	**	problem of accessing cells off the top or bottom of the map and into
	**	who-knows-what memory.
	*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if ((unsigned)(Cell_Y(cell)-1) >= MAP_CELL_H-2) return(-1);
#else
	if ((unsigned)(Cell_Y(cell)-1) > MAP_CELL_H-2) return(-1);
#endif
	//if ((unsigned)(Cell_Y(cell)-1) > MAP_CELL_H-2) return(-2);

	CellClass const * cellptr = &(*this)[cell];

	/*
	**	Presume solid black if that is what is here already.
	*/
	if (!cellptr->IsVisible && !cellptr->IsMapped) value = -2;

	if (cellptr->IsMapped /*&& !cellptr->IsVisible*/) {
		/*
		** Build an index into the lookup table using all 8 surrounding cells.
		** We're mapping a revealed cell and we only care about the existence
		** of black cells.  Bit numbering starts at the upper-right corner and
		** goes around the cell clockwise, so 0x80 = directly north.
		*/
		cellptr-= MAP_CELL_W + 1;
		if (!cellptr->IsMapped) index |= 0x40;
		cellptr++;
		if (!cellptr->IsMapped) index |= 0x80;
		cellptr++;
		if (!cellptr->IsMapped) index |= 0x01;
		cellptr += MAP_CELL_W - 2;
		if (!cellptr->IsMapped) index |= 0x20;
		cellptr += 2;
		if (!cellptr->IsMapped) index |= 0x02;
		cellptr += MAP_CELL_W - 2;
		if (!cellptr->IsMapped) index |= 0x10;
		cellptr++;
		if (!cellptr->IsMapped) index |= 0x08;
		cellptr++;
		if (!cellptr->IsMapped) index |= 0x04;

		value = _shadow[index];
	}
	return(value);
}
#endif

/***********************************************************************************************
 * DisplayClass::Map_Cell -- Mark specified cell as having been mapped.                        *
 *                                                                                             *
 *    This routine maps the specified cell. The cell must not already                          *
 *    have been mapped and the mapping player must be the human.                               *
 *    This routine will update any adjacent cell map icon as appropriate.                      *
 *                                                                                             *
 * INPUT:   cell  -- The cell to be mapped.                                                    *
 *                                                                                             *
 *          house -- The player that is doing the mapping.                                     *
 *                                                                                             *
 * OUTPUT:  bool; Was action taken to map this cell?                                           *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *   04/30/1994 JLB : Converted to member function.                                            *
 *   05/24/1994 JLB : Takes pointer to HouseClass.                                             *
 *   02/20/1996 JLB : Allied units reveal the map for the player.                              *
 *   08/05/2019  ST : Use per-player mapping so we can track the shroud for all players        *
 *=============================================================================================*/
bool DisplayClass::Map_Cell(CELL cell, HouseClass * house, bool check_radar_spied, bool and_for_allies)
{
	// OK for house not to be PlayerPtr. ST - 8/6/2019 10:05AM
	//if (house != PlayerPtr || !In_Radar(cell)) return(false);
	if (house == NULL || !In_Radar(cell)) return(false);
	
	if (!house->IsHuman) {
		if (!ShareAllyVisibility || !and_for_allies && !check_radar_spied) {
			return false;
		}
	}
	
	/*
	** First check for the condition where we're spying on a house's radar
	** facility, to see if his mapping is applicable to us.
	*/
	if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {
		// Original code. ST - 8/15/2019 10:26AM
		if (house && house != PlayerPtr) {
			if (house->RadarSpied & (1<<(PlayerPtr->Class->House))) house = PlayerPtr;
			if (Session.Type == GAME_NORMAL && house->Is_Ally(PlayerPtr)) house = PlayerPtr;
		}
	} else {
		// Version to work with any human player, not just PlayerPtr
		if (house && check_radar_spied) {
			
			for (int i=0 ; i<Session.Players.Count() ; i++) {
				HouseClass *player_ptr = HouseClass::As_Pointer(Session.Players[i]->Player.ID);
				if (player_ptr->IsHuman) {
					if (house->RadarSpied & (1<<(player_ptr->Class->House))) {
						Map_Cell(cell, player_ptr, false, false);
					}
				}
			}
		}
	}

	/*
	** Maybe also recurse to map for allies
	*/
	if (ShareAllyVisibility && and_for_allies && Session.Type == GAME_GLYPHX_MULTIPLAYER) {
		
		for (int i=0 ; i<Session.Players.Count() ; i++) {
			HouseClass *player_ptr = HouseClass::As_Pointer(Session.Players[i]->Player.ID);
			if (player_ptr && player_ptr->IsActive && player_ptr->IsHuman) {
				if (player_ptr != house && house->Is_Ally(player_ptr)) {
					Map_Cell(cell, player_ptr, check_radar_spied, false);
				}
			}
		}
	}

	CellClass * cellptr = &(*this)[cell];

	/*
	**	Don't bother remapping this cell if it is already mapped.
	*/
#if (1)	
	if (cellptr->Is_Mapped(house)) {
		if (!cellptr->Is_Visible(house)) {
			cellptr->Redraw_Objects();
		}
		return(false);
	}
#else
	if (cellptr->IsMapped) {
		if (!cellptr->IsVisible) {
			cellptr->Redraw_Objects();
		}
		return(false);
	}
#endif
	/*
	** Mark the cell as being mapped. This must be done first because
	**	if the IsVisible flag must be set, then it might affect the
	**	adjacent cell processing.
	*/
	// Set per player. ST - 8/6/2019 10:18AM
	cellptr->Set_Mapped(house);
	cellptr->Redraw_Objects();
	if (Cell_Shadow(cell, house) == -1) {
		cellptr->Set_Visible(house);
	}

	/*
	**	Check out all adjacent cells to see if they need
	**	to be mapped as well. This is necessary because of the
	**	"unique" method of showing shadowed cells. Many combinations
	**	are not allowed, and to fix this, just map the cells until
	**	all is ok.
	*/
	int xx = Cell_X(cell);
	for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir++) {
		int	shadow;
		CELL	c;
		int xdiff;

		c = Adjacent_Cell(cell, dir);

		/*
		**	Determine if the map edge has been wrapped. If so,
		**	then don't process the cell.
		*/
		if ((unsigned)c >= MAP_CELL_TOTAL) continue;
		xdiff = Cell_X(c) - xx;
		xdiff = ABS(xdiff);
		if (xdiff > 1) continue;

		CellClass * cptr = &(*this)[c];
		cptr->Redraw_Objects();

#if (1)
		// New client/server friendly code
		if (c != cell && !cptr->Is_Visible(house)) {
			shadow = Cell_Shadow(c, house);

			if (shadow == -1) {
				if (!cptr->Is_Mapped(house)) {
					Map_Cell(c, house, check_radar_spied, false);
				} else {
					cptr->Set_Visible(house);
				}
			} else {
				if (shadow != -2 && !cptr->Is_Mapped(house)) {
					Map_Cell(c, house, check_radar_spied, false);
				}
			}
		}
#else
		// Old peer/peer code
		if (c != cell && !cptr->IsVisible) {
			shadow = Cell_Shadow(c);

			if (shadow == -1) {
				if (!cptr->IsMapped) {
					Map_Cell(c, house);
				} else {
					cptr->IsVisible = true;
				}
			} else {
				if (shadow != -2 && !cptr->IsMapped) {
					Map_Cell(c, house);
				}
			}
		}
#endif
	}

	TechnoClass * tech = (*this)[cell].Cell_Techno();
	if (tech) {
		tech->Revealed(house);
	}
	return(true);
}


/***********************************************************************************************
 * DisplayClass::Coord_To_Pixel -- Determines X and Y pixel coordinates.                       *
 *                                                                                             *
 *    This is the routine that figures out the location on the screen for                      *
 *    a specified coordinate. It is one of the fundamental routines                            *
 *    necessary for rendering the game objects. It performs some quick                         *
 *    tests to see if the coordinate is in a visible region and returns                        *
 *    this check as a boolean value.                                                           *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to check.                                                  *
 *                                                                                             *
 *          x,y   -- Reference to the pixel coordinates that this                              *
 *                   coordinate would be when rendered.                                        *
 *                                                                                             *
 * OUTPUT:  bool; Is this coordinate in a visible portion of the map?                          *
 *                                                                                             *
 * WARNINGS:   If the coordinate is not in a visible portion of the                            *
 *             map, then this X and Y parameters are not set.                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *   12/15/1994 JLB : Converted to member function.                                            *
 *   01/07/1995 JLB : Uses inline functions to extract coord components.                       *
 *   08/09/1995 JLB : Uses new coordinate system.                                              *
 *=============================================================================================*/
#define	EDGE_ZONE	(CELL_LEPTON_W*2)
bool DisplayClass::Coord_To_Pixel(COORDINATE coord, int &x, int &y) const
{
	int xtac = Pixel_To_Lepton(Lepton_To_Pixel(Coord_X(TacticalCoord)));
	int xoff = Pixel_To_Lepton(Lepton_To_Pixel(Coord_X(coord)));
	xoff = (xoff + EDGE_ZONE) - xtac;

	int ytac = Pixel_To_Lepton(Lepton_To_Pixel(Coord_Y(TacticalCoord)));
	int yoff = Pixel_To_Lepton(Lepton_To_Pixel(Coord_Y(coord)));
	yoff = (yoff + EDGE_ZONE) - ytac;

	x = Lepton_To_Pixel(xoff) - CELL_PIXEL_W * 2;
	y = Lepton_To_Pixel(yoff) - CELL_PIXEL_H * 2;

	// Possibly ignore the view constraints if we aren't using the internal renderer. ST - 4/17/2019 9:06AM
	return(coord && (IgnoreViewConstraints || ((xoff <= TacLeptonWidth + EDGE_ZONE * 2) && (yoff <= TacLeptonHeight + EDGE_ZONE * 2))));
}

#if (0) //reference. ST - 5/8/2019
bool DisplayClass::Coord_To_Pixel(COORDINATE coord, int &x, int &y) const
{
	if (coord) {
		int xtac = Pixel_To_Lepton(Lepton_To_Pixel(Coord_X(TacticalCoord)));
		int xoff = Pixel_To_Lepton(Lepton_To_Pixel(Coord_X(coord)));

		xoff = (xoff+EDGE_ZONE) - xtac;
		if ((unsigned)xoff <= TacLeptonWidth + EDGE_ZONE*2) {
			int ytac = Pixel_To_Lepton(Lepton_To_Pixel(Coord_Y(TacticalCoord)));
			int yoff = Pixel_To_Lepton(Lepton_To_Pixel(Coord_Y(coord)));

			yoff = (yoff+EDGE_ZONE) - ytac;
			if ((unsigned)yoff <= TacLeptonHeight + EDGE_ZONE*2) {
				x = Lepton_To_Pixel(xoff)-CELL_PIXEL_W*2;
				y = Lepton_To_Pixel(yoff)-CELL_PIXEL_H*2;
				return(true);
			}
		}
	}
	return(false);
}
#endif

/***********************************************************************************************
 * DisplayClass::Push_Onto_TacMap -- Moves x & y coords to being on tactical map               *
 *                                                                                             *
 * This routine expects a line to be drawn between SOURCE & DEST, so it pushes the coords to   *
 * be within the region bounded by TacMapX,Y - + TacMapW,H.                                    *
 *                                                                                             *
 * INPUT:   source, dest -- References to the coordinates to check.                            *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:  bool; Are these coordinates in a visible portion of the map?                       *
 *                Returns true if the pushed source & dest are visible, but if neither are     *
 *                within the map, then it returns false.                                       *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/27/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
bool DisplayClass::Push_Onto_TacMap(COORDINATE & source, COORDINATE & dest)
{
	if (!source || !dest) return(false);

	int x1 = Coord_X(source);
	int y1 = Coord_Y(source);
	int x2 = Coord_X(dest);
	int y2 = Coord_Y(dest);
	int left = Coord_X(TacticalCoord);
	int right = Coord_X(TacticalCoord) + TacLeptonWidth;
	int top = Coord_Y(TacticalCoord);
	int bottom = Coord_Y(TacticalCoord) + TacLeptonHeight;

	if (x1 < left && x2 < left) return(false);
	if (x1 > right && x2 > right) return(false);
	if (y1 < top && y2 < top) return(false);
	if (y1 > bottom && y2 > bottom) return(false);

	x1 = Bound(x1, left, right);
	x2 = Bound(x2, left, right);
	y1 = Bound(y1, top, bottom);
	y2 = Bound(y2, top, bottom);

	source = XY_Coord(x1, y1);
	dest = XY_Coord(x2, y2);
	return(true);
}


/***********************************************************************************************
 * DisplayClass::Cell_Object -- Determines what has been clicked on.                           *
 *                                                                                             *
 *    This routine is used to determine what the player has clicked on.                        *
 *    It is passed the cell that the click was on and it then examines                         *
 *    the cell and returns with a pointer to the object that is there.                         *
 *                                                                                             *
 * INPUT:   cell  -- The cell that has been clicked upon.                                      *
 *                                                                                             *
 *          x,y   -- Optional offsets from the upper left corner of the cell to be used in     *
 *                   determining exactly which object in the cell is desired.                  *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the object that is "clickable" in                        *
 *          the specified cell.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * DisplayClass::Cell_Object(CELL cell, int x, int y) const
{
	return(*this)[cell].Cell_Object(x, y);
}


/***********************************************************************************************
 * DisplayClass::Draw_It -- Draws the tactical map.                                            *
 *                                                                                             *
 *    This will draw the tactical map at the recorded position.   This                         *
 *    routine is used whenever the tactical map moves or needs to be                           *
 *    completely redrawn. It will handle making the necessary adjustments                      *
 *    to accomodate a moving cursor.                                                           *
 *                                                                                             *
 * INPUT:      forced   -- bool; force redraw of the entire display?                           *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/15/1991 JLB : Created. (benchmark = 292)                                               *
 *   04/15/1991 JLB : Added _cell2meta[] reference array (206)                                 *
 *   04/15/1991 JLB : Added actual map reference for terrain (207)                             *
 *   04/16/1991 JLB : _cell2meta converted to int (194)                                        *
 *   04/16/1991 JLB : References actual CellIcon[] array (204)                                 *
 *   04/16/1991 JLB : Cell size increased to 16 x 16 (167)                                     *
 *   04/17/1991 JLB : Cell based tactical map rendering (165)                                  *
 *   04/22/1991 JLB : Uses Draw_Stamp() for icon rendering (426)                               *
 *   04/22/1991 JLB : Draw_Stamp uses LogicPage now (276)                                      *
 *   04/23/1991 JLB : Map active location cursor (334)                                         *
 *   05/02/1991 JLB : Added smoothing and 3 icons sets (431)                                   *
 *   05/22/1991 JLB : Broken into Draw_Map() and Refresh_Map().                                *
 *   09/14/1991 JLB : Uses Refresh_Cell when new cells scroll onto display.                    *
 *   05/12/1992 JLB : Destination page support.                                                *
 *   02/14/1994 JLB : Revamped.                                                                *
 *   05/01/1994 JLB : Converted to member function.                                            *
 *   12/15/1994 JLB : Updated to work with display hierarchy.                                  *
 *   12/24/1994 JLB : Examines redraw bit intelligently.                                       *
 *   12/24/1994 JLB : Combined with old Refresh_Map() function.                                *
 *   01/10/1995 JLB : Rubber band drawing.                                                     *
 *=============================================================================================*/
 void DisplayClass::Draw_It(bool forced)
{
	int	x,y;			// Working cell index values.

	MapClass::Draw_It(forced);

	if (IsToRedraw || forced) {
		BStart(BENCH_TACTICAL);
		IsToRedraw = false;

		/*
		**	In rubber band mode, mark all cells under the "rubber band" to be
		**	redrawn.
		*/
		Refresh_Band();

		/*
		** Mark all cells under the vortex to be redrawn
		*/
		ChronalVortex.Set_Redraw();


		/*
		** If the multiplayer message system is displaying one or more messages,
		** flag all cells covered by the messages to redraw.  This will prevent
		** messages from smearing the map if it scrolls.
		*/
		int num = Session.Messages.Num_Messages();
		if (num > 0) {
			CELL cell;
			for (cell = Coord_Cell(TacticalCoord); cell < Coord_Cell(TacticalCoord) + Lepton_To_Cell(TacLeptonWidth)+1; cell++) {
				(*this)[cell].Redraw_Objects();
			}
			for (cell = Coord_Cell(TacticalCoord) + MAP_CELL_W;
				cell < Coord_Cell(TacticalCoord) + MAP_CELL_W + Lepton_To_Cell(TacLeptonWidth)+1; cell++) {
				(*this)[cell].Redraw_Objects();
			}
			if (num > 1) {
				for (cell = Coord_Cell(TacticalCoord) + MAP_CELL_W*2;
					cell < Coord_Cell(TacticalCoord) + MAP_CELL_W*2 + Lepton_To_Cell(TacLeptonWidth)+1; cell++) {
					(*this)[cell].Redraw_Objects();
				}
			}
			if (num > 2) {
				for (cell = Coord_Cell(TacticalCoord) + MAP_CELL_W*3;
					cell < Coord_Cell(TacticalCoord) + MAP_CELL_W*3 + Lepton_To_Cell(TacLeptonWidth)+1; cell++) {
					(*this)[cell].Redraw_Objects();
				}
			}
			if (num > 3) {
				for (cell = Coord_Cell(TacticalCoord) + MAP_CELL_W*4;
					cell < Coord_Cell(TacticalCoord) + MAP_CELL_W*4 + Lepton_To_Cell(TacLeptonWidth)+1; cell++) {
					(*this)[cell].Redraw_Objects();
				}
			}
		}

		/*
		**	Check for a movement of the tactical map. If there has been some
		**	movement, then part (or all) of the icons must be redrawn.
		*/
		if (Lepton_To_Pixel(Coord_X(DesiredTacticalCoord)) != Lepton_To_Pixel(Coord_X(TacticalCoord)) ||
			Lepton_To_Pixel(Coord_Y(DesiredTacticalCoord)) != Lepton_To_Pixel(Coord_Y(TacticalCoord))) {

			int xmod = Lepton_To_Pixel(Coord_X(DesiredTacticalCoord));
			int ymod = Lepton_To_Pixel(Coord_Y(DesiredTacticalCoord));

			int oldx = Lepton_To_Pixel(Coord_X(TacticalCoord))-xmod;		// Old relative offset.
			int oldy = Lepton_To_Pixel(Coord_Y(TacticalCoord))-ymod;

			int oldw = Lepton_To_Pixel(TacLeptonWidth)-ABS(oldx);			// Replicable width.
			int oldh = Lepton_To_Pixel(TacLeptonHeight)-ABS(oldy);		// Replicable height.

			if (oldw < 1) forced = true;
			if (oldh < 1) forced = true;


#ifdef WIN32		//For WIN32 only redraw the edges of the map that move into view

			/*
			** Work out which map edges need to be redrawn
			*/
			BOOL redraw_right = (oldx < 0) ? true : false;		//Right hand edge
			BOOL redraw_left  = (oldx > 0) ? true : false;		//Left hand edge
			BOOL redraw_bottom= (oldy < 0) ? true : false;		//Bottom edge
			BOOL redraw_top	= (oldy > 0) ? true : false;		//Top edge

			/*
			**	Blit any replicable block to avoid having to drawstamp.
			*/
			if (!forced && (oldw != Lepton_To_Pixel(TacLeptonWidth) || oldh != Lepton_To_Pixel(TacLeptonHeight))) {
				Set_Cursor_Pos(-1);

				/*
				** If hid page is in video memory then blit from the seen page to avoid blitting
				**  an overlapped region.
				*/
				if (HidPage.Get_IsDirectDraw()) {
					Hide_Mouse();
							SeenBuff.Blit(HidPage,
									((oldx < 0) ? -oldx : 0) +TacPixelX,
									((oldy < 0) ? -oldy : 0) +TacPixelY,
									((oldx < 0) ? 0 : oldx) +TacPixelX,
									((oldy < 0) ? 0 : oldy) +TacPixelY,
									oldw,
									oldh);
					Show_Mouse();
				} else {
					HidPage.Blit(HidPage,
									((oldx < 0) ? -oldx : 0) +TacPixelX,
									((oldy < 0) ? -oldy : 0) +TacPixelY,
									((oldx < 0) ? 0 : oldx) +TacPixelX,
									((oldy < 0) ? 0 : oldy) +TacPixelY,
									oldw,
									oldh);
				}

			} else {
				forced = true;
			}

			if (oldx < 0) oldx = 0;
			if (oldy < 0) oldy = 0;

			/*
			** Record new map position for future reference.
			*/
			ScenarioInit++;
			Set_Tactical_Position(DesiredTacticalCoord);
			ScenarioInit--;

			if (!forced) {

				/*
				**
				**	Set the 'redraw stamp' bit for any cells that could not be copied.
				**
				*/
				int startx = -Lepton_To_Pixel(Coord_XLepton(TacticalCoord));
				int starty = -Lepton_To_Pixel(Coord_YLepton(TacticalCoord));
				oldw -= 24;
				oldh -= 24;

				if (abs(oldx) < 0x25 && abs(oldy) < 0x25) {

					/*
					** The width of the area we redraw depends on the scroll speed
					*/
					int extra_x = (abs(oldx)>=16) ? 2 : 1;
					int extra_y = (abs(oldy)>=16) ? 2 : 1;

					/*
					** Flag the cells across the top of the visible area if required
					*/
					if (redraw_top) {
						for (y = starty; y <= starty+CELL_PIXEL_H*extra_y; y += CELL_PIXEL_H) {
							for (x = startx; x <= Lepton_To_Pixel(TacLeptonWidth)+((CELL_PIXEL_W*2)); x += CELL_PIXEL_W) {
								CELL c = Click_Cell_Calc(Bound(x, 0, Lepton_To_Pixel(TacLeptonWidth)-1) + TacPixelX,
											Bound(y, 0, Lepton_To_Pixel(TacLeptonHeight)-1) + TacPixelY);

								if (c > 0) (*this)[c].Redraw_Objects(true);
							}
						}
					}

					/*
					** Flag the cells across the bottom of the visible area if required
					*/
					if (redraw_bottom) {
						for (y = Lepton_To_Pixel(TacLeptonHeight)-CELL_PIXEL_H*(1+extra_y); y <= Lepton_To_Pixel(TacLeptonHeight)+CELL_PIXEL_H*3; y += CELL_PIXEL_H) {
							for (x = startx; x <= Lepton_To_Pixel(TacLeptonWidth)+((CELL_PIXEL_W*2)); x += CELL_PIXEL_W) {
								CELL c = Click_Cell_Calc(Bound(x, 0, Lepton_To_Pixel(TacLeptonWidth)-1) + TacPixelX,
											Bound(y, 0, Lepton_To_Pixel(TacLeptonHeight)-1) + TacPixelY);

								if (c > 0) (*this)[c].Redraw_Objects(true);
							}
						}
					}

					/*
					** Flag the cells down the left of the visible area if required
					*/
					if (redraw_left) {
						for (x = startx; x <= startx + CELL_PIXEL_W*extra_x; x += CELL_PIXEL_W) {
							for (y = starty; y <= Lepton_To_Pixel(TacLeptonHeight)+((CELL_PIXEL_H*2)); y += CELL_PIXEL_H) {
								CELL c = Click_Cell_Calc(Bound(x, 0, Lepton_To_Pixel(TacLeptonWidth)-1) + TacPixelX,
											Bound(y, 0, Lepton_To_Pixel(TacLeptonHeight)-1) + TacPixelY);

								if (c > 0) (*this)[c].Redraw_Objects(true);
							}
						}
					}

					/*
					** Flag the cells down the right of the visible area if required
					*/
					if (redraw_right) {
						for (x = Lepton_To_Pixel(TacLeptonWidth)-CELL_PIXEL_W*(extra_x+1); x <= Lepton_To_Pixel(TacLeptonWidth)+CELL_PIXEL_W*3; x += CELL_PIXEL_W) {
							for (y = starty; y <= Lepton_To_Pixel(TacLeptonHeight)+((CELL_PIXEL_H*2)); y += CELL_PIXEL_H) {
								CELL c = Click_Cell_Calc(Bound(x, 0, Lepton_To_Pixel(TacLeptonWidth)-1) + TacPixelX,
											Bound(y, 0, Lepton_To_Pixel(TacLeptonHeight)-1) + TacPixelY);

								if (c > 0) (*this)[c].Redraw_Objects(true);
							}
						}
					}

				} else {

					/*
					**	Set the 'redraw stamp' bit for any cells that could not be copied.
					*/
					int startx = -Lepton_To_Pixel(Coord_XLepton(TacticalCoord));
					int starty = -Lepton_To_Pixel(Coord_YLepton(TacticalCoord));
					oldw -= 24;
					oldh -= 24;
					for (y = starty; y <= Lepton_To_Pixel(TacLeptonHeight)+((CELL_PIXEL_H*2)); y += CELL_PIXEL_H) {
						for (x = startx; x <= Lepton_To_Pixel(TacLeptonWidth)+((CELL_PIXEL_W*2)); x += CELL_PIXEL_W) {
							if (x <= oldx || x >= oldx+oldw || y <= oldy || y >= oldy+oldh) {
								CELL c = Click_Cell_Calc(Bound(x, 0, Lepton_To_Pixel(TacLeptonWidth)-1) + TacPixelX,
											Bound(y, 0, Lepton_To_Pixel(TacLeptonHeight)-1) + TacPixelY);

								if (c > 0) {
									(*this)[c].Redraw_Objects(true);
								}
							}
						}
					}
				}
			}

		} else {

			/*
			**	Set the tactical coordinate just in case the desired tactical has changed but
			**	not enough to result in any visible map change. This is likely to occur with very
			**	slow scroll rates.
			*/
			ScenarioInit++;
			if (DesiredTacticalCoord != TacticalCoord) {
				Set_Tactical_Position(DesiredTacticalCoord);
			}
			ScenarioInit--;
		}


#else	//WIN32
			/*
			**	Blit any replicable block to avoid having to drawstamp.
			*/
			if (!forced && (oldw != Lepton_To_Pixel(TacLeptonWidth) || oldh != Lepton_To_Pixel(TacLeptonHeight))) {
				Set_Cursor_Pos(-1);

				HidPage.Blit(HidPage,
								((oldx < 0) ? -oldx : 0) +TacPixelX,
								((oldy < 0) ? -oldy : 0) +TacPixelY,
								((oldx < 0) ? 0 : oldx) +TacPixelX,
								((oldy < 0) ? 0 : oldy) +TacPixelY,
								oldw,
								oldh);
			} else {
				forced = true;
			}

			if (oldx < 0) oldx = 0;
			if (oldy < 0) oldy = 0;

			/*
			** Record new map position for future reference.
			*/
			ScenarioInit++;
			Set_Tactical_Position(DesiredTacticalCoord);
			ScenarioInit--;

			if (!forced) {

				/*
				**	Set the 'redraw stamp' bit for any cells that could not be copied.
				*/
				int startx = -Lepton_To_Pixel(Coord_XLepton(TacticalCoord));
				int starty = -Lepton_To_Pixel(Coord_YLepton(TacticalCoord));
				oldw -= 24;
				oldh -= 24;
				for (y = starty; y <= Lepton_To_Pixel(TacLeptonHeight)+((CELL_PIXEL_H*2)); y += CELL_PIXEL_H) {
					for (x = startx; x <= Lepton_To_Pixel(TacLeptonWidth)+((CELL_PIXEL_W*2)); x += CELL_PIXEL_W) {
						if (x <= oldx || x >= oldx+oldw || y <= oldy || y >= oldy+oldh) {
							CELL c = Click_Cell_Calc(Bound(x, 0, Lepton_To_Pixel(TacLeptonWidth)-1) + TacPixelX,
										Bound(y, 0, Lepton_To_Pixel(TacLeptonHeight)-1) + TacPixelY);

							if (c > 0) {
								(*this)[c].Redraw_Objects(true);
							}
						}
					}
				}
			}

		} else {

			/*
			**	Set the tactical coordinate just in case the desired tactical has changed but
			**	not enough to result in any visible map change. This is likely to occur with very
			**	slow scroll rates.
			*/
			ScenarioInit++;
			if (DesiredTacticalCoord != TacticalCoord) {
				Set_Tactical_Position(DesiredTacticalCoord);
			}
			ScenarioInit--;
		}
#endif

		/*
		**	If the entire tactical map is forced to be redrawn, then set all the redraw flags
		**	and let the normal processing take care of the rest.
		*/
		if (forced) {
			CellRedraw.Set();
		}

		/*
		**	The first order of business is to redraw all the underlying icons that are
		**	flagged to be redrawn.
		*/
		if (HidPage.Lock()) {
			Redraw_Icons();

			/*
			**	Draw the infantry bodies in this special layer.
			*/
//			for (int index = 0; index < Anims.Count(); index++) {
//				AnimClass * anim = Anims.Ptr(index);
//				if (*anim >= ANIM_CORPSE1 && *anim <= ANIM_CORPSE3) {
//					anim->Render(forced);
//				}
//			}

#ifdef SORTDRAW
			/*
			** Draw the vortex effect over the terrain
			*/
			ChronalVortex.Render();

			Redraw_OIcons();
#endif

			HidPage.Unlock();
		}

#ifndef WIN32
		/*
		**	Once the icons are drawn, duplicate the bottom line of the screen into the phantom
		**	area one line below the screen. This causes the predator effect to work on any
		**	shape drawn at the bottom of the screen.
		*/
		HidPage.Blit(HidPage, 0, HidPage.Get_Height()-1, 0, HidPage.Get_Height(), HidPage.Get_Width(), 1, false);
#endif

		if (HidPage.Lock()) {

#ifndef SORTDRAW
			/*
			** Draw the vortex effect over the terrain
			*/
			ChronalVortex.Render();
#endif

			/*
			**	Redraw the game objects layer by layer. The layer drawing occurs on the ground layer
			**	first and then followed by all the layers in increasing altitude.
			*/
			BStart(BENCH_OBJECTS);
			for (LayerType layer = LAYER_FIRST; layer < LAYER_COUNT; layer++) {
				for (int index = 0; index < Layer[layer].Count(); index++) {
					ObjectClass * ptr = Layer[layer][index];

#ifdef SORTDRAW
					/*
					**	Techno objects are drawn as part of the cell redraw process since techno
					**	objects in the ground layer are handled by the Occupier and Overlapper
					**	pointer lists.
					*/
					if (!Debug_Map && ptr->Is_Techno() && layer == LAYER_GROUND && ((TechnoClass*)ptr)->Visual_Character() == VISUAL_NORMAL) continue;
#endif

//					if (ptr->What_Am_I() == RTTI_ANIM && *((AnimClass*)ptr) >= ANIM_CORPSE1 && *((AnimClass*)ptr) <= ANIM_CORPSE3) {
//						continue;
//					}
					assert(ptr->IsActive);
					ptr->Render(forced);
				}
			}
			BEnd(BENCH_OBJECTS);

	//ChronalVortex.Render();
			/*
			**	Finally, redraw the shadow overlay as necessary.
			*/
			BStart(BENCH_SHROUD);
			Redraw_Shadow();
			BEnd(BENCH_SHROUD);
		}
		HidPage.Unlock();

#ifdef SORTDRAW
		for (int index = 0; index < Layer[LAYER_GROUND].Count(); index++) {
			Layer[LAYER_GROUND][index]->IsToDisplay = false;
		}
#endif

		/*
		**	Draw the rubber band over the top of it all.
		*/
		if (IsRubberBand) {
			LogicPage->Draw_Rect(BandX+TacPixelX, BandY+TacPixelY, NewX+TacPixelX, NewY+TacPixelY, WHITE);
		}

		/*
		**	Clear the redraw flags so that normal redraw flag setting can resume.
		*/
		CellRedraw.Reset();

#ifdef SCENARIO_EDITOR
		/*
		**	If we're placing an object (PendingObject is non-NULL), and that object
		**	is NOT an icon, smudge, or overlay, draw it here.
		**	Terrain, Buildings & Aircraft aren't drawn at the cell's center coord;
		**	they're drawn at the upper left coord, so I have to AND the coord value
		**	with 0xFF00FF00 to strip off the lepton coordinates, but leave the
		**	cell coordinates.
		*/
		if (Debug_Map && PendingObjectPtr) {
			PendingObjectPtr->Coord = PendingObjectPtr->Class_Of().Coord_Fixup(Cell_Coord(ZoneCell + ZoneOffset));
			PendingObjectPtr->Render(true);
		}
#endif
		BEnd(BENCH_TACTICAL);
	}
}


/***********************************************************************************************
 * DisplayClass::Redraw_Icons -- Draws all terrain icons necessary.                            *
 *                                                                                             *
 *    This routine will redraw all of the terrain icons that are flagged                       *
 *    to be redrawn.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1994 JLB : Created.                                                                 *
 *   05/01/1994 JLB : Converted to member function.                                            *
 *   06/20/1994 JLB : Uses cell drawing support function.                                      *
 *   12/06/1994 JLB : Scans tactical view in separate row/column loops                         *
 *   12/24/1994 JLB : Uses the cell bit flag array to determine what to redraw.                *
 *=============================================================================================*/
void DisplayClass::Redraw_Icons(void)
{
	IsShadowPresent = false;
	for (int y = -Coord_YLepton(TacticalCoord); y <= TacLeptonHeight; y += CELL_LEPTON_H) {
		for (int x = -Coord_XLepton(TacticalCoord); x <= TacLeptonWidth; x += CELL_LEPTON_W) {
			COORDINATE coord = Coord_Add(TacticalCoord, XY_Coord(x, y));
			CELL cell = Coord_Cell(coord);
			coord = Coord_Whole(Cell_Coord(cell));

			/*
			**	Only cells flagged to be redraw are examined.
			*/
			if (In_View(cell) && Is_Cell_Flagged(cell)) {
				int xpixel;
				int ypixel;

				if (Coord_To_Pixel(coord, xpixel, ypixel)) {
					CellClass * cellptr = &(*this)[coord];

					/*
					**	If there is a portion of the underlying icon that could be visible,
					**	then draw it.  Also draw the cell if the shroud is off.
					*/
					//if (cellptr->IsMapped || Debug_Unshroud) {
					if (cellptr->Is_Mapped(PlayerPtr) || Debug_Unshroud) {		// Use PlayerPtr since we won't be rendering in MP. ST - 3/6/2019 2:49PM
						cellptr->Draw_It(xpixel, ypixel);
					}

					/*
					**	If any cell is not fully mapped, then flag it so that the shadow drawing
					**	process will occur.  Only draw the shadow if Debug_Unshroud is false.
					*/
					//if (!cellptr->IsVisible && !Debug_Unshroud) {
					if (!cellptr->Is_Visible(PlayerPtr) && !Debug_Unshroud) {	// Use PlayerPtr since we won't be rendering in MP. ST - 3/6/2019 2:49PM
						IsShadowPresent = true;
					}
				}
			}
		}
	}
}


#ifdef SORTDRAW
void DisplayClass::Redraw_OIcons(void)
{
	for (int y = -Coord_YLepton(TacticalCoord); y <= TacLeptonHeight; y += CELL_LEPTON_H) {
		for (int x = -Coord_XLepton(TacticalCoord); x <= TacLeptonWidth; x += CELL_LEPTON_W) {
			COORDINATE coord = Coord_Add(TacticalCoord, XY_Coord(x, y));
			CELL cell = Coord_Cell(coord);
			coord = Coord_Whole(Cell_Coord(cell));

			/*
			**	Only cells flagged to be redraw are examined.
			*/
			if (In_View(cell) && Is_Cell_Flagged(cell)) {
				int xpixel;
				int ypixel;

				if (Coord_To_Pixel(coord, xpixel, ypixel)) {
					CellClass * cellptr = &(*this)[coord];

					/*
					**	If there is a portion of the underlying icon that could be visible,
					**	then draw it.  Also draw the cell if the shroud is off.
					*/
					//if (cellptr->IsMapped || Debug_Unshroud) {
					if (cellptr->Is_Mapped(PlayerPtr) || Debug_Unshroud) {		// Use PlayerPtr since we won't be rendering in MP. ST - 3/6/2019 2:49PM
						cellptr->Draw_It(xpixel, ypixel, true);
					}
				}
			}
		}
	}
}
#endif


/***********************************************************************************************
 * DisplayClass::Redraw_Shadow -- Draw the shadow overlay.                                     *
 *                                                                                             *
 *    This routine is called after all other tactical map rendering takes place. It draws      *
 *    the shadow map over the tactical map.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *   08/06/1995 JLB : Clips the fill rect if necessary.                                        *
 *=============================================================================================*/
void DisplayClass::Redraw_Shadow(void)
{
	if (IsShadowPresent) {
		for (int y = -Coord_YLepton(TacticalCoord); y <= TacLeptonHeight; y += CELL_LEPTON_H) {
			for (int x = -Coord_XLepton(TacticalCoord); x <= TacLeptonWidth; x += CELL_LEPTON_W) {
				COORDINATE coord = Coord_Add(TacticalCoord, XY_Coord(x, y));
				CELL cell = Coord_Cell(coord);
				coord = Coord_Whole(Cell_Coord(cell));

				/*
				**	Only cells flagged to be redrawn are examined.
				*/
				if (In_View(cell) && Is_Cell_Flagged(cell)) {
					int xpixel;
					int ypixel;

					if (Coord_To_Pixel(coord, xpixel, ypixel)) {
						CellClass * cellptr = &(*this)[coord];
						//if (cellptr->IsVisible) continue;
						if (cellptr->Is_Visible(PlayerPtr)) continue;		// Use PlayerPtr since we won't be rendering in MP. ST - 8/6/2019 10:44AM
						int shadow = -2;
						//if (cellptr->IsMapped) {
						if (cellptr->Is_Mapped(PlayerPtr)) {					// Use PlayerPtr since we won't be rendering in MP. ST - 8/6/2019 10:44AM
							shadow = Cell_Shadow(cell, PlayerPtr);				// Use PlayerPtr since we won't be rendering in MP. ST - 8/6/2019 10:44AM
						}
						if (shadow >= 0) {
							CC_Draw_Shape(ShadowShapes, shadow, xpixel, ypixel, WINDOW_TACTICAL, SHAPE_GHOST, NULL, ShadowTrans);
						} else {
							if (shadow != -1) {
								int ww = CELL_PIXEL_W;
								int hh = CELL_PIXEL_H;

								if (Clip_Rect(&xpixel, &ypixel, &ww, &hh, Lepton_To_Pixel(TacLeptonWidth), Lepton_To_Pixel(TacLeptonHeight)) >= 0) {
									LogicPage->Fill_Rect(TacPixelX+xpixel, TacPixelY+ypixel, TacPixelX+xpixel+ww-1, TacPixelY+ypixel+hh-1, BLACK);
								}
							}
						}
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * DisplayClass::Next_Object -- Searches for next object on display.                           *
 *                                                                                             *
 *    This utility routine is used to find the "next" object from the object specified. This   *
 *    is typically used when <TAB> is pressed and the current object shifts.                   *
 *                                                                                             *
 * INPUT:   object   -- The current object to base the "next" calculation off of.              *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the next object. If there is no objects available,       *
 *          then NULL is returned.                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * DisplayClass::Next_Object(ObjectClass * object) const
{
	ObjectClass * firstobj = NULL;
	bool foundmatch = false;

	if (object == NULL) {
		foundmatch = true;
	}
	for (unsigned uindex = 0; uindex < (unsigned)Layer[LAYER_GROUND].Count(); uindex++) {
		ObjectClass * obj = Layer[LAYER_GROUND][uindex];

		/*
		**	Verify that the object can be selected by and is owned by the player.
		*/
		if (obj != NULL && obj->Is_Players_Army()) {
			if (firstobj == NULL) firstobj = obj;
			if (foundmatch) return(obj);
			if (object == obj) foundmatch = true;
		}
	}
	return(firstobj);
}


/***********************************************************************************************
 * DisplayClass::Prev_Object -- Searches for the previous object on the map.                   *
 *                                                                                             *
 *    This routine will search for the previous object. Previous is defined as the one listed  *
 *    before the specified object in the ground layer. If there is no specified object, then   *
 *    the last object in the ground layer is returned.                                         *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that "previous" is to be defined from.           *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the object previous to the specified one.                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * DisplayClass::Prev_Object(ObjectClass * object)  const
{
	ObjectClass * firstobj = NULL;
	bool foundmatch = false;

	if (object == NULL) {
		foundmatch = true;
	}
	for (int uindex = Layer[LAYER_GROUND].Count()-1; uindex >= 0; uindex--) {
		ObjectClass * obj = Layer[LAYER_GROUND][uindex];

		/*
		**	Verify that the object can be selected by and is owned by the player.
		*/
		if (obj != NULL && obj->Is_Players_Army()) {
			if (firstobj == NULL) firstobj = obj;
			if (foundmatch) return(obj);
			if (object == obj) foundmatch = true;
		}
	}

	return(firstobj);
}


/***********************************************************************************************
 * DisplayClass::Pixel_To_Coord -- converts screen coord to COORDINATE                         *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      x,y      pixel coordinates to convert                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      COORDINATE of pixel                                                                    *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/09/1994 BR : Created.                                                                  *
 *   12/06/1994 JLB : Uses map dimension variables in display class.                           *
 *   12/10/1994 JLB : Uses union to speed building coordinate value.                           *
 *=============================================================================================*/
COORDINATE DisplayClass::Pixel_To_Coord(int x, int y) const
{
	/*
	**	Normalize the pixel coordinates to be relative to the upper left corner
	**	of the tactical map. The coordinates are expressed in leptons.
	*/
	x -= TacPixelX;
	x = Pixel_To_Lepton(x);
	y -= TacPixelY;
	y = Pixel_To_Lepton(y);

	/*
	**	If pixel coordinate is over the tactical map, then translate it into a coordinate
	**	value. If not, then just return with NULL.
	*/
	// Possibly ignore the view constraints if we aren't using the internal renderer. ST - 8/6/2019 10:47AM
	//if ((unsigned)x < TacLeptonWidth && (unsigned)y < TacLeptonHeight) {
	if (IgnoreViewConstraints || ((unsigned)x < TacLeptonWidth && (unsigned)y < TacLeptonHeight)) {
		return(Coord_Add(TacticalCoord, XY_Coord(x, y)));
	}
	return(0);
}


/***********************************************************************************************
 * DisplayClass::Calculated_Cell -- Fetch a map cell based on specified method.                *
 *                                                                                             *
 *    Find a cell meeting the specified requirements. This function is                         *
 *    used for scenario reinforcements.                                                        *
 *                                                                                             *
 * INPUT:   dir   -- Method of picking a map cell.                                             *
 *                                                                                             *
 *          waypoint -- Closest waypoint to use for finding appropriate map edge.              *
 *                                                                                             *
 *          cell  -- Cell to find closest edge to if waypoint not specified.                   *
 *                                                                                             *
 *          loco  -- The locomotion of the reinforcements that are trying to enter.            *
 *                                                                                             *
 *          zonecheck   -- Is zone checking required?                                          *
 *                                                                                             *
 *          mzone    -- The movement zone type to check against (only if zone checking).       *
 *                                                                                             *
 * OUTPUT:  Returns with the calculated cell. If 0, then this indicates                        *
 *          that no legal cell was found.                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   04/11/1994 JLB : Revamped.                                                                *
 *   05/18/1994 JLB : Converted to member function.                                            *
 *   12/18/1995 JLB : Handles edge preference scan.                                            *
 *   06/24/1996 JLB : Removed Dune II legacy code.                                             *
 *   06/25/1996 JLB : Rewrote and greatly simplified.                                          *
 *   10/05/1996 JLB : Checks for zone and crushable status.                                    *
 *=============================================================================================*/
CELL DisplayClass::Calculated_Cell(SourceType dir, WAYPOINT waypoint, CELL cell, SpeedType loco, bool zonecheck, MZoneType mzone) const
{
	bool vert = false;
	bool horz = false;
	int x = 0;
	int y = 0;
	CELL punt = 0;			// If all else fails, return this cell location.
	int zone = -1;			// Tentative zone for legality checking.

	/*
	**	Waypoint edge detection for ground based reinforcements that have a waypoint origin are
	**	determined by finding the closest map edge to the waypoint. Reinforcement location
	**	scanning starts from that position.
	*/
	CELL trycell = -1;
	if (waypoint != -1) {
		trycell = Scen.Waypoint[waypoint];
	}
	if (trycell == -1) {
		trycell = cell;
	}

	/*
	**	If zone checking is requested, then find the correct zone to use.
	*/
	if (zonecheck && trycell != -1) {
		zone = (*this)[trycell].Zones[mzone];
	}

	/*
	**	If the cell or waypoint specified as been detected as legal, then set up the map edge
	**	scanning values accordingly.
	*/
	if (trycell != -1) {
		x = Cell_X(trycell) - MapCellX;
		x = min(x, (-Cell_X(trycell) + (MapCellX+MapCellWidth)));

		y = Cell_Y(trycell) - MapCellY;
		y = min(y, (-Cell_Y(trycell) + (MapCellY+MapCellHeight)));

		if (x < y) {
			vert = true;
			horz = false;
			if ((Cell_X(trycell)-MapCellX) < MapCellWidth/2) {
				x = -1;
			} else {
				x = MapCellWidth;
			}
			y = Cell_Y(trycell) - MapCellY;

		} else {

			vert = false;
			horz = true;
			if ((Cell_Y(trycell)-MapCellY) < MapCellHeight/2) {
				y = -1;
			} else {
				y = MapCellHeight;
			}
			x = Cell_X(trycell) - MapCellX;
		}
	}

	/*
	**	If no map edge can be inferred from the waypoint, then go with the
	**	map edge specified by the edge parameter.
	*/
	if (!vert && !horz) {
		switch (dir) {
			default:
			case SOURCE_NORTH:
				horz = true;
				y = -1;
				x = Random_Pick(0, MapCellWidth-1);
				break;

			case SOURCE_SOUTH:
				horz = true;
				y = MapCellHeight;
				x = Random_Pick(0, MapCellWidth-1);
				break;

			case SOURCE_EAST:
				vert = true;
				x = MapCellWidth;
				y = Random_Pick(0, MapCellHeight-1);
				break;

			case SOURCE_WEST:
				vert = true;
				x = -1;
				y = Random_Pick(0, MapCellHeight-1);
				break;
		}
	}

	/*
	**	Determine the default reinforcement cell if all else fails.
	*/
	punt = XY_Cell(x + MapCellX, y + MapCellY);

	/*
	**	Scan through the vertical and horizontal edges of the map looking for
	**	a relatively clear cell for object placement. The cell scanned is
	**	from the edge position specified by the X and Y variables.
	*/
	if (vert) {
		int modifier = (x > MapCellX) ? -1 : 1;

		for (int index = 0; index < MapCellHeight; index++) {
			CELL trycell = XY_Cell(x + MapCellX, ((y + index) % MapCellHeight) + MapCellY);

			if (Good_Reinforcement_Cell(trycell, trycell+modifier, loco, zone, mzone)) {
				return(trycell);
			}
		}
	}

	if (horz) {
		int modifier = (y > MapCellY) ? -MAP_CELL_W : MAP_CELL_W;

		for (int index = 0; index < MapCellWidth; index++) {
			CELL trycell = XY_Cell(((x + index) % MapCellWidth) + MapCellX, y + MapCellY);

			if (Good_Reinforcement_Cell(trycell, trycell+modifier, loco, zone, mzone)) {
				return(trycell);
			}
		}
	}

	/*
	**	If there was no success in finding a suitable reinforcement edge cell, then return
	**	with the default 'punt' cell location.
	*/
	return(punt);
}


/***********************************************************************************************
 * DisplayClass::Good_Reinforcement_Cell -- Checks cell for renforcement legality.             *
 *                                                                                             *
 *    This routine will check the secified cell (given the specified conditions) and determine *
 *    if that is a good cell for reinforcement purposes. It checks for passability of the cell *
 *    as well as zone and whether blocking walls can be crushed.                               *
 *                                                                                             *
 * INPUT:   outcell  -- The cell that is just outside the edge of the map.                     *
 *                                                                                             *
 *          incell   -- The cell that is just inside the edge of the map.                      *
 *                                                                                             *
 *          loco     -- The locomotion type of the reinforcement.                              *
 *                                                                                             *
 *          zone     -- The zone that the eventual movement destination lies. A reinforcement  *
 *                      edge must fall within the same zone.                                   *
 *                                                                                             *
 *          mzone    -- The zone check type to check against (if zone checking required)       *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified cell good for reinforcement purposes?                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool DisplayClass::Good_Reinforcement_Cell(CELL outcell, CELL incell, SpeedType loco, int zone, MZoneType mzone) const
{
	/*
	**	If the map edge location is not clear for object placement, then this is not
	**	a good cell for reinforcement purposes.
	*/
	if (!(*this)[outcell].Is_Clear_To_Move(loco, false, false)) {
		return(false);
	}

	/*
	**	If it looks like the on-map cell cannot be driven on to, then return with
	**	the failure code.
	*/
	if (!(*this)[incell].Is_Clear_To_Move(loco, false, false, zone, mzone)) {
		return(false);
	}

	/*
	**	If the reinforcement cell is already occupied, then return a failure code.
	*/
	if ((*this)[outcell].Cell_Techno() != NULL) {
		return(false);
	}
	if ((*this)[incell].Cell_Techno() != NULL) return(false);

	/*
	**	All tests have passed, return with success code.
	*/
//Mono_Printf("<%04X>\n", incell);Keyboard->Get();
	return(true);
}


/***********************************************************************************************
 * DisplayClass::Select_These -- All selectable objects in region are selected.                *
 *                                                                                             *
 *    Use this routine to simultaneously select all objects within the coordinate region       *
 *    specified. This routine is used by the multi-select rubber band handler.                 *
 *                                                                                             *
 * INPUT:   coord1   -- Coordinate of one corner of the selection region.                      *
 *                                                                                             *
 *          coord2   -- The opposite corner of the selection region.                           *
 *                                                                                             *
 *          additive -- Does this add to the existing selection or replace it.                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *   04/25/1995 JLB : Limited to non-building type.                                            *
 *   03/06/1996 JLB : Allows selection of aircraft with bounding box.                          *
 *=============================================================================================*/
static bool should_exclude_from_selection(ObjectClass* obj)
{
	return (obj->What_Am_I() == RTTI_UNIT) &&
		(((UnitClass *)obj)->Class->IsToHarvest ||
		*((UnitClass *)obj) == UNIT_MCV);
}

void DisplayClass::Select_These(COORDINATE coord1, COORDINATE coord2, bool additive)
{
	COORDINATE tcoord = TacticalCoord;	//Cell_Coord(TacticalCell) & 0xFF00FF00L;

	coord1 = Coord_Add(tcoord, coord1);
	coord2 = Coord_Add(tcoord, coord2);
	int x1 = Coord_X(coord1);
	int x2 = Coord_X(coord2);
	int y1 = Coord_Y(coord1);
	int y2 = Coord_Y(coord2);

	/*
	**	Ensure that coordinate number one represents the upper left corner
	**	and coordinate number two represents the lower right corner.
	*/
	if (x1 > x2) {
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}
	if (y1 > y2) {
		int temp = y1;
		y1 = y2;
		y2 = temp;
	}

	/*
	**	Sweep through all ground layer objects and select the ones within the
	**	bounding box.
	*/
	if (!additive) {
		Unselect_All();
	}
	AllowVoice = true;
	for (int index = 0; index < Layer[LAYER_GROUND].Count(); index++) {
		ObjectClass * obj = Layer[LAYER_GROUND][index];
		COORDINATE ocoord = obj->Center_Coord();
		int x = Coord_X(ocoord);
		int y = Coord_Y(ocoord);

		/*
		**	Only try to select objects that are allowed to be selected, and are within the bounding box.
		*/
		HouseClass * hptr = HouseClass::As_Pointer(obj->Owner());
		if (	obj->Class_Of().IsSelectable &&
				obj->What_Am_I() != RTTI_BUILDING &&
				x >= x1 && x <= x2 && y >= y1 && y <= y2) {
			bool old_allow_voice = AllowVoice;
			bool is_player_controlled = (hptr != NULL) && hptr->IsPlayerControl;
			AllowVoice &= is_player_controlled;
			if (obj->Select(true)) {
				if (is_player_controlled) {
					old_allow_voice = false;
				}
			}
			AllowVoice = old_allow_voice;
		}
	}

	/*
	**	Select any aircraft with the bounding box.
	*/
	for (int air_index = 0; air_index < Aircraft.Count(); air_index++) {
		AircraftClass * aircraft = Aircraft.Ptr(air_index);
		COORDINATE ocoord = aircraft->Center_Coord();
		int x = Coord_X(ocoord);
		int y = Coord_Y(ocoord);

		/*
		**	Only try to select objects that are allowed to be selected, and are within the bounding box.
		*/
		if (	aircraft->Class->IsSelectable &&
				!aircraft->Is_Selected_By_Player() &&
				x >= x1 && x <= x2 && y >= y1 && y <= y2) {
			bool old_allow_voice = AllowVoice;
			bool is_player_controlled = aircraft->House->IsPlayerControl;
			AllowVoice &= is_player_controlled;
			if (aircraft->Select(true)) {
				if (is_player_controlled) {
					old_allow_voice = false;
				}
			}
			AllowVoice = old_allow_voice;
		}
	}

	/*
	** If a mix of player and non-player controlled units were selected, make sure non-player controlled units are de-selected
	*/
	bool player_controlled_units = false, non_player_controlled_units = false;
	for (int i = 0; (i < CurrentObject.Count()) && (!player_controlled_units || !non_player_controlled_units); ++i) {
		HouseClass * hptr = HouseClass::As_Pointer(CurrentObject[i]->Owner());
		if (hptr->IsPlayerControl) {
			player_controlled_units = true;
		}
		else {
			non_player_controlled_units = true;
		}
	}
	if (player_controlled_units && non_player_controlled_units) {
		for (int i = 0; i < CurrentObject.Count(); ++i) {
			HouseClass * hptr = HouseClass::As_Pointer(CurrentObject[i]->Owner());
			if (!hptr->IsPlayerControl) {
				int count_before = CurrentObject.Count();
				CurrentObject[i]->Unselect();
				if (count_before <= CurrentObject.Count()) {
					GlyphX_Debug_Print("Select_These failed to remove an object");
					CurrentObject.Delete(CurrentObject[i]);
				}
				--i;
			}
		}
	}

	/*
	** If player-controlled units are non-additively selected, remove harvesters and MCVs if they aren't the only types of units selected
	*/
	if (!additive && player_controlled_units) {
		bool any_to_exclude = false, all_to_exclude = true;
		for (int i = 0; i < CurrentObject.Count(); ++i) {
			bool exclude = should_exclude_from_selection(CurrentObject[i]);
			any_to_exclude |= exclude;
			all_to_exclude &= exclude;
		}
		if (any_to_exclude && !all_to_exclude) {
			for (int i = 0; i < CurrentObject.Count(); ++i) {
				if (should_exclude_from_selection(CurrentObject[i])) {
					int count_before = CurrentObject.Count();
					CurrentObject[i]->Unselect();
					if (count_before <= CurrentObject.Count()) {
						GlyphX_Debug_Print("Select_These failed to remove an object");
						CurrentObject.Delete(CurrentObject[i]);
					}
					--i;
				}
			}
		}
	}

	AllowVoice = true;
}


/***********************************************************************************************
 * DisplayClass::Refresh_Band -- Causes all cells under the rubber band to be redrawn.         *
 *                                                                                             *
 *    Use this routine to flag all cells that are covered in some fashion by the multi-unit    *
 *    select "rubber band" to be redrawn. This is necessary whenever the rubber band changes   *
 *    size or is being removed.                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Refresh_Band(void)
{
	if (IsRubberBand) {

		/*
		**	In rubber band mode, mark all cells under the "rubber band" to be
		**	redrawn.
		*/
		int x1 = BandX+TacPixelX;
		int y1 = BandY+TacPixelY;
		int x2 = NewX+TacPixelX;
		int y2 = NewY+TacPixelY;

		if (x1 > x2) {
			int temp = x1;
			x1 = x2;
			x2 = temp;
		}
		if (y1 > y2) {
			int temp = y1;
			y1 = y2;
			y2 = temp;
		}

		CELL cell;
		for (int y = y1; y <= y2+CELL_PIXEL_H; y += CELL_PIXEL_H) {
			cell = Click_Cell_Calc(x1, Bound(y, 0, TacPixelY+Lepton_To_Pixel(TacLeptonHeight)));
			if (cell != -1) (*this)[cell].Redraw_Objects();

			cell = Click_Cell_Calc(x2, Bound(y, 0, TacPixelY+Lepton_To_Pixel(TacLeptonHeight)));
			if (cell != -1) (*this)[cell].Redraw_Objects();
		}

		for (int x = x1; x <= x2+CELL_PIXEL_W; x += CELL_PIXEL_W) {
			cell = Click_Cell_Calc(Bound(x, 0, TacPixelX+Lepton_To_Pixel(TacLeptonWidth)), y1);
			if (cell != -1) (*this)[cell].Redraw_Objects();

			cell = Click_Cell_Calc(Bound(x, 0, TacPixelX+Lepton_To_Pixel(TacLeptonWidth)), y2);
			if (cell != -1) (*this)[cell].Redraw_Objects();
		}

		/*
		**	Stretching the rubber band requires all objects to be redrawn.
		*/
		int index;
		for (index = 0; index < Layer[LAYER_TOP].Count(); index++) {
			Layer[LAYER_TOP][index]->Mark(MARK_CHANGE);
		}
		for (index = 0; index < Layer[LAYER_AIR].Count(); index++) {
			Layer[LAYER_AIR][index]->Mark(MARK_CHANGE);
		}
	}
}


/***********************************************************************************************
 * DisplayClass::TacticalClass::Action -- Processes input for the tactical map.                *
 *                                                                                             *
 *    This routine handles the input directed at the tactical map. Since input, in this        *
 *    regard, includes even the presence of the mouse over the tactical map, this routine      *
 *    is called nearly every game frame. It handles adjusting the mouse shape as well as       *
 *    giving orders to units.                                                                  *
 *                                                                                             *
 * INPUT:   flags -- The gadget event flags that triggered the call to this function.          *
 *                                                                                             *
 *          key   -- A reference to the keyboard event (if any).                               *
 *                                                                                             *
 * OUTPUT:  bool; Should processing be aborted on any succeeding buttons in the chain?         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/17/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int DisplayClass::TacticalClass::Action(unsigned flags, KeyNumType & key)
{
	int		x,y;					// Sub cell pixel coordinates.
	bool		shadow;
	ObjectClass * object = 0;
	ActionType action = ACTION_NONE;		// Action possible with currently selected object.

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
	bool edge = (y == 0 || x == 0 || x == SeenBuff.Get_Width()-1 || y == SeenBuff.Get_Height()-1);
	COORDINATE coord = Map.Pixel_To_Coord(x, y);
	CELL cell = Coord_Cell(coord);
	if (coord) {

		//shadow = (!Map[cell].IsMapped && !Debug_Unshroud);
		shadow = (!Map[cell].Is_Mapped(PlayerPtr) && !Debug_Unshroud);		// Use PlayerPtr since we won't be rendering in MP. ST - 8/6/2019 10:49AM
		x -= Map.TacPixelX;
		y -= Map.TacPixelY;

		/*
		** Cause any displayed cursor to move along with the mouse cursor.
		*/
		if (cell != Map.ZoneCell) {
			Map.Set_Cursor_Pos(cell);
		}

		/*
		**	Determine the object that the mouse is currently over.
		*/
		if (!shadow) {
			object = Map.Close_Object(coord);

			/*
			**	Special case check to ignore cloaked object if not owned by the player.
			*/
			if (object != NULL && object->Is_Techno() && !((TechnoClass *)object)->IsOwnedByPlayer && (((TechnoClass *)object)->Cloak == CLOAKED || ((TechnoClass *)object)->Techno_Type_Class()->IsInvisible)) {
				object = NULL;
			}
		}

		/*
		**	If there is a currently selected object, then the action to perform if
		**	the left mouse button were clicked must be determined.
		*/
		if (CurrentObject.Count()) {
			if (object != NULL) {
				action = Best_Object_Action(object);
			} else {
				action = Best_Object_Action(cell);
			}

		} else {

			if (object && object->Class_Of().IsSelectable) {
				action = ACTION_SELECT;
			}

			if (Map.IsRepairMode) {
				if (object && object->Owner() == PlayerPtr->Class->House && object->Can_Repair()) {
					action = ACTION_REPAIR;
				} else {
					action = ACTION_NO_REPAIR;
				}
			}

			if (Map.IsSellMode) {
				if (object && object->Owner() == PlayerPtr->Class->House && object->Can_Demolish()) {
					if (object->What_Am_I() == RTTI_BUILDING) {
						action = ACTION_SELL;
					} else {
						action = ACTION_SELL_UNIT;
					}
				} else {

					/*
					**	Check to see if the cursor is over an owned wall.
					*/
					if (Map[cell].Overlay != OVERLAY_NONE &&
						OverlayTypeClass::As_Reference(Map[cell].Overlay).IsWall &&
						Map[cell].Owner == PlayerPtr->Class->House) {
							action = ACTION_SELL;
					} else {
						action = ACTION_NO_SELL;
					}
				}
			}

			if (Map.IsTargettingMode == SPC_NUCLEAR_BOMB) {
				action = ACTION_NUKE_BOMB;
			}

			if (Map.IsTargettingMode == SPC_PARA_BOMB) {
				action = ACTION_PARA_BOMB;
			}

			if (Map.IsTargettingMode == SPC_PARA_INFANTRY) {
				action = ACTION_PARA_INFANTRY;
			}

			if (Map.IsTargettingMode == SPC_SPY_MISSION) {
				action = ACTION_SPY_MISSION;
			}

			if (Map.IsTargettingMode == SPC_IRON_CURTAIN) {
				action = ACTION_IRON_CURTAIN;
			}

			if (Map.IsTargettingMode == SPC_CHRONOSPHERE) {
				action = ACTION_CHRONOSPHERE;
			}

			if (Map.IsTargettingMode == SPC_CHRONO2) {
				action = ACTION_CHRONO2;
				if (shadow) action = ACTION_NOMOVE;
				ObjectClass const * tobject = As_Object(PlayerPtr->UnitToTeleport);

				/*
				**	Determine if the object can be teleported to the destination cell.
				*/
				if (tobject != NULL && tobject->Is_Techno()) {
					TechnoClass const * uobject = (TechnoClass const *)tobject;
					if (!uobject->Can_Teleport_Here(cell)) {
//					if (((UnitClass *)As_Object(PlayerPtr->UnitToTeleport))->Can_Enter_Cell(cell, FACING_NONE) != MOVE_OK) {
						action = ACTION_NOMOVE;
					}

				}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				else {	// If the object is no longer valid, cancel targetting mode.
					action = ACTION_NOMOVE;
					Map.IsTargettingMode = SPC_NONE;
				}
#endif
			}

			if (Map.PendingObject) {
				action = ACTION_NONE;
			}
		}

		/*
		**	Move any cursor displayed.
		*/
		if (cell != Map.ZoneCell) {
			Map.Set_Cursor_Pos(cell);
		}

		/*
		**	A right mouse button press cancels the current action or selection.
		*/
		if (flags & RIGHTPRESS) {
			Map.Mouse_Right_Press();
		}

		/*
		**	Make sure that if the mouse button has been released and the map doesn't know about it,
		**	then it must be informed. Do this by faking a mouse release event.
		*/
		if ((flags & LEFTUP) && Map.IsRubberBand) {
			flags |= LEFTRELEASE;
		}

		/*
		**	When the mouse buttons aren't pressed, only the mouse cursor shape is processed.
		**	The shape changes depending on what object the mouse is currently over and what
		**	object is currently selected.
		*/
		if (!edge) {
			if (flags & LEFTUP) {
				Map.Mouse_Left_Up(cell, shadow, object, action);
			}
		}

		/*
		**	Normal actions occur when the mouse button is released. The press event is
		**	intercepted and possible rubber-band mode is flagged.
		*/
		if (flags & LEFTRELEASE) {
			Map.Mouse_Left_Release(cell, x, y, object, action);
		}

		/*
		**	When the mouse is first pressed on the map, then record the mouse
		**	position so that a proper check before going into rubber band
		**	mode can be made. Rubber band mode starts when the mouse is
		**	held down and moved a certain minimum distance.
		*/
		if (!edge && (flags & LEFTPRESS)) {
			Map.Mouse_Left_Up(cell, shadow, object, action);
			Map.Mouse_Left_Press(x, y);
		}

		/*
		**	While the mouse is being held down, determine if rubber band mode should
		**	start. If rubber band mode is already active, then update the size
		**	and flag the map to redraw it.
		*/
		if (flags & LEFTHELD) {
			Map.Mouse_Left_Held(x, y);
		}
	}

	return(GadgetClass::Action(0, key));
}


/***********************************************************************************************
 * DisplayClass::TacticalClass::Selection_At_Mouse --  Object selection								  *
 *																															  *
 *     Selects any objects at the current mouse position.		                                *
 *																													        *
 *                                                                                             *
 * INPUT:   flags -- The gadget event flags that triggered the call to this function.          *
 *                                                                                             *
 *          key   -- A reference to the keyboard event (if any).                               *
 *                                                                                             *
 * OUTPUT:  bool; Should processing be aborted on any succeeding buttons in the chain?         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   2019/09/17  JAS					                                                              *
 *=============================================================================================*/
int DisplayClass::TacticalClass::Selection_At_Mouse(unsigned flags, KeyNumType & key)
{
	int		x, y;					// Sub cell pixel coordinates.
	bool edge = false;
	if (flags & (LEFTPRESS | LEFTRELEASE | RIGHTPRESS | RIGHTRELEASE)) {
		x = Keyboard->MouseQX;
		y = Keyboard->MouseQY;
	}
	else {
		x = Get_Mouse_X();
		y = Get_Mouse_Y();

		if (x == 0 || y == 199 || x == 319) edge = true;
	}
	COORDINATE coord = Map.Pixel_To_Coord(x, y);
	CELL cell = Coord_Cell(coord);

	if (coord) {
		bool shadow = (!Map[cell].Is_Visible(PlayerPtr) && !Debug_Unshroud);					// Use PlayerPtr since we won't be rendering in MP. ST - 3/6/2019 2:49PM
		x -= Map.TacPixelX;
		y -= Map.TacPixelY;

		/*
		** Cause any displayed cursor to move along with the mouse cursor.
		*/
		if (cell != Map.ZoneCell) {
			Map.Set_Cursor_Pos(cell);
		}

		ObjectClass* object = nullptr;

		/*
		**	Determine the object that the mouse is currently over.
		*/
		if (!shadow) {
			object = Map.Close_Object(coord);
		}

		if (object != nullptr)
		{
			bool shiftdown = DLL_Export_Get_Input_Key_State(KN_LSHIFT);

			if (shiftdown)
			{
				Map.Mouse_Left_Release(cell, x, y, object, ACTION_TOGGLE_SELECT);
			}
			else
			{
				Map.Mouse_Left_Release(cell, x, y, object, ACTION_SELECT);
			}

		}
		else
		{
			Unselect_All();
		}
	}

	return 0;
}

/***********************************************************************************************
 * DisplayClass::TacticalClass::Command_Object --  Commanding Units								     *
 *																															  *
 *     Issues a command to the currently selected unit.			                                *
 *																													        *
 *                                                                                             *
 * INPUT:   flags -- The gadget event flags that triggered the call to this function.          *
 *                                                                                             *
 *          key   -- A reference to the keyboard event (if any).                               *
 *                                                                                             *
 * OUTPUT:  bool; Should processing be aborted on any succeeding buttons in the chain?         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   2019/09/17  JAS					                                                              *
 *=============================================================================================*/
int DisplayClass::TacticalClass::Command_Object(unsigned flags, KeyNumType & key)
{
	int		x, y;					// Sub cell pixel coordinates.
	bool edge = false;
	if (flags & (LEFTPRESS | LEFTRELEASE | RIGHTPRESS | RIGHTRELEASE)) {
		x = Keyboard->MouseQX;
		y = Keyboard->MouseQY;
	}
	else {
		x = Get_Mouse_X();
		y = Get_Mouse_Y();

		if (x == 0 || y == 199 || x == 319) edge = true;
	}
	COORDINATE coord = Map.Pixel_To_Coord(x, y);
	CELL cell = Coord_Cell(coord);

	ActionType action = ACTION_NONE;

	if (coord) {
		bool shadow = (!Map[cell].Is_Mapped(PlayerPtr) && !Debug_Unshroud);					// Use PlayerPtr since we won't be rendering in MP. ST - 3/6/2019 2:49PM
		x -= Map.TacPixelX;
		y -= Map.TacPixelY;

		/*
		** Cause any displayed cursor to move along with the mouse cursor.
		*/
		if (cell != Map.ZoneCell) {
			Map.Set_Cursor_Pos(cell);
		}

		ObjectClass* object = nullptr;

		/*
		**	Determine the object that the mouse is currently over.
		*/
		if (!shadow) {
			object = Map.Close_Object(coord);
		}

		if (CurrentObject.Count()) {
			if (object) {
				action = Best_Object_Action(object);
			}
			else {
				action = Best_Object_Action(cell);
			}
		}

		if (action != ACTION_SELECT)
		{
			Map.Mouse_Left_Release(cell, x, y, object, action);
		}
	}
	return 0;
}


/***********************************************************************************************
 * DisplayClass::Mouse_Right_Press -- Handles the right mouse button press.                    *
 *                                                                                             *
 *    This routine is called when the right mouse button is pressed. This action is supposed   *
 *    to cancel whatever mode or process is active. If there is nothing to cancel, then it     *
 *    will default to unselecting any units that might be currently selected.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Mouse_Right_Press(void)
{
	if (PendingObjectPtr && PendingObjectPtr->Is_Techno()) {
		//PendingObjectPtr->Transmit_Message(RADIO_OVER_OUT);
		PendingObjectPtr = 0;
		PendingObject = 0;
		PendingHouse = HOUSE_NONE;
		Set_Cursor_Shape(0);
	} else {
		if (IsRepairMode) {
			IsRepairMode = false;
		} else {
			if (IsSellMode) {
				IsSellMode = false;
			} else {
				if (IsTargettingMode != SPC_NONE) {
					IsTargettingMode = SPC_NONE;
				} else {
					Unselect_All();
				}
			}
		}
	}

	// If it breaks... call 228.
	Set_Default_Mouse(MOUSE_NORMAL, Map.IsSmall);
}


/***********************************************************************************************
 * DisplayClass::Mouse_Left_Up -- Handles the left mouse "cruising" over the map.              *
 *                                                                                             *
 *    This routine is called continuously while the mouse is over the tactical map but there   *
 *    are no mouse buttons pressed. Typically, this adjusts the mouse shape and the pop-up     *
 *    help text.                                                                               *
 *                                                                                             *
 * INPUT:   shadow   -- Is the mouse hovering over shadowed terrain?                           *
 *                                                                                             *
 *          object   -- Pointer to the object that the mouse is currently over (may be NULL).  *
 *                                                                                             *
 *          action   -- This is the action that the currently selected object (if any) will    *
 *                      perform if the left mouse button were clicked at this location.        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/24/1995 JLB : Created.                                                                 *
 *   07/05/1995 JLB : Removed pop up help text for shadow and terrain after #3.                *
 *=============================================================================================*/
void DisplayClass::Mouse_Left_Up(CELL cell, bool shadow, ObjectClass * object, ActionType action, bool wsmall)
{
	IsTentative = false;

	TARGET target = TARGET_NONE;
	if (object != NULL) {
		target = object->As_Target();
	} else {
		if (cell != -1) {
			target = As_Target(cell);
		}
	}

	/*
	**	Don't allow selection of an object that is located in shadowed terrain.
	**	In fact, just show the normal move cursor in order to keep the shadowed
	**	terrain a mystery.
	*/
	if (shadow) {
		switch (action) {
			case ACTION_NO_DEPLOY:
				Set_Default_Mouse(MOUSE_NO_DEPLOY, wsmall);
				break;

			case ACTION_NO_ENTER:
				Set_Default_Mouse(MOUSE_NO_ENTER, wsmall);
				break;

			case ACTION_NO_GREPAIR:
				Set_Default_Mouse(MOUSE_NO_GREPAIR, wsmall);
				break;

			case ACTION_DAMAGE:
				Set_Default_Mouse(MOUSE_NORMAL, wsmall);
				break;

			case ACTION_GREPAIR:
				Set_Default_Mouse(MOUSE_NORMAL, wsmall);
				break;

			case ACTION_GUARD_AREA:
				Set_Default_Mouse(MOUSE_AREA_GUARD, wsmall);
				break;

			case ACTION_NONE:
				Set_Default_Mouse(MOUSE_NORMAL, wsmall);
				break;

			case ACTION_NO_SELL:
			case ACTION_SELL:
			case ACTION_SELL_UNIT:
				Set_Default_Mouse(MOUSE_NO_SELL_BACK, wsmall);
				break;

			case ACTION_NO_REPAIR:
			case ACTION_REPAIR:
				Set_Default_Mouse(MOUSE_NO_REPAIR, wsmall);
				break;

			case ACTION_NUKE_BOMB:
				Set_Default_Mouse(MOUSE_NUCLEAR_BOMB, wsmall);
				break;

			case ACTION_AIR_STRIKE:
			case ACTION_PARA_BOMB:
			case ACTION_PARA_INFANTRY:
			case ACTION_SPY_MISSION:
			case ACTION_IRON_CURTAIN:
				Set_Default_Mouse(MOUSE_AIR_STRIKE, wsmall);
				break;

			case ACTION_CHRONOSPHERE:
				Set_Default_Mouse(MOUSE_CHRONO_SELECT, wsmall);
				break;

			case ACTION_CHRONO2:
				Set_Default_Mouse(MOUSE_CHRONO_DEST, wsmall);
				break;

			case ACTION_HEAL:
				Set_Default_Mouse(MOUSE_HEAL, wsmall);
				break;

			case ACTION_NOMOVE:
				if (CurrentObject.Count()) {
					MouseType mouse_type = MOUSE_NO_MOVE;
					for (int i = 0; i < CurrentObject.Count(); ++i) {
						if (CurrentObject[i]->What_Am_I() != RTTI_AIRCRAFT) {
							mouse_type = MOUSE_CAN_MOVE;
							break;
						}
					}
					Set_Default_Mouse(mouse_type, wsmall);
					break;
				}
				// Fall into next case for non aircraft object types.

			default:
				Set_Default_Mouse(MOUSE_CAN_MOVE, wsmall);
				break;

		}
	} else {

		/*
		**	Change the mouse shape according to the default action that will occur
		**	if the mouse button were clicked at this location.
		*/
		switch (action) {
			case ACTION_NO_DEPLOY:
				Set_Default_Mouse(MOUSE_NO_DEPLOY, wsmall);
				break;

			case ACTION_NO_ENTER:
				Set_Default_Mouse(MOUSE_NO_ENTER, wsmall);
				break;

			case ACTION_NO_GREPAIR:
				Set_Default_Mouse(MOUSE_NO_GREPAIR, wsmall);
				break;

			case ACTION_DAMAGE:
				Set_Default_Mouse(MOUSE_DAMAGE, wsmall);
				break;

			case ACTION_GREPAIR:
				Set_Default_Mouse(MOUSE_GREPAIR, wsmall);
				break;

			case ACTION_TOGGLE_SELECT:
			case ACTION_SELECT:
				Set_Default_Mouse(MOUSE_CAN_SELECT, wsmall);
				break;

			case ACTION_MOVE:
				Set_Default_Mouse(MOUSE_CAN_MOVE, wsmall);
				break;

			case ACTION_GUARD_AREA:
				Set_Default_Mouse(MOUSE_AREA_GUARD, wsmall);
				break;

			case ACTION_ATTACK:
				if (Target_Legal(target) && CurrentObject.Count() == 1 && CurrentObject[0]->Is_Techno() && ((TechnoClass *)CurrentObject[0])->In_Range(target, 0)) {
					Set_Default_Mouse(MOUSE_STAY_ATTACK, wsmall);
					break;
				}
				// fall into next case.

			case ACTION_HARVEST:
				Set_Default_Mouse(MOUSE_CAN_ATTACK, wsmall);
				break;

			case ACTION_SABOTAGE:
				Set_Default_Mouse(MOUSE_DEMOLITIONS, wsmall);
				break;

			case ACTION_ENTER:
			case ACTION_CAPTURE:
				Set_Default_Mouse(MOUSE_ENTER, wsmall);
				break;

			case ACTION_NOMOVE:
				Set_Default_Mouse(MOUSE_NO_MOVE, wsmall);
				break;

			case ACTION_NO_SELL:
				Set_Default_Mouse(MOUSE_NO_SELL_BACK, wsmall);
				break;

			case ACTION_NO_REPAIR:
				Set_Default_Mouse(MOUSE_NO_REPAIR, wsmall);
				break;

			case ACTION_SELF:
				Set_Default_Mouse(MOUSE_DEPLOY, wsmall);
				break;

			case ACTION_REPAIR:
				Set_Default_Mouse(MOUSE_REPAIR, wsmall);
				break;

			case ACTION_SELL_UNIT:
				Set_Default_Mouse(MOUSE_SELL_UNIT, wsmall);
				break;

			case ACTION_SELL:
				Set_Default_Mouse(MOUSE_SELL_BACK, wsmall);
				break;

			case ACTION_NUKE_BOMB:
				Set_Default_Mouse(MOUSE_NUCLEAR_BOMB, wsmall);
				break;

			case ACTION_AIR_STRIKE:
			case ACTION_PARA_BOMB:
			case ACTION_PARA_INFANTRY:
			case ACTION_SPY_MISSION:
			case ACTION_IRON_CURTAIN:
				Set_Default_Mouse(MOUSE_AIR_STRIKE, wsmall);
				break;

			case ACTION_CHRONOSPHERE:
				Set_Default_Mouse(MOUSE_CHRONO_SELECT, wsmall);
				break;

			case ACTION_CHRONO2:
				Set_Default_Mouse(MOUSE_CHRONO_DEST, wsmall);
				break;

			case ACTION_HEAL:
				Set_Default_Mouse(MOUSE_HEAL, wsmall);
				break;

			case ACTION_TOGGLE_PRIMARY:
				Set_Default_Mouse(MOUSE_DEPLOY, wsmall);
				break;

			default:
				Set_Default_Mouse(MOUSE_NORMAL, wsmall);
				break;
		}
	}
#if 0
	/*
	**	Never display help text if the mouse is held over the radar map.
	*/
	if (wsmall) {
		return;
	}

	/*
	**	Give a generic help message when over shadow terrain.
	*/
	if (shadow) {
//		if (Scen.Scenario < 4) {
			Help_Text(TXT_SHADOW);
//		} else {
//			Help_Text(TXT_NONE);
//		}
	} else {

		/*
		**	If the mouse is held over objects on the map, then help text may
		**	pop up that tells what the object is. This call informs the help
		**	system of the text name for the object under the mouse.
		*/
		if (object != NULL) {
			int text;
			int color = LTGREY;

			/*
			**	Fetch the appropriate background color for help text.
			*/
			if (PlayerPtr->Is_Ally(object)) {
				color = GREEN;
			} else {
				if (object->Owner() == HOUSE_NONE || object->Owner() == HOUSE_NEUTRAL) {
					color = LTGREY;
				} else {
					color = PINK;
				}
			}

			/*
			**	Fetch the name of the object. If it is an enemy object, then
			**	the exact identity is glossed over with a generic text.
			*/
			text = object->Full_Name();
			if (object->Is_Techno() && !((TechnoTypeClass const &)object->Class_Of()).IsNominal) {

				if (!((TechnoClass *)object)->House->Is_Ally(PlayerPtr)) {
//				if (!PlayerPtr->Is_Ally(object)) {
					switch (object->What_Am_I()) {
						case RTTI_INFANTRY:
							text = TXT_ENEMY_SOLDIER;
							break;

						case RTTI_UNIT:
							text = TXT_ENEMY_VEHICLE;
							break;

						case RTTI_BUILDING:
							text = TXT_ENEMY_STRUCTURE;
							break;
					}
				}
			}

			if (/*Scen.Scenario > 3 ||*/ object->What_Am_I() != RTTI_TERRAIN) {
				Help_Text(text, -1, -1, color);
			} else {
				Help_Text(TXT_NONE);
			}
		} else {
			if ((*this)[cell].Land_Type() == LAND_TIBERIUM) {
				Help_Text(TXT_MINERALS);
			} else {
				Help_Text(TXT_NONE);
			}
		}
	}
#endif
}


/***********************************************************************************************
 * DisplayClass::Mouse_Left_Release -- Handles the left mouse button release.                  *
 *                                                                                             *
 *    This routine is called when the left mouse button is released over the tactical map.     *
 *    The release event is the workhorse of the game. Most actions occur at the moment of      *
 *    mouse release.                                                                           *
 *                                                                                             *
 * INPUT:   cell     -- The cell that the mouse is over.                                       *
 *                                                                                             *
 *          x,y      -- The mouse pixel coordinate.                                            *
 *                                                                                             *
 *          object   -- Pointer to the object that the mouse is over.                          *
 *                                                                                             *
 *          action   -- The action that the currently selected object (if any) will            *
 *                      perform.                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/24/1995 JLB : Created.                                                                 *
 *   03/27/1995 JLB : Handles sell and repair actions.                                         *
 *=============================================================================================*/
void DisplayClass::Mouse_Left_Release(CELL cell, int x, int y, ObjectClass * object, ActionType action, bool wsmall)
{
	if (PendingObjectPtr) {
		/*
		**	Try to place the pending object onto the map.
		*/
		if (ProximityCheck) {
			OutList.Add(EventClass(EventClass::PLACE, PendingObjectPtr->What_Am_I(), cell + ZoneOffset));
		} else {
			Speak(VOX_DEPLOY);
		}

	} else {

		if (IsRubberBand) {
			Refresh_Band();
			Select_These(XYP_Coord(BandX, BandY), XYP_Coord(x, y));

			Set_Default_Mouse(MOUSE_NORMAL, wsmall);
			IsRubberBand = false;
			IsTentative = false;
			Map.DisplayClass::IsToRedraw = true;
			Map.Flag_To_Redraw(false);

		} else {

			/*
			**	Toggle the select state of the object.
			*/
			if (action == ACTION_TOGGLE_SELECT) {
				if (!object || !CurrentObject.Count()) {
					action = ACTION_SELECT;
				} else {
					if (object->Is_Selected_By_Player()) {
						object->Unselect();
					} else {
						object->Select();
					}
				}
			}

			/*
			**	Selection of other object action.
			*/
			if (action == ACTION_SELECT || (action == ACTION_NONE && object && object->Class_Of().IsSelectable && !object->Is_Selected_By_Player())) {
				if (object->Is_Selected_By_Player()) {
					object->Unselect();
				}
				if (object->Select()) {
					Unselect_All_Except(object);
					Set_Default_Mouse(MOUSE_NORMAL, wsmall);
				}
			}

			/*
			**	If an action was detected as possible, then pass this action event
			**	to all selected objects.
			*/
			if (action != ACTION_NONE && action != ACTION_SELECT && action != ACTION_TOGGLE_SELECT) {

				/*
				**	Pass the action to all the selected objects. But first, redetermine
				**	what action that object should perform. This, seemingly redundant
				**	process, is necessary since multiple objects could be selected and each
				**	might perform a different action when the click occurs.
				*/
				bool doflash = true;
				AllowVoice = true;
				FormMove = false;
				FormSpeed = SPEED_WHEEL;
				FormMaxSpeed = MPH_LIGHT_SPEED;

				if ( (action == ACTION_MOVE || action == ACTION_NOMOVE) && CurrentObject.Count()) {

					/*
					** Scan all units.  If any are selected that shouldn't be, or aren't
					** selected but should be, then this is not a formation move.
					*/
					int group = 254;	// init to invalid group #

					if (CurrentObject[0]->Is_Foot()) {
						group = ((FootClass *)CurrentObject[0])->Group;
					}

					/*
					**	Presume this is a formation move unless something is detected
					**	that will prevent it.
					*/
					FormMove = true;

					/*
					**	First scan through all the selected units to make sure that they
					**	are all of the same team and have been assigned a particular formation
					*/
					for (int index = 0; index < CurrentObject.Count(); index++) {
						ObjectClass const * tobject = CurrentObject[index];

						/*
						**	Only moveable (i.e., FootClass) objects can ever be in a formation
						**	so if a selected object isn't of a FootClass type then it can't be
						**	a formation move.
						*/
						if (tobject->Is_Foot() == false) {
							FormMove = false;
							break;
						}

						/*
						**	If the object is not part of the same team as the rest of the
						**	selected group, or it just plain has never been assigned a
						**	formation offset, then it can't be a formation move.
						*/
						FootClass const * foot = (FootClass *)tobject;
						if (foot->Group != group || foot->XFormOffset == 0x80000000) {
							FormMove = false;
							break;
						}

						/*
						**	Determine the formation speed on the presumption that this
						**	will turn out to be a formation move.
						*/
						MPHType maxspeed = foot->Techno_Type_Class()->MaxSpeed;
						if (maxspeed < FormMaxSpeed) {
							FormMaxSpeed = maxspeed;
							FormSpeed = foot->Techno_Type_Class()->Speed;
						}
					}

					/*
					**	Loop through all objects (that can theoretically be part of a team) and
					**	if there are any that are part of the currently selected team, but
					**	are not currently selected themselves, then this will force this move
					**	to NOT be a formation move.
					*/
					if (FormMove) {
						for (int index = 0; index < ::Logic.Count(); index++) {
							ObjectClass const * obj = ::Logic[index];

							/*
							**	If the object is selected, then it has already been scanned
							**	by the previous loop.
							*/
							if (obj->Is_Selected_By_Player()) continue;

							/*
							**	Only consider footclass objects.
							*/
							if (!obj->Is_Foot()) continue;

							FootClass const * foot = (FootClass *)obj;

							/*
							**	Only consider objects that are owned by the player.
							*/
							if (!foot->IsOwnedByPlayer) continue;

							/*
							**	If another member of this team has been discovered and
							**	it isn't selected, then the formation move cannot take
							**	place.
							*/
							if (foot->Group == group) {
								FormMove = false;
								break;
							}
						}
					}
				}

				for (int index = 0; index < CurrentObject.Count(); index++) {
					ObjectClass * tobject = CurrentObject[index];

					if (object != NULL) {
						tobject->Active_Click_With(tobject->What_Action(object), object);
					} else {

						/*
						** Trap for formation moves: if this unit is part of a
						** formation (being part of a team qualifies) and they're
						** told to move, adjust the target destination so they stay
						** in formation when they arrive.
						*/
						CELL newmove = cell;
						int whatami = tobject->What_Am_I();
						if (action == ACTION_MOVE && tobject->Is_Foot()) {
							int oldisform;
							FootClass * foot = (FootClass *)tobject;
							oldisform = foot->IsFormationMove;
							foot->IsFormationMove = FormMove;
							if (FormMove && foot->Group != 255 ) {
								newmove = foot->Adjust_Dest(cell);
							}
							foot->IsFormationMove = oldisform;
						}
						tobject->Active_Click_With(tobject->What_Action(cell), newmove);
					}
					AllowVoice = false;
				}
				AllowVoice = true;
				FormMove = false;

				if (action == ACTION_REPAIR && object->What_Am_I() == RTTI_BUILDING) {
					OutList.Add(EventClass(EventClass::REPAIR, TargetClass(object)));
				}
				if (action == ACTION_SELL_UNIT && object) {
					switch (object->What_Am_I()) {
						case RTTI_AIRCRAFT:
						case RTTI_UNIT:
							OutList.Add(EventClass(EventClass::SELL, TargetClass(object)));
							break;

						default:
							break;
					}

				}
				if (action == ACTION_SELL) {
					if (object) {
						OutList.Add(EventClass(EventClass::SELL, TargetClass(object)));
					} else {
						OutList.Add(EventClass(EventClass::SELLCELL, cell));
//						OutList.Add(EventClass(EventClass::SELL, ::As_Target(cell)));
					}
				}

				if (action == ACTION_NUKE_BOMB) {
					OutList.Add(EventClass(EventClass::SPECIAL_PLACE, SPC_NUCLEAR_BOMB, cell));
				}

				if (action == ACTION_PARA_BOMB) {
					OutList.Add(EventClass(EventClass::SPECIAL_PLACE, SPC_PARA_BOMB, cell));
				}
				if (action == ACTION_PARA_INFANTRY) {
					OutList.Add(EventClass(EventClass::SPECIAL_PLACE, SPC_PARA_INFANTRY, cell));
				}
				if (action == ACTION_SPY_MISSION) {
					OutList.Add(EventClass(EventClass::SPECIAL_PLACE, SPC_SPY_MISSION, cell));
				}
				if (action == ACTION_IRON_CURTAIN) {
					OutList.Add(EventClass(EventClass::SPECIAL_PLACE, SPC_IRON_CURTAIN, cell));
				}
				if (action == ACTION_CHRONOSPHERE) {
					OutList.Add(EventClass(EventClass::SPECIAL_PLACE, SPC_CHRONOSPHERE, cell));
				}
				if (action == ACTION_CHRONO2) {
					OutList.Add(EventClass(EventClass::SPECIAL_PLACE, SPC_CHRONO2, cell));
				}
			}

			IsTentative = false;
		}
	}
}


/***********************************************************************************************
 * DisplayClass::Mouse_Left_Press -- Handles the left mouse button press.                      *
 *                                                                                             *
 *    Handle the left mouse button press while over the tactical map. If it isn't is           *
 *    repair or sell mode, then a tentative transition to rubber band mode is flagged. If the  *
 *    mouse moves a sufficient distance from this recorded position, then rubber band mode     *
 *    is officially started.                                                                   *
 *                                                                                             *
 * INPUT:   x,y   -- The mouse coordinates at the time of the press.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Mouse_Left_Press(int x, int y)
{
	if (!IsRepairMode && !IsSellMode && IsTargettingMode == SPC_NONE && !PendingObject) {
		IsTentative = true;
		BandX = x;
		BandY = y;
		NewX = x;
		NewY = y;
	}
}


/***********************************************************************************************
 * DisplayClass::Mouse_Left_Held -- Handles the left button held down.                         *
 *                                                                                             *
 *    This routine is called continuously while the left mouse button is held down over        *
 *    the tactical map. This handles the rubber band mode detection and dragging.              *
 *                                                                                             *
 * INPUT:   x,y   -- The mouse coordinate.                                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Mouse_Left_Held(int x, int y)
{
	if (IsRubberBand) {
		if (x != NewX || y != NewY) {
			x = Bound(x, 0, Lepton_To_Pixel(TacLeptonWidth)-1);
			y = Bound(y, 0, Lepton_To_Pixel(TacLeptonHeight)-1);
			Refresh_Band();
			NewX = x;
			NewY = y;
			IsToRedraw = true;
			Flag_To_Redraw(false);
		}
	} else {

		/*
		**	If the mouse is still held down while a tentative extended select is possible, then
		**	check to see if the mouse has moved a sufficient distance in order to activate
		**	extended select mode.
		*/
		if (IsTentative) {

			/*
			**	The mouse must have moved a minimum distance before rubber band mode can be
			**	initiated.
			*/
			if (ABS(x - BandX) > 4 || ABS(y - BandY) > 4) {
				IsRubberBand = true;
				x = Bound(x, 0, Lepton_To_Pixel(TacLeptonWidth)-1);
				y = Bound(y, 0, Lepton_To_Pixel(TacLeptonHeight)-1);
				NewX = x;
				NewY = y;
				IsToRedraw = true;
				Flag_To_Redraw(false);

				/*
				**	Stretching the rubber band requires all objects to be redrawn.
				*/
				int index;
				for (index = 0; index < Layer[LAYER_TOP].Count(); index++) {
					Layer[LAYER_TOP][index]->Mark(MARK_CHANGE);
				}
				for (index = 0; index < Layer[LAYER_AIR].Count(); index++) {
					Layer[LAYER_AIR][index]->Mark(MARK_CHANGE);
				}

			}
		}
	}
}


// Needed to accomodate Glyphx client sidebar. ST - 4/12/2019 5:29PM
extern int GlyphXClientSidebarWidthInLeptons;

/***********************************************************************************************
 * DisplayClass::Set_Tactical_Position -- Sets the tactical view position.                     *
 *                                                                                             *
 *    This routine is used to set the tactical view position. The requested position is        *
 *    clipped to the map dimensions as necessary.                                              *
 *                                                                                             *
 * INPUT:   coord -- The coordinate desired for the upper left corner.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Set_Tactical_Position(COORDINATE coord)
{
	/*
	**	Bound the desired location to fit the legal map edges.
	*/
	int xx = 0;// (int)Coord_X(coord) - (int)Cell_To_Lepton(MapCellX);
	int yy = 0;// (int)Coord_Y(coord) - (int)Cell_To_Lepton(MapCellY);

	Confine_Rect(&xx, &yy, TacLeptonWidth, TacLeptonHeight, Cell_To_Lepton(MapCellWidth) + GlyphXClientSidebarWidthInLeptons, Cell_To_Lepton(MapCellHeight));		// Needed to accomodate Glyphx client sidebar. ST - 4/12/2019 5:29PM
//	Confine_Rect(&xx, &yy, TacLeptonWidth, TacLeptonHeight, Cell_To_Lepton(MapCellWidth), Cell_To_Lepton(MapCellHeight));
	coord = XY_Coord(xx + Cell_To_Lepton(MapCellX), yy + Cell_To_Lepton(MapCellY));

	if (ScenarioInit) {
		TacticalCoord = coord;
	}
	DesiredTacticalCoord = coord;

	IsToRedraw = true;
	Flag_To_Redraw(false);
}


/***********************************************************************************************
 * DisplayClass::Compute_Start_Pos -- Computes player's start pos from unit coords.            *
 *                                                                                             *
 * Use this function in multiplayer games, to compute the scenario starting Tactical Pos.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  x, y -- Player starting location                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/28/1995 JLB : Commented.                                                               *
 *   06/26/1995 JLB : Fixed building loop.                                                     *
 *   10/20/1996 JLB : Doesn't wrap.                                                            *
 *=============================================================================================*/
void DisplayClass::Compute_Start_Pos(long& x, long& y)
{
	/*
	**	Find the summation cell-x & cell-y for all the player's units, infantry,
	**	and buildings.  Buildings are weighted so that they count 16 times more
	**	than units or infantry.
	*/
	x = 0;
	y = 0;
	long num = 0;
	int i;
	for (i = 0; i < Infantry.Count(); i++) {
		InfantryClass * infp = Infantry.Ptr(i);
		if (!infp->IsInLimbo && infp->Is_Owned_By_Player()) {
			x += (long)Coord_XCell(infp->Coord);
			y += (long)Coord_YCell(infp->Coord);
			num++;
		}
	}

	for (i = 0; i < Units.Count(); i++) {
		UnitClass * unitp = Units.Ptr(i);
		if (!unitp->IsInLimbo && unitp->Is_Owned_By_Player()) {
			x += (long)Coord_XCell(unitp->Coord);
			y += (long)Coord_YCell(unitp->Coord);
			num++;
		}
	}

	for (i = 0; i < Buildings.Count(); i++) {
		BuildingClass * bldgp = Buildings.Ptr(i);
		if (!bldgp->IsInLimbo && bldgp->Is_Owned_By_Player()) {
			x += (((long)Coord_XCell(bldgp->Coord)) * 16);
			y += (((long)Coord_YCell(bldgp->Coord)) * 16);
			num += 16;
		}
	}

	for (i = 0; i < Vessels.Count(); i++) {
		VesselClass * bldgp = Vessels.Ptr(i);
		if (!bldgp->IsInLimbo && bldgp->Is_Owned_By_Player()) {
			x += (((long)Coord_XCell(bldgp->Coord)));
			y += (((long)Coord_YCell(bldgp->Coord)));
			num++;
		}
	}

	/*
	**	Divide each coord by 'num' to compute the average value
	*/
	if (num > 0) {
		x /= num;
	} else {
		x = 0;
	}

	if (num > 0) {
		y /= num;
	} else {
		y = 0;
	}
}


/***********************************************************************************************
 * DisplayClass::Sell_Mode_Control -- Controls the sell mode.                                  *
 *                                                                                             *
 *    This routine will control the sell mode for the player.                                  *
 *                                                                                             *
 * INPUT:   control  -- The mode to set the sell state to.                                     *
 *                      0  = Turn sell mode off.                                               *
 *                      1  = Turn sell mode on.                                                *
 *                      -1 = Toggle sell mode.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Sell_Mode_Control(int control)
{
	bool mode = IsSellMode;
	switch (control) {
		case 0:
			mode = false;
			break;

		case -1:
			mode = (IsSellMode == false);
			break;

		case 1:
			mode = true;
			break;
	}

	if (mode != IsSellMode && !PendingObject) {
		IsRepairMode = false;
		if (mode && PlayerPtr->BScan) {
			IsSellMode = true;
			Unselect_All();
		} else {
			IsSellMode = false;
			Revert_Mouse_Shape();
		}
	}
}


/***********************************************************************************************
 * DisplayClass::Repair_Mode_Control -- Controls the repair mode.                              *
 *                                                                                             *
 *    This routine is used to control the repair mode for the player.                          *
 *                                                                                             *
 * INPUT:   control  -- The mode to set the repair to.                                         *
 *                      0 = Turn repair off.                                                   *
 *                      1 = Turn repair on.                                                    *
 *                      -1= Toggle repair state.                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Repair_Mode_Control(int control)
{
	bool mode = IsRepairMode;
	switch (control) {
		case 0:
			mode = false;
			break;

		case -1:
			mode = (IsRepairMode == false);
			break;

		case 1:
			mode = true;
			break;
	}

	if (mode != IsRepairMode && !PendingObject) {
		IsSellMode = false;
		if (mode && PlayerPtr->BScan) {
			IsRepairMode = true;
			Unselect_All();
		} else {
			IsRepairMode = false;
			Revert_Mouse_Shape();
		}
	}
}


/***********************************************************************************************
 * DisplayClass::In_View -- Determines if cell is visible on screen.                           *
 *                                                                                             *
 *    Use this routine to determine if the specified cell is visible on                        *
 *    the display. This is a useful fact, since many display operations                        *
 *    can be skipped if the cell is not visible.                                               *
 *                                                                                             *
 * INPUT:   cell  -- The cell number to check.                                                 *
 *                                                                                             *
 * OUTPUT:  bool; Is this cell visible on the display?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/30/1994 JLB : Created.                                                                 *
 *   04/30/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
bool DisplayClass::In_View(register CELL cell) const
{
	if (cell & 0xC000) return(false);

	COORDINATE coord = Coord_Whole(Cell_Coord(cell));
	COORDINATE tcoord = Coord_Whole(TacticalCoord);

	if ((Coord_X(coord) - Coord_X(tcoord)) > TacLeptonWidth+CELL_LEPTON_W-1) return(false);
	if ((Coord_Y(coord) - Coord_Y(tcoord)) > TacLeptonHeight+CELL_LEPTON_H-1) return(false);
	return(true);
}


/***********************************************************************************************
 * DisplayClass::Closest_Free_Spot -- Finds the closest cell sub spot that is free.            *
 *                                                                                             *
 *    Use this routine to find the sub cell spot closest to the coordinate specified that is   *
 *    free from occupation. Typical use of this is for infantry destination calculation.       *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to use as the starting point when finding the closest      *
 *                   free spot.                                                                *
 *                                                                                             *
 *          any   -- Ignore occupation and just return the closest sub cell spot?              *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate of the closest free (possibly) sub cell location.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE DisplayClass::Closest_Free_Spot(COORDINATE coord, bool any) const
{
	if (coord & HIGH_COORD_MASK) {
		return(0x00800080);
	}
	return Map[coord].Closest_Free_Spot(coord, any);
}


/***********************************************************************************************
 * DisplayClass::Is_Spot_Free -- Determines if cell sub spot is free of occupation.            *
 *                                                                                             *
 *    Use this routine to determine if the coordinate (rounded to the nearest sub cell         *
 *    position) is free for placement. Typical use of this would be for infantry placement.    *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to examine for "freeness". The coordinate is rounded to    *
 *          the nearest free sub cell spot.                                                    *
 *                                                                                             *
 * OUTPUT:  Is the sub spot indicated by the coordinate free from previous occupation?         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool DisplayClass::Is_Spot_Free(COORDINATE coord) const
{
	// This can't be right. Copy/paste error, maybe? ST - 5/8/2019
	//if (coord & HIGH_COORD_MASK) {
	//	return(0x00800080);
	//}
	return Map[coord].Is_Spot_Free(CellClass::Spot_Index(coord));
}


/***********************************************************************************************
 * DisplayClass::Center_Map -- Centers the map about the currently selected objects            *
 *                                                                                             *
 *    This routine will average the position of all the selected objects and then center       *
 *    the map about those objects.                                                             *
 *                                                                                             *
 * INPUT:   center   -- The is an optional center about override coordinate. If specified,     *
 *                      then the map will be centered about that coordinate. Otherwise it      *
 *                      will center about the average location of all selected objects.        *
 *                                                                                             *
 * OUTPUT:  The center coordinate.                                                             *
 *                                                                                             *
 * WARNINGS:   The map position changes by this routine.                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/22/1995 JLB : Created.                                                                 *
 *   09/16/1996 JLB : Takes coordinate to center about (as override).                          *
 *=============================================================================================*/
COORDINATE DisplayClass::Center_Map(COORDINATE center)
{
 	int x = 0;
//	unsigned x = 0;
 	int y = 0;
//	unsigned y = 0;
	bool centerit = false;

	if (CurrentObject.Count()) {

		for (int index = 0; index < CurrentObject.Count(); index++) {
			COORDINATE coord = CurrentObject[index]->Center_Coord();

			x += Coord_X(coord);
			y += Coord_Y(coord);
		}

		x /= CurrentObject.Count();
		y /= CurrentObject.Count();
		centerit = true;
	}

	if (center != 0L) {
		x = Coord_X(center);
		y = Coord_Y(center);
		centerit = true;
	}

	if (centerit) {
		center = XY_Coord(x, y);

		x = x - (int)TacLeptonWidth/2;
		if (x < Cell_To_Lepton(MapCellX)) x = Cell_To_Lepton(MapCellX);

		y = y - (int)TacLeptonHeight/2;
		if (y < Cell_To_Lepton(MapCellY)) y = Cell_To_Lepton(MapCellY);

		Set_Tactical_Position(XY_Coord(x, y));

		return center;
	}

	return 0;
}


/***********************************************************************************************
 * DisplayClass::Encroach_Shadow -- Causes the shadow to creep back by one cell.               *
 *                                                                                             *
 *    This routine will cause the shadow to creep back by one cell. Multiple calls to this     *
 *    routine will result in the shadow becoming more and more invasive until only the sight   *
 *    range of player controlled units will keep the shadow pushed back.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *          house -- Player to apply shroud to                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 JLB : Created.                                                                 *
 *   08/06/2019  ST: Added house parameter for multiplayer                                     *
 *=============================================================================================*/
void DisplayClass::Encroach_Shadow(HouseClass * house)
{
	CELL cell;
	for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		if (!In_Radar(cell)) continue;

		CellClass * cellptr = &(*this)[cell];
		if (cellptr->Is_Visible(house) || !cellptr->Is_Mapped(house)) continue;

		cellptr->IsToShroud = true;		// IsToShroud isn't used outside this function. ST - 8/6/2019 2:28PM
	}

	/*
	**	Mark all shadow edge cells to be fully shrouded. All adjacent mapped
	**	cell should become partially shrouded.
	*/
	for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		if (!In_Radar(cell)) continue;

		if ((*this)[cell].IsToShroud) {
			(*this)[cell].IsToShroud = false;
			Shroud_Cell(cell, house);
		}
	}

	All_To_Look(house);

	Flag_To_Redraw(true);
}


/***********************************************************************************************
 * DisplayClass::Shroud_Cell -- Returns the specified cell into the shrouded condition.        *
 *                                                                                             *
 *    This routine is called to add the shroud back to the cell specified. Typical of this     *
 *    would be when the shroud is to regenerate.                                               *
 *                                                                                             *
 * INPUT:   cell  -- The cell that the shroud is to be regenerated upon.                       *
 *          house -- Player to apply shroud to                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Adjacent cells might be affected by this routine. The affect is determined      *
 *             according to the legality of the partial shadow artwork. In the illegal cases   *
 *             the adjacent cell might become shrouded as well.                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1995 JLB : Created.                                                                 *
 *   06/17/1996 JLB : Modified to handle the new shadow pieces.                                *
 *   08/06/2019  ST: Added house parameter for multiplayer                                     *
 *=============================================================================================*/
void DisplayClass::Shroud_Cell(CELL cell, HouseClass * house)
{
	if (house->IsGPSActive) {
		if ( (*this)[cell].Is_Jamming(house)  ) {
			return;
		}
	}
	if (!In_Radar(cell)) return;

	CellClass * cellptr = &(*this)[cell];
	if (cellptr->Is_Mapped(house)) {

		cellptr->Set_Mapped(house, false);
		cellptr->Set_Visible(house, false);
		cellptr->Redraw_Objects();

		/*
		**	Check adjacent cells. There might be some weird combination of
		**	shrouded cells such that more cells must be shrouded in order for
		**	this to work.
		*/
		for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir++) {
			CELL c = Adjacent_Cell(cell, dir);
			CellClass * cptr = &(*this)[c];

			/*
			**	If this adjacent cell must be completely shrouded as a result
			**	of the map change, yet it isn't already shrouded, then recursively
			**	shroud that cell.
			*/
			if (c != cell) {
				cptr->Set_Visible(house, false);
			}

			/*
			**	Always redraw the cell because, more than likely, the shroud
			**	edge will change shape because of the map change.
			*/
			cptr->Redraw_Objects();
		}
	}
}


/***********************************************************************************************
 * DisplayClass::Read_INI -- Reads map control data from INI file.                             *
 *                                                                                             *
 *    This routine is used to read the map control data from the INI                           *
 *    file.                                                                                    *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the loaded INI file data.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The TriggerClass INI data must have been read before calling this function.     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Read_INI(CCINIClass & ini)
{
	/*
	**	Read the map dimensions.
	*/
	char const * const name = "Map";
	int x = ini.Get_Int(name, "X", 1);
	int y = ini.Get_Int(name, "Y", 1);
	int w = ini.Get_Int(name, "Width", MAP_CELL_W-2);
	int h = ini.Get_Int(name, "Height", MAP_CELL_H-2);

#ifndef FIXIT_VERSION_3		//	Map size no longer restricted.

#ifdef FIXIT_CSII	//	checked - ajw
	if(Session.Type >= GAME_MODEM && Session.Type <= GAME_INTERNET && PlayingAgainstVersion < VERSION_AFTERMATH_CS) {
		/*
		**	HACK ALERT:
		**	Force the map to be limited to the size that 96x96 would be. If the
		**	size is greater (due to hacking?) then shrink it down to legal size.
		** BG Note: only do this for multiplayer games against non-AfterMath.
		*/
		if (w * h > 96 * 96) {
			h -= (((w*h) - (96*96)) / w) + 1;
		}
	}
#else
	/*
	**	HACK ALERT:
	**	Force the map to be limited to the size that 96x96 would be. If the
	**	size is greater (due to hacking?) then shrink it down to legal size.
	*/
	if (w * h > 96 * 96) {
		h -= (((w*h) - (96*96)) / w) + 1;
	}
#endif

#endif	//	!FIXIT_VERSION_3

	Set_Map_Dimensions( x, y, w, h );

	/*
	**	The theater is determined at this point. There is specific data that
	**	is custom to this data. Load the custom data (as it related to terrain)
	**	at this point.
	*/
	Scen.Theater = ini.Get_TheaterType(name, "Theater", THEATER_TEMPERATE);
	if (Scen.Theater == THEATER_NONE) {
		Scen.Theater = THEATER_TEMPERATE;
	}

	/*
	** Remove any old theater specific uncompressed shapes
	*/
#ifdef WIN32
	if (Scen.Theater != LastTheater) {
		Reset_Theater_Shapes();
	}
#endif	//WIN32

	/*
	**	Now that the theater is known, init the entire map hierarchy
	*/
	Init(Scen.Theater);

	/*
	**	Special initializations occur when the theater is known.
	*/
	TerrainTypeClass::Init(Scen.Theater);
	TemplateTypeClass::Init(Scen.Theater);
	OverlayTypeClass::Init(Scen.Theater);
	UnitTypeClass::Init(Scen.Theater);
	InfantryTypeClass::Init(Scen.Theater);
	BuildingTypeClass::Init(Scen.Theater);
	BulletTypeClass::Init(Scen.Theater);
	AnimTypeClass::Init(Scen.Theater);
	AircraftTypeClass::Init(Scen.Theater);
	VesselTypeClass::Init(Scen.Theater);
	SmudgeTypeClass::Init(Scen.Theater);

	/*
	**	Read the Waypoint entries.
	*/
	for (int i = 0; i < WAYPT_COUNT; i++) {
		char buf[20];
		sprintf(buf, "%d", i);
		Scen.Waypoint[i] = ini.Get_Int("Waypoints", buf, -1);

		if (Scen.Waypoint[i] != -1) {
			(*this)[Scen.Waypoint[i]].IsWaypoint = 1;
		}
	}

	/*
	**	Set the starting position (do this after Init(), which clears the cells'
	**	IsWaypoint flags).
	*/
	if (Scen.Waypoint[WAYPT_HOME] == -1) {
		Scen.Waypoint[WAYPT_HOME] = XY_Cell(MapCellX + 5*RESFACTOR, MapCellY + 4*RESFACTOR);
	}

	Scen.Views[0] = Scen.Views[1] = Scen.Views[2] = Scen.Views[3] = Scen.Waypoint[WAYPT_HOME];
	Set_Tactical_Position(Cell_Coord((Scen.Waypoint[WAYPT_HOME] - (MAP_CELL_W * 4 * RESFACTOR)) - (5*RESFACTOR)));

	/*
	**	Loop through all CellTrigger entries.
	*/
	int len = ini.Entry_Count("CellTriggers");
	for (int index = 0; index < len; index++) {

		/*
		**	Get a cell trigger and cell assignment.
		*/
		char const * cellentry = ini.Get_Entry("CellTriggers", index);
		TriggerTypeClass * tp = ini.Get_TriggerType("CellTriggers", cellentry);
		CELL cell = atoi(cellentry);

		if (tp != NULL && !(*this)[cell].Trigger.Is_Valid()) {
			TriggerClass * tt = Find_Or_Make(tp);
			if (tt) {
				tt->AttachCount++;
				tt->Cell = cell;
				(*this)[cell].Trigger = tt;
			}
		}
	}

	/*
	**	Read the map template data.
	*/
	static char const * const MAPPACK = "MapPack";
	len = ini.Get_UUBlock(MAPPACK, _staging_buffer, sizeof(_staging_buffer));
	BufferStraw bstraw(_staging_buffer, len);
	Map.Read_Binary(bstraw);

	LastTheater = Scen.Theater;
}


/***********************************************************************************************
 * DisplayClass::Write_INI -- Write the map data to the INI file specified.                    *
 *                                                                                             *
 *    This routine will output all the data of this map to the INI database specified.         *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI handler to store the map data to.                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Any existing map data in the INI database will be replaced by this function.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Write_INI(CCINIClass & ini)
{
	char entry[20];

	/*
	**	Save the map parameters.
	*/
	static char const * const NAME = "Map";
	ini.Clear(NAME);
	ini.Put_TheaterType(NAME, "Theater", Scen.Theater);
	ini.Put_Int(NAME, "X", MapCellX);
	ini.Put_Int(NAME, "Y", MapCellY);
	ini.Put_Int(NAME, "Width", MapCellWidth);
	ini.Put_Int(NAME, "Height", MapCellHeight);

	/*
	**	Save the Waypoint entries.
	*/
	static char const * const WAYNAME = "Waypoints";
	ini.Clear(WAYNAME);
	for (int i = 0; i < WAYPT_COUNT; i++) {
		if (Scen.Waypoint[i] != -1) {
			sprintf(entry, "%d", i);
			ini.Put_Int(WAYNAME, entry, Scen.Waypoint[i]);
		}
	}

	/*
	**	Save the cell's triggers.
	*/
	static char const * const CELLTRIG = "CellTriggers";
	ini.Clear(CELLTRIG);
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		if ((*this)[cell].Trigger.Is_Valid()) {
			TriggerClass * tp = (*this)[cell].Trigger;
			if (tp != NULL) {

				/*
				**	Generate entry name.
				*/
				sprintf(entry, "%d", cell);

				/*
				**	Save entry.
				*/
				ini.Put_TriggerType(CELLTRIG, entry, tp->Class);
			}
		}
	}

	/*
	**	Write the map template data out to the ini file.
	*/
	static char const * const MAPPACK = "MapPack";
	BufferPipe bpipe(_staging_buffer, sizeof(_staging_buffer));
	int len = Map.Write_Binary(bpipe);
	ini.Clear(MAPPACK);
	if (len) {
		ini.Put_UUBlock(MAPPACK, _staging_buffer, len);
	}
}


/***********************************************************************************************
 * DisplayClass::All_To_Look -- Direct all objects to look around for the player.              *
 *                                                                                             *
 *    This routine will scan through all objects and tell them to look if they are supposed    *
 *    to be able to reveal the map for the player. This routine may be necessary in cases      *
 *    of gap generator reshroud logic.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *   08/06/2019  ST : Added house parameter so it can work for multiple players                *
 *=============================================================================================*/
void DisplayClass::All_To_Look(HouseClass *house, bool units_only)
{
	for (int index = 0; index < Layer[LAYER_GROUND].Count(); index++) {
		ObjectClass * object = Layer[LAYER_GROUND][index];
		if (object != NULL && object->Is_Techno()) {
			TechnoClass * tech = ((TechnoClass *)object);

			if (tech->What_Am_I() == RTTI_BUILDING && units_only) continue;

			if (tech->House == house) {
				if (tech->Is_Discovered_By_Player(house)) {
					object->Look();
				}
			} else {
				//if (tech->What_Am_I() == RTTI_BUILDING && Rule.IsAllyReveal && tech->House->Is_Ally(PlayerPtr)) {
				if (tech->What_Am_I() == RTTI_BUILDING && Rule.IsAllyReveal && tech->House->Is_Ally(house)) {
					tech->Look();
				}
			}
		}
	}
}


/*
** Added house parameter for client/server multiplayer. ST - 8/12/2019 11:48AM
** 
** 
*/
void DisplayClass::Constrained_Look(COORDINATE center, LEPTON distance, HouseClass *house)
{
	for (int index = 0; index < Layer[LAYER_GROUND].Count(); index++) {
		ObjectClass * object = Layer[LAYER_GROUND][index];
		if (object != NULL && object->Is_Techno()) {
			TechnoClass * tech = ((TechnoClass *)object);

//			if (tech->What_Am_I() == RTTI_BUILDING && units_only) continue;

			if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {
				if (tech->House->IsPlayerControl) {
					if (tech->IsDiscoveredByPlayer && Distance(tech->Center_Coord(), center) <= (tech->Techno_Type_Class()->SightRange * CELL_LEPTON_W) + distance) {
						object->Look();
					}
				} else {
					
					if (tech->What_Am_I() == RTTI_BUILDING && Rule.IsAllyReveal && tech->House->Is_Ally(PlayerPtr) &&
						Distance(tech->Center_Coord(), center) <= (tech->Techno_Type_Class()->SightRange * CELL_LEPTON_W) + distance) {

						tech->Look();
					}
				}
			} else {
				
				if (tech->House->IsHuman) {

					if (tech->House == house) {
						if (tech->Is_Discovered_By_Player(house) && Distance(tech->Center_Coord(), center) <= (tech->Techno_Type_Class()->SightRange * CELL_LEPTON_W) + distance) {
							object->Look();
						}
					} else {
						if (tech->What_Am_I() == RTTI_BUILDING && Rule.IsAllyReveal && tech->House->Is_Ally(house) && 
							Distance(tech->Center_Coord(), center) <= (tech->Techno_Type_Class()->SightRange * CELL_LEPTON_W) + distance) {
							tech->Look();
						}
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * DisplayClass::Flag_Cell -- Flag the specified cell to be redrawn.                           *
 *                                                                                             *
 *    This will flag the cell to be redrawn.                                                   *
 *                                                                                             *
 * INPUT:   cell  -- The cell to be flagged.                                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Flag_Cell(CELL cell)
{
	Flag_To_Redraw(false);
	IsToRedraw = true;
	CellRedraw[cell] = true;
}

static ActionType _priority_actions[] = {
	ACTION_ATTACK,
	ACTION_ENTER,
	ACTION_HEAL,
	ACTION_REPAIR,
	ACTION_SABOTAGE,
	ACTION_CAPTURE,
	ACTION_MOVE
};

static int get_action_priority(ActionType action)
{
	for (int i = 0; i < ARRAY_LENGTH(_priority_actions); ++i) {
		if (_priority_actions[i] == action) {
			return i;
		}
	}
	return INT_MAX;
}

template <typename T>
static int index_of(const DynamicVectorClass<T*>& list, T* object)
{
	for (int i = 0; i < list.Count(); i++) {
		if (list[i] == object) {
			return i;
		}
	}
	return -1;
}

template <typename T>
static ObjectClass* Best_Object_With_ActionT(DynamicVectorClass<ObjectClass*>& objects, T subject)
{
	DynamicVectorClass<const ObjectTypeClass*> checked_types;

	if (objects.Count()) {
		int best_priority = INT_MAX;
		ObjectClass* best_object = objects[0];
		for (int i = 0; i < objects.Count(); ++i) {
			ObjectClass* object = objects[i];
			const ObjectTypeClass* type = &object->Class_Of();
			if (index_of(checked_types, type) != -1) {
				continue;
			}
			checked_types.Add(type);
			ActionType action = object->What_Action(subject);
			int priority = get_action_priority(action);
			if (priority < best_priority) {
				best_priority = priority;
				best_object = object;
				if (best_priority == 0) {
					break;
				}
			}
		}
		return best_object;
	}
	return NULL;
}

ObjectClass* Best_Object_With_Action(DynamicVectorClass<ObjectClass*>& objects, const ObjectClass* object)
{
	return Best_Object_With_ActionT(objects, object);
}

ObjectClass* Best_Object_With_Action(DynamicVectorClass<ObjectClass*>& objects, CELL cell)
{
	return Best_Object_With_ActionT(objects, cell);
}

ActionType Best_Object_Action(DynamicVectorClass<ObjectClass*>& objects, const ObjectClass* object)
{
	ObjectClass* obj = Best_Object_With_Action(objects, object);
	return (obj != NULL) ? obj->What_Action(object) : ACTION_NONE;
}

ActionType Best_Object_Action(DynamicVectorClass<ObjectClass*>& objects, CELL cell)
{
	ObjectClass* obj = Best_Object_With_Action(objects, cell);
	return (obj != NULL) ? obj->What_Action(cell) : ACTION_NONE;
}

ObjectClass* Best_Object_With_Action(const ObjectClass* object)
{
	return Best_Object_With_Action(CurrentObject.Raw(), object);
}

ObjectClass* Best_Object_With_Action(CELL cell)
{
	return Best_Object_With_Action(CurrentObject.Raw(), cell);
}

ActionType Best_Object_Action(const ObjectClass* object)
{
	return Best_Object_Action(CurrentObject.Raw(), object);
}

ActionType Best_Object_Action(CELL cell)
{
	return Best_Object_Action(CurrentObject.Raw(), cell);
}