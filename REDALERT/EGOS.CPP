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

/* $Header: /counterstrike/EGOS.CPP 2     3/10/97 3:19p Steve_tall $ */
/*************************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S                  **
 *************************************************************************************
 *                                                                                   *
 *                 Project Name : Command & Conquer - Red Alert                      *
 *                                                                                   *
 *                    File Name : EGOS.CPP                                           *
 *                                                                                   *
 *                   Programmer : Steve Tall                                         *
 *                                                                                   *
 *                   Start Date : September 4th, 1996                                *
 *                                                                                   *
 *                  Last Update : September 4th, 1996 [ST]                           *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Overview:                                                                         *
 *                                                                                   *
 *   Scrolling movie style credits.                                                  *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Functions:                                                                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if(0)
//PG_TO_FIX
#include 	"function.h"

/*
** List of Ego Class instances
**  There will be one instance for each line of text.
*/
DynamicVectorClass<EgoClass *> EgoList;

/*
** Number of slideshow pictures
*/

#define NUM_SLIDES 17

/*
** Length of time frame is displayed for
*/
#define FRAME_DELAY	150

/*
** Number of frames that palete fade occurs over
*/
#define FADE_DELAY		37

/*
** Names of slideshow pictures to play behind the text
*/
char SlideNames[NUM_SLIDES][13]={

	"aftr_hi.pcx",
	"aly1.pcx",
	"apc_hi.pcx",
	"aphi0049.pcx",
	"bnhi0020.pcx",
	"dchi0040.pcx",
	"frhi0166.pcx",
	"lab.pcx",
	"landsbrg.pcx",
	"mahi0107.pcx",
	"mig_hi.pcx",
	"mtfacthi.pcx",
	"needle.pcx",
	"sov2.pcx",
	"spy.pcx",
	"stalin.pcx",
	"tent.pcx"
};

/*
** Names of low res slideshow pictures to play behind the text
*/
char LoresSlideNames[NUM_SLIDES][13]={
	"malo0107.cps",
	"mig_lo.cps",
	"mtfactlo.cps",
	"needl-lo.cps",
	"sov2-lo.cps",
	"spy-lo.cps",
	"staln-lo.cps",
	"tent-lo.cps",
	"aftr_lo.cps",
	"aly1-lo.cps",
	"apc_lo.cps",
	"aplo0049.cps",
	"bnlo0020.cps",
	"dclo0040.cps",
	"frlo0166.cps",
	"lab-lo.cps",
	"lands-lo.cps"
};

/*
** Array of all the palettes required for the slides
*/
char SlidePals[NUM_SLIDES][256*3];

/*
** Array of graphic buffers containing the slides
*/
GraphicBufferClass *SlideBuffers[NUM_SLIDES];

/*
** Original copy of slide (pref in video mem) that we use to undraw the text
*/
GraphicBufferClass *BackgroundPage;

/*
**  This palette contains both the font palette entries and the slide
**  palette.
*/
PaletteClass ComboPalette;

/*
** Ptr to the combo palette.
*/
unsigned char *ComboPalPtr;

/*
** Lookup table. If an entry is non-zero then it should be faded in/out when the slide changes.
*/
char PaletteLUT[256];

/*
** Height of the strips that are blitted from the slides to the backgound and hid pages.
** We blit in several strips over several frames so as not to impact on the frame rate.
*/
#define CHUNK_HEIGHT RESFACTOR * 50



#ifndef	WIN32
extern void Vsync(void);
#pragma aux Vsync modify [edx ebx eax] = \
	"mov	edx,03DAh"				\
	"mov	ebx,[VertBlank]"		\
	"and	bl,001h"					\
	"shl	bl,3"						\
	"in_vbi:"						\
	"in	al,dx"					\
	"and	al,008h"					\
	"xor	al,bl"					\
	"je	in_vbi"					\
	"out_vbi:"						\
	"in	al,dx"					\
	"and	al,008h"					\
	"xor	al,bl"					\
	"jne	out_vbi"
#endif	//WIN32



/***********************************************************************************************
 * EC::EgoClass -- EgoClass constructor                                                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    x position of text                                                                *
 *           y position of text                                                                *
 *           ptr to text string                                                                *
 *           flags to print text with                                                          *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:53PM ST : Created                                                              *
 *=============================================================================================*/
EgoClass::EgoClass (int x, int y, char *text, TextPrintType flags)
{
	XPos = x;
	YPos = y;
	Flags= flags;
	Text = new char [strlen (text)+1];
	strcpy (Text, text);
}


/***********************************************************************************************
 * EC::~EgoClass -- EgoClass destructor                                                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:54PM ST : Created                                                              *
 *=============================================================================================*/
EgoClass::~EgoClass(void)
{
	delete [] Text;
}


/***********************************************************************************************
 * EC::Scroll -- Apply the given distance to the y position of the text.                       *
 *               A positive distance scrolls up.                                               *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    distance in pixels to scroll up                                                   *
 *                                                                                             *
 * OUTPUT:   true if text scrolled beyond the top of the screen                                *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:55PM ST : Created                                                              *
 *=============================================================================================*/
bool EgoClass::Scroll(int distance)
{
	YPos -= distance;
	if (YPos < -20) {
		return (true);
	}else{
		return (false);
	}
}


/***********************************************************************************************
 * EC::Render -- Draws the text to the logic page                                              *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:57PM ST : Created                                                              *
 *=============================================================================================*/
void EgoClass::Render (void)
{
	if (YPos < LogicPage->Get_Height() && YPos > -16) {
		Fancy_Text_Print(Text, XPos, YPos, GadgetClass::Get_Color_Scheme(), TBLACK, Flags);
	}
}



/***********************************************************************************************
 * EC::Wipe -- Wipes the previously rendered text by blitting a rectangle from the given       *
 *             background screen.                                                              *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to screen containing original background                                      *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:58PM ST : Created                                                              *
 *=============================================================================================*/
void EgoClass::Wipe (GraphicBufferClass *background)
{
	int width = String_Pixel_Width (Text);
	int x = XPos;

	if (Flags & TPF_RIGHT) {
		x -= width;
	}else{
		if (Flags & TPF_CENTER){
			x -= width/2;
		}
	}

	background->Blit(*LogicPage, x-1, YPos, x-1, YPos, width+2, 7 * RESFACTOR +1, false);
}



/***********************************************************************************************
 * Set_Pal -- Low level palette set                                                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to palette                                                                    *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:59PM ST : Created                                                              *
 *=============================================================================================*/
void Set_Pal(char *palette)
{
//#ifndef WIN32
	//Vsync();
	//unsigned char *rgbptr = (unsigned char *) palette;
	//outportb(0x03C8, 0);												//Start from color 0

	//for (int index = 0; index < 256; index++) {
	//	outrgb(rgbptr[index*3], rgbptr[index*3+1], rgbptr[index*3+2]);
	//}
//#else	//WIN32

	Set_Palette((void*)palette);
//#endif
}


/***********************************************************************************************
 * Slide_Show -- Handles the blitting and fading of the background pictures.                   *
 *                                                                                             *
 *  The picture frame number is used to trigger blitting and fading events                     *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    picture number                                                                    *
 *           frame                                                                             *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/10/96 0:16AM ST : Created                                                              *
 *=============================================================================================*/
void Slide_Show (int slide, int frame)
{

	/*
	** Temprary storage to save CCPalette to
	*/
	char save_palette[256*3];


	if (frame >= 1 && frame <=4){
		/*
		** Blit in a quarter of the new frame to the background page.
		*/
		SlideBuffers[slide]->Blit (*BackgroundPage, 	0, (frame-1) * CHUNK_HEIGHT,
																	0, (frame-1) * CHUNK_HEIGHT,
																	SeenBuff.Get_Width(), CHUNK_HEIGHT, false);
		return;
	}

	if (frame >= 5 && frame <=8 ){
		/*
		** Blit in a quarter of the new frame to the hid page.
		*/
		BackgroundPage->Blit (HidPage,	0, (frame-5) * CHUNK_HEIGHT,
												0, (frame-5) * CHUNK_HEIGHT,
												SeenBuff.Get_Width(), CHUNK_HEIGHT, false);
		return;
	}

	if (frame ==9){
		/*
		** Create the combo palette from the font entries and the picture entries.
		*/
		for (int index = 0 ; index < 256 ; index++ ){
			if (PaletteLUT[index]) {
				ComboPalPtr[index*3] = SlidePals[slide][index*3];
				ComboPalPtr[index*3+1] = SlidePals[slide][index*3+1];
				ComboPalPtr[index*3+2] = SlidePals[slide][index*3+2];
			}
		}
		return;
	}


	if (frame >10 && frame < FADE_DELAY+10){
		/*
		** Fade up the picture in the background. The text colors never fade.
		*/
		memcpy (save_palette, CCPalette, sizeof(save_palette));
		//CCPalette.Partial_Adjust (MIN (6*(frame-5), 255), ComboPalette, PaletteLUT);
		CCPalette.Partial_Adjust (MIN ((255/FADE_DELAY)*(frame-10), 255), ComboPalette, PaletteLUT);
		Set_Pal ( (char *) &CCPalette);
		if (frame != 9+FADE_DELAY){
			memcpy (CCPalette, save_palette, sizeof(save_palette));
		}else{
			memcpy (CCPalette, CurrentPalette, sizeof (CCPalette));
		}
		return;
	}


	if (frame >FRAME_DELAY && frame < FRAME_DELAY+FADE_DELAY){
		/*
		** Fade down the picture in the background. The text colors never fade.
		*/
		memcpy (save_palette, CCPalette, sizeof(save_palette));
		CCPalette.Partial_Adjust (MIN ((255/FADE_DELAY)*(frame-FRAME_DELAY), 255), PaletteLUT);
		if (frame != FRAME_DELAY+FADE_DELAY-1){
			Set_Pal ( (char *) &CCPalette);
			memcpy (CCPalette, save_palette, sizeof(save_palette));
		}else{
			/*
			** If this is the last fade down frame then zero the picture palette entries.
			*/
			unsigned char *ccpalptr = (unsigned char*)CCPalette;
			for (int index = 0 ; index < 256 ; index++){
				if (PaletteLUT[index]){
					ccpalptr[index*3] = 0;
					ccpalptr[index*3+1] = 0;
					ccpalptr[index*3+2] = 0;
				}
			}
			Set_Pal ( (char *) &CCPalette);
		}

	}

}




/***********************************************************************************************
 * Show_Who_Was_Responsible -- Main function to print the credits.                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/10/96 0:20AM ST : Created                                                              *
 *=============================================================================================*/
void Show_Who_Was_Responsible (void)
{

	int	i;
	int	key;

	/*
	** Deault speed of credits scolling. This is the frame delay between pixel scrolls.
	*/
	static int speed = 3;

	/*
	** In DOS we need to scroll slower so we have a bool that lets us do it every other time
	*/
#ifndef WIN32
	bool	scroll_now = false;
#endif	//WIN32

	/*
	** Read in the credits file to be displayed
	**
	** Lines of text in CREDITS.TXT are treated as follows....
	**
	**  If the text starts and ends to the left of column 40 then text will be right justified.
	**  If the text starts before column 40 and ends after it then it will be centered.
	**  If the text starts after column 40 it will be right justified.
	*/
	CCFileClass creditsfile ("credits.txt");
	if ( !creditsfile.Is_Available()) return;
	char *credits = new char [creditsfile.Size()+1];
	creditsfile.Read (credits, creditsfile.Size());

	/*
	** Initialise the text printing system.
	*/
	GadgetClass::Set_Color_Scheme(&ColorRemaps[PCOLOR_GREEN]);
	Fancy_Text_Print(TXT_NONE, 0, 0, GadgetClass::Get_Color_Scheme(),
		TBLACK, TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);


	/*
	** Miscellaneous stuff for parsing the credits text file.
	*/
	int 				length = creditsfile.Size();
	int 				line   = 0;
	int 				column = 0;
	char				*cptr = credits;
	char				ch, lastchar, oldchar;
	char				*strstart, *strparse;
	bool				gotendstr;
	int				startcolumn, endcolumn, x;
	int				y=SeenBuff.Get_Height()+2;
	EgoClass 		*ego;
	TextPrintType 	flags;


	/*
	** Search through the text file and extract the strings, using each string to create
	** a new EgoClass
	*/
	do {
		/*
		** Search for text
		*/
		ch = *cptr++;
		length --;

		/*
		** Look for a non whitespace character.
		*/
		switch ( ch ){

			case 13:
				/*
				** Char was carriage return. Go on to the next line starting at column 0.
				*/
				line++;
				column = 0;
				break;


			case 10:
				/*
				** Ignore line feed. CR does both.
				*/
				break;

				/*
				** Space character. Just advance the cursor and move on.
				*/
			case 32:
				column++;
				break;

				/*
				** Tab char. Advance to the next tab column. Tabs are every 8 columns.
				*/
			case 9:
				column += 8;
				column &= 0xfffffff8;
				break;

			default:
				/*
				** Found new string. Work out where it ends so we know how to treat it.
				*/
				lastchar = ch;
				strstart = cptr-1;
				strparse = cptr-1;
				endcolumn = startcolumn = column;
				gotendstr = false;

				do	{
					ch = *strparse++;
					switch ( ch ){
						case 9:
						case 10:
						case 13:
							gotendstr = true;
							break;

						case 32:
							if (lastchar == 32) gotendstr = true;
							endcolumn++;
							break;

						default:
							endcolumn++;
					}
					if (strparse >= cptr+length) gotendstr = true;

					lastchar = ch;
				}while (!gotendstr);


				if (strparse >= cptr+length) break;

				/*
				** Strip off any trailing space.
				*/
				if (*(strparse-2) == 32){
					strparse--;
					endcolumn -= 2;
				}


				/*
				** If string straddles the center column then center it.
				**
				** If string is on the left hand side then right justify it.
				**
				** If string is on the right hand side then left justify it.
				*/
				flags = TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_DROPSHADOW;	//TPF_NOSHADOW;

				if (startcolumn <40 && endcolumn >40){
					flags = flags | TPF_CENTER;
					x = SeenBuff.Get_Width() / 2;
				}else{
					if (startcolumn <40){
						flags = flags | TPF_RIGHT;
						x = endcolumn *SeenBuff.Get_Width() /80;
					}else{
						x = startcolumn * SeenBuff.Get_Width() / 80;
					}
				}

				/*
				** Temporarily terminate the string.
				*/
				oldchar = *(strparse-1);
				*(strparse-1) = 0;

				/*
				** Create the new class and add it to our list.
				*/
				ego = new EgoClass (x, y+ line *8 *RESFACTOR, strstart, flags);

				EgoList.Add (ego);

				/*
				** Restore the character that was lost when we added the terminator.
				*/
				*(strparse-1) = oldchar;

				/*
				** Fix up our outer loop parsing variables.
				*/
				cptr = strparse;
				column += strparse - strstart;
				length -= strparse - strstart-1;

				if (ch == 13) {
					line++;
					column = 0;
				}else{
					if (ch == 9){
						column += 7;
						column &= 0xfffffff8;
					}
				}
				break;
		}

	} while ( length>0 );


	/*
	** Work out which palette entries the font needs so we dont fade those colors.
	*/
	memset (PaletteLUT, 1, sizeof (PaletteLUT));
	int pcolor = PCOLOR_GREEN;

	for (int index = 0; index < 6; index++) {
		PaletteLUT[ColorRemaps[pcolor].FontRemap[10+index]] =0;
	}
	//PaletteLUT[ColorRemaps[pcolor].BrightColor] = 0;
	PaletteLUT[ColorRemaps[pcolor].Color] = 0;
	PaletteLUT[ColorRemaps[pcolor].Shadow] = 0;
	PaletteLUT[ColorRemaps[pcolor].Background] = 0;
	PaletteLUT[ColorRemaps[pcolor].Corners] = 0;
	PaletteLUT[ColorRemaps[pcolor].Highlight] = 0;
	PaletteLUT[ColorRemaps[pcolor].Bright] = 0;
	PaletteLUT[ColorRemaps[pcolor].Underline] = 0;
	PaletteLUT[ColorRemaps[pcolor].Bar] = 0;
	PaletteLUT[ColorRemaps[pcolor].Box] = 0;


	/*
	** Stop the music.
	*/
	Theme.Stop();

	/*
	** Fade to black.
	*/
	BlackPalette.Set(TIMER_SECOND*2, Call_Back);

	/*
	** Load the reference palette for the font.
	*/
	//Load_Title_Page(true);
//#ifdef WIN32
//	Load_Picture("EGOPAL.CPS", SysMemPage, SysMemPage, CCPalette, BM_DEFAULT);
//#else	//WIN32
//	Load_Picture("EGOPAL.CPS", HidPage, HidPage, CCPalette, BM_DEFAULT);
//#endif	//WIN32


	CCFileClass("EGOPAL.PAL").Read(&CCPalette, sizeof(CCPalette));

	/*
	** Copy the font palette entries into the combo palette.
	*/
	PaletteClass credit_palette;
	ComboPalPtr = (unsigned char *) &ComboPalette;
	unsigned char *creditpal_ptr = (unsigned char *) &credit_palette;
	memcpy (ComboPalette, CCPalette, sizeof (ComboPalette));

	for (index = 0 ; index < 256 ; index++ ){
		if (PaletteLUT[index]) {
			ComboPalPtr[index*3] = 0;
			ComboPalPtr[index*3+1] = 0;
			ComboPalPtr[index*3+2] = 0;
		}
	}

	/*
	** Clear the Seen Page since we will not be blitting to all of it.
	*/
	SeenBuff.Clear();
	HidPage.Clear();

	/*
	** Set the font palette.
	*/
	memcpy ( CCPalette, ComboPalette, sizeof (ComboPalette) );
	CCPalette.Set();

	/*
	** Loop through and load up all the slideshow pictures
	*/
	for (index = 0 ; index < NUM_SLIDES ; index++){
#ifdef WIN32
		SlideBuffers[index] = new GraphicBufferClass;
		SlideBuffers[index]->Init (SeenBuff.Get_Width(), SeenBuff.Get_Height(), NULL , 0 , (GBC_Enum)0);
		Load_Title_Screen(&SlideNames[index][0], SlideBuffers[index], (unsigned char*) &SlidePals[index][0]);
#else	//WIN32
		SlideBuffers[index] = new GraphicBufferClass (SeenBuff.Get_Width(), SeenBuff.Get_Height(), (void*)NULL);
		Load_Picture(&LoresSlideNames[index][0], *SlideBuffers[index], *SlideBuffers[index], (unsigned char *)&SlidePals[index][0], BM_DEFAULT);
#endif	//WIN32
	}

	/*
	** Create a new graphic buffer to restore the background from. Initialise it to black so
	** we can start scrolling before the first slideshow picture is blitted.
	*/
#ifdef WIN32
	BackgroundPage = new GraphicBufferClass;
	BackgroundPage->Init (SeenBuff.Get_Width(), SeenBuff.Get_Height(), NULL , 0 , (GBC_Enum)(GBC_VIDEOMEM));
#else	//WIN32
	BackgroundPage = new GraphicBufferClass (SeenBuff.Get_Width(), SeenBuff.Get_Height(), (void*)NULL );
#endif	//WIN32

	SeenBuff.Blit(*BackgroundPage);

	/*
	** Go away nasty keyboard.
	*/
	Keyboard->Clear();

	Set_Logic_Page(HidPage);

	/*
	** Start any old song.
	*/
	fixed oldvolume = Options.ScoreVolume;
	if (oldvolume == 0) {
		Options.Set_Score_Volume(fixed(4, 10), false);
	}
	Theme.Queue_Song(THEME_CREDITS);

	/*
	** Init misc timing variables.
	*/
	int time  = TickCount;
	int frame = 0;
	int picture_frame = 0;
	int slide_number = 0;

	Hide_Mouse();

	/*
	** Save the priority of this process so we can change it back later
	*/
	//DWORD process_priority = GetPriorityClass(GetCurrentProcess());

	/*
	** Main scrolling loop.
	** Keeps going until all the EgoClass objects are deleted or esc is pressed.
	*/
	while ( EgoList.Count() ){

		frame++;

		/*
		** Once we have been running for a few frames, and Windows has time to do its virtual
		** memory stuff, increase our priority level.
		*/
		//if (frame == 30){
		//	SetPriorityClass (GetCurrentProcess() , HIGH_PRIORITY_CLASS);
		//}

		/*
		** Update the slideshow frame and switch to the next picture if its time.
		*/
		picture_frame++;

		if (picture_frame > FRAME_DELAY+50){
			if (slide_number <NUM_SLIDES-1){
				slide_number++;
				picture_frame = 0;
			}else{
				slide_number = 0;
				picture_frame = 0;
			}
		}

		/*
		** Do the slideshow background.
		*/
		Slide_Show (slide_number, picture_frame);


		/*
		** Scroll the text. If any text goes off the top then delete that object.
		*/
#ifndef WIN32
		scroll_now = !scroll_now;
		if (scroll_now){
#endif	//WIN32
			for (i=EgoList.Count()-1 ; i>=0 ; i--){
				EgoList[i]->Wipe(BackgroundPage);
				if ( EgoList[i]->Scroll(1) ){
					EgoList.Delete(i);
					break;
				}
			}
#ifndef WIN32
		}
#endif	//WIN32
		/*
		** Render all the text strings in their new positions.
		*/
		if (LogicPage->Lock()){
			for (i=EgoList.Count()-1 ; i>=0 ; i--){
				EgoList[i]->Render();
			}
			LogicPage->Unlock();
		}

		if (frame > 1000 && !Theme.Still_Playing()){
			Theme.Queue_Song(THEME_CREDITS);	//NONE);
		}

		/*
		** Stop calling Theme.AI after a while so a different song doesnt start playing
		*/
		Call_Back();
//		if (frame <1000 ){
//			Theme.AI();
//		}else{
//			Sound_Callback();
//		}

		/*
		** Kill any spare time before blitting the hid page forward.
		*/
		while (TickCount - time < frame *speed && !Keyboard->Check()) {}

		/*
		** Blit all but the top and bottom of the hid page. This is beacuse the text print doesn't
		** clip vertically and looks ugly when it suddenly appears and disappears.
		*/
#ifndef WIN32
		Wait_Vert_Blank(VertBlank);
		//Vsync();
#endif	//WIN32
		HidPage.Blit(SeenBuff, 0, 8*RESFACTOR, 0, 8*RESFACTOR, SeenBuff.Get_Width(), SeenBuff.Get_Height() - 16*RESFACTOR, false);

		/*
		** Try and prevent Win95 from swapping out pictures we havnt used yet.
		*/
#ifdef WIN32
		if (frame && 3 == 3){
			for (i=slide_number+1 ; i<NUM_SLIDES ; i++){
				if ( !SlideBuffers[i]->Get_IsDirectDraw() ){
					Force_VM_Page_In ((void*)SlideBuffers[i]->Get_Offset(), SeenBuff.Get_Width() * SeenBuff.Get_Height() );
				}
			}
		}
#endif	//WIN32

		/*
		** If user hits escape then break.
		*/
		key = KN_NONE;
		if (Keyboard->Check()){
			key = Keyboard->Get();
			if (key == KN_ESC){
				break;
			}
#if (0)
			if (key == KN_Z){
				speed--;
				if (speed <1 ) speed=1;
				time = TickCount;
				frame = 0;
			}
			if (key == KN_X){
				speed++;
				time = TickCount;
				frame = 0;
			}
#endif	//(0)

		}

	}

	if (key == KN_ESC){
		Theme.Fade_Out();
		BlackPalette.Set(TIMER_SECOND*2, Call_Back);
	}else{
		/*
		** Wait for the picture to fade down
		*/
		while (picture_frame <= FADE_DELAY+FRAME_DELAY){
			if (picture_frame < FRAME_DELAY && picture_frame > 10+FADE_DELAY){
				picture_frame = FRAME_DELAY;
			}
			frame++;
			picture_frame++;

			Slide_Show (slide_number, picture_frame);

			Call_Back();
//			Sound_Callback();		//Theme.AI();

			/*
			** Kill any spare time
			*/
			while (TickCount - time < frame *speed && !Keyboard->Check()) {}

		}
	}

	/*
	** Tidy up.
	*/
	//SetPriorityClass (GetCurrentProcess() , process_priority);
	SeenBuff.Clear();

	Show_Mouse();

	GadgetClass::Set_Color_Scheme(&ColorRemaps[PCOLOR_DIALOG_BLUE]);

	Theme.Stop();
	Options.Set_Score_Volume(oldvolume, false);

	for (index = 0 ; index < NUM_SLIDES ; index++){
		delete SlideBuffers[index];
	}

	delete BackgroundPage;

	delete [] credits;

	EgoList.Clear();
}

#endif












