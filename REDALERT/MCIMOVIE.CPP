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

#include "function.h"

#ifdef MCIMPEG
#include "mcimovie.h"
#include <memory.h>

/****************************************************************************
*
*	NAME
*     MCIMovie - Constructor
*
* DESCRIPTION
*
* INPUTS
*     HInstance - Application instance handle
*
* RESULT
*     NONE
*
****************************************************************************/

MCIMovie::MCIMovie(HWND mainWindow)
	: mMainWindow(mainWindow), mMCIWindow(NULL), mName(NULL), mDeviceID(0)
	{
	mWidth = mHeight = 0;
	}


/****************************************************************************
*
* NAME
*     ~MCIMovie - Destructor
*
* DESCRIPTION
*
* INPUTS
*
* RESULT
*
****************************************************************************/

MCIMovie::~MCIMovie()
	{
	// Stop any playing movie
	Close();

	// Free name
	if (mName != NULL)
		free(mName);
	}


/****************************************************************************
*
* NAME
*     Open()
*
* DESCRIPTION
*     Open the media file in preparation for playback.
*
* INPUTS
*     NONE
*
* RESULT
*     Success - Success/Failure flag
*
****************************************************************************/

bool MCIMovie::Open(const char* name, const char* device)
	{
	MCIERROR rc;
	MCI_DGV_RECT_PARMS rectParm;
	MCI_BREAK_PARMS breakParm;

	// Stop any currently playing movie
	Close();

	// Copy the movie name for our use
	if (mName != NULL)
		free(mName);

	mName = strdup(name);

	if (device == NULL)
		device = "mpegvideo";
		
	// Setup open parameters
	memset((void*)&mOpenParm, 0, sizeof(mOpenParm));
	mOpenParm.dwCallback = NULL;
	mOpenParm.lpstrDeviceType = device;
	mOpenParm.lpstrElementName = name;

	rc = mciSendCommand(0, MCI_OPEN, MCI_WAIT | MCI_OPEN_TYPE |
		MCI_OPEN_ELEMENT, (DWORD)&mOpenParm);
			
	if (rc)
		{
		char buffer[512];
		mciGetErrorString(rc, buffer, 512);
		return false;
		}

	// Set device ID
	mDeviceID = mOpenParm.wDeviceID;

	// Retrieve movie dimensions
	rectParm.dwCallback = NULL;

	rc = mciSendCommand(mDeviceID, MCI_WHERE, MCI_WAIT | MCI_DGV_WHERE_SOURCE,
			(DWORD)&rectParm);

	if (rc)
		{
		char buffer[512];
		mciGetErrorString(rc, buffer, 512);
		return false;
		}

	mWidth = rectParm.rc.right - rectParm.rc.left;
	mHeight = rectParm.rc.bottom - rectParm.rc.top;

	// Set break key to escape
 	breakParm.dwCallback = NULL;
	breakParm.nVirtKey = VK_ESCAPE;
	breakParm.hwndBreak = mMainWindow;
	
	rc = mciSendCommand(mDeviceID, MCI_BREAK, MCI_WAIT | MCI_BREAK_HWND |
		MCI_BREAK_KEY, (DWORD)&breakParm);

	if (rc)
		{
		char buffer[512];
		mciGetErrorString(rc, buffer, 512);
		}
	
	return true;
	}


/****************************************************************************
*
* NAME
*     Play - Play the specified movie.
*
* DESCRIPTION
*
* INPUTS
*
* RESULT
*     Success - Success/Failure flag
*
****************************************************************************/

bool MCIMovie::Play(HWND window)
	{
	MCIERROR rc;
	
	if (mDeviceID == 0)
		return false;
		
	// Provide window for playback
	if (AttachWindow(window))
		{
		// Size the video area
		if (SizeDestination())
			{
			// Start playing
			memset((void*)&mPlayParm, 0, sizeof(mPlayParm));
			mPlayParm.dwCallback = NULL;

			rc = mciSendCommand(mDeviceID, MCI_PLAY, MCI_WAIT, (DWORD)&mPlayParm);

			if (rc)
				{
				char buffer[512];
				mciGetErrorString(rc, buffer, 512);
				return false;
				}
				
			Close();
			}
		}

	return true;
	}


/****************************************************************************
*
* NAME
*     Pause
*
* DESCRIPTION
*
* INPUTS
*
* RESULT
*     Success - Success/Failure flag
*
****************************************************************************/

bool MCIMovie::Pause(void)
	{
	if (mDeviceID == 0)
		return false;

	if (mciSendCommand(mDeviceID, MCI_PAUSE, 0, (DWORD)NULL))
		return false;

	return true;
	}


/****************************************************************************
*
* NAME
*     Stop
*
* DESCRIPTION
*
* INPUTS
*
* RESULT
*     Success - Success/Failure flag
*
****************************************************************************/

bool MCIMovie::Close(void)
	{
	MCIERROR rc;
	
	if (mDeviceID == 0)
		return false;

	rc = mciSendCommand(mDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
	mDeviceID = 0;

	if (rc)
		{
		char buffer[512];
		mciGetErrorString(rc, buffer, 512);
		return false;
		}

	return true;
	}


/****************************************************************************
*
* NAME
*     SizeDestination
*
* DESCRIPTION
*
* INPUTS
*
* RESULT
*
****************************************************************************/

bool MCIMovie::SizeDestination(void)
	{
	MCIERROR rc;
	MCI_DGV_PUT_PARMS putParm;
	RECT rect;

	if (mMCIWindow == NULL)
		return false;

	GetClientRect(mMCIWindow, &rect);
	ClientToScreen(mMCIWindow, (LPPOINT)&rect);
	ClientToScreen(mMCIWindow, (LPPOINT)&rect + 1);

	putParm.dwCallback = NULL;
	putParm.rc.left = rect.left;
	putParm.rc.top = rect.top;
	putParm.rc.right = rect.right;
	putParm.rc.bottom = rect.bottom;

	rc = mciSendCommand(mDeviceID, MCI_PUT, MCI_WAIT | MCI_DGV_RECT |
		MCI_DGV_PUT_DESTINATION, (DWORD)&putParm);

	if (rc)
		{
		char buffer[512];
		mciGetErrorString(rc, buffer, 512);
		return false;
		}

	return true;
	}


/****************************************************************************
*
* NAME
*     AttachWindow
*
* DESCRIPTION
*
* INPUTS
*
* RESULT
*
****************************************************************************/

bool MCIMovie::AttachWindow(HWND window)
	{
	MCIERROR rc;
	MCI_DGV_WINDOW_PARMS winParm;

	mMCIWindow = window;

	memset((void*)&winParm, 0, sizeof(winParm));
	winParm.dwCallback = NULL;
	winParm.hWnd = window;
	winParm.nCmdShow = SW_SHOW;

	rc = mciSendCommand(mDeviceID, MCI_WINDOW, MCI_WAIT| MCI_DGV_WINDOW_HWND |
			MCI_DGV_WINDOW_STATE, (DWORD)&winParm);

	if (rc)
		{
		char buffer[512];
		mciGetErrorString(rc, buffer, 512);
		return false;
		}

	return true;
	}

#endif // MCIMPEG

