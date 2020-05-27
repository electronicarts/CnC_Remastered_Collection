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

/* $Header:   F:\projects\c&c\vcs\code\menus.cpv   2.17   16 Oct 1995 16:50:48   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MENUS.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Phil W. Gorrow                                               *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : May 17, 1995 [BRR]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Main_Menu -- Menu processing                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "ccdde.h"

/*****************************
**	Function prototypes
******************************/

#ifdef SCENARIO_EDITOR

PRIVATE int Coordinates_In_Region(int x,int y,int inx1,int iny1,int inx2,int iny2);
PRIVATE int Select_To_Entry(int select, unsigned long bitfield, int index);
PRIVATE void Flash_Line(char const *text,int xpix,int ypix,unsigned nfgc,unsigned hfgc,unsigned bgc);

int UnknownKey;

PRIVATE int MenuUpdate=1;
PRIVATE int MenuSkip;


/*=========================================================================*/
/*	SELECT_TO_ENTRY:																			*/
/*																									*/
/*		This routine converts a selection to the correct string entry. It	   */
/*	does this by search through a long bitfield starting at position index	*/
/*	until it finds the correct conversion to entries.								*/
/*																									*/
/*	INPUTS:	int selection from menu, long the bit field to search, int 	   */
/*				the starting index within the bit field.								*/
/*	RETURNS:	int the index into the table of entries								*/
/*=========================================================================*/
PRIVATE int Select_To_Entry(int select, unsigned long bitfield, int index)
{
	int placement;

	if (bitfield==0xFFFFFFFFL) 							/* if all bits are set	*/
		return(select);										/*		then it as is		*/

	placement=0;												/* current pos zero		*/
	while (select) {											/* while still ones		*/
		if (bitfield & (1L<<(placement+index)))			/* if this flagged then	*/
			select--;											/* decrement counter		*/
		placement++;											/* and we moved a place	*/
	}
	while (!(bitfield & (1L<<(placement+index)))) {
		placement++;
	}

	return(placement);										/* return the position	*/
}


/*=========================================================================*/
/*	FLASH_LINE:																					*/
/*																									*/
/*		This routine will flash the line at the desired location for the		*/
/*	menu routine. It is way cool awesome!												*/
/*																									*/
/*	INPUTS:	char *text, int x position on line, int y position, char		   */
/*				normal foreground color, char hilight foreground color, char 	*/
/*				background color																*/
/*	RETURNS:	none																				*/
/*=========================================================================*/
PRIVATE void Flash_Line(char const *text,int xpix,int ypix,unsigned nfgc,unsigned hfgc,unsigned bgc)
{
	int loop;

	for (loop=0;loop<3;loop++) {
		Hide_Mouse();
		Fancy_Text_Print(text,xpix,ypix,hfgc,bgc, TPF_8POINT|TPF_DROPSHADOW);
		Delay(2);
		Fancy_Text_Print(text,xpix,ypix,nfgc,bgc, TPF_8POINT|TPF_DROPSHADOW);
		Show_Mouse();
		Delay(2);
	}
}

/*=========================================================================*/
/*	COORDINATES_IN_REGION:																	*/
/*																									*/
/*		Test to see if a given pair of coordinates are within the given 		*/
/*	rectangular region.																		*/
/*																									*/
/*	INPUTS:	int x to be tested, int y to be tested, int left x pos,			*/
/*				int top y pos, int right x pos, int bottom y pos					*/
/*	RETURNS:	none																				*/
/*=========================================================================*/
PRIVATE int Coordinates_In_Region(int x,int y,int inx1,int iny1,int inx2,int iny2)
{
	return((x>=inx1)&&(x<=inx2)&&(y>=iny1)&&(y<=iny2));
}

#ifdef NEVER
/*=========================================================================*/
/*	FIND_MENU_ITEMS:																			*/
/*																									*/
/*		This routine finds the real total items in a menu when certain items	*/
/*	may be disabled by bit fields and the like. This is done by looping		*/
/*	through the fields, starting at the position passed in index and 			*/
/*	counting the number of bits that are set.											*/
/*																									*/
/*	INPUTS:	int the maximum number of items possible on the menu, long 		*/
/*				the bit field of enabled and disabled items, char the index		*/
/*				point to start at within the list.										*/
/*	RETURNS:	int the total number of items in the menu							*/
/*=========================================================================*/
 int Find_Menu_Items(int maxitems, unsigned long field, char index)
 {
	int loop,ctr;

	if (field==0xFFFFFFFFL) 										/* if all bits are set	*/
		return(maxitems);										/* then maxitems set		*/

	for (loop=ctr=0;loop<maxitems;loop++) {			/* loop through items	*/
		if (field & (1L<<(loop+index))) {				/* if the bit is set		*/
			ctr++;												/*		count the item		*/
		}
	}
	return(ctr);
}
#endif


/*=========================================================================*/
/*	SETUP_EOB_MONITOR_MENU:																	*/
/*																									*/
/*		This routine sets up the eye of the beholder monitor menu.				*/
/*																									*/
/*	INPUTS:	int the menu we are using, char *[] the array of text which		*/
/*				makes up the menu commands, long the info field, int the			*/
/*				index into the field, int the number of lines to skip.			*/
/*	RETURNS:	none																				*/
/*=========================================================================*/
void Setup_Menu(int menu,char const *text[], unsigned long field, int index, int skip)
{
	int *menuptr,lp;
	int menuy,menux,idx,item,num,drawy;

	menuptr=&MenuList[menu][0];							/* get pointer to menu	*/
	menuy=WinY+menuptr[MENUY];								/* get the absolute 		*/
	menux=(WinX+menuptr[MENUX])<<3;						/*		coords of menu		*/
	item=Select_To_Entry(menuptr[MSELECTED],field,index);
	num=menuptr[ITEMSHIGH];

	Fancy_Text_Print(0, 0, 0, TBLACK, TBLACK, TPF_8POINT|TPF_DROPSHADOW);
 	Hide_Mouse();
	for (lp=0;lp<num;lp++) {
		idx=Select_To_Entry(lp,field,index);
		drawy=menuy+(lp*FontHeight)+(lp*skip);
		Fancy_Text_Print(text[idx], menux, drawy, menuptr[((idx==item) && (MenuUpdate )) ? HILITE : NORMCOL], TBLACK, TPF_8POINT|TPF_DROPSHADOW);
//		if ((idx==item) && (MenuUpdate ))
//			Text_Print(text[idx],menux,drawy,menuptr[HILITE],TBLACK);
	}
	MenuSkip=skip;
	Show_Mouse();
	Keyboard::Clear();
}


/*=========================================================================*/
/*	CHECK_MENU:																					*/
/*																									*/
/*																									*/
/*																									*/
/*	INPUTS:																						*/
/*	RETURNS:																						*/
/*=========================================================================*/
int Check_Menu(int menu,char const *text[],char *,long field,int index)
{
	int maxitem,select,key,menuy,menux;
	int mx1,mx2,my1,my2,tempy;
	int drawy,menuskip,halfskip;
	int normcol,litcol,item,newitem,idx;
	int *menuptr;

	//selection++;												/* get rid of warning	*/

	menuptr = &MenuList[menu][0];							/* get pointer to menu	*/
	maxitem = menuptr[ITEMSHIGH]-1;							/* find max items			*/
	newitem = item = menuptr[MSELECTED]%(maxitem+1);	/* find selected 			*/
	select = -1;													/* no selection made		*/
	menuskip = FontHeight+MenuSkip;							/* calc new font height	*/
	halfskip = MenuSkip>>1;									/* adjustment for menus	*/

	menuy = WinY+menuptr[MENUY];								/* get the absolute 		*/
	menux = (WinX+menuptr[MENUX])<<3;						/*		coords of menu		*/
	normcol = menuptr[NORMCOL];
	litcol = menuptr[HILITE];

	/*
	**	Fetch a pending keystroke from the buffer if there is a keystroke
	**	present. If no keystroke is pending then simple mouse tracking will
	**	be done.
	*/
	key = 0;
	UnknownKey = 0;
	if (Keyboard::Check()) {
		key = (Keyboard::Get()&0x18FF);			/* mask off all but release bit	*/
	}

	/*
	**	if we are using the mouse and it is installed, then find the mouse
	**	coordinates of the menu and if we are not somewhere on the menu get
	**	the heck outta here. If we are somewhere on the menu, then figure
	**	out the new selected item, and continue forward.
	*/
	mx1=(WinX<<3)+(menuptr[MENUX]*FontWidth);		/* get menu coords		*/
	my1=(WinY)+(menuptr[MENUY])-halfskip;			/*		from the menu		*/
	mx2=mx1+(menuptr[ITEMWIDTH]*FontWidth)-1;		/*		structure as		*/
	my2=my1+(menuptr[ITEMSHIGH]*menuskip)-1;		/*		necessary			*/

		tempy=Get_Mouse_Y();
		if (Coordinates_In_Region(Get_Mouse_X(),tempy,mx1,my1,mx2,my2)&& MenuUpdate) {
			newitem=(tempy-my1)/menuskip;
		}

	switch (key) {

		case KN_UP:												/* if the key moves up	*/
			newitem--;											/* 	new item up one	*/
			if (newitem<0) 									/* if invalid new item	*/
				newitem=maxitem;								/* put at list bottom	*/
			break;
		case KN_DOWN:											/* if key moves down		*/
			newitem++;											/*		new item down one	*/
			if (newitem>maxitem) 							/* if new item past 		*/
				newitem=0;										/*		list end, clear	*/
			break;
		case KN_HOME:											/* if top of list key 	*/
		case KN_PGUP:											/*		is selected then	*/
			newitem=0;											/*		new item = top		*/
			break;
		case KN_END:											/* if bottom of list is	*/
		case KN_PGDN:											/*		selected then		*/
			newitem=maxitem;									/*		new item = bottom	*/
			break;

		/*
		**	Handle mouse button press. Set selection and then fall into the
		**	normal menu item select logic.
		*/
		case KN_RMOUSE:
		case KN_LMOUSE:
			if (Coordinates_In_Region(_Kbd->MouseQX,_Kbd->MouseQY,mx1,my1,mx2,my2)) {
				newitem = (_Kbd->MouseQY - my1) / menuskip;
			} else {
				UnknownKey = key;			//	Pass the unprocessed button click back.
				break;
			}

		/*
		**	Normal menu item select logic. Will flash line and exit with menu
		**	selection number.
		*/
		case KN_RETURN:										/* if a selection is 	*/
		case KN_SPACE:											/*		made with key		*/
		case KN_CENTER:
			select=newitem;									/*		flag it made.		*/
			break;

		case 0:
			break;

		/*
		**	When no key was pressed or an unknown key was pressed, set the
		**	global record of the key and exit normally.
		**	EXCEPTION:	If the key matches the first letter of any of the
		**					menu entries, then presume it as a selection of
		**					that entry.
		*/
		default:
			for (idx = 0; idx < menuptr[ITEMSHIGH]; idx++) {
				if (toupper(*(text[Select_To_Entry(idx,field,index)])) == toupper(Keyboard::To_ASCII((KeyNumType)(key&0x0FF)))) {
					newitem = select = idx;
					break;
				}
			}
			UnknownKey = key;
			break;
	}

	if (newitem!=item) {
		Hide_Mouse();
		idx=Select_To_Entry(item,field,index);
		drawy=menuy+(item*menuskip);
		Fancy_Text_Print(text[idx],menux,drawy,normcol,TBLACK, TPF_8POINT|TPF_DROPSHADOW);
		idx=Select_To_Entry(newitem,field,index);
		drawy=menuy+(newitem*menuskip);
		Fancy_Text_Print(text[idx],menux,drawy,litcol,TBLACK, TPF_8POINT|TPF_DROPSHADOW);
		Show_Mouse();												/* resurrect the mouse	*/
	}

	if (select!=-1) {
		idx=Select_To_Entry(select,field,index);
		Hide_Mouse();												/* get rid of the mouse	*/
		drawy=menuy+(newitem*menuskip);
		Flash_Line(text[idx], menux, drawy, normcol, litcol, TBLACK);
		Show_Mouse();
		select=idx;
	}

	menuptr[MSELECTED]=newitem;							/* update menu select	*/

	return(select);
}


/***************************************************************************
 * Do_Menu -- Generic menu processor.                                      *
 *                                                                         *
 *    This helper function displays a menu of specified entries and waits  *
 *    for the player to make a selection. If a selection is made, then     *
 *    a whole number (starting at 0) is returned matching the entry        *
 *    selected. If ESC is pressed, then -1 is returned.                    *
 *                                                                         *
 * INPUT:   strings  -- A pointer to an array of pointers to text strings. *
 *                      Each entry in the list will be a menu entry that   *
 *                      can be selected.                                   *
 *                                                                         *
 *          blue     -- Should the special blue color be used to display   *
 *                      the menu?                                          *
 *                                                                         *
 * OUTPUT:  Returns with the cardinal number of the selected menu entry.   *
 *          If ESC was pressed, then -1 is returned.                       *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/16/1994 JLB : Created.                                             *
 *=========================================================================*/
int Do_Menu(char const **strings, bool blue)
{
	int	count;		// Number of entries in this menu.
	int	length;		// The width of the menu (in pixels).
	char	const **ptr;		// Working menu text pointer.
	int	selection;	// Selection from user.

	if (!strings) return(-1);
	Set_Logic_Page(SeenBuff);
	Keyboard::Clear();

	/*
	**	Determine the number of entries in this string.
	*/
	ptr = strings;
	count = 0;
	while (*ptr++) {
		count++;
	}
	MenuList[0][ITEMSHIGH] = count;

	/*
	**	Determine the width of the menu by finding the length of the
	**	longest menu entry.
	*/
	Fancy_Text_Print(TXT_NONE, 0, 0, 0, 0, TPF_8POINT|TPF_DROPSHADOW);
	length = 0;
	ptr = strings;
	while (*ptr) {
		length = MAX(length, (int)String_Pixel_Width(*ptr));
		ptr++;
	}
	length += 7;
	MenuList[0][ITEMWIDTH] = length >> 3;

	/*
	**	Adjust the window values to match the size of the
	**	specified menu.
	*/
	WindowList[WINDOW_MENU][WINDOWWIDTH] = MenuList[0][ITEMWIDTH] + 2;
	WindowList[WINDOW_MENU][WINDOWX] = 19 - (length >> 4);
	WindowList[WINDOW_MENU][WINDOWY] = 174 - (unsigned)(MenuList[0][ITEMSHIGH] * (FontHeight+FontYSpacing));
	WindowList[WINDOW_MENU][WINDOWHEIGHT] = MenuList[0][ITEMSHIGH] * FontHeight + 5 /*11*/;

	/*
	**	Display the menu.
	*/
	Change_Window((int)WINDOW_MENU);
	Show_Mouse();
	Window_Box(WINDOW_MENU, blue ? BOXSTYLE_BLUE_UP : BOXSTYLE_RAISED);
	Setup_Menu(0, strings, 0xFFFFL, 0, 0);

	Keyboard::Clear();
	selection = -1;
	UnknownKey = 0;
	while (selection == -1) {
		Call_Back();
		selection = Check_Menu(0, strings, NULL, 0xFFL, 0);
		if (UnknownKey != 0 || UnknownKey == KN_ESC || UnknownKey==KN_LMOUSE || UnknownKey==KN_RMOUSE) break;
	}
	Keyboard::Clear();
	Hide_Mouse();

	Blit_Hid_Page_To_Seen_Buff();
	Change_Window((int)WINDOW_MAIN);
	Map.Flag_To_Redraw(true);
	return(selection);
}
#endif


/***************************************************************************
 * Main_Menu -- Menu processing                                            *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		index of item selected, -1 if time out											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/17/1995 BRR : Created.                                             *
 *=========================================================================*/
int Main_Menu(unsigned long timeout)
{
	enum {
		D_DIALOG_W = 152*2,
		D_DIALOG_H = 136*2,
		D_DIALOG_X = 85*2,
		D_DIALOG_Y = 0,
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

		D_START_W = 125*2,
		D_START_H = 9*2,
		D_START_X = 98*2,
		D_START_Y = 35*2,

#ifdef BONUS_MISSIONS
		D_BONUS_W = 125*2,
		D_BONUS_H = 9*2,
		D_BONUS_X = 98*2,
		D_BONUS_Y = 0,
#endif	//BONUS_MISSIONS

		D_INTERNET_W = 125*2,
		D_INTERNET_H = 9*2,
		D_INTERNET_X = 98*2,
		D_INTERNET_Y = 36*2,

		D_LOAD_W = 125*2,
		D_LOAD_H = 9*2,
		D_LOAD_X = 98*2,
		D_LOAD_Y = 53*2,

		D_MULTI_W = 125*2,
		D_MULTI_H = 9*2,
		D_MULTI_X = 98*2,
		D_MULTI_Y = 71*2,

		D_INTRO_W = 125*2,
		D_INTRO_H = 9*2,
		D_INTRO_X = 98*2,
		D_INTRO_Y = 89*2,
#if (GERMAN | FRENCH)
		D_EXIT_W = 83*2,
#else
		D_EXIT_W = 63*2,
#endif
		D_EXIT_H = 9*2,
#if (GERMAN | FRENCH)
		D_EXIT_X = 118*2,
#else
		D_EXIT_X = 128*2,
#endif
		D_EXIT_Y = 111*2,

	};

#ifdef NEWMENU
	int starty = 25*2;
#endif

	enum {
#ifdef NEWMENU
		BUTTON_EXPAND=100*2,
		BUTTON_START,
#ifdef BONUS_MISSIONS
		BUTTON_BONUS,
#endif	//BONUS_MISSIONS
		BUTTON_INTERNET,
#else
		BUTTON_START=100*2,
#endif
		BUTTON_LOAD,
		BUTTON_MULTI,
		BUTTON_INTRO,
		BUTTON_EXIT,
	};

#ifdef NEWMENU
	bool expansions = Expansion_Present();
#endif
	KeyNumType input;								// input from user
	int retval;										// return value
	int curbutton;
#ifdef NEWMENU
#ifdef BONUS_MISSIONS
	TextButtonClass *buttons[8];
#else
	TextButtonClass *buttons[7];
#endif	//BONUS_MISSIONS
#else
	TextButtonClass *buttons[5];
#endif
//	unsigned long starttime;

	ControlClass *commands = NULL;				// the button list

#ifdef NEWMENU
#ifdef BONUS_MISSIONS
	int ystep = 13*2;
#else
	int ystep = 15*2;
#endif	//BONUS_MISSIONS

	if (expansions) ystep -= 2*2;
	TextButtonClass expandbtn (BUTTON_EXPAND, TXT_NEW_MISSIONS,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_START_X, starty, D_START_W, D_START_H);
	if (expansions) starty += ystep;

	TextButtonClass startbtn (BUTTON_START, TXT_START_NEW_GAME,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_START_X, starty, D_START_W, D_START_H);
	starty += ystep;

#ifdef BONUS_MISSIONS
	TextButtonClass bonusbtn (BUTTON_BONUS, TXT_BONUS_MISSIONS,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_BONUS_X, starty, D_BONUS_W, D_BONUS_H);
	starty += ystep;
#endif	//BONUS_MISSIONS

	TextButtonClass internetbutton(BUTTON_INTERNET, TXT_INTERNET,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_INTERNET_X, starty, D_INTERNET_W, D_INTERNET_H);
	starty += ystep;

	TextButtonClass loadbtn (BUTTON_LOAD, TXT_LOAD_MISSION,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_LOAD_X, starty, D_LOAD_W, D_LOAD_H);
	starty += ystep;
#else

	TextButtonClass startbtn (BUTTON_START, TXT_START_NEW_GAME,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_START_X, D_START_Y, D_START_W, D_START_H);

	TextButtonClass loadbtn (BUTTON_LOAD, TXT_LOAD_MISSION,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_LOAD_X, D_LOAD_Y, D_LOAD_W, D_LOAD_H);

#endif


#ifdef DEMO
	TextButtonClass multibtn (BUTTON_MULTI, TXT_ORDER_INFO,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI_X, D_MULTI_Y, D_MULTI_W, D_MULTI_H);
#else

#ifdef NEWMENU
	TextButtonClass multibtn (BUTTON_MULTI, TXT_MULTIPLAYER_GAME,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI_X, starty, D_MULTI_W, D_MULTI_H);
	starty += ystep;

	//TextButtonClass internetbutton(BUTTON_INTERNET, TXT_INTERNET,
	//	TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
	//	D_INTERNET_X, starty, D_INTERNET_W, D_INTERNET_H);
	//starty += ystep;
#else
	TextButtonClass multibtn (BUTTON_MULTI, TXT_MULTIPLAYER_GAME,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI_X, D_MULTI_Y, D_MULTI_W, D_MULTI_H);
#endif
#endif

#ifdef NEWMENU
#ifdef DEMO
	TextButtonClass introbtn (BUTTON_INTRO, TXT_JUST_INTRO,
#else	//DEMO
	TextButtonClass introbtn (BUTTON_INTRO, TXT_INTRO,
#endif	//DEMO
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_INTRO_X, starty, D_INTRO_W, D_INTRO_H);
	starty += ystep;

	TextButtonClass exitbtn (BUTTON_EXIT, TXT_EXIT_GAME,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
#if (GERMAN | FRENCH)
		//D_EXIT_X, starty);
		D_EXIT_X, starty, D_EXIT_W, D_EXIT_H);
#else
		D_EXIT_X, starty, D_EXIT_W, D_EXIT_H);
#endif
	starty += ystep;

#else

#ifdef DEMO
	TextButtonClass introbtn (BUTTON_INTRO, TXT_JUST_INTRO,
#else	//DEMO
	TextButtonClass introbtn (BUTTON_INTRO, TXT_INTRO,
#endif	//DEMO
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_INTRO_X, D_INTRO_Y, D_INTRO_W, D_INTRO_H);

	TextButtonClass exitbtn (BUTTON_EXIT, TXT_EXIT_GAME,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
#if (GERMAN | FRENCH)
		//D_EXIT_X, D_EXIT_Y);
		D_EXIT_X, D_EXIT_Y, D_EXIT_W, D_EXIT_H);
#else
		D_EXIT_X, D_EXIT_Y, D_EXIT_W, D_EXIT_H);
#endif
#endif

	/*
	**	Initialize
	*/
	Set_Logic_Page(SeenBuff);
	Keyboard::Clear();
#if (0)	//PG_TO_FIX
	starttime = TickCount.Time();
#endif
	/*
	**	Create the list
	*/
	commands = &startbtn;
#ifdef NEWMENU
	if (expansions) {
		expandbtn.Add_Tail(*commands);
	}
#endif
#ifdef BONUS_MISSIONS
	bonusbtn.Add_Tail(*commands);
#endif	//BONUS_MISSIONS


#ifndef DEMO
	internetbutton.Add_Tail(*commands);
#endif	//DEMO
	loadbtn.Add_Tail(*commands);
	multibtn.Add_Tail(*commands);
	introbtn.Add_Tail(*commands);
	exitbtn.Add_Tail(*commands);

	/*
	**	Fill array of button ptrs
	*/
#ifdef NEWMENU
	if (expansions) {
		curbutton = 0;
	} else {
		curbutton = 1;
	}
	int butt = 0;

	buttons[butt++] = &expandbtn;
	buttons[butt++] = &startbtn;
#ifdef BONUS_MISSIONS
	buttons[butt++] = &bonusbtn;
#endif	//BONUS_MISSIONS
	buttons[butt++] = &internetbutton;
	buttons[butt++] = &loadbtn;
	buttons[butt++] = &multibtn;
	buttons[butt++] = &introbtn;
	buttons[butt++] = &exitbtn;
#else
	curbutton = 0;
	buttons[0] = &startbtn;
	buttons[1] = &loadbtn;
	buttons[2] = &multibtn;
	buttons[3] = &introbtn;
	buttons[4] = &exitbtn;
#endif
	buttons[curbutton]->Turn_On();

	Keyboard::Clear();

	Fancy_Text_Print(TXT_NONE, 0, 0, CC_GREEN, TBLACK, TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);
	while (Get_Mouse_State() > 0) Show_Mouse();

	/*
	**	Main Processing Loop.
	*/
	bool display = true;
	bool process = true;
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=TRUE;
		}

		/*
		**	If timeout expires, bail
		*/
//PG_TO_FIX
#if (0)
		if (timeout && TickCount.Time() - starttime > timeout) {
			retval = -1;
			process = false;
		}
#endif
		/*
		**	Invoke game callback.
		*/
		Call_Back();

		/*
		**	Refresh display if needed.
		*/
		if (display) {

			/*
			**	Load the background picture.
			*/
			Load_Title_Screen("HTITLE.PCX", &HidPage, Palette);
			Blit_Hid_Page_To_Seen_Buff();

			/*
			**	Display the title and text overlay for the menu.
			*/
			Set_Logic_Page(HidPage);
			Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
			Draw_Caption (TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);
#ifdef VIRGIN_CHEAT_KEYS
#ifdef DEMO
			Version_Number();
			Fancy_Text_Print("Demo%s", D_DIALOG_X+D_DIALOG_W-5*2, D_DIALOG_Y+D_DIALOG_H-10*2, DKGREY, TBLACK, TPF_6POINT|TPF_FULLSHADOW|TPF_RIGHT, VersionText);
#else
			Fancy_Text_Print("V.%d%s", D_DIALOG_X+D_DIALOG_W-5*2, D_DIALOG_Y+D_DIALOG_H-10*2, DKGREY, TBLACK, TPF_6POINT|TPF_FULLSHADOW|TPF_RIGHT, Version_Number(), VersionText, FOREIGN_VERSION_NUMBER);
#endif
//			Fancy_Text_Print("V.%d%s%02d", D_DIALOG_X+D_DIALOG_W-5, D_DIALOG_Y+D_DIALOG_H-10, DKGREY, TBLACK, TPF_6POINT|TPF_FULLSHADOW|TPF_RIGHT, Version_Number(), VersionText, FOREIGN_VERSION_NUMBER);
#else
#ifdef DEMO
			Version_Number();
			Fancy_Text_Print("Demo%s", D_DIALOG_X+D_DIALOG_W-5*2, D_DIALOG_Y+D_DIALOG_H-10*2, DKGREY, TBLACK, TPF_6POINT|TPF_FULLSHADOW|TPF_RIGHT, VersionText);
#else
			Fancy_Text_Print("V.%d%s", D_DIALOG_X+D_DIALOG_W-5*2, D_DIALOG_Y+D_DIALOG_H-10*2, DKGREY, TBLACK, TPF_6POINT|TPF_FULLSHADOW|TPF_RIGHT, Version_Number(), VersionText);
#endif
#endif

			/*
			**	Copy the menu to the visible page.
			*/
			Hide_Mouse();
			Blit_Hid_Page_To_Seen_Buff();
			Show_Mouse();

			Set_Logic_Page(SeenBuff);
			startbtn.Draw_All();
			if (ScreenWidth==320){
				//ST - 1/2/2019 5:27PM
				//ModeX_Blit (SeenBuff.Get_Graphic_Buffer());
			}
			display = false;
		} else {
			if (RunningAsDLL) {
				retval = -1;
				process = false;
			}
		}	

#ifndef DEMO
		/*
		** Check to see if WChat has told us to start playing an internet game
		*/
		if (DDEServer.Get_MPlayer_Game_Info()){
			retval = BUTTON_INTERNET - BUTTON_EXPAND;
			process = false;
		}
#endif	//DEMO

		/*
		**	Get and process player input.
		*/
		input = commands->Input();
		switch (input) {
#ifdef NEWMENU
			case (BUTTON_EXPAND | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
				process = false;
				break;

			case (BUTTON_INTERNET | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
				process = false;
				break;

#else
#define	BUTTON_EXPAND	BUTTON_START
#endif

			case (BUTTON_START | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
				process = false;
				break;

#ifdef BONUS_MISSIONS
			case (BUTTON_BONUS | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
				process = false;
				break;
#endif	//BONUS_MISSIONS

			case (BUTTON_LOAD | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
#ifdef DEMO
				retval += 1;
#endif	//DEMO
				process = false;
				break;

			case (BUTTON_MULTI | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
#ifdef DEMO
				retval += 1;
#endif	//DEMO
				process = false;
				break;

			case (BUTTON_INTRO | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
#ifdef DEMO
				retval += 1;
#endif	//DEMO
				process = false;
				break;

			case (BUTTON_EXIT | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
#ifdef DEMO
				retval += 1;
#endif	//DEMO
				process = false;
				break;

			case KN_UP:
				buttons[curbutton]->Turn_Off();
				buttons[curbutton]->Flag_To_Redraw();
				curbutton--;
#ifdef NEWMENU
				if (expansions) {
					if (curbutton < 0) {
						curbutton = 6;
					}
				} else {
					if (curbutton < 1) {
						curbutton = 6;
					}
				}
#else
				if (curbutton < 0) {
					curbutton = 4;
				}
#endif
				buttons[curbutton]->Turn_On();
				buttons[curbutton]->Flag_To_Redraw();
				break;

			case KN_DOWN:
				buttons[curbutton]->Turn_Off();
				buttons[curbutton]->Flag_To_Redraw();
				curbutton++;
#ifdef NEWMENU
				if (curbutton > 6) {
					if (expansions) {
						curbutton = 0;
					} else {
						curbutton = 1;
					}
				}
#else
				if (curbutton > 4) {
					curbutton = 0;
				}
#endif
				buttons[curbutton]->Turn_On();
				buttons[curbutton]->Flag_To_Redraw();
				break;

			case KN_RETURN:
				buttons[curbutton]->IsPressed = true;
				buttons[curbutton]->Draw_Me(true);
				retval = curbutton;
				process = false;
				break;

			default:
				break;
		}
	}
	return(retval);
}