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

/* $Header:   F:\projects\c&c\vcs\code\special.cpv   1.4   16 Oct 1995 16:50:06   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SPECIAL.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/27/95                                                     *
 *                                                                                             *
 *                  Last Update : May 27, 1995 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#define	OPTION_WIDTH	236
#define	OPTION_HEIGHT	162
#define	OPTION_X			((320 - OPTION_WIDTH) / 2)
#define	OPTION_Y			(200 - OPTION_HEIGHT) / 2

void Special_Dialog(void)
{
	SpecialClass oldspecial = Special;
	GadgetClass * buttons = NULL;
	static struct {
		int Description;
		int Setting;
		CheckBoxClass * Button;
	} _options[] = {
//		{TXT_DEFENDER_ADVANTAGE, 0, 0},
		{TXT_SEPARATE_HELIPAD, 0, 0},
		{TXT_VISIBLE_TARGET, 0, 0},
		{TXT_TREE_TARGET, 0, 0},
		{TXT_MCV_DEPLOY, 0, 0},
		{TXT_SMART_DEFENCE, 0, 0},
		{TXT_THREE_POINT, 0, 0},
//		{TXT_TIBERIUM_GROWTH, 0, 0},
//		{TXT_TIBERIUM_SPREAD, 0, 0},
		{TXT_TIBERIUM_FAST, 0, 0},
		{TXT_ROAD_PIECES, 0, 0},
		{TXT_SCATTER, 0, 0},
		{TXT_SHOW_NAMES, 0, 0},
	};

	TextButtonClass ok(200, TXT_OK, TPF_6PT_GRAD|TPF_NOSHADOW, OPTION_X+5, OPTION_Y+OPTION_HEIGHT-15);
	TextButtonClass cancel(201, TXT_CANCEL, TPF_6PT_GRAD|TPF_NOSHADOW, OPTION_X+OPTION_WIDTH-50, OPTION_Y+OPTION_HEIGHT-15);
	buttons = &ok;
	cancel.Add(*buttons);
	int index;
	for (index = 0; index < sizeof(_options)/sizeof(_options[0]); index++) {
		_options[index].Button = new CheckBoxClass(100+index, OPTION_X+7, OPTION_Y+20+(index*10));
		if (_options[index].Button) {
			_options[index].Button->Add(*buttons);

			bool value = false;
			switch (_options[index].Description) {
				case TXT_SEPARATE_HELIPAD:
					value = Special.IsSeparate;
					break;

				case TXT_SHOW_NAMES:
					value = Special.IsNamed;
					break;

				case TXT_DEFENDER_ADVANTAGE:
					value = Special.IsDefenderAdvantage;
					break;

				case TXT_VISIBLE_TARGET:
					value = Special.IsVisibleTarget;
					break;

				case TXT_TREE_TARGET:
					value = Special.IsTreeTarget;
					break;

				case TXT_MCV_DEPLOY:
					value = Special.IsMCVDeploy;
					break;

				case TXT_SMART_DEFENCE:
					value = Special.IsSmartDefense;
					break;

				case TXT_THREE_POINT:
					value = Special.IsThreePoint;
					break;

				case TXT_TIBERIUM_GROWTH:
					value = Special.IsTGrowth;
					break;

				case TXT_TIBERIUM_SPREAD:
					value = Special.IsTSpread;
					break;

				case TXT_TIBERIUM_FAST:
					value = Special.IsTFast;
					break;

				case TXT_ROAD_PIECES:
					value = Special.IsRoad;
					break;

				case TXT_SCATTER:
					value = Special.IsScatter;
					break;
			}

			_options[index].Setting = value;
			if (value) {
				_options[index].Button->Turn_On();
			} else {
				_options[index].Button->Turn_Off();
			}
		}
	}

	Map.Override_Mouse_Shape(MOUSE_NORMAL);
	Set_Logic_Page(SeenBuff);
	bool recalc = true;
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

		if (GameToPlay == GAME_NORMAL) {
			Call_Back();
		} else {
			if (Main_Loop()) {
				process = false;
			}
		}

		if (display) {
			display = false;

			Hide_Mouse();
			Dialog_Box(OPTION_X, OPTION_Y, OPTION_WIDTH, OPTION_HEIGHT);
			Draw_Caption(TXT_SPECIAL_OPTIONS, OPTION_X, OPTION_Y, OPTION_WIDTH);

			for (index = 0; index < sizeof(_options)/sizeof(_options[0]); index++) {
				Fancy_Text_Print(_options[index].Description, _options[index].Button->X+10, _options[index].Button->Y, CC_GREEN, TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);
			}
			buttons->Draw_All();
			Show_Mouse();
		}

		KeyNumType input = buttons->Input();
		switch (input) {
			case KN_ESC:
			case 200|KN_BUTTON:
				process = false;
				for (index = 0; index < sizeof(_options)/sizeof(_options[0]); index++) {
					switch (_options[index].Description) {
						case TXT_SEPARATE_HELIPAD:
							oldspecial.IsSeparate = _options[index].Setting;
							break;

						case TXT_SHOW_NAMES:
							oldspecial.IsNamed = _options[index].Setting;
							break;

						case TXT_DEFENDER_ADVANTAGE:
							oldspecial.IsDefenderAdvantage = _options[index].Setting;
							break;

						case TXT_VISIBLE_TARGET:
							oldspecial.IsVisibleTarget = _options[index].Setting;
							break;

						case TXT_TREE_TARGET:
							oldspecial.IsTreeTarget = _options[index].Setting;
							break;

						case TXT_MCV_DEPLOY:
							oldspecial.IsMCVDeploy = _options[index].Setting;
							break;

						case TXT_SMART_DEFENCE:
							oldspecial.IsSmartDefense = _options[index].Setting;
							break;

						case TXT_THREE_POINT:
							oldspecial.IsThreePoint = _options[index].Setting;
							break;

						case TXT_TIBERIUM_GROWTH:
							oldspecial.IsTGrowth = _options[index].Setting;
							break;

						case TXT_TIBERIUM_SPREAD:
							oldspecial.IsTSpread = _options[index].Setting;
							break;

						case TXT_TIBERIUM_FAST:
							oldspecial.IsTFast = _options[index].Setting;
							break;

						case TXT_ROAD_PIECES:
							oldspecial.IsRoad = _options[index].Setting;
							break;

						case TXT_SCATTER:
							oldspecial.IsScatter = _options[index].Setting;
							break;
					}
				}
				OutList.Add(EventClass(oldspecial));
				break;

			case 201|KN_BUTTON:
				process = false;
				break;

			case KN_NONE:
				break;

			default:
				index = (input & ~KN_BUTTON) - 100;
				if ((unsigned)index < sizeof(_options)/sizeof(_options[0])) {
					_options[index].Setting = (_options[index].Setting == false);
					if (_options[index].Setting) {
						_options[index].Button->Turn_On();
					} else {
						_options[index].Button->Turn_Off();
					}
				}
				break;
		}
	}

	Map.Revert_Mouse_Shape();
	HiddenPage.Clear();
	Map.Flag_To_Redraw(true);
	Map.Render();
}





