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

/* $Header:   F:\projects\c&c\vcs\code\sounddlg.cpv   2.17   16 Oct 1995 16:51:32   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : Jan 18, 1995   [MML]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   SoundControlsClass::Process -- Handles all the options graphic interface.                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"sounddlg.h"

class MusicListClass : public ListClass
{
	public:
		MusicListClass(int id, int x, int y, int w, int h) :
			ListClass(id, x, y, w, h, TPF_6PT_GRAD|TPF_NOSHADOW, Hires_Retrieve("BTN-UP.SHP"), Hires_Retrieve("BTN-DN.SHP"))
		{};
		virtual ~MusicListClass(void) {};

	protected:
		virtual void Draw_Entry(int index, int x, int y, int width, int selected);
};
int SoundControlsClass::Init(void)
{
	int factor 		= (SeenBuff.Get_Width() == 320) ? 1 : 2;
	Option_Width	=292 * factor;
	Option_Height	=146 * factor;

	Option_X			=((SeenBuff.Get_Width() - Option_Width) / 2);
	Option_Y			=(SeenBuff.Get_Height() - Option_Height) / 2;

	Listbox_X		=1 * factor;
	Listbox_Y		=54 * factor;
	Listbox_W		=290 * factor;
	Listbox_H		=73 * factor;

	Button_Width	=85 * factor;
	Button_X			=Option_Width-(Button_Width + (7 * factor));
	Button_Y			=130 * factor;

	Stop_X			=5 * factor;
	Stop_Y			=129 * factor;

	Play_X			=23 * factor;
	Play_Y			=129 * factor;

	OnOff_Width		=25 * factor;
	#ifdef GERMAN
		Shuffle_X	=79 * factor;
	#else
		Shuffle_X	=91 * factor;
	#endif

	Shuffle_Y		=130 * factor;

	Repeat_X			=166 * factor;
	Repeat_Y			=130 * factor;

	MSlider_X		=147 * factor;
	MSlider_Y		=28 * factor;
	MSlider_W		=108 * factor;
	MSlider_Height	=5 * factor;

	FXSlider_X		=147 * factor;
	FXSlider_Y		=40 * factor;
	FXSlider_W		=108 * factor;
	FXSlider_Height=5 * factor;
	return(factor);
}

/***********************************************************************************************
 * OptionsClass::Process -- Handles all the options graphic interface.                         *
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
//	ThemeType theme;

	int factor 		= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	Init();
	/*
	**	List box that holds the score text strings.
	*/
	MusicListClass listbox(0, Option_X+Listbox_X, Option_Y+Listbox_Y, Listbox_W, Listbox_H);

	/*
	**	Return to options menu button.
	*/
	TextButtonClass returnto(BUTTON_OPTIONS, TXT_OPTIONS_MENU, TPF_6PT_GRAD|TPF_NOSHADOW,
#ifdef FRENCH
			Option_X+Button_X-8*2, Option_Y+Button_Y, Button_Width+11*2);
#else
			Option_X+Button_X, Option_Y+Button_Y, Button_Width);
#endif

	/*
	**	Stop playing button.
	*/
	char filename[30];
	if (factor == 1)
		strcpy(filename,"BTN-ST.SHP");
	else
		strcpy(filename,"BTN-STH.SHP");
	ShapeButtonClass stopbtn(BUTTON_STOP, MixFileClass::Retrieve(filename),
			Option_X+Stop_X, Option_Y+Stop_Y);

	/*
	**	Start playing button.
	*/
	if (factor == 1)
		strcpy(filename,"BTN-PL.SHP");
	else
		strcpy(filename,"BTN-PLH.SHP");

	ShapeButtonClass playbtn(BUTTON_PLAY, MixFileClass::Retrieve(filename),
			Option_X+Play_X, Option_Y+Play_Y);

	/*
	**	Shuffle control.
	*/
	TextButtonClass shufflebtn(BUTTON_SHUFFLE, TXT_OFF, TPF_6PT_GRAD|TPF_NOSHADOW,
			Option_X+Shuffle_X, Option_Y+Shuffle_Y, OnOff_Width);

	/*
	**	Repeat control.
	*/
	TextButtonClass repeatbtn(BUTTON_REPEAT, TXT_OFF, TPF_6PT_GRAD|TPF_NOSHADOW, Option_X+Repeat_X, Option_Y+Repeat_Y, OnOff_Width);

	/*
	**	Music volume slider.
	*/
	SliderClass music(SLIDER_MUSIC, Option_X+MSlider_X, Option_Y+MSlider_Y, MSlider_W, MSlider_Height);

	/*
	**	Sound volume slider.
	*/
	SliderClass sound(SLIDER_SOUND, Option_X+FXSlider_X, Option_Y+FXSlider_Y, FXSlider_W, FXSlider_Height);

	/*
	**	Causes left mouse clicks inside the dialog area, but not on any
	**	particular button, to be ignored.
	*/
	GadgetClass area(Option_X, Option_Y, Option_Width, Option_Height, GadgetClass::LEFTPRESS);

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
	music.Set_Value(Options.ScoreVolume);
	sound.Set_Maximum(255);
	sound.Set_Thumb_Size(16);
	sound.Set_Value(Options.Volume);

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
//	char buffer[100];
	for (ThemeType index = THEME_FIRST; index < Theme.Max_Themes(); index++) {
		if (Theme.Is_Allowed(index)) {
			int length = Theme.Track_Length(index);
			char const * fullname = Theme.Full_Name(index);

			void * ptr = new char [sizeof(100)];
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
		55*factor, 72*factor, 90*factor
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
		if (GameToPlay == GAME_NORMAL) {
			Call_Back();
		} else {
			if (Main_Loop()) {
				process = false;
			}
		}

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=TRUE;
		}

		/*
		**	Refresh display if needed.
		*/
		if (display) {

			Hide_Mouse();

			/*
			**	Draw the background.
			*/
			Dialog_Box(Option_X, Option_Y, Option_Width, Option_Height);

			Draw_Caption(TXT_SOUND_CONTROLS, Option_X, Option_Y, Option_Width);

			/*
			** Draw the Music, Speech & Sound titles.
			*/
			Fancy_Text_Print(TXT_MUSIC_VOLUME, Option_X+MSlider_X-5, Option_Y+MSlider_Y-2, CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW|TPF_RIGHT);
			Fancy_Text_Print(TXT_SOUND_VOLUME, Option_X+FXSlider_X-5, Option_Y+FXSlider_Y-2, CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW|TPF_RIGHT);

			Fancy_Text_Print(TXT_SHUFFLE, Option_X+Shuffle_X-5, Option_Y+Shuffle_Y+1, CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW|TPF_RIGHT);
			Fancy_Text_Print(TXT_REPEAT, Option_X+Repeat_X-5, Option_Y+Repeat_Y+1, CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW|TPF_RIGHT);

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
				Options.Set_Score_Volume(music.Get_Value());
				break;

			/*
			**	Control sound volume.
			*/
			case SLIDER_SOUND|KN_BUTTON:
				Options.Set_Sound_Volume(sound.Get_Value(), true);
				break;

			case BUTTON_LISTBOX|KN_BUTTON:
//				Mono_Printf ("%d %s Listbox was pressed.\r",__LINE__, __FILE__);
				break;

			/*
			**	Stop all themes from playing.
			*/
			case BUTTON_STOP|KN_BUTTON:
				Theme.Queue_Song(THEME_NONE);
				break;

			/*
			**	Start the currently selected theme to play.
			*/
			case KN_SPACE:
			case BUTTON_PLAY|KN_BUTTON:
				if (listbox.Count()) {
					Theme.Queue_Song( (ThemeType)*((unsigned char *)listbox.Current_Item()) );
				}
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
	if (!Options.ScoreVolume) {
		Theme.Stop();
	}

	/*
	** Save them settings - you know it makes sense
	*/
	Options.Save_Settings();			// save new value

	/*
	**	Free the items from the list box.
	*/
	while (listbox.Count()) {
		char const * ptr = listbox.Get_Item(0);
		listbox.Remove_Item(ptr);
		delete [] (void*)ptr;
	}
}


void MusicListClass::Draw_Entry(int index, int x, int y, int width, int selected)
{
	if (TextFlags & TPF_6PT_GRAD) {
		TextPrintType flags = TextFlags;

		if (selected) {
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect (x, y, x + width - 1, y + LineHeight - 1, CC_GREEN_SHADOW);
		} else {
			if (!(flags & TPF_USE_GRAD_PAL)) {
				flags = flags | TPF_MEDIUM_COLOR;
			}
		}

		Conquer_Clip_Text_Print((char *)Add_Long_To_Pointer(List[index], 1), x, y, CC_GREEN, TBLACK, flags, width, Tabs);

	} else {
		Conquer_Clip_Text_Print((char *)Add_Long_To_Pointer(List[index], 1), x, y, (selected ? BLUE : WHITE), TBLACK, TextFlags, width, Tabs);
	}
}
