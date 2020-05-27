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

#ifndef _MPGSETTINGS_H_
#define _MPGSETTINGS_H_

#include "function.h"

#ifdef DVD
#include "mci.h"
#include "rawfile.h"

class MPGSettings
	{
	public:
		MPGSettings(const char* deviceName);
		MPGSettings(FileClass& file);
		virtual ~MPGSettings(void);

		void SetDeviceName(const char* device);
		const char* GetDeviceName(void) const
			{return mDeviceName;}
		bool Save(FileClass& file);
		void Dialog(void);

		char* mDeviceName;

		#ifdef MCIMPEG
		MCI mMCI;
		unsigned int mCount;
		MCIDevice* mMCIDevices;
		#endif
	};

#endif // DVD
#endif // _MPGSETTINGS_H_
