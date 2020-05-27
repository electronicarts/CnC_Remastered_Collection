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
*     MCI.cpp
*
* DESCRIPTION
*
* PROGRAMMER
*     Denzil E. Long, Jr.
*
* DATE
*     6/22/98
*
****************************************************************************/

#include "function.h"

#ifdef MCIMPEG
#include "mci.h"

/****************************************************************************
*
* NAME
*     GetDeviceCount()
*
* DESCRIPTION
*
* INPUTS
*     NONE
*
* RESULT
*     Count - Number of MCI device entries
*
****************************************************************************/

unsigned int MCI::GetDeviceCount(void)
	{
	MCIERROR rc;
	MCI_SYSINFO_PARMS sysInfo;
	unsigned int count;

	memset(&sysInfo, 0, sizeof(sysInfo));
	sysInfo.lpstrReturn = (LPSTR)&count;
	sysInfo.dwRetSize = sizeof(count);

	rc = mciSendCommand(MCI_ALL_DEVICE_ID, MCI_SYSINFO,
			MCI_WAIT | MCI_SYSINFO_QUANTITY, (DWORD)&sysInfo);

	if (rc)
		return 0;

	return count;
	}


/****************************************************************************
*
* NAME
*     GetDeviceName(entry, name)
*
* DESCRIPTION
*
* INPUTS
*     Entry - Entry number to get name for.
*     Name  - On return; device entry name
*
* RESULT
*     Success - Success / Failure flag
*
****************************************************************************/

bool MCI::GetDeviceName(unsigned int item, char* buffer)
	{
	MCIERROR rc;
	MCI_SYSINFO_PARMS sysInfo;

	// Get device name
	memset(&sysInfo, 0, sizeof(sysInfo));
	sysInfo.lpstrReturn = (LPSTR)buffer;
	sysInfo.dwRetSize = 63;
	sysInfo.dwNumber = item;

	rc = mciSendCommand(MCI_ALL_DEVICE_ID, MCI_SYSINFO,
			MCI_WAIT | MCI_SYSINFO_NAME, (DWORD)&sysInfo);

	if (rc)
		return false;

	return true;
	}


/****************************************************************************
*
* NAME
*
* DESCRIPTION
*
* INPUTS
*
* RESULT
*
****************************************************************************/

bool MCI::GetProductName(MCIDEVICEID id, char* buffer)
	{
	MCIERROR rc;
	MCI_INFO_PARMS info;

	// Get device product name
	memset(&info, 0, sizeof(info));
	info.lpstrReturn = (LPSTR)buffer;
	info.dwRetSize = 63;

	rc = mciSendCommand(id, MCI_INFO, MCI_WAIT | MCI_INFO_PRODUCT,
			(DWORD)&info);

	if (rc)
		return false;

	return true;
	}


/****************************************************************************
*
* NAME
*     OpenDevice(name)
*
* DESCRIPTION
*
* INPUTS
*     Name - Device name to open
*
* RESULT
*     DeviceID - ID of opened device, 0 if error.
*
****************************************************************************/

MCIDEVICEID MCI::OpenDevice(const char* name)
	{
	MCIERROR rc;
	MCI_OPEN_PARMS open;

	memset(&open, 0, sizeof(open));
	open.lpstrDeviceType = name;

	rc = mciSendCommand(0, MCI_OPEN, MCI_WAIT | MCI_OPEN_TYPE, (DWORD)&open);

	if (rc)
		return 0;

	return (open.wDeviceID);
	}


void MCI::CloseDevice(MCIDEVICEID id)
	{
	MCI_GENERIC_PARMS close;

	close.dwCallback = (DWORD)NULL;
	
	if (id)
		mciSendCommand(id, MCI_CLOSE, MCI_WAIT, (DWORD)&close);
	}


/****************************************************************************
*
* NAME
*     GetDeviceDescription
*
* DESCRIPTION
*
* INPUTS
*
* RESULT
*
****************************************************************************/

bool MCI::GetDeviceDescription(const char* name, MCIDevice* caps)
	{
	MCIDEVICEID id;
	unsigned long result;

	// Copy the name
	strncpy(caps->name, name, 63);

	if ((id = OpenDevice(name)) == 0)
		return false;

	// Get device product name
	GetProductName(id, caps->description);

	// Get device type
	if (GetCapability(id, MCI_GETDEVCAPS_DEVICE_TYPE, &result))
		caps->type = result;

	if (GetCapability(id, MCI_GETDEVCAPS_CAN_EJECT, &result))
		caps->canEject = ((result) ? true : false);

	if (GetCapability(id, MCI_GETDEVCAPS_CAN_PLAY, &result))
		caps->canPlay = ((result) ? true : false);

	if (GetCapability(id, MCI_GETDEVCAPS_CAN_RECORD, &result))
		caps->canRecord = ((result) ? true : false);

	if (GetCapability(id, MCI_GETDEVCAPS_CAN_SAVE, &result))
		caps->canSave = ((result) ? true : false);

	if (GetCapability(id, MCI_GETDEVCAPS_COMPOUND_DEVICE, &result))
		caps->usesDevElem = ((result) ? true : false);

	if (GetCapability(id, MCI_GETDEVCAPS_HAS_AUDIO, &result))
		caps->hasAudio = ((result) ? true : false);

	if (GetCapability(id, MCI_GETDEVCAPS_HAS_VIDEO, &result))
		caps->hasVideo = ((result) ? true : false);

	if (GetCapability(id, MCI_GETDEVCAPS_USES_FILES, &result))
		caps->reqElemFile = ((result) ? true : false);

	CloseDevice(id);

	return true;
	}


/****************************************************************************
*
* NAME
*
* DESCRIPTION
*
* INPUTS
*
* RESULT
*
****************************************************************************/

bool MCI::GetCapability(MCIDEVICEID id, unsigned long capItem,
		unsigned long* result)
	{
	MCIERROR rc;
	MCI_GETDEVCAPS_PARMS devCaps;

	memset(&devCaps, 0, sizeof(devCaps));
	devCaps.dwItem = capItem;
	rc = mciSendCommand(id, MCI_GETDEVCAPS, MCI_WAIT|MCI_GETDEVCAPS_ITEM,
		(DWORD)&devCaps);

	if (rc)
		return false;

	*result = devCaps.dwReturn;
	return true;
	}


/****************************************************************************
*
* NAME
*
* DESCRIPTION
*
* INPUTS
*
* RESULT
*
****************************************************************************/

const char* MCI::GetDeviceTypeName(unsigned long type)
	{
	static struct _DeviceType {unsigned long typeID; const char* typeName;}
		_deviceTypeNames[] =
		{
			{MCI_DEVTYPE_ANIMATION, "Animation"},
			{MCI_DEVTYPE_CD_AUDIO, "CD Audio"},
			{MCI_DEVTYPE_DAT, "DAT"},
			{MCI_DEVTYPE_DIGITAL_VIDEO, "Digital Video"},
			{MCI_DEVTYPE_OTHER, "Other"},
			{MCI_DEVTYPE_OVERLAY, "Overlay"},
			{MCI_DEVTYPE_SCANNER, "Scanner"},
			{MCI_DEVTYPE_SEQUENCER, "MIDI Sequencer"},
			{MCI_DEVTYPE_VCR, "VCR"},
			{MCI_DEVTYPE_VIDEODISC, "VideoDisc"},
			{MCI_DEVTYPE_WAVEFORM_AUDIO, "Wave Audio"},
			{0, NULL},
		};

	int i = 0;

	while (_deviceTypeNames[i].typeID != 0)
		{
		if (_deviceTypeNames[i].typeID == type)
			return _deviceTypeNames[i].typeName;

		i++;
		}

	return NULL;
	}


/****************************************************************************
*
* NAME
*     MCIEnumerate(callack, context)
*
* DESCRIPTION
*
* INPUTS
*     Callback -
*     Context  -
*
* RESULT
*     Success - Success / Failure flag
*
****************************************************************************/

bool MCI::EnumerateDevices(MCIEnumCB* callback, void* context)
	{
	DWORD count;
	DWORD i;
	char name[64];
	MCIDevice device;

	// Get the number of devices
	count = GetDeviceCount();

	// Do for each device
	for (i = 1; i <= count; i++)
		{
		GetDeviceName(i, name);
		memset(&device, 0, sizeof(device));

		if (GetDeviceDescription(name, &device))
			{
			if (!callback(&device, context))
				break;
			}
		}

	return true;
	}
#endif // MCIMPEG

