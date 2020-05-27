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

/* $Header: /CounterStrike/SOUNDDLG.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SOUNDDLG.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Maria del Mar McCready-Legg, Joe L. Bostic                   *
 *                                                                                             *
 *                   Start Date : Jan 8, 1995                                                  *
 *                                                                                             *
 *                  Last Update : September 22, 1995 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   MusicListClass::Draw_Entry -- Draw the score line in a list box.                          *
 *   SoundControlsClass::Process -- Handles all the options graphic interface.                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"sounddlg.h"

class MusicListClass : public ListClass
{
	public:
		MusicListClass(int id, int x, int y, int w, int h) :
			ListClass(id, x, y, w, h, TPF_6PT_GRAD|TPF_NOSHADOW, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"))
		{};
		virtual ~MusicListClass(void) {};

	protected:
		virtual void Draw_Entry(int index, int x, int y, int width, int selected);
};


/***********************************************************************************************
 * SoundControlsClass::Process -- Handles all the options graphic interface.                   *
 *                                                                                             *
 *    This routine is the main control for the visual representation of the options            *
 *    screen. It handles the visual overlay and the player input.                              *
 *                                                                                             *
 * INPUT:      none                                                                            *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:    12/31/1994 MML : Created.                                                       *
 *=============================================================================================*/
void SoundControlsClass::Process(void)
{

	/*
	** Adjust dialog controls for resolution
	*/
	int option_width=   	OPTION_WIDTH * RESFACTOR;
	int option_height=  	OPTION_HEIGHT * RESFACTOR;

	int option_x=       	OPTION_X * RESFACTOR;
	int option_y=       	OPTION_Y * RESFACTOR;

	int listbox_x=      	LISTBOX_X * RESFACTOR;
	int listbox_y=      	LISTBOX_Y * RESFACTOR;
	int listbox_w=      	LISTBOX_W * RESFACTOR;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
#ifdef WIN32
	int listbox_h=      	(LISTBOX_H * RESFACTOR)+2;
#else
	int listbox_h=      	LISTBOX_H * RESFACTOR;
#endif
#else
	int listbox_h=      	LISTBOX_H * RESFACTOR;
#endif

	int button_width=   	BUTTON_WIDTH * RESFACTOR;
	int button_x=       	BUTTON_X * RESFACTOR;
	int button_y=       	BUTTON_Y * RESFACTOR;

	int stop_x=         	STOP_X * RESFACTOR;
	int stop_y=         	STOP_Y * RESFACTOR;

	int play_x=         	PLAY_X * RESFACTOR;
	int play_y=         	PLAY_Y * RESFACTOR;

	int onoff_width=    	ONOFF_WIDTH * RESFACTOR;
	int shuffle_x=      	SHUFFLE_X * RESFACTOR;
	int shuffle_y=      	SHUFFLE_Y * RESFACTOR;
	int repeat_x=       	REPEAT_X * RESFACTOR;
	int repeat_y=       	REPEAT_Y * RESFACTOR;

	int mslider_x=      	MSLIDER_X * RESFACTOR;
	int mslider_y=      	MSLIDER_Y * RESFACTOR;
	int mslider_w=      	MSLIDER_W * RESFACTOR;
	int mslider_height= 	MSLIDER_HEIGHT * RESFACTOR;

	int fxslider_x=     	FXSLIDER_X * RESFACTOR;
	int fxslider_y=     	FXSLIDER_Y * RESFACTOR;
	int fxslider_w=     	FXSLIDER_W * RESFACTOR;
	int fxslider_height=	FXSLIDER_HEIGHT * RESFACTOR;

	int button_stop=    	BUTTON_STOP;
	int button_play=    	BUTTON_PLAY;
	int button_shuffle= 	BUTTON_SHUFFLE;
	int button_repeat=  	BUTTON_REPEAT;
	int button_options= 	BUTTON_OPTIONS;
	int slider_music=   	SLIDER_MUSIC;
	int slider_sound=   	SLIDER_SOUND;
	int button_listbox= 	BUTTON_LISTBOX;


	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();
//	ThemeType theme;

	/*
	**	List box that holds the score text strings.
	*/
	MusicListClass listbox(0, option_x+listbox_x, option_y+listbox_y, listbox_w, listbox_h);

	/*
	**	Return to options menu button.
	*/
	TextButtonClass returnto(BUTTON_OPTIONS, TXT_OK, TPF_BUTTON, option_x+button_x, option_y+button_y, button_width);
//	TextButtonClass returnto(BUTTON_OPTIONS, TXT_OPTIONS_MENU, TPF_BUTTON,

	/*
	**	Stop playing button.
	*/
	ShapeButtonClass stopbtn(BUTTON_STOP, MFCD::Retrieve("BTN-ST.SHP"), option_x+stop_x, option_y+stop_y);

	/*
	**	Start playing button.
	*/
	ShapeButtonClass playbtn(BUTTON_PLAY, MFCD::Retrieve("BTN-PL.SHP"), option_x+play_x, option_y+play_y);

	/*
	**	Shuffle control.
	*/
	TextButtonClass shufflebtn(BUTTON_SHUFFLE, TXT_OFF, TPF_BUTTON, option_x+shuffle_x, option_y+shuffle_y, onoff_width);
//	TextButtonClass shufflebtn(BUTTON_SHUFFLE, TXT_OFF, TPF_BUTTON, option_x+shuffle_x, option_y+shuffle_y, ONOFF_WIDTH);

	/*
	**	Repeat control.
	*/
	TextButtonClass repeatbtn(BUTTON_REPEAT, TXT_OFF, TPF_BUTTON, option_x+repeat_x, option_y+repeat_y, onoff_width);

	/*
	**	Music volume slider.
	*/
	SliderClass music(SLIDER_MUSIC, option_x+mslider_x, option_y+mslider_y, mslider_w, mslider_height, true);

	/*
	**	Sound volume slider.
	*/
	SliderClass sound(SLIDER_SOUND, option_x+fxslider_x, option_y+fxslider_y, fxslider_w, fxslider_height, true);

	/*
	**	Causes left mouse clicks inside the dialog area, but not on any
	**	particular button, to be ignored.
	*/
	GadgetClass area(option_x, option_y, option_width, option_height, GadgetClass::LEFTPRESS);

	/*
	**	Causes right clicks anywhere or left clicks outside of the dialog
	**	box area to be the same a clicking the return to game options button.
	*/
	ControlClass ctrl(BUTTON_OPTIONS, 0, 0, SeenBuff.Get_Width(), SeenBuff.Get_Height(), GadgetClass::RIGHTPRESS|GadgetClass::LEFTPRESS);

	/*
	**	The repeat and shuffle buttons are of the toggle type. They toggle
	**	between saying "on" and "off".
	*/
	shufflebtn.IsToggleType = true;
	if (Options.IsScoreShuffle) {
		shufflebtn.Turn_On();
	} else {
		shufflebtn.Turn_Off();
	}
	shufflebtn.Set_Text(shufflebtn.IsOn ? TXT_ON : TXT_OFF);

	repeatbtn.IsToggleType = true;
	if (Options.IsScoreRepeat) {
		repeatbtn.Turn_On();
	} else {
		repeatbtn.Turn_Off();
	}
	repeatbtn.Set_Text(repeatbtn.IsOn ? TXT_ON : TXT_OFF);

	/*
	**	Set the initial values of the sliders.
	*/
	music.Set_Maximum(255);
	music.Set_Thumb_Size(16);
	music.Set_Value(Options.ScoreVolume * 256);
	sound.Set_Maximum(255);
	sound.Set_Thumb_Size(16);
	sound.Set_Value(Options.Volume * 256);

	/*
	**	Set up the window.  Window x-coords are in bytes not pixels.
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Create Buttons.
	*/
	GadgetClass * optionsbtn = &returnto;
	listbox.Add_Tail(*optionsbtn);
	stopbtn.Add_Tail(*optionsbtn);
	playbtn.Add_Tail(*optionsbtn);
	shufflebtn.Add_Tail(*optionsbtn);
	repeatbtn.Add_Tail(*optionsbtn);
	music.Add_Tail(*optionsbtn);
	sound.Add_Tail(*optionsbtn);
	area.Add_Tail(*optionsbtn);
	ctrl.Add_Tail(*optionsbtn);

	/*
	**	Add all the themes to the list box. The list box entries are constructed
	**	and then stored into allocated EMS memory blocks.
	*/
	for (ThemeType index = THEME_FIRST; index < Theme.Max_Themes(); index++) {
		if (Theme.Is_Allowed(index)) {
			char buffer[100];
			int length = Theme.Track_Length(index);
			char const * fullname = Theme.Full_Name(index);

			void * ptr = new char [sizeof(buffer)];
			if (ptr) {
				sprintf((char *)ptr, "%cTrack %d\t%d:%02d\t%s", index, listbox.Count()+1, length / 60, length % 60, fullname);
				listbox.Add_Item((char const *)ptr);
			}

			if (Theme.What_Is_Playing() == index) {
				listbox.Set_Selected_Index(listbox.Count()-1);
			}
		}
	}
	static int _tabs[] = {
		55 * RESFACTOR, 72 * RESFACTOR, 90 * RESFACTOR
	};
	listbox.Set_Tabs(_tabs);

	/*
	**	Main Processing Loop.
	*/
	bool display = true;
	bool process = true;

	while (process) {

		/*
		**	Invoke game callback.
		*/
		if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
			Call_Back();
		} else {
			if (Main_Loop()) {
				process = false;
			}
		}

		#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = true;
		}
		#endif
		/*
		**	Refresh display if needed.
		*/
		if (display) {

			Hide_Mouse();

			/*
			**	Draw the background.
			*/
			Dialog_Box(option_x, option_y, option_width, option_height);

			Draw_Caption(TXT_SOUND_CONTROLS, option_x, option_y, option_width);

			/*
			** Draw the Music, Speech & Sound titles.
			*/
			Fancy_Text_Print(TXT_MUSIC_VOLUME, option_x+mslider_x-(5 * RESFACTOR), option_y+mslider_y-(2 * RESFACTOR), scheme, TBLACK, TPF_TEXT|TPF_RIGHT);
			Fancy_Text_Print(TXT_SOUND_VOLUME, option_x+fxslider_x-(5 * RESFACTOR), option_y+fxslider_y-(2 * RESFACTOR), scheme, TBLACK, TPF_TEXT|TPF_RIGHT);

#if defined(GERMAN) || defined(FRENCH)
			Fancy_Text_Print(TXT_SHUFFLE, option_x+4+shuffle_x-(5 * RESFACTOR), option_y+shuffle_y+(1 * RESFACTOR), scheme, TBLACK, TPF_TEXT|TPF_RIGHT);
#else
			Fancy_Text_Print(TXT_SHUFFLE, option_x+shuffle_x-(5 * RESFACTOR), option_y+shuffle_y+(1 * RESFACTOR), scheme, TBLACK, TPF_TEXT|TPF_RIGHT);
#endif
			Fancy_Text_Print(TXT_REPEAT, option_x+repeat_x-(5 * RESFACTOR), option_y+repeat_y+(1 * RESFACTOR), scheme, TBLACK, TPF_TEXT|TPF_RIGHT);

			optionsbtn->Draw_All();
			Show_Mouse();
			display = false;
		}

		/*
		**	Get user input.
		*/
		KeyNumType input = optionsbtn->Input();

		/*
		**	Process Input.
		*/
		switch (input) {

			case KN_ESC:
			case BUTTON_OPTIONS|KN_BUTTON:
				process = false;
				break;

			/*
			**	Control music volume.
			*/
			case SLIDER_MUSIC|KN_BUTTON:
				Options.Set_Score_Volume(fixed(music.Get_Value(), 256), true);
#ifdef FIXIT_VERSION_3
				if( Session.Type != GAME_NORMAL )
					Options.MultiScoreVolume = Options.ScoreVolume;
#endif
				break;

			/*
			**	Control sound volume.
			*/
			case SLIDER_SOUND|KN_BUTTON:
				Options.Set_Sound_Volume(fixed(sound.Get_Value(), 256), true);
				break;

			case BUTTON_LISTBOX|KN_BUTTON:
				break;

			/*
			**	Stop all themes from playing.
			*/
			case BUTTON_STOP|KN_BUTTON:
				Theme.Stop();
				Theme.Queue_Song(THEME_QUIET);
//				Theme.Queue_Song(THEME_NONE);
				break;

			/*
			**	Start the currently selected theme to play.
			*/
			case KN_SPACE:
			case BUTTON_PLAY|KN_BUTTON:
				Theme.Queue_Song( (ThemeType)*((unsigned char *)listbox.Current_Item()) );
				break;

			/*
			**	Toggle the shuffle button.
			*/
			case BUTTON_SHUFFLE|KN_BUTTON:
				shufflebtn.Set_Text(shufflebtn.IsOn ? TXT_ON : TXT_OFF);
				Options.Set_Shuffle(shufflebtn.IsOn);
				break;

			/*
			**	Toggle the repeat button.
			*/
			case BUTTON_REPEAT|KN_BUTTON:
				repeatbtn.Set_Text(repeatbtn.IsOn ? TXT_ON : TXT_OFF);
				Options.Set_Repeat(repeatbtn.IsOn);
				break;
		}
	}

	/*
	**	If the score volume was turned all the way down, then actually
	**	stop the scores from being played.
	*/
	if (Options.ScoreVolume == 0) {
		Theme.Stop();
	}

	/*
	**	Free the items from the list box.
	*/
	while (listbox.Count()) {
		char const * ptr = listbox.Get_Item(0);
		listbox.Remove_Item(ptr);
		delete [] (void*)ptr;
	}
}


/***********************************************************************************************
 * MusicListClass::Draw_Entry -- Draw the score line in a list box.                            *
 *                                                                                             *
 *    This routine will display the score line in a list box. It overrides the list box        *
 *    handler for line drawing.                                                                *
 *                                                                                             *
 * INPUT:   index    -- The index within the list box that is being drawn.                     *
 *                                                                                             *
 *          x,y      -- The pixel coordinates of the upper left position of the line.          *
 *                                                                                             *
 *          width    -- The width of the line that drawing is allowed to use.                  *
 *                                                                                             *
 *          selected-- Is the current line selected?                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void MusicListClass::Draw_Entry(int index, int x, int y, int width, int selected)
{
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	if (TextFlags & TPF_6PT_GRAD) {
		TextPrintType flags = TextFlags;

		if (selected) {
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect (x, y, x + width - 1, y + LineHeight - 1,
				GadgetClass::Get_Color_Scheme()->Shadow);
		} else {
			if (!(flags & TPF_USE_GRAD_PAL)) {
				flags = flags | TPF_MEDIUM_COLOR;
			}
		}

		Conquer_Clip_Text_Print((char *)List[index] + 1, x, y, scheme, TBLACK, flags, width, Tabs);

	} else {
		Conquer_Clip_Text_Print((char *)List[index] + 1, x, y,
			(selected ? &ColorRemaps[PCOLOR_DIALOG_BLUE] : &ColorRemaps[PCOLOR_GREY]), TBLACK, TextFlags, width, Tabs);
	}
}
