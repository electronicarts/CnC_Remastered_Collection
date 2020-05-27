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

/* $Header:   F:\projects\c&c\vcs\code\display.cpv   2.16   16 Oct 1995 16:48:24   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : August 24, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   DisplayClass::AI -- Handles the maintenance tasks for the map display.                    *
 *   DisplayClass::Calculated_Cell -- Fetch a map cell based on specified method.              *
 *   DisplayClass::Cell_Object -- Determines what has been clicked on.                         *
 *   DisplayClass::Cell_Shadow   -- Determine what shadow icon to use for the cell.            *
 *   DisplayClass::Click_Cell_Calc -- Determines cell from screen X & Y.                       *
 *   DisplayClass::Coord_To_Pixel -- Determines X and Y pixel coordinates.                     *
 *   DisplayClass::Cursor_Mark -- Set or resets the cursor display flag bits.                  *
 *   DisplayClass::DisplayClass -- Default constructor for display class.                      *
 *   DisplayClass::Draw_It -- Draws the tactical map.                                          *
 *   DisplayClass::Flag_To_Redraw -- Flags the display so that it will be redrawn as soon as poss*
 *   DisplayClass::Get_Occupy_Dimensions -- computes width & height of the given occupy list   *
 *   DisplayClass::Init_Clear -- Clears the display to a known state.                          *
 *   DisplayClass::Init_IO -- Creates the map's button list                                    *
 *   DisplayClass::Init_Theater -- Theater-specific initialization                             *
 *   DisplayClass::Map_Cell -- Mark specified cell as having been mapped.                      *
 *   DisplayClass::Mouse_Left_Held -- Handles the left button held down.                       *
 *   DisplayClass::Mouse_Left_Press -- Handles the left mouse button press.                    *
 *   DisplayClass::Mouse_Left_Release -- Handles the left mouse button release.                *
 *   DisplayClass::Mouse_Left_Up -- Handles the left mouse "cruising" over the map.            *
 *   DisplayClass::Mouse_Right_Press -- Handles the right mouse button press.                  *
 *   DisplayClass::Next_Object -- Searches for next object on display.                         *
 *   DisplayClass::One_Time -- Performs any special one time initializations.                  *
 *   DisplayClass::Passes_Proximity_Check -- Determines if building placement is near friendly sq*
 *   DisplayClass::Pixel_To_Coord -- converts screen coord to COORDINATE                            *
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
 *   DisplayClass::Set_View_Dimensions -- Sets the tactical display screen coordinates.        *
 *   DisplayClass::Submit -- Adds a game object to the map rendering system.                   *
 *   DisplayClass::TacticalClass::Action -- Processes input for the tactical map.              *
 *   DisplayClass::Text_Overlap_List -- Creates cell overlap list for specified text string.   *
 *   DisplayClass::Write_INI -- Writes map data into INI file.                                 *
 *   DisplayClass::Set_Tactical_Position -- Sets the tactical view position.                   *
 *   DisplayClass::Center_Map -- Centers the map about the currently selected objects          *
 *   DisplayClass::Prev_Object -- Searches for the previous object on the map.                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include	"function.h"

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
unsigned char DisplayClass::FadingLight[256];
unsigned char DisplayClass::RemapTables[HOUSE_COUNT][3][256];
unsigned char DisplayClass::FadingGreen[256];
unsigned char DisplayClass::FadingYellow[256];
unsigned char DisplayClass::FadingRed[256];
unsigned char DisplayClass::TranslucentTable[(MAGIC_COL_COUNT+1)*256];
unsigned char DisplayClass::WhiteTranslucentTable[(1+1)*256];
unsigned char DisplayClass::MouseTranslucentTable[(4+1)*256];
void const * DisplayClass::TransIconset;
unsigned char DisplayClass::UnitShadow[(USHADOW_COL_COUNT+1)*256];
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

/*
**	Define "_RETRIEVE" if the palette morphing tables are part of the loaded data. If this
**	is undefined, then the files will be created.
*/
#define	_RETRIEVE


static int const TEX_X = 0;
static int const TEX_Y = 6;
static int const TEX_W = 14;

extern MixFileClass *TheaterIcons;


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
DisplayClass::DisplayClass(void)
{
	TacticalCoord = 0;
	ShadowShapes = 0;
	TransIconset = 0;
	ZoneCell = 0;
	ZoneOffset = 0;
	CursorSize = 0;
	ProximityCheck = false;
	PendingObjectPtr = 0;
	PendingObject = 0;
	PendingHouse = HOUSE_NONE;
	IsRepairMode = false;
	IsTargettingMode = false;
	IsToRedraw = true;
	IsRubberBand = false;
	IsTentative = false;
	IsSellMode = false;
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
	Set_View_Dimensions(0, Map.Get_Tab_Height());

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
	TransIconset = MixFileClass::Retrieve("TRANS.ICN");

	ShadowShapes = MixFileClass::Retrieve("SHADOW.SHP");

	Set_View_Dimensions(0, Map.Get_Tab_Height());

	/*
	**	Allocate and initialize the remap tables needed for each "house".
	*/
	HousesType	hindex;
	int	fade;

	for (fade = 0; fade < 3; fade++) {
		for (hindex = HOUSE_FIRST; hindex < HOUSE_COUNT; hindex++) {
			int	color;

			switch (fade) {
				case 0:
					for (color = 0; color < 256; color++) {
						RemapTables[hindex][fade][color] = color;
					}
					break;

				case 1:
					Mem_Copy(FadingLight, RemapTables[hindex][fade], 256);
					break;

				case 2:
					Mem_Copy(FadingShade, RemapTables[hindex][fade], 256);
					break;
			}
			Mem_Copy(&RemapTables[hindex][fade][((int)hindex+11)*16], &RemapTables[hindex][fade][(0+11)*16], 16);
		}
	}
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
	IsTargettingMode = false;
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
 *=============================================================================================*/
void DisplayClass::Init_Theater(TheaterType theater)
{
	char			fullname[16];
	char			iconname[16];
#ifndef _RETRIEVE
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
		{DKGREY, BLACK, 140, 0}
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
#endif

	/*
	---------------------- Invoke parent's init routine ----------------------
	*/
	MapClass::Init_Theater(theater);

	/*
	** Save the new theater value
	*/
	Theater = theater;

#ifndef DEMO
	/*
	** Unload old mixfiles, and cache the new ones
	*/
	sprintf(fullname, "%s.MIX", Theaters[Theater].Root);
	if (Theater != LastTheater){
		if (TheaterData) {
			delete TheaterData;
		}
		TheaterData = new MixFileClass(fullname);
		TheaterData->Cache();
	}

#endif
	/*
	** Register the hi-res icons mix file now since it is theater specific
	*/
	sprintf(fullname, "%s.MIX", Theaters[Theater].Root);
	strcpy (iconname, fullname);
	strcpy (&iconname[4], "ICNH.MIX");
	if (Theater != LastTheater){
		if (TheaterIcons) {
			delete TheaterIcons;
		}
		TheaterIcons = new MixFileClass(iconname);
		TheaterIcons->Cache();
	}



	/*
	**	Load the custom palette associated with this theater.
	**	The fading palettes will have to be generated as well.
	*/
	sprintf(fullname, "%s.PAL", Theaters[theater].Root);
	void const * ptr = MixFileClass::Retrieve(fullname);
	Mem_Copy((void *)ptr, GamePalette, 768);


	Mem_Copy(GamePalette, OriginalPalette, 768);

#ifndef _RETRIEVE
	/*
	**	Make sure that remapping doesn't occur on the colors that cycle.
	*/
	memset(&GamePalette[CYCLE_COLOR_START*3], 0x3F, CYCLE_COLOR_COUNT*3);
#endif


#ifdef _RETRIEVE
	CCFileClass(Fading_Table_Name("GREEN", theater)).Read(FadingGreen, sizeof(FadingGreen));
#else
	Build_Fading_Table(GamePalette, FadingGreen, GREEN, 110);
	CCFileClass(Fading_Table_Name("GREEN", theater)).Write(FadingGreen, sizeof(FadingGreen));
#endif
	if (theater == THEATER_DESERT) {
		FadingGreen[196] = 160;
	}

#ifdef _RETRIEVE
	CCFileClass(Fading_Table_Name("YELLOW", theater)).Read(FadingYellow, sizeof(FadingYellow));
#else
	Build_Fading_Table(GamePalette, FadingYellow, YELLOW, 140);
	CCFileClass(Fading_Table_Name("YELLOW", theater)).Write(FadingYellow, sizeof(FadingYellow));
#endif

#ifdef _RETRIEVE
	CCFileClass(Fading_Table_Name("RED", theater)).Read(FadingRed, sizeof(FadingRed));
#else
	Build_Fading_Table(GamePalette, FadingRed, RED, 140);
	CCFileClass(Fading_Table_Name("RED", theater)).Write(FadingRed, sizeof(FadingRed));
#endif

#ifdef _RETRIEVE
	CCFileClass(Fading_Table_Name("MOUSE", theater)).Read(MouseTranslucentTable, sizeof(MouseTranslucentTable));
#else
	Build_Translucent_Table(GamePalette, &MouseCols[0], 4, MouseTranslucentTable);
	CCFileClass(Fading_Table_Name("MOUSE", theater)).Write(MouseTranslucentTable, sizeof(MouseTranslucentTable));
#endif

//	MouseDrawPtr = MouseTranslucentTable;
//	MouseDrawPtr2 = Add_Long_To_Pointer(MouseTranslucentTable, 256L);
//	MouseDrawVal = 1;
//	MouseDrawFlags = (int)SHAPE_GHOST;

#ifdef _RETRIEVE
	CCFileClass(Fading_Table_Name("TRANS", theater)).Read(TranslucentTable, sizeof(TranslucentTable));
#else
	Build_Translucent_Table(GamePalette, &MagicCols[0], MAGIC_COL_COUNT, TranslucentTable);
	CCFileClass(Fading_Table_Name("TRANS", theater)).Write(TranslucentTable, sizeof(TranslucentTable));
#endif

#ifdef _RETRIEVE
	CCFileClass(Fading_Table_Name("WHITE", theater)).Read(WhiteTranslucentTable, sizeof(WhiteTranslucentTable));
#else
	Build_Translucent_Table(GamePalette, &WhiteCols[0], 1, WhiteTranslucentTable);
	CCFileClass(Fading_Table_Name("WHITE", theater)).Write(WhiteTranslucentTable, sizeof(WhiteTranslucentTable));
#endif

#ifdef _RETRIEVE
	CCFileClass(Fading_Table_Name("SHADOW", theater)).Read(ShadowTrans, sizeof(ShadowTrans));
#else
	Build_Translucent_Table(GamePalette, &ShadowCols[0], SHADOW_COL_COUNT, ShadowTrans);
	CCFileClass(Fading_Table_Name("SHADOW", theater)).Write(ShadowTrans, sizeof(ShadowTrans));
#endif

#ifdef _RETRIEVE
	CCFileClass(Fading_Table_Name("UNITS", theater)).Read(UnitShadow, sizeof(UnitShadow));
#else
	Conquer_Build_Translucent_Table(GamePalette, &UShadowCols[0], USHADOW_COL_COUNT, UnitShadow);
	CCFileClass(Fading_Table_Name("UNITS", theater)).Write(UnitShadow, sizeof(UnitShadow));
#endif

#ifdef _RETRIEVE
	CCFileClass(Fading_Table_Name("SHADE", theater)).Read(FadingShade, sizeof(FadingShade));
#else
	Conquer_Build_Fading_Table(GamePalette, FadingShade, BLACK, 150);
	CCFileClass(Fading_Table_Name("SHADE", theater)).Write(FadingShade, sizeof(FadingShade));
#endif

#ifdef _RETRIEVE
	CCFileClass(Fading_Table_Name("LIGHT", theater)).Read(FadingLight, sizeof(FadingLight));
#else
	Conquer_Build_Fading_Table(GamePalette, FadingLight, WHITE, 85);
	CCFileClass(Fading_Table_Name("LIGHT", theater)).Write(FadingLight, sizeof(FadingLight));
#endif

	/*
	**	Create the shadow color used by aircraft.
	*/
	Conquer_Build_Fading_Table(GamePalette, &SpecialGhost[256], BLACK, 100);
	for (int index = 0; index < 256; index++) {
		SpecialGhost[index] = 0;
	}

	Build_Fading_Table(GamePalette, FadingBrighten, WHITE, 25);


#ifndef _RETRIEVE
	/*
	**	Restore the palette since it was mangled while building the fading tables.
	*/
	sprintf(fullname, "%s.PAL", Theaters[theater].Root);
	ptr = MixFileClass::Retrieve(fullname);
	Mem_Copy((void *)ptr, GamePalette, 768);
	Mem_Copy(GamePalette, OriginalPalette, 768);
#endif

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
short const * DisplayClass::Text_Overlap_List(char const * text, int x, int y, int lines)
{
	static short _list[30];

	if (text) {
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
		}

		/*
		**	Build the list of overlap cell offset values according to the text
		**	coordinate and the length.
		*/
		int height = (((FontHeight * lines) + 23) / 24) * 24;

		if (x <= right) {
			CELL ul = Click_Cell_Calc(x, y-1);
			CELL lr = Click_Cell_Calc(x+len-1, Bound(y+height, TacPixelY, SeenBuff.Get_Height() - 1));

			if (ul == -1) ul = Click_Cell_Calc(x, y);
//			if (lr == -1) lr = Click_Cell_Calc(x+len, y);

			if (ul != -1 && lr != -1) {
				for (int yy = Cell_Y(ul); yy <= Cell_Y(lr); yy++) {
					for (int xx = Cell_X(ul); xx <= Cell_X(lr); xx++) {
						*ptr++ = XY_Cell(xx, yy) - Coord_Cell(TacticalCoord);
					}
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
 *          width -- The width of the tactical display (in pixels). If this parameter is       *
 *                   omitted, then the width will be as wide as the screen will allow.         *
 *                                                                                             *
 *          height-- The height of the tactial display (in pixels). If this parameter is       *
 *                   omitted, then the width wil be as wide as the screen will allow.          *
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
#if (1)	// This code pulled in from RA1. ST - 1/9/2019 10:53AM
	if (width == -1) {
		TacLeptonWidth = Pixel_To_Lepton(SeenBuff.Get_Width()-x);
	} else {
		TacLeptonWidth = width * CELL_LEPTON_W;
	}

	// ST - 3/1/2019 12:05PM
	// Made the below code more consistent with the width calculation. This is needed if we aren't going to draw the tabs at the top of the screen
	//
	if (height == -1) {
		TacLeptonHeight = Pixel_To_Lepton(SeenBuff.Get_Height()-y);
		//height = (SeenBuff.Get_Height()-y) / CELL_PIXEL_H;
	} else {
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

#else	//CNC code
	if (width == -1) {
		width = SeenBuff.Get_Width() - x;
	}
	TacLeptonWidth = Pixel_To_Lepton(width);

	if (height == -1) {
		height = SeenBuff.Get_Height() - y;
	}
	TacLeptonHeight = Pixel_To_Lepton(height);

	/*
	**	Adjust the tactical cell if it is now in an invalid position
	**	because of the changed dimensions.
	*/
	int xx = Coord_X(TacticalCoord) - (MapCellX * CELL_LEPTON_W);
	int yy = Coord_Y(TacticalCoord) - (MapCellY * CELL_LEPTON_H);

	Confine_Rect(&xx, &yy, TacLeptonWidth, TacLeptonHeight, MapCellWidth * CELL_LEPTON_W, MapCellHeight * CELL_LEPTON_H);

	Set_Tactical_Position(XY_Coord(xx + (MapCellX * CELL_LEPTON_W), yy + (MapCellY * CELL_LEPTON_H)));

	TacPixelX = x;
	TacPixelY = y;
	WindowList[WINDOW_TACTICAL][WINDOWX] = x >> 3;
	WindowList[WINDOW_TACTICAL][WINDOWY] = y;
	WindowList[WINDOW_TACTICAL][WINDOWWIDTH] = width >> 3;
	WindowList[WINDOW_TACTICAL][WINDOWHEIGHT] = height;
	if (Window == WINDOW_TACTICAL) {
		Change_Window(0);
		Change_Window(Window);
	}
	IsToRedraw = true;
	Flag_To_Redraw(false);

	TacButton.X			= TacPixelX;
	TacButton.Y			= TacPixelY;
	TacButton.Width	= Lepton_To_Pixel(TacLeptonWidth);
	TacButton.Height	= Lepton_To_Pixel(TacLeptonHeight);
#endif
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
 * OUTPUT:  bool; Can the pending building object be placed at the present cursor location     *
 *                checking only for proximity to friendly buildings?  If this isn't for a      *
 *                building type object, then this routine always returns true.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/06/1994 JLB : Created.                                                                 *
 *   06/07/1994 JLB : Handles concrete check.                                                  *
 *=============================================================================================*/
bool DisplayClass::Passes_Proximity_Check(ObjectTypeClass const *object)
{
#ifdef USE_RA_AI
	
	return Passes_Proximity_Check(object, PendingHouse, CursorSize, ZoneCell + ZoneOffset);

#else //USE_RA_AI		                            // Replaced by more generic function from RA, for RA AI. ST - 7/24/2019 5:46PM

	short const *ptr;

	/*
	** In editor mode, the proximity check always passes.
	*/
	if (Debug_Map) {
		return(true);
	}

	if (!object || !CursorSize || object->What_Am_I() != RTTI_BUILDINGTYPE) {
		return(true);
	}

	/*
	**	Scan through all cells that the building foundation would cover. If any adjacent
	**	cells to these are of friendly persuasion, then consider the proximity check to
	**	have been a success.
	*/
	ptr = CursorSize;
	while (*ptr != REFRESH_EOL) {
		CELL	cell = ZoneCell + ZoneOffset + *ptr++;

		for (FacingType facing = FACING_N; facing < FACING_COUNT; facing++) {
			CELL	newcell = Adjacent_Cell(cell, facing);

			if (!In_Radar(cell)) return(false);

			TechnoClass * base = (*this)[newcell].Cell_Techno();

			/*
			**	The special cell ownership flag allows building adjacent
			**	to friendly walls and bibs even though there is no official
			**	building located there.
			*/
			if ((*this)[newcell].Owner == PendingHouse) {
				return(true);
			}

			if (base && base->What_Am_I() == RTTI_BUILDING && base->House->Class->House == PendingHouse) {
				return(true);
			}
		}
	}
	return(false);
#endif
}




#ifdef USE_RA_AI
/*
** Additional version of Passes_Proximity_Check, inspired by RA implementation. Needed for RA AI. ST - 7/24/2019 5:42PM
*/

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
	short const *ptr;

	/*
	** In editor mode, the proximity check always passes.
	*/
	if (Debug_Map) {
		return(true);
	}

	if (!object || object->What_Am_I() != RTTI_BUILDINGTYPE) {
		return(true);
	}

	/*
	**	Scan through all cells that the building foundation would cover. If any adjacent
	**	cells to these are of friendly persuasion, then consider the proximity check to
	**	have been a success.
	*/
	ptr = list;
	while (*ptr != REFRESH_EOL) {
		CELL	cell = trycell + *ptr++;

		for (FacingType facing = FACING_N; facing < FACING_COUNT; facing++) {
			CELL	newcell = Adjacent_Cell(cell, facing);

			if (!In_Radar(cell)) return(false);

			TechnoClass * base = (*this)[newcell].Cell_Techno();

			/*
			**	The special cell ownership flag allows building adjacent
			**	to friendly walls and bibs even though there is no official
			**	building located there.
			*/
			if ((*this)[newcell].Owner == house) {
				return(true);
			}

			if (base && base->What_Am_I() == RTTI_BUILDING && base->House->Class->House == house) {
				return(true);
			}
		}
	}
	return(false);
}

#endif //USE_RA_AI







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

	if (!CursorSize) {
		prevpos = ZoneCell;
		ZoneCell = pos;
		return(prevpos);
	}

	/*
	**	Adjusts the position so that the placement cursor is never partway off the
	**	tactical map.
	*/
	int w,h;
	Get_Occupy_Dimensions (w, h, CursorSize);

	int x = Cell_X(pos + ZoneOffset);
	int y = Cell_Y(pos + ZoneOffset);

	if (x < Coord_XCell(TacticalCoord)) x = Coord_XCell(TacticalCoord);
//	if (x < TacMapX) x = TacMapX;
	if (y < Coord_YCell(TacticalCoord)) y = Coord_YCell(TacticalCoord);
//	if (y < TacMapY) y = TacMapY;
	if (x+w >= Coord_XCell(TacticalCoord) + Lepton_To_Cell(TacLeptonWidth)) x = Coord_XCell(TacticalCoord)+Lepton_To_Cell(TacLeptonWidth)-w;
//	if (x+w >= TacMapX+TacWidth) x = TacMapX+TacWidth-w;
	if (y+h >= Coord_YCell(TacticalCoord) + Lepton_To_Cell(TacLeptonHeight)) x = Coord_YCell(TacticalCoord)+Lepton_To_Cell(TacLeptonHeight)-h;
//	if (y+h >= TacMapY+TacHeight) y = TacMapY+TacHeight-h;
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
	if (CursorSize) {

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
	ProximityCheck = Passes_Proximity_Check(PendingObject);

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
void DisplayClass::Get_Occupy_Dimensions(int & w, int & h, short const *list)
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

			max_x = MAX(max_x, x);
			min_x = MIN(min_x, x);
			max_y = MAX(max_y, y);
			min_y = MIN(min_y, y);

			list++;
		}

		w = MAX(1, max_x - min_x + 1);
		h = MAX(1, max_y - min_y + 1);
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
	CELL const *ptr;
	CellClass *cellptr;

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
	if (PendingObjectPtr) {
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
CELL DisplayClass::Click_Cell_Calc(int x, int y)
{
	x -= TacPixelX;
	x = Pixel_To_Lepton(x);
	y -= TacPixelY;
	y = Pixel_To_Lepton(y);

	// Possibly ignore the view constraints if we aren't using the internal renderer. ST - 4/17/2019 9:06AM
	//if (x < TacLeptonWidth && y < TacLeptonHeight) {
	if (IgnoreViewConstraints || (x < TacLeptonWidth && y < TacLeptonHeight)) {

		COORDINATE tcoord = XY_Coord(Pixel_To_Lepton(Lepton_To_Pixel(Coord_X(TacticalCoord))), Pixel_To_Lepton(Lepton_To_Pixel(Coord_Y(TacticalCoord))));

		return(Coord_Cell(Coord_Add(tcoord, XY_Coord(x, y))));
	}
	return(-1);
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
void DisplayClass::Read_INI(char *buffer)
{
	char	name[16];
	int len;						// Length of data in buffer.
	char *tbuffer;				// Accumulation buffer of Trigger names.
	char *trigsection = "CellTriggers";
	char buf[20];				// trigger name for a cell
	int cell;
	int i;

	/*
	**	Read the map dimensions.
	*/
	Set_Map_Dimensions(WWGetPrivateProfileInt("MAP", "X", 1, buffer),
		WWGetPrivateProfileInt("MAP", "Y", 1, buffer),
		WWGetPrivateProfileInt("MAP", "Width", MAP_CELL_W-2, buffer),
		WWGetPrivateProfileInt("MAP", "Height", MAP_CELL_H-2, buffer));

	/*
	**	The theater is determined at this point. There is specific data that
	**	is custom to this data. Load the custom data (as it related to terrain)
	**	at this point.
	*/
	WWGetPrivateProfileString("MAP", "Theater", Theaters[THEATER_DESERT].Name, name, 13, buffer);
	Theater = Theater_From_Name(name);
	if (Theater == THEATER_NONE) {
		Theater = THEATER_DESERT;
	}

	/*
	** Remove any old theater specific uncompressed shapes
	*/
	if (Theater != LastTheater){
		Reset_Theater_Shapes();
	}

	/*
	**	Now that the theater is known, init the entire map hierarchy
	*/
	Init(Theater);

	/*
	**	Special initializations occur when the theater is known.
	*/
	TerrainTypeClass::Init(Theater);
	TemplateTypeClass::Init(Theater);
	OverlayTypeClass::Init(Theater);
	UnitTypeClass::Init(Theater);
	InfantryTypeClass::Init(Theater);
	BuildingTypeClass::Init(Theater);
	BulletTypeClass::Init(Theater);
	AnimTypeClass::Init(Theater);
	AircraftTypeClass::Init(Theater);
	SmudgeTypeClass::Init(Theater);

	LastTheater = Theater;

	/*
	**	Read the Waypoint entries.
	*/
	for (i = 0; i < WAYPT_COUNT; i++) {
		sprintf(buf,"%d",i);
		Waypoint[i] = WWGetPrivateProfileInt ("Waypoints",buf,-1,buffer);
		if (Waypoint[i] != -1) {
			(*this)[Waypoint[i]].IsWaypoint = 1;
		}
	}

	/*
	**	Set the starting position (do this after Init(), which clears the cells'
	**	IsWaypoint flags).
	*/
	if (Waypoint[WAYPT_HOME] == -1) {
		Waypoint[WAYPT_HOME] = XY_Cell(MapCellX, MapCellY);
	}
	Set_Tactical_Position(Cell_Coord(Waypoint[WAYPT_HOME])&0xFF00FF00L);
	Views[0] = Views[1] = Views[2] = Views[3] = Waypoint[WAYPT_HOME];

	/*
	**	Read the cell trigger names, and assign TriggerClass pointers
	*/
	len = strlen(buffer) + 2;		// len is the length of the INI data
	tbuffer = buffer + len;			// tbuffer is after the INI data

	/*
	**	Read all entry names into 'tbuffer'.
	*/
	WWGetPrivateProfileString(trigsection, NULL, NULL, tbuffer, ShapeBufferSize-len, buffer);

	/*
	**	Loop through all CellTrigger entries.
	*/
	while (*tbuffer != '\0') {

		/*
		**	Get a cell trigger assignment.
		*/
		WWGetPrivateProfileString(trigsection, tbuffer, NULL, buf, sizeof(buf) - 1, buffer);

		/*
		**	Get cell # from entry name.
		*/
		cell = atoi(tbuffer);
		if (cell > 0 && cell < MAP_CELL_TOTAL && !(*this)[cell].IsTrigger) {

			/*
			**	Assign trigger pointer using trigger name.
			*/
			CellTriggers[cell] = TriggerClass::As_Pointer(buf);
			if (CellTriggers[cell]) {
				(*this)[cell].IsTrigger = 1;
				if (CellTriggers[cell]) {
					CellTriggers[cell]->AttachCount++;
				}
			}
		}

		/*
		**	Step to next entry name.
		*/
		tbuffer += strlen(tbuffer) + 1;
	}
}


/***********************************************************************************************
 * DisplayClass::Write_INI -- Writes map data into INI file.                                   *
 *                                                                                             *
 *    This routine is used to write the map control data into the INI                          *
 *    file. The scenario editor uses this when creating the scenario                           *
 *    startup file.                                                                            *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to INI file data.                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void DisplayClass::Write_INI(char *buffer)
{
	char entry[20];

	/*
	**	Save the map parameters.
	*/
	WWWritePrivateProfileString("MAP", "Theater", Theaters[Theater].Name, buffer);
	WWWritePrivateProfileInt("MAP", "X", MapCellX, buffer);
	WWWritePrivateProfileInt("MAP", "Y", MapCellY, buffer);
	WWWritePrivateProfileInt("MAP", "Width", MapCellWidth, buffer);
	WWWritePrivateProfileInt("MAP", "Height", MapCellHeight, buffer);

	/*
	**	Save the Waypoint entries.
	*/
	for (int i = 0; i < WAYPT_COUNT; i++) {
		sprintf(entry,"%d",i);
		WWWritePrivateProfileInt ("Waypoints",entry,Waypoint[i],buffer);
	}

	/*
	**	Erase the CellTriggers section.
	*/
	WWWritePrivateProfileString("CellTriggers",NULL,NULL,buffer);

	/*
	**	Save the cell's triggers.
	*/
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		if ((*this)[cell].IsTrigger) {

			/*
			**	Get cell trigger pointer.
			*/
			TriggerClass const * trig = CellTriggers[cell];

			/*
			**	Generate entry name.
			*/
			sprintf(entry,"%d",cell);

			/*
			**	Save entry.
			*/
			WWWritePrivateProfileString("CellTriggers", entry, trig->Get_Name(), buffer);
		}
	}
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
	int xx = Coord_X(coord) - Cell_To_Lepton(MapCellX);
	int yy = Coord_Y(coord) - Cell_To_Lepton(MapCellY);
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
void DisplayClass::Refresh_Cells(CELL cell, short const *list)
{
	if (*list == REFRESH_SIDEBAR) {
		list++;
	}
	while (*list != REFRESH_EOL) {
		CELL newcell = cell + *list++;
		if (In_Radar(newcell)) {
			(*this)[newcell].Redraw_Objects();
		}
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
 *   03/06/2019  ST : Added house parameter so we can do this per player                       *
 *=============================================================================================*/
int DisplayClass::Cell_Shadow(CELL cell, HouseClass *house)
{
	if (house == NULL) {
		return -1;
	}
	int	index;
	int	value = -1;
	CellClass	*cellptr;
	static char const CardShadow[16] = {-2,0,1,2,3,-1,4,-1,5,6,-1,-1,7,-1,-1,-1};
	static char const DiagShadow[16] = {-2,8,9,-1,10,-1,-1,-1,11,-1,-1,-1,-1,-1,-1,-1};

	/*
	**	Don't map cells that are at the edges. This solves
	**	problem of accessing cells off the bounds of map and into
	**	who-knows-what memory.
	*/
	if ((unsigned)(Cell_X(cell)-1) >= MAP_CELL_W-2) return(-2);
	if ((unsigned)(Cell_Y(cell)-1) >= MAP_CELL_H-2) return(-2);		// Changed > to >= per Red Alert to fix out of bounds crash. ST - 3/25/2020 11:02PM

	cellptr = &(*this)[cell];
	if (!cellptr->Is_Mapped(house)) {

		/*
		**	Check the cardinal directions first. This will either result
		**	in a solution or the flag to check the diagonals.
		*/
		index = 0;
		cellptr--;
		if (cellptr->Is_Mapped(house)) index |= 0x08;
		cellptr += MAP_CELL_W+1;
		if (cellptr->Is_Mapped(house)) index |= 0x04;
		cellptr -= MAP_CELL_W-1;
		if (cellptr->Is_Mapped(house)) index |= 0x02;
		cellptr -= MAP_CELL_W+1;
		if (cellptr->Is_Mapped(house)) index |= 0x01;
		value = CardShadow[index];

		/*
		**	The diagonals must be checked, since the cardinal directions
		**	did not yield a valid result.
		*/
		if (value == -2) {
			index = 0;
			cellptr--;
			if (cellptr->Is_Mapped(house)) index |= 0x08;
			cellptr += MAP_CELL_W*2;
			if (cellptr->Is_Mapped(house)) index |= 0x04;
			cellptr += 2;
			if (cellptr->Is_Mapped(house)) index |= 0x02;
			cellptr -= MAP_CELL_W*2;
			if (cellptr->Is_Mapped(house)) index |= 0x01;
			value = DiagShadow[index];
		}

		/*
		**	Randomizer should go here. Add sets in multiples of 12.
		*/

	}
	return(value);
}

#if (0)
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
int DisplayClass::Cell_Shadow(CELL cell)
{
	int	index;
	int	value = -1;
	CellClass	*cellptr;
	static char const CardShadow[16] = {-2,0,1,2,3,-1,4,-1,5,6,-1,-1,7,-1,-1,-1};
	static char const DiagShadow[16] = {-2,8,9,-1,10,-1,-1,-1,11,-1,-1,-1,-1,-1,-1,-1};

	/*
	**	Don't map cells that are at the top or bottom edge. This solves
	**	problem of accessing cells off the top or bottom of the map and into
	**	who-knows-what memory.
	*/
	if ((unsigned)(Cell_Y(cell)-1) > MAP_CELL_H-2) return(-2);

	cellptr = &(*this)[cell];
	if (!cellptr->IsMapped) {

		/*
		**	Check the cardinal directions first. This will either result
		**	in a solution or the flag to check the diagonals.
		*/
		index = 0;
		cellptr--;
		if (cellptr->IsMapped) index |= 0x08;
		cellptr += MAP_CELL_W+1;
		if (cellptr->IsMapped) index |= 0x04;
		cellptr -= MAP_CELL_W-1;
		if (cellptr->IsMapped) index |= 0x02;
		cellptr -= MAP_CELL_W+1;
		if (cellptr->IsMapped) index |= 0x01;
		value = CardShadow[index];

		/*
		**	The diagonals must be checked, since the cardinal directions
		**	did not yield a valid result.
		*/
		if (value == -2) {
			index = 0;
			cellptr--;
			if (cellptr->IsMapped) index |= 0x08;
			cellptr += MAP_CELL_W*2;
			if (cellptr->IsMapped) index |= 0x04;
			cellptr += 2;
			if (cellptr->IsMapped) index |= 0x02;
			cellptr -= MAP_CELL_W*2;
			if (cellptr->IsMapped) index |= 0x01;
			value = DiagShadow[index];
		}

		/*
		**	Randomizer should go here. Add sets in multiples of 12.
		*/

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
 *   03/06/2019  ST : Use per-player mapping so we can track the shroud for all players        *
 *=============================================================================================*/
bool DisplayClass::Map_Cell(CELL cell, HouseClass * house, bool and_for_allies)
{
	// It's OK to do this if the house isn't the local player. ST - 3/6/2019 11:06AM
	//if (house != PlayerPtr || cell >= (CELL)Size) return(false);
	if (house == NULL || cell >= (CELL)Size) return(false);

	if (!house->IsHuman) {
		if (!ShareAllyVisibility || !and_for_allies) {
			return false;
		}
	}

	/*
	** Maybe also recurse to map for allies
	*/
	if (ShareAllyVisibility && and_for_allies && GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
		for (HousesType house_type = HOUSE_MULTI1; house_type < HOUSE_COUNT; house_type++) {
			HouseClass *hptr = HouseClass::As_Pointer(house_type);
			if (hptr && hptr->IsActive) {
				if (hptr != house && house->Is_Ally(hptr)) {
					Map_Cell(cell, hptr, false);
				}
			}
		}
	}
	
	/*
	**	Don't bother remapping this cell if it is already mapped.
	*/
	//if ((*this)[cell].IsMapped) {
	if ((*this)[cell].Is_Mapped(house)) {		// Check by player. ST - 3/6/2019 10:28AM
		return(false);
	}

	/*
	** Mark the cell as being mapped.
	*/
	//(*this)[cell].IsMapped = true;
	//(*this)[cell].IsVisible = true;
	// Set by player. ST - 3/6/2019 10:29AM
	(*this)[cell].Set_Mapped(house);
	(*this)[cell].Set_Visible(house);

	(*this)[cell].Redraw_Objects();

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

		if (c != cell && !(*this)[c].Is_Mapped(house)) {		// Check by player. ST - 3/6/2019 10:28AM
			shadow = Cell_Shadow(c, house);

			/*
			**	Either map the cell or mark it to be refreshed. It
			**	will probably change form if it isn't actually mapped.
			*/
			if (shadow == -1) {
				Map_Cell(c, house, false);
			} else {
				if (shadow != -2) {
					//(*this)[c].IsVisible = true;
					(*this)[c].Set_Visible(house);  // Set by player. ST - 3/6/2019 11:07AM	
					(*this)[c].Redraw_Objects();
				}
			}
		}
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
bool DisplayClass::Coord_To_Pixel(COORDINATE coord, int &x, int &y)
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


#if (0)                                 // reference. ST - 4/17/2019 9:07AM
bool DisplayClass::Coord_To_Pixel(COORDINATE coord, int &x, int &y)
{
	if (coord) {
		int xtac = Pixel_To_Lepton(Lepton_To_Pixel(Coord_X(TacticalCoord)));
		int xoff = Pixel_To_Lepton(Lepton_To_Pixel(Coord_X(coord)));

		xoff = (xoff+EDGE_ZONE) - xtac;
		if (xoff <= TacLeptonWidth + EDGE_ZONE*2) {
			int ytac = Pixel_To_Lepton(Lepton_To_Pixel(Coord_Y(TacticalCoord)));
			int yoff = Pixel_To_Lepton(Lepton_To_Pixel(Coord_Y(coord)));

			yoff = (yoff+EDGE_ZONE) - ytac;
			if (yoff <= TacLeptonHeight + EDGE_ZONE*2) {
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
bool DisplayClass::Push_Onto_TacMap(COORDINATE &source, COORDINATE &dest)
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
ObjectClass * DisplayClass::Cell_Object(CELL cell, int x, int y)
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
 *   12/15/1994 JLB : Updated to work with display heirarchy.                                  *
 *   12/24/1994 JLB : Examines redraw bit intelligently.                                       *
 *   12/24/1994 JLB : Combined with old Refresh_Map() function.                                *
 *   01/10/1995 JLB : Rubber band drawing.                                                     *
 *=============================================================================================*/
 void DisplayClass::Draw_It(bool forced)
{
	int	x,y;			// Working cell index values.

	MapClass::Draw_It(forced);

	if (IsToRedraw || forced) {
		IsToRedraw = false;

		/*
		**	In rubber band mode, mark all cells under the "rubber band" to be
		**	redrawn.
		*/
		Refresh_Band();

		/*
		** If the multiplayer message system is displaying one or more messages,
		** flag all cells covered by the messages to redraw.  This will prevent
		** messages from smearing the map if it scrolls.
		*/
		int num = Messages.Num_Messages();
		if (num) {
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
			if (num > 3) {
				for (cell = Coord_Cell(TacticalCoord) + MAP_CELL_W*3;
					cell < Coord_Cell(TacticalCoord) + MAP_CELL_W*3 + Lepton_To_Cell(TacLeptonWidth)+1; cell++) {
					(*this)[cell].Redraw_Objects();
				}
			}
			if (num > 4) {
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

			/*
			** Work out which map edges need to be redrawn
			*/
			BOOL redraw_right = (oldx < 0) ? TRUE : FALSE;		//Right hand edge
			BOOL redraw_left  = (oldx > 0) ? TRUE : FALSE;		//Left hand edge
			BOOL redraw_bottom= (oldy < 0) ? TRUE : FALSE;		//Bottom edge
			BOOL redraw_top	= (oldy > 0) ? TRUE : FALSE;		//Top edge


//Colour_Debug(2);
			/*
			**	Blit any replicable block to avoid having to drawstamp.
			*/
			CachedIconsDrawn=0;
			UnCachedIconsDrawn=0;
			if (!forced && (oldw != Lepton_To_Pixel(TacLeptonWidth) || oldh != Lepton_To_Pixel(TacLeptonHeight))) {
				Set_Cursor_Pos(-1);
				/*
				** If hid page is in video memory then we may nned to blit from the seen page to
				**  avoid blitting an overlapped region.
				*/
				if (HidPage.Get_IsDirectDraw() && !OverlappedVideoBlits){
					Hide_Mouse();
							SeenBuff.Blit(HidPage,
									((oldx < 0) ? -oldx : 0) +TacPixelX,
									((oldy < 0) ? -oldy : 0) +TacPixelY,
									((oldx < 0) ? 0 : oldx) +TacPixelX,
									((oldy < 0) ? 0 : oldy) +TacPixelY,
									oldw,
									oldh);
					Show_Mouse();
				}else{
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

				if (abs(oldx) < 0x25 && abs(oldy) < 0x25){

					/*
					** The width of the area we redraw depends on the scroll speed
					*/
					int extra_x = (abs(oldx)>=16) ? 2 : 1;
					int extra_y = (abs(oldy)>=16) ? 2 : 1;

					/*
					** Flag the cells across the top of the visible area if required
					*/
					if (redraw_top){
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
					if (redraw_bottom){
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
					if (redraw_left){
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
					if (redraw_right){
						for (x = Lepton_To_Pixel(TacLeptonWidth)-CELL_PIXEL_W*(extra_x+1); x <= Lepton_To_Pixel(TacLeptonWidth)+CELL_PIXEL_W*3; x += CELL_PIXEL_W) {
							for (y = starty; y <= Lepton_To_Pixel(TacLeptonHeight)+((CELL_PIXEL_H*2)); y += CELL_PIXEL_H) {
								CELL c = Click_Cell_Calc(Bound(x, 0, Lepton_To_Pixel(TacLeptonWidth)-1) + TacPixelX,
											Bound(y, 0, Lepton_To_Pixel(TacLeptonHeight)-1) + TacPixelY);

								if (c > 0) (*this)[c].Redraw_Objects(true);
							}
						}
					}

				}else{

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

		/*
		**	If the entire tactical map is forced to be redrawn, then set all the redraw flags
		**	and let the normal processing take care of the rest.
		*/
		if (forced) {
			CellRedraw.Set();
		}

//Colour_Debug(3);
		/*
		**	The first order of business is to redraw all the underlying icons that are
		**	flagged to be redrawn.
		*/
		//Redraw_Icons(CELL_BLIT_ONLY);
		Redraw_Icons(0);

		/*
		**	Once the icons are drawn, duplicate the bottom line of the screen into the phantom
		**	area one line below the screen. This causes the predator effect to work on any
		**	shape drawn at the bottom of the screen.
		*/
//Colour_Debug(4);
#ifdef FIX_ME_LATER
//		HidPage.Blit(HidPage, 0, HidPage.Get_Height()-1, 0, HidPage.Get_Height(), HidPage.Get_Width(), 1, false);
#endif //FIX_ME_LATER
		if (HidPage.Lock()){

			//Redraw_Icons(CELL_DRAW_ONLY);

			/*
			**	Redraw the game objects layer by layer. The layer drawing occurs on the ground layer
			**	first and then followed by all the layers in increasing altituded.
			*/
			for (LayerType layer = LAYER_GROUND; layer < LAYER_COUNT; layer++) {
				for (int index = 0; index < Layer[layer].Count(); index++) {
					Layer[layer][index]->Render(forced);
				}
			}

			/*
			**	Finally, redraw the shadow overlay as necessary.
			*/
//Colour_Debug(5);
			Redraw_Shadow();
		}

		Redraw_Shadow_Rects();

		HidPage.Unlock();

//Colour_Debug(8);
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
//Colour_Debug(0);

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
 *   12/06/1994 JLB : Scans tactical view in separate row/colum loops                          *
 *   12/24/1994 JLB : Uses the cell bit flag array to determine what to redraw.                *
 *=============================================================================================*/
void DisplayClass::Redraw_Icons(int draw_flags)
{
	IsShadowPresent = false;
	for (int y = -Coord_YLepton(TacticalCoord); y <= TacLeptonHeight; y += CELL_LEPTON_H) {
		for (int x = -Coord_XLepton(TacticalCoord); x <= TacLeptonWidth; x += CELL_LEPTON_W) {
			COORDINATE coord = Coord_Add(TacticalCoord, XY_Coord(x, y));
			CELL cell = Coord_Cell(coord);
			coord = Cell_Coord(cell) & 0xFF00FF00L;

			/*
			**	Only cells flagged to be redraw are examined.
			*/
			if (In_View(cell) && Is_Cell_Flagged(cell)) {
				int xpixel;
				int ypixel;

				if (Coord_To_Pixel(coord, xpixel, ypixel)) {
					CellClass * cellptr = &(*this)[Coord_Cell(coord)];

					/*
					**	If there is a portion of the underlying icon that could be visible,
					**	then draw it.  Also draw the cell if the shroud is off.
					*/
					if (cellptr->Is_Visible(PlayerPtr) || Debug_Unshroud) {			// Use PlayerPtr since we won't be rendering in MP. ST - 3/6/2019 2:49PM
						cellptr->Draw_It(xpixel, ypixel, draw_flags);
					}

					/*
					**	If any cell is not fully mapped, then flag it so that the shadow drawing
					**	process will occur.  Only draw the shadow if Debug_Unshroud is false.
					*/
					if (!cellptr->Is_Mapped(PlayerPtr) && !Debug_Unshroud) {			// Use PlayerPtr since we won't be rendering in MP. ST - 3/6/2019 2:49PM
						IsShadowPresent = true;
					}
				}
			}
		}
	}
}


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
				coord = Cell_Coord(cell) & 0xFF00FF00;

				/*
				**	Only cells flagged to be redraw are examined.
				*/
				if (In_View(cell) && Is_Cell_Flagged(cell)) {
					int xpixel;
					int ypixel;

					if (Coord_To_Pixel(coord, xpixel, ypixel)) {
						CellClass * cellptr = &(*this)[Coord_Cell(coord)];

						if (!cellptr->Is_Mapped(PlayerPtr)) {						// Pass player pointer since we will only be rendering in single player mode. ST - 3/6/2019 1:36PM
							if (cellptr->Is_Visible(PlayerPtr)) {					// Pass player pointer since we will only be rendering in single player mode. ST - 3/6/2019 1:36PM
								int shadow = Cell_Shadow(cell, PlayerPtr);		// Pass player pointer since we will only be rendering in single player mode. ST - 3/6/2019 1:36PM
								if (shadow >= 0) {
									CC_Draw_Shape(ShadowShapes, shadow, xpixel, ypixel, WINDOW_TACTICAL, SHAPE_GHOST, NULL, ShadowTrans);
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
void DisplayClass::Redraw_Shadow_Rects(void)
{
	if (IsShadowPresent) {
		for (int y = -Coord_YLepton(TacticalCoord); y <= TacLeptonHeight; y += CELL_LEPTON_H) {
			for (int x = -Coord_XLepton(TacticalCoord); x <= TacLeptonWidth; x += CELL_LEPTON_W) {
				COORDINATE coord = Coord_Add(TacticalCoord, XY_Coord(x, y));
				CELL cell = Coord_Cell(coord);
				coord = Cell_Coord(cell) & 0xFF00FF00;

				/*
				**	Only cells flagged to be redraw are examined.
				*/
				if (In_View(cell) && Is_Cell_Flagged(cell)) {
					int xpixel;
					int ypixel;

					if (Coord_To_Pixel(coord, xpixel, ypixel)) {
						CellClass * cellptr = &(*this)[Coord_Cell(coord)];

						if (!cellptr->Is_Mapped(PlayerPtr)) {							// Use PlayerPtr since we won't be rendering in MP. ST - 3/6/2019 2:49PM
							if (!cellptr->Is_Visible(PlayerPtr)) {						// Use PlayerPtr since we won't be rendering in MP. ST - 3/6/2019 2:49PM
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
ObjectClass * DisplayClass::Next_Object(ObjectClass * object)
{
	ObjectClass * firstobj = 0;
	bool foundmatch = false;

	if (!object) {
		foundmatch = true;
	}
	for (unsigned uindex = 0; uindex < (unsigned)Layer[LAYER_GROUND].Count(); uindex++) {
		ObjectClass * obj = Layer[LAYER_GROUND][uindex];

		/*
		**	Verify that the object can be selected by and is owned by the player.
		*/
		if (obj && obj->Is_Techno() && ((TechnoClass *)obj)->Is_Discovered_By_Player() && obj->Class_Of().IsSelectable && obj->Owner() == PlayerPtr->Class->House) {
			if (!firstobj) firstobj = obj;
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
ObjectClass * DisplayClass::Prev_Object(ObjectClass * object)
{
	ObjectClass * firstobj = 0;
	bool foundmatch = false;

	if (!object) {
		foundmatch = true;
	}
	for (int uindex = Layer[LAYER_GROUND].Count()-1; uindex >= 0; uindex--) {
		ObjectClass * obj = Layer[LAYER_GROUND][uindex];

		/*
		**	Verify that the object can be selected by and is owned by the player.
		*/
		if (obj && obj->Is_Techno() && ((TechnoClass *)obj)->Is_Discovered_By_Player() && obj->Class_Of().IsSelectable && obj->Owner() == PlayerPtr->Class->House) {
			if (!firstobj) firstobj = obj;
			if (foundmatch) return(obj);
			if (object == obj) foundmatch = true;
		}
	}
	return(firstobj);
}


/***********************************************************************************************
 * DisplayClass::Pixel_To_Coord -- converts screen coord to COORDINATE                              *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      x,y      pixel coordinates to convert                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      COORDINATE of pixel                                                                         *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/09/1994 BR : Created.                                                                  *
 *   12/06/1994 JLB : Uses map dimension variables in display class.                           *
 *   12/10/1994 JLB : Uses union to speed building coordinate value.                           *
 *=============================================================================================*/
COORDINATE DisplayClass::Pixel_To_Coord(int x, int y)
{
	/*
	**	Normalize the pixel coorindates to be relative to the upper left corner
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
	// Possibly ignore the view constraints if we aren't using the internal renderer. ST - 4/17/2019 9:06AM
	//if (x < TacLeptonWidth && y < TacLeptonHeight) {
	if (IgnoreViewConstraints || (x < TacLeptonWidth && y < TacLeptonHeight)) {
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
 *          house -- The house to base calculation on.                                         *
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
 *=============================================================================================*/
CELL DisplayClass::Calculated_Cell(SourceType dir, HousesType house)
{
	CELL	cell = 0;	// Working cell number.

	while (cell == 0) {
		int x,y;
		int index;

		/*
		**	Select a candidate cell based on the desired method.
		*/
		switch (dir) {

			/*
			**	Looks for the northern most straight path shipping lane and returns
			**	the cell of one of the ends.
			*/
			case SOURCE_SHIPPING:
				for (y = 0; y < MapCellHeight; y++) {
					for (x = 0; x < MapCellWidth; x++) {
						if ((*this)[XY_Cell(MapCellX+x, MapCellY+y)].Land_Type() != LAND_WATER) break;
					}
					if (x == MapCellWidth) {
						return(XY_Cell(MapCellX+MapCellWidth, MapCellY+y));
					}
				}
				return(0);

			/*
			**	Select a map edge.
			*/
			case SOURCE_NORTH:
				index = Random_Pick(1, MapCellWidth);
				for (x = 0; x < MapCellWidth; x++) {
					cell = XY_Cell(MapCellX+((x+index)%MapCellWidth), MapCellY-1);
					if ((*this)[cell].Is_Generally_Clear() && (*this)[cell+MAP_CELL_W].Is_Generally_Clear()) break;
				}
				if (x == MapCellWidth) return(0);
				break;

			case SOURCE_EAST:
				index = Random_Pick(1, MapCellHeight);
				for (y = 0; y < MapCellHeight; y++) {
					cell = XY_Cell(MapCellX+MapCellWidth, MapCellY+((y+index)%MapCellHeight));
					if ((*this)[cell].Is_Generally_Clear() && (*this)[cell-1].Is_Generally_Clear()) break;
				}
				if (y == MapCellHeight) return(0);
				break;

			case SOURCE_SOUTH:
				index = Random_Pick(1, MapCellWidth);
				for (x = 0; x < MapCellWidth; x++) {
					cell = XY_Cell(MapCellX+((x+index)%MapCellWidth), MapCellY+MapCellHeight);
					if ((*this)[cell].Is_Generally_Clear() && (*this)[cell-MAP_CELL_W].Is_Generally_Clear()) break;
				}
				if (x == MapCellWidth) return(0);
				break;

			case SOURCE_WEST:
				index = Random_Pick(1, MapCellHeight);
				for (y = 0; y < MapCellHeight; y++) {
					cell = XY_Cell(MapCellX-1, MapCellY+((y+index)%MapCellHeight));
					if ((*this)[cell].Is_Generally_Clear() && (*this)[cell+1].Is_Generally_Clear()) break;
				}
				if (y == MapCellHeight) return(0);
				break;

			/*
			**	Drop in at a random location.
			*/
			case SOURCE_AIR:
				cell = Waypoint[WAYPT_REINF];
				if (cell < 1) {
					cell = Coord_Cell(TacticalCoord);
					return(cell);
				} else {
					if ((*this)[cell].Cell_Techno()) {
						for (int radius = 1; radius < 7; radius++) {
							CELL newcell = Coord_Cell(Coord_Scatter(Cell_Coord(cell), radius << 8, true));
							if (In_Radar(newcell) && !(*this)[newcell].Cell_Techno()) {
								cell = newcell;
								break;
							}
						}
					}
				}
				break;

			/*
			**	Dramatic entry point is somewhere on the visible screen as defined
			**	by the current tactical map position.
			*/
			case SOURCE_VISIBLE:
				cell = XY_Cell(Coord_XCell(TacticalCoord)+Random_Pick(0, Lepton_To_Cell(TacLeptonWidth)-1), Coord_YCell(TacticalCoord)+Random_Pick(0, Lepton_To_Cell(TacLeptonHeight)-1));
				if (house == PlayerPtr->Class->House && !In_Radar(cell)) {
					cell = 0;
				}
				break;

			/*
			**	Drop off near friendly base or near a friendly unit or
			**	just randomly if all else fails.
			*/
			case SOURCE_ENEMYBASE:
			case SOURCE_HOMEBASE:
				return(0);

			/*
			**	Find an unoccupied beach cell.
			*/
			case SOURCE_BEACH: {
				CELL	cells[MAP_CELL_W];
				CELL	alternate[MAP_CELL_W];
				unsigned	counter=0;

				for (x = 0; x < MapCellWidth; x++) {
					CELL	newcell = 0;

					if ((*this)[XY_Cell(x + MapCellX, MapCellHeight + MapCellY)].Land_Type() != LAND_WATER) continue;
					if ((*this)[XY_Cell(x + MapCellX, MapCellHeight + MapCellY-1)].Land_Type() != LAND_WATER) continue;
					for (y = MapCellHeight; y >= 0; y--) {
						newcell = XY_Cell(x + MapCellX, y + MapCellY);
						if ((*this)[newcell].Cell_Techno()) {
							break;
						}
						if ((*this)[newcell].Land_Type() != LAND_WATER) {
							break;
						}
					}
					LandType land = (*this)[newcell].Land_Type();
					if (( land == LAND_BEACH || land == LAND_CLEAR || land == LAND_ROAD) &&
						!(*this)[newcell].Cell_Techno() &&
						!(*this)[newcell].Cell_Terrain() &&
						!(*this)[newcell-MAP_CELL_W].Cell_Techno() &&
						!(*this)[newcell-MAP_CELL_W].Cell_Terrain() &&
						!(*this)[newcell-(MAP_CELL_W*2)].Cell_Terrain() &&
						!(*this)[newcell-(MAP_CELL_W*2)].Cell_Techno()) {

						cells[counter++] = newcell;
						if (counter >= (sizeof(cells) / sizeof(cells[0]))) {
							break;
						}
					}
				}

				/*
				**	Fixup entry list so that it doesn't come close to blocking terrain or other
				**	units.
				*/
				int counter2 = 0;
				for (int index = 1; index < (int)counter-1; index++) {
					if (Cell_X(cells[index-1])+1 == Cell_X(cells[index]) && Cell_X(cells[index+1])-1 == Cell_X(cells[index])) {
						alternate[counter2++] = cells[index];
					}
				}

				CELL cell = 0;
				if (counter2) {
					if (counter2 < 4) {
						cell = alternate[counter2-1];
					} else {
						cell = alternate[counter2 - (counter2 / 4)];
					}
				} else {
					if (counter) {
						if (counter < 4) {
							cell = cells[counter-1];
						} else {
							cell = cells[counter - (counter / 4)];
						}
					}
				}
				if (cell) {
					if (Map.Theater == THEATER_DESERT) {
						cell += MAP_CELL_W;
					}
				}
				return(cell);
			}

			case SOURCE_OCEAN:
				cell = XY_Cell(Random_Pick(0, MapCellWidth-2)+MapCellX, MapCellHeight+MapCellY);
				break;

			default:
				return(0);
		}

		/*
		**	The selected edge cell must be unoccupied and if this is for
		**	the player, then it must be on an accessible map cell.
		*/
		cell &= 0x0FFF;
		if (cell && (*this)[cell].Cell_Techno()) {
			cell = 0;
		}
	}
	return(cell);
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
		**	Only try to select objects that are owned by the player, are allowed to be
		**	selected, and are within the bouding box.
		*/
		if (	obj->Class_Of().IsSelectable &&
				obj->What_Am_I() != RTTI_BUILDING &&
				x >= x1 && x <= x2 && y >= y1 && y <= y2) {
			bool old_allow_voice = AllowVoice;
			bool is_player_controlled = obj->Owner() == PlayerPtr->Class->House;
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
		COORDINATE ocoord = Coord_Add(aircraft->Center_Coord(), XY_Coord(0, -Pixel_To_Lepton(aircraft->Altitude)));
		int x = Coord_X(ocoord);
		int y = Coord_Y(ocoord);

		/*
		**	Only try to select objects that are owned by the player, are allowed to be
		**	selected, and are within the bounding box.
		*/
		if (	aircraft->Class_Of().IsSelectable &&
				!aircraft->Is_Selected_By_Player() &&
				x >= x1 && x <= x2 && y >= y1 && y <= y2) {
			bool old_allow_voice = AllowVoice;
			bool is_player_controlled = aircraft->Owner() == PlayerPtr->Class->House;
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
		if (CurrentObject[i]->Owner() == PlayerPtr->Class->House) {
			player_controlled_units = true;
		}
		else {
			non_player_controlled_units = true;
		}
	}
	if (player_controlled_units && non_player_controlled_units) {
		for (int i = 0; i < CurrentObject.Count(); ++i) {
			HouseClass * hptr = HouseClass::As_Pointer(CurrentObject[i]->Owner());
			if (CurrentObject[i]->Owner() != PlayerPtr->Class->House) {
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
	ObjectClass *object = 0;
	ActionType action = ACTION_NONE;		// Action possible with currently selected object.

	/*
	**	Set some working variables that depend on the mouse position. For the press
	**	or release event, special mouse queuing storage variables are used. Other
	**	events must use the current mouse position globals.
	*/
	bool edge = false;
	if (flags & (LEFTPRESS|LEFTRELEASE|RIGHTPRESS|RIGHTRELEASE)) {
		x = _Kbd->MouseQX;
		y = _Kbd->MouseQY;
	} else {
		x = Get_Mouse_X();
		y = Get_Mouse_Y();

		if (x == 0 || y == 199 || x == 319) edge = true;
	}
	COORDINATE coord = Map.Pixel_To_Coord(x, y);
	CELL cell = Coord_Cell(coord);
//	CELL cell = Map.Click_Cell_Calc(x, y);
	if (coord) {
		shadow = (!Map[cell].Is_Visible(PlayerPtr) && !Debug_Unshroud);					// Use PlayerPtr since we won't be rendering in MP. ST - 3/6/2019 2:49PM
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
//			int xxx = x + Lepton_To_Pixel(Coord_XLepton(Map.TacticalCoord));
//			int yyy = y + Lepton_To_Pixel(Coord_YLepton(Map.TacticalCoord));
//			object = Map.Cell_Object(cell, xxx % CELL_PIXEL_W, yyy % CELL_PIXEL_H);
			object = Map.Close_Object(coord);

			/*
			**	Special case check to ignore cloaked object if not owned by the player.
			*/
//			if (object && object->Is_Techno() && !((TechnoClass *)object)->IsOwnedByPlayer && ((TechnoClass *)object)->Cloak == CLOAKED) {
//				object = NULL;
//			}
		}

		/*
		**	If there is a currently selected object, then the action to perform if
		**	the left mouse button were clicked must be determined.
		*/
		if (CurrentObject.Count()) {
			if (object) {
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

			if (Map.IsTargettingMode == SPC_ION_CANNON) {
				action = ACTION_ION;
			}

			if (Map.IsTargettingMode == SPC_NUCLEAR_BOMB) {
				action = ACTION_NUKE_BOMB;
			}

			if (Map.IsTargettingMode == SPC_AIR_STRIKE) {
				action = ACTION_AIR_STRIKE;
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
				Map.Mouse_Left_Up(shadow, object, action);
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
		x = _Kbd->MouseQX;
		y = _Kbd->MouseQY;
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
		x = _Kbd->MouseQX;
		y = _Kbd->MouseQY;
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
				if (IsTargettingMode) {
					IsTargettingMode = false;
				} else {
					Unselect_All();
				}
			}
		}
	}
	Set_Default_Mouse(MOUSE_NORMAL, false);
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
void DisplayClass::Mouse_Left_Up(bool shadow, ObjectClass * object, ActionType action, bool wwsmall)
{
	IsTentative = false;

	/*
	**	Don't allow selection of an object that is located in shadowed terrain.
	**	In fact, just show the normal move cursor in order to keep the shadowed
	**	terrain a mystery.
	*/
	if (shadow) {
		switch (action) {
			case ACTION_GUARD_AREA:
				Set_Default_Mouse(MOUSE_AREA_GUARD, wwsmall);
				break;

			case ACTION_NONE:
				Set_Default_Mouse(MOUSE_NORMAL, wwsmall);
				break;

			case ACTION_NO_SELL:
			case ACTION_SELL:
			case ACTION_SELL_UNIT:
				Set_Default_Mouse(MOUSE_NO_SELL_BACK, wwsmall);
				break;

			case ACTION_NO_REPAIR:
			case ACTION_REPAIR:
				Set_Default_Mouse(MOUSE_NO_REPAIR, wwsmall);
				break;

			case ACTION_ION:
				Set_Default_Mouse(MOUSE_ION_CANNON, wwsmall);
				break;

			case ACTION_NUKE_BOMB:
				Set_Default_Mouse(MOUSE_NUCLEAR_BOMB, wwsmall);
				break;

			case ACTION_AIR_STRIKE:
				Set_Default_Mouse(MOUSE_AIR_STRIKE, wwsmall);
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
					Set_Default_Mouse(mouse_type, wwsmall);
					break;
				}
				// Fall into next case for non aircraft object types.

			default:
				Set_Default_Mouse(MOUSE_CAN_MOVE, wwsmall);
				break;

		}
	} else {

		/*
		**	Change the mouse shape according to the default action that will occur
		**	if the mouse button were clicked at this location.
		*/
		switch (action) {
			case ACTION_TOGGLE_SELECT:
			case ACTION_SELECT:
				Set_Default_Mouse(MOUSE_CAN_SELECT, wwsmall);
				break;

			case ACTION_MOVE:
				Set_Default_Mouse(MOUSE_CAN_MOVE, wwsmall);
				break;

			case ACTION_GUARD_AREA:
				Set_Default_Mouse(MOUSE_AREA_GUARD, wwsmall);
				break;

			case ACTION_HARVEST:
			case ACTION_ATTACK:
				Set_Default_Mouse(MOUSE_CAN_ATTACK, wwsmall);
				break;

			case ACTION_SABOTAGE:
				Set_Default_Mouse(MOUSE_DEMOLITIONS, wwsmall);
				break;

			case ACTION_ENTER:
			case ACTION_CAPTURE:
				Set_Default_Mouse(MOUSE_ENTER, wwsmall);
				break;

			case ACTION_NOMOVE:
				Set_Default_Mouse(MOUSE_NO_MOVE, wwsmall);
				break;

			case ACTION_NO_SELL:
				Set_Default_Mouse(MOUSE_NO_SELL_BACK, wwsmall);
				break;

			case ACTION_NO_REPAIR:
				Set_Default_Mouse(MOUSE_NO_REPAIR, wwsmall);
				break;

			case ACTION_SELF:
				Set_Default_Mouse(MOUSE_DEPLOY, wwsmall);
				break;

			case ACTION_REPAIR:
				Set_Default_Mouse(MOUSE_REPAIR, wwsmall);
				break;

			case ACTION_SELL_UNIT:
				Set_Default_Mouse(MOUSE_SELL_UNIT, wwsmall);
				break;

			case ACTION_SELL:
				Set_Default_Mouse(MOUSE_SELL_BACK, wwsmall);
				break;

			case ACTION_ION:
				Set_Default_Mouse(MOUSE_ION_CANNON, wwsmall);
				break;

			case ACTION_NUKE_BOMB:
				Set_Default_Mouse(MOUSE_NUCLEAR_BOMB, wwsmall);
				break;

			case ACTION_AIR_STRIKE:
				Set_Default_Mouse(MOUSE_AIR_STRIKE, wwsmall);
				break;

			case ACTION_TOGGLE_PRIMARY:
				Set_Default_Mouse(MOUSE_DEPLOY, wwsmall);
				break;

			default:
				Set_Default_Mouse(MOUSE_NORMAL, wwsmall);
				break;
		}
	}
#if 0
	/*
	**	Give a generic help message when over shadow terrain.
	*/
	if (shadow) {
		if (Scenario < 4) {
			Help_Text(TXT_SHADOW);
		} else {
			Help_Text(TXT_NONE);
		}
	} else {

		/*
		**	If the mouse is held over objects on the map, then help text may
		**	pop up that tells what the object is. This call informs the help
		**	system of the text name for the object under the mouse.
		*/
		if (object) {
			int text;
			int color = LTGREY;

			/*
			**	Fetch the appropriate background color for help text.
			*/
			if (PlayerPtr->Is_Ally(object)) {
				color = CC_GREEN;
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

				if (!PlayerPtr->Is_Ally(object)) {
					switch (object->What_Am_I()) {
						case RTTI_INFANTRY:
							text = TXT_ENEMY_SOLDIER;
							break;

						case RTTI_UNIT:
							text = TXT_ENEMY_VEHICLE;
							break;

						case RTTI_BUILDING:
							if ( *((BuildingClass*)object) != STRUCT_MISSION) {
								text = TXT_ENEMY_STRUCTURE;
							}
							break;
					}
				}
			}

			if (Scenario > 3 || object->What_Am_I() != RTTI_TERRAIN) {
				Help_Text(text, -1, -1, color);
			} else {
				Help_Text(TXT_NONE);
			}
		} else {
			Help_Text(TXT_NONE);
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
void DisplayClass::Mouse_Left_Release(CELL cell, int x, int y, ObjectClass * object, ActionType action, bool wwsmall)
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
			Select_These(XYPixel_Coord(BandX, BandY), XYPixel_Coord(x, y));

			Set_Default_Mouse(MOUSE_NORMAL, wwsmall);
#ifdef NEVER
			if (CurrentObject.Count()) {
				if (CurrentObject[0]->Can_Player_Fire()) {
					Set_Default_Mouse(MOUSE_CAN_ATTACK, wwsmall);
				} else {
					Set_Default_Mouse(MOUSE_NORMAL, wwsmall);
				}
			} else {
				Set_Default_Mouse(MOUSE_NORMAL, wwsmall);
			}
#endif

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
					Set_Default_Mouse(MOUSE_NORMAL, wwsmall);
				}
#ifdef NEVER
				if (object->Can_Player_Fire()) {
					Set_Default_Mouse(MOUSE_CAN_ATTACK, wwsmall);
				} else {
					Set_Default_Mouse(MOUSE_NORMAL, wwsmall);
				}
#endif
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
				for (int index = 0; index < CurrentObject.Count(); index++) {
					ObjectClass * tobject = CurrentObject[index];
					if (object) {
						tobject->Active_Click_With(tobject->What_Action(object), object);
					} else {
						tobject->Active_Click_With(tobject->What_Action(cell), cell);
					}
					AllowVoice = false;
				}
				AllowVoice = true;

				if (action == ACTION_REPAIR && object->What_Am_I() == RTTI_BUILDING) {
					OutList.Add(EventClass(EventClass::REPAIR, object->As_Target()));
				}
				if (action == ACTION_SELL_UNIT && object) {
					switch (object->What_Am_I()) {
						case RTTI_AIRCRAFT:
						case RTTI_UNIT:
							OutList.Add(EventClass(EventClass::SELL, object->As_Target()));
							break;

						default:
							break;
					}

				}
				if (action == ACTION_SELL) {
					if (object) {
						OutList.Add(EventClass(EventClass::SELL, object->As_Target()));
					} else {
						OutList.Add(EventClass(EventClass::SELL, ::As_Target(cell)));
					}
				}
				if (action == ACTION_ION) {
					OutList.Add(EventClass(EventClass::SPECIAL_PLACE, SPC_ION_CANNON, cell));
				}
				if (action == ACTION_NUKE_BOMB) {
					OutList.Add(EventClass(EventClass::SPECIAL_PLACE, SPC_NUCLEAR_BOMB, cell));
				}
				if (action == ACTION_AIR_STRIKE) {
					OutList.Add(EventClass(EventClass::SPECIAL_PLACE, SPC_AIR_STRIKE, cell));
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
	if (!IsRepairMode && !IsSellMode && !IsTargettingMode && !PendingObject) {
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
	int xx = 0;// Coord_X(coord) - Cell_To_Lepton(MapCellX);
	int yy = 0;// Coord_Y(coord) - Cell_To_Lepton(MapCellY);

	Confine_Rect(&xx, &yy, TacLeptonWidth, TacLeptonHeight, Cell_To_Lepton(MapCellWidth) + GlyphXClientSidebarWidthInLeptons, Cell_To_Lepton(MapCellHeight));		// Needed to accomodate Glyphx client sidebar. ST - 4/12/2019 5:29PM
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
		if (!infp->IsInLimbo && infp->House == PlayerPtr) {
			x += (long)Coord_XCell (infp->Coord);
			y += (long)Coord_YCell (infp->Coord);
			num++;
		}
	}

	for (i = 0; i < Units.Count(); i++) {
		UnitClass * unitp = Units.Ptr(i);
		if (!unitp->IsInLimbo && unitp->House == PlayerPtr) {
			x += (long)Coord_XCell (unitp->Coord);
			y += (long)Coord_YCell (unitp->Coord);
			num++;
		}
	}

	for (i = 0; i < Buildings.Count(); i++) {
		BuildingClass * bldgp = Buildings.Ptr(i);
		if (!bldgp->IsInLimbo && bldgp->House == PlayerPtr) {
			x += (((long)Coord_XCell (bldgp->Coord)) << 4);
			y += (((long)Coord_YCell (bldgp->Coord)) << 4);
			num += 16;
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
bool DisplayClass::In_View(register CELL cell)
{
	COORDINATE coord = Cell_Coord(cell) & 0xFF00FF00L;
	COORDINATE tcoord = TacticalCoord & 0xFF00FF00L;

	if ((Coord_X(coord) - Coord_X(tcoord)) > TacLeptonWidth+255) return(false);
	if ((Coord_Y(coord) - Coord_Y(tcoord)) > TacLeptonHeight+255) return(false);
	return(true);

#ifdef OBSOLETE
	int fudgex = Coord_XLepton(TacticalCoord) ? -1 : 0;
	int fudgey = Coord_YLepton(TacticalCoord) ? -1 : 0;
	if ((unsigned)(Cell_X(cell)-Coord_XCell(TacticalCoord)) > Lepton_To_Cell(TacLeptonWidth)+fudgex) return(false);
	if ((unsigned)(Cell_Y(cell)-Coord_YCell(TacticalCoord)) > Lepton_To_Cell(TacLeptonHeight)+fudgey) return(false);
	return(true);
#endif

#ifdef OBSOLETE
	cell -= TacticalCell;

	if (Cell_X(cell) >= TacWidth + (TacPartialX ? 1 : 0)) return(false);
	if (Cell_Y(cell) >= TacHeight + (TacPartialY ? 1 : 0)) return(false);
	return(true);
#endif
}


COORDINATE DisplayClass::Closest_Free_Spot(COORDINATE coord, bool any) const
{
	if (coord & 0xC000C000) {
		return(0x00800080);
	}
	return (*this)[Coord_Cell(coord)].Closest_Free_Spot(coord, any);
}


bool DisplayClass::Is_Spot_Free(COORDINATE coord) const
{
	// This doesn't seem right... ST - 12/18/2018 10:09AM
	//if (coord & 0xC000C000) {
	//	return(0x00800080);
	//}
	return (*this)[Coord_Cell(coord)].Is_Spot_Free(CellClass::Spot_Index(coord));
}


/***********************************************************************************************
 * DisplayClass::Center_Map -- Centers the map about the currently selected objects            *
 *                                                                                             *
 *    This routine will average the position of all the selected objects and then center       *
 *    the map about those objects.                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  The center coordinate.                                                             *
 *                                                                                             *
 * WARNINGS:   The map position changes by this routine.                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE DisplayClass::Center_Map(void)
{
	if (CurrentObject.Count()) {
		unsigned x = 0;
		unsigned y = 0;

		for (int index = 0; index < CurrentObject.Count(); index++) {
			COORDINATE coord = CurrentObject[index]->Center_Coord();

			x += Coord_X(coord);
			y += Coord_Y(coord);
		}

		x /= CurrentObject.Count();
		y /= CurrentObject.Count();
		Set_Tactical_Position(XY_Coord(x - (TacLeptonWidth/2), y - (TacLeptonHeight/2)));

		return XY_Coord(x, y);
	}

	return 0;
}

static ActionType _priority_actions[] = {
	ACTION_ATTACK,
	ACTION_ENTER,
	ACTION_REPAIR,
	ACTION_SABOTAGE,
	ACTION_CAPTURE,
	ACTION_MOVE
};

static int get_action_priority(ActionType action)
{
	for (int i = 0; i < sizeof(_priority_actions) / sizeof(_priority_actions[0]); ++i) {
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

ObjectClass* Best_Object_With_Action(DynamicVectorClass<ObjectClass*>& objects, ObjectClass* object)
{
	return Best_Object_With_ActionT(objects, object);
}

ObjectClass* Best_Object_With_Action(DynamicVectorClass<ObjectClass*>& objects, CELL cell)
{
	return Best_Object_With_ActionT(objects, cell);
}

ActionType Best_Object_Action(DynamicVectorClass<ObjectClass*>& objects, ObjectClass* object)
{
	ObjectClass* obj = Best_Object_With_Action(objects, object);
	return (obj != NULL) ? obj->What_Action(object) : ACTION_NONE;
}

ActionType Best_Object_Action(DynamicVectorClass<ObjectClass*>& objects, CELL cell)
{
	ObjectClass* obj = Best_Object_With_Action(objects, cell);
	return (obj != NULL) ? obj->What_Action(cell) : ACTION_NONE;
}

ObjectClass* Best_Object_With_Action(ObjectClass* object)
{
	return Best_Object_With_Action(CurrentObject.Raw(), object);
}

ObjectClass* Best_Object_With_Action(CELL cell)
{
	return Best_Object_With_Action(CurrentObject.Raw(), cell);
}

ActionType Best_Object_Action(ObjectClass* object)
{
	return Best_Object_Action(CurrentObject.Raw(), object);
}

ActionType Best_Object_Action(CELL cell)
{
	return Best_Object_Action(CurrentObject.Raw(), cell);
}