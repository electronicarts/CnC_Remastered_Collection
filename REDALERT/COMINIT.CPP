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

//
// If you link with this it will automatically call the COM initialization stuff 
//

#include "cominit.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <windows.h>
#include <objbase.h>
//#include "externs.h"
//#include "text.rh"

//#include "WolDebug.h"

ComInit::ComInit()
{
  //HRESULT hRes = CoInitialize(NULL);
//	if (SUCCEEDED(hRes)==FALSE)
//		exit(0);
	HRESULT hRes = OleInitialize(NULL);
}

ComInit::~ComInit()
{
//  CoUninitialize();
//	debugprint( "pre OleUninitialize\n" );
	OleUninitialize();
//	debugprint( "post OleUninitialize\n" );
}

// Creating this instance will setup all COM stuff & do cleanup on program exit
ComInit  Global_COM_Initializer;

