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

/****************************************************************************
*
* FILE
*     MpgSet.cpp
*
* DESCRIPTION
*     Mpeg movie settings manager
*
* PROGRAMMER
*     Denzil E. Long, Jr.
*
* DATE
*     June 30, 1998
*
****************************************************************************/

#include "function.h"

#ifdef DVD
#include "mpgset.h"

#ifdef MCIMPEG
bool EnumMCI(MCIDevice* desc, void* context);
#endif

/****************************************************************************
*
* NAME
*     MPGSettings(DeviceName)
*
* DESCRIPTION
*     Default constructor
*
* INPUTS
*     DeviceName - Initial device to use for MPG playback (NULL = DXMedia)
*
* RESULT
*     NONE
*
****************************************************************************/

MPGSettings::MPGSettings(const char* deviceName)
	: mDeviceName(NULL)
	{
	SetDeviceName(deviceName);

	#ifdef MCIMPEG
	int count = mMCI.GetDeviceCount();

	mMCIDevices = NULL;
	mCount = 0;
	
	if (count)
		{
		mMCIDevices = new MCIDevice[count];
		
		if (mMCIDevices)
			{
			mMCI.EnumerateDevices(EnumMCI, this);
			}
		}
	#endif
	}


MPGSettings::MPGSettings(FileClass& file)
	: mDeviceName(NULL)
	{
	INIClass ini;
	char buffer[256];
	char* device = NULL;

	#ifdef MCIMPEG
	int count = mMCI.GetDeviceCount();

	mMCIDevices = NULL;
	mCount = 0;
	
	// Enumerate all the MCI devices that can play a movie
	if (count)
		{
		mMCIDevices = new MCIDevice[count];

		if (mMCIDevices)
			{
			mMCI.EnumerateDevices(EnumMCI, this);
			}
		}

	#endif
	
	// Retrieve the user specified device from the config file
	buffer[0] = '\0';

	if (ini.Load(file))
		{
		ini.Get_String("MovieSettings", "Device", "Default", buffer, sizeof(buffer));
		}

	// If there is a specification in the config and it isn't the default
	if ((strlen(buffer) != 0) && (stricmp(buffer, "Default") != 0))
		{
		#ifdef MCIMPEG
		// Search for selection
		for (int i = 0; i < mCount; i++)
			{
			if (stricmp(buffer, mMCIDevices[i].name) == 0)
				{
				device = mMCIDevices[i].name;
				break;
				}
			}
		#endif
		}

	SetDeviceName(device);
	}


/****************************************************************************
*
* NAME
*     ~MPGSettings
*
* DESCRIPTION
*     Destructor
*
* INPUTS
*     NONE
*
* RESULT
*     NONE
*
****************************************************************************/

MPGSettings::~MPGSettings(void)
	{
	if (mDeviceName)
		free(mDeviceName);
		
	#ifdef MCIMPEG
	if (mMCIDevices)
		delete[] mMCIDevices;
	#endif
	}


/****************************************************************************
*
* NAME
*     SetDeviceName(DeviceName)
*
* DESCRIPTION
*     Change current device used for mpeg playback
*
* INPUTS
*     DeviceName - Device name type (IE: mpegvideo)
*
* RESULT
*     NONE
*
****************************************************************************/

void MPGSettings::SetDeviceName(const char* deviceName)
	{
	if (mDeviceName)
		free(mDeviceName);

	mDeviceName = NULL;

	if (deviceName)
		mDeviceName = strdup(deviceName);
	}


bool MPGSettings::Save(FileClass& file)
	{
	INIClass ini;

	if (ini.Load(file))
		{
		const char* device = GetDeviceName();
		
		if (device)
			{
			ini.Put_String("MovieSettings", "Device", device);
			}
		else
			{
			ini.Put_String("MovieSettings", "Device", "Default");
			}
			
		ini.Save(file);
		return true;
		}

	return false;
	}


/****************************************************************************
*
* NAME
*     Dialog()
*
* DESCRIPTION
*     Mpeg playback settings dialog
*
* INPUTS
*     NONE
*
* RESULT
*     NONE
*
****************************************************************************/

void MPGSettings::Dialog(void)
	{
	//	Dialog & button dimensions
	int d_dialog_w = 200 *RESFACTOR;
	int d_dialog_h = 100 *RESFACTOR;
	int d_dialog_x = (((320*RESFACTOR) - d_dialog_w) / 2);
	int d_dialog_y = 70 * RESFACTOR;
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);

	int d_txt6_h = 7 *RESFACTOR;
	int d_margin = 7 *RESFACTOR;

	int d_okay_w = 40 *RESFACTOR;
	int d_okay_h = 9 *RESFACTOR;
	int d_okay_x = d_dialog_x + d_margin + 20;
	int d_okay_y = ((d_dialog_y + d_dialog_h) - (d_okay_h + 20));

	int d_test_w = 40 *RESFACTOR;
	int d_test_h = 9 *RESFACTOR;
	int d_test_x = (d_dialog_cx - (d_test_w / 2));
	int d_test_y = ((d_dialog_y + d_dialog_h) - (d_test_h + 20));

	int d_cancel_w = 40 *RESFACTOR;
	int d_cancel_h = 9 *RESFACTOR;
	int d_cancel_x = ((d_dialog_x + d_dialog_w) - (d_cancel_w + d_margin + 20));
	int d_cancel_y = ((d_dialog_y + d_dialog_h) - (d_cancel_h + 20));

	int d_method_w = 165 * RESFACTOR;
	int d_method_h = 50 * RESFACTOR;
	int d_method_x = (d_dialog_cx - (d_method_w / 2));
	int d_method_y = (d_dialog_y + 40);

	//	Button enumerations:
	enum
		{
		BUTTON_OKAY = 100,
		BUTTON_TEST,
		BUTTON_CANCEL,
		BUTTON_METHOD,
		NUM_OF_BUTTONS = 4,
		};

	int num_of_buttons = NUM_OF_BUTTONS;
	
	//	Redraw values: in order from "top" to "bottom" layer of the dialog
	typedef enum
		{
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,		// includes map interior & coord values
		REDRAW_BACKGROUND,	// includes box, map bord, key, coord labels, btns
		REDRAW_ALL = REDRAW_BACKGROUND
		} RedrawType;

	//	Dialog variables:
	KeyNumType input;								// input from user
	bool process;									// loop while true
	RedrawType display;							// true = re-draw everything
	int selection;
	bool pressed;
	int curbutton;
	TextButtonClass * buttons[NUM_OF_BUTTONS];
	int i;
	char* origDevice = NULL;

	//	Buttons
	ControlClass * commands = NULL;				// the button list

	TextButtonClass okaybtn(BUTTON_OKAY, TXT_OK, TPF_BUTTON,
		d_okay_x, d_okay_y, d_okay_w, d_okay_h);

	TextButtonClass testbtn(BUTTON_TEST, "Test", TPF_BUTTON,
		d_test_x, d_test_y, d_test_w, d_test_h);

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_BUTTON,
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);

	ListClass method(BUTTON_METHOD, d_method_x, d_method_y, d_method_w,
		d_method_h, TPF_TEXT, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
		
	//	Initialize
	Set_Logic_Page(SeenBuff);
	
	//	Create the list
	commands = &okaybtn;
	testbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);
	method.Add_Tail(*commands);

	//	Fill array of button ptrs
	curbutton = 0;
	buttons[0] = &okaybtn;
	buttons[1] = &testbtn;
	buttons[2] = &cancelbtn;
	buttons[curbutton]->Turn_On();

	Keyboard->Clear();

	Fancy_Text_Print(TXT_NONE, 0, 0, GadgetClass::Get_Color_Scheme(), TBLACK,
		TPF_CENTER | TPF_TEXT);

	// Add device to device control
	method.Add_Item("DirectX Media (Default)");

	#ifdef MCIMPEG
	for (i = 0; i < mCount; i++)
		{
		if (mMCIDevices[i].description != NULL)
			{
			method.Add_Item(mMCIDevices[i].description);
			}
		}
	#endif
	
	method.Set_Selected_Index(0);
	
	#ifdef MCIMPEG
	// Search for current selection
	if (GetDeviceName())
		{
		for (i = 0; i < mCount; i++)
			{
			if (stricmp(GetDeviceName(), mMCIDevices[i].name) == 0)
				{
				method.Set_Selected_Index(i + 1);
				break;
				}
			}
		}
	#endif
	
	// Save original device selection
	if (GetDeviceName())
		origDevice = strdup(GetDeviceName());
						
	//------------------------------------------------------------------------
	//	Main Processing Loop
	//------------------------------------------------------------------------
	display = REDRAW_ALL;
	process = true;
	pressed = false;
	
	while (process)
		{
		#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored)
			{
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
			}
		#endif

		//	Invoke game callback
		Call_Back();

		//	Refresh display if needed
		if (display)
			{
			Hide_Mouse();
			
			if (display >= REDRAW_BACKGROUND)
				{
				//	Refresh the backdrop
				Load_Title_Page(true);
				CCPalette.Set();

				//	Draw the background
				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);
				Draw_Caption("Movie Settings", d_dialog_x, d_dialog_y, d_dialog_w);
				}

			//	Redraw buttons
			if (display >= REDRAW_BUTTONS)
				commands->Flag_List_To_Redraw();
				
			Show_Mouse();
			display = REDRAW_NONE;
			}

		//	Get user input
		input = commands->Input();

		//	Process input
		switch (input)
			{
			case (BUTTON_OKAY | KN_BUTTON):
				selection = BUTTON_OKAY;
				pressed = true;
			break;

			case (BUTTON_TEST | KN_BUTTON):
				selection = BUTTON_TEST;
				pressed = true;
			break;

			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				selection = BUTTON_CANCEL;
				pressed = true;
			break;

			case KN_UP:
				buttons[curbutton]->Turn_Off();
				buttons[curbutton]->Flag_To_Redraw();
				curbutton--;
				
				if (curbutton < 0)
					curbutton = (num_of_buttons - 1);
					
				buttons[curbutton]->Turn_On();
				buttons[curbutton]->Flag_To_Redraw();
			break;

			case KN_DOWN:
				buttons[curbutton]->Turn_Off();
				buttons[curbutton]->Flag_To_Redraw();
				curbutton++;
				
				if (curbutton > (num_of_buttons - 1))
					curbutton = 0;
					
				buttons[curbutton]->Turn_On();
				buttons[curbutton]->Flag_To_Redraw();
			break;

			case KN_RETURN:
				selection = curbutton + BUTTON_OKAY;
				pressed = true;
			break;

			default:
			break;
			}

		if (pressed)
			{
			// to make sure the selection is correct in case they used the mouse
			buttons[curbutton]->Turn_Off();
			buttons[curbutton]->Flag_To_Redraw();

			curbutton = selection - BUTTON_OKAY;
			buttons[curbutton]->Turn_On();
			buttons[curbutton]->IsPressed = true;
			buttons[curbutton]->Draw_Me(true);

			switch (selection)
				{
				case (BUTTON_TEST):
					buttons[curbutton]->IsPressed = false;

					if (method.Current_Index() == 0)
						{
						SetDeviceName(NULL);
						}
					else
						{
						i = method.Current_Index();
						#ifdef MCIMPEG
						SetDeviceName(mMCIDevices[i - 1].name);
						#endif
						}

					Theme.Fade_Out();
					Hide_Mouse();
					VisiblePage.Clear();
					//PlayMpegMovie("acrop");	//PG
					Keyboard->Clear();
					Show_Mouse();
					Theme.Queue_Song(THEME_CRUS);
					display = REDRAW_ALL;

					buttons[curbutton]->Turn_Off();
					buttons[curbutton]->Flag_To_Redraw();
					buttons[0]->Turn_On();
					curbutton = 0;
				break;

				case (BUTTON_OKAY):
					process = false;
					
					if (method.Current_Index() == 0)
						{
						SetDeviceName(NULL);
						}
					else
						{
						i = method.Current_Index();
						#ifdef MCIMPEG
						SetDeviceName(mMCIDevices[i - 1].name);
						#endif
						}

					{
					RawFileClass file(CONFIG_FILE_NAME);
					Save(file);
					}
				break;

				case (BUTTON_CANCEL):
					process = false;
					SetDeviceName(origDevice);
				break;
				}

			pressed = false;
			}
		}

	if (origDevice)
		free(origDevice);
	}


#ifdef MCIMPEG
/****************************************************************************
*
* NAME
*     EnumMCI(DeviceDesc, Context)
*
* DESCRIPTION
*     MCI device enumeration callback
*
* INPUTS
*     DeviceDesc - MCI device description
*     Context    - User defined context variable
*
* RESULT
*     Continue - Continue with next device flag
*
****************************************************************************/

bool EnumMCI(MCIDevice* desc, void* context)
	{
	MPGSettings* mpgset = (MPGSettings*)context;

	// Digital video device type?
	if (desc->type == MCI_DEVTYPE_DIGITAL_VIDEO)
		{
		if (MciMovie)
			{
			CCFileClass file;
			const char* filename;

			filename = file.Set_Name("movies\\acrop.mpg");
	
			if (!file.Is_Available())
				{
				char buffer[256];
				sprintf(buffer, "Couldn't test MCI device %s\n", desc->name);
				VisiblePage.Clear();
				GamePalette.Set();
				Show_Mouse();
				WWMessageBox().Process(buffer);
				Hide_Mouse();
				VisiblePage.Clear();
				return true;
				}
			
			if (MciMovie->Open(filename, desc->name))
				{
				MciMovie->Close();
				memcpy((void*)&mpgset->mMCIDevices[mpgset->mCount], (void*)desc,
					sizeof(MCIDevice));
				mpgset->mCount++;
				}
			}
		}

	return true;
	}
#endif
#endif
